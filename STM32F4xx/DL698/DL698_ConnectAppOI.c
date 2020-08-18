
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_CONNECT.h"
#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"




//应用连接
//对象列表∷=array 一个可访问对象
//一个可访问对象∷=structure
//{
//   对象标识  OI，
//   访问权限  structure
//}
//访问权限∷=structure
//{
//属性访问权限  array 一个属性访问权限，
//方法访问权限  array 一个方法访问权限
//}
//一个属性访问权限∷=structure
//{
//属性ID           unsigned，
//属性访问权限类别  enum
//{
//不可访问（0）， 
//只读（1），
//只写（2），
//可读写（3）
//}
//}
//一个方法访问权限∷=structure
//{
//方法ID        unsigned，
//方法访问权限   bool
//}
__align(4) const u8 ConnectAppOI_LIST[]=
{
	DataType_array,0x82,
	(162>>8),162&0xff,//总列表个数
//1
	DataType_structure,2,
	DataType_OI,0x00,0x00,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//2
	DataType_structure,2,
	DataType_OI,0x00,0x10,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//3
	DataType_structure,2,
	DataType_OI,0x00,0x11,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//4	
	DataType_structure,2,
	DataType_OI,0x00,0x12,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//5	
	DataType_structure,2,
	DataType_OI,0x00,0x13,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//6
	DataType_structure,2,
	DataType_OI,0x00,0x20,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//7
	DataType_structure,2,
	DataType_OI,0x00,0x21,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//8
	DataType_structure,2,
	DataType_OI,0x00,0x22,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//9
	DataType_structure,2,
	DataType_OI,0x00,0x23,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//10
	DataType_structure,2,
	DataType_OI,0x00,0x30,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//11
	DataType_structure,2,
	DataType_OI,0x00,0x31,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//12
	DataType_structure,2,
	DataType_OI,0x00,0x32,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//13
	DataType_structure,2,
	DataType_OI,0x00,0x33,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//14
	DataType_structure,2,
	DataType_OI,0x00,0x40,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//15
	DataType_structure,2,
	DataType_OI,0x00,0x41,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//16
	DataType_structure,2,
	DataType_OI,0x00,0x42,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//17
	DataType_structure,2,
	DataType_OI,0x00,0x43,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//18
	DataType_structure,2,
	DataType_OI,0x00,0x50,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//19
	DataType_structure,2,
	DataType_OI,0x00,0x51,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//20
	DataType_structure,2,
	DataType_OI,0x00,0x52,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//21
	DataType_structure,2,
	DataType_OI,0x00,0x53,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//22
	DataType_structure,2,
	DataType_OI,0x00,0x60,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//23
	DataType_structure,2,
	DataType_OI,0x00,0x61,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//24
	DataType_structure,2,
	DataType_OI,0x00,0x62,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//25
	DataType_structure,2,
	DataType_OI,0x00,0x63,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//26
	DataType_structure,2,
	DataType_OI,0x00,0x70,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//27
	DataType_structure,2,
	DataType_OI,0x00,0x71,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//28
	DataType_structure,2,
	DataType_OI,0x00,0x72,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//29
	DataType_structure,2,
	DataType_OI,0x00,0x73,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//30
	DataType_structure,2,
	DataType_OI,0x00,0x80,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//31
	DataType_structure,2,
	DataType_OI,0x00,0x81,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//32
	DataType_structure,2,
	DataType_OI,0x00,0x82,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//33
	DataType_structure,2,
	DataType_OI,0x00,0x83,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//34
	DataType_structure,2,
	DataType_OI,0x00,0x90,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//35
	DataType_structure,2,
	DataType_OI,0x00,0x91,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//36
	DataType_structure,2,
	DataType_OI,0x00,0x92,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//37
	DataType_structure,2,
	DataType_OI,0x00,0x93,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//38
	DataType_structure,2,
	DataType_OI,0x00,0xA0,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//39
	DataType_structure,2,
	DataType_OI,0x00,0xA1,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//40
	DataType_structure,2,
	DataType_OI,0x00,0xA2,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//41
	DataType_structure,2,
	DataType_OI,0x00,0xA3,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//42
	DataType_structure,2,
	DataType_OI,0x01,0x10,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//43
	DataType_structure,2,
	DataType_OI,0x01,0x11,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//44
	DataType_structure,2,
	DataType_OI,0x01,0x12,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//45
	DataType_structure,2,
	DataType_OI,0x01,0x13,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//46
	DataType_structure,2,
	DataType_OI,0x01,0x20,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//47
	DataType_structure,2,
	DataType_OI,0x01,0x21,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//48
	DataType_structure,2,
	DataType_OI,0x01,0x22,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//49
	DataType_structure,2,
	DataType_OI,0x01,0x23,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//50
	DataType_structure,2,
	DataType_OI,0x02,0x10,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//51
	DataType_structure,2,
	DataType_OI,0x02,0x11,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//52
	DataType_structure,2,
	DataType_OI,0x02,0x12,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//53
	DataType_structure,2,
	DataType_OI,0x02,0x13,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//54
	DataType_structure,2,
	DataType_OI,0x02,0x20,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//55
	DataType_structure,2,
	DataType_OI,0x02,0x21,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//56
	DataType_structure,2,
	DataType_OI,0x02,0x22,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//57
	DataType_structure,2,
	DataType_OI,0x02,0x23,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//58
	DataType_structure,2,
	DataType_OI,0x03,0x00,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//59
	DataType_structure,2,
	DataType_OI,0x03,0x01,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//60
	DataType_structure,2,
	DataType_OI,0x03,0x02,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//61
	DataType_structure,2,
	DataType_OI,0x03,0x03,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//62
	DataType_structure,2,
	DataType_OI,0x04,0x00,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//63
	DataType_structure,2,
	DataType_OI,0x04,0x01,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//64
	DataType_structure,2,
	DataType_OI,0x04,0x02,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//65
	DataType_structure,2,
	DataType_OI,0x04,0x03,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//66
	DataType_structure,2,
	DataType_OI,0x05,0x00,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//67
	DataType_structure,2,
	DataType_OI,0x05,0x01,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//68
	DataType_structure,2,
	DataType_OI,0x05,0x02,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//69
	DataType_structure,2,
	DataType_OI,0x05,0x03,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//70
	DataType_structure,2,
	DataType_OI,0x10,0x10,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//71
	DataType_structure,2,
	DataType_OI,0x10,0x11,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//72
	DataType_structure,2,
	DataType_OI,0x10,0x12,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//73
	DataType_structure,2,
	DataType_OI,0x10,0x13,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//74
	DataType_structure,2,
	DataType_OI,0x10,0x20,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//75
	DataType_structure,2,
	DataType_OI,0x10,0x21,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//76
	DataType_structure,2,
	DataType_OI,0x10,0x22,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//77
	DataType_structure,2,
	DataType_OI,0x10,0x23,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//78
	DataType_structure,2,
	DataType_OI,0x10,0x30,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//79
	DataType_structure,2,
	DataType_OI,0x10,0x31,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//80
	DataType_structure,2,
	DataType_OI,0x10,0x32,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//81
	DataType_structure,2,
	DataType_OI,0x10,0x33,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//82
	DataType_structure,2,
	DataType_OI,0x10,0x40,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//83
	DataType_structure,2,
	DataType_OI,0x10,0x41,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//84
	DataType_structure,2,
	DataType_OI,0x10,0x42,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//85
	DataType_structure,2,
	DataType_OI,0x10,0x43,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//86
	DataType_structure,2,
	DataType_OI,0x10,0x50,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//87
	DataType_structure,2,
	DataType_OI,0x10,0x51,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//88
	DataType_structure,2,
	DataType_OI,0x10,0x52,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//89
	DataType_structure,2,
	DataType_OI,0x10,0x53,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//90
	DataType_structure,2,
	DataType_OI,0x10,0x60,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//91
	DataType_structure,2,
	DataType_OI,0x10,0x61,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//92
	DataType_structure,2,
	DataType_OI,0x10,0x62,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//93
	DataType_structure,2,
	DataType_OI,0x10,0x63,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//94
	DataType_structure,2,
	DataType_OI,0x10,0x70,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//95
	DataType_structure,2,
	DataType_OI,0x10,0x71,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//96
	DataType_structure,2,
	DataType_OI,0x10,0x72,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//97
	DataType_structure,2,
	DataType_OI,0x10,0x73,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//98
	DataType_structure,2,
	DataType_OI,0x10,0x80,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//99
	DataType_structure,2,
	DataType_OI,0x10,0x81,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//100
	DataType_structure,2,
	DataType_OI,0x10,0x82,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//101
	DataType_structure,2,
	DataType_OI,0x10,0x83,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//102
	DataType_structure,2,
	DataType_OI,0x10,0x90,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//103
	DataType_structure,2,
	DataType_OI,0x10,0x91,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//104
	DataType_structure,2,
	DataType_OI,0x10,0x92,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//105
	DataType_structure,2,
	DataType_OI,0x10,0x93,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//106
	DataType_structure,2,
	DataType_OI,0x10,0xA0,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//107
	DataType_structure,2,
	DataType_OI,0x10,0xA1,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//108
	DataType_structure,2,
	DataType_OI,0x10,0xA2,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//109
	DataType_structure,2,
	DataType_OI,0x10,0xA3,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//110
	DataType_structure,2,
	DataType_OI,0x11,0x10,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//111
	DataType_structure,2,
	DataType_OI,0x11,0x11,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//112
	DataType_structure,2,
	DataType_OI,0x11,0x12,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//113
	DataType_structure,2,
	DataType_OI,0x11,0x13,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//114
	DataType_structure,2,
	DataType_OI,0x11,0x20,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//115
	DataType_structure,2,
	DataType_OI,0x11,0x21,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//116
	DataType_structure,2,
	DataType_OI,0x11,0x22,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//117
	DataType_structure,2,
	DataType_OI,0x11,0x23,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//118
	DataType_structure,2,
	DataType_OI,0x11,0x30,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//119
	DataType_structure,2,
	DataType_OI,0x11,0x31,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//120
	DataType_structure,2,
	DataType_OI,0x11,0x32,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//121
	DataType_structure,2,
	DataType_OI,0x11,0x33,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//122
	DataType_structure,2,
	DataType_OI,0x11,0x40,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//123
	DataType_structure,2,
	DataType_OI,0x11,0x41,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//124
	DataType_structure,2,
	DataType_OI,0x11,0x42,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//125
	DataType_structure,2,
	DataType_OI,0x11,0x43,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//126
	DataType_structure,2,
	DataType_OI,0x11,0x50,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//127
	DataType_structure,2,
	DataType_OI,0x11,0x51,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//128
	DataType_structure,2,
	DataType_OI,0x11,0x52,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//129
	DataType_structure,2,
	DataType_OI,0x11,0x53,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//130
	DataType_structure,2,
	DataType_OI,0x11,0x60,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//131
	DataType_structure,2,
	DataType_OI,0x11,0x61,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//132
	DataType_structure,2,
	DataType_OI,0x11,0x62,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//133
	DataType_structure,2,
	DataType_OI,0x11,0x63,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//134
	DataType_structure,2,
	DataType_OI,0x11,0x70,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//135
	DataType_structure,2,
	DataType_OI,0x11,0x71,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//136
	DataType_structure,2,
	DataType_OI,0x11,0x72,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//137
	DataType_structure,2,
	DataType_OI,0x11,0x73,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//138
	DataType_structure,2,
	DataType_OI,0x11,0x80,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//139
	DataType_structure,2,
	DataType_OI,0x11,0x81,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//140
	DataType_structure,2,
	DataType_OI,0x11,0x82,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//141
	DataType_structure,2,
	DataType_OI,0x11,0x83,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//142
	DataType_structure,2,
	DataType_OI,0x11,0x90,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//143
	DataType_structure,2,
	DataType_OI,0x11,0x91,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//144
	DataType_structure,2,
	DataType_OI,0x11,0x92,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//145
	DataType_structure,2,
	DataType_OI,0x11,0x93,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//146
	DataType_structure,2,
	DataType_OI,0x11,0xA0,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//147
	DataType_structure,2,
	DataType_OI,0x11,0xA1,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//148
	DataType_structure,2,
	DataType_OI,0x11,0xA2,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//149
	DataType_structure,2,
	DataType_OI,0x11,0xA3,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//150
	DataType_structure,2,
	DataType_OI,0x20,0x00,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//151
	DataType_structure,2,
	DataType_OI,0x20,0x01,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//152
	DataType_structure,2,
	DataType_OI,0x20,0x02,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//153
	DataType_structure,2,
	DataType_OI,0x20,0x03,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//154
	DataType_structure,2,
	DataType_OI,0x20,0x04,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//155
	DataType_structure,2,
	DataType_OI,0x20,0x05,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//156
	DataType_structure,2,
	DataType_OI,0x20,0x06,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//157
	DataType_structure,2,
	DataType_OI,0x20,0x07,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//158
	DataType_structure,2,
	DataType_OI,0x20,0x08,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//159
	DataType_structure,2,
	DataType_OI,0x20,0x09,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//160
	DataType_structure,2,
	DataType_OI,0x20,0x0A,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//161
	DataType_structure,2,
	DataType_OI,0x20,0x0B,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//162
	DataType_structure,2,
	DataType_OI,0x20,0x0C,//对象标识  OI
	DataType_structure,2,//访问权限
	//属性访问权限  array 一个属性访问权限
	DataType_array,
	2,//n个属性访问权限
	//第1个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	2,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	//第2个属性访问权限
	DataType_structure,2,
	DataType_unsigned,
	3,//属性ID
	DataType_enum,
	1,//0=不可访问,1=只读,2=只写,3=可读写
	
	//方法访问权限  array 一个方法访问权限
	DataType_array,
	0,//n方法访问权限
	//方法ID
	//方法访问权限   bool
//163

	
	
	
	
	
	
};


//应用语境信息∷=structure
//{
//协议版本信息        long-unsigned，
//最大接收APDU尺寸   long-unsigned，
//最大发送APDU尺寸   long-unsigned，
//最大可处理APDU尺寸 long-unsigned，
//协议一致性块        bit-string(64)，
//功能一致性块        bit-string(128)，
//静态超时时间       double-long-unsigned
//}
const u8 UART_SIZE_LIST[]=
{
	DataType_structure,7,
//协议版本信息        long-unsigned，
	DataType_long_unsigned,
	SVER>>8,SVER&0xff,
//最大接收APDU尺寸   long-unsigned，
	DataType_long_unsigned,
	LENmax_APDU>>8,LENmax_APDU&0xff,
//最大发送APDU尺寸   long-unsigned，
	DataType_long_unsigned,
	2048>>8,2048&0xff,
//最大可处理APDU尺寸 long-unsigned，
	DataType_long_unsigned,
	LENmax_APDU>>8,LENmax_APDU&0xff,
//协议一致性块        bit-string(64)，
	DataType_bit_string,64,
	(Protocol0<<7)+(Protocol1<<6)+(Protocol2<<5)+(Protocol3<<4)+(Protocol4<<3)+(Protocol5<<2)+(Protocol6<<1)+(Protocol7<<0),
	(Protocol8<<7)+(Protocol9<<6)+(Protocol10<<5)+(Protocol11<<4)+(Protocol12<<3)+(Protocol13<<2)+(Protocol14<<1)+(Protocol15<<0),
	(Protocol16<<7)+(Protocol17<<6)+(Protocol18<<5)+(Protocol19<<4)+(Protocol20<<3)+(Protocol21<<2)+(Protocol22<<1)+(Protocol23<<0),
	(Protocol24<<7)+(Protocol25<<6)+(Protocol26<<5)+(Protocol27<<4)+(Protocol28<<3)+(Protocol29<<2)+(Protocol30<<1)+(Protocol31<<0),
	(Protocol32<<7)+(Protocol33<<6)+(Protocol34<<5)+(Protocol35<<4)+(Protocol36<<3)+(Protocol37<<2)+(Protocol38<<1)+(Protocol39<<0),
	0,
	0,
	0,
//功能一致性块        bit-string(128)，
	DataType_bit_string,0x82,0,128,
	(Function0<<7)+(Function1<<6)+(Function2<<5)+(Function3<<4)+(Function4<<3)+(Function5<<2)+(Function6<<1)+(Function7<<0),
	(Function8<<7)+(Function9<<6)+(Function10<<5)+(Function11<<4)+(Function12<<3)+(Function13<<2)+(Function14<<1)+(Function15<<0),
	(Function16<<7)+(Function17<<6)+(Function18<<5)+(Function19<<4)+(Function20<<3)+(Function21<<2)+(Function22<<1)+(Function23<<0),
	(Function24<<7)+(Function25<<6)+(Function26<<5)+(Function27<<4)+(Function28<<3)+(Function29<<2)+(Function30<<1)+(Function31<<0),
	(Function32<<7)+(Function33<<6)+(Function34<<5)+(Function35<<4)+(Function36<<3)+(Function37<<2)+(Function38<<1)+(Function39<<0),
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
//静态超时时间       double-long-unsigned
	DataType_double_long_unsigned,
	0,0,2000>>8,2000&0xff,
};



u32 GET_OAD_ConnectApp(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//读应用连接可访问对象列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 Array;
	u32 Len;
	u32 NUMmax;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	switch(OAD)
	{
		case 0x44000200://应用连接:对象列表∷=array 一个可访问对象
			p8tx[0]=DataType_array;
			Len=2;
			Array=0;//填入数组数
			NUMmax=(ConnectAppOI_LIST[2]<<8)|ConnectAppOI_LIST[3];
			n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
			p8=(u8*)ConnectAppOI_LIST+4;
			for(i=0;i<n;i++)
			{
				p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
			}
			while(1)
			{
				if(n>=NUMmax)
				{
					break;
				}
				x=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
				if(x>LENmax_TxSPACE)
				{//发送缓冲不够,分帧
					if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					{
						UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					else
					{
						UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
					p8tx[1]=Array;//填入配置组数
					return Len;
				}
				MR((u32)p8tx+Len,(u32)p8,x);
				p8+=x;
				Len+=x;
				LENmax_TxSPACE-=x;
				Array++;//填入配置组数
				n++;
			}
			if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			{
				UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			}
			UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
			p8tx[1]=Array;//填入配置组数
			return Len;
		case 0x44000300://应用连接:应用语境信息
			Len=sizeof(UART_SIZE_LIST);
			if(LENmax_TxSPACE<Len)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			MR((u32)p8tx,(u32)UART_SIZE_LIST,Len);
			return Len;
		case 0x44000400://应用连接:当前连接的客户机地址
			//当前连接的客户机地址∷=unsigned
			if(LENmax_TxSPACE<2)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			if(UARTCtrl->OAD_44000400[0]==DataType_unsigned)
			{
				p8tx[0]=DataType_unsigned;
				p8tx[1]=UARTCtrl->OAD_44000400[1];
				return 2;
			}
			p8tx[0]=0;
			return 1;
		case 0x44000500://应用连接:连接认证机制
			//连接认证机制∷=enum
			//{
			//  公共连接   (0)，
			//  普通密码   (1)，
			//  对称加密   (2)，
			//  数字签名   (3)
			//}
			if(LENmax_TxSPACE<2)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_enum;
			p8tx[1]=3;
			return 2;
		default:
			UARTCtrl->DAR=6;//数据访问结果
			return 0;
	}
}

#include "../MS/MS.h"
void TEST_ConnectAppOI_LIST(void)//测试对象列表
{
	u32 i;
	u32 x;
	u32 Len;
	u8* p8;
	u32 NUMmax;
	

	NUMmax=(ConnectAppOI_LIST[2]<<8)|ConnectAppOI_LIST[3];
	p8=(u8*)&ConnectAppOI_LIST+4;
	Len=0;
	for(i=0;i<NUMmax;i++)
	{
		x=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
		p8+=x;
		Len+=x;
	}
	Len+=4;
	x=sizeof(ConnectAppOI_LIST);
	if(Len!=x)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		DisplayStringAutoUp(0,9,(u8*)"\x0""可应用对象列表错");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	}
}





