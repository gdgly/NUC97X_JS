
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_ADC.h"
#include "../../STM32F4xx/Calculate/Calculate.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/Device/MEMRW.h"


#define VREF    250//ADC�����ѹx.xxV

u32 ADCx(u32 Channel,u32 mul,u32 div,u32 ADDR_RAM)//ADC;���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
{
	u32 i;
	u32 *p32;
	u32 ReNum;
	u8* p8;
	
//clock enabled PCLK
	p32=(u32*)(REG_CLK_HCLKEN);
	p32[0]|=1<<5;
//ADC Clock Enable
	p32=(u32*)(REG_CLK_PCLKEN1);
	p32[0]|=1<<24;
//ADC_CTL
	p32=(u32*)(REG_ADC_CTL);
	p32[0]=
	//[31]~[2]
	//[1]VBG_EN
		//ADC Internal Bandgap Power Control
		//0 = Power down internal bandgap.
		//1 = Power on internal bandgap.
	(1<<1)|
	//[0]
		//AD_ENADC Power Control
		//0 = Power down ADC.
		//1 = Power on ADC.
	1;
	
	i=0;
	ReNum=4;
	while(ReNum--)
	{
	//ADC Configure
		p32=(u32*)(REG_ADC_CONF);
		p32[0]=
		//[31:24]ADCSAMPCNT
			//ADC Sample Counter
			//Set the counter value to extend the ADC start signal period to get more sampling time for precise conversion.
		//[22]HSPEED
			//High Speed Enable
			//Enable ADC to high speed mode.
		//[21]DISZMAVEN
			//Display Z Mean Average Enable
			//Pressure Mean average for Z1 and Z2 function enable.
		//[20]DISTMAVEN
			//Display T Mean Average Enable
			//Touch Mean average for X and Y function enable.
		//[19:16]REFCNT
			//ADC Reference Counter
			//ADC reference voltage stable counter count value.
			//0 = 1 ADC clock.
			//1 = 2 ADC clock.
			//2 = 4 ADC clock.
			//3 = 8 ADC clock.
			//4 = 16 ADC clock.
			//5 = 32 ADC clock.
			//6 = 64 ADC clock.
			//7 = 128 ADC clock.
			//8 = 256 ADC clock.
			//9 = 512 ADC clock.
			//10 = 1024 ADC clock.
			//11 = 1024 ADC clock.
			//12 = 1024 ADC clock.
			//13 = 1024 ADC clock.
			//14 = 1024 ADC clock.
			//15 = 1024 ADC clock.
		//[15:11]Reserved
		//[10]SELFT_EN
			//Selft Test Enable
			//Selft test function enable.
		//[9]KPC_EN
			//Keypad Press Conversion Enable
			//Keypad press conversion function enable.
		//[8]VBAT_EN
			//Voltage Battery Enable
			//Battery voltage detection function enable.	
		//[7:6]REFSEL
			//ADC Reference Select
			//ADC reference voltage select when ADC operate in normal conversion.
			//00 = AGND33 vs 2.5v buffer output, or VREF input.
			//01 = YM vs YP.
			//10 = XM vs XP.
			//11 = AGND33 vs AVDD33.
		//[5:3]CHSEL
			//Channel Selection
			//ADC input channel selection.
			//000 = VBT.
			//001 = VHS.
			//010 = A_2.
			//011 = A_3.
			//100 = YM.
			//101 = YP.
			//110 = XM.
			//111 = XP.
		(Channel<<3)|
		//[2]NACEN
			//Normal AD Conversion Enable
			//ADC normal conversion function enable
		(1<<2)|
		//[1]Z_EN
			//Press Enable
			//Press measure function enable
		//[0]T_EN
			//Touch Enable
			//Touch detection function enable
		0;
	//ADC_ISR
		p32=(u32*)(REG_ADC_ISR);
		p32[0]=0xffffffff;//write 1 to clear this bit.
	//Menu Start Conversion
		p32=(u32*)(REG_ADC_CTL);
		p32[0]|=1<<8;
	//wait
		p32=(u32*)(REG_ADC_ISR);
		while((p32[0]&(1<<0))==0);// wait Normal AD Conversion Finish
		p32=(u32*)(REG_ADC_DATA);
		i+=p32[0]&0xfff;
	}
//�ر�ADC
//	p32=(u32*)(REG_ADC_CTL);
//	p32[0]=0;
//����
	if(Channel==0)
	{//VBT
		i*=100;
		i/=25;
	}
	i*=mul;
	i*=VREF;
	i/=0x1000*4;
	i/=div;
	i=hex_bcd(i);
	if(ADDR_RAM!=0)
	{
		p8=(u8 *)(ADDR_RAM);
		p8[0]=i;
		p8[1]=i>>8;
	}
	return i;
}


void RTC_Battery_Voltage(void)//RTC��ص�ѹADC����
{
	u32 i;
	i=ADCx(7,1+1,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
	Comm_Ram->URTC=i;
}

void Charge_Battery_Voltage(void)//�ɳ��4.8V��ص�ѹADC����
{
	u32 i;
	i=ADCx(6,5,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
	if(i>=0x600)
	{//û���Ͽɳ����ʱ��ѹֵΪ����ѹֵ
		i=0;
		Comm_Ram->ChargeBatteryTimer=2000/10;//30 �޳���ؽ���ʱ>6V���е�ؽ��붨ʱ,��Դ�½����г�����������Ϊ������ص�ѹ,��ʱһ��ʱ����ݷŵ��ADCֵ����Ϊ��ѹֵ
	}
	else
	{
		if(Comm_Ram->ChargeBatteryTimer!=0)
		{
			i=0;//��ʱû��������0V
		}
	}
	Comm_Ram->UBACKUP=i;
}

void GPRS_Voltage(void)//GPRS��Դ��ѹADC����
{
	u32 i;
	i=ADCx(4,1+1,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
	i=0x400;
	Comm_Ram->UGPRS=i;
}

void VDD5V0_Voltage(void)//VDD5V0��ѹADC����
{
	u32 i;
	i=ADCx(1,1+1,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
	Comm_Ram->VDD5V0=i;
}

u32 SuperCap_Voltage(void)//�������ݵ�ѹADC����
{
	u32 i;
	i=ADCx(5,2+1,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
	return i;
}

void STM32F4xx_Temperature(void)//STM32F4xx�����ı����¶�
{
}

/*
void ADCSelftTest(void)//ADC�Բ���
{
	u32 i;
	u32 *p32;
	u32 ReNum;
	
//clock enabled PCLK
	p32=(u32*)(REG_CLK_HCLKEN);
	p32[0]|=1<<5;
//ADC Clock Enable
	p32=(u32*)(REG_CLK_PCLKEN1);
	p32[0]|=1<<24;
//ADC_CTL
	p32=(u32*)(REG_ADC_CTL);
	p32[0]=
	//[31]~[2]
	//[1]VBG_EN
		//ADC Internal Bandgap Power Control
		//0 = Power down internal bandgap.
		//1 = Power on internal bandgap.
	(1<<1)|
	//[0]
		//AD_ENADC Power Control
		//0 = Power down ADC.
		//1 = Power on ADC.
	1;
	
	ReNum=1;
	while(ReNum--)
	{
	//ADC Configure
		p32=(u32*)(REG_ADC_CONF);
		p32[0]=
		//[31:24]ADCSAMPCNT
			//ADC Sample Counter
			//Set the counter value to extend the ADC start signal period to get more sampling time for precise conversion.
		//[22]HSPEED
			//High Speed Enable
			//Enable ADC to high speed mode.
		//[21]DISZMAVEN
			//Display Z Mean Average Enable
			//Pressure Mean average for Z1 and Z2 function enable.
		//[20]DISTMAVEN
			//Display T Mean Average Enable
			//Touch Mean average for X and Y function enable.
		//[19:16]REFCNT
			//ADC Reference Counter
			//ADC reference voltage stable counter count value.
			//0 = 1 ADC clock.
			//1 = 2 ADC clock.
			//2 = 4 ADC clock.
			//3 = 8 ADC clock.
			//4 = 16 ADC clock.
			//5 = 32 ADC clock.
			//6 = 64 ADC clock.
			//7 = 128 ADC clock.
			//8 = 256 ADC clock.
			//9 = 512 ADC clock.
			//10 = 1024 ADC clock.
			//11 = 1024 ADC clock.
			//12 = 1024 ADC clock.
			//13 = 1024 ADC clock.
			//14 = 1024 ADC clock.
			//15 = 1024 ADC clock.
		//[15:11]Reserved
		//[10]SELFT_EN
			//Selft Test Enable
			//Selft test function enable.
		(1<<10)|
		//[9]KPC_EN
			//Keypad Press Conversion Enable
			//Keypad press conversion function enable.
		//[8]VBAT_EN
			//Voltage Battery Enable
			//Battery voltage detection function enable.	
		//[7:6]REFSEL
			//ADC Reference Select
			//ADC reference voltage select when ADC operate in normal conversion.
			//00 = AGND33 vs 2.5v buffer output, or VREF input.
			//01 = YM vs YP.
			//10 = XM vs XP.
			//11 = AGND33 vs AVDD33.
		(3<<6)|
		//[5:3]CHSEL
			//Channel Selection
			//ADC input channel selection.
			//000 = VBT.
			//001 = VHS.
			//010 = A_2.
			//011 = A_3.
			//100 = YM.
			//101 = YP.
			//110 = XM.
			//111 = XP.
		//[2]NACEN
			//Normal AD Conversion Enable
			//ADC normal conversion function enable
		//[1]Z_EN
			//Press Enable
			//Press measure function enable
		//[0]T_EN
			//Touch Enable
			//Touch detection function enable
		0;
	//ADC_ISR
		p32=(u32*)(REG_ADC_ISR);
		p32[0]=0xffffffff;//write 1 to clear this bit.
	//Menu Start Conversion
		p32=(u32*)(REG_ADC_CTL);
		p32[0]|=1<<8;
	//wait
		p32=(u32*)(REG_ADC_ISR);
		while((p32[0]&(1<<0))==0);// wait Normal AD Conversion Finish
		p32=(u32*)(REG_ADC_SELFDATA);
		i=p32[0];
		if(i)
		{
			i=i;
		}
	}
//�ر�ADC
	//p32=(u32*)(REG_CLK_PCLKEN1);
	//p32[0]&=~(1<<24);
//
}
*/









