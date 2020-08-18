
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


u32 PORTn_NUC97xUARTn(u32 PORTn);//通信口号对应NUC97x的通道号;返回:NUC97x的通道号,255表示无对应
void Init_UARTn(u32 PORTn);//初始化UART口,入口通信控制字已填入
void Start_UARTn_Tx(u32 PORTn);//启动UART口发送
u32 Wait_UARTn_Tx(u32 PORTn);//等待UART口发送,返回0=发送完成,1=发送没完成
	 

#ifdef __cplusplus
 }
#endif
#endif
