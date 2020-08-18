

//电能量控
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"






#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

u32 kWhContrl1_Comp(u32 Pn,u8* p8s)//终端控制-电控.月电控电能量比较,入口:Pn=总加组号(0-7),p8s=总加组月电控配置单元;返回0没超限,1报警,2超限
{
	u32 i;
//	u32 x;
	u64 a64;
	u64 b64;

	//取月电能量控定值
	a64=MRR((u32)p8s+6,8);//取月电量控定值    long64（单位：kWh，换算：-4）
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(a64>>63)
	{//负
		a64=(~a64)+1;
	}
	//取终端月电能量控定值浮动系数
	i=p8s[17];
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
	//当前定值
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	MWR(a64,ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn)+8,8);//写寄存器数据到存贮器,Byte<=8
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	//取当前月电量
	b64=MRR(ADDR_230x_9+(Pn*LENper_230x_9)+2+1,8);//总加月有功电量
	b64=R_Inverse(b64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(b64>>63)
	{//负
		b64=(~b64)+1;
	}
	if(b64>=a64)
	{//当前月电量>=定值
		return 2;//超限
	}
	//报警门限值
	a64=MRR((u32)p8s+6,8);//取月电量控定值    long64（单位：kWh，换算：-4）
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(a64>>63)
	{//负
		a64=(~a64)+1;
	}
	//取报警门限浮动系数
	i=p8s[15];
	//i&=0x7f;
	a64*=i;
	a64/=100;
	if(b64>=a64)
	{//当前月电量>=定值80%
		return 1;//报警
	}
	return 0;//没超限
}


void Terminal_Contrl_MonthEnergy(void)//终端控制-月电控
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
//	u64 a64;
//	u64 b64;
	u32 TAG;//参与控制总加组b0-b7表示总加组1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
//	u16* p16timer;
	
	TAG=0;//参与控制总加组b0-b7表示总加组1-8
#if (LENmax_8108_3+LENmax_8108_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);//月电控-总加组控制投入状态(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8108_3,ADDR_8108_2,LENmax_8108_2);//月电控配置单元
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}

//属性2（控制方案集）∷=array 月电控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//	总加组对象      OI，
//	月电量控定值    long64（单位：kWh，换算：-4），
//	报警门限值系数  unsigned（单位：%），
//	月电量控定值浮动系数  integer（单位：%）
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
		p8s=(u8*)ADDR_DATABUFF+LENmax_8108_3;
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
		
		//控制
		p8d=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//参与控制总加组b0-b7表示总加组1-8
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				break;
			case 3://控制
				i=kWhContrl1_Comp(Pn,p8s);//终端控制-电控.月电控电能量比较,入口:Pn=总加组号(0-7),p8s=总加组月电控配置单元;返回0没超限,1报警,2超限
				switch(i)
				{
					case 1:
						//电控告警事件
						//入口:pOccurSour=事件发生源    OI
						//OI=控制对象      OI
						//pFixedValue=电控定值 long64（单位：kWh，换算-4）
						Event_32030200(0x8108,0x2300+(Pn+1),p8d+8);//电控告警事件记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						//重读入数据
						MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);//
						MR(ADDR_DATABUFF+LENmax_8108_3,ADDR_8108_2,LENmax_8108_2);//
						p8d[0]=100;//报警
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 100://报警
				i=kWhContrl1_Comp(Pn,p8s);//终端控制-电控.月电控电能量比较,入口:Pn=总加组号(0-7),p8s=总加组月电控配置单元;返回0没超限,1报警,2超限
				switch(i)
				{
					case 0:
						p8d[0]=3;
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 101://跳闸准备
				//跳闸轮次
				x=0;//最大全部轮次
				for(i=0;i<NUMmax_CR;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_230x_15+(LENper_230x_15*Pn)+2,1);//总加组电控轮次配置
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2]=i;//当前电控跳闸轮次B0-B7
				i&=x;
				if(i!=0x0)
				{//有轮次跳闸
					p8d[1]=i;
					//电控跳闸事件
					//入口:pOccurSour=事件发生源    OI
					//OI=控制对象      OI
					//bit=跳闸轮次      bit-string(SIZE(8))
					//pFixedValue=电控定值 long64（单位：kWh，换算-4）
					//pFirst=跳闸发生时总加电能量  long64（单位：kwh/元，换算-4）
				//当事件发生源类型为月电控8108，跳闸时总加电能量为总加月电能量；
				//当事件发生源类型为购电控8107，跳闸时总加电能量为剩余电能量/费。
					i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					Event_32010200(0x8108,0x2300+(Pn+1),i,p8d+8,(u8*)ADDR_230x_9+(Pn*LENper_230x_9)+2+1);//电控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					//重读入数据
					MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);//
					MR(ADDR_DATABUFF+LENmax_8108_3,ADDR_8108_2,LENmax_8108_2);//
				}
				p8d[0]=102;
				break;
			case 102://跳闸
				i=kWhContrl1_Comp(Pn,p8s);//终端控制-电控.月电控电能量比较,入口:Pn=总加组号(0-7),p8s=总加组月电控配置单元;返回0没超限,1报警,2超限
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//清轮次跳闸
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
			p8d=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}
}



u32 kWhContrl2_Comp(u32 Pn,u8* p8s)//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7),p8s=总加组购电控配置单元;返回0没超限,1报警,2超限
{
	s64 a64;
	s64 b64;

	//剩余电量或电费
	a64=MRR(ADDR_230x_11+(Pn*LENper_230x_11)+1,8);//总加剩余电量（费）
	a64=R_Inverse(a64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	//跳闸门限值
	b64=MRR((u32)p8s+23+9+1,8);
	b64=R_Inverse(b64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(a64<=b64)
	{//剩余电量或电费<=跳闸门限值
		return 2;//超限
	}
	//报警门限值
	b64=MRR((u32)p8s+23+1,8);
	b64=R_Inverse(b64,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(a64<=b64)
	{//剩余电量或电费<=报警门限值
		return 1;//报警
	}
	return 0;//没超限
}


void Terminal_Contrl_BuyEnergy(void)//终端控制-购电控
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
//	u64 a64;
//	u64 b64;
	u32 TAG;//参与控制总加组b0-b7表示总加组1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
//	u16* p16timer;
	
	TAG=0;//参与控制总加组b0-b7表示总加组1-8
#if (LENmax_8107_3+LENmax_8107_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);//购电控-总加组控制投入状态(初始化值0x00)
	MR(ADDR_DATABUFF+LENmax_8107_3,ADDR_8107_2,LENmax_8107_2);//购电控配置单元
//控制投入状态∷=array 一个总加组控制投入状态
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}

//属性2（控制方案集）∷=array 购电控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//	总加组对象      OI，
//购电单号        double-long-unsigned，
//追加/刷新标识   enum{追加（0），刷新（1）}，
//购电类型        enum{电量（0），电费（1）}，
//购电量（费）值  long64（单位：kWh/元， 换算：-4），
//	报警门限值      long64（单位：kWh/元，换算：-4），
//	跳闸门限值      long64（单位：kWh/元，换算：-4）
//	购电控模式      enum{本地模式（0），远程模式（1）}
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
//		p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
//		if(p8[1]!=0x0)
//		{//保电
//			break;
//		}
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
		p8s=(u8*)ADDR_DATABUFF+LENmax_8107_3;
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
		
		//控制
		p8d=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		p8d[7]=p8s[13];//购电类别 0=购电量,1=购电费
		MR((u32)p8d+8,(u32)p8s+23+1,8);//报警门限值
		TAG|=1<<Pn;//参与控制总加组b0-b7表示总加组1-8
		switch(p8d[0])
		{
			case 0://上电或不控制
				p8d[0]=3;
				break;
			case 3://控制
				p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
				if(p8[1]!=0x0)
				{//保电
					break;
				}
				i=kWhContrl2_Comp(Pn,p8s);//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7),p8s=总加组购电控配置单元;返回0没超限,1报警,2超限
				switch(i)
				{
					case 1:
						//电控告警事件
						//入口:pOccurSour=事件发生源    OI
						//OI=控制对象      OI
						//pFixedValue=电控定值 long64（单位：kWh，换算-4）
						Event_32030200(0x8107,0x2300+(Pn+1),p8d+8);//电控告警事件记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						//重读入数据
						MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);//购电控-总加组控制投入状态(初始化值0x00)
						MR(ADDR_DATABUFF+LENmax_8107_3,ADDR_8107_2,LENmax_8107_2);//购电控配置单元
						p8d[0]=100;//报警
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 100://报警
				p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
				if(p8[1]!=0x0)
				{//保电
					p8d[0]=3;
					break;
				}
				i=kWhContrl2_Comp(Pn,p8s);//终端控制-电控.购电控电量费比较,入口Pn=总加组号(0-7),p8s=总加组购电控配置单元;返回0没超限,1报警,2超限
				switch(i)
				{
					case 0:
						p8d[0]=3;
						break;
					case 2:
						p8d[0]=101;//跳闸准备
						break;
				}
				break;
			case 101://跳闸准备
				//跳闸轮次
				x=0;//最大全部轮次
				for(i=0;i<NUMmax_CR;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_230x_15+(LENper_230x_15*Pn)+2,1);//总加组电控轮次配置
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2]=i;//当前电控跳闸轮次B0-B7
				i&=x;
				if(i!=0x0)
				{//有轮次跳闸
					p8d[1]=i;
					//电控跳闸事件
					//入口:pOccurSour=事件发生源    OI
					//OI=控制对象      OI
					//bit=跳闸轮次      bit-string(SIZE(8))
					//pFixedValue=电控定值 long64（单位：kWh，换算-4）
					//pFirst=跳闸发生时总加电能量  long64（单位：kwh/元，换算-4）
				//当事件发生源类型为月电控8108，跳闸时总加电能量为总加月电能量；
				//当事件发生源类型为购电控8107，跳闸时总加电能量为剩余电能量/费。
					i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					Event_32010200(0x8107,0x2300+(Pn+1),i,p8d+8,(u8*)ADDR_230x_11+(Pn*LENper_230x_11)+1);//电控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					//重读入数据
					MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);//购电控-总加组控制投入状态(初始化值0x00)
					MR(ADDR_DATABUFF+LENmax_8107_3,ADDR_8107_2,LENmax_8107_2);//购电控配置单元
				}
				p8d[0]=102;
				break;
			case 102://跳闸
				p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
				if(p8[1]!=0x0)
				{//保电
					p8d[0]=3;
					p8d[1]=0;//清轮次跳闸
					break;
				}
				i=kWhContrl2_Comp(Pn,p8s);//终端控制-电控.月电控电能量比较,入口:Pn=总加组号(0-7),p8s=总加组月电控配置单元;返回0没超限,1报警,2超限
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//清轮次跳闸
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
			p8d=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}

}

#endif

