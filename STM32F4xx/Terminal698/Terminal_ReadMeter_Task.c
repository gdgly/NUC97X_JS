
//��������
#include "AllHead.h"
#include "Terminal_ReadMeter_Task.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"



void ClrRMTaskCompletePn(u32 Pn)//��ָ��Pn�ĳ���������ɱ�־;���:Pn
{
	u32 i;
	for(i=0;i<MaxRMTask;i++)
	{
		AllRMList->Pn[Pn].RMCompleteS[i]=0;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
	}
}

void ClrRMTaskCompletes(u32 PORTn,u32 RMTask)//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����
{
	u32 i;
	u32 Pn;
	u32 RS485PORT;
#if IC_SST39VF6401B==0//0=û,1=��
	u32 Addr;
#else
	u8 *p8;
#endif

	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=2;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=3;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=4;
			break;
		case RS485_4PORT://RS485-4(�ز�)
			RS485PORT=31;
			break;
		default:
			return;
	}
#if IC_SST39VF6401B==0//0=û,1=��
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//ͨ�����ʼ��˿ں�
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//ͨ�Ŷ˿ں���ͬ
			Pn=MRR(Addr,2);//����������� BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//�����������!=0x0
				AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8[2];//ͨ�����ʼ��˿ں�
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//ͨ�Ŷ˿ں���ͬ
			Pn=p8[0]+(p8[1]<<8);//����������� BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//�����������!=0x0
				AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
#endif
}



