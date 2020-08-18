

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_impIn.h"
#include "../Device/MEMRW.h"




const u8 Init_EnergyData_double_long_unsigned[]=//��ʼ������double_long_unsigned����
{
	DataType_array,
	TMaxTariff+1,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
	DataType_double_long_unsigned,
	0,0,0,0,
#if TMaxTariff>4
	#error
#endif
};

void Init_impInData(void)//��0���������ն˵��ܺ���������
{
#if NUMmax_IMPIN!=0
	u32 n;
	u8* p8;
	
//������������ESRAM
	//MC(0,ADDR_Terminal_impIn_ESRAM_START,(ADDR_Terminal_impIn_ESRAM_END)-(ADDR_Terminal_impIn_ESRAM_START));//������������ESRAM
	MC(0,ADDR_Terminal_impIn_Count,(ADDR_Terminal_impIn_ESRAM_END)-(ADDR_Terminal_impIn_Count));//������������ESRAM
//�����������
	MC(0,ADDR_impEnergy_Remainder,LEN_impEnergy_Remainder);
//����ڵ���ʾֵ
	for(n=0;n<NUMmax_IMPIN;n++)
	{
		p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
		MW((u32)Init_EnergyData_double_long_unsigned,(u32)p8,sizeof(Init_EnergyData_double_long_unsigned));
	}
//����ڵ��յ�����
	for(n=0;n<NUMmax_IMPIN;n++)
	{
		p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
		MW((u32)Init_EnergyData_double_long_unsigned,(u32)p8,sizeof(Init_EnergyData_double_long_unsigned));
	}
//����ڵ��µ�����
	for(n=0;n<NUMmax_IMPIN;n++)
	{
		p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
		MW((u32)Init_EnergyData_double_long_unsigned,(u32)p8,sizeof(Init_EnergyData_double_long_unsigned));
	}
//����˿ڵ������������ʱ��
	MC(0,ADDR_impDemand,LENper_impDemand*NUMmax_IMPIN);
#endif
}


void Terminal_impIn_Energy(void)//�ն���������-����
{
#if NUMmax_IMPIN!=0
	u32 i;
	u32 n;
	u32 Port;
	u32 imp;
	u64 d64;
	u8 * p8;
	u16 * p16;
	
	for(Port=0;Port<NUMmax_IMPIN;Port++)
	{
		i=MRR(ADDR_240x_4+(Port*LENper_240x_4)+5,4);//��������˿ں�	OAD
		i= R_Inverse(i,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		i&=0xff;
		if((i==0)||(i>NUMmax_IMPIN))
		{
			continue;
		}
		i--;
		//ȡ�ж��м�������
		p16=(u16 *)(ADDR_Terminal_impIn_Count);
		__disable_irq();
		imp=p16[i];
		p16[i]=0;
		__enable_irq();
	
		if(imp!=0x0)
		{//����������
			//�������������
			p16=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));
			p16[0]+=imp;
			
			i=MRR(ADDR_240x_4+(Port*LENper_240x_4)+12,2);//ȡ�����
			i=(i>>8)|((i&0xff)<<8);
			if(i!=0x0)
			{//�����!=0
				imp*=10000;
				imp+=MRR(ADDR_impEnergy_Remainder+(4*Port),4);
				MWR(imp%i,ADDR_impEnergy_Remainder+(4*Port),4);
				imp/=i;
			//�����������ʾֵ,���յ�����,���µ�����
				for(n=0;n<3;n++)
				{
					p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy*NUMmax_IMPIN)+(Port*LENper_impEnergy);
					p8[0]=DataType_array;
					p8[1]=1+TMaxTariff;
					p8+=2;
					for(i=0;i<TMaxTariff;i++)
					{
						p8[0]=DataType_double_long_unsigned;
						p8+=5;
					}
					//�ܵ���
					p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy*NUMmax_IMPIN)+(Port*LENper_impEnergy);
					p8+=3;
					d64=MRR((u32)p8,4);
					d64= R_Inverse(d64,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���)
					d64+=imp;
					d64%=100000000;
					d64= R_Inverse(d64,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���)
					MWR(d64,(u32)p8,4);
					//���ʵ���
					i=Meter_Ram->Tariff;//1BYTE ���ܱ�ǰִ�з���(HEX ����1,2,3,4 �ֱ��Ӧ���,��,ƽ,�ȵȷ���)
					if((i==0x0)||(i>TMaxTariff))
					{
						i=TMaxTariff;
					}
					d64=MRR((u32)p8+(5*i),4);
					d64= R_Inverse(d64,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���)
					d64+=imp;
					d64%=100000000;
					d64= R_Inverse(d64,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���)
					MWR(d64,(u32)p8+(5*i),4);
				}
			}
		}
	}
#endif
}

void Terminal_impIn_Demand(void)//�ն���������-���ʺ��������
{
#if NUMmax_IMPIN!=0
	u32 i;
	u32 x;
	u32 Port;
	u64 a64;
	u64 b64;
	u8 * p8;
	u16 * p16s;

	if(Terminal_Ram->SlipCYC_S_Timer!=0x0)
	{
		return;
	}
	Terminal_Ram->SlipCYC_S_Timer=15;//�´�15�붨ʱ
//ÿ15�빦�ʼ���
//����=(1������������*60)/(�����)
	for(Port=0;Port<NUMmax_IMPIN;Port++)
	{
		i=MRR(ADDR_240x_4+(Port*LENper_240x_4)+12,2);//ȡ�����
		i=(i>>8)|((i&0xff)<<8);
		if(i!=0x0)
		{//�����!=0
			p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//�ն���������,�������廬��(�ֶ���)
			a64=p16s[0];//1����Ϊ4��15��
			a64+=p16s[1];
			a64+=p16s[2];
			a64+=p16s[3];
			b64=60*10000;//4λС��
			a64*=b64;
			i=a64/i;
			if(i>=80000000)
			{
				i=0;//���
			}
			p8=(u8*)ADDR_240x_4+(Port*LENper_240x_4)+10;
			x=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
			if(x>=2)
			{
				i=~i;
				i++;
			}
			p8=(u8*)ADDR_240x_5+(Port*LENper_240x_5);//����ڹ���
			p8[0]=DataType_double_long;
			p8[1]=i>>24;
			p8[2]=i>>16;
			p8[3]=i>>8;
			p8[4]=i;
		}
	}

//����=(����������������*60)/(�����*��������)
//	if(Terminal_Ram->DemandCYC_S_Timer==0x0)//7 �ն������������������붨ʱ��
//	{
//		for(Port=0;Port<NUMmax_IMPIN;Port++)
//		{
//			MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//��ԭ�������
//			p8=(u8 *)(ADDR_DATABUFF+256);
//			i=p8[6];//ԭ���������������
//			p8=(u8 *)(ADDR_TYMDHMS);
//			x=p8[4];//��ǰʵʱ����
//			if(i!=x)
//			{//�¸���
//				if((p8[1]|p8[2])!=0x0)
//				{//ʵʱ��ʱ��!=0x0,Լ���¶������
//					MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//�¸�����0
//				}
//				else
//				{
//					if((Terminal_Ram->CongealFlags&0x10)==0x10)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
//					{//�¶������
//						MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//�¸�����0
//					}
//				}
//			}
//			i=MRR(ADDR_AFN04F11+(5*Port)+3,2);
//			if(i!=0x0)
//			{//�����!=0
//				p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//�ն���������,�������廬��(�ֶ���)
//				imp=0;
//				for(x=0;x<(4*15);x++)
//				{
//					imp+=p16s[x];
//				}
//				a64=imp;
//				b64=60*10000;//4λС��
//				a64*=b64;//����������������*60*10000(4λС��)
//				i*=15;//�����*��������
//				i=a64/i;
//				i=hex_bcd(i);
//				if(i>0x799999)
//				{
//					i=0;//���
//				}
//			}
//			else
//			{//�����=0
//				i=0;
//			}
//			//����������Ƚ�
//			x=MRR(ADDR_DATABUFF+256,3);//�����������ݵ��Ĵ���,Byte<=8
//			if(i>=x)
//			{//�Ƚ���>=,��Ϊ0ʱҲ��д��1����ʱ���ֵ,������ֵ��ͬ�޷�������1����
//				MWR(i,ADDR_DATABUFF+256,3);//д�Ĵ������ݵ�������,Byte<=8
//				MR(ADDR_DATABUFF+256+3,ADDR_TYMDHMS+1,4);
//			}
//			//������������Ƚ�
//			p8=(u8 *)(ADDR_Terminal_Period);
//			x=p8[0];
//			if(x!=0x0)
//			{
//				if(x<=TMaxTariff)
//				{
//					if(i>=MRR(ADDR_DATABUFF+256+(7*x),3))
//					{//�Ƚ���>=,��Ϊ0ʱҲ��д��1����ʱ���ֵ,������ֵ��ͬ�޷�������1����
//						MWR(i,ADDR_DATABUFF+256+(7*x),3);//�����������ݵ��Ĵ���,Byte<=8
//						MR(ADDR_DATABUFF+256+3+(7*x),ADDR_TYMDHMS+1,4);
//					}
//				}
//			}
//			MW(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//дԭ�������
//		}
//	}

//���廬��
	for(Port=0;Port<NUMmax_IMPIN;Port++)
	{
		p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//�ն���������,�������廬��(�ֶ���)
		for(i=((4*15)-2);i!=0x0;i--)
		{
			p16s[i+1]=p16s[i];
		}
		p16s[1]=p16s[0];
		p16s[0]=0;
	}


//ÿ1�����������������1����

#endif
}

























