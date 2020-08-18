

#ifndef STM32F4xx_RCC_H
#define STM32F4xx_RCC_H
#ifdef __cplusplus
 extern "C" {
#endif
	 

void RCC_SystemClock_Config(void);//œµÕ≥ ±÷”≈‰÷√
uint32_t RCC_GetSysClockFreq(void);
uint32_t RCC_GetHCLKFreq(void);
uint32_t RCC_GetPCLK1Freq(void);
uint32_t RCC_GetPCLK2Freq(void);

#ifdef __cplusplus
 }
#endif 
#endif
