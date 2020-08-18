
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




u32 Test_POWER(u32 ADDR_BASE)//��Դ����
{
#if MainProtocol==CSG
	return 0;
#else
	u32 i;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	u32 ReNUM;
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	DisplayStringAutoUp(0,9,(u8*)"\x0""VDD5V0");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
//VDD5V0
	Pin_Configure(PIN_ADC123_IN13,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (3<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(13,3,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
		if((i<0x480)&&(i>0x460))
		{
			i=0x480;
		}
		if((i>0x500)&&(i<0x510))
		{
			i=0x500;
		}
		if((i>=0x480)&&(i<=0x500))
		{
			break;
		}
		SoftDelayMS(100);//���ms��ʱ
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics,2);
	if((i<0x480)||(i>0x500))
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
//ADD5V0

	
//IO3V3
	
	
//RTC_BATTERY3V0
	DisplayStringAutoUp(0,9,(u8*)"\x0""RTC3V0");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	Pin_Configure(PIN_URTC,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (3<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(8,2,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
		if((i>=0x310)&&(i<=0x330))
		{
			break;
		}
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics+6,2);
	if((i<0x310)||(i>0x340))
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
//CHARGE_BATTERY4V8
	
	
//GPRS4V0
	OnR5V0();//��R5V0��Դ
	DisplayStringAutoUp(0,9,(u8*)"\x0""GPRS4V0");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	Pin_Configure(PIN_UGPRS,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (3<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	SoftDelayMS(100);//���ms��ʱ
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(12,2,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
		if((i>=0x390)&&(i<=0x410))
		{
			break;
		}
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics+10,2);
	if((i<0x390)||(i>0x410))
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
//ETH3V3
	OnE5V0();//��E5V0��Դ
	DisplayStringAutoUp(0,9,(u8*)"\x0""ETH3V3");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	Pin_Configure(PIN_ADC12_IN6,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (3<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	SoftDelayMS(100);//���ms��ʱ
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(6,2,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
		if((i>=0x320)&&(i<=0x340))
		{
			break;
		}
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics+12,2);
	if((i<0x320)||(i>0x340))
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	
	return TestResult;
#endif
}




















