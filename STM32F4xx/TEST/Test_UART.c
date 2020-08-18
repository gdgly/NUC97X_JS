

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"


u32 Test_UART(u32 ADDR_BASE)
{
	u32 i;
	u8 *p8txbuff;
	u8 *p8rxbuff;
	u16 *p16;
	u32 ReNum;
	u32 RS485_BPS;
	u32 IR_BPS;
	u32 RS232_BPS;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	UARTCtrl_TypeDef * UARTCtrl1;
	UARTCtrl_TypeDef * UARTCtrl2;
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	__enable_irq();//�����ж�
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
//RS485-1��RS485-2
	DisplayStringAutoUp(0,9,(u8*)"\x0""RS485-1 RS485-2");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	for(RS485_BPS=0;RS485_BPS<10;RS485_BPS++)
	{
		RCC->AHB1RSTR=3<<21;//��λDMA1,DMA2
		RCC->AHB1RSTR=0;
		UARTCtrl1->BpsCtrl=0x0b+(RS485_BPS<<5);//D8-D5λ��ʾ������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
		Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
		UARTCtrl2->BpsCtrl=0x0b+(RS485_BPS<<5);
		Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
		p8txbuff=(u8*)Get_ADDR_UARTnTx(RS485_1PORT);
		p8rxbuff=(u8*)(Get_ADDR_UARTn(RS485_2PORT)+10);
		for(i=0;i<16;i++)
		{
			p8txbuff[i]=i;
			p8rxbuff[i]=0;
		}
		UARTCtrl1->TxByte=16;
		Start_UARTn_Tx(RS485_1PORT);//����UART�ڷ���
		while(Wait_UARTn_Tx(RS485_1PORT));//�ȴ�UART�ڷ���,����0=�������,1=����û���
		p8txbuff=(u8*)Get_ADDR_UARTnTx(RS485_2PORT);
		for(i=0;i<16;i++)
		{
			p8txbuff[i]=p8rxbuff[i];
		}
		p8rxbuff=(u8*)(Get_ADDR_UARTn(RS485_1PORT)+10);
		for(i=0;i<16;i++)
		{
			p8rxbuff[i]=0;
		}
		UARTCtrl2->TxByte=16;
		Start_UARTn_Tx(RS485_2PORT);//����UART�ڷ���
		while(Wait_UARTn_Tx(RS485_2PORT));//�ȴ�UART�ڷ���,����0=�������,1=����û���
		for(i=0;i<16;i++)
		{
			if(p8rxbuff[i]!=i)
			{
				break;
			}
		}
		if(i<16)
		{
			if(RS485_BPS!=0)
			{
				RS485_BPS--;
			}
			break;
		}
	}
	if(RS485_BPS>=6)//������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
	
#if MainProtocol==CSG
//RS485-1��RS485-3
	DisplayStringAutoUp(0,9,(u8*)"\x0""RS485-3");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
	for(RS485_BPS=0;RS485_BPS<10;RS485_BPS++)
	{
		RCC->AHB1RSTR=3<<21;//��λDMA1,DMA2
		RCC->AHB1RSTR=0;
		UARTCtrl1->BpsCtrl=0x0b+(RS485_BPS<<5);//D8-D5λ��ʾ������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
		Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
		UARTCtrl2->BpsCtrl=0x0b+(RS485_BPS<<5);
		Init_UARTn(RS485_3PORT);//��ʼ��UART��,���ͨ�ſ�����������
		p8txbuff=(u8*)Get_ADDR_UARTnTx(RS485_1PORT);
		p8rxbuff=(u8*)(Get_ADDR_UARTn(RS485_3PORT)+10);
		for(i=0;i<16;i++)
		{
			p8txbuff[i]=i;
			p8rxbuff[i]=0;
		}
		UARTCtrl1->TxByte=16;
		Start_UARTn_Tx(RS485_1PORT);//����UART�ڷ���
		while(Wait_UARTn_Tx(RS485_1PORT));//�ȴ�UART�ڷ���,����0=�������,1=����û���
		p8txbuff=(u8*)Get_ADDR_UARTnTx(RS485_3PORT);
		for(i=0;i<16;i++)
		{
			p8txbuff[i]=p8rxbuff[i];
		}
		p8rxbuff=(u8*)(Get_ADDR_UARTn(RS485_1PORT)+10);
		for(i=0;i<16;i++)
		{
			p8rxbuff[i]=0;
		}
		UARTCtrl2->TxByte=16;
		Start_UARTn_Tx(RS485_3PORT);//����UART�ڷ���
		while(Wait_UARTn_Tx(RS485_3PORT));//�ȴ�UART�ڷ���,����0=�������,1=����û���
		for(i=0;i<16;i++)
		{
			if(p8rxbuff[i]!=i)
			{
				break;
			}
		}
		if(i<16)
		{
			if(RS485_BPS!=0)
			{
				RS485_BPS--;
			}
			break;
		}
	}
	if(RS485_BPS>=6)//������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
#endif
	
//����
	DisplayStringAutoUp(0,9,(u8*)"\x0""IR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
	for(IR_BPS=0;IR_BPS<10;IR_BPS++)
	{
		ReNum=3;
		while(1)
		{
		#if MainProtocol==CSG
			Comm_Ram->ErrorIC&=~(1<<14);
		#endif
			p16=(u16 *)(ADDR_UART6IntRx);
			p16[0]=0;
			UARTCtrl1->BpsCtrl=0x0b+(IR_BPS<<5);//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			Init_UARTn(IRPORT);//��ʼ��UART��,���ͨ�ſ�����������
			p8txbuff=(u8*)Get_ADDR_UARTnTx(IRPORT);
			p8rxbuff=(u8*)(Get_ADDR_UARTn(IRPORT));
			for(i=0;i<(10+16);i++)
			{
				p8txbuff[i]=i;
				p8rxbuff[i]=0;
			}
			UARTCtrl1->TxByte=16;
			Start_UARTn_Tx(IRPORT);//����UART�ڷ���
			while(Wait_UARTn_Tx(IRPORT));//�ȴ�UART�ڷ���,����0=�������,1=����û���
			SoftDelayUS(100000);//���us��ʱ
		#if MainProtocol==CSG
			if((Comm_Ram->ErrorIC&(1<<14))==0)//B14=����ͨ��(�Զ�,�Ƿ�������Է����ռ��,IO��������ж��м��)
			{
				break;
			}
		#else
			for(i=0;i<16;i++)
			{
				if(p8rxbuff[10+i]!=i)
				{
					break;
				}
			}
			if(i>=16)
			{
				break;
			}
		#endif
			ReNum--;
			if(ReNum==0)
			{
				if(IR_BPS!=0)
				{
					IR_BPS--;
				}
				goto IRError;
			}
		}
	}
IRError:
	if(IR_BPS>=3)//������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
//RS232
	RS232_BPS=10;
/*
	DisplayStringAutoUp(0,9,(u8*)"\x0""RS232");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);
	for(RS232_BPS=2;RS232_BPS<11;RS232_BPS++)
	{
		RCC->AHB1RSTR=3<<21;//��λDMA1,DMA2
		RCC->AHB1RSTR=0;
		p16=(u16 *)(ADDR_UART0IntRx);
		p16[0]=0;
		UARTCtrl1->BpsCtrl=0x0b+(RS232_BPS<<5);//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
		Init_UARTn(RS232PORT);//��ʼ��UART��,���ͨ�ſ�����������
		p8txbuff=(u8*)Get_ADDR_UARTnTx(RS232PORT);
		p8rxbuff=(u8*)(Get_ADDR_UARTn(RS232PORT)+10);
		for(i=0;i<16;i++)
		{
			p8txbuff[i]=i;
			p8rxbuff[i]=0;
		}
		UARTCtrl1->TxByte=16;
		Start_UARTn_Tx(RS232PORT);//����UART�ڷ���
		while(Wait_UARTn_Tx(RS232PORT));//�ȴ�UART�ڷ���,����0=�������,1=����û���
		SoftDelayUS(100000);//���us��ʱ
		for(i=0;i<16;i++)
		{
			if(p8rxbuff[i]!=i)
			{
				break;
			}
		}
		if(i<16)
		{
			if(RS232_BPS!=0)
			{
				RS232_BPS--;
			}
			break;
		}
	}
	if(RS232_BPS>=10)//������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
*/
	
	//�洢����
	p8txbuff=(u8*)ADDR_DATABUFF;
	p8txbuff[0]=RS485_BPS;
	p8txbuff[1]=IR_BPS;
	p8txbuff[2]=RS232_BPS;
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_UART_Characteristics),3);
	
	return TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
}








