




//---ͨ����---
__align(4) const u8 DL698_Attribute_4001[]=
{
	1,//�����б���
//����2��=TSA
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4001[]=//ͨ�ŵ�ַ
{
	0x40010200,//OAD
  ADDR_4001,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4001,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4001,//���������б�ָ��
	(u32*)InitData_4001,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4001),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4002[]=
{
	1,//�����б���
//����2��=octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4002[]=//���
{
	0x40020200,//OAD
  ADDR_4002,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4002,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4002,//���������б�ָ��
	(u32*)InitData_4002,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4002),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4003[]=
{
	1,//�����б���
//����2��=octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4003[]=//�ͻ����
{
	0x40030200,//OAD
  ADDR_4003,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4003,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4003,//���������б�ָ��
	(u32*)InitData_4003,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4003),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4005[]=
{
	3,//�����б���
//����2��=array 
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
//octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4005[]=//���ַ
{
	0x40050200,//OAD
  ADDR_4005,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4005,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4005,//���������б�ָ��
	(u32*)InitData_4005,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4005),//��ʼ��ԭʼ���ݳ���
};


__align(4) const u8 DL698_Attribute_4400_4[]=
{
	1,//�����б���
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4400_4[]=//��ǰ���ӵĿͻ�����ַ(��ʹ�п��б�,������ʵ��ָ��)
{
	0x44000400,//OAD
  ADDR_DATABUFF,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	2,//LENmax_4400,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4400_4,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4401[]=
{
	1,//�����б���
//����2��ֻд����=visible-string
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4401[]=//Ӧ��������֤����
{
	0x44010200,//OAD
  ADDR_4401,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4401,//FLASHԤ������󳤶�
	6,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4401,//���������б�ָ��
	(u32*)InitData_4401,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4401),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4500_2[]=
{
	15,//�����б���
//ͨ�����á�=structure
		DataType_structure,
//{
//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
		DataType_enum,
//���߷�ʽ      enum{�������ߣ�0�����������1��}��
		DataType_enum,
//���ӷ�ʽ      enum{TCP��0����UDP��1��}��
		DataType_enum,
//����Ӧ�÷�ʽ  enum{����ģʽ��0����������ģʽ��1��}��
		DataType_enum,
//�����˿��б�  array long-unsigned��
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
		DataType_long_unsigned,
//APN            visible-string��
		DataType_visible_string,
//�û���         visible-string��
		DataType_visible_string,
//����           visible-string��
		DataType_visible_string,
//�����������ַ octet-string��
		DataType_octet_string,
//����˿�            long-unsigned��
		DataType_long_unsigned,
//��ʱʱ�估�ط�����  unsigned
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
		DataType_unsigned,
//��������(��)  long-unsigned
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_2[]=//����ͨ��ģ��1;2ͨ������
{
	0x45000200,//OAD
  ADDR_4500_2,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_2,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_2,//���������б�ָ��
	(u32*)InitData_4500_2,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4500_2),//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_3[]=
{
	5,//�����б���
//��վͨ�Ų������=array ��վͨ�Ų���
		DataType_array,
		3,//������������������͸���,��������=0ʱ�������͸�����
//��վͨ�Ų�������=structure
		DataType_structure,
//{
//IP��ַ   octet-string��
		DataType_octet_string,
//�˿�     long-unsigned��
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_3[]=//����ͨ��ģ��1;3��վͨ�Ų�����
{
	0x45000300,//OAD
  ADDR_4500_3,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_3,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_3,//���������б�ָ��
	(u32*)InitData_4500_3,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4500_3),//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_4[]=
{
	8,//�����б���
//����ͨ�Ų�������=structure
		DataType_structure,
//{
//�������ĺ���     visible-string(SIZE(16))��
		DataType_visible_string,
//��վ����         array visible-string(SIZE(16))��
		DataType_array,
		1,//������������������͸���,���������=0ʱ����Ԫ�ظ�����
		DataType_visible_string,
//  ����֪ͨĿ�ĺ��� array visible-string(SIZE(16))
		DataType_array,
		1,//������������������͸���,���������=0ʱ����Ԫ�ظ�����
		DataType_visible_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_4[]=//����ͨ��ģ��1;4����ͨ�Ų���
{
	0x45000400,//OAD
  ADDR_4500_4,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_4,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_4,//���������б�ָ��
	(u32*)InitData_4500_4,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4500_4),//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_5[]=
{
	7,//�����б���
//�汾��Ϣ��=structure
		DataType_structure,
//{
//���̴���       visible-string(SIZE (4))��
		DataType_visible_string,
//����汾��     visible-string(SIZE (4))��
		DataType_visible_string,
//����汾����   visible-string(SIZE (6))��
		DataType_visible_string,
//Ӳ���汾��     visible-string(SIZE (4))��
		DataType_visible_string,
//Ӳ���汾����   visible-string(SIZE (6))��
		DataType_visible_string,
//������չ��Ϣ   visible-string(SIZE (8))
		DataType_visible_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_5[]=//����ͨ��ģ��1;5�汾��Ϣ
{
	0x45000500,//OAD
  ADDR_4500_5,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_5,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_5,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_6[]=
{
	2+1,//�����б���
//���ݸ�ʽ array visible-string
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_6[]=//����ͨ��ģ��1;6֧�ֹ�Լ�б�
{
	0x45000600,//OAD
  ADDR_4500_6,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_6,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_6,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_7[]=
{
	1,//�����б���
//visible-string(SIZE(20))
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_7[]=//����ͨ��ģ��1;7 SIM����ICCID
{
	0x45000700,//OAD
  ADDR_4500_7,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_7,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_7,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_8[]=
{
	1,//�����б���
//visible-string(SIZE(20))
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_8[]=//����ͨ��ģ��1;8 IMSI
{
	0x45000800,//OAD
  ADDR_4500_8,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_8,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_8,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_9[]=
{
	1,//�����б���
//long����λ��dBm
		DataType_long,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_9[]=//����ͨ��ģ��1;9 �ź�ǿ��
{
	0x45000900,//OAD
  ADDR_4500_9,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_9,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_9,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_10[]=
{
	1,//�����б���
//visible-string(SIZE(16))
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_10[]=//����ͨ��ģ��1;10 SIM������
{
	0x45000A00,//OAD
  ADDR_4500_10,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_10,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_10,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4500_11[]=
{
	1,//�����б���
//octet-string
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4500_11[]=//����ͨ��ģ��1;11 ����IP
{
	0x45000B00,//OAD
  ADDR_4500_11,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4500_11,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_11,//���������б�ָ��
	(u32*)InitData_4500_11,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4500_11),//��ʼ��ԭʼ���ݳ���
};


__align(4) const u8 DL698_Attribute_4510_2[]=
{
	11,//�����б���
//ͨ�����á�=structure
		DataType_structure,
//{
//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
		DataType_enum,
//���ӷ�ʽ  enum{TCP��0����UDP��1��}��
		DataType_enum,
//����Ӧ�÷�ʽenum{����ģʽ��0����������ģʽ��1��}
		DataType_enum,
//�����˿��б�array long-unsigned��
		DataType_array,
		1,
		DataType_long_unsigned,
//�����������ַ  octet-string��
		DataType_octet_string,
//����˿�  long-unsigned��
		DataType_long_unsigned,
//��ʱʱ�估�ط�����  unsigned
		DataType_unsigned,
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
//��������(��)  long-unsigned
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_2[]=//��̫��ͨ��ģ��1;2ͨ������
{
	0x45100200,//OAD
  ADDR_4510_2,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4510_2,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4510_2,//���������б�ָ��
	(u32*)InitData_4510_2,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4510_2),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_3[]=//��̫��ͨ��ģ��1;3��վͨ�Ų�����
{
	0x45100300,//OAD
  ADDR_4510_3,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4510_3,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4500_3,//���������б�ָ��
	(u32*)InitData_4510_3,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4510_3),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4510_4[]=
{
	7,//�����б���
//�������á�=structure
		DataType_structure,
//{
//IP���÷�ʽenum{DHCP��0������̬��1����PPPoE��2��}��
		DataType_enum,
//IP��ַ       octet-string��
		DataType_octet_string,
//��������     octet-string��
		DataType_octet_string,
//���ص�ַ     octet-string��
		DataType_octet_string,
//PPPoE�û���  octet-string��
		DataType_visible_string,
//PPPoE����    octet-string
		DataType_visible_string,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_4[]=//��̫��ͨ��ģ��1;4��������
{
	0x45100400,//OAD
  ADDR_4510_4,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4510_4,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4510_4,//���������б�ָ��
	(u32*)InitData_4510_4,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4510_4),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4510_5[]=
{
	1,//�����б���
//MAC��ַ��=octet-string 
		DataType_octet_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4510_5[]=//��̫��ͨ��ģ��1;5 MAC��ַ
{
	0x45100500,//OAD
  ADDR_4510_5,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4510_5,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4510_5,//���������б�ָ��
	(u32*)InitData_4510_5,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4510_5),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4520_2[]=
{
    15,
//��վͨ�Ų������=array ��վͨ�Ų���
	DataType_array,
    6,
//����ͨ����=structure 
    DataType_structure,
//��Ӫ�� enum 
    DataType_enum,
//�������� enum 
    DataType_enum,
//APN visible-string�� 
    DataType_visible_string,
//�û��� visible-string��
    DataType_visible_string,
//���� visible-string��
    DataType_visible_string,
//�����������ַ octet-string��
    DataType_octet_string,
//����˿� long-unsigned�� 
    DataType_long_unsigned,
//��վͨ�Ų��� array structure 
    DataType_array,
    2,
    DataType_structure,
//IP��ַ octet-string�� 
    DataType_octet_string,
//�˿� long-unsigned 
    DataType_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4520_2[]=//����Զ��ͨ�Ŷ����㱸��ͨ�� 
{
	0x45200200,//OAD
  ADDR_4520_2,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4520_2,//FLASHԤ������󳤶�
	3,//7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4520_2,//���������б�ָ��
	(u32*)InitData_4520_2,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4520_2),//��ʼ��ԭʼ���ݳ���
};




//---ͨ����---
__align(4) const u8 DL698_Attribute_4000[]=
{
	1,//�����б���
//����2��=date_time_s
		DataType_date_time_s,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4000[]=//����ʱ��
{
	0x40000200,//OAD
  ADDR_DL698YMDHMS,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4000,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4000_3[]=
{
	1,//�����б���
//����3��Уʱģʽ����=enum
		DataType_enum,
//{
//��վ��ʱ��0����
//�ն˾�ȷУʱ��1����
//����/GPS��2����
//������255��
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4000_3[]=//Уʱģʽ
{
	0x40000300,//OAD
  ADDR_4000_3,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4000_3,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4000_3,//���������б�ָ��
	(u32*)InitData_4000_3,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4000_3),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4000_4[]=
{
	6,//�����б���
//����4����׼Уʱ��������=structure
		DataType_structure,
//{
//�������ʱ���ܸ���   unsigned��
		DataType_unsigned,
//���ֵ�޳�����       unsigned��
		DataType_unsigned,
//��Сֵ�޳�����       unsigned��
		DataType_unsigned,
//ͨѶ��ʱ��ֵ         unsigned����λ���룩
		DataType_unsigned,
//������Ч����         unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4000_4[]=//��׼Уʱ����
{
	0x40000400,//OAD
  ADDR_4000_4,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4000_4,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4000_4,//���������б�ָ��
	(u32*)InitData_4000_4,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4000_4),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4004[]=
{
	12,//�����б���
//����2��=structure
		DataType_structure,
//{
//����  structure
		DataType_structure,
//{
//��λ   enum{E��0����W��1��}��
		DataType_enum,
//��     unsigned��
		DataType_unsigned,
//��     unsigned��
		DataType_unsigned,
//��     unsigned
		DataType_unsigned,
//}��
//γ��  structure
		DataType_structure,
//{
//��λ   enum{S��0����N��1��}��
		DataType_enum,
//��     unsigned��
		DataType_unsigned,
//��     unsigned��
		DataType_unsigned,
//��     unsigned
		DataType_unsigned,
//}��
//�߶ȣ�cm��  double-long-unsigned
		DataType_double_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4004[]=//����λ��
{
	0x40040200,//OAD
  ADDR_4004,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4004,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4004,//���������б�ָ��
	(u32*)InitData_4004,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4004),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4006[]=
{
	3,//�����б���
//����2��ֻ������=structure
		DataType_structure,
//{
//ʱ��Դ  enum
		DataType_enum,
//{
//�ڲ���0����ʱ��оƬ��1����������ʱ�ӣ�2����
//����ʱ�ӣ�3��������ʱ�ӣ�4��
//}��
//״̬  enum{���ã�0���������ã�1)
		DataType_enum,
//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4006[]=//ʱ��Դ
{
	0x40060200,//OAD
  ADDR_4006,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4006,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4006,//���������б�ָ��
	(u32*)InitData_4006,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4006),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4007[]=
{
	8,//�����б���
//����2::=structure
		DataType_structure,
//{
//  �ϵ�ȫ��ʱ��  unsigned��
		DataType_unsigned,
//  �������ʱ��  long-unsigned(����ʱ�������ʱ��)��
		DataType_long_unsigned,
//  ��ʾ�鿴�������ʱ��  long-unsigned��
		DataType_long_unsigned,
//  �е簴����Ļפ�����ʱ��  long-unsigned��
		DataType_long_unsigned,
//  �޵簴����Ļפ�����ʱ��  long-unsigned��
		DataType_long_unsigned,
//  ��ʾ����С��λ��  unsigned��
		DataType_unsigned,
//  ��ʾ���ʣ����������С��λ��  unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4007[]=//LCD����
{
	0x40070200,//OAD
  ADDR_4007,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4007,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4007,//���������б�ָ��
	(u32*)InitData_4007,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4007),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4008[]=
{
	1,//�����б���
//����2��=date_time_s
		DataType_date_time_s,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4008[]=//����ʱ�����л�ʱ��
{
	0x40080200,//OAD
  ADDR_4008,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4008,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4008,//���������б�ָ��
	(u32*)InitData_4008,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4008),//��ʼ��ԭʼ���ݳ���
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4009[]=//������ʱ���л�ʱ��
{
	0x40090200,//OAD
  ADDR_4009,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4009,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4008,//���������б�ָ��
	(u32*)InitData_4009,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4009),//��ʼ��ԭʼ���ݳ���
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400A[]=//���׷�ʱ�����л�ʱ��
{
	0x400A0200,//OAD
  ADDR_400A,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_400A,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4008,//���������б�ָ��
	(u32*)InitData_400A,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_400A),//��ʼ��ԭʼ���ݳ���
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400B[]=//���׽��ݵ���л�ʱ��
{
	0x400B0200,//OAD
  ADDR_400B,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_400B,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4008,//���������б�ָ��
	(u32*)InitData_400B,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_400B),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_400C[]=
{
	6,//�����б���
//����2��=structure
		DataType_structure,
//{
//��ʱ����(p��14)                 unsigned��
		DataType_unsigned,
//��ʱ�α�����q��8��              unsigned��
		DataType_unsigned,
//��ʱ����(ÿ���л���)��m��14��   unsigned��
		DataType_unsigned,
//��������k��63��                 unsigned��
		DataType_unsigned,
//������������n��254��            unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400C[]=//ʱ��ʱ����
{
	0x400C0200,//OAD
  ADDR_400C,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_400C,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_400C,//���������б�ָ��
	(u32*)InitData_400C,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_400C),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_400D[]=
{
	1,//�����б���
//����2��=unsigned���޵�λ���޻���
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400D[]=//������
{
	0x400D0200,//OAD
  ADDR_400D,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_400D,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_400D,//���������б�ָ��
	(u32*)InitData_400D,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_400D),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_400E[]=
{
	1,//�����б���
//����2��=unsigned���޵�λ���޻���
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400E[]=//г����������
{
	0x400E0200,//OAD
  ADDR_400E,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_400E,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_400E,//���������б�ָ��
	(u32*)InitData_400E,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_400E),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_400F[]=
{
	1,//�����б���
//����2��=unsigned���޵�λ���޻���
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_400F[]=//��Կ������
{
	0x400F0200,//OAD
  ADDR_400F,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_400F,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_400F,//���������б�ָ��
	(u32*)InitData_400F,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_400F),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4010[]=
{
	1,//�����б���
//����2��=unsigned���޵�λ���޻���
	DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4010[]=//����Ԫ����
{
	0x40100200,//OAD
  ADDR_4010,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4010,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4010,//���������б�ָ��
	(u32*)InitData_4010,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4010),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4011[]=
{
	4+1,//�����б���
//����2��=array ��������
		DataType_array,
		3,//������������������͸���,��������=0ʱ�������͸�����
//�������ա�=structure
		DataType_structure,
//{
//  ����        Date��
		DataType_date,
//  ��ʱ�α��  unsigned
		DataType_unsigned,
//}
//���ܱ�ʵ��ʹ�ö��ٸ���������ȡ���ڹ�����������������ܱ������ٿɱ���20���������ա�
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4011[]=//�������ձ�
{
	0x40110200,//OAD
  ADDR_4011,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4011,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4011,//���������б�ָ��
	(u32*)InitData_4011,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4011),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4012[]=
{
	1,//�����б���
//����2��=bit-string(SIZE(8))
		DataType_bit_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4012[]=//������������
{
	0x40120200,//OAD
  ADDR_4012,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4012,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4012,//���������б�ָ��
	(u32*)InitData_4012,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4012),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4013[]=
{
	1,//�����б���
//����2��=unsigned���޵�λ���޻���
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4013[]=//��������õ���ʱ�α��
{
	0x40130200,//OAD
  ADDR_4013,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4013,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4013,//���������б�ָ��
	(u32*)InitData_4013,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4013),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4014[]=
{
	5+1,//�����б���
//����2��=array ʱ��
		DataType_array,
		4,//������������������͸���,��������=0ʱ�������͸�����
//ʱ����=structure
		DataType_structure,
//{
//��          ComBCD2,
		DataType_unsigned,
//��          ComBCD2,
		DataType_unsigned,
//  ��ʱ�α��  unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4014[]=//��ǰ��ʱ����
{
	0x40140200,//OAD
  ADDR_4014,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4014,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4014,//���������б�ָ��
	(u32*)InitData_4014,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4014),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4015[]=//������ʱ����
{
	0x40150200,//OAD
  ADDR_4015,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4015,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4014,//���������б�ָ��
	(u32*)InitData_4015,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4015),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4016[]=
{
	6+2,//�����б���
//����2��=array ��ʱ�α�
		DataType_array,
		5,//������������������͸���,��������=0ʱ�������͸�����
//��ʱ�α��=array ʱ��
		DataType_array,
		4,//������������������͸���,��������=0ʱ�������͸�����
//ʱ�Ρ�=structure
		DataType_structure,
//{
//  ʱ     unsigned
		DataType_unsigned,
//  ��     unsigned
		DataType_unsigned,
//  ���ʺ� unsigned
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4016[]=//��ǰ����ʱ�α�
{
	0x40160200,//OAD
  ADDR_4016,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4016,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4016,//���������б�ָ��
	(u32*)InitData_4016,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4016),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4017[]=//��������ʱ�α�
{
	0x40170200,//OAD
  ADDR_4017,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4017,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4016,//���������б�ָ��
	(u32*)InitData_4017,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4017),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4018[]=
{
	2+1,//�����б���
//����2��=array ���ʵ��
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
//���ʵ�ۡ�=double-long-unsigned
		DataType_double_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4018[]=//��ǰ�׷��ʵ��,��ǰ�׷��ʵ��Э��ֻ��
{
	0x40180200,//OAD
  ADDR_4018,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4018,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4018,//���������б�ָ��
	(u32*)InitData_4018,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4018),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4019[]=//�����׷��ʵ��,�����׷��ʵ�ۿɶ���д
{
	0x40190200,//OAD
  ADDR_4019,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4019,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4018,//���������б�ָ��
	(u32*)InitData_4019,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4019),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_401A[]=
{
	10+3,//�����б���
//���ݲ�����=structure
		DataType_structure,
//{
//  ����ֵ����    array ����ֵ��
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
	//����ֵ��=double-long-unsigned
		DataType_double_long_unsigned,
//  ���ݵ������  array ���ݵ�ۣ�
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
	//���ݵ�ۡ�=double-long-unsigned
		DataType_double_long_unsigned,
//  ���ݽ���������  array ���ݽ�����
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
	//���ݽ����ա�=structure
		DataType_structure,
		//{
		//  ��  unsigned��
		DataType_unsigned,
		//  ��  unsigned��
		DataType_unsigned,
		//  ʱ  unsigned
		DataType_unsigned,
		//}
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401A[]=//��ǰ�׽��ݵ��,��ǰ�׽��ݲ���Э��ֻ��
{
	0x401A0200,//OAD
  ADDR_401A,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_401A,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_401A,//���������б�ָ��
	(u32*)InitData_401A,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_401A),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401B[]=//�����׽��ݵ��,�����׽��ݲ����ɶ���д
{
	0x401B0200,//OAD
  ADDR_401B,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_401B,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_401A,//���������б�ָ��
	(u32*)InitData_401B,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_401B),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_401C[]=
{
	1,//�����б���
//����2��=double-long-unsigned���޵�λ������:-2
		DataType_double_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401C[]=//�������������
{
	0x401C0200,//OAD
  ADDR_401C,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_401C,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_401C,//���������б�ָ��
	(u32*)InitData_401C,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_401C),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401D[]=//��ѹ���������
{
	0x401D0200,//OAD
  ADDR_401D,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_401D,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_401C,//���������б�ָ��
	(u32*)InitData_401D,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_401D),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_401E[]=
{
	3,//�����б���
//����2����������=structure
		DataType_structure,
//{
//���������ֵ1    double-long-unsigned��
		DataType_double_long_unsigned,
//���������ֵ2    double-long-unsigned��
		DataType_double_long_unsigned,
//}
//��λ��Ԫ�����㣺-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401E[]=//���������ֵ
{
	0x401E0200,//OAD
  ADDR_401E,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_401E,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_401E,//���������б�ָ��
	(u32*)InitData_401E,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_401E),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_401F[]=
{
	4,//�����б���
//����2����������=structure
		DataType_structure,
//{
//͸֧�����ֵ     double-long-unsigned��
		DataType_double_long_unsigned,
//�ڻ������ֵ     double-long-unsigned��
		DataType_double_long_unsigned,
//��բ��������ֵ double-long-unsigned��
		DataType_double_long_unsigned,
//}
//��λ��Ԫ�����㣺-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_401F[]=//���������ֵ
{
	0x401F0200,//OAD
  ADDR_401F,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_401F,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_401F,//���������б�ָ��
	(u32*)InitData_401F,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_401F),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4020[]=
{
	3,//�����б���
//����2����������=structure
		DataType_structure,
//{
//����������ֵ1     double-long-unsigned��
		DataType_double_long_unsigned,
//����������ֵ2     double-long-unsigned��
		DataType_double_long_unsigned,
//}
//��λ��Ԫ�����㣺-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4020[]=//����������ֵ
{
	0x40200200,//OAD
  ADDR_4020,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4020,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4020,//���������б�ָ��
	(u32*)InitData_4020,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4020),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4021[]=
{
	4,//�����б���
//����2��=structure
		DataType_structure,
//{
//�ڻ�������ֵ     double-long-unsigned��
		DataType_double_long_unsigned,
//͸֧������ֵ     double-long-unsigned��
		DataType_double_long_unsigned,
//��բ���������ֵ double-long-unsigned��
		DataType_double_long_unsigned,
//}
//��λ��kWh�����㣺-2
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4021[]=//����������ֵ
{
	0x40210200,//OAD
  ADDR_4021,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4021,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4021,//���������б�ָ��
	(u32*)InitData_4021,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4021),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4022[]=
{
	1,//�����б���
//����2��=bit-string(SIZE(16))���޵�λ���޻���
		DataType_bit_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4022[]=//�忨״̬��
{
	0x40220200,//OAD
  ADDR_4022,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4022,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4022,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4023[]=
{
	1,//�����б���
//����2��=long-unsigned ����λ�����ӣ��޻���
		DataType_long_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4023[]=//��֤��Чʱ��
{
	0x40230200,//OAD
  ADDR_4023,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4023,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4023,//���������б�ָ��
	(u32*)InitData_4023,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4023),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4024[]=
{
	1,//�����б���
//����2��=enum {�޳�Ͷ�루1�����޳������2��}
		DataType_enum,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4024[]=//�޳�
{
	0x40240200,//OAD
  ADDR_4024,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4024,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4024,//���������б�ָ��
	(u32*)InitData_4024,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4024),//��ʼ��ԭʼ���ݳ���
};


__align(4) const u8 DL698_Attribute_4030[]=
{
	5,//�����б���
//����2��=structure
		DataType_structure,
//{
//��ѹ��������  long-unsigned����λ��V�����㣺-1����
		DataType_long_unsigned,
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
		DataType_long_unsigned,
//��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1����
		DataType_long_unsigned,
//  ��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1��
		DataType_long_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4030[]=//��ѹ�ϸ��ʲ���
{
	0x40300200,//OAD
  ADDR_4030,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4030,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4030,//���������б�ָ��
	(u32*)InitData_4030,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4030),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4100[]=
{
	1,//�����б���
//����2��=unsigned����λ�����ӣ����㣺0
		DataType_unsigned,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4100[]=//�����������
{
	0x41000200,//OAD
  ADDR_4100,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4100,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4100,//���������б�ָ��
	(u32*)InitData_4100,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4100),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4101[]=//����ʱ��
{
	0x41010200,//OAD
  ADDR_4101,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4101,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4100,//���������б�ָ��
	(u32*)InitData_4101,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4101),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4102[]=//У��������
{
	0x41020200,//OAD
  ADDR_4102,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4102,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4100,//���������б�ָ��
	(u32*)InitData_4102,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4102),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4103[]=
{
	1,//�����б���
//����2��=visible-string
		DataType_visible_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4103[]=//�ʲ��������
{
	0x41030200,//OAD
  ADDR_4103,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4103,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4103,//���������б�ָ��
	(u32*)InitData_4103,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4103),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 ADDR_4104[]=
{
//����2��=visible-string(SIZE(6))
	DataType_visible_string,
	6,
#if (MeterUBbcd==0x2200)//�αȵ�ѹxxx.x(V)
	'2','2','0','.','0','V',
#endif
#if (MeterUBbcd==0x1000)//�αȵ�ѹxxx.x(V)
	'1','0','0','.','0','V',
#endif
#if (MeterUBbcd==0x577)//�αȵ�ѹxxx.x(V)
	' ','5','7','.','7','V',
#endif
	
#if (MeterUBbcd!=0x2200)&&(MeterUBbcd!=0x1000)&&(MeterUBbcd!=0x577)
	#error
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4104[]=//���ѹ
{
	0x41040200,//OAD
  (u32)ADDR_4104,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 ADDR_4105[]=
{
//����2��=visible-string(SIZE(6))
	DataType_visible_string,
	6,
#if (MeterIBbcd==0x100)//�αȵ���xx.xx(A)
	' ','1','.','0','0','A',
#endif
#if (MeterIBbcd==0x150)//�αȵ���xx.xx(A)
	' ','1','.','5','0','A',
#endif
#if (MeterIBbcd==0x030)//�αȵ���xx.xx(A)
	' ','0','.','3','0','A',
#endif
	
#if ((MeterIBbcd!=0x100)&&(MeterIBbcd!=0x150)&&(MeterIBbcd!=0x030))
	#error
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4105[]=//�����/��������
{
	0x41050200,//OAD
  (u32)ADDR_4105,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 ADDR_4106[]=
{
//����2��=visible-string(SIZE(6))
	DataType_visible_string,
	6,
#if MeterIMaxbcd==0x600//������xx.xx(A)
	' ','6','.','0','0','A',
#endif
#if MeterIMaxbcd==0x8000//������xx.xx(A)
	'8','0','.','0','0','A',
#endif	
	
#if (MeterIMaxbcd!=0x600)&&(MeterIMaxbcd!=0x8000)//������xx.xx(A)
	#error
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4106[]=//������
{
	0x41060200,//OAD
  (u32)ADDR_4106,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 ADDR_4107[]=
{
//����2��=visible-string(SIZE(4))
	DataType_visible_string,
	4,
#if (MeterClass==0x02)//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��	
	' ','0','.','2',
#endif
#if (MeterClass==0x05)//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��	
	' ','0','.','5',
#endif
#if (MeterClass==0x10)//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��	
	' ','1','.','0',
#endif
#if (MeterClass==0x20)//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��	
	' ','2','.','0',
#endif
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4107[]=//�й�׼ȷ�ȵȼ�
{
	0x41070200,//OAD
  (u32)ADDR_4107,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 ADDR_4108[]=
{
//����2��=visible-string(SIZE(4))
	DataType_visible_string,
	4,
	' ','2','.','0',
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4108[]=//�޹�׼ȷ�ȵȼ�
{
	0x41080200,//OAD
  (u32)ADDR_4108,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4109[]=
{
	1,//�����б���
//����2��=double-long-unsigned����λ��imp/kWh�����㣺0
		DataType_double_long_unsigned,
};
__align(4) const u8 ADDR_4109[]=
{
//����2��=double-long-unsigned����λ��imp/kWh�����㣺0
	DataType_double_long_unsigned,
	(MeterActiveConst>>24)&0xff,(MeterActiveConst>>16)&0xff,(MeterActiveConst>>8)&0xff,(MeterActiveConst>>0)&0xff
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4109[]=//���ܱ��й�����
{
	0x41090200,//OAD
  (u32)ADDR_4109,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 ADDR_410A[]=
{
//����2��=double-long-unsigned����λ��imp/kvarh�����㣺0
	DataType_double_long_unsigned,
	(MeterReactiveConst>>24)&0xff,(MeterReactiveConst>>16)&0xff,(MeterReactiveConst>>8)&0xff,(MeterReactiveConst>>0)&0xff
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410A[]=//���ܱ��޹�����
{
	0x410A0200,//OAD
  (u32)ADDR_410A,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 ADDR_410B[]=
{
//����2��=visible-string(SIZE(32))
	DataType_visible_string,
	32,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410B[]=//���ܱ��ͺ�
{
	0x410B0200,//OAD
  (u32)ADDR_410B,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_410C[]=
{
	4,//�����б���
//����2��=structure
		DataType_structure,
//{
//A ��絼  long����λ���ޣ����㣺-3
		DataType_long,
//B ��絼  long����λ���ޣ����㣺-3
		DataType_long,
//C ��絼  long����λ���ޣ����㣺-3
		DataType_long,
// }
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410C[]=//ABC����絼ϵ��
{
	0x410C0200,//OAD
  ADDR_410C,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_410C,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_410C,//���������б�ָ��
	(u32*)InitData_410C,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_410C),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410D[]=//ABC����翹ϵ��
{
	0x410D0200,//OAD
  ADDR_410D,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_410D,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_410C,//���������б�ָ��
	(u32*)InitData_410D,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_410D),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410E[]=//ABC�������ϵ��
{
	0x410E0200,//OAD
  ADDR_410E,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_410E,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_410C,//���������б�ָ��
	(u32*)InitData_410E,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_410E),//��ʼ��ԭʼ���ݳ���
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_410F[]=//ABC�������ϵ��
{
	0x410F0200,//OAD
  ADDR_410F,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_410F,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_410C,//���������б�ָ��
	(u32*)InitData_410F,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_410F),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 ADDR_4111[]=
{
//����2��=visible-string
	DataType_visible_string,
	8,
	'A','A','A','A','A','A','A','A',	
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4111[]=//���������
{
	0x41110200,//OAD
  (u32)ADDR_4111,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4112[]=
{
	1,//�����б���
//����2��=bit-string(SIZE (8))
		DataType_bit_string,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4112[]=//�й���Ϸ�ʽ������
{
	0x41120200,//OAD
  ADDR_4112,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4112,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4112,//���������б�ָ��
	(u32*)InitData_4112,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4112),//��ʼ��ԭʼ���ݳ���
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4113[]=//�޹���Ϸ�ʽ1������
{
	0x41130200,//OAD
  ADDR_4113,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4113,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4112,//���������б�ָ��
	(u32*)InitData_4113,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4113),//��ʼ��ԭʼ���ݳ���
};

__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4114[]=//�޹���Ϸ�ʽ2������
{
	0x41140200,//OAD
  ADDR_4114,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4114,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4112,//���������б�ָ��
	(u32*)InitData_4114,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4114),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4115[]=
{
	2,//�����б���
//����2��=structure
		DataType_structure,
//{
//IC������  enum
		DataType_enum,
//{
//����Ԥ�ƿ���0�����û���������1����
//�û����翨��2�����û�������3��
//}��
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4115[]=//IC��
{
	0x41150200,//OAD
  ADDR_4115,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4115,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4115,//���������б�ָ��
	(u32*)InitData_4115,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4115),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4116[]=
{
	4+1,//�����б���
//����2�����ò�������=array ����������
		DataType_array,
		3,//������������������͸���,��������=0ʱ�������͸�����
//���������ڡ�=structure
		DataType_structure,
//{
//��  ComBCD2,
		DataType_unsigned,
//ʱ  ComBCD2
		DataType_unsigned,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4116[]=//������
{
	0x41160200,//OAD
  ADDR_4116,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4116,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4116,//���������б�ָ��
	(u32*)InitData_4116,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4116),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4117[]=
{
	1,//�����б���
//����2�����ò�������= TI
		DataType_TI,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4117[]=//�ڼ�������������
{
	0x41170200,//OAD
  ADDR_4117,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4117,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4117,//���������б�ָ��
	(u32*)InitData_4117,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4117),//��ʼ��ԭʼ���ݳ���
};






//---�ն���---
//4200	11	·�ɱ�
//4201	8	·����Ϣ��Ԫ
__align(4) const u8 DL698_Attribute_4202[]=
{
	10+1,//�����б���
//����2��=structure
		DataType_structure,
//{
//������־            bool��
		DataType_bool,
//����ͨ�Ŷ˿ں�      OAD��
		DataType_OAD,
//�ܵȴ���ʱ��10ms��  long-unsigned��
		DataType_long_unsigned,
//�ֽڳ�ʱ��10ms��    long-unsigned��
		DataType_long_unsigned,
//�ط�����            unsigned��
		DataType_unsigned,
//Ѳ�����ڣ�min��     unsigned��
		DataType_unsigned,
//�����������˿���    unsigned��
		DataType_unsigned,
//�����������ն˵�ַ  array TSA
		DataType_array,
		1,//������������������͸���,��������=0ʱ�������͸�����
		DataType_TSA,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4202[]=//����ͨ�Ų���
{
	0x42020200,//OAD
  ADDR_4202,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4202,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4202,//���������б�ָ��
	(u32*)InitData_4202,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4202),//��ʼ��ԭʼ���ݳ���
};

__align(4) const u8 DL698_Attribute_4204_2[]=
{
	3,//�����б���
//����2���ն˹㲥Уʱ��������=structure
		DataType_structure,
//{
//     �ն˹㲥Уʱ����ʱ��  time��
		DataType_time,
//     �Ƿ�����              bool
		DataType_bool,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4204_2[]=//�ն˹㲥Уʱ-2
{
	0x42040200,//OAD
  ADDR_4204_2,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4204_2,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4204_2,//���������б�ָ��
	(u32*)InitData_4204_2,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4204_2),//��ʼ��ԭʼ���ݳ���
};
__align(4) const u8 DL698_Attribute_4204_3[]=
{
	4,//�����б���
//����3���ն˵���ַ�㲥Уʱ��������=structure
		DataType_structure,
//{
//     ʱ�������ֵ          unsigned����λ���룩��
		DataType_integer,
//     �ն˹㲥Уʱ����ʱ��  time��
		DataType_time,
//     �Ƿ�����              bool
		DataType_bool,
//}
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4204_3[]=//�ն˹㲥Уʱ-3
{
	0x42040300,//OAD
  ADDR_4204_3,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4204_3,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4204_3,//���������б�ָ��
	(u32*)InitData_4204_3,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4204_3),//��ʼ��ԭʼ���ݳ���
};






//0x4300�����豸
//1	�߼���	��ǽӿ������ʵ����ʹ��OI
//2	�豸������	visible-string
__align(4) const u8 ADDR_4300_2[]=
{
	DataType_visible_string,
	8,
	'T','e','r','m','i','n','a','l',
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_2[]=
{
	0x43000200,//OAD
  (u32)ADDR_4300_2,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
//3	�汾��Ϣ	�汾��Ϣ��=structure
//{
//���̴���       visible-string(SIZE (4))��
//����汾��     visible-string(SIZE (4))��
//����汾����   visible-string(SIZE (6))��
//Ӳ���汾��     visible-string(SIZE (4))��
//Ӳ���汾����   visible-string(SIZE (6))��
//������չ��Ϣ   visible-string(SIZE (8))
//}
__align(4) const u8 ADDR_4300_3[]=
{
	DataType_structure,
	6,
	DataType_visible_string,
	4,
	FactoryCode,//���̴���
	DataType_visible_string,
	4,
	'V',((SVER>>8)&0xf)+0x30,'.',((SVER>>4)&0xf)+0x30,//����汾��
	DataType_visible_string,
	6,
	((SVDATE>>20)&0xf)+0x30,((SVDATE>>16)&0xf)+0x30,((SVDATE>>12)&0xf)+0x30,((SVDATE>>8)&0xf)+0x30,((SVDATE>>4)&0xf)+0x30,((SVDATE>>0)&0xf)+0x30,//����汾����
	DataType_visible_string,
	4,
	((HVER>>12)&0xf)+0x30,((HVER>>8)&0xf)+0x30,((HVER>>4)&0xf)+0x30,((HVER>>0)&0xf)+0x30,//Ӳ���汾��
	DataType_visible_string,
	6,
	((HVDATE>>20)&0xf)+0x30,((HVDATE>>16)&0xf)+0x30,((HVDATE>>12)&0xf)+0x30,((HVDATE>>8)&0xf)+0x30,((HVDATE>>4)&0xf)+0x30,((HVDATE>>0)&0xf)+0x30,//Ӳ���汾����
	DataType_visible_string,
	8,
	FactoryInfoExt,//������չ��Ϣ
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_3[]=
{
	0x43000300,//OAD
  (u32)ADDR_4300_3,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
//4	��������	�豸�ĳ������ڣ����ݸ�ʽdate_time_s
__align(4) const u8 ADDR_4300_4[]=
{
	DataType_date_time_s,
	ProduceDate_year>>8,//����������(hex) 
	ProduceDate_year&0xff,
	ProduceDate_month,//����������(hex) 
	ProduceDate_day,//����������(hex) 
	ProduceDate_hour,//��������ʱ(hex)
	ProduceDate_minute,//�������ڷ�(hex)
	ProduceDate_second,//����������(hex)
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_4[]=
{
	0x43000400,//OAD
  (u32)ADDR_4300_4,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
//5	���豸�б�	array OI�������豸�������ģ����Ϣ
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
  (u32)ADDR_4300_5,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};
//6	֧�ֹ�Լ�б�	�豸֧�ֵĹ�Լ�б����ݸ�ʽ array visible-string
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
  (u32)ADDR_4300_6,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	0,//FLASHԤ������󳤶�
	1,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	0,//���������б�ָ��
	0,//��ʼ��ԭʼ����ָ��
	0,//��ʼ��ԭʼ���ݳ���
};

//7	��������ϱ�	True����������ϱ���False����ֹ�����ϱ���
__align(4) const u8 DL698_Attribute_4300_7[]=
{
	1,//�����б���
	DataType_bool,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_7[]=
{
	0x43000700,//OAD
  ADDR_4300_7,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4300_7,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4300_7,//���������б�ָ��
	(u32*)InitData_4300_7,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4300_7),//��ʼ��ԭʼ���ݳ���
};

//8	���������ϱ�	True�����������ϱ���False����ֹ�����ϱ���
__align(4) const u8 DL698_Attribute_4300_8[]=
{
	1,//�����б���
	DataType_bool,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_8[]=
{
	0x43000800,//OAD
  ADDR_4300_8,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4300_8,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4300_8,//���������б�ָ��
	(u32*)InitData_4300_8,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4300_8),//��ʼ��ԭʼ���ݳ���
};

//9	��������վͨ��	True������ͨ����False����ֹͨ����
__align(4) const u8 DL698_Attribute_4300_9[]=
{
	1,//�����б���
	DataType_bool,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_9[]=
{
	0x43000900,//OAD
  ADDR_4300_9,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4300_9,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4300_9,//���������б�ָ��
	(u32*)InitData_4300_9,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4300_9),//��ʼ��ԭʼ���ݳ���
};
//10	�ϱ�ͨ��	array OAD
__align(4) const u8 DL698_Attribute_4300_10[]=
{
	3,//�����б���
	DataType_array,
	1,//������������������͸���,��������=0ʱ�������͸�����
	DataType_OAD,
};
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_4300_10[]=
{
	0x43000A00,//OAD
  ADDR_4300_10,//���ݵ�ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_4300_10,//FLASHԤ������󳤶�
	7,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)DL698_Attribute_4300_10,//���������б�ָ��
	(u32*)InitData_4300_10,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_4300_10),//��ʼ��ԭʼ���ݳ���
};

#if (USER/100)==15//����
__align(4) const DL698_SETLIB_TypeDef  DL698_SETLIB_3106_11[]=//�ն�ͣ/�ϵ��¼�	
{
	0x31060B00,//�����ʶ
	ADDR_3106_11,//���ò�����ַ
	0,//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	LENmax_3106_11,//LENmax_4400,//FLASHԤ������󳤶�
	3,//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	(u32*)0,//���������б�ָ��
	(u32*)InitData_3106_11,//��ʼ��ԭʼ����ָ��
	sizeof(InitData_3106_11),//��ʼ��ԭʼ���ݳ���
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
