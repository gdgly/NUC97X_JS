
//Calculate
#include "../Hcsg/ProjectCSG.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"


u32 hex_bcd(u32 hex)//HEX数转为8位压缩BCD数
{
	u8 i;
	u32 bcd;
	u32 y;
	bcd=0;
	y=10000000;
	for(i=7;i!=0x0;i--)
	{
		bcd+=hex/y;
		bcd<<=4;
		hex%=y;
		y/=10;
	}
	bcd+=hex;
	return(bcd);
}

u64 hex_bcd16(u64 hex16)//HEX数转为16位压缩BCD数
{
	u32 i;
	u64 bcd;
	u64 x;
	u64 y;
	
	x=0x1000000000000000;
	y=1000000000000000;
	bcd=0;
	for(i=0;i<15;i++)
	{
		while(hex16>=y)
		{
			hex16-=y;
			bcd+=x;
		}
		x>>=4;
		y/=10;
	}
	bcd+=hex16;
	return(bcd);
}

u32 bcd_hex(u32 bcd)//8位压缩BCD数转为HEX
{
	u8 i;
	u32 hex;
	hex=0;
	for(i=7;i!=0x0;i--)
	{
		hex+=(bcd>>28)&0xf;
		hex*=10;
		bcd<<=4;
	}
	hex+=(bcd>>28)&0xf;
	return(hex);
}

u64 bcd16_hex(u64 bcd)//16位压缩BCD数转为HEX
{
	u32 i;
	u64 x;
	u64 hex;
	hex=0;
	x=1;
	for(i=0;i<16;i++)
	{
		hex+=((bcd&0x0f)*x);
		x*=10;
		bcd>>=4;
	}
	return(hex);
}

u8 CharToHex(u8 SourceChar)
{
	u8 TempByte;
	switch(SourceChar)
	{
		case '0':	TempByte = 0x00;	break;
		case '1':	TempByte = 0x01;	break;
		case '2':	TempByte = 0x02;	break;
		case '3':	TempByte = 0x03;	break;
		case '4':	TempByte = 0x04;	break;
		case '5':	TempByte = 0x05;	break;
		case '6':	TempByte = 0x06;	break;
		case '7':	TempByte = 0x07;	break;
		case '8':	TempByte = 0x08;	break;
		case '9':	TempByte = 0x09;	break;
		case 'A':	TempByte = 0x0A;	break;
		case 'B':	TempByte = 0x0B;	break;
		case 'C':	TempByte = 0x0C;	break;
		case 'D':	TempByte = 0x0D;	break;
		case 'E':	TempByte = 0x0E;	break;
		case 'F':	TempByte = 0x0F;	break;
		case 'a':	TempByte = 0x0A;	break;
		case 'b':	TempByte = 0x0B;	break;
		case 'c':	TempByte = 0x0C;	break;
		case 'd':	TempByte = 0x0D;	break;
		case 'e':	TempByte = 0x0E;	break;
		case 'f':	TempByte = 0x0F;	break;
		default:	TempByte = 0x0F;	break;
	}
	return TempByte;
}


u32 bcd_add(u32 bcd1,u32 bcd2)//无符号bcd数加
{
  bcd1=bcd_hex(bcd1);
  bcd2=bcd_hex(bcd2);
  bcd1+=bcd2;
  if(bcd1>=100000000)
  {
    bcd1-=100000000;
  }
  bcd1=hex_bcd(bcd1);
  return bcd1;
}

void Data_Inverse(u32 DataAddr,u32 Le)//数据倒序(高低字节调换),入口Le>=2
{
	u32 i;
	u32 h;
	u32 l;
	u8 * p8;
	
	if(Le>=2)
	{
		p8=(u8 *)(DataAddr);
		for(i=0;(Le-(i*2))>=2;i++)
		{
			l=p8[i];
			h=p8[Le-1-i];
			p8[i]=h;
			p8[Le-1-i]=l;
		}
	}
}

u32 DataComp(u32 DataAddr1,u32 DataAddr2,u32 Byte)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
{
	u32 i;
	u64 d64a;
	u64 d64b;
	u8 * p8s;
	u8 * p8d;

	if(Byte>8)
	{
		p8s=(u8 *)(DataAddr1);
		p8d=(u8 *)(DataAddr2);
		for(i=0;i<Byte;i++)
		{
			if(p8s[i]!=p8d[i])
			{
				break;
			}
		}
		if(i==Byte)
		{
			return 0;
		}
		return 1;
	}
	else
	{
		d64a=MRR(DataAddr1,Byte);
		d64b=MRR(DataAddr2,Byte);
		if(d64a==d64b)
		{
			return 0;
		}
		if(d64a>d64b)
		{
			return 1;
		}
		return 2;
	}
}

u32 WinDataComp(u32 DataAddr1,u32 DataAddr2,u32 Byte,u32 TByte)//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
{
	u32 i;
	u32 x;
	u8 * p8s;
	u8 * p8d;
	
	if(TByte<Byte)
	{
		return 0;
	}
	p8s=(u8 *)(DataAddr1);
	p8d=(u8 *)(DataAddr2);
	for(i=0;i<=(TByte-Byte);i++)
	{
		for(x=0;x<Byte;x++)
		{
			if(p8s[x]!=p8d[x])
			{
				break;
			}
		}
		if(x==Byte)
		{
			return((u32)p8d);
		}
		p8d+=1;
	}
	return 0;
}

u32 BitLoca(u32 bin)//计算第1个置位为1的位置(0-32)
{
	u32 i;
	if(bin==0x0)
	{
		return(0);
	}
	i=0;
	while((bin&(1<<i))==0x0)
	{
		i+=1;
	}
	return(i+1);
}

u32 BitNum(u32 bin)//计算置位为1的个数(0-32)
{
	u32 i;
	u32 x;
	x=0;
	for(i=0;i<32;i++)
	{
		if((bin&(1<<i))!=0x0)
		{
			x+=1;
		}
	}
	return(x);
}

u32 YMDCalculateW(u32 RTCAddr)//指定地址缓冲中的年月日周时分秒计算周
{
	u8 i;
	u8 * p8;
	
	s16 W;
	u8 C;//世纪-1(年份前两位数)
	u8 y;//年份后两位数
	u8 M;//月份
	u8 d;//日数
	
	p8=(u8 *)(RTCAddr);
	for(i=0;i<7;i++)
	{
		p8[i]=bcd_hex(p8[i]);//8位压缩BCD数转为HEX
	}
//蔡勒公式：
//W=[C/4]-2C+y+[y/4]+[13*(M+1)/5]+d-1
//C:世纪-1(年份前两位数）
//y:年份后两位数
//M:月份
//d:日数
//1月和2月要按上一年的13月和14月来算,这时C和y均按上一年取值。
//算出来的W 除以7，余数是几就是星期几。如果余数是0，则为星期日
	C=20;//世纪-1(年份前两位数）
	y=p8[6];//年份后两位数
	M=p8[5];//月份
	d=p8[4];//日数
	
	if(M<3)
	{
		M+=12;
		y--;
		if(y==99)
		{
			C--;
		}
	}
	W=(C/4)-(2*C)+y+(y/4)+((13*(M+1))/5)+d-1;
	W=((W%7)+7)%7;
	p8[3]=W;

	for(i=0;i<7;i++)
	{
		p8[i]=hex_bcd(p8[i]);//HEX转为BCD
	}
	return W;
}


const u8 MothVSDate[]=//每月日数列表
{
	0,
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31,
};

u32 YM_D(u32 YM)//取指定年月的天数
{
	u32 M;
	M=bcd_hex(YM&0xff);
	if(M>12)
	{
		return 0;
	}
	YM=bcd_hex((YM>>8)&0xff);
	if((YM%4)==0)
	{//能被4整除的年
		if(M==2)
		{//是2月
			return 29;
		}
	}
	return MothVSDate[M];
}

u32 Check_BCD(u32 BCD)//BCD合法检查,返回0合法,1非法
{
	u32 i;
	for(i=0;i<8;i++)
	{
		if((BCD&0x0f)>9)
		{
			return(1);
		}
		BCD>>=4;
	}
	return(0);
}
u32 Check_HMS(u32 HMS)//时分秒合法检查,返回0合法,1非法
{
	if(Check_BCD(HMS)!=0X0)
	{
		return(1);
	}
	if((HMS&0xff)>=0x60)
	{
		return(1);
	}
	HMS>>=8;
	if((HMS&0xff)>=0x60)
	{
		return(1);
	}
	HMS>>=8;
	if((HMS&0xff)>=0x24)
	{
		return(1);
	}
	return(0);
}
u32 Check_YMD(u32 YMD)//年月日合法检查,返回0合法,1非法
{
	u32 Y;
	u32 M;
	u32 D;
	
	if(Check_BCD(YMD)!=0X0)
	{
		return(1);
	}
	D=bcd_hex(YMD&0xff);
	M=bcd_hex((YMD>>8)&0xff);
	Y=bcd_hex((YMD>>16)&0xff);
	if(D==0x0)
	{
		return(1);
	}
	if(M==0x0)
	{
		return(1);
	}
	if(M>12)
	{
		return(1);
	}
	if((Y%4)==0x0)
	{
		if(M==2)
		{
			if(D>29)
			{
				return(1);
			}
			else
			{
				return(0);
			}
		}
	}
	if(D>MothVSDate[M])
	{
		return(1);
	}
	return(0);
}

u32 Check_YMDHMS(u64 YMDHMS)//年月日时分秒合法检查,返回0合法,1非法
{
	if(Check_HMS(YMDHMS)!=0x0)
	{
		return 1;
	}
	if(Check_YMD(YMDHMS>>24)!=0x0)
	{
		return 1;
	}
	return 0;
}


u32 YMDHMS_Sec(u32 RTCAddr)//时钟对00年01月01日00时00分00秒的秒计数
{
	u32 i;
	u32 x;
	u32 s;
	u8 * p8;

	p8=(u8 *)(RTCAddr);
	x=bcd_hex(p8[5]);//取年计算日数
	if((x%4)==0x0)
	{//能被4整除的年
		s=(x*365)+(x/4);
		if(p8[4]>0x2)
		{//月>2
			s+=1;
		}
	}
	else
	{//非4整除的年
		s=(x*365)+(x/4)+1;
	}
	x=bcd_hex(p8[4]);//取月计算日数
	for(i=1;i<x;i++)
	{
		s+=MothVSDate[i];
	}
	s+=bcd_hex(p8[3]);//取日
	s-=1;//01月01日不算
	s*=(24*60*60);
	s+=(bcd_hex(p8[2])*60*60);//时
	s+=(bcd_hex(p8[1])*60);//分
	s+=bcd_hex(p8[0]);//秒
	return(s);
}

u32 TimeDifference(u32 RTC1Addr,u32 RTC2Addr)//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
{
	u32 s1;
	u32 s2;
	u8 * p8;

	s1=0;
	p8=(u8 *)(RTC1Addr);
	s2=p8[0]+(p8[1]<<8)+(p8[2]<<16);
	s1+=Check_HMS(s2);
	s2=p8[3]+(p8[4]<<8)+(p8[5]<<16);
	s1+=Check_YMD(s2);
	p8=(u8 *)(RTC2Addr);
	s2=p8[0]+(p8[1]<<8)+(p8[2]<<16);
	s1+=Check_HMS(s2);
	s2=p8[3]+(p8[4]<<8)+(p8[5]<<16);
	s1+=Check_YMD(s2);
	if(s1!=0x0)
	{
		return(0);
	}
	s1=YMDHMS_Sec(RTC1Addr);
	s2=YMDHMS_Sec(RTC2Addr);
	return(s2-s1);
}

u32 YMDHMS_Sub(u64 YMDHMS1,u64 YMDHMS2)//时钟差值计算,入口时钟1(秒分时日月年);时钟2(秒分时日月年);返回HEX补码时钟快或慢的差值(秒),时钟1时钟2非法=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
{
	u32 s1;
	u32 s2;
	u64 *p64;
	s1=Check_YMDHMS(YMDHMS1);//年月日时分秒合法检查,返回0合法,1非法
	s1|=Check_YMDHMS(YMDHMS2);//年月日时分秒合法检查,返回0合法,1非法
	if(s1!=0x0)
	{
		return(0);
	}
	p64=(u64*)(ADDR_MRWBUFF);
	p64[0]=YMDHMS1;
	s1=YMDHMS_Sec(ADDR_MRWBUFF);
	p64[0]=YMDHMS2;
	s2=YMDHMS_Sec(ADDR_MRWBUFF);
	return(s2-s1);
}

u32 YMD_Add1D(u32 YMD)//年月日加1日,返回加1后的年月日
{
	u32 Y;
	u32 M;
	u32 D;

	D=bcd_hex(YMD&0xff);
	M=bcd_hex((YMD>>8)&0xff);
	Y=bcd_hex((YMD>>16)&0xff);

	D+=1;
	if((Y%4)==0x0)
	{
		if(M==2)
		{
			if(D>29)
			{
				D=1;
				M+=1;
			}
		}
		else
		{
			if(D>MothVSDate[M])
			{
				D=1;
				M+=1;
			}
		}
	}
	else
	{
		if(D>MothVSDate[M])
		{
			D=1;
			M+=1;
		}
	}
	if(M>12)
	{
		M=1;
		Y+=1;
		if(Y>99)
		{
			Y=0;
		}
	}

	D=hex_bcd(D);
	M=hex_bcd(M);
	Y=hex_bcd(Y);
	YMD=D+(M<<8)+(Y<<16);
	return(YMD);
}

u32 YMD_Sub1D(u32 YMD)//年月日减1日,返回减1后的年月日
{
	u32 Y;
	u32 M;
	u32 D;

	D=bcd_hex(YMD&0xff);
	M=bcd_hex((YMD>>8)&0xff);
	Y=bcd_hex((YMD>>16)&0xff);

	D-=1;
	if(D==0x0)
	{
		M-=1;
		if(M==0x0)
		{
			M=12;
			Y-=1;
			if(Y>99)
			{
				Y=99;
			}
		}
		if((Y%4)==0x0)
		{
			if(M==2)
			{
				D=29;
			}
			else
			{
				D=MothVSDate[M];
			}
		}
		else
		{
			D=MothVSDate[M];
		}
	}

	D=hex_bcd(D);
	M=hex_bcd(M);
	Y=hex_bcd(Y);
	YMD=D+(M<<8)+(Y<<16);
	return(YMD);
}

u32 YM_Add1M(u32 YM)//年月加1月,返回加1后的年月
{
	u32 Y;
	u32 M;

	M=bcd_hex(YM&0xff);
	Y=bcd_hex((YM>>8)&0xff);
	M+=1;
	if(M>12)
	{
		M=1;
		Y+=1;
		if(Y>99)
		{
			Y=0;
		}
	}
	M=hex_bcd(M);
	Y=hex_bcd(Y);
	YM=M+(Y<<8);
	return(YM);
}

u32 YM_Sub1M(u32 YM)//年月减1月,返回减1后的年月
{
	u32 Y;
	u32 M;

	M=bcd_hex(YM&0xff);
	Y=bcd_hex((YM>>8)&0xff);
	M-=1;
	if(M==0x0)
	{
		M=12;
		Y-=1;
		if(Y>99)
		{
			Y=99;
		}
	}
	M=hex_bcd(M);
	Y=hex_bcd(Y);
	YM=M+(Y<<8);
	return(YM);
}

u64 YMDHM_AddM(u64 YMDHM,u32 m)//年月日时分加m分,返回加m分后的年月日时分
{
	u32 YMD;
	
	YMD=YMDHM>>16;
	m+=bcd_hex(YMDHM&0xff);
	m+=bcd_hex((YMDHM>>8)&0xff)*60;
	while(m>=(24*60))
	{
		YMD=YMD_Add1D(YMD);//年月日加1日,返回加1后的年月日
		m-=(24*60);
	}
	YMDHM=YMD;
	YMDHM<<=16;
	if(m>=60)
	{
		YMDHM|=hex_bcd(m/60)<<8;
		m%=60;
	}
	YMDHM|=hex_bcd(m);
	return(YMDHM);
}

u64 YMDHM_SubM(u64 YMDHM,u32 m)//年月日时分减m分,返回减m分后的年月日时分
{
	u32 i;
	u32 YMD;
	
	YMD=YMDHM>>16;
	while(m>=(24*60))
	{
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		m-=(24*60);
	}
	i=bcd_hex(YMDHM&0xff);
	i+=bcd_hex((YMDHM>>8)&0xff)*60;
	if(i>=m)
	{
		m=i-m;
	}
	else
	{
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		i+=(24*60);
		m=i-m;
	}
	while(m>=(24*60))
	{
		YMD=YMD_Add1D(YMD);//年月日加1日,返回加1后的年月日
		m-=(24*60);
	}
	YMDHM=YMD;
	YMDHM<<=16;
	if(m>=60)
	{
		YMDHM|=hex_bcd(m/60)<<8;
		m%=60;
	}
	YMDHM|=hex_bcd(m);
	return(YMDHM);
}

u32 YMDH_AddH(u32 YMDH,u32 h)//年月日时加h时,返回加h时后的年月日时
{
	u64 d64;
	d64=YMDH;
	d64<<=8;
	h*=60;
	d64=YMDHM_AddM(d64,h);//年月日时分加m分,返回加m分后的年月日时分
	d64>>=8;
	return d64;
}

u32 YMDH_SubH(u32 YMDH,u32 h)//年月日时减h时,返回减h时后的年月日时
{
	u64 d64;
	d64=YMDH;
	d64<<=8;
	h*=60;
	d64=YMDHM_SubM(d64,h);//年月日时分减m分,返回减m分后的年月日时分
	d64>>=8;
	return d64;
}

u64 YMDHMS_AddS(u64 YMDHMS,u32 S)//年月日时分秒加S秒,返回加S秒后的年月日时分秒
{
	S+=bcd_hex(YMDHMS&0xff);
	YMDHMS>>=8;
	YMDHMS=YMDHM_AddM(YMDHMS,S/60);//年月日时分加m分,返回加m分后的年月日时分
	YMDHMS<<=8;
	S%=60;
	S=hex_bcd(S);
	YMDHMS|=S;
	return YMDHMS;
}

u64 YMDHMS_SubS(u64 YMDHMS,u32 S)//年月日时分秒减S秒,返回减S秒后的年月日时分秒
{
	u32 i;
	u32 m;
	i=YMDHMS&0xff;
	i=bcd_hex(i);
	m=S/60;
	S%=60;
	if(i<S)
	{
		m++;
		i+=60;
	}
	S=i-S;
	YMDHMS>>=8;
	YMDHMS=YMDHM_SubM(YMDHMS,m);//年月日时分减m分,返回减m分后的年月日时分
	YMDHMS<<=8;
	S=hex_bcd(S);
	YMDHMS|=S;
	return YMDHMS;
}


u32 CopyString(u8 *ps,u8 *pd)//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
{
	u32 i;
	i=0;
	while(ps[i]!=0)
	{
		pd[i]=ps[i];
		i++;
	}
	return i;
}


u32 ASCII_HEX(u32 ASCIIAddr)//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
{
	u32 x;
	u32 f;
	u8 * p8;

	p8=(u8 *)(ASCIIAddr);
	x=0;
	f=0;
	while(1)
	{
		if(p8[0]>=0x30)
		{
			if(p8[0]<=0x39)
			{
				x<<=4;
				x|=p8[0]-0x30;
				f=1;
				p8++;
				continue;
			}
		}
		if(f==0)
		{
			if(p8[0]==' ')
			{//有效数据开始前的空格
				p8++;
				continue;
			}
		}
		break;
	}
	x=bcd_hex(x);
	return x;
}

u32 ASCII_BCD(u32 ASCIIAddr,u32 Byte)//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
{
	u32 i;
	u32 x;
	u8 * p8;

	p8=(u8 *)(ASCIIAddr);
	x=0;
	for(i=0;i<Byte;i++)
	{
		x<<=4;
		if(p8[i]>=0x30)
		{
			if(p8[i]<=0x39)
			{
				x|=p8[i]-0x30;
				continue;
			}
		}
		if(p8[i]>=0x41)
		{
			if(p8[i]<=0x46)
			{
				x|=p8[i]-(0x41-10);
				continue;
			}
		}
		return 0xffffffff;
	}
	return x;
}

u32 BCD_ASCII(u32 BCD,u32 Addr)//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
{
	u32 i;
	u32 x;
	u32 Byte;
	u8 *p8;
	
	p8=(u8*)Addr;
	Byte=0;//有效字符数
	for(i=0;i<8;i++)
	{
		x=(BCD>>28)&0xf;
		BCD<<=4;
		if(x==0)
		{
			if(Byte==0)
			{
				continue;
			}
		}
		Byte++;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		p8[0]=x;
		p8++;
	}
	if(Byte==0)
	{
		p8[0]=0x30;
		Byte=1;
	}
	return(Byte);
}

u32 HEX_ASCII(u32 HEX,u32 Addr)//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
{
	u32 Byte;
	HEX=hex_bcd(HEX);//HEX数转为8位压缩BCD数
	Byte=BCD_ASCII(HEX,Addr);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	return(Byte);
}

u32 IP_ASCII_START(u32 Addr)//找到ip有效字符开始地址,若在256字符内没找到则返回0
{
	u32 i;
	u8 *p8;
	p8=(u8*)Addr;
	for(i=0;i<256;i++)
	{
		if((p8[0]<0x30)||(p8[0]>0x39))
		{
			break;
		}
		p8++;
	}
	if(i>255)
	{
		return 0;
	}
	for(i=0;i<256;i++)
	{
		if((p8[0]>=0x30)&&(p8[0]<=0x39))
		{
			break;
		}
		p8++;
	}
	if(i>255)
	{
		return 0;
	}	
	return ((u32)p8);
}

u32 IP_ASCII_HEX(u32 Addr)//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
{
	u32 ip;

	ip=0;
	//找到第1个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip=ASCII_HEX(Addr);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	//找到第2个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip+=ASCII_HEX(Addr)<<8;//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	//找到第3个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip+=ASCII_HEX(Addr)<<16;//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	//找到第4个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip+=ASCII_HEX(Addr)<<24;//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	return ip;
}

void IP4_HEX_ASCII(u8 *hex,u8 *ascii)
{
	u32 i;
	u32 x;
	
	for(x=0;x<4;x++)
	{
		i=*hex;
		i=hex_bcd(i);
		if(i<0x10)
		{
			ascii[0]=i+0x30;
			ascii[1]='.';
			ascii+=2;
			hex++;
			continue;
		}
		if(i<0x100)
		{
			ascii[0]=(i>>4)+0x30;
			ascii[1]=(i&0xf)+0x30;
			ascii[2]='.';
			ascii+=3;
			hex++;
			continue;
		}
		ascii[0]=(i>>8)+0x30;
		ascii[1]=((i>>4)&0xf)+0x30;
		ascii[2]=(i&0xf)+0x30;
		ascii[3]='.';
		ascii+=4;
		hex++;
	}
	ascii--;
	ascii[0]=0;
}


u64 YMDHM_Int15(u64 YMDHM)//年月日时分化为整15分,如00时18分化为00时15分
{
	u32 i;
	i=YMDHM;//集中器,专变等非电表类固定每15分存储
	i&=0xff;
	if(i<0x15)
	{
		i=0;
	}
	else
	{
		if(i<0x30)
		{
			i=0x15;
		}
		else
		{
			if(i<0x45)
			{
				i=0x30;
			}
			else
			{
				i=0x45;
			}
		}
	}
	YMDHM&=0xffffffff00;
	YMDHM|=i;
	return YMDHM;
}


//hex rtc
u32 Check_HMS_hex(u32 HMS)//时分秒合法检查,返回0合法,1非法
{
	if((HMS&0xff)>=60)
	{
		return(1);
	}
	HMS>>=8;
	if((HMS&0xff)>=60)
	{
		return(1);
	}
	HMS>>=8;
	if((HMS&0xff)>=24)
	{
		return(1);
	}
	return(0);
}
u32 Check_YYMD_hex(u32 YYMD)//年月日合法检查,返回0合法,1非法
{
	u32 Y;
	u32 M;
	u32 D;
	
	D=YYMD&0xff;
	M=(YYMD>>8)&0xff;
	Y=YYMD>16;
	if(D==0x0)
	{
		return(1);
	}
	if(M==0x0)
	{
		return(1);
	}
	if(M>12)
	{
		return(1);
	}
	if((Y%4)==0x0)
	{
		if(M==2)
		{
			if(D>29)
			{
				return(1);
			}
			else
			{
				return(0);
			}
		}
	}
	if(D>MothVSDate[M])
	{
		return(1);
	}
	return(0);
}

u32 Check_YYMDHMS_hex(u64 YYMDHMS)//年月日时分秒合法检查,返回0合法,1非法
{
	if(Check_HMS_hex(YYMDHMS)!=0x0)
	{
		return 1;
	}
	if(Check_YYMD_hex(YYMDHMS>>24)!=0x0)
	{
		return 1;
	}
	return 0;
}

u32 Check_pYYMDHMS_hex(u8* pYYMDHMS)//年年月日时分秒合法检查,返回0合法,1非法
{
	u32 i;
	
	i=(pYYMDHMS[0]<<24)|(pYYMDHMS[1]<<16)|(pYYMDHMS[2]<<8)|(pYYMDHMS[3]<<0);
	i=Check_YYMD_hex(i);//年月日合法检查,返回0合法,1非法
	if(i)
	{
		return 1;
	}
	//时
	if(pYYMDHMS[4]>24)
	{
		return 1;
	}
	//分
	if(pYYMDHMS[5]>60)
	{
		return 1;
	}
	//秒
	if(pYYMDHMS[6]>60)
	{
		return 1;
	}
	return 0;
}


u32 YYMDHMS_Sec_hex(u8* pRTC)//时钟对00年01月01日00时00分00秒的秒计数;年在低字节秒在高字节
{
	u32 i;
	u32 x;
	u32 s;

	x=(pRTC[0]<<8)+pRTC[1];//取年计算日数
	x%=100;
	if((x%4)==0x0)
	{//能被4整除的年
		s=(x*365)+(x/4);
		if(pRTC[2]>0x2)
		{//月>2
			s+=1;
		}
	}
	else
	{//非4整除的年
		s=(x*365)+(x/4)+1;
	}
	x=pRTC[2];//取月计算日数
	for(i=1;i<x;i++)
	{
		s+=MothVSDate[i];
	}
	s+=pRTC[3];//取日
	s-=1;//01月01日不算
	s*=(24*60*60);
	s+=(pRTC[4]*60*60);//时
	s+=(pRTC[5]*60);//分
	s+=pRTC[6];//秒
	return(s);
}

u32 YYMDHMS_Sub_hex(u8* pRTC1,u8* pRTC2)//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
{
	u32 s1;
	u32 s2;

	s1=0;
	s2=(pRTC1[4]<<16)+(pRTC1[5]<<8)+pRTC1[6];
	s1+=Check_HMS_hex(s2);
	s2=(pRTC1[0]<<24)+(pRTC1[1]<<16)+(pRTC1[2]<<8)+pRTC1[3];
	s1+=Check_YYMD_hex(s2);
	s2=(pRTC2[4]<<16)+(pRTC2[5]<<8)+pRTC2[6];
	s1+=Check_HMS_hex(s2);
	s2=(pRTC2[0]<<24)+(pRTC2[1]<<16)+(pRTC2[2]<<8)+pRTC2[3];
	s1+=Check_YYMD_hex(s2);
	if(s1!=0x0)
	{
		return(0);
	}
	s1=YYMDHMS_Sec_hex(pRTC1);
	s2=YYMDHMS_Sec_hex(pRTC2);
	return(s2-s1);
}

u32 YYMD_Add1D_hex(u32 YYMD)//年年月日加1日,返回加1后的年年月日
{
	u32 Y;
	u32 M;
	u32 D;

	D=YYMD&0xff;
	M=(YYMD>>8)&0xff;
	Y=(YYMD>>16);

	D+=1;
	if((Y%4)==0x0)
	{
		if(M==2)
		{
			if(D>29)
			{
				D=1;
				M+=1;
			}
		}
		else
		{
			if(D>MothVSDate[M])
			{
				D=1;
				M+=1;
			}
		}
	}
	else
	{
		if(D>MothVSDate[M])
		{
			D=1;
			M+=1;
		}
	}
	if(M>12)
	{
		M=1;
		Y+=1;
	}
	YYMD=D+(M<<8)+(Y<<16);
	return(YYMD);
}

u32 YYMD_Sub1D_hex(u32 YYMD)//年年月日减1日,返回减1后的年年月日
{
	u32 Y;
	u32 M;
	u32 D;

	D=YYMD&0xff;
	M=(YYMD>>8)&0xff;
	Y=YYMD>>16;

	D-=1;
	if(D==0x0)
	{
		M-=1;
		if(M==0x0)
		{
			M=12;
			if(Y)
			{
				Y-=1;
			}
			else
			{
				Y=2099;
			}
		}
		if((Y%4)==0x0)
		{
			if(M==2)
			{
				D=29;
			}
			else
			{
				D=MothVSDate[M];
			}
		}
		else
		{
			D=MothVSDate[M];
		}
	}
	YYMD=D+(M<<8)+(Y<<16);
	return(YYMD);
}

u32 YYM_AddM_hex(u32 YYM,u32 m)//年年月加m月,返回加m月后的年年月
{
	u32 Y;

	m+=YYM&0xff;
	Y=YYM>>8;
	Y+=m/12;
	m%=12;
	YYM=m+(Y<<8);
	return(YYM);
}

u32 YYM_SubM_hex(u32 YYM,u32 m)//年年月减m月,返回减m月后的年年月
{
	u32 Y;
	u32 M;

	M=YYM&0xff;
	Y=YYM>>8;
	if(M>=m)
	{
		M-=m;
	}
	else
	{
		Y-=m/12;
		m%=12;
		if(m>M)
		{
			Y-=1;
			M+=12;
		}
		M-=m;
		if(Y>2099)
		{
			Y=2099;
		}
	}
	YYM=M+(Y<<8);
	return(YYM);
}

u64 YYMDHM_AddM_hex(u64 YYMDHM,u32 m)//年年月日时分加m分,返回加m分后的年年月日时分
{
	u32 YMD;
	
	YMD=YYMDHM>>16;
	m+=YYMDHM&0xff;
	m+=((YYMDHM>>8)&0xff)*60;
	while(m>=(24*60))
	{
		YMD=YYMD_Add1D_hex(YMD);//年月日加1日,返回加1后的年月日
		m-=(24*60);
	}
	YYMDHM=YMD;
	YYMDHM<<=16;
	if(m>=60)
	{
		YYMDHM|=(m/60)<<8;
		m%=60;
	}
	YYMDHM|=m;
	return(YYMDHM);
}

u64 YYMDHM_SubM_hex(u64 YYMDHM,u32 m)//年年月日时分减m分,返回减m分后的年年月日时分
{
	u32 i;
	u32 YMD;
	
	YMD=YYMDHM>>16;
	while(m>=(24*60))
	{
		YMD=YYMD_Sub1D_hex(YMD);//年月日减1日,返回减1后的年月日
		m-=(24*60);
	}
	i=YYMDHM&0xff;
	i+=((YYMDHM>>8)&0xff)*60;
	if(i>=m)
	{
		m=i-m;
	}
	else
	{
		YMD=YYMD_Sub1D_hex(YMD);//年月日减1日,返回减1后的年月日
		i+=(24*60);
		m=i-m;
	}
	while(m>=(24*60))
	{
		YMD=YYMD_Add1D_hex(YMD);//年月日加1日,返回加1后的年月日
		m-=(24*60);
	}
	YYMDHM=YMD;
	YYMDHM<<=16;
	if(m>=60)
	{
		YYMDHM|=(m/60)<<8;
		m%=60;
	}
	YYMDHM|=m;
	return(YYMDHM);
}

u32 YYMDH_AddH_hex(u32 YYMDH,u32 h)//年年月日时加h时,返回加h时后的年年月日时
{
	u64 d64;
	
	d64=YYMDH;
	d64<<=8;
	h*=60;
	d64=YYMDHM_AddM_hex(d64,h);//年月日时分加m分,返回加m分后的年月日时分
	d64>>=8;
	return d64;
}

u32 YYMDH_SubH(u32 YYMDH,u32 h)//年年月日时减h时,返回减h时后的年年月日时
{
	u64 d64;
	
	d64=YYMDH;
	d64<<=8;
	h*=60;
	d64=YYMDHM_SubM_hex(d64,h);//年月日时分减m分,返回减m分后的年月日时分
	d64>>=8;
	return d64;
}

u64 YYMDHMS_AddS_hex(u64 YYMDHMS,u32 S)//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
{
	S+=YYMDHMS&0xff;
	YYMDHMS>>=8;
	YYMDHMS=YYMDHM_AddM_hex(YYMDHMS,S/60);//年月日时分加m分,返回加m分后的年月日时分
	YYMDHMS<<=8;
	S%=60;
	YYMDHMS|=S;
	return YYMDHMS;
}

u64 YYMDHMS_SubS_hex(u64 YYMDHMS,u32 S)//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
{
	u32 i;
	u32 m;
	
	i=YYMDHMS&0xff;
	m=S/60;
	S%=60;
	if(i<S)
	{
		m++;
		i+=60;
	}
	S=i-S;
	YYMDHMS>>=8;
	YYMDHMS=YYMDHM_SubM_hex(YYMDHMS,m);//年月日时分减m分,返回减m分后的年月日时分
	YYMDHMS<<=8;
	YYMDHMS|=S;
	return YYMDHMS;
}


void pYYMDHMS_AddS_hex(u8* pYYMDHMS,u32 S)//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
{
	u64 YYMDHMS;
	
	YYMDHMS=pYYMDHMS[0];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[1];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[2];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[3];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[4];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[5];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[6];
	S+=YYMDHMS&0xff;
	YYMDHMS>>=8;
	YYMDHMS=YYMDHM_AddM_hex(YYMDHMS,S/60);//年月日时分加m分,返回加m分后的年月日时分
	YYMDHMS<<=8;
	S%=60;
	YYMDHMS|=S;
	pYYMDHMS[6]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[5]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[4]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[3]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[2]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[1]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[0]=YYMDHMS;
	YYMDHMS>>=8;
}

void pYYMDHMS_SubS_hex(u8* pYYMDHMS,u32 S)//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
{
	u32 i;
	u32 m;
	u64 YYMDHMS;
	
	YYMDHMS=pYYMDHMS[0];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[1];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[2];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[3];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[4];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[5];
	YYMDHMS<<=8;
	YYMDHMS|=pYYMDHMS[6];
	i=YYMDHMS&0xff;
	m=S/60;
	S%=60;
	if(i<S)
	{
		m++;
		i+=60;
	}
	S=i-S;
	YYMDHMS>>=8;
	YYMDHMS=YYMDHM_SubM_hex(YYMDHMS,m);//年月日时分减m分,返回减m分后的年月日时分
	YYMDHMS<<=8;
	YYMDHMS|=S;
	YYMDHMS|=S;
	pYYMDHMS[6]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[5]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[4]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[3]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[2]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[1]=YYMDHMS;
	YYMDHMS>>=8;
	pYYMDHMS[0]=YYMDHMS;
	YYMDHMS>>=8;
}

void pYYM_AddM_hex(u8* pYYM,u32 m)//年年月加m月,返回加m月后的年年月
{
	u32 Y;
	
	m+=pYYM[2];
	Y=(pYYM[0]<<8)+pYYM[1];
	Y+=m/12;
	m%=12;
	pYYM[0]=Y>>8;
	pYYM[1]=Y;
	pYYM[2]=m;
}

void pYYM_SubM_hex(u8* pYYM,u32 m)//年年月减m月,返回减m月后的年年月
{
	u32 Y;
	u32 M;

	M=pYYM[2];
	Y=(pYYM[0]<<8)+pYYM[1];
	if(M>=m)
	{
		M-=m;
	}
	else
	{
		Y-=m/12;
		m%=12;
		if(m>M)
		{
			Y-=1;
			M+=12;
		}
		M-=m;
		if(Y>2099)
		{
			Y=2099;
		}
	}
	pYYM[0]=Y>>8;
	pYYM[1]=Y;
	pYYM[2]=M;
}

void bcdYMDHMS_hexYYMDHMS(u8* pBCD,u8* pHEX)//BCD的年月日时分秒转为HEX的年年月日时分秒
{
	u32 i;
	u32 x;
	u32 y;
	
	x=pBCD[0];
	y=x>>4;
	y*=10;
	x&=0xf;
	x+=y;
	x+=2000;
	pHEX[0]=x>>8;
	pHEX[1]=x;
	pBCD+=1;
	pHEX+=2;
	for(i=0;i<5;i++)
	{
		x=pBCD[0];
		y=x>>4;
		y*=10;
		x&=0xf;
		x+=y;
		pHEX[0]=x;
		pBCD+=1;
		pHEX+=1;
	}
}


