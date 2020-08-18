
#ifndef Terminal_ReadMeter_DL698_H
#define Terminal_ReadMeter_DL698_H


u32 DL698_Rx(u32 PORTn);//698规约帧接收;返回0没完成,1完成,2超时
u32 DL698_RxData(u32 PORTn);//698规约帧接收数据,比较发收地址、命令码;返回0正确,1地址错,2命令码错
void Terminal_ReadMeter_DL698(u32 PORTn);//终端抄表RS485口_上海规约
void Terminal_ReadMeter_DL698_Fn(u32 PORTn,u8 *p8RMLib);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)

#endif
