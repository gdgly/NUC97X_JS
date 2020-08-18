
//终端类2数据Fn
#include "Project.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "KeyUser.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"




extern Terminal_Class2DataLib_TypeDef Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



u32 Class2Data_FnOnly(u32 ClassNo,u32 Fn)//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
{
	u32 i;
	u8 * p8;
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F39+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
	i=MRR((u32)p8,1);
	if(i>=((Fn/8)+1))
	{//信息类组数n(0～31)
		p8+=1;
		p8+=(Fn/8);
		i=MRR((u32)p8,1);
		i>>=(Fn%8);
		i&=0x1;
		return(i);
	}
	return(0);
}

u32 Class2Data_Fn(u32 ClassNo,u32 Fn)//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
{
	u32 i;
	u8 * p8;
	
	if(Fn==0x0)
	{
		return(0);
	}
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F39+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
	i=MRR((u32)p8,1);
	if(i>=((Fn/8)+1))
	{//信息类组数n(0～31)
		p8+=1;
		p8+=(Fn/8);
		i=MRR((u32)p8,1);
		i>>=(Fn%8);
		i&=0x1;
	}
	else
	{
		i=0;
	}
	if(i!=0x0)
	{
		return(1);
	}
	else
	{
		Fn++;
		switch(Fn)
		{
//F1 日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			case 161://F161 日冻结正向有功电能示值（总、费率1～M）
			case 162://F162 日冻结正向无功（组合无功1）电能示值（总、费率1～M）
			case 165://F165 日冻结一象限无功电能示值（总、费率1～M）
			case 168://F168 日冻结四象限无功电能示值（总、费率1～M）
				i=Class2Data_FnOnly(ClassNo,1);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F2 日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			case 163://F163 日冻结反向有功电能示值（总、费率1～M）
			case 164://F164 日冻结反向无功（组合无功2）电能示值（总、费率1～M）
			case 166://F166 日冻结二象限无功电能示值（总、费率1～M）
			case 167://F167 日冻结三象限无功电能示值（总、费率1～M）
				i=Class2Data_FnOnly(ClassNo,2);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F3 日冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 185://日冻结正向有功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 186://日冻结正向无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				i=Class2Data_FnOnly(ClassNo,3);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F4 日冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 187://日冻结反向有功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 188://日冻结反向无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				i=Class2Data_FnOnly(ClassNo,4);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F17 月冻结正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			case 177://月冻结正向有功电能示值（总、费率1～M）
			case 178://月冻结正向无功（组合无功1）电能示值（总、费率1～M）
			case 181://月冻结一象限无功电能示值（总、费率1～M）
			case 184://月冻结四象限无功电能示值（总、费率1～M）
				i=Class2Data_FnOnly(ClassNo,17);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F18 月冻结反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			case 179://月冻结反向有功电能示值（总、费率1～M）
			case 180://月冻结反向无功（组合无功2）电能示值（总、费率1～M）
			case 182://月冻结二象限无功电能示值（总、费率1～M）
			case 183://月冻结三象限无功电能示值（总、费率1～M）
				i=Class2Data_FnOnly(ClassNo,18);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F19 月冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 193://月冻结正向有功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 194://月冻结正向无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				i=Class2Data_FnOnly(ClassNo,19);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
//F20 月冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 195://月冻结反向有功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 196://月冻结反向无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				i=Class2Data_FnOnly(ClassNo,20);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				return(i);
		}
	}

	return(0);
}


u32 GetClass2DataOfficeAddr(u32 Fn,u32 Pn)//取类2数据存贮偏移地址,返回0表示没找到
{
	u32 Addr;
	
	if(Fn>=Get_Class2Data_MaxFn())
	{
		return(0);
	}
	if(Pn>Terminal_Class2DataLib[Fn].PnMax)
	{
		if(Fn==246)
		{//AFN0DF246当前掉电记录数据映射的Pn
			if(Pn>MaxRS485AddCarrierPn)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	if((Terminal_Class2DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//本方案无此Fn功能
		return(0);
	}
	if(Terminal_Class2DataLib[Fn].PnType==0x0)
	{
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	}
	return(Addr);
}

u32 GetClass2DataF246OfficeAddr(u32 CongealNo,u32 Pn)//取类2数据F246存贮偏移地址,返回0表示没找到
{
	u32 i;
	u32 x;
	u32 Addr;
	
	Addr=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+ADDR_AFN0DF246Pn;
	for(i=0;i<F246MaxPn;i++)
	{
		x=MRR(Addr+(2*i),2);
		if(x==Pn)
		{
			Addr=Terminal_Class2DataLib[246].OfficeAddr+(Terminal_Class2DataLib[246].PnLen*i);
			return(Addr);
		}
	}
	return 0;
}

u32 GetClass2DataOfficeAddr_MapPn(u32 Fn,u32 Pn)//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
{
	u32 Addr;
	
	if(Fn>=Get_Class2Data_MaxFn())
	{
		return(0);
	}
//	if(Pn>Terminal_Class2DataLib[Fn].PnMax)
//	{
//		return(0);
//	}
	if((Terminal_Class2DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//本方案无此Fn功能
		return(0);
	}
	if(Terminal_Class2DataLib[Fn].PnType==0x0)
	{
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	}
	return(Addr);
}


u32 GetClass2DataCongealType(u32 Fn)//取类2数据冻结类型
{
	return(Terminal_Class2DataLib[Fn].CongealType);
}

u32 GetClass2DataLen(u32 Fn,u32 Pn,u32 DataAddr)//取类2数据长度
{
	u32 Len;
	u32 *p32;
	u32 Tariff;
	
	p32=(u32*)(ADDR_Terminal_ClassDataLen);//1类数据长度寄存器或2类数据费率寄存器(原2类数据直接改写FLASH,集中器的29GL写需0.5秒再若全部测量点太长太长时间,故增加费率寄存器)
	p32[0]=0;
#if DefaultTariff==0//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
		Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
	#else
		Tariff=TMaxTariff;
	#endif
#endif
	switch(Fn)
	{
		case 1://日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
		case 2://日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
		case 9://抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
		case 10://抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
		case 17://月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
		case 18://月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
#if AFN0DF1MaxPn==0
		//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成,合成后数据放ADDR_DATABUFF
			Len=MRR(ADDR_DATABUFF+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#endif
			Len=6+(17*(Len+1));
#else
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			Len=MRR(DataAddr+5,1);
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(17*(Len+1));
#endif
			break;
		case 3://日冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 4://日冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 11://抄表日冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 12://抄表日冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 19://月冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 20://月冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）	
#if AFN0DF1MaxPn==0
		//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成,合成后数据放ADDR_DATABUFF
			Len=MRR(ADDR_DATABUFF+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#endif
			Len=6+(14*(Len+1));
#else
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(14*(Len+1));
#endif
			break;
		case 5://日冻结正向有功电能量（总、费率1～M）
		case 6://日冻结正向无功电能量（总、费率1～M）
		case 7://日冻结反向有功电能量（总、费率1～M）
		case 8://日冻结反向无功电能量（总、费率1～M）
		case 21://月冻结正向有功电能量（总、费率1～M）
		case 22://月冻结正向无功电能量（总、费率1～M）
		case 23://月冻结反向有功电能量（总、费率1～M）
		case 24://月冻结反向无功电能量（总、费率1～M）
	#if AFN0DF5MaxPn==0
		//计算的电能量费率要固定,否则取通信长度不确定
		#if DefaultTariff==0//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			Tariff=TMaxTariff;
		#endif
			Len=Tariff;//固定的费率数
			p32[0]=0x10000+Len;
			Len=1+(4*(Len+1));
	#else
			Len=MRR(DataAddr,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10000+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10000+Len;
			}
		#endif
			Len=1+(4*(Len+1));
	#endif
			break;
		case 58://日冻结总加组日累计有功电能量（总、费率1～M）
		case 59://日冻结总加组日累计无功电能量（总、费率1～M）
		case 61://月冻结总加组日累计有功电能量（总、费率1～M）
		case 62://月冻结总加组日累计无功电能量（总、费率1～M）
			Len=MRR(DataAddr,1);
			if(Len>TMaxTariff)
			{
				Len=0;//TMaxTariff;
				p32[0]=0x10000+Len;
			}
			Len=1+(4*(Len+1));
			break;
		case 121://日冻结A相谐波越限日统计数据
		case 122://日冻结B相谐波越限日统计数据
		case 123://日冻结C相谐波越限日统计数据
			Len=MRR(DataAddr,1);
			if(Len>19)
			{
				p32[0]=0x10000+19;
			}
			Len=Terminal_Class2DataLib[Fn].PnLen;
			break;
		case 161://日冻结正向有功电能示值（总、费率1～M）
		case 163://日冻结反向有功电能示值（总、费率1～M）
		case 169://抄表日冻结正向有功电能示值（总、费率1～M）
		case 171://抄表日冻结反向有功电能示值（总、费率1～M）
		case 177://月冻结正向有功电能示值（总、费率1～M）
		case 179://月冻结反向有功电能示值（总、费率1～M）
		case 201://月冻结第1时区冻结正向有功电能示值（总、费率1～M）
		case 202://月冻结第2时区冻结正向有功电能示值（总、费率1～M）
		case 203://月冻结第3时区冻结正向有功电能示值（总、费率1～M）
		case 204://月冻结第4时区冻结正向有功电能示值（总、费率1～M）
		case 205://月冻结第5时区冻结正向有功电能示值（总、费率1～M）
		case 206://月冻结第6时区冻结正向有功电能示值（总、费率1～M）
		case 207://月冻结第7时区冻结正向有功电能示值（总、费率1～M）
		case 208://月冻结第8时区冻结正向有功电能示值（总、费率1～M）
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(5*(Len+1));
			break;
		case 162://日冻结正向无功（组合无功1）电能示值（总、费率1～M）
		case 164://日冻结反向无功（组合无功1）电能示值（总、费率1～M）
		case 165://日冻结1象限无功电能示值（总、费率1～M）
		case 166://日冻结2象限无功电能示值（总、费率1～M）
		case 167://日冻结3象限无功电能示值（总、费率1～M）
		case 168://日冻结4象限无功电能示值（总、费率1～M）
		case 170://抄表日冻结正向无功（组合无功1）电能示值（总、费率1～M）
		case 172://抄表日冻结反向无功（组合无功1）电能示值（总、费率1～M）
		case 173://抄表日冻结1象限无功电能示值（总、费率1～M）
		case 174://抄表日冻结2象限无功电能示值（总、费率1～M）
		case 175://抄表日冻结3象限无功电能示值（总、费率1～M）
		case 176://抄表日冻结4象限无功电能示值（总、费率1～M）
		case 178://月冻结正向无功（组合无功1）电能示值（总、费率1～M）
		case 180://月冻结反向无功（组合无功1）电能示值（总、费率1～M）
		case 181://月冻结1象限无功电能示值（总、费率1～M）
		case 182://月冻结2象限无功电能示值（总、费率1～M）
		case 183://月冻结3象限无功电能示值（总、费率1～M）
		case 184://月冻结4象限无功电能示值（总、费率1～M）
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(4*(Len+1));
			break;
		case 185://日冻结正向有功最大需量及发生时间（总、费率1～M）
		case 186://日冻结正向无功最大需量及发生时间（总、费率1～M）
		case 187://日冻结反向有功最大需量及发生时间（总、费率1～M）
		case 188://日冻结反向无功最大需量及发生时间（总、费率1～M）
		case 189://抄表日冻结正向有功最大需量及发生时间（总、费率1～M）
		case 190://抄表日冻结正向无功最大需量及发生时间（总、费率1～M）
		case 191://抄表日冻结反向有功最大需量及发生时间（总、费率1～M）
		case 192://抄表日冻结反向无功最大需量及发生时间（总、费率1～M）
		case 193://月冻结正向有功最大需量及发生时间（总、费率1～M）
		case 194://月冻结正向无功最大需量及发生时间（总、费率1～M）
		case 195://月冻结反向有功最大需量及发生时间（总、费率1～M）
		case 196://月冻结反向无功最大需量及发生时间（总、费率1～M）
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(7*(Len+1));
			break;
		case 216://月冻结电能表结算信息
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(10*(Len+1))+5;
			break;
			
		default:
			Len=Terminal_Class2DataLib[Fn].PnLen;
			break;
	}
	return(Len);
}

u32 GetDateCongealNo(u32 YMD)//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
{
	u32 i;
	u32 x;
	
	x=MRR(ADDR_TCongealCount+14,1);
	if((x&0x80)!=0x0)
	{
		x=MaxDateCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMD==MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),3))
		{
			return i;//找到
		}
	}
	return 0xffffffff;
}
u32 GetDateCongealAddr(u32 YMD,u32 Fn,u32 Pn)//得到日冻结存储地址,返回=0表示没找到
{
	u32 i;
	u32 x;
	i=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{
		return 0;
	}
	if(Fn==246)
	{//日冻结掉电记录数据(测量点映射)
		x=GetClass2DataF246OfficeAddr(i,Pn);//取类2数据F246存贮偏移地址,返回0表示没找到
	}
	else
	{
		x=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	}
	if(x==0)
	{
		return 0;
	}
	return (ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+x);
}

u32 GetRMDateCongealNo(u32 YMD)//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
{
	u32 i;
	u32 x;

	x=MRR(ADDR_TCongealCount+21,1);
	if((x&0x80)!=0x0)
	{
		x=MaxRMDateCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMD==MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3))
		{
			return i;//找到
		}
	}
	return 0xffffffff;
}
u32 GetRMDateCongealAddr(u32 YMD,u32 Fn,u32 Pn)//得到抄表日冻结存储地址,返回=0表示没找到
{
	u32 i;
	u32 x;
	i=GetRMDateCongealNo(YMD);//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{
		return 0;
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(x==0)
	{
		return 0;
	}
	return (ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+x);
}

u32 GetMonthCongealNo(u32 YM)//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
{
	u32 i;
	u32 x;

	x=MRR(ADDR_TCongealCount+28,1);
	if((x&0x80)!=0x0)
	{
		x=MaxMonthCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YM==MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),2))
		{
			return i;//找到
		}
	}
	return 0xffffffff;
}
u32 GetMonthCongealAddr(u32 YM,u32 Fn,u32 Pn)//得到月冻结存储地址,返回=0表示没找到
{
	u32 i;
	u32 x;
	i=GetMonthCongealNo(YM);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{
		return 0;
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(x==0)
	{
		return 0;
	}
	return (ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+x);
}
u32 GetCurveCongealNo(u64 YMDHM)//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
{
	u32 i;
	u32 x;
	
	x=MRR(ADDR_TCongealCount+7,2);
	if(x&0x8000)
	{
		x=MaxCurveCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMDHM==MRR(ADDR_CurveCongealList+2+(6*i),5))
		{
			return i;//找到
		}
	}
	return 0xffffffff;
}

u32 GetCurveCongealAddr(u64 YMDHM,u32 Fn,u32 Pn)//得到曲线冻结存储地址,返回=0表示没找到
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	CongealNo=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return 0;
	}
	MapPn=KeyUserMapPn(Pn);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return 0;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//冻结的测量点列表中没找到相同的测量点号
		//查冻结列表是否有空可用
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//有空可用
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				break;
			}
		}
		if(MapPn>=MaxKeyUser)
		{//没可用
			return 0;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return 0;
	}
	return (ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr);
}

u32 GetACSampleCurveCongealAddr(u64 YMDHM,u32 Fn)//得到交采测量点曲线冻结存储地址,返回=0表示没找到
{
#if MaxACSampleCurveCongeal==0//总冻结点数
	return 0;
#else
	u32 i;
	u8 *p8;
	u64 d64;
	
	MC(0,ADDR_DATABUFF,7);
	p8=(u8*)(ADDR_DATABUFF);
	p8[3]=1;//00年01月01日00时00分00秒
	p8[4]=1;
	MWR(YMDHM,ADDR_DATABUFF+7,5);
	i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
	i/=60;
	i%=MaxACSampleCurveCongeal;//偏移点数
	i*=LEN_ACSampleCurve;//偏移地址
	i+=ADDR_ACSampleCurve;//地址
	d64=MRR(i,5);
	if(d64==YMDHM)
	{//时标相同
		switch(Fn)
		{
			case 89://A相电压曲线
				return i+5;
			case 90://B相电压曲线
				return i+7;
			case 91://C相电压曲线
				return i+9;
		}
	}
	return 0;
#endif
}


u32 CheckCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn)//检查曲线冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	CongealNo=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
	CongealNo=PnCurveCongealNo(CongealNo,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return 2;
	}
	MapPn=KeyUserMapPn(Pn);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return 2;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//冻结的测量点列表中没找到相同的测量点号
		//查冻结列表是否有空可用
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//有空可用
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				return 0;//返回:0=数据没冻结
			}
		}
		if(MapPn>=MaxKeyUser)
		{//没可用
			return 2;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//相同时标已抄
		return 1;
	}
	return 0;
}

void AddCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//补曲线冻结点数据
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;

	CongealNo=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return;
	}
	MapPn=KeyUserMapPn(Pn);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//冻结的测量点列表中没找到相同的测量点号
		//查冻结列表是否有空可用
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//有空可用
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				break;
			}
		}
		if(MapPn>=MaxKeyUser)
		{//没可用
			return;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,Byte);
}

u64 CurveCongeal_YMDHM(void)//当前标准曲线冻结时间YMDHM
{
	u32 i;
	u64 YMDHM;
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=CurveCongealTime;
	i*=CurveCongealTime;
	i=hex_bcd(i);
	YMDHM=MRR(ADDR_TYMDHMS+2,4);
	YMDHM<<=8;
	YMDHM|=i;//当前标准曲线冻结点的YMDHM
	return YMDHM;
}

u32 CheckDateCongeal(u32 YMD,u32 Fn,u32 Pn)//检查日冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	CongealNo=PnDateCongealNo(CongealNo,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return 2;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//相同时标已抄
		return 1;
	}
	return 0;
}

void AddDateCongeal(u32 YMD,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//补日冻结点数据
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+OfficeAddr,Byte);
}

u32 CheckMonthCongeal(u32 YM,u32 Fn,u32 Pn)//检查月冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetMonthCongealNo(YM);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
	CongealNo=PnMonthCongealNo(CongealNo,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return 2;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//相同时标已抄
		return 1;
	}
	return 0;
}

void AddMonthCongeal(u32 YM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//补月冻结点数据
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetMonthCongealNo(YM);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(CongealNo==0xffffffff)
	{//没找到相同时标的冻结号
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*CongealNo)+OfficeAddr,Byte);
}









//2类数据.日冻结
//F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//2类数据.抄表日冻结
//F9测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//F10测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//2类数据.月冻结
//F17测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//F18测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
void Class2Group_F1_F2_F9_F10_F17_F18(u32 CongealAddr,u32 Pn,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4)//2类数据合成;出口:数据放ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 RMTime;
	u8 * p8;
	u32 Tariff;
#if DefaultTariff==0//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
		Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
	#else
		Tariff=TMaxTariff;
	#endif
#endif

	RMTime=0;//抄表时间填入标志,0=没,1=填入
	p8=(u8*)ADDR_DATABUFF;
	//正向有功
	i=GetClass2DataOfficeAddr(Fn1,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(i==0)
	{//没找到
		MC(0xee,ADDR_DATABUFF,6+5);
		x=Tariff;//费率数
		p8[5]=x;//费率数
	}
	else
	{//找到
		x=MRR(CongealAddr+i+5,1);//费率数
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
		if(x!=Tariff)
		{
			x=Tariff;//费率数
		}
	#else
		if(x>TMaxTariff)
		{
			x=Tariff;//费率数
		}
	#endif
		MR(ADDR_DATABUFF,CongealAddr+i,6+((x+1)*5));
		Data_0xFFto0xEE(ADDR_DATABUFF,6+((x+1)*5));//比较数据:全0xFF则替换为全0xEE
		p8[5]=x;//费率数
		if(p8[0]!=0xee)
		{
			RMTime=1;//抄表时间填入标志,0=没,1=填入
		}
	}
	//正向无功（组合无功1）
	i=GetClass2DataOfficeAddr(Fn2,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(i==0)
	{//没找到
		MC(0xee,ADDR_DATABUFF+6+((x+1)*5),(x+1)*4);
	}
	else
	{//找到
		MR(ADDR_DATABUFF+6+((x+1)*5),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*5),(x+1)*4);//比较数据:全0xFF则替换为全0xEE
		if(RMTime==0)//抄表时间填入标志,0=没,1=填入
		{
			if(p8[6+((x+1)*5)]!=0xee)
			{
				RMTime=1;//抄表时间填入标志,0=没,1=填入
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
	//一象限无功
	i=GetClass2DataOfficeAddr(Fn3,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(i==0)
	{//没找到
		MC(0xee,ADDR_DATABUFF+6+((x+1)*9),(x+1)*4);
	}
	else
	{//找到
		MR(ADDR_DATABUFF+6+((x+1)*9),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*9),(x+1)*4);//比较数据:全0xFF则替换为全0xEE
		if(RMTime==0)//抄表时间填入标志,0=没,1=填入
		{
			if(p8[6+((x+1)*9)]!=0xee)
			{
				RMTime=1;//抄表时间填入标志,0=没,1=填入
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
	//四象限无功
	i=GetClass2DataOfficeAddr(Fn4,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(i==0)
	{//没找到
		MC(0xee,ADDR_DATABUFF+6+((x+1)*13),(x+1)*4);
	}
	else
	{//找到
		MR(ADDR_DATABUFF+6+((x+1)*13),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*13),(x+1)*4);//比较数据:全0xFF则替换为全0xEE
		if(RMTime==0)//抄表时间填入标志,0=没,1=填入
		{
			if(p8[6+((x+1)*13)]!=0xee)
			{
				RMTime=1;//抄表时间填入标志,0=没,1=填入
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
}

//2类数据.日冻结
//F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//2类数据.抄表日冻结
//F11测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//F12测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//2类数据.月冻结
//F19测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//F20测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
void Class2Group_F3_F4_F11_F12_F19_F20(u32 CongealAddr,u32 Pn,u32 Fn1,u32 Fn2)//2类数据合成;出口:数据放ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 y;
	u32 RMTime;
	u8 * p8;
	u32 Tariff;
#if DefaultTariff==0//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
		Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
	#else
		Tariff=TMaxTariff;
	#endif
#endif

	RMTime=0;//抄表时间填入标志,0=没,1=填入
	p8=(u8*)ADDR_DATABUFF;
	//有功
	y=GetClass2DataOfficeAddr(Fn1,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(y==0)
	{//没找到
		MC(0xee,ADDR_DATABUFF,6+7);
		x=Tariff;//费率数
		p8[5]=x;//费率数
	}
	else
	{//找到
		x=MRR(CongealAddr+y+5,1);//费率数
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
		if(x!=Tariff)
		{
			x=Tariff;//费率数
		}
	#else
		if(x>TMaxTariff)
		{
			x=Tariff;//费率数
		}
	#endif
		MR(ADDR_DATABUFF,CongealAddr+y,6);
		Data_0xFFto0xEE(ADDR_DATABUFF,6);//比较数据:全0xFF则替换为全0xEE
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(3*i),CongealAddr+y+6+(7*i),3);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(3*i),3);//比较数据:全0xFF则替换为全0xEE
		}
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(3*(x+1)+(4*i)),CongealAddr+y+6+(7*i)+3,4);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(3*(x+1)+(4*i)),4);//比较数据:全0xFF则替换为全0xEE
		}
		p8[5]=x;//费率数
		if(p8[0]!=0xee)
		{
			RMTime=1;//抄表时间填入标志,0=没,1=填入
		}
	}
	//无功
	y=GetClass2DataOfficeAddr(Fn2,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(y==0)
	{//没找到
		MC(0xee,ADDR_DATABUFF+6+((x+1)*7),(x+1)*7);
	}
	else
	{//找到
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(7*(x+1))+(3*i),CongealAddr+y+6+(7*i),3);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(7*(x+1))+(3*i),3);//比较数据:全0xFF则替换为全0xEE
		}
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(10*(x+1)+(4*i)),CongealAddr+y+6+(7*i)+3,4);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(10*(x+1)+(4*i)),4);//比较数据:全0xFF则替换为全0xEE
		}
		if(RMTime==0)//抄表时间填入标志,0=没,1=填入
		{
			i=MRR(CongealAddr+y,1);
			if((i!=0xee)&&(i!=0xff))
			{
				RMTime=1;//抄表时间填入标志,0=没,1=填入
				MR(ADDR_DATABUFF,CongealAddr+y,5);
			}
		}
	}
}

void Class2_Group(u32 CongealAddr,u32 Fn,u32 Pn)//2类数据合成;出口:数据放ADDR_DATABUFF
{
	switch(Fn)
	{
	//2类数据.日冻结
		case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,161,162,165,168);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,163,164,166,167);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,185,186);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,187,188);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
	//2类数据.抄表日冻结
		case 9://F9测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,169,170,173,176);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 10://F10测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,171,172,174,175);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 11://F11测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,189,190);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 12://F12测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,191,192);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
	//2类数据.月冻结
		case 17://F17测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,177,178,181,184);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 18://F18测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,179,180,182,183);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 19://F19测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,193,194);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
		case 20://F20测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,195,196);//2类数据合成;出口:数据放ADDR_DATABUFF
			break;
	}
}

void UART_AFN0DF97F98F99F100_Computer(u32 ADDR_FullData,u64 YMDHM,u32 m,u32 Fn,u32 Pn,u32 CongealNo)//2类曲线数据电能量计算;入口：Fn为用于计算的示值曲线Fn
{
	u32 x;
	u32 y;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	MC(0xee,ADDR_FullData,4);//清数据
	p16=(u16 *)(ADDR_PnVSSetList);
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	for(x=0;x<MaxKeyUser;x++)
	{
		if(Pn==p16[x])
		{
			break;
		}
	}
	if(x>=MaxKeyUser)
	{//冻结的测量点列表中没找到相同的测量点号
		return;
	}
	MapPn=x+1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
	if(OfficeAddr!=0)
	{
		x=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,4);
		if((x!=0xeeeeeeee)&&(x!=0xffffffff))
		{
			YMDHM=YMDHM_SubM(YMDHM,m);//年月日时分减m分,返回减m分后的年月日时分
			CongealNo=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
			CongealNo=PnCurveCongealNo(CongealNo,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
			if(CongealNo!=0xffffffff)
			{//找到相同时标的冻结号
				MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
				for(y=0;y<MaxKeyUser;y++)
				{
					if(Pn==p16[y])
					{
						break;
					}
				}
				if(y>=MaxKeyUser)
				{//冻结的测量点列表中没找到相同的测量点号
					return;
				}
				MapPn=y+1;
				OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找�
				if(OfficeAddr!=0)
				{
					y=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,4);
					if((y!=0xeeeeeeee)&&(y!=0xffffffff))
					{
						x=bcd_hex(x);
						y=bcd_hex(y);
						if(x<y)
						{//可能值回0
							x+=100000000;
						}
						x-=y;
						x=hex_bcd(x);
						x<<=8;//电能量为4位小数
						MWR(x,ADDR_FullData,4);
					}
				}
			}
		}
	}
}


#if ((RMM_DateCongeal_0xEE!=0)||(RMM_MonthCongeal_0xEE!=0))//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
void UART_EntreyIncComputer(u32 Pn,u32 ADDR_FullData,u32 Addr1,u32 Addr2,u32 Len,u32 Tariff)//2类日冻结数据电能量计算
{
	u32 x;
	u32 y;
	u64 a64;
	u64 b64;
	
	for(x=0;x<=Tariff;x++)
	{
		//取当前值
		b64=MRR(Addr1+6+(Len*x),Len);
		//取昨天值
		a64=MRR(Addr2+6+(Len*x),Len);
		y=a64&0xff;
		if((y==0xee)||(y==0xff))
		{//原始值=空
			continue;
		}
		y=b64&0xff;
		if((y==0xee)||(y==0xff))
		{//当前值=空
			continue;
		}
		//增量计算
		a64=bcd16_hex(a64);
		b64=bcd16_hex(b64);
		if(b64>=a64)
		{//当前值>=原始值
			b64-=a64;
		}
		else
		{
			if(Len==5)
			{
				if(a64>9000000000)
				{//当前值回0
					b64+=10000000000;
					b64-=a64;
				}
				else
				{//错误
					b64=0;
				}
			}
			else
			{
				if(a64>90000000)
				{//当前值回0
					b64+=100000000;
					b64-=a64;
				}
				else
				{//错误
					b64=0;
				}
			}
		}
		b64=hex_bcd16(b64);
		y=b64;
		if(Len==4)
		{
			y<<=8;//小数点变4位
		}
		MWR(y,ADDR_FullData+1+(4*x),4);
	}
}
void UART_AFN0DF5F6F7F8_Computer(u32 ADDR_FullData,u32 YMD,u32 Fn,u32 Pn,u32 Len)//2类日冻结数据电能量计算;入口：Fn为用于计算的示值Fn
{
	u32 Addr1;
	u32 Addr2;
	u32 Tariff;//费率数固定为设置费率

//计算的电能量费率要固定,否则取通信长度不确定
#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
	Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
#else
	Tariff=TMaxTariff;
#endif
	MC(0xee,ADDR_FullData,1+(4*(Tariff+1)));//清数据
	Addr1=GetDateCongealAddr(YMD,Fn,Pn);//得到日冻结存储地址,返回=0表示没找到
	if(Addr1==0)
	{
		return;
	}
	YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
	Addr2=GetDateCongealAddr(YMD,Fn,Pn);//得到日冻结存储地址,返回=0表示没找到
	if(Addr2==0)
	{
		return;
	}
	UART_EntreyIncComputer(Pn,ADDR_FullData,Addr1,Addr2,Len,Tariff);//2类日冻结数据电能量计算
}
void UART_AFN0DF21F22F23F24_Computer(u32 ADDR_FullData,u32 YM,u32 Fn,u32 Pn,u32 Len)//2类月冻结数据电能量计算;入口：Fn为用于计算的示值Fn
{
	u32 Addr1;
	u32 Addr2;
	u32 Tariff;//费率数固定为设置费率

//计算的电能量费率要固定,否则取通信长度不确定
#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
	Tariff=Get_SetTariff(Pn);//取AFN04F10中配置的费率数
#else
	Tariff=TMaxTariff;
#endif
	MC(0xee,ADDR_FullData,1+(4*(Tariff+1)));//清数据
	Addr1=GetMonthCongealAddr(YM,Fn,Pn);//得到月冻结存储地址,返回=0表示没找到
	if(Addr1==0)
	{
		return;
	}
	YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
	Addr2=GetMonthCongealAddr(YM,Fn,Pn);//得到月冻结存储地址,返回=0表示没找到
	if(Addr2==0)
	{
		return;
	}
	UART_EntreyIncComputer(Pn,ADDR_FullData,Addr1,Addr2,Len,Tariff);//2类日冻结数据电能量计算
}
#endif


void SaveClass2Data(u32 PORTn,u32 Pn,u32 FnDataAddr,u8 *p8RMLib)//存贮类2测量点抄表数据
{
	u32 i;
	u32 x;
	u32 y;
	u32 Addr;
	u8 *p8FnData;
	u32 Fn;
	u32 YM;
	u32 YMD;
	u64 YMDHM;
	#if (RMM_DateCongeal_0xEE==0)//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	u32 f;
	u32 *p32;
	#endif
	#if (USER/100)==2//用户标识:山东版
	u64 LastDayEnergy;
	u64 TodayEnergy;
	#endif
	
	x=x;
	y=y;
	if((u32)p8RMLib==0)
	{
		return;
	}
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	p8FnData=(u8*)(FnDataAddr);
	i=p8RMLib[0];
	if((i==0xe3)||(i==0xee)||(i==0xef))
	{
		if(p8FnData[0]!=1)
		{//冻结日期错误或是DL645-1997表
			return;
		}
	}
	else
	{
		if(p8FnData[6]==0xee)
		{//数据无效
			return;
		}
	}

#if (RMM_CompCongealTime==0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
	//不比较
	YMD=MRR(ADDR_TYMDHMS+3,3);//取当前时间年月日
	YM=YMD>>8;
	YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
	switch(i)
	{
		case 0xe3://上1日电表日冻结
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			Addr=GetClass2SourceDataAddr(161,Pn)-3;//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			break;
		case 0xee://上2日电表日冻结
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			Addr=ADDR_LastDate_Source_Start+((Pn-1)*LEN_LastDate_Source_PerPn);
			break;
		case 0xef://上3日电表日冻结
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			Addr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+((Pn-1)*LEN_LastDate_Source_PerPn);
			break;
		default:
			Addr=0;
			break;
	}
#else//
	//要比较
	YMD=MRR(((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
	YM=YMD>>8;
	YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
	YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
	switch(i)
	{
		case 0xe3://上1日电表日冻结
		case 0xee://上2日电表日冻结
		case 0xef://上3日电表日冻结
			Addr=1;
			break;
		default:
			Addr=0;
			break;
	}
#endif

	if(Addr!=0)
	{//是0xe3,0xee,0xef上1-3日电表日冻结
	#if (USER/100)==2//用户标识:山东版
		if(p8RMLib[0]==0xE3)
		{//集中抄表日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f=GetClass2SourceDataAddr(161,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			//判断周期为日冻结电量的电能表飞走,超差
			i=MRR(f+2,3);//取原抄表年月日
			if(i==YMD)
			{//是相临2日抄表
				if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
				{//当前抄表正向有功电能示值有效
					TodayEnergy=MRR(FnDataAddr+6+Office_AFN0DF161_LastDate_Source+6,5);
					LastDayEnergy=MRR(f+6,5);
					if((LastDayEnergy&0xff)!=0xff)//集中抄抄表2类数据日月冻结原始数据(初始值为擦除值0xFF)
					{
						DayAveragePower(Pn,0,0);//日平均正有功功率计算;入口Pn从1开始,Power=有功功率(BCD),Count=本次计算次数
						ERC29_Event_Day(Pn,LastDayEnergy,TodayEnergy);//判断周期为日冻结电量的电能表飞走,超差;入口Pn=测量点号(1开始),LastDayEnergy=上是冻结电量,ToDayEnergy=今日冻结电量
					}
				}
			}
		#else
			if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
			{//当前抄表正向有功电能示值有效
				i=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
				i=GetDateCongealNo(i);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
				if(i!=0xffffffff)
				{//找到相同时标的冻结号
					f=GetClass2DataOfficeAddr(161,Pn);//取类2数据存贮偏移地址,返回0表示没找到
					if(f!=0)
					{
						TodayEnergy=MRR(FnDataAddr+6+Office_AFN0DF161_LastDate_Source+6,5);
						LastDayEnergy=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+f,5);
						DayAveragePower(Pn,0,0);//日平均正有功功率计算;入口Pn从1开始,Power=有功功率(BCD),Count=本次计算次数
						ERC29_Event_Day(Pn,LastDayEnergy,TodayEnergy);//判断周期为日冻结电量的电能表飞走,超差;入口Pn=测量点号(1开始),LastDayEnergy=上是冻结电量,ToDayEnergy=今日冻结电量
					}
				}
			}
		#endif
		}
	#endif
	
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
		MR(ADDR_DATABUFF,Addr,3+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//读原原始数据
		MR(ADDR_DATABUFF,ADDR_TYMDHMS+3,3);//终端抄表年月日
		f=0;//更新标志
		i=ADDR_DATABUFF+3;
	#endif
		//F161 测量点正向有功电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF161_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,161,Pn,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(5*(1+TMaxTariff));
		//F162 测量点正向无功（组合无功1）电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF162_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,162,Pn,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F163 测量点反向有功电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF163_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,163,Pn,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(5*(1+TMaxTariff));
		//F164 测量点反向无功（组合无功1）电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF164_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,164,Pn,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F165 测量点1象限无功电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF165_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,165,Pn,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F166 测量点2象限无功电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF166_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,166,Pn,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F167 测量点3象限无功电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF167_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,167,Pn,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F168 测量点4象限无功电能示值（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF168_LastDate_Source+2,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,168,Pn,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));//补日冻结点数据
		#endif
		}
	#if (((USER/100)==5)&&(Project==300))//上海专变
		//F185 正向有功最大需量及发生时间（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF185_LastDate_Source]!=0xee)
		{
			AddDateCongeal(YMD,185,Pn,FnDataAddr+6+Office_AFN0DF185_LastDate_Source,6+(7*(1+TMaxTariff)));//补日冻结点数据
		}
		//F187 反向有功最大需量及发生时间（总、费率1～M）
		if(p8FnData[6+Office_AFN0DF187_LastDate_Source]!=0xee)
		{
			AddDateCongeal(YMD,187,Pn,FnDataAddr+6+Office_AFN0DF187_LastDate_Source,6+(7*(1+TMaxTariff)));//补日冻结点数据
		}
	#endif
		
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
		if(f!=0)
		{//更新
			MW(ADDR_DATABUFF,Addr,3+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//写原始数据
		}
	#endif
		
	#if RMM_MonthCongealUserDate1==1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
		//补抄到的月初1日数据转为月冻结原始数据
		if(p8FnData[6]==1)
		{//1日数据
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			Addr=GetClass2SourceDataAddr(177,Pn)-2;//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			MR(ADDR_DATABUFF,Addr,2+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//读原原始数据
			MR(ADDR_DATABUFF,ADDR_TYMDHMS+4,2);//终端抄表年月
			f=0;//更新标志
			i=ADDR_DATABUFF+2;
		#endif
			//F177 测量点正向有功电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF161_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,177,Pn,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(5*(1+TMaxTariff));
			//F178 测量点正向无功（组合无功1）电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF162_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,178,Pn,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F179 测量点反向有功电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF163_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,179,Pn,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(5*(1+TMaxTariff));
			//F180 测量点反向无功（组合无功1）电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF164_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,180,Pn,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F181 测量点1象限无功电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF165_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,181,Pn,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F182 测量点2象限无功电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF166_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,182,Pn,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F183 测量点3象限无功电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF167_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,183,Pn,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F184 测量点4象限无功电能示值（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF168_LastDate_Source+3,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,184,Pn,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));//补月冻结点数据
			#endif
			}
		#if (((USER/100)==5)&&(Project==300))//上海专变
			//!!!月冻结最大需量不能用每月1日的最大需量,因过月清0 (上海要求按此做法)
			//F193 正向有功最大需量及发生时间（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF185_LastDate_Source]!=0xee)
			{
				AddMonthCongeal(YM,193,Pn,FnDataAddr+6+Office_AFN0DF185_LastDate_Source,6+(7*(1+TMaxTariff)));//补月冻结点数据
			}
			//F195 反向有功最大需量及发生时间（总、费率1～M）
			if(p8FnData[6+Office_AFN0DF187_LastDate_Source]!=0xee)
			{
				AddMonthCongeal(YM,195,Pn,FnDataAddr+6+Office_AFN0DF187_LastDate_Source,6+(7*(1+TMaxTariff)));//补月冻结点数据
			}
		#endif
			
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			if(f!=0)
			{
				MW(ADDR_DATABUFF,Addr,2+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//写原始数据
			}
		#endif
		}
	#endif
		return;
	}
	
	YMD=MRR(ADDR_TYMDHMS+3,3);//取当前时间年月日
	YM=YMD>>8;
	YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
	YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
	if((p8RMLib[4]&0x08)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据B3=日冻结
		MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//填入抄表时间
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
		Fn=p8RMLib[7];//取日冻结Fn
		i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			x=MRR(i+2,3);//取原抄表年月日
			y=MRR(ADDR_TYMDHMS+3,3);//取当前时间年月日
			if(x!=y)
			{//年月日不同
				p32=(u32 *)(ADDR_Terminal_ClassDataLen);
				MW(FnDataAddr+1,i,p32[0]);
			}
		#if RMM_MonthCongealUserDate1==1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
			if((Fn>=185)&&(Fn<=188))
			{
				Fn+=8;
				i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					x=MRR(i+3,2);//取原抄表年月
					y=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
					if(x!=y)
					{//年月不同
						p32=(u32 *)(ADDR_Terminal_ClassDataLen);
						MW(FnDataAddr+1,i,p32[0]);
					}
				}
			}
		#endif
		}
	#else
		//RMM_DateCongeal_0xEE=1//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
		Fn=p8RMLib[7];//取日冻结Fn
#if (USER/100)==5//上海
	#if (RMM_CompCongealTime==1)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
		if((Fn==185)||(Fn==187))
		{
			if(MRR(FnDataAddr,1)==1)
			{//数据有效标志
				AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补日冻结点数据
			}
		}
		else
		{
			AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补日冻结点数据
		}
	#else
		AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补日冻结点数据
	#endif
#else
		AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补日冻结点数据
#endif
	#if (((USER/100)==5)&&((Project/100)==3))//上海专变
		//!!!月冻结最大需量不能用每月1日的最大需量,因过月清0 (上海要求按此做法)
		#if RMM_MonthCongealUserDate1==1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
			if(MRR(ADDR_TYMDHMS+3,1)==1)//取当前时间日
			{
				if((Fn>=185)&&(Fn<=188))
				{//Fn=185-188日冻结正反有无功最大需量
					Fn+=8;//Fn=193-196上月冻结正反有无功最大需量
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
				}			
			}
		#endif
	#else
		//!!!月冻结最大需量不能用每月1日的最大需量,因过月清0
//		#if RMM_MonthCongealUserDate1==1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
//			if((Fn>=185)&&(Fn<=188))
//			{//Fn=185-188日冻结正反有无功最大需量
//				Fn+=8;//Fn=193-196上月冻结正反有无功最大需量
//				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
//			}
//		#endif
	#endif
	#endif
	}

#if RMDateCongealSour!=0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
	if((p8RMLib[4]&0x10)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据B4=抄表日冻结
		Fn=p8RMLib[8];//取抄表日冻结Fn
		Addr=GetLastRMDateCongealAddr(PORTn,Fn,Pn);//按端口号得到最后次抄表日冻结存储地址,返回=0表示没找到
		if(Addr!=0)
		{
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//填入抄表时间
			DMW(FnDataAddr+1,Addr,Terminal_Class2DataLib[Fn].PnLen);
		}
	}
#endif

	if((p8RMLib[4]&0x20)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据B5=月冻结
#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
		//上海集中器要求上海表次月1日采集表内上3月正向有功电量作为月冻结正向有功电量
		i=Check_AFN04F10_Pn(Pn);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+5,1)==21)
		{//上海规约
			Fn=p8RMLib[9];//取月冻结Fn
			i=p8RMLib[0];
			if(i==137)
			{//上月数据
				//上1月
				if(MRR(FnDataAddr+7,1)!=0xee)
				{
					MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//填入抄表时间
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
				}
				//上2月
				if(MRR(FnDataAddr+7+Terminal_Class2DataLib[Fn].PnLen,1)!=0xee)
				{
					MW(ADDR_TYMDHMS+1,FnDataAddr+1+Terminal_Class2DataLib[Fn].PnLen,5);//填入抄表时间
					YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1+Terminal_Class2DataLib[Fn].PnLen,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
				}
			}
		}
		else
		{
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//填入抄表时间
		#if RMM_MonthCongealUserDate1==0//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
			Fn=p8RMLib[9];//取月冻结Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//补抄上2月数据,在非填0xff模式无上2月
				return;
			}
			i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				x=MRR(i+3,2);//取原抄表年月
				y=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
				if(x!=y)
				{//年月不同
					p32=(u32 *)(ADDR_Terminal_ClassDataLen);
					MW(FnDataAddr+1,i,p32[0]);
				}
			}
			#else
			Fn=p8RMLib[9];//取月冻结Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//补抄上2月数据
				YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
			}
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
			#endif
		#else
			//2类数据当月冻结使用电表每月1日冻结的数据
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
			Fn=p8RMLib[9];//取月冻结Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//补抄上2月数据,没用1日数据代,在非填0xff模式无上2月
				return;
			}
			if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
			{//正反有无功最大需量没用1日数据代,电能表参数修改次数及时间等没用1日数据代
				i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					x=MRR(i+3,2);//取原抄表年月
					y=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
					if(x!=y)
					{//年月不同
						p32=(u32 *)(ADDR_Terminal_ClassDataLen);
						MW(FnDataAddr+1,i,p32[0]);
					}
				}
			}
			#else
			Fn=p8RMLib[9];//取月冻结Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//补抄上2月数据,没用1日数据代
				YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
				return;
			}
			if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
			{//正反有无功最大需量没用1日数据代,电能表参数修改次数及时间等没用1日数据代
				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
			}
			#endif
		#endif
		}
#else//#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
		MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//填入抄表时间
	#if RMM_MonthCongealUserDate1==0//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
		#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
		Fn=p8RMLib[9];//取月冻结Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//补抄上2月数据,在非填0xff模式无上2月
			return;
		}
		i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			x=MRR(i+3,2);//取原抄表年月
			y=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
			if(x!=y)
			{//年月不同
				p32=(u32 *)(ADDR_Terminal_ClassDataLen);
				MW(FnDataAddr+1,i,p32[0]);
			}
		}
		#else
		Fn=p8RMLib[9];//取月冻结Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//补抄上2月数据
			YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
		}
		AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
		#endif
	#else
		//2类数据当月冻结使用电表每月1日冻结的数据
		#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
		Fn=p8RMLib[9];//取月冻结Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//补抄上2月数据,没用1日数据代,在非填0xff模式无上2月
			return;
		}
		if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
		{//正反有无功最大需量没用1日数据代,电能表参数修改次数及时间等没用1日数据代
			i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				x=MRR(i+3,2);//取原抄表年月
				y=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
				if(x!=y)
				{//年月不同
					p32=(u32 *)(ADDR_Terminal_ClassDataLen);
					MW(FnDataAddr+1,i,p32[0]);
				}
			}
		}
		#else
		Fn=p8RMLib[9];//取月冻结Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//补抄上2月数据,没用1日数据代
			YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
			return;
		}
	#if (((USER/100)==5)&&((Project/100)==3))//上海专变
		if((Fn>=213)&&(Fn<=216))
	#else
		if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
	#endif
		{//正反有无功最大需量没用1日数据代,电能表参数修改次数及时间等没用1日数据代
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//补月冻结点数据
		}
		#endif
	#endif
#endif
	}
	
	if((p8RMLib[4]&0x04)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据B2=曲线冻结
		if(((p8RMLib[0]>=0xe4)&&(p8RMLib[0]<=0xea))||(p8RMLib[0]==1))
		{//读电表1-6类负荷曲线
			Addr=6+1+4;
			//负荷记录起始码：A0H，A0H(或 E0H，E0H：本数据块不正确),2 字节
			//负荷记录字节数：1 字节(十六进制)
			//负荷记录存储时间：年、月、日、时、分,  5 字节
			if(p8FnData[6]>=(Addr-7+8))
			{
				if((p8FnData[Addr]!=0xa0)||(p8FnData[Addr+1]!=0xa0))
				{
					return;
				}
				YMDHM=MRR(FnDataAddr+Addr+3,5);//负荷记录存储时间
				Addr+=8;
				//电压、电流、频率：17 字节； 
				//块分隔码：AAH，1 字节；
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+17))
					{
						return;
					}
					else
					{
						//A、B、C 相电压（每相 2 字节，共 6 字节，单位：0.1V）
						AddCurveCongeal(YMDHM,89,Pn,FnDataAddr+Addr,2);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,90,Pn,FnDataAddr+Addr+2,2);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,91,Pn,FnDataAddr+Addr+4,2);//补曲线冻结点数据
						//A、B、C 相电流（每相 3 字节，共 9 字节，单位：0.001A）
						AddCurveCongeal(YMDHM,92,Pn,FnDataAddr+Addr+6,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,93,Pn,FnDataAddr+Addr+9,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,94,Pn,FnDataAddr+Addr+12,3);//补曲线冻结点数据
						Addr+=17+1;
					}
				}
				//有、无功功率：24 字节
				//块分隔码：AAH，1 字节
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+24))
					{
						return;
					}
					else
					{
						//总及 A、B、C 相有功功率（每个 3 字节，共 12 字节，单位：0.0001kW）
						AddCurveCongeal(YMDHM,81,Pn,FnDataAddr+Addr,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,82,Pn,FnDataAddr+Addr+3,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,83,Pn,FnDataAddr+Addr+6,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,84,Pn,FnDataAddr+Addr+9,3);//补曲线冻结点数据
						//总及 A、B、C 相无功功率（每个 3 字节，共 12 字节，单位：0.0001kvar）
						AddCurveCongeal(YMDHM,85,Pn,FnDataAddr+Addr+12,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,86,Pn,FnDataAddr+Addr+15,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,87,Pn,FnDataAddr+Addr+18,3);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,88,Pn,FnDataAddr+Addr+21,3);//补曲线冻结点数据
						Addr+=24+1;
					}
				}
				//功率因数：8 字节
				//块分隔码：AAH，1 字节
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+8))
					{
						return;
					}
					else
					{
						//总及 A、B、C 相功率因数（每个 2 字节，共 8 字节，单位：0.001）
						AddCurveCongeal(YMDHM,105,Pn,FnDataAddr+Addr,2);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,106,Pn,FnDataAddr+Addr+2,2);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,107,Pn,FnDataAddr+Addr+4,2);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,108,Pn,FnDataAddr+Addr+6,2);//补曲线冻结点数据
						Addr+=8+1;
					}
				}
				//有、无功总电能：16 字节
				//块分隔码：AAH，1 字节
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+16))
					{
						return;
					}
					else
					{
						//正向有功总电能,反向有功总电能,组合无功 1 总电能,组合无功 2 总电能
					#if ((USER/100)==5)&&(Project==200)//上海集中器大类号=5时;正向有功=正向有功+反向有功
						i=Check_AFN04F10_Pn(Pn);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
						if(i!=0)
						{
							i--;
							i=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*i)+26,1);//取大小类号
							if((i!=0x02)&&(i!=0x62)&&(i!=0x71)&&(i!=0x72))
							{//大类号=5;正向有功=正向有功+反向有功
								x=MRR(FnDataAddr+Addr,4);
								y=MRR(FnDataAddr+Addr+4,4);
								if(((x&0xff)<=0x99)&&((y&0xff)<=0x99))
								{//正反有功都抄到
									x=bcd_hex(x);
									y=bcd_hex(y);
									x+=y;
									x=hex_bcd(x);
									MWR(x,FnDataAddr+Addr,4);
								}
								else
								{
									MC(0xee,FnDataAddr+Addr,4);
								}
							}
						}
					#endif
						AddCurveCongeal(YMDHM,101,Pn,FnDataAddr+Addr,4);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,103,Pn,FnDataAddr+Addr+4,4);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,102,Pn,FnDataAddr+Addr+8,4);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,104,Pn,FnDataAddr+Addr+12,4);//补曲线冻结点数据
						Addr+=16+1;
					}
				}
				//四象限无功总电能：16 字节
				//块分隔码：AAH，1 字节
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+16))
					{
						return;
					}
					else
					{
						//第一,第二,第三,第四象限无功总电能（4 字节，单位：0.01kvarh）
						AddCurveCongeal(YMDHM,145,Pn,FnDataAddr+Addr,4);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,147,Pn,FnDataAddr+Addr+4,4);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,148,Pn,FnDataAddr+Addr+8,4);//补曲线冻结点数据
						AddCurveCongeal(YMDHM,146,Pn,FnDataAddr+Addr+12,4);//补曲线冻结点数据
						Addr+=16+1;
					}
				}
				//当前需量：6 字节
				//块分隔码：AAH，1 字节
			}
			return;
		}
	}
}


u32 CreateCurveCongealNo(u64 YMDHM)//创建曲线冻结号;返回:原已存在或新创建的冻结号
{
	u32 i;
	u32 *p32;

	i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{//原冻结没相同时标
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_CURVECONGEAL);
		KeyUserPnList();//重点户曲线冻结Pn列表
		MR(ADDR_AFN0D_Congeal_Buff_Start+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
		i=MRR(ADDR_TCongealCount+7,2);
		if((i&0x7fff)>=MaxCurveCongeal)
		{
			i=0x8000;
			MWR(i,ADDR_TCongealCount+7,2);
		}
		i&=0x7fff;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		p32[0]=2;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i);
		p32[3]=LEN_CURVECONGEAL;
		p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
		MWR(YMDHM,ADDR_Class2Congeal_AutoRW+20,5);//冻结有效(冻结时标)
	}
	return i;
}

u32 CreateDateCongealNo(u32 YMD)//创建日冻结号;返回:原已存在或新创建的冻结号
{
	u32 i;
	u32 *p32;
	
	i=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{//原冻结没相同时标
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//清冻结缓冲
		i=MRR(ADDR_TCongealCount+14,1);
		if((i&0x7F)>=MaxDateCongeal)
		{
			i=0x80;
			MWR(i,ADDR_TCongealCount+14,1);
		}
		i&=0x7f;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		p32[0]=0x10002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
		p32[3]=LEN_DATECONGEAL;
		p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
		MWR(YMD,ADDR_Class2Congeal_AutoRW+20,3);//冻结有效(冻结时标)
	}
	return i;
}

u32 CreateMonthCongealNo(u32 YM)//创建月冻结号;返回:原已存在或新创建的冻结号
{
	u32 i;
	u32 *p32;
	
	i=GetMonthCongealNo(YM);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{//原冻结没相同时标
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_MONTHCONGEAL);//清冻结缓冲
		i=MRR(ADDR_TCongealCount+28,1);
		if((i&0x7f)>=MaxMonthCongeal)
		{
			i=0x80;
			MWR(i,ADDR_TCongealCount+28,1);
		}
		i&=0x7f;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		p32[0]=0x30002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
		p32[3]=LEN_MONTHCONGEAL;
		p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
		MWR(YM,ADDR_Class2Congeal_AutoRW+20,2);//冻结有效(冻结时标)
	}
	return i;
}


#if RMDateCongealSour==1//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
u32 LastRMDateCongealYMD(u32 PORTn)//按端口号计算最后次抄表日冻结的时标年月日;入口:通信口号;返回:0表示没
{
	u32 i;
	u32 x;
	u32 YMD;
	u32 HM;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1-1;
			break;
		case RS485_1PORT://RS485_1
			PORTn=2-1;
			break;
		case RS485_2PORT://RS485_2
			PORTn=3-1;
			break;
		case RS485_4PORT://RS485_4(载波)
			PORTn=31-1;
			break;
	}
	HM=MRR(ADDR_AFN04F33+7+((14+(4*24))*PORTn),2);
	PORTn=MRR(ADDR_AFN04F33+3+((14+(4*24))*PORTn),4);
	PORTn<<=1;
	YMD=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TYMDHMS+1,2);
	x=YMD&0xff;
	x=bcd_hex(x);
	x=1<<x;
	if((PORTn&x)&&(i>=HM))
	{
		return YMD;
	}
	else
	{
		for(i=0;i<30;i++)
		{
			YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
			x=YMD&0xff;
			x=bcd_hex(x);
			x=1<<x;
			if(PORTn&x)
			{
				return YMD;
			}
		}
	}
	return 0;
}
u32 GetLastRMDateCongealAddr(u32 PORTn,u32 Fn,u32 Pn)//按端口号得到最后次抄表日冻结存储地址,返回=0表示没找到
{
	u32 i;
	u32 x;
	u32 YMD;
	u32 *p32;
	
	YMD=LastRMDateCongealYMD(PORTn);//按端口号计算最后次抄表日冻结的时标年月日;入口:通信口号;返回:0表示没
	if(YMD==0)
	{
		return 0;
	}
	i=GetRMDateCongealNo(YMD);//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{
		//创建冻结号
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		if(p32[3]!=0x0)
		{//原类2数据冻结AUTOIOW没完成
			return 0;
		}
		i=MRR(ADDR_TCongealCount+21,1);
		if((i&0x7F)>=MaxRMDateCongeal)
		{
			i=0x80;
		}
		x=i;
		i&=0x7f;
		MC(0xff,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),LEN_RMDATECONGEAL);
		DMWR(YMD,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//冻结有效(写冻结时标)
		//冻结号计数+1
		YMD=x&0x80;
		x&=0x7f;
		x+=1;
		if(x>=MaxRMDateCongeal)
		{
			x=0x80;
		}
		x|=YMD;
		MWR(x,ADDR_TCongealCount+21,1);
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	if(x==0)
	{
		return 0;
	}
	return (ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+x);
}
#endif




