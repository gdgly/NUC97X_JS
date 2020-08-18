
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"


//采集规则库(事件类在另外库)
//重要约定:
//采集规则库中当为ROAD时只能有1个关联的OAD,抄表流程会自动ROAD关联OAD计数
//当DL698为ROAD关联多个OAD时,数据存储时以单个关联OAD数组数据形式存储(无总关联个数数组),当其中一个关联OAD数据不存在时数据存储中自动不关联此OAD
//故DL2007或1997采集规则库关联的OAD以单个OAD方式列表(可以用多个DI来表示1个关联OAD!!!)
//不能在DL2007或1997采集规则库中1个列表表示多个关联的OAD,因无法区分多个DI是表示1个OAD还是多个OAD
//数据抄读接收时在抄表子计数为0时表示数组的开始也正因为此可以用多个DI来表示1个关联OAD


#if (USER/100)==9//河南系

//电能量类
const u8 InitData_601E_00000200[]=
{
//组合有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x00,0xff,0x00,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x00,0x00,0x00,
	DataType_octet_string,4,0x00,0x00,0x01,0x00,
	DataType_octet_string,4,0x00,0x00,0x02,0x00,
	DataType_octet_string,4,0x00,0x00,0x03,0x00,
	DataType_octet_string,4,0x00,0x00,0x04,0x00,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00100200[]=
{
//正向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x01,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x01,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x90,0x1f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x90,0x10,//DI
	DataType_octet_string,2,0x90,0x11,//DI
	DataType_octet_string,2,0x90,0x12,//DI
	DataType_octet_string,2,0x90,0x13,//DI
	DataType_octet_string,2,0x90,0x14,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00110200[]=
{
//A相正向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x15,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00120200[]=
{
//B相正向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x29,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00130200[]=
{
//C相正向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x3D,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00200200[]=
{
//反向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x02,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x02,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x90,0x2f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x90,0x20,//DI
	DataType_octet_string,2,0x90,0x21,//DI
	DataType_octet_string,2,0x90,0x22,//DI
	DataType_octet_string,2,0x90,0x23,//DI
	DataType_octet_string,2,0x90,0x24,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00210200[]=
{
//A相反向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x16,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00220200[]=
{
//B相反向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2A,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00230200[]=
{
//C相反向有功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x3E,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00300200[]=
{
//组合无功1电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x30,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x03,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x03,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x1f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x10,//DI
	DataType_octet_string,2,0x91,0x11,//DI
	DataType_octet_string,2,0x91,0x12,//DI
	DataType_octet_string,2,0x91,0x13,//DI
	DataType_octet_string,2,0x91,0x14,//DI	
	
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00310200[]=
{
//A相组合无功1电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x31,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x17,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00320200[]=
{
//B相组合无功1电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x32,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2B,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00330200[]=
{
//C相组合无功1电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x33,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x3F,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00400200[]=
{
//组合无功2电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x40,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x04,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x04,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x2f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x20,//DI
	DataType_octet_string,2,0x91,0x21,//DI
	DataType_octet_string,2,0x91,0x22,//DI
	DataType_octet_string,2,0x91,0x23,//DI
	DataType_octet_string,2,0x91,0x24,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00410200[]=
{
//A相组合无功2电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x41,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x18,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00420200[]=
{
//B相组合无功2电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x42,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2C,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00430200[]=
{
//C相组合无功2电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x43,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x40,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00500200[]=
{
//第一象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x50,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x05,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x05,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x3f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x30,//DI
	DataType_octet_string,2,0x91,0x31,//DI
	DataType_octet_string,2,0x91,0x32,//DI
	DataType_octet_string,2,0x91,0x33,//DI
	DataType_octet_string,2,0x91,0x34,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00510200[]=
{
//A相第一象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x51,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x19,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00520200[]=
{
//B相第一象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x52,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2D,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00530200[]=
{
//C相第一象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x53,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x41,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00600200[]=
{
//第二象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x60,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x06,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x06,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x5f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x50,//DI
	DataType_octet_string,2,0x91,0x51,//DI
	DataType_octet_string,2,0x91,0x52,//DI
	DataType_octet_string,2,0x91,0x53,//DI
	DataType_octet_string,2,0x91,0x54,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00610200[]=
{
//A相第二象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x61,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1A,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00620200[]=
{
//B相第二象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x62,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2E,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00630200[]=
{
//C相第二象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x63,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x42,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00700200[]=
{
//第三象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x70,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x07,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x07,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x6f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x60,//DI
	DataType_octet_string,2,0x91,0x61,//DI
	DataType_octet_string,2,0x91,0x62,//DI
	DataType_octet_string,2,0x91,0x63,//DI
	DataType_octet_string,2,0x91,0x64,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00710200[]=
{
//A相第三象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x71,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1B,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00720200[]=
{
//B相第三象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x72,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2F,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00730200[]=
{
//C相第三象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x73,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x43,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00800200[]=
{
//第四象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x80,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x08,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x08,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x4f,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x40,//DI
	DataType_octet_string,2,0x91,0x41,//DI
	DataType_octet_string,2,0x91,0x42,//DI
	DataType_octet_string,2,0x91,0x43,//DI
	DataType_octet_string,2,0x91,0x44,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00810200[]=
{
//A相第四象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x81,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1C,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00820200[]=
{
//B相第四象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x82,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x30,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00830200[]=
{
//C相第四象限无功电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x83,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x44,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00900200[]=
{
//正向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x90,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x09,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x09,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00910200[]=
{
//A相正向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x91,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1D,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00920200[]=
{
//B相正向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x92,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x31,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00930200[]=
{
//C相正向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x93,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x45,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00A00200[]=
{
//反向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA0,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x0A,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x0A,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00A10200[]=
{
//A相反向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA1,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1E,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00A20200[]=
{
//B相反向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA2,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x32,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_00A30200[]=
{
//C相反向视在电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA3,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x46,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01100200[]=
{
//正向有功基波总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x81,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01110200[]=
{
//A相正向有功基波电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x95,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01120200[]=
{
//B相正向有功基波电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xA9,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01130200[]=
{
//C相正向有功基波电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBD,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01200200[]=
{
//反向有功基波 总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x82,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01210200[]=
{
//A相反向有功基波电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x96,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01220200[]=
{
//B相反向有功基波电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAA,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_01230200[]=
{
//C相反向有功基波电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBE,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02100200[]=
{
//正向有功谐波 总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x83,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02110200[]=
{
//A相正向有功谐波 电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x97,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02120200[]=
{
//B相正向有功谐波 电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAB,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02130200[]=
{
//C相正向有功谐波 电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBF,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02200200[]=
{
//反向有功谐波 总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x84,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02210200[]=
{
//A相反向有功谐波 电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x98,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02220200[]=
{
//B相反向有功谐波 电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAC,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_02230200[]=
{
//C相反向有功谐波 电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xC0,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_03000200[]=
{
//铜损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x85,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_03010200[]=
{
//A相铜损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x01,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x99,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_03020200[]=
{
//B相铜损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x02,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAD,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_03030200[]=
{
//C相铜损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xC1,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_04000200[]=
{
//铁损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x86,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_04010200[]=
{
//A相铁损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x01,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x9A,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_04020200[]=
{
//B相铁损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x02,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAE,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_04030200[]=
{
//C相铁损有功总电能补偿量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xC2,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_05000200[]=
{
//关联总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x80,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_05010200[]=
{
//A相关联总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x01,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x94,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_05020200[]=
{
//B相关联总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x02,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xA8,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_05030200[]=
{
//C相关联总电能
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBC,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10100200[]=
{
//最大需量类
//正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x01,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x01,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x1F,//DI
	DataType_octet_string,2,0xB0,0x1F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA0,0x10,//DI
	DataType_octet_string,2,0xA0,0x11,//DI
	DataType_octet_string,2,0xA0,0x12,//DI
	DataType_octet_string,2,0xA0,0x13,//DI
	DataType_octet_string,2,0xA0,0x14,//DI
	DataType_octet_string,2,0xB0,0x10,//DI
	DataType_octet_string,2,0xB0,0x11,//DI
	DataType_octet_string,2,0xB0,0x12,//DI
	DataType_octet_string,2,0xB0,0x13,//DI
	DataType_octet_string,2,0xB0,0x14,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10110200[]=
{
//A相正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x15,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10120200[]=
{
//B相正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x29,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10130200[]=
{
//C相正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x3D,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10200200[]=
{
//反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x02,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x02,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x2F,//DI
	DataType_octet_string,2,0xB0,0x2F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA0,0x20,//DI
	DataType_octet_string,2,0xA0,0x21,//DI
	DataType_octet_string,2,0xA0,0x22,//DI
	DataType_octet_string,2,0xA0,0x23,//DI
	DataType_octet_string,2,0xA0,0x24,//DI
	DataType_octet_string,2,0xB0,0x20,//DI
	DataType_octet_string,2,0xB0,0x21,//DI
	DataType_octet_string,2,0xB0,0x22,//DI
	DataType_octet_string,2,0xB0,0x23,//DI
	DataType_octet_string,2,0xB0,0x24,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10210200[]=
{
//A相反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x16,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10220200[]=
{
//B相反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2A,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10230200[]=
{
//C相反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x3E,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10300200[]=
{
//组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x30,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x03,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x03,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x1F,//DI
	DataType_octet_string,2,0xB1,0x1F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA1,0x10,//DI
	DataType_octet_string,2,0xA1,0x11,//DI
	DataType_octet_string,2,0xA1,0x12,//DI
	DataType_octet_string,2,0xA1,0x13,//DI
	DataType_octet_string,2,0xA1,0x14,//DI
	DataType_octet_string,2,0xB1,0x10,//DI
	DataType_octet_string,2,0xB1,0x11,//DI
	DataType_octet_string,2,0xB1,0x12,//DI
	DataType_octet_string,2,0xB1,0x13,//DI
	DataType_octet_string,2,0xB1,0x14,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10310200[]=
{
//A相组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x31,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x17,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10320200[]=
{
//B相组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x32,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2B,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10330200[]=
{
//C相组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x33,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x3F,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10400200[]=
{
//组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x40,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x04,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x04,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x2F,//DI
	DataType_octet_string,2,0xB1,0x2F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA1,0x20,//DI
	DataType_octet_string,2,0xA1,0x21,//DI
	DataType_octet_string,2,0xA1,0x22,//DI
	DataType_octet_string,2,0xA1,0x23,//DI
	DataType_octet_string,2,0xA1,0x24,//DI
	DataType_octet_string,2,0xB1,0x20,//DI
	DataType_octet_string,2,0xB1,0x21,//DI
	DataType_octet_string,2,0xB1,0x22,//DI
	DataType_octet_string,2,0xB1,0x23,//DI
	DataType_octet_string,2,0xB1,0x24,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10410200[]=
{
//A相组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x41,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x18,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10420200[]=
{
//B相组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x42,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2C,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10430200[]=
{
//C相组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x43,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x40,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10500200[]=
{
//第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x50,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x05,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x05,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x3F,//DI
	DataType_octet_string,2,0xB1,0x3F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA1,0x30,//DI
	DataType_octet_string,2,0xA1,0x31,//DI
	DataType_octet_string,2,0xA1,0x32,//DI
	DataType_octet_string,2,0xA1,0x33,//DI
	DataType_octet_string,2,0xA1,0x34,//DI
	DataType_octet_string,2,0xB1,0x30,//DI
	DataType_octet_string,2,0xB1,0x31,//DI
	DataType_octet_string,2,0xB1,0x32,//DI
	DataType_octet_string,2,0xB1,0x33,//DI
	DataType_octet_string,2,0xB1,0x34,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10510200[]=
{
//A相第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x51,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x19,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10520200[]=
{
//B相第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x52,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2D,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10530200[]=
{
//C相第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x53,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x41,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10600200[]=
{
//第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x60,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x06,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x06,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x5F,//DI
	DataType_octet_string,2,0xB1,0x5F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA1,0x50,//DI
	DataType_octet_string,2,0xA1,0x51,//DI
	DataType_octet_string,2,0xA1,0x52,//DI
	DataType_octet_string,2,0xA1,0x53,//DI
	DataType_octet_string,2,0xA1,0x54,//DI
	DataType_octet_string,2,0xB1,0x50,//DI
	DataType_octet_string,2,0xB1,0x51,//DI
	DataType_octet_string,2,0xB1,0x52,//DI
	DataType_octet_string,2,0xB1,0x53,//DI
	DataType_octet_string,2,0xB1,0x54,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10610200[]=
{
//A相第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x61,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1A,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10620200[]=
{
//B相第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x62,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2E,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10630200[]=
{
//C相第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x63,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x42,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10700200[]=
{
//第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x70,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x07,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x07,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x6F,//DI
	DataType_octet_string,2,0xB1,0x6F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA1,0x60,//DI
	DataType_octet_string,2,0xA1,0x61,//DI
	DataType_octet_string,2,0xA1,0x62,//DI
	DataType_octet_string,2,0xA1,0x63,//DI
	DataType_octet_string,2,0xA1,0x64,//DI
	DataType_octet_string,2,0xB1,0x60,//DI
	DataType_octet_string,2,0xB1,0x61,//DI
	DataType_octet_string,2,0xB1,0x62,//DI
	DataType_octet_string,2,0xB1,0x63,//DI
	DataType_octet_string,2,0xB1,0x64,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10710200[]=
{
//A相第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x71,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1B,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10720200[]=
{
//B相第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x72,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2F,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10730200[]=
{
//C相第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x73,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x43,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10800200[]=
{
//第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x80,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x08,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x08,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x4F,//DI
	DataType_octet_string,2,0xB1,0x4F,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,10,
	DataType_octet_string,2,0xA1,0x40,//DI
	DataType_octet_string,2,0xA1,0x41,//DI
	DataType_octet_string,2,0xA1,0x42,//DI
	DataType_octet_string,2,0xA1,0x43,//DI
	DataType_octet_string,2,0xA1,0x44,//DI
	DataType_octet_string,2,0xB1,0x40,//DI
	DataType_octet_string,2,0xB1,0x41,//DI
	DataType_octet_string,2,0xB1,0x42,//DI
	DataType_octet_string,2,0xB1,0x43,//DI
	DataType_octet_string,2,0xB1,0x44,//DI
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10810200[]=
{
//A相第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x81,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1C,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10820200[]=
{
//B相第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x82,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x30,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10830200[]=
{
//C相第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x83,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x44,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10900200[]=
{
//正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x90,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x09,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x09,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10910200[]=
{
//A相正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x91,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1D,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10920200[]=
{
//B相正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x92,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x31,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10930200[]=
{
//C相正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x93,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x45,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10A00200[]=
{
//反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA0,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x0A,0xff,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x0A,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x04,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10A10200[]=
{
//A 相反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA1,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1E,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10A20200[]=
{
//B 相反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA2,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x32,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_10A30200[]=
{
//C 相反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA3,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x46,0x00,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11100200[]=
{
//冻结周期内正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11110200[]=
{
//冻结周期内A相正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11120200[]=
{
//冻结周期内B相正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11130200[]=
{
//冻结周期内C相正向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11200200[]=
{
//冻结周期内反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11210200[]=
{
//冻结周期内A相反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11220200[]=
{
//冻结周期内B相反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11230200[]=
{
//冻结周期内C相反向有功最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11300200[]=
{
//冻结周期内组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x30,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11310200[]=
{
//冻结周期内A相组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x31,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11320200[]=
{
//冻结周期内B相组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x32,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11330200[]=
{
//冻结周期内C相组合无功1最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x33,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11400200[]=
{
//冻结周期内组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x40,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11410200[]=
{
//冻结周期内A相组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x41,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11420200[]=
{
//冻结周期内B相组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x42,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11430200[]=
{
//冻结周期内C相组合无功2最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x43,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11500200[]=
{
//冻结周期内第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x50,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11510200[]=
{
//冻结周期内A相第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x51,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11520200[]=
{
//冻结周期内B相第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x52,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11530200[]=
{
//冻结周期内C相第一象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x53,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11600200[]=
{
//冻结周期内第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x60,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11610200[]=
{
//冻结周期内A相第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x61,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11620200[]=
{
//冻结周期内B相第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x62,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11630200[]=
{
//冻结周期内C相第二象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x63,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11700200[]=
{
//冻结周期内第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x70,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11710200[]=
{
//冻结周期内A相第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x71,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11720200[]=
{
//冻结周期内B相第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x72,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11730200[]=
{
//冻结周期内C相第三象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x73,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11800200[]=
{
//冻结周期内第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x80,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11810200[]=
{
//冻结周期内A相第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x81,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11820200[]=
{
//冻结周期内B相第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x82,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11830200[]=
{
//冻结周期内C相第四象限最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x83,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11900200[]=
{
//冻结周期内正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x90,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11910200[]=
{
//冻结周期内A相正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x91,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11920200[]=
{
//冻结周期内B相正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x92,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11930200[]=
{
//冻结周期内C相正向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x93,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11A00200[]=
{
//冻结周期内反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA0,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11A10200[]=
{
//冻结周期内A 相反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA1,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11A20200[]=
{
//冻结周期内B 相反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA2,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_11A30200[]=
{
//冻结周期内C相反向视在最大需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA3,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20000200[]=
{
//变量类
//电压
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x01,0xFF,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x01,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x01,0x02,0x00,//DI
	DataType_octet_string,4,0x02,0x01,0x03,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,3,
	DataType_octet_string,2,0xB6,0x11,//DI
	DataType_octet_string,2,0xB6,0x12,//DI
	DataType_octet_string,2,0xB6,0x13,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20010200[]=
{
//电流
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x01,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x02,0xFF,0x00,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x02,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x02,0x02,0x00,//DI
	DataType_octet_string,4,0x02,0x02,0x03,0x00,//DI
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,3,
	DataType_octet_string,2,0xB6,0x21,//DI
	DataType_octet_string,2,0xB6,0x22,//DI
	DataType_octet_string,2,0xB6,0x23,//DI
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20010400[]=
{
//零线电流
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x01,0x04,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x01,//DI
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20020200[]=
{
//电压相角
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x02,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20030200[]=
{
//电压电流相角
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x07,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x07,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x07,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x07,0x03,0x00,//DI		
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20040200[]=
{
//有功功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x04,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x03,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x03,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x03,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x03,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x03,0x03,0x00,//DI		
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,4,
	DataType_octet_string,2,0xB6,0x30,//DI	
	DataType_octet_string,2,0xB6,0x31,//DI	
	DataType_octet_string,2,0xB6,0x32,//DI
	DataType_octet_string,2,0xB6,0x33,//DI	
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20050200[]=
{
//无功功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x05,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x04,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x04,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x04,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x04,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x04,0x03,0x00,//DI		
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,4,
	DataType_octet_string,2,0xB6,0x40,//DI	
	DataType_octet_string,2,0xB6,0x41,//DI	
	DataType_octet_string,2,0xB6,0x42,//DI
	DataType_octet_string,2,0xB6,0x43,//DI	
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20060200[]=
{
//视在功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x06,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x05,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x05,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x05,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x05,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x05,0x03,0x00,//DI		
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20070200[]=
{
//一分钟平均有功功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x07,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x03,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20080200[]=
{
//一分钟平均无功功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x08,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20090200[]=
{
//一分钟平均视在功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x09,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200A0200[]=
{
//功率因数
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0A,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x06,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x06,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x06,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x06,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x06,0x03,0x00,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,4,
	DataType_octet_string,2,0xB6,0x50,//DI	
	DataType_octet_string,2,0xB6,0x51,//DI	
	DataType_octet_string,2,0xB6,0x52,//DI	
	DataType_octet_string,2,0xB6,0x53,//DI	
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200B0200[]=
{
//电压波形失真度
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0B,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x08,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x08,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x08,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x08,0x03,0x00,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200C0200[]=
{
//电流波形失真度
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0C,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x09,0xFF,0x00,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x09,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x09,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x09,0x03,0x00,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200D0200[]=
{
//电压谐波含有量（总及2…n次）
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0A,0x01,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,21,
	DataType_octet_string,4,0x02,0x0A,0x01,0x01,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x02,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x03,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x04,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x05,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x06,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x07,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x08,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x09,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x0A,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x0B,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x0C,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x0D,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x0E,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x0F,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x10,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x11,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x12,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x13,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x14,//DI	
	DataType_octet_string,4,0x02,0x0A,0x01,0x15,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200D0300[]=
{
//电压谐波含有量（总及2…n次）
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0A,0x02,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,21,
	DataType_octet_string,4,0x02,0x0A,0x02,0x01,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x02,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x03,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x04,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x05,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x06,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x07,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x08,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x09,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x0A,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x0B,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x0C,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x0D,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x0E,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x0F,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x10,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x11,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x12,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x13,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x14,//DI	
	DataType_octet_string,4,0x02,0x0A,0x02,0x15,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200D0400[]=
{
//电压谐波含有量（总及2…n次）
//	属性4∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x04,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0A,0x03,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,21,
	DataType_octet_string,4,0x02,0x0A,0x03,0x01,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x02,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x03,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x04,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x05,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x06,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x07,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x08,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x09,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x0A,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x0B,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x0C,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x0D,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x0E,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x0F,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x10,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x11,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x12,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x13,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x14,//DI	
	DataType_octet_string,4,0x02,0x0A,0x03,0x15,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200D0500[]=
{
//电压谐波含有量（总及2…n次）
//	属性5∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x05,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x04,0x00,0x02,0x06,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200E0200[]=
{
//电流谐波含有量（总及2…n次）
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0B,0x01,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,21,
	DataType_octet_string,4,0x02,0x0B,0x01,0x01,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x02,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x03,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x04,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x05,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x06,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x07,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x08,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x09,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x0A,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x0B,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x0C,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x0D,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x0E,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x0F,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x10,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x11,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x12,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x13,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x14,//DI	
	DataType_octet_string,4,0x02,0x0B,0x01,0x15,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200E0300[]=
{
//电流谐波含有量（总及2…n次）
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0B,0x02,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,21,
	DataType_octet_string,4,0x02,0x0B,0x02,0x01,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x02,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x03,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x04,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x05,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x06,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x07,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x08,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x09,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x0A,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x0B,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x0C,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x0D,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x0E,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x0F,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x10,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x11,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x12,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x13,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x14,//DI	
	DataType_octet_string,4,0x02,0x0B,0x02,0x15,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200E0400[]=
{
//电流谐波含有量（总及2…n次）
//	属性4∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x04,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0B,0x03,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,21,
	DataType_octet_string,4,0x02,0x0B,0x03,0x01,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x02,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x03,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x04,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x05,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x06,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x07,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x08,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x09,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x0A,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x0B,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x0C,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x0D,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x0E,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x0F,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x10,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x11,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x12,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x13,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x14,//DI	
	DataType_octet_string,4,0x02,0x0B,0x03,0x15,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200E0500[]=
{
//电流谐波含有量（总及2…n次）
//	属性5∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x05,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x04,0x00,0x02,0x06,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_200F0200[]=
{
//电网频率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0F,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x02,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20100200[]=
{
//表内温度
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x07,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20110200[]=
{
//时钟电池电压
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x08,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20120200[]=
{
//停电抄表电池电压
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x09,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20130200[]=
{
//时钟电池工作时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x0A,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0xB2,0x14,//DI	
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20140200[]=
{
//电能表运行状态字
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x14,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x04,0x00,0x05,0xFF,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,7,
	DataType_octet_string,4,0x04,0x00,0x05,0x01,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x02,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x03,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x04,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x05,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x06,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x07,//DI	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0xC0,0x20,//DI	
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20170200[]=
{
//当前有功需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x17,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x04,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20180200[]=
{
//当前无功需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x18,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x05,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20190200[]=
{
//当前视在需量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x19,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x06,//DI	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_201A0200[]=
{
//当前电价
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1A,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_201B0200[]=
{
//当前费率电价
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1B,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_201C0200[]=
{
//当前阶梯电价
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1C,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_201E0200[]=
{
//事件发生时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1E,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20200200[]=
{
//事件结束时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20210200[]=
{
//数据冻结时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20220200[]=
{
//事件记录序号
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20230200[]=
{
//冻结记录序号
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20240200[]=
{
//事件发生源
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x24,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20250200[]=
{
//事件当前值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x25,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20260200[]=
{
//电压不平衡率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x26,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20270200[]=
{
//电流不平衡率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x27,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20280200[]=
{
//负载率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x28,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20290200[]=
{
//安时值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x29,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_202A0200[]=
{
//目标服务器地址
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2A,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_202C0200[]=
{
//（当前）钱包文件
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2C,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_202D0200[]=
{
//（当前）透支金额
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2D,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_202E0200[]=
{
//累计购电金额
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2E,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20310200[]=
{
//月度用电量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x31,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20320200[]=
{
//阶梯结算用电量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x32,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20400200[]=
{
//控制命令执行状态字
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x40,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_20410200[]=
{
//控制命令错误状态字
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x41,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21000200[]=
{
//分钟区间统计
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21000300[]=
{
//分钟区间统计
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x00,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21010200[]=
{
//小时区间统计
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x01,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21010300[]=
{
//小时区间统计
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x01,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21020200[]=
{
//日区间统计
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x02,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21020300[]=
{
//日区间统计
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x02,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21030200[]=
{
//月区间统计
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21030300[]=
{
//月区间统计
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x03,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21040200[]=
{
//年区间统计
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x04,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21040300[]=
{
//年区间统计
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x04,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21100200[]=
{
//分钟平均
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x10,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21100300[]=
{
//分钟平均
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x10,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21110200[]=
{
//小时平均
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x11,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21110300[]=
{
//小时平均
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x11,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21120200[]=
{
//日平均
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x12,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21120300[]=
{
//日平均
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x12,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21130200[]=
{
//月平均
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x13,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21130300[]=
{
//月平均
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x13,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21140200[]=
{
//年平均
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x14,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21140300[]=
{
//年平均
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x14,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21200200[]=
{
//分钟极值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x20,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21200300[]=
{
//分钟极值
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x20,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21210200[]=
{
//小时极值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x21,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21210300[]=
{
//小时极值
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x21,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21220200[]=
{
//日极值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x22,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21220300[]=
{
//日极值
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x22,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21230200[]=
{
//月极值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x23,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21230300[]=
{
//月极值
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x23,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21240200[]=
{
//年极值
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x24,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21240300[]=
{
//年极值
//	属性3∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x24,0x03,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21300200[]=
{
//总电压合格率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x30,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21310200[]=
{
//当月A相电压合格率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x31,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21320200[]=
{
//当月B相电压合格率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x32,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21330200[]=
{
//当月C相电压合格率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x33,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21400200[]=
{
//日最大有功功率及发生时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x40,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_21410200[]=
{
//月最大有功功率及发生时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x41,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_22000200[]=
{
//通讯流量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x22,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_22030200[]=
{
//供电时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x22,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_22040200[]=
{
//复位次数
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x22,0x04,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_25000200[]=
{
//累计水（热）流量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};	

const u8 InitData_601E_25010200[]=
{
//累计气流量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x01,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_25020200[]=
{
//累计热量
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x02,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_25030200[]=
{
//热功率
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x03,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};

const u8 InitData_601E_25040200[]=
{
//累计工作时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x04,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}
};	

const u8 InitData_601E_25050200[]=
{
//水温
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x05,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,	
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

const u8 InitData_601E_25060200[]=
{
//（仪表）状态ST
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x06,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

const u8 InitData_601E_40000200[]=
{
//日期时间
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,0,//0=OAD,1=ROAD
	0x40,0x00,0x02,0x00,//OAD
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,2,
	DataType_octet_string,4,0x04,0x00,0x01,0x02,//DI电能表日历时钟Hhmmss	
	DataType_octet_string,4,0x04,0x00,0x01,0x01,//DI电能表日历时钟YYMMDDWW	
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xC0,0x11,//DI电能表日历时钟Hhmmss	
	DataType_octet_string,2,0xC0,0x10,//DI电能表日历时钟YYMMDDWW	
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	





//日冻结类.数据冻结时间
const u8 InitData_601E_50040200_20210200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x20,0x21,0x02,0x00,  //数据冻结时间
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x00,0x01,//冻结时间
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0xC0,0x10,//当前电表的年月日周
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//日冻结类.正向有功
const u8 InitData_601E_50040200_00100200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x10,0x02,0x00,  //正向有功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x01,0x01,//正向有功电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x90,0x1F,//正向有功电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//日冻结类.反向有功
const u8 InitData_601E_50040200_00200200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x20,0x02,0x00,  //反向有功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x02,0x01,//反向有功电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x90,0x2F,//反向有功电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.组合无功1
const u8 InitData_601E_50040200_00300200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x30,0x02,0x00,  //组合无功1电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x03,0x01,//组合无功1电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x1F,//组合无功1电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.组合无功2
const u8 InitData_601E_50040200_00400200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x40,0x02,0x00,  //组合无功2电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x04,0x01,//组合无功2电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x2F,//组合无功2电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.第一象限无功
const u8 InitData_601E_50040200_00500200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x50,0x02,0x00,  //第一象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x05,0x01,//第一象限无功电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x3F,//第一象限无功电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.第二象限无功
const u8 InitData_601E_50040200_00600200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x60,0x02,0x00,  //第二象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x06,0x01,//第二象限无功电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x5F,//第二象限无功电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.第三象限无功
const u8 InitData_601E_50040200_00700200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x70,0x02,0x00,  //第三象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x07,0x01,//第三象限无功电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x6F,//第三象限无功电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.第四象限无功
const u8 InitData_601E_50040200_00800200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x80,0x02,0x00,  //第四象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x08,0x01,//第四象限无功电能
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x91,0x4F,//第四象限无功电能
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.正向有功最大需量及发生时间	
const u8 InitData_601E_50040200_10100200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x10,0x02,0x00,  //正向有功最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x09,0x01,//正向有功最大需量及发生时间
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x1F,//正向有功最大需量
	DataType_octet_string,2,0xB0,0x1F,//正向有功最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.反向有功最大需量及发生时间	
const u8 InitData_601E_50040200_10200200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x20,0x02,0x00,  //反向有功最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x0A,0x01,//反向有功最大需量及发生时间
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x2F,//反向有功最大需量
	DataType_octet_string,2,0xB0,0x2F,//反向有功最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.组合无功1最大需量及发生时间	
const u8 InitData_601E_50040200_10300200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x30,0x02,0x00,  //组合无功1最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x03,0xFF,0x00,//当月正向无功最大需量及发生时间（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x1F,//组合无功1最大需量
	DataType_octet_string,2,0xB1,0x1F,//组合无功1最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.组合无功2最大需量及发生时间	
const u8 InitData_601E_50040200_10400200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x40,0x02,0x00,  //组合无功2最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x04,0xFF,0x00,//当月反向无功最大需量及发生时间（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x2F,//组合无功2最大需量
	DataType_octet_string,2,0xB1,0x2F,//组合无功2最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.有功功率	
const u8 InitData_601E_50040200_20040200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x20,0x04,0x02,0x00,  //有功功率
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x10,0x01,//上1次日冻结变量数据（字节数3*8）(总/A/B/C有功功率,总/A/B/C无功功率)
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.无功功率	
const u8 InitData_601E_50040200_20050200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x20,0x05,0x02,0x00,  //无功功率
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x10,0x01,//上1次日冻结变量数据（字节数3*8）(总/A/B/C有功功率,总/A/B/C无功功率)
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.剩余金额、购电 次数	
const u8 InitData_601E_50040200_202C0200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x20,0x2C,0x02,0x00,  //剩余金额、购电 次数
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x08,0x02,0x01,//上1次剩余金额日冻结（2购电次数，4剩余金额，4透支金额）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.透支金额
const u8 InitData_601E_50040200_202D0200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x20,0x2D,0x02,0x00,  //透支金额
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x05,0x08,0x02,0x01,//上1次剩余金额日冻结（2购电次数，4剩余金额，4透支金额）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.A 相电压合格率
const u8 InitData_601E_50040200_21310201[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x31,0x02,0x01,  //A 相电压合格率
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.B 相电压合格率
const u8 InitData_601E_50040200_21320201[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x32,0x02,0x01,  //B 相电压合格率
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//日冻结类.C 相电压合格率
const u8 InitData_601E_50040200_21330201[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x33,0x02,0x01,  //C 相电压合格率
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.数据冻结时间
const u8 InitData_601E_50060200_20210200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x20,0x21,0x02,0x00,  //数据冻结时间
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0xFE,0x00,0x01,//上1次月结算日结算时间 
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//月冻结类.正向有功
const u8 InitData_601E_50060200_00100200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x10,0x02,0x00,  //正向有功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x01,0xFF,0x01,//（上一结算日）正向有功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x94,0x1F,//（上一结算日）正向有功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//月冻结类.反向有功
const u8 InitData_601E_50060200_00200200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x20,0x02,0x00,  //反向有功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x02,0xFF,0x01,//（上一结算日）反向有功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x94,0x2F,//（上一结算日）反向有功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.组合无功1
const u8 InitData_601E_50060200_00300200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x30,0x02,0x00,  //组合无功1电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x03,0xFF,0x01,//（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x95,0x1F,//（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.组合无功2
const u8 InitData_601E_50060200_00400200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x40,0x02,0x00,  //组合无功2电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x04,0xFF,0x01,//（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x95,0x2F,//（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.第一象限无功
const u8 InitData_601E_50060200_00500200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x50,0x02,0x00,  //第一象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x05,0xFF,0x01,//（上一结算日）一象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x95,0x3F,//（上一结算日）一象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.第二象限无功
const u8 InitData_601E_50060200_00600200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x60,0x02,0x00,  //第二象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x06,0xFF,0x01,//（上一结算日）二象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x95,0x5F,//（上一结算日）二象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.第三象限无功
const u8 InitData_601E_50060200_00700200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x70,0x02,0x00,  //第三象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x07,0xFF,0x01,//（上一结算日）三象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x95,0x6F,//（上一结算日）三象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.第四象限无功
const u8 InitData_601E_50060200_00800200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x80,0x02,0x00,  //第四象限无功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x08,0xFF,0x01,//（上一结算日）四象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,1,
	DataType_octet_string,2,0x95,0x4F,//（上一结算日）四象限无功电能示值（总、费率1～M）
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.正向有功最大需量及发生时间	
const u8 InitData_601E_50060200_10100200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x10,0x02,0x00,  //正向有功最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x01,0xFF,0x01,//（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA4,0x1F,//正向有功最大需量
	DataType_octet_string,2,0xB4,0x1F,//正向有功最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.反向有功最大需量及发生时间	
const u8 InitData_601E_50060200_10200200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x20,0x02,0x00,  //反向有功最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x02,0xFF,0x01,//（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA4,0x2F,//反向有功最大需量
	DataType_octet_string,2,0xB4,0x2F,//反向有功最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.组合无功1最大需量及发生时间
const u8 InitData_601E_50060200_10300200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x30,0x02,0x00,  //组合无功1最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x03,0xFF,0x01,//（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA5,0x1F,//组合无功1最大需量
	DataType_octet_string,2,0xB5,0x1F,//组合无功1最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.组合无功2最大需量及发生时间
const u8 InitData_601E_50060200_10400200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x10,0x40,0x02,0x00,  //组合无功1最大需量及发生时间	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x01,0x04,0xFF,0x01,//（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,2,
	DataType_octet_string,2,0xA5,0x2F,//组合无功1最大需量
	DataType_octet_string,2,0xB5,0x2F,//组合无功1最大需量发生时间
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.总电压合格率
const u8 InitData_601E_50060200_21300202[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x30,0x02,0x02,  //总电压合格率	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.A相电压合格率
const u8 InitData_601E_50060200_21310202[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x31,0x02,0x02,  //A相电压合格率	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.B相电压合格率
const u8 InitData_601E_50060200_21320202[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x32,0x02,0x02,  //B相电压合格率	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.C相电压合格率
const u8 InitData_601E_50060200_21330202[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x21,0x33,0x02,0x02,  //C相电压合格率	
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//月冻结类.组合有功电能
const u8 InitData_601E_50060200_00000200[]=
{
//	属性2∷=structure
	DataType_structure,2,
//	{
//		数据列选择描述符  CSD，
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//关联个数
	0x00,0x00,0x02,0x00,  //组合有功电能
//		规则描述  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(4))，
	DataType_array,1,
	DataType_octet_string,4,0x00,0x00,0xFF,0x01,//上1 结算日组合有功电能示值
//				备用DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}，
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				主用DI  array octet-string(SIZE(2))，
	DataType_array,0,
//				备用DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}，
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};







//const DL698_LIB_List_TypeDef SETINIT_601E_List[NUMmax_601E*2]=
const DL698_LIB_List_TypeDef SETINIT_601E_List[]=
{
//
	
//
	(u32*)InitData_601E_00000200,(u32*)sizeof(InitData_601E_00000200),//组合有功电能
	(u32*)InitData_601E_00100200,(u32*)sizeof(InitData_601E_00100200),//正向有功电能
	(u32*)InitData_601E_00110200,(u32*)sizeof(InitData_601E_00110200),//A相正向有功电能
	(u32*)InitData_601E_00120200,(u32*)sizeof(InitData_601E_00120200),//B相正向有功电能
	(u32*)InitData_601E_00130200,(u32*)sizeof(InitData_601E_00130200),//C相正向有功电能
	(u32*)InitData_601E_00200200,(u32*)sizeof(InitData_601E_00200200),//反向有功电能
	(u32*)InitData_601E_00210200,(u32*)sizeof(InitData_601E_00210200),//A相反向有功电能
	(u32*)InitData_601E_00220200,(u32*)sizeof(InitData_601E_00220200),//B相反向有功电能
	(u32*)InitData_601E_00230200,(u32*)sizeof(InitData_601E_00230200),//C相反向有功电能
	(u32*)InitData_601E_00300200,(u32*)sizeof(InitData_601E_00300200),//组合无功1电能
	(u32*)InitData_601E_00310200,(u32*)sizeof(InitData_601E_00310200),//A相组合无功1电能
	(u32*)InitData_601E_00320200,(u32*)sizeof(InitData_601E_00320200),//B相组合无功1电能
	(u32*)InitData_601E_00330200,(u32*)sizeof(InitData_601E_00330200),//C相组合无功1电能
	(u32*)InitData_601E_00400200,(u32*)sizeof(InitData_601E_00400200),//组合无功2电能
	(u32*)InitData_601E_00410200,(u32*)sizeof(InitData_601E_00410200),//A相组合无功2电能
	(u32*)InitData_601E_00420200,(u32*)sizeof(InitData_601E_00420200),//B相组合无功2电能
	(u32*)InitData_601E_00430200,(u32*)sizeof(InitData_601E_00430200),//C相组合无功2电能
	(u32*)InitData_601E_00500200,(u32*)sizeof(InitData_601E_00500200),//第一象限无功电能
	(u32*)InitData_601E_00510200,(u32*)sizeof(InitData_601E_00510200),//A相第一象限无功电能
	(u32*)InitData_601E_00520200,(u32*)sizeof(InitData_601E_00520200),//B相第一象限无功电能
	(u32*)InitData_601E_00530200,(u32*)sizeof(InitData_601E_00530200),//C相第一象限无功电能
	(u32*)InitData_601E_00600200,(u32*)sizeof(InitData_601E_00600200),//第二象限无功电能
	(u32*)InitData_601E_00610200,(u32*)sizeof(InitData_601E_00610200),//A相第二象限无功电能
	(u32*)InitData_601E_00620200,(u32*)sizeof(InitData_601E_00620200),//B相第二象限无功电能
	(u32*)InitData_601E_00630200,(u32*)sizeof(InitData_601E_00630200),//C相第二象限无功电能
	(u32*)InitData_601E_00700200,(u32*)sizeof(InitData_601E_00700200),//第三象限无功电能
	(u32*)InitData_601E_00710200,(u32*)sizeof(InitData_601E_00710200),//A相第三象限无功电能
	(u32*)InitData_601E_00720200,(u32*)sizeof(InitData_601E_00720200),//B相第三象限无功电能
	(u32*)InitData_601E_00730200,(u32*)sizeof(InitData_601E_00730200),//C相第三象限无功电能
	(u32*)InitData_601E_00800200,(u32*)sizeof(InitData_601E_00800200),//第四象限无功电能
	(u32*)InitData_601E_00810200,(u32*)sizeof(InitData_601E_00810200),//A相第四象限无功电能
	(u32*)InitData_601E_00820200,(u32*)sizeof(InitData_601E_00820200),//B相第四象限无功电能
	(u32*)InitData_601E_00830200,(u32*)sizeof(InitData_601E_00830200),//C相第四象限无功电能
	(u32*)InitData_601E_00900200,(u32*)sizeof(InitData_601E_00900200),//正向视在电能
	(u32*)InitData_601E_00910200,(u32*)sizeof(InitData_601E_00910200),//A相正向视在电能
	(u32*)InitData_601E_00920200,(u32*)sizeof(InitData_601E_00920200),//B相正向视在电能
	(u32*)InitData_601E_00930200,(u32*)sizeof(InitData_601E_00930200),//C相正向视在电能
	(u32*)InitData_601E_00A00200,(u32*)sizeof(InitData_601E_00A00200),//反向视在电能
	(u32*)InitData_601E_00A10200,(u32*)sizeof(InitData_601E_00A10200),//A相反向视在电能
	(u32*)InitData_601E_00A20200,(u32*)sizeof(InitData_601E_00A20200),//B相反向视在电能
	(u32*)InitData_601E_00A30200,(u32*)sizeof(InitData_601E_00A30200),//C相反向视在电能
	(u32*)InitData_601E_01100200,(u32*)sizeof(InitData_601E_01100200),//正向有功基波 总电能
	(u32*)InitData_601E_01110200,(u32*)sizeof(InitData_601E_01110200),//A相正向有功基波电能
	(u32*)InitData_601E_01120200,(u32*)sizeof(InitData_601E_01120200),//B相正向有功基波电能
	(u32*)InitData_601E_01130200,(u32*)sizeof(InitData_601E_01130200),//C相正向有功基波电能
	(u32*)InitData_601E_01200200,(u32*)sizeof(InitData_601E_01200200),//反向有功基波 总电能
	(u32*)InitData_601E_01210200,(u32*)sizeof(InitData_601E_01210200),//A相反向有功基波电能
	(u32*)InitData_601E_01220200,(u32*)sizeof(InitData_601E_01220200),//B相反向有功基波电能
	(u32*)InitData_601E_01230200,(u32*)sizeof(InitData_601E_01230200),//C相反向有功基波电能
	(u32*)InitData_601E_02100200,(u32*)sizeof(InitData_601E_02100200),//正向有功谐波 总电能
	(u32*)InitData_601E_02110200,(u32*)sizeof(InitData_601E_02110200),//A相正向有功谐波 电能
	(u32*)InitData_601E_02120200,(u32*)sizeof(InitData_601E_02120200),//B相正向有功谐波 电能
	(u32*)InitData_601E_02130200,(u32*)sizeof(InitData_601E_02130200),//C相正向有功谐波 电能
	(u32*)InitData_601E_02200200,(u32*)sizeof(InitData_601E_02200200),//反向有功谐波 总电能
	(u32*)InitData_601E_02210200,(u32*)sizeof(InitData_601E_02210200),//A相反向有功谐波 电能
	(u32*)InitData_601E_02220200,(u32*)sizeof(InitData_601E_02220200),//B相反向有功谐波 电能
	(u32*)InitData_601E_02230200,(u32*)sizeof(InitData_601E_02230200),//C相反向有功谐波 电能
	(u32*)InitData_601E_03000200,(u32*)sizeof(InitData_601E_03000200),//铜损有功总电能补偿量
	(u32*)InitData_601E_03010200,(u32*)sizeof(InitData_601E_03010200),//A相铜损有功总电能补偿量
	(u32*)InitData_601E_03020200,(u32*)sizeof(InitData_601E_03020200),//B相铜损有功总电能补偿量
	(u32*)InitData_601E_03030200,(u32*)sizeof(InitData_601E_03030200),//C相铜损有功总电能补偿量
	(u32*)InitData_601E_04000200,(u32*)sizeof(InitData_601E_04000200),//铁损有功总电能补偿量
	(u32*)InitData_601E_04010200,(u32*)sizeof(InitData_601E_04010200),//A相铁损有功总电能补偿量
	(u32*)InitData_601E_04020200,(u32*)sizeof(InitData_601E_04020200),//B相铁损有功总电能补偿量
	(u32*)InitData_601E_04030200,(u32*)sizeof(InitData_601E_04030200),//C相铁损有功总电能补偿量
	(u32*)InitData_601E_05000200,(u32*)sizeof(InitData_601E_05000200),//关联总电能
	(u32*)InitData_601E_05010200,(u32*)sizeof(InitData_601E_05010200),//A相关联总电能
	(u32*)InitData_601E_05020200,(u32*)sizeof(InitData_601E_05020200),//B相关联总电能
	(u32*)InitData_601E_05030200,(u32*)sizeof(InitData_601E_05030200),//C相关联总电能	
	(u32*)InitData_601E_10100200,(u32*)sizeof(InitData_601E_10100200),//正向有功最大需量
	(u32*)InitData_601E_10110200,(u32*)sizeof(InitData_601E_10110200),//A相正向有功最大需量
	(u32*)InitData_601E_10120200,(u32*)sizeof(InitData_601E_10120200),//B相正向有功最大需量
	(u32*)InitData_601E_10130200,(u32*)sizeof(InitData_601E_10130200),//C相正向有功最大需量
	(u32*)InitData_601E_10200200,(u32*)sizeof(InitData_601E_10200200),//反向有功最大需量
	(u32*)InitData_601E_10210200,(u32*)sizeof(InitData_601E_10210200),//A相反向有功最大需量
	(u32*)InitData_601E_10220200,(u32*)sizeof(InitData_601E_10220200),//B相反向有功最大需量
	(u32*)InitData_601E_10230200,(u32*)sizeof(InitData_601E_10230200),//C相反向有功最大需量
	(u32*)InitData_601E_10300200,(u32*)sizeof(InitData_601E_10300200),//组合无功1最大需量
	(u32*)InitData_601E_10310200,(u32*)sizeof(InitData_601E_10310200),//A相组合无功1最大需量
	(u32*)InitData_601E_10320200,(u32*)sizeof(InitData_601E_10320200),//B相组合无功1最大需量
	(u32*)InitData_601E_10330200,(u32*)sizeof(InitData_601E_10330200),//C相组合无功1最大需量
	(u32*)InitData_601E_10400200,(u32*)sizeof(InitData_601E_10400200),//组合无功2最大需量
	(u32*)InitData_601E_10410200,(u32*)sizeof(InitData_601E_10410200),//A相组合无功2最大需量
	(u32*)InitData_601E_10420200,(u32*)sizeof(InitData_601E_10420200),//B相组合无功2最大需量
	(u32*)InitData_601E_10430200,(u32*)sizeof(InitData_601E_10430200),//C相组合无功2最大需量
	(u32*)InitData_601E_10500200,(u32*)sizeof(InitData_601E_10500200),//第一象限最大需量
	(u32*)InitData_601E_10510200,(u32*)sizeof(InitData_601E_10510200),//A相第一象限最大需量
	(u32*)InitData_601E_10520200,(u32*)sizeof(InitData_601E_10520200),//B相第一象限最大需量
	(u32*)InitData_601E_10530200,(u32*)sizeof(InitData_601E_10530200),//C相第一象限最大需量
	(u32*)InitData_601E_10600200,(u32*)sizeof(InitData_601E_10600200),//第二象限最大需量
	(u32*)InitData_601E_10610200,(u32*)sizeof(InitData_601E_10610200),//A相第二象限最大需量
	(u32*)InitData_601E_10620200,(u32*)sizeof(InitData_601E_10620200),//B相第二象限最大需量
	(u32*)InitData_601E_10630200,(u32*)sizeof(InitData_601E_10630200),//C相第二象限最大需量
	(u32*)InitData_601E_10700200,(u32*)sizeof(InitData_601E_10700200),//第三象限最大需量
	(u32*)InitData_601E_10710200,(u32*)sizeof(InitData_601E_10710200),//A相第三象限最大需量
	(u32*)InitData_601E_10720200,(u32*)sizeof(InitData_601E_10720200),//B相第三象限最大需量	
	(u32*)InitData_601E_10730200,(u32*)sizeof(InitData_601E_10730200),//C相第三象限最大需量	
	(u32*)InitData_601E_10800200,(u32*)sizeof(InitData_601E_10800200),//第四象限最大需量
	(u32*)InitData_601E_10810200,(u32*)sizeof(InitData_601E_10810200),//A相第四象限最大需量
  (u32*)InitData_601E_10820200,(u32*)sizeof(InitData_601E_10820200),//B相第四象限最大需量
	(u32*)InitData_601E_10830200,(u32*)sizeof(InitData_601E_10830200),//C相第四象限最大需量
	(u32*)InitData_601E_10900200,(u32*)sizeof(InitData_601E_10900200),//正向视在最大需量
	(u32*)InitData_601E_10910200,(u32*)sizeof(InitData_601E_10910200),//A相正向视在最大需量
	(u32*)InitData_601E_10920200,(u32*)sizeof(InitData_601E_10920200),//B相正向视在最大需量
	(u32*)InitData_601E_10930200,(u32*)sizeof(InitData_601E_10930200),//C相正向视在最大需量
	(u32*)InitData_601E_10A00200,(u32*)sizeof(InitData_601E_10A00200),//反向视在最大需量
	(u32*)InitData_601E_10A10200,(u32*)sizeof(InitData_601E_10A10200),//A 相反向视在最大需量
	(u32*)InitData_601E_10A20200,(u32*)sizeof(InitData_601E_10A20200),//B 相反向视在最大需量
	(u32*)InitData_601E_10A30200,(u32*)sizeof(InitData_601E_10A30200),//C 相反向视在最大需量
//	(u32*)InitData_601E_11100200,(u32*)sizeof(InitData_601E_11100200),//冻结周期内正向有功最大需量
//	(u32*)InitData_601E_11110200,(u32*)sizeof(InitData_601E_11110200),//冻结周期内A相正向有功最大需量
//	(u32*)InitData_601E_11120200,(u32*)sizeof(InitData_601E_11120200),//冻结周期内B相正向有功最大需量
//	(u32*)InitData_601E_11130200,(u32*)sizeof(InitData_601E_11130200),//冻结周期内C相正向有功最大需量
//	(u32*)InitData_601E_11200200,(u32*)sizeof(InitData_601E_11200200),//冻结周期内反向有功最大需量
//	(u32*)InitData_601E_11210200,(u32*)sizeof(InitData_601E_11210200),//冻结周期内A相反向有功最大需量
//	(u32*)InitData_601E_11220200,(u32*)sizeof(InitData_601E_11220200),//冻结周期内B相反向有功最大需量
//	(u32*)InitData_601E_11230200,(u32*)sizeof(InitData_601E_11230200),//冻结周期内C相反向有功最大需量
//	(u32*)InitData_601E_11300200,(u32*)sizeof(InitData_601E_11300200),//冻结周期内组合无功1最大需量
//	(u32*)InitData_601E_11310200,(u32*)sizeof(InitData_601E_11310200),//冻结周期内A相组合无功1最大需量
//	(u32*)InitData_601E_11320200,(u32*)sizeof(InitData_601E_11320200),//冻结周期内B相组合无功1最大需量
//	(u32*)InitData_601E_11330200,(u32*)sizeof(InitData_601E_11330200),//冻结周期内C相组合无功1最大需量
//	(u32*)InitData_601E_11400200,(u32*)sizeof(InitData_601E_11400200),//冻结周期内组合无功2最大需量
//	(u32*)InitData_601E_11410200,(u32*)sizeof(InitData_601E_11410200),//冻结周期内A相组合无功2最大需量
//	(u32*)InitData_601E_11420200,(u32*)sizeof(InitData_601E_11420200),//冻结周期内B相组合无功2最大需量
//	(u32*)InitData_601E_11430200,(u32*)sizeof(InitData_601E_11430200),//冻结周期内C相组合无功2最大需量
//	(u32*)InitData_601E_11500200,(u32*)sizeof(InitData_601E_11500200),//冻结周期内第一象限最大需量
//	(u32*)InitData_601E_11510200,(u32*)sizeof(InitData_601E_11510200),//冻结周期内A相第一象限最大需量
//	(u32*)InitData_601E_11520200,(u32*)sizeof(InitData_601E_11520200),//冻结周期内B相第一象限最大需量
//	(u32*)InitData_601E_11530200,(u32*)sizeof(InitData_601E_11530200),//冻结周期内C相第一象限最大需量	
//	(u32*)InitData_601E_11600200,(u32*)sizeof(InitData_601E_11600200),//冻结周期内第二象限最大需量
//	(u32*)InitData_601E_11610200,(u32*)sizeof(InitData_601E_11610200),//冻结周期内A相第二象限最大需量
//	(u32*)InitData_601E_11620200,(u32*)sizeof(InitData_601E_11620200),//冻结周期内B相第二象限最大需量	
//	(u32*)InitData_601E_11630200,(u32*)sizeof(InitData_601E_11630200),//冻结周期内C相第二象限最大需量	
//	(u32*)InitData_601E_11700200,(u32*)sizeof(InitData_601E_11700200),//冻结周期内第三象限最大需量
//	(u32*)InitData_601E_11710200,(u32*)sizeof(InitData_601E_11710200),//冻结周期内A相第三象限最大需量
//	(u32*)InitData_601E_11720200,(u32*)sizeof(InitData_601E_11720200),//冻结周期内B相第三象限最大需量
//	(u32*)InitData_601E_11730200,(u32*)sizeof(InitData_601E_11730200),//冻结周期内C相第三象限最大需量
//	(u32*)InitData_601E_11800200,(u32*)sizeof(InitData_601E_11800200),//冻结周期内第四象限最大需量
//	(u32*)InitData_601E_11810200,(u32*)sizeof(InitData_601E_11810200),//冻结周期内A相第四象限最大需量
//	(u32*)InitData_601E_11820200,(u32*)sizeof(InitData_601E_11820200),//冻结周期内B相第四象限最大需量
//	(u32*)InitData_601E_11830200,(u32*)sizeof(InitData_601E_11830200),//冻结周期内C相第四象限最大需量
//	(u32*)InitData_601E_11900200,(u32*)sizeof(InitData_601E_11900200),//冻结周期内正向视在最大需量
//	(u32*)InitData_601E_11910200,(u32*)sizeof(InitData_601E_11910200),//冻结周期内A相正向视在最大需量
//	(u32*)InitData_601E_11920200,(u32*)sizeof(InitData_601E_11920200),//冻结周期内B相正向视在最大需量	
//	(u32*)InitData_601E_11930200,(u32*)sizeof(InitData_601E_11930200),//冻结周期内C相正向视在最大需量	
//	(u32*)InitData_601E_11A00200,(u32*)sizeof(InitData_601E_11A00200),//冻结周期内反向视在最大需量
//	(u32*)InitData_601E_11A10200,(u32*)sizeof(InitData_601E_11A10200),//冻结周期内A 相反向视在最大需量
//	(u32*)InitData_601E_11A20200,(u32*)sizeof(InitData_601E_11A20200),//冻结周期内B 相反向视在最大需量
//	(u32*)InitData_601E_11A30200,(u32*)sizeof(InitData_601E_11A30200),//冻结周期内C 相反向视在最大需量	
	(u32*)InitData_601E_20000200,(u32*)sizeof(InitData_601E_20000200),//电压
	(u32*)InitData_601E_20010200,(u32*)sizeof(InitData_601E_20010200),//电流
	(u32*)InitData_601E_20010400,(u32*)sizeof(InitData_601E_20010400),//零线电流
	(u32*)InitData_601E_20020200,(u32*)sizeof(InitData_601E_20020200),//电压相角
	(u32*)InitData_601E_20030200,(u32*)sizeof(InitData_601E_20030200),//电压电流相角
	(u32*)InitData_601E_20040200,(u32*)sizeof(InitData_601E_20040200),//有功功率
	(u32*)InitData_601E_20050200,(u32*)sizeof(InitData_601E_20050200),//无功功率
	(u32*)InitData_601E_20060200,(u32*)sizeof(InitData_601E_20060200),//视在功率
	(u32*)InitData_601E_20070200,(u32*)sizeof(InitData_601E_20070200),//一分钟平均有功功率
	(u32*)InitData_601E_20080200,(u32*)sizeof(InitData_601E_20080200),//一分钟平均无功功率
	(u32*)InitData_601E_20090200,(u32*)sizeof(InitData_601E_20090200),//一分钟平均视在功率
	(u32*)InitData_601E_200A0200,(u32*)sizeof(InitData_601E_200A0200),//功率因数
//	(u32*)InitData_601E_200B0200,(u32*)sizeof(InitData_601E_200B0200),//电压波形失真度
//	(u32*)InitData_601E_200C0200,(u32*)sizeof(InitData_601E_200C0200),//电流波形失真度
//	(u32*)InitData_601E_200D0200,(u32*)sizeof(InitData_601E_200D0200),//电压谐波含有量（总及2…n次）属性2
//	(u32*)InitData_601E_200D0300,(u32*)sizeof(InitData_601E_200D0300),//电压谐波含有量（总及2…n次）属性3
//	(u32*)InitData_601E_200D0400,(u32*)sizeof(InitData_601E_200D0400),//电压谐波含有量（总及2…n次）属性4
//  (u32*)InitData_601E_200D0500,(u32*)sizeof(InitData_601E_200D0500),//电压谐波含有量（总及2…n次）属性5
//	(u32*)InitData_601E_200E0200,(u32*)sizeof(InitData_601E_200E0200),//电流谐波含有量（总及2…n次）属性2
//	(u32*)InitData_601E_200E0300,(u32*)sizeof(InitData_601E_200E0300),//电流谐波含有量（总及2…n次）属性3
//	(u32*)InitData_601E_200E0400,(u32*)sizeof(InitData_601E_200E0400),//电流谐波含有量（总及2…n次）属性4
//	(u32*)InitData_601E_200E0500,(u32*)sizeof(InitData_601E_200E0500),//电流谐波含有量（总及2…n次）属性5	
	(u32*)InitData_601E_200F0200,(u32*)sizeof(InitData_601E_200F0200),//电网频率
	(u32*)InitData_601E_20100200,(u32*)sizeof(InitData_601E_20100200),//表内温度
	(u32*)InitData_601E_20110200,(u32*)sizeof(InitData_601E_20110200),//时钟电池电压
	(u32*)InitData_601E_20120200,(u32*)sizeof(InitData_601E_20120200),//停电抄表电池电压
	(u32*)InitData_601E_20130200,(u32*)sizeof(InitData_601E_20130200),//时钟电池工作时间
	(u32*)InitData_601E_20140200,(u32*)sizeof(InitData_601E_20140200),//电能表运行状态字
	(u32*)InitData_601E_20170200,(u32*)sizeof(InitData_601E_20170200),//当前有功需量
	(u32*)InitData_601E_20180200,(u32*)sizeof(InitData_601E_20180200),//当前无功需量
	(u32*)InitData_601E_20190200,(u32*)sizeof(InitData_601E_20190200),//当前视在需量
	(u32*)InitData_601E_201A0200,(u32*)sizeof(InitData_601E_201A0200),//当前电价
	(u32*)InitData_601E_201B0200,(u32*)sizeof(InitData_601E_201B0200),//当前费率电价
	(u32*)InitData_601E_201C0200,(u32*)sizeof(InitData_601E_201C0200),//当前阶梯电价
		
//	(u32*)InitData_601E_201E0200,(u32*)sizeof(InitData_601E_201E0200),//事件发生时间
//	(u32*)InitData_601E_20200200,(u32*)sizeof(InitData_601E_20200200),//事件结束时间
//	(u32*)InitData_601E_20210200,(u32*)sizeof(InitData_601E_20210200),//数据冻结时间
//	(u32*)InitData_601E_20220200,(u32*)sizeof(InitData_601E_20220200),//事件记录序号
//	(u32*)InitData_601E_20230200,(u32*)sizeof(InitData_601E_20230200),//冻结记录序号
//	(u32*)InitData_601E_20240200,(u32*)sizeof(InitData_601E_20240200),//事件发生源
//	(u32*)InitData_601E_20250200,(u32*)sizeof(InitData_601E_20250200),//事件当前值
		
	(u32*)InitData_601E_20260200,(u32*)sizeof(InitData_601E_20260200),//电压不平衡率
	(u32*)InitData_601E_20270200,(u32*)sizeof(InitData_601E_20270200),//电流不平衡率
	(u32*)InitData_601E_20280200,(u32*)sizeof(InitData_601E_20280200),//负载率
	(u32*)InitData_601E_20290200,(u32*)sizeof(InitData_601E_20290200),//安时值
	(u32*)InitData_601E_202A0200,(u32*)sizeof(InitData_601E_202A0200),//目标服务器地址
	(u32*)InitData_601E_202C0200,(u32*)sizeof(InitData_601E_202C0200),//（当前）钱包文件
	(u32*)InitData_601E_202D0200,(u32*)sizeof(InitData_601E_202D0200),//（当前）透支金额
	(u32*)InitData_601E_202E0200,(u32*)sizeof(InitData_601E_202E0200),//累计购电金额
	(u32*)InitData_601E_20310200,(u32*)sizeof(InitData_601E_20310200),//月度用电量
	(u32*)InitData_601E_20320200,(u32*)sizeof(InitData_601E_20320200),//阶梯结算用电量
	(u32*)InitData_601E_20400200,(u32*)sizeof(InitData_601E_20400200),//控制命令执行状态字
	(u32*)InitData_601E_20410200,(u32*)sizeof(InitData_601E_20410200),//控制命令错误状态字
	
//	(u32*)InitData_601E_21000200,(u32*)sizeof(InitData_601E_21000200),//分钟区间统计 属性2
//	(u32*)InitData_601E_21000300,(u32*)sizeof(InitData_601E_21000300),//分钟区间统计 属性3
//	(u32*)InitData_601E_21010200,(u32*)sizeof(InitData_601E_21010200),//小时区间统计 属性2
//	(u32*)InitData_601E_21010300,(u32*)sizeof(InitData_601E_21010300),//小时区间统计 属性3
//	(u32*)InitData_601E_21020200,(u32*)sizeof(InitData_601E_21020200),//日区间统计  属性2
//	(u32*)InitData_601E_21020300,(u32*)sizeof(InitData_601E_21020300),//日区间统计  属性3
//	(u32*)InitData_601E_21030200,(u32*)sizeof(InitData_601E_21030200),//月区间统计  属性2
//	(u32*)InitData_601E_21030300,(u32*)sizeof(InitData_601E_21030300),//月区间统计  属性3	
//  (u32*)InitData_601E_21040200,(u32*)sizeof(InitData_601E_21040200),//年区间统计  属性2	
//	(u32*)InitData_601E_21040300,(u32*)sizeof(InitData_601E_21040300),//年区间统计  属性3	
//	(u32*)InitData_601E_21100200,(u32*)sizeof(InitData_601E_21100200),//分钟平均  属性2
//	(u32*)InitData_601E_21100300,(u32*)sizeof(InitData_601E_21100300),//分钟平均  属性3
//	(u32*)InitData_601E_21110200,(u32*)sizeof(InitData_601E_21110200),//小时平均  属性2
//	(u32*)InitData_601E_21110300,(u32*)sizeof(InitData_601E_21110300),//小时平均  属性3
//	(u32*)InitData_601E_21120200,(u32*)sizeof(InitData_601E_21120200),//日平均 属性2
//	(u32*)InitData_601E_21120300,(u32*)sizeof(InitData_601E_21120300),//日平均 属性3
//	(u32*)InitData_601E_21130200,(u32*)sizeof(InitData_601E_21130200),//月平均 属性2
//	(u32*)InitData_601E_21130300,(u32*)sizeof(InitData_601E_21130300),//月平均 属性3	
//	(u32*)InitData_601E_21140200,(u32*)sizeof(InitData_601E_21140200),//年平均 属性2
//	(u32*)InitData_601E_21140300,(u32*)sizeof(InitData_601E_21140300),//年平均 属性3
//	(u32*)InitData_601E_21200200,(u32*)sizeof(InitData_601E_21200200),//分钟极值  属性2
//	(u32*)InitData_601E_21200300,(u32*)sizeof(InitData_601E_21200300),//分钟极值  属性3
//	(u32*)InitData_601E_21210200,(u32*)sizeof(InitData_601E_21210200),//小时极值  属性2
//	(u32*)InitData_601E_21210300,(u32*)sizeof(InitData_601E_21210300),//小时极值  属性3
//	(u32*)InitData_601E_21220200,(u32*)sizeof(InitData_601E_21220200),//日极值 属性2
//	(u32*)InitData_601E_21220300,(u32*)sizeof(InitData_601E_21220300),//日极值 属性3
//	(u32*)InitData_601E_21230200,(u32*)sizeof(InitData_601E_21230200),//月极值 属性2
//	(u32*)InitData_601E_21230300,(u32*)sizeof(InitData_601E_21230300),//月极值 属性3
//	(u32*)InitData_601E_21240200,(u32*)sizeof(InitData_601E_21240200),//年极值 属性2
//	(u32*)InitData_601E_21240300,(u32*)sizeof(InitData_601E_21240300),//年极值 属性3
		
	(u32*)InitData_601E_21300200,(u32*)sizeof(InitData_601E_21300200),//总电压合格率
	(u32*)InitData_601E_21310200,(u32*)sizeof(InitData_601E_21310200),//当月A相电压合格率
	(u32*)InitData_601E_21320200,(u32*)sizeof(InitData_601E_21320200),//当月B相电压合格率
	(u32*)InitData_601E_21330200,(u32*)sizeof(InitData_601E_21330200),//当月C相电压合格率
	(u32*)InitData_601E_21400200,(u32*)sizeof(InitData_601E_21400200),//日最大有功功率及发生时间
	(u32*)InitData_601E_21410200,(u32*)sizeof(InitData_601E_21410200),//月最大有功功率及发生时间

//	(u32*)InitData_601E_22000200,(u32*)sizeof(InitData_601E_22000200),//通讯流量
//	(u32*)InitData_601E_22030200,(u32*)sizeof(InitData_601E_22030200),//供电时间
//	(u32*)InitData_601E_22040200,(u32*)sizeof(InitData_601E_22040200),//复位次数
//	(u32*)InitData_601E_25000200,(u32*)sizeof(InitData_601E_25000200),//累计水（热）流量
//	(u32*)InitData_601E_25010200,(u32*)sizeof(InitData_601E_25010200),//累计气流量
//	(u32*)InitData_601E_25020200,(u32*)sizeof(InitData_601E_25020200),//累计热量
//	(u32*)InitData_601E_25030200,(u32*)sizeof(InitData_601E_25030200),//热功率
//	(u32*)InitData_601E_25040200,(u32*)sizeof(InitData_601E_25040200),//累计工作时间
//	(u32*)InitData_601E_25050200,(u32*)sizeof(InitData_601E_25050200),//水温
//	(u32*)InitData_601E_25060200,(u32*)sizeof(InitData_601E_25060200),//（仪表）状态ST
	(u32*)InitData_601E_40000200,(u32*)sizeof(InitData_601E_40000200),//日期时间
	
	
	(u32*)InitData_601E_50040200_20210200,(u32*)sizeof(InitData_601E_50040200_20210200),//1日冻结类.数据冻结时间
	(u32*)InitData_601E_50040200_00100200,(u32*)sizeof(InitData_601E_50040200_00100200),//2日冻结类.正向有功
	(u32*)InitData_601E_50040200_00200200,(u32*)sizeof(InitData_601E_50040200_00200200),//3日冻结类.反向有功
	(u32*)InitData_601E_50040200_00300200,(u32*)sizeof(InitData_601E_50040200_00300200),//4日冻结类.组合无功1
	(u32*)InitData_601E_50040200_00400200,(u32*)sizeof(InitData_601E_50040200_00400200),//5日冻结类.组合无功2
	(u32*)InitData_601E_50040200_00500200,(u32*)sizeof(InitData_601E_50040200_00500200),//6日冻结类.第一象限无功
	(u32*)InitData_601E_50040200_00600200,(u32*)sizeof(InitData_601E_50040200_00600200),//7日冻结类.第二象限无功
	(u32*)InitData_601E_50040200_00700200,(u32*)sizeof(InitData_601E_50040200_00700200),//8日冻结类.第三象限无功
	(u32*)InitData_601E_50040200_00800200,(u32*)sizeof(InitData_601E_50040200_00800200),//9日冻结类.第四象限无功
	(u32*)InitData_601E_50040200_10100200,(u32*)sizeof(InitData_601E_50040200_10100200),//10日冻结类.正向有功最大需量及发生时间	
	(u32*)InitData_601E_50040200_10200200,(u32*)sizeof(InitData_601E_50040200_10200200),//11日冻结类.反向有功最大需量及发生时间	
	(u32*)InitData_601E_50040200_10300200,(u32*)sizeof(InitData_601E_50040200_10300200),//12日冻结类.组合无功1最大需量及发生时间		
	(u32*)InitData_601E_50040200_10400200,(u32*)sizeof(InitData_601E_50040200_10400200),//13日冻结类.组合无功2最大需量及发生时间		
	(u32*)InitData_601E_50040200_20040200,(u32*)sizeof(InitData_601E_50040200_20040200),//14日冻结类.有功功率		
	(u32*)InitData_601E_50040200_20050200,(u32*)sizeof(InitData_601E_50040200_20050200),//15日冻结类.无功功率		
	(u32*)InitData_601E_50040200_202C0200,(u32*)sizeof(InitData_601E_50040200_202C0200),//16日冻结类.剩余金额、购电 次数			
	(u32*)InitData_601E_50040200_202D0200,(u32*)sizeof(InitData_601E_50040200_202D0200),//17日冻结类.透支金额			
	(u32*)InitData_601E_50040200_21310201,(u32*)sizeof(InitData_601E_50040200_21310201),//18日冻结类.A 相电压合格率			
	(u32*)InitData_601E_50040200_21320201,(u32*)sizeof(InitData_601E_50040200_21320201),//19日冻结类.B 相电压合格率			
	(u32*)InitData_601E_50040200_21330201,(u32*)sizeof(InitData_601E_50040200_21330201),//20日冻结类.C 相电压合格率			
	
	
	(u32*)InitData_601E_50060200_20210200,(u32*)sizeof(InitData_601E_50060200_20210200),//1月冻结类.数据冻结时间
	(u32*)InitData_601E_50060200_00100200,(u32*)sizeof(InitData_601E_50060200_00100200),//2月冻结类.正向有功	
	(u32*)InitData_601E_50060200_00200200,(u32*)sizeof(InitData_601E_50060200_00200200),//3月冻结类.反向有功	
	(u32*)InitData_601E_50060200_00300200,(u32*)sizeof(InitData_601E_50060200_00300200),//4月冻结类.组合无功1	
	(u32*)InitData_601E_50060200_00400200,(u32*)sizeof(InitData_601E_50060200_00400200),//5月冻结类.组合无功2	
	(u32*)InitData_601E_50060200_00500200,(u32*)sizeof(InitData_601E_50060200_00500200),//6月冻结类.第一象限无功	
	(u32*)InitData_601E_50060200_00600200,(u32*)sizeof(InitData_601E_50060200_00600200),//7月冻结类.第二象限无功	
	(u32*)InitData_601E_50060200_00700200,(u32*)sizeof(InitData_601E_50060200_00700200),//8月冻结类.第三象限无功	
	(u32*)InitData_601E_50060200_00800200,(u32*)sizeof(InitData_601E_50060200_00800200),//9月冻结类.第四象限无功	
	(u32*)InitData_601E_50060200_10100200,(u32*)sizeof(InitData_601E_50060200_10100200),//10月冻结类.正向有功最大需量及发生时间	
	(u32*)InitData_601E_50060200_10200200,(u32*)sizeof(InitData_601E_50060200_10200200),//11月冻结类.反向有功最大需量及发生时间	
	(u32*)InitData_601E_50060200_10300200,(u32*)sizeof(InitData_601E_50060200_10300200),//12月冻结类.组合无功1最大需量及发生时间	
	(u32*)InitData_601E_50060200_10400200,(u32*)sizeof(InitData_601E_50060200_10400200),//13月冻结类.组合无功2最大需量及发生时间	
	(u32*)InitData_601E_50060200_21300202,(u32*)sizeof(InitData_601E_50060200_21300202),//14月冻结类.总电压合格率	
	(u32*)InitData_601E_50060200_21310202,(u32*)sizeof(InitData_601E_50060200_21310202),//15月冻结类.A相电压合格率	
	(u32*)InitData_601E_50060200_21320202,(u32*)sizeof(InitData_601E_50060200_21320202),//16月冻结类.B相电压合格率	
	(u32*)InitData_601E_50060200_21330202,(u32*)sizeof(InitData_601E_50060200_21330202),//17月冻结类.C相电压合格率	
	(u32*)InitData_601E_50060200_00000200,(u32*)sizeof(InitData_601E_50060200_00000200),//18月冻结类.组合有功电能
	

	
	
};




void DL698_SETINIT_601E(u32 Command)//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
{
	u32 i;
	u32 Len;
	u32 *p32;
	
	if(Command==6)
	{//6=仅部份参数初始化

		return;
	}
	if((sizeof(SETINIT_601E_List)/8)>NUMmax_601E)
	{
		return;
	}
	p32=(u32*)SETINIT_601E_List;
	for(i=0;i<(sizeof(SETINIT_601E_List)/8);i++)
	{
		Len=p32[1];
		if(Len>=LENper_601E)
		{//长度>=预留空间
			return;
		}
		MW(p32[0],ADDR_601E+(i*LENper_601E),Len);
		p32+=2;
	}
//	for(;i<NUMmax_601E;i++)
//	{
//		MC(0,ADDR_601E+(i*LENper_601E),1);
//	}
}

u32 Get_SETINIT_601E_List_NUM(void)//得到采集规则库初始设置库列表个数
{
	return (sizeof(SETINIT_601E_List)/8);
}

#if RUN_DL698TEST_2==1//0无效,1=运行;测试601E规约库
void DL698TEST_2(void)//测试601E规约库
{
	u32 i;
	u32 Len;
	u32 *p32;
	

	if((sizeof(SETINIT_601E_List)/8)>NUMmax_601E)
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""if((sizeof(SETINIT_601E_List)/8)>NUMmax_601E)");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
		while(1);
	}
	p32=(u32*)SETINIT_601E_List;
	for(i=0;i<(sizeof(SETINIT_601E_List)/8);i++)
	{
		Len=p32[1];
		if(Len>=LENper_601E)
		{//长度>=预留空间
			DisplayStringAutoUp(0,9,(u8*)"\x0""if(Len<=LENper_601E)");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			while(1);
		}
		//MW(p32[0],ADDR_601E+(i*LENper_601E),Len);
		p32+=2;
	}
//	for(;i<NUMmax_601E;i++)
//	{
//		MC(0,ADDR_601E+(i*LENper_601E),1);
//	}
}
#endif

#endif






















