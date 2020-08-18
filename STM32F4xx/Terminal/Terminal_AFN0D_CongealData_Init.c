
#include "Project.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_SourceData.h"



void Init_Class2Data(u32 Pn)//类2数据初始化(指定的测量点数据,Pn=1~n)
{
#if (USER/100)==3//用户标识:福建版
	u8 *p8;
#endif

	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
#if (USER/100)==3//用户标识:福建版
	Init_Class2SourceData(Pn);//类2原始数据初始化(指定Pn数据)
	
	Pn--;
	p8=(u8*)(ADDR_DATABUFF);
	MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),1);
//1BYTE B0小时冻结0=无效1=有效
	p8[0]&=0xfe;
	MR(ADDR_DATABUFF+1,ADDR_TCongealCount+7,23);
//2BYTE B0-B14曲线冻结计数,b15=1表示曲线冻结计数满(初始化值0)
//1BYTE B0-B6日冻结计数,B7=1表示日冻结计数满(初始化值0)
	p8[3]=p8[1+7];
//1BYTE B0-B6日冻结计数,B7=1表示抄表日冻结计数满(初始化值0)
	p8[4]=p8[1+7+7];
//1BYTE B0-B6抄表月冻结计数,B7=1表示月冻结计数满(初始化值0)
	p8[5]=p8[1+7+7+7];
	MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),6);
#else
	Init_Class2SourceData(Pn);//类2原始数据初始化(指定Pn数据)
#endif
	
}

void PnHourCongealCount(void)//测量点的小时冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
{
	u32 i;
	u32 x;
	u32 Pn;
	u8 *p8;
	
	Pn=0;
	while(Pn<MaxRS485AddCarrierPn)
	{
		i=LEN_DATABUFF/LEN_PnCongealCount;
		if(i>(MaxRS485AddCarrierPn-Pn))
		{
			i=MaxRS485AddCarrierPn-Pn;
		}
		MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		p8=(u8*)(ADDR_DATABUFF);
		for(x=0;x<i;x++)
		{
			p8[0]|=0x01;//B0小时冻结0=无效1=有效,B1-B7保留
			p8+=LEN_PnCongealCount;
		}
		MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		Pn+=i;
	}
}

void PnCurveCongealCount(void)//测量点的曲线冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u32 Count;
	u8 *p8;
	
	Count=MRR(ADDR_TCongealCount+7,2);//当前曲线冻结计数
	Count&=0x7fff;
	Count+=1;
	if(Count>=MaxCurveCongeal)
	{
		Count=0;
	}
	Pn=0;
	while(Pn<MaxRS485AddCarrierPn)
	{
		i=LEN_DATABUFF/LEN_PnCongealCount;
		if(i>(MaxRS485AddCarrierPn-Pn))
		{
			i=MaxRS485AddCarrierPn-Pn;
		}
		MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		p8=(u8*)(ADDR_DATABUFF+1);
		for(x=0;x<i;x++)
		{
			y=p8[0]+(p8[1]<<8);//变更时的冻结计数值
			y&=0x7fff;
			if(y==Count)
			{//满轮回
				p8[0]=0;//清0,表示全部冻结点有效
				p8[1]=0;//清0,表示全部冻结点有效
			}
			p8+=LEN_PnCongealCount;
		}
		MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		Pn+=i;
	}
}

void PnCongealCount(u32 Count,u32 MaxCount,u32 Office)//清0电能表配置变更时的冻结计数值
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u8 *p8;

	Count&=0x7f;
	Count+=1;
	if(Count>=MaxCount)
	{
		Count=0;
	}
	Pn=0;
	while(Pn<MaxRS485AddCarrierPn)
	{
		i=LEN_DATABUFF/LEN_PnCongealCount;
		if(i>(MaxRS485AddCarrierPn-Pn))
		{
			i=MaxRS485AddCarrierPn-Pn;
		}
		MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		p8=(u8*)(ADDR_DATABUFF+Office);
		for(x=0;x<i;x++)
		{
			y=p8[0];//变更时的冻结计数值
			y&=0x7f;
			if(y==Count)
			{//满轮回
				p8[0]=0;//清0,表示全部冻结点有效
			}
			p8+=LEN_PnCongealCount;
		}
		MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		Pn+=i;
	}

}
void PnDateCongealCount(void)//测量点的日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
{
	u32 Count;
	Count=MRR(ADDR_TCongealCount+14,1);
	PnCongealCount(Count,MaxDateCongeal,3);//清0电能表配置变更时的冻结计数值
}

void PnRMDateCongealCount(void)//测量点的抄表日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
{
	u32 Count;
	Count=MRR(ADDR_TCongealCount+21,1);
	PnCongealCount(Count,MaxRMDateCongeal,4);//清0电能表配置变更时的冻结计数值
}

void PnMonthCongealCount(void)//测量点的月冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
{
	u32 Count;
	Count=MRR(ADDR_TCongealCount+28,1);
	PnCongealCount(Count,MaxMonthCongeal,5);//清0电能表配置变更时的冻结计数值
}


u32 PnHourCongeal(u32 Pn)//测量点小时冻结有效,返回=0无效,1=有效
{
	u32 i;
	if(Pn==0)
	{
		return 1;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0;
	}
	Pn--;
	i=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),1);
	i&=1;
	return i;
}


u32 PnCongealNo(u32 CongealNo,u32 PnCount,u32 Count)//测量点曲线冻结号有效,返回=0xffffffff表示无效(没找到)
{
	if(PnCount==Count)
	{//初始后无新冻结
		return 0xffffffff;
	}
	if(Count>PnCount)
	{//有新冻结,但没回0
		if((CongealNo>=PnCount)&&(CongealNo<Count))
		{//冻结号>=初始号,同时<当前号
			return CongealNo;
		}
	}
	else
	{//有新冻结,且已回0
		if(CongealNo>=PnCount)
		{//冻结号>=初始号
			return CongealNo;
		}
		if(CongealNo<Count)
		{//冻结号<当前号
			return CongealNo;
		}
	}
	return 0xffffffff;
}

u32 PnCurveCongealNo(u32 CongealNo,u32 Pn)//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+1,2);
	if(PnCount==0)
	{//无初始冻结值
		return CongealNo;
	}	
	PnCount&=0x7fff;
	Count=MRR(ADDR_TCongealCount+7,2);
	Count&=0x7fff;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//测量点曲线冻结号有效,返回=0xffffffff表示无效(没找到)
	return CongealNo;
}

u32 PnDateCongealNo(u32 CongealNo,u32 Pn)//测量点日冻结号有效,返回=0xffffffff表示无效(没找到)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+3,1);
	if(PnCount==0)
	{//无初始冻结值
		return CongealNo;
	}	
	PnCount&=0x7f;
	Count=MRR(ADDR_TCongealCount+14,1);
	Count&=0x7f;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//测量点曲线冻结号有效,返回=0xffffffff表示无效(没找到)
	return CongealNo;
}

u32 PnRMDateCongealNo(u32 CongealNo,u32 Pn)//测量点抄表日冻结号有效,返回=0xffffffff表示无效(没找到)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+4,1);
	if(PnCount==0)
	{//无初始冻结值
		return CongealNo;
	}	
	PnCount&=0x7f;
	Count=MRR(ADDR_TCongealCount+21,1);
	Count&=0x7f;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//测量点曲线冻结号有效,返回=0xffffffff表示无效(没找到)
	return CongealNo;
}

u32 PnMonthCongealNo(u32 CongealNo,u32 Pn)//测量点月冻结号有效,返回=0xffffffff表示无效(没找到)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+5,1);
	if(PnCount==0)
	{//无初始冻结值
		return CongealNo;
	}	
	PnCount&=0x7f;
	Count=MRR(ADDR_TCongealCount+28,1);
	Count&=0x7f;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//测量点曲线冻结号有效,返回=0xffffffff表示无效(没找到)
	return CongealNo;
}
















