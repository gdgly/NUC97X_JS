

#ifndef DL698_GET_SET_ACTION_2000_H
#define DL698_GET_SET_ACTION_2000_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 GET_23xx_24xx(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//���ܼ�����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
u32 SET_23xx_24xx(u32 PORTn,u8* p8rx,u32 LEN_RxAPDU);//�����ܼ�����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
u32 ACTION_23xx_24xx(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//�����ܼ�����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
	 
	 

#ifdef __cplusplus
 }
#endif
#endif




