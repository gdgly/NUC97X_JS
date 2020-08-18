

#ifndef Terminal_ReadMeter_HX_6E_LIB_H  //红相电表相关的库文件
#define Terminal_ReadMeter_HX_6E_LIB_H  //红相电表相关的库文件


u32 Get_RM_HX_6E_MaxFn(void);
u32 Get_RM_HX_6E_FnCount(u32 Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
extern const u32 HX_6E_RMLib[];

#endif

