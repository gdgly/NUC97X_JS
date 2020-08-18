

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RS485_Manage.h"
#include "../DL698/RM_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../DL698/MeterAutoReport.h"



void RM_RS485_Manage(u32 PORTn)//RS485�������
{
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(PORTn!=ACSAMPLEPORT)
	{//���ǽ���
	//�㲥Уʱ����
		if((UARTCtrl->BroadcastTimeTask&0x7f)>=2)//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
		{
			BroadcastTime_RS485(PORTn);//�㲥Уʱ-rs485;��ɺ���UARTCtrl->BroadcastTimeTask=0
			return;
		}
	//�Զ��ѱ�����
		if(UARTCtrl->AutoSearchTask>=2)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
		{
			AutoSearchMeter_RS485(PORTn);//�Զ��ѱ�����-rs485:0=��,1=�ȴ�,2...����
			return;
		}
	//���ܱ������ϱ�����
/*
//����
if(PORTn==RS485_2PORT)
{
	if(UARTCtrl->AutoReportTask<2)
	{
		UARTCtrl->AutoReportTask=2;
		UARTCtrl->AutoReportRMprotocol=3;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
		UARTCtrl->BpsCtrl=(0xb+(6<<5));//9600
		UARTCtrl->AutoReportRMAddr[0]=0x11;
		UARTCtrl->AutoReportRMAddr[1]=0x11;
		UARTCtrl->AutoReportRMAddr[2]=0x11;
		UARTCtrl->AutoReportRMAddr[3]=0x11;
		UARTCtrl->AutoReportRMAddr[4]=0x11;
		UARTCtrl->AutoReportRMAddr[5]=0x11;

	}
}
*/
		
		if(UARTCtrl->AutoReportTask>=2)//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
		{
			MeterAutoReport_RS485(PORTn);//���ܱ������ϱ�����-rs485:0=��,1=�������ϱ��ȴ���ȡ,2...������
			return;
		}
	}
	
	RM_RS485(PORTn);//RS485����
}

















