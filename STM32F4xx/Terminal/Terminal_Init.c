

#include "Project.h"
#include "../Terminal/Terminal_Init.h"
#include "../MS/SetInit.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "Terminal_Uart_Manage.h"
#include "Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_CongealData.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"
#include "Terminal_impIn.h"
#include "Terminal_Contrl_Manage.h"
#include "Terminal_Contrl_Out.h"
#include "AutoSearchMeter.h"
#include "../Device/MEMRW.h"
#include "../Test/Test_RAM.h"

void Init_Terminal(void)//������ʼ��
{
	u32 i;
  u32 x;
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
		p32[0]=0;
	}
	Terminal_Ram->S_PR=60;//1 60���Ƶ������
	Comm_Ram->BattWork_S_timer=30;//��ع����ػ��붨ʱ��

//����ֻ���ڵ����е�ʱ����
  if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  {
    return;
  }
	MC(0,ADDR_TerminalRealTimer,1);//0=����ʱ��δ����,1=����ʱ���Ѽ���
	Terminal_Ram->SlipCYC_S_Timer=15;//60;//23 �ն��������幦�ʼ�����������廬���붨ʱ��
//���ݳ�ʼ��
	Terminal_Ram->Power_S_Timer=5;//38 ͣ���¼��붨ʱ��
#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
	#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	if(MRR(ADDR_AFN04F297_Ext,1)!=0)
	{//�ն˾�ȷ��ʱ���ؿ���
		i=MRR(ADDR_AFN04F297_Ext+9,1);
		i*=60;
		if(i==0)
		{
			i=10;
		}
		Terminal_Ram->RMDelay_S_Timer=i;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
		MC(0,ADDR_ERC14SOUR+22,6);//ERC14�ϵ��¼���ϵ�ʱʱ��:������ʱ����
	}
	else
	{
		Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
	}
	#endif
	#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
	#endif
#else
	Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
#endif
	
	Init_Class1Data_UpPower();//��1���ݳ�ʼ��(�ϵ�)
	Init_Class2Data_UpPower();//��2���ݳ�ʼ��(�ϵ�)
	Init_Class3Data_UpPower();//��3����(�¼����㣩��ʼ��(�ϵ�)
	MC(0xee,ADDR_Average_Voltage,6);//�ڲ�����A,B,C��1����ƽ����ѹ
	#if IC_SDRAM
	MR(ADDR_HOURFLASH_RAM_START,ADDR_HOURFLASH_START,LEN_HOURFLASH_RAM);//�ָ��ն�ÿСʱRAM���ݱ��浽FLASH
	#endif
	PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
	AFN04F150_Computer();//��������Ч��־����
	
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

	Pin_Configure(PIN_ATTR_LED,//�ϵ�ʱ���ָʾLED�ʳ�ʼ���0(��)
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
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

#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	Terminal_Ram->ContrlLoopCheck_S_Timer=60;//36 ���������·�����Ч�ȴ��붨ʱ��
#endif

	Comm_Ram->CheckLEDTimer=3000/10;//�ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
/*
	p32=(u32*)(ADDR_AUTO_ERROR_RESTART);//�Զ���⵽��SDRAM�ȴ����Ź���λ��־
	if(p32[0]==0x12345678)
	{
		Pin_CFG(PIN_ATTR_LED,0+(0<<3)+(1<<5)+(1<<17)+(1<<18));//�ϵ�ʱ���ָʾLED�ʳ�ʼ���0(��)
		Terminal_Ram->RS485_1_TxLED_MS_Timer=0;//10 RS485_1������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		Terminal_Ram->RS485_1_RxLED_MS_Timer=0;//11 RS485_1�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		Terminal_Ram->RS485_2_TxLED_MS_Timer=0;//12 RS485_2������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		Terminal_Ram->RS485_2_RxLED_MS_Timer=0;//13 RS485_2�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		Pin_CFG(PIN_ENET_LINK_LED,0+(0<<3)+(0<<5)+(1<<17)+(1<<18));//�ϵ�ʱ���ָʾLED�ʳ�ʼ���0(��)
		p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
		p16[0]=0;//�ϵ�ʱ�ùػ���ʱ2s��ΪLED���Ƽ��
		Terminal_Ram->ENETLED_MS_Timer=0;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		Comm_Ram->CheckLEDTimer=0;//�ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
	}
*/
//UART��ʼ��
	Terminal_UartInit_Manage();//Terminal_Uart��ʼ������
	i=MRR(ADDR_AFN04F97+2,1);//ͣ���¼�����ʱ����ֵ	BIN	1	��λΪ����
	i*=60;
	p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1ͣ���¼�����ʱ����ֵ�붨ʱ��(�����ϳ���ʱ�����ֿ�)
	p16[0]=i;//RS485-1
	p16[1]=i;//RS485-2
	p16[2]=i;//RS485-3
	p16[3]=i;//RS485-4(�ز�)
	
//�ն˸�λ����
	Terminal_Reset_Count();//�ն˸�λ����

//
#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	MR(ADDR_ERC14SOUR+2,ADDR_TFM_YMDHMS+1,5);//ERC14�ϵ��¼��ͣ��ʱʱ��:������ʱ��
#endif

}

void Init_EnergyInc_Souser(void)//�������������������ԭʼ������Ϊ0xee
{
	u32 i;
//�����й���������
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_PKWH_INC+((1+(9*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
	}
//�����޹���������
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_PKVARH_INC+((1+(8*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
	}
//�����й���������
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_NKWH_INC+((1+(9*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
	}
//�����޹���������
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_NKVARH_INC+((1+(8*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
	}
}
void Init_IncRemainder(void)//�ܼ����ܼӵ���������С��λ��Ϊ0x0
{
//�����ܼ��й�����������С��λ(�ܼ����)
	MC(0x0,ADDR_AFN0CF19_Remainder,(4*(TMaxTariff+1)*AFN0CF19MaxPn));
//�����ܼ��޹�����������С��λ(�ܼ����)
	MC(0x0,ADDR_AFN0CF20_Remainder,(4*(TMaxTariff+1)*AFN0CF20MaxPn));
//�����ܼ��й�����������С��λ(�ܼ����)
	MC(0x0,ADDR_AFN0CF21_Remainder,(4*(TMaxTariff+1)*AFN0CF21MaxPn));
//�����ܼ��޹�����������С��λ(�ܼ����)
	MC(0x0,ADDR_AFN0CF22_Remainder,(4*(TMaxTariff+1)*AFN0CF22MaxPn));
//�ն˵�ǰʣ�����(��)����С��λ(�ܼ����)
	MC(0x0,ADDR_AFN0CF23_Remainder,(4*(TMaxTariff+1)*AFN0CF23MaxPn));
}

void Init_ReadMeter(void)//�����ʼ��
{
  UARTCtrl_TypeDef * UARTCtrl;
	
	Terminal_Ram->ACSampleRM_S_Timer=0;
	Terminal_Ram->RS4851RM_S_Timer=0;
	Terminal_Ram->RS4852RM_S_Timer=0;
	Terminal_Ram->RS4853RM_S_Timer=0;
	if(Get_LEN_UARTnCtrl(ACSAMPLEPORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
		UARTCtrl->MeterNo=0;//���ܱ�/��������װ�����(���ֶ���)
		UARTCtrl->FnCount=0;//Fn����
		UARTCtrl->RMCount=0;//�����������
	}
	if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		UARTCtrl->MeterNo=0;//���ܱ�/��������װ�����(���ֶ���)
		UARTCtrl->FnCount=0;//Fn����
		UARTCtrl->RMCount=0;//�����������
	}
	if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		UARTCtrl->MeterNo=0;//���ܱ�/��������װ�����(���ֶ���)
		UARTCtrl->FnCount=0;//Fn����
		UARTCtrl->RMCount=0;//�����������
	}
	if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
		UARTCtrl->MeterNo=0;//���ܱ�/��������װ�����(���ֶ���)
		UARTCtrl->FnCount=0;//Fn����
		UARTCtrl->RMCount=0;//�����������
	}
}


extern void Terminal_SetInitLib(void);
void Terminal_Init(u32 Command)//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
{
	u32 *p32;
#if POINT_AFN0DF89F90F91!=0//�ܶ������
	u32 i;
	u32 buff;
#endif

	switch(Command)
	{
		case 1://Ӳ����ʼ��
			p32=(u32*)(ADDR_HourCongeal_AutoRW);
			while(p32[3]!=0)
			{//R2����,ֵ=0��ʾ�������
				HourCongeal_AutoRW();//Сʱ�����Զ�IO��д����0,��ɺ�������
				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
			}
			p32=(u32*)(ADDR_Class2Congeal_AutoRW);
			while(p32[3]!=0)
			{//R2����,ֵ=0��ʾ�������
				Class2Congeal_AutoRW();//��2�����Զ�IO��д����0,��ɺ�������
				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
			}
			MC(0x0,ADDR_AFN01Fx,5);//������
			
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
			return;
		case 2://��������ʼ��
			break;
		case 3://������ȫ����������ʼ��(���ָ�����������)
		case 5://������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
			//������ʼ��
			MC(0,ADDR_AFN04F9,ADDR_AFN04_End-ADDR_AFN04F9);
			MC(0,ADDR_AFN05_Start,ADDR_AFN05_End-ADDR_AFN05_Start);
			p32=(u32*)ADDR_Terminal_ENTRY;
			SetInit(p32[118],p32[119],1);//������ʼ��,���ADDR_LIB������ʼ�����ַ,ADDR_LIBEnd�������ַ,Communicateͨ�Ų���0=����ʼ��,1=��ʼ��
			AFN04F150_Computer();//��������Ч��־����
			break;
		case 4://����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��
			//������ʼ��
			MC(0,ADDR_AFN04F9,ADDR_AFN04_End-ADDR_AFN04F9);//�˵�����,���ڻָ����������ò�����ʼ��ʱ��0
			MC(0,ADDR_AFN05_Start,ADDR_AFN05_End-ADDR_AFN05_Start);
			p32=(u32*)ADDR_Terminal_ENTRY;
			SetInit(p32[118],p32[119],0);//������ʼ��,���ADDR_LIB������ʼ�����ַ,ADDR_LIBEnd�������ַ,Communicateͨ�Ų���0=����ʼ��,1=��ʼ��
			AFN04F150_Computer();//��������Ч��־����
			break;
		case 6://6=�����ݲ�����ʼ��
			p32=(u32*)ADDR_Terminal_ENTRY;
			SetInit(p32[116],p32[117],0);//������ʼ��,���ADDR_LIB������ʼ�����ַ,ADDR_LIBEnd�������ַ,Communicateͨ�Ų���0=����ʼ��,1=��ʼ��
			AFN04F150_Computer();//��������Ч��־����
			ERC1_Event(2);//���ݳ�ʼ���Ͱ汾�����¼;f=1���ݳ�ʼ��,f=2�汾���,f=3���ݳ�ʼ��ͬʱ�汾���
			MC(0x0,ADDR_AFN01Fx,5);//������
			return;
		default://��
			return;
	}
	//��������ʼ��
	Init_EnergyInc_Souser();//�������������������ԭʼ������Ϊ0xee
	Init_IncRemainder();//�ܼ����ܼӵ���������С��λ��Ϊ0x0
	Init_Class1Data_Command();//��1���ݳ�ʼ��(����,ȫ�����)
	Init_Class2Data_Command();//��2���ݳ�ʼ��(����,ȫ�����)
	Init_Class3Data_Command();//��3����(�¼�����ʼ��(����,ȫ�����)
	Init_Contrl_Command();//�ն˿������ݳ�ʼ��(����,ȫ�����)
	Init_impInData_Command();//�ն˵��������������ݳ�ʼ��(����,ȫ�����)
	Init_StatisticData_Command();//�ն�ͳ�����ݳ�ʼ��(����,ȫ�����)
	ERC1_Event(1);//���ݳ�ʼ���Ͱ汾�����¼;f=1���ݳ�ʼ��,f=2�汾���,f=3���ݳ�ʼ��ͬʱ�汾���
	Init_ReadMeter();//�����ʼ��
	Init_Class2SourceData_Command();//��2ԭʼ���ݳ�ʼ��(����,ȫ�����Ϊ0xff)
	MC(0x0,ADDR_AFN01Fx,5);//������
	//�ڲ�����1����������0��ʼ��
#if POINT_AFN0DF89F90F91!=0//�ܶ������
	i=ADDR_AFN0DF89F90F91;
	while(1)
	{
		buff=ADDR_MeterCurve_End-i;
		if(buff<4096)
		{
			break;
		}
		if((i&0xffff)==0)
		{//64K����
			if(buff>=65536)
			{
				EFLASH_Block_Erase(i);//�����,���R0=Ҫ�����Ŀ��ڱ����е������ַ(uses AMS-A15 address lines,ÿ��32KWORD)
				i+=65536;
				continue;
			}
		}
		EFLASH_Sector_Erase(i);//��������,���R0=Ҫ�����������ڱ������е������ַ(ÿ����2KWORD)
		i+=4096;
	}
#endif
	
//RAMÿСʱ���ݱ��浽FLASH
#if IC_SDRAM
	MC(0xff,ADDR_HOURFLASH_RAM_START,1);//�ϴ�RAM���浽FLASH��ʵʱ��ʱ(�ϵ���Ϊ0,���¶������Ϊ0xffʹ�������б���)
#endif

	Comm_Ram->RESET=0x55;//0x55=������(����ʹ�ÿ��Ź���λ)
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

	Terminal_Init(Terminal_UpDataInit);//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
	Init_AFN0CF1();//��ʼ���ն���1����F1,�汾��Ϣ//ERC1_Event(2);//���ݳ�ʼ���Ͱ汾�����¼;f=1���ݳ�ʼ��,f=2�汾���,f=3���ݳ�ʼ��ͬʱ�汾���
	if((Terminal_UpDataInit==3)||(Terminal_UpDataInit==4))//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
	{//�����г�ʼ��
		Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
		Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
	#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
		Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
	#endif
	}
	
	if(Comm_Ram->RESET==0x55)//0x55=������(����ʹ�ÿ��Ź���λ)
	{
		if(Terminal_UpDataInit!=5)
		{
			Comm_Ram->RESET=0;//ȡ��0x55=������(����ʹ�ÿ��Ź���λ)
		}
	}
	
	
	//MS��BOOT���£�������ѡ��NAND128kBuff��NAND4kBuff����ͬ,���ݱ����ָ����ܴ���,���ڴ��Ƚ�����д��FLASH
#include "../Device/NAND.h"
#if NAND128kBuff==0//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
	NAND4kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
#else
	NAND128kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
#endif
}


void Terminal_Init_Command(void)//�ն˳�ʼ������
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	
	u8 * p8;
	u8 * p8d;
	
	i=MRR(ADDR_AFN01Fx+4,1);
	if(i!=0x0)
	{
		x=MRR(ADDR_AFN01Fx,4);
		if(x==0x55555555)
		{
			Terminal_Init(i);//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
		}
		else
		{
			if(x>ADDR_ERAM_START)
			{
				if(x<ADDR_ERAM_END)
				{
					p8=(u8 *)(x);
					if(p8[0]==0x0)
					{//ԭ�·�����Ŀ�Task=0(���ط��ͻش������)
						if(Terminal_Ram->ResetCommand_S_Timer==0)//��ʱ10S
						{
	            Terminal_Init(i);//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
						}
					}
				}
			}
		}
	}
  //ɾ��ָ��ͨ�Ŷ˿��µ�ȫ�����
  MR(ADDR_DATABUFF,ADDR_AFN05F53,31);
	p8=(u8 *)(ADDR_DATABUFF);
	for(i=0;i<31;i++)
	{
		if(MRR(ADDR_AFN05F53+i,1)==0x55)
		{
		#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(i==30)
			{//�ز���,·����ͬ��
				Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
				Terminal_Ram->RouterSynDelay_S_Timer=0;//30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
			}
			Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
		#endif	
			MC(0,ADDR_AFN05F53+i,1);
			x=0;//�Ѳ�����
			while((y=(MaxRS485AddCarrierPn-x))!=0x0)
			{
				if(y>151)
				{
					y=151;//4096/LEN_AFN04F10_Pn=151
				}
				MR(ADDR_DATABUFF,ADDR_AFN04F10+(LEN_AFN04F10_Pn*x),LEN_AFN04F10_Pn*y);
				p8d=(u8 *)(ADDR_DATABUFF);
				for(z=0;z<y;z++)
				{
					if((p8d[4]&0x1f)==(i+1))
					{
						p8d[0]=0;
						p8d[1]=0;
						p8d[2]=0;
						p8d[3]=0;
					}
					p8d+=LEN_AFN04F10_Pn;
				}
				MW(ADDR_DATABUFF,ADDR_AFN04F10+(LEN_AFN04F10_Pn*x),LEN_AFN04F10_Pn*y);
				x+=y;
			}
			PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
			AFN04F150_Computer();//��������Ч��־����
		}
	}
	//��λ��������������־
	if(MRR(ADDR_AFN05F150,1)==0x55)
	{
		MC(0,ADDR_AFN04F150+256,256);
		MC(0,ADDR_AFN05F150,1);
		Terminal_Ram->AutoUpdataAFN04F10_S_Timer=0;//25 �Զ�����F10�붨ʱ��;���������յ���ѯ������״̬����(F150)n����(n=0~20,һ��ȡ10)�ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
		Terminal_Ram->AutoUpdataAFN04F10ERC3_S_Timer=0;//26 �Զ�����F10�����ϱ�ERC3�¼��붨ʱ��;������ͨ�����ص��Զ����֣���ɲ����������������ɾ����Ӧ�����ò�������������־��F150��������ӦλΪ1���������ϱ���������¼���ERC3������վ,���ǽ����˲�������������־�ĸ�λ������ÿ���ϱ���������¼����m���ӣ�m=0~30��һ��ȡ10��֮�ڣ����۲����Ƿ�����������ٴ��ϱ�
		Terminal_Ram->AutoUpdataAFN04F10ERC3=0;//0=��,1=��AFN04F10���µ���10���ڲ����ϱ����
	}
	
#if (((Project/100)==2)||((Project/100)==5))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Terminal_Ram->AutoUpdataAFN04F10ERC3==0x55)//0=��,1=��AFN04F10���µ���10���ڲ����ϱ����
	{
		AutoUpdataAFN04F10ERC3();//�Զ�AFN04F10�ĵ��ܱ����õ�ERC3�¼�
	}
#endif
	if(Comm_Ram->TYMDHMS[0]!=Terminal_Ram->AFN04F150_Computer_RTCS)
	{//ÿ�����1��
		Terminal_Ram->AFN04F150_Computer_RTCS=Comm_Ram->TYMDHMS[0];
		AFN04F150_Computer();//��������Ч��־����
	}
}

