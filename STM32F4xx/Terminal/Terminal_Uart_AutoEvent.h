

#ifndef Terminal_Uart_AutoEvent_H
#define Terminal_Uart_AutoEvent_H

void Terminal_Uart_AutoEvent(void);//主动上报重要事件
void Terminal_AutoEvent(u32 PORTn);//主动上报重要事件,入口UART端口号
void Terminal_AutoEvent_Meter(u32 PORTn);//主动上报电能表事件,入口UART端口号

#endif


