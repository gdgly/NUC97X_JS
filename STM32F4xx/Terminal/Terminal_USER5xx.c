
//用户标识:上海系
#include "Project.h"
#include "../terminal/Terminal_USER5xx.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"




#if (USER/100)==5//用户标识:上海系
u32 GetCurveCongealNo_Ext(u64 YMDHM)//得到扩展曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
{
#if ((IC_SST39VF6401B||IC_S29GLxxx)==0)//0=没,1=有
	u32 i;
	u32 x;
	x=MRR(ADDR_CURVECONGEAL_ExtCount,2);
	if((x&0x8000)!=0x0)
	{
		x=MaxCurveCongeal_Ext;
	}
	for(i=0;i<x;i++)
	{
		if(YMDHM==MRR(ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*i),5))
		{
			return i;//找到
		}
	}
	return 0xffffffff;
#else
	u32 i;
	u32 x;
	u32 ADDR_EFLASH;
	u64 d64;
	u32 *p32;
	
	x=MRR(ADDR_CURVECONGEAL_ExtCount,2);
	if((x&0x8000)!=0x0)
	{
		x=MaxCurveCongeal_Ext;
	}
	for(i=0;i<x;i++)
	{
		ADDR_EFLASH=ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*i);
		if(ADDR_EFLASH<0x80800000)
		{
			if((ADDR_EFLASH+5)<0x80800000)
			{
				p32=(u32*)(ADDR_EFLASH);
				d64=p32[1]&0xff;
				d64<<=32;
				d64|=p32[0];
			}
			else
			{
				d64=MRR(ADDR_EFLASH,5);
			}
		}
		else
		{
			ADDR_EFLASH-=0x80800000;
			ADDR_EFLASH+=0x90000000;
			p32=(u32*)(ADDR_EFLASH);
			d64=p32[1]&0xff;
			d64<<=32;
			d64|=p32[0];
		}
		if(YMDHM==d64)
		{
			return i;//找到
		}
	}
	return 0xffffffff;
#endif
}



#endif


