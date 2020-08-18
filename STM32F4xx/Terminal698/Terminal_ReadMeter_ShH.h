#ifndef Terminal_ReadMeter_ShH_H
#define Terminal_ReadMeter_ShH_H


u32 ShH_Rx(u32 PORTn);//上海规约帧接收;返回0没完成,1完成,2超时
u32 ShH_RxData(u32 PORTn);//上海规约帧接收数据,比较发收地址、命令码、减0x33;返回0正确,1地址错,2命令码错
void Terminal_ReadMeter_ShH(u32 PORTn);//终端抄表RS485口_上海规约
void Terminal_ReadMeter_ShH_Fn(u32 PORTn,u8 *p8RMLib);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
void Read_ShH_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//上海规约读数据帧,入口:要填入的缓冲地址,数据标识,通信地址


#endif
