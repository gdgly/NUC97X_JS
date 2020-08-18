




__align(4) const u8 DL698_Attribute_F100[]=
{
	1,//类型列表数
	DataType_Data,
};
__align(4) const u8 DL698_Attribute_F002_4[]=
{
	5,//类型列表数
//属性4（服务器信息）∷=structure
	DataType_structure,
//{
//IP地址  octet-string，
	DataType_octet_string,
//端口    long-unsigned，
	DataType_long_unsigned,
//用户名  visible-string，
	DataType_visible_string,
//密码    visible-string
	DataType_visible_string,
//}
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F002_4[]=//FTP服务器信息
{
	0xF0020400,//OAD
  ADDR_F002_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F002_4,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F002_4,//数据类型列表指针
	(u32*)InitData_F002_4,//初始化原始数据指针
	sizeof(InitData_F002_4),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_2[]=//ESAM序列号
{
	0xF1000200,//OAD
  ADDR_F100_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_3[]=//ESAM版本号
{
	0xF1000300,//OAD
  ADDR_F100_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_3,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_4[]=//对称密钥版本
{
	0xF1000400,//OAD
  ADDR_F100_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_4,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度	
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_5[]=//会话时效门限
{
	0xF1000500,//OAD
  ADDR_F100_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_6[]=//会话时效剩余时间
{
	0xF1000600,//OAD
  ADDR_F100_6,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_6,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_7[]=//当前计数器
{
	0xF1000700,//OAD
  ADDR_F100_7,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_7,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_8[]=//证书版本
{
	0xF1000800,//OAD
  ADDR_F100_8,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_8,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_9[]=//终端证书序列号
{
	0xF1000900,//OAD
  ADDR_F100_9,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_9,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_10[]=//终端证书
{
	0xF1000A00,//OAD
  ADDR_F100_10,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_10,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_11[]=//主站证书序列号
{
	0xF1000B00,//OAD
  ADDR_F100_11,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_11,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_12[]=//主站证书
{
	0xF1000C00,//OAD
  ADDR_F100_12,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_12,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F100_13[]=//ESAM安全存储对象列表
{
	0xF1000D00,//OAD
  ADDR_F100_13,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F100_13,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//(u32*)DL698_Attribute_F100,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_F101_2[]=
{
	1,//类型列表数
//属性2（安全模式选择）∷=enum
	DataType_enum,
//{
//不启用安全模式参数（0），
//启用安全模式参数  （1）
//}
//安全模式选择意义：0—不启用安全模式参数、默认安全模式参数，1—启用安全模式参数、默认安全模式参数。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F101_2[]=//安全模式参数
{
	0xF1010200,//OAD
  ADDR_F101_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F101_2,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F101_2,//数据类型列表指针
	(u32*)InitData_F101_2,//初始化原始数据指针
	sizeof(InitData_F101_2),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F101_3[]=
{
	5,//类型列表数
//属性3（显式安全模式参数）∷=array 安全模式参数
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//安全模式参数∷=structure
	DataType_structure,
//{
//  对象标识   OI，
	DataType_OI,
//  安全模式   long-unsigned
	DataType_long_unsigned,
//}
//属性3为显式安全模式参数（设置值），如果对象安全性不在属性3中，则按默认安全模式参数。如果同一对象安全性在安全模式参数、默认安全模式参中均有说明，则按显式安全模式参数执行。安全模式参数、默认安全参数具体定义见附录F，安全模式定义见附录F中表F.1。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F101_3[]=//显式安全模式参数
{
	0xF1010300,//OAD
  ADDR_F101_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F101_3,//FLASH预留的最大长度
	1+4,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F101_3,//数据类型列表指针
	(u32*)InitData_F101_3,//初始化原始数据指针
	sizeof(InitData_F101_3),//初始化原始数据长度	
};

__align(4) const u8 DL698_Attribute_F200_2[]=
{
	6,//类型列表数
//属性2（设备对象列表，只读）∷=array 端口
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//端口∷=structure
	DataType_structure,
//{
//  端口描述符 visible-string，
	DataType_visible_string,
//端口参数   COMDCB，
	DataType_COMDCB,
//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F200_2[]=//RS232
{
	0xF2000200,//OAD
  ADDR_F200_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F200_2,//FLASH预留的最大长度
	5,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F200_2,//数据类型列表指针
	(u32*)InitData_F200_2,//初始化原始数据指针
	sizeof(InitData_F200_2),//初始化原始数据长度		
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F200_3[]=//RS232
{
	0xF2000300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	1,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F201_2[]=
{
	6,//类型列表数
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//端口∷=structure
	DataType_structure,
//{
//  端口描述符 visible-string，
	DataType_visible_string,
//端口参数   COMDCB，
	DataType_COMDCB,
//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F201_2[]=//RS485
{
	0xF2010200,//OAD
  ADDR_F201_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F201_2,//FLASH预留的最大长度
	5,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F201_2,//数据类型列表指针
	(u32*)InitData_F201_2,//初始化原始数据指针
	sizeof(InitData_F201_2),//初始化原始数据长度		
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F201_3[]=//RS485
{
	0xF2010300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	NUMmax_RS485,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F202_2[]=
{
	5,//类型列表数
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//端口∷=structure
	DataType_structure,
//{
//  端口描述符 visible-string，
	DataType_visible_string,
//端口参数   COMDCB，
	DataType_COMDCB,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F202_2[]=//红外
{
	0xF2020200,//OAD
  ADDR_F202_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F202_2,//FLASH预留的最大长度
	5,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F202_2,//数据类型列表指针
	(u32*)InitData_F202_2,//初始化原始数据指针
	sizeof(InitData_F202_2),//初始化原始数据长度		
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F202_3[]=//红外
{
	0xF2020300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	1,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F203_2[]=
{
	5,//类型列表数
//属性2（设备对象列表，只读）∷=array 开关量单元
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//开关量单元∷=structure
	DataType_structure,
//{
//状态ST  unsigned，
	DataType_unsigned,
//变位CD  unsigned
	DataType_unsigned,
//}
//状态ST——0：“分”状态；1：“合”状态。
//变位CD——0：自前次遥信传送后无状态变化；1：自前次遥信传送后至少有一次状态变化。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F203_2[]=//开关量单元-设备对象列表
{
	0xF2030200,//OAD
  ADDR_F203_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F203_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F203_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F203_3[]=//开关量单元-设备对象数
{
	0xF2030300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	NUMmax_SWITCHIN,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_F203_4[]=
{
	3,//类型列表数
//属性4∷=structure
	DataType_structure,
//{
//开关量接入标志bit-string(SIZE（8）)
	DataType_bit_string,
//（
//bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”：接入，置“0”：未接入。
//），
//开关量属性标志bit-string(SIZE（8）)
	DataType_bit_string,
//（
//  bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”常开触点。置“0”：常闭触点。
//）
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F203_4[]=//开关量接入与属性标志
{
	0xF2030400,//OAD
  ADDR_F203_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F203_4,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F203_4,//数据类型列表指针
	(u32*)InitData_F203_4,//初始化原始数据指针
	sizeof(InitData_F203_4),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F204_2[]=
{
	3,//类型列表数
//属性2（设备对象列表，只读）∷=array 直流模拟量
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//直流模拟量∷=double-long
	DataType_double_long,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F204_2[]=//直流模拟量
{
	0xF2040200,//OAD
  ADDR_F204_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F204_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F204_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F204_3[]=//直流模拟量-设备对象数
{
	0xF2040300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	NUMmax_DC,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_F204_4[]=
{
	6,//类型列表数
//属性2（设备对象列表，只读）∷=array 直流模拟量
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//直流模拟量配置∷=structure
	DataType_structure,
//{
//  量程起始值  double-long，
	DataType_double_long,
//  量程结束值  double-long，
	DataType_double_long,
//  换算及单位  Scaler_Unit
	DataType_Scaler_Unit,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F204_4[]=//直流模拟量
{
	0xF2040400,//OAD
  ADDR_F204_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F204_4,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F204_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};




__align(4) const u8 DL698_Attribute_F205_2[]=
{
	7,//类型列表数
//属性2（设备对象列表，只读）∷=array 继电器单元
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//继电器单元∷=structure
	DataType_structure,
//{
//  描述符    visible-string，
	DataType_visible_string,
//当前状态  enum{未输出（0），输出（1）}，
	DataType_enum,
//开关属性  enum{脉冲式（0），保持式（1）}，
	DataType_enum,
//接线状态  enum{接入（0），未接入（1) }
	DataType_enum,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F205_2[]=//继电器输出
{
	0xF2050200,//OAD
  ADDR_F205_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F205_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F205_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F205_3[]=//继电器输出-设备对象数
{
	0xF2050300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	NUMmax_CR,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_F206_2[]=
{
	3,//类型列表数
//属性2（设备对象列表，只读）∷=array 告警输出
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//告警输出∷=enum
	DataType_enum,
//{
//未输出（0），输出（1）
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F206_2[]=//告警输出
{
	0xF2060200,//OAD
  ADDR_F206_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F206_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F206_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F206_3[]=//告警输出-设备对象数
{
	0xF2060300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	0,//设备对象数//FLASH预留的最大长度
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	1,//设备对象数//FLASH预留的最大长度
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	1,//设备对象数//FLASH预留的最大长度
#endif
#if ((Project/100)!=2)&&((Project/100)!=3)&&((Project/100)!=4)
	0,//设备对象数//FLASH预留的最大长度
#endif
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_F206_4[]=
{
	5,//类型列表数
//属性4∷=array 允许告警时段
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//允许告警时段∷=structure
	DataType_structure,
//{
//  起始时间  Time，
	DataType_time,
//  结束时间  Time
	DataType_time,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F206_4[]=//告警时段
{
	0xF2060400,//OAD
  ADDR_F206_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F206_4,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F206_4,//数据类型列表指针
	(u32*)InitData_F206_4,//初始化原始数据指针
	sizeof(InitData_F206_4),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F207_2[]=
{
	3,//类型列表数
//属性2（设备对象列表，只读）∷=array 端子功能
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//端子功能∷=enum
	DataType_enum,
//{
//     秒脉冲输出（0），
//     需量周期  （1），
//     时段投切  （2）
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F207_2[]=//多功能端子
{
	0xF2070200,//OAD
  ADDR_F207_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F207_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F207_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F207_3[]=//多功能端子-设备对象数
{
	0xF2070300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	1,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F209_2[]=
{
	10,//类型列表数
//属性2（设备对象列表，只读）∷=array 本地通信模块单元
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//本地通信模块单元∷=structure
	DataType_structure,
//{
//端口描述符   visible-string，
	DataType_visible_string,
//通信参数     COMDCB，
	DataType_COMDCB,
//版本信息     VersionInfo
//}
//VersionInfo∷=structure
	DataType_structure,
//{
//  厂商代码    visible-string(SIZE(2))，
	DataType_visible_string,
//  芯片代码    visible-string(SIZE(2))，
	DataType_visible_string,
//  版本日期     Date，
	DataType_date,
//  软件版本     long-unsigned
	DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F209_2[]=//载波/微功率无线接口
{
	0xF2090200,//OAD
  ADDR_F209_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F209_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F209_2,//数据类型列表指针
	(u32*)InitData_F209_2,//初始化原始数据指针
	sizeof(InitData_F209_2),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F209_3[]=//载波/微功率无线接口-设备对象数
{
	0xF2090300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	1,//设备对象数//FLASH预留的最大长度
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	0,//设备对象数//FLASH预留的最大长度
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	1,//设备对象数//FLASH预留的最大长度
#endif
#if ((Project/100)!=2)&&((Project/100)!=3)&&((Project/100)!=4)
	0,//设备对象数//FLASH预留的最大长度
#endif
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_F209_6[]=
{
	1,//类型列表数
	DataType_TI,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F209_6[]=//载波/微功率无线接口:从节点对象列表、网络拓扑信息、多网信息等更新周期
{
	0xF2090600,//OAD
  ADDR_F209_6,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F209_6,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F209_6,//数据类型列表指针
	(u32*)InitData_F209_6,//初始化原始数据指针
	sizeof(InitData_F209_6),//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_F209_9[]=
{
	1,//类型列表数
	DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F209_9[]=//载波/微功率无线接口:宽带载波频段序号
{
	0xF2090900,//OAD
  ADDR_F209_9,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F209_9,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F209_9,//数据类型列表指针
	(u32*)InitData_F209_9,//初始化原始数据指针
	sizeof(InitData_F209_9),//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_F209_11[]=
{
	1,//类型列表数
	DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F209_11[]=//台区识别启停标记
{
	0xF2090B00,//OAD
	ADDR_F209_11,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F209_11,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F209_11,//数据类型列表指针
	(u32*)InitData_F209_11,//初始化原始数据指针
	sizeof(InitData_F209_11),//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_F209_22[]=
{
	1,//类型列表数
	DataType_enum,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F209_22[]=//拒绝节点上报使能标志
{
	0xF2091600,//OAD
	ADDR_F209_22,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F209_22,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F209_22,//数据类型列表指针
	(u32*)InitData_F209_22,//初始化原始数据指针
	sizeof(InitData_F209_22),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F20A_2[]=
{
	3,//类型列表数
//属性2（设备对象列表，只读）∷=array 脉冲输入端口描述符
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//脉冲输入端口描述符∷=visible-string
	DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F20A_2[]=//脉冲输入设备
{
	0xF20A0200,//OAD
  ADDR_F20A_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F20A_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F20A_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F20A_3[]=//脉冲输入设备-设备对象数
{
	0xF20A0300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	NUMmax_IMPIN,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_F20B_2[]=
{
	5,//类型列表数
//属性2（设备对象列表，只读）∷=array 蓝牙模块
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//蓝牙模块∷=structure
	DataType_structure,
//{
//端口描述符   visible-string，
	DataType_visible_string,
//通信参数     COMDCB
	DataType_COMDCB,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F20B_2[]=//蓝牙
{
	0xF20B0200,//OAD
  ADDR_F20B_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_F20B_2,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_F20B_2,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_F20B_3[]=//蓝牙-设备对象数
{
	0xF20B0300,//OAD
  0,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//设备对象数//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};














