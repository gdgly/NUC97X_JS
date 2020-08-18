
//主动上报重要事件
#include "Project.h"
#include "Terminal_Uart_AutoEvent.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"





void Terminal_Uart_AutoEvent(void)//主动上报重要事件
{
	Terminal_AutoEvent(GPRSPORT);//主动上报重要事件,入口UART端口号
	Terminal_AutoEvent(ENETPORT);//主动上报重要事件,入口UART端口号
#if RS232AutoRePort==1//RS232口主动上报0=禁止,1=允许
	Terminal_AutoEvent(RS232PORT);//主动上报重要事件,入口UART端口号
#endif

#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	Terminal_AutoEvent_Meter(GPRSPORT);//主动上报电能表事件,入口UART端口号
	Terminal_AutoEvent_Meter(ENETPORT);//主动上报电能表事件,入口UART端口号
#if RS232AutoRePort==1//RS232口主动上报0=禁止,1=允许
	Terminal_AutoEvent_Meter(RS232PORT);//主动上报电能表事件,入口UART端口号
#endif
#endif
}


const u8 AutoEvent_Example[]=
{
	0x68,
	16,0,
	16,0,
	0x68,
	0xc0,//控制码(DIR=1上行,启动标志位PRM=1,功能码暂4=发送∕无回答,若需要主站确认的通信服务（CON=1)10=请求∕响应)
	0,0,//行政区划码
	0,0,//终端地址
	0,//启动站地址(终端启动发送帧的MSA应为零)
	0xe,//AFN=0E
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON暂=0
	0,0,//Pn=0
	0x01,0,//Fn=1重要事件
	0,//当前重要事件计数器EC1
	0,//当前一般事件计数器EC2
	0,//本帧报文传送的事件记录起始指针Pm
	0,//本帧报文传送的事件记录结束指针Pn
	//...
	//cs
	//0x16
};



void Terminal_AutoEvent(u32 PORTn)//主动上报重要事件,入口UART端口号
{
	u32 i;
	u32 x;
	u32 y;
	u32 Len;
	u32 T;
	u8 * p8;
	u16 * p16;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u32 LEN_UARTnTx;
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	u32 f;
	u32 z;
#endif
#if (USER/100)==5  //上海 不上报恢复类事件
	u32 s;
	u32 k;
#endif
	
	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
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
				
				if(UARTCtrl->HostACK!=0x0)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
				{	
					if(UARTCtrl->HostACK!=4)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					{//有其他主动上报等待确认返回
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//等待主站确认秒定时器!=0
					}
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					i=MRR(ADDR_AFN04F1+3,1);
					i>>=4;
					i&=0x3;
					if(UARTCtrl->HostCount>i)
					{//重发次数到
						UARTCtrl->HostCount=0;
						if(PORTn==GPRSPORT)
						{
							MC(0,ADDR_AFN0CF7_FM+3,1);//清当前有?个重要事件主动上传需确认
						}
	#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_AFN0CF7_FM+5,1);//清当前有?个重要事件主动上传需确认
						}
	#else
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_AFN0CF7_FM+3,1);//清当前有?个重要事件主动上传需确认
						}
	#endif
						if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
						{
							MC(0,ADDR_AFN0CF7_FM+7,1);//清当前有?个重要事件主动上传需确认
						}
					}
				}
				if(MRR(ADDR_AFN05F29,1)!=0x55)
				{//不允许终端主动上报
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
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
				//上传
				p8=(u8 *)(ADDR_DATABUFF);
				MR(ADDR_DATABUFF,ADDR_AFN0CF7_FM,2);//读事件计数
				if(PORTn==GPRSPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
				}
	#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+4;
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_AFN0CF7_FM+6;
				}
				MR(ADDR_DATABUFF+2,i,2);//读事件计数
				if(p8[3]!=0x0)
				{//原有?个重要事件主动上传没确认
					i=p8[2]+p8[3];//总需上传个数
					x=p8[3];//本次需上传个数
				}
				else
				{
					i=p8[2];//总需上传个数
					x=p8[2];//本次需上传个数
				}
				if(x==0x0)
				{//需上传数=0
					return;
				}
				T=i;//总需上传个数
				if(i>p8[0])
				{
					i=(256+p8[0])-i;
				}
				else
				{
					i=p8[0]-i;
				}
				MR((u32)p8txbuff,(u32)AutoEvent_Example,22);
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
				MR(((u32)p8txbuff)+18,ADDR_AFN0CF7_FM,2);//事件计数
				
				if(UARTCtrl->HostCount==0)
				{//不是重复帧
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变时用)
				}
				else
				{//是重复帧
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
				}
				p8txbuff[20]=i;//本帧报文传送的事件记录起始指针Pm
				p8txbuff[21]=i;//本帧报文传送的事件记录结束指针Pn
				p8[3]=0;//当前有?个重要事件主动上传需确认
				Len=16;
			#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
				x=1;//每帧只发1个ERC
			#endif
				for(;x!=0x0;x--)
				{
					y=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+1,1);
					y+=2;
				#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
					z=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT),3);
					if((z&0xff)==14)
					{//ERC14
						z>>=16;
					#if ((USER/100)==7)//用户表示：浙江	
						f=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+14,1);
						if(f==0x80)
						{
							f=0;
						}
						else
						{
							z&=0x3;
							f=MRR(ADDR_AFN04F98+12,1);//停电事件主动上报标志位	BS8	1	
							f=~f;
							f&=0x3;
							f|=z;
						}
					#else
						z&=0x3;
						f=MRR(ADDR_AFN04F98+12,1);//停电事件主动上报标志位	BS8	1	
						f=~f;
						f&=0x3;
						f|=z;
					#endif
						if(f==0)
						{
							y=0;
						}
					}
				#endif
				#if (USER/100)==5  //上海 不上报恢复类事件,只上报测量点1的发生事件
					s=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT),1);
					if(((s&0xff)==9)||((s&0xff)==10)||((s&0xff)==11)||((s&0xff)==24)||((s&0xff)==25))
					{
						k=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+8,1);
						k>>=7;
						if((k&0xff)==0)
						{
							y=0;
						}
					}
					if(((s&0xff)==24)||((s&0xff)==25))
					{
						k=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+7,2);
						if(((k&0x8000)!=0) && ((k&0xfff)!=1))
						{
							y=0;
						}
					}
				#endif
					if((Len+6+2+y)>=LEN_UARTnTx)
					{
						break;
					}
					MR(((u32)p8txbuff)+6+Len,ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT),y);
					Len+=y;
					i++;
					if(i>255)
					{
						i=0;
					}
					p8txbuff[21]=i;//本帧报文传送的事件记录结束指针Pn
					if(T!=0)//总需上传个数
					{
						T--;//当前还没上传被确认重要事件计数
					}
					p8[3]++;//当前有?个重要事件主动上传需确认
				}
				p8[2]=T;//当前还没上传被确认重要事件计数
				p8txbuff[1]=Len;
				p8txbuff[2]=Len>>8;
				if((MRR(ADDR_AFN04F1+4,1)&0x04)!=0x0)
				{//需要主站确认的通信服务（CON=1）的标志
					p8txbuff[6]|=10;//功能码10=请求∕响应帧(请求1级数据)
					p8txbuff[13]|=0x10;//CON=1
					UARTCtrl->HostACK=4;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					UARTCtrl->HostCount+=1;//重发次数
				}
				else
				{
					p8txbuff[6]|=4;//功能码4=发送∕无回答
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					p8[3]=0;//当前有?个重要事件主动上传需确认=0
				}
				if(PORTn==GPRSPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
					MWR(0,ADDR_ADCFlags,1);//重要事件等待访问0=没,1=有
				}
	#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+4;
					MWR(0,ADDR_ADCFlags+1,1);//重要事件等待访问0=没,1=有
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
					MWR(0,ADDR_ADCFlags,1);//重要事件等待访问0=没,1=有
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_AFN0CF7_FM+6;
					MWR(0,ADDR_ADCFlags+2,1);//重要事件等待访问0=没,1=有
				}
			
			#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
				if(Len==16)
				{
					UARTCtrl->HostCount=0;//重发次数
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					p8[3]=0;//当前有?个重要事件主动上传需确认=0
					MW(ADDR_DATABUFF+2,i,2);//上传计数
					return;
				}
			#endif
				MW(ADDR_DATABUFF+2,i,2);//上传计数
				if(UARTCtrl->HostCount==1)
				{//不是重发
					UARTCtrl->TxSEQ+=1;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				}
				UARTCtrl->Task=2;//2=等待单帧数据发送
				UARTCtrl->TxByte=(Len+8);//需发送字节数
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				
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
			}
		}
	}
}


#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
const u8 AutoEventMeter_Example[]=
{
	0x68,
	21,0,
	21,0,
	0x68,
	0xc0,//控制码(DIR=1上行,启动标志位PRM=1,功能码暂4=发送∕无回答,若需要主站确认的通信服务（CON=1)10=请求∕响应)
	0,0,//行政区划码
	0,0,//终端地址
	0,//启动站地址(终端启动发送帧的MSA应为零)
	0x10,//AFN=0x10
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON暂=0
	0,0,//Pn=0
	0x02,0x01,//Fn=10转发主站直接对电表的批量抄读数据命令
	0,//端口号
	0,0,0,0,0,0,//电能表地址
	6,//转发结果标识,6=电能表数据主动上报
	0,//数据标识个数
	
	//第1个数据标识总字节数
	//第1个数据标识
	//第1个数据标识的数据
	//...
	
	//cs
	//0x16
};


void Terminal_AutoEvent_Meter(u32 PORTn)//主动上报电能表事件,入口UART端口号
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Len;
	u32 T;
	u8 * p8;
	u16 * p16;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u32 LEN_UARTnTx;
#if AutoEvent_Meter_Mode==1//主动上报电能表事件模式:0=每采集一条即上报,1=采集多条后组合上报
	u32 m;
	u32 n;
	u32 p;
	u32 di;
#endif

	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
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
				
				if(UARTCtrl->HostACK!=0x0)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
				{	
					if(UARTCtrl->HostACK!=6)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					{//有其他主动上报等待确认返回
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//等待主站确认秒定时器!=0
					}
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					i=MRR(ADDR_AFN04F1+3,1);
					i>>=4;
					i&=0x3;
					//i=16;//最大帧序列号0-15
					if(UARTCtrl->HostCount>i)
					{//重发次数到
						UARTCtrl->HostCount=0;
						if(PORTn==GPRSPORT)
						{
							MC(0,ADDR_MeterEventCount_FM+3,1);//清当前有?个重要事件主动上传需确认
						}
				#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_MeterEventCount_FM+5,1);//清当前有?个重要事件主动上传需确认
						}
				#else
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_MeterEventCount_FM+3,1);//清当前有?个重要事件主动上传需确认
						}
				#endif
						if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
						{
							MC(0,ADDR_MeterEventCount_FM+7,1);//清当前有?个重要事件主动上传需确认
						}
					}
			#if (USER/100)==5//上海
					//超时没返回,为保证事件重发时ESQ相同,中间不发心跳帧
					Terminal_Ram->GPRSHeartBeat_S_Timer=60;//5 GPRS心跳秒定时器
					Terminal_Ram->ENETHeartBeat_S_Timer=60;//2 以太网心跳秒定时器
			#else
					//超时没返回,即发心跳帧
					Terminal_Ram->GPRSHeartBeat_S_Timer=0;//5 GPRS心跳秒定时器
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 以太网心跳秒定时器
			#endif
				}
//				if(MRR(ADDR_AFN05F29,1)!=0x55)
//				{//不允许终端主动上报
//					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
//					UARTCtrl->HostCount=0;
//					return;
//				}
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
				p8=(u8 *)(ADDR_DATABUFF);
				MR(ADDR_DATABUFF,ADDR_MeterEventCount_FM,2);//读事件计数
				if(PORTn==GPRSPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
				}
	#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+4;
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_MeterEventCount_FM+6;
				}
				MR(ADDR_DATABUFF+2,i,2);//读事件计数
				if(p8[3]!=0x0)
				{//原有?个重要事件主动上传没确认
					i=p8[2]+p8[3];//总需上传个数
					x=p8[3];//本次需上传个数
				}
				else
				{
					i=p8[2];//总需上传个数
					x=p8[2];//本次需上传个数
				}
				if(x==0x0)
				{//需上传数=0
					return;
				}
				T=i;//总需上传个数
				if(i>p8[0])
				{
					i=(256+p8[0])-i;
				}
				else
				{
					i=p8[0]-i;
				}
				MR((u32)p8txbuff,(u32)AutoEventMeter_Example,27);
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
				MR(((u32)p8txbuff)+18,ADDR_AFN0CF7_FM,2);//事件计数
				
				if(UARTCtrl->HostCount==0)
				{//不是重复帧
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变时用)
				}
				else
				{//是重复帧
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
				}
				p8[3]=0;//当前有?个重要事件主动上传需确认
				Len=21;
			#if ((USER/100)==5)//上海用户
				for(;x!=0x0;x--)
				{
					if(Len!=21)
					{
						break;//暂每1条即上报
					}
					y=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+7,3);//取长度
					z=y&0xff;//长度(值4+3或4+6或4+18或4+3+4+3其他值无效,发生次数和总累计时间,4+3+4+3有单独的总累计时间数据标识和数据
					y>>=8;//长度(1-10次事件数据)
					if((z!=7)&&(z!=10)&&(z!=22)&&(z!=14))
					{
						z=0;//发生次数长度错
					}
					if(y>255)
					{
						y=255;//上报数据长度只有1BYTE
					}
					if((Len+6+2+(z+y+2))>=LEN_UARTnTx)
					{
						break;
					}
					if(Len==21)
					{//装入第1个标识
						//直接取入端口号,电能表地址
						MR(((u32)p8txbuff)+18,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
					}
					else
					{//后续标识
						//比较端口号,电能表地址
						MR(ADDR_DATABUFF+256,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
						if(DataComp(ADDR_DATABUFF+256,((u32)p8txbuff)+18,7)!=0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
						{//不相同
							break;
						}
					}
					//取发生次数
					if(z!=0)
					{
						if(z==14)
						{
							p8txbuff[6+Len]=7;//长度
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10,7);
							p8txbuff[26]++;//数据标识个数
							Len+=(1+7);
							p8txbuff[6+Len]=7;//长度
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10+7,7);
							p8txbuff[26]++;//数据标识个数
							Len+=(1+7);
						}
						else
						{
							p8txbuff[6+Len]=z;//长度
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10,z);
							p8txbuff[26]++;//数据标识个数
							Len+=(1+z);
						}
					}
					//取事件
					p8txbuff[6+Len]=y;//长度
					MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+32,y);
					p8txbuff[26]++;//数据标识个数
					Len+=(1+y);
					i++;
					if(i>255)
					{
						i=0;
					}
					if(T!=0)//总需上传个数
					{
						T--;//当前还没上传被确认重要事件计数
					}
					p8[3]++;//当前有?个重要事件主动上传需确认
				}
			#else//#if ((USER/100)==5)//上海用户
			//不是上海
				for(;x!=0x0;x--)
				{
				#if AutoEvent_Meter_Mode==0//主动上报电能表事件模式:0=每采集一条即上报,1=采集多条后组合上报
					if(Len!=21)
					{
						break;//每1条即上报
					}
				#endif
					y=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+7,3);//取长度
					z=y&0xff;//长度(值4+3或4+6或4+18其他值无效,发生次数)
					y>>=8;//总长度(1-10次事件数据)
				#if(USER/100)!=7//不是浙江系
					if((z!=7)&&(z!=10)&&(z!=22))
					{
						z=0;//发生次数长度错
					}
				#else
					if((z!=7)&&(z!=10)&&(z!=22)&&(z!=6)&&(z!=5))
					{
						z=0;//发生次数长度错
					}
				#endif
					if(y>(LEN_MeterEvent-10-22))
					{
						y=(LEN_MeterEvent-10-22);//FLASH存储时最大有效数据长度
					}
					if(y!=0)
					{
						y--;//1字节总标识个数
					}
					if((Len+6+2+(z+y+2))>=LEN_UARTnTx)
					{
						break;
					}
					if(Len==21)
					{//装入第1个标识
						//直接取入端口号,电能表地址
						MR(((u32)p8txbuff)+18,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
					#if AutoEvent_Meter_Mode==1//主动上报电能表事件模式:0=每采集一条即上报,1=采集多条后组合上报
						if((p8[2]+p8[3])<(1+10))//总需上传个数
						{
							if(p8txbuff[18]==2)
							{//RS485-1
								n=RS485_1PORT;
							}
							if(p8txbuff[18]==3)
							{//RS485-2
								n=RS485_2PORT;
							}
							if(p8txbuff[18]==31)
							{//载波口
								n=RS485_4PORT;
							}
							n=Check_AutoReportWord(n);//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
							if(n!=0)
							{
								return;
							}
						}
					#endif
					}
					else
					{//后续标识
						//比较端口号,电能表地址
						MR(ADDR_DATABUFF+256,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
						if(DataComp(ADDR_DATABUFF+256,((u32)p8txbuff)+18,7)!=0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
						{//不相同
							break;
						}
					}
					//取发生次数
					if(z!=0)
					{
						p8txbuff[6+Len]=z;//长度
						MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10,z);
					#if AutoEvent_Meter_Mode==0//主动上报电能表事件模式:0=每采集一条即上报,1=采集多条后组合上报
						p8txbuff[26]++;//数据标识个数
						Len+=(1+z);
					#else
						//查发生次数是否已有
						di=MRR(((u32)p8txbuff)+6+Len+1,4);
						p=6+21;
						for(n=0;n<p8txbuff[26];n++)
						{
							m=MRR(((u32)p8txbuff)+p+1,4);
							if(m==di)
							{
								//用最新发生次数
								MR(((u32)p8txbuff)+p+1,((u32)p8txbuff)+6+Len+1,z);
								break;
							}
							p+=(p8txbuff[p]+1);
						}
						if(n>=p8txbuff[26])
						{//查无
							p8txbuff[26]++;//数据标识个数
							Len+=(1+z);
						}
					#endif
					}
					//取事件
				#if ((USER/100)==10)//河北用户
				//开端纽盖事件河北终端只需上报第一个数据内容开盖次数，第二个开盖事件记录不需要。
					if(MRR(((u32)p8txbuff)+28,4)!=0x03300E00)
					{//不是开端钮盖事件
						MR(((u32)p8txbuff)+6+Len,ADDR_MeterEvent+(i*LEN_MeterEvent)+32+1,y);
						z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,1);//取数据标识个数
						p8txbuff[26]+=z;//总数据标识个数
						Len+=y;
					}
				#else
					z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+7,3);//取长度
					z&=0xff;//长度(值4+3或4+6或4+18其他值无效,发生次数)
					if(z==0)
					{
						if(MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,4)==0x04001501)
						{
							p8txbuff[26]++;//总数据标识个数
							y++;//前因y--,1字节总标识个数,但写主动上报状态字作为事件时无此字节是直接数据帧存储
							p8txbuff[6+Len]=y;//长度
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+32,y);
							Len+=(1+y);
						}
						else
						{
							MR(((u32)p8txbuff)+6+Len,ADDR_MeterEvent+(i*LEN_MeterEvent)+32+1,y);
							z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,1);//取数据标识个数
							p8txbuff[26]+=z;//总数据标识个数
							Len+=y;
						}
					}
					else
					{
						MR(((u32)p8txbuff)+6+Len,ADDR_MeterEvent+(i*LEN_MeterEvent)+32+1,y);
						z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,1);//取数据标识个数
						p8txbuff[26]+=z;//总数据标识个数
						Len+=y;
					}

				#endif
					i++;
					if(i>255)
					{
						i=0;
					}
					if(T!=0)//总需上传个数
					{
						T--;//当前还没上传被确认重要事件计数
					}
					p8[3]++;//当前有?个重要事件主动上传需确认
				}
			#endif//#if ((USER/100)==5)//上海用户
				p8[2]=T;//当前还没上传被确认重要事件计数
				p8txbuff[1]=Len;
				p8txbuff[2]=Len>>8;
//				if((MRR(ADDR_AFN04F1+4,1)&0x04)!=0x0)
//				{//需要主站确认的通信服务（CON=1）的标志
					p8txbuff[6]|=10;//功能码10=请求∕响应帧(请求1级数据)
					p8txbuff[13]|=0x10;//CON=1
					UARTCtrl->HostACK=6;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
					UARTCtrl->HostCount+=1;//重发次数
//				}
//				else
//				{
//					p8txbuff[6]|=4;//功能码4=发送∕无回答
//					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
//					p8[3]=0;//当前有?个重要事件主动上传需确认=0
//				}
				if(PORTn==GPRSPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
					//MWR(0,ADDR_ADCFlags,1);//重要事件等待访问0=没,1=有
				}
	#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+4;
					//MWR(0,ADDR_ADCFlags+1,1);//重要事件等待访问0=没,1=有
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
					//MWR(0,ADDR_ADCFlags,1);//重要事件等待访问0=没,1=有
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_MeterEventCount_FM+6;
					//MWR(0,ADDR_ADCFlags+2,1);//重要事件等待访问0=没,1=有
				}
			
				MW(ADDR_DATABUFF+2,i,2);//上传计数
				if(UARTCtrl->HostCount==1)
				{//不是重发
					UARTCtrl->TxSEQ+=1;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				}
				UARTCtrl->Task=2;//2=等待单帧数据发送
				UARTCtrl->TxByte=(Len+8);//需发送字节数
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				
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
			}
		}
	}
}
#endif
