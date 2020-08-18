
//376.2
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "Test_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"

#include "../Calculate/Calculate.h"
//#include "Terminal_AFN0E_Event.h"
//#include "Terminal_AFN0C_RealTimeData_Fn.h"
//#include "Terminal_ReadMeter_SaveFnData.h"

//#include "AutoSearchMeter.h"
//#include "Terminal_ReadMeter_Event.h"

#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void Uart_3762_RxTx(u32 PORTn)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			break;
		case 1://1=完成1帧发收
			break;
		case 2://2=接收超时(或错误)
			break;
		case 3://3=启动发送
		#if (USER/100)==0//电科院测试
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
			if(p16timer[0]!=0)
			{
				break;
			}
		#endif	
//			UARTCtrl->BpsCtrl=0xb+(6<<5);//9600,8e1
//			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
//			p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
//			p16fifo[0]=0;//中断接收指针
//			p16fifo[1]=0;//当前已读指针
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
			}
			break;
		case 5://5=正在接收
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
				return;
			}
			if(p0!=p1)
			{//有数据接收,重置接收超时定时
				p16timer[0]=500/10;//字节间超时定时500MS
			}
			else
			{
				if(p16timer[0]==0x0)
				{//超时
					UARTCtrl->Task=2;//2=接收超时(或错误)
					UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
					return;
				}
			}
			RxByte=UARTCtrl->RxByte;
			p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
			p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
				p8rxbuff[RxByte]=i;
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
						if(RxByte>=(p8rxbuff[1]+(p8rxbuff[2]<<8)))
						{
							if(i==0x16)
							{//接收完成
								i=Uart_3762_CS((u32)p8rxbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
								if(i!=0)
								{//和校验错误
									RxByte=0;
									continue;
								}
								//和校验正确
								UARTCtrl->Task=1;//1=完成1帧发收
								p16fifo[1]=p1;
								UARTCtrl->RxByte=RxByte;
								Uart_3762_RxData(PORTn);//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						#if (USER/100)==0//电科院测试
								p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
								p16timer[0]=200/10;//接收到发送延时
						#else
								p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
								p16timer[0]=50/10;//接收到发送延时
						#endif	
								return;
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
			break;
	}
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
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;

	PRM=1;//启动帧标志
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->TxToRxTimerOver=6000/10;//发送后接收超时定时值ms/10(半字对齐)
	switch(AFN)
	{
		case 0://确认/否认
			MC(0,((u32)p8txbuff)+4,5);//信息域R
			PRM=0;//启动帧标志
			UARTCtrl->TxToRxTimerOver=0;//发送后接收超时定时值=0,使能很快超时而转空闲以便数据转发
			break;
		case 1://初始化
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			break;
		case 2://转发数据
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			break;
	#if (USER/100)==14//福建
		case 0x53:
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			switch(Fn)
			{
				case 3://从节点侦听信息
					p8txbuff[4]|=0x5;//R-1的通信模块标识=1,路由标识=1
					break;
				case 4://主节点地址
					break;
				case 6://主节点干忧状态
					p8txbuff[4]|=0x1;//R-1的路由标识=1
					UARTCtrl->TxToRxTimerOver=(60000+10000)/10;//发送后接收超时定时值ms/10(半字对齐)
					break;
			}
			break;
	#else
		case 3://查询数据
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			switch(Fn)
			{
				case 3://从节点侦听信息
					p8txbuff[4]|=0x5;//R-1的通信模块标识=1,路由标识=1
					break;
				case 4://主节点地址
					break;
				case 6://主节点干忧状态
					p8txbuff[4]|=0x1;//R-1的路由标识=1
					UARTCtrl->TxToRxTimerOver=(60000+10000)/10;//发送后接收超时定时值ms/10(半字对齐)
					break;
			}
			break;
		#endif
		case 4://链路接口检测
//			MC(0,((u32)p8txbuff)+4,6);//信息域R
//			switch(Fn)
//			{
//				case 1://发送测试
//					p8=(u8 *)(MenuData_ADDR);
//					p16ctrl[15]=p8[0]+5;//发送后接收超时定时值S
//					break;
//				case 2://载波从节点名
//					p8txbuff[4]|=0x5;//R-1的通信模块标识=1,路由标识=1
//					p16ctrl[15]=10;//发送后接收超时定时值S
//					break;
//			}
//			break;
			return;
		case 5://控制命令
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			if(Fn==3)
			{//启动广播
				//真实超时用Terminal_Ram->RouterCheck_S_Timer//最大广播超时时间秒(半字对齐)
				UARTCtrl->TxToRxTimerOver=120000/10;//发送后接收超时定时值ms/10(半字对齐)
			}
			break;
		case 6://主动上报
			break;
		case 0x10://路由查询
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			break;
		case 0x11://路由设置
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			break;
		case 0x12://路由控制
			MC(0,((u32)p8txbuff)+4,6);//信息域R
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
					MC(0,((u32)p8txbuff)+4,5);//信息域R
					break;
			}
			PRM=0;//启动帧标志
			break;
		case 0x15://文件传输
			//F1文件传输方式1
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			break;
		case 0xF0://内部测试
			MC(0,((u32)p8txbuff)+4,6);//信息域R
			break;
		default:
			return;
	}
	A=0;
	if((p8txbuff[4]&0x4)!=0x0)
	{
		A=12;
	}
	A+=((p8txbuff[4]>>4)&0xf)*6;
	p8txbuff[10+A]=AFN;
	Fn--;
	p8txbuff[10+A+1]=1<<(Fn%8);
	p8txbuff[10+A+2]=Fn/8;
	MR(((u32)p8txbuff)+10+A+3,((u32)p8txbuff)+128,LEN_Data);
	L=10+A+3+LEN_Data+2;
	p8txbuff[0]=0x68;
	p8txbuff[1]=L;//总字节数
	p8txbuff[2]=L>>8;
	if(PRM==0)//启动帧标志
	{
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
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8txbuff[3]=0x43;//控制码;集中式路由载波通信
		}
		else
		{
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{
				p8txbuff[3]=0x4a;//控制码;微功率无线通信
			}
			else
			{
				p8txbuff[3]=0x41;//控制码;集中式路由载波通信
			}
		}
		UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
		p8txbuff[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	}
	Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
	UARTCtrl->TxByte=L;//发送字节数(半字对齐)
	UARTCtrl->Task=3;//等待发送
}

void Uart_3762_AFN13F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag)//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
{
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	p8txbuff[0]=0x68;
	p8txbuff[1]=(29+LEN_Data+2);//需总发送字节数
	p8txbuff[2]=(29+LEN_Data+2)>>8;
	if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{
		p8txbuff[3]=0x43;//控制码;集中式路由载波通信
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8txbuff[3]=0x4a;//控制码;微功率无线通信
		}
		else
		{
			p8txbuff[3]=0x41;//控制码;集中式路由载波通信
		}
	}
	p8txbuff[4]=4;//通信模块标识=1
	p8txbuff[5]=0;
	p8txbuff[6]=LEN_ESC;//预计应答字节数
	p8txbuff[7]=0;
	p8txbuff[8]=0;
	UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	p8txbuff[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
	MR(((u32)p8txbuff)+16,ADDR_Data+1,6);//从节点地址(目的地址)
	p8txbuff[22]=0x13;//AFN=13H
	p8txbuff[23]=1;//Fn=1
	p8txbuff[24]=0;
	p8txbuff[25]=Protocol;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
	p8txbuff[26]=DelayFlag;//通信延时相关标志
	p8txbuff[27]=0;//附属节点数=0
	p8txbuff[28]=LEN_Data;//DL645报文长度
	MR(((u32)p8txbuff)+29,ADDR_Data,LEN_Data);//标准645帧

	Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
}

u32 Uart_3762_AFN13F1_Rx(u32 PORTn)//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
{
	u32 i;
	u32 x;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	x=0;//正确
	i=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(i<=16)
	{//错误
		return 4;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	if(UARTCtrl->Task!=1)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{//超时
		return 5;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	i=MRR(((u32)p8rxbuff)+22,3);
	if(i!=0x113)
	{//AFN=13H,Fn=1
		return 255;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	i=p8rxbuff[28];//返回数据域长度
	if(i==0)
	{//接收数据长度=0
		return 255;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
	}
	if(x==0)
	{
		MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,i);
		//通信地址比较
		for(i=0;i<6;i++)
		{
			if(p8rxbuff[1+i]!=p8txbuff[29+1+i])
			{
				break;
			}
		}
		if(i!=6)
		{
			return 1;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
		}
		//控制码
		if((p8rxbuff[8]&0x40)!=0x0)
		{//从站异常回答
			return 2;//返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
		}
		//减0x33
		for(i=0;i<p8rxbuff[9];i++)
		{
			p8rxbuff[10+i]-=0x33;
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
	u8 * p8rxbuff;
	
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	AFN=p8rxbuff[10];
	Fn=BitLoca(p8rxbuff[11]);//计算第1个置位为1的位置(0-32)
	Fn+=(p8rxbuff[12]*8);
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


void Uart_3762_RxData(u32 PORTn)//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 AFN;
	u32 Fn;
//	u64 CollectorAddr;
	u32 Protocol;
//	u64 MeterAddr;
//	u32 Phase;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	//Phase=p8rxbuff[6]&0x0f;//电表相位
	if((p8rxbuff[4]&0x4)!=0)
	{//通信模块标识：0表示对集中器的通信模块操作,1表示对载波表的通信模块操作
		p8rxbuff+=12;
	}
	AFN=p8rxbuff[10];
	Fn=BitLoca(p8rxbuff[11]);//计算第1个置位为1的位置(0-32)
	Fn+=(p8rxbuff[12]*8);
	
	switch(AFN)
	{
		case 6://AFN=06H,主动上报
			switch(Fn)
			{
//F1上报从节点信息
				case 1://F1上报从节点信息
					n=p8rxbuff[13];//本次上报从节点的数量n
					for(i=0;i<n;i++)
					{
						//MeterAddr=MRR(((u32)p8rxbuff)+14+(i*9),6);
						Protocol=p8rxbuff[14+6+(i*9)];
						if(Protocol!=1)
						{
							Protocol=30;
						}
						//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,0x0,0,Phase);//自动设置AFN04F10的电能表配置
					}
	#if SoftBug4==0//0=正常功能,1=电科院送检(鼎信东软模块自动搜表为送检模式)
		//正常功能
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
	#else
		//送检模式
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
	#endif
					break;
//F2上报抄读数据
				case 2://F2上报抄读数据
					return;
//F3上报路由工况变动信息
				case 3://F3上报路由工况变动信息
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
					switch(p8rxbuff[13])
					{
						case 1://1为抄表任务结束
							break;
						case 2://2为搜表任务结束
							if(UARTCtrl->AutoSearchTask==6)//自动搜表任务:0=空,1=开始,2...过程
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
							n=p8rxbuff[13];//本次上报从节点的数量n
							//CollectorAddr=MRR(((u32)p8rxbuff)+13+1,6);
							Protocol=p8rxbuff[13+1+6];
							if(Protocol!=1)
							{
								Protocol=30;
							}
							//Driver=p8rxbuff[14+9];//设备类型：01:081 采集器，00:082 采集器，02：开箱监视器；03H—FFH 保留
							p8rxbuff+=13+11;
							for(i=0;i<n;i++)
							{
								m=p8rxbuff[1];//本次上报的从节点1下接的从节点数量n1
								if(m==0)
								{//只上报采集器,无下属485表
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
									p8rxbuff+=2;
									continue;
								}
								for(x=0;x<m;x++)
								{//下属485表
									//MeterAddr=MRR(((u32)p8rxbuff)+2+(7*x),6);
									Protocol=p8rxbuff[2+(7*x)+6];
									if(Protocol!=1)
									{
										Protocol=30;
									}
									//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
								}
								p8rxbuff+=2+(7*x);
							}
			#if SoftBug4==0//0=正常功能,1=电科院送检(鼎信模块自动搜表为送检模式)
				//正常功能
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
			#else
				//送检模式
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
			#endif
							break;
						case 3://东软
						case 5://瑞斯康
							//CollectorAddr=MRR(((u32)p8rxbuff)+13+1,6);
							Protocol=p8rxbuff[13+1+6];
							if(Protocol!=1)
							{//通信协议类型：00H＝透明传输；01H＝DL/T645-1997；02H＝DL/T645-2007；03H～FFH保留
								Protocol=30;
							}
							i=p8rxbuff[14+9];//c)	设备类型：00H＝采集器；01H＝电能表；02H-FFH 保留
							if(i==1)
							{//上报电能表
								//AutoSetAFN04F10(PORTn,CollectorAddr,Protocol,0x0,0,Phase);//自动设置AFN04F10的电能表配置
							}
							else
							{//上报采集器
								p8rxbuff+=13+11;
								m=p8rxbuff[1];//本次上报的从节点1下接的从节点数量n1
								if(m==0)
								{//只上报采集器,无下属485表
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
								}
								else
								{//下属485表
									for(x=0;x<m;x++)
									{
										//MeterAddr=MRR(((u32)p8rxbuff)+2,6);
										Protocol=p8rxbuff[2+6];
										if(Protocol!=1)
										{//通信协议类型：00H＝透明传输；01H＝DL/T645-1997；02H＝DL/T645-2007；03H～FFH保留
											Protocol=30;
										}
										//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//自动设置AFN04F10的电能表配置
										p8rxbuff+=6+1;
									}
								}
							}
			#if SoftBug4==0//0=正常功能,1=电科院送检(鼎信模块自动搜表为送检模式)
				//正常功能
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
			#else
				//送检模式
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
			#endif
							break;
					}
					break;
//F5上报从节点事件
				case 5://F5上报从节点事件
				#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
				#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
					//查前导0xfe个数
					i=0;
					while(p8rxbuff[i+13+3]==0xfe)
					{
						i++;
					}
					for(x=0;x<p8rxbuff[i+13+3+9];x++)
					{
						p8rxbuff[i+13+3+10+x]-=0x33;
					}
					x=MRR(((u32)p8rxbuff)+i+13+3+10,4);
					if(x==0x04001501)
					{
						//WriteAutoReportWord(PORTn,((u32)p8rxbuff)+i+13+3);//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
						//xxx
						if((UARTCtrl->RMFlag==0x0)||(Terminal_Router->RMTD==0))//0=抄表间隔暂停,或,0=非抄表时段
//						if(UARTCtrl->RMFlag==0x0)//0=抄表间隔暂停
						{//0=抄表间隔暂停
							UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=周期抄读电能表事件
							UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
							Terminal_Router->StopFlags|=(1<<17);//暂停路由;b17=抄主动上报事件;也做为间隔暂停时进入主动上报事件抄读标志
						}
					}
			#if(USER/100)==7//浙江用户
					if(p8rxbuff[i+13+3+10]==0xC3)
					{//读取采集器内电能表事件状态（标识C3H）
						//写电能表主动上报状态字;
						x=p8rxbuff[i+13+3+12];//本次上报有事件的电能表数量1字节（HEX格式）
						for(n=0;n<x;n++)
						{
							if(p8rxbuff[i+13+3+13+8*n+6]==1)
							{//1字节规约（1:97,2:07）
								m=Check_AFN04F10_Addr(31,p8rxbuff[i+13+3+13+8*n]);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
								if(m!=0)
								{//找到
									m--;
									if(p8rxbuff[i+13+3+13+8*n+7]&0x01)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord),1);
									}
									if(p8rxbuff[i+13+3+13+8*n+7]&0x02)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+1,1);
									}
									if(p8rxbuff[i+13+3+13+8*n+7]&0x04)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+2,1);
									}
									if(p8rxbuff[i+13+3+13+8*n+7]&0x08)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+3,1);
									}
									MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+96,1);
								}
							}	
						}
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
						if(UARTCtrl->RMFlag==0x0)//0=抄表间隔暂停,1=抄表进行中
						{//0=抄表间隔暂停
							UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=周期抄读电能表事件
							UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
							Terminal_Router->StopFlags|=(1<<17);//暂停路由;b17=抄主动上报事件;也做为间隔暂停时进入主动上报事件抄读标志
						}
					}
			#endif
				#endif
				#endif
					break;
			}
	  	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MR((u32)p8txbuff,(u32)p8rxbuff,10);//复制10字节接收头到发送头
			MWR(0xffffffff,((u32)p8txbuff)+128,6);
			Uart_3762_TxData(PORTn,0,1,6);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			break;
	}	
	
}
#endif







