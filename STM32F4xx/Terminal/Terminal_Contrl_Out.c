
//终端控制-输出.继电器和LED
#include "Project.h"
#include "Terminal_Contrl_Out.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"




void BUZZCTRL(void)//蜂鸣器声音允许/禁止,=0禁止,!=0允许
{
	u32 i;
	u32 y;
	u8 *p8;
	
	if(DOTMIXPMODE->Task==0)//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
	{
		Comm_Ram->BUZZCTRL[8]=1;//允许
		return;
	}
	i=MRR(ADDR_AFN04F57,3);//声音告警允许／禁止标志位	BS24	3
	p8=(u8 *)(ADDR_TYMDHMS);
	y=p8[2];//时
	y=bcd_hex(y);
	y=1<<y;
	i&=y;
	if(i!=0x0)
	{//允许
		Comm_Ram->BUZZCTRL[8]=1;//允许
	}
	else
	{//禁止
		Comm_Ram->BUZZCTRL[8]=0;//禁止
	}
}

#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
void Init_Contrl_Pin(void)//初始化终端输出控制和LED引脚
{
}
void Terminal_Contrl_Out(void)//终端控制-输出.继电器和LED
{
	#if ((Project/100)>=2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	if(Comm_Ram->CheckLEDTimer)//24 上电全部LED检查显示 每10MS减1定时器
	{
		return;
	}
	BUZZCTRL();//蜂鸣器声音允许/禁止,=0禁止,!=0允许
//报警LED 指示
	//没报警
	Pin_OutH(PIN_ATTR_LED);
	
	#endif
}
#else
void Init_Contrl_Pin(void)//初始化终端输出控制和LED引脚
{
	Pin_Configure(PIN_ATTR_LED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
//	Pin_CFG(PIN_TRIP_LED1,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));//上电时检查指示LED故初始输出0(亮)
//	Pin_CFG(PIN_P208_TRIP_LED2,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));//上电时检查指示LED故初始输出0(亮)
//	Pin_CFG(PIN_P208_TRIP_LED3,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));//上电时检查指示LED故初始输出0(亮)
//	Pin_CFG(PIN_P208_TRIP_LED4,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));//上电时检查指示LED故初始输出0(亮)
//	Pin_CFG(PIN_P208_LED5,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));//上电时检查指示LED故初始输出0(亮)
//	Pin_CFG(PIN_P208_LED6,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));//上电时检查指示LED故初始输出0(亮)
//	Pin_CFG(PIN_TRIP_ATTR,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));
//	Pin_CFG(PIN_TRIP1,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));
//	Pin_CFG(PIN_TRIP2,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));
//	Pin_CFG(PIN_P208_TRIP3,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));
//	Pin_CFG(PIN_P208_TRIP4,0+(0<<3)+(1<<5)+(1<<17)+(0<<18));
}

u32 Terminal_Contrl_Round(void)//终端当前设定的控制轮次(功控合电控);返回B0-B7位标志
{
	u32 i;
	u32 x;
	u8 *p8;
	
	//功控轮次设定
	MR(ADDR_DATABUFF,ADDR_AFN04F45,AFN04F45MaxPn);
	//电控轮次设定
	MR(ADDR_DATABUFF+AFN04F45MaxPn,ADDR_AFN04F48,AFN04F48MaxPn);
	p8=(u8 *)(ADDR_DATABUFF);
	x=0;
	for(i=0;i<(AFN04F45MaxPn+AFN04F48MaxPn);i++)
	{
		x|=p8[i];
	}
	return x;
}

void Terminal_Contrl_Out(void)//终端控制-输出.继电器和LED
{
	u32 i;
	u32 x;
	u32 y;
	u8 * p8;
//	u16 * p16;
	
	if(Comm_Ram->CheckLEDTimer)//24 上电全部LED检查显示 每10MS减1定时器
	{
		return;
	}
	
	BUZZCTRL();//蜂鸣器声音允许/禁止,=0禁止,!=0允许
//终端总跳闸合闸标志
	x=0;
	p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
	if(p8[0]==0x0)
	{//不保电
		p8=(u8 *)(ADDR_Terminal_Remote_State);//终端控制-遥控跳闸
		for(i=0;i<MaxControlRound;i++)
		{
			if(p8[0]>=102)
			{
				x|=1<<i;
			}
			p8+=Terminal_Ctrl_Pn_Len;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State);//终端控制-功控.当前功率下浮控
		for(i=0;i<(6*MaxTotalAddGroup);i++)
		{
			x|=p8[1];
			p8+=Terminal_Ctrl_Pn_Len;
		}
	}
	p8=(u8 *)(ADDR_Terminal_Total_State);//终端总跳闸
	y=p8[0];//y=原跳闸标志
	p8[0]=x;//x=当前跳闸标志
	i=x^y;
	i&=y;//当前合闸允许标志
	if(i!=0x0)
	{//有新合闸允许
		Terminal_Ram->EnSwitchON_S_Timer=60;//合闸允许通知延时60秒
		p8=(u8 *)(ADDR_Terminal_Total_State+1);//位标志(轮次1-8)允许合闸通知(总跳闸状态相应位转为合闸时置位,通知定时到后清0)
		p8[0]|=i;
	}
	i=x^y;
	i&=x;//当新跳闸标志
	if(i!=0x0)
	{//有新跳闸
		Terminal_Ram->EnSwitchON_S_Timer=0;//合闸允许通知延时秒
		Terminal_Ram->ReTrip_S_Timer=0;//8 终端补跳闸秒定时器
		Terminal_Ram->RoundTripFlags&=(~i);//终端轮次跳闸位标志,B0-B7=0没循跳,=1已循跳
	}
//终端总跳闸循跳
	p8=(u8*)(ADDR_Terminal_Total_State);//终端总跳闸
	i=p8[0];
	x=i&0x1;
	if(i&2)
	{
		x|=0x8;
	}
	p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
	i=p8[0];
	if(i!=0)
	{
		i=2;
	}
	p8=(u8*)(ADDR_ContrlUARTData);//控制模块输出输入数据
	p8[0]=x;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
	p8[1]=i;//要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯
/*
	if(Terminal_Ram->ReTrip_S_Timer==0x0)//8 终端补跳闸秒定时器
	{
		p16=(u16 *)(&Terminal_Ram->Trip1_MS_Timer);//2 终端跳闸1 MS定时器;1200ms,>1000跳,<1000合
		x=0;
		for(i=0;i<4;i++)
		{
			x|=p16[i];
		}
		if(x==0x0)
		{//上轮跳200ms和合1000ms定时完成
			p8=(u8 *)(ADDR_Terminal_Total_State);//终端总跳闸
			x=p8[0];//x=当前总跳闸状态(1跳闸,0允许合闸)
			//p8=(u8 *)(Terminal_Run_Count+9);//终端轮次跳闸位标志,B0-B7=0没循跳,=1已循跳
			y=Terminal_Ram->RoundTripFlags;//y=循跳位标志;0没循跳,=1已循跳
			if((y&0x0f)!=0x0f)
			{//4轮循跳没完
				y=~y;
				i=BitLoca(y);//计算第1个置位为1的位置(0-32)
				if(i==0x0)
				{//没循跳轮次错
					Terminal_Ram->RoundTripFlags=0;
				}
				else
				{
					i--;
					if(i>=MaxControlRound)
					{//最大轮次错
						Terminal_Ram->RoundTripFlags=0;
					}
					else
					{
						Terminal_Ram->RoundTripFlags|=1<<i;//已循跳位
						x&=1<<i;
						if(x!=0x0)
						{//是跳闸
							p16=(u16 *)(&Terminal_Ram->Trip1_MS_Timer);//2 终端跳闸1 MS定时器;1200ms,>1000跳,<1000合
							p16[i]=1500/10;
						}
					}
				}
			}
			else
			{//4轮循跳完
				Terminal_Ram->RoundTripFlags=0;
				Terminal_Ram->ReTrip_S_Timer=5*60;//终端补跳闸秒定时器,每5分循跳1次
			}
		}
	}
*/
	
//清合闸允许标志
	if(Terminal_Ram->EnSwitchON_S_Timer==0x0)//终端合闸允许通知秒定时器
	{
		p8=(u8 *)(ADDR_Terminal_Total_State+1);//位标志(轮次1-8)允许合闸通知(总跳闸状态相应位转为合闸时置位,通知定时到后清0)
		p8[0]=0;
	}

//功控LED指示
	x=0;
	p8=(u8 *)(ADDR_AFN05F9);//时段功控投入(总加组号)
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		if(MRR((u32)p8,1)!=0x0)
		{
			x=1;
		}
		p8+=3;
	}
	p8=(u8 *)(ADDR_AFN05F10);//厂休功控投入(总加组号)
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		if(MRR((u32)p8,1)!=0x0)
		{
			x=1;
		}
		p8+=1;
	}
	p8=(u8 *)(ADDR_AFN05F11);//营业报停功控投入(总加组号)
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		if(MRR((u32)p8,1)!=0x0)
		{
			x=1;
		}
		p8+=1;
	}
	p8=(u8 *)(ADDR_AFN05F12);//当前功率下浮控投入(总加组号)
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		if(MRR((u32)p8,1)!=0x0)
		{
			x=1;
		}
		p8+=9;
	}
	if(x!=0x0)
	{//投入
		//功控红灯亮--表示终端时段控、厂休控或当前功率下浮控至少一种控制投入
		p8=(u8*)(ADDR_ContrlUARTData);//控制模块输出输入数据
		p8[0]|=0x40;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
	}
	
//电控LED指示
	x=0;
	p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State);//终端控制-电控.月电控
	for(i=0;i<(2*MaxTotalAddGroup);i++)
	{
		if(p8[0]!=0x0)
		{
			x=1;
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=0x0)
	{//投入
		//电控红灯亮--表示终端购电控或月电控投入
		p8=(u8*)(ADDR_ContrlUARTData);//控制模块输出输入数据
		p8[0]|=0x80;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
	}

//跳闸红和绿LED 指示
	x=0;//功控轮次设定,b0-b7
	MR(ADDR_DATABUFF,ADDR_AFN04F45,AFN04F45MaxPn);//功控轮次设定(总加组号)
	p8=(u8 *)(ADDR_DATABUFF);
	for(i=0;i<AFN04F45MaxPn;i++)
	{
		x|=p8[i];
	}
	y=0;//电控轮次设定,b0-b7
	MR(ADDR_DATABUFF,ADDR_AFN04F48,AFN04F48MaxPn);//电控轮次设定(总加组号)
	for(i=0;i<AFN04F48MaxPn;i++)
	{
		y|=p8[i];
	}
	x|=y;//控制轮次设定,b0-b7
	
	p8=(u8 *)(ADDR_Terminal_Total_State);//终端总跳闸
	i=p8[0];
	p8=(u8*)(ADDR_ContrlUARTData);//控制模块输出输入数据
	if((i&0x1)!=0x0)
	{//跳
		//控制轮次红灯亮--表示终端相应轮次处于拉闸状态
		p8[0]|=2;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
	}
	else
	{//没跳
		if((p8[2]&0x03)!=0x03)
		{
			//控制轮次绿灯亮--表示终端相应轮次的跳闸回路正常，具备跳闸条件
			p8[0]|=4;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
		}
		else
		{
			if(x&1)
			{//轮次1投入
			//灯红一秒绿一秒交替闪烁表示控制回路开关接入异常,灯灭表示该轮次未投入控制
				if((Comm_Ram->TYMDHMS[0]&1)==0)
				{
					p8[0]|=2;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
				}
				else
				{
					p8[0]|=4;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
				}
			}
		}
	}
	if((i&0x2)!=0x0)
	{//跳
		//控制轮次红灯亮--表示终端相应轮次处于拉闸状态
		p8[0]|=0x10;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
	}
	else
	{//没跳
		if((p8[2]&0x0c)!=0x0c)
		{
			//控制轮次绿灯亮--表示终端相应轮次的跳闸回路正常，具备跳闸条件
			p8[0]|=0x20;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
		}
		else
		{
			if(x&2)
			{//轮次2投入
			//灯红一秒绿一秒交替闪烁表示控制回路开关接入异常
				if((Comm_Ram->TYMDHMS[0]&1)==0)
				{
					p8[0]|=0x10;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
				}
				else
				{
					p8[0]|=0x20;//要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
				}
			}
		}
	}


//终端报警
	x=0;//报警标志;11=崔费告警声,12=允许合闸声,13=功控告警声,14=电控告警声,15=遥控跳闸
	//崔费告警
	p8=(u8 *)(ADDR_Terminal_Attr_State);//催费告警控制状态
	p8[0]=0;
	if(MRR(ADDR_AFN05F26,1)==0x55)
	{
		//催费告警允许／禁止标志位	BS24	3
		i=MRR(ADDR_AFN04F23,3);//读存贮器数据到寄存器,Byte<=8
		p8=(u8 *)(ADDR_TYMDHMS);//终端控制-电控.月电控
		y=p8[2];//时
		y=bcd_hex(y);
		y=1<<y;
		i&=y;
		if(i!=0x0)
		{//告警
			x=11;//11=崔费告警声
			p8=(u8 *)(ADDR_Terminal_Attr_State);//催费告警控制状态
			p8[0]=1;
		}
		else
		{
			BUZZ_Cancel(11);//11=崔费告警声
		}
	}
	else
	{
		BUZZ_Cancel(11);//11=崔费告警声
	}
	//允许合闸通知
	if(Terminal_Ram->EnSwitchON_S_Timer!=0x0)
	{
		x=12;//12=允许合闸声
	}
	else
	{
		BUZZ_Cancel(12);//12=允许合闸声
	}
	//功控告警
	p8=(u8 *)(ADDR_Terminal_kWCtrl1_State);//终端控制-功控
	for(i=0;i<(4*MaxTotalAddGroup);i++)
	{
		if(p8[0]==100)
		{
			x=13;//13=功控告警声
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=13)
	{
		BUZZ_Cancel(13);//13=功控告警声
	}
	//电控告警
	p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State);//终端控制-电控.月电控
	for(i=0;i<(2*MaxTotalAddGroup);i++)
	{
		if(p8[0]==100)
		{
			x=14;//14=电控告警声
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=14)
	{
		BUZZ_Cancel(14);//14=电控告警声
	}
	//遥控跳闸
	p8=(u8 *)(ADDR_Terminal_Remote_State);//终端控制-遥控跳闸
	for(i=0;i<MaxControlRound;i++)
	{
		if(p8[0]==100)
		{
			x=15;//15=遥控跳闸告警声
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=15)
	{
		BUZZ_Cancel(15);//15=控跳闸告警声
	}
	//
	if(x!=0x0)
	{//有告警
		if(x==11)
		{//催费告警
			BUZZ_Generate(x);//蜂鸣产生,入口蜂鸣号
		}
		else
		{
			i=MRR(ADDR_AFN04F57,3);//声音告警允许／禁止标志位	BS24	3
			p8=(u8 *)(ADDR_TYMDHMS);
			y=p8[2];//时
			y=bcd_hex(y);
			y=1<<y;
			i&=y;
			if(i!=0x0)
			{//告警
				BUZZ_Generate(x);//蜂鸣产生,入口蜂鸣号
				p8=(u8*)(ADDR_ContrlUARTData);//控制模块输出输入数据
				p8[1]|=0x1;//要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯
			}
		}
	}

//报警LED 指示
	if(x==12)
	{
		x=0;//12=允许合闸不指示报警
	}
	if(x!=0x0)
	{//报警
		p8=(u8 *)(ADDR_TYMDHMS);
		if(p8[0]&0x1)
		{
			Pin_OutL(PIN_ATTR_LED);
		}
		else
		{
			Pin_OutH(PIN_ATTR_LED);
		}
	}
	else
	{//没报警
		Pin_OutH(PIN_ATTR_LED);
	}
}

#endif//#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块

