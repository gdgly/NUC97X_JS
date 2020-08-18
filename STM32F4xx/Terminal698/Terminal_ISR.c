
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../STM32F4xx/STM32F4xx_IO.h"


#if (USER/100)==0//�ǵ��Ժ����
#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#define NUM_ImpIn_Sample_Valid    7//������������������Ч������3��5��7
void ImpIn_ISR(void)//��������
{
	#if (NUMmax_IMPIN>=1)
	u32 i;
	u32 impC;
	u32 impL;
	u32 impH;
	u8 *p8;
	u16 *p16;
	
	p8=(u8*)(ADDR_Terminal_impIn_Ctrl);
	p16=(u16*)(ADDR_Terminal_impIn_Count);
	
	i=Pin_Read(PIN_SWIN3);//������,�������ʹ����;����0��1
	impC=p8[0];
	impL=impC&0x07;
	impH=(impC>>3)&0x07;
	impC&=0x80;
	if(i!=0x0)
	{
		impH+=1;
	}
	else
	{
		impL+=1;
	}
	if((impL+impH)<NUM_ImpIn_Sample_Valid)
	{
		p8[0]=(impL+(impH<<3))|impC;
	}
	else
	{
		if(impH>impL)
		{
			p8[0]=0x80;
		}
		else
		{
			if(impC==0x80)
			{
				i=p16[0];
				i++;
				p16[0]=i;
			}
			p8[0]=0;
		}
	}
	#endif
	#if (NUMmax_IMPIN>=2)
	i=Pin_Read(PIN_SWIN4);//������,�������ʹ����;����0��1
	impC=p8[1];
	impL=impC&0x07;
	impH=(impC>>3)&0x07;
	impC&=0x80;
	if(i!=0x0)
	{
		impH+=1;
	}
	else
	{
		impL+=1;
	}
	if((impL+impH)<NUM_ImpIn_Sample_Valid)
	{
		p8[1]=(impL+(impH<<3))|impC;
	}
	else
	{
		if(impH>impL)
		{
			p8[1]=0x80;
		}
		else
		{
			if(impC==0x80)
			{
				i=p16[1];
				i++;
				p16[1]=i;
			}
			p8[1]=0;
		}
	}
	#endif
}
#endif
#endif

void TIM7_IRQHandler(void)//71 TIM7��ʱ��10ms�ж�
{
	u32 i;
	u32 x;
	u16 *p16;


//��������
#if (USER/100)==0//�ǵ��Ժ����
#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	ImpIn_ISR();//��������
#endif
#if ((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		ImpIn_ISR();//��������
	}
#endif
#endif
	
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
	
#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//��բ���
	#if (MaxControlRound>=1)
	if(Terminal_Ram->Trip1_MS_Timer>(1000/10))
	{
		Pin_OutH(PIN_TRIP1);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_TRIP1);//���������,�������ʹ����
	}
	#endif
	#if (MaxControlRound>=2)
	if(Terminal_Ram->Trip2_MS_Timer>(1000/10))
	{
		Pin_OutH(PIN_TRIP2);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_TRIP2);//���������,�������ʹ����
	}
	#endif
#endif//#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#if ((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//��բ���
	if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		#if (MaxControlRound>=1)
		if(Terminal_Ram->Trip1_MS_Timer>(1000/10))
		{
			Pin_OutH(PIN_TRIP1);//���������,�������ʹ����
		}
		else
		{
			Pin_OutL(PIN_TRIP1);//���������,�������ʹ����
		}
		#endif
		#if (MaxControlRound>=2)
		if(Terminal_Ram->Trip2_MS_Timer>(1000/10))
		{
			Pin_OutH(PIN_TRIP2);//���������,�������ʹ����
		}
		else
		{
			Pin_OutL(PIN_TRIP2);//���������,�������ʹ����
		}
		#endif
	}
#endif//#if ((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

//GPRS LED
/*
	if(Terminal_Ram->GPRS_RxLED_MS_Timer==0)
	{
		Pin_OutL(PIN_DTR1_NSS2_SET);//���������,�������ʹ����
	}
	else
	{
		Pin_OutH(PIN_DTR1_NSS2_SET);//���������,�������ʹ����
	}
	if(Terminal_Ram->GPRS_TxLED_MS_Timer==0)
	{
		Pin_OutL(PIN_DSR1);//���������,�������ʹ����
	}
	else
	{
		Pin_OutH(PIN_DSR1);//���������,�������ʹ����
	}
*/
	
//ENET LED
	if(Terminal_Ram->ENETLED_MS_Timer==0)//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_ENET_ACT_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_ENET_ACT_LED);//���������,�������ʹ����
	}

#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//����485_1_LED(ԭ����485LED)
	//����
	if(Terminal_Ram->RS485_1_RxLED_MS_Timer==0)//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485R1_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485R1_LED);//���������,�������ʹ����
	}
	//����
	if(Terminal_Ram->RS485_1_TxLED_MS_Timer==0)//13 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485T1_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485T1_LED);//���������,�������ʹ����
	}
//����485_2_LED
	//����
	if(Terminal_Ram->RS485_2_RxLED_MS_Timer==0)//10 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485R2_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485R2_LED);//���������,�������ʹ����
	}
	//����
	if(Terminal_Ram->RS485_2_TxLED_MS_Timer==0)//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	{
		Pin_OutH(PIN_RS485T2_LED);//���������,�������ʹ����
	}
	else
	{
		Pin_OutL(PIN_RS485T2_LED);//���������,�������ʹ����
	}
#endif

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

