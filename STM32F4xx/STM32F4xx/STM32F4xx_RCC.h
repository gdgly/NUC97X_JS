

#ifndef STM32F4xx_RCC_H
#define STM32F4xx_RCC_H
#ifdef __cplusplus
 extern "C" {
#endif
	 

void RCC_SystemClock_Config(void);//系统时钟配置
uint32_t RCC_GetSysClockFreq(void);
uint32_t RCC_GetHCLKFreq(void);
uint32_t RCC_GetPCLK1Freq(void);
uint32_t RCC_GetPCLK2Freq(void);

	 
//NUC97x
#ifdef IC_NUC9xx
u32 GetClockMHz(CLK_Type clk);//NUC97x得到时钟频率MHz 
#endif 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
