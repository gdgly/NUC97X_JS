
#ifndef DL698_GET_4100_H
#define DL698_GET_4100_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 Get_MeterSpec(void);//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
u32 GET_Request_4100(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR




#ifdef __cplusplus
 }
#endif
#endif
