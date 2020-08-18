
#ifndef Terminal_ReadMeter_WSH41_H
#define Terminal_ReadMeter_WSH41_H


void Terminal_ReadMeter_WSH41(u32 PORTn);//终端抄表RS485口_威胜规约4.1版本
void Terminal_ReadMeter_WSH41_Fn(u32 PORTn,u8 *p8RMLib);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)


#endif


