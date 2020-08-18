
//定时任务
#include "Project.h"
#include "Terminal_Uart_TimerTask.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



void Terminal_Uart_AutoTask(void)//主动上报定时任务
{
	Terminal_TimerTask(GPRSPORT);//主动上报定时任务,入口UART端口号
	Terminal_TimerTask(ENETPORT);//主动上报定时任务,入口UART端口号
#if RS232AutoRePort==1//RS232口主动上报0=禁止,1=允许
	Terminal_TimerTask(RS232PORT);//主动上报定时任务,入口UART端口号
#endif
}


const u8 ClassData_Example[]=
{
	0x68,
	0,0,
	0,0,
	0x68,
	0x00,//控制码(DIR=0上行,启动标志位PRM=0,功能码暂4=发送∕无回答,若需要主站确认的通信服务（CON=1)10=请求∕响应)
	0,0,//行政区划码
	0,0,//终端地址
	0,//启动站地址(终端启动发送帧的MSA应为零)
	0x00,//AFN
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON暂=0
	//DI
	//cs
	//0x16
};




void Terminal_TimerTask(u32 PORTn)//主动上报定时任务,入口UART端口号
{
	u32 i;
	u32 x;
	u32 y;
	u32 Fn;
	u32 TaskCount;
	u32 t;
	u32 ReTxFlag;
	u64 YMDHM;
	u64 YMDHMS;
	
	u8 * p8;
	u16 * p16;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;


	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	//主动上报定时任务在完成冻结后继续
	if(Terminal_Ram->CongealFlags==0x1f)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	{
		if(DataComp(((u32)Terminal_Ram->LastCongealTime),ADDR_TYMDHMS+1,5))//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		{//年月日时分更新
			return;//定时任务和冻结异步进行,故在此又进行判断
		}
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
		{
			goto LoopChack;//级联巡测主动上报
		}
		if(UARTCtrl->Task==0x0)
		{//接收
			p16=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			if(p16[0]==0x0)
			{
				if(PORTn==RS232PORT)
				{
					goto LoopChack;
				}
				if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{//登入
LoopChack:
					if(PORTn==GPRSPORT)
					{
						p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
					}
					if(PORTn==ENETPORT)
					{
						p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
					}
					if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
					{
						p16ACKTimer=(u16 *)((u32)&Terminal_Ram->RS232_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
					}
					if(UARTCtrl->HostACK!=0x0)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					{	
						if(UARTCtrl->HostACK!=5)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
						{//有其他主动上报等待确认返回
							return;
						}
						if(p16ACKTimer[0]!=0x0)
						{
							return;//等待主站确认秒定时器!=0
						}
						UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
						i=MRR(ADDR_AFN04F1+3,1);
						i>>=4;
						i&=0x3;
						if(UARTCtrl->HostCount>i)
						{//重发次数到
							UARTCtrl->HostCount=0;//主动上报重发计数
						}
						else
						{//重发上计数任务
							if(UARTCtrl->TimerTaskCount!=0x0)
							{
								UARTCtrl->TimerTaskCount--;//定时任务分步计数
							}
							else
							{
								UARTCtrl->TimerTaskCount=(2*MaxTimerTask)-1;
							}
						}
					}
					if(MRR(ADDR_AFN05F29,1)!=0x55)
					{//不允许终端主动上报
						UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
						UARTCtrl->HostCount=0;//主动上报重发计数
						return;
					}
					//心跳帧优先
					if(PORTn==GPRSPORT)
					{
						if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//5 GPRS心跳秒定时器
						{
							return;
						}
					}
					if(PORTn==ENETPORT)
					{
						if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//2 以太网心跳秒定时器
						{
							return;
						}
					}
					//上传
					TaskCount=UARTCtrl->TimerTaskCount;
					if(TaskCount>=(2*MaxTimerTask))
					{
						TaskCount=0;
					}
					if(MRR(ADDR_AFN04F67+TaskCount,1)==0x55)
					{//启动
						if(MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+8,1)!=0x0)
						{//数据单元标识个数!=0
							//取基准时间
							MR(ADDR_DATABUFF,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+1,6);
							//取上次上报记录时间
							if(PORTn==GPRSPORT)
							{
								MR(ADDR_DATABUFF+6,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
							}
					#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
							if(PORTn==ENETPORT)
							{
								MR(ADDR_DATABUFF+6,ADDR_ENETTimerTaskNotes+(6*TaskCount),6);
							}
					#else
							if(PORTn==ENETPORT)
							{
								MR(ADDR_DATABUFF+6,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
							}
					#endif		
							if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
							{
								MR(ADDR_DATABUFF+6,ADDR_RS232TimerTaskNote+(6*TaskCount),6);
							}
							
							p8=(u8 *)(ADDR_DATABUFF);
							if((p8[4]&0xe0)!=0x0)
							{//376.1测试软件基准时间月字节包含有星期
								p8[4]&=0x1f;
								MW(ADDR_DATABUFF,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+1,6);
							}
							if(DataComp(ADDR_TYMDHMS,ADDR_DATABUFF,6)<2)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							{//当前实时钟>=发送基准时间
								i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount),1);//定时发送1类数据任务设置(任务号)
								t=i&0x3f;//定时上报数据的时间周期
								if(t!=0x0)
								{//定时上报数据的时间周期!=0
									ReTxFlag=UARTCtrl->HostCount;
									if(ReTxFlag==0x0)
									{//不是重发
										i>>=6;
										if(i!=3)
										{//定时上报周期单位=分、时、日
											switch(i)//定时上报周期
											{
												case 0://分
													break;
												case 1://时
													t*=60;//定时上报数据的时间周期(统一为分)
													break;
												case 2://日
													t*=(60*24);//定时上报数据的时间周期(统一为分)
													break;
											}
											i=TimeDifference(ADDR_DATABUFF,ADDR_TYMDHMS);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
											if((i&0x80000000)!=0x0)
											{//负值
												i=~i;
												i++;
											}
											i/=60;//从基准时间到当前实时钟的相隔时间分
											i/=t;//整数的周期数
											t*=i;//本次定时上报时间=基准时间+相隔分
											YMDHMS=MRR(ADDR_DATABUFF+1,5);
											YMDHMS=YMDHM_AddM(YMDHMS,t);//年月日时分加m分,返回加m分后的年月日时分
											MWR(YMDHMS,ADDR_DATABUFF+1,5);
											if(DataComp(ADDR_DATABUFF,ADDR_DATABUFF+6,6)==0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
											{//相同时间已上报
												goto No_Task;
											}
										}
										else
										{//定时上报周期单位=月
											if(DataComp(ADDR_TYMDHMS+2,ADDR_DATABUFF+2,2)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
											{//当前实时钟时日!=发送基准时间时日
												goto No_Task;
											}
											//当前实时钟的年月对0年1月的月数
											p8=(u8 *)(ADDR_TYMDHMS);
											i=bcd_hex(p8[5]);
											i*=12;
											i+=bcd_hex(p8[4]);
											//基准时间的年月对0年1月的月数
											p8=(u8 *)(ADDR_DATABUFF);
											x=bcd_hex(p8[5]);
											x*=12;
											x+=bcd_hex(p8[4]);
											if(i<x)
											{
												goto No_Task;
											}
											i-=x;
											i/=t;
											t*=i;//本次定时上报时间=基准时间+相隔月
											p8[4]=bcd_hex(p8[4]);
											p8[5]=bcd_hex(p8[5]);
											for(;t!=0x0;t--)
											{
												p8[4]++;
												if(p8[4]>=13)
												{
													p8[4]=1;
													p8[5]++;
													if(p8[5]>=100)
													{
														p8[5]=0;
													}
												}
											}
											p8[4]=hex_bcd(p8[4]);
											p8[5]=hex_bcd(p8[5]);
											if(DataComp(ADDR_DATABUFF,ADDR_DATABUFF+6,6)==0X0)
											{//相同时间已上报
												goto No_Task;
											}
										}
									}
									//取当前上报时标用于2类数据
									if(ReTxFlag==0x0)
									{//不是重发
										YMDHMS=MRR(ADDR_DATABUFF,6);//当前上报时间寄存
									}
									else
									{//是重发
										if(PORTn==GPRSPORT)
										{
											YMDHMS=MRR(ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);//上次定时上报时间
										}
								#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
										if(PORTn==ENETPORT)
										{
											YMDHMS=MRR(ADDR_ENETTimerTaskNotes+(6*TaskCount),6);//上次定时上报时间
										}
								#else
										if(PORTn==ENETPORT)
										{
											YMDHMS=MRR(ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);//上次定时上报时间
										}
								#endif
										if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
										{
											YMDHMS=MRR(ADDR_RS232TimerTaskNote+(6*TaskCount),6);//上次定时上报时间
										}
									}
									MR((u32)p8rxbuff,(u32)ClassData_Example,14);
									MR(((u32)p8rxbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
									p8rxbuff[13]|=(UARTCtrl->TxSEQ&0xf);//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									if(TaskCount<MaxTimerTask)
									{//类1数据
										i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+8,1);
										i*=4;//DI字节数
										MR(((u32)p8rxbuff)+14,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9,i);//取DI
										i+=8;
										p8rxbuff[1]=i;//修改作为接收帧长度L
										p8rxbuff[2]=i>>8;
										MW((u32)p8rxbuff,(u32)p8txbuff,30);//COPY 14+16字节数据头到发送区,作为发送头

										UARTCtrl->TxFrameCount=0;//寄存多帧数据时已发送帧计数
										UARTCtrl->MulFrameGPCR=0;//多帧时通用计数寄存器
										UARTCtrl->LockFlags|=(1<<4);//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
										p8rxbuff[12]=0x0c;//应用层功能码AFN
										
										UARTCtrl->HostCount+=1;//重发次数
										Uart_3761_Data(PORTn);//UART_376数据
										if(p8txbuff[12]!=0x0C)
										{//没上报数据
											UARTCtrl->Task=0;
											UARTCtrl->RxByte=0;//0=接收
											UARTCtrl->TxByte=0;//需发送字节数=0
											goto No_Task;
										}
									}
									else
									{//类2数据
										x=0;
										for(i=0;i<MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+8,1);i++)
										{
											Fn=BitLoca(MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9+(4*i)+2,1));
											Fn+=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9+(4*i)+3,1)*8;
											if((Fn>=Get_Class2Data_MaxFn())||(Fn==0x0))
											{//Fn无效时不装入,因时标长度不可知
												continue;
											}
											MR(((u32)p8rxbuff)+14+x,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9+(4*i),4);//取DI
											x+=4;
											switch(Terminal_Class2DataLib[Fn].CongealType)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
											{
												case 0://日冻结(上报上1日)
													t=YMD_Sub1D(YMDHMS>>24);//年月日减1日,返回减1后的年月日
													MWR(t,((u32)p8rxbuff)+14+x,3);
													x+=3;
													break;
												case 1://抄表日冻结
													t=YMDHMS>>24;
													MWR(t,((u32)p8rxbuff)+14+x,3);
													x+=3;
													break;
												case 2://月冻结(上报上1月)
													t=YM_Sub1M(YMDHMS>>32);//年月减1月,返回减1后的年月
													MWR(t,((u32)p8rxbuff)+14+x,2);
													x+=2;
													break;
												case 3://曲线
//-----------------------------------------------------------------------
/*
													//固定上报上1日整日点数
													t=YMD_Sub1D(YMDHMS>>24);//年月日减1日,返回减1后的年月日
													MC(0,((u32)p8rxbuff)+14+x,2);//时分==00:00
													MWR(t,((u32)p8rxbuff)+14+x+2,3);
													//曲线冻结间隔时间*抽取数据倍率
													t=CurveCongealTime*MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+7,1);
													switch(t)
													{
														case 15:
															Fn=1;//要读取曲线的数据密度m
															break;
														case 30:
															Fn=2;//要读取曲线的数据密度m
															break;
														case 60:
															Fn=3;//要读取曲线的数据密度m
															break;
														case 5:
															Fn=254;//要读取曲线的数据密度m
															break;
														case 1:
															Fn=255;//要读取曲线的数据密度m
															break;
														default:
															t=CurveCongealTime;
															switch(t)
															{
																case 15:
																	Fn=1;//要读取曲线的数据密度m
																	break;
																case 30:
																	Fn=2;//要读取曲线的数据密度m
																	break;
																case 60:
																	Fn=3;//要读取曲线的数据密度m
																	break;
																case 5:
																	Fn=254;//要读取曲线的数据密度m
																	break;
																case 1:
																	Fn=255;//要读取曲线的数据密度m
																	break;
															}
															break;
													}
													MC(hex_bcd(t),((u32)p8rxbuff)+14+x,1);//起始时间分为第1个数据点的分
													MC(Fn,((u32)p8rxbuff)+14+x+5,1);//数据冻结密度m
													t=(24*60)/t;//整1天点数
													MC(t,((u32)p8rxbuff)+14+x+6,1);//读数据点数
													x+=7;
*/
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
													//按定时发送周期内点数上报
													y=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount),1);//定时发送1类数据任务设置(任务号)
													t=y&0x3f;//定时上报数据的时间周期
													y>>=6;
													switch(y)//定时上报周期
													{
														case 0://分
															break;
														case 1://时
															t*=60;//定时上报数据的时间周期(统一为分)
															break;
														case 2://日
															t*=(60*24);//定时上报数据的时间周期(统一为分)
															break;
														case 3://月
															t=YMDHMS>>32;
															t=YM_Sub1M(t);//年月减1月,返回减1后的年月
															t=YM_D(t);//取指定年月的天数
															t*=(60*24);//定时上报数据的时间周期(统一为分)
															break;
													}
													YMDHM=YMDHM_SubM(YMDHMS>>8,t);//年月日时分减m分,返回减m分后的年月日时分
										#if (USER/100)==2//用户标识:山东版
													if((t%60)==0)
													{//定时上报周期=60分的整数倍
														//上报上整点冻结的曲线数据,当前整点冻结数据还没读
														YMDHM>>=8;
														YMDHM<<=8;
													}
										#endif										
													//曲线冻结间隔时间*抽取数据倍率
													y=CurveCongealTime*MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+7,1);
													switch(y)
													{
														case 15:
															Fn=1;//要读取曲线的数据密度m
															break;
														case 30:
															Fn=2;//要读取曲线的数据密度m
															break;
														case 60:
															Fn=3;//要读取曲线的数据密度m
															break;
														case 5:
															Fn=254;//要读取曲线的数据密度m
															break;
														case 1:
															Fn=255;//要读取曲线的数据密度m
															break;
														default:
															y=CurveCongealTime;
															switch(y)
															{
																case 15:
																	Fn=1;//要读取曲线的数据密度m
																	break;
																case 30:
																	Fn=2;//要读取曲线的数据密度m
																	break;
																case 60:
																	Fn=3;//要读取曲线的数据密度m
																	break;
																case 5:
																	Fn=254;//要读取曲线的数据密度m
																	break;
																case 1:
																	Fn=255;//要读取曲线的数据密度m
																	break;
															}
															break;
													}
													if((t%y)==0)
													{
														t/=y;//需上传点数
													}
													else
													{
														t/=y;//需上传点数
														t++;
													}
													if(t>96)
													{//最多只传最近96点
														YMDHM=YMDHM_AddM(YMDHM,(t-96)*y);
														t=96;
													}
													if(t==0)
													{
														t=1;//至少1个点
													}
													MWR(YMDHM,((u32)p8rxbuff)+14+x,5);
													MWR(Fn,((u32)p8rxbuff)+14+x+5,1);//数据冻结密度m
													MWR(t,((u32)p8rxbuff)+14+x+6,1);//读数据点数
													x+=7;
//-----------------------------------------------------------------------
													break;
											}
										}
										x+=8;
										p8rxbuff[1]=x;//修改作为接收帧长度L
										p8rxbuff[2]=x>>8;
										MW((u32)p8rxbuff,(u32)p8txbuff,30);//COPY 14+16字节数据头到发送区,作为发送头

										UARTCtrl->TxFrameCount=0;//寄存多帧数据时已发送帧计数
										UARTCtrl->MulFrameGPCR=0;//多帧时通用计数寄存器
										UARTCtrl->LockFlags|=(1<<5);//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
										p8rxbuff[12]=0x0d;//应用层功能码AFN
										
										UARTCtrl->HostCount+=1;//重发次数
										Uart_3761_Data(PORTn);//UART_376数据
										if(p8txbuff[12]!=0x0D)
										{//没上报数据
											UARTCtrl->Task=0;
											UARTCtrl->RxByte=0;//0=接收
											UARTCtrl->TxByte=0;//需发送字节数=0
											goto No_Task;
										}
									}
									//修改上次定时上报时间
									if(ReTxFlag==0x0)
									{//不是重发
										if(PORTn==GPRSPORT)
										{
											MWR(YMDHMS,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
										}
								#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
										if(PORTn==ENETPORT)
										{
											MWR(YMDHMS,ADDR_ENETTimerTaskNotes+(6*TaskCount),6);
										}
								#else
										if(PORTn==ENETPORT)
										{
											MWR(YMDHMS,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
										}
								#endif
										if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
										{
											MWR(YMDHMS,ADDR_RS232TimerTaskNote+(6*TaskCount),6);
										}
									}
									//UARTCtrl->TxSEQ+=1;//启动帧序列域SEQ+1;(已在类1和2的UART数据填入中加1)
									i=MRR(ADDR_AFN04F1+2,2);//终端等待从动站响应的超时时间
									i&=0xfff;
									if(i<WaitHostACKMinTimer)
									{
										i=WaitHostACKMinTimer;//主动上报等待主站确认最小定时秒
									}
									if(i>WaitHostACKMaxTimer)
									{
										i=WaitHostACKMaxTimer;//主动上报等待主站确认最大定时秒
									}
									p16ACKTimer[0]=i;//主动上报等待主站确认秒定时
									TaskCount++;//任务计数+1
									UARTCtrl->TimerTaskCount=TaskCount;
									return;
								}
							}
						}
					}
	No_Task:
					UARTCtrl->LockFlags&=~(3<<4);//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7=保留
					UARTCtrl->HostACK=0x0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					UARTCtrl->HostCount=0;//主动上报重发计数
					TaskCount++;//任务计数+1
					UARTCtrl->TimerTaskCount=TaskCount;
				}
			}
		}
	}
}

void Uart_376_CON(u32 PORTn)//主动定时任务CON标志位和功能码
{
	u32 i;
	u8 * p8txbuff;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	if(UARTCtrl->LockFlags&0x30)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
	{//主动定时任务
		p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
		i=MRR(ADDR_AFN04F1+4,1);
		i&=0x03;		
		if(i)
		{//需要主站确认的通信服务（CON=1）的标志
			p8txbuff[6]&=0xf0;
			p8txbuff[6]|=0xCA;//控制码DIR=1上行,启动标志位PRM=0,要求访问位ACD暂=0;功能码10=请求∕响应帧(请求1级数据)
			p8txbuff[13]|=0x10;//CON=1
			UARTCtrl->HostACK=5;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
			if(PORTn==GPRSPORT)
			{
				p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
			}
			if(PORTn==ENETPORT)
			{
				p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
			}
			if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
			{
				p16ACKTimer=(u16 *)((u32)&Terminal_Ram->RS232_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
			}
			i=MRR(ADDR_AFN04F1+2,2);//终端等待从动站响应的超时时间
			i&=0xfff;
			if(i<WaitHostACKMinTimer)
			{
				i=WaitHostACKMinTimer;//主动上报等待主站确认最小定时秒
			}
			if(i>WaitHostACKMaxTimer)
			{
				i=WaitHostACKMaxTimer;//主动上报等待主站确认最大定时秒
			}
			p16ACKTimer[0]=i;//主动上报等待主站确认秒定时
		}
		else
		{
			p8txbuff[6]&=0xf0;
			p8txbuff[6]|=0xC4;//控制码DIR=1上行,启动标志位PRM=0,要求访问位ACD暂=0;功能码4=发送∕无回答
			UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
		}
	}
}


