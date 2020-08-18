
#ifndef Terminal_ReadMeter_WSH_H
#define Terminal_ReadMeter_WSH_H


u32 WSH41_Rx(u32 PORTn);//威胜接收;返回0没完成,1完成,2超时
u32 WSH41_RxDATA(u32 PORTn);//威胜查询应答命令接收数据;返回0应答正确，1表址错，2命令标志字错，3标志地址错，4数据长度错


#endif

