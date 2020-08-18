
//UART376.1
#include "Project.h"
#include "../Terminal/Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"
#include "../Terminal/Terminal_USER11xx.h"


void Uart_3761_RxTx(u32 PORTn)//UART_3761���շ���
{
	u32 i;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)
	{
		case 0://0=���ڽ���
			//���ַ�͹㲥��ַʱ����ת������
			i=UARTCtrl->CascadingBroadcast;//0=��,�������㲥��ַת���Ķ˿ں�(RS485_1PORT,RS485_2PORT,RS485_3PORT)
			if(i!=0)
			{
				UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
				if(UARTCtrlRS485->Lock==0x55)
				{
					if(UARTCtrlRS485->TxByte==0)//�����ֽ���(���ֶ���)
					{
						UARTCtrl->CascadingBroadcast=0;//0=��,�������㲥��ַת���Ķ˿ں�(RS485_1PORT,RS485_2PORT,RS485_3PORT)
						UARTCtrlRS485->Lock=0;
					}
				}
			}
			//����
			i=Uart_3761_Rx(PORTn);//UART_3761����;����0û���,1���,2��ʱ
			if(i==1)
			{
				UARTCtrl->Task=1;
			}
			break;
		case 1://1=�������
			UARTCtrl->Task=0;//0=����
			UARTCtrl->AddrType=0;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			UARTCtrl->TxFrameCount=0;//�Ĵ��֡����ʱ�ѷ���֡����
			UARTCtrl->MulFrameGPCR=0;//��֡ʱͨ�ü����Ĵ���
			UARTCtrl->LockFlags&=(~(7<<4));//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7
			UARTCtrl->SubTask=0;//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
			//����-������ʱ
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS�����ͨ��
				p16timer[0]=0;//��ʱ0MS����
			}
			else
			{//RS232,RS485
			#if (USER/100)==0//���Ժ����
				p16timer[0]=200/10;//20/10;//��ʱ?MS����(��֮֡�����·���м��������33λ)
			#else
				p16timer[0]=50/10;//20/10;//��ʱ?MS����(��֮֡�����·���м��������33λ)
			#endif
			}
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{//ͨ��ģ��
				//��������ʱ(��ȷ�յ��κ�֡,��������ʱ)
				i=MRR(ADDR_AFN04F1+5,1);
				if(i==0x0)
				{
					i=1;//��С1��
				}
				i*=60;
				if((UARTCtrl->TCPUDP&0x30)==0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
				{//�Ƿ�����ģʽ
					i+=60;//��60����ܵ���վ����������ʱ���
				}
				if(PORTn==GPRSPORT)
				{
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS�����붨ʱ��
					if(UARTCtrl->HostACK<=1)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					{
						i=MRR(ADDR_AFN04F8+4,1);
						if(i==0x0)
						{
							i=1;//��С��������ģʽ������ͨ���Զ�����ʱ��1��
						}
						i*=60;
						Terminal_Ram->GPRSAutoDiscon_S_Timer=i;//GPRS��������ģʽ������ͨ���Զ������붨ʱ��
					}
				}
				if(PORTn==ENETPORT)
				{
					Terminal_Ram->ENETHeartBeat_S_Timer=i;//��̫�������붨ʱ��
				}
				//���Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				i=MRR(ADDR_AFN04F58,1);
				if(i==0)
				{
					i=1;//��С1
				}
				i*=60;//ʱ��Ϊ��
				Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
			}
			if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{
				Comm_Ram->BattWork_S_timer=60;//4 ��ع����ػ��붨ʱ��
			}
		#if (USER/100)==11//����ϵ
			i=USER11xx_RxAddr(PORTn);//����ϵ����֡�ն˵�ַ�Ƚ�,4�ֽڳ��ȱ�Ϊ��׼֡��2�ֽ�;����:0=��ַ��ͬ,1=��ַ��ͬ
			if(i==0)
			{
				UARTCtrl->RxByte=0;//rx����=0
				break;
			}
		#endif
			
		#if LOG_1376_1==1//0=û,1=��
			LOG(13761,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
			Uart_3761_Data(PORTn);//UART_3761����
			UARTCtrl->RxByte=0;//rx����=0
			break;
		case 2://2=�ȴ���֡���ݷ���
//			if(UARTCtrl->AddrType>1)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
//			{
//				UARTCtrl->AddrType=0;//����֡�ĵ�ַ����:0=����ַ,1=���ַ,2=�㲥��ַ
//				UARTCtrl->Task=0;
//				break;
//			}
			if(p16timer[0]==0x0)
			{
			#if (USER/100)==11//����ϵ
				USER11xx_TxAddr(PORTn);//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
			#endif
			#if LOG_1376_1==1//0=û,1=��
				LOG(13761,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				UARTCtrl->Task=3;//���ڷ��ͱ�־
				Start_UARTn_Tx(PORTn);//����UART�ڷ���
			}
			else
			{
				if(p16timer[0]>(200/10))
				{//����ת��,����errʱû���ö�ʱֵ!
					p16timer[0]=200/10;
				}
			}
			break;
		case 3://3=��֡�������ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{			
				UARTCtrl->Task=0;//0=���ڽ���
				UARTCtrl->RxByte=0;//rx����=0
				p16timer[0]=0;//��ʱ��ʱ=0;���ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
			}
			break;
			
		case 4://4=��֡�������������
			//����-������ʱ
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS�����ͨ��
				p16timer[0]=0;//��ʱ0MS����
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//��ʱ?MS����(��֮֡�����·���м��������33λ)
			}
			Uart_3761_Data(PORTn);//UART_3761����
			UARTCtrl->RxByte=0;//rx����=0
			break;
		case 5://5=�ȴ���֡���ݷ���
			if(p16timer[0]==0x0)
			{
			#if (USER/100)==11//����ϵ
				USER11xx_TxAddr(PORTn);//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
			#endif
			#if LOG_1376_1==1//0=û,1=��
				LOG(13761,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				UARTCtrl->Task=6;//���ڷ��ͱ�־
				Start_UARTn_Tx(PORTn);//����UART�ڷ���
			}
			else
			{
				if(p16timer[0]>(20/10))
				{//����ת��,����errʱû���ö�ʱֵ!
					p16timer[0]=20/10;
				}
			}
			break;
		case 6://6=��֡�������ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->RxByte=0;//rx����=0
				p16timer[0]=0;//��ʱ��ʱ=0;���ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
				
				UARTCtrl->Task=4;//4=��֡�������������
			}
			break;
		case 7://7=�ȴ���������תΪ����
			UARTCtrl->Task=0;//0=���ڽ���
			UARTCtrl->RxByte=0;//rx����=0
			p16timer[0]=0;//��ʱ��ʱ=0;���ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
			break;
		case 8://8=�ȴ�ͨ������������ת��
		case 9://9=�ȴ���֡����������ʱ
		case 10://10=����ͨ�������ڷ�������
		case 11://11=����ͨ�������ڽ�������
			Forwarding(PORTn);//����ת���������
			break;
		case 20://20=�����������
		case 21://21=����ת����������
		case 22://22=����ת����������
		case 23://22=���ռ�������
			Cascading(PORTn);//�����������
			break;
		default:
			UARTCtrl->Task=0;//0=���ڽ���
			break;
	}
}



void Uart_3761_Data(u32 PORTn)//UART_376����
{
	u32 i;
	u32 x;
	u16 * p16;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;
#if (((Project/100)==2)||((Project/100)==4))//������,�����ն�
	u32 RS485PORT;
	u8 * p8RS485txbuff;
  UARTCtrl_TypeDef * UARTCtrlRS485;
#endif
#if ((Extern_Uart_AFN255|Extern_Uart_AFN04|Extern_Uart_AFN05)!=0)//�ⲿ����:0=I2,1=E1,2=E2,3=	
	u32 *p32;
#endif

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	if(p8rxbuff[6]&0x80)
	{//����վ����֡
		return;
	}
	//����֡������SEQ
	UARTCtrl->RxSEQ=p8rxbuff[13];
	//COPY 14+16�ֽ�����ͷ��������,��Ϊ����ͷ
	MW((u32)p8rxbuff,(u32)p8txbuff,14+16);

	//����֡SEQ
	p8txbuff[13]|=0x60;//����֡��Ϊ��֡TpV,FIR,FIN,CON;PSEQ|RSEQ
	p8txbuff[13]&=0xef;//����֡CON��=0
	//����֡������
	p8txbuff[6]=(p8txbuff[6]&0xf)+0x80;//������DIR=1����,������־λPRM=0,Ҫ�����λACD��=0;�����벻��

	if((p8rxbuff[1]+(p8rxbuff[2]<<8))<12)
	{
		return;//�û������ݳ���<12byte��
	}
	
	//��ַ�Ƚ�
	MR(ADDR_DATABUFF,ADDR_AREACODE,4);//������������,�ն˵�ַ
	i=MRR(ADDR_DATABUFF,4);
	if(i==0xffffffff)
	{
		goto AddrOK;//�ն˵�ַ��Ϊ�㲥��ַ,���Ƚ�
	}
#if (USER/100)==11//����ϵ
	if(Comm_Ram->Factory==0x55)//0x55=����״̬
	{
#endif
		if(Comm_Ram->Factory==0x55)//0x55=����״̬
		{
			if(p8rxbuff[12]==255)
			{//AFN=255
				if((p8rxbuff[16]+(p8rxbuff[17]<<8))==0xc10)
				{//Fn=101д�ն˵�ַ��
					goto AddrOK;
				}
			}
		}
	#if (USER/100)==10//�ӱ����ܵ�ַ�ȽϺ͹㲥��ַ
		i=MRR(((u32)p8rxbuff)+7,4);
		if(i==0x44332211)
		{
			goto AddrOK;
		}
		if(i==0xffffffff)
		{//�㲥��ַ
			//�޳�Ͷ��
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
				if(p8rxbuff[12]!=5)
				{//AFN!=5
					return;
				}
				if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
				{//Pn!=0
					return;
				}
				if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
				{//Fn!=31��ʱ����
					return;
				}
			}
			UARTCtrl->AddrType=3;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			goto AddrOK;
		}
	#endif
	#if ((USER/100)==12)||((USER/100)==13)//�Ĵ����������㲥��ַ
		i=MRR(((u32)p8rxbuff)+7,4);
		if(i==0xffffffff)
		{//�㲥��ַ
			//�޳�Ͷ��
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
				if(p8rxbuff[12]!=5)
				{//AFN!=5
					return;
				}
				if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
				{//Pn!=0
					return;
				}
				if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
				{//Fn!=31��ʱ����
					return;
				}
			}
			UARTCtrl->AddrType=3;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			goto AddrOK;
		}
	#endif
		i=p8rxbuff[9]+(p8rxbuff[10]<<8);//��ַ��
		if(i==0xffff)
		{//�㲥��ַ
			//if(p8rxbuff[12]!=0x0F)//Ӧ�ò㹦����AFN
			{//�����ļ�����
				if((p8rxbuff[11]&0x1)==0x0)
				{//�������ַ��־��
					return;
				}
				//�޳�Ͷ��
				i=MRR(ADDR_AFN05F28,1);
				if(i==0x55)
				{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
					if(p8rxbuff[12]!=5)
					{//AFN!=5
						return;
					}
					if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
					{//Pn!=0
						return;
					}
					if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
					{//Fn!=31��ʱ����
						return;
					}
				}
				//����������Ƚ�
				if((p8rxbuff[7]+p8rxbuff[8])!=0x0)
				{//ע:����������=0ʱ�Զ�����Ϊ�㲥�����������ն˵�ַͨ������
					if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
					{//�����������
						return;
					}
				}
				UARTCtrl->AddrType=3;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			}
		}
		else
		{//�ǹ㲥��ַ
			if((p8rxbuff[11]&0x1)==0x0)
			{//����ַ�Ƚ�
				//���ն�����������͵�ַ�Ƚ�
				if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,4)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				{//���Ǳ��ն�����������͵�ַ
					i=MRR(ADDR_AFN04F37,1);//�ն˼���ͨ�Ų���
					if(i!=0x0)
					{//�м�����
						i=MRR(ADDR_AFN04F37+6,1);
						if((i&0x80)==0x0)
						{//������������
							x=i&0x7;//�������ն˸���
							MR(ADDR_DATABUFF+4,ADDR_AFN04F37+7,(4*x));//�ն˼���ͨ�Ų���
							for(i=0;i<x;i++)
							{
								if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF+4+(4*i),4)==0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
								{
									break;
								}
							}
							if(i!=x)
							{//�ҵ��������ն���ͬ����������͵�ַ
								UARTCtrl->Task=20;//20=�����������
								UARTCtrl->CascadingAddrType=1;//�������ݼ���ַ����0=�޼�������,1=����ַ,2=���ַ,3=�㲥��ַ
								UARTCtrl->CascadingReTxCount=0;//�����ط�����
							}
						}
					}
					return;
				}
			}
			else
			{//��ַΪ���ַ
				//�޳�Ͷ��
				i=MRR(ADDR_AFN05F28,1);
				if(i==0x55)
				{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
					if(p8rxbuff[12]!=5)
					{//AFN!=5
						return;
					}
					if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
					{//Pn!=0
						return;
					}
					if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
					{//Fn!=31��ʱ����
						return;
					}
				}
				//����������Ƚ�
				if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				{//�����������
					return;
				}
				p16=(u16 *)(ADDR_DATABUFF);
				p16[0]=p8rxbuff[9]+(p8rxbuff[10]<<8);//��ַ��
				if(p16[0]==0)
				{//���ַ=0Ϊ��Ч���ַ
					return;
				}
				MR(ADDR_DATABUFF+2,ADDR_AFN04F6,16);
				for(i=0;i<8;i++)
				{
					if(p16[0]==p16[1+i])
					{
						break;
					}
				}
				if(i==8)
				{//���ַ����ͬ
					return;
				}
				UARTCtrl->AddrType=2;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			}
		}
#if (USER/100)==11//����ϵ
	}
#endif

AddrOK:
#if (((Project/100)==2)||((Project/100)==4))//������,�����ն�
	//���ַ�͹㲥��ַʱ����ת��
	if(UARTCtrl->AddrType>1)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
	{
		//����������
		i=MRR(ADDR_AFN04F37+6,1);//����/��������־
		if((i&0x80)==0)
		{//��������
			i=MRR(ADDR_AFN04F37,1);//�ն˼���ͨ�Ŷ˿ں� BIN  1 
			RS485PORT=0;
			switch(i)
			{
				case 2://RS485-1
					if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)!=0))
					{//�����ն�ά��
						RS485PORT=RS485_1PORT;
					}
					break;
				case 3://RS485-2
					if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)!=0)
					{//�����ն�ά��
						RS485PORT=RS485_2PORT;
					}
					break;
				case 4://RS485-3
					RS485PORT=RS485_3PORT;
					break;
			}
			if(RS485PORT!=0)
			{
				UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485PORT);
				if((UARTCtrlRS485->Lock==0x55)||(UARTCtrlRS485->Task!=0))//��������Ѳ������:0=��,1=��������,2=���ڷ���,3=���շ���,100=�ȴ��ϴ�
				{
					UARTCtrl->Task=1;//����1=�������
					return;
				}
				UARTCtrl->CascadingBroadcast=RS485PORT;//0=��,�������㲥��ַת���Ķ˿ں�(RS485_1PORT,RS485_2PORT,RS485_3PORT)
				UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
				p8RS485txbuff=(u8 *)Get_ADDR_UARTnTx(RS485PORT);
				i=p8rxbuff[1]+(p8rxbuff[2]<<8);//�û�����������
				i+=8;
				MW((u32)p8rxbuff,(u32)p8RS485txbuff,i);
				p8RS485txbuff[1]=p8RS485txbuff[3];//�����ֽڻָ�
				p8RS485txbuff[2]=p8RS485txbuff[4];
				UARTCtrlRS485->TxByte=i;//�跢���ֽ���
				//Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				if(RS485PORT==RS485_1PORT)
				{
					Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;
				}
				if(RS485PORT==RS485_2PORT)
				{
					Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;
				}
				UARTCtrlRS485->BpsCtrl=MRR(ADDR_AFN04F37+1,1);
				Init_UARTn(RS485PORT);//��ʼ��UART��,���ͨ�ſ�����������
				Start_UARTn_Tx(RS485PORT);//����UART�ڷ���
			}
		}
	}
#endif
	//���ն�ʵ��ַ���뷢����
	MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);
	p8txbuff[11]&=0xfe;//����ַ��־
	UARTCtrl->Lock=0x55;//��ֹ���������ٴε���
	switch(p8rxbuff[12])//Ӧ�ò㹦����AFN
	{
		case 0://ȷ��/����
			Uart_AFN00(PORTn);//ȷ��/����
			break;
		case 1://��λ
			Uart_AFN01(PORTn);//��λ
			break;
		case 2://��·�ӿڼ��
			Uart_AFN02(PORTn);//��·�ӿڼ��
			break;
		case 3://�м�վ����
			break;
		case 4://���ò���
		#if Extern_Uart_AFN04==0//�ⲿ����:0=I2,1=E1,2=E2,3=;�������úͲ�ѯ
			Uart_AFN04(PORTn);//��������
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[108])(PORTn);//��������
			}
		#endif
			break;
		case 5://��������
		#if Extern_Uart_AFN05==0//�ⲿ����:0=I2,1=E1,2=E2,3=;��������
			Uart_AFN05(PORTn);//��������
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[109])(PORTn);//��������
			}
		#endif
			break;
		case 6://�����֤����ԿЭ��
			Uart_AFN06(PORTn);//�����֤����ԿЭ��
			break;
		case 7://����
			break;
		case 8://���󱻼����ն������ϱ�
			Uart_AFN08(PORTn);//���󱻼����ն������ϱ�
			break;
		case 9://�����ն�����
		#if Extern_Uart_AFN09==0//�ⲿ����:0=I2,1=E1,2=E2,3=;�����ն�����
			Uart_AFN09(PORTn);//�����ն�����
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[111])(PORTn);//�����ն�����
			}
		#endif
			break;
		case 10://��ѯ����
		#if Extern_Uart_AFN04==0//�ⲿ����:0=I2,1=E1,2=E2,3=;�������úͲ�ѯ
			Uart_AFN0A(PORTn);//������ѯ
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[110])(PORTn);//������ѯ
			}
		#endif
			break;
		case 11://������������
			Uart_AFN0B(PORTn);//������������
			break;
		case 12://����1������(ʵʱ����)
		#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
			x=p8rxbuff[14]+(p8rxbuff[15]<<8)+(p8rxbuff[16]<<16)+(p8rxbuff[17]<<24);
			switch(x)
			{
				case 0x19010000://AFN0CF201	P0 
					Uart_AFN0CF201(PORTn);//�Ϻ���������չAFN0CF201��ȡ���һ�γ���ʧ�ܱ����P0
					break;
				case 0x19020000://AFN0CF202	P0
					Uart_AFN0CF202(PORTn); //�Ϻ���������չAFN0CF202��ȡ���һ�γ������ܱ�ʧ�ܱ���ϢP0
					break;
				default:
					Uart_AFN0C(PORTn);//��1����
					break;
			}
		#elif ((USER/100)==12)//�Ĵ��û�
			x=p8rxbuff[16]+(p8rxbuff[17]<<8);
			switch(x)
			{
				case 0x0140://AFN0CF15	P0 
					Uart_AFN0CF15(PORTn);//��չ��ȡSIM���ź�ǿ��
					break;
				case 0x2601://AFN0CF305	P0 
					Uart_AFN0CF305(PORTn);//͸������ִ��״̬
					break;
				case 0x2602://AFN0CF306	Pn 
					Uart_AFN0CF306(PORTn);//͸������ִ�н��
					break;
				case 0xFE80://AFN0CF2040	P0 
					Uart_AFN0CF2040(PORTn);//��չ��ȡԶ��ģ���ź�ǿ��
					break;
				default:
					Uart_AFN0C(PORTn);//��1����
					break;
			}
		#else
			Uart_AFN0C(PORTn);//��1����
		#endif
			break;
		case 13://����2������(��ʷ����)
		#if (USER/100)==8//�û���ʶ��ͨ��
			if((p8rxbuff[16]==0x02)&&(p8rxbuff[17]==0x28))
			{//AFN0DF322
				Uart_AFN0DF322(PORTn);//��������ʱ��������ͳ����ϸ
			}
			else
			{
				if((p8rxbuff[16]==0x01)&&(p8rxbuff[17]==0x28))
				{//AFN0DF321
					Uart_AFN0DF321(PORTn);//��������ʱ��������ͳ����ϸ
				}
				else
				{
					Uart_AFN0D(PORTn);//��2����
				}
			}
		#else
			#if (USER/100)==6//�û���ʶ������
				if(((p8rxbuff[16]==0x80)&&(p8rxbuff[17]==0x31))||((p8rxbuff[16]==0x01)&&(p8rxbuff[17]==0x32))||((p8rxbuff[16]==0x02)&&(p8rxbuff[17]==0x32)))
				{//AFN0DF400��F401��F402
					Uart_AFN0DF40x(PORTn);
				}
				else
				{
					Uart_AFN0D(PORTn);//��2����
				}
			#else
				Uart_AFN0D(PORTn);//��2����
			#endif
		#endif
			break;
		case 14://����3������(�¼�����)
			Uart_AFN0E(PORTn);//��3����(�¼�)
			break;
		case 15://�ļ�����
			Uart_AFN0F(PORTn);//�ļ�����
			break;
		case 16://����ת��
			Uart_AFN10(PORTn);//����ת��
			break;
			
		case 255://��������(��չ)
		#if Extern_Uart_AFN255==0//�ⲿ����:0=I2,1=E1,2=E2,3=
			Uart_AFN255(PORTn);//��������(��չ)
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[100])(PORTn);//��������(��չ)
			}
		#endif
			break;
		default:
			break;
	}
	UARTCtrl->Lock=0;//�����ֹ���������ٴε���
}











