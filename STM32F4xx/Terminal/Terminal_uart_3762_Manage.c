
#include "Project.h"
#include "Terminal_Uart_3762_Manage.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "Terminal_ReadMeter_3762.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_ReadMeter_3762_Std.h"
#include "AutoSearchMeter_3762.h"
#include "BatchTask_3762.h"
#include "KeyUser.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_Event.h"
#include "RouterBroadcastTime_Collector.h"



#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void Terminal_AFN0CF11_3762(void)//终端类1数据F11,终端集中抄表状态信息
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u8 *p8;

#if DateRMTask==1//每日抄表任务;0=没有,1=有
	i=MRR(ADDR_TYMDHMS+1,2);
	if(i<RMM_StartHM)
	{
		return;
	}
#endif
	KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
	n=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	x=0;//成功数
	y=0;//失败数
	w=0;//重点户成功数
	for(i=0;i<n;i++)
	{
	#if DateRMTask==0//每日抄表任务;0=没有,1=有
		z=Terminal_Note->List[i].RMCompleteS[MaxRMTask-1];//记循抄任务号
	#else
		z=Terminal_Note->List[i].RMCompleteS[1];//只记任务号1
	#endif
		if(z==2)
		{
			x++;//成功数
			if(KeyUserPnSet(Terminal_Note->List[i].Pn))//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
			{
				w++;//重点户抄表成功数
			}
		}
		else
		{
			if(z!=0)
			{
				y++;//失败数
			}
		}
	}

	CopyString((u8 *)"载波____成____败____",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
	//总数
	p8=(u8*)(ADDR_DATABUFF+4+3);
	p8[0]='0';
	i=hex_bcd(n);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	//成功数
	p8=(u8*)(ADDR_DATABUFF+10+3);
	p8[0]='0';
	i=hex_bcd(x);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}	
	//失败数
	p8=(u8*)(ADDR_DATABUFF+16+3);
	p8[0]='0';
	p8[1]=0;
	i=hex_bcd(y);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	WarningOccur(100,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
	
	p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
	p8[0]=3;//本项数据块个数n	BIN	1(1-31) 
	p8[1]=2;//端口号2
	p8[19+1]=3;//端口号3
	p8[38+1]=31;//终端通信端口号	BIN	1
	p8+=(2*19);
	MWR(n,((u32)p8)+2,2);//要抄电表总数	BIN	2
	if((x==n)&&(n!=0))
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
	MWR(x,((u32)p8)+5,2);//抄表成功块数	BIN	2
	MWR(w,((u32)p8)+7,1);//抄重点表成功块数	BIN	1

	//暂停路由标志:
	//b0=抄表间隔定时,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
	//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
	//b16=路由软件更新,b17=,b18=,b19=,b20=,b21=,b22=,b23=
	if(((Terminal_Router->StopFlags&0xFFFFFF7D)==0)&&(Terminal_Router->RouterInitTask>=200)&&(Terminal_Router->NoteNum!=0))
	{
		p8[4]|=1;//D0=1,抄表
		//抄表开始时间	见附录A.1	6
		if(p8[8+3]==0xee)
		{//无开始时间
			MR(((u32)p8)+8,ADDR_AFN0CF2,6);
		}
	}
	else
	{
		p8[4]&=0xfe;//D0=0,未抄表
	}
	//抄表结束时间	见附录A.1	6
	
/*	
	//显示无线信道
	if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{
		CopyString((u8 *)"信道组------发射功率",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		p8=(u8*)(ADDR_DATABUFF);
		p8[20]=0;
		i=Terminal_Router->RadioChannel;
		i=hex_bcd(i);
		p8[6]=((i>>4)&0xf)+0x30;
		p8[7]=(i&0xf)+0x30;
		i=Terminal_Router->RadioPower;//无线主节点发射功率:00：最高发射功率，01：次高发射功率，02：次低发射功率，03：最低发射功率，04～254：保留，255：保持不变
		switch(i)
		{
			case 0://最高发射功率
				CopyString((u8 *)"最高",(u8*)ADDR_DATABUFF+8);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 1://次高发射功率
				CopyString((u8 *)"次高",(u8*)ADDR_DATABUFF+8);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 2://次低发射功率
				CopyString((u8 *)"次低",(u8*)ADDR_DATABUFF+8);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 3://最低发射功率
				CopyString((u8 *)"最低",(u8*)ADDR_DATABUFF+8);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
		}
		WarningOccur(101,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
	}
*/
}


u32 NoRMCompletes_3762(u32 RMTask)//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	
	n=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
	if(RMTask!=0)
	{//不是启抄
		if(n==0)
		{//已知总从节点数=0
			return 1;//返回b0-b15未抄数!=0以备配置表后即开始日抄
		}
	}
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	x=0;//未抄数
	y=0;//失败数
	for(i=0;i<n;i++)
	{
		z=Terminal_Note->List[i].RMCompleteS[RMTask];//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
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
	return (x+(y<<16));
}

void ClrRMTaskComplete_3762(u32 RMTask,u32 All)//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
{
	u32 i;
	u32 n;
	
	n=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	for(i=0;i<n;i++)
	{
		if(All==0)
		{//只清失败
			if(Terminal_Note->List[i].RMCompleteS[RMTask]==1)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
			{
				Terminal_Note->List[i].RMCompleteS[RMTask]=0;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
			}
		}
		else
		{//清全部
			Terminal_Note->List[i].RMCompleteS[RMTask]=0;
		}
	}
}

u32 NewTask_3762(u32 PORTn)//3762口执行新任务;返回：0=没执行新任务，1=执行新任务
{
	u32 i;
	u32 n;
  UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	u32 x;
	u32 y;
	u16 *p16timer;
	
	if(Terminal_Router->RouterInitTask<200)
	{
		return 0;
	}
	if(Terminal_Router->RouterCtrl!=0)//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
	{
		return 0;
	}
	if(Terminal_Router->StopFlags&0xfffffffc)//暂停路由标志:b0=抄表间隔定时,b1=抄重点户
	{//路由有非抄表类暂停
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
	{
		return 0;
	}
	if(((UARTCtrl->RMMeterEvent&1)!=0)||(UARTCtrl->SubsequentFrames!=0))
	{//正在抄读电能表主动上报事件或读后续帧
		return 0;
	}
	
	n=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	if(UARTCtrl->RMTaskN&0x10)//有每小时曲线抄读任务
	{//有新每小时曲线抄读任务,采用点抄
		if((UARTCtrl->RMTaskC!=0)&&(UARTCtrl->RMTaskC!=4)&&(UARTCtrl->RMCount==0))
		{//原任务不是启抄或4=每小时曲线任务
			UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
			UARTCtrl->RMTaskN&=0xef;//去新任务
			UARTCtrl->RMTaskCOfKeyUser=UARTCtrl->RMTaskC;
			UARTCtrl->RMTaskC=4;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].RMCompleteS[4]=0;
			}
			UARTCtrl->MeterNoBackupOfKeyUser=UARTCtrl->MeterNo;//485抄表原循抄任务的配置序号计数(半字对齐)
			UARTCtrl->FnCountBackupOfKeyUser=UARTCtrl->FnCount;//485抄表原循抄任务的Fn计数
			UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
			UARTCtrl->FnCount=0;//485抄表Fn计数
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].FnCountSOfKeyUser=Terminal_Note->List[i].FnCount;//插入每小时抄读曲线任务前原循抄任务的抄表Fn计数
				Terminal_Note->List[i].FnCount=0;//抄表Fn计数
				Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
				Terminal_Note->List[i].ReRMCount=0;//重发计数
				Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
			}
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
			return 1;
		}
	}
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
	{//只有循抄时或抄读电能表事件时才能插入高优先级
		if(UARTCtrl->DateReRMCount!=0)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
		{
			NewRMEvene(PORTn);//新或恢复读电能表事件任务;返回0=没新进入,1=新进入
		}
	}
#endif
	if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
	{//循抄时才能插入高优先级
		if(UARTCtrl->RMTaskN&2)//新每日任务或补抄任务
		{
		ReTask:
			UARTCtrl->RMTaskN&=0xfd;//去新每日任务
			UARTCtrl->RMTaskC=1;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			if(UARTCtrl->DateReRMCount!=0xff)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
			{
				UARTCtrl->DateReRMCount++;//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
			}
			for(i=0;i<n;i++)
			{
				if(Terminal_Note->List[i].RMCompleteS[1]==1)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
				{//只清不成功的
					Terminal_Note->List[i].RMCompleteS[1]=0;
				}
			}
		NewTask:
			UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
			UARTCtrl->MeterNoS=UARTCtrl->MeterNo;//485抄表原循抄任务的配置序号计数(半字对齐)
			UARTCtrl->FnCountS=UARTCtrl->FnCount;//485抄表原循抄任务的Fn计数
			UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
			UARTCtrl->FnCount=0;//485抄表Fn计数
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].FnCountS=Terminal_Note->List[i].FnCount;//插入任务前原循抄任务的抄表Fn计数
				Terminal_Note->List[i].FnCount=0;//抄表Fn计数
				Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
				Terminal_Note->List[i].ReRMCount=0;//重发计数
				Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
			}
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
			//Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
			return 1;
		}
		if(UARTCtrl->RMTaskN&4)//新每周任务
		{//采用点抄
			UARTCtrl->RMTaskN&=0xfb;//去新每周任务
			UARTCtrl->RMTaskC=2;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			ClrRMTaskComplete_3762(2,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
			goto NewTask;
		}
		if(UARTCtrl->RMTaskN&8)//新定时抄读停电记录任务
		{//采用点抄
			UARTCtrl->RMTaskN&=0xf7;//去新定时抄读停电记录任务
			UARTCtrl->RMTaskC=3;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			goto NewTask;
		}
	}
	else
	{//恢复
		y=NoRMCompletes_3762(UARTCtrl->RMTaskC);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
		if((y&0xffff)==0)//未抄数=0
		{//当前任务完成
			//恢复
			UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
			if(UARTCtrl->RMTaskC!=0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			{//不是上电启抄
				//恢复
				if(UARTCtrl->RMTaskC!=4)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=每小时曲线任务,(MaxRMTask-1)=循抄任务
				{//不是每小时抄重点户曲线恢复
					i=UARTCtrl->MeterNoS;//485抄表原循抄任务的配置序号计数(半字对齐)
					if(i!=0)
					{
						i--;
					}
					UARTCtrl->MeterNo=i;
					UARTCtrl->FnCount=UARTCtrl->FnCountS;//485抄表原循抄任务的Fn计数
					UARTCtrl->RMCount=0;//485抄表命令计数
					UARTCtrl->ReRMCount=0;//485抄表命令重发计数
					UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
					for(i=0;i<n;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountS;//插入任务前原循抄任务的抄表Fn计数
						Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
						Terminal_Note->List[i].ReRMCount=0;//重发计数
						Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					}
				#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					DelRMEvene(PORTn);//删除读电能表事件任务
					if(UARTCtrl->RMTaskC==(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
					{//恢复后还是抄读上低1级的电能表事件
						return 1;
					}
					if(UARTCtrl->DateReRMCount!=0)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
					{//至少已抄1轮
						y=NoRMCompletes_3762(1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
						x=y;
						x&=0xffff;
						y>>=16;//失败数
						y+=x;//加未抄数
						x=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
						if(x!=0)
						{
							y*=100;
							y/=x;//失败率
						}
					#if (USER/100)==5//上海
						if(y<=2)//抄表成功率>=98%
						{
							y=0;//失败率=0
						}
					#endif
						x=Comm_Ram->TYMDHMS[2];//实时钟的时
					#if (USER/100)==11//吉林系
						if(x>=0x09)
						{
							if(y<=2)//抄表成功率>=98%
							{
								x=0x12;//时钟等同12点
								y=0;//失败率=0
							}
						}
						else
						{
							y=1;//失败率=1
						}
					#endif
						if((y==0)||(x>=0x12))
						{//抄表成功率100%或时钟>=12点
							UARTCtrl->RMTaskC=(MaxRMTask-1);//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
						#if((USER/100)==5)//上海
							RMEveneTimer_ShH(PORTn,y);//上海集中器分级读电能表事件定时
						#endif
							NewRMEvene(PORTn);//新或恢复读电能表事件任务;返回0=没新进入,1=新进入
							if(UARTCtrl->RMTaskC==(MaxRMTask-2))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
							{//又新进入电能表事件
								return 1;
							}
						}
					}
				#endif
					if(UARTCtrl->RMTaskN&4)//新每周任务
					{//采用点抄
						UARTCtrl->RMTaskN&=0xfb;//去新每周任务
						UARTCtrl->RMTaskC=2;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
						ClrRMTaskComplete_3762(2,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
						goto NewTask;//在此进入,不进入循抄免重启路由而影响路由建立
					}
					if(UARTCtrl->RMTaskN&8)//新定时抄读停电记录任务
					{//采用点抄
						UARTCtrl->RMTaskN&=0xf7;//去新定时抄读停电记录任务
						UARTCtrl->RMTaskC=3;//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
						goto NewTask;//在此进入,不进入循抄免重启路由而影响路由建立
					}
					y=NoRMCompletes_3762(1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					y>>=16;//失败数
				#if((USER/100)!=5)//上海用户
					if(y!=0)
					{//有失败,立即补抄,不进入循抄免重启路由而影响路由建立
					#if (USER/100)!=0//微功率无线电科院送样不重新组网,以免影响抄表成功率测试
						//不是微功率无线电科院送样
						if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{//微功率无线
							if((Terminal_Router->RUNState&1)!=0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
							{//当前不是正在组网
								if((Comm_Ram->TYMDHMS[2]<=6)||(Comm_Ram->TYMDHMS[2]>=0x20))
								{//在时钟0-6点或20点-24点启动重新组网
									Terminal_Router->RouterCtrl=6;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
									return 0;
								}
							}
						}
					#endif
						UARTCtrl->MeterNo++;
						goto ReTask;
					}
				#else
					//上海需要进入循抄,其他地区若日冻结有失败，立即补抄,不进入循抄免重启路由而影响路由建立
					if(UARTCtrl->RMTaskC==(MaxRMTask-1))
					{//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
						x=NoRMCompletes_3762(MaxRMTask-1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
						if(x==0)
						{//全部抄成功
							ClrRMTaskComplete_3762(MaxRMTask-1,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
						}
						else
						{
							x&=0xffff;
							if(x==0)
							{//全部已抄但有失败
								ClrRMTaskComplete_3762(MaxRMTask-1,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
								goto SupplementRM;
							}
						}
					}
					else
					{
					SupplementRM:	
						if(y!=0)
						{//有失败,立即补抄,不进入循抄免重启路由而影响路由建立
						#if (USER/100)!=0//微功率无线电科院送样不重新组网,以免影响抄表成功率测试
							//不是微功率无线电科院送样
							if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{//微功率无线
								if((Terminal_Router->RUNState&1)!=0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
								{//当前不是正在组网
									if((Comm_Ram->TYMDHMS[2]<=6)||(Comm_Ram->TYMDHMS[2]>=0x20))
									{//在时钟0-6点或20点-24点启动重新组网
										Terminal_Router->RouterCtrl=6;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
										return 0;
									}
								}
							}
						#endif
							UARTCtrl->MeterNo++;
							goto ReTask;
						}
					}
				#endif
				}
				else
				{//是每小时抄重点户曲线恢复
					i=UARTCtrl->MeterNoBackupOfKeyUser;//485抄表原循抄任务的配置序号计数(半字对齐)
					if(i!=0)
					{
						i--;
					}
					UARTCtrl->MeterNo=i;
					UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485抄表原循抄任务的Fn计数
					UARTCtrl->RMCount=0;//485抄表命令计数
					UARTCtrl->ReRMCount=0;//485抄表命令重发计数
					UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
					for(i=0;i<n;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfKeyUser;//插入每小时抄读曲线任务前原循抄任务的抄表Fn计数
						Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
						Terminal_Note->List[i].ReRMCount=0;//重发计数
						Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					}
					UARTCtrl->RMTaskC=UARTCtrl->RMTaskCOfKeyUser;//进入重点户抄读任务前原任务
					UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
					return 0;
				}
			}
			else
			{//是上电启抄完成
				if((Terminal_Ram->PowerTask==1)||(Terminal_Ram->PowerTask==10))//10=等待抄表完成比对电能表
				{//事件任务:等待抄表完成比对电能表
					//i=NoRMCompletes_3762(UARTCtrl->RMTaskC);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					//if(i!=0)
					//{//没全抄成功
						p16timer=(u16*)((u32)&Terminal_Ram->RS4854RM_S_Timer);//20 RS485-4抄表秒定时器(载波)
						if(p16timer[8]!=0)//停电事件抄读时间限值秒定时器
						{
							ClrRMTaskComplete_3762(0,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
							return 0;
						}
					//}
				}
				//下次抄停电记录间隔时间
				i=MRR(ADDR_AFN04F97+1,1);//停电事件抄读时间间隔	BIN	1	单位为小时
				p16timer=(u16*)(&Terminal_Ram->RS4854RMF246_M_Timer);//25 RS485-1停电事件抄读时间间隔分定时器
				p16timer[0]=i*60;//分
				x=MRR(ADDR_TYMDHMS+2,4);
				x=YMDH_AddH(x,i);//年月日时加h时,返回加h时后的年月日时
				d64a=x;
				d64a<<=16;//
				MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//定时抄停电记录间隔周期下次抄表时间秒分时日月年
			#if DateRMTask==1//每日抄表任务;0=没有,1=有
				goto ReTask;//进入日任务,免进入1次循抄后再进入
			#endif
			}
			Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
			UARTCtrl->RMTaskC=(MaxRMTask-1);//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
		}
	}
	return 0;
}


void Terminal_uart_3762_Manage(void)//376.2路由模块管理
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u64 d64a;
	u8 *p8;
	u16 * p16;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 PORTn;
#if Extern_BatchTask_3762!=0//外部程序:0=I2,1=E1,2=E2,3=
	u32 *p32;
#endif
	
	PORTn=RS485_4PORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);


#if PowerOnRMTask==0//电源开抄表任务;0=没有,1=有
	if(UARTCtrl->RMTaskC==0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
	{
		UARTCtrl->RMTaskC=(MaxRMTask-1);
	}
#endif
	
//分更新
	if(UARTCtrl->RTCM!=Comm_Ram->TYMDHMS[1])
	{
		UARTCtrl->RTCM=Comm_Ram->TYMDHMS[1];
	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		RMEveneTimer(PORTn);//分级读电能表事件定时
	#if((USER/100)==5)//上海
		RMEveneTimer_ShH(PORTn,y);//上海集中器分级读电能表事件定时
	#endif
	#endif
		//广播校时
		i=MRR(ADDR_AFN05F30+1,1);//终端对电能表对时功能的开启与关闭
		if(i==1)
		{//0=关闭，1=开启，其它值关闭
			i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//终端抄表运行参数设置,端口31
			if((i&0x08)!=0x0)
			{//要求终端定时对电表广播校时
				x=MRR(ADDR_TYMDHMS+1,3);//取当前实时钟日时分
				y=MRR((ADDR_AFN04F33+((14+(4*24))*30))+10,3);//对电表广播校时定时时间日时分
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
		x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
		if(x==1)
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
							x=MRR(ADDR_AFN04F298_Ext,1);
							if((x==1)||(x==2))
							{//指定表地址或采集器地址的广播对时命令
								if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)<3)//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
								{
									MWR(3,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
								}
							}
							else
							{//全终端的广播对时命令
								UARTCtrl->BroadcastTime=3;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
							MWR(2,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
						}
					}
				}
			}
		}
	#endif	
	#if((USER/100)==5)//上海
		i=MRR(ADDR_AFN05F201,1);
		if(i&(1<<3))
		{
			if(UARTCtrl->BroadcastTime<3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			{
				UARTCtrl->BroadcastTime=3;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
		}
	#endif											    
	}
//时更新
	if(UARTCtrl->RTCH!=Comm_Ram->TYMDHMS[2])
	{
		UARTCtrl->RTCH=Comm_Ram->TYMDHMS[2];
		i=Comm_Ram->TYMDHMS[2];
		switch(i)//采集频度为每天2、6、10、14、18、22点
		{
			case 0x02:
			case 0x06:
			case 0x10:
			case 0x14:
			case 0x18:
			case 0x22:
			#if DateRMTask==1//每日抄表任务;0=没有,1=有
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
				{//当前是循抄
					i=NoRMCompletes_3762(1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					if(i!=0)
					{//需补抄
						UARTCtrl->RMTaskN|=2;//新每日任务
					}
				}
			#endif
	#if (((USER/100)==4)||((USER/100)==6)||((USER/100)==8)||((USER/100)==11))//用户标识:是陕西,湖南,通用市场,吉林
			//替代 周期抄读载波口07表的曲线
		#if WeekRMTask==1//每周抄表任务;0=没有,1=有(替代每日的时任务:三相表用户包括居民用户和工商业用户当前电压、电流、断相累计时间数据的采集)
			#if (((USER/100)==6))//用户标识:是湖南系
				if(UARTCtrl->DateReRMCount!=0)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
				{//至少已抄1轮
					y=NoRMCompletes_3762(1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					x=y;
					x&=0xffff;
					y>>=16;//失败数
					y+=x;//加未抄数
					x=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
					if(x!=0)
					{
						y*=100;
						y/=x;//失败率
					}
				#if (USER/100)==6//湖南
					if(y<=2)
					{
						y=0;//湖南抄表成功率>=98%
					}
				#endif
					x=Comm_Ram->TYMDHMS[2];//实时钟的时
					if((y==0)||(x>=0x14))
					{//抄表成功率100%或时钟>=14点
						UARTCtrl->RMTaskN|=4;//新每周任务
					}
				}
			#else//用户标识:是陕西,通用市场,吉林
				UARTCtrl->RMTaskN|=4;//新每周任务
			#endif

		#endif
	#endif
				break;
		}
	#if HourTask==1//每小时曲线抄读
		KeyUserPnList();//重点户曲线冻结Pn列表
		//判是否有重点户为单相表
		p16=(u16*)(ADDR_KeyUserPnList);
		for(i=0;i<MaxKeyUser;i++)
		{
			x=p16[i];//测量点号
			if(x!=0)
			{//是重点户
				x=Check_AFN04F10_Pn(x);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
				if(x!=0)
				{
			#if (USER/100)==8//通用版本（辽宁不判大小类号）
					break;
			#else
					x--;
					x=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*x)+26,1);//取大小类号
				#if (USER/100)==5//上海系
					if((x!=0x02)&&(x!=0x52)&&(x!=0x62)&&(x!=0x72))//大小类号=51或71,其他大小类号按51处理
				#else
					#if (USER/100)==11//吉林
					if((x==0x30)||(x==0x31)||(x==0x40)||(x==0x41)||(x==0x44)||(x==0x45))
					#else
					if((x==0x50)||(x==0x51)||(x==0x40)||(x==0x41))//湖南等其他
					#endif
				#endif
					{
						break;
					}
			#endif
				}
			}
		}
		if(i!=MaxKeyUser)
		{
			UARTCtrl->RMTaskN|=0x10;//新每小时曲线任务
		}
	#endif
	}
//日更新
	if((UARTCtrl->Task==0)&&(UARTCtrl->Lock!=0x55))//0=空闲
	{
		x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//原月日
		y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//当前月日
		if(x!=y)
		{
		#if ((USER/100)==5)
			UARTCtrl->RMEventTaskN|=0x20;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3;b5=日更新,b6=月更新(上海每日月抄事件用)
			if(UARTCtrl->RTCD[1]!=Comm_Ram->TYMDHMS[4])
			{
				//月更新
				UARTCtrl->RMEventTaskN|=0x40;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3;b5=日更新,b6=月更新(上海每日月抄事件用)
			}
		#endif
			UARTCtrl->RTCD[0]=y;
			UARTCtrl->RTCD[1]=y>>8;
			UARTCtrl->RMComplete&=0x7f;//485抄表D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			StoreRMEvene(PORTn);//暂存正在读电能表事件的任务
		#endif
	#if (USER/100)!=0
			//电科院测试过日不重初始化,免通信次数测试时返回否认,同时循抄标志不清0而继续原来循抄
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	#endif
			//每日任务
			UARTCtrl->DateReRMCount=0;//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
	#if DateRMTask==1//每日抄表任务;0=没有,1=有
			UARTCtrl->RMTaskN|=2;//新每日任务
			ClrRMTaskComplete_3762(1,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
			WarningCancel(100);//告警取消,入口告警代码
	#endif
			//清集中抄表信息
			p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
			p8+=(19*2);
			MC(0,((u32)p8)+4,4);
			MC(0xee,((u32)p8)+8,12);
			//自动搜表
	#if (USER/100)!=6//用户:不是湖南;湖南另有自行定时
			UARTCtrl->LockFlags&=0xfb;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
	#endif
	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
			//日更新时清零
			MC(0,ADDR_AFN0CF250_Ext1,10*MaxRS485AddCarrierPn+4);
			MC(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,3);
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
				x=UARTCtrl->RTCW[0]+(UARTCtrl->RTCW[1]<<8);//原年月日
				y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//当前年月日
				if(x!=y)
				{
					UARTCtrl->RTCW[0]=y;
					UARTCtrl->RTCW[1]=y>>8;
					UARTCtrl->RMTaskN|=4;//新每周任务
				}
			}
		}
	}
	#endif
#endif

//秒更新
	if(UARTCtrl->RTCS!=Comm_Ram->TYMDHMS[0])
	{//有新任务时空闲时也进入1次,因NewTask_3762新任务在空闲时有效
		UARTCtrl->RTCS=Comm_Ram->TYMDHMS[0];
		//抄表时段
		if(UARTCtrl->RMTaskC==0)
		{//是启抄,总是要抄表
			Terminal_Router->RMTD=1;//0=非抄表时段,1=抄表时段
		}
		else
		{
			x=MRR((ADDR_AFN04F33+((14+(4*24))*30))+13,1);//端口31,允许抄表时段数m
			if(x>24)
			{
				x=24;
			}
//		#if (USER/100)==0//xxx
//			//电科院测试总是抄表
//			w=w;
//			Terminal_Router->RMTD=1;//0=非抄表时段,1=抄表时段
//		#else
			y=MRR(ADDR_TYMDHMS+1,2);//取当前实时钟时分
			Terminal_Router->RMTD=0;//0=非抄表时段,1=抄表时段
			for(i=0;i<x;i++)
			{
				z=MRR((ADDR_AFN04F33+((14+(4*24))*30))+14+(i*4),2);//取允许抄表时段开始时间时分
				if(y>=z)
				{
					w=MRR((ADDR_AFN04F33+((14+(4*24))*30))+14+(i*4)+2,2);//取允许抄表时段结束时间时分
					if(w==0x0)
					{
						w=0x2400;//结束时间时分00:00点认为24:00
					}
					if(y<w)
					{//抄表时段内
						Terminal_Router->RMTD=1;//0=非抄表时段,1=抄表时段
						break;
					}
				}
			}
//		#endif	
		}
		if((Terminal_Router->RMTD!=0)&&(Terminal_Router->RouterInitTask>=200))//0=非抄表时段,1=抄表时段
		{//抄表时段内
			Terminal_AFN0CF11_3762();//终端类1数据F11,终端集中抄表状态信息

			//定时采集停电记录任务
		#if TimerRMTask==1//定时抄读停电记录任务;0=没有,1=有
			if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			{//循抄任务
				i=MRR(ADDR_AFN04F97,1);//停电数据采集配置参数
				i&=1;
				if(i!=0)
				{//要采集
					i=MRR(ADDR_AFN04F97+1,1);//停电事件抄读时间间隔	BIN	1	单位为小时
					if(i!=0)
					{//抄读时间间隔!=0
						p16=(u16*)(&Terminal_Ram->RS4854RMF246_M_Timer);//25 RS485-1停电事件抄读时间间隔分定时器
						if(p16[0]==0)
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
							ClrRMTaskComplete_3762(3,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
							i=MRR(ADDR_AFN04F97+1,1);//停电事件抄读时间间隔	BIN	1	单位为小时
							p16[0]=i*60;//分
							x=MRR(ADDR_TYMDHMS+2,4);
							x=YMDH_AddH(x,i);//年月日时加h时,返回加h时后的年月日时
							d64a=x;
							d64a<<=16;//
							MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//定时抄停电记录间隔周期下次抄表时间秒分时日月�
	
							i=MRR(ADDR_AFN04F97+2,1);//停电事件抄读时间限值	BIN	1	单位为分钟
							i*=60;
							p16=(u16*)(&Terminal_Ram->RS4854RMF246_S_Timer);//25 RS485-4停电事件抄读时间限值秒定时器
							p16[0]=i;
						}
					}
				}
			}
		#endif
		
//			i=NewTask_3762(PORTn);//3762口执行新任务;返回：0=没执行新任务，1=执行新任务
//			if(i!=0)
//			{//此返无别意义,仅使主程序每循环时间小
//				return;
//			}
			//抄表间隔暂停
			if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,4=抄曲线,(MaxRMTask-1)=循抄任务
			{//当前是循抄
				p16=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
//				z=DataComp(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
//				if(z!=2)
//				{
//					z=0;//相等或TYMDHMS>NextRMTime
//				}
//				if((p16[0]==0)||(z==0))
//				{
//					z=0;//抄表定时周期到
//				}
//				else
//				{
//					z=1;//没抄表定时周期到
//				}
				z=TimeDifference(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(z>>31)
				{//TYMDHMS>NextRMTime
					z=0;//相等或TYMDHMS>NextRMTime
				}
				else
				{
					i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);//抄表间隔小时1-60
					if(i>60)
					{
						i=60;
					}
					i*=3600;
					if(z>i)
					{
						z=0;//时钟倒退
					}
				}
				if(p16[0]==0)
				{
					z=0;//抄表定时周期到
				}
				if(UARTCtrl->RMFlag!=0)//0=抄表间隔暂停,1=抄表进行中
				{
					x=NoRMCompletes_3762(MaxRMTask-1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					if(x==0)
					{//全部抄成功	
						if(((Terminal_Router->StopFlags&0xfffffffe)==0)&&(Terminal_Router->RouterCtrl==0))//暂停路由标志:b0=抄表间隔定时
						{
						#if DateRMTask==1//每日抄表任务;0=没有,1=有
							i=NoRMCompletes_3762(1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
							if(i!=0)
							{//需补抄,不暂停
								UARTCtrl->RMTaskN|=2;//新每日任务
								NewTask_3762(PORTn);//3762口执行新任务;返回：0=没执行新任务，1=执行新任务
								Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
								return;
							}
						#endif
							UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
							if((Terminal_Router->StopFlags&1)==0)//暂停路由标志:b0=抄表间隔定时
							{
								Terminal_Router->StopFlags|=(1<<0);//暂停路由标志:b0=抄表间隔定时
								Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
							}
						}
					}
					else
					{//
						x&=0xffff;//未抄数
						if(x==0)
						{//全部已抄但有失败
							if(((Terminal_Router->StopFlags&0xfffffffe)==0)&&(Terminal_Router->RouterCtrl==0))//暂停路由标志:b0=抄表间隔定时
							{
							#if DateRMTask==1//每日抄表任务;0=没有,1=有
								i=NoRMCompletes_3762(1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
								if(i!=0)
								{//需补抄,不暂停
									UARTCtrl->RMTaskN|=2;//新每日任务
									NewTask_3762(PORTn);//3762口执行新任务;返回：0=没执行新任务，1=执行新任务
									Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
									return;
								}
							#endif
							}	
							if(z==0)
							{//抄表定时周期到
								UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
							}
						}
					#if (USER/100)==0
						//电科院互换性测试每到抄表周期即需重启
						if(z==0)
						{//抄表定时周期到
							UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
						}
					#endif
					}
				}
				if(UARTCtrl->RMFlag==0x0)//0=抄表间隔暂停,1=抄表进行中
				{//0=抄表间隔暂停
					if(z==0)
					{//抄表定时周期到
						if((Terminal_Router->StopFlags&0xfffffffe)==0)//暂停路由标志:b0=抄表间隔定时
						{
							Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
							UARTCtrl->RMFlag=1;//1=抄表进行中
							i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);//抄表间隔小时1-60
							if(i==0)
							{
								i=1;//最小1
							}
							if(i>60)
							{
								i=60;
							}
							y=MRR(ADDR_TYMDHMS+2,4);
							d64a=YMDH_AddH(y,i);//年月日时加h时,返回加h时后的年月日时
							d64a<<=16;
							MWR(d64a,(u32)&UARTCtrl->NextRMTime,6);
							i*=3600;
							i+=2;//秒加2,免在59分59秒定时到,同步后又归0,这会少1时
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16[0]=i;//抄表间隔时间s
							//清列表抄表标志
							ClrRMTaskComplete_3762(MaxRMTask-1,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
							UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
							UARTCtrl->FnCount=0;//485抄表Fn计数
							UARTCtrl->RMCount=0;//485抄表命令计数
							UARTCtrl->ReRMCount=0;//485抄表命令重发计数
							UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
							y=Terminal_Router->NoteNum;//当前路由器中总从节点数
							if(y>MaxRS485AddCarrierPn)
							{
								y=MaxRS485AddCarrierPn;
							}
							for(i=0;i<y;i++)
							{
								Terminal_Note->List[i].FnCount=0;//抄表Fn计数
								Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
								Terminal_Note->List[i].ReRMCount=0;//重发计数
								Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
							}
						}
					}
				}
			
			}
		}
	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江						
		x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
		switch(x)
		{
			case 0:
				i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
				if(i&0x01)
				{
					x=MRR(ADDR_AFN0CF249_Ext+9,1);
					if(x!=0)
					{//终端精确对时成功
						if(MRR(ADDR_AFN04F298_Ext,1)!=0)
						{//电表对时开关开启
							if(MRR(ADDR_AFN04F298_Ext+6,1))
							{//允许对时次数不为0
								if(UARTCtrl->RMFlag==0)//0=抄表间隔暂停,1=抄表进行中
								{
									x=NoRMCompletes_3762(MaxRMTask-1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
									if(x==0)
									{//全部抄成功
										MWR(1,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
									}	
								}
							}
						}
					}
				}
				break;
			case 2:
				if(UARTCtrl->BroadcastTime<3)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				{
					if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)<3)//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
					{
						MWR(3,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						MWR(i&0xfe,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
	
						Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
						UARTCtrl->RMFlag=1;//1=抄表进行中
						//清列表抄表标志
						ClrRMTaskComplete_3762(MaxRMTask-1,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
						UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
						UARTCtrl->FnCount=0;//485抄表Fn计数
						UARTCtrl->RMCount=0;//485抄表命令计数
						UARTCtrl->ReRMCount=0;//485抄表命令重发计数
						UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						y=Terminal_Router->NoteNum;//当前路由器中总从节点数
						if(y>MaxRS485AddCarrierPn)
						{
							y=MaxRS485AddCarrierPn;
						}
						for(i=0;i<y;i++)
						{
							Terminal_Note->List[i].FnCount=0;//抄表Fn计数
							Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
							Terminal_Note->List[i].ReRMCount=0;//重发计数
							Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
						}
					}
				}
				break;
			case 3:
				if(UARTCtrl->RMFlag==0)//0=抄表间隔暂停,1=抄表进行中
				{
					x=NoRMCompletes_3762(MaxRMTask-1);//载波未抄数和失败数;入口:RMTask=任务号;返回:b0-b15未抄数,b16-b31失败数
					if(x==0)
					{//全部抄成功
						MWR(4,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
					}	
				}
				break;
			case 4://日更新时清零
				break;
		}
	#endif	
	}
	if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{//间隔暂停时,等待接收模块的主动上报会进入超时状态
		if(Terminal_Router->RMTD!=0)//0=非抄表时段,1=抄表时段
		{//抄表时段内
			NewTask_3762(PORTn);//3762口执行新任务;返回：0=没执行新任务，1=执行新任务
		}
	}


//路由模块
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		//Comm_Ram->SelfTestFlags&=~(1<<16);//B16=载波模块,(置位表示OK)
		Terminal_Router->NoteNum=0;//已知总从节点数
		Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	}
	Uart_3762_RxTx(PORTn);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	Terminal_RouterCtrl(PORTn);//路由器控制;重启,暂停,恢复,检测
	if(Terminal_Router->RouterCtrl!=0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
	{
		return;
	}
//实时抄读
	if(Terminal_Router->RouterRealTimeTask!=0)//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
	{
		//RealTimeReadMeter_3762_Fn_Std(PORTn);//仅抄1个Fn>=129数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
		return;
	}
	
//批量对时任务
#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
#else
	if((Terminal_Router->RouterInitTask>=200)&&(Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
#endif
	{
	#if Extern_BatchTask_3762==0//外部程序:0=I2,1=E1,2=E2,3=
		BatchTask_3762_SETRTC(PORTn);//数据转发批量任务-3762-时钟对时
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
					BatchTask_3762_SETRTC(PORTn);//数据转发批量任务-3762-时钟对时
				#else
					if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
					{
		    		p32=(u32 *)(ADDR_E1_ENTRY);
		    		(*(void(*)())p32[104])(PORTn);//数据转发批量任务-3762-时钟对时
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
	}
	
//自动搜表
#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
#else
	if((Terminal_Router->RouterInitTask>=200)&&(Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
#endif
	{
		i=MRR(ADDR_AFN05F149+2+3,1);//电能表通信参数自动维护功能开启与关闭
		switch(i)
		{
			default:
				if((UARTCtrl->LockFlags&2)!=0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
				{//正在自动搜表
					if(UARTCtrl->AutoSearchTask<100)//自动搜表任务:0=空,1=开始,2...过程
					{
						UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
						UARTCtrl->AutoSearchTask=100;//终止主动注册
					}
					AutoSearchMeter_3762(PORTn);//自动搜表3762端口的电能表
					return;
				}
				break;
			case 0x55://收到此命令
				if((UARTCtrl->LockFlags&2)!=0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
				{//正在自动搜表
					if(UARTCtrl->AutoSearchTask<100)//自动搜表任务:0=空,1=开始,2...过程
					{
						UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
						UARTCtrl->AutoSearchTask=100;//终止主动注册
					}
					AutoSearchMeter_3762(PORTn);//自动搜表3762端口的电能表
					return;
				}
				if((UARTCtrl->Lock==0x55)||(UARTCtrl->Task==4))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					break;
				}
				//清列表抄表中抄表数据标识计数
				x=Terminal_Router->NoteNum;//当前路由器中总从节点数
				for(i=0;i<x;i++)
				{
					Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
					Terminal_Note->List[i].ReRMCount=0;//重发计数
					Terminal_Note->List[i].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				}
				MWR(0x5a,ADDR_AFN05F149+2+3,1);//立即执行
				UARTCtrl->AutoSearchTask=1;//自动搜表任务:0=空,1=开始,2...过程
				UARTCtrl->LockFlags|=6;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
				UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
		#if (USER/100)==6//用户:湖南
				i=MRR(ADDR_AFN05F103+1,2);
				i++;//加1分
				Terminal_Ram->AutoSearchMeterTimer=i;//2 自动搜表允许时间分定时器
		#endif
				return;
			case 0x5a://立即执行
		#if (USER/100)==6//用户:湖南
				if(Terminal_Ram->AutoSearchMeterTimer==0)//2 自动搜表允许时间分定时器
				{
					i=MRR(ADDR_AFN05F103,1);
					if(i==0x55)
					{//是F103启动,不每日执行
						MWR(0,ADDR_AFN05F149+2+3,1);//电能表通信参数自动维护功能开启与关闭
					}
					if(UARTCtrl->AutoSearchTask<100)//自动搜表任务:0=空,1=开始,2...过程
					{
						UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
						UARTCtrl->AutoSearchTask=100;//终止主动注册
					}
					AutoSearchMeter_3762(PORTn);//自动搜表3762端口的电能表
					return;
				}
		#endif
				UARTCtrl->LockFlags|=6;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
				UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
				AutoSearchMeter_3762(PORTn);//自动搜表3762端口的电能表
				return;
			case 0xaa://每日执行
				if((UARTCtrl->LockFlags&2)==0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
				{//没启动自动搜表,每日在抄表结束后启动
					if((UARTCtrl->LockFlags&4)==0)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
					{//本日已自动搜表==0
						if((UARTCtrl->Lock!=0x55)&&(UARTCtrl->Task==0))
						{
							if((Comm_Ram->TYMDHMS[2]>=0x20)&&(Comm_Ram->TYMDHMS[2]<0x22))
							{//每日在20:00-22:00启动,到23:00结束至小有60分
							#if (USER/100)!=7//非浙江用户
							//浙江集中器现场主站不会下发档案,要集中器自动搜表
								if(UARTCtrl->RMComplete&0x80)//485抄表D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
							#endif
								{
									//清列表抄表中抄表数据标识计数
									x=Terminal_Router->NoteNum;//当前路由器中总从节点数
									for(i=0;i<x;i++)
									{
										Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
										Terminal_Note->List[i].ReRMCount=0;//重发计数
									}
									UARTCtrl->AutoSearchTask=1;//自动搜表任务:0=空,1=开始,2...过程
									UARTCtrl->LockFlags|=6;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
									UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
									return;
								}
							}
						}
					}
				}
				else
				{//自动搜表
					AutoSearchMeter_3762(PORTn);//自动搜表3762端口的电能表
					if((Comm_Ram->TYMDHMS[2]<0x20)||(Comm_Ram->TYMDHMS[2]>=0x23))
					{//不是每日在20:00-23:00
						UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					}
					return;
				}
				break;
		}
	}

//命令指定通信端口重新抄表
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((MRR(ADDR_AFN05F51+30,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//命令指定通信端口重新抄表
	{
		if((UARTCtrl->Task!=1)&&(UARTCtrl->Task!=4))//0=空闲
		{
			MC(0,ADDR_AFN05F49+30,1);//清命令指定通信端口暂停抄表
			MC(0x5a,ADDR_AFN05F51+30,1);//启动重新抄表后0x5A
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			ClrRMEvene(PORTn);//清读电能表事件
		#endif
			p16=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
			p16[0]=0;//抄表间隔定时
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			for(i=1;i<MaxRMTask;i++)
			{//抄表任务从1到MaxRMTask-1
				ClrRMTaskComplete_3762(i,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
			}
			UARTCtrl->RMTaskN=0x02;//新任务b0-b7分别表示任务0-7有新任务
			UARTCtrl->RMTaskC=(MaxRMTask-1);//循抄时才能插入高优先级
			UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
			UARTCtrl->FnCount=0;//485抄表Fn计数
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			y=Terminal_Router->NoteNum;//当前路由器中总从节点数
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
			for(i=0;i<y;i++)
			{
				Terminal_Note->List[i].FnCount=0;//抄表Fn计数
				Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
				Terminal_Note->List[i].ReRMCount=0;//重发计数
				Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
			}	
			//集中抄表信息开始结束时间
			p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
			p8+=(19*2);
			MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			return;
		}
	}
	
//初始化指定通信端口下的全部中继路由信息
	if(UARTCtrl->Task==0)//0=空闲
	{
		if(MRR(ADDR_AFN05F52+30,1)==0x55)
		{
			MC(0,ADDR_AFN05F52+30,1);
			MWR(3,ADDR_AFN01Fx+5,1);//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
		}
	}



//
	if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
		if(UARTCtrl->Lock==0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
		{
			return;
		}
	}
//执行广播校时
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
#endif
		RouterBroadcastTime(PORTn);//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
		if(UARTCtrl->BroadcastTime>=4)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
		{
			if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			{
				UARTCtrl->BroadcastTime=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			return;
		}
	#if ((USER/100)==7)||((USER/100)==8)//浙江、通用版
		RouterBroadcastTime_Collector(PORTn);//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
		if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)>=4)//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
		{
			if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			{
				MWR(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			return;
		}
	#endif

#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	}
#endif

//执行抄表
	Terminal_ReadMeter_3762(PORTn);//终端抄表Q/GDW376.2
	
}

#endif



