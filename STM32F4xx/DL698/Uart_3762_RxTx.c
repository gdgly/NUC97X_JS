
//376.2
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../DL698/PROXY_TASK.h"
#include "../DL698/RM_TASK.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../DL698/SearchMeter_Record.h"
#include "../DL698/MeterAutoReport.h"
#include "../DL698/MeterAutoReport_Record.h"
#include "../TEST/LOG.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/DL698_JSON.h"




#include "../MS/MS.h"


#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

u32 UART_3762_Rx(u32 PORTn)//UART 3762����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
{
	u32 i;
	u16 * p16timer;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rx;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p0=p16fifo[0];//�жϽ���ָ��
	p1=p16fifo[1];//��ǰ�Ѷ�ָ��
	LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//�жϽ���ָ��
		p16fifo[1]=0;//��ǰ�Ѷ�ָ��
		return 0;//0=���ڽ���,1=���1֡����,2=���ճ�ʱ
	}
	if(p0!=p1)
	{//�����ݽ���,���ý��ճ�ʱ��ʱ
		p16timer[0]=500/10;//�ֽڼ䳬ʱ��ʱ500MS
	}
	else
	{
		if(p16timer[0]==0x0)
		{//��ʱ
			UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			return 2;//0=���ڽ���,1=���1֡����,2=���ճ�ʱ
		}
	}
	RxByte=UARTCtrl->RxByte;
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-10))
		{
			p1=0;
		}
		p8rx[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(i!=0x68)
			{
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte>6)
			{
				if(RxByte>=(p8rx[1]+(p8rx[2]<<8)))
				{
					if(i==0x16)
					{//�������
						i=Uart_3762_CS((u32)p8rx);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
						if(i!=0)
						{//��У�����
							RxByte=0;
							continue;
						}
						//��У����ȷ
					#if LOG_1376_2==1
						LOG(13762,0,PORTn,p8rx,RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					#endif
					#if DEBUG_RS485_4toRS232==1//0=��,1=��;����:RS485_4(�ز�)���շ����ݴ�ӡ��RS232��
						LOGRS232(13762,0,PORTn,p8rx,RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					#endif
						
						UARTCtrl->Task=1;//1=���1֡����
						p16fifo[1]=p1;
						UARTCtrl->RxByte=RxByte;
						Uart_3762_RxData(PORTn);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
						p16timer[0]=30/10;//���յ�������ʱ
						if(UARTCtrl->Task==1)//1=���1֡����
						{
							//�ز����ܵ��ֽ�
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�ز����1֡����\n ",(u32)&__func__,(u32)__LINE__,0);	
							for(i=0;i<UARTCtrl->RxByte;i++)
								myprintf("%02x ",p8rx[i],0,0);
							myprintf("\n",0,0,0);
							#endif
							return 1;//0=���ڽ���,1=���1֡����,2=���ճ�ʱ
						}
						return 0;//ȷ�ϻش�����Ϊ0=���ڽ���
					}
					else
					{
						RxByte=0;
						continue;
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	return 0;//0=���ڽ���,1=���1֡����,2=���ճ�ʱ
}


u32 Uart_3762_RxTx(u32 PORTn)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=�������,7=������(ͬRS485��3=��������);����:0=��������,1=������
{
	u32 i;
	u16 * p16timer;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->Task;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	switch(i)
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			{
				//����ʱ������������
				i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				if(i)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				{
					PROXY_TASK_3762(PORTn);//��������376.2
					return 1;//0=��������,1=������
				}
				//�㲥Уʱ
				i=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2~127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82~0xFFУʱ-3����
				if((i&0x7f)==1)
				{
					i++;
					UARTCtrl->BroadcastTimeTask=i;
					return 1;//0=��������,1=������
				}
				//�Զ��ѱ�
				i=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
				if(i==1)
				{
					i++;
					UARTCtrl->AutoSearchTask=i;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
					return 1;//0=��������,1=������
				}
				//���ܱ������ϱ�
				i=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
				if(i==0)
				{
					i=UARTCtrl->AutoReportCopyNUM;//�����ϱ������Ѿ�����֡��
					if(i)
					{
						MeterAutoReportStateWord(PORTn,(u8*)ADDR_UART4RxCopy);//�����ϱ�״̬��;���:�Զ��ϱ�����֡;����:0=û����,1=����
						MR(ADDR_UART4RxCopy,ADDR_UART4RxCopy+LEN_UART4RxCopy,(i-1)*LEN_UART4RxCopy);
						i--;
						UARTCtrl->AutoReportCopyNUM=i;//�����ϱ������Ѿ�����֡��
						UARTCtrl->AutoReportTask=2;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
						return 1;//0=��������,1=������
					}
				}
				if(i==1)
				{
					i++;
					UARTCtrl->AutoReportTask=i;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
					return 1;//0=��������,1=������
				}
				
			}
			return 0;//0=��������,1=������
		case 1://1=���1֡����
		case 2://2=���ճ�ʱ(�����)
		case 6://6=������(ͬRS485��3=��������)
			//��������ֻ�ڿ���ʱ����
			i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			if(i>=2)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			{
				if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				{
					UARTCtrl->Task=0;
				}
			}
			break;
		case 3://3=��������
		//#if (USER/100)==0//���Ժ����
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
			if(p16timer[0]!=0)
			{
				break;
			}
		//#endif	
//			UARTCtrl->BpsCtrl=0xb+(6<<5);//9600,8e1
//			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
//			p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
//			p16fifo[0]=0;//�жϽ���ָ��
//			p16fifo[1]=0;//��ǰ�Ѷ�ָ��
		#if LOG_1376_2==1
			LOG(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
		#if DEBUG_RS485_4toRS232==1//0=��,1=��;����:RS485_4(�ز�)���շ����ݴ�ӡ��RS232��
			LOGRS232(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=UARTCtrl->TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;
				p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
			#if((USER/100)==0||(USER/100)==17)//���Ժ���Ժͽ���ϵ
				p16timer[0]=600/10;//�´η�����ʱ
			#else
				p16timer[0]=100/10;//�´η�����ʱ
			#endif
			}
			break;
		case 5://5=���ڽ���
			//myprintf("[%s:%d]:�ز����ڽ���\n",(u32)&__func__,(u32)__LINE__,0);
			i=UART_3762_Rx(PORTn);//UART 3762����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
			switch(i)
			{
				default://���ڽ���
/*
				#if (USER/100)==0//���Ժ����
					i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
					i|=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
					i|=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
					i|=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
					if(i==1)
					{//����������׼�����ʱ
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						if(p16timer[0]>(15000/10))
						{
							p16timer[0]=(15000/10);
						}
						return;
					}
				#endif
*/
					break;
				case 1:
					//myprintf("[%s:%d]:�ز��������\n",(u32)&__func__,(u32)__LINE__,0);
					UARTCtrl->Task=1;//1=���1֡����
					break;
				case 2:
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					break;
			}
			break;
	}
	return 0;//0=��������,1=������
}


u32 Uart_3762_CS(u32 ADDR_BUFF)//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
{
	u32 i;
	u32 x;
	u32 cs;
	u8 * p8;

	p8=(u8 *)(ADDR_BUFF);
	x=(p8[1]+(p8[2]<<8))-5;
	cs=0;
	for(i=0;i<x;i++)
	{
		cs+=p8[3+i];
	}
	cs&=0xff;
	if(cs==p8[3+i])
	{
		cs=0;
	}
	else
	{
		p8[3+i]=cs;
		cs=1;
	}
	p8[4+i]=0x16;
	return cs;
}

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
void Uart_3762_TxData(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data)//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
{
	u32 A;
	u32 L;
	u32 PRM;//����֡��־
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;

	PRM=1;//����֡��־
  p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->TxToRxTimerOver=6000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
	switch(AFN)
	{
		case 0://ȷ��/����
			MC(0,((u32)p8tx)+4,5);//��Ϣ��R
			PRM=0;//����֡��־
			UARTCtrl->TxToRxTimerOver=0;//���ͺ���ճ�ʱ��ʱֵ=0,ʹ�ܺܿ쳬ʱ��ת�����Ա�����ת��
			break;
		case 1://��ʼ��
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			break;
		case 2://ת������
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			break;
		case 3://��ѯ����
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			switch(Fn)
			{
				case 3://�ӽڵ�������Ϣ
					p8tx[4]|=0x5;//R-1��ͨ��ģ���ʶ=1,·�ɱ�ʶ=1
					break;
				case 4://���ڵ��ַ
					break;
				case 6://���ڵ����״̬
					p8tx[4]|=0x1;//R-1��·�ɱ�ʶ=1
					UARTCtrl->TxToRxTimerOver=(60000+10000)/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					break;
			}
			break;
		case 4://��·�ӿڼ��
//			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
//			switch(Fn)
//			{
//				case 1://���Ͳ���
//					p8=(u8 *)(MenuData_ADDR);
//					p16ctrl[15]=p8[0]+5;//���ͺ���ճ�ʱ��ʱֵS
//					break;
//				case 2://�ز��ӽڵ���
//					p8tx[4]|=0x5;//R-1��ͨ��ģ���ʶ=1,·�ɱ�ʶ=1
//					p16ctrl[15]=10;//���ͺ���ճ�ʱ��ʱֵS
//					break;
//			}
//			break;
			return;
		case 5://��������
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			if(Fn==3)
			{//�����㲥
				//��ʵ��ʱ��Terminal_Ram->RouterCheck_S_Timer//���㲥��ʱʱ����(���ֶ���)
				UARTCtrl->TxToRxTimerOver=120000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			}
			break;
		case 6://�����ϱ�
			break;
		case 0x10://·�ɲ�ѯ
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			break;
		case 0x11://·������
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			break;
		case 0x12://·�ɿ���
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			break;
		case 0x13://·������ת��
			if((Terminal_Router->MaxOverTime!=0)&&(Terminal_Router->MaxOverTime<120))
			{
				UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			}
			else
			{
				UARTCtrl->TxToRxTimerOver=60000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			}
			break;
		case 0x14://·�����ݳ���
			switch(Fn)
			{
				case 3://������ͨ����ʱ����ͨ������
					break;
				default:
					MC(0,((u32)p8tx)+4,5);//��Ϣ��R
					break;
			}
			PRM=0;//����֡��־
			break;
		case 0x15://�ļ�����
			//F1�ļ����䷽ʽ1
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			break;
		case 0xF0://�ڲ�����
			MC(0,((u32)p8tx)+4,6);//��Ϣ��R
			break;
		default:
			return;
	}
	A=0;
	if((p8tx[4]&0x4)!=0x0)
	{
		A=12;
	}
	A+=((p8tx[4]>>4)&0xf)*6;
	p8tx[10+A]=AFN;
	Fn--;
	p8tx[10+A+1]=1<<(Fn%8);
	p8tx[10+A+2]=Fn/8;
	MR(((u32)p8tx)+10+A+3,((u32)p8tx)+128,LEN_Data);
	L=10+A+3+LEN_Data+2;
	p8tx[0]=0x68;
	p8tx[1]=L;//���ֽ���
	p8tx[2]=L>>8;
	if(PRM==0)//����֡��־
	{
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
	}
	else
	{
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
		UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
		p8tx[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	}
	Uart_3762_CS((u32)p8tx);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
	UARTCtrl->TxByte=L;//�����ֽ���(���ֶ���)
	UARTCtrl->Task=3;//�ȴ�����
}

void Uart_3762_AFN13F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag)//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
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
	UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
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
	
	p8tx[22]=0x13;//AFN=13H
	p8tx[23]=1;//Fn=1
	p8tx[24]=0;
	p8tx[25]=Protocol;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
	p8tx[26]=DelayFlag;//ͨ����ʱ��ر�־
	p8tx[27]=0;//�����ڵ���=0
	p8tx[28]=LEN_Data;//DL645���ĳ���
	MR(((u32)p8tx)+29,ADDR_Data,LEN_Data);//��׼645֡

	Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
}

u32 Uart_3762_AFN13F1_Rx(u32 PORTn)//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
{
	u32 i;
	u32 x;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	x=0;//��ȷ
	i=p8rx[1]+(p8rx[2]<<8);
	if(i<=16)
	{//����
		return 4;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	if(UARTCtrl->Task!=1)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{//��ʱ
		return 5;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	i=MRR(((u32)p8rx)+22,3);
	if(i!=0x113)
	{//AFN=13H,Fn=1
		return 255;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	i=p8rx[28];//���������򳤶�
	if(i==0)
	{//�������ݳ���=0
		return 255;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	if(x==0)
	{
		MR((u32)p8rx,((u32)p8rx)+28+1,i);
		//ͨ�ŵ�ַ�Ƚ�
		for(i=0;i<6;i++)
		{
			if(p8rx[1+i]!=p8tx[29+1+i])
			{
				break;
			}
		}
		if(i!=6)
		{
			return 1;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
		}
		//������
		if((p8rx[8]&0x40)!=0x0)
		{//��վ�쳣�ش�
			return 2;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
		}
		//��0x33
		for(i=0;i<p8rx[9];i++)
		{
			p8rx[10+i]-=0x33;
		}
	}
	return 0;
}

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
u32 Get_Uart_3762_RxData_AFN_Fn(u32 PORTn)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
{
	u32 AFN;
	u32 Fn;
	u8 * p8rx;
	
  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	AFN=p8rx[10];
	Fn=BitLoca(p8rx[11]);//�����1����λΪ1��λ��(0-32)
	Fn+=(p8rx[12]*8);
	return((AFN<<8)+Fn);
}

u32 Uart_3762_RxData_AFN00_F1(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data)//UART_3762����AFN00F1ȷ������,���AFN,Fn,LEN_Data����ʧ���ط��ò���;����0δȷ��1ȷ��2ʧ��
{
	UARTCtrl_TypeDef * UARTCtrl;
	u16 *p16timer;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		case 1://1=���1֡����
			if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
			{
				UARTCtrl->Task=0;//0=����,����ת���Ȳ������
				Terminal_Router->RouterInitReCount=0;//�����ط�����
				Terminal_Router->RouterInitSubTask=0;//������
				Terminal_Router->RouterInitTask++;//·������������
				return 1;//����0δȷ��1ȷ��2ʧ��
			}
			else
			{
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			}
			break;
		case 0://0=����
		case 2://2=���ճ�ʱ(�����)
			if(Terminal_Router->RouterInitReCount<2)
			{
				Terminal_Router->RouterInitReCount++;
				Uart_3762_TxData(PORTn,AFN,Fn,LEN_Data);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			}
			else
			{//ʧ��
				UARTCtrl->Task=0;//0=����,����ת���Ȳ������
				Terminal_Router->RouterInitReCount=0;//�����ط�����
				Terminal_Router->RouterInitSubTask=0;//������
				Terminal_Router->RouterInitTask++;//·������������
				return 2;//����0δȷ��1ȷ��2ʧ��
			}
			break;
	}
	return 0;//����0δȷ��1ȷ��2ʧ��
}

u32 Get_ListNo(u8* p8);//�õ������б��;���:�����ַ;����:�б��0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax��ʾ��Ч)
#ifdef DEBUG_JSON
void  log_CollectorAddr(void)
{
	u32 i;
	u32 y;
	u32 x;
	u32 z;
	u64 d64a;
	u64 d64b;
	
	x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
	myprintf("[%s:%d]:��ӡ������Ч�Ĳɼ�����ַ\n",(u32)__func__,(u32)__LINE__,0);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
		if(d64b!=0)
		{	
			for(z=0;z<i;z++)
			{
				d64a=MRR((u32)&NoteList->List[z].CollectorAddr,6);
				if(d64b==d64a)
				{
					break;
				}
			}
			if(z>=i||i==0)
			{
				for(y=6;y>0;y--)
				{	
					myprintf("%02x ",NoteList->List[i].CollectorAddr[y-1],0,0);
				}
				myprintf("\n",0,0,0);
			}	
		}
	}
}
#endif
void Uart_3762_RxData(u32 PORTn)//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
{
//	u32 iii;
	u32 i;
#if(USER/100)==17//����ϵ	
	u32 j;
#endif
	u32 x;
	u32 n;
	u32 m;
	u32 AFN;
	u32 Fn;
	u32 Protocol;
	u64 CollectorAddr;
	u64 MeterAddr;
	u32 Phase;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 *p16timer;
	u8* p8;
	u32 offset;
	u8	pOccurSour[16];		  // ����¼�����Դ    OAD
//	u8 *p8s;
//	u8 *p8d;
//	u32 z;
//	u64 d64a;
//	u64 d64b;

	ms_Type *ms;
	ms=Get_ms();
	
  	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	Phase=p8rx[6]&0x0f;//�����λ
	if((p8rx[4]&0x4)!=0)
	{//ͨ��ģ���ʶ��0��ʾ�Լ�������ͨ��ģ�����,1��ʾ���ز����ͨ��ģ�����
		p8rx+=12;
	}
	AFN=p8rx[10];
	Fn=BitLoca(p8rx[11]);//�����1����λΪ1��λ��(0-32)
	Fn+=(p8rx[12]*8);
	
	switch(AFN)
	{
		case 3://
			switch(Fn)
			{
				case 0x10:
					break;
			}
		case 0xF1://AFN=F1H,Fn=1,�����������֡
			switch(Fn)
			{
				case 1://AFN=F1H,Fn=1,�����������֡
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					MR(ADDR_DATABUFF,(u32)p8rx+10,6);
					Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
					i=Get_ListNo((u8*)ADDR_DATABUFF);//�õ������б��;���:�����ַ;����:�б��0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax��ʾ��Ч)
					if(i<NUM_RMLISTmax)
					{
						RMLIST->List[i].TIMER_S=0;//�����������ͺ�ȴ�������ֶ�ʱ
						RMLIST->List[i].TASK=1;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������),7=ͣ��(Ϊ����ִ���¸��б��)
						if(Terminal_Router->NUM_Enable_Concurrency<5)//����ز���ǰ��������֡��
						{
							Terminal_Router->NUM_Enable_Concurrency++;//����ز���ǰ��������֡��
						}
					}
					break;
			}
			return;
	#if(USER/100)==17//����ϵ
		case 0x14://AFN=14H,����ʱ��
			switch(Fn)
			{
				case 2://F2����ʱ��
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					//������������21Byte!!! �ⷵ��ȷ��֡����Ϊ���ճ�ʱ�������ط�����
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					n=p16timer[0];//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					MR(ADDR_DATABUFF,(u32)p8tx,32);
					MR((u32)p8tx,(u32)p8rx,10);//����10�ֽڽ���ͷ������ͷ
					MW(ADDR_MYMDHMS,((u32)p8tx)+128,6);
					Uart_3762_TxData(PORTn,0x14,2,6);
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
					p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
					if(p16timer[0]>(30/10))
					{
						p16timer[0]=30/10;
					}
					while(1)
					{
						Uart_3762_RxTx(PORTn);//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=�������,7=������(ͬRS485��3=��������);����:0=��������,1=������
						if((UARTCtrl->Task<3)||(UARTCtrl->Task>4))
						{
							break;
						}
					}
					//�ָ����������ݺ���س�ʱ��ʱֵ
					MW(ADDR_DATABUFF,(u32)p8tx,32);
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					p16timer[0]=n;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
					if(i>=2)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
					{
						if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
						{
							UARTCtrl->Task=0;
						}
					}
					break;
			}
			break;
	#endif
		case 6://AFN=06H,�����ϱ�
			switch(Fn)
			{
//F1�ϱ��ӽڵ���Ϣ
				case 1://F1�ϱ��ӽڵ���Ϣ
					n=p8rx[13];//�����ϱ��ӽڵ������n
					for(i=0;i<n;i++)
					{
						MeterAddr=MRR(((u32)p8rx)+14+(i*9),6);
						Protocol=p8rx[14+6+(i*9)];//00H��͸�����䣻01H��DL/T645��1997��02H��DL/T645��2007��03H��FFH����
						//if(Protocol!=1)
						//{
						//	Protocol=30;
						//}
						//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,0x0,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
						SearchMeter_Record(PORTn,3,MeterAddr,Protocol,0,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
					}
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
					break;
//F2�ϱ���������
				case 2://F2�ϱ���������
					return;
//F3�ϱ�·�ɹ����䶯��Ϣ
				case 3://F3�ϱ�·�ɹ����䶯��Ϣ
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
					switch(p8rx[13])
					{
						case 1://1Ϊ�����������
							Terminal_Router->RUNState&=~2;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
							break;
						case 2://2Ϊ�ѱ��������
							if(UARTCtrl->AutoSearchTask==(6+1))//�Զ��ѱ�����:0=��,1=��ʼ,2...����
							{//6=�Զ��ѱ�������ж�·������״̬��ʱ
								Terminal_Ram->AutoSearch_S_Timer=0;//28 �ز��Զ��ѱ��붨ʱ��
							}
							break;
					}
					break;
//F4�ϱ��ӽڵ���Ϣ���豸����
				case 4://F4�ϱ��ӽڵ���Ϣ���豸����
					switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
					{
						default://����
						case 2://2=����(�ϱ��ɼ���)
						#ifdef DEBUG_JSON
							myprintf("[%s:%d]:06f4�ϱ��ѱ���\n",(u32)__func__,(u32)__LINE__,0);
							for(i=0;i<120;i++)
							{	
								myprintf("%02x ",p8rx[i],0,0);
							}
							myprintf("\n",0,0,0);
						#endif
							n=p8rx[13];//�����ϱ��ӽڵ������n
							CollectorAddr=MRR(((u32)p8rx)+13+1,6);
							Protocol=p8rx[13+1+6];//00H��͸�����䣻01H��DL/T645��1997��02H��DL/T645��2007��03H��FFH����
							//if(Protocol!=1)
							//{
							//	Protocol=30;
							//}
							//Driver=p8rx[14+9];//�豸���ͣ�01:081 �ɼ�����00:082 �ɼ�����02�������������03H��FFH ����
							p8rx+=13+11;
							for(i=0;i<n;i++)
							{
								m=p8rx[1];//�����ϱ��Ĵӽڵ�1�½ӵĴӽڵ�����n1
								if(m==0)
								{//ֻ�ϱ��ɼ���,������485��
								#if(USER/100)==17//����ϵ
									SearchMeter_Record(PORTn,3,CollectorAddr,Protocol,CollectorAddr,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
									p8rx+=2;
									
									SearchMeter_CollectorAddrRecord_JS(CollectorAddr);
									continue;
								#else
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
									SearchMeter_Record(PORTn,3,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
									p8rx+=2;
									continue;
								#endif
								}
							#if (USER/100)==17//����ϵ
								SearchMeter_Record_JS(CollectorAddr,m,p8rx);
							#endif
								for(x=0;x<m;x++)
								{//����485��
									MeterAddr=MRR(((u32)p8rx)+2+(7*x),6);
									Protocol=p8rx[2+(7*x)+6];//00H��͸�����䣻01H��DL/T645��1997��02H��DL/T645��2007��03H��FFH����
									//if(Protocol!=1)
									//{
									//	Protocol=30;
									//}
									//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
									SearchMeter_Record(PORTn,3,MeterAddr,Protocol,CollectorAddr,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
								}
								p8rx+=2+(7*x);
							}
							#ifdef DEBUG_JSON
							log_CollectorAddr();
							#endif
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
							break;
//						default://����
						case 3://����
						case 5://��˹��
							CollectorAddr=MRR(((u32)p8rx)+13+1,6);
							Protocol=p8rx[13+1+6];//00H��͸�����䣻01H��DL/T645��1997��02H��DL/T645��2007��03H��FFH����
							//if(Protocol!=1)
							//{//ͨ��Э�����ͣ�00H��͸�����䣻01H��DL/T645-1997��02H��DL/T645-2007��03H��FFH����
							//	Protocol=30;
							//}
							i=p8rx[14+9];//c)	�豸���ͣ�00H���ɼ�����01H�����ܱ�02H-FFH ����
							if(i==1)
							{//�ϱ����ܱ�
								//AutoSetAFN04F10(PORTn,CollectorAddr,Protocol,0x0,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
								SearchMeter_Record(PORTn,3,CollectorAddr,Protocol,0,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
							}
							else
							{//�ϱ��ɼ���
								p8rx+=13+11;
								m=p8rx[1];//�����ϱ��Ĵӽڵ�1�½ӵĴӽڵ�����n1
								if(m==0)
								{//ֻ�ϱ��ɼ���,������485��
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
									SearchMeter_Record(PORTn,3,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
								}
								else
								{//����485��
									for(x=0;x<m;x++)
									{
										MeterAddr=MRR(((u32)p8rx)+2,6);
										Protocol=p8rx[2+6];//00H��͸�����䣻01H��DL/T645��1997��02H��DL/T645��2007��03H��FFH����
										//if(Protocol!=1)
										//{//ͨ��Э�����ͣ�00H��͸�����䣻01H��DL/T645-1997��02H��DL/T645-2007��03H��FFH����
										//	Protocol=30;
										//}
										//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
										SearchMeter_Record(PORTn,3,MeterAddr,Protocol,CollectorAddr,0,Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
										p8rx+=6+1;
									}
								}
							}
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
							break;
					}
					break;
//F5�ϱ��ӽڵ��¼�
				case 5:
					n=0;
					n|=p8rx[1];
					n|=p8rx[2]<<8;
					ms->link_printf("\n************************AFN06F5**************\r\n", 0, 0, 0);
					for(i=0;i<n;i++)
					{
						ms->link_printf(" %02x",p8rx[i],0,0);
					}
					ms->link_printf("\n", 0, 0, 0);
					//F5�ϱ��ӽڵ��¼�
					//ͣ���ϱ��¼���չ
					//�ӽڵ��豸���� �ӽڵ��豸���� �ӽڵ��豸����BIN 1
					//ͨ��Э������ ͨ��Э������ ͨ��Э������BIN 1
					//���ĳ��� ���ĳ��� L	BIN 1
					//�������� �������� -	L
					//�ӽڵ��豸���ͣ�00H Ϊ�ɼ�����01H Ϊ���ܱ�02H������ز�ͨ�ŵ�Ԫ��03H��խ���ز�ͨ�ŵ�Ԫ��04H��΢��������ͨ�ŵ�Ԫ��05H��˫ģͨ�ŵ�Ԫ��06H��FFH ����
					//ͨ��Э�����ͣ�00H Ϊ������01H ΪDL/T 645��1997��02H ΪDL/T 645��2007�� 03H����698.45,04H��FFH ����			
					//ͨ�ŵ�Ԫͣ���ϱ��¼��ġ����������򡱶������£�
					//1.ͨ�ŵ�Ԫ�¼����ͣ�һ���ֽڣ�01H����ͣ���¼���02H�������¼�,����������
					//2.����������Ϊ����ͣ���¼���ͨ�ŵ�Ԫ��ַ���У�������ɼ�����ַ��������6*N�ֽڣ�N���������¼��Ľڵ���������������ڵ�������255ʱ�����з�֡�ϱ���			
					Protocol=p8rx[13+1];
					
					ms->link_printf("\n************#11111**************\n", 0, 0, 0);
					if(Protocol==05)//ͨ��Э������, 05H ����ܾ��ڵ���Ϣ�ϱ�����
					{
					/*
						68 1A 00 C3 00 00 00 00 01 05 06 10 00 
						02 // �ڵ��豸����
						05 // ͨѶЭ������ 13 +1
						08 // ���ݳ��� 13+2
						01 //�����ϱ��ڵ����13+3
						32 00 00 00 00 00 // �ڵ��ַ
						05				  //  Э������
						26 16 
		
					*/	
						n=p8rx[13+3];
						p8rx+=(13+4);
						p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
						p8[0]=DataType_array;
						p8[1]=n;
						i=0;
						for(i=0;i<n;i++)
						{
							p8[2+(6+2)*i]=DataType_octet_string;
							p8[2+1+(6+2)*i]=6;
							Data_Inverse((u32)p8rx+i*7,6);
							MR((u32)p8+2+2+((6+2)*i),(u32)p8rx+i*7,6);
						}
						pOccurSour[0]=DataType_TSA;
						pOccurSour[1]=7;
						pOccurSour[2]=5;
						MR((u32)pOccurSour+3,ADDR_4001+2,6);
						ms->link_printf("\n********************311F********************\n", 0, 0, 0);
						Event_311F0200(p8,pOccurSour,2+8*n);
					}
				
					ms->link_printf("\n************#22222**************\n", 0, 0, 0);
					i=p8rx[13+3];
					if((i==1)||(i==2))
					{//��ͣ���ͣ��ָ��ϱ��¼�
						n=p8rx[13+2];//���ĳ���
						if(n>=7)
						{
							n--;//��ַ����
							n/=6;
							p8rx+=13+4;
							while(n--)
							{
#if(USER/100)==17//����ϵ			
								j=p8rx[6];
								if(((i==1)&&(j==0))||((i==2)&&(j==1)))
								{// ͣ��
									MeterAutoReport_3011(p8rx,i);//�ز����ϱ��ӽڵ��¼�(���ܱ�ͣ���¼�);���:pAddr=��·��ģ����յ��Ľ��ջ����е�6�ֽڵ�ַ(����ɼ�����ַ)							
								}
								p8rx+=7;							
#else
								MeterAutoReport_3011(p8rx,i);//�ز����ϱ��ӽڵ��¼�(���ܱ�ͣ���¼�);���:pAddr=��·��ģ����յ��Ľ��ջ����е�6�ֽڵ�ַ(����ɼ�����ַ)
								p8rx+=6;
#endif
							}
						}
						break;
					}
					//(����)̨������
					//68H	A0	����	A5	68H	9EH	07H	03H	PA0	����	PA5	CS		16H
					//�����룺C=9EH
					//��ַ�򣺵���ַ
					//������L=1+6=7
					//���ݵ�Ԫ��ʶ��02H����̨��վ�㵫���ڱ�̨�������С�������33H��
					//              03H����Ǳ�̨��վ�㵫�ڱ�̨�������С�������33H��
					//���ݵ�Ԫ���ݣ�PA0~ PA5Ϊ�����ز����ڵ��ַ
					//���飺��̨������ϱ��¼����ڵ��ַδ֪ʱ����ȫFF���
					i=p8rx[13+3];
					if(i==0x68)
					{
						i=p8rx[13+3+8];
						if(i==0x9e)
						{
							i=p8rx[13+3+9];
							if(i==7)
							{
#if(USER/100)==17//����ϵ									
								// �Ǳ�׼645Э�飬���ݱ�ʶ��������Ҫ��ȥ0x33
								for(offset=0;offset<i;offset++)
								{
									p8rx[13+3+10+offset]=p8rx[13+3+10+offset]-0x33;
								}
#endif								
								//̨��״̬�� 
								//0----δ֪����ȡ�Ľڵ��̨����ַΪ��Ч�ĵ�ַ��	
								//1----��̨��վ�㣺�ڵ�������̨����ַ�ͱ�̨����ַ��ͬ�����ڱ�̨���ڵ㣬��Ϊ�����нڵ㡣
								//2----��̨��վ���ҵ����ڱ������ڵ�������̨�����ڱ�̨���������ڱ�̨�������С�
								//3----�Ǳ�̨��վ�㵫�ڱ������ڵ�������̨�������ڱ�̨�������ڱ�̨�������С�
								//4----��վ�㲻֧��̨��ʶ�𣺻�ȡ�����ڵ�������̨����ַ��
								//����Ԥ����
								i=p8rx[13+3+10];
								switch(i)
								{
									case 1://1----��̨��վ�㣺�ڵ�������̨����ַ�ͱ�̨����ַ��ͬ�����ڱ�̨���ڵ㣬��Ϊ�����нڵ㡣
									case 2://2----��̨��վ���ҵ����ڱ������ڵ�������̨�����ڱ�̨���������ڱ�̨�������С�
										break;
									default:
										p8=(u8*)ADDR_DATABUFF+1024;
								//һ����̨�������=structure
								//{
										p8[0]=DataType_structure;
										p8[1]=3;
										offset=2;
								//	ͨ�ŵ�ַ       TSA��
										p8[offset]=DataType_TSA;
										p8[offset+1]=7;
										p8[offset+2]=5;
										MR((u32)p8+offset+3,(u32)p8rx+13+3+1,6);
										Data_Inverse((u32)p8+offset+3,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
										offset+=9;
								//	���ڵ��ַ     TSA��
										p8[offset]=DataType_TSA;
										p8[offset+1]=7;
										p8[offset+2]=5;
										MR((u32)p8+offset+3,(u32)p8rx+13+3+11,6);
										Data_Inverse((u32)p8+offset+3,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
										offset+=9;
								//	���ʱ��       date_time_s
										p8[offset]=DataType_date_time_s;
										Get_date_time_s(p8+offset+1);//�õ�date_time_s��7�ֽ�
										offset+=8;
								//}
								
										Event_31120200(p8);//��̨�����ܱ��¼���¼;���:pDATA=һ����̨�����;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF


										Data_Inverse((u32)(u32)p8rx+13+3+1,6);
										MeterAddr=MRR((u32)p8rx+13+3+1,6);
										
										Data_Inverse((u32)(u32)p8rx+13+3+11,6);
										CollectorAddr=MRR((u32)p8rx+13+3+11,6);
										SearchMeter_Record(PORTn,4,MeterAddr,0,CollectorAddr,0,0);
										break;
								}
							}
						}
					}
				#if(USER/100)==17//����ϵ
					i=p8rx[13+3];
					if(i==0x68)
					{
						i=p8rx[13+3+14];
						if(i==0x88)//�ϱ�֪ͨ
						{
							i=(p8rx[13+3+18]<<8)+p8rx[13+3+19];
							if(i==0x310A)//����ͣ��/�����¼��ϱ�
							{
								p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
								i=p8rx[13+4];
								if(i==0x50)//����ͣ�����¼�����
								{
									p8[0]=DataType_enum;//����Դ
									p8[1]=3;//�¼�����Դ��485�������
									Event_310A0200(p8,1,p8rx+21);
								}
								else		//����ͣ�����¼��ָ�
								{		
									p8[0]=DataType_enum;//����Դ
									p8[1]=3;//�¼�����Դ��485�������
									Event_310A0200(p8,0,p8rx+21);
								}
							}
						}
					}
				#endif
					//��ǰ��0xfe����
					i=0;
					while(p8rx[i+13+3]==0xfe)
					{
						i++;
					}
					for(x=0;x<p8rx[i+13+3+9];x++)
					{
						p8rx[i+13+3+10+x]-=0x33;
					}
					x=MRR(((u32)p8rx)+i+13+3+10,4);
					if(x==0x04001501)
					{
					#if EnMeterAutoReportEvent==1//������ܱ������ϱ��¼�:0=������,1=����
						//WriteAutoReportWord(PORTn,((u32)p8rx)+i+13+3);//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
						i=MeterAutoReportStateWord(PORTn,p8rx+i+13+3);//�����ϱ�״̬��;���:�Զ��ϱ�����֡;����:0=û����,1=����
						if(i==0)
						{
							//return;//0=û����
						}
					#endif
					}
					break;
			}
			
			//2018.3.1�޸�ȷ�Ϸ���֡
	  	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			//������������21Byte!!! �ⷵ��ȷ��֡����Ϊ���ճ�ʱ�������ط�����
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			n=p16timer[0];//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			MR(ADDR_DATABUFF,(u32)p8tx,32);
			MR((u32)p8tx,(u32)p8rx,10);//����10�ֽڽ���ͷ������ͷ
			MWR(0xffffffff,((u32)p8tx)+128,6);
			Uart_3762_TxData(PORTn,0,1,6);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
		//#if (USER/100)==0//���Ժ����
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
			if(p16timer[0]>(30/10))
			{
				p16timer[0]=30/10;
			}
		//#endif	
			while(1)
			{
				Uart_3762_RxTx(PORTn);//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=�������,7=������(ͬRS485��3=��������);����:0=��������,1=������
				if((UARTCtrl->Task<3)||(UARTCtrl->Task>4))
				{
					break;
				}
			}
			//�ָ����������ݺ���س�ʱ��ʱֵ
			MW(ADDR_DATABUFF,(u32)p8tx,32);
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
//		#if (USER/100)==0//���Ժ����
//			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
//			p16timer[0]=500/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
//		#else
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			p16timer[0]=n;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
//		#endif
			i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			if(i>=2)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			{
				if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				{
					UARTCtrl->Task=0;
				}
			}
			break;
	}	
	
}
#endif







