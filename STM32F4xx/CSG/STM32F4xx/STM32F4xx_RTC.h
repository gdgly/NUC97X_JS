

#ifndef STM32F4xxRTC_H
#define STM32F4xxRTC_H
#ifdef __cplusplus
 extern "C" {
#endif

void Init_STM32F4xx_RTC(void);//�ڲ�RTC��ʼ��
void Read_STM32F4xx_RTC(u32 ADDR_BUFF);//��STM32F4xx�ڲ�RTC
u32 Read_STM32F4xx_RTC_SS(void);//��STM32F4xx�ڲ�RTC������ֵ�����ms
void Write_STM32F4xx_RTC(u32 ADDR_BUFF);//дSTM32F4xx�ڲ�RTC	 
	 
void SyncERTC(void);//ͬ���ⲿRTC(���ⲿд�ڲ�)
void SyncIRTC(void);//ͬ���ڲ�RTC(���ڲ�д�ⲿ)


#ifdef __cplusplus
 }
#endif 
#endif
