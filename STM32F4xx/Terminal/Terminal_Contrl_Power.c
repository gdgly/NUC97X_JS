
//终端控制-功控;控制的优先级由高到低是当前功率下浮控、营业报停功控、厂休功控、时段功控
#include "Project.h"
#include "Terminal_Contrl_Power.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"








#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
void Terminal_Contrl1(void)//终端控制-功控.当前功率下浮控
{
}
void Terminal_Contrl2(void)//终端控制-功控.营业报停功控
{
}
void Terminal_Contrl3(void)//终端控制-功控.厂休功控
{
}
void Terminal_Contrl4(void)//终端控制-功控.时段功控
{
}
#else
u32 NextTrip(u32 Bin,u32 Pn)//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
{
	u32 i;
	
	i=MRR(ADDR_AFN04F45+Pn,1);//功控轮次设定(总加组号)
	Bin^=i;
	Bin&=i;
	Bin=BitLoca(Bin);//计算第1个置位为1的位置(0-32)
	if(Bin!=0x0)
	{
		if(Bin>MaxControlRound)
		{
			Bin=0;
		}
		else
		{
			Bin--;
			Bin=1<<Bin;
		}
	}
	return Bin;
}

u32 kWContrlSlipTime(u32 Pn)//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
{
	u32 i;
	i=MRR(ADDR_AFN04F43+Pn,1);//功率控制的功率计算滑差时间(范围：1～60)	BIN	min	1
	if(i==0)
	{
		i=1;
	}
	if(i>60)
	{
		i=60;
	}
	i*=60;
	return i;
}

u32 kWContrl_Comp(u32 FixedValue,u32 Pn)//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
{
	u32 i;
	u64 a64;
	u64 b64;
	u32 f;
	u16 * p16timer;
	
	p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
	f=0;//超限标志=0
	i=kWContrlSlipTime(Pn);//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
	p16timer[Pn]=i;//下次计算时间
	//当前功率
	i=MRR(ADDR_AFN0CF17+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
	a64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
	if((a64>>63)!=0x0)
	{//负
		a64=(~a64)+1;
	}
	//保安定值
	i=MRR(ADDR_AFN04F17,2);//读存贮器数据到寄存器,Byte<=8
	b64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
	if((b64>>63)!=0x0)
	{//负
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//当前功率>=保安定值
		b64=Data02_kW(FixedValue);//数据格式02转为单位统一为kw的hex补码,有3位小数
		if((b64>>63)!=0x0)
		{//负
			b64=(~b64)+1;
		}
		if(a64>=b64)
		{
			f=1;//超限
		}
	}
	return f;
}

u32 kWContrl1_Comp(u32 Pn)//终端控制-功控.当前功率下浮控功率比较,入口Pn=总加组号(0-7);返回0没超限,1超限
{
	u32 i;
	u64 a64;
	u64 b64;
	u32 f;
	u16 * p16timer;
	
	p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
	f=0;//超限标志=0
//	i=MRR(ADDR_AFN05F12+1,1);//当前功率下浮控定值滑差时间：数据范围：1～60
//	if(i==0)
//	{
//		i=1;
//	}
//	if(i>60)
//	{
//		i=60;
//	}
//	i*=60;
	i=kWContrlSlipTime(Pn);//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
	p16timer[Pn]=i;//下次计算时间
	//当前功率
	i=MRR(ADDR_AFN0CF17+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
	a64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
	if((a64>>63)!=0x0)
	{//负
		a64=(~a64)+1;
	}
	//保安定值
	i=MRR(ADDR_AFN04F17,2);//读存贮器数据到寄存器,Byte<=8
	b64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
	if((b64>>63)!=0x0)
	{//负
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//当前功率>=保安定值
		i=MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
		b64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
		if(a64>=b64)
		{
			f=1;//超限
		}
	}
	return f;
}


void Terminal_Contrl1(void)//终端控制-功控.当前功率下浮控
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F12);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl1_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0值表示投入但没控原因号1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		if(MRR(((u32)p8s),1)==0x55)
		{//刚投入
				i=MRR(((u32)p8s)+4,1)*30;//当前功率下浮控的控制时间0.5h
				MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);//写寄存器数据到存贮器,Byte<=8
				MC(0x5a,(u32)p8s,1);//0x55->0x5a
				MC(0xee,ADDR_kWCtrl1_FixedValue+(Pn*2),2);//清控制定值
				p8d[0]=0;
				p8d[1]=0;//跳闸轮次=0
				p8d[8]=0;//超定值标志=0
				goto NextPn;
		}
		if(MRR(((u32)p8s),1)!=0x5a)
		{//解除
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			p8d[1]=0;//跳闸轮次=0
			p8d[8]=0;//超定值标志=0
			goto NextPn;
		}
		i=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
		if(i==0x0)
		{//控制时间定时到
			p8d[4]=1;//!=0控制时间定时到
			goto NoCtrl;//不控制
		}

		p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
		if(p8[0]!=0x0)
		{//保电
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://上电或不控制
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 终端总加组1当前功率下浮控控后冻结秒定时器
				p16[Pn]=MRR(((u32)p8s)+3,1)*60;//控后总加有功功率冻结延时时间
				
				i=MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
				if(i!=0xeeee)
				{//定值已计算
					p8d[0]=2;
				}
				else
				{
					p8d[0]=1;
					i=MRR(((u32)p8s)+1,1);//当前功率下浮控定值滑差时间：数据范围：1～60
					if(i==0)
					{
						i=1;
					}
					if(i>60)
					{
						i=60;
					}
					i*=60;
					p16timer[Pn]=i;
					MC(0xee,ADDR_AFN0CF24+(Pn*2),2);//清控后总加有功功率冻结值
				}
				break;
			case 1://定值滑差时间内平均功率计算和定值计算
				if(p16timer[Pn]==0x0)
				{//滑差时间延时到
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
					a64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
					if((a64>>63)!=0x0)
					{//负
						a64=(~a64)+1;
					}
					i=MRR(((u32)p8s)+2,1);//当前功率下浮控定值浮动系数(定值浮动系数％ S0	BCD码十位（0-7）	BCD码个位;S0定义：S0=0，表示上浮；S0=1，表示下浮)
					i&=0x7f;
					i=bcd_hex(i);
					b64=(a64*i)/100;
					if((MRR(((u32)p8s)+2,1)>>7)==0x0)
					{//S0=0，表示上浮
						a64+=b64;
					}
					else
					{//S0=1，表示下浮
						a64-=b64;
						if((a64>>63)!=0x0)
						{//负值
							a64=0;
						}
					}
					i=kW_Data02(a64);//单位统一为kw有3位小数的hex补码转为数据格式02
					MWR(i,ADDR_kWCtrl1_FixedValue+(Pn*2),2);//写寄存器数据到存贮器,Byte<=8
					p8d[0]=2;
				}
				break;
			case 2:
				p8d[0]=3;
				break;
			case 3://控制
				//先检查控后总加有功功率冻结
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 终端总加组1当前功率下浮控控后冻结秒定时器
				if(p16[Pn]==0x0)
				{//控后总加有功功率冻结延时时间到
					i=MRR(ADDR_AFN0CF24+(Pn*2),2);
					if(i==0xeeee)
					{
						i=MRR(ADDR_AFN0CF17+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
						MWR(i,ADDR_AFN0CF24+(Pn*2),2);//控后总加有功功率冻结
					}
				}
				if(p16timer[Pn]==0x0)
				{//功率控制的功率计算滑差时间(总加组号)滑差时间延时到		
					x=kWContrl1_Comp(Pn);//终端控制-功控.当前功率下浮控功率比较,入口Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						p8d[8]=1;//超定值标志=0
						p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
						if(p8[0]!=0x0)
						{//保电
							break;
						}
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							if(i>4)
							{//当前功率下浮控只有4轮告警时间
								i=4;
							}
							i--;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=MRR(((u32)p8s)+5+i,1)*60;//第?轮告警时间
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//超定值标志=0
					}
				}
				break;
			case 100://报警
				//复查下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i==0x0)
				{//轮跳完,不再告警
					p8d[0]=3;
					break;
				}
				//先检查控后总加有功功率冻结
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 终端总加组1当前功率下浮控控后冻结秒定时器
				if(p16[Pn]==0x0)
				{//控后总加有功功率冻结延时时间到
					i=MRR(ADDR_AFN0CF24+(Pn*2),2);
					if(i==0xeeee)
					{
						i=MRR(ADDR_AFN0CF17+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
						MWR(i,ADDR_AFN0CF24+(Pn*2),2);//控后总加有功功率冻结
					}
				}
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=kWContrl1_Comp(Pn);//终端控制-功控.当前功率下浮控功率比较,入口Pn=总加组号(0-7);返回0没超限,1超限
					if(x==0x0)
					{//没超限
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//报警定时到
					p8d[0]=101;
				}
				break;
			case 101://跳闸准备
				//跳闸时功率
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//写寄存器数据到存贮器,Byte<=8
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				//先检查控后总加有功功率冻结
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 终端总加组1当前功率下浮控控后冻结秒定时器
				if(p16[Pn]==0x0)
				{//控后总加有功功率冻结延时时间到
					i=MRR(ADDR_AFN0CF24+(Pn*2),2);
					if(i==0xeeee)
					{
						i=MRR(ADDR_AFN0CF17+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
						MWR(i,ADDR_AFN0CF24+(Pn*2),2);//控后总加有功功率冻结
					}
				}
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//事件记录
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//跳闸2分后功率
					ERC6_Event(Pn+1,p8d[9],8,MRR(((u32)p8d)+2,2),i,MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2));//功控跳闸记录;入口Fn总加组号(从1开始),Bin跳闸轮次,Type功控类别,kw1跳闸前功率,kw2跳闸后2分钟功率,kw3跳闸时功率定值
					p8d[0]=103;
				}
				break;
			case 103://跳闸2分钟后
				p8d[0]=3;
				//功控滑差时间
				kWContrl1_Comp(Pn);//终端控制-功控.当前功率下浮控功率比较,入口Pn=总加组号(0-7);返回0没超限,1超限
				if(p16timer[Pn]>=120)
				{
					p16timer[Pn]-=120;//跳闸事件记录已延时2分钟
				}
				else
				{
					p16timer[Pn]=0;
				}
				break;
			default:
				goto NoCtrl;//不控制
		}
NextPn:
		p8s+=9;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

void Terminal_Contrl2(void)//终端控制-功控.营业报停功控
{
	u32 i;
	u32 x;
	u32 Pn;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F11);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl2_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0值表示投入但没控原因号1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//解除
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			p8d[1]=0;//跳闸轮次=0
			p8d[8]=0;//超定值标志=0
			goto NextPn;
		}

//#define ADDR_AFN04F44     ADDR_AFN04F43+(1*MaxTotalAddGroup)//营业报停控参数(总加组号)
//报停起始时间	数据格式20	日月年	3
//报停结束时间	数据格式20	日月年	3
//报停控功率定值	数据格式02	kW	2

		i=MRR(ADDR_TYMDHMS+3,3);//当前RTC年月日
		x=MRR(ADDR_AFN04F44+(8*Pn),3);//报停起始时间年月日
		if(i<x)
		{//当前RTC年月日<报停起始时间年月日
			p8d[4]=1;//!=0值表示投入但没控原因号1=没到起始时间
			goto NoCtrl;//不控制
		}
		x=MRR(ADDR_AFN04F44+(8*Pn)+3,3);//报停结束时间时间年月日
		if(i>=x)
		{//当前RTC年月日>=报停结束时间年月日
			p8d[4]=2;//!=0值表示投入但没控原因号2=已到结束时间
			goto NoCtrl;//不控制
		}
		//控制优先级1=当前功率下浮控
		i=MRR(ADDR_AFN05F12+(9*Pn),1);//当前功率下浮控
		if(i!=0x0)
		{
			p8d[4]=3;//!=0值表示投入但没控原因号3=有优先级高下浮控
			goto NoCtrl;//不控制
		}
		
		p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
		if(p8[0]!=0x0)
		{//保电
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				p16timer[Pn]=0;//功控功率计算秒定时器=0
				break;
			case 3://控制
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=MRR(ADDR_AFN04F44+(8*Pn)+6,2);//取定值
					x=kWContrl_Comp(x,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						p8d[8]=1;//超定值标志=0
						p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
						if(p8[0]!=0x0)
						{//保电
							break;
						}
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							i--;
							i=MRR((ADDR_AFN04F49+i),1);
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//第?轮告警时间
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//超定值标志=0
					}
				}
				break;
			case 100://报警
				//复查下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i==0x0)
				{//轮跳完,不再告警
					p8d[0]=3;
					break;
				}
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=MRR(ADDR_AFN04F44+(8*Pn)+6,2);//取定值
					x=kWContrl_Comp(x,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x==0x0)
					{//没超限
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//报警定时到
					p8d[0]=101;
				}
				break;
			case 101://跳闸准备
				//跳闸时功率
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//写寄存器数据到存贮器,Byte<=8
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//事件记录
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//跳闸2分后功率
					ERC6_Event(Pn+1,p8d[9],4,MRR(((u32)p8d)+2,2),i,MRR(ADDR_AFN04F44+(8*Pn)+6,2));//功控跳闸记录;入口Fn总加组号(从1开始),Bin跳闸轮次,Type功控类别,kw1跳闸前功率,kw2跳闸后2分钟功率,kw3跳闸时功率定值
					p8d[0]=103;
				}
				break;
			case 103://跳闸2分钟后
				p8d[0]=3;
				//功控滑差时间
				i=kWContrlSlipTime(Pn);//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
				if(i>=120)
				{
					i-=120;//跳闸事件记录已延时2分钟
				}
				else
				{
					i=0;
				}
				p16timer[Pn]=i;
				break;
			default:
				goto NoCtrl;//不控制
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

void Terminal_Contrl3(void)//终端控制-功控.厂休功控
{
	u32 i;
	u32 x;
	u32 Pn;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F10);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl3_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0值表示投入但没控原因号1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//解除
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			p8d[1]=0;//跳闸轮次=0
			p8d[8]=0;//超定值标志=0
			goto NextPn;
		}

//		p8=(u8 *)(Terminal_Period);//终端费率时段和功控时段
//		if(p8[3]==0x0)
//		{//0=当前功控时段号不控制,1=当前功控时段号控制
//			p8d[4]=1;//1=时段?不控制
//			goto NoCtrl;//不控制
//		}


//#define ADDR_AFN04F42     ADDR_AFN04F41+((1+(17*3))*MaxTotalAddGroup)//厂休功控参数(总加组号)
//厂休控定值	见附录A.2	kW	2
//限电起始时间	见附录A.19	分时	2
//限电延续时间（范围1～48）	BIN	0.5h	1
//每周限电日：D1～D7表示星期一～星期日，D0=0	BS8		1

		i=MRR(ADDR_AFN04F42+(6*Pn)+5,1);//每周限电日：D1～D7表示星期一～星期日，D0=0	BS8		1
		p8=(u8 *)(ADDR_TYMDHMS+6);
		x=p8[0];//取当前RTC星期
		if(x==0x0)
		{
			x=7;//星期日=7
		}
		x=1<<x;
		i&=x;
		if(i==0x0)
		{//当前周不是厂休
			p8d[4]=2;//2=不是厂休日
			goto NoCtrl;//不控制
		}
		i=MRR(ADDR_TYMDHMS+1,2);//当前RTC分时
		x=MRR(ADDR_AFN04F42+(6*Pn)+2,2);//限电起始时间分时
		if(i<x)
		{//当前RTC分时<限电起始时间分时
			p8d[4]=3;//3=不是限电时间
			goto NoCtrl;//不控制
		}
		//限电结束时间分时计算
		i=x&0xff;
		x=x>>8;
		x=bcd_hex(x);
		x*=60;
		i=bcd_hex(i);
		i+=x;
		x=MRR(ADDR_AFN04F42+(6*Pn)+4,1);//限电延续时间0.5h
		x*=30;
		i+=x;
		x=i/60;
		i%=60;
		x=hex_bcd(x);
		x<<=8;
		i=hex_bcd(i);
		x|=i;
		i=MRR(ADDR_TYMDHMS+1,2);//当前RTC分时
		if(i>=x)
		{//当前RTC分时>=限电结束时间分时
			p8d[4]=3;//3=不是限电时间
			goto NoCtrl;//不控制
		}
		
		//控制优先级1=当前功率下浮控
		i=MRR(ADDR_AFN05F12+(9*Pn),1);//当前功率下浮控
		if(i!=0x0)
		{
			p8d[4]=4;//4=有优先级高下浮控
			goto NoCtrl;//不控制
		}
		//控制优先级2=营业报停功控
		i=MRR(ADDR_AFN05F11+Pn,1);//营业报停功控
		if(i!=0x0)
		{
			p8d[4]=5;//5=有优先级高报停控
			goto NoCtrl;//不控制
		}

		p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
		if(p8[0]!=0x0)
		{//保电
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				p16timer[Pn]=0;//功控功率计算秒定时器=0
				break;
			case 3://控制
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=MRR(ADDR_AFN04F42+(6*Pn),2);//取定值
					x=kWContrl_Comp(x,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						p8d[8]=1;//超定值标志=0
						p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
						if(p8[0]!=0x0)
						{//保电
							break;
						}
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							i--;
							i=MRR((ADDR_AFN04F49+i),1);
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//第?轮告警时间
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//超定值标志=0
					}
				}
				break;
			case 100://报警
				//复查下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i==0x0)
				{//轮跳完,不再告警
					p8d[0]=3;
					break;
				}
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=MRR(ADDR_AFN04F42+(6*Pn),2);//取定值
					x=kWContrl_Comp(x,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x==0x0)
					{//没超限
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//报警定时到
					p8d[0]=101;
				}
				break;
			case 101://跳闸准备
				//跳闸时功率
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//写寄存器数据到存贮器,Byte<=8
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//事件记录
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//跳闸2分后功率
					ERC6_Event(Pn+1,p8d[9],2,MRR(((u32)p8d)+2,2),i,MRR(ADDR_AFN04F42+(6*Pn),2));//功控跳闸记录;入口Fn总加组号(从1开始),Bin跳闸轮次,Type功控类别,kw1跳闸前功率,kw2跳闸后2分钟功率,kw3跳闸时功率定值
					p8d[0]=103;
				}
				break;
			case 103://跳闸2分钟后
				p8d[0]=3;
				//功控滑差时间
				i=kWContrlSlipTime(Pn);//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
				if(i>=120)
				{
					i-=120;//跳闸事件记录已延时2分钟
				}
				else
				{
					i=0;
				}
				p16timer[Pn]=i;
				break;
			default:
				goto NoCtrl;//不控制
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

void Terminal_Contrl4(void)//终端控制-功控.时段功控
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F9);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl4_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0值表示投入但没控原因号1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//解除
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			p8d[1]=0;//跳闸轮次=0
			p8d[8]=0;//超定值标志=0
			goto NextPn;
		}

//时段功控投入标志：D0～D7按顺序对位表示第1～第8时段，置"1"：有效，置"0"：无效
//时段功控定值方案号：数值范围：0～2依次表示第1～第3套方案，其他值无效

		p8=(u8 *)(ADDR_Terminal_Period);//终端费率时段和功控时段
		if(p8[3]==0x0)
		{//0=当前功控时段号不控制,1或2当前功控时段号控制
			p8d[4]=1;//没控原因号2=当前时段不控制
			goto NoCtrl;//不控制
		}
		x=p8[2];//当前功控时段号1-8,=0表示时段号>9错误
		if(x==0x0)
		{
			p8d[4]=2;//没控原因号3=功控时段数超8
			goto NoCtrl;//不控制
		}
		x--;
		i=MRR(((u32)p8s)+1,1);
		i>>=x;
		i&=0x1;
		if(i==0x0)
		{//当前时段没投入
			p8d[4]=3;//没控原因号4=当前时段没投入
			goto NoCtrl;//不控制
		}
		i=MRR(((u32)p8s)+2,1);//时段功控定值方案号
		if(i>2)
		{//0～2依次表示第1～第3套方案，其他值无效
			p8d[4]=4;//没控原因号5=无效方案号
			goto NoCtrl;//不控制
		}
		p8d[5]=i;
		p8=(u8 *)(ADDR_AFN04F41+((1+(17*3))*Pn));//时段功控定值(总加组号)(存贮时3个方案的时段固定空间相隔)
		if((MRR(((u32)p8),1)&(1<<i))==0x0)
		{//无方案标志
			p8d[4]=5;//没控原因号6=无本方案定值
			goto NoCtrl;//不控制
		}
		p8+=1+(17*i);
		if((MRR(((u32)p8),1)&(1<<x))==0x0)
		{//无时段功率定值
			p8d[4]=6;//没控原因号7=本方案时段无定值
			goto NoCtrl;//不控制
		}
		p8+=1+(2*x);
		i=MRR((u32)p8,2);//取当前时段功率定值
		//终端时段功控定值浮动系数
		a64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
		if((a64>>63)!=0x0)
		{//负
			a64=(~a64)+1;
		}
		p8=(u8 *)(ADDR_AFN04F19);//终端时段功控定值浮动系数
		i=MRR(((u32)p8),1);//当前功率下浮控定值浮动系数(定值浮动系数％ S0	BCD码十位（0-7）	BCD码个位;S0定义：S0=0，表示上浮；S0=1，表示下浮)
		i&=0x7f;
		i=bcd_hex(i);
		b64=(a64*i)/100;
		if((MRR(((u32)p8),1)>>7)==0x0)
		{//S0=0，表示上浮
			a64+=b64;
		}
		else
		{//S0=1，表示下浮
			a64-=b64;
		}
		i=kW_Data02(a64);//单位统一为kw有3位小数的hex补码转为数据格式02
		MWR(i,((u32)p8d)+6,2);//写寄存器数据到存贮器,Byte<=8
		
		//控制优先级1=当前功率下浮控
		p8=(u8 *)(ADDR_AFN05F12+(9*Pn));//当前功率下浮控
		if(MRR(((u32)p8),1)!=0x0)
		{
			p8d[4]=7;//没控原因号1=有优先级高下浮控
			goto NoCtrl;//不控制
		}
		//控制优先级2=营业报停功控
		p8=(u8 *)(ADDR_AFN05F11+Pn);//营业报停功控
		if(MRR(((u32)p8),1)!=0x0)
		{
			p8d[4]=8;//没控原因号1=有优先级高报停控
			goto NoCtrl;//不控制
		}
		//控制优先级3=厂休功控
		p8=(u8 *)(ADDR_AFN05F10+Pn);//厂休功控
		if(MRR(((u32)p8),1)!=0x0)
		{
			p8d[4]=9;//没控原因号1=有优先级高厂休控
			goto NoCtrl;//不控制
		}

		p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
		if(p8[0]!=0x0)
		{//保电
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				p16timer[Pn]=0;//功控功率计算秒定时器=0
				break;
			case 3://控制
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=MRR(((u32)p8d)+6,2);//取定值
					x=kWContrl_Comp(x,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						p8d[8]=1;//超定值标志=0
						p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
						if(p8[0]!=0x0)
						{//保电
							break;
						}
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							i--;
							i=MRR((ADDR_AFN04F49+i),1);
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//第?轮告警时间
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//超定值标志=0
					}
				}
				break;
			case 100://报警
				//复查下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i==0x0)
				{//轮跳完,不再告警
					p8d[0]=3;
					break;
				}
				if(p16timer[Pn]==0x0)
				{//滑差时间到
					x=MRR(((u32)p8d)+6,2);//取定值
					x=kWContrl_Comp(x,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x==0x0)
					{//没超限
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//报警定时到
					p8d[0]=101;
				}
				break;
			case 101://跳闸准备
				//跳闸时功率
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//写寄存器数据到存贮器,Byte<=8
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//事件记录
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//跳闸2分后功率
					ERC6_Event(Pn+1,p8d[9],1,MRR(((u32)p8d)+2,2),i,MRR(((u32)p8d)+6,2));//功控跳闸记录;入口Fn总加组号(从1开始),Bin跳闸轮次,Type功控类别,kw1跳闸前功率,kw2跳闸后2分钟功率,kw3跳闸时功率定值
					p8d[0]=103;
				}
				break;
			case 103://跳闸2分钟后
				p8d[0]=3;
				//功控滑差时间
				i=kWContrlSlipTime(Pn);//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
				if(i>=120)
				{
					i-=120;//跳闸事件记录已延时2分钟
				}
				else
				{
					i=0;
				}
				p16timer[Pn]=i;
				break;
			default:
				goto NoCtrl;//不控制
		}
NextPn:
		p8s+=3;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

#endif//#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块




