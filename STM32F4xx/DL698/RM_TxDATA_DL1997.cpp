
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"


#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"




u32 RM_TxDATA_DL1997(u32 PORTn,u8 *p8CSD,u8 *p8tx)//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
	u32 i;
	u32 DI;
	u32 LEN_DATA;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx[0]=0x68;
	p8tx[1]=UARTCtrl->RMAddr[2]>>8;
	p8tx[2]=UARTCtrl->RMAddr[2];
	p8tx[3]=UARTCtrl->RMAddr[1]>>8;
	p8tx[4]=UARTCtrl->RMAddr[1];
	p8tx[5]=UARTCtrl->RMAddr[0]>>8;
	p8tx[6]=UARTCtrl->RMAddr[0];
	p8tx[7]=0x68;
	p8tx[8]=0x01;
	LEN_DATA=2;
	DI=UARTCtrl->RM_DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	i=UARTCtrl->TaskID;
	if(((i>>24)&0xf)==1)
	{//��ͨ�ɼ�����
		i>>=20;
		i&=0xf;
		switch(i)//�ɼ�����
		{
			case 0://0=�ɼ���ǰ����
				break;
			case 1://1=�ɼ��ϵ�N��
				break;
			case 2://2=������ʱ��ɼ�
				break;
			case 3://3=��ʱ�����ɼ�
				//����������
				break;
			case 4://RetryMetering ����
				DI=0xffff;//97��������,������뷢��ʱ�������DI
				break;
		}
	}
	p8tx[9]=LEN_DATA;
	for(i=0;i<2;i++)
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

















