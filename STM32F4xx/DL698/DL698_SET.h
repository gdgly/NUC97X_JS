

#ifndef Terminal698_Set_H
#define Terminal698_Set_H
#ifdef __cplusplus
 extern "C" {
#endif


void SET_Request(u32 PORTn);
u32 SET_OAD_Element(u32 PORTn,u8 *p8rx,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD);//����һ������Ԫ��;����:��Ԫ�ص����ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
u32 SET_OAD(u32 PORTn,u8 *p8rxbuff,u32 LEN_RxAPDU);//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR


#ifdef __cplusplus
 }
#endif
#endif
