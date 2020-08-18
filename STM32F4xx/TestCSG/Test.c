
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../TEST/Test.h"
#include "../Device/MEMRW.h"
#include "../Device/NAND.h"
#include "../Display/Display.h"
#include "../Test/Test_RAM.h"
#include "../TEST/TestReport.h"
#include "../STM32F4xx/STM32F4xx_RCC_Config.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../OS/OS.h"
#include "stm32f4xx_hal.h"
#include "../Device/IC_BUZZ.h"
#include "../MS/Data_Save.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


uint32_t SystemCoreClock;
u32 Test_POWER(u32 ADDR_BASE);//��Դ����
u32 Test_STM32F4xx(u32 ADDR_BASE);
u32 Test_SDRAM(u32 ADDR_BASE);
u32 Test_MT29FxG_NAND(u32 ADDR_BASE);
u32 Test_S34MLxG_NAND(u32 ADDR_BASE);
u32 Test_90E36(u32 ADDR_BASE);
u32 Test_RTC(u32 ADDR_BASE);
u32 Test_LAN8720A(u32 ADDR_BASE);
u32 Test_UART(u32 ADDR_BASE);
u32 Test_ETH(u32 ADDR_BASE);//��̫������
u32 Test_GPRS(u32 ADDR_BASE);//GPRSģ��
u32 Test_GPRS_MODULE(u32 ADDR_BASE);//GPRSģ��,�ز�ģ������ģ��
u32 Test_LOCAL_MODULE(u32 ADDR_BASE);//�����ز�ģ��
u32 Test_CONTRL_MODULE(u32 ADDR_BASE);//ר�����ģ��
u32 Test_ESAM(u32 ADDR_BASE);
u32 Test_24AA02E48(u32 ADDR_BASE);
u32 Test_INOUT(u32 ADDR_BASE);//�����������
u32 Test_KEY_LED_BUZZER(u32 ADDR_BASE);



void Test_ReStartOS(void)//�������ܲ�������OSǰ(��OS����ԭ���ǵ���̫��Ϊ�Զ�IPʱ�����Զ��õ�IPʱ����ʧ��!!!)
{
	Comm_Ram->Init_OS|=1<<7;//OS������ԭ���־:B0=�������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
	ReSTART_OS();//����OS
	while(1)
	{
	}
}


void Test(void)//�������ܲ���
{
	u32 i;
	u32 n;
	u32 ADDR_BASE;
	u16 *p16;
	u32 *p32;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	p32=p32;
	//����nand��������
#if NAND128kBuff==0//����128k nandд����
	NAND4kBuff_Write_NAND(0,0,0);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
#else
	NAND128kBuff_Write_NAND(0,0,0);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
#endif
	Disable_os();//��ͣos
	__disable_irq();//���ж�
	
	RCC_SystemClock_Config_168MHz();
	DisplayClr();//��ȫ��
	OnBACKLIGHT();//��BACKLIGHT��Դ
	OnLED3V3();//��LED3V3��Դ
	OnA5V0();//��A5V0��Դ
	OnE5V0();//��E5V0��Դ
	OnR5V0();//��R5V0��Դ
	
	n=0;//���¶����ֵĲ��Ժ�:0=����(-5<T<45),1=����(T<-5),2=���£�T>55)
	i=Comm_Ram->TEMP;//2BYTE BCD 1λС��
	if(i&0x8000)
	{//��
		if(i>0x8050)
		{
			n=1;
		}
	}
	else
	{//��
		if(i>0x0550)
		{
			n=2;
		}
	}
	MWR(n,ADDR_Characteristics_NO,1);
	ADDR_BASE=ADDR_Characteristics_BASE1+(n*LEN_BASE_Characteristics);
	MWR(i,ADDR_BASE+(OFFSET_Test_Temperature),2);
//��ȫ����������(������������)
	MC(0xff,ADDR_BASE+(OFFSET_POWER_Characteristics),(LEN_BASE_Characteristics)-(OFFSET_POWER_Characteristics));
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
//��ʾ�¶�
	DisplayString(0,0,0,(u8 *)"Temperature");//��ʾ�ַ���
	p16=(u16 *)(ADDR_STRINGBUFF+4+(12*2));
	i=Comm_Ram->TEMP;
	p16=DisplayData_OffZero(i,4,1,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MRWBUFF
	//DispalyStringContinue(p16,(u8*)"��");//��ʾ�ַ�������
	AutoDisplayRowCol->Row=1;
	

//POWER
	SoftDelayMS(500);//���ms��ʱ
	TestResult|=Test_POWER(ADDR_BASE);//��Դ����
//STM32F4xx
	TestResult|=Test_STM32F4xx(ADDR_BASE);
//nand
#if IC_MT29FxG==1
	TestResult|=Test_MT29FxG_NAND(ADDR_BASE);
#endif
#if IC_S34MLxG==1
	TestResult|=Test_S34MLxG_NAND(ADDR_BASE);
#endif
//90E36
	TestResult|=Test_90E36(ADDR_BASE);
//RTC
	TestResult|=Test_RTC(ADDR_BASE);
//LAN8720A
	TestResult|=Test_LAN8720A(ADDR_BASE);
//ETH
	TestResult|=Test_ETH(ADDR_BASE);//��̫������
/*
//GPRS
	TestResult|=Test_GPRS(ADDR_BASE);//GPRSģ��
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//LOCAL MODULE
	TestResult|=Test_LOCAL_MODULE(ADDR_BASE);//�����ز�ģ��
#endif
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//CONTRL MODULE
	TestResult|=Test_CONTRL_MODULE(ADDR_BASE);//ר�����ģ��
#endif
*/
//GPRS,MODULE
	TestResult|=Test_GPRS_MODULE(ADDR_BASE);//GPRSģ��,�ز�ģ������ģ��
//����UART���� ��GPRS,MODULE����ʹ���˸�λDMA1,DMA2
	TestResult|=Test_UART(ADDR_BASE);
//ESAM
	TestResult|=Test_ESAM(ADDR_BASE);
//24AA02E48
	TestResult|=Test_24AA02E48(ADDR_BASE);
//INOUT
	TestResult|=Test_INOUT(ADDR_BASE);//�����������
//sdram
	TestResult|=Test_SDRAM(ADDR_BASE);
	
  //�ָ�����ģʽ
	MWR(31*24*60,ADDR_MeterFMTimer+14,2);//31*24*60�ֶ�ʱ
	MWR(31*24*60,ADDR_TerminalFMTimer+14,2);//31*24*60�ֶ�ʱ
#if NAND128kBuff==0//����128k nandд����
	Check_4kNandBuff();//���4K(BKPSRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
#else
	Check_128kNandBuff();//���128K(BKPSDRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
#endif
	
//KEY_LED_BUZZER
	TestResult|=Test_KEY_LED_BUZZER(ADDR_BASE);
	
//�����ܽ���
	MWR(TestResult,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
//��������
	__enable_irq();//�����ж�
	//��4K(BKPSRAM)���б��ֲ���
	p32=(u32*)(ADDR_BKPSRAM);
	for(i=0;i<(4096/4);i++)
	{
		p32[i]=i;
	}
	MC(0x55,ADDR_BASE+OFFSET_POWER_DOWN_Characteristics,2);//0=OK,1=ERR,0xff=û����,0x55=���в���
	
	Charge_Battery_Voltage();//�ɳ��4.8V��ص�ѹADC����
	i=MRR(ADDR_UBACKUP,2);
	if(i)
	{//�г���ش���
		DisplayStringAutoUp(0,9,(u8*)"\x0""Load battery ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		while(1)
		{
			WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
			Charge_Battery_Voltage();//�ɳ��4.8V��ص�ѹADC����
			i=MRR(ADDR_UBACKUP,2);
			if(i==0)
			{//�����=0
				break;
			}
		}
	}
	BUZZ_Generate(12);//��������,��ڷ�����
	DisplayStringAutoUp(0,9,(u8*)"\x0""Turn off the power  to keep 30s above");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	//�ȴ�����
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
		{
		//���ݱ���
			if(Comm_Ram->DataSave==1)//���ݱ���:0=��,1=�����е糬��10�������豣��,2=�����ѱ���
			{
				Comm_Ram->DataSave=2;//���ݱ���:0=��,1=�����е糬��10�������豣��,2=�����ѱ���
				PowerDown_Data_Save();//��Դ�������ݱ���
			}
		}
	}
}






