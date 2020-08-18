

#ifndef STM32F4xx_TIM_H
#define STM32F4xx_TIM_H
#ifdef __cplusplus
 extern "C" {
#endif

	 

void START_STM32F4xx_TIM2(void);//������ʱ��TIM2Ϊʱ����Զ�ʱ��
u32 STOP_STM32F4xx_TIM2_US(void);//ֹͣ��ʱ��TIM2Ϊʱ����Զ�ʱ�������ؼ���ֵ(hex us)
u32 STOP_STM32F4xx_TIM2_MS(void);//ֹͣ��ʱ��TIM2Ϊʱ����Զ�ʱ�������ؼ���ֵ(hex ms)
	
void START_STM32F4xx_TIM3(void);//��UART6����ͨ��0.5BIT��ʱ�ж� 
void STOP_STM32F4xx_TIM3(void);//��UART6����ͨ��0.5BIT��ʱ�ж�
	 
	 
void START_STM32F4xx_TIM5_MEASURE_LSI(u32 TI4_RMP);//������ʱ��TIM5Ϊ�ڲ�(LSI,LSE)ʱ�Ӳ���;���:TI4_RMP=1(LSI),=2(LSE)
void START_STM32F4xx_TIM5_MEASURE_REF_CLK(void);//������ʱ��TIM5����LAN8720A�Ĳο�ʱ��

void Init_STM32F4xx_TIM6(u32 Hz);//��ʼ��������ʱ��TIM6ΪDAC TRGO �¼�;���:Hz=����Ƶ��(TIM6Ϊ16λ������,Max=1MHz/1,Min=1MHz/65535)
void Init_STM32F4xx_TIM7(void);//��ʼ��������ʱ��TIM7Ϊ1ms�ظ��ж�


void On38KHz(void);//��38KHzʱ��(TIM12_CH1)
void Off38KHz(void);//��38KHzʱ��(TIM12_CH1)


#ifdef __cplusplus
 }
#endif
#endif
