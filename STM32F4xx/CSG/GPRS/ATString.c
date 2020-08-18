
#include "../Hcsg/ProjectCSG.h"

#include "../GPRS/ATString.h"
#include "../Calculate/Calculate.h"


u32 ATRxSearch(u32 ADDR_String)//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
{
	u32 i;
	u32 x;
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;

	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		p16[0]=0;
		p16[1]=0;
		p16[2]=0;
		p16[3]=0;
		p16[4]=0;
		return 0;
	}
	while(p1>=(LEN_UART1IntATRx-10))
	{
		p1-=(LEN_UART1IntATRx-10);
		p16[1]=p1;
	}
	p8s=(u8*)(ADDR_UART1IntATRx+10);
	p8d=(u8*)(ADDR_String);
	Byte=0;//字符串比较字节数
	while(p8d[Byte]!=0)
	{
		Byte++;
	}
	while(p0!=p1)
	{
		if(p0>=p1)
		{
			i=p0-p1;
		}
		else
		{
			i=(p0+(LEN_UART1IntATRx-10))-p1;
		}
		if(i<Byte)
		{
			return 0;
		}
		x=p1;
		for(i=0;i<Byte;i++)
		{
			if(p8s[x]!=p8d[i])
			{
				break;
			}
			x++;
			if(x>=(LEN_UART1IntATRx-10))
			{
				x=0;
			}
		}
		if(i==Byte)
		{
			return (((u32)p8s)+x);
		}
		p1++;
		if(p1>=(LEN_UART1IntATRx-10))
		{
			p1=0;
		}
	}
	return 0;
}

u32 GetATRxString(u32 ADDR_StrBuff)//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
{
	u32 x;
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;

	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		p16[0]=0;
		p16[1]=0;
		p16[2]=0;
		p16[3]=0;
		p16[4]=0;
		return 0;
	}
	while(p1>=(LEN_UART1IntATRx-10))
	{
		p1-=(LEN_UART1IntATRx-10);
		p16[1]=p1;
	}
	p8s=(u8*)(ADDR_UART1IntATRx+10);
	p8d=(u8*)(ADDR_StrBuff);
	Byte=0;
	if(p0>=p1)
	{
		Byte=p0-p1;
	}
	else
	{
		Byte=(p0+(LEN_UART1IntATRx-10))-p1;
	}
	//去字符串前的无效0x0d,0x0a
	while(Byte!=0)
	{
		if((p8s[p1]==0x0d)||(p8s[p1]==0x0a))
		{
			Byte--;
			p1++;
			if(p1>=(LEN_UART1IntATRx-10))
			{
				p1=0;
			}
		}
		else
		{
			break;
		}
	}
	//取字符串
	x=0;
	while(Byte>=2)
	{
		if(p8s[p1]==0x0d)
		{
			p1++;
			if(p1>=(LEN_UART1IntATRx-10))
			{
				p1=0;
			}
			if(p8s[p1]==0x0a)
			{
				p1++;
				if(p1>=(LEN_UART1IntATRx-10))
				{
					p1=0;
				}
				p16[1]=p1;
				return x;
			}
			else
			{
				if(p8s[p1]==0x0d)
				{//中兴cdma模块
					p1++;
					if(p1>=(LEN_UART1IntATRx-10))
					{
						p1=0;
					}
					if(p8s[p1]==0x0a)
					{
						p1++;
						if(p1>=(LEN_UART1IntATRx-10))
						{
							p1=0;
						}
						p16[1]=p1;
						return x;
					}
				}
			}
			return 0;
		}
		p8d[x]=p8s[p1];
		x++;
		if(x>=LEN_DATABUFF)
		{//通常字符串先读到ADDR_DATABUFF
			x=0;
		}
		p1++;
		if(p1>=(LEN_UART1IntATRx-10))
		{
			p1=0;
		}
		Byte--;
	}
	return 0;
}

u32 GetATRxByte(void)//中断接收,计算当前p0对应p1还有未处理字节数
{
	u32 p0;
	u32 p1;
	u16 *p16;
	
	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		p16[0]=0;
		p16[1]=0;
		p16[2]=0;
		p16[3]=0;
		p16[4]=0;
		return 0;
	}
	while(p1>=(LEN_UART1IntATRx-10))
	{
		p1-=(LEN_UART1IntATRx-10);
		p16[1]=p1;
	}
	if(p0<p1)
	{
		p0+=(LEN_UART1IntATRx-10);
	}
	return(p0-p1);
}

void GPRS_Signal(u32 Addr)//GPRS信号强度0-4
{
	u32 i;
//	u8 *p8;
	u8 *p8s;
#if (USER/100)==11//吉林系
	u32 x;
#endif

	p8s=(u8*)(Addr);
//	p8=(u8 *)(ADDR_AFN0CF4+1+16+6);
//	p8[0]=p8s[0];
	i=p8s[0]-0x30;
	if(p8s[1]==0x2c)//','号
	{
//		p8[1]=0;
	}
	else
	{
//		p8[1]=p8s[1];
//		p8[2]=0;
		i<<=4;
		i|=p8s[1]-0x30;
	}
	Terminal_Ram->SignaldBm=i;//信号强度0-31或99
	if(i==0x99)
	{
		i=0;
	}
	i=bcd_hex(i);//8位压缩BCD数转为HEX
#if (USER/100)==11//吉林系
	//扩展AFN0CF201终端网络信号品质
	x=i;
	if(x>31)
	{
		x=0;
	}
	x*=2;
	x=113-x;
	x=hex_bcd(x);
	x<<=4;
	x|=0x8000;
//	p8=(u8 *)(ADDR_AFN0CF201);
//	p8[0]=x;
//	p8[1]=(x>>8);
#endif	
	if(i>=10)
	{
		i=(i-10)/4;//信号强度代号(0-4)
		if(i>4)
		{
			i=4;
		}
		if(i==0)
		{
			i=1;//>=10以上最小为1格
		}
		Terminal_Ram->Signal=i;
	}
	else
	{
		Terminal_Ram->Signal=0;
	}
}
































