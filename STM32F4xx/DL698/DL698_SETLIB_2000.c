



__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_230x_2[]=//总加配置表
{
	0x23010200,//OAD
  ADDR_230x_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_230x_2,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	(u32*)InitData_230x_2,//初始化原始数据指针
	sizeof(InitData_230x_2),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_230x_13[]=//总加组滑差时间周期
{
	0x23010D00,//OAD
  ADDR_230x_13,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_230x_13,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	(u32*)InitData_230x_13,//初始化原始数据指针
	sizeof(InitData_230x_13),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_230x_14[]=//总加组功控轮次配置
{
	0x23010E00,//OAD
  ADDR_230x_14,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_230x_14,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	(u32*)InitData_230x_14,//初始化原始数据指针
	sizeof(InitData_230x_14),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_230x_15[]=//总加组电控轮次配置
{
	0x23010F00,//OAD
  ADDR_230x_15,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_230x_15,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	(u32*)InitData_230x_15,//初始化原始数据指针
	sizeof(InitData_230x_15),//初始化原始数据长度
};




__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_240x_2[]=//脉冲计量-通信地址
{
	0x24010200,//OAD
  ADDR_240x_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_240x_2,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
#if NUMmax_IMPIN==0
	0,
	0,
#else
	(u32*)InitData_240x_2,//初始化原始数据指针
	sizeof(InitData_240x_2),//初始化原始数据长度
#endif
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_240x_3[]=//脉冲计量-互感器倍率
{
	0x24010300,//OAD
  ADDR_240x_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_240x_3,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
#if NUMmax_IMPIN==0
	0,
	0,
#else
	(u32*)InitData_240x_3,//初始化原始数据指针
	sizeof(InitData_240x_3),//初始化原始数据长度
#endif
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_240x_4[]=//脉冲计量-脉冲配置
{
	0x24010400,//OAD
  ADDR_240x_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_240x_4,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
#if NUMmax_IMPIN==0
	0,
	0,
#else
	(u32*)InitData_240x_4,//初始化原始数据指针
	sizeof(InitData_240x_4),//初始化原始数据长度
#endif
};








