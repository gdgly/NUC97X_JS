
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/TEST/Test.h"

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
#include "../Device/IC_BUZZ.h"
#include "../MS/Data_Save.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../NUC97x/NUC97x_AIC.h"



u32 Test_ALIGN(void);//���ݶ������;����:0=��ȷ,1=����
u32 Test_POWER(u32 ADDR_BASE);//��Դ����
u32 Test_STM32F4xx(u32 ADDR_BASE);
u32 Test_SDRAM(u32 ADDR_BASE);
u32 Test_GDMA(u32 ADDR_BASE);//����GDMA;���:Ҫ���ԵĿ��ַ;����:0=��ȷ,1=����
u32 Test_NAND_ECC(u32 ADDR_BLOCK);//����NAND_ECC;���:Ҫ���ԵĿ��ַ;����:0=��ȷ,1=����
u32 Test_YAFFS(void);//����YAFFS�ļ�ϵͳ;���أ�0=ok,1=fail
u32 Test_90E36(u32 ADDR_BASE);
u32 Test_ATT7022(u32 ADDR_BASE);
u32 Test_RTC(u32 ADDR_BASE);
u32 Test_IP101GRI(u32 ADDR_BASE);
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

void LED_TestResult(u32 TestResult)//���Խ��EDָʾ(��2�ͼ�������ָʾ)
{
#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
	u32 i;
	
	//Terminal�ļ���Ч�����ж��й�LED��
	FileDownload->File&=~0x8;//�ļ���־(��λ��ʾOK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=...B31=
	if(TestResult==0)
	{//OK
		i=			0|//B3-B0:���ù���ѡ��AF(0-15)
			 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
			 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
			 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
			 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
			(0<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
	}
	else
	{//ERR
		i=			0|//B3-B0:���ù���ѡ��AF(0-15)
			 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
			 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
			 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
			 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
			(1<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
	}
	Pin_Configure(PIN_ATTR_LED,i);//��������
	Pin_Configure(PIN_GPRSRX_LED,i);//��������
	Pin_Configure(PIN_RS485T1_LED,i);//��������
	Pin_Configure(PIN_LED_ONLINE,i);//��������
	Pin_Configure(PIN_WEAK_SINGLE,i);//��������
	Pin_Configure(PIN_STRONG_SINGLE,i);//��������
	Pin_Configure(PIN_RS485R1_LED,i);//��������
	Pin_Configure(PIN_GPRSTX_LED,i);//��������
#endif
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
//	Disable_os();//��ͣos
//	__disable_irq();//���ж�
	
	DisplayClr();//��ȫ��
	OnBACKLIGHT();//��BACKLIGHT��Դ
	OnLED3V3();//��LED3V3��Դ
	OnA5V0();//��A5V0��Դ
	OnEMU();//��EMU(��λ���)
	OnE5V0();//��E5V0��Դ
	OnR5V0();//��R5V0��Դ
	#ifdef PIN_PLC_PWR
	Pin_Configure(PIN_PLC_PWR,//��λ�������0
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	#endif
	
	n=0;//���¶����ֵĲ��Ժ�:0=����(-5<T<45),1=����(T<-5),2=���£�T>55)
	Comm_Ram->TEMP=0x0250;//2BYTE BCD 1λС��
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
	
//��ʾ�¶�
	DisplayString(0,0,0,(u8 *)"Temperature");//��ʾ�ַ���
	p16=(u16 *)(ADDR_STRINGBUFF+4+(12*2));
	i=Comm_Ram->TEMP;
	p16=DisplayData_OffZero(i,4,1,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MRWBUFF
	//DispalyStringContinue(p16,(u8*)"��");//��ʾ�ַ�������
	AutoDisplayRowCol->Row=1;
	
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	WWDT_Disable();//��ֹ���Ź� 
	

	LED_TestResult(TestResult);//���Խ��EDָʾ(��2�ͼ�������ָʾ)
//DATA_ALIGN
	DisplayStringAutoUp(0,9,(u8*)"\x0""DATA_ALIGN");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	i=Test_ALIGN();//���ݶ������;����:0=��ȷ,1=����
	TestResult|=i;
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
//STM32F4xx
//	TestResult|=Test_STM32F4xx(ADDR_BASE);

	TestResult|=Test_GDMA(ADDR_BASE);//����GDMA;���:Ҫ���ԵĿ��ַ;����:0=��ȷ,1=����
//NAND
	TestResult|=Test_NAND_ECC(ADDR_BASE);//����NAND_ECC;���:Ҫ���ԵĿ��ַ;����:0=��ȷ,1=����
//YAFFS
	TestResult|=Test_YAFFS();//����YAFFS�ļ�ϵͳ;���أ�0=ok,1=fail

//INOUT
	TestResult|=Test_INOUT(ADDR_BASE);//�����������
//RTC
	TestResult|=Test_RTC(ADDR_BASE);

//ESAM
#if IC_TESAM==1
	TestResult|=Test_ESAM(ADDR_BASE);
#endif
//RS485,����,RS232����
	TestResult|=Test_UART(ADDR_BASE);

//POWER
	TestResult|=Test_POWER(ADDR_BASE);//��Դ����
//����
#if ((Project/100)!=5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
	#if IC_90E36==1
	//90E36
		TestResult|=Test_90E36(ADDR_BASE);
	#endif
	#if IC_ATT7022==1
	//ATT7022
		TestResult|=Test_ATT7022(ADDR_BASE);
	#endif
#endif

//ETH,IP101
#if ((Project/100)!=5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
	TestResult|=Test_ETH(ADDR_BASE);//��̫������
#endif

//GPRS,MODULE
	TestResult|=Test_GPRS_MODULE(ADDR_BASE);//GPRSģ��,�ز�ģ������ģ��


////24AA02E48
//	TestResult|=Test_24AA02E48(ADDR_BASE);

//ISRAM(56K)
	DisplayStringAutoUp(0,9,(u8*)"\x0""ISRAM");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	i=RAM_Test(0x3c000000,56*1024);//���ݶ������;����:0=��ȷ,1=����
	TestResult|=i;
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
//DDR2(�Զ�64M��32M)
	DisplayStringAutoUp(0,9,(u8*)"\x0""DDR2");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	i=Test_SDRAM(ADDR_BASE);//���ݶ������;����:0=��ȷ,1=����
	TestResult|=i;
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	MC(i,ADDR_BASE+OFFSET_SDRAM_Characteristics,1);
	
  //�ָ�����ģʽ
	MWR(31*24*60,ADDR_MeterFMTimer+14,2);//31*24*60�ֶ�ʱ
	MWR(31*24*60,ADDR_TerminalFMTimer+14,2);//31*24*60�ֶ�ʱ
#if NAND128kBuff==0//����128k nandд����
	Check_4kNandBuff();//���4K(BKPSRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
#else
	Check_128kNandBuff();//���128K(BKPSDRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
#endif
//KEY_LED_BUZZER
#if ((Project/100)!=5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
	TestResult|=Test_KEY_LED_BUZZER(ADDR_BASE);
#endif
	
//�����ܽ���
	if(TestResult)
	{//���ϸ�
		MWR(1,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
	else
	{
	#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
		//�ϸ�
		MWR(0,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	#else
		//�����������ϸ�
		MWR(2,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	#endif
	}
	
//��������
	__enable_irq();//�����ж�
	MC(0x55,ADDR_BASE+OFFSET_POWER_DOWN_Characteristics,2);//0=OK,1=ERR,0xff=û����,0x55=���в���

	
#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
	//ɾ��TEST����
	MC(0,ADDR_SPIFLASH_START+ADDR_Menu_ENTRY,512);
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
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
		}
		if(TestResult==0)
		{
			SoftDelayMS(500);//���ms��ʱ
			LED_TestResult(1);//���Խ��EDָʾ(��2�ͼ�������ָʾ)
			SoftDelayMS(500);//���ms��ʱ
			LED_TestResult(0);//���Խ��EDָʾ(��2�ͼ�������ָʾ)
		}
		else
		{
			LED_TestResult(TestResult);//���Խ��EDָʾ(��2�ͼ�������ָʾ)
		}
	}
#else
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
		if(SuperCap_Voltage()>0x300)//�������ݵ�ص�ѹADC����;����:BCD��ʽ2λС��
		{//�г������ݲ�����
			SoftDelayMS(1000);//���ms��ʱ
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
		}
	}
#endif
	
}






