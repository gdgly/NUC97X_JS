
//�ն���2����Fn
#include "Project.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "KeyUser.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"




extern Terminal_Class2DataLib_TypeDef Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



u32 Class2Data_FnOnly(u32 ClassNo,u32 Fn)//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
{
	u32 i;
	u8 * p8;
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F39+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
	i=MRR((u32)p8,1);
	if(i>=((Fn/8)+1))
	{//��Ϣ������n(0��31)
		p8+=1;
		p8+=(Fn/8);
		i=MRR((u32)p8,1);
		i>>=(Fn%8);
		i&=0x1;
		return(i);
	}
	return(0);
}

u32 Class2Data_Fn(u32 ClassNo,u32 Fn)//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
{
	u32 i;
	u8 * p8;
	
	if(Fn==0x0)
	{
		return(0);
	}
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F39+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
	i=MRR((u32)p8,1);
	if(i>=((Fn/8)+1))
	{//��Ϣ������n(0��31)
		p8+=1;
		p8+=(Fn/8);
		i=MRR((u32)p8,1);
		i>>=(Fn%8);
		i&=0x1;
	}
	else
	{
		i=0;
	}
	if(i!=0x0)
	{
		return(1);
	}
	else
	{
		Fn++;
		switch(Fn)
		{
//F1 �ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 161://F161 �ն��������й�����ʾֵ���ܡ�����1��M��
			case 162://F162 �ն��������޹�������޹�1������ʾֵ���ܡ�����1��M��
			case 165://F165 �ն���һ�����޹�����ʾֵ���ܡ�����1��M��
			case 168://F168 �ն����������޹�����ʾֵ���ܡ�����1��M��
				i=Class2Data_FnOnly(ClassNo,1);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F2 �ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 163://F163 �ն��ᷴ���й�����ʾֵ���ܡ�����1��M��
			case 164://F164 �ն��ᷴ���޹�������޹�2������ʾֵ���ܡ�����1��M��
			case 166://F166 �ն���������޹�����ʾֵ���ܡ�����1��M��
			case 167://F167 �ն����������޹�����ʾֵ���ܡ�����1��M��
				i=Class2Data_FnOnly(ClassNo,2);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F3 �ն���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 185://�ն��������й��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 186://�ն��������޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				i=Class2Data_FnOnly(ClassNo,3);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F4 �ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 187://�ն��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 188://�ն��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				i=Class2Data_FnOnly(ClassNo,4);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F17 �¶���������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 177://�¶��������й�����ʾֵ���ܡ�����1��M��
			case 178://�¶��������޹�������޹�1������ʾֵ���ܡ�����1��M��
			case 181://�¶���һ�����޹�����ʾֵ���ܡ�����1��M��
			case 184://�¶����������޹�����ʾֵ���ܡ�����1��M��
				i=Class2Data_FnOnly(ClassNo,17);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F18 �¶��ᷴ����/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 179://�¶��ᷴ���й�����ʾֵ���ܡ�����1��M��
			case 180://�¶��ᷴ���޹�������޹�2������ʾֵ���ܡ�����1��M��
			case 182://�¶���������޹�����ʾֵ���ܡ�����1��M��
			case 183://�¶����������޹�����ʾֵ���ܡ�����1��M��
				i=Class2Data_FnOnly(ClassNo,18);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F19 �¶���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 193://�¶��������й��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 194://�¶��������޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				i=Class2Data_FnOnly(ClassNo,19);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
//F20 �¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 195://�¶��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 196://�¶��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				i=Class2Data_FnOnly(ClassNo,20);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				return(i);
		}
	}

	return(0);
}


u32 GetClass2DataOfficeAddr(u32 Fn,u32 Pn)//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
{
	u32 Addr;
	
	if(Fn>=Get_Class2Data_MaxFn())
	{
		return(0);
	}
	if(Pn>Terminal_Class2DataLib[Fn].PnMax)
	{
		if(Fn==246)
		{//AFN0DF246��ǰ�����¼����ӳ���Pn
			if(Pn>MaxRS485AddCarrierPn)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	if((Terminal_Class2DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//�������޴�Fn����
		return(0);
	}
	if(Terminal_Class2DataLib[Fn].PnType==0x0)
	{
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	}
	return(Addr);
}

u32 GetClass2DataF246OfficeAddr(u32 CongealNo,u32 Pn)//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
{
	u32 i;
	u32 x;
	u32 Addr;
	
	Addr=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+ADDR_AFN0DF246Pn;
	for(i=0;i<F246MaxPn;i++)
	{
		x=MRR(Addr+(2*i),2);
		if(x==Pn)
		{
			Addr=Terminal_Class2DataLib[246].OfficeAddr+(Terminal_Class2DataLib[246].PnLen*i);
			return(Addr);
		}
	}
	return 0;
}

u32 GetClass2DataOfficeAddr_MapPn(u32 Fn,u32 Pn)//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
{
	u32 Addr;
	
	if(Fn>=Get_Class2Data_MaxFn())
	{
		return(0);
	}
//	if(Pn>Terminal_Class2DataLib[Fn].PnMax)
//	{
//		return(0);
//	}
	if((Terminal_Class2DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//�������޴�Fn����
		return(0);
	}
	if(Terminal_Class2DataLib[Fn].PnType==0x0)
	{
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	}
	return(Addr);
}


u32 GetClass2DataCongealType(u32 Fn)//ȡ��2���ݶ�������
{
	return(Terminal_Class2DataLib[Fn].CongealType);
}

u32 GetClass2DataLen(u32 Fn,u32 Pn,u32 DataAddr)//ȡ��2���ݳ���
{
	u32 Len;
	u32 *p32;
	u32 Tariff;
	
	p32=(u32*)(ADDR_Terminal_ClassDataLen);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���(ԭ2������ֱ�Ӹ�дFLASH,��������29GLд��0.5������ȫ��������̫��̫��ʱ��,�����ӷ��ʼĴ���)
	p32[0]=0;
#if DefaultTariff==0//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
		Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
	#else
		Tariff=TMaxTariff;
	#endif
#endif
	switch(Fn)
	{
		case 1://�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
		case 2://�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
		case 9://�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
		case 10://�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
		case 17://�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
		case 18://�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#if AFN0DF1MaxPn==0
		//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�,�ϳɺ����ݷ�ADDR_DATABUFF
			Len=MRR(ADDR_DATABUFF+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#endif
			Len=6+(17*(Len+1));
#else
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			Len=MRR(DataAddr+5,1);
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(17*(Len+1));
#endif
			break;
		case 3://�ն���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
		case 4://�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
		case 11://�����ն���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
		case 12://�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
		case 19://�¶���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
		case 20://�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��	
#if AFN0DF1MaxPn==0
		//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�,�ϳɺ����ݷ�ADDR_DATABUFF
			Len=MRR(ADDR_DATABUFF+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#endif
			Len=6+(14*(Len+1));
#else
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(14*(Len+1));
#endif
			break;
		case 5://�ն��������й����������ܡ�����1��M��
		case 6://�ն��������޹����������ܡ�����1��M��
		case 7://�ն��ᷴ���й����������ܡ�����1��M��
		case 8://�ն��ᷴ���޹����������ܡ�����1��M��
		case 21://�¶��������й����������ܡ�����1��M��
		case 22://�¶��������޹����������ܡ�����1��M��
		case 23://�¶��ᷴ���й����������ܡ�����1��M��
		case 24://�¶��ᷴ���޹����������ܡ�����1��M��
	#if AFN0DF5MaxPn==0
		//����ĵ���������Ҫ�̶�,����ȡͨ�ų��Ȳ�ȷ��
		#if DefaultTariff==0//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			Tariff=TMaxTariff;
		#endif
			Len=Tariff;//�̶��ķ�����
			p32[0]=0x10000+Len;
			Len=1+(4*(Len+1));
	#else
			Len=MRR(DataAddr,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10000+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10000+Len;
			}
		#endif
			Len=1+(4*(Len+1));
	#endif
			break;
		case 58://�ն����ܼ������ۼ��й����������ܡ�����1��M��
		case 59://�ն����ܼ������ۼ��޹����������ܡ�����1��M��
		case 61://�¶����ܼ������ۼ��й����������ܡ�����1��M��
		case 62://�¶����ܼ������ۼ��޹����������ܡ�����1��M��
			Len=MRR(DataAddr,1);
			if(Len>TMaxTariff)
			{
				Len=0;//TMaxTariff;
				p32[0]=0x10000+Len;
			}
			Len=1+(4*(Len+1));
			break;
		case 121://�ն���A��г��Խ����ͳ������
		case 122://�ն���B��г��Խ����ͳ������
		case 123://�ն���C��г��Խ����ͳ������
			Len=MRR(DataAddr,1);
			if(Len>19)
			{
				p32[0]=0x10000+19;
			}
			Len=Terminal_Class2DataLib[Fn].PnLen;
			break;
		case 161://�ն��������й�����ʾֵ���ܡ�����1��M��
		case 163://�ն��ᷴ���й�����ʾֵ���ܡ�����1��M��
		case 169://�����ն��������й�����ʾֵ���ܡ�����1��M��
		case 171://�����ն��ᷴ���й�����ʾֵ���ܡ�����1��M��
		case 177://�¶��������й�����ʾֵ���ܡ�����1��M��
		case 179://�¶��ᷴ���й�����ʾֵ���ܡ�����1��M��
		case 201://�¶����1ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 202://�¶����2ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 203://�¶����3ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 204://�¶����4ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 205://�¶����5ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 206://�¶����6ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 207://�¶����7ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 208://�¶����8ʱ�����������й�����ʾֵ���ܡ�����1��M��
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(5*(Len+1));
			break;
		case 162://�ն��������޹�������޹�1������ʾֵ���ܡ�����1��M��
		case 164://�ն��ᷴ���޹�������޹�1������ʾֵ���ܡ�����1��M��
		case 165://�ն���1�����޹�����ʾֵ���ܡ�����1��M��
		case 166://�ն���2�����޹�����ʾֵ���ܡ�����1��M��
		case 167://�ն���3�����޹�����ʾֵ���ܡ�����1��M��
		case 168://�ն���4�����޹�����ʾֵ���ܡ�����1��M��
		case 170://�����ն��������޹�������޹�1������ʾֵ���ܡ�����1��M��
		case 172://�����ն��ᷴ���޹�������޹�1������ʾֵ���ܡ�����1��M��
		case 173://�����ն���1�����޹�����ʾֵ���ܡ�����1��M��
		case 174://�����ն���2�����޹�����ʾֵ���ܡ�����1��M��
		case 175://�����ն���3�����޹�����ʾֵ���ܡ�����1��M��
		case 176://�����ն���4�����޹�����ʾֵ���ܡ�����1��M��
		case 178://�¶��������޹�������޹�1������ʾֵ���ܡ�����1��M��
		case 180://�¶��ᷴ���޹�������޹�1������ʾֵ���ܡ�����1��M��
		case 181://�¶���1�����޹�����ʾֵ���ܡ�����1��M��
		case 182://�¶���2�����޹�����ʾֵ���ܡ�����1��M��
		case 183://�¶���3�����޹�����ʾֵ���ܡ�����1��M��
		case 184://�¶���4�����޹�����ʾֵ���ܡ�����1��M��
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(4*(Len+1));
			break;
		case 185://�ն��������й��������������ʱ�䣨�ܡ�����1��M��
		case 186://�ն��������޹��������������ʱ�䣨�ܡ�����1��M��
		case 187://�ն��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
		case 188://�ն��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��
		case 189://�����ն��������й��������������ʱ�䣨�ܡ�����1��M��
		case 190://�����ն��������޹��������������ʱ�䣨�ܡ�����1��M��
		case 191://�����ն��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
		case 192://�����ն��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��
		case 193://�¶��������й��������������ʱ�䣨�ܡ�����1��M��
		case 194://�¶��������޹��������������ʱ�䣨�ܡ�����1��M��
		case 195://�¶��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
		case 196://�¶��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(7*(Len+1));
			break;
		case 216://�¶�����ܱ������Ϣ
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(10*(Len+1))+5;
			break;
			
		default:
			Len=Terminal_Class2DataLib[Fn].PnLen;
			break;
	}
	return(Len);
}

u32 GetDateCongealNo(u32 YMD)//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
{
	u32 i;
	u32 x;
	
	x=MRR(ADDR_TCongealCount+14,1);
	if((x&0x80)!=0x0)
	{
		x=MaxDateCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMD==MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),3))
		{
			return i;//�ҵ�
		}
	}
	return 0xffffffff;
}
u32 GetDateCongealAddr(u32 YMD,u32 Fn,u32 Pn)//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
{
	u32 i;
	u32 x;
	i=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{
		return 0;
	}
	if(Fn==246)
	{//�ն�������¼����(������ӳ��)
		x=GetClass2DataF246OfficeAddr(i,Pn);//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	}
	else
	{
		x=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	}
	if(x==0)
	{
		return 0;
	}
	return (ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+x);
}

u32 GetRMDateCongealNo(u32 YMD)//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
{
	u32 i;
	u32 x;

	x=MRR(ADDR_TCongealCount+21,1);
	if((x&0x80)!=0x0)
	{
		x=MaxRMDateCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMD==MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3))
		{
			return i;//�ҵ�
		}
	}
	return 0xffffffff;
}
u32 GetRMDateCongealAddr(u32 YMD,u32 Fn,u32 Pn)//�õ������ն���洢��ַ,����=0��ʾû�ҵ�
{
	u32 i;
	u32 x;
	i=GetRMDateCongealNo(YMD);//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{
		return 0;
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(x==0)
	{
		return 0;
	}
	return (ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+x);
}

u32 GetMonthCongealNo(u32 YM)//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
{
	u32 i;
	u32 x;

	x=MRR(ADDR_TCongealCount+28,1);
	if((x&0x80)!=0x0)
	{
		x=MaxMonthCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YM==MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),2))
		{
			return i;//�ҵ�
		}
	}
	return 0xffffffff;
}
u32 GetMonthCongealAddr(u32 YM,u32 Fn,u32 Pn)//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
{
	u32 i;
	u32 x;
	i=GetMonthCongealNo(YM);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{
		return 0;
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(x==0)
	{
		return 0;
	}
	return (ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+x);
}
u32 GetCurveCongealNo(u64 YMDHM)//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
{
	u32 i;
	u32 x;
	
	x=MRR(ADDR_TCongealCount+7,2);
	if(x&0x8000)
	{
		x=MaxCurveCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMDHM==MRR(ADDR_CurveCongealList+2+(6*i),5))
		{
			return i;//�ҵ�
		}
	}
	return 0xffffffff;
}

u32 GetCurveCongealAddr(u64 YMDHM,u32 Fn,u32 Pn)//�õ����߶���洢��ַ,����=0��ʾû�ҵ�
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	CongealNo=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return 0;
	}
	MapPn=KeyUserMapPn(Pn);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return 0;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
		//�鶳���б��Ƿ��пտ���
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//�пտ���
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				break;
			}
		}
		if(MapPn>=MaxKeyUser)
		{//û����
			return 0;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return 0;
	}
	return (ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr);
}

u32 GetACSampleCurveCongealAddr(u64 YMDHM,u32 Fn)//�õ����ɲ��������߶���洢��ַ,����=0��ʾû�ҵ�
{
#if MaxACSampleCurveCongeal==0//�ܶ������
	return 0;
#else
	u32 i;
	u8 *p8;
	u64 d64;
	
	MC(0,ADDR_DATABUFF,7);
	p8=(u8*)(ADDR_DATABUFF);
	p8[3]=1;//00��01��01��00ʱ00��00��
	p8[4]=1;
	MWR(YMDHM,ADDR_DATABUFF+7,5);
	i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
	i/=60;
	i%=MaxACSampleCurveCongeal;//ƫ�Ƶ���
	i*=LEN_ACSampleCurve;//ƫ�Ƶ�ַ
	i+=ADDR_ACSampleCurve;//��ַ
	d64=MRR(i,5);
	if(d64==YMDHM)
	{//ʱ����ͬ
		switch(Fn)
		{
			case 89://A���ѹ����
				return i+5;
			case 90://B���ѹ����
				return i+7;
			case 91://C���ѹ����
				return i+9;
		}
	}
	return 0;
#endif
}


u32 CheckCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn)//������߶���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	CongealNo=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	CongealNo=PnCurveCongealNo(CongealNo,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return 2;
	}
	MapPn=KeyUserMapPn(Pn);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return 2;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
		//�鶳���б��Ƿ��пտ���
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//�пտ���
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				return 0;//����:0=����û����
			}
		}
		if(MapPn>=MaxKeyUser)
		{//û����
			return 2;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//��ͬʱ���ѳ�
		return 1;
	}
	return 0;
}

void AddCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//�����߶��������
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;

	CongealNo=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return;
	}
	MapPn=KeyUserMapPn(Pn);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
		//�鶳���б��Ƿ��пտ���
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//�пտ���
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				break;
			}
		}
		if(MapPn>=MaxKeyUser)
		{//û����
			return;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,Byte);
}

u64 CurveCongeal_YMDHM(void)//��ǰ��׼���߶���ʱ��YMDHM
{
	u32 i;
	u64 YMDHM;
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=CurveCongealTime;
	i*=CurveCongealTime;
	i=hex_bcd(i);
	YMDHM=MRR(ADDR_TYMDHMS+2,4);
	YMDHM<<=8;
	YMDHM|=i;//��ǰ��׼���߶�����YMDHM
	return YMDHM;
}

u32 CheckDateCongeal(u32 YMD,u32 Fn,u32 Pn)//����ն���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	CongealNo=PnDateCongealNo(CongealNo,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return 2;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//��ͬʱ���ѳ�
		return 1;
	}
	return 0;
}

void AddDateCongeal(u32 YMD,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//���ն��������
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+OfficeAddr,Byte);
}

u32 CheckMonthCongeal(u32 YM,u32 Fn,u32 Pn)//����¶���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetMonthCongealNo(YM);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	CongealNo=PnMonthCongealNo(CongealNo,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return 2;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//��ͬʱ���ѳ�
		return 1;
	}
	return 0;
}

void AddMonthCongeal(u32 YM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//���¶��������
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetMonthCongealNo(YM);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(CongealNo==0xffffffff)
	{//û�ҵ���ͬʱ��Ķ����
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*CongealNo)+OfficeAddr,Byte);
}









//2������.�ն���
//F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//2������.�����ն���
//F9������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F10�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//2������.�¶���
//F17������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F18�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
void Class2Group_F1_F2_F9_F10_F17_F18(u32 CongealAddr,u32 Pn,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4)//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 RMTime;
	u8 * p8;
	u32 Tariff;
#if DefaultTariff==0//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
		Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
	#else
		Tariff=TMaxTariff;
	#endif
#endif

	RMTime=0;//����ʱ�������־,0=û,1=����
	p8=(u8*)ADDR_DATABUFF;
	//�����й�
	i=GetClass2DataOfficeAddr(Fn1,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(i==0)
	{//û�ҵ�
		MC(0xee,ADDR_DATABUFF,6+5);
		x=Tariff;//������
		p8[5]=x;//������
	}
	else
	{//�ҵ�
		x=MRR(CongealAddr+i+5,1);//������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
		if(x!=Tariff)
		{
			x=Tariff;//������
		}
	#else
		if(x>TMaxTariff)
		{
			x=Tariff;//������
		}
	#endif
		MR(ADDR_DATABUFF,CongealAddr+i,6+((x+1)*5));
		Data_0xFFto0xEE(ADDR_DATABUFF,6+((x+1)*5));//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		p8[5]=x;//������
		if(p8[0]!=0xee)
		{
			RMTime=1;//����ʱ�������־,0=û,1=����
		}
	}
	//�����޹�������޹�1��
	i=GetClass2DataOfficeAddr(Fn2,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(i==0)
	{//û�ҵ�
		MC(0xee,ADDR_DATABUFF+6+((x+1)*5),(x+1)*4);
	}
	else
	{//�ҵ�
		MR(ADDR_DATABUFF+6+((x+1)*5),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*5),(x+1)*4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		if(RMTime==0)//����ʱ�������־,0=û,1=����
		{
			if(p8[6+((x+1)*5)]!=0xee)
			{
				RMTime=1;//����ʱ�������־,0=û,1=����
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
	//һ�����޹�
	i=GetClass2DataOfficeAddr(Fn3,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(i==0)
	{//û�ҵ�
		MC(0xee,ADDR_DATABUFF+6+((x+1)*9),(x+1)*4);
	}
	else
	{//�ҵ�
		MR(ADDR_DATABUFF+6+((x+1)*9),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*9),(x+1)*4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		if(RMTime==0)//����ʱ�������־,0=û,1=����
		{
			if(p8[6+((x+1)*9)]!=0xee)
			{
				RMTime=1;//����ʱ�������־,0=û,1=����
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
	//�������޹�
	i=GetClass2DataOfficeAddr(Fn4,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(i==0)
	{//û�ҵ�
		MC(0xee,ADDR_DATABUFF+6+((x+1)*13),(x+1)*4);
	}
	else
	{//�ҵ�
		MR(ADDR_DATABUFF+6+((x+1)*13),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*13),(x+1)*4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		if(RMTime==0)//����ʱ�������־,0=û,1=����
		{
			if(p8[6+((x+1)*13)]!=0xee)
			{
				RMTime=1;//����ʱ�������־,0=û,1=����
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
}

//2������.�ն���
//F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//2������.�����ն���
//F11������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//F12�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//2������.�¶���
//F19������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//F20�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
void Class2Group_F3_F4_F11_F12_F19_F20(u32 CongealAddr,u32 Pn,u32 Fn1,u32 Fn2)//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 y;
	u32 RMTime;
	u8 * p8;
	u32 Tariff;
#if DefaultTariff==0//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
		Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
	#else
		Tariff=TMaxTariff;
	#endif
#endif

	RMTime=0;//����ʱ�������־,0=û,1=����
	p8=(u8*)ADDR_DATABUFF;
	//�й�
	y=GetClass2DataOfficeAddr(Fn1,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(y==0)
	{//û�ҵ�
		MC(0xee,ADDR_DATABUFF,6+7);
		x=Tariff;//������
		p8[5]=x;//������
	}
	else
	{//�ҵ�
		x=MRR(CongealAddr+y+5,1);//������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
		if(x!=Tariff)
		{
			x=Tariff;//������
		}
	#else
		if(x>TMaxTariff)
		{
			x=Tariff;//������
		}
	#endif
		MR(ADDR_DATABUFF,CongealAddr+y,6);
		Data_0xFFto0xEE(ADDR_DATABUFF,6);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(3*i),CongealAddr+y+6+(7*i),3);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(3*i),3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		}
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(3*(x+1)+(4*i)),CongealAddr+y+6+(7*i)+3,4);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(3*(x+1)+(4*i)),4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		}
		p8[5]=x;//������
		if(p8[0]!=0xee)
		{
			RMTime=1;//����ʱ�������־,0=û,1=����
		}
	}
	//�޹�
	y=GetClass2DataOfficeAddr(Fn2,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(y==0)
	{//û�ҵ�
		MC(0xee,ADDR_DATABUFF+6+((x+1)*7),(x+1)*7);
	}
	else
	{//�ҵ�
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(7*(x+1))+(3*i),CongealAddr+y+6+(7*i),3);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(7*(x+1))+(3*i),3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		}
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(10*(x+1)+(4*i)),CongealAddr+y+6+(7*i)+3,4);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(10*(x+1)+(4*i)),4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE
		}
		if(RMTime==0)//����ʱ�������־,0=û,1=����
		{
			i=MRR(CongealAddr+y,1);
			if((i!=0xee)&&(i!=0xff))
			{
				RMTime=1;//����ʱ�������־,0=û,1=����
				MR(ADDR_DATABUFF,CongealAddr+y,5);
			}
		}
	}
}

void Class2_Group(u32 CongealAddr,u32 Fn,u32 Pn)//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
{
	switch(Fn)
	{
	//2������.�ն���
		case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,161,162,165,168);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,163,164,166,167);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,185,186);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,187,188);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
	//2������.�����ն���
		case 9://F9������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,169,170,173,176);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 10://F10�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,171,172,174,175);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 11://F11������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,189,190);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 12://F12�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,191,192);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
	//2������.�¶���
		case 17://F17������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,177,178,181,184);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 18://F18�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,179,180,182,183);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 19://F19������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,193,194);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
		case 20://F20�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,195,196);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			break;
	}
}

void UART_AFN0DF97F98F99F100_Computer(u32 ADDR_FullData,u64 YMDHM,u32 m,u32 Fn,u32 Pn,u32 CongealNo)//2���������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵ����Fn
{
	u32 x;
	u32 y;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	MC(0xee,ADDR_FullData,4);//������
	p16=(u16 *)(ADDR_PnVSSetList);
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	for(x=0;x<MaxKeyUser;x++)
	{
		if(Pn==p16[x])
		{
			break;
		}
	}
	if(x>=MaxKeyUser)
	{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
		return;
	}
	MapPn=x+1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(OfficeAddr!=0)
	{
		x=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,4);
		if((x!=0xeeeeeeee)&&(x!=0xffffffff))
		{
			YMDHM=YMDHM_SubM(YMDHM,m);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
			CongealNo=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
			CongealNo=PnCurveCongealNo(CongealNo,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
			if(CongealNo!=0xffffffff)
			{//�ҵ���ͬʱ��Ķ����
				MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
				for(y=0;y<MaxKeyUser;y++)
				{
					if(Pn==p16[y])
					{
						break;
					}
				}
				if(y>=MaxKeyUser)
				{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
					return;
				}
				MapPn=y+1;
				OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҽ
				if(OfficeAddr!=0)
				{
					y=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,4);
					if((y!=0xeeeeeeee)&&(y!=0xffffffff))
					{
						x=bcd_hex(x);
						y=bcd_hex(y);
						if(x<y)
						{//����ֵ��0
							x+=100000000;
						}
						x-=y;
						x=hex_bcd(x);
						x<<=8;//������Ϊ4λС��
						MWR(x,ADDR_FullData,4);
					}
				}
			}
		}
	}
}


#if ((RMM_DateCongeal_0xEE!=0)||(RMM_MonthCongeal_0xEE!=0))//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
void UART_EntreyIncComputer(u32 Pn,u32 ADDR_FullData,u32 Addr1,u32 Addr2,u32 Len,u32 Tariff)//2���ն������ݵ���������
{
	u32 x;
	u32 y;
	u64 a64;
	u64 b64;
	
	for(x=0;x<=Tariff;x++)
	{
		//ȡ��ǰֵ
		b64=MRR(Addr1+6+(Len*x),Len);
		//ȡ����ֵ
		a64=MRR(Addr2+6+(Len*x),Len);
		y=a64&0xff;
		if((y==0xee)||(y==0xff))
		{//ԭʼֵ=��
			continue;
		}
		y=b64&0xff;
		if((y==0xee)||(y==0xff))
		{//��ǰֵ=��
			continue;
		}
		//��������
		a64=bcd16_hex(a64);
		b64=bcd16_hex(b64);
		if(b64>=a64)
		{//��ǰֵ>=ԭʼֵ
			b64-=a64;
		}
		else
		{
			if(Len==5)
			{
				if(a64>9000000000)
				{//��ǰֵ��0
					b64+=10000000000;
					b64-=a64;
				}
				else
				{//����
					b64=0;
				}
			}
			else
			{
				if(a64>90000000)
				{//��ǰֵ��0
					b64+=100000000;
					b64-=a64;
				}
				else
				{//����
					b64=0;
				}
			}
		}
		b64=hex_bcd16(b64);
		y=b64;
		if(Len==4)
		{
			y<<=8;//С�����4λ
		}
		MWR(y,ADDR_FullData+1+(4*x),4);
	}
}
void UART_AFN0DF5F6F7F8_Computer(u32 ADDR_FullData,u32 YMD,u32 Fn,u32 Pn,u32 Len)//2���ն������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
{
	u32 Addr1;
	u32 Addr2;
	u32 Tariff;//�������̶�Ϊ���÷���

//����ĵ���������Ҫ�̶�,����ȡͨ�ų��Ȳ�ȷ��
#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
	Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
#else
	Tariff=TMaxTariff;
#endif
	MC(0xee,ADDR_FullData,1+(4*(Tariff+1)));//������
	Addr1=GetDateCongealAddr(YMD,Fn,Pn);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
	if(Addr1==0)
	{
		return;
	}
	YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
	Addr2=GetDateCongealAddr(YMD,Fn,Pn);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
	if(Addr2==0)
	{
		return;
	}
	UART_EntreyIncComputer(Pn,ADDR_FullData,Addr1,Addr2,Len,Tariff);//2���ն������ݵ���������
}
void UART_AFN0DF21F22F23F24_Computer(u32 ADDR_FullData,u32 YM,u32 Fn,u32 Pn,u32 Len)//2���¶������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
{
	u32 Addr1;
	u32 Addr2;
	u32 Tariff;//�������̶�Ϊ���÷���

//����ĵ���������Ҫ�̶�,����ȡͨ�ų��Ȳ�ȷ��
#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
	Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
#else
	Tariff=TMaxTariff;
#endif
	MC(0xee,ADDR_FullData,1+(4*(Tariff+1)));//������
	Addr1=GetMonthCongealAddr(YM,Fn,Pn);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
	if(Addr1==0)
	{
		return;
	}
	YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
	Addr2=GetMonthCongealAddr(YM,Fn,Pn);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
	if(Addr2==0)
	{
		return;
	}
	UART_EntreyIncComputer(Pn,ADDR_FullData,Addr1,Addr2,Len,Tariff);//2���ն������ݵ���������
}
#endif


void SaveClass2Data(u32 PORTn,u32 Pn,u32 FnDataAddr,u8 *p8RMLib)//������2�����㳭������
{
	u32 i;
	u32 x;
	u32 y;
	u32 Addr;
	u8 *p8FnData;
	u32 Fn;
	u32 YM;
	u32 YMD;
	u64 YMDHM;
	#if (RMM_DateCongeal_0xEE==0)//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	u32 f;
	u32 *p32;
	#endif
	#if (USER/100)==2//�û���ʶ:ɽ����
	u64 LastDayEnergy;
	u64 TodayEnergy;
	#endif
	
	x=x;
	y=y;
	if((u32)p8RMLib==0)
	{
		return;
	}
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	p8FnData=(u8*)(FnDataAddr);
	i=p8RMLib[0];
	if((i==0xe3)||(i==0xee)||(i==0xef))
	{
		if(p8FnData[0]!=1)
		{//�������ڴ������DL645-1997��
			return;
		}
	}
	else
	{
		if(p8FnData[6]==0xee)
		{//������Ч
			return;
		}
	}

#if (RMM_CompCongealTime==0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
	//���Ƚ�
	YMD=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰʱ��������
	YM=YMD>>8;
	YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
	switch(i)
	{
		case 0xe3://��1�յ���ն���
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			Addr=GetClass2SourceDataAddr(161,Pn)-3;//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			break;
		case 0xee://��2�յ���ն���
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			Addr=ADDR_LastDate_Source_Start+((Pn-1)*LEN_LastDate_Source_PerPn);
			break;
		case 0xef://��3�յ���ն���
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			Addr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+((Pn-1)*LEN_LastDate_Source_PerPn);
			break;
		default:
			Addr=0;
			break;
	}
#else//
	//Ҫ�Ƚ�
	YMD=MRR(((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
	YM=YMD>>8;
	YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
	YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
	switch(i)
	{
		case 0xe3://��1�յ���ն���
		case 0xee://��2�յ���ն���
		case 0xef://��3�յ���ն���
			Addr=1;
			break;
		default:
			Addr=0;
			break;
	}
#endif

	if(Addr!=0)
	{//��0xe3,0xee,0xef��1-3�յ���ն���
	#if (USER/100)==2//�û���ʶ:ɽ����
		if(p8RMLib[0]==0xE3)
		{//���г����ն����������޹�,4�������޹�����ʾֵ���ܡ�����1��M��
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f=GetClass2SourceDataAddr(161,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			//�ж�����Ϊ�ն�������ĵ��ܱ����,����
			i=MRR(f+2,3);//ȡԭ����������
			if(i==YMD)
			{//������2�ճ���
				if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
				{//��ǰ���������й�����ʾֵ��Ч
					TodayEnergy=MRR(FnDataAddr+6+Office_AFN0DF161_LastDate_Source+6,5);
					LastDayEnergy=MRR(f+6,5);
					if((LastDayEnergy&0xff)!=0xff)//���г�����2���������¶���ԭʼ����(��ʼֵΪ����ֵ0xFF)
					{
						DayAveragePower(Pn,0,0);//��ƽ�����й����ʼ���;���Pn��1��ʼ,Power=�й�����(BCD),Count=���μ������
						ERC29_Event_Day(Pn,LastDayEnergy,TodayEnergy);//�ж�����Ϊ�ն�������ĵ��ܱ����,����;���Pn=�������(1��ʼ),LastDayEnergy=���Ƕ������,ToDayEnergy=���ն������
					}
				}
			}
		#else
			if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
			{//��ǰ���������й�����ʾֵ��Ч
				i=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
				i=GetDateCongealNo(i);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
				if(i!=0xffffffff)
				{//�ҵ���ͬʱ��Ķ����
					f=GetClass2DataOfficeAddr(161,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
					if(f!=0)
					{
						TodayEnergy=MRR(FnDataAddr+6+Office_AFN0DF161_LastDate_Source+6,5);
						LastDayEnergy=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+f,5);
						DayAveragePower(Pn,0,0);//��ƽ�����й����ʼ���;���Pn��1��ʼ,Power=�й�����(BCD),Count=���μ������
						ERC29_Event_Day(Pn,LastDayEnergy,TodayEnergy);//�ж�����Ϊ�ն�������ĵ��ܱ����,����;���Pn=�������(1��ʼ),LastDayEnergy=���Ƕ������,ToDayEnergy=���ն������
					}
				}
			}
		#endif
		}
	#endif
	
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		MR(ADDR_DATABUFF,Addr,3+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//��ԭԭʼ����
		MR(ADDR_DATABUFF,ADDR_TYMDHMS+3,3);//�ն˳���������
		f=0;//���±�־
		i=ADDR_DATABUFF+3;
	#endif
		//F161 �����������й�����ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF161_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,161,Pn,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(5*(1+TMaxTariff));
		//F162 �����������޹�������޹�1������ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF162_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,162,Pn,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F163 �����㷴���й�����ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF163_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,163,Pn,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(5*(1+TMaxTariff));
		//F164 �����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF164_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,164,Pn,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F165 ������1�����޹�����ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF165_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,165,Pn,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F166 ������2�����޹�����ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF166_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,166,Pn,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F167 ������3�����޹�����ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF167_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,167,Pn,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));//���ն��������
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F168 ������4�����޹�����ʾֵ���ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF168_LastDate_Source+2,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,168,Pn,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));//���ն��������
		#endif
		}
	#if (((USER/100)==5)&&(Project==300))//�Ϻ�ר��
		//F185 �����й��������������ʱ�䣨�ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF185_LastDate_Source]!=0xee)
		{
			AddDateCongeal(YMD,185,Pn,FnDataAddr+6+Office_AFN0DF185_LastDate_Source,6+(7*(1+TMaxTariff)));//���ն��������
		}
		//F187 �����й��������������ʱ�䣨�ܡ�����1��M��
		if(p8FnData[6+Office_AFN0DF187_LastDate_Source]!=0xee)
		{
			AddDateCongeal(YMD,187,Pn,FnDataAddr+6+Office_AFN0DF187_LastDate_Source,6+(7*(1+TMaxTariff)));//���ն��������
		}
	#endif
		
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		if(f!=0)
		{//����
			MW(ADDR_DATABUFF,Addr,3+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//дԭʼ����
		}
	#endif
		
	#if RMM_MonthCongealUserDate1==1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
		//���������³�1������תΪ�¶���ԭʼ����
		if(p8FnData[6]==1)
		{//1������
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			Addr=GetClass2SourceDataAddr(177,Pn)-2;//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			MR(ADDR_DATABUFF,Addr,2+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//��ԭԭʼ����
			MR(ADDR_DATABUFF,ADDR_TYMDHMS+4,2);//�ն˳�������
			f=0;//���±�־
			i=ADDR_DATABUFF+2;
		#endif
			//F177 �����������й�����ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF161_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,177,Pn,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(5*(1+TMaxTariff));
			//F178 �����������޹�������޹�1������ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF162_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,178,Pn,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F179 �����㷴���й�����ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF163_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,179,Pn,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(5*(1+TMaxTariff));
			//F180 �����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF164_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,180,Pn,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F181 ������1�����޹�����ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF165_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,181,Pn,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F182 ������2�����޹�����ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF166_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,182,Pn,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F183 ������3�����޹�����ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF167_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,183,Pn,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));//���¶��������
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F184 ������4�����޹�����ʾֵ���ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF168_LastDate_Source+3,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,184,Pn,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));//���¶��������
			#endif
			}
		#if (((USER/100)==5)&&(Project==300))//�Ϻ�ר��
			//!!!�¶����������������ÿ��1�յ��������,�������0 (�Ϻ�Ҫ�󰴴�����)
			//F193 �����й��������������ʱ�䣨�ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF185_LastDate_Source]!=0xee)
			{
				AddMonthCongeal(YM,193,Pn,FnDataAddr+6+Office_AFN0DF185_LastDate_Source,6+(7*(1+TMaxTariff)));//���¶��������
			}
			//F195 �����й��������������ʱ�䣨�ܡ�����1��M��
			if(p8FnData[6+Office_AFN0DF187_LastDate_Source]!=0xee)
			{
				AddMonthCongeal(YM,195,Pn,FnDataAddr+6+Office_AFN0DF187_LastDate_Source,6+(7*(1+TMaxTariff)));//���¶��������
			}
		#endif
			
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			if(f!=0)
			{
				MW(ADDR_DATABUFF,Addr,2+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//дԭʼ����
			}
		#endif
		}
	#endif
		return;
	}
	
	YMD=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰʱ��������
	YM=YMD>>8;
	YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
	YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
	if((p8RMLib[4]&0x08)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������B3=�ն���
		MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//���볭��ʱ��
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		Fn=p8RMLib[7];//ȡ�ն���Fn
		i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			x=MRR(i+2,3);//ȡԭ����������
			y=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰʱ��������
			if(x!=y)
			{//�����ղ�ͬ
				p32=(u32 *)(ADDR_Terminal_ClassDataLen);
				MW(FnDataAddr+1,i,p32[0]);
			}
		#if RMM_MonthCongealUserDate1==1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
			if((Fn>=185)&&(Fn<=188))
			{
				Fn+=8;
				i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					x=MRR(i+3,2);//ȡԭ��������
					y=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
					if(x!=y)
					{//���²�ͬ
						p32=(u32 *)(ADDR_Terminal_ClassDataLen);
						MW(FnDataAddr+1,i,p32[0]);
					}
				}
			}
		#endif
		}
	#else
		//RMM_DateCongeal_0xEE=1//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		Fn=p8RMLib[7];//ȡ�ն���Fn
#if (USER/100)==5//�Ϻ�
	#if (RMM_CompCongealTime==1)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
		if((Fn==185)||(Fn==187))
		{
			if(MRR(FnDataAddr,1)==1)
			{//������Ч��־
				AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���ն��������
			}
		}
		else
		{
			AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���ն��������
		}
	#else
		AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���ն��������
	#endif
#else
		AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���ն��������
#endif
	#if (((USER/100)==5)&&((Project/100)==3))//�Ϻ�ר��
		//!!!�¶����������������ÿ��1�յ��������,�������0 (�Ϻ�Ҫ�󰴴�����)
		#if RMM_MonthCongealUserDate1==1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
			if(MRR(ADDR_TYMDHMS+3,1)==1)//ȡ��ǰʱ����
			{
				if((Fn>=185)&&(Fn<=188))
				{//Fn=185-188�ն����������޹��������
					Fn+=8;//Fn=193-196���¶����������޹��������
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
				}			
			}
		#endif
	#else
		//!!!�¶����������������ÿ��1�յ��������,�������0
//		#if RMM_MonthCongealUserDate1==1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
//			if((Fn>=185)&&(Fn<=188))
//			{//Fn=185-188�ն����������޹��������
//				Fn+=8;//Fn=193-196���¶����������޹��������
//				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
//			}
//		#endif
	#endif
	#endif
	}

#if RMDateCongealSour!=0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
	if((p8RMLib[4]&0x10)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������B4=�����ն���
		Fn=p8RMLib[8];//ȡ�����ն���Fn
		Addr=GetLastRMDateCongealAddr(PORTn,Fn,Pn);//���˿ںŵõ����γ����ն���洢��ַ,����=0��ʾû�ҵ�
		if(Addr!=0)
		{
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//���볭��ʱ��
			DMW(FnDataAddr+1,Addr,Terminal_Class2DataLib[Fn].PnLen);
		}
	}
#endif

	if((p8RMLib[4]&0x20)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������B5=�¶���
#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
		//�Ϻ�������Ҫ���Ϻ������1�ղɼ�������3�������й�������Ϊ�¶��������й�����
		i=Check_AFN04F10_Pn(Pn);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+5,1)==21)
		{//�Ϻ���Լ
			Fn=p8RMLib[9];//ȡ�¶���Fn
			i=p8RMLib[0];
			if(i==137)
			{//��������
				//��1��
				if(MRR(FnDataAddr+7,1)!=0xee)
				{
					MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//���볭��ʱ��
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
				}
				//��2��
				if(MRR(FnDataAddr+7+Terminal_Class2DataLib[Fn].PnLen,1)!=0xee)
				{
					MW(ADDR_TYMDHMS+1,FnDataAddr+1+Terminal_Class2DataLib[Fn].PnLen,5);//���볭��ʱ��
					YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1+Terminal_Class2DataLib[Fn].PnLen,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
				}
			}
		}
		else
		{
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//���볭��ʱ��
		#if RMM_MonthCongealUserDate1==0//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			Fn=p8RMLib[9];//ȡ�¶���Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//������2������,�ڷ���0xffģʽ����2��
				return;
			}
			i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				x=MRR(i+3,2);//ȡԭ��������
				y=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
				if(x!=y)
				{//���²�ͬ
					p32=(u32 *)(ADDR_Terminal_ClassDataLen);
					MW(FnDataAddr+1,i,p32[0]);
				}
			}
			#else
			Fn=p8RMLib[9];//ȡ�¶���Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//������2������
				YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
			}
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
			#endif
		#else
			//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			Fn=p8RMLib[9];//ȡ�¶���Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//������2������,û��1�����ݴ�,�ڷ���0xffģʽ����2��
				return;
			}
			if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
			{//�������޹��������û��1�����ݴ�,���ܱ�����޸Ĵ�����ʱ���û��1�����ݴ�
				i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					x=MRR(i+3,2);//ȡԭ��������
					y=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
					if(x!=y)
					{//���²�ͬ
						p32=(u32 *)(ADDR_Terminal_ClassDataLen);
						MW(FnDataAddr+1,i,p32[0]);
					}
				}
			}
			#else
			Fn=p8RMLib[9];//ȡ�¶���Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//������2������,û��1�����ݴ�
				YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
				return;
			}
			if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
			{//�������޹��������û��1�����ݴ�,���ܱ�����޸Ĵ�����ʱ���û��1�����ݴ�
				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
			}
			#endif
		#endif
		}
#else//#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
		MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//���볭��ʱ��
	#if RMM_MonthCongealUserDate1==0//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
		#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		Fn=p8RMLib[9];//ȡ�¶���Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//������2������,�ڷ���0xffģʽ����2��
			return;
		}
		i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			x=MRR(i+3,2);//ȡԭ��������
			y=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
			if(x!=y)
			{//���²�ͬ
				p32=(u32 *)(ADDR_Terminal_ClassDataLen);
				MW(FnDataAddr+1,i,p32[0]);
			}
		}
		#else
		Fn=p8RMLib[9];//ȡ�¶���Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//������2������
			YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
		}
		AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
		#endif
	#else
		//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������
		#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		Fn=p8RMLib[9];//ȡ�¶���Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//������2������,û��1�����ݴ�,�ڷ���0xffģʽ����2��
			return;
		}
		if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
		{//�������޹��������û��1�����ݴ�,���ܱ�����޸Ĵ�����ʱ���û��1�����ݴ�
			i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				x=MRR(i+3,2);//ȡԭ��������
				y=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
				if(x!=y)
				{//���²�ͬ
					p32=(u32 *)(ADDR_Terminal_ClassDataLen);
					MW(FnDataAddr+1,i,p32[0]);
				}
			}
		}
		#else
		Fn=p8RMLib[9];//ȡ�¶���Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//������2������,û��1�����ݴ�
			YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
			return;
		}
	#if (((USER/100)==5)&&((Project/100)==3))//�Ϻ�ר��
		if((Fn>=213)&&(Fn<=216))
	#else
		if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
	#endif
		{//�������޹��������û��1�����ݴ�,���ܱ�����޸Ĵ�����ʱ���û��1�����ݴ�
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//���¶��������
		}
		#endif
	#endif
#endif
	}
	
	if((p8RMLib[4]&0x04)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������B2=���߶���
		if(((p8RMLib[0]>=0xe4)&&(p8RMLib[0]<=0xea))||(p8RMLib[0]==1))
		{//�����1-6�ฺ������
			Addr=6+1+4;
			//���ɼ�¼��ʼ�룺A0H��A0H(�� E0H��E0H�������ݿ鲻��ȷ),2 �ֽ�
			//���ɼ�¼�ֽ�����1 �ֽ�(ʮ������)
			//���ɼ�¼�洢ʱ�䣺�ꡢ�¡��ա�ʱ����,  5 �ֽ�
			if(p8FnData[6]>=(Addr-7+8))
			{
				if((p8FnData[Addr]!=0xa0)||(p8FnData[Addr+1]!=0xa0))
				{
					return;
				}
				YMDHM=MRR(FnDataAddr+Addr+3,5);//���ɼ�¼�洢ʱ��
				Addr+=8;
				//��ѹ��������Ƶ�ʣ�17 �ֽڣ� 
				//��ָ��룺AAH��1 �ֽڣ�
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+17))
					{
						return;
					}
					else
					{
						//A��B��C ���ѹ��ÿ�� 2 �ֽڣ��� 6 �ֽڣ���λ��0.1V��
						AddCurveCongeal(YMDHM,89,Pn,FnDataAddr+Addr,2);//�����߶��������
						AddCurveCongeal(YMDHM,90,Pn,FnDataAddr+Addr+2,2);//�����߶��������
						AddCurveCongeal(YMDHM,91,Pn,FnDataAddr+Addr+4,2);//�����߶��������
						//A��B��C �������ÿ�� 3 �ֽڣ��� 9 �ֽڣ���λ��0.001A��
						AddCurveCongeal(YMDHM,92,Pn,FnDataAddr+Addr+6,3);//�����߶��������
						AddCurveCongeal(YMDHM,93,Pn,FnDataAddr+Addr+9,3);//�����߶��������
						AddCurveCongeal(YMDHM,94,Pn,FnDataAddr+Addr+12,3);//�����߶��������
						Addr+=17+1;
					}
				}
				//�С��޹����ʣ�24 �ֽ�
				//��ָ��룺AAH��1 �ֽ�
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+24))
					{
						return;
					}
					else
					{
						//�ܼ� A��B��C ���й����ʣ�ÿ�� 3 �ֽڣ��� 12 �ֽڣ���λ��0.0001kW��
						AddCurveCongeal(YMDHM,81,Pn,FnDataAddr+Addr,3);//�����߶��������
						AddCurveCongeal(YMDHM,82,Pn,FnDataAddr+Addr+3,3);//�����߶��������
						AddCurveCongeal(YMDHM,83,Pn,FnDataAddr+Addr+6,3);//�����߶��������
						AddCurveCongeal(YMDHM,84,Pn,FnDataAddr+Addr+9,3);//�����߶��������
						//�ܼ� A��B��C ���޹����ʣ�ÿ�� 3 �ֽڣ��� 12 �ֽڣ���λ��0.0001kvar��
						AddCurveCongeal(YMDHM,85,Pn,FnDataAddr+Addr+12,3);//�����߶��������
						AddCurveCongeal(YMDHM,86,Pn,FnDataAddr+Addr+15,3);//�����߶��������
						AddCurveCongeal(YMDHM,87,Pn,FnDataAddr+Addr+18,3);//�����߶��������
						AddCurveCongeal(YMDHM,88,Pn,FnDataAddr+Addr+21,3);//�����߶��������
						Addr+=24+1;
					}
				}
				//����������8 �ֽ�
				//��ָ��룺AAH��1 �ֽ�
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+8))
					{
						return;
					}
					else
					{
						//�ܼ� A��B��C �๦��������ÿ�� 2 �ֽڣ��� 8 �ֽڣ���λ��0.001��
						AddCurveCongeal(YMDHM,105,Pn,FnDataAddr+Addr,2);//�����߶��������
						AddCurveCongeal(YMDHM,106,Pn,FnDataAddr+Addr+2,2);//�����߶��������
						AddCurveCongeal(YMDHM,107,Pn,FnDataAddr+Addr+4,2);//�����߶��������
						AddCurveCongeal(YMDHM,108,Pn,FnDataAddr+Addr+6,2);//�����߶��������
						Addr+=8+1;
					}
				}
				//�С��޹��ܵ��ܣ�16 �ֽ�
				//��ָ��룺AAH��1 �ֽ�
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+16))
					{
						return;
					}
					else
					{
						//�����й��ܵ���,�����й��ܵ���,����޹� 1 �ܵ���,����޹� 2 �ܵ���
					#if ((USER/100)==5)&&(Project==200)//�Ϻ������������=5ʱ;�����й�=�����й�+�����й�
						i=Check_AFN04F10_Pn(Pn);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
						if(i!=0)
						{
							i--;
							i=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*i)+26,1);//ȡ��С���
							if((i!=0x02)&&(i!=0x62)&&(i!=0x71)&&(i!=0x72))
							{//�����=5;�����й�=�����й�+�����й�
								x=MRR(FnDataAddr+Addr,4);
								y=MRR(FnDataAddr+Addr+4,4);
								if(((x&0xff)<=0x99)&&((y&0xff)<=0x99))
								{//�����й�������
									x=bcd_hex(x);
									y=bcd_hex(y);
									x+=y;
									x=hex_bcd(x);
									MWR(x,FnDataAddr+Addr,4);
								}
								else
								{
									MC(0xee,FnDataAddr+Addr,4);
								}
							}
						}
					#endif
						AddCurveCongeal(YMDHM,101,Pn,FnDataAddr+Addr,4);//�����߶��������
						AddCurveCongeal(YMDHM,103,Pn,FnDataAddr+Addr+4,4);//�����߶��������
						AddCurveCongeal(YMDHM,102,Pn,FnDataAddr+Addr+8,4);//�����߶��������
						AddCurveCongeal(YMDHM,104,Pn,FnDataAddr+Addr+12,4);//�����߶��������
						Addr+=16+1;
					}
				}
				//�������޹��ܵ��ܣ�16 �ֽ�
				//��ָ��룺AAH��1 �ֽ�
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+16))
					{
						return;
					}
					else
					{
						//��һ,�ڶ�,����,���������޹��ܵ��ܣ�4 �ֽڣ���λ��0.01kvarh��
						AddCurveCongeal(YMDHM,145,Pn,FnDataAddr+Addr,4);//�����߶��������
						AddCurveCongeal(YMDHM,147,Pn,FnDataAddr+Addr+4,4);//�����߶��������
						AddCurveCongeal(YMDHM,148,Pn,FnDataAddr+Addr+8,4);//�����߶��������
						AddCurveCongeal(YMDHM,146,Pn,FnDataAddr+Addr+12,4);//�����߶��������
						Addr+=16+1;
					}
				}
				//��ǰ������6 �ֽ�
				//��ָ��룺AAH��1 �ֽ�
			}
			return;
		}
	}
}


u32 CreateCurveCongealNo(u64 YMDHM)//�������߶����;����:ԭ�Ѵ��ڻ��´����Ķ����
{
	u32 i;
	u32 *p32;

	i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{//ԭ����û��ͬʱ��
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_CURVECONGEAL);
		KeyUserPnList();//�ص㻧���߶���Pn�б�
		MR(ADDR_AFN0D_Congeal_Buff_Start+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
		i=MRR(ADDR_TCongealCount+7,2);
		if((i&0x7fff)>=MaxCurveCongeal)
		{
			i=0x8000;
			MWR(i,ADDR_TCongealCount+7,2);
		}
		i&=0x7fff;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		p32[0]=2;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i);
		p32[3]=LEN_CURVECONGEAL;
		p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
		MWR(YMDHM,ADDR_Class2Congeal_AutoRW+20,5);//������Ч(����ʱ��)
	}
	return i;
}

u32 CreateDateCongealNo(u32 YMD)//�����ն����;����:ԭ�Ѵ��ڻ��´����Ķ����
{
	u32 i;
	u32 *p32;
	
	i=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{//ԭ����û��ͬʱ��
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//�嶳�Ỻ��
		i=MRR(ADDR_TCongealCount+14,1);
		if((i&0x7F)>=MaxDateCongeal)
		{
			i=0x80;
			MWR(i,ADDR_TCongealCount+14,1);
		}
		i&=0x7f;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		p32[0]=0x10002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
		p32[3]=LEN_DATECONGEAL;
		p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
		MWR(YMD,ADDR_Class2Congeal_AutoRW+20,3);//������Ч(����ʱ��)
	}
	return i;
}

u32 CreateMonthCongealNo(u32 YM)//�����¶����;����:ԭ�Ѵ��ڻ��´����Ķ����
{
	u32 i;
	u32 *p32;
	
	i=GetMonthCongealNo(YM);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{//ԭ����û��ͬʱ��
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_MONTHCONGEAL);//�嶳�Ỻ��
		i=MRR(ADDR_TCongealCount+28,1);
		if((i&0x7f)>=MaxMonthCongeal)
		{
			i=0x80;
			MWR(i,ADDR_TCongealCount+28,1);
		}
		i&=0x7f;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		p32[0]=0x30002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
		p32[3]=LEN_MONTHCONGEAL;
		p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
		MWR(YM,ADDR_Class2Congeal_AutoRW+20,2);//������Ч(����ʱ��)
	}
	return i;
}


#if RMDateCongealSour==1//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
u32 LastRMDateCongealYMD(u32 PORTn)//���˿ںż������γ����ն����ʱ��������;���:ͨ�ſں�;����:0��ʾû
{
	u32 i;
	u32 x;
	u32 YMD;
	u32 HM;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1-1;
			break;
		case RS485_1PORT://RS485_1
			PORTn=2-1;
			break;
		case RS485_2PORT://RS485_2
			PORTn=3-1;
			break;
		case RS485_4PORT://RS485_4(�ز�)
			PORTn=31-1;
			break;
	}
	HM=MRR(ADDR_AFN04F33+7+((14+(4*24))*PORTn),2);
	PORTn=MRR(ADDR_AFN04F33+3+((14+(4*24))*PORTn),4);
	PORTn<<=1;
	YMD=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TYMDHMS+1,2);
	x=YMD&0xff;
	x=bcd_hex(x);
	x=1<<x;
	if((PORTn&x)&&(i>=HM))
	{
		return YMD;
	}
	else
	{
		for(i=0;i<30;i++)
		{
			YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
			x=YMD&0xff;
			x=bcd_hex(x);
			x=1<<x;
			if(PORTn&x)
			{
				return YMD;
			}
		}
	}
	return 0;
}
u32 GetLastRMDateCongealAddr(u32 PORTn,u32 Fn,u32 Pn)//���˿ںŵõ����γ����ն���洢��ַ,����=0��ʾû�ҵ�
{
	u32 i;
	u32 x;
	u32 YMD;
	u32 *p32;
	
	YMD=LastRMDateCongealYMD(PORTn);//���˿ںż������γ����ն����ʱ��������;���:ͨ�ſں�;����:0��ʾû
	if(YMD==0)
	{
		return 0;
	}
	i=GetRMDateCongealNo(YMD);//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{
		//���������
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		if(p32[3]!=0x0)
		{//ԭ��2���ݶ���AUTOIOWû���
			return 0;
		}
		i=MRR(ADDR_TCongealCount+21,1);
		if((i&0x7F)>=MaxRMDateCongeal)
		{
			i=0x80;
		}
		x=i;
		i&=0x7f;
		MC(0xff,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),LEN_RMDATECONGEAL);
		DMWR(YMD,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//������Ч(д����ʱ��)
		//����ż���+1
		YMD=x&0x80;
		x&=0x7f;
		x+=1;
		if(x>=MaxRMDateCongeal)
		{
			x=0x80;
		}
		x|=YMD;
		MWR(x,ADDR_TCongealCount+21,1);
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	if(x==0)
	{
		return 0;
	}
	return (ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+x);
}
#endif




