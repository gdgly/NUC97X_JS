


#ifndef BroadcastTime_H
#define BroadcastTime_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
void BroadcastTime_RS485(u32 PORTn);//�㲥Уʱ-rs485;��ɺ���UARTCtrl->BroadcastTimeTask=0
void BroadcastTime_3762(u32 PORTn);//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif

