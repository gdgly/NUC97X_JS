
#ifndef RM_RS485_H
#define RM_RS485_H
#ifdef __cplusplus
 extern "C" {
#endif


void RS485_TxLED(u32 PORTn);//RS485 LED����ָʾ��
void RS485_RxTx(u32 PORTn);//RS485���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	 
void RM_RS485(u32 PORTn);//RS485����




#ifdef __cplusplus
 }
#endif
#endif
