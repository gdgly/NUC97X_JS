
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif


__align(4) const u32 ADDR_Terminal_VER[]=//�汾��Ϣ
{
//VER
	EnProject+(EnHV<<1)+(EnUSER<<2)+(EnSV<<3)+(EnMeter<<4)+(EnMainProtocol<<5),//�������ʱ�Ƿ��б�
	Project,//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=,500=����������,600=,700=
	HVER,//Ӳ���汾��
	USER,//�û���ʶ
	SVER,//����汾��
#if SAME_Terminal==0//0=��,1=��;�����ͬʱ���±�־
	0,
#else
	SAME_MS+(SAME_OS<<1)+(SAME_Meter<<2)+(SAME_Terminal<<3)+(SAME_Menu<<4)+(SAME_Font<<5),//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
#endif
	DEBUG,// 0=����,1=����
	MeterCtrl+(MeterType<<8)+(MeterSpec<<16),//���ܱ������߷�ʽ�ȵ��ܱ���
	3,//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
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


	
	
	
	







