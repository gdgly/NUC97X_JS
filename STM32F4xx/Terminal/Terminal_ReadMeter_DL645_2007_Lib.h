

#ifndef Terminal_ReadMeter_DL645_2007_Lib_H
#define Terminal_ReadMeter_DL645_2007_Lib_H


u32 Get_RM_DL645_2007_MaxFn(void);
u32 Get_RM_DL645_2007_FnCount(u32 Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
u32 DL645_2007_DI_To_RMCount(u32 DI);//主动上报抄读电能表的事件次数或1-10次记录的数据标识转为与周期抄读相同标识的的抄表计数;返回0xffff(65535)表示没找到

#endif

