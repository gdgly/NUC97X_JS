
//376.2
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "Test_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"

#include "../Calculate/Calculate.h"
//#include "Terminal_AFN0E_Event.h"
//#include "Terminal_AFN0C_RealTimeData_Fn.h"
//#include "Terminal_ReadMeter_SaveFnData.h"

//#include "AutoSearchMeter.h"
//#include "Terminal_ReadMeter_Event.h"

#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void Uart_3762_RxTx(u32 PORTn)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			break;
		case 1://1=���1֡����
			break;
		case 2://2=���ճ�ʱ(�����)
			break;
		case 3://3=��������
		#if (USER/100)==0//���Ժ����
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
			if(p16timer[0]!=0)
			{
				break;
			}
		#endif	
//			UARTCtrl->BpsCtrl=0xb+(6<<5);//9600,8e1
//			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
//			p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
//			p16fifo[0]=0;//�жϽ���ָ��
//			p16fifo[1]=0;//��ǰ�Ѷ�ָ��
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
			}
			break;
		case 5://5=���ڽ���
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
				return;
			}
			if(p0!=p1)
			{//�����ݽ���,���ý��ճ�ʱ��ʱ
				p16timer[0]=500/10;//�ֽڼ䳬ʱ��ʱ500MS
			}
			else
			{
				if(p16timer[0]==0x0)
				{//��ʱ
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
					return;
				}
			}
			RxByte=UARTCtrl->RxByte;
			p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
			p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
				p8rxbuff[RxByte]=i;
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
						if(RxByte>=(p8rxbuff[1]+(p8rxbuff[2]<<8)))
						{
							if(i==0x16)
							{//�������
								i=Uart_3762_CS((u32)p8rxbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
								if(i!=0)
								{//��У�����
									RxByte=0;
									continue;
								}
								//��У����ȷ
								UARTCtrl->Task=1;//1=���1֡����
								p16fifo[1]=p1;
								UARTCtrl->RxByte=RxByte;
								Uart_3762_RxData(PORTn);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						#if (USER/100)==0//���Ժ����
								p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
								p16timer[0]=200/10;//���յ�������ʱ
						#else
								p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
								p16timer[0]=50/10;//���յ�������ʱ
						#endif	
								return;
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
			break;
	}
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
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;

	PRM=1;//����֡��־
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->TxToRxTimerOver=6000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
	switch(AFN)
	{
		case 0://ȷ��/����
			MC(0,((u32)p8txbuff)+4,5);//��Ϣ��R
			PRM=0;//����֡��־
			UARTCtrl->TxToRxTimerOver=0;//���ͺ���ճ�ʱ��ʱֵ=0,ʹ�ܺܿ쳬ʱ��ת�����Ա�����ת��
			break;
		case 1://��ʼ��
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			break;
		case 2://ת������
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			break;
	#if (USER/100)==14//����
		case 0x53:
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			switch(Fn)
			{
				case 3://�ӽڵ�������Ϣ
					p8txbuff[4]|=0x5;//R-1��ͨ��ģ���ʶ=1,·�ɱ�ʶ=1
					break;
				case 4://���ڵ��ַ
					break;
				case 6://���ڵ����״̬
					p8txbuff[4]|=0x1;//R-1��·�ɱ�ʶ=1
					UARTCtrl->TxToRxTimerOver=(60000+10000)/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					break;
			}
			break;
	#else
		case 3://��ѯ����
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			switch(Fn)
			{
				case 3://�ӽڵ�������Ϣ
					p8txbuff[4]|=0x5;//R-1��ͨ��ģ���ʶ=1,·�ɱ�ʶ=1
					break;
				case 4://���ڵ��ַ
					break;
				case 6://���ڵ����״̬
					p8txbuff[4]|=0x1;//R-1��·�ɱ�ʶ=1
					UARTCtrl->TxToRxTimerOver=(60000+10000)/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					break;
			}
			break;
		#endif
		case 4://��·�ӿڼ��
//			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
//			switch(Fn)
//			{
//				case 1://���Ͳ���
//					p8=(u8 *)(MenuData_ADDR);
//					p16ctrl[15]=p8[0]+5;//���ͺ���ճ�ʱ��ʱֵS
//					break;
//				case 2://�ز��ӽڵ���
//					p8txbuff[4]|=0x5;//R-1��ͨ��ģ���ʶ=1,·�ɱ�ʶ=1
//					p16ctrl[15]=10;//���ͺ���ճ�ʱ��ʱֵS
//					break;
//			}
//			break;
			return;
		case 5://��������
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			if(Fn==3)
			{//�����㲥
				//��ʵ��ʱ��Terminal_Ram->RouterCheck_S_Timer//���㲥��ʱʱ����(���ֶ���)
				UARTCtrl->TxToRxTimerOver=120000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			}
			break;
		case 6://�����ϱ�
			break;
		case 0x10://·�ɲ�ѯ
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			break;
		case 0x11://·������
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			break;
		case 0x12://·�ɿ���
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
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
					MC(0,((u32)p8txbuff)+4,5);//��Ϣ��R
					break;
			}
			PRM=0;//����֡��־
			break;
		case 0x15://�ļ�����
			//F1�ļ����䷽ʽ1
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			break;
		case 0xF0://�ڲ�����
			MC(0,((u32)p8txbuff)+4,6);//��Ϣ��R
			break;
		default:
			return;
	}
	A=0;
	if((p8txbuff[4]&0x4)!=0x0)
	{
		A=12;
	}
	A+=((p8txbuff[4]>>4)&0xf)*6;
	p8txbuff[10+A]=AFN;
	Fn--;
	p8txbuff[10+A+1]=1<<(Fn%8);
	p8txbuff[10+A+2]=Fn/8;
	MR(((u32)p8txbuff)+10+A+3,((u32)p8txbuff)+128,LEN_Data);
	L=10+A+3+LEN_Data+2;
	p8txbuff[0]=0x68;
	p8txbuff[1]=L;//���ֽ���
	p8txbuff[2]=L>>8;
	if(PRM==0)//����֡��־
	{
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
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{
			p8txbuff[3]=0x43;//������;����ʽ·���ز�ͨ��
		}
		else
		{
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{
				p8txbuff[3]=0x4a;//������;΢��������ͨ��
			}
			else
			{
				p8txbuff[3]=0x41;//������;����ʽ·���ز�ͨ��
			}
		}
		UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
		p8txbuff[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	}
	Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
	UARTCtrl->TxByte=L;//�����ֽ���(���ֶ���)
	UARTCtrl->Task=3;//�ȴ�����
}

void Uart_3762_AFN13F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag)//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
{
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	p8txbuff[0]=0x68;
	p8txbuff[1]=(29+LEN_Data+2);//���ܷ����ֽ���
	p8txbuff[2]=(29+LEN_Data+2)>>8;
	if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{
		p8txbuff[3]=0x43;//������;����ʽ·���ز�ͨ��
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{
			p8txbuff[3]=0x4a;//������;΢��������ͨ��
		}
		else
		{
			p8txbuff[3]=0x41;//������;����ʽ·���ز�ͨ��
		}
	}
	p8txbuff[4]=4;//ͨ��ģ���ʶ=1
	p8txbuff[5]=0;
	p8txbuff[6]=LEN_ESC;//Ԥ��Ӧ���ֽ���
	p8txbuff[7]=0;
	p8txbuff[8]=0;
	UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	p8txbuff[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
	MR(((u32)p8txbuff)+16,ADDR_Data+1,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
	p8txbuff[22]=0x13;//AFN=13H
	p8txbuff[23]=1;//Fn=1
	p8txbuff[24]=0;
	p8txbuff[25]=Protocol;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
	p8txbuff[26]=DelayFlag;//ͨ����ʱ��ر�־
	p8txbuff[27]=0;//�����ڵ���=0
	p8txbuff[28]=LEN_Data;//DL645���ĳ���
	MR(((u32)p8txbuff)+29,ADDR_Data,LEN_Data);//��׼645֡

	Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
}

u32 Uart_3762_AFN13F1_Rx(u32 PORTn)//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
{
	u32 i;
	u32 x;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	x=0;//��ȷ
	i=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(i<=16)
	{//����
		return 4;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	if(UARTCtrl->Task!=1)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{//��ʱ
		return 5;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	i=MRR(((u32)p8rxbuff)+22,3);
	if(i!=0x113)
	{//AFN=13H,Fn=1
		return 255;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	i=p8rxbuff[28];//���������򳤶�
	if(i==0)
	{//�������ݳ���=0
		return 255;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
	}
	if(x==0)
	{
		MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,i);
		//ͨ�ŵ�ַ�Ƚ�
		for(i=0;i<6;i++)
		{
			if(p8rxbuff[1+i]!=p8txbuff[29+1+i])
			{
				break;
			}
		}
		if(i!=6)
		{
			return 1;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
		}
		//������
		if((p8rxbuff[8]&0x40)!=0x0)
		{//��վ�쳣�ش�
			return 2;//����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
		}
		//��0x33
		for(i=0;i<p8rxbuff[9];i++)
		{
			p8rxbuff[10+i]-=0x33;
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
	u8 * p8rxbuff;
	
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	AFN=p8rxbuff[10];
	Fn=BitLoca(p8rxbuff[11]);//�����1����λΪ1��λ��(0-32)
	Fn+=(p8rxbuff[12]*8);
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


void Uart_3762_RxData(u32 PORTn)//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 AFN;
	u32 Fn;
//	u64 CollectorAddr;
	u32 Protocol;
//	u64 MeterAddr;
//	u32 Phase;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	//Phase=p8rxbuff[6]&0x0f;//�����λ
	if((p8rxbuff[4]&0x4)!=0)
	{//ͨ��ģ���ʶ��0��ʾ�Լ�������ͨ��ģ�����,1��ʾ���ز����ͨ��ģ�����
		p8rxbuff+=12;
	}
	AFN=p8rxbuff[10];
	Fn=BitLoca(p8rxbuff[11]);//�����1����λΪ1��λ��(0-32)
	Fn+=(p8rxbuff[12]*8);
	
	switch(AFN)
	{
		case 6://AFN=06H,�����ϱ�
			switch(Fn)
			{
//F1�ϱ��ӽڵ���Ϣ
				case 1://F1�ϱ��ӽڵ���Ϣ
					n=p8rxbuff[13];//�����ϱ��ӽڵ������n
					for(i=0;i<n;i++)
					{
						//MeterAddr=MRR(((u32)p8rxbuff)+14+(i*9),6);
						Protocol=p8rxbuff[14+6+(i*9)];
						if(Protocol!=1)
						{
							Protocol=30;
						}
						//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,0x0,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
					}
	#if SoftBug4==0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
	#else
		//�ͼ�ģʽ
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
	#endif
					break;
//F2�ϱ���������
				case 2://F2�ϱ���������
					return;
//F3�ϱ�·�ɹ����䶯��Ϣ
				case 3://F3�ϱ�·�ɹ����䶯��Ϣ
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
					switch(p8rxbuff[13])
					{
						case 1://1Ϊ�����������
							break;
						case 2://2Ϊ�ѱ��������
							if(UARTCtrl->AutoSearchTask==6)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
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
							n=p8rxbuff[13];//�����ϱ��ӽڵ������n
							//CollectorAddr=MRR(((u32)p8rxbuff)+13+1,6);
							Protocol=p8rxbuff[13+1+6];
							if(Protocol!=1)
							{
								Protocol=30;
							}
							//Driver=p8rxbuff[14+9];//�豸���ͣ�01:081 �ɼ�����00:082 �ɼ�����02�������������03H��FFH ����
							p8rxbuff+=13+11;
							for(i=0;i<n;i++)
							{
								m=p8rxbuff[1];//�����ϱ��Ĵӽڵ�1�½ӵĴӽڵ�����n1
								if(m==0)
								{//ֻ�ϱ��ɼ���,������485��
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
									p8rxbuff+=2;
									continue;
								}
								for(x=0;x<m;x++)
								{//����485��
									//MeterAddr=MRR(((u32)p8rxbuff)+2+(7*x),6);
									Protocol=p8rxbuff[2+(7*x)+6];
									if(Protocol!=1)
									{
										Protocol=30;
									}
									//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
								}
								p8rxbuff+=2+(7*x);
							}
			#if SoftBug4==0//0=��������,1=���Ժ�ͼ�(����ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
				//��������
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
			#else
				//�ͼ�ģʽ
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
			#endif
							break;
						case 3://����
						case 5://��˹��
							//CollectorAddr=MRR(((u32)p8rxbuff)+13+1,6);
							Protocol=p8rxbuff[13+1+6];
							if(Protocol!=1)
							{//ͨ��Э�����ͣ�00H��͸�����䣻01H��DL/T645-1997��02H��DL/T645-2007��03H��FFH����
								Protocol=30;
							}
							i=p8rxbuff[14+9];//c)	�豸���ͣ�00H���ɼ�����01H�����ܱ�02H-FFH ����
							if(i==1)
							{//�ϱ����ܱ�
								//AutoSetAFN04F10(PORTn,CollectorAddr,Protocol,0x0,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
							}
							else
							{//�ϱ��ɼ���
								p8rxbuff+=13+11;
								m=p8rxbuff[1];//�����ϱ��Ĵӽڵ�1�½ӵĴӽڵ�����n1
								if(m==0)
								{//ֻ�ϱ��ɼ���,������485��
									//AutoSetAFN04F10(PORTn,0xeeeeeeeeeeee,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
								}
								else
								{//����485��
									for(x=0;x<m;x++)
									{
										//MeterAddr=MRR(((u32)p8rxbuff)+2,6);
										Protocol=p8rxbuff[2+6];
										if(Protocol!=1)
										{//ͨ��Э�����ͣ�00H��͸�����䣻01H��DL/T645-1997��02H��DL/T645-2007��03H��FFH����
											Protocol=30;
										}
										//AutoSetAFN04F10(PORTn,MeterAddr,Protocol,CollectorAddr,0,Phase);//�Զ�����AFN04F10�ĵ��ܱ�����
										p8rxbuff+=6+1;
									}
								}
							}
			#if SoftBug4==0//0=��������,1=���Ժ�ͼ�(����ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
				//��������
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
			#else
				//�ͼ�ģʽ
							Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
			#endif
							break;
					}
					break;
//F5�ϱ��ӽڵ��¼�
				case 5://F5�ϱ��ӽڵ��¼�
				#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
				#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
					//��ǰ��0xfe����
					i=0;
					while(p8rxbuff[i+13+3]==0xfe)
					{
						i++;
					}
					for(x=0;x<p8rxbuff[i+13+3+9];x++)
					{
						p8rxbuff[i+13+3+10+x]-=0x33;
					}
					x=MRR(((u32)p8rxbuff)+i+13+3+10,4);
					if(x==0x04001501)
					{
						//WriteAutoReportWord(PORTn,((u32)p8rxbuff)+i+13+3);//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
						//xxx
						if((UARTCtrl->RMFlag==0x0)||(Terminal_Router->RMTD==0))//0=��������ͣ,��,0=�ǳ���ʱ��
//						if(UARTCtrl->RMFlag==0x0)//0=��������ͣ
						{//0=��������ͣ
							UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=���ڳ������ܱ��¼�
							UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
							Terminal_Router->StopFlags|=(1<<17);//��ͣ·��;b17=�������ϱ��¼�;Ҳ��Ϊ�����ͣʱ���������ϱ��¼�������־
						}
					}
			#if(USER/100)==7//�㽭�û�
					if(p8rxbuff[i+13+3+10]==0xC3)
					{//��ȡ�ɼ����ڵ��ܱ��¼�״̬����ʶC3H��
						//д���ܱ������ϱ�״̬��;
						x=p8rxbuff[i+13+3+12];//�����ϱ����¼��ĵ��ܱ�����1�ֽڣ�HEX��ʽ��
						for(n=0;n<x;n++)
						{
							if(p8rxbuff[i+13+3+13+8*n+6]==1)
							{//1�ֽڹ�Լ��1:97,2:07��
								m=Check_AFN04F10_Addr(31,p8rxbuff[i+13+3+13+8*n]);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
								if(m!=0)
								{//�ҵ�
									m--;
									if(p8rxbuff[i+13+3+13+8*n+7]&0x01)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord),1);
									}
									if(p8rxbuff[i+13+3+13+8*n+7]&0x02)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+1,1);
									}
									if(p8rxbuff[i+13+3+13+8*n+7]&0x04)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+2,1);
									}
									if(p8rxbuff[i+13+3+13+8*n+7]&0x08)
									{
										MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+3,1);
									}
									MWR(1,ADDR_AutoReportWord+(m*LEN_AutoReportWord)+96,1);
								}
							}	
						}
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
						if(UARTCtrl->RMFlag==0x0)//0=��������ͣ,1=���������
						{//0=��������ͣ
							UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=���ڳ������ܱ��¼�
							UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
							Terminal_Router->StopFlags|=(1<<17);//��ͣ·��;b17=�������ϱ��¼�;Ҳ��Ϊ�����ͣʱ���������ϱ��¼�������־
						}
					}
			#endif
				#endif
				#endif
					break;
			}
	  	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MR((u32)p8txbuff,(u32)p8rxbuff,10);//����10�ֽڽ���ͷ������ͷ
			MWR(0xffffffff,((u32)p8txbuff)+128,6);
			Uart_3762_TxData(PORTn,0,1,6);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			break;
	}	
	
}
#endif







