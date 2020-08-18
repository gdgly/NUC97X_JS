#ifndef Terminal_ReadMeter_DL698_Lib_H
#define Terminal_ReadMeter_DL698_Lib_H

u32 Get_RM_DL698_MaxFn(void);
u32 Get_RM_DL698_FnCount(u32 Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
#endif
