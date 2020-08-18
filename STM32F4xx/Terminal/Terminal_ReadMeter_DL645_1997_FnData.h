

#ifndef Terminal_ReadMeter_DL645_1997_Fndata_H
#define Terminal_ReadMeter_DL645_1997_Fndata_H


void Terminal_ReadMeter_DL645_1997_TxData(u32 PORTn,u8 *p8RMLib);//终端DL645-1997抄表发送数据
void ReadMeter_DL645_1997_FnData(u32 PORTn,u8 *p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
void ReadMeter_DL645_1997_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲

#endif
