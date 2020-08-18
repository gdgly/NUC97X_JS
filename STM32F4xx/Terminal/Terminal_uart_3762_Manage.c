
#include "Project.h"
#include "Terminal_Uart_3762_Manage.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "Terminal_ReadMeter_3762.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_ReadMeter_3762_Std.h"
#include "AutoSearchMeter_3762.h"
#include "BatchTask_3762.h"
#include "KeyUser.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_Event.h"
#include "RouterBroadcastTime_Collector.h"



#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void Terminal_AFN0CF11_3762(void)//�ն���1����F11,�ն˼��г���״̬��Ϣ
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u8 *p8;

#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
	i=MRR(ADDR_TYMDHMS+1,2);
	if(i<RMM_StartHM)
	{
		return;
	}
#endif
	KeyUserPnList();//���г����ص㻧���߶���Pn�б�
	n=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	x=0;//�ɹ���
	y=0;//ʧ����
	w=0;//�ص㻧�ɹ���
	for(i=0;i<n;i++)
	{
	#if DateRMTask==0//ÿ�ճ�������;0=û��,1=��
		z=Terminal_Note->List[i].RMCompleteS[MaxRMTask-1];//��ѭ�������
	#else
		z=Terminal_Note->List[i].RMCompleteS[1];//ֻ�������1
	#endif
		if(z==2)
		{
			x++;//�ɹ���
			if(KeyUserPnSet(Terminal_Note->List[i].Pn))//�ص㻧��Ч������ż��;����0=��Ч,1=��Ч;ʹ���ص㻧���߶���Pn�б�ADDR_KeyUserPnList
			{
				w++;//�ص㻧����ɹ���
			}
		}
		else
		{
			if(z!=0)
			{
				y++;//ʧ����
			}
		}
	}

	CopyString((u8 *)"�ز�____��____��____",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
	//����
	p8=(u8*)(ADDR_DATABUFF+4+3);
	p8[0]='0';
	i=hex_bcd(n);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	//�ɹ���
	p8=(u8*)(ADDR_DATABUFF+10+3);
	p8[0]='0';
	i=hex_bcd(x);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}	
	//ʧ����
	p8=(u8*)(ADDR_DATABUFF+16+3);
	p8[0]='0';
	p8[1]=0;
	i=hex_bcd(y);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	WarningOccur(100,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
	
	p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
	p8[0]=3;//�������ݿ����n	BIN	1(1-31) 
	p8[1]=2;//�˿ں�2
	p8[19+1]=3;//�˿ں�3
	p8[38+1]=31;//�ն�ͨ�Ŷ˿ں�	BIN	1
	p8+=(2*19);
	MWR(n,((u32)p8)+2,2);//Ҫ���������	BIN	2
	if((x==n)&&(n!=0))
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
	MWR(x,((u32)p8)+5,2);//����ɹ�����	BIN	2
	MWR(w,((u32)p8)+7,1);//���ص��ɹ�����	BIN	1

	//��ͣ·�ɱ�־:
	//b0=��������ʱ,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=,b18=,b19=,b20=,b21=,b22=,b23=
	if(((Terminal_Router->StopFlags&0xFFFFFF7D)==0)&&(Terminal_Router->RouterInitTask>=200)&&(Terminal_Router->NoteNum!=0))
	{
		p8[4]|=1;//D0=1,����
		//����ʼʱ��	����¼A.1	6
		if(p8[8+3]==0xee)
		{//�޿�ʼʱ��
			MR(((u32)p8)+8,ADDR_AFN0CF2,6);
		}
	}
	else
	{
		p8[4]&=0xfe;//D0=0,δ����
	}
	//�������ʱ��	����¼A.1	6
	
/*	
	//��ʾ�����ŵ�
	if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{
		CopyString((u8 *)"�ŵ���------���书��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		p8=(u8*)(ADDR_DATABUFF);
		p8[20]=0;
		i=Terminal_Router->RadioChannel;
		i=hex_bcd(i);
		p8[6]=((i>>4)&0xf)+0x30;
		p8[7]=(i&0xf)+0x30;
		i=Terminal_Router->RadioPower;//�������ڵ㷢�书��:00����߷��书�ʣ�01���θ߷��书�ʣ�02���εͷ��书�ʣ�03����ͷ��书�ʣ�04��254��������255�����ֲ���
		switch(i)
		{
			case 0://��߷��书��
				CopyString((u8 *)"���",(u8*)ADDR_DATABUFF+8);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 1://�θ߷��书��
				CopyString((u8 *)"�θ�",(u8*)ADDR_DATABUFF+8);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 2://�εͷ��书��
				CopyString((u8 *)"�ε�",(u8*)ADDR_DATABUFF+8);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 3://��ͷ��书��
				CopyString((u8 *)"���",(u8*)ADDR_DATABUFF+8);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
		}
		WarningOccur(101,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
	}
*/
}


u32 NoRMCompletes_3762(u32 RMTask)//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	
	n=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
	if(RMTask!=0)
	{//��������
		if(n==0)
		{//��֪�ܴӽڵ���=0
			return 1;//����b0-b15δ����!=0�Ա����ñ�󼴿�ʼ�ճ�
		}
	}
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	x=0;//δ����
	y=0;//ʧ����
	for(i=0;i<n;i++)
	{
		z=Terminal_Note->List[i].RMCompleteS[RMTask];//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
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
	return (x+(y<<16));
}

void ClrRMTaskComplete_3762(u32 RMTask,u32 All)//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
{
	u32 i;
	u32 n;
	
	n=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	for(i=0;i<n;i++)
	{
		if(All==0)
		{//ֻ��ʧ��
			if(Terminal_Note->List[i].RMCompleteS[RMTask]==1)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
			{
				Terminal_Note->List[i].RMCompleteS[RMTask]=0;//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
			}
		}
		else
		{//��ȫ��
			Terminal_Note->List[i].RMCompleteS[RMTask]=0;
		}
	}
}

u32 NewTask_3762(u32 PORTn)//3762��ִ��������;���أ�0=ûִ��������1=ִ��������
{
	u32 i;
	u32 n;
  UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	u32 x;
	u32 y;
	u16 *p16timer;
	
	if(Terminal_Router->RouterInitTask<200)
	{
		return 0;
	}
	if(Terminal_Router->RouterCtrl!=0)//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
	{
		return 0;
	}
	if(Terminal_Router->StopFlags&0xfffffffc)//��ͣ·�ɱ�־:b0=��������ʱ,b1=���ص㻧
	{//·���зǳ�������ͣ
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	{
		return 0;
	}
	if(((UARTCtrl->RMMeterEvent&1)!=0)||(UARTCtrl->SubsequentFrames!=0))
	{//���ڳ������ܱ������ϱ��¼��������֡
		return 0;
	}
	
	n=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	if(UARTCtrl->RMTaskN&0x10)//��ÿСʱ���߳�������
	{//����ÿСʱ���߳�������,���õ㳭
		if((UARTCtrl->RMTaskC!=0)&&(UARTCtrl->RMTaskC!=4)&&(UARTCtrl->RMCount==0))
		{//ԭ������������4=ÿСʱ��������
			UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			UARTCtrl->RMTaskN&=0xef;//ȥ������
			UARTCtrl->RMTaskCOfKeyUser=UARTCtrl->RMTaskC;
			UARTCtrl->RMTaskC=4;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].RMCompleteS[4]=0;
			}
			UARTCtrl->MeterNoBackupOfKeyUser=UARTCtrl->MeterNo;//485����ԭѭ�������������ż���(���ֶ���)
			UARTCtrl->FnCountBackupOfKeyUser=UARTCtrl->FnCount;//485����ԭѭ�������Fn����
			UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
			UARTCtrl->FnCount=0;//485����Fn����
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].FnCountSOfKeyUser=Terminal_Note->List[i].FnCount;//����ÿСʱ������������ǰԭѭ������ĳ���Fn����
				Terminal_Note->List[i].FnCount=0;//����Fn����
				Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
				Terminal_Note->List[i].ReRMCount=0;//�ط�����
				Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
			}
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
			return 1;
		}
	}
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
	{//ֻ��ѭ��ʱ�򳭶����ܱ��¼�ʱ���ܲ�������ȼ�
		if(UARTCtrl->DateReRMCount!=0)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
		{
			NewRMEvene(PORTn);//�»�ָ������ܱ��¼�����;����0=û�½���,1=�½���
		}
	}
#endif
	if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
	{//ѭ��ʱ���ܲ�������ȼ�
		if(UARTCtrl->RMTaskN&2)//��ÿ������򲹳�����
		{
		ReTask:
			UARTCtrl->RMTaskN&=0xfd;//ȥ��ÿ������
			UARTCtrl->RMTaskC=1;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			if(UARTCtrl->DateReRMCount!=0xff)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
			{
				UARTCtrl->DateReRMCount++;//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
			}
			for(i=0;i<n;i++)
			{
				if(Terminal_Note->List[i].RMCompleteS[1]==1)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
				{//ֻ�岻�ɹ���
					Terminal_Note->List[i].RMCompleteS[1]=0;
				}
			}
		NewTask:
			UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			UARTCtrl->MeterNoS=UARTCtrl->MeterNo;//485����ԭѭ�������������ż���(���ֶ���)
			UARTCtrl->FnCountS=UARTCtrl->FnCount;//485����ԭѭ�������Fn����
			UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
			UARTCtrl->FnCount=0;//485����Fn����
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].FnCountS=Terminal_Note->List[i].FnCount;//��������ǰԭѭ������ĳ���Fn����
				Terminal_Note->List[i].FnCount=0;//����Fn����
				Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
				Terminal_Note->List[i].ReRMCount=0;//�ط�����
				Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
			}
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
			//Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
			return 1;
		}
		if(UARTCtrl->RMTaskN&4)//��ÿ������
		{//���õ㳭
			UARTCtrl->RMTaskN&=0xfb;//ȥ��ÿ������
			UARTCtrl->RMTaskC=2;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			ClrRMTaskComplete_3762(2,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
			goto NewTask;
		}
		if(UARTCtrl->RMTaskN&8)//�¶�ʱ����ͣ���¼����
		{//���õ㳭
			UARTCtrl->RMTaskN&=0xf7;//ȥ�¶�ʱ����ͣ���¼����
			UARTCtrl->RMTaskC=3;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			goto NewTask;
		}
	}
	else
	{//�ָ�
		y=NoRMCompletes_3762(UARTCtrl->RMTaskC);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
		if((y&0xffff)==0)//δ����=0
		{//��ǰ�������
			//�ָ�
			UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			if(UARTCtrl->RMTaskC!=0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			{//�����ϵ�����
				//�ָ�
				if(UARTCtrl->RMTaskC!=4)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=ÿСʱ��������,(MaxRMTask-1)=ѭ������
				{//����ÿСʱ���ص㻧���߻ָ�
					i=UARTCtrl->MeterNoS;//485����ԭѭ�������������ż���(���ֶ���)
					if(i!=0)
					{
						i--;
					}
					UARTCtrl->MeterNo=i;
					UARTCtrl->FnCount=UARTCtrl->FnCountS;//485����ԭѭ�������Fn����
					UARTCtrl->RMCount=0;//485�����������
					UARTCtrl->ReRMCount=0;//485���������ط�����
					UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
					for(i=0;i<n;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountS;//��������ǰԭѭ������ĳ���Fn����
						Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
						Terminal_Note->List[i].ReRMCount=0;//�ط�����
						Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
					}
				#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					DelRMEvene(PORTn);//ɾ�������ܱ��¼�����
					if(UARTCtrl->RMTaskC==(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
					{//�ָ����ǳ����ϵ�1���ĵ��ܱ��¼�
						return 1;
					}
					if(UARTCtrl->DateReRMCount!=0)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
					{//�����ѳ�1��
						y=NoRMCompletes_3762(1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
						x=y;
						x&=0xffff;
						y>>=16;//ʧ����
						y+=x;//��δ����
						x=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
						if(x!=0)
						{
							y*=100;
							y/=x;//ʧ����
						}
					#if (USER/100)==5//�Ϻ�
						if(y<=2)//����ɹ���>=98%
						{
							y=0;//ʧ����=0
						}
					#endif
						x=Comm_Ram->TYMDHMS[2];//ʵʱ�ӵ�ʱ
					#if (USER/100)==11//����ϵ
						if(x>=0x09)
						{
							if(y<=2)//����ɹ���>=98%
							{
								x=0x12;//ʱ�ӵ�ͬ12��
								y=0;//ʧ����=0
							}
						}
						else
						{
							y=1;//ʧ����=1
						}
					#endif
						if((y==0)||(x>=0x12))
						{//����ɹ���100%��ʱ��>=12��
							UARTCtrl->RMTaskC=(MaxRMTask-1);//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
						#if((USER/100)==5)//�Ϻ�
							RMEveneTimer_ShH(PORTn,y);//�Ϻ��������ּ������ܱ��¼���ʱ
						#endif
							NewRMEvene(PORTn);//�»�ָ������ܱ��¼�����;����0=û�½���,1=�½���
							if(UARTCtrl->RMTaskC==(MaxRMTask-2))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
							{//���½�����ܱ��¼�
								return 1;
							}
						}
					}
				#endif
					if(UARTCtrl->RMTaskN&4)//��ÿ������
					{//���õ㳭
						UARTCtrl->RMTaskN&=0xfb;//ȥ��ÿ������
						UARTCtrl->RMTaskC=2;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
						ClrRMTaskComplete_3762(2,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						goto NewTask;//�ڴ˽���,������ѭ��������·�ɶ�Ӱ��·�ɽ���
					}
					if(UARTCtrl->RMTaskN&8)//�¶�ʱ����ͣ���¼����
					{//���õ㳭
						UARTCtrl->RMTaskN&=0xf7;//ȥ�¶�ʱ����ͣ���¼����
						UARTCtrl->RMTaskC=3;//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
						goto NewTask;//�ڴ˽���,������ѭ��������·�ɶ�Ӱ��·�ɽ���
					}
					y=NoRMCompletes_3762(1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					y>>=16;//ʧ����
				#if((USER/100)!=5)//�Ϻ��û�
					if(y!=0)
					{//��ʧ��,��������,������ѭ��������·�ɶ�Ӱ��·�ɽ���
					#if (USER/100)!=0//΢�������ߵ��Ժ��������������,����Ӱ�쳭��ɹ��ʲ���
						//����΢�������ߵ��Ժ����
						if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
						{//΢��������
							if((Terminal_Router->RUNState&1)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
							{//��ǰ������������
								if((Comm_Ram->TYMDHMS[2]<=6)||(Comm_Ram->TYMDHMS[2]>=0x20))
								{//��ʱ��0-6���20��-24��������������
									Terminal_Router->RouterCtrl=6;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
									return 0;
								}
							}
						}
					#endif
						UARTCtrl->MeterNo++;
						goto ReTask;
					}
				#else
					//�Ϻ���Ҫ����ѭ��,�����������ն�����ʧ�ܣ���������,������ѭ��������·�ɶ�Ӱ��·�ɽ���
					if(UARTCtrl->RMTaskC==(MaxRMTask-1))
					{//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
						x=NoRMCompletes_3762(MaxRMTask-1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
						if(x==0)
						{//ȫ�����ɹ�
							ClrRMTaskComplete_3762(MaxRMTask-1,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						}
						else
						{
							x&=0xffff;
							if(x==0)
							{//ȫ���ѳ�����ʧ��
								ClrRMTaskComplete_3762(MaxRMTask-1,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
								goto SupplementRM;
							}
						}
					}
					else
					{
					SupplementRM:	
						if(y!=0)
						{//��ʧ��,��������,������ѭ��������·�ɶ�Ӱ��·�ɽ���
						#if (USER/100)!=0//΢�������ߵ��Ժ��������������,����Ӱ�쳭��ɹ��ʲ���
							//����΢�������ߵ��Ժ����
							if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{//΢��������
								if((Terminal_Router->RUNState&1)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
								{//��ǰ������������
									if((Comm_Ram->TYMDHMS[2]<=6)||(Comm_Ram->TYMDHMS[2]>=0x20))
									{//��ʱ��0-6���20��-24��������������
										Terminal_Router->RouterCtrl=6;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
										return 0;
									}
								}
							}
						#endif
							UARTCtrl->MeterNo++;
							goto ReTask;
						}
					}
				#endif
				}
				else
				{//��ÿСʱ���ص㻧���߻ָ�
					i=UARTCtrl->MeterNoBackupOfKeyUser;//485����ԭѭ�������������ż���(���ֶ���)
					if(i!=0)
					{
						i--;
					}
					UARTCtrl->MeterNo=i;
					UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485����ԭѭ�������Fn����
					UARTCtrl->RMCount=0;//485�����������
					UARTCtrl->ReRMCount=0;//485���������ط�����
					UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
					for(i=0;i<n;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfKeyUser;//����ÿСʱ������������ǰԭѭ������ĳ���Fn����
						Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
						Terminal_Note->List[i].ReRMCount=0;//�ط�����
						Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
					}
					UARTCtrl->RMTaskC=UARTCtrl->RMTaskCOfKeyUser;//�����ص㻧��������ǰԭ����
					UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
					return 0;
				}
			}
			else
			{//���ϵ��������
				if((Terminal_Ram->PowerTask==1)||(Terminal_Ram->PowerTask==10))//10=�ȴ�������ɱȶԵ��ܱ�
				{//�¼�����:�ȴ�������ɱȶԵ��ܱ�
					//i=NoRMCompletes_3762(UARTCtrl->RMTaskC);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					//if(i!=0)
					//{//ûȫ���ɹ�
						p16timer=(u16*)((u32)&Terminal_Ram->RS4854RM_S_Timer);//20 RS485-4�����붨ʱ��(�ز�)
						if(p16timer[8]!=0)//ͣ���¼�����ʱ����ֵ�붨ʱ��
						{
							ClrRMTaskComplete_3762(0,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
							return 0;
						}
					//}
				}
				//�´γ�ͣ���¼���ʱ��
				i=MRR(ADDR_AFN04F97+1,1);//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
				p16timer=(u16*)(&Terminal_Ram->RS4854RMF246_M_Timer);//25 RS485-1ͣ���¼�����ʱ�����ֶ�ʱ��
				p16timer[0]=i*60;//��
				x=MRR(ADDR_TYMDHMS+2,4);
				x=YMDH_AddH(x,i);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
				d64a=x;
				d64a<<=16;//
				MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//��ʱ��ͣ���¼��������´γ���ʱ�����ʱ������
			#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
				goto ReTask;//����������,�����1��ѭ�����ٽ���
			#endif
			}
			Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
			UARTCtrl->RMTaskC=(MaxRMTask-1);//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
		}
	}
	return 0;
}


void Terminal_uart_3762_Manage(void)//376.2·��ģ�����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u64 d64a;
	u8 *p8;
	u16 * p16;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 PORTn;
#if Extern_BatchTask_3762!=0//�ⲿ����:0=I2,1=E1,2=E2,3=
	u32 *p32;
#endif
	
	PORTn=RS485_4PORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);


#if PowerOnRMTask==0//��Դ����������;0=û��,1=��
	if(UARTCtrl->RMTaskC==0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
	{
		UARTCtrl->RMTaskC=(MaxRMTask-1);
	}
#endif
	
//�ָ���
	if(UARTCtrl->RTCM!=Comm_Ram->TYMDHMS[1])
	{
		UARTCtrl->RTCM=Comm_Ram->TYMDHMS[1];
	#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
		RMEveneTimer(PORTn);//�ּ������ܱ��¼���ʱ
	#if((USER/100)==5)//�Ϻ�
		RMEveneTimer_ShH(PORTn,y);//�Ϻ��������ּ������ܱ��¼���ʱ
	#endif
	#endif
		//�㲥Уʱ
		i=MRR(ADDR_AFN05F30+1,1);//�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
		if(i==1)
		{//0=�رգ�1=����������ֵ�ر�
			i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//�ն˳������в�������,�˿�31
			if((i&0x08)!=0x0)
			{//Ҫ���ն˶�ʱ�Ե��㲥Уʱ
				x=MRR(ADDR_TYMDHMS+1,3);//ȡ��ǰʵʱ����ʱ��
				y=MRR((ADDR_AFN04F33+((14+(4*24))*30))+10,3);//�Ե��㲥Уʱ��ʱʱ����ʱ��
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
		x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
		if(x==1)
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
							x=MRR(ADDR_AFN04F298_Ext,1);
							if((x==1)||(x==2))
							{//ָ�����ַ��ɼ�����ַ�Ĺ㲥��ʱ����
								if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)<3)//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
								{
									MWR(3,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
								}
							}
							else
							{//ȫ�ն˵Ĺ㲥��ʱ����
								UARTCtrl->BroadcastTime=3;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
							MWR(2,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
						}
					}
				}
			}
		}
	#endif	
	#if((USER/100)==5)//�Ϻ�
		i=MRR(ADDR_AFN05F201,1);
		if(i&(1<<3))
		{
			if(UARTCtrl->BroadcastTime<3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			{
				UARTCtrl->BroadcastTime=3;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
		}
	#endif											    
	}
//ʱ����
	if(UARTCtrl->RTCH!=Comm_Ram->TYMDHMS[2])
	{
		UARTCtrl->RTCH=Comm_Ram->TYMDHMS[2];
		i=Comm_Ram->TYMDHMS[2];
		switch(i)//�ɼ�Ƶ��Ϊÿ��2��6��10��14��18��22��
		{
			case 0x02:
			case 0x06:
			case 0x10:
			case 0x14:
			case 0x18:
			case 0x22:
			#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
				{//��ǰ��ѭ��
					i=NoRMCompletes_3762(1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					if(i!=0)
					{//�貹��
						UARTCtrl->RMTaskN|=2;//��ÿ������
					}
				}
			#endif
	#if (((USER/100)==4)||((USER/100)==6)||((USER/100)==8)||((USER/100)==11))//�û���ʶ:������,����,ͨ���г�,����
			//��� ���ڳ����ز���07�������
		#if WeekRMTask==1//ÿ�ܳ�������;0=û��,1=��(���ÿ�յ�ʱ����:������û����������û��͹���ҵ�û���ǰ��ѹ�������������ۼ�ʱ�����ݵĲɼ�)
			#if (((USER/100)==6))//�û���ʶ:�Ǻ���ϵ
				if(UARTCtrl->DateReRMCount!=0)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
				{//�����ѳ�1��
					y=NoRMCompletes_3762(1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					x=y;
					x&=0xffff;
					y>>=16;//ʧ����
					y+=x;//��δ����
					x=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
					if(x!=0)
					{
						y*=100;
						y/=x;//ʧ����
					}
				#if (USER/100)==6//����
					if(y<=2)
					{
						y=0;//���ϳ���ɹ���>=98%
					}
				#endif
					x=Comm_Ram->TYMDHMS[2];//ʵʱ�ӵ�ʱ
					if((y==0)||(x>=0x14))
					{//����ɹ���100%��ʱ��>=14��
						UARTCtrl->RMTaskN|=4;//��ÿ������
					}
				}
			#else//�û���ʶ:������,ͨ���г�,����
				UARTCtrl->RMTaskN|=4;//��ÿ������
			#endif

		#endif
	#endif
				break;
		}
	#if HourTask==1//ÿСʱ���߳���
		KeyUserPnList();//�ص㻧���߶���Pn�б�
		//���Ƿ����ص㻧Ϊ�����
		p16=(u16*)(ADDR_KeyUserPnList);
		for(i=0;i<MaxKeyUser;i++)
		{
			x=p16[i];//�������
			if(x!=0)
			{//���ص㻧
				x=Check_AFN04F10_Pn(x);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
				if(x!=0)
				{
			#if (USER/100)==8//ͨ�ð汾���������д�С��ţ�
					break;
			#else
					x--;
					x=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*x)+26,1);//ȡ��С���
				#if (USER/100)==5//�Ϻ�ϵ
					if((x!=0x02)&&(x!=0x52)&&(x!=0x62)&&(x!=0x72))//��С���=51��71,������С��Ű�51����
				#else
					#if (USER/100)==11//����
					if((x==0x30)||(x==0x31)||(x==0x40)||(x==0x41)||(x==0x44)||(x==0x45))
					#else
					if((x==0x50)||(x==0x51)||(x==0x40)||(x==0x41))//���ϵ�����
					#endif
				#endif
					{
						break;
					}
			#endif
				}
			}
		}
		if(i!=MaxKeyUser)
		{
			UARTCtrl->RMTaskN|=0x10;//��ÿСʱ��������
		}
	#endif
	}
//�ո���
	if((UARTCtrl->Task==0)&&(UARTCtrl->Lock!=0x55))//0=����
	{
		x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//ԭ����
		y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//��ǰ����
		if(x!=y)
		{
		#if ((USER/100)==5)
			UARTCtrl->RMEventTaskN|=0x20;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3;b5=�ո���,b6=�¸���(�Ϻ�ÿ���³��¼���)
			if(UARTCtrl->RTCD[1]!=Comm_Ram->TYMDHMS[4])
			{
				//�¸���
				UARTCtrl->RMEventTaskN|=0x40;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3;b5=�ո���,b6=�¸���(�Ϻ�ÿ���³��¼���)
			}
		#endif
			UARTCtrl->RTCD[0]=y;
			UARTCtrl->RTCD[1]=y>>8;
			UARTCtrl->RMComplete&=0x7f;//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
		#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			StoreRMEvene(PORTn);//�ݴ����ڶ����ܱ��¼�������
		#endif
	#if (USER/100)!=0
			//���Ժ���Թ��ղ��س�ʼ��,��ͨ�Ŵ�������ʱ���ط���,ͬʱѭ����־����0������ԭ��ѭ��
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	#endif
			//ÿ������
			UARTCtrl->DateReRMCount=0;//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
	#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
			UARTCtrl->RMTaskN|=2;//��ÿ������
			ClrRMTaskComplete_3762(1,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
			WarningCancel(100);//�澯ȡ��,��ڸ澯����
	#endif
			//�弯�г�����Ϣ
			p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
			p8+=(19*2);
			MC(0,((u32)p8)+4,4);
			MC(0xee,((u32)p8)+8,12);
			//�Զ��ѱ�
	#if (USER/100)!=6//�û�:���Ǻ���;�����������ж�ʱ
			UARTCtrl->LockFlags&=0xfb;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
	#endif
	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
			//�ո���ʱ����
			MC(0,ADDR_AFN0CF250_Ext1,10*MaxRS485AddCarrierPn+4);
			MC(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,3);
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
				x=UARTCtrl->RTCW[0]+(UARTCtrl->RTCW[1]<<8);//ԭ������
				y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//��ǰ������
				if(x!=y)
				{
					UARTCtrl->RTCW[0]=y;
					UARTCtrl->RTCW[1]=y>>8;
					UARTCtrl->RMTaskN|=4;//��ÿ������
				}
			}
		}
	}
	#endif
#endif

//�����
	if(UARTCtrl->RTCS!=Comm_Ram->TYMDHMS[0])
	{//��������ʱ����ʱҲ����1��,��NewTask_3762�������ڿ���ʱ��Ч
		UARTCtrl->RTCS=Comm_Ram->TYMDHMS[0];
		//����ʱ��
		if(UARTCtrl->RMTaskC==0)
		{//������,����Ҫ����
			Terminal_Router->RMTD=1;//0=�ǳ���ʱ��,1=����ʱ��
		}
		else
		{
			x=MRR((ADDR_AFN04F33+((14+(4*24))*30))+13,1);//�˿�31,������ʱ����m
			if(x>24)
			{
				x=24;
			}
//		#if (USER/100)==0//xxx
//			//���Ժ�������ǳ���
//			w=w;
//			Terminal_Router->RMTD=1;//0=�ǳ���ʱ��,1=����ʱ��
//		#else
			y=MRR(ADDR_TYMDHMS+1,2);//ȡ��ǰʵʱ��ʱ��
			Terminal_Router->RMTD=0;//0=�ǳ���ʱ��,1=����ʱ��
			for(i=0;i<x;i++)
			{
				z=MRR((ADDR_AFN04F33+((14+(4*24))*30))+14+(i*4),2);//ȡ������ʱ�ο�ʼʱ��ʱ��
				if(y>=z)
				{
					w=MRR((ADDR_AFN04F33+((14+(4*24))*30))+14+(i*4)+2,2);//ȡ������ʱ�ν���ʱ��ʱ��
					if(w==0x0)
					{
						w=0x2400;//����ʱ��ʱ��00:00����Ϊ24:00
					}
					if(y<w)
					{//����ʱ����
						Terminal_Router->RMTD=1;//0=�ǳ���ʱ��,1=����ʱ��
						break;
					}
				}
			}
//		#endif	
		}
		if((Terminal_Router->RMTD!=0)&&(Terminal_Router->RouterInitTask>=200))//0=�ǳ���ʱ��,1=����ʱ��
		{//����ʱ����
			Terminal_AFN0CF11_3762();//�ն���1����F11,�ն˼��г���״̬��Ϣ

			//��ʱ�ɼ�ͣ���¼����
		#if TimerRMTask==1//��ʱ����ͣ���¼����;0=û��,1=��
			if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			{//ѭ������
				i=MRR(ADDR_AFN04F97,1);//ͣ�����ݲɼ����ò���
				i&=1;
				if(i!=0)
				{//Ҫ�ɼ�
					i=MRR(ADDR_AFN04F97+1,1);//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
					if(i!=0)
					{//����ʱ����!=0
						p16=(u16*)(&Terminal_Ram->RS4854RMF246_M_Timer);//25 RS485-1ͣ���¼�����ʱ�����ֶ�ʱ��
						if(p16[0]==0)
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
							ClrRMTaskComplete_3762(3,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
							i=MRR(ADDR_AFN04F97+1,1);//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
							p16[0]=i*60;//��
							x=MRR(ADDR_TYMDHMS+2,4);
							x=YMDH_AddH(x,i);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
							d64a=x;
							d64a<<=16;//
							MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//��ʱ��ͣ���¼��������´γ���ʱ�����ʱ�����
	
							i=MRR(ADDR_AFN04F97+2,1);//ͣ���¼�����ʱ����ֵ	BIN	1	��λΪ����
							i*=60;
							p16=(u16*)(&Terminal_Ram->RS4854RMF246_S_Timer);//25 RS485-4ͣ���¼�����ʱ����ֵ�붨ʱ��
							p16[0]=i;
						}
					}
				}
			}
		#endif
		
//			i=NewTask_3762(PORTn);//3762��ִ��������;���أ�0=ûִ��������1=ִ��������
//			if(i!=0)
//			{//�˷��ޱ�����,��ʹ������ÿѭ��ʱ��С
//				return;
//			}
			//��������ͣ
			if(UARTCtrl->RMTaskC==(MaxRMTask-1))//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,4=������,(MaxRMTask-1)=ѭ������
			{//��ǰ��ѭ��
				p16=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
//				z=DataComp(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
//				if(z!=2)
//				{
//					z=0;//��Ȼ�TYMDHMS>NextRMTime
//				}
//				if((p16[0]==0)||(z==0))
//				{
//					z=0;//����ʱ���ڵ�
//				}
//				else
//				{
//					z=1;//û����ʱ���ڵ�
//				}
				z=TimeDifference(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(z>>31)
				{//TYMDHMS>NextRMTime
					z=0;//��Ȼ�TYMDHMS>NextRMTime
				}
				else
				{
					i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);//������Сʱ1-60
					if(i>60)
					{
						i=60;
					}
					i*=3600;
					if(z>i)
					{
						z=0;//ʱ�ӵ���
					}
				}
				if(p16[0]==0)
				{
					z=0;//����ʱ���ڵ�
				}
				if(UARTCtrl->RMFlag!=0)//0=��������ͣ,1=���������
				{
					x=NoRMCompletes_3762(MaxRMTask-1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					if(x==0)
					{//ȫ�����ɹ�	
						if(((Terminal_Router->StopFlags&0xfffffffe)==0)&&(Terminal_Router->RouterCtrl==0))//��ͣ·�ɱ�־:b0=��������ʱ
						{
						#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
							i=NoRMCompletes_3762(1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
							if(i!=0)
							{//�貹��,����ͣ
								UARTCtrl->RMTaskN|=2;//��ÿ������
								NewTask_3762(PORTn);//3762��ִ��������;���أ�0=ûִ��������1=ִ��������
								Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
								return;
							}
						#endif
							UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
							if((Terminal_Router->StopFlags&1)==0)//��ͣ·�ɱ�־:b0=��������ʱ
							{
								Terminal_Router->StopFlags|=(1<<0);//��ͣ·�ɱ�־:b0=��������ʱ
								Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
							}
						}
					}
					else
					{//
						x&=0xffff;//δ����
						if(x==0)
						{//ȫ���ѳ�����ʧ��
							if(((Terminal_Router->StopFlags&0xfffffffe)==0)&&(Terminal_Router->RouterCtrl==0))//��ͣ·�ɱ�־:b0=��������ʱ
							{
							#if DateRMTask==1//ÿ�ճ�������;0=û��,1=��
								i=NoRMCompletes_3762(1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
								if(i!=0)
								{//�貹��,����ͣ
									UARTCtrl->RMTaskN|=2;//��ÿ������
									NewTask_3762(PORTn);//3762��ִ��������;���أ�0=ûִ��������1=ִ��������
									Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
									return;
								}
							#endif
							}	
							if(z==0)
							{//����ʱ���ڵ�
								UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
							}
						}
					#if (USER/100)==0
						//���Ժ�����Բ���ÿ���������ڼ�������
						if(z==0)
						{//����ʱ���ڵ�
							UARTCtrl->RMFlag=0;//0=��������ͣ,1=���������
						}
					#endif
					}
				}
				if(UARTCtrl->RMFlag==0x0)//0=��������ͣ,1=���������
				{//0=��������ͣ
					if(z==0)
					{//����ʱ���ڵ�
						if((Terminal_Router->StopFlags&0xfffffffe)==0)//��ͣ·�ɱ�־:b0=��������ʱ
						{
							Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
							UARTCtrl->RMFlag=1;//1=���������
							i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);//������Сʱ1-60
							if(i==0)
							{
								i=1;//��С1
							}
							if(i>60)
							{
								i=60;
							}
							y=MRR(ADDR_TYMDHMS+2,4);
							d64a=YMDH_AddH(y,i);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
							d64a<<=16;
							MWR(d64a,(u32)&UARTCtrl->NextRMTime,6);
							i*=3600;
							i+=2;//���2,����59��59�붨ʱ��,ͬ�����ֹ�0,�����1ʱ
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16[0]=i;//������ʱ��s
							//���б����־
							ClrRMTaskComplete_3762(MaxRMTask-1,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
							UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
							UARTCtrl->FnCount=0;//485����Fn����
							UARTCtrl->RMCount=0;//485�����������
							UARTCtrl->ReRMCount=0;//485���������ط�����
							UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
							y=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
							if(y>MaxRS485AddCarrierPn)
							{
								y=MaxRS485AddCarrierPn;
							}
							for(i=0;i<y;i++)
							{
								Terminal_Note->List[i].FnCount=0;//����Fn����
								Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
								Terminal_Note->List[i].ReRMCount=0;//�ط�����
								Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
							}
						}
					}
				}
			
			}
		}
	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭						
		x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
		switch(x)
		{
			case 0:
				i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
				if(i&0x01)
				{
					x=MRR(ADDR_AFN0CF249_Ext+9,1);
					if(x!=0)
					{//�ն˾�ȷ��ʱ�ɹ�
						if(MRR(ADDR_AFN04F298_Ext,1)!=0)
						{//����ʱ���ؿ���
							if(MRR(ADDR_AFN04F298_Ext+6,1))
							{//�����ʱ������Ϊ0
								if(UARTCtrl->RMFlag==0)//0=��������ͣ,1=���������
								{
									x=NoRMCompletes_3762(MaxRMTask-1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
									if(x==0)
									{//ȫ�����ɹ�
										MWR(1,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
									}	
								}
							}
						}
					}
				}
				break;
			case 2:
				if(UARTCtrl->BroadcastTime<3)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				{
					if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)<3)//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
					{
						MWR(3,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						MWR(i&0xfe,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
	
						Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
						UARTCtrl->RMFlag=1;//1=���������
						//���б����־
						ClrRMTaskComplete_3762(MaxRMTask-1,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
						UARTCtrl->FnCount=0;//485����Fn����
						UARTCtrl->RMCount=0;//485�����������
						UARTCtrl->ReRMCount=0;//485���������ط�����
						UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						y=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
						if(y>MaxRS485AddCarrierPn)
						{
							y=MaxRS485AddCarrierPn;
						}
						for(i=0;i<y;i++)
						{
							Terminal_Note->List[i].FnCount=0;//����Fn����
							Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
							Terminal_Note->List[i].ReRMCount=0;//�ط�����
							Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
						}
					}
				}
				break;
			case 3:
				if(UARTCtrl->RMFlag==0)//0=��������ͣ,1=���������
				{
					x=NoRMCompletes_3762(MaxRMTask-1);//�ز�δ������ʧ����;���:RMTask=�����;����:b0-b15δ����,b16-b31ʧ����
					if(x==0)
					{//ȫ�����ɹ�
						MWR(4,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
					}	
				}
				break;
			case 4://�ո���ʱ����
				break;
		}
	#endif	
	}
	if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{//�����ͣʱ,�ȴ�����ģ��������ϱ�����볬ʱ״̬
		if(Terminal_Router->RMTD!=0)//0=�ǳ���ʱ��,1=����ʱ��
		{//����ʱ����
			NewTask_3762(PORTn);//3762��ִ��������;���أ�0=ûִ��������1=ִ��������
		}
	}


//·��ģ��
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		//Comm_Ram->SelfTestFlags&=~(1<<16);//B16=�ز�ģ��,(��λ��ʾOK)
		Terminal_Router->NoteNum=0;//��֪�ܴӽڵ���
		Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	}
	Uart_3762_RxTx(PORTn);//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	Terminal_RouterCtrl(PORTn);//·��������;����,��ͣ,�ָ�,���
	if(Terminal_Router->RouterCtrl!=0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
	{
		return;
	}
//ʵʱ����
	if(Terminal_Router->RouterRealTimeTask!=0)//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
	{
		//RealTimeReadMeter_3762_Fn_Std(PORTn);//����1��Fn>=129������д��FnDataBuff;RouterRealTimeTask=0=����(��Fn�������)
		return;
	}
	
//������ʱ����
#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
#else
	if((Terminal_Router->RouterInitTask>=200)&&(Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
#endif
	{
	#if Extern_BatchTask_3762==0//�ⲿ����:0=I2,1=E1,2=E2,3=
		BatchTask_3762_SETRTC(PORTn);//����ת����������-3762-ʱ�Ӷ�ʱ
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
					BatchTask_3762_SETRTC(PORTn);//����ת����������-3762-ʱ�Ӷ�ʱ
				#else
					if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
					{
		    		p32=(u32 *)(ADDR_E1_ENTRY);
		    		(*(void(*)())p32[104])(PORTn);//����ת����������-3762-ʱ�Ӷ�ʱ
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
	}
	
//�Զ��ѱ�
#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
#else
	if((Terminal_Router->RouterInitTask>=200)&&(Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
#endif
	{
		i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
		switch(i)
		{
			default:
				if((UARTCtrl->LockFlags&2)!=0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				{//�����Զ��ѱ�
					if(UARTCtrl->AutoSearchTask<100)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
					{
						UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
						UARTCtrl->AutoSearchTask=100;//��ֹ����ע��
					}
					AutoSearchMeter_3762(PORTn);//�Զ��ѱ�3762�˿ڵĵ��ܱ�
					return;
				}
				break;
			case 0x55://�յ�������
				if((UARTCtrl->LockFlags&2)!=0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				{//�����Զ��ѱ�
					if(UARTCtrl->AutoSearchTask<100)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
					{
						UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
						UARTCtrl->AutoSearchTask=100;//��ֹ����ע��
					}
					AutoSearchMeter_3762(PORTn);//�Զ��ѱ�3762�˿ڵĵ��ܱ�
					return;
				}
				if((UARTCtrl->Lock==0x55)||(UARTCtrl->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					break;
				}
				//���б����г������ݱ�ʶ����
				x=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
				for(i=0;i<x;i++)
				{
					Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
					Terminal_Note->List[i].ReRMCount=0;//�ط�����
					Terminal_Note->List[i].RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				}
				MWR(0x5a,ADDR_AFN05F149+2+3,1);//����ִ��
				UARTCtrl->AutoSearchTask=1;//�Զ��ѱ�����:0=��,1=��ʼ,2...����
				UARTCtrl->LockFlags|=6;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
		#if (USER/100)==6//�û�:����
				i=MRR(ADDR_AFN05F103+1,2);
				i++;//��1��
				Terminal_Ram->AutoSearchMeterTimer=i;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
		#endif
				return;
			case 0x5a://����ִ��
		#if (USER/100)==6//�û�:����
				if(Terminal_Ram->AutoSearchMeterTimer==0)//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
				{
					i=MRR(ADDR_AFN05F103,1);
					if(i==0x55)
					{//��F103����,��ÿ��ִ��
						MWR(0,ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
					}
					if(UARTCtrl->AutoSearchTask<100)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
					{
						UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
						UARTCtrl->AutoSearchTask=100;//��ֹ����ע��
					}
					AutoSearchMeter_3762(PORTn);//�Զ��ѱ�3762�˿ڵĵ��ܱ�
					return;
				}
		#endif
				UARTCtrl->LockFlags|=6;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
				AutoSearchMeter_3762(PORTn);//�Զ��ѱ�3762�˿ڵĵ��ܱ�
				return;
			case 0xaa://ÿ��ִ��
				if((UARTCtrl->LockFlags&2)==0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				{//û�����Զ��ѱ�,ÿ���ڳ������������
					if((UARTCtrl->LockFlags&4)==0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
					{//�������Զ��ѱ�==0
						if((UARTCtrl->Lock!=0x55)&&(UARTCtrl->Task==0))
						{
							if((Comm_Ram->TYMDHMS[2]>=0x20)&&(Comm_Ram->TYMDHMS[2]<0x22))
							{//ÿ����20:00-22:00����,��23:00������С��60��
							#if (USER/100)!=7//���㽭�û�
							//�㽭�������ֳ���վ�����·�����,Ҫ�������Զ��ѱ�
								if(UARTCtrl->RMComplete&0x80)//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
							#endif
								{
									//���б����г������ݱ�ʶ����
									x=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
									for(i=0;i<x;i++)
									{
										Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
										Terminal_Note->List[i].ReRMCount=0;//�ط�����
									}
									UARTCtrl->AutoSearchTask=1;//�Զ��ѱ�����:0=��,1=��ʼ,2...����
									UARTCtrl->LockFlags|=6;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
									UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									return;
								}
							}
						}
					}
				}
				else
				{//�Զ��ѱ�
					AutoSearchMeter_3762(PORTn);//�Զ��ѱ�3762�˿ڵĵ��ܱ�
					if((Comm_Ram->TYMDHMS[2]<0x20)||(Comm_Ram->TYMDHMS[2]>=0x23))
					{//����ÿ����20:00-23:00
						UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					}
					return;
				}
				break;
		}
	}

//����ָ��ͨ�Ŷ˿����³���
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((MRR(ADDR_AFN05F51+30,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//����ָ��ͨ�Ŷ˿����³���
	{
		if((UARTCtrl->Task!=1)&&(UARTCtrl->Task!=4))//0=����
		{
			MC(0,ADDR_AFN05F49+30,1);//������ָ��ͨ�Ŷ˿���ͣ����
			MC(0x5a,ADDR_AFN05F51+30,1);//�������³����0x5A
		#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			ClrRMEvene(PORTn);//������ܱ��¼�
		#endif
			p16=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
			p16[0]=0;//��������ʱ
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			for(i=1;i<MaxRMTask;i++)
			{//���������1��MaxRMTask-1
				ClrRMTaskComplete_3762(i,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
			}
			UARTCtrl->RMTaskN=0x02;//������b0-b7�ֱ��ʾ����0-7��������
			UARTCtrl->RMTaskC=(MaxRMTask-1);//ѭ��ʱ���ܲ�������ȼ�
			UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
			UARTCtrl->FnCount=0;//485����Fn����
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			y=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
			for(i=0;i<y;i++)
			{
				Terminal_Note->List[i].FnCount=0;//����Fn����
				Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
				Terminal_Note->List[i].ReRMCount=0;//�ط�����
				Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
			}	
			//���г�����Ϣ��ʼ����ʱ��
			p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
			p8+=(19*2);
			MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			return;
		}
	}
	
//��ʼ��ָ��ͨ�Ŷ˿��µ�ȫ���м�·����Ϣ
	if(UARTCtrl->Task==0)//0=����
	{
		if(MRR(ADDR_AFN05F52+30,1)==0x55)
		{
			MC(0,ADDR_AFN05F52+30,1);
			MWR(3,ADDR_AFN01Fx+5,1);//��ʼ������:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
		}
	}



//
	if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
		if(UARTCtrl->Lock==0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
		{
			return;
		}
	}
//ִ�й㲥Уʱ
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
#endif
		RouterBroadcastTime(PORTn);//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
		if(UARTCtrl->BroadcastTime>=4)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
		{
			if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			{
				UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			return;
		}
	#if ((USER/100)==7)||((USER/100)==8)//�㽭��ͨ�ð�
		RouterBroadcastTime_Collector(PORTn);//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
		if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)>=4)//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
		{
			if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			{
				MWR(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			return;
		}
	#endif

#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	}
#endif

//ִ�г���
	Terminal_ReadMeter_3762(PORTn);//�ն˳���Q/GDW376.2
	
}

#endif



