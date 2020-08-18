
//UART3761_COM
#include "Project.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"




u32 Uart_376_Tp(u32 Tpaddr)//时间标签有效判定,入口时间标签首址,出口0有效,1无效
{
	u32 i;
	u8 * p8s;
	u8 * p8d;
	p8s=(u8 *)(Tpaddr);
	p8d=(u8 *)(ADDR_DATABUFF);
	if(p8s[5]==0x0)
	{//允许发送传输延时时间=0,不进行判断
		return(0);
	}
	for(i=0;i<12;i++)
	{
		p8d[i]=0x1;
	}
	for(i=0;i<4;i++)
	{
		p8d[i]=p8s[1+i];
	}
	p8s=(u8 *)(ADDR_TYMDHMS);
	for(i=0;i<4;i++)
	{
		p8d[6+i]=p8s[i];
	}
	i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
	if((i&0x80000000)!=0x0)
	{//差值为负
		return(0);
	}
	p8s=(u8 *)(Tpaddr);
	if(i>=(p8s[5]*60))
	{
		return(1);
	}
	return(0);
}

void CopyDI(u32 PORTn,u32 DIaddr)//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	MR((u32)(&UARTCtrl->SourDI),DIaddr,4);
	MR((u32)(&UARTCtrl->DestDI),DIaddr,4);
	i=MRR(DIaddr,2);
	if(i==0x00ff)
	{//全部有效测量点,则目的DI的2字节Pn部分清为从Hex的1开始
		MWR(1,(u32)(&UARTCtrl->DestDI),2);
	}
}

u32 DItoFnPn(u32 PORTn)//数据标识分解为FnPn
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8=(u8 *)(&UARTCtrl->SourDI);
	
	//Pn
	i=p8[0]+(p8[1]<<8);
	if(i!=0x00ff)
	{//不是全部有效测量点
		if(p8[4+1]==0x0)
		{
			Pn=0;
		}
		else
		{
			if(p8[4]==0x0)
			{
				Pn=0;
			}
			else
			{
				Pn=BitLoca(p8[4]);//计算第1个置位为1的位置(0-32)
				Pn+=((p8[4+1]-1)*8);
			}
		}
	}
	else
	{//全部有效测量点
		Pn=p8[4]+(p8[4+1]<<8);
	}
	//Fn
	if(p8[4+2]==0x0)
	{
		Fn=0;
	}
	else
	{
		Fn=BitLoca(p8[4+2]);//计算第1个置位为1的位置(0-32)
		Fn+=(p8[4+3]*8);
	}
	//准备下1点Fn
	i=p8[4+2];
	i<<=1;
	i&=(~(1<<BitLoca(p8[4+2])));//计算第1个置位为1的位置(0-32)
	i>>=1;
	p8[4+2]=i;
	if(i==0x0)
	{
		//Fn从小到大完
		p8[4+2]=p8[2];//恢复DT1
		//准备下1点Pn(DA1)
		i=p8[0]+(p8[1]<<8);
		if(i!=0x00ff)
		{//不是全部有效测量点
			i=p8[4];
			i<<=1;
			i&=(~(1<<BitLoca(p8[4])));//计算第1个置位为1的位置(0-32)
			i>>=1;
			p8[4]=i;
			if(i==0x0)
			{
				//DA1从小到大完(全部点结束)
				MC(0,(u32)(&UARTCtrl->DestDI),4);
			}
		}
		else
		{//是全部有效测量点
			MWR(Pn+1,(u32)(&UARTCtrl->DestDI),2);
		}
	}
	return((Fn<<16)+Pn);
}

void FnPntoDI(u32 DIaddr,u32 FnPn)//FnPn组合为数据标识
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(DIaddr);
	i=FnPn&0xffff;
	if(i!=0x0)
	{
		i-=1;
		p8[0]|=(1<<(i%8));
		p8[1]=((i/8)+1);
	}
	i=(FnPn>>16)&0xffff;
	if(i!=0x0)
	{
		i-=1;
		p8[2]|=(1<<(i%8));
		p8[3]=(i/8);
	}
}


void Uart_376_ACK(u32 PORTn,u32 ACK)//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,数据单元标识
{
	u8 * p8txbuff;
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8txbuff[12]=0;//AFN=0
	p8txbuff[14]=0;
	p8txbuff[15]=0;
	p8txbuff[16]=ACK;//Fn,Pn
	p8txbuff[17]=0;
	p8txbuff[1]=12;//数据长度
	p8txbuff[2]=0;
	p8txbuff[6]&=0x0f;
	p8txbuff[6]|=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
}

void Uart_376_AUX(u32 PORTn,u32 rxTpaddr)//附加信息(EC,TP),暂等待单帧数据发送
{
	u32 i;
	u32 len;
	u8 * p8rxTp;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8rxTp=(u8 *)(rxTpaddr);

	len=p8txbuff[1]+(p8txbuff[2]<<8);
//EC
	if(PORTn==GPRSPORT)
	{
		if(MRR(ADDR_ADCFlags,1)!=0)//重要事件等待访问0=没,1=有
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//用户区数据长度+2
			p8txbuff[6]|=0x20;//修改控制码ACD=1
		}
	}
#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
	if(PORTn==ENETPORT)
	{
		if(MRR(ADDR_ADCFlags+1,1)!=0)//重要事件等待访问0=没,1=有
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//用户区数据长度+2
			p8txbuff[6]|=0x20;//修改控制码ACD=1
		}
	}
#else
	if(PORTn==ENETPORT)
	{
		if(MRR(ADDR_ADCFlags,1)!=0)//重要事件等待访问0=没,1=有
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//用户区数据长度+2
			p8txbuff[6]|=0x20;//修改控制码ACD=1
		}
	}
#endif
	if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
	{
		if(MRR(ADDR_ADCFlags+2,1)!=0)//重要事件等待访问0=没,1=有
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//用户区数据长度+2
			p8txbuff[6]|=0x20;//修改控制码ACD=1
		}
	}
//Tp
	if((p8txbuff[13]&0x80)!=0x0)
	{
		for(i=0;i<6;i++)
		{
			p8txbuff[6+len+i]=p8rxTp[i];
		}
		len+=6;
	}

//长度
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
//发送
	len+=8;
	UARTCtrl->Task=2;//暂等待单帧数据发送
	UARTCtrl->TxByte=len;//需发送字节数
}

void Uart_376_AUXTp(u32 PORTn,u32 rxTpaddr)//附加信息(Tp),暂等待单帧数据发送
{
	u32 i;
	u32 len;
	u8 * p8rxTp;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8rxTp=(u8 *)(rxTpaddr);
	
	len=p8txbuff[1]+(p8txbuff[2]<<8);
//Tp
	if((p8txbuff[13]&0x80)!=0x0)
	{
		for(i=0;i<6;i++)
		{
			p8txbuff[6+len+i]=p8rxTp[i];
		}
		len+=6;
	}

//长度
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
//发送
	len+=8;
	UARTCtrl->Task=2;//暂等待单帧数据发送
	UARTCtrl->TxByte=len;//需发送字节数
}



void Uart_376_NoAUX(u32 PORTn)//没附加信息,暂等待单帧数据发送
{
	u32 len;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));

	len=p8txbuff[1]+(p8txbuff[2]<<8);
//SEQ

//长度
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
//发送
	len+=8;
	UARTCtrl->Task=2;//暂等待单帧数据发送
	UARTCtrl->TxByte=len;//需发送字节数
}

u32 Uart_376_CS(u32 buffaddr)//和校验计算,同时加结束0x16;修改长度字节的标志位
{
	u32 i;
	u32 len;
	u32 cs;
	u8 * p8;
	p8=(u8 *)(buffaddr);
	len=(p8[1]+(p8[2]<<8));
	cs=0;
	for(i=0;i<len;i++)
	{
		cs+=p8[6+i];
	}
	cs&=0xff;
	p8[6+i]=cs;
	p8[7+i]=0x16;
	len<<=2;
	len|=0x2;//2=本规约标志,1=2005规约
	p8[1]=len;
	p8[2]=len>>8;
	p8[3]=len;
	p8[4]=len>>8;
	return(cs);
}

u32 Uart_3761_Rx(u32 PORTn)//UART_3761接收;返回0没完成,1完成,2超时
{
	u32 i;
	u32 cs;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;

	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
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
		p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
				p16timer[0]=10000/10;//无线通信模块超时定时10S
			}
			else
			{
				if(PORTn==SMSPORT)
				{
					p16timer[0]=60000/10;//短信通道超时定时60S
				}
				else
				{
					p16timer[0]=1000/10;//其他通道1S
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
			if(RxByte==6)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
				else
				{//2个长度比较
					i=(p8rxbuff[1]+(p8rxbuff[2]<<8));
					if(i!=(p8rxbuff[3]+(p8rxbuff[4]<<8)))
					{
						RxByte=0;
						continue;
					}
					else
					{
						//去标志位D1D0,同时长度最大只有0-16383
						i>>=2;
						p8rxbuff[1]=i;
						p8rxbuff[2]=(i>>8);
						if(i>(LEN_UARTnRx-8))
						{
							RxByte=0;
							continue;
						}
					}
				}
			}
			else
			{
				if(RxByte>6)
				{
					if(RxByte>=(p8rxbuff[1]+(p8rxbuff[2]<<8)+8))
					{
						if(i==0x16)
						{//接收完成
							//和校验计算
							cs=0;
							for(i=0;i<(p8rxbuff[1]+(p8rxbuff[2]<<8));i++)
							{
								cs+=p8rxbuff[6+i];
							}
							cs&=0xff;
					
							if(cs!=p8rxbuff[6+i])
							{//和校验错误
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
							RxByte=0;
							continue;
						}
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

u32 CheckUartData_0xEE(u32 DataAddr,u32 Byte)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
{
	u32 i;
	u8 * p8;
	if(Byte>5)
	{
		Byte=5;
	}
	p8=(u8*)DataAddr;
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xEE)
		{
			return 0;
		}
	}
	return 1;
}





