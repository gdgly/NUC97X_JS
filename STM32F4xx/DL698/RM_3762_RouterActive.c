

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
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






#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void Uart_3762_AFN14F1_Tx(u32 PORTn,u32 RMFlag)//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
{
	u32 x;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;

	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{
		p8tx[3]=0x03;//������;����ʽ·���ز�ͨ��
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{
			p8tx[3]=0x0a;//������;΢��������ͨ��
		}
		else
		{
			p8tx[3]=0x01;//������;����ʽ·���ز�ͨ��
		}
	}
	p8tx[4]=4;//ͨ��ģ���ʶ=1
	MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
	MR(((u32)p8tx)+16,((u32)p8rx)+14,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
	p8tx[22]=0x14;//AFN=0x14
	p8tx[23]=0x01;//Fn=1
	p8tx[24]=0x00;//Fn=1
	p8tx[25]=RMFlag;//������־=0����ʧ��,=1�����ɹ�,2=����
	p8tx[26]=0;//ͨ����ʱ����Ա�־,0=�޹�,1=�й�
	p8tx[27]=0;//DL645֡����=0
	p8tx[28]=0;//�����ڵ�����N=0
	x=29+2;//���ܷ����ֽ���
	p8tx[1]=x;
	p8tx[2]=x>>8;
	UARTCtrl->TxByte=x;//�����ֽ���(���ֶ���)

//	switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
//	{
//		default://����
//		case 2://����
//			break;
//		case 3://����
//			p8tx[4]=0x0;//ͨ��ģ���ʶ=0
//			MR(((u32)p8tx)+10,((u32)p8tx)+22,7);
//			x=17+2;//���ܷ����ֽ���
//			p8tx[1]=x;
//			p8tx[2]=x>>8;
//			UARTCtrl->TxByte=x;//�����ֽ���(���ֶ���)
//			break;
//	}

	Uart_3762_CS((u32)p8tx);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
	UARTCtrl->Task=3;//�ȴ�����			
	UARTCtrl->TxToRxTimerOver=0;//1000/10//���ͺ���ճ�ʱ��ʱֵ=0,ʹ�ܺܿ쳬ʱ��ת�����Ա�����ת��
}

void Get_ReRMCount(u32 PORTn)//�õ������ط�����,ȡ�뵽UARTCtrl->ReRMCount
{
	u32 i;
	u32 id;
	u32 m;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	RMokfailLIST_TypeDef *rmOKFAILlist;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->ReRMCount=0;//�����ط���������
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return;
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	Addr0=UARTCtrl->RMAddr[0];
	Addr1=UARTCtrl->RMAddr[1];
	Addr2=UARTCtrl->RMAddr[2];
	//��ԭ�Ƿ�ʧ�ܼ�¼
	m=UARTCtrl->NUMrmFAIL[id];//�������Ѳɼ�ʧ�ܵ��ܱ���
	if(m>NUM_RMmax)
	{
		m=NUM_RMmax;
	}
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
	rmOKFAILlist--;
	for(i=0;i<m;i++)
	{
		if(rmOKFAILlist->Addr0==Addr0)
		{
			if(rmOKFAILlist->Addr1==Addr1)
			{
				if(rmOKFAILlist->Addr2==Addr2)
				{
					UARTCtrl->ReRMCount=rmOKFAILlist->ReRMCount;//�����ط���������
					return;
				}
			}
		}
		rmOKFAILlist--;
	}
}

void Save_ReRMCount(u32 PORTn)//���������ط�����,��UARTCtrl->ReRMCountд�뵽ʧ���б�
{
	u32 i;
	u32 id;
	u32 n;
	u32 m;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	RMokfailLIST_TypeDef *rmOKFAILlist;
  UARTCtrl_TypeDef* UARTCtrl;

	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return;
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	Addr0=UARTCtrl->RMAddr[0];
	Addr1=UARTCtrl->RMAddr[1];
	Addr2=UARTCtrl->RMAddr[2];
	n=UARTCtrl->NUMrmOK[id];
	if(n>=NUM_RMmax)
	{
		return;
	}
	m=UARTCtrl->NUMrmFAIL[id];//�������Ѳɼ�ʧ�ܵ��ܱ���
	if((n+m)>NUM_RMmax)
	{
		m=NUM_RMmax-n;//�������Ѳɼ�ʧ�ܵ��ܱ���
	}
	
	
////---test ����ʧ�ܼ����б�----
//	
//	if(UARTCtrl->ReRMCount==1)
//	{
//		id=id;
//	}
//	p8=(u8*)ADDR_Event_Sour;
//	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
//	rmOKFAILlist--;
//	for(i=0;i<m;i++)
//	{
//		p8[0]=rmOKFAILlist->ReRMCount;
//		p8++;
//		rmOKFAILlist--;
//	}
////----test end------------
	
	
	
	//��ԭ�Ƿ�ʧ�ܼ�¼
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
	rmOKFAILlist--;
	for(i=0;i<m;i++)
	{
		if(rmOKFAILlist->Addr0==Addr0)
		{
			if(rmOKFAILlist->Addr1==Addr1)
			{
				if(rmOKFAILlist->Addr2==Addr2)
				{
					rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;//�����ط���������
					return;
				}
			}
		}
		rmOKFAILlist--;
	}
	m++;
	if((n+m)>NUM_RMmax)
	{
		return;
	}
	UARTCtrl->NUMrmFAIL[id]=m;//�������Ѳɼ�ʧ�ܵ��ܱ���
	rmOKFAILlist->Addr0=Addr0;
	rmOKFAILlist->Addr1=Addr1;
	rmOKFAILlist->Addr2=Addr2;
	rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;//�����ط���������

}

void Clr_RMListRMCount(u32 ListNo)//�峭������������;���:ListNo=�б�Ŵ�0��ʼ
{
	RMLIST->List[ListNo].CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	RMLIST->List[ListNo].RMprotocol=0;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	RMLIST->List[ListNo].RMCSDCount=0;//����CSD����
	RMLIST->List[ListNo].RMROADCount=0;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
	RMLIST->List[ListNo].RMCSDSubCount=0;//����CSD�Ӽ���(��DL698����ʱ)
	RMLIST->List[ListNo].RMCSDCountMax=0;//����CSD������
	RMLIST->List[ListNo].RMROADCountMax=0;//����CSD��ROAD���й�����OAD������(��DL698�����Լ���趨)
	RMLIST->List[ListNo].RMCSDSubCountMax=0;//����CSD���ӳ������(��DL698�����Լ���趨)
	RMLIST->List[ListNo].RMROADCountOK=0;//����д�ļ����ݻ�����ROADʱ�����������OAD���ݸ�������
	RMLIST->List[ListNo].TASK=0;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	RMLIST->List[ListNo].TIMER_S=0;//�����������ͺ�ȴ������붨ʱ
	//TxSEQ;//������������֡���
	RMLIST->List[ListNo].RM_OKFAIL=0;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
	RMLIST->List[ListNo].RM_DI_CHOICE=0;//����DIѡ��:0=��DI,1=����DI
	RMLIST->List[ListNo].RM_DI=0;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	RMLIST->List[ListNo].RM_DI_TI_Count=0;//��ʱ�����ɼ�ʱ�ļ������
	RMLIST->List[ListNo].RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
	RMLIST->List[ListNo].RMOADROAD=0;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND

//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������,ȫ������Ϊ����˳��������������)
	RMLIST->List[ListNo].FnDataBuff[6]=0;//����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
	RMLIST->List[ListNo].FnDataBuff[7]=0;
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+0]=0;//����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+1]=0;
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+2]=0;//����=����д��buff����CSD�ĳ���(����������2�ֽ�)
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+3]=0;
}


void ClrALL_RMListRMCount(void)//��ȫ����������������(�ز���)
{
	u32 i;
	for(i=0;i<NUM_RMLISTmax;i++)
	{
		Clr_RMListRMCount(i);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ
	}
}

#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
u32 Get_RMListNO_Tx(u16* p16Addr)//�õ���ǰ�����ַ���б��0-(NUM_RMLISTmax-1),��ԭû�б�ʱ�Զ������б�Ų���ʼ����ؼ���ֵ
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	
	Addr0=p16Addr[0];
	Addr1=p16Addr[1];
	Addr2=p16Addr[2];
	for(i=0;i<NUM_RMLISTmax;i++)
	{
		if(Addr0==RMLIST->List[i].RMAddr[0])
		{
			if(Addr1==RMLIST->List[i].RMAddr[1])
			{
				if(Addr2==RMLIST->List[i].RMAddr[2])
				{
					return i;
				}
			}
		}
	}
	i=Terminal_Router->RMListCount;//�����ַ���б���Զ��������0-(NUM_RMLISTmax-1)
	if(i>=NUM_RMLISTmax)
	{
		i=0;
	}
	Terminal_Router->RMListCount=i+1;//�����ַ���б���Զ��������0-(NUM_RMLISTmax-1)
	Clr_RMListRMCount(i);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ
	RMLIST->List[i].RMAddr[0]=Addr0;
	RMLIST->List[i].RMAddr[1]=Addr1;
	RMLIST->List[i].RMAddr[2]=Addr2;
	return i;
}
u32 Get_RMListNO_Rx(u16* p16Addr)//�õ���ǰ�����ַ���б��0-(NUM_RMLISTmax-1),��ԭû�б�ʱ����:NUM_RMLISTmax��ʾ��Ч
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	
	Addr0=p16Addr[0];
	Addr1=p16Addr[1];
	Addr2=p16Addr[2];
	for(i=0;i<NUM_RMLISTmax;i++)
	{
		if(Addr0==RMLIST->List[i].RMAddr[0])
		{
			if(Addr1==RMLIST->List[i].RMAddr[1])
			{
				if(Addr2==RMLIST->List[i].RMAddr[2])
				{
					return i;
				}
			}
		}
	}
	return NUM_RMLISTmax;//����:NUM_RMLISTmax��ʾ��Ч
}
#endif


void Load_RMCount_NoCompAddr(u32 PORTn,u32 ListNo)//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
{
	u32 i;
	u32 x;
	u8* p8;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->CONNECT=RMLIST->List[ListNo].CONNECT;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	UARTCtrl->RMprotocol=RMLIST->List[ListNo].RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	UARTCtrl->RMCSDCount=RMLIST->List[ListNo].RMCSDCount;//����CSD����
	UARTCtrl->RMROADCount=RMLIST->List[ListNo].RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
	UARTCtrl->RMCSDSubCount=RMLIST->List[ListNo].RMCSDSubCount;//����CSD�Ӽ���(��DL698����ʱ)
	UARTCtrl->RMCSDCountMax=RMLIST->List[ListNo].RMCSDCountMax;//����CSD������
	UARTCtrl->RMROADCountMax=RMLIST->List[ListNo].RMROADCountMax;//����CSD��ROAD���й�����OAD������(��DL698�����Լ���趨)
	UARTCtrl->RMCSDSubCountMax=RMLIST->List[ListNo].RMCSDSubCountMax;//����CSD���ӳ������(��DL698�����Լ���趨)
	UARTCtrl->RMROADCountOK=RMLIST->List[ListNo].RMROADCountOK;//����д�ļ����ݻ�����ROADʱ�����������OAD���ݸ�������
//	RMLIST->List[ListNo].TASK=0;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
//	RMLIST->List[ListNo].TIMER_S=0;//�����������ͺ�ȴ������붨ʱ
	Get_ReRMCount(PORTn);//�õ������ط�����,ȡ�뵽UARTCtrl->ReRMCount
	UARTCtrl->RM_OKFAIL=RMLIST->List[ListNo].RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
	UARTCtrl->RM_DI_CHOICE=RMLIST->List[ListNo].RM_DI_CHOICE;//����DIѡ��:0=��DI,1=����DI
	UARTCtrl->RM_DI=RMLIST->List[ListNo].RM_DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	UARTCtrl->RM_DI_TI_Count=RMLIST->List[ListNo].RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
	UARTCtrl->RM_SubsequentFrames=RMLIST->List[ListNo].RM_SubsequentFrames;//0=�޶�����֡,1-n=����֡��֡��
	UARTCtrl->RMOADROAD=RMLIST->List[ListNo].RMOADROAD;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
	UARTCtrl->res23=RMLIST->List[ListNo].res23;
	//UARTCtrl->MinuteCollect=RMLIST->List[ListNo].MinuteCollect;

	for(i=0;i<8;i++)
	{
		UARTCtrl->RM_Rx_date_time_s[i]=RMLIST->List[ListNo].RM_Rx_date_time_s[i];//������յ�������ʱ��(����������),7byte,��Ϊ���붨��8byte
	}
	for(i=0;i<10;i++)
	{
		UARTCtrl->RM_Rx_date_40020200[i]=RMLIST->List[ListNo].RM_Rx_date_40020200[i];//������յ��������¼����ݴ漰����MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
	}
	for(i=0;i<16;i++)
	{
		UARTCtrl->Rand[i]=RMLIST->List[ListNo].Rand[i];//����ȫģʽʱ���͸�����16�ֽ������
	}
	for(i=0;i<LEN_RMCSD;i++)
	{
		UARTCtrl->RMCSD[i]=RMLIST->List[ListNo].RMCSD[i];//128-x byte ��ǰ���ڳ���CSD(����������)
	}
	
//FnData
	//i=Get_ADDR_UARTnFnDataBuff(PORTn);
	//MR(i,(u32)&RMLIST->List[ListNo].FnDataBuff,LEN_UART4FnDataBuff);
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)
	p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
	i=RMLIST->List[ListNo].FnDataBuff[6];//����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
	i|=RMLIST->List[ListNo].FnDataBuff[7]<<8;
	i+=8;
	if(i>LEN_RM_DI_CSD)
	{
		i=LEN_RM_DI_CSD;
	}
	MR((u32)p8,(u32)&RMLIST->List[ListNo].FnDataBuff,i);
//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������,ȫ������Ϊ����˳��������������)
	i=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD];
	i|=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+1]<<8;
	x=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+2];
	x|=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+3]<<8;
	i+=x;
	i+=4;
	if(i>(LEN_UART4FnDataBuff-LEN_RM_DI_CSD))
	{
		i=LEN_UART4FnDataBuff-LEN_RM_DI_CSD;
	}
	MR((u32)p8+LEN_RM_DI_CSD,(u32)&RMLIST->List[ListNo].FnDataBuff+LEN_RM_DI_CSD,i);
}

void Load_RMCount_CompAddr(u32 PORTn,u32 ListNo)//�õ���������������(�Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
{
	u32 i;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	for(i=0;i<3;i++)
	{
		if(UARTCtrl->RMAddr[i]!=RMLIST->List[ListNo].RMAddr[i])
		{//��ַ�仯
			Clr_RMListRMCount(ListNo);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ
			for(i=0;i<3;i++)
			{
				RMLIST->List[ListNo].RMAddr[i]=UARTCtrl->RMAddr[i];
			}
			break;
		}
	}
	Load_RMCount_NoCompAddr(PORTn,ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
}

void Save_RMCount(u32 PORTn,u32 ListNo)//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
{
	u32 i;
	u32 x;
	u8* p8;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	RMLIST->List[ListNo].CONNECT=UARTCtrl->CONNECT;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	RMLIST->List[ListNo].RMprotocol=UARTCtrl->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	RMLIST->List[ListNo].RMCSDCount=UARTCtrl->RMCSDCount;//����CSD����
	RMLIST->List[ListNo].RMROADCount=UARTCtrl->RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
	RMLIST->List[ListNo].RMCSDSubCount=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���(��DL698����ʱ)
	RMLIST->List[ListNo].RMCSDCountMax=UARTCtrl->RMCSDCountMax;//����CSD������
	RMLIST->List[ListNo].RMROADCountMax=UARTCtrl->RMROADCountMax;//����CSD��ROAD���й�����OAD������(��DL698�����Լ���趨)
	RMLIST->List[ListNo].RMCSDSubCountMax=UARTCtrl->RMCSDSubCountMax;//����CSD���ӳ������(��DL698�����Լ���趨)
	RMLIST->List[ListNo].RMROADCountOK=UARTCtrl->RMROADCountOK;//����д�ļ����ݻ�����ROADʱ�����������OAD���ݸ�������
//	RMLIST->List[ListNo].TASK=0;//��������:0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
//	RMLIST->List[ListNo].TIMER_S=0;//�����������ͺ�ȴ������붨ʱ
	Save_ReRMCount(PORTn);//���������ط�����,��UARTCtrl->ReRMCountд�뵽ʧ���б�
	RMLIST->List[ListNo].RM_OKFAIL=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
	RMLIST->List[ListNo].RM_DI_CHOICE=UARTCtrl->RM_DI_CHOICE;//����DIѡ��:0=��DI,1=����DI
	RMLIST->List[ListNo].RM_DI=UARTCtrl->RM_DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	RMLIST->List[ListNo].RM_DI_TI_Count=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
	RMLIST->List[ListNo].RM_SubsequentFrames=UARTCtrl->RM_SubsequentFrames;//0=�޶�����֡,1-n=����֡��֡��
	RMLIST->List[ListNo].RMOADROAD=UARTCtrl->RMOADROAD;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND

	for(i=0;i<8;i++)
	{
		RMLIST->List[ListNo].RM_Rx_date_time_s[i]=UARTCtrl->RM_Rx_date_time_s[i];//������յ�������ʱ��(����������),7byte,��Ϊ���붨��8byte
	}
	for(i=0;i<10;i++)
	{
		RMLIST->List[ListNo].RM_Rx_date_40020200[i]=UARTCtrl->RM_Rx_date_40020200[i];//������յ��������¼����ݴ漰����MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
	}
	for(i=0;i<16;i++)
	{
		RMLIST->List[ListNo].Rand[i]=UARTCtrl->Rand[i];//����ȫģʽʱ���͸�����16�ֽ������
	}
	for(i=0;i<LEN_RMCSD;i++)
	{
		RMLIST->List[ListNo].RMCSD[i]=UARTCtrl->RMCSD[i];//128-x byte ��ǰ���ڳ���CSD(����������)
	}
	
//FnData
	//i=Get_ADDR_UARTnFnDataBuff(PORTn);
	//MW(i,(u32)&RMLIST->List[ListNo].FnDataBuff,LEN_UART4FnDataBuff);
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)
	p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
	i=p8[6];//����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
	i|=p8[7]<<8;
	i+=8;
	if(i>LEN_RM_DI_CSD)
	{
		i=LEN_RM_DI_CSD;
	}
	MW((u32)p8,(u32)&RMLIST->List[ListNo].FnDataBuff,i);
//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������,ȫ������Ϊ����˳��������������)
	i=p8[LEN_RM_DI_CSD];
	i|=p8[LEN_RM_DI_CSD+1]<<8;
	x=p8[LEN_RM_DI_CSD+2];
	x|=p8[LEN_RM_DI_CSD+3]<<8;
	i+=x;
	i+=4;
	if(i>(LEN_UART4FnDataBuff-LEN_RM_DI_CSD))
	{
		i=LEN_UART4FnDataBuff-LEN_RM_DI_CSD;
	}
	MW((u32)p8+LEN_RM_DI_CSD,(u32)&RMLIST->List[ListNo].FnDataBuff+LEN_RM_DI_CSD,i);
}




void RM_3762_RouterActive(u32 PORTn)//�ز�����(·��ģ�����������ڳ���ģʽ)
{
	u32 i;
	u32 LEN_DATA;
	u32 Info;
	u8* p8rx;
	u8* p8tx;
	u64 d64a;
//	u64 d64b;
	u16* p16timer;
	u32 SetNo;
	u32 RMListNO;
  UARTCtrl_TypeDef* UARTCtrl;
	u32 NUM_SomeTaskID;
	u32 y;
	
	NUM_SomeTaskID=NUM_SomeTaskID;
	y=y;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		case 0://0=����
			UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			UARTCtrl->Task=5;//���ڽ���
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			p16timer[0]=1000/10;//��ʱ��ʱ
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			if(Terminal_Ram->RouterTASK_S_Timer==0)//36 �ز�·�����������ͺ󵽽��ճ�ʱʱ����TASK()��������붨ʱ��
			{
				Terminal_Ram->RouterTASK_S_Timer=10;//36 �ز�·�����������ͺ󵽽��ճ�ʱʱ����TASK()��������붨ʱ��
				UARTCtrl->RMMeterCountAuto=1;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
				TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������;����:0��ʾ�޲ɼ�����
				i=UARTCtrl->TaskID;
				if(i==0)
				{
					RM_NUM_Display(PORTn);//����������ʾ
					WarningOccur(PORTn,(u8 *)"�ز� ��ǰ�������");//�澯����,��ڸ澯����͸澯�ַ���
					return;
				}
				if((i>>28)==0)//���ȼ�=0;�ϵ�����,ÿСʱ���ߵ��õ㳭
				{
					return;
				}
				if((Terminal_Router->RUNState&2)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
				{
					if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
					{
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						Terminal_Router->RouterInitTask=201;//��ʱ·�ɻָ�
					}
				}
			}
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 �ز�·���������붨ʱ��
			{
				Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 1://1=�������
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�ز�·���������붨ʱֵ
			i=p8rx[4];//��ͨ��ģ���ʶ
			i&=0x4;
			if(i==0x0)
			{//������ͨ��ģ��
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				switch(i)
				{
					case 0x1401://AFN=0x14,Fn=1;·�����������󳭱�
						Terminal_Ram->RouterTASK_S_Timer=10;//36 �ز�·�����������ͺ󵽽��ճ�ʱʱ����TASK()��������붨ʱ��
						if(Terminal_Router->RouterCtrl!=0)//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
						{
							UARTCtrl->Task=0;//����ʱ������������
							return;
						}
						i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						if(i)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						{
							UARTCtrl->Task=0;//����ʱ������������
							return;
						}
						//�㲥Уʱ
						i=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
						if((i&0x7f)==1)
						{
							UARTCtrl->Task=0;//����ʱ������������
							return;
						}
						//�Զ��ѱ�
						i=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
						if(i==1)
						{
							UARTCtrl->Task=0;//����ʱ������������
							return;
						}
						//���ܱ������ϱ�
						i=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
						if(i==1)
						{
							UARTCtrl->Task=0;//����ʱ������������
							return;
						}
						MR((u32)p8tx,(u32)p8rx,10);//����10�ֽڽ���ͷ������ͷ
						d64a=MRR(((u32)p8rx)+14,6);//ȡ�ӽڵ��ַ
						d64a=R_Inverse(d64a,6);
						MWR(d64a,(u32)&UARTCtrl->RMAddr,6);//֡���ص�ַ
						SetNo=AddrToMeterSetNoJS((u16*)&UARTCtrl->RMAddr);
						//SetNo=AddrToMeterSetNo((u16*)&UARTCtrl->RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
						if(SetNo>=NUM_RMmax)
						{//û�ҵ�,ֱ�ӷ��س����ɹ�
							Uart_3762_AFN14F1_Tx(PORTn,1);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
							return;
						}
					#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
						RMListNO=Get_RMListNO_Tx((u16*)&UARTCtrl->RMAddr);//�õ���ǰ�����ַ���б��0-(NUM_RMLISTmax-1),��ԭû�б�ʱ�Զ������б�Ų���ʼ����ؼ���ֵ
					#else
						RMListNO=SetNo;
					#endif
						
						Load_RMCount_CompAddr(PORTn,RMListNO);//�õ���������������(�Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
						
						UARTCtrl->RMMeterCountAuto=1;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
						i=UARTCtrl->RM_DI_TI_Count;//����
						TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������;����:0��ʾ�޲ɼ�����
						if(Terminal_Router->RouterCtrl==1)//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
						{
							UARTCtrl->Task=0;//0=����
							return;
						}
						MWR(d64a,(u32)&UARTCtrl->RMAddr,6);//֡���ص�ַ
						if(UARTCtrl->CONNECT!=0)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
						{
							UARTCtrl->RM_DI_TI_Count=i;//�ָ�
						}
						
						i=UARTCtrl->TaskID;
						if(i==0)
						{
							UARTCtrl->Task=0;//����
							RM_NUM_Display(PORTn);//����������ʾ
							WarningOccur(PORTn,(u8 *)"�ز� ��ǰ�������");//�澯����,��ڸ澯����͸澯�ַ���
							return;
						}
						if((i>>28)==0)//���ȼ�=0;�ϵ�����,ÿСʱ���ߵ��õ㳭
						{
							return;
						}
						i>>=8;
						i&=0xff;
						if(i>=(RMTASKmax+ExtRMTASKmax))
						{
							UARTCtrl->Task=0;
							return;
						}
	
						UARTCtrl->MeterCount[i]=SetNo;//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
						
					#if RMovertimeACKfail==0//�ز�·���������󳭱����������ʧ��ʱ:0=����������֡,1=���س���ʧ��(����)
						i=UARTCtrl->ReRMCount;
						if(i>=1)//�����ط�����;·������ʱÿ������2�κ���ܾ��Զ�������,���ظ�����>=2ʱ������ʧ��!!!
						{
							if(i==0xff)
							{
								UARTCtrl->ReRMCount=0xfe;
							}
							RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
//							if(UARTCtrl->ReRMCount>=5)//�����ط�����;·������ʱÿ������2�κ���ܾ��Զ�������,���ظ�����>=2ʱ������ʧ��!!!
//							{
//								//Save_RMCount(PORTn,RMListNO);//���泭������������;���:SetNo=�����Ŵ�0��ʼ
//								Uart_3762_AFN14F1_Tx(PORTn,0);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
//								UARTCtrl->RMAddr[0]=0xffff;
//								UARTCtrl->RMAddr[1]=0xffff;
//								UARTCtrl->RMAddr[2]=0xffff;
//								return;
//							}
						}
					#else
						if(UARTCtrl->ReRMCount>=1)//�����ط�����;·������ʱÿ������2�κ���ܾ��Զ�������,���ظ�����>=2ʱ������ʧ��!!!
						{
							RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
							if(UARTCtrl->ReRMCount>=3)//�����ط�����;·������ʱÿ������2�κ���ܾ��Զ�������,���ظ�����>=2ʱ������ʧ��!!!
							{
								//Save_RMCount(PORTn,RMListNO);//���泭������������;���:SetNo=�����Ŵ�0��ʼ
								Uart_3762_AFN14F1_Tx(PORTn,0);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
								UARTCtrl->RMAddr[0]=0xffff;
								UARTCtrl->RMAddr[1]=0xffff;
								UARTCtrl->RMAddr[2]=0xffff;
								return;
							}
						}
					#endif
/*
						if(UARTCtrl->ReRMCount>=2)//�����ط�����;·������ʱÿ������2�κ���ܾ��Զ�������,���ظ�����>=2ʱ������ʧ��!!!
						{
						#if RMovertimeACKfail==0//�ز�·���������󳭱����������ʧ��ʱ:0=����������֡,1=���س���ʧ��(����)
							if(UARTCtrl->ReRMCount==0xff)
							{
								UARTCtrl->ReRMCount=0xfe;
							}
							RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
						#else
							//������ע��Ҫͬvoid CLR_RMCount(u32 PORTn)//��0������֡������ֵ
							UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������
							UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
							UARTCtrl->RMCSDCount=0;//����CSD����
							UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
							UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����(��DL698����ʱ)
							UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
							UARTCtrl->ReRMCount=0;//�����ط�����
							UARTCtrl->RMErr=0;//��������־
							UARTCtrl->RM_DI_TI_Count=0;//��ʱ�����ɼ�ʱ�ļ������
							UARTCtrl->RMOADROAD=0;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
							
							RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
							Save_RMCount(PORTn,RMListNO);//���泭������������;���:SetNo=�����Ŵ�0��ʼ
							Uart_3762_AFN14F1_Tx(PORTn,0);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
							UARTCtrl->RMAddr[0]=0;
							UARTCtrl->RMAddr[1]=0;
							UARTCtrl->RMAddr[2]=0;
							return;
						#endif
						}
*/
						UARTCtrl->RMMeterCountAuto=1;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
						i=RM_TxDATA_Manage(PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						//ע:RM_TxDATA_Manage(PORTn)����UARTCtrl-> ReRMCount=0!!!
						Get_ReRMCount(PORTn);//�õ������ط�����,ȡ�뵽UARTCtrl->ReRMCount
						UARTCtrl->ReRMCount++;//�����ط�����
						if(i==2)
						{//
							if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{
								p8tx[3]=0x03;//������;����ʽ·���ز�ͨ��
							}
							else
							{
								if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
								{
									p8tx[3]=0x0a;//������;΢��������ͨ��
								}
								else
								{
									p8tx[3]=0x01;//������;����ʽ·���ز�ͨ��
								}
							}
							p8tx[4]=4;//ͨ��ģ���ʶ=1
							if(UARTCtrl->RMprotocol==3)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
							{
								p8tx[6]=100;//Ԥ��Ӧ���ֽ���
							}
							else
							{
								p8tx[6]=0;//Ԥ��Ӧ���ֽ���
							}
							MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
							d64a=UARTCtrl->RMAddr[2];
							d64a<<=16;
							d64a|=UARTCtrl->RMAddr[1];
							d64a<<=16;
							d64a|=UARTCtrl->RMAddr[0];
							d64a=R_Inverse(d64a,6);
							MWR(d64a,((u32)p8tx)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
							p8tx[22]=0x14;//AFN=0x14
							p8tx[23]=0x01;//Fn=1
							p8tx[24]=0x00;//Fn=1
							p8tx[25]=2;//������־=2
							p8tx[26]=0;//ͨ����ʱ����Ա�־,0=�޹�,1=�й�
							i=UARTCtrl->TxByte;
							p8tx[27]=i;//DL645֡����
							MR((u32)p8tx+28,(u32)p8tx+128,i);
							p8tx[28+i]=0;//�����ڵ�����N=0
							i+=29+2;//���ܷ����ֽ���
							p8tx[1]=i;
							p8tx[2]=i>>8;
							UARTCtrl->TxByte=i;//�����ֽ���(���ֶ���)

							Uart_3762_CS((u32)p8tx);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
							UARTCtrl->Task=3;//�ȴ�����
							UARTCtrl->TxToRxTimerOver=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
							Terminal_Ram->RouterTASK_S_Timer=60;//36 �ز�·�����������ͺ󵽽��ճ�ʱʱ����TASK()��������붨ʱ��
							
							RM_ADDR_Display(PORTn);//�����ַ��ʾ
							i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
							i>>=8;
							i&=0xff;
							RM_Monitor_TxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Tx=��������
							
							Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
						}
						else
						{//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
							if(i<=1)
							{
								Uart_3762_AFN14F1_Tx(PORTn,1);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
								//�ɹ����建���еĳ����ַ;���´γ���ʱ�ҵ����ݳ������û��0
								RMLIST->List[RMListNO].RMAddr[0]=0xffff;
								RMLIST->List[RMListNO].RMAddr[1]=0xffff;
								RMLIST->List[RMListNO].RMAddr[2]=0xffff;
							}
							else
							{//>=3;3=ʧ��,4=������ͣ
								Uart_3762_AFN14F1_Tx(PORTn,0);//·�������󳭱���֡;���:RMFlag������־=0����ʧ��,=1�����ɹ�,2=����
							}
							RM_NUM_Display(PORTn);//����������ʾ
						}
						break;
					case 0x0602://AFN=0x06,Fn=2;·���������ϱ���������
						//����Ȳ��������ϱ��������ݴӴ˷���
						MR((u32)p8tx,(u32)p8rx,10);//����10�ֽڽ���ͷ������ͷ
						Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);//ȡ6�ֽ�376.2���ص���Ϣ��
						i=p8rx[16]+(p8rx[17]<<8);//��·�ϴ�����ʱʱ����
						UARTCtrl->TransferDelay=i;
						LEN_DATA=p8rx[18];//��DL645֡����
						MR(((u32)p8rx),((u32)p8rx)+19,LEN_DATA);//ȡ645֡
						goto RMESC;//������
					case 0x1402://AFN=0x14,Fn=2;·����������ʱ��
						MR((u32)p8tx,(u32)p8rx,10);//����10�ֽڽ���ͷ������ͷ
						MW(ADDR_MYMDHMS,((u32)p8tx)+128,6);
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
				i=p8rx[22]+(p8rx[23]<<8)+(p8rx[24]<<16);
				switch(i)
				{
					case 0x000206://AFN=0x06,Fn=2;·���������ϱ���������
						//���ŵȲ������ϱ��������ݴӴ˷���
						MR((u32)p8tx,(u32)p8rx,10);//����10�ֽڽ���ͷ������ͷ
						Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);
						i=p8rx[28]+(p8rx[29]<<8);//��·�ϴ�����ʱʱ����
						UARTCtrl->TransferDelay=i;
						LEN_DATA=p8rx[30];//��DL645֡����
						d64a=MRR(((u32)p8rx)+10,6);//ȡ�ӽڵ��ַ
						d64a=R_Inverse(d64a,6);
						MWR(d64a,(u32)&UARTCtrl->RMAddr,6);//֡���ص�ַ
						MR(((u32)p8rx),((u32)p8rx)+31,LEN_DATA);//ȡ645֡
					
				RMESC://������
						SetNo=AddrToMeterSetNoJS((u16*)&UARTCtrl->RMAddr);
						//SetNo=AddrToMeterSetNo((u16*)&UARTCtrl->RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
						if(SetNo>=NUM_RMmax)
						{//û�ҵ�,ֱ�ӷ��س����ɹ�
							goto RXESC;
						}
					#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
						NUM_SomeTaskID=UARTCtrl->NUM_SomeTaskID;//��ǰͬ��ɼ������id�ŵ��б����
						y=0;
						for(i=0;i<NUM_SomeTaskID;i++)
						{
							y=UARTCtrl->TaskID;
							y&=0xffff0000;
							y|=UARTCtrl->SomeTaskid[i];//RMTASKmax*2 byte ��ǰͬ��ɼ������b15-b8�����,b7-b0�������
							UARTCtrl->TaskID=y;
							y>>=8;
							y&=0xff;//�����
							if(y>=(RMTASKmax+ExtRMTASKmax))
							{
								continue;
							}
							UARTCtrl->MeterCount[y]=SetNo;//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
							y=RM_Meter(PORTn);//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����,
							if(y)
							{
								break;
							}
						}
						if(y==0)
						{//û�ҵ�
							goto RXESC;
						}
					#endif
						i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
						i>>=8;
						i&=0xff;
						if(i>=(RMTASKmax+ExtRMTASKmax))
						{
							goto RXESC;
						}
						RM_Monitor_RxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Rx=��������
						UARTCtrl->MeterCount[i]=SetNo;//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
						
					#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
						RMListNO=Get_RMListNO_Rx((u16*)&UARTCtrl->RMAddr);//�õ���ǰ�����ַ���б��0-(NUM_RMLISTmax-1),��ԭû�б�ʱ����:NUM_RMLISTmax��ʾ��Ч
						if(RMListNO>=NUM_RMLISTmax)
						{//û�ҵ�
							goto RXESC;
						}
					#else
						RMListNO=SetNo;
					#endif
						
						Load_RMCount_CompAddr(PORTn,RMListNO);//�õ���������������(�Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
						for(i=0;i<LEN_DATA;i++)
						{//ȥ����645֡��ǰ��0xfe
							if(p8rx[i]!=0xfe)
							{
								if(i!=0)
								{
									MR((u32)p8rx,((u32)p8rx)+i,LEN_DATA-i);
								}
								break;
							}
						}
						i=RM_RxDATA_CHECK(PORTn);//֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
						if((i==1)||(i==3))
						{//1��ַ��,3���ݱ�ʶ��
							//UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
							Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
							goto RXESC;
						}
						if(i==4)
						{//4�����ϱ�(�Ѵ���)
							Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
							goto RXESC;
						}
						UARTCtrl->RM_Rx_date_NULL=i;//������շ���:0=������,!=0������(������)
						if(i==2)
						{//2�������(������)
//							UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
//							UARTCtrl->ReRMCount=0;//�����ط�����
//							Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
//							goto RXESC;
						#if (USER/100)==15//����
							//����:����������Ҫ֧��06000001��061000FF�ȿ鳭��
							if(UARTCtrl->RM_DI==0x06000001)
							{
								//UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
								Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
								goto RXESC;
							}
						#endif
							//���ߵȶ������ݵ�ָ��ʱ��������ݶ���ʱд������ݱ��ⲹ��ʱ���³���
							i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
							Info=(UARTCtrl->TaskID>>20)&0xf;//�ɼ�����
							if(!((i==0x5002)||(Info==4)))
							{//���ǳ����Ӷ���(����������)�Ͳ���(2=�¼��ɼ�����ʱ����������ݶ��ϱ�)
								UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
								UARTCtrl->ReRMCount=0;//�����ط�����
								Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
								goto RXESC;
							}
						}
						RM_RxDATA(PORTn);//����������ݴ���
						RM_State_RMUpDate(PORTn,1,SetNo,(Info>>4)&0xf,(Info>>16)&0xf);//�ɼ�״̬,ÿ�γ�������:���:OK=0ʧ��=1�ɹ�,OK=1ʱClass=�м̼���,Phase=��λ
						
						UARTCtrl->ReRMCount=0;//�����ط�����
						UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
						Save_RMCount(PORTn,RMListNO);//�õ���������������;���:SetNo=�����Ŵ�0��ʼ
						//��ȷ�ϻش��ϱ���������
					RXESC:
						MWR(0xffffffff,((u32)p8tx)+128,6);
						Uart_3762_TxData(PORTn,0,1,6);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
						if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
						{
							p8tx[3]=0x03;//������;����ʽ·���ز�ͨ��
						}
						else
						{
							if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{
								p8tx[3]=0x0a;//������;΢��������ͨ��
							}
							else
							{
								p8tx[3]=0x01;//������;����ʽ·���ز�ͨ��
							}
						}
						Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
						break;
					default:
						UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
						UARTCtrl->Task=5;//���ڽ���
						break;
				}
			}
			return;
	}
}
#endif


