

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
#include "stm32f4xx_hal.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"



#if IC_S34MLxG==1

u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
NAND_HandleTypeDef hnand;
FMC_NAND_PCC_TimingTypeDef ComSpaceTiming;

void  MY_NAND_Init(NAND_HandleTypeDef *hnand, FMC_NAND_PCC_TimingTypeDef *ComSpace_Timing, FMC_NAND_PCC_TimingTypeDef *AttSpace_Timing)
{
  // Initialize the low level hardware (MSP)
//    HAL_NAND_MspInit(hnand);
  // Initialize NAND control Interface
  FMC_NAND_Init(hnand->Instance, &(hnand->Init));
  
  // Initialize NAND common space timing Interface
  FMC_NAND_CommonSpace_Timing_Init(hnand->Instance, ComSpace_Timing, hnand->Init.NandBank);
  
  // Initialize NAND attribute space timing Interface
  FMC_NAND_AttributeSpace_Timing_Init(hnand->Instance, AttSpace_Timing, hnand->Init.NandBank);
  
  // Enable the NAND device
  __FMC_NAND_ENABLE(hnand->Instance, hnand->Init.NandBank);
}

void NAND_BANK3_FMC_Init(void)
{
  FMC_NAND_PCC_TimingTypeDef AttSpaceTiming;
	
  hnand.Instance = FMC_NAND_DEVICE;
  // hnand.Init 
  hnand.Init.NandBank = FMC_NAND_BANK3;
  hnand.Init.Waitfeature = FMC_NAND_PCC_WAIT_FEATURE_DISABLE;//FMC_NAND_PCC_WAIT_FEATURE_ENABLE;
  hnand.Init.MemoryDataWidth = FMC_NAND_PCC_MEM_BUS_WIDTH_8;
  hnand.Init.EccComputation = FMC_NAND_ECC_DISABLE;//FMC_NAND_ECC_ENABLE;
  hnand.Init.ECCPageSize = FMC_NAND_ECC_PAGE_SIZE_256BYTE;

  // hnand.Info 
  hnand.Info.PageSize = 0;
  hnand.Info.SpareAreaSize = 0;
  hnand.Info.BlockSize = 0;
  hnand.Info.BlockNbr = 0;
  hnand.Info.ZoneSize = 0;

	// AttSpaceTiming 
	AttSpaceTiming.SetupTime = 8-1;//8
	AttSpaceTiming.WaitSetupTime = 4-1;//4
	AttSpaceTiming.HoldSetupTime = 3-1;//2
	AttSpaceTiming.HiZSetupTime = 1-1;//1
	
  MY_NAND_Init(&hnand, &ComSpaceTiming, &AttSpaceTiming);
}


u32 Test_S34MLxG_NAND(u32 ADDR_BASE)
{
	u32 i;
	u32 x;
	u32 Err;
	u8 *p8;
	u32 BlockEraseTimer;
	u32 PageWriteTimer;
	u32 PageReadTimer;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
#if IC_S34ML1G==1//0=没,1=有(128 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""S34ML01G");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_S34ML2G==1//0=没,1=有(256 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""S34ML02G");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_S34ML4G==1//0=没,1=有(512 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""S34ML04G");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
  hnand.Init.TCLRSetupTime = 2-1;//;CPU值(0-15);CLE 到 RE 的延迟
  hnand.Init.TARSetupTime = 2-1;//;CPU值(0-15);ALE 到 RE 的延迟 
	
	ComSpaceTiming.SetupTime = 8-1;//1-255;CPU值(0-255)通用存储器建立时间
	ComSpaceTiming.WaitSetupTime = 4-1;//2-255;CPU值(1-255)通用存储器等待时间
	ComSpaceTiming.HoldSetupTime = 3-1;//1-254;CPU值(1-255)通用存储器保持时间
	ComSpaceTiming.HiZSetupTime = 1-1;//1-255;CPU值(0-255)通用存储器数据总线高阻态时间

//hnand.Init.TCLRSetupTime = 0;//;CPU值(0-15);CLE 到 RE 的延迟 (CLE to RE delay)。
	i=4;
	while(1)
	{
		hnand.Init.TCLRSetupTime=i;
		NAND_BANK3_FMC_Init();
		x=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x)
		{
			hnand.Init.TCLRSetupTime++;
			break;
		}
		if(i==0)
		{
			break;
		}
		i--;
	}
//hnand.Init.TARSetupTime = 0;//;CPU值(0-15);ALE 到 RE 的延迟 
	i=4;
	while(1)
	{
		hnand.Init.TARSetupTime=i;
		NAND_BANK3_FMC_Init();
		x=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x)
		{
			hnand.Init.TARSetupTime++;
			break;
		}
		if(i==0)
		{
			break;
		}
		i--;
	}
//ComSpaceTiming.SetupTime = 8-1;//1-255;CPU值(0-255)通用存储器建立时间
	i=16;
	while(1)
	{
		ComSpaceTiming.SetupTime=i;
		NAND_BANK3_FMC_Init();
		x=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x)
		{
			ComSpaceTiming.SetupTime++;
			break;
		}
		if(i==0)
		{
			break;
		}
		i--;
	}
//ComSpaceTiming.WaitSetupTime = 4-1;//2-255;CPU值(1-255)通用存储器等待时间
	i=8;
	while(1)
	{
		ComSpaceTiming.WaitSetupTime=i;
		NAND_BANK3_FMC_Init();
		x=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x)
		{
			ComSpaceTiming.WaitSetupTime++;
			break;
		}
		if(i==1)
		{
			break;
		}
		i--;
	}
//ComSpaceTiming.HoldSetupTime = 2;//1-254;CPU值(1-255)通用存储器保持时间
	i=8;
	while(1)
	{
		ComSpaceTiming.HoldSetupTime=i;
		NAND_BANK3_FMC_Init();
		x=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x)
		{
			ComSpaceTiming.HoldSetupTime++;
			break;
		}
		if(i==1)
		{
			break;
		}
		i--;
	}
//ComSpaceTiming.HiZSetupTime = 1-1;//1-255;CPU值(0-255)通用存储器数据总线高阻态时间
	i=1;
	while(1)
	{
		ComSpaceTiming.HiZSetupTime=i;
		NAND_BANK3_FMC_Init();
		x=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x)
		{
			ComSpaceTiming.HiZSetupTime--;
			break;
		}
		if(i==16)
		{
			break;
		}
		i++;
	}
	NAND_BANK3_FMC_Init();

//块擦除时间(ms)
	p8=(u8*)0x80000000;
	i=(NAND_BLOCK_COUNT-1)*128*1024;
	p8[0x10000]=0x60;//Command
	p8[0x20000]=(i>>11);//Row Add. 1
	p8[0x20000]=(i>>19);//Row Add. 2
#if (NAND_2G|NAND_4G|NAND_8G)
	p8[0x20000]=(i>>27);//Row Add. 3
#endif
	p8[0x10000]=0xD0;//Command
	while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
	START_STM32F4xx_TIM2();//启动定时器TIM2为时间测试定时器
	//等待块擦除完成
	while(1)
	{
		if(Pin_Read(PIN_NAND_WAIT)==0)//读引脚,入口引脚使用名;返回0或1
		{
			break;
		}
	}
	while(1)
	{
		if(Pin_Read(PIN_NAND_WAIT)!=0)//读引脚,入口引脚使用名;返回0或1
		{
			break;
		}
	}
	BlockEraseTimer=STOP_STM32F4xx_TIM2_US();//停止定时器TIM2为时间测试定时器，返回计数值(hex us)
	BlockEraseTimer/=1000;
//页写时间(uS)
	i=(NAND_BLOCK_COUNT-1)*128*1024;
	p8[0x10000]=0x80;//Command
	p8[0x20000]=i;//Col.Add.1
	p8[0x20000]=(i>>8)&0x7;//Col.Add.2
	p8[0x20000]=(i>>11);//Row Add. 1
	p8[0x20000]=(i>>19);//Row Add. 2
#if (NAND_2G|NAND_4G|NAND_8G)
	p8[0x20000]=(i>>27);//Row Add. 3
#endif
	p8[0x10000]=0x10;//Command
	while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
	START_STM32F4xx_TIM2();//启动定时器TIM2为时间测试定时器
	//等待块擦除完成
	while(1)
	{
		if(Pin_Read(PIN_NAND_WAIT)==0)//读引脚,入口引脚使用名;返回0或1
		{
			break;
		}
	}
	while(1)
	{
		if(Pin_Read(PIN_NAND_WAIT)!=0)//读引脚,入口引脚使用名;返回0或1
		{
			break;
		}
	}
	PageWriteTimer=STOP_STM32F4xx_TIM2_US();//停止定时器TIM2为时间测试定时器，返回计数值(hex us)
//页读到BUFF时尚(uS)
	i=(NAND_BLOCK_COUNT-1)*128*1024;
	p8[0x10000]=0x00;//1st Cycle Page Read Command
	p8[0x20000]=i;//Col.Add.1
	p8[0x20000]=(i>>8)&0x07;//Col.Add.2
	p8[0x20000]=(i>>11);//Row Add. 1
	p8[0x20000]=(i>>19);//Row Add. 2
#if (NAND_2G|NAND_4G|NAND_8G)
	p8[0x20000]=(i>>27);//Row Add. 3
#endif
	p8[0x10000]=0x30;//2nd Cycle Cycle Page Read Command
	while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
	START_STM32F4xx_TIM2();//启动定时器TIM2为时间测试定时器
	//等待块擦除完成
	while(1)
	{
		if(Pin_Read(PIN_NAND_WAIT)==0)//读引脚,入口引脚使用名;返回0或1
		{
			break;
		}
	}
	while(1)
	{
		if(Pin_Read(PIN_NAND_WAIT)!=0)//读引脚,入口引脚使用名;返回0或1
		{
			break;
		}
	}
	PageReadTimer=STOP_STM32F4xx_TIM2_US();//停止定时器TIM2为时间测试定时器，返回计数值(hex us)
	
	//存储参数
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=hnand.Init.TCLRSetupTime;// = 0;//;CPU值(0-15);CLE 到 RE 的延迟
	p8[1]=hnand.Init.TARSetupTime;// = 0;//;CPU值(0-15);ALE 到 RE 的延迟 
	p8[2]=ComSpaceTiming.SetupTime;// = 8-1;//1-255;CPU值(0-255)通用存储器建立时间
	p8[3]=ComSpaceTiming.WaitSetupTime;// = 4-1;//2-255;CPU值(1-255)通用存储器等待时间
	p8[4]=ComSpaceTiming.HoldSetupTime;// = 2;//1-254;CPU值(1-255)通用存储器保持时间
	p8[5]=ComSpaceTiming.HiZSetupTime;// = 1-1;//1-255;CPU值(0-255)通用存储器数据总线高阻态时间
	p8[6]=BlockEraseTimer;
	p8[7]=BlockEraseTimer>>8;
	p8[8]=PageWriteTimer;
	p8[9]=PageWriteTimer>>8;
	p8[10]=PageReadTimer;
	p8[11]=PageReadTimer>>8;
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_NAND_Characteristics),12);
	
	Err=0;
	if(hnand.Init.TCLRSetupTime>2)
	{
		Err=1;
	}
	if(hnand.Init.TARSetupTime>2)
	{
		Err=1;
	}
	if(ComSpaceTiming.SetupTime>4)
	{
		Err=1;
	}
	if(ComSpaceTiming.WaitSetupTime>4)
	{
		Err=1;
	}
	if(ComSpaceTiming.HoldSetupTime>3)
	{
		Err=1;
	}
//	if(ComSpaceTiming.HiZSetupTime<4)
//	{
//		Err=1;
//	}
	if(BlockEraseTimer>10)//ms;max=10mS
	{
		Err=1;
	}
	if(PageWriteTimer>700)//us;max=700us
	{
		Err=1;
	}
	if(PageReadTimer>25)//us;max=25us
	{
		Err=1;
	}
	
//	Err|=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
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

#endif









