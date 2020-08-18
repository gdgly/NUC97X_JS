

#ifndef STM32F4xx_DMA_H
#define STM32F4xx_DMA_H
#ifdef __cplusplus
 extern "C" {
#endif


void DMA_UARTn_TX(u32 PORTn,u32 ADDR_USART_DR,u32 ADDR_TXBUFF,u32 Byte);//UART启动DMA发送
u32 Wait_DMA_UARTn_TX(u32 PORTn);//等待UART的DMA发送,返回0=发送完成,1=发送没完成
void DMA_UARTn_RX(u32 PORTn,u32 ADDR_USART_DR,u32 ADDR_RXBUFF,u32 LEN_BUFF);//UART启动DMA接收
void DMA_UARTn_RX_Pointer(u32 PORTn);//计算DMA_UARTn接收指针值(p0)

	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
