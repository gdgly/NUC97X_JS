
//376.2
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../DL698/PROXY_TASK.h"
#include "../DL698/RM_TASK.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../DL698/SearchMeter_Record.h"
#include "../DL698/MeterAutoReport.h"
#include "../DL698/MeterAutoReport_Record.h"
#include "../TEST/LOG.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/DL698_JSON.h"




#include "../MS/MS.h"


#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

u32 UART_3762_Rx(u32 PORTn)//UART 3762接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
{
	u32 i;
	u16 * p16timer;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rx;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		return 0;//0=正在接收,1=完成1帧接收,2=接收超时
	}
	if(p0!=p1)
	{//有数据接收,重置接收超时定时
		p16timer[0]=500/10;//字节间超时定时500MS
	}
	else
	{
		if(p16timer[0]==0x0)
		{//超时
			UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			return 2;//0=正在接收,1=完成1帧接收,2=接收超时
		}
	}
	RxByte=UARTCtrl->RxByte;
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-10))
		{
			p1=0;
		}
		p8rx[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(i!=0x68)
			{
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte>6)
			{
				if(RxByte>=(p8rx[1]+(p8rx[2]<<8)))
				{
					if(i==0x16)
					{//接收完成
						i=Uart_3762_CS((u32)p8rx);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
						if(i!=0)
						{//和校验错误
							RxByte=0;
							continue;
						}
						//和校验正确
					#if LOG_1376_2==1
						LOG(13762,0,PORTn,p8rx,RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
					#if DEBUG_RS485_4toRS232==1//0=否,1=是;调试:RS485_4(载波)口收发数据打印到RS232口
						LOGRS232(13762,0,PORTn,p8rx,RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
						
						UARTCtrl->Task=1;//1=完成1帧发收
						p16fifo[1]=p1;
						UARTCtrl->RxByte=RxByte;
						Uart_3762_RxData(PORTn);//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
						p16timer[0]=30/10;//接收到发送延时
						if(UARTCtrl->Task==1)//1=完成1帧发收
						{
							//载波接受的字节
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:载波完成1帧接收\n ",(u32)&__func__,(u32)__LINE__,0);	
							for(i=0;i<UARTCtrl->RxByte;i++)
								myprintf("%02x ",p8rx[i],0,0);
							myprintf("\n",0,0,0);
							#endif
							return 1;//0=正在接收,1=完成1帧接收,2=接收超时
						}
						return 0;//确认回答发送作为0=正在接收
					}
					else
					{
						RxByte=0;
						continue;
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	return 0;//0=正在接收,1=完成1帧接收,2=接收超时
}


u32 Uart_3762_RxTx(u32 PORTn)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=接收完成,7=抄表发送(同RS485的3=启动发送);返回:0=正常返回,1=代理返回
{
	u32 i;
	u16 * p16timer;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->Task;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	switch(i)
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			{
				//空闲时进入其他任务
				i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				if(i)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				{
					PROXY_TASK_3762(PORTn);//代理任务376.2
					return 1;//0=正常返回,1=代理返回
				}
				//广播校时
				i=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待,2~127校时-2过程;0x81=校时-3等待,0x82~0xFF校时-3过程
				if((i&0x7f)==1)
				{
					i++;
					UARTCtrl->BroadcastTimeTask=i;
					return 1;//0=正常返回,1=代理返回
				}
				//自动搜表
				i=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
				if(i==1)
				{
					i++;
					UARTCtrl->AutoSearchTask=i;//自动搜表任务:0=空,1=等待,2...过程
					return 1;//0=正常返回,1=代理返回
				}
				//电能表主动上报
				i=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
				if(i==0)
				{
					i=UARTCtrl->AutoReportCopyNUM;//主动上报接收已经缓存帧数
					if(i)
					{
						MeterAutoReportStateWord(PORTn,(u8*)ADDR_UART4RxCopy);//主动上报状态字;入口:自动上报接收帧;返回:0=没接受,1=接受
						MR(ADDR_UART4RxCopy,ADDR_UART4RxCopy+LEN_UART4RxCopy,(i-1)*LEN_UART4RxCopy);
						i--;
						UARTCtrl->AutoReportCopyNUM=i;//主动上报接收已经缓存帧数
						UARTCtrl->AutoReportTask=2;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
						return 1;//0=正常返回,1=代理返回
					}
				}
				if(i==1)
				{
					i++;
					UARTCtrl->AutoReportTask=i;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
					return 1;//0=正常返回,1=代理返回
				}
				
			}
			return 0;//0=正常返回,1=代理返回
		case 1://1=完成1帧发收
		case 2://2=接收超时(或错误)
		case 6://6=抄表发送(同RS485的3=启动发送)
			//代理任务只在空闲时进入
			i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			if(i>=2)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			{
				if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
				{
					UARTCtrl->Task=0;
				}
			}
			break;
		case 3://3=启动发送
		//#if (USER/100)==0//电科院测试
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
			if(p16timer[0]!=0)
			{
				break;
			}
		//#endif	
//			UARTCtrl->BpsCtrl=0xb+(6<<5);//9600,8e1
//			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
//			p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
//			p16fifo[0]=0;//中断接收指针
//			p16fifo[1]=0;//当前已读指针
		#if LOG_1376_2==1
			LOG(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
		#if DEBUG_RS485_4toRS232==1//0=否,1=是;调试:RS485_4(载波)口收发数据打印到RS232口
			LOGRS232(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=UARTCtrl->TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;
				p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
			#if((USER/100)==0||(USER/100)==17)//电科院测试和江苏系
				p16timer[0]=600/10;//下次发送延时
			#else
				p16timer[0]=100/10;//下次发送延时
			#endif
			}
			break;
		case 5://5=正在接收
			//myprintf("[%s:%d]:载波正在接受\n",(u32)&__func__,(u32)__LINE__,0);
			i=UART_3762_Rx(PORTn);//UART 3762接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
			switch(i)
			{
				default://正在接收
/*
				#if (USER/100)==0//电科院测试
					i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
					i|=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
					i|=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
					i|=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
					if(i==1)
					{//有其他任务准务进入时
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						if(p16timer[0]>(15000/10))
						{
							p16timer[0]=(15000/10);
						}
						return;
					}
				#endif
*/
					break;
				case 1:
					//myprintf("[%s:%d]:载波接受完成\n",(u32)&__func__,(u32)__LINE__,0);
					UARTCtrl->Task=1;//1=完成1帧发收
					break;
				case 2:
					UARTCtrl->Task=2;//2=接收超时(或错误)
					break;
			}
			break;
	}
	return 0;//0=正常返回,1=代理返回
}


u32 Uart_3762_CS(u32 ADDR_BUFF)//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
{
	u32 i;
	u32 x;
	u32 cs;
	u8 * p8;

	p8=(u8 *)(ADDR_BUFF);
	x=(p8[1]+(p8[2]<<8))-5;
	cs=0;
	for(i=0;i<x;i++)
	{
		cs+=p8[3+i];
	}
	cs&=0xff;
	if(cs==p8[3+i])
	{
		cs=0;
	}
	else
	{
		p8[3+i]=cs;
		cs=1;
	}
	p8[4+i]=0x16;
	return cs;
}

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
void Uart_3762_TxData(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data)//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
{
	u32 A;
	u32 L;
	u32 PRM;//启动帧标志
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;

	PRM=1;//启动帧标志
  p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->TxToRxTimerOver=6000/10;//发送后接收超时定时值ms/10(半字对齐)
	switch(AFN)
	{
		case 0://确认/否认
			MC(0,((u32)p8tx)+4,5);//信息域R
			PRM=0;//启动帧标志
			UARTCtrl->TxToRxTimerOver=0;//发送后接收超时定时值=0,使能很快超时而转空闲以便数据转发
			break;
		case 1://初始化
			MC(0,((u32)p8tx)+4,6);//信息域R
			break;
		case 2://转发数据
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			break;
		case 3://查询数据
			MC(0,((u32)p8tx)+4,6);//信息域R
			switch(Fn)
			{
				case 3://从节点侦听信息
					p8tx[4]|=0x5;//R-1的通信模块标识=1,路由标识=1
					break;
				case 4://主节点地址
					break;
				case 6://主节点干忧状态
					p8tx[4]|=0x1;//R-1的路由标识=1
					UARTCtrl->TxToRxTimerOver=(60000+10000)/10;//发送后接收超时定时值ms/10(半字对齐)
					break;
			}
			break;
		case 4://链路接口检测
//			MC(0,((u32)p8tx)+4,6);//信息域R
//			switch(Fn)
//			{
//				case 1://发送测试
//					p8=(u8 *)(MenuData_ADDR);
//					p16ctrl[15]=p8[0]+5;//发送后接收超时定时值S
//					break;
//				case 2://载波从节点名
//					p8tx[4]|=0x5;//R-1的通信模块标识=1,路由标识=1
//					p16ctrl[15]=10;//发送后接收超时定时值S
//					break;
//			}
//			break;
			return;
		case 5://控制命令
			MC(0,((u32)p8tx)+4,6);//信息域R
			if(Fn==3)
			{//启动广播
				//真实超时用Terminal_Ram->RouterCheck_S_Timer//最大广播超时时间秒(半字对齐)
				UARTCtrl->TxToRxTimerOver=120000/10;//发送后接收超时定时值ms/10(半字对齐)
			}
			break;
		case 6://主动上报
			break;
		case 0x10://路由查询
			MC(0,((u32)p8tx)+4,6);//信息域R
			break;
		case 0x11://路由设置
			MC(0,((u32)p8tx)+4,6);//信息域R
			break;
		case 0x12://路由控制
			MC(0,((u32)p8tx)+4,6);//信息域R
			break;
		case 0x13://路由数据转发
			if((Terminal_Router->MaxOverTime!=0)&&(Terminal_Router->MaxOverTime<120))
			{
				UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			}
			else
			{
				UARTCtrl->TxToRxTimerOver=60000/10;//发送后接收超时定时值ms/10(半字对齐)
			}
			break;
		case 0x14://路由数据抄读
			switch(Fn)
			{
				case 3://请求依通信延时修正通信数据
					break;
				default:
					MC(0,((u32)p8tx)+4,5);//信息域R
					break;
			}
			PRM=0;//启动帧标志
			break;
		case 0x15://文件传输
			//F1文件传输方式1
			MC(0,((u32)p8tx)+4,6);//信息域R
			break;
		case 0xF0://内部测试
			MC(0,((u32)p8tx)+4,6);//信息域R
			break;
		default:
			return;
	}
	A=0;
	if((p8tx[4]&0x4)!=0x0)
	{
		A=12;
	}
	A+=((p8tx[4]>>4)&0xf)*6;
	p8tx[10+A]=AFN;
	Fn--;
	p8tx[10+A+1]=1<<(Fn%8);
	p8tx[10+A+2]=Fn/8;
	MR(((u32)p8tx)+10+A+3,((u32)p8tx)+128,LEN_Data);
	L=10+A+3+LEN_Data+2;
	p8tx[0]=0x68;
	p8tx[1]=L;//总字节数
	p8tx[2]=L>>8;
	if(PRM==0)//启动帧标志
	{
		if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8tx[3]=0x03;//控制码;集中式路由载波通信
		}
		else
		{
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{
				p8tx[3]=0x0a;//控制码;微功率无线通信
			}
			else
			{
				p8tx[3]=0x01;//控制码;集中式路由载波通信
			}
		}
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8tx[3]=0x43;//控制码;集中式路由载波通信
		}
		else
		{
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{
				p8tx[3]=0x4a;//控制码;微功率无线通信
			}
			else
			{
				p8tx[3]=0x41;//控制码;集中式路由载波通信
			}
		}
		UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
		p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	}
	Uart_3762_CS((u32)p8tx);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
	UARTCtrl->TxByte=L;//发送字节数(半字对齐)
	UARTCtrl->Task=3;//等待发送
}

void Uart_3762_AFN13F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag)//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
{
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	p8tx[0]=0x68;
	p8tx[1]=(29+LEN_Data+2);//需总发送字节数
	p8tx[2]=(29+LEN_Data+2)>>8;
	if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{
		p8tx[3]=0x43;//控制码;集中式路由载波通信
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8tx[3]=0x4a;//控制码;微功率无线通信
		}
		else
		{
			p8tx[3]=0x41;//控制码;集中式路由载波通信
		}
	}
	p8tx[4]=4;//通信模块标识=1
	p8tx[5]=0;
	p8tx[6]=LEN_ESC;//预计应答字节数
	p8tx[7]=0;
	p8tx[8]=0;
	UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
	//MR(((u32)p8tx)+16,ADDR_Data+1,6);//从节点地址(目的地址)
	d64a=UARTCtrl->RMAddr[2];
	d64a<<=16;
	d64a|=UARTCtrl->RMAddr[1];
	d64a<<=16;
	d64a|=UARTCtrl->RMAddr[0];
	d64a=R_Inverse(d64a,6);
	MWR(d64a,((u32)p8tx)+16,6);//从节点地址(目的地址)
	
	p8tx[22]=0x13;//AFN=13H
	p8tx[23]=1;//Fn=1
	p8tx[24]=0;
	p8tx[25]=Protocol;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
	p8tx[26]=DelayFlag;//通信延时相关标志
	p8tx[27]=0;//附属节点数=0
	p8tx[28]=LEN_Data;//DL645报文长度
	MR(((u32)p8tx)+29,ADDR_Data,LEN_Data);//标准645帧

	Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
}

u32 Uart_3762_AFN13F1_Rx(u32 PORTn)//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
{
	u32 i;
	u32 x;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	x=0;//正确
	i=p8rx[1]+(p8rx[2]<<8);
	if(i<=16)
	{//错误
		return 4;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	if(UARTCtrl->Task!=1)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{//超时
		return 5;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	i=MRR(((u32)p8rx)+22,3);
	if(i!=0x113)
	{//AFN=13H,Fn=1
		return 255;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	i=p8rx[28];//返回数据域长度
	if(i==0)
	{//接收数据长度=0
		return 255;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	if(x==0)
	{
		MR((u32)p8rx,((u32)p8rx)+28+1,i);
		//通信地址比较
		for(i=0;i<6;i++)
		{
			if(p8rx[1+i]!=p8tx[29+1+i])
			{
				break;
			}
		}
		if(i!=6)
		{
			return 1;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
		}
		//控制码
		if((p8rx[8]&0x40)!=0x0)
		{//从站异常回答
			return 2;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
		}
		//减0x33
		for(i=0;i<p8rx[9];i++)
		{
			p8rx[10+i]-=0x33;
		}
	}
	return 0;
}

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
u32 Get_Uart_3762_RxData_AFN_Fn(u32 PORTn)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
{
	u32 AFN;
	u32 Fn;
	u8 * p8rx;
	
  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	AFN=p8rx[10];
	Fn=BitLoca(p8rx[11]);//计算第1个置位为1的位置(0-32)
	Fn+=(p8rx[12]*8);
	return((AFN<<8)+Fn);
}

u32 Uart_3762_RxData_AFN00_F1(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data)//UART_3762接收AFN00F1确认数据,入口AFN,Fn,LEN_Data接收失败重发用参数;返回0未确认1确认2失败
{
	UARTCtrl_TypeDef * UARTCtrl;
	u16 *p16timer;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		case 1://1=完成1帧发收
			if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
			{
				UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
				Terminal_Router->RouterInitReCount=0;//命令重发计数
				Terminal_Router->RouterInitSubTask=0;//子任务
				Terminal_Router->RouterInitTask++;//路由器抄读任务
				return 1;//返回0未确认1确认2失败
			}
			else
			{
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			}
			break;
		case 0://0=空闲
		case 2://2=接收超时(或错误)
			if(Terminal_Router->RouterInitReCount<2)
			{
				Terminal_Router->RouterInitReCount++;
				Uart_3762_TxData(PORTn,AFN,Fn,LEN_Data);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			}
			else
			{//失败
				UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
				Terminal_Router->RouterInitReCount=0;//命令重发计数
				Terminal_Router->RouterInitSubTask=0;//子任务
				Terminal_Router->RouterInitTask++;//路由器抄读任务
				return 2;//返回0未确认1确认2失败
			}
			break;
	}
	return 0;//返回0未确认1确认2失败
}

u32 Get_ListNo(u8* p8);//得到并发列表号;入口:抄表地址;返回:列表号0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax表示无效)
#ifdef DEBUG_JSON
void  log_CollectorAddr(void)
{
	u32 i;
	u32 y;
	u32 x;
	u32 z;
	u64 d64a;
	u64 d64b;
	
	x=Terminal_Router->NoteNum;//已知总从节点数
	myprintf("[%s:%d]:打印所有有效的采集器地址\n",(u32)__func__,(u32)__LINE__,0);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
		if(d64b!=0)
		{	
			for(z=0;z<i;z++)
			{
				d64a=MRR((u32)&NoteList->List[z].CollectorAddr,6);
				if(d64b==d64a)
				{
					break;
				}
			}
			if(z>=i||i==0)
			{
				for(y=6;y>0;y--)
				{	
					myprintf("%02x ",NoteList->List[i].CollectorAddr[y-1],0,0);
				}
				myprintf("\n",0,0,0);
			}	
		}
	}
}
#endif
void Uart_3762_RxData(u32 PORTn)//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
{
//	u32 iii;
	u32 i;
#if(USER/100)==17//江苏系	
	u32 j;
#endif
	u32 x;
	u32 n;
	u32 m;
	u32 AFN;
	u32 Fn;
	u32 Protocol;
	u64 CollectorAddr;
	u64 MeterAddr;
	u32 Phase;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 *p16timer;
	u8* p8;
	u32 offset;
	u8	pOccurSour[16];		  // 存放事件发生源    OAD
//	u8 *p8s;
//	u8 *p8d;
//	u32 z;
//	u64 d64a;
//	u64 d64b;

	ms_Type *ms;
	ms=Get_ms();
	
  	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	Phase=p8rx[6]&0x0f;//电表相位
	if((p8rx[4]&0x4)!=0)
	{//通信模块标识：0表示对集中器的通信模块操作,1表示对载波表的通信模块操作
		p8rx+=12;
	}
	AFN=p8rx[10];
	Fn=BitLoca(p8rx[11]);//计算第1个置位为1的位置(0-32)
	Fn+=(p8rx[12]*8);
	
	switch(AFN)
	{
		case 3://
			switch(Fn)
			{
				case 0x10:
					break;
			}
		case 0xF1://AFN=F1H,Fn=1,并发抄表接收帧
			switch(Fn)
			{
				case 1://AFN=F1H,Fn=1,并发抄表接收帧
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					MR(ADDR_DATABUFF,(u32)p8rx+10,6);
					Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
					i=Get_ListNo((u8*)ADDR_DATABUFF);//得到并发列表号;入口:抄表地址;返回:列表号0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax表示无效)
					if(i<NUM_RMLISTmax)
					{
						RMLIST->List[i].TIMER_S=0;//并发启动发送后等待接收秒分定时
						RMLIST->List[i].TASK=1;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
						if(Terminal_Router->NUM_Enable_Concurrency<5)//宽带载波当前还允许并发帧数
						{
							Terminal_Router->NUM_Enable_Concurrency++;//宽带载波当前还允许并发帧数
						}
					}
					break;
			}
			return;
	#if(USER/100)==17//江苏系
		case 0x14://AFN=14H,请求时钟
			switch(Fn)
			{
				case 2://F2请求时钟
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					//保护发送区的21Byte!!! 免返回确认帧后认为接收超时而错误重发数据
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					n=p16timer[0];//发送后接收超时定时值ms/10(半字对齐)
					MR(ADDR_DATABUFF,(u32)p8tx,32);
					MR((u32)p8tx,(u32)p8rx,10);//复制10字节接收头到发送头
					MW(ADDR_MYMDHMS,((u32)p8tx)+128,6);
					Uart_3762_TxData(PORTn,0x14,2,6);
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
					p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
					if(p16timer[0]>(30/10))
					{
						p16timer[0]=30/10;
					}
					while(1)
					{
						Uart_3762_RxTx(PORTn);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=接收完成,7=抄表发送(同RS485的3=启动发送);返回:0=正常返回,1=代理返回
						if((UARTCtrl->Task<3)||(UARTCtrl->Task>4))
						{
							break;
						}
					}
					//恢复发送区数据和相关超时定时值
					MW(ADDR_DATABUFF,(u32)p8tx,32);
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					p16timer[0]=n;//发送后接收超时定时值ms/10(半字对齐)
					i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
					if(i>=2)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
					{
						if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
						{
							UARTCtrl->Task=0;
						}
					}
					break;
			}
			break;
	#endif
		case 6://AFN=06H,主动上报
			switch(Fn)
			{
//F1上报从节点信息
				case 1://F1上报从节点信息
					n=p8rx[13];//本次上报从节点的数量n
					for(i=0;i<n;i++)
					{
						MeterAddr=MRR(((u32)p8rx)+14+(i*9),6);
						Protocol=p8rx[14+6+(i*9)];//00H＝透明传输；01H＝DL/T645—1997；02H＝DL/T645—2007，03H～FFH保留
						//if(Protocol!=1)
						//{
						//	Protocol=30;
						//}
						//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,0x0,0,Phase);//自动设置AFN04F10的电能表配置
						SearchMeter_Record(PORTn,3,MeterAddr,Protocol,0,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
					}
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
					break;
//F2上报抄读数据
				case 2://F2上报抄读数据
					return;
//F3上报路由工况变动信息
				case 3://F3上报路由工况变动信息
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
					switch(p8rx[13])
					{
						case 1://1为抄表任务结束
							Terminal_Router->RUNState&=~2;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
							break;
						case 2://2为搜表任务结束
							if(UARTCtrl->AutoSearchTask==(6+1))//自动搜表任务:0=空,1=开始,2...过程
							{//6=自动搜表接收中判读路由运行状态定时
								Terminal_Ram->AutoSearch_S_Timer=0;//28 载波自动搜表秒定时器
							}
							break;
					}
					break;
//F4上报从节点信息及设备类型
				case 4://F4上报从节点信息及设备类型
					switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
					{
						default://其他
						case 2://2=鼎信(上报采集器)
						#ifdef DEBUG_JSON
							myprintf("[%s:%d]:06f4上报搜表结果\n",(u32)__func__,(u32)__LINE__,0);
							for(i=0;i<120;i++)
							{	
								myprintf("%02x ",p8rx[i],0,0);
							}
							myprintf("\n",0,0,0);
						#endif
							n=p8rx[13];//本次上报从节点的数量n
							CollectorAddr=MRR(((u32)p8rx)+13+1,6);
							Protocol=p8rx[13+1+6];//00H＝透明传输；01H＝DL/T645—1997；02H＝DL/T645—2007，03H～FFH保留
							//if(Protocol!=1)
							//{
							//	Protocol=30;
							//}
							//Driver=p8rx[14+9];//设备类型：01:081 采集器，00:082 采集器，02：开箱监视器；03H—FFH 保留
							p8rx+=13+11;
							for(i=0;i<n;i++)
							{
								m=p8rx[1];//本次上报的从节点1下接的从节点数量n1
								if(m==0)
								{//只上报采集器,无下属485表
								#if(USER/100)==17//江苏系
									SearchMeter_Record(PORTn,3,CollectorAddr,Protocol,CollectorAddr,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
									p8rx+=2;
									
									SearchMeter_CollectorAddrRecord_JS(CollectorAddr);
									continue;
								#else
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
									SearchMeter_Record(PORTn,3,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
									p8rx+=2;
									continue;
								#endif
								}
							#if (USER/100)==17//江苏系
								SearchMeter_Record_JS(CollectorAddr,m,p8rx);
							#endif
								for(x=0;x<m;x++)
								{//下属485表
									MeterAddr=MRR(((u32)p8rx)+2+(7*x),6);
									Protocol=p8rx[2+(7*x)+6];//00H＝透明传输；01H＝DL/T645—1997；02H＝DL/T645—2007，03H～FFH保留
									//if(Protocol!=1)
									//{
									//	Protocol=30;
									//}
									//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
									SearchMeter_Record(PORTn,3,MeterAddr,Protocol,CollectorAddr,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
								}
								p8rx+=2+(7*x);
							}
							#ifdef DEBUG_JSON
							log_CollectorAddr();
							#endif
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
							break;
//						default://其他
						case 3://东软
						case 5://瑞斯康
							CollectorAddr=MRR(((u32)p8rx)+13+1,6);
							Protocol=p8rx[13+1+6];//00H＝透明传输；01H＝DL/T645—1997；02H＝DL/T645—2007，03H～FFH保留
							//if(Protocol!=1)
							//{//通信协议类型：00H＝透明传输；01H＝DL/T645-1997；02H＝DL/T645-2007；03H～FFH保留
							//	Protocol=30;
							//}
							i=p8rx[14+9];//c)	设备类型：00H＝采集器；01H＝电能表；02H-FFH 保留
							if(i==1)
							{//上报电能表
								//AutoSetAFN04F10(PORTn,CollectorAddr,Protocol,0x0,0,Phase);//自动设置AFN04F10的电能表配置
								SearchMeter_Record(PORTn,3,CollectorAddr,Protocol,0,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
							}
							else
							{//上报采集器
								p8rx+=13+11;
								m=p8rx[1];//本次上报的从节点1下接的从节点数量n1
								if(m==0)
								{//只上报采集器,无下属485表
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
									SearchMeter_Record(PORTn,3,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
								}
								else
								{//下属485表
									for(x=0;x<m;x++)
									{
										MeterAddr=MRR(((u32)p8rx)+2,6);
										Protocol=p8rx[2+6];//00H＝透明传输；01H＝DL/T645—1997；02H＝DL/T645—2007，03H～FFH保留
										//if(Protocol!=1)
										//{//通信协议类型：00H＝透明传输；01H＝DL/T645-1997；02H＝DL/T645-2007；03H～FFH保留
										//	Protocol=30;
										//}
										//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
										SearchMeter_Record(PORTn,3,MeterAddr,Protocol,CollectorAddr,0,Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
										p8rx+=6+1;
									}
								}
							}
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
							break;
					}
					break;
//F5上报从节点事件
				case 5:
					n=0;
					n|=p8rx[1];
					n|=p8rx[2]<<8;
					ms->link_printf("\n************************AFN06F5**************\r\n", 0, 0, 0);
					for(i=0;i<n;i++)
					{
						ms->link_printf(" %02x",p8rx[i],0,0);
					}
					ms->link_printf("\n", 0, 0, 0);
					//F5上报从节点事件
					//停电上报事件扩展
					//从节点设备类型 从节点设备类型 从节点设备类型BIN 1
					//通信协议类型 通信协议类型 通信协议类型BIN 1
					//报文长度 报文长度 L	BIN 1
					//报文内容 报文内容 -	L
					//从节点设备类型：00H 为采集器；01H 为电能表；02H：宽带载波通信单元；03H：窄带载波通信单元；04H：微功率无线通信单元；05H：双模通信单元；06H～FFH 保留
					//通信协议类型：00H 为保留；01H 为DL/T 645—1997；02H 为DL/T 645—2007； 03H代表698.45,04H～FFH 保留			
					//通信单元停电上报事件的“报文内容域”定义如下：
					//1.通信单元事件类型（一个字节，01H代表停电事件，02H代表复电事件,其他保留）
					//2.报文内容域为发生停电事件的通信单元地址序列（即电表或采集器地址），长度6*N字节（N代表发生该事件的节点数量）。当掉电节点数超过255时，进行分帧上报。			
					Protocol=p8rx[13+1];
					
					ms->link_printf("\n************#11111**************\n", 0, 0, 0);
					if(Protocol==05)//通信协议类型, 05H 代表拒绝节点信息上报报文
					{
					/*
						68 1A 00 C3 00 00 00 00 01 05 06 10 00 
						02 // 节点设备类型
						05 // 通讯协议类型 13 +1
						08 // 数据长度 13+2
						01 //本次上报节点个数13+3
						32 00 00 00 00 00 // 节点地址
						05				  //  协议类型
						26 16 
		
					*/	
						n=p8rx[13+3];
						p8rx+=(13+4);
						p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
						p8[0]=DataType_array;
						p8[1]=n;
						i=0;
						for(i=0;i<n;i++)
						{
							p8[2+(6+2)*i]=DataType_octet_string;
							p8[2+1+(6+2)*i]=6;
							Data_Inverse((u32)p8rx+i*7,6);
							MR((u32)p8+2+2+((6+2)*i),(u32)p8rx+i*7,6);
						}
						pOccurSour[0]=DataType_TSA;
						pOccurSour[1]=7;
						pOccurSour[2]=5;
						MR((u32)pOccurSour+3,ADDR_4001+2,6);
						ms->link_printf("\n********************311F********************\n", 0, 0, 0);
						Event_311F0200(p8,pOccurSour,2+8*n);
					}
				
					ms->link_printf("\n************#22222**************\n", 0, 0, 0);
					i=p8rx[13+3];
					if((i==1)||(i==2))
					{//是停电或停电恢复上报事件
						n=p8rx[13+2];//报文长度
						if(n>=7)
						{
							n--;//地址个数
							n/=6;
							p8rx+=13+4;
							while(n--)
							{
#if(USER/100)==17//江苏系			
								j=p8rx[6];
								if(((i==1)&&(j==0))||((i==2)&&(j==1)))
								{// 停电
									MeterAutoReport_3011(p8rx,i);//载波口上报从节点事件(电能表停电事件);入口:pAddr=从路由模块接收到的接收缓冲中的6字节地址(电表或采集器地址)							
								}
								p8rx+=7;							
#else
								MeterAutoReport_3011(p8rx,i);//载波口上报从节点事件(电能表停电事件);入口:pAddr=从路由模块接收到的接收缓冲中的6字节地址(电表或采集器地址)
								p8rx+=6;
#endif
							}
						}
						break;
					}
					//(河南)台区区分
					//68H	A0	……	A5	68H	9EH	07H	03H	PA0	……	PA5	CS		16H
					//控制码：C=9EH
					//地址域：电表地址
					//长度域：L=1+6=7
					//数据单元标识：02H代表本台区站点但不在本台区表档案中。（不加33H）
					//              03H代表非本台区站点但在本台区表档案中。（不加33H）
					//数据单元内容：PA0~ PA5为归属载波主节点地址
					//建议：跨台区电表上报事件主节点地址未知时按照全FF填充
					i=p8rx[13+3];
					if(i==0x68)
					{
						i=p8rx[13+3+8];
						if(i==0x9e)
						{
							i=p8rx[13+3+9];
							if(i==7)
							{
#if(USER/100)==17//江苏系									
								// 非标准645协议，数据标识和数据域都要减去0x33
								for(offset=0;offset<i;offset++)
								{
									p8rx[13+3+10+offset]=p8rx[13+3+10+offset]-0x33;
								}
#endif								
								//台区状态： 
								//0----未知：获取的节点的台区地址为无效的地址，	
								//1----本台区站点：节点所属的台区地址和本台区地址相同，属于本台区节点，且为档案中节点。
								//2----本台区站点且但不在表档案：节点所属的台区属于本台区，但不在本台区表档案中。
								//3----非本台区站点但在表档案：节点所属的台区不属于本台区，但在本台区表档案中。
								//4----该站点不支持台区识别：获取不到节点所属的台区地址。
								//其他预留。
								i=p8rx[13+3+10];
								switch(i)
								{
									case 1://1----本台区站点：节点所属的台区地址和本台区地址相同，属于本台区节点，且为档案中节点。
									case 2://2----本台区站点且但不在表档案：节点所属的台区属于本台区，但不在本台区表档案中。
										break;
									default:
										p8=(u8*)ADDR_DATABUFF+1024;
								//一个跨台区结果∷=structure
								//{
										p8[0]=DataType_structure;
										p8[1]=3;
										offset=2;
								//	通信地址       TSA，
										p8[offset]=DataType_TSA;
										p8[offset+1]=7;
										p8[offset+2]=5;
										MR((u32)p8+offset+3,(u32)p8rx+13+3+1,6);
										Data_Inverse((u32)p8+offset+3,6);//数据倒序(高低字节调换),入口Le>=2
										offset+=9;
								//	主节点地址     TSA，
										p8[offset]=DataType_TSA;
										p8[offset+1]=7;
										p8[offset+2]=5;
										MR((u32)p8+offset+3,(u32)p8rx+13+3+11,6);
										Data_Inverse((u32)p8+offset+3,6);//数据倒序(高低字节调换),入口Le>=2
										offset+=9;
								//	变更时间       date_time_s
										p8[offset]=DataType_date_time_s;
										Get_date_time_s(p8+offset+1);//得到date_time_s共7字节
										offset+=8;
								//}
								
										Event_31120200(p8);//跨台区电能表事件记录;入口:pDATA=一个跨台区结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF


										Data_Inverse((u32)(u32)p8rx+13+3+1,6);
										MeterAddr=MRR((u32)p8rx+13+3+1,6);
										
										Data_Inverse((u32)(u32)p8rx+13+3+11,6);
										CollectorAddr=MRR((u32)p8rx+13+3+11,6);
										SearchMeter_Record(PORTn,4,MeterAddr,0,CollectorAddr,0,0);
										break;
								}
							}
						}
					}
				#if(USER/100)==17//江苏系
					i=p8rx[13+3];
					if(i==0x68)
					{
						i=p8rx[13+3+14];
						if(i==0x88)//上报通知
						{
							i=(p8rx[13+3+18]<<8)+p8rx[13+3+19];
							if(i==0x310A)//疑似停电/复电事件上报
							{
								p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
								i=p8rx[13+4];
								if(i==0x50)//疑似停复电事件发生
								{
									p8[0]=DataType_enum;//产生源
									p8[1]=3;//事件发生源：485抄表故障
									Event_310A0200(p8,1,p8rx+21);
								}
								else		//疑似停复电事件恢复
								{		
									p8[0]=DataType_enum;//产生源
									p8[1]=3;//事件发生源：485抄表故障
									Event_310A0200(p8,0,p8rx+21);
								}
							}
						}
					}
				#endif
					//查前导0xfe个数
					i=0;
					while(p8rx[i+13+3]==0xfe)
					{
						i++;
					}
					for(x=0;x<p8rx[i+13+3+9];x++)
					{
						p8rx[i+13+3+10+x]-=0x33;
					}
					x=MRR(((u32)p8rx)+i+13+3+10,4);
					if(x==0x04001501)
					{
					#if EnMeterAutoReportEvent==1//允许电能表主动上报事件:0=不允许,1=允许
						//WriteAutoReportWord(PORTn,((u32)p8rx)+i+13+3);//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
						i=MeterAutoReportStateWord(PORTn,p8rx+i+13+3);//主动上报状态字;入口:自动上报接收帧;返回:0=没接受,1=接受
						if(i==0)
						{
							//return;//0=没接受
						}
					#endif
					}
					break;
			}
			
			//2018.3.1修改确认返回帧
	  	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			//保护发送区的21Byte!!! 免返回确认帧后认为接收超时而错误重发数据
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			n=p16timer[0];//发送后接收超时定时值ms/10(半字对齐)
			MR(ADDR_DATABUFF,(u32)p8tx,32);
			MR((u32)p8tx,(u32)p8rx,10);//复制10字节接收头到发送头
			MWR(0xffffffff,((u32)p8tx)+128,6);
			Uart_3762_TxData(PORTn,0,1,6);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
		//#if (USER/100)==0//电科院测试
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
			if(p16timer[0]>(30/10))
			{
				p16timer[0]=30/10;
			}
		//#endif	
			while(1)
			{
				Uart_3762_RxTx(PORTn);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=接收完成,7=抄表发送(同RS485的3=启动发送);返回:0=正常返回,1=代理返回
				if((UARTCtrl->Task<3)||(UARTCtrl->Task>4))
				{
					break;
				}
			}
			//恢复发送区数据和相关超时定时值
			MW(ADDR_DATABUFF,(u32)p8tx,32);
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
//		#if (USER/100)==0//电科院测试
//			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
//			p16timer[0]=500/10;//发送后接收超时定时值ms/10(半字对齐)
//		#else
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			p16timer[0]=n;//发送后接收超时定时值ms/10(半字对齐)
//		#endif
			i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			if(i>=2)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			{
				if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
				{
					UARTCtrl->Task=0;
				}
			}
			break;
	}	
	
}
#endif







