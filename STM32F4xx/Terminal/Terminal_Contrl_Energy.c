
//终端控制-电能量控制
#include "Project.h"
#include "Terminal_Contrl_Energy.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"







#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
void Terminal_Contrl_MonthEnergy(void)//终端控制-月电控
{
}
void Terminal_Contrl_BuyEnergy(void)//终端控制-购电控
{
}
#else
u32 kWhContrl1_Comp(u32 Pn)//终端控制-电控.月电控电能量比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
{
	u32 i;
	u32 x;
	u64 a64;
	u64 b64;

	//取月电能量控定值
	i=MRR(ADDR_AFN04F46+(5*Pn),4);//读存贮器数据到寄存器,Byte<=8
	i&=0xefffffff;//去符号位
	a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	//取终端月电能量控定值浮动系数
	i=MRR(ADDR_AFN04F20,1);//读存贮器数据到寄存器,Byte<=8
	x=i;
	i&=0x7f;
	i=bcd_hex(i);
	b64=(a64*i)/100;
	if((x>>7)==0x0)
	{//S0=0，表示上浮
		a64+=b64;
	}
	else
	{//S0=1，表示下浮
		a64-=b64;
	}
	//当前定值
	i=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
	MWR(i,ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn)+2,4);//写寄存器数据到存贮器,Byte<=8
	//取当前月电量
	i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//读存贮器数据到寄存器,Byte<=8
	i&=0xefffffff;//去符号位
	b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	if(b64>=a64)
	{//当前月电量>=定值
		return 2;//超限
	}

	//报警门限值(09版固定为定值的80%)
	i=MRR(ADDR_AFN04F46+(5*Pn),4);//读存贮器数据到寄存器,Byte<=8
	i&=0xefffffff;//去符号位
	a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	//取报警门限浮动系数
	i=MRR(ADDR_AFN04F46+(5*Pn)+4,1);//读存贮器数据到寄存器,Byte<=8
//	x=i;
//	i&=0x7f;
//	i=bcd_hex(i);
//	b64=(a64*i)/100;
//	if((x>>7)==0x0)
//	{//S0=0，表示上浮
//		a64+=b64;
//	}
//	else
//	{//S0=1，表示下浮
//		a64-=b64;
//	}
	i=bcd_hex(i);
	b64=i;
	a64*=b64;
	a64/=100;
	
	//取当前月电量
	i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//读存贮器数据到寄存器,Byte<=8
	i&=0xefffffff;//去符号位
	b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	if(b64>=a64)
	{//当前月电量>=定值80%
		return 1;//报警
	}
	return 0;//没超限
}


void Terminal_Contrl_MonthEnergy(void)//终端控制-月电控
{
	u32 i;
	u32 x;
	u32 Pn;
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	
	p8s=(u8 *)(ADDR_AFN05F15);
	p8d=(u8 *)(ADDR_Terminal_kWhCtrl1_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//解除
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			p8d[1]=0;//跳闸轮次=0
			goto NextPn;
		}
		
		p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
		if(p8[0]!=0x0)
		{//保电
			p8d[0]=0;
			p8d[1]=0;
		}
		
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				break;
			case 3://控制
				i=kWhContrl1_Comp(Pn);//终端控制-电控.月电控电能量比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
				switch(i)
				{
					case 1:
						i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//跳闸时电能量
						x=MRR(((u32)p8d)+2,4);//跳闸时电能量定值
						ERC23_Event(Pn+1,MRR(ADDR_AFN04F48+(1*Pn),1),1,i,x);//电控告警记录;入口Pn总加组号,Bin跳闸轮次,Type电控类别,kwh1告警时电能量,kwh2告警时电能量定值
						p8d[0]=100;//报警
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 100://报警
				i=kWhContrl1_Comp(Pn);//终端控制-电控.月电控电能量比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
				switch(i)
				{
					case 0:
						p8d[0]=3;//
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 101://跳闸准备
//				p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
//				if(p8[0]!=0x0)
//				{//保电
//					i=kWhContrl1_Comp(Pn);//终端控制-电控.月电控电能量比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
//					if(i!=2)
//					{
//						p8d[0]=3;
//					}
//					break;
//				}

				//跳闸轮次
				x=0;//最大全部轮次
				for(i=0;i<MaxControlRound;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_AFN04F48+(1*Pn),1);//读存贮器数据到寄存器,Byte<=8
				i&=x;
				if(i!=0x0)
				{//有轮次跳闸
					p8d[1]=i;
					i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//跳闸时电能量
					x=MRR(((u32)p8d)+2,4);//跳闸时电能量定值
					ERC7_Event(Pn+1,p8d[1],1,i,x);//电控跳闸记录;入口Pn总加组号(从1开始),Bin跳闸轮次,Type电控类别,kwh1跳闸时电能量,kwh2跳闸时电能量定值
				}
				p8d[0]=102;
				Terminal_Ctrl_Count(0);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				break;
			case 102://跳闸
				i=kWhContrl1_Comp(Pn);//终端控制-电控.月电控电能量比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//清轮次跳闸
				}
				break;
			default:
				goto NoCtrl;//不控制
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}



u32 kWhContrl2_Comp(u32 Pn)//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
{
	u32 i;
	s64 a64;
	s64 b64;

	//剩余电量或电费
	i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读存贮器数据到寄存器,Byte<=8
	a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	//跳闸门限值
	i=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//读存贮器数据到寄存器,Byte<=8
	b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	if(a64<=b64)
	{//剩余电量或电费<=跳闸门限值
		return 2;//超限
	}
	//报警门限值
	i=MRR(ADDR_AFN04F47+(17*Pn)+9,4);//读存贮器数据到寄存器,Byte<=8
	b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
	if(a64<=b64)
	{//剩余电量或电费<=报警门限值
		return 1;//报警
	}
	return 0;//没超限
}


void Terminal_Contrl_BuyEnergy(void)//终端控制-购电控
{
	u32 i;
	u32 x;
	u32 Pn;
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	
	p8s=(u8 *)(ADDR_AFN05F16);
	p8d=(u8 *)(ADDR_Terminal_kWhCtrl2_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//解除
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			p8d[1]=0;//跳闸轮次=0
			goto NextPn;
		}
		
		p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
		if(p8[0]!=0x0)
		{//保电
			p8d[0]=0;
			p8d[1]=0;//清轮次跳闸
		}
		
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				break;
			case 3://控制
				i=kWhContrl2_Comp(Pn);//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
				switch(i)
				{
					case 1:
						i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读存贮器数据到寄存器,Byte<=8
						x=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//读存贮器数据到寄存器,Byte<=8
						ERC23_Event(Pn+1,MRR(ADDR_AFN04F48+(1*Pn),1),2,i,x);//电控告警记录;入口Pn总加组号,Bin跳闸轮次,Type电控类别,kwh1告警时电能量,kwh2告警时电能量定值
						p8d[0]=100;//报警
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 100://报警
				i=kWhContrl2_Comp(Pn);//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
				switch(i)
				{
					case 0:
						p8d[0]=3;//
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 101://跳闸准备

//				p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
//				if(p8[0]!=0x0)
//				{//保电
//					i=kWhContrl2_Comp(Pn);//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
//					if(i!=2)
//					{
//						p8d[0]=3;
//					}
//					break;
//				}

				//跳闸轮次
				x=0;//最大全部轮次
				for(i=0;i<MaxControlRound;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_AFN04F48+(1*Pn),1);//读存贮器数据到寄存器,Byte<=8
				i&=x;
				if(i!=0x0)
				{//有轮次跳闸
					p8d[1]=i;
					i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读存贮器数据到寄存器,Byte<=8
					x=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//读存贮器数据到寄存器,Byte<=8
					ERC7_Event(Pn+1,p8d[1],2,i,x);//电控跳闸记录;入口Pn总加组号(从1开始),Bin跳闸轮次,Type电控类别,kwh1跳闸时电能量,kwh2跳闸时电能量定值
				}
				p8d[0]=102;
				Terminal_Ctrl_Count(1);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				break;
			case 102://跳闸
				i=kWhContrl2_Comp(Pn);//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7);返回0没超限,1报警,2超限
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//清轮次跳闸
				}
				break;
			default:
				goto NoCtrl;//不控制
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

#endif//#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块












