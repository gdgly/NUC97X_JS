
//uart类1数据
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "Terminal_ReadMeter_3762.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_AFN0C_RealTime.h"
#include "Terminal_Uart_TimerTask.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_SaveFnData.h"



extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];

void Uart_AFN0C(u32 PORTn)//类1数据
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Fn;
	u32 Pn;
	u32 DIlen;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
  u32 LEN_UARTnTx;
	u32 SourDI;//本次分解前的DI
	u32 ALLPn;//全部有效量点标志，0=否,1=是
	u32 Tariff;

	u8 * p8s;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 * p32;
	UARTCtrl_TypeDef * UARTCtrl;	
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	u32 Addr;
	UARTCtrl_TypeDef * UARTCtrlRouter;
#endif
#if (((USER/100)<=1)||(ReadF129Above==1))
	u16 *p16;
#endif
#if (((USER/100)==13)||((USER/100)==11))//黑龙江  吉林 
	u64 YMDHMS;
#endif


	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN(为定时上报帧填写）
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			if(DIlen<(12+6))
			{
	err:
				//全部否认
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
				Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
			if(i!=0x0)
			{
				return;//舍弃该报文
			}
		}
	}

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
		if(DIlen<4)
		{
			goto err;
		}
	}
	else
	{
		DIlen=UARTCtrl->DIlen;//数据标识长度寄存器
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		UARTCtrl->DIOffset=0;//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
		CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
	}
	RXDIaddr+=UARTCtrl->DIOffset;
	RXaddr=RXDIaddr+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=TXDIaddr+4;
	TXlen=0;
	f=0;//有无有效数据填入标志;0=无,1=有
	MC(0,TXDIaddr,4);
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//本数据标识为全部有效测量点
		ALLPn=0;//全部有效量点标志，0=否,1=是
	}
	else
	{
		ALLPn=1;//全部有效量点标志，0=否,1=是
	}
	while(DIlen>=4)
	{
		SourDI=MRR((u32)(&UARTCtrl->DestDI),4);//本次分解前的DI
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			RXDIaddr=RXaddr;
			UARTCtrl->DIOffset=RXDIaddr-(Get_ADDR_UARTnRx(PORTn)+14);//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
			CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//本数据标识为全部有效测量点
				ALLPn=0;//全部有效量点标志，0=否,1=是
			}
			else
			{
				ALLPn=1;//全部有效量点标志，0=否,1=是
			}
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
			}
			MC(0,TXDIaddr,4);
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;

		if(ALLPn!=0x0)
		{//本数据标识为全部有效测量点,发送时数据标识不进行组合
			i=Terminal_Class1DataLib[Fn].PnMax;
			if(Fn==246)
			{//当前掉电记录数据(Pn映射)
				i=MaxRS485AddCarrierPn;
			}
		#if AFN0CF33MaxPn==0
			//以下Fn可由其他分项合成
			switch(Fn)
			{
				case 33://F33测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
				case 34://F34测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
				case 35://F35测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				case 36://F36测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				case 37://F37上月（上一结算日）测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
				case 38://F38上月（上一结算日）测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12
				case 39://F39上月（上一结算日）测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				case 40://F40上月（上一结算日）测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12
					i=AFN0CF129MaxPn;
			}
		#endif
			if(Pn>i)
			{//全部Pn完
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			switch(Terminal_Class1DataLib[Fn].PnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
				//case p0Type://终端信息点
				//	break;
				case pnType://测量点号
					if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
				case GnType://总加组号
					if(GroupTotalSet(Pn)==0)//有效总加组号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
				//case CnType://电容器测量点号
				//case DnType://直流模拟量号
				//case RnType://控制轮次号
				//case TnType://任务号
				//case EnType;//事件号
				//	break;
			}
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
				MC(0,TXDIaddr,4);
			}
		}
		else
		{
			switch(Terminal_Class1DataLib[Fn].PnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
				//case p0Type://终端信息点
				//	break;
				case pnType://测量点号
					if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
				case GnType://总加组号
					if(GroupTotalSet(Pn)==0)//有效总加组号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
				//case CnType://电容器测量点号
				//case DnType://直流模拟量号
				//case RnType://控制轮次号
				//case TnType://任务号
				//case EnType;//事件号
				//	break;
			}
		}

#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		switch(Fn)
		{
//临时缓冲定义RXBUFF+128开始
//2BYTE Pn
//1BYTE 
//1BYTE 中继路由级数
//90BYTE(6*15) 中继1-15地址
//2BYTE 数据长度(字节)

//1BYTE 填写端口号(31)
//1BYTE 当前已读中继路由计数
//1BYTE 第1个中继路由级数
//6*n   第1个中继中继1-n地址
//...
//1BYTE 第N个中继路由级数
//6*n   第N个中继中继1-n地址
			case 169://集中抄表中继路由信息
				if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
				{//测量点号错
					goto err;
				}
				Addr=ADDR_AFN04F10+2;//找本测量点号的通信地址
				for(i=0;i<AFN04F10MaxPn;i++)
				{
					f=MRR(Addr,2);
					if(f==Pn)
					{
						break;
					}
					Addr+=LEN_AFN04F10_Pn;
				}
				if(i==AFN04F10MaxPn)
				{//没找到
					goto err;
				}
				f=MRR(Addr+2,1);
				f&=0x1f;
				p8rxbuff[128+4+(6*15)+2]=f;//所属终端通信端口号
				if(f!=31)
				{//不是载波端口
					p8rxbuff[128+4+(6*15)+2+1]=0;//中继路由级数=0
					i=((u32)p8rxbuff)+128+4+(6*15)+2;//数据地址
					x=2;//Pn数据长度
					goto Fn169;						
				}
				if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
				{//路由器初始化未完成,或错误等
					goto err;
				}
				
				//读路由信息
				if((Comm_Ram->RouterType==1)||(Comm_Ram->RouterType==5))//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
				{//1=正泰，无路由
					p8rxbuff[128+4+(6*15)+2+1]=0;//中继路由级数=0
					i=((u32)p8rxbuff)+128+4+(6*15)+2;//数据地址
					x=2;//Pn数据长度
					goto Fn169;
				}
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
				UARTCtrlRouter=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);//得到UART控制区地址
				p32=(u32 *)(ADDR_MS_ENTRY);
				while((UARTCtrlRouter->Lock==0x55)||(UARTCtrlRouter->Task!=0))
				{//0x55=通信功能正在被调用禁止其他程序再次调用,0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					(*(void(*)())p32[115])();//I0_Main_Return;63 开环主任务
					if(UARTCtrlRouter->Lock==0x55)
					{
						goto err;
					}
				}
				UARTCtrlRouter->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
				//暂停路由
				Terminal_Router->StopFlags|=1<<3;//暂停路由标志:b0=,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
				Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
				while(Terminal_Router->RouterCtrl!=0)
				{
					(*(void(*)())p32[115])();//I0_Main_Return;63 开环主任务
					if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					{
						UARTCtrlRouter->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						goto err;
					}
				}
				//读指定节点的上一级中继路由信息
				MR(Get_ADDR_UARTnTx(RS485_4PORT)+128,Addr+4,6);
				Uart_3762_TxData(RS485_4PORT,0x10,3,6);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
				while(UARTCtrlRouter->Task!=1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					if(UARTCtrlRouter->Task<=2)
					{//0=空闲,2=接收超时(或错误)
						//恢复路由
						Terminal_Router->StopFlags&=~(1<<3);//暂停路由标志:b0=,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
						Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
						UARTCtrlRouter->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						goto err;
					}
					(*(void(*)())p32[115])();//I0_Main_Return;63 开环主任务
				}
				Addr=Get_ADDR_UARTnRx(RS485_4PORT);
				i=MRR(Addr+10,3);
				if(i!=0x410)
				{
					//恢复路由
					Terminal_Router->StopFlags&=~(1<<3);//暂停路由标志:b0=,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
					Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
					UARTCtrlRouter->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
					goto err;
				}
				else
				{//AFN=10H,Fn=3
					x=MRR(Addr+10+3,1);
					if(x>15)
					{
						x=15;//最大15级
					}
					if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
					{//鼎信返回的中继路由信息包括主节点地址
						if(x!=0)
						{
							x--;//去主节点地址
						}
						p8rxbuff[128+3]=x;//中继路由级数
						for(i=0;i<x;i++)
						{
							MR(((u32)p8rxbuff)+128+4+(6*i),Addr+10+3+1+8+(8*i),6);
						}
						p8rxbuff[128+4+(6*15)]=2;//数据长度(字节)
						p8rxbuff[128+4+(6*15)+1]=0;//数据长度(字节)
						p8rxbuff[128+4+(6*15)+3]=0;//当前已读中继路由计数
					}
					else
					{//
						p8rxbuff[128+3]=x;//中继路由级数
						for(i=0;i<x;i++)
						{
							MR(((u32)p8rxbuff)+128+4+(6*i),Addr+10+3+1+(8*i),6);
						}
						p8rxbuff[128+4+(6*15)]=2;//数据长度(字节)
						p8rxbuff[128+4+(6*15)+1]=0;//数据长度(字节)
						p8rxbuff[128+4+(6*15)+3]=0;//当前已读中继路由计数
					}
				}
				//读各中继信息
				while(p8rxbuff[128+4+(6*15)+3]<p8rxbuff[128+3])
				{//当前已读中继路由计数<中继路由级数
					x=p8rxbuff[128+4+(6*15)+3];//当前已读中继路由计数
					Fn=p8rxbuff[128+4+(6*15)];//数据长度(字节)
					Fn|=p8rxbuff[128+4+(6*15)+1]<<8;//数据长度(字节)
					MR(Get_ADDR_UARTnTx(RS485_4PORT)+128,((u32)p8rxbuff)+128+4+(6*x),6);
					Uart_3762_TxData(RS485_4PORT,0x10,3,6);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
					while(UARTCtrlRouter->Task!=1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						if(UARTCtrlRouter->Task<=2)
						{//0=空闲,2=接收超时(或错误)
							//恢复路由
							Terminal_Router->StopFlags&=~(1<<3);//暂停路由标志:b0=,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
							Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
							UARTCtrlRouter->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
							goto err;
						}
						(*(void(*)())p32[115])();//I0_Main_Return;63 开环主任务
					}
					i=MRR(Addr+10,3);
					if(i!=0x410)
					{
						//恢复路由
						Terminal_Router->StopFlags&=~(1<<3);//暂停路由标志:b0=,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
						Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
						UARTCtrlRouter->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						goto err;
					}
					else
					{//AFN=10H,Fn=3
						Pn=MRR(Addr+10+3,1);
						if(Pn>15)
						{
							Pn=15;//最大15级
						}
						p8rxbuff[128+4+(6*15)+2+Fn]=1+Pn;//中继路由级数
						MR(((u32)p8rxbuff)+128+4+(6*15)+2+Fn+1,((u32)p8rxbuff)+128+4+(6*x),6);
						for(i=0;i<Pn;i++)
						{
							MR(((u32)p8rxbuff)+128+4+(6*15)+2+Fn+1+6+(6*i),Addr+10+3+1+(8*i),6);
						}
						Fn+=1+6+(6*Pn);
						p8rxbuff[128+4+(6*15)]=Fn;//数据长度(字节)
						p8rxbuff[128+4+(6*15)+1]=Fn>>8;//数据长度(字节)
						x++;;//当前已读中继路由计数+1
						p8rxbuff[128+4+(6*15)+3]=x;
					}
				}
				//恢复路由
				Terminal_Router->StopFlags&=~(1<<3);//暂停路由标志:b0=,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
				UARTCtrlRouter->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
				i=((u32)p8rxbuff)+128+4+(6*15)+2;//数据地址
				x=p8rxbuff[128+4+(6*15)];//数据长度(字节)
				x|=p8rxbuff[128+4+(6*15)+1]<<8;//数据长度(字节)
				goto Fn169;
		}
#endif

		i=GetClass1DataAddr(Fn,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
		if(i==0x0)
		{
		#if AFN0CF33MaxPn!=0
			continue;
		#else
			switch(Fn)
			{
				case 33://F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,129,130,133,136);//抄表数据组合成F33_F34_F37_F38
					break;
				case 34://F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,131,132,134,135);//抄表数据组合成F33_F34_F37_F38
					break;
				case 35://F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,145,146);//抄表数据组合成F35_F36_F39_F40
					break;
				case 36://F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,147,148);//抄表数据组合成F35_F36_F39_F40
					break;
				case 37://F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,137,138,141,144);//抄表数据组合成F33_F34_F37_F38
					break;
				case 38://F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,139,140,142,143);//抄表数据组合成F33_F34_F37_F38
					break;
				case 39://F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,149,150);//抄表数据组合成F35_F36_F39_F40
					break;
				case 40://F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,151,152);//抄表数据组合成F35_F36_F39_F40
					break;
				case 246://当前掉电记录数据(测量点映射)
					MC(0xee,ADDR_DATABUFF,1+Terminal_Class1DataLib[Fn].PnLen);
					break;
				default:
					continue;
			}
			i=ADDR_DATABUFF+1;//数据地址
			p8s=(u8 *)i;
			p32=(u32 *)(ADDR_Terminal_ClassDataLen);
			//重新计算长度
		#if DefaultTariff==0//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			Tariff=0;
		#else
			#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
				Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
			#else
				Tariff=TMaxTariff;
			#endif
		#endif
			switch(Fn)
			{
				case 33://F33测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
				case 34://F34测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
				case 37://F37上月（上一结算日）测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
				case 38://F38上月（上一结算日）测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12)
			#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
					if(p8s[5]!=Tariff)
					{
						p8s[5]=Tariff;
					}
			#else
					if(p8s[5]==0xee)
					{
						p8s[5]=Tariff;
					}
			#endif
					p32[0]=6+(17*(p8s[5]+1));
					break;
				case 35://F35测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				case 36://F36测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				case 39://F39上月（上一结算日）测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				case 40://F40上月（上一结算日）测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12
			#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
					if(p8s[5]!=Tariff)
					{
						p8s[5]=Tariff;
					}
			#else
					if(p8s[5]==0xee)
					{
						p8s[5]=Tariff;
					}
			#endif
					p32[0]=6+(14*(p8s[5]+1));
					break;
				case 246://当前掉电记录数据(测量点映射)
					p32[0]=Terminal_Class1DataLib[Fn].PnLen;
					break;
				default:
					continue;
			}
		#endif
		}
		p32=(u32 *)(ADDR_Terminal_ClassDataLen);
		x=p32[0];
		if(x==0x0)
		{
			continue;
		}
		
		if(Fn==13)
		{//F13：搜索到的电表信息
			//计算发送缓冲还可填入的电表信息个数
			if((TXlen+4+14+8+2+(4+13))>LEN_UARTnTx)
			{
				if(ALLPn==0)//全部有效量点标志;0=否,1=是
				{
					TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
				goto MulTx;//发送-还有后续帧
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
			x=LEN_UARTnTx-(TXlen+4+14+8+2+4);
			x/=13;//可发送电表信息个数
			y=MRR(ADDR_AFN0CF13,2);//搜索到的电表总数量
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
				
			i=y-UARTCtrl->MulFrameGPCR;//多帧时通用计数寄存器(在AFN0CF13中,计数仅F13已发电表信息个数,在F13完成后清0)
			if(x>i)
			{
				x=i;
			}
			z=ADDR_AFN0CF13+2+(LEN_PerMeterAFN0CF13*UARTCtrl->MulFrameGPCR);
			//x=当前要填入的电表信息个数
			//y=搜索到的电表总数量
			//z=ADDR_AFN0CF13地址
			MWR(y,TXaddr,2);//搜到结果总数量m	BIN	2
			MWR(x,TXaddr+2,2);//本帧包含结果数量n	BIN	2
			TXaddr+=4;
			TXlen+=4;
			for(i=0;i<x;i++)
			{
				MR(TXaddr,z+(LEN_PerMeterAFN0CF13*i),13);//读电表信息
				TXaddr+=13;
				TXlen+=13;
			}
			UARTCtrl->MulFrameGPCR+=x;//多帧时通用计数寄存器(在AFN0CF13中,计数仅F13已发电表信息个数,在F13完成后清0)
			if(UARTCtrl->MulFrameGPCR>=y)
			{
				UARTCtrl->MulFrameGPCR=0;//多帧时通用计数寄存器(在AFN0CF13中,计数仅F13已发电表信息个数,在F13完成后清0)
				goto FnSelectEnd;
			}
			if(ALLPn==0)//全部有效量点标志;0=否,1=是
			{
				TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
			goto MulTx;//发送-还有后续帧
		}
	#if (USER/100)==6//用户:湖南系扩展,自动搜到的未知电表
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if(Fn==171)
		{//F171
			//计算发送缓冲还可填入的电表信息个数
			if((TXlen+4+14+8+2+(4+(6+14)))>LEN_UARTnTx)
			{
				if(ALLPn==0)//全部有效量点标志;0=否,1=是
				{
					TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
				goto MulTx;//发送-还有后续帧
			}
			x=LEN_UARTnTx-(TXlen+4+14+8+2+(4+6));
			x/=14;//可发送电表信息个数
			
			y=MRR(ADDR_AFN0CF13,2);//搜索到的电表总数量
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
			MWR(y,TXaddr,2);//搜到结果总数量m	BIN	2
			z=MRR(RXaddr,2);//本次读取起始序号(从1开始)
			if(z==0)
			{
				z=1;
			}
			z--;
			MWR(z+1,RXaddr,2);//本次读取起始序号(从1开始)
			MWR(z+1,TXaddr+2,2);//起始序号
			Addr=ADDR_AFN0CF13+2+(LEN_PerMeterAFN0CF13*z);
			if(z>y)
			{
				z=y;
			}
			y-=z;//搜索到的电表总数量
			z=MRR(RXaddr+2,2);//本次读取的最大数量0=表示读取全部
			if(z==0)
			{
				if(UARTCtrl->MulFrameGPCR==0)//后续帧标志:0=没,1=还有
				{
					z=y;
				}
			}
			if(z>y)
			{
				z=y;
			}
			MWR(z,RXaddr+2,2);//本次读取的最大数量
			if(z>x)
			{
				UARTCtrl->MulFrameGPCR=1;//有后续帧标志:0=没,1=还有
				i=z-x;
				MWR(i,RXaddr+2,2);//下次读取的最大数量
				i=MRR(RXaddr,2);//下次读取起始序号(从1开始)
				i+=x;
				MWR(i,RXaddr,2);//下次读取起始序号(从1开始)
				z=x;
			}
			else
			{
				UARTCtrl->MulFrameGPCR=0;//有后续帧标志:0=没,1=还有
			}
			MWR(z,TXaddr+4,2);//本帧包含结果数量n	BIN	2
			//UARTCtrl->MulFrameGPCR=有后续帧标志:0=没,1=还有
			//x=当前可填入的电表信息个数
			//y=搜索到的电表总数量
			//A=本次读取起始序号(从1开始)
			//z=本次读取的最大数量0=表示读取全部
			TXaddr+=6;
			TXlen+=6;
			for(i=0;i<z;i++)
			{
				MR(TXaddr,Addr,7);//电能表地址,通信协议类型
				MR(TXaddr+7,Addr+14,1);//电表相位
				y=MRR(Addr+13,1);//端口号
				if(y==31)
				{
					MR(TXaddr+8,(u32)&Terminal_Router->RouterAddr,6);
				}
				else
				{
					MC(0,TXaddr+8,6);
				}
				Addr+=LEN_PerMeterAFN0CF13;
				TXaddr+=14;
				TXlen+=14;
			}
			if(UARTCtrl->MulFrameGPCR==0)//有后续帧标志:0=没,1=还有
			{
				RXaddr+=4;
				goto FnSelectEnd;
			}
			if(ALLPn==0)//全部有效量点标志;0=否,1=是
			{
				TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
			goto MulTx;//发送-还有后续帧
		}
	#endif
	#endif
		if((Fn>=81)&&(Fn<=121))
		{//小时冻结数据
			if(PnHourCongeal(Pn)==0)//测量点小时冻结有效,返回=0无效,1=有效
			{
				continue;
			}
			//2字节冻结时间,冻结密度
			if((i>ADDR_HOURCONGEAL_START)&&(i<ADDR_HOURCONGEAL_END))
			{//从EFLSH读
				p8s=(u8 *)(ADDR_HOURCONGEAL_START);
			}
			else
			{//从ESRAM读
				p8s=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
			}
			if(p8s[1]==0x0)
			{//小时冻结密度=0(不冻结)
				continue;
			}
			if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
			{
				if(ALLPn==0)//全部有效量点标志;0=否,1=是
				{
					TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
				goto MulTx;//发送-还有后续帧
			}
      
			MR(TXaddr,(u32)p8s,2);
	#if SoftBug1==1//0=正常功能,1==376主站测试软件BUG.小时冻结密度
			MC(MaxHourCongeal,TXaddr+1,1);
	#endif
			//X字节冻结数据
			MR(TXaddr+2,i,x);
		#if Uart3761Check0xEE==1//0xEE数据检查：0=不检查;1=检查,若全0xEE返回否认
			if(Terminal_Class1DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
				if(CheckUartData_0xEE(TXaddr+2,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
				{
					continue;
				}
			}
		#endif
			TXlen+=2+x;
			TXaddr+=2+x;
		}
		else
		{
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
Fn169:
#endif
			if((TXlen+4+14+8+2+(x))>LEN_UARTnTx)
			{
				if(ALLPn==0)//全部有效量点标志;0=否,1=是
				{
					TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
				goto MulTx;//发送-还有后续帧
			}
			
//AFN0CF129以后要实时抄读*****************************************************************
#if ReadF129Above==1//召读1类F129以上数据:0=读原存储数据,1=实时读取后再上传
	#if (USER/100)==6//湖南
			if((Fn>=129)||(Fn==27)||(Fn==28))//F169集中抄表中继路由信息专有实时抄读,F170集中抄表电表抄读信息
	#else
		#if (USER/100)==11//吉林系
			if((Fn>=129)||(Fn==28))//F169集中抄表中继路由信息专有实时抄读,F170集中抄表电表抄读信息
		#else
			if(Fn>=129)//F169集中抄表中继路由信息专有实时抄读,F170集中抄表电表抄读信息
		#endif
	#endif
			{
				switch(Fn)
				{
					case 169://集中抄表中继路由信息	测量点号
					case 170://集中抄表电表抄读信息	测量点号
						break;
					case 179://当日电压统计数据
					case 180://当月电压统计数据
					#if (USER/100)==11//吉林
						//吉林版库指针已改为指向RAM,若内部交采需专再计算数据地址
						if(Check_ACSamplePn(Pn))//有效交采测量点号检查;返回0=无效,1=有效
						{//是内部交采
							if((Pn<=MaxRS485Pn)&&(Pn!=0))
							{//内部交采测量点号<=MaxRS485Pn
								if(Fn==179)
								{//F179
									i=ADDR_AFN0DF27_SOUR+((66+27)*(Pn-1));
								}
								else
								{//F180
									i=ADDR_AFN0DF35_SOUR+((66+27)*(Pn-1));
								}
							}
						}
					#endif
						break;
				#if (USER/100)==5//上海
					case 191://终端当前线电压
					case 241://	当前登录主站信息（最近一次）
						break;
					case 242://	当日登录主站信息
					case 243://	当日信号强度
						i=ADDR_DATABUFF;
						break;
				#endif
					case 245://	终端事件分类数据
					case 246://	当前掉电记录数据(测量点映射)
					case 247://	当前终端电池状态
						break;
				#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
					case 249://电表对时数据
						break;
					case 250://电表对时统计数据
						x=1+10*MRR(i,1);
						break;
				#endif
				#if (USER/100)==3//用户标识：福建
					case 253://现场信号信息
					case 254://本地通信模块信息
						break;
				#endif
				#if (USER/100)==11//吉林
					case 201://现场信号信息
						break;
				#endif
					default:
						if((UARTCtrl->LockFlags&0x70)==0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7
						{
						#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
							if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
						#else
							if(Terminal_Ram->PowerTask>10)//停电事件任务
						#endif
							{//3=电网供电
								y=ReadMeter_RealTime(PORTn,Fn,Pn);//实时读取1类数据;返回0=没读,!=0读且是FnDataBuff地址
								if(y!=0)//实时读取1类数据;返回0=没读,!=0读且是FnDataBuff地址
								{//有实时读取
									if(Fn!=28)//F28 电表运行状态字及其变位标志
									{
										i=y;//数据地址
									}
								}
								else
								{//没读取
									y=Check_AFN04F10_Pn(Pn);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
									if(y==0)
									{//返回0无
										i=ADDR_DATABUFF;
										MC(0xee,ADDR_DATABUFF,Terminal_Class1DataLib[Fn].PnLen);
									}
									else
									{//
										y=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+4,2);//通信速率及端口号,通信协议类型
										if(((y&=0x1f)==1)||((y>>8)==2))
										{//端口号码=1或协议=2,是内部交采
											//交采取当前数据上送
										}
										else
										{//外部表测量点
											i=ADDR_DATABUFF;
											MC(0xee,ADDR_DATABUFF,Terminal_Class1DataLib[Fn].PnLen);
										}
									}
								}
								if(ALLPn!=0)//全部有效量点标志;0=否,1=是
								{
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
									DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
									if(p16[0]!=p16[1])
									{//已接收到下帧,放弃全部有效量点的实时读取帧
										UARTCtrl->Task=0;//0=接收
										UARTCtrl->RxByte=0;//rx计数=0
										return;
									}
								}
								//有实时读取则修改发送缓冲长度立即发送每只表
								LEN_UARTnTx=0;//有实时读取,则修改发送缓冲长度=0(模拟发送缓冲满进行多帧发送)
								//抄表时间
								switch(Fn)
								{
									case 49://F49 当前电压、电流相位角
									case 0xF1://扩展17 当前电压、电流相位角(扩展用于终端内部交采)
										Fn=49;
										break;
									case 57://F57 当前A、B、C三相电压、电流2～N次谐波有效值
									case 58://F58 当前A、B、C三相电压、电流2～N次谐波含有率
									case 162://F162 电能表日历时钟
										break;
									default:
									#if Uart3761Check0xEE==0//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
										#if (((USER/100)==13)||((USER/100)==11))//黑龙江  吉林 
											if(PORTn==ACSAMPLEPORT) 
											{//黑龙江反馈交采日冻结抄表时间为23:59，要求为00:00以后  (修改交采口的抄表时间如为23:59，则修改为下一日的00:00） 

													YMDHMS=MRR(ADDR_TYMDHMS,6); 
													if((YMDHMS&0xFFFFFF)>0x235950) 
													{ 
															YMDHMS=YMDHMS_AddS(YMDHMS,10);//+10s 
													} 
													YMDHMS=(YMDHMS>>8); 
												 MWR(YMDHMS,i,5);     
											} 
											else 
											{ 
												 MW(ADDR_TYMDHMS+1,i,5);//填入抄表时间 
											} 
										#else 
										 MW(ADDR_TYMDHMS+1,i,5);//填入抄表时间 
										#endif 
									#else
										p8s=(u8 *)i;
										for(y=0;y<Terminal_Class1DataLib[Fn].PnLen;y++)
										{
											if(p8s[y]!=0xee)
											{
												break;
											}
										}
										if(y!=Terminal_Class1DataLib[Fn].PnLen)
										{//有实时读取,填入抄表时间(是否返回否认只判开始的抄表时间5字节)
										#if (((USER/100)==13)||((USER/100)==11))//黑龙江  吉林 
											if(PORTn==ACSAMPLEPORT) 
											{//黑龙江反馈交采日冻结抄表时间为23:59，要求为00:00以后  (修改交采口的抄表时间如为23:59，则修改为下一日的00:00） 

													YMDHMS=MRR(ADDR_TYMDHMS,6); 
													if((YMDHMS&0xFFFFFF)>0x235950) 
													{ 
															YMDHMS=YMDHMS_AddS(YMDHMS,10);//+10s 
													} 
													YMDHMS=(YMDHMS>>8); 
												 MWR(YMDHMS,i,5);     
											} 
											else 
											{ 
												 MW(ADDR_TYMDHMS+1,i,5);//填入抄表时间 
											} 
										#else 
										 MW(ADDR_TYMDHMS+1,i,5);//填入抄表时间 
										#endif 
										}
									#endif
										break;
								}
								//计算有效数据长度,费率数等变更时长度也变
							#if DefaultTariff==0//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
								Tariff=0;
							#else
								#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
									Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
								#else
									Tariff=TMaxTariff;
								#endif
							#endif								
								p8s=(u8 *)i;
								switch(Fn)
								{
									case 129://F129	当前正向有功电能示值（总、费率1～M）	测量点号
									case 131://F131	当前反向有功电能示值（总、费率1～M）	测量点号
									case 137://F137	上月（上一结算日）正向有功电能示值（总、费率1～M）	测量点号
									case 139://F139	上月（上一结算日）反向有功电能示值（总、费率1～M）	测量点号
									case 153://F153	第一时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 154://F154	第二时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 155://F155	第三时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 156://F156	第四时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 157://F157	第五时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 158://F158	第六时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 159://F159	第七时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 160://F160	第八时区冻结正向有功电能示值（总、费率1～M）	测量点号
									case 177://F177 当前组合有功电能示值（总、费率1~M）
									case 178://F178 上1 结算日组合有功电能示值（总、费率1~M）
								#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(5*(p8s[5]+1));
										break;
									case 130://F130	当前正向无功（组合无功1）电能示值（总、费率1～M）	测量点号
									case 132://F132	当前反向无功（组合无功1）电能示值（总、费率1～M）	测量点号
									case 133://F133	当前一象限无功电能示值（总、费率1～M）	测量点号
									case 134://F134	当前二象限无功电能示值（总、费率1～M）	测量点号
									case 135://F135	当前三象限无功电能示值（总、费率1～M）	测量点号
									case 136://F136	当前四象限无功电能示值（总、费率1～M）	测量点号
									case 138://F138	上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）	测量点号
									case 140://F140	上月（上一结算日）反向无功（组合无功1）电能示值（总、费率1～M）	测量点号
									case 141://F141	上月（上一结算日）一象限无功电能示值（总、费率1～M）	测量点号
									case 142://F142	上月（上一结算日）二象限无功电能示值（总、费率1～M）	测量点号
									case 143://F143	上月（上一结算日）三象限无功电能示值（总、费率1～M）	测量点号
									case 144://F144	上月（上一结算日）四象限无功电能示值（总、费率1～M）	测量点号
								#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(4*(p8s[5]+1));
										break;
									case 145://F145	当月正向有功最大需量及发生时间（总、费率1～M）	测量点号
									case 146://F146	当月正向无功最大需量及发生时间（总、费率1～M）	测量点号
									case 147://F147	当月反向有功最大需量及发生时间（总、费率1～M）	测量点号
									case 148://F148	当月反向无功最大需量及发生时间（总、费率1～M）	测量点号
									case 149://F149	上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）	测量点号
									case 150://F150	上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）	测量点号
									case 151://F151	上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）	测量点号
									case 152://F152	上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）	测量点号
								#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(7*(p8s[5]+1));
										break;
									case 168://F168	电能表结算信息	测量点号
								#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(10*(bcd_hex(p8s[5]+1)));
										break;
								}
							}
						}
						break;
				}
			}
#endif
//****************************************************************************************

			MR(TXaddr,i,x);
		#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
			if(Terminal_Class1DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
				if(CheckUartData_0xEE(TXaddr,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
				{
					LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
					continue;
				}
			}
		#endif
			TXlen+=x;
			TXaddr+=x;
		}
	FnSelectEnd:
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
		f=1;//有无有效数据填入标志;0=无,1=有

		switch(Fn)
		{//读数据后清相关数据的Fn
			case 9://终端状态量及变位标志
				MC(0,ADDR_AFN0CF9+1,1);
				break;
			case 12://控制输出回路开关接入状态量变位标志
				MC(0,ADDR_AFN0CF12+1,1);
				break;
			case 28://电表运行状态字变位标志
				MC(0,ADDR_AFN0CF28+5+(33*(Pn-1)),14);
				break;
		}
	}
	
	if(TXlen==0x0)
	{
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			goto err;
		}
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	if(UARTCtrl->TxFrameCount!=0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
		p8txbuff[13]|=0x20;//返回帧暂为多帧结束帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CON(PORTn);//主动定时任务CON标志位和功能码
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	//下启动帧序号TSEQ+1(当为定时主动上报时需要)
	if(UARTCtrl->LockFlags&0x30)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7
	{
		UARTCtrl->TxSEQ=p8rxbuff[13]&0x0f;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
		UARTCtrl->TxSEQ+=1;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	}
	return;
MulTx://发送-还有后续帧
	if(TXlen<=4)
	{
//		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
//		{
			goto err;
//		}
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]|=0x40;//返回帧为多帧起始帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	UARTCtrl->DIlen=DIlen;//数据标识长度寄存器
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CON(PORTn);//主动定时任务CON标志位和功能码
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	//下响应帧序号RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=等待多帧数据发送
	return;
}


#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
void Uart_AFN0CF201(u32 PORTn)//上海集中器扩展AFN0CF201读取最近一次抄读失败表个数P0
{
	u32 i;
	u32 x;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+2;
	//长度
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	//从AFN0CF170集中抄表电表抄读信息中计算最近一次抄读失败表个数
	x=0;
	for(i=0;i<MaxRS485AddCarrierPn;i++)
	{
		p8=(u8 *)(ADDR_AFN0CF170+(18*i));
		if(p8[4]==0)
		{//最近一次抄表成功/失败标志：1/0表示成功/失败。
			x++;
		}
	}
	p8txbuff[18]=x;
	p8txbuff[19]=(x>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}

void Uart_AFN0CF202(u32 PORTn)//上海集中器扩展AFN0CF202读取最近一次抄读电能表失败表信息P0
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 TXlen;
	u32 DIlen;
	u32 n;//每帧最大发送抄收个数
	UARTCtrl_TypeDef * UARTCtrl;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8;

	if(PORTn==IRPORT)
	{//红外口
		n=10;//每帧最大发送抄收个数
	}
	else
	{
		n=50;//每帧最大发送抄收个数
	}

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
	//Tp时间标签有效判定
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			if(DIlen<(12+4+6))
			{
	err:
				//全部否认
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
				Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
			if(i!=0x0)
			{
				return;//舍弃该报文
			}
		}
		else
		{
			if(DIlen<(12+4))
			{
				goto err;
			}
		}
		//起始序号
		x=p8rxbuff[18]+(p8rxbuff[19]<<8);
		//结束序号
		y=p8rxbuff[20]+(p8rxbuff[21]<<8);
		if(x>y)
		{//起始序号>结束序号
			goto err;
		}

		x=0;
		for(y=0;y<MaxRS485AddCarrierPn;y++)
		{
			p8=(u8 *)(ADDR_AFN0CF170+(18*y));
			if(p8[4]==0)
			{//最近一次抄表成功/失败标志：1/0表示成功/失败。
				x++;
			}
		}
		if(x<(p8rxbuff[18]+(p8rxbuff[19]<<8)))
		{
			x=0;
			goto TX;
		}
		else
		{
			//抄收数
			y=(p8rxbuff[20]+(p8rxbuff[21]<<8))-(p8rxbuff[18]+(p8rxbuff[19]<<8))+1;
			//总数>=抄收数且抄收数<=一帧发送个数，总数<抄收数且总数<=一帧发送个数
			if(((x>=y)&&(y<=n)) || ((x<y)&&(x<=n)))	
			{//单帧发送
				//从AFN0CF170集中抄表电表抄读信息中计算最近x~y次抄读失败表信息
				x=0;
				for(i=0;i<MaxRS485AddCarrierPn;i++)
				{
					p8=(u8 *)(ADDR_AFN0CF170+(18*i));
					if(p8[4]==0)
					{//最近一次抄表成功/失败标志：1/0表示成功/失败。
						if(x==(p8rxbuff[18]+(p8rxbuff[19]<<8)))
						{
							break;
						}
						x++;
					}
				}
				x=0;
				for(y=i;y<MaxRS485AddCarrierPn;y++)
				{
					p8=(u8 *)(ADDR_AFN0CF170+(18*y));
					if(p8[4]!=0)
					{//最近一次抄表成功/失败标志：1/0表示成功/失败。
						continue;
					}
					//抄收信息序号
					p8txbuff[18*x+20]=x;
					p8txbuff[18*x+21]=(x>>8);
					z=Check_AFN04F10_Pn(y+1);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
					//测量点号、通信速率及端口号、通信协议类型、通信地址
					MR((u32)p8txbuff+18*x+22,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2,10);
					//所属测量点号
					MR((u32)p8txbuff+18*x+22+10,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2+8,6);
					x++;
					if(x==((p8rxbuff[20]+(p8rxbuff[21]<<8))-(p8rxbuff[18]+(p8rxbuff[19]<<8))+1))
					{
						break;
					}
				}
TX:
				//抄收个数
				p8txbuff[18]=x;
				p8txbuff[19]=(x>>8);
				//长度
				TXlen=12+2+18*x;
				p8txbuff[1]=TXlen;
				p8txbuff[2]=(TXlen>>8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			}
			else
			{//多帧发送
				goto MulTX;
			}
		}
	}
MulTX://多帧发送
	//起始序号
	x=(p8rxbuff[18]+(p8rxbuff[19]<<8))+n*UARTCtrl->TxFrameCount;
	//结束序号
	y=p8rxbuff[20]+(p8rxbuff[21]<<8);
	if(x>y)
	{//起始序号>结束序号
		//多帧结束帧
		p8txbuff[13]|=0x20;
		UARTCtrl->TxFrameCount=0;
		//抄收个数
		x=0;
		goto TX;
	}
//SEQ：D6D5，11单帧，10多帧第1帧有后续帧，00多帧中间帧，01多帧结束帧
	//多帧中间帧
	p8txbuff[13]&=0x9f;
	if(UARTCtrl->TxFrameCount==0)
	{//多帧第1帧
		p8txbuff[13]|=0x40;
	}
	//从AFN0CF170集中抄表电表抄读信息中计算最近x~y次抄读失败表信息
	x=0;
	for(i=0;i<MaxRS485AddCarrierPn;i++)
	{
		p8=(u8 *)(ADDR_AFN0CF170+(18*i));
		if(p8[4]==0)
		{//最近一次抄表成功/失败标志：1/0表示成功/失败。
			if(x==((p8rxbuff[18]+(p8rxbuff[19]<<8))+n*UARTCtrl->TxFrameCount))
			{
				break;
			}
			x++;
		}
	}
	if(i>=MaxRS485AddCarrierPn)
	{//在起始序号~结束序号之间未找到失败表
		//多帧结束帧
		p8txbuff[13]|=0x20;
		UARTCtrl->TxFrameCount=0;
		//抄收个数
		x=0;
		goto TX;
	}
	x=0;
	for(y=i;y<MaxRS485AddCarrierPn;y++)
	{
		p8=(u8 *)(ADDR_AFN0CF170+(18*y));
		if(p8[4]!=0)
		{//最近一次抄表成功/失败标志：1/0表示成功/失败。
			continue;
		}
		//抄收信息序号
		p8txbuff[18*x+20]=(x+n*UARTCtrl->TxFrameCount);
		p8txbuff[18*x+21]=((x+n*UARTCtrl->TxFrameCount)>>8);
		z=Check_AFN04F10_Pn(y+1);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		//测量点号、通信速率及端口号、通信协议类型、通信地址
		MR((u32)p8txbuff+18*x+22,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2,10);
		//所属测量点号
		MR((u32)p8txbuff+18*x+22+10,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2+8,6);
		x++;
		if(x==n)
		{
			break;
		}
		if(x==((p8rxbuff[20]+(p8rxbuff[21]<<8))-(p8rxbuff[18]+(p8rxbuff[19]<<8)+n*UARTCtrl->TxFrameCount)+1))
		{
			break;
		}
	}

	//抄收个数
	p8txbuff[18]=x;
	p8txbuff[19]=(x>>8);
	//长度
	TXlen=12+2+18*x;
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位

	UARTCtrl->TxFrameCount++;
	//下响应帧序号RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=等待多帧数据发送
	return;
}

#endif

#if (USER/100)==12//四川用户
void Uart_AFN0CF15(u32 PORTn)//四川扩展读取SIM卡信号强度
{
	u32 i;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+2;
	//长度
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	i=Terminal_Ram->SignaldBm;//信号强度0-31
	if(i==0x99)
	{
		i=0xeeee;//0x99未知或无法获取
	}
	else
	{
		i=bcd_hex(i);//8位压缩BCD数转为HEX
		if(i>31)
		{
			i=31;
		}
		i*=2;
		i=113-i;
		i=hex_bcd(i);
		i|=0x1000;
	}

	p8txbuff[18]=i;
	p8txbuff[19]=(i>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}
void Uart_AFN0CF2040(u32 PORTn)//2016年第三批中标四川扩展读取远程模块信号强度P0
{
	u32 i;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u64 z;
	
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+27;
	//长度
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	z=MRR(ADDR_AFN0CF2040+1,6);
	if(z==0)//当电话号码无法获取时全为E：EE EE EE EE EE EE。
	{
		MC(0xee,ADDR_AFN0CF2040+1,6);
	}
	MW(ADDR_AFN0CF2040,((u32)p8txbuff)+18,27);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}

void Uart_AFN0CF305(u32 PORTn)//透明任务执行状态P0 201703
{
	u32 i;
	u32 x;
	u32 byte;
	u32 bin;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8* p8;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+255;
	//长度
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
//---------------------------把1008个测量点的执行状态变为126个字节
	MR(ADDR_DATABUFF,ADDR_AFN0CF305,LEN_AFN0CF305);
	p8=(u8*)ADDR_DATABUFF;
	byte=0;
	i=0;
	while(i<LEN_AFN0CF305)
	{
		x=0;
		for(bin=0;bin<8;bin++)
		{
			if(p8[i]>=0xFE)
			{
				x|=1<<bin;
			}
			i++;
		}
		p8[byte]=x;
		byte++;
	}
	#if (LEN_AFN0CF305/8)>255
		#error
	#endif
	MC(0xFF,ADDR_DATABUFF+(LEN_AFN0CF305/8),255-(LEN_AFN0CF305/8));
//---------------------------
	MW(ADDR_DATABUFF,((u32)p8txbuff)+18,255);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}

void Uart_AFN0CF306(u32 PORTn)//透明任务执行结果Pn 201703
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 LEN_data;
	u32 Pn;
	u32 TXlen;
	u32 DIlen;
	u32 TXaddr;
	u32 LEN_UARTnTx;
//	u32 RXDIaddr;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0c;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
//用户区数据标识和参数数据长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
//	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	TXaddr=(u32)p8txbuff+14;
	TXlen=8;
	while(DIlen>4)
	{
		LEN_data=0;
		if((TXlen+4+14+8+2+9)>LEN_UARTnTx)
		{
			goto err;
		}
		MR(TXaddr,(u32)p8rxbuff+14,4);
		TXaddr+=4;
		TXlen+=4;
		CopyDI(PORTn,(u32)p8rxbuff+14);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		Pn=i&0xffff;
		if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
		{
			goto err;
		}
		#if (LEN_AFN0CF306>LEN_DATABUFF)
			#error LEN_AFN0CF306 Over
		#endif
//----------------------------------清除透明任务同时清透明任务执行结果		
			i=MRR(ADDR_AFN0CF305+(Pn-1),1);
			if(i==0xFE)
			{
				MC(0,ADDR_DATABUFF,LEN_AFN0CF306);
			}
			else
			{
				MR(ADDR_DATABUFF,ADDR_AFN0CF306+(Pn-1)*LEN_AFN0CF306,LEN_AFN0CF306);
			}
//----------------------------------		
//		MR(ADDR_DATABUFF,ADDR_AFN0CF306+(Pn-1)*LEN_AFN0CF306,LEN_AFN0CF306);
		i=MRR(ADDR_AFN10F306+(Pn-1)*LEN_AFN10F306,2);//取任务编号
		if(i==0)
		{//测量点无配置任务时，任务编号、完成时刻、本帧报文个数置为00
			MC(0,TXaddr,9);
			TXlen+=9;
			LEN_data=p8rxbuff[18];//查询数量
			LEN_data+=5;
		}
		else
		{
			if((TXlen+4+14+8+2+9)>LEN_UARTnTx)
			{
				goto err;
			}
			p8txbuff=(u8*)TXaddr;
			MWR(i,TXaddr,2);//任务编号
			TXaddr+=2;
			TXlen+=2;
			MW(ADDR_DATABUFF+2,TXaddr,6);//任务完成时刻
			TXaddr+=6+1;
			TXlen+=6+1;
			j=0;//本帧报文个数
			x=p8rxbuff[18];//查询数量
			LEN_data+=x+5;
			if(x==0)
			{//=0查询测量点所有报文执行结果
				for(i=0;i<15;i++)
				{
					if((TXlen+14+2+169)>LEN_UARTnTx)
					{
						goto err;
					}
					x=MRR(ADDR_DATABUFF+9+i*169,1);//报文序号i
					if(x!=0)
					{
						y=MRR(ADDR_DATABUFF+9+i*169+2,1);//报文i长度
						if(y>166)
						{//报文长度超过166字节
							goto err;
						}
						MR(TXaddr,ADDR_DATABUFF+9+(x-1)*169,(y+3));
						TXaddr+=(y+3);
						TXlen+=(y+3);
						j++;//本帧报文个数
					}
				}
				p8txbuff[8]=j;//本帧报文个数
			}
			else
			{//!=0查询指定报文，填充查询数量及报文序号m
				i=MRR(ADDR_AFN10F306+(Pn-1)*LEN_AFN10F306+5,1);//取任务报文总数
				if(x>i)
				{
					x=i;
				}
				for(i=0;i<x;i++)
				{
					if((TXlen+14+2+169)>LEN_UARTnTx)
					{
						goto err;
					}
					y=MRR(((u32)p8rxbuff)+18+1+i,1);//报文序号i
					if((MRR(ADDR_DATABUFF+9+(y-1)*169,1))==0)//报文序号
					{
						break;
					}
					z=MRR(ADDR_DATABUFF+9+(y-1)*169+2,1);//报文i长度
					if(z>166)
					{//报文长度超过166字节
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF+9+(y-1)*169,(z+3));
					TXaddr+=(z+3);
					TXlen+=(z+3);
					j++;//本帧报文个数
				}
				p8txbuff[8]=j;//本帧报文个数
			}
		}
		p8rxbuff+=LEN_data;
		if(DIlen>LEN_data)
		{
			DIlen-=LEN_data;
		}
		else
		{
			DIlen=0;
		}
	}
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}
#endif



