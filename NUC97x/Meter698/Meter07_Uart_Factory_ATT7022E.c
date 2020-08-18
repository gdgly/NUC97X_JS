

//Meter_Uart_����
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "Meter07_Uart_Factory_ATT7022E.h"
#include "../../STM32F4xx/Meter/Meter07_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"


#if IC_ATT7022==1//0=û,1=��

extern const u16 MeterCalInitLib[];//У�������ʼ����

u32 UI_gain0(u32 Ur,u32 ADDR_PARA)//�������gainΪ0ʱ��������
{
//gain=Ur/Urms-1
//Urms=Ur/(gain+1)
	u32 i;
	float Urms;
	float gain;
	i=MRR(ADDR_PARA,2);
	if((i&(1<<15))==0)
	{
		gain=i;
		gain+=1<<15;
		gain/=1<<15;
	}
	else
	{
		i=~i;
		i++;
		i&=0xffff;
		gain=1<<15;
		gain-=i;
		gain/=1<<15;
	}
	Urms=Ur;
	Urms/=gain;
	i=(u32)Urms;
	return i;
}

u32 Calculate_U(u32 Addr,u32 ADDR_DATA);//��ѹ����
void ADJ_U(u32 PORTn,u32 REGaddr,u32 ADDR_PARA)//��ѹ����У��
{
//��Ugain=0ʱ����׼���϶���ʵ�������ѹ��ЧֵUr��ͨ��SPI�ڶ���������ѹ��Чֵ�Ĵ�����ֵΪDataU
//������ѹ��ЧֵUrms=DataU/2^13
//Ugain=Ur/Urms-1
//���Ugain��0����Ugain=INT[Ugain*2^15]
//���Ugain<0����Ugain=INT[2^16+ Ugain*2^15]
	
	u32 i;
	float Ur;
	float Um;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	Ur=i;//4λС��
	i=Calculate_U(REGaddr,0);
	i>>=4;//4λС��
	i=bcd_hex(i);
	i=UI_gain0(i,ADDR_PARA);//�������gainΪ0ʱ��������
	Um=i;
	
	Ur/=Um;
	Ur-=1;
	if(Ur>=0)
	{
		Ur*=(1<<15);
	}
	else
	{
		Ur*=(1<<15);
		Ur+=(1<<16);
	}
	i=(u32)Ur;
	MWR(i,ADDR_PARA,2);
}

u32 Calculate_I(u32 Addr,u32 ADDR_DATA);//��������
void ADJ_I(u32 PORTn,u32 REGaddr,u32 ADDR_PARA)//��������У��
{
//��Igain=0ʱ����׼���϶���ʵ�����������ЧֵIr��ͨ��SPI�ڶ���������ѹ��Чֵ�Ĵ�����ֵΪDataI
//������ѹ��ЧֵIrms=(DataI/2^13)/N (�������Ӧȡ���ź�Ϊ25mV����N=30/Ib���������Ӧȡ���ź�Ϊ50mV����N=60/Ib�����4.2.2��Чֵ�Ĵ�������)
//Igain=Ir/Irms-1
//���Igain��0����Igain=INT[Igain*2^15]
//���Igain��0����Igain=INT[2^16+ Igain*2^15]
	u32 i;
	float Ir;
	float Im;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	Ir=i;//4λС��
	
	i=Calculate_I(REGaddr,0);
	i>>=4;//4λС��
	i=bcd_hex(i);
	i=UI_gain0(i,ADDR_PARA);//�������gainΪ0ʱ��������
	Im=i;
	
	Ir/=Im;
	Ir-=1;
	if(Ir>=0)
	{
		Ir*=(1<<15);
	}
	else
	{
		Ir*=(1<<15);
		Ir+=(1<<16);
	}
	i=(u32)Ir;
	MWR(i,ADDR_PARA,2);
}





u32 ADJ_Calculate_P(u32 Addr,u32 ADDR_DATA)//���ʼ���
{
//���ʼĴ�����ʽ���壺
//A/B/C���๦�ʲ�����X
//X��24λ���ݣ�������ʽ
//���X>2^23����XX=X-2^24
//����XX=X
//ʵ�ʵ�A/B/C���๦�ʲ���Ϊ��XXX=XX*K������KΪ���ʲ���ϵ�������й��ʲ������ã�
//��λ�����ʵ�λ����(W)������ϵ��K=2.592*10^10/(HFconst*EC*2^23)
	u32 Data;
	//float K;
	double K;
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

void ADJ_P(u32 PORTn,u32 REGaddr,u32 ADDR_P,u32 ADDR_Q,u32 ADDR_S)//��������У��
{
//��׼���϶������Ϊerr%
//���㹫ʽ��
//	Pgain=-err%/(1+err%)
//���Pgain>=0����GP1=INT[Pgain*2^15]
//����Pgain<0�� ��GP1=INT[2^16+Pgain*2^15]
	u32 i;
	float Pr;
	float Pm;
	float err;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	Pr=i;
	i=ADJ_Calculate_P(REGaddr,0);//���ʼ���
	i&=0x7fffffff;
	i=bcd_hex(i);
	Pm=i;
	Pm/=100;//6λ��4λС��
	err=(Pm-Pr)/Pr;
	Pr=(-err)/(1+err);
	if(Pr>=0)
	{
		Pr*=(1<<15);
	}
	else
	{
		Pr*=(1<<15);
		Pr+=(1<<16);
	}
	i=(u32)Pr;
	i+=MRR(ADDR_P,2);
	MWR(i,ADDR_P,2);
	MWR(i,ADDR_Q,2);
	MWR(i,ADDR_S,2);
}

void ADJ_TEMP(u32 PORTn)//�¶�У��
{
//����TPSһ����У����ʹ��TPSData����������0x2A��ֵ�ڳ��£�25�ȣ����Ϊ0x00��У����ʽ��
//ֱ�Ӷ�ȡTPSData����������0x2A���ڳ��£�25�ȣ����ֵ��ֱ��д��ToffsetУ���Ĵ���(У�����0x6B)���ɡ�
	u32 i;
	
	i=Read_ATT7022(0x2A);
	MWR(i,ADDR_EMU_6B,2);
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	i/=10000;//ȥ4λС��
	MWR(i,ADDR_MeterCalibration_IEEPROM_START+10,2);//�¶�CAL(ATT7022���¶�У��ʱ�ĵ�ǰ�¶�ֵxx��(hex,�������¶�ֵ)
}

void ADJ_Offset(u32 REGaddr,u32 ADDR_EMU_x)
{
//�����ź�Ϊ0��ʱ�򣬶�ȡ�Ĵ�����ֵIrms
//���㹫ʽ��IrmsOffset = (Irms^2)/ (2^15)��
//�ڷ�����Чֵ����У��֮ǰ������ItRmsoffsetУ��(У�����0x6A)��
//��֪�������ź�Ϊ0��ʱ�򣬶�ȡ�Ĵ�����ֵItrms
//���㹫ʽ��ItRmsoffset = (Irms^2)/ (2^15)
//��Чֵ�͹��ʵĸ�������ѡ��=1��ʾ����(1.76Hz)��=0����(14.4Hz)��
	u32 i;
	
	i=Read_ATT7022(REGaddr);
	i*=i;
	i/=1<<15;
	i+=MRR(ADDR_EMU_x,2);
	MWR(i,ADDR_EMU_x,2);
}

void ADJ_ERR10(u32 PORTn,u32 ADDR_P,u32 ADDR_Q,u32 ADDR_S)//1.0 Ib������
{
//��׼���϶������Ϊerr%
//���㹫ʽ��
//	Pgain=-err%/(1+err%)
//���Pgain>=0����GP1=INT[Pgain*2^15]
//����Pgain<0�� ��GP1=INT[2^16+Pgain*2^15]
	u32 i;
	float err;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	if(i&0x80000000)
	{
		i=~i;
		i++;
		err=i;
		err*=-1;
	}
	else
	{
		err=i;
	}
	err/=1000000;//����ʱ4λС��+err%��2λС��
	err=(-err)/(1+err);
	if(err>=0)
	{
		err*=(1<<15);
	}
	else
	{
		err*=(1<<15);
		err+=(1<<16);
	}
	i=(u32)err;
	i+=MRR(ADDR_P,2);
	MWR(i,ADDR_P,2);
	MWR(i,ADDR_Q,2);
	MWR(i,ADDR_S,2);
}

void ADJ_ERR05L(u32 PORTn,u32 ADDR_Q1,u32 ADDR_Q2,u32 ADDR_Q3)//05L Ib������
{
//�ڹ�������cos(��)=1ʱ�����������Ѿ�У����֮���ٽ�����λ��������λУ����cos(��)=0.5Lʱ����У����
//��֪��0.5L����׼��������Ϊerr%
//��λ������ʽ��
//��=(-err%)/1.732
//����� >= 0��PhSregpq = INT[�� *2 ^15]
//����� < 0��PhSregpq = INT[2 ^16 +�� *2 ^15]
	u32 i;
	float err;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	if(i&0x80000000)
	{
		i=~i;
		i++;
		err=i;
		err*=-1;
	}
	else
	{
		err=i;
	}
	err/=1000000;//����ʱ4λС��+err%��2λС��
	err=(-err)/1.732;
	if(err>=0)
	{
		err*=(1<<15);
	}
	else
	{
		err*=(1<<15);
		err+=(1<<16);
	}
	i=(u32)err;
	i+=MRR(ADDR_Q1,2);
	MWR(i,ADDR_Q1,2);
	MWR(i,ADDR_Q2,2);
	MWR(i,ADDR_Q3,2);
}

void ADJ_ERR10Ib5(u32 PORTn,u32 ADDR_P,u32 ADDR_Q,u32 ADDR_PL,u32 ADDR_QL)//5%Ib 1.0 ������
{
	
	
	
}




void DL645_07_Factory_ATT7022(u32 PORTn)//Meter07_����ģʽ:У������д,��ʼ�����ݶ�д
{
	u8 * p8Rx;
	

	if(Comm_Ram->Factory!=0x55)//0x55=����״̬
	{
		return;
	}
	p8Rx=(u8 *)(Get_ADDR_UARTnRx(PORTn));

//���0=��,1=A,2=B,3=C,4=N,5������������������(��ʼ�����ݶ�д)
	if(p8Rx[10]<=4)
	{//���<=4
//У�����:0=��,1=1.0���,2=0.5L���,3=,4=,5=,6=,7=ʱ�������,8=����Ƶ��,9=�¶�,10=,11=,12=��ѹ����,13=��������,14=�����ֵʱƫ��,15=���ѹֵʱƫ��,16=�㹦��ֵʱƫ��,17=��Ǳ������,18=У���ʼ��	
		switch(p8Rx[11])
		{
			case 1://1.0���
				switch(p8Rx[10])//��ǰУ����
				{
					case 1://A
						ADJ_ERR10(PORTn,ADDR_EMU_04,ADDR_EMU_07,ADDR_EMU_0A);//1.0 Ib������
						break;
					case 2://B
						ADJ_ERR10(PORTn,ADDR_EMU_05,ADDR_EMU_08,ADDR_EMU_0B);//1.0 Ib������
						break;
					case 3://C
						ADJ_ERR10(PORTn,ADDR_EMU_06,ADDR_EMU_09,ADDR_EMU_0C);//1.0 Ib������
						break;
				}
				break;
			case 2://0.5L���
				switch(p8Rx[10])//��ǰУ����
				{
					case 1://A
						ADJ_ERR05L(PORTn,ADDR_EMU_0D,ADDR_EMU_10,ADDR_EMU_61);//05L Ib������
						break;
					case 2://B
						ADJ_ERR05L(PORTn,ADDR_EMU_0E,ADDR_EMU_11,ADDR_EMU_62);//05L Ib������
						break;
					case 3://C
						ADJ_ERR05L(PORTn,ADDR_EMU_0F,ADDR_EMU_12,ADDR_EMU_63);//05L Ib������
						break;
				}
				break;
			case 3://����1.0���
				break;
			case 4://5%Ib 1.0 ������
				switch(p8Rx[10])//��ǰУ����
				{
					case 1://A
						
						break;
					case 2://B
						
						break;
					case 3://C
						
						break;
				}
				break;
			
			case 6://ֱ��ģ����У׼
				break;
			case 9://�¶�У��
				ADJ_TEMP(PORTn);//�¶�У��
				break;
			case 10://�й�����У��
				switch(p8Rx[10])//��ǰУ����
				{
					case 0://��
						ADJ_P(PORTn,0x01,ADDR_EMU_04,ADDR_EMU_07,ADDR_EMU_0A);//��������У��
						ADJ_P(PORTn,0x02,ADDR_EMU_05,ADDR_EMU_08,ADDR_EMU_0B);//��������У��
						ADJ_P(PORTn,0x03,ADDR_EMU_06,ADDR_EMU_09,ADDR_EMU_0C);//��������У��
						break;
					case 1://A
						ADJ_P(PORTn,0x01,ADDR_EMU_04,ADDR_EMU_07,ADDR_EMU_0A);//��������У��
						break;
					case 2://B
						ADJ_P(PORTn,0x02,ADDR_EMU_05,ADDR_EMU_08,ADDR_EMU_0B);//��������У��
						break;
					case 3://C
						ADJ_P(PORTn,0x03,ADDR_EMU_06,ADDR_EMU_09,ADDR_EMU_0C);//��������У��
						break;
				}
				break;
			
			case 12://��ѹУ��
				switch(p8Rx[10])//��ǰУ����
				{
					case 0://��
						ADJ_U(PORTn,0x0d,ADDR_EMU_17);//��ѹ����У��
						ADJ_U(PORTn,0x0e,ADDR_EMU_18);//��ѹ����У��
						ADJ_U(PORTn,0x0f,ADDR_EMU_19);//��ѹ����У��
						break;
					case 1://A
						ADJ_U(PORTn,0x0d,ADDR_EMU_17);//��ѹ����У��
						break;
					case 2://B
						ADJ_U(PORTn,0x0e,ADDR_EMU_18);//��ѹ����У��
						break;
					case 3://C
						ADJ_U(PORTn,0x0f,ADDR_EMU_19);//��ѹ����У��
						break;
				}
				break;
			case 13://����У��
				switch(p8Rx[10])//��ǰУ����
				{
					case 0://��
						ADJ_I(PORTn,0x10,ADDR_EMU_1A);//��������У��
						ADJ_I(PORTn,0x11,ADDR_EMU_1B);//��������У��
						ADJ_I(PORTn,0x12,ADDR_EMU_1C);//��������У��
						break;
					case 1://A
						ADJ_I(PORTn,0x10,ADDR_EMU_1A);//��������У��
						break;
					case 2://B
						ADJ_I(PORTn,0x11,ADDR_EMU_1B);//��������У��
						break;
					case 3://C
						ADJ_I(PORTn,0x12,ADDR_EMU_1C);//��������У��
						break;
				}
				break;
			case 14://�����ֵʱƫ��
				switch(p8Rx[10])//��ǰУ����
				{
					case 0://��
						ADJ_Offset(0x10,ADDR_EMU_27);
						ADJ_Offset(0x11,ADDR_EMU_28);
						ADJ_Offset(0x12,ADDR_EMU_29);
						ADJ_Offset(0x13,ADDR_EMU_6A);//����ʸ����(ItRms)
						break;
					case 1://A
						ADJ_Offset(0x10,ADDR_EMU_27);
						break;
					case 2://B
						ADJ_Offset(0x11,ADDR_EMU_28);
						break;
					case 3://C
						ADJ_Offset(0x12,ADDR_EMU_29);
						break;
				}
				break;
			case 15://���ѹֵʱƫ��
				switch(p8Rx[10])//��ǰУ����
				{
					case 1://A
						ADJ_Offset(0x0d,ADDR_EMU_24);
						break;
					case 2://B
						ADJ_Offset(0x0e,ADDR_EMU_25);
						break;
					case 3://C
						ADJ_Offset(0x0f,ADDR_EMU_26);
						break;
				}
				break;
			case 16://

				break;
			case 17://��Ǳ�����
				
				break;
			case 18://У���ʼ��
				MW((u32)MeterCalInitLib,ADDR_MeterCalibration_IEEPROM_START,ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START);
				break;
			default:
				return;
		}
		Meter_Ram->EMUParaCount=0;
    Meter_Ram->EMUInit=4;//0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
		DL645_07_WriteOKACK(PORTn);//��ȷд�ش�
	}
	else
	{//���>4
//���0=��,1=A,2=B,3=C,4=N,5������������������(��ʼ�����ݶ�д)	
	
	
	}
}

#endif//#if IC_ATT7022E==1//0=û,1=��






























