

#ifndef Terminal698_GET_H
#define Terminal698_GET_H
#ifdef __cplusplus
 extern "C" {
#endif


void GET_Request(u32 PORTn);
void GET_Request_Frame(u32 PORTn);//��֡��־	
u32 GET_OAD_Element(u32 PORTn,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ������Ԫ��;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
u32 GET_OAD_DATA(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR



u32 GET_RecordOAD(u32 PORTn,u8 *p8rx);//��һ����¼�Ͷ�������;����:b31-b16����ROAD����,b15-b0����ROAD�������ֽ���,���ݷ��ʽ��UARTCtrl->DAR
u32 GET_RecordOAD_Next(u32 PORTn);//��һ����¼�Ͷ������Է�֡��Ӧ����һ֡;���أ�LEN_TxAPDU
	 

	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
