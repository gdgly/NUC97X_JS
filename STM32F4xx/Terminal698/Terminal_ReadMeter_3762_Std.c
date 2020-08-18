
//鼎信技术支持:徐剑英	13625322351
//             李勇 18661440042

//东软路由问题联系人：吴常玉，手机13589207445,电话0532-83676919,邮箱wucy@eastsoft.com.cn

//瑞斯康15989482340 洪爱霞



#include "Project.h"
#include "Terminal_ReadMeter_3762_Std.h"
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



void Uart_3762_AFN14F1_Tx(u32 PORTn,u32 RMFlag)//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
{
	u32 x;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{
		p8txbuff[3]=0x03;//控制码;集中式路由载波通信
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8txbuff[3]=0x0a;//控制码;微功率无线通信
		}
		else
		{
			p8txbuff[3]=0x01;//控制码;集中式路由载波通信
		}
	}
	p8txbuff[4]=4;//通信模块标识=1
	MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
	MR(((u32)p8txbuff)+16,((u32)p8rxbuff)+14,6);//从节点地址(目的地址)
	p8txbuff[22]=0x14;//AFN=0x14
	p8txbuff[23]=0x01;//Fn=1
	p8txbuff[24]=0x00;//Fn=1
	p8txbuff[25]=RMFlag;//抄读标志=0抄读失败,=1抄读成功,2=抄读
	p8txbuff[26]=0;//通信延时相关性标志,0=无关,1=有关
	p8txbuff[27]=0;//DL645帧长度=0
	p8txbuff[28]=0;//附属节点数量N=0
	x=29+2;//需总发送字节数
	p8txbuff[1]=x;
	p8txbuff[2]=x>>8;
	UARTCtrl->TxByte=x;//发送字节数(半字对齐)
	/*
	switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
	{
		default://其他
		case 2://鼎信
			break;
		case 3://东软
			p8txbuff[4]=0x0;//通信模块标识=0
			MR(((u32)p8txbuff)+10,((u32)p8txbuff)+22,7);
			x=17+2;//需总发送字节数
			p8txbuff[1]=x;
			p8txbuff[2]=x>>8;
			UARTCtrl->TxByte=x;//发送字节数(半字对齐)
			break;
	}
	*/
	Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
	UARTCtrl->Task=3;//等待发送			
	UARTCtrl->TxToRxTimerOver=0;//1000/10//发送后接收超时定时值=0,使能很快超时而转空闲以便数据转发
}


void GeneralReadMeter_3762_Std(u32 PORTn)//标准载波模块,常规抄表
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	u32 z;
#endif
	u32 ClassNo;
	u32 ListNo;
	u32 Fn;
	u32 Pn;
	u32 MaxFnCount;
	u32 Protocol;
	u32 AddrAFN04F10;
	u64 d64a;
	u64 d64b;
	u32 Info;
	
	u8 * p8RMLib;
	u32 * p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
	u32 AutoReportSearch;//主动上报搜索标志
#endif
#endif

#if ((USER/100)==5)||((USER/100)==8)||((USER/100)==7)//上海、通用、浙江
	u8 *p8;
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	
	switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		case 0://0=空闲
			UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			UARTCtrl->Task=5;//正在接收
			break;
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
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
			if((i<RMM_EndHM)&&(i>=x))
			{//不是在每日0点附近暂停抄表
				if((Terminal_Router->StopFlags&(1<<14))!=0)//暂停路由标志:b14=等待时钟过0
				{
					Terminal_Router->StopFlags&=~(1<<14);//暂停路由标志:b14=等待时钟过0
					Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				}
			}
		#else
			if((i<RMM_EndHM)&&(i>=RMM_StartHM))
			{//不是在每日0点附近暂停抄表
				if((Terminal_Router->StopFlags&(1<<14))!=0)//暂停路由标志:b14=等待时钟过0
				{
					Terminal_Router->StopFlags&=~(1<<14);//暂停路由标志:b14=等待时钟过0
					Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				}
			}
		#endif
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 载波路由器死机秒定时器
			{
				Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
		case 1://1=完成1帧发收
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//载波路由器死机秒定时值
			i=p8rxbuff[4];//读通信模块标识
			i&=0x4;
			if(i==0x0)
			{//集中器通信模块
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				switch(i)
				{
					case 0x1401://AFN=0x14,Fn=1;路由器发送请求抄表
						if(UARTCtrl->RMTaskC!=0)//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=定时任务,(MaxRMTask-1)=循抄任务(只有循抄任务才可插入高优先级抄表)
						{//不是启抄
							i=MRR(ADDR_TYMDHMS+1,2);
							if((i>=RMM_EndHM)||(i<RMM_StartHM))
							{//在每日0点附近暂停抄表
								UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								WarningOccur(PORTn,(u8 *)"载波停抄等待时钟过0");//告警发生,入口告警代码和告警字符串
								if(Terminal_Router->StopFlags==0)//暂停路由标志:b14=等待时钟过0
								{
									Terminal_Router->StopFlags|=(1<<14);//暂停路由标志:b14=等待时钟过0
									Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
								}
								return;
							}
						}
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//复制10字节接收头到发送头
						d64a=MRR(((u32)p8rxbuff)+14,6);//取从节点地址
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
						{//没找到内存中从节点列表,直接返回抄读成功
							Uart_3762_AFN14F1_Tx(PORTn,1);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
							return;
						}
						if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
						{//本任务己成功抄表,直接返回抄读成功
							Uart_3762_AFN14F1_Tx(PORTn,1);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
							if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]==2)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
							{
								CopyString((u8 *)"载波成功",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							}
							else
							{
								CopyString((u8 *)"载波不抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							}
						//dispskip:
							d64a<<=16;
							for(i=0;i<6;i++)
							{
								x=d64a>>56;
								d64a<<=8;
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
							return;
						}
						p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
						x=Terminal_Note->List[ListNo].MeterNo;//取对应电能表/交流采样装置序号(从1开始)
						if((x==0)||(x>AFN04F10MaxPn))
						{//序号错,不抄,直接返回抄读成功
					#if (USER/100)!=0
						RMOK:
					#endif
							Terminal_Note->List[ListNo].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
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
								goto RMOK;//不抄,直接返回抄读成功
							}
						}
					#endif
						x--;
						AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
						d64b=MRR(AddrAFN04F10+6,6);//取配置的通信地址
						if(d64a!=d64b)
						{//配置变化
						RouterSyn:
							if(Terminal_Ram->RouterSynDelay_S_Timer==0)//AFN04F10设置变化标志0=没,1=变化;用于档案同步
							{//删除时不能马上同步,因电科院互换性测试软件约有30秒设置时间不响应
								Terminal_Router->AFN04F10=0;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
								Terminal_Router->RouterSynTask=1;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
								UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								return;
							}
							Terminal_Note->List[ListNo].RMErr=1;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
							goto END_Fn;
						}
						Pn=MRR(AddrAFN04F10+2,2);//取配置的测量点号
						if(Pn==0)
						{//配置删除
							goto RouterSyn;//路同步
						}
					#if (USER/100)!=0//电科院测试设置F33等BUG
						i=MRR(ADDR_AFN04F30+(Pn-1),1);//台区集中抄表停抄/投抄设置(测量点号)
						if(i!=0)
						{//停抄
							goto RMOK;//不抄,直接返回抄读成功
						}
					#endif
						Protocol=MRR(AddrAFN04F10+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
						switch(Protocol)
						{
							case 1://DL/T645-1997
								MaxFnCount=Get_RM_DL645_1997_MaxFn();
								break;
						#if (USER/100)==5//上海用户
							case 21://上海规约
								MaxFnCount=Get_RM_ShH_MaxFn();
								break;
						#endif
							default://DL/T645-2007
								MaxFnCount=Get_RM_DL645_2007_MaxFn();
								break;
						}
						if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
						{//Fn计数>=最大Fn个数,本表抄表结束
			END_Fn:
							if(Terminal_Note->List[ListNo].RMErr==0)//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
							{//成功
								Uart_3762_AFN14F1_Tx(PORTn,1);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
								Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
							}
							else
							{//至少有1个数据标识抄读失败
								Uart_3762_AFN14F1_Tx(PORTn,0);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
								Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
								
								CopyString((u8 *)"载波失败",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								d64a<<=16;
								for(i=0;i<6;i++)
								{
									x=d64a>>56;
									d64a<<=8;
									MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
									MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
								}
								MWR(0,ADDR_DATABUFF+8+12,1);
								WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
							}
							Terminal_Note->List[ListNo].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
							Terminal_Note->List[ListNo].FnCount=0;//Fn计数
							Terminal_Note->List[ListNo].RMCount=0;//抄表数据标识计数
							Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发次数
							Terminal_Note->List[ListNo].RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
							UARTCtrl->RMCount=0;//将此值也置为0,进入重点户抄读曲线判断用
							return;
						}
						
						ClassNo=MRR(AddrAFN04F10+26,1);//取大小类号
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
							#if (USER/100)==5//上海用户
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
								if(UARTCtrl->RMTaskC!=0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
								{//不是启抄
									if(AutoReportSearch==0)//主动上报搜索标志
									{
										AutoReportSearch=1;//主动上报搜索标志
										x=Check_Meter_Event_Save();//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
										if(x>=10)
										{//空间至少可存10条
											x=Check_AutoReportWord(PORTn);//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
											x>>=16;
											if(x!=0)
											{//有主动上报需要读
												Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485抄表Fn计数(原常规抄表的Fn计数寄存)
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
											#if (USER/100)==5//上海用户
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
													MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),LEN_AutoReportWord);
													UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
													return;
												}
												
												Terminal_Note->List[ListNo].FnCount=2;
												AutoReportSearch=0;//主动上报搜索标志
												UARTCtrl->RMMeterEvent|=1;//正在抄读电能表事件标志;b0=主动上报事件,b1=
												UARTCtrl->FnEnd=1;//485抄表Fn计数结束标志,0=结束,1=没结束
												
												Terminal_Router->StopFlags|=(1<<17);//暂停路由;b17=抄主动上报事件
												Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
												UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
												UARTCtrl->Task=0;//空闲
												return;
											}
										}
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
							UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//复制此值,进入重点户抄读曲线判断用
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
							if(Fn==26)
							{
							#if (USER/100)==5//上海
								if((Protocol!=1) && (Protocol!=21))//(1=1997,21=上海规约,其他=2007)
							#else
								if(Protocol!=1)//(1=1997,其他=2007)
							#endif
								{//2007
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
										i=MRR(AddrAFN04F10+18,1);//取电能表费率
										if(i==1)
										{
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
								ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
								if(p8RMLib[0]==129)
								{
									ClrERC29SOUR(Pn);//集中抄表失败清ERC29电能表飞走事件原始数据
								}
								//日抄表标志
								if(UARTCtrl->RMTaskC==1)//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=定时任务,(MaxRMTask-1)=循抄任务(只有循抄任务才可插入高优先级抄表)
								{//
									if(UARTCtrl->DateReRMCount>=3)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
									{
										DateRMFlags(Pn,1);//日抄表标志;入口:f=0成功,=1失败
									}
								}
								Terminal_Note->List[ListNo].RMErr=1;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
								Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
								Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数
								Terminal_Note->List[ListNo].ReRMCount=0;//重发计数
								Terminal_Note->List[ListNo].RMErr_0x901F=0;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
							#if (USER/100)==5//上海
								if(Protocol!=21)//(1=1997,21=上海规约,其他=2007)
								{
									if((p8RMLib[0]==0xe3)||(p8RMLib[0]==0xee)||(p8RMLib[0]==0xef))
									{//抄上1-3日日冻结数据
										if(Terminal_Note->List[ListNo].RMCount==1)//抄表数据标识计数
										{//是第1个抄日冻结时间YYMMDDhhmm错误后继抄到也无用
											goto END_Fn;//结束,快速返回失败,以便学抄快速切换路由
										}
									}
								}
							#else
								if((p8RMLib[0]==0xe3)||(p8RMLib[0]==0xee)||(p8RMLib[0]==0xef))
								{//抄上1-3日日冻结数据
									if(Terminal_Note->List[ListNo].RMCount==1)//抄表数据标识计数
									{//是第1个抄日冻结时间YYMMDDhhmm错误后继抄到也无用
										goto END_Fn;//结束,快速返回失败,以便学抄快速切换路由
									}
								}
							#endif
								if(UARTCtrl->RMTaskC==2)
								{//周抄或替代的时抄,为加快任务退出不抄后继数据项
									goto END_Fn;
								}
								continue;
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
						
							switch(Protocol)
							{
								case 1://DL/T645-1997
									//取DI同时加0x33
									i=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//取DI
								#if (USER/100)==4//陕西用户
									if(p8RMLib[0]==0xE3)
									{
										if(i==0x901f)
										{
											if(Terminal_Note->List[ListNo].RMErr_0x901F==2)//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
											{
												i=0x9010;
												Terminal_Note->List[ListNo].RMErr_0x901F=0;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
											}
											else
											{
												Terminal_Note->List[ListNo].RMErr_0x901F=1;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
											}
										}
										else
										{
											Terminal_Note->List[ListNo].RMErr_0x901F=0;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
										}
									}
								#endif
									p8txbuff[28+10+0]=i+0x33;
									p8txbuff[28+10+1]=(i>>8)+0x33;
									//取通信地址
									MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
									p8txbuff[28+0]=0x68;
									p8txbuff[28+7]=0x68;
									p8txbuff[28+8]=0x01;
									p8txbuff[28+9]=0x2;
									p8txbuff[28+13]=0x16;
									//CS计算
									p8txbuff[28+12]=0;
									for(i=0;i<12;i++)
									{
										p8txbuff[28+12]+=p8txbuff[28+i];
									}
									//预计应答字节数
									i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];//返回长度
									if(i==0)
									{//返回长度=0
										i=10+(3*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
										i=p8RMLib[i];//取预计返回字节数
									}
									p8txbuff[6]=i+14;
									i=14;//DL645帧长度
									break;
							#if (USER/100)==5//上海用户
								case 21://上海规约
									p8txbuff[28+0]=0x68;
									//取通信地址
									MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
									//鼎信要求上海规约表通信地址A5固定发0xff;
									p8txbuff[28+6]=0xff;
									p8txbuff[28+7]=0x68;
									//取命令码
									i=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//取DI
									i|=0x80;
									p8txbuff[28+8+0]=i;
									//长度
									p8txbuff[28+9]=0;
									//CS计算
									p8txbuff[28+10]=0;
									for(i=0;i<10;i++)
									{
										p8txbuff[28+10]+=p8txbuff[28+i];
									}
									p8txbuff[28+11]=0x16;
									//预计应答字节数
									i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];//返回长度
									p8txbuff[6]=i+14;
									i=12;//上海规约帧长度
									break;
							#endif
								default://DL/T645-2007
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
												continue;
										}
									#if (USER/100)==4//用户标识:陕西
										x=MRR(AddrAFN04F10+18,1);//取电能表费率
										if((x==1)&&(p8RMLib[0]==0xE3))
										{//变换日冻结数据标识
											switch(Terminal_Note->List[ListNo].RMCount)
											{
												default://日冻结时间YYMMDDhhmm
													Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
													Terminal_Note->List[ListNo].RMCount++;//抄表命令计数
													Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
													continue;
												case 1://上1次日正向有功电能示值（总、费率1～M）
													i=0x00010000;//当前正向有功电能示值（总、费率1～M）
													break;
											}
										}
									#endif
										//DI同时加0x33
										p8txbuff[28+10+0]=i+0x33;
										p8txbuff[28+10+1]=(i>>8)+0x33;
										p8txbuff[28+10+2]=(i>>16)+0x33;
										p8txbuff[28+10+3]=(i>>24)+0x33;
										//取通信地址
										MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
										p8txbuff[28+0]=0x68;
										p8txbuff[28+7]=0x68;
										p8txbuff[28+8]=0x11;
										p8txbuff[28+9]=0x4;
										p8txbuff[28+15]=0x16;
										//CS计算
										p8txbuff[28+14]=0;
										for(i=0;i<14;i++)
										{
											p8txbuff[28+14]+=p8txbuff[28+i];
										}
										//预计应答字节数
										i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//返回长度
										if(i==0)
										{//返回长度=0
											i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
											i=p8RMLib[i];//取预计返回字节数
										}
										p8txbuff[6]=i+16;
										i=16;//DL645帧长度
									}
									else
									{//读电表曲线
										//补当前到上日0点曲线点
										d64a=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
									#if RMM_Carrier_CurveTime==15//载波通道读电表曲线的整冻结间隔时间分;15分或60分
										d64a=YMDHM_Int15(d64a);//年月日时分化为整15分,如00时18分化为00时15分
										i=Terminal_Note->List[ListNo].RMCount*15;//每15分间隔
									#else
										d64a&=0xffffffff00;
										i=Terminal_Note->List[ListNo].RMCount*60;//每60分间隔
									#endif
										d64a=YMDHM_SubM(d64a,i);//年月日时分减m分,返回减m分后的年月日时分
										i=p8RMLib[0];
										i-=0xe4;
										i<<=16;
										i+=0x06000001;
										MWR(i,((u32)p8txbuff)+28+10,4);
										MWR(1,((u32)p8txbuff)+28+10+4,1);//读1个点
										MWR(d64a,((u32)p8txbuff)+28+10+4+1,5);
										//加0x33
										for(i=0;i<10;i++)
										{
											p8txbuff[28+10+i]+=0x33;
										}
										//取通信地址
										MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
										p8txbuff[28+0]=0x68;
										p8txbuff[28+7]=0x68;
										p8txbuff[28+8]=0x11;
										p8txbuff[28+9]=10;
										p8txbuff[28+21]=0x16;
										//CS计算
										p8txbuff[28+20]=0;
										for(i=0;i<20;i++)
										{
											p8txbuff[28+20]+=p8txbuff[28+i];
										}
										//预计应答字节数
										i=p8RMLib[10+4];//返回长度
										if(i==0)
										{//返回长度=0
											i=10+(5*1);
											i=p8RMLib[i];//取预计返回字节数
										}
										p8txbuff[6]=i+16;
										i=22;//DL645帧长度
									}
									break;
							}
							if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{
								p8txbuff[3]=0x03;//控制码;集中式路由载波通信
							}
							else
							{
								if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
								{
									p8txbuff[3]=0x0a;//控制码;微功率无线通信
								}
								else
								{
									p8txbuff[3]=0x01;//控制码;集中式路由载波通信
								}
							}
							p8txbuff[4]=4;//通信模块标识=1
							MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
							MR(((u32)p8txbuff)+16,(u32)&Terminal_Note->List[ListNo].Addr,6);//从节点地址(目的地址)
//						#if ((USER/100)==5)//上海用户
//							if(Protocol==21)
//							{//上海规约
//								p8txbuff[16+4]=bcd_hex(p8txbuff[16+4]);//8位压缩BCD数转为HEX
//							}
//						#endif
							p8txbuff[22]=0x14;//AFN=0x14
							p8txbuff[23]=0x01;//Fn=1
							p8txbuff[24]=0x00;//Fn=1
							p8txbuff[25]=2;//抄读标志=2
							p8txbuff[26]=0;//通信延时相关性标志,0=无关,1=有关
							p8txbuff[27]=i;//DL645帧长度
							p8txbuff[28+i]=0;//附属节点数量N=0
							i+=29+2;//需总发送字节数
							p8txbuff[1]=i;
							p8txbuff[2]=i>>8;
							UARTCtrl->TxByte=i;//发送字节数(半字对齐)

							Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
							UARTCtrl->Task=3;//等待发送
							UARTCtrl->TxToRxTimerOver=1000/10;//发送后接收超时定时值ms/10(半字对齐)
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

								default://循抄
									CopyString((u8 *)"载波循抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									break;
							}
							for(i=0;i<6;i++)
							{
								x=p8txbuff[16+5-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
							return;
						}
						//break;//不要,因上是while(1)
					case 0x0602://AFN=0x06,Fn=2;路由器发送上报抄表数据
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//复制10字节接收头到发送头
						Info=MRR(((u32)p8rxbuff)+4,4);//取6字节376.2返回的信息域
						i=p8rxbuff[18];//读DL645帧长度
						UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+16,2);//线路上传输延时时间秒
						MR(((u32)p8rxbuff),((u32)p8rxbuff)+19,i);//取645帧
						goto RMESC;//抄表返回
					case 0x1402://AFN=0x14,Fn=2;路由请求集中器时钟
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//复制10字节接收头到发送头
						MW(ADDR_TYMDHMS,((u32)p8txbuff)+128,6);
						Uart_3762_TxData(PORTn,0x14,2,6);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
						break;
					//case 0x1404://AFN=0x14,Fn=3;请求依通信延时修正通信数据
						//break;
					default:
						UARTCtrl->RxByte=0;//接收字节数(半字对齐)
						UARTCtrl->Task=5;//正在接收
						break;
				}
			}
			else
			{//电能表通信模块
				i=MRR(((u32)p8rxbuff)+22,3);
				switch(i)
				{
					case 0x000206://AFN=0x06,Fn=2;路由器发送上报抄表数据
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//复制10字节接收头到发送头
						Info=MRR(((u32)p8rxbuff)+4,4);//取6字节376.2返回的信息域
						i=p8rxbuff[30];//读DL645帧长度
						UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+28,2);//线路上传输延时时间秒
						MR(((u32)p8rxbuff),((u32)p8rxbuff)+31,i);//取645帧
				RMESC://抄表返回
						for(y=0;y<i;y++)
						{//去返回645帧的前导0xfe
							if(p8rxbuff[y]!=0xfe)
							{
								if(y!=0)
								{
									MR((u32)p8rxbuff,((u32)p8rxbuff)+y,i-y);
								}
								break;
							}
						}
						d64a=MRR(((u32)p8rxbuff)+1,6);//帧返回表地址
						x=Terminal_Router->NoteNum;//当前路由器中总从节点数
						p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
						for(ListNo=0;ListNo<x;ListNo++)
						{
							d64b=p32[1]&0xffff;
							d64b<<=32;
							d64b|=p32[0];
						#if (USER/100)==5//上海
							i=Terminal_Note->List[ListNo].MeterNo;
							Protocol=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
							if(Protocol==21)
							{//上海规约
								//通信地址A5不用比较
								d64a&=0xffffffffff;
								d64b&=0xffffffffff;
							}
						#endif
							if(d64a==d64b)
							{
								break;
							}
							p32+=(LEN_NoteList/4);
						}
						if(ListNo==x)
						{//没找到内存中从节点列表
							goto RXESC;//上报返回
						}
						p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
						//MWR(Info,(u32)&Terminal_Note->List[ListNo].Info,4);//4字节最后次载波抄表返回的信息域
						
						x=Terminal_Note->List[ListNo].MeterNo;//取对应电能表/交流采样装置序号(从1开始)
						if((x==0)||(x>AFN04F10MaxPn))
						{//序号错
							goto RXESC;//上报返回
						}
						x--;
						AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
						Pn=MRR(AddrAFN04F10+2,2);//取配置的测量点号
						if(Pn==0)
						{//配置删除
							goto RXESC;//上报返回	
						}
						d64b=MRR(AddrAFN04F10+6,6);//取配置的通信地址
//					#if ((USER/100)==5)//上海用户
//						Protocol=MRR(AddrAFN04F10+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
//						if(Protocol==21)
//						{//上海规约
//							i=(d64b>>24)&0xff;
//							d64a=hex_bcd(i);//HEX数转为8位压缩BCD数
//							d64b&=0xffff00ffffff;
//							d64b+=(d64a<<24);
//						}
//						d64a=MRR(((u32)p8rxbuff)+1,6);//帧返回表地址
//					#endif
					#if (USER/100)==5//上海
						Protocol=MRR(ADDR_AFN04F10+(x*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
						if(Protocol==21)
						{//上海规约
							//通信地址A5不用比较
							d64a&=0xffffffffff;
							d64b&=0xffffffffff;
						}
					#endif
						if(d64a!=d64b)
						{//配置变化
							goto RXESC;//上报返回
						}
					#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
						if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
						{//鼎信模块
							if(p8txbuff[8]==0x01)
							{//夹带上报模式
								MR(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
								MWR(1,ADDR_DATABUFF+1,1);//B1保留,现用于表示有主动上报
								MWR(1,ADDR_DATABUFF+96,1);//数据或
								MW(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
							}
						}
					#endif
					#endif
						Protocol=MRR(AddrAFN04F10+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
						switch(Protocol)
						{
							case 1://DL/T645-1997
								i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						#if (USER/100)==5//上海用户
							case 21://上海规约
								i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						#endif
							default://DL/T645-2007
								i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						}
						p8RMLib=(u8 *)i;//抄表库
						
						if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
						{//起始符0x68错
							goto RXESC;//上报返回
						}
						x=0;//和校验计算
						for(i=0;i<(p8rxbuff[9]+10);i++)
						{
							x+=p8rxbuff[i];
						}
						x&=0xff;
						if((x!=p8rxbuff[i])||(p8rxbuff[i+1]!=0x16))
						{//和校验错误
							goto RXESC;//上报返回
						}
						for(i=0;i<p8rxbuff[9];i++)
						{//数据域减0x33
							p8rxbuff[10+i]-=0x33;
						}
					#if (USER/100)==5//上海用户
						if(Protocol!=21)//不是上海规约
						{
							if((p8rxbuff[8]&0x40)!=0x0)
							{//从站异常回答,无此数据项,正常跳过
								Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
								Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
								Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
								goto RXESC;//上报返回
							}
						}
					#else
						i=Terminal_Note->List[ListNo].RMErr_0x901F;//载波DL645-1997无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表
						Terminal_Note->List[ListNo].RMErr_0x901F=0;//载波DL645-1997无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表
						if((p8rxbuff[8]&0x40)!=0x0)
						{//从站异常回答,无此数据项,正常跳过
//20150310
						#if (USER/100)==4//用户:陕西
							if(Protocol==1)
							{//DL/T645-1997
								if(p8RMLib[0]==0xE3)
								{
									if(Terminal_Note->List[ListNo].RMCount==1)//抄表数据标识计数=1,命令=0x901f
									{
										if(i==1)//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
										{
											Terminal_Note->List[ListNo].RMErr_0x901F=2;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
											if(	Terminal_Note->List[ListNo].ReRMCount!=0)//抄表重发计数
											{
												Terminal_Note->List[ListNo].ReRMCount--;//抄表重发计数
											}
											goto RXESC;//上报返回
										}
									}
								}
							}
						#endif
							Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
							Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
							Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
							goto RXESC;//上报返回
						}
					#endif
						switch(Protocol)
						{
							case 1://DL/T645-1997
								x=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//取DI
								y=MRR(((u32)p8rxbuff)+10,2);
//20150310
							#if (USER/100)==4//用户:陕西
								if(p8RMLib[0]==0xE3)
								{
									if(y==0x9010)
									{
										y=0x901f;
									}
								}
							#endif
								if(x!=y)
								{//DI错误
									goto RXESC;//上报返回
								}
								if(p8rxbuff[9]==2)
								{//电科院测试软件当事件记录无数据时,只返回2字节数据标识,同从站异常回答
									Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
									Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
									Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
									goto RXESC;//上报返回
								}
								i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];
								if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
								{//返回长度相同
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
									ReadMeter_DL645_1997_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//返回的抄表数据标识计数
									ReadMeterNote_OK(PORTn,Pn,Info);//集中抄表电表抄读信息.成功
									DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
								#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
									if((p8RMLib[0]==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))//抄表命令计数
									{
										goto Time;
									}
								#endif
//									switch(p8RMLib[0])
//									{
//										case 129://F129 当前正向有功电能示值
//										case 162://F162 电能表日历时钟
//										case 0xe3://扩展3 集中抄表日冻结正向有功电能示值（总、费率1～M）
//									}
									
								}
								break;
						#if (USER/100)==5//上海用户
							case 21://上海规约
								x=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//取DI
								y=MRR(((u32)p8rxbuff)+8,1);
								if(x!=y)
								{//DI错误
									goto RXESC;//上报返回
								}
								i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];
								if((p8rxbuff[9]==i))
								{//返回长度相同
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
									ReadMeter_ShH_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//上海规约抄表接收数据填入Fn数据缓冲
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//返回的抄表数据标识计数
									ReadMeterNote_OK(PORTn,Pn,Info);//集中抄表电表抄读信息.成功
									DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
								}
								break;
						#endif
							default://DL/T645-2007
								if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
								{//不是读电表曲线
									x=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//取DI
								#if (USER/100)==4//用户标识:陕西
									i=MRR(AddrAFN04F10+18,1);//取电能表费率
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
								}
								else
								{//是读电表曲线
									x=MRR((u32)&p8RMLib[10],4);//取DI
								}
								y=MRR(((u32)p8rxbuff)+10,4);
								if(x!=y)
								{//DI错误
									goto RXESC;//上报返回
								}
								if(p8rxbuff[9]==4)
								{//电科院测试软件当事件记录无数据时,只返回4字节数据标识,同从站异常回答
									Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
									Terminal_Note->List[ListNo].RMCount++;//抄表数据标识计数+1
									Terminal_Note->List[ListNo].RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
									goto RXESC;//上报返回
								}
								if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
								{//不是读电表曲线
									i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];
								}
								else
								{//是读电表曲线
									i=p8RMLib[10+4];
								}
								if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
								{//返回长度相同
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//抄表数据标识计数
									ReadMeter_DL645_2007_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//返回的抄表数据标识计数
	
									Pn=Terminal_Note->List[ListNo].Pn;
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
							#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
									if((p8RMLib[0]==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))//抄表命令计数
									{
									Time:
								 		MR(ADDR_DATABUFF,((u32)p8FnData)+1+6,6);
										MR(ADDR_DATABUFF+6,((u32)p8FnData)+1,6);
										p8=(u8 *)(ADDR_DATABUFF);
										if(p8[0]!=0xee)
										{
											p8[4]&=0x1f;//去加在月字节上的星期
											p8[10]&=0x1f;//去加在月字节上的星期
											i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
											if(i==0)
											{//电能表时钟合法
												i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
												if((i&0x01)==0)
												{
													MWR(i|0x01,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
												}
												i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
												x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
												if(x<3)
												{
													MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
													MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//电能表对时前的误差
													MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//电能表对时后的误差
													MWR(0xff,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
												}
												if(x==3)
												{
													if(MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1)==0xff)//电能表对时后的结果
													{
														x=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//电能表对时前的误差
														if((x>>15)!=0x0)
														{//负
															x=~x;
															x++;
															MWR(x,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//电能表对时前的误差
														}
														if(x>MRR(ADDR_AFN04F298_Ext+7,1))
														{
															MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
															if((i>>31)!=0x0)
															{//负
																i=~i;
																i++;
															}
															MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//电能表对时后的误差
															if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
															{//成功
																MWR(1,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
															}
															else
															{//失败
																MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
															}
															//ERC51
															p8=(u8 *)ADDR_DATABUFF;
															//电能表对时前的误差
															MWR(x,ADDR_DATABUFF+9,2);
															//电能表对时后的误差
															MWR(i,ADDR_DATABUFF+11,2);
															p8[13]=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);
															ERC51_Event(Pn+(1<<15),ADDR_DATABUFF);//校时记录事件;入口:Pn测量点号(从0开始)
														#if (USER/100)==8//用户标识：通用
															p8=(u8 *)ADDR_TYMDHMS;
															MR((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1),3);
															x=DataComp((u32)ADDR_DATABUFF,(u32)ADDR_TYMDHMS+3,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
															if(x==0)
															{
																x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);
																if(x==0xffffff)
																{
																	DMWR(i,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);//寄存器直接写到存储器(<=8字节)(无擦除)
																}
															}
															//时间超差 5min 测量点数量
															if(i>300)
															{
																x=MRR(ADDR_AFN0DF321_Ext_Source,2);
																x++;
																MWR(x,ADDR_AFN0DF321_Ext_Source,2);
															}
															//超过时间阈值未超差5min测量点数量
															if((i>MRR(ADDR_AFN04F298_Ext+7,1))&&(i<300))
															{
																x=MRR(ADDR_AFN0DF321_Ext_Source+2,2);
																x++;
																MWR(x,ADDR_AFN0DF321_Ext_Source+2,2);
															}
														#endif
														}
														else
														{
															MWR(2,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
														}
														x=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),1);//电能表对时次数
														if(x<31)
														{
															x++;
														}
														MWR(x,ADDR_DATABUFF,1);
														MR(ADDR_DATABUFF+1,ADDR_AFN0CF250_Ext1+10*(Pn-1),10);
														if(x>=31)
														{
															x=30;
														}
														MR(ADDR_DATABUFF+11,ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1)+1,10*x);
														MR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),ADDR_DATABUFF,1+10*x+10);
													}
												}
											}
										}
									}
							#endif
//									switch(p8RMLib[0])
//									{
//										case 129://F129 当前正向有功电能示值
//										case 162://F162 电能表日历时钟
//										case 0xe3://扩展3 集中抄表日冻结正向有功电能示值（总、费率1～M）
//									}
									
								}
								break;
						}
						Terminal_Note->List[ListNo].ReRMCount=0;//抄表重发计数=0
						Terminal_Note->List[ListNo].RMCount++;//抄表数据标识+1
						
						//发确认回答上报抄表数据
					RXESC:
						MWR(0xffffffff,((u32)p8txbuff)+128,6);
						Uart_3762_TxData(PORTn,0,1,6);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
						if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{
							p8txbuff[3]=0x03;//控制码;集中式路由载波通信
						}
						else
						{
							if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{
								p8txbuff[3]=0x0a;//控制码;微功率无线通信
							}
							else
							{
								p8txbuff[3]=0x01;//控制码;集中式路由载波通信
							}
						}
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
						break;
					default:
						UARTCtrl->RxByte=0;//接收字节数(半字对齐)
						UARTCtrl->Task=5;//正在接收
						break;
				}
			}
			break;
	}
}



	#if(USER/100)==0
#define OnlyFnMaxReRMCount     2//1//抄表重发次数(仅重发次数,不算初始1次的发送);电力线载波=1,微功率无线=2
	#else
#define OnlyFnMaxReRMCount     0//抄表重发次数(仅重发次数,不算初始1次的发送)
	#endif
void RealTimeReadMeter_3762_Fn_Std(u32 PORTn,u8 *p8RMLib)//仅抄1个Fn数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterRealTimeTask)//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
	{
		default:
			Terminal_Router->RouterRealTimeTask=0;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
			break;
		case 0://空闲
			break;
		case 1://暂停路由
			if(UARTCtrl->Task<=2)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				Terminal_Router->StopFlags|=(1<<7);//暂停路由
				Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				Terminal_Router->RouterRealTimeTask++;//载波Fn>=129的实时抄读任务
			}
			break;
		case 2://AFN13 F1 路由数据转发:监控载波从节点
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//本序号的电能表/交流采样装置删除
			END_Fn:
				UARTCtrl->Task=0;//
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				Terminal_Router->StopFlags&=~(1<<7);//恢复路由
				Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				Terminal_Router->RouterRealTimeTask=0;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
				break;
			}
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
			if((p8RMLib[3]&0x04)==0)//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
			{//本Fn不是载波抄表
				goto END_Fn;
			}
			if(p8RMLib[1]==0x0)
			{//本Fn抄表命令数=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//抄表命令计数=0,清Fn数据缓冲
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(p8RMLib[0]==165)
			{//F165 电能表开关操作次数及时间
				//命令计数0-5,其中最后2项4-5仅用于事件记录实抄时不抄
//				dcdu 0x03300000//编程总次数
//				dcb 3//返回长度
//				dcdu 0x03300001//最近一次编程发生时刻
//				dcb 50//返回长度
//				dcdu 0x03300E00//电能表尾盖打开次数
//				dcb 3//返回长度
//				dcdu 0x03300E01//最近一次尾盖打开时间
//				dcb 60//返回长度
//				dcdu 0x03300D00//电能表开表盖打开次数(仅用于事件记录)
//				dcb 3//返回长度
//				dcdu 0x03300D01//最近一次开表盖记录(仅用于事件记录)
//				dcb 60//返回と
				if(UARTCtrl->RMCount>=4)
				{
					UARTCtrl->RMCount=6;
				}
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//抄表命令计数>=Fn需发命令数;本Fn抄表完成
				if(p8RMLib[0]==28)
				{//F28 电表运行状态字及其变位标志
					ReadMeter_SaveFnData(PORTn,p8RMLib[0],Pn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				}
				goto END_Fn;
			}
			
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//从节点地址(目的地址)
			switch(Protocol)
			{
				case 1://DL/T645-1997
					i=MRR((u32)&p8RMLib[10+(3*UARTCtrl->RMCount)],2);//取DI
					Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
					//预计应答字节数
					i=p8RMLib[10+(3*UARTCtrl->RMCount)+2];//返回长度
					if(i==0)
					{//返回长度=0
						i=10+(3*p8RMLib[1])+UARTCtrl->RMCount;
						i=p8RMLib[i];//取预计返回字节数
					}
					i+=14;
					Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
					if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
					{//微功率无线,档案地址用采集器地址
						d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
						if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
						{
							MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
							Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
						}
					}
					break;
			#if (USER/100)==5//上海用户
				case 21://上海规约
					i=MRR((u32)&p8RMLib[10+(2*UARTCtrl->RMCount)],1);//取DI
					Read_ShH_TxData(ADDR_DATABUFF,i,d64a);//上海规约读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
					//预计应答字节数
					i=p8RMLib[10+(2*UARTCtrl->RMCount)+1];//返回长度
					i+=14;
					//鼎信要求添加上海协议电表时，要求规约类型必须为“00”
					Uart_3762_AFN13F1_Tx(PORTn,0,ADDR_DATABUFF,12,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
					MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
					Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
					{//微功率无线,档案地址用采集器地址
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
			#if (USER/100)==6//湖南
					if(p8RMLib[0]==27)//F27 电能表日历时钟、编程次数及其最近一次操作时间等
					{
						switch(UARTCtrl->RMCount)
						{
							case 0://电能表日历时钟Hhmmss  r
							case 1://电能表日历时钟YYMMDDWW r
							case 2://电池工作时间 r
							case 3://编程总次数  r
							case 4://最近一次编程发生时刻  r
							//case 5://电表清零总次数
							//case 6://最近一次电表清零发生时刻
							case 7://需量清零总次数  r
							//case 8://最近一次需量清零发生时刻
							//case 9://事件清零总次数
							//case 10://最近一次事件清零发生时刻
							//case 11://校时总次数
							//case 12://最近一次校时发生时刻
								break;
							default:
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;//抄表命令重发计数
								return;
						}
					}
			#endif
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//不是读电表曲线
						i=MRR((u32)&p8RMLib[10+(5*UARTCtrl->RMCount)],4);//取DI
						Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
						//预计应答字节数
						i=p8RMLib[10+(5*UARTCtrl->RMCount)+4];//返回长度
						if(i==0)
						{//返回长度=0
							i=10+(5*p8RMLib[1])+UARTCtrl->RMCount;
							i=p8RMLib[i];//取预计返回字节数
						}
						i+=16;
						Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
						if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{//微功率无线,档案地址用采集器地址
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
								Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
							}
						}
					}
					else
					{//读电表曲线
						i=MRR(((u32)p8RMLib)+10,4);
						d64b=MRR((u32)&UARTCtrl->RMCommandYMDHM[0],5);//曲线的时标年月日时分
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
						{//微功率无线,档案地址用采集器地址
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
								Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
							}
						}
					}
					break;
			}
			UARTCtrl->TxByte=31+p8txbuff[28];//发送字节数(半字对齐)
			i=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;
			Terminal_Router->RouterRealTimeTask++;//载波Fn>=129的实时抄读任务
			
			//显示抄表地址
			CopyString((u8 *)"载波实抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[30+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 3://接收
			if(UARTCtrl->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(UARTCtrl->Task!=1)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
			#if OnlyFnMaxReRMCount!=0
				if(UARTCtrl->ReRMCount<OnlyFnMaxReRMCount)//485抄表命令重发计数
				{
					UARTCtrl->ReRMCount++;//485抄表命令重发计数
				}
				else
				{
					UARTCtrl->RMCount=200;//抄表命令计数,点抄失败不抄后续命令
					UARTCtrl->ReRMCount=0;//抄表命令重发计数
					ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
				}
			#else
				UARTCtrl->RMCount=200;//抄表命令计数,点抄失败不抄后续命令
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
			#endif
				UARTCtrl->Task=0;//空闲1次
				Terminal_Router->RouterRealTimeTask--;//载波Fn>=129的实时抄读任务
				break;
			}
			d64a=MRR(((u32)p8rxbuff)+4,6);
			ReadMeterNote_OK(PORTn,Pn,d64a);//集中抄表电表抄读信息.成功
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//否认回答
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//取配置的通信地址
//		#if (USER/100)==5//上海
//			if(Protocol==21)
//			{//上海规约
//				i=(d64a>>24)&0xff;
//				d64b=bcd_hex(i);//8位压缩BCD数转为HEX
//				d64a&=0xffff00ffffff;
//				d64a+=(d64b<<24);
//			}
//		#endif
			y=p8rxbuff[28];//返回数据域长度
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				y=0;//接收数据长度
			}
			if(y==0)
			{//接收数据长度=0
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			i=MRR(((u32)p8rxbuff)+25,2);//线路上传输延时时间秒
			UARTCtrl->TransferDelay=i;
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
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//返回的地址
		#if (USER/100)==5//上海
			if(Protocol==21)
			{//上海规约
				//通信地址A5不用比较
				d64a&=0xffffffffff;
				d64b&=0xffffffffff;
			}
		#endif
			if(d64a!=d64b)
			{//表地址不相同
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			x=0;//和校验计算
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//和校验错
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
		#if (USER/100)==5//上海
			if(Protocol==21)
			{//上海规约
				//DI比较
				i=MRR(((u32)p8txbuff)+29+8,1);//发出的数据标识
				x=MRR(((u32)p8rxbuff)+8,1);//返回的数据标识
				x|=0x80;
			}
			else
			{
				if((p8rxbuff[8]&0x40)!=0x0)
				{//控制码从站回答异常
					goto NEXT_RMCount;//下抄表命令计数
				}
				//DI比较
				i=MRR(((u32)p8txbuff)+29+10,4);//发出的数据标识
				x=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
				if(Protocol==1)
				{//1997
					i&=0xffff;
					x&=0xffff;
				}
			}
		#else
			if((p8rxbuff[8]&0x40)!=0x0)
			{//控制码从站回答异常
				goto NEXT_RMCount;//下抄表命令计数
			}
			//DI比较
			i=MRR(((u32)p8txbuff)+29+10,4);//发出的数据标识
			x=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
			if(Protocol==1)
			{//1997
				i&=0xffff;
				x&=0xffff;
			}
		#endif
			if(i!=x)
			{//DI不相同
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//数据域减0x33
				p8rxbuff[10+i]-=0x33;
			}
			switch(Protocol)
			{
				case 1://1997
					p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
					if(p8RMLib[0]!=0x0)
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+2))
						{
							if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
							{//块命令后可能有0xaa
								if(p8rxbuff[9]==2)
								{//电科院测试软件当事件记录无数据时,只返回2字节数据标识,同从站异常回答
									goto NEXT_RMCount;//下抄表命令计数
								}
								UARTCtrl->Task=2;//2=接收超时(或错误)
								break;
							}
						}
					}
					p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
					ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
					break;
			#if(USER/100)==5//上海
				case 21:
					i=p8RMLib[10+(2*UARTCtrl->RMCount)+1];
					if(p8rxbuff[9]==i)
					{//返回长度相同
						ReadMeter_ShH_FnData(PORTn,p8RMLib);//上海规约抄表接收数据填入Fn数据缓冲
					}
					break;
			#endif
				default://2007
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//不是读电表曲线
						p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
					}
					else
					{//是读电表曲线
						p8RMLib+=10+4;
					}
					if(p8RMLib[0]!=0x0)
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+4))
						{
							if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
							{//块命令后可能有0xaa
								if(p8rxbuff[9]==4)
								{//电科院测试软件当事件记录无数据时,只返回4字节数据标识,同从站异常回答
									goto NEXT_RMCount;//下抄表命令计数
								}
								UARTCtrl->Task=2;//2=接收超时(或错误)
								break;
							}
						}
					}
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//不是读电表曲线
						p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
					}
					else
					{
						p8RMLib-=10+4;
					}
					ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲				
					break;
			}
	NEXT_RMCount://下抄表命令计数
			UARTCtrl->RMCount+=1;//抄表命令计数+1
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->Task=0;//空闲1次
			Terminal_Router->RouterRealTimeTask--;//载波Fn>=129的实时抄读任务
			break;
	}
}

/*
void KeyUserReadMeter_3762_Std(u32 PORTn)//载波模块,重点户抄表
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Fn;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u32 ClassNo;
	u8 * p8RMLib;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterKeyUserTask)//重点户抄读任务
	{
		case 0://检查是否有重点户若有则暂停路由
			if(UARTCtrl->Task<=2)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{	
				y=MRR(ADDR_AFN04F35,1);//台区集中抄表重点户设置
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				while(1)
				{
					if(UARTCtrl->KeyUserRMCount>=y)//重点户抄读电能表计数
					{//全部重点户抄完
						Terminal_Router->RouterKeyUserTask=0;//重点户抄读任务
						UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfKeyUser;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
						UARTCtrl->FnEnd=UARTCtrl->FnEndBackupOfKeyUser;//485抄表Fn计数结束标志,0=结束,1=没结束(原常规抄表的Fn计数结束标志寄存)
						UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485抄表Fn计数(原常规抄表的Fn计数寄存)
						UARTCtrl->RMError=UARTCtrl->RMErrorBackupOfKeyUser;//485抄表本电能表通信失败标志,B0=失败,B1=F129失败(原常规抄表的失败标志寄存)
						UARTCtrl->KeyUserRMCount=0;//重点户抄读电能表计数
						UARTCtrl->RMKeyUser=0;//抄读重点户标志,0=常规抄读,1=重点户抄读
						return;
					}
					x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//重点户的电能表/交流采样装置序号
					if((x==0)||(x>AFN04F10MaxPn))
					{//装置序号错
				NEXT_KeyUser_Start:
						UARTCtrl->FnCount=0;//Fn计数
						UARTCtrl->RMCount=0;//485抄表命令计数
						UARTCtrl->ReRMCount=0;//485抄表命令重发计数
						UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
						continue;
					}
					UARTCtrl->MeterNo=x;
					x--;
					x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
					Pn=MRR(x,2);
					if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
					{//测量点号错
						goto NEXT_KeyUser_Start;//下一个重点户
					}
					i=MRR(x+2,1);
					i&=0x1f;
					if(i!=31)
					{//端口不是载波
						goto NEXT_KeyUser_Start;//下一个重点户
					}
					Protocol=MRR(x+3,1);//通信协议类型 BIN 1 
					if(Protocol!=1)
					{//不是DL/T645-1997,2007抄整点冻结
						goto NEXT_KeyUser_Start;//下一个重点户
					}
					break;
				}
				UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				Terminal_Router->StopFlags|=(1<<1);//暂停路由:b1=抄重点户
				Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				Terminal_Router->RouterKeyUserTask++;//重点户抄读任务
			}
			break;
		case 1://AFN13 F1 路由数据转发:监控载波从节点
			i=MRR(ADDR_AFN04F35,1);//台区集中抄表重点户设置
			if(i>MaxKeyUser)
			{
				i=MaxKeyUser;
			}
			if(UARTCtrl->KeyUserRMCount>=i)//重点户抄读电能表计数
			{//全部重点户抄完
				Terminal_Router->RouterKeyUserTask=0;//重点户抄读任务
//				UARTCtrl->KeyUserRMOKCountLast=UARTCtrl->KeyUserRMOKCount;//上次重点户抄读成功计数
//				UARTCtrl->KeyUserRMOKCount=0;//当前重点户抄读成功计数
				UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfKeyUser;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
				UARTCtrl->FnEnd=UARTCtrl->FnEndBackupOfKeyUser;//485抄表Fn计数结束标志,0=结束,1=没结束(原常规抄表的Fn计数结束标志寄存)
				UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485抄表Fn计数(原常规抄表的Fn计数寄存)
				UARTCtrl->RMError=UARTCtrl->RMErrorBackupOfKeyUser;//485抄表本电能表通信失败标志,B0=失败,B1=F129失败(原常规抄表的失败标志寄存)
				UARTCtrl->RMCount=0;//485抄表命令计数
				UARTCtrl->ReRMCount=0;//485抄表命令重发计数
				UARTCtrl->KeyUserRMCount=0;//重点户抄读电能表计数
				UARTCtrl->RMKeyUser=0;//抄读重点户标志,0=常规抄读,1=重点户抄读
				
				Terminal_Router->StopFlags&=~(1<<1);//恢复路由
				Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				return;
			}
			x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//重点户的电能表/交流采样装置序号
			if((x==0)||(x>AFN04F10MaxPn))
			{//装置序号错
			NEXT_KeyUser:
				UARTCtrl->FnCount=0;//Fn计数
				UARTCtrl->RMCount=0;//485抄表命令计数
				UARTCtrl->ReRMCount=0;//485抄表命令重发计数
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				break;
			}
			UARTCtrl->MeterNo=x;
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MRR(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//测量点号错
				goto NEXT_KeyUser;//下一个重点户
			}
			i=MRR(x+2,1);
			i&=0x1f;
			if(i!=31)
			{//端口不是载波
				goto NEXT_KeyUser;//下一个重点户
			}
			Protocol=MRR(x+3,1);//通信协议类型 BIN 1 
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[UARTCtrl->FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[UARTCtrl->FnCount];
			}
			if(i==0)
			{//本Fn是空
			NEXT_Fn:
				UARTCtrl->FnCount+=1;//Fn计数+1
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				UARTCtrl->RMError&=0xfe;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				if(Protocol==1)
				{//DL/T645-1997
					i=Get_RM_DL645_1997_MaxFn();
				}
				else
				{//DL/T645-2007
					i=Get_RM_DL645_2007_MaxFn();
				}
				if(UARTCtrl->FnCount>=i)
				{//Fn计数>=最大Fn个数,本表抄表结束
			//END_Fn:
					UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
					if(UARTCtrl->RMError==0)//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					{
//						UARTCtrl->KeyUserRMOKCount++;//当前重点户抄读成功计数
					}
					UARTCtrl->RMError=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
				}
				break;
			}
			p8RMLib=(u8 *)i;//抄表库
			if((p8RMLib[3]&0x04)==0)//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
			{//本Fn不是载波抄表
				goto NEXT_Fn;
			}
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
			Fn=p8RMLib[0];
			y=0;
			for(i=0;i<Get_KeyUserReadMeterLib_MaxFn();i++)
			{
				if(KeyUserReadMeterLib[i].AFN0CFn==Fn)//要抄读1类实时数据Fn,为0表示无效不抄
				{
					x=KeyUserReadMeterLib[i].AFN0DFn;//重点户曲线Fn
					y|=Class2Data_Fn(ClassNo,x);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
			}
			if(y==0x0)
			{//本Fn不抄
				goto NEXT_Fn;
			}
			i=GetClass1DataAddr(Fn,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			if(i==0)
			{//没找到
				goto NEXT_Fn;
			}
			i=MRR(i+1,4);//取原抄表时间年月日时
			x=MRR(ADDR_TYMDHMS+2,4);//取当前抄表时间年月日时
			if(i==x)
			{//已抄
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//抄表命令计数=0,清Fn数据缓冲
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//抄表命令计数>=Fn需发命令数;本Fn抄表完成
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0	
				goto NEXT_Fn;	
			}
			if(UARTCtrl->RMError!=0x0)
			{//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				goto NEXT_Fn;
			}
			
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//从节点地址(目的地址)
			if(Protocol==1)
			{//dl645-1997
				i=MRR((u32)&p8RMLib[10+(3*UARTCtrl->RMCount)],2);//取DI
				Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
				//预计应答字节数
				i=p8RMLib[10+(3*UARTCtrl->RMCount)+2];//返回长度
				if(i==0)
				{//返回长度=0
					i=10+(3*p8RMLib[1])+UARTCtrl->RMCount;
					i=p8RMLib[i];//取预计返回字节数
				}
				i+=14;
				Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			}
			else
			{//dl645-2007
				goto NEXT_KeyUser;//下一个重点户,dl645-2007抄整点冻结

//				i=MRR((u32)&p8RMLib[10+(5*UARTCtrl->RMCount)],4);//取DI
//				Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
//				//预计应答字节数
//				i=p8RMLib[10+(5*UARTCtrl->RMCount)+4];//返回长度
//				if(i==0)
//				{//返回长度=0
//					i=10+(5*p8RMLib[1])+UARTCtrl->RMCount;
//					i=p8RMLib[i];//取预计返回字节数
//				}
//				i+=16;
//				Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数

			}		
			UARTCtrl->TxByte=31+p8txbuff[28];//发送字节数(半字对齐)
			i=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;
			Terminal_Router->RouterKeyUserTask++;//重点户抄读任务
			//显示抄表地址
			CopyString((u8 *)"载波重点",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[16+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 2://接收
			if(UARTCtrl->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task!=1)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
				if(UARTCtrl->ReRMCount<1)//485抄表命令重发计数
				{
					UARTCtrl->ReRMCount++;//485抄表命令重发计数
				}
				else
				{
					UARTCtrl->RMError|=0x1;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				}
				Terminal_Router->RouterKeyUserTask--;//重点户抄读任务
				break;
			}
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//否认回答
		FnErr:
				UARTCtrl->Task=0;//空闲1次
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				Terminal_Router->RouterKeyUserTask--;//重点户抄读任务
				UARTCtrl->FnCount=0;//Fn计数
				UARTCtrl->RMCount=0;//485抄表命令计数
				UARTCtrl->ReRMCount=0;//485抄表命令重发计数
				break;
			}
			x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//重点户的电能表/交流采样装置序号
			if((x==0)||(x>AFN04F10MaxPn))
			{//装置序号错
				goto FnErr;
			}
			UARTCtrl->MeterNo=x;
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MRR(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//测量点号错
				goto FnErr;
			}
			Protocol=MRR(x+3,1);//通信协议类型 BIN 1 
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[UARTCtrl->FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[UARTCtrl->FnCount];
			}
			if(i==0)
			{//本Fn是空
				goto FnErr;
			}
			p8RMLib=(u8 *)i;//抄表库
			d64a=MRR(x+4,6);//取配置的通信地址
			y=p8rxbuff[28];//返回数据域长度
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				y=0;//接收数据长度
			}
			if(y==0)
			{//接收数据长度=0
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,y);

			if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
			{//起始符0x68错
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//返回的地址
			if(d64a!=d64b)
			{//表地址不相同
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			x=0;//和校验计算
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//和校验错
				UARTCtrl->Task=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//控制码从站回答异常
				goto NEXT_RMCount;//下抄表命令计数
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//数据域减0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//1997
				p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
				if(p8RMLib[0]!=0x0)
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+2))
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
						{//块命令后可能有0xaa
							if(p8rxbuff[9]==2)
							{//电科院测试软件当事件记录无数据时,只返回2字节数据标识,同从站异常回答
								goto NEXT_RMCount;//下抄表命令计数
							}
							UARTCtrl->Task=2;//2=接收超时(或错误)
							break;
						}
					}
				}
				p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
				ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
			}
			else
			{//2007
				p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
				if(p8RMLib[0]!=0x0)
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+4))
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
						{//块命令后可能有0xaa
							if(p8rxbuff[9]==4)
							{//电科院测试软件当事件记录无数据时,只返回4字节数据标识,同从站异常回答
								goto NEXT_RMCount;//下抄表命令计数
							}
							UARTCtrl->Task=2;//2=接收超时(或错误)
							break;
						}
					}
				}
				p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
				ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
			}
	NEXT_RMCount://下抄表命令计数
			UARTCtrl->RMCount+=1;//抄表命令计数+1
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->Task=0;//空闲1次
			Terminal_Router->RouterKeyUserTask--;//重点户抄读任务
			break;
	}
}
*/



#endif

//0x68
//总长度L
//总长度L
//控制码
//信息域R-1
//信息域R-2
//信息域R-3
//信息域R-4
//信息域R-5
//信息域R-6
//地址域A1,A2,A3
//应用功能码AFN
//数据单元标识DT1
//数据单元标识DT2
//数据单元
//CS
//0x16


