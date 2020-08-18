
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_IO.h"


void TIM7_IRQHandler(void)//71 TIM7��ʱ��10ms�ж�
{
	u32 i;
	u32 x;
	u16 *p16;


//10ms��ʱ��
	p16=(u16*)(ADDR_Terminal_MS_Timer);
	for(i=0;i<Terminal_MS_Timer_Num;i++)
	{
		x=p16[i];
		if(x!=0x0)
		{
			x--;
			p16[i]=x;
		}
	}
	

//GPRS ONOFF�ſ���,=1�����,0=����
	if(Terminal_Ram->GPRS_ONOFF_MS_Timer==1)//15 GPRS ONOFF�ſ��� 10MS��ʱ��
	{
		Pin_OutH(PIN_ON_GPRS);//���������,�������ʹ����
	}
}


void RTC_Alarm_IRQHandler(void)//;57 RTC Alarm (A and B) through EXTI Line
{
	u32 i;
	u32 x;
	u16 *p16;
	
	p16=(u16*)(ADDR_Terminal_S_Timer);
	for(i=0;i<Terminal_S_Timer_Num;i++)
	{
		x=p16[i];
		if(x!=0x0)
		{
			x--;
			p16[i]=x;
		}
	}
	Terminal_Ram->Terminal_S_Count++;//�ն���Ӽ���,���������붨ʱ��
	if(Terminal_Ram->S_PR==0x0)
	{//1 60���Ƶ������
		Terminal_Ram->S_PR=60;
		p16=(u16*)(ADDR_Terminal_M_Timer);
		for(i=0;i<Terminal_M_Timer_Num;i++)
		{
			x=p16[i];
			if(x!=0x0)
			{
				x--;
				p16[i]=x;
			}
		}
		Terminal_Ram->Terminal_M_Count++;//�ն˷ּӼ���,���������ֶ�ʱ��
	}
	
//
	p16=(u16*)(ADDR_RUNSPEEDTEST);//ѭ�������ٶȲ���(���ֶ���)
	i=p16[0];
	p16[0]=0;
	p16[1]=i;
}





void I2_GPIO_ISR(void)//I2 GPIO�жϷ���
{
/*
//TESAMIO�ж�
	LPC_GPIOINT->IO0IntClr=(1<<(PIN_TESAMIO&0x1f));//���жϱ�־
*/
}

