
//��·�ӿڼ��
#include "Project.h"
#include "Terminal_Uart_AFN02_Heartbeat.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"



const u8 Heartberat_Example[]=
{
	0x68,
	12,0,
	12,0,
	0x68,
	0xc9,//������(DIR=1����,������־λPRM=1,������9=����M��Ӧ֡	��·����)
	0,0,//����������
	0,0,//�ն˵�ַ
	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	2,//AFN=2
	0x70,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ
	0,0,//Pn=0
	0x04,0,//Fn=1����,2�˳�,3����
	0,//cs
	0x16,
};


void Heartberat(u32 PORTn,u32 ADDR_HeartberatTimer)//��������(��·�ӿڼ��)
{
	u32 i;
	u8 * p8txbuff;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 UARTxTimerAddr;
	
	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
		{
			return;
		}
	}
	UARTxTimerAddr=Get_ADDR_UARTnTimer(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	p16=(u16 *)(ADDR_HeartberatTimer);//������������붨ʱ��
	if(p16[0]==0x0)
	{
		if(UARTCtrl->Task==0x0)
		{//����
			p16=(u16 *)(UARTxTimerAddr);//UART��ʱ��MS
			if(p16[0]==0x0)
			{
				if((UARTCtrl->LinkTask>=100)&&(UARTCtrl->LinkTask<=102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{//����
						if((UARTCtrl->TCPUDP&0x30)==0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
						{//������ģʽ,���趨(��������+60��)��û�յ���ȷ����֡Ϊ����
							if(PORTn==GPRSPORT)
							{
								i=Terminal_Ram->GPRSHeartBeat_S_Timer;//GPRS�����붨ʱ��
							}
							if(PORTn==ENETPORT)
							{
								i=Terminal_Ram->ENETHeartBeat_S_Timer;//��̫�������붨ʱ��
							}
							if(i==0)
							{
								UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							
							if(UARTCtrl->LinkTask==102)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							{
								UARTCtrl->LinkTask=103;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							return;
						}
						
						if(UARTCtrl->HostACK>7)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
						{//ֵ����
							UARTCtrl->HostACK=0;
						}
						if(UARTCtrl->HostACK>=4)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
						{//�����������ϱ��ȴ�ȷ�Ϸ���
							return;
						}
						UARTCtrl->HeartberatCount+=1;//������������,�յ�ȷ��֡��0
						if(UARTCtrl->HeartberatCount>3)
						{//����3��û�յ�ȷ��֡
							UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
							if(UARTCtrl->LinkTask==102)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							{
								UARTCtrl->LinkTask=103;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							else
							{
								UARTCtrl->LinkTask=0;//���³�ʼ��
								if(PORTn==GPRSPORT)
								{
									WarningOccur(PORTn,(u8 *)"Զ��������վ����Ӧ");//�澯����,��ڸ澯����͸澯�ַ���
								}
								if(PORTn==ENETPORT)
								{
									WarningOccur(PORTn,(u8 *)"��̫����վ����Ӧ");//�澯����,��ڸ澯����͸澯�ַ���
								}
							}
						}
						else
						{
							UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
							UARTCtrl->TxByte=20;//�跢���ֽ���
							p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
							MR((u32)p8txbuff,(u32)Heartberat_Example,18);
							MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
							p8txbuff[13]|=(UARTCtrl->TxSEQ&0xf);//����֡���PSEQ
							UARTCtrl->TxSEQ+=1;//����֡���PSEQ+1
							UARTCtrl->HostACK=3;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
							switch(UARTCtrl->LinkTask)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							{
								default://����
									MR(((u32)p8txbuff)+18,ADDR_AFN0CF2,6);//�ն�ʱ��
									p8txbuff[1]=12+6;
									UARTCtrl->TxByte=20+6;//�跢���ֽ���
									break;
								case 100://����
									p8txbuff[16]=1;//����
									UARTCtrl->HostACK=1;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
									break;
								case 102://�˳�
									p8txbuff[16]=2;//�˳�
									UARTCtrl->HostACK=2;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
									UARTCtrl->HeartberatCount=100;//�˳�ʱ��������������Ϊ100,�����ض�ʱ��ûȷ�ϻش����������³�ʼ���ر�ģ������
									break;
							}
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							p16=(u16 *)(ADDR_HeartberatTimer);//������������붨ʱ��
							i=MRR(ADDR_AFN04F1+2,2);
							i&=0xfff;
							if(i<WaitHeartberatACKMinTimer)
							{
								i=WaitHeartberatACKMinTimer;//���������ȴ���վȷ����С��ʱ��
							}
							if(i>WaitHeartberatACKMaxTimer)
							{
								i=WaitHeartberatACKMaxTimer;//���������ȴ���վȷ�����ʱ��
							}
							p16[0]=i;//���������ض�ʱ��
						}

				}
			}
		}
	}
}


void Uart_AFN02(u32 PORTn)//��·�ӿڼ��
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+11;//��Ӧ������ 11
	if((p8txbuff[16]==0x01)||(p8txbuff[16]==0x04))//Fn=1����,2�˳�,3����
	{//���������
		if(UARTCtrl->LinkTask==100)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
		{
			UARTCtrl->LinkTask=101;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
		}
	}
	if(p8txbuff[16]==0x02)//Fn=1����,2�˳�,3����
	{//�˳�
		UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	}
	Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,���ݵ�Ԫ��ʶ
	i=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}






