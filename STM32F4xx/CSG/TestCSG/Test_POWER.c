
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




u32 Test_POWER(u32 ADDR_BASE)//电源测试
{
#if MainProtocol==CSG
	return 0;
#else
	u32 i;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	u32 ReNUM;
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	DisplayStringAutoUp(0,9,(u8*)"\x0""VDD5V0");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
//VDD5V0
	Pin_Configure(PIN_ADC123_IN13,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(13,3,1,0);//ADC电压测量,入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址;返回:测量值(0xffffffff表示转换失败)
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
		SoftDelayMS(100);//软件ms延时
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics,2);
	if((i<0x480)||(i>0x500))
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
//ADD5V0

	
//IO3V3
	
	
//RTC_BATTERY3V0
	DisplayStringAutoUp(0,9,(u8*)"\x0""RTC3V0");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	Pin_Configure(PIN_URTC,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(8,2,1,0);//ADC电压测量,入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址;返回:测量值(0xffffffff表示转换失败)
		if((i>=0x310)&&(i<=0x330))
		{
			break;
		}
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics+6,2);
	if((i<0x310)||(i>0x340))
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
//CHARGE_BATTERY4V8
	
	
//GPRS4V0
	OnR5V0();//打开R5V0电源
	DisplayStringAutoUp(0,9,(u8*)"\x0""GPRS4V0");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	Pin_Configure(PIN_UGPRS,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	SoftDelayMS(100);//软件ms延时
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(12,2,1,0);//ADC电压测量,入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址;返回:测量值(0xffffffff表示转换失败)
		if((i>=0x390)&&(i<=0x410))
		{
			break;
		}
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics+10,2);
	if((i<0x390)||(i>0x410))
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
//ETH3V3
	OnE5V0();//打开E5V0电源
	DisplayStringAutoUp(0,9,(u8*)"\x0""ETH3V3");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	Pin_Configure(PIN_ADC12_IN6,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	SoftDelayMS(100);//软件ms延时
	ReNUM=5;
	while(ReNUM--)
	{
		i=ADCx(6,2,1,0);//ADC电压测量,入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址;返回:测量值(0xffffffff表示转换失败)
		if((i>=0x320)&&(i<=0x340))
		{
			break;
		}
	}
	MWR(i,ADDR_BASE+OFFSET_POWER_Characteristics+12,2);
	if((i<0x320)||(i>0x340))
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	
	return TestResult;
#endif
}




















