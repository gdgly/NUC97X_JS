


#ifndef DL698_FILE_H
#define DL698_FILE_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 GET_Request_F001(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
u32 ACTION_F001(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
u32 ACTION_F002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
	 

	 
	 

#ifdef __cplusplus
 }
#endif
#endif
