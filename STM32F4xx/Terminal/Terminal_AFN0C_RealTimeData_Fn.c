
//�ն���1����Fn
#include "Project.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../terminal/Terminal_AFN0D_CongealData_Fn.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"


extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];


u32 Class1Data_FnOnly(u32 ClassNo,u32 Fn)//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
{
	u32 i;
	u8 * p8;
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F38+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
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

u32 Class1Data_Fn(u32 ClassNo,u32 Fn)//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
{
	u32 i;
	u8 * p8;
	
	if(Fn==0x0)
	{
		return(0);
	}
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F38+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
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
//F33 ��ǰ������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//2��F9 �����ն��� ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 129://16 F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
			case 130://17 F130 ��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��
			case 133://20 F133 ��ǰһ�����޹�����ʾֵ���ܡ�����1��M��
			case 136://23 F136 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,33);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,9);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F34 ��ǰ������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//2��F10 �����ն��� ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 131://18 F131 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
			case 132://19 F132 ��ǰ�����޹�������޹�2������ʾֵ���ܡ�����1��M��
			case 134://21 F134 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
			case 135://22 F135 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,34);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,10);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F35 ����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//2��F11 �����ն��� ������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 145://32 F145 ���������й��������������ʱ�䣨�ܡ�����1��M��
			case 146://33 F146 ���������޹��������������ʱ�䣨�ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,35);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,11);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F36 ���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//2��F12 �����ն��� ������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 147://34 F147 ���·����й��������������ʱ�䣨�ܡ�����1��M��
			case 148://35 F148 ���·����޹��������������ʱ�䣨�ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,36);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,12);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F37 ���£���һ�����գ�������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 137://24 F137 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
			case 138://25 F138 ���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
			case 141://28 F141 ���£���һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��
			case 144://31 F144 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,37);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==1//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,9);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F38 ���£���һ�����գ�������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
			case 139://26 F139 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
			case 140://27 F140 ���£���һ�����գ������޹�������޹�2������ʾֵ���ܡ�����1��M��
			case 142://29 F142 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
			case 143://30 F143 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,38);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==1//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,10);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F39 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 149://36 F149 ���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
			case 150://37 F150 ���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,39);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==1//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,11);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
//F40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
			case 151://38 F151 ���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
			case 152://39 F152 ���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
				i=Class1Data_FnOnly(ClassNo,40);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMDateCongealSour==1//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
				i|=Class2Data_FnOnly(ClassNo,12);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#endif
				return(i);
		}
	}
	return(0);
}


u32 GetClass1DataAddr(u32 Fn,u32 Pn)//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
{
	u32 i;
	u32 Addr;
	u32 Len;
	u32 Tariff;
	u8 * p8;
	u32 * p32;
	
	if(Fn>=Get_Class1Data_MaxFn())
	{
		return(0);
	}
	if(Pn>Terminal_Class1DataLib[Fn].PnMax)
	{
		if(Fn==246)
		{//AFN0CF246��ǰ�����¼����ӳ���Pn
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
	if((Terminal_Class1DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//�������޴�Fn����
		return(0);
	}
	if(Terminal_Class1DataLib[Fn].PnType==p0Type)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
	{
		Addr=Terminal_Class1DataLib[Fn].Addr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		if(Fn==246)
		{//AFN0CF246��ǰ�����¼����ӳ���Pn
			i=F246MapPn(Pn);//F246�����¼����ӳ��Pn;����0��ʾ��Ч
			if(i==0)
			{
				Addr=0;
			}
			else
			{
				Addr=Terminal_Class1DataLib[Fn].Addr+(Terminal_Class1DataLib[Fn].PnLen*(i-1));
			}
		}
		else
		{
			Addr=Terminal_Class1DataLib[Fn].Addr+(Terminal_Class1DataLib[Fn].PnLen*(Pn-1));
		}
	}
	if(Addr==0x0)
	{
		return(0);
	}
	if(Fn>=81)
	{
		if(Fn<=121)
		{//Сʱ��������
			p32=(u32 *)(ADDR_HourCongeal_AutoRW);//Сʱ�����Զ�дEFLASH���Ƶ�ַ
			if(p32[3]!=0x0)
			{//����дEFLASHСʱ��������,��ESRAMСʱ���Ỻ����ֵ
				Addr-=ADDR_HOURCONGEAL_START;
				Addr+=ADDR_HOURCONGEAL_BUFF_START;
			}
		}
	}
	p8=(u8 *)(Addr);
	Len=Terminal_Class1DataLib[Fn].PnLen;
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
		case 5://f5 �ն˿�������״̬;p0
			Len=BitNum(p8[1]);//������λΪ1�ĸ���(0-32)
			Len*=6;
			Len+=2;
			break;
		case 6://f6 �ն˵�ǰ����״̬;p0
			Len=BitNum(p8[2]);//������λΪ1�ĸ���(0-32)
			Len*=8;
			Len+=3;
			break;
		case 11://f11 �ն˼��г���״̬��Ϣ;p0
			Len=p8[0];
			Len*=19;
			Len+=1;
			break;
		case 19://f19 �����ܼ��й����������ܡ�����1��M��;�ܼ����
		case 20://f20 �����ܼ��޹����������ܡ�����1��M��;�ܼ����
		case 21://f21 �����ܼ��й����������ܡ�����1��M��;�ܼ����
		case 22://f22 �����ܼ��޹����������ܡ�����1��M��;�ܼ����
			MR(ADDR_Terminal_ClassDataLen,Addr,1);
			p8=(u8 *)(ADDR_Terminal_ClassDataLen);
			if(p8[0]==0xee)
			{
				p8[0]=0;
				MC(0,Addr,1);
			}
			Len=1+(4*(p8[0]+1));
			break;
		case 41://f41 ���������й����������ܡ�����1��M��;�������
		case 42://f42 ���������޹����������ܡ�����1��M��;�������
		case 43://f43 ���շ����й����������ܡ�����1��M��;�������
		case 44://f44 ���շ����޹����������ܡ�����1��M��;�������
		case 45://f45 ���������й����������ܡ�����1��M��;�������
		case 46://f46 ���������޹����������ܡ�����1��M��;�������
		case 47://f47 ���·����й����������ܡ�����1��M��;�������
		case 48://f48 ���·����޹����������ܡ�����1��M��;�������
			MR(ADDR_Terminal_ClassDataLen,Addr,1);
			p8=(u8 *)(ADDR_Terminal_ClassDataLen);
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[0]!=Tariff)
			{
				p8[0]=Tariff;
				MC(Tariff,Addr,1);
			}
	#else
			if(p8[0]==0xee)
			{
				p8[0]=Tariff;
				MC(Tariff,Addr,1);
			}
	#endif
			Len=1+(4*(p8[0]+1));
			break;
		case 33://f33 ��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��;�������
		case 34://f34 ��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��;�������
		case 37://f37 ���£���һ�����գ�������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��;�������	
		case 38://f38 ���£���һ�����գ�������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��;�������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(17*(p8[5]+1));
			break;
		case 35://f35 ����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��;�������
		case 36://f36 ���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��;�������
		case 39://f39 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��;�������
		case 40://f40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��;�������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(14*(p8[5]+1));
			break;
		case 57://f57 ��ǰA��B��C�����ѹ������2��N��г����Чֵ;�������
			if((p8[0]==0xee)||(p8[0]==0))
			{
				p8[0]=0;
				Len=1;
			}
			else
			{
				Len=1+(12*18);
			}
			break;
		case 58://f58 ��ǰA��B��C�����ѹ������2��N��г��������;�������
			if((p8[0]==0xee)||(p8[0]==0))
			{
				p8[0]=0;
				Len=1+6;
			}
			else
			{
				p8[0]=19;
				Len=1+6+(12*18);
			}
			break;
		case 129://F129	��ǰ�����й�����ʾֵ���ܡ�����1��M��	�������
		case 131://F131	��ǰ�����й�����ʾֵ���ܡ�����1��M��	�������
		case 137://F137	���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��	�������
		case 139://F139	���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��	�������
		case 153://F153	��һʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 154://F154	�ڶ�ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 155://F155	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 156://F156	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 157://F157	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 158://F158	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 159://F159	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 160://F160	�ڰ�ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
		case 177://F177 ��ǰ����й�����ʾֵ���ܡ�����1~M��
		case 178://F178 ��1 ����������й�����ʾֵ���ܡ�����1~M��
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(5*(p8[5]+1));
			break;
		case 130://F130	��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
		case 132://F132	��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
		case 133://F133	��ǰһ�����޹�����ʾֵ���ܡ�����1��M��	�������
		case 134://F134	��ǰ�������޹�����ʾֵ���ܡ�����1��M��	�������
		case 135://F135	��ǰ�������޹�����ʾֵ���ܡ�����1��M��	�������
		case 136://F136	��ǰ�������޹�����ʾֵ���ܡ�����1��M��	�������
		case 138://F138	���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
		case 140://F140	���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
		case 141://F141	���£���һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��	�������
		case 142://F142	���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��	�������
		case 143://F143	���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��	�������
		case 144://F144	���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��	�������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(4*(p8[5]+1));
			break;
		case 145://F145	���������й��������������ʱ�䣨�ܡ�����1��M��	�������
		case 146://F146	���������޹��������������ʱ�䣨�ܡ�����1��M��	�������
		case 147://F147	���·����й��������������ʱ�䣨�ܡ�����1��M��	�������
		case 148://F148	���·����޹��������������ʱ�䣨�ܡ�����1��M��	�������
		case 149://F149	���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��	�������
		case 150://F150	���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��	�������
		case 151://F151	���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��	�������
		case 152://F152	���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��	�������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(7*(p8[5]+1));
			break;
		case 168://F168	���ܱ������Ϣ	�������
	#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			if(p8[5]!=Tariff)
			{
				p8[5]=Tariff;
			}
	#else
			if(p8[5]==0xee)
			{
				p8[5]=Tariff;
			}
	#endif
			Len=6+(10*(bcd_hex(p8[5]+1)));
			break;
		case 169://F169	���г����м�·����Ϣ	�������
		case 170://F170	���г���������Ϣ	�������
			break;
#if (USER/100)==5//�Ϻ�
		case 242://F242���յ�¼��վ��Ϣ���ݵ�Ԫ��ʽ
			Len=MRR(ADDR_AFN0CF242,4);
			if(Len>AFN0CF242MaxCount)//���յ�¼��վ��Ϣ����¼����
			{
				Len=AFN0CF242MaxCount;
			}
			MWR(Len,ADDR_DATABUFF,4);
			Len*=6;
			MR(ADDR_DATABUFF+4,ADDR_AFN0CF242_Next,Len);
			Len+=4;
			break;
		case 243://�����ź�ǿ��
			MR(ADDR_DATABUFF,ADDR_AFN0CF243,24);
			p8=(u8 *)(ADDR_DATABUFF);
			for(Tariff=0;Tariff<24;Tariff++)
			{
				if(p8[Tariff]==0xff)
				{
					p8[Tariff]=0xee;
				}
			}
			break;
#endif
		case 245://�ն��¼���������
			Len=p8[0];
			Len*=2;
			Len+=1;
			break;
	}
	p32=(u32 *)(ADDR_Terminal_ClassDataLen);
	p32[0]=Len;
	return(Addr);
}

u32 GetClass1Data_PnMax(u32 Fn)//ȡ��1���ݸ�Fn�����Pn
{
	return Terminal_Class1DataLib[Fn].PnMax;
}

void SaveClass1Data(u32 Fn,u32 Pn,u32 SourAddr)//������1����
{
	u32 i;
	
	i=GetClass1DataAddr(Fn,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�
	if(i!=0x0)
	{
		MW(SourAddr,i,Terminal_Class1DataLib[Fn].PnLen);
		if(Terminal_Class1DataLib[Fn].PnLen>(LEN_UART2FnDataBuff-1))
		{
			Comm_Ram->ErrorIC|=(1<<18);//B18=��������Fn���峤�Ȳ���
			Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		}
	}
}


void Class1_F33_F34_F37_F38(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4)//����������ϳ�F33_F34_F37_F38
{
	u32 i;
	u32 x;
	u8 * p8s;
	u8 * p8d;
	u32 RMTime;
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
	p8s=(u8 *)(FnDataAddr);
	//�����й�
	i=GetClass1DataAddr(Fn1,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
	p8d=(u8 *)(i);
	x=p8d[5];//������
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
	for(i=0;i<(6+((x+1)*5));i++)
	{
		p8s[1+i]=p8d[i];
	}
	p8s[6]=x;//������
	if(p8s[1]!=0xee)
	{
		RMTime=1;//����ʱ�������־,0=û,1=����
	}
	//�����޹�������޹�1��
	i=GetClass1DataAddr(Fn2,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
	p8d=(u8 *)(i);
	if(RMTime==0)//����ʱ�������־,0=û,1=����
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//����ʱ�������־,0=û,1=����
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1)*4;i++)
	{
		p8s[7+((x+1)*5)+i]=p8d[i];
	}
	//һ�����޹�
	i=GetClass1DataAddr(Fn3,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
	p8d=(u8 *)(i);
	if(RMTime==0)//����ʱ�������־,0=û,1=����
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//����ʱ�������־,0=û,1=����
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1)*4;i++)
	{
		p8s[7+((x+1)*9)+i]=p8d[i];
	}
	//�������޹�
	i=GetClass1DataAddr(Fn4,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
	p8d=(u8 *)(i);
	if(RMTime==0)//����ʱ�������־,0=û,1=����
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//����ʱ�������־,0=û,1=����
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1)*4;i++)
	{
		p8s[7+((x+1)*13)+i]=p8d[i];
	}
}
void Class1_F35_F36_F39_F40(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2)//����������ϳ�F35_F36_F39_F40
{
	u32 i;
	u32 x;
	u32 y;
	u8 * p8s;
	u8 * p8d;
	u32 RMTime;
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
	p8s=(u8 *)(FnDataAddr);
	//�й�
	i=GetClass1DataAddr(Fn1,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
	p8d=(u8 *)(i);
	x=p8d[5];//������
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
	for(i=0;i<5;i++)
	{
		p8s[1+i]=p8d[i];
	}
	p8s[6]=x;
	p8d+=6;
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<3;y++)
		{
			p8s[7+(3*i)+y]=p8d[(7*i)+y];
		}
	}
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<4;y++)
		{
			p8s[7+(3*(x+1))+(4*i)+y]=p8d[(7*i)+3+y];
		}
	}
	if(p8s[1]!=0xee)
	{
		RMTime=1;//����ʱ�������־,0=û,1=����
	}
	//�޹�
	i=GetClass1DataAddr(Fn2,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
	p8d=(u8 *)(i);
	if(RMTime==0)//����ʱ�������־,0=û,1=����
	{
		if(p8d[0]!=0xee)
		{
			RMTime=1;//����ʱ�������־,0=û,1=����
			MR(FnDataAddr+1,i,5);
		}
	}
	p8d+=6;
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<3;y++)
		{
			p8s[7+(7*(x+1))+(3*i)+y]=p8d[(7*i)+y];
		}
	}
	for(i=0;i<(x+1);i++)
	{
		for(y=0;y<4;y++)
		{
			p8s[7+(7*(x+1))+(3*(x+1))+(4*i)+y]=p8d[(7*i)+3+y];
		}
	}
}







