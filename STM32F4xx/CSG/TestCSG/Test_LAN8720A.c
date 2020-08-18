

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../Device/LAN8720A.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"



u32 Test_LAN8720A(u32 ADDR_BASE)//LAN8720A测试
{
	u32 i;
	u32 SMI;
	u32 LINK;
	u32 REF_CLK;
	u32 tRESET;
//	u32 x;
//	u64 d64;
	u8 *p8;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""LAN8720Ai");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	OnE5V0();//打开E5V0电源
	SoftDelayMS(50);//
	Init_STM32F4xx_MAC();
	Init_LAN8720A();
//	SoftDelayMS(1);//
//Serial Management Interface (SMI)
	i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);//读ETH_PHY寄存器
	if(i==7)
	{
		SMI=0;
	}
	else
	{
		SMI=1;
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	}
//Auto-Negotiation
	__enable_irq();//允许中断
	LINK=0;
	Comm_Ram->GPMSTimer=5000/10;
	while(1)
	{
		SoftDelayMS(50);//软件ms延时
		i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);//读ETH_PHY寄存器
		if((i&((1<<2)+(1<<5)))==((1<<2)+(1<<5)))//bit2 link is up,bit5 auto-negotiate process completed
		{
			i=i;
			break;
		}
		if(Comm_Ram->GPMSTimer==0)
		{
			LINK=1;
			TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
			break;
		}
	}
	
//REF_CLK(因定时器输入时钟频率较低故无法测准50MHz时钟,只能测有无时钟输入)
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	Pin_Configure(PIN_ETH_REF_CLK,
				2|//B3-B0:复用功能选择AF(0-15)
	 (2<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
	 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
	 (2<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
	 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
	(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	START_STM32F4xx_TIM5_MEASURE_REF_CLK();//启动定时器TIM5测量LAN8720A的参考时钟
	Comm_Ram->GPMSTimer=5000/10;
	REF_CLK=0;//0=OK,1=ERR,0xff=没测试
	while((TIM5->SR&(1<<2))==0)//位 2 CC2IF：捕获/比较 2 中断标志 (Capture/Compare 2 interrupt flag)
	{
		if(Comm_Ram->GPMSTimer==0)
		{
			REF_CLK=1;//0=OK,1=ERR,0xff=没测试
			TestResult=1;
			break;
		}
	}
//tRESET
	for(tRESET=110;tRESET!=0;tRESET--)
	{
		Write_ETH_PHYReg(PHYAddr_LAN8720A_0,2,0x0);//写ETH_PHY寄存器
		Pin_OutL(PIN_ETH_RST);
		SoftDelayUS(tRESET);//软件us延时
		Pin_OutH(PIN_ETH_RST);
		i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);//读ETH_PHY寄存器
		if(i!=7)
		{
			break;
		}
	}
	if(tRESET==0)
	{
		tRESET=1;//至少1us
	}
	if(tRESET>=110)
	{
		TestResult=1;
	}
	if(TestResult==0)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	//存储参数
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=SMI;
	p8[1]=LINK;
	p8[2]=REF_CLK;
	p8[3]=tRESET;
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_LAN8720A_Characteristics),4);
	return TestResult;
}



