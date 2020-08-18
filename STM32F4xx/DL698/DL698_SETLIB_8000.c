




__align(4) const u8 DL698_Attribute_8000_2[]=
{
	3,//类型列表数
//属性2（配置参数)∷=structure
	DataType_structure,
//{
//  继电器拉闸电流门限值    double-long-unsigned（单位：A，换算-4），
	DataType_double_long_unsigned,
//  超电流门限保护延时时间  long-unsigned（单位：分钟，换算0）
	DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8000_2[]=//遥控-属性2（配置参数)
{
	0x80000200,//OAD
  ADDR_8000_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8000_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8000_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8000_3[]=
{
	1,//类型列表数
//属性3（继电器输出状态，只读)∷=bit-string(SIZE(8))
	DataType_bit_string,
//继电器输出状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控跳闸输出状态，置“1”：跳闸状态；置“0”：合闸状态。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8000_3[]=//遥控-属性3（继电器输出状态，只读)
{
	0x80000300,//OAD
  ADDR_8000_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8000_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8000_3,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8000_4[]=
{
	1,//类型列表数
//属性4(告警状态，只读)∷=bit-string(SIZE(8))
	DataType_bit_string,
//告警状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控告警输出状态，置“1”：处于告警状态；置“0”：未处于告警状态。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8000_4[]=//遥控-属性4(告警状态，只读)
{
	0x80000400,//OAD
  ADDR_8000_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8000_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8000_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8000_5[]=
{
	1,//类型列表数
//属性5（命令状态，只读)∷=bit-string(SIZE(8))
	DataType_bit_string,
//继电器命令状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控跳闸命令输出状态，置“1”：跳闸命令；置“0”：合闸命令。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8000_5[]=//遥控-属性5（命令状态，只读)
{
	0x80000500,//OAD
  ADDR_8000_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8000_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8000_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8001_2[]=
{
	1,//类型列表数
//属性2（保电状态，只读）∷=enum{解除（0），保电（1），自动保电（2）}
	DataType_enum,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8001_2[]=//保电-属性2（保电状态，只读）
{
	0x80010200,//OAD
  ADDR_8001_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8001_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8001_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8001_3[]=
{
	1,//类型列表数
//属性3∷=long-unsigned
//允许与主站最大无通信时长（分钟），0表示不自动保电。
	DataType_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8001_3[]=//保电-允许与主站最大无通信时长（分钟）
{
	0x80010300,//OAD
  ADDR_8001_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8001_3,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8001_3,//数据类型列表指针
	(u32*)InitData_8001_3,//初始化原始数据指针
	sizeof(InitData_8001_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8001_4[]=
{
	1,//类型列表数
//属性4∷=long-unsigned
//上电自动保电时长（分钟），0表示上电不自动保电。
	DataType_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8001_4[]=//保电-上电自动保电时长（分钟）
{
	0x80010400,//OAD
  ADDR_8001_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8001_4,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8001_4,//数据类型列表指针
	(u32*)InitData_8001_4,//初始化原始数据指针
	sizeof(InitData_8001_4),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8001_5[]=
{
	5,//类型列表数
//属性5∷=array 自动保电时段
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//自动保电时段∷=structure
	DataType_structure,
//{
//起始时间（时）  unsigned，
	DataType_unsigned,
//结束时间（时）  unsigned
	DataType_unsigned,
//}
//时间段区间规则为前闭后开。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8001_5[]=//保电-自动保电时段
{
	0x80010500,//OAD
  ADDR_8001_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8001_5,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8001_5,//数据类型列表指针
	0,//(u32*)InitData_8001_5,//初始化原始数据指针
	0,//sizeof(InitData_8001_5),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8002_2[]=
{
	1,//类型列表数
//属性2（催费告警状态，只读）∷=enum{未告警(0)，告警(1)}
	DataType_enum,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8002_2[]=//属性2（催费告警状态，只读）
{
	0x80020200,//OAD
  ADDR_8002_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8002_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8002_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8002_3[]=
{
	3,//类型列表数
//属性3（催费告警参数）∷=structure 
	DataType_structure,
//{
//告警时段：octet-string(SIZE(3))， 
	DataType_octet_string,
//告警信息：visible-string(SIZE(1...200)) 
	DataType_visible_string,
//} 
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8002_3[]=//属性3（催费告警参数）
{
	0x80020300,//OAD
  ADDR_8002_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8002_3,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8002_3,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_8100_2[]=
{
	1,//类型列表数
//终端保安定值∷=long64（单位：W，换算：-1）
	DataType_long64,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8100_2[]=//终端保安定值
{
	0x81000200,//OAD
  ADDR_8100_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8100_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8100_2,//数据类型列表指针
	(u32*)InitData_8100_2,//初始化原始数据指针
	sizeof(InitData_8100_2),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8101_2[]=
{
	3,//类型列表数
//属性2（配置参数）∷=array unsigned
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8101_2[]=//终端功控时段
{
	0x81010200,//OAD
  ADDR_8101_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8101_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8101_2,//数据类型列表指针
	(u32*)InitData_8101_2,//初始化原始数据指针
	sizeof(InitData_8101_2),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8102_2[]=
{
	3,//类型列表数
//属性2（配置参数）∷=array unsigned
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8102_2[]=//功控告警时间
{
	0x81020200,//OAD
  ADDR_8102_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8102_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8102_2,//数据类型列表指针
	(u32*)InitData_8102_2,//初始化原始数据指针
	sizeof(InitData_8102_2),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8103_2[]=
{
	37,//类型列表数
//属性2（控制方案集）∷=array 时段功控配置单元
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//属性2∷=structure
	DataType_structure,
//{
//总加组对象  OI，
	DataType_OI,
//方案标识    bit-string(SIZE(8))，
	DataType_bit_string,
//第一套定值  PowerCtrlParam，
//PowerCtrlParam∷=structure
	DataType_structure,
//{
//	时段号          bit-string(SIZE(8))，
	DataType_bit_string,
//	时段1功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段2功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段3功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段4功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段5功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段6功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段7功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段8功控定值  long64（单位：W，换算：-1）
	DataType_long64,
//}
//第二套定值  PowerCtrlParam，
//PowerCtrlParam∷=structure
	DataType_structure,
//{
//	时段号          bit-string(SIZE(8))，
	DataType_bit_string,
//	时段1功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段2功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段3功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段4功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段5功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段6功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段7功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段8功控定值  long64（单位：W，换算：-1）
	DataType_long64,
//}
//第三套定值  PowerCtrlParam，
//PowerCtrlParam∷=structure
	DataType_structure,
//{
//	时段号          bit-string(SIZE(8))，
	DataType_bit_string,
//	时段1功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段2功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段3功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段4功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段5功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段6功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段7功控定值  long64（单位：W，换算：-1），
	DataType_long64,
//	时段8功控定值  long64（单位：W，换算：-1）
	DataType_long64,
//}
//时段功控定值浮动系数  integer（单位：%）
	DataType_integer,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8103_2[]=//时段功控-控制方案集
{
	0x81030200,//OAD
  ADDR_8103_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8103_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8103_2,//数据类型列表指针
	(u32*)InitData_8103_2,//初始化原始数据指针
	sizeof(InitData_8103_2),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8103_3[]=
{
	5,//类型列表数
//控制投入状态∷=array 一个总加组控制投入状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制投入状态∷=structure
	DataType_structure,
//{
//  总加组对象  OI，
	DataType_OI,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
//}
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8103_3[]=//时段功控-控制投入状态
{
	0x81030300,//OAD
  ADDR_8103_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8103_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8103_3,//数据类型列表指针
	(u32*)InitData_8103_3,//初始化原始数据指针
	sizeof(InitData_8103_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8103_4[]=
{
	5,//类型列表数
//控制输出状态∷=array  一个总加组控制输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  控制输出状态  bit-string(SIZE(8))
	DataType_bit_string,
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8103_4[]=//时段功控-控制输出状态
{
	0x81030400,//OAD
  ADDR_8103_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8103_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8103_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8103_5[]=
{
	5,//类型列表数
//越限告警状态∷=array 一个总加组告警输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组告警输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  告警输出状态  enum
	DataType_enum,
//{
//未告警（0），告警（1）
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8103_5[]=//时段功控-越限告警状态
{
	0x81030500,//OAD
  ADDR_8103_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8103_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8103_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8104_2[]=
{
	8,//类型列表数
//array
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//属性2∷=structure
	DataType_structure,
//{
//总加组对象    OI，
	DataType_OI,
//厂休控定值    long64（单位：W，换算：-1），
	DataType_long64,
//限电起始时间  date_time_s（年=FFFFH，月=FFH，日=FFH），
	DataType_date_time_s,
//限电延续时间  long-unsigned（单位：分钟），
	DataType_long_unsigned,
//每周限电日    bit-string(SIZE(8)) 
	DataType_bit_string,
//}
//每周限电日：D1～D7表示星期一～星期日，D0=0。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8104_2[]=//厂休控配置单元
{
	0x81040200,//OAD
  ADDR_8104_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8104_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8104_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8104_3[]=
{
	5,//类型列表数
//控制投入状态∷=array 一个总加组控制投入状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制投入状态∷=structure
	DataType_structure,
//{
//  总加组对象  OI，
	DataType_OI,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8104_3[]=//厂休控-控制投入状态
{
	0x81040300,//OAD
  ADDR_8104_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8104_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8104_3,//数据类型列表指针
	(u32*)InitData_8103_3,//初始化原始数据指针
	sizeof(InitData_8103_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8104_4[]=
{
	5,//类型列表数
//控制输出状态∷=array  一个总加组控制输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  控制输出状态  bit-string(SIZE(8))
	DataType_bit_string,
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8104_4[]=//厂休控-控制输出状态
{
	0x81040400,//OAD
  ADDR_8104_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8104_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8104_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8104_5[]=
{
	5,//类型列表数
//越限告警状态∷=array 一个总加组告警输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组告警输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  告警输出状态  enum
	DataType_enum,
//{
//未告警（0），告警（1）
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8104_5[]=//厂休控-越限告警状态
{
	0x81040500,//OAD
  ADDR_8104_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8104_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8104_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8105_2[]=
{
	7,//类型列表数
//array
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//属性2∷=structure
	DataType_structure,
//{
//总加组对象      OI，
	DataType_OI,
//报停起始时间    date_time_s（时=FFH，分=FFH），
	DataType_date_time_s,
//报停结束时间    date_time_s（时=FFH，分=FFH），
	DataType_date_time_s,
//报停控功率定值  long64（单位：W，换算：-1）
	DataType_long64,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8105_2[]=//营业报停控配置单元
{
	0x81050200,//OAD
  ADDR_8105_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8105_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8105_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8105_3[]=
{
	5,//类型列表数
//控制投入状态∷=array 一个总加组控制投入状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制投入状态∷=structure
	DataType_structure,
//{
//  总加组对象  OI，
	DataType_OI,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8105_3[]=//营业报停控-控制投入状态
{
	0x81050300,//OAD
  ADDR_8105_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8105_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8105_3,//数据类型列表指针
	(u32*)InitData_8103_3,//初始化原始数据指针
	sizeof(InitData_8103_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8105_4[]=
{
	5,//类型列表数
//控制输出状态∷=array  一个总加组控制输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  控制输出状态  bit-string(SIZE(8))
	DataType_bit_string,
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8105_4[]=//营业报停控-控制输出状态
{
	0x81050400,//OAD
  ADDR_8105_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8105_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8105_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8105_5[]=
{
	5,//类型列表数
//越限告警状态∷=array 一个总加组告警输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组告警输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  告警输出状态  enum
	DataType_enum,
//{
//未告警（0），告警（1）
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8105_5[]=//营业报停控-越限告警状态
{
	0x81050500,//OAD
  ADDR_8105_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8105_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8105_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8106_3[]=
{
	5,//类型列表数
//控制投入状态∷=array 一个总加组控制投入状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制投入状态∷=structure
	DataType_structure,
//{
//  总加组对象  OI，
	DataType_OI,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8106_3[]=//当前功率下浮控-控制投入状态
{
	0x81060300,//OAD
  ADDR_8106_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8106_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8106_3,//数据类型列表指针
	(u32*)InitData_8103_3,//初始化原始数据指针
	sizeof(InitData_8103_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8106_4[]=
{
	5,//类型列表数
//控制输出状态∷=array  一个总加组控制输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  控制输出状态  bit-string(SIZE(8))
	DataType_bit_string,
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8106_4[]=//当前功率下浮控-控制输出状态
{
	0x81060400,//OAD
  ADDR_8106_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8106_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8106_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8106_5[]=
{
	5,//类型列表数
//越限告警状态∷=array 一个总加组告警输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组告警输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  告警输出状态  enum
	DataType_enum,
//{
//未告警（0），告警（1）
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8106_5[]=//当前功率下浮控-越限告警状态
{
	0x81060500,//OAD
  ADDR_8106_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8106_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8106_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8107_2[]=
{
	11,//类型列表数
//array
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//属性2∷=structure
	DataType_structure,
//{
//	总加组对象      OI，
	DataType_OI,
//购电单号        double-long-unsigned，
	DataType_double_long_unsigned,
//追加/刷新标识   enum{追加（0），刷新（1）}，
	DataType_enum,
//购电类型        enum{电量（0），电费（1）}，
	DataType_enum,
//购电量（费）值  long64（单位：kWh/元， 换算：-4），
	DataType_long64,
//	报警门限值      long64（单位：kWh/元，换算：-4），
	DataType_long64,
//	跳闸门限值      long64（单位：kWh/元，换算：-4）
	DataType_long64,
//	购电控模式      enum{本地模式（0），远程模式（1）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8107_2[]=//购电控配置单元
{
	0x81070200,//OAD
  ADDR_8107_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8107_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8107_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8107_3[]=
{
	5,//类型列表数
//控制投入状态∷=array 一个总加组控制投入状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制投入状态∷=structure
	DataType_structure,
//{
//  总加组对象  OI，
	DataType_OI,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8107_3[]=//购电控-控制投入状态
{
	0x81070300,//OAD
  ADDR_8107_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8107_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8107_3,//数据类型列表指针
	(u32*)InitData_8103_3,//初始化原始数据指针
	sizeof(InitData_8103_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8107_4[]=
{
	5,//类型列表数
//控制输出状态∷=array  一个总加组控制输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  控制输出状态  bit-string(SIZE(8))
	DataType_bit_string,
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8107_4[]=//购电控-控制输出状态
{
	0x81070400,//OAD
  ADDR_8107_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8107_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8107_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8107_5[]=
{
	5,//类型列表数
//越限告警状态∷=array 一个总加组告警输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组告警输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  告警输出状态  enum
	DataType_enum,
//{
//未告警（0），告警（1）
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8107_5[]=//购电控-越限告警状态
{
	0x81070500,//OAD
  ADDR_8107_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8107_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8107_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8108_2[]=
{
	7,//类型列表数
//array
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//属性2∷=structure
	DataType_structure,
//{
//	总加组对象      OI，
	DataType_OI,
//	月电量控定值    long64（单位：kWh，换算：-4），
	DataType_long64,
//	报警门限值系数  unsigned（单位：%），
	DataType_unsigned,
//	月电量控定值浮动系数  integer（单位：%）
	DataType_integer,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8108_2[]=//月电控配置单元
{
	0x81080200,//OAD
  ADDR_8108_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8108_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8108_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8108_3[]=
{
	5,//类型列表数
//控制投入状态∷=array 一个总加组控制投入状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制投入状态∷=structure
	DataType_structure,
//{
//  总加组对象  OI，
	DataType_OI,
//投入状态  enum{未投入（0），投入（1）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8108_3[]=//月电控-控制投入状态
{
	0x81080300,//OAD
  ADDR_8108_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8108_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8108_3,//数据类型列表指针
	(u32*)InitData_8103_3,//初始化原始数据指针
	sizeof(InitData_8103_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8108_4[]=
{
	5,//类型列表数
//控制输出状态∷=array  一个总加组控制输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组控制输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  控制输出状态  bit-string(SIZE(8))
	DataType_bit_string,
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8108_4[]=//月电控-控制输出状态
{
	0x81080400,//OAD
  ADDR_8108_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8108_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8108_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_8108_5[]=
{
	5,//类型列表数
//越限告警状态∷=array 一个总加组告警输出状态
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个总加组告警输出状态∷=structure
	DataType_structure,
//{
//  总加组对象    OI，
	DataType_OI,
//  告警输出状态  enum
	DataType_enum,
//{
//未告警（0），告警（1）
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_8108_5[]=//月电控-越限告警状态
{
	0x81080500,//OAD
  ADDR_8108_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_8108_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_8108_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

























