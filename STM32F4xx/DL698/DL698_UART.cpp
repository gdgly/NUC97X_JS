
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_LINK.h"
#include "../DL698/DL698_CONNECT.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_SET.h"
#include "../DL698/DL698_ACTION.h"
#include "../DL698/DL698_REPORT.h"
#include "../DL698/DL698_PROXY.h"
#include "../DL698/DL698_SECURITY.h"
#include "../DL698/DL698_REPORT.h"
#include "../MS/MS.h"
#include "../TEST/LOG.h"



__align(4) const u16 fcstab[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

u32 Uart_698_HCS(u8 *p8)//帧头校验计算;返回0=正确，1=错误
{
	u32 i;
	u32 len;
	u32 HCS;
	
	p8++;
	len=p8[3]&0x0f;
	len+=6;
	HCS=0xffff;
	for(i=0;i<len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	p8[len] = HCS;//least significant byte first
	p8[len+1] = (HCS >> 8);
	if(i==HCS)
	{
		return 0;
	}
	return 1;
}
#include "../DL698/DL698_JSON.h"

u32 Uart_698_HCSFCS(u8 *p8)//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
{
	u32 i;
	u32 len;
	u32 FCS;
	u32 Err;
	
	Err=Uart_698_HCS(p8);//帧头校验计算;返回0=正确，1=错误
	p8++;
	len=p8[0]+(p8[1]<<8);
	if(len<2)
	{
		return 1;
	}
	len-=2;
	FCS=0xffff;
	for(i=0;i<len;i++)
	{
		FCS=((FCS >> 8)&0xff) ^ fcstab[(FCS ^ p8[i]) & 0xff];
	}
	FCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	p8[len] = FCS;//least significant byte first
	p8[len+1] = (FCS >> 8);
	p8[len+2] = 0x16;
	if(i!=FCS)
	{
		Err=1;
	}
	
	return Err;
//#if DEBUG==0//0=发行,1=调试
//	return Err;
//#else
//	Err=Err;
//	return 0;
//#endif
}


u32 FrameProtocol_698_HCS(u8 *p8)//通信协议自动识别帧头校验计算;返回0=正确，1=错误
{
	u32 i;
	u32 len;
	u32 HCS;
	
	p8++;
	len=p8[3]&0x0f;
	len+=6;
	HCS=0xffff;
	for(i=0;i<len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	if(i==HCS)
	{
		return 0;
	}
	return 1;
}

u32 FrameProtocol_698_HCSFCS(u8 *p8)//通信协议自动识别整帧校验计算(HCS和FCS);返回0=正确，1=错误
{
	u32 i;
	u32 len;
	u32 FCS;
	u32 Err;
	
	Err=FrameProtocol_698_HCS(p8);//帧头校验计算;返回0=正确，1=错误
	p8++;
	len=p8[0]+(p8[1]<<8);
	if(len<2)
	{
		return 1;
	}
	len-=2;
	FCS=0xffff;
	for(i=0;i<len;i++)
	{
		FCS=((FCS >> 8)&0xff) ^ fcstab[(FCS ^ p8[i]) & 0xff];
	}
	FCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	if(i!=FCS)
	{
		Err=1;
	}
	return Err;
}

u32 FrameProtocol(u8* p8,u32 Len)//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
{
	u32 i;
	u32 x;
	u32 cs;
	
//去开始的0xfe
	for(i=0;i<Len;i++)
	{
		if(p8[i]!=0xfe)
		{
			break;
		}
	}
	p8+=i;
	Len-=i;
	if(Len==0)
	{
		return 0;
	}
//DL698
	if(p8[0]==0x68)
	{
		i=p8[1]|(p8[2]<<8);
		if(p8[1+i]==0x16)
		{
			if((i+2)==Len)
			{
				i=FrameProtocol_698_HCSFCS(p8);//通信协议自动识别整帧校验计算(HCS和FCS);返回0=正确，1=错误
				if(i==0)
				{
					return 3;//3=DL698
				}
			}
		}
	}
//DL645
	if(p8[0]==0x68)
	{
		if(p8[7]==0x68)
		{
			if((p8[9]+12)==Len)
			{
				cs=0;
				for(i=0;i<(p8[9]+10);i++)
				{
					cs+=p8[i];
				}
				cs&=0xff;
				if(cs==p8[i])
				{//和校验正确
					if(p8[i+1]==0x16)
					{
						return 2;//1=DL/T645-1997,2=DL/T645-2007
					}
				}
			}
		}
	}
//DL376-2
	if(p8[0]==0x68)
	{
		x=(p8[1]+(p8[2]<<8));
		if(x==Len)
		{
			if(p8[x-1]==0x16)
			{
				if(x>=5)
				{
					x-=5;
					cs=0;
					for(i=0;i<x;i++)
					{
						cs+=p8[3+i];
					}
					cs&=0xff;
					if(cs==p8[3+i])
					{
						return 128;//128=DL376.2
					}
				}
			}
		}
	}

	return 0;//未知
}


u32 Uart_698_Rx(u32 PORTn)//UART_698接收;返回0没完成,1完成,2超时
{
	u32 i;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rx;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;

	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u32 LEN_SA;//服务器地址SA的长度
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;//返回0没完成,1完成,2超时
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收,重置接收超时定时
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
		p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
		LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
		//被动接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
		if(PORTn==ENETPORT)
		{
			p16timer[0]=5000/10;//以太网模块超时定时5S
		}
		else
		{
			if(PORTn==GPRSPORT)
			{
			#if (USER/100)==0//电科院测试
				p16timer[0]=5000/10;//无线通信模块超时定时5S
			#else
				p16timer[0]=10000/10;//无线通信模块超时定时10S
			#endif
			}
			else
			{
				if(PORTn==SMSPORT)
				{
					p16timer[0]=60000/10;//短信通道超时定时60S
				}
				else
				{
					p16timer[0]=2000/10;//其他通道1S
				}
			}
		}
	}
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
			LEN_SA=p8rx[4];//取地址字节数
			LEN_SA&=0x0f;
			LEN_SA++;
			if(RxByte==(5+LEN_SA+3))
			{
				i=Uart_698_HCS(p8rx);//帧头校验计算;返回0=正确，1=错误
				if(i)
				{//校验错误
					RxByte-=1;
					while(1)
					{
						if(p1>=RxByte)
						{
							p1-=RxByte;
							break;
						}
						p1+=LEN_UARTnIntRx-10;
					}								
					RxByte=0;
					continue;
				}
			}
			if(RxByte>13)
			{
				if(RxByte>=(p8rx[1]+(p8rx[2]<<8)+2))
				{
					if(i==0x16)
					{//接收完成
						//校验计算
						i=Uart_698_HCSFCS(p8rx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
						if(i)
						{//校验错误
							RxByte-=1;
							while(1)
							{
								if(p1>=RxByte)
								{
									p1-=RxByte;
									break;
								}
								p1+=LEN_UARTnIntRx-10;
							}								
							RxByte=0;
							continue;
						}
						//和校验正确
						p16fifo[1]=p1;//当前已读指针
						p16fifo[2]=p1;//检查指针==读指针,免检省时
						p16fifo[3]=p1;//检查指针==读指针,免检省时
						p16fifo[4]=p1;//检查指针==读指针,免检省时
						UARTCtrl->RxByte=RxByte;
						return 1;//返回0没完成,1完成,2超时
					}
					else
					{//结尾错
						RxByte-=1;
						while(1)
						{
							if(p1>=RxByte)
							{
								p1-=RxByte;
								break;
							}
							p1+=LEN_UARTnIntRx-10;
						}								
						RxByte=0;
						continue;
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		UARTCtrl->RxByte=0;//已接收字节数=0
		return 2;//返回0没完成,1完成,2超时
	}
	return 0;//返回0没完成,1完成,2超时
}


#include "../DL698/DL698_JSON.h"


void Uart_698_RxTx(u32 PORTn)//UART_698接收发送
{
	u32 i;
	u32 x;
	u16* p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	ms_Type* ms;
	u32 LEN_UARTnTx;
	u32 LEN_ADDR;
	u32 LEN_TAPDU;
	u32 LEN_TxAPDU;
	u32 LEN_Tx;
	u8* p8tx;
	
	u8 * p8rx;
	u8 * p8data;
	u32 y;

	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器

	if(p16timer[0]==0)
	{//释放我的内存堆分配秒定时
		if(UARTCtrl->pTxData)
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
	}
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)
	{
		case 0://0=正在接收
			if(UARTCtrl->Lock==0x55)
			{
				return;//0x55=通信功能正在被调用禁止其他程序再次调用
			}
			//组地址和广播地址时级连转发结束
/*
			i=UARTCtrl->CascadingBroadcast;//0=空,级联组或广播地址转发的端口号(RS485_1PORT,RS485_2PORT,RS485_3PORT)
			if(i!=0)
			{
				UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
				if(UARTCtrlRS485->Lock==0x55)
				{
					if(UARTCtrlRS485->TxByte==0)//发送字节数(半字对齐)
					{
						UARTCtrl->CascadingBroadcast=0;//0=空,级联组或广播地址转发的端口号(RS485_1PORT,RS485_2PORT,RS485_3PORT)
						UARTCtrlRS485->Lock=0;
					}
				}
			}
*/
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{
				if(UARTCtrl->ReNewLink==0x55)//0x55=通信参数变化当前帧发送完成后重新连接
				{
					UARTCtrl->ReNewLink=0;//0x55=通信参数变化当前帧发送完成后重新连接
					UARTCtrl->LinkTask=0;
					p16timer[0]=0;//延时关机
					return;
				}
			}
			//接收
			i=Uart_698_Rx(PORTn);//UART_698接收;返回0没完成,1完成,2超时
			if(i==1)
			{
				UARTCtrl->Task=1;
			}
			if(UARTCtrl->RxByte==0)
			{
				EVENT_REPORT_Notification(PORTn);//上报通知(事件)
				REPORT_Notification(PORTn);//上报通知(定时任务)
			}
			break;
		case 1://1=接收完成
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			if(p8rx[3]&(1<<5))//分帧标志位
			{
				if(p16timer[0]==0x0)
				{
					LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					p8data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
					LEN_ADDR=p8rx[4];//取地址字节数
					LEN_ADDR&=0x0f;
					LEN_ADDR++;
					LEN_ADDR+=2;
					LEN_TAPDU=11+LEN_ADDR;//除起始字符和结束字符之外的帧字节数
					LEN_TAPDU-=3+LEN_ADDR+2+2;//长度域L和控制域C=3byte,帧头校验=2byte,帧校验=2byte
					MR((u32)p8tx,(u32)p8rx,4+LEN_ADDR+2);
					p8tx[3]|=p8rx[3]|0x80;//分帧标志位
					LEN_Tx=4+LEN_ADDR+2;

					x=p8rx[LEN_Tx];//链路层分帧序号计数0-4095

					p8tx[LEN_Tx]=p8rx[LEN_Tx];
					p8tx[LEN_Tx+1]=(p8rx[LEN_Tx+1]&0x0f)|0x80;
					if((p8rx[LEN_Tx+1]&0xf0)==0x00)
					{
						p8data[6*1024]=0;
						p8data[6*1024+1]=0;
					}
					LEN_Tx+=2+1;
					UARTCtrl->TxByte=LEN_Tx+2;
					x=(p8rx[2]<<8|p8rx[1])-3-LEN_ADDR-4-2;
					y=(p8data[6*1024]<<8|p8data[6*1024+1]);
					MR((u32)p8data+2+y+6*1024,(u32)p8rx+16,x);
					p8data[6*1024+1]=(x+y)&0xff;
					p8data[6*1024]=((x+y)>>8)&0xff;
					
					if((p8rx[LEN_Tx-2]&0xf0)==0x40)
					{
						MR((u32)p8rx+4+LEN_ADDR+2,(u32)p8data+6*1024+2,x+y);
						LEN_Tx=4+LEN_ADDR+2+x+y+2-1;
						p8rx[3]=0x43;
						p8rx[1]=LEN_Tx;
						p8rx[2]=LEN_Tx>>8;
						Uart_698_HCSFCS(p8rx);
						UARTCtrl->Task=8;
						return;
					}
					p8tx[1]=LEN_Tx;
					p8tx[2]=LEN_Tx>>8;
					Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
					
				#if LOG_68==1//0=没,1=有
					LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
				#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
					LOGRS232(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
					if(PORTn!=GPRSPORT)
					{//GPRS另自计
						UARTCtrl->RxTxByte+=UARTCtrl->TxByte;
					}
					UARTCtrl->Task=2;//正在发送标志
				}
				else
				{
					if(p16timer[0]>(20/10))
					{//数据转发,返回err时没重置定时值!
						p16timer[0]=20/10;
					}
				}
				break;
			}
			if(UARTCtrl->Lock==0x55)
			{
				return;//0x55=通信功能正在被调用禁止其他程序再次调用
			}
			UARTCtrl->Task=0;//0=接收
			UARTCtrl->AddrType=0;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
//			UARTCtrl->TxFrameCount=0;//寄存多帧数据时已发送帧计数
//			UARTCtrl->MulFrameGPCR=0;//多帧时通用计数寄存器
//			UARTCtrl->LockFlags&=(~(7<<4));//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7
			UARTCtrl->SubTask=0;//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
			//接收-发送延时
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS或短信通道
				p16timer[0]=0;//延时0MS发送
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//延时xMS发送(两帧之间的线路空闲间隔最少需33位)
			}
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{//通信模块
				//清心跳定时(正确收到任何帧,清心跳定时)
				if(PORTn==GPRSPORT)
				{
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
				}
				else
				{
					i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
				}
				if(i==0)
				{
					i=120;
				}
				else
				{
					i=MRR(i+1,2);
					i=(i>>8)+((i&0xff)<<8);
				}
				
				if((UARTCtrl->TCPUDP&0x30)==0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
				{//是服务器模式
					i+=60;//加60秒可能的主站心跳发出定时误差
				}
				if(PORTn==GPRSPORT)
				{
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS心跳秒定时器
					if(UARTCtrl->HostACK<=1)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					{
/*
						i=MRR(ADDR_AFN04F8+4,1);
						if(i==0x0)
						{
							i=1;//最小被动激活模式连续无通信自动断线时间1分
						}
						i*=60;
						Terminal_Ram->GPRSAutoDiscon_S_Timer=i;//GPRS被动激活模式连续无通信自动断线秒定时器
*/
					}
				}
				if(PORTn==ENETPORT)
				{
					Terminal_Ram->ENETHeartBeat_S_Timer=i;//以太网心跳秒定时器
				}
			#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				//清自动保电允许与主站连续无通信时间分定时器
				i=MRR(ADDR_8001_3+1,2);
				i=(i>>8)|((i&0xff)<<8);
				if(i==0)
				{//0表示不自动保电
					i=60;
				}
				Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 自动保电允许与主站连续无通信时间分定时器
			#endif
			#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				//清自动保电允许与主站连续无通信时间分定时器
				if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
				{
					i=MRR(ADDR_8001_3+1,2);
					i=(i>>8)|((i&0xff)<<8);
					if(i==0)
					{//0表示不自动保电
						i=60;
					}
					Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 自动保电允许与主站连续无通信时间分定时器
				}
			#endif
			}
			if(PowerEvent->task!=100)
			{
				Comm_Ram->BattWork_S_timer=60;//4 电池工作关机秒定时器
			}
		#if LOG_698==1//0=没,1=有
			LOG(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
		#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
			LOGRS232(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			if(PORTn!=GPRSPORT)
			{//GPRS另自计
				UARTCtrl->RxTxByte+=UARTCtrl->RxByte;
			}
//		#if (USER/100)==0//电科院测试
			Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
		#if((Project/100)==2)||((Project/100)==4)
			if(UARTCtrl->Lock_InfoTask==0x55)//读路由网络拓扑信息等任务:0x55=锁定
			{
				UARTCtrl->Task=1;
				break;
			}
		#endif
//		#else
//			i=MRR(ADDR_F101_2+1,1);//安全模式参数
//			if(i)
//			{
//				Uart_698_Data(PORTn,1);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
//			}
//			else
//			{
//				Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
//			}
//		#endif
			UARTCtrl->RxByte=0;//rx计数=0
			break;
		case 2://2=等待单帧数据发送
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT))
			{
				break;
			}
			if(p16timer[0]==0x0)
			{
//				LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
//				p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
//				LEN_ADDR=p8tx[4];//取地址字节数
//				LEN_ADDR&=0x0f;
//				LEN_ADDR++;
//				LEN_ADDR+=2;
//				LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//除起始字符和结束字符之外的帧字节数
//				LEN_TAPDU-=3+LEN_ADDR+2+2;
				LEN_TAPDU=0;//RS232,RS485链路层不分帧
				if(LEN_TAPDU<=LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
				{
				#if LOG_698==1//0=没,1=有
					LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
				#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
					LOGRS232(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
					if(PORTn!=GPRSPORT)
					{//GPRS另自计
						UARTCtrl->RxTxByte+=UARTCtrl->TxByte;
					}
					UARTCtrl->Task=3;//正在发送标志
					Start_UARTn_Tx(PORTn);//启动UART口发送
				}
				else
				{
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UARTCtrl->TxByte);
					UARTCtrl->TxAPDUSEQ=0;//链路层分帧序号计数0-4095
					UARTCtrl->TxAPDUByte=0;//链路层分帧已发送APDU字节数
					UARTCtrl->Task=5;//5=等待链路层分帧数据发送
				}
			}
			else
			{
				if(p16timer[0]>(200/10))
				{//数据转发,返回err时没重置定时值!
					p16timer[0]=200/10;
				}
			}
			break;
		case 3://3=单帧数据正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{			
				UARTCtrl->Task=0;//0=正在接收
				UARTCtrl->RxByte=0;//rx计数=0
				p16timer[0]=0;//超时定时=0;接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
			}
			break;
			
		case 4://4=多帧数据续接收完成
			//接收-发送延时
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS或短信通道
				p16timer[0]=0;//延时0MS发送
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//延时?MS发送(两帧之间的线路空闲间隔最少需33位)
			}
//		#if (USER/100)==0//电科院测试
			Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
		#if((Project/100)==2)||((Project/100)==4)
			if(UARTCtrl->Lock_InfoTask==0x55)//读路由网络拓扑信息等任务:0x55=锁定
			{
				UARTCtrl->Task=4;
				break;
			}
		#endif
//		#else
//			i=MRR(ADDR_F101_2+1,1);//安全模式参数
//			if(i)
//			{
//				Uart_698_Data(PORTn,1);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
//			}
//			else
//			{
//				Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
//			}
//		#endif
			UARTCtrl->RxByte=0;//rx计数=0
			break;
		case 5://5=等待链路层分帧数据发送
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT))
			{
				break;
			}
			if(p16timer[0]==0x0)
			{
				LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
				p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
				LEN_ADDR=p8tx[4];//取地址字节数
				LEN_ADDR&=0x0f;
				LEN_ADDR++;
				LEN_ADDR+=2;
				LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//除起始字符和结束字符之外的帧字节数
				LEN_TAPDU-=3+LEN_ADDR+2+2;//长度域L和控制域C=3byte,帧头校验=2byte,帧校验=2byte
				MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
				p8tx[3]|=(1<<5);//分帧标志位
				LEN_Tx=4+LEN_ADDR+2;
				i=UARTCtrl->TxAPDUByte;
				if(i>=LEN_TAPDU)
				{
					UARTCtrl->RxByte=0;//rx计数=0
					UARTCtrl->Task=0;//0=接收
					return;
				}
				LEN_TxAPDU=LEN_TAPDU-i;
			//分帧格式域定义
			//bit0~bit11：表示分帧传输过程的帧序号，取值范围0~4095，循环使用；
			//bit12~bit13：保留；
			//bit15=0，bit14=0：表示分帧传输数据起始帧；
			//bit15=1，bit14=0：表示分帧传输确认帧（确认帧不包含APDU片段域）；
			//bit15=0，bit14=1：表示分帧传输最后帧；
			//bit15=1，bit14=1：表示分帧传输中间帧。
				x=UARTCtrl->TxAPDUSEQ;//链路层分帧序号计数0-4095
				UARTCtrl->TxAPDUSEQ++;//链路层分帧序号计数0-4095
				x&=0x7ff;
				if(UARTCtrl->TxAPDUByte==0)
				{//第1帧
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{
					if(LEN_TxAPDU>LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
					{//中间帧
						x|=0xC000;
						LEN_TxAPDU=LENmax_TxAPDU;
					}
					else
					{//最后帧
						x|=0x4000;
					}
				}
				p8tx[LEN_Tx]=x;
				p8tx[LEN_Tx+1]=x>>8;
				LEN_Tx+=2;
				MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UARTCtrl->TxAPDUByte,LEN_TxAPDU);
				UARTCtrl->TxAPDUByte+=LEN_TxAPDU;
				LEN_Tx+=LEN_TxAPDU;
				LEN_Tx+=2+1;
				UARTCtrl->TxByte=LEN_Tx;
				LEN_Tx-=2;
				p8tx[1]=LEN_Tx;
				p8tx[2]=LEN_Tx>>8;
				Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
				
			#if LOG_68==1//0=没,1=有
				LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
				LOGRS232(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				if(PORTn!=GPRSPORT)
				{//GPRS另自计
					UARTCtrl->RxTxByte+=UARTCtrl->TxByte;
				}
				UARTCtrl->Task=6;//正在发送标志
				Start_UARTn_Tx(PORTn);//启动UART口发送
			}
			else
			{
				if(p16timer[0]>(20/10))
				{//数据转发,返回err时没重置定时值!
					p16timer[0]=20/10;
				}
			}
			break;
		case 6://6=链路层分帧正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->RxByte=0;//rx计数=0
				p16timer[0]=0;//超时定时=0;接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
				
				UARTCtrl->Task=0;
			}
			break;
		case 7://7=等待发送器空转为接收
			UARTCtrl->Task=0;//0=正在接收
			UARTCtrl->RxByte=0;//rx计数=0
			p16timer[0]=0;//超时定时=0;接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
			break;		
		case 8:
			if(UARTCtrl->Lock==0x55)
			{
				return;//0x55=通信功能正在被调用禁止其他程序再次调用
			}
			UARTCtrl->Task=0;//0=接收
			UARTCtrl->AddrType=0;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
//			UARTCtrl->TxFrameCount=0;//寄存多帧数据时已发送帧计数
//			UARTCtrl->MulFrameGPCR=0;//多帧时通用计数寄存器
//			UARTCtrl->LockFlags&=(~(7<<4));//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7
			UARTCtrl->SubTask=0;//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
			//接收-发送延时
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS或短信通道
				p16timer[0]=0;//延时0MS发送
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//延时xMS发送(两帧之间的线路空闲间隔最少需33位)
			}
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{//通信模块
				//清心跳定时(正确收到任何帧,清心跳定时)
				if(PORTn==GPRSPORT)
				{
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
				}
				else
				{
					i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
				}
				if(i==0)
				{
					i=120;
				}
				else
				{
					i=MRR(i+1,2);
					i=(i>>8)+((i&0xff)<<8);
				}
				
				if((UARTCtrl->TCPUDP&0x30)==0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
				{//是服务器模式
					i+=60;//加60秒可能的主站心跳发出定时误差
				}
				if(PORTn==GPRSPORT)
				{
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS心跳秒定时器
					if(UARTCtrl->HostACK<=1)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					{
/*
						i=MRR(ADDR_AFN04F8+4,1);
						if(i==0x0)
						{
							i=1;//最小被动激活模式连续无通信自动断线时间1分
						}
						i*=60;
						Terminal_Ram->GPRSAutoDiscon_S_Timer=i;//GPRS被动激活模式连续无通信自动断线秒定时器
*/
					}
				}
				if(PORTn==ENETPORT)
				{
					Terminal_Ram->ENETHeartBeat_S_Timer=i;//以太网心跳秒定时器
				}
			#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				//清自动保电允许与主站连续无通信时间分定时器
				i=MRR(ADDR_8001_3+1,2);
				i=(i>>8)|((i&0xff)<<8);
				if(i==0)
				{//0表示不自动保电
					i=60;
				}
				Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 自动保电允许与主站连续无通信时间分定时器
			#endif
			#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				//清自动保电允许与主站连续无通信时间分定时器
				if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
				{
					i=MRR(ADDR_8001_3+1,2);
					i=(i>>8)|((i&0xff)<<8);
					if(i==0)
					{//0表示不自动保电
						i=60;
					}
					Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 自动保电允许与主站连续无通信时间分定时器
				}
			#endif
			}
			if(PowerEvent->task!=100)
			{
				Comm_Ram->BattWork_S_timer=60;//4 电池工作关机秒定时器
			}
		#if LOG_698==1//0=没,1=有
			LOG(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
		#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
			LOGRS232(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			if(PORTn!=GPRSPORT)
			{//GPRS另自计
				UARTCtrl->RxTxByte+=UARTCtrl->RxByte;
			}
//		#if (USER/100)==0//电科院测试
			i=MRR(ADDR_F101_2+1,1);//安全模式参数
			if(i)
			{
				Uart_698_Data(PORTn,1);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
			}
			else
			{
				Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
			}
//			Uart_698_Data(PORTn,1);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
		#if((Project/100)==2)||((Project/100)==4)
			if(UARTCtrl->Lock_InfoTask==0x55)//读路由网络拓扑信息等任务:0x55=锁定
			{
				UARTCtrl->Task=1;
				break;
			}
		#endif
//		#else
//			i=MRR(ADDR_F101_2+1,1);//安全模式参数
//			if(i)
//			{
//				Uart_698_Data(PORTn,1);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
//			}
//			else
//			{
//				Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
//			}
//		#endif
			UARTCtrl->RxByte=0;//rx计数=0
			break;
		default:
			UARTCtrl->Task=0;//0=正在接收
			break;
	}
}

void Uart_698_Data(u32 PORTn,u32 SECURITY)//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
{
	u32 i;
	u32 x;
	u32 LEN_SA;
	u32 LEN_RxAPDU;
	u8 * p8rx;
  UARTCtrl_TypeDef * UARTCtrl;
	ms_Type *ms;
	u8* p8;
	u32 DataAdd33;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	if(p8rx[3]&0x80)//控制域C,bit7=传输方向位：DIR=0表示此帧是由客户机发出的,DIR=1表示此帧是由服务器发出的;bit6=启动标志位：PRM=1表示此帧是由客户机发起的；PRM=0表示此帧是由服务器发起的;bit5=分帧标志位
	{//非主站下行帧
		return;
	}
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	if(LEN_RxAPDU<(LEN_SA+8+1+2))
	{
		return;//长度错
	}
	i=p8rx[4];
//#if (USER/100)==0//电科院测试
	if((i&0x30)==0x30)
	{
		return;//bit4…bit5逻辑地址=3；电科院一致性测试时认为错，功能测试软件测试电压合格率时用逻辑地址1其他测试用逻辑地址0
	}
//#endif
	i>>=6;
	switch(i)//bit6…bit7：为地址域服务器地址的类别标志，采用编码方式，0表示单地址，1表示通配地址，2表示组地址，3表示广播地址
	{
		case 0://0表示单地址
		case 1://1表示通配地址
			if(PORTn!=ACSAMPLEPORT)
			{
				i=Uart_TSA_Comp(p8rx+4,(u8 *)ADDR_4001+1);//通信地址比较,pFLASH数据读到ADDR_128K_DDATABUFF中比较,返回:0=相同,!=0不相同
				if(i)
				{
					return;//地址不同
				}
				i=p8rx[4];
				i&=0x0f;
				i++;
				MR((u32)p8rx+4+1,ADDR_128KDATABUFF+1,i);
			}
			UARTCtrl->AddrType=1;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			break;
		case 2://2表示组地址
//68 xx 00 43 LEN_SA 01 02 03 10 FF FF 06 01 02 40 00 02 00 1c 07 e1 07 0d 0f 38 06 00 1c 43 16
			//剔除投入
			i=MRR(ADDR_4024+1,1);//属性2∷=enum {剔除投入（1），剔除解除（2）}
			if(i==1)
			{//剔除投入,只有对时命令继续有效,其他全部无效
				i=(p8rx[5+LEN_SA+3]<<8)|p8rx[5+LEN_SA+4];
				if(i!=0x0601)
				{//
					return;
				}
				i=(p8rx[5+LEN_SA+6]<<24)|(p8rx[5+LEN_SA+7]<<16)|(p8rx[5+LEN_SA+8]<<8)|p8rx[5+LEN_SA+9];
				if(i!=0x40000200)
				{//!对时命令
					return;
				}
			}
			MR(ADDR_DATABUFF,ADDR_4005,LENmax_4005);
			p8=(u8*)ADDR_DATABUFF;
			x=p8[1];//数组数
			p8+=2;
			for(i=0;i<x;i++)
			{
				LEN_RxAPDU=Uart_TSA_Comp(p8rx+4,p8+1);//通信地址比较,pFLASH数据读到ADDR_128K_DDATABUFF中比较,返回:0=相同,!=0不相同
				if(LEN_RxAPDU==0)
				{//相同
					break;
				}
				p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);
			}
			if(i>=x)
			{
				return;//地址不同
			}
			UARTCtrl->AddrType=2;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			break;
		case 3://3表示广播地址
			//只有广播校时和读通信地址时有效
//68 xx 00 43 00 AA 10 FF FF 05 01 00 40 01 02 00 00 XX XX 16
//68 xx 00 43 00 AA 10 FF FF 07 01 01 40 00 7F 00 1C yy yy mm dd hh mm ss 00 XX XX 16
//68 xx 00 43 00 AA 10 FF FF 06 01 02 40 00 02 00 1c 07 e1 07 0d 0f 38 06 00 1c 43 16
			i=(p8rx[9]<<8)|p8rx[10];
			switch(i)
			{
				case 0x0501://读通信地址
					i=(p8rx[12]<<24)|(p8rx[13]<<16)|(p8rx[14]<<8)|p8rx[15];
					if(i==0x40010200)
					{
						break;
					}
					return;
				default:
					//剔除投入
					i=MRR(ADDR_4024+1,1);//属性2∷=enum {剔除投入（1），剔除解除（2）}
					if(i==1)
					{//剔除投入,只有对时命令继续有效,其他全部无效
						i=(p8rx[9]<<8)|p8rx[10];
						if(i!=0x0601)
						{//
							return;
						}
						i=(p8rx[12]<<24)|(p8rx[13]<<16)|(p8rx[14]<<8)|p8rx[15];
						if(i!=0x40000200)
						{//!对时命令
							return;
						}
					}
					break;
			}
			UARTCtrl->AddrType=3;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			break;
	}

	if(p8rx[3]&0x08)//扰码标志位 SC
	{
		//68 17 00 4B 05 02 00 00 00 00 00 10 82 21 38 34 48 73 33 35 33 33 38 78 16 
		DataAdd33=1;
		LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数
		i=4+LEN_SA+3+2;
		if(LEN_RxAPDU>=i)
		{
			LEN_RxAPDU-=i;
		}
		else
		{
			LEN_RxAPDU=0;
		}
		for(i=0;i<LEN_RxAPDU;i++)
		{
			p8rx[5+LEN_SA+3+i]-=0x33;
		}
	}
	else
	{
		DataAdd33=0;
	}
	
	if(p8rx[3]&(1<<5))//分帧标志位
	{
		//分帧传输确认
		UARTCtrl->Task=5;//5=等待链路层分帧数据发送
		return;
	}
	UARTCtrl->Lock=0x55;//禁止线程再次调用
	switch(p8rx[3]&0x7)//应用层功能码
	{
		//0	保留	
		case 1://	链路管理	链路连接管理（登录，心跳，退出登录）
//LINK-APDU∷=CHOICE
//{
//  预连接请求	     [1]		LINK-Request，
//预连接响应       [129]	LINK-Response
//}
			i=p8rx[LEN_SA+8];
			if(i==129)//预连接响应       [129]	LINK-Response
			{
				LINK_Response(PORTn);
			}
			break;
		//2	保留	
		case 3://	用户数据	应用连接管理及数据交换服务
//客户机应用层协议数据单元（Client-APDU）
//Client-APDU∷=SEQUENCE
//{
//  应用层服务  CHOICE
//  {
//    建立应用连接请求	 [2]		CONNECT-Request，
//    断开应用连接请求	 [3]		RELEASE-Request，
//    读取请求			 [5]		GET-Request，
//    设置请求 			 [6]		SET-Request，
//    操作请求 			 [7]		ACTION-Request，
//    上报应答 			 [8]		REPORT-Response，
//    代理请求   		 [9]		PROXY-Request
//}，
//时间标签    TimeTag  OPTIONAL
//}
			i=p8rx[LEN_SA+8];
			switch(i)
			{
				case 2://建立应用连接请求	 [2]		CONNECT-Request，
					UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					CONNECT_Request(PORTn);//连接
					break;
				case 3://断开应用连接请求	 [3]		RELEASE-Request，
					UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					RELEASE_Request(PORTn);//断开
					break;
				case 5://读取请求			 [5]		GET-Request，
					GET_Request(PORTn);
					break;
				case 6://设置请求 			 [6]		SET-Request，
					UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					if(SECURITY==0)//安全模式SECURITY=0不启用=1启用
					{
						SET_Request(PORTn);
					}
					break;
				case 7://操作请求 			 [7]		ACTION-Request，
					UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					if(SECURITY==0)//安全模式SECURITY=0不启用=1启用
					{
						ACTION_Request(PORTn);
					}
					break;
				case 8://上报应答 			 [8]		REPORT-Response，
					UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					REPORT_Response(PORTn);
					break;
				case 9://代理请求   		 [9]		PROXY-Request
					if(UARTCtrl->REPORT_NUMmax)//若主站没应答最大还需重发上报次数,每上报1次减1
					{
						break;//有主动上报正在进行，放弃代理请求，因主动上报帧也存在UARTnFnDataBuff
					}
					if(SECURITY==0)//安全模式SECURITY=0不启用=1启用
					{
						PROXY_Request(PORTn);
					}
					break;
				case 16://安全请求   		[16]		SECURITY-Request
					SECURITY_Request(PORTn);//安全请求
					UARTCtrl->SECURITY_Request=0;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					break;
				case 144://安全响应   		[144]	SECURITY-Response
					SECURITY_Response(PORTn);//安全响应
					break;
			}
			
			break;
		//4…7	保留	
	}
	UARTCtrl->Lock=0;//解除线程再次调用
	if(UARTCtrl->AddrType!=1)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
	{
		if(UARTCtrl->AddrType==3)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
		{
			i=(p8rx[12]<<24)|(p8rx[13]<<16)|(p8rx[14]<<8)|p8rx[15];
			if(i==0x40010200)
			{//是读通信地址
				return;
			}
		}
		UARTCtrl->Task=0;//无需回答
		if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
		return;
	}
	if(DataAdd33)
	{
		p8rx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		p8rx[3]|=0x08;//扰码标志位 SC
		LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数
		i=4+LEN_SA+3+2;
		if(LEN_RxAPDU>=i)
		{
			LEN_RxAPDU-=i;
		}
		else
		{
			LEN_RxAPDU=0;
		}
		for(i=0;i<LEN_RxAPDU;i++)
		{
			p8rx[5+LEN_SA+3+i]+=0x33;
		}
		Uart_698_HCSFCS(p8rx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	}
}



u32 Uart_TSA_Comp(u8 *p8rx,u8 *pFLASH)//通信地址比较,pFLASH数据读到ADDR_128K_DDATABUFF中比较,返回:0=相同,!=0不相同
{
	u32 i;
	
	i=p8rx[0];
	i&=0x0f;
	i++;
	MR(ADDR_128KDATABUFF,(u32)pFLASH,i+1);
	Data_Inverse(ADDR_128KDATABUFF+1,i);//数据倒序(高低字节调换),入口Le>=2
	pFLASH=(u8*)ADDR_128KDATABUFF;
	if(i!=pFLASH[0])
	{
		return 1;
	}
	while(i--)
	{
		p8rx++;
		pFLASH++;
		if((p8rx[0]&0x0f)!=0x0a)
		{
			if((p8rx[0]&0x0f)!=(pFLASH[0]&0x0f))
			{
				return 1;
			}
		}
		if((p8rx[0]&0xf0)!=0xa0)
		{
			if((p8rx[0]&0xf0)!=(pFLASH[0]&0xf0))
			{
				return 1;
			}
		}
	}
	return 0;
}


u32 Uart_698_TxHead(u8 *p8tx,u32 C,u32 CA)//Uart698帧头填入发送缓冲区,入口:C=控制域,CA=客户机地址;返回:帧头长度
{
	u32 Len;
	
	
//起始字符（68H）  1Byte
	p8tx[0]=0x68;
//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
//控制域C  1Byte
	p8tx[3]=C;
//地址域A
	//地址标识AF  1Byte(bit0…bit3：为地址域服务器地址的字节数，采用编码方式，取值范围：0…15，对应表示1…16个字节长度)
	//服务器地址SA  可变
	MR(((u32)p8tx)+4,ADDR_4001+1,17);
	Len=p8tx[4];
	if(Len>16)
	{
		Len=16;
	}
	if(Len==0)
	{
		Len=1;
	}
	Data_Inverse((u32)p8tx+5,Len);//数据倒序(高低字节调换),入口Le>=2
	p8tx[4]=Len-1;
	//客户机地址CA  1Byte
	p8tx[5+Len]=CA;
//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
	
	Len+=8;
	return Len;
	
}

void Uart_698_TxResponse(u32 PORTn,u32 TI,u32 LEN_TxAPDU)//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
{
	u32 i;
	u32 x;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	u16* p16;

	if(LEN_TxAPDU==0)
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	
//上报信息FollowReport OPTIONAL=0 表示没有
	p8tx[LEN_TxAPDU]=0;
	LEN_TxAPDU+=1;
	//PIID-ACD修改
	
//时间标签
	p8tx[LEN_TxAPDU]=0;
	LEN_TxAPDU+=1;
//返回帧
	if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	{
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		i=p8rx[4];//取地址字节数
		i&=0x0f;
		i+=9;
		MR((u32)p8tx,(u32)p8rx,i);//复制接收头
		p8tx[3]|=0x80;
		
		MR(((u32)p8tx)+i,((u32)p8tx)+TxAPDUOffset,LEN_TxAPDU);
		i+=LEN_TxAPDU;
	//2byte FCS
	//1byte 0x16
		i+=3;
		UARTCtrl->TxByte=i;
		i-=2;
		p8tx[1]=i;
		p8tx[2]=i>>8;
		Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		UARTCtrl->Task=2;//等待单帧数据发送
	}
	else
	{
		//有下帧
		//copy rx到rx+Rxoffset
		p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
		i=Get_LEN_UARTnRx(PORTn);
		x=p8rx[1]+(p8rx[2]<<8);
		x+=3;//整半字
		x>>=1;
		x<<=1;
		if((x+Rxoffset)>i)
		{
			x=i-Rxoffset;
		}
		x/=2;
		p16=(u16*)p8rx;
		while(x)
		{
			x--;
			p16[(Rxoffset/2)+x]=p16[x];
		}
//6.3.8.2.5　GetResponseNext数据类型
//GetResponseNext∷=SEQUENCE
//{
//  服务序号-优先级-ACD  	PIID-ACD，
//  末帧标志           	BOOLEAN，
//分帧序号           	long-unsigned，
//分帧响应             	CHOICE
//{
//错误信息    	    	[0]   DAR，
//  对象属性         	[1]   SEQUENCE OF A-ResultNormal，
//  记录型对象属性    	[2]   SEQUENCE OF A-ResultRecord
//}
//}
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		i=p8rx[4];//取地址字节数
		i&=0x0f;
		i+=9;
		MR((u32)p8tx,(u32)p8rx,i);//复制接收头
		p8tx[3]|=0x80;//0xA0
		p8tx[i]=p8tx[TxAPDUOffset];//读取响应
		//p8tx[i+1]=p8tx[TxAPDUOffset+1];//GeResponseRecord
		p8tx[i+1]=5;//分帧响应一个数据块 	[5]  GetResponseNext
		p8tx[i+2]=UARTCtrl->PIID;//PIID
		i+=3;
//  末帧标志           	BOOLEAN，
		if(UARTCtrl->NEXT==3)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
		{
			p8tx[i]=1;
		}
		else
		{
			p8tx[i]=0;
		}
//分帧序号           	long-unsigned，
		p8tx[i+1]=UARTCtrl->NEXTBlock>>8;
		p8tx[i+2]=UARTCtrl->NEXTBlock;
//分帧响应             	CHOICE
//{
//	错误信息    	    	[0]   DAR，
//  对象属性         	[1]   SEQUENCE OF A-ResultNormal，
//  记录型对象属性    	[2]   SEQUENCE OF A-ResultRecord
//}
		if((UARTCtrl->GET_Reques_CHOICE==3)||(UARTCtrl->GET_Reques_CHOICE==4))
		{//3=读取一个记录型对象属性请求,4=读取若干个记录型对象属性请求
			p8tx[i+3]=2;//记录型对象属性    	[2]   SEQUENCE OF A-ResultRecord
		}
		else
		{
			p8tx[i+3]=1;//对象属性         	[1]   SEQUENCE OF A-ResultNormal
		}
		p8tx[i+4]=1;//1=数据
		
		i+=5;
		if(LEN_TxAPDU<3)
		{
			return;
		}
		MR(((u32)p8tx)+i,((u32)p8tx)+TxAPDUOffset+3,LEN_TxAPDU-3);
		i+=(LEN_TxAPDU-3);
	//2byte FCS
	//1byte 0x16
		i+=3;
		UARTCtrl->TxByte=i;
		i-=2;
		p8tx[1]=i;
		p8tx[2]=i>>8;
		Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		UARTCtrl->Task=2;//等待单帧数据发送
	}
	
}

u32 Uart_TimeTag(u8* p8rx)//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
{
	u32 i;
	u32 TI;
	
	i=Check_pYYMDHMS_hex(p8rx);//年年月日时分秒合法检查,返回0合法,1非法
	if(i)
	{
		return 1;//返回:0=有效,1=无效
	}
	TI=(p8rx[8]<<8)+p8rx[9];//取频率值
	if(TI==0)
	{
		return 0;//返回:0=有效,1=无效
	}
	switch(p8rx[7])
	{
		case 0://秒      （0），
			break;
		case 1://分      （1），
			TI*=60;
			break;
		case 2://时      （2），
			TI*=60*60;
			break;
		case 3://日      （3），
		case 4://月      （4），
		case 5://年      （5）
		default:
			TI*=60*60*24;
			break;
	}
	i=YYMDHMS_Sub_hex((u8*)&Comm_Ram->DL698YMDHMS,p8rx);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
	if(i>>31)
	{
		i=~i;
		i++;
	}
	if(i>TI)
	{
		return 1;//返回:0=有效,1=无效
	}
	return 0;//返回:0=有效,1=无效
}

#endif
