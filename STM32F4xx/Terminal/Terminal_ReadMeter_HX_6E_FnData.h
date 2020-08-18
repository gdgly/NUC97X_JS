

#ifndef Terminal_ReadMeter_HX_6E_FnData_H
#define Terminal_ReadMeter_HX_6E_FnData_H


void Terminal_ReadMeter_HX_6E_TxData(u32 PORTn,u8 *p8RMLib);//终端DL645-2007抄表发送数据
void ReadMeter_HX_6E_FnData(u32 PORTn,u8 *p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
void ReadMeter_HX_6E_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲

#endif
