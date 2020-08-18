
//����ģ��ͨ��
#include "Project.h"
#include "Terminal_Contrl_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"



#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void Terminal_Contrl_Uart_RxTx(u32 PORTn)//����ģ��ͨ��RxTx
{
	u32 i;
	u32 RxByte;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	u32 p0;
	u32 p1;
	u8 *p8;
	u16 *p16;
//	u32 *p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
//��ʼ��
	switch(UARTCtrl->LinkTask)
	{
		case 0://�ϵ��λ
			UARTCtrl->Task=0;//����ģ����շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			i=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);//ʱ��
			Pin_Configure(PIN_CTR_EN,//�ؿ���ģ���������
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			if(i<0x0010)
			{//��С��0ʱ10��,�ص�Դ
				Pin_Configure(PIN_PLC_PWR,
										0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			}
			else
			{//���ص�Դ
				Pin_Configure(PIN_PLC_PWR,
										0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			}
			p16timer[0]=3000/10;//��λ��ʱ
			UARTCtrl->LinkTask=1;
			return;
		case 1://��λ���
			if(p16timer[0]==0)
			{
				Pin_OutH(PIN_PLC_PWR);//���������,�������ʹ����
				p16timer[0]=2000/10;//ģ�����г�ʼ����ʱ
				UARTCtrl->BpsCtrl=0xb+(7<<5);//19200,8e1
				Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
				UARTCtrl->LinkTask=2;
			}
			return;
		case 2://ģ�����г�ʼ����ʱ
			if(p16timer[0]==0)
			{
				UARTCtrl->LinkTask=3;
			}
			return;
	}
	
//���ͽ���
	switch(UARTCtrl->Task)//����ģ����շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		case 0://����
		case 1://���1֡����
		case 2://���ճ�ʱ(�����)
			break;
		case 3://��������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;//���ڷ��ͱ�־
			return;
		case 4://���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->RxByte=0;//rx����=0
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrl->Task=5;//0=���ڽ���
			}
			return;
		case 5://���ڽ���
			DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
			p16=(u16 *)Get_ADDR_UARTn(PORTn);
			p8=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
			LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
			LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
			p0 = p16[0];//�жϽ���ָ��
			p1 = p16[1];//��ǰ�Ѷ�ָ��
			if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
			{
				p16[0]=0;//�жϽ���ָ��
				p16[1]=0;//��ǰ�Ѷ�ָ��
				return;
			}
			if(p0!=p1)
			{//�����ݽ���,���ý��ճ�ʱ��ʱ
				//�������ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
				p16timer[0]=500/10;
			}
			RxByte=UARTCtrl->RxByte;
			while(p0 != p1)
			{//��������
				if(RxByte >= LEN_UARTnRx)
				{
					RxByte = 0;
				}
				i = p8[p1];
				p1 ++;
				if(p1 >= (LEN_UARTnIntRx-10))
				{
					p1 = 0;
				}
				p8rxbuff[RxByte] = i;
				RxByte ++;
				
				if(RxByte == 1)
				{
					if(i != 0x68)
					{
						RxByte = 0;
						continue;
					}
				}
				else
				{
					if(RxByte >= 7)
					{
						if(i == 0x16)
						{//�������
							//��У�����
							//i=RAMCRC16((u32)p8rxbuff,4);//RAM������CRC16����(Ӳ���㷨)
							i=RAMCRC16_Soft((u32)p8rxbuff,4);//RAM������CRC16����(����㷨)
							if(i != (p8rxbuff[4]+(p8rxbuff[5]<<8)))
							{//��У�����
								RxByte = 0;
								continue;
							}
							//��У����ȷ
							p16[1] = p1;//��ǰ�Ѷ�ָ��
							UARTCtrl->RxByte = RxByte;
							UARTCtrl->Task = 1;
							return;
						}
						else
						{//����������ȷ
							RxByte = 0;
							continue;
						}
					}
				}
			}
			p16[1] = p1;
			if(p16timer[0] == 0x0)
			{//��ʱ
				UARTCtrl->Task=2;
				break;
			}
			else
			{
				UARTCtrl->RxByte = RxByte;
				return;
			}
		default:
			UARTCtrl->Task=0;//����ģ����շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			return;
	}
	
//����ͨ������
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	switch(UARTCtrl->SubTask)
	{
		case 0://д��������
			if(p16timer[0]!=0x0)
			{
				break;//��ʱԼ500ms(�����ֽں��ֽڼ䳬ʱֵ),�ⲻ����Ϸ���
			}
			p8txbuff[1]=0xA3;//��������֡
			p8=(u8*)ADDR_ContrlUARTData;//����ģ�������������
			p8txbuff[2]=p8[0];
			p8txbuff[3]=p8[1];
		TX:
			p8txbuff[0]=0x68;
			//i=RAMCRC16((u32)p8txbuff,4);//RAM������CRC16����(Ӳ���㷨)
			i=RAMCRC16_Soft((u32)p8txbuff,4);//RAM������CRC16����(����㷨)
			p8txbuff[4]=i;
			p8txbuff[5]=i>>8;
			p8txbuff[6]=0x16;
			UARTCtrl->TxByte=7;//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;//����ģ����շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->SubTask++;
			break;
		case 1:
			if(UARTCtrl->Task==2)
			{//���ճ�ʱ(�����)
				WarningOccur(PORTn,(u8 *)"û��⵽����ģ��");//�澯����,��ڸ澯����͸澯�ַ���
				//����ģ��û�в���ʱ��F12��ʾ���������·����״̬,	��Ҫ�ĳɲ�����״̬
				p8=(u8*)ADDR_ContrlUARTData;//����ģ�������������
				p8[2]=0xff;//�ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
				UARTCtrl->LinkTask=0;//�ص�Դ
				UARTCtrl->SubTask=0;
				return;
			}
			WarningCancel(PORTn);//�澯ȡ��,��ڸ澯����
			UARTCtrl->SubTask++;
			break;
		case 2://����������
			p8txbuff[1]=0xA4;//����������
			p8txbuff[2]=0;
			p8txbuff[3]=0;
			goto TX;
		case 3:
			if(UARTCtrl->Task==1)
			{//��ȷ����
				p8=(u8*)ADDR_ContrlUARTData;//����ģ�������������
				if((p8[0]==p8rxbuff[2])&&(p8[1]==p8rxbuff[3]))
				{//д��������ȷ
					Pin_Configure(PIN_CTR_EN,//����ģ���������
											0|//B3-B0:���ù���ѡ��AF(0-15)
								 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
								 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
								 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
								 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
								(1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				}
			}
			UARTCtrl->SubTask++;
			break;
		case 4://���澯״̬
			p8txbuff[1]=0xA5;//��ȡ�澯״̬֡
			p8txbuff[2]=0;
			p8txbuff[3]=0;
			goto TX;
		case 5:
			if(UARTCtrl->Task==1)
			{//��ȷ����
				p8=(u8*)ADDR_ContrlUARTData;//����ģ�������������
				p8[2]=p8rxbuff[2];
				p8[3]=p8rxbuff[3];
			}
			UARTCtrl->SubTask++;
			break;
		case 6://������ģ��Ӳ��������Ϣ
			p8txbuff[1]=0x50;//��֡
			goto TX;
		case 7://
			if(UARTCtrl->Task==1)
			{//��ȷ����
				i=p8rxbuff[2]|p8rxbuff[3];
				if(i)
				{
					if((Comm_Ram->ErrorIC&0x80000000)==0)
					{
						Comm_Ram->ErrorIC|=0x80000000;//Ӳ�������־:(��λ��ʾ����)
						Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
					}
				}
				else
				{
					Comm_Ram->ErrorIC&=0x7fffffff;//Ӳ�������־:(��λ��ʾ����)
				}
			}
			UARTCtrl->SubTask++;
			break;
		case 8://�������
//			p32=(u32*)(ADDR_DATABUFF);
//  		MR(ADDR_DATABUFF,ADDR_Download_ENTRY+(4*64),8);//����ʼ������ַ
//			if(p32[0]==0x00002000)
//			{//��I0��ڵ�ַ
//				RxByte=p32[1]-p32[0];
//				if((p32[1]>p32[0])&&(RxByte<=0x1000))
//				{
//	        MR(ADDR_DATABUFF+8,ADDR_Download_ENTRY+(RxByte-8),8);//�洢����
//	        if(p32[3]==0xA55A5AA5)
//	        {
//						//���ļ�����
//						RxByte+=255;
//						RxByte/=256;//ÿ֡�̶���256BYTE
//						UARTCtrl->RxSEQ=RxByte;//���跢��֡��
//						UARTCtrl->TxSEQ=0;//�ѷ�֡��
//						UARTCtrl->SubTask++;
//						return;
//					}
//				}
//			}
			UARTCtrl->SubTask=0;
			break;
		case 9://�ļ�����֡
			p8txbuff[0]=0x68;
			p8txbuff[1]=0x51;//��������֡
			p8txbuff[2]=UARTCtrl->TxSEQ;//�ѷ�֡��
			p8txbuff[3]=0;
			MR(((u32)p8txbuff+4),ADDR_Download_ENTRY+(256*UARTCtrl->TxSEQ),256);
			//i=RAMCRC16((u32)p8txbuff,4+256);//RAM������CRC16����(Ӳ���㷨)
			i=RAMCRC16_Soft((u32)p8txbuff,4+256);//RAM������CRC16����(����㷨)
			p8txbuff[4+256]=i;
			p8txbuff[5+256]=i>>8;
			p8txbuff[6+256]=0x16;
			UARTCtrl->TxByte=7+256;//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;//����ģ����շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->SubTask++;
			break;
		case 10://�ļ����ͷ���֡
			if(UARTCtrl->Task==2)
			{//���ճ�ʱ(�����)
				UARTCtrl->SubTask=0;
				return;
			}
			if(UARTCtrl->Task==1)
			{//��ȷ����
				UARTCtrl->TxSEQ++;
				if(UARTCtrl->TxSEQ>=UARTCtrl->RxSEQ)
				{//����
					MC(0,ADDR_Download_ENTRY+(4*64),8);//�建����
					UARTCtrl->SubTask=0;
					return;
				}
				UARTCtrl->SubTask--;//����֡
			}
			break;
	}

}

#endif
