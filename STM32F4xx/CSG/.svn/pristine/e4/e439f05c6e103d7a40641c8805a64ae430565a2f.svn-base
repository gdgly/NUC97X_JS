

#ifndef STM32F4xxRTC_H
#define STM32F4xxRTC_H
#ifdef __cplusplus
 extern "C" {
#endif

void Init_STM32F4xx_RTC(void);//内部RTC初始化
void Read_STM32F4xx_RTC(u32 ADDR_BUFF);//读STM32F4xx内部RTC
u32 Read_STM32F4xx_RTC_SS(void);//读STM32F4xx内部RTC的亚秒值计算成ms
void Write_STM32F4xx_RTC(u32 ADDR_BUFF);//写STM32F4xx内部RTC	 
	 
void SyncERTC(void);//同步外部RTC(读外部写内部)
void SyncIRTC(void);//同步内部RTC(读内部写外部)


#ifdef __cplusplus
 }
#endif 
#endif
