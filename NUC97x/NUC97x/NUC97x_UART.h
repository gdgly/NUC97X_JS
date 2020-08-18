
#ifndef NUC97x_UART_H
#define NUC97x_UART_H
#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
	u32 DATA;
	u32 IER;
	u32 FCR;
	u32 LCR;
	u32 MCR;
	u32 MSR;
	u32 FSR;
	u32 ISR;
	u32 TOR;
	u32 BAUD;
	u32 IRCR;
	u32 ALT_CSR;
	u32 FUN_SEL;
	u32 LIN_CTL;
	u32 LIN_SR;
}UARTx_TypeDef;


u32 PORTn_NUC97xUARTn(u32 PORTn);//ͨ�ſںŶ�ӦNUC97x��ͨ����;����:NUC97x��ͨ����,255��ʾ�޶�Ӧ
void Init_UARTn(u32 PORTn);//��ʼ��UART��,���ͨ�ſ�����������
void Start_UARTn_Tx(u32 PORTn);//����UART�ڷ���
u32 Wait_UARTn_Tx(u32 PORTn);//�ȴ�UART�ڷ���,����0=�������,1=����û���
	 

#ifdef __cplusplus
 }
#endif
#endif
