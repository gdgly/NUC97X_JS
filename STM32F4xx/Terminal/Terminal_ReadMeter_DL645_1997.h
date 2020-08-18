

#ifndef Terminal_ReadMeter_DL645_1997_H
#define Terminal_ReadMeter_DL645_1997_H

void Terminal_ReadMeter_DL645_1997(u32 PORTn);//终端抄表DL645-2007
void Terminal_ReadMeter_DL645_1997_Fn(u32 PORTn,u8 *p8RMLib);//仅抄1个Fn>=129数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
void Terminal_ReadMeter_DL645_1997_KeyUser(u32 PORTn);//终端抄表DL645_1997重点户

#endif

