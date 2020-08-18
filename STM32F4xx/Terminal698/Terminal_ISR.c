
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../STM32F4xx/STM32F4xx_IO.h"


#if (USER/100)==0//是电科院测试
#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#define NUM_ImpIn_Sample_Valid    7//脉冲输入连续采样有效次数，3或5或7
void ImpIn_ISR(void)//脉冲输入
{
	#if (NUMmax_IMPIN>=1)
	u32 i;
	u32 impC;
	u32 impL;
	u32 impH;
	u8 *p8;
	u16 *p16;
	
	p8=(u8*)(ADDR_Terminal_impIn_Ctrl);
	p16=(u16*)(ADDR_Terminal_impIn_Count);
	
	i=Pin_Read(PIN_SWIN3);//读引脚,入口引脚使用名;返回0或1
	impC=p8[0];
	impL=impC&0x07;
	impH=(impC>>3)&0x07;
	impC&=0x80;
	if(i!=0x0)
	{
		impH+=1;
	}
	else
	{
		impL+=1;
	}
	if((impL+impH)<NUM_ImpIn_Sample_Valid)
	{
		p8[0]=(impL+(impH<<3))|impC;
	}
	else
	{
		if(impH>impL)
		{
			p8[0]=0x80;
		}
		else
		{
			if(impC==0x80)
			{
				i=p16[0];
				i++;
				p16[0]=i;
			}
			p8[0]=0;
		}
	}
	#endif
	#if (NUMmax_IMPIN>=2)
	i=Pin_Read(PIN_SWIN4);//读引脚,入口引脚使用名;返回0或1
	impC=p8[1];
	impL=impC&0x07;
	impH=(impC>>3)&0x07;
	impC&=0x80;
	if(i!=0x0)
	{
		impH+=1;
	}
	else
	{
		impL+=1;
	}
	if((impL+impH)<NUM_ImpIn_Sample_Valid)
	{
		p8[1]=(impL+(impH<<3))|impC;
	}
	else
	{
		if(impH>impL)
		{
			p8[1]=0x80;
		}
		else
		{
			if(impC==0x80)
			{
				i=p16[1];
				i++;
				p16[1]=i;
			}
			p8[1]=0;
		}
	}
	#endif
}
#endif
#endif

void TIM7_IRQHandler(void)//71 TIM7定时器10ms中断
{
	u32 i;
	u32 x;
	u16 *p16;


//脉冲输入
#if (USER/100)==0//是电科院测试
#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	ImpIn_ISR();//脉冲输入
#endif
#if ((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
	{
		ImpIn_ISR();//脉冲输入
	}
#endif
#endif
	
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
	
#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//跳闸输出
	#if (MaxControlRound>=1)
	if(Terminal_Ram->Trip1_MS_Timer>(1000/10))
	{
		Pin_OutH(PIN_TRIP1);//引脚输出高,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_TRIP1);//引脚输出低,入口引脚使用名
	}
	#endif
	#if (MaxControlRound>=2)
	if(Terminal_Ram->Trip2_MS_Timer>(1000/10))
	{
		Pin_OutH(PIN_TRIP2);//引脚输出高,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_TRIP2);//引脚输出低,入口引脚使用名
	}
	#endif
#endif//#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#if ((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//跳闸输出
	if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
	{
		#if (MaxControlRound>=1)
		if(Terminal_Ram->Trip1_MS_Timer>(1000/10))
		{
			Pin_OutH(PIN_TRIP1);//引脚输出高,入口引脚使用名
		}
		else
		{
			Pin_OutL(PIN_TRIP1);//引脚输出低,入口引脚使用名
		}
		#endif
		#if (MaxControlRound>=2)
		if(Terminal_Ram->Trip2_MS_Timer>(1000/10))
		{
			Pin_OutH(PIN_TRIP2);//引脚输出高,入口引脚使用名
		}
		else
		{
			Pin_OutL(PIN_TRIP2);//引脚输出低,入口引脚使用名
		}
		#endif
	}
#endif//#if ((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

//GPRS LED
/*
	if(Terminal_Ram->GPRS_RxLED_MS_Timer==0)
	{
		Pin_OutL(PIN_DTR1_NSS2_SET);//引脚输出高,入口引脚使用名
	}
	else
	{
		Pin_OutH(PIN_DTR1_NSS2_SET);//引脚输出低,入口引脚使用名
	}
	if(Terminal_Ram->GPRS_TxLED_MS_Timer==0)
	{
		Pin_OutL(PIN_DSR1);//引脚输出高,入口引脚使用名
	}
	else
	{
		Pin_OutH(PIN_DSR1);//引脚输出低,入口引脚使用名
	}
*/
	
//ENET LED
	if(Terminal_Ram->ENETLED_MS_Timer==0)//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_ENET_ACT_LED);//引脚输出高,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_ENET_ACT_LED);//引脚输出低,入口引脚使用名
	}

#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//抄表485_1_LED(原级连485LED)
	//接收
	if(Terminal_Ram->RS485_1_RxLED_MS_Timer==0)//12 级联485发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485R1_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485R1_LED);//引脚输出高,入口引脚使用名
	}
	//发送
	if(Terminal_Ram->RS485_1_TxLED_MS_Timer==0)//13 级联485接收LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485T1_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485T1_LED);//引脚输出高,入口引脚使用名
	}
//抄表485_2_LED
	//接收
	if(Terminal_Ram->RS485_2_RxLED_MS_Timer==0)//10 RS485抄表发送LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485R2_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485R2_LED);//引脚输出高,入口引脚使用名
	}
	//发送
	if(Terminal_Ram->RS485_2_TxLED_MS_Timer==0)//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
	{
		Pin_OutH(PIN_RS485T2_LED);//引脚输出低,入口引脚使用名
	}
	else
	{
		Pin_OutL(PIN_RS485T2_LED);//引脚输出高,入口引脚使用名
	}
#endif

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

