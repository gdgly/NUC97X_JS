
//用户标识:山东版
#include "Project.h"
#include "../terminal/Terminal_USER2xx.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../terminal/Terminal_AFN0E_Event.h"


#if (USER/100)==2//用户标识:山东版

void DayAveragePower(u32 Pn,u32 Power,u32 Count)//日平均正有功功率计算;入口Pn从1开始,Power=有功功率(BCD),Count=本次计算次数
{
	u32 i;
	u32 x;
	u32 Addr;
	u64 d64;
	
	if((Pn==0)||(Pn>ERC29MaxPn))
	{
		return;
	}
	Pn--;
	if(Power&0x800000)
	{
		return;//负功率
	}
	Power&=0x7fffff;
	Power=bcd_hex(Power);
	Addr=ADDR_ERC29_SOUR+(Pn*LEN_ERC29_SOUR);
	i=MRR(ADDR_TYMDHMS+3,3);//当前年月日
	x=MRR(Addr+7,3);//原年月日
	if(i==x)
	{//同日
		i=MRR(Addr+10,3);//次数+1
		i+=Count;
		MWR(i,Addr+10,3);
		d64=MRR(Addr+13,6);//功率加
		d64+=Power;
		MWR(d64,Addr+13,6);
	}
	else
	{//不同日
		MWR(i,Addr+7,3);//新年月日
		x=YMD_Add1D(x);//年月日加1日,返回加1后的年月日
		if(i==x)
		{//是上1日
			i=MRR(Addr+10,3);//次数
			d64=MRR(Addr+13,6);//功率加
			if(i==0)
			{
				MWR(0,Addr+19,3);
			}
			else
			{
				i=d64/i;
				i=hex_bcd(i);
				MWR(i,Addr+19,3);
			}
		}
		else
		{//不是上1日,平均功率无效
			MWR(0xee,Addr+19,3);
		}
		MWR(Count,Addr+10,3);
		MWR(Power,Addr+13,6);
	}
}

void ERC29_Event_Day(u32 Pn,u64 LastDayEnergy,u64 TodayEnergy)//判断周期为日冻结电量的电能表飞走,超差;入口Pn=测量点号(1开始),LastDayEnergy=上是冻结电量,ToDayEnergy=今日冻结电量
{
	u32 i;
	u32 x;
	u32 y;
	u32 Energy;
	u32 Errf;
	u32 f;
	double f64;
	u8 * p8;
	u8 * p8s;
	
	if((Pn==0)||(Pn>ERC29MaxPn))
	{
		return;
	}
	p8s=(u8 *)(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)));
	i=ERC_Event_Set(29);//检查事件记录设置,返回0=不记录,1=记录
	if(i==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8s[5]=0;
	}
	x=ERC_Event_Set(28);//检查事件记录设置,返回0=不记录,1=记录
	if(x==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8s[6]=0;
	}
	i|=x;
	if(i==0)
	{
		p8s[0]=0xee;
		return;
	}
	Energy=MRR(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1))+19,3);
	if((Energy&0xff)==0xee)
	{//平均功率没计算
		return;
	}
	i=MRR(ADDR_TYMDHMS+3,3);//当前年月日
	x=MRR(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1))+7,3);//日正有功功率平均加的年月日(BCD);初始化值0
	if(i!=x)
	{//平均功率计算日期错
		return;
	}
	Energy=bcd_hex(Energy);
	f64=Energy;
	f64*=24;
	Energy=(u32)f64;//t=应走电量kwh,4位小数
	if(Energy<=100)
	{//应走电量kwh<=0.0100时不比较
		return;
	}
	LastDayEnergy=bcd16_hex(LastDayEnergy);
	TodayEnergy=bcd16_hex(TodayEnergy);
	if(TodayEnergy>=LastDayEnergy)
	{//当前示值>=原示值
		i=TodayEnergy-LastDayEnergy;//i=示值计算的电量
		y=i*10;
		y/=Energy;//示值计算的电量/实际应走电量=x.x
		y=hex_bcd(y);
		
		f=0;//飞走标志
		Errf=0;//超差标志
		x=MRR(ADDR_AFN04F59+1,1);//取电能表飞走阈值	数据格式22		l
		if(x==0)
		{//阈值=0
			x=~x;
		}
		if(y>x)
		{//(示值计算的电量/实际发发电量)>电能表飞走阈值,即判为电能表飞走
			f=1;//飞走标志
			if(p8s[6]&0x01)
			{//原超差已产生,则继续
				Errf=1;//超差标志
			}
		}
		else
		{
			x=MRR(ADDR_AFN04F59,1);//取电能表超差阈值	数据格式22		l
			if(x==0)
			{//阈值=0
				x=~x;
			}
			if(y>x)
			{//(示值计算的电量/实际发发电量)>电能表超差阈值,即判为电能表超差
				Errf=1;//超差标志
			}
		}
	//记录
		LastDayEnergy=hex_bcd16(LastDayEnergy);
		TodayEnergy=hex_bcd16(TodayEnergy);
		//超差恢复记录
		if(Errf==0)
		{
			x=p8s[6]&0x1;
			if(x!=0)
			{
				if((x^Errf)!=0x0)
				{//变化
					p8s[6]&=0xfe;
					p8s[6]|=Errf;
					p8=(u8 *)(ADDR_DATABUFF+2048);
					p8[0]=28;//ERC
					p8[1]=18;//长度Le
					MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(Errf<<7);
					MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//超差发生时对应正向有功电能示值
					MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//超差发生时正向有功电能示值
					MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//取电能表超差阈值	数据格式22		l
					ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				}
			}
		}
		//飞走恢复记录
		if(f==0)
		{
			x=p8s[5]&0x1;
			if(x!=0)
			{
				if((x^f)!=0x0)
				{//变化
					p8s[5]&=0xfe;
					p8s[5]|=f;
					p8=(u8 *)(ADDR_DATABUFF+2048);
					p8[0]=29;//ERC
					p8[1]=18;//长度Le
					MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(f<<7);
					MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//飞走发生前正向有功电能示值
					MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//飞走发生后正向有功电能示值
					MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//取电能表飞走阈值	数据格式22		l
					ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				}
			}
		}
		//超差产生记录
		x=p8s[6]&0x1;
		if(x==0)
		{
			if((x^Errf)!=0x0)
			{//变化
				p8s[6]&=0xfe;
				p8s[6]|=Errf;
				p8=(u8 *)(ADDR_DATABUFF+2048);
				p8[0]=28;//ERC
				p8[1]=18;//长度Le
				MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
				p8[7]=Pn;
				p8[8]=(Pn>>8)|(Errf<<7);
				MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//超差发生时对应正向有功电能示值
				MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//超差发生时正向有功电能示值
				MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//取电能表超差阈值	数据格式22		l
				ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			}
		}
		//飞走产生记录
		x=p8s[5]&0x1;
		if(x==0)
		{
			if((x^f)!=0x0)
			{//变化
				p8s[5]&=0xfe;
				p8s[5]|=f;
				p8=(u8 *)(ADDR_DATABUFF+2048);
				p8[0]=29;//ERC
				p8[1]=18;//长度Le
				MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
				p8[7]=Pn;
				p8[8]=(Pn>>8)|(f<<7);
				MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//飞走发生前正向有功电能示值
				MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//飞走发生后正向有功电能示值
				MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//取电能表飞走阈值	数据格式22		l
				ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			}
		}
	}
}


#endif
















