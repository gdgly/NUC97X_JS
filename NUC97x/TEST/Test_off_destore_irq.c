
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../Calculate/Calculate.h"
#include "../NUC97x/NUC97x_AIC.h"



void Test_off_destore_irq(void)//测试打开和恢复IRQ中断
{
	u32 i;
	u32 Count;
	u16* p16;
	u32 irq;
	
	
	Count=0;
	DisplayClr();//清屏
	DisplayStringAutoUp(0,9,(u8*)"\x0""IRQ TEST");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//读引脚,入口引脚使用名;返回0或1
		{
			return;
		}
		#endif
		//关全部中断
		irq=off_irq();//关IRQ中断,返回:关闭前允许状态
		Comm_Ram->GPMSTimer=1000/10;
		for(i=0;i<100;i++)
		{
			SoftDelayMS(10);//软件ms延时
			if(Comm_Ram->GPMSTimer!=(1000/10))
			{
				DisplayStringAutoUp(0,9,(u8*)"off_irq      ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			}
		}
		//恢复全部中断
		destore_irq(irq);//恢复IRQ中断,入口:关闭前允许状态
		while(Comm_Ram->GPMSTimer)
		{
		}
		Count++;
		p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			p16[i]=0;
		}
		i=hex_bcd(Count);
		DisplayData_OffZero(i,8,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		LCDDataOut();//点阵数据输出到LCD屏
	}
}






