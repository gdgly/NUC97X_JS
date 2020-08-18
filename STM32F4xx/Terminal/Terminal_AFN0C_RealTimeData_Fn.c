
//终端类1数据Fn
#include "Project.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../terminal/Terminal_AFN0D_CongealData_Fn.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"


extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];


u32 Class1Data_FnOnly(u32 ClassNo,u32 Fn)//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
{
	u32 i;
	u8 * p8;
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F38+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
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

u32 Class1Data_Fn(u32 ClassNo,u32 Fn)//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
{
	u32 i;
	u8 * p8;
	
	if(Fn==0x0)
	{
		return(0);
	}
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F38+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
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
//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//2类F9 抄表日冻结 正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			case 129://16 F129 当前正向有功电能示值（总、费率1～M）
			case 130://17 F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
			case 133://20 F133 当前一象限无功电能示值（总、费率1～M）
			case 136://23 F136 当前四象限无功电能示值（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,33);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,9);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//2类F10 抄表日冻结 反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			case 131://18 F131 当前反向有功电能示值（总、费率1～M）
			case 132://19 F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
			case 134://21 F134 当前二象限无功电能示值（总、费率1～M）
			case 135://22 F135 当前三象限无功电能示值（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,34);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,10);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//2类F11 抄表日冻结 正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 145://32 F145 当月正向有功最大需量及发生时间（总、费率1～M）
			case 146://33 F146 当月正向无功最大需量及发生时间（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,35);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,11);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//2类F12 抄表日冻结 反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 147://34 F147 当月反向有功最大需量及发生时间（总、费率1～M）
			case 148://35 F148 当月反向无功最大需量及发生时间（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,36);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,12);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
			case 137://24 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
			case 138://25 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
			case 141://28 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
			case 144://31 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,37);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==1//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,9);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
			case 139://26 F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
			case 140://27 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
			case 142://29 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
			case 143://30 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,38);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==1//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,10);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 149://36 F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
			case 150://37 F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,39);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==1//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,11);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
//F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
			case 151://38 F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
			case 152://39 F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
				i=Class1Data_FnOnly(ClassNo,40);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#if RMDateCongealSour==1//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
				i|=Class2Data_FnOnly(ClassNo,12);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			#endif
				return(i);
		}
	}
	return(0);
}


u32 GetClass1DataAddr(u32 Fn,u32 Pn)//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
{
	u32 i;
	u32 Addr;
	u32 Len;
	u32 Tariff;
	u8 * p8;
	u32 * p32;
	
	if(Fn>=Get_Class1Data_MaxFn())
	{
		return(0);
	}
	if(Pn>Terminal_Class1DataLib[Fn].PnMax)
	{
		if(Fn==246)
		{//AFN0CF246当前掉电记录数据映射的Pn
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
	if((Terminal_Class1DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//本方案无此Fn功能
		return(0);
	}
	if(Terminal_Class1DataLib[Fn].PnType==p0Type)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
	{
		Addr=Terminal_Class1DataLib[Fn].Addr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		if(Fn==246)
		{//AFN0CF246当前掉电记录数据映射的Pn
			i=F246MapPn(Pn);//F246掉电记录数据映射Pn;返回0表示无效
			if(i==0)
			{
				Addr=0;
			}
			else
			{
				Addr=Terminal_Class1DataLib[Fn].Addr+(Terminal_Class1DataLib[Fn].PnLen*(i-1));
			}
		}
		else
		{
			Addr=Terminal_Class1DataLib[Fn].Addr+(Terminal_Class1DataLib[Fn].PnLen*(Pn-1));
		}
	}
	if(Addr==0x0)
	{
		return(0);
	}
	if(Fn>=81)
	{
		if(Fn<=121)
		{//小时冻结数据
			p32=(u32 *)(ADDR_HourCongeal_AutoRW);//小时冻结自动写EFLASH控制地址
			if(p32[3]!=0x0)
			{//正在写EFLASH小时冻结数据,用ESRAM小时冻结缓冲中值
				Addr-=ADDR_HOURCONGEAL_START;
				Addr+=ADDR_HOURCONGEAL_BUFF_START;
			}
		}
	}
	p8=(u8 *)(Addr);
	Len=Terminal_Class1DataLib[Fn].PnLen;
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
		case 5://f5 终端控制设置状态;p0
			Len=BitNum(p8[1]);//计算置位为1的个数(0-32)
			Len*=6;
			Len+=2;
			break;
		case 6://f6 终端当前控制状态;p0
			Len=BitNum(p8[2]);//计算置位为1的个数(0-32)
			Len*=8;
			Len+=3;
			break;
		case 11://f11 终端集中抄表状态信息;p0
			Len=p8[0];
			Len*=19;
			Len+=1;
			break;
		case 19://f19 当日总加有功电能量（总、费率1～M）;总加组号
		case 20://f20 当日总加无功电能量（总、费率1～M）;总加组号
		case 21://f21 当月总加有功电能量（总、费率1～M）;总加组号
		case 22://f22 当月总加无功电能量（总、费率1～M）;总加组号
			MR(ADDR_Terminal_ClassDataLen,Addr,1);
			p8=(u8 *)(ADDR_Terminal_ClassDataLen);
			if(p8[0]==0xee)
			{
				p8[0]=0;
				MC(0,Addr,1);
			}
			Len=1+(4*(p8[0]+1));
			break;
		case 41://f41 当日正向有功电能量（总、费率1～M）;测量点号
		case 42://f42 当日正向无功电能量（总、费率1～M）;测量点号
		case 43://f43 当日反向有功电能量（总、费率1～M）;测量点号
		case 44://f44 当日反向无功电能量（总、费率1～M）;测量点号
		case 45://f45 当月正向有功电能量（总、费率1～M）;测量点号
		case 46://f46 当月正向无功电能量（总、费率1～M）;测量点号
		case 47://f47 当月反向有功电能量（总、费率1～M）;测量点号
		case 48://f48 当月反向无功电能量（总、费率1～M）;测量点号
			MR(ADDR_Terminal_ClassDataLen,Addr,1);
			p8=(u8 *)(ADDR_Terminal_ClassDataLen);
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[0]!=Tariff)
			{
				p8[0]=Tariff;
				MC(Tariff,Addr,1);
			}
	#else
			if(p8[0]==0xee)
			{
				p8[0]=Tariff;
				MC(Tariff,Addr,1);
			}
	#endif
			Len=1+(4*(p8[0]+1));
			break;
		case 33://f33 当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）;测量点号
		case 34://f34 当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）;测量点号
		case 37://f37 上月（上一结算日）正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）;测量点号	
		case 38://f38 上月（上一结算日）反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）;测量点号
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(17*(p8[5]+1));
			break;
		case 35://f35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）;测量点号
		case 36://f36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）;测量点号
		case 39://f39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）;测量点号
		case 40://f40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）;测量点号
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(14*(p8[5]+1));
			break;
		case 57://f57 当前A、B、C三相电压、电流2～N次谐波有效值;测量点号
			if((p8[0]==0xee)||(p8[0]==0))
			{
				p8[0]=0;
				Len=1;
			}
			else
			{
				Len=1+(12*18);
			}
			break;
		case 58://f58 当前A、B、C三相电压、电流2～N次谐波含有率;测量点号
			if((p8[0]==0xee)||(p8[0]==0))
			{
				p8[0]=0;
				Len=1+6;
			}
			else
			{
				p8[0]=19;
				Len=1+6+(12*18);
			}
			break;
		case 129://F129	当前正向有功电能示值（总、费率1～M）	测量点号
		case 131://F131	当前反向有功电能示值（总、费率1～M）	测量点号
		case 137://F137	上月（上一结算日）正向有功电能示值（总、费率1～M）	测量点号
		case 139://F139	上月（上一结算日）反向有功电能示值（总、费率1～M）	测量点号
		case 153://F153	第一时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 154://F154	第二时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 155://F155	第三时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 156://F156	第四时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 157://F157	第五时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 158://F158	第六时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 159://F159	第七时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 160://F160	第八时区冻结正向有功电能示值（总、费率1～M）	测量点号
		case 177://F177 当前组合有功电能示值（总、费率1~M）
		case 178://F178 上1 结算日组合有功电能示值（总、费率1~M）
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(5*(p8[5]+1));
			break;
		case 130://F130	当前正向无功（组合无功1）电能示值（总、费率1～M）	测量点号
		case 132://F132	当前反向无功（组合无功1）电能示值（总、费率1～M）	测量点号
		case 133://F133	当前一象限无功电能示值（总、费率1～M）	测量点号
		case 134://F134	当前二象限无功电能示值（总、费率1～M）	测量点号
		case 135://F135	当前三象限无功电能示值（总、费率1～M）	测量点号
		case 136://F136	当前四象限无功电能示值（总、费率1～M）	测量点号
		case 138://F138	上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）	测量点号
		case 140://F140	上月（上一结算日）反向无功（组合无功1）电能示值（总、费率1～M）	测量点号
		case 141://F141	上月（上一结算日）一象限无功电能示值（总、费率1～M）	测量点号
		case 142://F142	上月（上一结算日）二象限无功电能示值（总、费率1～M）	测量点号
		case 143://F143	上月（上一结算日）三象限无功电能示值（总、费率1～M）	测量点号
		case 144://F144	上月（上一结算日）四象限无功电能示值（总、费率1～M）	测量点号
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(4*(p8[5]+1));
			break;
		case 145://F145	当月正向有功最大需量及发生时间（总、费率1～M）	测量点号
		case 146://F146	当月正向无功最大需量及发生时间（总、费率1～M）	测量点号
		case 147://F147	当月反向有功最大需量及发生时间（总、费率1～M）	测量点号
		case 148://F148	当月反向无功最大需量及发生时间（总、费率1～M）	测量点号
		case 149://F149	上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）	测量点号
		case 150://F150	上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）	测量点号
		case 151://F151	上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）	测量点号
		case 152://F152	上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）	测量点号
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(7*(p8[5]+1));
			break;
		case 168://F168	电能表结算信息	测量点号
	#if DefaultTariff==1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(10*(bcd_hex(p8[5]+1)));
			break;
		case 169://F169	集中抄表中继路由信息	测量点号
		case 170://F170	集中抄表电表抄读信息	测量点号
			break;
#if (USER/100)==5//上海
		case 242://F242当日登录主站信息数据单元格式
			Len=MRR(ADDR_AFN0CF242,4);
			if(Len>AFN0CF242MaxCount)//当日登录主站信息最大记录次数
			{
				Len=AFN0CF242MaxCount;
			}
			MWR(Len,ADDR_DATABUFF,4);
			Len*=6;
			MR(ADDR_DATABUFF+4,ADDR_AFN0CF242_Next,Len);
			Len+=4;
			break;
		case 243://当日信号强度
			MR(ADDR_DATABUFF,ADDR_AFN0CF243,24);
			p8=(u8 *)(ADDR_DATABUFF);
			for(Tariff=0;Tariff<24;Tariff++)
			{
				if(p8[Tariff]==0xff)
				{
					p8[Tariff]=0xee;
				}
			}
			break;
#endif
		case 245://终端事件分类数据
			Len=p8[0];
			Len*=2;
			Len+=1;
			break;
	}
	p32=(u32 *)(ADDR_Terminal_ClassDataLen);
	p32[0]=Len;
	return(Addr);
}

u32 GetClass1Data_PnMax(u32 Fn)//取类1数据各Fn的最大Pn
{
	return Terminal_Class1DataLib[Fn].PnMax;
}

void SaveClass1Data(u32 Fn,u32 Pn,u32 SourAddr)//存贮类1数据
{
	u32 i;
	
	i=GetClass1DataAddr(Fn,Pn);//取类1数据存贮地址,返回0表示没找到
	if(i!=0x0)
	{
		MW(SourAddr,i,Terminal_Class1DataLib[Fn].PnLen);
		if(Terminal_Class1DataLib[Fn].PnLen>(LEN_UART2FnDataBuff-1))
		{
			Comm_Ram->ErrorIC|=(1<<18);//B18=抄表数据Fn缓冲长度不够
			Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		}
	}
}


void Class1_F33_F34_F37_F38(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4)//抄表数据组合成F33_F34_F37_F38
{
	u32 i;
	u32 x;
	u8 * p8s;
	u8 * p8d;
	u32 RMTime;
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
	p8s=(u8 *)(FnDataAddr);
	//正向有功
	i=GetClass1DataAddr(Fn1,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
	p8d=(u8 *)(i);
	x=p8d[5];//费率数
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
	for(i=0;i<(6+((x+1)*5));i++)
	{
		p8s[1+i]=p8d[i];
	}
	p8s[6]=x;//费率数
	if(p8s[1]!=0xee)
	{
		RMTime=1;//抄表时间填入标志,0=没,1=填入
	}
	//正向无功（组合无功1）
	i=GetClass1DataAddr(Fn2,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
	p8d=(u8 *)(i);
	if(RMTime==0)//抄表时间填入标志,0=没,1=填入
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//抄表时间填入标志,0=没,1=填入
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1)*4;i++)
	{
		p8s[7+((x+1)*5)+i]=p8d[i];
	}
	//一象限无功
	i=GetClass1DataAddr(Fn3,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
	p8d=(u8 *)(i);
	if(RMTime==0)//抄表时间填入标志,0=没,1=填入
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//抄表时间填入标志,0=没,1=填入
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1)*4;i++)
	{
		p8s[7+((x+1)*9)+i]=p8d[i];
	}
	//四象限无功
	i=GetClass1DataAddr(Fn4,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
	p8d=(u8 *)(i);
	if(RMTime==0)//抄表时间填入标志,0=没,1=填入
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//抄表时间填入标志,0=没,1=填入
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1)*4;i++)
	{
		p8s[7+((x+1)*13)+i]=p8d[i];
	}
}
void Class1_F35_F36_F39_F40(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2)//抄表数据组合成F35_F36_F39_F40
{
	u32 i;
	u32 x;
	u32 y;
	u8 * p8s;
	u8 * p8d;
	u32 RMTime;
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
	p8s=(u8 *)(FnDataAddr);
	//有功
	i=GetClass1DataAddr(Fn1,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
	p8d=(u8 *)(i);
	x=p8d[5];//费率数
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
	for(i=0;i<5;i++)
	{
		p8s[1+i]=p8d[i];
	}
	p8s[6]=x;
	p8d+=6;
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<3;y++)
		{
			p8s[7+(3*i)+y]=p8d[(7*i)+y];
		}
	}
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<4;y++)
		{
			p8s[7+(3*(x+1))+(4*i)+y]=p8d[(7*i)+3+y];
		}
	}
	if(p8s[1]!=0xee)
	{
		RMTime=1;//抄表时间填入标志,0=没,1=填入
	}
	//无功
	i=GetClass1DataAddr(Fn2,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
	p8d=(u8 *)(i);
	if(RMTime==0)//抄表时间填入标志,0=没,1=填入
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//抄表时间填入标志,0=没,1=填入
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<3;y++)
		{
			p8s[7+(7*(x+1))+(3*i)+y]=p8d[(7*i)+y];
		}
	}
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<4;y++)
		{
			p8s[7+(7*(x+1))+(3*(x+1))+(4*i)+y]=p8d[(7*i)+3+y];
		}
	}
}







