
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/EVENT_CALC.h"
#include "../DL698/RM_Record.h"
#include "../DL698/RM_TASK.h"
#include "../Device/MEMRW.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../Calculate/Calculate.h"




u8* Get_RMOADDATA(u32 OAD,u8* pDATA)//�õ�����������ָ����OAD����ָ��,���:pDATA=��������;����:0=û�ҵ�
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 n;
	u32 OADcount;
	u8* pData;
	
	
	switch(OAD)
	{
		case 0x60400200://�ɼ�����ʱ��
			return pDATA;
		case 0x60410200://�ɼ��ɹ�ʱ��
			return pDATA+7;
		case 0x60420200://�ɼ��洢ʱ��
			return pDATA+14;
	}
	//�����¼���ݿ�ʼָ��
	Offset=21;
	n=pDATA[Offset+1];
	Offset+=2;
	for(i=0;i<n;i++)
	{
		if(pDATA[Offset+1]==0)
		{//OAD
			Offset+=6;
		}
		else
		{//ROAD
			x=pDATA[Offset+6];//����OAD����
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=pDATA+Offset;
//���ܱ����ݿ���,���Ȱ�ʵ��д����
//0 7byte �ɼ�����ʱ��  date_time_s(����������)
//7 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//14 7byte �ɼ��洢ʱ��  date_time_s(����������)
//21  �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������)
	//����ͬOAD�ڼ�¼�е�OAD����
	Offset=21+2;
	OADcount=0;//��ͬOAD�ڼ�¼�е�OAD����
	for(i=0;i<n;i++)
	{
		x=(pDATA[Offset+2]<<24)+(pDATA[Offset+3]<<16)+(pDATA[Offset+4]<<8)+pDATA[Offset+5];
		if(OAD==x)
		{
			break;
		}
		if(pDATA[Offset+1]==0)
		{//OAD
			OADcount++;
			Offset+=6;
		}
		else
		{//ROAD
			x=pDATA[Offset+5];//����OAD����
			OADcount+=x;
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	if(i>=n)
	{//û�ҵ�
		return 0;
	}
	Len=0;//����ƫ��
	while(OADcount--)
	{
		Len+=Get_DL698DataLen_S(pData+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	}
	return pData+Len;
}









u32 EVENT_CALC_3105(u32 PORTn,u8* pDATA)//����������ص��¼�-���ܱ�ʱ�ӳ����¼�;����:0=���¼�������id,1=���¼�������id
{
	u32 i;
	u32 x;
	u32 lastOccur;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	x=UARTCtrl->TaskID;
	x>>=8;
	x&=0xff;//�����
	i=MRR(ADDR_3105_6+3,4);
	if(((i>>24)&0xff)!=x)
	{
		return 0;//�����ɼ�����Ų�ͬ
	}
	i&=0xffff;//�쳣�б���ֵ  long-unsigned����λ���룩
	i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���)
#if ((USER%100)!=0 &&(USER/100)!=15)//���ǵ��Ժ̨�����
	x=(Comm_Ram->DL698YMDHMS[4]<<8)|Comm_Ram->DL698YMDHMS[5];//��ʱ��ʱ��
	if((x>=((23<<8)|55))||(x<5))
	{//�ն�ʱ��ʱ��>=23��55�ֻ�<0��5��,���ж�ʱ�ӳ����¼�
		return 1;//����:0=���¼�������id,1=���¼�������id
	}
#endif
	pDATA=Get_RMOADDATA(0x40000200,pDATA);//�õ�����������ָ����OAD��ROAD����ָ��,���:pOAD����������,pDATA=��������;����:0=û�ҵ�
	if(pDATA)
	{
		if(pDATA[0]==DataType_date_time_s)
		{
			x=YYMDHMS_Sub_hex(pDATA+1,Comm_Ram->DL698YMDHMS);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
			if(x>>31)
			{
				x=~x;
				x++;
			}
			UARTCtrl->TSA[0]=DataType_TSA;
			UARTCtrl->TSA[1]=7;
			UARTCtrl->TSA[2]=5;
			lastOccur=GET_LastEventOccurRecovery(0x31050200,UARTCtrl->TSA);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
			if(x>=i)
			{//����
				if(lastOccur==0)
				{
					Event_31050200(UARTCtrl->TSA,1,pDATA);//�ն�ͣ/�ϵ��¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
			else
			{//�ָ�
				if(lastOccur)
				{
					Event_31050200(UARTCtrl->TSA,0,pDATA);//�ն�ͣ/�ϵ��¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
		}
	}
	return 1;//����:0=���¼�������id,1=���¼�������id
}


u32 EVENT_CALC_310B_310C_310D_310E(u32 PORTn,u8* pDATA)//����������ص��¼�-���ܱ�ʾ���½��¼�,�����������¼�,���ܱ�����¼�,���ܱ�ͣ���¼�
{
	u32 i;
	u32 id;
	u32 eventid;//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
	u32 threshold_310C;//������ֵ  double-long-unsigned����λ��%���޻��㣩
	u32 threshold_310D;//������ֵ  double-long-unsigned����λ��%���޻��㣩
	u32 ti;//ͣ����ֵ TI
	u8* pKWHc;//��ǰ�������ʾֵ
	u8* pKWHs;//�ϴγ������ʾֵ
	u8* pTIMEs;//�ϴγ���ɹ�ʱ��
	u32 t;//2�γ���ʱ�����
	u64 d64a;
	u64 d64b;
	u32 lastOccur;
	EventSour_typedef* EventSour;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 x;
//	u8* pKWc;//��ǰ������ double-long����λ��W�����㣺-1
//	double f64;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	id>>=8;
	id&=0xff;//�����
#if (LENmax_310B_6+4+LENmax_EventOAD+LENmax_310C_6+4+LENmax_EventOAD+LENmax_310D_6+4+LENmax_EventOAD+LENmax_310E_6)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_310B_6,LENmax_310B_6+4+LENmax_EventOAD+LENmax_310C_6+4+LENmax_EventOAD+LENmax_310D_6+4+LENmax_EventOAD+LENmax_310E_6);
	eventid=0;//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
	pKWHs=(u8*)ADDR_DATABUFF;
	//���ܱ�ʾ���½��¼�����
	i=pKWHs[3];
	if(i==id)
	{
		eventid|=0x01;//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
	}
	//�����������¼�����
	pKWHs+=LENmax_310B_6+4+LENmax_EventOAD;
	threshold_310C=(pKWHs[3]<<24)|(pKWHs[4]<<16)|(pKWHs[5]<<8)|(pKWHs[6]<<0);//������ֵ  double-long-unsigned����λ��%���޻��㣩
	i=pKWHs[8];
	if(i==id)
	{
		if(PORTn!=RS485_4PORT)
		{//�����ز���,�ز��ڲ��������¼���ɼ����ʱ�䲻׼������
			eventid|=0x02;//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
		}
	}
	//���ܱ�����¼�����
	pKWHs+=LENmax_310C_6+4+LENmax_EventOAD;
	threshold_310D=(pKWHs[3]<<24)|(pKWHs[4]<<16)|(pKWHs[5]<<8)|(pKWHs[6]<<0);//������ֵ  double-long-unsigned����λ��%���޻��㣩
	i=pKWHs[8];
	if(i==id)
	{
		if(PORTn!=RS485_4PORT)
		{//�����ز���,�ز��ڲ��������¼���ɼ����ʱ�䲻׼������
			eventid|=0x04;//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
		}
	}
	//���ܱ�ͣ���¼�����
	pKWHs+=LENmax_310D_6+4+LENmax_EventOAD;
	ti=pTItoSECOND(pKWHs+3);//ͣ����ֵ TI//TIתΪ��ֵ,���:pTI����������;����:hex��
	i=pKWHs[7];
	if(i==id)
	{
		eventid|=0x08;//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
	}
	if(eventid==0)
	{
		return 0;//�����ɼ�����Ų�ͬ
	}
	
	if(eventid&8)//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
	{
		i=AddrToMeterSetNo((u16*)&UARTCtrl->RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
		if(i>=NUM_RMmax)
		{
			EventSour=0;//��Чָ��
		}
		else
		{
			EventSour=(EventSour_typedef*)(ADDR_Event_Sour+(i*sizeof(EventSour_typedef)));
			for(i=0;i<3;i++)
			{
				if(EventSour->RMAddr[i]!=UARTCtrl->RMAddr[i])
				{//��ַ��ͬ
					for(i=0;i<3;i++)
					{
						EventSour->RMAddr[i]=UARTCtrl->RMAddr[i];
					}
					EventSour->TimeCount=0;//���ܱ�ͣ��ʱ���ʱ(��)
					break;
				}
			}
		}
	}
	
	i=Get_LastRecordFileData(id,(u16*)&UARTCtrl->RMAddr);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF���ݳ��ȣ�0=û�ҵ�
	if(i==0)
	{
		return 1;//û�ҵ�ԭ��¼
	}
//�¼�����ʹ��Լ��
//ADDR_DATABUFF�¼��༭���
//ADDR_128KDATABUFF  32KBYTE ������DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE �¼�����,�����������Ա��
//ADDR_128KDATABUFF+64K 32KBYTE �����¼�����ʱ����
//ADDR_128KDATABUFF+96K 32KBYTE �¼��и������ݵ���ӦOAD˳���array OAD
	if(i>(32*1024))
	{
		return 1;//������DL698_DataDef���ȳ���
	}
	pKWHc=Get_RMOADDATA(0x00100200,pDATA);//�õ�����������ָ����OAD��ROAD����ָ��,���:pOAD����������,pDATA=��������;����:0=û�ҵ�
	if(pKWHc)
	{
		pKWHs=Get_RMOADDATA(0x00100200,(u8*)ADDR_128KDATABUFF);//�õ�����������ָ����OAD��ROAD����ָ��,���:pOAD����������,pDATA=��������;����:0=û�ҵ�
		if(pKWHs)
		{
			//����Դ
			UARTCtrl->TSA[0]=DataType_TSA;
			UARTCtrl->TSA[1]=7;
			UARTCtrl->TSA[2]=5;
			//00102201�¼�����ǰ�����й��ܵ���
			//00106201�¼�����ǰ�����й��ܵ���
			MR(ADDR_128KDATABUFF+(64*1024),(u32)pKWHs,2+((TMaxTariff+1)*9));
			//00104201�¼������������й��ܵ���
			//00108201�¼������������й��ܵ���
			MR(ADDR_128KDATABUFF+(64*1024)+1024,(u32)pKWHc,2+((TMaxTariff+1)*9));
			//00202201�¼�����ǰ�����й��ܵ���
			//00206201�¼�����ǰ�����й��ܵ���
			pKWHs=(u8*)ADDR_128KDATABUFF+(64*1024)+128;
			pKWHs[0]=0;
			pKWHs=Get_RMOADDATA(0x00200200,(u8*)ADDR_128KDATABUFF);//�õ�����������ָ����OAD��ROAD����ָ��,���:pOAD����������,pDATA=��������;����:0=û�ҵ�
			if(pKWHs)
			{
				MR(ADDR_128KDATABUFF+(64*1024)+128,(u32)pKWHs,2+((TMaxTariff+1)*9));
			}
			//00204201�¼����������й��ܵ���
			//00208201�¼����������й��ܵ���
			pKWHs=(u8*)ADDR_128KDATABUFF+(64*1024)+1024+128;
			pKWHs[0]=0;
			pKWHs=Get_RMOADDATA(0x00200200,pDATA);//�õ�����������ָ����OAD��ROAD����ָ��,���:pOAD����������,pDATA=��������;����:0=û�ҵ�
			if(pKWHs)
			{
				MR(ADDR_128KDATABUFF+(64*1024)+1024+128,(u32)pKWHs,2+((TMaxTariff+1)*9));
			}
			
			pKWHs=(u8*)ADDR_128KDATABUFF+(64*1024);//ADDR_128KDATABUFF  32KBYTE ������DL698_DataDef ���ܱ仯
			t=0;//2�γ���ʱ�����
			pTIMEs=Get_RMOADDATA(0x60410200,(u8*)ADDR_128KDATABUFF);//�ϴγ���ɹ�ʱ��
			if(pTIMEs)
			{
				t=YYMDHMS_Sub_hex(pTIMEs,(u8*)ADDR_DL698YMDHMS);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
				if(t>>31)
				{//��
					t=0;
				}
			}
			
			MC(0,ADDR_DATABUFF,16);
			if(pKWHc[2]!=DataType_double_long_unsigned)
			{
				if(pKWHc[2]!=DataType_long64_unsigned)
				{
					return 1;
				}
				MR(ADDR_DATABUFF,(u32)pKWHc+3,8);
			}
			else
			{
				MR(ADDR_DATABUFF+4,(u32)pKWHc+3,4);
			}
			if(pKWHs[2]!=DataType_double_long_unsigned)
			{
				if(pKWHs[2]!=DataType_long64_unsigned)
				{
					return 1;
				}
				MR(ADDR_DATABUFF+8,(u32)pKWHs+3,8);
			}
			else
			{
				MR(ADDR_DATABUFF+12,(u32)pKWHs+3,4);
			}
			d64a=MRR(ADDR_DATABUFF,8);//��ǰʾֵ
			d64b=MRR(ADDR_DATABUFF+8,8);//�ϴ�ʾֵ
			d64a=R_Inverse(d64a,8);
			d64b=R_Inverse(d64b,8);
		//���ܱ�ʾ���½��¼�
			if(eventid&1)//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
			{
				if(d64a<d64b)
				{
					Event_OAD(0x310B0200,UARTCtrl->TSA,2,1);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					return 1;
				}
			}
			
			if(t==0)
			{
				return 1;//ʱ���=0
			}
			d64a-=d64b;//���ܲ�
		//���ܱ�ͣ���¼�
			if(eventid&8)//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
			{
				if(EventSour)//��Чָ��
				{
					lastOccur=GET_LastEventOccurRecovery(0x310E0200,UARTCtrl->TSA);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
					if(d64a==0)//���ܲ�
					{//ͣ��
						t+=EventSour->TimeCount;//���ܱ�ͣ��ʱ���ʱ(��)
						EventSour->TimeCount=t;//���ܱ�ͣ��ʱ���ʱ(��)
						if(ti)
						{//ͣ����ֵ����0
							if(t>=ti)
							{//����ͣ����ֵ TI
								if(lastOccur==0)
								{
									Event_OAD(0x310E0200,UARTCtrl->TSA,1,1);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
								}
							}
						}
						return 1;
					}
					else
					{//ûͣ��
						EventSour->TimeCount=0;//���ܱ�ͣ��ʱ���ʱ(��)
						if(lastOccur)
						{
							Event_OAD(0x310E0200,UARTCtrl->TSA,0,0);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						}
					}
				}
			}

			if((PORTn!=RS485_1PORT)&&(PORTn!=RS485_2PORT))
			{
				return 1;//����485��
			}
/*
			pKWc=Get_RMOADDATA(0x20040200,pDATA);//�õ�����������ָ����OAD��ROAD����ָ��,���:pOAD����������,pDATA=��������;����:0=û�ҵ�
			if(pKWc==0)
			{
				return 1;
			}
			i=(pKWc[2]<<8)|pKWc[3];
			if(i&0x8000)
			{//����Ϊ��,����
				return 1;
			}
			f64=i;
			f64*=t;
			f64/=3600;//kws��Ϊkwh,4λС��
			x=(u32)f64;//t=Ӧ�ߵ���kwh,4λС��
			if(x<=100)
			{//Ӧ�ߵ���kwh<=0.0100ʱ���Ƚ�
				return 1;
			}
			i=d64a;
			i*=100;
			i/=x;
*/
			i=d64a;//���ܲ�
			i*=10;
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{//���ߡ������¼�ֻ��ͨ485��
			//���ܱ�����¼�
				if(eventid&4)//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
				{
					lastOccur=GET_LastEventOccurRecovery(0x310D0200,UARTCtrl->TSA);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
					if(i>=threshold_310D)//������ֵ  double-long-unsigned����λ��%���޻��㣩
					{
						if(lastOccur==0)
						{
							Event_OAD(0x310D0200,UARTCtrl->TSA,1,1);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						}
						return 1;
					}
					else
					{
						if(lastOccur)
						{
							Event_OAD(0x310D0200,UARTCtrl->TSA,0,0);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						}
					}
				}
			//�����������¼�
				if(eventid&2)//�¼�ID:b0=ʾ���½�,b1=����,b2=����,b3=ͣ��
				{
					lastOccur=GET_LastEventOccurRecovery(0x310C0200,UARTCtrl->TSA);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
					if(i>=threshold_310C)//������ֵ  double-long-unsigned����λ��%���޻��㣩
					{
						if(lastOccur==0)
						{
							Event_OAD(0x310C0200,UARTCtrl->TSA,1,1);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						}
						return 1;
					}
					else
					{
						if(lastOccur)
						{
							Event_OAD(0x310C0200,UARTCtrl->TSA,0,0);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						}
					}
				}
			}
			
		}
	}
	return 1;//����:0=���¼�������id,1=���¼�������id
}





u32 EVENT_CALC_311C(u32 PORTn,u8* pRecord)//����������ص��¼�-���ܱ����ݱ����ؼ�¼;����:0=���¼�������id,1=���¼�������id
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 m;
	u8* pRecord1;
	u32 LEN_CSD;
	u32 CSDoffset;
	u32 DATAoffset;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	n=UARTCtrl->TaskID;
	n>>=8;
	n&=0xff;//�����
	i=MRR(ADDR_311C_6+3,1);
	if(i!=n)
	{//�����ɼ�����Ų�ͬ
		return 0;//����:0=���¼�������id,1=���¼�������id
	}
	i=Get_LastRecordFileData(n,(u16*)UARTCtrl->RMAddr);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
	if(i==0)
	{
		return 1;//����:0=���¼�������id,1=���¼�������id
	}
	pRecord1=(u8*)ADDR_128KDATABUFF;
	pRecord+=21;
	pRecord1+=21;
	//�����¼����CSD����
	n=pRecord[1];//CSD����
	LEN_CSD=2;
	for(i=0;i<n;i++)
	{
		if(pRecord[LEN_CSD+1]==0)
		{//OAD
			LEN_CSD+=6;
		}
		else
		{//ROAD
			x=pRecord[LEN_CSD+6];//����OAD����
			x*=4;
			x+=7;
			LEN_CSD+=x;
		}
	}
	n=pRecord[1];//CSD����
	x=pRecord1[1];//CSD����
	if(x!=n)
	{
		return 1;//����:0=���¼�������id,1=���¼�������id
	}
	CSDoffset=2;
	DATAoffset=0;
	for(m=0;m<n;m++)
	{
		//�Ƚ�CSD�Ƿ���ͬ
		if(pRecord[CSDoffset+1]==0)
		{//OAD
			x=6;
		}
		else
		{//ROAD
			x=pRecord[CSDoffset+6];//����OAD����
			x*=4;
			x+=7;
		}
		for(i=0;i<x;i++)
		{
			if(pRecord[CSDoffset+i]!=pRecord1[CSDoffset+i])
			{//CSD��ͬ���Ƚ�
				return 1;
			}
		}
		//�Ƚ�DATA�Ƿ���ͬ
		y=Get_DL698DataLen_S(pRecord+LEN_CSD+DATAoffset,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		for(i=0;i<y;i++)
		{
			if(pRecord[LEN_CSD+DATAoffset+i]!=pRecord1[LEN_CSD+DATAoffset+i])
			{//DATA��ͬ
				Event_311C0200(PORTn,pRecord+CSDoffset,pRecord1+LEN_CSD+DATAoffset,pRecord+LEN_CSD+DATAoffset);//���ܱ����ݱ����ؼ�¼,���:pCSD=��ص�CSD,pDATA1=���ǰ����,pDATA2=���������
				break;
			}
		}
		CSDoffset+=x;
		DATAoffset+=y;
	}
	return 1;//����:0=���¼�������id,1=���¼�������id
}









u32 Event_RMDATA(u32 PORTn,u8* pDATA)//����������ص��¼�;���:pDATA=��������,����:0=���¼�������id,1=���¼�������id
{
	u32 EventID;
	
	EventID=0;
//3105 ���ܱ�ʱ�ӳ����¼�
	EventID|=EVENT_CALC_3105(PORTn,pDATA);//����������ص��¼�-���ܱ�ʱ�ӳ����¼�;����:0=���¼�������id,1=���¼�������id
//310B,310C,310D,310E �����������¼�,���ܱ�����¼�,���ܱ�ͣ���¼�
	EventID|=EVENT_CALC_310B_310C_310D_310E(PORTn,pDATA);//����������ص��¼�-���ܱ�ʾ���½��¼�,�����������¼�,���ܱ�����¼�,���ܱ�ͣ���¼�;����:0=���¼�������id,1=���¼�������id
//311C
	EventID|=EVENT_CALC_311C(PORTn,pDATA);//����������ص��¼�-���ܱ����ݱ����ؼ�¼;����:0=���¼�������id,1=���¼�������id
	
	
	
	
	
	return EventID;
}













