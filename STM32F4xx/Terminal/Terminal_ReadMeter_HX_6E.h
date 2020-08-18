

#ifndef Terminal_ReadMeter_HX_6E_H
#define Terminal_ReadMeter_HX_6E_H


void Terminal_ReadMeter_HX_6E(u32 PORTn);//终端抄表DL645-2007
void Terminal_ReadMeter_HX_6E_Fn(u32 PORTn,u8 *p8RMLib);//仅抄1个Fn>=129数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)



#endif
