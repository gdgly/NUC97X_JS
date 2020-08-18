
#include "Project.h"
#include "../Calculate/Calculate.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "RouterBroadcastTime_Collector.h"

#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#if ((USER/100)!=7)&&((USER/100)!=8)//�����㽭��ͨ�ð�
void RouterBroadcastTime_Collector(u32 PORTn)//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
{
}
#else
void RouterBroadcastTime_Collector(u32 PORTn)//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 d64;
	u8 *p8rxbuff;
	u8 *p8txbuff;
	u32 BroadcastTime;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
		return;
	}
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	BroadcastTime=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);
	switch(BroadcastTime)//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
	{
		default:
			break;
		case 3://3=�����
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Terminal_Router->StopFlags|=(1<<4);//��ͣ·��
				Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
				BroadcastTime++;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				WarningOccur(PORTn,(u8 *)"�ز�׼��Уʱ");//�澯����,��ڸ澯����͸澯�ַ���
				MWR(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			}
			break;
		case 4://�ȴ�1��
			p16timer[0]=60000/10;
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			BroadcastTime++;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 5://ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
			if(p16timer[0]==0)
			{
				Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
				i=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+2,2);//����������� BIN 2
				if(i!=0)
				{
					x=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+4,1);
					x&=0x1f;//�˿ں�
					if(x==31)
					{
						i=MRR(ADDR_AFN0CF250_Ext1+10*(i-1)+5,2);
						if((i>>15)!=0)
						{//��
							i=~i;
							i++;
						}
						if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
						{//�������ֵ��Χ���������Уʱ
							BroadcastTime=10;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							break;
						}
						p8txbuff[4]=0x04;
						Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
						MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
						MR(((u32)p8txbuff)+16,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
						p8txbuff+=128;
						p8txbuff[0]=0;//ͨ��Э�����ͣ�00HΪ͸�����䣻01HΪ DL/T 645-1997��02HΪ DL/T 645-2007��03H��FFH����
						p8txbuff[1]=1;//										
						p8txbuff[2]=0;//										
			
						x=MRR(ADDR_AFN04F298_Ext,1);
						if(x==2)
						{//ָ���ɼ�����ַ�Ĺ㲥��ʱ����
							p8txbuff[3]=(22+14);//���ĳ���
							//����ͷ8�ֽڵ�ַ��Ϊ��Ӧ�ı��ַ
							p8txbuff[4]=0x68;
							MR(((u32)p8txbuff)+5,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
							p8txbuff[11]=0x68;
							//������1�ֽ�
							p8txbuff[12]=0x33;
							//1�ֽ�L=2+645���ĳ���
							p8txbuff[13]=24;
							//������2�ֽ�
							p8txbuff[14]=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+4,1);
							p8txbuff[14]>>=5;
							if(p8txbuff[14]==0)
							{//Ĭ��
								if(MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+5,1)==1)
								{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007
									//97Ĭ��1200������
									p8txbuff[14]=2;
								}
								else
								{
									//07Ĭ��2400������
									p8txbuff[14]=3;
								}
							}
							p8txbuff[15]=0;
				
							//645����
							MC(0xfe,((u32)p8txbuff)+2+14,4);
							p8txbuff[6+14]=0x68;
							MC(0x99,((u32)p8txbuff)+7+14,6);
							p8txbuff[13+14]=0x68;
							p8txbuff[14+14]=0x08;
							p8txbuff[15+14]=0x06;
							MR(((u32)p8txbuff)+16+14,ADDR_TYMDHMS,6);
							for(i=0;i<6;i++)
							{
								p8txbuff[16+i+14]+=0x33;
							}
							x=0;
							for(i=0;i<16;i++)
							{
								x+=p8txbuff[6+i+14];
							}
							p8txbuff[22+14]=x;
							p8txbuff[23+14]=0x16;
				
							//CS
							x=0;
							for(i=0;i<34;i++)
							{
								x+=p8txbuff[4+i];
							}
							p8txbuff[38]=x;
							//16
							p8txbuff[39]=0x16;
				
							Uart_3762_TxData(PORTn,0x13,1,26+14);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
							CopyString((u8 *)"�ز�Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							for(i=0;i<6;i++)
							{
								x=p8txbuff[10-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
						}
						else
						{//ָ�����ַ�Ĺ㲥��ʱ����
							p8txbuff[3]=22;//���ĳ���
							//645����
							MC(0xfe,((u32)p8txbuff)+2+2,4);
							p8txbuff[6+2]=0x68;
							MR(((u32)p8txbuff)+7+2,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
							p8txbuff[13+2]=0x68;
							p8txbuff[14+2]=0x08;
							p8txbuff[15+2]=0x06;
							MR(((u32)p8txbuff)+16+2,ADDR_TYMDHMS,6);
							for(i=0;i<6;i++)
							{
								p8txbuff[16+i+2]+=0x33;
							}
							x=0;
							for(i=0;i<16;i++)
							{
								x+=p8txbuff[6+i+2];
							}
							p8txbuff[22+2]=x;
							p8txbuff[23+2]=0x16;
				
							Uart_3762_TxData(PORTn,0x13,1,26);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
							CopyString((u8 *)"�ز�Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							for(i=0;i<6;i++)
							{
								x=p8txbuff[14-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
						}
			
						Terminal_Router->ReRouterCtrl=0;//·���������ط�����
						Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//�ӽڵ������ʱʱ����
						BroadcastTime++;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
					}
					else
					{
						BroadcastTime=10;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
					}
				}
				else
				{
					BroadcastTime=10;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
			}
			break;
		case 6://���㲥Уʱ
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=MRR(((u32)p8rxbuff)+10,3);
			if((i!=0x000414)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
				{
					Terminal_Router->ReRouterCtrl++;
					BroadcastTime--;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{
					BroadcastTime=10;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				break;
			}
			i=MRR(((u32)p8rxbuff)+19,2);//�㲥ͨ���ӳ�ʱ����

			p8txbuff[4]=0x04;
			Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
			MR(((u32)p8txbuff)+16,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
			p8txbuff+=128;

			x=MRR(ADDR_AFN04F298_Ext,1);
			if(x==2)
			{//ָ���ɼ�����ַ�Ĺ㲥��ʱ����
				p8txbuff[0]=(22+14);//���ĳ���
				//����ͷ8�ֽڵ�ַ��Ϊ��Ӧ�ı��ַ
				p8txbuff[1]=0x68;
				MR(((u32)p8txbuff)+2,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
				p8txbuff[8]=0x68;
				//������1�ֽ�
				p8txbuff[9]=0x33;
				//1�ֽ�L=2+645���ĳ���
				p8txbuff[10]=24;
				//������2�ֽ�
				p8txbuff[11]=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+4,1);
				p8txbuff[11]>>=5;
				if(p8txbuff[11]==0)
				{//Ĭ��
					if(MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+5,1)==1)
					{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007
						//97Ĭ��1200������
						p8txbuff[11]=2;
					}
					else
					{
						//07Ĭ��2400������
						p8txbuff[11]=3;
					}
				}
				p8txbuff[12]=0;
	
				//645����
				MC(0xfe,((u32)p8txbuff)+13,4);
				p8txbuff[17]=0x68;
				MC(0x99,((u32)p8txbuff)+18,6);
				p8txbuff[24]=0x68;
				p8txbuff[25]=0x08;
				p8txbuff[26]=0x06;
				d64=MRR(ADDR_TYMDHMS,6);
				d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
				MWR(d64,((u32)p8txbuff)+27,6);
				for(i=0;i<6;i++)
				{
					p8txbuff[27+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8txbuff[17+i];
				}
				p8txbuff[33]=x;
				p8txbuff[34]=0x16;
	
				//CS
				x=0;
				for(i=0;i<34;i++)
				{
					x+=p8txbuff[1+i];
				}
				p8txbuff[35]=x;
				//16
				p8txbuff[36]=0x16;
	
				Uart_3762_TxData(PORTn,0x14,3,37);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			}
			else
			{//ָ�����ַ�Ĺ㲥��ʱ����
				p8txbuff[0]=22;//���ĳ���
				//645����
				MC(0xfe,((u32)p8txbuff)+1,4);
				p8txbuff[5]=0x68;
				MR(((u32)p8txbuff)+6,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
				p8txbuff[12]=0x68;
				p8txbuff[13]=0x08;
				p8txbuff[14]=0x06;
				d64=MRR(ADDR_TYMDHMS,6);
				d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
				MWR(d64,((u32)p8txbuff)+15,6);
				for(i=0;i<6;i++)
				{
					p8txbuff[15+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8txbuff[5+i];
				}
				p8txbuff[21]=x;
				p8txbuff[22]=0x16;
	
				Uart_3762_TxData(PORTn,0x14,3,23);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			}

			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//�ӽڵ������ʱʱ����
			BroadcastTime++;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 7://����ȷ��֡
			switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				case 1://1=���1֡����
					i=MRR(((u32)p8rxbuff)+22,3);
					if(i==0x000113)
					{
						p16timer[0]=0;
						BroadcastTime++;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
					}
					else
					{
						UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
						UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					}
					break;
				case 0://0=����
				case 2://2=���ճ�ʱ(�����)
					if(Terminal_Ram->RouterCheck_S_Timer==0)//�ӽڵ������ʱʱ����
					{
						p16timer[0]=0;
						BroadcastTime++;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
					}
					else
					{
						UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
						UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					}
					break;
			}
			break;
		case 8://�ȴ��㲥��ʱ
			if(p16timer[0]==0)
			{
				BroadcastTime=10;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
//		case 9:
//			break;
		case 10:
			Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			Pn++;
			MWR(Pn,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			if(Pn>MaxRS485AddCarrierPn)
			{
				BroadcastTime=11;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			else
			{
				Terminal_Router->ReRouterCtrl=0;//·���������ط�����
				BroadcastTime=5;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
		case 11:
			Terminal_Router->StopFlags&=~(1<<4);//�ָ�·��
			Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			BroadcastTime=0;//�ɼ�����չУʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
	}
	MWR(BroadcastTime,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);
}
#endif
#endif
