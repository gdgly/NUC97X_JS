
//终端类1数据计算
#include "Project.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_impIn.h"
#include "Terminal_SwitchIn.h"
#include "Terminal_AFN0D_CongealData_Init.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "../Terminal/Terminal_USER5xx.h"


extern u8 ADDR_AFN09F1[];//终端版本信息
extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];
extern Terminal_HourCongealLib_TypeDef	Terminal_HourCongealLib[];


void Init_AFN0CF1(void)//初始化终端类1数据F1,版本信息
{
	u32 * p32;
	
	MR(ADDR_DATABUFF,((u32)ADDR_AFN09F1)+12,4);
	MR(ADDR_DATABUFF+4,ADDR_AFN0CF1+12,4);
	p32=(u32 *)(ADDR_DATABUFF);
	if(p32[0]!=p32[1])
	{//版本变更
		ERC1_Event(2);//数据初始化和版本变更记录;f=1数据初始化,f=2版本变更,f=3数据初始化同时版本变更
	}
	MR(ADDR_DATABUFF,(u32)ADDR_AFN09F1,30);
	MR(ADDR_DATABUFF+4,ADDR_TERMINAL+2,8);//8字节设备编号ASCII
	MW(ADDR_DATABUFF,ADDR_AFN0CF1,30);
}

void Init_Class1Data_UpPower(void)//类1数据初始化(上电)
{
	u32 i;
	u32 Fn;
	u8 * p8;

	for(Fn=1;Fn<Get_InitClass1Data_MaxFn();Fn++)
	{
		i=Terminal_Class1DataLib[Fn].Addr;
		if(i!=0x0)
		{
			if((Fn>=81)&&(Fn<=121))
			{//小时冻结数据
				i-=ADDR_HOURCONGEAL_START;
				i+=ADDR_HOURCONGEAL_BUFF_START;
			}
			if(Terminal_Class1DataLib[Fn].Init==0x0)
			{
				if(Terminal_Class1DataLib[Fn].PnType==0x0)
				{
					MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen);
				}
				else
				{
					MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen*Terminal_Class1DataLib[Fn].PnMax);
				}
			}
		}
	}
	p8=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
	p8[0]=0xee;//小时冻结时标=0xee
	p8[1]=0;//小时冻结密度=0(不冻结)
	Init_AFN0CF1();//初始化终端类1数据F1版本信息

//集中抄表信息
	p8=(u8 *)(ADDR_AFN0CF11);//终端集中抄表状态信息
#if ((Project/100)==2)||((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	p8[0]=3;//本项数据块个数n	BIN	1(1-31)
	p8[1]=2;//端口号2
	p8[19+1]=3;//端口号3
	p8[38+1]=31;//终端通信端口号	BIN	1
	MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
	p8+=19;
	MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
	p8+=19;
	MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
#else
	p8[0]=2;//本项数据块个数n	BIN	1(1-31)
	p8[1]=2;//端口号2
	p8[19+1]=3;//端口号3
	MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
	p8+=19;
	MC(0xee,((u32)p8)+8,12);//开始时间,结束时间
#endif

}


void Init_Class1Data_Command(void)//类1数据初始化(命令,全部清除)
{
	u32 i;
	u32 Fn;
	u8 * p8;
	u32 * p32;

//清原冻结
	p32=(u32 *)(ADDR_HourCongeal_AutoRW);//清小时冻结自动IOW
	p32[3]=0;
	MC(0,ADDR_HOURCONGEAL_START+1,1);//无效冻结
//清冻结缓冲
	for(Fn=1;Fn<Get_InitClass1Data_MaxFn();Fn++)
	{
		i=Terminal_Class1DataLib[Fn].Addr;
		if(i!=0x0)
		{
			if((Fn>=81)&&(Fn<=121))
			{//小时冻结数据
				i-=ADDR_HOURCONGEAL_START;
				i+=ADDR_HOURCONGEAL_BUFF_START;
			}
			if(Terminal_Class1DataLib[Fn].PnType==0x0)
			{
				MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen);
			}
			else
			{
				MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen*Terminal_Class1DataLib[Fn].PnMax);
			}
		}
	}
	p8=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
	p8[0]=0xee;//小时冻结时标=0xee
	p8[1]=0;//小时冻结密度=0(不冻结)
	Init_AFN0CF1();//初始化终端类1数据F1版本信息
	Init_SwitchIn();//初始化终端开关量输入
	
	//刚初始化后此时如果心跳终端时钟还是0xee
	Terminal_AFN0CF2_Computer();//终端类1数据F2,终端日历时钟
}


void Terminal_AFN0CF2_Computer(void)//终端类1数据F2,终端日历时钟
{
	u32 i;
	u8 * p8;
	MR(ADDR_AFN0CF2,ADDR_TYMDHMS,6);
	p8=(u8 *)(ADDR_TYMDHMS+6);
	i=p8[0];
	if(i==0x0)
	{
		i=7;
	}
	i<<=5;
	p8=(u8 *)(ADDR_AFN0CF2+4);
	p8[0]|=i;
}

void Terminal_AFN0CF4_Computer(void)//终端类1数据F4,终端上行通信状态
{
	u32 i;
	if(MRR(ADDR_AFN05F29,1)==0x55)//允许禁止终端主动上报
	{
		i=1;
	}
	else
	{
		i=2;
	}
	if(MRR(ADDR_AFN05F27,1)==0x55)//允许禁止终端与主站通话
	{
		i|=0x4;
	}
	else
	{
		i|=0x08;
	}
  MWR(i,ADDR_AFN0CF4,1);//终端通信状态
}


void Terminal_EnergyInc_Computer(u32 f,u32 SourAddr,u32 SourLen,u32 Fn,u32 DateIncAddr,u32 MonthIncAddr)//电能增量和当日当月电能量计算(测量点)
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	u8 * p8s;
	u8 * p8d;
#if ((AFN0CF41F42F43F44!=0)||(AFN0CF45F46F47F48!=0))
	u32 Class1DataAddr;
	u32 DateCongealAddr;
#endif

	MR(ADDR_DATABUFF,ADDR_Inc_User_Flag,2);//读计算标志
	p8d=(u8 *)(ADDR_DATABUFF);
	if((p8d[0]&f)!=0x0)
	{//已计算
		return;
	}
	p8d[0]|=f;//测量点电能增量已计算标志
	p8d[1]=0;//总加组计算标志=0
	MW(ADDR_DATABUFF,ADDR_Inc_User_Flag,2);
	for(Pn=0;Pn<MaxRS485Pn;Pn++)
	{
		//清增量
		MC(0,ADDR_DATABUFF,1+(4*(TMaxTariff+1)));
		p8d[0]=TMaxTariff;//费率数暂为最大
		//读原始示值
		MR(ADDR_DATABUFF+1+(4*(TMaxTariff+1)),SourAddr+((1+((4+SourLen)*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),SourLen*(TMaxTariff+1));
		i=GetClass1DataAddr(Fn,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
		if(i!=0x0)
		{
			p8s=(u8 *)(i);
			for(x=0;x<=TMaxTariff;x++)
			{
				if(x>p8s[5])
				{//费率数超
					break;
				}
				if(p8s[6+(SourLen*x)]==0xee)
				{//当前值=0xee
					continue;
				}
				//取原始值
				a64=MRR(ADDR_DATABUFF+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				//取当前值
				b64=MRR(((u32)p8s)+6+(SourLen*x),SourLen);
				//更新原始值
				MWR(b64,ADDR_DATABUFF+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				if((a64&0xff)==0xee)
				{//原始值=空
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
					if(SourLen==5)
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
				//填入增量
				if(SourLen==4)
				{
					b64*=100;//小数点变4位
				}
				MWR(b64,ADDR_DATABUFF+1+(4*x),4);
				//填入费率数
				p8d[0]=p8s[5];
			}
		}
		//写增量
		MW(ADDR_DATABUFF,SourAddr+((1+((4+SourLen)*(TMaxTariff+1)))*Pn),1+((4+SourLen)*(TMaxTariff+1)));
		//测量点当日电能量
#if AFN0CF41F42F43F44==0//当日电能量AFN0CF41F42F43F44计算;0=示值实时计算,1=当前示值-当日0点冻结
			//0=示值实时计算
		MR(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		p8s=(u8 *)(ADDR_DATABUFF+512);
		p8d=(u8 *)(ADDR_DATABUFF);
		p8s[0]=p8d[0];//费率数
		for(x=0;x<=TMaxTariff;x++)
		{
			i=MRR(ADDR_DATABUFF+512+1+(4*x),4);//
			i=bcd_hex(i);
			i+=MRR(ADDR_DATABUFF+1+(4*x),4);//
			i=hex_bcd(i);
			MWR(i,ADDR_DATABUFF+512+1+(4*x),4);//
		}
		MW(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
#else
			//1=当前示值-当日0点冻结
		i=MRR(ADDR_TYMDHMS+3,3);
		i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
		switch(Fn)
		{
			case 129://正向有功电能
				Class1DataAddr=GetClass1DataAddr(129,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetDateCongealAddr(i,161,Pn+1);//得到日冻结存储地址,返回=0表示没找到
				break;
			case 130://正向无功电能
				Class1DataAddr=GetClass1DataAddr(130,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetDateCongealAddr(i,162,Pn+1);//得到日冻结存储地址,返回=0表示没找到
				break;
			case 131://反向有功电能
				Class1DataAddr=GetClass1DataAddr(131,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetDateCongealAddr(i,163,Pn+1);//得到日冻结存储地址,返回=0表示没找到
				break;
			case 132://反向无功电能
				Class1DataAddr=GetClass1DataAddr(132,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetDateCongealAddr(i,164,Pn+1);//得到日冻结存储地址,返回=0表示没找到
				break;
			default:
				Class1DataAddr=0;//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=0;//得到日冻结存储地址,返回=0表示没找到
				break;
		}
		if((Class1DataAddr==0)||(DateCongealAddr==0))
		{
			MC(0,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			MC(TMaxTariff,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1);
		}
		else
		{
			p8d=(u8 *)(ADDR_DATABUFF+512);
			//读原电能量
			MR(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			p8d[0]=TMaxTariff;//费率数暂为最大
			//读日冻结值
			MR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1)),DateCongealAddr+6,SourLen*(TMaxTariff+1));
			p8s=(u8 *)(Class1DataAddr);
			for(x=0;x<=TMaxTariff;x++)
			{
				if(x>p8s[5])
				{//费率数超
					break;
				}
				//取原始值
				a64=MRR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				if(((a64&0xff)==0xee)||((a64&0xff)==0xFF))
				{//原始值=空
					continue;
				}
				//取当前值
				b64=MRR(((u32)p8s)+6+(SourLen*x),SourLen);
				if(p8s[6+(SourLen*x)]==0xee)
				{//当前值=0xee
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
					if(SourLen==5)
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
				//填入增量
				if(SourLen==4)
				{
					b64*=100;//小数点变4位
				}
				i=b64;
				i=hex_bcd(i);
				MWR(i,ADDR_DATABUFF+512+1+(4*x),4);
				//填入费率数
				p8d[0]=p8s[5];
			}
			//写
			MW(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		}
#endif
		//测量点当月电能量
#if AFN0CF45F46F47F48==0//当月电能量AFN0CF45F46F47F48计算;0=示值实时计算,1=当前示值-当月0点冻结
			//0=示值实时计算
		MR(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		p8s=(u8 *)(ADDR_DATABUFF+512);
		p8d=(u8 *)(ADDR_DATABUFF);
		p8s[0]=p8d[0];//费率数
		for(x=0;x<=TMaxTariff;x++)
		{
			i=MRR(ADDR_DATABUFF+512+1+(4*x),4);//
			i=bcd_hex(i);
			i+=MRR(ADDR_DATABUFF+1+(4*x),4);//
			i=hex_bcd(i);
			MWR(i,ADDR_DATABUFF+512+1+(4*x),4);//
		}
		MW(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
#else
			//1=当前示值-当月0点冻结
		i=MRR(ADDR_TYMDHMS+4,2);
		i=YM_Sub1M(i);//年月减1月,返回减1后的年月
		switch(Fn)
		{
			case 129://正向有功电能
				Class1DataAddr=GetClass1DataAddr(129,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetMonthCongealAddr(i,177,Pn+1);//得到月冻结存储地址,返回=0表示没找到
				break;
			case 130://正向无功电能
				Class1DataAddr=GetClass1DataAddr(130,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetMonthCongealAddr(i,178,Pn+1);//得到月冻结存储地址,返回=0表示没找到
				break;
			case 131://反向有功电能
				Class1DataAddr=GetClass1DataAddr(131,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetMonthCongealAddr(i,179,Pn+1);//得到月冻结存储地址,返回=0表示没找到
				break;
			case 132://反向无功电能
				Class1DataAddr=GetClass1DataAddr(132,Pn+1);//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=GetMonthCongealAddr(i,180,Pn+1);//得到月冻结存储地址,返回=0表示没找到
				break;
			default:
				Class1DataAddr=0;//取类1测量点抄表数据存贮地址,返回0表示没找到
				DateCongealAddr=0;//得到日冻结存储地址,返回=0表示没找到
				break;
		}
		if((Class1DataAddr==0)||(DateCongealAddr==0))
		{
			MC(0,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			MC(TMaxTariff,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1);
		}
		else
		{
			p8d=(u8 *)(ADDR_DATABUFF+512);
			//读原电能量
			MR(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			p8d[0]=TMaxTariff;//费率数暂为最大
			//读月冻结值
			MR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1)),DateCongealAddr+6,SourLen*(TMaxTariff+1));
			p8s=(u8 *)(Class1DataAddr);
			for(x=0;x<=TMaxTariff;x++)
			{
				if(x>p8s[5])
				{//费率数超
					break;
				}
				//取原始值
				a64=MRR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				if(((a64&0xff)==0xee)||((a64&0xff)==0xFF))
				{//原始值=空
					continue;
				}
				//取当前值
				b64=MRR(((u32)p8s)+6+(SourLen*x),SourLen);
				if(p8s[6+(SourLen*x)]==0xee)
				{//当前值=0xee
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
					if(SourLen==5)
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
				//填入增量
				if(SourLen==4)
				{
					b64*=100;//小数点变4位
				}
				i=b64;
				i=hex_bcd(i);
				MWR(i,ADDR_DATABUFF+512+1+(4*x),4);
				//填入费率数
				p8d[0]=p8s[5];
			}
			//写
			MW(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		}
#endif
	}
}


void TotalAddEnergyInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn)//冻结区间总加电能量计算,结果值在DATABUFF
{
	u32 * p32;
	s64 d64a;
	s64 d64b;
	
	//读每次冻结开始时值
	MR(ADDR_DATABUFF,SourAddr+(4*Pn),4);
	//读当前值
	MR(ADDR_DATABUFF+4,CurrAddr+1+((1+(4*(1+TMaxTariff)))*Pn),4);
	//存当前值为开始时值
	MW(ADDR_DATABUFF+4,SourAddr+(4*Pn),4);
	p32=(u32 *)(ADDR_DATABUFF);
	if(p32[0]==0xeeeeeeee)
	{//每次冻结开始时值=0xee
		return;	
	}
	d64a=Data03_kWh(p32[0]);
	d64b=Data03_kWh(p32[1]);
	d64b-=d64a;
	p32[0]=kWh_Data03(d64b);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
}

void PointkWhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn)//冻结区间测量点有功电能量计算,结果值在DATABUFF
{
	u64 d64a;
	u64 d64b;
	u8 * p8;
	//读每次冻结开始时值
	MR(ADDR_DATABUFF,SourAddr+(5*Pn),5);
	//读当前值
	MR(ADDR_DATABUFF+5,CurrAddr+6+((6+(5*(1+TMaxTariff)))*Pn),5);
	//存当前值为开始时值
	MW(ADDR_DATABUFF+5,SourAddr+(5*Pn),5);

	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0xee)
	{
		return;
	}
	if(p8[5]==0xee)
	{
		MC(0xee,ADDR_DATABUFF,5);
		return;
	}
	d64a=MRR(ADDR_DATABUFF,5);//
	d64b=MRR(ADDR_DATABUFF+5,5);//
	d64a=bcd16_hex(d64a);
	d64b=bcd16_hex(d64b);
	if(d64b<d64a)
	{//可能测量点值回0
		d64b+=10000000000;
	}
	d64b-=d64a;
	d64b=hex_bcd16(d64b);
	MWR(d64b,ADDR_DATABUFF,4);//
}

void PointkvarhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn)//冻结区间无功电能量计算,结果值在DATABUFF
{
	u32 i;
	u32 * p32;
	//读每次冻结开始时值
	MR(ADDR_DATABUFF,SourAddr+(4*Pn),4);
	//读当前值
	MR(ADDR_DATABUFF+4,CurrAddr+6+((6+(4*(1+TMaxTariff)))*Pn),4);
	//存当前值为开始时值
	MW(ADDR_DATABUFF+4,SourAddr+(4*Pn),4);

	p32=(u32 *)(ADDR_DATABUFF);
	i=0xeeeeeeee;
	if(p32[0]==i)
	{
		return;
	}
	if(p32[1]==i)
	{
		p32[0]=i;
		return;
	}
	p32[0]=bcd_hex(p32[0]);
	p32[1]=bcd_hex(p32[1]);
	if(p32[1]<p32[0])
	{//可能测量点值回0
		p32[1]+=100000000;
	}
	p32[1]-=p32[0];
	p32[1]=hex_bcd(p32[1]);
	p32[0]=p32[1]<<8;//2位小数到4位小数
}



void Terminal_HourCongeal(void)//小时冻结
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 n;
	u32 m;

	u8 * p8;
	u8 * p8d;
	u32 * p32;

	if(Terminal_Ram->CongealFlags&1)
	{
		return;
	}
	Terminal_Ram->CongealFlags|=1;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	p8=(u8 *)(ADDR_TYMDHMS);
	i=bcd_hex(p8[1]);
	switch(MaxHourCongeal)//最大小时冻结数据个数(密度)0=不冻结;4=15,30,45,0;2=30,0;1=0;12=5,10,15...0;60=1,2,3...0
	{
		case 0://不冻结
			return;
		case 1://00分冻结
			m=3;
			if(i==0x0)
			{
				n=0;
				break;
			}
			return;
		case 2://30,0分冻结
			m=2;
			if(i==30)
			{
				n=0;
				break;
			}
			if(i==0x0)
			{
				n=1;
				break;
			}
			return;
		case 4://15,30,45,0分冻结
			m=1;
			if((i%15)==0x0)
			{
				if(i==0x0)
				{
					n=3;
					break;
				}
				else
				{
					n=i/15;
					n-=1;
					break;
				}
			}
			return;
		case 12://5,10,15...0分冻结
			m=254;
			if((i%5)==0x0)
			{
				if(i==0x0)
				{
					n=11;
					break;
				}
				else
				{
					n=i/5;
					n-=1;
					break;
				}
			}
			return;
		case 60://1,2,3...0分冻结
			m=255;
			if(i==0x0)
			{
				n=59;
			}
			else
			{
				n=(i-1);
			}
			break;
	}
	if(MRR(ADDR_TYMDHMS+1,5)==MRR(ADDR_TCongealCount+2,5))
	{//已冻结
		return;
	}
	//冻结
	MW(ADDR_TYMDHMS+1,ADDR_TCongealCount+2,5);//写上次冻结时标
//	MR(ADDR_DATABUFF,ADDR_TCongealCount,1);
//	p8=(u8 *)(ADDR_DATABUFF);
//	p8[0]|=0x1;//日更新后冻结状态B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结
//	MW(ADDR_DATABUFF,ADDR_TerminalCongealCount,1);
	
	for(Fn=0;Fn<Get_HourCongeal_MaxFn();Fn++)
	{
		if(Terminal_HourCongealLib[Fn].PnType==0x0)
		{
			i=Terminal_HourCongealLib[Fn].DestAddr+(Terminal_HourCongealLib[Fn].DestPnLen*n);
			MR(i,Terminal_HourCongealLib[Fn].SourAddr,Terminal_HourCongealLib[Fn].DestPnLen);
		}
		else
		{
			if(Terminal_HourCongealLib[Fn].PnMax!=0x0)
			{
				for(Pn=0;Pn<Terminal_HourCongealLib[Fn].PnMax;Pn++)
				{
					switch(Fn)
					{
						case (3-1)://83//总加有功总电能量
							TotalAddEnergyInc_Computer(ADDR_AFN0CF83_SOUR,ADDR_AFN0CF19,Pn);//小时冻结总加电能量计算,结果值在DATABUFF
							break;
						case (4-1)://84//总加无功总电能量
							TotalAddEnergyInc_Computer(ADDR_AFN0CF84_SOUR,ADDR_AFN0CF20,Pn);//小时冻结总加电能量计算,结果值在DATABUFF
							break;
						case (20-1)://105//正向有功总电能量
							PointkWhInc_Computer(ADDR_AFN0CF105_SOUR,ADDR_AFN0CF129,Pn);//小时冻结测量点有功电能量计算,结果值在DATABUFF
							break;
						case (21-1)://106//正向无功总电能量
							PointkvarhInc_Computer(ADDR_AFN0CF106_SOUR,ADDR_AFN0CF130,Pn);//小时冻结测量点无功电能量计算,结果值在DATABUFF
							break;
						case (22-1)://107//反向有功总电能量
							PointkWhInc_Computer(ADDR_AFN0CF107_SOUR,ADDR_AFN0CF131,Pn);//小时冻结测量点有功电能量计算,结果值在DATABUFF
							break;
						case (23-1)://108//反向无功总电能量
							PointkvarhInc_Computer(ADDR_AFN0CF108_SOUR,ADDR_AFN0CF132,Pn);//小时冻结测量点无功电能量计算,结果值在DATABUFF
							break;
					}
					i=Terminal_HourCongealLib[Fn].DestAddr+(Terminal_HourCongealLib[Fn].DestPnLen*MaxHourCongeal*Pn)+(Terminal_HourCongealLib[Fn].DestPnLen*n);
					if(Terminal_HourCongealLib[Fn].SourAddr==ADDR_DATABUFF)
					{
						MR(i,ADDR_DATABUFF,Terminal_HourCongealLib[Fn].DestPnLen);
					}
					else
					{
						MR(i,Terminal_HourCongealLib[Fn].SourAddr+(Terminal_HourCongealLib[Fn].SourPnLen*Pn),Terminal_HourCongealLib[Fn].DestPnLen);
					}
				}
			}
		}
	}
	p8=(u8 *)(ADDR_TYMDHMS);
	if(p8[1]==0x0)
	{//0分冻结,转到EFLASH
		p8d=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
		i=p8[2];//冻结时的时(表示上一整点的小时时间，数值范围0～23)
		i=bcd_hex(i);
		if(i==0)
		{
			i=23;
		}
		else
		{
			i--;
		}
		i=hex_bcd(i);
		p8d[0]=i;
		p8d[1]=m;//冻结密度
		p32=(u32 *)(ADDR_HourCongeal_AutoRW);
		p32[0]=2;//2=IOW
		p32[1]=ADDR_HOURCONGEAL_BUFF_START;
		p32[2]=ADDR_HOURCONGEAL_START;
		p32[3]=(ADDR_HOURCONGEAL_BUFF_END)-(ADDR_HOURCONGEAL_BUFF_START);
		Terminal_Ram->CongealFlags&=0xfe;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	}

}

void HourCongeal_AutoRW(void)//小时冻结自动IO读写或清0,完成后结果处理
{
	u32 i;
	u32 x;

	u8 * p8;
	u8 * p8d;
	u32 * p32;
	
	p32=(u32 *)(ADDR_HourCongeal_AutoRW);
	if(p32[3]!=0x0)
	{
    //i=Get_LEN_FLASHSectorWrite(p32[2]);//得到FLASH当前地址扇区可写字节长度
		i=(128*1024)-(p32[2]&0x1ffff);//当前块能写字节
		if(i>p32[3])
		{
			i=p32[3];
		}
		p8=(u8 *)(ADDR_HourCongeal_AutoRW);
		switch(p8[0])
		{
			case 0://IOCLR
				MC(p8[1],p32[2],i);
				break;
			case 1://IOR
				MR(p32[1],p32[2],i);
				break;
			case 2://IOW
				p8d=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);//写入过程密度暂=0冻结无效
				x=p8d[1];
				MW(p32[1],p32[2],i);
				p8d[1]=x;//恢复密度值
				break;
			default:
				p32[3]=0;
				return;
		}
		p32[1]+=i;
		p32[2]+=i;
		p32[3]-=i;
		if(p32[3]==0x0)
		{//长度=0,完成后处理
			PnHourCongealCount();//测量点的小时冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
			MW(ADDR_HOURCONGEAL_BUFF_START+1,ADDR_HOURCONGEAL_START+1,1);//冻结有效(写冻结密度)
			Terminal_Ram->CongealFlags|=1;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
		}
	}
}






void Terminal_AFN0CF17_F18_Computer(u32 f)//终端类1数据F17F18,当前总加功率,入口=0有功=1无功
{
#if MaxTotalAddGroup!=0	
	u32 i;
	u32 x;
	u32 y;
	u32 AddPn;
	u32 Pn;
	u64 d64;
	u64 x64;
	u64 y64;
	u8 * p8;

	for(i=0;i<MaxTotalAddGroup;i++)
	{	
		d64=0;
		AddPn=MRR(ADDR_AFN04F14+(66*i)+1,1);
		if(AddPn<=MaxRS485User)//总加组的测量点数量<=最大基本测量点数
		{
			for(x=0;x<AddPn;x++)
			{
        Pn=MRR(ADDR_AFN04F14+(66*i)+2+x,1)&0x3f;
				if(Pn>=AFN04F25MaxPn)
				{//测量点号超
					continue;
				}
        x64=MRR(ADDR_AFN04F25+(11*Pn),2);
				y64=MRR(ADDR_AFN04F25+(11*Pn)+2,2);
				x64*=y64;
				y=GetClass1DataAddr(25,(Pn+1));//取类1测量点抄表数据存贮地址,返回0表示没找到
				if(y!=0x0)
				{
					p8=(u8 *)(y+5);
					if(f!=0x0)
					{//无功总加
						p8+=12;
					}
					if((MRR(ADDR_AFN04F14+(66*i)+2+x,1)&0x40)==0x0)
					{//正向
						if((p8[2]&0x80)!=0x0)
						{//方向错
							continue;
						}
					}
					else
					{//反向
						if((p8[2]&0x80)==0x0)
						{//方向错
							continue;
						}
					}
					if(p8[0]==0xee)
					{
						continue;//没抄表不算,免作为最小功率值
					}
					else
					{
						y=p8[0]+(p8[1]<<8)+((p8[2]&0x7f)<<16);
						y=bcd_hex(y);
						y64=y;
					}
					x64*=y64;
					if((MRR(ADDR_AFN04F14+(66*i)+2+x,1)&0x80)==0x0)
					{//加
						d64+=x64;
					}
					else
					{//减
						d64-=x64;
					}
				}
			}
		}
		y=d64>>63;
		if(y==0x0)
		{//结果为正
			d64/=10;//4位小数去1位为3位小数
		}
		else
		{//结果为负
			d64=(~d64)+1;
			d64/=10;//4位小数去1位为3位小数
			d64=(~d64)+1;
		}
		x=kW_Data02(d64);//单位统一为kw有3位小数的hex补码转为数据格式02
		
		if(f==0x0)
		{
			p8=(u8 *)(ADDR_AFN0CF17+(2*i));
		}
		else
		{
			p8=(u8 *)(ADDR_AFN0CF18+(2*i));
		}
		p8[0]=x;
		p8[1]=x>>8;
	}
#endif
}

void Terminal_TotalEnergy_Computer(u32 f,u32 PSourAddr,u32 NSourAddr,u32 RemainderAddr,u32 TotalAddr)//总加电能量计算
{
#if MaxTotalAddGroup==0
	u32 i;
	i=MRR(ADDR_Inc_User_Flag+1,1);
	i|=0x0f;//总加组电能增量使用标志,D0-D5分别表示当日有功,当日无功,当月有功,当月无功,ERC22差动事件区间电量,剩余电量
	MWR(i,ADDR_Inc_User_Flag+1,1);
#else
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u64 d64;
	u8 * p8;
	u32 * p32s;
	u32 * p32d;
	u64 * p64;
	u32 TariffMax;
	
	TariffMax=4;//总加组费率数固定为4
	MR(ADDR_DATABUFF,ADDR_Inc_User_Flag,2);//读计算标志
	p8=(u8 *)(ADDR_DATABUFF);
	if((p8[0]&0x0f)!=0x0f)
	{//测量点增量没计算
		return;
	}
	if((p8[1]&0x3f)==0x3f)
	{//全已计算
		MC(0,ADDR_Inc_User_Flag,2);
		return;
	}
	if((p8[1]&f)!=0x0)
	{//已计算
		return;
	}
	p8[1]|=f;
	MW(ADDR_DATABUFF+1,ADDR_Inc_User_Flag+1,1);
	p64=(u64 *)(ADDR_DATABUFF);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)//总加组计数
	{
		//读原总加电量
		MR(ADDR_DATABUFF+512,TotalAddr+1+((1+(4*(1+TMaxTariff)))*Pn),(4*(1+TMaxTariff)));
		//原总加电量转为8字节的HEX,统一为KWH
		p32s=(u32 *)(ADDR_DATABUFF+512);
		for(i=0;i<=TariffMax;i++)
		{
			d64=bcd_hex(p32s[i]&0x0fffffff);
			if((p32s[i]&0x40000000)!=0x0)
			{//mwh
				d64*=1000;
			}
			if((p32s[i]&0x10000000)!=0x0)
			{//负
				d64=(~d64)+1;
			}
			p64[i]=d64;
		}
		//读原总加电量的小数位(hex补码)
		MR(ADDR_DATABUFF+512,RemainderAddr+(4*(1+TMaxTariff)*Pn),4*(1+TMaxTariff));
		//各测量点总加
		p32d=(u32 *)(ADDR_DATABUFF+1024);
		for(x=0;x<MRR(ADDR_AFN04F14+((2+64)*Pn)+1,1);x++)//总加组的测量点数量
		{
			i=MRR(ADDR_AFN04F14+((2+64)*Pn)+2+x,1)&0x3f;//测量点号0-63表示1-64
			if(i>=MaxRS485Pn)
			{
				continue;
			}
			if((MRR(ADDR_AFN04F14+((2+64)*Pn)+2+x,1)&0x40)==0x0)
			{//正向
				if(PSourAddr==ADDR_PKWH_INC)
				{
					MR(ADDR_DATABUFF+1024,PSourAddr+1+((1+(9*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
				else
				{
					MR(ADDR_DATABUFF+1024,PSourAddr+1+((1+(8*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
			}
			else
			{//反向
				if(NSourAddr==ADDR_NKWH_INC)
				{
					MR(ADDR_DATABUFF+1024,NSourAddr+1+((1+(9*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
				else
				{
					MR(ADDR_DATABUFF+1024,NSourAddr+1+((1+(8*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
			}
      y=MRR(ADDR_AFN04F25+(11*i),2);
			i=MRR(ADDR_AFN04F25+(11*i)+2,2);
			i*=y;//电压互感器倍率*电流互感器倍率
			for(y=0;y<=TariffMax;y++)
			{
				d64=p32d[y];//取增量
				d64*=i;//增量*倍率
				if((MRR(ADDR_AFN04F14+((2+64)*Pn)+2+x,1)&0x80)!=0x0)
				{//减测量点值
					d64=(~d64)+1;
				}
				if((p32s[y]&0x80000000)==0x0)
				{//原余数正
					d64+=p32s[y];
				}
				else
				{//原余数负
					d64-=((~p32s[y])+1);
				}	
				if((d64>>63)==0x0)
				{//增量为正
					p64[y]+=d64/10000;
					p32s[y]=d64%10000;
				}
				else
				{//增量为负
					d64=(~d64)+1;
					p64[y]-=d64/10000;
					p32s[y]=(~(d64%10000))+1;
				}
			}
		}
		//总加电量转为4字节BCD格式
		for(y=0;y<=TariffMax;y++)
		{
			d64=p64[y];
			if((d64>>63)==0x0)
			{//正
				f=0;
			}
			else
			{//负
				f=1;
				d64=(~d64)+1;
			}
			if(d64>=10000000)
			{//MWH
				i=d64%1000;
				d64/=1000;
				if(f==0x0)
				{
					p32s[y]+=(i*10000);
				}
				else
				{
					p32s[y]-=(i*10000);
				}
				f|=0x4;
			}
			i=hex_bcd(d64);
			i|=f<<28;
			p32d[y]=i;
		}
		p8=(u8 *)(ADDR_DATABUFF+(1024-1));
		p8[0]=TariffMax;//费率数
		//写总加电量
		MW(ADDR_DATABUFF+(1024-1),TotalAddr+((1+(4*(1+TMaxTariff)))*Pn),1+(4*(1+TMaxTariff)));
		//写原总加电量的小数位(hex补码)
		MW(ADDR_DATABUFF+512,RemainderAddr+(4*(1+TMaxTariff)*Pn),4*(1+TMaxTariff));
	}
#endif
}

void ERC22_TotalEnergy_Computer(void)//ERC22差动事件定时区间总加电量计算
{
#if (MaxTotalAddGroup==0)||(MaxDiffGroup==0)
	u32 i;
	i=MRR(ADDR_Inc_User_Flag+1,1);
	i|=0x10;//总加组电能增量使用标志,D0-D5分别表示当日有功,当日无功,当月有功,当月无功,ERC22差动事件区间电量,剩余电量
	MWR(i,ADDR_Inc_User_Flag+1,1);
#else
	u32 i;
	u32 Fn;
	u32 Pn;
	u64 a64;
	u64 b64;
	u32 * p32;
	
	//清总加电量增量存放区
	p32=(u32 *)(ADDR_DATABUFF+2048);
	for(i=0;i<(2048/4);i++)
	{
		p32[i]=0;
	}
	Terminal_TotalEnergy_Computer(0x10,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_ERC22_Remainder,ADDR_DATABUFF+2048);//总加电量增量计算

	for(Fn=0;Fn<MaxDiffGroup;Fn++)
	{
		for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
		{
			i=MRR(ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Fn)+(4*Pn),4);//读原总加电量 数据格式03
			a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			i=MRR(ADDR_DATABUFF+2048+1+((1+4+(4*TMaxTariff))*Pn),4);
			i&=0xefffffff;//去符号,总是用正值
			b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			a64+=b64;
			a64=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
			MWR(a64,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Fn)+(4*Pn),4);//写寄存器数据到存贮器,Byte<=8
		}
	}
#endif
}


//(ADC20MA-ADC4MA)*Gain=(直流模拟量量程终止值-直流模拟量量程起始值)
//Gain=(直流模拟量量程终止值-直流模拟量量程起始值)/(ADC20MA-ADC4MA)
void Terminal_AFN0CF73_Computer(void)//直流模拟量计算
{
#if (AFN0CF73MaxPn!=0)
	u32 i;
	u32 ADC4MA;//电流环4MA输入时ADC转换值
	u32 ADC20MA;//电流环20MA输入时ADC转换值
	s64 x;
	s64 y;
	double Gain;
	
//	ADC4MA=0;//电流环4MA输入时ADC转换值
//	ADC20MA=0xfff;//电流环20MA输入时ADC转换值
	ADC4MA=MRR(ADDR_ADC4MA,2);
	ADC20MA=MRR(ADDR_ADC20MA,2);
	
	i=MRR(ADDR_AFN04F61,1);//直流模拟量接入参数
	i&=1;
	if(i==0)
	{
		MC(0xee,ADDR_AFN0CF73,2);
	}
	else
	{//接入
		x=MRR(ADDR_AFN04F81,2);//直流模拟量量程起始值
		y=MRR(ADDR_AFN04F81+2,2);//直流模拟量量程终止值
		x=Data02_kW((u32)x);//数据格式02转为单位统一为kw的hex补码,有3位小数
		y=Data02_kW((u32)y);//数据格式02转为单位统一为kw的hex补码,有3位小数
		Gain=y-x;
		i=ADC20MA-ADC4MA;
		if(i==0)
		{//错
			MC(0xee,ADDR_AFN0CF73,2);
			return;
		}
		else
		{
			if(ADC20MA<ADC4MA)
			{//ADC20MA<ADC4MA
				MC(0xee,ADDR_AFN0CF73,2);
				return;
			}
			else
			{
				Gain/=i;
			}
		}
		i=MRR(ADDR_UDC,2);
		if(i<ADC4MA)
		{
			i=ADC4MA;
		}
		if(i>ADC20MA)
		{
			i=ADC20MA;
		}
		i-=ADC4MA;
		Gain*=i;
		y=(s64)Gain;
		y+=x;
		i=kW_Data02((u64)y);//单位统一为kw有3位小数的hex补码转为数据格式02
		MWR(i,ADDR_AFN0CF73,2);
	}
#endif
}



void Terminal_AFN0CF5_Computer(void)//终端类1数据F5,终端控制设置状态
{
	u32 i;
	u8 * p8s;
	u8 * p8d;
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=	
	u32 x;
#endif

	i=0;
	p8s=(u8 *)(ADDR_Terminal_Protect);//终端保电
	if(p8s[0]!=0x0)
	{
		i|=1;
	}
	if(MRR(ADDR_AFN05F28,1)==0x55)//终端剔除投入解除
	{
		i|=2;
	}
	if(MRR(ADDR_AFN05F26,1)==0x55)//催费告警投入解除
	{
		i|=4;
	}
	p8d=(u8 *)(ADDR_AFN0CF5);//终端控制设置状态
	p8d[0]=i;
	p8d[1]=0;//总加组有效标志位
	
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=	
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		p8d[1]=(p8d[1]<<1)+0x1;
	}
	p8d+=2;
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		p8d[0]=MRR(ADDR_AFN05F9+(3*i)+2,1);//功控定值方案号
		p8d[1]=MRR(ADDR_AFN05F9+(3*i)+1,1);//功控时段有效标志位
		p8d[2]=0;//功控状态:D0时段控,D1厂休控,D2营业报停控,D3当前功率下浮控
		if(MRR(ADDR_AFN05F9+(3*i),1)==0x55)
		{//时段控
			p8d[2]|=1;
		}
		if(MRR(ADDR_AFN05F10+(1*i),1)==0x55)
		{//厂休控
			p8d[2]|=2;
		}
		if(MRR(ADDR_AFN05F11+(1*i),1)==0x55)
		{//营业报停控
			p8d[2]|=4;
		}
		x=MRR(ADDR_AFN05F12+(9*i),1);
		if((x==0x55)||(x==0x5a))
		{//当前功率下浮控
			p8d[2]|=8;
		}
		p8d[3]=0;//电控状态:D0月电控,D1购电控
		if(MRR(ADDR_AFN05F15+(1*i),1)==0x55)
		{//月电控
			p8d[3]|=1;
		}
		if(MRR(ADDR_AFN05F16+(1*i),1)==0x55)
		{//购电控
			p8d[3]|=2;
		}
		p8d[4]=MRR(ADDR_AFN04F45+(1*i),1);//功控轮次状态
		p8d[5]=MRR(ADDR_AFN04F48+(1*i),1);//电控轮次状态		
		p8d+=6;
	}
#endif
}

void Terminal_AFN0CF6_Computer(void)//终端类1数据F6,终端当前控制状态
{
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=	
	u32 i;
	u32 Pn;
	u8 * p8s;
	u8 * p8d;

	p8d=(u8 *)(ADDR_AFN0CF6);
//遥控跳闸输出状态
	p8s=(u8 *)(ADDR_Terminal_Remote_State);
	p8d[0]=0;
	for(i=0;i<MaxControlRound;i++)
	{
		if(p8s[0]>=102)
		{
			p8d[0]|=1<<i;
		}
		p8s+=Terminal_Ctrl_Pn_Len;
	}
//当前催费告警状态
	p8s=(u8 *)(ADDR_Terminal_Attr_State);
	p8d[1]=p8s[0];
//总加组有效标志位
	p8d[2]=0;
	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[2]=(p8d[2]<<1)+0x1;
	}
	p8d+=3;
	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		//当前功控定值
		p8s=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]!=0x0)
		{
			MR((u32)p8d,ADDR_kWCtrl1_FixedValue+(Pn*2),2);
		}
		else
		{
			p8s=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
			if(p8s[0]!=0x0)
			{
				MR((u32)p8d,ADDR_AFN04F44+(8*Pn)+6,2);
			}
			else
			{
				p8s=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
				if(p8s[0]!=0x0)
				{
					MR((u32)p8d,ADDR_AFN04F42+(6*Pn),2);
				}
				else
				{
					p8s=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
					if(p8s[0]!=0x0)
					{
						MR((u32)p8d,((u32)p8s)+6,2);
					}
					else
					{
						p8d[0]=0xee;
						p8d[1]=0xee;
					}
				}
			}
		}
		p8d+=2;
		//当前功率下浮控浮动系数
		p8d[0]=MRR(ADDR_AFN05F12+(9*Pn)+2,1);
		p8d++;
		//功控跳闸输出状态
		p8d[0]=0;
		p8s=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8s=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8s=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8s=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8d++;
		//月电控跳闸输出状态
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]=p8s[1];
		p8d++;
		//购电控跳闸输出状态
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]=p8s[1];
		p8d++;
		//功控越限告警状态
		p8d[0]=0;
		p8s=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=8;
		}
		p8s=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=4;
		}
		p8s=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=2;
		}
		p8s=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=1;
		}
		p8d++;
		//电控越限告警状态
		p8d[0]=0;
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=1;
		}
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=2;
		}
		p8d++;
	}
#endif
}

void Terminal_ResidualEnergy_Computer(void)//剩余电量(费)计算
{
#if (Project/100)!=3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=	
	u32 i;
	i=MRR(ADDR_Inc_User_Flag+1,1);
	i|=0x20;//总加组电能增量使用标志,D0-D5分别表示当日有功,当日无功,当月有功,当月无功,ERC22差动事件区间电量,剩余电量
	MWR(i,ADDR_Inc_User_Flag+1,1);
#else
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	u64 c64;
	u8 * p8;
	u32 * p32;
	
	//清总加电量增量存放区
	p32=(u32 *)(ADDR_DATABUFF+2048);
	for(i=0;i<(2048/4);i++)
	{
		p32[i]=0;
	}
	Terminal_TotalEnergy_Computer(0x20,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_AFN0CF23_Remainder,ADDR_DATABUFF+2048);//总加电量增量计算

	//购电量或购电费判断(终端电能量费率是否全相同)
	Pn=ADDR_AFN04F22+1;
	x=MRR(Pn,4);
	Pn+=4;
	for(i=1;i<TMaxTariff;i++)
	{
		if(MRR(Pn,4)!=x)
		{
			break;
		}
		Pn+=4;
	}
	if(i==TMaxTariff)
	{//终端电能量费率全相同,购电量
		for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
		{
			i=MRR(ADDR_AFN05F16+Pn,1);
			if(i!=0x55)
			{//解除;购电控未投入时，剩余电量不减
				continue;
			}
			p8=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
			p8[6]=0;//购电类别在剩余电量(费)计算时识别,0=购电量,1=购电费
			i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读当前剩余电量(费)数据格式03
			a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			b64=MRR(ADDR_AFN0CF23_DP+(2*Pn),2);//读当前剩余电量单位G=1时(MWH)小数点后数据,格式hex=0.000
			if((i&0x10000000)!=0x0)
			{//数据格式03为负值
				b64=(~b64)+1;
			}
			a64+=b64;
			i=MRR(ADDR_DATABUFF+2048+1+((1+4+(4*TMaxTariff))*Pn),4);
			i&=0xefffffff;//去符号,总是用正值
			b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			a64-=b64;
			a64=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
			MWR(a64,ADDR_AFN0CF23+(4*Pn),4);//写寄存器数据到存贮器,Byte<=8
			a64>>=32;
			MWR(a64,ADDR_AFN0CF23_DP+(2*Pn),2);//写寄存器数据到存贮器,Byte<=8
		}
	}
	else
	{//终端电能量费率不全为1,购电费
		for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
		{
			i=MRR(ADDR_AFN05F16+Pn,1);
			if(i!=0x55)
			{//解除;购电控未投入时，剩余电费不减
				continue;
			}
			p8=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
			p8[6]=1;//购电类别在剩余电量(费)计算时识别,0=购电量,1=购电费
			i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读当前剩余电量(费)数据格式03
			a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			b64=MRR(ADDR_AFN0CF23_DP+(2*Pn),2);//读当前剩余电量单位G=1时(MWH)小数点后数据,格式hex=0.000
			if((i&0x10000000)!=0x0)
			{//数据格式03为负值
				b64=(~b64)+1;
			}
			a64+=b64;
			for(x=0;x<TMaxTariff;x++)
			{
				i=MRR(ADDR_DATABUFF+2048+5+((1+4+(4*TMaxTariff))*Pn)+(4*x),4);//取费率电量增量
				i&=0xefffffff;//去符号,总是用正值
				b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
				i=MRR(ADDR_AFN04F22+1+(4*x),4);//取终端电能量费率
				i&=0xefffffff;//去符号,总是用正值
				c64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
				b64*=c64;
				a64-=b64;
			}
			a64=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
			MWR(a64,ADDR_AFN0CF23+(4*Pn),4);//写寄存器数据到存贮器,Byte<=8
			a64>>=32;
			MWR(a64,ADDR_AFN0CF23_DP+(2*Pn),2);//写寄存器数据到存贮器,Byte<=8
		}
	}
#endif
}

void Terminal_AFN0CF12_Computer(void)//控制输出回路开关接入状态量及变位标志
{
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=	
	u32 i;
	u32 x;
	u8 *p8s;
	u8 *p8d;
	if(Terminal_Ram->ContrlLoopCheck_S_Timer==0)//36 控制输出回路检测有效等待秒定时器
	{
//1BYTE 状态ST BIN 1 D0～D7 按顺序对位，2 位一组，对应1～4 路控制输出的常开、常闭触点接入开关跳闸回路状态量的状态ST，置“0”：“未接入”状态；置“1”：“接入”状态。
//1BYTE 变位CD BIN 1 D0～D7 按顺序对位，2 为一组，对应1～4 路控制输出的常开、常闭触点接入开关跳闸回路状态量的变位CD，置“0”：自前次遥信传送后无状态变化；置“1”：自前次遥信传送后至少有一次状态变化。
		MR(ADDR_DATABUFF,ADDR_AFN0CF12,2);//控制输出回路开关接入状态量及变位标志
		p8s=(u8*)(ADDR_ContrlUARTData+2);//控制模块输出输入数据
		p8d=(u8*)(ADDR_DATABUFF);
//1BYTE	从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
//1BYTE 从控制模块输入的断线标志字节1
		i=p8s[0];
		i=(~i);
		x=i&0xf;
		i^=p8d[0];
		i&=0xf;
		p8d[0]=x;
		p8d[1]|=i;
		p8d[1]&=0xf;
		MW(ADDR_DATABUFF,ADDR_AFN0CF12,2);//控制输出回路开关接入状态量及变位标志
		if(i!=0x0)
		{
			ERC36_Event(i,x);//控制输出回路开关接入状态量变位记录;入口：CD=变位,ST=状态
		}
	}
#endif
}



void Terminal_Class1Data_Computer_Ext(void)//终端类1数据计算扩展任务
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
#if (USER/100)==5//用户标识:上海系
	u16 *p16;
	u32 *p32;
	u64 YMDHM;
	u64 YMDHMsour;
	u32 CongealNo;
#endif

	switch(Terminal_Ram->ExtTask1)//扩展任务1
	{
		case 0://终端事件分类
			p8=(u8*)(ADDR_AFN0CF245);//终端事件分类
			x=0;//终端当前事件类型总数n
			for(i=0;i<MaxERC;i++)
			{
				y=ADDR_AFN0EF3+(i*(2+(15*LEN_TERMINAL_EVENT)));
				y=MRR(y,2);
				if(y>15)
				{
					y=15;
				}
				if(y!=0)
				{
					p8[1+(2*x)+0]=i+1;//事件代码ERC
					p8[1+(2*x)+1]=y;//事件数量
					x++;//终端当前事件类型总数n
				}
			}
			p8[0]=x;//总数n
			break;
		case 1://当前终端电池状态
			p8=(u8*)ADDR_AFN0CF247;//当前终端电池状态
//		#if (	((USER/100)==7)||((USER/100)==5) )//浙江,上海
			if(Comm_Ram->UBACKUP<0x480)//可充电池启动工作最小电压4.80V
			{
				p8[0]=0;//D0：置"1"电池未失效，置"0"电池失效
				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45终端电池失效事件(上海扩展)
				if(p8[0]==0)
				{
					p8[0]=1;
					ERC45_Event();//终端电池失效事件
				}
			}
			if(Comm_Ram->UBACKUP>=0x500)//可充电池启动工作最小电压5.00V
			{
				p8[0]=1;//D0：置"1"电池未失效，置"0"电池失效
				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45终端电池失效事件(上海扩展)
				p8[0]=0;
			}
//		#else
//			//时钟电池电压(内部)(半字对齐)
//			if(Comm_Ram->URTC<0x200)//时钟电池电压最小电压2.00V
//			{
//				p8[0]=0;//D0：置"1"电池未失效，置"0"电池失效
//				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45终端电池失效事件(上海扩展)
//				if(p8[0]==0)
//				{
//					p8[0]=1;
//					ERC45_Event();//终端电池失效事件
//				}
//			}
//			if(Comm_Ram->URTC>=0x220)//2.2v
//			{
//				p8[0]=1;//D0：置"1"电池未失效，置"0"电池失效
//				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45终端电池失效事件(上海扩展)
//				p8[0]=0;
//			}
//		#endif
			break;
	#if (USER/100)==5//用户标识:上海系
		case 2://当日信号强度
			if(UART1Ctrl->LinkTask<8)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{//8=初始化AT命令
				i=0x99;//0x99未知或无法获取
			}
			else
			{
				i=Terminal_Ram->SignaldBm;//信号强度0-31或99
			}
			if(i==0x99)
			{
				i=0xee;//0x99未知或无法获取
			}
			else
			{
				i=bcd_hex(i);//8位压缩BCD数转为HEX
				if(i>31)
				{
					i=31;
				}
				i*=2;
				i=113-i;
				i=~i;//16进制的补码
				i++;
			}
			x=Comm_Ram->TYMDHMS[2];//取时
			x=bcd_hex(x);
			if(x>23)
			{
				x=23;
			}
			DMWR(i,ADDR_AFN0CF243+x,1);
			//日平均信号强度
			x=0;
			y=0;
//			p8=(u8*)(ADDR_AFN0CF243);
			p8=(u8*)(ADDR_DATABUFF);
			MR(ADDR_DATABUFF,ADDR_AFN0CF243,24);
			for(i=0;i<24;i++)
			{
				if((p8[i]!=0xee)&&(p8[i]!=0xff))
				{
					y++;
					x+=(0xffffff00+p8[i]);
				}
			}
			if(x==0)
			{
				x=0xee;
			}
			else
			{
				if(y==0)
				{
					x=0xee;
				}
				else
				{
					x=~x;
					x++;
					x/=y;
					x|=1;//奇数值
					x=~x;
					x++;
				}
			}
			MWR(x,ADDR_AFN0CF243_Per,1);
			break;
		case 3://终端线电压
			p8=(u8*)ADDR_AFN0CF191_Per;//1分钟平均线电压
			if(p8[0]!=Comm_Ram->TYMDHMS[0])
			{//上次参与平均的实时钟秒
				//取实时值
				p8[0]=Comm_Ram->TYMDHMS[0];
				GetACSample(0x020Dff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
				i=Get_ADDR_UARTnTx(ACSAMPLEPORT)+14;
				MR(ADDR_AFN0CF191,ADDR_TYMDHMS+1,5);//线电压抄表时间
				MR(ADDR_AFN0CF191+5,i,6);//线电压
				//1分钟平均
				p16=(u16*)(ADDR_AFN0CF191+5);
				p32=(u32*)(ADDR_AFN0CF191_Per+2);
				for(i=0;i<3;i++)
				{
					x=p16[i];
					x=bcd_hex(x);
					y=p32[i];
					y+=x;
					p32[i]=y;
				}
				i=p8[1];
				i++;
				p8[1]=i;
				if(i>=60)
				{
					p8[1]=0;
					MR(ADDR_AFN0CF191_Per+14,ADDR_TYMDHMS+1,5);//计算时间
					p16=(u16*)(ADDR_AFN0CF191_Per+19);
					for(x=0;x<3;x++)
					{
						y=p32[x];
						y/=i;
						y=hex_bcd(y);
						p16[x]=y;
						p32[x]=0;
					}
				}
			}
			//每5分曲线冻结
			i=Comm_Ram->TYMDHMS[1];//当前RTC分
			i=bcd_hex(i);
			i/=5;//每5分
			i*=5;
			i=hex_bcd(i);
			YMDHM=MRR(ADDR_TYMDHMS+2,4);
			YMDHM<<=8;
			YMDHM|=i;
			YMDHMsour=MRR(ADDR_CURVECONGEAL_ExtCount+2,5);
			if(YMDHM!=YMDHMsour)
			{//没冻结
				CongealNo=GetCurveCongealNo_Ext(YMDHM);//得到扩展曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
				if(CongealNo!=0xffffffff)
				{//找到原冻结
					//更新
					MWR(YMDHM,ADDR_DATABUFF,6);
					MR(ADDR_DATABUFF+6,ADDR_AFN0CF191_Per+19,6);
					MW(ADDR_DATABUFF,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext);
					//最近冻结时间
					MWR(YMDHM,ADDR_CURVECONGEAL_ExtCount+2,5);
				}
				else
				{//没找到
					//当前冻结号
					CongealNo=MRR(ADDR_CURVECONGEAL_ExtCount,2);//B0-B14曲线冻结计数,b15=1表示曲线冻结计数满
					CongealNo&=0x7fff;
					if(CongealNo>=MaxCurveCongeal_Ext)
					{
						CongealNo=0;
					}
					//检查是否擦除值班0xff
					MR(ADDR_DATABUFF,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext);
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<LEN_CURVECONGEAL_Ext;i++)
					{
						if(p8[i]!=0xff)
						{
							break;
						}
					}
					if(i!=LEN_CURVECONGEAL_Ext)
					{
						//先擦除1天288点后再写入
						if((CongealNo+288)<=MaxCurveCongeal_Ext)
						{
							MC(0xff,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext*288);
						}
						else
						{
							x=MaxCurveCongeal_Ext-CongealNo;
							MC(0xff,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext*x);
							x=288-x;
							MC(0xff,ADDR_CURVECONGEAL_ExtStart,LEN_CURVECONGEAL_Ext*x);
						}
					}
					//写入
					MWR(YMDHM,ADDR_DATABUFF,6);
					MR(ADDR_DATABUFF+6,ADDR_AFN0CF191_Per+19,6);
					DMW(ADDR_DATABUFF,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext);
					//最近冻结时间
					MWR(YMDHM,ADDR_CURVECONGEAL_ExtCount+2,5);
					//冻结计数+1
					CongealNo++;
					if(CongealNo>=MaxCurveCongeal_Ext)
					{
						CongealNo=0x8000;
					}
					i=MRR(ADDR_CURVECONGEAL_ExtCount,2);//B0-B14曲线冻结计数,b15=1表示曲线冻结计数满
					i&=0x8000;
					CongealNo|=i;
					MWR(CongealNo,ADDR_CURVECONGEAL_ExtCount,2);//B0-B14曲线冻结计数,b15=1表示曲线冻结计数满
				}
			}
			break;
	#endif
		default:
			Terminal_Ram->ExtTask1=0;//用户扩展任务1
			return;
	}
	Terminal_Ram->ExtTask1++;//用户扩展任务1
}



void Terminal_Class1Data_Computer(void)//终端类1数据计算
{

	Terminal_impIn_Energy();//终端脉冲输入-电能
	Terminal_impIn_Demand();//终端脉冲输入-功率和最大需量

	switch(Terminal_Ram->I2SUBTask1)
	{
		case 0:
			Terminal_AFN0CF2_Computer();//终端类1数据F2,终端日历时钟
			break;
		case 1:
			Terminal_AFN0CF4_Computer();//终端类1数据F4,终端上行通信状态
			break;
		case 2:
			Terminal_HourCongeal();//小时冻结	
			break;
		case 3:
			Terminal_SwitchIn();//终端开关量输入
			break;

		case 4:
			Terminal_EnergyInc_Computer(0x1,ADDR_PKWH_INC,5,129,ADDR_AFN0CF41,ADDR_AFN0CF45);//正向有功电能增量和当日当月电能量计算(测量点)
			break;
		case 5:
			Terminal_EnergyInc_Computer(0x2,ADDR_PKVARH_INC,4,130,ADDR_AFN0CF42,ADDR_AFN0CF46);//正向无功电能增量和当日当月电能量计算(测量点)
			break;
		case 6:
			Terminal_EnergyInc_Computer(0x4,ADDR_NKWH_INC,5,131,ADDR_AFN0CF43,ADDR_AFN0CF47);//反向有功电能增量和当日当月电能量计算(测量点)
			break;
		case 7:
			Terminal_EnergyInc_Computer(0x8,ADDR_NKVARH_INC,4,132,ADDR_AFN0CF44,ADDR_AFN0CF48);//反向无功电能增量和当日当月电能量计算(测量点)
			break;

		case 8:
			Terminal_AFN0CF17_F18_Computer(0);//终端类1数据F17F18,当前总加功率,入口=0有功=1无功
			break;
		case 9:
			Terminal_AFN0CF17_F18_Computer(1);//终端类1数据F17F18,当前总加功率,入口=0有功=1无功
			break;
		case 10:
			Terminal_TotalEnergy_Computer(0x1,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_AFN0CF19_Remainder,ADDR_AFN0CF19);//当日有功总加电能量计算
			break;
		case 11:
			Terminal_TotalEnergy_Computer(0x2,ADDR_PKVARH_INC,ADDR_NKVARH_INC,ADDR_AFN0CF20_Remainder,ADDR_AFN0CF20);//当日无功总加电能量计算
			break;
		case 12:
			Terminal_TotalEnergy_Computer(0x4,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_AFN0CF21_Remainder,ADDR_AFN0CF21);//当月有功总加电能量计算
			break;
		case 13:
			Terminal_TotalEnergy_Computer(0x8,ADDR_PKVARH_INC,ADDR_NKVARH_INC,ADDR_AFN0CF22_Remainder,ADDR_AFN0CF22);//当月无功总加电能量计算
			break;
		case 14:
			ERC22_TotalEnergy_Computer();//ERC22差动事件定时区间总加电量计算
			break;
		case 15:
			Terminal_AFN0CF73_Computer();//直流模拟量计算
			break;
		case 16:
			Terminal_AFN0CF5_Computer();//终端类1数据F5,终端控制设置状态
			break;
		case 17:
			Terminal_AFN0CF6_Computer();//终端类1数据F6,终端当前控制状态
			break;
		case 18:
			Terminal_ResidualEnergy_Computer();//剩余电量(费)计算
			break;
		case 19:
			Terminal_AFN0CF12_Computer();//控制输出回路开关接入状态量及变位标志
			break;
		case 20:
			Terminal_Class1Data_Computer_Ext();//终端类1数据计算扩展任务
			break;
		default:
			Terminal_Ram->I2SUBTask1=0;
			return;
	}
	Terminal_Ram->I2SUBTask1++;
}





