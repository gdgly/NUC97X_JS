

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/PROXY_TASK.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"


void PROXY_TASK_RS485(u32 PORTn)//��������RS485
{
	u32 i;
	u32 SourProtocol;
	u8* p8tx;
	u8* p8rx;
	u16* p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
	switch(i)
	{
		default:
			UARTCtrl->ProxyTxRxByte=0;
			UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			break;
		case 1://�д���
			UARTCtrl->ProxyTask=3;
			break;
		case 2://��ʱ
			UARTCtrl->ProxyTxRxByte=0;
			UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			break;
		case 3://��������
			if(p16timer[0]!=0)//��֡����������ʱ
			{
				return;
			}
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			p8rx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
			i=UARTCtrl->ProxyTxRxByte;
			if(i>LEN_UART2Tx)
			{
				i=LEN_UART2Tx;
			}
			MR((u32)p8tx,(u32)p8rx,i);
			UARTCtrl->TxByte=i;
			i=UARTCtrl->BpsCtrl;
			UARTCtrl->BpsCtrl=UARTCtrl->ProxyBpsCtrl;//����ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
		#if DEBUG_RS485_1toRS232==1//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->ProxyRMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->ProxyRMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			
			UARTCtrl->BpsCtrl=i;
			RS485_TxLED(PORTn);//RS485 LED����ָʾ��
			PROXY_ADDR_Display(PORTn);//�����ַ��ʾ
			UARTCtrl->ProxyTask++;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			break;
		case 4://���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				p16timer[0]=RMRS485OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->ProxyTask++;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			}
			break;
		case 5://���ڽ���
			SourProtocol=UARTCtrl->RMprotocol;
			UARTCtrl->RMprotocol=UARTCtrl->ProxyRMprotocol;
			i=RM_Rx(PORTn);//�������֡;���:PORTn;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
			switch(i)
			{
				case 0://0û���
					break;
				case 1://1���
					p16timer[0]=30/10;//��֡����������ʱ20MS��
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					p8tx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
					i=UARTCtrl->RxByte;
					if(i>LEN_UART2Rx)
					{
						i=LEN_UART2Rx;
					}
					MR((u32)p8tx,(u32)p8rx,i);
					UARTCtrl->ProxyTxRxByte=i;
					UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				#if DEBUG_RS485_1toRS232==1//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
					if(PORTn==RS485_1PORT)
					{
						LOGRS232(UARTCtrl->ProxyRMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
					}
				#endif
				#if DEBUG_RS485_2toRS232==1//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
					if(PORTn==RS485_2PORT)
					{
						LOGRS232(UARTCtrl->ProxyRMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
					}
				#endif
					break;
				default://2��ʱ
					UARTCtrl->ProxyTxRxByte=0;
					UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
					break;
			}
			UARTCtrl->RMprotocol=SourProtocol;
			break;
	}
}

#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
void PROXY_TASK_3762(u32 PORTn)//��������376.2
{
	u32 i;
	u32 x;
	u32 Len;
	u16 * p16timer;
	u8 * p8rx;
	u8* p8tx;
  UARTCtrl_TypeDef* UARTCtrl;
	UARTCtrl_TypeDef* UARTCtrlProxy;
#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
	u8 * p8;
	u64 d64;
#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
	if(i)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
	{
		UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(UARTCtrl->ProxyPORTn);
		switch(i)
		{
			default:
				UARTCtrl->ProxyTxRxByte=0;
				UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				break;
			case 1://�д���
				UARTCtrl->ProxyTask=3;
				break;
			case 2://��ʱ
				Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				UARTCtrl->ProxyTxRxByte=0;
				UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				break;
			case 3://��ͣ·��
				if(Terminal_Router->RouterCtrl!=0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				{
					break;
				}
				Terminal_Router->StopFlags|=(1<<2);//��ͣ·�ɱ�־
				Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				UARTCtrl->ProxyTask++;
				break;
			case 4://��ͣ·�ɽ���
				if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				{
					//��ս���FIFO
					while(1)
					{
						i=UART_3762_Rx(PORTn);//UART 3762����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
						if(i==1)
						{//1=���1֡����
//							Uart_3762_RxData(PORTn);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//							return;
							continue;
						}
						break;
					}
					UARTCtrl->ProxyTask++;
				}
				break;
			case 5://·��ת������
				p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
				if(UARTCtrlProxy->RMprotocol==128)//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
				{//ת��376.2֡
					Len=UARTCtrl->ProxyTxRxByte;
					if(Len>LEN_UART2Tx)
					{
						Len=LEN_UART2Tx;
					}
					p8rx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
					MR((u32)p8tx,(u32)p8rx,Len);//ת������
					UARTCtrl->TxByte=Len;//�����ֽ���(���ֶ���)
				}
				else
				{//����376.2֡
					p8tx[0]=0x68;
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
					p8tx[6]=100;//Ԥ��Ӧ���ֽ���
					p8tx[7]=0;
					p8tx[8]=0;
					UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
					p8tx[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
					MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
					MR(((u32)p8tx)+16,(u32)&UARTCtrlProxy->RMAddr,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
					Data_Inverse(((u32)p8tx)+16,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
					p8tx[22]=0x13;//AFN=13H
					p8tx[23]=1;//Fn=1
					p8tx[24]=0;
					//p8tx[25]=0;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
					if(UARTCtrlProxy->RMprotocol==128)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
					{
						p8tx[25]=0;//0͸������
					}
					else
					{
					#if (USER%100)==0//���Ժ
						p8tx[25]=UARTCtrlProxy->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
					#else
						//p8tx[25]=0;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
						switch(UARTCtrlProxy->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
						{
							case 1://DL/T645-1997��1��
							case 2://DL/T645-2007��2��
								p8tx[25]=UARTCtrlProxy->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
								break;
							case 3://DL/T698.45��3��
								if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��
								{//������698����͸��
									p8tx[25]=0;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
								}
								else
								{
									p8tx[25]=UARTCtrlProxy->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
								}
								break;
							case 4://CJ/T 188-2004��4��
							default://
								p8tx[25]=UARTCtrlProxy->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
								break;
						}
					#endif
					}
					p8tx[26]=0;//ͨ����ʱ��ر�־
					p8tx[27]=0;//�����ڵ���=0
					i=UARTCtrl->ProxyTxRxByte;
					if(i>(LEN_UART2Tx-30))
					{
						i=(LEN_UART2Tx-30);
					}
					p8tx[28]=i;//DL645���ĳ���(ǰ��0xfe������)
					p8rx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
					MR(((u32)p8tx)+29,(u32)p8rx,i);//ת������(ǰ��0xfe������)
					Len=29+i+2;//���ܷ����ֽ���
					p8tx[1]=Len;
					p8tx[2]=Len>>8;
				#if SEL_RouterRunMode3ADDR==0//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
					
				#else
					if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
					{//΢��������,������ַ�òɼ�����ַ
						x=AddrToMeterSetNo((u16*)&UARTCtrlProxy->RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
						p8=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
						p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ
						if((x>=NUM_RMmax)||(p8==0))
						{//û�ҵ��������
							Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
							Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
							UARTCtrl->ProxyTxRxByte=0;
							UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							break;
						}
						//d64=MRR((u32)p8+5,6);//ȡ���òɼ�����ַ
						d64=Get_TSA_Value(p8+3);//�õ�TSA�Ĳ�����6�ֽ�ͨ�ŵ�ַ,����˳�򲻱�,���:p8=TSA���ͺ���ֽڣ�����Чֵ��=0
						if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
						{//�ɼ�����ַ��Ч
							d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							MWR(d64,((u32)p8tx)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						}
					}
				#endif
					Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					UARTCtrl->TxByte=Len;//�����ֽ���(���ֶ���)
				}
				
			#if LOG_1376_2==1
				LOG(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_RS485_4toRS232==1//0=��,1=��;����:RS485_4(�ز�)���շ����ݴ�ӡ��RS232��
				LOGRS232(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				
				UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
				Start_UARTn_Tx(PORTn);//����UART�ڷ���
				
				UARTCtrl->ProxyTask++;
				PROXY_ADDR_Display(PORTn);//�����ַ��ʾ
				break;
			case 6://���ڷ���
				if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
				{
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					p16timer[0]=UARTCtrl->TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
					UARTCtrl->ProxyTask++;
				}
				break;
			case 7://����
				i=UART_3762_Rx(PORTn);//UART 3762����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
				switch(i)
				{
					default:
						break;
					case 2:
						UARTCtrl->ProxyTask=2;//��ʱ
						break;
					case 1:
						UARTCtrl->Task=0;//UART_3762_Rx()��������1֡����UARTCtrl->Task=1��������0����������仯UARTCtrl->Taskֵ
						p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
						p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
						//֡��űȽϣ����Ժ���������ʼʱ����Ӧ,���Կ�ʼ�����Ӧ��һ֡����
						if(p8rx[9]!=p8tx[9])
						{
							UARTCtrl->ProxyTask=7;//����
							UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
							p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
							p16timer[0]=30000/10;//ע:���Ժ��������ȴ�90��
							return;
						}
					#if LOG_1376_2==1
						LOG(13762,0,PORTn,p8rx,UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					#endif
					#if DEBUG_RS485_4toRS232==1//0=��,1=��;����:RS485_4(�ز�)���շ����ݴ�ӡ��RS232��
						LOGRS232(13762,0,PORTn,p8rx,UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					#endif
						if(UARTCtrlProxy->RMprotocol==128)//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
						{//ת��376.2֡
							Len=UARTCtrl->RxByte;
							p8tx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
							MW((u32)p8rx,(u32)p8tx,Len);
						}
						else
						{//����376.2֡
							if(UARTCtrl->RxByte<=16)
							{//���ϻش�
								UARTCtrl->ProxyTask=2;//��ʱ
								break;
							}
							Len=p8rx[28];//����645֡�ܳ���
							i=MRR(((u32)p8rx)+22,3);
							if(i!=0x113)
							{//AFN=13H,Fn=1
								UARTCtrl->ProxyTask=7;//����
								UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								if(i==0x0200)
								{//AFN=00,Fn=2ֱ�ӷ���֡(΢���������д����)
									p16timer[0]=6000/10;//6�볬ʱʱ��
								}
								else
								{
									//p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
									p16timer[0]=30000/10;//ע:���Ժ��������ȴ�90��
								}
								return;
							}
							//�鵱ǰ����ǰ��0xfe����
							for(x=0;x<14;x++)
							{
								if(p8rx[29+x]!=0xfe)
								{
									break;
								}
							}
							if(Len>x)
							{
								Len-=x;
							}
							//��ַ�Ƚ�
/*
					#if (USER/100)!=0//���Ժ���Բ��Ƚϵ�ַ,������ģ����bug
							MR(ADDR_DATABUFF,(u32)&UARTCtrlProxy->RMAddr,6);
							Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
						#if SEL_RouterRunMode3ADDR==0//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
						#else
							if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{//΢��������,������ַ�òɼ�����ַ
								x=AddrToMeterSetNo((u16*)&UARTCtrlProxy->RMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
								p8=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
								p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ
								if((x>=NUM_RMmax)||(p8==0))
								{//û�ҵ��������
									UARTCtrl->ProxyTask=7;//����
									UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
									p16timer[0]=6000/10;//6�볬ʱʱ��
									break;
								}
								MR(ADDR_DATABUFF,(u32)p8+5,6);
								Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
							}
						#endif
							x=DataComp(((u32)p8rx)+10,ADDR_DATABUFF,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							if(x!=0)
							{//����ͬ
								UARTCtrl->ProxyTask=7;//����
								UARTCtrl->RxByte=0;//�����ֽ���(���ֶ���)
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								if(i==0x0200)
								{//AFN=00,Fn=2ֱ�ӷ���֡(΢���������д����)
									p16timer[0]=6000/10;//6�볬ʱʱ��
								}
								else
								{
									p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
								}
								return;
							}
					#endif
*/
							p8tx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
							MW(((u32)p8rx)+28+1+x,(u32)p8tx,Len);
						}
						
						Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
						UARTCtrl->ProxyTxRxByte=Len;
						UARTCtrl->ProxyTask=0;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						break;
				}
				break;
		}
	}
}
#endif










