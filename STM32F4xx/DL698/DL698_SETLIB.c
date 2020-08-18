
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../Device/MEMRW.h"
#include "../MS/MS.h"
#include "../Display/Display.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"





//----集中器----
#if ((Project/100)==2)//集中器
#if USER<200//盲样电科院送检系,标准系
#include "../DL698/DL698_SETInit_USER0xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER0xx.c"
#endif

#if (USER/100)==9//河南系
#include "../DL698/DL698_SETInit_USER9xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER9xx.c"
#endif

#if (USER/100)==14//福建
#include "../DL698/DL698_SETInit_USER14xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER14xx.c"
#endif

#if (USER/100)==15//安徽系
#include "../DL698/DL698_SETInit_USER15xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER15xx.c"
#endif

#if (USER/100)==17//江苏系
#include "../DL698/DL698_SETInit_USER17xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER17xx.c"
#endif

#endif//#if ((Project/100)==2)//集中器
//----集中器 end----


//-----专变----
#if ((Project/100)==3)//专变
#if USER<200//盲样电科院送检系,标准系
#include "../DL698/DL698_SETInit_USER0xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER0xx.c"
#endif

#if (USER/100)==9//河南系
#include "../DL698/DL698_SETInit_USER9xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER9xx.c"
#endif
#if (USER/100)==12//四川系
#include "../DL698/DL698_SETInit_USER12xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER12xx.c"
#endif
#if (USER/100)==14//福建
#include "../DL698/DL698_SETInit_USER14xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER14xx.c"
#endif

#if (USER/100)==15//安徽系
#include "../DL698/DL698_SETInit_USER15xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER15xx.c"
#endif


#endif//#if ((Project/100)==3)//专变
//-----专变 end----


//----三合一终端----
#if ((Project/100)==4)//三合一终端
#if (USER/100)==16//内蒙系
#include "../DL698/DL698_SETInit_USER16xx.c"
#include "../DL698/DL698_SETInit_6000_Project4xx_USER16xx.c"
#endif


#endif//#if ((Project/100)==4)//三合一终端
//----三合一终端 end----




#include "../DL698/DL698_SETLIB_2000.c"
#include "../DL698/DL698_SETLIB_4000.c"
#include "../DL698/DL698_SETLIB_5000.c"
#include "../DL698/DL698_SETLIB_6000.c"
#include "../DL698/DL698_SETLIB_8000.c"
#include "../DL698/DL698_SETLIB_F000.c"


__align(4) const DL698_LIB_List_TypeDef  DL698_SETLIB_List[]=
{
//---通信类---
	{(u32*)DL698_SETLIB_4001,},
	{(u32*)DL698_SETLIB_4005,},
	{(u32*)DL698_SETLIB_4002,},
	{(u32*)DL698_SETLIB_4003,},
	{(u32*)DL698_SETLIB_4400_4,},
	{(u32*)DL698_SETLIB_4401,},
	{(u32*)DL698_SETLIB_4500_2,},
	{(u32*)DL698_SETLIB_4500_3,},
	{(u32*)DL698_SETLIB_4500_4,},
	{(u32*)DL698_SETLIB_4500_5,},
	{(u32*)DL698_SETLIB_4500_6,},
	{(u32*)DL698_SETLIB_4500_7,},
	{(u32*)DL698_SETLIB_4500_8,},
	{(u32*)DL698_SETLIB_4500_9,},
	{(u32*)DL698_SETLIB_4500_10,},
	{(u32*)DL698_SETLIB_4500_11,},
	{(u32*)DL698_SETLIB_4510_2,},
	{(u32*)DL698_SETLIB_4510_3,},
	{(u32*)DL698_SETLIB_4510_4,},
	{(u32*)DL698_SETLIB_4510_5,},
	{(u32*)DL698_SETLIB_4520_2,},//因放在扩展区故定义为非通信参数,否则非工厂时参数区会先清0但不初始化!


//---通用类---
	{(u32*)DL698_SETLIB_4000,},
	{(u32*)DL698_SETLIB_4000_3,},
	{(u32*)DL698_SETLIB_4000_4,},
	{(u32*)DL698_SETLIB_4004,},
	{(u32*)DL698_SETLIB_4006,},
	{(u32*)DL698_SETLIB_4007,},
	{(u32*)DL698_SETLIB_4008,},
	{(u32*)DL698_SETLIB_4009,},
	{(u32*)DL698_SETLIB_400A,},
	{(u32*)DL698_SETLIB_400B,},
	{(u32*)DL698_SETLIB_400C,},
	{(u32*)DL698_SETLIB_400D,},
	{(u32*)DL698_SETLIB_400E,},
	{(u32*)DL698_SETLIB_400F,},
	{(u32*)DL698_SETLIB_4010,},
	{(u32*)DL698_SETLIB_4011,},
	{(u32*)DL698_SETLIB_4012,},
	{(u32*)DL698_SETLIB_4013,},
	{(u32*)DL698_SETLIB_4014,},
	{(u32*)DL698_SETLIB_4015,},
	{(u32*)DL698_SETLIB_4016,},
	{(u32*)DL698_SETLIB_4017,},
	{(u32*)DL698_SETLIB_4018,},
	{(u32*)DL698_SETLIB_4019,},
	{(u32*)DL698_SETLIB_401A,},
	{(u32*)DL698_SETLIB_401B,},
	{(u32*)DL698_SETLIB_401C,},
	{(u32*)DL698_SETLIB_401D,},
	{(u32*)DL698_SETLIB_401E,},
	{(u32*)DL698_SETLIB_401F,},
	{(u32*)DL698_SETLIB_4020,},
	{(u32*)DL698_SETLIB_4021,},
	{(u32*)DL698_SETLIB_4022,},
	{(u32*)DL698_SETLIB_4023,},
	{(u32*)DL698_SETLIB_4024,},
	{(u32*)DL698_SETLIB_4030,},
	{(u32*)DL698_SETLIB_4100,},
	{(u32*)DL698_SETLIB_4101,},
	{(u32*)DL698_SETLIB_4102,},
	{(u32*)DL698_SETLIB_4103,},
	{(u32*)DL698_SETLIB_4104,},
	{(u32*)DL698_SETLIB_4105,},
	{(u32*)DL698_SETLIB_4106,},
	{(u32*)DL698_SETLIB_4107,},
	{(u32*)DL698_SETLIB_4108,},
	{(u32*)DL698_SETLIB_4109,},
	{(u32*)DL698_SETLIB_410A,},
	{(u32*)DL698_SETLIB_410B,},
	{(u32*)DL698_SETLIB_410C,},
	{(u32*)DL698_SETLIB_410D,},
	{(u32*)DL698_SETLIB_410E,},
	{(u32*)DL698_SETLIB_410F,},
	{(u32*)DL698_SETLIB_4111,},
	{(u32*)DL698_SETLIB_4112,},
	{(u32*)DL698_SETLIB_4113,},
	{(u32*)DL698_SETLIB_4114,},
	{(u32*)DL698_SETLIB_4115,},
	{(u32*)DL698_SETLIB_4116,},
	{(u32*)DL698_SETLIB_4117,},

	
//---终端类---
//4200	11	路由表
//4201	8	路由信息单元
	{(u32*)DL698_SETLIB_4202,},
	{(u32*)DL698_SETLIB_4204_2,},
	{(u32*)DL698_SETLIB_4204_3,},
	
//版本信息
	{(u32*)DL698_SETLIB_4300_2,},
	{(u32*)DL698_SETLIB_4300_3,},
	{(u32*)DL698_SETLIB_4300_4,},
	{(u32*)DL698_SETLIB_4300_5,},
	{(u32*)DL698_SETLIB_4300_6,},
	{(u32*)DL698_SETLIB_4300_7,},
	{(u32*)DL698_SETLIB_4300_8,},
	{(u32*)DL698_SETLIB_4300_9,},
	{(u32*)DL698_SETLIB_4300_10,},
	
//冻结类
	{(u32*)DL698_SETLIB_5000,},
	{(u32*)DL698_SETLIB_5001,},
	{(u32*)DL698_SETLIB_5002,},
	{(u32*)DL698_SETLIB_5003,},
	{(u32*)DL698_SETLIB_5004,},
	{(u32*)DL698_SETLIB_5005,},
	{(u32*)DL698_SETLIB_5006,},
	{(u32*)DL698_SETLIB_5007,},
	{(u32*)DL698_SETLIB_5008,},
	{(u32*)DL698_SETLIB_5009,},
	{(u32*)DL698_SETLIB_500A,},
	{(u32*)DL698_SETLIB_500B,},
	{(u32*)DL698_SETLIB_5011,},
	
//采集监控
	{(u32*)DL698_SETLIB_6000_2_0,},//定位第0个采集档案地址，不能删除
	{(u32*)DL698_SETLIB_6000_2_1,},
	{(u32*)DL698_SETLIB_6000_2_2,},
	{(u32*)DL698_SETLIB_6000_2_3,},
	{(u32*)DL698_SETLIB_6000_2_4,},
	{(u32*)DL698_SETLIB_6000_2_5,},
	{(u32*)DL698_SETLIB_6000_2_6,},
	{(u32*)DL698_SETLIB_6000_2_7,},
	{(u32*)DL698_SETLIB_6000_2_8,},
	{(u32*)DL698_SETLIB_6000_2_9,},

	{(u32*)DL698_SETLIB_6000_3,},
	{(u32*)DL698_SETLIB_6000_4,},
	
	{(u32*)DL698_SETLIB_6002_2,},//仅使库中OAD有效
	{(u32*)DL698_SETLIB_6002_3,},//仅使库中OAD有效
	{(u32*)DL698_SETLIB_6002_4,},//仅使库中OAD有效
	{(u32*)DL698_SETLIB_6002_5,},//仅使库中OAD有效
	{(u32*)DL698_SETLIB_6002_6,},//所有搜表结果记录数(仅使库中OAD有效)
	{(u32*)DL698_SETLIB_6002_7,},//跨台区搜表结果记录数(仅使库中OAD有效)
	{(u32*)DL698_SETLIB_6002_8,},
	{(u32*)DL698_SETLIB_6002_9,},
	{(u32*)DL698_SETLIB_6002_10,},


	{(u32*)DL698_SETLIB_6012_2_0,},//定位第0个地址，不能删除
#ifdef InitData_6012_2_1_
	{(u32*)DL698_SETLIB_6012_2_1,},
#endif
#ifdef InitData_6012_2_2_
	{(u32*)DL698_SETLIB_6012_2_2,},
#endif
#ifdef InitData_6012_2_3_
	{(u32*)DL698_SETLIB_6012_2_3,},
#endif
#ifdef InitData_6012_2_4_
	{(u32*)DL698_SETLIB_6012_2_4,},
#endif
#ifdef InitData_6012_2_5_
	{(u32*)DL698_SETLIB_6012_2_5,},
#endif
#ifdef InitData_6012_2_6_
	{(u32*)DL698_SETLIB_6012_2_6,},
#endif
#ifdef InitData_6012_2_7_
	{(u32*)DL698_SETLIB_6012_2_7,},
#endif
#ifdef InitData_6012_2_8_
	{(u32*)DL698_SETLIB_6012_2_8,},
#endif
#ifdef InitData_6012_2_9_
	{(u32*)DL698_SETLIB_6012_2_9,},
#endif
#ifdef InitData_6012_2_10_
	{(u32*)DL698_SETLIB_6012_2_10,},
#endif
#ifdef InitData_6012_2_11_
	{(u32*)DL698_SETLIB_6012_2_11,},
#endif
#ifdef InitData_6012_2_12_
	{(u32*)DL698_SETLIB_6012_2_12,},
#endif
#ifdef InitData_6012_2_13_
	{(u32*)DL698_SETLIB_6012_2_13,},
#endif
#ifdef InitData_6012_2_14_
	{(u32*)DL698_SETLIB_6012_2_14,},
#endif
#ifdef InitData_6012_2_15_
	{(u32*)DL698_SETLIB_6012_2_15,},
#endif
#ifdef InitData_6012_2_16_
	{(u32*)DL698_SETLIB_6012_2_16,},
#endif
#ifdef InitData_6012_2_17_
	{(u32*)DL698_SETLIB_6012_2_17,},
#endif
#ifdef InitData_6012_2_18_
	{(u32*)DL698_SETLIB_6012_2_18,},
#endif
#ifdef InitData_6012_2_19_
	{(u32*)DL698_SETLIB_6012_2_19,},
#endif
#ifdef InitData_6012_2_20_
	{(u32*)DL698_SETLIB_6012_2_20,},
#endif
#ifdef InitData_6012_2_21_
	{(u32*)DL698_SETLIB_6012_2_21,},
#endif
#ifdef InitData_6012_2_22_
	{(u32*)DL698_SETLIB_6012_2_22,},
#endif
#ifdef InitData_6012_2_23_
	{(u32*)DL698_SETLIB_6012_2_23,},
#endif
#ifdef InitData_6012_2_24_
	{(u32*)DL698_SETLIB_6012_2_24,},
#endif
#ifdef InitData_6012_2_25_
	{(u32*)DL698_SETLIB_6012_2_25,},
#endif
#ifdef InitData_6012_2_26_
	{(u32*)DL698_SETLIB_6012_2_26,},
#endif
#ifdef InitData_6012_2_27_
	{(u32*)DL698_SETLIB_6012_2_27,},
#endif
#ifdef InitData_6012_2_28_
	{(u32*)DL698_SETLIB_6012_2_28,},
#endif
#ifdef InitData_6012_2_29_
	{(u32*)DL698_SETLIB_6012_2_29,},
#endif
#ifdef InitData_6012_2_30_
	{(u32*)DL698_SETLIB_6012_2_30,},
#endif
#ifdef InitData_6012_2_31_
	{(u32*)DL698_SETLIB_6012_2_31,},
#endif
#ifdef InitData_6012_2_32_
	{(u32*)DL698_SETLIB_6012_2_32,},
#endif
#ifdef InitData_6012_2_33_
	{(u32*)DL698_SETLIB_6012_2_33,},
#endif
#ifdef InitData_6012_2_34_
	{(u32*)DL698_SETLIB_6012_2_34,},
#endif
#ifdef InitData_6012_2_35_
	{(u32*)DL698_SETLIB_6012_2_35,},
#endif
#ifdef InitData_6012_2_36_
	{(u32*)DL698_SETLIB_6012_2_36,},
#endif
#ifdef InitData_6012_2_37_
	{(u32*)DL698_SETLIB_6012_2_37,},
#endif
#ifdef InitData_6012_2_38_
	{(u32*)DL698_SETLIB_6012_2_38,},
#endif
#ifdef InitData_6012_2_39_
	{(u32*)DL698_SETLIB_6012_2_39,},
#endif
#ifdef InitData_6012_2_40_
	{(u32*)DL698_SETLIB_6012_2_40,},
#endif
#ifdef InitData_6012_2_41_
	{(u32*)DL698_SETLIB_6012_2_41,},
#endif
#ifdef InitData_6012_2_42_
	{(u32*)DL698_SETLIB_6012_2_42,},
#endif
#ifdef InitData_6012_2_43_
	{(u32*)DL698_SETLIB_6012_2_43,},
#endif
#ifdef InitData_6012_2_44_
	{(u32*)DL698_SETLIB_6012_2_44,},
#endif
#ifdef InitData_6012_2_45_
	{(u32*)DL698_SETLIB_6012_2_45,},
#endif
#ifdef InitData_6012_2_46_
	{(u32*)DL698_SETLIB_6012_2_46,},
#endif
#ifdef InitData_6012_2_47_
	{(u32*)DL698_SETLIB_6012_2_47,},
#endif
#ifdef InitData_6012_2_48_
	{(u32*)DL698_SETLIB_6012_2_48,},
#endif
#ifdef InitData_6012_2_49_
	{(u32*)DL698_SETLIB_6012_2_49,},
#endif
#ifdef InitData_6012_2_50_
	{(u32*)DL698_SETLIB_6012_2_50,},
#endif
#ifdef InitData_6012_2_51_
	{(u32*)DL698_SETLIB_6012_2_51,},
#endif
#ifdef InitData_6012_2_52_
	{(u32*)DL698_SETLIB_6012_2_52,},
#endif
#ifdef InitData_6012_2_53_
	{(u32*)DL698_SETLIB_6012_2_53,},
#endif
#ifdef InitData_6012_2_54_
	{(u32*)DL698_SETLIB_6012_2_54,},
#endif
#ifdef InitData_6012_2_55_
	{(u32*)DL698_SETLIB_6012_2_55,},
#endif
#ifdef InitData_6012_2_56_
	{(u32*)DL698_SETLIB_6012_2_56,},
#endif
#ifdef InitData_6012_2_57_
	{(u32*)DL698_SETLIB_6012_2_57,},
#endif
#ifdef InitData_6012_2_58_
	{(u32*)DL698_SETLIB_6012_2_58,},
#endif
#ifdef InitData_6012_2_59_
	{(u32*)DL698_SETLIB_6012_2_59,},
#endif
#ifdef InitData_6012_2_60_
	{(u32*)DL698_SETLIB_6012_2_60,},
#endif
#ifdef InitData_6012_2_61_
	{(u32*)DL698_SETLIB_6012_2_61,},
#endif	
#ifdef InitData_6012_2_62_
	{(u32*)DL698_SETLIB_6012_2_62,},
#endif
#ifdef InitData_6012_2_63_
	{(u32*)DL698_SETLIB_6012_2_63,},
#endif
#ifdef InitData_6012_2_64_
	{(u32*)DL698_SETLIB_6012_2_64,},
#endif
#ifdef InitData_6012_2_65_
	{(u32*)DL698_SETLIB_6012_2_65,},
#endif
#ifdef InitData_6012_2_66_
	{(u32*)DL698_SETLIB_6012_2_66,},
#endif
#ifdef InitData_6012_2_67_
	{(u32*)DL698_SETLIB_6012_2_67,},
#endif
#ifdef InitData_6012_2_68_
	{(u32*)DL698_SETLIB_6012_2_68,},
#endif
#ifdef InitData_6012_2_69_
	{(u32*)DL698_SETLIB_6012_2_69,},
#endif
#ifdef InitData_6012_2_70_
	{(u32*)DL698_SETLIB_6012_2_70,},
#endif
#ifdef InitData_6012_2_71_
	{(u32*)DL698_SETLIB_6012_2_71,},
#endif
#ifdef InitData_6012_2_72_
	{(u32*)DL698_SETLIB_6012_2_72,},
#endif
#ifdef InitData_6012_2_73_
	{(u32*)DL698_SETLIB_6012_2_73,},
#endif
#ifdef InitData_6012_2_74_
	{(u32*)DL698_SETLIB_6012_2_74,},
#endif
#ifdef InitData_6012_2_75_
	{(u32*)DL698_SETLIB_6012_2_75,},
#endif
#ifdef InitData_6012_2_76_
	{(u32*)DL698_SETLIB_6012_2_76,},
#endif
#ifdef InitData_6012_2_77_
	{(u32*)DL698_SETLIB_6012_2_77,},
#endif
#ifdef InitData_6012_2_78_
	{(u32*)DL698_SETLIB_6012_2_78,},
#endif
#ifdef InitData_6012_2_79_
	{(u32*)DL698_SETLIB_6012_2_79,},
#endif
#ifdef InitData_6012_2_80_
	{(u32*)DL698_SETLIB_6012_2_80,},
#endif
#ifdef InitData_6012_2_81_
	{(u32*)DL698_SETLIB_6012_2_81,},
#endif
#ifdef InitData_6012_2_82_
	{(u32*)DL698_SETLIB_6012_2_82,},
#endif
#ifdef InitData_6012_2_83_
	{(u32*)DL698_SETLIB_6012_2_83,},
#endif
#ifdef InitData_6012_2_84_
	{(u32*)DL698_SETLIB_6012_2_84,},
#endif	
#ifdef InitData_6012_2_85_
	{(u32*)DL698_SETLIB_6012_2_85,},
#endif
#ifdef InitData_6012_2_86_
	{(u32*)DL698_SETLIB_6012_2_86,},
#endif
#ifdef InitData_6012_2_87_
	{(u32*)DL698_SETLIB_6012_2_87,},
#endif
#ifdef InitData_6012_2_88_
	{(u32*)DL698_SETLIB_6012_2_88,},
#endif
#ifdef InitData_6012_2_89_
	{(u32*)DL698_SETLIB_6012_2_89,},
#endif
#ifdef InitData_6012_2_90_
	{(u32*)DL698_SETLIB_6012_2_90,},
#endif
#ifdef InitData_6012_2_91_
	{(u32*)DL698_SETLIB_6012_2_91,},
#endif
#ifdef InitData_6012_2_92_
	{(u32*)DL698_SETLIB_6012_2_92,},
#endif
#ifdef InitData_6012_2_93_
	{(u32*)DL698_SETLIB_6012_2_93,},
#endif
#ifdef InitData_6012_2_94_
	{(u32*)DL698_SETLIB_6012_2_94,},
#endif
#ifdef InitData_6012_2_95_
	{(u32*)DL698_SETLIB_6012_2_95,},
#endif
#ifdef InitData_6012_2_96_
	{(u32*)DL698_SETLIB_6012_2_96,},
#endif
#ifdef InitData_6012_2_97_
	{(u32*)DL698_SETLIB_6012_2_97,},
#endif
#ifdef InitData_6012_2_98_
	{(u32*)DL698_SETLIB_6012_2_98,},
#endif
#ifdef InitData_6012_2_99_
	{(u32*)DL698_SETLIB_6012_2_99,},
#endif
#ifdef InitData_6012_2_100_
	{(u32*)DL698_SETLIB_6012_2_100,},
#endif
#ifdef InitData_6012_2_101_
	{(u32*)DL698_SETLIB_6012_2_101,},
#endif
#ifdef InitData_6012_2_102_
	{(u32*)DL698_SETLIB_6012_2_102,},
#endif
#ifdef InitData_6012_2_103_
	{(u32*)DL698_SETLIB_6012_2_103,},
#endif
#ifdef InitData_6012_2_104_
	{(u32*)DL698_SETLIB_6012_2_104,},
#endif
#ifdef InitData_6012_2_105_
	{(u32*)DL698_SETLIB_6012_2_105,},
#endif
#ifdef InitData_6012_2_106_
	{(u32*)DL698_SETLIB_6012_2_106,},
#endif
#ifdef InitData_6012_2_107_
	{(u32*)DL698_SETLIB_6012_2_107,},
#endif
#ifdef InitData_6012_2_108_
	{(u32*)DL698_SETLIB_6012_2_108,},
#endif
#ifdef InitData_6012_2_109_
	{(u32*)DL698_SETLIB_6012_2_109,},
#endif
#ifdef InitData_6012_2_110_
	{(u32*)DL698_SETLIB_6012_2_110,},
#endif
#ifdef InitData_6012_2_111_
	{(u32*)DL698_SETLIB_6012_2_111,},
#endif
#ifdef InitData_6012_2_112_
	{(u32*)DL698_SETLIB_6012_2_112,},
#endif
#ifdef InitData_6012_2_113_
	{(u32*)DL698_SETLIB_6012_2_113,},
#endif
#ifdef InitData_6012_2_114_
	{(u32*)DL698_SETLIB_6012_2_114,},
#endif
#ifdef InitData_6012_2_115_
	{(u32*)DL698_SETLIB_6012_2_115,},
#endif
#ifdef InitData_6012_2_116_
	{(u32*)DL698_SETLIB_6012_2_116,},
#endif
#ifdef InitData_6012_2_117_
	{(u32*)DL698_SETLIB_6012_2_117,},
#endif
#ifdef InitData_6012_2_118_
	{(u32*)DL698_SETLIB_6012_2_118,},
#endif
#ifdef InitData_6012_2_119_
	{(u32*)DL698_SETLIB_6012_2_119,},
#endif
#ifdef InitData_6012_2_120_
	{(u32*)DL698_SETLIB_6012_2_120,},
#endif
#ifdef InitData_6012_2_121_
	{(u32*)DL698_SETLIB_6012_2_121,},
#endif
#ifdef InitData_6012_2_122_
	{(u32*)DL698_SETLIB_6012_2_122,},
#endif
#ifdef InitData_6012_2_123_
	{(u32*)DL698_SETLIB_6012_2_123,},
#endif
#ifdef InitData_6012_2_124_
	{(u32*)DL698_SETLIB_6012_2_124,},
#endif
#ifdef InitData_6012_2_125_
	{(u32*)DL698_SETLIB_6012_2_125,},
#endif
#ifdef InitData_6012_2_126_
	{(u32*)DL698_SETLIB_6012_2_126,},
#endif
#ifdef InitData_6012_2_127_
	{(u32*)DL698_SETLIB_6012_2_127,},
#endif
#ifdef InitData_6012_2_128_
	{(u32*)DL698_SETLIB_6012_2_128,},
#endif
		
#if NUMmax_6012_2>(128+1)//最大抄表任务数
	#error
#endif
	
	{(u32*)DL698_SETLIB_6014_2_0,},//定位第0个地址，不能删除
#ifdef InitData_6014_2_1_
	{(u32*)DL698_SETLIB_6014_2_1,},
#endif
#ifdef InitData_6014_2_2_
	{(u32*)DL698_SETLIB_6014_2_2,},
#endif
#ifdef InitData_6014_2_3_
	{(u32*)DL698_SETLIB_6014_2_3,},
#endif
#ifdef InitData_6014_2_4_
	{(u32*)DL698_SETLIB_6014_2_4,},
#endif
#ifdef InitData_6014_2_5_
	{(u32*)DL698_SETLIB_6014_2_5,},
#endif
#ifdef InitData_6014_2_6_
	{(u32*)DL698_SETLIB_6014_2_6,},
#endif
#ifdef InitData_6014_2_7_
	{(u32*)DL698_SETLIB_6014_2_7,},
#endif
#ifdef InitData_6014_2_8_
	{(u32*)DL698_SETLIB_6014_2_8,},
#endif
#ifdef InitData_6014_2_9_
	{(u32*)DL698_SETLIB_6014_2_9,},
#endif
#ifdef InitData_6014_2_10_
	{(u32*)DL698_SETLIB_6014_2_10,},
#endif
#ifdef InitData_6014_2_11_
	{(u32*)DL698_SETLIB_6014_2_11,},
#endif
#ifdef InitData_6014_2_12_
	{(u32*)DL698_SETLIB_6014_2_12,},
#endif
#ifdef InitData_6014_2_13_
	{(u32*)DL698_SETLIB_6014_2_13,},
#endif
#ifdef InitData_6014_2_14_
	{(u32*)DL698_SETLIB_6014_2_14,},
#endif
#ifdef InitData_6014_2_15_
	{(u32*)DL698_SETLIB_6014_2_15,},
#endif
#ifdef InitData_6014_2_16_
	{(u32*)DL698_SETLIB_6014_2_16,},
#endif
#ifdef InitData_6014_2_17_
	{(u32*)DL698_SETLIB_6014_2_17,},
#endif
#ifdef InitData_6014_2_18_
	{(u32*)DL698_SETLIB_6014_2_18,},
#endif
#ifdef InitData_6014_2_19_
	{(u32*)DL698_SETLIB_6014_2_19,},
#endif
#ifdef InitData_6014_2_20_
	{(u32*)DL698_SETLIB_6014_2_20,},
#endif
#ifdef InitData_6014_2_21_
	{(u32*)DL698_SETLIB_6014_2_21,},
#endif
#ifdef InitData_6014_2_22_
	{(u32*)DL698_SETLIB_6014_2_22,},
#endif
#ifdef InitData_6014_2_23_
	{(u32*)DL698_SETLIB_6014_2_23,},
#endif
#ifdef InitData_6014_2_24_
	{(u32*)DL698_SETLIB_6014_2_24,},
#endif
#ifdef InitData_6014_2_25_
	{(u32*)DL698_SETLIB_6014_2_25,},
#endif
#ifdef InitData_6014_2_26_
	{(u32*)DL698_SETLIB_6014_2_26,},
#endif
#ifdef InitData_6014_2_27_
	{(u32*)DL698_SETLIB_6014_2_27,},
#endif
#ifdef InitData_6014_2_28_
	{(u32*)DL698_SETLIB_6014_2_28,},
#endif
#ifdef InitData_6014_2_29_
	{(u32*)DL698_SETLIB_6014_2_29,},
#endif
#ifdef InitData_6014_2_30_
	{(u32*)DL698_SETLIB_6014_2_30,},
#endif
#ifdef InitData_6014_2_31_
	{(u32*)DL698_SETLIB_6014_2_31,},
#endif
#ifdef InitData_6014_2_32_
	{(u32*)DL698_SETLIB_6014_2_32,},
#endif
#ifdef InitData_6014_2_33_
	{(u32*)DL698_SETLIB_6014_2_33,},
#endif
#ifdef InitData_6014_2_34_
	{(u32*)DL698_SETLIB_6014_2_34,},
#endif
#ifdef InitData_6014_2_35_
	{(u32*)DL698_SETLIB_6014_2_35,},
#endif
#ifdef InitData_6014_2_36_
	{(u32*)DL698_SETLIB_6014_2_36,},
#endif
#ifdef InitData_6014_2_37_
	{(u32*)DL698_SETLIB_6014_2_37,},
#endif
#ifdef InitData_6014_2_38_
	{(u32*)DL698_SETLIB_6014_2_38,},
#endif
#ifdef InitData_6014_2_39_
	{(u32*)DL698_SETLIB_6014_2_39,},
#endif
#ifdef InitData_6014_2_40_
	{(u32*)DL698_SETLIB_6014_2_40,},
#endif
#ifdef InitData_6014_2_41_
	{(u32*)DL698_SETLIB_6014_2_41,},
#endif
#ifdef InitData_6014_2_42_
	{(u32*)DL698_SETLIB_6014_2_42,},
#endif
#ifdef InitData_6014_2_43_
	{(u32*)DL698_SETLIB_6014_2_43,},
#endif
#ifdef InitData_6014_2_44_
	{(u32*)DL698_SETLIB_6014_2_44,},
#endif
#ifdef InitData_6014_2_45_
	{(u32*)DL698_SETLIB_6014_2_45,},
#endif
#ifdef InitData_6014_2_46_
	{(u32*)DL698_SETLIB_6014_2_46,},
#endif
#ifdef InitData_6014_2_47_
	{(u32*)DL698_SETLIB_6014_2_47,},
#endif
#ifdef InitData_6014_2_48_
	{(u32*)DL698_SETLIB_6014_2_48,},
#endif
#ifdef InitData_6014_2_49_
	{(u32*)DL698_SETLIB_6014_2_49,},
#endif
#ifdef InitData_6014_2_50_
	{(u32*)DL698_SETLIB_6014_2_50,},
#endif
#ifdef InitData_6014_2_51_
	{(u32*)DL698_SETLIB_6014_2_51,},
#endif
#ifdef InitData_6014_2_52_
	{(u32*)DL698_SETLIB_6014_2_52,},
#endif
#ifdef InitData_6014_2_53_
	{(u32*)DL698_SETLIB_6014_2_53,},
#endif
#ifdef InitData_6014_2_54_
	{(u32*)DL698_SETLIB_6014_2_54,},
#endif
#ifdef InitData_6014_2_55_
	{(u32*)DL698_SETLIB_6014_2_55,},
#endif
#ifdef InitData_6014_2_56_
	{(u32*)DL698_SETLIB_6014_2_56,},
#endif
#ifdef InitData_6014_2_57_
	{(u32*)DL698_SETLIB_6014_2_57,},
#endif
#ifdef InitData_6014_2_58_
	{(u32*)DL698_SETLIB_6014_2_58,},
#endif
#ifdef InitData_6014_2_59_
	{(u32*)DL698_SETLIB_6014_2_59,},
#endif
#ifdef InitData_6014_2_60_
	{(u32*)DL698_SETLIB_6014_2_60,},
#endif
#ifdef InitData_6014_2_61_
	{(u32*)DL698_SETLIB_6014_2_61,},
#endif
#ifdef InitData_6014_2_62_
	{(u32*)DL698_SETLIB_6014_2_62,},
#endif
#ifdef InitData_6014_2_63_
	{(u32*)DL698_SETLIB_6014_2_63,},
#endif
#ifdef InitData_6014_2_64_
	{(u32*)DL698_SETLIB_6014_2_64,},
#endif
	
#if NUMmax_6014>(64+1)//最大普通采集方案数
	#error
#endif
	
	{(u32*)DL698_SETLIB_6014_3,},
	{(u32*)DL698_SETLIB_6014_4,},
	
	{(u32*)DL698_SETLIB_6016_2,},//定位第0个地址，不能删除
#ifdef InitData_6016_2_1_
	{(u32*)DL698_SETLIB_6016_2_1,},
#endif
#ifdef InitData_6016_2_2_
	{(u32*)DL698_SETLIB_6016_2_2,},
#endif
#ifdef InitData_6016_2_3_
	{(u32*)DL698_SETLIB_6016_2_3,},
#endif
#ifdef InitData_6016_2_4_
	{(u32*)DL698_SETLIB_6016_2_4,},
#endif
#ifdef InitData_6016_2_5_
	{(u32*)DL698_SETLIB_6016_2_5,},
#endif
#ifdef InitData_6016_2_6_
	{(u32*)DL698_SETLIB_6016_2_6,},
#endif
#ifdef InitData_6016_2_7_
	{(u32*)DL698_SETLIB_6016_2_7,},
#endif
#ifdef InitData_6016_2_8_
	{(u32*)DL698_SETLIB_6016_2_8,},
#endif
#ifdef InitData_6016_2_9_
	{(u32*)DL698_SETLIB_6016_2_9,},
#endif
#ifdef InitData_6016_2_10_
	{(u32*)DL698_SETLIB_6016_2_10,},
#endif
#ifdef InitData_6016_2_11_
	{(u32*)DL698_SETLIB_6016_2_11,},
#endif
#ifdef InitData_6016_2_12_
	{(u32*)DL698_SETLIB_6016_2_12,},
#endif
#ifdef InitData_6016_2_13_
	{(u32*)DL698_SETLIB_6016_2_13,},
#endif
#ifdef InitData_6016_2_14_
	{(u32*)DL698_SETLIB_6016_2_14,},
#endif
#ifdef InitData_6016_2_15_
	{(u32*)DL698_SETLIB_6016_2_15,},
#endif
#ifdef InitData_6016_2_16_
	{(u32*)DL698_SETLIB_6016_2_16,},
#endif
#ifdef InitData_6016_2_17_
	{(u32*)DL698_SETLIB_6016_2_17,},
#endif
#ifdef InitData_6016_2_18_
	{(u32*)DL698_SETLIB_6016_2_18,},
#endif
#ifdef InitData_6016_2_19_
	{(u32*)DL698_SETLIB_6016_2_19,},
#endif
#ifdef InitData_6016_2_20_
	{(u32*)DL698_SETLIB_6016_2_20,},
#endif
#ifdef InitData_6016_2_21_
	{(u32*)DL698_SETLIB_6016_2_21,},
#endif
#ifdef InitData_6016_2_22_
	{(u32*)DL698_SETLIB_6016_2_22,},
#endif
#ifdef InitData_6016_2_23_
	{(u32*)DL698_SETLIB_6016_2_23,},
#endif
#ifdef InitData_6016_2_24_
	{(u32*)DL698_SETLIB_6016_2_24,},
#endif
#ifdef InitData_6016_2_25_
	{(u32*)DL698_SETLIB_6016_2_25,},
#endif
#ifdef InitData_6016_2_26_
	{(u32*)DL698_SETLIB_6016_2_26,},
#endif
#ifdef InitData_6016_2_27_
	{(u32*)DL698_SETLIB_6016_2_27,},
#endif
#ifdef InitData_6016_2_28_
	{(u32*)DL698_SETLIB_6016_2_28,},
#endif
#ifdef InitData_6016_2_29_
	{(u32*)DL698_SETLIB_6016_2_29,},
#endif
#ifdef InitData_6016_2_30_
	{(u32*)DL698_SETLIB_6016_2_30,},
#endif
#ifdef InitData_6016_2_31_
	{(u32*)DL698_SETLIB_6016_2_31,},
#endif
#ifdef InitData_6016_2_32_
	{(u32*)DL698_SETLIB_6016_2_32,},
#endif
#ifdef InitData_6016_2_33_
	{(u32*)DL698_SETLIB_6016_2_33,},
#endif
#ifdef InitData_6016_2_34_
	{(u32*)DL698_SETLIB_6016_2_34,},
#endif
#ifdef InitData_6016_2_35_
	{(u32*)DL698_SETLIB_6016_2_35,},
#endif
#ifdef InitData_6016_2_36_
	{(u32*)DL698_SETLIB_6016_2_36,},
#endif
#ifdef InitData_6016_2_37_
	{(u32*)DL698_SETLIB_6016_2_37,},
#endif
#ifdef InitData_6016_2_38_
	{(u32*)DL698_SETLIB_6016_2_38,},
#endif
#ifdef InitData_6016_2_39_
	{(u32*)DL698_SETLIB_6016_2_39,},
#endif
#ifdef InitData_6016_2_40_
	{(u32*)DL698_SETLIB_6016_2_40,},
#endif
#ifdef InitData_6016_2_41_
	{(u32*)DL698_SETLIB_6016_2_41,},
#endif
#ifdef InitData_6016_2_42_
	{(u32*)DL698_SETLIB_6016_2_42,},
#endif
#ifdef InitData_6016_2_43_
	{(u32*)DL698_SETLIB_6016_2_43,},
#endif
#ifdef InitData_6016_2_44_
	{(u32*)DL698_SETLIB_6016_2_44,},
#endif
#ifdef InitData_6016_2_45_
	{(u32*)DL698_SETLIB_6016_2_45,},
#endif
#ifdef InitData_6016_2_46_
	{(u32*)DL698_SETLIB_6016_2_46,},
#endif
#ifdef InitData_6016_2_47_
	{(u32*)DL698_SETLIB_6016_2_47,},
#endif
#ifdef InitData_6016_2_48_
	{(u32*)DL698_SETLIB_6016_2_48,},
#endif
#ifdef InitData_6016_2_49_
	{(u32*)DL698_SETLIB_6016_2_49,},
#endif
#ifdef InitData_6016_2_50_
	{(u32*)DL698_SETLIB_6016_2_50,},
#endif
#ifdef InitData_6016_2_51_
	{(u32*)DL698_SETLIB_6016_2_51,},
#endif
#ifdef InitData_6016_2_52_
	{(u32*)DL698_SETLIB_6016_2_52,},
#endif
#ifdef InitData_6016_2_53_
	{(u32*)DL698_SETLIB_6016_2_53,},
#endif
#ifdef InitData_6016_2_54_
	{(u32*)DL698_SETLIB_6016_2_54,},
#endif
#ifdef InitData_6016_2_55_
	{(u32*)DL698_SETLIB_6016_2_55,},
#endif
#ifdef InitData_6016_2_56_
	{(u32*)DL698_SETLIB_6016_2_56,},
#endif
#ifdef InitData_6016_2_57_
	{(u32*)DL698_SETLIB_6016_2_57,},
#endif
#ifdef InitData_6016_2_58_
	{(u32*)DL698_SETLIB_6016_2_58,},
#endif
#ifdef InitData_6016_2_59_
	{(u32*)DL698_SETLIB_6016_2_59,},
#endif
#ifdef InitData_6016_2_60_
	{(u32*)DL698_SETLIB_6016_2_60,},
#endif
#ifdef InitData_6016_2_61_
	{(u32*)DL698_SETLIB_6016_2_61,},
#endif
#ifdef InitData_6016_2_62_
	{(u32*)DL698_SETLIB_6016_2_62,},
#endif
#ifdef InitData_6016_2_63_
	{(u32*)DL698_SETLIB_6016_2_63,},
#endif
#ifdef InitData_6016_2_64_
	{(u32*)DL698_SETLIB_6016_2_64,},
#endif
	
#if NUMmax_6016>(64+1)//最大事件采集方案数
	#error
#endif
	
	
	{(u32*)DL698_SETLIB_6016_3,},
	{(u32*)DL698_SETLIB_6016_4,},
	{(u32*)DL698_SETLIB_6018,},
	{(u32*)DL698_SETLIB_6018_3,},
	{(u32*)DL698_SETLIB_6018_4,},
	{(u32*)DL698_SETLIB_601A,},
	{(u32*)DL698_SETLIB_601A_3,},
	{(u32*)DL698_SETLIB_601A_4,},
	{(u32*)DL698_SETLIB_601C,},
#ifdef InitData_601C_2_1_
    {(u32*)DL698_SETLIB_601C_2_1},
#endif
#ifdef InitData_601C_2_2_
    {(u32*)DL698_SETLIB_601C_2_2},
#endif
#ifdef InitData_601C_2_3_
    {(u32*)DL698_SETLIB_601C_2_3},
#endif
#ifdef InitData_601C_2_4_
    {(u32*)DL698_SETLIB_601C_2_4},
#endif
#ifdef InitData_601C_2_5_
    {(u32*)DL698_SETLIB_601C_2_5},
#endif
#ifdef InitData_601C_2_6_
    {(u32*)DL698_SETLIB_601C_2_6},
#endif
#ifdef InitData_601C_2_7_
    {(u32*)DL698_SETLIB_601C_2_7},
#endif
#ifdef InitData_601C_2_8_
    {(u32*)DL698_SETLIB_601C_2_8},
#endif
#ifdef InitData_601C_2_9_
    {(u32*)DL698_SETLIB_601C_2_9},
#endif    
#ifdef InitData_601C_2_10_
    {(u32*)DL698_SETLIB_601C_2_10},
#endif    
#ifdef InitData_601C_2_11_
    {(u32*)DL698_SETLIB_601C_2_11},
#endif
#ifdef InitData_601C_2_12_
    {(u32*)DL698_SETLIB_601C_2_12},
#endif    
#ifdef InitData_601C_2_13_
    {(u32*)DL698_SETLIB_601C_2_13},
#endif    
#ifdef InitData_601C_2_14_
    {(u32*)DL698_SETLIB_601C_2_14},
#endif   
#ifdef InitData_601C_2_15_
    {(u32*)DL698_SETLIB_601C_2_15},
#endif    
#ifdef InitData_601C_2_16_
    {(u32*)DL698_SETLIB_601C_2_16},
#endif    
#ifdef InitData_601C_2_17_
    {(u32*)DL698_SETLIB_601C_2_17},
#endif
#ifdef InitData_601C_2_18_
    {(u32*)DL698_SETLIB_601C_2_18},
#endif
#ifdef InitData_601C_2_19_
    {(u32*)DL698_SETLIB_601C_2_19},
#endif    
#ifdef InitData_601C_2_20_
    {(u32*)DL698_SETLIB_601C_2_20},
#endif    
#ifdef InitData_601C_2_21_
    {(u32*)DL698_SETLIB_601C_2_21},
#endif
#ifdef InitData_601C_2_22_
    {(u32*)DL698_SETLIB_601C_2_22},
#endif
#ifdef InitData_601C_2_23_
    {(u32*)DL698_SETLIB_601C_2_23},
#endif
#ifdef InitData_601C_2_24_
    {(u32*)DL698_SETLIB_601C_2_24},
#endif
#ifdef InitData_601C_2_25_
    {(u32*)DL698_SETLIB_601C_2_25},
#endif
#ifdef InitData_601C_2_26_
    {(u32*)DL698_SETLIB_601C_2_26},
#endif
#ifdef InitData_601C_2_27_
    {(u32*)DL698_SETLIB_601C_2_27},
#endif
#ifdef InitData_601C_2_28_
    {(u32*)DL698_SETLIB_601C_2_28},
#endif
#ifdef InitData_601C_2_29_
    {(u32*)DL698_SETLIB_601C_2_29},
#endif   
#ifdef InitData_601C_2_30_
    {(u32*)DL698_SETLIB_601C_2_30},
#endif     
#ifdef InitData_601C_2_31_
    {(u32*)DL698_SETLIB_601C_2_31},
#endif
#ifdef InitData_601C_2_32_
    {(u32*)DL698_SETLIB_601C_2_32},
#endif
#ifdef InitData_601C_2_33_
    {(u32*)DL698_SETLIB_601C_2_33},
#endif
#ifdef InitData_601C_2_34_
    {(u32*)DL698_SETLIB_601C_2_34},
#endif
#ifdef InitData_601C_2_35_
    {(u32*)DL698_SETLIB_601C_2_35},
#endif
#ifdef InitData_601C_2_36_
    {(u32*)DL698_SETLIB_601C_2_36},
#endif
#ifdef InitData_601C_2_37_
    {(u32*)DL698_SETLIB_601C_2_37},
#endif
#ifdef InitData_601C_2_38_
    {(u32*)DL698_SETLIB_601C_2_38},
#endif
#ifdef InitData_601C_2_39_
    {(u32*)DL698_SETLIB_601C_2_39},
#endif    
#ifdef InitData_601C_2_40_
    {(u32*)DL698_SETLIB_601C_2_40},
#endif     
#ifdef InitData_601C_2_41_
    {(u32*)DL698_SETLIB_601C_2_41},
#endif
#ifdef InitData_601C_2_42_
    {(u32*)DL698_SETLIB_601C_2_42},
#endif
#ifdef InitData_601C_2_43_
    {(u32*)DL698_SETLIB_601C_2_43},
#endif
#ifdef InitData_601C_2_44_
    {(u32*)DL698_SETLIB_601C_2_44},
#endif
#ifdef InitData_601C_2_45_
    {(u32*)DL698_SETLIB_601C_2_45},
#endif
#ifdef InitData_601C_2_46_
    {(u32*)DL698_SETLIB_601C_2_46},
#endif
#ifdef InitData_601C_2_47_
    {(u32*)DL698_SETLIB_601C_2_47},
#endif
#ifdef InitData_601C_2_48_
    {(u32*)DL698_SETLIB_601C_2_48},
#endif
#ifdef InitData_601C_2_49_
    {(u32*)DL698_SETLIB_601C_2_49},
#endif
#ifdef InitData_601C_2_50_
    {(u32*)DL698_SETLIB_601C_2_50},
#endif
#ifdef InitData_601C_2_51_
    {(u32*)DL698_SETLIB_601C_2_51},
#endif
#ifdef InitData_601C_2_52_
    {(u32*)DL698_SETLIB_601C_2_52},
#endif
#ifdef InitData_601C_2_53_
    {(u32*)DL698_SETLIB_601C_2_53},
#endif
#ifdef InitData_601C_2_54_
    {(u32*)DL698_SETLIB_601C_2_54},
#endif
#ifdef InitData_601C_2_55_
    {(u32*)DL698_SETLIB_601C_2_55},
#endif
#ifdef InitData_601C_2_56_
    {(u32*)DL698_SETLIB_601C_2_56},
#endif
#ifdef InitData_601C_2_57_
    {(u32*)DL698_SETLIB_601C_2_57},
#endif
#ifdef InitData_601C_2_58_
    {(u32*)DL698_SETLIB_601C_2_58},
#endif
#ifdef InitData_601C_2_59_
    {(u32*)DL698_SETLIB_601C_2_59},
#endif
#ifdef InitData_601C_2_60_
    {(u32*)DL698_SETLIB_601C_2_60},
#endif
#ifdef InitData_601C_2_61_
    {(u32*)DL698_SETLIB_601C_2_61},
#endif
#ifdef InitData_601C_2_62_
    {(u32*)DL698_SETLIB_601C_2_62},
#endif
#ifdef InitData_601C_2_63_
    {(u32*)DL698_SETLIB_601C_2_63},
#endif
#ifdef InitData_601C_2_64_
    {(u32*)DL698_SETLIB_601C_2_64},
#endif
#if NUMmax_601C>(64+1)//最大上报方案数
	#error
#endif
	
	{(u32*)DL698_SETLIB_601C_3,},
	{(u32*)DL698_SETLIB_601C_4,},
	{(u32*)DL698_SETLIB_601E,},
	{(u32*)DL698_SETLIB_601E_3,},
	{(u32*)DL698_SETLIB_601E_4,},
	{(u32*)DL698_SETLIB_6032,},
	{(u32*)DL698_SETLIB_6032_3,},
	{(u32*)DL698_SETLIB_6032_4,},
	{(u32*)DL698_SETLIB_6034,},
	{(u32*)DL698_SETLIB_6034_3,},
	{(u32*)DL698_SETLIB_6034_4,},

#if (USER/100)==14//福建增补
	{(u32*)DL698_SETLIB_6E00,},
	{(u32*)DL698_SETLIB_6E01,},
	{(u32*)DL698_SETLIB_6E11,},
	{(u32*)DL698_SETLIB_6E13,},
	{(u32*)DL698_SETLIB_6E17,},
	{(u32*)DL698_SETLIB_6E30,},
	{(u32*)DL698_SETLIB_6E33,},

#endif//#if (USER/100)==14//福建增补
#if (USER/100)==15//安徽
	{(u32*)DL698_SETLIB_3106_11,},
#endif
	
//#if (USER/100)==17//???
	{(u32*)DL698_SETLIB_4048,},
//#endif
	
//集合类
	
	
	
//控制类
	{(u32*)DL698_SETLIB_8000_2,},
	{(u32*)DL698_SETLIB_8000_3,},
	{(u32*)DL698_SETLIB_8000_4,},
	{(u32*)DL698_SETLIB_8000_5,},
	{(u32*)DL698_SETLIB_8001_2,},
	{(u32*)DL698_SETLIB_8001_3,},
	{(u32*)DL698_SETLIB_8001_4,},
	{(u32*)DL698_SETLIB_8001_5,},
	{(u32*)DL698_SETLIB_8002_2,},
	{(u32*)DL698_SETLIB_8002_3,},
	
	{(u32*)DL698_SETLIB_8100_2,},
	{(u32*)DL698_SETLIB_8101_2,},
	{(u32*)DL698_SETLIB_8102_2,},
	{(u32*)DL698_SETLIB_8103_2,},
	{(u32*)DL698_SETLIB_8103_3,},
	{(u32*)DL698_SETLIB_8103_4,},
	{(u32*)DL698_SETLIB_8103_5,},
	{(u32*)DL698_SETLIB_8104_2,},
	{(u32*)DL698_SETLIB_8104_3,},
	{(u32*)DL698_SETLIB_8104_4,},
	{(u32*)DL698_SETLIB_8104_5,},
	{(u32*)DL698_SETLIB_8105_2,},
	{(u32*)DL698_SETLIB_8105_3,},
	{(u32*)DL698_SETLIB_8105_4,},
	{(u32*)DL698_SETLIB_8105_5,},
	{(u32*)DL698_SETLIB_8106_3,},
	{(u32*)DL698_SETLIB_8106_4,},
	{(u32*)DL698_SETLIB_8106_5,},
	{(u32*)DL698_SETLIB_8107_2,},
	{(u32*)DL698_SETLIB_8107_3,},
	{(u32*)DL698_SETLIB_8107_4,},
	{(u32*)DL698_SETLIB_8107_5,},
	{(u32*)DL698_SETLIB_8108_2,},
	{(u32*)DL698_SETLIB_8108_3,},
	{(u32*)DL698_SETLIB_8108_4,},
	{(u32*)DL698_SETLIB_8108_5,},
	
	{(u32*)DL698_SETLIB_230x_2,},//总加配置表(仅用于初始化)
	{(u32*)DL698_SETLIB_230x_13,},//总加组滑差时间周期(仅用于初始化)
	{(u32*)DL698_SETLIB_230x_14,},//总加组功控轮次配置(仅用于初始化)
	{(u32*)DL698_SETLIB_230x_15,},//总加组电控轮次配置(仅用于初始化)
	{(u32*)DL698_SETLIB_240x_2,},//脉冲计量-通信地址(仅用于初始化)
	{(u32*)DL698_SETLIB_240x_3,},//脉冲计量-互感器倍率(仅用于初始化)
	{(u32*)DL698_SETLIB_240x_4,},//脉冲计量-脉冲配置(仅用于初始化)
	
	
//文件传输类
	{(u32*)DL698_SETLIB_F002_4,},//FTP服务器信息
	
//ESAM接口类
	{(u32*)DL698_SETLIB_F100_2,},
	{(u32*)DL698_SETLIB_F100_3,},
	{(u32*)DL698_SETLIB_F100_4,},
	{(u32*)DL698_SETLIB_F100_5,},
	{(u32*)DL698_SETLIB_F100_6,},
	{(u32*)DL698_SETLIB_F100_7,},
	{(u32*)DL698_SETLIB_F100_8,},
	{(u32*)DL698_SETLIB_F100_9,},
	{(u32*)DL698_SETLIB_F100_10,},
	{(u32*)DL698_SETLIB_F100_11,},
	{(u32*)DL698_SETLIB_F100_12,},
	{(u32*)DL698_SETLIB_F100_13,},
	{(u32*)DL698_SETLIB_F101_2,},
	{(u32*)DL698_SETLIB_F101_3,},
//输入输出设备类
	{(u32*)DL698_SETLIB_F200_2,},
	{(u32*)DL698_SETLIB_F200_3,},
	{(u32*)DL698_SETLIB_F201_2,},
	{(u32*)DL698_SETLIB_F201_3,},
	{(u32*)DL698_SETLIB_F202_2,},
	{(u32*)DL698_SETLIB_F202_3,},
	{(u32*)DL698_SETLIB_F203_2,},
	{(u32*)DL698_SETLIB_F203_3,},
	{(u32*)DL698_SETLIB_F203_4,},
	{(u32*)DL698_SETLIB_F204_2,},
	{(u32*)DL698_SETLIB_F204_3,},
	{(u32*)DL698_SETLIB_F204_4,},
	{(u32*)DL698_SETLIB_F205_2,},
	{(u32*)DL698_SETLIB_F205_3,},
	{(u32*)DL698_SETLIB_F206_2,},
	{(u32*)DL698_SETLIB_F206_3,},
	{(u32*)DL698_SETLIB_F206_4,},
	{(u32*)DL698_SETLIB_F207_2,},
	{(u32*)DL698_SETLIB_F207_3,},
	{(u32*)DL698_SETLIB_F209_2,},
	{(u32*)DL698_SETLIB_F209_3,},
	{(u32*)DL698_SETLIB_F209_6,},
	{(u32*)DL698_SETLIB_F209_9,},
	{(u32*)DL698_SETLIB_F209_11,},
	{(u32*)DL698_SETLIB_F209_22,},
	{(u32*)DL698_SETLIB_F20A_2,},
	{(u32*)DL698_SETLIB_F20A_3,},
	{(u32*)DL698_SETLIB_F20B_2,},
	{(u32*)DL698_SETLIB_F20B_3,},
	

//显示类
	
	
};










DL698_SETLIB_TypeDef *Get_DL698_SET_pLib(u32 OAD)//得到SET_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
{
	u32 i;
	DL698_SETLIB_TypeDef *pLib;

	for(i=0;i<sizeof(DL698_SETLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_List[i].pLib;
		if(OAD==pLib->OAD)
		{
			return pLib;
		}
	}
	
	return 0;
}



extern const u8 InitData_6014_2_9[];//实时采集任务

#if (USER/100)==9//河南版
void FileID_V1toV2(void)//FileID版本V1->V2
{
//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
//V2区别V1关键点:
	//1. 增加采集类型14=月冻结数据
	//2. FileID中的存储时标类型V1=0-7,V2=0xf同时表示改为不用了
//--V1---
	//实时数据0x10xxxxxx
	//日冻结月冻结数据0x12xxxxx
	//曲线数据0x136xxxxx
//--V2---
	//实时数据0x10Fxxxxx
	//日冻结数据0x12Fxxxx
	//曲线数据0x13Fxxxxx
	//月冻结数据0x1EFxxxxx
	u32 FILEID;
	u32 filename;
	msFILE_TypeDef *file;
	u32 NUMmax_File;
	ms_Type *ms;

	ms=Get_ms();
	NUMmax_File=((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(msFILE_TypeDef));
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<NUMmax_File;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			FILEID=file->FILEID;
			if((FILEID&0xff)==0)
			{//文件分类=0;普通采集方案记录,全事件采集方案记录
				if((FILEID>>28)==1)
				{//方案类型=1;1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
					switch((FILEID>>20)&0xf)
					{//存储时标类型
						case 0xf:
							break;
					//V1版使用的存储时标类型
					//未定义              （0），
					//任务开始时间        （1），
					//相对当日0点0分    （2），
					//相对上日23点59分  （3），
					//相对上日0点0分    （4），
					//相对当月1日0点0分（5），
					//数据冻结时标        （6）
					//相对上月月末 23 点 59 分 （7）
						case 7:
							//删除V1版月冻结文件
							ms->msfile_Delete(filename);
							break;
						default:
							//其他文件
							FILEID|=0xf<<20;//避免，自动删除任务已不存在的记录文件
							file->FILEID=FILEID;
							break;
					}
				}
			}
		}
		file++;
	}
}
void DataMoveNewAddr(u8* pSOUR)//档案和文件数据转到新地址,入口:原设置参数读到RAM的指针
{
//V1.00
//#define RMTASKmax 			60//最大任务数
//#define NUMmax_6012_2   RMTASKmax//最大抄表任务数
//#define NUMmax_6014     60//最大普通采集方案数
//#define NUMmax_6016     32//最大事件采集方案数
//#define NUMmax_6018     32//最大透明方案数
//#define NUMmax_601C     16//最大上报方案数(此数影响文件管理的最大文件个数)
typedef __packed struct
{
	u16 BLOCK_ENTRY;//文件入口:nand flash块号,0=空;RAM 开始地址高2字节(值有效时!=0)
	u16 BLOCK_END;//文件结束:nand flash块号,0=空;RAM 开始地址低2字节
	u32 LEN_FILE;//文件预留占有存储空间总长度
	u32 LEN_DATA;//文件有效数据总长度
	
	u32 FILEID;//FILEID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	//文件分类:
	//0=普通采集方案数据记录
	//1=
	//2=按OI分类的事件记录
	//
	//
	u16 FILECOUNT;//相同FILEID的文件计数号
	u16 NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
	u16 NUM_FAIL;//文件头中写入抄表失败地址个数
	u8 save_date_time_s[7];//采集存储时标
	u8 start_date_time_s[7];//采集启动时间
	u16 NUM_GPRSreport[16];//已上报的数据个数(最大上报方案数)
	u16 NUM_ENETreport[16];//已上报的数据个数(最大上报方案数)
}V100_msFILE_TypeDef;

//V1.10
//#define RMTASKmax 			64//最大任务数
//#define NUMmax_6012_2   RMTASKmax//最大抄表任务数
//#define NUMmax_6014     64//60//最大普通采集方案数
//#define NUMmax_6016     64//32//最大事件采集方案数
//#define NUMmax_6018     64//32//最大透明方案数
//#define NUMmax_601C     64//16//最大上报方案数(此数影响文件管理的最大文件个数)
typedef __packed struct
{
	u16 BLOCK_ENTRY;//文件入口:nand flash块号,0=空;RAM 开始地址高2字节(值有效时!=0)
	u16 BLOCK_END;//文件结束:nand flash块号,0=空;RAM 开始地址低2字节
	u32 LEN_FILE;//文件预留占有存储空间总长度
	u32 LEN_DATA;//文件有效数据总长度
	
	u32 FILEID;//FILEID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	//文件分类:
	//0=普通采集方案数据记录
	//1=
	//2=按OI分类的事件记录
	//
	//
	u16 FILECOUNT;//相同FILEID的文件计数号
	u16 NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
	u16 NUM_FAIL;//文件头中写入抄表失败地址个数
	u8 save_date_time_s[7];//采集存储时标
	u8 start_date_time_s[7];//采集启动时间
 #if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
//GPRS,ENET各自上报
	u16 NUM_GPRSreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
	u16 NUM_ENETreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
 #else
//GPRS,ENET合并上报
	u16 NUM_GPRSorENETreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
 #endif
}V110_msFILE_TypeDef;

	u32 i;
	u32 n;
	u8* p8;
	u32* p32;
	u32 filename;
	V100_msFILE_TypeDef *V100_file;
	//V110_msFILE_TypeDef *V110_file;
	u32 NUMmax_FileSour;
	u32 NUMmax_File_New;
	ms_Type *ms;
	os_Type *os;
	FILE* fsfile;


	ms=Get_ms();
	os=Get_os();
	p8=pSOUR;
	p8+=(0x31b4bbbb-(ADDR_DL698_SET_START));//ADDR_6014+(9*512);//原普通采集方案9绝对地址
	for(i=0;i<sizeof(InitData_6014_2_9);i++)
	{
		if(p8[i]!=InitData_6014_2_9[i])
		{
			break;
		}
	}
	if(i>=sizeof(InitData_6014_2_9))
	{//原设置参数是更改前V1.00的参数
		//档案转移
		p8=pSOUR;
		p8+=(0x31b28b40-(ADDR_DL698_SET_START));//ADDR_6000;//原采集档案配置表绝对地址
		MW((u32)p8,ADDR_6000,128*1024);
		//文件管理转移
//#define FILENAMEmax_FLASH     ((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(msFILE_TypeDef))//存储在FLASH中的文件的最大文件数
		NUMmax_FileSour=((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(V100_msFILE_TypeDef));
		NUMmax_File_New=((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(V110_msFILE_TypeDef));
		MC(0,(u32)pSOUR,(LEN_msFILEmanage-LEN_msFILEunltFlags-12));
		n=0;
		V100_file=(V100_msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		//V110_file=(V110_msFILE_TypeDef*)(pSOUR);
		for(filename=0;filename<NUMmax_FileSour;filename++)
		{
			if(V100_file->BLOCK_ENTRY)
			{
				if(V100_file->FILECOUNT<400)//(3*96)//曲线数据最大保留点数,免新定义的总文件数比原小而无法全部转移
				{
					if((V100_file->FILEID&0xff)<=1)//限制转移文件类型,免新定义的总文件数比原小而无法全部转移
					{
					//0=普通采集方案,全事件采集方案数据记录
					//1=数据冻结(终端自生成)
					//2=按OI分类的事件记录(终端自生成)
					//3=所有搜表结果
					//4=跨台区搜表结果
					//5=电能表主动上报
						if(n<NUMmax_File_New)
						{
						#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
							MW(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(V100_msFILE_TypeDef)),(u32)pSOUR+(n*sizeof(V110_msFILE_TypeDef)),sizeof(V100_msFILE_TypeDef)-(2*16));
							MW(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(V100_msFILE_TypeDef))+(sizeof(V100_msFILE_TypeDef)-(2*16)),(u32)pSOUR+(n*sizeof(V110_msFILE_TypeDef))+(sizeof(V110_msFILE_TypeDef)-(2*NUMmax_601C)),(2*16));
						#else
							MW(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(V100_msFILE_TypeDef)),(u32)pSOUR+(n*sizeof(V110_msFILE_TypeDef)),sizeof(V100_msFILE_TypeDef)-(2*16));
						#endif
							n++;
							//V110_file++;
							V100_file++;
							continue;
						}
					}
				}
				//没转移的需同步删除文件解放占用的flash
				ms->msfile_Delete(filename);
			}
			V100_file++;
		}
		MW((u32)pSOUR,ADDR_msFILEmanage+LEN_msFILEunltFlags,LEN_msFILEmanage-LEN_msFILEunltFlags-12);
		
		FileID_V1toV2();//FileID版本V1->V2
		//CRC计算
		p32=(u32*)(ADDR_msFILEmanage+LEN_msFILEmanage-8);
		p32[0]=0x12345678;
		p32[1]=RAM_CRC32(ADDR_msFILEmanage,LEN_msFILEmanage-4);//RAM中数据CRC32计算
		MW(ADDR_msFILEmanage,ADDR_msFILEmanage+LEN_msFILEmanage,LEN_msFILEmanage);
		//写入os文件
		os->fchdrive ("N0:");
		os->fchdir("N0:/");
		fsfile=os->fopen("msFILEmanage","w");
		if(fsfile!=NULL)
		{
			i=os->fwrite((void*)ADDR_msFILEmanage,1,LEN_msFILEmanage,fsfile);
			if(i!=LEN_msFILEmanage)
			{
				i=i;
			}
			os->fclose(fsfile);
		}
		return;
	}
	p8=pSOUR;
	p8+=(0x31b4bdbb-(ADDR_DL698_SET_START));//ADDR_6014+(9*512);//原普通采集方案9绝对地址
	for(i=0;i<sizeof(InitData_6014_2_9);i++)
	{
		if(p8[i]!=InitData_6014_2_9[i])
		{
			break;
		}
	}
	if(i>=sizeof(InitData_6014_2_9))
	{//原设置参数是V1.10及以后的参数
		//档案转移
		p8=pSOUR;
		p8+=(0x31b28b40-(ADDR_DL698_SET_START));//ADDR_6000;//原采集档案配置表绝对地址
		MW((u32)p8,ADDR_6000,128*1024);
		//文件管理转移
		FileID_V1toV2();//FileID版本V1->V2
		return;
	}
	
}
#endif//#if (USER/100)==9//河南版


void DL698_SETINIT_601E(u32 Command);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
void DL698_SETINIT(u32 Command,u8* pOAD)//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
{
	u32 i;
	u32 x;
	u32 n;
	u32 OAD;
	u8* p8;
	DL698_SETLIB_TypeDef *pLib;
	ms_Type *ms;
	u8* pSOUR;
	
	ms=Get_ms();
#ifdef IC_STM32F4xx
	ms->Init_my_heap();
#endif
	i=LEN_FLASH_DL698_SET;
	if(i<LEN_msFILEmanage)
	{
		i=LEN_msFILEmanage;
	}
	pSOUR=ms->ms_malloc(i);//设置数据区总长度
	if(pSOUR)
	{//有内存分配
		//读原设置数据
		MR((u32)pSOUR,ADDR_DL698_SET_START,(ADDR_DL698_SET_END)-(ADDR_DL698_SET_START));
	}
	else
	{//内存分配失败
		if((Command==3)||(Command==7))
		{
			Command=4;//4/5=除与系统主站通信外的参数初始化
		}
	}
	if(Command==6)
	{//6=仅部份参数初始化
		for(i=0;i<sizeof(DL698_SETLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
		{
			pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_List[i].pLib;
			if(pLib->pInitData)
			{
				if(pLib->LEN_Sour>pLib->LENmax)
				{//原始数据长度>FLASH预留的最大长度
					Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					DisplayStringAutoUp(0,9,(u8*)"\x0""参数长度超限!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
					continue;
				}
				switch(pLib->OAD)
				{
			#if (USER/100)==9//河南
					//部份参数表
					case 0x40160200://当前套日时段表
					case 0x45200200://公网远程通信多接入点备用通道
					case 0x45000200://公网通信模块1; 2通信配置
						break;
			#endif
					
					case 0xffff://空OI,仅编译时不警告
						break;
					default:
						continue;
				}
				MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
			}
		}
		ms->ms_free(pSOUR);
		DL698_SETINIT_EVENT(Command,pOAD);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
		DL698_SETINIT_601E(Command);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
		return;
	}
	
#if (USER/100)==9//河南版
	if(Comm_Ram->Factory==0x55)
	{//菜单密码初始化
		i=(ProduceDate_year_BCD<<8)|ProduceDate_month_BCD;
	}
	else
	{
		i=MRR(ADDR_MenuKey,3);//菜单密码
	}
#endif
	
	if(Comm_Ram->Factory==0x55)
	{
		MC(0,ADDR_DL698_COMSET_END,(ADDR_DL698_SET_END)-(ADDR_DL698_COMSET_END));//清0
	}
	else
	{
		//非工厂模式，文件传输块状态不清0
		MC(0,ADDR_DL698_COMSET_END,(ADDR_F001_4)-(ADDR_DL698_COMSET_END));//清0
		MC(0,ADDR_F001_END,(ADDR_DL698_SET_END)-(ADDR_F001_END));//清0
	}
	
#if (USER/100)==9//河南版
	MWR(i,ADDR_MenuKey,3);//菜单密码
#endif
	
	for(i=0;i<sizeof(DL698_SETLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_List[i].pLib;
	#if (USER/100)==15//安徽
		if(pLib->OAD==0x43000800)
		{
			MR(ADDR_DATABUFF,ADDR_4300_8,LENmax_4300_8);
			p8=(u8*)ADDR_DATABUFF;
			p8[0]=DataType_bool;
			p8[1]=1;
			MW(ADDR_DATABUFF,ADDR_4300_8,LENmax_4300_8);
			continue;
		}
	#endif
		if(pLib->pInitData)
		{
			if(pLib->rw&0x04)
			{//通信参数
				if((Command!=3)&&(Command!=7))
				{//除通信参数外
					continue;
				}
				if((pLib->OAD==0x40010200)&&(Comm_Ram->Factory!=0x55))
				{//通信地址在非工厂状态不变
					continue;
				}
			}
			if(pLib->LEN_Sour>pLib->LENmax)
			{//原始数据长度>FLASH预留的最大长度
				Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""参数长度超限!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				continue;
			}
			if(pOAD)
			{//参数定义的OAD列表，保持现状，不在恢复之列
				p8=pOAD;//参数∷=array OAD
				n=p8[1];
				p8+=2;
				for(x=0;x<n;x++)
				{
					OAD=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
					if(OAD==pLib->OAD)
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
		#if (USER/100)!=0//不是电科院测试
			if((pLib->OAD==0x40010200)&&(Comm_Ram->Factory!=0x55))
			{//恢复出厂时通信地址
				MR(ADDR_DATABUFF,ADDR_4001_Factory,LENmax_4001);
				p8=(u8*)ADDR_DATABUFF;
				p8[0]=DataType_octet_string;
				p8[1]=6;
				MW(ADDR_DATABUFF,ADDR_4001,LENmax_4001);
				continue;
			}
		#endif
			if((pLib->OAD==0x40020200)&&(Comm_Ram->Factory!=0x55))
			{//恢复出厂时表号
				MR(ADDR_DATABUFF,ADDR_4001_Factory+LENmax_4001,LENmax_4002);
				p8=(u8*)ADDR_DATABUFF;
				p8[0]=DataType_octet_string;
				p8[1]=6;
				MW(ADDR_DATABUFF,ADDR_4002,LENmax_4002);
				continue;
			}
			if((pLib->OAD==0x40030200)&&(Comm_Ram->Factory!=0x55))
			{//恢复出厂时客户编号
				MR(ADDR_DATABUFF,ADDR_4001_Factory+LENmax_4001+LENmax_4002,LENmax_4003);
				p8=(u8*)ADDR_DATABUFF;
				p8[0]=DataType_octet_string;
				p8[1]=6;
				MW(ADDR_DATABUFF,ADDR_4003,LENmax_4003);
				continue;
			}
			MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
		}
	}
	
	if(Command==7)//7=参数初始化(档案和nand文件数据转移到新地址)
	{
	#if (USER/100)==9//河南版
		DataMoveNewAddr(pSOUR);//档案和文件数据转到新地址,入口:原设置参数读到RAM的指针
	#endif
	}
	
#if (USER/100)==0//电科院测试
	#if ((Project/100)==2)//集中器
//按终端地址配置档案
	i=MRR(ADDR_4001+7,1);
	i&=0x0f;
	//采集档案配置3
	x=MRR(ADDR_6000+(3*LENper_6000)+55,1);
	x&=0xf0;
	x|=i;
	MWR(x,ADDR_6000+(3*LENper_6000)+55,1);
	MWR(x,ADDR_6000+(3*LENper_6000)+15,1);
	//采集档案配置4
	x=MRR(ADDR_6000+(4*LENper_6000)+55,1);
	x&=0xf0;
	x|=i;
	MWR(x,ADDR_6000+(4*LENper_6000)+55,1);
	MWR(x,ADDR_6000+(4*LENper_6000)+15,1);
	#endif
#endif
	
	ms->ms_free(pSOUR);
	DL698_SETINIT_EVENT(Command,pOAD);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
	DL698_SETINIT_601E(Command);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化

#if (USER/100)==17//江苏版
	Init_6000_2_1_Addr();//档案序号1通信地址为ADDR_4001地址(固定配为交采时用)
#endif
	if(Command==7)
	{
		WWDT_RESET();//立即使能看门狗复位
	}
}

void Init_6000_2_1_Addr(void)//档案序号1通信地址为ADDR_4001地址(固定配为交采时用)
{
	if(sizeof(InitData_6000_2_1)>LEN_DATABUFF)
	{
		return;
	}
	MR(ADDR_DATABUFF,(u32)InitData_6000_2_1,sizeof(InitData_6000_2_1));
	//取终端地址
	MR(ADDR_DATABUFF+10,ADDR_4001+2,6);
	//
	MW(ADDR_DATABUFF,ADDR_6000+LENper_6000,sizeof(InitData_6000_2_1));
}
 





