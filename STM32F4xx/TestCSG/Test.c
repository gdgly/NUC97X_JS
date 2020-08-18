
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
u32 Test_POWER(u32 ADDR_BASE);//电源测试
u32 Test_STM32F4xx(u32 ADDR_BASE);
u32 Test_SDRAM(u32 ADDR_BASE);
u32 Test_MT29FxG_NAND(u32 ADDR_BASE);
u32 Test_S34MLxG_NAND(u32 ADDR_BASE);
u32 Test_90E36(u32 ADDR_BASE);
u32 Test_RTC(u32 ADDR_BASE);
u32 Test_LAN8720A(u32 ADDR_BASE);
u32 Test_UART(u32 ADDR_BASE);
u32 Test_ETH(u32 ADDR_BASE);//以太网测试
u32 Test_GPRS(u32 ADDR_BASE);//GPRS模块
u32 Test_GPRS_MODULE(u32 ADDR_BASE);//GPRS模块,载波模块或控制模块
u32 Test_LOCAL_MODULE(u32 ADDR_BASE);//本地载波模块
u32 Test_CONTRL_MODULE(u32 ADDR_BASE);//专变控制模块
u32 Test_ESAM(u32 ADDR_BASE);
u32 Test_24AA02E48(u32 ADDR_BASE);
u32 Test_INOUT(u32 ADDR_BASE);//输入输出测试
u32 Test_KEY_LED_BUZZER(u32 ADDR_BASE);



void Test_ReStartOS(void)//电气性能测试重启OS前(需OS重启原因是当以太网为自动IP时进入自动得到IP时测试失败!!!)
{
	Comm_Ram->Init_OS|=1<<7;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
	ReSTART_OS();//重启OS
	while(1)
	{
	}
}


void Test(void)//电气性能测试
{
	u32 i;
	u32 n;
	u32 ADDR_BASE;
	u16 *p16;
	u32 *p32;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	p32=p32;
	//保存nand缓冲数据
#if NAND128kBuff==0//定义128k nand写缓冲
	NAND4kBuff_Write_NAND(0,0,0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#else
	NAND128kBuff_Write_NAND(0,0,0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#endif
	Disable_os();//暂停os
	__disable_irq();//关中断
	
	RCC_SystemClock_Config_168MHz();
	DisplayClr();//清全屏
	OnBACKLIGHT();//打开BACKLIGHT电源
	OnLED3V3();//打开LED3V3电源
	OnA5V0();//打开A5V0电源
	OnE5V0();//打开E5V0电源
	OnR5V0();//打开R5V0电源
	
	n=0;//按温度区分的测试号:0=常温(-5<T<45),1=低温(T<-5),2=高温（T>55)
	i=Comm_Ram->TEMP;//2BYTE BCD 1位小数
	if(i&0x8000)
	{//负
		if(i>0x8050)
		{
			n=1;
		}
	}
	else
	{//正
		if(i>0x0550)
		{
			n=2;
		}
	}
	MWR(n,ADDR_Characteristics_NO,1);
	ADDR_BASE=ADDR_Characteristics_BASE1+(n*LEN_BASE_Characteristics);
	MWR(i,ADDR_BASE+(OFFSET_Test_Temperature),2);
//清全部测试数据(除寿命测试外)
	MC(0xff,ADDR_BASE+(OFFSET_POWER_Characteristics),(LEN_BASE_Characteristics)-(OFFSET_POWER_Characteristics));
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
//显示温度
	DisplayString(0,0,0,(u8 *)"Temperature");//显示字符串
	p16=(u16 *)(ADDR_STRINGBUFF+4+(12*2));
	i=Comm_Ram->TEMP;
	p16=DisplayData_OffZero(i,4,1,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MRWBUFF
	//DispalyStringContinue(p16,(u8*)"℃");//显示字符串继续
	AutoDisplayRowCol->Row=1;
	

//POWER
	SoftDelayMS(500);//软件ms延时
	TestResult|=Test_POWER(ADDR_BASE);//电源测试
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
	TestResult|=Test_ETH(ADDR_BASE);//以太网测试
/*
//GPRS
	TestResult|=Test_GPRS(ADDR_BASE);//GPRS模块
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//LOCAL MODULE
	TestResult|=Test_LOCAL_MODULE(ADDR_BASE);//本地载波模块
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//CONTRL MODULE
	TestResult|=Test_CONTRL_MODULE(ADDR_BASE);//专变控制模块
#endif
*/
//GPRS,MODULE
	TestResult|=Test_GPRS_MODULE(ADDR_BASE);//GPRS模块,载波模块或控制模块
//其他UART测试 在GPRS,MODULE后因使用了复位DMA1,DMA2
	TestResult|=Test_UART(ADDR_BASE);
//ESAM
	TestResult|=Test_ESAM(ADDR_BASE);
//24AA02E48
	TestResult|=Test_24AA02E48(ADDR_BASE);
//INOUT
	TestResult|=Test_INOUT(ADDR_BASE);//输入输出测试
//sdram
	TestResult|=Test_SDRAM(ADDR_BASE);
	
  //恢复工厂模式
	MWR(31*24*60,ADDR_MeterFMTimer+14,2);//31*24*60分定时
	MWR(31*24*60,ADDR_TerminalFMTimer+14,2);//31*24*60分定时
#if NAND128kBuff==0//定义128k nand写缓冲
	Check_4kNandBuff();//检查4K(BKPSRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
#else
	Check_128kNandBuff();//检查128K(BKPSDRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
#endif
	
//KEY_LED_BUZZER
	TestResult|=Test_KEY_LED_BUZZER(ADDR_BASE);
	
//测试总结论
	MWR(TestResult,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
	
//电网掉电
	__enable_irq();//允许中断
	//清4K(BKPSRAM)进行保持测试
	p32=(u32*)(ADDR_BKPSRAM);
	for(i=0;i<(4096/4);i++)
	{
		p32[i]=i;
	}
	MC(0x55,ADDR_BASE+OFFSET_POWER_DOWN_Characteristics,2);//0=OK,1=ERR,0xff=没测试,0x55=进行测试
	
	Charge_Battery_Voltage();//可充电4.8V电池电压ADC测量
	i=MRR(ADDR_UBACKUP,2);
	if(i)
	{//有充电电池错了
		DisplayStringAutoUp(0,9,(u8*)"\x0""Load battery ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
		while(1)
		{
			WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
			Charge_Battery_Voltage();//可充电4.8V电池电压ADC测量
			i=MRR(ADDR_UBACKUP,2);
			if(i==0)
			{//充电电池=0
				break;
			}
		}
	}
	BUZZ_Generate(12);//蜂鸣产生,入口蜂鸣号
	DisplayStringAutoUp(0,9,(u8*)"\x0""Turn off the power  to keep 30s above");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	//等待掉电
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
		{
		//数据保存
			if(Comm_Ram->DataSave==1)//数据保护:0=空,1=电网有电超过10秒数据需保护,2=数据已保护
			{
				Comm_Ram->DataSave=2;//数据保护:0=空,1=电网有电超过10秒数据需保护,2=数据已保护
				PowerDown_Data_Save();//电源掉电数据保存
			}
		}
	}
}






