
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"


#if ((Project/100)==3)//专变
#if (USER/100)==15//安微系



//#define ADDR_6000     ADDR_4204_3+LENmax_4204_3//采集档案配置表(初始化值0x00)
//#define LENper_6000     128
//#define NUMmax_6000     RMNmax
//#define LENmax_6000     (LENper_6000*NUMmax_6000)
const u8 InitData_6000_2_0[]=//采集档案配置0
{
	0,0,
};
const u8 InitData_6000_2_1[]=//采集档案配置1-交采
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,1,
//基本信息  Basic_object，
//	
//扩展信息  Extended_object，
//	
//附属信息  Annex_object
//}
//
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
	106,
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
//
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
	0,0,
};

const u8 InitData_6000_2_3[]=//采集档案配置3
{
	0,0,
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
	1,
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
 0,0
};
//专变三相表采集任务（用户类型1）
#define InitData_6012_2_1_
const u8 InitData_6012_2_1[]=//任务配置1，日采集任务
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
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,15,    //延时15分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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
const u8 InitData_6012_2_2[]=//任务配置2，日冻结曲线采集任务
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
	3,0,1,   //一天
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
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	2,0,1,    
//执行优先级  
	DataType_unsigned,
	4,
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
	2,
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

#define InitData_6012_2_3_
const u8 InitData_6012_2_3[]=//任务配置3，月采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	3,
//执行频率   TI，
	DataType_TI,
	4,0,1,   //1月
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
	3,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,15,    //延时15分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_4_
const u8 InitData_6012_2_4[]=//任务配置4，准实时采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	4,
//执行频率   TI，
	DataType_TI,
	1,0,15,   //15分钟
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
	4,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	0,0,0,    //不延时
//执行优先级  
	DataType_unsigned,
	3,
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
	2,
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

#define InitData_6012_2_15_
const u8 InitData_6012_2_15[]=//任务配置15，日上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	15,
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
	4,
//方案编号    unsigned，
	DataType_unsigned,
	15,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,30,    //延时30分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_16_
const u8 InitData_6012_2_16[]=//任务配置16，冻结曲线上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	16,
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
	4,
//方案编号    unsigned，
	DataType_unsigned,
	16,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,90,    //延时90分钟
//执行优先级  
	DataType_unsigned,
	4,
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
	2,
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

#define InitData_6012_2_17_
const u8 InitData_6012_2_17[]=//任务配置17，月上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	17,
//执行频率   TI，
	DataType_TI,
	4,0,1,   //1月
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	4,
//方案编号    unsigned，
	DataType_unsigned,
	17,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,30,    //延时30分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_18_
const u8 InitData_6012_2_18[]=//任务配置18，准实时上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	18,
//执行频率   TI，
	DataType_TI,
	1,0,15,   //15分钟
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	4,
//方案编号    unsigned，
	DataType_unsigned,
	18,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,5,    //延时5分钟
//执行优先级  
	DataType_unsigned,
	3,
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
	2,
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

//全部用户电表
#define InitData_6012_2_28_
const u8 InitData_6012_2_28[]=//用于判断电能表时钟超差事件
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	28,
//执行频率   TI，
	DataType_TI,
	3,0,1,//
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
	13,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,1,1,23,59,59,
//延时        TI，
	DataType_TI,
	0,0,0,
//执行优先级  
	DataType_unsigned,
	5,
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
	2,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	8,
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

//专变单表采集任务（用户类型2）
#define InitData_6012_2_5_
const u8 InitData_6012_2_5[]=//任务配置5，日采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	5,
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
	5,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,15,    //延时10分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_6_
const u8 InitData_6012_2_6[]=//任务配置6，日冻结曲线采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	6,
//执行频率   TI，
	DataType_TI,
	1,0,15,   //1天
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
	6,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,5,    //延时15分钟
//执行优先级  
	DataType_unsigned,
	4,
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
	2,
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

#define InitData_6012_2_19_
const u8 InitData_6012_2_19[]=//任务配置19，日上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	19,
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
	4,
//方案编号    unsigned，
	DataType_unsigned,
	19,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	2,0,4,    //延时4小时
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_20_
const u8 InitData_6012_2_20[]=//任务配置20，冻结曲线上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	20,
//执行频率   TI，
	DataType_TI,
	1,0,15,   //1天
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	4,
//方案编号    unsigned，
	DataType_unsigned,
	20,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,5,    //延时5个分钟
//执行优先级  
	DataType_unsigned,
	4,
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
	2,
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
//专变用户水表采集任务（用户类型3）
#define InitData_6012_2_33_
const u8 InitData_6012_2_33[]=//任务配置33，日采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	33,
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
	33,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,15,    //延时10分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_43_
const u8 InitData_6012_2_43[]=//任务配置43，日上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	43,
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
	4,
//方案编号    unsigned，
	DataType_unsigned,
	43,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	2,0,4,    //延时4小时
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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
//专变用户气表采集任务（用户类型4）
#define InitData_6012_2_34_
const u8 InitData_6012_2_34[]=//任务配置34，日采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	34,
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
	34,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	1,0,15,    //延时10分钟
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_44_
const u8 InitData_6012_2_44[]=//任务配置44，日上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	44,
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
	4,
//方案编号    unsigned，
	DataType_unsigned,
	44,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	2,0,4,    //延时4小时
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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
//专变用户热表采集任务（用户类型5）
#define InitData_6012_2_35_
const u8 InitData_6012_2_35[]=//任务配置35，日采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	35,
//执行频率   TI，
	DataType_TI,
	3,0,1,//1天
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
	35,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00,
//延时        TI，
	DataType_TI,
	1,0,15,
//执行优先级 
	DataType_unsigned,
	2,
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
	2,
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

#define InitData_6012_2_45_
const u8 InitData_6012_2_45[]=//任务配置45，日上报任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	45,
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
	4,
//方案编号    unsigned，
	DataType_unsigned,
	45,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,   
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00, 
//延时        TI，
	DataType_TI,
	2,0,4,    //延时4小时
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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
//专变用户电表采集任务（用户类型1）
#define InitData_6012_2_49_
const u8 InitData_6012_2_49[]=//任务配置49，日电压合格率采集任务
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	49,
//执行频率   TI，
	DataType_TI,
	1,0,1,//1分钟
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
	12,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,00,00,00,
//延时        TI，
	DataType_TI,
	0,0,0,
//执行优先级  
	DataType_unsigned,
	2,
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
	2,
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


/**********事件类采集任务**********/
#define InitData_6012_2_14_
const u8 InitData_6012_2_14[]=//二级三相事件采集任务（频率一日）
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	14,
//执行频率   TI，
	DataType_TI,
	3,0,1,//1日
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	2,
//方案编号    unsigned，
	DataType_unsigned,
	3,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,23,59,59,
//延时        TI，
	DataType_TI,
	1,0,10,//10分钟
//执行优先级  enum{首要（1），必要（2），需要（3），可能（4）}，
	DataType_unsigned,
	5,
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
	2,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	4,
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

#define InitData_6012_2_25_
const u8 InitData_6012_2_25[]=//二级单相事件采集任务（频率一日）
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	25,
//执行频率   TI，
	DataType_TI,
	3,0,1,//1日
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	2,
//方案编号    unsigned，
	DataType_unsigned,
	4,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,23,59,59,
//延时        TI，
	DataType_TI,
	1,0,10,//10分钟
//执行优先级  enum{首要（1），必要（2），需要（3），可能（4）}，
	DataType_unsigned,
	5,
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
	2,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	4,
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

#define InitData_6012_2_26_
const u8 InitData_6012_2_26[]=//三级三相事件采集任务（频率一月）
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	26,
//执行频率   TI，
	DataType_TI,
	4,0,1,//1月
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	2,
//方案编号    unsigned，
	DataType_unsigned,
	5,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,23,59,59,
//延时        TI，
	DataType_TI,
	1,0,10,//10分钟
//执行优先级  
	DataType_unsigned,
	5,
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
	2,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	4,
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

#define InitData_6012_2_27_
const u8 InitData_6012_2_27[]=//三级单相事件采集任务（频率一月）
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	27,
//执行频率   TI，
	DataType_TI,
	4,0,1,//1月
//方案类型   enum
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
	DataType_enum,
	2,
//方案编号    unsigned，
	DataType_unsigned,
	6,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,01,01,00,00,00,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,01,01,23,59,59,
//延时        TI，
	DataType_TI,
	1,0,10,//10分钟
//执行优先级
	DataType_unsigned,
	5,
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
	2,
//时段表  array 时段
	DataType_array,
	01,
//时段∷=structure
	DataType_structure,
	4,
//{
//起始小时  unsigned，
	DataType_unsigned,
	4,
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


#define InitData_6012_2_54_
const u8 InitData_6012_2_54[]=//停电五分钟之内电量数据
{
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	12,
//{
//任务ID    unsigned，
	DataType_unsigned,
	54,
//执行频率   TI，
	DataType_TI,
	1,0,5,//5分钟间隔
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
	54,
//开始时间    date_time_s，
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,1,1,23,59,59,
//延时        TI，
	DataType_TI,
	0,0,0,
//执行优先级  
	DataType_unsigned,
	4,
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
	2,
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
};

#if NUMmax_6012_2<(54+1)//最大抄表任务数
	#error
#endif



//#define ADDR_6014     ADDR_6012_2+LENmax_6012_2//普通采集方案
//#define LENper_6014     128
//#define NUMmax_6014     16
//#define LENmax_6014     (LENper_6014*NUMmax_6014)
#define InitData_6014_2_0_
const u8 InitData_6014_2_0[]=
{
	0,0,
};

//【01】普通采集方案（专变三相表采集任务，用户类型1）
#define InitData_6014_2_1_
const u8 InitData_6014_2_1[]=//日冻结采集方案
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
	(62>>8)&0xff,62&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	4,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	11,
	0x20,0x21,0x02,0x00,//1 冻结时标
	0x00,0x10,0x02,0x00,//2 正向有功电能
	0x00,0x11,0x02,0x00,//3	A 相正向有功
	0x00,0x12,0x02,0x00,//4	B 相正向有功
	0x00,0x13,0x02,0x00,//5	C 相正向有功
	0x00,0x20,0x02,0x00,//6 反向有功电能
	0x00,0x21,0x02,0x00,//7 A 相反向有功
	0x00,0x22,0x02,0x00,//8 B 相反向有功
	0x00,0x23,0x02,0x00,//9 C 相反向有功
	0x00,0x30,0x02,0x00,//10 组合无功1电能
	0x00,0x40,0x02,0x00,//11 组合无功2电能
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	4,
	0x00,0x50,0x02,0x00,//6 一象限无功电能
	0x00,0x60,0x02,0x00,//7 二象限无功电能
	0x00,0x70,0x02,0x00,//8 三象限无功电能
	0x00,0x80,0x02,0x00,//9 四象限无功电能	
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x10,0x10,0x02,0x00,//10 正向有功最大需量及发生时间
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	3,
	0x21,0x31,0x02,0x01,//11 A相电压合格率
	0x21,0x32,0x02,0x01,//12 B相电压合格率
	0x21,0x33,0x02,0x01,//13 C相电压合格率
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
/*存储时标选择 enum
{
未定义              （0），
任务开始时间        （1），
相对当日0点0分      （2），
相对上日23点59分    （3），
相对上日0点0分      （4），
相对当月1日0点0分   （5），
数据冻结时标        （6）
}
*/
	DataType_enum,
	3,
//}
/*采集方式数据格式如下：
采集类型	 采集内容	 表示
0	       NULL	     采集当前数据
1	       unsigned	 采集上第N次
2	NULL	按冻结时标采集
3	TI	按时标间隔采集
*/
};

#define InitData_6014_2_2_
const u8 InitData_6014_2_2[]=//15分钟冻结曲线采集方案
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
	(2976>>8)&0xff,2976&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
	DataType_structure,
	2,
//{
//  采集类型  unsigned，
	DataType_unsigned,
	3,
//  采集内容  Data
	DataType_TI,
	1,0,15,//15分钟
//}，
//记录列选择  array CSD，
	DataType_array,
	6,
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	1,
	0x20,0x21,0x02,0x00,// 冻结时标
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	3,
	0x20,0x00,0x02,0x00,// 电压
	0x20,0x01,0x02,0x00,// 电流
	0x20,0x01,0x04,0x00,// 零线电流（698表需要配置）
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	2,
	0x20,0x04,0x02,0x00,// 有功功率
	0x20,0x05,0x02,0x00,// 无功功率
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	1,
	0x20,0x0A,0x02,0x00,// 功率因素	
  DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	4,
	//0x20,0x21,0x02,0x00,// 冻结时标
	0x00,0x10,0x02,0x01,// 正向有功总电能
  0x00,0x20,0x02,0x01,// 反向有功总电能
  0x00,0x30,0x02,0x01,//组合无功1 总电能
  0x00,0x40,0x02,0x01,//组合无功 2总电能
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	4,
	0x00,0x50,0x02,0x01,// 一象限无功总电能
	0x00,0x60,0x02,0x01,// 二象限无功总电能
	0x00,0x70,0x02,0x01,// 三象限无功总电能
	0x00,0x80,0x02,0x01,// 四象限无功总电能	
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
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
	6,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};
#define InitData_6014_2_3_
const u8 InitData_6014_2_3[]=//月冻结采集方案
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
	(12>>8)&0xff,12&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	3,
	DataType_CSD,
	0x01,0x50,0x06,0x02,0x00,
	11,
	0x20,0x21,0x02,0x00,// 冻结时标
  0x00,0x10,0x02,0x00,// 月正向有功
	0x00,0x11,0x02,0x00,// 月A 相正向有功
	0x00,0x12,0x02,0x00,// 月B 相正向有功
	0x00,0x13,0x02,0x00,// 月C 相正向有功
	0x00,0x20,0x02,0x00,// 月反向有功
	0x00,0x21,0x02,0x00,// 月A 相反向有功
	0x00,0x22,0x02,0x00,// 月B 相反向有功
	0x00,0x23,0x02,0x00,// 月C 相反向有功
  0x00,0x30,0x02,0x00,// 组合无功1总电能
	0x00,0x40,0x02,0x00,// 组合无功2总电能
	DataType_CSD,
	0x01,0x50,0x06,0x02,0x00,
	4,
	0x00,0x50,0x02,0x00,// 第一象限无功电能
	0x00,0x60,0x02,0x00,// 第二象限无功电能
  0x00,0x70,0x02,0x00,// 第三象限无功电能
	0x00,0x80,0x02,0x00,// 第四象限无功电能
	DataType_CSD,
	0x01,0x50,0x06,0x02,0x00,
	4,
  0x10,0x10,0x02,0x00,// 正向有功最大需量及发生时间
  0x21,0x31,0x02,0x02,// A相电压合格率
	0x21,0x32,0x02,0x02,// B相电压合格率
	0x21,0x33,0x02,0x02,// C相电压合格率  
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
/*存储时标选择 enum
{
未定义              （0），
任务开始时间        （1），
相对当日0点0分      （2），
相对上日23点59分    （3），
相对上日0点0分      （4），
相对当月1日0点0分   （5），
数据冻结时标        （6）
}
*/
	DataType_enum,
	3,
//}
/*采集方式数据格式如下：
采集类型	 采集内容	 表示
0	       NULL	     采集当前数据
1	       unsigned	 采集上第N次
2	NULL	按冻结时标采集
3	TI	按时标间隔采集
*/
};

#define InitData_6014_2_4_
const u8 InitData_6014_2_4[]=//准实时采集方案
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	4,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(96>>8)&0xff,96&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	15,
	DataType_CSD,
	0x00,0x40,0x00,0x02,0x00,// 日期时间
	DataType_CSD,
	0x00,0x20,0x00,0x02,0x00,// 电压
	DataType_CSD,
	0x00,0x20,0x01,0x02,0x00,// 电流
	DataType_CSD,
	0x00,0x20,0x04,0x02,0x00,// 有功功率
	DataType_CSD,
	0x00,0x20,0x05,0x02,0x00,// 无功功率
	DataType_CSD,
	0x00,0x20,0x06,0x02,0x00,// 视在功率
	DataType_CSD,
	0x00,0x20,0x0a,0x02,0x00,// 功率因素
	DataType_CSD,
	0x00,0x20,0x01,0x04,0x00,// 零线电流（698.45表需要配置）
	DataType_CSD,
	0x00,0x00,0x10,0x02,0x00,// 正向有功电能
	DataType_CSD,
	0x00,0x00,0x20,0x02,0x00,// 反向有功电能
	DataType_CSD,
	0x00,0x00,0x30,0x02,0x00,// 组合无功1电能
	DataType_CSD,
	0x00,0x00,0x40,0x02,0x00,// 组合无功2电能
	DataType_CSD,
	0x00,0x00,0x11,0x02,0x00,// A相正向有功
	DataType_CSD,
	0x00,0x00,0x12,0x02,0x00,// B相正向有功
	DataType_CSD,
	0x00,0x00,0x13,0x02,0x00,// C相正向有功
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
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
	1,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};


#define InitData_6014_2_12_
const u8 InitData_6014_2_12[]=//日电压合格率采集
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	12,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(1440>>8)&0xff,1440&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	1,
	DataType_CSD,
	0x00,0x20,0x00,0x02,0x00,// 电压
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
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
	1,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};
//专变用户单相表采集任务（用户类型2）
#define InitData_6014_2_5_
const u8 InitData_6014_2_5[]=//日冻结采集方案
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	5,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(62>>8)&0xff,62&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	1,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	3,
	0x20,0x21,0x02,0x00,//1 冻结时标
	0x00,0x10,0x02,0x00,//2 正向有功电能
	0x00,0x20,0x02,0x00,//3 反向有功电能
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,2,
/*存储时标选择 enum
{
未定义              （0），
任务开始时间        （1），
相对当日0点0分      （2），
相对上日23点59分    （3），
相对上日0点0分      （4），
相对当月1日0点0分   （5），
数据冻结时标        （6）
}
*/
	DataType_enum,
	3,
//}
/*采集方式数据格式如下：
采集类型	 采集内容	 表示
0	       NULL	     采集当前数据
1	       unsigned	 采集上第N次
2	NULL	按冻结时标采集
3	TI	按时标间隔采集
*/
};

#define InitData_6014_2_6_
const u8 InitData_6014_2_6[]=//15分钟冻结曲线采集方案
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	6,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(2976>>8)&0xff,2976&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
	DataType_structure,
	2,
//{
//  采集类型  unsigned，
	DataType_unsigned,
	3,
//  采集内容  Data
	DataType_TI,
	1,0,15,//15分钟
//}，
//记录列选择  array CSD，
	DataType_array,
	1,
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	8,
	0x20,0x21,0x02,0x00,// 冻结时标
	0x20,0x00,0x02,0x00,// 电压
	0x20,0x01,0x02,0x00,// 电流
	0x20,0x04,0x02,0x00,// 总有功功率
  0x20,0x0A,0x02,0x00,// 功率因数
	0x20,0x01,0x04,0x00,// 零线电流（698表需要配置）
  0x00,0x10,0x02,0x01,// 正向有功总电能
  0x00,0x20,0x02,0x01,// 反向有功总电能
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,2,
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
	6,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};


//专变用户水表采集任务（用户类型3）
#define InitData_6014_2_33_
const u8 InitData_6014_2_33[]=//日冻结采集方案
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	33,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(62>>8)&0xff,62&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	1,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x25,0x00,0x02,0x00,// 累计水（热）流量
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,3,
/*存储时标选择 enum
{
未定义              （0），
任务开始时间        （1），
相对当日0点0分      （2），
相对上日23点59分    （3），
相对上日0点0分      （4），
相对当月1日0点0分   （5），
数据冻结时标        （6）
}
*/
	DataType_enum,
	3,
//}
/*采集方式数据格式如下：
采集类型	 采集内容	 表示
0	       NULL	     采集当前数据
1	       unsigned	 采集上第N次
2	NULL	按冻结时标采集
3	TI	按时标间隔采集
*/
};


//专变用户气表采集任务（用户类型4）
#define InitData_6014_2_34_
const u8 InitData_6014_2_34[]=//日冻结采集方案
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	34,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(62>>8)&0xff,62&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	1,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x25,0x01,0x02,0x00,// 累计气流量
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,4,
/*存储时标选择 enum
{
未定义              （0），
任务开始时间        （1），
相对当日0点0分      （2），
相对上日23点59分    （3），
相对上日0点0分      （4），
相对当月1日0点0分   （5），
数据冻结时标        （6）
}
*/
	DataType_enum,
	3,
//}
/*采集方式数据格式如下：
采集类型	 采集内容	 表示
0	       NULL	     采集当前数据
1	       unsigned	 采集上第N次
2	NULL	按冻结时标采集
3	TI	按时标间隔采集
*/
};


//专变用户热表采集任务（用户类型5）
#define InitData_6014_2_35_
const u8 InitData_6014_2_35[]=//日冻结采集方案
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	35,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(62>>8)&0xff,62&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	1,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x25,0x02,0x02,0x00,// 累计热量
//电能表集合  MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,5,
/*存储时标选择 enum
{
未定义              （0），
任务开始时间        （1），
相对当日0点0分      （2），
相对上日23点59分    （3），
相对上日0点0分      （4），
相对当月1日0点0分   （5），
数据冻结时标        （6）
}
*/
	DataType_enum,
	3,
//}
/*采集方式数据格式如下：
采集类型	 采集内容	 表示
0	       NULL	     采集当前数据
1	       unsigned	 采集上第N次
2	NULL	按冻结时标采集
3	TI	按时标间隔采集
*/
};


#define InitData_6014_2_13_
const u8 InitData_6014_2_13[]=//用于判定电能表超差事件，准实时数据
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	13,
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,1&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	01,
	DataType_CSD,
	0x00,0x40,0x00,0x02,0x00,// 日期时间
//电能表集合  MS，
	DataType_MS,
	0x01,	//全部用户地址	[1]  SEQUENCE OF unsigned
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
	1,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};

#define InitData_6014_2_54_
const u8 InitData_6014_2_54[]=//停电五分钟之内电量数据
{
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	6,
//{
//方案编号  unsigned，
	DataType_unsigned,
	54,	
//存储深度  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,1&0xff,
/*采集方式  structure
 0-->当前数据; 1-->上第N次;2-->冻结时标
 3-->冻结时标间隔;4-->补抄
*/
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
	05,
	DataType_CSD,
	0x00,0x00,0x10,0x02,0x00,// 正向有功总电能
	DataType_CSD,
	0x00,0x00,0x20,0x02,0x00,// 反向有功总电能
	DataType_CSD,
	0x00,0x10,0x10,0x02,0x00,// 正向有功最大需量及发生时间
	DataType_CSD,
	0x00,0x00,0x30,0x02,0x00,// 组合无功1总电能
	DataType_CSD,
	0x00,0x00,0x40,0x02,0x00,// 组合无功2总电能
//电能表集合  MS，
	DataType_MS,
	0x04,	//一组配置序号    [4]   SEQUENCE OF long-unsigned
	6,
	(3>>8)&0xff,3&0xff,//用户类型：一组配置序号 3
	(4>>8)&0xff,4&0xff,//用户类型：一组配置序号 4
	(5>>8)&0xff,5&0xff,//用户类型：一组配置序号 5
	(6>>8)&0xff,6&0xff,//用户类型：一组配置序号 6
	(7>>8)&0xff,7&0xff,//用户类型：一组配置序号 7
	(26>>8)&0xff,26&0xff,//用户类型：一组配置序号 7
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
	1,
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集
};

#if NUMmax_6014<(54+1)//最大普通采集方案数
	#error
#endif


//#define ADDR_6016     ADDR_6014+LENmax_6014//事件采集方案集(初始化值0x00)
//#define LENper_6016     768
//#define NUMmax_6016     64
//#define LENmax_6016     (LENper_6016*NUMmax_6016)
#define InitData_6016_2_0_
const u8 InitData_6016_2_0[]=
{
	0,0,
};

#define InitData_6016_2_1_
const u8 InitData_6016_2_1[]=//1 级三相表事件采集方案，根据通知采集指定事件数据
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	1,	
//采集方式        structure
	DataType_structure,
	2,	
//{
//  采集类型      unsigned，
	DataType_unsigned,
	2,
//  采集内容      Data
	DataType_array,
	4,
	DataType_ROAD,
	0x30,0x1B,0x02,0x00,//1 开盖事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x2A,0x02,0x00, //2 恒定磁场干扰事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x13,0x02,0x00, //3 电表清零事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x11,0x02,0x00, //4电能表掉电
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
//}
//电能表集合      MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	5,1,10,16,26,36,//2,1,26,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
	0x01,
//存储深度        long-unsigned
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,	
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3 NULL  直接存储通知事件数据
};

#define InitData_6016_2_2_
const u8 InitData_6016_2_2[]=//1 级单相表事件采集方案
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	2,	
//采集方式        structure
	DataType_structure,
	2,	
//{
//  采集类型      unsigned，
	DataType_unsigned,
	0x02,
//  采集内容      Data
	DataType_array,
	3,
	DataType_ROAD,
	0x30,0x1B,0x02,0x00,//1 开盖事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x13,0x02,0x00, //2 电表清零事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x11,0x02,0x00, //3电能表掉电
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
//}
//电能表集合      MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	5,2,12,13,14,37,//1,2,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
	0x01,
//存储深度        long-unsigned
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,	
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3 NULL  直接存储通知事件数据
};

#define InitData_6016_2_3_
const u8 InitData_6016_2_3[]=//2 级三相事件日采集任务
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	3,	
//采集方式        structure
	DataType_structure,
	2,	
//{
//  采集类型      unsigned，
	DataType_unsigned,
	0,
//  采集内容      Data
	DataType_array,
	8,
	DataType_ROAD,
	0x30,0x1B,0x02,0x00,//1 开表盖事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x2A,0x02,0x00, //2 恒定磁场干扰事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x13,0x02,0x00, //3 电表清零事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x11,0x02,0x00, //4电能表掉电
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x2C,0x02,0x00, //5 电源异常事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x1C,0x02,0x00, //6 开端纽盖事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x0F,0x02,0x00, //7 电压逆相序事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x2B,0x02,0x00, //8 负荷开关误动或拒动事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
//}
//电能表集合      MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	5,1,10,16,26,36,//2,1,26,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
	0x01,
//存储深度        long-unsigned
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,	
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3 NULL  直接存储通知事件数据
};

#define InitData_6016_2_4_
const u8 InitData_6016_2_4[]=//2 级单相事件日采集任务
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	4,	
//采集方式        structure
	DataType_structure,
	2,	
//{
//  采集类型      unsigned，
	DataType_unsigned,
	0,
//  采集内容      Data
	DataType_array,
	5,
	DataType_ROAD,
	0x30,0x1B,0x02,0x00,//1 开表盖事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x13,0x02,0x00, //2 电表清零事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x11,0x02,0x00, //3电能表掉电
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x2C,0x02,0x00, //4 电源异常事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  DataType_ROAD,
	0x30,0x2B,0x02,0x00, //5 负荷开关误动或拒动事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
  
//}
//电能表集合      MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	5,2,12,13,14,37,//1,2,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
	0x01,
//存储深度        long-unsigned
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,	
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3 NULL  直接存储通知事件数据
};

#define InitData_6016_2_5_
const u8 InitData_6016_2_5[]=//3 级三相事件月采集任务
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	5,	
//采集方式        structure
	DataType_structure,
	2,	
//{
//  采集类型      unsigned，
	DataType_unsigned,
	0,
//  采集内容      Data
	DataType_array,
	4,
	DataType_ROAD,
	0x30,0x16,0x02,0x00,// 1电能表校时事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x12,0x02,0x00,// 2编程事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x14,0x02,0x00,// 3需量清零
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x15,0x02,0x00,// 4事件清零
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
//}
//电能表集合      MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	5,1,10,16,26,36,//2,1,26,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
	0x01,
//存储深度        long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,12&0xff,	
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3 NULL  直接存储通知事件数据
};

#define InitData_6016_2_6_
const u8 InitData_6016_2_6[]=//3 级单相事件月采集任务
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	6,	
//采集方式        structure
	DataType_structure,
	2,	
//{
//  采集类型      unsigned，
	DataType_unsigned,
	0,
//  采集内容      Data
	DataType_array,
	3,
  DataType_ROAD,
	0x30,0x16,0x02,0x00,// 1电能表校时事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x12,0x02,0x00,// 2编程事件
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	DataType_ROAD,
	0x30,0x15,0x02,0x00,// 3事件清零
	0x03,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1E,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
//}
//电能表集合      MS，
	DataType_MS,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	5,2,12,13,14,37,//1,2,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
	0x01,
//存储深度        long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,12&0xff,	
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3 NULL  直接存储通知事件数据
};

#if NUMmax_6016<7//最大事件采集方案数
	#error
#endif


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
#define InitData_601C_2_0_
const u8 InitData_601C_2_0[]=
{
	0,0,
};
//专变三相表采集任务（用户类型1）

#define InitData_601C_2_15_
const u8 InitData_601C_2_15[]=//日冻结上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	15,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	5,
	0x00,0x20,0x2A,0x02,0x00,//目标服务器地址
  0x00,0x60,0x40,0x02,0x00,// 采集启动时间
  0x00,0x60,0x41,0x02,0x00,// 采集成功时间
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
  0x01,0x50,0x04,0x02,0x00,
	19,
	0x20,0x21,0x02,0x00,// 冻结时标
	0x00,0x10,0x02,0x00,// 正向有功电能
	0x00,0x11,0x02,0x00,// A 相正向有功
	0x00,0x12,0x02,0x00,// B 相正向有功
	0x00,0x13,0x02,0x00,// C 相正向有功
	0x00,0x20,0x02,0x00,// 反向有功电能
	0x00,0x21,0x02,0x00,// A 相反向有功
	0x00,0x22,0x02,0x00,// B 相反向有功
	0x00,0x23,0x02,0x00,// C 相反向有功
	0x00,0x30,0x02,0x00,// 组合无功1电能
	0x00,0x40,0x02,0x00,// 组合无功2电能
	0x00,0x50,0x02,0x00,// 第一象限无功电能
	0x00,0x60,0x02,0x00,// 第二象限无功电能
	0x00,0x70,0x02,0x00,// 第三象限无功电能
	0x00,0x80,0x02,0x00,// 第四象限无功电能
	0x10,0x10,0x02,0x00,// 正向有功最大需量及发生时间
	0x21,0x31,0x02,0x01,// A相电压合格率
	0x21,0x32,0x02,0x01,// B相电压合格率
	0x21,0x33,0x02,0x01,// C相电压合格率
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
//时间间隔
	3,0,1,
//一组用户类型
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
};

#define InitData_601C_2_16_
const u8 InitData_601C_2_16[]=//分钟冻结曲线上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	16,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	5,
	0x00,0x20,0x2A,0x02,0x00,//目标服务器地址
  0x00,0x60,0x40,0x02,0x00,// 采集启动时间
  0x00,0x60,0x41,0x02,0x00,// 采集成功时间
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x01,0x50,0x02,0x02,0x00,
	15,
	0x20,0x21,0x02,0x00,// 冻结时标
	0x20,0x00,0x02,0x00,// 电压
	0x20,0x01,0x02,0x00,// 电流
	0x20,0x04,0x02,0x00,// 有功功率
	0x20,0x05,0x02,0x00,// 无功功率
	0x20,0x0A,0x02,0x00,// 功率因素
	0x20,0x01,0x04,0x00,// 零线电流
	0x00,0x10,0x02,0x00,// 正向有功总电能
	0x00,0x20,0x02,0x01,// 反向有功总电能
  0x00,0x30,0x02,0x01,// 组合无功1 总电能
  0x00,0x40,0x02,0x01,// 组合无功2 总电能
	0x00,0x50,0x02,0x01,// 第一象限无功总电能
	0x00,0x60,0x02,0x01,// 第二象限无功总电能
	0x00,0x70,0x02,0x01,// 第三象限无功总电能
	0x00,0x80,0x02,0x01,// 第四象限无功总电能
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
//时间间隔   
	1,0,15,
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26
};

#define InitData_601C_2_17_
const u8 InitData_601C_2_17[]=//月冻结上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	17,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	5,
  0x00,0x20,0x2A,0x02,0x00,//目标服务器地址
  0x00,0x60,0x40,0x02,0x00,// 采集启动时间
  0x00,0x60,0x41,0x02,0x00,// 采集成功时间
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
  0x01,0x50,0x06,0x02,0x00,
	13,
	0x20,0x21,0x02,0x00,// 冻结时标
  0x00,0x10,0x02,0x00,// 月正向有功
	0x00,0x20,0x02,0x00,// 月反向有功
  0x00,0x30,0x02,0x00,// 组合无功1总电能
	0x00,0x40,0x02,0x00,// 组合无功2总电能
  0x00,0x50,0x02,0x00,// 第一象限无功电能
	0x00,0x60,0x02,0x00,// 第二象限无功电能
  0x00,0x70,0x02,0x00,// 第三象限无功电能
	0x00,0x80,0x02,0x00,// 第四象限无功电能
  0x10,0x10,0x02,0x00,// 正向有功最大需量及发生时间
  0x21,0x31,0x02,0x02,// A相电压合格率
	0x21,0x32,0x02,0x02,// B相电压合格率
	0x21,0x33,0x02,0x02,// C相电压合格率  
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	4,0,1,//时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
};

#define InitData_601C_2_18_
const u8 InitData_601C_2_18[]=//准实时数据上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	18,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	19,
	0x00,0x20,0x2A,0x02,0x00,//目标服务器地址
  0x00,0x60,0x40,0x02,0x00,// 采集启动时间
  0x00,0x60,0x41,0x02,0x00,// 采集成功时间
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x00,0x40,0x00,0x02,0x00,// 日期时间
	0x00,0x20,0x00,0x02,0x00,// 电压
	0x00,0x20,0x01,0x02,0x00,// 电流
	0x00,0x20,0x04,0x02,0x00,// 有功功率
	0x00,0x20,0x05,0x02,0x00,// 无功功率
	0x00,0x20,0x06,0x02,0x00,// 视在功率
	0x00,0x20,0x0a,0x02,0x00,// 功率因素
	0x00,0x20,0x01,0x04,0x00,// 零线电流（698.45表需要配置）
	0x00,0x00,0x10,0x02,0x00,// 正向有功电能
	0x00,0x00,0x20,0x02,0x00,// 反向有功电能
	0x00,0x00,0x30,0x02,0x00,// 组合无功1电能
	0x00,0x00,0x40,0x02,0x00,// 组合无功2电能
	0x00,0x00,0x11,0x02,0x00,// A相正向有功
	0x00,0x00,0x12,0x02,0x00,// B相正向有功
	0x00,0x00,0x13,0x02,0x00,// C相正向有功
  DataType_RSD,
  7,
//开始时间    date_time_s，
  (2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	1,0,15,//时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	2,1,26,
};


//专变单表采集任务（用户类型2）

#define InitData_601C_2_19_
const u8 InitData_601C_2_19[]=//日冻结上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	19,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	5,
	0x00,0x20,0x2A,0x02,0x00,//目标服务器地址
  0x00,0x60,0x40,0x02,0x00,// 采集启动时间
  0x00,0x60,0x41,0x02,0x00,// 采集成功时间
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x01,0x50,0x04,0x02,0x00,
	3,
	0x20,0x21,0x02,0x00,// 冻结时标
	0x00,0x10,0x02,0x00,// 正向有功电能
	0x00,0x20,0x02,0x00,// 反向有功电能
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	3,0,1,//时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,2,
};

#define InitData_601C_2_20_
const u8 InitData_601C_2_20[]=//分钟冻结曲线上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	20,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	5,
	0x00,0x20,0x2A,0x02,0x00,//目标服务器地址
  0x00,0x60,0x40,0x02,0x00,// 采集启动时间
  0x00,0x60,0x41,0x02,0x00,// 采集成功时间
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x01,0x50,0x02,0x02,0x00,
	8,
	0x20,0x21,0x02,0x00,// 冻结时标
	0x20,0x00,0x02,0x0,// 电压
	0x20,0x01,0x02,0x0,// 电流
	0x20,0x04,0x02,0x00,// 有功功率
  0x20,0x0A,0x02,0x00,// 功率因素
	0x20,0x01,0x04,0x00,// 零线电流
  0x00,0x10,0x02,0x01,// 正向有功电能
  0x00,0x20,0x02,0x01,// 正向有功电能
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	1,0,15,  //时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,2,
};
//专变水表采集任务（用户类型3）

#define InitData_601C_2_43_
const u8 InitData_601C_2_43[]=//水表日冻结上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	43,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	3,
	0x00,0x20,0x2A,0x02,0x00,
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x25,0x00,0x02,0x00,// 水量
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	3,0,1,  //时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,3,
};
//专变气表采集任务（用户类型4）


#define InitData_601C_2_44_
const u8 InitData_601C_2_44[]=//气表日冻结上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	44,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	3,
	0x00,0x20,0x2A,0x02,0x00,
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x25,0x01,0x02,0x00,   // 气表
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	3,0,1,   //时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,4,
};
//专变热表采集任务（用户类型5）

#define InitData_601C_2_45_
const u8 InitData_601C_2_45[]=//热表日冻结上报
{
//属性2（事件采集方案Event acq plan）∷=structure
	DataType_structure,
	5,	
//{
//方案编号        unsigned，
	DataType_unsigned,
	45,
//上报通道          array OAD，
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x00,0x00,
    DataType_OAD,
	0x45,0x10,0x00,0x00,
//上报响应超时时间  TI，
	DataType_TI,
	0,0,60,
//最大上报次数      unsigned，
	DataType_unsigned,
	3,
//上报内容          structure
//{
//  类型  unsigned，0     /    1
//  数据  Data     OAD    /  ROAD
//}
	DataType_structure,
	2,
	DataType_unsigned,
	1,
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//  记录型对象属性描述符  RCSD，
//  行选择                RSD
//}
	DataType_structure,
	3,
	DataType_OAD,
	0x60,0x12,0x03,0x00,
	DataType_RCSD,
	3,
	0x00,0x20,0x2A,0x02,0x00,
	0x00,0x60,0x42,0x02,0x00,// 采集存储时标
	0x01,0x50,0x04,0x02,0x00,
	1,
	0x25,0x02,0x02,0x00,   // 热表
	DataType_RSD,
	7,
//开始时间    date_time_s，
	(2000>>8)&0xff,2000&0xff,1,1,0,0,0,
//结束时间    date_time_s，
	(2099>>8)&0xff,2099&0xff,1,1,0,0,0,
	3,0,1,  //时间间隔
	0x02,	//一组用户类型	[2]  SEQUENCE OF unsigned
	1,5,
};

#if NUMmax_601C<20//最大上报采集方案数
	#error
#endif

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
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
////}
	0,0,
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
	0,0,0,
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

