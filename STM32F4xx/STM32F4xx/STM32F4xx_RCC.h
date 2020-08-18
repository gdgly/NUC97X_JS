

#ifndef STM32F4xx_RCC_H
#define STM32F4xx_RCC_H
#ifdef __cplusplus
 extern "C" {
#endif
	 

void RCC_SystemClock_Config(void);//ϵͳʱ������
uint32_t RCC_GetSysClockFreq(void);
uint32_t RCC_GetHCLKFreq(void);
uint32_t RCC_GetPCLK1Freq(void);
uint32_t RCC_GetPCLK2Freq(void);

	 
//NUC97x
#ifdef IC_NUC9xx
u32 GetClockMHz(CLK_Type clk);//NUC97x�õ�ʱ��Ƶ��MHz 
#endif 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
