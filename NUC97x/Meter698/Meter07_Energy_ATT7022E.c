
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Meter698/Meter07_Energy_ATT7022E.h"
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_DataDef.h"




#if IC_ATT7022==1//0=û,1=��

void GroupActiveEnergy_Total(void)//��������й�����
{
	u32 i;
	u32 word;
	u64 x;
	u64 y;

	//ȡ�����й����ܲ�תΪhex
	MR(ADDR_DATABUFF,ADDR_00010000,5*(1+MMaxTariff));
	MR(ADDR_DATABUFF+(5*(1+MMaxTariff)),ADDR_00020000,5*(1+MMaxTariff));
	for(i=0;i<(2*(1+MMaxTariff));i++)
	{
		x=MRR(ADDR_DATABUFF+(5*i),5);
		x=bcd16_hex(x);
		MWR(x,ADDR_DATABUFF+(5*i),5);
	}
	//����ϵ��������
	MC(0,ADDR_DATABUFF+(2*(5*(1+MMaxTariff))),8*(1+MMaxTariff));
	//ȡ���ģʽ��
#if MainProtocol==698
	word=MRR(ADDR_4112+2,1);
	word=Bit_Inverse(word,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
#else
	word=MRR(ADDR_04000601,1);
#endif
	//���
	if((word&0x1)!=0x0)
	{//�����
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y+=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	if((word&0x2)!=0x0)
	{//�����
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y-=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	if((word&0x4)!=0x0)
	{//�����
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y+=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	if((word&0x8)!=0x0)
	{//�����
		for(i=0;i<(1+MMaxTariff);i++)
		{
			x=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+(5*i),5);
			y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
			y-=x;
			MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		}
	}
	for(i=0;i<(1+MMaxTariff);i++)
	{
		y=MRR(ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(8*i),8);
		if((y&0x8000000000000000)!=0x0)
		{
			y=(~y)+1;
			x=0x8000000000;
		}
		else
		{
			x=0;
		}
		y%=8000000000;
		y=hex_bcd16(y);
		y|=x;
		MWR(y,ADDR_DATABUFF+(2*(5*(1+MMaxTariff)))+(5*i),5);
	}
	MW(ADDR_DATABUFF+(2*(5*(1+MMaxTariff))),ADDR_00000000,5*(1+MMaxTariff));
}

void GroupReactiveEnergy_Total(void)//�������1��2�޹�����
{
	u32 i;
	u32 word;
	u32 group;
	u64 x;
	u64 y;

	MR(ADDR_DATABUFF,ADDR_00050000,5*(MMaxTariff+1));
	MR(ADDR_DATABUFF+(5*(MMaxTariff+1)),ADDR_00060000,5*(MMaxTariff+1));
	MR(ADDR_DATABUFF+(10*(MMaxTariff+1)),ADDR_00070000,5*(MMaxTariff+1));
	MR(ADDR_DATABUFF+(15*(MMaxTariff+1)),ADDR_00080000,5*(MMaxTariff+1));
	for(i=0;i<(4*(MMaxTariff+1));i++)
	{
		x=MRR(ADDR_DATABUFF+(5*i),5);
		x=bcd16_hex(x);
		MWR(x,ADDR_DATABUFF+(5*i),5);
	}
	for(group=1;group<3;group++)
	{
#if MainProtocol==698
		if(group==1)
		{
			word=MRR(ADDR_4113+2,1);
		}
		else
		{
			word=MRR(ADDR_4114+2,1);
		}
		word=Bit_Inverse(word,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
#else
		if(group==1)
		{
			word=MRR(ADDR_04000601+1,1);
		}
		else
		{
			word=MRR(ADDR_04000601+2,1);
		}
#endif
		MC(0,ADDR_DATABUFF+(20*(MMaxTariff+1)),8*(MMaxTariff+1));//����ϵ��������
		if((word&0x1)!=0x0)
		{//1���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x2)!=0x0)
		{//1���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x4)!=0x0)
		{//2���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x8)!=0x0)
		{//2���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(5*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x10)!=0x0)
		{//3���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(10*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x20)!=0x0)
		{//3���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(10*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x40)!=0x0)
		{//4���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(15*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y+=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		if((word&0x80)!=0x0)
		{//4���޼�
			for(i=0;i<(MMaxTariff+1);i++)
			{
				x=MRR(ADDR_DATABUFF+(15*(MMaxTariff+1))+(5*i),5);
				y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
				y-=x;
				MWR(y,ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			}
		}
		for(i=0;i<(1+MMaxTariff);i++)
		{
			y=MRR(ADDR_DATABUFF+(20*(MMaxTariff+1))+(8*i),8);
			if((y&0x8000000000000000)!=0x0)
			{
				y=(~y)+1;
				x=0x8000000000;
			}
			else
			{
				x=0;
			}
			y%=8000000000;
			y=hex_bcd16(y);
			y|=x;
			MWR(y,ADDR_DATABUFF+(20*(1+MMaxTariff))+(5*i),5);
		}
		if(group==1)
		{
			MW(ADDR_DATABUFF+(20*(1+MMaxTariff)),ADDR_00030000,5*(1+MMaxTariff));
		}	
		else
		{
			MW(ADDR_DATABUFF+(20*(1+MMaxTariff)),ADDR_00040000,5*(1+MMaxTariff));
		}
	}
}


void GroupReactiveEnergy_Phase(u32 ADDR_Energy)//�������1��2�޹�����
{
	u32 i;
	u32 word;
	u64 x;
	u64 y;
	
	//��1-4���޵���
	MR(ADDR_DATABUFF,ADDR_Energy+20,20);
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_DATABUFF+(5*i),5);
		x=bcd16_hex(x);
		MWR(x,ADDR_DATABUFF+(5*i),5);
	}
	//���1
	y=0;
#if MainProtocol==698
	word=MRR(ADDR_4113+2,1);
	word=Bit_Inverse(word,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
#else
	word=MRR(ADDR_04000601+1,1);
#endif
	x=MRR(ADDR_DATABUFF+0,5);
	if((word&0x1)!=0x0)
	{//1���޼�
		y+=x;
	}
	if((word&0x2)!=0x0)
	{//1���޼�
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+5,5);
	if((word&0x4)!=0x0)
	{//2���޼�
		y+=x;
	}
	if((word&0x8)!=0x0)
	{//2���޼�
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+10,5);
	if((word&0x10)!=0x0)
	{//3���޼�
		y+=x;
	}
	if((word&0x20)!=0x0)
	{//3���޼�
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+15,5);
	if((word&0x40)!=0x0)
	{//4���޼�
		y+=x;
	}
	if((word&0x80)!=0x0)
	{//4���޼�
		y-=x;
	}
	if((y&0x8000000000000000)!=0x0)
	{
		y=(~y)+1;
		x=0x8000000000;
	}
	else
	{
		x=0;
	}
	y%=8000000000;
	y=hex_bcd16(y);
	y|=x;
	MWR(y,ADDR_Energy+10,5);
	//���2
	y=0;
#if MainProtocol==698
	word=MRR(ADDR_4114+2,1);
	word=Bit_Inverse(word,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
#else
	word=MRR(ADDR_04000601+2,1);
#endif
	x=MRR(ADDR_DATABUFF+0,5);
	if((word&0x1)!=0x0)
	{//1���޼�
		y+=x;
	}
	if((word&0x2)!=0x0)
	{//1���޼�
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+5,5);
	if((word&0x4)!=0x0)
	{//2���޼�
		y+=x;
	}
	if((word&0x8)!=0x0)
	{//2���޼�
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+10,5);
	if((word&0x10)!=0x0)
	{//3���޼�
		y+=x;
	}
	if((word&0x20)!=0x0)
	{//3���޼�
		y-=x;
	}
	x=MRR(ADDR_DATABUFF+15,5);
	if((word&0x40)!=0x0)
	{//4���޼�
		y+=x;
	}
	if((word&0x80)!=0x0)
	{//4���޼�
		y-=x;
	}
	if((y&0x8000000000000000)!=0x0)
	{
		y=(~y)+1;
		x=0x8000000000;
	}
	else
	{
		x=0;
	}
	y%=8000000000;
	y=hex_bcd16(y);
	y|=x;
	MWR(y,ADDR_Energy+15,5);
	//CRC
	MR(ADDR_DATABUFF,ADDR_Energy,85+17);
	//i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+85+17);//RAM������CRC32����,Len=4*1024ʱ��86us(cclk=96mhz)
	i=RAM_CRC32(ADDR_DATABUFF,85+17);//RAM������CRC32����
	MWR(i,ADDR_Energy+85+17,4);
}


void Meter07_ActiveEnergy_Total(void)//�����й�����
{
	u32 i;
	u32 cf;
	u32 Tariff;
	u64 Energy;
	u32 Data;
	u32 Dir;
	
//��
	Data=Security_Read_ATT7022(0x21);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	Dir=Security_Read_ATT7022(0x3D);//���ʷ���Ĵ���
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//���Ǽ�Ϊ��ֵ
	}
#if MeterMaxDemandCalculate==0//����������㣺0=���巨,1=����ƽ����
//��������
	i=Data;
	i+=Meter_Ram->ActiveImp;
	Meter_Ram->ActiveImp=i%100;
	Meter_Ram->ActiveImpShift[0]+=(i/100);
//1����ƽ���й���������
	Meter_Ram->MeanImp+=(i/100);
#endif

//����
/*
	//����=ʵʱ��
	MW(ADDR_YMDHMS+1,ADDR_00010000,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+5,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+10,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+15,5);
	MW(ADDR_YMDHMS+1,ADDR_00010000+20,5);
	return;
*/
	if((Data)&&((Dir&(1<<3))==0))
	{
		//ȡ��������
		MR(ADDR_DATABUFF,ADDR_00010000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
		//������
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
		//������ʵ���
		Tariff=Meter_Ram->Tariff;
		if((Tariff==0)||(Tariff>MMaxTariff))
		{
			Tariff=MMaxTariff;
		}
		Energy=MRR(ADDR_DATABUFF+(5*Tariff),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+(5*Tariff),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM������CRC32����,Len=4*1024ʱ��86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM������CRC32����
		MWR(i,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)),4);
		//д��
		MW(ADDR_DATABUFF,ADDR_00010000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
	}
//����
	if((Data)&&((Dir&(1<<3))!=0))
	{
		//ȡ��������
		MR(ADDR_DATABUFF,ADDR_00020000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
		//������
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
		//������ʵ���
		Tariff=Meter_Ram->Tariff;
		if((Tariff==0)||(Tariff>MMaxTariff))
		{
			Tariff=MMaxTariff;
		}
		Energy=MRR(ADDR_DATABUFF+(5*Tariff),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+(5*Tariff),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM������CRC32����,Len=4*1024ʱ��86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM������CRC32����
		MWR(i,ADDR_DATABUFF+(5*(1+MMaxTariff))+(1*(1+MMaxTariff)),4);
		//д��
		MW(ADDR_DATABUFF,ADDR_00020000,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
	}
//���
	GroupActiveEnergy_Total();//��Ϻ����й�����
}

void Meter07_ActiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy)//�����й�����,���:����ܵ�ַ
{
	u32 i;
	u32 cf;
	u64 Energy;
	u32 Data;
	u32 Dir;
	
//��
	Data=Security_Read_ATT7022(REGaddr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	Dir=Security_Read_ATT7022(0x3D);//���ʷ���Ĵ���
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//���Ǽ�Ϊ��ֵ
	}
	//ȡ��������
	MR(ADDR_DATABUFF,ADDR_Energy,85+17);
//����	
	if((Data)&&((Dir&DirBit)==0))
	{
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+85,1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+85,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
	}
//����
	if((Data)&&((Dir&DirBit)!=0))
	{
		Energy=MRR(ADDR_DATABUFF+5,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+85+1,1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+85+1,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+5,5);
	}
	//CRC
//	i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+85+17);//RAM������CRC32����,Len=4*1024ʱ��86us(cclk=96mhz)
	i=RAM_CRC32(ADDR_DATABUFF,85+17);//RAM������CRC32����
	MWR(i,ADDR_DATABUFF+85+17,4);
	//д��
	MW(ADDR_DATABUFF,ADDR_Energy,85+17+4);
}


void Meter07_ReactiveEnergy_Total(void)//�����޹�����
{
	u32 i;
	u32 cf;
	u32 Tariff;
	u32 ADDR_Energy;
	u64 Energy;
	u32 Data;
	u32 Dir;
	
//��
	Data=Security_Read_ATT7022(0x25);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	Dir=Security_Read_ATT7022(0x3D);//���ʷ���Ĵ���
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//���Ǽ�Ϊ��ֵ
	}
#if MeterMaxDemandCalculate==0//����������㣺0=���巨,1=����ƽ����
//��������
	i=Data;
	i+=Meter_Ram->ReactiveImp;
	Meter_Ram->ReactiveImp=i%100;
	Meter_Ram->ReactiveImpShift[0]+=(i/100);
#endif
//
	if(Data)
	{
		switch(Meter_Ram->TQ)//�ܵ�ǰ���� 1-4�ֱ��ʾ1-4����,0=�����ȷ��
		{
			case 1:
				ADDR_Energy=ADDR_00050000;
				break;
			case 2:
				ADDR_Energy=ADDR_00060000;
				break;
			case 3:
				ADDR_Energy=ADDR_00070000;
				break;
			case 4:
				ADDR_Energy=ADDR_00080000;
				break;
			default:
				return;
		}
		//ȡ��������
		MR(ADDR_DATABUFF,ADDR_Energy,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
		//�ܵ���
		Energy=MRR(ADDR_DATABUFF,5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterReactiveConst/100);
		cf%=(MeterReactiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff)),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF,5);
		//������ʵ���
		Tariff=Meter_Ram->Tariff;
		if((Tariff==0)||(Tariff>MMaxTariff))
		{
			Tariff=MMaxTariff;
		}
		Energy=MRR(ADDR_DATABUFF+(5*Tariff),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterActiveConst/100);
		cf%=(MeterActiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+(5*(1+MMaxTariff))+Tariff,1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+(5*Tariff),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))));//RAM������CRC32����,Len=4*1024ʱ��86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,(5*(1+MMaxTariff))+(1*(1+MMaxTariff)));//RAM������CRC32����
		MWR(i,ADDR_DATABUFF+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))),4);
		//д��
		MW(ADDR_DATABUFF,ADDR_Energy,((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4));
	}
	//��ϵ���
	GroupReactiveEnergy_Total();//�������1��2�޹�����
}

void Meter07_ReactiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy)//�����޹�����,���:����ܵ�ַ
{
	u32 i;
	u32 cf;
	u32 Quadrant;
	u64 Energy;
	u32 Data;
	u32 Dir;

//��
	Data=Security_Read_ATT7022(REGaddr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
	Dir=Security_Read_ATT7022(0x3D);//���ʷ���Ĵ���
	if(Data==0xffffffff)
	{
		return;
	}
	if(Dir==0xffffffff)
	{
		Dir=0;//���Ǽ�Ϊ��ֵ
	}
//
	if(Data)
	{
		switch(ADDR_Energy)
		{
			case ADDR_00150000:
				i=0;
				break;
			case ADDR_00290000:
				i=1;
				break;
			case ADDR_003D0000:
				i=2;
				break;
			default:
				return;
		}
		Quadrant=MRR(((u32)&Meter_Ram->AQ)+i,1);//ȡ��ǰ����
		if((Quadrant==0)||(Quadrant>4))
		{//���޴�
			Quadrant=1;
		}
		//ȡ��������
		MR(ADDR_DATABUFF,ADDR_Energy,85+17);
		//����
		Energy=MRR(ADDR_DATABUFF+20+(5*(Quadrant-1)),5);
		Energy=bcd16_hex(Energy);
		cf=MRR(ADDR_DATABUFF+87+4+(Quadrant-1),1);
		cf+=Data;
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0://220V
				i=cf/(MeterActiveConst220V/100);
				cf%=(MeterActiveConst220V/100);
				break;
			case 1://100V
				i=cf/(MeterActiveConst100V/100);
				cf%=(MeterActiveConst100V/100);
				break;
			default://57.7V
				i=cf/(MeterActiveConst57V/100);
				cf%=(MeterActiveConst57V/100);
				break;
		}
	#else
		i=cf/(MeterReactiveConst/100);
		cf%=(MeterReactiveConst/100);
	#endif
		i*=100;
		MWR(cf,ADDR_DATABUFF+87+4+(Quadrant-1),1);
		Energy+=i;
		Energy%=10000000000;
		Energy=hex_bcd16(Energy);
		MWR(Energy,ADDR_DATABUFF+20+(5*(Quadrant-1)),5);
		//CRC
//		i=RAMCRC32(ADDR_DATABUFF,ADDR_DATABUFF+85+17);//RAM������CRC32����,Len=4*1024ʱ��86us(cclk=96mhz)
		i=RAM_CRC32(ADDR_DATABUFF,85+17);//RAM������CRC32����
		MWR(i,ADDR_DATABUFF+85+17,4);
		//д��
		MW(ADDR_DATABUFF,ADDR_Energy,85+17+4);
	}
	//��ϵ���
	GroupReactiveEnergy_Phase(ADDR_Energy);//�������1��2�޹�����
}



void Meter07_AH(void)//��ʱ����
{
	u32 i;
	u32 s;
	u16 * p16;
	u32 * p32;
	
	s=Meter_Ram->AH_S_Timer;//��ʱ��������Ӽ���
	Meter_Ram->AH_S_Timer=0;
	if(s!=0x0)
	{
		p16=(u16 *)(ADDR_DATABUFF+12);
		p32=(u32 *)(ADDR_DATABUFF);
		MR(ADDR_DATABUFF,ADDR_AH,18);
//A
		i=MRR(ADDR_IA+1,3);
		i&=0x7fffff;
		i=bcd_hex(i);
		i*=s;
		i+=p16[0];
		p32[0]+=(i/36000);
		p16[0]=(i%36000);
//B
		i=MRR(ADDR_IB+1,3);
		i&=0x7fffff;
		i=bcd_hex(i);
		i*=s;
		i+=p16[1];
		p32[1]+=(i/36000);
		p16[1]=(i%36000);
//C
		i=MRR(ADDR_IC+1,3);
		i&=0x7fffff;
		i=bcd_hex(i);
		i*=s;
		i+=p16[2];
		p32[2]+=(i/36000);
		p16[2]=(i%36000);
		MW(ADDR_DATABUFF,ADDR_AH,18);
	}
}



#endif



