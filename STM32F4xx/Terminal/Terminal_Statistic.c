
//终端统计
#include "Project.h"
#include "Terminal_Statistic.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Calculate/ASMCalculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"

#include "Terminal_AFN0E_Event.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"



void Init_StatisticData_Command(void)//终端统计数据初始化(命令,全部清除)
{
	MC(0,ADDR_Terminal_StatData_FM_Start,(ADDR_Terminal_StatData_FM_End)-(ADDR_Terminal_StatData_FM_Start));
	MC(0,ADDR_Terminal_StatData_ESRAM_Start,(ADDR_Terminal_StatData_ESRAM_End)-(ADDR_Terminal_StatData_ESRAM_Start));
}



void Terminal_Reset_Count(void)//终端复位次数
{
	u32 i;
	
	i=MRR(ADDR_AFN0DF49_SOUR+2,2);
	i++;
	MWR(i,ADDR_AFN0DF49_SOUR+2,2);
	i=MRR(ADDR_AFN0DF51_SOUR+2,2);
	i++;
	MWR(i,ADDR_AFN0DF51_SOUR+2,2);

}


void AFN0DF25F33_Computer(void)//终端统计.日月总及分相最大有功功率及发生时间,有功功率为零时间
{
	u32 i;
	u32 Pn;
//	u32 PnMap;
	u32 x;
	u32 y;
	u32 z;
	u32 t;
	u32 f;
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_1;
	if(Pn>=AFN0DF25MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF25F33_SOUR);
	t=p8[Pn];
	if(t!=0x0)
	{//分更新
		p8[Pn]=0;//清分更新
		MR(ADDR_DATABUFF,ADDR_AFN0DF25_SOUR+(32*Pn),32);
		MR(ADDR_DATABUFF+32,ADDR_AFN0DF33_SOUR+(32*Pn),32);
		x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
		x+=5;
		p8=(u8 *)(x);
		f=0;//数据更新标志
		for(i=0;i<4;i++)
		{
			if(p8[3*i]!=0xee)
			{
				y=MRR(x+(3*i),3);
				if(y==0xeeeeee)
				{
					continue;
				}
				y&=0x7fffff;
				//日最大比较
				z=MRR(ADDR_DATABUFF+(6*i),3);
				if(y>z)
				{
					f=1;//数据更新标志
					MWR(y,ADDR_DATABUFF+(6*i),3);
					MR(ADDR_DATABUFF+(6*i)+3,ADDR_TYMDHMS+1,3);
				}
				//月最大比较
				z=MRR(ADDR_DATABUFF+32+(6*i),3);
				if(y>z)
				{
					f=1;//数据更新标志
					MWR(y,ADDR_DATABUFF+32+(6*i),3);
					MR(ADDR_DATABUFF+32+(6*i)+3,ADDR_TYMDHMS+1,3);
				}
				//=0
				if(y==0x0)
				{
					f=1;//数据更新标志
					z=MRR(ADDR_DATABUFF+24+(2*i),2);
					z+=t;
					MWR(z,ADDR_DATABUFF+24+(2*i),2);
					z=MRR(ADDR_DATABUFF+32+24+(2*i),2);
					z+=t;
					MWR(z,ADDR_DATABUFF+32+24+(2*i),2);
				}
			}
		}
		if(f!=0x0)
		{
			MW(ADDR_DATABUFF,ADDR_AFN0DF25_SOUR+(32*Pn),32);
			MW(ADDR_DATABUFF+32,ADDR_AFN0DF33_SOUR+(32*Pn),32);
		}
	}
	
	Pn++;
	Terminal_Ram->I2SUBTask4_1=Pn;
}


void AFN0DF29F37_Computer(void)//终端统计.电流统计数据
{
	u32 i;
	u32 Pn;
	u32 PnMap;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 gfs;//越上上限的ABC相产生标志
	u32 gfx;//越上限的ABC相产生标志
	u32 unfs;//越上上限的ABC相恢复标志
	u32 unfx;//越上限的ABC相恢复标志
	u32 t;
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_4;
	if(Pn>=AFN0DF29MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF29F37_SOUR+(LEN_AFN0DF29F37_SOUR*Pn));
	t=p8[0];
	p8[0]=0;
	
	MR(ADDR_DATABUFF,ADDR_AFN0DF29_SOUR+(38*Pn),38);
	MR(ADDR_DATABUFF+38,ADDR_AFN0DF37_SOUR+(38*Pn),38);
	PnMap=(Pn+1);
	if(PnMap>AFN04F26MaxPn)
	{//PnMap>测量点号(测量点限值参数)
		Pn++;
		Terminal_Ram->I2SUBTask4_4=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
	x+=43;
	y=ADDR_AFN04F26+(57*(PnMap-1));
	gfs=0;//越上上限的ABC相产生标志
	gfx=0;//越上限的ABC相产生标志
	unfs=0;//越上上限的ABC相恢复标志
	unfx=0;//越上限的ABC相恢复标志
	//ABC相
	for(i=0;i<3;i++)
	{
		a=MRR(x+(3*i),3);//取当前电流
		if(a==0xeeeeee)
		{
			continue;
		}
		a&=0x7fffff;
		switch(p8[1+(4*i)+3])//原越限标志:0=没越限,1=越上上限,2=越上限
		{
			case 0://没越限
				b=MRR(y+16,3);//取上上限
				b&=0x7fffff;
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a>=b)
				{//越上上限
					if(p8[1+(4*i)+2]!=1)
					{//原没越上上限
						p8[1+(4*i)+2]=1;//当前越上上限标志
						z=MRR(y+19,1)*60;//越限持续时间
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
							z=MRR(y+19,1);//越限持续时间
							if(z!=0x0)
							{
								//越上上限日累计时间
								c=MRR(ADDR_DATABUFF+(4*i),2);
								c+=z;
								MWR(c,ADDR_DATABUFF+(4*i),2);
								//越上上限月累计时间
								c=MRR(ADDR_DATABUFF+38+(4*i),2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+(4*i),2);
//							#if (USER/100)!=5//不是上海版;上海要求:越上上限时,要求不能统计到越上限中去
								//越上限日累计时间
								c=MRR(ADDR_DATABUFF+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+(4*i)+2,2);
								//越上限月累计时间
								c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
//							#endif
							}
						}
					}
					break;
				}
				b=MRR(y+22,3);//取上限
				b&=0x7fffff;
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a>=b)
				{//越上限
					if(p8[1+(4*i)+2]!=2)
					{//原没越上限
						p8[1+(4*i)+2]=2;//当前越上限标志
						z=MRR(y+25,1)*60;//越限持续时间
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{//原越上限
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+(4*i)+3]=2;//越限稳定标志=2
							gfx|=1<<i;//越上限的ABC相产生标志
							//越上限日月累计时间+越限持续判断时间
							z=MRR(y+25,1);//越限持续时间
							if(z!=0x0)
							{
								//越上限日累计时间
								c=MRR(ADDR_DATABUFF+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+(4*i)+2,2);
								//越上限月累计时间
								c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
							}
						}
					}
					break;
				}
				p8[1+(4*i)+2]=0;//原没越限标志(上上限,上限)
				break;
			case 1://越上上限
				b=MRR(y+16,3);//取上上限
				b&=0x7fffff;
				c=MRR(y+20,2);//取上上限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a<b)
				{
					b=MRR(y+22,3);//取上限
					b&=0x7fffff;
					if(b==0)
					{
						b=0xffffff;//事件判断阀值为0时,不做判断
					}
					if(a>=b)
					{//越上限
						if(p8[1+(4*i)+2]!=2)
						{//原没越上限
							p8[1+(4*i)+2]=2;//当前越上限标志
							z=MRR(y+19,1)*60;//越限持续时间
							p8[1+(4*i)]=z;
							p8[1+(4*i)+1]=z>>8;
						}
						else
						{//原越上限
							z=p8[1+(4*i)];
							z|=p8[1+(4*i)+1];
							if(z==0x0)
							{//越限持续时间定时到
								p8[1+(4*i)+3]=2;//越限稳定标志=2
								unfs|=1<<i;//越上上限的ABC相恢复标志
								gfx|=1<<i;//越上限的ABC相产生标志
							}
						}
					}
					else
					{
						if(p8[1+(4*i)+2]!=0)
						{//当前越上上限
							p8[1+(4*i)+2]=0;//当前没越限标志
							z=MRR(y+19,1)*60;//越限持续时间
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
				}
				else
				{
					p8[1+(4*i)+2]=1;//原当前越上上限
				}
				if(t!=0x0)
				{
					//越上上限日累计时间
					c=MRR(ADDR_DATABUFF+(4*i),2);
					c+=t;
					MWR(c,ADDR_DATABUFF+(4*i),2);
					//越上上限月累计时间
					c=MRR(ADDR_DATABUFF+38+(4*i),2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+(4*i),2);
//				#if (USER/100)!=5//不是上海版;上海要求:越上上限时,要求不能统计到越上限中去
					//越上限日累计时间
					c=MRR(ADDR_DATABUFF+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+(4*i)+2,2);
					//越上限月累计时间
					c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
//				#endif
				}
				break;
			case 2://越上限
				b=MRR(y+22,3);//取上限
				b&=0x7fffff;
				c=MRR(y+26,2);//取上限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a<b)
				{
					if(p8[1+(4*i)+2]!=0)
					{//原当前越上限
						p8[1+(4*i)+2]=0;//当前没越限标志
						z=MRR(y+25,1)*60;//越限持续时间
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
							unfx|=1<<i;//越上限的ABC相恢复标志
						}
					}
				}
				else
				{
					b=MRR(y+16,3);//取上上限
					b&=0x7fffff;
					if(b==0)
					{
						b=0xffffff;//事件判断阀值为0时,不做判断
					}
					if(a>=b)
					{//越上上限
						if(p8[1+(4*i)+2]!=1)
						{//原没越上上限
							p8[1+(4*i)+2]=1;//当前越上上限标志
							z=MRR(y+25,1)*60;//越限持续时间
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
								unfx|=1<<i;//越上限的ABC相恢复标志
								gfs|=1<<i;//越上上限的ABC相产生标志
							}
						}
					}
					else
					{
						p8[1+(4*i)+2]=2;//原当前越上限
					}
				}
				if(t!=0x0)
				{
					//越上限日累计时间
					c=MRR(ADDR_DATABUFF+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+(4*i)+2,2);
					//越上限月累计时间
					c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
				}
				break;
			default:
				p8[1+(4*i)+3]=0;//越限稳定标志=0
				break;
		}
		//日最大值
		b=MRR(ADDR_DATABUFF+14+(6*i),3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+(3*i),3),ADDR_DATABUFF+14+(6*i),3);
			MR(ADDR_DATABUFF+14+(6*i)+3,ADDR_TYMDHMS+1,3);
		}
		//月最大值
		b=MRR(ADDR_DATABUFF+38+14+(6*i),3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+(3*i),3),ADDR_DATABUFF+38+14+(6*i),3);
			MR(ADDR_DATABUFF+38+14+(6*i)+3,ADDR_TYMDHMS+1,3);
		}
	}	
	//零序电流	
	a=MRR(x+9,3);//取当前电流
	if(a!=0xeeeeee)
	{
		a&=0x7fffff;
		switch(p8[1+12+3])//原越限标志:0=没越限,1=越上限
		{
			case 0://没越限
				b=MRR(y+28,3);//取上限
				b&=0x7fffff;
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a>=b)
				{//越上限
					if(p8[1+12+2]!=1)
					{//原没越上限
						p8[1+12+2]=1;//当前越上限标志
						z=MRR(y+31,1)*60;//越限持续时间
						p8[1+12]=z;
						p8[1+12+1]=z>>8;
					}
					else
					{//原越上限
						z=p8[1+12];
						z|=p8[1+12+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+12+3]=1;//越限稳定标志=1
							//越上限日月累计时间+越限持续判断时间
							z=MRR(y+31,1);//越限持续时间
							if(z!=0x0)
							{
								//越上限日累计时间
								c=MRR(ADDR_DATABUFF+12,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+12,2);
								//越上限月累计时间
								c=MRR(ADDR_DATABUFF+38+12,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+12,2);
							}
						}
					}
					break;
				}
				p8[1+12+2]=0;//原没越限标志
				break;
			case 1://越上限
				b=MRR(y+28,3);//取上限
				b&=0x7fffff;
				c=MRR(y+32,2);//取上限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a<b)
				{
					if(p8[1+12+2]==1)
					{//原当前越上限
						p8[1+12+2]=0;//当前没越限标志
						z=MRR(y+31,1)*60;//越限持续时间
						p8[1+12]=z;
						p8[1+12+1]=z>>8;
					}
					else
					{
						z=p8[1+12];
						z|=p8[1+12+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+12+3]=0;//越限稳定标志=0
						}
					}
				}
				else
				{
					p8[1+12+2]=1;//原当前越上限
				}
				if(t!=0x0)
				{
					//越上限日累计时间
					c=MRR(ADDR_DATABUFF+12,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+12,2);
					//越上限月累计时间
					c=MRR(ADDR_DATABUFF+38+12,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+12,2);
				}
				break;
			default:
				p8[1+12+3]=0;//越限稳定标志=0
				break;
		}
		//日最大值
		b=MRR(ADDR_DATABUFF+32,3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+9,3),ADDR_DATABUFF+32,3);
			MR(ADDR_DATABUFF+32+3,ADDR_TYMDHMS+1,3);
		}
		//月最大值
		b=MRR(ADDR_DATABUFF+38+32,3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+9,3),ADDR_DATABUFF+38+32,3);
			MR(ADDR_DATABUFF+38+32+3,ADDR_TYMDHMS+1,3);
		}
	}	

	MW(ADDR_DATABUFF,ADDR_AFN0DF29_SOUR+(38*Pn),38);
	MW(ADDR_DATABUFF+38,ADDR_AFN0DF37_SOUR+(38*Pn),38);
	//事件
//	Pn++;
//	if(unfs!=0)//越上上限的ABC相恢复标志
//	{
//		unfs|=1<<6;
//		ERC25_Event(PnMap,0,unfs);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	if(unfx!=0)//越上限的ABC相恢复标志
//	{
//		unfx|=2<<6;
//		ERC25_Event(PnMap,0,unfx);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	if(gfs!=0)//越上上限的ABC相产生标志
//	{
//		gfs|=1<<6;
//		ERC25_Event(PnMap,1,gfs);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	if(gfx!=0)//越上限的ABC相产生标志
//	{
//		gfx|=2<<6;
//		ERC25_Event(PnMap,1,gfx);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
//	}
//	Pn--;
	if(ERC_Event_Set(25)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8[17]=0;//B0-B2分别ABC越上上限事件已记录,B4-B6分别ABC越上限事件已记录
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
		y=x^(p8[17]&0xf);
		if(y!=0)
		{
			//恢复
			z=y&(~x);
			if(z!=0)
			{
				p8[17]&=(~z);
				z|=1<<6;
			#if ERC25==1
				ERC25_Event(PnMap,0,z);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
		//越上限恢复
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
		y=x^(p8[17]&0xf0);
		if(y!=0)
		{
			//恢复
			z=y&(~x);
			if(z!=0)
			{
				p8[17]&=(~z);
				z>>=4;
				z|=2<<6;
			#if ERC25==1
				ERC25_Event(PnMap,0,z);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
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
		y=x^(p8[17]&0xf);
		if(y!=0)
		{
			//产生
			z=y&x;
			if(z!=0)
			{
				p8[17]|=z;
				z|=1<<6;
			#if ERC25==1
				ERC25_Event(PnMap,1,z);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
		//越上限产生
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
		y=x^(p8[17]&0xf0);
		if(y!=0)
		{
			//产生
			z=y&x;
			if(z!=0)
			{
				p8[17]|=z;
				z>>=4;
				z|=2<<6;
			#if ERC25==1
				ERC25_Event(PnMap,1,z);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
			#endif
			}
		}
		
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_4=Pn;
}


void AFN0DF28F36_Computer(void)//终端统计.电压电流不平衡
{
	u32 Pn;
	u32 pw;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 max;
	u32 min;
	u32 unu;//电压不平衡
	u32 uni;//电流不平衡
	u32 t;
	u32 gf;//产生标志
	u32 unf;//恢复标志
	
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_5;
	if(Pn>=AFN0DF28MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn));
	t=p8[0];
	p8[0]=0;
	if(Pn>=AFN04F25MaxPn)
	{//Pn>测量点号(测量点基本参数)
		Pn=0;
		Terminal_Ram->I2SUBTask4_5=Pn;
		return;
	}
	x=ADDR_AFN04F25+(11*Pn);
	pw=MRR(x+10,1);
	pw&=0x3;//D0～D1编码表示测量点电源接线方式，数值1～3依次表示三相三线、三相四线、单相表
	x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
	//电压不平衡
	a=MRR(x+37,2);
	b=MRR(x+37+2,2);
	c=MRR(x+37+4,2);

	gf=0;
	if(a!=0xeeee)
	{
		max=a;
		min=a;
		gf|=1;
	}
	if(b!=0xeeee)
	{
		if(pw==2)
		{
			if(b>max)
			{
				max=b;
			}
			if(b<min)
			{
				min=b;
			}
			gf|=2;
		}
	}
	if(c!=0xeeee)
	{
		if(c>max)
		{
			max=c;
		}
		if(c<min)
		{
			min=c;
		}
		gf|=4;
	}
	switch(pw)
	{
		case 1:
			if(gf!=0x5)
			{
				goto end;
			}
			break;
		case 2:
			if(gf!=0x7)
			{
				goto end;
			}
			break;
		default:
			goto end;
	}
	if(max!=0x0)
	{
		min=bcd_hex(min);
		max=bcd_hex(max);
		min=max-min;
		unu=(min*1000)/max;//电压不平衡
		unu=hex_bcd(unu);
	}
	else
	{
		unu=0;
	}
	MWR(unu,ADDR_AFN0DF28F36_SOUR+(Pn*LEN_AFN0DF28F36_SOUR)+12,2);//存当前电压不平衡度
	
	//电流不平衡
	a=MRR(x+43,3);
	b=MRR(x+43+3,3);
	c=MRR(x+43+6,3);
	gf=0;
	if(a!=0xeeeeee)
	{
		a&=0x7fffff;
		max=a;
		min=a;
		gf|=1;
	}
	if(b!=0xeeee)
	{
		if(pw==2)
		{
			b&=0x7fffff;
			if(b>max)
			{
				max=b;
			}
			if(b<min)
			{
				min=b;
			}
			gf|=2;
		}
	}
	if(c!=0xeeee)
	{
		c&=0x7fffff;
		if(c>max)
		{
			max=c;
		}
		if(c<min)
		{
			min=c;
		}
		gf|=4;
	}
	switch(pw)
	{
		case 1:
			if(gf!=0x5)
			{
				goto end;
			}
			break;
		case 2:
			if(gf!=0x7)
			{
				goto end;
			}
			break;
		default:
			goto end;
	}
	if(max!=0x0)
	{
		min=bcd_hex(min);
		max=bcd_hex(max);
		min=max-min;
		uni=(min*1000)/max;//电流不平衡
		uni=hex_bcd(uni);
	}
	else
	{
		uni=0;
	}
	MWR(uni,ADDR_AFN0DF28F36_SOUR+(Pn*LEN_AFN0DF28F36_SOUR)+10,2);//存当前电流不平衡度
	
	MR(ADDR_DATABUFF,ADDR_AFN0DF28_SOUR+(14*Pn),14);
	MR(ADDR_DATABUFF+14,ADDR_AFN0DF36_SOUR+(16*Pn),16);
	x=ADDR_AFN04F26+(57*Pn);
	
	gf=0;//产生标志
	unf=0;//恢复标志
	//电流不平衡
	switch(p8[1+3])//原电流不平衡稳定标志,0=没越限,1=越限
	{
		case 0://没越限
			a=MRR(x+51,2);//取限值
			a&=0x7fff;
			if(a==0)
			{
				a=0xffff;//事件判断阀值为0时,不做判断
			}
			if(uni>=a)
			{
					if(p8[1+2]!=1)
					{//原没越上限
						p8[1+2]=1;//当前越上限标志
						z=MRR(x+53,1)*60;//越限持续时间
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{//原越上限
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+3]=1;//越限稳定标志=1
							gf|=2;//产生标志
							//越上限日月累计时间+越限持续判断时间
							z=MRR(x+53,1);//越限持续时间
							if(z!=0x0)
							{
								//越上限日累计时间
								c=MRR(ADDR_DATABUFF,2);
								c+=z;
								MWR(c,ADDR_DATABUFF,2);
								//越上限月累计时间
								c=MRR(ADDR_DATABUFF+14,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+14,2);
							}
						}
					}
					break;
			}
			p8[1+2]=0;//原没越上限标志
			break;
		case 1://越上限
				b=MRR(x+51,2);//取上限
				b&=0x7fff;
				c=MRR(x+54,2);//取上限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(b==0)
				{
					b=0xffff;//事件判断阀值为0时,不做判断
				}
				if(uni<b)
				{
					if(p8[1+2]==1)
					{//原当前越上限
						p8[1+2]=0;//当前没越限标志
						z=MRR(x+53,1)*60;//越限持续时间
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+3]=0;//越限稳定标志=0
							unf|=2;//恢复标志
						}
					}
				}
				else
				{
					p8[1+2]=1;//原当前越上限
				}
				if(t!=0x0)
				{
					//越上限日累计时间
					c=MRR(ADDR_DATABUFF,2);
					c+=t;
					MWR(c,ADDR_DATABUFF,2);
					//越上限月累计时间
					c=MRR(ADDR_DATABUFF+14,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+14,2);
				}
				break;
			default:
				p8[1+3]=0;//越限稳定标志=0
				break;
	}
	//电流不平衡日最大
	a=MRR(ADDR_DATABUFF+4,2);
	if(uni>a)
	{
		MWR(uni,ADDR_DATABUFF+4,2);
		MR(ADDR_DATABUFF+4+2,ADDR_TYMDHMS+1,3);
	}
	//电流不平衡月最大
	a=MRR(ADDR_DATABUFF+14+4,2);
	if(uni>a)
	{
		MWR(uni,ADDR_DATABUFF+14+4,2);
		MR(ADDR_DATABUFF+14+4+2,ADDR_TYMDHMS+1,4);
	}
	//电压不平衡
	switch(p8[1+4+3])//原电压不平衡越稳定标志,0=没越限,1=越限
	{
		case 0://没越限
			a=MRR(x+46,2);//取限值
			a&=0x7fff;
			if(a==0)
			{
				a=0xffff;//事件判断阀值为0时,不做判断
			}
			if(unu>=a)
			{
					if(p8[1+4+2]!=1)
					{//原没越上限
						p8[1+4+2]=1;//当前越上限标志
						z=MRR(x+48,1)*60;//越限持续时间
						p8[1+4]=z;
						p8[1+4+1]=z>>8;
					}
					else
					{//原越上限
						z=p8[1+4];
						z|=p8[1+4+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+4+3]=1;//越限稳定标志=1
							gf|=1;//产生标志
							//越上限日月累计时间+越限持续判断时间
							z=MRR(x+53,1);//越限持续时间
							if(z!=0x0)
							{
								//越上限日累计时间
								c=MRR(ADDR_DATABUFF+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+2,2);
								//越上限月累计时间
								c=MRR(ADDR_DATABUFF+14+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+14+2,2);
							}
						}
					}
					break;
			}
			p8[1+4+2]=0;//原没越上限标志
			break;
		case 1://越上限
				b=MRR(x+46,2);//取上限
				b&=0x7fff;
				c=MRR(x+49,2);//取上限恢复系数
				b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
				if(b==0)
				{
					b=0xffff;//事件判断阀值为0时,不做判断
				}
				if(unu<b)
				{
					if(p8[1+4+2]==1)
					{//原当前越上限
						p8[1+4+2]=0;//当前没越限标志
						z=MRR(x+48,1)*60;//越限持续时间
						p8[1+4]=z;
						p8[1+4+1]=z>>8;
					}
					else
					{
						z=p8[1+4];
						z|=p8[1+4+1];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+4+3]=0;//越限稳定标志=0
							unf|=1;//恢复标志
						}
					}
				}
				else
				{
					p8[1+4+2]=1;//原越限标志
				}
				if(t!=0x0)
				{
					//越上限日累计时间
					c=MRR(ADDR_DATABUFF+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+2,2);
					//越上限月累计时间
					c=MRR(ADDR_DATABUFF+14+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+14+2,2);
				}
				break;
			default:
				p8[1+4+3]=0;//越限稳定标志=0
				break;
	}
	//电压不平衡日最大
	a=MRR(ADDR_DATABUFF+9,2);
	if(unu>a)
	{
		MWR(unu,ADDR_DATABUFF+9,2);
		MR(ADDR_DATABUFF+9+2,ADDR_TYMDHMS+1,3);
	}
	//电压不平衡月最大
	a=MRR(ADDR_DATABUFF+14+10,2);
	if(unu>a)
	{
		MWR(unu,ADDR_DATABUFF+14+10,2);
		MR(ADDR_DATABUFF+14+10+2,ADDR_TYMDHMS+1,4);
	}
	
	MW(ADDR_DATABUFF,ADDR_AFN0DF28_SOUR+(14*Pn),14);
	MW(ADDR_DATABUFF+14,ADDR_AFN0DF36_SOUR+(16*Pn),16);
	//事件
//	if(unf!=0x0)
//	{
//		ERC17_Event(PnMap,0,unf,unu,uni);//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
//	}
//	if(gf!=0x0)
//	{
//		ERC17_Event(PnMap,1,gf,unu,uni);//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
//	}
	if(ERC_Event_Set(17)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8[9]=0;//事件已记录标志,B0=电压不平衡越限,B1=电流不平衡越限
	}
	else
	{
		x=0;
		if(p8[8]!=0)
		{
			x|=1;
		}
		if(p8[4]!=0)
		{
			x|=2;
		}
		y=x^(p8[9]&0x3);
		if(y!=0)
		{
			//恢复
			z=y&(~x);
			if(z!=0)
			{
				p8[9]&=(~z);
			#if ERC17==1
				ERC17_Event(Pn+1,0,z,unu,uni);//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
			#endif
			}
			//产生
			z=y&x;
			if(z!=0)
			{
				p8[9]|=z;
			#if ERC17==1
				ERC17_Event(Pn+1,1,z,unu,uni);//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
			#endif
			}
		}
	}
	
end:
	Pn++;
	Terminal_Ram->I2SUBTask4_5=Pn;
}

void AFN0DF30F38_Computer(void)//终端统计.视在功率越限累计时间
{
	u32 Pn;
	u32 PnMap;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 t;
	u32 gfs;//越上上限产生标志
	u32 gfx;//越上限产生标志
	u32 unfs;//越上上限恢复标志
	u32 unfx;//越上限恢复标志
	
	u8 * p8;
	
	gfs=gfs;//越上上限产生标志
	gfx=gfx;//越上限产生标志
	unfs=unfs;//越上上限恢复标志
	unfx=unfx;//越上限恢复标志
	
	Pn=Terminal_Ram->I2SUBTask4_6;
	if(Pn>=AFN0DF30MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF30F38_SOUR+(LEN_AFN0DF30F38_SOUR*Pn));
	t=p8[0];
	p8[0]=0;
	PnMap=(Pn+1);
	if(PnMap>AFN04F26MaxPn)
	{//PnMap>测量点号(测量点限值参数)
		Pn++;
		Terminal_Ram->I2SUBTask4_6=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
	x+=55;
	a=MRR(x,3);//取当前视在功率
	if(a==0xeeeeee)
	{
		goto end;
	}
	a&=0x7fffff;

	MR(ADDR_DATABUFF,ADDR_AFN0DF30_SOUR+(4*Pn),4);
	MR(ADDR_DATABUFF+4,ADDR_AFN0DF38_SOUR+(4*Pn),4);

	y=ADDR_AFN04F26+(57*(PnMap-1));
	gfs=0;//越上上限产生标志
	gfx=0;//越上限产生标志
	unfs=0;//越上上限恢复标志
	unfx=0;//越上限恢复标志
	switch(p8[1+3])//原越限标志:0=没越限,1=越上上限,2=越上限
	{
		case 0://没越限
			b=MRR(y+34,3);//取上上限
			if(b==0)
			{
				b=0xffffff;//事件判断阀值为0时,不做判断
			}
			if(a>=b)
			{//越上上限
				if(p8[1+2]!=1)
				{//原没越上上限
					p8[1+2]=1;//当前越上上限标志
					z=MRR(y+37,1)*60;//越限持续时间
					p8[1]=z;
					p8[2]=z>>8;
				}
				else
				{//原越上上限
					z=p8[1];
					z|=p8[2];
					if(z==0x0)
					{//越限持续时间定时到
						p8[1+3]=1;//越限稳定标志=1
						gfs=1;//越上上限产生标志
						//越上上限日月累计时间+越限持续判断时间
						z=MRR(y+37,1);//越限持续时间
						if(z!=0x0)
						{
							//越上上限日累计时间
							c=MRR(ADDR_DATABUFF,2);
							c+=z;
							MWR(c,ADDR_DATABUFF,2);
							//越上上限月累计时间
							c=MRR(ADDR_DATABUFF+4,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+4,2);

							//越上限日累计时间(电科院测试越上上限也要计越上限累计时间)
							c=MRR(ADDR_DATABUFF+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+2,2);
							//越上限月累计时间(电科院测试越上上限也要计越上限累计时间)
							c=MRR(ADDR_DATABUFF+4+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+4+2,2);
						}
					}
				}
				break;
			}
			b=MRR(y+40,3);//取上限
			if(b==0)
			{
				b=0xffffff;//事件判断阀值为0时,不做判断
			}
			if(a>=b)
			{//越上限
				if(p8[1+2]!=2)
				{//原没越上限
					p8[1+2]=2;//当前越上限标志
					z=MRR(y+43,1)*60;//越限持续时间
					p8[1]=z;
					p8[2]=z>>8;
				}
				else
				{//原越上限
					z=p8[1];
					z|=p8[2];
					if(z==0x0)
					{//越限持续时间定时到
						p8[1+3]=2;//越限稳定标志=2
						gfx=1;//越上限产生标志
						//越上限日月累计时间+越限持续判断时间
						z=MRR(y+43,1);//越限持续时间
						if(z!=0x0)
						{
							//越上限日累计时间
							c=MRR(ADDR_DATABUFF+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+2,2);
							//越上限月累计时间
							c=MRR(ADDR_DATABUFF+4+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+4+2,2);
						}
					}
				}
				break;
			}
			p8[1+2]=0;//原没越限标志
			break;
		case 1://越上上限
			b=MRR(y+34,3);//取上上限
			c=MRR(y+38,2);//取上上限恢复系数
			b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
			if(b==0)
			{
				b=0xffffff;//事件判断阀值为0时,不做判断
			}
			if(a<b)
			{
				b=MRR(y+40,3);//取上限
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a>=b)
				{//越上限
					if(p8[1+2]!=2)
					{//原没越上限
						p8[1+2]=2;//当前越上限标志
						z=MRR(y+37,1)*60;//越限持续时间
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{//原越上限
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+3]=2;//越限稳定标志=2
							unfs=1;//越上上限恢复标志
							gfx=1;//越上限产生标志
						}
					}
				}
				else
				{
					if(p8[1+2]!=0)
					{//原当前越上上限
						p8[1+2]=0;//当前没越限标志
						z=MRR(y+37,1)*60;//越限持续时间
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+3]=0;//越限稳定标志=0
							unfs=1;//越上上限恢复标志
						}
					}
				}
			}
			else
			{
				p8[1+2]=1;//当前越上上限标志
			}
			if(t!=0x0)
			{
				//越上上限日累计时间
				c=MRR(ADDR_DATABUFF,2);
				c+=t;
				MWR(c,ADDR_DATABUFF,2);
				//越上上限月累计时间
				c=MRR(ADDR_DATABUFF+4,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+4,2);

				//越上限日累计时间(电科院测试越上上限也要计越上限累计时间)
				c=MRR(ADDR_DATABUFF+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+2,2);
				//越上限月累计时间(电科院测试越上上限也要计越上限累计时间)
				c=MRR(ADDR_DATABUFF+4+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+4+2,2);
			}
			break;
		case 2://越上限
			b=MRR(y+40,3);//取上限
			c=MRR(y+44,2);//取上限恢复系数
			b=ReNewValue(b,c);//恢复值计算,入口b=限值,c=恢复系数
			if(b==0)
			{
				b=0xffffff;//事件判断阀值为0时,不做判断
			}
			if(a<b)
			{
				if(p8[1+2]!=0)
				{//原当前越上限
					p8[1+2]=0;//当前没越限标志
					z=MRR(y+43,1)*60;//越限持续时间
					p8[1]=z;
					p8[2]=z>>8;
				}
				else
				{
					z=p8[1];
					z|=p8[2];
					if(z==0x0)
					{//越限持续时间定时到
						p8[1+3]=0;//越限稳定标志=0
						unfx=1;//越上限恢复标志
					}
				}
			}
			else
			{
				b=MRR(y+34,3);//取上上限
				if(b==0)
				{
					b=0xffffff;//事件判断阀值为0时,不做判断
				}
				if(a>=b)
				{//越上上限
					if(p8[1+2]!=1)
					{//原没越上上限
						p8[1+2]=1;//当前越上上限标志
						z=MRR(y+43,1)*60;//越限持续时间
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{//原越上上限
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//越限持续时间定时到
							p8[1+3]=1;//越限稳定标志=1
							unfx=1;//越上限恢复标志
							gfs=1;//越上上限产生标志
						}
					}
				}
				else
				{
					p8[1+2]=2;//原当前越上限
				}
			}
			if(t!=0x0)
			{
				//越上限日累计时间
				c=MRR(ADDR_DATABUFF+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+2,2);
				//越上限月累计时间
				c=MRR(ADDR_DATABUFF+4+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+4+2,2);
			}
			break;
		default:
			p8[1+3]=0;//越限稳定标志=0
			break;
	}
	MW(ADDR_DATABUFF,ADDR_AFN0DF30_SOUR+(4*Pn),4);
	MW(ADDR_DATABUFF+4,ADDR_AFN0DF38_SOUR+(4*Pn),4);
	//事件
//	Pn++;
//	if(unfs!=0)//越上上限恢复标志
//	{
//		b=MRR(y+34,3);//取上上限
//		ERC26_Event(PnMap,0,1<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
//	}
//	if(unfx!=0)//越上限恢复标志
//	{
//		b=MRR(y+40,3);//取上限
//		ERC26_Event(PnMap,0,2<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
//	}
//	if(gfs!=0)//越上上限产生标志
//	{
//		b=MRR(y+34,3);//取上上限
//		ERC26_Event(PnMap,1,1<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
//	}
//	if(gfx!=0)//越上限产生标志
//	{
//		b=MRR(y+40,3);//取上限
//		ERC26_Event(PnMap,1,2<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
//	}
//	Pn--;
	if(ERC_Event_Set(26)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8[5]=0;//视在功率越限事件记录标志,0=没,B0=越上上限,B1=越上限
	}
	else
	{
		x=(p8[4]&0x1)^(p8[5]&0x1);
		if(x!=0)
		{
			if((p8[4]&0x1)==0)
			{//越上上限恢复
				p8[5]&=0xfe;
				b=MRR(y+34,3);//取上上限
			#if ERC26==1
				ERC26_Event(PnMap,0,1<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
			#endif
			}
		}
		x=(p8[4]&0x2)^(p8[5]&0x2);
		if(x!=0)
		{
			if((p8[4]&0x2)==0)
			{//越上限恢复
				p8[5]&=0xfd;
				b=MRR(y+40,3);//取上限
			#if ERC26==1
				ERC26_Event(PnMap,0,2<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
			#endif
			}
		}

		x=(p8[4]&0x1)^(p8[5]&0x1);
		if(x!=0)
		{
			if((p8[4]&0x1)!=0)
			{//越上上限产生
				p8[5]|=1;
				b=MRR(y+34,3);//取上上限
			#if ERC26==1
				ERC26_Event(PnMap,1,1<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
			#endif
			}
		}
		x=(p8[4]&0x2)^(p8[5]&0x2);
		if(x!=0)
		{
			if((p8[4]&0x2)!=0)
			{//越上限产生
				p8[5]|=2;
				b=MRR(y+40,3);//取上限
			#if ERC26==1
				ERC26_Event(PnMap,1,2<<6,a,b);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
			#endif
			}
		}
		
	}
	
end:
	Pn++;
	Terminal_Ram->I2SUBTask4_6=Pn;
}

void AFN0DF31F39_Computer(void)//终端统计.负载率
{
	u32 i;
	u32 x;
	u32 Pn;
	u32 PnMap;
	u64 d64;
	
	Pn=Terminal_Ram->I2SUBTask4_7;
	if(Pn>=AFN0DF31MaxPn)
	{
		Pn=0;
	}
	PnMap=(Pn+1);
	if(PnMap>AFN04F25MaxPn)
	{//PnMap>测量点号(测量点基本参数)
		Pn++;
		Terminal_Ram->I2SUBTask4_7=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
	x+=55;
	x=MRR(x,3);//取当前视在功率
	if(x==0xeeeeee)
	{
		goto end;
	}
	x&=0x7fffff;
	x=bcd_hex(x);
	d64=x;
	d64*=1000;
	x=ADDR_AFN04F25+(11*(PnMap-1));
	x=MRR(x+7,3);//取额定负荷
	if(x==0x0)
	{
		goto end;
	}
	x=bcd_hex(x);
	x=d64/x;
	x=hex_bcd(x);
	
	MR(ADDR_DATABUFF,ADDR_AFN0DF31_SOUR+(10*Pn),10);
	MR(ADDR_DATABUFF+10,ADDR_AFN0DF39_SOUR+(12*Pn),12);
	//日最大
	i=MRR(ADDR_DATABUFF,2);
	if(x>i)
	{
		MWR(x,ADDR_DATABUFF,2);
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,3);
	}
	//日最小
	i=MRR(ADDR_DATABUFF+7,3);
	if(i==0x0)
	{//无最小当前值作为最小值
		MWR(x,ADDR_DATABUFF+5,2);
		MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
	}
	else
	{
		i=MRR(ADDR_DATABUFF+5,2);
		if(x<i)
		{
			MWR(x,ADDR_DATABUFF+5,2);
			MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
		}
	}
	//月最大
	i=MRR(ADDR_DATABUFF+10,2);
	if(x>i)
	{
		MWR(x,ADDR_DATABUFF+10,2);
		MR(ADDR_DATABUFF+10+2,ADDR_TYMDHMS+1,4);
	}
	//月最小
	i=MRR(ADDR_DATABUFF+10+8,4);
	if(i==0x0)
	{//无最小当前值作为最小值
		MWR(x,ADDR_DATABUFF+10+6,2);
		MR(ADDR_DATABUFF+10+8,ADDR_TYMDHMS+1,4);
	}
	else
	{
		i=MRR(ADDR_DATABUFF+10+6,2);
		if(x<i)
		{
			MWR(x,ADDR_DATABUFF+10+6,2);
			MR(ADDR_DATABUFF+10+8,ADDR_TYMDHMS+1,4);
		}
	}

	MW(ADDR_DATABUFF,ADDR_AFN0DF31_SOUR+(10*Pn),10);
	MW(ADDR_DATABUFF+10,ADDR_AFN0DF39_SOUR+(12*Pn),12);
end:
	Pn++;
	Terminal_Ram->I2SUBTask4_7=Pn;
}

void AFN0DF43F44_Computer(void)//终端统计.功率因数区段累计时间
{
	u32 i;
	u32 t;
	u32 Pn;
	u32 s;
	u32 s1;
	u32 s2;
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_9;
	if(Pn>=AFN0DF43MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF43F44_SOUR+(1*Pn));
	t=p8[0];
	if(t!=0x0)
	{//分更新加计数!=0
		p8[0]=0;
		if(Pn>=AFN04F28MaxPn)
		{//Pn>=测量点号(测量点功率因数分段限值)
			Pn++;
			Terminal_Ram->I2SUBTask4_9=Pn;
			return;
		}
		i=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
		i+=29;
		s=MRR(i,2);//取当前总功率因数
		if(s!=0xeeee)
		{
			s&=0x7fff;
			i=ADDR_AFN04F28+(4*Pn);//测量点功率因数分段限值(测量点号)
			s1=MRR(i,2);//取功率因数分段限值l
			s1&=0x7fff;
			s2=MRR(i+2,2);//取功率因数分段限值2
			s2&=0x7fff;
			if(s<s1)
			{//区段1累计时间（功率因数＜定值1）	BIN	min	2
				i=MRR(ADDR_AFN0DF43_SOUR+(6*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF43_SOUR+(6*Pn),2);
				i=MRR(ADDR_AFN0DF44_SOUR+(6*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF44_SOUR+(6*Pn),2);
			}
			else
			{
				if(s<s2)
				{//区段2累计时间（定值1≤功率因数＜定值2） 	BIN	min	2
					i=MRR(ADDR_AFN0DF43_SOUR+(6*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF43_SOUR+(6*Pn)+2,2);
					i=MRR(ADDR_AFN0DF44_SOUR+(6*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF44_SOUR+(6*Pn)+2,2);
				}
				else
				{//区段3累计时间（功率因数≥定值2）  	BIN	min	2
					i=MRR(ADDR_AFN0DF43_SOUR+(6*Pn)+4,2);
					i+=t;
					MWR(i,ADDR_AFN0DF43_SOUR+(6*Pn)+4,2);
					i=MRR(ADDR_AFN0DF44_SOUR+(6*Pn)+4,2);
					i+=t;
					MWR(i,ADDR_AFN0DF44_SOUR+(6*Pn)+4,2);
				}
			}
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_9=Pn;
}


void AFN0DF57F60_Computer(void)//终端统计.日月总加组最大最小有功功率及其发生时间,有功功率为零累计时间
{
#if MaxTotalAddGroup!=0	
	u32 i;
	u32 Pn;
	u32 x;
	u32 y;
	u32 t;
	u64 a64;
	u64 b64;
	
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;
	
	//在至小1个循回抄表,并延时1分后计算免累加0值时间
	p8=(u8 *)(ADDR_AFN0DF57F60_S_Timer);//36 总加组最大最小有功功率在抄表完成后计算秒定时器
	#if (((Project/100)!=2)&&((Project/100)!=4))//不是集中器,公变终端
	if(Comm_Ram->Factory!=0x55)
	{//在工厂模式,RS485-1口用于维护口
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			p8[0]=60;//在抄表完成后计算延时60秒
			return;
		}
	}
	//RS485-2
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			p8[0]=60;//在抄表完成后计算延时60秒
			return;
		}
	#endif
	#if (((Project/100)==2)||((Project/100)==4))//集中器,公变终端
/*
	i=MRR(ADDR_AFN04F37,1);//终端级联通信端口号 BIN  1 
	//RS485-1
	if(Comm_Ram->Factory!=0x55)
	{//在工厂模式,RS485-1口用于维护口
		if(i!=2)
		{
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			if(UARTCtrl->RMComplete==0x0)
			{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
				p8[0]=60;//在抄表完成后计算延时60秒
				return;
			}
		}
	}
	//RS485-2
	if(i!=3)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			p8[0]=60;//在抄表完成后计算延时60秒
			return;
		}
	}
*/
	if(Comm_Ram->Factory!=0x55)
	{//在工厂模式,RS485-1口用于维护口
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			p8[0]=60;//在抄表完成后计算延时60秒
			return;
		}
	}
	//RS485-2
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	if(UARTCtrl->RMComplete==0x0)
	{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
		p8[0]=60;//在抄表完成后计算延时60秒
		return;
	}
	#endif

	//内部交采
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
	if(UARTCtrl->RMComplete==0x0)
	{//1字节 16 D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
		p8[0]=60;//在抄表完成后计算延时60秒
		return;
	}
	if(p8[0]!=0x0)
	{//抄表后延时定时没到
		return;
	}
	
	Pn=Terminal_Ram->I2SUBTask4_2;
	if(Pn>=AFN0DF57MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF57F60_SOUR);
	t=p8[Pn];
	if(t!=0x0)
	{//分更新
		p8[Pn]=0;//清分更新
		MR(ADDR_DATABUFF,ADDR_AFN0DF57_SOUR+(12*Pn),12);
		MR(ADDR_DATABUFF+12,ADDR_AFN0DF60_SOUR+(12*Pn),12);
		x=GetClass1DataAddr(17,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen
		x=MRR(x,2);
		a64=Data02_kW(x&0xefff);//数据格式02转为单位统一为kw的hex补码,有3位小数
		//日最大比较
		y=MRR(ADDR_DATABUFF,2);
		b64=Data02_kW(y&0xefff);//数据格式02转为单位统一为kw的hex补码,有3位小数
		if(a64>b64)
		{
			MWR(x,ADDR_DATABUFF,2);
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,3);
		}
		//日最小
		i=MRR(ADDR_DATABUFF+7,3);
		if(i==0x0)
		{//原没最小值
			if(a64!=0x0)
			{//开始值作为最小值
				MWR(x,ADDR_DATABUFF+5,2);
				MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
			}
		}
		else
		{
			y=MRR(ADDR_DATABUFF+5,2);
			b64=Data02_kW(y&0xefff);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if(a64<b64)
			{
				MWR(x,ADDR_DATABUFF+5,2);
				MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
			}
		}
		//月最大比较
		y=MRR(ADDR_DATABUFF+12,2);
		b64=Data02_kW(y&0xefff);//数据格式02转为单位统一为kw的hex补码,有3位小数
		if(a64>b64)
		{
			MWR(x,ADDR_DATABUFF+12,2);
			MR(ADDR_DATABUFF+12+2,ADDR_TYMDHMS+1,3);
		}
		//月最小
		i=MRR(ADDR_DATABUFF+12+7,3);
		if(i==0x0)
		{//原没最小值
			if(a64!=0x0)
			{//开始值人为最小值
				MWR(x,ADDR_DATABUFF+12+5,2);
				MR(ADDR_DATABUFF+12+7,ADDR_TYMDHMS+1,3);
			}
		}
		else
		{
			y=MRR(ADDR_DATABUFF+12+5,2);
			b64=Data02_kW(y&0xefff);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if(a64<b64)
			{
				MWR(x,ADDR_DATABUFF+12+5,2);
				MR(ADDR_DATABUFF+12+7,ADDR_TYMDHMS+1,3);
			}
		}
		//=0时间
		if(a64==0x0)
		{
			i=MRR(ADDR_DATABUFF+10,2);
			i+=t;
			MWR(i,ADDR_DATABUFF+10,2);
			i=MRR(ADDR_DATABUFF+12+10,2);
			i+=t;
			MWR(i,ADDR_DATABUFF+12+10,2);
		}
		MW(ADDR_DATABUFF,ADDR_AFN0DF57_SOUR+(12*Pn),12);
		MW(ADDR_DATABUFF+12,ADDR_AFN0DF60_SOUR+(12*Pn),12);
	}
	
	Pn++;
	Terminal_Ram->I2SUBTask4_2=Pn;
#endif
}

void AFN0DF65F66_Computer(void)//终端统计.总加组超功率定值月电能量定值的月累计时间累计电能量
{
#if MaxTotalAddGroup!=0	
	u32 i;
	u32 a;
	u32 b;
	u64 a64;
	u64 b64;
	u32 Pn;
	u32 t;
	u8 * p8;
	

	Pn=Terminal_Ram->I2SUBTask4_8;
	if(Pn>=MaxTotalAddGroup)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF65F66_SOUR+(9*Pn));
	t=p8[0];
	if(t!=0x0)
	{//分更新加计数1=0
		p8[0]=0;
		//超功率定值
		a=0;
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		for(i=0;i<4;i++)
		{
			if(p8[8]!=0x0)
			{//超定值标志0=没超,1=超定值
				a=1;
				break;
			}
			p8+=(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup);
		}
		if(a==0x0)
		{//没超定值
			MC(0,ADDR_AFN0DF65F66_SOUR+(9*Pn)+1,4);//清功控超定值已用于计算的原总加有功电能量
		}
		else
		{//超定值
			a=MRR(ADDR_AFN0DF65F66_SOUR+(9*Pn)+1,4);
			b=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);
			b&=0xefffffff;
			MWR(b,ADDR_AFN0DF65F66_SOUR+(9*Pn)+1,4);
			if(a!=0x0)
			{
				a64=Data03_kWh(a);//数据格式03转为单位统一为kwh的hex补码
				b64=Data03_kWh(b);//数据格式03转为单位统一为kwh的hex补码
				if(b64>=a64)
				{
					b64-=a64;
				}
				else
				{
					b64=0;
				}
				a=MRR(ADDR_AFN0DF65_SOUR+(6*Pn)+2,4);
				a64=Data03_kWh(a);//数据格式03转为单位统一为kwh的hex补码
				a64+=b64;
				a=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
				MWR(a,ADDR_AFN0DF65_SOUR+(6*Pn)+2,4);
				//时间
				a=MRR(ADDR_AFN0DF65_SOUR+(6*Pn),2);
				a+=t;
				MWR(a,ADDR_AFN0DF65_SOUR+(6*Pn),2);
			}
		}
		//超月电能量定值
		p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8[0]>101)
		{//控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
			a=MRR(ADDR_AFN0DF65F66_SOUR+(9*Pn)+5,4);
			b=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);
			b&=0xefffffff;
			MWR(b,ADDR_AFN0DF65F66_SOUR+(9*Pn)+5,4);
			if(a!=0x0)
			{
				a64=Data03_kWh(a);//数据格式03转为单位统一为kwh的hex补码
				b64=Data03_kWh(b);//数据格式03转为单位统一为kwh的hex补码
				if(b64>=a64)
				{
					b64-=a64;
				}
				else
				{
					b64=0;
				}
				a=MRR(ADDR_AFN0DF66_SOUR+(6*Pn)+2,4);
				a64=Data03_kWh(a);//数据格式03转为单位统一为kwh的hex补码
				a64+=b64;
				a=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
				MWR(a,ADDR_AFN0DF66_SOUR+(6*Pn)+2,4);
			}
			//时间
			a=MRR(ADDR_AFN0DF66_SOUR+(6*Pn),2);
			a+=t;
			MWR(a,ADDR_AFN0DF66_SOUR+(6*Pn),2);
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_8=Pn;
#endif
}




void AFN0DF116F117F118_Computer(u32 ADDR_Current,u32 ADDR_Sour)//终端统计.电压谐波含有率日最大值及发生时间
{
#if MeterHarmonic==1//谐波计算0=没,1=有
#if (IC_SDRAM!=0)
	u32 Pn;
	u32 n;
	u32 x;
	u32 y;
	
	for(Pn=0;Pn<AFN0DF116MaxPn;Pn++)
	{
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_Current+2+(2*n),2);
			if(x!=0xeeee)
			{
				y=MRR(ADDR_Sour+(6*n),2);
				if(x>y)
				{
					MWR(x,ADDR_Sour+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
				}
			}
		}
		x=MRR(ADDR_Current,2);//总畸变
		if(x!=0xeeee)
		{
			y=MRR(ADDR_Sour+(6*n),2);
			if(x>y)
			{
				MWR(x,ADDR_Sour+(6*n),2);
				MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
			}
		}
		ADDR_Current+=1+222;
		ADDR_Sour+=114;
	}
#endif
#endif
}
void AFN0DF113F114F115_Computer(u32 ADDR_Current,u32 ADDR_Sour)//终端统计.谐波电流日最大值及发生时间
{
#if MeterHarmonic==1//谐波计算0=没,1=有
#if (IC_SDRAM!=0)
	u32 Pn;
	u32 n;
	u32 x;
	u32 y;
	u32 T;
	
	for(Pn=0;Pn<AFN0DF113MaxPn;Pn++)
	{
		T=0;//总畸变
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_Current+(2*n),2);
			if(x!=0xeeee)
			{
				y=MRR(ADDR_Sour+(6*n),2);
				if(x>y)
				{
					MWR(x,ADDR_Sour+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
				}
				x=bcd_hex(x);
				T+=(x*x);
			}
		}
		T=sqrt4_2(T);//开平方
		x=hex_bcd(T);
		y=MRR(ADDR_Sour+(6*n),2);
		if(x>y)
		{
			MWR(x,ADDR_Sour+(6*n),2);
			MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
		}
		ADDR_Current+=1+216;
		ADDR_Sour+=114;
	}
#endif
#endif
}
void AFN0DF121F122F123(u32 ADDR_UH,u32 ADDR_IH,u32 ADDR_UHMax,u32 ADDR_IHMax,u32 ADDR_Time,u32 ABC)//终端统计.相谐波越限日统计
{
#if MeterHarmonic==1//谐波计算0=没,1=有
#if (IC_SDRAM!=0)
	u32 i;
	u32 t;
	u32 Pn;
	u32 n;
	u32 x;
	u32 y;
	u32 TH;//电流总畸变
	u32 ufs;//电压谐波越限标志;D0-D18分别表示总畸变,2次-19次
	u32 ifs;//电流谐波越限标志;D0-D18分别表示总畸变,2次-19次
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_10;
	if(Pn>=AFN0CF57MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn));
	i=p8[0]+(p8[1]<<8);
	t=i/60;
	if(t!=0x0)
	{//分加计数!=0
		ADDR_UH+=((1+222)*Pn);//当前电压含有率地址
		ADDR_IH+=((1+216)*Pn);//当前谐波电流地址
		ADDR_Time+=(77*Pn);//谐波越限日统计数据地址
		ADDR_UHMax+=(144*Pn);//电压含有率最大值及发生时间地址
		ADDR_IHMax+=(144*Pn);//谐波电流最大值及发生时间地址
		
		MWR(19,ADDR_Time,1);//谐波次数
	//电压含有率
		ufs=0;//电压谐波越限标志;D0-D18分别表示总畸变,2次-19次
		//2-19次
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_UH+2+(2*n),2);//取当前含有率
			if(x!=0xeeee)
			{
				y=MRR(ADDR_UHMax+(6*n),2);//取原含有率最大值
				if(x>y)
				{//当前含有率>原含有率
					MWR(x,ADDR_UHMax+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_UHMax+(6*n)+2,4);
				}
				//分次上限
				if((n%2)==0)
				{//偶次
					y=MRR(ADDR_AFN04F60+6+n,2);//取分次偶次含有率上限
				}
				else
				{//奇次
					y=MRR(ADDR_AFN04F60+6+18+n-1,2);//取分次奇次含有率上限
				}
				//不分次上限
				if((n%2)==0)
				{//偶次
					i=MRR(ADDR_AFN04F60+4,2);//取偶次含有率上限
				}
				else
				{//奇次
					i=MRR(ADDR_AFN04F60+2,2);//取奇次含有率上限
				}
				if(i>y)
				{//用上限较大者
					y=i;
				}
				if(y==0)
				{//上限=0没设置
					continue;
				}
				if(x>=y)
				{//超上限
					ufs|=2<<n;//电压谐波越限标志;D0-D18分别表示总畸变,2次-19次
					i=MRR(ADDR_Time+3+(n*2),2);
					i+=t;
					MWR(i,ADDR_Time+3+(n*2),2);
				}
			}
		}
		//总畸变
		x=MRR(ADDR_UH,2);
		if(x!=0xeeee)
		{
			y=MRR(ADDR_UHMax+(6*n),2);
			if(x>y)
			{//当前含有率>原含有率
				MWR(x,ADDR_UHMax+(6*n),2);
				MW(ADDR_TYMDHMS+1,ADDR_UHMax+(6*n)+2,4);
			}
			y=MRR(ADDR_AFN04F60,2);//取总畸变含有率上限
			if(y!=0)
			{
				if(x>=y)
				{//总畸变超上限
					ufs|=1;//电压谐波越限标志;D0-D18分别表示总畸变,2次-19次
					i=MRR(ADDR_Time+1,2);
					i+=t;
					MWR(i,ADDR_Time+1,2);
				}
			}
		}
	//谐波电流
		TH=0;//电流总畸变
		ifs=0;//电流谐波越限标志;D0-D18分别表示总畸变,2次-19次
		//2-19次
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_IH+(2*n),2);//取当前谐波电流
			if(x!=0xeeee)
			{
				i=x;
				i=bcd_hex(i);
				TH+=(i*i);
				y=MRR(ADDR_IHMax+(6*n),2);//取原谐波电流最大值
				if(x>y)
				{//当前谐波电流>原谐波电流
					MWR(x,ADDR_IHMax+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_IHMax+(6*n)+2,4);
				}
				if((n%2)==0)
				{//偶次
					y=MRR(ADDR_AFN04F60+42+2+n,2);//取偶次谐波电流上限
				}
				else
				{//奇次
					y=MRR(ADDR_AFN04F60+42+2+18+n-1,2);//取奇次谐波电流上限
				}
				if(y==0)
				{//上限=0没设置
					continue;
				}
				if(x>=y)
				{//超上限
					ifs|=2<<n;//电流谐波越限标志;D0-D18分别表示总畸变,2次-19次
					i=MRR(ADDR_Time+39+2+(n*2),2);
					i+=t;
					MWR(i,ADDR_Time+39+2+(n*2),2);
				}
			}
		}
		//谐波电流总畸变
		TH=sqrt4_2(TH);//开平方
		TH=hex_bcd(TH);
		y=MRR(ADDR_IHMax+(6*n),2);
		if(TH>y)
		{//当前谐波电流总畸变>原谐波电流总畸变
			MWR(TH,ADDR_IHMax+(6*n),2);
			MW(ADDR_TYMDHMS+1,ADDR_IHMax+(6*n)+2,4);
		}
		y=MRR(ADDR_AFN04F60+42,2);//取谐波电流总畸变上限
		if(y!=0)
		{
			if(TH>=y)
			{//谐波电流总畸变超上限
				ifs|=1;//电流谐波越限标志;D0-D18分别表示总畸变,2次-19次
				i=MRR(ADDR_Time+39,2);
				i+=t;
				MWR(i,ADDR_Time+39,2);
			}
		}
	//事件
	#if ERC15==1//ERC15谐波越限告警
		if(ERC_Event_Set(15)==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			MC(0,ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+2,18);//清谐波越限标志D0-D18分别表示总畸变,2次-19次
		}
		else
		{
			//电压越限事件恢复
			x=MRR(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+2+(ABC*6),3);//原电压谐波越限标志D0-D18分别表示总畸变,2次-19次
			i=ufs^x;
			i&=x;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//长度Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
				MWR(Pn+1,ADDR_DATABUFF+7,2);//D15：起/止标志,D11～D0：pn（测量点号1～2048）
				p8[9]=1<<ABC;//异常标志
				MWR(i,ADDR_DATABUFF+10,3);//谐波越限标志
				MR(ADDR_DATABUFF+13,ADDR_UH,38);//越限时值
				ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			}
			//电流越限事件恢复
			y=MRR(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+5+(ABC*6),3);//原电流谐波越限标志D0-D18分别表示总畸变,2次-19次
			i=ifs^y;
			i&=y;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//长度Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
				MWR(Pn+1,ADDR_DATABUFF+7,2);//D15：起/止标志,D11～D0：pn（测量点号1～2048）
				p8[9]=(1<<ABC)+0x80;//异常标志
				MWR(i,ADDR_DATABUFF+10,3);//谐波越限标志
				MWR(TH,ADDR_DATABUFF+13,2);//越限时电流总畸变值
				MR(ADDR_DATABUFF+15,ADDR_IH,36);//越限时值
				ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			}
			//电压越限事件产生
			i=ufs^x;
			i&=ufs;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//长度Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
				MWR((Pn+1)+0x8000,ADDR_DATABUFF+7,2);//D15：起/止标志,D11～D0：pn（测量点号1～2048）
				p8[9]=1<<ABC;//异常标志
				MWR(i,ADDR_DATABUFF+10,3);//谐波越限标志
				MR(ADDR_DATABUFF+13,ADDR_UH,38);//越限时值
				ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			}
			//电流越限事件产生
			i=ifs^y;
			i&=ifs;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//长度Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
				MWR((Pn+1)+0x8000,ADDR_DATABUFF+7,2);//D15：起/止标志,D11～D0：pn（测量点号1～2048）
				p8[9]=(1<<ABC)+0x80;//异常标志
				MWR(i,ADDR_DATABUFF+10,3);//谐波越限标志
				MWR(TH,ADDR_DATABUFF+13,2);//越限时电流总畸变值
				MR(ADDR_DATABUFF+15,ADDR_IH,36);//越限时值
				ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			}
			
			//新越限标志
			MWR(ufs,ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+2+(ABC*6),3);//电压谐波越限标志D0-D18分别表示总畸变,2次-19次
			MWR(ifs,ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+5+(ABC*6),3);//电流谐波越限标志D0-D18分别表示总畸变,2次-19次
		}
	#endif//#if ERC15==1//ERC15谐波越限告警	
	}
#endif
#endif
}
void AFN0DF121F122F123_Computer(void)//终端统计.谐波越限
{
#if MeterHarmonic==1//谐波计算0=没,1=有
#if (IC_SDRAM!=0)
	u32 i;
	u32 Pn;
	u8 *p8;
	
	Pn=Terminal_Ram->I2SUBTask4_10;
	if(Pn>=AFN0CF57MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn));
	i=p8[0]+(p8[1]<<8);
	if((i/60)!=0x0)
	{//分加计数!=0
		AFN0DF121F122F123(ADDR_AFN0CF58+1+(0*38),ADDR_AFN0CF57+1+(3*36),ADDR_AFN0DF116_SOUR,ADDR_AFN0DF113_SOUR,ADDR_AFN0DF121_SOUR,0);//终端统计.A相谐波越限日统计
		AFN0DF121F122F123(ADDR_AFN0CF58+1+(1*38),ADDR_AFN0CF57+1+(4*36),ADDR_AFN0DF117_SOUR,ADDR_AFN0DF114_SOUR,ADDR_AFN0DF122_SOUR,1);//终端统计.B相谐波越限日统计
		AFN0DF121F122F123(ADDR_AFN0CF58+1+(2*38),ADDR_AFN0CF57+1+(5*36),ADDR_AFN0DF118_SOUR,ADDR_AFN0DF115_SOUR,ADDR_AFN0DF123_SOUR,2);//终端统计.C相谐波越限日统计
		i%=60;
		p8[0]=i;
		p8[1]=i>>8;
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_10=Pn;
#endif
#endif
}



void AFN0DF129F130_Computer(void)//终端统计.直流模拟量越限
{
#if MaxDCPn!=0//最大直流模拟量测量点数
	u32 i;
	s64 x;
	s64 y;
	s64 z;
	u32 t;
	u32 Pn;
	u32 f;//当前越限标志,B0=越上限,B1=越下限
	u8 *p8;
	
	Pn=Terminal_Ram->I2SUBTask4_11;//终端统计数据计算任务.直流模拟量越限
	if(Pn>=MaxDCPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF129F130_SOUR+(LEN_AFN0DF129F130_SOUR*Pn));
	t=p8[0];
	if(t!=0x0)
	{//分更新加计数!=0
		p8[0]=0;
		z=MRR(ADDR_AFN0CF73+(2*Pn),2);//当前直流模拟量	数据格式02	    2
		if(z!=0xeeee)
		{
			f=0;//当前越限标志,B0=越上限,B1=越下限
			x=MRR(ADDR_AFN04F82+(4*Pn),2);//直流模拟量上限	数据格式02	    2
			y=MRR(ADDR_AFN04F82+(4*Pn)+2,2);//直流模拟量下限	数据格式02	    2
			x=Data02_kW((u64)x);//数据格式02转为单位统一为kw的hex补码,有3位小数
			y=Data02_kW((u64)y);//数据格式02转为单位统一为kw的hex补码,有3位小数
			z=Data02_kW((u64)z);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if(z>=x)
			{//当前直流模拟量>=直流模拟量上限
				f=1;//当前越限标志,B0=越上限,B1=越下限
				//直流模拟量越上限日累计时间	BIN	min	2
				i=MRR(ADDR_AFN0DF129_SOUR+(14*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn),2);
				//直流模拟量越上限月累计时间	BIN	min	2
				i=MRR(ADDR_AFN0DF130_SOUR+(14*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn),2);
			}
			else
			{
				if(z<y)
				{//当前直流模拟量<直流模拟量下限
					f=2;//当前越限标志,B0=越上限,B1=越下限
					//直流模拟量越下限日累计时间	BIN	min	2
					i=MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+2,2);
					//直流模拟量越下限月累计时间	BIN	min	2
					i=MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+2,2);
				}
			}
			//
			i=MRR(ADDR_AFN0CF73+(2*Pn),2);//当前直流模拟量	数据格式02	    2
			//日最大值
			if(MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+6,3)==0)
			{//原最大值发生时间=0,即无最大值
				MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+4,2);//原直流模拟量最大值	数据格式02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+6,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+4,2);//原直流模拟量最大值	数据格式02	    2
				x=Data02_kW((u64)x);//数据格式02转为单位统一为kw的hex补码,有3位小数
				if(z>x)
				{//当前>原最大值
					MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+4,2);//原直流模拟量最大值	数据格式02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+6,3);
				}
			}
			//日最小值
			if(MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+11,3)==0)
			{//原最小值发生时间=0,即无最小值
				MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+9,2);//原直流模拟量最大值	数据格式02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+11,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+9,2);//原直流模拟量最小值	数据格式02	    2
				x=Data02_kW((u64)x);//数据格式02转为单位统一为kw的hex补码,有3位小数
				if(z<x)
				{//当前<原最小值
					MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+9,2);//原直流模拟量最大值	数据格式02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+11,3);
				}
			}
			//月最大值
			if(MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+6,3)==0)
			{//原最大值发生时间=0,即无最大值
				MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+4,2);//原直流模拟量最大值	数据格式02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+6,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+4,2);//原直流模拟量最大值	数据格式02	    2
				x=Data02_kW((u64)x);//数据格式02转为单位统一为kw的hex补码,有3位小数
				if(z>x)
				{//当前>原最大值
					MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+4,2);//原直流模拟量最大值	数据格式02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+6,3);
				}
			}
			//月最小值
			if(MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+11,3)==0)
			{//原最小值发生时间=0,即无最小值
				MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+9,2);//原直流模拟量最大值	数据格式02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+11,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+9,2);//原直流模拟量最小值	数据格式02	    2
				x=Data02_kW((u64)x);//数据格式02转为单位统一为kw的hex补码,有3位小数
				if(z<x)
				{//当前<原最小值
					MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+9,2);//原直流模拟量最大值	数据格式02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+11,3);
				}
			}
			//事件
			if(ERC_Event_Set(16)==0)//检查事件记录设置,返回0=不记录,1=记录
			{
				p8[2]=0;//事件已记录标志,B0=越上限,B1=越下限
			}
			else
			{
				//恢复
				t=f^p8[2];
				t&=(~f);
				if(t!=0)
				{
					ERC16_Event(Pn+1,0,t,i);//直流模拟量越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=越限标志,dc=当前直流模拟量
				}
				//产生
				t=f^p8[2];
				t&=f;
				if(t!=0)
				{
					ERC16_Event(Pn+1,1,t,i);//直流模拟量越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=越限标志,dc=当前直流模拟量
				}
				p8[2]=f;//当前超限标志
			}
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_11=Pn;
#endif
}

void AFN0DF124F125F126(u32 Pn,u32 ADDR_SOUR,u32 ABC)//电压谐波电压含有率、谐波电流95%概率值
{
#if MeterHarmonic!=0//谐波计算0=没,1=有
#if (IC_SDRAM!=0)
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Count;
	u8 *p8;
	u16 *p16;
	
	//取原始值
	MR(ADDR_DATABUFF,ADDR_SOUR+(LEN_AFN0DF124_SOUR*Pn),LEN_AFN0DF124_SOUR);
	p8=(u8*)(ADDR_DATABUFF);
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=15;
	if(p8[0]!=i)//实时钟分;每0,15,30,45分计算每日最多取96点;每小时数据保存到FLASH
	{
		p8[0]=i;//上次计算的实时钟0,15,30,45分
		Count=MRR(ADDR_DATABUFF+1,2);
		Count++;//计算次数计数
		MWR(Count,ADDR_DATABUFF+1,2);
		p8[3]=19;//谐波次数N=19
		//总畸变电压含有率95%概率值(第5大值-第1大值)
		x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+1+(38*ABC),2);//取当前总畸变电压含有率
		if(x!=0xeeee)
		{
			p16=(u16*)(ADDR_DATABUFF+4+80+8);
			for(i=5;i!=0;i--)
			{
				if(x>p16[0])
				{
					MR(ADDR_DATABUFF+4+80,ADDR_DATABUFF+4+80+2,(2*i));
					p16[0]=x;
					break;
				}
				p16--;
			}
		}
		//奇偶次畸变电压含有率计算
		y=0;//偶次畸变电压含有率=0
		z=0;//奇次畸变电压含有率=0
		for(i=0;i<9;i++)
		{
			x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+3+(38*ABC)+(4*i),2);//偶次电压含有率
			if(x!=0xeeee)
			{
				x=bcd_hex(x);
				y+=(x*x);
			}
			x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+3+(38*ABC)+(4*i)+2,2);//奇次电压含有率
			if(x!=0xeeee)
			{
				x=bcd_hex(x);
				z+=(x*x);
			}
		}
		y=sqrt4_2(y);//开平方
		y=hex_bcd(y);
		z=sqrt4_2(z);//开平方
		z=hex_bcd(z);
		//奇次畸变电压含有率95%概率值(第5大值-第1大值)
			p16=(u16*)(ADDR_DATABUFF+4+90+8);
			for(i=5;i!=0;i--)
			{
				if(z>p16[0])
				{
					MR(ADDR_DATABUFF+4+90,ADDR_DATABUFF+4+90+2,(2*i));
					p16[0]=z;
					break;
				}
				p16--;
			}
		//偶次畸变电压含有率95%概率值(第5大值-第1大值)
			p16=(u16*)(ADDR_DATABUFF+4+100+8);
			for(i=5;i!=0;i--)
			{
				if(y>p16[0])
				{
					MR(ADDR_DATABUFF+4+100,ADDR_DATABUFF+4+100+2,(2*i));
					p16[0]=y;
					break;
				}
				p16--;
			}
		//2-19次谐波电压含有率95%概率值(第5大值-第1大值)
		for(y=0;y<18;y++)
		{
			x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+3+(38*ABC)+(2*y),2);//取电压含有率
			if(x!=0xeeee)
			{
				p16=(u16*)(ADDR_DATABUFF+4+110+(10*y)+8);
				for(i=5;i!=0;i--)
				{
					if(x>p16[0])
					{
						MR(ADDR_DATABUFF+4+110+(10*y),ADDR_DATABUFF+4+110+(10*y)+2,(2*i));
						p16[0]=x;
						break;
					}
					p16--;
				}
			}
		}
		//总畸变电流计算
		y=0;//总畸变电流=0
		for(i=0;i<18;i++)
		{
			x=MRR(ADDR_AFN0CF57+(LEN_AFN0CF57*Pn)+109+(36*ABC)+(2*i),2);//取各谐波电流
			if(x!=0xeeee)
			{
				x=bcd_hex(x);
				y+=(x*x);
			}
		}
		y=sqrt4_2(y);//开平方
		y=hex_bcd(y);
		//总畸变电流95%概率值(第5大值-第1大值)
			p16=(u16*)(ADDR_DATABUFF+4+290+8);
			for(i=5;i!=0;i--)
			{
				if(y>p16[0])
				{
					MR(ADDR_DATABUFF+4+290,ADDR_DATABUFF+4+290+2,(2*i));
					p16[0]=y;
					break;
				}
				p16--;
			}
		//2-19次谐波电流95%概率值(第5大值-第1大值)
		for(y=0;y<18;y++)
		{
			x=MRR(ADDR_AFN0CF57+(LEN_AFN0CF57*Pn)+109+(36*ABC)+(2*y),2);//取谐波电流
			if(x!=0xeeee)
			{
				p16=(u16*)(ADDR_DATABUFF+4+300+(10*y)+8);
				for(i=5;i!=0;i--)
				{
					if(x>p16[0])
					{
						MR(ADDR_DATABUFF+4+300+(10*y),ADDR_DATABUFF+4+300+(10*y)+2,(2*i));
						p16[0]=x;
						break;
					}
					p16--;
				}
			}
		}
		//当前概率值
		Count*=5;//去5%大值
		Count/=100;
		if(Count>4)
		{
			Count=4;
		}
		for(i=0;i<40;i++)
		{
			MR(ADDR_DATABUFF+4+(2*i),(ADDR_DATABUFF+84+(10*i)+8)-(Count*2),2);
		}
		//存储
		MW(ADDR_DATABUFF,ADDR_SOUR+(LEN_AFN0DF124_SOUR*Pn),LEN_AFN0DF124_SOUR);
	}
#endif
#endif
}

void AFN0DF124F125F126_Computer(void)//终端统计.日冻结电压谐波电压含有率、谐波电流95%概率值
{
#if MeterHarmonic==1//谐波计算0=没,1=有
#if (IC_SDRAM!=0)
	u32 Pn;
	
	Pn=Terminal_Ram->I2SUBTask4_14;//终端统计数据计算任务.日冻结电压谐波电压含有率、谐波电流95%概率值
	if(Pn>=AFN0DF124MaxPn)
	{
		Pn=0;
	}
	
	AFN0DF124F125F126(Pn,ADDR_AFN0DF124_SOUR,0);//A电压谐波电压含有率、谐波电流95%概率值
	AFN0DF124F125F126(Pn,ADDR_AFN0DF125_SOUR,1);//B电压谐波电压含有率、谐波电流95%概率值
	AFN0DF124F125F126(Pn,ADDR_AFN0DF126_SOUR,2);//C电压谐波电压含有率、谐波电流95%概率值
	
	Pn++;
	Terminal_Ram->I2SUBTask4_14=Pn;
#endif
#endif
}


void AFN0DF211_Computer(void)//终端统计.日三相电压、电流不平衡度95%概率值
{
#if (IC_SDRAM!=0)
	u32 i;
	u32 x;
	u32 Count;
	u32 Pn;
	u8 *p8;
	u16 *p16;
	
	Pn=Terminal_Ram->I2SUBTask4_12;//终端统计数据计算任务.日三相电压、电流不平衡度95%概率值
	if(Pn>=AFN0DF211MaxPn)
	{
		Pn=0;
	}
	//取原始值
	MR(ADDR_DATABUFF,ADDR_AFN0DF211_SOUR+(LEN_AFN0DF211_SOUR*Pn),LEN_AFN0DF211_SOUR);
	p8=(u8*)(ADDR_DATABUFF);
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=15;
	if(p8[0]!=i)//实时钟分;每0,15,30,45分计算每日最多取96点;每小时数据保存到FLASH
	{
		p8[0]=i;//上次计算的实时钟0,15,30,45分
		p16=(u16*)(ADDR_DATABUFF+2);
		Count=p16[0];
		Count++;//计算次数计数
		p16[0]=Count;
		//电流不平衡排序
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+10,2);//当前电流不平衡度(上电和初始化时清为0)
		p16=(u16*)(ADDR_DATABUFF+8+8);
		for(i=5;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8,ADDR_DATABUFF+8+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//电压不平衡排序
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+12,2);//当前电压不平衡度(上电和初始化时清为0)
		p16=(u16*)(ADDR_DATABUFF+8+10+8);
		for(i=5;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8+10,ADDR_DATABUFF+8+10+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//电流电压当前不平衡概率值
		Count*=5;//去5%大值
		Count/=100;
		if(Count>4)
		{
			Count=4;
		}
		MR(ADDR_DATABUFF+4,(ADDR_DATABUFF+8+8)-(Count*2),2);
		MR(ADDR_DATABUFF+6,(ADDR_DATABUFF+8+10+8)-(Count*2),2);
		//存储
		MW(ADDR_DATABUFF,ADDR_AFN0DF211_SOUR+(LEN_AFN0DF211_SOUR*Pn),LEN_AFN0DF211_SOUR);
	}

	Pn++;
	Terminal_Ram->I2SUBTask4_12=Pn;
#endif
}

void AFN0DF212_Computer(void)//终端统计.月三相电压、电流不平衡度95%概率值
{
#if (IC_SDRAM!=0)
	u32 i;
	u32 x;
	u32 Count;
	u32 Pn;
	u8 *p8;
	u16 *p16;
	
	Pn=Terminal_Ram->I2SUBTask4_13;//终端统计数据计算任务.月三相电压、电流不平衡度95%概率值
	if(Pn>=AFN0DF212MaxPn)
	{
		Pn=0;
	}

	//取原始值
	MR(ADDR_DATABUFF,ADDR_AFN0DF212_SOUR+(LEN_AFN0DF212_SOUR*Pn),LEN_AFN0DF212_SOUR);
	p8=(u8*)(ADDR_DATABUFF);
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=15;
	i*=15;
	i=hex_bcd(i);
	if(p8[0]!=i)//实时钟分;每0,15,30,45分计算每日最多取96点;每小时数据保存到FLASH
	{
		p8[0]=i;//上次计算的实时钟0,15,30,45分
		p16=(u16*)(ADDR_DATABUFF+2);
		Count=p16[0];
		Count++;//计算次数计数
		p16[0]=Count;
		//电流不平衡排序
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+10,2);//当前电流不平衡度(上电和初始化时清为0)
		p16=(u16*)(ADDR_DATABUFF+8+(149*2));
		for(i=150;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8,ADDR_DATABUFF+8+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//电压不平衡排序
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+12,2);//当前电压不平衡度(上电和初始化时清为0)
		p16=(u16*)(ADDR_DATABUFF+8+300+(149*2));
		for(i=150;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8+300,ADDR_DATABUFF+8+300+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//电流电压当前不平衡概率值
		Count*=5;//去5%大值
		Count/=100;
		if(Count>149)
		{
			Count=149;
		}
		MR(ADDR_DATABUFF+4,(ADDR_DATABUFF+8+(149*2))-(Count*2),2);
		MR(ADDR_DATABUFF+6,(ADDR_DATABUFF+8+300+(149*2))-(Count*2),2);

		MW(ADDR_DATABUFF,ADDR_AFN0DF212_SOUR+(LEN_AFN0DF212_SOUR*Pn),LEN_AFN0DF212_SOUR);
	}

	Pn++;
	Terminal_Ram->I2SUBTask4_13=Pn;
#endif
}

void HOURFLASH(void)//每小时RAM数据保存到FLASH
{
#if (IC_SDRAM!=0)
	u8 *p8;
	
	p8=(u8*)(ADDR_HOURFLASH_RAM_START);
	if(p8[0]!=Comm_Ram->TYMDHMS[2])
	{
		p8[0]=Comm_Ram->TYMDHMS[2];
		MW(ADDR_HOURFLASH_RAM_START,ADDR_HOURFLASH_START,LEN_HOURFLASH_RAM);
	}
#endif
}





void Terminal_Statistic(void)//终端统计计算,分任务运行
{

	switch(Terminal_Ram->I2SUBTask4)
	{
		case 0:
			AFN0DF25F33_Computer();//终端统计.日月总及分相最大有功功率及发生时间,有功功率为零时间
			break;
		case 1:
			AFN0DF27F35_Computer();//终端统计.电压统计数据
			break;
		case 2:
			AFN0DF29F37_Computer();//终端统计.电流统计数据
			break;
		case 3:
			AFN0DF28F36_Computer();//终端统计.电压电流不平衡
			break;
		case 4:
			AFN0DF30F38_Computer();//终端统计.视在功率越限累计时间
			break;
		case 5:
			AFN0DF31F39_Computer();//终端统计.负载率
			break;
		case 6:
			AFN0DF43F44_Computer();//终端统计.功率因数区段累计时间
			break;
		case 7:
		#if ERC22==1
			ERC22_Event();//有功总电能量差动越限计算和事件记录
		#endif
			break;
		case 8:
			AFN0DF57F60_Computer();//终端统计.日月总加组最大最小有功功率及其发生时间,有功功率为零累计时间		
			break;
		case 9:
			AFN0DF65F66_Computer();//终端统计.总加组超功率定值月电能量定值的月累计时间累计电能量
			break;
		case 10:
			AFN0DF121F122F123_Computer();//终端统计.谐波越限
			break;
		case 11:
			AFN0DF129F130_Computer();//终端统计.直流模拟量越限
			break;
		case 12:
			AFN0DF124F125F126_Computer();//终端统计.日冻结电压谐波电压含有率、谐波电流95%概率值
			break;
		case 13:
			AFN0DF211_Computer();//终端统计.日三相电压、电流不平衡度95%概率值
			break;
		case 14:
			AFN0DF212_Computer();//终端统计.月三相电压、电流不平衡度95%概率值
			break;
		case 15:
			HOURFLASH();//每小时RAM数据保存到FLASH
			break;
		case 16:
			AFN0DF26F34_Computer();//F26F34日月总及分相最大需量及发生时间
		default:
			Terminal_Ram->I2SUBTask4=0;
			return;
	}
	Terminal_Ram->I2SUBTask4++;
}

















