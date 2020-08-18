
//Uart_Auto_645
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "UartAuto_645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"




void UartAuto_645(u32 PORTn)//�Զ�ʶ��DL_645֡
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

	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));//�õ�UART��������ַ);
	if(UARTCtrl->Task!=0x0)
	{//�������ڽ���
		return;
	}
	if(UARTCtrl->RxByte!=0x0)
	{////��ǰ֡δ��������(����ת��ʱ����645֡)
		return;
	}
	p16=(u16 *)(Get_ADDR_UARTn(PORTn));
	x=p16[0];
	y=p16[2];
	if(x<y)
	{//FIFOָ��<FIFO�Ѽ��ָ��
		x+=(Get_LEN_UARTnIntRx(PORTn)-10);//�õ�UART�жϽ��ջ��峤��
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
	{//�������
		y++;
		p16[2]=y;
		return;
	}
	//��У�����
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
	{//��У���
		y++;
		p16[2]=y;
		return;
	}
	UARTCtrl->Protocol=1;//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
	UARTCtrl->RxByte=0;//�����ֽ���=0
	p16[1]=y;//����FIFO�Ѷ�ָ��
	i=y+10+x+2;
	i%=LEN_UARTnIntRx;
	p16[2]=i;//FIFO��Լ���ָ��
}




