
//�����ϱ���Ҫ�¼�
#include "Project.h"
#include "Terminal_Uart_AutoEvent.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"





void Terminal_Uart_AutoEvent(void)//�����ϱ���Ҫ�¼�
{
	Terminal_AutoEvent(GPRSPORT);//�����ϱ���Ҫ�¼�,���UART�˿ں�
	Terminal_AutoEvent(ENETPORT);//�����ϱ���Ҫ�¼�,���UART�˿ں�
#if RS232AutoRePort==1//RS232�������ϱ�0=��ֹ,1=����
	Terminal_AutoEvent(RS232PORT);//�����ϱ���Ҫ�¼�,���UART�˿ں�
#endif

#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	Terminal_AutoEvent_Meter(GPRSPORT);//�����ϱ����ܱ��¼�,���UART�˿ں�
	Terminal_AutoEvent_Meter(ENETPORT);//�����ϱ����ܱ��¼�,���UART�˿ں�
#if RS232AutoRePort==1//RS232�������ϱ�0=��ֹ,1=����
	Terminal_AutoEvent_Meter(RS232PORT);//�����ϱ����ܱ��¼�,���UART�˿ں�
#endif
#endif
}


const u8 AutoEvent_Example[]=
{
	0x68,
	16,0,
	16,0,
	0x68,
	0xc0,//������(DIR=1����,������־λPRM=1,��������4=���ͨM�޻ش�,����Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1)10=����M��Ӧ)
	0,0,//����������
	0,0,//�ն˵�ַ
	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	0xe,//AFN=0E
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON��=0
	0,0,//Pn=0
	0x01,0,//Fn=1��Ҫ�¼�
	0,//��ǰ��Ҫ�¼�������EC1
	0,//��ǰһ���¼�������EC2
	0,//��֡���Ĵ��͵��¼���¼��ʼָ��Pm
	0,//��֡���Ĵ��͵��¼���¼����ָ��Pn
	//...
	//cs
	//0x16
};



void Terminal_AutoEvent(u32 PORTn)//�����ϱ���Ҫ�¼�,���UART�˿ں�
{
	u32 i;
	u32 x;
	u32 y;
	u32 Len;
	u32 T;
	u8 * p8;
	u16 * p16;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u32 LEN_UARTnTx;
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
	u32 f;
	u32 z;
#endif
#if (USER/100)==5  //�Ϻ� ���ϱ��ָ����¼�
	u32 s;
	u32 k;
#endif
	
	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
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
				
				if(UARTCtrl->HostACK!=0x0)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
				{	
					if(UARTCtrl->HostACK!=4)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					{//�����������ϱ��ȴ�ȷ�Ϸ���
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//�ȴ���վȷ���붨ʱ��!=0
					}
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					i=MRR(ADDR_AFN04F1+3,1);
					i>>=4;
					i&=0x3;
					if(UARTCtrl->HostCount>i)
					{//�ط�������
						UARTCtrl->HostCount=0;
						if(PORTn==GPRSPORT)
						{
							MC(0,ADDR_AFN0CF7_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
	#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_AFN0CF7_FM+5,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
	#else
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_AFN0CF7_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
	#endif
						if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
						{
							MC(0,ADDR_AFN0CF7_FM+7,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
					}
				}
				if(MRR(ADDR_AFN05F29,1)!=0x55)
				{//�������ն������ϱ�
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
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
				//�ϴ�
				p8=(u8 *)(ADDR_DATABUFF);
				MR(ADDR_DATABUFF,ADDR_AFN0CF7_FM,2);//���¼�����
				if(PORTn==GPRSPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
				}
	#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+4;
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_AFN0CF7_FM+6;
				}
				MR(ADDR_DATABUFF+2,i,2);//���¼�����
				if(p8[3]!=0x0)
				{//ԭ��?����Ҫ�¼������ϴ�ûȷ��
					i=p8[2]+p8[3];//�����ϴ�����
					x=p8[3];//�������ϴ�����
				}
				else
				{
					i=p8[2];//�����ϴ�����
					x=p8[2];//�������ϴ�����
				}
				if(x==0x0)
				{//���ϴ���=0
					return;
				}
				T=i;//�����ϴ�����
				if(i>p8[0])
				{
					i=(256+p8[0])-i;
				}
				else
				{
					i=p8[0]-i;
				}
				MR((u32)p8txbuff,(u32)AutoEvent_Example,22);
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
				MR(((u32)p8txbuff)+18,ADDR_AFN0CF7_FM,2);//�¼�����
				
				if(UARTCtrl->HostCount==0)
				{//�����ظ�֡
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����ʱ��)
				}
				else
				{//���ظ�֡
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
				}
				p8txbuff[20]=i;//��֡���Ĵ��͵��¼���¼��ʼָ��Pm
				p8txbuff[21]=i;//��֡���Ĵ��͵��¼���¼����ָ��Pn
				p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��
				Len=16;
			#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
				x=1;//ÿֻ֡��1��ERC
			#endif
				for(;x!=0x0;x--)
				{
					y=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+1,1);
					y+=2;
				#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
					z=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT),3);
					if((z&0xff)==14)
					{//ERC14
						z>>=16;
					#if ((USER/100)==7)//�û���ʾ���㽭	
						f=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+14,1);
						if(f==0x80)
						{
							f=0;
						}
						else
						{
							z&=0x3;
							f=MRR(ADDR_AFN04F98+12,1);//ͣ���¼������ϱ���־λ	BS8	1	
							f=~f;
							f&=0x3;
							f|=z;
						}
					#else
						z&=0x3;
						f=MRR(ADDR_AFN04F98+12,1);//ͣ���¼������ϱ���־λ	BS8	1	
						f=~f;
						f&=0x3;
						f|=z;
					#endif
						if(f==0)
						{
							y=0;
						}
					}
				#endif
				#if (USER/100)==5  //�Ϻ� ���ϱ��ָ����¼�,ֻ�ϱ�������1�ķ����¼�
					s=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT),1);
					if(((s&0xff)==9)||((s&0xff)==10)||((s&0xff)==11)||((s&0xff)==24)||((s&0xff)==25))
					{
						k=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+8,1);
						k>>=7;
						if((k&0xff)==0)
						{
							y=0;
						}
					}
					if(((s&0xff)==24)||((s&0xff)==25))
					{
						k=MRR(ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT)+7,2);
						if(((k&0x8000)!=0) && ((k&0xfff)!=1))
						{
							y=0;
						}
					}
				#endif
					if((Len+6+2+y)>=LEN_UARTnTx)
					{
						break;
					}
					MR(((u32)p8txbuff)+6+Len,ADDR_AFN0EF1+(i*LEN_TERMINAL_EVENT),y);
					Len+=y;
					i++;
					if(i>255)
					{
						i=0;
					}
					p8txbuff[21]=i;//��֡���Ĵ��͵��¼���¼����ָ��Pn
					if(T!=0)//�����ϴ�����
					{
						T--;//��ǰ��û�ϴ���ȷ����Ҫ�¼�����
					}
					p8[3]++;//��ǰ��?����Ҫ�¼������ϴ���ȷ��
				}
				p8[2]=T;//��ǰ��û�ϴ���ȷ����Ҫ�¼�����
				p8txbuff[1]=Len;
				p8txbuff[2]=Len>>8;
				if((MRR(ADDR_AFN04F1+4,1)&0x04)!=0x0)
				{//��Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1���ı�־
					p8txbuff[6]|=10;//������10=����M��Ӧ֡(����1������)
					p8txbuff[13]|=0x10;//CON=1
					UARTCtrl->HostACK=4;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					UARTCtrl->HostCount+=1;//�ط�����
				}
				else
				{
					p8txbuff[6]|=4;//������4=���ͨM�޻ش�
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��=0
				}
				if(PORTn==GPRSPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
					MWR(0,ADDR_ADCFlags,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
	#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+4;
					MWR(0,ADDR_ADCFlags+1,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_AFN0CF7_FM+2;
					MWR(0,ADDR_ADCFlags,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_AFN0CF7_FM+6;
					MWR(0,ADDR_ADCFlags+2,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
			
			#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
				if(Len==16)
				{
					UARTCtrl->HostCount=0;//�ط�����
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��=0
					MW(ADDR_DATABUFF+2,i,2);//�ϴ�����
					return;
				}
			#endif
				MW(ADDR_DATABUFF+2,i,2);//�ϴ�����
				if(UARTCtrl->HostCount==1)
				{//�����ط�
					UARTCtrl->TxSEQ+=1;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				}
				UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
				UARTCtrl->TxByte=(Len+8);//�跢���ֽ���
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				
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
			}
		}
	}
}


#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
const u8 AutoEventMeter_Example[]=
{
	0x68,
	21,0,
	21,0,
	0x68,
	0xc0,//������(DIR=1����,������־λPRM=1,��������4=���ͨM�޻ش�,����Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1)10=����M��Ӧ)
	0,0,//����������
	0,0,//�ն˵�ַ
	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	0x10,//AFN=0x10
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON��=0
	0,0,//Pn=0
	0x02,0x01,//Fn=10ת����վֱ�ӶԵ�������������������
	0,//�˿ں�
	0,0,0,0,0,0,//���ܱ��ַ
	6,//ת�������ʶ,6=���ܱ����������ϱ�
	0,//���ݱ�ʶ����
	
	//��1�����ݱ�ʶ���ֽ���
	//��1�����ݱ�ʶ
	//��1�����ݱ�ʶ������
	//...
	
	//cs
	//0x16
};


void Terminal_AutoEvent_Meter(u32 PORTn)//�����ϱ����ܱ��¼�,���UART�˿ں�
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Len;
	u32 T;
	u8 * p8;
	u16 * p16;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u32 LEN_UARTnTx;
#if AutoEvent_Meter_Mode==1//�����ϱ����ܱ��¼�ģʽ:0=ÿ�ɼ�һ�����ϱ�,1=�ɼ�����������ϱ�
	u32 m;
	u32 n;
	u32 p;
	u32 di;
#endif

	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
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
				
				if(UARTCtrl->HostACK!=0x0)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
				{	
					if(UARTCtrl->HostACK!=6)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					{//�����������ϱ��ȴ�ȷ�Ϸ���
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//�ȴ���վȷ���붨ʱ��!=0
					}
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					i=MRR(ADDR_AFN04F1+3,1);
					i>>=4;
					i&=0x3;
					//i=16;//���֡���к�0-15
					if(UARTCtrl->HostCount>i)
					{//�ط�������
						UARTCtrl->HostCount=0;
						if(PORTn==GPRSPORT)
						{
							MC(0,ADDR_MeterEventCount_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
				#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_MeterEventCount_FM+5,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
				#else
						if(PORTn==ENETPORT)
						{
							MC(0,ADDR_MeterEventCount_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
				#endif
						if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
						{
							MC(0,ADDR_MeterEventCount_FM+7,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
						}
					}
			#if (USER/100)==5//�Ϻ�
					//��ʱû����,Ϊ��֤�¼��ط�ʱESQ��ͬ,�м䲻������֡
					Terminal_Ram->GPRSHeartBeat_S_Timer=60;//5 GPRS�����붨ʱ��
					Terminal_Ram->ENETHeartBeat_S_Timer=60;//2 ��̫�������붨ʱ��
			#else
					//��ʱû����,��������֡
					Terminal_Ram->GPRSHeartBeat_S_Timer=0;//5 GPRS�����붨ʱ��
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 ��̫�������붨ʱ��
			#endif
				}
//				if(MRR(ADDR_AFN05F29,1)!=0x55)
//				{//�������ն������ϱ�
//					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
//					UARTCtrl->HostCount=0;
//					return;
//				}
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
				//�ϴ�
				p8=(u8 *)(ADDR_DATABUFF);
				MR(ADDR_DATABUFF,ADDR_MeterEventCount_FM,2);//���¼�����
				if(PORTn==GPRSPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
				}
	#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+4;
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_MeterEventCount_FM+6;
				}
				MR(ADDR_DATABUFF+2,i,2);//���¼�����
				if(p8[3]!=0x0)
				{//ԭ��?����Ҫ�¼������ϴ�ûȷ��
					i=p8[2]+p8[3];//�����ϴ�����
					x=p8[3];//�������ϴ�����
				}
				else
				{
					i=p8[2];//�����ϴ�����
					x=p8[2];//�������ϴ�����
				}
				if(x==0x0)
				{//���ϴ���=0
					return;
				}
				T=i;//�����ϴ�����
				if(i>p8[0])
				{
					i=(256+p8[0])-i;
				}
				else
				{
					i=p8[0]-i;
				}
				MR((u32)p8txbuff,(u32)AutoEventMeter_Example,27);
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
				MR(((u32)p8txbuff)+18,ADDR_AFN0CF7_FM,2);//�¼�����
				
				if(UARTCtrl->HostCount==0)
				{//�����ظ�֡
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����ʱ��)
				}
				else
				{//���ظ�֡
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
				}
				p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��
				Len=21;
			#if ((USER/100)==5)//�Ϻ��û�
				for(;x!=0x0;x--)
				{
					if(Len!=21)
					{
						break;//��ÿ1�����ϱ�
					}
					y=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+7,3);//ȡ����
					z=y&0xff;//����(ֵ4+3��4+6��4+18��4+3+4+3����ֵ��Ч,�������������ۼ�ʱ��,4+3+4+3�е��������ۼ�ʱ�����ݱ�ʶ������
					y>>=8;//����(1-10���¼�����)
					if((z!=7)&&(z!=10)&&(z!=22)&&(z!=14))
					{
						z=0;//�����������ȴ�
					}
					if(y>255)
					{
						y=255;//�ϱ����ݳ���ֻ��1BYTE
					}
					if((Len+6+2+(z+y+2))>=LEN_UARTnTx)
					{
						break;
					}
					if(Len==21)
					{//װ���1����ʶ
						//ֱ��ȡ��˿ں�,���ܱ��ַ
						MR(((u32)p8txbuff)+18,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
					}
					else
					{//������ʶ
						//�Ƚ϶˿ں�,���ܱ��ַ
						MR(ADDR_DATABUFF+256,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
						if(DataComp(ADDR_DATABUFF+256,((u32)p8txbuff)+18,7)!=0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
						{//����ͬ
							break;
						}
					}
					//ȡ��������
					if(z!=0)
					{
						if(z==14)
						{
							p8txbuff[6+Len]=7;//����
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10,7);
							p8txbuff[26]++;//���ݱ�ʶ����
							Len+=(1+7);
							p8txbuff[6+Len]=7;//����
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10+7,7);
							p8txbuff[26]++;//���ݱ�ʶ����
							Len+=(1+7);
						}
						else
						{
							p8txbuff[6+Len]=z;//����
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10,z);
							p8txbuff[26]++;//���ݱ�ʶ����
							Len+=(1+z);
						}
					}
					//ȡ�¼�
					p8txbuff[6+Len]=y;//����
					MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+32,y);
					p8txbuff[26]++;//���ݱ�ʶ����
					Len+=(1+y);
					i++;
					if(i>255)
					{
						i=0;
					}
					if(T!=0)//�����ϴ�����
					{
						T--;//��ǰ��û�ϴ���ȷ����Ҫ�¼�����
					}
					p8[3]++;//��ǰ��?����Ҫ�¼������ϴ���ȷ��
				}
			#else//#if ((USER/100)==5)//�Ϻ��û�
			//�����Ϻ�
				for(;x!=0x0;x--)
				{
				#if AutoEvent_Meter_Mode==0//�����ϱ����ܱ��¼�ģʽ:0=ÿ�ɼ�һ�����ϱ�,1=�ɼ�����������ϱ�
					if(Len!=21)
					{
						break;//ÿ1�����ϱ�
					}
				#endif
					y=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+7,3);//ȡ����
					z=y&0xff;//����(ֵ4+3��4+6��4+18����ֵ��Ч,��������)
					y>>=8;//�ܳ���(1-10���¼�����)
				#if(USER/100)!=7//�����㽭ϵ
					if((z!=7)&&(z!=10)&&(z!=22))
					{
						z=0;//�����������ȴ�
					}
				#else
					if((z!=7)&&(z!=10)&&(z!=22)&&(z!=6)&&(z!=5))
					{
						z=0;//�����������ȴ�
					}
				#endif
					if(y>(LEN_MeterEvent-10-22))
					{
						y=(LEN_MeterEvent-10-22);//FLASH�洢ʱ�����Ч���ݳ���
					}
					if(y!=0)
					{
						y--;//1�ֽ��ܱ�ʶ����
					}
					if((Len+6+2+(z+y+2))>=LEN_UARTnTx)
					{
						break;
					}
					if(Len==21)
					{//װ���1����ʶ
						//ֱ��ȡ��˿ں�,���ܱ��ַ
						MR(((u32)p8txbuff)+18,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
					#if AutoEvent_Meter_Mode==1//�����ϱ����ܱ��¼�ģʽ:0=ÿ�ɼ�һ�����ϱ�,1=�ɼ�����������ϱ�
						if((p8[2]+p8[3])<(1+10))//�����ϴ�����
						{
							if(p8txbuff[18]==2)
							{//RS485-1
								n=RS485_1PORT;
							}
							if(p8txbuff[18]==3)
							{//RS485-2
								n=RS485_2PORT;
							}
							if(p8txbuff[18]==31)
							{//�ز���
								n=RS485_4PORT;
							}
							n=Check_AutoReportWord(n);//�����ܱ������ϱ�״̬��;����:0=��,!=0 B31-B16Ϊ�������(1��ʼ),B15-B0ΪҪ��ȡ�������ϱ�״̬��λ��(0-95)
							if(n!=0)
							{
								return;
							}
						}
					#endif
					}
					else
					{//������ʶ
						//�Ƚ϶˿ں�,���ܱ��ַ
						MR(ADDR_DATABUFF+256,ADDR_MeterEvent+(i*LEN_MeterEvent),7);
						if(DataComp(ADDR_DATABUFF+256,((u32)p8txbuff)+18,7)!=0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
						{//����ͬ
							break;
						}
					}
					//ȡ��������
					if(z!=0)
					{
						p8txbuff[6+Len]=z;//����
						MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+10,z);
					#if AutoEvent_Meter_Mode==0//�����ϱ����ܱ��¼�ģʽ:0=ÿ�ɼ�һ�����ϱ�,1=�ɼ�����������ϱ�
						p8txbuff[26]++;//���ݱ�ʶ����
						Len+=(1+z);
					#else
						//�鷢�������Ƿ�����
						di=MRR(((u32)p8txbuff)+6+Len+1,4);
						p=6+21;
						for(n=0;n<p8txbuff[26];n++)
						{
							m=MRR(((u32)p8txbuff)+p+1,4);
							if(m==di)
							{
								//�����·�������
								MR(((u32)p8txbuff)+p+1,((u32)p8txbuff)+6+Len+1,z);
								break;
							}
							p+=(p8txbuff[p]+1);
						}
						if(n>=p8txbuff[26])
						{//����
							p8txbuff[26]++;//���ݱ�ʶ����
							Len+=(1+z);
						}
					#endif
					}
					//ȡ�¼�
				#if ((USER/100)==10)//�ӱ��û�
				//����Ŧ���¼��ӱ��ն�ֻ���ϱ���һ���������ݿ��Ǵ������ڶ��������¼���¼����Ҫ��
					if(MRR(((u32)p8txbuff)+28,4)!=0x03300E00)
					{//���ǿ���ť���¼�
						MR(((u32)p8txbuff)+6+Len,ADDR_MeterEvent+(i*LEN_MeterEvent)+32+1,y);
						z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,1);//ȡ���ݱ�ʶ����
						p8txbuff[26]+=z;//�����ݱ�ʶ����
						Len+=y;
					}
				#else
					z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+7,3);//ȡ����
					z&=0xff;//����(ֵ4+3��4+6��4+18����ֵ��Ч,��������)
					if(z==0)
					{
						if(MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,4)==0x04001501)
						{
							p8txbuff[26]++;//�����ݱ�ʶ����
							y++;//ǰ��y--,1�ֽ��ܱ�ʶ����,��д�����ϱ�״̬����Ϊ�¼�ʱ�޴��ֽ���ֱ������֡�洢
							p8txbuff[6+Len]=y;//����
							MR(((u32)p8txbuff)+6+Len+1,ADDR_MeterEvent+(i*LEN_MeterEvent)+32,y);
							Len+=(1+y);
						}
						else
						{
							MR(((u32)p8txbuff)+6+Len,ADDR_MeterEvent+(i*LEN_MeterEvent)+32+1,y);
							z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,1);//ȡ���ݱ�ʶ����
							p8txbuff[26]+=z;//�����ݱ�ʶ����
							Len+=y;
						}
					}
					else
					{
						MR(((u32)p8txbuff)+6+Len,ADDR_MeterEvent+(i*LEN_MeterEvent)+32+1,y);
						z=MRR(ADDR_MeterEvent+(i*LEN_MeterEvent)+32,1);//ȡ���ݱ�ʶ����
						p8txbuff[26]+=z;//�����ݱ�ʶ����
						Len+=y;
					}

				#endif
					i++;
					if(i>255)
					{
						i=0;
					}
					if(T!=0)//�����ϴ�����
					{
						T--;//��ǰ��û�ϴ���ȷ����Ҫ�¼�����
					}
					p8[3]++;//��ǰ��?����Ҫ�¼������ϴ���ȷ��
				}
			#endif//#if ((USER/100)==5)//�Ϻ��û�
				p8[2]=T;//��ǰ��û�ϴ���ȷ����Ҫ�¼�����
				p8txbuff[1]=Len;
				p8txbuff[2]=Len>>8;
//				if((MRR(ADDR_AFN04F1+4,1)&0x04)!=0x0)
//				{//��Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1���ı�־
					p8txbuff[6]|=10;//������10=����M��Ӧ֡(����1������)
					p8txbuff[13]|=0x10;//CON=1
					UARTCtrl->HostACK=6;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
					UARTCtrl->HostCount+=1;//�ط�����
//				}
//				else
//				{
//					p8txbuff[6]|=4;//������4=���ͨM�޻ش�
//					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
//					p8[3]=0;//��ǰ��?����Ҫ�¼������ϴ���ȷ��=0
//				}
				if(PORTn==GPRSPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
					//MWR(0,ADDR_ADCFlags,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
	#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+4;
					//MWR(0,ADDR_ADCFlags+1,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
	#else
				if(PORTn==ENETPORT)
				{
					i=ADDR_MeterEventCount_FM+2;
					//MWR(0,ADDR_ADCFlags,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
	#endif
				if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
				{
					i=ADDR_MeterEventCount_FM+6;
					//MWR(0,ADDR_ADCFlags+2,1);//��Ҫ�¼��ȴ�����0=û,1=��
				}
			
				MW(ADDR_DATABUFF+2,i,2);//�ϴ�����
				if(UARTCtrl->HostCount==1)
				{//�����ط�
					UARTCtrl->TxSEQ+=1;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				}
				UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
				UARTCtrl->TxByte=(Len+8);//�跢���ֽ���
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				
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
			}
		}
	}
}
#endif
