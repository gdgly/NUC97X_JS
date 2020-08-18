
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"


#if ((Project/100)==3)//ר��
#if USER<200//ä�����Ժ�ͼ�ϵ,��׼ϵ



//#define ADDR_6000     ADDR_4204_3+LENmax_4204_3//�ɼ��������ñ�(��ʼ��ֵ0x00)
//#define LENper_6000     128
//#define NUMmax_6000     RMNmax
//#define LENmax_6000     (LENper_6000*NUMmax_6000)
const u8 InitData_6000_2_0[]=//�ɼ���������0
{
	0,0,
};
const u8 InitData_6000_2_1[]=//�ɼ���������1
{
//����2��Acquisition document definition����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	4,
//{
//�������  long-unsigned��
	DataType_long_unsigned,
	0,1,
//������Ϣ  Basic_object��
	
//��չ��Ϣ  Extended_object��
	
//������Ϣ  Annex_object
//}

//Basic_object��=structure
	DataType_structure,
	10,
//{
//ͨ�ŵ�ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x01,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	255,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
	DataType_enum,
	3,
//�˿�      OAD��
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //��������ͨ�ſں�(����)
//ͨ������  octet-string��
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//���ʸ���  unsigned��
	DataType_unsigned,
	4,
//�û�����  unsigned��
	DataType_unsigned,
	0,
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}
	DataType_enum,
	#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
	3,
	#else
	2,
	#endif
//���ѹ    long-unsigned(����-1����λV),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//�����    long-unsigned(����-1����λA)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object��=structure
	DataType_structure,
	4,
//{
//�ɼ�����ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//�ʲ���      octet-string��
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned��
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object��=array structure
	DataType_array,
	0,
//{
//������������  OAD��
//����ֵ        Data
//}
};
const u8 InitData_6000_2_2[]=//�ɼ���������2
{
//����2��Acquisition document definition����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	4,
//{
//�������  long-unsigned��
	DataType_long_unsigned,
	0,2,
//������Ϣ  Basic_object��
	
//��չ��Ϣ  Extended_object��
	
//������Ϣ  Annex_object
//}

//Basic_object��=structure
	DataType_structure,
	10,
//{
//ͨ�ŵ�ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x11,0x11,0x11,0x11,0x11,0x11,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	3,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
	DataType_enum,
	2,
//�˿�      OAD��
	DataType_OAD,
	0xF2,0x01,0x02,0x01, //RS485_1
	//0xF2,0x01,0x02,0x02, //RS485_2
//ͨ������  octet-string��
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//���ʸ���  unsigned��
	DataType_unsigned,
	4,
//�û�����  unsigned��
	DataType_unsigned,
	0,
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}
	DataType_enum,
	3,
//���ѹ    long-unsigned(����-1����λV),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//�����    long-unsigned(����-1����λA)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object��=structure
	DataType_structure,
	4,
//{
//�ɼ�����ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//�ʲ���      octet-string��
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned��
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object��=array structure
	DataType_array,
	0,
//{
//������������  OAD��
//����ֵ        Data
//}
};
const u8 InitData_6000_2_3[]=//�ɼ���������3
{
//����2��Acquisition document definition����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	4,
//{
//�������  long-unsigned��
	DataType_long_unsigned,
	0,3,
//������Ϣ  Basic_object��
	
//��չ��Ϣ  Extended_object��
	
//������Ϣ  Annex_object
//}

//Basic_object��=structure
	DataType_structure,
	10,
//{
//ͨ�ŵ�ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x03,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	3,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
	DataType_enum,
	3,
//�˿�      OAD��
	DataType_OAD,
	0xF2,0x01,0x02,0x01, //RS485_1
//ͨ������  octet-string��
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//���ʸ���  unsigned��
	DataType_unsigned,
	4,
//�û�����  unsigned��
	DataType_unsigned,
	0,
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}
	DataType_enum,
	3,
//���ѹ    long-unsigned(����-1����λV),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//�����    long-unsigned(����-1����λA)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object��=structure
	DataType_structure,
	4,
//{
//�ɼ�����ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//�ʲ���      octet-string��
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned��
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object��=array structure
	DataType_array,
	0,
//{
//������������  OAD��
//����ֵ        Data
//}
};

const u8 InitData_6000_2_4[]=//�ɼ���������4
{
	0,0,
};

const u8 InitData_6000_2_5[]=//�ɼ���������5
{
	0,0,
};

const u8 InitData_6000_2_6[]=//�ɼ���������6
{
	0,0,
};

const u8 InitData_6000_2_7[]=//�ɼ���������7
{
	0,0,
};

const u8 InitData_6000_2_8[]=//�ɼ���������8
{
	0,0,
};

const u8 InitData_6000_2_9[]=//�ɼ���������9
{
	0,0,
};

//#define ADDR_6002_8     ADDR_6000+LENmax_6000//�ѱ����
//#define LENmax_6002_8     10
const u8 InitData_6002_8[]=
{
//����8��=structure
	DataType_structure,
	4,
//{
//�Ƿ�����ÿ�������ѱ�   bool��
	DataType_bool,
	0,
//�Զ����²ɼ�����       bool��
	DataType_bool,
	0,
//�Ƿ�����ѱ�����¼�   bool��
	DataType_bool,
	0,
//����ѱ���ѡ��       enum
//{
//�����             ��0����
//ÿ�������ѱ�ǰ��� ��1����
//ÿ���ѱ�ǰ���     ��2��
//}
//}
	DataType_enum,
	0,
};

//#define ADDR_6002_9     ADDR_6002_8+LENmax_6002_8//ÿ�������ѱ��������
//#define LENmax_6002_9     (4*16)
const u8 InitData_6002_9[]=
{
//����9��ÿ�������ѱ�������ã���=array ��ʱ�ѱ����
	DataType_array,
	0,
//��ʱ�ѱ������=structure
//{
//   ��ʼʱ��         time��
//   �ѱ�ʱ����min��  long-unsigned
//}
};

//#define ADDR_6002_127     ADDR_6002_9+LENmax_6002_9//ʵʱ�����ѱ�(��ʼ��ֵ0xFF)
//#define LENmax_6002_127     3
const u8 InitData_6002_127[]=
{
//�ѱ�ʱ����=long-unsigned����λ�����ӣ���ʾ�ѱ����ʱ�䣬0��ʾ����ʱ��ֱ���ѱ������
	DataType_long_unsigned,
	0xFF,0xFF,
};

//#define ADDR_6012_2     ADDR_6002_127+LENmax_6002_127//�������ñ�(��ʼ��ֵ0x00)
//#define LENper_6012_2     128
//#define NUMmax_6012_2     16
//#define LENmax_6012_2     (LENper_6012_2*NUMmax_6012_2)
#define InitData_6012_2_0_
const u8 InitData_6012_2_0[]=//��������0
{
	0,0,
};

#define InitData_6012_2_1_
const u8 InitData_6012_2_1[]=//��������1�����ܱ�ǰ���ݣ�
{
//����2���������õ�Ԫ����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	12,
//{
//����ID    unsigned��
	DataType_unsigned,
	1,
//ִ��Ƶ��   TI��
	DataType_TI,
	3,0,1,   //1��
//��������   enum
//{
//��ͨ�ɼ����� ��1����   �¼��ɼ����� ��2����
//͸������     ��3����   �ϱ�����     ��4����
//�ű�����     ��5��
//}��
	DataType_enum,
	1,
//�������    unsigned��
	DataType_unsigned,
	1,
//��ʼʱ��    date_time_s��
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,0x01,0x01,0x00,0x00,0x00,   
//����ʱ��    date_time_s��
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,0x01,0x01,0x00,0x00,0x00, 
//��ʱ        TI��
	DataType_TI,
	1,0,2,    //2����
//ִ�����ȼ�  enum{��Ҫ��1������Ҫ��2������Ҫ��3�������ܣ�4��}��
	DataType_unsigned,
	0,
//״̬        enum{������1����ͣ�ã�2��}��
	DataType_enum,
  1,
//����ʼǰ�ű�id   long-unsigned��
	DataType_long_unsigned,
	0,0,
//������ɺ�ű�id   long-unsigned��
	DataType_long_unsigned,
	0,0,
//��������ʱ��       structure��
	DataType_structure,
	2,
//{
//   ����  enum
//{
//       ǰ�պ�    ��0����
//       ǰ�����    ��1����
//       ǰ�պ��    ��2����
//       ǰ����    ��3��
//}��
	DataType_enum,
	0,
//ʱ�α�  array ʱ��
	DataType_array,
	01,
//ʱ�Ρ�=structure
	DataType_structure,
	4,
//{
//��ʼСʱ  unsigned��
	DataType_unsigned,
	0,
//��ʼ����  unsigned��
	DataType_unsigned,
	0,
//����Сʱ  unsigned��
	DataType_unsigned,
	23,
//��������  unsigned
	DataType_unsigned,
	59,
//}
//}
//}
//����������Ϊ�ű�ʱ���������Ϊ�ű�id
};

#define InitData_6012_2_2_
const u8 InitData_6012_2_2[]=//��������2����ʷ�����ݣ�
{
//����2���������õ�Ԫ����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	12,
//{
//����ID    unsigned��
	DataType_unsigned,
	2,
//ִ��Ƶ��   TI��
	DataType_TI,
	1,0,5, //5����
//��������   enum
//{
//��ͨ�ɼ����� ��1����   �¼��ɼ����� ��2����
//͸������     ��3����   �ϱ�����     ��4����
//�ű�����     ��5��
//}��
	DataType_enum,
	1,
//�������    unsigned��
	DataType_unsigned,
	2,
//��ʼʱ��    date_time_s��
	DataType_date_time_s,
 (2000>>8)&0xff,2000&0xff,0x01,0x01,0x00,0x00,0x00,
//����ʱ��    date_time_s��
	DataType_date_time_s,
 (2099>>8)&0xff,2099&0xff,0x01,0x01,0x00,0x00,0x00,
//��ʱ        TI��
	DataType_TI,
	0,0,0,
//ִ�����ȼ�  enum{��Ҫ��1������Ҫ��2������Ҫ��3�������ܣ�4��}��
	DataType_unsigned,
	0,
//״̬        enum{������1����ͣ�ã�2��}��
	DataType_enum,
  1,
//����ʼǰ�ű�id   long-unsigned��
	DataType_long_unsigned,
	0,0,
//������ɺ�ű�id   long-unsigned��
	DataType_long_unsigned,
	0,0,
//��������ʱ��       structure��
	DataType_structure,
	2,
//{
//   ����  enum
//{
//       ǰ�պ�    ��0����
//       ǰ�����    ��1����
//       ǰ�պ��    ��2����
//       ǰ����    ��3��
//}��
	DataType_enum,
	0,
//ʱ�α�  array ʱ��
	DataType_array,
	01,
//ʱ�Ρ�=structure
	DataType_structure,
	4,
//{
//��ʼСʱ  unsigned��
	DataType_unsigned,
	0,
//��ʼ����  unsigned��
	DataType_unsigned,
	0,
//����Сʱ  unsigned��
	DataType_unsigned,
	23,
//��������  unsigned
	DataType_unsigned,
	59,
//}
//}
//}
//����������Ϊ�ű�ʱ���������Ϊ�ű�id
};

//#define InitData_6012_2_3_
//const u8 InitData_6012_2_3[]=//��������3�������������ݣ�
//{
////����2���������õ�Ԫ����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//	DataType_structure,
//	12,
////{
////����ID    unsigned��
//	DataType_unsigned,
//	3,
////ִ��Ƶ��   TI��
//	DataType_TI,
//	3,0,1,
////��������   enum
////{
////��ͨ�ɼ����� ��1����   �¼��ɼ����� ��2����
////͸������     ��3����   �ϱ�����     ��4����
////�ű�����     ��5��
////}��
//	DataType_enum,
//	1,
////�������    unsigned��
//	DataType_unsigned,
//	3,
////��ʼʱ��    date_time_s��
//	DataType_date_time_s,
// 0x07,0xE0,0x09,0x0C,0x00,0x02,0x00,
////����ʱ��    date_time_s��
//	DataType_date_time_s,
// 0x08,0x33,0x09,0x09,0x09,0x09,0x09,
////��ʱ        TI��
//	DataType_TI,
//	0,0,0,
////ִ�����ȼ�  enum{��Ҫ��1������Ҫ��2������Ҫ��3�������ܣ�4��}��
//	DataType_unsigned,
//	3,
////״̬        enum{������1����ͣ�ã�2��}��
//	DataType_enum,
//  1,
////����ʼǰ�ű�id   long-unsigned��
//	DataType_long_unsigned,
//	0,0,
////������ɺ�ű�id   long-unsigned��
//	DataType_long_unsigned,
//	0,0,
////��������ʱ��       structure��
//	DataType_structure,
//	2,
////{
////   ����  enum
////{
////       ǰ�պ�    ��0����
////       ǰ�����    ��1����
////       ǰ�պ��    ��2����
////       ǰ����    ��3��
////}��
//	DataType_enum,
//	0,
////ʱ�α�  array ʱ��
//	DataType_array,
//	01,
////ʱ�Ρ�=structure
//	DataType_structure,
//	4,
////{
////��ʼСʱ  unsigned��
//	DataType_unsigned,
//	0,
////��ʼ����  unsigned��
//	DataType_unsigned,
//	0,
////����Сʱ  unsigned��
//	DataType_unsigned,
//	0x17,
////��������  unsigned
//	DataType_unsigned,
//	0x3B,
////}
////}
////}
////����������Ϊ�ű�ʱ���������Ϊ�ű�id
//};

//#define ADDR_6014     ADDR_6012_2+LENmax_6012_2//��ͨ�ɼ�����
//#define LENper_6014     128
//#define NUMmax_6014     16
//#define LENmax_6014     (LENper_6014*NUMmax_6014)
#define InitData_6014_2_0_
const u8 InitData_6014_2_0[]=
{
	0,0,
};
#define InitData_6014_2_1_
const u8 InitData_6014_2_1[]=//���ܱ�ǰ����
{
//����2����ͨ�ɼ���������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	6,
//{
//�������  unsigned��
	DataType_unsigned,
	1,
//�洢���  long-unsigned��
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//�ɼ���ʽ  structure
	DataType_structure,
	2,
//{
//  �ɼ�����  unsigned��
	DataType_unsigned,
	2,
//  �ɼ�����  Data
	DataType_NULL,
//}��
//��¼��ѡ��  array CSD��
	DataType_array,
	01,
	DataType_CSD,
	0x01,0x50,0x04,0x02,0x00,
	2,
	0x00,0x10,0x02,0x00,
	0x00,0x20,0x02,0x00,
//���ܱ���  MS��
	DataType_MS,
	0x01,	
//�洢ʱ��ѡ�� enum
//{
//δ����              ��0����
//����ʼʱ��        ��1����
//��Ե���0��0��    ��2����
//�������23��59��  ��3����
//�������0��0��    ��4����
//��Ե���1��0��0�֣�5����
//���ݶ���ʱ��        ��6��
//}
	DataType_enum,
	4,
//}
//�ɼ���ʽ���ݸ�ʽ���£�
//�ɼ�����	 �ɼ�����	 ��ʾ
//0	       NULL	     �ɼ���ǰ����
//1	       unsigned	 �ɼ��ϵ�N��
//2	NULL	������ʱ��ɼ�
//3	TI	��ʱ�����ɼ�
};
#define InitData_6014_2_2_
const u8 InitData_6014_2_2[]=//��ʷ������
{
//����2����ͨ�ɼ���������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	6,
//{
//�������  unsigned��
	DataType_unsigned,
	2,
//�洢���  long-unsigned��
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//�ɼ���ʽ  structure
	DataType_structure,
	2,
//{
//  �ɼ�����  unsigned��
	DataType_unsigned,
	0,
//  �ɼ�����  Data
	DataType_NULL,
//}��
//��¼��ѡ��  array CSD��
	DataType_array,
	5,
	DataType_CSD,
	0x00,0x00,0x10,0x02,0x00,//�����й�
	DataType_CSD,
	0x00,0x00,0x20,0x02,0x00,//�����й�
	DataType_CSD,
	0x00,0x20,0x00,0x02,0x00,//��ѹ
	DataType_CSD,
	0x00,0x20,0x01,0x02,0x00,//����
	DataType_CSD,
	0x00,0x20,0x04,0x02,0x00,//�й�����
//���ܱ���  MS��
	DataType_MS,
	0x01,	
//�洢ʱ��ѡ�� enum
//{
//δ����              ��0����
//����ʼʱ��        ��1����
//��Ե���0��0��    ��2����
//�������23��59��  ��3����
//�������0��0��    ��4����
//��Ե���1��0��0�֣�5����
//���ݶ���ʱ��        ��6��
//}
	DataType_enum,
	0,
//}
//�ɼ���ʽ���ݸ�ʽ���£�
//�ɼ�����	 �ɼ�����	 ��ʾ
//0	       NULL	     �ɼ���ǰ����
//1	       unsigned	 �ɼ��ϵ�N��
//2	NULL	������ʱ��ɼ�
//3	TI	��ʱ�����ɼ�
};

/*
#define InitData_6014_2_3_
const u8 InitData_6014_2_3[]=//������������
{
//����2����ͨ�ɼ���������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	6,
//{
//�������  unsigned��
	DataType_unsigned,
	3,
//�洢���  long-unsigned��
	DataType_long_unsigned,
	0,96,
//�ɼ���ʽ  structure
	DataType_structure,
	2,
//{
//  �ɼ�����  unsigned��
	DataType_unsigned,
	3,
//  �ɼ�����  Data
	DataType_TI,
	1,0,15,
//}��
//��¼��ѡ��  array CSD��
	DataType_array,
	1,
	DataType_CSD,
	0x01,0x50,0x02,0x02,0x00,
	3,
	0x20,0x21,0x02,0x00,
	0x00,0x10,0x02,0x00,
	0x00,0x20,0x02,0x00,
//���ܱ���  MS��
	DataType_MS,
	0x01,	
//�洢ʱ��ѡ�� enum
//{
//δ����              ��0����
//����ʼʱ��        ��1����
//��Ե���0��0��    ��2����
//�������23��59��  ��3����
//�������0��0��    ��4����
//��Ե���1��0��0�֣�5����
//���ݶ���ʱ��        ��6��
//}
	DataType_enum,
	4,
//}
//�ɼ���ʽ���ݸ�ʽ���£�
//�ɼ�����	 �ɼ�����	 ��ʾ
//0	       NULL	     �ɼ���ǰ����
//1	       unsigned	 �ɼ��ϵ�N��
//2	NULL	������ʱ��ɼ�
//3	TI	��ʱ�����ɼ�
};
*/

//#define ADDR_6016     ADDR_6014+LENmax_6014//�¼��ɼ�������(��ʼ��ֵ0x00)
////����2���¼��ɼ�����Event acq plan����=structure
////{
////�������        unsigned��
////�ɼ���ʽ        structure
////{
////  �ɼ�����      unsigned��
////  �ɼ�����      Data
////}
////���ܱ���      MS��
////�ϱ���ʶ        bool��True�������ϱ���False�����ϱ�����
////�洢���        long-unsigned
////}
////�ɼ���ʽ���ݸ�ʽ���£�
////�ɼ�����	�ɼ�����	��ʾ
////0	array ROAD	���ڲɼ��¼�����
////1	NULL	����֪ͨ�ɼ������¼�����
////2	array ROAD	����֪ͨ�ɼ�ָ���¼�����
////3 NULL  ֱ�Ӵ洢֪ͨ�¼�����
//#define LENper_6016     1024
//#define NUMmax_6016     64
//#define LENmax_6016     (LENper_6016*NUMmax_6016)

//#define ADDR_601C     ADDR_6016+LENmax_6016//�ϱ�������(��ʼ��ֵ0x00)
////����2���ϱ����� report plan����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
////{
////�������          unsigned��
////�ϱ�ͨ��          array OAD��
////�ϱ���Ӧ��ʱʱ��  TI��
////����ϱ�����      unsigned��
////�ϱ�����          strcuture
////{
////  ����  unsigned��
////  ����  Data
////}
////}
////�ϱ����ݣ�
////�ϱ�����	  �ϱ�����	��ʾ
//// 0	      OAD	������������
//// 1	      RecordData	�ϱ���¼�Ͷ�������
////RecordData��=structure
////{
////  ����������������      OAD��
////��¼�Ͷ�������������  RCSD��
////��ѡ��                RSD
////}
//#define LENper_601C     1024
//#define NUMmax_601C     16
//#define LENmax_601C     (LENper_601C*NUMmax_601C)

const u8 InitData_601C_2_0[]=
{
	0,0,
};


//#define ADDR_601E     ADDR_601C+LENmax_601C//�ɼ������(��ʼ��ֵ0x00)
////����2��=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
////{
////������ѡ��������  CSD��
////��������  structrue
////{
////AcqCmd_2007  structure
////{
////����DI  array octet-string(SIZE(4))��
////����DI  array octet-string(SIZE(4))
////}��
////AcqCmd_1997  structure
////{
////����DI  array octet-string(SIZE(2))��
////����DI  array octet-string(SIZE(2))
////}��
////AcqCmd_Trans  structure
////{
////Frame  octet-string
////}
////}
////}
//#define LENper_601E     256
//#define NUMmax_601E     256
//#define LENmax_601E     (LENper_601E*NUMmax_601E)


//#define ADDR_8000_2     ADDR_601E+LENmax_601E//ң��-����2�����ò���)
//#define LENmax_8000_2     10
const u8 InitData_8000_2[]=
{
//����2�����ò���)��=structure
//{
//  �̵�����բ��������ֵ    double-long-unsigned����λ��A������-4����
//  ���������ޱ�����ʱʱ��  long-unsigned����λ�����ӣ�����0��
//}
	0,0,
};

//#define ADDR_8000_127     ADDR_8000_2+LENmax_8000_2//ң��-����127�������澯��������
//#define LENmax_8000_127     1
const u8 InitData_8000_127[]=
{
//����127�������澯��������������=NULL
//����128�����������������������=NULL
//����=0x55,���=0x0
	0,
};

//#define ADDR_8000_129     ADDR_8000_127+LENmax_8000_127//ң��-����129����բ��������
//#define LENper_8000_129     16
//#define NUMmax_8000_129     4
//#define LENmax_8000_129     (LENper_8000_129*NUMmax_8000_129)
const u8 InitData_8000_129[]=
{
//����129����բ��������
//������=array structure
//{
//�̵���      OAD��
//�澯��ʱ    unsigned����λ�����ӣ����㣺0����
//�޵�ʱ��    long-unsigned����λ�����ӣ����㣺0��ֵΪ0��ʾ�����޵磩��
//�Զ���բ    bool��True���Զ���բ��False�����Զ���բ��
//}
	0,0,
};

//#define ADDR_8000_130     ADDR_8000_129+LENmax_8000_129//ң��-����130����բ��������
//#define LENper_8000_130     12
//#define NUMmax_8000_130     4
//#define LENmax_8000_130     (LENper_8000_130*NUMmax_8000_130)
const u8 InitData_8000_130[]=
{
//����130����բ��������
//������=array structure
//{
//�̵���    OAD��
//����      enum{��բ����0����ֱ�Ӻ�բ��1��}
//}
	0,0,
};

//#define ADDR_8001_127     ADDR_8000_130+LENmax_8000_130//����127��Ͷ�뱣��
//#define LENmax_8001_127     1
const u8 InitData_8001_127[]=
{
//����127��Ͷ�뱣�磨������������=NULL
//����Ͷ�뱣��״̬������ֹһ��ִ����բ�ļ̵���������ƣ��һָ�����բ�ļ̵���������ơ�
//����128��������磨������������=NULL
//���ڽ������״̬��
//Ͷ��=0x55,���=0x0
	0,
};

//#define ADDR_8001_3     ADDR_8000_130+LENmax_8000_130//����-��������վ�����ͨ��ʱ�������ӣ�
//#define LENmax_8001_3     3
const u8 InitData_8001_3[]=
{
//����3��=long-unsigned
//��������վ�����ͨ��ʱ�������ӣ���0��ʾ���Զ����硣
	DataType_long_unsigned,
	0,0,
};

//#define ADDR_8001_4     ADDR_8001_3+LENmax_8001_3//����-�ϵ��Զ�����ʱ�������ӣ�
//#define LENmax_8001_4     3
const u8 InitData_8001_4[]=
{
//����4��=long-unsigned
//�ϵ��Զ�����ʱ�������ӣ���0��ʾ�ϵ粻�Զ����硣
	DataType_long_unsigned,
	0,0,
};

//#define ADDR_8001_5     ADDR_8001_4+LENmax_8001_4//����-�Զ�����ʱ��
//#define LENper_8001_5     8
//#define NUMmax_8001_5     24
//#define LENmax_8001_5     (LENper_8001_5*NUMmax_8001_5)
const u8 InitData_8001_5[]=
{
//����5��=array �Զ�����ʱ��
//�Զ�����ʱ�Ρ�=structure
//{
//��ʼʱ�䣨ʱ��  unsigned��
//����ʱ�䣨ʱ��  unsigned
//}
//ʱ����������Ϊǰ�պ󿪡�
	0,0,
};

//#define ADDR_8001_129     ADDR_8001_127+LENmax_8001_127//����129������Զ�����
////����129������Զ����磨������������=NULL
////���ڽ���Զ�����״̬��
////����Զ�����=0x55,�Զ�����=0x0
//#define LENmax_8001_129     1

//#define ADDR_8002_127     ADDR_8001_129+LENmax_8001_129//����127���߷Ѹ澯Ͷ��
//#define LENmax_8002_127     256
const u8 InitData_8002_127[]=
{
//����127���߷Ѹ澯Ͷ�루������
//������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//  �澯ʱ�Σ�octet-string(SIZE(3))��
//  �澯��Ϣ��visible-string(SIZE(200))
//}
	0,0,
//����128��ȡ���߷Ѹ澯��������������=NULL
};

//#define ADDR_8100_2     ADDR_8002_127+LENmax_8002_127//�ն˱�����ֵ
//#define LENmax_8100_2     9
const u8 InitData_8100_2[]=
{
//�ն˱�����ֵ��=long64����λ��W�����㣺-1��
	DataType_long64,
  0,0,0,0,0,(1000000>>16)&0xff,(1000000>>8)&0xff,1000000&0xff,//������ֵ 100kW	
	
};

//#define ADDR_8101_2     ADDR_8100_2+LENmax_8100_2//�ն˹���ʱ��
//#define LENmax_8101_2     16
const u8 InitData_8101_2[]=
{
//����2�����ò�������=array unsigned
	DataType_array,
	12,
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
	DataType_unsigned,
	0x55,//����1
};

//#define ADDR_8102_2     ADDR_8101_2+LENmax_8101_2//���ظ澯ʱ��
//#define LENmax_8102_2     16
const u8 InitData_8102_2[]=
{
//����2�����ò�������=array unsigned
	DataType_array,
	2,
	DataType_unsigned,
	1,
	DataType_unsigned,
	1,
};

//#define ADDR_8103_2     ADDR_8102_2+LENmax_8102_2//ʱ�ι���-���Ʒ�����(��ʼ��ֵ0x00)
//#define LENper_8103_2     241//ʵ����
//#define NUMmax_8103_2     TAGmax
//#define LENmax_8103_2     (2+( LENper_8103_2*NUMmax_8103_2))
//#if LENmax_8103_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8103_2[]=
{
//����2�����Ʒ���������=array ʱ�ι������õ�Ԫ
	DataType_array,
	1,
//����2��=structure//
	DataType_structure,
	6,
//{
//�ܼ������  OI��
	DataType_OI,
	0x23,0x01,
//������ʶ    bit-string(SIZE(8))��
	DataType_bit_string,
	8,
	0xE0,
//��һ�׶�ֵ  PowerCtrlParam��
	DataType_structure,
	9,
	DataType_bit_string,
	8,
	0xFF,
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
	DataType_long64,
	0,0,0,0,(30000000>>24)&0xff,(30000000>>16)&0xff,(30000000>>8)&0xff,30000000&0xff,//3000kW	
//�ڶ��׶�ֵ  PowerCtrlParam��
	DataType_structure,
	9,
	DataType_bit_string,
	8,
	0xFF,
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
	DataType_long64,
	0,0,0,0,(70000000>>24)&0xff,(70000000>>16)&0xff,(70000000>>8)&0xff,70000000&0xff,//7000kW	
//�����׶�ֵ  PowerCtrlParam��
	DataType_structure,
	9,
	DataType_bit_string,
	8,
	0xFF,
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
	DataType_long64,
	0,0,0,0,(80000000>>24)&0xff,(80000000>>16)&0xff,(80000000>>8)&0xff,80000000&0xff,//8000kW	
//ʱ�ι��ض�ֵ����ϵ��  integer����λ��%��
	DataType_integer,
	0,
//}
//PowerCtrlParam��=structure
//{
//	ʱ�κ�          bit-string(SIZE(8))��
//	ʱ��1���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��2���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��3���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��4���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��5���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��6���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��7���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��8���ض�ֵ  long64����λ��W�����㣺-1��
//}
};

//#define ADDR_8103_127     ADDR_8103_2+LENmax_8103_2//ʱ�ι���-�����л�(��ʼ��ֵ0x00)
//#define LENper_8103_127     12//ʵ����
//#define LENmax_8103_127     (2+(LENper_8103_127*NUMmax_TAG))
const u8 InitData_8103_127[]=
{
//ʱ�ι��ط����л���=structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//ʱ�ι���Ͷ���ʶ    bit-string(SIZE(8))��
//ʱ�ι��ض�ֵ������  unsigned
//}
//ʱ�ι���Ͷ���ʶ��D0��D7��˳���λ��ʾ��1����8ʱ�Σ��á�1������Ч���á�0������Ч��
//ʱ�ι��ض�ֵ�����ţ���ֵ��Χ��0��2���α�ʾ��1����3�׷���������ֵ��Ч��
	0,0,
};

//#define ADDR_8104_2     ADDR_8103_127+LENmax_8103_127//���ݿ����õ�Ԫ
//#define LENper_8104_2     28//ʵ����
//#define NUMmax_8104_2     TAGmax
//#define LENmax_8104_2     (2+(LENper_8104_2*NUMmax_8104_2))
//#if LENmax_8104_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8104_2[]=
{
//array
//����2��=structure
//{
//�ܼ������    OI��
//���ݿض�ֵ    long64����λ��W�����㣺-1����
//�޵���ʼʱ��  date_time_s����=FFFFH����=FFH����=FFH����
//�޵�����ʱ��  long-unsigned����λ�����ӣ���
//ÿ���޵���    bit-string(SIZE(8)) 
//}
//ÿ���޵��գ�D1��D7��ʾ����һ�������գ�D0=0��
	0,0,
};

//#define ADDR_8105_2     ADDR_8104_3+LENmax_8104_3//Ӫҵ��ͣ�����õ�Ԫ(��ʼ��ֵ0x00)
//#define LENper_8105_2     30//ʵ����
//#define NUMmax_8105_2     TAGmax
//#define LENmax_8105_2     (2+(LENper_8105_2*NUMmax_8105_2))
//#if LENmax_8105_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8105_2[]=
{
//array
//����2��=structure
//{
//�ܼ������      OI��
//��ͣ��ʼʱ��    date_time_s��ʱ=FFH����=FFH����
//��ͣ����ʱ��    date_time_s��ʱ=FFH����=FFH����
//��ͣ�ع��ʶ�ֵ  long64����λ��W�����㣺-1��
//}
	0,0,
};

//#define ADDR_8106_127     ADDR_8105_3+LENmax_8105_3//��ǰ�����¸���-����127 Ͷ�루�ܼ�����󣬿��Ʒ�����(��ʼ��ֵ0x00)
//#define LENper_8106_127     23//ʵ����
//#define NUMmax_8106_127     TAGmax
//#define LENmax_8106_127     (2+(LENper_8106_127*NUMmax_8106_127))
//#if LENmax_8106_127>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8106_127[]=
{
//array
//structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//��ǰ�����¸��ض�ֵ����ʱ��    unsigned����λ�����ӣ���
//��ǰ�����¸��ض�ֵ����ϵ��    integer����λ��%����
//�غ��ܼ��й����ʶ�����ʱʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��صĿ���ʱ��      unsigned����λ��0.5Сʱ����
//��ǰ�����¸��ص�1�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�2�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�3�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�4�ָ澯ʱ��  unsigned����λ�����ӣ�
////}
	0,0,
};

//#define ADDR_8107_2     ADDR_8106_3+LENmax_8106_3//��������õ�Ԫ
//#define LENper_8107_2     43//ʵ����
//#define NUMmax_8107_2     TAGmax
//#define LENmax_8107_2     (2+(LENper_8107_2*NUMmax_8107_2))
//#if LENmax_8107_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8107_2[]=
{
//array
//����2��=structure
//{
//	�ܼ������      OI��
//���絥��        double-long-unsigned��
//׷��/ˢ�±�ʶ   enum{׷�ӣ�0����ˢ�£�1��}��
//��������        enum{������0������ѣ�1��}��
//���������ѣ�ֵ  long64����λ��kWh/Ԫ�� ���㣺-4����
//	��������ֵ      long64����λ��kWh/Ԫ�����㣺-4����
//	��բ����ֵ      long64����λ��kWh/Ԫ�����㣺-4��
//	�����ģʽ      enum{����ģʽ��0����Զ��ģʽ��1��}
//}
	0,0,
};

//#define ADDR_8108_2     ADDR_8107_3+LENmax_8107_3//�µ�����õ�Ԫ
//#define LENper_8108_2     18//ʵ����
//#define NUMmax_8108_2     TAGmax
//#define LENmax_8108_2     (2+(LENper_8108_2*NUMmax_8108_2))
//#if LENmax_8108_2>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_8108_2[]=
{
//array
//����2��=structure
//{
//	�ܼ������      OI��
//	�µ����ض�ֵ    long64����λ��kWh�����㣺-4����
//	��������ֵϵ��  unsigned����λ��%����
//	�µ����ض�ֵ����ϵ��  integer����λ��%��
//}
	0,0,
};

//#define ADDR_8103_3     ADDR_8103_2+LENmax_8103_2//ʱ�ι���-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//#define LENper_8103_3     7//ʵ����
//#define LENmax_8103_3     (2+(LENper_8103_3*NUMmax_8103_2))
const u8 InitData_8103_3[]=
{
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
	DataType_array,
	NUMmax_TAG,
#if NUMmax_TAG>=1
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x01,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=2
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x02,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=3
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x03,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=4
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x04,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=5
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x05,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=6
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x06,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=7
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x07,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
#if NUMmax_TAG>=8
//һ���ܼ������Ͷ��״̬��=structure
	DataType_structure,
	2,
//{
//  �ܼ������  OI��
	DataType_OI,
	0x23,0x08,
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
	DataType_enum,
	0,
#endif
//}
};

//#define ADDR_8104_3     ADDR_8104_2+LENmax_8104_2//���ݿ�-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//#define LENper_8104_3     7//ʵ����
//#define LENmax_8104_3     (2+(LENper_8104_3*NUMmax_8104_2))
const u8 InitData_8104_3[]=
{
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
	0,0,
};

//#define ADDR_8105_3     ADDR_8105_2+LENmax_8105_2//Ӫҵ��ͣ��-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//#define LENper_8105_3     7//ʵ����
//#define LENmax_8105_3     (2+(LENper_8105_3*NUMmax_8105_2))
const u8 InitData_8105_3[]=
{
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
	0,0,
};

//#define ADDR_8106_3     ADDR_8106_127+LENmax_8106_127//��ǰ�����¸���-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//#define LENper_8106_3     7//ʵ����
//#define LENmax_8106_3     (2+(LENper_8106_3*NUMmax_8106_127))
const u8 InitData_8106_3[]=
{
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
	0,0,
};

//#define ADDR_8107_3     ADDR_8107_2+LENmax_8107_2//�����-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//#define LENper_8107_3     7//ʵ����
//#define LENmax_8107_3     (2+(LENper_8107_3*NUMmax_8107_2))
const u8 InitData_8107_3[]=
{
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
////}
	0,0,
};

//#define ADDR_8108_3     ADDR_8108_2+LENmax_8108_2//�µ��-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//#define LENper_8108_3     7//ʵ����
//#define LENmax_8108_3     (2+(LENper_8108_3*NUMmax_8108_2))
const u8 InitData_8108_3[]=
{
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
////}
	0,0,
};


//#define ADDR_F002_4     ADDR_F001_7+LENmax_F001_7
//#define LENmax_F002_4     64
const u8 InitData_F002_4[]=
{
//����4����������Ϣ����=structure
	DataType_structure,
	4,
//{
//IP��ַ  octet-string��
	DataType_octet_string,4,
	192,168,1,100,
//�˿�    long-unsigned��
	DataType_long_unsigned,
	0,31,
//�û���  visible-string��
	DataType_visible_string,4,
	'c','a','r','d',
//����    visible-string
	DataType_visible_string,4,
	'c','a','r','d',
//}
};

//#define ADDR_F101_2     ADDR_8108_3+LENmax_8108_3//��ȫģʽ����(��ʼ��ֵ0x00)
//#define LENmax_F101_2     2
const u8 InitData_F101_2[]=
{
//����2����ȫģʽѡ�񣩡�=enum
//{
//�����ð�ȫģʽ������0����
//���ð�ȫģʽ����  ��1��
//}
//��ȫģʽѡ�����壺0�������ð�ȫģʽ������Ĭ�ϰ�ȫģʽ������1�����ð�ȫģʽ������Ĭ�ϰ�ȫģʽ������
	DataType_enum,
	0,
};

//#define ADDR_F101_3     ADDR_F101_2+LENmax_F101_2//��ʽ��ȫģʽ����(��ʼ��ֵ0x00)
//#define LENper_F101_3     8//ʵ����
//#define NUMmax_F101_3     127
//#define LENmax_F101_3     (4+(LENper_F101_3*NUMmax_F101_3))
//#if LENmax_F101_3>(LEN_128KDATABUFF/2)
//#error
//#endif
const u8 InitData_F101_3[]=
{
//����3����ʽ��ȫģʽ��������=array ��ȫģʽ����
	DataType_array,
	0,
//��ȫģʽ������=structure
//{
//  �����ʶ   OI��
//  ��ȫģʽ   long-unsigned
//}
//����3Ϊ��ʽ��ȫģʽ����������ֵ�����������ȫ�Բ�������3�У���Ĭ�ϰ�ȫģʽ���������ͬһ����ȫ���ڰ�ȫģʽ������Ĭ�ϰ�ȫģʽ���о���˵��������ʽ��ȫģʽ����ִ�С���ȫģʽ������Ĭ�ϰ�ȫ�������嶨�����¼F����ȫģʽ�������¼F�б�F.1��
};

//#define ADDR_F200_2     ADDR_F101_3+LENmax_F101_3//RS232
//#define LENper_F200_2     15
//#define LENmax_F200_2     LENper_F200_2
const u8 InitData_F200_2[]=
{
//�˿ڡ�=structure
	DataType_structure,
	3,
//{
//�˿ں�     OAD
	DataType_OAD,
	0xF2,0x00,0x02,0x01,
//�˿ڲ���   COMDCB��
	DataType_COMDCB,
	0x06,0x02,0x08,0x01,0x00,
//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
	DataType_enum,
	0,
//}
};

//#define ADDR_F201_2     ADDR_F200_2+LENmax_F200_2//RS485
//#define LENper_F201_2     15
//#define NUMmax_F201_2     3
//#define LENmax_F201_2     (LENper_F201_2*NUMmax_F201_2)
const u8 InitData_F201_2[]=
{
//�˿ڡ�=structure
	DataType_structure,
	3,
//{
//�˿ں�     OAD
	DataType_OAD,
	0xF2,0x01,0x02,0x01,
//�˿ڲ���   COMDCB��
	DataType_COMDCB,
	0x03,0x02,0x08,0x01,0x00,
//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
	DataType_enum,
	1,
//}
};

//#define ADDR_F202_2     ADDR_F201_2+LENmax_F201_2//����
//#define LENper_F202_2     15
//#define LENmax_F202_2     LENper_F202_2
const u8 InitData_F202_2[]=
{
//�˿ڡ�=structure
	DataType_structure,
	2,
//{
//�˿ں�     OAD
	DataType_OAD,
	0xF2,0x02,0x02,0x01,
//�˿ڲ���   COMDCB��
	DataType_COMDCB,
	0x02,0x02,0x08,0x01,0x00,
//}
};

//#define ADDR_F203_4     ADDR_F202_2+LENmax_F202_2
const u8 InitData_F203_4[]=
{
//����4��=structure
	DataType_structure,
	2,
//{
//�����������־bit-string(SIZE��8��)
//��
//bit0��bit7��˳���λ��ʾ��1��8·״̬�����룬�á�1�������룬�á�0����δ���롣
//����
	DataType_bit_string,
	8,
	0xff,
//���������Ա�־bit-string(SIZE��8��)
//��
//  bit0��bit7��˳���λ��ʾ��1��8·״̬�����룬�á�1���������㡣�á�0�������մ��㡣
//��
	DataType_bit_string,
	8,
	0xff,
//}
};
//#define LENmax_F203_4     8

//#define ADDR_F204_4     ADDR_F203_4+LENmax_F203_4
////����4��=array ֱ��ģ��������
////ֱ��ģ�������á�=structure
////{
////  ������ʼֵ  double-long��
////  ���̽���ֵ  double-long��
////  ���㼰��λ  Scaler_Unit
////}
//#define LENper_F204_4     15
//#define NUMmax_F204_4     DCmax
//#define LENmax_F204_4     (2+(LENper_F204_4*NUMmax_F204_4))
//#define ADDR_F205_127     ADDR_F204_4+LENmax_F204_4
////����127���޸Ŀ������ԣ��̵����ţ��������ԣ�
////��=structure
////{
////�̵����š�=OAD
////�������ԡ�=enum{����ʽ��0��������ʽ��1��}��
////}
//#define LENper_F205_127     9
//#define NUMmax_F205_127     CRmax
//#define LENmax_F205_127     (LENper_F205_127*NUMmax_F205_127)

//#define ADDR_F206_4     ADDR_F205_127+LENmax_F205_127
//#define LENper_F206_4     10
//#define NUMmax_F206_4     12
//#define LENmax_F206_4     (2+(LENper_F206_4*NUMmax_F206_4))
const u8 InitData_F206_4[]=
{
//����4��=array ����澯ʱ��
	DataType_array,
	1,
//����澯ʱ�Ρ�=structure
	DataType_structure,
	2,
//{
//  ��ʼʱ��  Time��
	DataType_time,
	0,0,0,
//  ����ʱ��  Time
	DataType_time,
	23,59,59,
//}
};

//#define ADDR_F207_127     ADDR_F206_4+LENmax_F206_4
////����127���޸Ĺ���ģʽ��·�ţ�����ģʽ��
////��=structure
////{
////·�š�=OAD
////���ӹ��ܡ�=enum
////{
////     �����������0����
////     ��������  ��1����
////     ʱ��Ͷ��  ��2��
////}
////}
//#define LENper_F207_127     9
//#define NUMmax_F207_127     1
//#define LENmax_F207_127     (LENper_F207_127*NUMmax_F207_127)

//#define ADDR_F209_128     ADDR_F207_127+LENmax_F207_127
//#define LENper_F209_128     13
//#define NUMmax_F209_128     1
//#define LENmax_F209_128     (LENper_F209_128*NUMmax_F209_128)
const u8 InitData_F209_2[]=
{
//����128�����ö˿ڲ������˿ںţ�ͨ�Ų�����
//��=structure
	DataType_structure,
	2,
//{
//�˿ںš�=OAD
	DataType_OAD,
	0xF2,0x09,0x80,0x01,
//�˿ڲ���   COMDCB��
	DataType_COMDCB,
	0x03,0x02,0x08,0x01,0x00,
//}
};
//#define ADDR_F209_6     ADDR_F207_127+LENmax_F207_127//�ز�/΢�������߽ӿ�
//����6���ӽڵ�����б�������ڣ���= TI
//�������ڣ�ָ�ӽڵ�����б�����������Ϣ��������Ϣ�ȸ�������
//#define LENmax_F209_6     4
const u8 InitData_F209_6[]=
{
//TI��=SEQUENCE
	DataType_TI,
//{
//   ��λ  ENUMERATED
//   {
//       ��      ��0����
//       ��      ��1����
//       ʱ      ��2����
//       ��      ��3����
//       ��      ��4����
//       ��      ��5��
//    }��
	3,//��
//    ���ֵ  long-unsigned
	1>>8,1&0xff,
//}
};
//#define ADDR_F209_9     ADDR_F209_6+LENmax_F209_6//�ز�/΢�������߽ӿ�
//���� 9������ز�Ƶ����ţ��� =unsigned
//#define LENmax_F209_9     2
const u8 InitData_F209_9[]=
{
	DataType_unsigned,
	0,//����ز�Ƶ�Σ�0��1.953��11.96MHz��1�� 2.441��5.615 MHz
};

//#define ADDR_F20B_127     ADDR_F209_128+LENmax_F209_128
////����127�����ö˿ڣ��˿ںţ�ͨ�Ų�����
////��=structure
////{
////�˿ںš�=OAD
////�˿ڲ���   COMDCB��
////}
//#define LENper_F20B_127     13
//#define NUMmax_F20B_127     1
//#define LENmax_F20B_127     (LENper_F20B_127*NUMmax_F20B_127)

//�ܼ���
//#define ADDR_230x_2     ADDR_F002_8+LENmax_F002_8
//#define LENper_230x_2     (2+(15*8))
//#define LENmax_230x_2     (LENper_230x_2*NUMmax_TAG)
const u8 InitData_230x_2[]=
{
#if NUMmax_TAG<2
#error
#endif	
//�ܼ����ñ��=array �ܼ������õ�Ԫ
	DataType_array,
	1,
//�ܼ����õ�Ԫ��=structure
	DataType_structure,
	3,
//{
//�����ܼӵķ�·ͨ�ŵ�ַ  TSA��
	DataType_TSA,
	0x07,0x05,0x10,0x00,0x00,0x00,0x00,0x01,
//�ܼӱ�־	            enum{����0��������1��}��
	DataType_enum,
	0,
//�������־	            enum{�ӣ�0��������1��}
	DataType_enum,
	0,
//}
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,
	
//�ܼ����ñ��=array �ܼ������õ�Ԫ
	DataType_array,
	1,
//�ܼ����õ�Ԫ��=structure
	DataType_structure,
	3,
//{
//�����ܼӵķ�·ͨ�ŵ�ַ  TSA��
	DataType_TSA,
	0x07,0x05,0x00,0x00,0x00,0x00,0x00,0x02,
//�ܼӱ�־	            enum{����0��������1��}��
	DataType_enum,
	0,
//�������־	            enum{�ӣ�0��������1��}
	DataType_enum,
	0,
//}
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,
};

//#define ADDR_230x_13     ADDR_230x_2+LENmax_230x_2
//#define LENper_230x_13     2
//#define LENmax_230x_13     (LENper_230x_13*NUMmax_TAG)
const u8 InitData_230x_13[]=
{
#if NUMmax_TAG<2
#error
#endif
//�ܼ��黬��ʱ������	unsigned����λ���֣�
	DataType_unsigned,
	1,
	
//�ܼ��黬��ʱ������	unsigned����λ���֣�
	DataType_unsigned,
	1,
};

//#define ADDR_230x_14     ADDR_230x_13+LENmax_230x_13
//#define LENper_230x_14     3
//#define LENmax_230x_14     (LENper_230x_14*NUMmax_TAG)
const u8 InitData_230x_14[]=
{
#if NUMmax_TAG<2
#error
#endif	
//�ܼ��鹦���ִ�����
//�����ִα�ʶλ��=bit-string��SIZE(8)��
	DataType_bit_string,
	8,
	0xC0,
//�����ִα�ʶλ��bit0��bit7��˳���λ��ʾ��1����8�ִο��ص��ܿ����ã��á�1����ʾ���ִο����ܿأ��á�0����ʾ���ܿ�
	
//�ܼ��鹦���ִ�����
//�����ִα�ʶλ��=bit-string��SIZE(8)��
	DataType_bit_string,
	8,
	0xC0,
//�����ִα�ʶλ��bit0��bit7��˳���λ��ʾ��1����8�ִο��ص��ܿ����ã��á�1����ʾ���ִο����ܿأ��á�0����ʾ���ܿ�	
};

//#define ADDR_230x_15     ADDR_230x_14+LENmax_230x_14
//#define LENper_230x_15     3
//#define LENmax_230x_15     (LENper_230x_15*NUMmax_TAG)
const u8 InitData_230x_15[]=
{
#if NUMmax_TAG<2
#error
#endif	
//�ܼ������ִ�����
//�����ִα�ʶλ��=bit-string��SIZE(8)��
	DataType_bit_string,
	8,
	0xC0,
//�����ִα�ʶλ��bit0��bit7��˳���λ��ʾ��1����8�ִο��ص��ܿ����ã��á�1����ʾ���ִο����ܿأ��á�0����ʾ���ܿ�
	
//�ܼ������ִ�����
//�����ִα�ʶλ��=bit-string��SIZE(8)��
	DataType_bit_string,
	8,
	0xC0,
//�����ִα�ʶλ��bit0��bit7��˳���λ��ʾ��1����8�ִο��ص��ܿ����ã��á�1����ʾ���ִο����ܿأ��á�0����ʾ���ܿ�
	
};

//�������
#if NUMmax_IMPIN!=0
//#define ADDR_240x_2     ADDR_230x_15+LENmax_230x_15
//#define LENper_240x_2     18
//#define LENmax_240x_2     (LENper_240x_2*NUMmax_IMPIN)
const u8 InitData_240x_2[]=
{
#if NUMmax_IMPIN<2
#error
#endif	
//ͨ�ŵ�ַ��=octet-string
	DataType_octet_string,
	6,
	0x10,0x00,0x00,0x00,0x00,0x01,
	0,0,0,0,0,0,0,0,0,0,

//ͨ�ŵ�ַ��=octet-string
	DataType_octet_string,
	6,
	0x10,0x00,0x00,0x00,0x00,0x02,
	0,0,0,0,0,0,0,0,0,0,	
};

//#define ADDR_240x_3     ADDR_240x_2+LENmax_240x_2
//#define LENper_240x_3     8
//#define LENmax_240x_3     (LENper_240x_3*NUMmax_IMPIN)
const u8 InitData_240x_3[]=
{
#if NUMmax_IMPIN<2
#error
#endif	
	//���������ʡ�=structure
	DataType_structure,
	2,
//{
//PT  long-unsigned��
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//CT  long-unsigned
	DataType_long_unsigned,
	(100>>8)&0xff,100&0xff,
//}
	
	//���������ʡ�=structure
	DataType_structure,
	2,
//{
//PT  long-unsigned��
	DataType_long_unsigned,
	(10>>8)&0xff,10&0xff,
//CT  long-unsigned
	DataType_long_unsigned,
	(100>>8)&0xff,100&0xff,
//}
};

//#define ADDR_240x_4     ADDR_240x_3+LENmax_240x_3
//#define LENper_240x_4     (2+12)
//#define LENmax_240x_4     (LENper_240x_4*NUMmax_IMPIN)
const u8 InitData_240x_4[]=
{
#if NUMmax_IMPIN<2
#error
#endif	
//�������á�=array ���嵥Ԫ
	DataType_array,
	1,
//���嵥Ԫ��=structure
	DataType_structure,
	3,
//{
//��������˿ں�	OAD��
	DataType_OAD,
	0xF2,0x0A,0x02,0x01,
//��������		enum
//{
//	 �����й���0����
//	 �����޹���1����
//	 �����й���2����
//	 �����޹���3��
//}��
	DataType_enum,
	0,
//���峣��k     long-unsigned
	DataType_long_unsigned,
	(1000>>8)&0xff,1000&0xff,
//}

//�������á�=array ���嵥Ԫ
	DataType_array,
	1,
//���嵥Ԫ��=structure
	DataType_structure,
	3,
//{
//��������˿ں�	OAD��
	DataType_OAD,
	0xF2,0x0A,0x02,0x02,
//��������		enum
//{
//	 �����й���0����
//	 �����޹���1����
//	 �����й���2����
//	 �����޹���3��
//}��
	DataType_enum,
	0,
//���峣��k     long-unsigned
	DataType_long_unsigned,
	(1000>>8)&0xff,1000&0xff,
//}
};
#endif





#endif
#endif

