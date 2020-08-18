
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Meter698/Meter07_Energy_ATT7022E.h"
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_DataDef.h"




#if IC_ATT7022==1//0=没,1=有

void GroupActiveEnergy_Total(void)//合相组合有功电能
{
	u32 i;
	u32 word;
	u64 x;
	u64 y;

	//取正反有功电能并转为hex
	MR(ADDR_DATABUFF,ADDR_00010000,5*(1+MMaxTariff));
	MR(ADDR_DATABUFF+(5*(1+MMaxTariff)),ADDR_00020000,5*(1+MMaxTariff));
	for(i=0;i<(2*(1+MMaxTariff));i++)
	{
		x=MRR(ADDR_DATABUFF+(5*i),5);
		x=bcd16_hex(x);
		MWR(x,ADDR_DATABUFF+(5*i),5);
	}
	//清组合电量存放区
	MC(0,ADDR_DATABUFF+(2*(5*(1+MMaxTariff))),8*(1+MMaxTariff));
	//取组合模式字
#if MainProtocol==698
	word=MRR(ADDR_4112+2,1);
	word=Bit_Inverse(word,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
#else
	word=MRR(ADDR_04000601,1);
#endif
	//组合
	if((word&0x1)!=0x0)
	{//正向加
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y+=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	if((word&0x2)!=0x0)
	{//正向减
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y-=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	if((word&0x4)!=0x0)
	{//反向加
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y+=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	if((word&0x8)!=0x0)
	{//反向减
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y-=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	for(i=0;i<(1+MMaxTariff);i++)
	{
		y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		if((y&0x8000000000000000)!=0x0)
		{
			y=(~y)+1;
			x=0x8000000000;
		}
		else
		{
			x=0;
		}
		y%=8000000000;
		y=hex_bcd16(y);
		y|=x;
		MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(5*i),5);
	}
	MW(ADDR_DATABUFF+(2*(5*(1+MMaxTariff))),ADDR_00000000,5*(1+MMaxTariff));
}

void GroupReactiveEnergy_Total(void)//合相组合1和2无功电能
{
	u32 i;
	u32 word;
	u32 group;
	u64 x;
	u64 y;

	MR(ADDR_DATABUFF,ADDR_00050000,5*(MMaxTariff+1));
	MR(ADDR_DATABUFF+(5*(MMaxTariff+1)),ADDR_00060000,5*(MMaxTariff+1));
	MR(ADDR_DATABUFF+(10*(MMaxTariff+1)),ADDR_00070000,5*(MMaxTariff+1));
	MR(ADDR_DATABUFF+(15*(MMaxTariff+1)),ADDR_00080000,5*(MMaxTariff+1));
	for(i=0;i<(4*(MMaxTariff+1));i++)
	{
		x=MRR(ADDR_DATABUFF+(5*i),5);
		x=bcd16_hex(x);
		MWR(x,ADDR_DATABUFF+(5*i),5);
	}
	for(group=1;group<3;group++)
	{
#if MainProtocol==698
		if(group==1)
		{
			word=MRR(ADDR_4113+2,1);
		}
		else
		{
			word=MRR(ADDR_4114+2,1);
		}
		word=Bit_Inverse(word,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
#else
		if(group==1)
		{
			word=MRR(ADDR_04000601+1,1);
		}
		else
		{
			word=MRR(ADDR_04000601+2,1);
		}
#endif
		MC(0,ADDR_DATABUFF+(20*(MMaxTariff+1)),8*(MMaxTariff+1));//清组合电量存放区
		if((word&0x1)!=0x0)
		{//1象限加
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x2)!=0x0)
		{//1象限减
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x4)!=0x0)
		{//2象限加
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x8)!=0x0)
		{//2象限减
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x10)!=0x0)
		{//3象限加
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(10*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x20)!=0x0)
		{//3象限减
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(10*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x40)!=0x0)
		{//4象限加
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(15*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x80)!=0x0)
		{//4象限减
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(15*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		for(i=0;i<(1+MMaxTariff);i++)
		{
			y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			if((y&0x8000000000000000)!=0x0)
			{
				y=(~y)+1;
				x=0x8000000000;
			}
			else
			{
				x=0;
			}
			y%=8000000000;
			y=hex_bcd16(y);
			y|=x;
			MWR(y,ADDR_DATABUFF+(20*(1+MMaxTariff))+(5*i),5);
		}
		if(group==1)
		{
			MW(ADDR_DATABUFF+(20*(1+MMaxTariff)),ADDR_00030000,5*(1+MMaxTariff));
		}	
		else
		{
			MW(ADDR_DATABUFF+(20*(1+MMaxTariff)),ADDR_00040000,5*(1+MMaxTariff));
		}
	}
}


void GroupReactiveEnergy_Phase(u32 ADDR_Energy)//分相组合1和2无功电能
{
	u32 i;
	u32 word;
	u64 x;
	u64 y;
	
	//读1-4象限电能
	MR(ADDR_DATABUFF,ADDR_Energy+20,20);
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_DATABUFF+(5*i),5);
		x=bcd16_hex(x);
		MWR(x,ADDR_DATABUFF+(5*i),5);
	}
	//组合1
	y=0;
#if MainProtocol==698
	word=MRR(ADDR_4113+2,1);
	word=Bit_Inverse(word,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
#else
	word=MRR(ADDR_04000601+1,1);
#endif
	x=MRR(ADDR_DATABUFF+0,5);
	if((word&0x1)!=0x0)
	{//1象限加
		y+=x;
	}
	if((word&0x2)!=0x0)
	{//1象限减
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+5,5);
	if((word&0x4)!=0x0)
	{//2象限加
		y+=x;
	}
	if((word&0x8)!=0x0)
	{//2象限减
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+10,5);
	if((word&0x10)!=0x0)
	{//3象限加
		y+=x;
	}
	if((word&0x20)!=0x0)
	{//3象限减
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+15,5);
	if((word&0x40)!=0x0)
	{//4象限加
		y+=x;
	}
	if((word&0x80)!=0x0)
	{//4象限减
		y-=x;
	}
	if((y&0x8000000000000000)!=0x0)
	{
		y=(~y)+1;
		x=0x8000000000;
	}
	else
	{
		x=0;
	}
	y%=8000000000;
	y=hex_bcd16(y);
	y|=x;
	MWR(y,ADDR_Energy+10,5);
	//组合2
	y=0;
#if MainProtocol==698
	word=MRR(ADDR_4114+2,1);
	word=Bit_Inverse(word,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
#else
	word=MRR(ADDR_04000601+2,1);
#endif
	x=MRR(ADDR_DATABUFF+0,5);
	if((word&0x1)!=0x0)
	{//1象限加
		y+=x;
	}
	if((word&0x2)!=0x0)
	{//1象限减
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+5,5);
	if((word&0x4)!=0x0)
	{//2象限加
		y+=x;
	}
	if((word&0x8)!=0x0)
	{//2象限减
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+10,5);
	if((word&0x10)!=0x0)
	{//3象限加
		y+=x;
	}
	if((word&0x20)!=0x0)
	{//3象限减
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+15,5);
	if((word&0x40)!=0x0)
	{//4象限加
		y+=x;
	}
	if((word&0x80)!=0x0)
	{//4象限减
		y-=x;
	}
	if((y&0x8000000000000000)!=0x0)
	{
		y=(~y)+1;
		x=0x8000000000;
	}
	else
	{
		x=0;
	}
	y%=8000000000;
	y=hex_bcd16(y);
	y|=x;
	MWR(y,ADDR_Energy+15,5);
	//CRC
	MR(ADDR_DATABUFF,ADDR_Energy,85+17);
	//i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+85+17);//RAM中数据CRC32计算,Len=4*1024时需86us(cclk=96mhz)
	i=RAM_CRC32(ADDR_DATABUFF,85+17);//RAM中数据CRC32计算
	MWR(i,ADDR_Energy+85+17,4);
}


void Meter07_ActiveEnergy_Total(void)//合相有功电能
{
	u32 i;
	u32 cf;
	u32 Tariff;
	u64 Energy;
	u32 Data;
	u32 Dir;
	
//读
	Data=Security_Read_ATT7022(0x21);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	Dir=Security_Read_ATT7022(0x3D);//功率方向寄存器
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//总是计为正值
	}
#if MeterMaxDemandCalculate==0//最大需量计算：0=脉冲法,1=功率平均法
//需量脉冲
	i=Data;
	i+=Meter_Ram->ActiveImp;
	Meter_Ram->ActiveImp=i%100;
	Meter_Ram->ActiveImpShift[0]+=(i/100);
//1分钟平均有功功率脉冲
	Meter_Ram->MeanImp+=(i/100);
#endif

//正向
/*
	//电量=实时钟
	MW(ADDR_YMDHMS+1,ADDR_00010000,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+5,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+10,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+15,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+20,5);
	return;
*/
	if((Data)&&((Dir&(1<<3))==0))
	{
		//取整块数据
		MR(ADDR_DATABUFF,ADDR_00010000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
		//合相总
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
		//合相费率电能
		Tariff=Meter_Ram->Tariff;
		if((Tariff==0)||(Tariff>MMaxTariff))
		{
			Tariff=MMaxTariff;
		}
		Energy=MRR(ADDR_DATABUFF+(5*Tariff),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+(5*Tariff),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM中数据CRC32计算,Len=4*1024时需86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM中数据CRC32计算
		MWR(i,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)),4);
		//写回
		MW(ADDR_DATABUFF,ADDR_00010000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
	}
//反向
	if((Data)&&((Dir&(1<<3))!=0))
	{
		//取整块数据
		MR(ADDR_DATABUFF,ADDR_00020000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
		//合相总
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
		//合相费率电能
		Tariff=Meter_Ram->Tariff;
		if((Tariff==0)||(Tariff>MMaxTariff))
		{
			Tariff=MMaxTariff;
		}
		Energy=MRR(ADDR_DATABUFF+(5*Tariff),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+(5*Tariff),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM中数据CRC32计算,Len=4*1024时需86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM中数据CRC32计算
		MWR(i,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)),4);
		//写回
		MW(ADDR_DATABUFF,ADDR_00020000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
	}
//组合
	GroupActiveEnergy_Total();//组合合相有功电能
}

void Meter07_ActiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy)//分相有功电能,入口:相电能地址
{
	u32 i;
	u32 cf;
	u64 Energy;
	u32 Data;
	u32 Dir;
	
//读
	Data=Security_Read_ATT7022(REGaddr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	Dir=Security_Read_ATT7022(0x3D);//功率方向寄存器
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//总是计为正值
	}
	//取整块数据
	MR(ADDR_DATABUFF,ADDR_Energy,85+17);
//正向	
	if((Data)&&((Dir&DirBit)==0))
	{
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+85,1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+85,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
	}
//反向
	if((Data)&&((Dir&DirBit)!=0))
	{
		Energy=MRR(ADDR_DATABUFF+5,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+85+1,1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+85+1,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+5,5);
	}
	//CRC
//	i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+85+17);//RAM中数据CRC32计算,Len=4*1024时需86us(cclk=96mhz)
	i=RAM_CRC32(ADDR_DATABUFF,85+17);//RAM中数据CRC32计算
	MWR(i,ADDR_DATABUFF+85+17,4);
	//写回
	MW(ADDR_DATABUFF,ADDR_Energy,85+17+4);
}


void Meter07_ReactiveEnergy_Total(void)//合相无功电能
{
	u32 i;
	u32 cf;
	u32 Tariff;
	u32 ADDR_Energy;
	u64 Energy;
	u32 Data;
	u32 Dir;
	
//读
	Data=Security_Read_ATT7022(0x25);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	Dir=Security_Read_ATT7022(0x3D);//功率方向寄存器
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//总是计为正值
	}
#if MeterMaxDemandCalculate==0//最大需量计算：0=脉冲法,1=功率平均法
//需量脉冲
	i=Data;
	i+=Meter_Ram->ReactiveImp;
	Meter_Ram->ReactiveImp=i%100;
	Meter_Ram->ReactiveImpShift[0]+=(i/100);
#endif
//
	if(Data)
	{
		switch(Meter_Ram->TQ)//总当前象限 1-4分别表示1-4象限,0=掉电或不确定
		{
			case 1:
				ADDR_Energy=ADDR_00050000;
				break;
			case 2:
				ADDR_Energy=ADDR_00060000;
				break;
			case 3:
				ADDR_Energy=ADDR_00070000;
				break;
			case 4:
				ADDR_Energy=ADDR_00080000;
				break;
			default:
				return;
		}
		//取整块数据
		MR(ADDR_DATABUFF,ADDR_Energy,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
		//总电能
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterReactiveConst/100);
		cf%=(MeterReactiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
		//合相费率电能
		Tariff=Meter_Ram->Tariff;
		if((Tariff==0)||(Tariff>MMaxTariff))
		{
			Tariff=MMaxTariff;
		}
		Energy=MRR(ADDR_DATABUFF+(5*Tariff),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+(5*Tariff),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))));//RAM中数据CRC32计算,Len=4*1024时需86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM中数据CRC32计算
		MWR(i,ADDR_DATABUFF+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))),4);
		//写回
		MW(ADDR_DATABUFF,ADDR_Energy,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
	}
	//组合电能
	GroupReactiveEnergy_Total();//合相组合1和2无功电能
}

void Meter07_ReactiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy)//分相无功电能,入口:相电能地址
{
	u32 i;
	u32 cf;
	u32 Quadrant;
	u64 Energy;
	u32 Data;
	u32 Dir;

//读
	Data=Security_Read_ATT7022(REGaddr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	Dir=Security_Read_ATT7022(0x3D);//功率方向寄存器
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//总是计为正值
	}
//
	if(Data)
	{
		switch(ADDR_Energy)
		{
			case ADDR_00150000:
				i=0;
				break;
			case ADDR_00290000:
				i=1;
				break;
			case ADDR_003D0000:
				i=2;
				break;
			default:
				return;
		}
		Quadrant=MRR(((u32)&Meter_Ram->AQ)+i,1);//取当前象限
		if((Quadrant==0)||(Quadrant>4))
		{//象限错
			Quadrant=1;
		}
		//取整块数据
		MR(ADDR_DATABUFF,ADDR_Energy,85+17);
		//电能
		Energy=MRR(ADDR_DATABUFF+20+(5*(Quadrant-1)),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+87+4+(Quadrant-1),1);
		cf+=Data;
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterReactiveConst/100);
		cf%=(MeterReactiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+87+4+(Quadrant-1),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+20+(5*(Quadrant-1)),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+85+17);//RAM中数据CRC32计算,Len=4*1024时需86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,85+17);//RAM中数据CRC32计算
		MWR(i,ADDR_DATABUFF+85+17,4);
		//写回
		MW(ADDR_DATABUFF,ADDR_Energy,85+17+4);
	}
	//组合电能
	GroupReactiveEnergy_Phase(ADDR_Energy);//分相组合1和2无功电能
}



void Meter07_AH(void)//安时计算
{
	u32 i;
	u32 s;
	u16 * p16;
	u32 * p32;
	
	s=Meter_Ram->AH_S_Timer;//安时计算用秒加计数
	Meter_Ram->AH_S_Timer=0;
	if(s!=0x0)
	{
		p16=(u16 *)(ADDR_DATABUFF+12);
		p32=(u32 *)(ADDR_DATABUFF);
		MR(ADDR_DATABUFF,ADDR_AH,18);
//A
		i=MRR(ADDR_IA+1,3);
		i&=0x7fffff;
		i=bcd_hex(i);
		i*=s;
		i+=p16[0];
		p32[0]+=(i/36000);
		p16[0]=(i%36000);
//B
		i=MRR(ADDR_IB+1,3);
		i&=0x7fffff;
		i=bcd_hex(i);
		i*=s;
		i+=p16[1];
		p32[1]+=(i/36000);
		p16[1]=(i%36000);
//C
		i=MRR(ADDR_IC+1,3);
		i&=0x7fffff;
		i=bcd_hex(i);
		i*=s;
		i+=p16[2];
		p32[2]+=(i/36000);
		p16[2]=(i%36000);
		MW(ADDR_DATABUFF,ADDR_AH,18);
	}
}



#endif



