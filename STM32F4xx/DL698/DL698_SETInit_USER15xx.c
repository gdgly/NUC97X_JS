
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"



#if (USER/100)==15//安微系



////***通信地址类***
//#define ADDR_4001     ADDR_DL698_SET_START//通信地址
//#define LENmax_4001     18
const u8 InitData_4001[]=
{
//属性2∷=octet-string
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x01,
};

const u8 InitData_3106_11[]=//终端停/上电事件-11
{
//属性11（配置参数2）∷=structure
	DataType_structure,
	1,
//{
//关联采集任务号 unsigned
	DataType_unsigned,
	54,
//}
};


//#define ADDR_4002     ADDR_4005+LENmax_4005//表号
//#define LENmax_4002     18
const u8 InitData_4002[]=
{
//属性2∷=octet-string
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x01,
};

//#define ADDR_4003     ADDR_4002+LENmax_4002//客户编号
//#define LENmax_4003     18
const u8 InitData_4003[]=
{
//属性2∷=octet-string
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x01,
};

//#define ADDR_4005     ADDR_4001+LENmax_4001//组地址
//#define LENmax_4005     (2+(16*18))
const u8 InitData_4005[]=
{
//属性2∷=array octet-string
	DataType_array,
	1,
	DataType_octet_string,
	6,
	0x11,0x22,0x33,0x44,0x55,0x66,
};

//#define  ADDR_4300_7     ADDR_4003+LENmax_4003//允许跟随上报
//#define LENmax_4300_7     2
const u8 InitData_4300_7[]=
{
//属性7∷=bool
	DataType_bool,
	0,
};
//#define  ADDR_4300_8     ADDR_4300_7+LENmax_4300_7//允许主动上报
//#define LENmax_4300_8     2
const u8 InitData_4300_8[]=
{
//属性8∷=bool
	DataType_bool,
	1,
};
//#define  ADDR_4300_9     ADDR_4300_8+LENmax_4300_8//允许与主站通话
//#define LENmax_4300_9     2
const u8 InitData_4300_9[]=
{
//属性8∷=bool
	DataType_bool,
	0,
};
//#define  ADDR_4300_10     ADDR_4300_9+LENmax_4300_9//上报通道
//#define LENmax_4300_10     32
const u8 InitData_4300_10[]=
{
//属性10∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x45,0x00,0x02,0x00,//GPRS
	DataType_OAD,
	0x45,0x10,0x02,0x00,//ENET
//	DataType_OAD,
//	0xF2,0x00,0x02,0x01,//RS232
//	DataType_OAD,
//	0xF2,0x01,0x02,0x01,//RS485-1
//	DataType_OAD,
//	0xF2,0x01,0x02,0x02,//RS485-2
};

//#define ADDR_4401     ADDR_4003+LENmax_4003//应用连接认证密码
//#define LENmax_4401     128
const u8 InitData_4401[]=
{
//属性2（只写）∷=visible-string
	DataType_visible_string,
	6,
	'0','0','0','0','0','0',
};

//#define ADDR_4500_2     ADDR_4003+LENmax_4003//公网通信模块1;2通信配置
//#define LENmax_4500_2     128
const u8 InitData_4500_2[]=
{
//通信配置∷=structure
	DataType_structure,
	12,
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
	DataType_enum,
	1,
//在线方式      enum{永久在线（0），被动激活（1）}，
	DataType_enum,
	0,
//连接方式      enum{TCP（0），UDP（1）}，
	DataType_enum,
	0,
//连接应用方式  enum{主备模式（0），多连接模式（1）}，
	DataType_enum,
	0,
//侦听端口列表  array long-unsigned，
	DataType_array,
	0,
//	DataType_long_unsigned,
//	0x23,0x43,//9027,
//APN            visible-string，	
	DataType_visible_string,
	//10,
	//'c','m','i','o','t','y','t','.','a','h',
	11,
	'c','m','i','o','t','y','t','x','.','a','h',
//用户名         visible-string，
	DataType_visible_string,
	4,
	'C','A','R','D',	
//密码           visible-string，
	DataType_visible_string,
	4,
	'C','A','R','D',	
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//超时时间及重发次数  bit-string(SIZE(8))
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
	DataType_unsigned,
  (30<<2)+0x03,
//心跳周期(秒)  long-unsigned
	DataType_long_unsigned,
	(300>>8)&0xff,300&0xff,	
//}
};

//#define ADDR_4500_3     ADDR_4500_2+LENmax_4500_2//公网通信模块1;3主站通信参数表
//#define LENmax_4500_3     64
const u8 InitData_4500_3[]=
{
//主站通信参数表∷=array 主站通信参数
	DataType_array,
	2,
//第1个主站通信参数：：=structure
	DataType_structure,
	2,
//{
//主用IP地址   octet-string，
	DataType_octet_string,
	4,
	10,138,16,133,
//主用端口     long-unsigned，
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}
//第2个主站通信参数：：=structure
	DataType_structure,
	2,
//{
//备用IP地址   octet-string，
	DataType_octet_string,
	4,
	10,138,16,133,
//备用端口     long-unsigned，
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}	
};

//#define ADDR_4500_4     ADDR_4500_3+LENmax_4500_3//公网通信模块1;4短信通信参数
//#define LENmax_4500_4     64
const u8 InitData_4500_4[]=
{
//短信通信参数：：=structure
	DataType_structure,
  3,
//{
//短信中心号码     visible-string(SIZE(16))，
	DataType_visible_string,
	0,
//主站号码         array visible-string(SIZE(16))，
	DataType_array,
	0,
//  短信通知目的号码 array visible-string(SIZE(16))
	DataType_array,
	0,
//}
};

//#define ADDR_4500_11     ADDR_4500_10+LENmax_4500_10//公网通信模块1;11 拨号IP
//#define LENmax_4500_11     22
const u8 InitData_4500_11[]=
{
//octet-string
	DataType_octet_string,
	0,
};

//#define ADDR_4510_2     ADDR_4500_4+LENmax_4500_4//以太网通信模块1;2通信配置
//#define LENmax_4510_2     128
const u8 InitData_4510_2[]=
{
//通信配置∷=structure
	DataType_structure,
	8,
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
	DataType_enum,
	1,
//连接方式  enum{TCP（0），UDP（1）}，
	DataType_enum,
	0,
//连接应用方式enum{主备模式（0），多连接模式（1）}
	DataType_enum,
	0,
//侦听端口列表array long-unsigned，
	DataType_array,
	0,
//	DataType_long_unsigned,
//	9027>>8,9027&0xff,//9027,
//代理服务器地址  octet-string，
	DataType_octet_string,
	0,
//代理端口  long-unsigned，
	DataType_long_unsigned,
	0,0,
//超时时间及重发次数  bit-string(SIZE(8))
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
	DataType_unsigned,
  (30<<2)+0x03,
//心跳周期(秒)  long-unsigned
	DataType_long_unsigned,
	(60>>8)&0xff,60&0xff,	
//}
};

//#define ADDR_4510_3     ADDR_4510_2+LENmax_4510_2//以太网通信模块1;3主站通信参数表
//#define LENmax_4510_3     64
const u8 InitData_4510_3[]=
{
//主站通信参数表∷=array 主站通信参数
	DataType_array,
	2,
//第1个主站通信参数：：=structure
	DataType_structure,
	2,
//{
//主用IP地址   octet-string，
	DataType_octet_string,
	4,
	10,138,16,133,
//主用端口     long-unsigned，
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}
//第2个主站通信参数：：=structure
	DataType_structure,
	2,
//{
//备用IP地址   octet-string，
	DataType_octet_string,
	4,
	10,138,16,133,
//备用端口     long-unsigned，
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}
};

//#define ADDR_4510_4     ADDR_4510_3+LENmax_4510_3//以太网通信模块1;4网络配置
//#define LENmax_4510_4     128
const u8 InitData_4510_4[]=
{
//网络配置∷=structure
	DataType_structure,
	6,
//{
//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}，
	DataType_enum,
	1,
//IP地址       octet-string，
	DataType_octet_string,
	4,
	192,168,127,244,
//子网掩码     octet-string，
	DataType_octet_string,
	4,
	255,255,255,0x00,
//网关地址     octet-string，
	DataType_octet_string,
	4,
	192,168,127,1,
//PPPoE用户名  visible-string，
	DataType_visible_string,
	0,
//PPPoE密码    visible-string
	DataType_visible_string,
	0,
//}
};

//#define ADDR_4510_5     ADDR_4510_4+LENmax_4510_4// 以太网通信模块1;5 MAC地址
//#define LENmax_4510_5     16
const u8 InitData_4510_5[]=
{
//MAC地址∷=octet-string 
	DataType_octet_string,
    6,
	0x12,0x34,0x56,0x78,0x00,0x00,
};	

//#define ADDR_4520_2     ADDR_240x_4+LENmax_240x_4//公网远程通信多接入点备用通道
//#define LENmax_4520_2     512
const u8 InitData_4520_2[]=
{
//主站通信参数表∷=array 主站通信参数
	DataType_array,
	6,
//第1个主站通信参数：：=structure
	DataType_structure,
	8,
//运营商 enum 
//{ 
//CMCC（移动） = 0， 
//CTCC（电信） = 1， 
//CUCC（联通） = 2， 
//未知 = 255 
//}，
    DataType_enum,
    0,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
    DataType_enum,
    2,
//APN            visible-string，	
	DataType_visible_string,
	11,
	'c','m','i','o','t','y','t','x','.','a','h',
//用户名         visible-string，
	DataType_visible_string,
	0,
//密码           visible-string，
	DataType_visible_string,
    0,
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	
	//第2个主站通信参数：：=structure
	DataType_structure,
	8,
//运营商 enum 
//{ 
//CMCC（移动） = 0， 
//CTCC（电信） = 1， 
//CUCC（联通） = 2， 
//未知 = 255 
//}，
  DataType_enum,
  2,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
  DataType_enum,
  2,
//APN            visible-string，	
	DataType_visible_string,
	24,
	'a','h','b','z','g','w','d','l','0','1','.','z','n','c','b',
	'.','n','j','m','2','m','a','p','n',
//用户名         visible-string，
	DataType_visible_string,
	0,
//密码           visible-string，
	DataType_visible_string,
    0,
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	
	//第3个主站通信参数：：=structure
	DataType_structure,
	8,
//运营商 enum 
//{ 
//CMCC（移动） = 0， 
//CTCC（电信） = 1， 
//CUCC（联通） = 2， 
//未知 = 255 
//}，
    DataType_enum,
    1,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
    DataType_enum,
    2,
//APN            visible-string，	
	DataType_visible_string,
	12,
	'd','l','c','b','.','v','p','d','n','.','a','h',
//用户名         visible-string，
	DataType_visible_string,
	19,
	'y','z','w','x','@','d','l','c','d','m','a',
	'.','v','p','d','n','.','a','h',
//密码           visible-string，
	DataType_visible_string,
    6,
    '1','2','3','4','5','6',
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
    
//第四个主站通信参数：：=structure
	DataType_structure,
	8,
//运营商 enum 
//{ 
//CMCC（移动） = 0， 
//CTCC（电信） = 1， 
//CUCC（联通） = 2， 
//未知 = 255 
//}，
    DataType_enum,
    0,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
    DataType_enum,
    1,
//APN            visible-string，	
	DataType_visible_string,
	11,
	'd','l','g','p','r','s','c','j','.','a','h',
//用户名         visible-string，
	DataType_visible_string,
	0,
//密码           visible-string，
	DataType_visible_string,
    0,
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	
//第5个主站通信参数：：=structure
	DataType_structure,
	8,
//运营商 enum 
//{ 
//CMCC（移动） = 0， 
//CTCC（电信） = 1， 
//CUCC（联通） = 2， 
//未知 = 255 
//}，
  DataType_enum,
  2,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
  DataType_enum,
  1,
//APN            visible-string，	
	DataType_visible_string,
	16,
	'h','f','g','d','j','.','y','c','c','j','.','a','h','a','p','n',
//用户名         visible-string，
	DataType_visible_string,
	0,
//密码           visible-string，
	DataType_visible_string,
    0,
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
    
//第6个主站通信参数：：=structure
	DataType_structure,
	8,
//运营商 enum 
//{ 
//CMCC（移动） = 0， 
//CTCC（电信） = 1， 
//CUCC（联通） = 2， 
//未知 = 255 
//}，
    DataType_enum,
    1,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
    DataType_enum,
    1,
//APN            visible-string，	
	DataType_visible_string,
	14,
	'd','l','c','d','m','a','.','v','p','d','n','.','a','h',
//用户名         visible-string，
	DataType_visible_string,
	19,
	'y','z','w','x','@','d','l','c','d','m','a',
	'.','v','p','d','n','.','a','h',
//密码           visible-string，
	DataType_visible_string,
    6,
    '1','2','3','4','5','6',
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	10,138,16,133,
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
    
};

	 
//*************************通用类*************************
//#define ADDR_4000_3     ADDR_4510_5+LENmax_4510_5//校时模式
//#define LENmax_4000_3     2
const u8 InitData_4000_3[]=
{
//属性3（校时模式）∷=enum
//{
//主站授时（0），
//终端精确校时（1），
//北斗/GPS（2），
//其它（255）
//}
	DataType_enum,
  1,
};

//#define ADDR_4000_4     ADDR_4000_3+LENmax_4000_3//精准校时参数
//#define LENmax_4000_4     12
const u8 InitData_4000_4[]=
{
//属性4（精准校时参数）∷=structure
	DataType_structure,
	5,
//{
//最近心跳时间总个数   unsigned，
	DataType_unsigned,
	10,
//最大值剔除个数       unsigned，
	DataType_unsigned,
	1,
//最小值剔除个数       unsigned，
	DataType_unsigned,
	1,
//通讯延时阈值         unsigned（单位：秒）， 
	DataType_unsigned,
	5,
//最少有效个数         unsigned
	DataType_unsigned,
	5,
//}
};

//#define ADDR_4000_127     ADDR_4000_4+LENmax_4000_4//方法127：广播校时（参数）
////参数∷=date_time_s
//#define LENmax_4000_127     8

//#define ADDR_4004     ADDR_4000_127+LENmax_4000_127//设备地理位置
//#define LENmax_4004     30
const u8 InitData_4004[]=
{
//属性2∷=structure
	DataType_structure,
	3,
//{
	//经度  structure
	DataType_structure,
	4,
	//{
		//方位   enum{E（0），W（1）}，
	DataType_enum,
	0,
		//度     unsigned，
	DataType_unsigned,
	0,
		//分     unsigned，
	DataType_unsigned,
	0,
		//秒     unsigned
	DataType_unsigned,
	0,
	//}，
	//纬度  structure
	DataType_structure,
	4,
	//{
		//方位   enum{S（0），N（1）}，
	DataType_enum,
	0,
		//度     unsigned，
	DataType_unsigned,
	0,
		//分     unsigned，
	DataType_unsigned,
	0,
		//秒     unsigned
	DataType_unsigned,
	0,
	//}，
	//高度（cm）  double-long-unsigned
	DataType_double_long_unsigned,
	0,
//}
};

//#define ADDR_4006     ADDR_4004+LENmax_4004//时钟源
//#define LENmax_4006     16
const u8 InitData_4006[]=
{
//属性2（只读）∷=structure
	DataType_structure,
	2,
//{
//	时钟源  enum
//	{
//		内部（0），时钟芯片（1），互联网时钟（2），卫星时钟（3），长波时钟（4）
//	}，
	DataType_enum,
	1,
//	状态  enum
//	{
//		可用（0），不可用（1）
//  }
	DataType_enum,
	0,
//}
};

//#define ADDR_4007     ADDR_4006+LENmax_4006//LCD参数
//#define LENmax_4007     20
const u8 InitData_4007[]=
{
//属性2::=structure
	DataType_structure,
	7,
//{
//  上电全显时间  unsigned，
	DataType_unsigned,
	5,
//  背光点亮时间  long-unsigned(按键时背光点亮时间)，
	DataType_long_unsigned,
	0,5,
//  显示查看背光点亮时间  long-unsigned，
	DataType_long_unsigned,
	0,5,
//  有电按键屏幕驻留最大时间  long-unsigned，
	DataType_long_unsigned,
	0,5,
//  无电按键屏幕驻留最大时间  long-unsigned，
	DataType_long_unsigned,
	0,5,
//  显示电能小数位数  unsigned，
	DataType_unsigned,
	2,
//  显示功率（最大需量）小数位数  unsigned
	DataType_unsigned,
	2,
//}
//以上时间的单位均为：秒。
};



//***电能表类***
//#define ADDR_4008     ADDR_4007+LENmax_4007//两套时区表切换时间
//#define LENmax_4008     10
const u8 InitData_4008[]=
{
//属性2∷=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_4009     ADDR_4008+LENmax_4008//两套日时段切换时间
//#define LENmax_4009     10
const u8 InitData_4009[]=
{
//属性2∷=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_400A     ADDR_4009+LENmax_4009//两套分时费率切换时间
//#define LENmax_400A     10
const u8 InitData_400A[]=
{
//属性2∷=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_400B     ADDR_400A+LENmax_400A//两套阶梯电价切换时间
//#define LENmax_400B     10
const u8 InitData_400B[]=
{
//属性2∷=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_400C     ADDR_400B+LENmax_400B//时区时段数
//#define LENmax_400C     16
const u8 InitData_400C[]=
{
//属性2∷=structure
	DataType_structure,
	5,
//{
//年时区数(p≤14)                 unsigned，
	DataType_unsigned,
	1,
//日时段表数（q≤8）              unsigned，
	DataType_unsigned,
	1,
//日时段数(每日切换数)（m≤14）   unsigned，
	DataType_unsigned,
	4,
//费率数（k≤63）                 unsigned，
	DataType_unsigned,
	4,
//公共假日数（n≤254）            unsigned
	DataType_unsigned,
	0,
//}
};

//#define ADDR_400D     ADDR_400C+LENmax_400C//阶梯数
//#define LENmax_400D     2
const u8 InitData_400D[]=
{
//属性2∷=unsigned，无单位，无换算
	DataType_unsigned,
	0,
};

//#define ADDR_400E     ADDR_400D+LENmax_400D//谐波分析次数
//#define LENmax_400E     2
const u8 InitData_400E[]=
{
//属性2∷=unsigned，无单位，无换算
	DataType_unsigned,
	21,
};

//#define ADDR_400F     ADDR_400E+LENmax_400E//密钥总条数
//#define LENmax_400F     2
const u8 InitData_400F[]=
{
//属性2∷=unsigned，无单位，无换算
	DataType_unsigned,
	20,
};

//#define ADDR_4010     ADDR_400F+LENmax_400F//计量元件数
//#define LENmax_4010     2
const u8 InitData_4010[]=
{
//属性2∷=unsigned，无单位，无换算
	DataType_unsigned,
#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
	3,
#else
	2,
#endif
	
};

//#define ADDR_4011     ADDR_400F+LENmax_4010//公共假日表
//#define LENper_4011     8
//#define NUMmax_4011     30
//#define LENmax_4011     (2+(LENper_4011*NUMmax_4011))
const u8 InitData_4011[]=
{
//属性2∷=array 公共假日
	DataType_array,	
	0,
//公共假日∷=structure
//{
//  日期        date，
	
//  日时段表号  unsigned
//}
//电能表实际使用多少个公共假日取决于公共假日数，建议电能表内至少可保存20个公共假日。
};

//#define ADDR_4012     ADDR_4011+LENmax_4011//周休日特征字
//#define LENmax_4012     3
const u8 InitData_4012[]=
{
//属性2∷=bit-string(SIZE(8))
	DataType_bit_string,	
	8,
	0xFE,
};

//#define ADDR_4013     ADDR_4012+LENmax_4012//周休日釆用的日时段表号
//#define LENmax_4013     2
const u8 InitData_4013[]=
{
//属性2∷=unsigned，无单位，无换算
	DataType_unsigned,
	1,
};

//#define ADDR_4014     ADDR_4013+LENmax_4013//当前套时区表
//#define LENper_4014     8
//#define NUMmax_4014     12
//#define LENmax_4014     (2+(LENper_4014*NUMmax_4014))
const u8 InitData_4014[]=
{
//属性2∷=array
	DataType_array,	
	1,
//时区∷=structure
	DataType_structure,	
	3,
//{
//月          unsigned,
	DataType_unsigned,
	1,
//日          unsigned,
	DataType_unsigned,
	1,
//  日时段表号  unsigned
	DataType_unsigned,
	1,
//}
};

//#define ADDR_4015     ADDR_4014+LENmax_4014//备用套时区表
//#define LENper_4015     8
//#define NUMmax_4015     12
//#define LENmax_4015     (2+(LENper_4015*NUMmax_4015))
const u8 InitData_4015[]=
{
//属性2∷=array
	DataType_array,	
	2,
//时区∷=structure
	DataType_structure,	
	3,
//{
//月          unsigned,
	DataType_unsigned,
	1,
//日          unsigned,
	DataType_unsigned,
	1,
//  日时段表号  unsigned
	DataType_unsigned,
	1,
//}
//时区∷=structure
	DataType_structure,	
	3,
//{
//月          unsigned,
	DataType_unsigned,
	6,
//日          unsigned,
	DataType_unsigned,
	1,
//  日时段表号  unsigned
	DataType_unsigned,
	2,
//}
};

//#define ADDR_4016     ADDR_4015+LENmax_4015//当前套日时段表
//#define LENper_4016     (2+(8*8))
//#define NUMmax_4016     4
//#define LENmax_4016     (2+(LENper_4016*NUMmax_4016))
const u8 InitData_4016[]=
{
//属性2∷=array 日时段表
	DataType_array,	
	1,
	
//峰：9：00—12：00、17：00—22：00；平：8：00—9：00、12：00—17：00、22：00——23：00；谷：23：00——8：00
//日时段表∷=array 时段
	DataType_array,	
	6,
	
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	0,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	4,
//}
	
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	9,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	2,
//}

//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	12,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	3,
//}

//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	17,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	2,
//}

//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	22,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	3,
//}

//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	23,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	4,
//}

};

//#define ADDR_4017     ADDR_4016+LENmax_4016//备用套日时段表
//#define LENper_4017     (2+(8*8))
//#define NUMmax_4017     4
//#define LENmax_4017     (2+(LENper_4017*NUMmax_4017))
const u8 InitData_4017[]=
{
//属性2∷=array 日时段表
	DataType_array,	
	1,
//日时段表∷=array 时段
	DataType_array,	
	8,
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	0,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	1,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	12,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	1,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	3,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	2,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	15,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	2,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	6,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	3,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	18,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	3,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	9,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	4,
//}
//费率号：该时段采用的费率号
//时段∷=structure
	DataType_structure,	
	3,
//{
//  时     unsigned,
	DataType_unsigned,
	21,
//  分     unsigned,
	DataType_unsigned,
	0,
//  费率号 unsigned
	DataType_unsigned,
	4,
//}
//费率号：该时段采用的费率号
};

//#define ADDR_4018     ADDR_4017+LENmax_4017//当前套费率电价
//#define LENper_4018     5
//#define NUMmax_4018     8
//#define LENmax_4018     (2+(LENper_4018*NUMmax_4018))
const u8 InitData_4018[]=
{
//属性2∷=array 费率电价
	DataType_array,	
	4,	
//费率电价∷=double-long-unsigned
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1元/kWh,
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1元/kWh,
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1元/kWh,
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1元/kWh,
//单位：元/kWh，换算：-4，电能表最多支持32个费率电价，当前套费率电价协议只读。
};

//#define ADDR_4019     ADDR_4018+LENmax_4018//备用套费率电价
//#define LENper_4019     5
//#define NUMmax_4019     8
//#define LENmax_4019     (2+(LENper_4019*NUMmax_4019))
const u8 InitData_4019[]=
{
//属性2∷=array 费率电价
	DataType_array,	
	4,	
//费率电价∷=double-long-unsigned
	DataType_double_long_unsigned,
	0x00,0x00,0x13,0x88,//5000,
	DataType_double_long_unsigned,
	0x00,0x00,0x13,0x24,//4900,
	DataType_double_long_unsigned,
	0x00,0x00,0x12,0xC0,//4800,
	DataType_double_long_unsigned,
	0x00,0x00,0x12,0x5C,//4700,
//单位：元/kWh，换算：-4，电能表最多支持32个费率电价，当前套费率电价协议只读。
};

//#define ADDR_401A     ADDR_4019+LENmax_4019//当前套阶梯电价
//#define LENmax_401A     1024
const u8 InitData_401A[]=
{
//阶梯参数∷=structure
	DataType_structure,	
	3,
//{
//  阶梯值数组    array 阶梯值，
	DataType_array,	
	6,
//阶梯值∷=double-long-unsigned
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
//  阶梯电价数组  array 阶梯电价，
	DataType_array,	
	7,	
//阶梯电价∷=double-long-unsigned
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
//  阶梯结算日数组  array 阶梯结算日
	DataType_array,	
	4,
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}	
//}
//阶梯值：单位：kWh，换算：-2
//阶梯电价：单位：元/kWh，换算：-4
//阶梯结算日中月、日、时均有效时则按结算日执行阶梯冻结，如果第1阶梯结算日中仅月、日有效则以此每月该日时执行阶梯冻结，当全部无效时不执行阶梯冻结。
//当前套阶梯参数协议只读。
};

//#define ADDR_401B     ADDR_401A+LENmax_401A//备用套阶梯电价
//#define LENmax_401B     1024
const u8 InitData_401B[]=
{
//阶梯参数∷=structure
	DataType_structure,	
	3,
//{
//  阶梯值数组    array 阶梯值，
	DataType_array,	
	6,
//阶梯值∷=double-long-unsigned
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
//  阶梯电价数组  array 阶梯电价，
	DataType_array,	
	7,	
//阶梯电价∷=double-long-unsigned
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
	DataType_double_long_unsigned,	
	0,0,0,0,
//  阶梯结算日数组  array 阶梯结算日
	DataType_array,	
	4,
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//阶梯结算日∷=structure
	DataType_structure,	
	3,
//{
//月  unsigned，
	DataType_unsigned,	
	0xFF,	
//  日  unsigned，
	DataType_unsigned,	
	0xFF,	
//  时  unsigned
	DataType_unsigned,	
	0xFF,	
//}	
//}
//阶梯值：单位：kWh，换算：-2
//阶梯电价：单位：元/kWh，换算：-4
//阶梯结算日中月、日、时均有效时则按结算日执行阶梯冻结，如果第1阶梯结算日中仅月、日有效则以此每月该日时执行阶梯冻结，当全部无效时不执行阶梯冻结。
//当前套阶梯参数协议只读。
};


//#define ADDR_401C     ADDR_401B+LENmax_401B//电流互感器变比
//#define LENmax_401C     8
const u8 InitData_401C[]=
{
//属性2∷=double-long-unsigned，无单位，换算:-2
	DataType_double_long_unsigned,
	0,0,0,1,
};

//#define ADDR_401D     ADDR_401C+LENmax_401C//电压互感器变比
//#define LENmax_401D     8
const u8 InitData_401D[]=
{
//属性2∷=double-long-unsigned，无单位，换算:-2
	DataType_double_long_unsigned,
	0,0,0,1,
};

//#define ADDR_401E     ADDR_401D+LENmax_401D//报警金额限值
//#define LENmax_401E     12
const u8 InitData_401E[]=
{
//属性2（参数）∷=structure
	DataType_structure,	
	2,
//{
//报警金额限值1    double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//报警金额限值2    double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//单位：元，换算：-2
};

//#define ADDR_401F     ADDR_401E+LENmax_401E//其它金额限值
//#define LENmax_401F     18
const u8 InitData_401F[]=
{
//属性2（参数）∷=structure
	DataType_structure,	
	3,
//{
//透支金额限值     double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//囤积金额限值     double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//合闸允许金额限值 double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//单位：元，换算：-2
};

//#define ADDR_4020     ADDR_401F+LENmax_401F//报警电量限值
//#define LENmax_4020     12
const u8 InitData_4020[]=
{
//属性2∷=structure
	DataType_structure,	
	2,
//{
//报警电量限值1    double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//报警电量限值2    double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//单位：kWh，换算：-2
};

//#define ADDR_4021     ADDR_4020+LENmax_4020//其它电量限值
//#define LENmax_4021     18
const u8 InitData_4021[]=
{
//属性2∷=structure
	DataType_structure,	
	3,
//{
//囤积电量限值     double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//透支电量限值     double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//合闸允许电量限值 double-long-unsigned，
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//单位：kWh，换算：-2
};
/*
//#define ADDR_4022     ADDR_4021+LENmax_4021//插卡状态字
//#define LENmax_4022     4
const u8 InitData_4022[]=
{
//属性2∷=bit-string(SIZE(16))，无单位，无换算
	DataType_bit_string,	
	16,
	0x00,0x00,	
};
*/
//#define ADDR_4023     ADDR_4022+LENmax_4022//认证有效时长
//#define LENmax_4023     3
const u8 InitData_4023[]=
{
//属性2∷=long-unsigned ，单位：分钟，无换算
	DataType_long_unsigned,	
	0,0,
};

//#define ADDR_4024     ADDR_4023+LENmax_4023//剔除
//#define LENmax_4024     2
const u8 InitData_4024[]=
{
//属性2∷=enum {剔除投入（1），剔除解除（2）}
	DataType_enum,	
	2,
};

//#define ADDR_4030     ADDR_4024+LENmax_4024//电压合格率参数
//#define LENmax_4030     14
const u8 InitData_4030[]=
{
//属性2∷=structure
	DataType_structure,
  4,	
//{
#if MeterUBhex==2200//参比电压xxx.x(V)
//电压考核上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	2640>>8,2640&0xff,
//  电压考核下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	1540>>8,1540&0xff,
//电压合格上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	2354>>8,2354&0xff,
//  电压合格下限  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
	2046>>8,2046&0xff,
#endif
#if MeterUBhex==1000//参比电压xxx.x(V)
//电压考核上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	1200>>8,1200&0xff,
//  电压考核下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	780>>8,780&0xff,
//电压合格上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	1070>>8,1070&0xff,
//  电压合格下限  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
	900>>8,900&0xff,
#endif
#if MeterUBhex==577//参比电压xxx.x(V)
//电压考核上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	692>>8,692&0xff,
//  电压考核下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	450>>8,450&0xff,
//电压合格上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	617>>8,617&0xff,
//  电压合格下限  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
	519>>8,519&0xff,
#endif
//}
};

//#define ADDR_4100     ADDR_4030+LENmax_4030//最大需量周期
//#define LENmax_4100     2
const u8 InitData_4100[]=
{
//属性2∷=unsigned，单位：分钟，换算：0
	DataType_unsigned,
	15,
};

//#define ADDR_4101     ADDR_4100+LENmax_4100//滑差时间
//#define LENmax_4101     2
const u8 InitData_4101[]=
{
//属性2∷=unsigned，单位：分钟，换算：0
	DataType_unsigned,
	1,
};

//#define ADDR_4102     ADDR_4101+LENmax_4101//校表脉冲宽度
//#define LENmax_4102     2
const u8 InitData_4102[]=
{
//属性2∷=unsigned，单位：毫秒，换算：0
	DataType_unsigned,
	80,
};

//#define ADDR_4103     ADDR_4102+LENmax_4102//资产管理编码
//#define LENmax_4103     34
const u8 InitData_4103[]=
{
//属性2∷=visible-string(SIZE(32))
	DataType_visible_string,
	32,
	'0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1',
};

//#define ADDR_410C     ADDR_410B+LENmax_410B//ABC各相电导系数
//#define LENmax_410C     12
const u8 InitData_410C[]=
{
//属性2∷=structure
	DataType_structure,
	3,
//{
//A 相电导  long，单位：无，换算：-3
	DataType_long,
	0,0,
//B 相电导  long，单位：无，换算：-3
	DataType_long,
	0,0,
//C 相电导  long，单位：无，换算：-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_410D     ADDR_410C+LENmax_410C//ABC各相电抗系数
//#define LENmax_410D     12
const u8 InitData_410D[]=
{
//属性2∷=structure
	DataType_structure,
	3,
//{
//A 相电抗  long，单位：无，换算：-3
	DataType_long,
	0,0,
//B 相电抗  long，单位：无，换算：-3
	DataType_long,
	0,0,
//C 相电抗  long，单位：无，换算：-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_410E     ADDR_410D+LENmax_410D//ABC各相电阻系数
//#define LENmax_410E     12
const u8 InitData_410E[]=
{
//属性2∷=structure
	DataType_structure,
	3,
//{
//A 相电阻  long，单位：无，换算：-3
	DataType_long,
	0,0,
//B 相电阻  long，单位：无，换算：-3
	DataType_long,
	0,0,
//C 相电阻  long，单位：无，换算：-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_410F     ADDR_410E+LENmax_410E//ABC各相电纳系数
//#define LENmax_410F     12
const u8 InitData_410F[]=
{
//属性2∷=structure
	DataType_structure,
	3,
//{
//A 相电纳  long，单位：无，换算：-3
	DataType_long,
	0,0,
//B 相电纳  long，单位：无，换算：-3
	DataType_long,
	0,0,
//C 相电纳  long，单位：无，换算：-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_4112     ADDR_4111+LENmax_4111//有功组合方式特征字
//#define LENmax_4112     3
const u8 InitData_4112[]=
{
//属性2∷=bit-string(SIZE(8))，见附录G
	DataType_bit_string,
	8,
	0xA0,	
};

//#define ADDR_4113     ADDR_4112+LENmax_4112//无功组合方式1特征字
//#define LENmax_4113     3
const u8 InitData_4113[]=
{
//属性2∷=bit-string(SIZE(8))，见附录G
	DataType_bit_string,
	8,
	0x82,	
};

//#define ADDR_4114     ADDR_4113+LENmax_4113//无功组合方式2特征字
//#define LENmax_4114     3
const u8 InitData_4114[]=
{
//属性2∷=bit-string(SIZE(8))，见附录G
	DataType_bit_string,
	8,
	0x28,	
};

//#define ADDR_4115     ADDR_4114+LENmax_4114//IC卡
//#define LENmax_4115     4
const u8 InitData_4115[]=
{
//属性2∷=structure
	DataType_structure,
	1,
//{
//IC卡类型  enum
//{
//参数预制卡（0），用户开户卡（1），
//用户购电卡（2），用户补卡（3）
//}，
	DataType_enum,
	2,
//}
};

//#define ADDR_4116     ADDR_4115+LENmax_4115//结算日
//#define LENper_4116     6
//#define NUMmax_4116     31
//#define LENmax_4116     (2+(LENper_4116*NUMmax_4116))
const u8 InitData_4116[]=
{
//属性2（配置参数）∷=array 结算日日期
	DataType_array,
	1,
//结算日日期∷=structure
	DataType_structure,
	2,
//{
//日  unsigned,
	DataType_unsigned,
	1,
//时  unsigned
	DataType_unsigned,
	0,
//}
};

//#define ADDR_4117     ADDR_4116+LENmax_4116//期间需量冻结周期
//#define LENmax_4117     5
const u8 InitData_4117[]=
{
//属性2（配置参数）∷= TI
	DataType_TI,
	01,0,15,	
};




//***终端类***

//4200	11	路由表
//4201	8	路由信息单元
//#define ADDR_4202     ADDR_4117+LENmax_4117//级联通信参数
//#define LENmax_4202     128
const u8 InitData_4202[]=
{
//属性2∷=structure
	DataType_structure,
	8,
//{
//级联标志            bool，
	DataType_bool,
	0,
//级联通信端口号      OAD，
	DataType_OAD,
	0xF2,0x01,0x02,0x01,
//总等待超时（10ms）  long-unsigned，
	DataType_long_unsigned,
	0,0,
//字节超时（10ms）    long-unsigned，
	DataType_long_unsigned,
	0,0,
//重发次数            unsigned，
	DataType_unsigned,
	0,
//巡测周期（min）     unsigned，
	DataType_unsigned,
	0,
//级联（被）端口数    unsigned，
	DataType_unsigned,
	0,
//级联（被）终端地址  array TSA
	DataType_array,
	0,
//}
};

//#define ADDR_4204_2     ADDR_4202+LENmax_4202//终端广播校时-2
//#define LENmax_4204_2     16
const u8 InitData_4204_2[]=
{
//属性2（终端广播校时参数）∷=structure
	DataType_structure,
	2,
//{
//     终端广播校时启动时间  time，
	DataType_time,
	0x00,0x00,0x00,
//     是否启用              bool
	DataType_bool,
	0,
//}
};

//#define ADDR_4204_3     ADDR_4204_2+LENmax_4204_2//终端广播校时-3
//#define LENmax_4204_3     16
const u8 InitData_4204_3[]=
{
//属性3（终端单地址广播校时参数）∷=structure
	DataType_structure,
	3,
//{
//     时钟误差阈值          unsigned（单位：秒），
	DataType_integer,
	0,
//     终端广播校时启动时间  time，
	DataType_time,
	0x00,0x00,0x00,
//     是否启用              bool
	DataType_bool,
	0,
//}
//电表时钟误差由终端自动计算获得，前提是采集任务中配置有采集电表时钟。执行单地址广播校时后生成对应的终端对电表校时事件
};






//---冻结对象类---
//瞬时冻结关联对象属性表
const u8 InitData_5000[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};

//秒冻结关联对象属性表
const u8 InitData_5001[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};

//分钟冻结关联对象属性表
const u8 InitData_5002[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	11,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
//正向有功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00100200>>24)&0xff,(0x00100200>>16)&0xff,(0x00100200>>8)&0xff,(0x00100200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//反向有功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00200200>>24)&0xff,(0x00200200>>16)&0xff,(0x00200200>>8)&0xff,(0x00200200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//1象限无功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00500200>>24)&0xff,(0x00500200>>16)&0xff,(0x00500200>>8)&0xff,(0x00500200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//2象限无功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00600200>>24)&0xff,(0x00600200>>16)&0xff,(0x00600200>>8)&0xff,(0x00600200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//3象限无功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00700200>>24)&0xff,(0x00700200>>16)&0xff,(0x00700200>>8)&0xff,(0x00700200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//4象限无功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00800200>>24)&0xff,(0x00800200>>16)&0xff,(0x00800200>>8)&0xff,(0x00800200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//电压
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x20000200>>24)&0xff,(0x20000200>>16)&0xff,(0x20000200>>8)&0xff,(0x20000200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//电流
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x20010200>>24)&0xff,(0x20010200>>16)&0xff,(0x20010200>>8)&0xff,(0x20010200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//有功功率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x20040200>>24)&0xff,(0x20040200>>16)&0xff,(0x20040200>>8)&0xff,(0x20040200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//无功功率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x20050200>>24)&0xff,(0x20050200>>16)&0xff,(0x20050200>>8)&0xff,(0x20050200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//功率因率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x200A0200>>24)&0xff,(0x200A0200>>16)&0xff,(0x200A0200>>8)&0xff,(0x200A0200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
	
};
//小时冻结关联对象属性表
const u8 InitData_5003[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};

//日冻结关联对象属性表
const u8 InitData_5004[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	9,
	
//正向有功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00100200>>24)&0xff,(0x00100200>>16)&0xff,(0x00100200>>8)&0xff,(0x00100200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//反向有功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00200200>>24)&0xff,(0x00200200>>16)&0xff,(0x00200200>>8)&0xff,(0x00200200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//组合无功1电能
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00300200>>24)&0xff,(0x00300200>>16)&0xff,(0x00300200>>8)&0xff,(0x00300200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//组合无功2电能
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00400200>>24)&0xff,(0x00400200>>16)&0xff,(0x00400200>>8)&0xff,(0x00400200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//正向有功最大需量
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x10100200>>24)&0xff,(0x10100200>>16)&0xff,(0x10100200>>8)&0xff,(0x10100200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//反向有功最大需量
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x10200200>>24)&0xff,(0x10200200>>16)&0xff,(0x10200200>>8)&0xff,(0x10200200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//当月A相电压合格率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x21310200>>24)&0xff,(0x21310200>>16)&0xff,(0x21310200>>8)&0xff,(0x21310200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//当月B相电压合格率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x21320200>>24)&0xff,(0x21320200>>16)&0xff,(0x21320200>>8)&0xff,(0x21320200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//当月C相电压合格率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x21330200>>24)&0xff,(0x21330200>>16)&0xff,(0x21330200>>8)&0xff,(0x21330200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
	
};
//结算日冻结关联对象属性表
const u8 InitData_5005[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};

//月冻结关联对象属性表
const u8 InitData_5006[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	9,
	
//正向有功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00100200>>24)&0xff,(0x00100200>>16)&0xff,(0x00100200>>8)&0xff,(0x00100200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}
	
//反向有功
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00200200>>24)&0xff,(0x00200200>>16)&0xff,(0x00200200>>8)&0xff,(0x00200200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//组合无功1电能
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00300200>>24)&0xff,(0x00300200>>16)&0xff,(0x00300200>>8)&0xff,(0x00300200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//组合无功2电能
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x00400200>>24)&0xff,(0x00400200>>16)&0xff,(0x00400200>>8)&0xff,(0x00400200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//正向有功最大需量
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x10100200>>24)&0xff,(0x10100200>>16)&0xff,(0x10100200>>8)&0xff,(0x10100200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//反向有功最大需量
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x10200200>>24)&0xff,(0x10200200>>16)&0xff,(0x10200200>>8)&0xff,(0x10200200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//当月A相电压合格率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x21310200>>24)&0xff,(0x21310200>>16)&0xff,(0x21310200>>8)&0xff,(0x21310200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}
	
//当月B相电压合格率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x21320200>>24)&0xff,(0x21320200>>16)&0xff,(0x21320200>>8)&0xff,(0x21320200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}
	
//当月C相电压合格率
//一个关联的对象属性∷=structure
	DataType_structure,
	3,
//{
//  冻结周期  long-unsigned，
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  关联对象属性描述符  OAD，
	DataType_OAD,
	(0x21330200>>24)&0xff,(0x21330200>>16)&0xff,(0x21330200>>8)&0xff,(0x21330200>>0)&0xff,
//  存储深度  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}


};

//年冻结关联对象属性表
const u8 InitData_5007[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};
//时区表切换冻结关联对象属性表
const u8 InitData_5008[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};
//日时段表切换冻结关联对象属性表
const u8 InitData_5009[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};
//费率电价切换冻结关联对象属性表
const u8 InitData_500A[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};
//阶梯切换冻结关联对象属性表
const u8 InitData_500B[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};
//阶梯结算冻结关联对象属性表
const u8 InitData_5011[]=
{
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
	DataType_array,
	0,
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
};





#endif






