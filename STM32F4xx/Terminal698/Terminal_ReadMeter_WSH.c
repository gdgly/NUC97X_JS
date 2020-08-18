
#include "Project.h"
#include "Terminal_ReadMeter_WSH.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"


#if (USER/100)==10//河北
u32 WSH41_Rx(u32 PORTn)//威胜接收;返回0没完成,1完成,2超时
{
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+6);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	if((p0>=(LEN_UARTnIntRx-6))||(p1>=(LEN_UARTnIntRx-6)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收重置超时定时
		p16timer[0]=2000/10;//字节间超时定时
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-6))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(p8txbuff[0]!=p8rxbuff[0])
			{//接收到的帧表址和发送的不一样
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte==4)
			{
				if(i!=0x0D)
				{//威胜规约结束码为0x0D
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=9)
				{
					if(RxByte>=(p8rxbuff[8]+11))
					{
						if(i==0x0D)
						{//接收完成
							//和校验计算
							Pn=0;
							for(i=4;i<(p8rxbuff[8]+9);i++)
							{
								Pn+=p8rxbuff[i];
							}
							Pn&=0xff;
							if(Pn==p8rxbuff[i])
							{//和校验正确
								p16fifo[1]=p1;//当前已读指针
								p16fifo[2]=p1;//检查指针==读指针,免检省时
								UARTCtrl->RxByte=RxByte;
								return 1;
							}
							else
							{//和校验错误
								RxByte=0;
								continue;
							}
		
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
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		return 2;//2=接收超时
	}
	return 0;
}


u32 WSH41_RxDATA(u32 PORTn)//威胜查询应答命令接收数据;返回0应答正确，1表址错，2命令标志字错，3标志地址错，4数据长度错
{
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8rxbuff+=4;
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//表址比较
	if(p8txbuff[0]!=p8rxbuff[0])
	{
		return 1;//返回0应答正确，1表址错，2命令标志字错，3标志地址错，4数据长度错
	}
	//命令标志字比较
	if(p8rxbuff[1]!=0xAA&&p8rxbuff[1]!=0xA5)
	{
		return 2;//返回0应答正确，1表址错，2命令标志字错，3标志地址错，4数据长度错
	}
	//标志地址比较
	if((p8txbuff[2]!=p8rxbuff[2])||(p8txbuff[3]!=p8rxbuff[3]))
	{
		return 3;//返回0应答正确，1表址错，2命令标志字错，3标志地址错，4数据长度错
	}
	//数据长度比较
	if(p8txbuff[4]!=p8rxbuff[4])
	{
		return 4;//返回0应答正确，1表址错，2命令标志字错，3标志地址错，4数据长度错
	}
	return 0;//返回0正确,1错误
}

#endif
