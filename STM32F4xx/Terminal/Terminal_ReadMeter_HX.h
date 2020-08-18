
#ifndef Terminal_ReadMeter_HX_H
#define Terminal_ReadMeter_HX_H

//extern void Uart_HX_RxTx(u32 PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
extern void Read_HX_6E_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
extern u32 HX_Rx(u32 PORTn);//DL645帧接收;返回0没完成,1完成,2超时
extern u32 HX_6E_RxData(u32 PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错

#endif
