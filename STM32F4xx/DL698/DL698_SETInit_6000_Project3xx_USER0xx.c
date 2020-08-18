
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"


#if ((Project/100)==3)//专变
#if USER<200//盲样电科院送检系,标准系



//#define ADDR_6000     ADDR_4204_3+LENmax_4204_3//采集档案配置表(初始化值0x00)
//#define LENper_6000     128
//#define NUMmax_6000     RMNmax
//#define LENmax_6000     (LENper_6000*NUMmax_6000)
const u8 InitData_6000_2_0[]=//采集档案配置0
{
	0,0,
};
const u8 InitData_6000_2_1[]=//采集档案配置1
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,1,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x01,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	255,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
	DataType_enum,
	3,
//端口      OAD，
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //交流采样通信口号(虚拟)
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	0,
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
	3,
	#else
	2,
	#endif
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};
const u8 InitData_6000_2_2[]=//采集档案配置2
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,2,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x11,0x11,0x11,0x11,0x11,0x11,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	3,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
	DataType_enum,
	2,
//端口      OAD，
	DataType_OAD,
	0xF2,0x01,0x02,0x01, //RS485_1
	//0xF2,0x01,0x02,0x02, //RS485_2
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	0,
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	3,
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};
const u8 InitData_6000_2_3[]=//采集档案配置3
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,3,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x03,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	3,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
	DataType_enum,
	3,
//端口      OAD，
	DataType_OAD,
	0xF2,0x01,0x02,0x01, //RS485_1
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	0,
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	3,
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};

const u8 InitData_6000_2_4[]=//采集档案配置4
{
	0,0,
};

const u8 InitData_6000_2_5[]=//采集档案配置5
{
	0,0,
};

const u8 InitData_6000_2_6[]=//采集档案配置6
{
	0,0,
};

const u8 InitData_6000_2_7[]=//采集档案配置7
{
	0,0,
};

const u8 InitData_6000_2_8[]=//采集档案配置8
{
	0,0,
};

const u8 InitData_6000_2_9[]=//采集档案配置9
{
	0,0,
};

//#define ADDR_6002_8     ADDR_6000+LENmax_6000//搜表参数
//#define LENmax_6002_8     10
const u8 InitData_6002_8[]=
{
//属性8∷=structure
	DataType_structure,
	4,
//{
//是否启用每天周期搜表   bool，
	DataType_bool,
	0,
//自动更新采集档案       bool，
	DataType_bool,
	0,
//是否产生搜表相关事件   bool，
	DataType_bool,
	0,
//清空搜表结果选项       enum
//{
//不清空             （0），
//每天周期搜表前清空 （1），
//每次搜表前清空     （2）
//}
//}
	DataType_enum,
	0,
};

//#define ADDR_6002_9     ADDR_6002_8+LENmax_6002_8//每天周期搜表参数配置
//#define LENmax_6002_9     (4*16)
const u8 InitData_6002_9[]=
{
//属性9（每天周期搜表参数配置）∷=array 定时搜表参数
	DataType_array,
	0,
//定时搜表参数∷=structure
//{
//   开始时间         time，
//   搜表时长（min）  long-unsigned
//}
};

//#define ADDR_6002_127     ADDR_6002_9+LENmax_6002_9//实时启动搜表(初始化值0xFF)
//#define LENmax_6002_127     3
const u8 InitData_6002_127[]=
{
//搜表时长∷=long-unsigned，单位：分钟，表示搜表持续时间，0表示不限时间直至搜表结束。
	DataType_long_unsigned,
	0xFF,0xFF,
};

//#define ADDR_6012_2     ADDR_6002_127+LENmax_6002_127//任务配置表(初始化值0x00)
//#define LENper_6012_2     128
//#define NUMmax_6012_2     16
//#define LENmax_6012_2     (LENper_6012_2*NUMmax_6012_2)
#define InitData_6012_2_0_
const u8 InitData_6012_2_0[]=//任务配置0
{
	0,0,
};

#define InitData_6012_2_1_
const u8 InitData_6012_2_1[]=//任务配置1（电能表当前数据）
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	1,
//执行频率   TI，
	DataType_TI,
	3,0,1,   //1天
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	1,
//方案编号    unsigned，
	DataType_unsigned,
	1,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,0x01,0x01,0x00,0x00,0x00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,0x01,0x01,0x00,0x00,0x00, 
//延时        TI，
	DataType_TI,
	1,0,2,    //2分钟
//执行优先级  enum{首要（1），必要（2），需要（3），可能（4）}，
	DataType_unsigned,
	0,
//状态        enum{正常（1），停用（2）}，
	DataType_enum,
  1,
//任务开始前脚本id   long-unsigned，
	DataType_long_unsigned,
	0,0,
//任务完成后脚本id   long-unsigned，
	DataType_long_unsigned,
	0,0,
//任务运行时段       structure，
	DataType_structure,
	2,
//{
//   类型  enum
//{
//       前闭后开    （0），
//       前开后闭    （1），
//       前闭后闭    （2），
//       前开后开    （3）
//}，
	DataType_enum,
	0,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	0,
//起始分钟  unsigned，
	DataType_unsigned,
	0,
//结束小时  unsigned，
	DataType_unsigned,
	23,
//结束分钟  unsigned
	DataType_unsigned,
	59,
//}
//}
//}
//当方案类型为脚本时，方案编号为脚本id
};

#define InitData_6012_2_2_
const u8 InitData_6012_2_2[]=//任务配置2（历史日数据）
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	2,
//执行频率   TI，
	DataType_TI,
	1,0,5, //5分钟
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	1,
//方案编号    unsigned，
	DataType_unsigned,
	2,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,0x01,0x01,0x00,0x00,0x00,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,0x01,0x01,0x00,0x00,0x00,
//延时        TI，
	DataType_TI,
	0,0,0,
//执行优先级  enum{首要（1），必要（2），需要（3），可能（4）}，
	DataType_unsigned,
	0,
//状态        enum{正常（1），停用（2）}，
	DataType_enum,
  1,
//任务开始前脚本id   long-unsigned，
	DataType_long_unsigned,
	0,0,
//任务完成后脚本id   long-unsigned，
	DataType_long_unsigned,
	0,0,
//任务运行时段       structure，
	DataType_structure,
	2,
//{
//   类型  enum
//{
//       前闭后开    （0），
//       前开后闭    （1），
//       前闭后闭    （2），
//       前开后开    （3）
//}，
	DataType_enum,
	0,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	0,
//起始分钟  unsigned，
	DataType_unsigned,
	0,
//结束小时  unsigned，
	DataType_unsigned,
	23,
//结束分钟  unsigned
	DataType_unsigned,
	59,
//}
//}
//}
//当方案类型为脚本时，方案编号为脚本id
};

//#define InitData_6012_2_3_
//const u8 InitData_6012_2_3[]=//任务配置3（负荷曲线数据）
//{
////属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
//	DataType_structure,
//	12,
////{
////任务ID    unsigned，
//	DataType_unsigned,
//	3,
////执行频率   TI，
//	DataType_TI,
//	3,0,1,
////方案类型   enum
////{
////普通采集方案 （1），   事件采集方案 （2），
////透明方案     （3），   上报方案     （4），
////脚本方案     （5）
////}，
//	DataType_enum,
//	1,
////方案编号    unsigned，
//	DataType_unsigned,
//	3,
////开始时间    date_time_s，
//	DataType_date_time_s,
// 0x07,0xE0,0x09,0x0C,0x00,0x02,0x00,
////结束时间    date_time_s，
//	DataType_date_time_s,
// 0x08,0x33,0x09,0x09,0x09,0x09,0x09,
////延时        TI，
//	DataType_TI,
//	0,0,0,
////执行优先级  enum{首要（1），必要（2），需要（3），可能（4）}，
//	DataType_unsigned,
//	3,
////状态        enum{正常（1），停用（2）}，
//	DataType_enum,
//  1,
////任务开始前脚本id   long-unsigned，
//	DataType_long_unsigned,
//	0,0,
////任务完成后脚本id   long-unsigned，
//	DataType_long_unsigned,
//	0,0,
////任务运行时段       structure，
//	DataType_structure,
//	2,
////{
////   类型  enum
////{
////       前闭后开    （0），
////       前开后闭    （1），
////       前闭后闭    （2），
////       前开后开    （3）
////}，
//	DataType_enum,
//	0,
////时段表  array 时段
//	DataType_array,
//	01,
////时段∷=structure
//	DataType_structure,
//	4,
////{
////起始小时  unsigned，
//	DataType_unsigned,
//	0,
////起始分钟  unsigned，
//	DataType_unsigned,
//	0,
////结束小时  unsigned，
//	DataType_unsigned,
//	0x17,
////结束分钟  unsigned
//	DataType_unsigned,
//	0x3B,
////}
////}
////}
////当方案类型为脚本时，方案编号为脚本id
//};

//#define ADDR_6014     ADDR_6012_2+LENmax_6012_2//普通采集方案
//#define LENper_6014     128
//#define NUMmax_6014     16
//#define LENmax_6014     (LENper_6014*NUMmax_6014)
#define InitData_6014_2_0_
const u8 InitData_6014_2_0[]=
{
	0,0,
};
#define InitData_6014_2_1_
const u8 InitData_6014_2_1[]=//电能表当前数据
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	1,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//采集方式  structure
	DataType_structure,
	2,
//{
//  采集类型  unsigned，
	DataType_unsigned,
	2,
//  采集内容  Data
	DataType_NULL,
//}，
//记录列选择  array CSD，
	DataType_array,
	01,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	2,
	0x00,0x10,0x02,0x00,
	0x00,0x20,0x02,0x00,
//电能表集合  MS，
	DataType_MS,
	0x01,	
//存储时标选择 enum
//{
//未定义              （0），
//任务开始时间        （1），
//相对当日0点0分    （2），
//相对上日23点59分  （3），
//相对上日0点0分    （4），
//相对当月1日0点0分（5），
//数据冻结时标        （6）
//}
	DataType_enum,
	4,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};
#define InitData_6014_2_2_
const u8 InitData_6014_2_2[]=//历史日数据
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	2,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//采集方式  structure
	DataType_structure,
	2,
//{
//  采集类型  unsigned，
	DataType_unsigned,
	0,
//  采集内容  Data
	DataType_NULL,
//}，
//记录列选择  array CSD，
	DataType_array,
	5,
	DataType_CSD,
	0x00,0x00,0x10,0x02,0x00,//正向有功
	DataType_CSD,
	0x00,0x00,0x20,0x02,0x00,//反向有功
	DataType_CSD,
	0x00,0x20,0x00,0x02,0x00,//电压
	DataType_CSD,
	0x00,0x20,0x01,0x02,0x00,//电流
	DataType_CSD,
	0x00,0x20,0x04,0x02,0x00,//有功功率
//电能表集合  MS，
	DataType_MS,
	0x01,	
//存储时标选择 enum
//{
//未定义              （0），
//任务开始时间        （1），
//相对当日0点0分    （2），
//相对上日23点59分  （3），
//相对上日0点0分    （4），
//相对当月1日0点0分（5），
//数据冻结时标        （6）
//}
	DataType_enum,
	0,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};

/*
#define InitData_6014_2_3_
const u8 InitData_6014_2_3[]=//负荷曲线数据
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	3,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	0,96,
//采集方式  structure
	DataType_structure,
	2,
//{
//  采集类型  unsigned，
	DataType_unsigned,
	3,
//  采集内容  Data
	DataType_TI,
	1,0,15,
//}，
//记录列选择  array CSD，
	DataType_array,
	1,
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	3,
	0x20,0x21,0x02,0x00,
	0x00,0x10,0x02,0x00,
	0x00,0x20,0x02,0x00,
//电能表集合  MS，
	DataType_MS,
	0x01,	
//存储时标选择 enum
//{
//未定义              （0），
//任务开始时间        （1），
//相对当日0点0分    （2），
//相对上日23点59分  （3），
//相对上日0点0分    （4），
//相对当月1日0点0分（5），
//数据冻结时标        （6）
//}
	DataType_enum,
	4,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};
*/

//#define ADDR_6016     ADDR_6014+LENmax_6014//事件采集方案集(初始化值0x00)
////属性2（事件采集方案Event acq plan）∷=structure
////{
////方案编号        unsigned，
////采集方式        structure
////{
////  采集类型      unsigned，
////  采集内容      Data
////}
////电能表集合      MS，
////上报标识        bool（True：立即上报，False：不上报），
////存储深度        long-unsigned
////}
////采集方式数据格式如下：
////采集类型	采集内容	表示
////0	array ROAD	周期采集事件数据
////1	NULL	根据通知采集所有事件数据
////2	array ROAD	根据通知采集指定事件数据
////3 NULL  直接存储通知事件数据
//#define LENper_6016     1024
//#define NUMmax_6016     64
//#define LENmax_6016     (LENper_6016*NUMmax_6016)

//#define ADDR_601C     ADDR_6016+LENmax_6016//上报方案集(初始化值0x00)
////属性2（上报方案 report plan）∷=structure//删除时structure的数据单元清为0(NULL)
////{
////方案编号          unsigned，
////上报通道          array OAD，
////上报响应超时时间  TI，
////最大上报次数      unsigned，
////上报内容          strcuture
////{
////  类型  unsigned，
////  数据  Data
////}
////}
////上报内容：
////上报类型	  上报内容	表示
//// 0	      OAD	对象属性数据
//// 1	      RecordData	上报记录型对象属性
////RecordData∷=structure
////{
////  主对象属性描述符      OAD，
////记录型对象属性描述符  RCSD，
////行选择                RSD
////}
//#define LENper_601C     1024
//#define NUMmax_601C     16
//#define LENmax_601C     (LENper_601C*NUMmax_601C)

const u8 InitData_601C_2_0[]=
{
	0,0,
};


//#define ADDR_601E     ADDR_601C+LENmax_601C//采集规则库(初始化值0x00)
////属性2∷=structure//删除时structure的数据单元清为0(NULL)
////{
////数据列选择描述符  CSD，
////规则描述  structrue
////{
////AcqCmd_2007  structure
////{
////主用DI  array octet-string(SIZE(4))，
////备用DI  array octet-string(SIZE(4))
////}，
////AcqCmd_1997  structure
////{
////主用DI  array octet-string(SIZE(2))，
////备用DI  array octet-string(SIZE(2))
////}，
////AcqCmd_Trans  structure
////{
////Frame  octet-string
////}
////}
////}
//#define LENper_601E     256
//#define NUMmax_601E     256
//#define LENmax_601E     (LENper_601E*NUMmax_601E)


//#define ADDR_8000_2     ADDR_601E+LENmax_601E//遥控-属性2（配置参数)
//#define LENmax_8000_2     10
const u8 InitData_8000_2[]=
{
//属性2（配置参数)∷=structure
//{
//  继电器拉闸电流门限值    double-long-unsigned（单位：A，换算-4），
//  超电流门限保护延时时间  long-unsigned（单位：分钟，换算0）
//}
	0,0,
};

//#define ADDR_8000_127     ADDR_8000_2+LENmax_8000_2//遥控-方法127：触发告警（参数）
//#define LENmax_8000_127     1
const u8 InitData_8000_127[]=
{
//方法127：触发告警（参数）参数∷=NULL
//方法128：解除报警（参数）参数∷=NULL
//触发=0x55,解除=0x0
	0,
};

//#define ADDR_8000_129     ADDR_8000_127+LENmax_8000_127//遥控-方法129：跳闸（参数）
//#define LENper_8000_129     16
//#define NUMmax_8000_129     4
//#define LENmax_8000_129     (LENper_8000_129*NUMmax_8000_129)
const u8 InitData_8000_129[]=
{
//方法129：跳闸（参数）
//参数∷=array structure
//{
//继电器      OAD，
//告警延时    unsigned（单位：分钟，换算：0），
//限电时间    long-unsigned（单位：分钟，换算：0；值为0表示永久限电），
//自动合闸    bool（True：自动合闸；False：非自动合闸）
//}
	0,0,
};

//#define ADDR_8000_130     ADDR_8000_129+LENmax_8000_129//遥控-方法130：合闸（参数）
//#define LENper_8000_130     12
//#define NUMmax_8000_130     4
//#define LENmax_8000_130     (LENper_8000_130*NUMmax_8000_130)
const u8 InitData_8000_130[]=
{
//方法130：合闸（参数）
//参数∷=array structure
//{
//继电器    OAD，
//命令      enum{合闸允许（0），直接合闸（1）}
//}
	0,0,
};

//#define ADDR_8001_127     ADDR_8000_130+LENmax_8000_130//方法127：投入保电
//#define LENmax_8001_127     1
const u8 InitData_8001_127[]=
{
//方法127：投入保电（参数）参数∷=NULL
//用于投入保电状态，即禁止一切执行跳闸的继电器输出控制，且恢复已跳闸的继电器输出控制。
//方法128：解除保电（参数）参数∷=NULL
//用于解除保电状态。
//投入=0x55,解除=0x0
	0,
};

//#define ADDR_8001_3     ADDR_8000_130+LENmax_8000_130//保电-允许与主站最大无通信时长（分钟）
//#define LENmax_8001_3     3
const u8 InitData_8001_3[]=
{
//属性3∷=long-unsigned
//允许与主站最大无通信时长（分钟），0表示不自动保电。
	DataType_long_unsigned,
	0,0,
};

//#define ADDR_8001_4     ADDR_8001_3+LENmax_8001_3//保电-上电自动保电时长（分钟）
//#define LENmax_8001_4     3
const u8 InitData_8001_4[]=
{
//属性4∷=long-unsigned
//上电自动保电时长（分钟），0表示上电不自动保电。
	DataType_long_unsigned,
	0,0,
};

//#define ADDR_8001_5     ADDR_8001_4+LENmax_8001_4//保电-自动保电时段
//#define LENper_8001_5     8
//#define NUMmax_8001_5     24
//#define LENmax_8001_5     (LENper_8001_5*NUMmax_8001_5)
const u8 InitData_8001_5[]=
{
//属性5∷=array 自动保电时段
//自动保电时段∷=structure
//{
//起始时间（时）  unsigned，
//结束时间（时）  unsigned
//}
//时间段区间规则为前闭后开。
	0,0,
};

//#define ADDR_8001_129     ADDR_8001_127+LENmax_8001_127//方法129：解除自动保电
////方法129：解除自动保电（参数）参数∷=NULL
////用于解除自动保电状态。
////解除自动保电=0x55,自动保电=0x0
//#define LENmax_8001_129     1

//#define ADDR_8002_127     ADDR_8001_129+LENmax_8001_129//方法127：催费告警投入
//#define LENmax_8002_127     256
const u8 InitData_8002_127[]=
{
//方法127：催费告警投入（参数）
//参数∷=structure//删除时structure的数据单元清为0(NULL)
//{
//  告警时段：octet-string(SIZE(3))，
//  告警信息：visible-string(SIZE(200))
//}
	0,0,
//方法128：取消催费告警（参数）参数∷=NULL
};

//#define ADDR_8100_2     ADDR_8002_127+LENmax_8002_127//终端保安定值
//#define LENmax_8100_2     9
const u8 InitData_8100_2[]=
{
//终端保安定值∷=long64（单位：W，换算：-1）
	DataType_long64,
  0,0,0,0,0,(1000000>>16)&0xff,(1000000>>8)&0xff,1000000&0xff,//保安定值 100kW	
	
};

//#define ADDR_8101_2     ADDR_8100_2+LENmax_8100_2//终端功控时段
//#define LENmax_8101_2     16
const u8 InitData_8101_2[]=
{
//属性2（配置参数）∷=array unsigned
	DataType_array,
	12,
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
	DataType_unsigned,
	0x55,//控制1
};

//#define ADDR_8102_2     ADDR_8101_2+LENmax_8101_2//功控告警时间
//#define LENmax_8102_2     16
const u8 InitData_8102_2[]=
{
//属性2（配置参数）∷=array unsigned
	DataType_array,
	2,
	DataType_unsigned,
	1,
	DataType_unsigned,
	1,
};

//#define ADDR_8103_2     ADDR_8102_2+LENmax_8102_2//时段功控-控制方案集(初始化值0x00)
//#define LENper_8103_2     241//实长度
//#define NUMmax_8103_2     TAGmax
//#define LENmax_8103_2     (2+( LENper_8103_2*NUMmax_8103_2))
//#if LENmax_8103_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8103_2[]=
{
//属性2（控制方案集）∷=array 时段功控配置单元
	DataType_array,
	1,
//属性2∷=structure//
	DataType_structure,
	6,
//{
//总加组对象  OI，
	DataType_OI,
	0x23,0x01,
//方案标识    bit-string(SIZE(8))，
	DataType_bit_string,
	8,
	0xE0,
//第一套定值  PowerCtrlParam，
	DataType_structure,
	9,
	DataType_bit_string,
	8,
	0xFF,
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
//第二套定值  PowerCtrlParam，
	DataType_structure,
	9,
	DataType_bit_string,
	8,
	0xFF,
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
//第三套定值  PowerCtrlParam，
	DataType_structure,
	9,
	DataType_bit_string,
	8,
	0xFF,
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
//时段功控定值浮动系数  integer（单位：%）
	DataType_integer,
	0,
//}
//PowerCtrlParam∷=structure
//{
//	时段号          bit-string(SIZE(8))，
//	时段1功控定值  long64（单位：W，换算：-1），
//	时段2功控定值  long64（单位：W，换算：-1），
//	时段3功控定值  long64（单位：W，换算：-1），
//	时段4功控定值  long64（单位：W，换算：-1），
//	时段5功控定值  long64（单位：W，换算：-1），
//	时段6功控定值  long64（单位：W，换算：-1），
//	时段7功控定值  long64（单位：W，换算：-1），
//	时段8功控定值  long64（单位：W，换算：-1）
//}
};

//#define ADDR_8103_127     ADDR_8103_2+LENmax_8103_2//时段功控-方案切换(初始化值0x00)
//#define LENper_8103_127     12//实长度
//#define LENmax_8103_127     (2+(LENper_8103_127*NUMmax_TAG))
const u8 InitData_8103_127[]=
{
//时段功控方案切换∷=structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//时段功控投入标识    bit-string(SIZE(8))，
//时段功控定值方案号  unsigned
//}
//时段功控投入标识：D0～D7按顺序对位表示第1～第8时段，置“1”：有效，置“0”：无效。
//时段功控定值方案号：数值范围：0～2依次表示第1～第3套方案，其他值无效。
	0,0,
};

//#define ADDR_8104_2     ADDR_8103_127+LENmax_8103_127//厂休控配置单元
//#define LENper_8104_2     28//实长度
//#define NUMmax_8104_2     TAGmax
//#define LENmax_8104_2     (2+(LENper_8104_2*NUMmax_8104_2))
//#if LENmax_8104_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8104_2[]=
{
//array
//属性2∷=structure
//{
//总加组对象    OI，
//厂休控定值    long64（单位：W，换算：-1），
//限电起始时间  date_time_s（年=FFFFH，月=FFH，日=FFH），
//限电延续时间  long-unsigned（单位：分钟），
//每周限电日    bit-string(SIZE(8)) 
//}
//每周限电日：D1～D7表示星期一～星期日，D0=0。
	0,0,
};

//#define ADDR_8105_2     ADDR_8104_3+LENmax_8104_3//营业报停控配置单元(初始化值0x00)
//#define LENper_8105_2     30//实长度
//#define NUMmax_8105_2     TAGmax
//#define LENmax_8105_2     (2+(LENper_8105_2*NUMmax_8105_2))
//#if LENmax_8105_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8105_2[]=
{
//array
//属性2∷=structure
//{
//总加组对象      OI，
//报停起始时间    date_time_s（时=FFH，分=FFH），
//报停结束时间    date_time_s（时=FFH，分=FFH），
//报停控功率定值  long64（单位：W，换算：-1）
//}
	0,0,
};

//#define ADDR_8106_127     ADDR_8105_3+LENmax_8105_3//当前功率下浮控-方法127 投入（总加组对象，控制方案）(初始化值0x00)
//#define LENper_8106_127     23//实长度
//#define NUMmax_8106_127     TAGmax
//#define LENmax_8106_127     (2+(LENper_8106_127*NUMmax_8106_127))
//#if LENmax_8106_127>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8106_127[]=
{
//array
//structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//当前功率下浮控定值滑差时间    unsigned（单位：分钟），
//当前功率下浮控定值浮动系数    integer（单位：%），
//控后总加有功功率冻结延时时间  unsigned（单位：分钟），
//当前功率下浮控的控制时间      unsigned（单位：0.5小时），
//当前功率下浮控第1轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第2轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第3轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第4轮告警时间  unsigned（单位：分钟）
////}
	0,0,
};

//#define ADDR_8107_2     ADDR_8106_3+LENmax_8106_3//购电控配置单元
//#define LENper_8107_2     43//实长度
//#define NUMmax_8107_2     TAGmax
//#define LENmax_8107_2     (2+(LENper_8107_2*NUMmax_8107_2))
//#if LENmax_8107_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8107_2[]=
{
//array
//属性2∷=structure
//{
//	总加组对象      OI，
//购电单号        double-long-unsigned，
//追加/刷新标识   enum{追加（0），刷新（1）}，
//购电类型        enum{电量（0），电费（1）}，
//购电量（费）值  long64（单位：kWh/元， 换算：-4），
//	报警门限值      long64（单位：kWh/元，换算：-4），
//	跳闸门限值      long64（单位：kWh/元，换算：-4）
//	购电控模式      enum{本地模式（0），远程模式（1）}
//}
	0,0,
};

//#define ADDR_8108_2     ADDR_8107_3+LENmax_8107_3//月电控配置单元
//#define LENper_8108_2     18//实长度
//#define NUMmax_8108_2     TAGmax
//#define LENmax_8108_2     (2+(LENper_8108_2*NUMmax_8108_2))
//#if LENmax_8108_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8108_2[]=
{
//array
//属性2∷=structure
//{
//	总加组对象      OI，
//	月电量控定值    long64（单位：kWh，换算：-4），
//	报警门限值系数  unsigned（单位：%），
//	月电量控定值浮动系数  integer（单位：%）
//}
	0,0,
};

//#define ADDR_8103_3     ADDR_8103_2+LENmax_8103_2//时段功控-总加组控制投入状态(初始化值0x00)
//#define LENper_8103_3     7//实长度
//#define LENmax_8103_3     (2+(LENper_8103_3*NUMmax_8103_2))
const u8 InitData_8103_3[]=
{
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
	DataType_array,
	NUMmax_TAG,
#if NUMmax_TAG>=1
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x01,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=2
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x02,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=3
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x03,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=4
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x04,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=5
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x05,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=6
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x06,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=7
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x07,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=8
//一个总加组控制投入状态∷=structure
	DataType_structure,
	2,
//{
//  总加组对象  OI，
	DataType_OI,
	0x23,0x08,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
	0,
#endif
//}
};

//#define ADDR_8104_3     ADDR_8104_2+LENmax_8104_2//厂休控-总加组控制投入状态(初始化值0x00)
//#define LENper_8104_3     7//实长度
//#define LENmax_8104_3     (2+(LENper_8104_3*NUMmax_8104_2))
const u8 InitData_8104_3[]=
{
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
	0,0,
};

//#define ADDR_8105_3     ADDR_8105_2+LENmax_8105_2//营业报停控-总加组控制投入状态(初始化值0x00)
//#define LENper_8105_3     7//实长度
//#define LENmax_8105_3     (2+(LENper_8105_3*NUMmax_8105_2))
const u8 InitData_8105_3[]=
{
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
	0,0,
};

//#define ADDR_8106_3     ADDR_8106_127+LENmax_8106_127//当前功率下浮控-总加组控制投入状态(初始化值0x00)
//#define LENper_8106_3     7//实长度
//#define LENmax_8106_3     (2+(LENper_8106_3*NUMmax_8106_127))
const u8 InitData_8106_3[]=
{
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
	0,0,
};

//#define ADDR_8107_3     ADDR_8107_2+LENmax_8107_2//购电控-总加组控制投入状态(初始化值0x00)
//#define LENper_8107_3     7//实长度
//#define LENmax_8107_3     (2+(LENper_8107_3*NUMmax_8107_2))
const u8 InitData_8107_3[]=
{
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
////}
	0,0,
};

//#define ADDR_8108_3     ADDR_8108_2+LENmax_8108_2//月电控-总加组控制投入状态(初始化值0x00)
//#define LENper_8108_3     7//实长度
//#define LENmax_8108_3     (2+(LENper_8108_3*NUMmax_8108_2))
const u8 InitData_8108_3[]=
{
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
////}
	0,0,
};


//#define ADDR_F002_4     ADDR_F001_7+LENmax_F001_7
//#define LENmax_F002_4     64
const u8 InitData_F002_4[]=
{
//属性4（服务器信息）∷=structure
	DataType_structure,
	4,
//{
//IP地址  octet-string，
	DataType_octet_string,4,
	192,168,1,100,
//端口    long-unsigned，
	DataType_long_unsigned,
	0,31,
//用户名  visible-string，
	DataType_visible_string,4,
	'c','a','r','d',
//密码    visible-string
	DataType_visible_string,4,
	'c','a','r','d',
//}
};

//#define ADDR_F101_2     ADDR_8108_3+LENmax_8108_3//安全模式参数(初始化值0x00)
//#define LENmax_F101_2     2
const u8 InitData_F101_2[]=
{
//属性2（安全模式选择）∷=enum
//{
//不启用安全模式参数（0），
//启用安全模式参数  （1）
//}
//安全模式选择意义：0—不启用安全模式参数、默认安全模式参数，1—启用安全模式参数、默认安全模式参数。
	DataType_enum,
	0,
};

//#define ADDR_F101_3     ADDR_F101_2+LENmax_F101_2//显式安全模式参数(初始化值0x00)
//#define LENper_F101_3     8//实长度
//#define NUMmax_F101_3     127
//#define LENmax_F101_3     (4+(LENper_F101_3*NUMmax_F101_3))
//#if LENmax_F101_3>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_F101_3[]=
{
//属性3（显式安全模式参数）∷=array 安全模式参数
	DataType_array,
	0,
//安全模式参数∷=structure
//{
//  对象标识   OI，
//  安全模式   long-unsigned
//}
//属性3为显式安全模式参数（设置值），如果对象安全性不在属性3中，则按默认安全模式参数。如果同一对象安全性在安全模式参数、默认安全模式参中均有说明，则按显式安全模式参数执行。安全模式参数、默认安全参数具体定义见附录F，安全模式定义见附录F中表F.1。
};

//#define ADDR_F200_2     ADDR_F101_3+LENmax_F101_3//RS232
//#define LENper_F200_2     15
//#define LENmax_F200_2     LENper_F200_2
const u8 InitData_F200_2[]=
{
//端口∷=structure
	DataType_structure,
	3,
//{
//端口号     OAD
	DataType_OAD,
	0xF2,0x00,0x02,0x01,
//端口参数   COMDCB，
	DataType_COMDCB,
	0x06,0x02,0x08,0x01,0x00,
//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
	DataType_enum,
	0,
//}
};

//#define ADDR_F201_2     ADDR_F200_2+LENmax_F200_2//RS485
//#define LENper_F201_2     15
//#define NUMmax_F201_2     3
//#define LENmax_F201_2     (LENper_F201_2*NUMmax_F201_2)
const u8 InitData_F201_2[]=
{
//端口∷=structure
	DataType_structure,
	3,
//{
//端口号     OAD
	DataType_OAD,
	0xF2,0x01,0x02,0x01,
//端口参数   COMDCB，
	DataType_COMDCB,
	0x03,0x02,0x08,0x01,0x00,
//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
	DataType_enum,
	1,
//}
};

//#define ADDR_F202_2     ADDR_F201_2+LENmax_F201_2//红外
//#define LENper_F202_2     15
//#define LENmax_F202_2     LENper_F202_2
const u8 InitData_F202_2[]=
{
//端口∷=structure
	DataType_structure,
	2,
//{
//端口号     OAD
	DataType_OAD,
	0xF2,0x02,0x02,0x01,
//端口参数   COMDCB，
	DataType_COMDCB,
	0x02,0x02,0x08,0x01,0x00,
//}
};

//#define ADDR_F203_4     ADDR_F202_2+LENmax_F202_2
const u8 InitData_F203_4[]=
{
//属性4∷=structure
	DataType_structure,
	2,
//{
//开关量接入标志bit-string(SIZE（8）)
//（
//bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”：接入，置“0”：未接入。
//），
	DataType_bit_string,
	8,
	0xff,
//开关量属性标志bit-string(SIZE（8）)
//（
//  bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”常开触点。置“0”：常闭触点。
//）
	DataType_bit_string,
	8,
	0xff,
//}
};
//#define LENmax_F203_4     8

//#define ADDR_F204_4     ADDR_F203_4+LENmax_F203_4
////属性4∷=array 直流模拟量配置
////直流模拟量配置∷=structure
////{
////  量程起始值  double-long，
////  量程结束值  double-long，
////  换算及单位  Scaler_Unit
////}
//#define LENper_F204_4     15
//#define NUMmax_F204_4     DCmax
//#define LENmax_F204_4     (2+(LENper_F204_4*NUMmax_F204_4))
//#define ADDR_F205_127     ADDR_F204_4+LENmax_F204_4
////方法127：修改开关属性（继电器号，开关属性）
////∷=structure
////{
////继电器号∷=OAD
////开关属性∷=enum{脉冲式（0），保持式（1）}，
////}
//#define LENper_F205_127     9
//#define NUMmax_F205_127     CRmax
//#define LENmax_F205_127     (LENper_F205_127*NUMmax_F205_127)

//#define ADDR_F206_4     ADDR_F205_127+LENmax_F205_127
//#define LENper_F206_4     10
//#define NUMmax_F206_4     12
//#define LENmax_F206_4     (2+(LENper_F206_4*NUMmax_F206_4))
const u8 InitData_F206_4[]=
{
//属性4∷=array 允许告警时段
	DataType_array,
	1,
//允许告警时段∷=structure
	DataType_structure,
	2,
//{
//  起始时间  Time，
	DataType_time,
	0,0,0,
//  结束时间  Time
	DataType_time,
	23,59,59,
//}
};

//#define ADDR_F207_127     ADDR_F206_4+LENmax_F206_4
////方法127：修改工作模式（路号，工作模式）
////∷=structure
////{
////路号∷=OAD
////端子功能∷=enum
////{
////     秒脉冲输出（0），
////     需量周期  （1），
////     时段投切  （2）
////}
////}
//#define LENper_F207_127     9
//#define NUMmax_F207_127     1
//#define LENmax_F207_127     (LENper_F207_127*NUMmax_F207_127)

//#define ADDR_F209_128     ADDR_F207_127+LENmax_F207_127
//#define LENper_F209_128     13
//#define NUMmax_F209_128     1
//#define LENmax_F209_128     (LENper_F209_128*NUMmax_F209_128)
const u8 InitData_F209_2[]=
{
//方法128：配置端口参数（端口号，通信参数）
//∷=structure
	DataType_structure,
	2,
//{
//端口号∷=OAD
	DataType_OAD,
	0xF2,0x09,0x80,0x01,
//端口参数   COMDCB，
	DataType_COMDCB,
	0x03,0x02,0x08,0x01,0x00,
//}
};
//#define ADDR_F209_6     ADDR_F207_127+LENmax_F207_127//载波/微功率无线接口
//属性6（从节点对象列表更新周期）∷= TI
//更新周期：指从节点对象列表、网络拓扑信息、多网信息等更新周期
//#define LENmax_F209_6     4
const u8 InitData_F209_6[]=
{
//TI∷=SEQUENCE
	DataType_TI,
//{
//   单位  ENUMERATED
//   {
//       秒      （0），
//       分      （1），
//       时      （2），
//       日      （3），
//       月      （4），
//       年      （5）
//    }，
	3,//日
//    间隔值  long-unsigned
	1>>8,1&0xff,
//}
};
//#define ADDR_F209_9     ADDR_F209_6+LENmax_F209_6//载波/微功率无线接口
//属性 9（宽带载波频段序号）∷ =unsigned
//#define LENmax_F209_9     2
const u8 InitData_F209_9[]=
{
	DataType_unsigned,
	0,//宽带载波频段：0：1.953～11.96MHz；1： 2.441～5.615 MHz
};

//#define ADDR_F20B_127     ADDR_F209_128+LENmax_F209_128
////方法127：配置端口（端口号，通信参数）
////∷=structure
////{
////端口号∷=OAD
////端口参数   COMDCB，
////}
//#define LENper_F20B_127     13
//#define NUMmax_F20B_127     1
//#define LENmax_F20B_127     (LENper_F20B_127*NUMmax_F20B_127)

//总加组
//#define ADDR_230x_2     ADDR_F002_8+LENmax_F002_8
//#define LENper_230x_2     (2+(15*8))
//#define LENmax_230x_2     (LENper_230x_2*NUMmax_TAG)
const u8 InitData_230x_2[]=
{
#if NUMmax_TAG<2
#error
#endif	
//总加配置表∷=array 总加组配置单元
	DataType_array,
	1,
//总加配置单元∷=structure
	DataType_structure,
	3,
//{
//参与总加的分路通信地址  TSA，
	DataType_TSA,
	0x07,0x05,0x10,0x00,0x00,0x00,0x00,0x01,
//总加标志	            enum{正向（0），反向（1）}，
	DataType_enum,
	0,
//运算符标志	            enum{加（0），减（1）}
	DataType_enum,
	0,
//}
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,
	
//总加配置表∷=array 总加组配置单元
	DataType_array,
	1,
//总加配置单元∷=structure
	DataType_structure,
	3,
//{
//参与总加的分路通信地址  TSA，
	DataType_TSA,
	0x07,0x05,0x00,0x00,0x00,0x00,0x00,0x02,
//总加标志	            enum{正向（0），反向（1）}，
	DataType_enum,
	0,
//运算符标志	            enum{加（0），减（1）}
	DataType_enum,
	0,
//}
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,
};

//#define ADDR_230x_13     ADDR_230x_2+LENmax_230x_2
//#define LENper_230x_13     2
//#define LENmax_230x_13     (LENper_230x_13*NUMmax_TAG)
const u8 InitData_230x_13[]=
{
#if NUMmax_TAG<2
#error
#endif
//总加组滑差时间周期	unsigned（单位：分）
	DataType_unsigned,
	1,
	
//总加组滑差时间周期	unsigned（单位：分）
	DataType_unsigned,
	1,
};

//#define ADDR_230x_14     ADDR_230x_13+LENmax_230x_13
//#define LENper_230x_14     3
//#define LENmax_230x_14     (LENper_230x_14*NUMmax_TAG)
const u8 InitData_230x_14[]=
{
#if NUMmax_TAG<2
#error
#endif	
//总加组功控轮次配置
//功控轮次标识位∷=bit-string（SIZE(8)）
	DataType_bit_string,
	8,
	0xC0,
//功控轮次标识位：bit0～bit7按顺序对位表示第1～第8轮次开关的受控设置，置“1”表示该轮次开关受控，置“0”表示不受控
	
//总加组功控轮次配置
//功控轮次标识位∷=bit-string（SIZE(8)）
	DataType_bit_string,
	8,
	0xC0,
//功控轮次标识位：bit0～bit7按顺序对位表示第1～第8轮次开关的受控设置，置“1”表示该轮次开关受控，置“0”表示不受控	
};

//#define ADDR_230x_15     ADDR_230x_14+LENmax_230x_14
//#define LENper_230x_15     3
//#define LENmax_230x_15     (LENper_230x_15*NUMmax_TAG)
const u8 InitData_230x_15[]=
{
#if NUMmax_TAG<2
#error
#endif	
//总加组电控轮次配置
//功控轮次标识位∷=bit-string（SIZE(8)）
	DataType_bit_string,
	8,
	0xC0,
//功控轮次标识位：bit0～bit7按顺序对位表示第1～第8轮次开关的受控设置，置“1”表示该轮次开关受控，置“0”表示不受控
	
//总加组电控轮次配置
//功控轮次标识位∷=bit-string（SIZE(8)）
	DataType_bit_string,
	8,
	0xC0,
//功控轮次标识位：bit0～bit7按顺序对位表示第1～第8轮次开关的受控设置，置“1”表示该轮次开关受控，置“0”表示不受控
	
};

//脉冲计量
#if NUMmax_IMPIN!=0
//#define ADDR_240x_2     ADDR_230x_15+LENmax_230x_15
//#define LENper_240x_2     18
//#define LENmax_240x_2     (LENper_240x_2*NUMmax_IMPIN)
const u8 InitData_240x_2[]=
{
#if NUMmax_IMPIN<2
#error
#endif	
//通信地址∷=octet-string
	DataType_octet_string,
	6,
	0x10,0x00,0x00,0x00,0x00,0x01,
	0,0,0,0,0,0,0,0,0,0,

//通信地址∷=octet-string
	DataType_octet_string,
	6,
	0x10,0x00,0x00,0x00,0x00,0x02,
	0,0,0,0,0,0,0,0,0,0,	
};

//#define ADDR_240x_3     ADDR_240x_2+LENmax_240x_2
//#define LENper_240x_3     8
//#define LENmax_240x_3     (LENper_240x_3*NUMmax_IMPIN)
const u8 InitData_240x_3[]=
{
#if NUMmax_IMPIN<2
#error
#endif	
	//互感器倍率∷=structure
	DataType_structure,
	2,
//{
//PT  long-unsigned，
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//CT  long-unsigned
	DataType_long_unsigned,
	(100>>8)&0xff,100&0xff,
//}
	
	//互感器倍率∷=structure
	DataType_structure,
	2,
//{
//PT  long-unsigned，
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//CT  long-unsigned
	DataType_long_unsigned,
	(100>>8)&0xff,100&0xff,
//}
};

//#define ADDR_240x_4     ADDR_240x_3+LENmax_240x_3
//#define LENper_240x_4     (2+12)
//#define LENmax_240x_4     (LENper_240x_4*NUMmax_IMPIN)
const u8 InitData_240x_4[]=
{
#if NUMmax_IMPIN<2
#error
#endif	
//脉冲配置∷=array 脉冲单元
	DataType_array,
	1,
//脉冲单元∷=structure
	DataType_structure,
	3,
//{
//脉冲输入端口号	OAD，
	DataType_OAD,
	0xF2,0x0A,0x02,0x01,
//脉冲属性		enum
//{
//	 正向有功（0），
//	 正向无功（1），
//	 反向有功（2），
//	 反向无功（3）
//}，
	DataType_enum,
	0,
//脉冲常数k     long-unsigned
	DataType_long_unsigned,
	(1000>>8)&0xff,1000&0xff,
//}

//脉冲配置∷=array 脉冲单元
	DataType_array,
	1,
//脉冲单元∷=structure
	DataType_structure,
	3,
//{
//脉冲输入端口号	OAD，
	DataType_OAD,
	0xF2,0x0A,0x02,0x02,
//脉冲属性		enum
//{
//	 正向有功（0），
//	 正向无功（1），
//	 反向有功（2），
//	 反向无功（3）
//}，
	DataType_enum,
	0,
//脉冲常数k     long-unsigned
	DataType_long_unsigned,
	(1000>>8)&0xff,1000&0xff,
//}
};
#endif





#endif
#endif

