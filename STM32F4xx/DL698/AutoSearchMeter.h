

#ifndef AutoSearchMeter_H
#define AutoSearchMeter_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
void AutoSearchMeter_RS485(u32 PORTn);//自动搜表任务-rs485:0=空,1=等待,2...过程
void AutoSearchMeter_3762(u32 PORTn);//自动搜表3762
void AutoSearchMeter_3762_TC(u32 PORTn);//自动搜表3762-鼎信
void AutoSearchMeter_3762_ES(u32 PORTn);//自动搜表3762-东软
void AutoSearchMeter_3762_01(u32 PORTn);//自动搜表3762-瑞斯康
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
