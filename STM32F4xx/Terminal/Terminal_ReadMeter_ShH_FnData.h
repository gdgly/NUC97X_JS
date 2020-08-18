

#ifndef Terminal_ReadMeter_ShH_Fndata_H
#define Terminal_ReadMeter_ShH_Fndata_H


void Terminal_ReadMeter_ShH_TxData(u32 PORTn,u8 *p8RMLib);//终端上海规约抄表发送数据
void ReadMeter_ShH_FnData(u32 PORTn,u8 *p8RMLib);//上海规约表抄表接收数据填入Fn数据缓冲
void ReadMeter_ShH_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib);//上海规约表抄表接收数据填入Fn数据缓冲


#endif
