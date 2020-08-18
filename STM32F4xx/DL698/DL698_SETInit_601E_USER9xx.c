
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"


//�ɼ������(�¼����������)
//��ҪԼ��:
//�ɼ�������е�ΪROADʱֻ����1��������OAD,�������̻��Զ�ROAD����OAD����
//��DL698ΪROAD�������OADʱ,���ݴ洢ʱ�Ե�������OAD����������ʽ�洢(���ܹ�����������),������һ������OAD���ݲ�����ʱ���ݴ洢���Զ���������OAD
//��DL2007��1997�ɼ�����������OAD�Ե���OAD��ʽ�б�(�����ö��DI����ʾ1������OAD!!!)
//������DL2007��1997�ɼ��������1���б��ʾ���������OAD,���޷����ֶ��DI�Ǳ�ʾ1��OAD���Ƕ��OAD
//���ݳ�������ʱ�ڳ����Ӽ���Ϊ0ʱ��ʾ����Ŀ�ʼҲ����Ϊ�˿����ö��DI����ʾ1������OAD


#if (USER/100)==9//����ϵ

//��������
const u8 InitData_601E_00000200[]=
{
//����й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x00,0xff,0x00,
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x00,0x00,0x00,
	DataType_octet_string,4,0x00,0x00,0x01,0x00,
	DataType_octet_string,4,0x00,0x00,0x02,0x00,
	DataType_octet_string,4,0x00,0x00,0x03,0x00,
	DataType_octet_string,4,0x00,0x00,0x04,0x00,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x01,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x01,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x01,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x90,0x1f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x90,0x10,//DI
	DataType_octet_string,2,0x90,0x11,//DI
	DataType_octet_string,2,0x90,0x12,//DI
	DataType_octet_string,2,0x90,0x13,//DI
	DataType_octet_string,2,0x90,0x14,//DI
//			}��
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
//A�������й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x15,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�������й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x29,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�������й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x3D,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x02,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x02,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x02,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x90,0x2f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x90,0x20,//DI
	DataType_octet_string,2,0x90,0x21,//DI
	DataType_octet_string,2,0x90,0x22,//DI
	DataType_octet_string,2,0x90,0x23,//DI
	DataType_octet_string,2,0x90,0x24,//DI
//			}��
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
//A�෴���й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x16,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�෴���й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2A,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�෴���й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x3E,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����޹�1����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x30,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x03,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x03,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x03,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x1f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x10,//DI
	DataType_octet_string,2,0x91,0x11,//DI
	DataType_octet_string,2,0x91,0x12,//DI
	DataType_octet_string,2,0x91,0x13,//DI
	DataType_octet_string,2,0x91,0x14,//DI	
	
//			}��
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
//A������޹�1����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x31,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x17,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B������޹�1����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x32,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2B,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C������޹�1����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x33,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x3F,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����޹�2����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x40,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x04,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x04,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x04,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x2f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x20,//DI
	DataType_octet_string,2,0x91,0x21,//DI
	DataType_octet_string,2,0x91,0x22,//DI
	DataType_octet_string,2,0x91,0x23,//DI
	DataType_octet_string,2,0x91,0x24,//DI
//			}��
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
//A������޹�2����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x41,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x18,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B������޹�2����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x42,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2C,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C������޹�2����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x43,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x40,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��һ�����޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x50,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x05,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x05,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x05,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x3f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x30,//DI
	DataType_octet_string,2,0x91,0x31,//DI
	DataType_octet_string,2,0x91,0x32,//DI
	DataType_octet_string,2,0x91,0x33,//DI
	DataType_octet_string,2,0x91,0x34,//DI
//			}��
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
//A���һ�����޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x51,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x19,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B���һ�����޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x52,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2D,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C���һ�����޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x53,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x41,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ڶ������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x60,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x06,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x06,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x06,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x5f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x50,//DI
	DataType_octet_string,2,0x91,0x51,//DI
	DataType_octet_string,2,0x91,0x52,//DI
	DataType_octet_string,2,0x91,0x53,//DI
	DataType_octet_string,2,0x91,0x54,//DI
//			}��
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
//A��ڶ������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x61,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1A,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B��ڶ������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x62,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2E,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C��ڶ������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x63,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x42,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x70,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x07,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x07,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x07,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x6f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x60,//DI
	DataType_octet_string,2,0x91,0x61,//DI
	DataType_octet_string,2,0x91,0x62,//DI
	DataType_octet_string,2,0x91,0x63,//DI
	DataType_octet_string,2,0x91,0x64,//DI
//			}��
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
//A����������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x71,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1B,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B����������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x72,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x2F,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C����������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x73,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x43,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x80,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x08,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x08,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x08,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x4f,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,5,
	DataType_octet_string,2,0x91,0x40,//DI
	DataType_octet_string,2,0x91,0x41,//DI
	DataType_octet_string,2,0x91,0x42,//DI
	DataType_octet_string,2,0x91,0x43,//DI
	DataType_octet_string,2,0x91,0x44,//DI
//			}��
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
//A����������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x81,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1C,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B����������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x82,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x30,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C����������޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x83,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x44,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�������ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x90,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x09,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x09,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x09,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A���������ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x91,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1D,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B���������ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x92,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x31,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C���������ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0x93,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x45,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�������ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA0,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x0A,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x00,0x0A,0x00,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x01,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x02,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x03,0x00,//DI
	DataType_octet_string,4,0x00,0x0A,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�෴�����ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA1,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x1E,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�෴�����ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA2,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x32,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�෴�����ڵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x00,0xA3,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x46,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й������ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x81,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�������й���������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x95,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�������й���������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xA9,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�������й���������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBD,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й����� �ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x82,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�෴���й���������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x96,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�෴���й���������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAA,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�෴���й���������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x01,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBE,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й�г�� �ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x83,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�������й�г�� ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x97,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�������й�г�� ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAB,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�������й�г�� ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBF,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й�г�� �ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x84,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�෴���й�г�� ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x98,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�෴���й�г�� ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAC,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�෴���й�г�� ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x02,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xC0,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//ͭ���й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x85,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A��ͭ���й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x01,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x99,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B��ͭ���й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x02,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAD,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C��ͭ���й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x03,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xC1,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x86,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�������й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x01,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x9A,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�������й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x02,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xAE,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�������й��ܵ��ܲ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x04,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xC2,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x80,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A������ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x01,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x94,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B������ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x02,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xA8,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C������ܵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x05,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xBC,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������
//�����й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x01,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x01,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x01,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x1F,//DI
	DataType_octet_string,2,0xB0,0x1F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A�������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x15,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x29,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x3D,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x02,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x02,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x02,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x2F,//DI
	DataType_octet_string,2,0xB0,0x2F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A�෴���й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x16,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�෴���й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2A,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�෴���й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x3E,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x30,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x03,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x03,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x03,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x1F,//DI
	DataType_octet_string,2,0xB1,0x1F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x31,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x17,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x32,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2B,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x33,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x3F,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x40,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x04,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x04,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x04,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x2F,//DI
	DataType_octet_string,2,0xB1,0x2F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x41,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x18,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x42,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2C,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x43,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x40,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x50,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x05,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x05,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x05,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x3F,//DI
	DataType_octet_string,2,0xB1,0x3F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A���һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x51,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x19,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B���һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x52,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2D,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C���һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x53,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x41,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x60,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x06,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x06,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x06,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x5F,//DI
	DataType_octet_string,2,0xB1,0x5F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A��ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x61,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1A,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B��ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x62,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2E,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C��ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x63,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x42,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x70,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x07,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x07,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x07,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x6F,//DI
	DataType_octet_string,2,0xB1,0x6F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x71,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1B,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x72,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x2F,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x73,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x43,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x80,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x08,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x08,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x08,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x4F,//DI
	DataType_octet_string,2,0xB1,0x4F,//DI
//				����DI  array octet-string(SIZE(2))
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
//			}��
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
//A����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x81,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1C,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x82,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x30,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x83,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x44,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x90,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x09,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x09,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x09,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A�����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x91,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1D,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B�����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x92,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x31,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C�����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0x93,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x45,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA0,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x0A,0xff,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,5,
	DataType_octet_string,4,0x01,0x0A,0x00,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x01,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x02,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x03,0x00,//DI
	DataType_octet_string,4,0x01,0x0A,0x04,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//A �෴�������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA1,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x1E,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//B �෴�������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA2,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x32,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//C �෴�������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x10,0xA3,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x46,0x00,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A�������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B�������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C�������й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������ڷ����й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A�෴���й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B�෴���й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C�෴���й��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x30,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x31,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x32,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C������޹�1�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x33,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x40,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x41,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x42,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C������޹�2�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x43,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������ڵ�һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x50,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A���һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x51,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B���һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x52,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C���һ�����������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x53,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������ڵڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x60,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A��ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x61,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B��ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x62,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C��ڶ������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x63,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������ڵ��������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x70,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x71,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x72,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x73,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������ڵ��������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x80,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x81,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x82,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x83,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�������������������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x90,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A�����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x91,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B�����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x92,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C�����������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0x93,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���������ڷ��������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA0,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������A �෴�������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA1,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������B �෴�������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA2,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����������C�෴�������������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x11,0xA3,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������
//��ѹ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x01,0xFF,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x01,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x01,0x02,0x00,//DI
	DataType_octet_string,4,0x02,0x01,0x03,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,3,
	DataType_octet_string,2,0xB6,0x11,//DI
	DataType_octet_string,2,0xB6,0x12,//DI
	DataType_octet_string,2,0xB6,0x13,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x01,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x02,0xFF,0x00,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x02,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x02,0x02,0x00,//DI
	DataType_octet_string,4,0x02,0x02,0x03,0x00,//DI
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,3,
	DataType_octet_string,2,0xB6,0x21,//DI
	DataType_octet_string,2,0xB6,0x22,//DI
	DataType_octet_string,2,0xB6,0x23,//DI
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���ߵ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x01,0x04,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x01,//DI
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x02,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹ�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x07,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x07,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x07,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x07,0x03,0x00,//DI		
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x04,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x03,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x03,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x03,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x03,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x03,0x03,0x00,//DI		
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,4,
	DataType_octet_string,2,0xB6,0x30,//DI	
	DataType_octet_string,2,0xB6,0x31,//DI	
	DataType_octet_string,2,0xB6,0x32,//DI
	DataType_octet_string,2,0xB6,0x33,//DI	
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x05,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x04,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x04,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x04,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x04,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x04,0x03,0x00,//DI		
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,4,
	DataType_octet_string,2,0xB6,0x40,//DI	
	DataType_octet_string,2,0xB6,0x41,//DI	
	DataType_octet_string,2,0xB6,0x42,//DI
	DataType_octet_string,2,0xB6,0x43,//DI	
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���ڹ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x06,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x05,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x05,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x05,0x01,0x00,//DI	
	DataType_octet_string,4,0x02,0x05,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x05,0x03,0x00,//DI		
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//һ����ƽ���й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x07,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x03,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//һ����ƽ���޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x08,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//һ����ƽ�����ڹ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x09,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0A,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x06,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,4,
	DataType_octet_string,4,0x02,0x06,0x00,0x00,//DI	
	DataType_octet_string,4,0x02,0x06,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x06,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x06,0x03,0x00,//DI	
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,4,
	DataType_octet_string,2,0xB6,0x50,//DI	
	DataType_octet_string,2,0xB6,0x51,//DI	
	DataType_octet_string,2,0xB6,0x52,//DI	
	DataType_octet_string,2,0xB6,0x53,//DI	
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹ����ʧ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0B,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x08,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x08,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x08,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x08,0x03,0x00,//DI	
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������ʧ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0C,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x09,0xFF,0x00,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,3,
	DataType_octet_string,4,0x02,0x09,0x01,0x00,//DI
	DataType_octet_string,4,0x02,0x09,0x02,0x00,//DI	
	DataType_octet_string,4,0x02,0x09,0x03,0x00,//DI	
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹг�����������ܼ�2��n�Σ�
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0A,0x01,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
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
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹг�����������ܼ�2��n�Σ�
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0A,0x02,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
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
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹг�����������ܼ�2��n�Σ�
//	����4��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x04,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0A,0x03,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
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
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹг�����������ܼ�2��n�Σ�
//	����5��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0D,0x05,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x04,0x00,0x02,0x06,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����г�����������ܼ�2��n�Σ�
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0B,0x01,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
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
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����г�����������ܼ�2��n�Σ�
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0B,0x02,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
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
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����г�����������ܼ�2��n�Σ�
//	����4��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x04,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x0B,0x03,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
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
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����г�����������ܼ�2��n�Σ�
//	����5��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0E,0x05,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x04,0x00,0x02,0x06,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����Ƶ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x0F,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x02,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����¶�
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x07,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//ʱ�ӵ�ص�ѹ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x08,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//ͣ�糭���ص�ѹ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x09,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//ʱ�ӵ�ع���ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x0A,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0xB2,0x14,//DI	
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���ܱ�����״̬��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x14,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x04,0x00,0x05,0xFF,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,7,
	DataType_octet_string,4,0x04,0x00,0x05,0x01,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x02,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x03,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x04,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x05,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x06,//DI	
	DataType_octet_string,4,0x04,0x00,0x05,0x07,//DI	
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0xC0,0x20,//DI	
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ǰ�й�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x17,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x04,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ǰ�޹�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x18,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x05,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ǰ��������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x19,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x02,0x80,0x00,0x06,//DI	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ǰ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1A,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ǰ���ʵ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1B,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ǰ���ݵ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1C,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼�����ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x1E,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼�����ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���ݶ���ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼���¼���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����¼���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼�����Դ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x24,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼���ǰֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x25,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ѹ��ƽ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x26,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ƽ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x27,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x28,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ʱֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x29,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Ŀ���������ַ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2A,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����ǰ��Ǯ���ļ�
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2C,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����ǰ��͸֧���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2D,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ۼƹ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x2E,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¶��õ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x31,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���ݽ����õ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x32,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������ִ��״̬��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x40,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�����������״̬��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x20,0x41,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������ͳ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��������ͳ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x00,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Сʱ����ͳ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x01,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Сʱ����ͳ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x01,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ͳ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x02,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ͳ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x02,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ͳ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ͳ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x03,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ͳ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x04,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������ͳ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x04,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����ƽ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x10,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����ƽ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x10,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Сʱƽ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x11,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Сʱƽ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x11,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ƽ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x12,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ƽ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x12,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ƽ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x13,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ƽ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x13,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ƽ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x14,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��ƽ��
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x14,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���Ӽ�ֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x20,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���Ӽ�ֵ
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x20,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Сʱ��ֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x21,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//Сʱ��ֵ
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x21,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ռ�ֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x22,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ռ�ֵ
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x22,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼�ֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x23,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�¼�ֵ
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x23,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�꼫ֵ
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x24,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�꼫ֵ
//	����3��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x24,0x03,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ܵ�ѹ�ϸ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x30,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����A���ѹ�ϸ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x31,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����B���ѹ�ϸ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x32,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����C���ѹ�ϸ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x33,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������й����ʼ�����ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x40,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//������й����ʼ�����ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x21,0x41,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//ͨѶ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x22,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x22,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//��λ����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x22,0x04,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ۼ�ˮ���ȣ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ۼ�������
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x01,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ۼ�����
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x02,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ȹ���
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x03,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//�ۼƹ���ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x04,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//ˮ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x05,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,	
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//���Ǳ�״̬ST
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x25,0x06,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
//����ʱ��
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,0,//0=OAD,1=ROAD
	0x40,0x00,0x02,0x00,//OAD
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,2,
	DataType_octet_string,4,0x04,0x00,0x01,0x02,//DI���ܱ�����ʱ��Hhmmss	
	DataType_octet_string,4,0x04,0x00,0x01,0x01,//DI���ܱ�����ʱ��YYMMDDWW	
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xC0,0x11,//DI���ܱ�����ʱ��Hhmmss	
	DataType_octet_string,2,0xC0,0x10,//DI���ܱ�����ʱ��YYMMDDWW	
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	





//�ն�����.���ݶ���ʱ��
const u8 InitData_601E_50040200_20210200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x20,0x21,0x02,0x00,  //���ݶ���ʱ��
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x00,0x01,//����ʱ��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0xC0,0x10,//��ǰ������������
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//�ն�����.�����й�
const u8 InitData_601E_50040200_00100200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x10,0x02,0x00,  //�����й�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x01,0x01,//�����й�����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x90,0x1F,//�����й�����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//�ն�����.�����й�
const u8 InitData_601E_50040200_00200200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x20,0x02,0x00,  //�����й�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x02,0x01,//�����й�����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x90,0x2F,//�����й�����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.����޹�1
const u8 InitData_601E_50040200_00300200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x30,0x02,0x00,  //����޹�1����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x03,0x01,//����޹�1����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x1F,//����޹�1����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.����޹�2
const u8 InitData_601E_50040200_00400200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x40,0x02,0x00,  //����޹�2����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x04,0x01,//����޹�2����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x2F,//����޹�2����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.��һ�����޹�
const u8 InitData_601E_50040200_00500200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x50,0x02,0x00,  //��һ�����޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x05,0x01,//��һ�����޹�����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x3F,//��һ�����޹�����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.�ڶ������޹�
const u8 InitData_601E_50040200_00600200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x60,0x02,0x00,  //�ڶ������޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x06,0x01,//�ڶ������޹�����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x5F,//�ڶ������޹�����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.���������޹�
const u8 InitData_601E_50040200_00700200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x70,0x02,0x00,  //���������޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x07,0x01,//���������޹�����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x6F,//���������޹�����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.���������޹�
const u8 InitData_601E_50040200_00800200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x00,0x80,0x02,0x00,  //���������޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x08,0x01,//���������޹�����
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x91,0x4F,//���������޹�����
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.�����й��������������ʱ��	
const u8 InitData_601E_50040200_10100200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x10,0x10,0x02,0x00,  //�����й��������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x09,0x01,//�����й��������������ʱ��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x1F,//�����й��������
	DataType_octet_string,2,0xB0,0x1F,//�����й������������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.�����й��������������ʱ��	
const u8 InitData_601E_50040200_10200200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x10,0x20,0x02,0x00,  //�����й��������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x0A,0x01,//�����й��������������ʱ��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA0,0x2F,//�����й��������
	DataType_octet_string,2,0xB0,0x2F,//�����й������������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.����޹�1�������������ʱ��	
const u8 InitData_601E_50040200_10300200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x10,0x30,0x02,0x00,  //����޹�1�������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x03,0xFF,0x00,//���������޹��������������ʱ�䣨�ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x1F,//����޹�1�������
	DataType_octet_string,2,0xB1,0x1F,//����޹�1�����������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.����޹�2�������������ʱ��	
const u8 InitData_601E_50040200_10400200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x10,0x40,0x02,0x00,  //����޹�2�������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x04,0xFF,0x00,//���·����޹��������������ʱ�䣨�ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA1,0x2F,//����޹�2�������
	DataType_octet_string,2,0xB1,0x2F,//����޹�2�����������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.�й�����	
const u8 InitData_601E_50040200_20040200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x20,0x04,0x02,0x00,  //�й�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x10,0x01,//��1���ն���������ݣ��ֽ���3*8��(��/A/B/C�й�����,��/A/B/C�޹�����)
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.�޹�����	
const u8 InitData_601E_50040200_20050200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x20,0x05,0x02,0x00,  //�޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x06,0x10,0x01,//��1���ն���������ݣ��ֽ���3*8��(��/A/B/C�й�����,��/A/B/C�޹�����)
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.ʣ������� ����	
const u8 InitData_601E_50040200_202C0200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x20,0x2C,0x02,0x00,  //ʣ������� ����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x08,0x02,0x01,//��1��ʣ�����ն��ᣨ2���������4ʣ���4͸֧��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.͸֧���
const u8 InitData_601E_50040200_202D0200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x20,0x2D,0x02,0x00,  //͸֧���
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x05,0x08,0x02,0x01,//��1��ʣ�����ն��ᣨ2���������4ʣ���4͸֧��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.A ���ѹ�ϸ���
const u8 InitData_601E_50040200_21310201[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x21,0x31,0x02,0x01,  //A ���ѹ�ϸ���
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.B ���ѹ�ϸ���
const u8 InitData_601E_50040200_21320201[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x21,0x32,0x02,0x01,  //B ���ѹ�ϸ���
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�ն�����.C ���ѹ�ϸ���
const u8 InitData_601E_50040200_21330201[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x04,0x02,0x00,//OAD
	1,//��������
	0x21,0x33,0x02,0x01,  //C ���ѹ�ϸ���
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.���ݶ���ʱ��
const u8 InitData_601E_50060200_20210200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x20,0x21,0x02,0x00,  //���ݶ���ʱ��
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0xFE,0x00,0x01,//��1���½����ս���ʱ�� 
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//�¶�����.�����й�
const u8 InitData_601E_50060200_00100200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x10,0x02,0x00,  //�����й�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x01,0xFF,0x01,//����һ�����գ������й�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x94,0x1F,//����һ�����գ������й�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};	

//�¶�����.�����й�
const u8 InitData_601E_50060200_00200200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x20,0x02,0x00,  //�����й�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x02,0xFF,0x01,//����һ�����գ������й�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x94,0x2F,//����һ�����գ������й�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.����޹�1
const u8 InitData_601E_50060200_00300200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x30,0x02,0x00,  //����޹�1����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x03,0xFF,0x01,//����һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x95,0x1F,//����һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.����޹�2
const u8 InitData_601E_50060200_00400200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x40,0x02,0x00,  //����޹�2����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x04,0xFF,0x01,//����һ�����գ������޹�������޹�2������ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x95,0x2F,//����һ�����գ������޹�������޹�2������ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.��һ�����޹�
const u8 InitData_601E_50060200_00500200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x50,0x02,0x00,  //��һ�����޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x05,0xFF,0x01,//����һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x95,0x3F,//����һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.�ڶ������޹�
const u8 InitData_601E_50060200_00600200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x60,0x02,0x00,  //�ڶ������޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x06,0xFF,0x01,//����һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x95,0x5F,//����һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.���������޹�
const u8 InitData_601E_50060200_00700200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x70,0x02,0x00,  //���������޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x07,0xFF,0x01,//����һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x95,0x6F,//����һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.���������޹�
const u8 InitData_601E_50060200_00800200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x80,0x02,0x00,  //���������޹�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x08,0xFF,0x01,//����һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,1,
	DataType_octet_string,2,0x95,0x4F,//����һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.�����й��������������ʱ��	
const u8 InitData_601E_50060200_10100200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x10,0x10,0x02,0x00,  //�����й��������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x01,0xFF,0x01,//����һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA4,0x1F,//�����й��������
	DataType_octet_string,2,0xB4,0x1F,//�����й������������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.�����й��������������ʱ��	
const u8 InitData_601E_50060200_10200200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x10,0x20,0x02,0x00,  //�����й��������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x02,0xFF,0x01,//����һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA4,0x2F,//�����й��������
	DataType_octet_string,2,0xB4,0x2F,//�����й������������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.����޹�1�������������ʱ��
const u8 InitData_601E_50060200_10300200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x10,0x30,0x02,0x00,  //����޹�1�������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x03,0xFF,0x01,//����һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA5,0x1F,//����޹�1�������
	DataType_octet_string,2,0xB5,0x1F,//����޹�1�����������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.����޹�2�������������ʱ��
const u8 InitData_601E_50060200_10400200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x10,0x40,0x02,0x00,  //����޹�1�������������ʱ��	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x01,0x04,0xFF,0x01,//����һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,2,
	DataType_octet_string,2,0xA5,0x2F,//����޹�1�������
	DataType_octet_string,2,0xB5,0x2F,//����޹�1�����������ʱ��
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.�ܵ�ѹ�ϸ���
const u8 InitData_601E_50060200_21300202[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x21,0x30,0x02,0x02,  //�ܵ�ѹ�ϸ���	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.A���ѹ�ϸ���
const u8 InitData_601E_50060200_21310202[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x21,0x31,0x02,0x02,  //A���ѹ�ϸ���	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.B���ѹ�ϸ���
const u8 InitData_601E_50060200_21320202[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x21,0x32,0x02,0x02,  //B���ѹ�ϸ���	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.C���ѹ�ϸ���
const u8 InitData_601E_50060200_21330202[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x21,0x33,0x02,0x02,  //C���ѹ�ϸ���	
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
//			AcqCmd_Trans  structure
	DataType_structure,0,
//			{
//				Frame  octet-string
//			}
//		}
//	}	
};

//�¶�����.����й�����
const u8 InitData_601E_50060200_00000200[]=
{
//	����2��=structure
	DataType_structure,2,
//	{
//		������ѡ��������  CSD��
	DataType_CSD,1,//0=OAD,1=ROAD
	0x50,0x06,0x02,0x00,//OAD
	1,//��������
	0x00,0x00,0x02,0x00,  //����й�����
//		��������  structrue
	DataType_structure,3,
//		{
//			AcqCmd_2007  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(4))��
	DataType_array,1,
	DataType_octet_string,4,0x00,0x00,0xFF,0x01,//��1 ����������й�����ʾֵ
//				����DI  array octet-string(SIZE(4))
	DataType_array,0,
//			}��
//			AcqCmd_1997  structure
	DataType_structure,2,
//			{
//				����DI  array octet-string(SIZE(2))��
	DataType_array,0,
//				����DI  array octet-string(SIZE(2))
	DataType_array,0,
//			}��
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
	(u32*)InitData_601E_00000200,(u32*)sizeof(InitData_601E_00000200),//����й�����
	(u32*)InitData_601E_00100200,(u32*)sizeof(InitData_601E_00100200),//�����й�����
	(u32*)InitData_601E_00110200,(u32*)sizeof(InitData_601E_00110200),//A�������й�����
	(u32*)InitData_601E_00120200,(u32*)sizeof(InitData_601E_00120200),//B�������й�����
	(u32*)InitData_601E_00130200,(u32*)sizeof(InitData_601E_00130200),//C�������й�����
	(u32*)InitData_601E_00200200,(u32*)sizeof(InitData_601E_00200200),//�����й�����
	(u32*)InitData_601E_00210200,(u32*)sizeof(InitData_601E_00210200),//A�෴���й�����
	(u32*)InitData_601E_00220200,(u32*)sizeof(InitData_601E_00220200),//B�෴���й�����
	(u32*)InitData_601E_00230200,(u32*)sizeof(InitData_601E_00230200),//C�෴���й�����
	(u32*)InitData_601E_00300200,(u32*)sizeof(InitData_601E_00300200),//����޹�1����
	(u32*)InitData_601E_00310200,(u32*)sizeof(InitData_601E_00310200),//A������޹�1����
	(u32*)InitData_601E_00320200,(u32*)sizeof(InitData_601E_00320200),//B������޹�1����
	(u32*)InitData_601E_00330200,(u32*)sizeof(InitData_601E_00330200),//C������޹�1����
	(u32*)InitData_601E_00400200,(u32*)sizeof(InitData_601E_00400200),//����޹�2����
	(u32*)InitData_601E_00410200,(u32*)sizeof(InitData_601E_00410200),//A������޹�2����
	(u32*)InitData_601E_00420200,(u32*)sizeof(InitData_601E_00420200),//B������޹�2����
	(u32*)InitData_601E_00430200,(u32*)sizeof(InitData_601E_00430200),//C������޹�2����
	(u32*)InitData_601E_00500200,(u32*)sizeof(InitData_601E_00500200),//��һ�����޹�����
	(u32*)InitData_601E_00510200,(u32*)sizeof(InitData_601E_00510200),//A���һ�����޹�����
	(u32*)InitData_601E_00520200,(u32*)sizeof(InitData_601E_00520200),//B���һ�����޹�����
	(u32*)InitData_601E_00530200,(u32*)sizeof(InitData_601E_00530200),//C���һ�����޹�����
	(u32*)InitData_601E_00600200,(u32*)sizeof(InitData_601E_00600200),//�ڶ������޹�����
	(u32*)InitData_601E_00610200,(u32*)sizeof(InitData_601E_00610200),//A��ڶ������޹�����
	(u32*)InitData_601E_00620200,(u32*)sizeof(InitData_601E_00620200),//B��ڶ������޹�����
	(u32*)InitData_601E_00630200,(u32*)sizeof(InitData_601E_00630200),//C��ڶ������޹�����
	(u32*)InitData_601E_00700200,(u32*)sizeof(InitData_601E_00700200),//���������޹�����
	(u32*)InitData_601E_00710200,(u32*)sizeof(InitData_601E_00710200),//A����������޹�����
	(u32*)InitData_601E_00720200,(u32*)sizeof(InitData_601E_00720200),//B����������޹�����
	(u32*)InitData_601E_00730200,(u32*)sizeof(InitData_601E_00730200),//C����������޹�����
	(u32*)InitData_601E_00800200,(u32*)sizeof(InitData_601E_00800200),//���������޹�����
	(u32*)InitData_601E_00810200,(u32*)sizeof(InitData_601E_00810200),//A����������޹�����
	(u32*)InitData_601E_00820200,(u32*)sizeof(InitData_601E_00820200),//B����������޹�����
	(u32*)InitData_601E_00830200,(u32*)sizeof(InitData_601E_00830200),//C����������޹�����
	(u32*)InitData_601E_00900200,(u32*)sizeof(InitData_601E_00900200),//�������ڵ���
	(u32*)InitData_601E_00910200,(u32*)sizeof(InitData_601E_00910200),//A���������ڵ���
	(u32*)InitData_601E_00920200,(u32*)sizeof(InitData_601E_00920200),//B���������ڵ���
	(u32*)InitData_601E_00930200,(u32*)sizeof(InitData_601E_00930200),//C���������ڵ���
	(u32*)InitData_601E_00A00200,(u32*)sizeof(InitData_601E_00A00200),//�������ڵ���
	(u32*)InitData_601E_00A10200,(u32*)sizeof(InitData_601E_00A10200),//A�෴�����ڵ���
	(u32*)InitData_601E_00A20200,(u32*)sizeof(InitData_601E_00A20200),//B�෴�����ڵ���
	(u32*)InitData_601E_00A30200,(u32*)sizeof(InitData_601E_00A30200),//C�෴�����ڵ���
	(u32*)InitData_601E_01100200,(u32*)sizeof(InitData_601E_01100200),//�����й����� �ܵ���
	(u32*)InitData_601E_01110200,(u32*)sizeof(InitData_601E_01110200),//A�������й���������
	(u32*)InitData_601E_01120200,(u32*)sizeof(InitData_601E_01120200),//B�������й���������
	(u32*)InitData_601E_01130200,(u32*)sizeof(InitData_601E_01130200),//C�������й���������
	(u32*)InitData_601E_01200200,(u32*)sizeof(InitData_601E_01200200),//�����й����� �ܵ���
	(u32*)InitData_601E_01210200,(u32*)sizeof(InitData_601E_01210200),//A�෴���й���������
	(u32*)InitData_601E_01220200,(u32*)sizeof(InitData_601E_01220200),//B�෴���й���������
	(u32*)InitData_601E_01230200,(u32*)sizeof(InitData_601E_01230200),//C�෴���й���������
	(u32*)InitData_601E_02100200,(u32*)sizeof(InitData_601E_02100200),//�����й�г�� �ܵ���
	(u32*)InitData_601E_02110200,(u32*)sizeof(InitData_601E_02110200),//A�������й�г�� ����
	(u32*)InitData_601E_02120200,(u32*)sizeof(InitData_601E_02120200),//B�������й�г�� ����
	(u32*)InitData_601E_02130200,(u32*)sizeof(InitData_601E_02130200),//C�������й�г�� ����
	(u32*)InitData_601E_02200200,(u32*)sizeof(InitData_601E_02200200),//�����й�г�� �ܵ���
	(u32*)InitData_601E_02210200,(u32*)sizeof(InitData_601E_02210200),//A�෴���й�г�� ����
	(u32*)InitData_601E_02220200,(u32*)sizeof(InitData_601E_02220200),//B�෴���й�г�� ����
	(u32*)InitData_601E_02230200,(u32*)sizeof(InitData_601E_02230200),//C�෴���й�г�� ����
	(u32*)InitData_601E_03000200,(u32*)sizeof(InitData_601E_03000200),//ͭ���й��ܵ��ܲ�����
	(u32*)InitData_601E_03010200,(u32*)sizeof(InitData_601E_03010200),//A��ͭ���й��ܵ��ܲ�����
	(u32*)InitData_601E_03020200,(u32*)sizeof(InitData_601E_03020200),//B��ͭ���й��ܵ��ܲ�����
	(u32*)InitData_601E_03030200,(u32*)sizeof(InitData_601E_03030200),//C��ͭ���й��ܵ��ܲ�����
	(u32*)InitData_601E_04000200,(u32*)sizeof(InitData_601E_04000200),//�����й��ܵ��ܲ�����
	(u32*)InitData_601E_04010200,(u32*)sizeof(InitData_601E_04010200),//A�������й��ܵ��ܲ�����
	(u32*)InitData_601E_04020200,(u32*)sizeof(InitData_601E_04020200),//B�������й��ܵ��ܲ�����
	(u32*)InitData_601E_04030200,(u32*)sizeof(InitData_601E_04030200),//C�������й��ܵ��ܲ�����
	(u32*)InitData_601E_05000200,(u32*)sizeof(InitData_601E_05000200),//�����ܵ���
	(u32*)InitData_601E_05010200,(u32*)sizeof(InitData_601E_05010200),//A������ܵ���
	(u32*)InitData_601E_05020200,(u32*)sizeof(InitData_601E_05020200),//B������ܵ���
	(u32*)InitData_601E_05030200,(u32*)sizeof(InitData_601E_05030200),//C������ܵ���	
	(u32*)InitData_601E_10100200,(u32*)sizeof(InitData_601E_10100200),//�����й��������
	(u32*)InitData_601E_10110200,(u32*)sizeof(InitData_601E_10110200),//A�������й��������
	(u32*)InitData_601E_10120200,(u32*)sizeof(InitData_601E_10120200),//B�������й��������
	(u32*)InitData_601E_10130200,(u32*)sizeof(InitData_601E_10130200),//C�������й��������
	(u32*)InitData_601E_10200200,(u32*)sizeof(InitData_601E_10200200),//�����й��������
	(u32*)InitData_601E_10210200,(u32*)sizeof(InitData_601E_10210200),//A�෴���й��������
	(u32*)InitData_601E_10220200,(u32*)sizeof(InitData_601E_10220200),//B�෴���й��������
	(u32*)InitData_601E_10230200,(u32*)sizeof(InitData_601E_10230200),//C�෴���й��������
	(u32*)InitData_601E_10300200,(u32*)sizeof(InitData_601E_10300200),//����޹�1�������
	(u32*)InitData_601E_10310200,(u32*)sizeof(InitData_601E_10310200),//A������޹�1�������
	(u32*)InitData_601E_10320200,(u32*)sizeof(InitData_601E_10320200),//B������޹�1�������
	(u32*)InitData_601E_10330200,(u32*)sizeof(InitData_601E_10330200),//C������޹�1�������
	(u32*)InitData_601E_10400200,(u32*)sizeof(InitData_601E_10400200),//����޹�2�������
	(u32*)InitData_601E_10410200,(u32*)sizeof(InitData_601E_10410200),//A������޹�2�������
	(u32*)InitData_601E_10420200,(u32*)sizeof(InitData_601E_10420200),//B������޹�2�������
	(u32*)InitData_601E_10430200,(u32*)sizeof(InitData_601E_10430200),//C������޹�2�������
	(u32*)InitData_601E_10500200,(u32*)sizeof(InitData_601E_10500200),//��һ�����������
	(u32*)InitData_601E_10510200,(u32*)sizeof(InitData_601E_10510200),//A���һ�����������
	(u32*)InitData_601E_10520200,(u32*)sizeof(InitData_601E_10520200),//B���һ�����������
	(u32*)InitData_601E_10530200,(u32*)sizeof(InitData_601E_10530200),//C���һ�����������
	(u32*)InitData_601E_10600200,(u32*)sizeof(InitData_601E_10600200),//�ڶ������������
	(u32*)InitData_601E_10610200,(u32*)sizeof(InitData_601E_10610200),//A��ڶ������������
	(u32*)InitData_601E_10620200,(u32*)sizeof(InitData_601E_10620200),//B��ڶ������������
	(u32*)InitData_601E_10630200,(u32*)sizeof(InitData_601E_10630200),//C��ڶ������������
	(u32*)InitData_601E_10700200,(u32*)sizeof(InitData_601E_10700200),//���������������
	(u32*)InitData_601E_10710200,(u32*)sizeof(InitData_601E_10710200),//A����������������
	(u32*)InitData_601E_10720200,(u32*)sizeof(InitData_601E_10720200),//B����������������	
	(u32*)InitData_601E_10730200,(u32*)sizeof(InitData_601E_10730200),//C����������������	
	(u32*)InitData_601E_10800200,(u32*)sizeof(InitData_601E_10800200),//���������������
	(u32*)InitData_601E_10810200,(u32*)sizeof(InitData_601E_10810200),//A����������������
  (u32*)InitData_601E_10820200,(u32*)sizeof(InitData_601E_10820200),//B����������������
	(u32*)InitData_601E_10830200,(u32*)sizeof(InitData_601E_10830200),//C����������������
	(u32*)InitData_601E_10900200,(u32*)sizeof(InitData_601E_10900200),//���������������
	(u32*)InitData_601E_10910200,(u32*)sizeof(InitData_601E_10910200),//A�����������������
	(u32*)InitData_601E_10920200,(u32*)sizeof(InitData_601E_10920200),//B�����������������
	(u32*)InitData_601E_10930200,(u32*)sizeof(InitData_601E_10930200),//C�����������������
	(u32*)InitData_601E_10A00200,(u32*)sizeof(InitData_601E_10A00200),//���������������
	(u32*)InitData_601E_10A10200,(u32*)sizeof(InitData_601E_10A10200),//A �෴�������������
	(u32*)InitData_601E_10A20200,(u32*)sizeof(InitData_601E_10A20200),//B �෴�������������
	(u32*)InitData_601E_10A30200,(u32*)sizeof(InitData_601E_10A30200),//C �෴�������������
//	(u32*)InitData_601E_11100200,(u32*)sizeof(InitData_601E_11100200),//���������������й��������
//	(u32*)InitData_601E_11110200,(u32*)sizeof(InitData_601E_11110200),//����������A�������й��������
//	(u32*)InitData_601E_11120200,(u32*)sizeof(InitData_601E_11120200),//����������B�������й��������
//	(u32*)InitData_601E_11130200,(u32*)sizeof(InitData_601E_11130200),//����������C�������й��������
//	(u32*)InitData_601E_11200200,(u32*)sizeof(InitData_601E_11200200),//���������ڷ����й��������
//	(u32*)InitData_601E_11210200,(u32*)sizeof(InitData_601E_11210200),//����������A�෴���й��������
//	(u32*)InitData_601E_11220200,(u32*)sizeof(InitData_601E_11220200),//����������B�෴���й��������
//	(u32*)InitData_601E_11230200,(u32*)sizeof(InitData_601E_11230200),//����������C�෴���й��������
//	(u32*)InitData_601E_11300200,(u32*)sizeof(InitData_601E_11300200),//��������������޹�1�������
//	(u32*)InitData_601E_11310200,(u32*)sizeof(InitData_601E_11310200),//����������A������޹�1�������
//	(u32*)InitData_601E_11320200,(u32*)sizeof(InitData_601E_11320200),//����������B������޹�1�������
//	(u32*)InitData_601E_11330200,(u32*)sizeof(InitData_601E_11330200),//����������C������޹�1�������
//	(u32*)InitData_601E_11400200,(u32*)sizeof(InitData_601E_11400200),//��������������޹�2�������
//	(u32*)InitData_601E_11410200,(u32*)sizeof(InitData_601E_11410200),//����������A������޹�2�������
//	(u32*)InitData_601E_11420200,(u32*)sizeof(InitData_601E_11420200),//����������B������޹�2�������
//	(u32*)InitData_601E_11430200,(u32*)sizeof(InitData_601E_11430200),//����������C������޹�2�������
//	(u32*)InitData_601E_11500200,(u32*)sizeof(InitData_601E_11500200),//���������ڵ�һ�����������
//	(u32*)InitData_601E_11510200,(u32*)sizeof(InitData_601E_11510200),//����������A���һ�����������
//	(u32*)InitData_601E_11520200,(u32*)sizeof(InitData_601E_11520200),//����������B���һ�����������
//	(u32*)InitData_601E_11530200,(u32*)sizeof(InitData_601E_11530200),//����������C���һ�����������	
//	(u32*)InitData_601E_11600200,(u32*)sizeof(InitData_601E_11600200),//���������ڵڶ������������
//	(u32*)InitData_601E_11610200,(u32*)sizeof(InitData_601E_11610200),//����������A��ڶ������������
//	(u32*)InitData_601E_11620200,(u32*)sizeof(InitData_601E_11620200),//����������B��ڶ������������	
//	(u32*)InitData_601E_11630200,(u32*)sizeof(InitData_601E_11630200),//����������C��ڶ������������	
//	(u32*)InitData_601E_11700200,(u32*)sizeof(InitData_601E_11700200),//���������ڵ��������������
//	(u32*)InitData_601E_11710200,(u32*)sizeof(InitData_601E_11710200),//����������A����������������
//	(u32*)InitData_601E_11720200,(u32*)sizeof(InitData_601E_11720200),//����������B����������������
//	(u32*)InitData_601E_11730200,(u32*)sizeof(InitData_601E_11730200),//����������C����������������
//	(u32*)InitData_601E_11800200,(u32*)sizeof(InitData_601E_11800200),//���������ڵ��������������
//	(u32*)InitData_601E_11810200,(u32*)sizeof(InitData_601E_11810200),//����������A����������������
//	(u32*)InitData_601E_11820200,(u32*)sizeof(InitData_601E_11820200),//����������B����������������
//	(u32*)InitData_601E_11830200,(u32*)sizeof(InitData_601E_11830200),//����������C����������������
//	(u32*)InitData_601E_11900200,(u32*)sizeof(InitData_601E_11900200),//�������������������������
//	(u32*)InitData_601E_11910200,(u32*)sizeof(InitData_601E_11910200),//����������A�����������������
//	(u32*)InitData_601E_11920200,(u32*)sizeof(InitData_601E_11920200),//����������B�����������������	
//	(u32*)InitData_601E_11930200,(u32*)sizeof(InitData_601E_11930200),//����������C�����������������	
//	(u32*)InitData_601E_11A00200,(u32*)sizeof(InitData_601E_11A00200),//���������ڷ��������������
//	(u32*)InitData_601E_11A10200,(u32*)sizeof(InitData_601E_11A10200),//����������A �෴�������������
//	(u32*)InitData_601E_11A20200,(u32*)sizeof(InitData_601E_11A20200),//����������B �෴�������������
//	(u32*)InitData_601E_11A30200,(u32*)sizeof(InitData_601E_11A30200),//����������C �෴�������������	
	(u32*)InitData_601E_20000200,(u32*)sizeof(InitData_601E_20000200),//��ѹ
	(u32*)InitData_601E_20010200,(u32*)sizeof(InitData_601E_20010200),//����
	(u32*)InitData_601E_20010400,(u32*)sizeof(InitData_601E_20010400),//���ߵ���
	(u32*)InitData_601E_20020200,(u32*)sizeof(InitData_601E_20020200),//��ѹ���
	(u32*)InitData_601E_20030200,(u32*)sizeof(InitData_601E_20030200),//��ѹ�������
	(u32*)InitData_601E_20040200,(u32*)sizeof(InitData_601E_20040200),//�й�����
	(u32*)InitData_601E_20050200,(u32*)sizeof(InitData_601E_20050200),//�޹�����
	(u32*)InitData_601E_20060200,(u32*)sizeof(InitData_601E_20060200),//���ڹ���
	(u32*)InitData_601E_20070200,(u32*)sizeof(InitData_601E_20070200),//һ����ƽ���й�����
	(u32*)InitData_601E_20080200,(u32*)sizeof(InitData_601E_20080200),//һ����ƽ���޹�����
	(u32*)InitData_601E_20090200,(u32*)sizeof(InitData_601E_20090200),//һ����ƽ�����ڹ���
	(u32*)InitData_601E_200A0200,(u32*)sizeof(InitData_601E_200A0200),//��������
//	(u32*)InitData_601E_200B0200,(u32*)sizeof(InitData_601E_200B0200),//��ѹ����ʧ���
//	(u32*)InitData_601E_200C0200,(u32*)sizeof(InitData_601E_200C0200),//��������ʧ���
//	(u32*)InitData_601E_200D0200,(u32*)sizeof(InitData_601E_200D0200),//��ѹг�����������ܼ�2��n�Σ�����2
//	(u32*)InitData_601E_200D0300,(u32*)sizeof(InitData_601E_200D0300),//��ѹг�����������ܼ�2��n�Σ�����3
//	(u32*)InitData_601E_200D0400,(u32*)sizeof(InitData_601E_200D0400),//��ѹг�����������ܼ�2��n�Σ�����4
//  (u32*)InitData_601E_200D0500,(u32*)sizeof(InitData_601E_200D0500),//��ѹг�����������ܼ�2��n�Σ�����5
//	(u32*)InitData_601E_200E0200,(u32*)sizeof(InitData_601E_200E0200),//����г�����������ܼ�2��n�Σ�����2
//	(u32*)InitData_601E_200E0300,(u32*)sizeof(InitData_601E_200E0300),//����г�����������ܼ�2��n�Σ�����3
//	(u32*)InitData_601E_200E0400,(u32*)sizeof(InitData_601E_200E0400),//����г�����������ܼ�2��n�Σ�����4
//	(u32*)InitData_601E_200E0500,(u32*)sizeof(InitData_601E_200E0500),//����г�����������ܼ�2��n�Σ�����5	
	(u32*)InitData_601E_200F0200,(u32*)sizeof(InitData_601E_200F0200),//����Ƶ��
	(u32*)InitData_601E_20100200,(u32*)sizeof(InitData_601E_20100200),//�����¶�
	(u32*)InitData_601E_20110200,(u32*)sizeof(InitData_601E_20110200),//ʱ�ӵ�ص�ѹ
	(u32*)InitData_601E_20120200,(u32*)sizeof(InitData_601E_20120200),//ͣ�糭���ص�ѹ
	(u32*)InitData_601E_20130200,(u32*)sizeof(InitData_601E_20130200),//ʱ�ӵ�ع���ʱ��
	(u32*)InitData_601E_20140200,(u32*)sizeof(InitData_601E_20140200),//���ܱ�����״̬��
	(u32*)InitData_601E_20170200,(u32*)sizeof(InitData_601E_20170200),//��ǰ�й�����
	(u32*)InitData_601E_20180200,(u32*)sizeof(InitData_601E_20180200),//��ǰ�޹�����
	(u32*)InitData_601E_20190200,(u32*)sizeof(InitData_601E_20190200),//��ǰ��������
	(u32*)InitData_601E_201A0200,(u32*)sizeof(InitData_601E_201A0200),//��ǰ���
	(u32*)InitData_601E_201B0200,(u32*)sizeof(InitData_601E_201B0200),//��ǰ���ʵ��
	(u32*)InitData_601E_201C0200,(u32*)sizeof(InitData_601E_201C0200),//��ǰ���ݵ��
		
//	(u32*)InitData_601E_201E0200,(u32*)sizeof(InitData_601E_201E0200),//�¼�����ʱ��
//	(u32*)InitData_601E_20200200,(u32*)sizeof(InitData_601E_20200200),//�¼�����ʱ��
//	(u32*)InitData_601E_20210200,(u32*)sizeof(InitData_601E_20210200),//���ݶ���ʱ��
//	(u32*)InitData_601E_20220200,(u32*)sizeof(InitData_601E_20220200),//�¼���¼���
//	(u32*)InitData_601E_20230200,(u32*)sizeof(InitData_601E_20230200),//�����¼���
//	(u32*)InitData_601E_20240200,(u32*)sizeof(InitData_601E_20240200),//�¼�����Դ
//	(u32*)InitData_601E_20250200,(u32*)sizeof(InitData_601E_20250200),//�¼���ǰֵ
		
	(u32*)InitData_601E_20260200,(u32*)sizeof(InitData_601E_20260200),//��ѹ��ƽ����
	(u32*)InitData_601E_20270200,(u32*)sizeof(InitData_601E_20270200),//������ƽ����
	(u32*)InitData_601E_20280200,(u32*)sizeof(InitData_601E_20280200),//������
	(u32*)InitData_601E_20290200,(u32*)sizeof(InitData_601E_20290200),//��ʱֵ
	(u32*)InitData_601E_202A0200,(u32*)sizeof(InitData_601E_202A0200),//Ŀ���������ַ
	(u32*)InitData_601E_202C0200,(u32*)sizeof(InitData_601E_202C0200),//����ǰ��Ǯ���ļ�
	(u32*)InitData_601E_202D0200,(u32*)sizeof(InitData_601E_202D0200),//����ǰ��͸֧���
	(u32*)InitData_601E_202E0200,(u32*)sizeof(InitData_601E_202E0200),//�ۼƹ�����
	(u32*)InitData_601E_20310200,(u32*)sizeof(InitData_601E_20310200),//�¶��õ���
	(u32*)InitData_601E_20320200,(u32*)sizeof(InitData_601E_20320200),//���ݽ����õ���
	(u32*)InitData_601E_20400200,(u32*)sizeof(InitData_601E_20400200),//��������ִ��״̬��
	(u32*)InitData_601E_20410200,(u32*)sizeof(InitData_601E_20410200),//�����������״̬��
	
//	(u32*)InitData_601E_21000200,(u32*)sizeof(InitData_601E_21000200),//��������ͳ�� ����2
//	(u32*)InitData_601E_21000300,(u32*)sizeof(InitData_601E_21000300),//��������ͳ�� ����3
//	(u32*)InitData_601E_21010200,(u32*)sizeof(InitData_601E_21010200),//Сʱ����ͳ�� ����2
//	(u32*)InitData_601E_21010300,(u32*)sizeof(InitData_601E_21010300),//Сʱ����ͳ�� ����3
//	(u32*)InitData_601E_21020200,(u32*)sizeof(InitData_601E_21020200),//������ͳ��  ����2
//	(u32*)InitData_601E_21020300,(u32*)sizeof(InitData_601E_21020300),//������ͳ��  ����3
//	(u32*)InitData_601E_21030200,(u32*)sizeof(InitData_601E_21030200),//������ͳ��  ����2
//	(u32*)InitData_601E_21030300,(u32*)sizeof(InitData_601E_21030300),//������ͳ��  ����3	
//  (u32*)InitData_601E_21040200,(u32*)sizeof(InitData_601E_21040200),//������ͳ��  ����2	
//	(u32*)InitData_601E_21040300,(u32*)sizeof(InitData_601E_21040300),//������ͳ��  ����3	
//	(u32*)InitData_601E_21100200,(u32*)sizeof(InitData_601E_21100200),//����ƽ��  ����2
//	(u32*)InitData_601E_21100300,(u32*)sizeof(InitData_601E_21100300),//����ƽ��  ����3
//	(u32*)InitData_601E_21110200,(u32*)sizeof(InitData_601E_21110200),//Сʱƽ��  ����2
//	(u32*)InitData_601E_21110300,(u32*)sizeof(InitData_601E_21110300),//Сʱƽ��  ����3
//	(u32*)InitData_601E_21120200,(u32*)sizeof(InitData_601E_21120200),//��ƽ�� ����2
//	(u32*)InitData_601E_21120300,(u32*)sizeof(InitData_601E_21120300),//��ƽ�� ����3
//	(u32*)InitData_601E_21130200,(u32*)sizeof(InitData_601E_21130200),//��ƽ�� ����2
//	(u32*)InitData_601E_21130300,(u32*)sizeof(InitData_601E_21130300),//��ƽ�� ����3	
//	(u32*)InitData_601E_21140200,(u32*)sizeof(InitData_601E_21140200),//��ƽ�� ����2
//	(u32*)InitData_601E_21140300,(u32*)sizeof(InitData_601E_21140300),//��ƽ�� ����3
//	(u32*)InitData_601E_21200200,(u32*)sizeof(InitData_601E_21200200),//���Ӽ�ֵ  ����2
//	(u32*)InitData_601E_21200300,(u32*)sizeof(InitData_601E_21200300),//���Ӽ�ֵ  ����3
//	(u32*)InitData_601E_21210200,(u32*)sizeof(InitData_601E_21210200),//Сʱ��ֵ  ����2
//	(u32*)InitData_601E_21210300,(u32*)sizeof(InitData_601E_21210300),//Сʱ��ֵ  ����3
//	(u32*)InitData_601E_21220200,(u32*)sizeof(InitData_601E_21220200),//�ռ�ֵ ����2
//	(u32*)InitData_601E_21220300,(u32*)sizeof(InitData_601E_21220300),//�ռ�ֵ ����3
//	(u32*)InitData_601E_21230200,(u32*)sizeof(InitData_601E_21230200),//�¼�ֵ ����2
//	(u32*)InitData_601E_21230300,(u32*)sizeof(InitData_601E_21230300),//�¼�ֵ ����3
//	(u32*)InitData_601E_21240200,(u32*)sizeof(InitData_601E_21240200),//�꼫ֵ ����2
//	(u32*)InitData_601E_21240300,(u32*)sizeof(InitData_601E_21240300),//�꼫ֵ ����3
		
	(u32*)InitData_601E_21300200,(u32*)sizeof(InitData_601E_21300200),//�ܵ�ѹ�ϸ���
	(u32*)InitData_601E_21310200,(u32*)sizeof(InitData_601E_21310200),//����A���ѹ�ϸ���
	(u32*)InitData_601E_21320200,(u32*)sizeof(InitData_601E_21320200),//����B���ѹ�ϸ���
	(u32*)InitData_601E_21330200,(u32*)sizeof(InitData_601E_21330200),//����C���ѹ�ϸ���
	(u32*)InitData_601E_21400200,(u32*)sizeof(InitData_601E_21400200),//������й����ʼ�����ʱ��
	(u32*)InitData_601E_21410200,(u32*)sizeof(InitData_601E_21410200),//������й����ʼ�����ʱ��

//	(u32*)InitData_601E_22000200,(u32*)sizeof(InitData_601E_22000200),//ͨѶ����
//	(u32*)InitData_601E_22030200,(u32*)sizeof(InitData_601E_22030200),//����ʱ��
//	(u32*)InitData_601E_22040200,(u32*)sizeof(InitData_601E_22040200),//��λ����
//	(u32*)InitData_601E_25000200,(u32*)sizeof(InitData_601E_25000200),//�ۼ�ˮ���ȣ�����
//	(u32*)InitData_601E_25010200,(u32*)sizeof(InitData_601E_25010200),//�ۼ�������
//	(u32*)InitData_601E_25020200,(u32*)sizeof(InitData_601E_25020200),//�ۼ�����
//	(u32*)InitData_601E_25030200,(u32*)sizeof(InitData_601E_25030200),//�ȹ���
//	(u32*)InitData_601E_25040200,(u32*)sizeof(InitData_601E_25040200),//�ۼƹ���ʱ��
//	(u32*)InitData_601E_25050200,(u32*)sizeof(InitData_601E_25050200),//ˮ��
//	(u32*)InitData_601E_25060200,(u32*)sizeof(InitData_601E_25060200),//���Ǳ�״̬ST
	(u32*)InitData_601E_40000200,(u32*)sizeof(InitData_601E_40000200),//����ʱ��
	
	
	(u32*)InitData_601E_50040200_20210200,(u32*)sizeof(InitData_601E_50040200_20210200),//1�ն�����.���ݶ���ʱ��
	(u32*)InitData_601E_50040200_00100200,(u32*)sizeof(InitData_601E_50040200_00100200),//2�ն�����.�����й�
	(u32*)InitData_601E_50040200_00200200,(u32*)sizeof(InitData_601E_50040200_00200200),//3�ն�����.�����й�
	(u32*)InitData_601E_50040200_00300200,(u32*)sizeof(InitData_601E_50040200_00300200),//4�ն�����.����޹�1
	(u32*)InitData_601E_50040200_00400200,(u32*)sizeof(InitData_601E_50040200_00400200),//5�ն�����.����޹�2
	(u32*)InitData_601E_50040200_00500200,(u32*)sizeof(InitData_601E_50040200_00500200),//6�ն�����.��һ�����޹�
	(u32*)InitData_601E_50040200_00600200,(u32*)sizeof(InitData_601E_50040200_00600200),//7�ն�����.�ڶ������޹�
	(u32*)InitData_601E_50040200_00700200,(u32*)sizeof(InitData_601E_50040200_00700200),//8�ն�����.���������޹�
	(u32*)InitData_601E_50040200_00800200,(u32*)sizeof(InitData_601E_50040200_00800200),//9�ն�����.���������޹�
	(u32*)InitData_601E_50040200_10100200,(u32*)sizeof(InitData_601E_50040200_10100200),//10�ն�����.�����й��������������ʱ��	
	(u32*)InitData_601E_50040200_10200200,(u32*)sizeof(InitData_601E_50040200_10200200),//11�ն�����.�����й��������������ʱ��	
	(u32*)InitData_601E_50040200_10300200,(u32*)sizeof(InitData_601E_50040200_10300200),//12�ն�����.����޹�1�������������ʱ��		
	(u32*)InitData_601E_50040200_10400200,(u32*)sizeof(InitData_601E_50040200_10400200),//13�ն�����.����޹�2�������������ʱ��		
	(u32*)InitData_601E_50040200_20040200,(u32*)sizeof(InitData_601E_50040200_20040200),//14�ն�����.�й�����		
	(u32*)InitData_601E_50040200_20050200,(u32*)sizeof(InitData_601E_50040200_20050200),//15�ն�����.�޹�����		
	(u32*)InitData_601E_50040200_202C0200,(u32*)sizeof(InitData_601E_50040200_202C0200),//16�ն�����.ʣ������� ����			
	(u32*)InitData_601E_50040200_202D0200,(u32*)sizeof(InitData_601E_50040200_202D0200),//17�ն�����.͸֧���			
	(u32*)InitData_601E_50040200_21310201,(u32*)sizeof(InitData_601E_50040200_21310201),//18�ն�����.A ���ѹ�ϸ���			
	(u32*)InitData_601E_50040200_21320201,(u32*)sizeof(InitData_601E_50040200_21320201),//19�ն�����.B ���ѹ�ϸ���			
	(u32*)InitData_601E_50040200_21330201,(u32*)sizeof(InitData_601E_50040200_21330201),//20�ն�����.C ���ѹ�ϸ���			
	
	
	(u32*)InitData_601E_50060200_20210200,(u32*)sizeof(InitData_601E_50060200_20210200),//1�¶�����.���ݶ���ʱ��
	(u32*)InitData_601E_50060200_00100200,(u32*)sizeof(InitData_601E_50060200_00100200),//2�¶�����.�����й�	
	(u32*)InitData_601E_50060200_00200200,(u32*)sizeof(InitData_601E_50060200_00200200),//3�¶�����.�����й�	
	(u32*)InitData_601E_50060200_00300200,(u32*)sizeof(InitData_601E_50060200_00300200),//4�¶�����.����޹�1	
	(u32*)InitData_601E_50060200_00400200,(u32*)sizeof(InitData_601E_50060200_00400200),//5�¶�����.����޹�2	
	(u32*)InitData_601E_50060200_00500200,(u32*)sizeof(InitData_601E_50060200_00500200),//6�¶�����.��һ�����޹�	
	(u32*)InitData_601E_50060200_00600200,(u32*)sizeof(InitData_601E_50060200_00600200),//7�¶�����.�ڶ������޹�	
	(u32*)InitData_601E_50060200_00700200,(u32*)sizeof(InitData_601E_50060200_00700200),//8�¶�����.���������޹�	
	(u32*)InitData_601E_50060200_00800200,(u32*)sizeof(InitData_601E_50060200_00800200),//9�¶�����.���������޹�	
	(u32*)InitData_601E_50060200_10100200,(u32*)sizeof(InitData_601E_50060200_10100200),//10�¶�����.�����й��������������ʱ��	
	(u32*)InitData_601E_50060200_10200200,(u32*)sizeof(InitData_601E_50060200_10200200),//11�¶�����.�����й��������������ʱ��	
	(u32*)InitData_601E_50060200_10300200,(u32*)sizeof(InitData_601E_50060200_10300200),//12�¶�����.����޹�1�������������ʱ��	
	(u32*)InitData_601E_50060200_10400200,(u32*)sizeof(InitData_601E_50060200_10400200),//13�¶�����.����޹�2�������������ʱ��	
	(u32*)InitData_601E_50060200_21300202,(u32*)sizeof(InitData_601E_50060200_21300202),//14�¶�����.�ܵ�ѹ�ϸ���	
	(u32*)InitData_601E_50060200_21310202,(u32*)sizeof(InitData_601E_50060200_21310202),//15�¶�����.A���ѹ�ϸ���	
	(u32*)InitData_601E_50060200_21320202,(u32*)sizeof(InitData_601E_50060200_21320202),//16�¶�����.B���ѹ�ϸ���	
	(u32*)InitData_601E_50060200_21330202,(u32*)sizeof(InitData_601E_50060200_21330202),//17�¶�����.C���ѹ�ϸ���	
	(u32*)InitData_601E_50060200_00000200,(u32*)sizeof(InitData_601E_50060200_00000200),//18�¶�����.����й�����
	

	
	
};




void DL698_SETINIT_601E(u32 Command)//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
{
	u32 i;
	u32 Len;
	u32 *p32;
	
	if(Command==6)
	{//6=�����ݲ�����ʼ��

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
		{//����>=Ԥ���ռ�
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

u32 Get_SETINIT_601E_List_NUM(void)//�õ��ɼ�������ʼ���ÿ��б����
{
	return (sizeof(SETINIT_601E_List)/8);
}

#if RUN_DL698TEST_2==1//0��Ч,1=����;����601E��Լ��
void DL698TEST_2(void)//����601E��Լ��
{
	u32 i;
	u32 Len;
	u32 *p32;
	

	if((sizeof(SETINIT_601E_List)/8)>NUMmax_601E)
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""if((sizeof(SETINIT_601E_List)/8)>NUMmax_601E)");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		while(1);
	}
	p32=(u32*)SETINIT_601E_List;
	for(i=0;i<(sizeof(SETINIT_601E_List)/8);i++)
	{
		Len=p32[1];
		if(Len>=LENper_601E)
		{//����>=Ԥ���ռ�
			DisplayStringAutoUp(0,9,(u8*)"\x0""if(Len<=LENper_601E)");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
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






















