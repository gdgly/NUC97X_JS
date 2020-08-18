
//级联处理
#include "Project.h"
#include "Terminal_Uart_Cascading.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




void Cascading(u32 PORTn)//级联处理过程
{
/*
	u32 i;
	u32 RS485PORT;
	u16 *p16;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8RS485txbuff;
	u8 * p8fifo;
	u16 * p16fifo;
	u16 * p16timer;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnTx;
	u32 p0;
	u32 p1;
	u32 TxByte;
	u32 cs;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	switch(MEMReadReg(ADDR_AFN04F37,1))//终端级联通信参数
	{
		case 2://RS485-1
			RS485PORT=RS485_1PORT;
			break;
		case 3://RS485-2
			RS485PORT=RS485_2PORT;
			break;
		case 4://RS485-3
			RS485PORT=RS485_3PORT;
			break;
		default:
			UARTCtrl->Task=0;
			return;
	}
  UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485PORT);
  p8RS485txbuff=(u8 *)Get_ADDR_UARTnTx(RS485PORT);
	switch(UARTCtrl->Task)
	{
		case 20://20=级联接收完成
			if(UARTCtrlRS485->Lock==0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
			{
				break;
			}
			if(UARTCtrlRS485->Task!=0)//主动级联巡测任务:0=空,1=启动发送,2=正在发送,3=接收返回,100=等待上传
			{
				if(UARTCtrlRS485->Task==100)
				{
					break;
				}
			}
			UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);//用户数据区长度
			i+=8;
			MEMWrite((u32)p8rxbuff,(u32)p8RS485txbuff,i);
			p8RS485txbuff[1]=p8RS485txbuff[3];//长度字节恢复
			p8RS485txbuff[2]=p8RS485txbuff[4];
			UARTCtrlRS485->TxByte=i;//需发送字节数
			UARTCtrl->Task=21;
			break;
		case 21://21=启动转发级联数据
			Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 级联485发送LED指示MS定时器,每10MS减1定时器
			UARTCtrlRS485->BpsCtrl=MEMReadReg(ADDR_AFN04F37+1,1);
			Init_UARTn(RS485PORT);//初始化UART口,入口通信控制字已填入
			p16=(u16 *)Get_ADDR_UARTn(RS485PORT);//得到UART缓冲首地址
			p16[0]=0;//FIFO指针
			p16[1]=0;//FIFO已读指针
			p16[2]=0;//检查指针==读指针,免检省时
			Start_UARTn_Tx(RS485PORT);//启动UART口发送
			UARTCtrl->Task=22;
			break;
		case 22://22=正在转发级联数据
			if(Wait_UARTn_Tx(RS485PORT)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
//				i=MEMReadReg(ADDR_AFN04F37+2,1);//接收等待报文超时时间 BIN 100ms 1 
//				if(i==0x0)
//				{
//					i=1;
//				}
//				i*=(100/10);
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485PORT);
				p16timer[0]=2000/10;//接收超时时间
				UARTCtrl->TxByte=0;//接收字节数=0
				UARTCtrl->Task=23;//23=接收级联数据
			}
			break;
		case 23://23=接收级联数据(接收在原fifo缓冲中不放在rxbuff,免和主动级联巡测任务接收数据冲突!)
			if((p8rxbuff[12]==0x0))
			{//应用层功能码AFN=0确认/否认,无返回数据
				UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
				UARTCtrl->Task=0;
				return;
			}
			
			p8fifo=(u8 *)(Get_ADDR_UARTn(RS485PORT)+6);
			p16fifo=(u16 *)Get_ADDR_UARTn(RS485PORT);
			p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485PORT);
			LEN_UARTnIntRx=Get_LEN_UARTnIntRx(RS485PORT);
			LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
			
			p0=p16fifo[0];//中断接收指针
			p1=p16fifo[1];//当前已读指针
			if((p0>=(LEN_UARTnIntRx-6))||(p1>=(LEN_UARTnIntRx-6)))
			{
				p16fifo[0]=0;//中断接收指针
				p16fifo[1]=0;//当前已读指针
				p16fifo[2]=0;//规约检查指针
				return;
			}
			TxByte=UARTCtrl->TxByte;
			if(p0!=p1)
			{//有数据接收,重置接收超时定时
				p16timer[0]=100/10;//字节超时定时
			}
			while(p0!=p1)
			{
				if(TxByte>=LEN_UARTnTx)
				{
					TxByte=0;
				}
				i=p8fifo[p1];
				p1++;
				if(p1>=(LEN_UARTnIntRx-6))
				{
					p1=0;
				}
				p8txbuff[TxByte]=i;
				TxByte++;
				
				if(TxByte==1)
				{
					if(i!=0x68)
					{
						TxByte=0;
						continue;
					}
				}
				else
				{
					if(TxByte==6)
					{
						if(i!=0x68)
						{
							TxByte=0;
							continue;
						}
						else
						{//2个长度比较
							i=(p8txbuff[1]+(p8txbuff[2]<<8));
							if(i!=(p8txbuff[3]+(p8txbuff[4]<<8)))
							{
								TxByte=0;
								continue;
							}
							else
							{
								i>>=2;//去标志位D1D0,同时长度最大只有0-16383
								if(i>(LEN_UARTnTx-8))
								{
									TxByte=0;
									continue;
								}
								
							}
						}
					}
					else
					{
						if(TxByte>6)
						{
							if(TxByte>=(((p8txbuff[1]+(p8txbuff[2]<<8))>>2)+8))
							{
								if(i==0x16)
								{//接收完成
									//和校验计算
									cs=0;
									for(i=0;i<((p8txbuff[1]+(p8txbuff[2]<<8))>>2);i++)
									{
										cs+=p8txbuff[6+i];
									}
									cs&=0xff;
							
									if(cs!=p8txbuff[6+i])
									{//和校验错误
										TxByte=0;
										continue;
									}
									//和校验正确
									p16fifo[1]=p1;//当前已读指针
									p16fifo[2]=p1;//检查指针==读指针,免检省时
									UARTCtrl->TxByte=TxByte;
									Terminal_Ram->CD485RxLED_MS_Timer=100/10;//级联485接收LED指示MS定时器,每10MS减1定时器
									UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrl->Task=2;//2=等待单帧数据发关送
									return;
								}
								else
								{//结尾错
									TxByte=0;
									continue;
								}
							}
						
						}
					}
				}
			}
			p16fifo[1]=p1;
			UARTCtrl->TxByte=TxByte;
			if(p16timer[0]==0x0)
			{//接收超时
				i=UARTCtrl->CascadingReTxCount;//级联重发计数
				if(i<MEMReadReg(ADDR_AFN04F37+4,1))//级联方（主动站）接收失败重发次数 BIN 次 1 
				{
					i++;
					UARTCtrl->CascadingReTxCount=i;//级联重发计数
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);//用户数据区长度
					i+=8;
					UARTCtrlRS485->TxByte=i;//需发送字节数
					UARTCtrl->Task=21;//21=启动转发级联数据
				}
				else
				{
					UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
					UARTCtrl->Task=0;
				}
			}
			break;
	}
*/
}

const u8 Cascading_LoopChack_Example[]=
{
	0x68,
	12,0,
	12,0,
	0x68,
	0x4B,//控制码(DIR=0下行,启动标志位PRM=1,功能码11=请求2级数据
	0,0,//行政区划码
	0,0,//终端地址
	0x2,//启动站地址=1
	8,//AFN=8
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON暂=0
	0,0,//Pn=0
	0,0,//Fn=0
	//cs
	//0x16
};

void Cascading_LoopChack(u32 PORTn)//主动级联巡测
{
/*
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	
	if((Comm_Ram->SelfTestCommand==0x55555555)||(Comm_Ram->SelfTestCommand==0xAAAAAAAA))
	{//自测试命令
		return;
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
	{//登入
		goto LoopChack;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
	{//登入
		goto LoopChack;
	}
	return;
	
LoopChack:
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->Task)//主动级联巡测任务:0=空,1=启动发送,2=正在发送,3=接收返回,100=等待上传
	{
		case 0://空
			if(Terminal_Ram->Cascading_LoopChack_S_Timer==0x0)//37 级联巡测周期秒定时器
			{//级联巡测周期秒定时器=0
				i=MEMReadReg(ADDR_AFN04F37+6,1);
				i&=0x3;//级联的终端个数
				if(Terminal_Ram->Cascading_LoopChack_Count>=i)
				{//巡测终端计数>=级联的终端个数
					Terminal_Ram->Cascading_LoopChack_Count=0;
					Terminal_Ram->Cascading_LoopChack_S_Timer=MEMReadReg(ADDR_AFN04F37+5,1)*60;//下巡测周期定时
				}
				else
				{
					UARTCtrl->Task++;
				}
			}
			break;
		case 1://启动发送数据
			Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 级联485发送LED指示MS定时器,每10MS减1定时器
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MEMRead((u32)p8txbuff,(u32)Cascading_LoopChack_Example,20);
			MEMRead(((u32)p8txbuff)+7,ADDR_AFN04F37+7+(4*Terminal_Ram->Cascading_LoopChack_Count),4);
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			UARTCtrl->TxByte=20;//需发送字节数
			UARTCtrl->BpsCtrl=MEMReadReg(ADDR_AFN04F37+1,1);
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task++;
			break;
		case 2://正在发送数据
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
//					i=MEMReadReg(ADDR_AFN04F37+2,1);//接收等待报文超时时间 BIN 100ms 1 
//					if(i==0x0)
//					{
//						i=1;
//					}
//					i*=100/10;
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=2000/10;//接收超时时间
				UARTCtrl->RxByte=0;//接收字节数
				UARTCtrl->Task++;//接收返回数据
			}
			break;
		case 3://接收返回数据
			i=Uart_3761_Rx(PORTn);//UART_3761接收;返回0没完成,1完成,2超时
			switch(i)
			{
				case 0://没完成
					break;
				case 1://完成
					Terminal_Ram->CD485RxLED_MS_Timer=100/10;//级联485接收LED指示MS定时器,每10MS减1定时器
					p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
					if((p8rxbuff[12])!=0x0)//应用层功能码AFN
					{//巡测不是否认
						UARTCtrl->Task=100;//100=等待数据上传
					}
					else
					{
						Terminal_Ram->Cascading_LoopChack_Count++;//级联周期巡测终端计数+1
						UARTCtrl->Task=0;
					}
					break;
				default://超时
					Terminal_Ram->Cascading_LoopChack_Count++;//级联周期巡测终端计数+1
					UARTCtrl->Task=0;
					break;
			}
			break;
		case 100://等待数据上传
			break;
		default:
			UARTCtrl->Task=0;
			break;
	}
}

void AutoCascadingData(u32 PORTn)//自动上传级联巡测到的数据
{
	u32 i;
	u32 RS485PORT;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	i=MEMReadReg(ADDR_AFN04F37,1);//终端级联通信参数
	if(i!=0x0)
	{//有级联口
		if((MEMReadReg(ADDR_AFN04F37+6,1)&0x80)==0x0)
		{//是主动级联口
			if(UARTCtrl->Task==0x0)
			{//接收
				if(p16timer[0]==0x0)
				{
					if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
					{//登入
						switch(i)
						{
							case 2:
								RS485PORT=RS485_1PORT;
								break;
							case 3:
								RS485PORT=RS485_2PORT;
								break;
							case 4:
								RS485PORT=RS485_3PORT;
								break;
							default:
								return;
						}
						UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485PORT);
						if(UARTCtrlRS485->Task==100)
						{//100=等待数据上传
							UARTCtrlRS485->Task=0;//上传后清0
							i=UARTCtrlRS485->RxByte;//RX计数(已接收字节数)
							MEMRead(Get_ADDR_UARTnTx(PORTn),Get_ADDR_UARTnRx(RS485PORT),i);
							UARTCtrl->Task=2;//2=等待单帧数据发送
							UARTCtrl->TxByte=i;//需发送字节数
						}
					}
				}
			}
		}
	}
*/
}




