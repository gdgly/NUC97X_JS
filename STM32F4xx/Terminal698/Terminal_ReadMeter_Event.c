
//���ּ�1-3�������ܱ��¼�
#include "Project.h"
#include "Terminal_ReadMeter_Event.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_ReadMeter_RS485.h"
#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_AFN0E_Event.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"


#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����

#if (USER/100)==5//�Ϻ�;���ܱ��¼��ɼ���������
void RMEveneTimer_ShH(u32 PORTn,u32 SuccessRate)//�Ϻ��������ּ������ܱ��¼���ʱ
{
	//RS485�ڵ���������Terminal_ReadMeter_RS485.c��ִ��
	//�ز��ڵ��������ڴ�
	//���ݵȼ����ȼ�1Ĭ��ʵʱ�ɼ������ڸ���ʵʱ����ȷ�����ȼ�2Ĭ��1�죬����ϲɼ����̣��ȼ�3Ĭ��1���£�����ϲɼ����̡�
	UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	u32 x;
	u64 YMDH1;
	u64 YMDH2;
	
	if(PORTn==RS485_4PORT)
	{//RS485_4ͨ�ſں�(�ز�)
		UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
		if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
		{//��ѭ��
			UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
			//�����ּ��¼���ʱ
			YMDH1=MRR(ADDR_TYMDHMS+2,4);
			YMDH2=MRR(((u32)&UARTRMEvent->YMDHMS)+2,4);
			if(YMDH1<YMDH2)
			{//ʱ�ӵ���
				MC(0,((u32)&UARTRMEvent->YMDHMS),24);//(ȫ0��ʾ��Ч)
			}
			else
			{
				if(YMDH1!=YMDH2)
				{//ʱ����
					MWR(YMDH1,((u32)&UARTRMEvent->YMDHMS)+2,4);//�ϴ����ڶ�ʱ�����������ʱ����(ȫ0��ʾ��Ч)
					YMDH2=MRR(((u32)&UARTRMEvent->YMDHMS)+6+2,4);//�ּ�1�¼��´ζ�ʱ��ʼ��������ʱ����(ȫ0��ʾ��Ч)
					//����ʱ�����
					//1���¼�Ĭ��ʵʱ�ɼ��������������ڲɼ�
					x=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);
					YMDH2=YMDH_AddH(YMDH2,x);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
					if(YMDH1>=YMDH2)
					{//��ʱ��
						//�´ζ�ʱ��ʼ��������ʱ����(ȫ0��ʾ��Ч)
						MWR(YMDH1,((u32)&UARTRMEvent->YMDHMS)+6+2,4);
						//������
						//ÿ���������ڳ�1���¼�
						UARTCtrl->RMEventTaskN|=1;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
					}
				}
			}
			//ÿ���1�ֳ���,��2���¼�
			if((UARTCtrl->RMEventTaskN&0x20)!=0)
			{
				UARTCtrl->RMEventTaskN&=~0x20;
				UARTCtrl->RMEventTaskN|=2;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3,;b6=�¸���(�Ϻ�ÿ�³��¼���)
			}
			//ÿ�³�3���¼�
			if((UARTCtrl->RMEventTaskN&0x40)!=0)
			{//�¸���
				//���ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����20150122.doc
				//����ĳ�ղɼ��ɹ���>98%�����ڳ���15����ʱ�䳬��12:00�ſ�ʼ�ɼ�
				if(SuccessRate!=0)
				{//����ɹ���<100%
					x=Comm_Ram->TYMDHMS[3];//ʵʱ�ӵ���
					if(x>0x15)
					{
						UARTCtrl->RMEventTaskN&=~0x40;
						UARTCtrl->RMEventTaskN|=4;
					}
				}
				else
				{//����ɹ���100%
					UARTCtrl->RMEventTaskN&=~0x40;
					UARTCtrl->RMEventTaskN|=4;
				}
			}
		}
	}
}
#endif

void RMEveneTimer(u32 PORTn)//�ּ������ܱ��¼���ʱ
{
#if (USER/100)==5//�Ϻ�;���ܱ��¼��ɼ���������
	//RS485�ڵ���������Terminal_ReadMeter_RS485.c��ִ��
	//�ز��ڵ�������,�ݲ����Ժ�����ʱ���������溯��������
#else
	u32 i;
	u32 x;
	u32 t;
	u64 YMDHM1;
	u64 YMDHM2;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	//�����ּ��¼���ʱ
	YMDHM1=MRR(ADDR_TYMDHMS+1,5);
	YMDHM2=MRR(((u32)&UARTRMEvent->YMDHMS)+1,5);
	if(YMDHM1<YMDHM2)
	{//ʱ�ӵ���
		MC(0,((u32)&UARTRMEvent->YMDHMS),24);//(ȫ0��ʾ��Ч)
	}
	else
	{
		if(YMDHM1!=YMDHM2)
		{//�ָ���
			MWR(YMDHM1,((u32)&UARTRMEvent->YMDHMS)+1,5);
			for(i=0;i<3;i++)
			{
				YMDHM2=MRR(((u32)&UARTRMEvent->YMDHMS)+6+(6*i)+1,5);
				if(YMDHM2==0)
				{//��Ч
					MWR(YMDHM1,((u32)&UARTRMEvent->YMDHMS)+6+(6*i)+1,5);
					continue;
				}
				//����ʱ�����
				x=MRR(ADDR_AFN04F107+(2*i),2);
				t=x&0xff;//������ֵ
				if(t==0)
				{//��Ч
					continue;
				}
				x>>=8;//���ڵ�λ
				switch(x)
				{
					default://��Ч
						continue;
					case 1://��
						YMDHM2=YMDHM_AddM(YMDHM2,t);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						break;
					case 2://ʱ
						YMDHM2>>=8;//��Ϊ0��
						YMDHM2=YMDH_AddH(YMDHM2,t);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
						YMDHM2<<=8;
						break;
					case 3://��
						YMDHM2>>=16;//��Ϊ0ʱ0��
						YMDHM2<<=16;
						YMDHM2=YMDHM_AddM(YMDHM2,t*60*24);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						break;
					case 4://��
						YMDHM2>>=24;//��Ϊ0��0ʱ0��
						while(t!=0)
						{
							YMDHM2=YM_Add1M(YMDHM2);//���¼�1��,���ؼ�1�������
							t--;
						}
						YMDHM2<<=24;//��Ϊ01��0ʱ0��
						YMDHM2|=0x010000;
						break;
				}
				if(YMDHM1<YMDHM2)
				{//��ʱû��
					continue;
				}
				//��ʱ��
				//�´ζ�ʱ��ʼ��������ʱ����(ȫ0��ʾ��Ч)
				MWR(YMDHM1,((u32)&UARTRMEvent->YMDHMS)+6+(6*i)+1,5);
				//������
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
				UARTCtrl->RMEventTaskN|=1<<i;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
			}
		}
	}
#endif
}


u32 NewRMEvene(u32 PORTn)//�»�ָ������ܱ��¼�����;����0=û�½���,1=�½���
{
	u32 i;
	u32 x;
	u32 y;
  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	
	i=Check_Meter_Event_Save();//�����ܱ��¼������ռ�;����0=�洢�M(û�ϱ�),1-255�����ɴ洢������
	if(i<30)
	{//�ռ�ɴ�<30��,ÿ��10��
		StoreRMEvene(PORTn);//�ݴ�����ܱ��¼�
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	if(UARTCtrl->RMDI!=0xffffffff)//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
	{//��ǰ���ڳ�������1����������Ķ������,���л�,����������û�Ĵ�
		return 0;
	}
	if(UARTRMEvent->TaskStore!=0)//�ݴ�����ܱ��¼���־:0=û,1=��
	{//���ݴ�
		//���Ǳ��浱ǰ���񵽵�һ���¼�
		UARTRMEvent->RMEventStack[0].MeterNo=UARTCtrl->MeterNo;
		UARTRMEvent->RMEventStack[0].FnCount=UARTCtrl->FnCount;
		UARTRMEvent->RMEventStack[0].RMCount=UARTCtrl->RMCount;
		UARTRMEvent->RMEventStack[0].RMTaskC=UARTCtrl->RMTaskC;
	#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		if(PORTn==RS485_4PORT)//(�ز�)
		{
			x=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			for(i=0;i<x;i++)
			{
				Terminal_Note->List[i].FnCountSOfMeterEvent=Terminal_Note->List[i].FnCount;//���붨ʱ�������ܱ��¼�ǰԭ�����Fn����
			}
		}
	#endif
		//�ָ��¼�����
		UARTCtrl->MeterNo=UARTRMEvent->RMEventStack[3].MeterNo;
		if(UARTCtrl->MeterNo!=0)
		{
			UARTCtrl->MeterNo--;
		}
		UARTCtrl->FnCount=UARTRMEvent->RMEventStack[3].FnCount;
		UARTCtrl->RMCount=UARTRMEvent->RMEventStack[3].RMCount;
		UARTCtrl->RMEventTaskC=UARTRMEvent->RMEventStack[3].RMEventTaskC;
	#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		if(PORTn==RS485_4PORT)//(�ز�)
		{
			for(i=0;i<x;i++)
			{
				Terminal_Note->List[i].FnCount=0;//����Fn����
				Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
				Terminal_Note->List[i].ReRMCount=0;//�ط�����
				Terminal_Note->List[i].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
			}
		}
	#endif
		UARTRMEvent->TaskStore=0;//�ݴ�����ܱ��¼���־:0=û,1=��
		
		UARTCtrl->RMTaskC=MaxRMTask-2;//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=ͣ���¼����,4=ÿСʱ��������,5=,(MaxRMTask-2)=���ڳ������ܱ��¼�,(MaxRMTask-1)=ѭ������(��ÿСʱ����������,ֻ��ѭ������ſɲ�������ȼ�����)
		if(PORTn==RS485_4PORT)
		{//�ز�
		#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			ClrRMTaskComplete_3762(MaxRMTask-2,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
		#endif
		}
		else
		{//RS485
			ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-2,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
		}
		UARTCtrl->FnCount=0;//����Fn����
		UARTCtrl->ReRMCount=0;//485���������ط�����
		UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
		UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
		UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
		
		UARTCtrl->RMCount&=0xfffffffe;//�ӳ�������ʼ
		UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
		UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
		return 1;
	}
	
	y=UARTCtrl->RMEventTaskN&0x07;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
	if(y)
	{
		x=BitLoca(y);//�����1����λΪ1��λ��(0-32)
		if((UARTCtrl->RMEventTaskC==0)||(x<UARTCtrl->RMEventTaskC))
		{//��ǰ�¼�����=��,�����¼����ȼ����ڵ�ǰ����ִ�е��¼�
			i=UARTRMEvent->StackCount;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
			if(i<3)//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
			{
				y&=~(1<<(x-1));//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
			#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
				//���ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����20150122.doc
				//����ĳ�ղɼ��ɹ���>98%�����ڳ���15����ʱ�䳬��12:00�ſ�ʼ�ɼ�
				//�ʿ��ܲ���ÿ�µ�1�ֳ����ȥ��3���¼���3���¼�����δ��ʱ�����־UARTCtrl->RMEventTaskN
				if((UARTCtrl->RMEventTaskN&0x40)!=0)
				{//�¸���
					if(y!=4)
					{
						UARTCtrl->RMEventTaskN=y;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
						UARTCtrl->RMEventTaskN|=0x40;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3;b6=�¸���(�Ϻ�ÿ�³��¼���)
					}
				}
				else
				{
					UARTCtrl->RMEventTaskN=y;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
				}
			#else
				UARTCtrl->RMEventTaskN=y;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3
			#endif
				
				UARTRMEvent->RMEventStack[i].MeterNo=UARTCtrl->MeterNo;
				UARTRMEvent->RMEventStack[i].FnCount=UARTCtrl->FnCount;
				UARTRMEvent->RMEventStack[i].RMCount=UARTCtrl->RMCount;
				UARTRMEvent->RMEventStack[i].RMTaskC=UARTCtrl->RMTaskC;
				UARTRMEvent->RMEventStack[i].RMEventTaskC=UARTCtrl->RMEventTaskC;

				i++;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
				UARTRMEvent->StackCount=i;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
				UARTCtrl->RMEventTaskC=x;//��ǰ�������ܱ��¼�����:0=��,1-3=1-3��				
			#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				if(PORTn==RS485_4PORT)//(�ز�)
				{
					if(i==1)
					{
						x=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
						if(x>MaxRS485AddCarrierPn)
						{
							x=MaxRS485AddCarrierPn;
						}
						for(i=0;i<x;i++)
						{
							Terminal_Note->List[i].FnCountSOfMeterEvent=Terminal_Note->List[i].FnCount;//���붨ʱ�������ܱ��¼�ǰԭ�����Fn����
							Terminal_Note->List[i].FnCount=0;//��0ֵ������F3ֵ�ǿ��ǵ��ܽ��������ϱ�
							Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
							Terminal_Note->List[i].ReRMCount=0;//�ط�����
							Terminal_Note->List[i].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
							Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
						}
					}
				}
			#endif

				UARTCtrl->RMTaskC=MaxRMTask-2;//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=ͣ���¼����,4=ÿСʱ��������,5=,(MaxRMTask-2)=���ڳ������ܱ��¼�,(MaxRMTask-1)=ѭ������(��ÿСʱ����������,ֻ��ѭ������ſɲ�������ȼ�����)
				if(PORTn==RS485_4PORT)
				{//�ز�
				#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					ClrRMTaskComplete_3762(MaxRMTask-2,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
				#endif
				}
				else
				{//RS485
					ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-2,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
				}
				UARTCtrl->MeterNo=0;
				UARTCtrl->FnCount=0;//��0ֵ������3(F3)ֵ�ǿ��ǵ��ܽ��������ϱ�
				UARTCtrl->RMCount=0;
				UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
				UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
				
				UARTCtrl->ReRMCount=0;//485���������ط�����
				UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
				UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
				return 1;
			}
		}
	}
	return 0;
}

void DelRMEvene(u32 PORTn)//ɾ�������ܱ��¼�����
{
	u32 i;
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	u32 x;
#endif

  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	if(UARTCtrl->RMTaskC==(MaxRMTask-2))//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=ͣ���¼����,4=ÿСʱ��������,5=,(MaxRMTask-2)=���ڳ������ܱ��¼�,(MaxRMTask-1)=ѭ������(��ÿСʱ����������,ֻ��ѭ������ſɲ�������ȼ�����)
	{
		i=UARTRMEvent->StackCount;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
		if(i!=0)
		{
			i--;
			UARTRMEvent->StackCount=i;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
			
			UARTCtrl->MeterNo=UARTRMEvent->RMEventStack[i].MeterNo;
			if(UARTCtrl->MeterNo!=0)
			{
				UARTCtrl->MeterNo--;
			}
			UARTCtrl->FnCount=UARTRMEvent->RMEventStack[i].FnCount;
			UARTCtrl->RMCount=UARTRMEvent->RMEventStack[i].RMCount;
			UARTCtrl->RMTaskC=UARTRMEvent->RMEventStack[i].RMTaskC;
			UARTCtrl->RMEventTaskC=UARTRMEvent->RMEventStack[i].RMEventTaskC;
			
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
			UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
			if(UARTCtrl->RMTaskC==(MaxRMTask-2))
			{//�¼�����
				UARTCtrl->RMCount&=0xfffffffe;//�ӳ�������ʼ
				UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
				UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
			}
			else
			{//���¼�����
				UARTCtrl->RMCount=0;
			}
		#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(PORTn==RS485_4PORT)//(�ز�)
			{
				x=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
				if(x>MaxRS485AddCarrierPn)
				{
					x=MaxRS485AddCarrierPn;
				}
				for(i=0;i<x;i++)
				{
					Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
					Terminal_Note->List[i].ReRMCount=0;//�ط�����
					Terminal_Note->List[i].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
					Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
				}
				if(UARTRMEvent->StackCount==0)
				{//ջ����=0,�˳������¼�
					for(i=0;i<x;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfMeterEvent;//���붨ʱ�������ܱ��¼�ǰԭ�����Fn����
					}
				}
				else
				{//�����ϼ��¼�
					for(i=0;i<x;i++)
					{
						Terminal_Note->List[i].FnCount=0;//��0ֵ������3(F3)ֵ�ǿ��ǵ��ܽ��������ϱ����¼�ǰԭ�����Fn����
					}
				}
			}
		#endif
		}
		else
		{
			UARTCtrl->RMTaskC=MaxRMTask-1;
			UARTCtrl->RMEventTaskC=0;
		}
	}
}

void StoreRMEvene(u32 PORTn)//�ݴ����ڶ����ܱ��¼�������
{
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	u32 i;
	u32 x;
#endif

  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	if(UARTCtrl->RMTaskC==(MaxRMTask-2))
	{//��ǰ�����Ƕ����ܱ��¼�
		UARTRMEvent->TaskStore=1;//�ݴ�����ܱ��¼���־:0=û,1=��
		//���¼�
		UARTRMEvent->RMEventStack[3].MeterNo=UARTCtrl->MeterNo;
		UARTRMEvent->RMEventStack[3].FnCount=UARTCtrl->FnCount;
		UARTRMEvent->RMEventStack[3].RMCount=UARTCtrl->RMCount;
		UARTRMEvent->RMEventStack[3].RMTaskC=UARTCtrl->RMTaskC;
		UARTRMEvent->RMEventStack[3].RMEventTaskC=UARTCtrl->RMEventTaskC;
		//���ǻָ�����1���¼���������ǰ����
		UARTCtrl->MeterNo=UARTRMEvent->RMEventStack[0].MeterNo;
		if(UARTCtrl->MeterNo!=0)
		{
			UARTCtrl->MeterNo--;
		}
		UARTCtrl->FnCount=UARTRMEvent->RMEventStack[0].FnCount;
		UARTCtrl->RMCount=0;
		UARTCtrl->RMTaskC=UARTRMEvent->RMEventStack[0].RMTaskC;
		UARTCtrl->ReRMCount=0;//485���������ط�����
		UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
		UARTCtrl->RMEventTaskC=0;//��ǰ�������ܱ��¼�����:0=��,1-3=1-3��
	#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		if(PORTn==RS485_4PORT)//(�ز�)
		{
			x=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			for(i=0;i<x;i++)
			{
				Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfMeterEvent;//���붨ʱ�������ܱ��¼�ǰԭ�����Fn����
				Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
				Terminal_Note->List[i].ReRMCount=0;//�ط�����
				Terminal_Note->List[i].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
			}
		}
	#endif	
		UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
		UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
	}
}

void ClrRMEvene(u32 PORTn)//������ܱ��¼�
{
  UARTCtrl_TypeDef * UARTCtrl;
	UARTRMEvent_TypeDef * UARTRMEvent;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTRMEvent=(UARTRMEvent_TypeDef *)Get_ADDR_UARTnRMEvent(PORTn);
	
	UARTCtrl->RMEventTaskN=0;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3,;b6=�¸���(�Ϻ�ÿ�³��¼���)
	UARTCtrl->RMEventTaskC=0;//��ǰ�¼�����
	MC(0,(u32)&UARTRMEvent->YMDHMS,24);//�嶨ʱ(ȫ0��ʾ��Ч)
	UARTRMEvent->TaskStore=0;//�ݴ�����ܱ��¼���־:0=û,1=��
	UARTRMEvent->StackCount=0;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
}



#endif




















