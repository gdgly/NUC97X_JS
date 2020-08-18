
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Device/ATM90E36.h"
#include "../STM32F4xx/STM32F4xx_SPI.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"


u32 Test_90E36(u32 ADDR_BASE)
{
	u32 i;
	u32 x;
	u32 y;
	u32 BR;
	u32 Err;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""M90E36");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	OnA5V0();//打开A5V0电源
	OnEMU();//打开EMU(复位解除)
	SoftDelayMS(100);//软件ms延时
	
//SPI速率测试
	Init_SPI2();//SPI2初始化(ATM90E36)
	SPI2->CR2=0;
	SoftDelayUS(1);
	BR=7;
	for(i=0;i<8;i++)
	{
		SPI2->CR1&=~(0x7<<3);
		SPI2->CR1|=(BR<<3);
		BR--;
		//写
		Pin_OutL(PIN_SPI2_NSS);
		y=SPI2->DR;
		SPI2->DR=0x08;
		while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
		y=SPI2->DR;
		SPI2->DR=0x55AA;
		while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
		y=SPI2->DR;
		Pin_OutH(PIN_SPI2_NSS);
		SoftDelayUS(1);
		//读
		for(x=0;x<16;x++)
		{
			Pin_OutL(PIN_SPI2_NSS);
			SPI2->DR=0x8008;
			while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
			y=SPI2->DR;
			SPI2->DR=0;
			while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
			y=SPI2->DR;
			if(y!=0x55AA)
			{
				BR+=2;
				if(BR>7)
				{
					BR=7;
				}
				i=8;
				break;
			}
			Pin_OutH(PIN_SPI2_NSS);
			SoftDelayUS(1);
		}
	}
	SPI2->CR1|=(0x7<<3);
	Err=0;
	BR=RCC_GetPCLK1Freq()/(2<<BR);
	if(BR<1200000)
	{
		Err=1;
	}
//复位引脚测试
	//写
	Pin_OutL(PIN_SPI2_NSS);
	y=SPI2->DR;
	SPI2->DR=0x08;
	while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
	y=SPI2->DR;
	SPI2->DR=0x55AA;
	while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
	y=SPI2->DR;
	Pin_OutH(PIN_SPI2_NSS);
	SoftDelayUS(1);
	//复位
	for(i=0;i<100;i++)
	{
		OffEMU();//关EMU(复位)
		SoftDelayUS(i);
		OnEMU();//打开EMU(复位解除)
		SoftDelayMS(50);//软件ms延时
		Pin_OutL(PIN_SPI2_NSS);
		SPI2->DR=0x8008;
		while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
		y=SPI2->DR;
		SPI2->DR=0;
		while((SPI2->SR&1)==0);//SPI（或 I2S）忙于通信或者发送缓冲区不为空
		y=SPI2->DR;
		Pin_OutH(PIN_SPI2_NSS);
		SoftDelayUS(1);
		if(y==0)
		{
			break;
		}
	}
	if(i>=100)
	{
		Err=1;
	}
	
	//存储参数
	MWR(BR,ADDR_DATABUFF,4);
	MWR(i,ADDR_DATABUFF,1);
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_90E36_Characteristics),5);
	
//结果
	if(Err)
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













