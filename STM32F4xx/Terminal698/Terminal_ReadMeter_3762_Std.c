
//���ż���֧��:�콣Ӣ	13625322351
//             ���� 18661440042

//����·��������ϵ�ˣ��ⳣ���ֻ�13589207445,�绰0532-83676919,����wucy@eastsoft.com.cn

//��˹��15989482340 �鰮ϼ



#include "Project.h"
#include "Terminal_ReadMeter_3762_Std.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "KeyUser.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_ReadMeter_Fn.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"

#include "Terminal_ReadMeter_ShH_Lib.h"
#include "Terminal_ReadMeter_ShH_FnData.h"
#include "Terminal_ReadMeter_ShH.h"

#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

	#if (USER/100)==5//�Ϻ�
extern const u32 ShH_RMLib[];
u32 Get_RM_ShH_MaxFn(void);
	#endif


extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];

#define MaxReRMCount     1//�����ط�����(���ط�����,�����ʼ1�εķ���)



void Uart_3762_AFN14F1_Tx(u32 PORTn,u32 RMFlag)//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
{
	u32 x;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{
		p8txbuff[3]=0x03;//������;����ʽ·���ز�ͨ��
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{
			p8txbuff[3]=0x0a;//������;΢��������ͨ��
		}
		else
		{
			p8txbuff[3]=0x01;//������;����ʽ·���ز�ͨ��
		}
	}
	p8txbuff[4]=4;//ͨ��ģ���ʶ=1
	MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
	MR(((u32)p8txbuff)+16,((u32)p8rxbuff)+14,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
	p8txbuff[22]=0x14;//AFN=0x14
	p8txbuff[23]=0x01;//Fn=1
	p8txbuff[24]=0x00;//Fn=1
	p8txbuff[25]=RMFlag;//������־=0����ʧ��,=1�����ɹ�,2=����
	p8txbuff[26]=0;//ͨ����ʱ����Ա�־,0=�޹�,1=�й�
	p8txbuff[27]=0;//DL645֡����=0
	p8txbuff[28]=0;//�����ڵ�����N=0
	x=29+2;//���ܷ����ֽ���
	p8txbuff[1]=x;
	p8txbuff[2]=x>>8;
	UARTCtrl->TxByte=x;//�����ֽ���(���ֶ���)
	/*
	switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
	{
		default://����
		case 2://����
			break;
		case 3://����
			p8txbuff[4]=0x0;//ͨ��ģ���ʶ=0
			MR(((u32)p8txbuff)+10,((u32)p8txbuff)+22,7);
			x=17+2;//���ܷ����ֽ���
			p8txbuff[1]=x;
			p8txbuff[2]=x>>8;
			UARTCtrl->TxByte=x;//�����ֽ���(���ֶ���)
			break;
	}
	*/
	Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
	UARTCtrl->Task=3;//�ȴ�����			
	UARTCtrl->TxToRxTimerOver=0;//1000/10//���ͺ���ճ�ʱ��ʱֵ=0,ʹ�ܺܿ쳬ʱ��ת�����Ա�����ת��
}


void GeneralReadMeter_3762_Std(u32 PORTn)//��׼�ز�ģ��,���泭��
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	u32 z;
#endif
	u32 ClassNo;
	u32 ListNo;
	u32 Fn;
	u32 Pn;
	u32 MaxFnCount;
	u32 Protocol;
	u32 AddrAFN04F10;
	u64 d64a;
	u64 d64b;
	u32 Info;
	
	u8 * p8RMLib;
	u32 * p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
	u32 AutoReportSearch;//�����ϱ�������־
#endif
#endif

#if ((USER/100)==5)||((USER/100)==8)||((USER/100)==7)//�Ϻ���ͨ�á��㽭
	u8 *p8;
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	
	switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		case 0://0=����
			UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			UARTCtrl->Task=5;//���ڽ���
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			i=MRR(ADDR_TYMDHMS+1,2);
		#if ((USER/100)==5)//�Ϻ�
			p8=(u8*)(ADDR_AFN04F242);//������ʱ������
			x=p8[0];
			if(x>=60)//������ʱ��������1-60����
			{
				x=0x100;//1:00
			}
			else
			{
				x=hex_bcd(x);
			}
			if((i<RMM_EndHM)&&(i>=x))
			{//������ÿ��0�㸽����ͣ����
				if((Terminal_Router->StopFlags&(1<<14))!=0)//��ͣ·�ɱ�־:b14=�ȴ�ʱ�ӹ�0
				{
					Terminal_Router->StopFlags&=~(1<<14);//��ͣ·�ɱ�־:b14=�ȴ�ʱ�ӹ�0
					Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				}
			}
		#else
			if((i<RMM_EndHM)&&(i>=RMM_StartHM))
			{//������ÿ��0�㸽����ͣ����
				if((Terminal_Router->StopFlags&(1<<14))!=0)//��ͣ·�ɱ�־:b14=�ȴ�ʱ�ӹ�0
				{
					Terminal_Router->StopFlags&=~(1<<14);//��ͣ·�ɱ�־:b14=�ȴ�ʱ�ӹ�0
					Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				}
			}
		#endif
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 �ز�·���������붨ʱ��
			{
				Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 1://1=���1֡����
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�ز�·���������붨ʱֵ
			i=p8rxbuff[4];//��ͨ��ģ���ʶ
			i&=0x4;
			if(i==0x0)
			{//������ͨ��ģ��
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				switch(i)
				{
					case 0x1401://AFN=0x14,Fn=1;·�����������󳭱�
						if(UARTCtrl->RMTaskC!=0)//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
						{//��������
							i=MRR(ADDR_TYMDHMS+1,2);
							if((i>=RMM_EndHM)||(i<RMM_StartHM))
							{//��ÿ��0�㸽����ͣ����
								UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								WarningOccur(PORTn,(u8 *)"�ز�ͣ���ȴ�ʱ�ӹ�0");//�澯����,��ڸ澯����͸澯�ַ���
								if(Terminal_Router->StopFlags==0)//��ͣ·�ɱ�־:b14=�ȴ�ʱ�ӹ�0
								{
									Terminal_Router->StopFlags|=(1<<14);//��ͣ·�ɱ�־:b14=�ȴ�ʱ�ӹ�0
									Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
								}
								return;
							}
						}
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//����10�ֽڽ���ͷ������ͷ
						d64a=MRR(((u32)p8rxbuff)+14,6);//ȡ�ӽڵ��ַ
						x=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
						if(x>MaxRS485AddCarrierPn)
						{
							x=MaxRS485AddCarrierPn;
						}
						p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
						for(ListNo=0;ListNo<x;ListNo++)
						{
							d64b=p32[1]&0xffff;
							d64b<<=32;
							d64b|=p32[0];
							if(d64a==d64b)
							{
								break;
							}
							p32+=(LEN_NoteList/4);
						}
						if(ListNo==x)
						{//û�ҵ��ڴ��дӽڵ��б�,ֱ�ӷ��س����ɹ�
							Uart_3762_AFN14F1_Tx(PORTn,1);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
							return;
						}
						if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
						{//�����񼺳ɹ�����,ֱ�ӷ��س����ɹ�
							Uart_3762_AFN14F1_Tx(PORTn,1);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
							if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]==2)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
							{
								CopyString((u8 *)"�ز��ɹ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							}
							else
							{
								CopyString((u8 *)"�ز�����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							}
						//dispskip:
							d64a<<=16;
							for(i=0;i<6;i++)
							{
								x=d64a>>56;
								d64a<<=8;
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
							return;
						}
						p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
						x=Terminal_Note->List[ListNo].MeterNo;//ȡ��Ӧ���ܱ�/��������װ�����(��1��ʼ)
						if((x==0)||(x>AFN04F10MaxPn))
						{//��Ŵ�,����,ֱ�ӷ��س����ɹ�
					#if (USER/100)!=0
						RMOK:
					#endif
							Terminal_Note->List[ListNo].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
							goto END_Fn;
						}
					#if (USER/100)!=0//���Ժ��������F33ֻ���ص��BUG
						i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//�ն˳������в�������,�˿�31
						if((i&0x02)!=0x0)
						{//D1��"1"Ҫ���ն�ֻ���ص����"0"Ҫ���ն˳����б�
							y=MRR(ADDR_AFN04F35,1);//̨�����г����ص㻧����
							if(y>MaxKeyUser)
							{
								y=MaxKeyUser;
							}
							for(i=0;i<y;i++)
							{
								if(MRR(ADDR_AFN04F35+1+(2*i),2)==x)
								{
									break;
								}
							}
							if(i==y)
							{//�����ص㻧
								goto RMOK;//����,ֱ�ӷ��س����ɹ�
							}
						}
					#endif
						x--;
						AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
						d64b=MRR(AddrAFN04F10+6,6);//ȡ���õ�ͨ�ŵ�ַ
						if(d64a!=d64b)
						{//���ñ仯
						RouterSyn:
							if(Terminal_Ram->RouterSynDelay_S_Timer==0)//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
							{//ɾ��ʱ��������ͬ��,����Ժ�����Բ������Լ��30������ʱ�䲻��Ӧ
								Terminal_Router->AFN04F10=0;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
								Terminal_Router->RouterSynTask=1;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
								UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								return;
							}
							Terminal_Note->List[ListNo].RMErr=1;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
							goto END_Fn;
						}
						Pn=MRR(AddrAFN04F10+2,2);//ȡ���õĲ������
						if(Pn==0)
						{//����ɾ��
							goto RouterSyn;//·ͬ��
						}
					#if (USER/100)!=0//���Ժ��������F33��BUG
						i=MRR(ADDR_AFN04F30+(Pn-1),1);//̨�����г���ͣ��/Ͷ������(�������)
						if(i!=0)
						{//ͣ��
							goto RMOK;//����,ֱ�ӷ��س����ɹ�
						}
					#endif
						Protocol=MRR(AddrAFN04F10+5,1);//ȡͨ��Э������:1=DL/T645-1997,30=DL/T645-2007
						switch(Protocol)
						{
							case 1://DL/T645-1997
								MaxFnCount=Get_RM_DL645_1997_MaxFn();
								break;
						#if (USER/100)==5//�Ϻ��û�
							case 21://�Ϻ���Լ
								MaxFnCount=Get_RM_ShH_MaxFn();
								break;
						#endif
							default://DL/T645-2007
								MaxFnCount=Get_RM_DL645_2007_MaxFn();
								break;
						}
						if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
						{//Fn����>=���Fn����,���������
			END_Fn:
							if(Terminal_Note->List[ListNo].RMErr==0)//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
							{//�ɹ�
								Uart_3762_AFN14F1_Tx(PORTn,1);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
								Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
							}
							else
							{//������1�����ݱ�ʶ����ʧ��
								Uart_3762_AFN14F1_Tx(PORTn,0);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
								Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
								
								CopyString((u8 *)"�ز�ʧ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								d64a<<=16;
								for(i=0;i<6;i++)
								{
									x=d64a>>56;
									d64a<<=8;
									MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
									MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
								}
								MWR(0,ADDR_DATABUFF+8+12,1);
								WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
							}
							Terminal_Note->List[ListNo].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
							Terminal_Note->List[ListNo].FnCount=0;//Fn����
							Terminal_Note->List[ListNo].RMCount=0;//�������ݱ�ʶ����
							Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����
							Terminal_Note->List[ListNo].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
							UARTCtrl->RMCount=0;//����ֵҲ��Ϊ0,�����ص㻧���������ж���
							return;
						}
						
						ClassNo=MRR(AddrAFN04F10+26,1);//ȡ��С���
					#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
						AutoReportSearch=0;//�����ϱ�������־
					#endif
					#endif
						while(1)
						{
							if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
							{//Fn����>=���Fn����,���������
								goto END_Fn;
							}
							switch(Protocol)
							{
								case 1://DL/T645-1997
									i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
									break;
							#if (USER/100)==5//�Ϻ��û�
								case 21://�Ϻ���Լ
									i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
									break;
							#endif
								default://DL/T645-2007
									i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
									break;
							}
							if(i==0)
							{//��Fn�ǿ�
					NEXT_Fn:
								Terminal_Note->List[ListNo].FnCount++;//Fn����+1
								Terminal_Note->List[ListNo].RMCount=0;//�������ݱ�ʶ����
								Terminal_Note->List[ListNo].ReRMCount=0;//�ط�����
								Terminal_Note->List[ListNo].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
								
					#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
								if(UARTCtrl->RMTaskC!=0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
								{//��������
									if(AutoReportSearch==0)//�����ϱ�������־
									{
										AutoReportSearch=1;//�����ϱ�������־
										x=Check_Meter_Event_Save();//�����ܱ��¼������ռ�;����0=�洢�M(û�ϱ�),1-255�����ɴ洢������
										if(x>=10)
										{//�ռ����ٿɴ�10��
											x=Check_AutoReportWord(PORTn);//�����ܱ������ϱ�״̬��;����:0=��,!=0 B31-B16Ϊ�������(1��ʼ),B15-B0ΪҪ��ȡ�������ϱ�״̬��λ��(0-95)
											x>>=16;
											if(x!=0)
											{//�������ϱ���Ҫ��
												Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485����Fn����(ԭ���泭���Fn�����Ĵ�)
												UARTCtrl->MeterNo=x;
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//ȡ�ӽڵ��ַ
												x=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
												if(x>MaxRS485AddCarrierPn)
												{
													x=MaxRS485AddCarrierPn;
												}
												p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
												for(ListNo=0;ListNo<x;ListNo++)
												{
													d64b=p32[1]&0xffff;
													d64b<<=32;
													d64b|=p32[0];
													if(d64a==d64b)
													{
														break;
													}
													p32+=(LEN_NoteList/4);
												}
												Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ȡͨ��Э������:1=DL/T645-1997,30=DL/T645-2007
												Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
											#if (USER/100)==5//�Ϻ��û�
												if((ListNo==x)||(Protocol==1)||(Pn==0)||(Protocol==21))
											#else
												#if(USER/100)==7//�㽭�û�
												if((ListNo==x)||(Pn==0))
												#else
												if((ListNo==x)||(Protocol==1)||(Pn==0))
												#endif
											#endif
												{//û�ҵ��ڴ��дӽڵ��б��DL645-1997�������ɾ��
													//���¼���־,����ѭ��
													MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),LEN_AutoReportWord);
													UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485������ܱ�/��������װ�����(���ֶ���)(ԭ���泭���װ����żĴ�)
													return;
												}
												
												Terminal_Note->List[ListNo].FnCount=2;
												AutoReportSearch=0;//�����ϱ�������־
												UARTCtrl->RMMeterEvent|=1;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
												UARTCtrl->FnEnd=1;//485����Fn����������־,0=����,1=û����
												
												Terminal_Router->StopFlags|=(1<<17);//��ͣ·��;b17=�������ϱ��¼�
												Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
												UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
												UARTCtrl->Task=0;//����
												return;
											}
										}
									}
									if(Protocol==30)//30=DL/T645-2007
									{//��ǰֻ��2007�����F2(���ܱ������ϱ��¼�)
										p8RMLib=(u8 *)DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
										if(p8RMLib[0]==2)//F2
										{
											Terminal_Note->List[ListNo].FnCount++;//Fn����+1;�������¼���������
										}
									}
								#if(USER/100)==7//�㽭�û�
									if(Protocol==1)//1=DL/T645-1997
									{//�㽭1997��Ҳ��F2(���ܱ������ϱ��¼�)
										MaxFnCount=Get_RM_DL645_1997_MaxFn();
										if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
										{//Fn����>=���Fn����,���������
											goto END_Fn;
										}
										p8RMLib=(u8 *)DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
										if(p8RMLib[0]==2)//F2
										{
											Terminal_Note->List[ListNo].FnCount++;//Fn����+1;�������¼���������
										}
									}
								#endif
								}
					#endif
					#endif
								continue;
							}
							p8RMLib=(u8 *)i;//�����
							Fn=p8RMLib[0];//ȡʵʱFn
							if((p8RMLib[3]&0x04)==0)//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
							{
								goto NEXT_Fn;
							}
							if(p8RMLib[1]!=0)
							{//��Fn����������!=0
								if(Terminal_Note->List[ListNo].RMCount>=p8RMLib[1])
								{//�������ݱ�ʶ����>=Fn�跢������;��Fn�������
									if(p8RMLib[1]>Terminal_Note->List[ListNo].RMCountErr)
									{//����ȫ����ʶȫ������ʧ��
										ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
									}
									goto NEXT_Fn;
								}
							}
							UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//���ƴ�ֵ,�����ص㻧���������ж���
						#if Extern_ReadMeter_Fn_YesNo==0//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)	
							y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,Terminal_Note->List[ListNo].RMCount,Protocol);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��
						#else
							y=0;
							if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
							{
								p32=(u32 *)(ADDR_E1_ENTRY);
								y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)Terminal_Note->List[ListNo].RMCount,(u32)Protocol);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������
							}
						#endif
							if(Fn==26)
							{
							#if (USER/100)==5//�Ϻ�
								if((Protocol!=1) && (Protocol!=21))//(1=1997,21=�Ϻ���Լ,����=2007)
							#else
								if(Protocol!=1)//(1=1997,����=2007)
							#endif
								{//2007
								#if RMM_AFN0CF26==0//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
									if(Terminal_Note->List[ListNo].RMCount==12)//�������ݱ�ʶ����
								#endif
								#if RMM_AFN0CF26==1//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
									if(Terminal_Note->List[ListNo].RMCount==16)//�������ݱ�ʶ����
								#endif
									{//�����������16�����Ǽ�������,ֻ�в��ɹ�ʱ�ų�
										for(i=0;i<(1+61);i++)
										{
											if(p8FnData[i]!=0xee)
											{
												break;
											}
										}
										if(i!=(1+61))
										{//�г��ɹ�
											y=0;//����
											Terminal_Note->List[ListNo].RMCount=100;//�������ݱ�ʶ����
										}
									}
								}
							}
						#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
							if(y!=0)
							{
							//��չ14 ������2���ն�������
							//��չ15 ������3���ն�������
								if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
								{
									y=0;//
									x=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰ������
									x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
									if(p8RMLib[0]==0xEE)
									{
										y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
									}
									else
									{
										x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
										y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
									}
									y=MRR(y,3);//ȡ��?�ղ���ʱ��
									if(x==y)
									{//����
										goto NEXT_Fn;
									}
									x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
									i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
									if(i!=0xffffffff)
									{//�ҵ�
										z=Terminal_Note->List[ListNo].RMCount;//�����������
										if((z>0)&&(z<9))
										{
											z+=160;//�����������1-8��ӦFn=161-168
											z=GetClass2DataOfficeAddr(z,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
											if(z!=0)
											{//�ҵ�
												y=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰ������
												z=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+z+2,3);//ȡԭ����ʱ��������
												if((z!=0xeeeeee)&&(z!=0xffffff))
												{
													if((z>x)&&(z<=y))
													{//ԭ����ʱ��>��2��ʱ��,ͬʱ<=��ǰʱ��
														goto NEXT_Fn;//����
													}
												}
											}
										}
									}
									while(1)
									{
										i=Terminal_Note->List[ListNo].RMCount;//�������ݱ�ʶ����
										y=1;//�������ݱ�ʶ����=0��9����Ҫ��
										if(i>9)
										{
											break;
										}
										if((i>0)&&(i<9))
										{
											i+=160;//Ҫ����Fn=161-168
											y=Class2Data_Fn(ClassNo,i);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��		
										}
										if(y==0)
										{
											Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
											Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����
										}
										else
										{
											break;
										}
									}
									if(Terminal_Note->List[ListNo].RMCount>=p8RMLib[1])
									{//�������ݱ�ʶ����>=Fn�跢������;��Fn�������
										if(p8RMLib[1]>Terminal_Note->List[ListNo].RMCountErr)
										{//����ȫ����ʶȫ������ʧ��
											ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
										}
										goto NEXT_Fn;
									}
								}
							}
						#endif
							if(y!=0)
							{
								if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
								{//����1-3���ն�������
								#if ERC39==1
									if(p8RMLib[0]==0xEF)
									{
										if(Terminal_Note->List[ListNo].RMCount<=1)
										{
											ERC39_Event(Pn);//����ʧ���¼���¼
										}
									}
								#endif
								#if (USER/100)==4//�û���ʶ:����
									if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
									{
										i=MRR(AddrAFN04F10+18,1);//ȡ���ܱ����
										if(i==1)
										{
											y=0;//����ũ�������ն���
										}
									}
								#endif
									if(Terminal_Note->List[ListNo].RMCount==9)
									{//�ٴ�ȷ���ն���ʱ��YYMMDDhhmm
										//ʡ�Բ���,�򳭱��ڹ�0�����ʱ����,�ڴ��жϲ��޸ĳ������ٴ���
										y=0;
									}
								}
							}
							if(y==0x0)
							{//�������
								if(p8RMLib[1]==0x0)
								{//��Fn����������=0
									goto NEXT_Fn;
								}
								if(Terminal_Note->List[ListNo].RMCount==0)//�������ݱ�ʶ����
								{//�������ݱ�ʶ����=0,��Fn���ݻ���
									Terminal_Note->List[ListNo].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
									MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
								}
								Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
								Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����
								Terminal_Note->List[ListNo].ReRMCount=0;//�ط�����
								continue;
							}
							if(p8RMLib[1]==0x0)
							{//��Fn����������=0
							#if AFN0CF33MaxPn==0
								goto NEXT_Fn;
							#else
								if((p8RMLib[4]&0x01)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
								{//1������,ʵʱ
									switch(Fn)
									{
									//F33 ��ǰ������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
									//F34 ��ǰ������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
									//F35 ����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
									//F36 ���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
										case 33:
										case 34:
										case 35:
										case 36:
									//F37 ���£���һ�����գ�������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
									//F38 ���£���һ�����գ�������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
									//F39 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
									//F40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M�12�
										case 37:
										case 38:
										case 39:
										case 40:
											break;
										default:
											goto NEXT_Fn;
									}
									ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
									goto NEXT_Fn;
								}
							#endif
							}

							if(Terminal_Note->List[ListNo].RMCount==0)//�������ݱ�ʶ����
							{///�������ݱ�ʶ����=0,��Fn���ݻ���
								Terminal_Note->List[ListNo].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
								MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
							}
							
							if(Terminal_Note->List[ListNo].ReRMCount>MaxReRMCount)
							{//�����ط�����>MaxReRMCount
								ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
								if(p8RMLib[0]==129)
								{
									ClrERC29SOUR(Pn);//���г���ʧ����ERC29���ܱ�����¼�ԭʼ����
								}
								//�ճ����־
								if(UARTCtrl->RMTaskC==1)//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
								{//
									if(UARTCtrl->DateReRMCount>=3)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
									{
										DateRMFlags(Pn,1);//�ճ����־;���:f=0�ɹ�,=1ʧ��
									}
								}
								Terminal_Note->List[ListNo].RMErr=1;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
								Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
								Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����
								Terminal_Note->List[ListNo].ReRMCount=0;//�ط�����
								Terminal_Note->List[ListNo].RMErr_0x901F=0;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
							#if (USER/100)==5//�Ϻ�
								if(Protocol!=21)//(1=1997,21=�Ϻ���Լ,����=2007)
								{
									if((p8RMLib[0]==0xe3)||(p8RMLib[0]==0xee)||(p8RMLib[0]==0xef))
									{//����1-3���ն�������
										if(Terminal_Note->List[ListNo].RMCount==1)//�������ݱ�ʶ����
										{//�ǵ�1�����ն���ʱ��YYMMDDhhmm�����̳���Ҳ����
											goto END_Fn;//����,���ٷ���ʧ��,�Ա�ѧ�������л�·��
										}
									}
								}
							#else
								if((p8RMLib[0]==0xe3)||(p8RMLib[0]==0xee)||(p8RMLib[0]==0xef))
								{//����1-3���ն�������
									if(Terminal_Note->List[ListNo].RMCount==1)//�������ݱ�ʶ����
									{//�ǵ�1�����ն���ʱ��YYMMDDhhmm�����̳���Ҳ����
										goto END_Fn;//����,���ٷ���ʧ��,�Ա�ѧ�������л�·��
									}
								}
							#endif
								if(UARTCtrl->RMTaskC==2)
								{//�ܳ��������ʱ��,Ϊ�ӿ������˳��������������
									goto END_Fn;
								}
								continue;
							}
							Terminal_Note->List[ListNo].ReRMCount++;//�����ط�����+1
						
							if(p8RMLib[0]==0xED)
							{//��չ13 ��1-24��������������й�����ʾֵ
								//Fn101���й�����ʾֵ��������
								y=Class2Data_Fn(ClassNo,101);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								//Fn103���й�����ʾֵ��������
								y|=Class2Data_Fn(ClassNo,103);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								if(y==0)
								{
									goto NEXT_Fn;
								}
								if(Terminal_Note->List[ListNo].RMCount==0)
								{//�����������=0ʱ�б�,��������ʱ��
									KeyUserPnList();//���г����ص㻧���߶���Pn�б�
								}
								i=MRR(ADDR_TYMDHMS+2,4);//ȡ��ǰ������ʱ
								d64a=YMDH_SubH(i,Terminal_Note->List[ListNo].RMCount);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
								d64a<<=8;//��=0
								i=CheckCurveCongeal(d64a,101,Pn);//������߶���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
								if(i!=0)
								{//�Ѷ����û�ҵ������
									Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
									Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����
									Terminal_Note->List[ListNo].ReRMCount=0;//�ط�����
									continue;
								}
							}
						
							switch(Protocol)
							{
								case 1://DL/T645-1997
									//ȡDIͬʱ��0x33
									i=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//ȡDI
								#if (USER/100)==4//�����û�
									if(p8RMLib[0]==0xE3)
									{
										if(i==0x901f)
										{
											if(Terminal_Note->List[ListNo].RMErr_0x901F==2)//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
											{
												i=0x9010;
												Terminal_Note->List[ListNo].RMErr_0x901F=0;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
											}
											else
											{
												Terminal_Note->List[ListNo].RMErr_0x901F=1;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
											}
										}
										else
										{
											Terminal_Note->List[ListNo].RMErr_0x901F=0;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
										}
									}
								#endif
									p8txbuff[28+10+0]=i+0x33;
									p8txbuff[28+10+1]=(i>>8)+0x33;
									//ȡͨ�ŵ�ַ
									MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
									p8txbuff[28+0]=0x68;
									p8txbuff[28+7]=0x68;
									p8txbuff[28+8]=0x01;
									p8txbuff[28+9]=0x2;
									p8txbuff[28+13]=0x16;
									//CS����
									p8txbuff[28+12]=0;
									for(i=0;i<12;i++)
									{
										p8txbuff[28+12]+=p8txbuff[28+i];
									}
									//Ԥ��Ӧ���ֽ���
									i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];//���س���
									if(i==0)
									{//���س���=0
										i=10+(3*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
										i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
									}
									p8txbuff[6]=i+14;
									i=14;//DL645֡����
									break;
							#if (USER/100)==5//�Ϻ��û�
								case 21://�Ϻ���Լ
									p8txbuff[28+0]=0x68;
									//ȡͨ�ŵ�ַ
									MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
									//����Ҫ���Ϻ���Լ��ͨ�ŵ�ַA5�̶���0xff;
									p8txbuff[28+6]=0xff;
									p8txbuff[28+7]=0x68;
									//ȡ������
									i=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//ȡDI
									i|=0x80;
									p8txbuff[28+8+0]=i;
									//����
									p8txbuff[28+9]=0;
									//CS����
									p8txbuff[28+10]=0;
									for(i=0;i<10;i++)
									{
										p8txbuff[28+10]+=p8txbuff[28+i];
									}
									p8txbuff[28+11]=0x16;
									//Ԥ��Ӧ���ֽ���
									i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];//���س���
									p8txbuff[6]=i+14;
									i=12;//�Ϻ���Լ֡����
									break;
							#endif
								default://DL/T645-2007
									if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
									{//���Ƕ��������
										i=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//ȡDI
										switch(i)
										{//����������̫֡������
											case 0x03300101://F27 ���һ�ε�����㷢��ʱ��
											case 0x03300201://F27 ���һ���������㷢��ʱ��
											case 0x03300501://F166 ���һ�ε��ܱ�ʱ�β����޸�ʱ��
												Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
												Terminal_Note->List[ListNo].RMCount++;//�����������
												Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
												continue;
										}
									#if (USER/100)==4//�û���ʶ:����
										x=MRR(AddrAFN04F10+18,1);//ȡ���ܱ����
										if((x==1)&&(p8RMLib[0]==0xE3))
										{//�任�ն������ݱ�ʶ
											switch(Terminal_Note->List[ListNo].RMCount)
											{
												default://�ն���ʱ��YYMMDDhhmm
													Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
													Terminal_Note->List[ListNo].RMCount++;//�����������
													Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
													continue;
												case 1://��1���������й�����ʾֵ���ܡ�����1��M��
													i=0x00010000;//��ǰ�����й�����ʾֵ���ܡ�����1��M��
													break;
											}
										}
									#endif
										//DIͬʱ��0x33
										p8txbuff[28+10+0]=i+0x33;
										p8txbuff[28+10+1]=(i>>8)+0x33;
										p8txbuff[28+10+2]=(i>>16)+0x33;
										p8txbuff[28+10+3]=(i>>24)+0x33;
										//ȡͨ�ŵ�ַ
										MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
										p8txbuff[28+0]=0x68;
										p8txbuff[28+7]=0x68;
										p8txbuff[28+8]=0x11;
										p8txbuff[28+9]=0x4;
										p8txbuff[28+15]=0x16;
										//CS����
										p8txbuff[28+14]=0;
										for(i=0;i<14;i++)
										{
											p8txbuff[28+14]+=p8txbuff[28+i];
										}
										//Ԥ��Ӧ���ֽ���
										i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//���س���
										if(i==0)
										{//���س���=0
											i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
											i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
										}
										p8txbuff[6]=i+16;
										i=16;//DL645֡����
									}
									else
									{//���������
										//����ǰ������0�����ߵ�
										d64a=MRR(ADDR_TYMDHMS+1,5);//ȡ��ǰʱ��������ʱ��
									#if RMM_Carrier_CurveTime==15//�ز�ͨ����������ߵ���������ʱ���;15�ֻ�60��
										d64a=YMDHM_Int15(d64a);//������ʱ�ֻ�Ϊ��15��,��00ʱ18�ֻ�Ϊ00ʱ15��
										i=Terminal_Note->List[ListNo].RMCount*15;//ÿ15�ּ��
									#else
										d64a&=0xffffffff00;
										i=Terminal_Note->List[ListNo].RMCount*60;//ÿ60�ּ��
									#endif
										d64a=YMDHM_SubM(d64a,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
										i=p8RMLib[0];
										i-=0xe4;
										i<<=16;
										i+=0x06000001;
										MWR(i,((u32)p8txbuff)+28+10,4);
										MWR(1,((u32)p8txbuff)+28+10+4,1);//��1����
										MWR(d64a,((u32)p8txbuff)+28+10+4+1,5);
										//��0x33
										for(i=0;i<10;i++)
										{
											p8txbuff[28+10+i]+=0x33;
										}
										//ȡͨ�ŵ�ַ
										MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
										p8txbuff[28+0]=0x68;
										p8txbuff[28+7]=0x68;
										p8txbuff[28+8]=0x11;
										p8txbuff[28+9]=10;
										p8txbuff[28+21]=0x16;
										//CS����
										p8txbuff[28+20]=0;
										for(i=0;i<20;i++)
										{
											p8txbuff[28+20]+=p8txbuff[28+i];
										}
										//Ԥ��Ӧ���ֽ���
										i=p8RMLib[10+4];//���س���
										if(i==0)
										{//���س���=0
											i=10+(5*1);
											i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
										}
										p8txbuff[6]=i+16;
										i=22;//DL645֡����
									}
									break;
							}
							if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{
								p8txbuff[3]=0x03;//������;����ʽ·���ز�ͨ��
							}
							else
							{
								if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
								{
									p8txbuff[3]=0x0a;//������;΢��������ͨ��
								}
								else
								{
									p8txbuff[3]=0x01;//������;����ʽ·���ز�ͨ��
								}
							}
							p8txbuff[4]=4;//ͨ��ģ���ʶ=1
							MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
							MR(((u32)p8txbuff)+16,(u32)&Terminal_Note->List[ListNo].Addr,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
//						#if ((USER/100)==5)//�Ϻ��û�
//							if(Protocol==21)
//							{//�Ϻ���Լ
//								p8txbuff[16+4]=bcd_hex(p8txbuff[16+4]);//8λѹ��BCD��תΪHEX
//							}
//						#endif
							p8txbuff[22]=0x14;//AFN=0x14
							p8txbuff[23]=0x01;//Fn=1
							p8txbuff[24]=0x00;//Fn=1
							p8txbuff[25]=2;//������־=2
							p8txbuff[26]=0;//ͨ����ʱ����Ա�־,0=�޹�,1=�й�
							p8txbuff[27]=i;//DL645֡����
							p8txbuff[28+i]=0;//�����ڵ�����N=0
							i+=29+2;//���ܷ����ֽ���
							p8txbuff[1]=i;
							p8txbuff[2]=i>>8;
							UARTCtrl->TxByte=i;//�����ֽ���(���ֶ���)

							Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
							UARTCtrl->Task=3;//�ȴ�����
							UARTCtrl->TxToRxTimerOver=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
							//��ʾ�����ַ
							switch(UARTCtrl->RMTaskC)
							{
								case 0://�ϵ�
									CopyString((u8 *)"�ز�����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									break;
								case 1://�ճ�
									if(UARTCtrl->DateReRMCount<=1)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
									{
										CopyString((u8 *)"�ز��ճ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									}
									else
									{
										CopyString((u8 *)"�ز�����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									}
									break;
								case 2://�ܳ�
									CopyString((u8 *)"�ز�ʱ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									//CopyString((u8 *)"�ز��ܳ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									break;
								case 3://����
									CopyString((u8 *)"�ز�����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									break;
								case 4://ÿСʱ�ص㻧���߳���(�����������߶���ĵ����)
									CopyString((u8 *)"�ز��ص�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									break;

								default://ѭ��
									CopyString((u8 *)"�ز�ѭ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									break;
							}
							for(i=0;i<6;i++)
							{
								x=p8txbuff[16+5-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
							return;
						}
						//break;//��Ҫ,������while(1)
					case 0x0602://AFN=0x06,Fn=2;·���������ϱ���������
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//����10�ֽڽ���ͷ������ͷ
						Info=MRR(((u32)p8rxbuff)+4,4);//ȡ6�ֽ�376.2���ص���Ϣ��
						i=p8rxbuff[18];//��DL645֡����
						UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+16,2);//��·�ϴ�����ʱʱ����
						MR(((u32)p8rxbuff),((u32)p8rxbuff)+19,i);//ȡ645֡
						goto RMESC;//������
					case 0x1402://AFN=0x14,Fn=2;·����������ʱ��
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//����10�ֽڽ���ͷ������ͷ
						MW(ADDR_TYMDHMS,((u32)p8txbuff)+128,6);
						Uart_3762_TxData(PORTn,0x14,2,6);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
						break;
					//case 0x1404://AFN=0x14,Fn=3;������ͨ����ʱ����ͨ������
						//break;
					default:
						UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
						UARTCtrl->Task=5;//���ڽ���
						break;
				}
			}
			else
			{//���ܱ�ͨ��ģ��
				i=MRR(((u32)p8rxbuff)+22,3);
				switch(i)
				{
					case 0x000206://AFN=0x06,Fn=2;·���������ϱ���������
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//����10�ֽڽ���ͷ������ͷ
						Info=MRR(((u32)p8rxbuff)+4,4);//ȡ6�ֽ�376.2���ص���Ϣ��
						i=p8rxbuff[30];//��DL645֡����
						UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+28,2);//��·�ϴ�����ʱʱ����
						MR(((u32)p8rxbuff),((u32)p8rxbuff)+31,i);//ȡ645֡
				RMESC://������
						for(y=0;y<i;y++)
						{//ȥ����645֡��ǰ��0xfe
							if(p8rxbuff[y]!=0xfe)
							{
								if(y!=0)
								{
									MR((u32)p8rxbuff,((u32)p8rxbuff)+y,i-y);
								}
								break;
							}
						}
						d64a=MRR(((u32)p8rxbuff)+1,6);//֡���ر��ַ
						x=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
						p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
						for(ListNo=0;ListNo<x;ListNo++)
						{
							d64b=p32[1]&0xffff;
							d64b<<=32;
							d64b|=p32[0];
						#if (USER/100)==5//�Ϻ�
							i=Terminal_Note->List[ListNo].MeterNo;
							Protocol=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
							if(Protocol==21)
							{//�Ϻ���Լ
								//ͨ�ŵ�ַA5���ñȽ�
								d64a&=0xffffffffff;
								d64b&=0xffffffffff;
							}
						#endif
							if(d64a==d64b)
							{
								break;
							}
							p32+=(LEN_NoteList/4);
						}
						if(ListNo==x)
						{//û�ҵ��ڴ��дӽڵ��б�
							goto RXESC;//�ϱ�����
						}
						p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
						//MWR(Info,(u32)&Terminal_Note->List[ListNo].Info,4);//4�ֽ������ز������ص���Ϣ��
						
						x=Terminal_Note->List[ListNo].MeterNo;//ȡ��Ӧ���ܱ�/��������װ�����(��1��ʼ)
						if((x==0)||(x>AFN04F10MaxPn))
						{//��Ŵ�
							goto RXESC;//�ϱ�����
						}
						x--;
						AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
						Pn=MRR(AddrAFN04F10+2,2);//ȡ���õĲ������
						if(Pn==0)
						{//����ɾ��
							goto RXESC;//�ϱ�����	
						}
						d64b=MRR(AddrAFN04F10+6,6);//ȡ���õ�ͨ�ŵ�ַ
//					#if ((USER/100)==5)//�Ϻ��û�
//						Protocol=MRR(AddrAFN04F10+5,1);//ȡͨ��Э������:1=DL/T645-1997,30=DL/T645-2007
//						if(Protocol==21)
//						{//�Ϻ���Լ
//							i=(d64b>>24)&0xff;
//							d64a=hex_bcd(i);//HEX��תΪ8λѹ��BCD��
//							d64b&=0xffff00ffffff;
//							d64b+=(d64a<<24);
//						}
//						d64a=MRR(((u32)p8rxbuff)+1,6);//֡���ر��ַ
//					#endif
					#if (USER/100)==5//�Ϻ�
						Protocol=MRR(ADDR_AFN04F10+(x*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
						if(Protocol==21)
						{//�Ϻ���Լ
							//ͨ�ŵ�ַA5���ñȽ�
							d64a&=0xffffffffff;
							d64b&=0xffffffffff;
						}
					#endif
						if(d64a!=d64b)
						{//���ñ仯
							goto RXESC;//�ϱ�����
						}
					#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
						if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
						{//����ģ��
							if(p8txbuff[8]==0x01)
							{//�д��ϱ�ģʽ
								MR(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
								MWR(1,ADDR_DATABUFF+1,1);//B1����,�����ڱ�ʾ�������ϱ�
								MWR(1,ADDR_DATABUFF+96,1);//���ݻ�
								MW(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
							}
						}
					#endif
					#endif
						Protocol=MRR(AddrAFN04F10+5,1);//ȡͨ��Э������:1=DL/T645-1997,30=DL/T645-2007
						switch(Protocol)
						{
							case 1://DL/T645-1997
								i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						#if (USER/100)==5//�Ϻ��û�
							case 21://�Ϻ���Լ
								i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						#endif
							default://DL/T645-2007
								i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						}
						p8RMLib=(u8 *)i;//�����
						
						if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
						{//��ʼ��0x68��
							goto RXESC;//�ϱ�����
						}
						x=0;//��У�����
						for(i=0;i<(p8rxbuff[9]+10);i++)
						{
							x+=p8rxbuff[i];
						}
						x&=0xff;
						if((x!=p8rxbuff[i])||(p8rxbuff[i+1]!=0x16))
						{//��У�����
							goto RXESC;//�ϱ�����
						}
						for(i=0;i<p8rxbuff[9];i++)
						{//�������0x33
							p8rxbuff[10+i]-=0x33;
						}
					#if (USER/100)==5//�Ϻ��û�
						if(Protocol!=21)//�����Ϻ���Լ
						{
							if((p8rxbuff[8]&0x40)!=0x0)
							{//��վ�쳣�ش�,�޴�������,��������
								Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
								Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����+1
								Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
								goto RXESC;//�ϱ�����
							}
						}
					#else
						i=Terminal_Note->List[ListNo].RMErr_0x901F;//�ز�DL645-1997��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F����
						Terminal_Note->List[ListNo].RMErr_0x901F=0;//�ز�DL645-1997��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F����
						if((p8rxbuff[8]&0x40)!=0x0)
						{//��վ�쳣�ش�,�޴�������,��������
//20150310
						#if (USER/100)==4//�û�:����
							if(Protocol==1)
							{//DL/T645-1997
								if(p8RMLib[0]==0xE3)
								{
									if(Terminal_Note->List[ListNo].RMCount==1)//�������ݱ�ʶ����=1,����=0x901f
									{
										if(i==1)//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
										{
											Terminal_Note->List[ListNo].RMErr_0x901F=2;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
											if(	Terminal_Note->List[ListNo].ReRMCount!=0)//�����ط�����
											{
												Terminal_Note->List[ListNo].ReRMCount--;//�����ط�����
											}
											goto RXESC;//�ϱ�����
										}
									}
								}
							}
						#endif
							Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
							Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����+1
							Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
							goto RXESC;//�ϱ�����
						}
					#endif
						switch(Protocol)
						{
							case 1://DL/T645-1997
								x=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//ȡDI
								y=MRR(((u32)p8rxbuff)+10,2);
//20150310
							#if (USER/100)==4//�û�:����
								if(p8RMLib[0]==0xE3)
								{
									if(y==0x9010)
									{
										y=0x901f;
									}
								}
							#endif
								if(x!=y)
								{//DI����
									goto RXESC;//�ϱ�����
								}
								if(p8rxbuff[9]==2)
								{//���Ժ����������¼���¼������ʱ,ֻ����2�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
									Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
									Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����+1
									Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
									goto RXESC;//�ϱ�����
								}
								i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];
								if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
								{//���س�����ͬ
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//����Fn����
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//�������ݱ�ʶ����
									ReadMeter_DL645_1997_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997���������������Fn���ݻ���
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//���صĳ������ݱ�ʶ����
									ReadMeterNote_OK(PORTn,Pn,Info);//���г���������Ϣ.�ɹ�
									DateRMFlags(Pn,0);//�ճ����־;���:f=0�ɹ�,=1ʧ��
								#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
									if((p8RMLib[0]==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))//�����������
									{
										goto Time;
									}
								#endif
//									switch(p8RMLib[0])
//									{
//										case 129://F129 ��ǰ�����й�����ʾֵ
//										case 162://F162 ���ܱ�����ʱ��
//										case 0xe3://��չ3 ���г����ն��������й�����ʾֵ���ܡ�����1��M��
//									}
									
								}
								break;
						#if (USER/100)==5//�Ϻ��û�
							case 21://�Ϻ���Լ
								x=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//ȡDI
								y=MRR(((u32)p8rxbuff)+8,1);
								if(x!=y)
								{//DI����
									goto RXESC;//�ϱ�����
								}
								i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];
								if((p8rxbuff[9]==i))
								{//���س�����ͬ
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//����Fn����
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//�������ݱ�ʶ����
									ReadMeter_ShH_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//�Ϻ���Լ���������������Fn���ݻ���
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//���صĳ������ݱ�ʶ����
									ReadMeterNote_OK(PORTn,Pn,Info);//���г���������Ϣ.�ɹ�
									DateRMFlags(Pn,0);//�ճ����־;���:f=0�ɹ�,=1ʧ��
								}
								break;
						#endif
							default://DL/T645-2007
								if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
								{//���Ƕ��������
									x=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//ȡDI
								#if (USER/100)==4//�û���ʶ:����
									i=MRR(AddrAFN04F10+18,1);//ȡ���ܱ����
									if((i==1)&&(p8RMLib[0]==0xE3))
									{//�任�ն������ݱ�ʶ
										switch(Terminal_Note->List[ListNo].RMCount)
										{
											default://�ն���ʱ��YYMMDDhhmm
												break;
											case 1://��1���������й�����ʾֵ���ܡ�����1��M��
												x=0x00010000;//��ǰ�����й�����ʾֵ���ܡ�����1��M��
												break;
										}
									}
								#endif
								}
								else
								{//�Ƕ��������
									x=MRR((u32)&p8RMLib[10],4);//ȡDI
								}
								y=MRR(((u32)p8rxbuff)+10,4);
								if(x!=y)
								{//DI����
									goto RXESC;//�ϱ�����
								}
								if(p8rxbuff[9]==4)
								{//���Ժ����������¼���¼������ʱ,ֻ����4�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
									Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
									Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ����+1
									Terminal_Note->List[ListNo].RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
									goto RXESC;//�ϱ�����
								}
								if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
								{//���Ƕ��������
									i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];
								}
								else
								{//�Ƕ��������
									i=p8RMLib[10+4];
								}
								if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
								{//���س�����ͬ
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//����Fn����
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//�������ݱ�ʶ����
									ReadMeter_DL645_2007_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_2007���������������Fn���ݻ���
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//���صĳ������ݱ�ʶ����
	
									Pn=Terminal_Note->List[ListNo].Pn;
									Fn=p8RMLib[0];
									//�����1-6�ฺ������
									if((Fn>=0xe4)&&(Fn<=0xea))
									{
										ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
										Terminal_Note->List[ListNo].RMCountErr=255;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����=���ֵ����δ洢����
										MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
									}
									//75 ��չ13 ��1-24��������������й�����ʾֵ
									if(Fn==0xED)
									{
										d64a=MRR(((u32)p8FnData)+6+1,4);//ȡ����صĶ���ʱ��������ʱ
										d64a<<=8;
										AddCurveCongeal(d64a,101,Pn,((u32)p8FnData)+6+1+4,4);//�����߶��������
										AddCurveCongeal(d64a,103,Pn,((u32)p8FnData)+6+1+4+4,4);//�����߶��������
									#if (USER/100)==11//����ϵ
										//����̨�����������㶳������
										d64a=YMDHM_SubM(d64a,30);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
										AddCurveCongeal(d64a,101,Pn,((u32)p8FnData)+6+1+4,4);//�����߶��������
									#endif
									}
							#if (USER/100)==11//����ϵ
									if(Fn==0xFB)
									{
										d64a=MRR(((u32)p8FnData)+6+1,4);//ȡ���Ķ���ʱ��������ʱ
										d64a<<=8;
										AddCurveCongeal(d64a,89,Pn,((u32)p8FnData)+6+1+4,2);//�����߶��������
										AddCurveCongeal(d64a+0x15,89,Pn,((u32)p8FnData)+6+1+4+2,2);//�����߶��������
										AddCurveCongeal(d64a+0x30,89,Pn,((u32)p8FnData)+6+1+4+2+2,2);//�����߶��������
										AddCurveCongeal(d64a+0x45,89,Pn,((u32)p8FnData)+6+1+4+2+2+2,2);//�����߶��������
									}
									if(Fn==0xFA)
									{
										if(UARTCtrl->RMCount==2)
										{
											i=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
											i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
											AddMonthCongeal(i,35,Pn,((u32)p8FnData)+6+1,93);//���¶��������
										}
									}
							#endif
									
									ReadMeterNote_OK(PORTn,Pn,Info);//���г���������Ϣ.�ɹ�
									DateRMFlags(Pn,0);//�ճ����־;���:f=0�ɹ�,=1ʧ��
							#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
									if((p8RMLib[0]==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))//�����������
									{
									Time:
								 		MR(ADDR_DATABUFF,((u32)p8FnData)+1+6,6);
										MR(ADDR_DATABUFF+6,((u32)p8FnData)+1,6);
										p8=(u8 *)(ADDR_DATABUFF);
										if(p8[0]!=0xee)
										{
											p8[4]&=0x1f;//ȥ�������ֽ��ϵ�����
											p8[10]&=0x1f;//ȥ�������ֽ��ϵ�����
											i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
											if(i==0)
											{//���ܱ�ʱ�ӺϷ�
												i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
												if((i&0x01)==0)
												{
													MWR(i|0x01,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
												}
												i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
												x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
												if(x<3)
												{
													MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
													MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//���ܱ��ʱǰ�����
													MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//���ܱ��ʱ������
													MWR(0xff,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
												}
												if(x==3)
												{
													if(MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1)==0xff)//���ܱ��ʱ��Ľ��
													{
														x=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//���ܱ��ʱǰ�����
														if((x>>15)!=0x0)
														{//��
															x=~x;
															x++;
															MWR(x,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//���ܱ��ʱǰ�����
														}
														if(x>MRR(ADDR_AFN04F298_Ext+7,1))
														{
															MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
															if((i>>31)!=0x0)
															{//��
																i=~i;
																i++;
															}
															MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//���ܱ��ʱ������
															if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
															{//�ɹ�
																MWR(1,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
															}
															else
															{//ʧ��
																MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
															}
															//ERC51
															p8=(u8 *)ADDR_DATABUFF;
															//���ܱ��ʱǰ�����
															MWR(x,ADDR_DATABUFF+9,2);
															//���ܱ��ʱ������
															MWR(i,ADDR_DATABUFF+11,2);
															p8[13]=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);
															ERC51_Event(Pn+(1<<15),ADDR_DATABUFF);//Уʱ��¼�¼�;���:Pn�������(��0��ʼ)
														#if (USER/100)==8//�û���ʶ��ͨ��
															p8=(u8 *)ADDR_TYMDHMS;
															MR((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1),3);
															x=DataComp((u32)ADDR_DATABUFF,(u32)ADDR_TYMDHMS+3,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
															if(x==0)
															{
																x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);
																if(x==0xffffff)
																{
																	DMWR(i,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);//�Ĵ���ֱ��д���洢��(<=8�ֽ�)(�޲���)
																}
															}
															//ʱ�䳬�� 5min ����������
															if(i>300)
															{
																x=MRR(ADDR_AFN0DF321_Ext_Source,2);
																x++;
																MWR(x,ADDR_AFN0DF321_Ext_Source,2);
															}
															//����ʱ����ֵδ����5min����������
															if((i>MRR(ADDR_AFN04F298_Ext+7,1))&&(i<300))
															{
																x=MRR(ADDR_AFN0DF321_Ext_Source+2,2);
																x++;
																MWR(x,ADDR_AFN0DF321_Ext_Source+2,2);
															}
														#endif
														}
														else
														{
															MWR(2,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
														}
														x=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),1);//���ܱ��ʱ����
														if(x<31)
														{
															x++;
														}
														MWR(x,ADDR_DATABUFF,1);
														MR(ADDR_DATABUFF+1,ADDR_AFN0CF250_Ext1+10*(Pn-1),10);
														if(x>=31)
														{
															x=30;
														}
														MR(ADDR_DATABUFF+11,ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1)+1,10*x);
														MR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),ADDR_DATABUFF,1+10*x+10);
													}
												}
											}
										}
									}
							#endif
//									switch(p8RMLib[0])
//									{
//										case 129://F129 ��ǰ�����й�����ʾֵ
//										case 162://F162 ���ܱ�����ʱ��
//										case 0xe3://��չ3 ���г����ն��������й�����ʾֵ���ܡ�����1��M��
//									}
									
								}
								break;
						}
						Terminal_Note->List[ListNo].ReRMCount=0;//�����ط�����=0
						Terminal_Note->List[ListNo].RMCount++;//�������ݱ�ʶ+1
						
						//��ȷ�ϻش��ϱ���������
					RXESC:
						MWR(0xffffffff,((u32)p8txbuff)+128,6);
						Uart_3762_TxData(PORTn,0,1,6);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
						if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
						{
							p8txbuff[3]=0x03;//������;����ʽ·���ز�ͨ��
						}
						else
						{
							if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{
								p8txbuff[3]=0x0a;//������;΢��������ͨ��
							}
							else
							{
								p8txbuff[3]=0x01;//������;����ʽ·���ز�ͨ��
							}
						}
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
						break;
					default:
						UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
						UARTCtrl->Task=5;//���ڽ���
						break;
				}
			}
			break;
	}
}



	#if(USER/100)==0
#define OnlyFnMaxReRMCount     2//1//�����ط�����(���ط�����,�����ʼ1�εķ���);�������ز�=1,΢��������=2
	#else
#define OnlyFnMaxReRMCount     0//�����ط�����(���ط�����,�����ʼ1�εķ���)
	#endif
void RealTimeReadMeter_3762_Fn_Std(u32 PORTn,u8 *p8RMLib)//����1��Fn������д��FnDataBuff;RouterRealTimeTask=0=����(��Fn�������)
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterRealTimeTask)//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
	{
		default:
			Terminal_Router->RouterRealTimeTask=0;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
			break;
		case 0://����
			break;
		case 1://��ͣ·��
			if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				Terminal_Router->StopFlags|=(1<<7);//��ͣ·��
				Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				Terminal_Router->RouterRealTimeTask++;//�ز�Fn>=129��ʵʱ��������
			}
			break;
		case 2://AFN13 F1 ·������ת��:����ز��ӽڵ�
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//����ŵĵ��ܱ�/��������װ��ɾ��
			END_Fn:
				UARTCtrl->Task=0;//
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				Terminal_Router->StopFlags&=~(1<<7);//�ָ�·��
				Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				Terminal_Router->RouterRealTimeTask=0;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
				break;
			}
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
			if((p8RMLib[3]&0x04)==0)//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
			{//��Fn�����ز�����
				goto END_Fn;
			}
			if(p8RMLib[1]==0x0)
			{//��Fn����������=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//�����������=0,��Fn���ݻ���
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(p8RMLib[0]==165)
			{//F165 ���ܱ��ز���������ʱ��
				//�������0-5,�������2��4-5�������¼���¼ʵ��ʱ����
//				dcdu 0x03300000//����ܴ���
//				dcb 3//���س���
//				dcdu 0x03300001//���һ�α�̷���ʱ��
//				dcb 50//���س���
//				dcdu 0x03300E00//���ܱ�β�Ǵ򿪴���
//				dcb 3//���س���
//				dcdu 0x03300E01//���һ��β�Ǵ�ʱ��
//				dcb 60//���س���
//				dcdu 0x03300D00//���ܱ���Ǵ򿪴���(�������¼���¼)
//				dcb 3//���س���
//				dcdu 0x03300D01//���һ�ο���Ǽ�¼(�������¼���¼)
//				dcb 60//���ؤ�
				if(UARTCtrl->RMCount>=4)
				{
					UARTCtrl->RMCount=6;
				}
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//�����������>=Fn�跢������;��Fn�������
				if(p8RMLib[0]==28)
				{//F28 �������״̬�ּ����λ��־
					ReadMeter_SaveFnData(PORTn,p8RMLib[0],Pn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				}
				goto END_Fn;
			}
			
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
			switch(Protocol)
			{
				case 1://DL/T645-1997
					i=MRR((u32)&p8RMLib[10+(3*UARTCtrl->RMCount)],2);//ȡDI
					Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
					//Ԥ��Ӧ���ֽ���
					i=p8RMLib[10+(3*UARTCtrl->RMCount)+2];//���س���
					if(i==0)
					{//���س���=0
						i=10+(3*p8RMLib[1])+UARTCtrl->RMCount;
						i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
					}
					i+=14;
					Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
					if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
					{//΢��������,������ַ�òɼ�����ַ
						d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
						if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
						{
							MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
							Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
						}
					}
					break;
			#if (USER/100)==5//�Ϻ��û�
				case 21://�Ϻ���Լ
					i=MRR((u32)&p8RMLib[10+(2*UARTCtrl->RMCount)],1);//ȡDI
					Read_ShH_TxData(ADDR_DATABUFF,i,d64a);//�Ϻ���Լ������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
					//Ԥ��Ӧ���ֽ���
					i=p8RMLib[10+(2*UARTCtrl->RMCount)+1];//���س���
					i+=14;
					//����Ҫ������Ϻ�Э����ʱ��Ҫ���Լ���ͱ���Ϊ��00��
					Uart_3762_AFN13F1_Tx(PORTn,0,ADDR_DATABUFF,12,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
					MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
					Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
					{//΢��������,������ַ�òɼ�����ַ
						d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
						if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
						{
							MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
							Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
						}
					}
					break;
			#endif
				default://DL/T645-2007
			#if (USER/100)==6//����
					if(p8RMLib[0]==27)//F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ���
					{
						switch(UARTCtrl->RMCount)
						{
							case 0://���ܱ�����ʱ��Hhmmss  r
							case 1://���ܱ�����ʱ��YYMMDDWW r
							case 2://��ع���ʱ�� r
							case 3://����ܴ���  r
							case 4://���һ�α�̷���ʱ��  r
							//case 5://��������ܴ���
							//case 6://���һ�ε�����㷢��ʱ��
							case 7://���������ܴ���  r
							//case 8://���һ���������㷢��ʱ��
							//case 9://�¼������ܴ���
							//case 10://���һ���¼����㷢��ʱ��
							//case 11://Уʱ�ܴ���
							//case 12://���һ��Уʱ����ʱ��
								break;
							default:
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;//���������ط�����
								return;
						}
					}
			#endif
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//���Ƕ��������
						i=MRR((u32)&p8RMLib[10+(5*UARTCtrl->RMCount)],4);//ȡDI
						Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
						//Ԥ��Ӧ���ֽ���
						i=p8RMLib[10+(5*UARTCtrl->RMCount)+4];//���س���
						if(i==0)
						{//���س���=0
							i=10+(5*p8RMLib[1])+UARTCtrl->RMCount;
							i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
						}
						i+=16;
						Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
						if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
						{//΢��������,������ַ�òɼ�����ַ
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
								Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
							}
						}
					}
					else
					{//���������
						i=MRR(((u32)p8RMLib)+10,4);
						d64b=MRR((u32)&UARTCtrl->RMCommandYMDHM[0],5);//���ߵ�ʱ��������ʱ��
						ReadMeter_DL645_2007_TxCurveData(ADDR_DATABUFF,i,d64a,d64b);//DL645-2007��������������֡
						//Ԥ��Ӧ���ֽ���
						i=p8RMLib[10+4];//���س���
						if(i==0)
						{//���س���=0
							i=10+(5*1);
							i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
						}
						i+=16;
						Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,22,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
						if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
						{//΢��������,������ַ�òɼ�����ַ
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
								Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
							}
						}
					}
					break;
			}
			UARTCtrl->TxByte=31+p8txbuff[28];//�����ֽ���(���ֶ���)
			i=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;
			Terminal_Router->RouterRealTimeTask++;//�ز�Fn>=129��ʵʱ��������
			
			//��ʾ�����ַ
			CopyString((u8 *)"�ز�ʵ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[30+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 3://����
			if(UARTCtrl->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(UARTCtrl->Task!=1)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
			#if OnlyFnMaxReRMCount!=0
				if(UARTCtrl->ReRMCount<OnlyFnMaxReRMCount)//485���������ط�����
				{
					UARTCtrl->ReRMCount++;//485���������ط�����
				}
				else
				{
					UARTCtrl->RMCount=200;//�����������,�㳭ʧ�ܲ�����������
					UARTCtrl->ReRMCount=0;//���������ط�����
					ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
				}
			#else
				UARTCtrl->RMCount=200;//�����������,�㳭ʧ�ܲ�����������
				UARTCtrl->ReRMCount=0;//���������ط�����
				ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
			#endif
				UARTCtrl->Task=0;//����1��
				Terminal_Router->RouterRealTimeTask--;//�ز�Fn>=129��ʵʱ��������
				break;
			}
			d64a=MRR(((u32)p8rxbuff)+4,6);
			ReadMeterNote_OK(PORTn,Pn,d64a);//���г���������Ϣ.�ɹ�
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//���ϻش�
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//ȡ���õ�ͨ�ŵ�ַ
//		#if (USER/100)==5//�Ϻ�
//			if(Protocol==21)
//			{//�Ϻ���Լ
//				i=(d64a>>24)&0xff;
//				d64b=bcd_hex(i);//8λѹ��BCD��תΪHEX
//				d64a&=0xffff00ffffff;
//				d64a+=(d64b<<24);
//			}
//		#endif
			y=p8rxbuff[28];//���������򳤶�
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				y=0;//�������ݳ���
			}
			if(y==0)
			{//�������ݳ���=0
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			i=MRR(((u32)p8rxbuff)+25,2);//��·�ϴ�����ʱʱ����
			UARTCtrl->TransferDelay=i;
			MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,y);
			for(i=0;i<y;i++)
			{//ȥ����645֡��ǰ��0xfe
				if(p8rxbuff[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rxbuff,((u32)p8rxbuff)+i,y-i);
					}
					break;
				}
			}
			if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
			{//��ʼ��0x68��
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//���صĵ�ַ
		#if (USER/100)==5//�Ϻ�
			if(Protocol==21)
			{//�Ϻ���Լ
				//ͨ�ŵ�ַA5���ñȽ�
				d64a&=0xffffffffff;
				d64b&=0xffffffffff;
			}
		#endif
			if(d64a!=d64b)
			{//���ַ����ͬ
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			x=0;//��У�����
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//��У���
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
		#if (USER/100)==5//�Ϻ�
			if(Protocol==21)
			{//�Ϻ���Լ
				//DI�Ƚ�
				i=MRR(((u32)p8txbuff)+29+8,1);//���������ݱ�ʶ
				x=MRR(((u32)p8rxbuff)+8,1);//���ص����ݱ�ʶ
				x|=0x80;
			}
			else
			{
				if((p8rxbuff[8]&0x40)!=0x0)
				{//�������վ�ش��쳣
					goto NEXT_RMCount;//�³����������
				}
				//DI�Ƚ�
				i=MRR(((u32)p8txbuff)+29+10,4);//���������ݱ�ʶ
				x=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
				if(Protocol==1)
				{//1997
					i&=0xffff;
					x&=0xffff;
				}
			}
		#else
			if((p8rxbuff[8]&0x40)!=0x0)
			{//�������վ�ش��쳣
				goto NEXT_RMCount;//�³����������
			}
			//DI�Ƚ�
			i=MRR(((u32)p8txbuff)+29+10,4);//���������ݱ�ʶ
			x=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
			if(Protocol==1)
			{//1997
				i&=0xffff;
				x&=0xffff;
			}
		#endif
			if(i!=x)
			{//DI����ͬ
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//�������0x33
				p8rxbuff[10+i]-=0x33;
			}
			switch(Protocol)
			{
				case 1://1997
					p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
					if(p8RMLib[0]!=0x0)
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+2))
						{
							if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
							{//������������0xaa
								if(p8rxbuff[9]==2)
								{//���Ժ����������¼���¼������ʱ,ֻ����2�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
									goto NEXT_RMCount;//�³����������
								}
								UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
								break;
							}
						}
					}
					p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
					ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997���������������Fn���ݻ���
					break;
			#if(USER/100)==5//�Ϻ�
				case 21:
					i=p8RMLib[10+(2*UARTCtrl->RMCount)+1];
					if(p8rxbuff[9]==i)
					{//���س�����ͬ
						ReadMeter_ShH_FnData(PORTn,p8RMLib);//�Ϻ���Լ���������������Fn���ݻ���
					}
					break;
			#endif
				default://2007
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//���Ƕ��������
						p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
					}
					else
					{//�Ƕ��������
						p8RMLib+=10+4;
					}
					if(p8RMLib[0]!=0x0)
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+4))
						{
							if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
							{//������������0xaa
								if(p8rxbuff[9]==4)
								{//���Ժ����������¼���¼������ʱ,ֻ����4�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
									goto NEXT_RMCount;//�³����������
								}
								UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
								break;
							}
						}
					}
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//���Ƕ��������
						p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
					}
					else
					{
						p8RMLib-=10+4;
					}
					ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007���������������Fn���ݻ���				
					break;
			}
	NEXT_RMCount://�³����������
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->Task=0;//����1��
			Terminal_Router->RouterRealTimeTask--;//�ز�Fn>=129��ʵʱ��������
			break;
	}
}

/*
void KeyUserReadMeter_3762_Std(u32 PORTn)//�ز�ģ��,�ص㻧����
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Fn;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u32 ClassNo;
	u8 * p8RMLib;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterKeyUserTask)//�ص㻧��������
	{
		case 0://����Ƿ����ص㻧��������ͣ·��
			if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{	
				y=MRR(ADDR_AFN04F35,1);//̨�����г����ص㻧����
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				while(1)
				{
					if(UARTCtrl->KeyUserRMCount>=y)//�ص㻧�������ܱ����
					{//ȫ���ص㻧����
						Terminal_Router->RouterKeyUserTask=0;//�ص㻧��������
						UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfKeyUser;//485������ܱ�/��������װ�����(���ֶ���)(ԭ���泭���װ����żĴ�)
						UARTCtrl->FnEnd=UARTCtrl->FnEndBackupOfKeyUser;//485����Fn����������־,0=����,1=û����(ԭ���泭���Fn����������־�Ĵ�)
						UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485����Fn����(ԭ���泭���Fn�����Ĵ�)
						UARTCtrl->RMError=UARTCtrl->RMErrorBackupOfKeyUser;//485�������ܱ�ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��(ԭ���泭���ʧ�ܱ�־�Ĵ�)
						UARTCtrl->KeyUserRMCount=0;//�ص㻧�������ܱ����
						UARTCtrl->RMKeyUser=0;//�����ص㻧��־,0=���泭��,1=�ص㻧����
						return;
					}
					x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//�ص㻧�ĵ��ܱ�/��������װ�����
					if((x==0)||(x>AFN04F10MaxPn))
					{//װ����Ŵ�
				NEXT_KeyUser_Start:
						UARTCtrl->FnCount=0;//Fn����
						UARTCtrl->RMCount=0;//485�����������
						UARTCtrl->ReRMCount=0;//485���������ط�����
						UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
						continue;
					}
					UARTCtrl->MeterNo=x;
					x--;
					x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
					Pn=MRR(x,2);
					if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
					{//������Ŵ�
						goto NEXT_KeyUser_Start;//��һ���ص㻧
					}
					i=MRR(x+2,1);
					i&=0x1f;
					if(i!=31)
					{//�˿ڲ����ز�
						goto NEXT_KeyUser_Start;//��һ���ص㻧
					}
					Protocol=MRR(x+3,1);//ͨ��Э������ BIN 1 
					if(Protocol!=1)
					{//����DL/T645-1997,2007�����㶳��
						goto NEXT_KeyUser_Start;//��һ���ص㻧
					}
					break;
				}
				UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				Terminal_Router->StopFlags|=(1<<1);//��ͣ·��:b1=���ص㻧
				Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				Terminal_Router->RouterKeyUserTask++;//�ص㻧��������
			}
			break;
		case 1://AFN13 F1 ·������ת��:����ز��ӽڵ�
			i=MRR(ADDR_AFN04F35,1);//̨�����г����ص㻧����
			if(i>MaxKeyUser)
			{
				i=MaxKeyUser;
			}
			if(UARTCtrl->KeyUserRMCount>=i)//�ص㻧�������ܱ����
			{//ȫ���ص㻧����
				Terminal_Router->RouterKeyUserTask=0;//�ص㻧��������
//				UARTCtrl->KeyUserRMOKCountLast=UARTCtrl->KeyUserRMOKCount;//�ϴ��ص㻧�����ɹ�����
//				UARTCtrl->KeyUserRMOKCount=0;//��ǰ�ص㻧�����ɹ�����
				UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfKeyUser;//485������ܱ�/��������װ�����(���ֶ���)(ԭ���泭���װ����żĴ�)
				UARTCtrl->FnEnd=UARTCtrl->FnEndBackupOfKeyUser;//485����Fn����������־,0=����,1=û����(ԭ���泭���Fn����������־�Ĵ�)
				UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485����Fn����(ԭ���泭���Fn�����Ĵ�)
				UARTCtrl->RMError=UARTCtrl->RMErrorBackupOfKeyUser;//485�������ܱ�ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��(ԭ���泭���ʧ�ܱ�־�Ĵ�)
				UARTCtrl->RMCount=0;//485�����������
				UARTCtrl->ReRMCount=0;//485���������ط�����
				UARTCtrl->KeyUserRMCount=0;//�ص㻧�������ܱ����
				UARTCtrl->RMKeyUser=0;//�����ص㻧��־,0=���泭��,1=�ص㻧����
				
				Terminal_Router->StopFlags&=~(1<<1);//�ָ�·��
				Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				return;
			}
			x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//�ص㻧�ĵ��ܱ�/��������װ�����
			if((x==0)||(x>AFN04F10MaxPn))
			{//װ����Ŵ�
			NEXT_KeyUser:
				UARTCtrl->FnCount=0;//Fn����
				UARTCtrl->RMCount=0;//485�����������
				UARTCtrl->ReRMCount=0;//485���������ط�����
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				break;
			}
			UARTCtrl->MeterNo=x;
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MRR(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//������Ŵ�
				goto NEXT_KeyUser;//��һ���ص㻧
			}
			i=MRR(x+2,1);
			i&=0x1f;
			if(i!=31)
			{//�˿ڲ����ز�
				goto NEXT_KeyUser;//��һ���ص㻧
			}
			Protocol=MRR(x+3,1);//ͨ��Э������ BIN 1 
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[UARTCtrl->FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[UARTCtrl->FnCount];
			}
			if(i==0)
			{//��Fn�ǿ�
			NEXT_Fn:
				UARTCtrl->FnCount+=1;//Fn����+1
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->RMError&=0xfe;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				if(Protocol==1)
				{//DL/T645-1997
					i=Get_RM_DL645_1997_MaxFn();
				}
				else
				{//DL/T645-2007
					i=Get_RM_DL645_2007_MaxFn();
				}
				if(UARTCtrl->FnCount>=i)
				{//Fn����>=���Fn����,���������
			//END_Fn:
					UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
					if(UARTCtrl->RMError==0)//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
					{
//						UARTCtrl->KeyUserRMOKCount++;//��ǰ�ص㻧�����ɹ�����
					}
					UARTCtrl->RMError=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
				}
				break;
			}
			p8RMLib=(u8 *)i;//�����
			if((p8RMLib[3]&0x04)==0)//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
			{//��Fn�����ز�����
				goto NEXT_Fn;
			}
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
			Fn=p8RMLib[0];
			y=0;
			for(i=0;i<Get_KeyUserReadMeterLib_MaxFn();i++)
			{
				if(KeyUserReadMeterLib[i].AFN0CFn==Fn)//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
				{
					x=KeyUserReadMeterLib[i].AFN0DFn;//�ص㻧����Fn
					y|=Class2Data_Fn(ClassNo,x);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
			}
			if(y==0x0)
			{//��Fn����
				goto NEXT_Fn;
			}
			i=GetClass1DataAddr(Fn,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			if(i==0)
			{//û�ҵ�
				goto NEXT_Fn;
			}
			i=MRR(i+1,4);//ȡԭ����ʱ��������ʱ
			x=MRR(ADDR_TYMDHMS+2,4);//ȡ��ǰ����ʱ��������ʱ
			if(i==x)
			{//�ѳ�
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//�����������=0,��Fn���ݻ���
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//�����������>=Fn�跢������;��Fn�������
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0	
				goto NEXT_Fn;	
			}
			if(UARTCtrl->RMError!=0x0)
			{//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				goto NEXT_Fn;
			}
			
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
			if(Protocol==1)
			{//dl645-1997
				i=MRR((u32)&p8RMLib[10+(3*UARTCtrl->RMCount)],2);//ȡDI
				Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
				//Ԥ��Ӧ���ֽ���
				i=p8RMLib[10+(3*UARTCtrl->RMCount)+2];//���س���
				if(i==0)
				{//���س���=0
					i=10+(3*p8RMLib[1])+UARTCtrl->RMCount;
					i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
				}
				i+=14;
				Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			}
			else
			{//dl645-2007
				goto NEXT_KeyUser;//��һ���ص㻧,dl645-2007�����㶳��

//				i=MRR((u32)&p8RMLib[10+(5*UARTCtrl->RMCount)],4);//ȡDI
//				Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
//				//Ԥ��Ӧ���ֽ���
//				i=p8RMLib[10+(5*UARTCtrl->RMCount)+4];//���س���
//				if(i==0)
//				{//���س���=0
//					i=10+(5*p8RMLib[1])+UARTCtrl->RMCount;
//					i=p8RMLib[i];//ȡԤ�Ʒ����ֽ���
//				}
//				i+=16;
//				Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���

			}		
			UARTCtrl->TxByte=31+p8txbuff[28];//�����ֽ���(���ֶ���)
			i=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;
			Terminal_Router->RouterKeyUserTask++;//�ص㻧��������
			//��ʾ�����ַ
			CopyString((u8 *)"�ز��ص�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[16+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 2://����
			if(UARTCtrl->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task!=1)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
				if(UARTCtrl->ReRMCount<1)//485���������ط�����
				{
					UARTCtrl->ReRMCount++;//485���������ط�����
				}
				else
				{
					UARTCtrl->RMError|=0x1;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				}
				Terminal_Router->RouterKeyUserTask--;//�ص㻧��������
				break;
			}
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//���ϻش�
		FnErr:
				UARTCtrl->Task=0;//����1��
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				Terminal_Router->RouterKeyUserTask--;//�ص㻧��������
				UARTCtrl->FnCount=0;//Fn����
				UARTCtrl->RMCount=0;//485�����������
				UARTCtrl->ReRMCount=0;//485���������ط�����
				break;
			}
			x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//�ص㻧�ĵ��ܱ�/��������װ�����
			if((x==0)||(x>AFN04F10MaxPn))
			{//װ����Ŵ�
				goto FnErr;
			}
			UARTCtrl->MeterNo=x;
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MRR(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//������Ŵ�
				goto FnErr;
			}
			Protocol=MRR(x+3,1);//ͨ��Э������ BIN 1 
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[UARTCtrl->FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[UARTCtrl->FnCount];
			}
			if(i==0)
			{//��Fn�ǿ�
				goto FnErr;
			}
			p8RMLib=(u8 *)i;//�����
			d64a=MRR(x+4,6);//ȡ���õ�ͨ�ŵ�ַ
			y=p8rxbuff[28];//���������򳤶�
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				y=0;//�������ݳ���
			}
			if(y==0)
			{//�������ݳ���=0
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,y);

			if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
			{//��ʼ��0x68��
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//���صĵ�ַ
			if(d64a!=d64b)
			{//���ַ����ͬ
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			x=0;//��У�����
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//��У���
				UARTCtrl->Task=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//�������վ�ش��쳣
				goto NEXT_RMCount;//�³����������
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//�������0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//1997
				p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
				if(p8RMLib[0]!=0x0)
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+2))
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
						{//������������0xaa
							if(p8rxbuff[9]==2)
							{//���Ժ����������¼���¼������ʱ,ֻ����2�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
								goto NEXT_RMCount;//�³����������
							}
							UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
							break;
						}
					}
				}
				p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
				ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997���������������Fn���ݻ���
			}
			else
			{//2007
				p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
				if(p8RMLib[0]!=0x0)
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+4))
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
						{//������������0xaa
							if(p8rxbuff[9]==4)
							{//���Ժ����������¼���¼������ʱ,ֻ����4�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
								goto NEXT_RMCount;//�³����������
							}
							UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
							break;
						}
					}
				}
				p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
				ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007���������������Fn���ݻ���
			}
	NEXT_RMCount://�³����������
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->Task=0;//����1��
			Terminal_Router->RouterKeyUserTask--;//�ص㻧��������
			break;
	}
}
*/



#endif

//0x68
//�ܳ���L
//�ܳ���L
//������
//��Ϣ��R-1
//��Ϣ��R-2
//��Ϣ��R-3
//��Ϣ��R-4
//��Ϣ��R-5
//��Ϣ��R-6
//��ַ��A1,A2,A3
//Ӧ�ù�����AFN
//���ݵ�Ԫ��ʶDT1
//���ݵ�Ԫ��ʶDT2
//���ݵ�Ԫ
//CS
//0x16


