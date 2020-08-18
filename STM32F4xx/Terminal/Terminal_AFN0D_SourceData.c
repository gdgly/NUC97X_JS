
#include "Project.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Calculate/Calculate.h"
#include "KeyUser.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../MS/MS.h"





extern Terminal_Class2SourceDataLib_TypeDef Terminal_Class2SourceDataLib[];
u32 Get_Class2SourceData_MaxLib(void);//得到类2原始最大库列表个数
extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);





void Init_Class2SourceData_Command(void)//类2原始数据初始化(命令,全部清除为0xff)
{
	u32 i;
	u32 Addr;
	//上1日原始
	//上2-3日原始
	Addr=ADDR_AFN0D_Source_Date_Start;
	i=ADDR_LastDate_Source_End-ADDR_AFN0D_Source_Date_Start;
	MC(0xff,Addr,i);
//	while(i>=0x1000)
//	{
//		if(((Addr&0xffff)==0)&&(i>=0x10000))
//		{
//			EFLASH_Block_Erase(Addr);//块擦除,入口R0=要擦除的块在本块中的任意地址(uses AMS-A15 address lines,每块32KWORD)
//			Addr+=0x10000;
//			i-=0x10000;
//		}
//		else
//		{
//			EFLASH_Sector_Erase(Addr);//扇区擦除,入口R0=要擦除的扇区在本扇区中的任意地址(每扇区2KWORD)
//			Addr+=0x1000;
//			i-=0x1000;
//		}
//	}
}


u32 ValidRMDate(u32 CongealType,u32 ValidDate,u32 RMDate)//按冻结类型比较抄表日期,入口:ValidDate=有效年月日(值0表示不比较);返回:0=相同有效,1=不相同无效
{
	if(ValidDate==0)
	{
		return 0;
	}
	switch(CongealType)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
	{
		case 0://0=日冻结
			if(ValidDate!=RMDate)
			{//不是有效抄表时间
				return 1;
			}
			return 0;
		case 1://1=抄表日冻结
			return 0;
		case 2://2=月冻结
			if((ValidDate>>8)!=(RMDate>>8))
			{//不是有效抄表时间
				return 1;
			}
			return 0;
		case 3://3=曲线
			return 0;
	}
	return 0;
}

/*
void AFN0DEnergyGroup(u32 CongealType,u32 YMD,u32 Fn,u32 Pn,u32 Addr_CongealBuff,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4,u32 Full0xee)//2类数据电能示值组合
{
	u32 i;
	u32 DestAddr;
	u32 SourAddr;
	u64 DestYMDHM;
	u64 SourYMDHM;
	u8 *p8;
	u32 Fullf;
	u8 *pn8;
	
	Fullf=0;//b0-b3分别为Fn1~Fn4有数据填入标志
	pn8=(u8*)(ADDR_PnVSSetList);
	DestAddr=Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	p8=(u8 *)DestAddr;
//有功电能示值
	SourAddr=GetClass2SourceDataAddr(Fn1,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//有原始数据库
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//原始数据有效(清0时为0xff)
			i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
			if(i==0)
			{//相同有效
				Fullf|=1;//b0-b3分别为Fn1~Fn4有数据填入标志
				//费率数和示值
				MR(DestAddr+5,SourAddr+5,1+(5*(MaxTariff+1)));
				//抄表时间
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//原无抄表时间
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//当前抄表时间>原抄表时间
					}
				}
			}
		}
	}
	if((Fullf&1)==0)//b0-b3分别为Fn1~Fn4有数据填入标志
	{//没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr+6,5*(MaxTariff+1));
			}
		}
	}
	//费率数
	if(p8[5]>MaxTariff)
	{
		p8[5]=MaxTariff;
	}
//无功电能示值
	SourAddr=GetClass2SourceDataAddr(Fn2,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//有原始数据库
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//原始数据有效(清0时为0xff)
			i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
			if(i==0)
			{//相同有效
				Fullf|=2;//b0-b3分别为Fn1~Fn4有数据填入标志
				//示值
				MR(DestAddr+6+(5*(p8[5]+1)),SourAddr+6,(4*(MaxTariff+1)));
				//抄表时间
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//原无抄表时间
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//当前抄表时间>原抄表时间
					}
				}
			}
		}
	}
	if((Fullf&2)==0)//b0-b3分别为Fn1~Fn4有数据填入标志
	{//没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr+6+(5*(p8[5]+1)),(4*(MaxTariff+1)));
			}
		}
	}
//象限无功电能示值
	SourAddr=GetClass2SourceDataAddr(Fn3,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//有原始数据库
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//原始数据有效(清0时为0xff)
			i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
			if(i==0)
			{//相同有效
				Fullf|=4;//b0-b3分别为Fn1~Fn4有数据填入标志
				//示值
				MR(DestAddr+6+(9*(p8[5]+1)),SourAddr+6,(4*(MaxTariff+1)));
				//抄表时间
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//原无抄表时间
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//当前抄表时间>原抄表时间
					}
				}
			}
		}
	}
	if((Fullf&4)==0)//b0-b3分别为Fn1~Fn4有数据填入标志
	{//没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr+6+(9*(p8[5]+1)),(4*(MaxTariff+1)));
			}
		}
	}
//象限无功电能示值
	SourAddr=GetClass2SourceDataAddr(Fn4,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//有原始数据库
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//原始数据有效(清0时为0xff)
			i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
			if(i==0)
			{//相同有效
				Fullf|=8;//b0-b3分别为Fn1~Fn4有数据填入标志
				//示值
				MR(DestAddr+6+(13*(p8[5]+1)),SourAddr+6,(4*(MaxTariff+1)));
				//抄表时间
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//原无抄表时间
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//当前抄表时间>原抄表时间
					}
				}
			}
		}
	}
	if((Fullf&8)==0)//b0-b3分别为Fn1~Fn4有数据填入标志
	{//没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr+6+(13*(p8[5]+1)),(4*(MaxTariff+1)));
			}
		}
	}
//抄表时间
	if(Fullf==0)//b0-b3分别为Fn1~Fn4有数据填入标志
	{//全没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr,6);
			}
		}
	}
}
void AFN0DDemandGroup(u32 CongealType,u32 YMD,u32 Fn,u32 Pn,u32 Addr_CongealBuff,u32 Fn1,u32 Fn2,u32 Full0xee)//2类数据需量值组合
{
	u32 i;
	u32 x;
	u32 Tariff;
	u32 DestAddr;
	u32 SourAddr;
	u64 DestYMDHM;
	u64 SourYMDHM;
	u8 * p8s;
	u8 * p8d;
	u32 Fullf;
	u8 *pn8;
	
	Fullf=0;//b0-b1分别为Fn1~Fn2有数据填入标志
	pn8=(u8*)(ADDR_PnVSSetList);
	DestAddr=Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	p8d=(u8 *)DestAddr;
//有功需量
	SourAddr=GetClass2SourceDataAddr(Fn1,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//有原始数据库
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//原始数据有效(清0时为0xff)
			i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
			if(i==0)
			{//相同有效
				Fullf|=1;//b0-b1分别为Fn1~Fn2有数据填入标志
				p8s=(u8 *)SourAddr;
				//费率数
				Tariff=p8s[5];
				if(Tariff>MaxTariff)
				{
					Tariff=MaxTariff;
				}
				p8d[5]=Tariff;
				//需量值
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(3*i)+x]=p8s[6+(7*i)+x];
					}
				}
				//发生时间
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(3*(Tariff+1))+(4*i)+x]=p8s[6+(7*i)+3+x];
					}
				}
				//抄表时间
				if(p8d[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//原无抄表时间
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//当前抄表时间>原抄表时间
					}
				}
			}
		}
	}
	if((Fullf&1)==0)//b0-b1分别为Fn1~Fn2有数据填入标志
	{//没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr+6,7*(MaxTariff+1));
			}
		}
	}
	//费率数
	Tariff=p8d[5];
	if(Tariff>MaxTariff)
	{
		Tariff=MaxTariff;
	}
	p8d[5]=Tariff;
//无功需量
	SourAddr=GetClass2SourceDataAddr(Fn2,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//有原始数据库
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//原始数据有效(清0时为0xff)
			i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
			if(i==0)
			{//相同有效
				Fullf|=2;//b0-b1分别为Fn1~Fn2有数据填入标志
				p8s=(u8 *)SourAddr;
				//需量值
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(7*(Tariff+1))+(3*i)+x]=p8s[6+(7*i)+x];
					}
				}
				//发生时间
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(7*(Tariff+1))+(3*(Tariff+1))+(4*i)+x]=p8s[6+(7*i)+3+x];
					}
				}
				//抄表时间
				if(p8d[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//原无抄表时间
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//当前抄表时间>原抄表时间
					}
				}
			}
		}
	}
	if((Fullf&2)==0)//b0-b1分别为Fn1~Fn2有数据填入标志
	{//没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr+6+(7*(p8d[5]+1)),7*(MaxTariff+1));
			}
		}
	}
//抄表时间
	if(Fullf==0)//b0-b1分别为Fn1~Fn2有数据填入标志
	{//全没填入
		if(Full0xee)
		{//无原始数据填0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
				MC(0xee,DestAddr,6);
			}
		}
	}
}
*/

void Source_Full_CongealBuff(u32 Addr_CongealBuff,u32 CongealType,u32 ADDR_CongealFlags,u32 YMD,u32 Full0xee)//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 MaxFn;
	u32 MaxPn;
	u32 SourAddr;
	u8 * p8f;
	u8 *pn8;
	ms_Type* ms;
	
//为加速先清nand写buff
	ms=Get_ms();
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
	ms->NAND4kBuff_Write_NAND(0,0,0);
#else
	ms->NAND128kBuff_Write_NAND(0,0,0);
#endif
	p8f=(u8*)ADDR_CongealFlags;//测量点分端口需冻结标志
	MaxFn=Get_Class2Data_MaxFn();
	if(Full0xee)
	{//无原始数据填0xee
		PnVSPORTProtocolList(ADDR_PnVSSetList);//测量点号Pn对应AFN04F10配置端口号和通信协议列表,无配置且非脉冲口默认端口号0通信协议为30(DL645-2007)
	}
	pn8=(u8*)(ADDR_PnVSSetList);
	
	for(Fn=1;Fn<MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==CongealType)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				MaxPn=Terminal_Class2DataLib[Fn].PnMax;
				for(Pn=1;Pn<=MaxPn;Pn++)
				{
					if(CongealType==1)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
					{//抄表日冻结,按端口号进行
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//测量点号类
						{
							i=p8f[(Pn-1)/8];
							i&=1<<((Pn-1)%8);
							if(i==0)
							{//分端口本测量点冻结标志=0
								continue;
							}
						}
					}
					SourAddr=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
					if(SourAddr!=0)
					{//有原始数据库
						i=MRR(SourAddr,1);
						if(i==0xff)
						{//原始数据无效(清0时为0xff)
							if(Full0xee)
							{//无原始数据填0xFF
								if(CongealType==0)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
								{//是日冻结
									if(pn8[(2*Pn)+1]==1)
									{//是DL/T 645-1997
										if((Fn>=185)&&(Fn<=188))
										{//是日冻结97表的最大需量,不清
											continue;
										}
									}
								#if RMM_RS485_DateCongeal==0//RS485通道抄表抄日冻结;0=否,1=是
									if(((pn8[(2*Pn)]==0)||(pn8[(2*Pn)]==31))&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]>=30)))//交采,485口,脉冲口不清(只清载波口)
									{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#else
									#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
									//97规约、上海规约实时电能过日存储为上一日日冻结
									//if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]>=30)))//交采,脉冲口,1997规约,上海规约不清
									//97规约、上海规约过日采集实时电能存储为上一日日冻结
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]==21)||(pn8[(2*Pn)+1]>=30)))//交采,脉冲口不清
									#else//#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]>=30)))//交采,脉冲口不清
									#endif
									{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#endif
								}
								else
								{//是1=抄表日冻结,2=月冻结
								#if RMM_RS485_DateCongeal==0//RS485通道抄表抄日冻结;0=否,1=是
									if(((pn8[(2*Pn)]==0)||(pn8[(2*Pn)]==31))&&(pn8[(2*Pn)+1]>=30))//交采,485口,脉冲口,1997规约不清
									{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#else
									#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
									if(CongealType==2)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
									{
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==1))
										{//是DL/T 645-1997
											if((Fn>=193)&&(Fn<=196))
											{//是月冻结97表的最大需量,清掉
												MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
											}
										}
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==21))
										{//是上海规约
											//上海表采集表内上3月正向有功电量作为月冻结正向有功电量
											MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
										}
									}
									#endif
									if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]>=30))//交采,脉冲口,1997规约不清
									{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#endif
								}
							}
							continue;
						}
				#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
						if((Fn==209)||(Fn==213)||(Fn==214)||(Fn==215)||(Fn==216))
						{//以下Fn不比较抄表时间,总是用最后次抄表数据冻结
							//Fn=209日冻结电能表远程控制通断电状态及记录
							//Fn=213月冻结电能表开关操作次数及时间
							//Fn=214月冻结电能表参数修改次数及时间
							//Fn=215月冻结电能表购、用电信息
							//Fn=216月冻结电能表结算信息
							MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							continue;
						}
				#endif
						i=MRR(SourAddr+2,3);//取抄表时间年月日
						i=ValidRMDate(CongealType,YMD,i);//按冻结类型比较抄表日期,返回:0=相同有效,1=不相同无效
						if(i!=0)
						{//不相同无效
							if(Full0xee)
							{//无原始数据填0xFF
								if(CongealType==0)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
								{//是日冻结
									if(pn8[(2*Pn)+1]==1)
									{//是DL/T 645-1997
										if((Fn>=185)&&(Fn<=188))
										{//是日冻结97表的最大需量,不清
											continue;
										}
									}
								#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]>=30)))//交采,脉冲口,1997规约,上海规约不清
								#else//#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]>=30)))//交采,脉冲口不清
								#endif
									{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								}
								else
								{//是1=抄表日冻结,2=月冻结
								#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
									if(CongealType==2)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
									{
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==1))
										{//是DL/T 645-1997
											if((Fn>=193)&&(Fn<=196))
											{//是月冻结97表的最大需量,清掉
												MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
											}
										}
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==21))
										{//是上海规约
											//上海表采集表内上3月正向有功电量作为月冻结正向有功电量
											MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
										}
									}
								#endif
									if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]>=30))//交采,脉冲口,1997规约不清
									{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议				
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								}
							}
							continue;
						}
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{//无原始数据库
						/*
						switch(Fn)
						{
							case 1://日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
							case 9://抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
								//F161 日冻结正向有功电能示值（总、费率1～M）
								//F162 日冻结正向无功（组合无功1）电能示值（总、费率1～M）
								//F165 日冻结一象限无功电能示值（总、费率1～M）
								//F168 日冻结四象限无功电能示值（总、费率1～M）
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,161,162,165,168,Full0xee);//2类数据电能示值组合
								break;
							case 2://日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
							case 10://抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
								//F163 日冻结反向有功电能示值（总、费率1～M）
								//F164 日冻结反向无功（组合无功1）电能示值（总、费率1～M）
								//F166 日冻结2象限无功电能示值（总、费率1～M）
								//F167 日冻结3象限无功电能示值（总、费率1～M）
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,163,164,166,167,Full0xee);//2类数据电能示值组合
								break;
							case 3://日冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 11://抄表日冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								//F185 日冻结正向有功最大需量及发生时间（总、费率1～M）
								//F186 日冻结正向无功最大需量及发生时间（总、费率1～M）
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,185,186,Full0xee);//2类数据需量值组合
								break;
							case 4://日冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 12://抄表日冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								//F187 日冻结反向有功最大需量及发生时间（总、费率1～M）
								//F188 日冻结反向无功最大需量及发生时间（总、费率1～M）
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,187,188,Full0xee);//2类数据需量值组合
								break;
							case 17://月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
								//F177 月冻结正向有功电能示值（总、费率1～M）
								//F178 月冻结正向无功（组合无功1）电能示值（总、费率1～M）
								//F181 月冻结1象限无功电能示值（总、费率1～M）
								//F184 月冻结4象限无功电能示值（总、费率1～M）
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,177,178,181,184,Full0xee);//2类数据电能示值组合
								break;
							case 18://月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
								//F179 月冻结反向有功电能示值（总、费率1～M）
								//F180 月冻结反向无功（组合无功1）电能示值（总、费率1～M）
								//F182 月冻结2象限无功电能示值（总、费率1～M）
								//F183 月冻结3象限无功电能示值（总、费率1～M）
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,179,180,182,183,Full0xee);//2类数据电能示值组合
								break;
							case 19://月冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								//F193 月冻结正向有功最大需量及发生时间（总、费率1～M）
								//F194 月冻结正向无功最大需量及发生时间（总、费率1～M）
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,193,194,Full0xee);//2类数据需量值组合
								break;
							case 20://月冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								//F195 月冻结反向有功最大需量及发生时间（总、费率1～M）
								//F196 月冻结反向无功最大需量及发生时间（总、费率1～M）
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,195,196,Full0xee);//2类数据需量值组合
								break;
						}
					*/
					}

				}
			}
		}
	}
}


u32 GetLastSourceDataOffice(u32 Fn)//取类2上2-3日原始数据偏移地址,返回0表示没找到
{
	switch(Fn)
	{
		case 161://正向有功电能示值（总、费率1～M）
			return Office_AFN0DF161_LastDate_Source;
		case 162://正向无功（组合无功1）电能示值（总、费率1～M）
			return Office_AFN0DF162_LastDate_Source;
		case 163://反向有功电能示值（总、费率1～M）
			return Office_AFN0DF163_LastDate_Source;
		case 164://反向无功（组合无功2）电能示值（总、费率1～M）
			return Office_AFN0DF164_LastDate_Source;
		case 165://1象限无功电能示值（总、费率1～M）
			return Office_AFN0DF165_LastDate_Source;
		case 166://2象限无功电能示值（总、费率1～M）
			return Office_AFN0DF166_LastDate_Source;
		case 167://3象限无功电能示值（总、费率1～M）
			return Office_AFN0DF167_LastDate_Source;
		case 168://4象限无功电能示值（总、费率1～M）
			return Office_AFN0DF168_LastDate_Source;
		default:
			return 0;
	}
}

u32 LastSource_Full_CongealBuff(u32 Addr_CongealBuff,u32 LastYMD)//上2-3日抄表原始数据填入冻结缓冲;入口:LastYMD=电表冻结时标;返回:0=没补抄填入,1=有
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 MaxPn;
	u32 SourAddr;
	u32 Full;//有填入标志
	ms_Type* ms;
	
//为加速先清nand写buff
	ms=Get_ms();
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
	ms->NAND4kBuff_Write_NAND(0,0,0);
#else
	ms->NAND128kBuff_Write_NAND(0,0,0);
#endif
	
	Full=0;//有填入标志
	for(Fn=161;Fn<168;Fn++)
	{
		MaxPn=Terminal_Class2DataLib[Fn].PnMax;
		for(Pn=1;Pn<=MaxPn;Pn++)
		{
			SourAddr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
			i=MRR(SourAddr,3);//取原始冻结时标
			if(i==LastYMD)
			{//数据时标有效
				SourAddr=GetLastSourceDataOffice(Fn);//取类2上2-3日原始数据偏移地址,返回0表示没找到
				SourAddr+=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
			}
			else
			{
				SourAddr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
				i=MRR(SourAddr,3);//取原始冻结时标
				if(i==LastYMD)
				{//数据时标有效
					SourAddr=GetLastSourceDataOffice(Fn);//取类2上2-3日原始数据偏移地址,返回0表示没找到
					SourAddr+=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
				}
				else
				{
					continue;
				}
			}
			i=MRR(SourAddr,1);
			if(i!=0xee)
			{//数据有效
				Full=1;//有填入标志
				MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),SourAddr,Terminal_Class2DataLib[Fn].PnLen);
			}
		}
	}
	return Full;
}







