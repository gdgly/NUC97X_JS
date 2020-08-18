
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



u32 Test_ALIGN(void);//数据对齐测试;返回:0=正确,1=错误
u32 Test_POWER(u32 ADDR_BASE);//电源测试
u32 Test_STM32F4xx(u32 ADDR_BASE);
u32 Test_SDRAM(u32 ADDR_BASE);
u32 Test_GDMA(u32 ADDR_BASE);//测试GDMA;入口:要测试的块地址;返回:0=正确,1=错误
u32 Test_NAND_ECC(u32 ADDR_BLOCK);//测试NAND_ECC;入口:要测试的块地址;返回:0=正确,1=错误
u32 Test_YAFFS(void);//测试YAFFS文件系统;返回：0=ok,1=fail
u32 Test_90E36(u32 ADDR_BASE);
u32 Test_ATT7022(u32 ADDR_BASE);
u32 Test_RTC(u32 ADDR_BASE);
u32 Test_IP101GRI(u32 ADDR_BASE);
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

void LED_TestResult(u32 TestResult)//测试结果ED指示(仅2型集中器才指示)
{
#if ((Project/100)==5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
	u32 i;
	
	//Terminal文件无效，免中断中关LED灯
	FileDownload->File&=~0x8;//文件标志(置位表示OK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=...B31=
	if(TestResult==0)
	{//OK
		i=			0|//B3-B0:复用功能选择AF(0-15)
			 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
			 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
			 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
			 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
			(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
	}
	else
	{//ERR
		i=			0|//B3-B0:复用功能选择AF(0-15)
			 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
			 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
			 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
			 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
			(1<<11);//B11  :当为输出时：0=输出0,1=输出1	
	}
	Pin_Configure(PIN_ATTR_LED,i);//引脚配置
	Pin_Configure(PIN_GPRSRX_LED,i);//引脚配置
	Pin_Configure(PIN_RS485T1_LED,i);//引脚配置
	Pin_Configure(PIN_LED_ONLINE,i);//引脚配置
	Pin_Configure(PIN_WEAK_SINGLE,i);//引脚配置
	Pin_Configure(PIN_STRONG_SINGLE,i);//引脚配置
	Pin_Configure(PIN_RS485R1_LED,i);//引脚配置
	Pin_Configure(PIN_GPRSTX_LED,i);//引脚配置
#endif
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
//	Disable_os();//暂停os
//	__disable_irq();//关中断
	
	DisplayClr();//清全屏
	OnBACKLIGHT();//打开BACKLIGHT电源
	OnLED3V3();//打开LED3V3电源
	OnA5V0();//打开A5V0电源
	OnEMU();//打开EMU(复位解除)
	OnE5V0();//打开E5V0电源
	OnR5V0();//打开R5V0电源
	#ifdef PIN_PLC_PWR
	Pin_Configure(PIN_PLC_PWR,//复位引脚输出0
							0|//B3-B0:复用功能选择AF(0-15)
				 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(1<<11));//B11  :当为输出时：0=输出0,1=输出1	
	#endif
	
	n=0;//按温度区分的测试号:0=常温(-5<T<45),1=低温(T<-5),2=高温（T>55)
	Comm_Ram->TEMP=0x0250;//2BYTE BCD 1位小数
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
	
//显示温度
	DisplayString(0,0,0,(u8 *)"Temperature");//显示字符串
	p16=(u16 *)(ADDR_STRINGBUFF+4+(12*2));
	i=Comm_Ram->TEMP;
	p16=DisplayData_OffZero(i,4,1,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MRWBUFF
	//DispalyStringContinue(p16,(u8*)"℃");//显示字符串继续
	AutoDisplayRowCol->Row=1;
	
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	WWDT_Disable();//禁止看门狗 
	

	LED_TestResult(TestResult);//测试结果ED指示(仅2型集中器才指示)
//DATA_ALIGN
	DisplayStringAutoUp(0,9,(u8*)"\x0""DATA_ALIGN");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	i=Test_ALIGN();//数据对齐测试;返回:0=正确,1=错误
	TestResult|=i;
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
//STM32F4xx
//	TestResult|=Test_STM32F4xx(ADDR_BASE);

	TestResult|=Test_GDMA(ADDR_BASE);//测试GDMA;入口:要测试的块地址;返回:0=正确,1=错误
//NAND
	TestResult|=Test_NAND_ECC(ADDR_BASE);//测试NAND_ECC;入口:要测试的块地址;返回:0=正确,1=错误
//YAFFS
	TestResult|=Test_YAFFS();//测试YAFFS文件系统;返回：0=ok,1=fail

//INOUT
	TestResult|=Test_INOUT(ADDR_BASE);//输入输出测试
//RTC
	TestResult|=Test_RTC(ADDR_BASE);

//ESAM
#if IC_TESAM==1
	TestResult|=Test_ESAM(ADDR_BASE);
#endif
//RS485,红外,RS232测试
	TestResult|=Test_UART(ADDR_BASE);

//POWER
	TestResult|=Test_POWER(ADDR_BASE);//电源测试
//交采
#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
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
#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
	TestResult|=Test_ETH(ADDR_BASE);//以太网测试
#endif

//GPRS,MODULE
	TestResult|=Test_GPRS_MODULE(ADDR_BASE);//GPRS模块,载波模块或控制模块


////24AA02E48
//	TestResult|=Test_24AA02E48(ADDR_BASE);

//ISRAM(56K)
	DisplayStringAutoUp(0,9,(u8*)"\x0""ISRAM");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	i=RAM_Test(0x3c000000,56*1024);//数据对齐测试;返回:0=正确,1=错误
	TestResult|=i;
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
//DDR2(自动64M或32M)
	DisplayStringAutoUp(0,9,(u8*)"\x0""DDR2");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	i=Test_SDRAM(ADDR_BASE);//数据对齐测试;返回:0=正确,1=错误
	TestResult|=i;
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	MC(i,ADDR_BASE+OFFSET_SDRAM_Characteristics,1);
	
  //恢复工厂模式
	MWR(31*24*60,ADDR_MeterFMTimer+14,2);//31*24*60分定时
	MWR(31*24*60,ADDR_TerminalFMTimer+14,2);//31*24*60分定时
#if NAND128kBuff==0//定义128k nand写缓冲
	Check_4kNandBuff();//检查4K(BKPSRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
#else
	Check_128kNandBuff();//检查128K(BKPSDRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
#endif
//KEY_LED_BUZZER
#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
	TestResult|=Test_KEY_LED_BUZZER(ADDR_BASE);
#endif
	
//测试总结论
	if(TestResult)
	{//不合格
		MWR(1,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
	}
	else
	{
	#if ((Project/100)==5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
		//合格
		MWR(0,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
	#else
		//除掉电测试外合格
		MWR(2,ADDR_BASE+OFFSET_TestResult_Characteristics,1);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
	#endif
	}
	
//电网掉电
	__enable_irq();//允许中断
	MC(0x55,ADDR_BASE+OFFSET_POWER_DOWN_Characteristics,2);//0=OK,1=ERR,0xff=没测试,0x55=进行测试

	
#if ((Project/100)==5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
	//删除TEST程序
	MC(0,ADDR_SPIFLASH_START+ADDR_Menu_ENTRY,512);
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
			WWDT_RESET();//立即使能看门狗复位
		}
		if(TestResult==0)
		{
			SoftDelayMS(500);//软件ms延时
			LED_TestResult(1);//测试结果ED指示(仅2型集中器才指示)
			SoftDelayMS(500);//软件ms延时
			LED_TestResult(0);//测试结果ED指示(仅2型集中器才指示)
		}
		else
		{
			LED_TestResult(TestResult);//测试结果ED指示(仅2型集中器才指示)
		}
	}
#else
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
		if(SuperCap_Voltage()>0x300)//超级电容电池电压ADC测量;返回:BCD格式2位小数
		{//有超级电容不测试
			SoftDelayMS(1000);//软件ms延时
			WWDT_RESET();//立即使能看门狗复位
		}
	}
#endif
	
}






