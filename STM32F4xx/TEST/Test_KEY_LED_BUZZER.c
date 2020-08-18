
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../Device/Button.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/IC_BUZZ.h"
#include "../../NUC97x/NUC97x/NUC97x_AIC.h"



const u16 Pin_LED_LIST[]=
{
#if ((Project/100)!=5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
	PIN_RUNLED,
	PIN_ATTR_LED,
	PIN_ENET_LINK_LED,
	PIN_ENET_ACT_LED,
	PIN_RS485R1_LED,
	PIN_RS485T1_LED,
	PIN_RS485R2_LED,
	PIN_RS485T2_LED,
#else
	PIN_ATTR_LED,
	PIN_GPRSRX_LED,
	PIN_RS485T1_LED,
	PIN_LED_ONLINE,
	PIN_WEAK_SINGLE,
	PIN_STRONG_SINGLE,
	PIN_RS485R1_LED,
	PIN_GPRSTX_LED,
#endif
};



u32 Test_KEY_LED_BUZZER(u32 ADDR_BASE)
{
	u32 i;
	u16 *p16;

	__enable_irq();//�����ж�
	DisplayStringAutoUp(0,9,(u8*)"\x0""LED KEY BUZZER");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""         \x1e");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""      \x11 \x11 \x10 \x10");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""         \x1f");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(6*2));
	p16[0]=62;
	p16[6]=61;
	LCDDataOut();//�������������LCD��
	
//�����Ӱ���(0)���طſ�(1)��ΪOK
	//Comm_Ram->TESTKEY=0;
//BUZZER
	#ifdef PIN_BUZZ
	Pin_Configure(PIN_BUZZ,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Comm_Ram->BUZZCTRL[8]=1;//��������������/��ֹ,=0��ֹ,!=0����
	#endif
//LED
	for(i=0;i<(sizeof(Pin_LED_LIST)/2);i++)
	{
		Pin_Configure(Pin_LED_LIST[i],//��������
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (1<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	}
	Comm_Ram->GPMSTimer=500/10;//LED��ʱ
	Comm_Ram->BattWork_S_timer=120;//�ȴ����������ܶ�ʱ
	while(1)
	{
		if(Comm_Ram->BattWork_S_timer==0)//�ȴ����������ܶ�ʱ
		{
			MWR(1,ADDR_BASE+(OFFSET_KEY_Characteristics),1);
			return 1;//��������
		}
		WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		Button();//��������
		i=Comm_Ram->ButtonCurrent;
		i&=0x3f;
		i=BitNum(i);//������λΪ1�ĸ���(0-32)
		if(i<=1)
		{
			if((Comm_Ram->TESTKEY&0x3f)==0x3f)
			{
				MC(0,ADDR_BASE+(OFFSET_KEY_Characteristics),1);
				break;
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-1)*84)+4+(9*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&1)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//��������,��ڷ�����
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-3)*84)+4+(9*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&2)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//��������,��ڷ�����
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(8*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&4)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//��������,��ڷ�����
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(10*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&8)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//��������,��ڷ�����
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(12*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&0x10)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//��������,��ڷ�����
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(6*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&0x20)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//��������,��ڷ�����
			}
			LCDDataOut();//�������������LCD��
		}
		if(Comm_Ram->GPMSTimer==0)
		{
			Comm_Ram->GPMSTimer=500/10;
			if(Pin_Read(PIN_RUNLED))
			{
				for(i=0;i<(sizeof(Pin_LED_LIST)/2);i++)
				{
					Pin_OutL(Pin_LED_LIST[i]);
				}
			}
			else
			{
				for(i=0;i<(sizeof(Pin_LED_LIST)/2);i++)
				{
					Pin_OutH(Pin_LED_LIST[i]);
				}
			}
		}
	}
	MWR(0,ADDR_BASE+(OFFSET_KEY_Characteristics),1);
	return 0;
}

















