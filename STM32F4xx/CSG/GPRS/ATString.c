
#include "../Hcsg/ProjectCSG.h"

#include "../GPRS/ATString.h"
#include "../Calculate/Calculate.h"


u32 ATRxSearch(u32 ADDR_String)//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
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
	Byte=0;//�ַ����Ƚ��ֽ���
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

u32 GetATRxString(u32 ADDR_StrBuff)//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
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
	//ȥ�ַ���ǰ����Ч0x0d,0x0a
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
	//ȡ�ַ���
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
				{//����cdmaģ��
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
		{//ͨ���ַ����ȶ���ADDR_DATABUFF
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

u32 GetATRxByte(void)//�жϽ���,���㵱ǰp0��Ӧp1����δ�����ֽ���
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

void GPRS_Signal(u32 Addr)//GPRS�ź�ǿ��0-4
{
	u32 i;
//	u8 *p8;
	u8 *p8s;
#if (USER/100)==11//����ϵ
	u32 x;
#endif

	p8s=(u8*)(Addr);
//	p8=(u8 *)(ADDR_AFN0CF4+1+16+6);
//	p8[0]=p8s[0];
	i=p8s[0]-0x30;
	if(p8s[1]==0x2c)//','��
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
	Terminal_Ram->SignaldBm=i;//�ź�ǿ��0-31��99
	if(i==0x99)
	{
		i=0;
	}
	i=bcd_hex(i);//8λѹ��BCD��תΪHEX
#if (USER/100)==11//����ϵ
	//��չAFN0CF201�ն������ź�Ʒ��
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
		i=(i-10)/4;//�ź�ǿ�ȴ���(0-4)
		if(i>4)
		{
			i=4;
		}
		if(i==0)
		{
			i=1;//>=10������СΪ1��
		}
		Terminal_Ram->Signal=i;
	}
	else
	{
		Terminal_Ram->Signal=0;
	}
}
































