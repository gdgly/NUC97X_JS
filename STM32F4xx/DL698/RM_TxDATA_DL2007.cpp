

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"



void DL645_68H_Add33H_CS_16H(u8* p8tx)//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
{
	u32 i;
	u32 x;

	//��ʼ��
	p8tx[0]=0x68;
	p8tx[7]=0x68;
	//��0x33
	x=p8tx[9];
	for(i=0;i<x;i++)
	{
		p8tx[10+i]+=0x33;
	}
	//��β0x16
	p8tx[10+x+1]=0x16;
	//cs
	x+=10;
	p8tx[x]=0;
	for(i=0;i<x;i++)
	{
		p8tx[x]+=p8tx[i];
	}
}

u32 RM_TxDATA_DL2007_DI(u32 PORTn,u32 DI)//������֡DI����n�θ���
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	if(((i>>24)&0xf)==1)
	{//��ͨ�ɼ�����
		i>>=20;
		i&=0xf;
		switch(i)//�ɼ�����
		{
			default:
			case 0://0=�ɼ���ǰ����
				break;
			case 1://1=�ɼ��ϵ�N��
				i=UARTCtrl->RM_DI_N;//�ɼ��ϵ�N��
				if(i)
				{
					i--;
				}
				DI+=i;
				break;
			case 2://2=������ʱ��ɼ�
				break;
			case 3://3=��ʱ�����ɼ�
				////����������,������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
				break;
			case 4://RetryMetering ����
				i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
				i++;
				DI&=0xffffff00;
				DI|=i;
				break;
		}
	}
	return DI;
}

u32 RM_TxDATA_DL2007(u32 PORTn,u8 *p8CSD,u8 *p8tx)//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
	u32 i;
	u32 DI;
	u32 LEN_DATA;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->RM_SubsequentFrames==0)//0=�޶�����֡,1-n=����֡��֡��
	{//�Ǻ���֡
		p8tx[0]=0x68;
		p8tx[1]=UARTCtrl->RMAddr[2]>>8;
		p8tx[2]=UARTCtrl->RMAddr[2];
		p8tx[3]=UARTCtrl->RMAddr[1]>>8;
		p8tx[4]=UARTCtrl->RMAddr[1];
		p8tx[5]=UARTCtrl->RMAddr[0]>>8;
		p8tx[6]=UARTCtrl->RMAddr[0];
		p8tx[7]=0x68;
		p8tx[8]=0x11;
		
		LEN_DATA=4;
		DI=UARTCtrl->RM_DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	#if (USER/100)==15//����
		//����:����������Ҫ֧��06000001��061000FF�ȿ鳭��
		if(DI==0x06000001)
		{
			if(UARTCtrl->ReRMCount>=1)//�����ط�����
			{
				DI=0x060000FF;//ȫ��6�ฺ������(����)
				UARTCtrl->RM_DI=DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
			}
		}
	#endif
		switch(DI)
		{
		#if (USER/100)==15//����
			case 0x060000FF://ȫ��6�ฺ������(����)
		#endif
			case 0x06000001://ȫ��6�ฺ������
			case 0x06010001://1�ฺ������(��ѹ��������Ƶ��)
			case 0x06020001://2�ฺ������(���޹�����)
			case 0x06030001://3�ฺ������(��������)
			case 0x06040001://4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
			case 0x06050001://5�ฺ������(1-4�����޹��ܵ���)
			case 0x06060001://6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
				//����������,������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
				DateSaveTI(PORTn,p8tx+32);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
				p8tx[10+4+0]=1;//��1����
				i=p8tx[37];//��
				i=hex_bcd(i);
				p8tx[10+4+1]=i;
				i=p8tx[36];//ʱ
				i=hex_bcd(i);
				p8tx[10+4+2]=i;
				i=p8tx[35];//��
				i=hex_bcd(i);
				p8tx[10+4+3]=i;
				i=p8tx[34];//��
				i=hex_bcd(i);
				p8tx[10+4+4]=i;
				i=(p8tx[32]<<8)+p8tx[33];//��
				i-=2000;
				i=hex_bcd(i);
				p8tx[10+4+5]=i;
				LEN_DATA+=6;
				break;
			default:
				DI=RM_TxDATA_DL2007_DI(PORTn,DI);//������֡DI����n�θ���
				break;
		}
		p8tx[9]=LEN_DATA;
		for(i=0;i<4;i++)
		{
			p8tx[10+i]=DI;
			DI>>=8;
		}
		//��0x33
		for(i=0;i<LEN_DATA;i++)
		{
			p8tx[10+i]+=0x33;
		}
		//cs
		p8tx[10+LEN_DATA]=0;
		for(i=0;i<(10+LEN_DATA);i++)
		{
			p8tx[10+LEN_DATA]+=p8tx[i];
		}
		p8tx[10+LEN_DATA+1]=0x16;
		return 10+LEN_DATA+2;
	}
	else
	{//����֡
		p8tx[8]=0x12;
		p8tx[9]=5;
		p8tx[14]=UARTCtrl->RM_SubsequentFrames+0x33;
		p8tx[15]=0;//cs
		for(i=0;i<15;i++)
		{
			p8tx[15]+=p8tx[i];
		}
		p8tx[16]=0x16;
		return 17;//�����ֽ���
	}
}

//645-07�㲥Уʱ
//68 99 99 99 99 99 99 68 08 06 84 55 43 38 39 4A 4B 16  
//645�㲥����ַУʱ
//��ַΪ 11 11 11 11 11 11
//68 11 11 11 11 11 11 68 08 06 58 73 43 38 39 4A 0D 16 

u32 BroadcastTime_TxDATA_DL645(u8 *p8tx)//DL645�㲥Уʱ����֡;����:��֡���ܳ����ֽ���
{
	u32 i;
	
	p8tx[0]=0x68;
	p8tx[1]=0x99;
	p8tx[2]=0x99;
	p8tx[3]=0x99;
	p8tx[4]=0x99;
	p8tx[5]=0x99;
	p8tx[6]=0x99;
	p8tx[7]=0x68;
	p8tx[8]=0x08;
	p8tx[9]=0x06;
	MR((u32)p8tx+10,ADDR_MYMDHMS,6);
	//��0x33
	for(i=0;i<6;i++)
	{
		p8tx[10+i]+=0x33;
	}
	//cs
	p8tx[10+6]=0;
	for(i=0;i<(10+6);i++)
	{
		p8tx[10+6]+=p8tx[i];
	}
	p8tx[10+6+1]=0x16;
	return 10+6+2;
}




