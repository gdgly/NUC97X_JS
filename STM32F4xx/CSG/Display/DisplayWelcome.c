
#include "Project.h"
#include "../Display/DisplayWelcome.h"
#include "../Display/Display.h"
#include "../Display/Draw.h"
#include "../Driver/IC_BUZZ.h"



void DisplayWelcome(void)//上电屏幕
{
//	u32 i;
//	u16 *p16;
//	u32 *p32;
//
//	i=Comm_Ram->DisplayAllErrorTimer;
//	if(i>9)
//	{
//		i=9;
//	}
//	if(i==1)
//	{
//		if(DOTMIXPMODE->FlashCount==160)
//		{
//			Comm_Ram->BUZZCTRL[8]=1;//允许发声
//			BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
//			DOTMIXPMODE->FlashCount++;
//			Draw_Clr();//清绘画显示
//		}
//	}
//	p16=(u16*)(ADDR_STRINGBUFF+(5*84)+4+(5*2));
//	p16[0]=i+0x30+0x2000;
//	
//	if(DOTMIXPMODE->FlashCount<160)
//	{
//		if(DOTMIXPMODE->FlashCount<80)
//		{
//			DOTMIXPMODE->Mix=4;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
//		}
//		else
//		{
//			DOTMIXPMODE->Mix=3;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
//			if(DOTMIXPMODE->FlashCount==80)
//			{
//				p32=(u32*)(ADDR_DOTDATABUFF2);
//				for(i=0;i<((20/4)*160);i++)
//				{
//					p32[i]=0x0;
//				}
//			}
//		}
//		p32=(u32 *)(ADDR_DOTDATABUFF2+(20*DOTMIXPMODE->FlashCount));
//		for(i=0;i<(20/4);i++)
//		{
//			p32[i]=0xffffffff;
//		}
//		p32=(u32 *)((ADDR_DOTDATABUFF2+(20*159))-(20*DOTMIXPMODE->FlashCount));
//		for(i=0;i<(20/4);i++)
//		{
//			p32[i]=0xffffffff;
//		}
//		
//		DOTMIXPMODE->FlashCount+=1;
//	}
}




