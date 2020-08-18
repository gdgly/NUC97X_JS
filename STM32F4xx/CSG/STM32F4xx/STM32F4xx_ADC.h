

#ifndef STM32F4xx_ADC_H
#define STM32F4xx_ADC_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
u32 ADCx(u32 Channel,u32 mul,u32 div,u32 ADDR_RAM);//ADC;入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)转换失败)
void RTC_Battery_Voltage(void);//RTC电池电压ADC测量
void Charge_Battery_Voltage(void);//可充电4.8V电池电压ADC测量
void GPRS_Voltage(void);//GPRS电源电压ADC测量
	
void STM32F4xx_Temperature(void);//STM32F4xx测量的表内温度

#ifdef __cplusplus
 }
#endif 
#endif

