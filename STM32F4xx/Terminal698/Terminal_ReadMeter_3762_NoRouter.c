

#include "Project.h"
#include "Terminal_ReadMeter_3762_NoRouter.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "KeyUser.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_ReadMeter_Fn.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"

#include "Terminal_ReadMeter_ShH_Lib.h"
#include "Terminal_ReadMeter_ShH_FnData.h"
#include "Terminal_ReadMeter_ShH.h"



#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#if (USER/100)==5//上海
extern const u32 ShH_RMLib[];
u32 Get_RM_ShH_MaxFn(void);
	#endif

extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];

#define MaxReRMCount     1//抄表重发次数(仅重发次数,不算初始1次的发送)


void Terminal_ReadMeter_3762_NoRouter(u32 PORTn)//载波抄表(集中器主导的周期抄表模式)
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	u32 z;
#endif
	u32 Protocol;
	u32 Fn;
	u32 Pn;
	u32 MaxFnCount;
	u32 ClassNo;
	u32 ListNo;
	u64 d64a;
	u64 d64b;
	u32 Info;
	u8 * p8RMLib;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 *p8FnData;
	u32 *p32;
	u16 *p16timer;
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
	u32 AutoReportSearch;//主动上报搜索标志
#endif
#endif

#if ((USER/100)==5)//上海
	u8 *p8;
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	
	if(UARTCtrl->RMFlag==0x0)
	{//0=抄表间隔暂停,1=抄表进行中
		if((Terminal_Router->StopFlags&(1<<17))==0)//暂停路由;b17=抄主动上报事件;也做为间隔暂停时进入主动上报事件抄读标志
		{//载波等待上电10秒内若有主动上报,有UARTCtrl->RMFlag=0抄表间隔暂停情况,抄表将无法执行而死循环!!!
			//允许主动上报接收
			switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 0://0=空闲
					UARTCtrl->Task=5;//接收
					UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					p16timer[0]=100/10;//发送后接收超时定时值ms/10(半字对齐)
					break;
				case 1://1=完成1帧发收
				case 2://2=接收超时(或错误)
					UARTCtrl->Task=0;
					break;
			}
			return;
		}
	}
	if(UARTCtrl->FnEnd==0)//485抄表Fn计数结束标志,0=结束,1=没结束
	{
		y=AFN04F10MaxPn;
		if(y>100)
		{
			y=100;//本次最大查找100只表
		}
		for(x=0;x<y;x++)
		{
			UARTCtrl->MeterNo++;
			if(UARTCtrl->MeterNo>AFN04F10MaxPn)
			{
				UARTCtrl->MeterNo=0;//完成1个循环抄表,电能表/交流采样装置序号=0
			#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
				DelRMEvene(PORTn);//删除读电能表事件任务
			#endif
				/*
				for(i=0;i<Terminal_Router->NoteNum;i++)
				{
					if(Terminal_Note->List[i].RMFLAGS!=0xff)//抄表标志:0=上电或日更新,1=命令强制重抄,2=抄表间隔周期;0xfe=已抄过但没成功,0xff=成功完成抄表
					{//有表没抄读成功,不暂停
						return;
					}
				}
				i=UARTCtrl->RMComplete;
				if((i&0x0f)<0x0f)
				{
					i+=1;//完成循环抄表次数+1
				}
				i|=0x80;
				UARTCtrl->RMComplete=i;
				UARTCtrl->RMFlag=0;//0=抄表间隔暂停,1=抄表进行中
				*/
				return;
			}
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//端口号
			i&=0x1f;
			if(i==31)
			{
				Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//所属测量点号 BIN 2 
				if(Pn!=0x0)
				{//所属测量点号!=0x0
					if(Pn<=MaxRS485AddCarrierPn)
					{
						//启动发送
						UARTCtrl->FnEnd=1;//485抄表Fn计数结束标志,0=结束,1=没结束
						return;
					}
				}
			}
		}
		return;
	}
	if((Terminal_Ram->CongealFlags&0x1f)!=0x1f)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	{//抄表在冻结后进行
		return;
	}
	
//抄表	
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		case 0://0=空闲
			if(UARTCtrl->RMTaskC!=0)//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=定时任务,(MaxRMTask-1)=循抄任务(只有循抄任务才可插入高优先级抄表)
			{//不是启抄
				i=MRR(ADDR_TYMDHMS+1,2);
			#if ((USER/100)==5)//上海
				p8=(u8*)(ADDR_AFN04F242);//抄表延时分钟数
				x=p8[0];
				if(x>=60)//抄表延时分钟数：1-60分钟
				{
					x=0x100;//1:00
				}
				else
				{
					x=hex_bcd(x);
				}
				if((i>=RMM_EndHM)||(i<x))
				{//在每日0点附近暂停抄表
					WarningOccur(PORTn,(u8 *)"载波停抄等待时钟过0");//告警发生,入口告警代码和告警字符串
					return;
				}
			#else
				if((i>=RMM_EndHM)||(i<RMM_StartHM))
				{//在每日0点附近暂停抄表
					WarningOccur(PORTn,(u8 *)"载波停抄等待时钟过0");//告警发生,入口告警代码和告警字符串
					return;
				}
			#endif
			}
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//取从节点地址
			x=Terminal_Router->NoteNum;//当前路由器中总从节点数
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
			for(ListNo=0;ListNo<x;ListNo++)
			{
				d64b=p32[1]&0xffff;
				d64b<<=32;
				d64b|=p32[0];
				if(d64a==d64b)
				{
					break;
				}
				p32+=(LEN_NoteList/4);
			}
			if(ListNo==x)
			{//没找到内存中从节点列表
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				return;
			}
		#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
			{//本任务己成功抄表,直接返回抄读成功
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				return;
			}
		#else
			if((Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)&&(UARTCtrl->RMMeterEvent==0))//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
			{//本任务己成功抄表,直接返回抄读成功
				if((Terminal_Router->StopFlags&(1<<17))==0)//暂停路由;b17=抄主动上报事件;也做为间隔暂停时进入主动上报事件抄读标志
				{
					UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
					return;
				}
			}
		#endif
			p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//本序号的电能表/交流采样装置删除
				goto END_Fn;//不抄
			}
			x=UARTCtrl->MeterNo;//取对应电能表/交流采样装置序号(从1开始)
			if((x==0)||(x>AFN04F10MaxPn))
			{//序号错,不抄,直接返回抄读成功
				goto END_Fn;
			}
		#if (USER/100)!=0//电科院测试设置F33只抄重点表BUG
			i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//终端抄表运行参数设置,端口31
			if((i&0x02)!=0x0)
			{//D1置"1"要求终端只抄重点表，置"0"要求终端抄所有表
				y=MRR(ADDR_AFN04F35,1);//台区集中抄表重点户设置
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MRR(ADDR_AFN04F35+1+(2*i),2)==x)
					{
						break;
					}
				}
				if(i==y)
				{//不是重点户
					goto END_Fn;//不抄
				}
			}
			i=MRR(ADDR_AFN04F30+(Pn-1),1);//台区集中抄表停抄/投抄设置(测量点号)
			if(i!=0)
			{//停抄
				goto END_Fn;//不抄
			}
		#endif
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
			switch(Protocol)
			{
				case 1://DL/T645-1997
					MaxFnCount=Get_RM_DL645_1997_MaxFn();
					break;
			#if ((USER/100)==5)//上海用户
				case 21://上海规约
					MaxFnCount=Get_RM_ShH_MaxFn();
					break;
			#endif
				default://DL/T645-2007
					MaxFnCount=Get_RM_DL645_2007_MaxFn();
					break;
			}
			if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
			{//Fn计数>=最大Fn个数,本表抄表完成
	END_Fn:	
			#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
				if(Terminal_Note->List[ListNo].RMErr==0)//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
				{//抄表成功
					Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
				}
				else
				{//至少有1个数据标识抄读失败
					Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
				}
			#else
				if(UARTCtrl->RMMeterEvent==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
				{
					if(Terminal_Note->List[ListNo].RMErr==0)//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					{//抄表成功
						Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
					}
					else
					{//至少有1个数据标识抄读失败
						Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
					}
				}
			#endif
				Terminal_Note->List[ListNo].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
				Terminal_Note->List[ListNo].FnCount=0;//Fn计数=0
				Terminal_Note->List[ListNo].RMCount=0;//抄表命令计数=0
				Terminal_Note->List[ListNo].ReRMCount=0;//抄表命令重发计数
				UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
				UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
				
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				Terminal_Note->List[ListNo].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
				return;
			}
			
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
			AutoReportSearch=0;//主动上报搜索标志
		#endif
		#endif
			while(1)
			{
				if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
				{//Fn计数>=最大Fn个数,本表抄表结束
					goto END_Fn;
				}
				switch(Protocol)
				{
					case 1://DL/T645-1997
						i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
						break;
				#if ((USER/100)==5)//上海用户
					case 21://上海规约
						i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
						break;
				#endif
					default://DL/T645-2007
						i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
						break;
				}

				if(i==0)
				{//本Fn是空
		NEXT_Fn:
					Terminal_Note->List[ListNo].FnCount++;//Fn计数+1
					Terminal_Note->List[ListNo].RMCount=0;//抄表数据标识计数
					Terminal_Note->List[ListNo].ReRMCount=0;//重发计数
					Terminal_Note->List[ListNo].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数

		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
					if((UARTCtrl->RMMeterEvent&1)==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
					{//0=否
						UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
						UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
						UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
						if(UARTCtrl->RMTaskC!=0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						{//不是启抄
							if(AutoReportSearch==0)//主动上报搜索标志
							{
								AutoReportSearch=1;//主动上报搜索标志
								x=Check_Meter_Event_Save();//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
								if(x>=30)
								{//空间可存>=30条,每口10条

										x=Check_AutoReportWord(PORTn);//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
										x>>=16;
										if(x!=0)
										{//有主动上报需要读
											UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
											UARTCtrl->MeterNo=x;
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//取从节点地址
											x=Terminal_Router->NoteNum;//当前路由器中总从节点数
											if(x>MaxRS485AddCarrierPn)
											{
												x=MaxRS485AddCarrierPn;
											}
											p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
											for(ListNo=0;ListNo<x;ListNo++)
											{
												d64b=p32[1]&0xffff;
												d64b<<=32;
												d64b|=p32[0];
												if(d64a==d64b)
												{
													break;
												}
												p32+=(LEN_NoteList/4);
											}
											Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
											Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
									#if ((USER/100)==5)//上海用户
											if((ListNo==x)||(Protocol==1)||(Pn==0)||(Protocol==21))
									#else
										#if(USER/100)==7//浙江用户
											if((ListNo==x)||(Pn==0))
										#else
											if((ListNo==x)||(Protocol==1)||(Pn==0))
										#endif
									#endif
											{//没找到内存中从节点列表或DL645-1997或测量点删除
												//清事件标志,免死循环
												MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
												UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
												return;
											}
											Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485抄表Fn计数(原常规抄表的Fn计数寄存)
											Terminal_Note->List[ListNo].FnCount=2;
											
											AutoReportSearch=0;//主动上报搜索标志
											UARTCtrl->RMMeterEvent|=1;//正在抄读电能表事件标志;b0=主动上报事件,b1=
											return;
										}

								}
								Terminal_Router->StopFlags&=~(1<<17);//暂停路由;b17=抄主动上报事件
							}
							if(Protocol==30)//30=DL/T645-2007
							{//当前只有2007库才有F2(电能表主动上报事件)
								p8RMLib=(u8 *)DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
								if(p8RMLib[0]==2)//F2
								{
									Terminal_Note->List[ListNo].FnCount++;//Fn计数+1;非主动事件跳过不抄
								}
							}
						#if(USER/100)==7//浙江用户
							if(Protocol==1)//1=DL/T645-1997
							{//浙江1997库也有F2(电能表主动上报事件)
								MaxFnCount=Get_RM_DL645_1997_MaxFn();
								if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
								{//Fn计数>=最大Fn个数,本表抄表结束
									goto END_Fn;
								}
								p8RMLib=(u8 *)DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
								if(p8RMLib[0]==2)//F2
								{
									Terminal_Note->List[ListNo].FnCount++;//Fn计数+1;非主动事件跳过不抄
								}
							}
						#endif
						}
					}
					else
					{//是
						UARTCtrl->RMMeterEvent&=0xfe;//正在抄读电能表事件标志;b0=主动上报事件,b1=
						UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
						Terminal_Note->List[ListNo].FnCount=Terminal_Note->List[ListNo].FnCountBackupOfAutoReport;//485抄表Fn计数(原常规抄表的Fn计数寄存)
						if((Terminal_Router->StopFlags&(1<<17))!=0)//暂停路由;b17=抄主动上报事件;也做为间隔暂停时进入主动上报事件抄读标志
						{//
							x=Check_Meter_Event_Save();//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
							if(x>=30)
							{//空间可存>=30条,每口10条
								while(1)
								{
									x=Check_AutoReportWord(PORTn);//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
									x>>=16;
									if(x==0)
									{
										break;
									}
									else
									{//有主动上报需要读
										UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
										UARTCtrl->MeterNo=x;
										d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//取从节点地址
										x=Terminal_Router->NoteNum;//当前路由器中总从节点数
										if(x>MaxRS485AddCarrierPn)
										{
											x=MaxRS485AddCarrierPn;
										}
										p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
										for(ListNo=0;ListNo<x;ListNo++)
										{
											d64b=p32[1]&0xffff;
											d64b<<=32;
											d64b|=p32[0];
											if(d64a==d64b)
											{
												break;
											}
											p32+=(LEN_NoteList/4);
										}
										Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
										Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
								#if ((USER/100)==5)//上海用户
										if((ListNo==x)||(Protocol==1)||(Pn==0)||(Protocol==21))
								#else
										#if(USER/100)==7//浙江用户
											if((ListNo==x)||(Pn==0))
										#else
											if((ListNo==x)||(Protocol==1)||(Pn==0))
										#endif
								#endif
										{//没找到内存中从节点列表或DL645-1997或测量点删除
											//清事件标志,免死循环
											MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
											UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
											continue;
										}
										Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485抄表Fn计数(原常规抄表的Fn计数寄存)
										Terminal_Note->List[ListNo].FnCount=2;
										
										UARTCtrl->RMMeterEvent|=1;//正在抄读电能表事件标志;b0=主动上报事件,b1=
										return;
									}
								}
							}
						}
						Terminal_Router->StopFlags&=~(1<<17);//暂停路由;b17=抄主动上报事件;也做为间隔暂停时进入事件抄读标志
						return;
					}
		#endif
		#endif
					continue;
				}
				p8RMLib=(u8 *)i;//抄表库
				Fn=p8RMLib[0];//取实时Fn
				if((p8RMLib[3]&0x04)==0)//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
				{
					goto NEXT_Fn;
				}
				if(p8RMLib[1]!=0)
				{//本Fn抄表命令数!=0
					if(Terminal_Note->List[ListNo].RMCount>=p8RMLib[1])
					{//抄表数据标识计数>=Fn需发命令数;本Fn抄表完成
						if(p8RMLib[1]>Terminal_Note->List[ListNo].RMCountErr)
						{//不是全部标识全部抄读失败
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
						}
						goto NEXT_Fn;
					}
				}
				
			#if Extern_ReadMeter_Fn_YesNo==0//外部程序:0=I2,1=E1,按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)	
				y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,Terminal_Note->List[ListNo].RMCount,Protocol);//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针
			#else
				y=0;
				if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
				{
					p32=(u32 *)(ADDR_E1_ENTRY);
					y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)Terminal_Note->List[ListNo].RMCount,(u32)Protocol);//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数
				}
			#endif
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
				if(UARTCtrl->RMMeterEvent!=0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
				{
					y=1;//抄读主动上报总是允许
				}
		#endif
		#endif
				if(Fn==26)
				{
					if(Protocol!=1)//(1=1997,其他=2007)
					{
					#if RMM_AFN0CF26==0//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
						if(Terminal_Note->List[ListNo].RMCount==12)//抄表数据标识计数
					#endif
					#if RMM_AFN0CF26==1//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
						if(Terminal_Note->List[ListNo].RMCount==16)//抄表数据标识计数
					#endif
						{//抄表命令计数16以上是兼容命令,只有不成功时才抄
							for(i=0;i<(1+61);i++)
							{
								if(p8FnData[i]!=0xee)
								{
									break;
								}
							}
							if(i!=(1+61))
							{//有抄成功
								y=0;//不抄
								Terminal_Note->List[ListNo].RMCount=100;//抄表数据标识计数
							}
						}
					}
				}
			#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
				if(y!=0)
				{
				//扩展14 补抄上2日日冻结数据
				//扩展15 补抄上3日日冻结数据
					if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{
						y=0;//
							x=MRR(ADDR_TYMDHMS+3,3);//取当前年月日
							x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
							if(p8RMLib[0]==0xEE)
							{
								y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
							}
							else
							{
								x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
								y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
							}
							y=MRR(y,3);//取上?日补抄时标
							if(x==y)
							{//己抄
								goto NEXT_Fn;
							}
							x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
							i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
							if(i!=0xffffffff)
							{//找到
								z=Terminal_Note->List[ListNo].RMCount;//抄表命令计数
								if((z>0)&&(z<9))
								{
									z+=160;//抄表命令计数1-8对应Fn=161-168
									z=GetClass2DataOfficeAddr(z,Pn);//取类2数据存贮偏移地址,返回0表示没找到
									if(z!=0)
									{//找到
										y=MRR(ADDR_TYMDHMS+3,3);//取当前年月日
										z=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+z+2,3);//取原抄表时间年月日
										if((z!=0xeeeeee)&&(z!=0xffffff))
										{
											if((z>x)&&(z<=y))
											{//原抄表时间>上2日时标,同时<=当前时间
												goto NEXT_Fn;//己抄
											}
										}
									}
								}
							}
							while(1)
							{
								i=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
								y=1;//抄表数据标识计数=0和9总是要抄
								if(i>9)
								{
									break;
								}
								if((i>0)&&(i<9))
								{
									i+=160;//要抄的Fn=161-168
									y=Class2Data_Fn(ClassNo,i);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持		
								}
								if(y==0)
								{
									Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
									Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数
								}
								else
								{
									break;
								}
							}
							if(Terminal_Note->List[ListNo].RMCount>=p8RMLib[1])
							{//抄表数据标识计数>=Fn需发命令数;本Fn抄表完成
								if(p8RMLib[1]>Terminal_Note->List[ListNo].RMCountErr)
								{//不是全部标识全部抄读失败
									ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
								}
								goto NEXT_Fn;
							}
					}
				}
			#endif
				if(y!=0)
				{
					if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{//抄上1-3日日冻结数据
					#if ERC39==1
						if(p8RMLib[0]==0xEF)
						{
							if(Terminal_Note->List[ListNo].RMCount<=1)
							{
								ERC39_Event(Pn);//补抄失败事件记录
							}
						}
					#endif
					#if (USER/100)==4//用户标识:陕西
						if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
						{
							i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
							if(i==1)
							{//抄上2-3日日冻结数据
								y=0;//陕西农网表无日冻结
							}
						}
					#endif
						if(Terminal_Note->List[ListNo].RMCount==9)
						{//再次确认日冻结时间YYMMDDhhmm
							//省略不抄,因抄表在过0点后延时进行,在此判断不修改抄表库减少错误
							y=0;
						}
					}
				}
				if(y==0x0)
				{//本命令不抄
					if(p8RMLib[1]==0x0)
					{//本Fn抄表命令数=0
						goto NEXT_Fn;
					}
					if(Terminal_Note->List[ListNo].RMCount==0)//抄表数据标识计数
					{//抄表数据标识计数=0,清Fn数据缓冲
						Terminal_Note->List[ListNo].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
						MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
					}
					Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
					Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数
					Terminal_Note->List[ListNo].ReRMCount=0;//重发计数
					continue;
				}
				if(p8RMLib[1]==0x0)
				{//本Fn抄表命令数=0
				#if AFN0CF33MaxPn==0
					goto NEXT_Fn;
				#else
					if((p8RMLib[4]&0x01)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
					{//1类数据,实时
						switch(Fn)
						{
						//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						//F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						//F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 33:
							case 34:
							case 35:
							case 36:
						//F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						//F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						//F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						//F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M�12�
							case 37:
							case 38:
							case 39:
							case 40:
								break;
							default:
								goto NEXT_Fn;
						}
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
						goto NEXT_Fn;
					}
				#endif
				}

				if(Terminal_Note->List[ListNo].RMCount==0)//抄表数据标识计数
				{///抄表数据标识计数=0,清Fn数据缓冲
					Terminal_Note->List[ListNo].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				
				if(Terminal_Note->List[ListNo].ReRMCount>MaxReRMCount)
				{//抄表重发次数>MaxReRMCount
			#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					if(p8RMLib[0]==2)//抄读主动上报事件
					{
						//清事件标志,免死循环
						MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
						goto NEXT_Fn;
					}
			#endif
					ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
					if(p8RMLib[0]==129)
					{
						ClrERC29SOUR(Pn);//集中抄表失败清ERC29电能表飞走事件原始数据
					}
					//日抄表标志
					if(UARTCtrl->RMTaskC==1)//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=,(MaxRMTask-1)=循抄任务(只有循抄任务才可插入高优先级抄表)
					{
						if(UARTCtrl->DateReRMCount>=3)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
						{
							DateRMFlags(Pn,1);//日抄表标志;入口:f=0成功,=1失败
						}
					}
					Terminal_Note->List[ListNo].RMErr=1;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
					Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数
					Terminal_Note->List[ListNo].ReRMCount=0;//重发计数
					
					//先抄其他表
					goto END_Fn;
				}
				Terminal_Note->List[ListNo].ReRMCount++;//抄表重发计数+1
				
				if(p8RMLib[0]==0xED)
				{//扩展13 上1-24整点电表冻结的正反有功电能示值
					//Fn101正有功电能示值曲线配置
					y=Class2Data_Fn(ClassNo,101);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					//Fn103反有功电能示值曲线配置
					y|=Class2Data_Fn(ClassNo,103);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					if(y==0)
					{
						goto NEXT_Fn;
					}
					if(Terminal_Note->List[ListNo].RMCount==0)
					{//仅当命令计数=0时列表,减少运行时间
						KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
					}
					i=MRR(ADDR_TYMDHMS+2,4);//取当前年月日时
					d64a=YMDH_SubH(i,Terminal_Note->List[ListNo].RMCount);//年月日时减h时,返回减h时后的年月日时
					d64a<<=8;//分=0
					i=CheckCurveCongeal(d64a,101,Pn);//检查曲线冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
					if(i!=0)
					{//已冻结或没找到冻结号
						Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
						Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数
						Terminal_Note->List[ListNo].ReRMCount=0;//重发计数
						continue;
					}					
				}
		
				d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//从节点地址(目的地址)
				switch(Protocol)
				{
					case 1://DL/T645-1997
						i=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//取DI
						Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
						//预计应答字节数
						i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];//返回长度
						if(i==0)
						{//返回长度=0
							i=10+(3*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
							i=p8RMLib[i];//取预计返回字节数
						}
						i+=14;
						Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
						if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{//微功率无线,目标地址用采集器地址
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
								Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
							}
						}
				#if(USER/100)==7//浙江用户
					#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
						if(p8RMLib[0]==2)//F2 电能表主动上报事件
						{
							//取DI	
							x=(u32)p8RMLib;
							x+=10+(3*Terminal_Note->List[ListNo].RMCount);
							x=MRR(x,2);
							if(x==0)
							{//保留
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+(Terminal_Note->List[ListNo].RMCount/2),1);
								Terminal_Note->List[ListNo].RMCount++;
								if(Terminal_Note->List[ListNo].RMCount&1)
								{
									Terminal_Note->List[ListNo].RMCount++;
								}
								Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数
								return;
							}
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+(Terminal_Note->List[ListNo].RMCount/2),1);
							if(i==0)
							{//不需读
								Terminal_Note->List[ListNo].RMCount++;
								if(Terminal_Note->List[ListNo].RMCount&1)
								{
									Terminal_Note->List[ListNo].RMCount++;
								}
								Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数
								return;
							}
							MC(0xee,(u32)p8FnData,4);//第1帧标志
						}
					#endif
				#endif
						break;
				#if (USER/100)==5//上海用户
					case 21://上海规约
						i=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//取DI
						Read_ShH_TxData(ADDR_DATABUFF,i,d64a);//上海规约读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
						//预计应答字节数
						i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];//返回长度
						i+=14;
						Uart_3762_AFN13F1_Tx(PORTn,0,ADDR_DATABUFF,14,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
						if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{//微功率无线,目标地址用采集器地址
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
								Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
							}
						}
						break;
				#endif
					default://DL/T645-2007
					#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
						//EventProject==0
						if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
						{//不是读电表曲线
							i=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//取DI
							switch(i)
							{//以下命令因帧太长不抄
								case 0x03300101://F27 最近一次电表清零发生时刻
								case 0x03300201://F27 最近一次需量清零发生时刻
								case 0x03300501://F166 最近一次电能表时段参数修改时间
									Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
									Terminal_Note->List[ListNo].RMCount++;//抄表命令计数
									Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
									return;
							}
						#if (USER/100)==4//用户标识:陕西
							x=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
							if((x==1)&&(p8RMLib[0]==0xE3))
							{//变换日冻结数据标识
								switch(Terminal_Note->List[ListNo].RMCount)
								{
									default://日冻结时间YYMMDDhhmm
										Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
										Terminal_Note->List[ListNo].RMCount++;//抄表命令计数
										Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
										return;
									case 1://上1次日正向有功电能示值（总、费率1～M）
										i=0x00010000;//当前正向有功电能示值（总、费率1～M）
										break;
								}
							}
						#endif
							Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
							//预计应答字节数
							i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//返回长度
							if(i==0)
							{//返回长度=0
								i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
								i=p8RMLib[i];//取预计返回字节数
							}
							i+=16;
							Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
							if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{//微功率无线,目标地址用采集器地址
								d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
								if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
								{
									MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
									Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
								}
							}
						}
						else
						{
							//补当前到上日0点曲线点
							d64b=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
						#if RMM_Carrier_CurveTime==15//载波通道读电表曲线的整冻结间隔时间分;15分或60分
							d64b=YMDHM_Int15(d64b);//年月日时分化为整15分,如00时18分化为00时15分
							i=Terminal_Note->List[ListNo].RMCount*15;//每15分间隔
						#else
							d64b&=0xffffffff00;
							i=Terminal_Note->List[ListNo].RMCount*60;//每60分间隔
						#endif
							d64b=YMDHM_SubM(d64b,i);//年月日时分减m分,返回减m分后的年月日时分
							i=p8RMLib[0];
							i-=0xe4;
							i<<=16;
							i+=0x06000001;
							ReadMeter_DL645_2007_TxCurveData(ADDR_DATABUFF,i,d64a,d64b);//DL645-2007读负荷曲线数据帧
							//预计应答字节数
							i=p8RMLib[10+4];//返回长度
							if(i==0)
							{//返回长度=0
								i=10+(5*1);
								i=p8RMLib[i];//取预计返回字节数
							}
							i+=16;
							Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,22,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
							if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{//微功率无线,目标地址用采集器地址
								d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
								if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
								{
									MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
									Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
								}
							}
						}
					#else//#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
						//EventProject==1
						if(UARTCtrl->SubsequentFrames==0)//0=无读后续帧,1-255=DL645读续帧的帧号
						{//非后续帧
							if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
							{//不是读电表曲线
								i=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//取DI
//								switch(i)
//								{//以下命令因帧太长不抄
//									case 0x03300101://F27 最近一次电表清零发生时刻
//									case 0x03300201://F27 最近一次需量清零发生时刻
//									case 0x03300501://F166 最近一次电能表时段参数修改时间
//										Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
//										Terminal_Note->List[ListNo].RMCount++;//抄表命令计数
//										Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
//										return;
//								}
							#if (USER/100)==4//用户标识:陕西
								x=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
								if((x==1)&&(p8RMLib[0]==0xE3))
								{//变换日冻结数据标识
									switch(Terminal_Note->List[ListNo].RMCount)
									{
										default://日冻结时间YYMMDDhhmm
											Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
											Terminal_Note->List[ListNo].RMCount++;//抄表命令计数
											Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
											return;
										case 1://上1次日正向有功电能示值（总、费率1～M）
											i=0x00010000;//当前正向有功电能示值（总、费率1～M）
											break;
									}
								}
							#endif
								Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
								//预计应答字节数
								i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//返回长度
								if(i==0)
								{//返回长度=0
									i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
									i=p8RMLib[i];//取预计返回字节数
								}
								i+=16;
								Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
								if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
								{//微功率无线,目标地址用采集器地址
									d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
									if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
									{
										MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
										Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
									}
								}
							#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
								if(p8RMLib[0]==2)//F2 电能表主动上报事件
								{
									i=Terminal_Note->List[ListNo].RMCount;
									switch(i)
									{
										case 0:
											Terminal_Note->List[ListNo].RMCount++;
											Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数
											return;
										case (2+(96*2))://复位主动上报状态字
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											p8rxbuff=(u8*)(ADDR_DATABUFF);
											MWR(d64a,ADDR_DATABUFF+1,6);
											p8rxbuff[8]=0x14;
											p8rxbuff[9]=24;
											MWR(0x04001503,ADDR_DATABUFF+10,4);
											MC(0,ADDR_DATABUFF+14,8);//密码,操作者代码
											MR(((u32)p8rxbuff)+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//复位主动上报状态字的位标志
											for(i=0;i<12;i++)
											{//取反标志位
												x=p8rxbuff[22+i];
												x=~x;
												p8rxbuff[22+i]=x;
											}
											DL645_68H_Add33H_CS_16H(ADDR_DATABUFF);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
											UARTCtrl->TxSEQ--;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,36,12,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
											if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//微功率无线,目标地址用采集器地址
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
													Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
												}
											}
											break;
										case 1://读主动上报状态字(仅当鼎信模块时)
											if(Comm_Ram->RouterType!=2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
											{//不是鼎信模块
												Terminal_Note->List[ListNo].RMCount++;
												Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数
												return;
											}
										case (2+(96*2)+1)://读主动上报状态字
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											x=0x04001501;
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
											UARTCtrl->TxSEQ--;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
											if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//微功率无线,目标地址用采集器地址
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
													Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
												}
											}
											break;
										default://2-97读事件
											//取DI	
											x=(u32)p8RMLib;
											x+=10+(5*Terminal_Note->List[ListNo].RMCount);
											x=MRR(x,4);
											if(x==0)
											{//保留
												MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
												Terminal_Note->List[ListNo].RMCount++;
												if(Terminal_Note->List[ListNo].RMCount&1)
												{
													Terminal_Note->List[ListNo].RMCount++;
												}
												Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数
												return;
											}
											i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
											if(i==0)
											{//不需读
												Terminal_Note->List[ListNo].RMCount++;
												if(Terminal_Note->List[ListNo].RMCount&1)
												{
													Terminal_Note->List[ListNo].RMCount++;
												}
												Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数
												return;
											}
											if(Terminal_Note->List[ListNo].RMCount&1)
											{//读1-10次事件
												if(i>10)
												{
													i=10;
												}
												i--;
												x+=i;
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
												Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
												UARTCtrl->TxSEQ--;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
												Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
												if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
												{//微功率无线,目标地址用采集器地址
													d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
													if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
													{
														MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
														Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
													}
												}
											}
											MC(0xee,(u32)p8FnData,4);//第1帧标志
											break;
									}
								}
							#endif
								if(p8RMLib[0]==3)//F3 周期抄读电能表事件
								{
								#if ((USER/100)==5)//上海用户
									switch(Terminal_Note->List[ListNo].RMCount)
									{
									#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
										case ((60)*3)://读主动上报状态字
											break;
										case ((60)*3+1)://复位主动上报状态字
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											p8rxbuff=(u8*)(ADDR_DATABUFF);
											MWR(d64a,ADDR_DATABUFF+1,6);
											p8rxbuff[8]=0x14;
											p8rxbuff[9]=24;
											MWR(0x04001503,ADDR_DATABUFF+10,4);
											MC(0,ADDR_DATABUFF+14,8);//密码,操作者代码
											MR(((u32)p8rxbuff)+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//复位主动上报状态字的位标志
											
											for(i=0;i<12;i++)
											{
												p8txbuff[22+i]=0xff;
											}
											//取反标志位bit5、bit7
											x=p8txbuff[22];
											x=(~(x&0xa0));
											p8txbuff[22]=x;
											DL645_68H_Add33H_CS_16H(ADDR_DATABUFF);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
											UARTCtrl->TxSEQ--;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,36,12,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
											if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//微功率无线,目标地址用采集器地址
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
													Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
												}
											}
											break;
										case ((60)*3+3)://读电表运行状态字
											break;
									#endif
										default:
											i=Terminal_Note->List[ListNo].RMCount;
											if((i%3)!=0)
											{//读1-10次事件
												i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/3))+3,1);//新增次数(初始化值0xff)
												if(i==0xff)
												{
													i=0;
												}
												if(i==0)
												{//不需读
													Terminal_Note->List[ListNo].RMCount++;
													Terminal_Note->List[ListNo].ReRMCount=0;
													return;
												}
												if(i>10)
												{
													i=10;
												}
												i--;
												//取DI	
												x=(u32)p8RMLib;
												x+=10+(5*Terminal_Note->List[ListNo].RMCount);
												x=MRR(x,4);
												x+=i;
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
												Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
												Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
												if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
												{//微功率无线,目标地址用采集器地址
													d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
													if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
													{
														MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
														Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
													}
												}
												MC(0xee,(u32)p8FnData+((Terminal_Note->List[ListNo].RMCount%3)-1)*2,2);//第1帧标志
											}
											break;
									}
								#else//#if ((USER/100)==5)//上海用户
								//不是上海的周期事件抄读
									if(UARTCtrl->RMDI==0xffffffff)//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
									{//UARTCtrl->RMDI无效
										i=Terminal_Note->List[ListNo].RMCount;
										if((i&1)!=0)
										{//读1-10次事件
											i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//新增次数(初始化值0xff)
											if(i==0xff)
											{
												i=0;
											}
											if(i==0)
											{//不需读
												Terminal_Note->List[ListNo].RMCount++;
												Terminal_Note->List[ListNo].ReRMCount=0;
												return;
											}
											if(i>10)
											{
												i=10;
											}
											i--;
											//取DI	
											x=(u32)p8RMLib;
											x+=10+(5*Terminal_Note->List[ListNo].RMCount);
											x=MRR(x,4);
											x+=i;
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
											if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//微功率无线,目标地址用采集器地址
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
													Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
												}
											}
											MC(0xee,(u32)p8FnData,4);//第1帧标志
										}
									}
									else
									{//UARTCtrl->RMDI有效
										i=Terminal_Note->List[ListNo].RMCount;
										if((i&1)==0)
										{//读次数(非事件类数据)
											x=UARTCtrl->RMDI;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
											if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//微功率无线,目标地址用采集器地址
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
													Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
												}
											}
											if(UARTCtrl->RMDICount==0)//周期事件抄读已抄DI计数
											{
												MC(0xee,(u32)p8FnData,4);//第1帧标志
											}
										}
										else
										{//读1-10次事件
											i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//新增次数(初始化值0xff)
											if(i==0xff)
											{
												i=0;
											}
											if(i==0)
											{//不需读
												Terminal_Note->List[ListNo].RMCount++;
												Terminal_Note->List[ListNo].ReRMCount=0;
												return;
											}
											if(i>10)
											{
												i=10;
											}
											i--;
											//取DI	
											x=UARTCtrl->RMDI;
											x&=0xffffff00;
											x|=0x01;
											x+=i;
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
											if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//微功率无线,目标地址用采集器地址
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
													Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
												}
											}
											if(UARTCtrl->RMDICount==0)//周期事件抄读已抄DI计数
											{
												MC(0xee,(u32)p8FnData,4);//第1帧标志
											}
										}
									}
								#endif//#if ((USER/100)==5)//上海用户
								}
							}
							else
							{
								//补当前到上日0点曲线点
								d64b=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
							#if RMM_Carrier_CurveTime==15//载波通道读电表曲线的整冻结间隔时间分;15分或60分
								d64b=YMDHM_Int15(d64b);//年月日时分化为整15分,如00时18分化为00时15分
								i=Terminal_Note->List[ListNo].RMCount*15;//每15分间隔
							#else
								d64b&=0xffffffff00;
								i=Terminal_Note->List[ListNo].RMCount*60;//每60分间隔
							#endif
								d64b=YMDHM_SubM(d64b,i);//年月日时分减m分,返回减m分后的年月日时分
								i=p8RMLib[0];
								i-=0xe4;
								i<<=16;
								i+=0x06000001;
								ReadMeter_DL645_2007_TxCurveData(ADDR_DATABUFF,i,d64a,d64b);//DL645-2007读负荷曲线数据帧
								//预计应答字节数
								i=p8RMLib[10+4];//返回长度
								if(i==0)
								{//返回长度=0
									i=10+(5*1);
									i=p8RMLib[i];//取预计返回字节数
								}
								i+=16;
								Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,22,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
								if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
								{//微功率无线,目标地址用采集器地址
									d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
									if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
									{
										MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
										Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
									}
								}
							}
						}
						else
						{//是后续帧
							//p8txbuff[0]=0x68;
							p8txbuff[1]=(29+17+2);//需总发送字节数
							p8txbuff[2]=(29+17+2)>>8;
							//p8txbuff[3]=0x41;//控制码为0x41
							//p8txbuff[4]=4;//通信模块标识=1
							//p8txbuff[5]=0;
							p8txbuff[6]=0;//预计应答字节数
							//p8txbuff[7]=0;
							//p8txbuff[8]=0;
							UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
							p8txbuff[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
							//MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
							//MR(((u32)p8txbuff)+16,ADDR_Data+1,6);//从节点地址(目的地址)
							//p8txbuff[22]=0x13;//AFN=13H
							//p8txbuff[23]=1;//Fn=1
							//p8txbuff[24]=0;
							//p8txbuff[25]=2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
							//p8txbuff[26]=0;//通信延时相关标志
							//p8txbuff[27]=0;//附属节点数=0
							p8txbuff[28]=17;//DL645报文长度
							p8txbuff[29+8]=0x12;
							p8txbuff[29+9]=5;
							p8txbuff[29+14]=UARTCtrl->SubsequentFrames+0x33;
							p8txbuff[29+15]=0;//cs
							for(i=0;i<15;i++)
							{
								p8txbuff[29+15]+=p8txbuff[29+i];
							}
							p8txbuff[29+16]=0x16;
							Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
						}
					#endif//#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
						break;
				}
				UARTCtrl->TxByte=31+p8txbuff[28];//发送字节数(半字对齐)
				i=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
				UARTCtrl->TxToRxTimerOver=i;
				UARTCtrl->Task=3;//启动发送
				
				//显示抄表地址
				switch(UARTCtrl->RMTaskC)
				{
					case 0://上电
						CopyString((u8 *)"载波启抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 1://日抄
						if(UARTCtrl->DateReRMCount<=1)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
						{
							CopyString((u8 *)"载波日抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						}
						else
						{
							CopyString((u8 *)"载波补抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						}
						break;
					case 2://周抄
						CopyString((u8 *)"载波时抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						//CopyString((u8 *)"载波周抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 3://定抄
						CopyString((u8 *)"载波定抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 4://每小时重点户曲线抄读(仅用于无曲线冻结的单相表)
						CopyString((u8 *)"载波重点",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case (MaxRMTask-2)://周期抄读电能表事件
						switch(UARTCtrl->RMEventTaskC)//当前抄读电能表事件任务:0=空,1-3=1-3级
						{
							case 1:
								CopyString((u8 *)"载波 1级",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 2:
								CopyString((u8 *)"载波 2级",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 3:
								CopyString((u8 *)"载波 3级",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
						}
						break;
					default://循抄
						CopyString((u8 *)"载波循抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
				}
			#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
				if(UARTCtrl->RMMeterEvent&1)//正在抄读电能表事件标志;b0=主动上报事件,b1=
				{
					CopyString((u8 *)"载波上报",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				}
			#endif
				for(i=0;i<6;i++)
				{
					x=p8txbuff[30+5-i];
					MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
					MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
				}
				MWR(0,ADDR_DATABUFF+8+12,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字
				return;
			}
			//break;//不要,因上是while(1)
			
		case 1://1=完成1帧发收
			Terminal_Ram->RouterCheck_S_Timer=(10*60);//载波路由器死机秒定时值
			UARTCtrl->Task=0;//空闲1次
			
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//否认回答
				return;
			}
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//取从节点地址
			x=Terminal_Router->NoteNum;//当前路由器中总从节点数
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
			for(ListNo=0;ListNo<x;ListNo++)
			{
				d64b=p32[1]&0xffff;
				d64b<<=32;
				d64b|=p32[0];
				if(d64a==d64b)
				{
					break;
				}
				p32+=(LEN_NoteList/4);
			}
			if(ListNo==x)
			{//没找到内存中从节点列表
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				return;
			}
			p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
			switch(Protocol)
			{
				case 1://DL/T645-1997
					i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
					MaxFnCount=Get_RM_DL645_1997_MaxFn();
					break;
			#if ((USER/100)==5)//上海用户
				case 21://上海规约
					i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
					break;
			#endif
				default://DL/T645-2007
					i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
					break;
			}
			if(i==0x0)
			{//本Fn无抄表
				Terminal_Note->List[ListNo].FnCount=0;//Fn计数=0
				Terminal_Note->List[ListNo].RMCount=0;//抄表命令计数=0
				Terminal_Note->List[ListNo].ReRMCount=0;//抄表命令重发计数
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束				
				break;
			}
			p8RMLib=(u8 *)i;//抄表库
			
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=1000/10;
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=正在接收
				return;
			}
			y=p8rxbuff[28];//返回数据域长度
			if(y==0)
			{//接收数据长度=0
				return;
			}
			i=MRR(((u32)p8rxbuff)+25,2);//线路上传输延时时间秒
			UARTCtrl->TransferDelay=i;
			
			Info=MRR(((u32)p8rxbuff)+4,4);
			Fn=p8rxbuff[8];//鼎信模块用暂存主动上报标志
			MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,y);
			for(i=0;i<y;i++)
			{//去返回645帧的前导0xfe
				if(p8rxbuff[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rxbuff,((u32)p8rxbuff)+i,y-i);
					}
					break;
				}
			}
			
			if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
			{//起始符0x68错
				return;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//帧返回表地址
			if(d64a!=d64b)
			{//地址错
				return;
			}
			x=0;//和校验计算
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if((x!=p8rxbuff[i])||(p8rxbuff[i+1]!=0x16))
			{//和校验错误和结束符错
				return;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//数据域减0x33
				p8rxbuff[10+i]-=0x33;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//从站异常回答,无此数据项,正常跳过
//20150310
			#if (USER/100)==4//陕西用户
				Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1
				if(Protocol==1)
				{////DL/T645-1997
					if(p8RMLib[0]==0xE3)
					{
						if((p8txbuff[39]==0x52) && (p8txbuff[40]==0xC3))
						{
							p8txbuff[39]=0x43;
							p8txbuff[41]-=0x0f;
							p8txbuff[43]-=0x0f;
							p8txbuff[43]-=0x0f;
							//继续接收
							UARTCtrl->TxByte=31+p8txbuff[28];//发送字节数(半字对齐)
							i=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
							UARTCtrl->TxToRxTimerOver=i;
							UARTCtrl->Task=3;//启动发送
							return;
						}
					}
				}
			#endif
//xxx
				if(p8RMLib[0]==2)
				{//抄主动上报,抄读事件返回非法时,计数减
					i=Terminal_Note->List[ListNo].RMCount;
					switch(i)//抄表命令计数
					{
						case 0:
						case (2+(96*2))://复位主动上报状态字
						case 1://读主动上报状态字(仅当鼎信模块时)
						case (2+(96*2)+1)://读主动上报状态字
							break;
						default://读次数和各事件
							if((i&1)!=0)//读1-10次事件
							{
								//需读计数-1
								i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
								if(i>10)
								{
									i=10;
								}
								if(i!=0)
								{
									i--;
								}
								MWR(i,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
							}
					}
				}
				Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
				Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
				Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
				return;
			}
			Pn=Terminal_Note->List[ListNo].Pn;
			
	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
			if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
			{//鼎信模块
				if(Fn==0x01)
				{//夹带上报模式
					x=Terminal_Note->List[ListNo].MeterNo;//取对应电能表/交流采样装置序号(从1开始)
					if((x!=0)&&(x<=AFN04F10MaxPn))
					{//序号有效
						x--;
						MR(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
						MWR(1,ADDR_DATABUFF+1,1);//B1保留,现用于表示有主动上报
						MWR(1,ADDR_DATABUFF+96,1);//数据或
						MW(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
					}
				}
			}
		#endif
			if(UARTCtrl->SubsequentFrames!=0)//0=无读后续帧,1-255=DL645读续帧的帧号
			{//是后续帧
				if((p8RMLib[0]<2)||(p8RMLib[0]>3))
				{//不是读主动上报事件和周期读电能表事件
					i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
					if(i==0x04001501)
					{//是收到主动上报帧
						if((Terminal_Note->List[ListNo].RMCount+1)>=p8RMLib[1])//抄表命令计数
						{//只记1次,否则当主动上报无事件计数时每Fn中有多个抄表命令时会记多次.
							WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
						}
						UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
						Terminal_Note->List[ListNo].ReRMCount=0;//抄表命令重发计数
						return;
					}
					if(p8rxbuff[8]&0x20)
					{//有后续帧
						UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
					}
					else
					{//无后续帧
						UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
					}
					Terminal_Note->List[ListNo].ReRMCount=0;//抄表命令重发计数
					return;
				}
			}
			if(p8rxbuff[8]&0x20)
			{//有后续帧
			/*
			#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
				UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
			#else
				if(p8RMLib[0]==3)
				{//只有周期读电能表事件时才允许后续帧
					UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
				}
			#endif
			*/
				if((p8RMLib[0]==2)||(p8RMLib[0]==3))
				{//载波只有主动抄事件或周期读电能表事件时才允许后续帧
					UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
				}
			}
			else
			{//无后续帧
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
			}
	#endif
			switch(Protocol)
			{
				case 1://DL/T645-1997
					x=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//取DI
					y=MRR(((u32)p8rxbuff)+10,2);
//20150310
				#if (USER/100)==4//陕西用户
					if(p8RMLib[0]==0xE3)
					{
						if(y==0x9010)
						{
							y=0x901f;
						}
					}
				#endif
//20150310
					if(x!=y)
					{//DI错误
						//继续接收
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
						UARTCtrl->RxByte=0;
						UARTCtrl->Task=5;//5=正在接收
						return;
					}
					if(p8rxbuff[9]==2)
					{//电科院测试软件当事件记录无数据时,只返回2字节数据标识,同从站异常回答
						Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
						Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
						Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
						return;//上报返回
					}
					i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];
					if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
					{//返回长度相同
						UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
						UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
						ReadMeter_DL645_1997_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
						Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//返回的抄表数据标识计数
						ReadMeterNote_OK(PORTn,Pn,Info);//集中抄表电表抄读信息.成功
						DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
//						switch(p8RMLib[0])
//						{
//							case 129://F129 当前正向有功电能示值
//							case 162://F162 电能表日历时钟
//							case 0xe3://扩展3 集中抄表日冻结正向有功电能示值（总、费率1～M）
//						}
					}
					break;
			#if ((USER/100)==5)//上海用户
				case 21://上海规约
					x=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//取DI
					y=MRR(((u32)p8rxbuff)+8,1);
					if(x!=y)
					{//DI错误
						//继续接收
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
						UARTCtrl->RxByte=0;
						UARTCtrl->Task=5;//5=正在接收
						return;
					}
					i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];
					if(p8rxbuff[9]==i)
					{//返回长度相同
						UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
						UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
						ReadMeter_ShH_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
						Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//返回的抄表数据标识计数
						ReadMeterNote_OK(PORTn,Pn,Info);//集中抄表电表抄读信息.成功
						DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
//						switch(p8RMLib[0])
//						{
//							case 129://F129 当前正向有功电能示值
//							case 162://F162 电能表日历时钟
//							case 0xe3://扩展3 集中抄表日冻结正向有功电能示值（总、费率1～M）
//						}
					}
					break;
			#endif
				default://DL/T645-2007
					y=MRR(((u32)p8rxbuff)+10,4);
					i=y;
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//不是读电表曲线
						x=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//取DI
						//读电能表的上1-10次事件,数据标识与上x次有关
						if((p8RMLib[0]==2)&&((Terminal_Note->List[ListNo].RMCount&1)!=0))
						{
							if((Terminal_Note->List[ListNo].RMCount>=2)&&(Terminal_Note->List[ListNo].RMCount<=(2+(2*96))))
							{
								x&=0xffffff00;
								y&=0xffffff00;
							}
						}
						if((p8RMLib[0]==3)&&(UARTCtrl->RMDI!=0xffffffff))
						{//周期事件抄读非库中命令时
							x=UARTCtrl->RMDI;//DI
						}
						if((p8RMLib[0]==3)&&((Terminal_Note->List[ListNo].RMCount&1)!=0))
						{
							x&=0xffffff00;
							y&=0xffffff00;
						}
//20150113
					#if (USER/100)==4//用户标识:陕西
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
						if((i==1)&&(p8RMLib[0]==0xE3))
						{//变换日冻结数据标识
							switch(Terminal_Note->List[ListNo].RMCount)
							{
								default://日冻结时间YYMMDDhhmm
									break;
								case 1://上1次日正向有功电能示值（总、费率1～M）
									x=0x00010000;//当前正向有功电能示值（总、费率1～M）
									break;
							}
						}
					#endif
//20150113
					}
					else
					{//是读电表曲线
						x=MRR((u32)&p8RMLib[10],4);//取DI
					}
					if(x!=y)
					{//DI错误
					#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
						//继续接收
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
						UARTCtrl->RxByte=0;
						UARTCtrl->Task=5;//5=正在接收
						return;
					#else
						//EventProject==1
						if((p8RMLib[0]==2)&&((Terminal_Note->List[ListNo].RMCount==(2+(96*2)))||(i==0x04001501)))
						{//F2 电能表主动上报事件的复位主动上报状态字回答或后续帧
						}
						else
						{
							if((p8RMLib[0]==3)&&(i==0x04001501))
							{//F3 周期读电能表事件
							}
							else
							{
								//继续接收
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
								UARTCtrl->RxByte=0;
								UARTCtrl->Task=5;//5=正在接收
								return;
							}
						}
					#endif
					}
					if(p8rxbuff[9]==4)
					{//电科院测试软件当事件记录无数据时,只返回4字节数据标识,同从站异常回答
						Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
						Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
						Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
						return;//上报返回
					}
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//不是读电表曲线
						i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];
					}
					else
					{//是读电表曲线
						i=p8RMLib[10+4];
					}
				#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				#else
					if((p8RMLib[0]==3)&&(UARTCtrl->RMDI!=0xffffffff))
					{
						goto SKIP;//周期事件抄读非库中命令时无返回长度可比较
					}
					if((i==0x0)||(p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				#endif
					{//返回长度相同
			SKIP:
						UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
						UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
						ReadMeter_DL645_2007_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
						Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//返回的抄表数据标识计数
						
						Fn=p8RMLib[0];
						//读电表1-6类负荷曲线
						if((Fn>=0xe4)&&(Fn<=0xea))
						{
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
							Terminal_Note->List[ListNo].RMCountErr=255;//Fn内有抄读失败或不抄的数据标识计数=最大值免最次存储数据
							MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
						}
						//75 扩展13 上1-24整点电表冻结的正反有功电能示值
						if(Fn==0xED)
						{
							d64a=MRR(((u32)p8FnData)+6+1,4);//取电表返回的冻结时标年月日时
							d64a<<=8;
							AddCurveCongeal(d64a,101,Pn,((u32)p8FnData)+6+1+4,4);//补曲线冻结点数据
							AddCurveCongeal(d64a,103,Pn,((u32)p8FnData)+6+1+4+4,4);//补曲线冻结点数据
						#if (USER/100)==11//吉林系
							//吉林台体测试有整半点冻结数据
							d64a=YMDHM_SubM(d64a,30);//年月日时分减m分,返回减m分后的年月日时分
							AddCurveCongeal(d64a,101,Pn,((u32)p8FnData)+6+1+4,4);//补曲线冻结点数据
						#endif
						}
						#if (USER/100)==11//吉林系
						if(Fn==0xFB)
						{
							d64a=MRR(((u32)p8FnData)+6+1,4);//取电表的冻结时标年月日时
							d64a<<=8;
							AddCurveCongeal(d64a,89,Pn,((u32)p8FnData)+6+1+4,2);//补曲线冻结点数据
							AddCurveCongeal(d64a+0x15,89,Pn,((u32)p8FnData)+6+1+4+2,2);//补曲线冻结点数据
							AddCurveCongeal(d64a+0x30,89,Pn,((u32)p8FnData)+6+1+4+2+2,2);//补曲线冻结点数据
							AddCurveCongeal(d64a+0x45,89,Pn,((u32)p8FnData)+6+1+4+2+2+2,2);//补曲线冻结点数据
						}
						if(Fn==0xFA)
						{
							if(UARTCtrl->RMCount==2)
							{
								i=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
								i=YM_Sub1M(i);//年月减1月,返回减1后的年月
								AddMonthCongeal(i,35,Pn,((u32)p8FnData)+6+1,93);//补月冻结点数据
							}
						}
						#endif
						ReadMeterNote_OK(PORTn,Pn,Info);//集中抄表电表抄读信息.成功
						DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
//						switch(p8RMLib[0])
//						{
//							case 129://F129 当前正向有功电能示值
//							case 162://F162 电能表日历时钟
//							case 0xe3://扩展3 集中抄表日冻结正向有功电能示值（总、费率1～M）
//						}
					}
					else
					{
					#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
						if(p8RMLib[0]==2)
						{//F2 电能表主动上报事件
							if(p8rxbuff[9]>=4)
							{
								goto SKIP;//不比较长度
							}
						}
					#endif
					}
					break;
			}
			Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
			Terminal_Note->List[ListNo].RMCount++;//抄表数据标识+1
			break;
		case 2://2=接收超时(或错误)
//			if(Terminal_Ram->RouterCheck_S_Timer==0)//载波路由器死机秒定时器
//			{
//				Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
//				break;
//			}
			UARTCtrl->Task=0;//空闲1次
			break;
	}
	
}

#endif

