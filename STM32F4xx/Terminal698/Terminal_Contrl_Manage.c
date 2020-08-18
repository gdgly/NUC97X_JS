
//终端控制管理
#include "Project.h"
#include "Terminal_Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_Contrl_Remote.h"
#include "Terminal_Contrl_Power.h"
#include "Terminal_Contrl_Energy.h"
#include "Terminal_Contrl_Out.h"







void Init_Contrl_Command(void)//终端控制数据初始化(命令,全部清除)
{
	MC(0,ADDR_GPRSTimerTaskNotes,6*MaxTimerTask*2);//GPRS通道定时任务上报记录
	MC(0,ADDR_ENETTimerTaskNotes,6*MaxTimerTask*2);//ENET通道定时任务上报记录
	MC(0,ADDR_RS232TimerTaskNote,6*MaxTimerTask*2);//RS232通道定时任务上报记录
}



void Terminal_Contrl_Protect(void)//终端控制-保电
{
	u32 i;
	u32 x;
	u8 * p8;
	
	i=0;
	x=MRR(ADDR_AFN05F25,1);
	if(x==0x55)//终端保电投入解除
	{
		x=MRR(ADDR_AFN05F25+1,1);
		if(x!=0x0)
		{//定时保电
			i=x*30;//0.5h
			MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+0,2);//写寄存器数据到存贮器,Byte<=8
			MC(0x5A,ADDR_AFN05F25,1);//存贮器清0
		}
		i=1;
	}
	else
	{
		if(x==0x5A)
		{//进入保电定时
			if(MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+0,2)==0x0)
			{
				MC(0x0,ADDR_AFN05F25,1);//存贮器清0
			}
			else
			{
				i=1;
			}
		}
	}
	if(i==0x0)
	{
		x=MRR(ADDR_AFN04F58,1);//终端自动保电参数
		if(x!=0x0)
		{//自动保电
			if(Terminal_Ram->DisconAutoProtect_M_Timer==0x0)//自动保电允许与主站连续无通信时间分定时器
			{
				i=2;
			}
		}
	}
	p8=(u8 *)(ADDR_Terminal_Protect);
	p8[0]=i;
}

void Terminal_Time_Period(void)//终端费率时段和功控时段
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	u8 * p8d;
	u8 * p8rtc;

//终端费率时段
	p8rtc=(u8 *)(ADDR_TYMDHMS);
	i=p8rtc[2];
	i=bcd_hex(i);
	i*=2;
	if(p8rtc[1]>=0x30)
	{
		i++;
	}
	i=MRR(ADDR_AFN04F21+i,1);//取值0～47依次表示费率1～费率48，其他值无效
	i++;
	if(i>TMaxTariff)
	{
		i=0;
	}
	p8d=(u8 *)(ADDR_Terminal_Period);//终端费率时段和功控时段
	p8d[0]=i;
//功控时段
	//当前RTC的时分按30分计数
	n=p8rtc[2];
	n=bcd_hex(n);
	n*=2;
	if(p8rtc[1]>=0x30)
	{
		n++;
	}
	n++;//0点算1个30分
	x=MRR(ADDR_AFN04F18,1);//终端功控时段
	x&=0x3;//原控制代码
	y=1;//时段号从1开始(相当于I值)
	for(i=0;i<n;i++)
	{
		z=MRR(ADDR_AFN04F18+(i/4),1);
		z>>=(i%4)*2;
		z&=0x3;
		if(z!=x)
		{//控制码不相同
			x=z;
			y++;//时段号+1
		}
	}
	if(y>8)
	{//只能1-8,>9作为0
		y=0;
	}
	p8d[2]=y;
	if(p8d[3]!=x)
	{//控制代码变化
		p8d[3]=x;
		//控制时段变化告警
		BUZZ_Generate(3);//蜂鸣产生,入口蜂鸣号
		//清原时段功控
		MC(0,ADDR_Terminal_kWCtrl4_State,Terminal_Ctrl_Pn_Len*MaxTotalAddGroup);
	}
}









void Terminal_Contrl_Manage(void)//终端控制管理(分任务运行)
{
	
	Terminal_Contrl_Protect();//终端控制-保电
	switch(Terminal_Ram->I2SUBTask5)
	{
		case 0:
			Terminal_Time_Period();//终端费率时段和功控时段
			break;
		case 1:
			Terminal_Remote_Control();//终端控制-遥控跳闸
			break;
		case 2:
			Terminal_Contrl1();//终端控制-功控.当前功率下浮控
			break;
		case 3:
			Terminal_Contrl2();//终端控制-功控.营业报停功控
			break;
		case 4:
			Terminal_Contrl3();//终端控制-功控.厂休功控
			break;
		case 5:
			Terminal_Contrl4();//终端控制-功控.时段功控
			break;
		case 6:
			Terminal_Contrl_MonthEnergy();//终端控制-月电控
			break;
		case 7:
			Terminal_Contrl_BuyEnergy();//终端控制-购电控
			break;
		case 8:
			Terminal_Contrl_Out();//终端控制-输出.继电器和LED
			break;
	
		default:
			Terminal_Ram->I2SUBTask5=0;
			return;
	}
	Terminal_Ram->I2SUBTask5++;
}












