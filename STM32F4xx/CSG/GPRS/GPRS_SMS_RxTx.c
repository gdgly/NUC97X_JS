
//#if MainProtocol==376
//#include "../Head/Project.h"
//#include "../Terminal/Terminal_Uart_3761.h"
//#endif
//#if MainProtocol==698
//#include "../H698/Project698.h"
//#include "../DL698/DL698_DataDef.h"
//#include "../DL698/DL698_Uart.h"
//#endif

#include "../Hcsg/ProjectCSG.h"

#include "../GPRS/GPRS_SMS_RxTx.h"

//#include "../Device/GPRS_3763.h"

#include "../GPRS/ATNo.h"
#include "../GPRS/ATString.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "../STM32F4xx/STM32F4xx_SoftDelay.h"



u32 LEN_TelephoneNumber(u32 Addr)//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
{
	u32 i;
	u32 x;
	u8 * p8;
	
	MR(ADDR_DATABUFF,Addr,8);
	i=8;//8�ֽ�����
	x=0;//����λ��=0
	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0x86)
	{
		i--;
		p8++;
	}
	for(;i!=0x0;i--)
	{
		if((p8[0]>>4)==0xf)
		{//����δ
			break;
		}
		x++;
		if((p8[0]&0xf)==0xf)
		{//����δ
			break;
		}
		x++;
		p8++;
	}
	return x;
}

u8 * SMS_BCD_ASCII(u32 BCD,u8 *p8)//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ;�����¸�����ָ��p8
{
	u32 i;
	i=BCD>>4;
	if(i<10)
	{
		i+=0x30;
	}
	else
	{
		i+=55;
	}
	p8[0]=i;
	i=BCD&0xf;
	if(i<10)
	{
		i+=0x30;
	}
	else
	{
		i+=55;
	}
	p8[1]=i;
	p8+=2;
	return(p8);
}

u32 SMS_ASCII_BCD(u32 Addr,u32 BCDByte)//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8s;
	u8 *p8d;
	p8s=(u8*)Addr;
	p8d=p8s;
	y=0;
	for(i=0;i<BCDByte;i++)
	{
		y<<=8;
		x=p8s[0];
		if((x>=0x30)&&(x<=0x39))
		{
			x-=0x30;
		}
		else
		{
			x-=(0x41-10);
		}
		y|=x<<4;
		x=p8s[1];
		if((x>=0x30)&&(x<=0x39))
		{
			x-=0x30;
		}
		else
		{
			x-=(0x41-10);
		}
		y|=x;
		p8d[i]=y;
		p8s+=2;
	}
	if(y>=0x100)
	{
		y=0;
	}
	return y;
}


u32 GPRS_SMS_Tx(void)//GPRS_SMS_Tx����;���ط����ֽ���
{
	return 0;
}

void GPRS_SMS_Rx(void)//GPRS_SMS_Rx����

{

}


	
