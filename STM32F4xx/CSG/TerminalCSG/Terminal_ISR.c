
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/IC_BUZZ.h"

#include "../QCSG_Head/QCSG_Hal.h"

void TerminalTaskTimeOutCount(void);


void I2_ImpIn_ISR(void)//脉冲输入
{
	#if (MaxImpInPort>=1)
	u32 i;
	u32 x;
	u8 *p8;
	u16 *p16;
	
	p8 = (u8*)(ADDR_Terminal_impIn_Ctrl);
	p16 = (u16*)(ADDR_Terminal_impIn_Count);
	i = Pin_Read(PIN_P208_SIN3);//读引脚,入口引脚使用名;返回0或1
	if(i != 0x0)
	{
		p8[0] = 0;
	}
	else
	{
		i = p8[0];
		if(i < 2)
		{
			i++;
			if(i == 2)
			{
				x = p16[0];
				x++;
				p16[0] = x;
			}
			p8[0] = i;
		}
	}
	#endif
	#if (MaxImpInPort>=2)
	i=Pin_Read(PIN_P208_SIN4);//读引脚,入口引脚使用名;返回0或1
	if(i != 0x0)
	{
		p8[1] = 0;
	}
	else
	{
		i = p8[1];
		if(i < 2)
		{
			i++;
			if(i == 2)
			{
				x = p16[1];
				x++;
				p16[1] = x;
			}
			p8[1] = i;
		}
	}
	#endif
/*
	#if (MaxImpInPort>=3)
	i=Pin_Read(PIN_P208_SIN3);//读引脚,入口引脚使用名;返回0或1
	if(i!=0x0)
	{
		p8[2]=0;
	}
	else
	{
		i=p8[2];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[2];
				x++;
				p16[2]=x;
			}
			p8[2]=i;
		}
	}
	#endif
	#if (MaxImpInPort>=4)
	i=Pin_Read(PIN_P208_SIN4);//读引脚,入口引脚使用名;返回0或1
	if(i!=0x0)
	{
		p8[3]=0;
	}
	else
	{
		i=p8[3];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[3];
				x++;
				p16[3]=x;
			}
			p8[3]=i;
		}
	}
	#endif
	#if (MaxImpInPort>=5)
	i=Pin_Read(PIN_P208_SIN5);//读引脚,入口引脚使用名;返回0或1
	if(i!=0x0)
	{
		p8[4]=0;
	}
	else
	{
		i=p8[4];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[4];
				x++;
				p16[4]=x;
			}
			p8[4]=i;
		}
	}
	#endif
	#if (MaxImpInPort>=6)
	i=Pin_Read(PIN_P208_SIN6);//读引脚,入口引脚使用名;返回0或1
	if(i!=0x0)
	{
		p8[5]=0;
	}
	else
	{
		i=p8[5];
		if(i<2)
		{
			i++;
			if(i==2)
			{
				x=p16[5];
				x++;
				p16[5]=x;
			}
			p8[5]=i;
		}
	}
	#endif
*/
}

void TIM7_IRQHandler(void)//71 TIM7定时器10ms中断
{
	u32 i;
	u32 x;
	u16 *p16;
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4)||((Project/100)==9))
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
#endif
//脉冲输入第1次
	I2_ImpIn_ISR();//脉冲输入
	
//10ms定时器
	p16 = (u16*)(ADDR_Terminal_MS_Timer);
	for(i = 0; i < Terminal_MS_Timer_Num; i++)
	{
		x=p16[i];
		if(x!=0x0)
		{
			x--;
			p16[i]=x;
		}
	}

	pTerminal_Ram->Ticks++;

	
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)||((Project/100)==9)	//I型集中器、负荷管理终端、配变
	//ENET LED
	if(pTerminal_Ram->ENETLED_MS_Timer == 0)	// 14 以太网接收发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_ENET_ACT_LED);//引脚输出高,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_ENET_ACT_LED);//引脚输出低,入口引脚使用名
	}
	

	// 抄表485_1_LED(原级连485LED)
	// 接收
	if(pTerminal_Ram->RS485_1_RxLED_MS_Timer==0)//12 级联485发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485R1_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485R1_LED);//引脚输出高,入口引脚使用名
	}
	//发送
	if(pTerminal_Ram->RS485_1_TxLED_MS_Timer==0)//13 级联485接收LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485T1_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485T1_LED);//引脚输出高,入口引脚使用名
	}

	//抄表485_2_LED
	//接收
	if(pTerminal_Ram->RS485_2_RxLED_MS_Timer==0)//10 RS485抄表发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485R2_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485R2_LED);//引脚输出高,入口引脚使用名
	}
	//发送
	if(pTerminal_Ram->RS485_2_TxLED_MS_Timer==0)//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485T2_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485T2_LED);//引脚输出高,入口引脚使用名
	}

	//蜂鸣器毫秒定时器
	if(pCommRam->BUZZCTRL[8] == 0)
	{
		if(pTerminal_Ram->BUZZMSTimer == 0)
		{
			 Pin_OutL(PIN_BUZZ);
		}
		else
		{
			Pin_OutH(PIN_BUZZ);
		}
	}
#endif

	
#if (Project/100)==5   //II型集中器
	// 抄表485_1_LED(原级连485LED)
	// 接收
	if(pTerminal_Ram->RS485_1_RxLED_MS_Timer==0)//12 级联485发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485R1_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485R1_LED);//引脚输出高,入口引脚使用名
	}
	//发送
	if(pTerminal_Ram->RS485_1_TxLED_MS_Timer==0)//13 级联485接收LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485T1_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485T1_LED);//引脚输出高,入口引脚使用名
	}
	
//此处屏蔽掉，2路485与1路485使用同一个毫秒定时器ybzyl20180601	
//	if(pTerminal_Ram->RS485_2_RxLED_MS_Timer==0)//10 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
//	{
//		Pin_OutH(PIN_RS485R1_LED);//引脚输出低,入口引脚使用名
//	}
//	else
//	{
//		Pin_OutL(PIN_RS485R1_LED);//引脚输出高,入口引脚使用名
//	}
//	//发送
//	if(pTerminal_Ram->RS485_2_TxLED_MS_Timer==0)//11 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
//	{
//		Pin_OutH(PIN_RS485T1_LED);//引脚输出低,入口引脚使用名
//	}
//	else
//	{
//		Pin_OutL(PIN_RS485T1_LED);//引脚输出高,入口引脚使用名
//	}
	
	//GPRS数据接收发送指示灯
	//接收
	if(pTerminal_Ram->GPRS_RxLED_MS_Timer==0)//GPRS接收LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_GPRSRX_LED);
	}
	else
	{
		Pin_OutL(PIN_GPRSRX_LED);
	}
	//发送
	if(pTerminal_Ram->GPRS_TxLED_MS_Timer==0)//GPRS发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_GPRSTX_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_GPRSTX_LED);//引脚输出高,入口引脚使用名
	}
	
	//GPRS在线指示灯
	if(pTerminal_Ram->GPRS_ONLINELED_MS_Timer==0)
	{
		Pin_OutH(PIN_LED_ONLINE);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_LED_ONLINE);//引脚输出高,入口引脚使用名
	}
	
	//GPRS信号最弱指示灯
	if(pTerminal_Ram->GPRS_WEAK_SINGLE_LED_MS_Timer==0)
	{
		Pin_OutH(PIN_WEAK_SINGLE);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_WEAK_SINGLE);//引脚输出高,入口引脚使用名
	}
	
	//GPRS信号最强指示灯
	if(pTerminal_Ram->GPRS_STRONG_SINGLE_LED_MS_Timer==0)
	{
		Pin_OutH(PIN_STRONG_SINGLE);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_STRONG_SINGLE);//引脚输出高,入口引脚使用名
	}

	
#endif

	
//脉冲输入第2次
	I2_ImpIn_ISR();//脉冲输入

//GPRS ONOFF脚控制,=1输出高,0=不变
	if(pTerminal_Ram->GPRS_ONOFF_MS_Timer == 1)//15 GPRS ONOFF脚控制 10MS定时器
	{
		Pin_OutH(PIN_ON_GPRS);//引脚输出高,入口引脚使用名
	}
}


void RTC_Alarm_IRQHandler(void)//;57 RTC Alarm (A and B) through EXTI Line
{
	u32 i;
	u32 x;
	u16 *p16;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	//Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	//HAL_PinReverse(PIN_RUNLED);
	Pin_OutL(PIN_RUNLED);
	//Pin_OutH(PIN_ATTR_LED);
	p16 = (u16*)(ADDR_Terminal_S_Timer);
	for(i = 0; i < Terminal_S_Timer_Num; i++)
	{
		x = p16[i];
		if(x != 0x0)
		{
			x--;
			p16[i] = x;
		}
	}
	pTerminalRam->Terminal_S_Count++;			// 终端秒加计数,用于其他秒定时器
	if(pTerminalRam->S_PR == 0x0)
	{ 											// 60秒分频产生分
		pTerminalRam->S_PR = 60;
		p16=(u16*)(ADDR_Terminal_M_Timer);
		for(i = 0; i < Terminal_M_Timer_Num; i++)
		{
			x = p16[i];
			if(x != 0x0)
			{
				x--;
				p16[i] = x;
			}
		}
		pTerminalRam->Terminal_M_Count++;		// 终端分加计数,用于其他分定时器
	}
	//TerminalTaskTimeOutCount();
	pTerminalRam->Seconds++;
	if(pTerminalRam->RT_AMRTimeOut > 0) pTerminalRam->RT_AMRTimeOut--;
	if(pTerminalRam->AT_AMSTimeOut > 0) pTerminalRam->AT_AMSTimeOut--;
	
	p16 = (u16*)(ADDR_RUNSPEEDTEST);				// 循环运行速度测试(半字对齐)
	i = p16[0];
	p16[0] = 0;
	p16[1] = i;
}


void TerminalTaskTimeOutCount(void)
{
	u32 i = 0;

	UartTaskCtrlStruct* pUART_Task = (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl;
	for(i = 0; i < UART_TASK_NUM; i++)
	{		
		if(pUART_Task->WaitAckTimeOut != 0)
		{
			pUART_Task->WaitAckTimeOut--;
			if(pUART_Task->WaitAckTimeOut == 0)
			{
				pUART_Task->Flag |= UART_FLAG_WAIT_ACK_TIME_OUT;
			}
		}
		pUART_Task++;
	}


	
}


void I2_GPIO_ISR(void)//I2 GPIO中断服务
{

}

