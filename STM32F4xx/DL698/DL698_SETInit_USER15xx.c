
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"



#if (USER/100)==15//��΢ϵ



////***ͨ�ŵ�ַ��***
//#define ADDR_4001     ADDR_DL698_SET_START//ͨ�ŵ�ַ
//#define LENmax_4001     18
const u8 InitData_4001[]=
{
//����2��=octet-string
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x01,
};

const u8 InitData_3106_11[]=//�ն�ͣ/�ϵ��¼�-11
{
//����11�����ò���2����=structure
	DataType_structure,
	1,
//{
//�����ɼ������ unsigned
	DataType_unsigned,
	54,
//}
};


//#define ADDR_4002     ADDR_4005+LENmax_4005//���
//#define LENmax_4002     18
const u8 InitData_4002[]=
{
//����2��=octet-string
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x01,
};

//#define ADDR_4003     ADDR_4002+LENmax_4002//�ͻ����
//#define LENmax_4003     18
const u8 InitData_4003[]=
{
//����2��=octet-string
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x01,
};

//#define ADDR_4005     ADDR_4001+LENmax_4001//���ַ
//#define LENmax_4005     (2+(16*18))
const u8 InitData_4005[]=
{
//����2��=array octet-string
	DataType_array,
	1,
	DataType_octet_string,
	6,
	0x11,0x22,0x33,0x44,0x55,0x66,
};

//#define  ADDR_4300_7     ADDR_4003+LENmax_4003//��������ϱ�
//#define LENmax_4300_7     2
const u8 InitData_4300_7[]=
{
//����7��=bool
	DataType_bool,
	0,
};
//#define  ADDR_4300_8     ADDR_4300_7+LENmax_4300_7//���������ϱ�
//#define LENmax_4300_8     2
const u8 InitData_4300_8[]=
{
//����8��=bool
	DataType_bool,
	1,
};
//#define  ADDR_4300_9     ADDR_4300_8+LENmax_4300_8//��������վͨ��
//#define LENmax_4300_9     2
const u8 InitData_4300_9[]=
{
//����8��=bool
	DataType_bool,
	0,
};
//#define  ADDR_4300_10     ADDR_4300_9+LENmax_4300_9//�ϱ�ͨ��
//#define LENmax_4300_10     32
const u8 InitData_4300_10[]=
{
//����10��=array OAD
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

//#define ADDR_4401     ADDR_4003+LENmax_4003//Ӧ��������֤����
//#define LENmax_4401     128
const u8 InitData_4401[]=
{
//����2��ֻд����=visible-string
	DataType_visible_string,
	6,
	'0','0','0','0','0','0',
};

//#define ADDR_4500_2     ADDR_4003+LENmax_4003//����ͨ��ģ��1;2ͨ������
//#define LENmax_4500_2     128
const u8 InitData_4500_2[]=
{
//ͨ�����á�=structure
	DataType_structure,
	12,
//{
//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
	DataType_enum,
	1,
//���߷�ʽ      enum{�������ߣ�0�����������1��}��
	DataType_enum,
	0,
//���ӷ�ʽ      enum{TCP��0����UDP��1��}��
	DataType_enum,
	0,
//����Ӧ�÷�ʽ  enum{����ģʽ��0����������ģʽ��1��}��
	DataType_enum,
	0,
//�����˿��б�  array long-unsigned��
	DataType_array,
	0,
//	DataType_long_unsigned,
//	0x23,0x43,//9027,
//APN            visible-string��	
	DataType_visible_string,
	//10,
	//'c','m','i','o','t','y','t','.','a','h',
	11,
	'c','m','i','o','t','y','t','x','.','a','h',
//�û���         visible-string��
	DataType_visible_string,
	4,
	'C','A','R','D',	
//����           visible-string��
	DataType_visible_string,
	4,
	'C','A','R','D',	
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��ʱʱ�估�ط�����  bit-string(SIZE(8))
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
	DataType_unsigned,
  (30<<2)+0x03,
//��������(��)  long-unsigned
	DataType_long_unsigned,
	(300>>8)&0xff,300&0xff,	
//}
};

//#define ADDR_4500_3     ADDR_4500_2+LENmax_4500_2//����ͨ��ģ��1;3��վͨ�Ų�����
//#define LENmax_4500_3     64
const u8 InitData_4500_3[]=
{
//��վͨ�Ų������=array ��վͨ�Ų���
	DataType_array,
	2,
//��1����վͨ�Ų�������=structure
	DataType_structure,
	2,
//{
//����IP��ַ   octet-string��
	DataType_octet_string,
	4,
	10,138,16,133,
//���ö˿�     long-unsigned��
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}
//��2����վͨ�Ų�������=structure
	DataType_structure,
	2,
//{
//����IP��ַ   octet-string��
	DataType_octet_string,
	4,
	10,138,16,133,
//���ö˿�     long-unsigned��
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}	
};

//#define ADDR_4500_4     ADDR_4500_3+LENmax_4500_3//����ͨ��ģ��1;4����ͨ�Ų���
//#define LENmax_4500_4     64
const u8 InitData_4500_4[]=
{
//����ͨ�Ų�������=structure
	DataType_structure,
  3,
//{
//�������ĺ���     visible-string(SIZE(16))��
	DataType_visible_string,
	0,
//��վ����         array visible-string(SIZE(16))��
	DataType_array,
	0,
//  ����֪ͨĿ�ĺ��� array visible-string(SIZE(16))
	DataType_array,
	0,
//}
};

//#define ADDR_4500_11     ADDR_4500_10+LENmax_4500_10//����ͨ��ģ��1;11 ����IP
//#define LENmax_4500_11     22
const u8 InitData_4500_11[]=
{
//octet-string
	DataType_octet_string,
	0,
};

//#define ADDR_4510_2     ADDR_4500_4+LENmax_4500_4//��̫��ͨ��ģ��1;2ͨ������
//#define LENmax_4510_2     128
const u8 InitData_4510_2[]=
{
//ͨ�����á�=structure
	DataType_structure,
	8,
//{
//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
	DataType_enum,
	1,
//���ӷ�ʽ  enum{TCP��0����UDP��1��}��
	DataType_enum,
	0,
//����Ӧ�÷�ʽenum{����ģʽ��0����������ģʽ��1��}
	DataType_enum,
	0,
//�����˿��б�array long-unsigned��
	DataType_array,
	0,
//	DataType_long_unsigned,
//	9027>>8,9027&0xff,//9027,
//�����������ַ  octet-string��
	DataType_octet_string,
	0,
//����˿�  long-unsigned��
	DataType_long_unsigned,
	0,0,
//��ʱʱ�估�ط�����  bit-string(SIZE(8))
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
	DataType_unsigned,
  (30<<2)+0x03,
//��������(��)  long-unsigned
	DataType_long_unsigned,
	(60>>8)&0xff,60&0xff,	
//}
};

//#define ADDR_4510_3     ADDR_4510_2+LENmax_4510_2//��̫��ͨ��ģ��1;3��վͨ�Ų�����
//#define LENmax_4510_3     64
const u8 InitData_4510_3[]=
{
//��վͨ�Ų������=array ��վͨ�Ų���
	DataType_array,
	2,
//��1����վͨ�Ų�������=structure
	DataType_structure,
	2,
//{
//����IP��ַ   octet-string��
	DataType_octet_string,
	4,
	10,138,16,133,
//���ö˿�     long-unsigned��
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}
//��2����վͨ�Ų�������=structure
	DataType_structure,
	2,
//{
//����IP��ַ   octet-string��
	DataType_octet_string,
	4,
	10,138,16,133,
//���ö˿�     long-unsigned��
	DataType_long_unsigned,
	(29010>>8)&0xff,29010&0xff,
//}
};

//#define ADDR_4510_4     ADDR_4510_3+LENmax_4510_3//��̫��ͨ��ģ��1;4��������
//#define LENmax_4510_4     128
const u8 InitData_4510_4[]=
{
//�������á�=structure
	DataType_structure,
	6,
//{
//IP���÷�ʽenum{DHCP��0������̬��1����PPPoE��2��}��
	DataType_enum,
	1,
//IP��ַ       octet-string��
	DataType_octet_string,
	4,
	192,168,127,244,
//��������     octet-string��
	DataType_octet_string,
	4,
	255,255,255,0x00,
//���ص�ַ     octet-string��
	DataType_octet_string,
	4,
	192,168,127,1,
//PPPoE�û���  visible-string��
	DataType_visible_string,
	0,
//PPPoE����    visible-string
	DataType_visible_string,
	0,
//}
};

//#define ADDR_4510_5     ADDR_4510_4+LENmax_4510_4// ��̫��ͨ��ģ��1;5 MAC��ַ
//#define LENmax_4510_5     16
const u8 InitData_4510_5[]=
{
//MAC��ַ��=octet-string 
	DataType_octet_string,
    6,
	0x12,0x34,0x56,0x78,0x00,0x00,
};	

//#define ADDR_4520_2     ADDR_240x_4+LENmax_240x_4//����Զ��ͨ�Ŷ����㱸��ͨ��
//#define LENmax_4520_2     512
const u8 InitData_4520_2[]=
{
//��վͨ�Ų������=array ��վͨ�Ų���
	DataType_array,
	6,
//��1����վͨ�Ų�������=structure
	DataType_structure,
	8,
//��Ӫ�� enum 
//{ 
//CMCC���ƶ��� = 0�� 
//CTCC�����ţ� = 1�� 
//CUCC����ͨ�� = 2�� 
//δ֪ = 255 
//}��
    DataType_enum,
    0,
// �������� enum 
// { 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 }��
    DataType_enum,
    2,
//APN            visible-string��	
	DataType_visible_string,
	11,
	'c','m','i','o','t','y','t','x','.','a','h',
//�û���         visible-string��
	DataType_visible_string,
	0,
//����           visible-string��
	DataType_visible_string,
    0,
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��վͨ�Ų��� array structure 
//{ 
//IP��ַ octet-string�� 
//�˿� long-unsigned 
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
	
	//��2����վͨ�Ų�������=structure
	DataType_structure,
	8,
//��Ӫ�� enum 
//{ 
//CMCC���ƶ��� = 0�� 
//CTCC�����ţ� = 1�� 
//CUCC����ͨ�� = 2�� 
//δ֪ = 255 
//}��
  DataType_enum,
  2,
// �������� enum 
// { 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 }��
  DataType_enum,
  2,
//APN            visible-string��	
	DataType_visible_string,
	24,
	'a','h','b','z','g','w','d','l','0','1','.','z','n','c','b',
	'.','n','j','m','2','m','a','p','n',
//�û���         visible-string��
	DataType_visible_string,
	0,
//����           visible-string��
	DataType_visible_string,
    0,
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��վͨ�Ų��� array structure 
//{ 
//IP��ַ octet-string�� 
//�˿� long-unsigned 
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
	
	//��3����վͨ�Ų�������=structure
	DataType_structure,
	8,
//��Ӫ�� enum 
//{ 
//CMCC���ƶ��� = 0�� 
//CTCC�����ţ� = 1�� 
//CUCC����ͨ�� = 2�� 
//δ֪ = 255 
//}��
    DataType_enum,
    1,
// �������� enum 
// { 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 }��
    DataType_enum,
    2,
//APN            visible-string��	
	DataType_visible_string,
	12,
	'd','l','c','b','.','v','p','d','n','.','a','h',
//�û���         visible-string��
	DataType_visible_string,
	19,
	'y','z','w','x','@','d','l','c','d','m','a',
	'.','v','p','d','n','.','a','h',
//����           visible-string��
	DataType_visible_string,
    6,
    '1','2','3','4','5','6',
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��վͨ�Ų��� array structure 
//{ 
//IP��ַ octet-string�� 
//�˿� long-unsigned 
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
    
//���ĸ���վͨ�Ų�������=structure
	DataType_structure,
	8,
//��Ӫ�� enum 
//{ 
//CMCC���ƶ��� = 0�� 
//CTCC�����ţ� = 1�� 
//CUCC����ͨ�� = 2�� 
//δ֪ = 255 
//}��
    DataType_enum,
    0,
// �������� enum 
// { 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 }��
    DataType_enum,
    1,
//APN            visible-string��	
	DataType_visible_string,
	11,
	'd','l','g','p','r','s','c','j','.','a','h',
//�û���         visible-string��
	DataType_visible_string,
	0,
//����           visible-string��
	DataType_visible_string,
    0,
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��վͨ�Ų��� array structure 
//{ 
//IP��ַ octet-string�� 
//�˿� long-unsigned 
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
	
//��5����վͨ�Ų�������=structure
	DataType_structure,
	8,
//��Ӫ�� enum 
//{ 
//CMCC���ƶ��� = 0�� 
//CTCC�����ţ� = 1�� 
//CUCC����ͨ�� = 2�� 
//δ֪ = 255 
//}��
  DataType_enum,
  2,
// �������� enum 
// { 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 }��
  DataType_enum,
  1,
//APN            visible-string��	
	DataType_visible_string,
	16,
	'h','f','g','d','j','.','y','c','c','j','.','a','h','a','p','n',
//�û���         visible-string��
	DataType_visible_string,
	0,
//����           visible-string��
	DataType_visible_string,
    0,
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��վͨ�Ų��� array structure 
//{ 
//IP��ַ octet-string�� 
//�˿� long-unsigned 
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
    
//��6����վͨ�Ų�������=structure
	DataType_structure,
	8,
//��Ӫ�� enum 
//{ 
//CMCC���ƶ��� = 0�� 
//CTCC�����ţ� = 1�� 
//CUCC����ͨ�� = 2�� 
//δ֪ = 255 
//}��
    DataType_enum,
    1,
// �������� enum 
// { 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 }��
    DataType_enum,
    1,
//APN            visible-string��	
	DataType_visible_string,
	14,
	'd','l','c','d','m','a','.','v','p','d','n','.','a','h',
//�û���         visible-string��
	DataType_visible_string,
	19,
	'y','z','w','x','@','d','l','c','d','m','a',
	'.','v','p','d','n','.','a','h',
//����           visible-string��
	DataType_visible_string,
    6,
    '1','2','3','4','5','6',
//�����������ַ octet-string��
	DataType_octet_string,
	0,
//����˿�            long-unsigned��
	DataType_long_unsigned,
	0,0,
//��վͨ�Ų��� array structure 
//{ 
//IP��ַ octet-string�� 
//�˿� long-unsigned 
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

	 
//*************************ͨ����*************************
//#define ADDR_4000_3     ADDR_4510_5+LENmax_4510_5//Уʱģʽ
//#define LENmax_4000_3     2
const u8 InitData_4000_3[]=
{
//����3��Уʱģʽ����=enum
//{
//��վ��ʱ��0����
//�ն˾�ȷУʱ��1����
//����/GPS��2����
//������255��
//}
	DataType_enum,
  1,
};

//#define ADDR_4000_4     ADDR_4000_3+LENmax_4000_3//��׼Уʱ����
//#define LENmax_4000_4     12
const u8 InitData_4000_4[]=
{
//����4����׼Уʱ��������=structure
	DataType_structure,
	5,
//{
//�������ʱ���ܸ���   unsigned��
	DataType_unsigned,
	10,
//���ֵ�޳�����       unsigned��
	DataType_unsigned,
	1,
//��Сֵ�޳�����       unsigned��
	DataType_unsigned,
	1,
//ͨѶ��ʱ��ֵ         unsigned����λ���룩�� 
	DataType_unsigned,
	5,
//������Ч����         unsigned
	DataType_unsigned,
	5,
//}
};

//#define ADDR_4000_127     ADDR_4000_4+LENmax_4000_4//����127���㲥Уʱ��������
////������=date_time_s
//#define LENmax_4000_127     8

//#define ADDR_4004     ADDR_4000_127+LENmax_4000_127//�豸����λ��
//#define LENmax_4004     30
const u8 InitData_4004[]=
{
//����2��=structure
	DataType_structure,
	3,
//{
	//����  structure
	DataType_structure,
	4,
	//{
		//��λ   enum{E��0����W��1��}��
	DataType_enum,
	0,
		//��     unsigned��
	DataType_unsigned,
	0,
		//��     unsigned��
	DataType_unsigned,
	0,
		//��     unsigned
	DataType_unsigned,
	0,
	//}��
	//γ��  structure
	DataType_structure,
	4,
	//{
		//��λ   enum{S��0����N��1��}��
	DataType_enum,
	0,
		//��     unsigned��
	DataType_unsigned,
	0,
		//��     unsigned��
	DataType_unsigned,
	0,
		//��     unsigned
	DataType_unsigned,
	0,
	//}��
	//�߶ȣ�cm��  double-long-unsigned
	DataType_double_long_unsigned,
	0,
//}
};

//#define ADDR_4006     ADDR_4004+LENmax_4004//ʱ��Դ
//#define LENmax_4006     16
const u8 InitData_4006[]=
{
//����2��ֻ������=structure
	DataType_structure,
	2,
//{
//	ʱ��Դ  enum
//	{
//		�ڲ���0����ʱ��оƬ��1����������ʱ�ӣ�2��������ʱ�ӣ�3��������ʱ�ӣ�4��
//	}��
	DataType_enum,
	1,
//	״̬  enum
//	{
//		���ã�0���������ã�1��
//  }
	DataType_enum,
	0,
//}
};

//#define ADDR_4007     ADDR_4006+LENmax_4006//LCD����
//#define LENmax_4007     20
const u8 InitData_4007[]=
{
//����2::=structure
	DataType_structure,
	7,
//{
//  �ϵ�ȫ��ʱ��  unsigned��
	DataType_unsigned,
	5,
//  �������ʱ��  long-unsigned(����ʱ�������ʱ��)��
	DataType_long_unsigned,
	0,5,
//  ��ʾ�鿴�������ʱ��  long-unsigned��
	DataType_long_unsigned,
	0,5,
//  �е簴����Ļפ�����ʱ��  long-unsigned��
	DataType_long_unsigned,
	0,5,
//  �޵簴����Ļפ�����ʱ��  long-unsigned��
	DataType_long_unsigned,
	0,5,
//  ��ʾ����С��λ��  unsigned��
	DataType_unsigned,
	2,
//  ��ʾ���ʣ����������С��λ��  unsigned
	DataType_unsigned,
	2,
//}
//����ʱ��ĵ�λ��Ϊ���롣
};



//***���ܱ���***
//#define ADDR_4008     ADDR_4007+LENmax_4007//����ʱ�����л�ʱ��
//#define LENmax_4008     10
const u8 InitData_4008[]=
{
//����2��=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_4009     ADDR_4008+LENmax_4008//������ʱ���л�ʱ��
//#define LENmax_4009     10
const u8 InitData_4009[]=
{
//����2��=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_400A     ADDR_4009+LENmax_4009//���׷�ʱ�����л�ʱ��
//#define LENmax_400A     10
const u8 InitData_400A[]=
{
//����2��=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_400B     ADDR_400A+LENmax_400A//���׽��ݵ���л�ʱ��
//#define LENmax_400B     10
const u8 InitData_400B[]=
{
//����2��=date_time_s
	DataType_date_time_s,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

//#define ADDR_400C     ADDR_400B+LENmax_400B//ʱ��ʱ����
//#define LENmax_400C     16
const u8 InitData_400C[]=
{
//����2��=structure
	DataType_structure,
	5,
//{
//��ʱ����(p��14)                 unsigned��
	DataType_unsigned,
	1,
//��ʱ�α�����q��8��              unsigned��
	DataType_unsigned,
	1,
//��ʱ����(ÿ���л���)��m��14��   unsigned��
	DataType_unsigned,
	4,
//��������k��63��                 unsigned��
	DataType_unsigned,
	4,
//������������n��254��            unsigned
	DataType_unsigned,
	0,
//}
};

//#define ADDR_400D     ADDR_400C+LENmax_400C//������
//#define LENmax_400D     2
const u8 InitData_400D[]=
{
//����2��=unsigned���޵�λ���޻���
	DataType_unsigned,
	0,
};

//#define ADDR_400E     ADDR_400D+LENmax_400D//г����������
//#define LENmax_400E     2
const u8 InitData_400E[]=
{
//����2��=unsigned���޵�λ���޻���
	DataType_unsigned,
	21,
};

//#define ADDR_400F     ADDR_400E+LENmax_400E//��Կ������
//#define LENmax_400F     2
const u8 InitData_400F[]=
{
//����2��=unsigned���޵�λ���޻���
	DataType_unsigned,
	20,
};

//#define ADDR_4010     ADDR_400F+LENmax_400F//����Ԫ����
//#define LENmax_4010     2
const u8 InitData_4010[]=
{
//����2��=unsigned���޵�λ���޻���
	DataType_unsigned,
#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
	3,
#else
	2,
#endif
	
};

//#define ADDR_4011     ADDR_400F+LENmax_4010//�������ձ�
//#define LENper_4011     8
//#define NUMmax_4011     30
//#define LENmax_4011     (2+(LENper_4011*NUMmax_4011))
const u8 InitData_4011[]=
{
//����2��=array ��������
	DataType_array,	
	0,
//�������ա�=structure
//{
//  ����        date��
	
//  ��ʱ�α��  unsigned
//}
//���ܱ�ʵ��ʹ�ö��ٸ���������ȡ���ڹ�����������������ܱ������ٿɱ���20���������ա�
};

//#define ADDR_4012     ADDR_4011+LENmax_4011//������������
//#define LENmax_4012     3
const u8 InitData_4012[]=
{
//����2��=bit-string(SIZE(8))
	DataType_bit_string,	
	8,
	0xFE,
};

//#define ADDR_4013     ADDR_4012+LENmax_4012//��������õ���ʱ�α��
//#define LENmax_4013     2
const u8 InitData_4013[]=
{
//����2��=unsigned���޵�λ���޻���
	DataType_unsigned,
	1,
};

//#define ADDR_4014     ADDR_4013+LENmax_4013//��ǰ��ʱ����
//#define LENper_4014     8
//#define NUMmax_4014     12
//#define LENmax_4014     (2+(LENper_4014*NUMmax_4014))
const u8 InitData_4014[]=
{
//����2��=array
	DataType_array,	
	1,
//ʱ����=structure
	DataType_structure,	
	3,
//{
//��          unsigned,
	DataType_unsigned,
	1,
//��          unsigned,
	DataType_unsigned,
	1,
//  ��ʱ�α��  unsigned
	DataType_unsigned,
	1,
//}
};

//#define ADDR_4015     ADDR_4014+LENmax_4014//������ʱ����
//#define LENper_4015     8
//#define NUMmax_4015     12
//#define LENmax_4015     (2+(LENper_4015*NUMmax_4015))
const u8 InitData_4015[]=
{
//����2��=array
	DataType_array,	
	2,
//ʱ����=structure
	DataType_structure,	
	3,
//{
//��          unsigned,
	DataType_unsigned,
	1,
//��          unsigned,
	DataType_unsigned,
	1,
//  ��ʱ�α��  unsigned
	DataType_unsigned,
	1,
//}
//ʱ����=structure
	DataType_structure,	
	3,
//{
//��          unsigned,
	DataType_unsigned,
	6,
//��          unsigned,
	DataType_unsigned,
	1,
//  ��ʱ�α��  unsigned
	DataType_unsigned,
	2,
//}
};

//#define ADDR_4016     ADDR_4015+LENmax_4015//��ǰ����ʱ�α�
//#define LENper_4016     (2+(8*8))
//#define NUMmax_4016     4
//#define LENmax_4016     (2+(LENper_4016*NUMmax_4016))
const u8 InitData_4016[]=
{
//����2��=array ��ʱ�α�
	DataType_array,	
	1,
	
//�壺9��00��12��00��17��00��22��00��ƽ��8��00��9��00��12��00��17��00��22��00����23��00���ȣ�23��00����8��00
//��ʱ�α��=array ʱ��
	DataType_array,	
	6,
	
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	0,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	4,
//}
	
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	9,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	2,
//}

//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	12,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	3,
//}

//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	17,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	2,
//}

//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	22,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	3,
//}

//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	23,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	4,
//}

};

//#define ADDR_4017     ADDR_4016+LENmax_4016//��������ʱ�α�
//#define LENper_4017     (2+(8*8))
//#define NUMmax_4017     4
//#define LENmax_4017     (2+(LENper_4017*NUMmax_4017))
const u8 InitData_4017[]=
{
//����2��=array ��ʱ�α�
	DataType_array,	
	1,
//��ʱ�α��=array ʱ��
	DataType_array,	
	8,
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	0,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	1,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	12,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	1,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	3,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	2,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	15,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	2,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	6,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	3,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	18,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	3,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	9,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	4,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
//ʱ�Ρ�=structure
	DataType_structure,	
	3,
//{
//  ʱ     unsigned,
	DataType_unsigned,
	21,
//  ��     unsigned,
	DataType_unsigned,
	0,
//  ���ʺ� unsigned
	DataType_unsigned,
	4,
//}
//���ʺţ���ʱ�β��õķ��ʺ�
};

//#define ADDR_4018     ADDR_4017+LENmax_4017//��ǰ�׷��ʵ��
//#define LENper_4018     5
//#define NUMmax_4018     8
//#define LENmax_4018     (2+(LENper_4018*NUMmax_4018))
const u8 InitData_4018[]=
{
//����2��=array ���ʵ��
	DataType_array,	
	4,	
//���ʵ�ۡ�=double-long-unsigned
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1Ԫ/kWh,
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1Ԫ/kWh,
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1Ԫ/kWh,
	DataType_double_long_unsigned,
	0x00,0x00,(10000>>8)&0xff,10000&0xff,//1Ԫ/kWh,
//��λ��Ԫ/kWh�����㣺-4�����ܱ����֧��32�����ʵ�ۣ���ǰ�׷��ʵ��Э��ֻ����
};

//#define ADDR_4019     ADDR_4018+LENmax_4018//�����׷��ʵ��
//#define LENper_4019     5
//#define NUMmax_4019     8
//#define LENmax_4019     (2+(LENper_4019*NUMmax_4019))
const u8 InitData_4019[]=
{
//����2��=array ���ʵ��
	DataType_array,	
	4,	
//���ʵ�ۡ�=double-long-unsigned
	DataType_double_long_unsigned,
	0x00,0x00,0x13,0x88,//5000,
	DataType_double_long_unsigned,
	0x00,0x00,0x13,0x24,//4900,
	DataType_double_long_unsigned,
	0x00,0x00,0x12,0xC0,//4800,
	DataType_double_long_unsigned,
	0x00,0x00,0x12,0x5C,//4700,
//��λ��Ԫ/kWh�����㣺-4�����ܱ����֧��32�����ʵ�ۣ���ǰ�׷��ʵ��Э��ֻ����
};

//#define ADDR_401A     ADDR_4019+LENmax_4019//��ǰ�׽��ݵ��
//#define LENmax_401A     1024
const u8 InitData_401A[]=
{
//���ݲ�����=structure
	DataType_structure,	
	3,
//{
//  ����ֵ����    array ����ֵ��
	DataType_array,	
	6,
//����ֵ��=double-long-unsigned
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
//  ���ݵ������  array ���ݵ�ۣ�
	DataType_array,	
	7,	
//���ݵ�ۡ�=double-long-unsigned
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
//  ���ݽ���������  array ���ݽ�����
	DataType_array,	
	4,
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}	
//}
//����ֵ����λ��kWh�����㣺-2
//���ݵ�ۣ���λ��Ԫ/kWh�����㣺-4
//���ݽ��������¡��ա�ʱ����Чʱ�򰴽�����ִ�н��ݶ��ᣬ�����1���ݽ������н��¡�����Ч���Դ�ÿ�¸���ʱִ�н��ݶ��ᣬ��ȫ����Чʱ��ִ�н��ݶ��ᡣ
//��ǰ�׽��ݲ���Э��ֻ����
};

//#define ADDR_401B     ADDR_401A+LENmax_401A//�����׽��ݵ��
//#define LENmax_401B     1024
const u8 InitData_401B[]=
{
//���ݲ�����=structure
	DataType_structure,	
	3,
//{
//  ����ֵ����    array ����ֵ��
	DataType_array,	
	6,
//����ֵ��=double-long-unsigned
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
//  ���ݵ������  array ���ݵ�ۣ�
	DataType_array,	
	7,	
//���ݵ�ۡ�=double-long-unsigned
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
//  ���ݽ���������  array ���ݽ�����
	DataType_array,	
	4,
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}
//���ݽ����ա�=structure
	DataType_structure,	
	3,
//{
//��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ��  unsigned��
	DataType_unsigned,	
	0xFF,	
//  ʱ  unsigned
	DataType_unsigned,	
	0xFF,	
//}	
//}
//����ֵ����λ��kWh�����㣺-2
//���ݵ�ۣ���λ��Ԫ/kWh�����㣺-4
//���ݽ��������¡��ա�ʱ����Чʱ�򰴽�����ִ�н��ݶ��ᣬ�����1���ݽ������н��¡�����Ч���Դ�ÿ�¸���ʱִ�н��ݶ��ᣬ��ȫ����Чʱ��ִ�н��ݶ��ᡣ
//��ǰ�׽��ݲ���Э��ֻ����
};


//#define ADDR_401C     ADDR_401B+LENmax_401B//�������������
//#define LENmax_401C     8
const u8 InitData_401C[]=
{
//����2��=double-long-unsigned���޵�λ������:-2
	DataType_double_long_unsigned,
	0,0,0,1,
};

//#define ADDR_401D     ADDR_401C+LENmax_401C//��ѹ���������
//#define LENmax_401D     8
const u8 InitData_401D[]=
{
//����2��=double-long-unsigned���޵�λ������:-2
	DataType_double_long_unsigned,
	0,0,0,1,
};

//#define ADDR_401E     ADDR_401D+LENmax_401D//���������ֵ
//#define LENmax_401E     12
const u8 InitData_401E[]=
{
//����2����������=structure
	DataType_structure,	
	2,
//{
//���������ֵ1    double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//���������ֵ2    double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//��λ��Ԫ�����㣺-2
};

//#define ADDR_401F     ADDR_401E+LENmax_401E//���������ֵ
//#define LENmax_401F     18
const u8 InitData_401F[]=
{
//����2����������=structure
	DataType_structure,	
	3,
//{
//͸֧�����ֵ     double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//�ڻ������ֵ     double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//��բ��������ֵ double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//��λ��Ԫ�����㣺-2
};

//#define ADDR_4020     ADDR_401F+LENmax_401F//����������ֵ
//#define LENmax_4020     12
const u8 InitData_4020[]=
{
//����2��=structure
	DataType_structure,	
	2,
//{
//����������ֵ1    double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//����������ֵ2    double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//��λ��kWh�����㣺-2
};

//#define ADDR_4021     ADDR_4020+LENmax_4020//����������ֵ
//#define LENmax_4021     18
const u8 InitData_4021[]=
{
//����2��=structure
	DataType_structure,	
	3,
//{
//�ڻ�������ֵ     double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//͸֧������ֵ     double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//��բ���������ֵ double-long-unsigned��
	DataType_double_long_unsigned,	
	0,0,0,0,
//}
//��λ��kWh�����㣺-2
};
/*
//#define ADDR_4022     ADDR_4021+LENmax_4021//�忨״̬��
//#define LENmax_4022     4
const u8 InitData_4022[]=
{
//����2��=bit-string(SIZE(16))���޵�λ���޻���
	DataType_bit_string,	
	16,
	0x00,0x00,	
};
*/
//#define ADDR_4023     ADDR_4022+LENmax_4022//��֤��Чʱ��
//#define LENmax_4023     3
const u8 InitData_4023[]=
{
//����2��=long-unsigned ����λ�����ӣ��޻���
	DataType_long_unsigned,	
	0,0,
};

//#define ADDR_4024     ADDR_4023+LENmax_4023//�޳�
//#define LENmax_4024     2
const u8 InitData_4024[]=
{
//����2��=enum {�޳�Ͷ�루1�����޳������2��}
	DataType_enum,	
	2,
};

//#define ADDR_4030     ADDR_4024+LENmax_4024//��ѹ�ϸ��ʲ���
//#define LENmax_4030     14
const u8 InitData_4030[]=
{
//����2��=structure
	DataType_structure,
  4,	
//{
#if MeterUBhex==2200//�αȵ�ѹxxx.x(V)
//��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	2640>>8,2640&0xff,
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	1540>>8,1540&0xff,
//��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	2354>>8,2354&0xff,
//  ��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1��
	DataType_long_unsigned,
	2046>>8,2046&0xff,
#endif
#if MeterUBhex==1000//�αȵ�ѹxxx.x(V)
//��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	1200>>8,1200&0xff,
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	780>>8,780&0xff,
//��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	1070>>8,1070&0xff,
//  ��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1��
	DataType_long_unsigned,
	900>>8,900&0xff,
#endif
#if MeterUBhex==577//�αȵ�ѹxxx.x(V)
//��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	692>>8,692&0xff,
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	450>>8,450&0xff,
//��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	617>>8,617&0xff,
//  ��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1��
	DataType_long_unsigned,
	519>>8,519&0xff,
#endif
//}
};

//#define ADDR_4100     ADDR_4030+LENmax_4030//�����������
//#define LENmax_4100     2
const u8 InitData_4100[]=
{
//����2��=unsigned����λ�����ӣ����㣺0
	DataType_unsigned,
	15,
};

//#define ADDR_4101     ADDR_4100+LENmax_4100//����ʱ��
//#define LENmax_4101     2
const u8 InitData_4101[]=
{
//����2��=unsigned����λ�����ӣ����㣺0
	DataType_unsigned,
	1,
};

//#define ADDR_4102     ADDR_4101+LENmax_4101//У��������
//#define LENmax_4102     2
const u8 InitData_4102[]=
{
//����2��=unsigned����λ�����룬���㣺0
	DataType_unsigned,
	80,
};

//#define ADDR_4103     ADDR_4102+LENmax_4102//�ʲ��������
//#define LENmax_4103     34
const u8 InitData_4103[]=
{
//����2��=visible-string(SIZE(32))
	DataType_visible_string,
	32,
	'0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1',
};

//#define ADDR_410C     ADDR_410B+LENmax_410B//ABC����絼ϵ��
//#define LENmax_410C     12
const u8 InitData_410C[]=
{
//����2��=structure
	DataType_structure,
	3,
//{
//A ��絼  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//B ��絼  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//C ��絼  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_410D     ADDR_410C+LENmax_410C//ABC����翹ϵ��
//#define LENmax_410D     12
const u8 InitData_410D[]=
{
//����2��=structure
	DataType_structure,
	3,
//{
//A ��翹  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//B ��翹  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//C ��翹  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_410E     ADDR_410D+LENmax_410D//ABC�������ϵ��
//#define LENmax_410E     12
const u8 InitData_410E[]=
{
//����2��=structure
	DataType_structure,
	3,
//{
//A �����  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//B �����  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//C �����  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_410F     ADDR_410E+LENmax_410E//ABC�������ϵ��
//#define LENmax_410F     12
const u8 InitData_410F[]=
{
//����2��=structure
	DataType_structure,
	3,
//{
//A �����  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//B �����  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
//C �����  long����λ���ޣ����㣺-3
	DataType_long,
	0,0,
// }
};

//#define ADDR_4112     ADDR_4111+LENmax_4111//�й���Ϸ�ʽ������
//#define LENmax_4112     3
const u8 InitData_4112[]=
{
//����2��=bit-string(SIZE(8))������¼G
	DataType_bit_string,
	8,
	0xA0,	
};

//#define ADDR_4113     ADDR_4112+LENmax_4112//�޹���Ϸ�ʽ1������
//#define LENmax_4113     3
const u8 InitData_4113[]=
{
//����2��=bit-string(SIZE(8))������¼G
	DataType_bit_string,
	8,
	0x82,	
};

//#define ADDR_4114     ADDR_4113+LENmax_4113//�޹���Ϸ�ʽ2������
//#define LENmax_4114     3
const u8 InitData_4114[]=
{
//����2��=bit-string(SIZE(8))������¼G
	DataType_bit_string,
	8,
	0x28,	
};

//#define ADDR_4115     ADDR_4114+LENmax_4114//IC��
//#define LENmax_4115     4
const u8 InitData_4115[]=
{
//����2��=structure
	DataType_structure,
	1,
//{
//IC������  enum
//{
//����Ԥ�ƿ���0�����û���������1����
//�û����翨��2�����û�������3��
//}��
	DataType_enum,
	2,
//}
};

//#define ADDR_4116     ADDR_4115+LENmax_4115//������
//#define LENper_4116     6
//#define NUMmax_4116     31
//#define LENmax_4116     (2+(LENper_4116*NUMmax_4116))
const u8 InitData_4116[]=
{
//����2�����ò�������=array ����������
	DataType_array,
	1,
//���������ڡ�=structure
	DataType_structure,
	2,
//{
//��  unsigned,
	DataType_unsigned,
	1,
//ʱ  unsigned
	DataType_unsigned,
	0,
//}
};

//#define ADDR_4117     ADDR_4116+LENmax_4116//�ڼ�������������
//#define LENmax_4117     5
const u8 InitData_4117[]=
{
//����2�����ò�������= TI
	DataType_TI,
	01,0,15,	
};




//***�ն���***

//4200	11	·�ɱ�
//4201	8	·����Ϣ��Ԫ
//#define ADDR_4202     ADDR_4117+LENmax_4117//����ͨ�Ų���
//#define LENmax_4202     128
const u8 InitData_4202[]=
{
//����2��=structure
	DataType_structure,
	8,
//{
//������־            bool��
	DataType_bool,
	0,
//����ͨ�Ŷ˿ں�      OAD��
	DataType_OAD,
	0xF2,0x01,0x02,0x01,
//�ܵȴ���ʱ��10ms��  long-unsigned��
	DataType_long_unsigned,
	0,0,
//�ֽڳ�ʱ��10ms��    long-unsigned��
	DataType_long_unsigned,
	0,0,
//�ط�����            unsigned��
	DataType_unsigned,
	0,
//Ѳ�����ڣ�min��     unsigned��
	DataType_unsigned,
	0,
//�����������˿���    unsigned��
	DataType_unsigned,
	0,
//�����������ն˵�ַ  array TSA
	DataType_array,
	0,
//}
};

//#define ADDR_4204_2     ADDR_4202+LENmax_4202//�ն˹㲥Уʱ-2
//#define LENmax_4204_2     16
const u8 InitData_4204_2[]=
{
//����2���ն˹㲥Уʱ��������=structure
	DataType_structure,
	2,
//{
//     �ն˹㲥Уʱ����ʱ��  time��
	DataType_time,
	0x00,0x00,0x00,
//     �Ƿ�����              bool
	DataType_bool,
	0,
//}
};

//#define ADDR_4204_3     ADDR_4204_2+LENmax_4204_2//�ն˹㲥Уʱ-3
//#define LENmax_4204_3     16
const u8 InitData_4204_3[]=
{
//����3���ն˵���ַ�㲥Уʱ��������=structure
	DataType_structure,
	3,
//{
//     ʱ�������ֵ          unsigned����λ���룩��
	DataType_integer,
	0,
//     �ն˹㲥Уʱ����ʱ��  time��
	DataType_time,
	0x00,0x00,0x00,
//     �Ƿ�����              bool
	DataType_bool,
	0,
//}
//���ʱ��������ն��Զ������ã�ǰ���ǲɼ������������вɼ����ʱ�ӡ�ִ�е���ַ�㲥Уʱ�����ɶ�Ӧ���ն˶Ե��Уʱ�¼�
};






//---���������---
//˲ʱ��������������Ա�
const u8 InitData_5000[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};

//�붳������������Ա�
const u8 InitData_5001[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};

//���Ӷ�������������Ա�
const u8 InitData_5002[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	11,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
//�����й�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00100200>>24)&0xff,(0x00100200>>16)&0xff,(0x00100200>>8)&0xff,(0x00100200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//�����й�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00200200>>24)&0xff,(0x00200200>>16)&0xff,(0x00200200>>8)&0xff,(0x00200200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//1�����޹�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00500200>>24)&0xff,(0x00500200>>16)&0xff,(0x00500200>>8)&0xff,(0x00500200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//2�����޹�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00600200>>24)&0xff,(0x00600200>>16)&0xff,(0x00600200>>8)&0xff,(0x00600200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//3�����޹�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00700200>>24)&0xff,(0x00700200>>16)&0xff,(0x00700200>>8)&0xff,(0x00700200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//4�����޹�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00800200>>24)&0xff,(0x00800200>>16)&0xff,(0x00800200>>8)&0xff,(0x00800200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//��ѹ
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x20000200>>24)&0xff,(0x20000200>>16)&0xff,(0x20000200>>8)&0xff,(0x20000200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x20010200>>24)&0xff,(0x20010200>>16)&0xff,(0x20010200>>8)&0xff,(0x20010200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//�й�����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x20040200>>24)&0xff,(0x20040200>>16)&0xff,(0x20040200>>8)&0xff,(0x20040200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//�޹�����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x20050200>>24)&0xff,(0x20050200>>16)&0xff,(0x20050200>>8)&0xff,(0x20050200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
//��������
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(CongealTime_5002>>8)&0xff,(CongealTime_5002>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x200A0200>>24)&0xff,(0x200A0200>>16)&0xff,(0x200A0200>>8)&0xff,(0x200A0200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	((96*2)>>8)&0xff,((96*2)>>0)&0xff,
//}
	
	
};
//Сʱ��������������Ա�
const u8 InitData_5003[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};

//�ն�������������Ա�
const u8 InitData_5004[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	9,
	
//�����й�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00100200>>24)&0xff,(0x00100200>>16)&0xff,(0x00100200>>8)&0xff,(0x00100200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//�����й�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00200200>>24)&0xff,(0x00200200>>16)&0xff,(0x00200200>>8)&0xff,(0x00200200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//����޹�1����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00300200>>24)&0xff,(0x00300200>>16)&0xff,(0x00300200>>8)&0xff,(0x00300200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//����޹�2����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00400200>>24)&0xff,(0x00400200>>16)&0xff,(0x00400200>>8)&0xff,(0x00400200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//�����й��������
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x10100200>>24)&0xff,(0x10100200>>16)&0xff,(0x10100200>>8)&0xff,(0x10100200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}

//�����й��������
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x10200200>>24)&0xff,(0x10200200>>16)&0xff,(0x10200200>>8)&0xff,(0x10200200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//����A���ѹ�ϸ���
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x21310200>>24)&0xff,(0x21310200>>16)&0xff,(0x21310200>>8)&0xff,(0x21310200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//����B���ѹ�ϸ���
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x21320200>>24)&0xff,(0x21320200>>16)&0xff,(0x21320200>>8)&0xff,(0x21320200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
//����C���ѹ�ϸ���
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x21330200>>24)&0xff,(0x21330200>>16)&0xff,(0x21330200>>8)&0xff,(0x21330200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(62>>8)&0xff,(62>>0)&0xff,
//}
	
	
};
//�����ն�������������Ա�
const u8 InitData_5005[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};

//�¶�������������Ա�
const u8 InitData_5006[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	9,
	
//�����й�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00100200>>24)&0xff,(0x00100200>>16)&0xff,(0x00100200>>8)&0xff,(0x00100200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}
	
//�����й�
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00200200>>24)&0xff,(0x00200200>>16)&0xff,(0x00200200>>8)&0xff,(0x00200200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//����޹�1����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00300200>>24)&0xff,(0x00300200>>16)&0xff,(0x00300200>>8)&0xff,(0x00300200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//����޹�2����
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x00400200>>24)&0xff,(0x00400200>>16)&0xff,(0x00400200>>8)&0xff,(0x00400200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//�����й��������
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x10100200>>24)&0xff,(0x10100200>>16)&0xff,(0x10100200>>8)&0xff,(0x10100200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//�����й��������
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x10200200>>24)&0xff,(0x10200200>>16)&0xff,(0x10200200>>8)&0xff,(0x10200200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}

//����A���ѹ�ϸ���
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x21310200>>24)&0xff,(0x21310200>>16)&0xff,(0x21310200>>8)&0xff,(0x21310200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}
	
//����B���ѹ�ϸ���
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x21320200>>24)&0xff,(0x21320200>>16)&0xff,(0x21320200>>8)&0xff,(0x21320200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}
	
//����C���ѹ�ϸ���
//һ�������Ķ������ԡ�=structure
	DataType_structure,
	3,
//{
//  ��������  long-unsigned��
	DataType_long_unsigned,
	(1>>8)&0xff,(1>>0)&0xff,
//  ������������������  OAD��
	DataType_OAD,
	(0x21330200>>24)&0xff,(0x21330200>>16)&0xff,(0x21330200>>8)&0xff,(0x21330200>>0)&0xff,
//  �洢���  long-unsigned
	DataType_long_unsigned,
	(12>>8)&0xff,(12>>0)&0xff,
//}


};

//�궳������������Ա�
const u8 InitData_5007[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};
//ʱ�����л���������������Ա�
const u8 InitData_5008[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};
//��ʱ�α��л���������������Ա�
const u8 InitData_5009[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};
//���ʵ���л���������������Ա�
const u8 InitData_500A[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};
//�����л���������������Ա�
const u8 InitData_500B[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};
//���ݽ��㶳������������Ա�
const u8 InitData_5011[]=
{
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_array,
	0,
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
};





#endif






