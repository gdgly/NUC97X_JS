
//�ն˼���
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_Uart_Cascading.h"
#include "Terminal_Uart_AutoEvent.h"
#include "Terminal_Uart_TimerTask.h"





void Uart_AFN08(u32 PORTn)//���󱻼����ն������ϱ�
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	
	i=MRR(ADDR_AFN04F37,1);//�ն˼���ͨ�Ų���
	if(i==0x0)
	{//û������
err:
		//ȫ������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		i=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
		Uart_376_CS(Get_ADDR_UARTnTx(PORTn));//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	}
	else
	{//�м�����
		i=MRR(ADDR_AFN04F37+6,1);//�ն˼���ͨ�Ų���
		if((i&0x80)==0x0)
		{//������������
			Terminal_Ram->Cascading_LoopChack_S_Timer=0;//����Ѳ�������붨ʱ��
		}
		else
		{//�Ǳ���������
			//�����ϱ���Ҫ�¼�
			Terminal_AutoEvent(PORTn);//�����ϱ���Ҫ�¼�
			if(UARTCtrl->Task==2)
			{
				if(UARTCtrl->TxByte!=0)//�����ֽ���(���ֶ���)
				{
					return;
				}
				else
				{
					UARTCtrl->Task=0;
				}
			}
			//��ʱ����
			for(i=0;i<(2*MaxTimerTask);i++)
			{
				Terminal_TimerTask(PORTn);//��ʱ����
				if(UARTCtrl->Task==2)
				{
					if(UARTCtrl->TxByte!=0)//�����ֽ���(���ֶ���)
					{
						return;
					}
					else
					{
						UARTCtrl->Task=0;
					}
				}
			}
			goto err;
		}
	}
	
	
}


















