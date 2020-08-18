

//终端数据

__align(4) const u8 InitData_2131[]=
{
//属性2（电压合格率数据）∷=structure
	DataType_structure,
	2,
//{
//  当日电压合格率         电压合格率，
//电压合格率∷=structure
	DataType_structure,
	5,
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
	DataType_double_long_unsigned,
	0,0,0,0,
//电压合格率   long-unsigned（单位：%，换算：-2），
	DataType_long_unsigned,
	0,0,
//电压超限率   long-unsigned（单位：%，换算：-2），
	DataType_long_unsigned,
	0,0,
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
	DataType_double_long_unsigned,
	0,0,0,0,
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
	DataType_double_long_unsigned,
	0,0,0,0,
//}
//  当月电压合格率         电压合格率
//电压合格率∷=structure
	DataType_structure,
	5,
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
	DataType_double_long_unsigned,
	0,0,0,0,
//电压合格率   long-unsigned（单位：%，换算：-2），
	DataType_long_unsigned,
	0,0,
//电压超限率   long-unsigned（单位：%，换算：-2），
	DataType_long_unsigned,
	0,0,
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
	DataType_double_long_unsigned,
	0,0,0,0,
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
	DataType_double_long_unsigned,
	0,0,0,0,
//}
//}
};

__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2131[]=//当月A相电压合格率
{
	0x21310200,//OAD
  ADDR_2131,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2131,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2131,//初始化原始数据指针
	sizeof(InitData_2131),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2132[]=//当月B相电压合格率
{
	0x21320200,//OAD
  ADDR_2132,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2132,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2131,//初始化原始数据指针
	sizeof(InitData_2131),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2133[]=//当月C相电压合格率
{
	0x21330200,//OAD
  ADDR_2133,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2133,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2131,//初始化原始数据指针
	sizeof(InitData_2131),//初始化原始数据长度
};

__align(4) const u8 InitData_2140[]=
{
//最大功率及发生时间∷=structure
	DataType_structure,
	2,
//{
//  最大功率值 double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//  发生时间   date_time_s
	DataType_date_time_s,
	0,0,0,0,0,0,0,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2140[]=//日最大有功功率及发生时间
{
	0x21400200,//OAD
  ADDR_2140,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2140,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2140,//初始化原始数据指针
	sizeof(InitData_2140),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2141[]=//月最大有功功率及发生时间
{
	0x21410200,//OAD
  ADDR_2141,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2141,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2140,//初始化原始数据指针
	sizeof(InitData_2140),//初始化原始数据长度
};



__align(4) const u8 InitData_2200[]=
{
//数值∷=structure
	DataType_structure,
	2,
//{
//当日通信流量  double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//当月通信流量  double-long-unsigned
	DataType_double_long_unsigned,
	0,0,0,0,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2200[]=//通信流量
{
	0x22000200,//OAD
  ADDR_2200,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2200,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2200,//初始化原始数据指针
	sizeof(InitData_2200),//初始化原始数据长度
};

__align(4) const u8 InitData_2203[]=
{
//数值∷=structure
	DataType_structure,
	2,
//{
//日供电累计时间  double-long-unsigned，
	DataType_double_long_unsigned,
	0,0,0,0,
//月供电累计时间  double-long-unsigned
	DataType_double_long_unsigned,
	0,0,0,0,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2203[]=//供电时间
{
	0x22030200,//OAD
  ADDR_2203,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2203,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2203,//初始化原始数据指针
	sizeof(InitData_2203),//初始化原始数据长度
};

__align(4) const u8 InitData_2204[]=
{
//数值∷=structure
	DataType_structure,
	2,
//{
//日复位累计次数  long-unsigned
	DataType_long_unsigned,
	0,0,
//月复位累计次数  long-unsigned
	DataType_long_unsigned,
	0,0,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_GETLIB_2204[]=//复位次数
{
	0x22040200,//OAD
  ADDR_2204,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_2204,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_2204,//初始化原始数据指针
	sizeof(InitData_2204),//初始化原始数据长度
};




















