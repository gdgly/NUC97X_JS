
//�ڲ������¼���¼�ļ�
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/EVENT_CALC_iMeter.h"
#include "../DL698/EVENT_Record.h"

#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"



void EVENT_CALC_300F_3010(void)//��ѹ�����������¼�
{
	u32 lastOccur;
	
	if(((Meter_Ram->IPFlags>>7)&0x1)!=((Meter_Ram->IPFlags>>3)&0x1))
	{//��ѹ������仯
		Meter_Ram->VIP_S_Timer=30;//��ѹ�������붨ʱ��;���Ժ����ʱΪ30������30����FLASH��ȡֵ��Ϊʡʱ��
		Meter_Ram->IPFlags&=0xf7;
		Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>7)&0x1)<<3);
	}
	else
	{
		if(Meter_Ram->VIP_S_Timer==0x0)
		{
			Meter_Ram->VIP_S_Timer=30;//2;//��ѹ�������붨ʱ��,ÿ2�����н���
			Meter_Ram->IPFlags&=0xdf;
			Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>3)&0x1)<<5);//b7=��ѹ������,b6=����������,b5=��ѹ������ȷ��,b4=����������ȷ��,b3=��ѹ������˲ʱ,b2=����������˲ʱ,b1=, b0=
			lastOccur=GET_LastEventOccurRecovery(0x300f0200,0);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
			if(Meter_Ram->IPFlags&(1<<5))
			{//����
				if(lastOccur==0)
				{
					Event_OAD(0x300f0200,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
			else
			{//�ָ�
				if(lastOccur)
				{
					Event_OAD(0x300f0200,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
		}
	}
	if(((Meter_Ram->IPFlags>>6)&0x1)!=((Meter_Ram->IPFlags>>2)&0x1))
	{//����������仯
		Meter_Ram->IIP_S_Timer=60;//�����������붨ʱ��
		Meter_Ram->IPFlags&=0xfb;
		Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>6)&0x1)<<2);
	}
	else
	{
		if(Meter_Ram->IIP_S_Timer==0x0)
		{
			Meter_Ram->IIP_S_Timer=60;//2;//�����������붨ʱ��,ÿ2�����н���
			Meter_Ram->IPFlags&=0xef;
			Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>2)&0x1)<<4);//b7=��ѹ������,b6=����������,b5=��ѹ������ȷ��,b4=����������ȷ��,b3=��ѹ������˲ʱ,b2=����������˲ʱ,b1=, b0=
			lastOccur=GET_LastEventOccurRecovery(0x30100200,0);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
			if(Meter_Ram->IPFlags&(1<<4))
			{//����
				if(lastOccur==0)
				{
					Event_OAD(0x30100200,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
			else
			{//�ָ�
				if(lastOccur)
				{
					Event_OAD(0x30100200,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
		}
	}
}

void EventCurrentvalue_NUM(u8* pADDR,u32 Phase)//�¼���ǰֵ��¼��������+1�����:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��)
{
	u32 i;
//��ǰֵ��¼���=array structure
//{
//	�¼��������� double-long-unsigned��
//	�¼��ۼ�ʱ�� double-long-unsigned 
//}	
	pADDR[0]=DataType_array;
	pADDR[1]=4;
	//��
	pADDR[2]=DataType_structure;
	pADDR[3]=2;
	pADDR[4]=DataType_double_long_unsigned;
	if(Phase)
	{
		i=(pADDR[5]<<24)|(pADDR[6]<<16)|(pADDR[7]<<8)|pADDR[8];
		i++;
		pADDR[5]=i>>24;
		pADDR[6]=i>>16;
		pADDR[7]=i>>8;
		pADDR[8]=i;
	}
	pADDR[9]=DataType_double_long_unsigned;
	//A��
	pADDR[14]=DataType_structure;
	pADDR[15]=2;
	pADDR[16]=DataType_double_long_unsigned;
	if(Phase&1)
	{
		i=(pADDR[17]<<24)|(pADDR[18]<<16)|(pADDR[19]<<8)|pADDR[20];
		i++;
		pADDR[17]=i>>24;
		pADDR[18]=i>>16;
		pADDR[19]=i>>8;
		pADDR[20]=i;
	}
	pADDR[21]=DataType_double_long_unsigned;
	//B��
	pADDR[26]=DataType_structure;
	pADDR[27]=2;
	pADDR[28]=DataType_double_long_unsigned;
	if(Phase&2)
	{
		i=(pADDR[29]<<24)|(pADDR[30]<<16)|(pADDR[31]<<8)|pADDR[32];
		i++;
		pADDR[29]=i>>24;
		pADDR[30]=i>>16;
		pADDR[31]=i>>8;
		pADDR[32]=i;
	}
	pADDR[33]=DataType_double_long_unsigned;
	//C��
	pADDR[38]=DataType_structure;
	pADDR[39]=2;
	pADDR[40]=DataType_double_long_unsigned;
	if(Phase&4)
	{
		i=(pADDR[41]<<24)|(pADDR[42]<<16)|(pADDR[43]<<8)|pADDR[44];
		i++;
		pADDR[41]=i>>24;
		pADDR[42]=i>>16;
		pADDR[43]=i>>8;
		pADDR[44]=i;
	}
	pADDR[45]=DataType_double_long_unsigned;
}

void EventCurrentvalue_TIME(u8* pADDR,u32 Phase,u32 s)//�¼���ǰֵ��¼���ۼ�ʱ�䣬���:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��),s=ʱ����
{
	u32 i;
//��ǰֵ��¼���=array structure
//{
//	�¼��������� double-long-unsigned��
//	�¼��ۼ�ʱ�� double-long-unsigned 
//}	
	pADDR[0]=DataType_array;
	pADDR[1]=4;
	//��
	pADDR[2]=DataType_structure;
	pADDR[3]=2;
	pADDR[4]=DataType_double_long_unsigned;
	pADDR[9]=DataType_double_long_unsigned;
	if(Phase)
	{
		i=(pADDR[10]<<24)|(pADDR[11]<<16)|(pADDR[12]<<8)|pADDR[13];
		i+=s;
		pADDR[10]=i>>24;
		pADDR[11]=i>>16;
		pADDR[12]=i>>8;
		pADDR[13]=i;
	}
	//A��
	pADDR[14]=DataType_structure;
	pADDR[15]=2;
	pADDR[16]=DataType_double_long_unsigned;
	pADDR[21]=DataType_double_long_unsigned;
	if(Phase&1)
	{
		i=(pADDR[22]<<24)|(pADDR[23]<<16)|(pADDR[24]<<8)|pADDR[25];
		i+=s;
		pADDR[22]=i>>24;
		pADDR[23]=i>>16;
		pADDR[24]=i>>8;
		pADDR[25]=i;
	}
	//B��
	pADDR[26]=DataType_structure;
	pADDR[27]=2;
	pADDR[28]=DataType_double_long_unsigned;
	pADDR[33]=DataType_double_long_unsigned;
	if(Phase&2)
	{
		i=(pADDR[34]<<24)|(pADDR[35]<<16)|(pADDR[36]<<8)|pADDR[37];
		i+=s;
		pADDR[34]=i>>24;
		pADDR[35]=i>>16;
		pADDR[36]=i>>8;
		pADDR[37]=i;
	}
	//C��
	pADDR[38]=DataType_structure;
	pADDR[39]=2;
	pADDR[40]=DataType_double_long_unsigned;
	pADDR[45]=DataType_double_long_unsigned;
	if(Phase&4)
	{
		i=(pADDR[46]<<24)|(pADDR[47]<<16)|(pADDR[48]<<8)|pADDR[49];
		i+=s;
		pADDR[46]=i>>24;
		pADDR[47]=i>>16;
		pADDR[48]=i>>8;
		pADDR[49]=i;
	}
}


void EVENT_CALC_3003(void)//�����־���¼�(��׼�¼�)
{
	u32 i;
	u32 f;
	u32 us;
	u32 is;
	u32 delay;
	u8* p8;
	u32 lastOccur;
	
	
	if(Meter_Ram->ALP_S_Timer&1)
	{
		return;//ÿ2��1�Σ���С���д���
	}
	i=MRR(ADDR_3003+1,1);//��Ч��ʶ
	if(i==0)
	{//��Ч
		Meter_Ram->ALP_S_Timer=59;//�������´β���������
		Meter_Ram->LPFlags=0;
		return;
	}
	
	f=0;//������־
//#define ADDR_3003_5     ADDR_3003+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  ������������  double-long����λ��A�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
	//���������
	MR(ADDR_DATABUFF,ADDR_3003_5,LENmax_3003_5);
	p8=(u8*)ADDR_DATABUFF;
	us=(p8[3]<<8)|p8[4];
	is=(p8[6]<<24)|(p8[7]<<16)|(p8[8]<<8)|p8[9];
	us=hex_bcd(us);
	is=hex_bcd(is);
	delay=p8[11];
	if(delay==0x0)
	{
		delay=60;
	}
//A
	i=MRR(ADDR_UA+2,2);
	if(i<=us)
	{
		i=MRR(ADDR_IA,4);
		i&=0x7fffffff;
		i>>=4;
		if(i<=is)
		{
			f|=1;//A��ǰ����
		}
	}
//B
	if(Meter_Ram->PW==0)//���߷�ʽ;0=��������,1=��������
	{
		i=MRR(ADDR_UB+2,2);
		if(i<=us)
		{
			i=MRR(ADDR_IB,4);
			i&=0x7fffffff;
			i>>=4;
			if(i<=is)
			{
				f|=2;//B��ǰ����
			}
		}
	}
//C
	i=MRR(ADDR_UC+2,2);
	if(i<=us)
	{
		i=MRR(ADDR_IC,4);
		i&=0x7fffffff;
		i>>=4;
		if(i<=is)
		{
			f|=4;//C��ǰ����
		}
	}

//A�ж�
	i=f&0x01;
	if(i!=(Meter_Ram->LPFlags&0x01))
	{//A����仯
		Meter_Ram->LPFlags&=0xfe;
		Meter_Ram->LPFlags|=i;
		Meter_Ram->ALP_S_Timer=delay;
	}
	else
	{
		if(Meter_Ram->ALP_S_Timer==0x0)
		{
			Meter_Ram->LPFlags&=0xef;
			Meter_Ram->LPFlags|=(i<<4);
			Meter_Ram->ALP_S_Timer=delay;//ÿx�����н���
			lastOccur=GET_LastEventOccurRecovery(0x30030700,0);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
			if(i)
			{//����
				if(lastOccur==0)
				{
				#if (USER/100)!=15//����
					Event_OAD(0x30030600,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				#endif	
					Event_OAD(0x30030700,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					EventCurrentvalue_NUM((u8*)ADDR_EventCurrentvalue_3003,1<<0);//�¼���ǰֵ��¼��������+1�����:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��)
				}
			}
			else
			{//�ָ�
				if(lastOccur)
				{
				#if (USER/100)!=15//����
					Event_OAD(0x30030600,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				#endif	
					Event_OAD(0x30030700,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
			
		}
	}
//B�ж�
	i=f&0x02;
	if(i!=(Meter_Ram->LPFlags&0x02))
	{//B����仯
		Meter_Ram->LPFlags&=0xfd;
		Meter_Ram->LPFlags|=i;
		Meter_Ram->BLP_S_Timer=delay;
	}
	else
	{
		if(Meter_Ram->BLP_S_Timer==0x0)
		{
			Meter_Ram->LPFlags&=0xdf;
			Meter_Ram->LPFlags|=(i<<4);
			Meter_Ram->BLP_S_Timer=delay;//ÿx�����н���
			lastOccur=GET_LastEventOccurRecovery(0x30030800,0);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
			if(i)
			{//����
				if(lastOccur==0)
				{
				#if (USER/100)!=15//����
					Event_OAD(0x30030600,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				#endif	
					Event_OAD(0x30030800,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					EventCurrentvalue_NUM((u8*)ADDR_EventCurrentvalue_3003,1<<1);//�¼���ǰֵ��¼��������+1�����:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��)
				}
			}
			else
			{//�ָ�
				if(lastOccur)
				{
				#if (USER/100)!=15//����
					Event_OAD(0x30030600,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				#endif
					Event_OAD(0x30030800,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
			
		}
	}
//C�ж�
	i=f&0x04;
	if(i!=(Meter_Ram->LPFlags&0x04))
	{//C����仯
		Meter_Ram->LPFlags&=0xfb;
		Meter_Ram->LPFlags|=i;
		Meter_Ram->CLP_S_Timer=delay;
	}
	else
	{
		if(Meter_Ram->CLP_S_Timer==0x0)
		{
			Meter_Ram->LPFlags&=0xbf;
			Meter_Ram->LPFlags|=(i<<4);
			Meter_Ram->CLP_S_Timer=delay;//ÿx�����н���
			lastOccur=GET_LastEventOccurRecovery(0x30030900,0);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
			if(i)
			{//����
				if(lastOccur==0)
				{
				#if (USER/100)!=15//����
					Event_OAD(0x30030600,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				#endif
					Event_OAD(0x30030900,0,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					EventCurrentvalue_NUM((u8*)ADDR_EventCurrentvalue_3003,1<<2);//�¼���ǰֵ��¼��������+1�����:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��)
				}
			}
			else
			{//�ָ�
				if(lastOccur)
				{
				#if (USER/100)!=15//����
					Event_OAD(0x30030600,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				#endif
					Event_OAD(0x30030900,0,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
		}
	}
	
}































