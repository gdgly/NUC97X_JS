
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_UART.h"

#include "../DL698/Contrl_Manage.h"

#include "../DL698/DL698_JSON.h"
//#include "../DL698/cJSON.h"
#include "../Calculate/ascii.h"
#include "../Calculate/Calculate.h"

#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#pragma  diag_suppress 870     //��ֹKEIL������
//�����ϱ�ȫ��ʵʱ���ݵ�OAD
__align(4) const u16 fcstab[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

const u32 RealDataOAD_JSON[]=//ʵʱJSON�ļ������������
{
	0x40020200,	//�ɼ��洢ʱ��
	0x20000201,	//A���ѹ
	0x20000202,	//B���ѹ
	0x20000203,	//C���ѹ

	0x20010201,	//A�����
	0x20010202,	//B�����
	0x20010203,	//C�����

	0x20010400,	//���ߵ���
	0x20010500,	//ʣ�����

	0x20040201,	//���й�����
	0x20040202,	//A���й�����
	0x20040203,	//B���й�����
	0x20040204,	//C���й�����
	
	0x43140506,	//(�����¶�)
	0x43140505,	//(�����¶�)
	0x43140501,	//(�յ������¶�)
	0x43140400,	//(�յ����ػ�״̬)
	0x43110502,	//(��ˮ�������¶�)
	0x43110400,	//(��ˮ�����ػ�״̬)

	0x00100201, //������
	0x00100203,	//���з�
	0x00100205,	//���й�

	0x00200201, //������
	0x00200203,	//���з�
	0x00200205,	//���й�
};		
//�����ϱ�ȫ��ʵʱ���ݵ�OAD����
const u32 RealDataOAD_JSONSIZE=sizeof(RealDataOAD_JSON)/sizeof(u32);

typedef struct
{
	u16 OI;
	u16 POINTNUM;//С����λ��
}POINTNUM_type;

const POINTNUM_type OAD_PointNUM_list[]=//OAD��ӦС����λ��
{
	{0x2000,	1},//	��ѹ	�������ͣ�long-unsigned����λ��V�����㣺-1
	{0x2001,	3},//	����	�������ͣ�double-long����λ��A ���㣺-3
	{0x2002,	1},//	��ѹ���	�������ͣ�long-unsigned����λ���ȣ����㣺-1
	{0x2003,	1},//	��ѹ�������	�������ͣ�long-unsigned����λ���ȣ����㣺-1
	{0x2004,	1},//	�й�����	�������ͣ�double-long����λ��W�����㣺-1
	{0x2005,	1},//	�޹�����	�������ͣ�double-long����λ��var�����㣺-1
	{0x2006,	1},//	���ڹ���	�������ͣ�double-long����λ��VA�����㣺-1
	{0x2007,	1},//	һ����ƽ���й����� �������ͣ�double-long����λ��W�����㣺-1
	{0x2008,	1},//	һ����ƽ���޹����� �������ͣ�double-long����λ��var�����㣺-1
	{0x2009,	1},//	һ����ƽ�����ڹ��� �������ͣ�double-long����λ��VA�����㣺-1
	{0x200A,	3},//	��������	�������ͣ�long����λ���ޣ����㣺-3
	{0x200B,	2},//	��ѹ����ʧ���	�������ͣ�long����λ��%�����㣺-2
	{0x200C,	2},//	��������ʧ���	�������ͣ�long����λ��%�����㣺-2
	{0x200D,	2},//	��ѹг�����������ܼ� 2?n �Σ� �������ͣ�long����λ��%�����㣺-2
	{0x200E,	2},//	����г�����������ܼ� 2?n �Σ� �������ͣ�long����λ��%�����㣺-2
	{0x200F,	2},//	����Ƶ��	�������ͣ�long-unsigned����λ��Hz�����㣺-2
	 
	
	//�����ʶ OI �ӿ��� IC	��������	ʵ���Ķ������Լ���������
	{0x2010,	1},//	�����¶�	�������ͣ�long����λ���棬���㣺-1
	{0x2011,	2},//	ʱ�ӵ�ص�ѹ	�������ͣ�long-unsigned����λ��V�����㣺-2
	{0x2012,	2},//	ͣ�糭���ص�ѹ	�������ͣ�long-unsigned����λ��V�����㣺-2
	//{0x2013,	0},//	ʱ�ӵ�ع���ʱ��	�������ͣ�double-long-unsigned����λ�����ӣ��޻���
	//2014	6	���ܱ�����״̬��	�������ͣ�array bit-string���޵�λ���޻��㣬�������� ������״̬�� 1��7������  ¼ G
	
	//2015	6	���ܱ�����ϱ�״̬��	�������ͣ�bit-string(SIZE(32))���޵�λ���޻��㣬���� ¼  G
	//���� 4�����ܱ�����ϱ�ģʽ�֣���= bit-string(SIZE(32)) ���� 127��ȷ�ϵ��ܱ�����ϱ�״̬�֣���=
	//bit-string(SIZE(32))
	{0x2017,	4},//	��ǰ�й�����	�������ͣ�double-long����λ��kW�����㣺-4
	{0x2018,	4},//	��ǰ�޹�����	�������ͣ�double-long����λ��kvar�����㣺-4
	{0x2019,	4},//	��ǰ��������	�������ͣ�double-long����λ��kVA�����㣺-4
	{0x201A,	4},//	��ǰ���	�������ͣ�double-long-unsigned����λ��Ԫ/kWh�����㣺-4
	{0x201B,	4},//	��ǰ���ʵ��	�������ͣ�double-long-unsigned����λ��Ԫ/kWh�����㣺-4
	{0x201C,	4},//	��ǰ���ݵ��	�������ͣ�double-long-unsigned����λ��Ԫ/kWh�����㣺-4
	//201E	8	�¼�����ʱ��	�������ͣ�date_time_s
	//2020	8	�¼�����ʱ��	�������ͣ�date_time_s
	//2021	8	���ݶ���ʱ��	�������ͣ�date_time_s
	//2022	8	�¼���¼���	�������ͣ�double-long-unsigned
	//2023	8	�����¼���	�������ͣ�double-long-unsigned
	//2024	8	�¼�����Դ ��������塣
	//2025	8	�¼���ǰֵ structure
	//{
	//�¼��������� double-long-unsigned��
	//�¼��ۼ�ʱ�� double-long-unsigned����λ���룬�޻� �㣩
	//}
	{0x2026,	2},//	��ѹ��ƽ����	�������ͣ�long-unsigned����λ��%�����㣺-2
	{0x2027,	2},//	������ƽ����	�������ͣ�long-unsigned����λ��%�����㣺-2
	{0x2028,	2},//	������	�������ͣ�long-unsigned����λ��%�����㣺-2
	{0x2029,	2},//	��ʱֵ	���� 2 ��ʱ��ֵ��=array �లʱֵ�లʱֵ��=double-long-unsigned����λ��Ah�����㣺-2 �లʱֵ���ܡ�A��B��C �ࡣ
	//202A	8	Ŀ���������ַ	���� 2��=TSA
	 
	
	//�����ʶ OI �ӿ��� IC	��������	ʵ���Ķ������Լ���������
	{0x202C,	2},//	����ǰ��Ǯ���ļ�	��ֵ��=structure
	//{
	//ʣ����	double-long-unsigned����λ��Ԫ�����㣺-2���� �������	double-long-unsigned
	//}
	{0x202D,	2},//	����ǰ��͸֧���	�������ͣ�double-long-unsigned����λ��Ԫ�����㣺-2
	{0x202E,	2},//	�ۼƹ�����	�������ͣ�double-long-unsigned����λ��Ԫ�����㣺-2
	{0x2031,	2},//	�¶��õ��� ���� 2 �õ�����=double-long-unsigned�� ��λ��kWh�����㣺-2
	{0x2032,	2},//	���ݽ����õ���	���� 2 �õ�����=double-long-unsigned�� ��λ��kWh�����㣺-2
	//2040	6	��������ִ��״̬�� �������ͣ�bit-string(SIZE(16))���޵�λ���޻���
	//2041	6	�����������״̬�� �������ͣ�bit-string(SIZE(16))���޵�λ���޻���
	//2100	14	��������ͳ��	ͳ�����ڵ�λΪ����
	//2101	14	Сʱ����ͳ��	ͳ�����ڵ�λΪСʱ
	//2102	14	������ͳ�� ͳ�����ڵ�λΪ��
	//2103	14	������ͳ�� ͳ�����ڵ�λΪ��
	//2104	14	������ͳ�� ͳ�����ڵ�λΪ��
	//2110	15	����ƽ��	ͳ�����ڵ�λΪ����
	//2111	15	Сʱƽ��	ͳ�����ڵ�λΪʱ
	//2112	15	��ƽ��	ͳ�����ڵ�λΪ��
	//2113	15	��ƽ��	ͳ�����ڵ�λΪ��
	//2114	15	��ƽ��	ͳ�����ڵ�λΪ��
	//2120	16	���Ӽ�ֵ	ͳ�����ڵ�λΪ����
	//2121	16	Сʱ��ֵ	ͳ�����ڵ�λΪʱ
	//2122	16	�ռ�ֵ	ͳ�����ڵ�λΪ��
	//2123	16	�¼�ֵ	ͳ�����ڵ�λΪ��
	//2124	16	�꼫ֵ	ͳ�����ڵ�λΪ��
	//2131	6	���� A ���ѹ�ϸ��� ���� 2����ѹ�ϸ������ݣ���=structure
	//{
	//���յ�ѹ�ϸ���	��ѹ�ϸ��ʣ�
	//���µ�ѹ�ϸ���	��ѹ�ϸ���
	//}
	
	
	//��ѹ�ϸ��ʡ�=structure
	//{
	//��ѹ���ʱ�� double-long-unsigned����λ�����ӣ��޻� �㣩��
	 
	
	//�����ʶ OI �ӿ��� IC	��������	ʵ���Ķ������Լ���������
	{0x2131,	2},//	���� A ���ѹ�ϸ��� ��ѹ�ϸ��� long-unsigned����λ��%�����㣺-2���� ��ѹ������	long-unsigned����λ��%�����㣺-2���� ��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩����ѹ������ʱ�� double-long-unsigned����λ�����ӣ� �޻��㣩}
	{0x2132,	2},//	���� B ���ѹ�ϸ��� ͬ 2131
	{0x2133,	2},//	���� C ���ѹ�ϸ��� ͬ 2131
	//2140	2	������й����ʼ�����ʱ��	����ʼ�����ʱ���=structure
	//{
	//�����ֵ double-long-unsigned�� ����ʱ��	date_time_s
	//}
	//���ʵ�λ��kW�����㣺-4
	{0x2141,	4},//	������й����ʼ�����ʱ��	ͬ 2140��
	//2200	6	ͨ������	��ֵ��=structure
	//{
	//����ͨ������ double-long-unsigned�� ����ͨ������ double-long-unsigned
	//}
	//��λ��byte�����㣺0
	//2203	6	����ʱ��	��ֵ��=structure
	//{
	//�չ����ۼ�ʱ�� double-long-unsigned�� �¹����ۼ�ʱ�� double-long-unsigned
	//}
	//��λ�����ӣ����㣺0
	//2204	6	��λ����	��ֵ��=structure
	//{
	//�ո�λ�ۼƴ��� long-unsigned�� �¸�λ�ۼƴ��� long-unsigned
	//}
	//2301	23	�ܼ��� 1 
	//2302	23	�ܼ��� 2 
	//2303	23	�ܼ��� 3 
	//2304	23	�ܼ��� 4 
	//2305	23	�ܼ��� 5 
	 
	
	//�����ʶ OI �ӿ��� IC	��������	ʵ���Ķ������Լ���������
	//2306	23	�ܼ��� 6 
	//2307	23	�ܼ��� 7 
	//2308	23	�ܼ��� 8 
	//2401	12	������� 1	
	//2402	12	������� 2	
	//2403	12	������� 3	
	//2404	12	������� 4	
	//2405	12	������� 5	
	//2406	12	������� 6	
	//2407	12	������� 7	
	//2408	12	������� 8	
	//2500	6	�ۼ�ˮ���ȣ�����	�������ͣ�double-long-unsigned����λ��m3�����㣺-4
	//��ע��ˮ����ȱ��ã�
	{0x2501,	4},//	�ۼ������� �������ͣ�double-long-unsigned����λ��m3�����㣺-4
	{0x2502,	2},//	�ۼ�����	�������ͣ�double-long-unsigned����λ��J�����㣺-2
	{0x2503,	2},//	�ȹ���	�������ͣ�double-long-unsigned����λ��J/h�����㣺-2
	//2504	6	�ۼƹ���ʱ��	�������ͣ�double-long-unsigned����λ��Сʱ�����㣺0
	{0x2505,	2},//	ˮ��	��ֵ��=structure
	//{
	//��ˮ�¶� double-long-unsigned�� ��ˮ�¶� double-long-unsigned
	//}
	//��λ���棬���㣺-2��
	//2506	6	���Ǳ�״̬ ST	��ֵ��=structure
	///{
	//����״̬  enum{����0�����أ�1����������2�����쳣��3��}�� ��ص�ѹ  enum{������0����Ƿѹ��0��}
	//}
};
const u32 OAD_PointNUMSIZE=sizeof(OAD_PointNUM_list)/sizeof(POINTNUM_type);



u32 Str_CRandLF(u8 *p)//�س��ͻ���
{
#ifdef UNFORMAT_JSON
	*p=0x0D;
	p++;
	*p=0x0A;

	return 2;
#else
	return 0;
#endif
}

u32 Str_CR(u8 *p)//�س�
{
	*p=0x0D;
	return 1;
}

u32 Str_LF(u8 *p)//����
{
	*p=0x0A;
	return 1;
}

u32 Str_SPASE(u8 *p)//�ո�
{
	*p=0x20;
	return 1;
}

u32 Str_nSPASE(u8 *p,u32 NUM)//�ո�
{
#ifdef UNFORMAT_JSON
	u32 i;
	for(i=0;i<NUM;i++)
	{
		*p=0x20;
		p++;
	}
	return NUM;
#else
	return 0;
#endif
}

u32 Str_COMMA(u8 *p)//����
{
	*p=0x2C;
	return 1;
}

u32 Str_QM(u8 *p)//����
{
	*p=0x22;
	return 1;
}

u32 Str_COL(u8 *p)//ð��
{
	*p=0x3A;
	return 1;
}
u32 Str_L_SQB(u8 *p)//������
{
	*p=0x5B;
	return 1;
}

u32 Str_R_SQB(u8 *p)//�ҷ�����
{
	*p=0x5D;
	return 1;
}
u32 Str_L_CUB(u8 *p)//������
{
	*p=0x7B;
	return 1;
}

u32 Str_R_CUB(u8 *p)//�һ�����
{
	*p=0x7D;
	return 1;
}

void myprintf(char *pStr,u32 param1,u32 param2,u32 param3)
{
#ifdef DEBUG_JSON
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf((char *)pStr,param1,param2,param3);
#endif
}

u32 mystrlen(const char *p)//�����ַ�������
{
	u32 i;
	i=0;
	while((*p!='\0')||(*p!=0))
	{
		i++;
		p++;
	}
	return i;
}

void ByteInverte(u8 *ps,u8 *pc,u32 Len)//ps��Len�ֽڵ��������pc��
{
	u32 i;
	u32 j;
	j=0;
	for(i=Len;i>0;i--)
	{
		pc[j]=ps[i-1];
		j++;
	}
}
u32 mymemcmp(u8 *p,u32 val,u32 Len)//�Ƚ���������,1����ͬ,0��ͬ
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		if(*p!=val)
		{
			return 1;
		}
		p++;
	}
	return 0;
}
void mystrcpy(char *p,const char *pc)//pc�ֽڿ�����p����ȥ
{
	u32 pclen;
	u32 i;
	pclen=mystrlen(pc);
	for(i=0;i<pclen;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}
void mystrncpy(char *p,const char *pc,u32 Len)//pc�ֽڿ���n����p����ȥ
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}
void mystrncat(char *p,const char *pc,u32 Len)//p��������Len����pc�ַ�
{
	u32 plen;
	//u32 pclen;
	u32 i;
	plen=mystrlen(p);
	//pclen=mystrlen(pc);
	p+=plen;
	for(i=0;i<Len;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}

u32 Data_Ascii(u64 ida,u8 *p,u32 NUM)//��idaֵ�浽p��,NUMΪС�����λ��
{
	u32 i;
	u64 data;
	u32 Len=0;
	u8 tm;
	data=hex_bcd16(ida);
	if(0==data)
	{	
		*p=0x30;
		p++;
		Len++;
		for(i=0;i<NUM;i++)
		{			
			if(0==i)
			{
				*p='.';	
				p++;
				Len++;
				*p=0x30;
				Len++;
			}else
			{
				*p=0x30;
				Len++;
			}
			p++;
		}
	}else
	{
		for(i=16;i>0;i--)
		{
			tm=(data>>((i-1)*4))&0xf;
			if(NUM==i)
			{
				*p='.';	
				p++;	
				Len++;
			}
			if(0!=tm)
			{
				*p=tm+0x30;
				p++;
				Len++;
			}else
			{
				if(Len!=0||NUM+1==i)
				{
					*p=tm+0x30;
					p++;
					Len++;
				}
			}
		}
	}
	*p=0;
	return Len;
}

void mystrcat(char *p,const char *pc)//p�ַ�����pc��p
{
	u32 plen;
	u32 pclen;
	u32 i;
	plen=mystrlen(p);
	pclen=mystrlen(pc);
	p+=plen;
	for(i=0;i<pclen;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}

u32 Cmp_Ascii_Inverte(u8 *p,u8 *pc,u32 Byte)//�ֽ�pcת��asc�뵹���p�Ƚ�
{
	u32 i;
	u8 data;
	u32 x;
	for(i=Byte;i>0;i--)
	{
		data=pc[i-1];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

u32 Cmp_Ascii(u8 *p,u8 *pc,u32 Byte)//�ֽ�pcת��asc���p�Ƚ�
{
	u32 i;
	u8 data;
	u32 x;
	for(i=0;i<Byte;i++)
	{
		data=pc[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

void Add_StrToASCII_Inverte(u8 *pStr,u8 *pbuf,u32 Byte)//pbuf������ֽڵ��������pstr��ת��asc��
{
	u32 i;
	u32 j=0;
	u8 data;
	u32 x;
	for(i=Byte;i>0;i--)
	{
		data=pbuf[i-1];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		
	}
	pStr[j]=0;
}

void Add_StrToASCII(u8 *pStr,u8 *pbuf,u32 Byte)//pbuf������ֽ������pstr��ת��asc��
{
	u32 i;
	u32 j=0;
	u8 data;
	u32 x;
	for(i=0;i<Byte;i++)
	{
		data=pbuf[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		
	}
	pStr[j]=0;
}

void Get698CurTime_Ascii(u8 *p8,u32 subD)//�õ�698��ǰʱ��ת����asc��,��ȥsubD
{
	u32 i;
	u32 x;
	u8 data;
	u8 *p8s;
	//дʱ��
	//��
	u32 YYMD;
	YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
	if(subD!=0)
	{
		for(i=0;i<subD;i++)
			YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������	
	}
	
	p8s=(u8*)ADDR_4KWRITE_BUFF;	
	p8s[0]=YYMD>>24;
	p8s[1]=YYMD>>16;
	p8s[2]=YYMD>>8;
	p8s[3]=YYMD>>0;
	p8s[4]=Comm_Ram->DL698YMDHMS[4];//ʱ��:ʱ
	p8s[5]=Comm_Ram->DL698YMDHMS[5];//ʱ��:��
	p8s[6]=Comm_Ram->DL698YMDHMS[5];//ʱ��:��
	//x=(Comm_Ram->DL698YMDHMS[0]<<8)|Comm_Ram->DL698YMDHMS[1];
	x=(p8s[0]<<8)|p8s[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<7;i++)
	{
		data=p8s[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	*p8=0;
}

void Get698Time_Ascii(u8 *p,u8 *p8)//pʱ��ǰ�����ֽڱ�ʾ���p8���asc��
{
	u32 i;
	u32 x;
	u8 data;
	//дʱ��
	//��
	x=(p[0]<<8)|p[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<7;i++)
	{
		data=p[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	*p8=0;
}

u32 Cmp698Time_Ascii(u8 *p,u8 *pc,u32 Byte)//698ʱ��Ƚ�pcǰ�����ֽڴ������
{
	u32 i;
	u8 data;
	u32 x;
	u8 ptim[4]={0};
	//дʱ��
	//��
	x=(pc[0]<<8)|pc[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(ptim,(u8 *)&x,2);
	//�ȱȽ����
	for(i=0;i<4;i++)
	{
		if(*p!=ptim[i])
		{
			return 1;
		}
	}
	for(i=2;i<Byte;i++)
	{
		data=pc[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

void Get698Time_HMS_Ascii(u8 *pt,u8 *p8,u8 hour,u8 min,u8 sec)//pʱ��ǰ�����ֽڱ�ʾ���p8���asc��
{
	u32 i;
	u32 x;
	u8 data;
	//дʱ��
	//��
	//x=(Comm_Ram->DL698YMDHMS[0]<<8)|Comm_Ram->DL698YMDHMS[1];
	x=(pt[0]<<8)|pt[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<4;i++)
	{
		//data=Comm_Ram->DL698YMDHMS[i];
		data=pt[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	data=hex_bcd(hour);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	data=hex_bcd(min);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	data=hex_bcd(sec);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	*p8=0;
}
void GetNullTime(u8 *pt)//�õ�һ��JSON��ʽҪ��Ŀ�ʱ���ʽ
{
	u32 offset;
	offset=0;
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]='-';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]='-';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=' ';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=':';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=':';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=0;
}
void GetJsonTime(u8 *pt)//�Ѵ���õ�ʱ��buf ptת����JSON�ļ������ʱ���ʽ��2020-05-01 20:00:00��
{
	u8 tt[14];
	MW((u32)pt,(u32)tt,sizeof(tt));
	//��
	pt[0]=tt[0];
	pt[1]=tt[1];
	pt[2]=tt[2];
	pt[3]=tt[3];	
	//��
	pt[4]='-';
	pt[5]=tt[4];
	pt[6]=tt[5];
	//��
	pt[7]='-';
	pt[8]=tt[6];
	pt[9]=tt[7];

	pt[10]=' ';
	//ʱ
	pt[11]=tt[8];
	pt[12]=tt[9];
	pt[13]=':';
	//��
	pt[14]=tt[10];
	pt[15]=tt[11];
	pt[16]=':';
	//��
	pt[17]=tt[12];
	pt[18]=tt[13];
	pt[19]=0;
}

void WriteNullString(u8 *p8)//����BUF����дASC ��NULL
{
	p8[0]='n';
	p8[1]='u';
	p8[2]='l';
	p8[3]='l';
	p8[4]=0;	
}

u32 Get_DataPointNUM(u16 OI)//��OAD_PointNUM_list�������OAD���õ����OAD��С��λ��
{
	u32 i;
	u32 ret=0;
	for(i=0;i<OAD_PointNUMSIZE;i++)
	{
		if(OAD_PointNUM_list[i].OI==OI)
		{
			ret=OAD_PointNUM_list[i].POINTNUM;
			break;
		}
	}
	return ret;
}
u32 DealMetData_Ascii(u16 OI,u8 *pdata,u8 *pout,u32 isReal)//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
{
	u32 i;
	u32 datalen;
	u32 pointnum;
	u64 data;
	i=Get_DL698DataLen_S(pdata,0);
	if(0==i)
	{		
		return i;
	}
	if(pdata[0])
	{
		i--;
		data=0;
		if(i>8)
		{
			i=8;
		}
		datalen=i;
		while(i>0)
		{
			pdata++;
			i--;
			data|=((*pdata)<<(i*8));
		}
		if((data&0xffffffff)==0xffffffff || (data&0xffffffff)==0x7fffffff)
		{
			if(!isReal)
			{
				WriteNullString(pout);//����BUF����дASC ��NULL
				i=4;
			}else
			{
				pout[0]=0;
				i=1;
			}
		}else
		{
			switch (OI>>8)
			{
				case 0x00:
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
					pointnum=2;
					break;
				case 0x10:
				case 0x11:
					pointnum=4;
					break;
				default:
					pointnum=Get_DataPointNUM(OI);
					break;		
			}
			i=0;
			i=(data>>((datalen-1)*8));
			if(i>>4)//��������Ǹ���
			{
				*pout='-';
				pout++;
				i=(u32)data;
				data=(u64)(~(i)+1);
			}
			i=Data_Ascii(data,pout,pointnum);//��idaֵ�浽p��,NUMΪС�����λ��			
		}
	}else
	{
		if(!isReal)
		{
			WriteNullString(pout);//����BUF����дASC ��NULL
			i=4;
		}else
		{
			pout[0]=0;
			i=1;
		}
	}
	return i;
}

u32 Createitem_Json(u8 *pout,u8 *ph,u8 *pd,u32 spacenum)//����JSON��ʽ: "XXXXX":"XXXXXXX",
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//ð��
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)pd);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	i=Str_COMMA(pout+CJSONLEN);//����
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateitemNum_Json(u8 *pout,u8 *ph,u8 *pd,u32 spacenum)//����JSON��ʽ: "XXXXX":XXXXXXX,
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//ð��
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)pd);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_COMMA(pout+CJSONLEN);//����
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateitemArrayHead_Json(u8 *pout,u8 *ph,u32 spacenum)//����JSON��ʽ: "XXXXX":[
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//ð��
	CJSONLEN+=i;
	i=Str_L_SQB(pout+CJSONLEN);//������
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 CreateitemArrayEnd_Json(u8 *pout,u32 Len,u32 spacenum)//����JSON��ʽ: ],
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pout+Len+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+Len+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_R_SQB(pout+Len+CJSONLEN);//�ҷ�����
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//����
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}

u32 CreateArrayHead_Json(u8 *pout,u32 spacenum)//����JSON��ʽ: {
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_L_CUB(pout+CJSONLEN);//������
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 CreateArrayEnd_Json(u8 *pout,u32 Len,u32 spacenum)//����JSON��ʽ: },
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pout+Len+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+Len+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_R_CUB(pout+Len+CJSONLEN);//�һ�����
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//����
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}

u32 CreateDataHead_Json(u8 *pout,u8 *ph,u32 spacenum)//����JSON��ʽ: "XXXXXXX","
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//�س��ͻ���
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//�ո�
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//ð��
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//����
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateData_Json(u8 *pout,u8 *pd)//����JSON��ʽ: XXXX,
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	if(pd[0]!=0)
	{
		mystrcpy((char *)pout+CJSONLEN, (const char *)pd);
		i=mystrlen((const char *)pout+CJSONLEN);
		CJSONLEN+=i;
	}
	i=Str_COMMA(pout+CJSONLEN);//����
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateDataEnd_Json(u8 *pout,u32 Len)//����JSON��ʽ: ",
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	//CJSONLEN=Len;
	i=Str_QM(pout+Len+CJSONLEN);//����
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//����
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}
u32 Delet_COMMA(u8 *pout,u32 Len)//ɾ������,�����ܳ�
{
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
		}
	}
	return Len;
}
u32 Get_OneFreDataLen_FromFlash(u8 *pIn)//����һ����ʱ�����ݰ����ݵĳ���,pin������������İ�����21��ʱ���ֽ�
{
	u8 OADNUM;

	u32 i;
	u32 x;

	u32 Offset;
	Offset=21;

	Offset+=4;//ROAD

	Offset+=4;//OADNUM
	OADNUM=pIn[Offset];
	Offset++;
	Offset+=OADNUM*4;

	for(x=0;x<OADNUM;x++)
	{		
		i=Get_DL698DataLen_S(pIn+Offset,0);//�����һ�����ݵĳ���
		Offset+=i;
	}
	return Offset;
}

u32 Day_Month_PackJson(u8 *pInbuf,u8 *pout)//���¶������ݴ��pINbufǰ�����ֽڱ�ʾһ���������ݰ�
{
	u8 *p8;
	u8 *p8s;
	u8 *p8Str;
	u8 *pdata;
	u16 *p16;
	
	u8 NUM;//���ݸ���
	u32 ROAD;//ROAD��ʼλ��
	u8 OADNUM;//OAD������
	u8 OADPORT;//OAD��ʼλ��
	u8 DataNUM;
	
	u16 TotalNUM;
	u32 OAD;

	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Offset;
	u32 CJSONLEN=0;

	p16=(u16 *)pInbuf;
	TotalNUM=p16[0];//һ���Ĳɼ�����
	pInbuf+=2;
	while(TotalNUM>0)
	{
		TotalNUM--;	
		Offset=21;//7���ֽڲɼ�����ʱ��,�ɼ��ɹ�ʱ��,�ɼ�����ʱ��
		if(pInbuf[Offset]!=1)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			goto JSON_ERROR;
		}
		p8=(u8 *)ADDR_DATABUFF; 
		Offset++;
		NUM=pInbuf[Offset];//���ݸ���
		//myprintf("[%s:%d]:NUM=%d\n",(u32)&__func__,(u32)__LINE__,NUM);
		Offset++;
	for(x=0;x<NUM;x++)
	{
		p8Str=Get_Element(pInbuf+21,1+x,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(!p8Str)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
			#endif
			goto JSON_ERROR;
		}
		Offset=0;
		if(p8Str[Offset]!=0x5b)//PROAD
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д��� p8Str[Offset]=%02x\n",(u32)&__func__,__LINE__,p8Str[Offset]);
			#endif
			goto JSON_ERROR;
		}
		Offset+=2;
		
		ROAD=(p8Str[Offset]<<24)|(p8Str[Offset+1]<<16)|(p8Str[Offset+2]<<8)|p8Str[Offset+3];
		//myprintf("[%s:%d]:ROAD=%08x\n",(u32)&__func__,__LINE__,ROAD);
		Offset+=4;
		OADNUM=p8Str[Offset];//OAD������
		Offset++;
		OADPORT=Offset;//OAD��ʼλ��
		Offset+=OADNUM*4;
		p8s=p8Str+Offset;
		//�Ҷ���ʱ���λ��
		for(y=0;y<OADNUM;y++)
		{
			p8s=p8Str+Offset;
			p8s=Get_Element(p8s,y+1,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(!p8s)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
				#endif
				goto JSON_ERROR;
			}
			OAD=(p8Str[OADPORT+y*4]<<24)|(p8Str[OADPORT+1+y*4]<<16)|(p8Str[OADPORT+2+y*4]<<8)|(p8Str[OADPORT+3+y*4]);
			if(0x20210200==OAD)//����ʱ��
			{
				p8s+=8;//һ����ʶ7������
				continue;
			}
			if(1==p8s[0])//����
			{
				DataNUM=p8s[1];
				if(0==p8s[1])
				{
					DataNUM=1;
				}
			}else
			{
				DataNUM=1;
			}
			for(z=0;z<DataNUM;z++)
			{
				if(0!=p8s[1])
				{
					pdata=Get_Element(p8s,z+1,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
					if(!pdata)
					{
						#ifdef DEBUG_JSON
						myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
						#endif
						goto JSON_ERROR;
					}
				}
				i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=ROAD>>16;
				Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbuf������ֽڵ��������pstr��ת��asc��
				i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=OAD+z+1;
				Add_StrToASCII_Inverte(p8,(u8 *)&i,4);//pbuf������ֽڵ��������pstr��ת��asc��(p8, p8+4,4);
				i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				Get698Time_Ascii(pInbuf+7,p8);
				GetJsonTime(p8);
				i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				Get698Time_Ascii(pInbuf+14,p8);
				GetJsonTime(p8);
				i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=Data_Ascii(1,p8,0);//��idaֵ�浽p��,NUMΪС�����λ��
				i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=Data_Ascii(1,p8,0);//��idaֵ�浽p��,NUMΪС�����λ��
				i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				if(0==p8s[1])
				{//ȫ��дNULL
					if(((OAD>>28)&0xf) == 1)//����
					{	
						i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
						i=0;
						i=DealMetData_Ascii(OAD>>16,(u8 *)&i,p8,0);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}
						i=CreateitemNum_Json(pout+CJSONLEN, STR_MV, p8, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
						
						GetNullTime(p8);
						i=Createitem_Json(pout+CJSONLEN, STR_MDT,p8,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;	
						i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
					}else
					{	
						i=0;
						i=DealMetData_Ascii(OAD>>16,(u8 *)&i,p8,0);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;

					}
				}else
				{
					if(((OAD>>28)&0xf) == 1)//����
					{	
						i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
		
						if(pdata[0]==0x02)
						{		
							pdata+=2;
							//��������������
							i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
							if(0==i)
							{
								#ifdef DEBUG_JSON
								myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
								#endif
								goto JSON_ERROR;
							}
							i=CreateitemNum_Json(pout+CJSONLEN, STR_MV, p8, DEBUG_THIRD_SPACENUM);
							CJSONLEN+=i;
							//���������������ʱ��
							i=Get_DL698DataLen_S(pdata,0);
							pdata+=i;
							if(pdata[0]==0x1c)//������ʱ��
							{
								pdata++;
								Get698Time_Ascii(pdata,p8);
								GetJsonTime(p8);
							}else
							{
								GetNullTime(p8);
							}
							i=Createitem_Json(pout+CJSONLEN, STR_MDT,p8,DEBUG_THIRD_SPACENUM);
							CJSONLEN+=i;
						}
						
						i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
					}else
					{	
						i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}

						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;

					}	
				}
				i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json( pout, CJSONLEN, DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
			}
		}
	}
	i=Get_OneFreDataLen_FromFlash(pInbuf);
	pInbuf+=i;
}
	return CJSONLEN;
JSON_ERROR:
	return 0;
}

void Add_NewOADToBuff(u8 *pOAD,u8 *pBuff,u8 OADNUM)//pBuff�е�OAD��ӵ�pPAD�� ����ΪOADNUM
{
	u8 x;	
	pOAD[0]=OADNUM;
	pOAD++;
	for(x=0;x<OADNUM;x++)
	{
		MW((u32)pBuff+x*4,(u32)pOAD+x*4,4);
	}
}

u32 IsFindOneOADFromBuff(u8 *pOAD,u8 *pBuff)//�Ӵ���õ�POAD buf����PBUFF��ӦOAD,����ҵ���˵����pOAD�д���pBuff���OAD
{

	u8 x;
	u8 NUM;
	u32 OAD;
	u32 OAD1;
	u32 ret=1;
	NUM=pOAD[0];
	pOAD++;

	OAD1=(pBuff[0]<<24)|(pBuff[1]<<16)|(pBuff[2]<<8)|pBuff[3];
	if(0x20210200==OAD1)
	{
		return 0;
	}
	for(x=0;x<NUM;x++)
	{
		OAD=(pOAD[x*4]<<24)|(pOAD[x*4+1]<<16)|(pOAD[x*4+2]<<8)|pOAD[x*4+3];	
		if(OAD==OAD1)
		{
			break;
		}
	}
	if(x>=NUM)
	{
		ret=0;
	}	
	return ret;
}

u8 *Find_DataHead(u8 *pIn,u8 *pOAD,u32 oadcnt)//����Flash����Ĵ洢��ʽ��pIn����OAD��λ��ȥ��pIn�������ݵ�λ��,pOAD��һ�ֽ���OAD����,oadcnt��pOAD�����ƫ��,����Pin�������ݵ���ȷλ��
{
	u8 OADNUM;
	u32 i;
	u32 y;
	u32 Offset;
	pIn+=21;
	Offset=0;
	Offset+=4;
	Offset+=4;

	OADNUM=pIn[Offset];
	Offset++;
	Offset+=(OADNUM*4);
	
	if(oadcnt>OADNUM-1)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:����������OAD�±����ʵ�ʵ�OAD����\n",(u32)&__func__,__LINE__,0);
		#endif
		return 0;
	}
	pIn+=Offset;
	Offset=0;
	for(i=0;i<=oadcnt;i++)
	{
		if(i==oadcnt)
		{			
			return pIn+Offset;
		}
		y=Get_DL698DataLen_S(pIn+Offset,0);//�����һ�����ݵĳ���
		Offset+=y;
	}
	return 0;
}

u32 JudegMin_FromTime(u8 *pch,u32 cnt,u32 Min)//��flash�����ʱ��ȥ�ж����ʱ���ķ��Ӵ治����(����1440���Ӵ�0���ӿ�ʼ�ж�)
{
	u8 hh;
	u8 mm;
	u32 i;
	pch+=14;
	//�ɼ��洢ʱ��
	//07 00 01 02 00 00 00
	pch+=4;
	hh=pch[0];
	mm=pch[1];
	i=hh*60+mm;
	
	if(i==(cnt*Min))
	{
		return 1;
	}
	return 0;
}

u32 Is_SingleOADNUM_FromData(u8 *pOAD,u8 *pData,u32 OADNUM)//����˵���OAD��00100200 ��һ����5�� �ܼ��ƽ��
{
	u32 i;
	u32 n;
	u32 Len;
	u32 OAD;
	OAD=(pOAD[0]<<24)|(pOAD[1]<<16)|(pOAD[2]<<8)|pOAD[3];

	if((OAD&0xff)==0)
	{
		Len=0;
		for(i=0;i<OADNUM;i++)
		{
			n=Get_DL698DataLen_S(pData+Len,0);//�����һ�����ݵĳ���
			Len+=n;
		}
		if(1==pData[Len])//�Ǹ����飨һ�㶼�����飩
		{
			i=pData[Len+1];
			if(i>1)
			{
				return i;
			}else
			{
				return 0;
			}
		}
	}
	return 0;
}

u32 Min_PackJson(u8 *pInbuf,u8 *pout,u32 Min)//���Ӷ������ݵĴ��MiN������15���ӻ���1����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;	
	u32 CJSONLEN;
	u8 *p8;
	u8 *pOAD;
	u8 *pbuf=pInbuf;
	u8 *pdata;//ָ����������ָ��

	u32 Offset;
	u32 TotalNUM;
	u32 OAD;
	u32 ROAD;		//��¼������ROAD
	u32 TIMESPOT;	//ʱ���ǩ��λ��
	u8 OADNUM;		//��¼������ROAD����
	u8 OADNUM_S;
	u32 OADSPOT; 	//��¼������ROAD������λ��//20210200
	u32 Fre_Dst;	//���ݶ����ܶ�
	u16 *p16;
	CJSONLEN=0;
	p16=(u16 *)pbuf;
	TotalNUM=p16[0];
	if(0==TotalNUM)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:û���������� %d\n", (u32)&__func__, __LINE__, TotalNUM);
		#endif
		return 0;
	}
	pbuf+=2;//������������
	for(TIMESPOT=0;TIMESPOT<1440;TIMESPOT++)
	{
		x=mymemcmp(pbuf+TIMESPOT*ONEDATASIZE, 0, 2);
		if(x)
		{
			break;
		}
	}
	if(TIMESPOT>=1440)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:û���������� %d\n", (u32)&__func__, __LINE__, TotalNUM);
		#endif
		return 0;
	}
	Offset=TIMESPOT*ONEDATASIZE;//���ҵ�һ����Ч,����һ����Ч��ͷ������
	p8=(u8 *)ADDR_DATABUFF;
	
	pOAD=(u8 *)ADDR_DATABUFF+1024;
	MC(0,(u32)ADDR_DATABUFF+1024,1024);

	Offset+=21;

	Offset+=4;
	ROAD=(pbuf[Offset]<<24)|(pbuf[Offset+1]<<16)|(pbuf[Offset+2]<<8)|pbuf[Offset+3];
	if(ROAD!=0x50020200)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:���Ƿ��Ӷ�������\n", (u32)&__func__, __LINE__, 0);
		#endif
		goto Min_JSON_ERROR;
	}
	Offset+=4;
	OADNUM=pbuf[Offset];
	Offset++;	
	Add_NewOADToBuff(pOAD,pbuf+Offset,OADNUM);//pBuff�е�OAD��ӵ�pPAD�� ����ΪOADNUM
	OADSPOT=Offset;
	Offset+=(OADNUM*4);	
	for(y=0;y<OADNUM;y++)
	{
		if(!IsFindOneOADFromBuff(pOAD,pbuf+OADSPOT+y*4))//�Ӵ���õ�POAD buf����PBUFF��ӦOAD,����ҵ���˵����pOAD�д���pBuff���OAD
		{	
			continue;
		}
		
		i=Is_SingleOADNUM_FromData(pbuf+OADSPOT+y*4,pbuf+Offset,y);
		if(0==i)
		{
			i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=ROAD>>16;
			Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbuf������ֽڵ��������pstr��ת��asc��
			i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			OAD=(pbuf[OADSPOT+y*4]<<24)|(pbuf[OADSPOT+y*4+1]<<16)|(pbuf[OADSPOT+y*4+2]<<8)|(pbuf[OADSPOT+y*4+3]<<0);
			Add_StrToASCII_Inverte(p8,(u8 *)&OAD,4);//pbuf������ֽ������pstr��ת��asc��
			i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			Get698Time_Ascii(pbuf+TIMESPOT*ONEDATASIZE+7,p8);//pʱ��ǰ�����ֽڱ�ʾ���p8���asc��
			//Get698Time_HMS_Ascii(p8,0,0,0);
			//p8[3]-=1;
			GetJsonTime(p8);
			i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			switch(Min)
			{
				case 1:
					Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,59,0);
					//cJSON_AddStringToObject(obj,"dt",(char*)p8);//�ɼ��洢ʱ��(����ʱ��)
					break;
				case 15:
					Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,45,0);
					//cJSON_AddStringToObject(obj,"dt",(char*)p8);//�ɼ��洢ʱ��(����ʱ��)
					break;
			}
			GetJsonTime(p8);
			i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=Data_Ascii((u64)Min,p8,0);//��idaֵ�浽p��,NUMΪС�����λ��
			i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			Fre_Dst=(24*60)/Min;
			i=Data_Ascii((u64)Fre_Dst,p8,0);//��idaֵ�浽p��,NUMΪС�����λ��
			i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			//�жϷ��ӵ����ݴ治����		

			for(z=0;z<Fre_Dst;z++)
			{	
				x=mymemcmp(pbuf+z*ONEDATASIZE*Min, 0, 2);
				if(x)
				{
					pdata=Find_DataHead(pbuf+z*ONEDATASIZE*Min,pOAD,y);//����Flash����Ĵ洢��ʽ��pIn����OAD��λ��ȥ��pIn�������ݵ�λ��,pOAD��һ�ֽ���OAD����,oadcnt��pOAD�����ƫ��,����Pin�������ݵ���ȷλ��
					if(pdata)
					{	
						i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
							#endif
							goto Min_JSON_ERROR;
						}
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;
					}else
					{
						WriteNullString(p8);//����BUF����дASC ��NULL
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;
					}
				}else
				{
					WriteNullString(p8);//����BUF����дASC ��NULL
					i=CreateData_Json(pout+CJSONLEN, p8);
					CJSONLEN+=i;
				}
			}

			i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=CreateArrayEnd_Json(pout,CJSONLEN, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
		}
		else//�����ɵ�OAD,��OAD�ϲ�����
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:����15�������ݴ���\n",(u32)&__func__,__LINE__,0);
			#endif
				OADNUM_S=i;//����ĸ�������OAD�ķ����ĸ���
				OAD=(pbuf[OADSPOT+y*4]<<24)|(pbuf[OADSPOT+y*4+1]<<16)|(pbuf[OADSPOT+y*4+2]<<8)|(pbuf[OADSPOT+y*4+3]);
				for(x=1;x<=OADNUM_S;x++)//ѭ����¼ÿһ��OAD�ж��ٸ���OAD
				{
					i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
					CJSONLEN+=i;
					i=ROAD>>16;
					Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbuf������ֽڵ��������pstr��ת��asc��
					i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					OAD&=0xffffff00;
					OAD|=x;
					Add_StrToASCII_Inverte(p8,(u8 *)&OAD,4);//pbuf������ֽ������pstr��ת��asc��
					i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					Get698Time_Ascii(pbuf+TIMESPOT*ONEDATASIZE+7,p8);//pʱ��ǰ�����ֽڱ�ʾ���p8���asc��
					//Get698Time_HMS_Ascii(p8,0,0,0);
					//p8[3]-=1;
					GetJsonTime(p8);
					i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					switch(Min)
					{
						case 1:
							Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,59,0);
							//cJSON_AddStringToObject(obj,"dt",(char*)p8);//�ɼ��洢ʱ��(����ʱ��)
							break;
						case 15:
							Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,45,0);
							//cJSON_AddStringToObject(obj,"dt",(char*)p8);//�ɼ��洢ʱ��(����ʱ��)
							break;
					}
					GetJsonTime(p8);
					i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=Data_Ascii((u64)Min,p8,0);//��idaֵ�浽p��,NUMΪС�����λ��
					i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					Fre_Dst=(24*60)/Min;
					i=Data_Ascii((u64)Fre_Dst,p8,0);//��idaֵ�浽p��,NUMΪС�����λ��
					i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;

					//�жϷ��ӵ����ݴ治����	
					for(z=0;z<Fre_Dst;z++)
					{
						i=mymemcmp(pbuf+z*ONEDATASIZE*Min, 0, 2);
						if(i)
						{
							pdata=Find_DataHead(pbuf+z*ONEDATASIZE*Min,pOAD,y);//����Flash����Ĵ洢��ʽ��pIn����OAD��λ��ȥ��pIn�������ݵ�λ��,pOAD��һ�ֽ���OAD����,oadcnt��pOAD�����ƫ��,����Pin�������ݵ���ȷλ��
							pdata=Get_Element(pdata,x,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
							if(pdata)
							{	
								i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii
								if(0==i)
								{
									#ifdef DEBUG_JSON
									myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
									#endif
									goto Min_JSON_ERROR;
								}
								i=CreateData_Json(pout+CJSONLEN, p8);
								CJSONLEN+=i;
							}else
							{
								WriteNullString(p8);//����BUF����дASC ��NULL
								i=CreateData_Json(pout+CJSONLEN, p8);
								CJSONLEN+=i;
							}
						}else
						{
							WriteNullString(p8);//����BUF����дASC ��NULL
							i=CreateData_Json(pout+CJSONLEN, p8);
							CJSONLEN+=i;
						}
					}

					i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=CreateArrayEnd_Json(pout,CJSONLEN, DEBUG_SECOND_SPACENUM);
					CJSONLEN+=i;
				}
			}
	}
	
	return CJSONLEN;
Min_JSON_ERROR:
	return 0;
}

u32 RCSDtoFileID_JS(u16 *pAddr,u32 ROAD,u32 Min)//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff��MIN��1���ӻ���15����������
{	
	if(0==ROAD)
	{	
		if(Min==15)
		{
			return 0x13f12000;
		}
		if(Min==1)
		{
			return 0x13f17000;
		}
	}else
	{
		switch(ROAD)
		{
			case 0x50040200:
				return 0x13f16000;
			case 0x50020200:
				if(Min==15)
				{
					return 0x13f19000;
				}
				if(Min==1)
				{
					return 0x13f22000;
				}
			case 0x50060200:
				return 0x13f18000;
		}
	}				
	return 0;//����:0��ʾû�ҵ���ЧFileID
}

u32 Filter_Time(u8 *pfiletm,u32 ROAD,u32 isReal,u32 All)//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
{
	u32 i;
	u32 x;
	u32 ret=0;
	switch (ROAD)
	{
		case 0x50020200://���Ӷ���
			//�Ƚ�ʱ������һ���
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			x=(pfiletm[0]<<16)|(pfiletm[1]<<24)|(pfiletm[2]<<8)|(pfiletm[3]);
			if(i==x)
			{
				ret=1;
			}		
		  	break;
		case 0x50040200://�ն���
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			x=(pfiletm[0]<<16)|(pfiletm[1]<<24)|(pfiletm[2]<<8)|(pfiletm[3]);
			if(i==x)
			{
				ret=1;
			}
			break;
		case 0x50060200://�¶���
			//�Ƚ�ʱ������һ���
			i=(JSON->Lastrealtime[0]<<8)|(JSON->Lastrealtime[1]<<16)|JSON->Lastrealtime[2];
			i=YYM_SubM_hex(i, 1);//�������ռ�1��,���ؼ�1�����������
			x=(pfiletm[0]<<8)|(pfiletm[1]<<16)|pfiletm[2];
			if(i==x)
			{
				if(JSON->Lastrealtime[3]==1)//ÿ���µ�һ�ŲŻ�ȥ����¶���
				{
					ret=1;
				}
			}			
			break;
		default://ʵʱ����
		break;
	}
	return ret;
}

u32 Min_FrzNUM(u8 *pIn)//���ذ�Сʱ����,ת���ɷ�����
{
	u8 hh;
	u8 mm;
	u32 i;
	pIn+=14;
	pIn+=4;
	hh=pIn[0];
	mm=pIn[1];
	i=60*hh+mm;
	return i;
}

u32 Filter_Same_Data(u8 *pt,u32 datanum,u8 *pdata)//�òɼ��洢ʱ��ȥ�ж��ǲ����ظ�������,����1���ظ�������,����0�����ظ�������
{
	u32 i;
	u32 x;
	if(0==datanum)//һ�����ݶ�û�е�ʱ��Ĭ��û���ظ�������
	{
		return 0;
	}
	pdata+=14;//ָ��ɼ��洢ʱ��
	for(i=0;i<datanum;i++)
	{
		//ɾѡ������
		for(x=0;x<6;x++)
		{
			if(pt[x]!=pdata[x])
			{
				break;
			}
		}
		if(x>=6)
		{
			return 1;
		}
		pt+=7;
	}
	return 0;
}

u32 Is_1MinData(u8 *pdata,u8 ACSAMPLE_FLAG)//�ж���������ǲ���1��������,1��.0����,ACSAMPLE_FLAG����û��1��������
{
	u32 i;
	u32 OAD;
	u32 OADnum;
	if(ACSAMPLE_FLAG!=0)
	{
		return 0;
	}
	pdata+=21;
	pdata+=4;
	pdata+=4;
	OADnum=*pdata;
	pdata++;
	
	for(i=0;i<OADnum;i++)
	{
		OAD=(pdata[0]<<24)|(pdata[1]<<16)|(pdata[2]<<8)|(pdata[3]);
		if((OAD>>8)==0x200002)
		{
			return 1;
		}
		pdata+=4;
	}
	return 0;
}

u32 Filter_Realtime(u8 *ptime)//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
{
	u32 ret=0;
	u32 i;
	i=Compare_DL698DataString(JSON->Lastrealtime,ptime,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
	if(i==1)
	{
		ret=1;
	}	
	return ret;
}

u32 Is_ACSAMPLE(u16 *pAddr)
{
	u8 *p8;
	u8 *p8s;
	//u16 *p16;
	u16 *p16s;
	u32 i;
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			p8s=Get_Element(p8,2,4,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(0x08!=p8s[2])//����
			{
				p8+=LENper_6000;
				continue;
			}
			p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
			if(pAddr[0]==p16s[0])
			{
				if(pAddr[1]==p16s[1])
				{
					if(pAddr[2]==p16s[2])
					{
						return 0;
					}
				}
			}
					
		}
		p8+=LENper_6000;
	}
	#ifdef DEBUG_JSON
	myprintf("%04x %04x \n",pAddr[0],p16s[0],0);
	myprintf("%04x %04x \n",pAddr[1],p16s[1],0);
	myprintf("%04x %04x \n",pAddr[2],p16s[2],0);
	#endif
	return 1;
}

u32 Get_RecordData(u32 ROAD,u32 FileID,u16 *pAddr,u8 *pstack,u32 *DataLen,u32 isReal,u32 All)//��Flash�л�ȡ����,����һ������ָ��,��ָ����ҪFree�ͷ�
{
	//u32 iii;
	u32 z;
	u8 *p8;
	u8 *p8time;
	u32 MetNUM;
	u32 i;
	u32 n;
	u32 offset;
	u32 DataNUM;
	u32 filename;
	u8 ACSAMPLE_FLAG;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ�ļ�
	ms=(ms_Type *)Get_ms();
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
	
	DataNUM=0;
	*DataLen=0;
	Comm_Ram->msFILESearchCount=0;	
	
	ACSAMPLE_FLAG=0;
	if(ROAD==0x50020200&&FileID==0x13f19000)//�ж��ǲ��ǽ��ɵ���,15��������
	{
		//�ǲ��ǽ��ɵ���
		i=Is_ACSAMPLE(pAddr);
		if(i==0)
		{
			ACSAMPLE_FLAG=1;
		}
	}
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
	{		
		filename=Get_RecordFileName_JS(FileID,0xffffffff,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
		if(filename==0xffffffff)
		{
			continue;
		}
		//ֻ��Flash�ļ�������� RAM�ļ�����û������
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			continue;
		}
		if(file->BLOCK_ENTRY==0)
		{
			continue;
		}
		if(0==ROAD)//ʵʱ�����òɼ�����ʱ�����ж�
		{
			i=Filter_Realtime(file->save_date_time_s);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
		}else
		{	
			i=Filter_Time(file->save_date_time_s,ROAD,isReal,All);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
		}
		if(0==i)
		{
			continue;
		}
		if(0==ROAD)//����������ظ��ĵ�ַ�����������ݲ�ͬҲҪȡ����
		{
			p8time=(u8 *)ms->ms_malloc(7*1440);
			if(!p8time)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:����ռ�ʧ��\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				return 0;
			}
			n=file->NUM_DATA;
			//myprintf("[%s:%d]:����������� n=%d\n ",(u32)&__func__,(u32)__LINE__,n);	
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
			for(MetNUM=0;MetNUM<n;MetNUM++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							i=RecordFileHead->LEN_DATA;
							if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
							{
								#ifdef DEBUG_JSON
								myprintf("[%s:%d]:ȡ�������ݳ��ȳ���\n ",(u32)&__func__,(u32)__LINE__,0);	
								#endif
								continue;
							}
							ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);		
							p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));	
							if(0!=p8[0])//ȡ�������������п��ܶ���0��ֱ�ӹ��˵�,Ϊ�˼���ѭ�������Ͳ��һ���ֽ�
							{
								if(!Filter_Same_Data(p8time,DataNUM,p8))//�òɼ��洢ʱ��ȥ�ж��ǲ����ظ�������,����1���ظ�������,����0�����ظ�������
								{
									MW((u32)p8,(u32)pstack+(*DataLen),i);
									MW((u32)p8+14,(u32)p8time+(DataNUM*7),7);//����ɼ��洢ʱ�귽�����
									(*DataLen)+=i;
									DataNUM++;//�鵽һ���µ������Զ���1	
								}
							}	
						}
								
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			ms->ms_free(p8time);
			p8time=NULL;
		}else
		{
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i>=n)
			{//û�ҵ�		
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:��ַû���ҵ�\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				continue;
			}
			i=RecordFileHead->LEN_DATA;
			if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:ȡ�������ݳ��ȳ���\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				continue;
			}
			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);		
			
			p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));	
			
			if(0x50020200==ROAD)//����Ƿ���,�Ǿ���֡��ǰ���������ֽ���Ϊ���
			{
				z=Min_FrzNUM(p8);
				if(i<ONEDATASIZE&&z<1440)
				{
					if(FileID==0x13f22000)
					{
						if(Is_1MinData(p8,ACSAMPLE_FLAG))
						{
							MW((u32)p8,(u32)pstack+z*ONEDATASIZE,i);	
							DataNUM++;//�鵽һ���µ������Զ���1	
						}
					}else
					{
						if(!Is_1MinData(p8,ACSAMPLE_FLAG))
						{
							MW((u32)p8,(u32)pstack+z*ONEDATASIZE,i);	
							DataNUM++;//�鵽һ���µ������Զ���1	
						}
					}
					
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("�������ݴ�Flash�������������,��Ҫ�޸� i=%d,z=%d\n ",i,z,0);	
					#endif
				}
				
			}else
			{	
				MW((u32)p8,(u32)pstack+(*DataLen),i);
				(*DataLen)+=i;
				DataNUM++;//�鵽һ���µ������Զ���1	
			}
		}
	}
	return DataNUM;
}

u8 *Get_MetData_ROAD(u16 *pAddr,u32 ROAD,u32 Min,u32 isReal,u32 All)//����ROAD��ȡFlash�е�����,pAddr���ַ,Minȡ���Ӷ����ʱ����,1��ʾ1����15��ʾ15����,����0������,���򷵻�һ������bufָ��,isReal�Ƿ���ȫ��ʵʱ
{
	u16 *p16;
	u16 DataNUM;
	u32 Len;
	u32 i;
	u32 x;
	u8 *pstack=NULL;
	u32 FileID;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	pstack=(u8 *)ms->ms_malloc(1024*1024);
	if(!pstack)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:����ռ�ʧ��\n ",(u32)&__func__,(u32)__LINE__,0);	
		#endif
		return 0;
	}
	MC(0,(u32)pstack,1024*1024);
	Len=0;
	DataNUM=0;
	switch(ROAD)
	{
		case 0x50020200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			break;
		case 0x50040200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			FileID=0x13f11000;
			x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
			Len+=i;
			DataNUM+=x;
			break;
		case 0x50060200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			FileID=0x13f13000;
			x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
			Len+=i;
			DataNUM+=x;
			break;
		default://ʵʱ
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
		break;
	}
	if(0==DataNUM)
	{
		ms->ms_free(pstack);
		pstack=NULL;
	}else
	{
		p16=(u16 *)pstack;
		p16[0]=DataNUM;	
	}
	return pstack;
}

u32 FreData_PackJson(u32 ROAD,u8 *pInbuf,u8 *pout,u32 Min)//�������ݴ����ROAD���� ���Ӷ�������¶���
{
	u32 ret=0;
	switch(ROAD)
	{
		case 0x50020200:
			ret=Min_PackJson(pInbuf,pout,Min);
			break;
		case 0x50040200:
		case 0x50060200:
			ret=Day_Month_PackJson(pInbuf,pout);
			break;
		default:
			break;
	}
	return ret;
}

void GetMetAddr(void)//ȡ�����б�ĵ�ַ
{
	u8 *p8;
	u16 *p16;
	u16 *p16s;
	u32 i;
	u32 AddrNUM;
//1.ȡ�ñ�����������Ч��ַ,���˽��ɵ�ַ
	AddrNUM=0;
	p16=(u16*)(ADDR_AddrList+2);
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
			p16[0]=p16s[0];
			p16[1]=p16s[1];
			p16[2]=p16s[2];
			p16+=3;
			AddrNUM+=1;
		}
		p8+=LENper_6000;
	}
	p16=(u16*)(ADDR_AddrList);
	p16[0]=AddrNUM;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ȡ�õ��������еı��ַ ����:%d\n",(u32)&__func__, __LINE__, AddrNUM);
	#endif
}

u8 *CreateRealJsonFile(void)//����һ���µ�ʵʱjson��Ŀ�ļ�
{	
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//�����ļ���
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//�õ�698��ǰʱ��ת����asc��		
	Get698Time_Ascii(JSON->Lastrealtime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".json");	
	i=mystrlen((const char *)p8);
	if(sizeof(JSON->RealFileName)>=i+1)
	{
		mystrncpy((char *)JSON->RealFileName,(const char *)p8,i);
	}else
	{
		#ifdef DEBUG_JSON
		myprintf("\n[%s:%d]:JSON->RealFileName buf�ռ��С���� ����i+1=%d\n", (u32)&__func__, __LINE__, i+1);
		#endif
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:�ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	myprintf("\n[%s:%d]:�ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )JSON->RealFileName);
	#endif
	return p8;
}

u8 *CreateFreJsonFile(void)//����һ���µĶ���json��Ŀ�ļ�
{
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//�����ļ���
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//�õ�698��ǰʱ��ת����asc��	
	mystrcpy((char *)p8+Len+8, ".json");	
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:�ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	#endif
	return p8;
}

u8 *CreateFreJsonFile_RAM(void)//��RAM��,����һ���µĶ�����Ŀ�ļ�
{
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//�����ļ���
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//�õ�698��ǰʱ��ת����asc��	
	mystrcpy((char *)p8+Len+8, ".json");	
	
	return p8;
}

void Compress_JSONFile(u8 *fname,u32 fnamesiz,u32 isReal)//��JSON�ļ������tar.gz��ʽ,isReal,1ʵʱ�ļ�,0�����ļ�
{
	u8 *p8;
	u32 i;
	u32 SLen;
	ms_Type *ms;
	ms=Get_ms();
	p8=(u8 *)ADDR_DATABUFF+1024;
	SLen=0;
	//���ļ���������ļ��ƶ�����ǰĿ¼����
	mystrcpy((char *)p8+SLen,"mv ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	mystrcpy((char *)p8+SLen,(const char *)fname);
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	mystrcpy((char *)p8+SLen," .");
	ms->link_system((char *)p8);
	//�������ļ�
	SLen=0;
	if(isReal)
	{
		mystrcpy((char *)p8+SLen, "tar jcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		i=mystrlen(JSONREALPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		for(i=0;i<SLen;i++)
		{
			if('.'==p8[i])
			{
				break;
			}
		}
		mystrcpy((char *)p8+i, ".tar.bz2 ");
		i=mystrlen((const char *)p8);
		SLen=i;
		i=mystrlen(JSONREALPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
	}else
	{
		mystrcpy((char *)p8+SLen, "tar zcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		i=mystrlen(JSONFREPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		for(i=0;i<SLen;i++)
		{
			if('.'==p8[i])
			{
				break;
			}
		}
		mystrcpy((char *)p8+i, ".tar.gz ");
		i=mystrlen((const char *)p8);
		SLen=i;
		i=mystrlen(JSONFREPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
	}	
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:��ѹ�ļ��ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	//��.JSON�ļ�ɾ����,Ȼ���ٰ�ѹ�����ƶ�����Ӧ��λ��
	SLen=0;
	mystrcpy((char *)p8+SLen, "mv ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	if(isReal)
	{
		i=mystrlen(JSONREALPATH);
	}else
	{
		i=mystrlen(JSONFREPATH);
	}
	mystrcpy((char *)p8+SLen, (const char *)fname+i);
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	for(i=0;i<SLen;i++)
	{
		if('.'==p8[i])
		{
			break;
		}
	}
	if(isReal)
	{
		mystrcpy((char *)p8+i, ".tar.bz2 ");
		i=mystrlen((const char *)p8);
		SLen=i;
		mystrcpy((char *)p8+SLen, JSONREALPATH);
	}else
	{
		mystrcpy((char *)p8+i, ".tar.gz ");
		i=mystrlen((const char *)p8);
		SLen=i;
		mystrcpy((char *)p8+SLen, JSONFREPATH);
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:�ƶ��ļ��ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	//ɾ����ǰ��.json�ļ�
	SLen=0;
	mystrcpy((char *)p8+SLen, "rm ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	if(isReal)
	{
		mystrcpy((char *)p8+SLen, JSONFILENAMEHEAD_REAL);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		p8[SLen++]='*';
		p8[SLen++]=0;
	}else
	{
		mystrcpy((char *)p8+SLen, JSONFILENAMEHEAD_FRE);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		p8[SLen++]='*';
		p8[SLen++]=0;
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ɾ����ǰJSON�ļ��ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	if(isReal)
	{
		Delet_JsonFile(JSON->RealFileName);
	}else
	{
		p8=(u8 *)ADDR_DATABUFF+1024;
		mystrcpy((char *)p8, (const char *)fname);
		Delet_JsonFile(p8);
	}
}

u32 HeadStart_JSON(u8 *pstc)//����JSON��ʽ: [
{
	u32 Offset;
	u32 i;
	Offset=0;
	i=Str_L_SQB(pstc+Offset);//������
	Offset+=i;
	return Offset;
}	

u32 HeadEnd_JSON(u8 *pstc,u32 Len)//����JSON��ʽ: ]
{
	u32 flag=0;
	u32 Offset;
	u32 i;
	Offset=0;
	if(Len>0)
	{
		if(','==pstc[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pstc+Len+Offset);//�س��ͻ���
	Offset+=i;
	i=Str_R_SQB(pstc+Len+Offset);//�ҷ�����
	Offset+=i;
	if(1==flag)
	{
		Offset--;
	}
	return Offset;
}

u32 FreDataPack(u16 *pAddr,u8 *pd,u32 ROAD,u32 Min,u32 isReal)
{
	u32 i;
	u8 *pDATA;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pDATA=Get_MetData_ROAD(pAddr,ROAD,Min,isReal,0);//����ROAD��ȡFlash�е�����,pAddr���ַ,Minȡ���Ӷ����ʱ����,1��ʾ1����15��ʾ15����,����0������,���򷵻�һ������bufָ��
	if(NULL==pDATA)
	{	
		return 0;	
	}
	i=FreData_PackJson(ROAD,pDATA,pd,Min);
	ms->ms_free(pDATA);
	pDATA=NULL;	
	return i;
}

u32 AllDataPack(u16 *pAddr,u8 *pd)
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	//��ȡ�������ݲ������JSON��ʽ,����0û���ҵ�����
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50040200,0,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50060200,0,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50020200,15,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50020200,1,0);
	CJSONLEN+=i;	
	return CJSONLEN;
}

void Delet_FrzJsonFile(void)//ɾ��Flash�ռ�����,���еĶ����ļ�
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);
}

void Crl_AllJsonFileAndTar(void)//��������йص�JSON�ļ�����
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.gz");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.bz2");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);
}


void Delet_Yesterday_Data_Flash(void)
{
	//ɾ��������ն���
	Delet_All_FromFlash(0x13f16000);
	Delet_All_FromFlash(0x13f11000);
	//ɾ��������¶���
	Delet_All_FromFlash(0x13f18000);
	Delet_All_FromFlash(0x13f13000);
	//ɾ������ķ��Ӷ���
	Delet_All_FromFlash(0x13f19000);
	Delet_All_FromFlash(0x13f22000);	
}

u32 Get_OneRealDataLen_FromFlash(u8 *pIn)//����һ��ʵʱ���ݰ����ݵĳ���,pin������������İ�����21��ʱ���ֽ�
{
	u8 *p8;
	u32 i;
	u32 x;
	u32 OADNUM;
	u32 Len;
	Len=0;
	Len+=21;//����21��ʱ���ֽ�
	OADNUM=pIn[Len+1];//OAD����
	Len+=2;
	Len+=OADNUM*6;
	p8=pIn+Len;//ָ�����ݲ���
	for(x=0;x<OADNUM;x++)
	{
		i=Get_DL698DataLen_S(p8,0);
		p8+=i;
		Len+=i;
	}
	return Len;
}

u32 RealData_PackJson(u8 *pInbuf,u8 *pout,u8 *pAddr,u32 All)//����ʵʱ����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;	
	u32 CJSONLEN=0;
	u32 CJSONL;
	u8 *p8;
	u8 *pdata;//ָ����������ָ��
	u8 val;

	u32 Offset=0;
	u32 TotalNUM;
	u32 OAD;
	u32 ROAD;		//��¼������ROAD
	u8 OADNUM;		//��¼������ROAD����
	u8 OADSPOT; 	//��¼������ROAD������λ��//20210200
	u32 DataLen;	//���ݴ�С

	u16 *p16;
	p16=(u16 *)pInbuf;
	TotalNUM=p16[0];

	pInbuf+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MC(0,(u32)ADDR_DATABUFF+1024,1024);
	DataLen=0;
	val=0;
	WWDT_Disable();
	if(TotalNUM==0)
	{
		if(All!=0)
		{
			TotalNUM=1;
		}
	}
	for(z=0;z<TotalNUM;z++)
	{
		Offset=21;
		Offset+=4;
		ROAD=(pInbuf[Offset]<<24)|(pInbuf[Offset+1]<<16)|(pInbuf[Offset+2]<<8)|pInbuf[Offset+3];
		if(ROAD!=0x50020200)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:���Ƿ��Ӷ�������:%08x\n", (u32)&__func__, __LINE__, ROAD);
			#endif
			goto Real_JSON_ERROR;
		}
		Offset+=4;
		OADNUM=pInbuf[Offset];
		Offset++;
		OADSPOT=Offset;
		Offset+=(OADNUM*4); 
		//=====================
		CJSONL=CJSONLEN;
		val=0;
		i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
		CJSONLEN+=i;
		Add_StrToASCII(p8,pAddr,6);//pbuf������ֽ������pstr��ת��asc��
		i=CreateDataHead_Json(pout+CJSONLEN, p8, DEBUG_THIRD_SPACENUM);
		CJSONLEN+=i;
		
		//����ʱ��
		Get698Time_Ascii(pInbuf+14,p8);//pʱ��ǰ�����ֽڱ�ʾ���p8���asc��
		GetJsonTime(p8);
		i=CreateData_Json(pout+CJSONLEN, p8);
		CJSONLEN+=i;
		for(y=1;y<RealDataOAD_JSONSIZE;y++)//y��1��ʼ�� ��Ϊ��һ����ʱ����ǰ���Ѿ�д����
		{
			for(x=0;x<OADNUM;x++)
			{
				OAD=(pInbuf[OADSPOT+x*4]<<24)|(pInbuf[OADSPOT+x*4+1]<<16)|(pInbuf[OADSPOT+x*4+2]<<8)|pInbuf[OADSPOT+x*4+3];
				
				if((RealDataOAD_JSON[y])==(OAD))
				{
					break;
				}else
				{
					if((OAD&0xff)==0)//���OAD���һ���ֽ���0�п��ܾ��ǵ�ѹ �������ֺ����
					{
						if((RealDataOAD_JSON[y]>>8)==(OAD>>8))
						{
							break;
						}	
					}
				}
			}
			if(x<OADNUM)//�ҵ�����ЧOAD
			{	
				pdata=Find_DataHead(pInbuf,pInbuf+OADSPOT,x);//����Flash����Ĵ洢��ʽ��pIn����OAD��λ��ȥ��pIn�������ݵ�λ��,pOAD��һ�ֽ���OAD����,oadcnt��pOAD�����ƫ��,����Pin�������ݵ���ȷλ��
				if((RealDataOAD_JSON[y]!=OAD)&&((OAD&0xff)==0))
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:�ǽ�������\n",(u32)&__func__,__LINE__,0);
					myprintf("[%s:%d]:�ǽ�������OAD=%08x\n",(u32)&__func__,__LINE__,OAD);
					
					for(i=0;i<50;i++)
							myprintf("%02x ",pdata[i],0,0);
						myprintf("\n",0,0,0);
					#endif
					i=RealDataOAD_JSON[y]&0xff;
					pdata=Get_Element(pdata,i,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
				}
				if(!pdata)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:û���ҵ���Ч������\n",(u32)&__func__,__LINE__,0);
					#endif
					continue;
				}
				i=DealMetData_Ascii(OAD>>16,pdata,p8,1);//�������ص�������,���ش���pout���ֽ�����,pout�Ѿ�����õ�ASCii			
				if(0==i)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:�ɼ��������ݴ洢��ʽ�д���\n",(u32)&__func__,__LINE__,0);
					#endif
					goto Real_JSON_ERROR;
				}
				if(All)
				{//ȫ��
					val=1;
				}else
				{//����  ���˵�0
					if(0==p8[0])
					{
						continue;
					}else
					{
						val=1;
					}
				}
				i=CreateData_Json(pout+CJSONLEN, p8);
				CJSONLEN+=i;
			}else
			{
				i=Str_COMMA(pout+CJSONLEN);//����
				CJSONLEN+=i;
			}
		}
		i=CreateDataEnd_Json(pout,CJSONLEN);
		CJSONLEN+=i;
		i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_SECOND_SPACENUM);
		CJSONLEN+=i;
		i=Str_LF(pout+CJSONLEN);
		CJSONLEN+=i;
		DataLen=Get_OneFreDataLen_FromFlash(pInbuf);//����һ����ʱ�����ݰ����ݵĳ���,pin������������İ�����21��ʱ���ֽ�
		if(z<TotalNUM)
		{	
			pInbuf+=DataLen;
		}
		if(!All)
		{
			if(0==val)
			{
				CJSONLEN=CJSONL;
			}
		}
	}
	if(!All)
	{
		if(0==val)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:����Ϊ��!!!!!!\n",(u32)&__func__,__LINE__,0);
			#endif
			return 0;
		}
	}
	return CJSONLEN;
Real_JSON_ERROR:
	return 0;
}

u32 RealDataPackNull(u8 *pout,u8 *pAddr)//ȫ��ʵʱ�����������б�
{
	u8 *p8;
	u32 i;
	u32 y;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
	CJSONLEN+=i;
	p8=(u8 *)ADDR_DATABUFF;
	Add_StrToASCII(p8,pAddr,6);//pbuf������ֽ������pstr��ת��asc��
	i=CreateDataHead_Json(pout+CJSONLEN, p8, DEBUG_THIRD_SPACENUM);
	CJSONLEN+=i;
	
	//����ʱ��
	//Get698CurTime_Ascii(p8);
	Get698Time_Ascii(JSON->Lastrealtime,p8);//pʱ��ǰ�����ֽڱ�ʾ���p8���asc��
	GetJsonTime(p8);
	i=CreateData_Json(pout+CJSONLEN, p8);
	CJSONLEN+=i;	
	for(y=1;y<RealDataOAD_JSONSIZE;y++)//y��1��ʼ�� ��Ϊ��һ����ʱ����ǰ���Ѿ�д����
	{
		i=Str_COMMA(pout+CJSONLEN);//����
		CJSONLEN+=i;
	}
	i=CreateDataEnd_Json(pout,CJSONLEN);
	CJSONLEN+=i;
	i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_SECOND_SPACENUM);
	CJSONLEN+=i;
	i=Str_LF(pout+CJSONLEN);
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 RealDataPack(u16 *pAddr,u8 *pd,u32 ROAD,u32 Min,u32 isReal,u32 All)
{
	u32 i;
	u8 *pDATA;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pDATA=Get_MetData_ROAD(pAddr,ROAD,Min,isReal,All);//����ROAD��ȡFlash�е�����,pAddr���ַ,Minȡ���Ӷ����ʱ����,1��ʾ1����15��ʾ15����,����0������,���򷵻�һ������bufָ��
	if(NULL==pDATA)
	{	
		if(All)
		{
			p8=(u8 *)pAddr;
			i=RealDataPackNull(pd,p8);
			ms->ms_free(pDATA);
			pDATA=NULL;	
			return i;
		}
		return 0;	
	}
	p8=(u8 *)pAddr;
	i=RealData_PackJson(pDATA,pd,p8,All);
	ms->ms_free(pDATA);
	pDATA=NULL;	
	return i;
}

u32 DeletFlashData_SelectTime(u32 ROAD,u8 *ptime)//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
{
	u32 i;
	u32 x;
	u32 ret=0;
	switch (ROAD)
	{
		case 0x50020200://���Ӷ���
			//�Ƚ�ʱ������һ���
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			x=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|(ptime[3]);
			if(i>=x)
			{
				ret=1;
			}		
			break;
		case 0x50040200://�ն���
		//�����ʱ���ǵڶ���
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
		//��һ����������ն���
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
			x=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|(ptime[3]);
			if(i>=x)
			{
				ret=1;
			}
			break;
		case 0x50060200://�¶���(����һ���µ�ʱ��)
			//�Ƚ�ʱ������һ���
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8)|JSON->Lastrealtime[2];
			i=YYM_SubM_hex(i, 2);//�������ռ�1��,���ؼ�1�����������
			x=(ptime[0]<<8)|(ptime[1]<<16)|ptime[2];
			if(i>=x)
			{
				ret=1;
			}			
			break;
		case 0x5002020E:
		case 0x50020201:
			i=Compare_DL698DataString(JSON->Lastrealtime,ptime,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i==1)
			{
				ret=1;
			}		
			break;	
		default:
		break;
	}
	return ret;
}

void Delet_All_FromFlash(u32 FileID)
{
	u32 i;
	u32	filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
	Comm_Ram->msFILESearchCount=0;
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
	{	
		filename=Get_RecordFileName_JS(FileID,0xffffffff,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
		if(filename==0xffffffff)
		{
			continue;
		}
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			//file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			continue;
		}
		if(file->BLOCK_ENTRY==0)
		{
			continue;
		}
		switch (FileID)
		{
			case 0x13f11000://�ն���
				//i=Filter_Time(file->save_date_time_s,0x50040200,0,0);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
				i=DeletFlashData_SelectTime(0x50040200,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f13000://�¶���
				//i=Filter_Time(file->save_date_time_s,0x50060200,0,0);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
				i=DeletFlashData_SelectTime(0x50060200,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f16000://�ն���
				//i=Filter_Time(file->save_date_time_s,0x50040200,0,0);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
				i=DeletFlashData_SelectTime(0x50040200,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f18000://�¶���
				//i=Filter_Time(file->save_date_time_s,0x50060200,0,0);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
				i=DeletFlashData_SelectTime(0x50060200,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f19000://15���Ӷ���
				//i=Filter_Time(file->save_date_time_s,0x50020200,0,0);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
				i=DeletFlashData_SelectTime(0x50020200,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f22000://1���Ӷ���
				//i=Filter_Time(file->save_date_time_s,0x50020200,0,0);//ȡ�ļ��Ĵ洢ʱ����й���,�ɹ�����1,ʧ��0
				i=DeletFlashData_SelectTime(0x50020200,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f12000://15���Ӷ���
				//i=1;//Ĭ��ȫ��ɾ��
				i=DeletFlashData_SelectTime(0x5002020E,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			case 0x13f17000://1���Ӷ���
				//i=1;//Ĭ��ȫ��ɾ��
				i=DeletFlashData_SelectTime(0x50020201,file->save_date_time_s);//��ÿһ�δ��֮����ɾ����������(��NANDFLASH),�ɹ��ͷ���1,ʧ�ܷ���0
				break;
			default:
				return;
		}
		if(0==i)
		{
			continue;
		}
		file->BLOCK_ENTRY=0;
		ms->msfile_Delete(filename);
	}
}

void Delet_FileName(void)
{
	Delet_All_FromFlash(0x13f12000);//ɾ�����ʱ�����ļ�
	Delet_All_FromFlash(0x13f17000);//ɾ�����ʱ�����ļ�
}

void Delet_JsonFile(u8 *fanme)//ɾ����ǰ��Realtime.json�ļ�,����ѹ����
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)fanme);
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ɾ��tmp����JSON�ļ��ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}
void Delet_JsonFile_PATH(const char *path)//ɾ��·��path�µ�json�ļ�
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)path);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ɾ��tmp����JSON�ļ��ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}
void Delet_AllData_PATH(const char *path)//ɾ��·��path�µ������ļ�
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)path);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ɾ��tmp����JSON�ļ��ַ�:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}

void Delet_CurPath_JSON_And_TarGz(void)//ɾ����ǰĿ¼�µ�json�ļ���tar.gzѹ���ļ�
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ɾ������:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.gz");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ɾ������:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	#endif
	ms->link_system((char *)p8);
}


void Delet_RealTime_BZ2(void)//ɾ��ʵʱ��ѹ���ļ�,(�ϱ�֮��Ҫɾ��)
{
	u32 i;
	u32 Len;
	u8 *p8;
	u8 *pfilename;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	p8=(u8 *)ADDR_DATABUFF+1024;
	Len=0;	
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)pfilename);
	ms->link_system((char *)p8);
}

void Mkdir_JsonFile(const char *path)//����path·���ļ���
{
	u32 i;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	mystrcpy((char *)p8, "mkdir ");
	i=mystrlen((const char *)p8);
	mystrcpy((char *)p8+i, (const char *)path);
	ms->link_system((char *)p8);
}

u32 Is_MinData_Flash(u32 FileID)//��Flash�ռ�������������ǲ��Ƿ�������,0��ɾ�� 1ɾ��
{
	u32 ret=0;
	//����FileID��ɾ����ͬ���ļ�
	switch(FileID)
	{
		//case 0x13f12000:
		//case 0x13f17000:
		case 0x13f16000://�ն���
		case 0x13f11000:
		case 0x13f18000://�¶���
		case 0x13f13000:
		case 0x13f19000://15���Ӷ���
		//case 0x13f22000://1���Ӷ���
		case 3://�ѱ�����ɾ��
		case 4:
			break;
		default:
			ret=1;
		break;
	}
	return ret;
}

void Delet_MinData_Flash(u32 Delcnt)//��Flash����ɾ��Delcnt���������
{
	u32 i;
	u32 FileCount;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=Get_ms();
	FileCount=0;	
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);	
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(Delcnt<=FileCount)//���ɾ���Ĵ����Ѿ�����,�Ǿ��˳�
		{
			break;
		}
		if(file->BLOCK_ENTRY)
		{
			//i=file->FILECOUNT;
			FileID=file->FILEID;
			i=Is_MinData_Flash(FileID);//��Flash�ռ�������������ǲ��Ƿ�������,0��ɾ�� 1ɾ��
			if(i)
			{
				ms->msfile_Delete(filename);
				FileCount++;
			}
		}
		file++;
	}
}

void Delet_AllFile(u8 *ptime)//ɾ����ʱ�ļ�����������ļ� �������ļ���
{
	u32  i;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	//u8 time[7];
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	i=Check_pYYMDHMS_hex(ptime);
	if(i)
	{
		return;
	}
	
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "mkdir ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "mv ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//�õ�698��ǰʱ��ת����asc��		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+8, "* ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//�ļ�������
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
	Len=0;
	mystrcpy((char *)p8+Len, "mv ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='/';
	p8[Len++]='*';
	p8[Len++]=' ';
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//�ļ�������
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]=' ';
	p8[Len++]='-';
	p8[Len++]='r';
	p8[Len++]='f';
	p8[Len++]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
}

u32 Check_TarFtzFile(u8 *ptime)
{
	u32 i;
	s32 fd;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	i=Check_pYYMDHMS_hex(ptime);
	if(i)
	{
		return 0;
	}
	if(ptime[4]==0&&ptime[5]==0)
	{
		JSON->IsCreateFrzJson=0;
	}
	i=ptime[4]*60+ptime[5];
	if(i<70)//���ʱ����ڲ��������ѹ���ļ�����
	{
		return 0;
	}
	if(JSON->IsCreateFrzJson!=0)
	{
		return 0;
	}
	Mkdir_JsonFile(JSONFREPATH);
	pname=GetFrzFileJsonName_Compress();//�õ�ѹ���������ִ�·��
	fd=ms->link_open((char *)pname,O_CREAT|O_EXCL);//�����ļ��治����
	if(fd!=-1)//˵���ļ�������
	{
		//�Ǿ�˵������û�д��,����˵�Ǵ���Ѿ�ʧ����,��Ҫ�����ٴ��
		//1.ɾ����ǰĿ¼�������е�.json��.tar
		Delet_CurPath_JSON_And_TarGz();
		Delet_AllFile(ptime);//ɾ����һ����ļ�������
		ms->link_close(fd);
		return 1;
	}else
	{
		//�ļ����ڽ���Ͳ�����ȫ���ļ��������
		JSON->IsCreateFrzJson=1;
	}
	return 0;
}

u32 IsTimeTask(u8 *pt)//��ʱ���ж��ǲ��Ǵ������ʱ�䵽��,pt��һ�δ����ʱ��
{
	u64 YYMDHM1;
	u64 YYMDHM2;
	u32 ret=0;
	YYMDHM1=0;
	YYMDHM1=((u64)pt[0]<<32)|((u64)pt[1]<<40)|((u64)pt[2]<<24)|((u64)pt[3]<<16)|((u64)pt[4]<<8)|((u64)pt[5]);
	//����Flash�ļ������ʱ��
	YYMDHM2=0;
	YYMDHM2=((u64)Comm_Ram->DL698YMDHMS[0]<<32)|((u64)Comm_Ram->DL698YMDHMS[1]<<40)|((u64)Comm_Ram->DL698YMDHMS[2]<<24)|((u64)Comm_Ram->DL698YMDHMS[3]<<16)|((u64)Comm_Ram->DL698YMDHMS[4]<<8)|((u64)Comm_Ram->DL698YMDHMS[5]);
	if(JSON->reportfiletime[4]==0&&0==JSON->reportfiletime[5])
	{
		Delet_AllFile(JSON->reportfiletime);
	}
	if(YYMDHM1<YYMDHM2)
	{
		if(0==pt[5])
		{
			return 3;
		}
		return 1;
	}
	if(YYMDHM1>YYMDHM2)
	{
		return 2;
	}
	return ret;
}

u32 Can_GetVaildMetAddr(void)
{
	u8 *p8;
	u16 *p16s;
	u32 i;
	for(i=0;i<NUMmax_6000;i++)
	{
		p8=(u8*)ADDR_6000_SDRAM+i*LENper_6000;
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
			if(p16s[0]==0)
			{
				if(p16s[1]==0)
				{
					if(p16s[2]==0)
					{
						continue;
					}
				}
			}
			break;
		}
	}
	if(i>=NUMmax_6000)
	{
		return 0;
	}
	return 1;
}

void Terminal_FrzPack(void)
{
	u32 i;
	u32 CJSONLEN;//JSON�ַ���������,��ʼ��0
	u32 JSONDataLen;
	u8 *p8;
	u8 *p8s;
	u8 *pAddr;//��ַu8����ָ��
	u8 *pname;
	u16 *p16;
	s32 fd;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	
	
	switch (JSON->PackFrzJsonTask)
	{
		case 0://��ʼ������
			JSON->PackFrzJsonTask++;
			break;
		case 1://ʱ����ж�	
			//����ǲ�����Ϊͣ�ϵ�����Ĵ��ʧ�ܻ���ѹ��ʧ��
			i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
			if(i)
			{
				MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			}			
			i=i=Check_TarFtzFile(JSON->Lastrealtime);//ֻ�й���һ���ӲŻ�ִ����Ӧ������
			if(i!=0)//�ն���������
			{
				#ifdef DEBUG_JSON
				myprintf("==============================================================================\n", 0, 0, 0);
				myprintf("==========================׼����ʼ��������JSON��ʽ�ļ�========================\n", 0, 0, 0);
				myprintf("==============================================================================\n", 0, 0, 0);
				#endif
				JSON->PackFrzJsonTask++;
			}
			break;
		case 2://���֮ǰҪ���Ĺ���
			//TEST_PACK();
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:���ǰʱ��\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			i=Can_GetVaildMetAddr();
			if(i==0)
			{
				JSON->PackFrzJsonTask=0;
				return;
			}
			JSON->CurFrzMetCount=0;
			//���֮ǰ���RAM�������еĿռ�
			Delet_RAM_FrzFile();
			JSON->PackFrzJsonTask++;
			break;
		case 3://�������ڴ�����
			//#ifndef DEBUG_JSON
			i=GetOneMetAddr((u16 *)&JSON->CurFrzMetCount);
			//#else
			//i=GetOneMetAddr_TEST(&JSON->CurFrzMetCount);
			//#endif
			if(i==0)//ȫ�����ַ�Ѿ��������
			{
				JSON->PackFrzJsonTask++;;//����ÿ�����ն���ı�־
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:���ݴ�����Ͽ�ʼѹ��\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				break;
			}
			JSON->CurFrzMetCount++;//��һ��ѭ��������һֻ��
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:JSON->CurFrzMetCount=%d\n",(u32)&__func__,(u32)__LINE__,JSON->CurFrzMetCount);
			#endif
			//�õ��ļ�����
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//����һ���µ���Ŀ�ļ�,�����ļ����
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_END);
				if((i>=RAMTOFLASH_COPYMAXSIZE))
				{
					//��RAM�ռ��ƶ���Flash�ռ�
					i=Copy_RamToFlash(fd);
					if(i!=0)
					{
						ms->link_close(fd);
						return ;
					}
				}
				ms->link_close(fd);
			}
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:����ռ�ʧ��i\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				return ;
			}
			CJSONLEN=0;
			JSONDataLen=0;
			p8s=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_AddrList);
			p16++;//ǰ��һ���Ǳ������,����һ���Ǳ�ĵ�ַ
			pAddr=(u8 *)p16;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			Add_StrToASCII(p8s,pAddr,6);//pbuf������ֽ������pstr��ת��asc��
			i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=AllDataPack(p16,p8+CJSONLEN);
			CJSONLEN+=i;
			if(i>0)//˵���������ݵ�
			{
				i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
				CJSONLEN+=i;
				CJSONLEN=Delet_COMMA(p8,CJSONLEN);//ɾ������
				i=Str_LF(p8+CJSONLEN);
				CJSONLEN+=i;			
			}else
			{		
				if(CJSONLEN>=JSONDataLen)
				{
					CJSONLEN-=JSONDataLen;
				}
			}
			
			if(CJSONLEN>0)//��һֻ��ʾ�����ݵ�
			{
				//���濪ʼ��RAM�ռ�����д����
				i=CalCulate_Space(JSONFREPATH_RAM);//��������ռ�����ʣ�������
				if(i>=CJSONLEN)//���ʣ��Ŀռ乻��
				{
					//�õ��ļ�����
					Mkdir_JsonFile(JSONFREPATH_RAM);
					pname=CreateFreJsonFile_RAM();//����һ���µ���Ŀ�ļ�,�����ļ����
					fd=ms->link_open((char *)pname,O_RDWR);
					if(fd!=-1)
					{
						ms->link_lseek(fd,0,SEEK_END);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}
					}else
					{
						fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
						if(-1==fd)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�����ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							ms->ms_free(p8);
							return;
						}
						ms->link_lseek(fd,0,SEEK_SET);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}
					}
				}else
				{
					
				}
				ms->link_close(fd);//�ر��ļ����
			}
			ms->ms_free(p8);
			break;
		case 4://���ݴ������,׼�������ļ�ѹ��
			//�õ��ļ�����
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:��������꿪ʼѹ��\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//����һ���µ���Ŀ�ļ�,�����ļ����
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_SET);
				//��RAM�ռ��ƶ���Flash�ռ�
				i=Copy_RamToFlash(fd);
				if(i!=0)
				{
					ms->link_close(fd);
					return ;
				}
				ms->link_close(fd);
			}
			Mkdir_JsonFile(JSONFREPATH);
			pname=CreateFreJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				ms->link_close(fd);
				WWDT_Disable();
				Compress_JSONFile(pname,1024,0);//��JSON�ļ������tar.gz��ʽ
				//Delet_Yesterday_Data_Flash();//ɾ��flash�����������е�����
				Delet_JsonFile_PATH(JSONFREPATH_RAM);//ɾ��JSON�ļ�
				JSON->IsCreateFrzJson=1;
				JSON->PackFrzJsonTask=1;
			}else
			{
				//˵�����ʧ����
				if(JSON->IsCreateFrzJson>=1)//��������ݴ��ʧ����
				{
					JSON->IsCreateFrzJson=1;
					JSON->PackFrzJsonTask=1;
					Delet_JsonFile_PATH(JSONFREPATH_RAM);//ɾ��JSON�ļ�
				}
				else
				{
					JSON->IsCreateFrzJson++;//�ٸ�һ�λ�����
				}
			}
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�����ʱ��\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			break;
		default:
			JSON->PackFrzJsonTask=0;
		break;
	}
}

u32 GetOneMetAddr(u16 *MetCount)//�ҵ�һ�����ַ
{
	u8 *p8;
	u16 *p16;
	u16 *p16s;
	#ifdef DEBUG_JSON
	u32 i;
	#endif
	u16 Met=*MetCount;
	p16=(u16*)(ADDR_AddrList+2);
	if(Met>=NUMmax_6000)
	{
		return 0;
	}
	for(;Met<NUMmax_6000;Met++)
	{
		p8=(u8*)ADDR_6000_SDRAM+(Met)*LENper_6000;
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
			p16[0]=p16s[0];
			p16[1]=p16s[1];
			p16[2]=p16s[2];
			break;
		}
	}
	if(Met>=NUMmax_6000)
	{
		return 0;
	}
	*MetCount=Met;
	p16=(u16*)ADDR_AddrList;
	p16[0]=1;

	p8=(u8 *)(ADDR_AddrList+2);
	#ifdef DEBUG_JSON
	for(i=6;i>0;i--)
	{
		myprintf("%02x",p8[i-1],0,0);
	}
	myprintf("\n",0,0,0);
	#endif
	return 1;
}

void Terminal_RealPack(void)
{
	s8 *pc="dateTime(数据时间),20000201(A相电压),20000202(B相电压),20000203(C相电压),20010201(A相电流),20010202(B相电流),20010203(C相电流),20010400(零线电流),20010500(剩余电流),20040201(总有功功率),20040202(A相有功功率),20040203(B相有功功率),20040204(C相有功功率),43140506(室外温度),43140505(室内温度),43140501(空调设置温度),43140400(空调开关机状态),43110502(热水器加热温度),43110400(热水器开关机状态),00100201(正向有功总电能),00100203(正向有功峰电能),00100205(正向有功谷电能),00200201(反向有功总电能),00200203(反向有功峰电能),00200205(反向有功谷电能)";
	s8 *pc1="]";
	u32 i;
	//u32 x;
	u32 y;
	u32 MAXMETNUM=1;
	u32 CJSONLEN;
	u8 *p8;                               
	u8 *pname;
	u16 *p16;
	//u16 AddrNUM;
	s32 fd;
	ms_Type *ms;	
	ms=(ms_Type *)Get_ms();
	switch (JSON->PackRealJsonTask)
	{
		case 0://��ʼ������
			Delet_FileName();//ɾ�����ʱ�����ļ�
			pYYMDHMS_AddS_hex(JSON->Lastrealtime,60);//ÿִ��һ�δ������֮�� ���Ӵ����ʱ���־����1����
			JSON->PackRealJsonTask++;
			break;
		case 1://ʱ����ж�	
			//����ǲ�����Ϊͣ�ϵ�����Ĵ��ʧ�ܻ���ѹ��ʧ��
			i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
			if(i)
			{
				MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			}			
			i=IsTimeTask(JSON->Lastrealtime);//ֻ�й���һ���ӲŻ�ִ����Ӧ������
			switch (i)
			{
				case 1:
					JSON->PackRealJsonTask++;
					JSON->Is_AllRealData=0;
					break;
				case 2:
					MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
					break;
				case 3:
					JSON->PackRealJsonTask++;
					JSON->Is_AllRealData=1;
					break;
				default:
					break;
			}	
			break;
		case 2://ʵʱ�ļ���дһ��ͷ
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:ʵʱʱ���־\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",JSON->Lastrealtime[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			i=Can_GetVaildMetAddr();
			if(i==0)
			{
				JSON->PackRealJsonTask=0;//���¿�ʼ������
				return;
			}
			JSON->CurRealMetCount=0;
			//�ȴ���ʵʱ�ļ���ͷ
			p8=(u8 *)ms->ms_calloc(4*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc�ռ䲻��\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				return ;
			}
			CJSONLEN=0;
			i=HeadStart_JSON(p8+CJSONLEN);
			CJSONLEN+=i;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			i=CreateDataHead_Json(p8+CJSONLEN, (u8 *)STR_COLOUMNHEAD, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=CreateData_Json(p8+CJSONLEN, (u8 *)pc);
			CJSONLEN+=i;
			i=CreateDataEnd_Json(p8,CJSONLEN);
			CJSONLEN+=i;
			i=CreateArrayEnd_Json(p8, CJSONLEN,DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			i=Str_LF(p8+CJSONLEN);
			CJSONLEN+=i;
			Mkdir_JsonFile(JSONREALPATH);//��������ļ���
			pname=CreateRealJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����()
			fd=ms->link_open((char *)pname,O_RDWR);
			if(-1!=fd)//�Ѿ����ڸ��ļ���ɾ��
			{
				ms->link_remove((char *)pname);
				#ifdef DEBUG_JSON
				myprintf("\n\n[%s:%d]:ɾ���ļ�:%s \n\n", (u32)&__func__, __LINE__, (u32 )pname);
				#endif
			}
			ms->link_close(fd);
			fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
			if(-1==fd)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:�����ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				ms->ms_free(p8);
				JSON->PackRealJsonTask=0;//���¿�ʼ������
				return;
			}
			y=ms->link_write(fd,(char *)p8,CJSONLEN);
			if(y==CJSONLEN)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
				#endif
				JSON->PackRealJsonTask++;
			}else
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����y=%d\n",(u32)&__func__,(u32)__LINE__,y);
				#endif
			}
			ms->link_close(fd);
			ms->ms_free(p8);
			break;
		case 3://���ݴ������
			while(MAXMETNUM>0)
			{
			MAXMETNUM--;	
			i=GetOneMetAddr((u16 *)&JSON->CurRealMetCount);
			//i=GetOneMetAddr_TEST(JSON->CurRealMetCount);
			if(i==0)//ȫ�����ַ�Ѿ��������
			{
				//���Ǵ��ȫ���ļ�,�Ǿ���û�б�������
				Delet_JsonFile_PATH(JSONREALPATH);
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:û��һֻ��������\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//���¿�ʼ������
				break;
			}
			JSON->CurRealMetCount++;//��һ��ѭ��������һֻ��		
			p16=(u16*)(ADDR_AddrList);
			p16++;	
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc�ռ䲻��\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//���¿�ʼ������
				return;
			}
			CJSONLEN=0;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 15, 1,JSON->Is_AllRealData);
			CJSONLEN+=i;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 1, 1,JSON->Is_AllRealData);
			CJSONLEN+=i;

			if((CJSONLEN>0))
			{
				Mkdir_JsonFile(JSONREALPATH);//��������ļ���
				pname=CreateRealJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����()
				fd=ms->link_open((char *)pname,O_RDWR);
				if(-1==fd)//����������ļ�
				{
					ms->ms_free(p8);
					JSON->PackRealJsonTask=2;
					return;
				}
				ms->link_lseek(fd,0,SEEK_END);
				y=ms->link_write(fd,(char *)p8,CJSONLEN);
				if(y==CJSONLEN)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
					#endif
					JSON->PackRealJsonTask=0xff;
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����y=%d\n",(u32)&__func__,(u32)__LINE__,y);
					#endif
				}
				ms->link_close(fd);
			}else
			{
				//myprintf("[%s:%d]:û������\n",(u32)&__func__,(u32)__LINE__,0);
			}
			ms->ms_free(p8);
			if(JSON->PackRealJsonTask==0xff)
			{
				break;
			}
			}
			break;
		case 4://��ʼѹ������
			Mkdir_JsonFile(JSONREALPATH);//��������ļ���
			pname=CreateRealJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����()
			fd=ms->link_open((char *)pname,O_RDWR);
			if(-1==fd)//����������ļ�
			{
				JSON->PackRealJsonTask=2;
				return;
			}
			i=ms->link_lseek(fd,0,SEEK_END);
			if(i>1)
			{
				ms->link_lseek(fd,i-2,SEEK_SET);
			}
			y=ms->link_write(fd,pc1,1);
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:y=%d\n",(u32)&__func__,(u32)__LINE__,y);
			#endif
			if(y==1)
			{
				Compress_JSONFile(pname,1024,1);//��JSON�ļ������tar.gz��ʽ
				//ɾ��Flash�����filename
				SetReportInfo();//�����ϱ���Ϣ
			}else
			{
				y=ms->link_write(fd,(char *)p8,1);
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:y=%d\n",(u32)&__func__,(u32)__LINE__,y);
				#endif
				if(y==1)
				{
					Compress_JSONFile(pname,1024,1);//��JSON�ļ������tar.gz��ʽ
					//ɾ��Flash�����filename
					SetReportInfo();//�����ϱ���Ϣ
				}
			}
			JSON->PackRealJsonTask=0;//���¿�ʼ������
			ms->link_close(fd);
			break;
		case 0xff://���ݴ������
			while (MAXMETNUM>0)
			{
			MAXMETNUM--;
			i=GetOneMetAddr((u16 *)&JSON->CurRealMetCount);
			//i=GetOneMetAddr_TEST(JSON->CurRealMetCount);
			if(i==0)//ȫ�����ַ�Ѿ��������
			{
				JSON->PackRealJsonTask=4;;//��ʼѹ��
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:���ݴ�����Ͽ�ʼѹ��\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				break;
			}
			JSON->CurRealMetCount++;//��һ��ѭ��������һֻ��		
			p16=(u16*)(ADDR_AddrList);
			p16++;	
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc�ռ䲻��\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//���¿�ʼ������
				return ;
			}
			CJSONLEN=0;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 15, 1,JSON->IsCreateFrzJson);
			CJSONLEN+=i;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 1, 1,JSON->IsCreateFrzJson);
			CJSONLEN+=i;

			if(CJSONLEN>0)
			{
				Mkdir_JsonFile(JSONREALPATH);//��������ļ���
				pname=CreateRealJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����()
				fd=ms->link_open((char *)pname,O_RDWR);
				if(-1==fd)//����������ļ�
				{
					ms->ms_free(p8);
					JSON->PackRealJsonTask=2;
					return;
				}
				ms->link_lseek(fd,0,SEEK_END);
				y=ms->link_write(fd,(char *)p8,CJSONLEN);
				if(y==CJSONLEN)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
					#endif
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����y=%d\n",(u32)&__func__,(u32)__LINE__,y);
					#endif
				}
				ms->link_close(fd);
			}	
			ms->ms_free(p8);
			}
			break;
		default:
			JSON->PackRealJsonTask=0;
		break;
	}		
}

void Terminal_PackJSON(void)//�ն˴��JSON�ļ�����
{
	u32 i;
	if(FileDownload->New==0)//Զ���������,���������
	{
		return;
	}
	switch (JSON->PackJsonTask)
	{
		case 0://��ʼ������
			JSON->IsCreateFrzJson=0;//���ϵ��ж����ֵ��ʼ��Ϊ0
			i=Check_pYYMDHMS_hex(Comm_Ram->DL698YMDHMS);
			if(i)
			{
				return;
			}	
			MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->reportfiletime, sizeof(JSON->reportfiletime));
			//Mkdir_JsonFile();//�����ļ���
			//TEST_PACK();
			//tar_thread();
			//tar_text();
			//Crl_AllJsonFileAndTar();
			#ifdef DEBUG_JSON
			//log_Flash();
			//test_saveMIN15();
			//test_saveMIN();
			//test_saveDAY();
			#endif
			//TEST_RAM() ;
			//return;
			JSON->PackRealJsonTask=1;//ʵʱ��1��ʼ
			JSON->PackJsonTask++;
			break;
		case 1:
			Terminal_FrzPack();//�ն������ݴ��
			Terminal_RealPack();//ʵʱ���ݴ��
			break;
		default:
			JSON->PackJsonTask=0;
		break;
	}
	
}
		
//=======================================ȫ��ʵʱ�����ϱ�=====================================
void SetReportInfo(void)//�ϱ������0��ʼ,�����ϱ�
{
	JSON->g_ReportFlag=1;
}
u8 GetReportInfo(void)//�õ��ǲ�����Ҫ�ϱ���û�������µ��ϱ��ļ�
{
	return JSON->g_ReportFlag;	
}
void ClrReportInfo(void)//����ϱ���־,���ϱ��ļ���
{
	JSON->Reportjsonfile_Task=0;
	JSON->g_ReportFlag=0;
	MC(0,(u32)JSON->RealFileName,sizeof(JSON->RealFileName));
	pYYMDHMS_AddS_hex(JSON->reportfiletime,60);//ÿִ��һ�δ������֮�� ���Ӵ����ʱ���־����1����
}
u32 IsReportEnd(void)//�ǲ����ϱ��ɹ�,���ϱ���=�Ѿ��ϱ���
{
	if(JSON->g_Block_Number==JSON->g_Need_report_Block)
	{
		return 1;
	}
	return 0;
}
void Next_DataBlock(void)//ָ����һ���ϱ������ݿ�
{
	JSON->g_Block_Number++;
}

u32 Merge_RealTimeData_JsonFile(u8 *ptime)//�ϲ������ϱ�ѹ���ļ���Ϊһ���µ�ѹ���ļ�
{
	u64 YYMDHM1;
	u64 YYMDHM2;
	u32 i;
	u32 n;
	u8 isemptyfile;//����ҵ����ǵ�һ���ļ�
	u8 *pfilename;
	s32 fd1=-1;
	s32 fd_new=-1;
	u8 *p8;
	//u8 *paddr;
	u32 Len;
	u8 time[7];
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	MW((u32)ptime,(u32)time,sizeof(time));
	i=Check_pYYMDHMS_hex(time);
	if(i)
	{
		return 0;
	}
	pYYMDHMS_SubS_hex(time,60);//ÿִ��һ�δ������֮��
	isemptyfile=1;
	pfilename=GetFileJsonName_Compress(time);//�õ��ļ���·��	
	fd_new=ms->link_open((char *)pfilename,O_RDWR);
	if(-1==fd_new)
	{
		//����һ���µ��ļ�
		p8=GetRealTime_FileJson(time);
		//����һ���µ��ļ�,�ļ������ָ��µ����µĴ��ʱ��
		fd_new=ms->link_open((char *)p8,O_CREAT|O_RDWR);
		if(-1==fd_new)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�����ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		ms->link_lseek(fd_new,0,SEEK_SET);
		isemptyfile=0;
	}else
	{
		ms->link_close(fd_new);//�����ͷŵ�����ļ����
		//��ʼ��ѹ��
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "tar jxvf ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:��ѹ�ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
		#endif
		ms->link_system((char *)p8);

		p8=GetRealTime_FileJson(time);

		fd_new=ms->link_open((char *)p8,O_RDWR);
		if(-1==fd_new)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�����ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		isemptyfile=1;
	}
	
	YYMDHM2=0;
	YYMDHM2=((u64)JSON->reportfiletime[0]<<32)|((u64)JSON->reportfiletime[1]<<40)|((u64)JSON->reportfiletime[2]<<24)|((u64)JSON->reportfiletime[3]<<16)|((u64)JSON->reportfiletime[4]<<8)|((u64)JSON->reportfiletime[5]);
	while(1){
		pYYMDHMS_SubS_hex(time,60);//ÿִ��һ�δ������֮�� ���Ӵ����ʱ���־����1����
		YYMDHM1=0;
		YYMDHM1=((u64)time[0]<<32)|((u64)time[1]<<40)|((u64)time[2]<<24)|((u64)time[3]<<16)|((u64)time[4]<<8)|((u64)time[5]);
		
		if(YYMDHM2>YYMDHM1)
		{
			break;
		}	
		//�������Ǹ�ʱ�����ļ���ʼ��
		pfilename=GetFileJsonName_Compress(time);//�õ��ļ���·��	
		fd1=ms->link_open((char *)pfilename,O_RDWR);
		if(-1!=fd1)
		{
			ms->link_close(fd1);//�����ͷŵ�����ļ����
			//��ʼ��ѹ��
			p8=(u8 *)ADDR_DATABUFF+1024;
			Len=0;
			mystrcpy((char *)p8+Len, "tar jxvf ");
			i=mystrlen((const char *)p8+Len);
			Len+=i;
			mystrcpy((char *)p8+Len, (const char *)pfilename);
			i=mystrlen((const char *)p8+Len);
			Len+=i;
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:��ѹ�ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
			#endif
			ms->link_system((char *)p8);
			
			//������ļ�
			p8=GetRealTime_FileJson(time);

			fd1=ms->link_open((char *)p8,O_RDWR);
			if(-1!=fd1)
			{
				Len=ms->link_lseek(fd1,0,SEEK_END);
				if(Len)
				{	
					p8=(u8 *)ms->ms_malloc(Len);
					if(p8)
					{	
						if(0==isemptyfile)//�ҵ��˵�һ���ļ�,//��Ҫ�������е��ļ���Ϣ
						{
							isemptyfile=1;//�������ҵĶ����ǵ�һ���ļ� Ҫ�����⴦��
							ms->link_lseek(fd1,0,SEEK_SET);
							i=ms->link_read(fd1,(u8 *)p8,Len);
							if(i==Len)
							{
								i=ms->link_write(fd_new,(u8 *)p8,Len);
							}
						}else
						{
							ms->link_lseek(fd1,0,SEEK_SET);
							i=ms->link_read(fd1,(u8 *)p8,Len);	
						
							if(i==Len&&Len>0)
							{
								for(i=0;i<Len-1;i++)
								{
									if(p8[i]==',')
									{
										if(p8[i+1]==0x0A)//���з���
										{
											break;
										}
									}
								}
								if(i<Len)
								{
									n=ms->link_lseek(fd_new,0,SEEK_END);
									if(n>0)
									{
										ms->link_lseek(fd_new,n-1,SEEK_SET);
										Len-=i;
										ms->link_write(fd_new,(u8 *)p8+i,Len);
									}	
								}
								
							}
						}
						ms->ms_free(p8);
						p8=NULL;
					}
				}
				ms->link_close(fd1);//�����ͷŵ�����ļ����
			}
		}
		
	}
	
	Len=ms->link_lseek(fd_new,0,SEEK_END);
	if(Len)
	{	
		MW((u32)ptime,(u32)JSON->reportfiletime,sizeof(JSON->reportfiletime));
		i=Check_pYYMDHMS_hex(JSON->reportfiletime);
		if(i)
		{
			ms->link_close(fd_new);
			return 0;
		}
		pYYMDHMS_SubS_hex(JSON->reportfiletime,60);//ÿִ��һ�δ������֮��
		//�õ����ļ����ļ�����
		pfilename=GetRealTime_FileJson_Pack(JSON->reportfiletime);
		
		//ѹ���µ��ļ�
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "tar jcvf ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;	
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, " ");
		Len++;

		n=mystrlen((const char *)pfilename);
		for(i=0;i<n;i++)
		{
			if(pfilename[i]=='.')
			{
				break;
			}
		}
		if(i>=n)
		{
			ms->link_close(fd_new);
			return 0;
		}
		
		mystrcpy((char *)pfilename+i, ".json");
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:��ѹ�ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
		#endif
		ms->link_system((char *)p8);
		//�ƶ��ļ�����Ӧ��Ŀ¼����
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "mv *.tar.bz2 ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;	
		mystrcpy((char *)p8+Len, JSONREALPATH);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, " -f");
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		ms->link_system((char *)p8);
	}
	//ɾ������ǰJson�ļ�
	
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm *.json");
	i=mystrlen((const char *)p8+Len);
	Len+=i;	
	ms->link_system((char *)p8);
	
	ms->link_close(fd_new);
	return 1;
}
   
u32 Selec_ReportTime(void)//�ϱ�ʱ���ɾѡ
{
	u32 i;
	u8 time[7];
	i=Compare_DL698DataString(JSON->Lastrealtime,JSON->reportfiletime,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
	if(i==0)
	{
		return 1;
	}
	if(i==1)
	{
		if(JSON->Lastrealtime[5]==0)
		{
			MR((u32)time,(u32)JSON->reportfiletime,7);
			pYYMDHMS_AddS_hex(time,300);//ÿִ��һ�δ������֮�� ���Ӵ����ʱ���־����1����
			i=Compare_DL698DataString(JSON->Lastrealtime,time,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i!=2)
			{
				//����10����֮�ڵĶ���ƴ��һ���ϱ�һ���ļ�
				Merge_RealTimeData_JsonFile(JSON->Lastrealtime);
			}
		}
		return 1;
	}
	return 0;
}

u8 *GetFileJsonName(u8 *ptime)//�õ��ϱ��ļ���ѹ�������� (����·��)
{
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	Len=0;
	p8=(u8 *)ADDR_DATABUFF;
	//������֮�����ļ�������,���ϴ���ʱ�����
	Len=0;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:��֡�����ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
	#endif
	return p8;
}

u8 *GetFileJsonName_Compress(u8 *ptime)//�õ��ϱ��ļ�ѹ����������(������·��)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;	
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:�ϱ��ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u8 *GetFrzFileJsonName_Compress(void)//�õ��ϱ��ļ�ѹ����������(������·��)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//�õ�698��ǰʱ��ת����asc��		
	mystrcpy((char *)p8+Len+8, ".tar.gz");
	//myprintf("[%s:%d]:�ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	return p8;
}

u8 *GetRealTime_FileJson(u8 *ptime)//�õ��ϱ��ļ�ѹ����������(������·��)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	//mystrcpy((char *)p8+Len, JSONREALPATH);
	//i=mystrlen((const char *)p8);
	//Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//�õ�698��ǰʱ��ת����asc��		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".json");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:�ϱ��ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u8 *GetRealTime_FileJson_Pack(u8 *ptime)//�õ��ϱ��ļ�ѹ����������(������·��)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	//mystrcpy((char *)p8+Len, JSONREALPATH);
	//i=mystrlen((const char *)p8);
	//Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//д��ַ
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf������ֽ������pstr��ת��asc��
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//дʱ��
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//�õ�698��ǰʱ��ת����asc��		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:�ϱ��ļ�������  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u32 AnalyzeReportFile(void)//�����ϱ����ļ�,(�ļ���С,�ļ�������)
{
	u8 *pfilename;
	s32 fd;
	u32 i;
	u32 filesize;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();

	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		return 0;
	}
	fd=ms->link_open((char *)pfilename,O_RDWR);
	if(-1==fd)
	{
		i=Compare_DL698DataString(JSON->Lastrealtime,JSON->reportfiletime,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
		if(i==1)
		{
			pYYMDHMS_AddS_hex(JSON->reportfiletime, 60);
		}
		return 0;
	}
	#ifdef DEBUG_JSON
	myprintf("===============��ʼ�����ļ�============= \n",0,0,0);
	#endif
	filesize=ms->link_lseek(fd,0,SEEK_END);
	if(0==filesize)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:�ļ�Ϊ��\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	JSON->g_Filesize=filesize;
	//��Ҫ�ϴ�������
	JSON->g_Need_report_Block=filesize/REPORT_FILEJSON_DATASIZE;
	//�����ϴ�һ��
	if(0==JSON->g_Need_report_Block)
	{
		JSON->g_Need_report_Block=1;
	}else
	{
		i=filesize%REPORT_FILEJSON_DATASIZE;
		if(i!=0)
		{
			JSON->g_Need_report_Block++;//���������ټ�һ��
		}
	}
	JSON->g_Block_Number=0;
	ms->link_close(fd);
	
	return 1;
}

u32 GetCurBlockSize(void)//������һ���ϱ����ݴ�С
{
	u32 LEN;
	//07 01 08 f0 01 08 00 02 02 12 00 00 09
	LEN=0;
	if(JSON->g_Filesize>((JSON->g_Block_Number+1)*REPORT_FILEJSON_DATASIZE))
	{
		LEN=REPORT_FILEJSON_DATASIZE;
	}else
	{
		if(JSON->g_Filesize>(JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE))
		{
			LEN=JSON->g_Filesize-(JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE);
		}else
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:������Ŵ��� JSON->g_Block_Number=%d\n",(u32)&__func__,(u32)__LINE__,JSON->g_Block_Number);
			#endif
			return 0;
		}
	}
	return LEN;
}

u32 GetCurBlockData(u8 *pout,u32 Len)//�����ϱ���һ���������
{
	u8 *pfilename;
	s32 fd;
	u32 i;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	if(0==Len)
	{
		return 0;
	}
	//ms->link_system("cd /tmp/real_data");
	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		return 0;
	}
	//fd=ms->link_open((char *)JSON->RealFileName,O_RDONLY);
	fd=ms->link_open((char *)pfilename,O_RDONLY);
	if(-1==fd)
	{
		return 0;
	}
	ms->link_lseek(fd,JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE,SEEK_SET);
	i=ms->link_read(fd,(u8 *)pout,Len);
	if(i!=Len)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:���ϱ��ļ�ʧ�� i=%d\n",(u32)&__func__,(u32)__LINE__,i);
		#endif
		return 0;
	}
	ms->link_close(fd);
	return i;
}

u32 File_HCS(void)//����ļ���У��(�㲻��������Ӧ��Ҳ���Ǻܴ�)
{
	s32 fd;
	u32 i;
	u32 Len;
	u32 HCS;
	u8 *p8;
	u8 *pfilename;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	

	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:ȡ�ļ���ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	fd=ms->link_open((char *)pfilename,O_RDONLY);
	if(-1==fd)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:���ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	i=ms->link_lseek(fd,0,SEEK_END);
	p8=(u8 *)ms->ms_malloc(i);
	if(!p8)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:����ռ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	ms->link_lseek(fd,0,SEEK_SET);
	Len=ms->link_read(fd,(u8 *)p8,i);
	if(Len!=i)
	{
		ms->link_close(fd);
		ms->ms_free(p8);
		p8=NULL;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:�����ݳ��Ȳ���i=%d,",(u32)&__func__,(u32)__LINE__,i);
		myprintf("Len=%d\n",Len,0,0);
		#endif
		return 0;
	}
	HCS=0xffff;
	for(i=0;i<Len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; //complement
	
	ms->link_close(fd);
	ms->ms_free(p8);
	p8=NULL;
	return HCS;
}

u32 Report_FileInfo(u16 *pTime,u32 PORTn)//F0010700//��������
{
	u32 i;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//��󻹿ɷ��͵��ֽ���
	//u8* p8;
	u8* p8tx;
	u8* pfilename;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ
	
	
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
	ClientConnectInfo=ClientConnectInfo;
#else
	//�ͻ������ջ��峤��
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//��Ϊ0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif


UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

//APDU��88 04 01 01 40 00 02 00 01 1C 07 E0 0B 0E 00 02 00 00 00	
		p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
		p8tx[0]=0x88;//�ϱ�֪ͨ REPORT-Notification
		p8tx[1]=0x04;//֪ͨ����ͻ��������ϱ�͸������
		i=UARTCtrl->REPORT_PIID;
		i++;
		i&=0x3f;
		UARTCtrl->REPORT_PIID=i;
		p8tx[2]=i;//PIID
//--------------�ϱ�������������֡��֡�����ַ�����ʽ������������������֡���ݹ̶��������ַ������ܳ��ȿ��Թ̶�------//			
		p8tx[3]=0x81;
		p8tx[4]=0x89;//������������֡���ַ�������
		
		p8tx[5]=0x07;//����7�������ļ�����
		p8tx[6]=0x01;
		p8tx[7]=i;//PIID
		
		p8tx[8]=0xf0;
		p8tx[9]=0x01;
		p8tx[10]=0x07;
		p8tx[11]=0x00;//�ļ�����OMD=F0010700
		
		p8tx[12]=0x02;
		p8tx[13]=0x03;//�����ṹ��
		
		p8tx[14]=0x02;
		p8tx[15]=0x06;//�ļ���Ϣ�ṹ��
		
		p8tx[16]=DataType_visible_string;//Դ�ļ�
		p8tx[17]=0x2f;
		LEN_TxAPDU=18;
		LENmax_TxSPACE-=LEN_TxAPDU+2;//����2byte(1byteʱ���ǩ,1byte�ϱ���ϢFollowReport OPTIONAL��

		pfilename=GetFileJsonName(JSON->reportfiletime);
		if(!pfilename)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:�ļ������name�д�\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		i=mystrlen((const char*)pfilename);
		MW((u32 )pfilename,(u32)p8tx+LEN_TxAPDU,i);
		LEN_TxAPDU+=i;
		LENmax_TxSPACE-=i;
		p8tx[LEN_TxAPDU]=DataType_visible_string;//Ŀ���ļ�
		p8tx[LEN_TxAPDU+1]=0x2f;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		i=mystrlen((const char*)pfilename);
		MW((u32 )pfilename,(u32)p8tx+LEN_TxAPDU,i);
		LEN_TxAPDU+=i;
		LENmax_TxSPACE-=i;

		p8tx[LEN_TxAPDU]=0x06;//�ļ���С
		LEN_TxAPDU++;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>24)&0xff;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>16)&0xff;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>8)&0xff;
		p8tx[LEN_TxAPDU++]=JSON->g_Filesize&0xff;
		LENmax_TxSPACE-=5;

		p8tx[LEN_TxAPDU]=0x04;//�ļ�����
		p8tx[LEN_TxAPDU+1]=0x08;//����
		p8tx[LEN_TxAPDU+2]=0x80;
		LEN_TxAPDU+=3;
		LENmax_TxSPACE-=3;
		
		p8tx[LEN_TxAPDU]=0x0a;
		p8tx[LEN_TxAPDU+1]=0x04;
		p8tx[LEN_TxAPDU+2]=0x56;
		p8tx[LEN_TxAPDU+3]=0x31;
		p8tx[LEN_TxAPDU+4]=0x2e;
		p8tx[LEN_TxAPDU+5]=0x30;
		LEN_TxAPDU+=6;
		LENmax_TxSPACE-=6;
		//
		p8tx[LEN_TxAPDU]=0x16;
		p8tx[LEN_TxAPDU+1]=0;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		p8tx[LEN_TxAPDU]=0x12;//������С
		p8tx[LEN_TxAPDU+1]=REPORT_FILEJSON_DATASIZE/256;
		p8tx[LEN_TxAPDU+2]=REPORT_FILEJSON_DATASIZE%256;
		LEN_TxAPDU+=3;
		LENmax_TxSPACE-=3;
		
		p8tx[LEN_TxAPDU]=0x02;//У��
		p8tx[LEN_TxAPDU+1]=0x02;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		p8tx[LEN_TxAPDU]=0x16;
		p8tx[LEN_TxAPDU+1]=0;
		p8tx[LEN_TxAPDU+2]=0x09;
		p8tx[LEN_TxAPDU+3]=0x02;
		//����У��
		i=File_HCS();
		p8tx[LEN_TxAPDU+4]=(i>>8)&0xff;
		p8tx[LEN_TxAPDU+5]=i&0xff;
		p8tx[LEN_TxAPDU+6]=0x0;
		LEN_TxAPDU+=7;
		LENmax_TxSPACE-=7;
		
		p8tx[LEN_TxAPDU+0]=0;//ʱ���ǩ
		p8tx[LEN_TxAPDU+1]=0;//�ϱ���ϢFollowReport OPTIONAL
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;

//----------------��������END------------------------// 		

			p8tx-=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=0x68;
			//2byte������L
			p8tx[3]=0x83;//������C
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			p8tx[11]=0;//�ͻ�����ַCA
			//2byte֡ͷУ��HCS
			MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			//2byteУ��
			LEN_Tx=14+LEN_TxAPDU+2;
			p8tx[LEN_Tx]=0x16;
			LEN_Tx++;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=(LEN_Tx)&0xff;
			p8tx[2]=(LEN_Tx>>8)&0xff;

			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����

			UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
			//����֡������DataBuff+LEN_UARTnTx,�����ط�
			i=Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
			i+=Get_LEN_UARTnTx(PORTn);
			MW((u32)p8tx,i,LEN_Tx+2);
			//�ϱ���Ӧ��ʱʱ��	TI
			pTime[0]=0;//Ĭ����Сֵ
			UARTCtrl->REPORT_OVER_SECOND=0;//�ϱ���Ӧ��ʱʱ��Ĵ�
			UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
			UARTCtrl->HostACK=8;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
			//����ϱ�����
			#ifdef DEBUG_JSON
			myprintf("======================�����ϱ��ֽ���:%d==========\n",UARTCtrl->TxByte,0,0);
			//create_file();
			for(i=0;i<UARTCtrl->TxByte;i++)
				myprintf("%02x ",p8tx[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			return 1;
}

u32 Report_FileData(u16 *pTime,u32 PORTn)//F0010800//�ϱ���������
{
	u8 *p8tx;
	u32 i;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u16 filesize;
	u32 LENmax_TxSPACE;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef* ClientConnectInfo;
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
	#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
	ClientConnectInfo=ClientConnectInfo;
	#else
		//�ͻ������ջ��峤��
		ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
		i=ClientConnectInfo->LEN_Rx;
		i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		if(i<2048)
		{
			i=2048;
		}
		if(i>(TxAPDUOffset+1))
		{//��Ϊ0
			i-=(TxAPDUOffset+1);
			if(LENmax_TxSPACE>i)
			{
				LENmax_TxSPACE=i;
			}
		}
	#endif
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	LEN_TxAPDU=0;
	p8tx[LEN_TxAPDU++]=0x88;
	p8tx[LEN_TxAPDU++]=0x04;
	i=UARTCtrl->REPORT_PIID;
	i++;
	i&=0x3f;
	UARTCtrl->REPORT_PIID=i;
	p8tx[LEN_TxAPDU++]=i;//PIID
	
	filesize=GetCurBlockSize();
	//����
	if(256>16+filesize)
	{
		p8tx[LEN_TxAPDU++]=0x81;//
		p8tx[LEN_TxAPDU++]=16+filesize;//����
	}else
	{		
		p8tx[LEN_TxAPDU++]=0x82;//
		p8tx[LEN_TxAPDU++]=(17+filesize)>>8;//����
		p8tx[LEN_TxAPDU++]=(17+filesize)&0xff;//����	
	}
	
	p8tx[LEN_TxAPDU++]=0x07;
	p8tx[LEN_TxAPDU++]=0x01;
	p8tx[LEN_TxAPDU++]=i;

	p8tx[LEN_TxAPDU++]=0xf0;
	p8tx[LEN_TxAPDU++]=0x01;
	p8tx[LEN_TxAPDU++]=0x08;
	p8tx[LEN_TxAPDU++]=0x00;

	p8tx[LEN_TxAPDU++]=0x02;
	p8tx[LEN_TxAPDU++]=0x02;
	p8tx[LEN_TxAPDU++]=0x12;
	p8tx[LEN_TxAPDU++]=JSON->g_Block_Number>>8;
	p8tx[LEN_TxAPDU++]=JSON->g_Block_Number&0xff;

	p8tx[LEN_TxAPDU++]=0x09;
	if(LENmax_TxSPACE<LEN_TxAPDU+REPORT_FILEJSON_DATASIZE)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:�ַ����ȳ�\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	LENmax_TxSPACE-=(LEN_TxAPDU+REPORT_FILEJSON_DATASIZE);
	//���ļ�����ȡ����
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:�ַ�����filesize=%d\n",(u32)&__func__,(u32)__LINE__,filesize);
	#endif
	if(filesize<256)
	{
		p8tx[LEN_TxAPDU++]=0x81;
		p8tx[LEN_TxAPDU++]=filesize;
	}else
	{
		p8tx[LEN_TxAPDU++]=0x82;
		p8tx[LEN_TxAPDU++]=filesize/256;
		p8tx[LEN_TxAPDU++]=filesize%256;
	}
	GetCurBlockData(p8tx+LEN_TxAPDU,filesize);
	LEN_TxAPDU+=filesize;	
	p8tx[LEN_TxAPDU++]=0;
	
	p8tx[LEN_TxAPDU++]=0;//ʱ���ǩ
	p8tx[LEN_TxAPDU++]=0;//�ϱ���ϢFollowReport OPTIONAL
				
	//----------------��������END------------------------//			

	p8tx-=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	p8tx[0]=0x68;
	//2byte������L
	p8tx[3]=0x83;//������C
	p8tx[4]=5;//TSA
	MR((u32)p8tx+5,ADDR_4001+2,6);
	Data_Inverse((u32)p8tx+5,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	p8tx[11]=0;//�ͻ�����ַCA
	//2byte֡ͷУ��HCS
	MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
	//2byteУ��
	LEN_Tx=14+LEN_TxAPDU+2;
	p8tx[LEN_Tx]=0x16;
	LEN_Tx++;
	UARTCtrl->TxByte=LEN_Tx;
	LEN_Tx-=2;
	p8tx[1]=(LEN_Tx)&0xff;
	p8tx[2]=(LEN_Tx>>8)&0xff;
	
	Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	
	UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
	//����֡������DataBuff+LEN_UARTnTx,�����ط�
	i=Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
	i+=Get_LEN_UARTnTx(PORTn);
	MW((u32)p8tx,i,LEN_Tx+2);
	//�ϱ���Ӧ��ʱʱ��  TI
	pTime[0]=0;//Ĭ����Сֵ
	UARTCtrl->REPORT_OVER_SECOND=0;//�ϱ���Ӧ��ʱʱ��Ĵ�
	UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
	UARTCtrl->HostACK=8;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��,8�����ϱ�����
	#ifdef DEBUG_JSON
	myprintf("======================�����ϱ��ֽ���:%d==========\n",LEN_Tx,0,0);
	for(i=0;i<UARTCtrl->TxByte;i++)
		myprintf("%02x ",p8tx[i],0,0);
	myprintf("\n",0,0,0);
	#endif
	return 1;
}

u32 Report_JsonFile(u16 *pTime,u32 PORTn)//PORTn�˿ں�,pTime�˿ڳ�ʱʱ��
{
	//u16 *p16time;
	u32 i;
	u32 ret;
	ret=0;
	if(FileDownload->New==0)//Զ���������,�ϱ�ֹͣ
	{
		return ret;
	}
	i=Check_pYYMDHMS_hex(JSON->reportfiletime);
	if(i)
	{
		return ret;
	}
	i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
	if(i)
	{
		return ret;
	}
	#ifdef DEBUG_JSON
	myprintf("===============�����ϱ��ĳ���JSON->Reportjsonfile_Task = %d============= \n",JSON->Reportjsonfile_Task,0,0);
	#endif
	switch(JSON->Reportjsonfile_Task)
	{
		case 0://�����ϱ��ļ�
		//�ӽ���ĵ�һ�����ɵ��ļ���ʼ�ϱ�	
			i=Selec_ReportTime();
			if(0==i)
			{
				return ret;
			}
			i=AnalyzeReportFile();
			if(i)
			{
				#ifdef DEBUG_JSON
				myprintf("===============�����ļ��ɹ�,���濪ʼ�ϴ��ļ�============= \n",0,0,0);
				#endif
				JSON->Reportjsonfile_Task++;
				ret=1;
			}
			break;
		case 1://��������
			i=Report_FileInfo(pTime,PORTn);
			if(i)
			{
				JSON->Reportjsonfile_Task++;
				ret=1;
			}else
			{
				//�ϱ�ʧ�ܵ�ʱ��,�ϱ�ʱ����Ҫ��һ��������ļ�,��Ȼһֱ��ѭ��
				ClrReportInfo();
			}
			break;
		case 2://�ϴ��ļ�
			#ifdef DEBUG_JSON
			myprintf("===============��ʼ�ϴ��ļ�============= \n",0,0,0);
			#endif
			i=Report_FileData(pTime,PORTn);
			if(i)
			{
				Next_DataBlock();
				if(IsReportEnd())//������
				{
					#ifdef DEBUG_JSON
					myprintf("===============�ϴ��ļ��ɹ�============= \n",0,0,0);
					#endif
					ClrReportInfo();
				}
				ret=1;
			}else
			{
				ClrReportInfo();
			}
			break;
		default:
			JSON->Reportjsonfile_Task=0;
		break;
	}
	return ret;
}
/*
void tar_text(void)
{
	
	u32 i;
	u32 SLen;
	u32 size;
	u8 *p8;

	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	p8=(u8 *)ADDR_DATABUFF;
	//while(1)
	{
		SLen=0;
		mystrcpy((char *)p8+SLen, "tar zcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		mystrcpy((char *)p8+SLen, "111.tar.gz ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		mystrcpy((char *)p8+SLen, "/usr/app/frz_data/");
		size=ms->ms_malloc_sizemax();
		//p8=(u8 *)ms->ms_malloc(13*1024*1024);
		//if(!p8)
		{
			//return;
		}
		myprintf("size=%d\n",size,0,0);
		
		myprintf("��ʼѹ�� \n",0,0,0);
		myprintf("[%s:%d]:���� %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
		ms->link_system((char *)p8);
		myprintf("ѹ������ \n",0,0,0);
		//ms->link_sleep(1);
	}
}

void save(u8 *pbuf,u32 FileID)
{
	u32 i;
	u32 x;
	u32 y;
	//u32 FileID;
	//u32 EventID;//�¼�������ID
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	//u8* p8;
	//UARTCtrl_TypeDef *UARTCtrl;
	Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//FileID=Get_FileID(PORTn);//UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	Comm_Ram->msFILESearchCount=0;

		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{	
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
		}
		else
		{
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
		}

		//log_Flash();

	
	if(filename!=0xffffffff)
	{
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
		if(y>NUM_RMmax)
		{
			y=NUM_RMmax;
		}
		ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
		RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
		Addr0=pbuf[0]|(pbuf[1]<<8);
		Addr1=pbuf[2]|(pbuf[3]<<8);
		Addr2=pbuf[4]|(pbuf[5]<<8);
		for(i=0;i<y;i++)
		{
			if(RecordFileHead->Addr0==Addr0)
			{
				if(RecordFileHead->Addr1==Addr1)
				{
					if(RecordFileHead->Addr2==Addr2)
					{
						break;
					}
				}
			}
			RecordFileHead++;
		}
		if(i<y)
		{//��ַ�Ѵ���
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//�������ߵ�����ʱ���ܻ��ظ�,�ʲ���ɾ���ļ�
			}
		}
	}
	if(filename==0xffffffff)
	{//û�ҵ�

		{//FLASH�ļ�	
			while(1)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
				for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
				{
					if(file->BLOCK_ENTRY==0)
					{
						break;
					}
					file++;
				}
				if(filename>=recordFILENAMEmax_FLASH)
				{//û��
					//myprintf("[%s:%d]:��ʼִ��ɾ������\n",(u32)&__func__,(u32)__LINE__,0);
					i=AutoDeleteFile(0);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
					if(i)
					{//��ɾ��
						continue;
					}
					return;
				}
				break;
			}
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//���ļ�
		ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;

			file->LEN_DATA=0;
			file->NUM_DATA=0;
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
				MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
			}else
			{
				MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
				MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
			}
	}
//д�ļ�
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//��
		ms->msfile_Delete(filename);
		return;
	}
	//д����
	if(filename>=FILENAMEmax_FLASH)
	{//RAM���ļ�
		i=ms->ms_malloc_sizemax();//���ɷ���ĳߴ�(�ֽ���)
		if(i<(1024*1024))
		{//�ռ�<1M,��������1M�ռ����ڼ�¼���ݶ�ȡ
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y==0)
			{//ûɾ��
				return;
			}
		}
	}
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pbuf;
	//myprintf("[%s:%d]:RecordFileHead->Addr0=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr0);
	///myprintf("[%s:%d]:RecordFileHead->Addr1=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr1);
	//myprintf("[%s:%d]:RecordFileHead->Addr2=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr2);
	//myprintf("[%s:%d]:RecordFileHead->LEN_DATA=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->LEN_DATA);

	while(1)
	{
		y=RecordFileHead->LEN_DATA;
		y=ms->msfile_write(filename,i,pbuf+sizeof(RecordFileHead_TypeDef),y);
		if(y!=RecordFileHead->LEN_DATA)
		{//д����
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y)
			{//��ɾ��
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//д��ַ�б�
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//������+1
	x++;
	file->NUM_DATA=x;
	//myprintf("[%s:%d]:д��Flash�ɹ�\n",(u32)&__func__,(u32)__LINE__,0);

}

u32 MoreThan_StackLen(u32 LENMAX)
{
	return 0;
}
u32 GetOneMetAddr_TEST(u16 *MetCount)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u16 Met=*MetCount;
	p16=(u16*)(ADDR_AddrList+2);
	for(;Met<TEST_METNUM;Met++)
	{	
		//i=bcd_hex((Met)+1);
		i=Met;
		//myprintf("i=%d \n",i,0,0);
		p16[0]=(i&0xff)|(i>>8)&0xff;
		p16[1]=(i>>16)&0xff|(i>>24)&0xff;
		p16[2]=0;
		p16+=3;
		break;
	}
	if((Met)>=TEST_METNUM)
	{
		p16[0]=0;
		return 0;
	}
	*MetCount=Met;
	p16=(u16*)ADDR_AddrList;
	p16[0]=1;
	p8=(u8 *)(ADDR_AddrList+2);
	for(i=6;i>0;i--)
	{
		myprintf("%02x",p8[i-1],0,0);
	}
	myprintf("\n",0,0,0);
	return 1;
}
 */
u32 CalCulate_Space(const char *path)
{
	return 8*1024*1024;
}

void Delet_RAM_FrzFile(void)
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);
}

u32  Copy_RamToFlash(s32 fd)
{
	s32 fd1;
	u32 i;
	u32 filelen;
	u32 x;
	u32 y;
	u8 *p8;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	Mkdir_JsonFile(JSONFREPATH);
	pname=CreateFreJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����
	fd1=ms->link_open((char *)pname,O_RDWR);
	if(fd1==-1)//���ҵ�����ļ�
	{
		fd1=ms->link_open((char *)pname,O_CREAT|O_RDWR);
		if(fd1==-1)
		{
			return 1;
		}
	}
		ms->link_lseek(fd1,0,SEEK_END);
		p8=(u8 *)ms->ms_malloc(RAMTOFLASH_COPYONESIZE);
		if(!p8)
		{
			ms->link_close(fd1);
			return 1;
		}
		filelen=ms->link_lseek(fd,0,SEEK_END);//RAM�ռ��µ��ļ���С
		if(filelen<=(RAMTOFLASH_COPYONESIZE))
		{
			//����ֱ�ӿ�����ȥ
			ms->link_lseek(fd,0,SEEK_SET);//RAM�ռ��µ��ļ���С
			i=ms->link_read(fd,(u8 *)p8,filelen);
			if(i==filelen)//���ɹ�
			{
				i=ms->link_write(fd1,(u8 *)p8,filelen);
				if(i==filelen)
				{
					ms->link_close(fd1);
					return 0;
				}else//д����ʧ���п����Ǵ洢�ռ��Ѿ�����Flash,��Ҫɾ��Filename
				{
					
				}
			}
		}else
		{
			y=RAMTOFLASH_COPYONESIZE;
			x=0;
			while(x<filelen)
			{
				ms->link_lseek(fd,x,SEEK_SET);//RAM�ռ��µ��ļ���С
				if(filelen>=(y+x))//���ļ���С=���һ��д����+�Ѿ�д����
				{//�Ǿ������д����д��
					i=ms->link_read(fd,(u8 *)p8,y);
					if(y==i)
					{
						i=ms->link_write(fd1,(u8 *)p8,y);
					}else//д����ʧ���п����Ǵ洢�ռ��Ѿ�����Flash,��Ҫɾ��Filename
					{
						
					}
					x+=y;
				}else
				{
					if(filelen>x)
					{
						y=filelen-x;
						i=ms->link_read(fd,(u8 *)p8,y);
						if(y==i)
						{
							i=ms->link_write(fd1,(u8 *)p8,y);
							if(y==i)
							{
								Delet_RAM_FrzFile();
								ms->link_close(fd1);
								return 0;
							}else
							{
								Delet_RAM_FrzFile();
								ms->link_close(fd1);
								return 1;
							}
						}
					}else//˵���Ѿ�ȫ��д����
					{
						Delet_RAM_FrzFile();
						ms->link_close(fd1);
						return 0;
					}
				}
				
			}
		}
		ms->link_close(fd1);
		return 1;
	
}

u32 Find_TodayFrzFile()
{
	s32 fd;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pname=GetFrzFileJsonName_Compress();//����һ���µ���Ŀ�ļ�,�����ļ����
	fd=ms->link_open((char *)pname,O_CREAT|O_EXCL);
	if(fd!=-1)
	{
		ms->link_close(fd);
		return 0;
	}
	return 1;
}

/*
void TEST_PACK(void)
{
	u32 i;
	u32 x;
	u32 CJSONLEN;//JSON�ַ���������,��ʼ��0
	u32 JSONDataLen;
	u8 *p8;
	u8 *p8s;
	u8 *pAddr;//��ַu8����ָ��
	u8 *pname;
	u16 *p16;
	u16 AddrNUM;//��������Ч����
	s32 fd;
	//u8 addr[6]={0};
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	
	
	switch(FrzPackTask)
	{
		case 0://���֮ǰ��Ҫ����׼��
			CurMetCount=0;
			FrzPackTask++;
			//���֮ǰ���RAM�������еĿռ�
			Delet_RAM_FrzFile();
			break;
		case 1:	
			//i=GetOneMetAddr();
			i=GetOneMetAddr_TEST();
			if(i==0)//ȫ�����ַ�Ѿ��������
			{
				FrzPackTask=2;//����ÿ�����ն���ı�־
				myprintf("[%s:%d]:���ݴ�����Ͽ�ʼѹ��\n",(u32)&__func__,(u32)__LINE__,0);
				break;
			}
			CurMetCount++;//��һ��ѭ��������һֻ��
			//�õ��ļ�����
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//����һ���µ���Ŀ�ļ�,�����ļ����
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_END);
				if((i>=8*1024*1024))
				//if(1024)
				{
					//��RAM�ռ��ƶ���Flash�ռ�
					i=Copy_RamToFlash(fd);
					if(i!=0)
					{
						ms->link_close(fd);
						return ;
					}
				}
				ms->link_close(fd);
			}
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				myprintf("[%s:%d]:����ռ�ʧ��i\n",(u32)&__func__,(u32)__LINE__,0);
				return ;
			}
			CJSONLEN=0;
			JSONDataLen=0;
			p8s=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_AddrList);
			p16++;//ǰ��һ���Ǳ������,����һ���Ǳ�ĵ�ַ
			pAddr=(u8 *)p16;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			Add_StrToASCII(p8s,pAddr,6);//pbuf������ֽ������pstr��ת��asc��
			i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=AllDataPack(p16,p8+CJSONLEN);
			CJSONLEN+=i;
			if(i>0)//˵���������ݵ�
			{
				i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
				CJSONLEN+=i;
				CJSONLEN=Delet_COMMA(p8,CJSONLEN);//ɾ������
				i=Str_LF(p8+CJSONLEN);
				CJSONLEN+=i;			
			}else
			{		
				if(CJSONLEN>=JSONDataLen)
				{
					CJSONLEN-=JSONDataLen;
				}
			}
			
			if(CJSONLEN>0)//��һֻ��ʾ�����ݵ�
			{
				//���濪ʼ��RAM�ռ�����д����
				i=CalCulate_Space(JSONFREPATH_RAM);//��������ռ�����ʣ�������
				if(i>=CJSONLEN)//���ʣ��Ŀռ乻��
				{
					//�õ��ļ�����
					Mkdir_JsonFile(JSONFREPATH_RAM);
					pname=CreateFreJsonFile_RAM();//����һ���µ���Ŀ�ļ�,�����ļ����
					fd=ms->link_open((char *)pname,O_RDWR);
					if(fd!=-1)
					{
						ms->link_lseek(fd,0,SEEK_END);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}
					}else
					{
						fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
						if(-1==fd)
						{
							myprintf("[%s:%d]:�����ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
							ms->ms_free(p8);
							return;
						}
						ms->link_lseek(fd,0,SEEK_SET);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}
					}
				}else
				{
					
				}
				ms->link_close(fd);//�ر��ļ����
			}
			ms->ms_free(p8);
			break;
		case 2:
			//�õ��ļ�����
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//����һ���µ���Ŀ�ļ�,�����ļ����
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_SET);
				//��RAM�ռ��ƶ���Flash�ռ�
				i=Copy_RamToFlash(fd);
				if(i!=0)
				{
					ms->link_close(fd);
					return ;
				}
				ms->link_close(fd);
			}
			Mkdir_JsonFile(JSONFREPATH);
			pname=CreateFreJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				ms->link_close(fd);
				Compress_JSONFile(pname,1024,0);//��JSON�ļ������tar.gz��ʽ
				JSON->IsCreateFrzJson=1;
				JSON->PackFrzJsonTask=1;
			}else
			{
				FrzPackTask=0;	
				//˵�����ʧ����
				if(JSON->IsCreateFrzJson==2)
				{
					JSON->IsCreateFrzJson=1;
					JSON->PackFrzJsonTask=1;
				}
				else
				{
					JSON->IsCreateFrzJson++;
				}
			}	
			break;
		default:
			FrzPackTask=0;
		break;
	}

//===============================1.ȡ�����������еı��ַ=========================== 
	//GetMetAddr();//ȡ�����б�ĵ�ַ	
	p16=(u16*)(ADDR_AddrList);//ȡ�õ�����������unsigned short��ʾ
	p16[0]=2;
	p16++;	
	for(x=0;x<2;x++)
	{	
		i=hex_bcd(x+1);
		p16[0]=(i&0xff)|(i>>8)&0xff;
		p16[1]=(i>>16)&0xff|(i>>24)&0xff;
		p16[2]=0;

		p8=(u8 *)p16;
		for(i=0;i<3;i++)
			myprintf("%04x ",p16[i],0,0);
		myprintf("\n",0,0,0);
		p16+=3;
	}
	AddrNUM=2;
	//p16++;		
//===============================2.�������ռ�=====================================
	p8=(u8 *)ms->ms_calloc(1024);
	if(!p8)
	{
		myprintf("[%s:%d]:malloc�ռ䲻��\n",(u32)&__func__,(u32)__LINE__,0);
		return ;
	}
//==============================3.��ʼ��������========================================
	CJSONLEN=0;//JSON�ַ���������,��ʼ��0
	p8s=(u8 *)ADDR_DATABUFF;

	p16=(u16*)(ADDR_AddrList);//ȡ�õ�����������unsigned short��ʾ
	p16++;
	for(x=0;x<AddrNUM;x++)
	{
		if(0!=x)
		{
			p16+=3;
		}
		pAddr=(u8 *)p16;
		JSONDataLen=0;
		i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		Add_StrToASCII(p8s,pAddr,6);//pbuf������ֽ������pstr��ת��asc��
		i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		i=AllDataPack(p16,p8+CJSONLEN);
		CJSONLEN+=i;
		
		if(i>0)//˵���������ݵ�
		{
			i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			CJSONLEN=Delet_COMMA(p8,CJSONLEN);//ɾ������
			i=Str_LF(p8+CJSONLEN);
			CJSONLEN+=i;			
		}else
		{		
			if(CJSONLEN>=JSONDataLen)
			{
				CJSONLEN-=JSONDataLen;
			}
		}	
	}
	if(CJSONLEN)
	{
		Delet_FrzJsonFile();//ɾ����ԭ����JSON�ļ�
		pname=CreateFreJsonFile();//����һ���µ���Ŀ�ļ�,�����ļ����
		fd=ms->link_open((char *)pname,O_RDWR);
		if(-1!=fd)//�Ѿ����ڸ��ļ���ɾ��
		{
			//ms->link_remove((char *)pname);
			//myprintf("\n\n[%s:%d]:ɾ���ļ�:%s \n\n", (u32)&__func__, __LINE__, (u32 )pname);
			i=ms->link_write(fd,p8,CJSONLEN);
			if(i==CJSONLEN)
			{
				myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
				Compress_JSONFile(pname,1024,0);//��JSON�ļ������tar.gz��ʽ
			}else
			{
				myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
			}
		}else
		{
			fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
			if(-1==fd)
			{
				myprintf("[%s:%d]:�����ļ�ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
				return;
			}
			i=ms->link_write(fd,p8,CJSONLEN);
			if(i==CJSONLEN)
			{
				myprintf("[%s:%d]:д���ļ��ɹ�  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
				Compress_JSONFile(pname,1024,0);//��JSON�ļ������tar.gz��ʽ
			}else
			{
				myprintf("[%s:%d]:д���ļ�ʧ��  �ַ�����i=%d\n",(u32)&__func__,(u32)__LINE__,i);
			}
		}
		ms->link_close(fd);
	}
	ms->ms_free(p8);
	*/
	/*
u8 day[]={0x01,0x01,0x5b,0x01,0x50,0x04,0x02,0x00,0x03,0x20,0x21,0x02,0x00,0x00,0x10,0x02,0x00,0x00,0x20,0x02,0x00,0x1C,0x07,0xE4,0x07,0x0A,0x00,0x00,0x00,0x01,0x05,0x06,0x00,0x1C,0x34,0x6B,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x52,0x9D,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x0C,0xE1,0xCD,0x01,0x05,0x06,0x00,0x00,0x00,0x08,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x08,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};
//u8 min15[]={0x50,0x02,0x02,0x00,0x03,0x20,0x21,0x02,0x00,0x00,0x10,0x02,0x01,0x00,0x20,0x02,0x01,0x01,0x1C,0x06,0x06,0x0A,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1C,0x34,0x6B,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x20,0x2D,0x84,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x21,0xC8,0x48,0x00,0x00,0x00,0x0C,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x20,0x5D,0xF2,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,
	//0x09,0x16,0x00,0x00,0x00,0x1F,0x78,0xC6,0x00,0x00,0x00,0x34,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x26,0xC3,0xD3,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1E,0xFB,0xD1,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1F,0x91,0xAF,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1D,0x34,0xD5,0x00,0x00,0x00,0x08,
	//0x00,0x00
//};

u8 min[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x0B,0x20,0x00,0x02,0x01,0x20,0x00,0x02,0x02,0x20,0x00,0x02,0x03,0x20,0x01,0x02,0x01,0x20,0x01,0x02,0x02,0x20,0x01,0x02,0x03,0x20,0x01,0x04,0x00,0x20,0x04,0x02,0x01,0x20,0x04,0x02,0x02,0x20,0x04,0x02,0x03,0x20,0x04,0x02,0x04,0x01,0x12,0x12,0x12,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x06,0x08,0xF8,0x08,0xED,0x08,0xF1,0x00,0x00,0x08,0xEB,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC7,0x00,0x00,0x03,0x58,0x00,0x00,
	0x4C,0x5C,0x00,0x00,0x14,0x79,0x00,0x00,0x1A,0xA7,0x00,0x00,0x1D,0x3A,0x08,0xFB,0x08,0xF0,0x08,0xF5,0x00,0x00,0x08,0xF4,0x00,0x00,0x0B,0xAF,0x00,0x00,0x0C,0xCA,0x00,0x00,0x03,0x52,0x00,0x00,0x4C,0x9B,0x00,0x00,0x14,0x96,0x00,0x00,0x1A,0xB5,0x00,0x00,0x1D,0x4E,0x08,0xFA,0x08,0xF0,0x08,0xF4,0x00,0x00,0x08,0xE7,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x50,0x00,0x00,0x14,0x70,0x00,0x00,0x1A,0xA8,0x00,
	0x00,0x1D,0x38,0x08,0xFC,0x08,0xF1,0x08,0xF6,0x00,0x00,0x08,0xF0,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC8,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0xA0,0x00,0x00,0x14,0x94,0x00,0x00,0x1A,0xBC,0x00,0x00,0x1D,0x4C,0x08,0xF9,0x08,0xF0,0x08,0xF3,0x00,0x00,0x08,0xE7,0x00,0x00,0x0B,0xAB,0x00,0x00,0x0C,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x70,0x00,0x00,0x14,0x84,0x00,0x00,0x1A,0xA8,0x00,0x00,0x1D,0x4C,0x08,0xF9,0x08,0xEF,0x08,0xF3,0x00,0x00,
	0x08,0xED,0x00,0x00,0x0B,0xAA,0x00,0x00,0x0C,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x90,0x00,0x00,0x14,0x84,0x00,0x00,0x1A,0xBC,0x00,0x00,0x1D,0x4C,0x00,0x00
};

//u8 min[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x05,0x20,0x00,0x02,0x01,0x20,0x01,0x02,0x01,0x20,0x01,0x04,0x00,0x20,0x04,0x02,0x01,0x20,0x04,0x02,0x02,0x01,0x12,0x07,0x05,0x05,0x05,0x0A,0x00,0x00,0x00};

u8 min15[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x06,0x00,0x10,0x02,0x01,0x00,0x20,0x02,0x01,0x00,0x50,0x02,0x01,0x00,0x60,0x02,0x01,0x00,0x70,0x02,0x01,0x00,0x80,0x02,0x01,0x01,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x00,0x07,0x72,0x7B,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0xA8,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x72,0xD5,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x73,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x32,0xC9,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x32,0xF6,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAD,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x33,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x33,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x07,0x71,0xF7,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xA6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x24,0x00,0x00,0x00,0x00,0x00,
	0x00,0x51,0xA7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xA9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

VOID test_saveMIN(void)
{
	u32 i;
	u16 datalen;
	u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1A,0x00,0x00,0x00};
		p8=(u8 *)ADDR_DATABUFF;
	myprintf("���Ӵ洢 \n",0,0,0);

	for(FILEID=0;FILEID<TEST_TIME;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(min);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)min,sizeof(min));
			Offset+=sizeof(min);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f22000);
			//break;
		}
		pYYMDHMS_AddS_hex(time, 60);
		//break;
	}
	myprintf("ȫ����д��� \n",0,0,0);
}

VOID test_saveMIN15(void)
{
	u32 i;
	u16 datalen;
	u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1A,0x00,0x00,0x00};
	p8=(u8 *)ADDR_DATABUFF;
	myprintf("���Ӵ洢 \n",0,0,0);

	for(FILEID=0;FILEID<96;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(min15);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)min15,sizeof(min15));
			Offset+=sizeof(min15);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f19000);
			//break;
		}
		pYYMDHMS_AddS_hex(time, 900);
		//break;
	}
	myprintf("ȫ����д��� \n",0,0,0);
}

VOID test_saveDAY(void)
{
	u32 i;
	u16 datalen;
	//u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1a,0x17,0x3b,0x00};
	p8=(u8 *)ADDR_DATABUFF;
	myprintf("������洢 \n",0,0,0);
	//for(FILEID=0;FILEID<TEST_TIME;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(day);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)day,sizeof(day));
			Offset+=sizeof(day);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f16000);
			//break;
		}
		//pYYMDHMS_AddS_hex(time, 60);
		//break;
	}
	myprintf("ȫ����д��� \n",0,0,0);
}
*/

