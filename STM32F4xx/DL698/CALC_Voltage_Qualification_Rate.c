

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_Voltage_Qualification_Rate.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_GET_4100.h"


void CALC_Voltage_Qualification_Rate(void)//电压合格率计算
{
	u32 i;
	u32 a;
	u32 b;
	u32 c;
	u32 f;//当前越限标志
	u32 t;
	u32 n;
	u8 *p8;
	u16 *p16;
	u32 limitH;//电压考核上限
	u32 limitL;//电压考核下限
	u32 standardH;//电压合格上限
	u32 standardL;//电压合格下限

	p8=(u8*)ADDR_213x_REAL;//当月相电压合格率
	t=Comm_Ram->DL698YMDHMS[6];
	i=p8[0];//上次计算RTC秒
	if(t==i)
	{
		return;
	}
	p8[0]=t;//上次计算RTC秒
	a=p8[1];//hex上电秒延时定时
	if(a<5)//5秒延时
	{
		a++;
		p8[1]=a;
		return;
	}
	if(t<i)
	{
		t+=60;
	}
	t-=i;//秒值
	t+=p8[2];//秒计数
	n=p8[3];//电压加平均计数
	n++;
	if(t<60)
	{
		p8[2]=t;
		p8[3]=n;//电压加平均计数
	}
	else
	{
		p8[2]=0;//秒计数
		p8[3]=0;//电压加平均计数
//#define ADDR_4030     ADDR_4024+LENmax_4024//电压合格率参数
////属性2∷=structure
////{
////	电压考核上限  long-unsigned（单位：V，换算：-1），
////  电压考核下限  long-unsigned（单位：V，换算：-1），
////	电压合格上限  long-unsigned（单位：V，换算：-1），
////  电压合格下限  long-unsigned（单位：V，换算：-1）
////}
//#define LENmax_4030     14
	#if MeterSpec==20//自动适应电能表规格
		switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
		{
			case 0:
				limitH=2640;//电压考核上限
				limitL=1540;//电压考核下限
				standardH=2354;//电压合格上限
				standardL=2046;//电压合格下限
				break;
			case 1:
				limitH=1200;//电压考核上限
				limitL=780;//电压考核下限
				standardH=1070;//电压合格上限
				standardL=900;//电压合格下限
				break;
			default:
				limitH=692;//电压考核上限
				limitL=450;//电压考核下限
				standardH=617;//电压合格上限
				standardL=519;//电压合格下限
				break;
		}
	#else
		MR(ADDR_DATABUFF,ADDR_4030,LENmax_4030);
		p8=(u8*)ADDR_DATABUFF;
		limitH=(p8[3]<<8)|p8[4];//电压考核上限
		limitL=(p8[6]<<8)|p8[7];//电压考核下限
		standardH=(p8[9]<<8)|p8[10];//电压合格上限
		standardL=(p8[12]<<8)|p8[13];//电压合格下限
	#endif
	}
	
	for(i=0;i<3;i++)
	{
//初始化数据结构
		p8=(u8*)ADDR_2131+(i*LENmax_2131);
//属性2（电压合格率数据）∷=structure
		p8[0]=DataType_structure;
		p8[1]=2;
//{
//  当日电压合格率         电压合格率，
//电压合格率∷=structure
		p8[2]=DataType_structure;
		p8[3]=5;
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
		p8[4]=DataType_double_long_unsigned;
//电压合格率   long-unsigned（单位：%，换算：-2），
		p8[9]=DataType_long_unsigned;
//电压超限率   long-unsigned（单位：%，换算：-2），
		p8[12]=DataType_long_unsigned;
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
		p8[15]=DataType_double_long_unsigned;
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
		p8[20]=DataType_double_long_unsigned;
//}
//  当月电压合格率         电压合格率
//电压合格率∷=structure
		p8[25]=DataType_structure;
		p8[26]=5;
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
		p8[27]=DataType_double_long_unsigned;
//电压合格率   long-unsigned（单位：%，换算：-2），
		p8[32]=DataType_long_unsigned;
//电压超限率   long-unsigned（单位：%，换算：-2），
		p8[35]=DataType_long_unsigned;
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
		p8[38]=DataType_double_long_unsigned;
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
		p8[43]=DataType_double_long_unsigned;
		
		p16=(u16*)(ADDR_UA+2+(i*4));//取交采电压
		a=p16[0];
		a=bcd_hex(a);
		p16=(u16*)(ADDR_213x_REAL+4+(i*6));
		b=p16[0]|(p16[1]<<16);
		a+=b;//电压加
		if(t<60)
		{//没到60秒
			p16[0]=a;
			p16[1]=a>>16;
			f=3;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
			//continue;
		}
		else
		{//到60秒
			p16[0]=0;
			p16[1]=0;
			if((Meter_Ram->PW==1)&&(i==1))//初始接线方式;0=三相四线,1=三相三线
			{//三相三线B相
				f=3;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
			}
			else
			{
				a/=n;
				if(a>=limitH)//电压考核上限
				{
					f=1;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
				}
				else
				{
					if(a>=standardH)//电压合格上限
					{
						f=2;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
					}
					else
					{
						if(a<=limitL)//电压考核下限
						{
							f=5;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
						}
						else
						{
							if(a<=standardL)//电压合格下限
							{
								f=4;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
							}
							else
							{
								f=3;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
							}
						}
					}
				}
			}
		}
/*
		//当日电压监测时间
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+3,4);
		a=R_Inverse(a,4);
		a++;
		a=R_Inverse(a,4);
		MWR(a,ADDR_2131+(i*LENmax_2131)+2+3,4);
		//当月电压监测时间
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+23+3,4);
		a=R_Inverse(a,4);
		a++;
		a=R_Inverse(a,4);
		MWR(a,ADDR_2131+(i*LENmax_2131)+2+23+3,4);
*/
		a=Comm_Ram->DL698YMDHMS[4];//时
		a*=60;
		a+=Comm_Ram->DL698YMDHMS[5];//分
		a++;//多1分
		//当日电压监测时间
		b=R_Inverse(a,4);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+3,4);
		//当月电压监测时间
		b=Comm_Ram->DL698YMDHMS[3];//日
		if(b)
		{
			b--;
		}
		b*=24*60;
		a+=b;
		b=R_Inverse(a,4);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+23+3,4);
		
		switch(f)
		{
			case 1://1=越考核上限
			case 2://2=越合格上限,
				//当日电压超上限时间
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+14,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+14,4);
				//当月电压超上限时间
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+23+14,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+23+14,4);
				break;
			case 3://3=合格
				break;
			case 4://4=越合格下限,
			case 5://5=越考核下限
				//当日电压超下限时间
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+19,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+19,4);
				//当月电压超下限时间
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+23+19,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+23+19,4);
				break;
		}
		//当日电压超限率
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+3,4);//电压监测时间
		b=MRR(ADDR_2131+(i*LENmax_2131)+2+14,4);//电压超上限时间
		c=MRR(ADDR_2131+(i*LENmax_2131)+2+19,4);//电压超下限时间
		a=R_Inverse(a,4);
		b=R_Inverse(b,4);
		c=R_Inverse(c,4);
		b+=c;
		if(b>a)
		{
			b=a;
		}
		b*=10000;//%二位小数
		c=b/a;//超限率
		b=10000-c;//合格率
		b=R_Inverse(b,2);
		c=R_Inverse(c,2);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+8,2);//电压合格率
		MWR(c,ADDR_2131+(i*LENmax_2131)+2+11,2);//电压超限率
		//当月电压合格率
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+23+3,4);//电压监测时间
		b=MRR(ADDR_2131+(i*LENmax_2131)+2+23+14,4);//电压超上限时间
		c=MRR(ADDR_2131+(i*LENmax_2131)+2+23+19,4);//电压超下限时间
		a=R_Inverse(a,4);
		b=R_Inverse(b,4);
		c=R_Inverse(c,4);
		b+=c;
		if(b>a)
		{
			b=a;
		}
		b*=10000;//%二位小数
		c=b/a;//超限率
		b=10000-c;//合格率
		b=R_Inverse(b,2);
		c=R_Inverse(c,2);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+23+8,2);//电压合格率
		MWR(c,ADDR_2131+(i*LENmax_2131)+2+23+11,2);//电压超限率
		//事件
		p8=(u8*)ADDR_213x_REAL+4+(i*6);//当月相电压合格率
		if(f!=p8[5])
		{
			
			p8[5]=f;//越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
		}
	}
}


















