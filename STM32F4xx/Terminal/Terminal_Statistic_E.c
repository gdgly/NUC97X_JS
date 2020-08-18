
//终端统计2， 用于外部程序E1
#include "Project.h"
#include "Terminal_Statistic.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0E_Event.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"




u32 ReNewValue(u32 b,u32 c)//恢复值计算,入口b=限值,c=恢复系数
{
	u64 d64a;
	u64 d64b;
	b=bcd_hex(b);
#if (USER/100)==5//上海
	//上海出厂参数F26里的恢复系数设为正，但上海理解的恢复值是1-恢复系数来计算
	c&=0x7fff;
	c=bcd_hex(c);
	d64a=b;
	d64b=c;
	d64a*=d64b;
	c=d64a/1000;
	b-=c;
	if((b>>31)!=0x0)
	{//结果为负
		b=0;
	}
#else
	if((c>>15)==0x0)
	{//系数正
		c&=0x7fff;
		c=bcd_hex(c);
		d64a=b;
		d64b=c;
		d64a*=d64b;
		c=d64a/1000;
		b+=c;
	}
	else
	{//系数负
		c&=0x7fff;
		c=bcd_hex(c);
		d64a=b;
		d64b=c;
		d64a*=d64b;
		c=d64a/1000;
		b-=c;
		if((b>>31)!=0x0)
		{//结果为负
			b=0;
		}
	}
#endif
	b=hex_bcd(b);//恢复值
	return b;
}

void Terminal_Ctrl_Count(u32 ci)//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
{
	u32 i;
	i=MRR(ADDR_AFN0DF50_SOUR+ci,1);
	i++;
	MWR(i,ADDR_AFN0DF50_SOUR+ci,1);
	i=MRR(ADDR_AFN0DF52_SOUR+ci,1);
	i++;
	MWR(i,ADDR_AFN0DF52_SOUR+ci,1);
}

void AFN0DF27F35_Computer(void)//终端统计.电压统计数据
{
	u32 i;
	u32 Pn;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 gfs;//越上上限的ABC相产生标志
	u32 gfx;//越下下限的ABC相产生标志
	u32 unfs;//越上上限的ABC相恢复标志
	u32 unfx;//越下下限的ABC相恢复标志
	u32 t;
	u8 * p8;
	u64 d64;
	
	Pn=Terminal_Ram->I2SUBTask4_3;
	if(Pn>=AFN0DF27MaxPn)
	{
		Pn=0;
	}

	p8=(u8 *)(ADDR_AFN0DF27F35_SOUR+(LEN_AFN0DF27F35_SOUR*Pn));
	t=p8[0];//分更新加计数
	p8[0]=0;

	MR(ADDR_DATABUFF,ADDR_AFN0DF27_SOUR+((66+27)*Pn),66+27);
	MR(ADDR_DATABUFF+(66+27),ADDR_AFN0DF27_PAR+(18*Pn),18);
	MR(ADDR_DATABUFF+(66+27)+18,ADDR_AFN0DF35_SOUR+((66+27)*Pn),66+27);
	MR(ADDR_DATABUFF+(66+27)+18+(66+27),ADDR_AFN0DF35_PAR+(18*Pn),18);
	if(Pn>=AFN04F26MaxPn)
	{//Pn>测量点号(测量点限值参数)
		Pn++;
		Terminal_Ram->I2SUBTask4_3=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
	x+=37;
	y=ADDR_AFN04F26+(57*Pn);
	gfs=0;//越上上限的ABC相产生标志
	gfx=0;//越下下限的ABC相产生标志
	unfs=0;//越上上限的ABC相恢复标志
	unfx=0;//越下下限的ABC相恢复标志
	for(i=0;i<3;i++)
	{
		#if (((Project/100)==3)&&(MeterPW==1))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if(i==1)
		{//B相电压统计为合格
			MWR(0x100000,ADDR_DATABUFF+66+(9*i)+6,3);
			MWR(0x100000,ADDR_DATABUFF+(66+27)+18+66+(9*i)+6,3);
			continue;
		}
		#endif
		if(Check_ACSamplePn(Pn+1)==0)//有效交采测量点号检查;返回0=无效,1=有效
		{
			a=MRR(x+(2*i),2);//取当前电压
		}
		else
		{
		#if (((USER/100)==13) || ((USER/100)==11))//黑龙江、吉林
			//黑龙江跑台体时，台体升源后大约1分半钟后就要召读事件，若用交采1分钟平均电压的话，事件产生较慢，故改成当前电压
			a=MRR(x+(2*i),2);//取当前电压
		#else
			a=MRR(ADDR_Average_Voltage+(2*i),2);//取内部交采A,B,C相1分钟平均电压(上电初始为0xee)
		#endif	
		}
		if(a==0xeeee)
		{
			continue;
		}
		switch(p8[1+(4*i)+3])//原越限稳定标志:0=没越限,1=越上上限,2=越下下限
		{
			case 0://没越限
				b=MRR(y+6,2);//取电压上上限
				if(b==0)
				{
					b=0xffff;//事件判断阀值为0时,不做判断
				}
				if(a>=b)
				{//越上上限
					if(p8[1+(4*i)+2]!=1)
					{//原没越上上限
						p8[1+(4*i)+2]=1;//当前越上上限标志
						z=MRR(y+8,1)*60;//越限持续时间
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{//原越上上限
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+(4*i)+3]=1;//越限稳定标志=1
							gfs|=1<<i;//越上上限的ABC相产生标志
							//越上上限日月累计时间+越限持续判断时间
							z=MRR(y+8,1);//越限持续时间
							if(z!=0x0)
							{
								//越上上限日累计时间
								c=MRR(ADDR_DATABUFF+(10*i),2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i),2);
								//越上上限月累计时间
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i),2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i),2);
//							#if (USER/100)!=5//不是上海版;上海要求:越上上限时,要求不能统计到越上限中去
								//日越上限时间加分
								c=MRR(ADDR_DATABUFF+(10*i)+4,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i)+4,2);
								//月越上限时间加分
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
//							#endif
							}
						}
					}
					break;
				}
				b=MRR(y+11,2);//取下下限
				if(a<b)
				{//越下下限
					if(p8[1+(4*i)+2]!=2)
					{//原没越下下限
						p8[1+(4*i)+2]=2;//当前越下下限标志
						z=MRR(y+13,1)*60;//越限持续时间
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{//原越下下限
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+(4*i)+3]=2;//越限稳定标志=2
							gfx|=1<<i;//越下下限的ABC相产生标志
							//越下下限日月累计时间+越限持续判断时间
							z=MRR(y+13,1);//越限持续时间
							if(z!=0x0)
							{
								//越下下限日累计时间
								c=MRR(ADDR_DATABUFF+(10*i)+2,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i)+2,2);
								//越下下限月累计时间
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
//							#if (USER/100)!=5//不是上海版;上海要求:越下下限时,要求不能统计到越下限中去
								//日越下限时间加分
								c=MRR(ADDR_DATABUFF+(10*i)+6,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i)+6,2);
								//月越下限时间加分
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
//							#endif
							}
						}
					}
					break;
				}
				p8[1+(4*i)+2]=0;//原没越限标志(上上限,下下限)
				if(t!=0x0)
				{
					b=MRR(y,2);//取电压上限
					if(b==0)
					{
						b=0xffff;//事件判断阀值为0时,不做判断
					}
					if(a>=b)
					{//越上限
						//日越上限时间加分
						c=MRR(ADDR_DATABUFF+(10*i)+4,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(10*i)+4,2);
						//月越上限时间加分
						c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
						break;
					}
					b=MRR(y+2,2);//取电压下限
					if(a<b)
					{//越下限
						//日越下限时间加分
						c=MRR(ADDR_DATABUFF+(10*i)+6,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(10*i)+6,2);
						//月越下限时间加分
						c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
						break;
					}
					//日合格电压时间加分
					c=MRR(ADDR_DATABUFF+(10*i)+8,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+8,2);
					//月合格电压时间加分
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+8,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+8,2);
				}
				break;
			case 1://越上上限
				b=MRR(y+6,2);//取电压上上限
				c=MRR(y+9,2);//取电压上上限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(b==0)
				{
					b=0xffff;//事件判断阀值为0时,不做判断
				}
				if(a<b)
				{
					if(p8[1+(4*i)+2]==1)
					{//原当前越上上限
						p8[1+(4*i)+2]=0;//当前没越限标志
						z=MRR(y+8,1)*60;//越限持续时间
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+(4*i)+3]=0;//越限稳定标志=0
							unfs|=1<<i;//越上上限的ABC相恢复标志
						}
					}
				}
				else
				{
					p8[1+(4*i)+2]=1;//原当前越上上限
				}
				if(t!=0x0)
				{
					//越上上限日累计时间
					c=MRR(ADDR_DATABUFF+(10*i),2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i),2);
					//越上上限月累计时间
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i),2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i),2);
//				#if (USER/100)!=5//不是上海版;上海要求:越下下限时,要求不能统计到越下限中去
					//日越上限时间加分
					c=MRR(ADDR_DATABUFF+(10*i)+4,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+4,2);
					//月越上限时间加分
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
//				#endif	
				}
				break;
			case 2://越下下限
				b=MRR(y+11,2);//取电压下下限
				c=MRR(y+14,2);//取电压下下限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(a>=b)
				{
					if(p8[1+(4*i)+2]==2)
					{//原当前越下下限
						p8[1+(4*i)+2]=0;//当前没越限标志
						z=MRR(y+13,1)*60;//越限持续时间
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+(4*i)+3]=0;//越限稳定标志=0
							unfx|=1<<i;//越下下限的ABC相恢复标志
						}
					}
				}
				else
				{
					p8[1+(4*i)+2]=2;//原当前越下下限
				}
				if(t!=0x0)
				{
					//越下下限日累计时间
					c=MRR(ADDR_DATABUFF+(10*i)+2,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+2,2);
					//越下下限月累计时间
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
//				#if (USER/100)!=5//不是上海版;上海要求:越下下限时,要求不能统计到越下限中去
					//日越下限时间加分
					c=MRR(ADDR_DATABUFF+(10*i)+6,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+6,2);
					//月越下限时间加分
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
//				#endif
				}
				break;
			default:
				p8[1+(4*i)+3]=0;//越限稳定标志=0
				break;
		}
		//日最大电压
		b=MRR(ADDR_DATABUFF+30+(10*i),2);
		if(a>b)
		{
			MWR(a,ADDR_DATABUFF+30+(10*i),2);
			MR(ADDR_DATABUFF+30+(10*i)+2,ADDR_TYMDHMS+1,3);
		}
		//月最大电压
		b=MRR(ADDR_DATABUFF+(66+27)+18+30+(10*i),2);
		if(a>b)
		{
			MWR(a,ADDR_DATABUFF+(66+27)+18+30+(10*i),2);
			MR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+2,ADDR_TYMDHMS+1,3);
		}
		//日最小电压
		b=MRR(ADDR_DATABUFF+30+(10*i)+5+2,3);
		if(b==0x0)
		{//原最小电压=空,当前电压作为最小电压
			MWR(a,ADDR_DATABUFF+30+(10*i)+5,2);
			MR(ADDR_DATABUFF+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
		}
		else
		{
			b=MRR(ADDR_DATABUFF+30+(10*i)+5,2);
			if(a<b)
			{
				MWR(a,ADDR_DATABUFF+30+(10*i)+5,2);
				MR(ADDR_DATABUFF+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
			}
		}
		//月最小电压
		b=MRR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5+2,3);
		if(b==0x0)
		{//原最小电压=空,当前电压作为最小电压
			MWR(a,ADDR_DATABUFF+(66+27)+18+30+(10*i)+5,2);
			MR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
		}
		else
		{
			b=MRR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5,2);
			if(a<b)
			{
				MWR(a,ADDR_DATABUFF+(66+27)+18+30+(10*i)+5,2);
				MR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
			}
		}
		//日平均电压
		a=bcd_hex(a);
		b=MRR(ADDR_DATABUFF+(66+27)+(6*i),2);
		b++;//次数+1
		MWR(b,ADDR_DATABUFF+(66+27)+(6*i),2);
		c=MRR(ADDR_DATABUFF+(66+27)+(6*i)+2,4);
		c+=a;
		MWR(c,ADDR_DATABUFF+(66+27)+(6*i)+2,4);
		c/=b;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+60+(2*i),2);
		//月平均电压
		b=MRR(ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i),2);
		b++;//次数+1
		MWR(b,ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i),2);
		c=MRR(ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i)+2,4);
		c+=a;
		MWR(c,ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i)+2,4);
		c/=b;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+(66+27)+18+60+(2*i),2);
		
		//日电压越上限率 见附录A.25	%	3
		a=MRR(ADDR_DATABUFF+(10*i)+4,2);//越上限日累计时间
		b=MRR(ADDR_DATABUFF+(10*i)+4+2,2);//越下限日累计时间
//	#if (USER/100)==5//不是上海版;上海要求:越下下限时,要求不能统计到越下限中去
//		c=MRR(ADDR_DATABUFF+(10*i),2);	//越上上限日累计时间
//		a+=c;
//		c=MRR(ADDR_DATABUFF+(10*i)+2,2);//越下下限日累计时间		
//		b+=c;						
//	#endif
		c=MRR(ADDR_DATABUFF+(10*i)+4+4,2);//合格日累计时间
		z=a+b+c;
		if(z==0)
		{
			a=0;
			b=0;
			c=0;
			z=1;
		}
		d64=a;
		d64*=100000;//3位小数的%
		a=d64/z;
		a=hex_bcd(a);
		MWR(a,ADDR_DATABUFF+66+(9*i),3);
		//日电压越下限率 见附录A.25	%	3
		d64=b;
		d64*=100000;//3位小数的%
		b=d64/z;
		b=hex_bcd(b);
		MWR(b,ADDR_DATABUFF+66+(9*i)+3,3);
		//日电压合格率 见附录A.25	%	3
		d64=c;
		d64*=100000;//3位小数的%
		c=d64/z;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+66+(9*i)+6,3);
		//月电压越上限率 见附录A.25	%	3
		a=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);//越上限月累计时间
		b=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);//越下限月累计时间
//	#if (USER/100)==5//不是上海版;上海要求:越下下限时,要求不能统计到越下限中去
//		c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i),2);//越上上限月累计时间
//		a+=c;
//		c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);//越下下限月累计时间
//		b+=c;
//	#endif
		c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4+4,2);//合格日累计时间
		z=a+b+c;
		if(z==0)
		{
			a=0;
			b=0;
			c=0;
			z=1;
		}
		d64=a;
		d64*=100000;//3位小数的%
		a=d64/z;
		a=hex_bcd(a);
		MWR(a,ADDR_DATABUFF+(66+27)+18+66+(9*i),3);
		//月电压越下限率 见附录A.25	%	3
		d64=b;
		d64*=100000;//3位小数的%
		b=d64/z;
		b=hex_bcd(b);
		MWR(b,ADDR_DATABUFF+(66+27)+18+66+(9*i)+3,3);
		//月电压合格率 见附录A.25	%	3
		d64=c;
		d64*=100000;//3位小数的%
		c=d64/z;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+(66+27)+18+66+(9*i)+6,3);
	}
	MW(ADDR_DATABUFF,ADDR_AFN0DF27_SOUR+((66+27)*Pn),(66+27));
	MW(ADDR_DATABUFF+(66+27),ADDR_AFN0DF27_PAR+(18*Pn),18);
	MW(ADDR_DATABUFF+(66+27)+18,ADDR_AFN0DF35_SOUR+((66+27)*Pn),(66+27));
	MW(ADDR_DATABUFF+(66+27)+18+(66+27),ADDR_AFN0DF35_PAR+(18*Pn),18);
	//事件
//	if(unfs!=0)//越上上限的ABC相恢复标志
//	{
//		unfs|=1<<6;
//		ERC24_Event(Pn+1,0,unfs);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	if(unfx!=0)//越下下限的ABC相恢复标志
//	{
//		unfx|=2<<6;
//		ERC24_Event(Pn+1,0,unfx);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	if(gfs!=0)//越上上限的ABC相产生标志
//	{
//		gfs|=1<<6;
//		ERC24_Event(Pn+1,1,gfs);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	if(gfx!=0)//越下下限的ABC相产生标志
//	{
//		gfx|=2<<6;
//		ERC24_Event(Pn+1,1,gfx);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
	if(ERC_Event_Set(24)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8[13]=0;//B0-B2分别ABC越上上限事件已记录,B4-B6分别ABC越上限事件已记录
	}
	else
	{
		//越上上限恢复
		x=0;
		if(p8[4]==1)
		{
			x|=1;
		}
		if(p8[8]==1)
		{
			x|=2;
		}
		if(p8[12]==1)
		{
			x|=4;
		}
		y=x^(p8[13]&0xf);
		if(y!=0)
		{
			//恢复
			z=y&(~x);
			if(z!=0)
			{
				p8[13]&=(~z);
				z|=1<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,0,z);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
		//越下下限恢复
		x=0;
		if(p8[4]==2)
		{
			x|=0x10;
		}
		if(p8[8]==2)
		{
			x|=0x20;
		}
		if(p8[12]==2)
		{
			x|=0x40;
		}
		y=x^(p8[13]&0xf0);
		if(y!=0)
		{
			//恢复
			z=y&(~x);
			if(z!=0)
			{
				p8[13]&=(~z);
				z>>=4;
				z|=2<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,0,z);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
		//越上上限产生
		x=0;
		if(p8[4]==1)
		{
			x|=1;
		}
		if(p8[8]==1)
		{
			x|=2;
		}
		if(p8[12]==1)
		{
			x|=4;
		}
		y=x^(p8[13]&0xf);
		if(y!=0)
		{
			//产生
			z=y&x;
			if(z!=0)
			{
				p8[13]|=z;
				z|=1<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,1,z);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
		//越下下限产生
		x=0;
		if(p8[4]==2)
		{
			x|=0x10;
		}
		if(p8[8]==2)
		{
			x|=0x20;
		}
		if(p8[12]==2)
		{
			x|=0x40;
		}
		y=x^(p8[13]&0xf0);
		if(y!=0)
		{
			//产生
			z=y&x;
			if(z!=0)
			{
				p8[13]|=z;
				z>>=4;
				z|=2<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,1,z);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
	
	}
	
	Pn++;
	Terminal_Ram->I2SUBTask4_3=Pn;
}

void AFN0DF26F34_Computer(void)//F26F34日月总及分相最大需量及发生时间
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u32 Addr;
	u64 d64;
	u8 *p8;
	
	for(Pn=0;Pn<AFN0DF26MaxPn;Pn++)
	{
		d64=MRR(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);
		if(d64==0)
		{//没上次最大需量计算时间
			MW(ADDR_TYMDHMS+1,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);
		}
		p8=(u8*)(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+5);
		if(p8[0]==Comm_Ram->TYMDHMS[1])
		{
			continue;
		}
		p8[0]=Comm_Ram->TYMDHMS[1];
		p8++;
		
		Addr=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		if(Addr!=0)
		{
			for(i=0;i<4;i++)
			{
				x=MRR(Addr+5+(i*3),3);
				if(x!=0xeeeeee)
				{
					x&=0x7fffff;//去符号
					x=bcd_hex(x);
					y=MRR(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+10+(i*4),4);
					y+=x;
					MWR(y,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+10+(i*4),4);
					p8[i]++;//加次数加1
				}
			}
		}
		MC(0,ADDR_DATABUFF,1);
		MR(ADDR_DATABUFF+1,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);
		i=TimeDifference(ADDR_DATABUFF,ADDR_TYMDHMS);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
		i/=60;
		if(i>=15)
		{//15分需量时间到
			MW(ADDR_TYMDHMS+1,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);//上次最大需量计算时间
			for(i=0;i<4;i++)
			{
				x=MRR(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+10+(i*4),4);
				y=p8[i];//加次数
				if(y!=0)
				{
					x/=y;
				}
				x=hex_bcd(x);
				//日最大
				y=MRR(ADDR_AFN0DF26_SOUR+(24*Pn)+(i*6),3);
				if(x>y)
				{
					MWR(x,ADDR_AFN0DF26_SOUR+(24*Pn)+(i*6),3);
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF26_SOUR+(24*Pn)+(i*6)+3,3);
				}
				//月最大
				y=MRR(ADDR_AFN0DF34_SOUR+(24*Pn)+(i*6),3);
				if(x>y)
				{
					MWR(x,ADDR_AFN0DF34_SOUR+(24*Pn)+(i*6),3);
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF34_SOUR+(24*Pn)+(i*6)+3,3);
				}
			}
			//清平均计算
			MC(0,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+6,20);
		}
	}
}





