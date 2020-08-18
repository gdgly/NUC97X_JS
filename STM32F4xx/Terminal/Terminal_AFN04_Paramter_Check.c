
#include "Project.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


void PnVSAFN04F10SetNoList(void)//测量点号Pn对应AFN04F10配置序号列表
{
	u32 Pn;
	u32 Meter;
	u8 *p8;
	u16 *p16;
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//测量点号Pn对应AFN04F10配置序号
	for(Meter=0;Meter<=AFN04F10MaxPn;Meter++)
	{
		p16[Meter]=0;//清列表
	}
	p8=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		Pn=p8[0]+(p8[1]<<8);
		if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
		{
			p16[Pn]=Meter;
		}
		p8+=LEN_AFN04F10_Pn;
	}
}

u32 Check_AFN04F10_Pn(u32 Pn)//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
{
	u16 *p16;
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//测量点号Pn对应AFN04F10配置序号
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return 0;
	}
	return p16[Pn];
/*
#if IC_SST39VF6401B==0//0=没,1=有
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F10+2;
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=MRR(Addr,2);
		if(i==Pn)
		{
			return Meter;
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return 0;
#else
	u32 i;
	u32 Meter;
	u8 *p8;
	if(Pn==0)
	{
		return 0;
	}
	p8=(u8*)(ADDR_AFN04F10+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[0]+(p8[1]<<8);
		if(i==Pn)
		{
			return Meter;
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
#endif
*/
}

u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
{
	u32 i;
	u32 Meter;
	u64 d64;
	u8 *p8;
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p8=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[0]+(p8[1]<<8);
		if(i!=0)
		{//测量点号有效
			i=p8[2];
			i&=0x1f;
			if(i==port)
			{//端口号相同
				d64=(p8[4+5]<<24)+(p8[4+4]<<16)+(p8[4+3]<<8)+p8[4+2];
				d64<<=16;
				d64+=(p8[4+1]<<8)+p8[4];
				if(d64==CommAddr)
				{//地址相同
					return Meter;//相同的配置序号
				}
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
}

u32 Check_AFN04F11_Pn(u32 Pn)//检查终端脉冲配置参数指定的测量点号;返回0无,1有
{
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F11+1;
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		i=MRR(Addr,1);
		if(i==Pn)
		{
			return 1;
		}
		Addr+=5;
	}
	return 0;
}

u32 Check_AFN04F13_Pn(u32 Pn)//检查终端电压/电流模拟量配置参数指定的测量点号;返回0无,1有
{
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F13+1;
	for(Meter=1;Meter<=AFN04F13MaxPn;Meter++)
	{
		i=MRR(Addr,1);
		if(i==Pn)
		{
			return 1;
		}
		Addr+=3;
	}
	return 0;
}

void AFN04F150_Computer(void)//测量点有效标志计算
{
	u32 i;
	u32 Meter;
	u8 *p8;
	u8 *p8s;
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	u32 Addr;
#endif
	
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p8=(u8 *)(ADDR_DATABUFF);
	MC(0,ADDR_DATABUFF,256);
	//终端电能表／交流采样装置配置参数
	/*
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//测量点号Pn对应AFN04F10配置序号,0为无效
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=Meter;
		if(p16[i]!=0)
		{
	#if AFN04F150Dn==0//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
			i--;
	#endif
			p8[i/8]|=(1<<(i%8));
		}
	}
	*/
	p8s=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8s[0]+(p8s[1]<<8);
		if((i!=0)&&(i<(256*8)))
		{
	#if AFN04F150Dn==0//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
			i--;
	#endif
			p8[i/8]|=(1<<(i%8));
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	//终端脉冲配置参数
	Addr=ADDR_AFN04F11+1;
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		i=MRR(Addr,1);
		if(i!=0)
		{
	#if AFN04F150Dn==0//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
			i--;
	#endif
			p8[i/8]|=(1<<(i%8));
		}
		Addr+=5;
	}
#endif
	MW(ADDR_DATABUFF,ADDR_AFN04F150,256);
}

u32 PnSet(u32 Pn)//有效测量点号检查;返回0=无效,1=有效
{
	u32 i;
	u32 x;
	
	if((Pn==0)||(Pn>=(256*8)))
	{
		return 0;
	}
#if AFN04F150Dn==0//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
	Pn-=1;
#endif
	i=Pn/8;
	x=MRR(ADDR_AFN04F150+i,1);
	i=Pn%8;
	x>>=i;
	x&=1;
	return x;
}

u32 Check_ACSamplePn(u32 Pn)//有效交采测量点号检查;返回0=无效,1=有效
{
	u32 i;
	u16 *p16;
	
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//测量点号Pn对应AFN04F10配置序号
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return 0;
	}
	i=p16[Pn];
	if(i!=0)
	{
		i--;
		i=(ADDR_AFN04F10+(LEN_AFN04F10_Pn*i));
		i=MRR(i+4,2);//通信速率及端口号
		if((i&0x1f)==1)
		{//端口号1
			return 1;
		}
		if((i>>8)==2)//通信协议类型
		{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
			return 1;
		}
	}
	return 0;
}

u32 ACSampleVoltageCurvePn(u32 Fn,u32 Pn)//有效交采电压曲线测量点号检查;返回0=无效,1=有效
{
	if((Fn<89)||(Fn>91))
	{
		return 0;
	}
	Fn=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	return Fn;
}

u32 GroupTotalSet(u32 Pn)//有效总加组号检查;返回0=无效,1=有效
{
	if((Pn==0)||(Pn>AFN04F14MaxPn))
	{
		return 0;
	}
	Pn-=1;
	Pn=ADDR_AFN04F14+1+(LEN_AFN04F14_Pn*Pn);
	Pn=MRR(Pn,1);
	if(Pn!=0)
	{
		Pn=1;
	}
	return Pn;
}

void Terminal_AFN0CF3_Computer(u32 Fn)//终端类1数据F3,终端参数状态
{
	u32 i;
	u32 x;

	if(Fn!=0x0)
	{
		if(Fn<=248)
		{
			Fn-=1;
			i=Fn/8;
			x=MRR(ADDR_AFN0CF3+i,1);
			x|=1<<(Fn%8);
			MWR(x,ADDR_AFN0CF3+i,1);
		}
	}
}

void PnVSProtocolList(u32 ADDR_BUFF)//测量点号Pn对应AFN04F10配置通信协议列表,无配置且非脉冲口通信协议为30(DL645-2007)
{
	u32 Pn;
	u32 Meter;
	u8 *p8s;
	u8 *p8d;

#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	//清列表
	p8d=(u8*)(ADDR_BUFF);//测量点号Pn对应AFN04F10配置列表
	for(Pn=1;Pn<=MaxRS485AddCarrierPn;Pn++)
	{
		p8d[Pn]=30;//通信协议类型:1=DL/T645-1997,30=DL645-2007
	}
	p8s=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		Pn=p8s[0]+(p8s[1]<<8);
		if(Pn<=MaxRS485AddCarrierPn)
		{//测量点号有效
			p8d[Pn]=p8s[3];//取通信协议
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	//脉冲口通信协议为0
	p8s=(u8*)(ADDR_AFN04F11+1);
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		//Pn=p8s[0];
		Pn=MRR((u32)p8s,1);
		if(Pn<=MaxRS485AddCarrierPn)
		{//测量点号有效
			p8d[Pn]=0;//端口号和通信协议
		}
		p8s+=5;
	}
}

void PnVSPORTProtocolList(u32 ADDR_BUFF)//测量点号Pn对应AFN04F10配置端口号和通信协议列表,无配置且非脉冲口默认端口号0通信协议为30(DL645-2007)
{
	u32 Pn;
	u32 Meter;
	u8 *p8s;
	u16 *p16;
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	//清列表
	p16=(u16*)(ADDR_BUFF);//测量点号Pn对应AFN04F10配置列表
	for(Pn=1;Pn<=MaxRS485AddCarrierPn;Pn++)
	{
		p16[Pn]=0+(30<<8);//端口号和通信协议类型:1=DL/T645-1997,30=DL645-2007
	}
	p8s=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		Pn=p8s[0]+(p8s[1]<<8);
		if(Pn<=MaxRS485AddCarrierPn)
		{//测量点号有效
			p16[Pn]=(p8s[2]&0x1f)+(p8s[3]<<8);//取端口号和通信协议
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	//脉冲口通信协议为0
	p8s=(u8*)(ADDR_AFN04F11+1);
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		//Pn=p8s[0];
		Pn=MRR((u32)p8s,1);
		if(Pn<=MaxRS485AddCarrierPn)
		{//测量点号有效
			p16[Pn]=0;//端口号和通信协议
		}
		p8s+=5;
	}
}

u32 Get_SetTariff(u32 Pn)//取AFN04F10中配置的费率数
{
	u32 i;
	i=Check_AFN04F10_Pn(Pn);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
	if(i!=0)
	{
		i--;
		i=MRR(ADDR_AFN04F10+(i*LEN_AFN04F10_Pn)+18,1);
	}
	else
	{//脉冲测量点用终端04参数设置的费率数(费率数：取值范围1～12，其他无效)
		i=Check_AFN04F11_Pn(Pn);//检查终端脉冲配置参数指定的测量点号;返回0无,1有
		if(i!=0)
		{
			i=MRR(ADDR_AFN04F21+48,1);
			if(i==0)
			{
				i=1;
			}
			else
			{
				if(i>12)
				{
					i=12;
				}
			}
		}
		else
		{
			return 4;//没找到
		}
	}
	if(i>TMaxTariff)
	{
		i=TMaxTariff;
	}
	return i;
}


u32 F246MapPn(u32 Pn)//F246掉电记录数据映射Pn;返回0表示无效
{
	u32 i;
	u16 *p16;
	p16=(u16 *)(ADDR_F246PnList);//要抄读的掉电记录数据Pn列表,半字对齐
	for(i=0;i<F246MaxPn;i++)
	{
		if(p16[i]==Pn)
		{
			return (i+1);
		}
	}
	return 0;
}

void F246PnList(void)//要抄读的F246掉电记录数据Pn列表
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Pn;
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	u16 *pn16;
	u32 YMD;
	
	p16=(u16 *)(ADDR_F246PnList);//要抄读的掉电记录数据Pn列表,半字对齐
	for(i=0;i<F246MaxPn;i++)
	{//清列表
		p16[i]=0;
	}
	i=MRR(ADDR_AFN04F97,1);//停电数据采集配置参数
	if((i&1)==0)
	{//停电数据采集标志D0：置"1"有效，置"0"无效
		return;
	}
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	if((i&2)==0)
	{//D1：置"0"只采集设置的测量点
		y=MRR(ADDR_AFN04F97+3,2);//需要读取停电事件电能表个数n	BIN	2	
		if(y>F246MaxPn)
		{
			y=F246MaxPn;
		}
		MR(ADDR_F246PnList,ADDR_AFN04F97+5,y*2);
		//选1只公变考核点
		if(y<F246MaxPn)
		{
			p8=(u8*)(ADDR_128KDATABUFF);
			for(i=0;i<AFN04F10MaxPn;i++)
			{
				Pn=p8[2]+(p8[3]<<8);
				if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
				{
					x=p8[4]&0x1f;
					if((x==2)||(x==3)||(x==31))
					{//是端口2,3或31
						x=p8[26];//用户大类号及用户小类号
					#if (USER/100)==11//吉林
						if(((x>>4)<=2)||(x==0x42))//吉林公变考核点为大类号5内部交采,故用任意三相表代
					#else
						x>>=4;
						if(x==6)
					#endif
						{//
							for(z=0;z<y;z++)
							{
								if(Pn==p16[z])
								{
									break;;//原已加入
								}
							}
							if(z==y)
							{
								p16[y]=Pn;
								y++;
								break;
							}
						}
					}
				}
				p8+=LEN_AFN04F10_Pn;
			}
		}
	}
	else
	{//D1：置"1"随机选择测量点
		y=0;//已选表数
		//大类号1最大小类号(1-16),大型专变用户(A类)
		//大类号2最大小类号(1-16),中小型专变用户(B类)
		//大类号3最大小类号(1-16),三相一般工商业用户(C类)
		//大类号4最大小类号(1-16),单相一般工商业用户(D类)
		//大类号5最大小类号(1-16),居民用户(E类)
		//大类号6最大小类号(1-16),公用考核计量点(F类)
		//先选1只公变考核点
		p8=(u8*)(ADDR_128KDATABUFF);
		for(i=0;i<AFN04F10MaxPn;i++)
		{
			Pn=p8[2]+(p8[3]<<8);
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				x=p8[4]&0x1f;
			#if (USER/100)==0//电科院测试,载波口无启抄
				if((x==2)||(x==3))
			#else
				if((x==2)||(x==3)||(x==31))
			#endif
				{//是端口2,3或31
					x=p8[26];//用户大类号及用户小类号
				#if (USER/100)==11//吉林
					if(((x>>4)<=2)||(x==0x42))//吉林公变考核点为大类号5内部交采,故用任意三相表代
				#else
					x>>=4;
					if(x==6)
				#endif
					{//
						p16[y]=Pn;
						y++;
						break;
					}
				}
			}
			p8+=LEN_AFN04F10_Pn;
		}
		//再选上日冻结正常抄表的三相表
		YMD=MRR(ADDR_TYMDHMS+3,3);
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		i=GetDateCongealAddr(YMD,161,1);//得到日冻结存储地址,返回=0表示没找到
		if(i!=0)
		{
			YMD=YMD_Add1D(YMD);//年月日加1日,返回加1后的年月日
			YMD&=0xffff;
			p8=(u8*)i;
			pn16=(u16*)(ADDR_PnVSAFN04F10SetNo);//测量点号Pn对应AFN04F10配置序号,0为无效
			for(Pn=1;Pn<=MaxRS485AddCarrierPn;Pn++)
			{
				x=p8[2]+(p8[3]<<8);//原抄表月日
				if(x==YMD)
				{//本日抄表正常
					x=pn16[Pn];
					if((x!=0)&&(x<=MaxRS485AddCarrierPn))
					{
						x--;
						p8d=(u8*)(ADDR_128KDATABUFF+(x*LEN_AFN04F10_Pn));
						x=p8d[4];
						x&=0x1f;
					#if (USER/100)==0//电科院测试,载波口无启抄
						if((x==2)||(x==3))
					#else
						if((x==2)||(x==3)||(x==31))
					#endif
						{//是端口2,3或31
							x=p8d[26];//用户大类号及用户小类号
						#if (USER/100)==11//吉林
							if(((x>>4)<=2)||(x==0x42))//吉林公变考核点为大类号5内部交采,故用任意三相表代
						#else
							x>>=4;
							if((x!=4)&&(x!=6))
						#endif
							{//
								for(z=0;z<y;z++)
								{
									if(Pn==p16[z])
									{
										break;;//原已加入
									}
								}
								if(z==y)
								{
									p16[y]=Pn;
									y++;
									x=MRR(ADDR_AFN04F97+3,2);//需要读取停电事件电能表个数n	BIN	2
									x++;//需要读取停电事件电能表个数n为除台区考核表外采集停电事件的有效测量点个数，即共采集n+1个电能表停电事件
									if(x>F246MaxPn)
									{
										x=F246MaxPn;
									}
									if(y>=x)
									{
										return;
									}
								}
							}
						}
					}
				}
				p8+=6+(5*(1+TMaxTariff));
			}
		}
		//选任意三相表
		p8=(u8*)(ADDR_128KDATABUFF);
		for(i=0;i<AFN04F10MaxPn;i++)
		{
			Pn=p8[2]+(p8[3]<<8);
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				x=p8[4]&0x1f;
			#if (USER/100)==0//电科院测试,载波口无启抄
				if((x==2)||(x==3))
			#else
				if((x==2)||(x==3)||(x==31))
			#endif
				{//是端口2,3或31
					x=p8[26];//用户大类号及用户小类号
				#if (USER/100)==11//吉林
					if(((x>>4)<=2)||(x==0x42))//吉林公变考核点为大类号5内部交采,故用任意三相表代
				#else
					x>>=4;
					if((x!=4)&&(x!=6))
				#endif
					{//
						for(z=0;z<y;z++)
						{
							if(Pn==p16[z])
							{
								break;//原已加入
							}
						}
						if(z==y)
						{
							p16[y]=Pn;
							y++;
							x=MRR(ADDR_AFN04F97+3,2);//需要读取停电事件电能表个数n	BIN	2
							x++;//需要读取停电事件电能表个数n为除台区考核表外采集停电事件的有效测量点个数，即共采集n+1个电能表停电事件
							if(x>F246MaxPn)
							{
								x=F246MaxPn;
							}
							if(y>=x)
							{
								return;
							}
						}
					}
				}
			}
			p8+=LEN_AFN04F10_Pn;
		}
		//最后任意表
		p8=(u8*)(ADDR_128KDATABUFF);
		for(i=0;i<AFN04F10MaxPn;i++)
		{
			Pn=p8[2]+(p8[3]<<8);
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				x=p8[4]&0x1f;
			#if (USER/100)==0//电科院测试,载波口无启抄
				if((x==2)||(x==3))
			#else
				if((x==2)||(x==3)||(x==31))
			#endif
				{//是端口2,3或31
					for(z=0;z<y;z++)
					{
						if(Pn==p16[z])
						{
							break;//原已加入
						}
					}
					if(z==y)
					{
						p16[y]=Pn;
						y++;
						x=MRR(ADDR_AFN04F97+3,2);//需要读取停电事件电能表个数n	BIN	2
						x++;//需要读取停电事件电能表个数n为除台区考核表外采集停电事件的有效测量点个数，即共采集n+1个电能表停电事件
						if(x>F246MaxPn)
						{
							x=F246MaxPn;
						}
						if(y>=x)
						{
							return;
						}
					}
				}
			}
			p8+=LEN_AFN04F10_Pn;
		}
	}
}


const u8 CurveSetFnList[]=//2类数据曲线冻结密度参数Fn列表(同步ADDR_AFN04F104）
{
	73,//1 AFN0DF73 总加组有功功率曲线
	74,//2 AFN0DF74 总加组无功功率曲线
	75,//3 AFN0DF75 总加组有功电能量曲线
	76,//4 AFN0DF76 总加组无功电能量曲线
	81,//5 AFN0DF81 测量点有功功率曲线
	82,//6 AFN0DF82 测量点A相有功功率曲线
	83,//7 AFN0DF83 测量点B相有功功率曲线
	84,//8 AFN0DF84 测量点C相有功功率曲线
	85,//9 AFN0DF85 测量点无功功率曲线
	86,//10 AFN0DF86 测量点A无功功率曲线
	87,//11 AFN0DF87 测量点B无功功率曲线
	88,//12 AFN0DF88 测量点C无功功率曲线
	89,//13 AFN0DF89 测量点A相电压曲线
	90,//14 AFN0DF90 测量点B相电压曲线
	91,//15 AFN0DF91 测量点C相电压曲线
	92,//16 AFN0DF92 测量点A相电流曲线
	93,//17 AFN0DF93 测量点B相电流曲线
	94,//18 AFN0DF94 测量点C相电流曲线
	95,//19 AFN0DF95 测量点零序电流曲线
	97,//20 AFN0DF97 测量点正向有功总电能量曲线
	98,//21 AFN0DF98 测量点正向无功总电能量曲线
	99,//22 AFN0DF99 测量点反向有功总电能量曲线
	100,//23 AFN0DF100 测量点反向无功总电能量曲线
	101,//24 AFN0DF101 测量点正向有功总电能示值
	102,//25 AFN0DF102 测量点正向无功总电能示值
	103,//26 AFN0DF103 测量点反向有功总电能示值
	104,//27 AFN0DF104 测量点反向无功总电能示值
	105,//28 AFN0DF105 测量点总功率因数
	106,//29 AFN0DF106 测量点A相功率因数
	107,//30 AFN0DF107 测量点B相功率因数
	108,//31 AFN0DF108 测量点C相功率因数
	109,//32 AFN0DF109 测量点电压相位角曲线
	110,//33 AFN0DF110 测量点电流相位角曲线
	138,//34 AFN0DF138 直流模拟量数据曲线
	145,//35 AFN0DF145 测量点1象限无功总电能示值曲线
	146,//36 AFN0DF146 测量点4象限无功总电能示值曲线
	147,//37 AFN0DF147 测量点2象限无功总电能示值曲线
	148,//38 AFN0DF148 测量点3象限无功总电能示值曲线

#if LEN_AFN04F104_PerPn!=38
	#error "LEN_AFN04F104_PerPn!=38"
#endif
};

u32 Get_CurveSetFnListNo(u32 Fn)//得到曲线冻结密度参数Fn列表号;返回:列表号0-n,0xffffffff表示无
{
	u32 i;
	
	for(i=0;i<sizeof(CurveSetFnList);i++)
	{
		if(CurveSetFnList[i]==Fn)
		{
			return i;
		}
	}
	return 0xffffffff;
}



