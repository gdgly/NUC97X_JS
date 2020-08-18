

#ifndef CALC_RESET_FLOW_TIME_H
#define CALC_RESET_FLOW_TIME_H
#ifdef __cplusplus
 extern "C" {
#endif



void FLOW_Count(u32 PORTn);//通信流量计数
void POWER_Time(void);//供电时间
void RESET_Count(void);//终端复位次数
void CLR_RESET_FLOW_TIME(void);//清0终端复位次数_通信流量_供电时间
	 
	 
	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
