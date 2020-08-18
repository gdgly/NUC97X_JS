

#ifndef Terminal698_Uart_H
#define Terminal698_Uart_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 Uart_698_HCS(u8 *p8);//֡ͷУ�����;����0=��ȷ��1=����
u32 Uart_698_HCSFCS(u8 *p8);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
u32 FrameProtocol(u8* p8,u32 Len);//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004
u32 Uart_698_Rx(u32 PORTn);//UART_698����;����0û���,1���,2��ʱ
void Uart_698_RxTx(u32 PORTn);//UART_698���շ��� 
void Uart_698_Data(u32 PORTn,u32 SECURITY);//UART_698����;���:��ȫģʽSECURITY=0������=1����

#define Rxoffset     64//ԭ����֡�ڽ��ջ�������ƫ���ֽ�
#define TxAPDUOffset    32//����APDU�ڷ��ͻ�������ƫ���ֽ�
u32 Uart_TSA_Comp(u8 *p8rxbuff,u8 *pFLASH);//ͨ�ŵ�ַ�Ƚ�,pFLASH���ݶ���ADDR_128K_DDATABUFF�бȽ�,����:0=��ͬ,!=0����ͬ
u32 Uart_698_TxHead(u8 *p8txbuff,u32 C,u32 CA);//Uart698֡ͷ���뷢�ͻ�����,���:C=������,CA=�ͻ�����ַ;����:֡ͷ����
void Uart_698_TxResponse(u32 PORTn,u32 TI,u32 LEN_TxAPDU);//DL698������Ӧ����,��������p8txbuff+TxAPDUOffset�Ŀ�ʼ��ַ��
u32 Uart_TimeTag(u8* p8rx);//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
