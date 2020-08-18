

#include "../H698/Project698.h"
#include "../Terminal698/Terminal_Init.h"
#include "../MS/SetInit.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../MS/MS.h"
#include "Terminal698_Uart_Manage.h"

#include "Terminal_Contrl_Out.h"
//#include "../Device/MEMRW.h"
#include "../Test/Test_RAM.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"

#include "../DL698/RM_TASK.h"
#include "../DL698/DL698_DATALIB.h"
#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/CALC_RESET_FLOW_TIME.h"
#include "../DL698/CALC_impIn.h"
#include "../DL698/CALC_TAG.h"

#include "../DL698/DL698TEST.h"



const u8 InitData_4500_5[]=//��ʼ������.���߹����汾��Ϣ
{
	DataType_structure,
	6,
	DataType_visible_string,
	4,
	'0',' ',' ',' ',//���̴���
	DataType_visible_string,
	4,
	'0',' ',' ',' ',//����汾��
	DataType_visible_string,
	6,
	'0',' ',' ',' ',' ',' ',//����汾����
	DataType_visible_string,
	4,
	'0',' ',' ',' ',//Ӳ���汾��
	DataType_visible_string,
	6,
	'0',' ',' ',' ',' ',' ',//Ӳ���汾����
	DataType_visible_string,
	8,
	'0',' ',' ',' ',' ',' ',' ',' ',//������չ��Ϣ
};


const u8 Init_double_long[]=//��ʼ������double_long
{
	DataType_double_long,
	0,0,0,0,
};


void FLASHtoSDRAM(void)//����FLASHtoSDRAM
{
	
	MR(ADDR_6000_SDRAM,ADDR_6000,LENmax_6000);//�ɼ��������ñ�FLASHtoSDRAM
	MR(ADDR_6012_2_SDRAM,ADDR_6012_2,LENmax_6012_2);//�������ñ�FLASHtoSDRAM
	MR(ADDR_6014_SDRAM,ADDR_6014,LENmax_6014);//��ͨ�ɼ�����FLASHtoSDRAM
	MR(ADDR_6016_SDRAM,ADDR_6016,LENmax_6016);//�¼��ɼ�����FLASHtoSDRAM
	MR(ADDR_601C_SDRAM,ADDR_601C,LENmax_601C);//�ϱ�����FLASHtoSDRAM
	
}



const u8 Init_DATA_6000_TEST[]=//�����òɼ���������
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
	0x05,0x11,0x11,0x11,0x11,0x11,0x11,
	//0x05,0x00,0x00,0x00,0x00,0x00,0x01,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	6,
	//3,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//ABB (5)
//LANDIS (6)
//}��
	DataType_enum,
	3,
//�˿�      OAD��
	DataType_OAD,
	//0xF2,0x08,0x02,0x01, //��������ͨ�ſں�(����)
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
	51,//1,
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


void TEST_ConnectAppOI_LIST(void);//���Զ����б�
void Init_Terminal(void)//������ʼ��
{
	u32 i;
  u32 x;
//	u8* p8;
	u16 *p16;
  u32 * p32;


//IRAM������0(����ʾ��)
	Comm_Ram->ErrorIC|=RAM_Test(ADDR_TerminalDisplay_IRAM_End,ADDR_Terminal_IRAM_End-ADDR_TerminalDisplay_IRAM_End);//RAM����;����:0=��ȷ,1=����
//ERAM������0
	p32=(u32*)ADDR_SDRAM_CLR_START;
	x=ADDR_ERAM_END-ADDR_SDRAM_CLR_START;
	x/=4;
	for(i=0;i<x;i++)
	{
		p32[i]=0;
	}
	Terminal_Ram->S_PR=60;//1 60���Ƶ������
	Comm_Ram->BattWork_S_timer=30;//��ع����ػ��붨ʱ��

//����ֻ���ڵ����е�ʱ����
  if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  {
    return;
  }
	
	MC(0,ADDR_TerminalRealTimer,1);//0=����ʱ��δ����,1=����ʱ���Ѽ���
	Terminal_Ram->SlipCYC_S_Timer=15;//23 �ն��������幦�ʼ�����������廬���붨ʱ��
//���ݳ�ʼ��
	Terminal_Ram->Power_S_Timer=5;//38 ͣ���¼��붨ʱ��
	p16=(u16 *)Get_ADDR_UARTnTimer_S(RS485_1PORT);
	p16[0]=10;//10���ϵ����ʱ����
	p16=(u16 *)Get_ADDR_UARTnTimer_S(RS485_2PORT);
	p16[0]=10;//10���ϵ����ʱ����
	p16=(u16 *)Get_ADDR_UARTnTimer_S(RS485_4PORT);//(�������ز�,ר�����ģ��)
	p16[0]=10;//10���ϵ����ʱ����


//���ų�ʼ��
	Pin_Configure(PIN_BUZZ,
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_GPRS_ID0,//����������
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_GPRS_ID1,//����������
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Init_Contrl_Pin();//��ʼ���ն�������ƺ�LED����

	Pin_Configure(PIN_RS485T1_LED,//����485_1LED
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_RS485R1_LED,//����485_1LED
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_RS485T2_LED,//����485_2LED
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_RS485R2_LED,//����485_2LED
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Terminal_Ram->RS485_1_TxLED_MS_Timer=2000/10;//10 RS485_1������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	Terminal_Ram->RS485_1_RxLED_MS_Timer=2000/10;//11 RS485_1�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	Terminal_Ram->RS485_2_TxLED_MS_Timer=2000/10;//12 RS485_2������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	Terminal_Ram->RS485_2_RxLED_MS_Timer=2000/10;//13 RS485_2�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��

	OnE5V0();//����̫����Դ��ʹLED������
	Pin_Configure(PIN_ENET_LINK_LED,//�ϵ�ʱ���ָʾLED�ʳ�ʼ���0(��)
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_ENET_ACT_LED,//
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	p16[0]=2000/10;//�ϵ�ʱ�ùػ���ʱ2s��ΪLED���Ƽ��
	Terminal_Ram->ENETLED_MS_Timer=2000/10;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��

#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
	Terminal_Ram->ContrlLoopCheck_S_Timer=60;//36 ���������·�����Ч�ȴ��붨ʱ��
#endif

	Comm_Ram->CheckLEDTimer=3000/10;//�ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
//UART��ʼ��
	Terminal_UartInit_Manage();//Terminal_Uart��ʼ������
//


//��DL698
//---TEST START---

//	MC(0,ADDR_6000,16*LENper_6000);
//	MW((u32)Init_DATA_6000_TEST,ADDR_6000+(1*LENper_6000),sizeof(Init_DATA_6000_TEST));

//	MC(0,ADDR_6012_2,16*LENper_6012_2);
//	MC(0,ADDR_6014,16*LENper_6014);
//	MWR(1,ADDR_3110+1,1);
//	Event_OAD(0x31100200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF

//---TEST END---

	FLASHtoSDRAM();//����FLASHtoSDRAM
	MR(ADDR_4500_5,(u32)InitData_4500_5,sizeof(InitData_4500_5));//��ʼ������.���߹����汾��Ϣ
	Init_SwitchIn();//��ʼ���ն˿���������
	RESET_Count();//�ն˸�λ����
#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
	i=MRR(ADDR_8001_4+1,2);
	i=(i>>8)|((i&0xff)<<8);
	if(i)
	{//0��ʾ�ϵ粻�Զ�����
		i*=60;
		if(i>65535)
		{
			i=65535;
		}
		Terminal_Ram->UpPower_Protect_S_Timer=i;//35 �ն��ϵ籣���붨ʱ��
	}
#endif
	
#if DEBUG==1//0=����,1=����
TEST_ConnectAppOI_LIST();//���Զ����б�
#endif
#if RUN_DL698TEST_USER==1//0��Ч,1=����;�û������
DL698TEST_Init_USER();//�û�����Գ�̨��
#endif
	
}


void Init_ALLDATA(void)//�ն�ȫ�����ݳ�ʼ��(��0)
{
	u32 i;
	ms_Type* ms;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
//��ȫ���ļ�
	ms=Get_ms();
	ms->msfile_format();
	ms->Init_my_heap();//��ʼ�����ں�RTOS�޹ص��ҵ��ڴ��
	
//���ݿ����б������
	DL698_DATALIB_INIT();//ȫ�����ݿ����б�����ݳ�ʼ��(��0)
//�¼�ԭʼ����
	MC(0,ADDR_Event_Sour,NUM_RMmax*LEN_Event_Sour);
//�ɼ�������
	for(i=0;i<RMTASKmax;i++)
	{
		RM_Monitor_Init(i);//�ɼ������س�ʼ��;���:ID=�����
	}
//
	Init_SwitchIn();//��ʼ���ն˿���������
//
	Init_impInData();//��0���������ն˵��ܺ���������
//
	Init_Data230x();//��ʼ���ܼ�������
//
#if (USER/100)==15//����
	MC(0,ADDR_2131_All,LENmax_2131_All*31*3);//�洢ÿ���յ�ABC���ѹ�ϸ��ʴ洢������,����һ����31����������
#endif
#if CompareMeterEventCount==1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
	MC(0,ADDR_SourMeterEventCount,LENper_SourMeterEventCount*NUM_RMmax);
#endif
//
	MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*0),7);//RS485-1��ʼ�ѱ��ַ
	MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*1),7);//RS485-2��ʼ�ѱ��ַ
//
	MC(0,ADDR_MeterAutoReport_3011_Count,4);//���ܱ�ͣ�������ϱ��¼���¼��ţ�˳�����ɣ�
//
	MC(0,ADDR_EventCurrentvalue_3003,LENper_EventCurrentvalue*NUM_EventCurrentvalue);//�¼���ǰֵ��¼��
	
//��0������������Ϣ����������Ϣ������ز�оƬ��Ϣ
	MC(0,ADDR_Read_Info_Time,7);
	MC(0,ADDR_AFN10F21,2);
	MC(0,ADDR_AFN10F111,2);
	MC(0,ADDR_AFN10F112,2);
	
	
//RS485-1�ڳ��������ʼ��
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl->TaskID=0;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	MC(0,(u32)&UARTCtrl->TaskTime,LEN_UART2Ctrl-1024);
//RS485-2�ڳ��������ʼ��
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	UARTCtrl->TaskID=0;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	MC(0,(u32)&UARTCtrl->TaskTime,LEN_UART3Ctrl-1024);
#if ((Project/100)==2)||((Project/100)==4)//������
//�ز��ڳ��������ʼ��
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	UARTCtrl->TaskID=0;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	MC(0,(u32)&UARTCtrl->TaskTime,LEN_UART4Ctrl-1024);
	Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
#endif
	
	
//�ڲ�����������0
	if(Comm_Ram->Factory==0x55)
	{//�ǹ���ģʽ
		//��������ģʽ��ʱ
		i=MRR(ADDR_MeterFMTimer+14,2);//����ģʽ��ʱ
    //��SaveRam
    MC(0,ADDR_Meter_SaveRam_Start,LEN_Meter_SaveRam*2);
		
		//�ָ�����ģʽ��ʱ
		MWR(i,ADDR_MeterFMTimer+14,2);//����ģʽ��ʱ
    //��EFLASH
    MC(0,ADDR_MeterEnergy_EFLASH_Start,(ADDR_MeterClr_End)-(ADDR_MeterEnergy_EFLASH_Start));
		MC(0,ADDR_Meter_EFLASH_Ext_Start,(ADDR_Meter_EFLASH_Ext_End)-(ADDR_Meter_EFLASH_Ext_Start));//�Ǳ������չ
//		//�������(�����0ʱ�Զ�����128K�����)
//		i=(ADDR_MeterCurve_End)-(ADDR_MeterCurve_Start);
//		MC(0xff,ADDR_MeterCurve_Start,i);//NAND_FLASH �����
		//������ʼ��
		
	}
//�ն˶���ʱ��
	//�ն˶���ʱ�䲻����0,�����ն���������ʼ����λ����Ϊ1����������Ϊ�¸��½��¸�λ������0�������λ����С1��
	//MC(0,ADDR_TerminalCongeal_Time,7);//�ϴζ���ʱ��
	
	

}




void DL698_INIT(u32 Command)//��ʼ��,���Command:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
{
	switch(Command)
	{
		case 0:
			return;
		case 1://Ӳ����ʼ��
			return;
		case 2://��������ʼ��
			Init_ALLDATA();//�ն�ȫ�����ݳ�ʼ��(��0)
			return;
		case 3://������ȫ����������ʼ��(���ָ�����������)
		case 4://4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��
		case 5://4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��
			Init_ALLDATA();//�ն�ȫ�����ݳ�ʼ��(��0)
			DL698_SETINIT(Command,0);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
			return;
		case 6://�����ݲ�����ʼ��
			DL698_SETINIT(Command,0);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
			return;
		case 7://7=������ʼ��(������nand�ļ�����ת�Ƶ��µ�ַ)
			DL698_SETINIT(Command,0);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
			return;
	}
}


#include "../Device/ATNo.h"
void Init_Terminal_SoftwareUpdate(void)//������³�ʼ��
{
	//���ܵ���ͬAT�����AT�ű仯������
	switch(UART1Ctrl->Task)
	{
		case 3://��֡�������ڷ���
		case 6://��֡�������ڷ���
			UART1Ctrl->ATCount=AT_MYNETWRITE;//��������
			break;
	}

	DL698_INIT(Terminal_UpDataInit);//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ

	//MS��BOOT���£�������ѡ��NAND128kBuff��NAND4kBuff����ͬ,���ݱ����ָ����ܴ���,���ڴ��Ƚ�����д��FLASH
#if NAND128kBuff==0//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
	NAND4kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
#else
	NAND128kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
#endif
}




