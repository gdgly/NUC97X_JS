

#ifndef Terminal_ReadMeter_WSH41_Fndata_H
#define Terminal_ReadMeter_WSH41_Fndata_H


void Terminal_ReadMeter_WSH41_TxData(u32 PORTn,u8 *p8RMLib);//终端威胜抄表发送数据
void ReadMeter_WSH41_FnData(u32 PORTn,u8 *p8RMLib);//威胜规约4.1版本抄表接收数据填入Fn数据缓冲
void ReadMeter_WSH41_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib);//威胜规约4.1版本抄表接收数据填入Fn数据缓冲


#endif
