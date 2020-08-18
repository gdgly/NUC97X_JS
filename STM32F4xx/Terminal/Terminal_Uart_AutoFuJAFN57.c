
//主动上报重要事件
#include "Project.h"
#include "Terminal_Uart_AutoFuJAFN57.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_Uart_AutoFuJAFN57.h"
#include "Terminal_AFN56_SourceData.h"
#include "Terminal_Uart_AFN54_Paramter.h"
#include "FuJCollectTask.h"


#if (USER/100)==14//福建系
#if ComTest==1 //福建232口与485口输出数据调试程序开关：0==关闭;1==开启
void LogOut_RS232(u32 PORTn,u32 ADDR_LOGDATA,u32 LEN_Data,u32 Ascii);
#endif
void Terminal_Uart_AutoFuJAFN57(void)//主动上报重要事件
{//双通道上报存在问题，暂时不进行处理
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UARTCtrl->LinkTask>=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		Terminal_AutoFuJAFN57(GPRSPORT);//主动上报重要事件,入口UART端口号
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask>=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		Terminal_AutoFuJAFN57(ENETPORT);//主动上报重要事件,入口UART端口号
	}
#if RS232AutoRePort==1//RS232口主动上报0=禁止,1=允许
	Terminal_AutoFuJAFN57(RS232PORT);//主动上报重要事件,入口UART端口号
#endif
}


const u8 AutoFuJTaskData_57Example[]=
{
	0x68,
	16,0,
	16,0,
	0x68,
	0xc0,//控制码(DIR=1上行,启动标志位PRM=1,功能码暂4=发送∕无回答,若需要主站确认的通信服务（CON=1)10=请求∕响应)
	0,0,//行政区划码
	0,0,//终端地址
	0,//启动站地址(终端启动发送帧的MSA应为零)
	0x57,//AFN=0E
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON暂=0
	0,0,//Pn=0
	0x01,0,//Fn=1任务数据请求上报
	0,//本次上报的具体任务条数
	//...
	//cs
	//0x16
};
const u8 AutoFuJEventData_57Example[]=
{
	0x68,
	16,0,
	16,0,
	0x68,
	0xc0,//控制码(DIR=1上行,启动标志位PRM=1,功能码暂4=发送∕无回答,若需要主站确认的通信服务（CON=1)10=请求∕响应)
	0,0,//行政区划码
	0,0,//终端地址
	0,//启动站地址(终端启动发送帧的MSA应为零)
	0x57,//AFN=0E
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON暂=0
	0,0,//Pn=0
	0x02,0,//Fn=2事件数据上报
	0,//上报序号
	//...
	//cs
	//0x16
};

//福建四类数据上报状态确认
//u32 Flag: 0-确认 1-超时
void FuJTaskReportACK(u32 Flag)
{
	u32 i;
//	u32 ID;
	u32 ReportFlag;
	ReportFlag = MRR(ADDR_AFN54F10+2*(FuJTaskReportCtrl->Task-1),1);
	if(ReportFlag == 0xFF)
		ReportFlag = 0;
	if(((ReportFlag>>4)&0x07) >= 3)//被动召测
	{
		if(Flag != 0)
		{//主站未确认,等待下轮召测
			Flag = 0xFF;
		}
		else
		{
			Flag = 0;
		}
	}
	else
	{//主动上报的数据
		if(Flag != 0)//主站未回复，不清除上报状态
			return;
//		Flag = 0;//无论主站是否正常回复，均清上报标志位
	}
	for(i = 0;i < FuJTaskReportCtrl->Num;i ++)
	{
//		ID = GetIDBySubTask(FuJTaskReportCtrl->Task,u32 SubTask);
//		if(IsFuJSubTaskRight(FuJTaskReportCtrl->Task,FuJTaskReportCtrl->SubTaskList[i]))
		{
			ClearFuJDataReportFlag(FuJTaskReportCtrl->Task,FuJTaskReportCtrl->SubTaskIDList[i],FuJTaskReportCtrl->CurRound,Flag);
			FuJTaskReportCtrl->SubTaskID = FuJTaskReportCtrl->SubTaskIDList[i];
		}
	}
}
u32 GetReportFuJTask(void)
{
	u32 i;
	u32 Task;
	u32 Prio;
	Task = 0;
	Prio = 255;
	for(i = 0;i < MAX_FUJ_TASK;i ++)
	{
		if(FuJClloectTask[i].State != FUJTASK_DISABLE_FLAG)
		{//非无效任务
			if((0 < FuJClloectTask[i].Prio) && (FuJClloectTask[i].Prio < Prio))
			{
				if(FuJClloectTask[i].ReportFlag == 1)
				{//允许上报
					Task = i+1;
					Prio = FuJClloectTask[i].Prio;
				}
				else if(FuJClloectTask[i].ReportFlag == 2)
				{//等待批量召测
					if(FuJTaskReportCtrl->AutoPackFlag == 2)
					{//批量召测已启动
						Task = i+1;
						Prio = FuJClloectTask[i].Prio;
					}
				}
			}
		}
	}
	if(Task == 0)
	{
		FuJTaskReportCtrl->AutoPackFlag = 0;//无召测任务
	}
	return Task;
}
void ForbidReportFlag(void)
{
	u32 y;
	FuJTaskReportCtrl->AutoPackFlag = 0;
	for(y = 0;y < MAX_FUJ_TASK;y ++)
	{
		if(FuJClloectTask[y].State != FUJTASK_DISABLE_FLAG)
		{//非无效任务
			if(FuJClloectTask[y].ReportFlag == 2)
			{
				FuJClloectTask[y].ReportFlag = 3;
			}
			else
				FuJClloectTask[y].ReportFlag = 0;
		}
	}
}
void Terminal_AutoFuJAFN57(u32 PORTn)//主动上报重要事件,入口UART端口号
{
	u32 i;
	u32 x;
	u32 y;
	u32 Len;
	u32 m;
	u32 n;
	u32 k;
//	u8 * p8;
	u16 * p16;
	FuJSubTaskDataHead_TypeDef *DataHead;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u32 LEN_UARTnTx;
	
	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
//	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	LEN_UARTnTx = 800;//福建上报以800字节为上限
	if(UARTCtrl->Lock==0x55)
	{
		return;
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
				
				if(UARTCtrl->HostACK!=0x0)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
				{	
					if((UARTCtrl->HostACK!=7) && (UARTCtrl->HostACK!=8))//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
					{//有其他主动上报等待确认返回
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//等待主站确认秒定时器!=0
					}
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
					i=MRR(ADDR_AFN04F1+3,1);
					i>>=4;
					i&=0x3;
					if(UARTCtrl->HostCount>i)
					{//重发次数到
						UARTCtrl->HostCount=0;
						//对标志位的处理
						if(UARTCtrl->HostACK==7)
							FuJTaskReportACK(1);//超时
						if(UARTCtrl->HostACK==8)
							ClearFuJEventLastRecord();
					}
				}
				i = MRR(ADDR_AFN54F22,1);
				if((i&0x03) == 2)
//				if(MRR(ADDR_AFN05F29,1)!=0x55)
				{//不允许终端主动上报四类数据
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
					UARTCtrl->HostCount=0;
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
				//事件上报
				x = FuJEventLastRecordRead(ADDR_DATABUFF+1024);
				if(x != 0)
				{//有数据等待上报
					//重复上报判断
					n = MRR(ADDR_DATABUFF+1024,4);//序号
					if(FuJTaskReportCtrl->EventIndex != n)
					{
						UARTCtrl->HostCount = 0;//序号不一致，首次上报
						FuJTaskReportCtrl->EventIndex = n;
					}
					//组帧
					MR((u32)p8txbuff,(u32)AutoFuJEventData_57Example,19);//
					MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
					if(UARTCtrl->HostCount==0)
					{//不是重复帧
						p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
						UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变时用)
					}
					else
					{//是重复帧
						p8txbuff[13]|=UARTCtrl->ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
					}
					Len=13;
					p8txbuff[18] = n;//上报事件序号
					MR(((u32)p8txbuff)+19,ADDR_DATABUFF+1024+4,x-1);
					UARTCtrl->HostACK=8;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报,8-福建事件上报
					goto ReportFrame;
				}
				//上传
				if(FuJTaskReportCtrl->CurRound > FuJTaskReportCtrl->MaxRound)
				{//寻找下一个任务方案
					if((FuJTaskReportCtrl->Task > 0) && (FuJTaskReportCtrl->Task <= MAX_FUJ_TASK))
					{//该任务置为已上报完成
						y = MRR(ADDR_AFN54F10+2*(FuJTaskReportCtrl->Task-1),1);//上报标志
						if(y == 0xFF)
							y = 0;
						if(((y>>4)&0x07) >= 3)
							FuJClloectTask[FuJTaskReportCtrl->Task-1].ReportFlag = 3;//上报完成
						else
							FuJClloectTask[FuJTaskReportCtrl->Task-1].ReportFlag = 0;//上报完成
					}
					y = GetReportFuJTask();//获取可上报的任务方案
					if((y == 0) || (y > MAX_FUJ_TASK))
					{//无任务等待上报
						FuJTaskReportCtrl->Task = 0;
						FuJTaskReportCtrl->CurRound = 0xFFFFFFFF;
						return;
					}
					m = MRR(ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+6,1);//Unit
					n = MRR(ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+5,1);//Num
//					MR(ADDR_DATABUFF+12,ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+11,6);//BaseTime
					if((m == 0) || (n == 0))
					{//只执行一次的任务
						x = 0;
						m = 0;
						MR(ADDR_DATABUFF,ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+11,6);//任务开始时间
					}
					else
					{
						x = StructuredTimeToPriod(ADDR_TYMDHMS,ADDR_DATABUFF,m,n);//轮次--规整时间
						if(x == ERROR_RET)
						{
							//结束当前上报
							FuJTaskReportCtrl->MaxRound = 0;
							FuJTaskReportCtrl->CurRound = 0xFFFFFFFF;
							return;
						}
						m = MRR(ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+10,1);//Depth
					}
					FuJTaskReportCtrl->Task = y;
					FuJTaskReportCtrl->MaxRound = x;//最大轮次
					if(x < m)
						m = x;
					if(FuJTaskReportCtrl->AutoPackFlag == 2)
						FuJTaskReportCtrl->CurRound = x-m;
					else
						FuJTaskReportCtrl->CurRound = x;
					FuJTaskReportCtrl->SubTaskID = 0;//从0开始查找具体任务
				}
				if((FuJTaskReportCtrl->Task < 1) && (FuJTaskReportCtrl->Task > MAX_FUJ_TASK))
					return;
				k = MRR(ADDR_AFN54F10+2*(FuJTaskReportCtrl->Task-1),1);//上报标志
				if(k == 0xFF)
					k = 0;
				if((k&0x80) == 0)
				{//不允许上报
					//结束当前任务号的所有上报
					FuJTaskReportCtrl->CurRound = 0xFFFFFFFF;
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
					UARTCtrl->HostCount=0;//重发次数
					return;
				}
				MR((u32)p8txbuff,(u32)AutoFuJTaskData_57Example,19);//
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
				if(UARTCtrl->HostCount==0)
				{//不是重复帧
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变时用)
				}
				else
				{//是重复帧
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
				}
//				p8txbuff[20]=i;//本帧报文传送的事件记录起始指针Pm
//				p8txbuff[21]=i;//本帧报文传送的事件记录结束指针Pn
//				p8[3]=0;//当前有?个重要事件主动上传需确认
				Len=13;
				//组帧
//				p8=(u8 *)(ADDR_DATABUFF);
				DataHead = (FuJSubTaskDataHead_TypeDef *)(ADDR_DATABUFF);
				n = 0;//已组帧的具体任务数
				m = FuJTaskReportCtrl->SubTaskID;//具体任务存储序号
				for(x = 0;x < MAX_PER_SUB_TASK;x ++)
				{
					if(UARTCtrl->HostCount == 0)
					{//首次上报
						if(n >= 15)
							break;
						m = GetNextSubTaskID(FuJTaskReportCtrl->Task,m);//具体任务存储序号
						FuJTaskReportCtrl->SubTaskIDList[n] = m;
					}
					else
					{//重发
						if(n >= FuJTaskReportCtrl->Num)
							break;
						if(m == FuJTaskReportCtrl->SubTaskIDList[n])
						{
							n ++;//有具体任务已读取不到数据
							continue;
						}
						m = FuJTaskReportCtrl->SubTaskIDList[n];//具体任务号
					}
					if((m == 0) || (m > MAX_FUJ_SUB_TASK))
					{//异常m
						break;
					}
					//读取数据
					if(FuJTaskReportCtrl->AutoPackFlag == 2)//批量上报时，不判断任何数据，只要FLASH保留的可上报的数据均上报给主站
						y = ReadFuJTaskData(ADDR_DATABUFF,FuJTaskReportCtrl->Task,m,FuJTaskReportCtrl->CurRound,ADDR_TYMDHMS,0x23);//不判时标，只判轮次
					else
						y = ReadFuJTaskData(ADDR_DATABUFF,FuJTaskReportCtrl->Task,m,FuJTaskReportCtrl->CurRound,ADDR_TYMDHMS,0x22);//不判时标，只判轮次
					if((FuJTaskReportCtrl->Task == 0x03FD)&&(m==0x1D))
					{
						m = 0x1D;
					}
					if((y == 0) || (y > 288))//读取失败
						continue;
					if(DataHead->ReportFlag == 0)//无需上报
						continue;
					if(FuJTaskReportCtrl->AutoPackFlag != 2)
					{//非正在批量召测
						if(k&0x10)//只上报抄读成功的
						{
							if(DataHead->Len <= 6)
								continue;
						}
					}//else 批量召测时，不论成功失败，均要上报
					if((Len+6+2+(25+DataHead->Len))>=LEN_UARTnTx)
					{
						break;
					}
					n ++;
					//填充数据
					MWR(FuJTaskReportCtrl->Task,(u32)(p8txbuff+Len+6),2);//任务方案号
					Len += 2;
					MR((u32)(p8txbuff+Len+6),ADDR_AFN54F8+(m-1)*LEN_AFN54F8_Pn+2,2);//具体任务序号
					Len += 2;
					MR((u32)(p8txbuff+Len+6),(u32)DataHead->MeterAddr,6);//任务对象通信地址
					Len += 6;
					if(DataHead->Len == 0)
					{
						MC(0,(u32)(p8txbuff+Len+6),1);//任务结果
					}
					else
					{
						MC(1,(u32)(p8txbuff+Len+6),1);//任务结果
					}
					Len ++;
					MR((u32)(p8txbuff+Len+6),(u32)DataHead->STime,6);//归整时间
					Len += 6;
					MR((u32)(p8txbuff+Len+6),(u32)DataHead->RTime,6);//实际时间
					Len += 6;
					MC(DataHead->ReadTimes,(u32)(p8txbuff+Len+6),1);//重试次数
					Len ++;
					if(DataHead->Len == 0)
					{
						MC(1,(u32)(p8txbuff+Len+6),1);//报文长度
						Len ++;
						MC(11,(u32)(p8txbuff+Len+6),1);//报文-ERR响应超时
						Len ++;
					}
					else
					{
						MC(DataHead->Len,(u32)(p8txbuff+Len+6),1);//报文长度
						Len ++;
						MR((u32)(p8txbuff+Len+6),ADDR_DATABUFF+LEN_FuJSubTaskDataHead,DataHead->Len);//报文
						Len += DataHead->Len;
					}
				}
				if(n == 0)
				{//当前轮次结束，开启下一轮次的数据上报
					FuJTaskReportCtrl->CurRound++;
					FuJTaskReportCtrl->SubTaskID = 0;//从0开始查找具体任务
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
					UARTCtrl->HostCount=0;//重发次数
					return;
				}
				FuJTaskReportCtrl->Num = n;
				p8txbuff[18] = n;//上报总数量
				UARTCtrl->HostACK=7;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报,8-福建事件上报
				
			
ReportFrame:
				p8txbuff[1]=Len;
				p8txbuff[2]=Len>>8;
//				if((MRR(ADDR_AFN04F1+4,1)&0x04)!=0x0)
				{//需要主站确认的通信服务（CON=1）的标志
					p8txbuff[6]|=10;//功能码10=请求∕响应帧(请求1级数据)
					p8txbuff[13]|=0x10;//CON=1
					UARTCtrl->HostCount+=1;//重发次数
				}
//				else
//				{
//					p8txbuff[6]|=4;//功能码4=发送∕无回答
//					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
//					p8[3]=0;//当前有?个重要事件主动上传需确认=0
//				}
				if(UARTCtrl->HostCount==1)
				{//不是重发
					UARTCtrl->TxSEQ+=1;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				}
				UARTCtrl->Task=2;//2=等待单帧数据发送
				UARTCtrl->TxByte=(Len+8);//需发送字节数
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			#if (USER/100)==14
				#if ComTest==1 //福建232口与485口输出数据调试程序开关：0==关闭;1==开启				
				LogOut_RS232(RS485_2PORT,(u32)"[3761-GS]",9,1);
				LogOut_RS232(RS485_2PORT,(u32)p8txbuff,UARTCtrl->TxByte,0);
				#endif
			#endif
				//超时定时器
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
				p16ACKTimer[0]=i;//主动上报等待主站确认秒
//				p16ACKTimer[0]=15;//主动上报等待主站确认秒-默认为15s
			}//if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机   /*串口处于登陆状态*/
			else
				ForbidReportFlag();//禁止主动上报
		}//if(p16[0]==0x0)--串口定时器为0
	}//if(UARTCtrl->Task==0x0)--串口空闲
}

#endif
