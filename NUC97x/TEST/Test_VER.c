
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif


const u32 ADDR_Test_VER[]=//�汾��Ϣ
{
//VER
	EnProject+(EnHV<<1)+(EnUSER<<2)+(EnSV<<3)+(EnMeter<<4)+(EnMainProtocol<<5),//�������ʱ�Ƿ��б�
	Project,//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=,500=����������,600=,700=
	HVER,//Ӳ���汾��
	USER,//�û���ʶ
	SVER,//����汾��
	0,//�����ͬʱ���±�־
	DEBUG,// 0=����,1=����
	MeterCtrl+(MeterType<<8)+(MeterSpec<<16),//���ܱ������߷�ʽ�ȵ��ܱ���
	8,//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Test,5=Font,6=PACK,7=Router,8=TEST
	0+(UpDataReset<<8),//��0�ֽ�//KEILֱ��д��=0,��PC�н����滻������=1
		//��1�ֽ�//������º�����:0=��,1=��(ע:��OS������ǿ������)
		//��2�ֽ�
		//��3�ֽ�
//RAM
	ADDR_Comm_Ram_Start,//0
	ADDR_Meter_SaveRam_Start,//1
	ADDR_Meter_Ram_Start,//2
	ADDR_Terminal_SaveRam_Start,//3
	ADDR_Terminal_IRAM_Start,//4
	ADDR_DATABUFF,//5
	ADDR_4KWRITE_BUFF,//6
	0,//7
//ERAM
	ADDR_ERAM_START,//0
	ADDR_UARTnCtrl_End,//1
	0,
	0,
	0,
	0,
	0,
	0,
//EFLASH
	ADDR_DISK_Start,//0
	ADDR_Com_EFLASH_Start,//1
	ADDR_Meter_EFLASH_Start,//2
	ADDR_Terminal_EFLASH_Start,//3
	0,
	0,
	0,
	0,
//
};


	
	
	
	







