
//���ܱ����(ATT7022E)
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Meter698/Meter07_Measure_ATT7022E.h"
#include "../../NUC97x/Meter698/Meter07_Energy_ATT7022E.h"
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/Meter698/Meter07_In.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"


#if IC_ATT7022==1//0=û,1=��

void EMUStartStopUerrIerr(void)//��Ǳ����ѹ�������������
{
//Bit00 PA =1����ʾA��ʧѹ��=0��A��δʧѹ��
//Bit01 PB =1����ʾB��ʧѹ��=0��B��δʧѹ��
//Bit02 PC =1����ʾC��ʧѹ��=0��C��δʧѹ��
//Bit03 Uorder =1����ʾ��ѹ�����=0����ѹδ������
//Bit04 Iorder =1����ʾ���������=0������δ������
//Bit05 Revp =1����ʾ������һ���й�����Ϊ����=0���������й�����Ϊ����
//Bit06 Revq =1����ʾ������һ���޹�����Ϊ����=0���������޹�����Ϊ����
//Bit07 �ϵ縴λ��IRQ pin�ź��Զ���ͣ�ͬʱSFlag.7�øߣ���SPIд�����ݺ�
//IRQ�ź��Զ���ߵ�ͬʱSFlag.7�Զ���ͣ���SFlag��bit07 IRQ��־λ��Ӳ��
//IRQ�ź�����ȫͬ���ġ�
//Bit09 =1����ʾA�ദ��Ǳ��״̬��=0��A�ദ����״̬��
//Bit10 =1����ʾB�ദ��Ǳ��״̬��=0��B�ദ����״̬��
//Bit11 =1����ʾC�ദ��Ǳ��״̬��=0��C�ദ����״̬��
//Bit12 =1����ʾ�����й�����Ϊ����=0�������й�����Ϊ����
//Bit13 =1����ʾ�����޹�����Ϊ����=0�������޹�����Ϊ����
//Bit14 =1����ʾͬ�����ݻ�����ϣ�Sync_Enд0�����
//Bit15 =1����ʾͬ�����ݹ���ϵ��������Χ��ync_Enд0�����
//Bit17 =1����ʾA���������Ǳ��״̬��=0��A�����δǱ����
//Bit18 =1����ʾB���������Ǳ��״̬��=0��B�����δǱ����
//Bit19 =1����ʾC���������Ǳ��״̬��=0��C�����δǱ����
	u32 i;
	u32 Data;
	
	Data=Security_Read_ATT7022(0x2C);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return;
	}
//��Ǳ��
	i=(Data>>9)&0x7;//λΪ1��ʾ����;b2=�й�C,b1=�й�B,b0=�й�A
	Meter_Ram->NOPLoad=i;
	Meter_Ram->NOQLoad=i;
	Meter_Ram->NOSLoad=i;
//��ѹ�����
	i=Data&(1<<3);
	if(i)
	{
		Meter_Ram->IPFlags|=0x80;
	}
	else
	{
		Meter_Ram->IPFlags&=~0x80;
	}
//��ʾ���������
	i=Data&(1<<4);
	if(i)
	{
		Meter_Ram->IPFlags|=0x40;
	}
	else
	{
		Meter_Ram->IPFlags&=~0x40;
	}
	
}

u32 Calculate_U(u32 Addr,u32 ADDR_DATA)//��ѹ����
{
//����Vrms��24λ���ݣ�������ʽ
//ʵ�ʷ����ѹ��ЧֵΪ��Urms = Vrms/2^13
	u32 Data;
	u64 d64;
	
	Data=Security_Read_ATT7022(Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	d64=Data;
	d64*=100000;//4BYTE_BCD 5λС��
	d64/=1<<13;
	Data=d64;
#if DEBUG==0//0=����,1=����
	if(Data<(((MeterUBhex*10)/100)*10000))
	{//<(10%UN)
		Data=0;
	}
#endif
	Data=hex_bcd(Data);
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

u32 Calculate_I(u32 Addr,u32 ADDR_DATA)//��������
{
//ʵ�ʷ��������ЧֵΪ��Irms = (Vrms/2^13)/N
//����ϵ��N���壺�����Ib���뵽оƬ��ȡ����ѹΪ50mVʱ����Ӧ�ĵ�����Чֵ�Ĵ���ֵΪVrms��Vrms/2^13Լ����60����ʱN=60/Ib�� Ib=1.5A��N=60/1.5=40��Ib=6A��N=60/6=10
//ͬ�������뵽оƬ��ȡ����ѹΪ25mVʱ��Vrms/2^13Լ����30��Ib=1.5A��N=30/1.5=20��Ib=6A��N=30/6=5���ɸ��ݵ�ǰIb������ʵ��ֵ������Nֵ
	u32 Data;
	float N;
	float Irms;
	
	N=60;
	N*=ADC_Vi;
	N/=0.05;//��оƬ��ȡ����ѹΪ50mVʱ,Vrms/2^13Լ����60Ϊ׼
	N*=100;//MeterIBhex�ж�λС��
	N/=MeterIBhex;
	
	Data=Security_Read_ATT7022(Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	Irms=Data;
	Irms*=100000;//4BYTE_BCD 5λС��
	Irms/=1<<13;
	Irms/=N;
	Data=Irms;
#if DEBUG==0//0=����,1=����
	#if MeterIMaxhex<=2000//������xx.xx(A)
	if(Data<200)
	{//<2.00MA
		Data=0;
	}
	#else
	if(Data<400)
	{//<4.00MA
		Data=0;
	}
	#endif
#endif
	Data=hex_bcd(Data);
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

u32 Calculate_P(u32 Addr,u32 ADDR_DATA)//���ʼ���
{
//���ʼĴ�����ʽ���壺
//A/B/C���๦�ʲ�����X
//X��24λ���ݣ�������ʽ
//���X>2^23����XX=X-2^24
//����XX=X
//ʵ�ʵ�A/B/C���๦�ʲ���Ϊ��XXX=XX*K������KΪ���ʲ���ϵ�������й��ʲ������ã�
//��λ�����ʵ�λ����(W)������ϵ��K=2.592*10^10/(HFconst*EC*2^23)
	u32 Data;
	float K;
	u32 s;
#if DEBUG==0//0=����,1=����
	u64 d64a;
	u64 d64b;
	u32 UAddr;
#endif
	
	K=25920000000;
	K/=HFConst;
	K/=MeterActiveConst;
	K/=1<<23;
	
	Data=Security_Read_ATT7022(Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{
		s=1;//����
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{
		s=0;//����
	}
	K*=Data;
	K*=1000;//4BYTE_SBCD 6λС��
	Data=K;
#if DEBUG==0//0=����,1=����
	d64a=MeterUBhex;//�αȵ�ѹxxx.x(V)
	d64b=MeterIBhex;//�αȵ���xx.xx(A)
	d64a*=d64b;//x.xxxVA
	d64a/=1000;//0.1%IB
	if(Data<=d64a)
	{//<=0.1%IB����,����=0
		Data=0;
	}
	if(Data<=(d64a*10))
	{//<=1.0%IB����,���ʷ��򲻱�
		s=MRR(ADDR_DATA+3,1);
		s&=0x80;
	}
	switch(ADDR_DATA)
	{//���ڹ��ʷ���=�й����ʷ���
		case ADDR_SA:
			s=MRR(ADDR_PA+3,1);
			s&=0x80;
			break;
		case ADDR_SB:
			s=MRR(ADDR_PB+3,1);
			s&=0x80;
			break;
		case ADDR_SC:
			s=MRR(ADDR_PC+3,1);
			s&=0x80;
			break;
	}
	switch(ADDR_DATA)
	{
		case ADDR_PA:
		case ADDR_QA:
		case ADDR_SA:
			UAddr=ADDR_UA;
			break;
		case ADDR_PB:
		case ADDR_QB:
		case ADDR_SB:
			UAddr=ADDR_UB;
			break;
		case ADDR_PC:
		case ADDR_QC:
		case ADDR_SC:
			UAddr=ADDR_UC;
			break;
	}
	UAddr=MRR(UAddr+2,2);
	if(UAddr<((MeterUBhex*10)/100))
	{//��ѹ<(10%UN),���ʷ��򲻱�
		s=MRR(ADDR_DATA+3,1);
		s&=0x80;
	}
#endif
	Data*=1004;//ʵ��У�鷢����ʾֵС0.4%
	Data/=1000;
	Data=hex_bcd(Data);
	if(Data)
	{
		if(s)
		{
			Data|=0x80000000;
		}
	}
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

u32 Calculate_PT(u32 Addr,u32 ADDR_DATA)//���๦�ʼ���
{
//���ʼĴ�����ʽ���壺
//A/B/C���๦�ʲ�����T
//T��24λ���ݣ�������ʽ
//���T>2^23����TT=T-2^24
//����TT=T
//ʵ�ʵĺ��๦�ʲ���Ϊ��TTT=TT*2*K������KΪ���ʲ���ϵ�������й��ʲ������ã���
//��λ�����ʵ�λ����(W)������ϵ��K=2.592*10^10/(HFconst*EC*2^23)
	u32 Data;
	float K;
	u32 s;
#if DEBUG==0//0=����,1=����
	u64 d64a;
	u64 d64b;
#endif
	
	K=25920000000;
	K/=HFConst;
	K/=MeterActiveConst;
	K/=1<<23;
	K*=2;
	
	Data=Security_Read_ATT7022(Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{
		s=1;//����
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{
		s=0;//����
	}
	K*=Data;
	K*=1000;//4BYTE_SBCD 6λС��
	Data=K;
#if DEBUG==0//0=����,1=����
	d64a=MeterUBhex;//�αȵ�ѹxxx.x(V)
	d64b=MeterIBhex;//�αȵ���xx.xx(A)
	d64a*=d64b;//x.xxxVA
	d64a/=1000;//0.1%IB
	if(Data<=d64a)
	{//<=0.1%IB����,����=0
		Data=0;
	}
	if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
	{
		d64a*=(3*10);
	}
	else
	{
		d64a*=(2*10);
	}
	if(Data<=d64a)
	{//<=1.0%IB����,���ʷ��򲻱�
		s=MRR(ADDR_DATA+3,1);
		s&=0x80;
	}
	if(ADDR_DATA==ADDR_ST)
	{//���ڹ��ʷ���=�й����ʷ���
		s=MRR(ADDR_PT+3,1);
		s&=0x80;
	}
#endif
	Data*=1004;//ʵ��У�鷢����ʾֵС0.4%
	Data/=1000;
	Data=hex_bcd(Data);
	if(Data)
	{
		if(s)
		{
			Data|=0x80000000;
		}
	}
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

void Quadrant(u32 ADDR_P,u32 ADDR_Q,u32 ADDR_FLAG)//���޼���
{
	u32 i;
	u8 *p8;
	
	i=0;
	p8=(u8 *)(ADDR_P);
	if((p8[3]&0x80)!=0x0)
	{
		i=1;
	}
	p8=(u8 *)(ADDR_Q);
	if((p8[3]&0x80)!=0x0)
	{
		i|=0x2;
	}
	p8=(u8 *)(ADDR_FLAG);
	switch(i)
	{
		case 0://1����
			p8[0]=1;
		break;
		case 1://2����
			p8[0]=2;
		break;
		case 2://4����
			p8[0]=4;
		break;
		case 3://3����
			p8[0]=3;
		break;
	}
}

u32 Calculate_COS(u32 Addr,u32 ADDR_DATA)//������������
{
//ATT7022E/26E/28E���������Ĵ������ò�����ʽ���������λ�Ƿ���λ�����������ķ���λ���й����ʵķ���λһ��
//PF��24λ���ݣ�������ʽ
//���PF>2^23����PFF=PF-2^24
//���� PFF=PF
//ʵ�ʹ�������Ϊ��pf=PFF/2^23
	u32 Data;
	u64 d64;
	u32 s;
	
	Data=Security_Read_ATT7022(Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{//��
		s=1;
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{//��
		s=0;
	}
	d64=Data;
	d64*=1000;//2BYTE_SBCD 3λС��
	d64/=1<<23;
	Data=d64;
	Data=hex_bcd(Data);
	if(s)
	{
		Data|=0x8000;
	}
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,2);
	}
	return Data;
}

u32 Calculate_TEMP(void)//�¶ȼ���
{
//�¶ȴ�������Ҫ���üĴ���0x31��TPS_En=1������TPS_Sel=0 ѡ��PN�¶ȴ�������
//���ݸ�ʽΪTM��24λ���ݵĵ�8λ��Ч
//���TM����128����TMM=TM-256
//���� TMM=TM
//�ⲿMCU��ȡ�üĴ�����ֵ�����������任֮���ٸ������й�ʽ�õ���ʵ�¶�ֵ��
//��ʵ�¶�TP=TC - 0.726*TMM
//����TCΪУ��ֵ��������Ϊ25��ʱ�������¶�У���õ�TC
	
//#define ADDR_TEMP     (ADDR_UDC+2)//�����¶�(���ֶ���)
//	u16 TEMP;//BCD 1λС��(b15����λ)
	
	u32 Data;
	u32 TC;
	float t;
	
	Data=Security_Read_ATT7022(0x2A);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	t=Data;
	if(Data>128)
	{
		t-=256;
	}
	TC=MRR(ADDR_MeterCalibration_IEEPROM_START+10,1);
	t*=0.726;
	t=TC-t;
	t*=10;//1λС��
	if(t>=0)
	{
		TC=t;
		TC=hex_bcd(TC);
		MWR(TC,ADDR_TEMP,2);
		return TC;
	}
	else
	{
		t*=-1;
		TC=t;
		TC=hex_bcd(TC);
		TC|=0x8000;
		MWR(TC,ADDR_TEMP,2);
		return TC;
	}
}

u32 Calculate_Hz(void)//Ƶ�ʼ���
{
//��ѹ��Ƶ�ʼĴ�������24λ������ʽ���������λΪ����λ������λ����Ϊ0��
//Freq��24λ���ݣ�������ʽ
//ʵ��Ƶ��Ϊ��f=Freq/2^13����λ������(Hz)��
	u32 Data;
	
	Data=Security_Read_ATT7022(0x1C);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	Data*=100;//2BYTE_BCD 2λС��
	Data/=1<<13;
	Data=hex_bcd(Data);
	MWR(Data,ADDR_FREQUENCY,2);
	return Data;
}

u32 Calculate_Angle(u32 Addr,u32 ADDR_DATA)//�Ƕȼ���
{
//��ǼĴ������ò�����ʽ��������λ�Ƿ���λ����ʾ-180��~+180��֮��ļнǡ�
//�ȣ�21λ��Ч���ݣ�������ʽ����3λ��Ϊ��չ�ķ���λ
//�����>=2^20�����=��-2^24 �����=��
//ʵ�����Ϊ��Pg=(��/2^20)*180��
	u32 Data;
	u32 s;
	
	Data=Security_Read_ATT7022(Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{//��
		s=1;
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{//��
		s=0;
	}
	Data*=180;
	Data*=10;//2BYTE_BCD 1λС��
	Data/=1<<20;
	if(s)
	{//��
		Data=3600-Data;
	}
	Data=hex_bcd(Data);
	MWR(Data,ADDR_DATA,2);
	return Data;
}

u32 Calculate_AngleUI(u32 ADDR_AngleU,u32 ADDR_AngleI,u32 ADDR_AngleUI)//��ѹ�����н�
{
	u32 Qu;
	u32 Qi;
	
	Qu=MRR(ADDR_AngleU,2);
	Qi=MRR(ADDR_AngleI,2);
	Qu=bcd_hex(Qu);
	Qi=bcd_hex(Qi);
	if(Qi<Qu)
	{
		Qi+=3600;
	}
	Qi-=Qu;
	Qi=hex_bcd(Qi);
	MWR(Qi,ADDR_AngleUI,2);
	return Qi;
}



void Meter07_Measure(void)//������
{
	u32 i;
	u32 x;
	u32 ReNum;
	u8* p8;
	
	if(Meter_Ram->EMUInit!=5)
	{
		return;
	}
	switch(Meter_Ram->I1SubTask1)//I1�ӷ�ʱ����1,���ڱ�������:��ѹ,����,����,...��
	{
		case 0://CS�Ƚ�
			for(ReNum=100;ReNum>0;ReNum--)
			{//��EMC����ʱ�����У�ע:100��Լ��50ms
				i=Read_ATT7022(0x3e);
				x=Read_ATT7022(0x5e);
				if((Meter_Ram->SSP1Data0==i)&&(Meter_Ram->SSP1Data1==x))//У������У���
				{
					break;
				}
				if(Pin_Read(PIN_WKUP)==0)
				{//����
					 break;
				}
			}
			if(ReNum==0)
			{//CS��
				Meter_Ram->EMUParaCount=0;
				Meter_Ram->EMUInit=4;//0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
				return;
			}
			break;
		case 1:
			EMUStartStopUerrIerr();//��Ǳ����ѹ�������������
			break;
		case 2://UA����
			i=Calculate_U(0x0d,ADDR_UA);//��ѹ����
		#if (USER/100)==5//�Ϻ�
			Meter07_UAB();//UAB���ѹ����
		#endif
			break;
		case 3://UB����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_U(0x0e,ADDR_UB);//��ѹ����
			}
			else
			{
				MWR(0,ADDR_UB,4);
			}
		#if (USER/100)==5//�Ϻ�
			Meter07_UBC();//UBC���ѹ����
		#endif
			break;
		case 4://UC����
			i=Calculate_U(0x0f,ADDR_UC);//��ѹ����
		#if (USER/100)==5//�Ϻ�
			Meter07_UCA();//UCA���ѹ����
		#endif
			break;
		case 5://PT����
			i=Calculate_PT(0x04,ADDR_PT);//���๦�ʼ���
			break;
		case 6://PA����
			i=Calculate_P(0x01,ADDR_PA);//���ʼ���
			break;
		case 7://PB����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_P(0x02,ADDR_PB);//���ʼ���
			}
			else
			{
				MWR(0,ADDR_PB,4);
			}
			break;
		case 8://PC����
			i=Calculate_P(0x03,ADDR_PC);//���ʼ���
			break;
		case 9://QT����
			i=Calculate_PT(0x08,ADDR_QT);//���๦�ʼ���
			Quadrant(ADDR_PT,ADDR_QT,(u32)&Meter_Ram->TQ);//���޼���
			break;
		case 10://QA����
			i=Calculate_P(0x05,ADDR_QA);//���ʼ���
			Quadrant(ADDR_PA,ADDR_QA,(u32)&Meter_Ram->AQ);//���޼���
			break;
		case 11://QB����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_P(0x06,ADDR_QB);//���ʼ���
			}
			else
			{
				MWR(0,ADDR_QB,4);
			}
			Quadrant(ADDR_PB,ADDR_QB,(u32)&Meter_Ram->BQ);//���޼���
			break;
		case 12://QC����
			i=Calculate_P(0x07,ADDR_QC);//���ʼ���
			Quadrant(ADDR_PC,ADDR_QC,(u32)&Meter_Ram->CQ);//���޼���
			break;
		case 13://ST����
			i=Calculate_PT(0x0C,ADDR_ST);//���๦�ʼ���
			break;
		case 14://SA����
			i=Calculate_P(0x09,ADDR_SA);//���ʼ���
			break;
		case 15://SB����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_P(0x0A,ADDR_SB);//���ʼ���
			}
			else
			{
				MWR(0,ADDR_SB,4);
			}
			break;
		case 16://SC����
			i=Calculate_P(0x0B,ADDR_SC);//���ʼ���
			break;
		case 17://IA����
			i=Calculate_I(0x10,ADDR_IA);//��������
			p8=(u8*)(ADDR_PA);
			i=p8[3]&0x80;
			p8=(u8*)(ADDR_IA);
			p8[3]|=i;
			break;
		case 18://IB����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_I(0x11,ADDR_IB);//��������
				p8=(u8*)(ADDR_PB);
				i=p8[3]&0x80;
				p8=(u8*)(ADDR_IB);
				p8[3]|=i;
			}
			else
			{
				MWR(0,ADDR_IB,4);
			}
			break;
		case 19://IC����
			i=Calculate_I(0x12,ADDR_IC);//��������
			p8=(u8*)(ADDR_PC);
			i=p8[3]&0x80;
			p8=(u8*)(ADDR_IC);
			p8[3]|=i;
			break;
		case 20://IN����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
//				x=MRR(ADDR_IN,4);//ԭ����
//				i=Calculate_I(0x29,ADDR_IN);//��������
//        Data_Valid(x,i,ADDR_IN);//������Ч���ж�(EMC����)
//				p8=(u8*)(ADDR_PT);
//				i=p8[3]&0x80;
//				p8=(u8*)(ADDR_IN);
//				p8[3]|=i;
				Meter07_In();//���ߵ�������
			}
			break;
		case 21://COST����
			i=Calculate_COS(0x17,ADDR_COST);//������������
			x=0;
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x|=1;
			}
			i=MRR(ADDR_IB,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x|=2;
			}
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x|=4;
			}
			if(x==7)
			{//������޵���
				MWR(0x1000,ADDR_COST,2);
			}
			break;
		case 22://COSA����
			i=Calculate_COS(0x14,ADDR_COSA);//������������
			x=0;
			i=MRR(ADDR_UA+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x=1;
			}
			if(x!=0)
			{
				MWR(0x1000,ADDR_COSA,2);
			}
			break;
		case 23://COSB����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_COS(0x15,ADDR_COSB);//������������
				x=0;
				i=MRR(ADDR_UB+2,2);
				i=bcd_hex(i);
				if(i<((MeterUBhex*10)/100))
				{//��ѹ<(10%UN)
					x=1;
				}
				i=MRR(ADDR_IB,4);
				i&=0x7fffffff;
				i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
				i=bcd_hex(i);
				if(i<((MeterIBhex*2)/3))
				{//����<(1%IB)��2/3
					x=1;
				}
				if(x!=0)
				{
					MWR(0x1000,ADDR_COSB,2);
				}
			}
			break;
		case 24://COSC����
			i=Calculate_COS(0x16,ADDR_COSC);//������������
			x=0;
			i=MRR(ADDR_UC+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x=1;
			}
			if(x!=0)
			{
				MWR(0x1000,ADDR_COSC,2);
			}
			break;
		case 25://�¶ȼ���
			i=Calculate_TEMP();//�¶ȼ���
			break;
		case 26://Ƶ�ʼ���
			i=Calculate_Hz();//Ƶ�ʼ���
			break;

		case 27://UB�Ƕȼ���
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_Angle(0x27,ADDR_ANGLE_UB);//�Ƕȼ���
			}
			i=MRR(ADDR_UB+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				MC(0,ADDR_ANGLE_UB,2);
			}
			break;
		case 28://UC�Ƕȼ���
			i=Calculate_Angle(0x28,ADDR_ANGLE_UC);//�Ƕȼ���
			i=MRR(ADDR_UC+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				MC(0,ADDR_ANGLE_UC,2);
			}
			break;
		case 29://IA�Ƕȼ���
			i=Calculate_Angle(0x18,ADDR_ANGLE_IA);//�Ƕȼ���
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				MC(0,ADDR_ANGLE_IA,2);
			}
			break;
		case 30://IB�Ƕȼ���
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				i=Calculate_Angle(0x19,ADDR_ANGLE_IB);//�Ƕȼ���
				i=MRR(ADDR_IB,4);
				i&=0x7fffffff;
				i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
				i=bcd_hex(i);
				if(i<((MeterIBhex*2)/3))
				{//����<(1%IB)��2/3
					MC(0,ADDR_ANGLE_IB,2);
				}
			}
			else
			{
				MC(0,ADDR_ANGLE_IB,2);
			}
			break;
		case 31://IC�Ƕȼ���
			i=Calculate_Angle(0x1A,ADDR_ANGLE_IC);//�Ƕȼ���
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				MC(0,ADDR_ANGLE_IC,2);
			}
			break;
		case 32://��ѹ�����н�
			Calculate_AngleUI(ADDR_ANGLE_UA,ADDR_ANGLE_IA,ADDR_ANGLE_UIA);//��ѹ�����н�
			Calculate_AngleUI(ADDR_ANGLE_UB,ADDR_ANGLE_IB,ADDR_ANGLE_UIB);//��ѹ�����н�
			Calculate_AngleUI(ADDR_ANGLE_UC,ADDR_ANGLE_IC,ADDR_ANGLE_UIC);//��ѹ�����н�
			x=0;
			i=MRR(ADDR_UA+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x=1;
			}
			if(x!=0)
			{
				MC(0x0,ADDR_ANGLE_UIA,2);
			}
			x=0;
			i=MRR(ADDR_UB+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IB,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x=1;
			}
			if(x!=0)
			{
				MC(0x0,ADDR_ANGLE_UIB,2);
			}
			x=0;
			i=MRR(ADDR_UC+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//��ѹ<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//ȡ4λС��,��αȵ���xx.xx(A)��1%IBҲ��4λС��
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//����<(1%IB)��2/3
				x=1;
			}
			if(x!=0)
			{
				MC(0x0,ADDR_ANGLE_UIC,2);
			}
			break;
		
		case 33://���������й�����
			Meter07_ActiveEnergy_Total();//�����й�����
			break;
		case 34://�������޹�����
			Meter07_ReactiveEnergy_Total();//�����޹�����
			break;
		case 35://A�������й�����
			Meter07_ActiveEnergy_Phase(0x1E,1<<0,ADDR_00150000);//�����й�����,���:����ܵ�ַ
			break;
		case 36://B�������й�����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				Meter07_ActiveEnergy_Phase(0x1F,1<<1,ADDR_00290000);//�����й�����,���:����ܵ�ַ
			}
			break;
		case 37://C�������й�����
			Meter07_ActiveEnergy_Phase(0x20,1<<2,ADDR_003D0000);//�����й�����,���:����ܵ�ַ
			break;
		case 38://A�����޹�����
			Meter07_ReactiveEnergy_Phase(0x22,1<<4,ADDR_00150000);//�����й�����,���:����ܵ�ַ
			break;
		case 39://B�����޹�����
			if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
			{
				Meter07_ReactiveEnergy_Phase(0x23,1<<5,ADDR_00290000);//�����й�����,���:����ܵ�ַ
			}
			break;
		case 40://C�����޹�����
			Meter07_ReactiveEnergy_Phase(0x24,1<<6,ADDR_003D0000);//�����й�����,���:����ܵ�ַ
			break;
		
		
		
		default:
			//ע�����ٶȺܿ�,ÿ����ѭ��ֻԼXXXms,�ʼ��붨ʱÿ��һ��ѭ��
			if(Meter_Ram->Read90E36Timer!=0)
			{
				return;
			}
			Meter_Ram->Read90E36Timer=1000/10;//ѭ����90E36��������ÿ10MS��1��ʱ��
			Meter_Ram->I1SubTask1=0;//I1�ӷ�ʱ����1,���ڱ�������:��ѹ,����,����,...��
			if(Comm_Ram->SPI2Timer==0)//�´β�����д��2���(��Meter07_Measure()�м���ֵ)
			{
			#if (USER%100)!=0
				//������дʱReNum=ֻ��2����EMC����ʱ���ж���λ7022����ѹ�����Ȼ�˲��Ϊ0
				Meter_Ram->EMUParaCount=0;//����оƬ������ʼ������
				Meter_Ram->EMUInit=4;//4=������ʼ��
			#endif
			}
			return;
	}
	Meter_Ram->I1SubTask1++;//I1�ӷ�ʱ����1,���ڱ�������:��ѹ,����,����,...��
}


#endif//#if IC_ATT7022==1//0=û,1=��




