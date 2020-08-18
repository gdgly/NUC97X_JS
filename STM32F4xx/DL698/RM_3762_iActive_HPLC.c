
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762_iActive.h"
#include "../DL698/RM_3762_RouterActive.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"

#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
void Find_Next_CollectAddr(u32 PORTn,u32 id)
{
	u32 i;
	#ifdef DEBUG_JSON
	u32 z;
	#endif
	u32 SETNO;
	u8 *pADDR_6000;
	UARTCtrl_TypeDef *UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	pADDR_6000=(u8 *)(ADDR_6000_SDRAM);
	SETNO=UARTCtrl->MeterCount[id];

	pADDR_6000+=SETNO*LENper_6000;
	for(i=SETNO;i<NUM_RMmax;i++)
	{
		if(pADDR_6000[0]!=DataType_structure)
		{
			pADDR_6000+=LENper_6000;
			continue;
		}
		break;
	}
	if(i>=NUM_RMmax)
	{
		pADDR_6000=(u8 *)(ADDR_6000_SDRAM);
		i=0;
		for(;i<SETNO;i++)
		{
			if(pADDR_6000[0]!=DataType_structure)
			{
				pADDR_6000+=LENper_6000;
				continue;
			}
			break;
		}
		if(i>=SETNO)
		{
			return;
		}
	}
	UARTCtrl->MeterCount[id]=i;
#ifdef DEBUG_JSON
	myprintf("[%s:%d]:�ɹ��ҵ��˵�������һֻ���λ��:%d\n",(u32)&__func__,(u32)__LINE__,i);
	pADDR_6000+=10;
	for(z=0;z<6;z++)
	{	
		myprintf("%02x",pADDR_6000[z],0,0);
	}
	myprintf("\n",0,0,0);
#endif
}

u32 Get_ListNo(u8* p8)//�õ������б��;���:�����ַ;����:�б��0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax��ʾ��Ч)
{
	u32 ListNo;
	u32 ListTask;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u32 i;
//	u16 *pAddr;
	u32 x;
	u64 d64a;
	u64 d64b;
	
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
	
	
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;////0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		if((ListTask>=1)&&(ListTask<=6))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		{//��Ч
			if(Addr0!=RMLIST->List[ListNo].RMAddr[0])
			{
				continue;
			}
			if(Addr1!=RMLIST->List[ListNo].RMAddr[1])
			{
				continue;
			}
			if(Addr2!=RMLIST->List[ListNo].RMAddr[2])
			{
				continue;
			}
			return ListNo;
		}
	}
	
	x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���

	d64a=MRR((u32)p8,6);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
		d64b=R_Inverse(d64b,6);
		if(d64a==d64b)
		{
			d64b=MRR((u32)&NoteList->List[i].Addr,6);
			d64b=R_Inverse(d64b,6);
			MWR(d64b,(u32)p8,6);
			Addr0=p8[0]|(p8[1]<<8);
			Addr1=p8[2]|(p8[3]<<8);
			Addr2=p8[4]|(p8[5]<<8);
			break;//ԭ�Ѵ���
		}
	}
//	p8=(u8*)ADDR_Meter_Collector;
//	for(i=0;i<NUM_RMmax;i++)
//	{
//		Data_Inverse((u32)p8+6,6);
//		pAddr=(u16*)(p8+6);
//		if(pAddr[0]==Addr0)
//		{
//			if(pAddr[1]==Addr1)
//			{
//				if(pAddr[2]==Addr2)
//				{
//					Data_Inverse((u32)p8,6);
//					Addr0=p8[0]|(p8[1]<<8);
//					Addr1=p8[2]|(p8[3]<<8);
//					Addr2=p8[4]|(p8[5]<<8);
//					Data_Inverse((u32)p8,6);
//					break;
//				}
//			}
//		}
//		Data_Inverse((u32)p8+6,6);
//		p8+=12;
//	}
	
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;////0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		if((ListTask>=1)&&(ListTask<=6))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		{//��Ч
			if(Addr0!=RMLIST->List[ListNo].RMAddr[0])
			{
				continue;
			}
			if(Addr1!=RMLIST->List[ListNo].RMAddr[1])
			{
				continue;
			}
			if(Addr2!=RMLIST->List[ListNo].RMAddr[2])
			{
				continue;
			}
			return ListNo;
		}
	}
	return ListNo;
}

u32 AddrComp(u8* p8)
{
	u64 d64a;
	u64 d64b;
	int i;
	
	d64a=MRR((u32)p8,6);
	d64a=R_Inverse(d64a,6);
	for(i=0;i<NUM_RMmax;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].Addr,6);
		if((d64a==d64b))//&&(NoteList->List[i].user==0))
			return 0;
		if(d64b==0)
			break;
	}
	return 1;
}

u32 CompListRMAddr(u8* p8)//�����ַ�뻺������Ч�еĳ����ַ�Ƚ��Ƿ�����ͬ;���:p8ָ���ַ(�������ͺ�);����:�б��(��0��ʼ,>=NUM_RMLISTmax��ʾû�ҵ�)
{
	u32 ListNo;
	u32 ListTask;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;////0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		if((ListTask>=1)&&(ListTask<=6))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		{//��Ч
			if(Addr0!=RMLIST->List[ListNo].RMAddr[0])
			{
				continue;
			}
			if(Addr1!=RMLIST->List[ListNo].RMAddr[1])
			{
				continue;
			}
			if(Addr2!=RMLIST->List[ListNo].RMAddr[2])
			{
				continue;
			}
			return ListNo;
		}
	}
	return ListNo;
}

void IncAndCheckTxSEQ(u32 PORTn)//����֡���+1,ͬʱ���֡����Ƿ��뻺���еȴ����յ�֡����Ƿ�����ͬ��������ͬ���ټ�1
{
	u32 i;
	u32 ListNo;
	u32 ListTask;
	u32 TxSEQ;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	TxSEQ=UARTCtrl->TxSEQ;
	for(i=0;i<256;i++)
	{
		TxSEQ++;
		TxSEQ&=0xff;
		for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
		{
			ListTask=RMLIST->List[ListNo].TASK;////0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
			if((ListTask>=3)&&(ListTask<=6))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
			{//��Ч
				if(TxSEQ==RMLIST->List[ListNo].TxSEQ)
				{
					break;
				}
			}
		}
		if(ListNo>=NUM_RMLISTmax)
		{
			UARTCtrl->TxSEQ=TxSEQ;
			return;
		}
	}
	UARTCtrl->TxSEQ=TxSEQ;
}

void ConcurrencyRx(u32 PORTn)//���������볬ʱ��ʱ,��ʱ״̬
{
	u32 i;
	u32 ListTask;
	u32 ListNo;
	u16* p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
		if(ListTask==5)
		{//���ڽ���
			i=RMLIST->List[ListNo].TIMER_S;//�����������ͺ�ȴ�������ֶ�ʱ
			if(i==0)
			{
				RMLIST->List[ListNo].TASK=2;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
				if(Terminal_Router->NUM_Enable_Concurrency<5)//����ز���ǰ��������֡��
				{
					Terminal_Router->NUM_Enable_Concurrency++;//����ز���ǰ��������֡��
				}
			}
			else
			{
				UARTCtrl->Task=5;//��״̬���ڽ���
				i*=1000/10;
				if(i>p16timer[0])
				{
					p16timer[0]=i;
				}
			}
		}
	}
	
}

void Uart_3762_AFNF1F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag)//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
{
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	p8tx[0]=0x68;
	p8tx[1]=(29+LEN_Data+2);//���ܷ����ֽ���
	p8tx[2]=(29+LEN_Data+2)>>8;
	if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{
		p8tx[3]=0x43;//������;����ʽ·���ز�ͨ��
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{
			p8tx[3]=0x4a;//������;΢��������ͨ��
		}
		else
		{
			p8tx[3]=0x41;//������;����ʽ·���ز�ͨ��
		}
	}
	p8tx[4]=4;//ͨ��ģ���ʶ=1
	p8tx[5]=0;
	p8tx[6]=LEN_ESC;//Ԥ��Ӧ���ֽ���
	p8tx[7]=0;
	p8tx[8]=0;
	//UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	IncAndCheckTxSEQ(PORTn);//����֡���+1,ͬʱ���֡����Ƿ��뻺���еȴ����յ�֡����Ƿ�����ͬ��������ͬ���ټ�1
	p8tx[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
	//MR(((u32)p8tx)+16,ADDR_Data+1,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
	d64a=UARTCtrl->RMAddr[2];
	d64a<<=16;
	d64a|=UARTCtrl->RMAddr[1];
	d64a<<=16;
	d64a|=UARTCtrl->RMAddr[0];
	d64a=R_Inverse(d64a,6);
	MWR(d64a,((u32)p8tx)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
	
	p8tx[22]=0xF1;//AFN=F1H
	p8tx[23]=0x1;//Fn=1
	p8tx[24]=0;
	p8tx[25]=Protocol;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
	p8tx[26]=0;//����
	p8tx[27]=LEN_Data;//���ĳ���
	p8tx[28]=LEN_Data>>8;
	MR(((u32)p8tx)+29,ADDR_Data,LEN_Data);//��׼645֡

	Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
}

void RM_3762_iActive_HPLC(u32 PORTn)//��������������(����ز���������)
{
	u32 i;
	u32 x;
	u32 ListNo;
	u32 ListTask;
	u32 id;
  UARTCtrl_TypeDef * UARTCtrl;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 SetNo;
	u32 Info;
#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
	u8* pCSD;
	u64 d64a;
#endif
	u32 y;
	u64 d64b;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
		case 0://0=����
			UARTCtrl->Task=6;//3;//��������
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			break;
		case 3://3=��������
			break;
		case 5://5=���ڽ���
			if(UARTCtrl->RxByte!=0)
			{
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				if(p16timer[0]>(1000/10))
				{
					p16timer[0]=1000/10;
				}
				return;
			}
		case 6://6=������(ͬRS485��3=��������)
			//��������
			i=0;
			if(UARTCtrl->ProxyTask>=1)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			{
				i=1;
			}
			//�㲥Уʱ����
			if((UARTCtrl->BroadcastTimeTask&0x7f)>=1)//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
			{
				i=1;
			}
			//�Զ��ѱ�����
			if(UARTCtrl->AutoSearchTask>=1)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
			{
				i=1;
			}
			//���ܱ������ϱ�����
			if(UARTCtrl->AutoReportTask>=1)//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
			{
				i=1;
			}
			ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
			//����������ȴ�,ֹͣ����
//			if(UARTCtrl->Task!=5)
//			{
				if(i)
				{
					UARTCtrl->Task=0;//����
					return;
				}
//			}
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
			i=p16timer[0];
			if(i!=0)
			{
			#if (USER/100)==0//���Ժ����
				if(i>(5000/10))//��֡����������ʱ
				{
					p16timer[0]=5000/10;
				}
//				if(i>(4000/10))
//				{
//					//��״̬
//					Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
//				}
			#else
				//if(i>(1000/10))//��֡����������ʱ
				//{
					//p16timer[0]=1000/10;
				//}
			#endif
				break;
			}
			UARTCtrl->RMMeterCountAuto=0;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
			if(UARTCtrl->RTCS_TASK!=Comm_Ram->DL698YMDHMS[6])//ʵʱ�Ӹ��±�־ԭʼ��(Task����)
			{
				UARTCtrl->RTCS_TASK=Comm_Ram->DL698YMDHMS[6];//ʵʱ�Ӹ��±�־ԭʼ��(Task����)
				TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
			}
			i=UARTCtrl->TaskID;
			if(i==0)//�����ǰû����������
			{
				p16timer[0]=1000/10;//�´μ���TASK()��ʱ;ע��376.2ÿ���յ�1֡����30/10ֵ
				UARTCtrl->Task=5;//���ڽ���
				RM_NUM_Display(PORTn);//����������ʾ
				WarningOccur(PORTn,(u8 *)"�ز� ��ǰ�������");//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
			//�����б�����
			for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
			{
				ListTask=RMLIST->List[ListNo].TASK;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
				if(ListTask<=2)
				{//0=����,1=���1֡����,2=���ճ�ʱ(�����)
					break;
				}
			}
			if(ListNo>=NUM_RMLISTmax)
			{//û�ҵ��ջ����1֡���ջ���ճ�ʱ
				//��ͣ��
				for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
				{
					ListTask=RMLIST->List[ListNo].TASK;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
					if(ListTask==7)
					{
						ListTask=0;
						RMLIST->List[ListNo].TASK=0;//�ñ������0��ʼ
						break;
					}
				}
				if(ListNo>=NUM_RMLISTmax)
				{//ûͣ��
					UARTCtrl->Task=5;//���ڽ���
					return;
				}
			}
			i=Terminal_Router->NUM_Enable_Concurrency;//����ز���ǰ��������֡��
			if(i==0)
			{
				for(i=0;i<NUM_RMLISTmax;i++)
				{
					if(RMLIST->List[i].TASK==5)
					{//���ڵȴ�����
						UARTCtrl->Task=5;//���ڽ���
						return;
					}
				}
				Terminal_Router->NUM_Enable_Concurrency=5;//����ز���ǰ��������֡��
			}
			
			id=UARTCtrl->TaskID;
			id>>=8;
			id&=0xff;
			UARTCtrl->SourMeterCount_id=UARTCtrl->MeterCount[id];//��������ʱ��ǰ����id��ԭʼ������ܱ����
			switch(ListTask)
			{
				case 0://0=����
					Clr_RMListRMCount(ListNo);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ
					MC(0xFF,(u32)&UARTCtrl->RMAddr,6);
					break;
				case 1://1=���1֡����
					break;
				case 2://2=���ճ�ʱ(�����)
					i=AddrToMeterSetNoJS((u16*)&RMLIST->List[ListNo].RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
					//i=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
					
					i=AddrToMeterSetNoJS((u16*)&UARTCtrl->RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
					if(i>=NUM_RMmax)
					{//��Ч
						Clr_RMListRMCount(ListNo);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ
						MC(0xFF,(u32)&UARTCtrl->RMAddr,6);
					}
					else
					{
						UARTCtrl->MeterCount[id]=i;//������ܱ����
						MR((u32)&UARTCtrl->RMAddr,(u32)&RMLIST->List[ListNo].RMAddr,6);
					}
					break;
			}
			Load_RMCount_NoCompAddr(PORTn,ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
			UARTCtrl->PIID=RMLIST->List[ListNo].PIID;//PIID��ֵ�ڵ�ǰ�͸ı�(һ��ʼ��0��ʼ)
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			if(UARTCtrl->ReRMCount>=3)//�����ط�����
			{
				RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
				UARTCtrl-> ReRMCount=0;//�����ط�����
				UARTCtrl->RM_OKFAIL=0;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
				//�µ��ܱ�
				RMLIST->List[ListNo].TASK=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
				Clr_RMListRMCount(ListNo);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ 
				MC(0xFF,(u32)&UARTCtrl->RMAddr,6);
				Load_RMCount_NoCompAddr(PORTn,ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
				UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
				RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
				Find_Next_CollectAddr(PORTn,id);
			}

			if(UARTCtrl->RM_SubsequentFrames==0)//0=�޶�����֡,1-n=����֡��֡��
			{//�Ǻ���֡
				UARTCtrl->RMMeterCountAuto=0;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
			#if(USER/100)==17//����ϵ
				if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
				//if(((UARTCtrl->TaskID>>20)&0xf)==5)
					UARTCtrl->RMprotocol=3;
			#endif
				i=RM_TxDATA_Manage(PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				//ע:RM_TxDATA_Manage(PORTn)����UARTCtrl-> ReRMCount=0
				//Get_ReRMCount(PORTn);//�õ������ط�����,ȡ�뵽UARTCtrl->ReRMCount
				UARTCtrl->ReRMCount++;//�����ط�����
				
				if(UARTCtrl->TxByte==0)
				{//���賭
					RMLIST->List[ListNo].TASK=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					//ע:����Save_RMCount�����Ὣ�ɹ�������Ϊʧ��!!!
					//Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
					UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
					ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
					UARTCtrl->Task=5;//���ڽ���
					//myprintf("[%s:%d]:�ز����͵��ֽ�Ϊ0,����Ҫ����\n",(u32)&__func__,(u32)__LINE__,0);
					return;
				}
				i=CompListRMAddr((u8*)&UARTCtrl->RMAddr);//�����ַ�뻺������Ч�еĳ����ַ�Ƚ��Ƿ�����ͬ;���:p8ָ���ַ(�������ͺ�);����:�б��(��0��ʼ,>=NUM_RMLISTmax��ʾû�ҵ�)
				if(Terminal_Router->MinuteCollect==1)//����ز�֧�ַ��Ӳɼ��Ͳ��Ƚ���һ��
				{
					i=NUM_RMLISTmax;
				}
				if((i<NUM_RMLISTmax)&&(i!=ListNo))
				{//��ǰ��ַԭ���б���Ѵ���,�ҵ�ǰ���б�ź�ԭ�б�Ų�ͬ
					RMLIST->List[ListNo].TASK=7;//7=ͣ��(Ϊ����ִ���¸��б��)
					UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
					ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
					RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
					UARTCtrl->Task=5;//���ڽ���
					return;
				}
				MW((u32)&UARTCtrl->RMAddr,(u32)&RMLIST->List[ListNo].RMAddr,6);
				if(UARTCtrl->RMprotocol==3)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
				{
					i=100;//Ԥ��Ӧ���ֽ���
				}
				else
				{
					i=0;//Ԥ��Ӧ���ֽ���
				}
			#if (USER/100)==0//���Ժ������
				Uart_3762_AFNF1F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			#else
				if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��
				{//������698����͸��
					if(UARTCtrl->RMprotocol==3)
					{
						Uart_3762_AFNF1F1_Tx(PORTn,0,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
					}
					else
					{
						Uart_3762_AFNF1F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
					}
				}
				else
				{
					Uart_3762_AFNF1F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
				}
			#endif
			#if(USER/100)==17//����ϵ
				i=UARTCtrl->TaskID;
				i>>=8;
				i&=0xff;
				if(i==0x22)
				{
					if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
					{
						switch(i)
						{
							case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
								i=UARTCtrl->MeterCount[i];
								i=PowerEvent->setno[i];
								break;
							default:
								i=UARTCtrl->MeterCount[i];
								break;
						}
						if(i>=NUM_RMmax)
						{
							UARTCtrl->Task=5;//���ڽ���
							return;
						}
						x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
						d64a=MRR((u32)p8tx+16,6);
						for(y=0;y<x;y++)
						{
							d64b=MRR((u32)&NoteList->List[y].Addr,6);
							if(d64a==d64b)
							{
								d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
								d64b=R_Inverse(d64b,6);
								MWR(d64b,((u32)p8tx)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
								Uart_3762_CS((u32)p8tx);
								break;//ԭ�Ѵ���
							}
						}
					}
				}
			#endif
			#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
				if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
				{//΢��������,Ŀ���ַ�òɼ�����ַ
					i=UARTCtrl->TaskID;
					i>>=8;
					i&=0xff;
					switch(i)
					{
						case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
							i=UARTCtrl->MeterCount[i];
							i=PowerEvent->setno[i];
							break;
						default:
							i=UARTCtrl->MeterCount[i];
							break;
					}
					if(i>=NUM_RMmax)
					{
						UARTCtrl->Task=5;//���ڽ���
						return;
					}
					d64a=0;
					pCSD=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
					pCSD=Get_Element(pCSD,3,0,0);//����Ԫ�ص�ַ
					if(pCSD)
					{//�ɼ�����ȡ
						d64a=MRR((u32)pCSD+5,6);//ȡ���òɼ�����ַ(Ŀ�ĵ�ַ)
						d64a=R_Inverse(d64a,6);
					}
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8tx)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			#endif
				//Terminal_Router->NUM_Enable_Concurrency--;//����ز���ǰ��������֡��
			}
			else
			{//�Ǻ���֡
				UARTCtrl-> ReRMCount++;//�����ط�����
				//p8tx[0]=0x68;
				p8tx[1]=(29+17+2);//���ܷ����ֽ���
				p8tx[2]=(29+17+2)>>8;
				//p8tx[3]=0x41;//������Ϊ0x41
				//p8tx[4]=4;//ͨ��ģ���ʶ=1
				//p8tx[5]=0;
				p8tx[6]=0;//Ԥ��Ӧ���ֽ���
				//p8tx[7]=0;
				//p8tx[8]=0;
				//UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				IncAndCheckTxSEQ(PORTn);//����֡���+1,ͬʱ���֡����Ƿ��뻺���еȴ����յ�֡����Ƿ�����ͬ��������ͬ���ټ�1
				p8tx[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				//MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
				//MR(((u32)p8tx)+16,ADDR_Data+1,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
				//p8tx[22]=0x13;//AFN=13H
				//p8tx[23]=1;//Fn=1
				//p8tx[24]=0;
				//p8tx[25]=2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
				//p8tx[26]=0;//����
				p8tx[27]=17;//DL645���ĳ���
				p8tx[28]=0;
				p8tx[29+8]=0x12;
				p8tx[29+9]=5;
				p8tx[29+14]=UARTCtrl->RM_SubsequentFrames+0x33;
				p8tx[29+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8tx[29+15]+=p8tx[29+i];
				}
				p8tx[29+16]=0x16;
				Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
				UARTCtrl->TxByte=29+17+2;//�����ֽ���(���ֶ���)
			}
			UARTCtrl->TxByte=p8tx[1]+(p8tx[2]<<8);//�����ֽ���(���ֶ���)
			i=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;//��������
			RM_ADDR_Display(PORTn);//�����ַ��ʾ
			//Comm_Ram->DisplayScanTimer=0;//������ʾ
			i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
			i>>=8;
			i&=0xff;
			RM_Monitor_TxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Tx=��������

			Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
			UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
			RMLIST->List[ListNo].TASK=5;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
			//i=Terminal_Router->MaxOverTime;
			RMLIST->List[ListNo].TIMER_S=120;//�����������ͺ�ȴ����շ��붨ʱ
			RMLIST->List[ListNo].TxSEQ=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
			break;
		case 1://1=�������
			UARTCtrl->Task=0;//����1��
			//myprintf("[%s:%d]:�������֮��Ĵ���\n",(u32)&__func__,(u32)__LINE__,0);
			UARTCtrl->RxByte=0;
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�ز�·���������붨ʱֵ
			
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			
			i=p8rx[1]+(p8rx[2]<<8);
			if(i<=16)
			{//ȷ�Ϸ���
				i=p8rx[10]+(p8rx[11]<<8)+(p8rx[12]<<16);
				if(i==0x0200)
				{//���ϻش�
					i=p8rx[9];//SEQ
					for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
					{
						if(RMLIST->List[ListNo].TxSEQ==i)
						{
							break;
						}
					}
					if(ListNo<NUM_RMLISTmax)
					{//�ҵ�
						Load_RMCount_NoCompAddr(PORTn,ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
						if(UARTCtrl->ReRMCount)//�����ط���������
						{
							UARTCtrl->ReRMCount--;//�����ط���������
						}
						i=p8rx[13];
						switch(i)
						{
					//CCO�˻ظ�����֡��������,�����¿���ԭ��:
							case 109://0x6d
							case 110://0x6e
					//a���������·��ĳ����ĵĲ�����������CCO��������ֵ������ԭ��Ϊ109��������󲢷�����;  
					//b��ÿ����������֡�п��Գ��ض������Э�鱨�ģ����������·������еĵ��Э�鱨������������������ֵ��CCO���ط���֡������ԭ��Ϊ110����������Q/GDW 1376.2֡�������ĵ��Э�鱨��������; 
								Terminal_Router->NUM_Enable_Concurrency=0;//����ز���ǰ��������֡��
								RMLIST->List[ListNo].TASK=1;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
								break;
							case 111://0x6f
								//RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
								//Find_Next_CollectAddr(PORTn,id);
					//c������������ʱ������������ͬһ������������ģ����������·��ڶ��������ĵ�ǰ����Ҫ�ȵ���һ�������ĵĻظ�������CCO�����ط���֡������ԭ��Ϊ111���ñ����ڳ����У�; 
								//RMLIST->List[ListNo].TASK=7;//7=ͣ��(Ϊ����ִ���¸��б��)
								if(Terminal_Router->MinuteCollect==1)
								{
									UARTCtrl->Task=5;//�ز�����һ��
								}
								RMLIST->List[ListNo].TASK=5;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
								//RMLIST->List[ListNo].TIMER_S=240;//�����������ͺ�ȴ������붨ʱ
								break;
							case 9://�ز������������
							default:
					//d����CCO�������Լ�·���Ż���δ���ʱ���������������ʱ���������Ͷ���֡��CCOʱ��CCO�᷵�ط���֡������ԭ��Ϊ9��CCOģ��æ������������ 
								RMLIST->List[ListNo].TASK=1;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
								break;
						}
						Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
					}
				}
				ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬	
				return;
			}
		
			id=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
			id>>=8;
			id&=0xff;
			RM_Monitor_RxNUM(id,1);//�ɼ�������;���:ID=�����,NUM_Rx=��������
			ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬

			if(Terminal_Router->MinuteCollect==0)//���Ƿ��Ӳɼ��Ż�ȥ�ж���һ��
			{
				MR((u32)&UARTCtrl->RMAddr,((u32)p8rx)+10,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
				Data_Inverse((u32)&UARTCtrl->RMAddr,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			}
			ListNo=Get_ListNo((u8*)&UARTCtrl->RMAddr);//�õ������б��;���:�����ַ;����:�б��0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax��ʾ��Ч)
			if(ListNo>=NUM_RMLISTmax)
			{
				ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
				return;
			}
			Load_RMCount_NoCompAddr(PORTn,ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
			
			i=p8rx[22]+(p8rx[23]<<8)+(p8rx[24]<<16);
			if(i!=0x1F1)
			{//AFN=F1H,Fn=1
				//��������
				Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
				ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
				return;
			}
			
			x=p8rx[26]+(p8rx[27]<<8);//���������򳤶�
			if(x==0)
			{//�������ݳ���=0,û�յ�֡Ҳ���س���0
				RMLIST->List[ListNo].TASK=1;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
				Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
				ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
				//i=UARTCtrl->MeterCount[id];
				//i++;
				//UARTCtrl->MeterCount[id]=i;
				//Find_Next_CollectAddr(PORTn,id);
				return;
			}
			Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);
			MR((u32)p8rx,((u32)p8rx)+28,x);
			for(i=0;i<x;i++)
			{//ȥ����֡��ǰ��0xfe
				if(p8rx[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rx,((u32)p8rx)+i,x-i);
					}
					break;
				}
			}
			if(Terminal_Router->MinuteCollect==1)
			{
				i=0;//������֡�ĵ�ַ��������ǿ��ͨ��
			}else
			{	
				i=RM_RxDATA_CHECK(PORTn);//֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			if((i==1)||(i==3))
			{//1��ַ��,3���ݱ�ʶ��
				//��������
				Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
				ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
				return;
			}
			if(i==4)
			{//4�����ϱ�(�Ѵ���)
				//��������
				Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
				ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
				return;
			}
			UARTCtrl->RM_Rx_date_NULL=i;//������շ���:0=������,!=0������(������)
			if(i==2)
			{//2�������(������)
//				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
//				UARTCtrl->ReRMCount=0;//�����ط�����
//				return;
			#if (USER/100)==15//����
				//����:����������Ҫ֧��06000001��061000FF�ȿ鳭��
				if(UARTCtrl->RM_DI==0x06000001)
				{
					//��������
					Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
					ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
					return;
				}
			#endif
				//���ߵȶ������ݵ�ָ��ʱ��������ݶ���ʱд������ݱ��ⲹ��ʱ���³���
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				x=(UARTCtrl->TaskID>>20)&0xf;//�ɼ�����
				if(!((i==0x5002)||(x==4)))
				{//���ǳ����Ӷ���(����������)�Ͳ���(2=�¼��ɼ�����ʱ����������ݶ��ϱ�)
					UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
					UARTCtrl->ReRMCount=0;//�����ط�����
					RMLIST->List[ListNo].TASK=1;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
					Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
					ConcurrencyRx(PORTn);//���������볬ʱ��ʱ,��ʱ״̬
					return;
				}
			}
			UARTCtrl->SourMeterCount_id=UARTCtrl->MeterCount[id];//��������ʱ��ǰ����id��ԭʼ������ܱ����
		
		#if (USER/100)==17//����ϵ
			if(id==0x22)
			{
				if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
				//if(((UARTCtrl->TaskID>>20)&0xf)==5)
					SetNo=AddrToMeterSetNoJS((u16*)&RMLIST->List[ListNo].RMAddr);
				else
					SetNo=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч

			}
			else
				SetNo=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
		#else
			SetNo=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
		#endif	
			
		
			//UARTCtrl->MeterCount[id]=SetNo;
			
//		#if(USER/100)==17//����ϵ
//			pMeterAddrNo=MeterSetAddrNo((u16*)&RMLIST->List[ListNo].RMAddr);
//			
//		#endif
			RM_RxDATA(PORTn);//����������ݴ���
			RM_State_RMUpDate(PORTn,1,SetNo,(Info>>4)&0xf,(Info>>16)&0xf);//�ɼ�״̬,ÿ�γ�������:���:OK=0ʧ��=1�ɹ�,OK=1ʱClass=�м̼���,Phase=��λ
			UARTCtrl->ReRMCount=0;//�����ط�����
		#if(USER/100)==17//����ϵ
			if(UARTCtrl->res23==5)//0=�޶�����֡,1-n=����֡��֡��
			{//�޺���֡
				RM_OK_FAIL(PORTn,1);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
				UARTCtrl-> ReRMCount=0;//�����ط�����
				UARTCtrl->RM_OKFAIL=1;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
				//�µ��ܱ�
				RMLIST->List[ListNo].TASK=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
				Clr_RMListRMCount(ListNo);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ 
				Load_RMCount_NoCompAddr(PORTn,ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
				UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
				RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
				Find_Next_CollectAddr(PORTn,id);//
				UARTCtrl->res23=0;
			}
			else
			{
				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
				RMLIST->List[ListNo].TASK=1;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
				Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
				UARTCtrl->res23=3;//��������8806
				//UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
			}
		#else
			UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
			RMLIST->List[ListNo].TASK=1;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			Save_RMCount(PORTn,ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
			UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
		#endif
			
			RMLIST->List[ListNo].res23=UARTCtrl->res23;
//			RMLIST->List[ListNo].MinuteCollect=UARTCtrl->MinuteCollect;
			RMLIST->List[ListNo].PIID=UARTCtrl->PIID;
			

	}
}

#endif


