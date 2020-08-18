
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_CONNECT.h"
#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"




//Ӧ������
//�����б��=array һ���ɷ��ʶ���
//һ���ɷ��ʶ����=structure
//{
//   �����ʶ  OI��
//   ����Ȩ��  structure
//}
//����Ȩ�ޡ�=structure
//{
//���Է���Ȩ��  array һ�����Է���Ȩ�ޣ�
//��������Ȩ��  array һ����������Ȩ��
//}
//һ�����Է���Ȩ�ޡ�=structure
//{
//����ID           unsigned��
//���Է���Ȩ�����  enum
//{
//���ɷ��ʣ�0���� 
//ֻ����1����
//ֻд��2����
//�ɶ�д��3��
//}
//}
//һ����������Ȩ�ޡ�=structure
//{
//����ID        unsigned��
//��������Ȩ��   bool
//}
__align(4) const u8 ConnectAppOI_LIST[]=
{
	DataType_array,0x82,
	(162>>8),162&0xff,//���б����
//1
	DataType_structure,2,
	DataType_OI,0x00,0x00,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//2
	DataType_structure,2,
	DataType_OI,0x00,0x10,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//3
	DataType_structure,2,
	DataType_OI,0x00,0x11,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//4	
	DataType_structure,2,
	DataType_OI,0x00,0x12,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//5	
	DataType_structure,2,
	DataType_OI,0x00,0x13,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//6
	DataType_structure,2,
	DataType_OI,0x00,0x20,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//7
	DataType_structure,2,
	DataType_OI,0x00,0x21,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//8
	DataType_structure,2,
	DataType_OI,0x00,0x22,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//9
	DataType_structure,2,
	DataType_OI,0x00,0x23,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//10
	DataType_structure,2,
	DataType_OI,0x00,0x30,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//11
	DataType_structure,2,
	DataType_OI,0x00,0x31,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//12
	DataType_structure,2,
	DataType_OI,0x00,0x32,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//13
	DataType_structure,2,
	DataType_OI,0x00,0x33,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//14
	DataType_structure,2,
	DataType_OI,0x00,0x40,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//15
	DataType_structure,2,
	DataType_OI,0x00,0x41,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//16
	DataType_structure,2,
	DataType_OI,0x00,0x42,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//17
	DataType_structure,2,
	DataType_OI,0x00,0x43,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//18
	DataType_structure,2,
	DataType_OI,0x00,0x50,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//19
	DataType_structure,2,
	DataType_OI,0x00,0x51,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//20
	DataType_structure,2,
	DataType_OI,0x00,0x52,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//21
	DataType_structure,2,
	DataType_OI,0x00,0x53,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//22
	DataType_structure,2,
	DataType_OI,0x00,0x60,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//23
	DataType_structure,2,
	DataType_OI,0x00,0x61,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//24
	DataType_structure,2,
	DataType_OI,0x00,0x62,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//25
	DataType_structure,2,
	DataType_OI,0x00,0x63,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//26
	DataType_structure,2,
	DataType_OI,0x00,0x70,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//27
	DataType_structure,2,
	DataType_OI,0x00,0x71,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//28
	DataType_structure,2,
	DataType_OI,0x00,0x72,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//29
	DataType_structure,2,
	DataType_OI,0x00,0x73,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//30
	DataType_structure,2,
	DataType_OI,0x00,0x80,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//31
	DataType_structure,2,
	DataType_OI,0x00,0x81,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//32
	DataType_structure,2,
	DataType_OI,0x00,0x82,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//33
	DataType_structure,2,
	DataType_OI,0x00,0x83,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//34
	DataType_structure,2,
	DataType_OI,0x00,0x90,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//35
	DataType_structure,2,
	DataType_OI,0x00,0x91,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//36
	DataType_structure,2,
	DataType_OI,0x00,0x92,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//37
	DataType_structure,2,
	DataType_OI,0x00,0x93,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//38
	DataType_structure,2,
	DataType_OI,0x00,0xA0,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//39
	DataType_structure,2,
	DataType_OI,0x00,0xA1,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//40
	DataType_structure,2,
	DataType_OI,0x00,0xA2,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//41
	DataType_structure,2,
	DataType_OI,0x00,0xA3,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//42
	DataType_structure,2,
	DataType_OI,0x01,0x10,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//43
	DataType_structure,2,
	DataType_OI,0x01,0x11,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//44
	DataType_structure,2,
	DataType_OI,0x01,0x12,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//45
	DataType_structure,2,
	DataType_OI,0x01,0x13,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//46
	DataType_structure,2,
	DataType_OI,0x01,0x20,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//47
	DataType_structure,2,
	DataType_OI,0x01,0x21,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//48
	DataType_structure,2,
	DataType_OI,0x01,0x22,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//49
	DataType_structure,2,
	DataType_OI,0x01,0x23,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//50
	DataType_structure,2,
	DataType_OI,0x02,0x10,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//51
	DataType_structure,2,
	DataType_OI,0x02,0x11,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//52
	DataType_structure,2,
	DataType_OI,0x02,0x12,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//53
	DataType_structure,2,
	DataType_OI,0x02,0x13,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//54
	DataType_structure,2,
	DataType_OI,0x02,0x20,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//55
	DataType_structure,2,
	DataType_OI,0x02,0x21,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//56
	DataType_structure,2,
	DataType_OI,0x02,0x22,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//57
	DataType_structure,2,
	DataType_OI,0x02,0x23,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//58
	DataType_structure,2,
	DataType_OI,0x03,0x00,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//59
	DataType_structure,2,
	DataType_OI,0x03,0x01,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//60
	DataType_structure,2,
	DataType_OI,0x03,0x02,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//61
	DataType_structure,2,
	DataType_OI,0x03,0x03,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//62
	DataType_structure,2,
	DataType_OI,0x04,0x00,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//63
	DataType_structure,2,
	DataType_OI,0x04,0x01,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//64
	DataType_structure,2,
	DataType_OI,0x04,0x02,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//65
	DataType_structure,2,
	DataType_OI,0x04,0x03,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//66
	DataType_structure,2,
	DataType_OI,0x05,0x00,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//67
	DataType_structure,2,
	DataType_OI,0x05,0x01,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//68
	DataType_structure,2,
	DataType_OI,0x05,0x02,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//69
	DataType_structure,2,
	DataType_OI,0x05,0x03,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//70
	DataType_structure,2,
	DataType_OI,0x10,0x10,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//71
	DataType_structure,2,
	DataType_OI,0x10,0x11,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//72
	DataType_structure,2,
	DataType_OI,0x10,0x12,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//73
	DataType_structure,2,
	DataType_OI,0x10,0x13,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//74
	DataType_structure,2,
	DataType_OI,0x10,0x20,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//75
	DataType_structure,2,
	DataType_OI,0x10,0x21,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//76
	DataType_structure,2,
	DataType_OI,0x10,0x22,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//77
	DataType_structure,2,
	DataType_OI,0x10,0x23,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//78
	DataType_structure,2,
	DataType_OI,0x10,0x30,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//79
	DataType_structure,2,
	DataType_OI,0x10,0x31,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//80
	DataType_structure,2,
	DataType_OI,0x10,0x32,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//81
	DataType_structure,2,
	DataType_OI,0x10,0x33,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//82
	DataType_structure,2,
	DataType_OI,0x10,0x40,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//83
	DataType_structure,2,
	DataType_OI,0x10,0x41,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//84
	DataType_structure,2,
	DataType_OI,0x10,0x42,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//85
	DataType_structure,2,
	DataType_OI,0x10,0x43,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//86
	DataType_structure,2,
	DataType_OI,0x10,0x50,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//87
	DataType_structure,2,
	DataType_OI,0x10,0x51,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//88
	DataType_structure,2,
	DataType_OI,0x10,0x52,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//89
	DataType_structure,2,
	DataType_OI,0x10,0x53,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//90
	DataType_structure,2,
	DataType_OI,0x10,0x60,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//91
	DataType_structure,2,
	DataType_OI,0x10,0x61,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//92
	DataType_structure,2,
	DataType_OI,0x10,0x62,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//93
	DataType_structure,2,
	DataType_OI,0x10,0x63,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//94
	DataType_structure,2,
	DataType_OI,0x10,0x70,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//95
	DataType_structure,2,
	DataType_OI,0x10,0x71,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//96
	DataType_structure,2,
	DataType_OI,0x10,0x72,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//97
	DataType_structure,2,
	DataType_OI,0x10,0x73,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//98
	DataType_structure,2,
	DataType_OI,0x10,0x80,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//99
	DataType_structure,2,
	DataType_OI,0x10,0x81,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//100
	DataType_structure,2,
	DataType_OI,0x10,0x82,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//101
	DataType_structure,2,
	DataType_OI,0x10,0x83,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//102
	DataType_structure,2,
	DataType_OI,0x10,0x90,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//103
	DataType_structure,2,
	DataType_OI,0x10,0x91,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//104
	DataType_structure,2,
	DataType_OI,0x10,0x92,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//105
	DataType_structure,2,
	DataType_OI,0x10,0x93,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//106
	DataType_structure,2,
	DataType_OI,0x10,0xA0,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//107
	DataType_structure,2,
	DataType_OI,0x10,0xA1,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//108
	DataType_structure,2,
	DataType_OI,0x10,0xA2,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//109
	DataType_structure,2,
	DataType_OI,0x10,0xA3,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//110
	DataType_structure,2,
	DataType_OI,0x11,0x10,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//111
	DataType_structure,2,
	DataType_OI,0x11,0x11,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//112
	DataType_structure,2,
	DataType_OI,0x11,0x12,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//113
	DataType_structure,2,
	DataType_OI,0x11,0x13,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//114
	DataType_structure,2,
	DataType_OI,0x11,0x20,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//115
	DataType_structure,2,
	DataType_OI,0x11,0x21,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//116
	DataType_structure,2,
	DataType_OI,0x11,0x22,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//117
	DataType_structure,2,
	DataType_OI,0x11,0x23,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//118
	DataType_structure,2,
	DataType_OI,0x11,0x30,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//119
	DataType_structure,2,
	DataType_OI,0x11,0x31,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//120
	DataType_structure,2,
	DataType_OI,0x11,0x32,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//121
	DataType_structure,2,
	DataType_OI,0x11,0x33,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//122
	DataType_structure,2,
	DataType_OI,0x11,0x40,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//123
	DataType_structure,2,
	DataType_OI,0x11,0x41,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//124
	DataType_structure,2,
	DataType_OI,0x11,0x42,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//125
	DataType_structure,2,
	DataType_OI,0x11,0x43,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//126
	DataType_structure,2,
	DataType_OI,0x11,0x50,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//127
	DataType_structure,2,
	DataType_OI,0x11,0x51,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//128
	DataType_structure,2,
	DataType_OI,0x11,0x52,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//129
	DataType_structure,2,
	DataType_OI,0x11,0x53,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//130
	DataType_structure,2,
	DataType_OI,0x11,0x60,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//131
	DataType_structure,2,
	DataType_OI,0x11,0x61,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//132
	DataType_structure,2,
	DataType_OI,0x11,0x62,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//133
	DataType_structure,2,
	DataType_OI,0x11,0x63,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//134
	DataType_structure,2,
	DataType_OI,0x11,0x70,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//135
	DataType_structure,2,
	DataType_OI,0x11,0x71,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//136
	DataType_structure,2,
	DataType_OI,0x11,0x72,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//137
	DataType_structure,2,
	DataType_OI,0x11,0x73,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//138
	DataType_structure,2,
	DataType_OI,0x11,0x80,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//139
	DataType_structure,2,
	DataType_OI,0x11,0x81,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//140
	DataType_structure,2,
	DataType_OI,0x11,0x82,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//141
	DataType_structure,2,
	DataType_OI,0x11,0x83,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//142
	DataType_structure,2,
	DataType_OI,0x11,0x90,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//143
	DataType_structure,2,
	DataType_OI,0x11,0x91,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//144
	DataType_structure,2,
	DataType_OI,0x11,0x92,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//145
	DataType_structure,2,
	DataType_OI,0x11,0x93,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//146
	DataType_structure,2,
	DataType_OI,0x11,0xA0,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//147
	DataType_structure,2,
	DataType_OI,0x11,0xA1,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//148
	DataType_structure,2,
	DataType_OI,0x11,0xA2,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//149
	DataType_structure,2,
	DataType_OI,0x11,0xA3,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//150
	DataType_structure,2,
	DataType_OI,0x20,0x00,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//151
	DataType_structure,2,
	DataType_OI,0x20,0x01,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//152
	DataType_structure,2,
	DataType_OI,0x20,0x02,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//153
	DataType_structure,2,
	DataType_OI,0x20,0x03,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//154
	DataType_structure,2,
	DataType_OI,0x20,0x04,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//155
	DataType_structure,2,
	DataType_OI,0x20,0x05,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//156
	DataType_structure,2,
	DataType_OI,0x20,0x06,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//157
	DataType_structure,2,
	DataType_OI,0x20,0x07,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//158
	DataType_structure,2,
	DataType_OI,0x20,0x08,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//159
	DataType_structure,2,
	DataType_OI,0x20,0x09,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//160
	DataType_structure,2,
	DataType_OI,0x20,0x0A,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//161
	DataType_structure,2,
	DataType_OI,0x20,0x0B,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//162
	DataType_structure,2,
	DataType_OI,0x20,0x0C,//�����ʶ  OI
	DataType_structure,2,//����Ȩ��
	//���Է���Ȩ��  array һ�����Է���Ȩ��
	DataType_array,
	2,//n�����Է���Ȩ��
	//��1�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	2,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	//��2�����Է���Ȩ��
	DataType_structure,2,
	DataType_unsigned,
	3,//����ID
	DataType_enum,
	1,//0=���ɷ���,1=ֻ��,2=ֻд,3=�ɶ�д
	
	//��������Ȩ��  array һ����������Ȩ��
	DataType_array,
	0,//n��������Ȩ��
	//����ID
	//��������Ȩ��   bool
//163

	
	
	
	
	
	
};


//Ӧ���ﾳ��Ϣ��=structure
//{
//Э��汾��Ϣ        long-unsigned��
//������APDU�ߴ�   long-unsigned��
//�����APDU�ߴ�   long-unsigned��
//���ɴ���APDU�ߴ� long-unsigned��
//Э��һ���Կ�        bit-string(64)��
//����һ���Կ�        bit-string(128)��
//��̬��ʱʱ��       double-long-unsigned
//}
const u8 UART_SIZE_LIST[]=
{
	DataType_structure,7,
//Э��汾��Ϣ        long-unsigned��
	DataType_long_unsigned,
	SVER>>8,SVER&0xff,
//������APDU�ߴ�   long-unsigned��
	DataType_long_unsigned,
	LENmax_APDU>>8,LENmax_APDU&0xff,
//�����APDU�ߴ�   long-unsigned��
	DataType_long_unsigned,
	2048>>8,2048&0xff,
//���ɴ���APDU�ߴ� long-unsigned��
	DataType_long_unsigned,
	LENmax_APDU>>8,LENmax_APDU&0xff,
//Э��һ���Կ�        bit-string(64)��
	DataType_bit_string,64,
	(Protocol0<<7)+(Protocol1<<6)+(Protocol2<<5)+(Protocol3<<4)+(Protocol4<<3)+(Protocol5<<2)+(Protocol6<<1)+(Protocol7<<0),
	(Protocol8<<7)+(Protocol9<<6)+(Protocol10<<5)+(Protocol11<<4)+(Protocol12<<3)+(Protocol13<<2)+(Protocol14<<1)+(Protocol15<<0),
	(Protocol16<<7)+(Protocol17<<6)+(Protocol18<<5)+(Protocol19<<4)+(Protocol20<<3)+(Protocol21<<2)+(Protocol22<<1)+(Protocol23<<0),
	(Protocol24<<7)+(Protocol25<<6)+(Protocol26<<5)+(Protocol27<<4)+(Protocol28<<3)+(Protocol29<<2)+(Protocol30<<1)+(Protocol31<<0),
	(Protocol32<<7)+(Protocol33<<6)+(Protocol34<<5)+(Protocol35<<4)+(Protocol36<<3)+(Protocol37<<2)+(Protocol38<<1)+(Protocol39<<0),
	0,
	0,
	0,
//����һ���Կ�        bit-string(128)��
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
//��̬��ʱʱ��       double-long-unsigned
	DataType_double_long_unsigned,
	0,0,2000>>8,2000&0xff,
};



u32 GET_OAD_ConnectApp(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//��Ӧ�����ӿɷ��ʶ����б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
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
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	switch(OAD)
	{
		case 0x44000200://Ӧ������:�����б��=array һ���ɷ��ʶ���
			p8tx[0]=DataType_array;
			Len=2;
			Array=0;//����������
			NUMmax=(ConnectAppOI_LIST[2]<<8)|ConnectAppOI_LIST[3];
			n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
			p8=(u8*)ConnectAppOI_LIST+4;
			for(i=0;i<n;i++)
			{
				p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
			}
			while(1)
			{
				if(n>=NUMmax)
				{
					break;
				}
				x=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				if(x>LENmax_TxSPACE)
				{//���ͻ��岻��,��֡
					if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					{
						UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					else
					{
						UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
					p8tx[1]=Array;//������������
					return Len;
				}
				MR((u32)p8tx+Len,(u32)p8,x);
				p8+=x;
				Len+=x;
				LENmax_TxSPACE-=x;
				Array++;//������������
				n++;
			}
			if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			{
				UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			}
			UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
			p8tx[1]=Array;//������������
			return Len;
		case 0x44000300://Ӧ������:Ӧ���ﾳ��Ϣ
			Len=sizeof(UART_SIZE_LIST);
			if(LENmax_TxSPACE<Len)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			MR((u32)p8tx,(u32)UART_SIZE_LIST,Len);
			return Len;
		case 0x44000400://Ӧ������:��ǰ���ӵĿͻ�����ַ
			//��ǰ���ӵĿͻ�����ַ��=unsigned
			if(LENmax_TxSPACE<2)
			{
				GET_Request_Frame(PORTn);//��֡��־
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
		case 0x44000500://Ӧ������:������֤����
			//������֤���ơ�=enum
			//{
			//  ��������   (0)��
			//  ��ͨ����   (1)��
			//  �ԳƼ���   (2)��
			//  ����ǩ��   (3)
			//}
			if(LENmax_TxSPACE<2)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_enum;
			p8tx[1]=3;
			return 2;
		default:
			UARTCtrl->DAR=6;//���ݷ��ʽ��
			return 0;
	}
}

#include "../MS/MS.h"
void TEST_ConnectAppOI_LIST(void)//���Զ����б�
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
		x=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
		p8+=x;
		Len+=x;
	}
	Len+=4;
	x=sizeof(ConnectAppOI_LIST);
	if(Len!=x)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		DisplayStringAutoUp(0,9,(u8*)"\x0""��Ӧ�ö����б��");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
}





