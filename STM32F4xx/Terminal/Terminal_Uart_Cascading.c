
//��������
#include "Project.h"
#include "Terminal_Uart_Cascading.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




void Cascading(u32 PORTn)//�����������
{
/*
	u32 i;
	u32 RS485PORT;
	u16 *p16;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8RS485txbuff;
	u8 * p8fifo;
	u16 * p16fifo;
	u16 * p16timer;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnTx;
	u32 p0;
	u32 p1;
	u32 TxByte;
	u32 cs;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	switch(MEMReadReg(ADDR_AFN04F37,1))//�ն˼���ͨ�Ų���
	{
		case 2://RS485-1
			RS485PORT=RS485_1PORT;
			break;
		case 3://RS485-2
			RS485PORT=RS485_2PORT;
			break;
		case 4://RS485-3
			RS485PORT=RS485_3PORT;
			break;
		default:
			UARTCtrl->Task=0;
			return;
	}
  UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485PORT);
  p8RS485txbuff=(u8 *)Get_ADDR_UARTnTx(RS485PORT);
	switch(UARTCtrl->Task)
	{
		case 20://20=�����������
			if(UARTCtrlRS485->Lock==0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			{
				break;
			}
			if(UARTCtrlRS485->Task!=0)//��������Ѳ������:0=��,1=��������,2=���ڷ���,3=���շ���,100=�ȴ��ϴ�
			{
				if(UARTCtrlRS485->Task==100)
				{
					break;
				}
			}
			UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);//�û�����������
			i+=8;
			MEMWrite((u32)p8rxbuff,(u32)p8RS485txbuff,i);
			p8RS485txbuff[1]=p8RS485txbuff[3];//�����ֽڻָ�
			p8RS485txbuff[2]=p8RS485txbuff[4];
			UARTCtrlRS485->TxByte=i;//�跢���ֽ���
			UARTCtrl->Task=21;
			break;
		case 21://21=����ת����������
			Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			UARTCtrlRS485->BpsCtrl=MEMReadReg(ADDR_AFN04F37+1,1);
			Init_UARTn(RS485PORT);//��ʼ��UART��,���ͨ�ſ�����������
			p16=(u16 *)Get_ADDR_UARTn(RS485PORT);//�õ�UART�����׵�ַ
			p16[0]=0;//FIFOָ��
			p16[1]=0;//FIFO�Ѷ�ָ��
			p16[2]=0;//���ָ��==��ָ��,���ʡʱ
			Start_UARTn_Tx(RS485PORT);//����UART�ڷ���
			UARTCtrl->Task=22;
			break;
		case 22://22=����ת����������
			if(Wait_UARTn_Tx(RS485PORT)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
//				i=MEMReadReg(ADDR_AFN04F37+2,1);//���յȴ����ĳ�ʱʱ�� BIN 100ms 1 
//				if(i==0x0)
//				{
//					i=1;
//				}
//				i*=(100/10);
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485PORT);
				p16timer[0]=2000/10;//���ճ�ʱʱ��
				UARTCtrl->TxByte=0;//�����ֽ���=0
				UARTCtrl->Task=23;//23=���ռ�������
			}
			break;
		case 23://23=���ռ�������(������ԭfifo�����в�����rxbuff,�����������Ѳ������������ݳ�ͻ!)
			if((p8rxbuff[12]==0x0))
			{//Ӧ�ò㹦����AFN=0ȷ��/����,�޷�������
				UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
				UARTCtrl->Task=0;
				return;
			}
			
			p8fifo=(u8 *)(Get_ADDR_UARTn(RS485PORT)+6);
			p16fifo=(u16 *)Get_ADDR_UARTn(RS485PORT);
			p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485PORT);
			LEN_UARTnIntRx=Get_LEN_UARTnIntRx(RS485PORT);
			LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
			
			p0=p16fifo[0];//�жϽ���ָ��
			p1=p16fifo[1];//��ǰ�Ѷ�ָ��
			if((p0>=(LEN_UARTnIntRx-6))||(p1>=(LEN_UARTnIntRx-6)))
			{
				p16fifo[0]=0;//�жϽ���ָ��
				p16fifo[1]=0;//��ǰ�Ѷ�ָ��
				p16fifo[2]=0;//��Լ���ָ��
				return;
			}
			TxByte=UARTCtrl->TxByte;
			if(p0!=p1)
			{//�����ݽ���,���ý��ճ�ʱ��ʱ
				p16timer[0]=100/10;//�ֽڳ�ʱ��ʱ
			}
			while(p0!=p1)
			{
				if(TxByte>=LEN_UARTnTx)
				{
					TxByte=0;
				}
				i=p8fifo[p1];
				p1++;
				if(p1>=(LEN_UARTnIntRx-6))
				{
					p1=0;
				}
				p8txbuff[TxByte]=i;
				TxByte++;
				
				if(TxByte==1)
				{
					if(i!=0x68)
					{
						TxByte=0;
						continue;
					}
				}
				else
				{
					if(TxByte==6)
					{
						if(i!=0x68)
						{
							TxByte=0;
							continue;
						}
						else
						{//2�����ȱȽ�
							i=(p8txbuff[1]+(p8txbuff[2]<<8));
							if(i!=(p8txbuff[3]+(p8txbuff[4]<<8)))
							{
								TxByte=0;
								continue;
							}
							else
							{
								i>>=2;//ȥ��־λD1D0,ͬʱ�������ֻ��0-16383
								if(i>(LEN_UARTnTx-8))
								{
									TxByte=0;
									continue;
								}
								
							}
						}
					}
					else
					{
						if(TxByte>6)
						{
							if(TxByte>=(((p8txbuff[1]+(p8txbuff[2]<<8))>>2)+8))
							{
								if(i==0x16)
								{//�������
									//��У�����
									cs=0;
									for(i=0;i<((p8txbuff[1]+(p8txbuff[2]<<8))>>2);i++)
									{
										cs+=p8txbuff[6+i];
									}
									cs&=0xff;
							
									if(cs!=p8txbuff[6+i])
									{//��У�����
										TxByte=0;
										continue;
									}
									//��У����ȷ
									p16fifo[1]=p1;//��ǰ�Ѷ�ָ��
									p16fifo[2]=p1;//���ָ��==��ָ��,���ʡʱ
									UARTCtrl->TxByte=TxByte;
									Terminal_Ram->CD485RxLED_MS_Timer=100/10;//����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
									UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrl->Task=2;//2=�ȴ���֡���ݷ�����
									return;
								}
								else
								{//��β��
									TxByte=0;
									continue;
								}
							}
						
						}
					}
				}
			}
			p16fifo[1]=p1;
			UARTCtrl->TxByte=TxByte;
			if(p16timer[0]==0x0)
			{//���ճ�ʱ
				i=UARTCtrl->CascadingReTxCount;//�����ط�����
				if(i<MEMReadReg(ADDR_AFN04F37+4,1))//������������վ������ʧ���ط����� BIN �� 1 
				{
					i++;
					UARTCtrl->CascadingReTxCount=i;//�����ط�����
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);//�û�����������
					i+=8;
					UARTCtrlRS485->TxByte=i;//�跢���ֽ���
					UARTCtrl->Task=21;//21=����ת����������
				}
				else
				{
					UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					UARTCtrl->Task=0;
				}
			}
			break;
	}
*/
}

const u8 Cascading_LoopChack_Example[]=
{
	0x68,
	12,0,
	12,0,
	0x68,
	0x4B,//������(DIR=0����,������־λPRM=1,������11=����2������
	0,0,//����������
	0,0,//�ն˵�ַ
	0x2,//����վ��ַ=1
	8,//AFN=8
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON��=0
	0,0,//Pn=0
	0,0,//Fn=0
	//cs
	//0x16
};

void Cascading_LoopChack(u32 PORTn)//��������Ѳ��
{
/*
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	
	if((Comm_Ram->SelfTestCommand==0x55555555)||(Comm_Ram->SelfTestCommand==0xAAAAAAAA))
	{//�Բ�������
		return;
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
	{//����
		goto LoopChack;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
	{//����
		goto LoopChack;
	}
	return;
	
LoopChack:
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->Task)//��������Ѳ������:0=��,1=��������,2=���ڷ���,3=���շ���,100=�ȴ��ϴ�
	{
		case 0://��
			if(Terminal_Ram->Cascading_LoopChack_S_Timer==0x0)//37 ����Ѳ�������붨ʱ��
			{//����Ѳ�������붨ʱ��=0
				i=MEMReadReg(ADDR_AFN04F37+6,1);
				i&=0x3;//�������ն˸���
				if(Terminal_Ram->Cascading_LoopChack_Count>=i)
				{//Ѳ���ն˼���>=�������ն˸���
					Terminal_Ram->Cascading_LoopChack_Count=0;
					Terminal_Ram->Cascading_LoopChack_S_Timer=MEMReadReg(ADDR_AFN04F37+5,1)*60;//��Ѳ�����ڶ�ʱ
				}
				else
				{
					UARTCtrl->Task++;
				}
			}
			break;
		case 1://������������
			Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MEMRead((u32)p8txbuff,(u32)Cascading_LoopChack_Example,20);
			MEMRead(((u32)p8txbuff)+7,ADDR_AFN04F37+7+(4*Terminal_Ram->Cascading_LoopChack_Count),4);
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			UARTCtrl->TxByte=20;//�跢���ֽ���
			UARTCtrl->BpsCtrl=MEMReadReg(ADDR_AFN04F37+1,1);
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task++;
			break;
		case 2://���ڷ�������
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
//					i=MEMReadReg(ADDR_AFN04F37+2,1);//���յȴ����ĳ�ʱʱ�� BIN 100ms 1 
//					if(i==0x0)
//					{
//						i=1;
//					}
//					i*=100/10;
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=2000/10;//���ճ�ʱʱ��
				UARTCtrl->RxByte=0;//�����ֽ���
				UARTCtrl->Task++;//���շ�������
			}
			break;
		case 3://���շ�������
			i=Uart_3761_Rx(PORTn);//UART_3761����;����0û���,1���,2��ʱ
			switch(i)
			{
				case 0://û���
					break;
				case 1://���
					Terminal_Ram->CD485RxLED_MS_Timer=100/10;//����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
					p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
					if((p8rxbuff[12])!=0x0)//Ӧ�ò㹦����AFN
					{//Ѳ�ⲻ�Ƿ���
						UARTCtrl->Task=100;//100=�ȴ������ϴ�
					}
					else
					{
						Terminal_Ram->Cascading_LoopChack_Count++;//��������Ѳ���ն˼���+1
						UARTCtrl->Task=0;
					}
					break;
				default://��ʱ
					Terminal_Ram->Cascading_LoopChack_Count++;//��������Ѳ���ն˼���+1
					UARTCtrl->Task=0;
					break;
			}
			break;
		case 100://�ȴ������ϴ�
			break;
		default:
			UARTCtrl->Task=0;
			break;
	}
}

void AutoCascadingData(u32 PORTn)//�Զ��ϴ�����Ѳ�⵽������
{
	u32 i;
	u32 RS485PORT;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	i=MEMReadReg(ADDR_AFN04F37,1);//�ն˼���ͨ�Ų���
	if(i!=0x0)
	{//�м�����
		if((MEMReadReg(ADDR_AFN04F37+6,1)&0x80)==0x0)
		{//������������
			if(UARTCtrl->Task==0x0)
			{//����
				if(p16timer[0]==0x0)
				{
					if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
					{//����
						switch(i)
						{
							case 2:
								RS485PORT=RS485_1PORT;
								break;
							case 3:
								RS485PORT=RS485_2PORT;
								break;
							case 4:
								RS485PORT=RS485_3PORT;
								break;
							default:
								return;
						}
						UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485PORT);
						if(UARTCtrlRS485->Task==100)
						{//100=�ȴ������ϴ�
							UARTCtrlRS485->Task=0;//�ϴ�����0
							i=UARTCtrlRS485->RxByte;//RX����(�ѽ����ֽ���)
							MEMRead(Get_ADDR_UARTnTx(PORTn),Get_ADDR_UARTnRx(RS485PORT),i);
							UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
							UARTCtrl->TxByte=i;//�跢���ֽ���
						}
					}
				}
			}
		}
	}
*/
}




