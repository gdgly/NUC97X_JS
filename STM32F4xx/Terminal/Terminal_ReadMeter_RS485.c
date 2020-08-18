
//终端抄表
#include "Project.h"
#include "Terminal_ReadMeter_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_ReadMeter_ACSample.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"

#include "AutoSearchMeter_RS485.h"
#include "BatchTask_RS485.h"
#include "../Display/Warning.h"
#include "KeyUser.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_ReadMeter_Event.h"

#include "Terminal_ReadMeter_WSH41.h"
#include "Terminal_ReadMeter_ShH.h"
#include "Terminal_ReadMeter_DL698.h"

	
#if ((Project/100)==3)&&((USER/100)==12)//四川专变
#include "Terminal_ReadMeter_HX.h"
#include "Terminal_ReadMeter_HX_6E.h"
#endif

void ReadMeter_Protocol(u32 PORTn)//抄表规约
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
#if (Extern_Terminal_ReadMeter_DL645_1997!=0)||(Extern_Terminal_ReadMeter_ShH!=0)//外部程序:0=I2,1=E1,2=E2,3=;终端抄表DL645-1997或外部程序:0=I2,1=E1,2=E2,3=;终端抄表 上海表规约
	u32 *p32;
	p32=p32;
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型
	if(i==2)
	{//交流采样装置(2)
		if(PORTn==ACSAMPLEPORT)
		{//当前口是内部采样口
			Terminal_ReadMeter_ACSample(PORTn);//终端抄表,交流采样装置
		}
		else
		{
			UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
		}
		return;
	}
	if(i==1)
	{//DL/T645-1997(1)
	#if Extern_Terminal_ReadMeter_DL645_1997==0//外部程序:0=I2,1=E1,2=E2,3=;终端抄表DL645-1997
		Terminal_ReadMeter_DL645_1997(PORTn);//终端抄表DL645-1997
	#else
		if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[101])(PORTn);//终端抄表DL645-1997
		}
	#endif	
		return;
	}
	if(i==30)
	{//DL/T645-2007(30)
		Terminal_ReadMeter_DL645_2007(PORTn);//终端抄表DL645-2007
		return;
	}
#if ((Project/100)==3)&&((USER/100)==12)//四川专变
	if(i==4)
	{//HX读表相关
		Terminal_ReadMeter_HX_6E(PORTn);//终端抄表DL645-2007
		return;
	}
#endif
#if (USER/100)==10//河北专用
	if(i==7)
	{//威胜规约(河北)
		Terminal_ReadMeter_WSH41(PORTn);//终端抄表RS485口_威胜规约4.1版本
		return;
	}
#endif

#if (USER/100)==5//上海专用
	if(i==21)
	{//上海规约
	#if Extern_Terminal_ReadMeter_ShH==0//外部程序:0=I2,1=E1,2=E2,3=;终端抄表 上海表规约
		Terminal_ReadMeter_ShH(PORTn);//终端抄表RS485口_上海规约
	#else
		if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[112])(PORTn);//终端抄表_上海规约
		}
	#endif
		return;
	}
	if(i==3)
	{//698规约
		Terminal_ReadMeter_DL698(PORTn);//终端抄表RS485口_上海规约
		return;
	}
#endif

	//无对应规约
	UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
}


void Terminal_AFN0CF11_RS485(u32 PORTn,u32 RMTask)//终端类1数据F11,终端集中抄表状态信息;入口要计算的抄表任务号
{
	u32 i;
	u32 x;
	u32 Pn;
	u32 RS485PORT;
	u8 *p8;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 MeterNum;
#if IC_SST39VF6401B==0//0=没,1=有
	u32 Addr;
#else
	u8 *p8d;
#endif
#if ((Project/100)==2)||((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	u32 y;
	u32 KeyUser;
	u32 n;
	#if IC_SST39VF6401B==1//0=没,1=有
	u8 *p8s;
	#endif
#endif


	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
#if DateRMTask==1//每日抄表任务;0=没有,1=有
	i=MRR(ADDR_TYMDHMS+1,2);
	#if (((Project/100)==2) && ((USER/100)==5))//上海集中器
	p8=(u8*)(ADDR_AFN04F242);//抄表延时分钟数
	n=p8[0];
	if(n>=60)//抄表延时分钟数：1-60分钟
	{
		n=0x100;//1:00
	}
	else
	{
		n=hex_bcd(n);
	}
	if(i<n)
	{
		return;
	}
	#else
	if(i<RMM_StartHM)
	{
		return;
	}
	#endif
#endif
#if (USER/100)==5//上海
	i=MRR(ADDR_TYMDHMS+1,2);
#if ((Project/100)==2)//上海集中器
	p8=(u8*)(ADDR_AFN04F242);//抄表延时分钟数
	n=p8[0];
	if(n>=60)//抄表延时分钟数：1-60分钟
	{
		n=0x100;//1:00
	}
	else
	{
		n=hex_bcd(n);
	}
	if(i<n)
	{
		return;
	}
#endif
	if(i<RMM_StartHM)
	{
		return;
	}
#endif
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=2;
			break;
		case RS485_4PORT://RS485-4
			RS485PORT=3;
			break;
		default:
			return;
	}
	p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
#if ((Project/100)==2)||((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	KeyUser=MRR(ADDR_AFN04F35,1);
	if(KeyUser>MaxKeyUser)
	{
		KeyUser=MaxKeyUser;
	}
	p8[0]=3;//本项数据块个数n	BIN	1(1-31)
#else
	p8[0]=2;//本项数据块个数n	BIN	1(1-31)
#endif
	p8+=(19*(RS485PORT-1));
	if(PORTn!=RS485_4PORT)
	{//485口
		RS485PORT++;
		p8[1]=RS485PORT;//终端通信端口号	BIN	1
	}
	else
	{//载波口
		p8[1]=31;//终端通信端口号	BIN	1
		RS485PORT=31;
	}
	MeterNum=0;//要抄电表总数=0
	x=0;//抄表成功数
#if ((Project/100)==2)||((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	y=0;//抄重点户成功数
#endif
	#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
		#error
	#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	Addr=ADDR_128KDATABUFF+2;
	//Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//通信速率及端口号
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//通信端口号相同
			Pn=MRR(Addr,2);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//所属测量点号!=0x0
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]!=3)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
				{
					MeterNum++;//要抄电表总数
				}
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==2)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
				{
					x++;//抄表成功数
				#if ((Project/100)==2)||((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					for(n=0;n<KeyUser;n++)
					{
						if((i+1)==MRR(ADDR_AFN04F35+1+(2*n),2))//重点户的电能表/交流采样装置序号
						{
							y++;//抄重点户成功数
							break;
						}
					}
				#endif
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	MWR(MeterNum,((u32)p8)+2,2);//要抄电表总数	BIN	2
	if(MeterNum==x)
	{
		p8[4]|=2;//D0=1正在抄表,D1=1时段内完成
		if(p8[14+3]==0xee)
		{//结束时间未填
			MR(((u32)p8)+14,ADDR_AFN0CF2,6);
		}
	}
	else
	{
		p8[4]&=0xfd;//D0=1正在抄表,D1=0时段内未完成
		MC(0xee,((u32)p8)+14,6);//抄表结束时间	见附录A.1	6
	}
	if(UARTCtrl->RMFlag!=0)//0=抄表间隔暂停,1=抄表进行中
	{//注:抄表暂停时清0了完成标志
		MWR(x,((u32)p8)+5,2);//抄表成功块数	BIN	2
	#if ((Project/100)==2)||((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		MWR(y,((u32)p8)+7,1);//抄重点表成功块数	BIN	1
	#else
		MWR(0,((u32)p8)+7,1);//抄重点表成功块数	BIN	1
	#endif
	}
	//抄表开始时间	见附录A.1	6
	if(p8[8+3]==0xee)
	{//无开始时间
		MR(((u32)p8)+8,ADDR_AFN0CF2,6);
	}
	//抄表结束时间	见附录A.1	6
}

u32 NoRMCompletes_RS485(u32 PORTn,u32 RMTask)//RS485未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Pn;
	u32 RS485PORT;

	u32 Addr;




	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=2;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=3;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=4;
			break;
		default:
			return 0;
	}
	x=0;//未抄数
	y=0;//失败数
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//通信速率及端口号
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//通信端口号相同
			Pn=MRR(Addr,2);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//所属测量点号!=0x0
				z=AllRMList->Pn[Pn].RMCompleteS[RMTask];
				if(z==0)
				{
					x++;//未抄数
				}
				else
				{
					if(z==1)
					{
						y++;//失败数
					}
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return (x+(y<<16));
}
/*
void ClrRMTaskCompletePn_RS485(u32 Pn)//清指定Pn的抄表任务完成标志;入口:Pn
{
	u32 i;
	for(i=0;i<MaxRMTask;i++)
	{
		AllRMList->Pn[Pn].RMCompleteS[i]=0;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
	}
}
*/
void ClrRMTaskCompletes_RS485(u32 PORTn,u32 RMTask,u32 All)//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
{
	u32 i;
	u32 Pn;
	u32 RS485PORT;
	u32 Addr;

	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=2;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=3;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=4;
			break;
		default:
			return;
	}
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//通信速率及端口号
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//通信端口号相同
			Pn=MRR(Addr,2);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//所属测量点号!=0x0
				if(All==0)
				{//只清失败
					if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==1)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
					{
						AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
					}
				}
				else
				{//清全部
					AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
}

u32 NewTask_RS485(u32 PORTn)//RS485口执行新任务;返回：0=没执行新任务，1=执行新任务
{
  UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(UARTCtrl->Lock==0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
	{
		return 0;
	}
	if(((UARTCtrl->RMMeterEvent&1)!=0)||(UARTCtrl->SubsequentFrames!=0))
	{//正在抄读电能表主动上报事件或读后续帧
		return 0;
	}
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	#if DateRMTask==1//每日抄表任务;0=没有,1=有
	if(UARTCtrl->DateReRMCount!=0)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
	{
		if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
		{//只有循抄时或抄读电能表事件时才能插入高优先级
			NewRMEvene(PORTn);//新或恢复读电能表事件任务;返回0=没新进入,1=新进入
		}
	}
	#else
	if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
	{//只有循抄时或抄读电能表事件时才能插入高优先级
		NewRMEvene(PORTn);//新或恢复读电能表事件任务;返回0=没新进入,1=新进入
	}
	#endif
#endif
	if(UARTCtrl->RMTaskC>=(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
	{//只有循抄时才能插入高优先级
		if(UARTCtrl->RMTaskN&2)//新每日任务
		{
			UARTCtrl->RMTaskN&=0xfd;//去新每日任务
			UARTCtrl->RMTaskC=1;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
			ClrRMTaskCompletes_RS485(PORTn,1,0);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
			if(UARTCtrl->DateReRMCount!=0xff)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
			{
				UARTCtrl->DateReRMCount++;//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
			}
		NewTask:
			UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
			UARTCtrl->FnCountS=UARTCtrl->FnCount;//485抄表原循抄任务的Fn计数
			UARTCtrl->MeterNoS=UARTCtrl->MeterNo;//485抄表原循抄任务的配置序号计数(半字对齐)
			UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
			UARTCtrl->FnCount=0;//485抄表Fn计数
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
			UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
			return 1;
		}
		if(UARTCtrl->RMTaskN&4)//新每周任务
		{
			UARTCtrl->RMTaskN&=0xfb;//去新每周任务
			UARTCtrl->RMTaskC=2;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
			goto NewTask;
		}
		if(UARTCtrl->RMTaskN&8)//新定时抄读停电记录任务
		{
			UARTCtrl->RMTaskN&=0xf7;//去新定时抄读停电记录任务
			UARTCtrl->RMTaskC=3;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
			goto NewTask;
		}
	}
	return 0;
}


void ReadMeter_RS485(u32 PORTn)//RS485抄表
{
	u32 i;
	u32 x;
	u32 y;
	u32 RS485PORT;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	u8 *p8;
	u16 *p16;
#if Extern_BatchTask_RS485!=0//外部程序:0=I2,1=E1,2=E2,3=
#if IC_SST39VF6401B==1//0=没,1=有
	u32 * p32;
#endif
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(PORTn)
	{
		case ACSAMPLEPORT://内部交流采样
			RS485PORT=0;
			p16timer=(u16 *)&Terminal_Ram->ACSampleRM_S_Timer;
			break;
		case RS485_1PORT://RS485-1
			RS485PORT=1;
			p16timer=(u16 *)&Terminal_Ram->RS4851RM_S_Timer;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=2;
			p16timer=(u16 *)&Terminal_Ram->RS4852RM_S_Timer;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=3;
			p16timer=(u16 *)&Terminal_Ram->RS4853RM_S_Timer;
			break;
		default:
			return;
	}
	
	if(RS485PORT!=0)
	{//不是交采
		if(Terminal_Ram->RMDelay_S_Timer!=0x0)//10 上电后延时抄表秒定时器
		{
			CopyString((u8 *)"485 等待上电",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			MWR(RS485PORT,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			return;
		}
	}

#if PowerOnRMTask==0//电源开抄表任务;0=没有,1=有
	if(UARTCtrl->RMTaskC==0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
	{
		UARTCtrl->RMTaskC=(MaxRMTask-1);
	}
#endif

//转发批量任务
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
#endif
		#if Extern_BatchTask_RS485==0//外部程序:0=I2,1=E1,2=E2,3=
		BatchTask_RS485_SETRTC(PORTn);//数据转发批量任务-RS485-时钟对时
		if(UARTCtrl->BatchTask>=2)//转发批量任务:0=空,1=等待口空闲启动,2...过程
		{
			return;
		}
		#else
		if(UARTCtrl->BatchTaskLock!=0x55)//0x55=转发批量任务正在被调用禁止其他程序再次调用
		{
			i=MRR(ADDR_AFN05F30+1,1);//终端对电能表对时功能的开启与关闭
			if(i!=1)
			{//0=关闭，1=开启，其它值关闭
				if(UARTCtrl->BatchTask>=2)//转发批量任务:0=空,1=等待口空闲启动,2...过程
				{
					Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					if(UARTCtrl->Task<=2)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						UARTCtrl->Task=0;//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
					}
				}
				else
				{
					UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
				}
			}
			else
			{//开启
				if(UARTCtrl->BatchTask==0)//转发批量任务:0=空,1=等待口空闲启动,2...过程
				{//0=空
					x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//取任务类型1(对时任务)配置电表数量n
					if(x==0)
					{//配置电表数量n=0
						UARTCtrl->BatchMeterCount=0;//转发批量任务电表计数
					}
					else
					{
						if(x>MaxRS485AddCarrierPn)
						{//配置电表数量>MaxRS485AddCarrierPn
							x=MaxRS485AddCarrierPn;
						}
						if(UARTCtrl->BatchMeterCount<x)
						{//转发批量任务计数<配置电表数量
							UARTCtrl->BatchTask=1;//转发批量任务:0=空,1=等待口空闲启动,2...过程
						}
					}
				}
				else
				{//1=等待口空闲启动,2...过程
				#if IC_SST39VF6401B==0//0=没,1=有
					BatchTask_RS485_SETRTC(PORTn);//数据转发批量任务-RS485-时钟对时
				#else
					if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
					{
		    		p32=(u32 *)(ADDR_E1_ENTRY);
		    		(*(void(*)())p32[103])(PORTn);//数据转发批量任务-RS485-时钟对时
					}
				#endif
				}
			}
		}
		if(UARTCtrl->BatchTask>=2)//转发批量任务:0=空,1=等待口空闲启动,2...过程
		{
			return;
		}
		#endif
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	}
#endif
	if(PORTn==ACSAMPLEPORT)
	{//是内部交流采样
		//日更新
		if(UARTCtrl->Task==0)//0=空闲
		{
			x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//原月日
			y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//当前月日
			if(x!=y)
			{
				UARTCtrl->RTCD[0]=y;
				UARTCtrl->RTCD[1]=y>>8;
				UARTCtrl->RMComplete&=0x7f;//485抄表D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
				UARTCtrl->FnCount=0;//485抄表Fn计数
				UARTCtrl->RMCount=0;//485抄表命令计数
				UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			}
		}
	}
	else
	{//不是内部交流采样
//分更新
		if(UARTCtrl->RTCM!=Comm_Ram->TYMDHMS[1])
		{
			UARTCtrl->RTCM=Comm_Ram->TYMDHMS[1];
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			RMEveneTimer(PORTn);//分级读电能表事件定时
		#endif
			//广播校时
			i=MRR(ADDR_AFN05F30+1,1);//终端对电能表对时功能的开启与关闭
			if(i==1)
			{//0=关闭，1=开启，其它值关闭
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+1,1);//终端抄表运行参数设置,端口31
				if((i&0x08)!=0x0)
				{//要求终端定时对电表广播校时
					x=MRR(ADDR_TYMDHMS+1,3);//取当前实时钟日时分
					y=MRR((ADDR_AFN04F33+((14+(4*24))*RS485PORT))+10,3);//对电表广播校时定时时间日时分
					if((y>>16)==0x0)
					{//当日=0时,表示每日校时
						x&=0xffff;
					}
					if(x==y)
					{
						if(UARTCtrl->BroadcastTime<3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						{
							UARTCtrl->BroadcastTime=3;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
					}
				}
			}
		#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江						
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					x=0;
					break;
				case RS485_2PORT://RS485-2
					x=1;
					break;
			}
			x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
			if(x==2)
			{
				x=Check_HMS(MRR(ADDR_AFN04F298_Ext+2,2));//时分秒合法检查,返回0合法,1非法
				x<<=8;
				y=Check_HMS(MRR(ADDR_AFN04F298_Ext+4,2));//时分秒合法检查,返回0合法,1非法
				y<<=8;
				if((x==0)&&(y==0))
				{//时钟合法
					if(DataComp(ADDR_AFN04F298_Ext+2,ADDR_TYMDHMS+1,2)!=1)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
					{
						if(DataComp(ADDR_AFN04F298_Ext+4,ADDR_TYMDHMS+1,2)!=2)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
						{
							if(UARTCtrl->BroadcastTime<3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							{
								UARTCtrl->BroadcastTime=3;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										x=0;
										break;
									case RS485_2PORT://RS485-2
										x=1;
										break;
								}
								MWR(3,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
							}
						}
					}
				}
			}
		#endif	
		#if((USER/100)==5)//上海
			i=MRR(ADDR_AFN05F201,1);
			if(i&(1<<(RS485PORT-1)))
			{
				if(UARTCtrl->BroadcastTime<3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				{
					UARTCtrl->BroadcastTime=3;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
			}
		#endif
		}
//时更新
		if(UARTCtrl->Task==0)//0=空闲
		{
			if(UARTCtrl->RTCH!=Comm_Ram->TYMDHMS[2])
			{
				UARTCtrl->RTCH=Comm_Ram->TYMDHMS[2];
			#if DateRMTask==1//每日抄表任务;0=没有,1=有
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
				{//当前是循抄
					i=NoRMCompletes_RS485(PORTn,1);//RS485未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					if(i!=0)
					{//需补抄
						UARTCtrl->RMTaskN|=2;//新每日任务
					}
				}
			#endif
				/*
				if(UARTCtrl->RMKeyUser==0)//抄读重点户标志,0=常规抄读,1=重点户抄读
				{
					UARTCtrl->MeterNoBackupOfKeyUser=UARTCtrl->MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
					UARTCtrl->FnEndBackupOfKeyUser=UARTCtrl->FnEnd;//485抄表Fn计数结束标志,0=结束,1=没结束(原常规抄表的Fn计数结束标志寄存)
					UARTCtrl->FnCountBackupOfKeyUser=UARTCtrl->FnCount;//485抄表Fn计数(原常规抄表的Fn计数寄存)
					UARTCtrl->RMErrorBackupOfKeyUser=UARTCtrl->RMError;//485抄表本电能表通信失败标志,B0=失败,B1=F129失败(原常规抄表的失败标志寄存)
					UARTCtrl->KeyUserRMCount=0;//重点户抄读表数计数
					UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
					UARTCtrl->FnCount=0;//485抄表Fn计数
					UARTCtrl->RMCount=0;//485抄表命令计数
					UARTCtrl->ReRMCount=0;//485抄表命令重发计数
				}
				UARTCtrl->RMKeyUser=1;//抄读重点户标志,0=常规抄读,1=重点户抄读
				*/
			}
		}
//日更新
		if(UARTCtrl->Task==0)//0=空闲
		{
			x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//原月日
			y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//当前月日
			if(x!=y)
			{
				UARTCtrl->RMEventTaskN|=0x20;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3;b5=日更新,b6=月更新(上海每日月抄事件用)
				if(UARTCtrl->RTCD[1]!=Comm_Ram->TYMDHMS[4])
				{
					//月更新
					UARTCtrl->RMEventTaskN|=0x40;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3;b5=日更新,b6=月更新(上海每日月抄事件用)
				}
				UARTCtrl->RTCD[0]=y;
				UARTCtrl->RTCD[1]=y>>8;
				UARTCtrl->RMComplete&=0x7f;//485抄表D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
				StoreRMEvene(PORTn);//暂存正在读电能表事件的任务
			#endif
				//每日任务
				UARTCtrl->DateReRMCount=0;//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
		#if DateRMTask==1//每日抄表任务;0=没有,1=有
				UARTCtrl->RMTaskN|=0x2;//新每日任务
				ClrRMTaskCompletes_RS485(PORTn,1,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
		#endif
				//清集中抄表信息
				p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
				p8+=(19*(RS485PORT-1));
				MC(0,((u32)p8)+4,4);
				MC(0xee,((u32)p8)+8,12);
				//自动搜表
		#if (USER/100)!=6//用户:不是湖南;湖南另有自行定时
				UARTCtrl->LockFlags&=0xfb;//清本已自动搜表
				i=MRR(ADDR_AFN05F149+2+(RS485PORT-1),1);//电能表通信参数自动维护功能开启与关闭
				if(i==0xaa)
				{//每日执行
					MC(0,Get_ADDR_UARTn(PORTn),6);//请fifo指针
					UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
				}
		#endif
		#if ((USER/100)==8)||((USER/100)==7)//用户标识：通用、浙江
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						i=0;
						break;
					case RS485_2PORT://RS485-2
						i=1;
						break;
				}
				MWR(0,ADDR_AFN04F299_Ext+28,1);
				if(MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1)>=5)
				{//日更新时清零
					MWR(0,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
				}
		#endif
		#if (USER/100)==8//用户标识：通用
				y=Comm_Ram->TYMDHMS[3];//当前日
				y--;
				MC(0xff,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3+4+MaxRS485AddCarrierPn*3);
				p8=(u8 *)ADDR_DATABUFF;
				p8[0]=Comm_Ram->TYMDHMS[3];//当前日
				p8[1]=Comm_Ram->TYMDHMS[4];//当前月
				p8[2]=Comm_Ram->TYMDHMS[5];//当前年
				DMW((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3);//直接存储器写(无擦除)

				y=p8[0]+(p8[1]<<8)+(p8[2]<<16);//当前日月年
				y=YMD_Sub1D(y);//年月日减1日,返回减1后的年月日
				y&=0xff;
				y--;
				x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3);
				x=YMD_Add1D(x);//年月日加1日,返回加1后的年月日
				if(x==(p8[0]+(p8[1]<<8)+(p8[2]<<16)))
				{
					x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y+3,4);
					if(x==0xffffffff)
					{
						DMW((u32)ADDR_AFN0DF321_Ext_Source,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y+3,4);//直接存储器写(无擦除)
					}
				}

				MC(0,ADDR_AFN0DF321_Ext_Source,4);
		#endif
			}
		}
//周更新
#if (USER/100)==3//用户标识:福建系
	#if WeekRMTask==1//每周抄表任务;0=没有,1=有
		if(UARTCtrl->Task==0)//0=空闲
		{
			if(Comm_Ram->TYMDHMS[6]==1)
			{//是星期一
				if(Comm_Ram->TYMDHMS[2]>=0x15)
				{//15点开始
					x=UARTCtrl->RTCW[0]+(UARTCtrl->RTCW[1]<<8);//原月日
					y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//当前月日
					if(x!=y)
					{
						UARTCtrl->RTCW[0]=y;
						UARTCtrl->RTCW[1]=y>>8;
						UARTCtrl->RMTaskN|=4;//新每周任务
						ClrRMTaskCompletes_RS485(PORTn,2,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
					}
				}
			}
		}
	#endif
#endif

//秒更新
		if(UARTCtrl->Task==0)
		{
			if(UARTCtrl->RTCS!=Comm_Ram->TYMDHMS[0])
			{
				UARTCtrl->RTCS=Comm_Ram->TYMDHMS[0];
				if(MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)//命令指定通信端口暂停抄表
				{
					UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
					p16timer[0]=0;//抄表间隔时间
				}
			#if DateRMTask==1//每日抄表任务;0=没有,1=有
				Terminal_AFN0CF11_RS485(PORTn,1);//终端类1数据F11,终端集中抄表状态信息;入口要计算的抄表任务号
			#else
				Terminal_AFN0CF11_RS485(PORTn,MaxRMTask-1);//终端类1数据F11,终端集中抄表状态信息;入口要计算的抄表任务号
			#endif
			//定时采集停电记录任务
			#if TimerRMTask==1//定时抄读停电记录任务;0=没有,1=有
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
				{//循抄任务
					i=MRR(ADDR_AFN04F97,1);//停电数据采集配置参数
					i&=1;
					if(i!=0)
					{//要采集
						i=MRR(ADDR_AFN04F97+1,1);//停电事件抄读时间间隔	BIN	1	单位为小时
						if(i!=0)
						{//抄读时间间隔!=0
							p16=(u16*)(&Terminal_Ram->RS4851RMF246_M_Timer);//25 RS485-1停电事件抄读时间间隔分定时器
							if(p16[RS485PORT-1]==0)
							{
								UARTCtrl->RMTaskN|=8;//新定时抄停电记录任务
							}
							i=MRR(ADDR_TYMDHMS+2,4);//当前YMDH
							x=MRR(((u32)&UARTCtrl->NextTimerRMTime)+2,4);
							if(i>=x)
							{
								UARTCtrl->RMTaskN|=8;//新定时抄停电记录任务
							}
							if(UARTCtrl->RMTaskN&8)
							{
								ClrRMTaskCompletes_RS485(PORTn,3,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
								i=MRR(ADDR_AFN04F97+1,1);//停电事件抄读时间间隔	BIN	1	单位为小时
								p16[RS485PORT-1]=i*60;//分
								x=MRR(ADDR_TYMDHMS+2,4);
								x=YMDH_AddH(x,i);//年月日时加h时,返回加h时后的年月日时
								d64a=x;
								d64a<<=16;//
								MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//定时抄停电记录间隔周期下次抄表时间秒分时日月
	
								i=MRR(ADDR_AFN04F97+2,1);//停电事件抄读时间限值	BIN	1	单位为分钟
							#if (USER/100)==13//黑龙江专变
								//为了跑黑龙江台体无效停上电时快点产生上电事件
								i=90;
							#else
								i*=60;
							#endif
								p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1停电事件抄读时间限值秒定时器
								p16[RS485PORT-1]=i;
							}
						}
					}
				}
			#endif
				//插入高优先级抄表
				NewTask_RS485(PORTn);//RS485口执行新任务;返回：0=没执行新任务，1=执行新任务
			#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江						
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						x=0;
						break;
					case RS485_2PORT://RS485-2
						x=1;
						break;
				}
				x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
				switch(x)
				{
					case 0:
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								i&=0x02;
								break;
							case RS485_2PORT://RS485-2
								i&=0x04;
								break;
						}
						if(i)
						{
							if(MRR(ADDR_AFN0CF249_Ext+9,1)!=0)
							{//终端精确对时成功
								if(MRR(ADDR_AFN04F298_Ext,1)!=0)
								{//电表对时开关开启
									if(MRR(ADDR_AFN04F298_Ext+6,1))
									{//允许对时次数不为0
										//重新抄表
										if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
										{
											switch(PORTn)
											{
												case RS485_1PORT://RS485-1
													i=0;
													break;
												case RS485_2PORT://RS485-2
													i=1;
													break;
											}
											MWR(1,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
											goto Re;
										}
									}
								}
							}
						}
						break;
					case 3:
						if(UARTCtrl->BroadcastTime<3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						{
							//重新抄表
							if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							{
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i=0;
										break;
									case RS485_2PORT://RS485-2
										i=1;
										break;
								}
								MWR(4,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
								i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i&=0xfd;
										break;
									case RS485_2PORT://RS485-2
										i&=0xfb;
										break;
								}
						Re:
							#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
								ClrRMEvene(PORTn);//清读电能表事件
							#endif
								p16timer[0]=0;//抄表间隔定时
								for(i=1;i<MaxRMTask;i++)
								{//抄表任务从1到MaxRMTask-1
									ClrRMTaskCompletes_RS485(PORTn,i,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
								}
								UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
								UARTCtrl->RMTaskN=0x02;//新任务b0-b7分别表示任务0-7有新任务
								UARTCtrl->RMTaskC=(MaxRMTask-1);//循抄时才能插入高优先级
								UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
								UARTCtrl->FnCount=0;//485抄表Fn计数
								UARTCtrl->RMCount=0;//485抄表命令计数
								UARTCtrl->ReRMCount=0;//485抄表命令重发计数
								UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
								UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
								//集中抄表信息
								p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
								p8+=(19*(RS485PORT-1));
								MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
							}
						}
						break;
				}
			#endif	
			}
		}


//自动搜表
	#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
		if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
		{
	#endif
			i=MRR(ADDR_AFN05F149+2+(RS485PORT-1),1);//电能表通信参数自动维护功能开启与关闭
			switch(i)
			{
				default:
					if(UARTCtrl->Task==0)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
					}
					break;
				case 0x55://收到此命令
					if(UARTCtrl->Task==0)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						UARTCtrl->RMCount=0;//抄表数据标识计数
						UARTCtrl->ReRMCount=0;//485抄表命令重发计数
						MWR(0x5a,ADDR_AFN05F149+2+(RS485PORT-1),1);//立即执行
					#if (USER/100)!=5//不是上海
						MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+((RS485PORT-1)*LEN_PerPortAutoSearchMeter),7);//从0地址开始搜表,DL645-2007速率3=2400
					#else
						//上海默认的速率4800
						MWR(0x04aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+((RS485PORT-1)*LEN_PerPortAutoSearchMeter),7);//从0地址开始搜表,DL645-2007速率3=2400
					#endif
						UARTCtrl->LockFlags|=6;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
			#if (USER/100)==6//用户:湖南
						i=MRR(ADDR_AFN05F103+1,2);
						i++;//加1分
						Terminal_Ram->AutoSearchMeterTimer=i;//2 自动搜表允许时间分定时器
			#endif
						return;
					}
					else
					{//等待空闲
						if((UARTCtrl->LockFlags&2)!=0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
						{
							AutoSearchMeter_RS485_DL645(PORTn);//自动搜表RS485端口的DL645
							return;
						}
					}
					break;
				case 0x5a://立即执行
		#if (USER/100)==6//用户:湖南
					if(Terminal_Ram->AutoSearchMeterTimer==0)//2 自动搜表允许时间分定时器
					{
						if(UARTCtrl->Task==0)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						{
							i=MRR(ADDR_AFN05F103,1);
							if(i==0x55)
							{//是F103启动,不每日执行
								MWR(0,ADDR_AFN05F149+2+(RS485PORT-1),1);//电能表通信参数自动维护功能开启与关闭
							}
							UARTCtrl->RMCount=0;//抄表命令计数=0
							UARTCtrl->ReRMCount=0;//抄表命令重发计数
							UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
							MC(0,Get_ADDR_UARTn(PORTn),6);//请fifo指针
							UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=菜单操作,B1=自动搜表,B2-B7保留
							return;
						}
					}
		#endif
					UARTCtrl->LockFlags|=6;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
					AutoSearchMeter_RS485_DL645(PORTn);//自动搜表RS485端口的DL645
					return;
				case 0xaa://每日执行
					if((UARTCtrl->LockFlags&2)==0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
					{//没启动自动搜表,每日在抄表结束后启动
						if((UARTCtrl->LockFlags&4)==0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
						{//本日已自动搜表==0
							if((Comm_Ram->TYMDHMS[2]>=0x20)&&(Comm_Ram->TYMDHMS[2]<0x23))
							{//每日在20:00-23:00启动
								if(UARTCtrl->RMComplete&0x80)//485抄表D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
								{
									if(UARTCtrl->Lock!=0x55)
									{
										UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
										UARTCtrl->RMCount=0;//抄表数据标识计数
										UARTCtrl->ReRMCount=0;//485抄表命令重发计数
										UARTCtrl->LockFlags|=6;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
										return;
									}
								}
							}
						}
					}
					else
					{//自动搜表
						//if(UARTCtrl->RMKeyUser==0)//抄读重点户标志,0=常规抄读,1=重点户抄读
						//{//不是重点户抄读
							AutoSearchMeter_RS485_DL645(PORTn);//自动搜表RS485端口的DL645
							if((Comm_Ram->TYMDHMS[2]<0x20)||(Comm_Ram->TYMDHMS[2]>=0x23))
							{//不是每日在20:00-23:00
								if(UARTCtrl->Lock!=0x55)
								{
									UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
								}
							}
							return;
						//}
					}
					break;
			}
	#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
		}
	#endif
	
	}

//命令指定通信端口重新抄表
	if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		if(MRR(ADDR_AFN05F51+RS485PORT,1)==0x55)//命令指定通信端口重新抄表
		{
			MC(0,ADDR_AFN05F49+RS485PORT,1);//清命令指定通信端口暂停抄表
			MC(0x5a,ADDR_AFN05F51+RS485PORT,1);//启动重新抄表后0x5A
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			ClrRMEvene(PORTn);//清读电能表事件
		#endif
			p16timer[0]=0;//抄表间隔定时
			for(i=1;i<MaxRMTask;i++)
			{//抄表任务从1到MaxRMTask-1
				ClrRMTaskCompletes_RS485(PORTn,i,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
			}
			UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
			UARTCtrl->RMTaskN=0x02;//新任务b0-b7分别表示任务0-7有新任务
			UARTCtrl->RMTaskC=(MaxRMTask-1);//循抄时才能插入高优先级
			UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
			UARTCtrl->FnCount=0;//485抄表Fn计数
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
			UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			//集中抄表信息
			p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
			p8+=(19*(RS485PORT-1));
			MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
		}
	}
	
//执行广播校时
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
#endif
		if(UARTCtrl->BroadcastTime>=3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
		{
			Terminal_BroadcastTime_DL645(PORTn);//DL645广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			return;
		}
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	}
#endif

//抄表
	if(UARTCtrl->RMFlag!=0x0)
	{//1=抄表进行中
		if(PORTn!=ACSAMPLEPORT)
		{
			p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
			p8+=(19*(RS485PORT-1));
			p8[4]|=0x1;//D0=1,抄表
		}
	}
	else
	{//0=抄表间隔暂停
		if(PORTn!=ACSAMPLEPORT)
		{
			p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
			p8+=(19*(RS485PORT-1));
			p8[4]&=0xfe;//D0=0,未抄表
			if(MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)//命令指定通信端口暂停抄表
			{
				CopyString((u8 *)"485 命令暂停抄表",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			}
			else
			{
				CopyString((u8 *)"485 抄表间隔暂停",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			}
			MWR(RS485PORT,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
		}
		UARTCtrl->RMTaskFlags=0;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	#if (USER/100)==5//上海
		if(RS485PORT!=0)
		{//不是内部交流采样通信口
			if(p16timer[4]==0)
			{//曲线延时抄表定时到
				p16timer[4]=0xffff;//下间隔定时
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);
				i=hex_bcd(i);
				x=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);//时分
			#if ((Project/100)==2)
				if(x<i)
				{//过0点后的第1个抄表周期内
					UARTCtrl->RMTaskFlags|=0xfe;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
					UARTCtrl->RMFlag=1;//1=抄表进行中
				}
			#else
				if(x<i)
				{//过0点后的第1个抄表周期内
					UARTCtrl->RMTaskFlags|=0xfe;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				}
				else
				{
					UARTCtrl->RMTaskFlags|=0x04;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				}
				UARTCtrl->RMFlag=1;//1=抄表进行中
			#endif
			}
		}
	#endif
		i=DataComp(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		if((i!=2)||(p16timer[0]==0))
		{//相等或TYMDHMS>NextRMTime
			//抄表标志
			UARTCtrl->RMTaskFlags|=0xfb;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
		#if (USER/100)==5//上海
			if(RS485PORT!=0)
			{//不是内部交流采样通信口
				//设延时抄表定时
				i=MRR(ADDR_AFN04F242,1);//终端对电能表抄表机制参数(分)
				x=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);//分
				if(i>=x)
				{//延时抄表定时分>=抄表周期
					i=x;
					if(i!=0)
					{//至小小1分
						i--;
					}
				}
				i*=60;
				i+=2;//多2秒免过0时同时
				p16timer[4]=i;//延时抄表秒定时器
			}
		#endif
		}
		if(UARTCtrl->RMTaskFlags==0)
		{//定时没到
		/*
			//集中抄表信息开始结束时间
			p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
			p8+=(19*(RS485PORT-1));
			if(p8[14]==0xee)
			{//无结束时间
				MR(((u32)p8)+14,ADDR_AFN0CF2,6);
			}
		*/
			return;
		}
		if((MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//命令指定通信端口暂停抄表
		{
			return;
		}
		UARTCtrl->RMFlag=1;//1=抄表进行中
		if(RS485PORT!=0)
		{//不是内部交流采样通信口
			if(UARTCtrl->RMTaskFlags&0x1)
			{//是常规抄定时到
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);//分
				if(i==0)
				{
					i=1;//最小1分
				}
				d64a=MRR(ADDR_TYMDHMS+1,5);
				d64a=YMDHM_AddM(d64a,i);//年月日时分加m分,返回加m分后的年月日时分
				if((60%i)==0)
				{//间隔时间的整数倍是60分
					//同步到整间隔时间分值
					x=d64a;
					x&=0xff;
					x=bcd_hex(x);
					x/=i;
					x*=i;
					x=hex_bcd(x);
					d64a>>=8;
					d64a<<=8;
					d64a|=x;
				}
				d64a<<=8;//秒值=0
				MWR(d64a,(u32)&UARTCtrl->NextRMTime,6);
				
				i*=60;
				i+=2;//秒加2,免在59秒定时到,同步后又归0,这会少1分
				p16timer[0]=i;//终端抄表间隔
			/*
				//集中抄表信息开始结束时间
				Terminal_AFN0CF2_Computer();//终端类1数据F2,终端日历时钟
				p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
				p8+=(19*(RS485PORT-1));
				MR(((u32)p8)+8,ADDR_AFN0CF2,6);//开始时间
				MC(0xee,((u32)p8)+14,6);//结束时间
			*/
			}
		}
		else
		{//是内部交流采样通信口
			p16timer[0]=1;//抄表间隔时间固定为1s
		}
	}

	if(UARTCtrl->FnEnd==0)//485抄表Fn计数结束标志,0=结束,1=没结束
	{
		if(AFN04F10MaxPn>100)
		{
			y=100;//本次最大查找100只表
		}
		else
		{
			y=AFN04F10MaxPn;
		}
		for(x=0;x<y;x++)
		{
			UARTCtrl->MeterNo++;
			if(UARTCtrl->MeterNo>AFN04F10MaxPn)
			{
				//全部表1轮抄完
			#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江						
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
				{//循抄任务结束
					switch(PORTn)
					{
						case RS485_1PORT://RS485-1
							i=0;
							break;
						case RS485_2PORT://RS485-2
							i=1;
							break;
					}
					i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
					switch(i)
					{
						case 1:
							switch(PORTn)
							{
								case RS485_1PORT://RS485-1
									i=0;
									break;
								case RS485_2PORT://RS485-2
									i=1;
									break;
							}
							MWR(2,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
							break;
						case 4:
							switch(PORTn)
							{
								case RS485_1PORT://RS485-1
									i=0;
									break;
								case RS485_2PORT://RS485-2
									i=1;
									break;
							}
							MWR(5,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
							break;
					}
				}
			#endif	

			#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			#if (USER/100)==5//上海;电能表事件采集方案另定义
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
				{//是循抄
					if(UARTCtrl->RMTaskFlags&1)//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
					{//是抄实时
						//每个抄表周期数据项采集完成后,抄1级事件
						UARTCtrl->RMEventTaskN|=1;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
						//每天第1轮抄完,抄2级事件
						if((UARTCtrl->RMEventTaskN&0x20)!=0)
						{
							UARTCtrl->RMEventTaskN&=~0x20;
							UARTCtrl->RMEventTaskN|=2;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3,;b6=月更新(上海每月抄事件用)
						}
						//每月第1轮抄完,抄3级事件
						if((UARTCtrl->RMEventTaskN&0x40)!=0)
						{//月更新
							UARTCtrl->RMEventTaskN&=~0x40;
							UARTCtrl->RMEventTaskN|=4;
						}
					}
				}
			#endif
			#endif
				UARTCtrl->MeterNo=0;//完成1个循环抄表,电能表/交流采样装置序号=0
				if(RS485PORT==0)
				{//是内部交流采样通信口
					i=UARTCtrl->RMComplete;
					if((i&0x0f)<0x0f)
					{
						i+=1;//完成循环抄表次数+1
					}
					i|=0x80;
					UARTCtrl->RMComplete=i;
					UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
				}
				else
				{//是外部485口
					if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
					{//循抄任务结束
						i=UARTCtrl->RMComplete;
						if((i&0x0f)<0x0f)
						{
							i+=1;//完成循环抄表次数+1
						}
						i|=0x80;
						UARTCtrl->RMComplete=i;
					#if DateRMTask==0//每日抄表任务;0=没有,1=有
						//暂停前填终端集中抄表状态信息,因会清0完成标志
						Terminal_AFN0CF11_RS485(PORTn,MaxRMTask-1);//终端类1数据F11,终端集中抄表状态信息;入口要计算的抄表任务号
					#endif
						ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-1,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
						UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
					}
					else
					{//是插入高优先级抄表结束
						if(UARTCtrl->RMTaskC!=0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						{//不是上电
							//恢复循抄
							UARTCtrl->FnCount=UARTCtrl->FnCountS;//485抄表原循抄任务的Fn计数
							i=UARTCtrl->MeterNoS;//485抄表原循抄任务的配置序号计数(半字对齐)
							if(i!=0)
							{
								i--;
							}
							UARTCtrl->MeterNo=i;
							//立即补抄每日任务
							if(UARTCtrl->DateReRMCount<2)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
							{
								i=NoRMCompletes_RS485(PORTn,1);//RS485未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
								if(i!=0)
								{//需补抄
									UARTCtrl->MeterNo++;
									UARTCtrl->RMTaskN|=2;//新每日任务
								}
							}
						#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
							if(UARTCtrl->RMTaskC==(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
							{
								DelRMEvene(PORTn);//删除读电能表事件任务
								if(UARTCtrl->RMTaskC==(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
								{//恢复后还是抄读上低1级的电能表事件
									return;
								}
							#if (USER/100)==5//上海;电能表事件采集方案另定义
							//上海在循抄结束后抄事件,事件结束后需暂停
								UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
							#endif
							}
						#endif
						}
						if((UARTCtrl->RMTaskC==0)||(UARTCtrl->RMTaskC==3))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						{//是抄停电记录任务结束
							if((Terminal_Ram->PowerTask==1)||(Terminal_Ram->PowerTask==10))//10=等待抄表完成比对电能表
							{//事件任务:等待抄表完成比对电能表
								//i=NoRMCompletes_RS485(PORTn,UARTCtrl->RMTaskC);//RS485未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
								//if(i!=0)
								//{//没全抄成功
									if(UARTCtrl->RMTaskC==0)
									{
										if(p16timer[8]!=0)//停电事件抄读时间限值秒定时器
										{
											ClrRMTaskCompletes_RS485(PORTn,0,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
											return;
										}
									}
								//}
							}
							//下次抄停电记录间隔时间
							i=MRR(ADDR_AFN04F97+1,1);//停电事件抄读时间间隔	BIN	1	单位为小时
							p16=(u16*)(&Terminal_Ram->RS4851RMF246_M_Timer);//25 RS485-1停电事件抄读时间间隔分定时器
							p16[RS485PORT-1]=i*60;//分
							x=MRR(ADDR_TYMDHMS+2,4);
							x=YMDH_AddH(x,i);//年月日时加h时,返回加h时后的年月日时
							d64a=x;
							d64a<<=16;//
							MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//定时抄停电记录间隔周期下次抄表时间秒分时日月年
						}
						UARTCtrl->RMTaskC=(MaxRMTask-1);//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
					}
					NewTask_RS485(PORTn);//RS485口执行新任务;返回：0=没执行新任务，1=执行新任务
				}
				return;
			}
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
			//if(i==MRR(ADDR_Protocol_No,1))
			if(i==2)
			{//交流采样装置通信协议
				if(PORTn==ACSAMPLEPORT)
				{//当前口是内部采样口
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//所属测量点号 BIN 2 
					if((i!=0x0)&&(i<=MaxRS485AddCarrierPn))
					{//所属测量点号!=0x0
						//启动发送
						UARTCtrl->FnEnd=1;//485抄表Fn计数结束标志,0=结束,1=没结束
						return;
					}
				}
			}
			else
			{
				if(PORTn!=ACSAMPLEPORT)
				{//当前口不是内部采样口
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//通信速率及端口号
					i&=0x1f;
					if(i==(RS485PORT+1))
					{//通信端口号相同
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//所属测量点号 BIN 2 
						if((i!=0x0)&&(i<=MaxRS485AddCarrierPn))
						{//所属测量点号!=0x0
							//启动发送
							UARTCtrl->FnEnd=1;//485抄表Fn计数结束标志,0=结束,1=没结束
							return;
						}
					}
				}
			}
		}
		return;
	}
	if((Terminal_Ram->CongealFlags&0x1f)==0x1f)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	{//抄表在冻结后进行
		ReadMeter_Protocol(PORTn);//抄表规约
	}
}



















