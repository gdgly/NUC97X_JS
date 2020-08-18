

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Display/Display.h"
#include "../Test/Test_RAM.h"
#include "stm32f4xx_hal.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"





FMC_SDRAM_TimingTypeDef SdramTiming;
void MY_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing)
{   
  //Initialize the low level hardware (MSP)
//  HAL_SDRAM_MspInit(hsdram);
  // Initialize SDRAM control Interface
  FMC_SDRAM_Init(hsdram->Instance, &(hsdram->Init));
  // Initialize SDRAM timing Interface
  FMC_SDRAM_Timing_Init(hsdram->Instance, Timing, hsdram->Init.SDBank); 
}

void SDRAM_FMC_Init(void)
{
	SDRAM_HandleTypeDef hsdram;
	
  hsdram.Instance = FMC_SDRAM_DEVICE;
  // hsdram.Init
  hsdram.Init.SDBank = FMC_SDRAM_BANK1;
	
#if IC_MT48LC4M16==1//0=没,1=有(8 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
#endif
#if IC_MT48LC8M16==1//0=没,1=有(16 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
#endif
#if IC_MT48LC16M16==1//0=没,1=有(32 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
#endif
#if IC_MT48LC32M16==1//0=没,1=有(32 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
#endif

#if IC_MT48LC8M8==1//0=没,1=有(8 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_8;
#endif
#if IC_MT48LC16M8==1//0=没,1=有(16 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_8;
#endif
#if IC_MT48LC32M8==1//0=没,1=有(32 Mbyte)
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_8;
#endif
#if IC_MT48LC64M8==1//0=没,1=有(32 Mbyte)
	//A11无法设置?
  hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;
  hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_8;
#endif

  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;//FMC_SDRAM_RBURST_DISABLE;
  hsdram.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;//0-2个HCLK 时钟周期延迟,这些位可定义在 CAS 延迟后延后多少个 HCLK 时钟周期读取数据
  // SdramTiming
#if (IC_MT48LC4M16|IC_MT48LC8M8)
	FMC_Bank5_6->SDRTR=((1312-20)<<1);//SDRAM 刷新定时器寄存器;刷新速率=64ms/(4096行)=15.625μs;15.625μs*84MHz=1312.5;COUNT=1312-20
#endif
#if (IC_MT48LC8M16|IC_MT48LC16M8)
	FMC_Bank5_6->SDRTR=((1312-20)<<1);//SDRAM 刷新定时器寄存器;刷新速率=64ms/(4096行)=15.625μs;15.625μs*84MHz=1312.5;COUNT=1312-20
#endif
#if (IC_MT48LC16M16|IC_MT48LC32M8)
	FMC_Bank5_6->SDRTR=((656-20)<<1);//SDRAM 刷新定时器寄存器;刷新速率=64ms/(8192行)=7.8125μs;7.8125μs*84MHz=656.25;COUNT=656-20
#endif
#if (IC_MT48LC32M16|IC_MT48LC64M8)
	FMC_Bank5_6->SDRTR=((656-20)<<1);//SDRAM 刷新定时器寄存器;刷新速率=64ms/(8192行)=7.8125μs;7.8125μs*84MHz=656.25;COUNT=656-20
#endif
	MY_SDRAM_Init(&hsdram, &SdramTiming);
}

u32 Test_SDRAM(u32 ADDR_BASE)
{
	u32 i;
	u32 x;
	u32 Err;
	u8 *p8;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	__disable_irq();//关中断
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
#if IC_MT48LC4M16==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC4M16");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC8M16==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC8M16");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC16M16==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC16M16");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC32M16==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC32M16");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC8M8==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC8M8");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC16M8==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC16M8");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC32M8==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC32M8");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_MT48LC64M8==1
	DisplayStringAutoUp(0,9,(u8*)"\x0""MT48LC64M8");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
	__disable_irq();//关中断
	SdramTiming.LoadToActiveDelay = 2;//1-16//加载模式寄存器到激活 (Load Mode Register to Active)
	SdramTiming.ExitSelfRefreshDelay = 6;//1-16//退出自刷新延迟 (Exit Self-refresh delay)
	SdramTiming.SelfRefreshTime = 4;//1-16//自刷新时间 (Self refresh time)
	SdramTiming.RowCycleDelay = 6;//1-16//行循环延迟 (Row cycle delay)
	SdramTiming.WriteRecoveryTime = 4;//1-16//恢复延迟 (Recovery delay)
	SdramTiming.RPDelay = 2;//1-16//行预充电延迟 (Row precharge delay)
	SdramTiming.RCDDelay = 2;//1-16//行到列延迟 (Row to column delay)

//SdramTiming.ExitSelfRefreshDelay = 1;//1-16//退出自刷新延迟 (Exit Self-refresh delay)
	i=16;
	while(i--)
	{
		SdramTiming.ExitSelfRefreshDelay =i;
		SDRAM_FMC_Init();
		x=RAM_Test(0xc0000000,8192);//RAM测试;返回:0=正确,1=错误
		if(x)
		{
			SdramTiming.ExitSelfRefreshDelay++;
			break;
		}
		if(i==1)
		{
			break;
		}
	}
//SdramTiming.SelfRefreshTime = 1;//1-16//自刷新时间 (Self refresh time)
	i=16;
	while(i--)
	{
		SdramTiming.SelfRefreshTime =i;
		SDRAM_FMC_Init();
		x=RAM_Test(0xc0000000,8192);//RAM测试;返回:0=正确,1=错误
		if(x)
		{
			SdramTiming.SelfRefreshTime++;
			break;
		}
		if(i==1)
		{
			break;
		}
	}
//SdramTiming.RowCycleDelay = 4;//1-16//行循环延迟 (Row cycle delay)
	i=16;
	while(i--)
	{
		SdramTiming.RowCycleDelay =i;
		SDRAM_FMC_Init();
		x=RAM_Test(0xc0000000,8192);//RAM测试;返回:0=正确,1=错误
		if(x)
		{
			SdramTiming.RowCycleDelay++;
			break;
		}
		if(i==1)
		{
			break;
		}
	}
//SdramTiming.WriteRecoveryTime = 2;//1-16//恢复延迟 (Recovery delay)
	i=16;
	while(i--)
	{
		SdramTiming.WriteRecoveryTime =i;
		SDRAM_FMC_Init();
		x=RAM_Test(0xc0000000,8192);//RAM测试;返回:0=正确,1=错误
		if(x)
		{
			SdramTiming.WriteRecoveryTime++;
			break;
		}
		if(i==2)
		{
			break;
		}
	}
//SdramTiming.RPDelay = 1;//1-16//行预充电延迟 (Row precharge delay)
	i=16;
	while(i--)
	{
		SdramTiming.RPDelay =i;
		SDRAM_FMC_Init();
		x=RAM_Test(0xc0000000,8192);//RAM测试;返回:0=正确,1=错误
		if(x)
		{
			SdramTiming.RPDelay++;
			break;
		}
		if(i==1)
		{
			break;
		}
	}
//SdramTiming.RCDDelay = 1;//1-16//行到列延迟 (Row to column delay)
	i=16;
	while(i--)
	{
		SdramTiming.RCDDelay =i;
		SDRAM_FMC_Init();
		x=RAM_Test(0xc0000000,8192);//RAM测试;返回:0=正确,1=错误
		if(x)
		{
			SdramTiming.RCDDelay++;
			break;
		}
		if(i==1)
		{
			break;
		}
	}
	SDRAM_FMC_Init();
	
	//存储参数
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=2;//SDRAM_CAS_LATENCY
	p8[1]=SdramTiming.LoadToActiveDelay;// = 16;//1-16//加载模式寄存器到激活 (Load Mode Register to Active)
	p8[2]=SdramTiming.ExitSelfRefreshDelay;// = 1;//1-16//退出自刷新延迟 (Exit Self-refresh delay)
	p8[3]=SdramTiming.SelfRefreshTime;// = 1;//1-16//自刷新时间 (Self refresh time)
	p8[4]=SdramTiming.RowCycleDelay;// = 4;//1-16//行循环延迟 (Row cycle delay)
	p8[5]=SdramTiming.WriteRecoveryTime;// = 2;//1-16//恢复延迟 (Recovery delay)
	p8[6]=SdramTiming.RPDelay;// = 1;//1-16//行预充电延迟 (Row precharge delay)
	p8[7]=SdramTiming.RCDDelay;// = 1;//1-16//行到列延迟 (Row to column delay)
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_SDRAM_Characteristics),8);
	
	Err=0;
	if(SdramTiming.ExitSelfRefreshDelay>6)
	{
		Err=1;
	}
	if(SdramTiming.SelfRefreshTime>4)
	{
		Err=1;
	}
	if(SdramTiming.RowCycleDelay>6)
	{
		Err=1;
	}
	if(SdramTiming.WriteRecoveryTime>4)
	{
		Err=1;
	}
	if(SdramTiming.RPDelay>2)
	{
		Err=1;
	}
	if(SdramTiming.RCDDelay>2)
	{
		Err=1;
	}
	
	
#if IC_MT48LC4M16==1
	Err|=RAM_Test(ADDR_ERAM_START,0x00800000);//SDRAM测试;返回:0=正确,1=错误
#endif
#if IC_MT48LC8M16==1
	Err|=RAM_Test(ADDR_ERAM_START,0x01000000);//RAM测试;返回:0=正确,1=错误
#endif
#if IC_MT48LC16M16==1
	Err|=RAM_Test(ADDR_ERAM_START,0x02000000);//RAM测试;返回:0=正确,1=错误
#endif
#if IC_MT48LC32M16==1
	Err|=RAM_Test(ADDR_ERAM_START,0x04000000);//RAM测试;返回:0=正确,1=错误
#endif
	
#if IC_MT48LC8M8==1
	Err|=RAM_Test(ADDR_ERAM_START,0x00800000);//SDRAM测试;返回:0=正确,1=错误
#endif
#if IC_MT48LC16M8==1
	Err|=RAM_Test(ADDR_ERAM_START,0x01000000);//RAM测试;返回:0=正确,1=错误
#endif
#if IC_MT48LC32M8==1
	Err|=RAM_Test(ADDR_ERAM_START,0x02000000);//RAM测试;返回:0=正确,1=错误
#endif
#if IC_MT48LC64M8==1
	Err|=RAM_Test(ADDR_ERAM_START,0x04000000);//RAM测试;返回:0=正确,1=错误
#endif

	if(Err)
	{//1=错误
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	
	return TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
}








