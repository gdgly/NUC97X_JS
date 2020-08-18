

#ifndef Terminal_PowerDown_H
#define Terminal_PowerDown_H


void Terminal_PowerDown(void);//2=电网供电转电池供电
u32 VoltageComp(u32 V);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值



#endif
