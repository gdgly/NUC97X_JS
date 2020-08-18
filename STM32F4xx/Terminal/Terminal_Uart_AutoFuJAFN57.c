
//�����ϱ���Ҫ�¼�
#include "Project.h"
#include "Terminal_Uart_AutoFuJAFN57.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_Uart_AutoFuJAFN57.h"
#include "Terminal_AFN56_SourceData.h"
#include "Terminal_Uart_AFN54_Paramter.h"
#include "FuJCollectTask.h"


#if (USER/100)==14//����ϵ
#if ComTest==1 //����232����485��������ݵ��Գ��򿪹أ�0==�ر�;1==����
void LogOut_RS232(u32 PORTn,u32 ADDR_LOGDATA,u32 LEN_Data,u32 Ascii);
#endif
void Terminal_Uart_AutoFuJAFN57(void)//�����ϱ���Ҫ�¼�
{//˫ͨ���ϱ��������⣬��ʱ�����д���
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UARTCtrl->LinkTask>=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		Terminal_AutoFuJAFN57(GPRSPORT);//�����ϱ���Ҫ�¼�,���UART�˿ں�
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask>=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		Terminal_AutoFuJAFN57(ENETPORT);//�����ϱ���Ҫ�¼�,���UART�˿ں�
	}
#if RS232AutoRePort==1//RS232�������ϱ�0=��ֹ,1=����
	Terminal_AutoFuJAFN57(RS232PORT);//�����ϱ���Ҫ�¼�,���UART�˿ں�
#endif
}


const u8 AutoFuJTaskData_57Example[]=
{
	0x68,
	16,0,
	16,0,
	0x68,
	0xc0,//������(DIR=1����,������־λPRM=1,��������4=���ͨM�޻ش�,����Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1)10=����M��Ӧ)
	0,0,//����������
	0,0,//�ն˵�ַ
	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	0x57,//AFN=0E
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON��=0
	0,0,//Pn=0
	0x01,0,//Fn=1�������������ϱ�
	0,//�����ϱ��ľ�����������
	//...
	//cs
	//0x16
};
const u8 AutoFuJEventData_57Example[]=
{
	0x68,
	16,0,
	16,0,
	0x68,
	0xc0,//������(DIR=1����,������־λPRM=1,��������4=���ͨM�޻ش�,����Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1)10=����M��Ӧ)
	0,0,//����������
	0,0,//�ն˵�ַ
	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	0x57,//AFN=0E
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON��=0
	0,0,//Pn=0
	0x02,0,//Fn=2�¼������ϱ�
	0,//�ϱ����
	//...
	//cs
	//0x16
};

//�������������ϱ�״̬ȷ��
//u32 Flag: 0-ȷ�� 1-��ʱ
void FuJTaskReportACK(u32 Flag)
{
	u32 i;
//	u32 ID;
	u32 ReportFlag;
	ReportFlag = MRR(ADDR_AFN54F10+2*(FuJTaskReportCtrl->Task-1),1);
	if(ReportFlag == 0xFF)
		ReportFlag = 0;
	if(((ReportFlag>>4)&0x07) >= 3)//�����ٲ�
	{
		if(Flag != 0)
		{//��վδȷ��,�ȴ������ٲ�
			Flag = 0xFF;
		}
		else
		{
			Flag = 0;
		}
	}
	else
	{//�����ϱ�������
		if(Flag != 0)//��վδ�ظ���������ϱ�״̬
			return;
//		Flag = 0;//������վ�Ƿ������ظ��������ϱ���־λ
	}
	for(i = 0;i < FuJTaskReportCtrl->Num;i ++)
	{
//		ID = GetIDBySubTask(FuJTaskReportCtrl->Task,u32 SubTask);
//		if(IsFuJSubTaskRight(FuJTaskReportCtrl->Task,FuJTaskReportCtrl->SubTaskList[i]))
		{
			ClearFuJDataReportFlag(FuJTaskReportCtrl->Task,FuJTaskReportCtrl->SubTaskIDList[i],FuJTaskReportCtrl->CurRound,Flag);
			FuJTaskReportCtrl->SubTaskID = FuJTaskReportCtrl->SubTaskIDList[i];
		}
	}
}
u32 GetReportFuJTask(void)
{
	u32 i;
	u32 Task;
	u32 Prio;
	Task = 0;
	Prio = 255;
	for(i = 0;i < MAX_FUJ_TASK;i ++)
	{
		if(FuJClloectTask[i].State != FUJTASK_DISABLE_FLAG)
		{//����Ч����
			if((0 < FuJClloectTask[i].Prio) && (FuJClloectTask[i].Prio < Prio))
			{
				if(FuJClloectTask[i].ReportFlag == 1)
				{//�����ϱ�
					Task = i+1;
					Prio = FuJClloectTask[i].Prio;
				}
				else if(FuJClloectTask[i].ReportFlag == 2)
				{//�ȴ������ٲ�
					if(FuJTaskReportCtrl->AutoPackFlag == 2)
					{//�����ٲ�������
						Task = i+1;
						Prio = FuJClloectTask[i].Prio;
					}
				}
			}
		}
	}
	if(Task == 0)
	{
		FuJTaskReportCtrl->AutoPackFlag = 0;//���ٲ�����
	}
	return Task;
}
void ForbidReportFlag(void)
{
	u32 y;
	FuJTaskReportCtrl->AutoPackFlag = 0;
	for(y = 0;y < MAX_FUJ_TASK;y ++)
	{
		if(FuJClloectTask[y].State != FUJTASK_DISABLE_FLAG)
		{//����Ч����
			if(FuJClloectTask[y].ReportFlag == 2)
			{
				FuJClloectTask[y].ReportFlag = 3;
			}
			else
				FuJClloectTask[y].ReportFlag = 0;
		}
	}
}
void Terminal_AutoFuJAFN57(u32 PORTn)//�����ϱ���Ҫ�¼�,���UART�˿ں�
{
	u32 i;
	u32 x;
	u32 y;
	u32 Len;
	u32 m;
	u32 n;
	u32 k;
//	u8 * p8;
	u16 * p16;
	FuJSubTaskDataHead_TypeDef *DataHead;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u32 LEN_UARTnTx;
	
	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
//	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	LEN_UARTnTx = 800;//�����ϱ���800�ֽ�Ϊ����
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
	{
		goto LoopChack;//����Ѳ�������ϱ�
	}
	if(UARTCtrl->Task==0x0)
	{//����
		p16=(u16 *)Get_ADDR_UARTnTimer(PORTn);
		if(p16[0]==0x0)
		{
			if(PORTn==RS232PORT)
			{
				goto LoopChack;
			}
			if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{//����
LoopChack:
				if(PORTn==GPRSPORT)
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
				}
				if(PORTn==ENETPORT)
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
				}
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->RS232_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
				}
				
				if(UARTCtrl->HostACK!=0x0)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
				{	
					if((UARTCtrl->HostACK!=7) && (UARTCtrl->HostACK!=8))//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
					{//�����������ϱ��ȴ�ȷ�Ϸ���
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//�ȴ���վȷ���붨ʱ��!=0
					}
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
					i=MRR(ADDR_AFN04F1+3,1);
					i>>=4;
					i&=0x3;
					if(UARTCtrl->HostCount>i)
					{//�ط�������
						UARTCtrl->HostCount=0;
						//�Ա�־λ�Ĵ���
						if(UARTCtrl->HostACK==7)
							FuJTaskReportACK(1);//��ʱ
						if(UARTCtrl->HostACK==8)
							ClearFuJEventLastRecord();
					}
				}
				i = MRR(ADDR_AFN54F22,1);
				if((i&0x03) == 2)
//				if(MRR(ADDR_AFN05F29,1)!=0x55)
				{//�������ն������ϱ���������
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
					UARTCtrl->HostCount=0;
					return;
				}
				//����֡����
				if(PORTn==GPRSPORT)
				{
					if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//5 GPRS�����붨ʱ��
					{
						return;
					}
				}
				if(PORTn==ENETPORT)
				{
					if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//2 ��̫�������붨ʱ��
					{
						return;
					}
				}
				//�¼��ϱ�
				x = FuJEventLastRecordRead(ADDR_DATABUFF+1024);
				if(x != 0)
				{//�����ݵȴ��ϱ�
					//�ظ��ϱ��ж�
					n = MRR(ADDR_DATABUFF+1024,4);//���
					if(FuJTaskReportCtrl->EventIndex != n)
					{
						UARTCtrl->HostCount = 0;//��Ų�һ�£��״��ϱ�
						FuJTaskReportCtrl->EventIndex = n;
					}
					//��֡
					MR((u32)p8txbuff,(u32)AutoFuJEventData_57Example,19);//
					MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
					if(UARTCtrl->HostCount==0)
					{//�����ظ�֡
						p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
						UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����ʱ��)
					}
					else
					{//���ظ�֡
						p8txbuff[13]|=UARTCtrl->ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
					}
					Len=13;
					p8txbuff[18] = n;//�ϱ��¼����
					MR(((u32)p8txbuff)+19,ADDR_DATABUFF+1024+4,x-1);
					UARTCtrl->HostACK=8;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�,8-�����¼��ϱ�
					goto ReportFrame;
				}
				//�ϴ�
				if(FuJTaskReportCtrl->CurRound > FuJTaskReportCtrl->MaxRound)
				{//Ѱ����һ�����񷽰�
					if((FuJTaskReportCtrl->Task > 0) && (FuJTaskReportCtrl->Task <= MAX_FUJ_TASK))
					{//��������Ϊ���ϱ����
						y = MRR(ADDR_AFN54F10+2*(FuJTaskReportCtrl->Task-1),1);//�ϱ���־
						if(y == 0xFF)
							y = 0;
						if(((y>>4)&0x07) >= 3)
							FuJClloectTask[FuJTaskReportCtrl->Task-1].ReportFlag = 3;//�ϱ����
						else
							FuJClloectTask[FuJTaskReportCtrl->Task-1].ReportFlag = 0;//�ϱ����
					}
					y = GetReportFuJTask();//��ȡ���ϱ������񷽰�
					if((y == 0) || (y > MAX_FUJ_TASK))
					{//������ȴ��ϱ�
						FuJTaskReportCtrl->Task = 0;
						FuJTaskReportCtrl->CurRound = 0xFFFFFFFF;
						return;
					}
					m = MRR(ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+6,1);//Unit
					n = MRR(ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+5,1);//Num
//					MR(ADDR_DATABUFF+12,ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+11,6);//BaseTime
					if((m == 0) || (n == 0))
					{//ִֻ��һ�ε�����
						x = 0;
						m = 0;
						MR(ADDR_DATABUFF,ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+11,6);//����ʼʱ��
					}
					else
					{
						x = StructuredTimeToPriod(ADDR_TYMDHMS,ADDR_DATABUFF,m,n);//�ִ�--����ʱ��
						if(x == ERROR_RET)
						{
							//������ǰ�ϱ�
							FuJTaskReportCtrl->MaxRound = 0;
							FuJTaskReportCtrl->CurRound = 0xFFFFFFFF;
							return;
						}
						m = MRR(ADDR_AFN54F5+(y-1)*LEN_AFN54F5_Pn+10,1);//Depth
					}
					FuJTaskReportCtrl->Task = y;
					FuJTaskReportCtrl->MaxRound = x;//����ִ�
					if(x < m)
						m = x;
					if(FuJTaskReportCtrl->AutoPackFlag == 2)
						FuJTaskReportCtrl->CurRound = x-m;
					else
						FuJTaskReportCtrl->CurRound = x;
					FuJTaskReportCtrl->SubTaskID = 0;//��0��ʼ���Ҿ�������
				}
				if((FuJTaskReportCtrl->Task < 1) && (FuJTaskReportCtrl->Task > MAX_FUJ_TASK))
					return;
				k = MRR(ADDR_AFN54F10+2*(FuJTaskReportCtrl->Task-1),1);//�ϱ���־
				if(k == 0xFF)
					k = 0;
				if((k&0x80) == 0)
				{//�������ϱ�
					//������ǰ����ŵ������ϱ�
					FuJTaskReportCtrl->CurRound = 0xFFFFFFFF;
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
					UARTCtrl->HostCount=0;//�ط�����
					return;
				}
				MR((u32)p8txbuff,(u32)AutoFuJTaskData_57Example,19);//
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
				if(UARTCtrl->HostCount==0)
				{//�����ظ�֡
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����ʱ��)
				}
				else
				{//���ظ�֡
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
				}
//				p8txbuff[20]=i;//��֡���Ĵ��͵��¼���¼��ʼָ��Pm
//				p8txbuff[21]=i;//��֡���Ĵ��͵��¼���¼����ָ��Pn
//				p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��
				Len=13;
				//��֡
//				p8=(u8 *)(ADDR_DATABUFF);
				DataHead = (FuJSubTaskDataHead_TypeDef *)(ADDR_DATABUFF);
				n = 0;//����֡�ľ���������
				m = FuJTaskReportCtrl->SubTaskID;//��������洢���
				for(x = 0;x < MAX_PER_SUB_TASK;x ++)
				{
					if(UARTCtrl->HostCount == 0)
					{//�״��ϱ�
						if(n >= 15)
							break;
						m = GetNextSubTaskID(FuJTaskReportCtrl->Task,m);//��������洢���
						FuJTaskReportCtrl->SubTaskIDList[n] = m;
					}
					else
					{//�ط�
						if(n >= FuJTaskReportCtrl->Num)
							break;
						if(m == FuJTaskReportCtrl->SubTaskIDList[n])
						{
							n ++;//�о��������Ѷ�ȡ��������
							continue;
						}
						m = FuJTaskReportCtrl->SubTaskIDList[n];//���������
					}
					if((m == 0) || (m > MAX_FUJ_SUB_TASK))
					{//�쳣m
						break;
					}
					//��ȡ����
					if(FuJTaskReportCtrl->AutoPackFlag == 2)//�����ϱ�ʱ�����ж��κ����ݣ�ֻҪFLASH�����Ŀ��ϱ������ݾ��ϱ�����վ
						y = ReadFuJTaskData(ADDR_DATABUFF,FuJTaskReportCtrl->Task,m,FuJTaskReportCtrl->CurRound,ADDR_TYMDHMS,0x23);//����ʱ�ֻ꣬���ִ�
					else
						y = ReadFuJTaskData(ADDR_DATABUFF,FuJTaskReportCtrl->Task,m,FuJTaskReportCtrl->CurRound,ADDR_TYMDHMS,0x22);//����ʱ�ֻ꣬���ִ�
					if((FuJTaskReportCtrl->Task == 0x03FD)&&(m==0x1D))
					{
						m = 0x1D;
					}
					if((y == 0) || (y > 288))//��ȡʧ��
						continue;
					if(DataHead->ReportFlag == 0)//�����ϱ�
						continue;
					if(FuJTaskReportCtrl->AutoPackFlag != 2)
					{//�����������ٲ�
						if(k&0x10)//ֻ�ϱ������ɹ���
						{
							if(DataHead->Len <= 6)
								continue;
						}
					}//else �����ٲ�ʱ�����۳ɹ�ʧ�ܣ���Ҫ�ϱ�
					if((Len+6+2+(25+DataHead->Len))>=LEN_UARTnTx)
					{
						break;
					}
					n ++;
					//�������
					MWR(FuJTaskReportCtrl->Task,(u32)(p8txbuff+Len+6),2);//���񷽰���
					Len += 2;
					MR((u32)(p8txbuff+Len+6),ADDR_AFN54F8+(m-1)*LEN_AFN54F8_Pn+2,2);//�����������
					Len += 2;
					MR((u32)(p8txbuff+Len+6),(u32)DataHead->MeterAddr,6);//�������ͨ�ŵ�ַ
					Len += 6;
					if(DataHead->Len == 0)
					{
						MC(0,(u32)(p8txbuff+Len+6),1);//������
					}
					else
					{
						MC(1,(u32)(p8txbuff+Len+6),1);//������
					}
					Len ++;
					MR((u32)(p8txbuff+Len+6),(u32)DataHead->STime,6);//����ʱ��
					Len += 6;
					MR((u32)(p8txbuff+Len+6),(u32)DataHead->RTime,6);//ʵ��ʱ��
					Len += 6;
					MC(DataHead->ReadTimes,(u32)(p8txbuff+Len+6),1);//���Դ���
					Len ++;
					if(DataHead->Len == 0)
					{
						MC(1,(u32)(p8txbuff+Len+6),1);//���ĳ���
						Len ++;
						MC(11,(u32)(p8txbuff+Len+6),1);//����-ERR��Ӧ��ʱ
						Len ++;
					}
					else
					{
						MC(DataHead->Len,(u32)(p8txbuff+Len+6),1);//���ĳ���
						Len ++;
						MR((u32)(p8txbuff+Len+6),ADDR_DATABUFF+LEN_FuJSubTaskDataHead,DataHead->Len);//����
						Len += DataHead->Len;
					}
				}
				if(n == 0)
				{//��ǰ�ִν�����������һ�ִε������ϱ�
					FuJTaskReportCtrl->CurRound++;
					FuJTaskReportCtrl->SubTaskID = 0;//��0��ʼ���Ҿ�������
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
					UARTCtrl->HostCount=0;//�ط�����
					return;
				}
				FuJTaskReportCtrl->Num = n;
				p8txbuff[18] = n;//�ϱ�������
				UARTCtrl->HostACK=7;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�,8-�����¼��ϱ�
				
			
ReportFrame:
				p8txbuff[1]=Len;
				p8txbuff[2]=Len>>8;
//				if((MRR(ADDR_AFN04F1+4,1)&0x04)!=0x0)
				{//��Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1���ı�־
					p8txbuff[6]|=10;//������10=����M��Ӧ֡(����1������)
					p8txbuff[13]|=0x10;//CON=1
					UARTCtrl->HostCount+=1;//�ط�����
				}
//				else
//				{
//					p8txbuff[6]|=4;//������4=���ͨM�޻ش�
//					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
//					p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��=0
//				}
				if(UARTCtrl->HostCount==1)
				{//�����ط�
					UARTCtrl->TxSEQ+=1;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				}
				UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
				UARTCtrl->TxByte=(Len+8);//�跢���ֽ���
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			#if (USER/100)==14
				#if ComTest==1 //����232����485��������ݵ��Գ��򿪹أ�0==�ر�;1==����				
				LogOut_RS232(RS485_2PORT,(u32)"[3761-GS]",9,1);
				LogOut_RS232(RS485_2PORT,(u32)p8txbuff,UARTCtrl->TxByte,0);
				#endif
			#endif
				//��ʱ��ʱ��
				i=MRR(ADDR_AFN04F1+2,2);//�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
				i&=0xfff;
				if(i<WaitHostACKMinTimer)
				{
					i=WaitHostACKMinTimer;//�����ϱ��ȴ���վȷ����С��ʱ��
				}
				if(i>WaitHostACKMaxTimer)
				{
					i=WaitHostACKMaxTimer;//�����ϱ��ȴ���վȷ�����ʱ��
				}
				p16ACKTimer[0]=i;//�����ϱ��ȴ���վȷ����
//				p16ACKTimer[0]=15;//�����ϱ��ȴ���վȷ����-Ĭ��Ϊ15s
			}//if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�   /*���ڴ��ڵ�½״̬*/
			else
				ForbidReportFlag();//��ֹ�����ϱ�
		}//if(p16[0]==0x0)--���ڶ�ʱ��Ϊ0
	}//if(UARTCtrl->Task==0x0)--���ڿ���
}

#endif
