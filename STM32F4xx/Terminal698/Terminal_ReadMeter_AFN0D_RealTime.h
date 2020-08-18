

#ifndef Terminal_ReadMeter_AFN0D_RealTime_H
#define Terminal_ReadMeter_AFN0D_RealTime_H


u32 ReadMeter_AFN0D_RealTime(u32 Fn,u32 Pn,u64 YMDHM);//实时读取2类数据;返回0=没读,!=0读且已写入FLASH返回是FnDataBuff地址


#endif
