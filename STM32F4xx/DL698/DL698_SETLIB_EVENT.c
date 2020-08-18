

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../Device/MEMRW.h"
#include "../MS/MS.h"
#include "../Display/Display.h"





#if USER<200//盲样电科院送检系,标准系
#include "../DL698/DL698_SETInit_Event_USER0xx.c"
#endif
#if (USER/100)==9//河南系
#include "../DL698/DL698_SETInit_Event_USER9xx.c"
#endif
#if (USER/100)==14//福建
#include "../DL698/DL698_SETInit_Event_USER14xx.c"
#endif
#if (USER/100)==12//四川系
#include "../DL698/DL698_SETInit_Event_USER12xx.c"
#endif
#if (USER/100)==15//安徽系
#include "../DL698/DL698_SETInit_Event_USER15xx.c"
#endif
#if (USER/100)==16//内蒙系
#include "../DL698/DL698_SETInit_Event_USER16xx.c"
#endif
#if (USER/100)==17//江苏系
#include "../DL698/DL698_SETInit_Event_USER17xx.c"
#endif






__align(4) const u8 DL698_Attribute_3000_5[]=
{
	5,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//电压触发上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  电压恢复下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  电流触发下限  double-long（单位：A，换算：-4），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3000[]=//电能表失压事件
{
	0x3000,//对象标识
	ADDR_3000_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3000_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3000_5,//数据类型列表指针
	(u32*)InitData_3000_5,//初始化原始数据指针
	sizeof(InitData_3000_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3000,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3000,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3000),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3001_5[]=
{
	3,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3001[]=//电能表欠压事件
{
	0x3001,//对象标识
	ADDR_3001_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3001_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3001_5,//数据类型列表指针
	(u32*)InitData_3001_5,//初始化原始数据指针
	sizeof(InitData_3001_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3001,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3001,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3001),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3002_5[]=
{
	3,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3002[]=//电能表过压事件
{
	0x3002,//对象标识
	ADDR_3002_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3002_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3002_5,//数据类型列表指针
	(u32*)InitData_3002_5,//初始化原始数据指针
	sizeof(InitData_3002_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3002,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3002,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3002),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3003_5[]=
{
	4,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  电流触发上限  double-long（单位：A，换算：-4），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3003[]=//电能表断相事件
{
	0x3003,//对象标识
	ADDR_3003_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3003_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3003_5,//数据类型列表指针
	(u32*)InitData_3003_5,//初始化原始数据指针
	sizeof(InitData_3003_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3003,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3003,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3003),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	ADDR_EventCurrentvalue_3003,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3004_5[]=
{
	5,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  电流触发上限  double-long（单位：A，换算：-4），
	DataType_double_long,
//  电流恢复下限  double-long（单位：A，换算：-4），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3004[]=//电能表失流事件
{
	0x3004,//对象标识
	ADDR_3004_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3004_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3004_5,//数据类型列表指针
	(u32*)InitData_3004_5,//初始化原始数据指针
	sizeof(InitData_3004_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3004,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3004,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3004),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3005_5[]=
{
	3,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  电流触发下限  double-long（单位：A，换算：-4），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3005[]=//电能表过流事件
{
	0x3005,//对象标识
	ADDR_3005_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3005_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3005_5,//数据类型列表指针
	(u32*)InitData_3005_5,//初始化原始数据指针
	sizeof(InitData_3005_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3005,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3005,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3005),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3006_5[]=
{
	4,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//  电流触发上限  double-long（单位：A，换算：-4），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3006[]=//电能表断流事件
{
	0x3006,//对象标识
	ADDR_3006_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3006_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3006_5,//数据类型列表指针
	(u32*)InitData_3006_5,//初始化原始数据指针
	sizeof(InitData_3006_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3006,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3006,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3006),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
};

__align(4) const u8 DL698_Attribute_3007_5[]=
{
	3,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  有功功率触发下限  double-long（单位：W，换算：-1），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3007[]=//电能表潮流反向事件
{
	0x3007,//对象标识
	ADDR_3007_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3007_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3007_5,//数据类型列表指针
	(u32*)InitData_3007_5,//初始化原始数据指针
	sizeof(InitData_3007_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3007,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3007,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3007),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3008_5[]=
{
	3,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  有功功率触发下限  double-long（单位：W，换算：-1），
	DataType_double_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3008[]=//电能表过载事件
{
	0x3008,//对象标识
	ADDR_3008_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3008_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3008_5,//数据类型列表指针
	(u32*)InitData_3008_5,//初始化原始数据指针
	sizeof(InitData_3008_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_3008,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3008,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3008),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3009_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  触发下限  double-long-unsigned（单位：kW，换算：-4），
	DataType_double_long_unsigned,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3009[]=//电能表正向有功需量超限事件
{
	0x3009,//对象标识
	ADDR_3009_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3009_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3009_6,//数据类型列表指针
	(u32*)InitData_3009_6,//初始化原始数据指针
	sizeof(InitData_3009_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3009,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3009,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3009),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_300A_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  触发下限  double-long-unsigned（单位：kW，换算：-4），
	DataType_double_long_unsigned,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_300A[]=//电能表反向有功需量超限事件
{
	0x300A,//对象标识
	ADDR_300A_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_300A_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_300A_6,//数据类型列表指针
	(u32*)InitData_300A_6,//初始化原始数据指针
	sizeof(InitData_300A_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_300A,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_300A,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_300A),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_300B_5[]=
{
	3,//类型列表数
//属性5（配置参数）∷=structure
	DataType_structure,
//{
//  触发下限 double-long-unsigned（单位：kvar，换算：-4），
	DataType_double_long_unsigned,
//  判定延时时间 unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_300B[]=//电能表无功需量超限事件
{
	0x300B,//对象标识
	ADDR_300B_5,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_300B_5,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_300B_5,//数据类型列表指针
	(u32*)InitData_300B_5,//初始化原始数据指针
	sizeof(InitData_300B_5),//初始化原始数据长度
	
	24,//接口类
  ADDR_300B,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_300B,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_300B),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_300C_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  下限阀值  long（单位：%，换算：-1），
	DataType_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_300C[]=//电能表功率因数超下限事件
{
	0x300C,//对象标识
	ADDR_300C_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_300C_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_300C_6,//数据类型列表指针
	(u32*)InitData_300C_6,//初始化原始数据指针
	sizeof(InitData_300C_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_300C,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_300C,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_300C),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_300D_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_300D[]=//电能表全失压事件
{
	0x300D,//对象标识
	ADDR_300D_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_300D_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_300D_6,//数据类型列表指针
	(u32*)InitData_300D_6,//初始化原始数据指针
	sizeof(InitData_300D_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_300D,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_300D,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_300D),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_300E_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_300E[]=//电能表辅助电源掉电事件
{
	0x300E,//对象标识
	ADDR_300E_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_300E_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_300E_6,//数据类型列表指针
	(u32*)InitData_300E_6,//初始化原始数据指针
	sizeof(InitData_300E_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_300E,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_300E,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_300E),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_300F_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_300F[]=//电能表电压逆相序事件
{
	0x300F,//对象标识
	ADDR_300F_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_300F_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_300F_6,//数据类型列表指针
	(u32*)InitData_300F_6,//初始化原始数据指针
	sizeof(InitData_300F_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_300F,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_300F,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_300F),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3010_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3010[]=//电能表电流逆相序事件
{
	0x3010,//对象标识
	ADDR_3010_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3010_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3010_6,//数据类型列表指针
	(u32*)InitData_3010_6,//初始化原始数据指针
	sizeof(InitData_3010_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3010,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3010,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3010),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3011_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3011[]=//电能表掉电事件
{
	0x3011,//对象标识
	ADDR_3011_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3011_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3011_6,//数据类型列表指针
	(u32*)InitData_3011_6,//初始化原始数据指针
	sizeof(InitData_3011_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3011,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3011,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3011),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3012_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3012[]=//电能表编程事件
{
	0x3012,//对象标识
	ADDR_3012_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3012_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3012_6,//数据类型列表指针
	(u32*)InitData_3012_6,//初始化原始数据指针
	sizeof(InitData_3012_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3012,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3012,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3012),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3013_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3013[]=//电能表清零事件
{
	0x3013,//对象标识
	ADDR_3013_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3013_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3013_6,//数据类型列表指针
	(u32*)InitData_3013_6,//初始化原始数据指针
	sizeof(InitData_3013_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3013,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3013,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3013),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3014_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3014[]=//电能表需量清零事件
{
	0x3014,//对象标识
	ADDR_3014_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3014_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3014_6,//数据类型列表指针
	(u32*)InitData_3014_6,//初始化原始数据指针
	sizeof(InitData_3014_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3014,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3014,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3014),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3015_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3015[]=//电能表事件清零事件
{
	0x3015,//对象标识
	ADDR_3015_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3015_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3015_6,//数据类型列表指针
	(u32*)InitData_3015_6,//初始化原始数据指针
	sizeof(InitData_3015_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3015,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3015,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3015),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3016_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3016[]=//电能表校时事件
{
	0x3016,//对象标识
	ADDR_3016_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3016_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3016_6,//数据类型列表指针
	(u32*)InitData_3016_6,//初始化原始数据指针
	sizeof(InitData_3016_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3016,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3016,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3016),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3017_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3017[]=//电能表时段表编程事件
{
	0x3017,//对象标识
	ADDR_3017_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3017_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3017_6,//数据类型列表指针
	(u32*)InitData_3017_6,//初始化原始数据指针
	sizeof(InitData_3017_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3017,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3017,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3017),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3018_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3018[]=//电能表时区表编程事件
{
	0x3018,//对象标识
	ADDR_3018_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3018_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3018_6,//数据类型列表指针
	(u32*)InitData_3018_6,//初始化原始数据指针
	sizeof(InitData_3018_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3018,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3018,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3018),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3019_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3019[]=//电能表周休日编程事件
{
	0x3019,//对象标识
	ADDR_3019_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3019_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3019_6,//数据类型列表指针
	(u32*)InitData_3019_6,//初始化原始数据指针
	sizeof(InitData_3019_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3019,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3019,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3019),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_301A_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_301A[]=//电能表结算日编程事件
{
	0x301A,//对象标识
	ADDR_301A_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_301A_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_301A_6,//数据类型列表指针
	(u32*)InitData_301A_6,//初始化原始数据指针
	sizeof(InitData_301A_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_301A,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_301A,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_301A),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_301B_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_301B[]=//电能表开盖事件
{
	0x301B,//对象标识
	ADDR_301B_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_301B_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_301B_6,//数据类型列表指针
	(u32*)InitData_301B_6,//初始化原始数据指针
	sizeof(InitData_301B_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_301B,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_301B,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_301B),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_301C_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_301C[]=//电能表开端钮盒事件
{
	0x301C,//对象标识
	ADDR_301C_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_301C_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_301C_6,//数据类型列表指针
	(u32*)InitData_301C_6,//初始化原始数据指针
	sizeof(InitData_301C_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_301C,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_301C,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_301C),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_301D_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  限值  long（单位：%，换算：-2），
	DataType_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_301D[]=//电能表电压不平衡事件
{
	0x301D,//对象标识
	ADDR_301D_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_301D_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_301D_6,//数据类型列表指针
	(u32*)InitData_301D_6,//初始化原始数据指针
	sizeof(InitData_301D_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_301D,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_301D,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_301D),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_301E_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  限值  long（单位：%，换算：-2），
	DataType_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_301E[]=//电能表电流不平衡事件
{
	0x301E,//对象标识
	ADDR_301E_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_301E_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_301E_6,//数据类型列表指针
	(u32*)InitData_301E_6,//初始化原始数据指针
	sizeof(InitData_301E_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_301E,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_301E,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_301E),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_301F_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_301F[]=//电能表跳闸事件
{
	0x301F,//对象标识
	ADDR_301F_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_301F_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_301F_6,//数据类型列表指针
	(u32*)InitData_301F_6,//初始化原始数据指针
	sizeof(InitData_301F_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_301F,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_301F,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_301F),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3020_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3020[]=//电能表合闸事件
{
	0x3020,//对象标识
	ADDR_3020_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3020_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3020_6,//数据类型列表指针
	(u32*)InitData_3020_6,//初始化原始数据指针
	sizeof(InitData_3020_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3020,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3020,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3020),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3021_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3021[]=//电能表节假日编程事件
{
	0x3021,//对象标识
	ADDR_3021_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3021_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3021_6,//数据类型列表指针
	(u32*)InitData_3021_6,//初始化原始数据指针
	sizeof(InitData_3021_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3021,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3021,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3021),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3022_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3022[]=//电能表有功组合方式编程事件
{
	0x3022,//对象标识
	ADDR_3022_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3022_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3022_6,//数据类型列表指针
	(u32*)InitData_3022_6,//初始化原始数据指针
	sizeof(InitData_3022_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3022,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3022,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3022),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3023_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3023[]=//电能表无功组合方式编程事件
{
	0x3023,//对象标识
	ADDR_3023_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3023_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3023_6,//数据类型列表指针
	(u32*)InitData_3023_6,//初始化原始数据指针
	sizeof(InitData_3023_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3023,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3023,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3023),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3024_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3024[]=//电能表费率参数表编程事件
{
	0x3024,//对象标识
	ADDR_3024_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3024_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3024_6,//数据类型列表指针
	(u32*)InitData_3024_6,//初始化原始数据指针
	sizeof(InitData_3024_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3024,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3024,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3024),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3025_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3025[]=//电能表阶梯表编程事件
{
	0x3025,//对象标识
	ADDR_3025_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3025_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3025_6,//数据类型列表指针
	(u32*)InitData_3025_6,//初始化原始数据指针
	sizeof(InitData_3025_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3025,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3025,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3025),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3026_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3026[]=//电能表密钥更新事件
{
	0x3026,//对象标识
	ADDR_3026_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3026_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3026_6,//数据类型列表指针
	(u32*)InitData_3026_6,//初始化原始数据指针
	sizeof(InitData_3026_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3026,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3026,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3026),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3027_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3027[]=//电能表异常插卡事件
{
	0x3027,//对象标识
	ADDR_3027_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3027_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3027_6,//数据类型列表指针
	(u32*)InitData_3027_6,//初始化原始数据指针
	sizeof(InitData_3027_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3027,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3027,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3027),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3028_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3028[]=//电能表购电记录
{
	0x3028,//对象标识
	ADDR_3028_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3028_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3028_6,//数据类型列表指针
	(u32*)InitData_3028_6,//初始化原始数据指针
	sizeof(InitData_3028_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3028,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3028,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3028),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_3029_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3029[]=//电能表退费记录
{
	0x3029,//对象标识
	ADDR_3029_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3029_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3029_6,//数据类型列表指针
	(u32*)InitData_3029_6,//初始化原始数据指针
	sizeof(InitData_3029_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3029,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3029,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3029),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_302A_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_302A[]=//电能表恒定磁场干扰事件
{
	0x302A,//对象标识
	ADDR_302A_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_302A_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_302A_6,//数据类型列表指针
	(u32*)InitData_302A_6,//初始化原始数据指针
	sizeof(InitData_302A_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_302A,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_302A,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_302A),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_302B_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_302B[]=//电能表负荷开关误动作事件
{
	0x302B,//对象标识
	ADDR_302B_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_302B_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_302B_6,//数据类型列表指针
	(u32*)InitData_302B_6,//初始化原始数据指针
	sizeof(InitData_302B_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_302B,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_302B,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_302B),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_302C_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_302C[]=//电能表电源异常事件
{
	0x302C,//对象标识
	ADDR_302C_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_302C_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_302C_6,//数据类型列表指针
	(u32*)InitData_302C_6,//初始化原始数据指针
	sizeof(InitData_302C_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_302C,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_302C,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_302C),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_302D_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  限值  long（单位：%，换算：-2）
	DataType_long,
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_302D[]=//电能表电流严重不平衡事件
{
	0x302D,//对象标识
	ADDR_302D_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_302D_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_302D_6,//数据类型列表指针
	(u32*)InitData_302D_6,//初始化原始数据指针
	sizeof(InitData_302D_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_302D,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_302D,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_302D),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_302E_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_302E[]=//电能表时钟故障事件
{
	0x302E,//对象标识
	ADDR_302E_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_302E_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_302E_6,//数据类型列表指针
	(u32*)InitData_302E_6,//初始化原始数据指针
	sizeof(InitData_302E_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_302E,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_302E,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_302E),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

__align(4) const u8 DL698_Attribute_302F_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_302F[]=//电能表计量芯片故障事件
{
	0x302F,//对象标识
	ADDR_302F_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_302F_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_302F_6,//数据类型列表指针
	(u32*)InitData_302F_6,//初始化原始数据指针
	sizeof(InitData_302F_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_302F,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_302F,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_302F),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3030_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3030[]=//通信模块变更事件
{
	0x3030,//对象标识
	ADDR_3030_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3030_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3030_6,//数据类型列表指针
	(u32*)InitData_3030_6,//初始化原始数据指针
	sizeof(InitData_3030_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3030,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3030,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3030),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3100_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3100[]=//终端初始化事件
{
	0x3100,//对象标识
	ADDR_3100_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3100_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3100_6,//数据类型列表指针
	(u32*)InitData_3100_6,//初始化原始数据指针
	sizeof(InitData_3100_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3100,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3100,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3100),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3101_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3101[]=//终端版本变更事件
{
	0x3101,//对象标识
	ADDR_3101_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3101_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3101_6,//数据类型列表指针
	(u32*)InitData_3101_6,//初始化原始数据指针
	sizeof(InitData_3101_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3101,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3101,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3101),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3104_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3104[]=//终端状态量变位事件
{
	0x3104,//对象标识
	ADDR_3104_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3104_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3104_6,//数据类型列表指针
	(u32*)InitData_3104_6,//初始化原始数据指针
	sizeof(InitData_3104_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3104,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3104,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3104),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3105_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//异常判别阈值  long-unsigned（单位：秒），
	DataType_long_unsigned,
//关联采集任务号 unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3105[]=//电能表时钟超差事件	
{
	0x3105,//对象标识
	ADDR_3105_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3105_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3105_6,//数据类型列表指针
	(u32*)InitData_3105_6,//初始化原始数据指针
	sizeof(InitData_3105_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3105,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3105,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3105),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3106_6[]=
{
	14+1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//停电数据采集配置参数  structure
	DataType_structure,
//{
//采集标志  bit-string(SIZE(8))，
	DataType_bit_string,
//停电事件抄读时间间隔（小时） unsigned ，
	DataType_unsigned,
//停电事件抄读时间限值（分钟） unsigned，
	DataType_unsigned,
//需要读取停电事件电能表       array TSA
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_TSA,
//}，
//停电事件甄别限值参数  structure
	DataType_structure,
//{
//停电时间最小有效间隔（分钟） long-unsigned，
	DataType_long_unsigned,
//停电时间最大有效间隔（分钟） long-unsigned，
	DataType_long_unsigned,
//停电事件起止时间偏差限值（分钟） long-unsigned，
	DataType_long_unsigned,
//停电事件时间区段偏差限值（分钟） long-unsigned，
	DataType_long_unsigned,
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
//}
//}
//采集标志：
//bit0：置“1”有效，置“0”无效；
//bit1：置“1”随机选择测量点，置“0”只采集设置的测量点。
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3106[]=//终端停/上电事件	
{
	0x3106,//对象标识
	ADDR_3106_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3106_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3106_6,//数据类型列表指针
	(u32*)InitData_3106_6,//初始化原始数据指针
	sizeof(InitData_3106_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3106,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3106,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3106),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3107_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  直流模拟量上限  double-long
	DataType_double_long,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3107[]=//终端直流模拟量越上限事件	
{
	0x3107,//对象标识
	ADDR_3107_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3107_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3107_6,//数据类型列表指针
	(u32*)InitData_3107_6,//初始化原始数据指针
	sizeof(InitData_3107_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3107,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3107,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3107),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3108_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  直流模拟量下限  double-long
	DataType_double_long,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3108[]=//终端直流模拟量越下限事件	
{
	0x3108,//对象标识
	ADDR_3108_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3108_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3108_6,//数据类型列表指针
	(u32*)InitData_3108_6,//初始化原始数据指针
	sizeof(InitData_3108_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3108,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3108,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3108),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3109_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3109[]=//终端消息认证错误事件	
{
	0x3109,//对象标识
	ADDR_3109_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3109_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3109_6,//数据类型列表指针
	(u32*)InitData_3109_6,//初始化原始数据指针
	sizeof(InitData_3109_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3109,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3109,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3109),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_310A_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_310A[]=//设备故障记录	
{
	0x310A,//对象标识
	ADDR_310A_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_310A_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_310A_6,//数据类型列表指针
	(u32*)InitData_310A_6,//初始化原始数据指针
	sizeof(InitData_310A_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_310A,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_310A,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_310A),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_310B_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  关联采集任务号 unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_310B[]=//电能表示度下降事件	
{
	0x310B,//对象标识
	ADDR_310B_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_310B_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_310B_6,//数据类型列表指针
	(u32*)InitData_310B_6,//初始化原始数据指针
	sizeof(InitData_310B_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_310B,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_310B,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_310B),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_310C_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  阈值  double-long-unsigned（单位：%，无换算）,
	DataType_double_long_unsigned,
//  关联采集任务号 unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_310C[]=//电能量超差事件	
{
	0x310C,//对象标识
	ADDR_310C_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_310C_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_310C_6,//数据类型列表指针
	(u32*)InitData_310C_6,//初始化原始数据指针
	sizeof(InitData_310C_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_310C,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_310C,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_310C),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_310D_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  阈值  double-long-unsigned（单位：%，无换算）,
	DataType_double_long_unsigned,
//  关联采集任务号 unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_310D[]=//电能表飞走事件	
{
	0x310D,//对象标识
	ADDR_310D_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_310D_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_310D_6,//数据类型列表指针
	(u32*)InitData_310D_6,//初始化原始数据指针
	sizeof(InitData_310D_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_310D,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_310D,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_310D),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_310E_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  阈值           TI,
	DataType_TI,
//  关联采集任务号 unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_310E[]=//电能表停走事件	
{
	0x310E,//对象标识
	ADDR_310E_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_310E_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_310E_6,//数据类型列表指针
	(u32*)InitData_310E_6,//初始化原始数据指针
	sizeof(InitData_310E_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_310E,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_310E,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_310E),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_310F_6[]=
{
	3,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  重试轮次      unsigned,
	DataType_unsigned,
//    关联采集任务号 unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_310F[]=//终端抄表失败事件	
{
	0x310F,//对象标识
	ADDR_310F_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_310F_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_310F_6,//数据类型列表指针
	(u32*)InitData_310F_6,//初始化原始数据指针
	sizeof(InitData_310F_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_310F,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_310F,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_310F),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3110_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  通信流量门限  double-long-unsigned（单位：byte）
	DataType_double_long_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3110[]=//月通信流量超限事件	
{
	0x3110,//对象标识
	ADDR_3110_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3110_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3110_6,//数据类型列表指针
	(u32*)InitData_3110_6,//初始化原始数据指针
	sizeof(InitData_3110_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3110,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3110,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3110),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3111_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3111[]=//发现未知电能表事件	
{
	0x3111,//对象标识
	ADDR_3111_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3111_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3111_6,//数据类型列表指针
	(u32*)InitData_3111_6,//初始化原始数据指针
	sizeof(InitData_3111_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3111,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3111,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3111),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3112_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3112[]=//跨台区电能表事件	
{
	0x3112,//对象标识
	ADDR_3112_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3112_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3112_6,//数据类型列表指针
	(u32*)InitData_3112_6,//初始化原始数据指针
	sizeof(InitData_3112_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3112,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3112,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3112),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3114_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3114[]=//终端对时事件	
{
	0x3114,//对象标识
	ADDR_3114_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3114_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3114_6,//数据类型列表指针
	(u32*)InitData_3114_6,//初始化原始数据指针
	sizeof(InitData_3114_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3114,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3114,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3114),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3115_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3115[]=//遥控跳闸记录	
{
	0x3115,//对象标识
	ADDR_3115_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3115_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3115_6,//数据类型列表指针
	(u32*)InitData_3115_6,//初始化原始数据指针
	sizeof(InitData_3115_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3115,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3115,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3115),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3116_6[]=
{
	8+1,//类型列表数
//属性6（配置参数）∷=array 有功总电能量差动组配置
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//有功总电能量差动组配置∷=structure
	DataType_structure,
//{
//  有功总电能量差动组序号 unsigned，
	DataType_unsigned,
//  对比的总加组           OI，
	DataType_OI,
//  参照的总加组           OI，
	DataType_OI,
//  参与差动的电能量的时间区间及对比方法标志 bit-string（SIZE（8）），
	DataType_bit_string,
//  差动越限相对偏差值 integer（单位：%，换算：0），
	DataType_integer,
//  差动越限绝对偏差值 long64（单位：kWh，换算：-4）
	DataType_long64,
//}
//参与差动的电能量的时间区间及对比方法标志：
//bit0～bit1编码表示电能量的时间跨度，取值范围0～2依次表示60分钟电量、30分钟电量、15分钟电量，其他值无效。
//bit7表示对比方法标志，置“0”：相对对比，公式见公式（1）；置“1”：绝对对比，公式见公式（2）。
//bit2～bit6备用。
//	 ％  .....	(1)
//	  .............	(2)
//式中：
//    Q——对比的总加组总电能量；
//    q——参照的总加组总电能量。
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3116[]=//有功总电能量差动越限事件记录	
{
	0x3116,//对象标识
	ADDR_3116_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3116_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3116_6,//数据类型列表指针
	(u32*)InitData_3116_6,//初始化原始数据指针
	sizeof(InitData_3116_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3116,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3116,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3116),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};

 __align(4) const u8 DL698_Attribute_3117_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3117[]=//输出回路接入状态变位事件记录	
{
	0x3117,//对象标识
	ADDR_3117_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3117_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3117_6,//数据类型列表指针
	(u32*)InitData_3117_6,//初始化原始数据指针
	sizeof(InitData_3117_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3117,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3117,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3117),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_3118_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3118[]=//终端编程记录	
{
	0x3118,//对象标识
	ADDR_3118_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3118_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3118_6,//数据类型列表指针
	(u32*)InitData_3118_6,//初始化原始数据指针
	sizeof(InitData_3118_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3118,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3118,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3118),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_3119_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3119[]=//终端电流回路异常事件	
{
	0x3119,//对象标识
	ADDR_3119_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3119_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3119_6,//数据类型列表指针
	(u32*)InitData_3119_6,//初始化原始数据指针
	sizeof(InitData_3119_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3119,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3119,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3119),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_311A_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//   判定延时时间  long-unsigned（单位：s，换算：0）
	DataType_long_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_311A[]=//电能表在网状态切换事件	
{
	0x311A,//对象标识
	ADDR_311A_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_311A_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_311A_6,//数据类型列表指针
	(u32*)InitData_311A_6,//初始化原始数据指针
	sizeof(InitData_311A_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_311A,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_311A,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_311A),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_311B_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_311B[]=//终端对电表校时记录	
{
	0x311B,//对象标识
	ADDR_311B_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_311B_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_311B_6,//数据类型列表指针
	(u32*)InitData_311B_6,//初始化原始数据指针
	sizeof(InitData_311B_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_311B,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_311B,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_311B),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_311C_6[]=
{
	2,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//  关联采集任务序号    unsigned
	DataType_unsigned,
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_311C[]=//电能表数据变更监控记录	
{
	0x311C,//对象标识
	ADDR_311C_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_311C_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_311C_6,//数据类型列表指针
	(u32*)InitData_311C_6,//初始化原始数据指针
	sizeof(InitData_311C_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_311C,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_311C,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_311C),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};


__align(4) const u8 DL698_Attribute_311F_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_311F[]=//拒绝从节点入网记录 	
{
	0x311F,//对象标识
	ADDR_311F_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_311F_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_311F_6,//数据类型列表指针
	(u32*)InitData_311F_6,//初始化原始数据指针
	sizeof(InitData_311F_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_311F,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_311F,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_311F),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};



 __align(4) const u8 DL698_Attribute_3200_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3200[]=//功控跳闸记录	
{
	0x3200,//对象标识
	ADDR_3200_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3200_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3200_6,//数据类型列表指针
	(u32*)InitData_3200_6,//初始化原始数据指针
	sizeof(InitData_3200_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3200,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3200,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3200),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_3201_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3201[]=//电控跳闸记录	
{
	0x3201,//对象标识
	ADDR_3201_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3201_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3201_6,//数据类型列表指针
	(u32*)InitData_3201_6,//初始化原始数据指针
	sizeof(InitData_3201_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3201,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3201,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3201),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

 __align(4) const u8 DL698_Attribute_3202_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3202[]=//购电参数设置记录	
{
	0x3202,//对象标识
	ADDR_3202_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3202_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3202_6,//数据类型列表指针
	(u32*)InitData_3202_6,//初始化原始数据指针
	sizeof(InitData_3202_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3202,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3202,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3202),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};

__align(4) const u8 DL698_Attribute_3203_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3203[]=//电控告警事件记录	
{
	0x3203,//对象标识
	ADDR_3203_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3203_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3203_6,//数据类型列表指针
	(u32*)InitData_3203_6,//初始化原始数据指针
	sizeof(InitData_3203_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3203,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3203,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3203),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无

};


//扩展
#if (USER/100)==9//河南
__align(4) const u8 DL698_Attribute_3411_6[]=
{
	1,//类型列表数
//属性6（配置参数）∷=structure
	DataType_structure,
//{
//}
};
__align(4) const DL698_EVENTLIB_TypeDef  DL698_EVENTLIB_3411[]=//电表接线错误事件记录	
{
	0x3411,//对象标识
	ADDR_3411_6,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3411_6,//配置参数FLASH空间中预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_3411_6,//数据类型列表指针
	(u32*)InitData_3411_6,//初始化原始数据指针
	sizeof(InitData_3411_6),//初始化原始数据长度
	
	7,//接口类
  ADDR_3411,//有效标识,上报标识,关联对象OAD地址
	(u32*)InitData_3411,//有效标识,上报标识,关联对象OAD初始化原始数据指针
	sizeof(InitData_3411),//有效标识,上报标识,关联对象OAD初始化原始数据长度
	0,//当前值记录表地址,0=无
	
};
#endif


__align(4) __align(4) const DL698_LIB_List_TypeDef  DL698_EVENTLIB_List[]=
{
	{(u32*)DL698_EVENTLIB_3000,},
	{(u32*)DL698_EVENTLIB_3001,},
	{(u32*)DL698_EVENTLIB_3002,},
	{(u32*)DL698_EVENTLIB_3003,},
	{(u32*)DL698_EVENTLIB_3004,},
	{(u32*)DL698_EVENTLIB_3005,},
	{(u32*)DL698_EVENTLIB_3006,},
	{(u32*)DL698_EVENTLIB_3007,},
	{(u32*)DL698_EVENTLIB_3008,},
	{(u32*)DL698_EVENTLIB_3009,},
	{(u32*)DL698_EVENTLIB_300A,},
	{(u32*)DL698_EVENTLIB_300B,},
	{(u32*)DL698_EVENTLIB_300C,},
	{(u32*)DL698_EVENTLIB_300D,},
	{(u32*)DL698_EVENTLIB_300E,},
	{(u32*)DL698_EVENTLIB_300F,},
	{(u32*)DL698_EVENTLIB_3010,},
	{(u32*)DL698_EVENTLIB_3011,},
	{(u32*)DL698_EVENTLIB_3012,},
	{(u32*)DL698_EVENTLIB_3013,},
	{(u32*)DL698_EVENTLIB_3014,},
	{(u32*)DL698_EVENTLIB_3015,},
	{(u32*)DL698_EVENTLIB_3016,},
	{(u32*)DL698_EVENTLIB_3017,},
	{(u32*)DL698_EVENTLIB_3018,},
	{(u32*)DL698_EVENTLIB_3019,},
	{(u32*)DL698_EVENTLIB_301A,},
	{(u32*)DL698_EVENTLIB_301B,},
	{(u32*)DL698_EVENTLIB_301C,},
	{(u32*)DL698_EVENTLIB_301D,},
	{(u32*)DL698_EVENTLIB_301E,},
	{(u32*)DL698_EVENTLIB_301F,},
	{(u32*)DL698_EVENTLIB_3020,},
	{(u32*)DL698_EVENTLIB_3021,},
	{(u32*)DL698_EVENTLIB_3022,},
	{(u32*)DL698_EVENTLIB_3023,},
	{(u32*)DL698_EVENTLIB_3024,},
	{(u32*)DL698_EVENTLIB_3025,},
	{(u32*)DL698_EVENTLIB_3026,},
	{(u32*)DL698_EVENTLIB_3027,},
	{(u32*)DL698_EVENTLIB_3028,},
	{(u32*)DL698_EVENTLIB_3029,},
	{(u32*)DL698_EVENTLIB_302A,},
	{(u32*)DL698_EVENTLIB_302B,},
	{(u32*)DL698_EVENTLIB_302C,},
	{(u32*)DL698_EVENTLIB_302D,},
	{(u32*)DL698_EVENTLIB_302E,},
	{(u32*)DL698_EVENTLIB_302F,},
	{(u32*)DL698_EVENTLIB_3030,},
	{(u32*)DL698_EVENTLIB_3100,},
	{(u32*)DL698_EVENTLIB_3101,},
	{(u32*)DL698_EVENTLIB_3104,},
	{(u32*)DL698_EVENTLIB_3105,},
	{(u32*)DL698_EVENTLIB_3106,},
	{(u32*)DL698_EVENTLIB_3107,},
	{(u32*)DL698_EVENTLIB_3108,},
	{(u32*)DL698_EVENTLIB_3109,},
	{(u32*)DL698_EVENTLIB_310A,},
	{(u32*)DL698_EVENTLIB_310B,},
	{(u32*)DL698_EVENTLIB_310C,},
	{(u32*)DL698_EVENTLIB_310D,},
	{(u32*)DL698_EVENTLIB_310E,},
	{(u32*)DL698_EVENTLIB_310F,},
	{(u32*)DL698_EVENTLIB_3110,},
	{(u32*)DL698_EVENTLIB_3111,},
	{(u32*)DL698_EVENTLIB_3112,},
	{(u32*)DL698_EVENTLIB_3114,},
	{(u32*)DL698_EVENTLIB_3115,},
	{(u32*)DL698_EVENTLIB_3116,},
	{(u32*)DL698_EVENTLIB_3117,},
	{(u32*)DL698_EVENTLIB_3118,},
	{(u32*)DL698_EVENTLIB_3119,},
	{(u32*)DL698_EVENTLIB_311A,},
	{(u32*)DL698_EVENTLIB_311B,},
	{(u32*)DL698_EVENTLIB_311C,},
	{(u32*)DL698_EVENTLIB_311F,},
	{(u32*)DL698_EVENTLIB_3200,},
	{(u32*)DL698_EVENTLIB_3201,},
	{(u32*)DL698_EVENTLIB_3202,},
	{(u32*)DL698_EVENTLIB_3203,},		
	
//扩展
#if (USER/100)==9//河南
	{(u32*)DL698_EVENTLIB_3411,},
#endif
	
	
};


DL698_EVENTLIB_TypeDef *Get_DL698_EVENT_pLib(u32 OAD)//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
{
	u32 i;
	DL698_EVENTLIB_TypeDef *pLib;

	OAD>>=16;
	for(i=0;i<sizeof(DL698_EVENTLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_EVENTLIB_TypeDef *)DL698_EVENTLIB_List[i].pLib;
		if(OAD==pLib->OI)
		{
			return pLib;
		}
	}
	return 0;
}


/*
__align(4) const u8 InitData_IC7_NUMrecord[]=
{
	DataType_long_unsigned,
	0,0,
};
__align(4) const u8 InitData_IC24_NUMrecord[]=
{
//当前记录数∷=structure
	DataType_structure,
	4,
//{
//  记录表1当前记录数 long-unsigned，
	DataType_long_unsigned,
	0,0,
//  记录表2当前记录数 long-unsigned，
	DataType_long_unsigned,
	0,0,
//  记录表3当前记录数 long-unsigned，
	DataType_long_unsigned,
	0,0,
//  记录表4当前记录数 long-unsigned，
	DataType_long_unsigned,
	0,0,
//}
};
__align(4) const u8 InitData_IC24_Value[]=
{
//当前值记录表∷=array 当前值
	DataType_array,
	4,
//当前值∷=structure
	DataType_structure,
	2,
//{
//事件发生次数 double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//事件累计时间 double-long-unsigned
	DataType_double_long_unsigned,
	0,0,0,0,
//}
//当前值∷=structure
	DataType_structure,
	2,
//{
//事件发生次数 double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//事件累计时间 double-long-unsigned
	DataType_double_long_unsigned,
	0,0,0,0,
//}
//当前值∷=structure
	DataType_structure,
	2,
//{
//事件发生次数 double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//事件累计时间 double-long-unsigned
	DataType_double_long_unsigned,
	0,0,0,0,
//}	
//当前值∷=structure
	DataType_structure,
	2,
//{
//事件发生次数 double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//事件累计时间 double-long-unsigned
	DataType_double_long_unsigned,
	0,0,0,0,
//}
};
*/


void DL698_SETINIT_EVENT(u32 Command,u8* pOAD)//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
{
	u32 i;
	u32 x;
	u32 n;
	u32 OAD;
	u8* p8;
	DL698_EVENTLIB_TypeDef *pLib;
	ms_Type *ms;
	u8* pSOUR;
	
	ms=Get_ms();
	pSOUR=ms->ms_malloc((ADDR_DL698_SET_END)-(ADDR_DL698_SET_START));//设置数据区总长度
	if(pSOUR)
	{//有内存分配
		//读原设置数据
		MR((u32)pSOUR,ADDR_DL698_SET_START,(ADDR_DL698_SET_END)-(ADDR_DL698_SET_START));
	}
	if(Command==6)
	{//6=仅部份参数初始化
		for(i=0;i<(sizeof(DL698_EVENTLIB_List)/sizeof(DL698_LIB_List_TypeDef));i++)
		{
			pLib=(DL698_EVENTLIB_TypeDef*)DL698_EVENTLIB_List[i].pLib;
			//仅部份参数初始化
			switch(pLib->OI)
			{
				//部份参数表
			#if (USER/100)==9//河南
				case 0x3106://停上电参数
				case 0x3112://跨台区电能表事件
				case 0x3411://电表接线错误事件
					break;
			#endif
				
				case 0xffff://空OI,仅编译时不警告
					break;
				default:
					continue;
			}
			
			if(pLib->pInitData)
			{
				if(pLib->LEN_Sour<=pLib->LENmax)
				{//原始数据长度<=FLASH预留的最大长度
					MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
				}
				else
				{
					Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					DisplayStringAutoUp(0,9,(u8*)"\x0""参数长度超限!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				}
			}
			//有效标识,上报标识,关联对象OAD初始化	
			if(pLib->pInitEventOAD)
			{
				if(pLib->LEN_EventOAD<=(4+LENmax_EventOAD))
				{
					MW((u32)pLib->pInitEventOAD,(u32)pLib->Addr_EventOAD,pLib->LEN_EventOAD);
				}
				else
				{
					Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					DisplayStringAutoUp(0,9,(u8*)"\x0""参数长度超限!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				}
			}
		}
		ms->ms_free(pSOUR);
		return;
	}
	for(i=0;i<(sizeof(DL698_EVENTLIB_List)/sizeof(DL698_LIB_List_TypeDef));i++)
	{
		pLib=(DL698_EVENTLIB_TypeDef*)DL698_EVENTLIB_List[i].pLib;
		//配置参数初始化
		if(pOAD)
		{//参数定义的OAD列表，保持现状，不在恢复之列
			p8=pOAD;//参数∷=array OAD
			n=p8[1];
			p8+=2;
			for(x=0;x<n;x++)
			{
				OAD=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
				if((OAD>>16)==pLib->OI)
				{
					break;
				}
				p8+=5;
			}
			if(x<n)
			{//有列表，用原设置数据
				if(pSOUR)
				{//有内存分配
					if(pLib->Addr)
					{
						n=pLib->Addr-(ADDR_DL698_SET_START);
						n+=(u32)pSOUR;
						MW(n,(u32)pLib->Addr,pLib->LENmax);
					}
				}
				continue;
			}
		}
		if(pLib->pInitData)
		{
			if(pLib->LEN_Sour<=pLib->LENmax)
			{//原始数据长度>FLASH预留的最大长度
				MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
			}
			else
			{
				Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""参数长度超限!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			}
		}
		//有效标识,上报标识,关联对象OAD初始化	
		if(pLib->pInitEventOAD)
		{
			if(pLib->LEN_EventOAD<=(4+LENmax_EventOAD))
			{
				MW((u32)pLib->pInitEventOAD,(u32)pLib->Addr_EventOAD,pLib->LEN_EventOAD);
			}
			else
			{
				Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""参数长度超限!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			}
		}
	}
	ms->ms_free(pSOUR);
}





