

#ifndef STM32F4xx_TIM_H
#define STM32F4xx_TIM_H
#ifdef __cplusplus
 extern "C" {
#endif

	 

void START_STM32F4xx_TIM2(void);//启动定时器TIM2为时间测试定时器
u32 STOP_STM32F4xx_TIM2_US(void);//停止定时器TIM2为时间测试定时器，返回计数值(hex us)
u32 STOP_STM32F4xx_TIM2_MS(void);//停止定时器TIM2为时间测试定时器，返回计数值(hex ms)
	
void START_STM32F4xx_TIM3(void);//开UART6红外通信0.5BIT定时中断 
void STOP_STM32F4xx_TIM3(void);//关UART6红外通信0.5BIT定时中断
	 
	 
void START_STM32F4xx_TIM5_MEASURE_LSI(u32 TI4_RMP);//启动定时器TIM5为内部(LSI,LSE)时钟测量;入口:TI4_RMP=1(LSI),=2(LSE)
void START_STM32F4xx_TIM5_MEASURE_REF_CLK(void);//启动定时器TIM5测量LAN8720A的参考时钟

void Init_STM32F4xx_TIM6(u32 Hz);//初始化基本定时器TIM6为DAC TRGO 事件;入口:Hz=触发频率(TIM6为16位计数器,Max=1MHz/1,Min=1MHz/65535)
void Init_STM32F4xx_TIM7(void);//初始化基本定时器TIM7为1ms重复中断


void On38KHz(void);//开38KHz时钟(TIM12_CH1)
void Off38KHz(void);//关38KHz时钟(TIM12_CH1)


#ifdef __cplusplus
 }
#endif
#endif
