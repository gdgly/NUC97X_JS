


#ifndef BroadcastTime_H
#define BroadcastTime_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
void BroadcastTime_RS485(u32 PORTn);//广播校时-rs485;完成后置UARTCtrl->BroadcastTimeTask=0
void BroadcastTime_3762(u32 PORTn);//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif

