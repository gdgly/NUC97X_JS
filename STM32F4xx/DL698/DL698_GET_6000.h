
#ifndef DL698_GET_6000_H
#define DL698_GET_6000_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 GET_NUMper(DL698_SETLIB_TypeDef *pLib);//���㵱ǰԪ�ظ���
u32 GET_Request_6000(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
u32 GET_Request_6002(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
	 



#ifdef __cplusplus
 }
#endif
#endif
