

#ifndef STM32F4xx_DMA_H
#define STM32F4xx_DMA_H
#ifdef __cplusplus
 extern "C" {
#endif


void DMA_UARTn_TX(u32 PORTn,u32 ADDR_USART_DR,u32 ADDR_TXBUFF,u32 Byte);//UART����DMA����
u32 Wait_DMA_UARTn_TX(u32 PORTn);//�ȴ�UART��DMA����,����0=�������,1=����û���
void DMA_UARTn_RX(u32 PORTn,u32 ADDR_USART_DR,u32 ADDR_RXBUFF,u32 LEN_BUFF);//UART����DMA����
void DMA_UARTn_RX_Pointer(u32 PORTn);//����DMA_UARTn����ָ��ֵ(p0)

	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
