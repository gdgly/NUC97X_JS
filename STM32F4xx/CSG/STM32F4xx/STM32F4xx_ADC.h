

#ifndef STM32F4xx_ADC_H
#define STM32F4xx_ADC_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
u32 ADCx(u32 Channel,u32 mul,u32 div,u32 ADDR_RAM);//ADC;���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)ת��ʧ��)
void RTC_Battery_Voltage(void);//RTC��ص�ѹADC����
void Charge_Battery_Voltage(void);//�ɳ��4.8V��ص�ѹADC����
void GPRS_Voltage(void);//GPRS��Դ��ѹADC����
	
void STM32F4xx_Temperature(void);//STM32F4xx�����ı����¶�

#ifdef __cplusplus
 }
#endif 
#endif

