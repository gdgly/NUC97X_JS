

#include "Project.h"
#include "Terminal_ReadMeter_HX.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../terminal/airthmetic.h"


//void Uart_HX_RxTx(u32 PORTn);
void Read_HX_6E_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);
u32 HX_Rx(u32 PORTn);
u32 HX_6E_RxData(u32 PORTn);

#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
//void Uart_HX_RxTx(u32 PORTn)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//{
//	u32 i;
//    UARTCtrl_TypeDef * UARTCtrl;
//	u16 * p16timer;
//	
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
//	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//	{
//		default:
//			UARTCtrl->Task=0;
//			break;
//		case 0://0=����
//			break;
//		case 1://1=���1֡����
//			break;
//		case 2://2=���ճ�ʱ(�����)
//			break;
//		case 3://3=��������
//			if(p16timer[0]!=0)
//			{
//				break;
//			}
//			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
//			Start_UARTn_Tx(PORTn);//����UART�ڷ���
//			UARTCtrl->Task=4;
//#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
//			if(PORTn==RS485_1PORT)
//			{
//				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//			}
//			if(PORTn==RS485_2PORT)
//			{
//				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//			}
//#endif
//			break;
//		case 4://4=���ڷ���
//			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
//			{
//				p16timer[0]=UARTCtrl->TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
//				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
//				UARTCtrl->Task=5;
//			}
//			break;
//		case 5://5=���ڽ���
//			i=HX_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
//			switch(i)
//			{
//				case 0://0û���
//					return;
//				case 1://1���
//					UARTCtrl->Task=1;
//					break;
//				default://2��ʱ
//					UARTCtrl->Task=2;
//					return;
//			}
//			p16timer[0]=50/10;//��֡����������ʱ50MS��	  (��ʱ���ĵ�λ��10ms)
//#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
//			if(PORTn==RS485_1PORT)
//			{
//				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//			}
//			if(PORTn==RS485_2PORT)
//			{
//				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//			}
//#endif
//			break;
//	}
//}

u32 HX_Rx(u32 PORTn)//HX֡����;����0û���,1���,2��ʱ
					//���������ת�������ռĴ����������Ѿ������˱仯
{
	u32 i;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u16 j;
	uint8_t crc16_resh = 0;
  uint8_t crc16_resl = 0;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);  //�õ�UART�жϽ��ջ��峤��	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);		   //�õ�UART���ջ��峤��
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);		   //�õ�UART���ջ����ַ
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+6);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	
	p0=p16fifo[0];//�жϽ���ָ��	  2�ֽ�
	p1=p16fifo[1];//��ǰ�Ѷ�ָ��	  2�ֽ�
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
//		p16timer[0]=1000/10;//�ֽڼ䳬ʱ��ʱ	
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
		if(p1>=(LEN_UARTnIntRx-6))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;

		if(RxByte==1)
		{
			if(i!=2)
			{	
				RxByte=0;	
				continue;
			}
		}
		else
		{
			if(RxByte==2) 	
			{
				if(i!=0x45)
				{	
					RxByte=0;   	
					continue; 	
				}
			}
			else
			{
				if(RxByte>=16)	//���㲻��չ��������ĳ���
				{
					if(i == 3)
					{
						i = CheckHXRxData(p8rxbuff,0,RxByte);
						if(i>=1000)	
						{	
							RxByte=0; 
							continue;	
						}
						else
						{
							j = Crc_HX(p8rxbuff, i-3); //���������У��ֵ
							crc16_resl = (uint8_t)(j&0XFF);
					    j  >>=8;
					    crc16_resh  = (uint8_t)(j&0XFF);
							if ((p8rxbuff[i-3]==crc16_resh)&&(p8rxbuff[i-2]== crc16_resl))
					    {//��У����ȷ
								p16fifo[1]=p1;//��ǰ�Ѷ�ָ��
								p16fifo[2]=p1;//���ָ��==��ָ��,���ʡʱ
								UARTCtrl->RxByte=i;
								return 1;
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
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		return 2;//2=���ճ�ʱ
	}
	return 0;
}

u32 HX_6E_RxData(u32 PORTn)//HX֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
{
	u32 i,j;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 datalength;
	UARTCtrl_TypeDef * UARTCtrl;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);					    
	datalength = UARTCtrl->TxCount;
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	j=0;

	//ͨ�ŵ�ַ�Ƚ�
	for(i=0;i<4;i++)
	{
		if(p8txbuff[2+i+j] == 0x10)
		{
			j++;
			if(p8rxbuff[6+i]!=(p8txbuff[2+i+j]-0x40))	
			{
				break; //�������ȣ�ֱ���˳�	
			}
		}
		else 
		{
			if(p8rxbuff[6+i]!=p8txbuff[2+i+j])
			{
				break;
			}
		}
	}
	if(i!=4)
	{
	  return 1;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
	}
	CheckHXRxData(p8txbuff,0,(u32)datalength);
	if(p8txbuff[12] == 0x4c)	   //�����ж�
	{
	//	if([12] != 0x06) return 3;  
	}
	else 	   //�ǵ����ж�
	{
		for(i=0;i<3;i++)
		{
			if(p8rxbuff[12+i]!=p8txbuff[12+i])	
			{
				break;
			}
		}
		if(i!=3)
		{
			return 3;	//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
		}	   
	}
	return 0;//����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
}

#endif			

