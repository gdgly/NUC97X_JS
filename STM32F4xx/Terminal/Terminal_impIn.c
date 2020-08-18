
//终端电能脉冲输入
#include "Project.h"
#include "Terminal_impIn.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"









void Init_impInData_Command(void)//终端电能脉冲输入数据初始化(命令,全部清除)
{
	MC(0,ADDR_Terminal_impIn_FM_START,(ADDR_Terminal_impIn_FM_END)-(ADDR_Terminal_impIn_FM_START));//清输入脉冲电量和需量
	MC(0,ADDR_Terminal_impIn_ESRAM_START,(ADDR_Terminal_impIn_ESRAM_END)-(ADDR_Terminal_impIn_ESRAM_START));//清输入脉冲在ESARM
}


void Terminal_impIn_Energy(void)//终端脉冲输入-电能
{
#if MaxImpInPort!=0//最大脉冲输入端口数
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 Port;
	u32 imp;
	u64 d64;
	u8 * p8;
	u16 * p16;
	
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		//取中断中计数脉冲
		p16=(u16 *)(ADDR_Terminal_impIn_Count+(2*Port));
		__disable_irq();
		imp=p16[0];
		p16[0]=0;
		__enable_irq();
		
//		//状态量接入标志
//		i=MRR(ADDR_AFN04F12,1);//终端状态量输入参数
//		i>>=Port;
//		i&=1;
//		if(i!=0x0)
//		{//接入,不是脉冲输入口用
//			imp=0;//脉冲计数=0
//		}
		//需量滑移脉冲加
		p16=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));
		p16[0]+=imp;
		
		//电量计算
		Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
		if(Pn!=0x0)
		{//所属测量点号!=0
			if(Pn<=AFN0CF129MaxPn)
			{//测量点有效
				MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*Port),(4+(5*(TMaxTariff+1))));//读原电量值
				if(imp!=0x0)
				{//有输入脉冲
					i=MRR(ADDR_AFN04F11+(5*Port)+3,2);//取电表常数
					if(i!=0x0)
					{//电表常数!=0
						imp*=10000;
						imp+=MRR(ADDR_DATABUFF+256,4);
						MWR(imp%i,ADDR_DATABUFF+256,4);//写寄存器数据到存贮器,Byte<=8
						imp/=i;
						//总电量
						d64=MRR(ADDR_DATABUFF+256+4,5);
						d64+=imp;
						d64%=10000000000;
						MWR(d64,ADDR_DATABUFF+256+4,5);
						//费率电量
						p8=(u8 *)(ADDR_Terminal_Period);
						i=p8[0];
						if((i==0x0)||(i>TMaxTariff))
						{
							i=TMaxTariff;
						}
						d64=MRR(ADDR_DATABUFF+256+4+(5*i),5);
						d64+=imp;
						d64%=10000000000;
						MWR(d64,ADDR_DATABUFF+256+4+(5*i),5);
						MW(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*Port),(4+(5*(TMaxTariff+1))));
					}
				}
				//数据填入类1数据
				MW(ADDR_DATABUFF+256+4,ADDR_DATABUFF+512,5*(TMaxTariff+1));
				p8=(u8 *)(ADDR_DATABUFF+(256+4-1));
				p8[0]=TMaxTariff;//费率数
				switch(MRR(ADDR_AFN04F11+(5*Port)+2,1))//脉冲属性
				{
					case 0://正向有功
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							MWR(d64,ADDR_DATABUFF+256+4+(5*i),5);//写寄存器数据到存贮器,Byte<=8
						}
						Fn=129;
						break;
					case 1://正向无功
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							d64>>=8;//2位小数
							MWR(d64,ADDR_DATABUFF+256+4+(4*i),4);//写寄存器数据到存贮器,Byte<=8
						}
						//同测量点的正向有功费率数设为TMaxTariff,否则F33,F34,F35,F36组合无费率数
						i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
						p8=(u8 *)(i);
						p8[5]=TMaxTariff;
						Fn=130;
						break;
					case 2://反向有功
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							MWR(d64,ADDR_DATABUFF+256+4+(5*i),5);//写寄存器数据到存贮器,Byte<=8
						}
						Fn=131;
						break;
					case 3://反向无功	
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							d64>>=8;//2位小数
							MWR(d64,ADDR_DATABUFF+256+4+(4*i),4);//写寄存器数据到存贮器,Byte<=8
						}
						//同测量点的反向有功费率数设为TMaxTariff,否则F33,F34,F35,F36组合无费率数
						i=GetClass1DataAddr(131,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
						p8=(u8 *)(i);
						p8[5]=TMaxTariff;
						Fn=132;
						break;
					default:
						continue;
				}
				ReadMeter_SaveFnData(0,Fn,Pn,ADDR_DATABUFF+(256+4-6-1),0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				//当前数据F33,F34组合
				ReadMeter_SaveFnData(0,33,Pn,ADDR_DATABUFF+(256+4-6-1),0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				ReadMeter_SaveFnData(0,34,Pn,ADDR_DATABUFF+(256+4-6-1),0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
			}
		}
	}
#endif//#if MaxImpInPort!=0//最大脉冲输入端口数(等于状态量输入端口数)
}

void Terminal_impIn_Demand(void)//终端脉冲输入-功率和最大需量
{
#if MaxImpInPort!=0//最大脉冲输入端口数
	u32 i;
	u32 x;
	u32 Fn;
	u32 Pn;
	u32 Port;
	u32 imp;
	u64 a64;
	u64 b64;
	u64 ActivePointFlags;//有功功率测量点标志
	u64 ReactivePointFlags;//无功功率测量点标志
	u8 * p8;
	u16 * p16s;

	if(Terminal_Ram->SlipCYC_S_Timer!=0x0)
	{
		return;
	}
	Terminal_Ram->SlipCYC_S_Timer=15;//下次15秒定时
//每15秒功率计算同时填入类1数据
//功率=(1分钟内脉冲数*60)/(电表常数)
	ActivePointFlags=0;//有功功率测量点标志
	ReactivePointFlags=0;//无功功率测量点标志
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
		if(Pn!=0x0)
		{//所属测量点号!=0
			if(Pn<=AFN0CF25MaxPn)
			{//测量点有效
				i=MRR(ADDR_AFN04F11+(5*Port)+3,2);//取电表常数
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
					if(i>=800000)
					{
						i=0;//溢出
					}
					i=hex_bcd(i);
				}
				x=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
				MR(x,ADDR_TYMDHMS+1,5);//终端抄表时间
				a64=1;
				a64<<=(Pn-1);
				switch(MRR(ADDR_AFN04F11+(5*Port)+2,1))//脉冲属性
				{
					case 0://正向有功
						if((ActivePointFlags&a64)==0)
						{
							ActivePointFlags|=a64;//有功功率测量点标志
							MWR(i,x+5,3);//写寄存器数据到存贮器,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								MWR(i,x+5,3);//写寄存器数据到存贮器,Byte<=8
							}
						}
						break;
					case 1://正向无功
						if((ReactivePointFlags&a64)==0)
						{
							ReactivePointFlags|=a64;//无功功率测量点标志
							MWR(i,x+5+12,3);//写寄存器数据到存贮器,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								MWR(i,x+5+12,3);//写寄存器数据到存贮器,Byte<=8
							}
						}
						break;
					case 2://反向有功
						if((ActivePointFlags&a64)==0)
						{
							ActivePointFlags|=a64;//有功功率测量点标志
							i|=0x800000;
							MWR(i,x+5,3);//写寄存器数据到存贮器,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								i|=0x800000;
								MWR(i,x+5,3);//写寄存器数据到存贮器,Byte<=8
							}
						}
						break;
					case 3://反向无功
						if((ReactivePointFlags&a64)==0)
						{
							ReactivePointFlags|=a64;//无功功率测量点标志
							i|=0x800000;
							MWR(i,x+5+12,3);//写寄存器数据到存贮器,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								i|=0x800000;
								MWR(i,x+5+12,3);//写寄存器数据到存贮器,Byte<=8
							}
						}
						break;
					default:
						continue;
				}
			}
		}
	}

//需量=(需量周期内脉冲数*60)/(电表常数*需量周期)
	if(Terminal_Ram->DemandCYC_S_Timer==0x0)//7 终端输入脉冲需量计算秒定时器
	{
		for(Port=0;Port<MaxImpInPort;Port++)
		{
			Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
			if(Pn!=0x0)
			{//所属测量点号!=0
				if(Pn<=AFN0CF145MaxPn)
				{//测量点有效
					MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//读原最大需量
					p8=(u8 *)(ADDR_DATABUFF+256);
					i=p8[6];//原最大需量存贮的月
					p8=(u8 *)(ADDR_TYMDHMS);
					x=p8[4];//当前实时钟月
					if(i!=x)
					{//月更新
						if((p8[1]|p8[2])!=0x0)
						{//实时钟时分!=0x0,约定月冻结完成
							MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//月更新清0
						}
						else
						{
							if((Terminal_Ram->CongealFlags&0x10)==0x10)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
							{//月冻结完成
								MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//月更新清0
							}
						}
					}
					i=MRR(ADDR_AFN04F11+(5*Port)+3,2);
					if(i!=0x0)
					{//电表常数!=0
						p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//终端脉冲输入,需量脉冲滑移(字对齐)
						imp=0;
						for(x=0;x<(4*15);x++)
						{
							imp+=p16s[x];
						}
						a64=imp;
						b64=60*10000;//4位小数
						a64*=b64;//需量周期内脉冲数*60*10000(4位小数)
						i*=15;//电表常数*需量周期
						i=a64/i;
						i=hex_bcd(i);
						if(i>0x799999)
						{
							i=0;//溢出
						}
					}
					else
					{//电表常数=0
						i=0;
					}
					//总最大需量比较
					x=MRR(ADDR_DATABUFF+256,3);//读存贮器数据到寄存器,Byte<=8
					if(i>=x)
					{//比较用>=,当为0时也能写入1个有时间的值,否则月值不同无法填入类1数据
						MWR(i,ADDR_DATABUFF+256,3);//写寄存器数据到存贮器,Byte<=8
						MR(ADDR_DATABUFF+256+3,ADDR_TYMDHMS+1,4);
					}
					//费率最大需量比较
					p8=(u8 *)(ADDR_Terminal_Period);
					x=p8[0];
					if(x!=0x0)
					{
						if(x<=TMaxTariff)
						{
							if(i>=MRR(ADDR_DATABUFF+256+(7*x),3))
							{//比较用>=,当为0时也能写入1个有时间的值,否则月值不同无法填入类1数据
								MWR(i,ADDR_DATABUFF+256+(7*x),3);//读存贮器数据到寄存器,Byte<=8
								MR(ADDR_DATABUFF+256+3+(7*x),ADDR_TYMDHMS+1,4);
							}
						}
					}
					MW(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//写原最大需量
				}
			}
		}
	}
	
//脉冲滑移
	for(Port=0;Port<MaxImpInPort;Port++)
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
	if((Terminal_Ram->CongealFlags&0x1f)!=0x1f)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	{//抄表在冻结后进行
		return;
	}
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
		if(Pn!=0x0)
		{//所属测量点号!=0
			if(Pn<=AFN0CF145MaxPn)
			{//测量点有效
				MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//读原最大需量
				p8=(u8 *)(ADDR_DATABUFF+(256-1));
				p8[0]=TMaxTariff;//费率数
				switch(MRR(ADDR_AFN04F11+(5*Port)+2,1))//脉冲属性
				{
					case 0://正向有功
						Fn=145;
						break;
					case 1://正向无功
						//同测量点的正向有功费率数设为TMaxTariff,否则F33,F34,F35,F36组合费率数=0
						i=GetClass1DataAddr(145,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
						if(i!=0x0)
						{
							p8=(u8 *)(i);
							p8[5]=TMaxTariff;
						}
						Fn=146;
						break;
					case 2://反向有功
						Fn=147;
						break;
					case 3://反向无功	
						//同测量点的反向有功费率数设为TMaxTariff,否则F33,F34,F35,F36组合费率数=0
						i=GetClass1DataAddr(147,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
						if(i!=0x0)
						{
							p8=(u8 *)(i);
							p8[5]=TMaxTariff;
						}
						Fn=148;
						break;
					default:
						continue;
				}
				ReadMeter_SaveFnData(0,Fn,Pn,ADDR_DATABUFF+(256-6-1),0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				//当前数据F35,F36组合
				ReadMeter_SaveFnData(0,35,Pn,ADDR_DATABUFF+(256-6-1),0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				ReadMeter_SaveFnData(0,36,Pn,ADDR_DATABUFF+(256-6-1),0);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
			}
		}
	}
#endif//#if MaxImpInPort!=0//最大脉冲输入端口数(等于状态量输入端口数)
}












