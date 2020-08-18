
//�ն˳���
#include "Project.h"
#include "Terminal_ReadMeter_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_ReadMeter_ACSample.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"

#include "AutoSearchMeter_RS485.h"
#include "BatchTask_RS485.h"
#include "../Display/Warning.h"
#include "KeyUser.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_ReadMeter_Event.h"

#include "Terminal_ReadMeter_WSH41.h"
#include "Terminal_ReadMeter_ShH.h"



void ReadMeter_Protocol(u32 PORTn)//�����Լ
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
#if (Extern_Terminal_ReadMeter_DL645_1997!=0)||(Extern_Terminal_ReadMeter_ShH!=0)//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳���DL645-1997���ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳��� �Ϻ����Լ
	u32 *p32;
	p32=p32;
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������
	if(i==2)
	{//��������װ��(2)
		if(PORTn==ACSAMPLEPORT)
		{//��ǰ�����ڲ�������
			Terminal_ReadMeter_ACSample(PORTn);//�ն˳���,��������װ��
		}
		else
		{
			UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
		}
		return;
	}
	if(i==1)
	{//DL/T645-1997(1)
	#if Extern_Terminal_ReadMeter_DL645_1997==0//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳���DL645-1997
		Terminal_ReadMeter_DL645_1997(PORTn);//�ն˳���DL645-1997
	#else
		if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[101])(PORTn);//�ն˳���DL645-1997
		}
	#endif	
		return;
	}
	if(i==30)
	{//DL/T645-2007(30)
		Terminal_ReadMeter_DL645_2007(PORTn);//�ն˳���DL645-2007
		return;
	}

#if (USER/100)==10//�ӱ�ר��
	if(i==7)
	{//��ʤ��Լ(�ӱ�)
		Terminal_ReadMeter_WSH41(PORTn);//�ն˳���RS485��_��ʤ��Լ4.1�汾
		return;
	}
#endif

#if (USER/100)==5//�Ϻ�ר��
	if(i==21)
	{//�Ϻ���Լ
	#if Extern_Terminal_ReadMeter_ShH==0//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳��� �Ϻ����Լ
		Terminal_ReadMeter_ShH(PORTn);//�ն˳���RS485��_�Ϻ���Լ
	#else
		if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[112])(PORTn);//�ն˳���_�Ϻ���Լ
		}
	#endif
		return;
	}
#endif

	//�޶�Ӧ��Լ
	UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
}


void Terminal_AFN0CF11_RS485(u32 PORTn,u32 RMTask)//�ն���1����F11,�ն˼��г���״̬��Ϣ;���Ҫ����ĳ��������
{
	u32 i;
	u32 x;
	u32 Pn;
	u32 RS485PORT;
	u8 *p8;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 MeterNum;
#if IC_SST39VF6401B==0//0=û,1=��
	u32 Addr;
#else
	u8 *p8d;
#endif
#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	u32 y;
	u32 KeyUser;
	u32 n;
	#if IC_SST39VF6401B==1//0=û,1=��
	u8 *p8s;
	#endif
#endif


	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
	i=MRR(ADDR_TYMDHMS+1,2);
	#if (((Project/100)==2) && ((USER/100)==5))//�Ϻ�������
	p8=(u8*)(ADDR_AFN04F242);//������ʱ������
	n=p8[0];
	if(n>=60)//������ʱ��������1-60����
	{
		n=0x100;//1:00
	}
	else
	{
		n=hex_bcd(n);
	}
	if(i<n)
	{
		return;
	}
	#else
	if(i<RMM_StartHM)
	{
		return;
	}
	#endif
#endif
#if (USER/100)==5//�Ϻ�
	i=MRR(ADDR_TYMDHMS+1,2);
#if ((Project/100)==2)//�Ϻ�������
	p8=(u8*)(ADDR_AFN04F242);//������ʱ������
	n=p8[0];
	if(n>=60)//������ʱ��������1-60����
	{
		n=0x100;//1:00
	}
	else
	{
		n=hex_bcd(n);
	}
	if(i<n)
	{
		return;
	}
#endif
	if(i<RMM_StartHM)
	{
		return;
	}
#endif
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=2;
			break;
		case RS485_4PORT://RS485-4
			RS485PORT=3;
			break;
		default:
			return;
	}
	p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	KeyUser=MRR(ADDR_AFN04F35,1);
	if(KeyUser>MaxKeyUser)
	{
		KeyUser=MaxKeyUser;
	}
	p8[0]=3;//�������ݿ����n	BIN	1(1-31)
#else
	p8[0]=2;//�������ݿ����n	BIN	1(1-31)
#endif
	p8+=(19*(RS485PORT-1));
	if(PORTn!=RS485_4PORT)
	{//485��
		RS485PORT++;
		p8[1]=RS485PORT;//�ն�ͨ�Ŷ˿ں�	BIN	1
	}
	else
	{//�ز���
		p8[1]=31;//�ն�ͨ�Ŷ˿ں�	BIN	1
		RS485PORT=31;
	}
	MeterNum=0;//Ҫ���������=0
	x=0;//����ɹ���
#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	y=0;//���ص㻧�ɹ���
#endif
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
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]!=3)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				{
					MeterNum++;//Ҫ���������
				}
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==2)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				{
					x++;//����ɹ���
				#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					for(n=0;n<KeyUser;n++)
					{
						if((i+1)==MRR(ADDR_AFN04F35+1+(2*n),2))//�ص㻧�ĵ��ܱ�/��������װ�����
						{
							y++;//���ص㻧�ɹ���
							break;
						}
					}
				#endif
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8d=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8d[2];//ͨ�����ʼ��˿ں�
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//ͨ�Ŷ˿ں���ͬ
			Pn=p8d[0]+(p8d[1]<<8);//����������� BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//�����������!=0x0
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]!=3)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				{
					MeterNum++;//Ҫ���������
				}
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==2)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				{
					x++;//����ɹ���
				#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					p8s=(u8*)(ADDR_AFN04F35+1);
					for(n=0;n<KeyUser;n++)
					{
						if((i+1)==(p8s[0]+(p8s[1]<<8)))//�ص㻧�ĵ��ܱ�/��������װ�����
						{
							y++;//���ص㻧�ɹ���
							break;
						}
						p8s+=2;
					}
				#endif
				}
			}
		}
		p8d+=LEN_AFN04F10_Pn;
	}
#endif
	MWR(MeterNum,((u32)p8)+2,2);//Ҫ���������	BIN	2
	if(MeterNum==x)
	{
		p8[4]|=2;//D0=1���ڳ���,D1=1ʱ�������
		if(p8[14+3]==0xee)
		{//����ʱ��δ��
			MR(((u32)p8)+14,ADDR_AFN0CF2,6);
		}
	}
	else
	{
		p8[4]&=0xfd;//D0=1���ڳ���,D1=0ʱ����δ���
		MC(0xee,((u32)p8)+14,6);//�������ʱ��	����¼A.1	6
	}
	if(UARTCtrl->RMFlag!=0)//0=��������ͣ,1=���������
	{//ע:������ͣʱ��0����ɱ�־
		MWR(x,((u32)p8)+5,2);//����ɹ�����	BIN	2
	#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		MWR(y,((u32)p8)+7,1);//���ص��ɹ�����	BIN	1
	#else
		MWR(0,((u32)p8)+7,1);//���ص��ɹ�����	BIN	1
	#endif
	}
	//����ʼʱ��	����¼A.1	6
	if(p8[8+3]==0xee)
	{//�޿�ʼʱ��
		MR(((u32)p8)+8,ADDR_AFN0CF2,6);
	}
	//�������ʱ��	����¼A.1	6
}

u32 NoRMCompletes_RS485(u32 PORTn,u32 RMTask)//RS485δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
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
		default:
			return 0;
	}
	x=0;//δ����
	y=0;//ʧ����
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
				z=AllRMList->Pn[Pn].RMCompleteS[RMTask];
				if(z==0)
				{
					x++;//δ����
				}
				else
				{
					if(z==1)
					{
						y++;//ʧ����
					}
				}
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
				z=AllRMList->Pn[Pn].RMCompleteS[RMTask];
				if(z==0)
				{
					x++;//δ����
				}
				else
				{
					if(z==1)
					{
						y++;//ʧ����
					}
				}
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
#endif
	return (x+(y<<16));
}
/*
void ClrRMTaskCompletePn_RS485(u32 Pn)//��ָ��Pn�ĳ���������ɱ�־;���:Pn
{
	u32 i;
	for(i=0;i<MaxRMTask;i++)
	{
		AllRMList->Pn[Pn].RMCompleteS[i]=0;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
	}
}
*/
void ClrRMTaskCompletes_RS485(u32 PORTn,u32 RMTask,u32 All)//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
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
				if(All==0)
				{//ֻ��ʧ��
					if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==1)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
					{
						AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
					}
				}
				else
				{//��ȫ��
					AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				}
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
				if(All==0)
				{//ֻ��ʧ��
					if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==1)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
					{
						AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
					}
				}
				else
				{//��ȫ��
					AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				}
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
#endif
}

u32 NewTask_RS485(u32 PORTn)//RS485��ִ��������;���أ�0=ûִ��������1=ִ��������
{
  UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(UARTCtrl->Lock==0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	{
		return 0;
	}
	if(((UARTCtrl->RMMeterEvent&1)!=0)||(UARTCtrl->SubsequentFrames!=0))
	{//���ڳ������ܱ������ϱ��¼��������֡
		return 0;
	}
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
	if(UARTCtrl->DateReRMCount!=0)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
	{
		if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
		{//ֻ��ѭ��ʱ�򳭶����ܱ��¼�ʱ���ܲ�������ȼ�
			NewRMEvene(PORTn);//�»�ָ������ܱ��¼�����;����0=û�½���,1=�½���
		}
	}
	#else
	if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
	{//ֻ��ѭ��ʱ�򳭶����ܱ��¼�ʱ���ܲ�������ȼ�
		NewRMEvene(PORTn);//�»�ָ������ܱ��¼�����;����0=û�½���,1=�½���
	}
	#endif
#endif
	if(UARTCtrl->RMTaskC>=(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
	{//ֻ��ѭ��ʱ���ܲ�������ȼ�
		if(UARTCtrl->RMTaskN&2)//��ÿ������
		{
			UARTCtrl->RMTaskN&=0xfd;//ȥ��ÿ������
			UARTCtrl->RMTaskC=1;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
			ClrRMTaskCompletes_RS485(PORTn,1,0);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
			if(UARTCtrl->DateReRMCount!=0xff)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
			{
				UARTCtrl->DateReRMCount++;//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
			}
		NewTask:
			UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			UARTCtrl->FnCountS=UARTCtrl->FnCount;//485����ԭѭ�������Fn����
			UARTCtrl->MeterNoS=UARTCtrl->MeterNo;//485����ԭѭ�������������ż���(���ֶ���)
			UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
			UARTCtrl->FnCount=0;//485����Fn����
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
			UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
			return 1;
		}
		if(UARTCtrl->RMTaskN&4)//��ÿ������
		{
			UARTCtrl->RMTaskN&=0xfb;//ȥ��ÿ������
			UARTCtrl->RMTaskC=2;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
			goto NewTask;
		}
		if(UARTCtrl->RMTaskN&8)//�¶�ʱ����ͣ���¼����
		{
			UARTCtrl->RMTaskN&=0xf7;//ȥ�¶�ʱ����ͣ���¼����
			UARTCtrl->RMTaskC=3;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
			goto NewTask;
		}
	}
	return 0;
}


void ReadMeter_RS485(u32 PORTn)//RS485����
{
	u32 i;
	u32 x;
	u32 y;
	u32 RS485PORT;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	u8 *p8;
	u16 *p16;
#if Extern_BatchTask_RS485!=0//�ⲿ����:0=I2,1=E1,2=E2,3=
#if IC_SST39VF6401B==1//0=û,1=��
	u32 * p32;
#endif
#endif

	if((Comm_Ram->SelfTestCommand==0x55555555)||(Comm_Ram->SelfTestCommand==0xAAAAAAAA))
	{//�����ڲ��Բ�������
		return;
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(PORTn)
	{
		case ACSAMPLEPORT://�ڲ���������
			RS485PORT=0;
			p16timer=(u16 *)&Terminal_Ram->ACSampleRM_S_Timer;
			break;
		case RS485_1PORT://RS485-1
			RS485PORT=1;
			p16timer=(u16 *)&Terminal_Ram->RS4851RM_S_Timer;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=2;
			p16timer=(u16 *)&Terminal_Ram->RS4852RM_S_Timer;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=3;
			p16timer=(u16 *)&Terminal_Ram->RS4853RM_S_Timer;
			break;
		default:
			return;
	}
	
	if(RS485PORT!=0)
	{//���ǽ���
		if(Terminal_Ram->RMDelay_S_Timer!=0x0)//10 �ϵ����ʱ�����붨ʱ��
		{
			CopyString((u8 *)"485 �ȴ��ϵ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			MWR(RS485PORT,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			return;
		}
	}

#if PowerOnRMTask==0//��Դ����������;0=û��,1=��
	if(UARTCtrl->RMTaskC==0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
	{
		UARTCtrl->RMTaskC=(MaxRMTask-1);
	}
#endif

//ת����������
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
#endif
		#if Extern_BatchTask_RS485==0//�ⲿ����:0=I2,1=E1,2=E2,3=
		BatchTask_RS485_SETRTC(PORTn);//����ת����������-RS485-ʱ�Ӷ�ʱ
		if(UARTCtrl->BatchTask>=2)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
		{
			return;
		}
		#else
		if(UARTCtrl->BatchTaskLock!=0x55)//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
		{
			i=MRR(ADDR_AFN05F30+1,1);//�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
			if(i!=1)
			{//0=�رգ�1=����������ֵ�ر�
				if(UARTCtrl->BatchTask>=2)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
				{
					Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					if(UARTCtrl->Task<=2)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						UARTCtrl->Task=0;//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
					}
				}
				else
				{
					UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
				}
			}
			else
			{//����
				if(UARTCtrl->BatchTask==0)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
				{//0=��
					x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//ȡ��������1(��ʱ����)���õ������n
					if(x==0)
					{//���õ������n=0
						UARTCtrl->BatchMeterCount=0;//ת���������������
					}
					else
					{
						if(x>MaxRS485AddCarrierPn)
						{//���õ������>MaxRS485AddCarrierPn
							x=MaxRS485AddCarrierPn;
						}
						if(UARTCtrl->BatchMeterCount<x)
						{//ת�������������<���õ������
							UARTCtrl->BatchTask=1;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
						}
					}
				}
				else
				{//1=�ȴ��ڿ�������,2...����
				#if IC_SST39VF6401B==0//0=û,1=��
					BatchTask_RS485_SETRTC(PORTn);//����ת����������-RS485-ʱ�Ӷ�ʱ
				#else
					if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
					{
		    		p32=(u32 *)(ADDR_E1_ENTRY);
		    		(*(void(*)())p32[103])(PORTn);//����ת����������-RS485-ʱ�Ӷ�ʱ
					}
				#endif
				}
			}
		}
		if(UARTCtrl->BatchTask>=2)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
		{
			return;
		}
		#endif
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	}
#endif
	if(PORTn==ACSAMPLEPORT)
	{//���ڲ���������
		//�ո���
		if(UARTCtrl->Task==0)//0=����
		{
			x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//ԭ����
			y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//��ǰ����
			if(x!=y)
			{
				UARTCtrl->RTCD[0]=y;
				UARTCtrl->RTCD[1]=y>>8;
				UARTCtrl->RMComplete&=0x7f;//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
				UARTCtrl->FnCount=0;//485����Fn����
				UARTCtrl->RMCount=0;//485�����������
				UARTCtrl->ReRMCount=0;//485���������ط�����
			}
		}
	}
	else
	{//�����ڲ���������
//�ָ���
		if(UARTCtrl->RTCM!=Comm_Ram->TYMDHMS[1])
		{
			UARTCtrl->RTCM=Comm_Ram->TYMDHMS[1];
		#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			RMEveneTimer(PORTn);//�ּ������ܱ��¼���ʱ
		#endif
			//�㲥Уʱ
			i=MRR(ADDR_AFN05F30+1,1);//�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
			if(i==1)
			{//0=�رգ�1=����������ֵ�ر�
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+1,1);//�ն˳������в�������,�˿�31
				if((i&0x08)!=0x0)
				{//Ҫ���ն˶�ʱ�Ե��㲥Уʱ
					x=MRR(ADDR_TYMDHMS+1,3);//ȡ��ǰʵʱ����ʱ��
					y=MRR((ADDR_AFN04F33+((14+(4*24))*RS485PORT))+10,3);//�Ե��㲥Уʱ��ʱʱ����ʱ��
					if((y>>16)==0x0)
					{//����=0ʱ,��ʾÿ��Уʱ
						x&=0xffff;
					}
					if(x==y)
					{
						if(UARTCtrl->BroadcastTime<3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						{
							UARTCtrl->BroadcastTime=3;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
					}
				}
			}
		#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭						
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					x=0;
					break;
				case RS485_2PORT://RS485-2
					x=1;
					break;
			}
			x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
			if(x==2)
			{
				x=Check_HMS(MRR(ADDR_AFN04F298_Ext+2,2));//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
				x<<=8;
				y=Check_HMS(MRR(ADDR_AFN04F298_Ext+4,2));//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
				y<<=8;
				if((x==0)&&(y==0))
				{//ʱ�ӺϷ�
					if(DataComp(ADDR_AFN04F298_Ext+2,ADDR_TYMDHMS+1,2)!=1)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
					{
						if(DataComp(ADDR_AFN04F298_Ext+4,ADDR_TYMDHMS+1,2)!=2)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
						{
							if(UARTCtrl->BroadcastTime<3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							{
								UARTCtrl->BroadcastTime=3;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										x=0;
										break;
									case RS485_2PORT://RS485-2
										x=1;
										break;
								}
								MWR(3,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
							}
						}
					}
				}
			}
		#endif	
		#if((USER/100)==5)//�Ϻ�
			i=MRR(ADDR_AFN05F201,1);
			if(i&(1<<(RS485PORT-1)))
			{
				if(UARTCtrl->BroadcastTime<3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				{
					UARTCtrl->BroadcastTime=3;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
			}
		#endif
		}
//ʱ����
		if(UARTCtrl->Task==0)//0=����
		{
			if(UARTCtrl->RTCH!=Comm_Ram->TYMDHMS[2])
			{
				UARTCtrl->RTCH=Comm_Ram->TYMDHMS[2];
			#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
				{//��ǰ��ѭ��
					i=NoRMCompletes_RS485(PORTn,1);//RS485δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					if(i!=0)
					{//�貹��
						UARTCtrl->RMTaskN|=2;//��ÿ������
					}
				}
			#endif
				/*
				if(UARTCtrl->RMKeyUser==0)//�����ص㻧��־,0=���泭��,1=�ص㻧����
				{
					UARTCtrl->MeterNoBackupOfKeyUser=UARTCtrl->MeterNo;//485������ܱ�/��������װ�����(���ֶ���)(ԭ���泭���װ����żĴ�)
					UARTCtrl->FnEndBackupOfKeyUser=UARTCtrl->FnEnd;//485����Fn����������־,0=����,1=û����(ԭ���泭���Fn����������־�Ĵ�)
					UARTCtrl->FnCountBackupOfKeyUser=UARTCtrl->FnCount;//485����Fn����(ԭ���泭���Fn�����Ĵ�)
					UARTCtrl->RMErrorBackupOfKeyUser=UARTCtrl->RMError;//485�������ܱ�ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��(ԭ���泭���ʧ�ܱ�־�Ĵ�)
					UARTCtrl->KeyUserRMCount=0;//�ص㻧������������
					UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
					UARTCtrl->FnCount=0;//485����Fn����
					UARTCtrl->RMCount=0;//485�����������
					UARTCtrl->ReRMCount=0;//485���������ط�����
				}
				UARTCtrl->RMKeyUser=1;//�����ص㻧��־,0=���泭��,1=�ص㻧����
				*/
			}
		}
//�ո���
		if(UARTCtrl->Task==0)//0=����
		{
			x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//ԭ����
			y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//��ǰ����
			if(x!=y)
			{
				UARTCtrl->RMEventTaskN|=0x20;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3;b5=�ո���,b6=�¸���(�Ϻ�ÿ���³��¼���)
				if(UARTCtrl->RTCD[1]!=Comm_Ram->TYMDHMS[4])
				{
					//�¸���
					UARTCtrl->RMEventTaskN|=0x40;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3;b5=�ո���,b6=�¸���(�Ϻ�ÿ���³��¼���)
				}
				UARTCtrl->RTCD[0]=y;
				UARTCtrl->RTCD[1]=y>>8;
				UARTCtrl->RMComplete&=0x7f;//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
				StoreRMEvene(PORTn);//�ݴ����ڶ����ܱ��¼�������
			#endif
				//ÿ������
				UARTCtrl->DateReRMCount=0;//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
		#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
				UARTCtrl->RMTaskN|=0x2;//��ÿ������
				ClrRMTaskCompletes_RS485(PORTn,1,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
		#endif
				//�弯�г�����Ϣ
				p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
				p8+=(19*(RS485PORT-1));
				MC(0,((u32)p8)+4,4);
				MC(0xee,((u32)p8)+8,12);
				//�Զ��ѱ�
		#if (USER/100)!=6//�û�:���Ǻ���;�����������ж�ʱ
				UARTCtrl->LockFlags&=0xfb;//�屾���Զ��ѱ�
				i=MRR(ADDR_AFN05F149+2+(RS485PORT-1),1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				if(i==0xaa)
				{//ÿ��ִ��
					MC(0,Get_ADDR_UARTn(PORTn),6);//��fifoָ��
					UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				}
		#endif
		#if ((USER/100)==8)||((USER/100)==7)//�û���ʶ��ͨ�á��㽭
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						i=0;
						break;
					case RS485_2PORT://RS485-2
						i=1;
						break;
				}
				MWR(0,ADDR_AFN04F299_Ext+28,1);
				if(MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1)>=5)
				{//�ո���ʱ����
					MWR(0,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
				}
		#endif
		#if (USER/100)==8//�û���ʶ��ͨ��
				y=Comm_Ram->TYMDHMS[3];//��ǰ��
				y--;
				MC(0xff,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3+4+MaxRS485AddCarrierPn*3);
				p8=(u8 *)ADDR_DATABUFF;
				p8[0]=Comm_Ram->TYMDHMS[3];//��ǰ��
				p8[1]=Comm_Ram->TYMDHMS[4];//��ǰ��
				p8[2]=Comm_Ram->TYMDHMS[5];//��ǰ��
				DMW((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3);//ֱ�Ӵ洢��д(�޲���)

				y=p8[0]+(p8[1]<<8)+(p8[2]<<16);//��ǰ������
				y=YMD_Sub1D(y);//�����ռ�1��,���ؼ�1���������
				y&=0xff;
				y--;
				x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3);
				x=YMD_Add1D(x);//�����ռ�1��,���ؼ�1���������
				if(x==(p8[0]+(p8[1]<<8)+(p8[2]<<16)))
				{
					x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y+3,4);
					if(x==0xffffffff)
					{
						DMW((u32)ADDR_AFN0DF321_Ext_Source,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y+3,4);//ֱ�Ӵ洢��д(�޲���)
					}
				}

				MC(0,ADDR_AFN0DF321_Ext_Source,4);
		#endif
			}
		}
//�ܸ���
#if (USER/100)==3//�û���ʶ:����ϵ
	#if WeekRMTask==1//ÿ�ܳ�������;0=û��,1=��
		if(UARTCtrl->Task==0)//0=����
		{
			if(Comm_Ram->TYMDHMS[6]==1)
			{//������һ
				if(Comm_Ram->TYMDHMS[2]>=0x15)
				{//15�㿪ʼ
					x=UARTCtrl->RTCW[0]+(UARTCtrl->RTCW[1]<<8);//ԭ����
					y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//��ǰ����
					if(x!=y)
					{
						UARTCtrl->RTCW[0]=y;
						UARTCtrl->RTCW[1]=y>>8;
						UARTCtrl->RMTaskN|=4;//��ÿ������
						ClrRMTaskCompletes_RS485(PORTn,2,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
					}
				}
			}
		}
	#endif
#endif

//�����
		if(UARTCtrl->Task==0)
		{
			if(UARTCtrl->RTCS!=Comm_Ram->TYMDHMS[0])
			{
				UARTCtrl->RTCS=Comm_Ram->TYMDHMS[0];
				if(MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)//����ָ��ͨ�Ŷ˿���ͣ����
				{
					UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
					p16timer[0]=0;//������ʱ��
				}
			#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
				Terminal_AFN0CF11_RS485(PORTn,1);//�ն���1����F11,�ն˼��г���״̬��Ϣ;���Ҫ����ĳ��������
			#else
				Terminal_AFN0CF11_RS485(PORTn,MaxRMTask-1);//�ն���1����F11,�ն˼��г���״̬��Ϣ;���Ҫ����ĳ��������
			#endif
			//��ʱ�ɼ�ͣ���¼����
			#if TimerRMTask==1//��ʱ����ͣ���¼����;0=û��,1=��
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
				{//ѭ������
					i=MRR(ADDR_AFN04F97,1);//ͣ�����ݲɼ����ò���
					i&=1;
					if(i!=0)
					{//Ҫ�ɼ�
						i=MRR(ADDR_AFN04F97+1,1);//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
						if(i!=0)
						{//����ʱ����!=0
							p16=(u16*)(&Terminal_Ram->RS4851RMF246_M_Timer);//25 RS485-1ͣ���¼�����ʱ�����ֶ�ʱ��
							if(p16[RS485PORT-1]==0)
							{
								UARTCtrl->RMTaskN|=8;//�¶�ʱ��ͣ���¼����
							}
							i=MRR(ADDR_TYMDHMS+2,4);//��ǰYMDH
							x=MRR(((u32)&UARTCtrl->NextTimerRMTime)+2,4);
							if(i>=x)
							{
								UARTCtrl->RMTaskN|=8;//�¶�ʱ��ͣ���¼����
							}
							if(UARTCtrl->RMTaskN&8)
							{
								ClrRMTaskCompletes_RS485(PORTn,3,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
								i=MRR(ADDR_AFN04F97+1,1);//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
								p16[RS485PORT-1]=i*60;//��
								x=MRR(ADDR_TYMDHMS+2,4);
								x=YMDH_AddH(x,i);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
								d64a=x;
								d64a<<=16;//
								MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//��ʱ��ͣ���¼��������´γ���ʱ�����ʱ�����
	
								i=MRR(ADDR_AFN04F97+2,1);//ͣ���¼�����ʱ����ֵ	BIN	1	��λΪ����
								i*=60;
								p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1ͣ���¼�����ʱ����ֵ�붨ʱ��
								p16[RS485PORT-1]=i;
							}
						}
					}
				}
			#endif
				//��������ȼ�����
				NewTask_RS485(PORTn);//RS485��ִ��������;���أ�0=ûִ��������1=ִ��������
			#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭						
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						x=0;
						break;
					case RS485_2PORT://RS485-2
						x=1;
						break;
				}
				x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
				switch(x)
				{
					case 0:
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								i&=0x02;
								break;
							case RS485_2PORT://RS485-2
								i&=0x04;
								break;
						}
						if(i)
						{
							if(MRR(ADDR_AFN0CF249_Ext+9,1)!=0)
							{//�ն˾�ȷ��ʱ�ɹ�
								if(MRR(ADDR_AFN04F298_Ext,1)!=0)
								{//����ʱ���ؿ���
									if(MRR(ADDR_AFN04F298_Ext+6,1))
									{//�����ʱ������Ϊ0
										//���³���
										if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
										{
											switch(PORTn)
											{
												case RS485_1PORT://RS485-1
													i=0;
													break;
												case RS485_2PORT://RS485-2
													i=1;
													break;
											}
											MWR(1,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
											goto Re;
										}
									}
								}
							}
						}
						break;
					case 3:
						if(UARTCtrl->BroadcastTime<3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						{
							//���³���
							if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							{
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i=0;
										break;
									case RS485_2PORT://RS485-2
										i=1;
										break;
								}
								MWR(4,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
								i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i&=0xfd;
										break;
									case RS485_2PORT://RS485-2
										i&=0xfb;
										break;
								}
						Re:
							#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
								ClrRMEvene(PORTn);//������ܱ��¼�
							#endif
								p16timer[0]=0;//��������ʱ
								for(i=1;i<MaxRMTask;i++)
								{//���������1��MaxRMTask-1
									ClrRMTaskCompletes_RS485(PORTn,i,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
								}
								UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
								UARTCtrl->RMTaskN=0x02;//������b0-b7�ֱ��ʾ����0-7��������
								UARTCtrl->RMTaskC=(MaxRMTask-1);//ѭ��ʱ���ܲ�������ȼ�
								UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
								UARTCtrl->FnCount=0;//485����Fn����
								UARTCtrl->RMCount=0;//485�����������
								UARTCtrl->ReRMCount=0;//485���������ط�����
								UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
								UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
								//���г�����Ϣ
								p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
								p8+=(19*(RS485PORT-1));
								MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
							}
						}
						break;
				}
			#endif	
			}
		}


//�Զ��ѱ�
	#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
		if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
		{
	#endif
			i=MRR(ADDR_AFN05F149+2+(RS485PORT-1),1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			switch(i)
			{
				default:
					if(UARTCtrl->Task==0)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
					}
					break;
				case 0x55://�յ�������
					if(UARTCtrl->Task==0)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						UARTCtrl->RMCount=0;//�������ݱ�ʶ����
						UARTCtrl->ReRMCount=0;//485���������ط�����
						MWR(0x5a,ADDR_AFN05F149+2+(RS485PORT-1),1);//����ִ��
					#if (USER/100)!=5//�����Ϻ�
						MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+((RS485PORT-1)*LEN_PerPortAutoSearchMeter),7);//��0��ַ��ʼ�ѱ�,DL645-2007����3=2400
					#else
						//�Ϻ�Ĭ�ϵ�����4800
						MWR(0x04aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+((RS485PORT-1)*LEN_PerPortAutoSearchMeter),7);//��0��ַ��ʼ�ѱ�,DL645-2007����3=2400
					#endif
						UARTCtrl->LockFlags|=6;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
			#if (USER/100)==6//�û�:����
						i=MRR(ADDR_AFN05F103+1,2);
						i++;//��1��
						Terminal_Ram->AutoSearchMeterTimer=i;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
			#endif
						return;
					}
					else
					{//�ȴ�����
						if((UARTCtrl->LockFlags&2)!=0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
						{
							AutoSearchMeter_RS485_DL645(PORTn);//�Զ��ѱ�RS485�˿ڵ�DL645
							return;
						}
					}
					break;
				case 0x5a://����ִ��
		#if (USER/100)==6//�û�:����
					if(Terminal_Ram->AutoSearchMeterTimer==0)//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
					{
						if(UARTCtrl->Task==0)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{
							i=MRR(ADDR_AFN05F103,1);
							if(i==0x55)
							{//��F103����,��ÿ��ִ��
								MWR(0,ADDR_AFN05F149+2+(RS485PORT-1),1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
							}
							UARTCtrl->RMCount=0;//�����������=0
							UARTCtrl->ReRMCount=0;//���������ط�����
							UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
							MC(0,Get_ADDR_UARTn(PORTn),6);//��fifoָ��
							UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=�˵�����,B1=�Զ��ѱ�,B2-B7����
							return;
						}
					}
		#endif
					UARTCtrl->LockFlags|=6;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
					AutoSearchMeter_RS485_DL645(PORTn);//�Զ��ѱ�RS485�˿ڵ�DL645
					return;
				case 0xaa://ÿ��ִ��
					if((UARTCtrl->LockFlags&2)==0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
					{//û�����Զ��ѱ�,ÿ���ڳ������������
						if((UARTCtrl->LockFlags&4)==0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
						{//�������Զ��ѱ�==0
							if((Comm_Ram->TYMDHMS[2]>=0x20)&&(Comm_Ram->TYMDHMS[2]<0x23))
							{//ÿ����20:00-23:00����
								if(UARTCtrl->RMComplete&0x80)//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
								{
									if(UARTCtrl->Lock!=0x55)
									{
										UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
										UARTCtrl->RMCount=0;//�������ݱ�ʶ����
										UARTCtrl->ReRMCount=0;//485���������ط�����
										UARTCtrl->LockFlags|=6;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
										return;
									}
								}
							}
						}
					}
					else
					{//�Զ��ѱ�
						//if(UARTCtrl->RMKeyUser==0)//�����ص㻧��־,0=���泭��,1=�ص㻧����
						//{//�����ص㻧����
							AutoSearchMeter_RS485_DL645(PORTn);//�Զ��ѱ�RS485�˿ڵ�DL645
							if((Comm_Ram->TYMDHMS[2]<0x20)||(Comm_Ram->TYMDHMS[2]>=0x23))
							{//����ÿ����20:00-23:00
								if(UARTCtrl->Lock!=0x55)
								{
									UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
								}
							}
							return;
						//}
					}
					break;
			}
	#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
		}
	#endif
	
	}

//����ָ��ͨ�Ŷ˿����³���
	if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		if(MRR(ADDR_AFN05F51+RS485PORT,1)==0x55)//����ָ��ͨ�Ŷ˿����³���
		{
			MC(0,ADDR_AFN05F49+RS485PORT,1);//������ָ��ͨ�Ŷ˿���ͣ����
			MC(0x5a,ADDR_AFN05F51+RS485PORT,1);//�������³����0x5A
		#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			ClrRMEvene(PORTn);//������ܱ��¼�
		#endif
			p16timer[0]=0;//��������ʱ
			for(i=1;i<MaxRMTask;i++)
			{//���������1��MaxRMTask-1
				ClrRMTaskCompletes_RS485(PORTn,i,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
			}
			UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			UARTCtrl->RMTaskN=0x02;//������b0-b7�ֱ��ʾ����0-7��������
			UARTCtrl->RMTaskC=(MaxRMTask-1);//ѭ��ʱ���ܲ�������ȼ�
			UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
			UARTCtrl->FnCount=0;//485����Fn����
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
			UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			//���г�����Ϣ
			p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
			p8+=(19*(RS485PORT-1));
			MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
		}
	}
	
//ִ�й㲥Уʱ
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
#endif
		if(UARTCtrl->BroadcastTime>=3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
		{
			Terminal_BroadcastTime_DL645(PORTn);//DL645�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			return;
		}
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	}
#endif

//����
	if(UARTCtrl->RMFlag!=0x0)
	{//1=���������
		if(PORTn!=ACSAMPLEPORT)
		{
			p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
			p8+=(19*(RS485PORT-1));
			p8[4]|=0x1;//D0=1,����
		}
	}
	else
	{//0=��������ͣ,1=���������
		if(PORTn!=ACSAMPLEPORT)
		{
			p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
			p8+=(19*(RS485PORT-1));
			p8[4]&=0xfe;//D0=0,δ����
			if(MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)//����ָ��ͨ�Ŷ˿���ͣ����
			{
				CopyString((u8 *)"485 ������ͣ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			}
			else
			{
				CopyString((u8 *)"485 ��������ͣ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			}
			MWR(RS485PORT,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
		}
		UARTCtrl->RMTaskFlags=0;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	#if (USER/100)==5//�Ϻ�
		if(RS485PORT!=0)
		{//�����ڲ���������ͨ�ſ�
			if(p16timer[4]==0)
			{//������ʱ����ʱ��
				p16timer[4]=0xffff;//�¼����ʱ
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);
				i=hex_bcd(i);
				x=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);//ʱ��
			#if ((Project/100)==2)
				if(x<i)
				{//��0���ĵ�1������������
					UARTCtrl->RMTaskFlags|=0xfe;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
					UARTCtrl->RMFlag=1;//1=���������
				}
			#else
				if(x<i)
				{//��0���ĵ�1������������
					UARTCtrl->RMTaskFlags|=0xfe;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				}
				else
				{
					UARTCtrl->RMTaskFlags|=0x04;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				}
				UARTCtrl->RMFlag=1;//1=���������
			#endif
			}
		}
	#endif
		i=DataComp(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		if((i!=2)||(p16timer[0]==0))
		{//��Ȼ�TYMDHMS>NextRMTime
			//�����־
			UARTCtrl->RMTaskFlags|=0xfb;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
		#if (USER/100)==5//�Ϻ�
			if(RS485PORT!=0)
			{//�����ڲ���������ͨ�ſ�
				//����ʱ����ʱ
				i=MRR(ADDR_AFN04F242,1);//�ն˶Ե��ܱ�����Ʋ���(��)
				x=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);//��
				if(i>=x)
				{//��ʱ����ʱ��>=��������
					i=x;
					if(i!=0)
					{//��СС1��
						i--;
					}
				}
				i*=60;
				i+=2;//��2�����0ʱͬʱ
				p16timer[4]=i;//��ʱ�����붨ʱ��
			}
		#endif
		}
		if(UARTCtrl->RMTaskFlags==0)
		{//��ʱû��
		/*
			//���г�����Ϣ��ʼ����ʱ��
			p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
			p8+=(19*(RS485PORT-1));
			if(p8[14]==0xee)
			{//�޽���ʱ��
				MR(((u32)p8)+14,ADDR_AFN0CF2,6);
			}
		*/
			return;
		}
		if((MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//����ָ��ͨ�Ŷ˿���ͣ����
		{
			return;
		}
		UARTCtrl->RMFlag=1;//1=���������
		if(RS485PORT!=0)
		{//�����ڲ���������ͨ�ſ�
			if(UARTCtrl->RMTaskFlags&0x1)
			{//�ǳ��泭��ʱ��
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);//��
				if(i==0)
				{
					i=1;//��С1��
				}
				d64a=MRR(ADDR_TYMDHMS+1,5);
				d64a=YMDHM_AddM(d64a,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
				if((60%i)==0)
				{//���ʱ�����������60��
					//ͬ���������ʱ���ֵ
					x=d64a;
					x&=0xff;
					x=bcd_hex(x);
					x/=i;
					x*=i;
					x=hex_bcd(x);
					d64a>>=8;
					d64a<<=8;
					d64a|=x;
				}
				d64a<<=8;//��ֵ=0
				MWR(d64a,(u32)&UARTCtrl->NextRMTime,6);
				
				i*=60;
				i+=2;//���2,����59�붨ʱ��,ͬ�����ֹ�0,�����1��
				p16timer[0]=i;//�ն˳�����
			/*
				//���г�����Ϣ��ʼ����ʱ��
				Terminal_AFN0CF2_Computer();//�ն���1����F2,�ն�����ʱ��
				p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
				p8+=(19*(RS485PORT-1));
				MR(((u32)p8)+8,ADDR_AFN0CF2,6);//��ʼʱ��
				MC(0xee,((u32)p8)+14,6);//����ʱ��
			*/
			}
		}
		else
		{//���ڲ���������ͨ�ſ�
			p16timer[0]=1;//������ʱ��̶�Ϊ1s
		}
	}

	if(UARTCtrl->FnEnd==0)//485����Fn����������־,0=����,1=û����
	{
		if(AFN04F10MaxPn>100)
		{
			y=100;//����������100ֻ��
		}
		else
		{
			y=AFN04F10MaxPn;
		}
		for(x=0;x<y;x++)
		{
			UARTCtrl->MeterNo++;
			if(UARTCtrl->MeterNo>AFN04F10MaxPn)
			{
				//ȫ����1�ֳ���
			#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭						
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
				{//ѭ���������
					switch(PORTn)
					{
						case RS485_1PORT://RS485-1
							i=0;
							break;
						case RS485_2PORT://RS485-2
							i=1;
							break;
					}
					i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
					switch(i)
					{
						case 1:
							switch(PORTn)
							{
								case RS485_1PORT://RS485-1
									i=0;
									break;
								case RS485_2PORT://RS485-2
									i=1;
									break;
							}
							MWR(2,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
							break;
						case 4:
							switch(PORTn)
							{
								case RS485_1PORT://RS485-1
									i=0;
									break;
								case RS485_2PORT://RS485-2
									i=1;
									break;
							}
							MWR(5,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
							break;
					}
				}
			#endif	

			#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			#if (USER/100)==5//�Ϻ�;���ܱ��¼��ɼ���������
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
				{//��ѭ��
					if(UARTCtrl->RMTaskFlags&1)//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
					{//�ǳ�ʵʱ
						//ÿ����������������ɼ���ɺ�,��1���¼�
						UARTCtrl->RMEventTaskN|=1;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
						//ÿ���1�ֳ���,��2���¼�
						if((UARTCtrl->RMEventTaskN&0x20)!=0)
						{
							UARTCtrl->RMEventTaskN&=~0x20;
							UARTCtrl->RMEventTaskN|=2;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3,;b6=�¸���(�Ϻ�ÿ�³��¼���)
						}
						//ÿ�µ�1�ֳ���,��3���¼�
						if((UARTCtrl->RMEventTaskN&0x40)!=0)
						{//�¸���
							UARTCtrl->RMEventTaskN&=~0x40;
							UARTCtrl->RMEventTaskN|=4;
						}
					}
				}
			#endif
			#endif
				UARTCtrl->MeterNo=0;//���1��ѭ������,���ܱ�/��������װ�����=0
				if(RS485PORT==0)
				{//���ڲ���������ͨ�ſ�
					i=UARTCtrl->RMComplete;
					if((i&0x0f)<0x0f)
					{
						i+=1;//���ѭ���������+1
					}
					i|=0x80;
					UARTCtrl->RMComplete=i;
					UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
				}
				else
				{//���ⲿ485��
					if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
					{//ѭ���������
						i=UARTCtrl->RMComplete;
						if((i&0x0f)<0x0f)
						{
							i+=1;//���ѭ���������+1
						}
						i|=0x80;
						UARTCtrl->RMComplete=i;
					#if DateRMTask==0//ÿ�ճ�������;0=û��,1=��
						//��ͣǰ���ն˼��г���״̬��Ϣ,�����0��ɱ�־
						Terminal_AFN0CF11_RS485(PORTn,MaxRMTask-1);//�ն���1����F11,�ն˼��г���״̬��Ϣ;���Ҫ����ĳ��������
					#endif
						ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-1,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
					}
					else
					{//�ǲ�������ȼ��������
						if(UARTCtrl->RMTaskC!=0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						{//�����ϵ�
							//�ָ�ѭ��
							UARTCtrl->FnCount=UARTCtrl->FnCountS;//485����ԭѭ�������Fn����
							i=UARTCtrl->MeterNoS;//485����ԭѭ�������������ż���(���ֶ���)
							if(i!=0)
							{
								i--;
							}
							UARTCtrl->MeterNo=i;
							//��������ÿ������
							if(UARTCtrl->DateReRMCount<2)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
							{
								i=NoRMCompletes_RS485(PORTn,1);//RS485δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
								if(i!=0)
								{//�貹��
									UARTCtrl->MeterNo++;
									UARTCtrl->RMTaskN|=2;//��ÿ������
								}
							}
						#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
							if(UARTCtrl->RMTaskC==(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
							{
								DelRMEvene(PORTn);//ɾ�������ܱ��¼�����
								if(UARTCtrl->RMTaskC==(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
								{//�ָ����ǳ����ϵ�1���ĵ��ܱ��¼�
									return;
								}
							#if (USER/100)==5//�Ϻ�;���ܱ��¼��ɼ���������
							//�Ϻ���ѭ���������¼�,�¼�����������ͣ
								UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
							#endif
							}
						#endif
						}
						if((UARTCtrl->RMTaskC==0)||(UARTCtrl->RMTaskC==3))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						{//�ǳ�ͣ���¼�������
							if((Terminal_Ram->PowerTask==1)||(Terminal_Ram->PowerTask==10))//10=�ȴ�������ɱȶԵ��ܱ�
							{//�¼�����:�ȴ�������ɱȶԵ��ܱ�
								//i=NoRMCompletes_RS485(PORTn,UARTCtrl->RMTaskC);//RS485δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
								//if(i!=0)
								//{//ûȫ���ɹ�
									if(UARTCtrl->RMTaskC==0)
									{
										if(p16timer[8]!=0)//ͣ���¼�����ʱ����ֵ�붨ʱ��
										{
											ClrRMTaskCompletes_RS485(PORTn,0,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
											return;
										}
									}
								//}
							}
							//�´γ�ͣ���¼���ʱ��
							i=MRR(ADDR_AFN04F97+1,1);//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
							p16=(u16*)(&Terminal_Ram->RS4851RMF246_M_Timer);//25 RS485-1ͣ���¼�����ʱ�����ֶ�ʱ��
							p16[RS485PORT-1]=i*60;//��
							x=MRR(ADDR_TYMDHMS+2,4);
							x=YMDH_AddH(x,i);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
							d64a=x;
							d64a<<=16;//
							MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//��ʱ��ͣ���¼��������´γ���ʱ�����ʱ������
						}
						UARTCtrl->RMTaskC=(MaxRMTask-1);//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
					}
					NewTask_RS485(PORTn);//RS485��ִ��������;���أ�0=ûִ��������1=ִ��������
				}
				return;
			}
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
			//if(i==MRR(ADDR_Protocol_No,1))
			if(i==2)
			{//��������װ��ͨ��Э��
				if(PORTn==ACSAMPLEPORT)
				{//��ǰ�����ڲ�������
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//����������� BIN 2 
					if((i!=0x0)&&(i<=MaxRS485AddCarrierPn))
					{//�����������!=0x0
						//��������
						UARTCtrl->FnEnd=1;//485����Fn����������־,0=����,1=û����
						return;
					}
				}
			}
			else
			{
				if(PORTn!=ACSAMPLEPORT)
				{//��ǰ�ڲ����ڲ�������
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//ͨ�����ʼ��˿ں�
					i&=0x1f;
					if(i==(RS485PORT+1))
					{//ͨ�Ŷ˿ں���ͬ
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//����������� BIN 2 
						if((i!=0x0)&&(i<=MaxRS485AddCarrierPn))
						{//�����������!=0x0
							//��������
							UARTCtrl->FnEnd=1;//485����Fn����������־,0=����,1=û����
							return;
						}
					}
				}
			}
		}
		return;
	}
	if((Terminal_Ram->CongealFlags&0x1f)==0x1f)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	{//�����ڶ�������
		ReadMeter_Protocol(PORTn);//�����Լ
	}
}



















