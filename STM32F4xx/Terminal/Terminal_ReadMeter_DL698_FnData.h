#ifndef Terminal_ReadMeter_DL698_FnData_H
#define Terminal_ReadMeter_DL698_FnData_H

void Terminal_ReadMeter_DL698_TxData(u32 PORTn,u8 *p8RMLib);//终端DL698表规约抄表发送数据
void ReadMeter_DL698_FnData(u32 PORTn,u8 *p8RMLib);//上海规约表抄表接收数据填入Fn数据缓冲
void ReadMeter_DL698_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib);//上海规约表抄表接收数据填入Fn数据缓冲
#endif
