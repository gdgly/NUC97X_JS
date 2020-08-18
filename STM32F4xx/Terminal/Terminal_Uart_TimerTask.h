

#ifndef Terminal_Uart_TimerTask_H
#define Terminal_Uart_TimerTask_H



void Terminal_Uart_AutoTask(void);//主动上报定时任务
void Terminal_TimerTask(u32 PORTn);//主动上报定时任务,入口UART端口号
void Uart_376_CON(u32 PORTn);//主动定时任务CON标志位和功能码



#endif

