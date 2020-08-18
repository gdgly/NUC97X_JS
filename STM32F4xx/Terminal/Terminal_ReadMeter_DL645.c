

#include "Project.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"



void Uart_DL645_RxTx(u32 PORTn)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
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
			if(p16timer[0]!=0)
			{
				break;
			}
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				p16timer[0]=UARTCtrl->TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;
			}
			break;
		case 5://5=���ڽ���
			i=DL645_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
			switch(i)
			{
				case 0://0û���
					return;
				case 1://1���
					UARTCtrl->Task=1;
					break;
				default://2��ʱ
					UARTCtrl->Task=2;
					return;
			}
			p16timer[0]=50/10;//��֡����������ʱ50MS��
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			break;
	}
}

void DL645_68H_Add33H_CS_16H(u32 TxBuffAddr)//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
{
	u32 i;
	u32 x;
	u8 * p8;
	p8=(u8 *)(TxBuffAddr);
	//��ʼ��
	p8[0]=0x68;
	p8[7]=0x68;
	//��0x33
	x=p8[9];
	for(i=0;i<x;i++)
	{
		p8[10+i]+=0x33;
	}
	//��β0x16
	p8[10+x+1]=0x16;
	//cs
	x+=10;
	p8[x]=0;
	for(i=0;i<x;i++)
	{
		p8[x]+=p8[i];
	}
}


void Read_DL645_2007_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x11;
	p8[9]=0x4;
	MWR(DI,BuffAddr+10,4);
	//��0x33
	for(i=0;i<4;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[14]=0;
	for(i=0;i<14;i++)
	{
		p8[14]+=p8[i];
	}
	p8[15]=0x16;
}

void Read_DL645_2007_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr,u32 Frames)//DL645-2007����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ,����֡��
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x12;
	p8[9]=5;
	MWR(DI,BuffAddr+10,4);
	p8[14]=Frames;
	//��0x33
	for(i=0;i<5;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[15]=0;
	for(i=0;i<15;i++)
	{
		p8[15]+=p8[i];
	}
	p8[16]=0x16;
}


void ReadMeter_DL645_2007_TxCurveData(u32 BuffAddr,u32 DI,u64 CommAddr,u64 YMDHM)//DL645-2007��������������֡
{
	u32 i;
	u8 * p8txbuff;
	
	p8txbuff=(u8 *)BuffAddr;
	p8txbuff[0]=0x68;
	MWR(CommAddr,((u32)p8txbuff)+1,6);
	p8txbuff[7]=0x68;
	p8txbuff[8]=0x11;
	p8txbuff[9]=10;
	MWR(DI,((u32)p8txbuff)+10,4);
	MWR(1,((u32)p8txbuff)+10+4,1);//��1����
	MWR(YMDHM,((u32)p8txbuff)+10+4+1,5);
	//��0x33
	for(i=0;i<10;i++)
	{
		p8txbuff[10+i]+=0x33;
	}
	//CS����
	p8txbuff[20]=0;
	for(i=0;i<20;i++)
	{
		p8txbuff[20]+=p8txbuff[i];
	}
	p8txbuff[21]=0x16;
}

void Read_DL645_1997_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x01;
	p8[9]=0x2;
	MWR(DI,BuffAddr+10,2);
	//��0x33
	for(i=0;i<2;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[12]=0;
	for(i=0;i<12;i++)
	{
		p8[12]+=p8[i];
	}
	p8[13]=0x16;
}

void Read_DL645_1997_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-1997����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x02;
	p8[9]=0x2;
	MWR(DI,BuffAddr+10,2);
	//��0x33
	for(i=0;i<2;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[12]=0;
	for(i=0;i<12;i++)
	{
		p8[12]+=p8[i];
	}
	p8[13]=0x16;
}

u32 DL645_Rx(u32 PORTn)//DL645֡����;����0û���,1���,2��ʱ
{
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
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
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//�����ݽ������ó�ʱ��ʱ
		p16timer[0]=500/10;//�ֽڼ䳬ʱ��ʱ
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
			if(RxByte==8)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=12)
				{
					if(RxByte>=(p8rxbuff[9]+12))
					{
						if(i==0x16)
						{//�������
							//��У�����
							Pn=0;
							for(i=0;i<(p8rxbuff[9]+10);i++)
							{
								Pn+=p8rxbuff[i];
							}
							Pn&=0xff;
							if(Pn==p8rxbuff[i])
							{//��У����ȷ
								p16fifo[1]=p1;//��ǰ�Ѷ�ָ��
								p16fifo[2]=p1;//���ָ��==��ָ��,���ʡʱ
								UARTCtrl->RxByte=RxByte;
								return 1;
							}
							else
							{//��У�����
								RxByte=0;
								continue;
							}
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
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		return 2;//2=���ճ�ʱ
	}
	return 0;
}

u32 DL645_2007_RxData(u32 PORTn)//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//ͨ�ŵ�ַ�Ƚ�
	for(i=0;i<6;i++)
	{
		if(p8txbuff[4+1+i]==0xaa)
		{
			continue;
		}
		if(p8rxbuff[1+i]!=p8txbuff[4+1+i])
		{
			break;
		}
	}
	if(i!=6)
	{
		return 1;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
	}
	//������
	if((p8rxbuff[8]&0x40)!=0x0)
	{//��վ�쳣�ش�
		return 2;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
	}
	//DI�Ƚ�
	for(i=0;i<4;i++)
	{
		if(p8rxbuff[10+i]!=p8txbuff[4+10+i])
		{
			break;
		}
	}
	if(i!=4)
	{
		return 3;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
	}
	//��0x33
	for(i=0;i<p8rxbuff[9];i++)
	{
		p8rxbuff[10+i]-=0x33;
	}
	return 0;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
}

u32 DL645_1997_RxData(u32 PORTn)//DL645_1997֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//ͨ�ŵ�ַ�Ƚ�
	for(i=0;i<6;i++)
	{
		if(p8txbuff[4+1+i]!=0x99)
		{
			break;
		}
	}
	if(i!=6)
	{//��ַ999999999999���Ƚ�
		for(i=0;i<6;i++)
		{
			if(p8rxbuff[1+i]!=p8txbuff[4+1+i])
			{
				break;
			}
		}
		if(i!=6)
		{
			return 1;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
		}
	}
	//������
	if((p8rxbuff[8]&0x40)!=0x0)
	{//��վ�쳣�ش�
		return 2;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
	}
	//DI�Ƚ�
	for(i=0;i<2;i++)
	{
		if(p8rxbuff[10+i]!=p8txbuff[4+10+i])
		{
			break;
		}
	}
	if(i!=2)
	{
		return 3;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
	}
	//��0x33
	for(i=0;i<p8rxbuff[9];i++)
	{
		p8rxbuff[10+i]-=0x33;
	}
	return 0;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
}

void Terminal_BroadcastTime_DL645(u32 PORTn)//DL645�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u16 * p16timer;
#if ((USER/100)==5)//�Ϻ�
	u32 x;
	u64 d64;
#endif

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->BroadcastTime)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
	{
		default:
			UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 3:
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					i=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					i=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					i=3;//RS485PORT=3;
					break;
				default:
					i=0;//RS485PORT=0;
					break;
			}
			CopyString((u8 *)"��x ���ڹ㲥Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			MWR(i+0x30,ADDR_DATABUFF+2,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			p16timer[0]=1000/10;//��ʱ1������
			UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 4://1200����
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(2<<5)+0x0b;
		TX:
			MC(0xfe,((u32)p8txbuff),4);
			MC(0x99,((u32)p8txbuff)+5,6);
			p8txbuff+=4;
			p8txbuff[0]=0x68;
			p8txbuff[7]=0x68;
			p8txbuff[8]=0x08;
			p8txbuff[9]=6;
		#if ((USER/100)==5)//�Ϻ�
			i=MRR(ADDR_AFN05F201,1);
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					x=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					x=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					x=3;//RS485PORT=3;
					break;
			}
			if(i&(1<<(x-1)))
			{
				d64=MRR(ADDR_TYMDHMS,6);
				i=MRR(ADDR_AFN05F201+7,2);
				if(i&0x1000)
				{//��ֵ
					i&=0xfff;
					i=bcd_hex(i);
					d64=YMDHMS_SubS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
				}
				else
				{//��ֵ
					i=bcd_hex(i);
					d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
				}
				MWR(d64,((u32)p8txbuff)+10,6);
			}
			else
			{
				MR(((u32)p8txbuff)+10,ADDR_TYMDHMS,6);
			}
		#else
			MR(((u32)p8txbuff)+10,ADDR_TYMDHMS,6);
		#endif
			for(i=0;i<6;i++)
			{
				p8txbuff[10+i]+=0x33;
			}
			p8txbuff[16]=0;
			p8txbuff[17]=0x16;
			for(i=0;i<16;i++)
			{
				p8txbuff[16]+=p8txbuff[i];
			}
			UARTCtrl->TxByte=22;//�����ֽ���(���ֶ���)
			
			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;//���ڷ���
			
			UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			break;
		case 5:
			Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
		case 6://2400����
			UARTCtrl->BpsCtrl=(3<<5)+0x0b;
			goto TX;
		case 7:
	#if ((USER/100)==5)//�Ϻ�
			Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
		case 8:
			//�Ϻ����ܱ�����Ϊ4800
			//4800����
			UARTCtrl->BpsCtrl=(4<<5)+0x0b;
			goto TX;
		case 9:
	#endif
			Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			#if ((USER/100)==5)//�Ϻ�
				i=MRR(ADDR_AFN05F201,1);
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						x=1;//RS485PORT=1;
						break;
					case RS485_2PORT://RS485-2
						x=2;//RS485PORT=2;
						break;
					case RS485_3PORT://RS485-3
						x=3;//RS485PORT=3;
						break;
				}
				if(i&(1<<(x-1)))
				{
					i&=~(1<<(x-1));
					MWR(i,ADDR_AFN05F201,1);
				}
			#endif
			}
			break;
	}
}


