

#ifndef Terminal_Uart_3761_COM_H

u32 Uart_376_Tp(u32 Tpaddr);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
u32 DItoFnPn(u32 PORTn);//数据标识分解为FnPn
void FnPntoDI(u32 DIaddr,u32 FnPn);//FnPn组合为数据标识
void Uart_376_ACK(u32 PORTn,u32 ACK);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,数据单元标识
void Uart_376_AUX(u32 PORTn,u32 rxTpaddr);//附加信息(EC,TP),暂等待单帧数据发送
void Uart_376_AUXTp(u32 PORTn,u32 rxTpaddr);//附加信息(Tp),暂等待单帧数据发送
void Uart_376_NoAUX(u32 PORTn);//没附加信息,暂等待单帧数据发送
u32 Uart_376_CS(u32 buffaddr);//和校验计算,同时加结束0x16;修改长度字节的标志位
void CopyDI(u32 PORTn,u32 DIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
u32 Uart_3761_Rx(u32 PORTn);//UART_3761接收;返回0没完成,1完成,2超时

u32 CheckUartData_0xEE(u32 DataAddr,u32 Byte);//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE

#endif

