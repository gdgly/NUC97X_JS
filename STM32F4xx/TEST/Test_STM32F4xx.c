
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Test/Test_RAM.h"
#include "../STM32F4xx/STM32F4xx_ECC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"




u32  GetCRC32(u32 initcrc,u8 * DataPtr,u32 DataLen,u32 Poly)
{
	u32 *Table_CRC;
	u32 i, j;
	u32 nData;
	u32 nAccum;

	Table_CRC=(u32 *)(ADDR_DATABUFF);
	for ( i = 0; i < 256; i++ )
	{
		nData =( i << 24 );
		nAccum = 0;
		for ( j = 0; j < 8; j++ )
		{
			if ( ( nData ^ nAccum ) & 0x80000000 )
			{
				nAccum = ( nAccum << 1 ) ^ Poly;
			}
			else
			{
				nAccum <<= 1;
			}
			nData <<= 1;
		}
		Table_CRC[i] = nAccum;
	}
	nAccum=initcrc;
  for (i = 0; i < DataLen/4; i++)
	{
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[3]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[2]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[1]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[0]];
		DataPtr+=4;
	}
	return nAccum;//^0xffffffff;
}


u32 Test_STM32F4xx(u32 ADDR_BASE)
{
	u32 i;
	u32 x;
	u32 y;
	u64 d64;
	u32 ISRAMErr;
	u32 ECCErr;
	u32 CRCErr;
	u32 fLSI;
	u32 fLSE;//0=OK,1=ERR,0xff=没测试
	u32 fHSE;
	u8 *p8;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	__disable_irq();//关中断
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""STM32F4xx");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
//isram的64k掉电保护区测试
	ISRAMErr=RAM_Test(ADDR_Meter_SaveRam_Start,64*1024);//RAM测试;返回:0=正确,1=错误
//isram的4K(BKPSRAM)测试
	ISRAMErr|=RAM_Test(ADDR_BKPSRAM,4*1024);//RAM测试;返回:0=正确,1=错误
	ISRAMErr|=Comm_Ram->ErrorIC&1;
//isram的RTC->BKP0R-RTC->BKP19R
	ISRAMErr|=RAM_Test((u32)&RTC->BKP0R,4*20);//RAM测试;返回:0=正确,1=错误
	TestResult|=ISRAMErr;
//ECC
	ECCErr=0;//ECC_TEST();//ECC计算和校验测试(数据中的每字节按位取反测试,循环次数=长度*8);返回：0=正确,1=错误
	TestResult|=ECCErr;
//CRC
	i=RAM_CRC32(ADDR_DATABUFF+1024,256);
	CRCErr=GetCRC32(0xffffffff,(u8 *)ADDR_DATABUFF+1024,256,0x04C11DB7);
	if(i==CRCErr)
	{
		CRCErr=0;
	}
	else
	{
		CRCErr=1;
		TestResult=1;
	}

	
//开启LSE时钟
	fLSE=0;//0=OK,1=ERR,0xff=没测试
	RCC->BDCR=//位 31:17 保留，必须保持复位值
				(0<<16)|//位 16 BDRST：备份域软件复位 (Backup domain software reset)由软件置 1 和清零。注意： BKPSRAM 不受此复位影响，只能在 Flash 保护级别从级别 1 更改为级别 0 时复位BKPSRAM。
					//0：复位未激活
					//1：复位整个备份域
				(0<<15)|//位 15 RTCEN：RTC 时钟使能 (RTC clock enable)由软件置 1 和清零。
					//0：RTC 时钟禁止
					//1：RTC 时钟使能
				//位 14:10 保留，必须保持复位值。
				(1<<8)|//位 9:8 RTCSEL[1:0]：RTC 时钟源选择 (RTC clock source selection)由软件置 1，用于选择 RTC 的时钟源。选择 RTC 时钟源后，除非备份域复位，否则其不可再更改。可使用 BDRST 位对其进行复位。
					//00：无时钟
					//01：LSE 振荡器时钟用作 RTC 时钟
					//10：LSI 振荡器时钟用作 RTC 时钟
					//11：由可编程预分频器分频的 HSE 振荡器时钟（通过 RCC 时钟配置寄存器 (RCC_CFGR)中的 RTCPRE[4:0] 位选择）用作 RTC 时钟
				//位 7:3 保留，必须保持复位值。
				(1<<2)|//位 2 LSEBYP：外部低速振荡器旁路 (External low-speed oscillator bypass)由软件置 1 和清零，用于旁路调试模式下的振荡器。只有在禁止 LSE 时钟后才能写入该位。
					//0：不旁路 LSE 振荡器
					//1：旁路 LSE 振荡器
				//位 1 LSERDY：外部低速振荡器就绪 (External low-speed oscillator ready)由硬件置 1 和清零，用于指示外部 32 kHz 振荡器已稳定。在 LSEON 位被清零后，LSERDY将在 6 个外部低速振荡器时钟周期后转为低电平。
					//0：LSE 时钟未就绪
					//1：LSE 时钟就绪
				1;//位 0 LSEON：外部低速振荡器使能 (External low-speed oscillator enable)由软件置 1 和清零。
					//0：LSE 时钟关闭
					//1：LSE 时钟开启
					//当 LSE 振荡器处于关闭状态时，可分别将 LSE 振荡器引脚 OSC32_IN 和 OSC32_OUT 用 作通用 PC14 I/O 和 PC15 I/O。
					//当 LSE 振荡器处于开启状态时，PC14 I/O 和 PC15 I/O 只能 被配置为 LSE 振荡器引脚 OSC32_IN 和 OSC32_OUT。
					//可通过在 RCC_BDCR 寄存器中将 LSEON 位置 1 来完成此操作。LSE 的优先级高于 GPIO 功能
	for(i=0;i<10;i++)
	{
		SoftDelayMS(100);//软件ms延时
		if(RCC->BDCR&(1<<1))
		{
			break;//LSERDY：外部低速振荡器就绪
		}
	}
	if(i>=10)
	{
		fLSE=1;//0=OK,1=ERR,0xff=没测试
		TestResult=1;
	}
	else
	{
//HSE(16MHz)
		START_STM32F4xx_TIM5_MEASURE_LSI(2);//启动定时器TIM5为内部(LSI,LSE)时钟测量;入口:TI4_RMP=1(LSI),=2(LSE)
		while((TIM5->SR&(1<<4))==0)//位 4 CC4IF：捕获/比较 4 中断标志 (Capture/Compare 4 interrupt flag)
		{
		}
		i=TIM5->CCR4;
		for(x=0;x<(32768/8);x++)
		{
			TIM5->SR=0;
			while((TIM5->SR&(1<<12))==0)//位 12 CC4OF：捕获/比较 4 重复捕获标志
			{
			}
			y=TIM5->CCR4;
		}
		y-=i;
		d64=RCC_GetPCLK1Freq()*2;
		d64*=32768*2;
		
		y=d64/y;
		if(y==32768)
		{
			fHSE=16000000;
		}
		else
		{
			if(y>32768)
			{
				y-=32768;
				fHSE=16000000-((y*16000000)/32768);
			}
			else
			{//y<32768
				y=32768-y;
				fHSE=16000000+((y*16000000)/32768);
			}
			y*=1000000;
			y/=32768;
			if(y>200)//200ppm
			{//晶体误差超出 ?ppm
				TestResult=1;
			}
		}
	}
//LSI(32k)
	RCC->CSR|=1;//位 0 LSION：内部低速振荡器使能
	while((RCC->CSR&2)==0)
	{//等待LSI RC 振荡器就绪
	}
	START_STM32F4xx_TIM5_MEASURE_LSI(1);//启动定时器TIM5为内部(LSI,LSE)时钟测量;入口:TI4_RMP=1(LSI),=2(LSE)
	while((TIM5->SR&(1<<4))==0)//位 4 CC4IF：捕获/比较 4 中断标志 (Capture/Compare 4 interrupt flag)
	{
	}
	i=TIM5->CCR4;
	for(x=0;x<(16384/8);x++)
	{
		TIM5->SR=0;
		while((TIM5->SR&(1<<12))==0)//位 12 CC4OF：捕获/比较 4 重复捕获标志
		{
		}
		fLSI=TIM5->CCR4;
	}
	
	fLSI-=i;
	d64=RCC_GetPCLK1Freq()*2;
	d64*=16384*2;
	
	fLSI=d64/fLSI;
	if((fLSI<17000)||(fLSI>47000))
	{
		TestResult=1;
	}
//

//存储参数
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=ISRAMErr;
	p8[1]=ECCErr;
	p8[2]=CRCErr;
	p8[3]=fLSE;
	MWR(fHSE,ADDR_DATABUFF+4,4);
	MWR(fLSI,ADDR_DATABUFF+8,4);

	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_STM32F4xx_Characteristics),12);
	if(TestResult)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	return TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
}

