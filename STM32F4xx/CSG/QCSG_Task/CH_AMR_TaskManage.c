/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN13.c
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网AFN13处理文件
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "Terminal_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#if (((Project/100)==2) || ((Project/100)==5) || ((Project/100)==9))

const u32 RT_AMR_SE_DI[MAX_RT_AMR_SE_DI_NUM] = 
{
	0x0201FF00,	//电压数据块
	0x0202FF00,	//电流数据块
	0x0203FF00,	//有功功率数据块
	//0x0204FF00,	//无功功率数据块
	0x0206FF00,	//功率因数数据块
	0x0001FF00,	//正向有功电能数据块
	0x0002FF00		//反向有功电能数据块
};

//err20181012 配变 修改 曲线冻结数据项
const u32 HourFreez_SE_DI[MAX_HOUR_SE_DI_NUM] = 
{
	0x0001FF00,	//正向有功电能数据块
	0x0002FF00,	//反向有功电能数据块
	0x0003FF00,	//组合无功 1 数据块
	0x0004FF00,	//组合无功 2 数据块
	0x0201FF00,	//电压数据块
	0x0202FF00,	//电流数据块
	0x0206FF00		//功率因数数据块
};


const u32 DayFreez_SE_DI[MAX_DAY_SE_DI_NUM] = 
{
	0x050601FF,
	0x050602FF,
	0x050603FF,
	0x050604FF,
	0x050605FF,
	0x050606FF,
	0x050607FF,
	0x050608FF,
	0x050609FF,
	0x05060AFF,
	0x05060BFF,
	0x05060CFF
};


#if ((Project/100)==5  || ((Project/100)==9))
#define E2000032_645_07_NUM			(1)                      //II型集中器增加时钟电池电压低告警ybzyl20180627
const DI_645Struct E2000032_645_07[E2000032_645_07_NUM] = 
{
	{0x040005FF, 14, 14, 2},
};
#endif

//err20190215 改变失压失流潮流反向告警产生的方式(判断状态字 -> 判断生成总次数)
#define E200000D_645_07_NUM			(10)
const DI_645Struct E200000D_645_07[E200000D_645_07_NUM] = 
{
	{0x1B010001, 3,  3,  3},		// 3 byte A 相潮流反向总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};

#define E200000E_645_07_NUM			(10)
const DI_645Struct E200000E_645_07[E200000E_645_07_NUM] = 
{
	{0x1B020001, 3,  3,  3},		// 3 byte B 相潮流反向总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};

#define E200000F_645_07_NUM			(10)
const DI_645Struct E200000F_645_07[E200000F_645_07_NUM] = 
{
	{0x1B030001, 3,  3,  3},		// 3 byte C 相潮流反向总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};

#define E2000013_645_07_NUM			(10)
const DI_645Struct E2000013_645_07[E2000013_645_07_NUM] = 
{
	{0x18010001, 3,  3,  3},		// 3 byte A 相失流总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};

#define E2000014_645_07_NUM			(10)
const DI_645Struct E2000014_645_07[E2000014_645_07_NUM] = 
{
	{0x18020001, 3,  3,  3},		// 3 byte B 相失流总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};
	
#define E2000015_645_07_NUM			(10)
const DI_645Struct E2000015_645_07[E2000015_645_07_NUM] = 
{
	{0x18030001, 3,  3,  3},		// 3 byte C 相失流总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};

#define E2000016_645_07_NUM			(10)
const DI_645Struct E2000016_645_07[E2000016_645_07_NUM] = 
{
	{0x10010001, 3,  3,  3},		// 3 byte A 相失压总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};


#define E2000017_645_07_NUM			(10)
const DI_645Struct E2000017_645_07[E2000017_645_07_NUM] = 
{
	{0x10020001, 3,  3,  3},		// 3 byte B 相失压总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};


#define E2000018_645_07_NUM			(10)
const DI_645Struct E2000018_645_07[E2000018_645_07_NUM] = 
{
	{0x10030001, 3,  3,  3},		// 3 byte C 相失压总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}			// 4*2 byte 总功率因数，A B C 相功率因数
};



#define E200003B_645_07_NUM			(1)
const DI_645Struct E200003B_645_07[E200003B_645_07_NUM] = 
{
	{0x04000503, 2, 2, 2},//err20190312 台体不支持读数据块
	//{0x04000504, 2, 2, 2}
	//{0x040005FF, 14, 14, 2},
};

#define E200003C_645_07_NUM			(1)
const DI_645Struct E200003C_645_07[E200003C_645_07_NUM] = 
{
	//{0x04000503, 2, 2, 2}
	{0x040005FF, 14, 14, 2},
};

#define E200003E_645_07_NUM			(2)
const DI_645Struct E200003E_645_07[E200003E_645_07_NUM] = 
{
	{0x04000101, 4, 4, 4},
	{0x04000102, 3, 3, 3}
};

#define E2000036_645_07_NUM			(1)
const DI_645Struct E2000036_645_07[E2000036_645_07_NUM] = 
{
	{0x03300500, 3, 3, 3}
};

#define E200002B_645_07_NUM			(5)	//(1)
const DI_645Struct E200002B_645_07[E200002B_645_07_NUM] = 
{
//err20190215 该告警数据项缺失 增加下列4个
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能

	{0x00900200, 4,  4,  4}			// 4 byte 剩余金额
};

#define E200002C_645_07_NUM			(8)
const DI_645Struct E200002C_645_07[E200002C_645_07_NUM] = 
{
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 4},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 4}		// 4*3 byte 无功功率，A B C 相无功功率
};

#define E200002E_645_07_NUM			(9)
const DI_645Struct E200002E_645_07[E200002E_645_07_NUM] = 
{
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}		// 4*2 byte 总功率因数，A B C 相功率因数
};


#define E2000035_645_07_NUM			(10)
const DI_645Struct E2000035_645_07[E2000035_645_07_NUM] = 
{
	{0x03300000, 3,  3,  3},		// 3 byte 编程总次数
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
	{0x00030000, 4,  4,  4},		// 4 byte 当前组合无功 1 总电能
	{0x00040000, 4,  4,  4},		// 4 byte 当前组合无功 2 总电能
	{0x0201FF00, 6,  6,  2},		// 3*2 byte 电压数据块，A B C 相电压
	{0x0202FF00, 9,  9,  3},		// 3*3 byte 电流数据块，A B C 相电流
	{0x0203FF00, 12, 12, 3},		// 4*3 byte 有功功率，A B C 相有功功率
	{0x0204FF00, 12, 12, 3},		// 4*3 byte 无功功率，A B C 相无功功率
	{0x0206FF00, 8,  8,  2}		// 4*2 byte 总功率因数，A B C 相功率因数
};

#define E2000040_645_07_NUM			(6)
const DI_645Struct E2000040_645_07[E2000040_645_07_NUM] = 
{
	{0x10010001, 3, 3, 3},			// A B C 相失压次数及时间
	{0x10010002, 3, 3, 3},
	{0x10020001, 3, 3, 3},
	{0x10020002, 3, 3, 3},
	{0x10030001, 3, 3, 3},
	{0x10030002, 3, 3, 3}
};

#define E2000041_645_07_NUM			(6)
const DI_645Struct E2000041_645_07[E2000041_645_07_NUM] = 
{
	{0x18010001, 3, 3, 3},			// A B C 相失流次数及时间
	{0x18010002, 3, 3, 3},
	{0x18020001, 3, 3, 3},
	{0x18020002, 3, 3, 3},
	{0x18030001, 3, 3, 3},
	{0x18030002, 3, 3, 3}
};

#define E2000042_645_07_NUM			(6)
const DI_645Struct E2000042_645_07[E2000042_645_07_NUM] = 
{
	{0x1B010001, 3, 3, 3},			// A B C 相潮流反向次数及时间
	{0x1B010002, 3, 3, 3},
	{0x1B020001, 3, 3, 3},
	{0x1B020002, 3, 3, 3},
	{0x1B030001, 3, 3, 3},
	{0x1B030002, 3, 3, 3}
};

#define E200003D_645_07_NUM			(0)


//err20190218 将事件数据临时放在此处抄读
#define E201000E_645_07_NUM			(2)
const DI_645Struct E201000E_645_07[E201000E_645_07_NUM] = 
{
	{0x00010000, 4,  4,  4},		// 4 byte 当前正向有功总电能
	{0x00020000, 4,  4,  4},		// 4 byte 当前反向有功总电能
};
#define E201000E_645_97_NUM			(0)
//err20190218 将事件数据临时放在此处抄读


#if (Project == 500) || ((Project/100)==9)
	#define E2000032_645_97_NUM			(0)
#endif
#define E200000D_645_97_NUM			(0)
#define	E200000E_645_97_NUM			(0)
#define	E200000F_645_97_NUM			(0)
#define	E2000013_645_97_NUM			(0)
#define	E2000014_645_97_NUM			(0)
#define	E2000015_645_97_NUM			(0)
#define	E2000016_645_97_NUM			(0)
#define	E2000017_645_97_NUM			(0)
#define	E2000018_645_97_NUM			(0)

#define E200003B_645_97_NUM			(0)
#define E200003C_645_97_NUM			(0)
#define E200003D_645_97_NUM			(0)
#define E200003E_645_97_NUM			(0)
#define E2000036_645_97_NUM			(0)
#define E200002B_645_97_NUM			(0)
#define E200002C_645_97_NUM			(0)
#define E200002E_645_97_NUM			(0)
#define E2000035_645_97_NUM			(0)
#define E2000040_645_97_NUM			(0)
#define E2000041_645_97_NUM			(0)
#define E2000042_645_97_NUM			(0)

#if (Project == 200)
	#define ALARM_SINGLE_METER_END_POS	(7+1)//err20190218 增加 1个控制事件    	9个失压失流潮流反向告警非单相表告警
#endif

#if (Project == 500 || ((Project/100)==9))
	#define ALARM_SINGLE_METER_END_POS	(8+1)//err20190218 增加 1个控制事件 9个失压失流潮流反向告警非单相表告警
#endif

const SE_645_DIStruct Alarm_SE_DI[MAX_ALARM_SE_DI_NUM] = 
{
#if(Project == 500 || ((Project/100)==9))
	{0xE2000032, sizeof(ARD01Struct), E2000032_645_07_NUM, E2000032_645_97_NUM, E2000032_645_07, NULL},//时钟电池电压低告警 II型集中器
#endif
	{0xE200003B, sizeof(ARD12Struct), E200003B_645_07_NUM, E200003B_645_97_NUM, E200003B_645_07, NULL},	// 继电器变位告警 		终端
	{0xE200003C, sizeof(ARD12Struct), E200003C_645_07_NUM, E200003C_645_97_NUM, E200003C_645_07, NULL},	// 电能表拉合闸失败 终端
	{0xE200003D, sizeof(ARD21Struct), E200003D_645_07_NUM, E200003D_645_97_NUM, NULL,            NULL},	// 抄表失败 			终端
	{0xE200003E, sizeof(ARD13Struct), E200003E_645_07_NUM, E200003E_645_97_NUM, E200003E_645_07, NULL},	// 电能表时钟异常 		终端
	{0xE2000036, sizeof(ARD01Struct), E2000036_645_07_NUM, E2000036_645_97_NUM, E2000036_645_07, NULL},	// 电能表时段或费率更改 单相/多功能表
	{0xE200002B, sizeof(ARD04Struct), E200002B_645_07_NUM, E200002B_645_97_NUM, E200002B_645_07, NULL},	// 剩余电费不足 单相/多功能表
	{0xE200002C, sizeof(ARD03Struct), E200002C_645_07_NUM, E200002C_645_97_NUM, E200002C_645_07, NULL},	// 示度下降 		单相/多功能表
	{0xE200002E, sizeof(ARD02Struct), E200002E_645_07_NUM, E200002E_645_97_NUM, E200002E_645_07, NULL},	// 电能表停走 		单相/多功能表

		//err20190218 临时放置 以下位置为事件对应的数据标识
	{0xE201000E, sizeof(ERD06Struct), E201000E_645_07_NUM, E201000E_645_97_NUM, E201000E_645_07, NULL},	// 控制事件记录 终端
	
	// 以下位置为仅三相表支持的告警数据标识
	{0xE2000035, sizeof(ARD02Struct), E2000035_645_07_NUM, E2000035_645_97_NUM, E2000035_645_07, NULL},	// 电能表编程时间更改 多功能表
		//以下9个告警为新增加 err20190215 改变失压失流潮流反向告警产生的方式
	{0xE200000D, sizeof(ARD02Struct), E200000D_645_07_NUM, E200000D_645_97_NUM, E200000D_645_07, NULL}, // A相潮流反向告警 多功能表
	{0xE200000E, sizeof(ARD02Struct), E200000E_645_07_NUM, E200000E_645_97_NUM, E200000E_645_07, NULL}, // B相潮流反向告警 多功能表
	{0xE200000F, sizeof(ARD02Struct), E200000F_645_07_NUM, E200000F_645_97_NUM, E200000F_645_07, NULL}, // C相潮流反向告警 多功能表

	{0xE2000013, sizeof(ARD02Struct), E2000013_645_07_NUM, E2000013_645_97_NUM, E2000013_645_07, NULL}, // A相失流告警 多功能表
	{0xE2000014, sizeof(ARD02Struct), E2000014_645_07_NUM, E2000014_645_97_NUM, E2000014_645_07, NULL}, // B相失流告警 多功能表
	{0xE2000015, sizeof(ARD02Struct), E2000015_645_07_NUM, E2000015_645_97_NUM, E2000015_645_07, NULL}, // C相失流告警 多功能表

	{0xE2000016, sizeof(ARD02Struct), E2000016_645_07_NUM, E2000016_645_97_NUM, E2000016_645_07, NULL}, // A相失压告警 多功能表
	{0xE2000017, sizeof(ARD02Struct), E2000017_645_07_NUM, E2000017_645_97_NUM, E2000017_645_07, NULL}, // B相失压告警 多功能表
	{0xE2000018, sizeof(ARD02Struct), E2000018_645_07_NUM, E2000018_645_97_NUM, E2000018_645_07, NULL}, // C相失压告警 多功能表
		//以下3个告警目前没有用到
	{0xE2000040, sizeof(ARD15Struct), E2000040_645_07_NUM, E2000040_645_97_NUM, E2000040_645_07, NULL},	// 电能表A、B、C相失压总次数
	{0xE2000041, sizeof(ARD16Struct), E2000041_645_07_NUM, E2000041_645_97_NUM, E2000041_645_07, NULL},	// 电能表A、B、C相失流总次数
	{0xE2000042, sizeof(ARD17Struct), E2000042_645_07_NUM, E2000042_645_97_NUM, E2000042_645_07, NULL}		// 电能表A、B、C 相潮流反向总次数
				
};


void CH_AMR_TaskCtrlDataValidCheck(u32 PORTn)
{
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	RS485PortParaStruct RS485PortPara;
	u32 FlashAddr = 0;
	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	if(AMRCtrl == NULL) return;

	switch((u8)PORTn)
	{
		case RS485_1PORT:
			FlashAddr = ADDR_RS485PortParaTbl;
			break;
		case RS485_2PORT:
			FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
			break;
		case RS485_3PORT:
			FlashAddr = ADDR_RS485PortParaTbl + 2*sizeof(RS485PortParaStruct);
			break;
		default:
			FlashAddr = 0;
			break;
	}

	if(FlashAddr != 0)
	{
		MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
		if(RS485PortPara.Function != UART_FUNC_AMR)
		{
			MC(0, (u32)AMRCtrl, sizeof(AMRCtrlStruct));
		}
	}
	
	if((MeterAddrCheck(AMRCtrl->Addr) != NO_ERROR) || (TimeValidCheck(&AMRCtrl->LastCheckTime) != NO_ERROR) ||
	   (AMRCtrl->PnNum > MAX_PN_NUM) || (TimeValidCheck(&AMRCtrl->LastDayFrozenTime) != NO_ERROR))
	{
		MC(0, (u32)AMRCtrl, sizeof(AMRCtrlStruct));
	}

	HAL_GetBeforePoweroffTime(&YYTime);
	HAL_GetSystemTime(&RealTime);

	AMRCtrl->CurNW_DI_Step = 0;
	AMRCtrl->Cur645_DI_Step = 0;
	AMRCtrl->Cur645DataLen = 0;
	AMRCtrl->AMRTotalDataLen = 0;
	AMRCtrl->PLCTaskIDAckNum = 0;
	AMRCtrl->PLCTaskIDAddNum = 0;
	if((RealTime.u8Year == YYTime.YearL) && (RealTime.u8Month == YYTime.Month) && (RealTime.u8Day == YYTime.Day))
	{
		if(((AMRCtrl->AMR_ID & (AMR_DAY_FREEZ_ID | AMR_DAY_FREEZ_PATCH_ID | AMR_HOUR_FREEZ_ID)) != 0) && (AMRCtrl->State != AMR_Idle))
		{
			if((AMRCtrl->State == AMR_Tx) || (AMRCtrl->State == AMR_WaitRx))
			{
				AMRCtrl->OldState = AMR_Tx;
			}
			AMRCtrl->State = AMR_Resume;
		}
		else
		{
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
		}
	}
	else
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
	}
}

void AMRTaskManage(void)
{
	RealTimeStruct RealTime;
	AMRTaskManageStruct* AMRTask = (AMRTaskManageStruct*)ADDR_AMRTaskManageCtrl;
	if(GetTerminalPower_S_TimerValue() > 0) return;
	HAL_GetSystemTime(&RealTime);
	if(AMRTask->LastCheckTime.u8Minute == RealTime.u8Minute) return;
	
	AMRTask->RS485_1_AMRTask = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_1PORT);
	AMRTask->RS485_2_AMRTask = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_2PORT);
	AMRTask->RS485_3_AMRTask = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_3PORT);
	AMRTask->PLC_AMRTask = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PLC_PORT);

	//if((AMRTask->RS485_1_AMRTask->State == AMR_Idle) && (AMRTask->RS485_2_AMRTask->State == AMR_Idle) &&
	//   (AMRTask->RS485_3_AMRTask->State == AMR_Idle) && (AMRTask->PLC_AMRTask->State == AMR_Idle))
	{
		//MC(0, (u32)AMRTask->NormalTaskExe, MAX_NORMAL_TASK_NUM);
		//MC(0, (u32)AMRTask->RelayTaskExe, MAX_RELAY_TASK_NUM);
		//AMRTaskManageCheckNormalTask(AMRTask, &RealTime);
		//AMRTaskManageCheckRelayTask(AMRTask, &RealTime);
		AMRTaskManageCheckAlarmTask(AMRTask, &RealTime);

		AMRTask->LastCheckTime.u8Second = RealTime.u8Second;
		AMRTask->LastCheckTime.u8Minute = RealTime.u8Minute;
		AMRTask->LastCheckTime.u8Hour   = RealTime.u8Hour;
		AMRTask->LastCheckTime.u8Day    = RealTime.u8Day;
		AMRTask->LastCheckTime.u8Month  = RealTime.u8Month;
		AMRTask->LastCheckTime.u8Year   = RealTime.u8Year;
		AMRTask->LastCheckTime.u8Week   = RealTime.u8Week;
		AMRTask->LastCheckTime.u8Sync   = RealTime.u8Sync;
	}
}


void AMRTaskManageCheckAlarmTask(AMRTaskManageStruct* AMRTask, RealTimeStruct* CurTime)
{
	u32 SE_DI_Pos = 0, AlarmSE_DI_Num = 0, MaskPos = 0, MaskBit = 0, DI_645 = 0;
	u8 DI_Num = 0, Index = 0;
	AlarmEventMaskParaStruct AlarmMaskPara;
	if(AMRTask == NULL) return;
	if(AMRTask->AlarmTaskCheckLock != 0) return;

	MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
	AMRTask->AlarmTask.NW_DI_Num = 0;
	AMRTask->AlarmTask.DI_645_07_Num = 0;
	AMRTask->AlarmTask.DI_645_97_Num = 0;
	//AlarmSE_DI_Num = MAX_ALARM_SE_DI_NUM;
	AlarmSE_DI_Num = TEST_ALARM_SE_DI_NUM;
	for(SE_DI_Pos = 0; SE_DI_Pos < AlarmSE_DI_Num; SE_DI_Pos++)
	{

		if((Alarm_SE_DI[SE_DI_Pos].SE_DI & 0x00010000) == 0)//err20190218 判断为告警数据标识
		{
			MaskPos = (Alarm_SE_DI[SE_DI_Pos].SE_DI & 0x000000FF) - 1;
			MaskBit = 1 << (MaskPos & 0x00000007);
			MaskPos >>= 3;	
			if((AlarmMaskPara.AlarmJudgeMask[MaskPos] & MaskBit) == 0) continue;
		}
		else	//判断为事件数据标识
		{
			MaskPos = (Alarm_SE_DI[SE_DI_Pos].SE_DI & 0x000000FF) - 1;
			MaskBit = 1 << (MaskPos & 0x00000007);
			MaskPos >>= 3;	
			if((AlarmMaskPara.EventRecordMask[MaskPos] & MaskBit) == 0) continue;
		}
		
		AMRTask->AlarmTask.NW_DI[AMRTask->AlarmTask.NW_DI_Num++] = Alarm_SE_DI[SE_DI_Pos].SE_DI;
		for(DI_Num = 0; DI_Num < Alarm_SE_DI[SE_DI_Pos].DI_645_07_Num; DI_Num++)
		{
			DI_645 = Alarm_SE_DI[SE_DI_Pos].DI_645_07[DI_Num].DI_645;
			for(Index = 0; Index < AMRTask->AlarmTask.DI_645_07_Num; Index++)	// MAX_ALARM_645_DI_NUM
			{
				if(DI_645 == AMRTask->AlarmTask.DI_645_07[Index]) break;
			}

			if(Index >= AMRTask->AlarmTask.DI_645_07_Num)
			{
				Index = AMRTask->AlarmTask.DI_645_07_Num;
				AMRTask->AlarmTask.DI_645_07[Index] = DI_645;
				AMRTask->AlarmTask.DI_645_07_Num++;

				if(SE_DI_Pos > ALARM_SINGLE_METER_END_POS)
				{
					AMRTask->AlarmTask.DI_645_07_Flag[Index] = DI_645_FLAG_MULTI_METER;
				}
				else
				{
					AMRTask->AlarmTask.DI_645_07_Flag[Index] = DI_645_FLAG_SINGLE_METER | DI_645_FLAG_MULTI_METER;
				}
			}
		}

		for(DI_Num = 0; DI_Num < Alarm_SE_DI[SE_DI_Pos].DI_645_97_Num; DI_Num++)
		{
			DI_645 = Alarm_SE_DI[SE_DI_Pos].DI_645_97[DI_Num].DI_645;
			for(Index = 0; Index < AMRTask->AlarmTask.DI_645_97_Num; Index++)	// MAX_ALARM_645_DI_NUM
			{
				if(DI_645 == AMRTask->AlarmTask.DI_645_97[Index]) break;
			}

			if(Index >= AMRTask->AlarmTask.DI_645_97_Num)
			{
				Index = AMRTask->AlarmTask.DI_645_97_Num;
				AMRTask->AlarmTask.DI_645_97[Index] = DI_645;
				AMRTask->AlarmTask.DI_645_97_Num++;

				if(SE_DI_Pos > ALARM_SINGLE_METER_END_POS)
				{
					AMRTask->AlarmTask.DI_645_97_Flag[Index] = DI_645_FLAG_MULTI_METER;
				}
				else
				{
					AMRTask->AlarmTask.DI_645_97_Flag[Index] = DI_645_FLAG_SINGLE_METER | DI_645_FLAG_MULTI_METER;
				}
			}
		}		
	}

}


void AMRTaskManageCheckNormalTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime)
{
	NormalTaskParaStruct NormalTaskPara;
	u32 i = 0;	
	RealTimeStruct* LastTime = NULL;
	switch(PORTn)
	{
		case RS485_1PORT:
			i = 0;
			break;
		case RS485_2PORT:
			i = 1;
			break;
		case RS485_3PORT:
			i = 2;
			break;
		case PLC_PORT:
			i = 3;
			break;
		default:
			i = 0;
			break;
	}
	LastTime = (RealTimeStruct*)(ADDR_LastNormalTaskAMRTime + i * MAX_NORMAL_TASK_NUM * sizeof(RealTimeStruct));
	MC(0, (u32)AMRCtrl->NormalTaskExe, MAX_NORMAL_TASK_NUM);
	for(i = 0; i < MAX_NORMAL_TASK_NUM; i++)
	{
		MR((u32)&NormalTaskPara, ADDR_NormalTaskPara + i*sizeof(NormalTaskParaStruct), sizeof(NormalTaskParaStruct));
		if(NormalTaskPara.Flag != 0x01) continue;
		if(NormalTaskPara.DANum == 0) continue;
		if(NormalTaskPara.DataIDNum == 0) continue;
		if(NormalTaskPara.DataIDNum > AMR_MAX_DATA_ID) continue;
		if(NormalTaskPara.SamplePeriod == 0) continue;
		if(NormalTaskPara.SamplePeriodUnit > QCSG_SAMPLE_UNIT_MONTH) continue;
		if(TimeValidCheck(&LastTime[i]) != NO_ERROR)
		{
			MR((u32)&LastTime[i], (u32)&NormalTaskPara.SampleTime, sizeof(RealTimeStruct));
		}
		AMRCtrl->NormalTaskExe[i] = QCSG_Judge_TaskTiming(&LastTime[i], CurTime, &NormalTaskPara.SampleTime, NormalTaskPara.SamplePeriod, NormalTaskPara.SamplePeriodUnit);
		if(AMRCtrl->NormalTaskExe[i] == 0) continue;
		MR((u32)&LastTime[i], (u32)CurTime, sizeof(RealTimeStruct));

		if((NormalTaskPara.ExecCnt != 0) && (NormalTaskPara.ExecCnt <= NormalTaskPara.RealExeCnt))//err20180404-07 执行次数不只对PLC有效 对应上报次数
		{
			AMRCtrl->NormalTaskExe[i] = 0;//上报次数达到设定值时 同时取消 采集
		}
		//if(NormalTaskPara.ExecCnt != 0)//err20180404-07 执行次数不只对PLC有效 对应上报次数
		//{
			//if(NormalTaskPara.ExecCnt > NormalTaskPara.RealExeCnt)
			//{	
				//if(PORTn == PLC_PORT)
				//{
				//	NormalTaskPara.RealExeCnt++;
				//	MW((u32)&NormalTaskPara, ADDR_NormalTaskPara + i*sizeof(NormalTaskParaStruct), sizeof(NormalTaskParaStruct));
				//}	
			//}
			//else
			//{
			//	AMRCtrl->NormalTaskExe[i] = 0;
			//}
		//}
	}
}

void AMRTaskManageCheckRelayTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime)
{
	RelayTaskParaStruct RelayTaskPara;
	u32 i = 0, PortID = 0, PortID2 = PORT_ID_WIRELESS;	
	RealTimeStruct* LastTime = NULL;
	switch(PORTn)
	{
		case RS485_1PORT:
			i = 0;
			PortID = PORT_ID_RS485_1;
			break;
		case RS485_2PORT:
			i = 1;
			PortID = PORT_ID_RS485_2;
			break;
		case RS485_3PORT:
			i = 2;
			PortID = PORT_ID_RS485_3;
			break;
		case PLC_PORT:
			i = 3;
			PortID = PORT_ID_PLC;
			break;
		default:
			i = 0;
			break;
	}
	LastTime = (RealTimeStruct*)(ADDR_LastRelayTaskAMRTime + i * MAX_RELAY_TASK_NUM * sizeof(RealTimeStruct));
	MC(0, (u32)AMRCtrl->RelayTaskExe, MAX_RELAY_TASK_NUM);
	for(i = 0; i < MAX_RELAY_TASK_NUM; i++)
	{
		MR((u32)&RelayTaskPara, ADDR_RelayTaskPara + i*sizeof(RelayTaskParaStruct), sizeof(RelayTaskParaStruct));
		if(RelayTaskPara.Flag != 0x01) continue;
		if(RelayTaskPara.Length == 0) continue;
		if(RelayTaskPara.SamplePeriod == 0) continue;
		if(RelayTaskPara.SamplePeriodUnit > QCSG_SAMPLE_UNIT_MONTH) continue;
		if((RelayTaskPara.ComPortId != PortID) && (RelayTaskPara.ComPortId != PortID2)) continue;
		//if(DLT645FrameCheck(RelayTaskPara.Frame, RelayTaskPara.Length) != NO_ERROR) continue;
		if(TimeValidCheck(&LastTime[i]) != NO_ERROR)
		{
			MR((u32)&LastTime[i], (u32)&RelayTaskPara.SampleTime, sizeof(RealTimeStruct));
		}
		AMRCtrl->RelayTaskExe[i] = QCSG_Judge_TaskTiming(&LastTime[i], CurTime, &RelayTaskPara.SampleTime, RelayTaskPara.SamplePeriod, RelayTaskPara.SamplePeriodUnit);
		if(AMRCtrl->RelayTaskExe[i] == 0) continue;

		MR((u32)&LastTime[i], (u32)CurTime, sizeof(RealTimeStruct));

		if((RelayTaskPara.ExecCnt != 0) && (RelayTaskPara.ExecCnt <= RelayTaskPara.RealExeCnt))//err20180404-07 执行次数不只对PLC有效 对应上报次数
		{
			AMRCtrl->RelayTaskExe[i] = 0;//上报次数达到设定值时 同时取消 采集
		}
		
		/*
		if(RelayTaskPara.ExecCnt != 0) //err20180404-07 执行次数不只对PLC有效 对应上报次数
		{
			if(RelayTaskPara.ExecCnt > RelayTaskPara.RealExeCnt)
			{	
				//if(PORTn == PLC_PORT)err20180404-07 执行次数不只对PLC有效
				{
					RelayTaskPara.RealExeCnt++;
					MW((u32)&RelayTaskPara, ADDR_RelayTaskPara + i*sizeof(RelayTaskParaStruct), sizeof(RelayTaskParaStruct));
				}	
			}
			else
			{
				AMRCtrl->RelayTaskExe[i] = 0;
			}
		}
		*/
	}
}


void CheckAlarmTasktoExecute(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime)
{
	u32 i = 0, ChAddr = 0, ChSetAddr = 0;
	AMRChTimeStruct AMRChTime;
	AMRChSetStruct AMRChSet;
	RealTimeStruct RefTime;
	AMRTaskManageStruct* AMRTask = (AMRTaskManageStruct*)ADDR_AMRTaskManageCtrl;
	
	if(AMRCtrl == NULL) return;
	AMRCtrl->AlarmTask = &AMRTask->AlarmTask;
	if(AMRCtrl->AlarmTask->NW_DI_Num == 0) return;
	if((AMRCtrl->AlarmTask->DI_645_07_Num == 0) && (AMRCtrl->AlarmTask->DI_645_97_Num == 0)) return;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			ChAddr = ADDR_AMRChTimeTbl;
			ChSetAddr = ADDR_AMRChSetTbl;
			break;
		case RS485_2PORT:
			ChAddr = ADDR_AMRChTimeTbl + sizeof(AMRChTimeStruct);
			ChSetAddr = ADDR_AMRChSetTbl + sizeof(AMRChSetStruct);
			break;
		case RS485_3PORT:
			ChAddr = ADDR_AMRChTimeTbl + 2*sizeof(AMRChTimeStruct);
			ChSetAddr = ADDR_AMRChSetTbl + 2*sizeof(AMRChSetStruct);
			break;
		case PLC_PORT:
			ChAddr = ADDR_AMRChTimeTbl + 3*sizeof(AMRChTimeStruct);
			ChSetAddr = ADDR_AMRChSetTbl + 3*sizeof(AMRChSetStruct);
			break;
		default:
			ChAddr = 0;
			ChSetAddr = 0;
			break;
	}
	if((ChAddr == 0) || (ChSetAddr == 0)) return;	

	if(PORTn != PLC_PORT)
	{
		MR((u32)&AMRChSet, ChSetAddr, sizeof(AMRChSetStruct));
		if(AMRChSet.Flag != 0x01) return;
		MR((u32)&AMRChTime, ChAddr, sizeof(AMRChTimeStruct));
		if(AMRChTime.PeriodUnit > PERIOD_UNIT_MONTH) return;
	}
	else
	{
		MR((u32)&AMRChSet, ChSetAddr, sizeof(AMRChSetStruct));
		MR((u32)&AMRChTime, ChAddr, sizeof(AMRChTimeStruct));
		if((AMRChSet.Flag != 0x01) || (AMRChTime.PeriodUnit > PERIOD_UNIT_MONTH))
		{
			ChAddr = ADDR_AMRChTimeTbl + 4*sizeof(AMRChTimeStruct);
			ChSetAddr = ADDR_AMRChSetTbl + 4*sizeof(AMRChSetStruct);
			MR((u32)&AMRChSet, ChSetAddr, sizeof(AMRChSetStruct));
			MR((u32)&AMRChTime, ChAddr, sizeof(AMRChTimeStruct));
			if((AMRChSet.Flag != 0x01) || (AMRChTime.PeriodUnit > PERIOD_UNIT_MONTH)) return;
		}
	}
	
	RefTime.u8Year   = AMRChTime.Year;
	RefTime.u8Month  = AMRChTime.Month;
	RefTime.u8Day    = AMRChTime.Day;
	RefTime.u8Hour   = AMRChTime.Hour;
	RefTime.u8Minute = AMRChTime.Minute;
	RefTime.u8Second = 0;

	if(TimeValidCheck(&AMRCtrl->LastChAMRTime) != NO_ERROR)
	{
		AMRCtrl->LastChAMRTime.u8Year   = AMRChTime.Year;
		AMRCtrl->LastChAMRTime.u8Month  = AMRChTime.Month;
		AMRCtrl->LastChAMRTime.u8Day    = AMRChTime.Day;
		AMRCtrl->LastChAMRTime.u8Hour   = AMRChTime.Hour;
		AMRCtrl->LastChAMRTime.u8Minute = AMRChTime.Minute;
		AMRCtrl->LastChAMRTime.u8Second = 0;
	}

	//err20190305 此处为过台体测试 缩短周期抄表时间，使告警能够及时产生。
	//评审后决定当设置周期为5min时，缩短周期抄表时间为3min 其他值则保持不变以用于现场。
	#if (TEST_ON == 1)
		if(AMRChTime.PeriodUnit == PERIOD_UNIT_MINUTE)
		{
			//if(AMRChTime.Period > 3) AMRChTime.Period -= 2;
			if(AMRChTime.Period == 5) AMRChTime.Period = 3;//err20190305 若周期设置为5min 则缩短周期为3min
		}
	#endif

	i = QCSG_Judge_TaskTiming(&AMRCtrl->LastChAMRTime, CurTime, &RefTime, AMRChTime.Period, AMRChTime.PeriodUnit);
	
	if((u8)i == TRUE)
	{
		// 此处将抄 南网告警数据标识的数量设置为1，是因为；不为每个南网数据标识分别抄表，而是将所有南网数据标识
		// 对应的 645 标识集合在一起，统一抄表，可以极大减少抄表次数(因为不同告警南网标识可能抄同样的645数据标识)
		AMRCtrl->AlarmNwDINum = 0x01;
		AMRCtrl->AMR_ID |= AMR_ALARM_TASK_ID;
		MR((u32)&AMRCtrl->LastChAMRTime, (u32)CurTime, sizeof(RealTimeStruct));
	}
}


void CheckNormalTasktoExecute(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime)
{
	u32 i = 0;

	if(AMRCtrl == NULL) return;
	i = Get_AMR_NormalTaskCtrlAddr(PORTn);
	if(i == 0) return;
	
	AMRCtrl->NormalTask = (NormalTaskCtrlStruct*)i;
	for(i = 0; i < MAX_NORMAL_TASK_NUM; i++)
	{
		if(AMRCtrl->NormalTaskExe[i] != 0)
		{		
			AMRCtrl->AMR_ID |= AMR_NORMAL_TASK_ID;
			break;
		}
	}
}

void CheckRelayTasktoExecute(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime)
{
	u32 i = 0;
	
	if(AMRCtrl == NULL) return;
	i = Get_AMR_RelayTaskCtrlAddr(PORTn);
	if(i == 0) return;
	
	AMRCtrl->RelayTask = (RelayTaskCtrlStruct*)i;
	for(i = 0; i < MAX_RELAY_TASK_NUM; i++)
	{
		if(AMRCtrl->RelayTaskExe[i] != 0)
		{		
			AMRCtrl->AMR_ID |= AMR_RELAY_TASK_ID;
			break;
		}
	}
}


void RealTimeAMRTask(void)
{
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	switch(Task->State)
	{
		case RT_AMR_IDLE:
			RT_AMR_TaskIdle(Task);
			break;
		case RT_AMR_SEND_REQ:
			RT_AMR_TaskSendReq(Task);
			break;
		case RT_AMR_WAIT_REQ_RESP:
			RT_AMR_TaskWaitReqResp(Task);
			break;
		//case RT_AMR_SEND_DATA:
		//	RT_AMR_TaskSendData(Task);
		//	break;
		case RT_AMR_WAIT_DATA_RESP:
			RT_AMR_TaskWaitDataResp(Task);
			break;
		case RT_AMR_ACK_TO_SRC_PORT:
			RT_AMR_TaskAckToSrcPort(Task);
			break;
		case RT_AMR_DONE:
			RT_AMR_TaskDone(Task);
			Task->State = RT_AMR_IDLE;
			break;
		default:
			Task->State = RT_AMR_IDLE;
			break;
	}

	if((Task->State == RT_AMR_IDLE) && (Task->Lock != TASK_UNLOCK))
	{
		Task->Lock = TASK_UNLOCK;
	}
}

ResultEnum RT_AMR_TaskIdle(RT_AMRCtrlStruct* Task)
{
	QCSG_PnListStruct* pQCSG_PnList;
	UartRxChCtrlStruct* UartRxChCtrl = NULL;
	QCSG_LCD_RAMDATA_S* pLcdRTAMR = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	u8 SrcPort = 0;
	if((Task->Flag & RT_AMR_RS485_CH1_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_RS485_CH1_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(RS485_1PORT);
		SrcPort = RS485_1PORT;
	}
	else if((Task->Flag & RT_AMR_RS485_CH2_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_RS485_CH2_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(RS485_2PORT);
		SrcPort = RS485_2PORT;
	}
	else if((Task->Flag & RT_AMR_RS485_CH3_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_RS485_CH3_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(RS485_3PORT);
		SrcPort = RS485_3PORT;
	}
	else if((Task->Flag & RT_AMR_RS232_PORT_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_RS232_PORT_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(RS232PORT);
		SrcPort = RS232PORT;
	}
	else if((Task->Flag & RT_AMR_GPRS_PORT_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_GPRS_PORT_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(GPRSPORT);
		SrcPort = GPRSPORT;
	}
	else if((Task->Flag & RT_AMR_IR_PORT_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_IR_PORT_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(IRPORT);
		SrcPort = IRPORT;
	}
	else if((Task->Flag & RT_AMR_ETHE_PORT_REQ) != 0)
	{
		Task->Flag &= ~RT_AMR_ETHE_PORT_REQ;
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(ENETPORT);
		SrcPort = ENETPORT;
	}
	else 
	{
	#if ((Project/100)==9)//配变 0号测量点为交采测量点，若液晶召读实时数据则跳出集中器代码由负控代码处理
		if(pLcdRTAMR->u16Pn == 0)
			return NO_ERROR;
	#endif
		SrcPort = GUI_PORT;	
		if((pLcdRTAMR->u16Res1 & LCD_RT_AMR_REQ) != 0)
		{
			pLcdRTAMR->u16Res1 &= ~LCD_RT_AMR_REQ;

			Task->SrcDataPos = 0;
			Task->SrcFrameType = 0;
			Task->SrcDataType = DATA_TYPE_STRUCT;
			Task->SrcPort = GUI_PORT;
			//Task->PnListNum = MAX_RT_AMR_SE_DI_NUM;
			Task->PnIndex = 0;
			Task->RetryCnt = 0;
			Task->RequestCnt = 0;
			
			pQCSG_PnList = (QCSG_PnListStruct*)Task->SrcData;
			for(Task->PnListNum = 0; Task->PnListNum < MAX_RT_AMR_SE_DI_NUM; Task->PnListNum++)
			{
				pQCSG_PnList[Task->PnListNum].Pn = SwitchNumToPn(pLcdRTAMR->u16Pn);
				pQCSG_PnList[Task->PnListNum].Len = 0;
				pQCSG_PnList[Task->PnListNum].DI = RT_AMR_SE_DI[Task->PnListNum];	
			}
			Task->SrcDataLen = Task->PnListNum * sizeof(QCSG_PnListStruct);
			Task->State = RT_AMR_SEND_REQ;
		}
#if(TEST_ON == 1)//err 过台体 产生实时指令 用于切换采集器波特率
		else if((pLcdRTAMR->u16Res1 & LCD_RT_AMR_REQ_TEST_END) != 0)
		{
			pLcdRTAMR->u16Res1 &= ~LCD_RT_AMR_REQ_TEST_END;

			Task->SrcDataPos = 0;
			Task->SrcFrameType = 0;
			Task->SrcDataType = DATA_TYPE_STRUCT;
			Task->SrcPort = GUI_PORT;
			//Task->PnListNum = MAX_RT_AMR_SE_DI_NUM;
			Task->PnIndex = 0;
			Task->RetryCnt = 0;
			Task->RequestCnt = 0;
			Task->PnListNum = 1;
			
			pQCSG_PnList = (QCSG_PnListStruct*)Task->SrcData;
			pQCSG_PnList[0].Pn = SwitchNumToPn(9);
			pQCSG_PnList[0].Len = 0;
			pQCSG_PnList[0].DI = 0x0201FF00;//RT_AMR_SE_DI[Task->PnListNum];
			
			Task->SrcDataLen = sizeof(QCSG_PnListStruct);
			Task->State = RT_AMR_SEND_REQ;
		}
#endif
	}

	if((SrcPort != GUI_PORT) && (UartRxChCtrl != NULL) && (UartRxChCtrl->Empty == BUFFER_FULL))
	{
		MR((u32)Task->SrcData, (u32)UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
		Task->SrcDataPos = 0;
		Task->SrcDataLen = UartRxChCtrl->FrameLen;
		Task->SrcFrameType = UartRxChCtrl->FrameType;
		Task->SrcDataType = UartRxChCtrl->DataType;
		Task->SrcPort = UartRxChCtrl->SrcPort;
		Task->SrcPort = SrcPort;
		Task->PnListNum = UartRxChCtrl->PnListNum;
		Task->PnIndex = 0;
		Task->RetryCnt = 0;
		Task->RequestCnt = 0;
		MR((u32)&Task->QCSGFrameInfo, (u32)&UartRxChCtrl->QCSGFrameInfo, sizeof(QCSGFrameInfoStruct));
		Task->State = RT_AMR_SEND_REQ;
		UartRxChCtrl->Empty = BUFFER_EMPTY;

		if(UartRxChCtrl->DataType == DATA_TYPE_BCAST_CMD)
		{
			Task->DstPort = RS485_1PORT;
			Task->BCAST_BaudRate_Index = 0;
		}
	}
	return NO_ERROR;
}

//err20190312 按2400 1200 4800 9600 的顺序下发广播校时帧
const u8 BCAST_BaudRate_Tbl[4]={8,4,16,32};

ResultEnum RT_AMR_TaskSendReq(RT_AMRCtrlStruct* Task)
{
	RS485PortParaStruct RS485PortPara;
	QCSG_PnListStruct* pQCSG_PnList;
	AMRCtrlStruct* AMRCtrl = NULL;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	u32 PnNum = 0, Addr = 0, Index = 0, FlashAddr = 0;

#if ((Project/100)==9)
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;
#endif

	if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{
		if(Task->SrcDataPos != 0)
		{
			if(Task->DstPort == PLC_PORT)
			{
				Task->BCAST_BaudRate_Index = 0;
				Task->DstPort++;
			}
			else 
				Task->BCAST_BaudRate_Index ++ ;
			
			if(Task->BCAST_BaudRate_Index >= 4)
			{
				Task->BCAST_BaudRate_Index = 0;
				Task->DstPort++;
			}
		}
		while(Task->DstPort <= RS485_3PORT)
		{
			switch(Task->DstPort)
			{
				case RS485_1PORT:
					FlashAddr = ADDR_RS485PortParaTbl;
					MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
					RS485PortPara.BaudRate = BCAST_BaudRate_Tbl[Task->BCAST_BaudRate_Index];//err20190312 广播校时4波特率下发 //8;//err20190109 广播校时强制按波特率2400下发
					RS485PortPara.Parity = 1;
					RS485PortPara.DataBit = 8;
					RS485PortPara.StopBit = 0;
					break;
				case RS485_2PORT:
					FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
					MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
					RS485PortPara.BaudRate = BCAST_BaudRate_Tbl[Task->BCAST_BaudRate_Index];//err20190312 广播校时4波特率下发//8;//err20190109 广播校时强制按波特率2400下发
					RS485PortPara.Parity = 1;
					RS485PortPara.DataBit = 8;
					RS485PortPara.StopBit = 0;
					break;
				case RS485_3PORT:
					FlashAddr = ADDR_RS485PortParaTbl + 2*sizeof(RS485PortParaStruct);
					MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
					RS485PortPara.BaudRate = BCAST_BaudRate_Tbl[Task->BCAST_BaudRate_Index];//err20190312 广播校时4波特率下发//8;//err20190109 广播校时强制按波特率2400下发
					RS485PortPara.Parity = 1;
					RS485PortPara.DataBit = 8;
					RS485PortPara.StopBit = 0;
					break;
				case PLC_PORT:
				//case ACSAMPLEPORT:
					RS485PortPara.BaudRate = 32;
					RS485PortPara.Parity = 1;
					RS485PortPara.DataBit = 8;
					RS485PortPara.StopBit = 0;
					RS485PortPara.Function = UART_FUNC_AMR;
					break;
				default:
					RS485PortPara.Function = UART_FUNC_UNKNOWN;
					break;
			}
			if(RS485PortPara.Function == UART_FUNC_AMR) break;
			Task->DstPort++;
		}	
		//if(RS485PortPara.Function == UART_FUNC_AMR)
		if((Task->DstPort <= RS485_3PORT) && (RS485PortPara.Function == UART_FUNC_AMR))//err20190315 RS485_3PORT(5) > PLC_PORT(4) 原来485-III不支持广播校时 
		{	
			Task->NeedAck = 0;
			Task->SrcDataPos = 0;	
			pQCSG_PnList = (QCSG_PnListStruct*)&Task->SrcData[Task->SrcDataPos];
			Task->CurPn = pQCSG_PnList->Pn;
			Task->CurPnDataLen = pQCSG_PnList->Len;
			Task->CurNW_DI = pQCSG_PnList->DI;
			Task->SrcDataPos += sizeof(QCSG_PnListStruct);
			Addr = Get_AMR_TaskCtrlAddr(Task->DstPort);
			AMRCtrl = (AMRCtrlStruct*)Addr;
			Task->AMRCtrl = AMRCtrl;
			Task->AMRCtrl->Flag |= AMR_FLAG_PAUSE;
			if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_Idle)) Task->RequestCnt++;
			Task->RequestCnt++;
			Task->AckDataLen = 0;

			Task->PortBaudRate 	= RS485PortPara.BaudRate;
			Task->PortParity 	= RS485PortPara.Parity;
			Task->PortDataBit 	= RS485PortPara.DataBit;
			Task->PortStopBit 	= RS485PortPara.StopBit;
			
			Task->State = RT_AMR_WAIT_REQ_RESP;	
			//MC(0x99, (u32)Task->Addr, METER_ADDR_SIZE);
			MR((u32)Task->Addr, (u32)BroadCastAddr, METER_ADDR_SIZE);
			pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_REQ_TIME_OUT;			
		}
		else
		{
			Task->State = RT_AMR_DONE;
		}
		return NO_ERROR;
	}
	
	if(Task->SrcDataType == DATA_TYPE_FRAME)
	{
		Task->NeedAck = 0x00;
		while(Task->SrcDataPos < Task->SrcDataLen)
		{
			pQCSG_PnList = (QCSG_PnListStruct*)&Task->SrcData[Task->SrcDataPos];
			Task->CurPn = pQCSG_PnList->Pn;
			Task->CurPnDataLen = pQCSG_PnList->Len;
			Task->CurNW_DI = pQCSG_PnList->DI;
			Task->SrcDataPos += sizeof(QCSG_PnListStruct);
			PnNum = SwitchPnToNum(Task->CurPn);
			if((PnNum >= MAX_PN_NUM) || (NodeList[PnNum].Effective != 0x01))
			{
				Task->SrcDataPos += Task->CurPnDataLen;
				continue;
			}
			Task->DstPort = (u8)Get_UartPortByNodePortID(NodeList[PnNum].PortID);
			Addr = Get_AMR_TaskCtrlAddr(Task->DstPort);
			if(Addr == 0)
			{
				Task->SrcDataPos += Task->CurPnDataLen;
				continue;
			}
			AMRCtrl = (AMRCtrlStruct*)Addr;
			MR((u32)Task->Addr, (u32)NodeList[PnNum].Addr, METER_ADDR_SIZE);
			Task->Protocol 		= NodeList[PnNum].ProtocolType;
			Task->PMType 		= NodeList[PnNum].PointMeterType;
			Task->PortBaudRate 	= NodeList[PnNum].PortBaudRate;
			Task->PortParity 	= NodeList[PnNum].PortParity;
			Task->PortDataBit 	= NodeList[PnNum].PortDataBit;
			Task->PortStopBit 	= NodeList[PnNum].PortStopBit;
			Task->AMRCtrl = AMRCtrl;
			Task->AMRCtrl->Flag |= AMR_FLAG_PAUSE;
			if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_Idle)) Task->RequestCnt++;
			Task->RequestCnt++;
			Task->AckDataLen = 0;
			Task->State = RT_AMR_WAIT_REQ_RESP;
			pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_REQ_TIME_OUT;
			break;
		}
		if(Task->SrcDataPos >= Task->SrcDataLen)
		{
			Task->State = RT_AMR_DONE;
		}
		return NO_ERROR;
	}	

	if(Task->SrcDataType == DATA_TYPE_RELAY)
	{
		if(Task->SrcDataPos >= Task->SrcDataLen)
		{
			Task->State = RT_AMR_DONE;
			return ERROR_NO_AMR_TASK;
		}
		pQCSG_PnList = (QCSG_PnListStruct*)&Task->SrcData[Task->SrcDataPos];
		Task->CurPn = pQCSG_PnList->Pn;
		Task->CurPnDataLen = pQCSG_PnList->Len;
		Task->CurNW_DI = pQCSG_PnList->DI;
		Task->NeedAck = 0x01;
		Task->SrcDataPos        += sizeof(QCSG_PnListStruct);
		Task->RelayType 		= Task->SrcData[Task->SrcDataPos++];
		Task->RelayDstPort 		= Task->SrcData[Task->SrcDataPos++];
		Task->RelayBaudRate 	= Task->SrcData[Task->SrcDataPos++];
		Task->RelayParityMode 	= Task->SrcData[Task->SrcDataPos++];
		Task->RelayDataBits 	= Task->SrcData[Task->SrcDataPos++];
		Task->RelayStopBits 	= Task->SrcData[Task->SrcDataPos++];
		Task->RelayTimeOut		= Task->SrcData[Task->SrcDataPos++];
		Task->RelayFrameLen		= Task->SrcData[Task->SrcDataPos++];
		Task->CurPnDataLen 		-= 8;		
		Task->DstPort = (u8)Get_UartPortByNodePortID(Task->RelayDstPort);
		Addr = Get_AMR_TaskCtrlAddr(Task->DstPort);
		if(Addr == 0)
		{
			pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
			pQCSG_PnList->Pn = Task->CurPn;
			pQCSG_PnList->Len = 0;
			pQCSG_PnList->DI = Task->CurNW_DI;
			Task->AckDataLen = sizeof(QCSG_PnListStruct);

			Task->Deny_Frame =1;
			Task->State = RT_AMR_ACK_TO_SRC_PORT;
		}
		else
		{
			AMRCtrl = (AMRCtrlStruct*)Addr;
			Task->AMRCtrl = AMRCtrl;
			//PnNum = SwitchPnToNum(Task->CurPn);
			for(Index = 0; Index < Task->RelayFrameLen; Index++)
			{
				if(Task->SrcData[Task->SrcDataPos + Index] == DLT_645_HEAD) break;
			}
			if(Index >= Task->RelayFrameLen)
			{
				pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
				pQCSG_PnList->Pn = Task->CurPn;
				pQCSG_PnList->Len = 0;
				pQCSG_PnList->DI = Task->CurNW_DI;
				Task->AckDataLen = sizeof(QCSG_PnListStruct);

				Task->Deny_Frame =1;
				Task->State = RT_AMR_ACK_TO_SRC_PORT;
			}
			else
			{
				MR((u32)Task->Addr, (u32)&Task->SrcData[Task->SrcDataPos + Index + 1], METER_ADDR_SIZE);
				Task->AMRCtrl->Flag |= AMR_FLAG_PAUSE;
				if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_Idle)) Task->RequestCnt++;
				Task->RequestCnt++;
				Task->AckDataLen = 0;
				Task->PortBaudRate = Task->RelayBaudRate;
				Task->PortParity = Task->RelayParityMode;
				Task->PortDataBit = Task->RelayDataBits;
				Task->PortStopBit = Task->RelayStopBits;
				Task->State = RT_AMR_WAIT_REQ_RESP;
				pTerminalRam->RT_AMRTimeOut = Task->RelayTimeOut;
				//pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_REQ_TIME_OUT;
			}		
		}
		return NO_ERROR;
	}

	//if(Task->SrcDataType == DATA_TYPE_STRUCT)
	if(Task->PnIndex < Task->PnListNum)
	{
		pQCSG_PnList = (QCSG_PnListStruct*)&Task->SrcData[Task->PnIndex * sizeof(QCSG_PnListStruct)];
		Task->CurPn = pQCSG_PnList->Pn;
		Task->CurPnDataLen = 0;
		Task->CurNW_DI = pQCSG_PnList->DI;	
		Task->NeedAck = 0x01;
		Index = FindNwDIFrom_AFN0C_0D_Tbl(Task->CurNW_DI);
		PnNum = SwitchPnToNum(Task->CurPn);	
		if(PnNum < MAX_PN_NUM) 
		{
			Task->DstPort = (u8)Get_UartPortByNodePortID(NodeList[PnNum].PortID);
			Addr = Get_AMR_TaskCtrlAddr(Task->DstPort);
		}
		
#if ((Project/100)==9)//err20181019 配变 总表类型测量点实时数据采用集中器代码实现，其他功能由负控代码实现，由于未同步RAM档案，测量点参数从FLASH中获取。
		FlashLenAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
        FlashLen = sizeof(MeasurePointStruct) ;
        MR((u32)&MeasurePoint, FlashLenAddr, FlashLen);
#endif
		
		if((Addr != 0) && (Index < MAX_AFN0C_0D_IDENT_NUM) && (PnNum < MAX_PN_NUM) && (NodeList[PnNum].Effective == 0x01))
		{
			AMRCtrl = (AMRCtrlStruct*)Addr;
			Task->AMRCtrl = AMRCtrl;
			Task->AMRCtrl->Flag |= AMR_FLAG_PAUSE;
			if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_Idle)) Task->RequestCnt++;
			Task->RequestCnt++;
			MR((u32)Task->Addr, (u32)NodeList[PnNum].Addr, METER_ADDR_SIZE);
			Task->NW_DI_Num = 0x01;
			Task->Protocol = NodeList[PnNum].ProtocolType;
			Task->PMType = NodeList[PnNum].PointMeterType;
			Task->PortBaudRate = NodeList[PnNum].PortBaudRate;
			Task->PortParity = NodeList[PnNum].PortParity;
			Task->PortDataBit = NodeList[PnNum].PortDataBit;
			Task->PortStopBit = NodeList[PnNum].PortStopBit;	
			Task->CurNW_DI_Step = 0;
			Task->Cur645_DI_Step = 0;
			Task->Cur645_DI = 0;					
			Task->CurNW_DI_TblIndex = Index;				
			Task->AckDataLen = 0;	
			Task->State = RT_AMR_WAIT_REQ_RESP;
			pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_REQ_TIME_OUT;
		}
#if ((Project/100)==9)
		else if((Addr != 0) && (Index < MAX_AFN0C_0D_IDENT_NUM) && (PnNum < MAX_PN_NUM) && (MeasurePoint.State == 0x01) &&(MeasurePoint.TotalType == 0x01))
		{//err20181019 配变 总表类型测量点实时数据采用集中器代码实现，其他功能由负控代码实现，由于未同步RAM档案，测量点参数从FLASH中获取。
			AMRCtrl = (AMRCtrlStruct*)Addr;
			Task->AMRCtrl = AMRCtrl;
			Task->AMRCtrl->Flag |= AMR_FLAG_PAUSE;
			if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_Idle)) Task->RequestCnt++;
			Task->RequestCnt++;
			MR((u32)Task->Addr, (u32)MeasurePoint.Addr, METER_ADDR_SIZE);
			Task->NW_DI_Num = 0x01;
			Task->Protocol = MeasurePoint.ProtocolType;//NodeList[PnNum].ProtocolType;
			Task->PMType = MeasurePoint.PointMeterType;//NodeList[PnNum].PointMeterType;
			Task->PortBaudRate = MeasurePoint.PortBaudRate;//NodeList[PnNum].PortBaudRate;
			Task->PortParity = MeasurePoint.PortParity;//NodeList[PnNum].PortParity;
			Task->PortDataBit = MeasurePoint.PortDataBit;//NodeList[PnNum].PortDataBit;
			Task->PortStopBit = MeasurePoint.PortStopBit;//NodeList[PnNum].PortStopBit;	
			Task->CurNW_DI_Step = 0;
			Task->Cur645_DI_Step = 0;
			Task->Cur645_DI = 0;					
			Task->CurNW_DI_TblIndex = Index;				
			Task->AckDataLen = 0;	
			Task->State = RT_AMR_WAIT_REQ_RESP;
			pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_REQ_TIME_OUT;	
		}
#endif
		else 
		{
			pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
			pQCSG_PnList->Pn = Task->CurPn;
			pQCSG_PnList->Len = 0;
			pQCSG_PnList->DI = Task->CurNW_DI;
			Task->AckDataLen = sizeof(QCSG_PnListStruct);

			
			Task->Deny_Frame =1;
			Task->State = RT_AMR_ACK_TO_SRC_PORT;
		}
	}
	else
	{
		Task->State = RT_AMR_DONE;
		return ERROR_NO_AMR_TASK;
	}
	
	return NO_ERROR;
}


ResultEnum RT_AMR_TaskWaitReqResp(RT_AMRCtrlStruct* Task)
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	QCSG_PnListStruct* pQCSG_PnList;
	
	if(pTerminalRam->RT_AMRTimeOut == 0)
	{
		Task->RequestCnt = 0;
		if(Task->NeedAck != 0)
		{
			pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
			pQCSG_PnList->Pn = Task->CurPn;
			pQCSG_PnList->Len = 0;
			pQCSG_PnList->DI = Task->CurNW_DI;
			Task->AckDataLen = sizeof(QCSG_PnListStruct);

			Task->Deny_Frame =1;
			Task->State = RT_AMR_ACK_TO_SRC_PORT;
		}
		else
		{
			Task->PnIndex++;
			Task->SrcDataPos += Task->CurPnDataLen;
			Task->RetryCnt = 0;
			Task->RequestCnt = 0;
			Task->State = RT_AMR_SEND_REQ;
		}
		return ERROR_TIME_OUT;
	}
	
	if((Task->Flag & RT_AMR_CH_READY) == 0) return ERROR_TASK_BUSY;//等待抄表完成
	Task->Flag &= ~RT_AMR_CH_READY;
	Task->RetryCnt = 0;

	if(Task->NeedAck != 0)
	{
		pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
		pQCSG_PnList->Pn = Task->CurPn;
		pQCSG_PnList->Len = 0;
		pQCSG_PnList->DI = Task->CurNW_DI;
		Task->AckDataLen += sizeof(QCSG_PnListStruct);
	}

	pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_DATA_TIME_OUT;
	
	if(Task->SrcDataType == DATA_TYPE_RELAY)
	{//err20190409 中继转发设置的超时时间大于最小超时时间时，则按设置的时间计算超时。
		if(Task->RelayTimeOut > RS485_AMR_TIME_OUT)
			pTerminalRam->RT_AMRTimeOut = (Task->RelayTimeOut * RT_AMR_MAX_RETRY_CNT) + 2;//中继转发无回应将下发第二次 +2是为了防止比 WaitRTRX函数 先超时
	}
	
	if(Task->NeedAck == 0) 
	{
		pTerminalRam->RT_AMRTimeOut = RT_AMR_WAIT_REQ_TIME_OUT;
		//err20190312 广播校时下发间隔1s就可以了，但是PLC下发较慢，可能未完成添加任务srcdatapos就指向下一处，导致数据帧丢失 //RT_AMR_WAIT_REQ_TIME_OUT;
	}
	Task->AMRCtrl->Flag |= AMR_FLAG_RT_AMR_START;
	Task->State = RT_AMR_WAIT_DATA_RESP;
	Task->Flag = 0;
	return NO_ERROR;
}


ResultEnum RT_AMR_TaskWaitDataResp(RT_AMRCtrlStruct* Task)
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	QCSG_PnListStruct* pQCSG_PnList;
	if(pTerminalRam->RT_AMRTimeOut == 0)
	{
		Task->Flag |= RT_AMR_DATA_READY;
	}
	if(((Task->Flag & RT_AMR_DATA_REFUSE) != 0) || ((Task->Flag & RT_AMR_DATA_READY) != 0))
	{
		Task->Flag &= ~RT_AMR_DATA_REFUSE;
		Task->Flag &= ~RT_AMR_DATA_READY;
		pTerminalRam->RT_AMRTimeOut = 0;
		if(Task->NeedAck != 0)
		{
			pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
			pQCSG_PnList->Len = Task->AckDataLen - sizeof(QCSG_PnListStruct);

			if(pQCSG_PnList->Len==0) Task->Deny_Frame =1;
			Task->State = RT_AMR_ACK_TO_SRC_PORT;
		}
		else
		{
			Task->PnIndex++;
			Task->SrcDataPos += Task->CurPnDataLen;
			Task->RetryCnt = 0;
			Task->RequestCnt = 0;
			Task->State = RT_AMR_SEND_REQ;
		}
	}	
	return NO_ERROR;
}


ResultEnum RT_AMR_TaskAckToSrcPort(RT_AMRCtrlStruct* Task)
{
	MultiFrameSendReqStruct MultiFrameSendReq;
	QCSG_PnListStruct* pQCSG_PnList;
	u32 DI = 0;
	ResultEnum Result = NO_ERROR;
	QCSG_LCD_RAMDATA_S* pLcdRTAMR = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	// 返回的数据格式，根据 Task->SrcDataType 判断
	pQCSG_PnList = (QCSG_PnListStruct*)Task->AckData;
	
	if(Task->SrcPort == GUI_PORT)
	{
		if(pQCSG_PnList->Len != 0)
		{
			DI = pQCSG_PnList->DI;
			switch(DI)
			{
				case 0x0001FF00:
					if(pQCSG_PnList->Len <= 21)
					{
						MR((u32)pLcdRTAMR->u8Data0001FF00, (u32)&Task->AckData[sizeof(QCSG_PnListStruct) + 1], pQCSG_PnList->Len - 1);
					}
					break;
				case 0x0002FF00:
					if(pQCSG_PnList->Len <= 21)
					{
						MR((u32)pLcdRTAMR->u8Data0002FF00, (u32)&Task->AckData[sizeof(QCSG_PnListStruct) + 1], pQCSG_PnList->Len - 1);
					}
					break;
				case 0x0201FF00:
					MR((u32)pLcdRTAMR->u8Data0201FF00, (u32)&Task->AckData[sizeof(QCSG_PnListStruct)], 6);
					break;
				case 0x0202FF00:
					MR((u32)pLcdRTAMR->u8Data0202FF00, (u32)&Task->AckData[sizeof(QCSG_PnListStruct)], 9);
					break;
				case 0x0203FF00:
					if(pQCSG_PnList->Len <= 12)
					{
						MR((u32)pLcdRTAMR->u8Data0203FF00, (u32)&Task->AckData[sizeof(QCSG_PnListStruct)], pQCSG_PnList->Len);
					}
					break;
				case 0x0206FF00:
					if(pQCSG_PnList->Len <= 8)
					{
						MR((u32)pLcdRTAMR->u8Data0206FF00, (u32)&Task->AckData[sizeof(QCSG_PnListStruct)], pQCSG_PnList->Len);
					}
					break;
				default:
					break;	
			}
		}
		Task->PnIndex++;
		Task->SrcDataPos += Task->CurPnDataLen;
		Task->RetryCnt = 0;
		Task->RequestCnt = 0;
		Task->State = RT_AMR_SEND_REQ;
		return NO_ERROR;
	}

	MC(0, (u32)&MultiFrameSendReq, sizeof(MultiFrameSendReqStruct));
	MultiFrameSendReq.SrcTask = TASK_ID_RealTimeAMR;
	MultiFrameSendReq.RetryCnt = 0;
	MultiFrameSendReq.FrameType = Task->SrcFrameType;
	MultiFrameSendReq.DataType = Task->SrcDataType;
	MultiFrameSendReq.AckToSrcTask = 0;
	MultiFrameSendReq.NeedAck = 0;
	MultiFrameSendReq.QCSGFrameInfo.A1 = Task->QCSGFrameInfo.A1;
	MultiFrameSendReq.QCSGFrameInfo.A2 = Task->QCSGFrameInfo.A2;
	MultiFrameSendReq.QCSGFrameInfo.A3 = Task->QCSGFrameInfo.A3;
	MultiFrameSendReq.QCSGFrameInfo.FC = Task->QCSGFrameInfo.FC;
	//if(pQCSG_PnList->Len == 0)	//否认
	if(Task->Deny_Frame == 1)	//否认帧
	{
		MultiFrameSendReq.QCSGFrameInfo.AFN = 0x00;
		pQCSG_PnList->Len = 0x01;
		
		Task->AckData[8] = 0x01;
		Task->AckDataLen=9;
		//Task->AckData[Task->AckDataLen++] = 0x01;
	}
	else
	{
		MultiFrameSendReq.QCSGFrameInfo.AFN = Task->QCSGFrameInfo.AFN;
	}

	MultiFrameSendReq.QCSGFrameInfo.SEQ = Task->QCSGFrameInfo.SEQ++;
	MultiFrameSendReq.QCSGFrameInfo.Port = Task->QCSGFrameInfo.Port;
	MultiFrameSendReq.WaitAckTimeOut = 0;
	MultiFrameSendReq.PnListNum = 1;
	MultiFrameSendReq.DataLen = Task->AckDataLen;
	MultiFrameSendReq.DataBuf = Task->AckData;
	Result = QCSGSingleFrameSendReq(Task->SrcPort, &MultiFrameSendReq);

	if(Result != ERROR_TASK_BUSY)	//切下一个表
	{
		Task->PnIndex++;
		Task->SrcDataPos += Task->CurPnDataLen;
		Task->RetryCnt = 0;
		Task->RequestCnt = 0;

		Task->Deny_Frame =0;	//清否认帧标志
		Task->State = RT_AMR_SEND_REQ;
	}
	return Result;
}

ResultEnum RT_AMR_TaskDone(RT_AMRCtrlStruct* Task)
{
	QCSG_LCD_RAMDATA_S* pLcdRTAMR = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	if(Task->SrcPort == GUI_PORT)
	{
		pLcdRTAMR->u16Res1 |= LCD_RT_AMR_READY;
	}
	return NO_ERROR;
}


ResultEnum Convert645ToNWAMRData(DLT645FrameStruct* Frame, u32 NW_DI_TblIndex, u32 DI_Step, u8 Protocol, u8* OutData, u16* OutLen)
{
	RealTimeStruct RealTime;

	u8 DI_Size = 0, NW_DI_BlockLen = 0, DataMethod = 0, BlockLen = 0, UnitLen = 0;
	u32 DataLen = 0, i = 0, TempNum = 0, TempData = 0, DI_645 = 0;
	u16 OutDataLen = 0;
	u32 u32Value = 0;
	u32 SendIdent = 0;

	if(Protocol == DLT_645_PROTOCOL_97)
	{
		BlockLen = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64597Resolve[DI_Step].u8BlDataLen;
		UnitLen  = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64597Resolve[DI_Step].u8SiDataLen;
		DataMethod = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64597Resolve[DI_Step].u8TakeMethod;

		SendIdent = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64597Resolve[DI_Step].u32Ident;
	}
	else
	{
		BlockLen = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64507Resolve[DI_Step].u8BlDataLen;
		UnitLen  = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64507Resolve[DI_Step].u8SiDataLen;
		DataMethod = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].stDLT64507Resolve[DI_Step].u8TakeMethod;
	}
	NW_DI_BlockLen = gc_stQCSGAFN0C_0D_DLT645_Table[NW_DI_TblIndex].u16Dlen;
	OutDataLen = OutLen[0];	
	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)
	{	
		if(Protocol == DLT_645_PROTOCOL_97)
		{
			DI_Size = DLT_645_97_ID_SIZE;
		}
		else
		{
			DI_Size = DLT_645_07_ID_SIZE;
		}
		DI_645 = Get645FrameDataID(Frame, Protocol);
		DataLen = Frame->DataLen - DI_Size;
		if(DataLen == 0) return ERROR_WRONG_LENGTH;
		switch(DataMethod)
		{
			case COMMON_COV_METHOD:
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				OutLen[0] = OutDataLen;
				break;
			case PRICE_COV_METHOD:
				OutData[OutDataLen++] = (DataLen / UnitLen) - 1;
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				OutLen[0] = OutDataLen;
				break;
			case HBYTE_ADD_YEAR_COV_METHOD:
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				HAL_GetSystemTime(&RealTime);
				OutData[OutDataLen++] = RealTime.u8Year;
				OutLen[0] = OutDataLen;
				break;
			case HLBIT_ADD_ZERO_COV_METHOD:
				TempNum = DataLen >> 1;
				for(i = 0; i < TempNum; i++)
				{
					TempData = 0;
					TempData += Frame->Data[DI_Size + i*2 + 1];
					TempData <<= 8;
					TempData += Frame->Data[DI_Size + i*2];

					TempData <<= 4;
					TempData &= 0xFFFFFFF0;

					OutData[OutDataLen++] = TempData;
					OutData[OutDataLen++] = TempData >> 8;
					OutData[OutDataLen++] = TempData >> 16;
				}
				OutLen[0] = OutDataLen;
				break;
			case LBYTE_ADD_ZERO_COV_METHOD:
				OutData[OutDataLen++] = 0x00;
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				OutLen[0] = OutDataLen;
				break;
			case HBYTE_ADD_FF_COV_METHOD:
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				DI_Size = NW_DI_BlockLen - BlockLen;
				for(DataLen = 0; DataLen < DI_Size; DataLen++)
				{
					OutData[OutDataLen++] = 0xFF;
				}
				OutLen[0] = OutDataLen;
				break;
			case TAKE_PRICE_DATA_COV_METHOD:
				i = DI_645 & 0x000000FF;
				i *= UnitLen;
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size + i], UnitLen);
				OutDataLen += UnitLen;
				break;
			case LBYTE_ADD_PRICE_METHOD://20181120  集中器抄97表需量数据块用到
				OutData[OutDataLen++] = 0x04;
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				OutLen[0] = OutDataLen;		
				break;
			case DEMAND_PRICE_COV_METHOD://20181120  集中器抄07表需量数据块用到
		//数据头增加1个字节费率字段,对需量数据的处理
				OutData[OutDataLen++] = DataLen/8 - 1;
				MR((u32)&OutData[OutDataLen], (u32)&Frame->Data[DI_Size], DataLen);
				OutDataLen += DataLen;
				OutLen[0] = OutDataLen;	
				break;
			
			case LBIT_ADD_ZERO_COV_METHOD://20181120  集中器抄97表电压用到
				if((QCSG_BCDCheck(&Frame->Data[DI_Size],DataLen) != TRUE) || (DataLen > 4))//为什么是4，此处参考负控
				{//正确回复，但数据为FF
					MC(0xFF, (u32)&OutData[OutDataLen], DataLen);
					OutDataLen += DataLen;
					OutLen[0] = OutDataLen;	
				}		
				else{
					MR((u32)&u32Value, (u32)&Frame->Data[DI_Size], DataLen);
					u32Value = (u32Value << 4) & 0xFFFFFFF0;
					MR((u32)&OutData[OutDataLen], (u32)&u32Value, DataLen);
					OutDataLen += DataLen;
					OutLen[0] = OutDataLen;
				}
				break;
			case HBYTE_ADD_ZERO_COV_METHOD://20181121  97表运行状态字、编程次数用到
				if(QCSG_BCDCheck(&Frame->Data[DI_Size],DataLen) != TRUE)
				{//正确应答，但数据域无效
					DataLen++;
					MC(0xFF, (u32)&OutData[OutDataLen], DataLen);
					
					OutDataLen += DataLen;
					OutLen[0] = OutDataLen;	
				}
				else//正确应答，且数据域有效
				{
					MR((u32)&OutData[OutDataLen],(u32)&Frame->Data[DI_Size], DataLen);
					OutDataLen += DataLen;
					
					OutData[OutDataLen] = 0x00;
					OutDataLen++;
					
					OutLen[0] = OutDataLen;	
				}
				break;
			case LBYTE_ABANDON_HBYTE_ZERO://20190109   07表剩余金额用到
				OutData[OutDataLen++]=Frame->Data[DI_Size+1];
				OutData[OutDataLen++]=Frame->Data[DI_Size+2];
				OutData[OutDataLen++]=Frame->Data[DI_Size+3];
				OutData[OutDataLen++]=0;
				
				OutLen[0] = OutDataLen;	
				break;
			
			default:
				break;	
		}	
	}
	else
	{
		switch(DataMethod)
		{
			case COMMON_COV_METHOD:
				MC(0xFF, (u32)&OutData[OutDataLen], BlockLen);
				OutDataLen += BlockLen;
				OutLen[0] = OutDataLen;
				break;
			case PRICE_COV_METHOD:
				OutData[OutDataLen++] = (BlockLen / UnitLen) - 1;
				MC(0xFF, (u32)&OutData[OutDataLen], BlockLen);
				OutDataLen += BlockLen;
				OutLen[0] = OutDataLen;
				break;
			case HBYTE_ADD_YEAR_COV_METHOD:
				MC(0xFF, (u32)&OutData[OutDataLen], UnitLen);
				OutDataLen += UnitLen;
				OutData[OutDataLen++] = 0xFF;
				OutLen[0] = OutDataLen;
				break;
			case HLBIT_ADD_ZERO_COV_METHOD:
				TempNum = UnitLen >> 1;
				for(i = 0; i < TempNum; i++)
				{
					TempData = 0xFFFFFFFF;
					OutData[OutDataLen++] = TempData;
					OutData[OutDataLen++] = TempData >> 8;
					OutData[OutDataLen++] = TempData >> 16;
				}
				OutLen[0] = OutDataLen;
				break;
			case LBYTE_ADD_ZERO_COV_METHOD:
				OutData[OutDataLen++] = 0xFF;
				MC(0xFF, (u32)&OutData[OutDataLen], UnitLen);
				OutDataLen += UnitLen;
				OutLen[0] = OutDataLen;
				break;
			case HBYTE_ADD_FF_COV_METHOD:
				MC(0xFF, (u32)&OutData[OutDataLen], BlockLen);
				OutDataLen += BlockLen;
				DI_Size = NW_DI_BlockLen - BlockLen;
				for(DataLen = 0; DataLen < DI_Size; DataLen++)
				{
					OutData[OutDataLen++] = 0xFF;
				}
				OutLen[0] = OutDataLen;
			case TAKE_PRICE_DATA_COV_METHOD:
				MC(0xFF, (u32)&OutData[OutDataLen], UnitLen);
				OutDataLen += UnitLen;
				break;
			case LBYTE_ADD_PRICE_METHOD://20181120  集中器抄97表需量数据块用到
				OutData[OutDataLen++] = 0x04;
				MC(0xFF, (u32)&OutData[OutDataLen], UnitLen);
				OutDataLen += UnitLen;
				OutLen[0] = OutDataLen;		
				break;
			case DEMAND_PRICE_COV_METHOD://20181120  集中器抄07表需量数据块用到
		//数据头增加1个字节费率字段,对需量数据的处理
				OutData[OutDataLen++] = 0x04;
				MC(0xFF, (u32)&OutData[OutDataLen], BlockLen);
				OutDataLen += BlockLen;
				OutLen[0] = OutDataLen;		
				break;
			case LBIT_ADD_ZERO_COV_METHOD://20181120  集中器抄97表电压用到
				MC(0xFF, (u32)&OutData[OutDataLen], UnitLen);
				OutDataLen += UnitLen;
				OutLen[0] = OutDataLen;		
				break;

			case HBYTE_ADD_ZERO_COV_METHOD://20181121  97表运行状态字、编程次数用到
				if(SendIdent!= 0xc020){	//20181213 状态字不处理，直接回否认
					UnitLen++;
					MC(0xFF, (u32)&OutData[OutDataLen], UnitLen);
					OutDataLen += UnitLen;
					OutLen[0] = OutDataLen;		
				}
				break;
				
			case LBYTE_ABANDON_HBYTE_ZERO://20190109   07表剩余金额用到
				MC(0xFF, (u32)&OutData[OutDataLen], BlockLen);
				OutDataLen += BlockLen;
				OutLen[0] = OutDataLen;
				break;

			default:
				break;	
		}	
		
	}
	return NO_ERROR;
}



ResultEnum Get_NextAMR_ID(AMRCtrlStruct* AMRCtrl)
{
	if(AMRCtrl->AMR_ID == 0) return ERROR_InvalidData;

	while(AMRCtrl->AMR_ID_Step <= AMR_EVENT_TASK_ID)
	{	
		if((AMRCtrl->AMR_ID_Step & AMRCtrl->AMR_ID) != 0)
		{
			if(AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID)
			{
				MC(0, (u32)AMRCtrl->NormalTask, sizeof(NormalTaskCtrlStruct));
				while(Get_NextNormalTask(AMRCtrl) == NO_ERROR)
				{
					if(Get_NextNormalTaskPn(AMRCtrl) == NO_ERROR) break;	
					AMRCtrl->NormalTask->CurTaskID++;
				}
				if(AMRCtrl->NormalTask->CurTaskID < MAX_NORMAL_TASK_NUM) break;
			}
			else if(AMRCtrl->AMR_ID_Step == AMR_RELAY_TASK_ID)
			{
				MC(0, (u32)AMRCtrl->RelayTask, sizeof(RelayTaskCtrlStruct));
				break;
			}
			else
			{
				break;
			}
		}
		AMRCtrl->AMR_ID_Step <<= 1;
	}
	if(AMRCtrl->AMR_ID_Step > AMR_EVENT_TASK_ID) return ERROR_WRONG_ACCESS;

	AMRCtrl->MeterIndex = 0;
	AMRCtrl->CurNW_DI_Step = 0;
	AMRCtrl->Cur645_DI_Step = 0;
	AMRCtrl->AMRTotalDataLen = 0;
	AMRCtrl->Cur645DataLen = 0;
	AMRCtrl->TryCnt = 0;
	AMRCtrl->FailCnt = 0;
	AMRCtrl->FailMeters = 0;
	AMRCtrl->SuccessMeters = 0;

	return NO_ERROR;
}

ResultEnum Get_NextMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
			Result = Get_NextDayFreezMeter(PORTn, AMRCtrl);
			break;
		case AMR_DAY_FREEZ_PATCH_ID:
			Result = Get_NextDayFreezPatchMeter(PORTn, AMRCtrl);
			break;
		case AMR_HOUR_FREEZ_ID:
			Result = Get_NextHourFreezMeter(PORTn, AMRCtrl);
			break;
		case AMR_NORMAL_TASK_ID:
			Result = Get_NextNormalTaskMeter(PORTn, AMRCtrl);
			break;
		case AMR_RELAY_TASK_ID:
			Result = Get_NextRelayTaskMeter(PORTn, AMRCtrl);
			break;
		case AMR_ALARM_TASK_ID:
			Result = Get_NextAlarmTaskMeter(PORTn, AMRCtrl);
			break;
		case AMR_EVENT_TASK_ID:
			Result = ERROR_WRONG_ACCESS;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}

	return Result;
}

ResultEnum Get_NextDayFreezMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	//MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
		case RS485_2PORT:
		case RS485_3PORT:
			while(AMRCtrl->MeterIndex < MAX_PN_NUM)
			{
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && 
				   (NodeList[AMRCtrl->MeterIndex].PortID == ((PORTn == RS485_3PORT) ? PORT_ID_RS485_3 : (u8)(PORTn - 2))))//bug20180427-1 port_id转换
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;					
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				AMRCtrl->MeterIndex++;
			}
			if(AMRCtrl->MeterIndex >= MAX_PN_NUM) Result = ERROR_WRONG_ADDR;
			break;
		case PLC_PORT:
			while(AMRCtrl->MeterIndex < MAX_PN_NUM)
			{
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && 
				   (NodeList[AMRCtrl->MeterIndex].PortID == PORT_ID_PLC))
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				AMRCtrl->MeterIndex++;
			}
			if(AMRCtrl->MeterIndex >= MAX_PN_NUM) Result = ERROR_WRONG_ADDR;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}

	if(Result == NO_ERROR)
	{
		if(AMRCtrl->PMType == METER_TYPE_MULTI)
		{
			AMRCtrl->DayFreezNwDINum = METER_DAY_M_DI_NUM + METER_MONTH_M_ADD;
		}
		else// if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{		
			AMRCtrl->DayFreezNwDINum = METER_DAY_S_DI_NUM;
		}
	}
	
	return Result;
}

ResultEnum Get_NextDayFreezPatchMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	u16 Index = 0, PortIDEqual = 0;
	u8 TmpPatchMeter[LEN_ListForOneMeter];
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	u8* PatchMeter = (u8*)ADDR_MeterDayFreezFailList;

	while(AMRCtrl->MeterIndex < MeterFileInfo->AMRDayFreezFailMeters)
	{
		MR((u32)TmpPatchMeter, (u32)&PatchMeter[AMRCtrl->MeterIndex*LEN_ListForOneMeter], LEN_ListForOneMeter);
		Index = FindMeterInNodeList(TmpPatchMeter, NodeList, MAX_PN_NUM);
		if(Index >= MAX_PN_NUM)
		{
			AMRCtrl->MeterIndex++;
			continue;
		}
		PortIDEqual = 0;
		switch((u8)PORTn)
		{
			case RS485_1PORT:
			case RS485_2PORT:
			case RS485_3PORT:
				if(NodeList[Index].PortID == ((PORTn == RS485_3PORT) ? PORT_ID_RS485_3 : (u8)(PORTn - 2)))//bug20180427-1 port_id转换
				{
					PortIDEqual = 1;
				}
				break;
			case PLC_PORT:
				if(NodeList[Index].PortID == PORT_ID_PLC)
				{
					PortIDEqual = 1;
				}
				break;
			default:
				break;
		}
		if(PortIDEqual == 0)
		{
			AMRCtrl->MeterIndex++;
			continue;
		}
		
		AMRCtrl->Addr[0] = NodeList[Index].Addr[0];
		AMRCtrl->Addr[1] = NodeList[Index].Addr[1];
		AMRCtrl->Addr[2] = NodeList[Index].Addr[2];
		AMRCtrl->Addr[3] = NodeList[Index].Addr[3];
		AMRCtrl->Addr[4] = NodeList[Index].Addr[4];
		AMRCtrl->Addr[5] = NodeList[Index].Addr[5];
		AMRCtrl->PnNum   = NodeList[Index].PnNum;
		AMRCtrl->Protocol = NodeList[Index].ProtocolType;	
		AMRCtrl->PMType  = NodeList[Index].PointMeterType;
		AMRCtrl->UerType = NodeList[Index].UerType;
		AMRCtrl->PortBaudRate = NodeList[Index].PortBaudRate;
		AMRCtrl->PortDataBit = NodeList[Index].PortDataBit;
		AMRCtrl->PortParity = NodeList[Index].PortParity;
		AMRCtrl->PortStopBit = NodeList[Index].PortStopBit;
		AMRCtrl->CurNW_DI_Step = 0;
		AMRCtrl->AMRTotalDataLen = 0;
		break;
	}

	if(AMRCtrl->MeterIndex >= MeterFileInfo->AMRDayFreezFailMeters) Result = ERROR_WRONG_ADDR;
	
	if(Result == NO_ERROR)
	{
		if(AMRCtrl->PMType == METER_TYPE_MULTI)
		{
			AMRCtrl->DayFreezNwDINum = METER_DAY_M_DI_NUM + METER_MONTH_M_ADD;
		}
		else// if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{		
			AMRCtrl->DayFreezNwDINum = METER_DAY_S_DI_NUM;
		}
	}
	
	return Result;
}

ResultEnum Get_NextHourFreezMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
		case RS485_2PORT:
		case RS485_3PORT:
			while(AMRCtrl->MeterIndex < MAX_PN_NUM)
			{
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && 
				   (NodeList[AMRCtrl->MeterIndex].PortID == ((PORTn == RS485_3PORT) ? PORT_ID_RS485_3 : (u8)(PORTn - 2))) && //bug20180427-1 port_id转换
				   (NodeList[AMRCtrl->MeterIndex].UerType == USER_TYPE_IMPORTANT))
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				AMRCtrl->MeterIndex++;
			}
			if(AMRCtrl->MeterIndex >= MAX_PN_NUM) Result = ERROR_WRONG_ADDR;
			break;
		case PLC_PORT:
			while(AMRCtrl->MeterIndex < MAX_PN_NUM)
			{
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && 
				   (NodeList[AMRCtrl->MeterIndex].PortID == PORT_ID_PLC) && 
				   (NodeList[AMRCtrl->MeterIndex].UerType == USER_TYPE_IMPORTANT ))
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				AMRCtrl->MeterIndex++;
			}
			if(AMRCtrl->MeterIndex >= MAX_PN_NUM) Result = ERROR_WRONG_ADDR;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}

	if(Result == NO_ERROR)
	{
		if(AMRCtrl->PMType == METER_TYPE_MULTI)
		{
			AMRCtrl->HourFreezNwDINum = METER_HOUR_M_DI_NUM;
		}
		else //if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{		
			AMRCtrl->HourFreezNwDINum = METER_HOUR_S_DI_NUM;
		}
	}
	
	return Result;
}

ResultEnum Get_NextNormalTaskMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	while(Get_NextNormalTaskPnMeter(PORTn, AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->NormalTask->PnIndex++;
		while(Get_NextNormalTaskPn(AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->NormalTask->CurTaskID++;
			if(Get_NextNormalTask(AMRCtrl) != NO_ERROR)
			{
				return ERROR_WRONG_ACCESS;
			}
		}
	}

	return NO_ERROR;
}
ResultEnum Get_NextNormalTask(AMRCtrlStruct* AMRCtrl)
{
	NormalTaskParaStruct NormalTaskPara;
	ResultEnum Result = NO_ERROR;
	u32 Addr = 0, CurTaskID = 0, i = 0, NW_DI = 0, Index = 0;
	
	CurTaskID = AMRCtrl->NormalTask->CurTaskID;
	while(CurTaskID < MAX_NORMAL_TASK_NUM)
	{
		if(AMRCtrl->NormalTaskExe[CurTaskID] != 0)
		{
			Addr = ADDR_NormalTaskPara + CurTaskID*sizeof(NormalTaskParaStruct);
			MR((u32)&NormalTaskPara, Addr, sizeof(NormalTaskParaStruct));
			if(NormalTaskPara.Flag != 0x01) continue;
			if(NormalTaskPara.DANum == 0) continue;
			if(NormalTaskPara.DataIDNum == 0) continue;
			Index = 0;
			if(NormalTaskPara.DataIDNum > AMR_MAX_DATA_ID)
			{
				NormalTaskPara.DataIDNum = AMR_MAX_DATA_ID;
			}
			for(i = 0; i < NormalTaskPara.DataIDNum; i++)
			{
				NW_DI = NormalTaskPara.DataID[i];
				Addr = 0;
				Addr = FindNwDIFrom_AFN0C_0D_Tbl(NW_DI);
				if(Addr < MAX_AFN0C_0D_IDENT_NUM)
				{
					AMRCtrl->NormalTask->DataList[Index].NW_DI = NW_DI;
					AMRCtrl->NormalTask->DataList[Index].TblIndex = Addr;
					AMRCtrl->NormalTask->DataList[Index].DataLen = 0;
					AMRCtrl->NormalTask->DataList[Index].DI645Num = 0;
					AMRCtrl->NormalTask->DataList[Index].TaskIDNum = 0;
					//MC(0, AMRCtrl->NormalTask->DataList[Index].Data, AMR_MAX_645_BUF_LEN);
					Index++;
				}
			}
			if(Index != 0) break;
		}
		CurTaskID++;
	}
	AMRCtrl->NormalTask->CurTaskID = CurTaskID;
	if(CurTaskID >= MAX_NORMAL_TASK_NUM) return ERROR_WRONG_ACCESS;
	
	AMRCtrl->NormalTask->PnNum = NormalTaskPara.DANum;
	AMRCtrl->NormalTask->DINum = Index;
	AMRCtrl->NormalTask->PnIndex = 0;
	AMRCtrl->NormalTask->PnListIndex = 0;
	AMRCtrl->NormalTask->PnListNum = 0;
	for(i = 0; i < NormalTaskPara.DANum; i++)
	{
		AMRCtrl->NormalTask->DA_Pn[i] = NormalTaskPara.DA[i];
	}	
	return Result;
}

ResultEnum Get_NextNormalTaskPn(AMRCtrlStruct* AMRCtrl)
{
	u32 Index = 0;
	
	Index = AMRCtrl->NormalTask->PnIndex;
	while(Index < AMRCtrl->NormalTask->PnNum)
	{
		AMRCtrl->NormalTask->PnListNum = SplitPnToList(AMRCtrl->NormalTask->DA_Pn[Index], AMRCtrl->NormalTask->PnList, PN_CHECK_ENABLE);
		if(AMRCtrl->NormalTask->PnListNum != 0) break;
		Index++;
	}
	AMRCtrl->NormalTask->PnIndex = Index;
	AMRCtrl->NormalTask->PnListIndex = 0;
	if(Index >= AMRCtrl->NormalTask->PnNum) return ERROR_WRONG_ADDR;
	return NO_ERROR;
}

ResultEnum Get_NextNormalTaskPnMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	u32 PnListIndex = 0, Pn = 0;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	
	switch((u8)PORTn)
	{
		case RS485_1PORT:
		case RS485_2PORT:
		case RS485_3PORT:
			PnListIndex = AMRCtrl->NormalTask->PnListIndex;
			while(PnListIndex < AMRCtrl->NormalTask->PnListNum)
			{
				Pn = AMRCtrl->NormalTask->PnList[PnListIndex];
				AMRCtrl->MeterIndex = SwitchPnToNum(Pn);
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && (NodeList[AMRCtrl->MeterIndex].PortID == ((PORTn == RS485_3PORT) ? PORT_ID_RS485_3 : (u8)(PORTn - 2))))//bug20180427-1 port_id转换
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				PnListIndex++;
			}
			AMRCtrl->NormalTask->PnListIndex = PnListIndex;
			if(PnListIndex >= AMRCtrl->NormalTask->PnListNum) Result = ERROR_WRONG_ADDR;
			break;
		case PLC_PORT:
			PnListIndex = AMRCtrl->NormalTask->PnListIndex;
			while(PnListIndex < AMRCtrl->NormalTask->PnListNum)
			{
				Pn = AMRCtrl->NormalTask->PnList[PnListIndex];
				AMRCtrl->MeterIndex = SwitchPnToNum(Pn);
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && (NodeList[AMRCtrl->MeterIndex].PortID == PORT_ID_PLC))
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				PnListIndex++;
			}
			AMRCtrl->NormalTask->PnListIndex = PnListIndex;
			if(PnListIndex >= AMRCtrl->NormalTask->PnListNum) Result = ERROR_WRONG_ADDR;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}	

	if(Result == NO_ERROR)
	{
		if(AMRCtrl->PMType == METER_TYPE_OTHER)
		{
			Result = ERROR_WRONG_ACCESS;
		}		
	}
	return Result;
}



ResultEnum Get_NextRelayTaskMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	RelayTaskParaStruct RelayTaskPara;
	ResultEnum Result = NO_ERROR;
	u32 Addr = 0, CurTaskID = 0;
	
	CurTaskID = AMRCtrl->RelayTask->CurTaskID;
	while(CurTaskID < MAX_RELAY_TASK_NUM)
	{
		if(AMRCtrl->RelayTaskExe[CurTaskID] != 0)
		{
			Addr = ADDR_RelayTaskPara + CurTaskID*sizeof(RelayTaskParaStruct);
			MR((u32)&RelayTaskPara, Addr, sizeof(RelayTaskParaStruct));
			if(RelayTaskPara.Flag != 0x01) continue;
			if(RelayTaskPara.ComPortId == ((PORTn == RS485_3PORT) ? PORT_ID_RS485_3 : (u8)(PORTn - 2))) break; //(u8)(PORTn - 2)) break;//bug20180427-1 port_id转换
			if((PLC_PORT == (u8)PORTn) && (RelayTaskPara.ComPortId == PORT_ID_PLC))
			{
				break;
			}
			if((PLC_PORT == (u8)PORTn) && (RelayTaskPara.ComPortId == PORT_ID_WIRELESS))
			{
				break;
			}
		}
		CurTaskID++;
	}
	AMRCtrl->RelayTask->CurTaskID = CurTaskID;
	if(CurTaskID >= MAX_RELAY_TASK_NUM) return ERROR_WRONG_ACCESS;
	
	AMRCtrl->RelayTask->RelayType = RelayTaskPara.RelayType;
	AMRCtrl->PortBaudRate = RelayTaskPara.Baud;
	AMRCtrl->PortDataBit = RelayTaskPara.DataBit;
	AMRCtrl->PortParity = RelayTaskPara.Parity;
	AMRCtrl->PortStopBit = RelayTaskPara.StopBit;
	AMRCtrl->CurNW_DI_Step = 0;
	AMRCtrl->AMRTotalDataLen = 0;
	return Result;
}

ResultEnum Get_NextAlarmTaskMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
		case RS485_2PORT:
		case RS485_3PORT:
			while(AMRCtrl->MeterIndex < MAX_PN_NUM)
			{
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && 
				   (NodeList[AMRCtrl->MeterIndex].PortID == ((PORTn == RS485_3PORT) ? PORT_ID_RS485_3 : (u8)(PORTn - 2))))//bug20180427-1 port_id转换
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				AMRCtrl->MeterIndex++;
			}
			if(AMRCtrl->MeterIndex >= MAX_PN_NUM) Result = ERROR_WRONG_ADDR;
			break;
		case PLC_PORT:
			while(AMRCtrl->MeterIndex < MAX_PN_NUM)
			{
				if((NodeList[AMRCtrl->MeterIndex].Effective == 0x01) && 
				   (NodeList[AMRCtrl->MeterIndex].PortID == PORT_ID_PLC))
				{
					AMRCtrl->Addr[0] = NodeList[AMRCtrl->MeterIndex].Addr[0];
					AMRCtrl->Addr[1] = NodeList[AMRCtrl->MeterIndex].Addr[1];
					AMRCtrl->Addr[2] = NodeList[AMRCtrl->MeterIndex].Addr[2];
					AMRCtrl->Addr[3] = NodeList[AMRCtrl->MeterIndex].Addr[3];
					AMRCtrl->Addr[4] = NodeList[AMRCtrl->MeterIndex].Addr[4];
					AMRCtrl->Addr[5] = NodeList[AMRCtrl->MeterIndex].Addr[5];
					AMRCtrl->PnNum   = NodeList[AMRCtrl->MeterIndex].PnNum;
					AMRCtrl->Protocol = NodeList[AMRCtrl->MeterIndex].ProtocolType;	
					AMRCtrl->PMType  = NodeList[AMRCtrl->MeterIndex].PointMeterType;
					AMRCtrl->UerType = NodeList[AMRCtrl->MeterIndex].UerType;
					AMRCtrl->PortBaudRate = NodeList[AMRCtrl->MeterIndex].PortBaudRate;
					AMRCtrl->PortDataBit = NodeList[AMRCtrl->MeterIndex].PortDataBit;
					AMRCtrl->PortParity = NodeList[AMRCtrl->MeterIndex].PortParity;
					AMRCtrl->PortStopBit = NodeList[AMRCtrl->MeterIndex].PortStopBit;
					AMRCtrl->CurNW_DI_Step = 0;
					AMRCtrl->AMRTotalDataLen = 0;
					break;
				}
				AMRCtrl->MeterIndex++;
			}
			if(AMRCtrl->MeterIndex >= MAX_PN_NUM) Result = ERROR_WRONG_ADDR;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}

	if(Result == NO_ERROR)
	{
		// 此处将抄 南网告警数据标识的数量设置为1，是因为；不为每个南网数据标识分别抄表，而是将所有南网数据标识
		// 对应的 645 标识集合在一起，统一抄表，可以极大减少抄表次数(因为不同告警南网标识可能抄同样的645数据标识)
		if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{
			AMRCtrl->AlarmNwDINum = 0x01;
		}
		else if(AMRCtrl->PMType == METER_TYPE_MULTI)
		{
			AMRCtrl->AlarmNwDINum = 0x01;
		}
		else
		{
			AMRCtrl->AlarmNwDINum = 0;
			Result = ERROR_WRONG_ACCESS;
		}
	}
	
	return Result;
}

ResultEnum Get_NextAMR_NW_DI(AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
		case AMR_DAY_FREEZ_PATCH_ID:
			while(AMRCtrl->CurNW_DI_Step < AMRCtrl->DayFreezNwDINum)
			{
				AMRCtrl->CurNW_DI = DayFreez_SE_DI[AMRCtrl->CurNW_DI_Step];
				AMRCtrl->CurNW_DI_TblIndex = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->CurNW_DI);
				if(AMRCtrl->CurNW_DI_TblIndex < MAX_AFN0C_0D_IDENT_NUM) break;
				AMRCtrl->CurNW_DI_Step++;
			}
			if(AMRCtrl->CurNW_DI_Step >= AMRCtrl->DayFreezNwDINum)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->Cur645_DI_Step = 0;
				AMRCtrl->Cur645DataLen = 0;
			}
			break;
		case AMR_HOUR_FREEZ_ID:
			while(AMRCtrl->CurNW_DI_Step < AMRCtrl->HourFreezNwDINum)
			{
				AMRCtrl->CurNW_DI = HourFreez_SE_DI[AMRCtrl->CurNW_DI_Step];
				AMRCtrl->CurNW_DI_TblIndex = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->CurNW_DI);
				if(AMRCtrl->CurNW_DI_TblIndex < MAX_AFN0C_0D_IDENT_NUM) break;
				AMRCtrl->CurNW_DI_Step++;
			}
			if(AMRCtrl->CurNW_DI_Step >= AMRCtrl->HourFreezNwDINum)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->Cur645_DI_Step = 0;
				AMRCtrl->Cur645DataLen = 0;
			}
			break;
		case AMR_NORMAL_TASK_ID:
			while(AMRCtrl->CurNW_DI_Step < AMRCtrl->NormalTask->DINum)
			{
				AMRCtrl->CurNW_DI = AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].NW_DI;
				//AMRCtrl->CurNW_DI_TblIndex = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->CurNW_DI);
				AMRCtrl->CurNW_DI_TblIndex = AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].TblIndex;
				if(AMRCtrl->CurNW_DI_TblIndex < MAX_AFN0C_0D_IDENT_NUM) break;
				AMRCtrl->CurNW_DI_Step++;
			}
			if(AMRCtrl->CurNW_DI_Step >= AMRCtrl->NormalTask->DINum)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->Cur645_DI_Step = 0;
				AMRCtrl->Cur645DataLen = 0;
				AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].DataLen = 0;
				AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].DI645Num = 0;
				AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].TaskIDNum = 0;
				MC(0, (u32)AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].TaskIDList, 2*SIZE_12);
			}
			break;
		case AMR_RELAY_TASK_ID:
			if(AMRCtrl->CurNW_DI_Step >= 1)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->Cur645_DI_Step = 0;
				AMRCtrl->Cur645DataLen = 0;
			}
			break;
		case AMR_ALARM_TASK_ID:
			if(AMRCtrl->CurNW_DI_Step >= 1)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->Cur645_DI_Step = 0;
				AMRCtrl->Cur645DataLen = 0;
			}
			break;
		case AMR_EVENT_TASK_ID:
			Result = ERROR_WRONG_ACCESS;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}
	return Result;
}

ResultEnum Get_NextAMR_645_DI(AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
		case AMR_DAY_FREEZ_PATCH_ID:
		case AMR_HOUR_FREEZ_ID:
		case AMR_NORMAL_TASK_ID:
			if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
			{
				AMRCtrl->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].stDLT64597Resolve[AMRCtrl->Cur645_DI_Step].u32Ident;
			}
			else
			{
				AMRCtrl->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].stDLT64507Resolve[AMRCtrl->Cur645_DI_Step].u32Ident;
			}
			if(AMRCtrl->Cur645_DI == 0xFFFFFFFF)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->TryCnt = 0;
			}
			break;	
		case AMR_RELAY_TASK_ID:
			if(AMRCtrl->Cur645_DI_Step >= 1)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				AMRCtrl->TryCnt = 0;
			}
			break;
		case AMR_ALARM_TASK_ID:
			if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
			{				
				while(AMRCtrl->Cur645_DI_Step < AMRCtrl->AlarmTask->DI_645_97_Num)
				{
					if(AMRCtrl->PMType == METER_TYPE_SINGLE)
					{
						if((AMRCtrl->AlarmTask->DI_645_97_Flag[AMRCtrl->Cur645_DI_Step] & DI_645_FLAG_SINGLE_METER) != 0)
						{
							AMRCtrl->Cur645_DI = AMRCtrl->AlarmTask->DI_645_97[AMRCtrl->Cur645_DI_Step];
							break;
						}
					}

					if(AMRCtrl->PMType == METER_TYPE_MULTI)
					{
						if((AMRCtrl->AlarmTask->DI_645_97_Flag[AMRCtrl->Cur645_DI_Step] & DI_645_FLAG_MULTI_METER) != 0)
						{
							AMRCtrl->Cur645_DI = AMRCtrl->AlarmTask->DI_645_97[AMRCtrl->Cur645_DI_Step];
							break;
						}
					}
					AMRCtrl->Cur645_DI_Step++;
				}
				if(AMRCtrl->Cur645_DI_Step >= AMRCtrl->AlarmTask->DI_645_97_Num) Result = ERROR_WRONG_ACCESS;
			}
			else
			{
				while(AMRCtrl->Cur645_DI_Step < AMRCtrl->AlarmTask->DI_645_07_Num)
				{
					if(AMRCtrl->PMType == METER_TYPE_SINGLE)
					{
						if((AMRCtrl->AlarmTask->DI_645_07_Flag[AMRCtrl->Cur645_DI_Step] & DI_645_FLAG_SINGLE_METER) != 0)
						{
							AMRCtrl->Cur645_DI = AMRCtrl->AlarmTask->DI_645_07[AMRCtrl->Cur645_DI_Step];
							break;
						}
					}

					if(AMRCtrl->PMType == METER_TYPE_MULTI)
					{
						if((AMRCtrl->AlarmTask->DI_645_07_Flag[AMRCtrl->Cur645_DI_Step] & DI_645_FLAG_MULTI_METER) != 0)
						{
							AMRCtrl->Cur645_DI = AMRCtrl->AlarmTask->DI_645_07[AMRCtrl->Cur645_DI_Step];
							break;
						}
					}
					AMRCtrl->Cur645_DI_Step++;
				}
				if(AMRCtrl->Cur645_DI_Step >= AMRCtrl->AlarmTask->DI_645_07_Num) Result = ERROR_WRONG_ACCESS;
			}
			if(Result == NO_ERROR)
			{
				AMRCtrl->TryCnt = 0;
			}
			break;
		case AMR_EVENT_TASK_ID:
			Result = ERROR_WRONG_ACCESS;
			break;
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}
	return Result;
}


ResultEnum AMR_SwitchToNextMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	AMRCtrl->MeterIndex++;
	if(AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID)
	{
		AMRCtrl->NormalTask->PnListIndex++;
	}
	else if(AMRCtrl->AMR_ID_Step == AMR_RELAY_TASK_ID)
	{
		AMRCtrl->RelayTask->CurTaskID++;
	}
	else 
	{

	}
	while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->AMR_ID_Step <<= 1;
		if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
		{
			return ERROR_NO_AMR_TASK;
		}
	}
	
	while(Get_NextAMR_NW_DI(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->MeterIndex++;
		while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->AMR_ID_Step <<= 1;
			if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
			{
				return ERROR_NO_AMR_TASK;
			}
		}
	}

	while(Get_NextAMR_645_DI(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->CurNW_DI_Step++;
		while(Get_NextAMR_NW_DI(AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->MeterIndex++;
			while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
			{
				AMRCtrl->AMR_ID_Step <<= 1;
				if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
				{
					return ERROR_NO_AMR_TASK;
				}
			}
		}
	}

	return NO_ERROR;
}

ResultEnum StoreNormalTaskAMRData(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	u32 NormalTaskID = 0, DataLenPos = 0;
	u8* AMRDataBuf = NULL;
	u16 Index = 0, NW_DI_Num = 0, DataLen = 0;
	
	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	AMRCtrl->AMRTotalDataLen = 0;
	if(AMRDataBuf != NULL)
	{	
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->NormalTask->DINum; NW_DI_Num++)
		{
			if(AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen == 0) continue;
			//Index = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->NormalTask->DataList[NW_DI_Num].NW_DI);
			Index = AMRCtrl->NormalTask->DataList[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->NormalTask->DataList[NW_DI_Num].NW_DI;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->NormalTask->DataList[NW_DI_Num].NW_DI >> 8;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->NormalTask->DataList[NW_DI_Num].NW_DI >> 16;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->NormalTask->DataList[NW_DI_Num].NW_DI >> 24;
			
			DataLenPos = AMRCtrl->AMRTotalDataLen;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
			
			if((AMRCtrl->Protocol == DLT_645_PROTOCOL_97) && (gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen == 41) && 
			   (AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen < gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen))
			{
				DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen/gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[0].u8SiDataLen;
				DataLen -= 1;
				AMRCtrl->Cur645Data[0] = DataLen;
				DataLen = AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen;
				MR((u32)&AMRCtrl->Cur645Data[1], (u32)AMRCtrl->NormalTask->DataList[NW_DI_Num].Data, DataLen);
				MR((u32)AMRCtrl->NormalTask->DataList[NW_DI_Num].Data, (u32)AMRCtrl->Cur645Data, DataLen+1);
				AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen = DataLen + 1;
			}

			DataLen = AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen;
			MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)AMRCtrl->NormalTask->DataList[NW_DI_Num].Data, DataLen);
			AMRCtrl->AMRTotalDataLen += DataLen;
			if(gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen > DataLen)
			{
				DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen - DataLen;
				MC(0xFF, (u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], DataLen);
				AMRCtrl->AMRTotalDataLen += DataLen;
				DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen;
			}
			AMRDataBuf[DataLenPos]     = DataLen;
			AMRDataBuf[DataLenPos + 1] = DataLen >> 8;
		}
	}

	if((AMRCtrl->AMRTotalDataLen != 0) && (AMRDataBuf != NULL))
	{
		HAL_GetSystemTime(&RealTime);
		RealTimeToYYTime(&RealTime, &YYTime);
		YYTime.Ack = 0;
		YYTime.Report = 0;
		NormalTaskID = 0xE0000301 + AMRCtrl->NormalTask->CurTaskID;
		NAND_WriteNormalTask(AMRCtrl->Addr, AMRCtrl->PMType, NormalTaskID, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
	}
	return NO_ERROR;
}

ResultEnum StoreAlarmTaskAMRData_07(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	ResultEnum Result = NO_ERROR;
	u32 Cur645_DI = 0, pData = 0, DataLen = 0;
	AlarmAMRDataStruct* AlarmAMRData = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	
	DataLen = Frame->DataLen - DLT_645_07_ID_SIZE;
	if(DataLen == 0) return ERROR_WRONG_LENGTH;

	AlarmAMRData += AMRCtrl->MeterIndex;
	Cur645_DI = Get645FrameDataID(Frame, AMRCtrl->Protocol);
	pData = (u32)&Frame->Data[DLT_645_07_ID_SIZE];
	
	switch(Cur645_DI)
	{
		case 0x04000503:
			MR((u32)AlarmAMRData->DI_04000503, pData, DataLen);
			break;
		//case 0x04000504:
		//	MR((u32)AlarmAMRData->DI_04000504, pData, DataLen);
		//	break;
		case 0x040005FF:
			MR((u32)AlarmAMRData->DI_040005FF, pData, DataLen);
			break;
		case 0x04000101:
			MR((u32)AlarmAMRData->DI_04000101, pData, DataLen);
			break;
		case 0x04000102:
			MR((u32)AlarmAMRData->DI_04000102, pData, DataLen);
			break;
		case 0x03300500:
			MR((u32)AlarmAMRData->DI_03300500, pData, DataLen);
			break;
		case 0x00900200:
			MR((u32)AlarmAMRData->DI_00900200, pData, DataLen);
			break;
		case 0x00010000:
			MR((u32)AlarmAMRData->DI_00010000, pData, DataLen);
			break;
		case 0x00020000:
			MR((u32)AlarmAMRData->DI_00020000, pData, DataLen);
			break;
		case 0x00030000:
			MR((u32)AlarmAMRData->DI_00030000, pData, DataLen);
			break;
		case 0x00040000:
			MR((u32)AlarmAMRData->DI_00040000, pData, DataLen);
			break;
		case 0x0201FF00:
			MR((u32)AlarmAMRData->DI_0201FF00, pData, DataLen);
			break;
		case 0x0202FF00:
			MR((u32)AlarmAMRData->DI_0202FF00, pData, DataLen);
			break;
		case 0x0203FF00:
			MR((u32)AlarmAMRData->DI_0203FF00, pData, DataLen);
			break;
		case 0x0204FF00:
			MR((u32)AlarmAMRData->DI_0204FF00, pData, DataLen);
			break;
		case 0x0206FF00:
			MR((u32)AlarmAMRData->DI_0206FF00, pData, DataLen);
			break;
		case 0x03300000://编程总次数
			MR((u32)AlarmAMRData->DI_03300000, pData, DataLen);
			break;
		case 0x10010001:
			MR((u32)AlarmAMRData->DI_10010001, pData, DataLen);
			break;
		case 0x10010002:
			MR((u32)AlarmAMRData->DI_10010002, pData, DataLen);
			break;
		case 0x10020001:
			MR((u32)AlarmAMRData->DI_10020001, pData, DataLen);
			break;
		case 0x10020002:
			MR((u32)AlarmAMRData->DI_10020002, pData, DataLen);
			break;
		case 0x10030001:
			MR((u32)AlarmAMRData->DI_10030001, pData, DataLen);
			break;
		case 0x10030002:
			MR((u32)AlarmAMRData->DI_10030002, pData, DataLen);
			break;
		case 0x18010001:
			MR((u32)AlarmAMRData->DI_18010001, pData, DataLen);
			break;
		case 0x18010002:
			MR((u32)AlarmAMRData->DI_18010002, pData, DataLen);
			break;
		case 0x18020001:
			MR((u32)AlarmAMRData->DI_18020001, pData, DataLen);
			break;
		case 0x18020002:
			MR((u32)AlarmAMRData->DI_18020002, pData, DataLen);
			break;
		case 0x18030001:
			MR((u32)AlarmAMRData->DI_18030001, pData, DataLen);
			break;
		case 0x18030002:
			MR((u32)AlarmAMRData->DI_18030002, pData, DataLen);
			break;
		case 0x1B010001:
			MR((u32)AlarmAMRData->DI_1B010001, pData, DataLen);
			break;
		case 0x1B010002:
			MR((u32)AlarmAMRData->DI_1B010002, pData, DataLen);
			break;
		case 0x1B020001:
			MR((u32)AlarmAMRData->DI_1B020001, pData, DataLen);
			break;
		case 0x1B020002:
			MR((u32)AlarmAMRData->DI_1B020002, pData, DataLen);
			break;
		case 0x1B030001:
			MR((u32)AlarmAMRData->DI_1B030001, pData, DataLen);
			break;
		case 0x1B030002:
			MR((u32)AlarmAMRData->DI_1B030002, pData, DataLen);
			break;
		default:
			break;
		
	}
	return Result;
}

ResultEnum StoreAlarmTaskAMRData_97(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	return NO_ERROR;
}

void MeterAlarmRecordGenerate(void)
{	
	ThresholdParaStruct Threshold;
	AMRTaskManageStruct* AMRTask = (AMRTaskManageStruct*)ADDR_AMRTaskManageCtrl;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	u32 Index = 0;
	Index = AMRTask->AlarmGenerateIndex;
	if(Index >= MAX_PN_NUM) 
	{
		Index = 0;
		AMRTask->AlarmGenerateIndex = Index;
	}
	else
	{
		AMRTask->AlarmGenerateIndex = Index + 1;
	}

	
#if (TEST_ON == 0)
// 上一次数据无效，本次抄到了的情况，即第一次抄到
	if((AlarmAMRDataOld[Index].State[0] == 0) && (AlarmAMRDataNew[Index].State[1] == 0x01))
	{
		MR((u32)&AlarmAMRDataOld[Index], (u32)&AlarmAMRDataNew[Index], sizeof(AlarmAMRDataStruct));
		AlarmAMRDataOld[Index].State[0] = 0x01;
		AlarmAMRDataNew[Index].State[1] = 0x00;
		return;
	}
	if(AlarmAMRDataNew[Index].State[1] == 0x00) return;
	if(AlarmAMRDataOld[Index].State[0] == 0x00) return;
	
	MR((u32)&Threshold, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));
	
	//err20181218 增加B C相失压失流潮流反向
	GenerateAVoltageLossAlarm_E2000016(Index);//E2000016 	A 相电压失压 ARD2
	GenerateBVoltageLossAlarm_E2000017(Index);//E2000017	B 相电压失压 ARD2
	GenerateCVoltageLossAlarm_E2000018(Index);//E2000018 	C 相电压失压 ARD2

	GenerateACurrentLossAlarm_E2000013(Index);//E2000013 A 相电流失流 ARD2
	GenerateBCurrentLossAlarm_E2000014(Index);//E2000014 B 相电流失流 ARD2
	GenerateCCurrentLossAlarm_E2000015(Index);//E2000015 C 相电流失流 ARD2

	GenerateACurrentReverseAlarm_E200000D(Index);//E200000D 	A 相潮流反向 ARD2
	GenerateBCurrentReverseAlarm_E200000E(Index);//E200000E 	B 相潮流反向 ARD2
	GenerateCCurrentReverseAlarm_E200000F(Index);//E200000F 	C 相潮流反向 ARD2

	GenerateLessMoneyAlarm_E200002B(Index, &Threshold);//E200002B 	剩余电费不足 ARD4

	GeneratePowerGoDownAlarm_E200002C(Index);//E200002C 示度下降 ARD3

	GeneratePowerStopAlarm_E200002E(Index, &Threshold);//E200002E 	电能表停走 ARD2

	GenerateProgramTimeChangeAlarm_E2000035(Index, &Threshold);//电能表编程时间更改 ARD2

	GenerateTimeRateChangeAlarm_E2000036(Index, &Threshold);//E2000036 电能表时段或费率更改 ARD1

	GenerateRelayChangeAlarm_E200003B(Index, &Threshold);//E200003B 继电器变位 ARD12

	GenerateMeterRelayControlAlarm_E200003C(Index, &Threshold);//E200003C 电能表拉合闸失败 ARD12
#if (Project == 500)
	GenerateMeterClockBatteryLowVoltageAlarm_E2000032(Index);//E2000032 	时钟电池电压过低 ARD1
#endif

	//GenerateMeterAMRFailAlarm_E200003D(Index);//在485/PLC抄表失败后才进行判断

	GenerateTimeFaultAlarm_E200003E(Index, &Threshold);//电能表时钟异常 ARD13

	//GenerateVoltageLossCntAlarm_E2000040(Index, &Threshold);

	//GenerateCurrentLossCntAlarm_E2000041(Index, &Threshold);

	//GenerateCurrentReverseCntAlarm_E2000042(Index, &Threshold);

	MR((u32)AlarmAMRDataOld[Index].State, (u32)AlarmAMRDataNew[Index].State, sizeof(AlarmAMRDataStruct) - 8);
	AlarmAMRDataOld[Index].State[0] = 0x01;
	AlarmAMRDataNew[Index].State[1] = 0x00;
#else
	if((AlarmAMRDataOld[Index].State[0] == 0) && (AlarmAMRDataNew[Index].State[1] == 0x01))
	{//以下四个告警不需要比较两个周期
		//err20190216 判断方式改变 比较两次发生总次数 不同则产生告警，此处屏蔽否则将误报
		//GenerateAVoltageLossAlarm_E2000016(Index);//err20181218 增加B C相失压失流潮流反向 此处暂不修改，不确定台体是否能够合格
		//GenerateACurrentLossAlarm_E2000013(Index);
		//GenerateProgramTimeChangeAlarm_E2000035(Index, &Threshold);
		//GenerateTimeFaultAlarm_E200003E(Index, &Threshold);
		MR((u32)&AlarmAMRDataOld[Index], (u32)&AlarmAMRDataNew[Index], sizeof(AlarmAMRDataStruct));
		AlarmAMRDataOld[Index].State[0] = 0x01;
		AlarmAMRDataNew[Index].State[1] = 0x00;
		return;
	}
	if(AlarmAMRDataNew[Index].State[1] == 0x00) return;
	if(AlarmAMRDataOld[Index].State[0] == 0x00) return;
	
	MR((u32)&Threshold, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));

	//err20181218 增加B C相失压失流潮流反向
	GenerateAVoltageLossAlarm_E2000016(Index);//E2000016 	A 相电压失压 ARD2
	GenerateBVoltageLossAlarm_E2000017(Index);//E2000017	B 相电压失压 ARD2
	GenerateCVoltageLossAlarm_E2000018(Index);//E2000018 	C 相电压失压 ARD2

	GenerateACurrentLossAlarm_E2000013(Index);//E2000013 A 相电流失流 ARD2
	GenerateBCurrentLossAlarm_E2000014(Index);//E2000014 B 相电流失流 ARD2
	GenerateCCurrentLossAlarm_E2000015(Index);//E2000015 C 相电流失流 ARD2

	GenerateACurrentReverseAlarm_E200000D(Index);//E200000D 	A 相潮流反向 ARD2
	GenerateBCurrentReverseAlarm_E200000E(Index);//E200000E 	B 相潮流反向 ARD2
	GenerateCCurrentReverseAlarm_E200000F(Index);//E200000F 	C 相潮流反向 ARD2

	GenerateLessMoneyAlarm_E200002B(Index, &Threshold);

	GeneratePowerGoDownAlarm_E200002C(Index);

	GeneratePowerStopAlarm_E200002E(Index, &Threshold);

	GenerateProgramTimeChangeAlarm_E2000035(Index, &Threshold);

	GenerateTimeRateChangeAlarm_E2000036(Index, &Threshold);

	GenerateRelayChangeAlarm_E200003B(Index, &Threshold);

	GenerateMeterRelayControlAlarm_E200003C(Index, &Threshold);
#if (Project == 500)
	GenerateMeterClockBatteryLowVoltageAlarm_E2000032(Index);
#endif

	//GenerateMeterAMRFailAlarm_E200003D(Index);

	GenerateTimeFaultAlarm_E200003E(Index, &Threshold);

	//GenerateVoltageLossCntAlarm_E2000040(Index, &Threshold);

	//GenerateCurrentLossCntAlarm_E2000041(Index, &Threshold);

	//GenerateCurrentReverseCntAlarm_E2000042(Index, &Threshold);

	MR((u32)AlarmAMRDataOld[Index].State, (u32)AlarmAMRDataNew[Index].State, sizeof(AlarmAMRDataStruct) - 8);
	AlarmAMRDataOld[Index].State[0] = 0x01;
	AlarmAMRDataNew[Index].State[1] = 0x00;
#endif
}

void SwitchMeterClearAMRInfo(AMRCtrlStruct* AMRCtrl)
{
	AMRCtrl->PLCTaskIDAckNum = 0;
	AMRCtrl->PLCTaskIDAddNum = 0;
	AMRCtrl->AMRTotalDataLen = 0;
	AMRCtrl->Cur645DataLen = 0;
	
}
/*
u16 FindNwDIFrom_AFN0C_0D_Tbl(u32 NwDI)
{
	u16 i = 0;
	for(i = 0; i < MAX_AFN0C_0D_IDENT_NUM; i++)
	{
		if(gc_stQCSGAFN0C_0D_DLT645_Table[i].u32Fn == NwDI) break;
	}
	return i;
}

u16 Find_AFN0D_NW_DI_Info(u32 NwDI)
{
	u16 i= 0;
	for(i = 0; i < MAX_AFN0D_DATA_LEN_NUM; i++)
	{
		if(gc_NW_AFN0D_DataLen_Table[i].u32Ident == NwDI) break;
	}
	return i;
}


u16 FindMeterInNodeList(u8* MeterAddr, NodeListStruct* NodeList, u16 NodeListNum)
{
	u16 i = 0;
	for(i = 0; i < NodeListNum; i++)
	{
		if((MeterAddr[0] == NodeList[i].Addr[0]) && (MeterAddr[1] == NodeList[i].Addr[1]) &&
		   (MeterAddr[2] == NodeList[i].Addr[2]) && (MeterAddr[3] == NodeList[i].Addr[3]) &&
		   (MeterAddr[4] == NodeList[i].Addr[4]) && (MeterAddr[5] == NodeList[i].Addr[5]))
		{
			break;
		}
	}
	return i;
}

u16 FindPortNextMeterInNodeList(u32 PORTn, NodeListStruct* NodeList, u16 StartIndex, u16 NodeListNum)
{
	u16 i = 0;
	for(i = StartIndex; i < NodeListNum; i++)
	{
		if(NodeList[i].PortID == (u8)PORTn) break;
	}
	return i;
}
*/

u32 Get_AMR_NormalTaskCtrlAddr(u32 PORTn)
{
	u32 AmrCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			AmrCtrlAddr = ADDR_RS485_CH1_NormalTaskCtrl;
			break;
		case RS485_2PORT:
			AmrCtrlAddr = ADDR_RS485_CH2_NormalTaskCtrl;
			break;
		case RS485_3PORT:
			AmrCtrlAddr = ADDR_RS485_CH3_NormalTaskCtrl;
			break;
		case PLC_PORT:
			AmrCtrlAddr = ADDR_PLC_NormalTaskCtrl;
			break;
		default:
			AmrCtrlAddr = 0;
			break;
	}
	return AmrCtrlAddr;
}


u32 Get_AMR_RelayTaskCtrlAddr(u32 PORTn)
{
	u32 AmrCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			AmrCtrlAddr = ADDR_RS485_CH1_RelayTaskCtrl;
			break;
		case RS485_2PORT:
			AmrCtrlAddr = ADDR_RS485_CH2_RelayTaskCtrl;
			break;
		case RS485_3PORT:
			AmrCtrlAddr = ADDR_RS485_CH3_RelayTaskCtrl;
			break;
		case PLC_PORT:
			AmrCtrlAddr = ADDR_PLC_RelayTaskCtrl;
			break;
		default:
			AmrCtrlAddr = 0;
			break;
	}
	return AmrCtrlAddr;
}



u32 Get_AMR_TaskCtrlAddr(u32 PORTn)
{
	u32 AmrCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			AmrCtrlAddr = ADDR_RS485_CH1_AMR_TaskCtrl;
			break;
		case RS485_2PORT:
			AmrCtrlAddr = ADDR_RS485_CH2_AMR_TaskCtrl;
			break;
		case RS485_3PORT:
			AmrCtrlAddr = ADDR_RS485_CH3_AMR_TaskCtrl;
			break;
		case PLC_PORT:
			AmrCtrlAddr = ADDR_PLC_AMR_TaskCtrl;
			break;
		case ACSAMPLEPORT:
			AmrCtrlAddr = 0;
			break;
		default:
			AmrCtrlAddr = 0;
			break;
	}
	return AmrCtrlAddr;
}

TaskIDEnum Get_AMR_TaskID(u32 PORTn)
{
	TaskIDEnum TaskID = TASK_ID_NONE;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			TaskID = TASK_ID_RS485_1_AMR;
			break;
		case RS485_2PORT:
			TaskID = TASK_ID_RS485_2_AMR;
			break;
		case RS485_3PORT:
			TaskID = TASK_ID_RS485_3_AMR;
			break;
		case PLC_PORT:
			TaskID = TASK_ID_PLC_AMR;
			break;
		default:
			break;
	}
	return TaskID;
}

u32 Get_AMS_TaskCtrlAddr(u32 PORTn)
{
	u32 AmrCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			AmrCtrlAddr = ADDR_RS485_CH1_AMS_TaskCtrl;
			break;
		case RS485_2PORT:
			AmrCtrlAddr = ADDR_RS485_CH2_AMS_TaskCtrl;
			break;
		case RS485_3PORT:
			AmrCtrlAddr = ADDR_RS485_CH3_AMS_TaskCtrl;
			break;
		case PLC_PORT:
			AmrCtrlAddr = 0;
			break;
		case ACSAMPLEPORT:
			AmrCtrlAddr = 0;
			break;
		default:
			AmrCtrlAddr = 0;
			break;
	}
	return AmrCtrlAddr;
}

TaskIDEnum Get_AMS_TaskID(u32 PORTn)
{
	TaskIDEnum TaskID = TASK_ID_NONE;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			TaskID = TASK_ID_RS485_1_AMS;
			break;
		case RS485_2PORT:
			TaskID = TASK_ID_RS485_2_AMS;
			break;
		case RS485_3PORT:
			TaskID = TASK_ID_RS485_3_AMS;
			break;
		case PLC_PORT:
			TaskID = TASK_ID_PLC_AMS;
			break;
		default:
			break;
	}
	return TaskID;
}

u32 Get_AMR_CH_DataBufAddr(u32 PORTn)
{
	u32 DataBufAddr = 0;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			DataBufAddr = ADDR_RS485_CH1_AMR_DataBuf;
			break;
		case RS485_2PORT:
			DataBufAddr = ADDR_RS485_CH2_AMR_DataBuf;
			break;
		case RS485_3PORT:
			DataBufAddr = ADDR_RS485_CH3_AMR_DataBuf;
			break;
		case PLC_PORT:
			DataBufAddr = ADDR_PLC_AMR_DataBuf;
			break;
		default:
			DataBufAddr = 0;
			break;
	}
	return DataBufAddr;
}

u32 Get_UartPortByNodePortID(u32 PortID)
{
	u32 Port = 0;
	switch(PortID)
	{
		case PORT_ID_RS485_1:
			Port = RS485_1PORT;
			break;
		case PORT_ID_RS485_2:
			Port = RS485_2PORT;
			break;
		case PORT_ID_RS485_3:
			Port = RS485_3PORT;
			break;
		case PORT_ID_PLC:
			Port = PLC_PORT;
			break;
		case PORT_ID_WIRELESS:
			Port = PLC_PORT;
			break;
		default:
			Port = 0;
			break;
	}
	return Port;
}

ResultEnum BuildMeterAMR645FrameStruct(DLT645FrameStruct* Frame, AMRCtrlStruct* AMR)
{
	ResultEnum Result = NO_ERROR;
	MC(0, (u32)Frame, sizeof(DLT645FrameStruct));

	//Frame->UsartID = RS485_PORT1_ID | RS485_PORT2_ID | RS485_PORT3_ID;
	//Frame->BaudRate = AMR->BaudRate;
	//Frame->NeedResp = 1;
	//Frame->WaitRespTime = METER_AMR_TIME_OUT;
	
	MR((u32)Frame->Addr, (u32)AMR->Addr, METER_ADDR_SIZE);
	if(AMR->Protocol == DLT_645_PROTOCOL_07)
	{
		Frame->CtrlCode = DLT_07_READ;
		Frame->DataLen = DLT_645_07_ID_SIZE;
		Frame->Data[0] = AMR->Cur645_DI;
		Frame->Data[1] = AMR->Cur645_DI >> 8;
		Frame->Data[2] = AMR->Cur645_DI >> 16;
		Frame->Data[3] = AMR->Cur645_DI >> 24;
	}
	else if(AMR->Protocol == DLT_645_PROTOCOL_97)
	{
		Frame->CtrlCode = DLT_97_READ;
		Frame->DataLen = DLT_645_97_ID_SIZE;
		Frame->Data[0] = AMR->Cur645_DI;
		Frame->Data[1] = AMR->Cur645_DI >> 8;		
	}
	else
	{
		Result = ERROR_WRONG_ACCESS;
	}
	return Result;
}

/****************************************************************************
功     能: 645帧合法性判断
入口参 数: FrameLen,FrameBuf
出口参 数: Frame,将解析后的645帧数据装载到Frame帧结构中，剔除头尾以及校验信息
返     回: 成功标识及错误标识
编     写: zhaoyun 
编写时 间; 2015-11-10
****************************************************************************/
ResultEnum DLT645FrameDcompress(DLT645FrameStruct* Frame, u8* FrameBuf, u32 FrameLen)
{
	u32 DataLen = 0, i = 0, j = 0;
	if(FrameLen < MIN_645_FRAME_LEN) return ERROR_WRONG_LENGTH;

	MC(0, (u32)Frame, sizeof(DLT645FrameStruct));

	for(i = 0; i < FrameLen; i++)
	{
		if((FrameBuf[i] != DLT_645_HEAD) || (FrameBuf[i + 7] != DLT_645_HEAD)) continue;
		
		DataLen = FrameBuf[i + 9];
		if(DataLen > MAX_645_DATA_LEN) continue;
		if(FrameBuf[i + 11 + DataLen] != DLT_645_TAIL) continue;
				
		if((FrameBuf[i + 10 + DataLen] != (u8)SumCaculation(&FrameBuf[i], DataLen + 10))) continue;
		
	 	MemCopy(Frame->Addr, &FrameBuf[i + 1], METER_ADDR_SIZE);
		Frame->CtrlCode = FrameBuf[i + 8];
		Frame->DataLen = FrameBuf[i + 9];
		MemCopy(Frame->Data, &FrameBuf[i + 10], Frame->DataLen);
		for(j = 0; j < Frame->DataLen; j++)
		{
			Frame->Data[j] -= DLT_645_FIXED_DATA_SUM;
		}
		return NO_ERROR;
	}
	
	return ERROR_InvalidData;
}

u32 Get645FrameDataID(DLT645FrameStruct* Frame, u8 ProtocolType)
{
	u32 DataID = 0;
	if(ProtocolType == DLT_645_PROTOCOL_97)
	{
		
		DataID += Frame->Data[1];
		DataID <<= 8;
		DataID += Frame->Data[0];
	}
	else
	{
		DataID += Frame->Data[3];
		DataID <<= 8;
		DataID += Frame->Data[2];
		DataID <<= 8;
		DataID += Frame->Data[1];
		DataID <<= 8;
		DataID += Frame->Data[0];
	}
	return DataID;
}


u8 GetProtocolTypeByCtrlCode(u8 CtrlCode)
{
	u8 i = 0;
	for(i = 0; i < DLT_645_07_CODE_NUM; i++)
	{
		if(CtrlCode == DLT645CtrlCode07[i])
		{
			return DLT_645_PROTOCOL_07;
		}
	}
	for(i = 0;i < DLT_645_97_CODE_NUM;i++)
	{
		if(CtrlCode == DLT645CtrlCode97[i])
		{
			return DLT_645_PROTOCOL_97;
		}
	}
	return DLT_645_PROTOCOL_NONE;
}

void AMR_ADDR_Display(u32 PORTn, u8* Addr)//抄表地址显示
{
	u32 i;
	u32 x;
	u8* p8;
	
	if(PORTn == ACSAMPLEPORT) return;

	p8 = (u8*)ADDR_DATABUFF;
	if((PORTn == RS485_1PORT) || (PORTn == RS485_2PORT) || (PORTn == RS485_3PORT))
	{
		CopyString((u8 *)"485     ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		if(PORTn==RS485_1PORT)
		{
			i = 1;
		}
		else if(PORTn==RS485_2PORT)
		{
			i = 2;
		}
		else
		{
			i = 3;
		}
		p8[3] = i;
	}
	else
	{
		CopyString((u8 *)"载波     ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
	}

	p8 += 5;
	for(i = 0; i < METER_ADDR_SIZE; i++)
	{
		x = 0;
		x = Addr[METER_ADDR_SIZE - i -1];
		if(((x >> 4) & 0xf) < 10)
		{
			p8[0] = ((x >> 4) & 0xf) + 0x30;
		}
		else
		{	
			p8[0] = ((x >> 4) & 0xf) + 55;
		}
		p8++;
		if((x & 0xf) < 10)
		{
			p8[0] = (x & 0xf) + 0x30;
		}
		else
		{
			p8[0] = (x & 0xf) + 55;
		}
		p8++;
	}
	p8[0]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
}

void AMR_NUM_Display(u32 PORTn, u32 TotalMeters, u32 SuccessMeters, u32 FailMeters)//抄表数量显示
{
	u32 i;
	u8* p8;
	
	if(PORTn == ACSAMPLEPORT) return;

	if((PORTn == RS485_1PORT) || (PORTn == RS485_2PORT) || (PORTn == RS485_3PORT))
	{
		//CopyString((u8 *)"485x____成____败____",(u8*)ADDR_DATABUFF);	// 拷贝字符串;返回拷贝字符串字节数(不计结束0)
		CopyString((u8 *)"485x____成____",(u8*)ADDR_DATABUFF);			// 拷贝字符串;返回拷贝字符串字节数(不计结束0)
		p8=(u8*)(ADDR_DATABUFF);
		if(PORTn==RS485_1PORT)
		{
			i = 1;
		}
		else if(PORTn==RS485_2PORT)
		{
			i = 2;
		}
		else
		{
			i = 3;
		}
		p8[3] = i;
	}
	else
	{
		//CopyString((u8 *)"载波____成____败____",(u8*)ADDR_DATABUFF);	//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		CopyString((u8 *)"载波____成____",(u8*)ADDR_DATABUFF);			//拷贝字符串;返回拷贝字符串字节数(不计结束0)
	}
	//总数
	p8 = (u8*)(ADDR_DATABUFF+4+3);
	p8[0] = '0';
	i = TotalMeters;
	i = hex_bcd(i);
	while(i)
	{
		p8[0] = (i & 0xf) + 0x30;
		p8--;
		i >>= 4;
	}
	//成功数
	p8 = (u8*)(ADDR_DATABUFF+10+3);
	p8[0] = '0';
	i = SuccessMeters;
 	i = hex_bcd(i);
	while(i)
	{
		p8[0] = (i & 0xf) + 0x30;
		p8--;
		i >>= 4;
	}
	//失败数
	p8 = (u8*)(ADDR_DATABUFF+14);
	p8[0] = 0;
/*
	p8 = (u8*)(ADDR_DATABUFF+16+3);
	p8[0] = '0';
	i = AMRCtrl->FailMeters;
	i = hex_bcd(i);
	while(i)
	{
		p8[0] = (i & 0xf) + 0x30;
		p8--;
		i >>= 4;
	}

	p8 = (u8*)(ADDR_DATABUFF + 20);
	p8[0] = 0;
*/	
	if(PORTn == PLC_PORT)
	{//载波口
		WarningOccur(100,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
	}
	else
	{
		//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
		WarningOccur(101,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
	}
}

void RT_AMR_Info_Display(u32 PORTn, u8* Addr, u32 Code)//抄表地址显示
{
	u32 i;
	u8* p8;
	
	if(PORTn == ACSAMPLEPORT) return;

	p8 = (u8*)ADDR_DATABUFF;
	if((PORTn == RS485_1PORT) || (PORTn == RS485_2PORT) || (PORTn == RS485_3PORT))
	{
		if(Code == RT_AMR_INFO_CODE_RT)
		{
			CopyString((u8 *)"485 正在实时抄表...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else if(Code == RT_AMR_INFO_CODE_RELAY)
		{
			CopyString((u8 *)"485 正在中继转发...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else if(Code == RT_AMR_INFO_CODE_CONTROL)
		{
			CopyString((u8 *)"485 正在控制转发...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else
		{
			CopyString((u8 *)"485 正在广播校时...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		if(PORTn==RS485_1PORT)
		{
			i = 1;
		}
		else if(PORTn==RS485_2PORT)
		{
			i = 2;
		}
		else
		{
			i = 3;
		}
		p8[3] = i;
	}
	else
	{
		if(Code == RT_AMR_INFO_CODE_RT)
		{
			CopyString((u8 *)"载波正在实时抄表...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else if(Code == RT_AMR_INFO_CODE_RELAY)
		{
			CopyString((u8 *)"载波正在中继转发...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else if(Code == RT_AMR_INFO_CODE_CONTROL)
		{
			CopyString((u8 *)"载波正在控制转发...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else
		{
			CopyString((u8 *)"载波正在广播校时...",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
	}

	p8[19]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
}

void GetTerminalReadMeterFlag(void)
{
	u32 ReadMeterOn = 0;
	
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	AMRCtrlStruct* AMRCtrl_RS4851 = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_1PORT);
	AMRCtrlStruct* AMRCtrl_RS4852= (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_2PORT);
	AMRCtrlStruct* AMRCtrl_RS4853 = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_3PORT);
	AMRCtrlStruct* AMRCtrl_PLC= (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PLC_PORT);
	
	
	if(AMRCtrl_RS4851->AMR_ID != 0)
	{
		pTSysCtrl->Flag |= TERMINAL_READ_METER_ON_FLAG;
		ReadMeterOn++;
	}	
	if(AMRCtrl_RS4852->AMR_ID != 0)
	{
		pTSysCtrl->Flag |= TERMINAL_READ_METER_ON_FLAG;
		ReadMeterOn++;
	}
	
	if(AMRCtrl_RS4853->AMR_ID != 0)
	{
		pTSysCtrl->Flag |= TERMINAL_READ_METER_ON_FLAG;
		ReadMeterOn++;
	}
	if(AMRCtrl_PLC->AMR_ID != 0)
	{
		pTSysCtrl->Flag |= TERMINAL_READ_METER_ON_FLAG;
		ReadMeterOn++;
	}
	if(ReadMeterOn == 0)
	{
		pTSysCtrl->Flag &= ~TERMINAL_READ_METER_ON_FLAG; 
	}
	
}
#endif
/**************************************** (C) COPYRIGHT zhaoyun **********************************END OF FILE****/


