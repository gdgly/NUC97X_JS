




//---通信类---
__align(4) const u8 DL698_Attribute_4001[]=
{
	1,//类型列表数
//属性2∷=TSA
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4001[]=//通信地址
{
	0x40010200,//OAD
  ADDR_4001,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4001,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4001,//数据类型列表指针
	(u32*)InitData_4001,//初始化原始数据指针
	sizeof(InitData_4001),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4002[]=
{
	1,//类型列表数
//属性2∷=octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4002[]=//表号
{
	0x40020200,//OAD
  ADDR_4002,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4002,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4002,//数据类型列表指针
	(u32*)InitData_4002,//初始化原始数据指针
	sizeof(InitData_4002),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4003[]=
{
	1,//类型列表数
//属性2∷=octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4003[]=//客户编号
{
	0x40030200,//OAD
  ADDR_4003,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4003,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4003,//数据类型列表指针
	(u32*)InitData_4003,//初始化原始数据指针
	sizeof(InitData_4003),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4005[]=
{
	3,//类型列表数
//属性2∷=array 
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4005[]=//组地址
{
	0x40050200,//OAD
  ADDR_4005,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4005,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4005,//数据类型列表指针
	(u32*)InitData_4005,//初始化原始数据指针
	sizeof(InitData_4005),//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_4400_4[]=
{
	1,//类型列表数
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4400_4[]=//当前连接的客户机地址(仅使有库列表,无其他实际指针)
{
	0x44000400,//OAD
  ADDR_DATABUFF,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	2,//LENmax_4400,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4400_4,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4401[]=
{
	1,//类型列表数
//属性2（只写）∷=visible-string
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4401[]=//应用连接认证密码
{
	0x44010200,//OAD
  ADDR_4401,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4401,//FLASH预留的最大长度
	6,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4401,//数据类型列表指针
	(u32*)InitData_4401,//初始化原始数据指针
	sizeof(InitData_4401),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4500_2[]=
{
	15,//类型列表数
//通信配置∷=structure
		DataType_structure,
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
		DataType_enum,
//在线方式      enum{永久在线（0），被动激活（1）}，
		DataType_enum,
//连接方式      enum{TCP（0），UDP（1）}，
		DataType_enum,
//连接应用方式  enum{主备模式（0），多连接模式（1）}，
		DataType_enum,
//侦听端口列表  array long-unsigned，
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
		DataType_long_unsigned,
//APN            visible-string，
		DataType_visible_string,
//用户名         visible-string，
		DataType_visible_string,
//密码           visible-string，
		DataType_visible_string,
//代理服务器地址 octet-string，
		DataType_octet_string,
//代理端口            long-unsigned，
		DataType_long_unsigned,
//超时时间及重发次数  unsigned
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
		DataType_unsigned,
//心跳周期(秒)  long-unsigned
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_2[]=//公网通信模块1;2通信配置
{
	0x45000200,//OAD
  ADDR_4500_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_2,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_2,//数据类型列表指针
	(u32*)InitData_4500_2,//初始化原始数据指针
	sizeof(InitData_4500_2),//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_3[]=
{
	5,//类型列表数
//主站通信参数表∷=array 主站通信参数
		DataType_array,
		3,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//主站通信参数：：=structure
		DataType_structure,
//{
//IP地址   octet-string，
		DataType_octet_string,
//端口     long-unsigned，
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_3[]=//公网通信模块1;3主站通信参数表
{
	0x45000300,//OAD
  ADDR_4500_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_3,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_3,//数据类型列表指针
	(u32*)InitData_4500_3,//初始化原始数据指针
	sizeof(InitData_4500_3),//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_4[]=
{
	8,//类型列表数
//短信通信参数：：=structure
		DataType_structure,
//{
//短信中心号码     visible-string(SIZE(16))，
		DataType_visible_string,
//主站号码         array visible-string(SIZE(16))，
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组烽=0时跳过元素个数用
		DataType_visible_string,
//  短信通知目的号码 array visible-string(SIZE(16))
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组烽=0时跳过元素个数用
		DataType_visible_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_4[]=//公网通信模块1;4短信通信参数
{
	0x45000400,//OAD
  ADDR_4500_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_4,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_4,//数据类型列表指针
	(u32*)InitData_4500_4,//初始化原始数据指针
	sizeof(InitData_4500_4),//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_5[]=
{
	7,//类型列表数
//版本信息∷=structure
		DataType_structure,
//{
//厂商代码       visible-string(SIZE (4))，
		DataType_visible_string,
//软件版本号     visible-string(SIZE (4))，
		DataType_visible_string,
//软件版本日期   visible-string(SIZE (6))，
		DataType_visible_string,
//硬件版本号     visible-string(SIZE (4))，
		DataType_visible_string,
//硬件版本日期   visible-string(SIZE (6))，
		DataType_visible_string,
//厂家扩展信息   visible-string(SIZE (8))
		DataType_visible_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_5[]=//公网通信模块1;5版本信息
{
	0x45000500,//OAD
  ADDR_4500_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_5,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_5,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_6[]=
{
	2+1,//类型列表数
//数据格式 array visible-string
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_6[]=//公网通信模块1;6支持规约列表
{
	0x45000600,//OAD
  ADDR_4500_6,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_6,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_6,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_7[]=
{
	1,//类型列表数
//visible-string(SIZE(20))
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_7[]=//公网通信模块1;7 SIM卡的ICCID
{
	0x45000700,//OAD
  ADDR_4500_7,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_7,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_7,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_8[]=
{
	1,//类型列表数
//visible-string(SIZE(20))
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_8[]=//公网通信模块1;8 IMSI
{
	0x45000800,//OAD
  ADDR_4500_8,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_8,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_8,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_9[]=
{
	1,//类型列表数
//long，单位：dBm
		DataType_long,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_9[]=//公网通信模块1;9 信号强度
{
	0x45000900,//OAD
  ADDR_4500_9,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_9,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_9,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_10[]=
{
	1,//类型列表数
//visible-string(SIZE(16))
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_10[]=//公网通信模块1;10 SIM卡号码
{
	0x45000A00,//OAD
  ADDR_4500_10,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_10,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_10,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4500_11[]=
{
	1,//类型列表数
//octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_11[]=//公网通信模块1;11 拨号IP
{
	0x45000B00,//OAD
  ADDR_4500_11,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4500_11,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_11,//数据类型列表指针
	(u32*)InitData_4500_11,//初始化原始数据指针
	sizeof(InitData_4500_11),//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_4510_2[]=
{
	11,//类型列表数
//通信配置∷=structure
		DataType_structure,
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
		DataType_enum,
//连接方式  enum{TCP（0），UDP（1）}，
		DataType_enum,
//连接应用方式enum{主备模式（0），多连接模式（1）}
		DataType_enum,
//侦听端口列表array long-unsigned，
		DataType_array,
		1,
		DataType_long_unsigned,
//代理服务器地址  octet-string，
		DataType_octet_string,
//代理端口  long-unsigned，
		DataType_long_unsigned,
//超时时间及重发次数  unsigned
		DataType_unsigned,
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
//心跳周期(秒)  long-unsigned
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_2[]=//以太网通信模块1;2通信配置
{
	0x45100200,//OAD
  ADDR_4510_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4510_2,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4510_2,//数据类型列表指针
	(u32*)InitData_4510_2,//初始化原始数据指针
	sizeof(InitData_4510_2),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_3[]=//以太网通信模块1;3主站通信参数表
{
	0x45100300,//OAD
  ADDR_4510_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4510_3,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4500_3,//数据类型列表指针
	(u32*)InitData_4510_3,//初始化原始数据指针
	sizeof(InitData_4510_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4510_4[]=
{
	7,//类型列表数
//网络配置∷=structure
		DataType_structure,
//{
//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}，
		DataType_enum,
//IP地址       octet-string，
		DataType_octet_string,
//子网掩码     octet-string，
		DataType_octet_string,
//网关地址     octet-string，
		DataType_octet_string,
//PPPoE用户名  octet-string，
		DataType_visible_string,
//PPPoE密码    octet-string
		DataType_visible_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_4[]=//以太网通信模块1;4网络配置
{
	0x45100400,//OAD
  ADDR_4510_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4510_4,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4510_4,//数据类型列表指针
	(u32*)InitData_4510_4,//初始化原始数据指针
	sizeof(InitData_4510_4),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4510_5[]=
{
	1,//类型列表数
//MAC地址∷=octet-string 
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_5[]=//以太网通信模块1;5 MAC地址
{
	0x45100500,//OAD
  ADDR_4510_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4510_5,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4510_5,//数据类型列表指针
	(u32*)InitData_4510_5,//初始化原始数据指针
	sizeof(InitData_4510_5),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4520_2[]=
{
    15,
//主站通信参数表∷=array 主站通信参数
	DataType_array,
    6,
//备用通道∷=structure 
    DataType_structure,
//运营商 enum 
    DataType_enum,
//网络类型 enum 
    DataType_enum,
//APN visible-string， 
    DataType_visible_string,
//用户名 visible-string，
    DataType_visible_string,
//密码 visible-string，
    DataType_visible_string,
//代理服务器地址 octet-string，
    DataType_octet_string,
//代理端口 long-unsigned， 
    DataType_long_unsigned,
//主站通信参数 array structure 
    DataType_array,
    2,
    DataType_structure,
//IP地址 octet-string， 
    DataType_octet_string,
//端口 long-unsigned 
    DataType_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4520_2[]=//公网远程通信多接入点备用通道 
{
	0x45200200,//OAD
  ADDR_4520_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4520_2,//FLASH预留的最大长度
	3,//7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4520_2,//数据类型列表指针
	(u32*)InitData_4520_2,//初始化原始数据指针
	sizeof(InitData_4520_2),//初始化原始数据长度
};




//---通用类---
__align(4) const u8 DL698_Attribute_4000[]=
{
	1,//类型列表数
//属性2∷=date_time_s
		DataType_date_time_s,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4000[]=//日期时间
{
	0x40000200,//OAD
  ADDR_DL698YMDHMS,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4000,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4000_3[]=
{
	1,//类型列表数
//属性3（校时模式）∷=enum
		DataType_enum,
//{
//主站授时（0），
//终端精确校时（1），
//北斗/GPS（2），
//其它（255）
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4000_3[]=//校时模式
{
	0x40000300,//OAD
  ADDR_4000_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4000_3,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4000_3,//数据类型列表指针
	(u32*)InitData_4000_3,//初始化原始数据指针
	sizeof(InitData_4000_3),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4000_4[]=
{
	6,//类型列表数
//属性4（精准校时参数）∷=structure
		DataType_structure,
//{
//最近心跳时间总个数   unsigned，
		DataType_unsigned,
//最大值剔除个数       unsigned，
		DataType_unsigned,
//最小值剔除个数       unsigned，
		DataType_unsigned,
//通讯延时阈值         unsigned（单位：秒）
		DataType_unsigned,
//最少有效个数         unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4000_4[]=//精准校时参数
{
	0x40000400,//OAD
  ADDR_4000_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4000_4,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4000_4,//数据类型列表指针
	(u32*)InitData_4000_4,//初始化原始数据指针
	sizeof(InitData_4000_4),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4004[]=
{
	12,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//经度  structure
		DataType_structure,
//{
//方位   enum{E（0），W（1）}，
		DataType_enum,
//度     unsigned，
		DataType_unsigned,
//分     unsigned，
		DataType_unsigned,
//秒     unsigned
		DataType_unsigned,
//}，
//纬度  structure
		DataType_structure,
//{
//方位   enum{S（0），N（1）}，
		DataType_enum,
//度     unsigned，
		DataType_unsigned,
//分     unsigned，
		DataType_unsigned,
//秒     unsigned
		DataType_unsigned,
//}，
//高度（cm）  double-long-unsigned
		DataType_double_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4004[]=//地理位置
{
	0x40040200,//OAD
  ADDR_4004,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4004,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4004,//数据类型列表指针
	(u32*)InitData_4004,//初始化原始数据指针
	sizeof(InitData_4004),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4006[]=
{
	3,//类型列表数
//属性2（只读）∷=structure
		DataType_structure,
//{
//时钟源  enum
		DataType_enum,
//{
//内部（0），时钟芯片（1），互联网时钟（2），
//卫星时钟（3），长波时钟（4）
//}，
//状态  enum{可用（0），不可用（1)
		DataType_enum,
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4006[]=//时钟源
{
	0x40060200,//OAD
  ADDR_4006,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4006,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4006,//数据类型列表指针
	(u32*)InitData_4006,//初始化原始数据指针
	sizeof(InitData_4006),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4007[]=
{
	8,//类型列表数
//属性2::=structure
		DataType_structure,
//{
//  上电全显时间  unsigned，
		DataType_unsigned,
//  背光点亮时间  long-unsigned(按键时背光点亮时间)，
		DataType_long_unsigned,
//  显示查看背光点亮时间  long-unsigned，
		DataType_long_unsigned,
//  有电按键屏幕驻留最大时间  long-unsigned，
		DataType_long_unsigned,
//  无电按键屏幕驻留最大时间  long-unsigned，
		DataType_long_unsigned,
//  显示电能小数位数  unsigned，
		DataType_unsigned,
//  显示功率（最大需量）小数位数  unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4007[]=//LCD参数
{
	0x40070200,//OAD
  ADDR_4007,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4007,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4007,//数据类型列表指针
	(u32*)InitData_4007,//初始化原始数据指针
	sizeof(InitData_4007),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4008[]=
{
	1,//类型列表数
//属性2∷=date_time_s
		DataType_date_time_s,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4008[]=//两套时区表切换时间
{
	0x40080200,//OAD
  ADDR_4008,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4008,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4008,//数据类型列表指针
	(u32*)InitData_4008,//初始化原始数据指针
	sizeof(InitData_4008),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4009[]=//两套日时段切换时间
{
	0x40090200,//OAD
  ADDR_4009,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4009,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4008,//数据类型列表指针
	(u32*)InitData_4009,//初始化原始数据指针
	sizeof(InitData_4009),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400A[]=//两套分时费率切换时间
{
	0x400A0200,//OAD
  ADDR_400A,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_400A,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4008,//数据类型列表指针
	(u32*)InitData_400A,//初始化原始数据指针
	sizeof(InitData_400A),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400B[]=//两套阶梯电价切换时间
{
	0x400B0200,//OAD
  ADDR_400B,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_400B,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4008,//数据类型列表指针
	(u32*)InitData_400B,//初始化原始数据指针
	sizeof(InitData_400B),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_400C[]=
{
	6,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//年时区数(p≤14)                 unsigned，
		DataType_unsigned,
//日时段表数（q≤8）              unsigned，
		DataType_unsigned,
//日时段数(每日切换数)（m≤14）   unsigned，
		DataType_unsigned,
//费率数（k≤63）                 unsigned，
		DataType_unsigned,
//公共假日数（n≤254）            unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400C[]=//时区时段数
{
	0x400C0200,//OAD
  ADDR_400C,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_400C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_400C,//数据类型列表指针
	(u32*)InitData_400C,//初始化原始数据指针
	sizeof(InitData_400C),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_400D[]=
{
	1,//类型列表数
//属性2∷=unsigned，无单位，无换算
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400D[]=//阶梯数
{
	0x400D0200,//OAD
  ADDR_400D,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_400D,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_400D,//数据类型列表指针
	(u32*)InitData_400D,//初始化原始数据指针
	sizeof(InitData_400D),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_400E[]=
{
	1,//类型列表数
//属性2∷=unsigned，无单位，无换算
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400E[]=//谐波分析次数
{
	0x400E0200,//OAD
  ADDR_400E,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_400E,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_400E,//数据类型列表指针
	(u32*)InitData_400E,//初始化原始数据指针
	sizeof(InitData_400E),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_400F[]=
{
	1,//类型列表数
//属性2∷=unsigned，无单位，无换算
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400F[]=//密钥总条数
{
	0x400F0200,//OAD
  ADDR_400F,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_400F,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_400F,//数据类型列表指针
	(u32*)InitData_400F,//初始化原始数据指针
	sizeof(InitData_400F),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4010[]=
{
	1,//类型列表数
//属性2∷=unsigned，无单位，无换算
	DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4010[]=//计量元件数
{
	0x40100200,//OAD
  ADDR_4010,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4010,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4010,//数据类型列表指针
	(u32*)InitData_4010,//初始化原始数据指针
	sizeof(InitData_4010),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4011[]=
{
	4+1,//类型列表数
//属性2∷=array 公共假日
		DataType_array,
		3,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//公共假日∷=structure
		DataType_structure,
//{
//  日期        Date，
		DataType_date,
//  日时段表号  unsigned
		DataType_unsigned,
//}
//电能表实际使用多少个公共假日取决于公共假日数，建议电能表内至少可保存20个公共假日。
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4011[]=//公共假日表
{
	0x40110200,//OAD
  ADDR_4011,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4011,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4011,//数据类型列表指针
	(u32*)InitData_4011,//初始化原始数据指针
	sizeof(InitData_4011),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4012[]=
{
	1,//类型列表数
//属性2∷=bit-string(SIZE(8))
		DataType_bit_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4012[]=//周休日特征字
{
	0x40120200,//OAD
  ADDR_4012,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4012,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4012,//数据类型列表指针
	(u32*)InitData_4012,//初始化原始数据指针
	sizeof(InitData_4012),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4013[]=
{
	1,//类型列表数
//属性2∷=unsigned，无单位，无换算
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4013[]=//周休日釆用的日时段表号
{
	0x40130200,//OAD
  ADDR_4013,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4013,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4013,//数据类型列表指针
	(u32*)InitData_4013,//初始化原始数据指针
	sizeof(InitData_4013),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4014[]=
{
	5+1,//类型列表数
//属性2∷=array 时区
		DataType_array,
		4,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//时区∷=structure
		DataType_structure,
//{
//月          ComBCD2,
		DataType_unsigned,
//日          ComBCD2,
		DataType_unsigned,
//  日时段表号  unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4014[]=//当前套时区表
{
	0x40140200,//OAD
  ADDR_4014,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4014,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4014,//数据类型列表指针
	(u32*)InitData_4014,//初始化原始数据指针
	sizeof(InitData_4014),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4015[]=//备用套时区表
{
	0x40150200,//OAD
  ADDR_4015,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4015,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4014,//数据类型列表指针
	(u32*)InitData_4015,//初始化原始数据指针
	sizeof(InitData_4015),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4016[]=
{
	6+2,//类型列表数
//属性2∷=array 日时段表
		DataType_array,
		5,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//日时段表∷=array 时段
		DataType_array,
		4,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//时段∷=structure
		DataType_structure,
//{
//  时     unsigned
		DataType_unsigned,
//  分     unsigned
		DataType_unsigned,
//  费率号 unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4016[]=//当前套日时段表
{
	0x40160200,//OAD
  ADDR_4016,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4016,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4016,//数据类型列表指针
	(u32*)InitData_4016,//初始化原始数据指针
	sizeof(InitData_4016),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4017[]=//备用套日时段表
{
	0x40170200,//OAD
  ADDR_4017,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4017,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4016,//数据类型列表指针
	(u32*)InitData_4017,//初始化原始数据指针
	sizeof(InitData_4017),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4018[]=
{
	2+1,//类型列表数
//属性2∷=array 费率电价
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//费率电价∷=double-long-unsigned
		DataType_double_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4018[]=//当前套费率电价,当前套费率电价协议只读
{
	0x40180200,//OAD
  ADDR_4018,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4018,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4018,//数据类型列表指针
	(u32*)InitData_4018,//初始化原始数据指针
	sizeof(InitData_4018),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4019[]=//备用套费率电价,备用套费率电价可读可写
{
	0x40190200,//OAD
  ADDR_4019,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4019,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4018,//数据类型列表指针
	(u32*)InitData_4019,//初始化原始数据指针
	sizeof(InitData_4019),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_401A[]=
{
	10+3,//类型列表数
//阶梯参数∷=structure
		DataType_structure,
//{
//  阶梯值数组    array 阶梯值，
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	//阶梯值∷=double-long-unsigned
		DataType_double_long_unsigned,
//  阶梯电价数组  array 阶梯电价，
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	//阶梯电价∷=double-long-unsigned
		DataType_double_long_unsigned,
//  阶梯结算日数组  array 阶梯结算日
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	//阶梯结算日∷=structure
		DataType_structure,
		//{
		//  月  unsigned，
		DataType_unsigned,
		//  日  unsigned，
		DataType_unsigned,
		//  时  unsigned
		DataType_unsigned,
		//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401A[]=//当前套阶梯电价,当前套阶梯参数协议只读
{
	0x401A0200,//OAD
  ADDR_401A,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_401A,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_401A,//数据类型列表指针
	(u32*)InitData_401A,//初始化原始数据指针
	sizeof(InitData_401A),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401B[]=//备用套阶梯电价,备用套阶梯参数可读可写
{
	0x401B0200,//OAD
  ADDR_401B,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_401B,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_401A,//数据类型列表指针
	(u32*)InitData_401B,//初始化原始数据指针
	sizeof(InitData_401B),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_401C[]=
{
	1,//类型列表数
//属性2∷=double-long-unsigned，无单位，换算:-2
		DataType_double_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401C[]=//电流互感器变比
{
	0x401C0200,//OAD
  ADDR_401C,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_401C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_401C,//数据类型列表指针
	(u32*)InitData_401C,//初始化原始数据指针
	sizeof(InitData_401C),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401D[]=//电压互感器变比
{
	0x401D0200,//OAD
  ADDR_401D,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_401D,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_401C,//数据类型列表指针
	(u32*)InitData_401D,//初始化原始数据指针
	sizeof(InitData_401D),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_401E[]=
{
	3,//类型列表数
//属性2（参数）∷=structure
		DataType_structure,
//{
//报警金额限值1    double-long-unsigned，
		DataType_double_long_unsigned,
//报警金额限值2    double-long-unsigned，
		DataType_double_long_unsigned,
//}
//单位：元，换算：-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401E[]=//报警金额限值
{
	0x401E0200,//OAD
  ADDR_401E,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_401E,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_401E,//数据类型列表指针
	(u32*)InitData_401E,//初始化原始数据指针
	sizeof(InitData_401E),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_401F[]=
{
	4,//类型列表数
//属性2（参数）∷=structure
		DataType_structure,
//{
//透支金额限值     double-long-unsigned，
		DataType_double_long_unsigned,
//囤积金额限值     double-long-unsigned，
		DataType_double_long_unsigned,
//合闸允许金额限值 double-long-unsigned，
		DataType_double_long_unsigned,
//}
//单位：元，换算：-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401F[]=//其它金额限值
{
	0x401F0200,//OAD
  ADDR_401F,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_401F,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_401F,//数据类型列表指针
	(u32*)InitData_401F,//初始化原始数据指针
	sizeof(InitData_401F),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4020[]=
{
	3,//类型列表数
//属性2（参数）∷=structure
		DataType_structure,
//{
//报警电量限值1     double-long-unsigned，
		DataType_double_long_unsigned,
//报警电量限值2     double-long-unsigned，
		DataType_double_long_unsigned,
//}
//单位：元，换算：-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4020[]=//报警电量限值
{
	0x40200200,//OAD
  ADDR_4020,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4020,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4020,//数据类型列表指针
	(u32*)InitData_4020,//初始化原始数据指针
	sizeof(InitData_4020),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4021[]=
{
	4,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//囤积电量限值     double-long-unsigned，
		DataType_double_long_unsigned,
//透支电量限值     double-long-unsigned，
		DataType_double_long_unsigned,
//合闸允许电量限值 double-long-unsigned，
		DataType_double_long_unsigned,
//}
//单位：kWh，换算：-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4021[]=//其它电量限值
{
	0x40210200,//OAD
  ADDR_4021,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4021,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4021,//数据类型列表指针
	(u32*)InitData_4021,//初始化原始数据指针
	sizeof(InitData_4021),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4022[]=
{
	1,//类型列表数
//属性2∷=bit-string(SIZE(16))，无单位，无换算
		DataType_bit_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4022[]=//插卡状态字
{
	0x40220200,//OAD
  ADDR_4022,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4022,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4022,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4023[]=
{
	1,//类型列表数
//属性2∷=long-unsigned ，单位：分钟，无换算
		DataType_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4023[]=//认证有效时长
{
	0x40230200,//OAD
  ADDR_4023,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4023,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4023,//数据类型列表指针
	(u32*)InitData_4023,//初始化原始数据指针
	sizeof(InitData_4023),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4024[]=
{
	1,//类型列表数
//属性2∷=enum {剔除投入（1），剔除解除（2）}
		DataType_enum,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4024[]=//剔除
{
	0x40240200,//OAD
  ADDR_4024,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4024,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4024,//数据类型列表指针
	(u32*)InitData_4024,//初始化原始数据指针
	sizeof(InitData_4024),//初始化原始数据长度
};


__align(4) const u8 DL698_Attribute_4030[]=
{
	5,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//电压考核上限  long-unsigned（单位：V，换算：-1），
		DataType_long_unsigned,
//  电压考核下限  long-unsigned（单位：V，换算：-1），
		DataType_long_unsigned,
//电压合格上限  long-unsigned（单位：V，换算：-1），
		DataType_long_unsigned,
//  电压合格下限  long-unsigned（单位：V，换算：-1）
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4030[]=//电压合格率参数
{
	0x40300200,//OAD
  ADDR_4030,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4030,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4030,//数据类型列表指针
	(u32*)InitData_4030,//初始化原始数据指针
	sizeof(InitData_4030),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4100[]=
{
	1,//类型列表数
//属性2∷=unsigned，单位：分钟，换算：0
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4100[]=//最大需量周期
{
	0x41000200,//OAD
  ADDR_4100,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4100,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4100,//数据类型列表指针
	(u32*)InitData_4100,//初始化原始数据指针
	sizeof(InitData_4100),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4101[]=//滑差时间
{
	0x41010200,//OAD
  ADDR_4101,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4101,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4100,//数据类型列表指针
	(u32*)InitData_4101,//初始化原始数据指针
	sizeof(InitData_4101),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4102[]=//校表脉冲宽度
{
	0x41020200,//OAD
  ADDR_4102,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4102,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4100,//数据类型列表指针
	(u32*)InitData_4102,//初始化原始数据指针
	sizeof(InitData_4102),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4103[]=
{
	1,//类型列表数
//属性2∷=visible-string
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4103[]=//资产管理编码
{
	0x41030200,//OAD
  ADDR_4103,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4103,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4103,//数据类型列表指针
	(u32*)InitData_4103,//初始化原始数据指针
	sizeof(InitData_4103),//初始化原始数据长度
};

__align(4) const u8 ADDR_4104[]=
{
//属性2∷=visible-string(SIZE(6))
	DataType_visible_string,
	6,
#if (MeterUBbcd==0x2200)//参比电压xxx.x(V)
	'2','2','0','.','0','V',
#endif
#if (MeterUBbcd==0x1000)//参比电压xxx.x(V)
	'1','0','0','.','0','V',
#endif
#if (MeterUBbcd==0x577)//参比电压xxx.x(V)
	' ','5','7','.','7','V',
#endif
	
#if (MeterUBbcd!=0x2200)&&(MeterUBbcd!=0x1000)&&(MeterUBbcd!=0x577)
	#error
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4104[]=//额定电压
{
	0x41040200,//OAD
  (u32)ADDR_4104,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 ADDR_4105[]=
{
//属性2∷=visible-string(SIZE(6))
	DataType_visible_string,
	6,
#if (MeterIBbcd==0x100)//参比电流xx.xx(A)
	' ','1','.','0','0','A',
#endif
#if (MeterIBbcd==0x150)//参比电流xx.xx(A)
	' ','1','.','5','0','A',
#endif
#if (MeterIBbcd==0x030)//参比电流xx.xx(A)
	' ','0','.','3','0','A',
#endif
	
#if ((MeterIBbcd!=0x100)&&(MeterIBbcd!=0x150)&&(MeterIBbcd!=0x030))
	#error
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4105[]=//额定电流/基本电流
{
	0x41050200,//OAD
  (u32)ADDR_4105,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 ADDR_4106[]=
{
//属性2∷=visible-string(SIZE(6))
	DataType_visible_string,
	6,
#if MeterIMaxbcd==0x600//最大电流xx.xx(A)
	' ','6','.','0','0','A',
#endif
#if MeterIMaxbcd==0x8000//最大电流xx.xx(A)
	'8','0','.','0','0','A',
#endif	
	
#if (MeterIMaxbcd!=0x600)&&(MeterIMaxbcd!=0x8000)//最大电流xx.xx(A)
	#error
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4106[]=//最大电流
{
	0x41060200,//OAD
  (u32)ADDR_4106,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 ADDR_4107[]=
{
//属性2∷=visible-string(SIZE(4))
	DataType_visible_string,
	4,
#if (MeterClass==0x02)//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级	
	' ','0','.','2',
#endif
#if (MeterClass==0x05)//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级	
	' ','0','.','5',
#endif
#if (MeterClass==0x10)//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级	
	' ','1','.','0',
#endif
#if (MeterClass==0x20)//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级	
	' ','2','.','0',
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4107[]=//有功准确度等级
{
	0x41070200,//OAD
  (u32)ADDR_4107,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 ADDR_4108[]=
{
//属性2∷=visible-string(SIZE(4))
	DataType_visible_string,
	4,
	' ','2','.','0',
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4108[]=//无功准确度等级
{
	0x41080200,//OAD
  (u32)ADDR_4108,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4109[]=
{
	1,//类型列表数
//属性2∷=double-long-unsigned，单位：imp/kWh，换算：0
		DataType_double_long_unsigned,
};
__align(4) const u8 ADDR_4109[]=
{
//属性2∷=double-long-unsigned，单位：imp/kWh，换算：0
	DataType_double_long_unsigned,
	(MeterActiveConst>>24)&0xff,(MeterActiveConst>>16)&0xff,(MeterActiveConst>>8)&0xff,(MeterActiveConst>>0)&0xff
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4109[]=//电能表有功常数
{
	0x41090200,//OAD
  (u32)ADDR_4109,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 ADDR_410A[]=
{
//属性2∷=double-long-unsigned，单位：imp/kvarh，换算：0
	DataType_double_long_unsigned,
	(MeterReactiveConst>>24)&0xff,(MeterReactiveConst>>16)&0xff,(MeterReactiveConst>>8)&0xff,(MeterReactiveConst>>0)&0xff
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410A[]=//电能表无功常数
{
	0x410A0200,//OAD
  (u32)ADDR_410A,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
__align(4) const u8 ADDR_410B[]=
{
//属性2∷=visible-string(SIZE(32))
	DataType_visible_string,
	32,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410B[]=//电能表型号
{
	0x410B0200,//OAD
  (u32)ADDR_410B,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_410C[]=
{
	4,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//A 相电导  long，单位：无，换算：-3
		DataType_long,
//B 相电导  long，单位：无，换算：-3
		DataType_long,
//C 相电导  long，单位：无，换算：-3
		DataType_long,
// }
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410C[]=//ABC各相电导系数
{
	0x410C0200,//OAD
  ADDR_410C,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_410C,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_410C,//数据类型列表指针
	(u32*)InitData_410C,//初始化原始数据指针
	sizeof(InitData_410C),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410D[]=//ABC各相电抗系数
{
	0x410D0200,//OAD
  ADDR_410D,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_410D,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_410C,//数据类型列表指针
	(u32*)InitData_410D,//初始化原始数据指针
	sizeof(InitData_410D),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410E[]=//ABC各相电阻系数
{
	0x410E0200,//OAD
  ADDR_410E,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_410E,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_410C,//数据类型列表指针
	(u32*)InitData_410E,//初始化原始数据指针
	sizeof(InitData_410E),//初始化原始数据长度
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410F[]=//ABC各相电纳系数
{
	0x410F0200,//OAD
  ADDR_410F,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_410F,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_410C,//数据类型列表指针
	(u32*)InitData_410F,//初始化原始数据指针
	sizeof(InitData_410F),//初始化原始数据长度
};

__align(4) const u8 ADDR_4111[]=
{
//属性2∷=visible-string
	DataType_visible_string,
	8,
	'A','A','A','A','A','A','A','A',	
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4111[]=//软件备案号
{
	0x41110200,//OAD
  (u32)ADDR_4111,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4112[]=
{
	1,//类型列表数
//属性2∷=bit-string(SIZE (8))
		DataType_bit_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4112[]=//有功组合方式特征字
{
	0x41120200,//OAD
  ADDR_4112,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4112,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4112,//数据类型列表指针
	(u32*)InitData_4112,//初始化原始数据指针
	sizeof(InitData_4112),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4113[]=//无功组合方式1特征字
{
	0x41130200,//OAD
  ADDR_4113,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4113,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4112,//数据类型列表指针
	(u32*)InitData_4113,//初始化原始数据指针
	sizeof(InitData_4113),//初始化原始数据长度
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4114[]=//无功组合方式2特征字
{
	0x41140200,//OAD
  ADDR_4114,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4114,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4112,//数据类型列表指针
	(u32*)InitData_4114,//初始化原始数据指针
	sizeof(InitData_4114),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4115[]=
{
	2,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//IC卡类型  enum
		DataType_enum,
//{
//参数预制卡（0），用户开户卡（1），
//用户购电卡（2），用户补卡（3）
//}，
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4115[]=//IC卡
{
	0x41150200,//OAD
  ADDR_4115,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4115,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4115,//数据类型列表指针
	(u32*)InitData_4115,//初始化原始数据指针
	sizeof(InitData_4115),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4116[]=
{
	4+1,//类型列表数
//属性2（配置参数）∷=array 结算日日期
		DataType_array,
		3,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
//结算日日期∷=structure
		DataType_structure,
//{
//日  ComBCD2,
		DataType_unsigned,
//时  ComBCD2
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4116[]=//结算日
{
	0x41160200,//OAD
  ADDR_4116,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4116,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4116,//数据类型列表指针
	(u32*)InitData_4116,//初始化原始数据指针
	sizeof(InitData_4116),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4117[]=
{
	1,//类型列表数
//属性2（配置参数）∷= TI
		DataType_TI,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4117[]=//期间需量冻结周期
{
	0x41170200,//OAD
  ADDR_4117,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4117,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4117,//数据类型列表指针
	(u32*)InitData_4117,//初始化原始数据指针
	sizeof(InitData_4117),//初始化原始数据长度
};






//---终端类---
//4200	11	路由表
//4201	8	路由信息单元
__align(4) const u8 DL698_Attribute_4202[]=
{
	10+1,//类型列表数
//属性2∷=structure
		DataType_structure,
//{
//级联标志            bool，
		DataType_bool,
//级联通信端口号      OAD，
		DataType_OAD,
//总等待超时（10ms）  long-unsigned，
		DataType_long_unsigned,
//字节超时（10ms）    long-unsigned，
		DataType_long_unsigned,
//重发次数            unsigned，
		DataType_unsigned,
//巡测周期（min）     unsigned，
		DataType_unsigned,
//级联（被）端口数    unsigned，
		DataType_unsigned,
//级联（被）终端地址  array TSA
		DataType_array,
		1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
		DataType_TSA,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4202[]=//级联通信参数
{
	0x42020200,//OAD
  ADDR_4202,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4202,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4202,//数据类型列表指针
	(u32*)InitData_4202,//初始化原始数据指针
	sizeof(InitData_4202),//初始化原始数据长度
};

__align(4) const u8 DL698_Attribute_4204_2[]=
{
	3,//类型列表数
//属性2（终端广播校时参数）∷=structure
		DataType_structure,
//{
//     终端广播校时启动时间  time，
		DataType_time,
//     是否启用              bool
		DataType_bool,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4204_2[]=//终端广播校时-2
{
	0x42040200,//OAD
  ADDR_4204_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4204_2,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4204_2,//数据类型列表指针
	(u32*)InitData_4204_2,//初始化原始数据指针
	sizeof(InitData_4204_2),//初始化原始数据长度
};
__align(4) const u8 DL698_Attribute_4204_3[]=
{
	4,//类型列表数
//属性3（终端单地址广播校时参数）∷=structure
		DataType_structure,
//{
//     时钟误差阈值          unsigned（单位：秒），
		DataType_integer,
//     终端广播校时启动时间  time，
		DataType_time,
//     是否启用              bool
		DataType_bool,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4204_3[]=//终端广播校时-3
{
	0x42040300,//OAD
  ADDR_4204_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4204_3,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4204_3,//数据类型列表指针
	(u32*)InitData_4204_3,//初始化原始数据指针
	sizeof(InitData_4204_3),//初始化原始数据长度
};






//0x4300电气设备
//1	逻辑名	标记接口类对象实例，使用OI
//2	设备描述符	visible-string
__align(4) const u8 ADDR_4300_2[]=
{
	DataType_visible_string,
	8,
	'T','e','r','m','i','n','a','l',
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_2[]=
{
	0x43000200,//OAD
  (u32)ADDR_4300_2,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
//3	版本信息	版本信息∷=structure
//{
//厂商代码       visible-string(SIZE (4))，
//软件版本号     visible-string(SIZE (4))，
//软件版本日期   visible-string(SIZE (6))，
//硬件版本号     visible-string(SIZE (4))，
//硬件版本日期   visible-string(SIZE (6))，
//厂家扩展信息   visible-string(SIZE (8))
//}
__align(4) const u8 ADDR_4300_3[]=
{
	DataType_structure,
	6,
	DataType_visible_string,
	4,
	FactoryCode,//厂商代码
	DataType_visible_string,
	4,
	'V',((SVER>>8)&0xf)+0x30,'.',((SVER>>4)&0xf)+0x30,//软件版本号
	DataType_visible_string,
	6,
	((SVDATE>>20)&0xf)+0x30,((SVDATE>>16)&0xf)+0x30,((SVDATE>>12)&0xf)+0x30,((SVDATE>>8)&0xf)+0x30,((SVDATE>>4)&0xf)+0x30,((SVDATE>>0)&0xf)+0x30,//软件版本日期
	DataType_visible_string,
	4,
	((HVER>>12)&0xf)+0x30,((HVER>>8)&0xf)+0x30,((HVER>>4)&0xf)+0x30,((HVER>>0)&0xf)+0x30,//硬件版本号
	DataType_visible_string,
	6,
	((HVDATE>>20)&0xf)+0x30,((HVDATE>>16)&0xf)+0x30,((HVDATE>>12)&0xf)+0x30,((HVDATE>>8)&0xf)+0x30,((HVDATE>>4)&0xf)+0x30,((HVDATE>>0)&0xf)+0x30,//硬件版本日期
	DataType_visible_string,
	8,
	FactoryInfoExt,//厂家扩展信息
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_3[]=
{
	0x43000300,//OAD
  (u32)ADDR_4300_3,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
//4	生产日期	设备的出厂日期，数据格式date_time_s
__align(4) const u8 ADDR_4300_4[]=
{
	DataType_date_time_s,
	ProduceDate_year>>8,//生产日期年(hex) 
	ProduceDate_year&0xff,
	ProduceDate_month,//生产日期月(hex) 
	ProduceDate_day,//生产日期日(hex) 
	ProduceDate_hour,//生产日期时(hex)
	ProduceDate_minute,//生产日期分(hex)
	ProduceDate_second,//生产日期秒(hex)
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_4[]=
{
	0x43000400,//OAD
  (u32)ADDR_4300_4,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
//5	子设备列表	array OI，包含设备自身的子模块信息
__align(4) const u8 ADDR_4300_5[]=
{
	DataType_array,
	0,
	//DataType_OI,
	//0x00,0x01,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_5[]=
{
	0x43000500,//OAD
  (u32)ADDR_4300_5,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};
//6	支持规约列表	设备支持的规约列表，数据格式 array visible-string
__align(4) const u8 ADDR_4300_6[]=
{
	DataType_array,
	1,
	DataType_visible_string,
	10,
	'D','L','/','T','6','9','8','.','4','5',
	
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_6[]=
{
	0x43000600,//OAD
  (u32)ADDR_4300_6,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	0,//FLASH预留的最大长度
	1,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	0,//数据类型列表指针
	0,//初始化原始数据指针
	0,//初始化原始数据长度
};

//7	允许跟随上报	True：允许跟随上报，False：禁止跟随上报。
__align(4) const u8 DL698_Attribute_4300_7[]=
{
	1,//类型列表数
	DataType_bool,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_7[]=
{
	0x43000700,//OAD
  ADDR_4300_7,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4300_7,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4300_7,//数据类型列表指针
	(u32*)InitData_4300_7,//初始化原始数据指针
	sizeof(InitData_4300_7),//初始化原始数据长度
};

//8	允许主动上报	True：允许主动上报，False：禁止主动上报。
__align(4) const u8 DL698_Attribute_4300_8[]=
{
	1,//类型列表数
	DataType_bool,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_8[]=
{
	0x43000800,//OAD
  ADDR_4300_8,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4300_8,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4300_8,//数据类型列表指针
	(u32*)InitData_4300_8,//初始化原始数据指针
	sizeof(InitData_4300_8),//初始化原始数据长度
};

//9	允许与主站通话	True：允许通话，False：禁止通话。
__align(4) const u8 DL698_Attribute_4300_9[]=
{
	1,//类型列表数
	DataType_bool,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_9[]=
{
	0x43000900,//OAD
  ADDR_4300_9,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4300_9,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4300_9,//数据类型列表指针
	(u32*)InitData_4300_9,//初始化原始数据指针
	sizeof(InitData_4300_9),//初始化原始数据长度
};
//10	上报通道	array OAD
__align(4) const u8 DL698_Attribute_4300_10[]=
{
	3,//类型列表数
	DataType_array,
	1,//本数组包括的数据类型个数,用于数组=0时跳过类型个数用
	DataType_OAD,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_10[]=
{
	0x43000A00,//OAD
  ADDR_4300_10,//数据地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_4300_10,//FLASH预留的最大长度
	7,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)DL698_Attribute_4300_10,//数据类型列表指针
	(u32*)InitData_4300_10,//初始化原始数据指针
	sizeof(InitData_4300_10),//初始化原始数据长度
};

#if (USER/100)==15//安徽
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_3106_11[]=//终端停/上电事件	
{
	0x31060B00,//对象标识
	ADDR_3106_11,//配置参数地址
	0,//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	LENmax_3106_11,//LENmax_4400,//FLASH预留的最大长度
	3,//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	(u32*)0,//数据类型列表指针
	(u32*)InitData_3106_11,//初始化原始数据指针
	sizeof(InitData_3106_11),//初始化原始数据长度
};
#endif


#if (USER/100)==17//???
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4048[]=//???/????	
{
	0x40480200,//????
	ADDR_4048_127,//??????
	0,//??????????????????????,???0
	LENmax_4048_127,//LENmax_4400,//FLASH???????
	3,//??:b0=?,b1=?,b2=????;0=???,1=??,2=??,3=??
	(u32*)0,//????????
	(u32*)InitData_4048_127,//?????????
	sizeof(InitData_4048_127),//?????????
};
#endif
