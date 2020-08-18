

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Info_3762.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../MS/MS.h"
#include "../DL698/EVENT_Record.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"


#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

u8* UART_3762_AFNxFn(u32 AFN,u32 Fn)//查询多网络信息,宽带载波频段序号等(自动暂停和恢复路由);入口: AFN,Fn;返回:数据单元指针
{
	u32 i;
	u8* p8rx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
	#ifndef LINUX
	u32 *p32;
	#else
	ms_Type *ms;//得到ms表地址
	#endif
	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//无数据单元
//上行：
	//多网络节点总数量n BIN 1
	//本节点网络标识号  BIN 3
	//本节点主节点地址  BIN 6
	//邻居节点1网络标识号  BIN 3
	//……
	//邻居节点n网络标识号  BIN 3
	MC(0,(u32)p8rx,16);//主线程可能使用了放置缓冲区
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		if((AFN==0x03)&&(Fn==16))
		{//电科院互换性测试bug,查询宽带载波频段序号失败时返回值直接用设置值
			MR((u32)p8rx,ADDR_F209_9+1,1);//属性 9（宽带载波频段序号）∷ =unsigned
		}
		return p8rx;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程

	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(RS485_4PORT);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8rx,16);//主线程可能使用了放置缓冲区
			if((AFN==0x03)&&(Fn==16))
			{//电科院互换性测试bug,查询宽带载波频段序号失败时返回值直接用设置值
				MR((u32)p8rx,ADDR_F209_9+1,1);//属性 9（宽带载波频段序号）∷ =unsigned
			}
			return p8rx;
		}
	}
	p16timer[0]=6;//6秒
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4)||(UARTCtrlPLC->Task==5))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(UARTCtrlPLC->Task==5)
		{
			if(p16timer[0]==0)
			{
				break;
			}
		}
	}
	
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		Uart_3762_TxData(RS485_4PORT,AFN,Fn,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn			
				if(i==((AFN<<8)|Fn))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					//路由恢复
					Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
					Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
					return p8rx+13;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8rx,16);//主线程可能使用了放置缓冲区
	if((AFN==0x03)&&(Fn==16))
	{//电科院互换性测试bug,查询宽带载波频段序号失败时返回值直接用设置值
		MR((u32)p8rx,ADDR_F209_9+1,1);//属性 9（宽带载波频段序号）∷ =unsigned
	}
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
	return p8rx;
}

void UART_3762_AFN10F21(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//查询网络拓扑信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
	#ifndef LINUX
	u32 *p32;
	#else
	ms_Type *ms;//得到ms表地址
	#endif
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
	//节点总数量	BIN	2
	//节点起始序号	BIN	2
	//本次应答的节点数量n	BIN	1
	//节点1地址	BIN	6
	//节点1网络节点信息	BIN	5
	//……	……	……
	//节点n地址	BIN	6
	//节点n网络节点信息	BIN	5
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,21,3);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1015)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=21
					i=p8rx[13+4];//本次应答的节点数量n	BIN	1
					i*=11;
					i+=5;
				#if ((255*11)+5)>LEN_DATABUFF
					#error "当p8Buff=ADDR_DATABUFF时"
				#endif
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
}

void UART_3762_AFN10F31(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//查询相位信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type *ms;//得到ms表地址
#endif
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
	//节点总数量	BIN	2
	//节点起始序号	BIN	2
	//本次应答的节点数量n	BIN	1
	//节点1地址	BIN	6
	//节点1相位信息	BIN	2
	//……	……	……
	//节点n地址	BIN	6
	//节点n相位信息	BIN	2
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,31,3);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x101F)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=31
					i=p8rx[13+4];//本次应答的节点数量n	BIN	1
					i*=8;
					i+=5;
				#if ((255*8)+5)>LEN_DATABUFF
					#error "当p8Buff=ADDR_DATABUFF时"
				#endif
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
}

void UART_3762_AFN10F112(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
//节点总数量 BIN 2
//节点起始序号 BIN 2
//本次应答的节点数量n BIN 1
//节点1地址 BIN 6
//节点1设备类型 BIN 1
//节点1芯片ID信息 BIN 24
//节点1芯片软件版本信息 BCD 2
//...
//节点n地址 BIN 6
//节点n设备类型 BIN 1
//节点n芯片ID信息 BIN 24
//节点n芯片软件版本信息 BCD 2
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,112,3);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1070)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=112
					i=p8rx[13+4];//本次应答的节点数量n	BIN	1
					i*=33;
					i+=5;
					//#error "当p8Buff=ADDR_DATABUFF时"
					if(((u32)p8Buff>=ADDR_DATABUFF)&&((u32)p8Buff<(ADDR_DATABUFF+LEN_DATABUFF)))
					{
						if(i>((ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff))
						{
							i=(ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff;
						}
					}
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
}


void UART_3762_AFN10F104(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
//节点总数量 BIN 2
//节点起始序号 BIN 2
//本次应答的节点数量n BIN 1
//节点1地址 BIN 6
//节点1设备类型 BIN 1
//节点1芯片ID信息 BIN 24
//节点1芯片软件版本信息 BCD 2
//...
//节点n地址 BIN 6
//节点n设备类型 BIN 1
//节点n芯片ID信息 BIN 24
//节点n芯片软件版本信息 BCD 2
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,104,3);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1068)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=104
					i=p8rx[13+2];//本次应答的节点数量n	BIN	1
					i*=15;
					i+=3;
					//#error "当p8Buff=ADDR_DATABUFF时"
					if(((u32)p8Buff>=ADDR_DATABUFF)&&((u32)p8Buff<(ADDR_DATABUFF+LEN_DATABUFF)))
					{
						if(i>((ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff))
						{
							i=(ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff;
						}
					}
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
}

void UART_3762_AFN5F16(u32 PORTn,u8* data)//网络拓扑识别开关 0：关闭 1：打开
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=data[0];	// 设置HPLC通信频段
		Uart_3762_TxData(RS485_4PORT,0x05,16,1);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)//确认帧
				{
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
}



void UART_3762_AFN5F200(u32 PORTn,u8* data)//拒绝节点上报使能标志：禁止（0），使能（1）
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=10;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		ms->link_printf("A-%x\n",ReNum,0,0);
		p8tx[128]=data[0];	// 允许/禁止拒绝节点信息上报
		Uart_3762_TxData(RS485_4PORT,0x05,200,1);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)//确认帧
				{
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
}
void UART_3762_AFN5F6(u32 PORTn,u32 EnableFlag)//网络拓扑识别开关 0：关闭 1：打开
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		if((EnableFlag&0x01) == 1)
			p8tx[128]=0x01;	// 使能台区识别
		else
			p8tx[128]=0;	// 关闭台区识别
		Uart_3762_TxData(RS485_4PORT,0x05,6,1);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)//确认帧
				{
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
}

void UART_3762_AFN10F7(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
{
	u32 i;
	u8  n;
	u16 Len;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
//节点总数量 BIN 2
//本次应答的节点数量n BIN 1

//从节点1地址	BCD 		     6 
//从节点1节点类型BS				 1 
//从节点1模块厂商代码ASCII 			 2 
//从节点1节点模块ID号长度BIN			 1 
//从节点1节点模块ID号格式BIN			 1 
//从节点1节点模块ID号				 50
//...
//从节点N地址	BCD 		     6 
//从节点N节点类型BS				 1 
//从节点N模块厂商代码ASCII 			 2 
//从节点N节点模块ID号长度BIN			 1 
//从节点N节点模块ID号格式BIN			 1 
//从节点N节点模块ID号				 50

	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,7,3);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1007)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=7
					n=p8rx[13+2];//本次应答的节点数量n	BIN	1 变长数据计算用
					i=0;
					i+=3;
					while(n)
					{
						Len=p8rx[i+13+9];
						i+=(Len+11);
						n-=1;
					}
					//#error "当p8Buff=ADDR_DATABUFF时"
					if(((u32)p8Buff>=ADDR_DATABUFF)&&((u32)p8Buff<(ADDR_DATABUFF+LEN_DATABUFF)))
					{
						if(i>((ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff))
						{
							i=(ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff;
						}
					}
					MR((u32)p8Buff,(u32)(p8rx+13),i);
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
}


void READ_AFN3F12_TO_ADDR_AFN3F12(void)//查询本地主节点通信模块ID信息
{
	u32 n;	
	u8* p8;
	u8* p8s;
	u32 offset;
	u8* p8sAFN3F12;
	u8	pOccurSour[5];		  // 存放事件发生源    OAD
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN3F12******************\n", 0, 0, 0);
	p8=UART_3762_AFNxFn(0x03,12);//查询本地主节点通信模块 ID 信息
	//模块厂商代码 ASCII 2
	//模块 ID 长度 BIN 1
	//模块 ID 格式 BIN 1
	//模块 ID 对应模块 ID 格式 M
	n=p8[2];
	p8sAFN3F12=(u8*)ADDR_AFN3F12;
	if(IsAllAByte(p8sAFN3F12, 0, 4)==0)
	{
		if(Compare_DL698DataString(p8sAFN3F12+4, p8+4, n))//如果获取的本地主节点通信模块ID信息改变了，触发模块ID号变更事件中的载波模块变更
		{
			p8s=((u8*)ADDR_DATABUFF + 3072);
			//模块对应的通信地址 octet-string
			offset = 0;
			p8s[offset]=DataType_octet_string;
			p8s[offset+1]=6;
			MR((u32)p8s+offset+2,ADDR_4001+2,6);

			offset+=8;
			//变更前的模块描述符 visible-string， 
			p8s[offset++]=DataType_visible_string;
			p8s[offset++]= (105 + 2*n); //128 字节模块ID信息
			
			//	从节点描述符	  visible-string
			//type=
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x79;
			p8s[offset++] = 0x70;
			p8s[offset++] = 0x65;
			p8s[offset++] = 0x3d;
	
			//模块类型 PLC写死
			p8s[offset++] = 0x50;
			p8s[offset++] = 0x4c;
			p8s[offset++] = 0x43;
	
			// ;
			p8s[offset++] = 0x3b;
	
			//mfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
			
			// 芯片厂商代码
			p8s[offset++] = p8sAFN3F12[1];
			p8s[offset++] = p8sAFN3F12[0];
	
			//;
			p8s[offset++] = 0x3b; 
			
			//idformat=
			p8s[offset++] = 0x69; 
			p8s[offset++] = 0x64; 
			p8s[offset++] = 0x66; 
			p8s[offset++] = 0x6f; 
			p8s[offset++] = 0x72; 
			p8s[offset++] = 0x6d; 
			p8s[offset++] = 0x61; 
			p8s[offset++] = 0x74; 
			p8s[offset++] = 0x3d; 
	
			// bcd bin ascll 
			p8s[offset++] = 0x32;
	
			// ；
			p8s[offset++] = 0x3b;
	
			// id =
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			Data_Inverse((u32)ADDR_AFN10F112_old+9,24);
			HexToASCII((u8*)ADDR_AFN10F112_old+9,&p8s[offset],24);
			Data_Inverse((u32)ADDR_AFN10F112_old+9,24); // RAM数据需要归位
			offset+=48;
			
			// ；
			p8s[offset++] = 0x3b;	
	
			// mmfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
	
			//	模块厂商代码
			p8s[offset++] = p8sAFN3F12[1];
			p8s[offset++] = p8sAFN3F12[0];
	
			// ；
			p8s[offset++] = 0x3b;
	
			// midformat=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x6f;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x61;
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x3d;
	
			// 02
			p8s[offset++] = 0x31;
	
			// ;
			p8s[offset++] = 0x3b;
			
			//mid =
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			// 模块ID信息		
			Data_Inverse((u32)p8sAFN3F12+4,n);
			HexToASCII(p8sAFN3F12+4,&p8s[offset],n);
			Data_Inverse((u32)p8sAFN3F12+4,n);	// RAM数据需要归位
			offset+=(2*n);
	
	//变更后的模块描述符 visible-string
			p8s[offset++]=DataType_visible_string;
			p8s[offset++]= (105 + 2*n); //128 字节模块ID信息
	
			
			//	从节点描述符	  visible-string
			//type=
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x79;
			p8s[offset++] = 0x70;
			p8s[offset++] = 0x65;
			p8s[offset++] = 0x3d;
	
			//模块类型 PLC写死
			p8s[offset++] = 0x50;
			p8s[offset++] = 0x4c;
			p8s[offset++] = 0x43;
	
			// ;
			p8s[offset++] = 0x3b;
	
			//mfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
			
			// 芯片厂商代码
			p8s[offset++] = p8[1];
			p8s[offset++] = p8[0];
	
			//;
			p8s[offset++] = 0x3b; 
			
			//idformat=
			p8s[offset++] = 0x69; 
			p8s[offset++] = 0x64; 
			p8s[offset++] = 0x66; 
			p8s[offset++] = 0x6f; 
			p8s[offset++] = 0x72; 
			p8s[offset++] = 0x6d; 
			p8s[offset++] = 0x61; 
			p8s[offset++] = 0x74; 
			p8s[offset++] = 0x3d; 
	
			// bcd bin ascll 
			p8s[offset++] = 0x32;
	
			// ；
			p8s[offset++] = 0x3b;
	
			// id =
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			Data_Inverse((u32)ADDR_AFN10F112+9,24);
			HexToASCII((u8*)ADDR_AFN10F112+9,&p8s[offset],24);
			Data_Inverse((u32)ADDR_AFN10F112+9,24); // RAM数据需要归位
			offset+=48;
			
			// ；
			p8s[offset++] = 0x3b;	
	
			// mmfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
	
			//	模块厂商代码
			p8s[offset++] = p8[1];
			p8s[offset++] = p8[0];
	
			// ；
			p8s[offset++] = 0x3b;
	
			// midformat=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x6f;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x61;
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x3d;
	
			// 02
			p8s[offset++] = 0x31;
	
			// ;
			p8s[offset++] = 0x3b;
			
			//mid =
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			// 模块ID信息		
			Data_Inverse((u32)p8+4,n);
			HexToASCII(p8+4,&p8s[offset],n);
			Data_Inverse((u32)p8+4,n);	// RAM数据需要归位
			offset+=(2*n);
			
			pOccurSour[0]=DataType_OAD;
			pOccurSour[1]=0xF2;
			pOccurSour[2]=0x09;
			pOccurSour[3]=0x02;
			pOccurSour[4]=0x01;
			Event_30300200(p8s,pOccurSour,offset);//跨台区电能表事件记录;入口:pDATA=一个跨台区结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			ms->link_printf("\n********************************************\n", 0, 0, 0);
			ms->link_printf("***************CcoIdChang***********\n", 0, 0, 0);
			ms->link_printf("**********************************************\n", 0, 0, 0);
		}
	}
	MR(ADDR_AFN3F12,(u32)p8,4+n);
}
void READ_AFN10F21_TO_ADDR_AFN10F21(void)//查询全部网络拓扑信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F21
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F21******************\n", 0, 0, 0);
//2byte 节点总数量
//6byte 节点1地址
//5byte 节点1网络拓扑信息
//...
//6byte 节点n地址
//5byte 节点n网络拓扑信息
	n=0;
	p8d=(u8*)ADDR_AFN10F21+2;
	while(1)
	{
		UART_3762_AFN10F21(ACSAMPLEPORT,n+1,32,(u8*)ADDR_DATABUFF);//查询网络拓扑信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
		p8s=(u8*)ADDR_DATABUFF;
	//上行：
		//节点总数量	BIN	2
		//节点起始序号	BIN	2
		//本次应答的节点数量n	BIN	1
		//节点1地址	BIN	6
		//节点1网络节点信息	BIN	5
		//……	……	……
		//节点n地址	BIN	6
		//节点n网络节点信息	BIN	5
		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		m=p8s[4];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		p8s+=5;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(11*n),(u32)p8s,11*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F21;
	p8d[0]=n;
	p8d[1]=n>>8;
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
}

void READ_AFN10F31_TO_ADDR_AFN10F31(void)//查询相位信息
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;
	
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F31******************\n", 0, 0, 0);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
	//节点总数量	BIN	2
	//节点起始序号	BIN	2
	//本次应答的节点数量n	BIN	1
	//节点1地址	BIN	6
	//节点1相位信息	BIN	2
	//……	……	……
	//节点n地址	BIN	6
	//节点n相位信息	BIN	2
	n=0;
	p8d=(u8*)ADDR_AFN10F31+2;
	while(1)
	{
		UART_3762_AFN10F31(ACSAMPLEPORT,n+1,15,(u8*)ADDR_DATABUFF);//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
		p8s=(u8*)ADDR_DATABUFF;

		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		m=p8s[4];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		p8s+=5;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(8*n),(u32)p8s,8*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F31;
	p8d[0]=n;
	p8d[1]=n>>8;
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
}


void READ_AFN10F111_TO_ADDR_AFN10F111(void)//查询全部多网络信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F111
{
	u32 n;
	u8* p8s;
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F101******************\n", 0, 0, 0);
	p8s=UART_3762_AFNxFn(0x10,111);//查询多网络信息,宽带载波频段序号等(自动暂停和恢复路由);入口: AFN,Fn;返回:数据单元指针
	//多网络节点总数量n BIN 1
	//本节点网络标识号  BIN 3
	//本节点主节点地址  BIN 6
	//邻居节点1网络标识号  BIN 3
	//……
	//邻居节点n网络标识号  BIN 3
	n=p8s[0];
	n*=3;
	n+=10;
	MR(ADDR_AFN10F111,(u32)p8s,n);
}

void READ_AFN10F112_TO_ADDR_AFN10F112(void)//查询全部宽带载波芯片信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F112
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F112******************\n", 0, 0, 0);

//节点总数量 BIN 2
//节点1地址 BIN 6
//节点1设备类型 BIN 1
//节点1芯片ID信息 BIN 24
//节点1芯片软件版本信息 BIN 2
//...
//节点n地址 BIN 6
//节点n设备类型 BIN 1
//节点n芯片ID信息 BIN 24
//节点n芯片软件版本信息 BIN 2
	n=0;
	p8d=(u8*)ADDR_AFN10F112+2;
	MR(ADDR_AFN10F112_old,ADDR_AFN10F112, 2 + (33 * NUM_RMmax));
	while(1)
	{
		UART_3762_AFN10F112(ACSAMPLEPORT,n+1,32,(u8*)ADDR_DATABUFF);//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
		p8s=(u8*)ADDR_DATABUFF;
	//上行：
		//节点总数量 BIN 2
		//节点起始序号 BIN 2
		//本次应答的节点数量n BIN 1
		//节点1地址 BIN 6
		//节点1设备类型 BIN 1
		//节点1芯片ID信息 BIN 24
		//节点1芯片软件版本信息 BCD 2
		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		m=p8s[4];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		p8s+=5;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(33*n),(u32)p8s,33*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F112;
	p8d[0]=n;
	p8d[1]=n>>8;
	
	myprintf("======================ADDR_AFN10F112-S==========\n",0,0,0);
	for(n=0;n<100;n++)
		myprintf("%02x ", p8d[n], 0, 0);
	myprintf("\n",0,0,0);
	myprintf("======================ADDR_AFN10F112-E==========\n",0,0,0);
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
}

void READ_AFN10F104_TO_ADDR_AFN10F104(void)//查询全部宽带载波芯片信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F112
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;

	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F104******************\n", 0, 0, 0);

	//节点总数量 BIN 2
	//节点1地址 BIN 6
	//节点1软件版本 BIN 2
	//节点1软件版本日期 BIN 3
	//节点1模块厂商代码 BIN 2
	//节点1芯片代码 BIN 2
	//...
	//节点n地址 BIN 6
	//节点n软件版本 BIN 2
	//节点n软件版本日期 BIN 3
	//节点n模块厂商代码 BIN 2
	//节点n芯片代码 BIN 2

	n=0;
	
	MR(ADDR_AFN10F104_old, ADDR_AFN10F104,2 + (15 * NUM_RMmax));	// 拷贝升级前的模块ID信息，用于事件属性
	p8d=(u8*)ADDR_AFN10F104 + 2;
	while(1)
	{
		UART_3762_AFN10F104(ACSAMPLEPORT,n+1,15,(u8*)ADDR_DATABUFF);//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
		p8s=(u8*)ADDR_DATABUFF;
		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		m=p8s[2];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		p8s+=3;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(15*n),(u32)p8s,15*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F104;
	p8d[0]=n;
	p8d[1]=n>>8;
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
}

void READ_AFN10F7_TO_ADDR_AFN10F7(void)//查询全部宽带载波芯片信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F112
{
	u32 n;
	u32 m;
	u32 M;
	u32 k;
	u32 w,x,y,z;
	u32 Minus;
	u32 NUMmax;
	u32 addr;
	u32 Len;
	u32 i,j;
	u32 offset;
	u8	pOccurSour[5];		  // 存放事件发生源    OAD
	u8* p8s;
	u8* p8s1;
	u8* p8s2;
	u8* p8s3;
	u8* p8s4;
	u8* p8s5;
	u8* p8d;
	ms_Type *ms;
	ms=Get_ms();

	
	ms->link_printf("\n*****************AFN10F7******************\n", 0, 0, 0);

	//节点总数量 BIN					 2
	
	//从节点1地址  BCD 				 6 
	//从节点1节点类型BS  				 1 
	//从节点1模块厂商代码ASCII  			 2 
	//从节点1节点模块ID号长度BIN  		 1 
	//从节点1节点模块ID号格式BIN       	 1 
	//从节点1节点模块ID号				 50

	//...
	//从节点N地址  BCD 				 6 
	//从节点N节点类型BS  				 1 
	//从节点N模块厂商代码ASCII  			 2 
	//从节点N节点模块ID号长度BIN  		 1 
	//从节点N节点模块ID号格式BIN       	 1 
	//从节点N节点模块ID号				 50
	
	n=0;
	p8d=(u8*)ADDR_AFN10F7 + 2;
	MR(ADDR_AFN10F7_old,ADDR_AFN10F7,2 + (61 * NUM_RMmax));
	p8s1=(u8*)ADDR_AFN10F7_old + 2;
	while(1)
	{
		MR(ADDR_DATABUFF + 1024 + 512,(ADDR_AFN10F7 + 2 + n* 61),  15 * 61);	// 拷贝升级前的模块ID信息，用于事件属性
		UART_3762_AFN10F7(ACSAMPLEPORT,n+1,15,(u8*)ADDR_DATABUFF);//查询宽带载波芯片信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
		p8s=(u8*)ADDR_DATABUFF;
	//上行：
		//节点总数量 BIN					 2
		//本次应答的数量					 1
		//从节点1地址	BCD 			 6 
		//从节点1节点类型BS				 1 
		//从节点1模块厂商代码ASCII 			 2 
		//从节点1节点模块ID号长度BIN			 1 
		//从节点1节点模块ID号格式BIN			 1 
		//从节点1节点模块ID号				 M(<=50)

		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		m=p8s[2];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		p8s+=3;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}

		// 第一段
		k = m;
		addr=0;
		if( k == 1)
		{
			MR((u32)p8d+(61*n),(u32)p8s,61);	// 从节点开始 
		}
		else
		{
			while(k)
			{
				Len=p8s[addr+9];
				Minus = (m-k);
				MR((u32)(p8d+(61*(n + Minus))),(u32)(p8s + addr),61);
				addr+=(Len+11);
				k-=1;
				if(k < 1)
				{
					break;
				}
			} 
		}
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}

#if 1		
	// 数据初始化第一次 不应该产生模块ID变更事件
	if(IsAllAByte(p8s1, 0, 9) == 0)
	{
		// 产生模块ID变更事件
		for(i=0; i<n; i++)
		{
			// 变更前模块ID动态变长长度
			M = p8s1[(61 * i) + 9];
			if(M>50)
			{
				continue;
			}

			// 没有抄到模块ID信息数据，也不产生事件
			if(IsAllAByte(p8s1 + 61 * i + 11, 0, M) == 1)
			{
				continue;
			}

			// 没有抄到模块ID信息数据，也不产生事件
			if(IsAllAByte(p8d + 61 * i + 11, 0, M) == 1)
			{
				continue;
			}

			// 地址全0跳过
			if(IsAllAByte(p8s1 + (61 * i), 0, 6) == 1)
			{
				continue;
			}

			// 地址全0跳过
			if(IsAllAByte(p8d + (61 * i), 0, 6) == 1)
			{
				continue;
			}
#if 1		
			// 终端里需有变更前该电表档案。
			for(j=0;j<NUM_RMmax;j++)
			{
				if(Compare_DL698DataString(((u8*)ADDR_NoteList+(j * LEN_NoteList)),p8s1 + (61 * i), 6) == 0)
				{
					break;
				}
			}
			if(j == NUM_RMmax)
			{
				continue;
			}	

			// 终端里需有变更后该电表档案。
			for(j=0;j<NUM_RMmax;j++)
			{
				if(Compare_DL698DataString(((u8*)ADDR_NoteList+(j * LEN_NoteList)),p8d + (61 * i), 6) == 0)
				{
					break;
				}
			}
			if(j == NUM_RMmax)
			{
				continue;
			}
#endif

			// 找到变更前电表地址 对应的模块ID存储位置 
			for(x=0;x<NUM_RMmax;x++)
			{
				if(Compare_DL698DataString(p8d + 61 * i ,p8s1+61*x, 6) == 0)
				{
					break;
				}
			}

			if(x == NUM_RMmax)
			{
				continue;
			}
			
			// 模块ID变更条件
			if(Compare_DL698DataString(p8d + (61 * i), p8s1 + (61 * x), 11 + M))
			{
				//模块对应的通信地址 octet-string
				p8s=(u8*)ADDR_DATABUFF+1024;
				offset = 0;
				p8s[offset]=DataType_octet_string;
				p8s[offset+1]=6;
				MR((u32)p8s+offset+2,(u32)p8d+(61 * i),6);
				p8s2=(u8*)ADDR_AFN10F112;
				p8s3= (u8*)ADDR_AFN10F104_old;
				p8s4=(u8*)ADDR_AFN10F112_old;
				p8s5= (u8*)ADDR_AFN10F104;
				for(k=0;k<NUM_RMmax;k++)
				{
					if(Compare_DL698DataString(p8s2 + 2 + 33 * k ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}
				for(z=0;z<NUM_RMmax;z++)
				{
					if(Compare_DL698DataString(p8s4 + 2 + 33 * z ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}
				
				for(y=0;y<NUM_RMmax;y++)
				{
					if(Compare_DL698DataString(p8s3 + 2 + 15 * y ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}

				for(w=0;w<NUM_RMmax;w++)
				{
					if(Compare_DL698DataString(p8s5 + 2 + 15 * w ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}

				
				Data_Inverse((u32)p8s+offset+2,6);//数据倒序(高低字节调换),入口Le>=2
				offset+=8;
	
				//变更前的模块描述符 visible-string， 
				p8s[offset++]=DataType_visible_string;
				p8s[offset++]= (103 + 2*M); //128 字节模块ID信息
				
				//	从节点描述符	  visible-string
				//type=
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x79;
				p8s[offset++] = 0x70;
				p8s[offset++] = 0x65;
				p8s[offset++] = 0x3d;
	
				
				//HexToASCII(p8s4 + (33 * z) + 6 + 2,&p8s[offset],1);
				p8s[offset++] = p8s4[(33 * z) + 6 + 2] + 0x30;
	
				// ;
				p8s[offset++] = 0x3b;
	
				//mfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
				
				// 芯片厂商代码
				p8s[offset++] = p8s3[14+ 2 + 15*y];
				p8s[offset++] = p8s3[13+ 2 + 15*y];
	
				//;
				p8s[offset++] = 0x3b; 
				
				//idformat=
				p8s[offset++] = 0x69; 
				p8s[offset++] = 0x64; 
				p8s[offset++] = 0x66; 
				p8s[offset++] = 0x6f; 
				p8s[offset++] = 0x72; 
				p8s[offset++] = 0x6d; 
				p8s[offset++] = 0x61; 
				p8s[offset++] = 0x74; 
				p8s[offset++] = 0x3d; 
	
				// bcd bin ascll 
				p8s[offset++] = 0x32;
	
				// ；
				p8s[offset++] = 0x3b;
	
				// id =
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;

				Data_Inverse((u32)p8s4 + (33 * z) + 7 + 2,24);
				HexToASCII(p8s4 + (33 * z) + 7 + 2,&p8s[offset],24);
				Data_Inverse((u32)p8s4 + (33 * z) + 7 + 2,24);	// RAM数据需要归位
				offset+=48;
				
				// ；
				p8s[offset++] = 0x3b;	
	
				// mmfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
	
				//	模块厂商代码
				p8s[offset++]= p8s3[12+ 2 + 15*y];
				p8s[offset++]= p8s3[11+ 2 + 15*y];
	
				// ；
				p8s[offset++] = 0x3b;
	
				// midformat=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x6f;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x61;
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x3d;
	
				// 02
				p8s[offset++] = 0x31;
	
				// ;
				p8s[offset++] = 0x3b;
				
				//mid =
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;
	
				// 模块ID信息		
				Data_Inverse((u32)p8s1+11+x*61,M);
				HexToASCII(&p8s1[11+x*61],&p8s[offset],M);
				Data_Inverse((u32)p8s1+11+x*61,M);	// RAM数据需要归位
				offset+=(2*M);
	
		//变更后的模块描述符 visible-string
				// 模块ID动态变长长度
				M = p8d[(61 * i) + 9];
				if(M>50)
				{
					M=50;
				}
				p8s[offset++]=DataType_visible_string;
				p8s[offset++]= (103 + 2*M); //128 字节模块ID信息
	
				
				//128 字节模块ID信息
				//128 字节模块ID信息
				//type=
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x79;
				p8s[offset++] = 0x70;
				p8s[offset++] = 0x65;
				p8s[offset++] = 0x3d;
	
				//HexToASCII(&p8s2[2] + (33 * k) + 6,&p8s[offset],1);
				//offset+=2;
				p8s[offset++] = p8s2[2 + (33 * k) + 6]+0x30;
	
				// ;
				p8s[offset++] = 0x3b;
	
				//mfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
				
				// 芯片厂商代码
				p8s[offset++] = p8s5[14+ 2 + 15*w];
				p8s[offset++] = p8s5[13+ 2 + 15*w];

				//;
				p8s[offset++] = 0x3b; 
				
				//idformat=
				p8s[offset++] = 0x69; 
				p8s[offset++] = 0x64; 
				p8s[offset++] = 0x66; 
				p8s[offset++] = 0x6f; 
				p8s[offset++] = 0x72; 
				p8s[offset++] = 0x6d; 
				p8s[offset++] = 0x61; 
				p8s[offset++] = 0x74; 
				p8s[offset++] = 0x3d; 
	
				// bcd bin ascll
				p8s[offset++] = 0x32;
	
				// ；
				p8s[offset++] = 0x3b;
	
				// id =
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;
				Data_Inverse((u32)p8s2 + 2 + (33 * k) + 7,24);
				HexToASCII(p8s2 + 2 + (33 * k) + 7,&p8s[offset],24);
				Data_Inverse((u32)p8s2 + 2 + (33 * k) + 7,24);
				
				offset+=48;
				
				// ；
				p8s[offset++] = 0x3b;	
	
				// mmfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
	
				//	模块厂商代码
				p8s[offset++]= p8s5[12+ 2 + 15*w];
				p8s[offset++]= p8s5[11+ 2 + 15*w];

	
				// ；
				p8s[offset++] = 0x3b;
	
				// midformat=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x6f;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x61;
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x3d;
	
				// 02
				p8s[offset++] = 0x31;
	
				// ;
				p8s[offset++] = 0x3b;
				
				//mid =
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;
	
				// 模块ID信息		
				Data_Inverse((u32)p8d+11+i*61,M);
				HexToASCII(&p8d[11+i*61],&p8s[offset],M);
				Data_Inverse((u32)p8d+11+i*61,M);
				offset+=(2*M);
				pOccurSour[0]=DataType_OAD;
				pOccurSour[1]=0xF2;
				pOccurSour[2]=0x09;
				pOccurSour[3]=0x05;
				pOccurSour[4]=0x00;
				Event_30300200(p8s,pOccurSour,offset);//跨台区电能表事件记录;入口:pDATA=一个跨台区结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				ms->link_printf("\n********************************************\n", 0, 0, 0);
				ms->link_printf("*******************NetIDChang*****************\n", 0, 0, 0);
			}
		}
	}
#endif

	p8d=(u8*)ADDR_AFN10F7;
	p8d[0]=n;
	p8d[1]=n>>8;
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
}

void READ_Info(void)//宽带载波按F209_6设置的TI周期读网络拓扑信息,多网络信息等(不需要周期读取时可设置读取的TI周期=0)
{
	u32 i;
	u32 t;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	ms_Type *ms;
	ms=Get_ms();
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	if(UARTCtrl->Lock_InfoTask==0x55)//读路由网络拓扑信息等任务:0x55=锁定
	{
		return;
	}
	if(Terminal_Router->RouterRunMode_1!=2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{//非宽带电力线载波
		return;
	}
	if((Terminal_Router->RUNState&1)==0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
	{//非路由完成标志
		return;
	}
	MR(ADDR_DATABUFF,ADDR_F209_6+1,3);//更新周期：指从节点对象列表、网络拓扑信息、多网信息等更新周期
	t=pTItoSECOND((u8 *)ADDR_DATABUFF);//TI转为秒值,入口:pTI无数据类型,出口:hex秒
	if(t==0)
	{//周期TI没设置,或=0
		return;
	}
	i=YYMDHMS_Sub_hex((u8*)ADDR_Read_Info_Time,(u8*)ADDR_DL698YMDHMS);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
	if(i==0)
	{
		p8=(u8*)ADDR_Read_Info_Time;
		if(p8[2]|p8[3])
		{//上次计算的时钟非全部 0
			return;
		}
	}
	
	if((i==0)||(i>=t))
	{
		ms->link_printf("======================周期更新开始==========\n",0,0,0);
		MW(ADDR_DL698YMDHMS,ADDR_Read_Info_Time,7);
		UARTCtrl->Lock_InfoTask=0x55;//读路由网络拓扑信息等任务:0x55=锁定
		READ_AFN10F21_TO_ADDR_AFN10F21();//查询全部网络拓扑信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F21
		READ_AFN10F111_TO_ADDR_AFN10F111();//查询全部多网络信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F111
		READ_AFN10F112_TO_ADDR_AFN10F112();//查询全部宽带载波芯片信息(自动暂停和恢复路由);返回:数据在ERAM的ADDR_AFN10F112
		READ_AFN10F104_TO_ADDR_AFN10F104();//查询从节点非芯片信息
		READ_AFN10F31_TO_ADDR_AFN10F31();//查询相位信息
		READ_AFN3F12_TO_ADDR_AFN3F12();//查询本地主节点通信模块ID信息
		READ_AFN10F7_TO_ADDR_AFN10F7();//查询从节点ID号信息,必须在F10F112之后
		ms->link_printf("======================周期更新结束==========\n",0,0,0);
		UARTCtrl->Lock_InfoTask=0;//读路由网络拓扑信息等任务:0x55=锁定
	}
}


void UART_3762_AFN03F101(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//(河南)查询节点台区信息(只暂停路由,没恢复);入口:StartNo=节点起始序号首帧从0开始,Num=节点数量(最大支持15个),p8Buff=数据放置缓冲
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//下行：
	//节点起始序号	BIN	2
	//节点数量n	BIN	1
//上行：
	//台区节点总数量	BIN	2
	//本次上报节点数量n(n<=15)	BIN	1
	//节点1地址	BCD	6
	//b7	b6	b5-3	b2-1	b0	BS	1
	//保留	线路异常	相序	台区状态	表档案		
	//节点1台区地址	BCD	6
	//…	…	…
	//a)	表档案：0表示不是集中器下发的表档案，1表示是集中器下发的表档案。
	//b)	台区状态：0表示节点台区地址不确定；1表示节点台区地址和本集中器相同，属于本台区；2表示节点台区地址和本集中器地址不相同；
	//c)	相序：节点所在电源相别，置0表示不确定；置1依次表示第1、2、3相（001代表A相；010代表B相；100该表C相；111代表三相）；
	//d)	线路异常：从节点线路异常标记：置“1”时，表示从节点线路有异常。当电能表为单相供电时，线路异常表示零火线反接；当电能表为三相供电时，线路异常表示逆相序。
	//e)	节点台区地址：节点地址归属的台区地址（包括跨台区地址），不确定全填F；
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		return;
	}
	UARTCtrlPLC->InfoTask=1;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16timer[0]=60;//60秒
	while(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
			MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//暂停路由
//Terminal_Router->StopFlags;//暂停路由标志:(字对齐)
	//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//暂停路由标志
		Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
		while(Terminal_Router->RouterCtrl)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
		}
	}
//读信息
	UARTCtrlPLC->InfoTask=2;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	ReNum=3;//重复次数
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x03,101,3);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//否认回答
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0365)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=101
					i=p8rx[13+2];//本次应答的节点数量n	BIN	1
					i*=13;
					i+=3;
				#if ((255*13)+3)>LEN_DATABUFF
					#error "当p8Buff=ADDR_DATABUFF时 "
				#endif
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
					return;
				}
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//超时定时
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=正在接收
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	UARTCtrlPLC->InfoTask=0;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	MC(0,(u32)p8Buff,5);//主线程可能使用了放置缓冲区
}


#endif










