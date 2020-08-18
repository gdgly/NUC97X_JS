
#include "Project.h"
#include "Terminal_ReadMeter_WSH.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"


#if (USER/100)==10//�ӱ�
u32 WSH41_Rx(u32 PORTn)//��ʤ����;����0û���,1���,2��ʱ
{
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+6);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	
	p0=p16fifo[0];//�жϽ���ָ��
	p1=p16fifo[1];//��ǰ�Ѷ�ָ��
	if((p0>=(LEN_UARTnIntRx-6))||(p1>=(LEN_UARTnIntRx-6)))
	{
		p16fifo[0]=0;//�жϽ���ָ��
		p16fifo[1]=0;//��ǰ�Ѷ�ָ��
		p16fifo[2]=0;//��Լ���ָ��
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//�����ݽ������ó�ʱ��ʱ
		p16timer[0]=2000/10;//�ֽڼ䳬ʱ��ʱ
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-6))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(p8txbuff[0]!=p8rxbuff[0])
			{//���յ���֡��ַ�ͷ��͵Ĳ�һ��
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte==4)
			{
				if(i!=0x0D)
				{//��ʤ��Լ������Ϊ0x0D
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=9)
				{
					if(RxByte>=(p8rxbuff[8]+11))
					{
						if(i==0x0D)
						{//�������
							//��У�����
							Pn=0;
							for(i=4;i<(p8rxbuff[8]+9);i++)
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


u32 WSH41_RxDATA(u32 PORTn)//��ʤ��ѯӦ�������������;����0Ӧ����ȷ��1��ַ��2�����־�ִ�3��־��ַ��4���ݳ��ȴ�
{
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8rxbuff+=4;
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//��ַ�Ƚ�
	if(p8txbuff[0]!=p8rxbuff[0])
	{
		return 1;//����0Ӧ����ȷ��1��ַ��2�����־�ִ�3��־��ַ��4���ݳ��ȴ�
	}
	//�����־�ֱȽ�
	if(p8rxbuff[1]!=0xAA&&p8rxbuff[1]!=0xA5)
	{
		return 2;//����0Ӧ����ȷ��1��ַ��2�����־�ִ�3��־��ַ��4���ݳ��ȴ�
	}
	//��־��ַ�Ƚ�
	if((p8txbuff[2]!=p8rxbuff[2])||(p8txbuff[3]!=p8rxbuff[3]))
	{
		return 3;//����0Ӧ����ȷ��1��ַ��2�����־�ִ�3��־��ַ��4���ݳ��ȴ�
	}
	//���ݳ��ȱȽ�
	if(p8txbuff[4]!=p8rxbuff[4])
	{
		return 4;//����0Ӧ����ȷ��1��ַ��2�����־�ִ�3��־��ַ��4���ݳ��ȴ�
	}
	return 0;//����0��ȷ,1����
}

#endif
