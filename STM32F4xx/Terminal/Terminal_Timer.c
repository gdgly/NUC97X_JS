
#include "Project.h"
#include "Terminal_Timer.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../device/MEMRW.h"

void Terminal_FMTimer(void)//掉电保持秒和分定时器
{
	u32 i;
	u32 s;
	u32 m;
	u8 *p8;
	u16 *p16;
	
	MR(ADDR_DATABUFF,ADDR_TerminalFMTimer,6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2));
	i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
	if(i)
	{//原时钟错误
		MW(ADDR_TYMDHMS,ADDR_TerminalFMTimer,6);//更新时钟
		return;
	}
	s=TimeDifference(ADDR_DATABUFF,ADDR_TYMDHMS);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
	if(s>>31)
	{//时钟2<时钟1差值为负
		MW(ADDR_TYMDHMS,ADDR_TerminalFMTimer,6);//更新时钟
		return;
	}
	if(s==0)
	{
		return;
	}
	
	MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
	p16=(u16*)(ADDR_DATABUFF+6);
//秒加计数计算分
	m=p16[0];
	m+=s;
	p16[0]=m%60;
	m/=60;
//秒减计数
	for(i=1;i<TFMSTIMER_NUM;i++)
	{
		if(p16[i]>=s)
		{
			p16[i]-=s;
		}
		else
		{
			p16[i]=0;
		}
	}
//分定时器
	p16=(u16*)(ADDR_DATABUFF+6+(TFMSTIMER_NUM*2));
	for(i=0;i<TFMMTIMER_NUM;i++)
	{
		if(p16[i]>=m)
		{
			p16[i]-=m;
		}
		else
		{
			p16[i]=0;
		}
	}
	MW(ADDR_DATABUFF,ADDR_TerminalFMTimer,6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2));

//实走定时器,调整时钟无效
	if(m!=0)
	{
		MR(ADDR_DATABUFF,ADDR_TerminalRealTimer,2+(TREALMTIMER_NUM*2));
		p8=(u8*)(ADDR_DATABUFF);
		p16=(u16*)(ADDR_DATABUFF+2);
		if(p8[0])
		{//0=掉电时间未计算,1=掉电时间已计算
			if(m>1)
			{
				m=1;
			}
		}
		p8[0]=1;//1=掉电时间已计算
		for(i=0;i<TREALMTIMER_NUM;i++)
		{
			if(p16[i]>=m)
			{
				p16[i]-=m;
			}
			else
			{
				p16[i]=0;
			}
		}
		MW(ADDR_DATABUFF,ADDR_TerminalRealTimer,2+(TREALMTIMER_NUM*2));
	}
}


void Terminal_Timer(void)//终端非中断秒和分定时器
{
	u32 i;
	u32 x;
	u32 t;
	u8 *p8;
	u16 *p16;

//秒定时器
	t=Terminal_Ram->Terminal_S_Count;//终端秒加计数,用于其他秒定时器
	if(t!=0x0)
	{
		//类3(事件)秒减定时器
	  p16=(u16*)(ADDR_ERC10_Timer);
		for(i=0;i<EVENT_S_TNum;i++)
		{
			x=p16[i];
			if(x>t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p16[i]=x;
		}
		//统计用原始数据
		//总加组最大最小有功功率在抄表完成后计算秒定时器
		p8=(u8 *)ADDR_AFN0DF57F60_S_Timer;
		if(p8[0]>t)
		{
			p8[0]-=t;
		}
		else
		{
			p8[0]=0;
		}
		//终端电压越限事件用S减定时器
		p8=(u8 *)(ADDR_AFN0DF27F35_SOUR+1);
		for(i=0;i<AFN0DF27MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			x=p8[4]+(p8[5]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[4]=x;
			p8[5]=x>>8;
			x=p8[8]+(p8[9]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[8]=x;
			p8[9]=x>>8;
			p8+=LEN_AFN0DF27F35_SOUR;
		}
		//终端电流越限事件用S减定时器
		p8=(u8 *)(ADDR_AFN0DF29F37_SOUR+1);
		for(i=0;i<AFN0DF29MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			x=p8[4]+(p8[5]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[4]=x;
			p8[5]=x>>8;
			x=p8[8]+(p8[9]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[8]=x;
			p8[9]=x>>8;
			x=p8[12]+(p8[13]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[12]=x;
			p8[13]=x>>8;
			p8+=LEN_AFN0DF29F37_SOUR;
		}
		//终端电压电流不平衡事件用S减定时器
		p8=(u8 *)(ADDR_AFN0DF28F36_SOUR+1);
		for(i=0;i<AFN0DF28MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			x=p8[4]+(p8[5]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[4]=x;
			p8[5]=x>>8;
			p8+=LEN_AFN0DF28F36_SOUR;
		}
		//终端视在功率越限事件用S减定时器
		p8=(u8 *)(ADDR_AFN0DF30F38_SOUR+1);
		for(i=0;i<AFN0DF30MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			p8+=LEN_AFN0DF30F38_SOUR;
		}
		//谐波越限秒加计数计算分
#if MeterHarmonic!=0//谐波计算0=没,1=有
#if IC_SDRAM
	  p8=(u8*)(ADDR_ERC15_SOUR);
		for(i=0;i<AFN0CF57MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			x+=t;
			p8[0]=x;
			p8[1]=x>>8;
			p8+=LEN_ERC15_SOUR;
		}
#endif
#endif	
		
	  __disable_irq();
		Terminal_Ram->Terminal_S_Count-=t;
	  __enable_irq();
		
	}
	
//分定时器
	t=Terminal_Ram->Terminal_M_Count;//终端分加计数,用于其他分定时器
	if(t!=0x0)
	{
		//终端日月工作时间
		i=MRR(ADDR_AFN0DF49_SOUR,2);
		i+=t;
		MWR(i,ADDR_AFN0DF49_SOUR,2);//写寄存器数据到存贮器,Byte<=8
		i=MRR(ADDR_AFN0DF51_SOUR,2);
		i+=t;
		MWR(i,ADDR_AFN0DF51_SOUR,2);//写寄存器数据到存贮器,Byte<=8
	
		//统计用原始数据
		//有功功率为零时间分更新加计数
	  p8=(u8*)(ADDR_AFN0DF25F33_SOUR);
		for(i=0;i<AFN0DF25MaxPn;i++)
		{
			p8[i]+=t;
		}
		//总加组有功功率为零时间分更新加计数
#if AFN0DF57MaxPn!=0
	  p8=(u8*)(ADDR_AFN0DF57F60_SOUR);
		for(i=0;i<AFN0DF57MaxPn;i++)
		{
			p8[i]+=t;
		}
#endif
		//日月电压统计
	  p8=(u8*)(ADDR_AFN0DF27F35_SOUR);
		for(i=0;i<AFN0DF27MaxPn;i++)
		{
			p8[i*LEN_AFN0DF27F35_SOUR]+=t;
		}
		//日月电流统计
	  p8=(u8*)(ADDR_AFN0DF29F37_SOUR);
		for(i=0;i<AFN0DF29MaxPn;i++)
		{
			p8[i*LEN_AFN0DF29F37_SOUR]+=t;
		}
		//电压电流不平衡
	  p8=(u8*)(ADDR_AFN0DF28F36_SOUR);
		for(i=0;i<AFN0DF28MaxPn;i++)
		{
			p8[i*LEN_AFN0DF28F36_SOUR]+=t;
		}
		//视在功率越限
	  p8=(u8*)(ADDR_AFN0DF30F38_SOUR);
		for(i=0;i<AFN0DF30MaxPn;i++)
		{
			p8[i*LEN_AFN0DF30F38_SOUR]+=t;
		}
		//总加组超功率定值月电能量定值
#if AFN0DF65MaxPn!=0
	  p8=(u8*)(ADDR_AFN0DF65F66_SOUR);
		for(i=0;i<AFN0DF65MaxPn;i++)
		{
			p8[i*9]+=t;
		}
#endif
		//功率因数区段累计时间
	  p8=(u8*)(ADDR_AFN0DF43F44_SOUR);
		for(i=0;i<AFN0DF43MaxPn;i++)
		{
			p8[i]+=t;
		}
		//直流模拟量越限累计时间
#if AFN0DF129MaxPn!=0
	  p8=(u8*)(ADDR_AFN0DF129F130_SOUR);
		for(i=0;i<AFN0DF129MaxPn;i++)
		{
			p8[i*LEN_AFN0DF129F130_SOUR]+=t;
		}
#endif	
		
		
	  __disable_irq();
		Terminal_Ram->Terminal_M_Count-=t;
	  __enable_irq();
	}
}









