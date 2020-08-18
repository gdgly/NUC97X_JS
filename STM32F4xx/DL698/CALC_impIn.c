

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_impIn.h"
#include "../Device/MEMRW.h"




const u8 Init_EnergyData_double_long_unsigned[]=//初始化数据double_long_unsigned电能
{
	DataType_array,
	TMaxTariff+1,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
#if TMaxTariff>4
	#error
#endif
};

void Init_impInData(void)//清0脉冲输入终端电能和需量数据
{
#if NUMmax_IMPIN!=0
	u32 n;
	u8* p8;
	
//清输入脉冲在ESRAM
	//MC(0,ADDR_Terminal_impIn_ESRAM_START,(ADDR_Terminal_impIn_ESRAM_END)-(ADDR_Terminal_impIn_ESRAM_START));//清输入脉冲在ESRAM
	MC(0,ADDR_Terminal_impIn_Count,(ADDR_Terminal_impIn_ESRAM_END)-(ADDR_Terminal_impIn_Count));//清输入脉冲在ESRAM
//脉冲电能余数
	MC(0,ADDR_impEnergy_Remainder,LEN_impEnergy_Remainder);
//脉冲口电能示值
	for(n=0;n<NUMmax_IMPIN;n++)
	{
		p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
		MW((u32)Init_EnergyData_double_long_unsigned,(u32)p8,sizeof(Init_EnergyData_double_long_unsigned));
	}
//脉冲口当日电能量
	for(n=0;n<NUMmax_IMPIN;n++)
	{
		p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
		MW((u32)Init_EnergyData_double_long_unsigned,(u32)p8,sizeof(Init_EnergyData_double_long_unsigned));
	}
//脉冲口当月电能量
	for(n=0;n<NUMmax_IMPIN;n++)
	{
		p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
		MW((u32)Init_EnergyData_double_long_unsigned,(u32)p8,sizeof(Init_EnergyData_double_long_unsigned));
	}
//脉冲端口当月最大需量和时间
	MC(0,ADDR_impDemand,LENper_impDemand*NUMmax_IMPIN);
#endif
}


void Terminal_impIn_Energy(void)//终端脉冲输入-电能
{
#if NUMmax_IMPIN!=0
	u32 i;
	u32 n;
	u32 Port;
	u32 imp;
	u64 d64;
	u8 * p8;
	u16 * p16;
	
	for(Port=0;Port<NUMmax_IMPIN;Port++)
	{
		i=MRR(ADDR_240x_4+(Port*LENper_240x_4)+5,4);//脉冲输入端口号	OAD
		i= R_Inverse(i,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		i&=0xff;
		if((i==0)||(i>NUMmax_IMPIN))
		{
			continue;
		}
		i--;
		//取中断中计数脉冲
		p16=(u16 *)(ADDR_Terminal_impIn_Count);
		__disable_irq();
		imp=p16[i];
		p16[i]=0;
		__enable_irq();
	
		if(imp!=0x0)
		{//有输入脉冲
			//需量滑移脉冲加
			p16=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));
			p16[0]+=imp;
			
			i=MRR(ADDR_240x_4+(Port*LENper_240x_4)+12,2);//取电表常数
			i=(i>>8)|((i&0xff)<<8);
			if(i!=0x0)
			{//电表常数!=0
				imp*=10000;
				imp+=MRR(ADDR_impEnergy_Remainder+(4*Port),4);
				MWR(imp%i,ADDR_impEnergy_Remainder+(4*Port),4);
				imp/=i;
			//脉冲计量电能示值,当日电能量,当月电能量
				for(n=0;n<3;n++)
				{
					p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy*NUMmax_IMPIN)+(Port*LENper_impEnergy);
					p8[0]=DataType_array;
					p8[1]=1+TMaxTariff;
					p8+=2;
					for(i=0;i<TMaxTariff;i++)
					{
						p8[0]=DataType_double_long_unsigned;
						p8+=5;
					}
					//总电量
					p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy*NUMmax_IMPIN)+(Port*LENper_impEnergy);
					p8+=3;
					d64=MRR((u32)p8,4);
					d64= R_Inverse(d64,4);//寄存器数据倒序(高低字节调换)
					d64+=imp;
					d64%=100000000;
					d64= R_Inverse(d64,4);//寄存器数据倒序(高低字节调换)
					MWR(d64,(u32)p8,4);
					//费率电量
					i=Meter_Ram->Tariff;//1BYTE 电能表当前执行费率(HEX 费率1,2,3,4 分别对应尖峰,峰,平,谷等费率)
					if((i==0x0)||(i>TMaxTariff))
					{
						i=TMaxTariff;
					}
					d64=MRR((u32)p8+(5*i),4);
					d64= R_Inverse(d64,4);//寄存器数据倒序(高低字节调换)
					d64+=imp;
					d64%=100000000;
					d64= R_Inverse(d64,4);//寄存器数据倒序(高低字节调换)
					MWR(d64,(u32)p8+(5*i),4);
				}
			}
		}
	}
#endif
}

void Terminal_impIn_Demand(void)//终端脉冲输入-功率和最大需量
{
#if NUMmax_IMPIN!=0
	u32 i;
	u32 x;
	u32 Port;
	u64 a64;
	u64 b64;
	u8 * p8;
	u16 * p16s;

	if(Terminal_Ram->SlipCYC_S_Timer!=0x0)
	{
		return;
	}
	Terminal_Ram->SlipCYC_S_Timer=15;//下次15秒定时
//每15秒功率计算
//功率=(1分钟内脉冲数*60)/(电表常数)
	for(Port=0;Port<NUMmax_IMPIN;Port++)
	{
		i=MRR(ADDR_240x_4+(Port*LENper_240x_4)+12,2);//取电表常数
		i=(i>>8)|((i&0xff)<<8);
		if(i!=0x0)
		{//电表常数!=0
			p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//终端脉冲输入,需量脉冲滑移(字对齐)
			a64=p16s[0];//1分钟为4个15秒
			a64+=p16s[1];
			a64+=p16s[2];
			a64+=p16s[3];
			b64=60*10000;//4位小数
			a64*=b64;
			i=a64/i;
			if(i>=80000000)
			{
				i=0;//溢出
			}
			p8=(u8*)ADDR_240x_4+(Port*LENper_240x_4)+10;
			x=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
			if(x>=2)
			{
				i=~i;
				i++;
			}
			p8=(u8*)ADDR_240x_5+(Port*LENper_240x_5);//脉冲口功率
			p8[0]=DataType_double_long;
			p8[1]=i>>24;
			p8[2]=i>>16;
			p8[3]=i>>8;
			p8[4]=i;
		}
	}

//需量=(需量周期内脉冲数*60)/(电表常数*需量周期)
//	if(Terminal_Ram->DemandCYC_S_Timer==0x0)//7 终端输入脉冲需量计算秒定时器
//	{
//		for(Port=0;Port<NUMmax_IMPIN;Port++)
//		{
//			MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//读原最大需量
//			p8=(u8 *)(ADDR_DATABUFF+256);
//			i=p8[6];//原最大需量存贮的月
//			p8=(u8 *)(ADDR_TYMDHMS);
//			x=p8[4];//当前实时钟月
//			if(i!=x)
//			{//月更新
//				if((p8[1]|p8[2])!=0x0)
//				{//实时钟时分!=0x0,约定月冻结完成
//					MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//月更新清0
//				}
//				else
//				{
//					if((Terminal_Ram->CongealFlags&0x10)==0x10)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
//					{//月冻结完成
//						MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//月更新清0
//					}
//				}
//			}
//			i=MRR(ADDR_AFN04F11+(5*Port)+3,2);
//			if(i!=0x0)
//			{//电表常数!=0
//				p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//终端脉冲输入,需量脉冲滑移(字对齐)
//				imp=0;
//				for(x=0;x<(4*15);x++)
//				{
//					imp+=p16s[x];
//				}
//				a64=imp;
//				b64=60*10000;//4位小数
//				a64*=b64;//需量周期内脉冲数*60*10000(4位小数)
//				i*=15;//电表常数*需量周期
//				i=a64/i;
//				i=hex_bcd(i);
//				if(i>0x799999)
//				{
//					i=0;//溢出
//				}
//			}
//			else
//			{//电表常数=0
//				i=0;
//			}
//			//总最大需量比较
//			x=MRR(ADDR_DATABUFF+256,3);//读存贮器数据到寄存器,Byte<=8
//			if(i>=x)
//			{//比较用>=,当为0时也能写入1个有时间的值,否则月值不同无法填入类1数据
//				MWR(i,ADDR_DATABUFF+256,3);//写寄存器数据到存贮器,Byte<=8
//				MR(ADDR_DATABUFF+256+3,ADDR_TYMDHMS+1,4);
//			}
//			//费率最大需量比较
//			p8=(u8 *)(ADDR_Terminal_Period);
//			x=p8[0];
//			if(x!=0x0)
//			{
//				if(x<=TMaxTariff)
//				{
//					if(i>=MRR(ADDR_DATABUFF+256+(7*x),3))
//					{//比较用>=,当为0时也能写入1个有时间的值,否则月值不同无法填入类1数据
//						MWR(i,ADDR_DATABUFF+256+(7*x),3);//读存贮器数据到寄存器,Byte<=8
//						MR(ADDR_DATABUFF+256+3+(7*x),ADDR_TYMDHMS+1,4);
//					}
//				}
//			}
//			MW(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//写原最大需量
//		}
//	}

//脉冲滑移
	for(Port=0;Port<NUMmax_IMPIN;Port++)
	{
		p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//终端脉冲输入,需量脉冲滑移(字对齐)
		for(i=((4*15)-2);i!=0x0;i--)
		{
			p16s[i+1]=p16s[i];
		}
		p16s[1]=p16s[0];
		p16s[0]=0;
	}


//每1分钟最大需量填入类1数据

#endif
}

























