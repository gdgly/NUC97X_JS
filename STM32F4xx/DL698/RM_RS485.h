
#ifndef RM_RS485_H
#define RM_RS485_H
#ifdef __cplusplus
 extern "C" {
#endif


void RS485_TxLED(u32 PORTn);//RS485 LED发送指示灯
void RS485_RxTx(u32 PORTn);//RS485接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	 
void RM_RS485(u32 PORTn);//RS485抄表




#ifdef __cplusplus
 }
#endif
#endif
