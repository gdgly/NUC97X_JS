
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_IO.h"


void TIM7_IRQHandler(void)//71 TIM7定时器10ms中断
{
	u32 i;
	u32 x;
	u16 *p16;


//10ms定时器
	p16=(u16*)(ADDR_Terminal_MS_Timer);
	for(i=0;i<Terminal_MS_Timer_Num;i++)
	{
		x=p16[i];
		if(x!=0x0)
		{
			x--;
			p16[i]=x;
		}
	}
	

//GPRS ONOFF脚控制,=1输出高,0=不变
	if(Terminal_Ram->GPRS_ONOFF_MS_Timer==1)//15 GPRS ONOFF脚控制 10MS定时器
	{
		Pin_OutH(PIN_ON_GPRS);//引脚输出高,入口引脚使用名
	}
}


void RTC_Alarm_IRQHandler(void)//;57 RTC Alarm (A and B) through EXTI Line
{
	u32 i;
	u32 x;
	u16 *p16;
	
	p16=(u16*)(ADDR_Terminal_S_Timer);
	for(i=0;i<Terminal_S_Timer_Num;i++)
	{
		x=p16[i];
		if(x!=0x0)
		{
			x--;
			p16[i]=x;
		}
	}
	Terminal_Ram->Terminal_S_Count++;//终端秒加计数,用于其他秒定时器
	if(Terminal_Ram->S_PR==0x0)
	{//1 60秒分频产生分
		Terminal_Ram->S_PR=60;
		p16=(u16*)(ADDR_Terminal_M_Timer);
		for(i=0;i<Terminal_M_Timer_Num;i++)
		{
			x=p16[i];
			if(x!=0x0)
			{
				x--;
				p16[i]=x;
			}
		}
		Terminal_Ram->Terminal_M_Count++;//终端分加计数,用于其他分定时器
	}
	
//
	p16=(u16*)(ADDR_RUNSPEEDTEST);//循环运行速度测试(半字对齐)
	i=p16[0];
	p16[0]=0;
	p16[1]=i;
}





void I2_GPIO_ISR(void)//I2 GPIO中断服务
{
/*
//TESAMIO中断
	LPC_GPIOINT->IO0IntClr=(1<<(PIN_TESAMIO&0x1f));//清中断标志
*/
}

