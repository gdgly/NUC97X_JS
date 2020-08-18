
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../Device/MEMRW.h"
#include "../MS/MS.h"
#include "../Display/Display.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"





//----������----
#if ((Project/100)==2)//������
#if USER<200//ä�����Ժ�ͼ�ϵ,��׼ϵ
#include "../DL698/DL698_SETInit_USER0xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER0xx.c"
#endif

#if (USER/100)==9//����ϵ
#include "../DL698/DL698_SETInit_USER9xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER9xx.c"
#endif

#if (USER/100)==14//����
#include "../DL698/DL698_SETInit_USER14xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER14xx.c"
#endif

#if (USER/100)==15//����ϵ
#include "../DL698/DL698_SETInit_USER15xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER15xx.c"
#endif

#if (USER/100)==17//����ϵ
#include "../DL698/DL698_SETInit_USER17xx.c"
#include "../DL698/DL698_SETInit_6000_Project2xx_USER17xx.c"
#endif

#endif//#if ((Project/100)==2)//������
//----������ end----


//-----ר��----
#if ((Project/100)==3)//ר��
#if USER<200//ä�����Ժ�ͼ�ϵ,��׼ϵ
#include "../DL698/DL698_SETInit_USER0xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER0xx.c"
#endif

#if (USER/100)==9//����ϵ
#include "../DL698/DL698_SETInit_USER9xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER9xx.c"
#endif
#if (USER/100)==12//�Ĵ�ϵ
#include "../DL698/DL698_SETInit_USER12xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER12xx.c"
#endif
#if (USER/100)==14//����
#include "../DL698/DL698_SETInit_USER14xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER14xx.c"
#endif

#if (USER/100)==15//����ϵ
#include "../DL698/DL698_SETInit_USER15xx.c"
#include "../DL698/DL698_SETInit_6000_Project3xx_USER15xx.c"
#endif


#endif//#if ((Project/100)==3)//ר��
//-----ר�� end----


//----����һ�ն�----
#if ((Project/100)==4)//����һ�ն�
#if (USER/100)==16//����ϵ
#include "../DL698/DL698_SETInit_USER16xx.c"
#include "../DL698/DL698_SETInit_6000_Project4xx_USER16xx.c"
#endif


#endif//#if ((Project/100)==4)//����һ�ն�
//----����һ�ն� end----




#include "../DL698/DL698_SETLIB_2000.c"
#include "../DL698/DL698_SETLIB_4000.c"
#include "../DL698/DL698_SETLIB_5000.c"
#include "../DL698/DL698_SETLIB_6000.c"
#include "../DL698/DL698_SETLIB_8000.c"
#include "../DL698/DL698_SETLIB_F000.c"


__align(4) const DL698_LIB_List_TypeDef  DL698_SETLIB_List[]=
{
//---ͨ����---
	{(u32*)DL698_SETLIB_4001,},
	{(u32*)DL698_SETLIB_4005,},
	{(u32*)DL698_SETLIB_4002,},
	{(u32*)DL698_SETLIB_4003,},
	{(u32*)DL698_SETLIB_4400_4,},
	{(u32*)DL698_SETLIB_4401,},
	{(u32*)DL698_SETLIB_4500_2,},
	{(u32*)DL698_SETLIB_4500_3,},
	{(u32*)DL698_SETLIB_4500_4,},
	{(u32*)DL698_SETLIB_4500_5,},
	{(u32*)DL698_SETLIB_4500_6,},
	{(u32*)DL698_SETLIB_4500_7,},
	{(u32*)DL698_SETLIB_4500_8,},
	{(u32*)DL698_SETLIB_4500_9,},
	{(u32*)DL698_SETLIB_4500_10,},
	{(u32*)DL698_SETLIB_4500_11,},
	{(u32*)DL698_SETLIB_4510_2,},
	{(u32*)DL698_SETLIB_4510_3,},
	{(u32*)DL698_SETLIB_4510_4,},
	{(u32*)DL698_SETLIB_4510_5,},
	{(u32*)DL698_SETLIB_4520_2,},//�������չ���ʶ���Ϊ��ͨ�Ų���,����ǹ���ʱ������������0������ʼ��!


//---ͨ����---
	{(u32*)DL698_SETLIB_4000,},
	{(u32*)DL698_SETLIB_4000_3,},
	{(u32*)DL698_SETLIB_4000_4,},
	{(u32*)DL698_SETLIB_4004,},
	{(u32*)DL698_SETLIB_4006,},
	{(u32*)DL698_SETLIB_4007,},
	{(u32*)DL698_SETLIB_4008,},
	{(u32*)DL698_SETLIB_4009,},
	{(u32*)DL698_SETLIB_400A,},
	{(u32*)DL698_SETLIB_400B,},
	{(u32*)DL698_SETLIB_400C,},
	{(u32*)DL698_SETLIB_400D,},
	{(u32*)DL698_SETLIB_400E,},
	{(u32*)DL698_SETLIB_400F,},
	{(u32*)DL698_SETLIB_4010,},
	{(u32*)DL698_SETLIB_4011,},
	{(u32*)DL698_SETLIB_4012,},
	{(u32*)DL698_SETLIB_4013,},
	{(u32*)DL698_SETLIB_4014,},
	{(u32*)DL698_SETLIB_4015,},
	{(u32*)DL698_SETLIB_4016,},
	{(u32*)DL698_SETLIB_4017,},
	{(u32*)DL698_SETLIB_4018,},
	{(u32*)DL698_SETLIB_4019,},
	{(u32*)DL698_SETLIB_401A,},
	{(u32*)DL698_SETLIB_401B,},
	{(u32*)DL698_SETLIB_401C,},
	{(u32*)DL698_SETLIB_401D,},
	{(u32*)DL698_SETLIB_401E,},
	{(u32*)DL698_SETLIB_401F,},
	{(u32*)DL698_SETLIB_4020,},
	{(u32*)DL698_SETLIB_4021,},
	{(u32*)DL698_SETLIB_4022,},
	{(u32*)DL698_SETLIB_4023,},
	{(u32*)DL698_SETLIB_4024,},
	{(u32*)DL698_SETLIB_4030,},
	{(u32*)DL698_SETLIB_4100,},
	{(u32*)DL698_SETLIB_4101,},
	{(u32*)DL698_SETLIB_4102,},
	{(u32*)DL698_SETLIB_4103,},
	{(u32*)DL698_SETLIB_4104,},
	{(u32*)DL698_SETLIB_4105,},
	{(u32*)DL698_SETLIB_4106,},
	{(u32*)DL698_SETLIB_4107,},
	{(u32*)DL698_SETLIB_4108,},
	{(u32*)DL698_SETLIB_4109,},
	{(u32*)DL698_SETLIB_410A,},
	{(u32*)DL698_SETLIB_410B,},
	{(u32*)DL698_SETLIB_410C,},
	{(u32*)DL698_SETLIB_410D,},
	{(u32*)DL698_SETLIB_410E,},
	{(u32*)DL698_SETLIB_410F,},
	{(u32*)DL698_SETLIB_4111,},
	{(u32*)DL698_SETLIB_4112,},
	{(u32*)DL698_SETLIB_4113,},
	{(u32*)DL698_SETLIB_4114,},
	{(u32*)DL698_SETLIB_4115,},
	{(u32*)DL698_SETLIB_4116,},
	{(u32*)DL698_SETLIB_4117,},

	
//---�ն���---
//4200	11	·�ɱ�
//4201	8	·����Ϣ��Ԫ
	{(u32*)DL698_SETLIB_4202,},
	{(u32*)DL698_SETLIB_4204_2,},
	{(u32*)DL698_SETLIB_4204_3,},
	
//�汾��Ϣ
	{(u32*)DL698_SETLIB_4300_2,},
	{(u32*)DL698_SETLIB_4300_3,},
	{(u32*)DL698_SETLIB_4300_4,},
	{(u32*)DL698_SETLIB_4300_5,},
	{(u32*)DL698_SETLIB_4300_6,},
	{(u32*)DL698_SETLIB_4300_7,},
	{(u32*)DL698_SETLIB_4300_8,},
	{(u32*)DL698_SETLIB_4300_9,},
	{(u32*)DL698_SETLIB_4300_10,},
	
//������
	{(u32*)DL698_SETLIB_5000,},
	{(u32*)DL698_SETLIB_5001,},
	{(u32*)DL698_SETLIB_5002,},
	{(u32*)DL698_SETLIB_5003,},
	{(u32*)DL698_SETLIB_5004,},
	{(u32*)DL698_SETLIB_5005,},
	{(u32*)DL698_SETLIB_5006,},
	{(u32*)DL698_SETLIB_5007,},
	{(u32*)DL698_SETLIB_5008,},
	{(u32*)DL698_SETLIB_5009,},
	{(u32*)DL698_SETLIB_500A,},
	{(u32*)DL698_SETLIB_500B,},
	{(u32*)DL698_SETLIB_5011,},
	
//�ɼ����
	{(u32*)DL698_SETLIB_6000_2_0,},//��λ��0���ɼ�������ַ������ɾ��
	{(u32*)DL698_SETLIB_6000_2_1,},
	{(u32*)DL698_SETLIB_6000_2_2,},
	{(u32*)DL698_SETLIB_6000_2_3,},
	{(u32*)DL698_SETLIB_6000_2_4,},
	{(u32*)DL698_SETLIB_6000_2_5,},
	{(u32*)DL698_SETLIB_6000_2_6,},
	{(u32*)DL698_SETLIB_6000_2_7,},
	{(u32*)DL698_SETLIB_6000_2_8,},
	{(u32*)DL698_SETLIB_6000_2_9,},

	{(u32*)DL698_SETLIB_6000_3,},
	{(u32*)DL698_SETLIB_6000_4,},
	
	{(u32*)DL698_SETLIB_6002_2,},//��ʹ����OAD��Ч
	{(u32*)DL698_SETLIB_6002_3,},//��ʹ����OAD��Ч
	{(u32*)DL698_SETLIB_6002_4,},//��ʹ����OAD��Ч
	{(u32*)DL698_SETLIB_6002_5,},//��ʹ����OAD��Ч
	{(u32*)DL698_SETLIB_6002_6,},//�����ѱ�����¼��(��ʹ����OAD��Ч)
	{(u32*)DL698_SETLIB_6002_7,},//��̨���ѱ�����¼��(��ʹ����OAD��Ч)
	{(u32*)DL698_SETLIB_6002_8,},
	{(u32*)DL698_SETLIB_6002_9,},
	{(u32*)DL698_SETLIB_6002_10,},


	{(u32*)DL698_SETLIB_6012_2_0,},//��λ��0����ַ������ɾ��
#ifdef InitData_6012_2_1_
	{(u32*)DL698_SETLIB_6012_2_1,},
#endif
#ifdef InitData_6012_2_2_
	{(u32*)DL698_SETLIB_6012_2_2,},
#endif
#ifdef InitData_6012_2_3_
	{(u32*)DL698_SETLIB_6012_2_3,},
#endif
#ifdef InitData_6012_2_4_
	{(u32*)DL698_SETLIB_6012_2_4,},
#endif
#ifdef InitData_6012_2_5_
	{(u32*)DL698_SETLIB_6012_2_5,},
#endif
#ifdef InitData_6012_2_6_
	{(u32*)DL698_SETLIB_6012_2_6,},
#endif
#ifdef InitData_6012_2_7_
	{(u32*)DL698_SETLIB_6012_2_7,},
#endif
#ifdef InitData_6012_2_8_
	{(u32*)DL698_SETLIB_6012_2_8,},
#endif
#ifdef InitData_6012_2_9_
	{(u32*)DL698_SETLIB_6012_2_9,},
#endif
#ifdef InitData_6012_2_10_
	{(u32*)DL698_SETLIB_6012_2_10,},
#endif
#ifdef InitData_6012_2_11_
	{(u32*)DL698_SETLIB_6012_2_11,},
#endif
#ifdef InitData_6012_2_12_
	{(u32*)DL698_SETLIB_6012_2_12,},
#endif
#ifdef InitData_6012_2_13_
	{(u32*)DL698_SETLIB_6012_2_13,},
#endif
#ifdef InitData_6012_2_14_
	{(u32*)DL698_SETLIB_6012_2_14,},
#endif
#ifdef InitData_6012_2_15_
	{(u32*)DL698_SETLIB_6012_2_15,},
#endif
#ifdef InitData_6012_2_16_
	{(u32*)DL698_SETLIB_6012_2_16,},
#endif
#ifdef InitData_6012_2_17_
	{(u32*)DL698_SETLIB_6012_2_17,},
#endif
#ifdef InitData_6012_2_18_
	{(u32*)DL698_SETLIB_6012_2_18,},
#endif
#ifdef InitData_6012_2_19_
	{(u32*)DL698_SETLIB_6012_2_19,},
#endif
#ifdef InitData_6012_2_20_
	{(u32*)DL698_SETLIB_6012_2_20,},
#endif
#ifdef InitData_6012_2_21_
	{(u32*)DL698_SETLIB_6012_2_21,},
#endif
#ifdef InitData_6012_2_22_
	{(u32*)DL698_SETLIB_6012_2_22,},
#endif
#ifdef InitData_6012_2_23_
	{(u32*)DL698_SETLIB_6012_2_23,},
#endif
#ifdef InitData_6012_2_24_
	{(u32*)DL698_SETLIB_6012_2_24,},
#endif
#ifdef InitData_6012_2_25_
	{(u32*)DL698_SETLIB_6012_2_25,},
#endif
#ifdef InitData_6012_2_26_
	{(u32*)DL698_SETLIB_6012_2_26,},
#endif
#ifdef InitData_6012_2_27_
	{(u32*)DL698_SETLIB_6012_2_27,},
#endif
#ifdef InitData_6012_2_28_
	{(u32*)DL698_SETLIB_6012_2_28,},
#endif
#ifdef InitData_6012_2_29_
	{(u32*)DL698_SETLIB_6012_2_29,},
#endif
#ifdef InitData_6012_2_30_
	{(u32*)DL698_SETLIB_6012_2_30,},
#endif
#ifdef InitData_6012_2_31_
	{(u32*)DL698_SETLIB_6012_2_31,},
#endif
#ifdef InitData_6012_2_32_
	{(u32*)DL698_SETLIB_6012_2_32,},
#endif
#ifdef InitData_6012_2_33_
	{(u32*)DL698_SETLIB_6012_2_33,},
#endif
#ifdef InitData_6012_2_34_
	{(u32*)DL698_SETLIB_6012_2_34,},
#endif
#ifdef InitData_6012_2_35_
	{(u32*)DL698_SETLIB_6012_2_35,},
#endif
#ifdef InitData_6012_2_36_
	{(u32*)DL698_SETLIB_6012_2_36,},
#endif
#ifdef InitData_6012_2_37_
	{(u32*)DL698_SETLIB_6012_2_37,},
#endif
#ifdef InitData_6012_2_38_
	{(u32*)DL698_SETLIB_6012_2_38,},
#endif
#ifdef InitData_6012_2_39_
	{(u32*)DL698_SETLIB_6012_2_39,},
#endif
#ifdef InitData_6012_2_40_
	{(u32*)DL698_SETLIB_6012_2_40,},
#endif
#ifdef InitData_6012_2_41_
	{(u32*)DL698_SETLIB_6012_2_41,},
#endif
#ifdef InitData_6012_2_42_
	{(u32*)DL698_SETLIB_6012_2_42,},
#endif
#ifdef InitData_6012_2_43_
	{(u32*)DL698_SETLIB_6012_2_43,},
#endif
#ifdef InitData_6012_2_44_
	{(u32*)DL698_SETLIB_6012_2_44,},
#endif
#ifdef InitData_6012_2_45_
	{(u32*)DL698_SETLIB_6012_2_45,},
#endif
#ifdef InitData_6012_2_46_
	{(u32*)DL698_SETLIB_6012_2_46,},
#endif
#ifdef InitData_6012_2_47_
	{(u32*)DL698_SETLIB_6012_2_47,},
#endif
#ifdef InitData_6012_2_48_
	{(u32*)DL698_SETLIB_6012_2_48,},
#endif
#ifdef InitData_6012_2_49_
	{(u32*)DL698_SETLIB_6012_2_49,},
#endif
#ifdef InitData_6012_2_50_
	{(u32*)DL698_SETLIB_6012_2_50,},
#endif
#ifdef InitData_6012_2_51_
	{(u32*)DL698_SETLIB_6012_2_51,},
#endif
#ifdef InitData_6012_2_52_
	{(u32*)DL698_SETLIB_6012_2_52,},
#endif
#ifdef InitData_6012_2_53_
	{(u32*)DL698_SETLIB_6012_2_53,},
#endif
#ifdef InitData_6012_2_54_
	{(u32*)DL698_SETLIB_6012_2_54,},
#endif
#ifdef InitData_6012_2_55_
	{(u32*)DL698_SETLIB_6012_2_55,},
#endif
#ifdef InitData_6012_2_56_
	{(u32*)DL698_SETLIB_6012_2_56,},
#endif
#ifdef InitData_6012_2_57_
	{(u32*)DL698_SETLIB_6012_2_57,},
#endif
#ifdef InitData_6012_2_58_
	{(u32*)DL698_SETLIB_6012_2_58,},
#endif
#ifdef InitData_6012_2_59_
	{(u32*)DL698_SETLIB_6012_2_59,},
#endif
#ifdef InitData_6012_2_60_
	{(u32*)DL698_SETLIB_6012_2_60,},
#endif
#ifdef InitData_6012_2_61_
	{(u32*)DL698_SETLIB_6012_2_61,},
#endif	
#ifdef InitData_6012_2_62_
	{(u32*)DL698_SETLIB_6012_2_62,},
#endif
#ifdef InitData_6012_2_63_
	{(u32*)DL698_SETLIB_6012_2_63,},
#endif
#ifdef InitData_6012_2_64_
	{(u32*)DL698_SETLIB_6012_2_64,},
#endif
#ifdef InitData_6012_2_65_
	{(u32*)DL698_SETLIB_6012_2_65,},
#endif
#ifdef InitData_6012_2_66_
	{(u32*)DL698_SETLIB_6012_2_66,},
#endif
#ifdef InitData_6012_2_67_
	{(u32*)DL698_SETLIB_6012_2_67,},
#endif
#ifdef InitData_6012_2_68_
	{(u32*)DL698_SETLIB_6012_2_68,},
#endif
#ifdef InitData_6012_2_69_
	{(u32*)DL698_SETLIB_6012_2_69,},
#endif
#ifdef InitData_6012_2_70_
	{(u32*)DL698_SETLIB_6012_2_70,},
#endif
#ifdef InitData_6012_2_71_
	{(u32*)DL698_SETLIB_6012_2_71,},
#endif
#ifdef InitData_6012_2_72_
	{(u32*)DL698_SETLIB_6012_2_72,},
#endif
#ifdef InitData_6012_2_73_
	{(u32*)DL698_SETLIB_6012_2_73,},
#endif
#ifdef InitData_6012_2_74_
	{(u32*)DL698_SETLIB_6012_2_74,},
#endif
#ifdef InitData_6012_2_75_
	{(u32*)DL698_SETLIB_6012_2_75,},
#endif
#ifdef InitData_6012_2_76_
	{(u32*)DL698_SETLIB_6012_2_76,},
#endif
#ifdef InitData_6012_2_77_
	{(u32*)DL698_SETLIB_6012_2_77,},
#endif
#ifdef InitData_6012_2_78_
	{(u32*)DL698_SETLIB_6012_2_78,},
#endif
#ifdef InitData_6012_2_79_
	{(u32*)DL698_SETLIB_6012_2_79,},
#endif
#ifdef InitData_6012_2_80_
	{(u32*)DL698_SETLIB_6012_2_80,},
#endif
#ifdef InitData_6012_2_81_
	{(u32*)DL698_SETLIB_6012_2_81,},
#endif
#ifdef InitData_6012_2_82_
	{(u32*)DL698_SETLIB_6012_2_82,},
#endif
#ifdef InitData_6012_2_83_
	{(u32*)DL698_SETLIB_6012_2_83,},
#endif
#ifdef InitData_6012_2_84_
	{(u32*)DL698_SETLIB_6012_2_84,},
#endif	
#ifdef InitData_6012_2_85_
	{(u32*)DL698_SETLIB_6012_2_85,},
#endif
#ifdef InitData_6012_2_86_
	{(u32*)DL698_SETLIB_6012_2_86,},
#endif
#ifdef InitData_6012_2_87_
	{(u32*)DL698_SETLIB_6012_2_87,},
#endif
#ifdef InitData_6012_2_88_
	{(u32*)DL698_SETLIB_6012_2_88,},
#endif
#ifdef InitData_6012_2_89_
	{(u32*)DL698_SETLIB_6012_2_89,},
#endif
#ifdef InitData_6012_2_90_
	{(u32*)DL698_SETLIB_6012_2_90,},
#endif
#ifdef InitData_6012_2_91_
	{(u32*)DL698_SETLIB_6012_2_91,},
#endif
#ifdef InitData_6012_2_92_
	{(u32*)DL698_SETLIB_6012_2_92,},
#endif
#ifdef InitData_6012_2_93_
	{(u32*)DL698_SETLIB_6012_2_93,},
#endif
#ifdef InitData_6012_2_94_
	{(u32*)DL698_SETLIB_6012_2_94,},
#endif
#ifdef InitData_6012_2_95_
	{(u32*)DL698_SETLIB_6012_2_95,},
#endif
#ifdef InitData_6012_2_96_
	{(u32*)DL698_SETLIB_6012_2_96,},
#endif
#ifdef InitData_6012_2_97_
	{(u32*)DL698_SETLIB_6012_2_97,},
#endif
#ifdef InitData_6012_2_98_
	{(u32*)DL698_SETLIB_6012_2_98,},
#endif
#ifdef InitData_6012_2_99_
	{(u32*)DL698_SETLIB_6012_2_99,},
#endif
#ifdef InitData_6012_2_100_
	{(u32*)DL698_SETLIB_6012_2_100,},
#endif
#ifdef InitData_6012_2_101_
	{(u32*)DL698_SETLIB_6012_2_101,},
#endif
#ifdef InitData_6012_2_102_
	{(u32*)DL698_SETLIB_6012_2_102,},
#endif
#ifdef InitData_6012_2_103_
	{(u32*)DL698_SETLIB_6012_2_103,},
#endif
#ifdef InitData_6012_2_104_
	{(u32*)DL698_SETLIB_6012_2_104,},
#endif
#ifdef InitData_6012_2_105_
	{(u32*)DL698_SETLIB_6012_2_105,},
#endif
#ifdef InitData_6012_2_106_
	{(u32*)DL698_SETLIB_6012_2_106,},
#endif
#ifdef InitData_6012_2_107_
	{(u32*)DL698_SETLIB_6012_2_107,},
#endif
#ifdef InitData_6012_2_108_
	{(u32*)DL698_SETLIB_6012_2_108,},
#endif
#ifdef InitData_6012_2_109_
	{(u32*)DL698_SETLIB_6012_2_109,},
#endif
#ifdef InitData_6012_2_110_
	{(u32*)DL698_SETLIB_6012_2_110,},
#endif
#ifdef InitData_6012_2_111_
	{(u32*)DL698_SETLIB_6012_2_111,},
#endif
#ifdef InitData_6012_2_112_
	{(u32*)DL698_SETLIB_6012_2_112,},
#endif
#ifdef InitData_6012_2_113_
	{(u32*)DL698_SETLIB_6012_2_113,},
#endif
#ifdef InitData_6012_2_114_
	{(u32*)DL698_SETLIB_6012_2_114,},
#endif
#ifdef InitData_6012_2_115_
	{(u32*)DL698_SETLIB_6012_2_115,},
#endif
#ifdef InitData_6012_2_116_
	{(u32*)DL698_SETLIB_6012_2_116,},
#endif
#ifdef InitData_6012_2_117_
	{(u32*)DL698_SETLIB_6012_2_117,},
#endif
#ifdef InitData_6012_2_118_
	{(u32*)DL698_SETLIB_6012_2_118,},
#endif
#ifdef InitData_6012_2_119_
	{(u32*)DL698_SETLIB_6012_2_119,},
#endif
#ifdef InitData_6012_2_120_
	{(u32*)DL698_SETLIB_6012_2_120,},
#endif
#ifdef InitData_6012_2_121_
	{(u32*)DL698_SETLIB_6012_2_121,},
#endif
#ifdef InitData_6012_2_122_
	{(u32*)DL698_SETLIB_6012_2_122,},
#endif
#ifdef InitData_6012_2_123_
	{(u32*)DL698_SETLIB_6012_2_123,},
#endif
#ifdef InitData_6012_2_124_
	{(u32*)DL698_SETLIB_6012_2_124,},
#endif
#ifdef InitData_6012_2_125_
	{(u32*)DL698_SETLIB_6012_2_125,},
#endif
#ifdef InitData_6012_2_126_
	{(u32*)DL698_SETLIB_6012_2_126,},
#endif
#ifdef InitData_6012_2_127_
	{(u32*)DL698_SETLIB_6012_2_127,},
#endif
#ifdef InitData_6012_2_128_
	{(u32*)DL698_SETLIB_6012_2_128,},
#endif
		
#if NUMmax_6012_2>(128+1)//��󳭱�������
	#error
#endif
	
	{(u32*)DL698_SETLIB_6014_2_0,},//��λ��0����ַ������ɾ��
#ifdef InitData_6014_2_1_
	{(u32*)DL698_SETLIB_6014_2_1,},
#endif
#ifdef InitData_6014_2_2_
	{(u32*)DL698_SETLIB_6014_2_2,},
#endif
#ifdef InitData_6014_2_3_
	{(u32*)DL698_SETLIB_6014_2_3,},
#endif
#ifdef InitData_6014_2_4_
	{(u32*)DL698_SETLIB_6014_2_4,},
#endif
#ifdef InitData_6014_2_5_
	{(u32*)DL698_SETLIB_6014_2_5,},
#endif
#ifdef InitData_6014_2_6_
	{(u32*)DL698_SETLIB_6014_2_6,},
#endif
#ifdef InitData_6014_2_7_
	{(u32*)DL698_SETLIB_6014_2_7,},
#endif
#ifdef InitData_6014_2_8_
	{(u32*)DL698_SETLIB_6014_2_8,},
#endif
#ifdef InitData_6014_2_9_
	{(u32*)DL698_SETLIB_6014_2_9,},
#endif
#ifdef InitData_6014_2_10_
	{(u32*)DL698_SETLIB_6014_2_10,},
#endif
#ifdef InitData_6014_2_11_
	{(u32*)DL698_SETLIB_6014_2_11,},
#endif
#ifdef InitData_6014_2_12_
	{(u32*)DL698_SETLIB_6014_2_12,},
#endif
#ifdef InitData_6014_2_13_
	{(u32*)DL698_SETLIB_6014_2_13,},
#endif
#ifdef InitData_6014_2_14_
	{(u32*)DL698_SETLIB_6014_2_14,},
#endif
#ifdef InitData_6014_2_15_
	{(u32*)DL698_SETLIB_6014_2_15,},
#endif
#ifdef InitData_6014_2_16_
	{(u32*)DL698_SETLIB_6014_2_16,},
#endif
#ifdef InitData_6014_2_17_
	{(u32*)DL698_SETLIB_6014_2_17,},
#endif
#ifdef InitData_6014_2_18_
	{(u32*)DL698_SETLIB_6014_2_18,},
#endif
#ifdef InitData_6014_2_19_
	{(u32*)DL698_SETLIB_6014_2_19,},
#endif
#ifdef InitData_6014_2_20_
	{(u32*)DL698_SETLIB_6014_2_20,},
#endif
#ifdef InitData_6014_2_21_
	{(u32*)DL698_SETLIB_6014_2_21,},
#endif
#ifdef InitData_6014_2_22_
	{(u32*)DL698_SETLIB_6014_2_22,},
#endif
#ifdef InitData_6014_2_23_
	{(u32*)DL698_SETLIB_6014_2_23,},
#endif
#ifdef InitData_6014_2_24_
	{(u32*)DL698_SETLIB_6014_2_24,},
#endif
#ifdef InitData_6014_2_25_
	{(u32*)DL698_SETLIB_6014_2_25,},
#endif
#ifdef InitData_6014_2_26_
	{(u32*)DL698_SETLIB_6014_2_26,},
#endif
#ifdef InitData_6014_2_27_
	{(u32*)DL698_SETLIB_6014_2_27,},
#endif
#ifdef InitData_6014_2_28_
	{(u32*)DL698_SETLIB_6014_2_28,},
#endif
#ifdef InitData_6014_2_29_
	{(u32*)DL698_SETLIB_6014_2_29,},
#endif
#ifdef InitData_6014_2_30_
	{(u32*)DL698_SETLIB_6014_2_30,},
#endif
#ifdef InitData_6014_2_31_
	{(u32*)DL698_SETLIB_6014_2_31,},
#endif
#ifdef InitData_6014_2_32_
	{(u32*)DL698_SETLIB_6014_2_32,},
#endif
#ifdef InitData_6014_2_33_
	{(u32*)DL698_SETLIB_6014_2_33,},
#endif
#ifdef InitData_6014_2_34_
	{(u32*)DL698_SETLIB_6014_2_34,},
#endif
#ifdef InitData_6014_2_35_
	{(u32*)DL698_SETLIB_6014_2_35,},
#endif
#ifdef InitData_6014_2_36_
	{(u32*)DL698_SETLIB_6014_2_36,},
#endif
#ifdef InitData_6014_2_37_
	{(u32*)DL698_SETLIB_6014_2_37,},
#endif
#ifdef InitData_6014_2_38_
	{(u32*)DL698_SETLIB_6014_2_38,},
#endif
#ifdef InitData_6014_2_39_
	{(u32*)DL698_SETLIB_6014_2_39,},
#endif
#ifdef InitData_6014_2_40_
	{(u32*)DL698_SETLIB_6014_2_40,},
#endif
#ifdef InitData_6014_2_41_
	{(u32*)DL698_SETLIB_6014_2_41,},
#endif
#ifdef InitData_6014_2_42_
	{(u32*)DL698_SETLIB_6014_2_42,},
#endif
#ifdef InitData_6014_2_43_
	{(u32*)DL698_SETLIB_6014_2_43,},
#endif
#ifdef InitData_6014_2_44_
	{(u32*)DL698_SETLIB_6014_2_44,},
#endif
#ifdef InitData_6014_2_45_
	{(u32*)DL698_SETLIB_6014_2_45,},
#endif
#ifdef InitData_6014_2_46_
	{(u32*)DL698_SETLIB_6014_2_46,},
#endif
#ifdef InitData_6014_2_47_
	{(u32*)DL698_SETLIB_6014_2_47,},
#endif
#ifdef InitData_6014_2_48_
	{(u32*)DL698_SETLIB_6014_2_48,},
#endif
#ifdef InitData_6014_2_49_
	{(u32*)DL698_SETLIB_6014_2_49,},
#endif
#ifdef InitData_6014_2_50_
	{(u32*)DL698_SETLIB_6014_2_50,},
#endif
#ifdef InitData_6014_2_51_
	{(u32*)DL698_SETLIB_6014_2_51,},
#endif
#ifdef InitData_6014_2_52_
	{(u32*)DL698_SETLIB_6014_2_52,},
#endif
#ifdef InitData_6014_2_53_
	{(u32*)DL698_SETLIB_6014_2_53,},
#endif
#ifdef InitData_6014_2_54_
	{(u32*)DL698_SETLIB_6014_2_54,},
#endif
#ifdef InitData_6014_2_55_
	{(u32*)DL698_SETLIB_6014_2_55,},
#endif
#ifdef InitData_6014_2_56_
	{(u32*)DL698_SETLIB_6014_2_56,},
#endif
#ifdef InitData_6014_2_57_
	{(u32*)DL698_SETLIB_6014_2_57,},
#endif
#ifdef InitData_6014_2_58_
	{(u32*)DL698_SETLIB_6014_2_58,},
#endif
#ifdef InitData_6014_2_59_
	{(u32*)DL698_SETLIB_6014_2_59,},
#endif
#ifdef InitData_6014_2_60_
	{(u32*)DL698_SETLIB_6014_2_60,},
#endif
#ifdef InitData_6014_2_61_
	{(u32*)DL698_SETLIB_6014_2_61,},
#endif
#ifdef InitData_6014_2_62_
	{(u32*)DL698_SETLIB_6014_2_62,},
#endif
#ifdef InitData_6014_2_63_
	{(u32*)DL698_SETLIB_6014_2_63,},
#endif
#ifdef InitData_6014_2_64_
	{(u32*)DL698_SETLIB_6014_2_64,},
#endif
	
#if NUMmax_6014>(64+1)//�����ͨ�ɼ�������
	#error
#endif
	
	{(u32*)DL698_SETLIB_6014_3,},
	{(u32*)DL698_SETLIB_6014_4,},
	
	{(u32*)DL698_SETLIB_6016_2,},//��λ��0����ַ������ɾ��
#ifdef InitData_6016_2_1_
	{(u32*)DL698_SETLIB_6016_2_1,},
#endif
#ifdef InitData_6016_2_2_
	{(u32*)DL698_SETLIB_6016_2_2,},
#endif
#ifdef InitData_6016_2_3_
	{(u32*)DL698_SETLIB_6016_2_3,},
#endif
#ifdef InitData_6016_2_4_
	{(u32*)DL698_SETLIB_6016_2_4,},
#endif
#ifdef InitData_6016_2_5_
	{(u32*)DL698_SETLIB_6016_2_5,},
#endif
#ifdef InitData_6016_2_6_
	{(u32*)DL698_SETLIB_6016_2_6,},
#endif
#ifdef InitData_6016_2_7_
	{(u32*)DL698_SETLIB_6016_2_7,},
#endif
#ifdef InitData_6016_2_8_
	{(u32*)DL698_SETLIB_6016_2_8,},
#endif
#ifdef InitData_6016_2_9_
	{(u32*)DL698_SETLIB_6016_2_9,},
#endif
#ifdef InitData_6016_2_10_
	{(u32*)DL698_SETLIB_6016_2_10,},
#endif
#ifdef InitData_6016_2_11_
	{(u32*)DL698_SETLIB_6016_2_11,},
#endif
#ifdef InitData_6016_2_12_
	{(u32*)DL698_SETLIB_6016_2_12,},
#endif
#ifdef InitData_6016_2_13_
	{(u32*)DL698_SETLIB_6016_2_13,},
#endif
#ifdef InitData_6016_2_14_
	{(u32*)DL698_SETLIB_6016_2_14,},
#endif
#ifdef InitData_6016_2_15_
	{(u32*)DL698_SETLIB_6016_2_15,},
#endif
#ifdef InitData_6016_2_16_
	{(u32*)DL698_SETLIB_6016_2_16,},
#endif
#ifdef InitData_6016_2_17_
	{(u32*)DL698_SETLIB_6016_2_17,},
#endif
#ifdef InitData_6016_2_18_
	{(u32*)DL698_SETLIB_6016_2_18,},
#endif
#ifdef InitData_6016_2_19_
	{(u32*)DL698_SETLIB_6016_2_19,},
#endif
#ifdef InitData_6016_2_20_
	{(u32*)DL698_SETLIB_6016_2_20,},
#endif
#ifdef InitData_6016_2_21_
	{(u32*)DL698_SETLIB_6016_2_21,},
#endif
#ifdef InitData_6016_2_22_
	{(u32*)DL698_SETLIB_6016_2_22,},
#endif
#ifdef InitData_6016_2_23_
	{(u32*)DL698_SETLIB_6016_2_23,},
#endif
#ifdef InitData_6016_2_24_
	{(u32*)DL698_SETLIB_6016_2_24,},
#endif
#ifdef InitData_6016_2_25_
	{(u32*)DL698_SETLIB_6016_2_25,},
#endif
#ifdef InitData_6016_2_26_
	{(u32*)DL698_SETLIB_6016_2_26,},
#endif
#ifdef InitData_6016_2_27_
	{(u32*)DL698_SETLIB_6016_2_27,},
#endif
#ifdef InitData_6016_2_28_
	{(u32*)DL698_SETLIB_6016_2_28,},
#endif
#ifdef InitData_6016_2_29_
	{(u32*)DL698_SETLIB_6016_2_29,},
#endif
#ifdef InitData_6016_2_30_
	{(u32*)DL698_SETLIB_6016_2_30,},
#endif
#ifdef InitData_6016_2_31_
	{(u32*)DL698_SETLIB_6016_2_31,},
#endif
#ifdef InitData_6016_2_32_
	{(u32*)DL698_SETLIB_6016_2_32,},
#endif
#ifdef InitData_6016_2_33_
	{(u32*)DL698_SETLIB_6016_2_33,},
#endif
#ifdef InitData_6016_2_34_
	{(u32*)DL698_SETLIB_6016_2_34,},
#endif
#ifdef InitData_6016_2_35_
	{(u32*)DL698_SETLIB_6016_2_35,},
#endif
#ifdef InitData_6016_2_36_
	{(u32*)DL698_SETLIB_6016_2_36,},
#endif
#ifdef InitData_6016_2_37_
	{(u32*)DL698_SETLIB_6016_2_37,},
#endif
#ifdef InitData_6016_2_38_
	{(u32*)DL698_SETLIB_6016_2_38,},
#endif
#ifdef InitData_6016_2_39_
	{(u32*)DL698_SETLIB_6016_2_39,},
#endif
#ifdef InitData_6016_2_40_
	{(u32*)DL698_SETLIB_6016_2_40,},
#endif
#ifdef InitData_6016_2_41_
	{(u32*)DL698_SETLIB_6016_2_41,},
#endif
#ifdef InitData_6016_2_42_
	{(u32*)DL698_SETLIB_6016_2_42,},
#endif
#ifdef InitData_6016_2_43_
	{(u32*)DL698_SETLIB_6016_2_43,},
#endif
#ifdef InitData_6016_2_44_
	{(u32*)DL698_SETLIB_6016_2_44,},
#endif
#ifdef InitData_6016_2_45_
	{(u32*)DL698_SETLIB_6016_2_45,},
#endif
#ifdef InitData_6016_2_46_
	{(u32*)DL698_SETLIB_6016_2_46,},
#endif
#ifdef InitData_6016_2_47_
	{(u32*)DL698_SETLIB_6016_2_47,},
#endif
#ifdef InitData_6016_2_48_
	{(u32*)DL698_SETLIB_6016_2_48,},
#endif
#ifdef InitData_6016_2_49_
	{(u32*)DL698_SETLIB_6016_2_49,},
#endif
#ifdef InitData_6016_2_50_
	{(u32*)DL698_SETLIB_6016_2_50,},
#endif
#ifdef InitData_6016_2_51_
	{(u32*)DL698_SETLIB_6016_2_51,},
#endif
#ifdef InitData_6016_2_52_
	{(u32*)DL698_SETLIB_6016_2_52,},
#endif
#ifdef InitData_6016_2_53_
	{(u32*)DL698_SETLIB_6016_2_53,},
#endif
#ifdef InitData_6016_2_54_
	{(u32*)DL698_SETLIB_6016_2_54,},
#endif
#ifdef InitData_6016_2_55_
	{(u32*)DL698_SETLIB_6016_2_55,},
#endif
#ifdef InitData_6016_2_56_
	{(u32*)DL698_SETLIB_6016_2_56,},
#endif
#ifdef InitData_6016_2_57_
	{(u32*)DL698_SETLIB_6016_2_57,},
#endif
#ifdef InitData_6016_2_58_
	{(u32*)DL698_SETLIB_6016_2_58,},
#endif
#ifdef InitData_6016_2_59_
	{(u32*)DL698_SETLIB_6016_2_59,},
#endif
#ifdef InitData_6016_2_60_
	{(u32*)DL698_SETLIB_6016_2_60,},
#endif
#ifdef InitData_6016_2_61_
	{(u32*)DL698_SETLIB_6016_2_61,},
#endif
#ifdef InitData_6016_2_62_
	{(u32*)DL698_SETLIB_6016_2_62,},
#endif
#ifdef InitData_6016_2_63_
	{(u32*)DL698_SETLIB_6016_2_63,},
#endif
#ifdef InitData_6016_2_64_
	{(u32*)DL698_SETLIB_6016_2_64,},
#endif
	
#if NUMmax_6016>(64+1)//����¼��ɼ�������
	#error
#endif
	
	
	{(u32*)DL698_SETLIB_6016_3,},
	{(u32*)DL698_SETLIB_6016_4,},
	{(u32*)DL698_SETLIB_6018,},
	{(u32*)DL698_SETLIB_6018_3,},
	{(u32*)DL698_SETLIB_6018_4,},
	{(u32*)DL698_SETLIB_601A,},
	{(u32*)DL698_SETLIB_601A_3,},
	{(u32*)DL698_SETLIB_601A_4,},
	{(u32*)DL698_SETLIB_601C,},
#ifdef InitData_601C_2_1_
    {(u32*)DL698_SETLIB_601C_2_1},
#endif
#ifdef InitData_601C_2_2_
    {(u32*)DL698_SETLIB_601C_2_2},
#endif
#ifdef InitData_601C_2_3_
    {(u32*)DL698_SETLIB_601C_2_3},
#endif
#ifdef InitData_601C_2_4_
    {(u32*)DL698_SETLIB_601C_2_4},
#endif
#ifdef InitData_601C_2_5_
    {(u32*)DL698_SETLIB_601C_2_5},
#endif
#ifdef InitData_601C_2_6_
    {(u32*)DL698_SETLIB_601C_2_6},
#endif
#ifdef InitData_601C_2_7_
    {(u32*)DL698_SETLIB_601C_2_7},
#endif
#ifdef InitData_601C_2_8_
    {(u32*)DL698_SETLIB_601C_2_8},
#endif
#ifdef InitData_601C_2_9_
    {(u32*)DL698_SETLIB_601C_2_9},
#endif    
#ifdef InitData_601C_2_10_
    {(u32*)DL698_SETLIB_601C_2_10},
#endif    
#ifdef InitData_601C_2_11_
    {(u32*)DL698_SETLIB_601C_2_11},
#endif
#ifdef InitData_601C_2_12_
    {(u32*)DL698_SETLIB_601C_2_12},
#endif    
#ifdef InitData_601C_2_13_
    {(u32*)DL698_SETLIB_601C_2_13},
#endif    
#ifdef InitData_601C_2_14_
    {(u32*)DL698_SETLIB_601C_2_14},
#endif   
#ifdef InitData_601C_2_15_
    {(u32*)DL698_SETLIB_601C_2_15},
#endif    
#ifdef InitData_601C_2_16_
    {(u32*)DL698_SETLIB_601C_2_16},
#endif    
#ifdef InitData_601C_2_17_
    {(u32*)DL698_SETLIB_601C_2_17},
#endif
#ifdef InitData_601C_2_18_
    {(u32*)DL698_SETLIB_601C_2_18},
#endif
#ifdef InitData_601C_2_19_
    {(u32*)DL698_SETLIB_601C_2_19},
#endif    
#ifdef InitData_601C_2_20_
    {(u32*)DL698_SETLIB_601C_2_20},
#endif    
#ifdef InitData_601C_2_21_
    {(u32*)DL698_SETLIB_601C_2_21},
#endif
#ifdef InitData_601C_2_22_
    {(u32*)DL698_SETLIB_601C_2_22},
#endif
#ifdef InitData_601C_2_23_
    {(u32*)DL698_SETLIB_601C_2_23},
#endif
#ifdef InitData_601C_2_24_
    {(u32*)DL698_SETLIB_601C_2_24},
#endif
#ifdef InitData_601C_2_25_
    {(u32*)DL698_SETLIB_601C_2_25},
#endif
#ifdef InitData_601C_2_26_
    {(u32*)DL698_SETLIB_601C_2_26},
#endif
#ifdef InitData_601C_2_27_
    {(u32*)DL698_SETLIB_601C_2_27},
#endif
#ifdef InitData_601C_2_28_
    {(u32*)DL698_SETLIB_601C_2_28},
#endif
#ifdef InitData_601C_2_29_
    {(u32*)DL698_SETLIB_601C_2_29},
#endif   
#ifdef InitData_601C_2_30_
    {(u32*)DL698_SETLIB_601C_2_30},
#endif     
#ifdef InitData_601C_2_31_
    {(u32*)DL698_SETLIB_601C_2_31},
#endif
#ifdef InitData_601C_2_32_
    {(u32*)DL698_SETLIB_601C_2_32},
#endif
#ifdef InitData_601C_2_33_
    {(u32*)DL698_SETLIB_601C_2_33},
#endif
#ifdef InitData_601C_2_34_
    {(u32*)DL698_SETLIB_601C_2_34},
#endif
#ifdef InitData_601C_2_35_
    {(u32*)DL698_SETLIB_601C_2_35},
#endif
#ifdef InitData_601C_2_36_
    {(u32*)DL698_SETLIB_601C_2_36},
#endif
#ifdef InitData_601C_2_37_
    {(u32*)DL698_SETLIB_601C_2_37},
#endif
#ifdef InitData_601C_2_38_
    {(u32*)DL698_SETLIB_601C_2_38},
#endif
#ifdef InitData_601C_2_39_
    {(u32*)DL698_SETLIB_601C_2_39},
#endif    
#ifdef InitData_601C_2_40_
    {(u32*)DL698_SETLIB_601C_2_40},
#endif     
#ifdef InitData_601C_2_41_
    {(u32*)DL698_SETLIB_601C_2_41},
#endif
#ifdef InitData_601C_2_42_
    {(u32*)DL698_SETLIB_601C_2_42},
#endif
#ifdef InitData_601C_2_43_
    {(u32*)DL698_SETLIB_601C_2_43},
#endif
#ifdef InitData_601C_2_44_
    {(u32*)DL698_SETLIB_601C_2_44},
#endif
#ifdef InitData_601C_2_45_
    {(u32*)DL698_SETLIB_601C_2_45},
#endif
#ifdef InitData_601C_2_46_
    {(u32*)DL698_SETLIB_601C_2_46},
#endif
#ifdef InitData_601C_2_47_
    {(u32*)DL698_SETLIB_601C_2_47},
#endif
#ifdef InitData_601C_2_48_
    {(u32*)DL698_SETLIB_601C_2_48},
#endif
#ifdef InitData_601C_2_49_
    {(u32*)DL698_SETLIB_601C_2_49},
#endif
#ifdef InitData_601C_2_50_
    {(u32*)DL698_SETLIB_601C_2_50},
#endif
#ifdef InitData_601C_2_51_
    {(u32*)DL698_SETLIB_601C_2_51},
#endif
#ifdef InitData_601C_2_52_
    {(u32*)DL698_SETLIB_601C_2_52},
#endif
#ifdef InitData_601C_2_53_
    {(u32*)DL698_SETLIB_601C_2_53},
#endif
#ifdef InitData_601C_2_54_
    {(u32*)DL698_SETLIB_601C_2_54},
#endif
#ifdef InitData_601C_2_55_
    {(u32*)DL698_SETLIB_601C_2_55},
#endif
#ifdef InitData_601C_2_56_
    {(u32*)DL698_SETLIB_601C_2_56},
#endif
#ifdef InitData_601C_2_57_
    {(u32*)DL698_SETLIB_601C_2_57},
#endif
#ifdef InitData_601C_2_58_
    {(u32*)DL698_SETLIB_601C_2_58},
#endif
#ifdef InitData_601C_2_59_
    {(u32*)DL698_SETLIB_601C_2_59},
#endif
#ifdef InitData_601C_2_60_
    {(u32*)DL698_SETLIB_601C_2_60},
#endif
#ifdef InitData_601C_2_61_
    {(u32*)DL698_SETLIB_601C_2_61},
#endif
#ifdef InitData_601C_2_62_
    {(u32*)DL698_SETLIB_601C_2_62},
#endif
#ifdef InitData_601C_2_63_
    {(u32*)DL698_SETLIB_601C_2_63},
#endif
#ifdef InitData_601C_2_64_
    {(u32*)DL698_SETLIB_601C_2_64},
#endif
#if NUMmax_601C>(64+1)//����ϱ�������
	#error
#endif
	
	{(u32*)DL698_SETLIB_601C_3,},
	{(u32*)DL698_SETLIB_601C_4,},
	{(u32*)DL698_SETLIB_601E,},
	{(u32*)DL698_SETLIB_601E_3,},
	{(u32*)DL698_SETLIB_601E_4,},
	{(u32*)DL698_SETLIB_6032,},
	{(u32*)DL698_SETLIB_6032_3,},
	{(u32*)DL698_SETLIB_6032_4,},
	{(u32*)DL698_SETLIB_6034,},
	{(u32*)DL698_SETLIB_6034_3,},
	{(u32*)DL698_SETLIB_6034_4,},

#if (USER/100)==14//��������
	{(u32*)DL698_SETLIB_6E00,},
	{(u32*)DL698_SETLIB_6E01,},
	{(u32*)DL698_SETLIB_6E11,},
	{(u32*)DL698_SETLIB_6E13,},
	{(u32*)DL698_SETLIB_6E17,},
	{(u32*)DL698_SETLIB_6E30,},
	{(u32*)DL698_SETLIB_6E33,},

#endif//#if (USER/100)==14//��������
#if (USER/100)==15//����
	{(u32*)DL698_SETLIB_3106_11,},
#endif
	
//#if (USER/100)==17//???
	{(u32*)DL698_SETLIB_4048,},
//#endif
	
//������
	
	
	
//������
	{(u32*)DL698_SETLIB_8000_2,},
	{(u32*)DL698_SETLIB_8000_3,},
	{(u32*)DL698_SETLIB_8000_4,},
	{(u32*)DL698_SETLIB_8000_5,},
	{(u32*)DL698_SETLIB_8001_2,},
	{(u32*)DL698_SETLIB_8001_3,},
	{(u32*)DL698_SETLIB_8001_4,},
	{(u32*)DL698_SETLIB_8001_5,},
	{(u32*)DL698_SETLIB_8002_2,},
	{(u32*)DL698_SETLIB_8002_3,},
	
	{(u32*)DL698_SETLIB_8100_2,},
	{(u32*)DL698_SETLIB_8101_2,},
	{(u32*)DL698_SETLIB_8102_2,},
	{(u32*)DL698_SETLIB_8103_2,},
	{(u32*)DL698_SETLIB_8103_3,},
	{(u32*)DL698_SETLIB_8103_4,},
	{(u32*)DL698_SETLIB_8103_5,},
	{(u32*)DL698_SETLIB_8104_2,},
	{(u32*)DL698_SETLIB_8104_3,},
	{(u32*)DL698_SETLIB_8104_4,},
	{(u32*)DL698_SETLIB_8104_5,},
	{(u32*)DL698_SETLIB_8105_2,},
	{(u32*)DL698_SETLIB_8105_3,},
	{(u32*)DL698_SETLIB_8105_4,},
	{(u32*)DL698_SETLIB_8105_5,},
	{(u32*)DL698_SETLIB_8106_3,},
	{(u32*)DL698_SETLIB_8106_4,},
	{(u32*)DL698_SETLIB_8106_5,},
	{(u32*)DL698_SETLIB_8107_2,},
	{(u32*)DL698_SETLIB_8107_3,},
	{(u32*)DL698_SETLIB_8107_4,},
	{(u32*)DL698_SETLIB_8107_5,},
	{(u32*)DL698_SETLIB_8108_2,},
	{(u32*)DL698_SETLIB_8108_3,},
	{(u32*)DL698_SETLIB_8108_4,},
	{(u32*)DL698_SETLIB_8108_5,},
	
	{(u32*)DL698_SETLIB_230x_2,},//�ܼ����ñ�(�����ڳ�ʼ��)
	{(u32*)DL698_SETLIB_230x_13,},//�ܼ��黬��ʱ������(�����ڳ�ʼ��)
	{(u32*)DL698_SETLIB_230x_14,},//�ܼ��鹦���ִ�����(�����ڳ�ʼ��)
	{(u32*)DL698_SETLIB_230x_15,},//�ܼ������ִ�����(�����ڳ�ʼ��)
	{(u32*)DL698_SETLIB_240x_2,},//�������-ͨ�ŵ�ַ(�����ڳ�ʼ��)
	{(u32*)DL698_SETLIB_240x_3,},//�������-����������(�����ڳ�ʼ��)
	{(u32*)DL698_SETLIB_240x_4,},//�������-��������(�����ڳ�ʼ��)
	
	
//�ļ�������
	{(u32*)DL698_SETLIB_F002_4,},//FTP��������Ϣ
	
//ESAM�ӿ���
	{(u32*)DL698_SETLIB_F100_2,},
	{(u32*)DL698_SETLIB_F100_3,},
	{(u32*)DL698_SETLIB_F100_4,},
	{(u32*)DL698_SETLIB_F100_5,},
	{(u32*)DL698_SETLIB_F100_6,},
	{(u32*)DL698_SETLIB_F100_7,},
	{(u32*)DL698_SETLIB_F100_8,},
	{(u32*)DL698_SETLIB_F100_9,},
	{(u32*)DL698_SETLIB_F100_10,},
	{(u32*)DL698_SETLIB_F100_11,},
	{(u32*)DL698_SETLIB_F100_12,},
	{(u32*)DL698_SETLIB_F100_13,},
	{(u32*)DL698_SETLIB_F101_2,},
	{(u32*)DL698_SETLIB_F101_3,},
//��������豸��
	{(u32*)DL698_SETLIB_F200_2,},
	{(u32*)DL698_SETLIB_F200_3,},
	{(u32*)DL698_SETLIB_F201_2,},
	{(u32*)DL698_SETLIB_F201_3,},
	{(u32*)DL698_SETLIB_F202_2,},
	{(u32*)DL698_SETLIB_F202_3,},
	{(u32*)DL698_SETLIB_F203_2,},
	{(u32*)DL698_SETLIB_F203_3,},
	{(u32*)DL698_SETLIB_F203_4,},
	{(u32*)DL698_SETLIB_F204_2,},
	{(u32*)DL698_SETLIB_F204_3,},
	{(u32*)DL698_SETLIB_F204_4,},
	{(u32*)DL698_SETLIB_F205_2,},
	{(u32*)DL698_SETLIB_F205_3,},
	{(u32*)DL698_SETLIB_F206_2,},
	{(u32*)DL698_SETLIB_F206_3,},
	{(u32*)DL698_SETLIB_F206_4,},
	{(u32*)DL698_SETLIB_F207_2,},
	{(u32*)DL698_SETLIB_F207_3,},
	{(u32*)DL698_SETLIB_F209_2,},
	{(u32*)DL698_SETLIB_F209_3,},
	{(u32*)DL698_SETLIB_F209_6,},
	{(u32*)DL698_SETLIB_F209_9,},
	{(u32*)DL698_SETLIB_F209_11,},
	{(u32*)DL698_SETLIB_F209_22,},
	{(u32*)DL698_SETLIB_F20A_2,},
	{(u32*)DL698_SETLIB_F20A_3,},
	{(u32*)DL698_SETLIB_F20B_2,},
	{(u32*)DL698_SETLIB_F20B_3,},
	

//��ʾ��
	
	
};










DL698_SETLIB_TypeDef *Get_DL698_SET_pLib(u32 OAD)//�õ�SET_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
{
	u32 i;
	DL698_SETLIB_TypeDef *pLib;

	for(i=0;i<sizeof(DL698_SETLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_List[i].pLib;
		if(OAD==pLib->OAD)
		{
			return pLib;
		}
	}
	
	return 0;
}



extern const u8 InitData_6014_2_9[];//ʵʱ�ɼ�����

#if (USER/100)==9//���ϰ�
void FileID_V1toV2(void)//FileID�汾V1->V2
{
//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
//V2����V1�ؼ���:
	//1. ���Ӳɼ�����14=�¶�������
	//2. FileID�еĴ洢ʱ������V1=0-7,V2=0xfͬʱ��ʾ��Ϊ������
//--V1---
	//ʵʱ����0x10xxxxxx
	//�ն����¶�������0x12xxxxx
	//��������0x136xxxxx
//--V2---
	//ʵʱ����0x10Fxxxxx
	//�ն�������0x12Fxxxx
	//��������0x13Fxxxxx
	//�¶�������0x1EFxxxxx
	u32 FILEID;
	u32 filename;
	msFILE_TypeDef *file;
	u32 NUMmax_File;
	ms_Type *ms;

	ms=Get_ms();
	NUMmax_File=((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(msFILE_TypeDef));
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<NUMmax_File;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			FILEID=file->FILEID;
			if((FILEID&0xff)==0)
			{//�ļ�����=0;��ͨ�ɼ�������¼,ȫ�¼��ɼ�������¼
				if((FILEID>>28)==1)
				{//��������=1;1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
					switch((FILEID>>20)&0xf)
					{//�洢ʱ������
						case 0xf:
							break;
					//V1��ʹ�õĴ洢ʱ������
					//δ����              ��0����
					//����ʼʱ��        ��1����
					//��Ե���0��0��    ��2����
					//�������23��59��  ��3����
					//�������0��0��    ��4����
					//��Ե���1��0��0�֣�5����
					//���ݶ���ʱ��        ��6��
					//���������ĩ 23 �� 59 �� ��7��
						case 7:
							//ɾ��V1���¶����ļ�
							ms->msfile_Delete(filename);
							break;
						default:
							//�����ļ�
							FILEID|=0xf<<20;//���⣬�Զ�ɾ�������Ѳ����ڵļ�¼�ļ�
							file->FILEID=FILEID;
							break;
					}
				}
			}
		}
		file++;
	}
}
void DataMoveNewAddr(u8* pSOUR)//�������ļ�����ת���µ�ַ,���:ԭ���ò�������RAM��ָ��
{
//V1.00
//#define RMTASKmax 			60//���������
//#define NUMmax_6012_2   RMTASKmax//��󳭱�������
//#define NUMmax_6014     60//�����ͨ�ɼ�������
//#define NUMmax_6016     32//����¼��ɼ�������
//#define NUMmax_6018     32//���͸��������
//#define NUMmax_601C     16//����ϱ�������(����Ӱ���ļ����������ļ�����)
typedef __packed struct
{
	u16 BLOCK_ENTRY;//�ļ����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�(ֵ��Чʱ!=0)
	u16 BLOCK_END;//�ļ�����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�
	u32 LEN_FILE;//�ļ�Ԥ��ռ�д洢�ռ��ܳ���
	u32 LEN_DATA;//�ļ���Ч�����ܳ���
	
	u32 FILEID;//FILEID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	//�ļ�����:
	//0=��ͨ�ɼ��������ݼ�¼
	//1=
	//2=��OI������¼���¼
	//
	//
	u16 FILECOUNT;//��ͬFILEID���ļ�������
	u16 NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
	u16 NUM_FAIL;//�ļ�ͷ��д�볭��ʧ�ܵ�ַ����
	u8 save_date_time_s[7];//�ɼ��洢ʱ��
	u8 start_date_time_s[7];//�ɼ�����ʱ��
	u16 NUM_GPRSreport[16];//���ϱ������ݸ���(����ϱ�������)
	u16 NUM_ENETreport[16];//���ϱ������ݸ���(����ϱ�������)
}V100_msFILE_TypeDef;

//V1.10
//#define RMTASKmax 			64//���������
//#define NUMmax_6012_2   RMTASKmax//��󳭱�������
//#define NUMmax_6014     64//60//�����ͨ�ɼ�������
//#define NUMmax_6016     64//32//����¼��ɼ�������
//#define NUMmax_6018     64//32//���͸��������
//#define NUMmax_601C     64//16//����ϱ�������(����Ӱ���ļ����������ļ�����)
typedef __packed struct
{
	u16 BLOCK_ENTRY;//�ļ����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�(ֵ��Чʱ!=0)
	u16 BLOCK_END;//�ļ�����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�
	u32 LEN_FILE;//�ļ�Ԥ��ռ�д洢�ռ��ܳ���
	u32 LEN_DATA;//�ļ���Ч�����ܳ���
	
	u32 FILEID;//FILEID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	//�ļ�����:
	//0=��ͨ�ɼ��������ݼ�¼
	//1=
	//2=��OI������¼���¼
	//
	//
	u16 FILECOUNT;//��ͬFILEID���ļ�������
	u16 NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
	u16 NUM_FAIL;//�ļ�ͷ��д�볭��ʧ�ܵ�ַ����
	u8 save_date_time_s[7];//�ɼ��洢ʱ��
	u8 start_date_time_s[7];//�ɼ�����ʱ��
 #if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
//GPRS,ENET�����ϱ�
	u16 NUM_GPRSreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
	u16 NUM_ENETreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
 #else
//GPRS,ENET�ϲ��ϱ�
	u16 NUM_GPRSorENETreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
 #endif
}V110_msFILE_TypeDef;

	u32 i;
	u32 n;
	u8* p8;
	u32* p32;
	u32 filename;
	V100_msFILE_TypeDef *V100_file;
	//V110_msFILE_TypeDef *V110_file;
	u32 NUMmax_FileSour;
	u32 NUMmax_File_New;
	ms_Type *ms;
	os_Type *os;
	FILE* fsfile;


	ms=Get_ms();
	os=Get_os();
	p8=pSOUR;
	p8+=(0x31b4bbbb-(ADDR_DL698_SET_START));//ADDR_6014+(9*512);//ԭ��ͨ�ɼ�����9���Ե�ַ
	for(i=0;i<sizeof(InitData_6014_2_9);i++)
	{
		if(p8[i]!=InitData_6014_2_9[i])
		{
			break;
		}
	}
	if(i>=sizeof(InitData_6014_2_9))
	{//ԭ���ò����Ǹ���ǰV1.00�Ĳ���
		//����ת��
		p8=pSOUR;
		p8+=(0x31b28b40-(ADDR_DL698_SET_START));//ADDR_6000;//ԭ�ɼ��������ñ���Ե�ַ
		MW((u32)p8,ADDR_6000,128*1024);
		//�ļ�����ת��
//#define FILENAMEmax_FLASH     ((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(msFILE_TypeDef))//�洢��FLASH�е��ļ�������ļ���
		NUMmax_FileSour=((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(V100_msFILE_TypeDef));
		NUMmax_File_New=((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(V110_msFILE_TypeDef));
		MC(0,(u32)pSOUR,(LEN_msFILEmanage-LEN_msFILEunltFlags-12));
		n=0;
		V100_file=(V100_msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		//V110_file=(V110_msFILE_TypeDef*)(pSOUR);
		for(filename=0;filename<NUMmax_FileSour;filename++)
		{
			if(V100_file->BLOCK_ENTRY)
			{
				if(V100_file->FILECOUNT<400)//(3*96)//�����������������,���¶�������ļ�����ԭС���޷�ȫ��ת��
				{
					if((V100_file->FILEID&0xff)<=1)//����ת���ļ�����,���¶�������ļ�����ԭС���޷�ȫ��ת��
					{
					//0=��ͨ�ɼ�����,ȫ�¼��ɼ��������ݼ�¼
					//1=���ݶ���(�ն�������)
					//2=��OI������¼���¼(�ն�������)
					//3=�����ѱ���
					//4=��̨���ѱ���
					//5=���ܱ������ϱ�
						if(n<NUMmax_File_New)
						{
						#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
							MW(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(V100_msFILE_TypeDef)),(u32)pSOUR+(n*sizeof(V110_msFILE_TypeDef)),sizeof(V100_msFILE_TypeDef)-(2*16));
							MW(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(V100_msFILE_TypeDef))+(sizeof(V100_msFILE_TypeDef)-(2*16)),(u32)pSOUR+(n*sizeof(V110_msFILE_TypeDef))+(sizeof(V110_msFILE_TypeDef)-(2*NUMmax_601C)),(2*16));
						#else
							MW(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(V100_msFILE_TypeDef)),(u32)pSOUR+(n*sizeof(V110_msFILE_TypeDef)),sizeof(V100_msFILE_TypeDef)-(2*16));
						#endif
							n++;
							//V110_file++;
							V100_file++;
							continue;
						}
					}
				}
				//ûת�Ƶ���ͬ��ɾ���ļ����ռ�õ�flash
				ms->msfile_Delete(filename);
			}
			V100_file++;
		}
		MW((u32)pSOUR,ADDR_msFILEmanage+LEN_msFILEunltFlags,LEN_msFILEmanage-LEN_msFILEunltFlags-12);
		
		FileID_V1toV2();//FileID�汾V1->V2
		//CRC����
		p32=(u32*)(ADDR_msFILEmanage+LEN_msFILEmanage-8);
		p32[0]=0x12345678;
		p32[1]=RAM_CRC32(ADDR_msFILEmanage,LEN_msFILEmanage-4);//RAM������CRC32����
		MW(ADDR_msFILEmanage,ADDR_msFILEmanage+LEN_msFILEmanage,LEN_msFILEmanage);
		//д��os�ļ�
		os->fchdrive ("N0:");
		os->fchdir("N0:/");
		fsfile=os->fopen("msFILEmanage","w");
		if(fsfile!=NULL)
		{
			i=os->fwrite((void*)ADDR_msFILEmanage,1,LEN_msFILEmanage,fsfile);
			if(i!=LEN_msFILEmanage)
			{
				i=i;
			}
			os->fclose(fsfile);
		}
		return;
	}
	p8=pSOUR;
	p8+=(0x31b4bdbb-(ADDR_DL698_SET_START));//ADDR_6014+(9*512);//ԭ��ͨ�ɼ�����9���Ե�ַ
	for(i=0;i<sizeof(InitData_6014_2_9);i++)
	{
		if(p8[i]!=InitData_6014_2_9[i])
		{
			break;
		}
	}
	if(i>=sizeof(InitData_6014_2_9))
	{//ԭ���ò�����V1.10���Ժ�Ĳ���
		//����ת��
		p8=pSOUR;
		p8+=(0x31b28b40-(ADDR_DL698_SET_START));//ADDR_6000;//ԭ�ɼ��������ñ���Ե�ַ
		MW((u32)p8,ADDR_6000,128*1024);
		//�ļ�����ת��
		FileID_V1toV2();//FileID�汾V1->V2
		return;
	}
	
}
#endif//#if (USER/100)==9//���ϰ�


void DL698_SETINIT_601E(u32 Command);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
void DL698_SETINIT(u32 Command,u8* pOAD)//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
{
	u32 i;
	u32 x;
	u32 n;
	u32 OAD;
	u8* p8;
	DL698_SETLIB_TypeDef *pLib;
	ms_Type *ms;
	u8* pSOUR;
	
	ms=Get_ms();
#ifdef IC_STM32F4xx
	ms->Init_my_heap();
#endif
	i=LEN_FLASH_DL698_SET;
	if(i<LEN_msFILEmanage)
	{
		i=LEN_msFILEmanage;
	}
	pSOUR=ms->ms_malloc(i);//�����������ܳ���
	if(pSOUR)
	{//���ڴ����
		//��ԭ��������
		MR((u32)pSOUR,ADDR_DL698_SET_START,(ADDR_DL698_SET_END)-(ADDR_DL698_SET_START));
	}
	else
	{//�ڴ����ʧ��
		if((Command==3)||(Command==7))
		{
			Command=4;//4/5=����ϵͳ��վͨ����Ĳ�����ʼ��
		}
	}
	if(Command==6)
	{//6=�����ݲ�����ʼ��
		for(i=0;i<sizeof(DL698_SETLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
		{
			pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_List[i].pLib;
			if(pLib->pInitData)
			{
				if(pLib->LEN_Sour>pLib->LENmax)
				{//ԭʼ���ݳ���>FLASHԤ������󳤶�
					Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
					DisplayStringAutoUp(0,9,(u8*)"\x0""�������ȳ���!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
					continue;
				}
				switch(pLib->OAD)
				{
			#if (USER/100)==9//����
					//���ݲ�����
					case 0x40160200://��ǰ����ʱ�α�
					case 0x45200200://����Զ��ͨ�Ŷ����㱸��ͨ��
					case 0x45000200://����ͨ��ģ��1; 2ͨ������
						break;
			#endif
					
					case 0xffff://��OI,������ʱ������
						break;
					default:
						continue;
				}
				MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
			}
		}
		ms->ms_free(pSOUR);
		DL698_SETINIT_EVENT(Command,pOAD);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
		DL698_SETINIT_601E(Command);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
		return;
	}
	
#if (USER/100)==9//���ϰ�
	if(Comm_Ram->Factory==0x55)
	{//�˵������ʼ��
		i=(ProduceDate_year_BCD<<8)|ProduceDate_month_BCD;
	}
	else
	{
		i=MRR(ADDR_MenuKey,3);//�˵�����
	}
#endif
	
	if(Comm_Ram->Factory==0x55)
	{
		MC(0,ADDR_DL698_COMSET_END,(ADDR_DL698_SET_END)-(ADDR_DL698_COMSET_END));//��0
	}
	else
	{
		//�ǹ���ģʽ���ļ������״̬����0
		MC(0,ADDR_DL698_COMSET_END,(ADDR_F001_4)-(ADDR_DL698_COMSET_END));//��0
		MC(0,ADDR_F001_END,(ADDR_DL698_SET_END)-(ADDR_F001_END));//��0
	}
	
#if (USER/100)==9//���ϰ�
	MWR(i,ADDR_MenuKey,3);//�˵�����
#endif
	
	for(i=0;i<sizeof(DL698_SETLIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_List[i].pLib;
	#if (USER/100)==15//����
		if(pLib->OAD==0x43000800)
		{
			MR(ADDR_DATABUFF,ADDR_4300_8,LENmax_4300_8);
			p8=(u8*)ADDR_DATABUFF;
			p8[0]=DataType_bool;
			p8[1]=1;
			MW(ADDR_DATABUFF,ADDR_4300_8,LENmax_4300_8);
			continue;
		}
	#endif
		if(pLib->pInitData)
		{
			if(pLib->rw&0x04)
			{//ͨ�Ų���
				if((Command!=3)&&(Command!=7))
				{//��ͨ�Ų�����
					continue;
				}
				if((pLib->OAD==0x40010200)&&(Comm_Ram->Factory!=0x55))
				{//ͨ�ŵ�ַ�ڷǹ���״̬����
					continue;
				}
			}
			if(pLib->LEN_Sour>pLib->LENmax)
			{//ԭʼ���ݳ���>FLASHԤ������󳤶�
				Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				DisplayStringAutoUp(0,9,(u8*)"\x0""�������ȳ���!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				continue;
			}
			if(pOAD)
			{//���������OAD�б�������״�����ڻָ�֮��
				p8=pOAD;//������=array OAD
				n=p8[1];
				p8+=2;
				for(x=0;x<n;x++)
				{
					OAD=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
					if(OAD==pLib->OAD)
					{
						break;
					}
					p8+=5;
				}
				if(x<n)
				{//���б���ԭ��������
					if(pSOUR)
					{//���ڴ����
						if(pLib->Addr)
						{
							n=pLib->Addr-(ADDR_DL698_SET_START);
							n+=(u32)pSOUR;
							MW(n,(u32)pLib->Addr,pLib->LENmax);
						}
					}
					continue;
				}
			}
		#if (USER/100)!=0//���ǵ��Ժ����
			if((pLib->OAD==0x40010200)&&(Comm_Ram->Factory!=0x55))
			{//�ָ�����ʱͨ�ŵ�ַ
				MR(ADDR_DATABUFF,ADDR_4001_Factory,LENmax_4001);
				p8=(u8*)ADDR_DATABUFF;
				p8[0]=DataType_octet_string;
				p8[1]=6;
				MW(ADDR_DATABUFF,ADDR_4001,LENmax_4001);
				continue;
			}
		#endif
			if((pLib->OAD==0x40020200)&&(Comm_Ram->Factory!=0x55))
			{//�ָ�����ʱ���
				MR(ADDR_DATABUFF,ADDR_4001_Factory+LENmax_4001,LENmax_4002);
				p8=(u8*)ADDR_DATABUFF;
				p8[0]=DataType_octet_string;
				p8[1]=6;
				MW(ADDR_DATABUFF,ADDR_4002,LENmax_4002);
				continue;
			}
			if((pLib->OAD==0x40030200)&&(Comm_Ram->Factory!=0x55))
			{//�ָ�����ʱ�ͻ����
				MR(ADDR_DATABUFF,ADDR_4001_Factory+LENmax_4001+LENmax_4002,LENmax_4003);
				p8=(u8*)ADDR_DATABUFF;
				p8[0]=DataType_octet_string;
				p8[1]=6;
				MW(ADDR_DATABUFF,ADDR_4003,LENmax_4003);
				continue;
			}
			MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
		}
	}
	
	if(Command==7)//7=������ʼ��(������nand�ļ�����ת�Ƶ��µ�ַ)
	{
	#if (USER/100)==9//���ϰ�
		DataMoveNewAddr(pSOUR);//�������ļ�����ת���µ�ַ,���:ԭ���ò�������RAM��ָ��
	#endif
	}
	
#if (USER/100)==0//���Ժ����
	#if ((Project/100)==2)//������
//���ն˵�ַ���õ���
	i=MRR(ADDR_4001+7,1);
	i&=0x0f;
	//�ɼ���������3
	x=MRR(ADDR_6000+(3*LENper_6000)+55,1);
	x&=0xf0;
	x|=i;
	MWR(x,ADDR_6000+(3*LENper_6000)+55,1);
	MWR(x,ADDR_6000+(3*LENper_6000)+15,1);
	//�ɼ���������4
	x=MRR(ADDR_6000+(4*LENper_6000)+55,1);
	x&=0xf0;
	x|=i;
	MWR(x,ADDR_6000+(4*LENper_6000)+55,1);
	MWR(x,ADDR_6000+(4*LENper_6000)+15,1);
	#endif
#endif
	
	ms->ms_free(pSOUR);
	DL698_SETINIT_EVENT(Command,pOAD);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
	DL698_SETINIT_601E(Command);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��

#if (USER/100)==17//���հ�
	Init_6000_2_1_Addr();//�������1ͨ�ŵ�ַΪADDR_4001��ַ(�̶���Ϊ����ʱ��)
#endif
	if(Command==7)
	{
		WWDT_RESET();//����ʹ�ܿ��Ź���λ
	}
}

void Init_6000_2_1_Addr(void)//�������1ͨ�ŵ�ַΪADDR_4001��ַ(�̶���Ϊ����ʱ��)
{
	if(sizeof(InitData_6000_2_1)>LEN_DATABUFF)
	{
		return;
	}
	MR(ADDR_DATABUFF,(u32)InitData_6000_2_1,sizeof(InitData_6000_2_1));
	//ȡ�ն˵�ַ
	MR(ADDR_DATABUFF+10,ADDR_4001+2,6);
	//
	MW(ADDR_DATABUFF,ADDR_6000+LENper_6000,sizeof(InitData_6000_2_1));
}
 





