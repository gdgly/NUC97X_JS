
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_LINK.h"
#include "../DL698/DL698_CONNECT.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_SET.h"
#include "../DL698/DL698_ACTION.h"
#include "../DL698/DL698_REPORT.h"
#include "../DL698/DL698_PROXY.h"
#include "../DL698/DL698_SECURITY.h"
#include "../DL698/DL698_REPORT.h"
#include "../MS/MS.h"
#include "../TEST/LOG.h"



__align(4) const u16 fcstab[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

u32 Uart_698_HCS(u8 *p8)//֡ͷУ�����;����0=��ȷ��1=����
{
	u32 i;
	u32 len;
	u32 HCS;
	
	p8++;
	len=p8[3]&0x0f;
	len+=6;
	HCS=0xffff;
	for(i=0;i<len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	p8[len] = HCS;//least significant byte first
	p8[len+1] = (HCS >> 8);
	if(i==HCS)
	{
		return 0;
	}
	return 1;
}
#include "../DL698/DL698_JSON.h"

u32 Uart_698_HCSFCS(u8 *p8)//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
{
	u32 i;
	u32 len;
	u32 FCS;
	u32 Err;
	
	Err=Uart_698_HCS(p8);//֡ͷУ�����;����0=��ȷ��1=����
	p8++;
	len=p8[0]+(p8[1]<<8);
	if(len<2)
	{
		return 1;
	}
	len-=2;
	FCS=0xffff;
	for(i=0;i<len;i++)
	{
		FCS=((FCS >> 8)&0xff) ^ fcstab[(FCS ^ p8[i]) & 0xff];
	}
	FCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	p8[len] = FCS;//least significant byte first
	p8[len+1] = (FCS >> 8);
	p8[len+2] = 0x16;
	if(i!=FCS)
	{
		Err=1;
	}
	
	return Err;
//#if DEBUG==0//0=����,1=����
//	return Err;
//#else
//	Err=Err;
//	return 0;
//#endif
}


u32 FrameProtocol_698_HCS(u8 *p8)//ͨ��Э���Զ�ʶ��֡ͷУ�����;����0=��ȷ��1=����
{
	u32 i;
	u32 len;
	u32 HCS;
	
	p8++;
	len=p8[3]&0x0f;
	len+=6;
	HCS=0xffff;
	for(i=0;i<len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	if(i==HCS)
	{
		return 0;
	}
	return 1;
}

u32 FrameProtocol_698_HCSFCS(u8 *p8)//ͨ��Э���Զ�ʶ����֡У�����(HCS��FCS);����0=��ȷ��1=����
{
	u32 i;
	u32 len;
	u32 FCS;
	u32 Err;
	
	Err=FrameProtocol_698_HCS(p8);//֡ͷУ�����;����0=��ȷ��1=����
	p8++;
	len=p8[0]+(p8[1]<<8);
	if(len<2)
	{
		return 1;
	}
	len-=2;
	FCS=0xffff;
	for(i=0;i<len;i++)
	{
		FCS=((FCS >> 8)&0xff) ^ fcstab[(FCS ^ p8[i]) & 0xff];
	}
	FCS ^= 0xffff; // complement
	i=p8[len];
	i|=p8[len+1]<<8;
	if(i!=FCS)
	{
		Err=1;
	}
	return Err;
}

u32 FrameProtocol(u8* p8,u32 Len)//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
{
	u32 i;
	u32 x;
	u32 cs;
	
//ȥ��ʼ��0xfe
	for(i=0;i<Len;i++)
	{
		if(p8[i]!=0xfe)
		{
			break;
		}
	}
	p8+=i;
	Len-=i;
	if(Len==0)
	{
		return 0;
	}
//DL698
	if(p8[0]==0x68)
	{
		i=p8[1]|(p8[2]<<8);
		if(p8[1+i]==0x16)
		{
			if((i+2)==Len)
			{
				i=FrameProtocol_698_HCSFCS(p8);//ͨ��Э���Զ�ʶ����֡У�����(HCS��FCS);����0=��ȷ��1=����
				if(i==0)
				{
					return 3;//3=DL698
				}
			}
		}
	}
//DL645
	if(p8[0]==0x68)
	{
		if(p8[7]==0x68)
		{
			if((p8[9]+12)==Len)
			{
				cs=0;
				for(i=0;i<(p8[9]+10);i++)
				{
					cs+=p8[i];
				}
				cs&=0xff;
				if(cs==p8[i])
				{//��У����ȷ
					if(p8[i+1]==0x16)
					{
						return 2;//1=DL/T645-1997,2=DL/T645-2007
					}
				}
			}
		}
	}
//DL376-2
	if(p8[0]==0x68)
	{
		x=(p8[1]+(p8[2]<<8));
		if(x==Len)
		{
			if(p8[x-1]==0x16)
			{
				if(x>=5)
				{
					x-=5;
					cs=0;
					for(i=0;i<x;i++)
					{
						cs+=p8[3+i];
					}
					cs&=0xff;
					if(cs==p8[3+i])
					{
						return 128;//128=DL376.2
					}
				}
			}
		}
	}

	return 0;//δ֪
}


u32 Uart_698_Rx(u32 PORTn)//UART_698����;����0û���,1���,2��ʱ
{
	u32 i;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rx;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;

	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u32 LEN_SA;//��������ַSA�ĳ���
	
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
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
		p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
			#if (USER/100)==0//���Ժ����
				p16timer[0]=5000/10;//����ͨ��ģ�鳬ʱ��ʱ5S
			#else
				p16timer[0]=10000/10;//����ͨ��ģ�鳬ʱ��ʱ10S
			#endif
			}
			else
			{
				if(PORTn==SMSPORT)
				{
					p16timer[0]=60000/10;//����ͨ����ʱ��ʱ60S
				}
				else
				{
					p16timer[0]=2000/10;//����ͨ��1S
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
		p8rx[RxByte]=i;
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
			LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
			LEN_SA&=0x0f;
			LEN_SA++;
			if(RxByte==(5+LEN_SA+3))
			{
				i=Uart_698_HCS(p8rx);//֡ͷУ�����;����0=��ȷ��1=����
				if(i)
				{//У�����
					RxByte-=1;
					while(1)
					{
						if(p1>=RxByte)
						{
							p1-=RxByte;
							break;
						}
						p1+=LEN_UARTnIntRx-10;
					}								
					RxByte=0;
					continue;
				}
			}
			if(RxByte>13)
			{
				if(RxByte>=(p8rx[1]+(p8rx[2]<<8)+2))
				{
					if(i==0x16)
					{//�������
						//У�����
						i=Uart_698_HCSFCS(p8rx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
						if(i)
						{//У�����
							RxByte-=1;
							while(1)
							{
								if(p1>=RxByte)
								{
									p1-=RxByte;
									break;
								}
								p1+=LEN_UARTnIntRx-10;
							}								
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
						RxByte-=1;
						while(1)
						{
							if(p1>=RxByte)
							{
								p1-=RxByte;
								break;
							}
							p1+=LEN_UARTnIntRx-10;
						}								
						RxByte=0;
						continue;
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


#include "../DL698/DL698_JSON.h"


void Uart_698_RxTx(u32 PORTn)//UART_698���շ���
{
	u32 i;
	u32 x;
	u16* p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	ms_Type* ms;
	u32 LEN_UARTnTx;
	u32 LEN_ADDR;
	u32 LEN_TAPDU;
	u32 LEN_TxAPDU;
	u32 LEN_Tx;
	u8* p8tx;
	
	u8 * p8rx;
	u8 * p8data;
	u32 y;

	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��

	if(p16timer[0]==0)
	{//�ͷ��ҵ��ڴ�ѷ����붨ʱ
		if(UARTCtrl->pTxData)
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
	}
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)
	{
		case 0://0=���ڽ���
			if(UARTCtrl->Lock==0x55)
			{
				return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			}
			//���ַ�͹㲥��ַʱ����ת������
/*
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
*/
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{
				if(UARTCtrl->ReNewLink==0x55)//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
				{
					UARTCtrl->ReNewLink=0;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
					UARTCtrl->LinkTask=0;
					p16timer[0]=0;//��ʱ�ػ�
					return;
				}
			}
			//����
			i=Uart_698_Rx(PORTn);//UART_698����;����0û���,1���,2��ʱ
			if(i==1)
			{
				UARTCtrl->Task=1;
			}
			if(UARTCtrl->RxByte==0)
			{
				EVENT_REPORT_Notification(PORTn);//�ϱ�֪ͨ(�¼�)
				REPORT_Notification(PORTn);//�ϱ�֪ͨ(��ʱ����)
			}
			break;
		case 1://1=�������
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			if(p8rx[3]&(1<<5))//��֡��־λ
			{
				if(p16timer[0]==0x0)
				{
					LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					p8data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
					LEN_ADDR=p8rx[4];//ȡ��ַ�ֽ���
					LEN_ADDR&=0x0f;
					LEN_ADDR++;
					LEN_ADDR+=2;
					LEN_TAPDU=11+LEN_ADDR;//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
					LEN_TAPDU-=3+LEN_ADDR+2+2;//������L�Ϳ�����C=3byte,֡ͷУ��=2byte,֡У��=2byte
					MR((u32)p8tx,(u32)p8rx,4+LEN_ADDR+2);
					p8tx[3]|=p8rx[3]|0x80;//��֡��־λ
					LEN_Tx=4+LEN_ADDR+2;

					x=p8rx[LEN_Tx];//��·���֡��ż���0-4095

					p8tx[LEN_Tx]=p8rx[LEN_Tx];
					p8tx[LEN_Tx+1]=(p8rx[LEN_Tx+1]&0x0f)|0x80;
					if((p8rx[LEN_Tx+1]&0xf0)==0x00)
					{
						p8data[6*1024]=0;
						p8data[6*1024+1]=0;
					}
					LEN_Tx+=2+1;
					UARTCtrl->TxByte=LEN_Tx+2;
					x=(p8rx[2]<<8|p8rx[1])-3-LEN_ADDR-4-2;
					y=(p8data[6*1024]<<8|p8data[6*1024+1]);
					MR((u32)p8data+2+y+6*1024,(u32)p8rx+16,x);
					p8data[6*1024+1]=(x+y)&0xff;
					p8data[6*1024]=((x+y)>>8)&0xff;
					
					if((p8rx[LEN_Tx-2]&0xf0)==0x40)
					{
						MR((u32)p8rx+4+LEN_ADDR+2,(u32)p8data+6*1024+2,x+y);
						LEN_Tx=4+LEN_ADDR+2+x+y+2-1;
						p8rx[3]=0x43;
						p8rx[1]=LEN_Tx;
						p8rx[2]=LEN_Tx>>8;
						Uart_698_HCSFCS(p8rx);
						UARTCtrl->Task=8;
						return;
					}
					p8tx[1]=LEN_Tx;
					p8tx[2]=LEN_Tx>>8;
					Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
					
				#if LOG_68==1//0=û,1=��
					LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
				#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
					LOGRS232(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
					if(PORTn!=GPRSPORT)
					{//GPRS���Լ�
						UARTCtrl->RxTxByte+=UARTCtrl->TxByte;
					}
					UARTCtrl->Task=2;//���ڷ��ͱ�־
				}
				else
				{
					if(p16timer[0]>(20/10))
					{//����ת��,����errʱû���ö�ʱֵ!
						p16timer[0]=20/10;
					}
				}
				break;
			}
			if(UARTCtrl->Lock==0x55)
			{
				return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			}
			UARTCtrl->Task=0;//0=����
			UARTCtrl->AddrType=0;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
//			UARTCtrl->TxFrameCount=0;//�Ĵ��֡����ʱ�ѷ���֡����
//			UARTCtrl->MulFrameGPCR=0;//��֡ʱͨ�ü����Ĵ���
//			UARTCtrl->LockFlags&=(~(7<<4));//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7
			UARTCtrl->SubTask=0;//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
			//����-������ʱ
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS�����ͨ��
				p16timer[0]=0;//��ʱ0MS����
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//��ʱxMS����(��֮֡�����·���м��������33λ)
			}
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{//ͨ��ģ��
				//��������ʱ(��ȷ�յ��κ�֡,��������ʱ)
				if(PORTn==GPRSPORT)
				{
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				}
				else
				{
					i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				}
				if(i==0)
				{
					i=120;
				}
				else
				{
					i=MRR(i+1,2);
					i=(i>>8)+((i&0xff)<<8);
				}
				
				if((UARTCtrl->TCPUDP&0x30)==0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
				{//�Ƿ�����ģʽ
					i+=60;//��60����ܵ���վ����������ʱ���
				}
				if(PORTn==GPRSPORT)
				{
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS�����붨ʱ��
					if(UARTCtrl->HostACK<=1)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					{
/*
						i=MRR(ADDR_AFN04F8+4,1);
						if(i==0x0)
						{
							i=1;//��С��������ģʽ������ͨ���Զ�����ʱ��1��
						}
						i*=60;
						Terminal_Ram->GPRSAutoDiscon_S_Timer=i;//GPRS��������ģʽ������ͨ���Զ������붨ʱ��
*/
					}
				}
				if(PORTn==ENETPORT)
				{
					Terminal_Ram->ENETHeartBeat_S_Timer=i;//��̫�������붨ʱ��
				}
			#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				//���Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				i=MRR(ADDR_8001_3+1,2);
				i=(i>>8)|((i&0xff)<<8);
				if(i==0)
				{//0��ʾ���Զ�����
					i=60;
				}
				Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
			#endif
			#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				//���Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
				{
					i=MRR(ADDR_8001_3+1,2);
					i=(i>>8)|((i&0xff)<<8);
					if(i==0)
					{//0��ʾ���Զ�����
						i=60;
					}
					Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				}
			#endif
			}
			if(PowerEvent->task!=100)
			{
				Comm_Ram->BattWork_S_timer=60;//4 ��ع����ػ��붨ʱ��
			}
		#if LOG_698==1//0=û,1=��
			LOG(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
		#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
			LOGRS232(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
			if(PORTn!=GPRSPORT)
			{//GPRS���Լ�
				UARTCtrl->RxTxByte+=UARTCtrl->RxByte;
			}
//		#if (USER/100)==0//���Ժ����
			Uart_698_Data(PORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
		#if((Project/100)==2)||((Project/100)==4)
			if(UARTCtrl->Lock_InfoTask==0x55)//��·������������Ϣ������:0x55=����
			{
				UARTCtrl->Task=1;
				break;
			}
		#endif
//		#else
//			i=MRR(ADDR_F101_2+1,1);//��ȫģʽ����
//			if(i)
//			{
//				Uart_698_Data(PORTn,1);//UART_698����;���:��ȫģʽSECURITY=0������=1����
//			}
//			else
//			{
//				Uart_698_Data(PORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
//			}
//		#endif
			UARTCtrl->RxByte=0;//rx����=0
			break;
		case 2://2=�ȴ���֡���ݷ���
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT))
			{
				break;
			}
			if(p16timer[0]==0x0)
			{
//				LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
//				p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
//				LEN_ADDR=p8tx[4];//ȡ��ַ�ֽ���
//				LEN_ADDR&=0x0f;
//				LEN_ADDR++;
//				LEN_ADDR+=2;
//				LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
//				LEN_TAPDU-=3+LEN_ADDR+2+2;
				LEN_TAPDU=0;//RS232,RS485��·�㲻��֡
				if(LEN_TAPDU<=LENmax_TxAPDU)//��·���֡���ͣ�ÿ֡���APDU�ֽ���
				{
				#if LOG_698==1//0=û,1=��
					LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
				#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
					LOGRS232(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
					if(PORTn!=GPRSPORT)
					{//GPRS���Լ�
						UARTCtrl->RxTxByte+=UARTCtrl->TxByte;
					}
					UARTCtrl->Task=3;//���ڷ��ͱ�־
					Start_UARTn_Tx(PORTn);//����UART�ڷ���
				}
				else
				{
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UARTCtrl->TxByte);
					UARTCtrl->TxAPDUSEQ=0;//��·���֡��ż���0-4095
					UARTCtrl->TxAPDUByte=0;//��·���֡�ѷ���APDU�ֽ���
					UARTCtrl->Task=5;//5=�ȴ���·���֡���ݷ���
				}
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
//		#if (USER/100)==0//���Ժ����
			Uart_698_Data(PORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
		#if((Project/100)==2)||((Project/100)==4)
			if(UARTCtrl->Lock_InfoTask==0x55)//��·������������Ϣ������:0x55=����
			{
				UARTCtrl->Task=4;
				break;
			}
		#endif
//		#else
//			i=MRR(ADDR_F101_2+1,1);//��ȫģʽ����
//			if(i)
//			{
//				Uart_698_Data(PORTn,1);//UART_698����;���:��ȫģʽSECURITY=0������=1����
//			}
//			else
//			{
//				Uart_698_Data(PORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
//			}
//		#endif
			UARTCtrl->RxByte=0;//rx����=0
			break;
		case 5://5=�ȴ���·���֡���ݷ���
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT))
			{
				break;
			}
			if(p16timer[0]==0x0)
			{
				LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
				p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
				LEN_ADDR=p8tx[4];//ȡ��ַ�ֽ���
				LEN_ADDR&=0x0f;
				LEN_ADDR++;
				LEN_ADDR+=2;
				LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
				LEN_TAPDU-=3+LEN_ADDR+2+2;//������L�Ϳ�����C=3byte,֡ͷУ��=2byte,֡У��=2byte
				MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
				p8tx[3]|=(1<<5);//��֡��־λ
				LEN_Tx=4+LEN_ADDR+2;
				i=UARTCtrl->TxAPDUByte;
				if(i>=LEN_TAPDU)
				{
					UARTCtrl->RxByte=0;//rx����=0
					UARTCtrl->Task=0;//0=����
					return;
				}
				LEN_TxAPDU=LEN_TAPDU-i;
			//��֡��ʽ����
			//bit0~bit11����ʾ��֡������̵�֡��ţ�ȡֵ��Χ0~4095��ѭ��ʹ�ã�
			//bit12~bit13��������
			//bit15=0��bit14=0����ʾ��֡����������ʼ֡��
			//bit15=1��bit14=0����ʾ��֡����ȷ��֡��ȷ��֡������APDUƬ���򣩣�
			//bit15=0��bit14=1����ʾ��֡�������֡��
			//bit15=1��bit14=1����ʾ��֡�����м�֡��
				x=UARTCtrl->TxAPDUSEQ;//��·���֡��ż���0-4095
				UARTCtrl->TxAPDUSEQ++;//��·���֡��ż���0-4095
				x&=0x7ff;
				if(UARTCtrl->TxAPDUByte==0)
				{//��1֡
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{
					if(LEN_TxAPDU>LENmax_TxAPDU)//��·���֡���ͣ�ÿ֡���APDU�ֽ���
					{//�м�֡
						x|=0xC000;
						LEN_TxAPDU=LENmax_TxAPDU;
					}
					else
					{//���֡
						x|=0x4000;
					}
				}
				p8tx[LEN_Tx]=x;
				p8tx[LEN_Tx+1]=x>>8;
				LEN_Tx+=2;
				MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UARTCtrl->TxAPDUByte,LEN_TxAPDU);
				UARTCtrl->TxAPDUByte+=LEN_TxAPDU;
				LEN_Tx+=LEN_TxAPDU;
				LEN_Tx+=2+1;
				UARTCtrl->TxByte=LEN_Tx;
				LEN_Tx-=2;
				p8tx[1]=LEN_Tx;
				p8tx[2]=LEN_Tx>>8;
				Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
				
			#if LOG_68==1//0=û,1=��
				LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
				LOGRS232(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				if(PORTn!=GPRSPORT)
				{//GPRS���Լ�
					UARTCtrl->RxTxByte+=UARTCtrl->TxByte;
				}
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
		case 6://6=��·���֡���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->RxByte=0;//rx����=0
				p16timer[0]=0;//��ʱ��ʱ=0;���ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
				
				UARTCtrl->Task=0;
			}
			break;
		case 7://7=�ȴ���������תΪ����
			UARTCtrl->Task=0;//0=���ڽ���
			UARTCtrl->RxByte=0;//rx����=0
			p16timer[0]=0;//��ʱ��ʱ=0;���ճ�ʱ��ʱ�ӽ���FIFO��������ʱ��ʼ��ʱ,ƽʱ=0�Ա�������������
			break;		
		case 8:
			if(UARTCtrl->Lock==0x55)
			{
				return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			}
			UARTCtrl->Task=0;//0=����
			UARTCtrl->AddrType=0;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
//			UARTCtrl->TxFrameCount=0;//�Ĵ��֡����ʱ�ѷ���֡����
//			UARTCtrl->MulFrameGPCR=0;//��֡ʱͨ�ü����Ĵ���
//			UARTCtrl->LockFlags&=(~(7<<4));//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7
			UARTCtrl->SubTask=0;//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
			//����-������ʱ
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS�����ͨ��
				p16timer[0]=0;//��ʱ0MS����
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//��ʱxMS����(��֮֡�����·���м��������33λ)
			}
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{//ͨ��ģ��
				//��������ʱ(��ȷ�յ��κ�֡,��������ʱ)
				if(PORTn==GPRSPORT)
				{
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				}
				else
				{
					i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				}
				if(i==0)
				{
					i=120;
				}
				else
				{
					i=MRR(i+1,2);
					i=(i>>8)+((i&0xff)<<8);
				}
				
				if((UARTCtrl->TCPUDP&0x30)==0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
				{//�Ƿ�����ģʽ
					i+=60;//��60����ܵ���վ����������ʱ���
				}
				if(PORTn==GPRSPORT)
				{
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS�����붨ʱ��
					if(UARTCtrl->HostACK<=1)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					{
/*
						i=MRR(ADDR_AFN04F8+4,1);
						if(i==0x0)
						{
							i=1;//��С��������ģʽ������ͨ���Զ�����ʱ��1��
						}
						i*=60;
						Terminal_Ram->GPRSAutoDiscon_S_Timer=i;//GPRS��������ģʽ������ͨ���Զ������붨ʱ��
*/
					}
				}
				if(PORTn==ENETPORT)
				{
					Terminal_Ram->ENETHeartBeat_S_Timer=i;//��̫�������붨ʱ��
				}
			#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				//���Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				i=MRR(ADDR_8001_3+1,2);
				i=(i>>8)|((i&0xff)<<8);
				if(i==0)
				{//0��ʾ���Զ�����
					i=60;
				}
				Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
			#endif
			#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				//���Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
				{
					i=MRR(ADDR_8001_3+1,2);
					i=(i>>8)|((i&0xff)<<8);
					if(i==0)
					{//0��ʾ���Զ�����
						i=60;
					}
					Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
				}
			#endif
			}
			if(PowerEvent->task!=100)
			{
				Comm_Ram->BattWork_S_timer=60;//4 ��ع����ػ��붨ʱ��
			}
		#if LOG_698==1//0=û,1=��
			LOG(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
		#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
			LOGRS232(698,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
			if(PORTn!=GPRSPORT)
			{//GPRS���Լ�
				UARTCtrl->RxTxByte+=UARTCtrl->RxByte;
			}
//		#if (USER/100)==0//���Ժ����
			i=MRR(ADDR_F101_2+1,1);//��ȫģʽ����
			if(i)
			{
				Uart_698_Data(PORTn,1);//UART_698����;���:��ȫģʽSECURITY=0������=1����
			}
			else
			{
				Uart_698_Data(PORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
			}
//			Uart_698_Data(PORTn,1);//UART_698����;���:��ȫģʽSECURITY=0������=1����
		#if((Project/100)==2)||((Project/100)==4)
			if(UARTCtrl->Lock_InfoTask==0x55)//��·������������Ϣ������:0x55=����
			{
				UARTCtrl->Task=1;
				break;
			}
		#endif
//		#else
//			i=MRR(ADDR_F101_2+1,1);//��ȫģʽ����
//			if(i)
//			{
//				Uart_698_Data(PORTn,1);//UART_698����;���:��ȫģʽSECURITY=0������=1����
//			}
//			else
//			{
//				Uart_698_Data(PORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
//			}
//		#endif
			UARTCtrl->RxByte=0;//rx����=0
			break;
		default:
			UARTCtrl->Task=0;//0=���ڽ���
			break;
	}
}

void Uart_698_Data(u32 PORTn,u32 SECURITY)//UART_698����;���:��ȫģʽSECURITY=0������=1����
{
	u32 i;
	u32 x;
	u32 LEN_SA;
	u32 LEN_RxAPDU;
	u8 * p8rx;
  UARTCtrl_TypeDef * UARTCtrl;
	ms_Type *ms;
	u8* p8;
	u32 DataAdd33;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	if(p8rx[3]&0x80)//������C,bit7=���䷽��λ��DIR=0��ʾ��֡���ɿͻ���������,DIR=1��ʾ��֡���ɷ�����������;bit6=������־λ��PRM=1��ʾ��֡���ɿͻ�������ģ�PRM=0��ʾ��֡���ɷ����������;bit5=��֡��־λ
	{//����վ����֡
		return;
	}
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	if(LEN_RxAPDU<(LEN_SA+8+1+2))
	{
		return;//���ȴ�
	}
	i=p8rx[4];
//#if (USER/100)==0//���Ժ����
	if((i&0x30)==0x30)
	{
		return;//bit4��bit5�߼���ַ=3�����Ժһ���Բ���ʱ��Ϊ�����ܲ���������Ե�ѹ�ϸ���ʱ���߼���ַ1�����������߼���ַ0
	}
//#endif
	i>>=6;
	switch(i)//bit6��bit7��Ϊ��ַ���������ַ������־�����ñ��뷽ʽ��0��ʾ����ַ��1��ʾͨ���ַ��2��ʾ���ַ��3��ʾ�㲥��ַ
	{
		case 0://0��ʾ����ַ
		case 1://1��ʾͨ���ַ
			if(PORTn!=ACSAMPLEPORT)
			{
				i=Uart_TSA_Comp(p8rx+4,(u8 *)ADDR_4001+1);//ͨ�ŵ�ַ�Ƚ�,pFLASH���ݶ���ADDR_128K_DDATABUFF�бȽ�,����:0=��ͬ,!=0����ͬ
				if(i)
				{
					return;//��ַ��ͬ
				}
				i=p8rx[4];
				i&=0x0f;
				i++;
				MR((u32)p8rx+4+1,ADDR_128KDATABUFF+1,i);
			}
			UARTCtrl->AddrType=1;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			break;
		case 2://2��ʾ���ַ
//68 xx 00 43 LEN_SA 01 02 03 10 FF FF 06 01 02 40 00 02 00 1c 07 e1 07 0d 0f 38 06 00 1c 43 16
			//�޳�Ͷ��
			i=MRR(ADDR_4024+1,1);//����2��=enum {�޳�Ͷ�루1�����޳������2��}
			if(i==1)
			{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
				i=(p8rx[5+LEN_SA+3]<<8)|p8rx[5+LEN_SA+4];
				if(i!=0x0601)
				{//
					return;
				}
				i=(p8rx[5+LEN_SA+6]<<24)|(p8rx[5+LEN_SA+7]<<16)|(p8rx[5+LEN_SA+8]<<8)|p8rx[5+LEN_SA+9];
				if(i!=0x40000200)
				{//!��ʱ����
					return;
				}
			}
			MR(ADDR_DATABUFF,ADDR_4005,LENmax_4005);
			p8=(u8*)ADDR_DATABUFF;
			x=p8[1];//������
			p8+=2;
			for(i=0;i<x;i++)
			{
				LEN_RxAPDU=Uart_TSA_Comp(p8rx+4,p8+1);//ͨ�ŵ�ַ�Ƚ�,pFLASH���ݶ���ADDR_128K_DDATABUFF�бȽ�,����:0=��ͬ,!=0����ͬ
				if(LEN_RxAPDU==0)
				{//��ͬ
					break;
				}
				p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);
			}
			if(i>=x)
			{
				return;//��ַ��ͬ
			}
			UARTCtrl->AddrType=2;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			break;
		case 3://3��ʾ�㲥��ַ
			//ֻ�й㲥Уʱ�Ͷ�ͨ�ŵ�ַʱ��Ч
//68 xx 00 43 00 AA 10 FF FF 05 01 00 40 01 02 00 00 XX XX 16
//68 xx 00 43 00 AA 10 FF FF 07 01 01 40 00 7F 00 1C yy yy mm dd hh mm ss 00 XX XX 16
//68 xx 00 43 00 AA 10 FF FF 06 01 02 40 00 02 00 1c 07 e1 07 0d 0f 38 06 00 1c 43 16
			i=(p8rx[9]<<8)|p8rx[10];
			switch(i)
			{
				case 0x0501://��ͨ�ŵ�ַ
					i=(p8rx[12]<<24)|(p8rx[13]<<16)|(p8rx[14]<<8)|p8rx[15];
					if(i==0x40010200)
					{
						break;
					}
					return;
				default:
					//�޳�Ͷ��
					i=MRR(ADDR_4024+1,1);//����2��=enum {�޳�Ͷ�루1�����޳������2��}
					if(i==1)
					{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
						i=(p8rx[9]<<8)|p8rx[10];
						if(i!=0x0601)
						{//
							return;
						}
						i=(p8rx[12]<<24)|(p8rx[13]<<16)|(p8rx[14]<<8)|p8rx[15];
						if(i!=0x40000200)
						{//!��ʱ����
							return;
						}
					}
					break;
			}
			UARTCtrl->AddrType=3;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			break;
	}

	if(p8rx[3]&0x08)//�����־λ SC
	{
		//68 17 00 4B 05 02 00 00 00 00 00 10 82 21 38 34 48 73 33 35 33 33 38 78 16 
		DataAdd33=1;
		LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
		i=4+LEN_SA+3+2;
		if(LEN_RxAPDU>=i)
		{
			LEN_RxAPDU-=i;
		}
		else
		{
			LEN_RxAPDU=0;
		}
		for(i=0;i<LEN_RxAPDU;i++)
		{
			p8rx[5+LEN_SA+3+i]-=0x33;
		}
	}
	else
	{
		DataAdd33=0;
	}
	
	if(p8rx[3]&(1<<5))//��֡��־λ
	{
		//��֡����ȷ��
		UARTCtrl->Task=5;//5=�ȴ���·���֡���ݷ���
		return;
	}
	UARTCtrl->Lock=0x55;//��ֹ�߳��ٴε���
	switch(p8rx[3]&0x7)//Ӧ�ò㹦����
	{
		//0	����	
		case 1://	��·����	��·���ӹ�����¼���������˳���¼��
//LINK-APDU��=CHOICE
//{
//  Ԥ��������	     [1]		LINK-Request��
//Ԥ������Ӧ       [129]	LINK-Response
//}
			i=p8rx[LEN_SA+8];
			if(i==129)//Ԥ������Ӧ       [129]	LINK-Response
			{
				LINK_Response(PORTn);
			}
			break;
		//2	����	
		case 3://	�û�����	Ӧ�����ӹ������ݽ�������
//�ͻ���Ӧ�ò�Э�����ݵ�Ԫ��Client-APDU��
//Client-APDU��=SEQUENCE
//{
//  Ӧ�ò����  CHOICE
//  {
//    ����Ӧ����������	 [2]		CONNECT-Request��
//    �Ͽ�Ӧ����������	 [3]		RELEASE-Request��
//    ��ȡ����			 [5]		GET-Request��
//    �������� 			 [6]		SET-Request��
//    �������� 			 [7]		ACTION-Request��
//    �ϱ�Ӧ�� 			 [8]		REPORT-Response��
//    ��������   		 [9]		PROXY-Request
//}��
//ʱ���ǩ    TimeTag  OPTIONAL
//}
			i=p8rx[LEN_SA+8];
			switch(i)
			{
				case 2://����Ӧ����������	 [2]		CONNECT-Request��
					UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					CONNECT_Request(PORTn);//����
					break;
				case 3://�Ͽ�Ӧ����������	 [3]		RELEASE-Request��
					UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					RELEASE_Request(PORTn);//�Ͽ�
					break;
				case 5://��ȡ����			 [5]		GET-Request��
					GET_Request(PORTn);
					break;
				case 6://�������� 			 [6]		SET-Request��
					UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					if(SECURITY==0)//��ȫģʽSECURITY=0������=1����
					{
						SET_Request(PORTn);
					}
					break;
				case 7://�������� 			 [7]		ACTION-Request��
					UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					if(SECURITY==0)//��ȫģʽSECURITY=0������=1����
					{
						ACTION_Request(PORTn);
					}
					break;
				case 8://�ϱ�Ӧ�� 			 [8]		REPORT-Response��
					UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					REPORT_Response(PORTn);
					break;
				case 9://��������   		 [9]		PROXY-Request
					if(UARTCtrl->REPORT_NUMmax)//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
					{
						break;//�������ϱ����ڽ��У��������������������ϱ�֡Ҳ����UARTnFnDataBuff
					}
					if(SECURITY==0)//��ȫģʽSECURITY=0������=1����
					{
						PROXY_Request(PORTn);
					}
					break;
				case 16://��ȫ����   		[16]		SECURITY-Request
					SECURITY_Request(PORTn);//��ȫ����
					UARTCtrl->SECURITY_Request=0;//��ȫ����:b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
					break;
				case 144://��ȫ��Ӧ   		[144]	SECURITY-Response
					SECURITY_Response(PORTn);//��ȫ��Ӧ
					break;
			}
			
			break;
		//4��7	����	
	}
	UARTCtrl->Lock=0;//����߳��ٴε���
	if(UARTCtrl->AddrType!=1)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
	{
		if(UARTCtrl->AddrType==3)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
		{
			i=(p8rx[12]<<24)|(p8rx[13]<<16)|(p8rx[14]<<8)|p8rx[15];
			if(i==0x40010200)
			{//�Ƕ�ͨ�ŵ�ַ
				return;
			}
		}
		UARTCtrl->Task=0;//����ش�
		if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
		return;
	}
	if(DataAdd33)
	{
		p8rx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		p8rx[3]|=0x08;//�����־λ SC
		LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
		i=4+LEN_SA+3+2;
		if(LEN_RxAPDU>=i)
		{
			LEN_RxAPDU-=i;
		}
		else
		{
			LEN_RxAPDU=0;
		}
		for(i=0;i<LEN_RxAPDU;i++)
		{
			p8rx[5+LEN_SA+3+i]+=0x33;
		}
		Uart_698_HCSFCS(p8rx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	}
}



u32 Uart_TSA_Comp(u8 *p8rx,u8 *pFLASH)//ͨ�ŵ�ַ�Ƚ�,pFLASH���ݶ���ADDR_128K_DDATABUFF�бȽ�,����:0=��ͬ,!=0����ͬ
{
	u32 i;
	
	i=p8rx[0];
	i&=0x0f;
	i++;
	MR(ADDR_128KDATABUFF,(u32)pFLASH,i+1);
	Data_Inverse(ADDR_128KDATABUFF+1,i);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	pFLASH=(u8*)ADDR_128KDATABUFF;
	if(i!=pFLASH[0])
	{
		return 1;
	}
	while(i--)
	{
		p8rx++;
		pFLASH++;
		if((p8rx[0]&0x0f)!=0x0a)
		{
			if((p8rx[0]&0x0f)!=(pFLASH[0]&0x0f))
			{
				return 1;
			}
		}
		if((p8rx[0]&0xf0)!=0xa0)
		{
			if((p8rx[0]&0xf0)!=(pFLASH[0]&0xf0))
			{
				return 1;
			}
		}
	}
	return 0;
}


u32 Uart_698_TxHead(u8 *p8tx,u32 C,u32 CA)//Uart698֡ͷ���뷢�ͻ�����,���:C=������,CA=�ͻ�����ַ;����:֡ͷ����
{
	u32 Len;
	
	
//��ʼ�ַ���68H��  1Byte
	p8tx[0]=0x68;
//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
//������C  1Byte
	p8tx[3]=C;
//��ַ��A
	//��ַ��ʶAF  1Byte(bit0��bit3��Ϊ��ַ���������ַ���ֽ��������ñ��뷽ʽ��ȡֵ��Χ��0��15����Ӧ��ʾ1��16���ֽڳ���)
	//��������ַSA  �ɱ�
	MR(((u32)p8tx)+4,ADDR_4001+1,17);
	Len=p8tx[4];
	if(Len>16)
	{
		Len=16;
	}
	if(Len==0)
	{
		Len=1;
	}
	Data_Inverse((u32)p8tx+5,Len);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	p8tx[4]=Len-1;
	//�ͻ�����ַCA  1Byte
	p8tx[5+Len]=CA;
//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
	
	Len+=8;
	return Len;
	
}

void Uart_698_TxResponse(u32 PORTn,u32 TI,u32 LEN_TxAPDU)//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
{
	u32 i;
	u32 x;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	u16* p16;

	if(LEN_TxAPDU==0)
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	
//�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
	p8tx[LEN_TxAPDU]=0;
	LEN_TxAPDU+=1;
	//PIID-ACD�޸�
	
//ʱ���ǩ
	p8tx[LEN_TxAPDU]=0;
	LEN_TxAPDU+=1;
//����֡
	if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	{
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		i=p8rx[4];//ȡ��ַ�ֽ���
		i&=0x0f;
		i+=9;
		MR((u32)p8tx,(u32)p8rx,i);//���ƽ���ͷ
		p8tx[3]|=0x80;
		
		MR(((u32)p8tx)+i,((u32)p8tx)+TxAPDUOffset,LEN_TxAPDU);
		i+=LEN_TxAPDU;
	//2byte FCS
	//1byte 0x16
		i+=3;
		UARTCtrl->TxByte=i;
		i-=2;
		p8tx[1]=i;
		p8tx[2]=i>>8;
		Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		UARTCtrl->Task=2;//�ȴ���֡���ݷ���
	}
	else
	{
		//����֡
		//copy rx��rx+Rxoffset
		p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
		i=Get_LEN_UARTnRx(PORTn);
		x=p8rx[1]+(p8rx[2]<<8);
		x+=3;//������
		x>>=1;
		x<<=1;
		if((x+Rxoffset)>i)
		{
			x=i-Rxoffset;
		}
		x/=2;
		p16=(u16*)p8rx;
		while(x)
		{
			x--;
			p16[(Rxoffset/2)+x]=p16[x];
		}
//6.3.8.2.5��GetResponseNext��������
//GetResponseNext��=SEQUENCE
//{
//  �������-���ȼ�-ACD  	PIID-ACD��
//  ĩ֡��־           	BOOLEAN��
//��֡���           	long-unsigned��
//��֡��Ӧ             	CHOICE
//{
//������Ϣ    	    	[0]   DAR��
//  ��������         	[1]   SEQUENCE OF A-ResultNormal��
//  ��¼�Ͷ�������    	[2]   SEQUENCE OF A-ResultRecord
//}
//}
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		i=p8rx[4];//ȡ��ַ�ֽ���
		i&=0x0f;
		i+=9;
		MR((u32)p8tx,(u32)p8rx,i);//���ƽ���ͷ
		p8tx[3]|=0x80;//0xA0
		p8tx[i]=p8tx[TxAPDUOffset];//��ȡ��Ӧ
		//p8tx[i+1]=p8tx[TxAPDUOffset+1];//GeResponseRecord
		p8tx[i+1]=5;//��֡��Ӧһ�����ݿ� 	[5]  GetResponseNext
		p8tx[i+2]=UARTCtrl->PIID;//PIID
		i+=3;
//  ĩ֡��־           	BOOLEAN��
		if(UARTCtrl->NEXT==3)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
		{
			p8tx[i]=1;
		}
		else
		{
			p8tx[i]=0;
		}
//��֡���           	long-unsigned��
		p8tx[i+1]=UARTCtrl->NEXTBlock>>8;
		p8tx[i+2]=UARTCtrl->NEXTBlock;
//��֡��Ӧ             	CHOICE
//{
//	������Ϣ    	    	[0]   DAR��
//  ��������         	[1]   SEQUENCE OF A-ResultNormal��
//  ��¼�Ͷ�������    	[2]   SEQUENCE OF A-ResultRecord
//}
		if((UARTCtrl->GET_Reques_CHOICE==3)||(UARTCtrl->GET_Reques_CHOICE==4))
		{//3=��ȡһ����¼�Ͷ�����������,4=��ȡ���ɸ���¼�Ͷ�����������
			p8tx[i+3]=2;//��¼�Ͷ�������    	[2]   SEQUENCE OF A-ResultRecord
		}
		else
		{
			p8tx[i+3]=1;//��������         	[1]   SEQUENCE OF A-ResultNormal
		}
		p8tx[i+4]=1;//1=����
		
		i+=5;
		if(LEN_TxAPDU<3)
		{
			return;
		}
		MR(((u32)p8tx)+i,((u32)p8tx)+TxAPDUOffset+3,LEN_TxAPDU-3);
		i+=(LEN_TxAPDU-3);
	//2byte FCS
	//1byte 0x16
		i+=3;
		UARTCtrl->TxByte=i;
		i-=2;
		p8tx[1]=i;
		p8tx[2]=i>>8;
		Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		UARTCtrl->Task=2;//�ȴ���֡���ݷ���
	}
	
}

u32 Uart_TimeTag(u8* p8rx)//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
{
	u32 i;
	u32 TI;
	
	i=Check_pYYMDHMS_hex(p8rx);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
	if(i)
	{
		return 1;//����:0=��Ч,1=��Ч
	}
	TI=(p8rx[8]<<8)+p8rx[9];//ȡƵ��ֵ
	if(TI==0)
	{
		return 0;//����:0=��Ч,1=��Ч
	}
	switch(p8rx[7])
	{
		case 0://��      ��0����
			break;
		case 1://��      ��1����
			TI*=60;
			break;
		case 2://ʱ      ��2����
			TI*=60*60;
			break;
		case 3://��      ��3����
		case 4://��      ��4����
		case 5://��      ��5��
		default:
			TI*=60*60*24;
			break;
	}
	i=YYMDHMS_Sub_hex((u8*)&Comm_Ram->DL698YMDHMS,p8rx);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
	if(i>>31)
	{
		i=~i;
		i++;
	}
	if(i>TI)
	{
		return 1;//����:0=��Ч,1=��Ч
	}
	return 0;//����:0=��Ч,1=��Ч
}

#endif
