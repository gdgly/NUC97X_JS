





__align(4) const u8 DL698_Attribute_6000_2[]=
{
	23,//类型列表数
//属性2（Acquisition document definition）∷=structure
	DataType_structure,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
//基本信息  Basic_object，
//Basic_object∷=structure
	DataType_structure,
//{
//通信地址  TSA，
	DataType_TSA,
//波特率    enum
	DataType_enum,
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
//规约类型  enum
	DataType_enum,
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
//端口      OAD，
	DataType_OAD,
//通信密码  octet-string，
	DataType_octet_string,
//费率个数  unsigned，
	DataType_unsigned,
//用户类型  unsigned，
	DataType_unsigned,
//接线方式  enum
	DataType_enum,
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
//}
//扩展信息  Extended_object，
//Extended_object∷=structure
	DataType_structure,
//{
//采集器地址  TSA，
	DataType_TSA,
//资产号      octet-string，
	DataType_octet_string,
//PT          long-unsigned，
	DataType_long_unsigned,
//CT          long-unsigned
	DataType_long_unsigned,
//}
//附属信息  Annex_object
//Annex_object∷=array structure
	DataType_array,
	3,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_structure,
//{
//对象属性描述  OAD，
	DataType_OAD,
//属性值        Data
	DataType_Data,
//}
//}
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_0[]=//采集档案配置0
{
	0x60000200,//OAD
  ADDR_6000,//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_0,//初始化原始数据指针
	sizeof(InitData_6000_2_0),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_1[]=//采集档案配置1
{
	0x60000200,//OAD
  ADDR_6000+(1*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_1,//初始化原始数据指针
	sizeof(InitData_6000_2_1),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_2[]=//采集档案配置2
{
	0x60000200,//OAD
  ADDR_6000+(2*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_2,//初始化原始数据指针
	sizeof(InitData_6000_2_2),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_3[]=//采集档案配置3
{
	0x60000200,//OAD
  ADDR_6000+(3*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_3,//初始化原始数据指针
	sizeof(InitData_6000_2_3),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_4[]=//采集档案配置4
{
	0x60000200,//OAD
  ADDR_6000+(4*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_4,//初始化原始数据指针
	sizeof(InitData_6000_2_4),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_5[]=//采集档案配置5
{
	0x60000200,//OAD
  ADDR_6000+(5*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_5,//初始化原始数据指针
	sizeof(InitData_6000_2_5),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_6[]=//采集档案配置6
{
	0x60000200,//OAD
  ADDR_6000+(6*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_6,//初始化原始数据指针
	sizeof(InitData_6000_2_6),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_7[]=//采集档案配置7
{
	0x60000200,//OAD
  ADDR_6000+(7*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_7,//初始化原始数据指针
	sizeof(InitData_6000_2_7),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_8[]=//采集档案配置8
{
	0x60000200,//OAD
  ADDR_6000+(8*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_8,//初始化原始数据指针
	sizeof(InitData_6000_2_8),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_9[]=//采集档案配置9
{
	0x60000200,//OAD
  ADDR_6000+(9*LENper_6000),//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	(u32*)InitData_6000_2_9,//初始化原始数据指针
	sizeof(InitData_6000_2_9),//初始化原始数据长度
};




__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_3[]=//采集档案配置-当前元素个数
{
	0x60000300,//OAD
  ADDR_6000,//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_4[]=//采集档案配置-最大元素个数
{
	0x60000400,//OAD
  ADDR_6000,//数据地址
	LENper_6000,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6000,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6000_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_2[]=//所有搜表结果(仅使库中OAD有效)
{
	0x60020200,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_3[]=//所有搜表结果.当前元素个数(仅使库中OAD有效)
{
	0x60020300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_4[]=//所有搜表结果.最大元素个数(仅使库中OAD有效)
{
	0x60020400,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_5[]=//跨台区搜表结果(仅使库中OAD有效)
{
	0x60020500,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_6[]=//所有搜表结果记录数(仅使库中OAD有效)
{
	0x60020600,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_7[]=//跨台区搜表结果记录数(仅使库中OAD有效)
{
	0x60020700,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};





__align(4) const u8 DL698_Attribute_6002_8[]=
{
	5,//类型列表数
//属性8∷=structure
	DataType_structure,
//{
//是否启用每天周期搜表   bool，
	DataType_bool,
//自动更新采集档案       bool，
	DataType_bool,
//是否产生搜表相关事件   bool，
	DataType_bool,
//清空搜表结果选项       enum
	DataType_enum,
//{
//不清空             （0），
//每天周期搜表前清空 （1），
//每次搜表前清空     （2）
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_8[]=//搜表参数
{
	0x60020800,//OAD
  ADDR_6002_8,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6002_8,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6002_8,//数据类型列表指针
	(u32*)InitData_6002_8,//初始化原始数据指针
	sizeof(InitData_6002_8),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_6002_9[]=
{
	5,//类型列表数
//属性9（每天周期搜表参数配置）∷=array 定时搜表参数
	DataType_array,
	3,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//定时搜表参数∷=structure
	DataType_structure,
//{
//   开始时间         time，
	DataType_time,
//   搜表时长（min）  long-unsigned
	DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_9[]=//每天周期搜表参数
{
	0x60020900,//OAD
  ADDR_6002_9,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6002_9,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6002_9,//数据类型列表指针
	(u32*)InitData_6002_9,//初始化原始数据指针
	sizeof(InitData_6002_9),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_6002_10[]=
{
	1,//类型列表数
//属性10∷=enum
	DataType_enum,
//{
//空闲（0），搜表中（1） 
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6002_10[]=//搜表状态
{
	0x60020A00,//OAD
  ADDR_6002_10,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6002_10,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6002_10,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_6012_2[]=
{
	21,//类型列表数
//属性2（任务配置单元）∷=structure
	DataType_structure,
//{
//任务ID    unsigned，
	DataType_unsigned,
//执行频率   TI，
	DataType_TI,
//方案类型   enum
	DataType_enum,
//{
//普通采集方案 （1），   事件采集方案 （2），
//透明方案     （3），   上报方案     （4），
//脚本方案     （5）
//}，
//方案编号    unsigned，
	DataType_unsigned,
//开始时间    date_time_s，
	DataType_date_time_s,
//结束时间    date_time_s，
	DataType_date_time_s,
//延时        TI，
	DataType_TI,
//执行优先级  enum{首要（1），必要（2），需要（3），可能（4）}，
	DataType_unsigned,
//状态        enum{正常（1），停用（2）}，
	DataType_enum,
//任务开始前脚本id   long-unsigned，
	DataType_long_unsigned,
//任务完成后脚本id   long-unsigned，
	DataType_long_unsigned,
//任务运行时段       structure，
	DataType_structure,
//}
//当方案类型为脚本时，方案编号为脚本id
//任务运行时段∷=structrue
//{
//   类型  enum
	DataType_enum,
//{
//       前闭后开    （0），
//       前开后闭    （1），
//       前闭后闭    （2），
//       前开后开    （3）
//}，
//时段表  array 时段
	DataType_array,
	5,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//}
//时段∷=structure
	DataType_structure,
//{
//起始小时  unsigned，
	DataType_unsigned,
//起始分钟  unsigned，
	DataType_unsigned,
//结束小时  unsigned，
	DataType_unsigned,
//结束分钟  unsigned
	DataType_unsigned,
//}
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_0[]=//任务配置表0
{
	0x60120200,//OAD
  ADDR_6012_2,//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_0,//初始化原始数据指针
	sizeof(InitData_6012_2_0),//初始化原始数据长度
};
#ifdef InitData_6012_2_1_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_1[]=//任务配置表1
{
	0x60120200,//OAD
  ADDR_6012_2+(1*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_1,//初始化原始数据指针
	sizeof(InitData_6012_2_1),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_2_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_2[]=//任务配置表2
{
	0x60120200,//OAD
  ADDR_6012_2+(2*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_2,//初始化原始数据指针
	sizeof(InitData_6012_2_2),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_3_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_3[]=//任务配置表3
{
	0x60120200,//OAD
  ADDR_6012_2+(3*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_3,//初始化原始数据指针
	sizeof(InitData_6012_2_3),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_4_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_4[]=//任务配置表4
{
	0x60120200,//OAD
  ADDR_6012_2+(4*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_4,//初始化原始数据指针
	sizeof(InitData_6012_2_4),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_5_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_5[]=//任务配置表5
{
	0x60120200,//OAD
  ADDR_6012_2+(5*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_5,//初始化原始数据指针
	sizeof(InitData_6012_2_5),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_6_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_6[]=//任务配置表6
{
	0x60120200,//OAD
  ADDR_6012_2+(6*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_6,//初始化原始数据指针
	sizeof(InitData_6012_2_6),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_7_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_7[]=//任务配置表7
{
	0x60120200,//OAD
  ADDR_6012_2+(7*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_7,//初始化原始数据指针
	sizeof(InitData_6012_2_7),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_8_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_8[]=//任务配置表8
{
	0x60120200,//OAD
  ADDR_6012_2+(8*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_8,//初始化原始数据指针
	sizeof(InitData_6012_2_8),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_9_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_9[]=//任务配置表9
{
	0x60120200,//OAD
  ADDR_6012_2+(9*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_9,//初始化原始数据指针
	sizeof(InitData_6012_2_9),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_10_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_10[]=//任务配置表10
{
	0x60120200,//OAD
  ADDR_6012_2+(10*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_10,//初始化原始数据指针
	sizeof(InitData_6012_2_10),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_11_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_11[]=//任务配置表11
{
	0x60120200,//OAD
  ADDR_6012_2+(11*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_11,//初始化原始数据指针
	sizeof(InitData_6012_2_11),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_12_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_12[]=//任务配置表12
{
	0x60120200,//OAD
  ADDR_6012_2+(12*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_12,//初始化原始数据指针
	sizeof(InitData_6012_2_12),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_13_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_13[]=//任务配置表13
{
	0x60120200,//OAD
  ADDR_6012_2+(13*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_13,//初始化原始数据指针
	sizeof(InitData_6012_2_13),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_14_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_14[]=//任务配置表14
{
	0x60120200,//OAD
  ADDR_6012_2+(14*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_14,//初始化原始数据指针
	sizeof(InitData_6012_2_14),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_15_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_15[]=//任务配置表15
{
	0x60120200,//OAD
  ADDR_6012_2+(15*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_15,//初始化原始数据指针
	sizeof(InitData_6012_2_15),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_16_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_16[]=//任务配置表16
{
	0x60120200,//OAD
  ADDR_6012_2+(16*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_16,//初始化原始数据指针
	sizeof(InitData_6012_2_16),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_17_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_17[]=//任务配置表17
{
	0x60120200,//OAD
  ADDR_6012_2+(17*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_17,//初始化原始数据指针
	sizeof(InitData_6012_2_17),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_18_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_18[]=//任务配置表18
{
	0x60120200,//OAD
  ADDR_6012_2+(18*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_18,//初始化原始数据指针
	sizeof(InitData_6012_2_18),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_19_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_19[]=//任务配置表19
{
	0x60120200,//OAD
  ADDR_6012_2+(19*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_19,//初始化原始数据指针
	sizeof(InitData_6012_2_19),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_20_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_20[]=//任务配置表20
{
	0x60120200,//OAD
  ADDR_6012_2+(20*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_20,//初始化原始数据指针
	sizeof(InitData_6012_2_20),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_21_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_21[]=//任务配置表21
{
	0x60120200,//OAD
  ADDR_6012_2+(21*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_21,//初始化原始数据指针
	sizeof(InitData_6012_2_21),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_22_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_22[]=//任务配置表22
{
	0x60120200,//OAD
  ADDR_6012_2+(22*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_22,//初始化原始数据指针
	sizeof(InitData_6012_2_22),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_23_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_23[]=//任务配置表23
{
	0x60120200,//OAD
  ADDR_6012_2+(23*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_23,//初始化原始数据指针
	sizeof(InitData_6012_2_23),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_24_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_24[]=//任务配置表24
{
	0x60120200,//OAD
  ADDR_6012_2+(24*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_24,//初始化原始数据指针
	sizeof(InitData_6012_2_24),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_25_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_25[]=//任务配置表25
{
	0x60120200,//OAD
  ADDR_6012_2+(25*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_25,//初始化原始数据指针
	sizeof(InitData_6012_2_25),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_26_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_26[]=//任务配置表26
{
	0x60120200,//OAD
  ADDR_6012_2+(26*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_26,//初始化原始数据指针
	sizeof(InitData_6012_2_26),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_27_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_27[]=//任务配置表27
{
	0x60120200,//OAD
  ADDR_6012_2+(27*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_27,//初始化原始数据指针
	sizeof(InitData_6012_2_27),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_28_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_28[]=//任务配置表28
{
	0x60120200,//OAD
  ADDR_6012_2+(28*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_28,//初始化原始数据指针
	sizeof(InitData_6012_2_28),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_29_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_29[]=//任务配置表29
{
	0x60120200,//OAD
  ADDR_6012_2+(29*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_29,//初始化原始数据指针
	sizeof(InitData_6012_2_29),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_30_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_30[]=//任务配置表30
{
	0x60120200,//OAD
  ADDR_6012_2+(30*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_30,//初始化原始数据指针
	sizeof(InitData_6012_2_30),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_31_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_31[]=//任务配置表31
{
	0x60120200,//OAD
  ADDR_6012_2+(31*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_31,//初始化原始数据指针
	sizeof(InitData_6012_2_31),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_32_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_32[]=//任务配置表32
{
	0x60120200,//OAD
  ADDR_6012_2+(32*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_32,//初始化原始数据指针
	sizeof(InitData_6012_2_32),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_33_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_33[]=//任务配置表33
{
	0x60120200,//OAD
  ADDR_6012_2+(33*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_33,//初始化原始数据指针
	sizeof(InitData_6012_2_33),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_34_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_34[]=//任务配置表34
{
	0x60120200,//OAD
  ADDR_6012_2+(34*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_34,//初始化原始数据指针
	sizeof(InitData_6012_2_34),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_35_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_35[]=//任务配置表35
{
	0x60120200,//OAD
  ADDR_6012_2+(35*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_35,//初始化原始数据指针
	sizeof(InitData_6012_2_35),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_36_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_36[]=//任务配置表36
{
	0x60120200,//OAD
  ADDR_6012_2+(36*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_36,//初始化原始数据指针
	sizeof(InitData_6012_2_36),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_37_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_37[]=//任务配置表37
{
	0x60120200,//OAD
  ADDR_6012_2+(37*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_37,//初始化原始数据指针
	sizeof(InitData_6012_2_37),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_38_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_38[]=//任务配置表38
{
	0x60120200,//OAD
  ADDR_6012_2+(38*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_38,//初始化原始数据指针
	sizeof(InitData_6012_2_38),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_39_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_39[]=//任务配置表39
{
	0x60120200,//OAD
  ADDR_6012_2+(39*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_39,//初始化原始数据指针
	sizeof(InitData_6012_2_39),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_40_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_40[]=//任务配置表40
{
	0x60120200,//OAD
  ADDR_6012_2+(40*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_40,//初始化原始数据指针
	sizeof(InitData_6012_2_40),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_41_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_41[]=//任务配置表41
{
	0x60120200,//OAD
  ADDR_6012_2+(41*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_41,//初始化原始数据指针
	sizeof(InitData_6012_2_41),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_42_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_42[]=//任务配置表42
{
	0x60120200,//OAD
  ADDR_6012_2+(42*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_42,//初始化原始数据指针
	sizeof(InitData_6012_2_42),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_43_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_43[]=//任务配置表43
{
	0x60120200,//OAD
  ADDR_6012_2+(43*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_43,//初始化原始数据指针
	sizeof(InitData_6012_2_43),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_44_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_44[]=//任务配置表44
{
	0x60120200,//OAD
  ADDR_6012_2+(44*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_44,//初始化原始数据指针
	sizeof(InitData_6012_2_44),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_45_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_45[]=//任务配置表45
{
	0x60120200,//OAD
  ADDR_6012_2+(45*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_45,//初始化原始数据指针
	sizeof(InitData_6012_2_45),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_46_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_46[]=//任务配置表46
{
	0x60120200,//OAD
  ADDR_6012_2+(46*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_46,//初始化原始数据指针
	sizeof(InitData_6012_2_46),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_47_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_47[]=//任务配置表47
{
	0x60120200,//OAD
  ADDR_6012_2+(47*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_47,//初始化原始数据指针
	sizeof(InitData_6012_2_47),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_48_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_48[]=//任务配置表48
{
	0x60120200,//OAD
  ADDR_6012_2+(48*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_48,//初始化原始数据指针
	sizeof(InitData_6012_2_48),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_49_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_49[]=//任务配置表49
{
	0x60120200,//OAD
  ADDR_6012_2+(49*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_49,//初始化原始数据指针
	sizeof(InitData_6012_2_49),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_50_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_50[]=//任务配置表50
{
	0x60120200,//OAD
  ADDR_6012_2+(50*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_50,//初始化原始数据指针
	sizeof(InitData_6012_2_50),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_51_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_51[]=//任务配置表51
{
	0x60120200,//OAD
  ADDR_6012_2+(51*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_51,//初始化原始数据指针
	sizeof(InitData_6012_2_51),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_52_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_52[]=//任务配置表52
{
	0x60120200,//OAD
  ADDR_6012_2+(52*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_52,//初始化原始数据指针
	sizeof(InitData_6012_2_52),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_53_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_53[]=//任务配置表53
{
	0x60120200,//OAD
  ADDR_6012_2+(53*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_53,//初始化原始数据指针
	sizeof(InitData_6012_2_53),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_54_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_54[]=//任务配置表54
{
	0x60120200,//OAD
  ADDR_6012_2+(54*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_54,//初始化原始数据指针
	sizeof(InitData_6012_2_54),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_55_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_55[]=//任务配置表55
{
	0x60120200,//OAD
  ADDR_6012_2+(55*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_55,//初始化原始数据指针
	sizeof(InitData_6012_2_55),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_56_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_56[]=//任务配置表56
{
	0x60120200,//OAD
  ADDR_6012_2+(56*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_56,//初始化原始数据指针
	sizeof(InitData_6012_2_56),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_57_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_57[]=//任务配置表57
{
	0x60120200,//OAD
  ADDR_6012_2+(57*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_57,//初始化原始数据指针
	sizeof(InitData_6012_2_57),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_58_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_58[]=//任务配置表58
{
	0x60120200,//OAD
  ADDR_6012_2+(58*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_50,//初始化原始数据指针
	sizeof(InitData_6012_2_58),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_59_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_59[]=//任务配置表59
{
	0x60120200,//OAD
  ADDR_6012_2+(59*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_59,//初始化原始数据指针
	sizeof(InitData_6012_2_59),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_60_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_60[]=//任务配置表60
{
	0x60120200,//OAD
  ADDR_6012_2+(60*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_60,//初始化原始数据指针
	sizeof(InitData_6012_2_60),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_61_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_61[]=//任务配置表61
{
	0x60120200,//OAD
  ADDR_6012_2+(61*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_61,//初始化原始数据指针
	sizeof(InitData_6012_2_61),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_62_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_62[]=//任务配置表62
{
	0x60120200,//OAD
  ADDR_6012_2+(62*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_62,//初始化原始数据指针
	sizeof(InitData_6012_2_62),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_63_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_63[]=//任务配置表63
{
	0x60120200,//OAD
  ADDR_6012_2+(63*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_63,//初始化原始数据指针
	sizeof(InitData_6012_2_63),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_64_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_64[]=//任务配置表64
{
	0x60120200,//OAD
  ADDR_6012_2+(64*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_64,//初始化原始数据指针
	sizeof(InitData_6012_2_64),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_65_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_65[]=//任务配置表65
{
	0x60120200,//OAD
  ADDR_6012_2+(65*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_65,//初始化原始数据指针
	sizeof(InitData_6012_2_65),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_66_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_66[]=//任务配置表66
{
	0x60120200,//OAD
  ADDR_6012_2+(66*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_66,//初始化原始数据指针
	sizeof(InitData_6012_2_66),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_67_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_67[]=//任务配置表67
{
	0x60120200,//OAD
  ADDR_6012_2+(67*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_67,//初始化原始数据指针
	sizeof(InitData_6012_2_67),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_68_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_68[]=//任务配置表68
{
	0x60120200,//OAD
  ADDR_6012_2+(68*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_68,//初始化原始数据指针
	sizeof(InitData_6012_2_68),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_69_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_69[]=//任务配置表69
{
	0x60120200,//OAD
  ADDR_6012_2+(69*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_69,//初始化原始数据指针
	sizeof(InitData_6012_2_69),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_70_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_70[]=//任务配置表70
{
	0x60120200,//OAD
  ADDR_6012_2+(70*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_70,//初始化原始数据指针
	sizeof(InitData_6012_2_70),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_71_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_71[]=//任务配置表71
{
	0x60120200,//OAD
  ADDR_6012_2+(71*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_71,//初始化原始数据指针
	sizeof(InitData_6012_2_71),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_72_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_72[]=//任务配置表72
{
	0x60120200,//OAD
  ADDR_6012_2+(72*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_72,//初始化原始数据指针
	sizeof(InitData_6012_2_72),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_73_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_73[]=//任务配置表73
{
	0x60120200,//OAD
  ADDR_6012_2+(73*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_73,//初始化原始数据指针
	sizeof(InitData_6012_2_73),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_74_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_74[]=//任务配置表74
{
	0x60120200,//OAD
  ADDR_6012_2+(74*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_74,//初始化原始数据指针
	sizeof(InitData_6012_2_74),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_75_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_75[]=//任务配置表75
{
	0x60120200,//OAD
  ADDR_6012_2+(75*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_75,//初始化原始数据指针
	sizeof(InitData_6012_2_75),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_76_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_76[]=//任务配置表76
{
	0x60120200,//OAD
  ADDR_6012_2+(76*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_76,//初始化原始数据指针
	sizeof(InitData_6012_2_76),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_77_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_77[]=//任务配置表77
{
	0x60120200,//OAD
  ADDR_6012_2+(77*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_77,//初始化原始数据指针
	sizeof(InitData_6012_2_77),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_78_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_78[]=//任务配置表78
{
	0x60120200,//OAD
  ADDR_6012_2+(78*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_78,//初始化原始数据指针
	sizeof(InitData_6012_2_78),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_79_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_79[]=//任务配置表79
{
	0x60120200,//OAD
  ADDR_6012_2+(79*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_79,//初始化原始数据指针
	sizeof(InitData_6012_2_79),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_80_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_80[]=//任务配置表80
{
	0x60120200,//OAD
  ADDR_6012_2+(80*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_80,//初始化原始数据指针
	sizeof(InitData_6012_2_80),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_81_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_81[]=//任务配置表81
{
	0x60120200,//OAD
  ADDR_6012_2+(81*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_81,//初始化原始数据指针
	sizeof(InitData_6012_2_81),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_82_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_82[]=//任务配置表82
{
	0x60120200,//OAD
  ADDR_6012_2+(82*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_82,//初始化原始数据指针
	sizeof(InitData_6012_2_82),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_83_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_83[]=//任务配置表83
{
	0x60120200,//OAD
  ADDR_6012_2+(83*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_83,//初始化原始数据指针
	sizeof(InitData_6012_2_83),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_84_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_84[]=//任务配置表84
{
	0x60120200,//OAD
  ADDR_6012_2+(84*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_84,//初始化原始数据指针
	sizeof(InitData_6012_2_84),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_85_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_85[]=//任务配置表85
{
	0x60120200,//OAD
  ADDR_6012_2+(85*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_85,//初始化原始数据指针
	sizeof(InitData_6012_2_85),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_86_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_86[]=//任务配置表86
{
	0x60120200,//OAD
  ADDR_6012_2+(86*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_86,//初始化原始数据指针
	sizeof(InitData_6012_2_86),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_87_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_87[]=//任务配置表87
{
	0x60120200,//OAD
  ADDR_6012_2+(87*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_87,//初始化原始数据指针
	sizeof(InitData_6012_2_87),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_88_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_88[]=//任务配置表88
{
	0x60120200,//OAD
  ADDR_6012_2+(88*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_88,//初始化原始数据指针
	sizeof(InitData_6012_2_88),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_89_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_89[]=//任务配置表89
{
	0x60120200,//OAD
  ADDR_6012_2+(89*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_89,//初始化原始数据指针
	sizeof(InitData_6012_2_89),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_90_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_90[]=//任务配置表90
{
	0x60120200,//OAD
  ADDR_6012_2+(90*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_90,//初始化原始数据指针
	sizeof(InitData_6012_2_90),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_91_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_91[]=//任务配置表91
{
	0x60120200,//OAD
  ADDR_6012_2+(91*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_91,//初始化原始数据指针
	sizeof(InitData_6012_2_91),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_92_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_92[]=//任务配置表92
{
	0x60120200,//OAD
  ADDR_6012_2+(92*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_92,//初始化原始数据指针
	sizeof(InitData_6012_2_92),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_93_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_93[]=//任务配置表93
{
	0x60120200,//OAD
  ADDR_6012_2+(93*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_93,//初始化原始数据指针
	sizeof(InitData_6012_2_93),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_94_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_94[]=//任务配置表94
{
	0x60120200,//OAD
  ADDR_6012_2+(94*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_94,//初始化原始数据指针
	sizeof(InitData_6012_2_94),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_95_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_95[]=//任务配置表95
{
	0x60120200,//OAD
  ADDR_6012_2+(95*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_95,//初始化原始数据指针
	sizeof(InitData_6012_2_95),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_96_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_96[]=//任务配置表96
{
	0x60120200,//OAD
  ADDR_6012_2+(96*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_96,//初始化原始数据指针
	sizeof(InitData_6012_2_96),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_97_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_97[]=//任务配置表97
{
	0x60120200,//OAD
  ADDR_6012_2+(97*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_97,//初始化原始数据指针
	sizeof(InitData_6012_2_97),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_98_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_98[]=//任务配置表98
{
	0x60120200,//OAD
  ADDR_6012_2+(98*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_98,//初始化原始数据指针
	sizeof(InitData_6012_2_98),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_99_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_99[]=//任务配置表99
{
	0x60120200,//OAD
  ADDR_6012_2+(99*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_99,//初始化原始数据指针
	sizeof(InitData_6012_2_99),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_100_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_100[]=//任务配置表100
{
	0x60120200,//OAD
  ADDR_6012_2+(100*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_100,//初始化原始数据指针
	sizeof(InitData_6012_2_100),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_101_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_101[]=//任务配置表101
{
	0x60120200,//OAD
  ADDR_6012_2+(101*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_101,//初始化原始数据指针
	sizeof(InitData_6012_2_101),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_102_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_102[]=//任务配置表102
{
	0x60120200,//OAD
  ADDR_6012_2+(102*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_102,//初始化原始数据指针
	sizeof(InitData_6012_2_102),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_103_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_103[]=//任务配置表103
{
	0x60120200,//OAD
  ADDR_6012_2+(103*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_103,//初始化原始数据指针
	sizeof(InitData_6012_2_103),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_104_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_104[]=//任务配置表104
{
	0x60120200,//OAD
  ADDR_6012_2+(104*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_104,//初始化原始数据指针
	sizeof(InitData_6012_2_104),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_105_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_105[]=//任务配置表105
{
	0x60120200,//OAD
  ADDR_6012_2+(105*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_105,//初始化原始数据指针
	sizeof(InitData_6012_2_105),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_106_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_106[]=//任务配置表106
{
	0x60120200,//OAD
  ADDR_6012_2+(106*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_106,//初始化原始数据指针
	sizeof(InitData_6012_2_106),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_107_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_107[]=//任务配置表107
{
	0x60120200,//OAD
  ADDR_6012_2+(107*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_107,//初始化原始数据指针
	sizeof(InitData_6012_2_107),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_108_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_108[]=//任务配置表108
{
	0x60120200,//OAD
  ADDR_6012_2+(108*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_108,//初始化原始数据指针
	sizeof(InitData_6012_2_108),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_109_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_109[]=//任务配置表109
{
	0x60120200,//OAD
  ADDR_6012_2+(109*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_109,//初始化原始数据指针
	sizeof(InitData_6012_2_109),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_110_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_110[]=//任务配置表110
{
	0x60120200,//OAD
  ADDR_6012_2+(110*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_110,//初始化原始数据指针
	sizeof(InitData_6012_2_110),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_111_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_111[]=//任务配置表111
{
	0x60120200,//OAD
  ADDR_6012_2+(111*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_111,//初始化原始数据指针
	sizeof(InitData_6012_2_111),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_112_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_112[]=//任务配置表112
{
	0x60120200,//OAD
  ADDR_6012_2+(112*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_112,//初始化原始数据指针
	sizeof(InitData_6012_2_112),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_113_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_113[]=//任务配置表113
{
	0x60120200,//OAD
  ADDR_6012_2+(113*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_113,//初始化原始数据指针
	sizeof(InitData_6012_2_113),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_114_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_114[]=//任务配置表114
{
	0x60120200,//OAD
  ADDR_6012_2+(114*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_114,//初始化原始数据指针
	sizeof(InitData_6012_2_114),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_115_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_115[]=//任务配置表115
{
	0x60120200,//OAD
  ADDR_6012_2+(115*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_115,//初始化原始数据指针
	sizeof(InitData_6012_2_115),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_116_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_116[]=//任务配置表116
{
	0x60120200,//OAD
  ADDR_6012_2+(116*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_116,//初始化原始数据指针
	sizeof(InitData_6012_2_116),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_117_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_117[]=//任务配置表117
{
	0x60120200,//OAD
  ADDR_6012_2+(117*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_117,//初始化原始数据指针
	sizeof(InitData_6012_2_117),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_118_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_118[]=//任务配置表118
{
	0x60120200,//OAD
  ADDR_6012_2+(118*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_118,//初始化原始数据指针
	sizeof(InitData_6012_2_118),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_119_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_119[]=//任务配置表119
{
	0x60120200,//OAD
  ADDR_6012_2+(119*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_119,//初始化原始数据指针
	sizeof(InitData_6012_2_119),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_120_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_120[]=//任务配置表120
{
	0x60120200,//OAD
  ADDR_6012_2+(120*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_120,//初始化原始数据指针
	sizeof(InitData_6012_2_120),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_121_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_121[]=//任务配置表121
{
	0x60120200,//OAD
  ADDR_6012_2+(121*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_121,//初始化原始数据指针
	sizeof(InitData_6012_2_121),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_122_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_122[]=//任务配置表122
{
	0x60120200,//OAD
  ADDR_6012_2+(122*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_122,//初始化原始数据指针
	sizeof(InitData_6012_2_122),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_123_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_123[]=//任务配置表123
{
	0x60120200,//OAD
  ADDR_6012_2+(123*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_123,//初始化原始数据指针
	sizeof(InitData_6012_2_123),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_124_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_124[]=//任务配置表124
{
	0x60120200,//OAD
  ADDR_6012_2+(124*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_124,//初始化原始数据指针
	sizeof(InitData_6012_2_124),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_125_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_125[]=//任务配置表125
{
	0x60120200,//OAD
  ADDR_6012_2+(125*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_125,//初始化原始数据指针
	sizeof(InitData_6012_2_125),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_126_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_126[]=//任务配置表126
{
	0x60120200,//OAD
  ADDR_6012_2+(126*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_126,//初始化原始数据指针
	sizeof(InitData_6012_2_126),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_127_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_127[]=//任务配置表127
{
	0x60120200,//OAD
  ADDR_6012_2+(127*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_127,//初始化原始数据指针
	sizeof(InitData_6012_2_127),//初始化原始数据长度
};
#endif
#ifdef InitData_6012_2_128_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_128[]=//任务配置表128
{
	0x60120200,//OAD
  ADDR_6012_2+(128*LENper_6012_2),//数据地址
	LENper_6012_2,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6012_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6012_2,//数据类型列表指针
	(u32*)InitData_6012_2_128,//初始化原始数据指针
	sizeof(InitData_6012_2_128),//初始化原始数据长度
};
#endif


#if NUMmax_6012_2>(128+1)//最大抄表任务数
	#error
#endif



__align(4) const u8 DL698_Attribute_6014[]=
{
	11,//类型列表数
//属性2（普通采集方案）∷=structure
	DataType_structure,
//{
//方案编号  unsigned，
	DataType_unsigned,
//存储深度  long-unsigned，
	DataType_long_unsigned,
//采集方式  structure
	DataType_structure,
//{
//  采集类型  unsigned，
	DataType_unsigned,
//  采集内容  Data
	DataType_Data,
//}，
//记录列选择  array CSD，
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_CSD,
	
//电能表集合  MS，
	DataType_MS,
//存储时标选择 enum
	DataType_enum,
//{
//未定义              （0），
//任务开始时间        （1），
//相对当日0点0分    （2），
//相对上日23点59分  （3），
//相对上日0点0分    （4），
//相对当月1日0点0分（5），
//数据冻结时标        （6）
//}
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2	NULL	按冻结时标采集
//3	TI	按时标间隔采集

};


__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_0[]=//普通采集方案0
{
	0x60140200,//OAD
  ADDR_6014,//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_0,//初始化原始数据指针
	sizeof(InitData_6014_2_0),//初始化原始数据长度
};
#ifdef InitData_6014_2_1_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_1[]=//普通采集方案1
{
	0x60140200,//OAD
  ADDR_6014+(1*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_1,//初始化原始数据指针
	sizeof(InitData_6014_2_1),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_2_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_2[]=//普通采集方案2
{
	0x60140200,//OAD
  ADDR_6014+(2*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_2,//初始化原始数据指针
	sizeof(InitData_6014_2_2),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_3_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_3[]=//普通采集方案3
{
	0x60140200,//OAD
  ADDR_6014+(3*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_3,//初始化原始数据指针
	sizeof(InitData_6014_2_3),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_4_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_4[]=//普通采集方案4
{
	0x60140200,//OAD
  ADDR_6014+(4*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_4,//初始化原始数据指针
	sizeof(InitData_6014_2_4),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_5_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_5[]=//普通采集方案5
{
	0x60140200,//OAD
  ADDR_6014+(5*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_5,//初始化原始数据指针
	sizeof(InitData_6014_2_5),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_6_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_6[]=//普通采集方案6
{
	0x60140200,//OAD
  ADDR_6014+(6*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_6,//初始化原始数据指针
	sizeof(InitData_6014_2_6),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_7_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_7[]=//普通采集方案7
{
	0x60140200,//OAD
  ADDR_6014+(7*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_7,//初始化原始数据指针
	sizeof(InitData_6014_2_7),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_8_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_8[]=//普通采集方案8
{
	0x60140200,//OAD
  ADDR_6014+(8*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_8,//初始化原始数据指针
	sizeof(InitData_6014_2_8),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_9_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_9[]=//普通采集方案9
{
	0x60140200,//OAD
  ADDR_6014+(9*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_9,//初始化原始数据指针
	sizeof(InitData_6014_2_9),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_10_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_10[]=//普通采集方案10
{
	0x60140200,//OAD
  ADDR_6014+(10*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_10,//初始化原始数据指针
	sizeof(InitData_6014_2_10),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_11_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_11[]=//普通采集方案11
{
	0x60140200,//OAD
  ADDR_6014+(11*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_11,//初始化原始数据指针
	sizeof(InitData_6014_2_11),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_12_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_12[]=//普通采集方案12
{
	0x60140200,//OAD
  ADDR_6014+(12*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_12,//初始化原始数据指针
	sizeof(InitData_6014_2_12),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_13_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_13[]=//普通采集方案13
{
	0x60140200,//OAD
  ADDR_6014+(13*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_13,//初始化原始数据指针
	sizeof(InitData_6014_2_13),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_14_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_14[]=//普通采集方案14
{
	0x60140200,//OAD
  ADDR_6014+(14*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_14,//初始化原始数据指针
	sizeof(InitData_6014_2_14),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_15_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_15[]=//普通采集方案15
{
	0x60140200,//OAD
  ADDR_6014+(15*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_15,//初始化原始数据指针
	sizeof(InitData_6014_2_15),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_16_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_16[]=//普通采集方案16
{
	0x60140200,//OAD
  ADDR_6014+(16*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_16,//初始化原始数据指针
	sizeof(InitData_6014_2_16),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_17_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_17[]=//普通采集方案17
{
	0x60140200,//OAD
  ADDR_6014+(17*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_17,//初始化原始数据指针
	sizeof(InitData_6014_2_17),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_18_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_18[]=//普通采集方案18
{
	0x60140200,//OAD
  ADDR_6014+(18*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_18,//初始化原始数据指针
	sizeof(InitData_6014_2_18),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_19_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_19[]=//普通采集方案19
{
	0x60140200,//OAD
  ADDR_6014+(19*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_19,//初始化原始数据指针
	sizeof(InitData_6014_2_19),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_20_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_20[]=//普通采集方案20
{
	0x60140200,//OAD
  ADDR_6014+(20*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_20,//初始化原始数据指针
	sizeof(InitData_6014_2_20),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_21_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_21[]=//普通采集方案21
{
	0x60140200,//OAD
  ADDR_6014+(21*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_21,//初始化原始数据指针
	sizeof(InitData_6014_2_21),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_22_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_22[]=//普通采集方案22
{
	0x60140200,//OAD
  ADDR_6014+(22*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_22,//初始化原始数据指针
	sizeof(InitData_6014_2_22),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_23_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_23[]=//普通采集方案23
{
	0x60140200,//OAD
  ADDR_6014+(23*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_23,//初始化原始数据指针
	sizeof(InitData_6014_2_23),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_24_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_24[]=//普通采集方案24
{
	0x60140200,//OAD
  ADDR_6014+(24*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_24,//初始化原始数据指针
	sizeof(InitData_6014_2_24),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_25_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_25[]=//普通采集方案25
{
	0x60140200,//OAD
  ADDR_6014+(25*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_25,//初始化原始数据指针
	sizeof(InitData_6014_2_25),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_26_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_26[]=//普通采集方案26
{
	0x60140200,//OAD
  ADDR_6014+(26*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_26,//初始化原始数据指针
	sizeof(InitData_6014_2_26),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_27_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_27[]=//普通采集方案27
{
	0x60140200,//OAD
  ADDR_6014+(27*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_27,//初始化原始数据指针
	sizeof(InitData_6014_2_27),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_28_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_28[]=//普通采集方案28
{
	0x60140200,//OAD
  ADDR_6014+(28*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_28,//初始化原始数据指针
	sizeof(InitData_6014_2_28),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_29_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_29[]=//普通采集方案29
{
	0x60140200,//OAD
  ADDR_6014+(29*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_29,//初始化原始数据指针
	sizeof(InitData_6014_2_29),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_30_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_30[]=//普通采集方案30
{
	0x60140200,//OAD
  ADDR_6014+(30*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_30,//初始化原始数据指针
	sizeof(InitData_6014_2_30),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_31_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_31[]=//普通采集方案31
{
	0x60140200,//OAD
  ADDR_6014+(31*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_31,//初始化原始数据指针
	sizeof(InitData_6014_2_31),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_32_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_32[]=//普通采集方案32
{
	0x60140200,//OAD
  ADDR_6014+(32*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_32,//初始化原始数据指针
	sizeof(InitData_6014_2_32),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_33_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_33[]=//普通采集方案33
{
	0x60140200,//OAD
  ADDR_6014+(33*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_33,//初始化原始数据指针
	sizeof(InitData_6014_2_33),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_34_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_34[]=//普通采集方案34
{
	0x60140200,//OAD
  ADDR_6014+(34*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_34,//初始化原始数据指针
	sizeof(InitData_6014_2_34),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_35_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_35[]=//普通采集方案35
{
	0x60140200,//OAD
  ADDR_6014+(35*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_35,//初始化原始数据指针
	sizeof(InitData_6014_2_35),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_36_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_36[]=//普通采集方案36
{
	0x60140200,//OAD
  ADDR_6014+(36*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_36,//初始化原始数据指针
	sizeof(InitData_6014_2_36),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_37_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_37[]=//普通采集方案37
{
	0x60140200,//OAD
  ADDR_6014+(37*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_37,//初始化原始数据指针
	sizeof(InitData_6014_2_37),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_38_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_38[]=//普通采集方案38
{
	0x60140200,//OAD
  ADDR_6014+(38*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_38,//初始化原始数据指针
	sizeof(InitData_6014_2_38),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_39_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_39[]=//普通采集方案39
{
	0x60140200,//OAD
  ADDR_6014+(39*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_39,//初始化原始数据指针
	sizeof(InitData_6014_2_39),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_40_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_40[]=//普通采集方案40
{
	0x60140200,//OAD
  ADDR_6014+(40*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_40,//初始化原始数据指针
	sizeof(InitData_6014_2_40),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_41_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_41[]=//普通采集方案41
{
	0x60140200,//OAD
  ADDR_6014+(41*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_41,//初始化原始数据指针
	sizeof(InitData_6014_2_41),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_42_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_42[]=//普通采集方案42
{
	0x60140200,//OAD
  ADDR_6014+(42*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_42,//初始化原始数据指针
	sizeof(InitData_6014_2_42),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_43_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_43[]=//普通采集方案43
{
	0x60140200,//OAD
  ADDR_6014+(43*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_43,//初始化原始数据指针
	sizeof(InitData_6014_2_43),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_44_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_44[]=//普通采集方案44
{
	0x60140200,//OAD
  ADDR_6014+(44*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_44,//初始化原始数据指针
	sizeof(InitData_6014_2_44),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_45_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_45[]=//普通采集方案45
{
	0x60140200,//OAD
  ADDR_6014+(45*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_45,//初始化原始数据指针
	sizeof(InitData_6014_2_45),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_46_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_46[]=//普通采集方案46
{
	0x60140200,//OAD
  ADDR_6014+(46*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_46,//初始化原始数据指针
	sizeof(InitData_6014_2_46),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_47_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_47[]=//普通采集方案47
{
	0x60140200,//OAD
  ADDR_6014+(47*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_47,//初始化原始数据指针
	sizeof(InitData_6014_2_47),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_48_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_48[]=//普通采集方案48
{
	0x60140200,//OAD
  ADDR_6014+(48*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_48,//初始化原始数据指针
	sizeof(InitData_6014_2_48),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_49_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_49[]=//普通采集方案49
{
	0x60140200,//OAD
  ADDR_6014+(49*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_49,//初始化原始数据指针
	sizeof(InitData_6014_2_49),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_50_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_50[]=//普通采集方案50
{
	0x60140200,//OAD
  ADDR_6014+(50*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_50,//初始化原始数据指针
	sizeof(InitData_6014_2_50),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_51_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_51[]=//普通采集方案51
{
	0x60140200,//OAD
  ADDR_6014+(51*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_51,//初始化原始数据指针
	sizeof(InitData_6014_2_51),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_52_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_52[]=//普通采集方案52
{
	0x60140200,//OAD
  ADDR_6014+(52*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_52,//初始化原始数据指针
	sizeof(InitData_6014_2_52),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_53_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_53[]=//普通采集方案53
{
	0x60140200,//OAD
  ADDR_6014+(53*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_53,//初始化原始数据指针
	sizeof(InitData_6014_2_53),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_54_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_54[]=//普通采集方案54
{
	0x60140200,//OAD
  ADDR_6014+(54*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_54,//初始化原始数据指针
	sizeof(InitData_6014_2_54),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_55_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_55[]=//普通采集方案55
{
	0x60140200,//OAD
  ADDR_6014+(55*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_55,//初始化原始数据指针
	sizeof(InitData_6014_2_55),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_56_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_56[]=//普通采集方案56
{
	0x60140200,//OAD
  ADDR_6014+(56*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_56,//初始化原始数据指针
	sizeof(InitData_6014_2_56),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_57_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_57[]=//普通采集方案57
{
	0x60140200,//OAD
  ADDR_6014+(57*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_57,//初始化原始数据指针
	sizeof(InitData_6014_2_57),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_58_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_58[]=//普通采集方案58
{
	0x60140200,//OAD
  ADDR_6014+(58*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_58,//初始化原始数据指针
	sizeof(InitData_6014_2_58),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_59_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_59[]=//普通采集方案59
{
	0x60140200,//OAD
  ADDR_6014+(59*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_59,//初始化原始数据指针
	sizeof(InitData_6014_2_59),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_60_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_60[]=//普通采集方案60
{
	0x60140200,//OAD
  ADDR_6014+(60*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_60,//初始化原始数据指针
	sizeof(InitData_6014_2_60),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_61_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_61[]=//普通采集方案61
{
	0x60140200,//OAD
  ADDR_6014+(61*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_61,//初始化原始数据指针
	sizeof(InitData_6014_2_61),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_62_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_62[]=//普通采集方案62
{
	0x60140200,//OAD
  ADDR_6014+(62*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_62,//初始化原始数据指针
	sizeof(InitData_6014_2_62),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_63_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_63[]=//普通采集方案63
{
	0x60140200,//OAD
  ADDR_6014+(63*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_63,//初始化原始数据指针
	sizeof(InitData_6014_2_63),//初始化原始数据长度
};
#endif
#ifdef InitData_6014_2_64_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_64[]=//普通采集方案64
{
	0x60140200,//OAD
  ADDR_6014+(64*LENper_6014),//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	(u32*)InitData_6014_2_64,//初始化原始数据指针
	sizeof(InitData_6014_2_64),//初始化原始数据长度
};
#endif


#if NUMmax_6014>(64+1)//最大普通采集方案数
	#error
#endif



__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_3[]=//当前元素个数
{
	0x60140300,//OAD
  ADDR_6014,//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_4[]=//最大元素个数
{
	0x60140400,//OAD
  ADDR_6014,//数据地址
	LENper_6014,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6014,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6014,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_6016[]=
{
	8,//类型列表数
//属性2（事件采集方案Event acq plan）∷=structure
//{
	DataType_structure,
//方案编号        unsigned，
	DataType_unsigned,
//采集方式        structure
	DataType_structure,
//{
//  采集类型      unsigned，
	DataType_unsigned,
//  采集内容      Data
	DataType_Data,
//}
//电能表集合      MS，
	DataType_MS,
//上报标识        bool（True：立即上报，False：不上报），
	DataType_bool,
//存储深度        long-unsigned
	DataType_long_unsigned,
//}
//采集方式数据格式如下：
//采集类型	采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
};


__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2[]=//事件采集方案
{
	0x60160200,//OAD
  ADDR_6016,//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

#ifdef InitData_6016_2_1_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_1[]=//事件采集方案1
{
	0x60160200,//OAD
  ADDR_6016+(1*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_1,//初始化原始数据指针
	sizeof(InitData_6016_2_1),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_2_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_2[]=//事件采集方案2
{
	0x60160200,//OAD
  ADDR_6016+(2*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_2,//初始化原始数据指针
	sizeof(InitData_6016_2_2),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_3_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_3[]=//事件采集方案3
{
	0x60160200,//OAD
  ADDR_6016+(3*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_3,//初始化原始数据指针
	sizeof(InitData_6016_2_3),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_4_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_4[]=//事件采集方案4
{
	0x60160200,//OAD
  ADDR_6016+(4*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_4,//初始化原始数据指针
	sizeof(InitData_6016_2_4),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_5_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_5[]=//事件采集方案5
{
	0x60160200,//OAD
  ADDR_6016+(5*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_5,//初始化原始数据指针
	sizeof(InitData_6016_2_5),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_6_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_6[]=//事件采集方案6
{
	0x60160200,//OAD
  ADDR_6016+(6*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_6,//初始化原始数据指针
	sizeof(InitData_6016_2_6),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_7_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_7[]=//事件采集方案7
{
	0x60160200,//OAD
  ADDR_6016+(7*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_7,//初始化原始数据指针
	sizeof(InitData_6016_2_7),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_8_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_8[]=//事件采集方案8
{
	0x60160200,//OAD
  ADDR_6016+(8*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_8,//初始化原始数据指针
	sizeof(InitData_6016_2_8),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_9_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_9[]=//事件采集方案9
{
	0x60160200,//OAD
  ADDR_6016+(9*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_9,//初始化原始数据指针
	sizeof(InitData_6016_2_9),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_10_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_10[]=//事件采集方案10
{
	0x60160200,//OAD
  ADDR_6016+(10*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_10,//初始化原始数据指针
	sizeof(InitData_6016_2_10),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_11_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_11[]=//事件采集方案11
{
	0x60160200,//OAD
  ADDR_6016+(11*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_11,//初始化原始数据指针
	sizeof(InitData_6016_2_11),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_12_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_12[]=//事件采集方案12
{
	0x60160200,//OAD
  ADDR_6016+(12*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_12,//初始化原始数据指针
	sizeof(InitData_6016_2_12),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_13_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_13[]=//事件采集方案13
{
	0x60160200,//OAD
  ADDR_6016+(13*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_13,//初始化原始数据指针
	sizeof(InitData_6016_2_13),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_14_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_14[]=//事件采集方案14
{
	0x60160200,//OAD
  ADDR_6016+(14*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_14,//初始化原始数据指针
	sizeof(InitData_6016_2_14),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_15_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_15[]=//事件采集方案15
{
	0x60160200,//OAD
  ADDR_6016+(15*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_15,//初始化原始数据指针
	sizeof(InitData_6016_2_15),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_16_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_16[]=//事件采集方案16
{
	0x60160200,//OAD
  ADDR_6016+(16*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_16,//初始化原始数据指针
	sizeof(InitData_6016_2_16),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_17_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_17[]=//事件采集方案17
{
	0x60160200,//OAD
  ADDR_6016+(17*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_17,//初始化原始数据指针
	sizeof(InitData_6016_2_17),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_18_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_18[]=//事件采集方案18
{
	0x60160200,//OAD
  ADDR_6016+(18*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_18,//初始化原始数据指针
	sizeof(InitData_6016_2_18),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_19_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_19[]=//事件采集方案19
{
	0x60160200,//OAD
  ADDR_6016+(19*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_19,//初始化原始数据指针
	sizeof(InitData_6016_2_19),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_20_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_20[]=//事件采集方案20
{
	0x60160200,//OAD
  ADDR_6016+(20*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_20,//初始化原始数据指针
	sizeof(InitData_6016_2_20),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_21_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_21[]=//事件采集方案21
{
	0x60160200,//OAD
  ADDR_6016+(21*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_21,//初始化原始数据指针
	sizeof(InitData_6016_2_21),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_22_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_22[]=//事件采集方案22
{
	0x60160200,//OAD
  ADDR_6016+(22*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_22,//初始化原始数据指针
	sizeof(InitData_6016_2_22),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_23_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_23[]=//事件采集方案23
{
	0x60160200,//OAD
  ADDR_6016+(23*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_23,//初始化原始数据指针
	sizeof(InitData_6016_2_23),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_24_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_24[]=//事件采集方案24
{
	0x60160200,//OAD
  ADDR_6016+(24*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_24,//初始化原始数据指针
	sizeof(InitData_6016_2_24),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_25_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_25[]=//事件采集方案25
{
	0x60160200,//OAD
  ADDR_6016+(25*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_25,//初始化原始数据指针
	sizeof(InitData_6016_2_25),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_26_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_26[]=//事件采集方案26
{
	0x60160200,//OAD
  ADDR_6016+(26*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_26,//初始化原始数据指针
	sizeof(InitData_6016_2_26),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_27_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_27[]=//事件采集方案27
{
	0x60160200,//OAD
  ADDR_6016+(27*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_27,//初始化原始数据指针
	sizeof(InitData_6016_2_27),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_28_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_28[]=//事件采集方案28
{
	0x60160200,//OAD
  ADDR_6016+(28*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_28,//初始化原始数据指针
	sizeof(InitData_6016_2_28),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_29_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_29[]=//事件采集方案29
{
	0x60160200,//OAD
  ADDR_6016+(29*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_29,//初始化原始数据指针
	sizeof(InitData_6016_2_29),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_30_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_30[]=//事件采集方案30
{
	0x60160200,//OAD
  ADDR_6016+(30*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_30,//初始化原始数据指针
	sizeof(InitData_6016_2_30),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_31_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_31[]=//事件采集方案31
{
	0x60160200,//OAD
  ADDR_6016+(31*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_31,//初始化原始数据指针
	sizeof(InitData_6016_2_31),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_32_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_32[]=//事件采集方案32
{
	0x60160200,//OAD
  ADDR_6016+(32*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_32,//初始化原始数据指针
	sizeof(InitData_6016_2_32),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_33_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_33[]=//事件采集方案33
{
	0x60160200,//OAD
  ADDR_6016+(33*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_33,//初始化原始数据指针
	sizeof(InitData_6016_2_33),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_34_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_34[]=//事件采集方案34
{
	0x60160200,//OAD
  ADDR_6016+(34*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_34,//初始化原始数据指针
	sizeof(InitData_6016_2_34),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_35_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_35[]=//事件采集方案35
{
	0x60160200,//OAD
  ADDR_6016+(35*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_35,//初始化原始数据指针
	sizeof(InitData_6016_2_35),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_36_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_36[]=//事件采集方案36
{
	0x60160200,//OAD
  ADDR_6016+(36*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_36,//初始化原始数据指针
	sizeof(InitData_6016_2_36),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_37_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_37[]=//事件采集方案37
{
	0x60160200,//OAD
  ADDR_6016+(37*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_37,//初始化原始数据指针
	sizeof(InitData_6016_2_37),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_38_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_38[]=//事件采集方案38
{
	0x60160200,//OAD
  ADDR_6016+(38*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_38,//初始化原始数据指针
	sizeof(InitData_6016_2_38),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_39_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_39[]=//事件采集方案39
{
	0x60160200,//OAD
  ADDR_6016+(39*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_39,//初始化原始数据指针
	sizeof(InitData_6016_2_39),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_40_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_40[]=//事件采集方案40
{
	0x60160200,//OAD
  ADDR_6016+(40*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_40,//初始化原始数据指针
	sizeof(InitData_6016_2_40),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_41_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_41[]=//事件采集方案41
{
	0x60160200,//OAD
  ADDR_6016+(41*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_41,//初始化原始数据指针
	sizeof(InitData_6016_2_41),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_42_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_42[]=//事件采集方案42
{
	0x60160200,//OAD
  ADDR_6016+(42*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_42,//初始化原始数据指针
	sizeof(InitData_6016_2_42),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_43_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_43[]=//事件采集方案43
{
	0x60160200,//OAD
  ADDR_6016+(43*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_43,//初始化原始数据指针
	sizeof(InitData_6016_2_43),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_44_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_44[]=//事件采集方案44
{
	0x60160200,//OAD
  ADDR_6016+(44*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_44,//初始化原始数据指针
	sizeof(InitData_6016_2_44),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_45_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_45[]=//事件采集方案45
{
	0x60160200,//OAD
  ADDR_6016+(45*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_45,//初始化原始数据指针
	sizeof(InitData_6016_2_45),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_46_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_46[]=//事件采集方案46
{
	0x60160200,//OAD
  ADDR_6016+(46*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_46,//初始化原始数据指针
	sizeof(InitData_6016_2_46),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_47_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_47[]=//事件采集方案47
{
	0x60160200,//OAD
  ADDR_6016+(47*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_47,//初始化原始数据指针
	sizeof(InitData_6016_2_47),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_48_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_48[]=//事件采集方案48
{
	0x60160200,//OAD
  ADDR_6016+(48*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_48,//初始化原始数据指针
	sizeof(InitData_6016_2_48),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_49_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_49[]=//事件采集方案49
{
	0x60160200,//OAD
  ADDR_6016+(49*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_49,//初始化原始数据指针
	sizeof(InitData_6016_2_49),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_50_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_50[]=//事件采集方案50
{
	0x60160200,//OAD
  ADDR_6016+(50*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_50,//初始化原始数据指针
	sizeof(InitData_6016_2_50),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_51_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_51[]=//事件采集方案51
{
	0x60160200,//OAD
  ADDR_6016+(51*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_51,//初始化原始数据指针
	sizeof(InitData_6016_2_51),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_52_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_52[]=//事件采集方案52
{
	0x60160200,//OAD
  ADDR_6016+(52*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_52,//初始化原始数据指针
	sizeof(InitData_6016_2_52),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_53_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_53[]=//事件采集方案53
{
	0x60160200,//OAD
  ADDR_6016+(53*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_53,//初始化原始数据指针
	sizeof(InitData_6016_2_53),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_54_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_54[]=//事件采集方案54
{
	0x60160200,//OAD
  ADDR_6016+(54*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_54,//初始化原始数据指针
	sizeof(InitData_6016_2_54),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_55_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_55[]=//事件采集方案55
{
	0x60160200,//OAD
  ADDR_6016+(55*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_55,//初始化原始数据指针
	sizeof(InitData_6016_2_55),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_56_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_56[]=//事件采集方案56
{
	0x60160200,//OAD
  ADDR_6016+(56*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_56,//初始化原始数据指针
	sizeof(InitData_6016_2_56),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_57_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_57[]=//事件采集方案57
{
	0x60160200,//OAD
  ADDR_6016+(57*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_57,//初始化原始数据指针
	sizeof(InitData_6016_2_57),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_58_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_58[]=//事件采集方案58
{
	0x60160200,//OAD
  ADDR_6016+(58*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_58,//初始化原始数据指针
	sizeof(InitData_6016_2_58),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_59_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_59[]=//事件采集方案59
{
	0x60160200,//OAD
  ADDR_6016+(59*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_59,//初始化原始数据指针
	sizeof(InitData_6016_2_59),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_60_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_60[]=//事件采集方案60
{
	0x60160200,//OAD
  ADDR_6016+(60*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_60,//初始化原始数据指针
	sizeof(InitData_6016_2_60),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_61_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_61[]=//事件采集方案61
{
	0x60160200,//OAD
  ADDR_6016+(61*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_61,//初始化原始数据指针
	sizeof(InitData_6016_2_61),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_62_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_62[]=//事件采集方案62
{
	0x60160200,//OAD
  ADDR_6016+(62*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_62,//初始化原始数据指针
	sizeof(InitData_6016_2_62),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_63_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_63[]=//事件采集方案63
{
	0x60160200,//OAD
  ADDR_6016+(63*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_63,//初始化原始数据指针
	sizeof(InitData_6016_2_63),//初始化原始数据长度
};
#endif
#ifdef InitData_6016_2_64_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2_64[]=//事件采集方案64
{
	0x60160200,//OAD
  ADDR_6016+(64*LENper_6016),//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	(u32*)InitData_6016_2_64,//初始化原始数据指针
	sizeof(InitData_6016_2_64),//初始化原始数据长度
};
#endif

#if NUMmax_6016>(64+1)//最大事件采集方案数
	#error
#endif

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_3[]=//当前元素个数
{
	0x60160300,//OAD
  ADDR_6016,//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_4[]=//最大元素个数
{
	0x60160400,//OAD
  ADDR_6016,//数据地址
	LENper_6016,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6016,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6016,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_6018[]=
{
	22,//类型列表数
//属性2（透明方案）∷=structure
	DataType_structure,
//{
//	方案编号     unsigned，
	DataType_unsigned,
//	方案内容集   array 方案内容，
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//	方案内容∷=structure
	DataType_structure,
//	{
//	通信地址        	TSA，
	DataType_TSA,
//	开始前脚本id   	long-unsigned，
	DataType_long_unsigned,
//	完成后脚本id   	long-unsigned，
	DataType_long_unsigned,
//	方案控制标志  	structure，
	DataType_structure,
//		{
//		上报透明方案结果并等待后续报文  bool，
	DataType_bool,
//		等待后续报文超时时间（秒）      long-unsigned，
	//DataType_long_unsigned,
	DataType_Data,
//		结果比对标识    enum{不比对（0），比（1），比对上报（2）}，
	DataType_enum,
//		结果比对参数    structure
	DataType_structure,
//			{
//				特征字节      unsigned，
	DataType_unsigned,
//				截取开始      long-unsigned，
	DataType_long_unsigned,
//				截取长度      long-unsigned
	DataType_long_unsigned,
//			}
//		}
//	方案报文集 	 	array 方案报文
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//		方案报文∷=structure
	DataType_structure,
//		{
//		报文序号  unsigned，
	DataType_unsigned,
//		报文内容  octet-string
	DataType_octet_string,
//		}
//	}
//	存储深度     long-unsigned
	DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6018[]=//透明方案
{
	0x60180200,//OAD
  ADDR_6018,//数据地址
	LENper_6018,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6018,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6018,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6018_3[]=//当前元素个数
{
	0x60180300,//OAD
  ADDR_6018,//数据地址
	LENper_6018,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6018,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6018,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6018_4[]=//最大元素个数
{
	0x60180400,//OAD
  ADDR_6018,//数据地址
	LENper_6018,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6018,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6018,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_601A[]=
{
	12,//类型列表数
//属性2∷=array一个透明方案结果
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个透明方案结果∷=structure
	DataType_structure,
//{
//方案编号      unsigned，
	DataType_unsigned,
//方案执行时间  date_time_s，
	DataType_date_time_s,
//通信地址      TSA，
	DataType_TSA,
//结果集        array 结果
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//}
//结果∷=structure
	DataType_structure,
//{
//报文序号      unsigned，
	DataType_unsigned,
//报文响应时间  date_time_s，
	DataType_date_time_s,
//命令结果      octet-string
	DataType_octet_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601A[]=//透明方案结果
{
	0x601A0200,//OAD
  0,//ADDR_601A,//数据地址
	0,//LENper_601A,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//LENmax_601A,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601A,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601A_3[]=//当前元素个数
{
	0x601A0300,//OAD
  0,//ADDR_601A,//数据地址
	0,//LENper_601A,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//LENmax_601A,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601A,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601A_4[]=//最大元素个数
{
	0x601A0400,//OAD
  0,//ADDR_601A,//数据地址
	0,//LENper_601A,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//LENmax_601A,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601A,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_601C[]=
{
	12,//类型列表数
//属性2（上报方案 report plan）∷=structure
	DataType_structure,
//{
//方案编号          unsigned，
	DataType_unsigned,
//上报通道          array OAD，
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_OAD,
//上报响应超时时间  TI，
	DataType_TI,
//最大上报次数      unsigned，
	DataType_unsigned,
//上报内容          strcuture
	DataType_structure,
//{
//  类型  unsigned，
	DataType_unsigned,
//  数据  Data
	DataType_Data,
//}
//}
//上报内容：
//上报类型	  上报内容	表示
// 0	      OAD	对象属性数据
// 1	      RecordData	上报记录型对象属性
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//记录型对象属性描述符  RCSD，
//行选择                RSD
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C[]=//上报方案集
{
	0x601C0200,//OAD
	ADDR_601C,//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_0,//初始化原始数据指针
	sizeof(InitData_601C_2_0),//初始化原始数据长度
};

#ifdef InitData_601C_2_1_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_1[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(1*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_1,//初始化原始数据指针
	sizeof(InitData_601C_2_1),//初始化原始数据长度
};
#endif

#ifdef InitData_601C_2_2_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_2[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(2*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_2,//初始化原始数据指针
	sizeof(InitData_601C_2_2),//初始化原始数据长度
};
#endif

#ifdef InitData_601C_2_3_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_3[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(3*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_3,//初始化原始数据指针
	sizeof(InitData_601C_2_3),//初始化原始数据长度
};
#endif

#ifdef InitData_601C_2_4_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_4[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(4*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_4,//初始化原始数据指针
	sizeof(InitData_601C_2_4),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_5_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_5[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(5*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_5,//初始化原始数据指针
	sizeof(InitData_601C_2_5),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_6_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_6[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(6*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_6,//初始化原始数据指针
	sizeof(InitData_601C_2_6),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_7_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_7[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(7*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_7,//初始化原始数据指针
	sizeof(InitData_601C_2_7),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_8_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_8[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(8*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_8,//初始化原始数据指针
	sizeof(InitData_601C_2_8),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_9_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_9[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(9*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_9,//初始化原始数据指针
	sizeof(InitData_601C_2_9),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_10_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_10[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(10*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_10,//初始化原始数据指针
	sizeof(InitData_601C_2_10),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_11_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_11[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(11*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_11,//初始化原始数据指针
	sizeof(InitData_601C_2_11),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_12_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_12[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(12*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_12,//初始化原始数据指针
	sizeof(InitData_601C_2_12),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_13_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_13[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(13*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_13,//初始化原始数据指针
	sizeof(InitData_601C_2_13),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_14_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_14[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(14*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_14,//初始化原始数据指针
	sizeof(InitData_601C_2_14),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_15_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_15[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(15*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_15,//初始化原始数据指针
	sizeof(InitData_601C_2_15),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_16_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_16[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(16*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_16,//初始化原始数据指针
	sizeof(InitData_601C_2_16),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_17_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_17[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(17*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_17,//初始化原始数据指针
	sizeof(InitData_601C_2_17),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_18_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_18[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(18*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_18,//初始化原始数据指针
	sizeof(InitData_601C_2_18),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_19_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_19[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(19*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_19,//初始化原始数据指针
	sizeof(InitData_601C_2_19),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_20_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_20[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(20*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_20,//初始化原始数据指针
	sizeof(InitData_601C_2_20),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_21_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_21[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(21*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_21,//初始化原始数据指针
	sizeof(InitData_601C_2_21),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_22_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_22[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(22*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_22,//初始化原始数据指针
	sizeof(InitData_601C_2_22),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_23_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_23[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(23*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_23,//初始化原始数据指针
	sizeof(InitData_601C_2_23),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_24_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_24[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(24*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_24,//初始化原始数据指针
	sizeof(InitData_601C_2_24),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_25_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_25[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(25*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_25,//初始化原始数据指针
	sizeof(InitData_601C_2_25),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_26_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_26[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(26*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_26,//初始化原始数据指针
	sizeof(InitData_601C_2_26),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_27_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_27[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(27*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_27,//初始化原始数据指针
	sizeof(InitData_601C_2_27),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_28_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_28[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(28*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_28,//初始化原始数据指针
	sizeof(InitData_601C_2_28),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_29_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_29[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(29*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_29,//初始化原始数据指针
	sizeof(InitData_601C_2_29),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_30_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_30[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(30*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_30,//初始化原始数据指针
	sizeof(InitData_601C_2_30),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_31_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_31[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(31*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_31,//初始化原始数据指针
	sizeof(InitData_601C_2_31),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_32_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_32[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(32*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_32,//初始化原始数据指针
	sizeof(InitData_601C_2_32),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_33_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_33[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(33*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_33,//初始化原始数据指针
	sizeof(InitData_601C_2_33),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_34_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_34[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(34*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_34,//初始化原始数据指针
	sizeof(InitData_601C_2_34),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_35_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_35[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(35*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_35,//初始化原始数据指针
	sizeof(InitData_601C_2_35),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_36_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_36[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(36*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_36,//初始化原始数据指针
	sizeof(InitData_601C_2_36),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_37_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_37[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(37*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_37,//初始化原始数据指针
	sizeof(InitData_601C_2_37),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_38_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_38[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(38*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_38,//初始化原始数据指针
	sizeof(InitData_601C_2_38),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_39_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_39[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(39*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_39,//初始化原始数据指针
	sizeof(InitData_601C_2_39),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_40_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_40[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(40*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_40,//初始化原始数据指针
	sizeof(InitData_601C_2_40),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_41_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_41[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(41*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_41,//初始化原始数据指针
	sizeof(InitData_601C_2_41),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_42_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_42[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(42*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_42,//初始化原始数据指针
	sizeof(InitData_601C_2_42),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_43_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_43[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(43*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_43,//初始化原始数据指针
	sizeof(InitData_601C_2_43),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_44_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_44[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(44*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_44,//初始化原始数据指针
	sizeof(InitData_601C_2_44),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_45_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_45[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(45*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_45,//初始化原始数据指针
	sizeof(InitData_601C_2_45),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_46_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_46[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(46*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_46,//初始化原始数据指针
	sizeof(InitData_601C_2_46),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_47_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_47[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(47*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_47,//初始化原始数据指针
	sizeof(InitData_601C_2_47),//初始化原始数据长度
};
#endif
#ifdef InitData_601C_2_48_
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_2_48[]=//上报方案集
{
	0x601C0200,//OAD
  ADDR_601C+(48*LENper_601C),//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	(u32*)InitData_601C_2_48,//初始化原始数据指针
	sizeof(InitData_601C_2_48),//初始化原始数据长度
};
#endif

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_3[]=//当前元素个数
{
	0x601C0300,//OAD
  ADDR_601C,//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601C_4[]=//最大元素个数
{
	0x601C0400,//OAD
  ADDR_601C,//数据地址
	LENper_601C,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601C,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601C,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_601E[]=
{
	19,//类型列表数
//属性2∷=structure
	DataType_structure,
//{
//数据列选择描述符  CSD，
	DataType_CSD,
//规则描述  structrue
	DataType_structure,
//{
//AcqCmd_2007  structure
	DataType_structure,
//{
//主用DI  array octet-string(SIZE(4))，
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_octet_string,
//备用DI  array octet-string(SIZE(4))
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_octet_string,
//}，
//AcqCmd_1997  structure
	DataType_structure,
//{
//主用DI  array octet-string(SIZE(2))，
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_octet_string,
//备用DI  array octet-string(SIZE(2))
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_octet_string,
//}，
//AcqCmd_Trans  structure
	DataType_structure,
//{
//Frame  octet-string
	DataType_octet_string,
//}
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601E[]=//采集规则库
{
	0x601E0200,//OAD
  ADDR_601E,//数据地址
	LENper_601E,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601E,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601E,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601E_3[]=//当前元素个数
{
	0x601E0300,//OAD
  ADDR_601E,//数据地址
	LENper_601E,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601E,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601E,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_601E_4[]=//最大元素个数
{
	0x601E0400,//OAD
  ADDR_601E,//数据地址
	LENper_601E,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_601E,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_601E,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_6032[]=
{
	9,//类型列表数
//一个采集状态∷=structure
	DataType_structure,
//{
//通信地址              TSA，
	DataType_TSA,
//中继级别              unsigned，
	DataType_unsigned,
//中继地址              TSA，
	DataType_TSA,
//端口                  OAD，
	DataType_OAD,
//最后一次采集成功时间  date_time_s，
	DataType_date_time_s,
//采集失败次数          unsigned，
	DataType_unsigned,
//相位       enum{未知（0），A相（1），B相（2），C相（3）}，
	DataType_enum,
//相序异常   enum{正常（0），LN互易（1），逆相序（2）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6032[]=//采集状态集
{
	0x60320200,//OAD
  ADDR_6032,//数据地址
	LENper_6032,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6032,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6032,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6032_3[]=//当前元素个数
{
	0x60320300,//OAD
  ADDR_6032,//数据地址
	LENper_6032,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6032,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6032,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6032_4[]=//最大元素个数
{
	0x60320400,//OAD
  ADDR_6032,//数据地址
	LENper_6032,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6032,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6032,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_6034[]=
{
	9,//类型列表数
//采集任务监控单元∷=structure
	DataType_structure,
//{
//任务ID       unsigned
	DataType_unsigned,
//任务执行状态  enum
	DataType_enum,
//{
//未执行（0），
//执行中（1），
//已执行（2）
//}，
//任务执行开始时间   date_time_s，
	DataType_date_time_s,
//任务执行结束时间   date_time_s，
	DataType_date_time_s,
//采集总数量         long-unsigned，
	DataType_long_unsigned,
//采集成功数量       long-unsigned，
	DataType_long_unsigned,
//已发送报文条数     long-unsigned，
	DataType_long_unsigned,
//已接收报文条数     long-unsigned
	DataType_long_unsigned,
//}
//说明：采集成功数量、已发送报文条数、已接收报文条数，每次任务执行先清零。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6034[]=//采集任务监控集
{
	0x60340200,//OAD
  ADDR_6034,//数据地址
	LENper_6034,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6034,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6034,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6034_3[]=//当前元素个数
{
	0x60340300,//OAD
  ADDR_6034,//数据地址
	LENper_6034,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6034,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6034,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6034_4[]=//最大元素个数
{
	0x60340400,//OAD
  ADDR_6034,//数据地址
	LENper_6034,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6034,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_6034,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


#if (USER/100)==14//福建增补
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E00[]=//(福建)外部数据指的是除交采和终端本体类数据通过端口交换的数据
{
	0x6E000200,//OAD
  ADDR_6E00,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E00,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)InitData_6E00,//初始化原始数据指针
	sizeof(InitData_6E00),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E01[]=//(福建)交采应能支持原终端的采集模式
{
	0x6E010200,//OAD
  ADDR_6E01,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E01,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)InitData_6E01,//初始化原始数据指针
	sizeof(InitData_6E01),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E11[]=//(福建)全局透明任务方案集
{
	0x6E110200,//OAD
  ADDR_6E11,//数据地址
	LENper_6E11,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E11,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E13[]=//(福建)全局透明具体任务集
{
	0x6E130200,//OAD
  ADDR_6E13,//数据地址
	LENper_6E13,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E13,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E17[]=//(福建)上报方案集
{
	0x6E170200,//OAD
  ADDR_6E17,//数据地址
	LENper_6E17,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E17,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E30[]=//(福建)主站穿透命令优先级
{
	0x6E300200,//OAD
  ADDR_6E30,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E30,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)InitData_6E30,//初始化原始数据指针
	sizeof(InitData_6E30),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_6E33[]=//(福建)主动上报事件上报策略
{
	0x6E330200,//OAD
  ADDR_6E33,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_6E33,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)InitData_6E33,//初始化原始数据指针
	sizeof(InitData_6E33),//初始化原始数据长度
};



#endif//#if (USER/100)==14//福建增补











