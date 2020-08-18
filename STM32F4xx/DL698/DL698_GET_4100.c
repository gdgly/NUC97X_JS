
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET_4100.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"


u32 Get_MeterSpec(void)//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
{
	u32 i;
	u32 Umax;
	
	Umax=MRR(ADDR_UA+2,2);
	i=MRR(ADDR_UB+2,2);
	if(i>Umax)
	{
		Umax=i;
	}
	i=MRR(ADDR_UC+2,2);
	if(i>Umax)
	{
		Umax=i;
	}
	if(Umax>0x1300)//130.0V
	{
		Meter_Ram->PW=0;//��ʼ���߷�ʽ;0=��������,1=��������
		return 0;//0=220V,1=100V,2=57.7V
	}
	if(Umax>0x0750)//75.0V
	{
		Meter_Ram->PW=1;//��ʼ���߷�ʽ;0=��������,1=��������
		return 1;//0=220V,1=100V,2=57.7V
	}
	Meter_Ram->PW=0;//��ʼ���߷�ʽ;0=��������,1=��������
	return 2;//0=220V,1=100V,2=57.7V
}

u32 GET_Request_4100(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	if(LENmax_TxSPACE<8)
	{
		GET_Request_Frame(PORTn);//��֡��־
		return 0;
	}
	switch(OAD)
	{
		case 0x41040200://���ѹ
			switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
			{
				case 0:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=4;
					p8tx[2]='2';
					p8tx[3]='2';
					p8tx[4]='0';
					p8tx[5]='V';
					return 6;
				case 1:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=4;
					p8tx[2]='1';
					p8tx[3]='0';
					p8tx[4]='0';
					p8tx[5]='V';
					return 6;
				default:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=5;
					p8tx[2]='5';
					p8tx[3]='7';
					p8tx[4]='.';
					p8tx[5]='7';
					p8tx[6]='V';
					return 7;
			}
		case 0x41050200://�����/��������
			switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
			{
				case 0:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=2;
					p8tx[2]='1';
					p8tx[3]='A';
					return 4;
				default:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=4;
					p8tx[2]='1';
					p8tx[3]='.';
					p8tx[4]='5';
					p8tx[5]='A';
					return 6;
			}
		case 0x41060200://������
			switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
			{
				case 0:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=3;
					p8tx[2]='8';
					p8tx[3]='0';
					p8tx[4]='A';
					return 5;
				default:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=2;
					p8tx[2]='6';
					p8tx[3]='A';
					return 4;
			}
		case 0x41090200://���ܱ��й�����
		case 0x410A0200://���ܱ��޹�����
			switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
			{
				case 0:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=4;
					p8tx[2]='3';
					p8tx[3]='2';
					p8tx[4]='0';
					p8tx[5]='0';
					return 6;
				default:
					p8tx[0]=DataType_visible_string;
					p8tx[1]=5;
					p8tx[2]='1';
					p8tx[3]='0';
					p8tx[4]='0';
					p8tx[5]='0';
					p8tx[6]='0';
					return 7;
			}
	}
	return 0;
}








