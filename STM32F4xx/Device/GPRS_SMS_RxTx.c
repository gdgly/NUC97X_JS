
#if MainProtocol==376
#include "../Head/Project.h"
#include "../Terminal/Terminal_Uart_3761.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#endif
#include "GPRS_SMS_RxTx.h"
#include "../Device/GPRS_3763.h"
#include "../Device/ATNo.h"
#include "..//Terminal/ATString.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "../STM32F4xx/STM32F4xx_SoftDelay.h"



u32 LEN_TelephoneNumber(u32 Addr)//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
{
	u32 i;
	u32 x;
	u8 * p8;
	
	MR(ADDR_DATABUFF,Addr,8);
	i=8;//8�ֽ�����
	x=0;//����λ��=0
	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0x86)
	{
		i--;
		p8++;
	}
	for(;i!=0x0;i--)
	{
		if((p8[0]>>4)==0xf)
		{//����δ
			break;
		}
		x++;
		if((p8[0]&0xf)==0xf)
		{//����δ
			break;
		}
		x++;
		p8++;
	}
	return x;
}

u8 * SMS_BCD_ASCII(u32 BCD,u8 *p8)//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ;�����¸�����ָ��p8
{
	u32 i;
	i=BCD>>4;
	if(i<10)
	{
		i+=0x30;
	}
	else
	{
		i+=55;
	}
	p8[0]=i;
	i=BCD&0xf;
	if(i<10)
	{
		i+=0x30;
	}
	else
	{
		i+=55;
	}
	p8[1]=i;
	p8+=2;
	return(p8);
}

u32 SMS_ASCII_BCD(u32 Addr,u32 BCDByte)//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8s;
	u8 *p8d;
	p8s=(u8*)Addr;
	p8d=p8s;
	y=0;
	for(i=0;i<BCDByte;i++)
	{
		y<<=8;
		x=p8s[0];
		if((x>=0x30)&&(x<=0x39))
		{
			x-=0x30;
		}
		else
		{
			x-=(0x41-10);
		}
		y|=x<<4;
		x=p8s[1];
		if((x>=0x30)&&(x<=0x39))
		{
			x-=0x30;
		}
		else
		{
			x-=(0x41-10);
		}
		y|=x;
		p8d[i]=y;
		p8s+=2;
	}
	if(y>=0x100)
	{
		y=0;
	}
	return y;
}


u32 GPRS_SMS_Tx(void)//GPRS_SMS_Tx����;���ط����ֽ���
{
	u32 i;
	u32 x;
	u32 y;
	u32 TxByte;
	u8 *p8txbuff;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrlSMS;

	p8txbuff=(u8*)(ADDR_ATTx);
	UARTCtrlSMS=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(SMSPORT);
	TxByte=0;
	switch(UART1Ctrl->SubTask)//������
	{
		default:
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask++;//������
			break;
	//����
		case 0://��SMS��AT����AT+CMGS(PDU��ʽ)
			if(UART1Ctrl->Task!=0)
			{
				GPRS_3763_NextATCount();//��1��AT
				return 0;
			}
//���½�����ʱ����������
//			UARTCtrlSMS->Task=2;
//			UARTCtrlSMS->TxByte=2;
			if((UARTCtrlSMS->Task==2)||(UARTCtrlSMS->Task==5))
			{//2=�ȴ���֡���ݷ���,5=�ȴ���֡���ݷ���
				UARTCtrlSMS->Task++;
				UARTCtrlSMS->TxCount=0;//�ѷ��ͼ���(���ֶ���)
			}
			if((UARTCtrlSMS->Task==3)||(UARTCtrlSMS->Task==6))
			{//2=��֡�������ڷ���,5=��֡�������ڷ���
				x=UARTCtrlSMS->TxByte;//�����ֽ���(���ֶ���)
				if(x==0)
				{//����
					if(UARTCtrlSMS->Task==3)
					{//��֡������
						UARTCtrlSMS->Task=0;//0=���ڽ���
					}
					else
					{//��֡������
						UARTCtrlSMS->Task=4;//4=��֡�������������
						//SMS��֡����+1
						UARTCtrlSMS->SMSTxSEQ++;
						UARTCtrlSMS->SMSTxSEQ&=0xfff;
					#if MainProtocol==376
						Uart_3761_RxTx(SMSPORT);//UART_3761���շ���
					#endif
					#if MainProtocol==698
						Uart_698_RxTx(SMSPORT);//UART_3761���շ���
					#endif
					}
				}
				else
				{
					TxByte=CopyString((u8 *)"AT+CMGS=",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				#if SMSEQ2Byte==1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
					x+=2;//����ͨ��2BYTE�ӿ�
				#endif
					
				#if MainProtocol==376	
					y=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
				#endif
				#if MainProtocol==698
					p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(p8)
					{
						y=p8[1];
						if(((p8[2]<<8)+p8[3])==(('8'<<8)+'6'))
						{
							if(y>=2)
							{
								y-=2;
							}
							else
							{
								y=0;
							}
						}
					}
					else
					{
						y=0;
					}
				#endif
					if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
					{//GSM����֧��GPRS
						if(x>140)
						{
							x=140;//���ŷ�������û����ݳ���
						}
						y++;//��������ż���ֽ���
						y>>=1;//ÿ2λ1�ֽ�
						y+=1;//���ʵ绰ͷ1�ֽ�
						x+=y;//
						x+=8;//��PDUͷ�ֽ���,���ǲ���SCA�������ĺ��볤������
					}
					else
					{//����CDMA MC8332
						if(x>140)
						{
							x=140;//���ŷ�������û����ݳ���
						}
						x+=y;
						x+=16;//��PDUͷ�ֽ���
					}
					y=hex_bcd(x);
					TxByte+=BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8txbuff[TxByte]='\r';
					TxByte++;
					return TxByte;
				}
			}
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask=10;//������
			break;
		case 1://��SMS������
			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			{//GSM����֧��GPRS
			//SMS-SUBMIT ����PDU��ʽ,ÿ1�ֽڵ�HEX��Ϊ����2�ֽ�ASCII��
			//1BYTE SMSC��ַ��Ϣ�ĳ���
			//1BYTE SMSC��ַ��ʽ(TON/NPI)
			//M_BYTE SMSC��ַ
				//PDU���ݳ��ȴ����¿�ʼ����,����SCA�������ĺ��벻������
			//1BYTE ��������(TP-MTI/VFP=11)
			//1BYTE ��Ϣ��׼ֵ(TP-MR)
			//1BYTE Ŀ�ĺ���λ��
			//1BYTE Ŀ�ĺ����ʽ(TON/NPI)
			//M_BYTE Ŀ�ĺ���
			//1BYTE Protocol identifier (TP-PID) Э��
			//1BYTE Data coding scheme (TP-DCS)���ݱ���
			//1BYTE Validity period��Чʱ��(TP-VP),0xa7=24Сʱ,0x00=5����
			//1BYTE User Data Length (TP-UDL)�û����ݳ���
			// <=140BYTE TP_UD ����

				//1 SCA�������ĺ���
				//1.1 SMSC��ַ��Ϣ�ĳ���(����1�ֽ�SMSC��ַ��ʽ,n�ֽ�SMSC��ַ)
			#if MainProtocol==376
				y=LEN_TelephoneNumber(ADDR_AFN04F4+8);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
				y++;//��������ż���ֽ���
				y>>=1;//ÿ2λ1�ֽ�
				if(y==0)
				{//û���ã��Զ��ÿ��ϵĶ������ĺ���
					p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				}
				else
				{//����
					x=y+2;//��ַ��ʽ1�ֽ�,���ʵ绰ͷ1�ֽ�
					x=hex_bcd(x);
					p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//1.2 SMSC��ַ��ʽ(TON/NPI=91)
					p8txbuff=SMS_BCD_ASCII(0x91,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//1.3 SMSC��ַ(����F���ճ�ż����)
						//������λ��
					p8txbuff=SMS_BCD_ASCII(0x86,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
					p8txbuff-=2;
					i=p8txbuff[0];
					p8txbuff[0]=p8txbuff[1];
					p8txbuff[1]=i;
					p8txbuff+=2;
						//����
					MR(ADDR_DATABUFF,ADDR_AFN04F4+8,8);
					p8=(u8 *)(ADDR_DATABUFF);
					if(p8[0]==0x86)
					{
						p8++;
					}
					for(i=0;i<y;i++)
					{
						p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
						p8txbuff-=2;
						x=p8txbuff[0];
						p8txbuff[0]=p8txbuff[1];
						p8txbuff[1]=x;
						p8txbuff+=2;
					}
				}
			#endif
			#if MainProtocol==698
				MR(ADDR_DATABUFF,ADDR_4500_4+3,17);//�������ĺ���
				p8=(u8 *)(ADDR_DATABUFF);
				y=p8[0];
				p8++;
				if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
				{
					if(y>=2)
					{
						p8+=2;
						y-=2;
					}
					else
					{
						y=0;
					}
				}
				y++;//��������ż���ֽ���
				y>>=1;//ÿ2λ1�ֽ�
				if(y==0)
				{//û���ã��Զ��ÿ��ϵĶ������ĺ���
					p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				}
				else
				{//����
					x=y+2;//��ַ��ʽ1�ֽ�,���ʵ绰ͷ1�ֽ�
					x=hex_bcd(x);
					p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//1.2 SMSC��ַ��ʽ(TON/NPI=91)
					p8txbuff=SMS_BCD_ASCII(0x91,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//1.3 SMSC��ַ(����F���ճ�ż����)
						//������λ��
					p8txbuff=SMS_BCD_ASCII(0x86,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
					p8txbuff-=2;
					i=p8txbuff[0];
					p8txbuff[0]=p8txbuff[1];
					p8txbuff[1]=i;
					p8txbuff+=2;
						//����
					y*=2;
					for(i=0;i<y;i++)
					{
						p8txbuff[0]=p8[0];
						p8txbuff++;
						p8++;
					}
				}
			#endif
				//2 ��������(TP-MTI/VFP=11)
				//TP-RP=0(B7),TP-UDHI=0(B6),TP-SRR=0(B5),TP-VPF=10(B4B3),TP-RD=0(B2),TP-MTI=01(B1B0)
				p8txbuff=SMS_BCD_ASCII(0x11,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//3 ��Ϣ��׼ֵ(TP-MR)
				//Message reference field (TP-MR),0-255���ż���,ͨ����Ϊ0
				p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//4 Ŀ�ĵ绰�����ַ
				//Destination Address (TP-da)
				//4.1 ����λ��(��?��ʮ������,������91�͡�F��)
			#if MainProtocol==376
				y=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
			#endif
			#if MainProtocol==698
				p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				if(p8)
				{
					y=p8[1];
					p8+=2;
					if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
					{
						if(y>=2)
						{
							p8+=2;
							y-=2;
						}
						else
						{
							y=0;
						}
					}
				}
				else
				{
					y=0;
				}
			#endif
					//��2λ������λ��
				p8txbuff=SMS_BCD_ASCII(y+2,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				y++;//��������ż���ֽ���
				y>>=1;//ÿ2λ1�ֽ�
				//4.2 Ŀ���ַ��ʽ(TON/NPI=91)
				p8txbuff=SMS_BCD_ASCII(0x91,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//4.3 Ŀ���ַ(TP-DA)(����F���ճ�ż����)
					//������λ��
				p8txbuff=SMS_BCD_ASCII(0x86,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				p8txbuff-=2;
				i=p8txbuff[0];
				p8txbuff[0]=p8txbuff[1];
				p8txbuff[1]=i;
				p8txbuff+=2;
					//����
		#if MainProtocol==376
				MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
				p8=(u8 *)(ADDR_DATABUFF);
				if(p8[0]==0x86)
				{
					p8++;
				}
				for(i=0;i<y;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
					p8txbuff-=2;
					x=p8txbuff[0];
					p8txbuff[0]=p8txbuff[1];
					p8txbuff[1]=x;
					p8txbuff+=2;
				}
		#endif
		#if MainProtocol==698
				y*=2;
				for(i=0;i<y;i++)
				{
					p8txbuff[0]=p8[0];
					p8txbuff++;
					p8++;
				}
		#endif
				//5 Protocol identifierЭ���ʶ(TP-PID),00=����ͨGSM����,�㵽�㷽ʽ
				p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//6 Data coding scheme (TP-DCS)���ݱ���;0X00GSM����,0X04=8bit���ݱ���,0X08=UCS2 16BIT(UNICODE),0XC0δ����
				p8txbuff=SMS_BCD_ASCII(0x04,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//7 Validity period��Чʱ��(TP-VP)
					//00 �C 8F (VF+1)*5 ���Ӵ� 5 ���Ӽ���� 12 ��Сʱ
					//90 �C A7 12 Сʱ + (VF �C 143)*30 ����
					//A8 �C C4 (VP �C 166) * 1 ��
					//C5 �C FF (VP �C 192) * 1 ��
				p8txbuff=SMS_BCD_ASCII(0x00,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//8 User data length�û���Ϣ����(TP-UDL)
				x=UARTCtrlSMS->TxByte;//�����ֽ���(���ֶ���)
			#if SMSEQ2Byte==1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				x+=2;//����ͨ��2BYTE�ӿ�
			#endif
				if(x>140)
				{
					x=140;//���ŷ�������û����ݳ���
				}
			#if SMSEQ2Byte==0//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				UARTCtrlSMS->TxByte-=x;//���跢���ֽ���
			#else
				UARTCtrlSMS->TxByte-=(x-2);//���跢���ֽ���
			#endif
				if(UARTCtrlSMS->TxByte==0x0)
				{//�����SMS��־
					UARTCtrlSMS->SMSTxSEQ|=0x8000;//���ŷ��ͽӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
				}
				//User data length (TP-UDL)
				p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				//User Data (TP-UD)�û�����
			#if SMSEQ2Byte==1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ>>8,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				x-=2;
			#endif
				p8=(u8 *)Get_ADDR_UARTnTx(SMSPORT);
				p8+=UARTCtrlSMS->TxCount;//�ѷ��ͼ���(���ֶ���)
				UARTCtrlSMS->TxCount+=x;//��������ָ��
				for(i=0;i<x;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				}
				p8txbuff[0]=26;//<ctrl-Z> (IRA 26)
				p8txbuff++;
				TxByte=((u32)p8txbuff)-(ADDR_ATTx);
				//֡��SMS����+1
				UARTCtrlSMS->SMSTxSEQ+=0x1000;
				UARTCtrlSMS->SMSTxSEQ&=0x7fff;
			}
			else
			{//����CDMA MC8332
			//<callerID_length>��1 byte����Χ0-20����������<caller_ID>
		#if MainProtocol==376
				y=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
				p8txbuff=SMS_BCD_ASCII(y,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Caller_ID>�� 0-20 byte����Χ +,*, #, 0-9,������Ϊ0ʱ�����Բ���д��
				MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
				p8=(u8 *)(ADDR_DATABUFF);
				i=0;
				if(p8[i]==0x86)
				{
					i++;
				}
				x=0;
				while(x<y)
				{
					p8txbuff=SMS_BCD_ASCII((p8[i]>>4)+0x30,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
					x++;
					if(x>=y)
					{
						break;
					}
					p8txbuff=SMS_BCD_ASCII((p8[i]&0xf)+0x30,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
					x++;
					i++;
				}
			//<Time>�� 6 byte����ΧYear�� 0-99��Month ��1-12��Day ��1-31��Hour��0-23�� Minute��0-59��Second��0-59��Day�ķ�Χ�ǲ��̶��ģ�Ҫ���������ȷ����
				//�ǿ���ʡ�Եģ�PDUģʽ�¿���ʹ��FFFFFFFFFFFF�滻��
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				for(i=0;i<6;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				}
		#endif
		#if MainProtocol==698
				p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
				if(p8)
				{
					y=p8[1];
					p8+=2;
					if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
					{
						if(y>=2)
						{
							p8+=2;
							y-=2;
						}
						else
						{
							y=0;
						}
					}
				}
				else
				{
					y=0;
				}
				p8txbuff=SMS_BCD_ASCII(y,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Caller_ID>�� 0-20 byte����Χ +,*, #, 0-9,������Ϊ0ʱ�����Բ���д��
				i=0;
				while(x<y)
				{
					p8txbuff[0]=p8[i];
					p8txbuff++;
					i++;
				}
			//<Time>�� 6 byte����ΧYear�� 0-99��Month ��1-12��Day ��1-31��Hour��0-23�� Minute��0-59��Second��0-59��Day�ķ�Χ�ǲ��̶��ģ�Ҫ���������ȷ����
				//�ǿ���ʡ�Եģ�PDUģʽ�¿���ʹ��FFFFFFFFFFFF�滻��
				MR(ADDR_DATABUFF,ADDR_DL698YMDHMS+1,6);
				for(i=0;i<6;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				}
		#endif
			//<Lang> ��1 byte����Χ0-7������ʡ�ԣ�ʡ��ʱ��FF�滻
				//0 UNSPECIFIED 
				//1 ENGLISH 
				//2 FRENCH 
				//3 SPANISH 
				//4 JAPANESE 
				//5 KOREAN 
				//6 CHINESE 
				//7 HEBREW
				p8txbuff=SMS_BCD_ASCII(1,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Format>��1 byte����Χ0, 1, 6
				//0 GSM 7-BIT 
				//1 ASCII 
				//6 UNICODE
				p8txbuff=SMS_BCD_ASCII(1,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Prt>��1 byte����Χ0-3,����ʡ�ԣ�ʡ��ʱ��FF�滻
				//0 NORMAL 
				//1 INTERACTIVE 
				//2 URGENT 
				//3 EMERGENCY
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Prv>��1 byte����Χ0-3,����ʡ�ԣ�ʡ��ʱ��FF�滻
				//0 NORMAL 
				//1 RESTRICTED 
				//2 CONFIDENTIAL 
				//3 SECRET
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Ack>��1 byte����Χ0-1
				//0 NEED ACK 
				//1 NO NEED ACK
				p8txbuff=SMS_BCD_ASCII(1,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Type>��1 byte����Χ0������ģʽ��ֻ��Ϊ0��������ֵ����֧�֡�
				//0 PDU
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<Stat>��1 byte ����Χ 0-3��(�洢�ͷ��Ͷ���ʱ����ֵֻ������Ϊ0��1)
				//0 MSG RECEIVED UNREAD 
				//1 MSG RECEIVED READ 
				//2 MSG STORED UNSEND 
				//3 MSG STORED SEND
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<udh_length>��1 byte ����Χ0, 6, 7����������<udh>
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<udh>�� 6-7 byte ������Ϊ0ʱ�����Բ���д��
				//���磺06050003120201 ����06�ǳ��ȣ�050003Ϊ�̶��ֶΣ�12�ֶ�Ϊindex���շ�˫��ͨ�����ֶα�ʶ��ͬ�ĳ����ţ�02Ϊ�����ŷְ�������01Ϊ��ǰ�������.

			//<msg_length> ��1 byte ����Χ0-160��<format>Ϊ0��1ʱ�����ֵ��160��Ϊ6ʱ�����ֵ��140
				x=UARTCtrlSMS->TxByte;//�����ֽ���(���ֶ���)
			#if SMSEQ2Byte==1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				x+=2;//����ͨ��2BYTE�ӿ�
			#endif
				if(x>140)
				{
					x=140;//���ŷ�������û����ݳ���
				}
			#if SMSEQ2Byte==0//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				UARTCtrlSMS->TxByte-=x;//���跢���ֽ���
			#else
				UARTCtrlSMS->TxByte-=(x-2);//���跢���ֽ���
			#endif
				if(UARTCtrlSMS->TxByte==0x0)
				{//�����SMS��־
					UARTCtrlSMS->SMSTxSEQ|=0x8000;//���ŷ��ͽӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
				}
				p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
			//<msg> 0-160 byte��������Ϊ0ʱ�����Բ���д��
			#if SMSEQ2Byte==1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ>>8,p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				x-=2;
			#endif
				p8=(u8 *)Get_ADDR_UARTnTx(SMSPORT);
				p8+=UARTCtrlSMS->TxCount;//�ѷ��ͼ���(���ֶ���)
				UARTCtrlSMS->TxCount+=x;//��������ָ��
				for(i=0;i<x;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//������1�ֽ�BCDתΪ2�ֽ�ASCII�벢����p8ָ��Ĵ�����Ԫ,�����¸�����ָ��p8
				}
				p8txbuff[0]=26;//<ctrl-Z> (IRA 26)
				p8txbuff++;
				TxByte=((u32)p8txbuff)-(ADDR_ATTx);
				//֡��SMS����+1
				UARTCtrlSMS->SMSTxSEQ+=0x1000;
				UARTCtrlSMS->SMSTxSEQ&=0x7fff;
			}
			break;
	//����
		case 10://������(�ѽ��յ�δ����Ϣ)
			if(UART1Ctrl->Task!=0)
			{
				GPRS_3763_NextATCount();//��1��AT
				return 0;
			}
			if(UARTCtrlSMS->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
			{
				UARTCtrlSMS->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
			#if SM_CMGF==0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
				TxByte=CopyString((u8 *)"AT+CMGL=4\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			#else
				TxByte=CopyString((u8 *)"AT+CMGL=\"ALL\"\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			#endif
			}
			else
			{
				GPRS_3763_NextATCount();//��1��AT
			}
			break;
	//ɾ��
		case 20://ȫ��ɾ���洢���е��Ѷ�����
		case 30://ȫ��ɾ���洢���е��Ѷ����ź�Ӳ����λ
		case 40://ȫ��ɾ���洢���е��Ѷ����ź��ģ��
		#if SM_CMGF==0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
			TxByte=CopyString((u8 *)"AT+CMGD=1,4\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
		#else
			TxByte=CopyString((u8 *)"AT+CMGD=1,4\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
		#endif
			break;
	}
	return TxByte;
}

void GPRS_SMS_Rx(void)//GPRS_SMS_Rx����
{
	u32 i;
	u32 x;
	u32 y;
	u32 Addr;
	u16 *p16fifo;
	u32 ReadFlag;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrlSMS;
//	u32 *p32;
#if SM_CMGF==0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
	u32 LEN_UARTnIntRx;
	u8 *p8d;
	u16 *p16;
#endif
#if (USER/100)==6//�û���ʶ:����
	u32 z;
#endif
	
	UARTCtrlSMS=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(SMSPORT);
	p16fifo=(u16*)(ADDR_UART1IntATRx);
	switch(UART1Ctrl->SubTask)//������
	{
		default:
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask=0;//������
			break;
	//����
		case 0://��SMS��AT����AT+CMGS(PDU��ʽ)
			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			{//GSM����֧��GPRS
				Addr=ATRxSearch((u32)((u8*)">"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{
					if(Terminal_Ram->AT_MS_Timer!=0)
					{//����û���
						return;
					}
				}
			}
			else
			{//����CDMA MC8332
			}
			UART1Ctrl->SubTask++;//������
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			return;
		case 1://��SMS������
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask=0;//������
			break;
	//����
		case 10://������(�ѽ��յ�δ����Ϣ)
		#if SM_CMGF==0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
			//PDU����+CMGL:<index>,<stat>,[<alpha>],<length><CR><LF><pdu><CR><LF>+CMGL:....
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			ReadFlag=0;//�ж���־
			while(1)
			{
				Addr=ATRxSearch((u32)((u8*)"+CMGL:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{//û�ҵ�SMSͷ'+CMGL:'
				ReadEnd:
					if(ReadFlag==0)//�ж���־
					{//û
						if(UARTCtrlSMS->Task==0)
						{//û�յ�����1֡
						#ifdef DL698
							Uart_698_RxTx(SMSPORT);//UART_3761���շ���
						#else
							Uart_3761_RxTx(SMSPORT);//UART_3761���շ���
						#endif
							if(UARTCtrlSMS->Task==1)
							{//�յ�����1֡
							#ifdef DL698
								Uart_698_RxTx(SMSPORT);//UART_3761���շ���
							#else
								Uart_3761_RxTx(SMSPORT);//UART_3761���շ���
							#endif
								UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
								UART1Ctrl->SubTask=0;//������
								return;
							}
						}
						GPRS_3763_NextATCount();//��1��AT
					}
					else
					{//��
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
						UART1Ctrl->SubTask=20;//ȫ��ɾ���洢���е��Ѷ�����
					}
					break;
				}
				//����ȷSMSͷ
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				ReadFlag=1;//�ж���־
				//��<pdu>��ַ,ǰ��<CR><LF>
				Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{//û�ҵ�<pdu>��ַ
					goto ReadEnd;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				
				if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				{//GSM����֧��GPRS
				//SMS-DELIVER ����PDU��ʽ,ÿ1�ֽڵ�HEX��Ϊ����2�ֽ�ASCII��
				//SCA
				//1BYTE SCA����
				//1BYTE SCA��������
				//N_BYTE SCA����
				//TPDU
				//1BYTE TP-MTI and more friends
				//1BYTE ����Դ����λ��
				//1BYTE ����Դ��������
				//M_BYTE ����Դ����
				//1BYTE Protocol identifier (TP-PID) Э��
				//1BYTE Data coding scheme (TP-DCS)���ݱ���
				//7BYTE The service centre time stamp (TP-SCTS)����ʱ��
				//������
				//�µ��¸�
				//�յ��ո�
				//ʱ��ʱ��
				//�ֵͷָ�
				//������
				//ʱ��
				//1BYTE User Data Length (TP-UDL)�û����ݳ���
				// <=140BYTE TP_UD ����
					//ȡSCA�ֽڳ���
					LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					i<<=1;//ÿ�ֽ�2��ASCII��
					Addr+=2+i;
					Addr+=2;//1BYTE TP-MTI and more friends
					//ȡ����Դ����λ��
					LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
					x=SMS_ASCII_BCD(ADDR_DATABUFF,1);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					Addr+=2+2;//1BYTE����λ��,1BYTE��������,ָ������ַ
					if((x<2)||(x>16))
					{//���볤�ȴ�,��С2λ(��ǰ11λ),���16λ
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						continue;
					}
					//����Ƚ�(��վ�ֻ�����)
			#if MainProtocol==376
					MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
					p8=(u8 *)(ADDR_DATABUFF);
					if(p8[0]==0x86)
					{
						MR(ADDR_DATABUFF,ADDR_AFN04F4+1,7);///������ͷ86���Ƚ�
					}
					//����BCD����תΪSMS��ʽ��ASCII��
					for(i=0;i<8;i++)
					{
						y=p8[i]&0xf;
						if(y<10)
						{
							p8[8+(i*2)]=y+0x30;
						}
						else
						{
							p8[8+(i*2)]=y+55;
						}
						y=(p8[i]>>4)&0xf;
						if(y<10)
						{
							p8[8+(i*2)+1]=y+0x30;
						}
						else
						{
							p8[8+(i*2)+1]=y+55;
						}
					}
			#else
				p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				if(p8)
				{
					i=p8[1];//�绰������Чλ��
					p8+=2;
					if(((p8[0]<<8)+p8[1])!=(('8'<<8)+'6'))
					{
						MR(ADDR_DATABUFF,(u32)p8,16);
					}
					else
					{
						MR(ADDR_DATABUFF,(u32)p8+2,16);
					}
				}
				else
				{
					MC(0,ADDR_DATABUFF,16);
				}
				p8=(u8 *)(ADDR_DATABUFF);
			#endif	
					LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					i=((i>>4)&0xf)+((i&0xf)<<4);//�ߵ�λ����
					if(i==0x86)
					{//������ͷ86���Ƚ�
						Addr+=2;
						x-=2;
					}
					LineATIntRx(Addr,ADDR_DATABUFF+8+16,16);//ȡAT�жϽ����������Է��ڻ�����
					if((x&1)&&(x!=0))
					{//����λ,���1λ,ȡ��1�ֽ�,��2�ֽ�ASCII���BCD��ϵ
						p8[8+(x-1)]=p8[8+x];
						p8[8+16+(x-1)]=p8[8+16+x];
					}
					for(i=0;i<x;i++)
					{
						if(p8[8+i]!=p8[8+16+i])
						{
							break;
						}
					}
					if(i!=x)
					{//���벻��ͬ
					#if MainProtocol==376
						i=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
						if(i!=0)
						{//��վ����������
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							continue;
						}
					#else
						p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
						if(p8)
						{
							i=p8[1];//�绰������Чλ��
							p8+=2;
							if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
							{
								if(i>2)
								{
									i-=2;
								}
								else
								{
									i=0;
								}
							}
						}
						else
						{
							i=0;
						}
						if(i!=0)
						{//��վ����������
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							continue;
						}
					#endif
					}
					x++;//��������ʱż���ֽ�
					x>>=1;
					x<<=1;//����λ��=ASCII�ֽ���
					Addr+=x;
					Addr+=(2*9);
					//ȡPDU����=i
					LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					Addr+=2;
					LineATIntRx(Addr,ADDR_DATABUFF,2*i);//ȡAT�жϽ����������Է��ڻ�����
					SMS_ASCII_BCD(ADDR_DATABUFF,i);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					p8=(u8 *)(ADDR_DATABUFF);
					p8[i]=0;//���ݽ�����0
					Addr+=(2*i);
				}
				else
				{//����CDMA MC8332
				//<callerID_length>��1 byte����Χ0-20����������<caller_ID>
					LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
					x=SMS_ASCII_BCD(ADDR_DATABUFF,1);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					Addr+=2+2;//����ASCII��2�ֽ�+'"'ASCII��2�ֽ�
				//<Caller_ID>�� 0-20 byte����Χ +,*, #, 0-9,������Ϊ0ʱ�����Բ���д��
			#if MainProtocol==376
					i=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
					if(i!=0)
					{
						//����Ƚ�(��վ�ֻ�����)
						MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
						p8=(u8 *)(ADDR_DATABUFF);
						//����BCD����תΪSMS��ʽ��ASCII��
						for(i=0;i<8;i++)
						{
							y=(p8[i]>>4)&0xf;
							if(y<10)
							{
								p8[8+(i*2)]=y+0x30;
							}
							else
							{
								p8[8+(i*2)]=y+55;
							}
							y=p8[i]&0xf;
							if(y<10)
							{
								p8[8+(i*2)+1]=y+0x30;
							}
							else
							{
								p8[8+(i*2)+1]=y+55;
							}
						}
						LineATIntRx(Addr,ADDR_DATABUFF+32,2*x);//ȡAT�жϽ����������Է��ڻ�����
						SMS_ASCII_BCD(ADDR_DATABUFF+32,x);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
						i=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
						i=WinDataComp(ADDR_DATABUFF+8,ADDR_DATABUFF+32,i,x);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
						if(i==0)
						{//��վ����������
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							continue;
						}
					}
				#else
					p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(p8)
					{
						i=p8[1];//�绰������Чλ��
						p8+=2;
						if(((p8[0]<<8)+p8[1])!=(('8'<<8)+'6'))
						{
							MR(ADDR_DATABUFF,(u32)p8,16);
						}
						else
						{
							MR(ADDR_DATABUFF,(u32)p8+2,16);
							if(i>2)
							{
								i-=2;
							}
							else
							{
								i=0;
							}
						}
						LineATIntRx(Addr,ADDR_DATABUFF+32,2*x);//ȡAT�жϽ����������Է��ڻ�����
						SMS_ASCII_BCD(ADDR_DATABUFF+32,x);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
						i=WinDataComp(ADDR_DATABUFF+8,ADDR_DATABUFF+32,i,x);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
						if(i==0)
						{//��վ����������
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							continue;
						}
					}
				#endif
					Addr+=(2*x)+2;//��ASCII��2*x�ֽ�+'"'ASCII��2�ֽ�
				//<Time>�� 6 byte����ΧYear�� 0-99��Month ��1-12��Day ��1-31��Hour��0-23�� Minute��0-59��Second��0-59��Day�ķ�Χ�ǲ��̶��ģ�Ҫ���������ȷ����
					//�ǿ���ʡ�Եģ�PDUģʽ�¿���ʹ��FFFFFFFFFFFF�滻��
				//<Lang> ��1 byte����Χ0-7������ʡ�ԣ�ʡ��ʱ��FF�滻
					//0 UNSPECIFIED 
					//1 ENGLISH 
					//2 FRENCH 
					//3 SPANISH 
					//4 JAPANESE 
					//5 KOREAN 
					//6 CHINESE 
					//7 HEBREW
				//<Format>��1 byte����Χ0, 1, 6
					//0 GSM 7-BIT 
					//1 ASCII 
					//6 UNICODE
				//<Prt>��1 byte����Χ0-3,����ʡ�ԣ�ʡ��ʱ��FF�滻
					//0 NORMAL 
					//1 INTERACTIVE 
					//2 URGENT 
					//3 EMERGENCY
				//<Prv>��1 byte����Χ0-3,����ʡ�ԣ�ʡ��ʱ��FF�滻
					//0 NORMAL 
					//1 RESTRICTED 
					//2 CONFIDENTIAL 
					//3 SECRET
				//<Ack>��1 byte����Χ0-1
					//0 NEED ACK 
					//1 NO NEED ACK
				//<Type>��1 byte����Χ0������ģʽ��ֻ��Ϊ0��������ֵ����֧�֡�
					//0 PDU
				//<Stat>��1 byte ����Χ 0-3��(�洢�ͷ��Ͷ���ʱ����ֵֻ������Ϊ0��1)
					//0 MSG RECEIVED UNREAD 
					//1 MSG RECEIVED READ 
					//2 MSG STORED UNSEND 
					//3 MSG STORED SEND
				//<udh_length>��1 byte ����Χ0, 6, 7����������<udh>
				//<udh>�� 6-7 byte ������Ϊ0ʱ�����Բ���д��
					//���磺06050003120201 ����06�ǳ��ȣ�050003Ϊ�̶��ֶΣ�12�ֶ�Ϊindex���շ�˫��ͨ�����ֶα�ʶ��ͬ�ĳ����ţ�02Ϊ�����ŷְ�������01Ϊ��ǰ�������.
					Addr+=(2*14);
				//<msg_length> ��1 byte ����Χ0-160��<format>Ϊ0��1ʱ�����ֵ��160��Ϊ6ʱ�����ֵ��140
					LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					Addr+=2;
					LineATIntRx(Addr,ADDR_DATABUFF,2*i);//ȡAT�жϽ����������Է��ڻ�����
					SMS_ASCII_BCD(ADDR_DATABUFF,i);//������2�ֽ�ASCIIתΪ1�ֽ�BCD��,�����ڴ�����Ԫ
					p8=(u8 *)(ADDR_DATABUFF);
					p8[i]=0;//���ݽ�����0
					Addr+=(2*i);
				}
				
			#if SMSEQ2Byte==1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
				if(i<=2)
				{//���Ŵ���ӿ���2�ֽ�
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					continue;
				}
				x=p8[0]+(p8[1]<<8);
				UARTCtrlSMS->SMSRxSEQ=x;
				UARTCtrlSMS->SMSTxSEQ=x&0xfff;
				MR(ADDR_DATABUFF,ADDR_DATABUFF+2,i);
				i-=2;
			#endif	
				p16=(u16*)Get_ADDR_UARTn(SMSPORT);//�õ�UART�����׵�ַ
				y=p16[0];
				p8d=(u8*)(Get_ADDR_UARTn(SMSPORT)+10);//�õ�UART�����׵�ַ
				LEN_UARTnIntRx=Get_LEN_UARTnIntRx(SMSPORT)-10;//�õ�UART�жϽ��ջ��峤��
				for(x=0;x<i;x++)
				{
					while(y>=LEN_UARTnIntRx)
					{
						y-=LEN_UARTnIntRx;
					}
					p8d[y]=p8[x];
					y++;
				}
				p16[0]=y;
				if(UARTCtrlSMS->Task==0)
				{//û�յ�����1֡
				#ifdef DL698
					Uart_698_RxTx(SMSPORT);//UART_3761���շ���
				#else
					Uart_3761_RxTx(SMSPORT);//UART_3761���շ���
				#endif
				}
				if(UARTCtrlSMS->Task==1)
				{//�յ�����1֡,������̶��������SEQ��д
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->SubTask=20;//ȫ��ɾ���洢���е��Ѷ�����
					break;
				}
			}
			if(UARTCtrlSMS->Task==1)
			{//�յ�����1֡
			#ifdef DL698
				Uart_698_RxTx(SMSPORT);//UART_3761���շ���
			#else
				Uart_3761_RxTx(SMSPORT);//UART_3761���շ���
			#endif
			}
		#endif//#if SM_CMGF==0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
		#if SM_CMGF==1//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
			//�ı�ģʽ����+CMGL :<index>,<stat>,<da/oa>,<lang>,<encod><CR><LF><data>(����/���͵Ķ���)
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			ReadFlag=0;//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
			while(1)
			{
				Addr=ATRxSearch((u32)((u8*)"+CMGL:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{//û�ҵ�SMSͷ'+CMGL:'
					if(ReadFlag==0)//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
					{//û
						GPRS_3763_NextATCount();//��1��AT
					}
					else
					{//��
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
						UART1Ctrl->SubTask=20;//ɾ������
						if(ReadFlag&4)//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
						{
							UART1Ctrl->SubTask=40;//ȫ��ɾ���洢���е��Ѷ����ź��ģ��
						}
						if(ReadFlag&2)//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
						{
							UART1Ctrl->SubTask=30;//������;ȫ��ɾ���洢���е��Ѷ����ź�Ӳ����λ
						}
					}
					break;
				}
				//����ȷSMSͷ
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				ReadFlag|=1;//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
				
				//��<da/oa>��ַ,ǰ��<index>,<stat>,<da/oa>
				Addr=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{//û�ҵ�
					continue;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{//û�ҵ�
					continue;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				
				x=LEN_TelephoneNumber(ADDR_AFN04F4);//�绰������Чλ��,��ڵ绰�Ŵ�����ַ
				if(x!=0)
				{
					//����Ƚ�(��վ�ֻ�����)
					MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
					p8=(u8 *)(ADDR_DATABUFF);
					//����BCD����תΪSMS��ʽ��ASCII��
					for(i=0;i<8;i++)
					{
						y=(p8[i]>>4)&0xf;
						if(y<10)
						{
							p8[8+(i*2)]=y+0x30;
						}
						else
						{
							p8[8+(i*2)]=y+55;
						}
						y=p8[i]&0xf;
						if(y<10)
						{
							p8[8+(i*2)+1]=y+0x30;
						}
						else
						{
							p8[8+(i*2)+1]=y+55;
						}
					}
					p8[8+x]=0;//�Ƚ��ַ�������
					x=ATRxSearch(ADDR_DATABUFF+8);//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(x==0)
					{//û�ҵ�
						continue;
					}
				}
				//��<Data>��ʼ��ַ,ǰ��<CR><LF>
				Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr==0)
				{//û�ҵ�
					continue;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				//��<Data>������ַ,����<CR><LF>
				i=ATRxSearch((u32)((u8*)"\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(i==0)
				{//û�ҵ�
					continue;
				}
				p16fifo[1]=i-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				while(i<Addr)
				{
					i+=LEN_UART1IntATRx-10;
				}
				i-=Addr;//�ı�����
				if(i<2)
				{
					continue;
				}
				i-=2;
				if(i>LEN_DATABUFF)
				{
					i=LEN_DATABUFF;
				}
				LineATIntRx(Addr,ADDR_DATABUFF,i);//ȡAT�жϽ����������Է��ڻ�����
				p8=(u8 *)(ADDR_DATABUFF);
				p8[i]=0;//�ı�������0
				
		#if (USER/100)==6//�û���ʶ:����
				//Զ��Ӳ����ʼ�����ܣ����ն��յ�ָ������������Զ�Ӳ����ʼ��
				x=WinDataComp((u32)((u8*)"cssdl 0"),ADDR_DATABUFF,7,i);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
				if(x!=0)
				{
					ReadFlag|=2;//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
				}
				//GPRSԶ����վ�л����ܣ����ն��յ�ָ����������󣬸���ԭ�ն˵�ͨѶ������������������վ�������վ�л��Ĺ���
				x=WinDataComp((u32)((u8*)"cssdl 1 "),ADDR_DATABUFF,8,i);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
				if(x!=0)
				{
					y=IP_ASCII_HEX(x+7);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
					if(y!=0)
					{
						MWR(y,ADDR_DATABUFF+2048,4);//дip
						y=i-((x-ADDR_DATABUFF)+8);//ʣ��û���ֽ���
						x=WinDataComp((u32)((u8*)":"),x+8+7,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
						if(x!=0)
						{
							y=ASCII_HEX(x+1);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
							MWR(y,ADDR_DATABUFF+2048+4,2);//д�˿�
							MW(ADDR_DATABUFF+2048,ADDR_DATABUFF+2048+6,6);//����
							y=i-((x-ADDR_DATABUFF)+1);//ʣ��û���ֽ���
							x=WinDataComp((u32)((u8*)":"),x+1,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
							if(x!=0)
							{
								y=i-((x-ADDR_DATABUFF));//ʣ��û���ֽ���,��1��������0
								//y=WinDataComp((u32)((u8*)"\x00"),x+1,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
								p8=(u8 *)(x+1);
								for(z=0;z<y;z++)
								{
									if(p8[z]<0x21)
									{//�ҵ�
										if(z<=16)
										{
											x++;
											MC(0,ADDR_DATABUFF+2048+12,16);//����16�ֽڲ�0
											MW(x,ADDR_DATABUFF+2048+12,z);//дAPN
											MW(ADDR_DATABUFF+2048,ADDR_AFN04F3,28);//д��AFN04F3����
											ReadFlag|=4;//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
											break;
										}
									}
								}
							}
						}
					}
				}
				//CDMAԶ����վ�л����ܣ����ն��յ�ָ����������󣬸���ԭ�ն˵�CDMAͨѶ������������������վ
				x=WinDataComp((u32)((u8*)"cssdl 2 "),ADDR_DATABUFF,8,i);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
				if(x!=0)
				{
					y=IP_ASCII_HEX(x+7);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
					if(y!=0)
					{
						MWR(y,ADDR_DATABUFF+2048,4);//дip
						y=i-((x-ADDR_DATABUFF)+8);//ʣ��û���ֽ���
						x=WinDataComp((u32)((u8*)":"),x+8+7,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
						if(x!=0)
						{
							y=ASCII_HEX(x+1);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
							MWR(y,ADDR_DATABUFF+2048+4,2);//д�˿�
							MW(ADDR_DATABUFF+2048,ADDR_DATABUFF+2048+6,6);//����
							y=i-((x-ADDR_DATABUFF)+1);//ʣ��û���ֽ���
							x=WinDataComp((u32)((u8*)":"),x+1,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
							if(x!=0)
							{
								y=i-((x-ADDR_DATABUFF)+1);//ʣ��û���ֽ���
								y=WinDataComp((u32)((u8*)":"),x+1,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
								if(y!=0)
								{
									x++;
									MC(0,ADDR_DATABUFF+2048+32,32);//�����ֽڲ�0
									MW(x,ADDR_DATABUFF+2048+32,y-x);
									x=y;
									y=i-((x-ADDR_DATABUFF));//ʣ��û���ֽ���,��1��������0
									//y=WinDataComp((u32)((u8*)"\x00"),x+1,1,y);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
									p8=(u8 *)(x+1);
									for(z=0;z<y;z++)
									{
										if(p8[z]<0x30)
										{//�ҵ�
											if(z<=32)
											{
												x++;
												MC(0,ADDR_DATABUFF+2048+64,32);//�����ֽڲ�0
												MW(x,ADDR_DATABUFF+2048+64,z);
												MW(ADDR_DATABUFF+2048,ADDR_AFN04F3,12);//д��AFN04F3����
												MW(ADDR_DATABUFF+2048+32,ADDR_AFN04F16,64);//д��AFN04F16����
			
												ReadFlag|=4;//�ж���־:D0λ=�ж���־,D1λ=��λ,D2λ=��ģ��
												break;
											}
										}
									}
								}
							}
						}
					}
				}
		#endif//#if (USER/100)==6//�û���ʶ:����
		
			}
		#endif//#if SM_CMGF==1//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
			break;
	//ɾ��
		case 20://ȫ��ɾ���洢���е��Ѷ�����
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			if(UARTCtrlSMS->Task!=0)
			{//�յ�����1֡
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
			}
			else
			{
				GPRS_3763_NextATCount();//��1��AT
			}
			break;
		case 30://ȫ��ɾ���洢���е��Ѷ����ź�Ӳ����λ
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����

//			p32=(u32*)(ADDR_HourCongeal_AutoRW);
//			while(p32[3]!=0)
//			{//R2����,ֵ=0��ʾ�������
//				HourCongeal_AutoRW();//Сʱ�����Զ�IO��д����0,��ɺ�������
//				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
//			}
//			p32=(u32*)(ADDR_Class2Congeal_AutoRW);
//			while(p32[3]!=0)
//			{//R2����,ֵ=0��ʾ�������
//				Class2Congeal_AutoRW();//��2�����Զ�IO��д����0,��ɺ�������
//				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
//			}

			SoftDelayMS(2000);//���ms��ʱ
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
			return;
		case 40://ȫ��ɾ���洢���е��Ѷ����ź��ģ��
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
			break;
	}
	Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
	if(Addr!=0)
	{
		p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
	}
}


	
