


//---冻结对象类 关联对象属性表---
__align(4) const u8 DL698_Attribute_50xx[]=
{
	6,//类型列表数
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	4,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//一个关联的对象属性∷=structure
	DataType_structure,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
//  关联对象属性描述符  OAD，
	DataType_OAD,
//  存储深度  long-unsigned
	DataType_long_unsigned,
//}
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5000[]=//瞬时冻结关联对象属性表
{
	0x50000300,//OAD
  ADDR_5000,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5000,//初始化原始数据指针
	sizeof(InitData_5000),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5001[]=//秒冻结关联对象属性表
{
	0x50010300,//OAD
  ADDR_5001,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5001,//初始化原始数据指针
	sizeof(InitData_5001),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5002[]=//分钟冻结关联对象属性表
{
	0x50020300,//OAD
  ADDR_5002,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5002,//初始化原始数据指针
	sizeof(InitData_5002),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5003[]=//小时冻结关联对象属性表
{
	0x50030300,//OAD
  ADDR_5003,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5003,//初始化原始数据指针
	sizeof(InitData_5003),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5004[]=//日冻结关联对象属性表
{
	0x50040300,//OAD
  ADDR_5004,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5004,//初始化原始数据指针
	sizeof(InitData_5004),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5005[]=//结算日冻结关联对象属性表
{
	0x50050300,//OAD
  ADDR_5005,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5005,//初始化原始数据指针
	sizeof(InitData_5005),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5006[]=//月冻结关联对象属性表
{
	0x50060300,//OAD
  ADDR_5006,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5006,//初始化原始数据指针
	sizeof(InitData_5006),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5007[]=//年冻结关联对象属性表
{
	0x50070300,//OAD
  ADDR_5007,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5007,//初始化原始数据指针
	sizeof(InitData_5007),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5008[]=//时区表切换冻结关联对象属性表
{
	0x50080300,//OAD
  ADDR_5008,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5008,//初始化原始数据指针
	sizeof(InitData_5008),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5009[]=//日时段表切换冻结关联对象属性表
{
	0x50090300,//OAD
  ADDR_5009,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5009,//初始化原始数据指针
	sizeof(InitData_5009),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_500A[]=//费率电价切换冻结关联对象属性表
{
	0x500A0300,//OAD
  ADDR_500A,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_500A,//初始化原始数据指针
	sizeof(InitData_500A),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_500B[]=//阶梯切换冻结关联对象属性表
{
	0x500B0300,//OAD
  ADDR_500B,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_500B,//初始化原始数据指针
	sizeof(InitData_500B),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_5011[]=//阶梯结算冻结关联对象属性表
{
	0x50110300,//OAD
  ADDR_5011,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_50xx,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_50xx,//数据类型列表指针
	(u32*)InitData_5011,//初始化原始数据指针
	sizeof(InitData_5011),//初始化原始数据长度
};








