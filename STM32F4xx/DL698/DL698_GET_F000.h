

#ifndef DL698_GET_F000_H
#define DL698_GET_F000_H
#ifdef __cplusplus
 extern "C" {
#endif




u32 GET_Request_F200(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
u32 GET_Request_FF10(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
u32 Get_Request_4500(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR



#ifdef __cplusplus
 }
#endif
#endif
