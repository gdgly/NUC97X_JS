
//���ݸ�ʽת��
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Calculate/DataConversion.h"
#include "../Calculate/Calculate.h"



u64 Data02_kW(u32 Data02)//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
{
	u32 x;
	u32 y;
	u64 d64;

	x=Data02>>13;
	y=Data02&0x1000;
	d64=Data02&0xfff;
	while(x<7)
	{
		x++;
		d64<<=4;
	}
	d64=bcd16_hex(d64);
	if(y!=0x0)
	{//��
		d64=(~d64)+1;
	}
	return d64;
}

u32 kW_Data02(u64 d64)//��λͳһΪkw��3λС����hex����תΪ���ݸ�ʽ02
{
	u32 x;
	u32 y;
	
	y=d64>>63;
	if(y!=0x0)
	{//���Ϊ��
		y=0x1000;
		d64=(~d64)+1;
	}
	d64=hex_bcd16(d64);//�ܼӹ���HEX,��3λС��
	x=7;//G3G2G1��3λС������
	while((d64>>12)!=0x0)
	{
		x-=1;
		d64>>=4;
	}
	x<<=13;
	x|=y;
	x|=d64;
	return x;
}


u64 Data03_kWh(u32 Data03)//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
{
	u64 d64;
	d64=bcd_hex(Data03&0xfffffff);
	if((Data03&0x40000000)!=0x0)
	{//mwh
		d64*=1000;
	}
	if((Data03&0x10000000)!=0x0)
	{//��
		d64=(~d64)+1;
	}
	return(d64);
}

u64 kWh_Data03(u64 d64)//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
{
	u32 i;
	u32 f;
	
	if((d64>>63)==0x0)
	{//��
		f=0;
	}
	else
	{//��
		f=1;
		d64=(~d64)+1;
	}
	if(d64>=10000000)
	{//MWh
		i=d64/1000;
		f|=0x4;
		i=hex_bcd(i);
		i|=f<<28;
		d64%=1000;
		d64<<=32;
		d64+=i;
	}
	else
	{//kWh
		i=d64;
		i=hex_bcd(i);
		i|=f<<28;
		d64=i;
	}
	return(d64);
}


u32 Data_0xFFto0xEE(u32 DataAddr,u32 Byte)//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
{
	u32 i;
	u8 * p8;
	p8=(u8*)DataAddr;
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xff)
		{
			return 0;
		}
	}
//ȫ0xFF���滻Ϊȫ0xEE
	for(i=0;i<Byte;i++)
	{
		p8[i]=0xee;
	}
	return 1;
}






