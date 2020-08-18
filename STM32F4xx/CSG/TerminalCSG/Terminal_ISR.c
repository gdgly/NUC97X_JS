
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/IC_BUZZ.h"

#include "../QCSG_Head/QCSG_Hal.h"

void TerminalTaskTimeOutCount(void);


void I2_ImpIn_ISR(void)//��������
{
	#if (MaxImpInPort>=1)
	u32 i;
	u32 x;
	u8 *p8;
	u16 *p16;
	
	p8 = (u8*)(ADDR_Terminal_impIn_Ctrl);
	p16 = (u16*)(ADDR_Terminal_impIn_Count);
	i = Pin_Read(PIN_P208_SIN3);//������,�������ʹ����;����0��1
	if(i != 0x0)
	{
		p8[0] = 0;
	}
	else
	{
		i = p8[0];
		if(i < 2)
		{
			i++;
			if(i == 2)
			{
				x = p16[0];
				x++;
				p16[0] = x;
			}
			p8[0] = i;
		}
	}
	#endif
	#if (MaxImpInPort>=2)
	i=Pin_Read(PIN_P208_SIN4);//������,�������ʹ����;����0��1
	if(i != 0x0)
	{
		p8[1] = 0;
	}
	else
	{
		i = p8[1];
		if(i < 2)
		{
			i++;
			if(i == 2)
			{
				x = p16[1];
				x++;
				p16[1] = x;
			}
			p8[1] = i;
		}
	}
	#endif
/*
	#if (MaxImpInPort>=3)
	i=Pin_Read(PIN_P208_SIN3);//������,�������ʹ����;����0��1
	if(i!=0x0)
	{
		p8[2]=0;
	}
	else
	{
		i=p8[2];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[2];
				x++;
				p16[2]=x;
			}
			p8[2]=i;
		}
	}
	#endif
	#if (MaxImpInPort>=4)
	i=Pin_Read(PIN_P208_SIN4);//������,�������ʹ����;����0��1
	if(i!=0x0)
	{
		p8[3]=0;
	}
	else
	{
		i=p8[3];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[3];
				x++;
				p16[3]=x;
			}
			p8[3]=i;
		}
	}
	#endif
	#if (MaxImpInPort>=5)
	i=Pin_Read(PIN_P208_SIN5);//������,�������ʹ����;����0��1
	if(i!=0x0)
	{
		p8[4]=0;
	}
	else
	{
		i=p8[4];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[4];
				x++;
				p16[4]=x;
			}
			p8[4]=i;
		}
	}
	#endif
	#if (MaxImpInPort>=6)
	i=Pin_Read(PIN_P208_SIN6);//������,�������ʹ����;����0��1
	if(i!=0x0)
	{
		p8[5]=0;
	}
	else
	{
		i=p8[5];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[5];
				x++;
				p16[5]=x;
			}
			p8[5]=i;
		}
	}
	#endif
*/
}

void TIM7_IRQHandler(void)//71 TIM7��ʱ��10ms�ж�
{
	u32 i;
	u32 x;
	u16 *p16;
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4)||((Project/100)==9))
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
#endif
//���������1��
	I2_ImpIn_ISR();//��������
	
//10ms��ʱ��
	p16 = (u16*)(ADDR_Terminal_MS_Timer);
	for(i = 0; i < Terminal_MS_Timer_Num; i++)
	{
		x=p16[i];
		if(x!=0x0)
		{
			x--;
			p16[i]=x;
		}
	}

	pTerminal_Ram->Ticks++;

	
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)||((Project/100)==9)	//I�ͼ����������ɹ����նˡ����
	//ENET LED
	if(pTerminal_Ram->ENETLED_MS_Timer == 0)	// 14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_ENET_ACT_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_ENET_ACT_LED);//���������,�������ʹ����
	}
	

	// ����485_1_LED(ԭ����485LED)
	// ����
	if(pTerminal_Ram->RS485_1_RxLED_MS_Timer==0)//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485R1_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485R1_LED);//���������,�������ʹ����
	}
	//����
	if(pTerminal_Ram->RS485_1_TxLED_MS_Timer==0)//13 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485T1_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485T1_LED);//���������,�������ʹ����
	}

	//����485_2_LED
	//����
	if(pTerminal_Ram->RS485_2_RxLED_MS_Timer==0)//10 RS485��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485R2_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485R2_LED);//���������,�������ʹ����
	}
	//����
	if(pTerminal_Ram->RS485_2_TxLED_MS_Timer==0)//11 RS485��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485T2_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485T2_LED);//���������,�������ʹ����
	}

	//���������붨ʱ��
	if(pCommRam->BUZZCTRL[8] == 0)
	{
		if(pTerminal_Ram->BUZZMSTimer == 0)
		{
			 Pin_OutL(PIN_BUZZ);
		}
		else
		{
			Pin_OutH(PIN_BUZZ);
		}
	}
#endif

	
#if (Project/100)==5   //II�ͼ�����
	// ����485_1_LED(ԭ����485LED)
	// ����
	if(pTerminal_Ram->RS485_1_RxLED_MS_Timer==0)//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485R1_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485R1_LED);//���������,�������ʹ����
	}
	//����
	if(pTerminal_Ram->RS485_1_TxLED_MS_Timer==0)//13 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485T1_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485T1_LED);//���������,�������ʹ����
	}
	
//�˴����ε���2·485��1·485ʹ��ͬһ�����붨ʱ��ybzyl20180601	
//	if(pTerminal_Ram->RS485_2_RxLED_MS_Timer==0)//10 RS485_2��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//	{
//		Pin_OutH(PIN_RS485R1_LED);//���������,�������ʹ����
//	}
//	else
//	{
//		Pin_OutL(PIN_RS485R1_LED);//���������,�������ʹ����
//	}
//	//����
//	if(pTerminal_Ram->RS485_2_TxLED_MS_Timer==0)//11 RS485_2��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//	{
//		Pin_OutH(PIN_RS485T1_LED);//���������,�������ʹ����
//	}
//	else
//	{
//		Pin_OutL(PIN_RS485T1_LED);//���������,�������ʹ����
//	}
	
	//GPRS���ݽ��շ���ָʾ��
	//����
	if(pTerminal_Ram->GPRS_RxLED_MS_Timer==0)//GPRS����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_GPRSRX_LED);
	}
	else
	{
		Pin_OutL(PIN_GPRSRX_LED);
	}
	//����
	if(pTerminal_Ram->GPRS_TxLED_MS_Timer==0)//GPRS����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_GPRSTX_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_GPRSTX_LED);//���������,�������ʹ����
	}
	
	//GPRS����ָʾ��
	if(pTerminal_Ram->GPRS_ONLINELED_MS_Timer==0)
	{
		Pin_OutH(PIN_LED_ONLINE);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_LED_ONLINE);//���������,�������ʹ����
	}
	
	//GPRS�ź�����ָʾ��
	if(pTerminal_Ram->GPRS_WEAK_SINGLE_LED_MS_Timer==0)
	{
		Pin_OutH(PIN_WEAK_SINGLE);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_WEAK_SINGLE);//���������,�������ʹ����
	}
	
	//GPRS�ź���ǿָʾ��
	if(pTerminal_Ram->GPRS_STRONG_SINGLE_LED_MS_Timer==0)
	{
		Pin_OutH(PIN_STRONG_SINGLE);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_STRONG_SINGLE);//���������,�������ʹ����
	}

	
#endif

	
//���������2��
	I2_ImpIn_ISR();//��������

//GPRS ONOFF�ſ���,=1�����,0=����
	if(pTerminal_Ram->GPRS_ONOFF_MS_Timer == 1)//15 GPRS ONOFF�ſ��� 10MS��ʱ��
	{
		Pin_OutH(PIN_ON_GPRS);//���������,�������ʹ����
	}
}


void RTC_Alarm_IRQHandler(void)//;57 RTC Alarm (A and B) through EXTI Line
{
	u32 i;
	u32 x;
	u16 *p16;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	//Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	//HAL_PinReverse(PIN_RUNLED);
	Pin_OutL(PIN_RUNLED);
	//Pin_OutH(PIN_ATTR_LED);
	p16 = (u16*)(ADDR_Terminal_S_Timer);
	for(i = 0; i < Terminal_S_Timer_Num; i++)
	{
		x = p16[i];
		if(x != 0x0)
		{
			x--;
			p16[i] = x;
		}
	}
	pTerminalRam->Terminal_S_Count++;			// �ն���Ӽ���,���������붨ʱ��
	if(pTerminalRam->S_PR == 0x0)
	{ 											// 60���Ƶ������
		pTerminalRam->S_PR = 60;
		p16=(u16*)(ADDR_Terminal_M_Timer);
		for(i = 0; i < Terminal_M_Timer_Num; i++)
		{
			x = p16[i];
			if(x != 0x0)
			{
				x--;
				p16[i] = x;
			}
		}
		pTerminalRam->Terminal_M_Count++;		// �ն˷ּӼ���,���������ֶ�ʱ��
	}
	//TerminalTaskTimeOutCount();
	pTerminalRam->Seconds++;
	if(pTerminalRam->RT_AMRTimeOut > 0) pTerminalRam->RT_AMRTimeOut--;
	if(pTerminalRam->AT_AMSTimeOut > 0) pTerminalRam->AT_AMSTimeOut--;
	
	p16 = (u16*)(ADDR_RUNSPEEDTEST);				// ѭ�������ٶȲ���(���ֶ���)
	i = p16[0];
	p16[0] = 0;
	p16[1] = i;
}


void TerminalTaskTimeOutCount(void)
{
	u32 i = 0;

	UartTaskCtrlStruct* pUART_Task = (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl;
	for(i = 0; i < UART_TASK_NUM; i++)
	{		
		if(pUART_Task->WaitAckTimeOut != 0)
		{
			pUART_Task->WaitAckTimeOut--;
			if(pUART_Task->WaitAckTimeOut == 0)
			{
				pUART_Task->Flag |= UART_FLAG_WAIT_ACK_TIME_OUT;
			}
		}
		pUART_Task++;
	}


	
}


void I2_GPIO_ISR(void)//I2 GPIO�жϷ���
{

}
