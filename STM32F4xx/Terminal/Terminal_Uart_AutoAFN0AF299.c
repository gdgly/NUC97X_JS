
#include "Project.h"
#include "Terminal_Uart_AutoAFN0AF299.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"


#if ((USER/100)==8 || (USER/100)==7)//ͨ���г�ϵ���㽭��

const u8 AutoAFN0AF299_Example[]=
{//�ն�������վʱ��
	0x68,
	40,0,
	40,0,
	0x68,
	0xC0,//������(DIR=1����,������־λPRM=1,��������4=���ͨM�޻ش�,����Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1)10=����M��Ӧ)
	0x71,0x05,//����������
	0x05,0x00,//�ն˵�ַ
	0x02,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	0x0A,//AFN0A
	0x60,//SEQ
	0x00,0x00,//Pn=0
	0x04,0x25,//Fn=299
	0x01,//ͨ������:1GPRS 2CDMA 3Ethernet 4phone_line 5PLC
	0x00,0x00,0x22,0x08,0x10,0x14,0x10,0x15,0x20,//�ն˲�ѯʱ��T1 2015��10��14��10:08:22��0000ms
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,//��վ����ʱ��T2 ������ʱ�������
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,//��վ��Ӧʱ��T3 ������ʱ�������
	0x20,//CS
	0x16,
};

void Terminal_AutoAFN0AF299(u32 PORTn)//�������㽭��չAFN0AF299�ն�������վʱ��
{
	u32 i;
	u16 * p16;
	u16 * p16ACKTimer;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
		{
			return;
		}
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	if(UARTCtrl->Task==0x0)
	{//����
		p16=(u16 *)Get_ADDR_UARTnTimer(PORTn);
		if(p16[0]==0x0)
		{
			if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{//����
				if(PORTn==GPRSPORT)
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
				}
				if(PORTn==ENETPORT)
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
				}
				if(UARTCtrl->HostACK!=0x0)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=������վʱ��
				{
					if(UARTCtrl->HostACK!=7)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=������վʱ��
					{//�����������ϱ��ȴ�ȷ�Ϸ���
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//�ȴ���վȷ���붨ʱ��!=0
					}
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=������վʱ��
					if(UARTCtrl->HostCount>2)
					{//�ط�������
						UARTCtrl->HostCount=0;
						if(Terminal_Ram->PowerTask==0)//ͣ���¼�����
						{
							Terminal_Ram->RMDelay_S_Timer=10;
							Terminal_Ram->PowerTask=1;//ͣ���¼�����
						}
						else
						{//������վ3��δ��Ӧ�򱾴ξ�ȷ��ʱֹͣ
							MWR(0,ADDR_AFN04F299_Ext+28,1);
						}
						return;
					}
				}
				//����
				UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
				UARTCtrl->TxByte=48;//�跢���ֽ���
				p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
				MR((u32)p8txbuff,(u32)AutoAFN0AF299_Example,48);
				if(PORTn==GPRSPORT)
				{
					if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
					{//GSM����֧��GPRS
						p8txbuff[18]=1;
					}
					else
					{//CDMA
						p8txbuff[18]=2;
					}
				}
				if(PORTn==ENETPORT)
				{
					p8txbuff[18]=3;
				}
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
				MR(((u32)p8txbuff)+21,ADDR_TYMDHMS,6);//�ն�ʱ��
				//�·�ȥ������
				p8txbuff[25]&=0x1f;

				if(UARTCtrl->HostCount==0)
				{//�����ظ�֡
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����ʱ��)
					UARTCtrl->TxSEQ+=1;//����֡���PSEQ+1
				}
				else
				{//���ظ�֡
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
				}
				UARTCtrl->HostACK=7;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=������վʱ��
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				UARTCtrl->HostCount++;

				i=MRR(ADDR_AFN04F1+2,2);//�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
				i&=0xfff;
				if(i<WaitHostACKMinTimer)
				{
					i=WaitHostACKMinTimer;//�����ϱ��ȴ���վȷ����С��ʱ��
				}
				if(i>WaitHostACKMaxTimer)
				{
					i=WaitHostACKMaxTimer;//�����ϱ��ȴ���վȷ�����ʱ��
				}
				p16ACKTimer[0]=i;//�����ϱ��ȴ���վȷ����
			}
		}
	}
}

#endif
