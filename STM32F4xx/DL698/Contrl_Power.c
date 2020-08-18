

//功控
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"




#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

u32 NextTrip(u32 Bin,u32 Pn)//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
{
	u32 i;
	
	i=MRR(ADDR_230x_14+(LENper_230x_14*Pn)+2,1);//功控轮次设定(总加组号)
	i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
	Bin^=i;
	Bin&=i;
	Bin=BitLoca(Bin);//计算第1个置位为1的位置(0-32)
	if(Bin!=0x0)
	{
		if(Bin>NUMmax_CR)
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
	i=MRR(ADDR_230x_13+(LENper_230x_13*Pn)+1,1);//功率控制的功率计算滑差时间(范围：1～60)	BIN	min	1
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

u32 kWContrl_Comp(u64 FixedValue,u32 Pn)//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
{
	u32 f;
	u32 i;
	u64 a64;
	u64 b64;
	u16 * p16timer;
	
	p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
	f=0;//超限标志=0
	i=kWContrlSlipTime(Pn);//取功率控制的功率计算滑差时间秒(范围：1～60分*60=秒),入口：总加组号从0开始
	i=0;//滑差时间不用,脉冲输入功率计算固定为4个15秒(1分)
	p16timer[Pn]=i;//下次计算时间
	//当前功率
	a64=MRR(ADDR_230x_3+(LENper_230x_3*Pn)+1,8);//总加有功功率
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if((a64>>63)!=0x0)
	{//负
		a64=(~a64)+1;
	}
	//保安定值
	b64=MRR(ADDR_8100_2+1,8);//终端保安定值∷=long64（单位：W，换算：-1）
	b64=R_Inverse(b64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if((b64>>63)!=0x0)
	{//负
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//当前功率>=保安定值
		if((FixedValue>>63)!=0x0)
		{//负
			FixedValue=(~FixedValue)+1;
		}
		if(a64>=FixedValue)
		{
			f=1;//超限
		}
	}
	return f;
}

u32 kWContrl1_Comp(u32 Pn)//终端控制-功控.当前功率下浮控功率比较,入口Pn=总加组号(0-7);返回0没超限,1超限
{
	u32 f;
	u32 i;
	u64 a64;
	u64 b64;
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
	i=0;//滑差时间不用,脉冲输入功率计算固定为4个15秒(1分)
	p16timer[Pn]=i;//下次计算时间
	//当前功率
	a64=MRR(ADDR_230x_3+(LENper_230x_3*Pn)+1,8);//总加有功功率
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if((a64>>63)!=0x0)
	{//负
		a64=(~a64)+1;
	}
	//保安定值
	b64=MRR(ADDR_8100_2+1,8);//终端保安定值∷=long64（单位：W，换算：-1）
	b64=R_Inverse(b64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if((b64>>63)!=0x0)
	{//负
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//当前功率>=保安定值
		b64=MRR(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);//取功率定值
		b64=R_Inverse(b64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
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
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
	u64 b64;
	u32 TAG;//参与控制总加组b0-b7表示总加组1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
	u16* p16;
	u16* p16timer;
	
	TAG=0;//参与控制总加组b0-b7表示总加组1-8
#if (LENmax_8106_3+LENmax_8106_127)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8106_3,LENmax_8106_3);//当前功率下浮控-总加组控制投入状态(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8106_3,ADDR_8106_127,LENmax_8106_127);//当前功率下浮控-方法127 投入（总加组对象，控制方案）(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}

//array
//structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//当前功率下浮控定值滑差时间    unsigned（单位：分钟），
//当前功率下浮控定值浮动系数    integer（单位：%），
//控后总加有功功率冻结延时时间  unsigned（单位：分钟），
//当前功率下浮控的控制时间      unsigned（单位：0.5小时），
//当前功率下浮控第1轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第2轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第3轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第4轮告警时间  unsigned（单位：分钟）
//}
	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
		if(p8[1]!=0x0)
		{//保电
			break;
		}
		if(p8c[6]!=1)
		{//没投入
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//总加组对象∷=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//找相应OI的控制方案
		p8s=(u8*)ADDR_DATABUFF+LENmax_8106_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
		}
		if(j>=x)
		{//没找到
			p8c+=7;
			continue;
		}
		//控制时间
		i=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);
		if(i==0x0)
		{//控制时间定时到
			p8c+=7;
			continue;
		}
		//控制
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//参与控制总加组b0-b7表示总加组1-8
		switch(p8d[0])
		{
			case 0://上电或不控制
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 终端总加组1当前功率下浮控控后冻结秒定时器
				i=p8s[12];//控后总加有功功率冻结延时时间,分钟
				i*=60;
				p16[Pn]=i;
				i=p8s[8];//当前功率下浮控定值滑差时间 unsigned（单位：分钟）;376.1中数据范围：1～60
				if(i==0)
				{
					i=1;
				}
				if(i>60)
				{
					i=60;
				}
				i*=60;
				i=60;//滑差时间不用,至少4*15=60秒脉冲完整功率计算周期
				p16timer[Pn]=i;
				MC(0,ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//清控后总加有功功率冻结值
				p8d[0]=1;
				break;
			case 1://定值滑差时间内平均功率计算和定值计算
				if(p16timer[Pn]==0x0)
				{//滑差时间延时到
					a64=MRR(ADDR_230x_3+(Pn*LENper_230x_3)+1,8);//总加有功功率
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					if((a64>>63)!=0x0)
					{//负
						a64=(~a64)+1;
					}
					i=p8s[10];//当前功率下浮控定值浮动系数    integer（单位：%）
					p8d[3]=i;
					
					if((i&0x80)==0)
					{//S0=0，表示上浮
						b64=(a64*i)/100;
						a64+=b64;
					}
					else
					{//S0=1，表示下浮
						i=~i;
						i++;
						i&=0xff;
						b64=(a64*i)/100;
						if(a64>=b64)
						{
							a64-=b64;
						}
						else
						{
							a64=0;
						}
					}
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					MWR(a64,ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
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
					MW(ADDR_230x_3+(Pn*LENper_230x_3),ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//控后总加有功功率冻结
				}
				if(p16timer[Pn]==0x0)
				{//功率控制的功率计算滑差时间(总加组号)滑差时间延时到		
					x=kWContrl1_Comp(Pn);//终端控制-功控.当前功率下浮控功率比较,入口Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
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
							i=p8s[15+(2*i)+1];//第?轮告警时间,分
							i*=60;
							p16timer[Pn]=i;
							p8d[0]=100;
						}
					}
					else
					{//没超限
						
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
					MW(ADDR_230x_3+(Pn*LENper_230x_3),ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//控后总加有功功率冻结
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
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
					MW(ADDR_230x_3+(Pn*LENper_230x_3),ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//控后总加有功功率冻结
				}
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//跳闸后2分钟总加有功功率long64(单位：W，换算-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
					//事件记录
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//bit=跳闸轮次      bit-string(SIZE(8))
	//pFixedValue=功控定值      long64（单位：kW，换算-4）
	//pFirst=跳闸发生前总加有功功率    long64（单位：kW，换算-4）
	//pAfter=事件发生后2分钟功率  long64(单位：W，换算-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					Event_32000200(0x8106,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//功控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					//重读入数据
					MR(ADDR_DATABUFF,ADDR_8106_3,LENmax_8106_3);//当前功率下浮控-总加组控制投入状态(初始化值0x00)
					MR(ADDR_DATABUFF+LENmax_8106_3,ADDR_8106_127,LENmax_8106_127);//当前功率下浮控-方法127 投入（总加组对象，控制方案）(初始化值0x00)
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//清没参与于控制的总加组
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}

}

void Terminal_Contrl2(void)//终端控制-功控.营业报停功控
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
//	u64 b64;
	u32 TAG;//参与控制总加组b0-b7表示总加组1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
	u16* p16timer;
	
	TAG=0;//参与控制总加组b0-b7表示总加组1-8
#if (LENmax_8105_3+LENmax_8105_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8105_3,LENmax_8105_3);//总加组控制投入状态(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8105_3,ADDR_8105_2,LENmax_8105_2);//配置单元(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}

//属性2（控制方案集）∷=array 营业报停控配置单元
//属性2∷=structure
//{
//总加组对象      OI，
//报停起始时间    date_time_s（时=FFH，分=FFH），
//报停结束时间    date_time_s（时=FFH，分=FFH），
//报停控功率定值  long64（单位：W，换算：-1）
//}
	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
		if(p8[1]!=0x0)
		{//保电
			break;
		}
		if(p8c[6]!=1)
		{//没投入
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//总加组对象∷=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//优先级
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		
		//找相应OI的控制方案
		p8s=(u8*)ADDR_DATABUFF+LENmax_8105_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
		}
		if(j>=x)
		{//没找到
			p8c+=7;
			continue;
		}
		//营业报停时间
		i=MRR(ADDR_DL698YMDHMS,4);//当前年月日
		i=R_Inverse(i,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		x=MRR((u32)p8s+6,4);//报停起始时间
		x=R_Inverse(x,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		if(i<x)
		{
			p8c+=7;
			continue;
		}
		x=MRR((u32)p8s+14,4);//报停结束时间
		x=R_Inverse(x,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		if(i>=x)
		{
			p8c+=7;
			continue;
		}
		//控制
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//参与控制总加组b0-b7表示总加组1-8
		switch(p8d[0])
		{
			case 0://上电或不控制
				MR((u32)p8d+8,(u32)p8s+22,8);//当前定值
				p16timer[Pn]=0;//功控功率计算秒定时器=0
				p8d[0]=3;
				break;
			case 3://控制
				if(p16timer[Pn]==0)//功控功率计算秒定时器=0
				{
					MR((u32)p8d+8,(u32)p8s+22,8);//当前定值
					a64=MRR((u32)p8s+22,8);//当前定值
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					x=kWContrl_Comp(a64,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							i--;
							i=MRR(ADDR_8102_2+2+(2*i)+1,1);//功控告警时间
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//第?轮告警时间
							p8d[0]=100;
						}
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
					a64=MRR((u32)p8s+22,8);//当前定值
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					x=kWContrl_Comp(a64,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//跳闸后2分钟总加有功功率long64(单位：W，换算-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
					//事件记录
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//bit=跳闸轮次      bit-string(SIZE(8))
	//pFixedValue=功控定值      long64（单位：kW，换算-4）
	//pFirst=跳闸发生前总加有功功率    long64（单位：kW，换算-4）
	//pAfter=事件发生后2分钟功率  long64(单位：W，换算-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					Event_32000200(0x8105,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//功控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					//重读入数据
					MR(ADDR_DATABUFF,ADDR_8105_3,LENmax_8105_3);//总加组控制投入状态(初始化值0x00)
					MR(ADDR_DATABUFF+LENmax_8105_3,ADDR_8105_2,LENmax_8105_2);//配置单元(初始化值0x00)
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//清没参与于控制的总加组
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}
}

void Terminal_Contrl3(void)//终端控制-功控.厂休功控
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
//	u64 b64;
	u32 TAG;//参与控制总加组b0-b7表示总加组1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
	u16* p16timer;
	
	TAG=0;//参与控制总加组b0-b7表示总加组1-8
#if (LENmax_8104_3+LENmax_8104_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8104_3,LENmax_8104_3);//总加组控制投入状态(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8104_3,ADDR_8104_2,LENmax_8104_2);//配置单元(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}

//属性2（控制方案集）∷=array 厂休控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//总加组对象    OI，
//厂休控定值    long64（单位：W，换算：-1），
//限电起始时间  date_time_s（年=FFFFH，月=FFH，日=FFH），
//限电延续时间  long-unsigned（单位：分钟），
//每周限电日    bit-string(SIZE(8)) 
//}
//每周限电日：D1～D7表示星期一～星期日，D0=0。
	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
		if(p8[1]!=0x0)
		{//保电
			break;
		}
		if(p8c[6]!=1)
		{//没投入
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//总加组对象∷=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//优先级
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		
		//找相应OI的控制方案
		p8s=(u8*)ADDR_DATABUFF+LENmax_8104_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
		}
		if(j>=x)
		{//没找到
			p8c+=7;
			continue;
		}
		//每周限电日    bit-string(SIZE(8)) 
		i=p8s[27];//每周限电日：D1～D7表示星期一～星期日，D0=0。
		i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
		x=Comm_Ram->MYMDHMS[6];//取当前RTC星期
		if(x==0x0)
		{
			x=7;//星期日=7
		}
		x=1<<x;
		i&=x;
		if(i==0x0)
		{//当前周不是厂休
			p8c+=7;
			continue;
		}
		//限电起始时间
		i=(Comm_Ram->DL698YMDHMS[4]<<8)|Comm_Ram->DL698YMDHMS[5];//当前RTC时分
		x=(p8s[19]<<8)|p8s[20];//限电起始时间时分
		if(i<x)
		{//当前RTC分时<限电起始时间分时
			p8c+=7;
			continue;
		}
		//限电结束时间时分计算
		j=(p8s[23]<<8)|p8s[24];
		j+=x&0xff;
		x>>=8;
		x+=j/60;
		x<<=8;
		x|=j%60;
		if(i>=x)
		{//当前RTC分时>=限电结束时间分时
			p8c+=7;
			continue;
		}
		//控制
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//参与控制总加组b0-b7表示总加组1-8
		switch(p8d[0])
		{
			case 0://上电或不控制
				MR((u32)p8d+8,(u32)p8s+6,8);//当前定值
				p16timer[Pn]=0;//功控功率计算秒定时器=0
				p8d[0]=3;
				break;
			case 3://控制
				if(p16timer[Pn]==0)//功控功率计算秒定时器=0
				{
					MR((u32)p8d+8,(u32)p8s+6,8);//当前定值
					a64=MRR((u32)p8s+6,8);//当前定值
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					x=kWContrl_Comp(a64,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							i--;
							i=MRR(ADDR_8102_2+2+(2*i)+1,1);//功控告警时间
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//第?轮告警时间
							p8d[0]=100;
						}
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
					a64=MRR((u32)p8s+6,8);//当前定值
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					x=kWContrl_Comp(a64,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//跳闸后2分钟总加有功功率long64(单位：W，换算-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
					//事件记录
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//bit=跳闸轮次      bit-string(SIZE(8))
	//pFixedValue=功控定值      long64（单位：kW，换算-4）
	//pFirst=跳闸发生前总加有功功率    long64（单位：kW，换算-4）
	//pAfter=事件发生后2分钟功率  long64(单位：W，换算-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					Event_32000200(0x8104,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//功控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					//重读入数据
					MR(ADDR_DATABUFF,ADDR_8104_3,LENmax_8104_3);//总加组控制投入状态(初始化值0x00)
					MR(ADDR_DATABUFF+LENmax_8104_3,ADDR_8104_2,LENmax_8104_2);//配置单元(初始化值0x00)
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//清没参与于控制的总加组
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}
}

void Terminal_Contrl4(void)//终端控制-功控.时段功控
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
	u64 b64;
	u32 TAG;//参与控制总加组b0-b7表示总加组1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8p;
	u8* p8d;
//	u16* p16;
	u16* p16timer;
	
	TAG=0;//参与控制总加组b0-b7表示总加组1-8
#if (LENmax_8103_3+LENmax_8103_2+LENmax_8103_127)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8103_3,LENmax_8103_3);//总加组控制投入状态(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8103_3,ADDR_8103_2,LENmax_8103_2);//配置单元(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8103_3+LENmax_8103_2,ADDR_8103_127,LENmax_8103_127);
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}

//属性2（控制方案集）∷=array 时段功控配置单元;//删除时数组数=0
//属性2∷=structure//
//{
//总加组对象  OI，
//方案标识    bit-string(SIZE(8))，
//第一套定值  PowerCtrlParam，
//第二套定值  PowerCtrlParam，
//第三套定值  PowerCtrlParam，
//时段功控定值浮动系数  integer（单位：%）
//}
//PowerCtrlParam∷=structure
//{
//	时段号          bit-string(SIZE(8))，
//	时段1功控定值  long64（单位：W，换算：-1），
//	时段2功控定值  long64（单位：W，换算：-1），
//	时段3功控定值  long64（单位：W，换算：-1），
//	时段4功控定值  long64（单位：W，换算：-1），
//	时段5功控定值  long64（单位：W，换算：-1），
//	时段6功控定值  long64（单位：W，换算：-1），
//	时段7功控定值  long64（单位：W，换算：-1），
//	时段8功控定值  long64（单位：W，换算：-1）
//}

//时段功控方案切换∷=structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//时段功控投入标识    bit-string(SIZE(8))，
//时段功控定值方案号  unsigned
//}
//时段功控投入标识：D0～D7按顺序对位表示第1～第8时段，置“1”：有效，置“0”：无效。
//时段功控定值方案号：数值范围：0～2依次表示第1～第3套方案，其他值无效。

	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
		if(p8[1]!=0x0)
		{//保电
			break;
		}
		if(p8c[6]!=1)
		{//没投入
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//总加组对象∷=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//优先级
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		
		//找相应OI的控制方案
		p8s=(u8*)ADDR_DATABUFF+LENmax_8103_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
		}
		if(j>=x)
		{//没找到
			p8c+=7;
			continue;
		}
		//找相应OI的方案切换
		p8p=(u8*)ADDR_DATABUFF+LENmax_8103_3+LENmax_8103_2;
		x=p8p[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8p+=2;
		for(j=0;j<x;j++)
		{
			i=(p8p[3]<<8)+p8p[4];//总加组对象∷=OI
			if(i==OI)
			{
				break;
			}
			p8p+=Get_DL698DataLen_S(p8p,0);//得到DL698数据类型长度
		}
		if(j>=x)
		{//没找到
			p8c+=7;
			continue;
		}
		//
		p8=(u8 *)(ADDR_Terminal_Period);//终端费率时段和功控时段
		if(p8[3]==0x0)
		{//0=当前功控时段号不控制,1或2当前功控时段号控制
			p8c+=7;
			continue;
		}
		x=p8[2];//当前功控时段号1-8,=0表示时段号>9错误
		if(x==0x0)
		{
			p8c+=7;
			continue;
		}
		x--;
		i=p8p[9];
//	#if (USER/100)==15//安徽
		i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
//	#endif	
		i>>=x;
		i&=0x1;
		if(i==0x0)
		{//当前时段没投入
			p8c+=7;
			continue;
		}
		i=p8p[11];//时段功控定值方案号
		if(i>2)
		{//0～2依次表示第1～第3套方案，其他值无效
			p8c+=7;
			continue;
		}
		j=p8s[7];//方案标识    bit-string(SIZE(8))，
		j=Bit_Inverse(j,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
		if((j&(1<<i))==0)
		{//无方案标识
			p8c+=7;
			continue;
		}
		p8=Get_Element(p8s,3+i,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8==0)
		{
			p8c+=7;
			continue;
		}
		j=p8[4];
		j=Bit_Inverse(j,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
		if((j&(1<<x))==0)
		{//无时段功率定值
			p8c+=7;
			continue;
		}
		p8+=2+3+(9*x);
		//取当前时段功率定值
		a64=MRR((u32)p8+1,8);//取当前时段功率定值
		a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		if((a64>>63)!=0x0)
		{//负
			a64=(~a64)+1;
		}
		//终端时段功控定值浮动系数
		i=p8s[1];//结构元素个数
		p8=Get_Element(p8s,i,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8==0)
		{//没找到
			p8c+=7;
			continue;
		}
		
		i=p8[1];//时段功控定值浮动系数  integer（单位：%）
		if((i>>7)==0x0)
		{//S0=0，表示上浮
			b64=(a64*i)/100;
			a64+=b64;
		}
		else
		{//S0=1，表示下浮
			i=~i;
			i++;
			i&=0xff;
			b64=(a64*i)/100;
			if(a64>=b64)
			{
				a64-=b64;
			}
			else
			{
				a64=0;
			}
		}
		a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		
		//控制
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len));
		MWR(a64,(u32)p8d+8,8);//存当前定值
		TAG|=1<<Pn;//参与控制总加组b0-b7表示总加组1-8
		switch(p8d[0])
		{
			case 0://上电或不控制
				p16timer[Pn]=0;//功控功率计算秒定时器=0
				p8d[0]=3;
				break;
			case 3://控制
				if(p16timer[Pn]==0)//功控功率计算秒定时器=0
				{
					a64=MRR((u32)p8d+8,8);//取当前定值
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					x=kWContrl_Comp(a64,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
					if(x!=0x0)
					{//超限
						//下跳闸轮次计算
						i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
						if(i==0x0)
						{//轮跳完,不再告警
						}
						else
						{
							i=BitLoca(i);//计算第1个置位为1的位置(0-32)
							i--;
							i=MRR(ADDR_8102_2+2+(2*i)+1,1);//功控告警时间
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//第?轮告警时间
							p8d[0]=100;
						}
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
					a64=MRR((u32)p8d+8,8);//取当前定值
					a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					x=kWContrl_Comp(a64,Pn);//终端控制-功控功率比较,入口FixedValue=定值,Pn=总加组号(0-7);返回0没超限,1超限
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
				//下跳闸轮次计算
				i=NextTrip(p8d[1],Pn);//下跳闸轮次计算,入口Bin=已跳位标志,Pn=总加组号(0-7),返回0=无下轮次,!=0下轮次的位标志
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//当前功控跳闸轮次B0-B7
					//2分钟定时
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				}
				else
				{//轮次完
					p8d[0]=3;
				}
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//跳闸后2分钟总加有功功率long64(单位：W，换算-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//总加组Pn
					//事件记录
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//bit=跳闸轮次      bit-string(SIZE(8))
	//pFixedValue=功控定值      long64（单位：kW，换算-4）
	//pFirst=跳闸发生前总加有功功率    long64（单位：kW，换算-4）
	//pAfter=事件发生后2分钟功率  long64(单位：W，换算-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					Event_32000200(0x8103,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//功控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					//重读入数据
					MR(ADDR_DATABUFF,ADDR_8103_3,LENmax_8103_3);//总加组控制投入状态(初始化值0x00)
					MR(ADDR_DATABUFF+LENmax_8103_3,ADDR_8103_2,LENmax_8103_2);//配置单元(初始化值0x00)
					MR(ADDR_DATABUFF+LENmax_8103_3+LENmax_8103_2,ADDR_8103_127,LENmax_8103_127);
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//清没参与于控制的总加组
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}

}

#endif

