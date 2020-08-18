

#ifndef Terminal_ReadMeter_3762_Std_H
#define Terminal_ReadMeter_3762_Std_H



void GeneralReadMeter_3762_Std(u32 PORTn);//标准载波模块,常规抄表
void RealTimeReadMeter_3762_Fn_Std(u32 PORTn,u8 *p8RMLib);//仅抄1个Fn数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
void KeyUserReadMeter_3762_Std(u32 PORTn);//标准载波模块,重点户抄表



#endif


