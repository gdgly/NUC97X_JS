
//Uart_Auto_645
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "UartAuto_645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"




void UartAuto_645(u32 PORTn)//自动识别DL_645帧
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 rxbyte;
	u32 LEN_UARTnIntRx;
	u8 * p8;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;

	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));//得到UART控制区地址);
	if(UARTCtrl->Task!=0x0)
	{//不是正在接收
		return;
	}
	if(UARTCtrl->RxByte!=0x0)
	{////当前帧未接收完整(数据转发时内有645帧)
		return;
	}
	p16=(u16 *)(Get_ADDR_UARTn(PORTn));
	x=p16[0];
	y=p16[2];
	if(x<y)
	{//FIFO指针<FIFO已检查指针
		x+=(Get_LEN_UARTnIntRx(PORTn)-10);//得到UART中断接收缓冲长度
	}
	rxbyte=x-y;
	if(rxbyte<8)
	{
		return;
	}
	p8=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn)-10;
	rxbyte%=LEN_UARTnIntRx;
	while(rxbyte!=0x0)
	{
		y%=LEN_UARTnIntRx;
		if(p8[y]==0x68)
		{
			break;
		}
		y++;
		rxbyte--;
	}
	p16[2]=y;
	if(rxbyte<8)
	{
		return;
	}
	i=y+7;
	i%=LEN_UARTnIntRx;
	if(p8[i]!=0x68)
	{
		y++;
		p16[2]=y;
		return;
	}
	i=y+9;
	i%=LEN_UARTnIntRx;
	x=p8[i];
	if(rxbyte<(10+x+2))
	{
		return;
	}
	i=y+10+x+1;
	i%=LEN_UARTnIntRx;
	if(p8[i]!=0x16)
	{//结束码错
		y++;
		p16[2]=y;
		return;
	}
	//和校验计算
	z=0;
	for(rxbyte=0;rxbyte<(10+x);rxbyte++)
	{
		i=y+rxbyte;
		i%=LEN_UARTnIntRx;
		z+=p8[i];
	}
	z&=0xff;
	i=y+10+x;
	i%=LEN_UARTnIntRx;
	if(p8[i]!=z)
	{//和校验错
		y++;
		p16[2]=y;
		return;
	}
	UARTCtrl->Protocol=1;//通信规约自动识别:0=376.1,1=DL/T645-2007
	UARTCtrl->RxByte=0;//接收字节数=0
	p16[1]=y;//重置FIFO已读指针
	i=y+10+x+2;
	i%=LEN_UARTnIntRx;
	p16[2]=i;//FIFO规约检查指针
}




