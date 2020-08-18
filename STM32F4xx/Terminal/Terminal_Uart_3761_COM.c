
//UART3761_COM
#include "Project.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"




u32 Uart_376_Tp(u32 Tpaddr)//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
{
	u32 i;
	u8 * p8s;
	u8 * p8d;
	p8s=(u8 *)(Tpaddr);
	p8d=(u8 *)(ADDR_DATABUFF);
	if(p8s[5]==0x0)
	{//�����ʹ�����ʱʱ��=0,�������ж�
		return(0);
	}
	for(i=0;i<12;i++)
	{
		p8d[i]=0x1;
	}
	for(i=0;i<4;i++)
	{
		p8d[i]=p8s[1+i];
	}
	p8s=(u8 *)(ADDR_TYMDHMS);
	for(i=0;i<4;i++)
	{
		p8d[6+i]=p8s[i];
	}
	i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
	if((i&0x80000000)!=0x0)
	{//��ֵΪ��
		return(0);
	}
	p8s=(u8 *)(Tpaddr);
	if(i>=(p8s[5]*60))
	{
		return(1);
	}
	return(0);
}

void CopyDI(u32 PORTn,u32 DIaddr)//COPY���ݱ�ʶ�����ƻ���׼���ֽ�;�����ݱ�ʶΪȫ����Ч��������Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	MR((u32)(&UARTCtrl->SourDI),DIaddr,4);
	MR((u32)(&UARTCtrl->DestDI),DIaddr,4);
	i=MRR(DIaddr,2);
	if(i==0x00ff)
	{//ȫ����Ч������,��Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
		MWR(1,(u32)(&UARTCtrl->DestDI),2);
	}
}

u32 DItoFnPn(u32 PORTn)//���ݱ�ʶ�ֽ�ΪFnPn
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8=(u8 *)(&UARTCtrl->SourDI);
	
	//Pn
	i=p8[0]+(p8[1]<<8);
	if(i!=0x00ff)
	{//����ȫ����Ч������
		if(p8[4+1]==0x0)
		{
			Pn=0;
		}
		else
		{
			if(p8[4]==0x0)
			{
				Pn=0;
			}
			else
			{
				Pn=BitLoca(p8[4]);//�����1����λΪ1��λ��(0-32)
				Pn+=((p8[4+1]-1)*8);
			}
		}
	}
	else
	{//ȫ����Ч������
		Pn=p8[4]+(p8[4+1]<<8);
	}
	//Fn
	if(p8[4+2]==0x0)
	{
		Fn=0;
	}
	else
	{
		Fn=BitLoca(p8[4+2]);//�����1����λΪ1��λ��(0-32)
		Fn+=(p8[4+3]*8);
	}
	//׼����1��Fn
	i=p8[4+2];
	i<<=1;
	i&=(~(1<<BitLoca(p8[4+2])));//�����1����λΪ1��λ��(0-32)
	i>>=1;
	p8[4+2]=i;
	if(i==0x0)
	{
		//Fn��С������
		p8[4+2]=p8[2];//�ָ�DT1
		//׼����1��Pn(DA1)
		i=p8[0]+(p8[1]<<8);
		if(i!=0x00ff)
		{//����ȫ����Ч������
			i=p8[4];
			i<<=1;
			i&=(~(1<<BitLoca(p8[4])));//�����1����λΪ1��λ��(0-32)
			i>>=1;
			p8[4]=i;
			if(i==0x0)
			{
				//DA1��С������(ȫ�������)
				MC(0,(u32)(&UARTCtrl->DestDI),4);
			}
		}
		else
		{//��ȫ����Ч������
			MWR(Pn+1,(u32)(&UARTCtrl->DestDI),2);
		}
	}
	return((Fn<<16)+Pn);
}

void FnPntoDI(u32 DIaddr,u32 FnPn)//FnPn���Ϊ���ݱ�ʶ
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(DIaddr);
	i=FnPn&0xffff;
	if(i!=0x0)
	{
		i-=1;
		p8[0]|=(1<<(i%8));
		p8[1]=((i/8)+1);
	}
	i=(FnPn>>16)&0xffff;
	if(i!=0x0)
	{
		i-=1;
		p8[2]|=(1<<(i%8));
		p8[3]=(i/8);
	}
}


void Uart_376_ACK(u32 PORTn,u32 ACK)//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,���ݵ�Ԫ��ʶ
{
	u8 * p8txbuff;
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8txbuff[12]=0;//AFN=0
	p8txbuff[14]=0;
	p8txbuff[15]=0;
	p8txbuff[16]=ACK;//Fn,Pn
	p8txbuff[17]=0;
	p8txbuff[1]=12;//���ݳ���
	p8txbuff[2]=0;
	p8txbuff[6]&=0x0f;
	p8txbuff[6]|=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
}

void Uart_376_AUX(u32 PORTn,u32 rxTpaddr)//������Ϣ(EC,TP),�ݵȴ���֡���ݷ���
{
	u32 i;
	u32 len;
	u8 * p8rxTp;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8rxTp=(u8 *)(rxTpaddr);

	len=p8txbuff[1]+(p8txbuff[2]<<8);
//EC
	if(PORTn==GPRSPORT)
	{
		if(MRR(ADDR_ADCFlags,1)!=0)//��Ҫ�¼��ȴ�����0=û,1=��
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//�û������ݳ���+2
			p8txbuff[6]|=0x20;//�޸Ŀ�����ACD=1
		}
	}
#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
	if(PORTn==ENETPORT)
	{
		if(MRR(ADDR_ADCFlags+1,1)!=0)//��Ҫ�¼��ȴ�����0=û,1=��
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//�û������ݳ���+2
			p8txbuff[6]|=0x20;//�޸Ŀ�����ACD=1
		}
	}
#else
	if(PORTn==ENETPORT)
	{
		if(MRR(ADDR_ADCFlags,1)!=0)//��Ҫ�¼��ȴ�����0=û,1=��
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//�û������ݳ���+2
			p8txbuff[6]|=0x20;//�޸Ŀ�����ACD=1
		}
	}
#endif
	if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
	{
		if(MRR(ADDR_ADCFlags+2,1)!=0)//��Ҫ�¼��ȴ�����0=û,1=��
		{
			MR(((u32)p8txbuff)+6+len,ADDR_AFN0CF7_FM,2);
			len+=2;//�û������ݳ���+2
			p8txbuff[6]|=0x20;//�޸Ŀ�����ACD=1
		}
	}
//Tp
	if((p8txbuff[13]&0x80)!=0x0)
	{
		for(i=0;i<6;i++)
		{
			p8txbuff[6+len+i]=p8rxTp[i];
		}
		len+=6;
	}

//����
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
//����
	len+=8;
	UARTCtrl->Task=2;//�ݵȴ���֡���ݷ���
	UARTCtrl->TxByte=len;//�跢���ֽ���
}

void Uart_376_AUXTp(u32 PORTn,u32 rxTpaddr)//������Ϣ(Tp),�ݵȴ���֡���ݷ���
{
	u32 i;
	u32 len;
	u8 * p8rxTp;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8rxTp=(u8 *)(rxTpaddr);
	
	len=p8txbuff[1]+(p8txbuff[2]<<8);
//Tp
	if((p8txbuff[13]&0x80)!=0x0)
	{
		for(i=0;i<6;i++)
		{
			p8txbuff[6+len+i]=p8rxTp[i];
		}
		len+=6;
	}

//����
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
//����
	len+=8;
	UARTCtrl->Task=2;//�ݵȴ���֡���ݷ���
	UARTCtrl->TxByte=len;//�跢���ֽ���
}



void Uart_376_NoAUX(u32 PORTn)//û������Ϣ,�ݵȴ���֡���ݷ���
{
	u32 len;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));

	len=p8txbuff[1]+(p8txbuff[2]<<8);
//SEQ

//����
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
//����
	len+=8;
	UARTCtrl->Task=2;//�ݵȴ���֡���ݷ���
	UARTCtrl->TxByte=len;//�跢���ֽ���
}

u32 Uart_376_CS(u32 buffaddr)//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
{
	u32 i;
	u32 len;
	u32 cs;
	u8 * p8;
	p8=(u8 *)(buffaddr);
	len=(p8[1]+(p8[2]<<8));
	cs=0;
	for(i=0;i<len;i++)
	{
		cs+=p8[6+i];
	}
	cs&=0xff;
	p8[6+i]=cs;
	p8[7+i]=0x16;
	len<<=2;
	len|=0x2;//2=����Լ��־,1=2005��Լ
	p8[1]=len;
	p8[2]=len>>8;
	p8[3]=len;
	p8[4]=len>>8;
	return(cs);
}

u32 Uart_3761_Rx(u32 PORTn)//UART_3761����;����0û���,1���,2��ʱ
{
	u32 i;
	u32 cs;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;

	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
	p0=p16fifo[0];//�жϽ���ָ��
	p1=p16fifo[1];//��ǰ�Ѷ�ָ��
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//�жϽ���ָ��
		p16fifo[1]=0;//��ǰ�Ѷ�ָ��
		p16fifo[2]=0;//��Լ���ָ��
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;//����0û���,1���,2��ʱ
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//�����ݽ���,���ý��ճ�ʱ��ʱ
		p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
		LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
		//�������ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
		if(PORTn==ENETPORT)
		{
			p16timer[0]=5000/10;//��̫��ģ�鳬ʱ��ʱ5S
		}
		else
		{
			if(PORTn==GPRSPORT)
			{
				p16timer[0]=10000/10;//����ͨ��ģ�鳬ʱ��ʱ10S
			}
			else
			{
				if(PORTn==SMSPORT)
				{
					p16timer[0]=60000/10;//����ͨ����ʱ��ʱ60S
				}
				else
				{
					p16timer[0]=1000/10;//����ͨ��1S
				}
			}
		}
	}
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
			if(RxByte==6)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
				else
				{//2�����ȱȽ�
					i=(p8rxbuff[1]+(p8rxbuff[2]<<8));
					if(i!=(p8rxbuff[3]+(p8rxbuff[4]<<8)))
					{
						RxByte=0;
						continue;
					}
					else
					{
						//ȥ��־λD1D0,ͬʱ�������ֻ��0-16383
						i>>=2;
						p8rxbuff[1]=i;
						p8rxbuff[2]=(i>>8);
						if(i>(LEN_UARTnRx-8))
						{
							RxByte=0;
							continue;
						}
					}
				}
			}
			else
			{
				if(RxByte>6)
				{
					if(RxByte>=(p8rxbuff[1]+(p8rxbuff[2]<<8)+8))
					{
						if(i==0x16)
						{//�������
							//��У�����
							cs=0;
							for(i=0;i<(p8rxbuff[1]+(p8rxbuff[2]<<8));i++)
							{
								cs+=p8rxbuff[6+i];
							}
							cs&=0xff;
					
							if(cs!=p8rxbuff[6+i])
							{//��У�����
								RxByte=0;
								continue;
							}
							//��У����ȷ
							p16fifo[1]=p1;//��ǰ�Ѷ�ָ��
							p16fifo[2]=p1;//���ָ��==��ָ��,���ʡʱ
							p16fifo[3]=p1;//���ָ��==��ָ��,���ʡʱ
							p16fifo[4]=p1;//���ָ��==��ָ��,���ʡʱ
							UARTCtrl->RxByte=RxByte;
							return 1;//����0û���,1���,2��ʱ
						}
						else
						{//��β��
							RxByte=0;
							continue;
						}
					}
				
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		UARTCtrl->RxByte=0;//�ѽ����ֽ���=0
		return 2;//����0û���,1���,2��ʱ
	}
	return 0;//����0û���,1���,2��ʱ
}

u32 CheckUartData_0xEE(u32 DataAddr,u32 Byte)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
{
	u32 i;
	u8 * p8;
	if(Byte>5)
	{
		Byte=5;
	}
	p8=(u8*)DataAddr;
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xEE)
		{
			return 0;
		}
	}
	return 1;
}





