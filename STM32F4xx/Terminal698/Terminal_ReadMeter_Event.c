
//按分级1-3抄读电能表事件
#include "Project.h"
#include "Terminal_ReadMeter_Event.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_ReadMeter_RS485.h"
#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_AFN0E_Event.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"


#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则

#if (USER/100)==5//上海;电能表事件采集方案另定义
void RMEveneTimer_ShH(u32 PORTn,u32 SuccessRate)//上海集中器分级读电能表事件定时
{
	//RS485口的新任务在Terminal_ReadMeter_RS485.c中执行
	//载波口的新任务在此
	//数据等级：等级1默认实时采集，周期根据实时数据确定；等级2默认1天，需配合采集流程；等级3默认1个月，需配合采集流程。
	UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	u32 x;
	u64 YMDH1;
	u64 YMDH2;
	
	if(PORTn==RS485_4PORT)
	{//RS485_4通信口号(载波)
		UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
		if(UARTCtrl->RMTaskC==(MaxRMTask-1))//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
		{//是循抄
			UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
			//抄读分级事件定时
			YMDH1=MRR(ADDR_TYMDHMS+2,4);
			YMDH2=MRR(((u32)&UARTRMEvent->YMDHMS)+2,4);
			if(YMDH1<YMDH2)
			{//时钟倒调
				MC(0,((u32)&UARTRMEvent->YMDHMS),24);//(全0表示无效)
			}
			else
			{
				if(YMDH1!=YMDH2)
				{//时更新
					MWR(YMDH1,((u32)&UARTRMEvent->YMDHMS)+2,4);//上次用于定时计算的年月日时分秒(全0表示无效)
					YMDH2=MRR(((u32)&UARTRMEvent->YMDHMS)+6+2,4);//分级1事件下次定时开始的年月日时分秒(全0表示无效)
					//到期时间计算
					//1级事件默认实时采集，即按抄表周期采集
					x=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);
					YMDH2=YMDH_AddH(YMDH2,x);//年月日时加h时,返回加h时后的年月日时
					if(YMDH1>=YMDH2)
					{//定时到
						//下次定时开始的年月日时分秒(全0表示无效)
						MWR(YMDH1,((u32)&UARTRMEvent->YMDHMS)+6+2,4);
						//新任务
						//每个抄表周期抄1级事件
						UARTCtrl->RMEventTaskN|=1;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
					}
				}
			}
			//每天第1轮抄完,抄2级事件
			if((UARTCtrl->RMEventTaskN&0x20)!=0)
			{
				UARTCtrl->RMEventTaskN&=~0x20;
				UARTCtrl->RMEventTaskN|=2;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3,;b6=月更新(上海每月抄事件用)
			}
			//每月抄3级事件
			if((UARTCtrl->RMEventTaskN&0x40)!=0)
			{//月更新
				//智能电能表及采集终端事件记录采集规则20150122.doc
				//当月某日采集成功率>98%或日期超过15日且时间超过12:00才开始采集
				if(SuccessRate!=0)
				{//抄表成功率<100%
					x=Comm_Ram->TYMDHMS[3];//实时钟的日
					if(x>0x15)
					{
						UARTCtrl->RMEventTaskN&=~0x40;
						UARTCtrl->RMEventTaskN|=4;
					}
				}
				else
				{//抄表成功率100%
					UARTCtrl->RMEventTaskN&=~0x40;
					UARTCtrl->RMEventTaskN|=4;
				}
			}
		}
	}
}
#endif

void RMEveneTimer(u32 PORTn)//分级读电能表事件定时
{
#if (USER/100)==5//上海;电能表事件采集方案另定义
	//RS485口的新任务在Terminal_ReadMeter_RS485.c中执行
	//载波口的新任务,暂不做以后集中器时做。在上面函数单独做
#else
	u32 i;
	u32 x;
	u32 t;
	u64 YMDHM1;
	u64 YMDHM2;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	//抄读分级事件定时
	YMDHM1=MRR(ADDR_TYMDHMS+1,5);
	YMDHM2=MRR(((u32)&UARTRMEvent->YMDHMS)+1,5);
	if(YMDHM1<YMDHM2)
	{//时钟倒调
		MC(0,((u32)&UARTRMEvent->YMDHMS),24);//(全0表示无效)
	}
	else
	{
		if(YMDHM1!=YMDHM2)
		{//分更新
			MWR(YMDHM1,((u32)&UARTRMEvent->YMDHMS)+1,5);
			for(i=0;i<3;i++)
			{
				YMDHM2=MRR(((u32)&UARTRMEvent->YMDHMS)+6+(6*i)+1,5);
				if(YMDHM2==0)
				{//无效
					MWR(YMDHM1,((u32)&UARTRMEvent->YMDHMS)+6+(6*i)+1,5);
					continue;
				}
				//到期时间计算
				x=MRR(ADDR_AFN04F107+(2*i),2);
				t=x&0xff;//周期数值
				if(t==0)
				{//无效
					continue;
				}
				x>>=8;//周期单位
				switch(x)
				{
					default://无效
						continue;
					case 1://分
						YMDHM2=YMDHM_AddM(YMDHM2,t);//年月日时分加m分,返回加m分后的年月日时分
						break;
					case 2://时
						YMDHM2>>=8;//化为0分
						YMDHM2=YMDH_AddH(YMDHM2,t);//年月日时加h时,返回加h时后的年月日时
						YMDHM2<<=8;
						break;
					case 3://日
						YMDHM2>>=16;//化为0时0分
						YMDHM2<<=16;
						YMDHM2=YMDHM_AddM(YMDHM2,t*60*24);//年月日时分加m分,返回加m分后的年月日时分
						break;
					case 4://月
						YMDHM2>>=24;//化为0日0时0分
						while(t!=0)
						{
							YMDHM2=YM_Add1M(YMDHM2);//年月加1月,返回加1后的年月
							t--;
						}
						YMDHM2<<=24;//化为01日0时0分
						YMDHM2|=0x010000;
						break;
				}
				if(YMDHM1<YMDHM2)
				{//定时没到
					continue;
				}
				//定时到
				//下次定时开始的年月日时分秒(全0表示无效)
				MWR(YMDHM1,((u32)&UARTRMEvent->YMDHMS)+6+(6*i)+1,5);
				//新任务
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
				UARTCtrl->RMEventTaskN|=1<<i;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
			}
		}
	}
#endif
}


u32 NewRMEvene(u32 PORTn)//新或恢复读电能表事件任务;返回0=没新进入,1=新进入
{
	u32 i;
	u32 x;
	u32 y;
  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	
	i=Check_Meter_Event_Save();//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
	if(i<30)
	{//空间可存<30条,每口10条
		StoreRMEvene(PORTn);//暂存读电能表事件
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	if(UARTCtrl->RMDI!=0xffffffff)//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
	{//当前正在抄读库中1个命令计数的多个命令,不切换,因搜索计数没寄存
		return 0;
	}
	if(UARTRMEvent->TaskStore!=0)//暂存读电能表事件标志:0=没,1=有
	{//有暂存
		//总是保存当前任务到第一个事件
		UARTRMEvent->RMEventStack[0].MeterNo=UARTCtrl->MeterNo;
		UARTRMEvent->RMEventStack[0].FnCount=UARTCtrl->FnCount;
		UARTRMEvent->RMEventStack[0].RMCount=UARTCtrl->RMCount;
		UARTRMEvent->RMEventStack[0].RMTaskC=UARTCtrl->RMTaskC;
	#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if(PORTn==RS485_4PORT)//(载波)
		{
			x=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			for(i=0;i<x;i++)
			{
				Terminal_Note->List[i].FnCountSOfMeterEvent=Terminal_Note->List[i].FnCount;//插入定时抄读电能表事件前原抄表的Fn计数
			}
		}
	#endif
		//恢复事件抄读
		UARTCtrl->MeterNo=UARTRMEvent->RMEventStack[3].MeterNo;
		if(UARTCtrl->MeterNo!=0)
		{
			UARTCtrl->MeterNo--;
		}
		UARTCtrl->FnCount=UARTRMEvent->RMEventStack[3].FnCount;
		UARTCtrl->RMCount=UARTRMEvent->RMEventStack[3].RMCount;
		UARTCtrl->RMEventTaskC=UARTRMEvent->RMEventStack[3].RMEventTaskC;
	#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if(PORTn==RS485_4PORT)//(载波)
		{
			for(i=0;i<x;i++)
			{
				Terminal_Note->List[i].FnCount=0;//抄表Fn计数
				Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
				Terminal_Note->List[i].ReRMCount=0;//重发计数
				Terminal_Note->List[i].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
			}
		}
	#endif
		UARTRMEvent->TaskStore=0;//暂存读电能表事件标志:0=没,1=有
		
		UARTCtrl->RMTaskC=MaxRMTask-2;//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=停电记录任务,4=每小时曲线任务,5=,(MaxRMTask-2)=周期抄读电能表事件,(MaxRMTask-1)=循抄任务(除每小时曲线任务外,只有循抄任务才可插入高优先级抄表)
		if(PORTn==RS485_4PORT)
		{//载波
		#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			ClrRMTaskComplete_3762(MaxRMTask-2,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
		#endif
		}
		else
		{//RS485
			ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-2,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
		}
		UARTCtrl->FnCount=0;//抄表Fn计数
		UARTCtrl->ReRMCount=0;//485抄表命令重发计数
		UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
		UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
		UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
		
		UARTCtrl->RMCount&=0xfffffffe;//从抄次数开始
		UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
		UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
		return 1;
	}
	
	y=UARTCtrl->RMEventTaskN&0x07;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
	if(y)
	{
		x=BitLoca(y);//计算第1个置位为1的位置(0-32)
		if((UARTCtrl->RMEventTaskC==0)||(x<UARTCtrl->RMEventTaskC))
		{//当前事件任务=空,或新事件优先级高于当前正在执行的事件
			i=UARTRMEvent->StackCount;//栈计数(值1-3,4固定为临时用)
			if(i<3)//栈计数(值1-3,4固定为临时用)
			{
				y&=~(1<<(x-1));//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
			#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
				//智能电能表及采集终端事件记录采集规则20150122.doc
				//当月某日采集成功率>98%或日期超过15日且时间超过12:00才开始采集
				//故可能不是每月第1轮抄表就去抄3级事件，3级事件周期未到时不清标志UARTCtrl->RMEventTaskN
				if((UARTCtrl->RMEventTaskN&0x40)!=0)
				{//月更新
					if(y!=4)
					{
						UARTCtrl->RMEventTaskN=y;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
						UARTCtrl->RMEventTaskN|=0x40;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3;b6=月更新(上海每月抄事件用)
					}
				}
				else
				{
					UARTCtrl->RMEventTaskN=y;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
				}
			#else
				UARTCtrl->RMEventTaskN=y;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3
			#endif
				
				UARTRMEvent->RMEventStack[i].MeterNo=UARTCtrl->MeterNo;
				UARTRMEvent->RMEventStack[i].FnCount=UARTCtrl->FnCount;
				UARTRMEvent->RMEventStack[i].RMCount=UARTCtrl->RMCount;
				UARTRMEvent->RMEventStack[i].RMTaskC=UARTCtrl->RMTaskC;
				UARTRMEvent->RMEventStack[i].RMEventTaskC=UARTCtrl->RMEventTaskC;

				i++;//栈计数(值1-3,4固定为临时用)
				UARTRMEvent->StackCount=i;//栈计数(值1-3,4固定为临时用)
				UARTCtrl->RMEventTaskC=x;//当前抄读电能表事件任务:0=空,1-3=1-3级				
			#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				if(PORTn==RS485_4PORT)//(载波)
				{
					if(i==1)
					{
						x=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
						if(x>MaxRS485AddCarrierPn)
						{
							x=MaxRS485AddCarrierPn;
						}
						for(i=0;i<x;i++)
						{
							Terminal_Note->List[i].FnCountSOfMeterEvent=Terminal_Note->List[i].FnCount;//插入定时抄读电能表事件前原抄表的Fn计数
							Terminal_Note->List[i].FnCount=0;//用0值而不用F3值是考虑到能进入主动上报
							Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
							Terminal_Note->List[i].ReRMCount=0;//重发计数
							Terminal_Note->List[i].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
							Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
						}
					}
				}
			#endif

				UARTCtrl->RMTaskC=MaxRMTask-2;//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=停电记录任务,4=每小时曲线任务,5=,(MaxRMTask-2)=周期抄读电能表事件,(MaxRMTask-1)=循抄任务(除每小时曲线任务外,只有循抄任务才可插入高优先级抄表)
				if(PORTn==RS485_4PORT)
				{//载波
				#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					ClrRMTaskComplete_3762(MaxRMTask-2,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
				#endif
				}
				else
				{//RS485
					ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-2,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
				}
				UARTCtrl->MeterNo=0;
				UARTCtrl->FnCount=0;//用0值而不用3(F3)值是考虑到能进入主动上报
				UARTCtrl->RMCount=0;
				UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
				
				UARTCtrl->ReRMCount=0;//485抄表命令重发计数
				UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				return 1;
			}
		}
	}
	return 0;
}

void DelRMEvene(u32 PORTn)//删除读电能表事件任务
{
	u32 i;
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	u32 x;
#endif

  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	if(UARTCtrl->RMTaskC==(MaxRMTask-2))//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=停电记录任务,4=每小时曲线任务,5=,(MaxRMTask-2)=周期抄读电能表事件,(MaxRMTask-1)=循抄任务(除每小时曲线任务外,只有循抄任务才可插入高优先级抄表)
	{
		i=UARTRMEvent->StackCount;//栈计数(值1-3,4固定为临时用)
		if(i!=0)
		{
			i--;
			UARTRMEvent->StackCount=i;//栈计数(值1-3,4固定为临时用)
			
			UARTCtrl->MeterNo=UARTRMEvent->RMEventStack[i].MeterNo;
			if(UARTCtrl->MeterNo!=0)
			{
				UARTCtrl->MeterNo--;
			}
			UARTCtrl->FnCount=UARTRMEvent->RMEventStack[i].FnCount;
			UARTCtrl->RMCount=UARTRMEvent->RMEventStack[i].RMCount;
			UARTCtrl->RMTaskC=UARTRMEvent->RMEventStack[i].RMTaskC;
			UARTCtrl->RMEventTaskC=UARTRMEvent->RMEventStack[i].RMEventTaskC;
			
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
			UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
			if(UARTCtrl->RMTaskC==(MaxRMTask-2))
			{//事件抄读
				UARTCtrl->RMCount&=0xfffffffe;//从抄次数开始
				UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
			}
			else
			{//非事件抄读
				UARTCtrl->RMCount=0;
			}
		#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(PORTn==RS485_4PORT)//(载波)
			{
				x=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
				if(x>MaxRS485AddCarrierPn)
				{
					x=MaxRS485AddCarrierPn;
				}
				for(i=0;i<x;i++)
				{
					Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
					Terminal_Note->List[i].ReRMCount=0;//重发计数
					Terminal_Note->List[i].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
					Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
				}
				if(UARTRMEvent->StackCount==0)
				{//栈计数=0,退出抄读事件
					for(i=0;i<x;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfMeterEvent;//插入定时抄读电能表事件前原抄表的Fn计数
					}
				}
				else
				{//返回上级事件
					for(i=0;i<x;i++)
					{
						Terminal_Note->List[i].FnCount=0;//用0值而不用3(F3)值是考虑到能进入主动上报表事件前原抄表的Fn计数
					}
				}
			}
		#endif
		}
		else
		{
			UARTCtrl->RMTaskC=MaxRMTask-1;
			UARTCtrl->RMEventTaskC=0;
		}
	}
}

void StoreRMEvene(u32 PORTn)//暂存正在读电能表事件的任务
{
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	u32 i;
	u32 x;
#endif

  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	if(UARTCtrl->RMTaskC==(MaxRMTask-2))
	{//当前任务是读电能表事件
		UARTRMEvent->TaskStore=1;//暂存读电能表事件标志:0=没,1=有
		//存事件
		UARTRMEvent->RMEventStack[3].MeterNo=UARTCtrl->MeterNo;
		UARTRMEvent->RMEventStack[3].FnCount=UARTCtrl->FnCount;
		UARTRMEvent->RMEventStack[3].RMCount=UARTCtrl->RMCount;
		UARTRMEvent->RMEventStack[3].RMTaskC=UARTCtrl->RMTaskC;
		UARTRMEvent->RMEventStack[3].RMEventTaskC=UARTCtrl->RMEventTaskC;
		//总是恢复到第1个事件抄读进入前任务
		UARTCtrl->MeterNo=UARTRMEvent->RMEventStack[0].MeterNo;
		if(UARTCtrl->MeterNo!=0)
		{
			UARTCtrl->MeterNo--;
		}
		UARTCtrl->FnCount=UARTRMEvent->RMEventStack[0].FnCount;
		UARTCtrl->RMCount=0;
		UARTCtrl->RMTaskC=UARTRMEvent->RMEventStack[0].RMTaskC;
		UARTCtrl->ReRMCount=0;//485抄表命令重发计数
		UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
		UARTCtrl->RMEventTaskC=0;//当前抄读电能表事件任务:0=空,1-3=1-3级
	#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if(PORTn==RS485_4PORT)//(载波)
		{
			x=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			for(i=0;i<x;i++)
			{
				Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfMeterEvent;//插入定时抄读电能表事件前原抄表的Fn计数
				Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
				Terminal_Note->List[i].ReRMCount=0;//重发计数
				Terminal_Note->List[i].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
			}
		}
	#endif	
		UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
		UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
	}
}

void ClrRMEvene(u32 PORTn)//清读电能表事件
{
  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	UARTCtrl->RMEventTaskN=0;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3,;b6=月更新(上海每月抄事件用)
	UARTCtrl->RMEventTaskC=0;//当前事件任务
	MC(0,(u32)&UARTRMEvent->YMDHMS,24);//清定时(全0表示无效)
	UARTRMEvent->TaskStore=0;//暂存读电能表事件标志:0=没,1=有
	UARTRMEvent->StackCount=0;//栈计数(值1-3,4固定为临时用)
}



#endif




















