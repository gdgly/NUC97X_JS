
#ifndef Terminal698_ACTION_H
#define Terminal698_ACTION_H
#ifdef __cplusplus
 extern "C" {
#endif



void ACTION_Request(u32 PORTn);
u32 ACTION(u32 PORTn,u8 *p8rxbuff,u32 LEN_RxAPDU,u8 *p8txbuff,u32 LENmax_TxSPACE);//����һ�����󷽷�;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
