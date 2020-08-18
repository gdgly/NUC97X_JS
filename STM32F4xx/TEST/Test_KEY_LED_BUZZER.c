
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../Device/Button.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/IC_BUZZ.h"
#include "../../NUC97x/NUC97x/NUC97x_AIC.h"



const u16 Pin_LED_LIST[]=
{
#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
	PIN_RUNLED,
	PIN_ATTR_LED,
	PIN_ENET_LINK_LED,
	PIN_ENET_ACT_LED,
	PIN_RS485R1_LED,
	PIN_RS485T1_LED,
	PIN_RS485R2_LED,
	PIN_RS485T2_LED,
#else
	PIN_ATTR_LED,
	PIN_GPRSRX_LED,
	PIN_RS485T1_LED,
	PIN_LED_ONLINE,
	PIN_WEAK_SINGLE,
	PIN_STRONG_SINGLE,
	PIN_RS485R1_LED,
	PIN_GPRSTX_LED,
#endif
};



u32 Test_KEY_LED_BUZZER(u32 ADDR_BASE)
{
	u32 i;
	u16 *p16;

	__enable_irq();//允许中断
	DisplayStringAutoUp(0,9,(u8*)"\x0""LED KEY BUZZER");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""         \x1e");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""      \x11 \x11 \x10 \x10");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""         \x1f");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(6*2));
	p16[0]=62;
	p16[6]=61;
	LCDDataOut();//点阵数据输出到LCD屏
	
//按键从按下(0)返回放开(1)认为OK
	//Comm_Ram->TESTKEY=0;
//BUZZER
	#ifdef PIN_BUZZ
	Pin_Configure(PIN_BUZZ,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
	Comm_Ram->BUZZCTRL[8]=1;//蜂鸣器声音允许/禁止,=0禁止,!=0允许
	#endif
//LED
	for(i=0;i<(sizeof(Pin_LED_LIST)/2);i++)
	{
		Pin_Configure(Pin_LED_LIST[i],//引脚配置
								0|//B3-B0:复用功能选择AF(0-15)
					 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (1<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
	}
	Comm_Ram->GPMSTimer=500/10;//LED定时
	Comm_Ram->BattWork_S_timer=120;//等待按键操作总定时
	while(1)
	{
		if(Comm_Ram->BattWork_S_timer==0)//等待按键操作总定时
		{
			MWR(1,ADDR_BASE+(OFFSET_KEY_Characteristics),1);
			return 1;//按键错误
		}
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		Button();//按键输入
		i=Comm_Ram->ButtonCurrent;
		i&=0x3f;
		i=BitNum(i);//计算置位为1的个数(0-32)
		if(i<=1)
		{
			if((Comm_Ram->TESTKEY&0x3f)==0x3f)
			{
				MC(0,ADDR_BASE+(OFFSET_KEY_Characteristics),1);
				break;
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-1)*84)+4+(9*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&1)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-3)*84)+4+(9*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&2)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(8*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&4)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(10*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&8)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(12*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&0x10)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
			}
			p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(6*2));
			p16[0]&=0x7fff;
			if(Comm_Ram->ButtonCurrent&0x20)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
			{
				p16[0]|=0x8000;
				BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
			}
			LCDDataOut();//点阵数据输出到LCD屏
		}
		if(Comm_Ram->GPMSTimer==0)
		{
			Comm_Ram->GPMSTimer=500/10;
			if(Pin_Read(PIN_RUNLED))
			{
				for(i=0;i<(sizeof(Pin_LED_LIST)/2);i++)
				{
					Pin_OutL(Pin_LED_LIST[i]);
				}
			}
			else
			{
				for(i=0;i<(sizeof(Pin_LED_LIST)/2);i++)
				{
					Pin_OutH(Pin_LED_LIST[i]);
				}
			}
		}
	}
	MWR(0,ADDR_BASE+(OFFSET_KEY_Characteristics),1);
	return 0;
}

















