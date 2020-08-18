

#ifndef Terminal_ReadMeter_DL645_H
#define Terminal_ReadMeter_DL645_H

void Uart_DL645_RxTx(u32 PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
void DL645_68H_Add33H_CS_16H(u32 TxBuffAddr);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
void Read_DL645_2007_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
void Read_DL645_2007_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr,u32 Frames);//DL645-2007读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址,后续帧号
void DL645_2007_TxData_04001503(u32 BuffAddr,u64 CommAddr);//(总长36字节)DL645-2007写数据帧复位主动上报状态字,入口:要填入的缓冲地址,通信地址
void ReadMeter_DL645_2007_TxCurveData(u32 BuffAddr,u32 DI,u64 CommAddr,u64 YMDHM);//DL645-2007读负荷曲线数据帧
void Read_DL645_1997_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
void Read_DL645_1997_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-1997读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址
u32 DL645_Rx(u32 PORTn);//DL645帧接收;返回0没完成,1完成,2超时
u32 DL645_2007_RxData(u32 PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
u32 DL645_1997_RxData(u32 PORTn);//DL645_1997帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
void Terminal_BroadcastTime_DL645(u32 PORTn);//DL645广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0

#endif

