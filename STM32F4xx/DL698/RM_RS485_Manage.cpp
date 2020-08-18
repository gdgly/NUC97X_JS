

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RS485_Manage.h"
#include "../DL698/RM_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../DL698/MeterAutoReport.h"



void RM_RS485_Manage(u32 PORTn)//RS485抄表管理
{
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(PORTn!=ACSAMPLEPORT)
	{//不是交采
	//广播校时任务
		if((UARTCtrl->BroadcastTimeTask&0x7f)>=2)//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
		{
			BroadcastTime_RS485(PORTn);//广播校时-rs485;完成后置UARTCtrl->BroadcastTimeTask=0
			return;
		}
	//自动搜表任务
		if(UARTCtrl->AutoSearchTask>=2)//自动搜表任务:0=空,1=等待,2...过程
		{
			AutoSearchMeter_RS485(PORTn);//自动搜表任务-rs485:0=空,1=等待,2...过程
			return;
		}
	//电能表主动上报任务
/*
//测试
if(PORTn==RS485_2PORT)
{
	if(UARTCtrl->AutoReportTask<2)
	{
		UARTCtrl->AutoReportTask=2;
		UARTCtrl->AutoReportRMprotocol=3;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
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
		
		if(UARTCtrl->AutoReportTask>=2)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
		{
			MeterAutoReport_RS485(PORTn);//电能表主动上报任务-rs485:0=空,1=有主动上报等待读取,2...抄读中
			return;
		}
	}
	
	RM_RS485(PORTn);//RS485抄表
}

















