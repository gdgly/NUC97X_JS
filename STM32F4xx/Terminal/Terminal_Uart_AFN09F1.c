
#include "Project.h"





const u8 ADDR_AFN09F1[]=//�ն˰汾��Ϣ
{
#if FactoryDI==0//0=��̩,1=
	'C','H','N','T',//���̴���	ASCII	4
#endif
#if FactoryDI==1//0=��̩,1=
	'C','H','N','T',//���̴���	ASCII	4
#endif
	'1','2','3','4','5','6','7','8',//�豸���	ASCII	8
	
 #if (USER/100)==5//�Ϻ�
 	'Z','Z',((SVER>>4)&0xf)+0x30,((SVER>>0)&0xf)+0x30,//�ն������汾��	ASCII	4
 #else
	#if (USER/100)==12//�Ĵ�
		'1','6','C','4',//�ն������汾��	ASCII	4
	#else
		//((SV>>8)&0xf)+0x30,'.',((SV>>4)&0xf)+0x30,(SV&0xf)+0x30,//�ն������汾��	ASCII	4
		'V',((SVER>>8)&0xf)+0x30,'.',((SVER>>4)&0xf)+0x30,//�ն������汾��	ASCII	4
	#endif
 #endif
	SVDATE&0xff,(SVDATE>>8)&0xff,(SVDATE>>16)&0xff,//�ն������������ڣ�������	����¼A.20	3
	
#if NAND_1G==1//0=û,1=��
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','1','2','8','M',' ',' ',' ','R','8','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','1','2','8','M',' ',' ','R','1','6','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','1','2','8','M',' ',' ','R','3','2','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','1','2','8','M',' ',' ','R','6','4','M',//�ն�����������Ϣ��	ASCII	11
	#endif
#endif
#if NAND_2G==1//0=û,1=��
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','2','5','6','M',' ',' ',' ','R','8','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','2','5','6','M',' ',' ','R','1','6','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','2','5','6','M',' ',' ','R','3','2','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','2','5','6','M',' ',' ','R','6','4','M',//�ն�����������Ϣ��	ASCII	11
	#endif
#endif
#if NAND_4G==1//0=û,1=��
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','5','1','2','M',' ',' ',' ','R','8','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','5','1','2','M',' ',' ','R','1','6','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','5','1','2','M',' ',' ','R','3','2','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','5','1','2','M',' ',' ','R','6','4','M',//�ն�����������Ϣ��	ASCII	11
	#endif
#endif
#if NAND_8G==1//0=û,1=��
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','1','0','2','4','M',' ',' ','R','8','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','1','0','2','4','M',' ','R','1','6','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','1','0','2','4','M',' ','R','3','2','M',//�ն�����������Ϣ��	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','1','0','2','4','M',' ','R','6','4','M',//�ն�����������Ϣ��	ASCII	11
	#endif
#endif

#if USER<200//�û���ʶ
	'1','3','0','1',//�ն�ͨ��Э��.�汾��	ASCII	4
#else
	#if (USER/100)==2//ɽ��ϵ
	'1','3','7','6',//�ն�ͨ��Э��.�汾��	ASCII	4
	#endif
	#if (USER/100)==3//����ϵ
	'1','3','7','6',//�ն�ͨ��Э��.�汾��	ASCII	4
	#endif
	#if (USER/100)==4//����ϵ
	'1','3','7','6',//�ն�ͨ��Э��.�汾��	ASCII	4
	#endif
	#if (USER/100)==5//�Ϻ�ϵ
	'1','3','7','6',//�ն�ͨ��Э��.�汾��	ASCII	4
	#endif
	#if (USER/100)>=6//����ϵ
	'1','3','7','6',//�ն�ͨ��Э��.�汾��	ASCII	4
	#endif
#endif
	
	//((HVER>>8)&0xf)+0x30,'.',((HVER>>4)&0xf)+0x30,(HVER&0xf)+0x30,//�ն�Ӳ���汾��	ASCII	4
	'V',((HVER>>8)&0xf)+0x30,'.',((HVER>>4)&0xf)+0x30,//�ն�Ӳ���汾��	ASCII	4
	HVDATE&0xff,(HVDATE>>8)&0xff,(HVDATE>>16)&0xff,//�ն�Ӳ���������ڣ�������	����¼A.20	3
};





