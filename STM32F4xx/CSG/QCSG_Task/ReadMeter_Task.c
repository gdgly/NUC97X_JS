/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:ReadMeter_Task.c
��  ��:�����
��  ��:1.00
��  ��:20170601
��  ��:���������ļ�
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../MS/MS.h"
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/ReadMeter_Task.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
#include "../Hcsg/ERAMCSG.h"
#include "../Hcsg/ERAM_For_Mxn.h"
#include "../Hcsg/IRAM_TerminalSaveCSG.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "Terminal_Task.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


#if ((Project/100)==3) || ((Project/100)==9)	//�����ն�
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define JC_IN								1000
#define JC_UN								2200
#define RD_METER_TDC_SEM                    0x01
#define RD_METER_TDD_SEM                    0x02
#define RD_METER_TDM_SEM                    0x04
#define RD_METER_EVENT_SEM                  0x08
#define RD_RELAY_TASK_SEM                   0x10
#define RD_COMMON_TASK_SEM                  0x20
#define ALARM_START							0x01
#define ALARM_END							0x02
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/* */
/*
 * TODO: Ŀǰ97Э����ܱ���֧�ָ澯���ݲ���
 */
 /**--------------------------------------------------------------------
*��    ��:���²����㲨����
*�������:u8PortBaudRate
*�������:pu32BpsCtrl
*��    ��:��
--------------------------------------------------------------------**/
void RD_GetPn_Bps(u8 u8PortBaudRate,u8 u8PortParity,u8 u8PortDataBit,u8 u8PortStopBit,u32* pu32BpsCtrl)
{
	u32 u32Value = 0;
	
	(*pu32BpsCtrl) = 0;
	if((u8PortDataBit > 8) || (u8PortDataBit < 5))
	{
		u8PortDataBit = 8;
	}
	if(u8PortParity > 2)
	{
		u8PortParity = 1;
	}
	if(u8PortStopBit >= 3)
	{
		u8PortStopBit = 0;
	}
	if((u8PortBaudRate != 4) && (u8PortBaudRate != 8) && (u8PortBaudRate != 16) && (u8PortBaudRate != 32))
	{
		u8PortBaudRate = 8;
	}
	if(u8PortDataBit == 5)
	{
		//5λ����λ
		u32Value = 0;
	}
	if(u8PortDataBit == 6)
	{
		//6λ����λ
		u32Value = 1;
	}
	if(u8PortDataBit == 7)
	{
		//7λ����λ
		u32Value = 2;
	}
	if(u8PortDataBit == 8)
	{
		//8λ����λ
		u32Value = 3;
	}
	(*pu32BpsCtrl) = (*pu32BpsCtrl) | u32Value;
	u32Value = 0;
	if(u8PortParity == 1)
	{
		//żУ��
		u32Value = 0;
	}
	if(u8PortParity == 2)
	{
		//��У��
		u32Value = 1;
	}
	u32Value = u32Value << 2;
	(*pu32BpsCtrl) = (*pu32BpsCtrl) | u32Value;
	u32Value = 0;
	if(u8PortParity == 0)
	{
		u32Value = 0;
	}
	else
	{
		u32Value = 1;
	}
	u32Value = u32Value << 3;
	(*pu32BpsCtrl) = (*pu32BpsCtrl) | u32Value;
	u32Value = 0;
	if((u8PortStopBit == 0) || (u8PortStopBit == 1))
	{
		//1λֹͣλ
		u32Value = 0;
	}
	if(u8PortStopBit == 2)
	{
		//2λֹͣλ
		u32Value = 1;
	}
	u32Value = u32Value << 4;
	(*pu32BpsCtrl) = (*pu32BpsCtrl) | u32Value;
	u32Value = 0;
	if(u8PortBaudRate <= 4)
	{
		//1200
		u32Value = 2;
	}
	if((u8PortBaudRate > 4) && (u8PortBaudRate <= 8))
	{
		//2400
		u32Value = 3;				
	}
	if((u8PortBaudRate > 8) && (u8PortBaudRate <= 16))
	{
		//4800
		u32Value = 4;				
	}
	if(u8PortBaudRate > 16)
	{
		//9600
		u32Value = 6;				
	}
	u32Value = u32Value << 5;
	(*pu32BpsCtrl) = (*pu32BpsCtrl) | u32Value;
}
 /**--------------------------------------------------------------------
*��    ��:ʱ��ṹ��ת��
*�������:pstTime��ʱ��
*�������:pstARD02Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Time_TO_YYTime(TIME_PARAM_S* pstTime,RealTimeYYStruct* pstYYTime)
{
	pstYYTime->YearH = 0x20;
	pstYYTime->YearL = pstTime->u8Year;
	pstYYTime->Month = pstTime->u8Month;
	pstYYTime->Day = pstTime->u8Day;
	pstYYTime->Hour = pstTime->u8Hour;
	pstYYTime->Minute = pstTime->u8Minute;
}
 /**--------------------------------------------------------------------
*��    ��:������һ��ִ��ʱ���������Ҫ����������һ��ִ��ʱ�䲻�Ϸ������
*�������:pstRefTime,u8PeriodValue��u8PeriodUnit
*�������:pstLastTime
*��    ��:��
--------------------------------------------------------------------**/
void RD_Task_LastTimeCalc(TIME_PARAM_S* pstLastTime,TIME_PARAM_S* pstCurrTime,TIME_PARAM_S* pstRefTime,u8 u8PeriodValue,u8 u8PeriodUnit)
{
//	u8 i = 0;
	u32 au32Result[4];
	u32 u32PeriodMin = 0,u32IntervalMin = 0,u32IntervalNum = 0,u32IntervalSec = 0;
	
	if(QCSG_TimerSub(pstCurrTime,pstRefTime,au32Result) <= 0)
    {
        //��ǰʱ��С�ڻ���ڲο�ʱ�䣬��һ���ϱ�ʱ�䲻���¸�ֵ
        return;
    }
	if((au32Result[0] == 0)&&(au32Result[1] == 0)&&(au32Result[2] == 0)&&(au32Result[3] == 0))
	{
		return;
	}
    if(u8PeriodUnit == 3)
	{//err20190528 �������/�м̶�������ȫF����
		//��λΪ��
//		for(i = 0;i < u8PeriodValue;i++)
//		{
//			QCSG_IncOneMonth(pstRefTime);
//			if(QCSG_TimerSub(pstCurrTime,pstRefTime,au32Result) <= 0)
//            {
//                //ʱ��ӹ�
//                break;
//            }
//		}
//		QCSG_DecOneMonth(pstRefTime);
//		MemCopy(pstLastTime,pstRefTime,sizeof(TIME_PARAM_S));
		MemCopy(pstLastTime,pstCurrTime,sizeof(TIME_PARAM_S));	

	}
    if(u8PeriodUnit == 2)
	{//err20190528 �������/�м̶�������ȫF����
		//��λΪ��
		//err20190523 ��/���������ݴ��������
//		for(i = 0;i < u8PeriodValue;i++)
//		{
//			QCSG_IncOneDay(pstRefTime);
//			if(QCSG_TimerSub(pstCurrTime,pstRefTime,au32Result) <= 0)
//            {
//                //ʱ��ӹ�
//                break;
//            }
//		}
//		QCSG_DecOneDay(pstRefTime);
//		MemCopy(pstLastTime,pstRefTime,sizeof(TIME_PARAM_S));	
		MemCopy(pstLastTime,pstCurrTime,sizeof(TIME_PARAM_S));	

	}
	if(u8PeriodUnit == 1)
	{
		//��λΪСʱ
		u32PeriodMin = u8PeriodValue * 60;
		u32IntervalMin = au32Result[0] * 24 * 60 + au32Result[1] * 60 + au32Result[2];
		u32IntervalNum = u32IntervalMin	/ u32PeriodMin;
		u32IntervalSec = u32IntervalNum * u32PeriodMin * 60;
		QCSG_TimerAdd(pstRefTime,u32IntervalSec,pstLastTime);
	}
	if(u8PeriodUnit == 0)
	{
		//��λΪ����
		u32PeriodMin = u8PeriodValue;
		u32IntervalMin = au32Result[0] * 24 * 60 + au32Result[1] * 60 + au32Result[2];
		u32IntervalNum = u32IntervalMin	/ u32PeriodMin;
		u32IntervalSec = u32IntervalNum * u32PeriodMin * 60;
		QCSG_TimerAdd(pstRefTime,u32IntervalSec,pstLastTime);		
	}
}
/**--------------------------------------------------------------------
*��    ��:ARD1�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD02Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD01_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,ARD01Struct* pstARD01Data)
{
	pstARD01Data->State = u8AlarmFlag;
	pstARD01Data->ReportState = 0;
	MemCopy(&pstARD01Data->Time,pstTime,sizeof(TIME_PARAM_S));
}
/**--------------------------------------------------------------------
*��    ��:ARD2�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD02Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD02_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,QCSG_RAM_DATA_S* pstRam_Data,ARD02Struct* pstARD02Data)
{
	MemSet(pstARD02Data,0xFF,sizeof(ARD02Struct));//err20180911 ARD02�ṹ��ǰ��ȫF����ֹ��Чλ�ó��ִ�������
	pstARD02Data->State = u8AlarmFlag;
	pstARD02Data->ReportState = 0;
	MemCopy(&pstARD02Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD02Data->C_00010000[0],pstRam_Data->u8Data00010000,4);
	MemCopy(&pstARD02Data->C_00020000[0],pstRam_Data->u8Data00020000,4);
	MemCopy(&pstARD02Data->C_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD02Data->C_00040000[0],pstRam_Data->u8Data00040000,4);	
	
	MemCopy(&pstARD02Data->C_0201FF00[0],&pstRam_Data->u8Data0201FF00,6);		
	MemCopy(&pstARD02Data->C_0202FF00[0],&pstRam_Data->u8Data0202FF00,9);	
	MemCopy(&pstARD02Data->C_0203FF00[0],&pstRam_Data->u8Data0203FF00,12);
	MemCopy(&pstARD02Data->C_0204FF00[0],&pstRam_Data->u8Data0204FF00,12);
	MemCopy(&pstARD02Data->C_0206FF00[0],&pstRam_Data->u8Data0206FF00,8);
}
/**--------------------------------------------------------------------
*��    ��:ARD3�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD02Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD03_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,u32* u32LastValue,u32* u32CurrValue,QCSG_RAM_DATA_S* pstRam_Data,ARD03Struct* pstARD03Data)
{

	MemSet(pstARD03Data,0xFF,sizeof(ARD03Struct));//err20180911 ARD03�ṹ��ǰ��ȫF����ֹ��Чλ�ó��ִ������ݡ�
	pstARD03Data->State = u8AlarmFlag;
	pstARD03Data->ReportState = 0;
	MemCopy(&pstARD03Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD03Data->B_00010000[0],&u32LastValue[0],4);
	MemCopy(&pstARD03Data->B_00020000[0],&u32LastValue[1],4);
	MemCopy(&pstARD03Data->B_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD03Data->B_00040000[0],pstRam_Data->u8Data00040000,4);
	MemCopy(&pstARD03Data->B_0201FF00[0],pstRam_Data->u8Data0201FF00,6);
	MemCopy(&pstARD03Data->B_0202FF00[0],pstRam_Data->u8Data0202FF00,9);	
	MemCopy(&pstARD03Data->B_0203FF00[0],pstRam_Data->u8Data0203FF00,12);
	MemCopy(&pstARD03Data->B_0204FF00[0],pstRam_Data->u8Data0204FF00,12);	
	
	MemCopy(&pstARD03Data->A_00010000[0],&u32CurrValue[0],4);
	MemCopy(&pstARD03Data->A_00020000[0],&u32CurrValue[1],4);
	MemCopy(&pstARD03Data->A_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD03Data->A_00040000[0],pstRam_Data->u8Data00040000,4);
	MemCopy(&pstARD03Data->A_0201FF00[0],pstRam_Data->u8Data0201FF00,6);
	MemCopy(&pstARD03Data->A_0202FF00[0],pstRam_Data->u8Data0202FF00,9);	
	MemCopy(&pstARD03Data->A_0203FF00[0],pstRam_Data->u8Data0203FF00,12);
	MemCopy(&pstARD03Data->A_0204FF00[0],pstRam_Data->u8Data0204FF00,12);		
}
/**--------------------------------------------------------------------
*��    ��:ARD4�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD04_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,u32* pu32Value,QCSG_RAM_DATA_S* pstRam_Data,ARD04Struct* pstARD04Data)
{
	u8 temp[4];
	pstARD04Data->State = u8AlarmFlag;
	pstARD04Data->ReportState = 0;
	
	MemCopy(&pstARD04Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD04Data->C_00010000[0],pstRam_Data->u8Data00010000,4);
	MemCopy(&pstARD04Data->C_00020000[0],pstRam_Data->u8Data00020000,4);
	MemCopy(&pstARD04Data->C_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD04Data->C_00040000[0],pstRam_Data->u8Data00040000,4);
	
	//MemCopy(&pstARD04Data->C_E1000008[0],pu32Value,sizeof(pstARD04Data->C_E1000008));//err201080911 ���ʣ����
	MemCopy(temp,pu32Value,sizeof(pstARD04Data->C_E1000008));
	pstARD04Data->C_E1000008[0]=temp[1];
	pstARD04Data->C_E1000008[1]=temp[2];
	pstARD04Data->C_E1000008[2]=temp[3];
	pstARD04Data->C_E1000008[3]=0;
	
}
/**--------------------------------------------------------------------
*��    ��:ARD5�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD05_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,u16* pu16Value,QCSG_RAM_DATA_S* pstRam_Data,ARD05Struct* pstARD05Data)
{
	pstARD05Data->State = u8AlarmFlag;
	pstARD05Data->ReportState = 0;
	MemCopy(&pstARD05Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD05Data->B_00010000[0],pstRam_Data->u8Data00010000,4);
	MemCopy(&pstARD05Data->B_00020000[0],pstRam_Data->u8Data00020000,4);
	MemCopy(&pstARD05Data->B_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD05Data->B_00040000[0],pstRam_Data->u8Data00040000,4);
	MemCopy(&pstARD05Data->B_E080000C,(u8*)&pu16Value[0],2);
	MemCopy(&pstARD05Data->B_E080000D,(u8*)&pu16Value[1],2);
	MemCopy(&pstARD05Data->A_E080000C,(u8*)&pu16Value[2],2);
	MemCopy(&pstARD05Data->A_E080000D,(u8*)&pu16Value[3],2);
}
/**--------------------------------------------------------------------
*��    ��:ARD6�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD06_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,u32* pu32Value,QCSG_RAM_DATA_S* pstRam_Data,ARD06Struct* pstARD06Data)
{
	pstARD06Data->State = u8AlarmFlag;
	pstARD06Data->ReportState = 0;
	MemCopy(&pstARD06Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD06Data->B_00010000[0],pstRam_Data->u8Data00010000,4);
	MemCopy(&pstARD06Data->B_00020000[0],pstRam_Data->u8Data00020000,4);
	MemCopy(&pstARD06Data->B_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD06Data->B_00040000[0],pstRam_Data->u8Data00040000,4);
	MemCopy(&pstARD06Data->B_E1000003,&pu32Value[0],3);
	MemCopy(&pstARD06Data->B_E1000004,&pu32Value[1],3);
	MemCopy(&pstARD06Data->A_E1000003,&pu32Value[2],3);
	MemCopy(&pstARD06Data->A_E1000004,&pu32Value[3],3);
}
/**--------------------------------------------------------------------
*��    ��:ARD7�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD07_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,QCSG_RAM_DATA_S* pstRam_Data,ARD07Struct* pstARD07Data)
{
	pstARD07Data->State = u8AlarmFlag;
	pstARD07Data->ReportState = 0;
	MemCopy(&pstARD07Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD07Data->B_00010000[0],pstRam_Data->u8Data00010000,4);
	MemCopy(&pstARD07Data->B_00020000[0],pstRam_Data->u8Data00020000,4);
	MemCopy(&pstARD07Data->B_00030000[0],pstRam_Data->u8Data00030000,4);
	MemCopy(&pstARD07Data->B_00040000[0],pstRam_Data->u8Data00040000,4);
	MemCopy(&pstARD07Data->B_01010000[0],pstRam_Data->u8Data01010000,8);	
	MemCopy(&pstARD07Data->B_01020000[0],pstRam_Data->u8Data01020000,8);	
	MemCopy(&pstARD07Data->B_01030000[0],pstRam_Data->u8Data01030000,8);
	MemCopy(&pstARD07Data->B_01040000[0],pstRam_Data->u8Data01040000,8);
}
/**--------------------------------------------------------------------
*��    ��:ARD7�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD11_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime, u8* pu8Data03300X00, u32* pu32Value, QCSG_RAM_DATA_S* pstRam_Data,ARD11Struct* pstARD11Data)
{
	pstARD11Data->State = u8AlarmFlag;
	pstARD11Data->ReportState = 0;
	MemCopy(&pstARD11Data->Time,pstTime,sizeof(TIME_PARAM_S));
	
	MemCopy(&pstARD11Data->C_03300X00[0],pu8Data03300X00,3);//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
	
	//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
	MemCopy(&pstARD11Data->B_00010000[0],&pu32Value[0],4);
	MemCopy(&pstARD11Data->B_00020000[0],&pu32Value[1],4);
	MemCopy(&pstARD11Data->B_00050000[0],&pu32Value[2],4);	
	MemCopy(&pstARD11Data->B_00060000[0],&pu32Value[3],4);	
	MemCopy(&pstARD11Data->B_00070000[0],&pu32Value[4],4);
	MemCopy(&pstARD11Data->B_00080000[0],&pu32Value[5],4);

	//err20180925 ���ӷ���������
	MemCopy(&pstARD11Data->A_00010000[0],pstRam_Data->u8Data00010000,4);
	MemCopy(&pstARD11Data->A_00020000[0],pstRam_Data->u8Data00020000,4);
	MemCopy(&pstARD11Data->A_00050000[0],pstRam_Data->u8Data00050000,4);	
	MemCopy(&pstARD11Data->A_00060000[0],pstRam_Data->u8Data00060000,4);	
	MemCopy(&pstARD11Data->A_00070000[0],pstRam_Data->u8Data00070000,4);
	MemCopy(&pstARD11Data->A_00080000[0],pstRam_Data->u8Data00080000,4);
}
/**--------------------------------------------------------------------
*��    ��:ARD13�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD13_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,u8* pu8Data04000101,u8* pu8Data04000102,ARD13Struct* pstARD13Data)
{
	pstARD13Data->State = u8AlarmFlag;
	pstARD13Data->ReportState = 0;
	MemCopy(&pstARD13Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD13Data->C_04000101,pu8Data04000101,4);
	MemCopy(&pstARD13Data->C_04000102,pu8Data04000102,3);
	MemCopy(&pstARD13Data->C_E0000130,pstTime,6);
}
/**--------------------------------------------------------------------
*��    ��:ARD14�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD04Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD14_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,ARD14Struct* pstARD14Data)
{
	MemSet(pstARD14Data,0xFF,sizeof(ARD14Struct));
	MemCopy(&pstARD14Data->Time,pstTime,sizeof(TIME_PARAM_S));
	pstARD14Data->State = u8AlarmFlag;
	pstARD14Data->ReportState = 0;
}
/**--------------------------------------------------------------------
*��    ��:ARD18�������
*�������:u8AlarmFlag���¼�������ʶ��pstTime��ʱ�䣬pstRam_Data��RAM�е�����
*�������:pstARD02Data�����ARD2�ṹ
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ARD18_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,u8* pu8Data03300000,ARD18Struct* pstARD18Data)
{
	pstARD18Data->State = u8AlarmFlag;
	pstARD18Data->ReportState = 0;
	MemCopy(&pstARD18Data->Time,pstTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstARD18Data->C_03300000[0],pu8Data03300000,3);
}
/**--------------------------------------------------------------------
*��    ��:ERD3�������
*�������:pstEvent_Info
*�������:pstERD3Data
*��    ��:��
--------------------------------------------------------------------**/
void RD_Write_ERD3_Data(QCSG_EVENT_ERD3_INFO_S* pstEvent_Info,ERD03Struct* pstERD3Data)
{
	u32 u32StartData = 0,u32EndData = 0,u32Data = 0;
	
	MemCopy(&pstERD3Data->StartTime,&pstEvent_Info->stStartTime,sizeof(TIME_PARAM_S));
	MemCopy(&pstERD3Data->EndTime,&pstEvent_Info->stEndTime,sizeof(TIME_PARAM_S));
	
	MemCopy(&u32StartData,pstEvent_Info->u8Start00010000,4);
	u32StartData = QCSG_BCD2HEX((u8)(u32StartData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32StartData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32StartData>>8)) * 100 + QCSG_BCD2HEX((u8)u32StartData);
	MemCopy(&u32EndData,pstEvent_Info->u8End00010000,4);
	u32EndData = QCSG_BCD2HEX((u8)(u32EndData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32EndData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32EndData>>8)) * 100 + QCSG_BCD2HEX((u8)u32EndData);
	if(u32EndData >=  u32StartData)
	{
		u32Data = u32EndData - u32StartData;
	}
	else
	{
		u32Data = 0;
	}
	u32Data = hex_bcd(u32Data);
	MemCopy(pstERD3Data->C_00010000,(u8*)&u32Data,4);
	
	MemCopy(&u32StartData,pstEvent_Info->u8Start00020000,4);
	u32StartData = QCSG_BCD2HEX((u8)(u32StartData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32StartData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32StartData>>8)) * 100 + QCSG_BCD2HEX((u8)u32StartData);
	MemCopy(&u32EndData,pstEvent_Info->u8End00020000,4);
	u32EndData = QCSG_BCD2HEX((u8)(u32EndData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32EndData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32EndData>>8)) * 100 + QCSG_BCD2HEX((u8)u32EndData);
	if(u32EndData >=  u32StartData)
	{
		u32Data = u32EndData - u32StartData;
	}
	else
	{
		u32Data = 0;
	}
	u32Data = hex_bcd(u32Data);
	MemCopy(pstERD3Data->C_00020000,(u8*)&u32Data,4);
	
	MemCopy(&u32StartData,pstEvent_Info->u8Start00030000,4);
	u32StartData = QCSG_BCD2HEX((u8)(u32StartData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32StartData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32StartData>>8)) * 100 + QCSG_BCD2HEX((u8)u32StartData);
	MemCopy(&u32EndData,pstEvent_Info->u8End00030000,4);
	u32EndData = QCSG_BCD2HEX((u8)(u32EndData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32EndData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32EndData>>8)) * 100 + QCSG_BCD2HEX((u8)u32EndData);
	if(u32EndData >=  u32StartData)
	{
		u32Data = u32EndData - u32StartData;
	}
	else
	{
		u32Data = 0;
	}
	u32Data = hex_bcd(u32Data);
	MemCopy(pstERD3Data->C_00030000,(u8*)&u32Data,4);

	MemCopy(&u32StartData,pstEvent_Info->u8Start00040000,4);
	u32StartData = QCSG_BCD2HEX((u8)(u32StartData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32StartData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32StartData>>8)) * 100 + QCSG_BCD2HEX((u8)u32StartData);
	MemCopy(&u32EndData,pstEvent_Info->u8End00040000,4);
	u32EndData = QCSG_BCD2HEX((u8)(u32EndData>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32EndData>>16)) * 10000 
				 + QCSG_BCD2HEX((u8)(u32EndData>>8)) * 100 + QCSG_BCD2HEX((u8)u32EndData);
	if(u32EndData >=  u32StartData)
	{
		u32Data = u32EndData - u32StartData;
	}
	else
	{
		u32Data = 0;
	}
	u32Data = hex_bcd(u32Data);
	MemCopy(pstERD3Data->C_00040000,(u8*)&u32Data,4);	
}
/**--------------------------------------------------------------------
*��    ��:��ѹ��ƽ��ȼ��㣬�ԳƷ���������
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��ƽ���
--------------------------------------------------------------------**/
float RD_Vol_UnbalanceCalc(float fUa,float fUb,float fUc,float fPha,float fPhb,float fPhc)
{
	float freal1,freal2,freal3;        
	float fimag1,fimag2,fimag3;
	float fA0 = 0.0,fA1 = 0.0,fA2 = 0.0;
	float fPI = 3.1415926536;
	float fe = 0.0;
	float fe0 = 0.0;

	fPha = fPha * fPI / 180;
	fPhb = fPhb * fPI / 180;
	fPhc = fPhc * fPI / 180;
	freal2  = fUa * QCSG_COS(fPha) + fUb * QCSG_COS(fPhb + fPI * 2/3) + fUc * QCSG_COS(fPhc + fPI * 4/3);
	fimag2  = fUa * QCSG_SIN(fPha) + fUb * QCSG_SIN(fPhb + fPI * 2/3) + fUc * QCSG_SIN(fPhc + fPI * 4/3);
	freal1  = fUa * QCSG_COS(fPha) + fUb * QCSG_COS(fPhb + fPI * 4/3) + fUc * QCSG_COS(fPhc + fPI * 2/3);
	fimag1  = fUa * QCSG_SIN(fPha) + fUb * QCSG_SIN(fPhb + fPI * 4/3) + fUc * QCSG_SIN(fPhc + fPI * 2/3);
	freal3  = fUa * QCSG_COS(fPha) + fUb * QCSG_COS(fPhb) + fUc * QCSG_COS(fPhc);
	fimag3  = fUa * QCSG_SIN(fPha) + fUb * QCSG_SIN(fPhb) + fUc * QCSG_SIN(fPhc);
	fA1 = QCSG_SQRT((freal1 * freal1) + (fimag1 * fimag1)) / 3.0;
	fA2 = QCSG_SQRT((freal2 * freal2) + (fimag2 * fimag2)) / 3.0;
	fA0 = QCSG_SQRT((freal3 * freal3) + (fimag3 * fimag3)) / 3.0;
	
	if(fA1 > 0.00000001)
	{
		if(fA2 < 0.00000001)
		{
			fe = 0.0;
		}
		else
		{
			fe = (fA2 / fA1)*100;
		}
		if(fA0 < 0.00000001)
		{
			fe0 = 0.0;
		}
		else
		{
			fe0 = (fA0 / fA1) * 100;
		}
	}
	else
	{
		fe = 99.9;
		fe0 = 99.9;
	}
	if(fe > 99.9)
	{
		fe = 99.9;
	}
	if(fe0 > 99.9)
	{
		fe0 = 99.9;
	}
	return fe;		
}

/**--------------------------------------------------------------------
*��    ��:�¼���¼��������ѹ������,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000003(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	* TODO:�澯�������ƣ�
	* �˺�������0x14000100��0x04000507���ݱ�ʶ�£�������������仯�����ٸ��ݵ������״̬���������ʶ���ж��¼��Ƿ�����
	* ����澯�Ѿ�������������ݵ��ܱ�����״̬���¼���ʶ�Ƿ���ڣ��жϻָ���
	* ע�⣬�¼�gc_stQCSGAlarm_DLT645_Table�����У��������ݱ�ʶ��˳�򲻿ɵ���������
	*/
	QCSG_ALARM_E2000003_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000003_LIST_S*)QCSG_ALARM_E2000003_LIST;
	QCSG_ALARM_E2000003_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;
	u8 u8PnNum = 0,i = 0,j = 0;
	u32 u32Ident = 0,u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	
	if(QCSG_CheckAlarmConfig(0xE2000003) != TRUE)
	{
		return;
	}
	if(u16Pn != 0)
	{
		//��ѹ������Ϊ�ն��¼�
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000184,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000184,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
		#if (TEST_ON == 1)
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000184) * 60 * 100 - 3000;
		#else
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000184) * 60 * 100;
		#endif
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000185,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000185) * 60 * 100;
		}

	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000003_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000003Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000003_LIST_S));		
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(j = 0;j < u8PnNum;j++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[j].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[j];
			break;
		}
	}	
	if(j >= u8PnNum)
	{
		return;
	}	
	u8PnNum = pstAlarm_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000003Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000003Data[i];
			break;
		}
	}
	MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
	u32CurrTick = HAL_GetSystemTick();
	if(u32Ident == 0x04000507)
	{
		if(i >= u8PnNum)
		{
			//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
			if(u8PnNum < MAX_PN_NUM2)
			{
				pstAlarm_Info = &pstAlarm_List->stE2000003Data[u8PnNum];
				pstAlarm_Info->u16Pn = u16Pn;
				MemCopy(pstAlarm_Info->u8Data04000507,&pstDlt645Frame_Info->u8RevBuf[14],2);
				MemSet(pstAlarm_Info->u8Data14000001,0xFF,3);
				pstAlarm_Info->u8AlarmFlag = 0xFF;
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_List->u8PnNum ++;
			}
			return;
		}
		MemCopy(pstAlarm_Info->u8Data04000507,&pstDlt645Frame_Info->u8RevBuf[14],2);
		if((pstAlarm_Info->u8Data04000507[0] & 0x01) == 0)
		{
			//δ�е�ѹ������״̬�����ж��Ƿ���Ҫ�����ָ��¼�
			if(pstAlarm_Info->u8AlarmFlag == 1)
			{
				//��ǰ�¼����ڷ���״̬
				if(pstAlarm_Info->u8RestoreStep == ALARM_END)
				{
					pstAlarm_Info->u8RestoreStep = ALARM_START;
					pstAlarm_Info->u32RestoreTick = u32CurrTick;
				}
				if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
				{
					//�����ָ��¼�
					MemSet(&stYYTime,0,sizeof(stYYTime));
					RD_Time_TO_YYTime(pstTime,&stYYTime);
					RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
					u32ReadLen = sizeof(stARD02Data);
					NAND_WriteAlarmRecord(pu8MeterId,0xE2000003,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
					TerminalSetLastAlarmDI(0xE2000003);
					if(QCSG_CheckAlarmReport(0xE2000003) == TRUE)
					{
						TerminalReportAlarmInc();
					}					
					pstAlarm_Info->u8RestoreStep = ALARM_END;
					pstAlarm_Info->u32RestoreTick = u32CurrTick;
					pstAlarm_Info->u8AlarmFlag = 0xFF;						
				}
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;	
		}
	}
	if(u32Ident == 0x14000001)
	{
		if(pstAlarm_Info->u8Data14000001[0] == 0xFF)
		{
			//��һ�γ���������
			MemCopy(pstAlarm_Info->u8Data14000001,&pstDlt645Frame_Info->u8RevBuf[14],3);
			return;
		}
		if(memcmp(pstAlarm_Info->u8Data14000001,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
		{
			//���γ��������������һ�£�׼�����������¼�
			pstAlarm_Info->u8OccurStep = ALARM_START;
			pstAlarm_Info->u32OccurTick = u32CurrTick;				
		}
		if((pstAlarm_Info->u8OccurStep == ALARM_START)&&((pstAlarm_Info->u8Data04000507[0] & 0x01) == 1))
		{
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000003,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000003);
				if(QCSG_CheckAlarmReport(0xE2000003) == TRUE)
				{
					TerminalReportAlarmInc();
				}
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
		}
		MemCopy(pstAlarm_Info->u8Data14000001,&pstDlt645Frame_Info->u8RevBuf[14],3);
	}
	return;
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������������ƽ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000004(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000004_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000004_LIST_S*)QCSG_ALARM_E2000004_LIST;
	QCSG_ALARM_E2000004_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16CurrValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32Ib = 0,u32Ic = 0,u32In = 0,u32Iaver = 0,u32Isub = 0,u32Ipmax = 0,u32Ipmin = 0;
	
	if(QCSG_CheckAlarmConfig(0xE2000004) != TRUE)
	{
		return;
	}
	if(u16Pn != 0)
	{
		//������ƽ��Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();	
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000004_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000004Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000004_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000004Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000004Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000004Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;	
		}
		else
		{			
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;	
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);	
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800100,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		if(u8Meterphase != 0x04)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x30) * 100;
			u16LowLimitValue = QCSG_BCD2HEX(0x25) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x50) * 100;
			u16LowLimitValue = QCSG_BCD2HEX(0x40) * 100;
		}
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800100,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x50) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800100>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800101,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x40) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800101>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);	
		}
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);

//���������������һ��
	u32Ipmax = u32Ia;
	if(u8Meterphase == 0x04)
	{
		if(u32Ib > u32Ipmax)
		{
			u32Ipmax = u32Ib;
		}
	}
	if(u32Ic > u32Ipmax)
	{
		u32Ipmax = u32Ic;
	}

//�����������С����һ��

	u32Ipmin = u32Ia;
	if(u8Meterphase == 0x04)
	{
		if(u32Ipmin > u32Ib)
		{
			u32Ipmin = u32Ib;
		}
	}
	if(u32Ipmin > u32Ic)
	{
		u32Ipmin = u32Ic;
	}	


//�����������ƽ��ֵ	
	if(u8Meterphase == 0x04)
	{
		u32Iaver = (u32Ia + u32Ib + u32Ic)/3;
	}
	else
	{
		u32Iaver = (u32Ia + u32Ic)/2;
	}

	
	if((u32Iaver - u32Ipmin) >= (u32Ipmax - u32Iaver))
	{
		u32Isub = u32Iaver - u32Ipmin;
	}
	else
	{
		u32Isub = u32Ipmax - u32Iaver;
	}
	if((u32Iaver * 100) < (20 * u32In))
	{
		return ;
	}
	if(u32Iaver == 0)
	{
		return ;
	}
	u16CurrValue = (u32Isub * 10000)/u32Iaver;
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16CurrValue > u16UpLimitValue)
		{
			//��ƽ��ȴ������ޣ�׼�������¼�
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000004,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000004);
				if(QCSG_CheckAlarmReport(0xE2000004) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16CurrValue < u16LowLimitValue)
		{
			//��ƽ���С�����ޣ�׼�������ָ��¼�
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000004,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000004);
				if(QCSG_CheckAlarmReport(0xE2000004) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼��������ѹ��ƽ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000005(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�ԳƷ��������������ڶ�ȡ�������ݿ�0x0201FF00��
	 */
	QCSG_ALARM_E2000005_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000005_LIST_S*)QCSG_ALARM_E2000005_LIST;
	QCSG_ALARM_E2000005_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Pha = 0,u16Phb = 0,u16Phc = 0;	
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	float fUa = 0.0,fUb = 0.0,fUc = 0.0,fPha = 0.0,fPhb = 0.0,fPhc = 0.0,fUpLimitValue = 0.0,fLowLimitValue = 0.0,fCurrValue = 0.0;
	
	if(QCSG_CheckAlarmConfig(0xE2000005) != TRUE)
	{
		return;
	}

	if(u16Pn != 0)
	{
		//��ѹ��ƽ��Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();	
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000005_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000005Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000005_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000005Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000005Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000005Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0207FF00,6) != TRUE)
	{
		return;
	}
	MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	MemCopy(&u16Pha,(u8*)ADDR_ANGLE_UA,2);
	MemCopy(&u16Phb,(u8*)ADDR_ANGLE_UB,2);
	MemCopy(&u16Phc,(u8*)ADDR_ANGLE_UC,2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	u16Pha = QCSG_BCD2HEX((u8)(u16Pha>>8)) * 100 + QCSG_BCD2HEX((u8)u16Pha);
	u16Phb = QCSG_BCD2HEX((u8)(u16Phb>>8)) * 100 + QCSG_BCD2HEX((u8)u16Phb);
	u16Phc = QCSG_BCD2HEX((u8)(u16Phc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Phc);
	if((u16Pha == 0)&&(u16Phb == 0)&&(u16Phc == 0))
	{
		return;
	}
	fUa = u16Ua;
	fUa /= 10;
	
	fUb = u16Ub;
	fUb /= 10;
	
	fUc = u16Uc;
	fUc /= 10;
	
	fPha = u16Pha;
	fPha /= 10;
	
	fPhb = u16Phb;
	fPhb /= 10;
	
	fPhc = u16Phc;
	fPhc /= 10;
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡ��������߷�ʽ
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010A,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x02) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E080010A,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x02) * 100;	
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E080010A>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E080010A);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E080010B,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		}
		else
		{		
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E080010B>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E080010B);
		}
	}
	fUpLimitValue = u16UpLimitValue;
	fUpLimitValue /= 10000;
	fLowLimitValue = u16LowLimitValue;
	fLowLimitValue /= 10000;
	fCurrValue = RD_Vol_UnbalanceCalc(fUa,fUb,fUc,fPha,fPhb,fPhc);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(fCurrValue > (fUpLimitValue * 100))//err20180625 ̨������е�ѹ��ƽ���ڵ�ѹ��ѹ��������
		{
			//��ƽ��ȴ������ޣ�׼�������¼�
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000005,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000005);
				if(QCSG_CheckAlarmReport(0xE2000005) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(fCurrValue < (fLowLimitValue*100))//err20180625 ̨������е�ѹ��ƽ���ڵ�ѹ��ѹ��������
		{
			//��ƽ���С�����ޣ�׼�������ָ��¼�
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000005,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000005);
				if(QCSG_CheckAlarmReport(0xE2000005) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼�������������ƫ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000006(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶��һ�£��������ڶ�ȡ��������0x02800001������
	 */
	QCSG_ALARM_E2000006_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000006_LIST_S*)QCSG_ALARM_E2000006_LIST;
	QCSG_ALARM_E2000006_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ib = 0,u32In = 0;
	
	if(QCSG_CheckAlarmConfig(0xE2000006) != TRUE)
	{
		return;
	}
	if(u16Pn != 0)
	{
		//�������ƫ��Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();	
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000006_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000006Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000006_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000006Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000006Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000006Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800107,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x25) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x15) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800107,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x25) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800107>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800107);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800108,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x15) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800108>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800108);
		}
	}
	//��ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800012,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u32Ib = JC_IN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32Ib = JC_IN;
		}
		else
		{
			u32Ib = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u32In &= 0x007FFFFF;
	u32In = QCSG_BCD2HEX((u8)(u32In>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32In>>8)) * 100 + QCSG_BCD2HEX((u8)u32In);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if((u32In * 10000) > (u32Ib * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000006,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000006);
				if(QCSG_CheckAlarmReport(0xE2000006) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}		
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if((u32In * 10000) < (u32Ib * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000006,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000006);
				if(QCSG_CheckAlarmReport(0xE2000006) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A�����������,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200000D(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶��һ�£��жϵ�������λ���������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E200000D_LIST_S* pstAlarm_List = (QCSG_ALARM_E200000D_LIST_S*)QCSG_ALARM_E200000D_LIST;
	QCSG_ALARM_E200000D_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32Iahex = 0,u32In = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200000D) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		//����������Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200000D_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200000DData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200000D_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200000DData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200000DData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200000DData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000186,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000186,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000186) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000187,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000187) * 60 * 100;		
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Iahex = u32Ia & 0x007FFFFF;
	u32Iahex = QCSG_BCD2HEX((u8)(u32Iahex>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Iahex>>8)) * 100 + QCSG_BCD2HEX((u8)u32Iahex);
	if((u32Iahex * 100) < (5 * u32In))
	{
		return ;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ia & 0x800000) != 0)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200000D,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200000D);
				if(QCSG_CheckAlarmReport(0xE200000D) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ia & 0x800000) == 0)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200000D,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200000D);
				if(QCSG_CheckAlarmReport(0xE200000D) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B�����������,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200000E(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶��һ�£��жϵ�������λ���������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E200000E_LIST_S* pstAlarm_List = (QCSG_ALARM_E200000E_LIST_S*)QCSG_ALARM_E200000E_LIST;
	QCSG_ALARM_E200000E_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ib = 0,u32Ibhex = 0,u32In = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200000E) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		//����������Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200000E_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200000EData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200000E_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200000EData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200000EData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200000EData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[17],3) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000186,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000186,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000186) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000187,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000187) * 60 * 100;		
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ibhex = u32Ib & 0x007FFFFF;
	u32Ibhex = QCSG_BCD2HEX((u8)(u32Ibhex>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ibhex>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ibhex);
	if((u32Ibhex * 100) < (5 * u32In))
	{
		return ;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ib & 0x800000) != 0)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200000E,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200000E);
				if(QCSG_CheckAlarmReport(0xE200000E) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ib & 0x800000) == 0)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200000E,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200000E);
				if(QCSG_CheckAlarmReport(0xE200000E) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C�����������,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200000F(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶��һ�£��жϵ�������λ���������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E200000F_LIST_S* pstAlarm_List = (QCSG_ALARM_E200000F_LIST_S*)QCSG_ALARM_E200000F_LIST;
	QCSG_ALARM_E200000F_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ic = 0,u32Ichex = 0,u32In = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200000F) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		//����������Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200000F_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200000FData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200000F_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200000FData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200000FData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200000FData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[20],3) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000186,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000186,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000186) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000187,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000187) * 60 * 100;		
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ichex = u32Ic & 0x007FFFFF;	
	u32Ichex = QCSG_BCD2HEX((u8)(u32Ichex>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ichex>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ichex);
	if((u32Ichex * 100) < (5 * u32In))
	{
		return ;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ic & 0x800000) != 0)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200000F,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200000F);
				if(QCSG_CheckAlarmReport(0xE200000F) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ic & 0x800000) == 0)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200000F,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200000F);
				if(QCSG_CheckAlarmReport(0xE200000F) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A�����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000010(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000010_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000010_LIST_S*)QCSG_ALARM_E2000010_LIST;
	QCSG_ALARM_E2000010_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32In = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000010) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		//��������Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000010_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000010Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000010_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000010Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000010Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000010Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800104,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x00);
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800104,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800104>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800104);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800109,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x00);			
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800109>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800109);	
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia &= 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ia * 100) > (u32In * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000010,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000010);
				if(QCSG_CheckAlarmReport(0xE2000010) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ia * 100) < (u32In * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000010,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000010);
				if(QCSG_CheckAlarmReport(0xE2000010) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B�����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000011(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000011_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000011_LIST_S*)QCSG_ALARM_E2000011_LIST;
	QCSG_ALARM_E2000011_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ib = 0,u32In = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000011) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		//��������Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000011_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000011Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000011_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000011Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000011Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000011Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[17],3) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800104,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x00);
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800104,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800104>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800104);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800109,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x00);			
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800109>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800109);	
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}	
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib &= 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ib * 100) > (u32In * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000011,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000011);
				if(QCSG_CheckAlarmReport(0xE2000011) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ib * 100) < (u32In * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000011,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000011);
				if(QCSG_CheckAlarmReport(0xE2000011) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C�����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000012(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000012_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000012_LIST_S*)QCSG_ALARM_E2000012_LIST;
	QCSG_ALARM_E2000012_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ic = 0,u32In = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000012) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		//��������Ϊ�ն��¼�
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000012_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000012Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000012_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000012Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000012Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000012Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[20],3) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800104,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x00);
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800104,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x30);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800104>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800104);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800109,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x00);			
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800109>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800109);	
		}
	}
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic &= 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ic * 100) > (u32In * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000012,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000012);
				if(QCSG_CheckAlarmReport(0xE2000012) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ic * 100) < (u32In * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000012,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000012);
				if(QCSG_CheckAlarmReport(0xE2000012) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A��ʧ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000013(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000013_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000013_LIST_S*)QCSG_ALARM_E2000013_LIST;
	QCSG_ALARM_E2000013_INFO_S* pstAlarm_Info = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32In = 0,u32Ib = 0,u32Ic = 0,u32Ua = 0,u32Un = 0;//u32Ua = 0,u32Ub = 0,u32Uc = 0,
	
	if((QCSG_CheckAlarmConfig(0xE2000013) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000013_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000013Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000013_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000013Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000013Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000013Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05);
		u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[2],2);//��������,ȱʡΪ������� 0.5%
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[3],2);//���ص���,ȱʡΪ������� 5%
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);

	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);


	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);

	
	
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);	
	//MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	//MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);	
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);

	
	//u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	//u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
//	if(((u32Ua * 100) < (78 * u32Un))&&((u32Ub * 100) < (78 * u32Un))&&((u32Uc * 100) < (78 * u32Un)))
//����Ҫ��һ���������
	if((u32Ia * 10000) < (u32In * u16LowLimitValue)&&(u32Ib * 10000) <(u32In * u16LowLimitValue)&&(u32Ic * 10000) <(u32In * u16LowLimitValue))

	{
		return;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ia * 10000) < (u32In * u16UpLimitValue)&&(u32Ua * 100) > (78 * u32Un))//С�������������ҵ�ѹ����������ѹ
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000013,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000013);
				if(QCSG_CheckAlarmReport(0xE2000013) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;				
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}		
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ia * 10000) > (u32In * u16LowLimitValue))	//ֻ�е�������ֻҪ�����ָ������Ѿ�����ʧ����
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000013,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000013);
				if(QCSG_CheckAlarmReport(0xE2000013) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;				
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B��ʧ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000014(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000014_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000014_LIST_S*)QCSG_ALARM_E2000014_LIST;
	QCSG_ALARM_E2000014_INFO_S* pstAlarm_Info = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ib = 0,u32Ic = 0,u32Ia = 0,u32In = 0,u32Ub = 0,u32Un = 0;//,u32Ua = 0,u32Uc = 0
	
	if((QCSG_CheckAlarmConfig(0xE2000014) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000014_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000014Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000014_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000014Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000014Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000014Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05);
		u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[2],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[3],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un =JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}

	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);

	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);

	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);


	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	
	//MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);
	//MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);	

	//u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	//u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	//if(((u32Ua * 100) < (78 * u32Un))&&((u32Ub * 100) < (78 * u32Un))&&((u32Uc * 100) < (78 * u32Un)))
//����Ҫ��һ���������
	if((u32Ia * 10000) < (u32In * u16LowLimitValue)&&(u32Ib * 10000) <(u32In * u16LowLimitValue)&&(u32Ic * 10000) <(u32In * u16LowLimitValue))

	{
		return;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ib * 10000) < (u32In * u16UpLimitValue)&&(u32Ub * 100) > (78 * u32Un))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000014,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000014);
				if(QCSG_CheckAlarmReport(0xE2000014) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;				
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}		
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ib * 10000) > (u32In * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000014,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000014);
				if(QCSG_CheckAlarmReport(0xE2000014) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;			
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C��ʧ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000015(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000015_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000015_LIST_S*)QCSG_ALARM_E2000015_LIST;
	QCSG_ALARM_E2000015_INFO_S* pstAlarm_Info = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ic = 0, u32Ia = 0, u32Ib = 0,u32In = 0,u32Uc = 0,u32Un = 0;//u32Ua = 0,u32Ub = 0,
	
	if((QCSG_CheckAlarmConfig(0xE2000015) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000015_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000015Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000015_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000015Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000015Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000015Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05);
		u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[2],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[3],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);

	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);

	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);

	//MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);
	//MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	//u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	//u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);	
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	
	//if(((u32Ua * 100) < (78 * u32Un))&&((u32Ub * 100) < (78 * u32Un))&&((u32Uc * 100) < (78 * u32Un)))
//����Ҫ��һ���������
	if((u32Ia * 10000) < (u32In * u16LowLimitValue)&&(u32Ib * 10000) <(u32In * u16LowLimitValue)&&(u32Ic * 10000) <(u32In * u16LowLimitValue))

	{
		return;
	}

	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ic * 10000) < (u32In * u16UpLimitValue)&&(u32Uc * 100) > (78 * u32Un))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000015,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000015);				
				if(QCSG_CheckAlarmReport(0xE2000015) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;				
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}		
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ic * 10000) > (u32In * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000015,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000015);
				if(QCSG_CheckAlarmReport(0xE2000015) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;					
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A��ʧѹ,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000016(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000016_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000016_LIST_S*)QCSG_ALARM_E2000016_LIST;
	QCSG_ALARM_E2000016_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32In = 0,u32Ua = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000016) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;		
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000016_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000016Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}		
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000016_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000016Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000016Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000016Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
#if(TEST_ON == 1)//err20180718 ̨�������澯 ��С����ʱ��
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100 - 3000;
#else 
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
#endif			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{

#if(TEST_ON == 1)//err20180718 ̨�������澯 ��С����ʱ��
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100 - 3000;
#else 
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
#endif
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);	
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{			
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);	
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	if((u32Ia * 1000) < (5 * u32In))
	{
		return;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ua * 10000) < (u32Un * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000016,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000016);
				if(QCSG_CheckAlarmReport(0xE2000016) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ua * 10000) > (u32Un * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000016,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000016);
				if(QCSG_CheckAlarmReport(0xE2000016) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;			
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B��ʧѹ,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000017(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000017_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000017_LIST_S*)QCSG_ALARM_E2000017_LIST;
	QCSG_ALARM_E2000017_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ib = 0,u32In = 0,u32Ub = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000017) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000017_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000017Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}		
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000017_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000017Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000017Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000017Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
#if(TEST_ON == 1)//err20180718 ̨�������澯 ��С����ʱ��
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100 - 3000;
#else 
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
#endif
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
#if(TEST_ON == 1)//err20180718 ̨�������澯 ��С����ʱ��
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100 - 3000;
#else 
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
#endif

		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase == 3)
		{
			return;
		}
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);	
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{			
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	if((u32Ib * 1000) < (5 * u32In))
	{
		return;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ub * 10000) < (u32Un * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000017,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000017);
				if(QCSG_CheckAlarmReport(0xE2000017) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;			
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ub * 10000) > (u32Un * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000017,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000017);
				if(QCSG_CheckAlarmReport(0xE2000017) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;			
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C��ʧѹ,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000018(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E2000018_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000018_LIST_S*)QCSG_ALARM_E2000018_LIST;
	QCSG_ALARM_E2000018_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ic = 0,u32In = 0,u32Uc = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000018) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000018_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000018Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000018_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000018Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000018Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000018Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
#if(TEST_ON == 1)//err20180718 ̨�������澯 ��С����ʱ��
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100 - 3000;
#else 
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
#endif		
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
#if(TEST_ON == 1)//err20180718 ̨�������澯 ��С����ʱ��
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100 - 3000;
#else 
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
#endif

		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);	
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{			
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);	
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	if((u32Ic * 1000) < (5 * u32In))
	{
		return;
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Uc * 10000) < (u32Un * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000018,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000018);
				if(QCSG_CheckAlarmReport(0xE2000018) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;				
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Uc * 10000) > (u32Un * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000018,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000018);
				if(QCSG_CheckAlarmReport(0xE2000018) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;				
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A���ѹ,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200001A(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��ѹ���ݿ�0x0201FF00��
	 */
	QCSG_ALARM_E200001A_LIST_S* pstAlarm_List = (QCSG_ALARM_E200001A_LIST_S*)QCSG_ALARM_E200001A_LIST;
	QCSG_ALARM_E200001A_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ua = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200001A) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001A_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200001AData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001A_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200001AData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001AData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001AData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)	//��ֵ
		{
			u32MaxOccurTick = 1 * 60 * 100;	
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)	//����ʱ��
		{
			u32MaxRestoreTick = 1 * 60 * 100;		
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;			
		}	
	}
	//��ȡ��������߷�ʽ�����ѹ
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}		
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}
	MemCopy(&u32Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ua * 10) > (u32Un * 12))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001A,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001A);
				if(QCSG_CheckAlarmReport(0xE200001A) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ua * 100) < (u32Un * 105))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001A,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001A);
				if(QCSG_CheckAlarmReport(0xE200001A) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B���ѹ,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200001B(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��ѹ���ݿ�0x0201FF00��
	 */
	QCSG_ALARM_E200001B_LIST_S* pstAlarm_List = (QCSG_ALARM_E200001B_LIST_S*)QCSG_ALARM_E200001B_LIST;
	QCSG_ALARM_E200001B_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ub = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200001B) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001B_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200001BData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001B_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200001BData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001BData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001BData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;	
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;		
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;			
		}	
	}
	//��ȡ��������߷�ʽ�����ѹ
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}		
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}
	MemCopy(&u32Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ub * 10) > (u32Un * 12))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001B,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001B);
				if(QCSG_CheckAlarmReport(0xE200001B) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ub * 100) < (u32Un * 105))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001B,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001B);				
				if(QCSG_CheckAlarmReport(0xE200001B) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C���ѹ,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200001C(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��ѹ���ݿ�0x0201FF00��
	 */
	QCSG_ALARM_E200001C_LIST_S* pstAlarm_List = (QCSG_ALARM_E200001C_LIST_S*)QCSG_ALARM_E200001C_LIST;
	QCSG_ALARM_E200001C_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Uc = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200001C) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001C_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200001CData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001C_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200001CData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001CData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001CData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;	
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;		
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;			
		}	
	}
	//��ȡ��������߷�ʽ�����ѹ
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}		
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	}
	MemCopy(&u32Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);	
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Uc * 10) > (u32Un * 12))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001C,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001C);
				if(QCSG_CheckAlarmReport(0xE200001C) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Uc * 100) < (u32Un * 105))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001C,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001C);
				if(QCSG_CheckAlarmReport(0xE200001C) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A�����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200001D(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E200001D_LIST_S* pstAlarm_List = (QCSG_ALARM_E200001D_LIST_S*)QCSG_ALARM_E200001D_LIST;
	QCSG_ALARM_E200001D_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32In = 0,u32Ua = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200001D) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001D_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200001DData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001D_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200001DData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001DData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001DData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);

	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ia * 1000) > (5 * u32In))
		{
			return;
		}
		if((u32Ua * 10000) < (u32Un * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001D,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001D);
				if(QCSG_CheckAlarmReport(0xE200001D) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ua * 10000) > (u32Un * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001D,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001D);				
				if(QCSG_CheckAlarmReport(0xE200001D) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B�����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200001E(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E200001E_LIST_S* pstAlarm_List = (QCSG_ALARM_E200001E_LIST_S*)QCSG_ALARM_E200001E_LIST;
	QCSG_ALARM_E200001E_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ib = 0,u32In = 0,u32Ub = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200001E) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001E_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200001EData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001E_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200001EData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001EData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001EData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);	
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);

	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ib * 1000) > (5 * u32In))
		{
			return;
		}
		if((u32Ub * 10000) < (u32Un * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001E,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001E);				
				if(QCSG_CheckAlarmReport(0xE200001E) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Ub * 10000) > (u32Un * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001E,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001E);				
				if(QCSG_CheckAlarmReport(0xE200001E) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C�����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200001F(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_ALARM_E200001F_LIST_S* pstAlarm_List = (QCSG_ALARM_E200001F_LIST_S*)QCSG_ALARM_E200001F_LIST;
	QCSG_ALARM_E200001F_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ic = 0,u32In = 0,u32Uc = 0,u32Un = 0;
	
	if((QCSG_CheckAlarmConfig(0xE200001F) != TRUE))
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001F_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200001FData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200001F_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200001FData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001FData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200001FData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000182,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 1 * 60 * 100;
		u32MaxRestoreTick = 1 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000182,1) != TRUE)
		{
			u32MaxOccurTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000182) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000183,1) != TRUE)
		{
			u32MaxRestoreTick = 1 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000183) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);	
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);

	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((u32Ic * 1000) > (5 * u32In))
		{
			return;
		}
		if((u32Uc * 10000) < (u32Un * u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001F,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001F);				
				if(QCSG_CheckAlarmReport(0xE200001F) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}			
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((u32Uc * 10000) > (u32Un * u16LowLimitValue))
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200001F,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200001F);
				if(QCSG_CheckAlarmReport(0xE200001F) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����,A���ѹ����,ARD14
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000020(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡA���ѹʧ���0x02080100��
	 */
	QCSG_ALARM_E2000020_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000020_LIST_S*)QCSG_ALARM_E2000020_LIST;
	QCSG_ALARM_E2000020_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	PowerQualityParaStruct stPowerQualityPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD14Struct stARD14Data;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16Value = 0,u16Ua = 0,u16Un = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;	

	if((QCSG_CheckAlarmConfig(0xE2000020) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000020_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000020Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000020_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000020Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000020Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000020Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u16Un = JC_UN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u16Un = JC_UN;
		}
		else
		{
			u16Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u16Ua,&pstRam_Data->u8Data0201FF00[0],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	if(((u16Ua * 100) < (70 * u16Un)) || ((u16Ua * 100) > (130 * u16Un)))
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16Value,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16Value = QCSG_BCD2HEX((u8)(u16Value>>8)) * 100 + QCSG_BCD2HEX((u8)u16Value);
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800200,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800200,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800200>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800200);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800202,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800202>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800202);
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16Value > u16UpLimitValue)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(1,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000020,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);	
				TerminalSetLastAlarmDI(0xE2000020);
				if(QCSG_CheckAlarmReport(0xE2000020) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16Value < u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(0,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000020,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000020);
				if(QCSG_CheckAlarmReport(0xE2000020) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����,B���ѹ����,ARD14
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000021(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡB���ѹʧ���0x02080200��
	 */
	QCSG_ALARM_E2000021_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000021_LIST_S*)QCSG_ALARM_E2000021_LIST;
	QCSG_ALARM_E2000021_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	PowerQualityParaStruct stPowerQualityPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD14Struct stARD14Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16Value = 0,u16Ub = 0,u16Un = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;	

	if((QCSG_CheckAlarmConfig(0xE2000021) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	u32CurrTick = HAL_GetSystemTick();
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000021_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000021Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000021_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000021Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000021Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000021Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u16Un = JC_UN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u16Un = JC_UN;
		}
		else
		{
			u16Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u16Ub,&pstRam_Data->u8Data0201FF00[2],2);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	if(((u16Ub * 100) < (70 * u16Un)) || ((u16Ub * 100) > (130 * u16Un)))
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16Value,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16Value = QCSG_BCD2HEX((u8)(u16Value>>8)) * 100 + QCSG_BCD2HEX((u8)u16Value);
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800200,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800200,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800200>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800200);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800202,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800202>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800202);
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16Value > u16UpLimitValue)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(1,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000021,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000021);
				if(QCSG_CheckAlarmReport(0xE2000021) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16Value < u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(0,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000021,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000021);
				if(QCSG_CheckAlarmReport(0xE2000021) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����,C���ѹ����,ARD14
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000022(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡC���ѹʧ���0x02080300��
	 */
	QCSG_ALARM_E2000022_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000022_LIST_S*)QCSG_ALARM_E2000022_LIST;
	QCSG_ALARM_E2000022_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	PowerQualityParaStruct stPowerQualityPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD14Struct stARD14Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16Value = 0,u16Uc = 0,u16Un = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;	

	if((QCSG_CheckAlarmConfig(0xE2000022) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000022_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000022Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000022_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000022Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000022Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000022Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u16Un = JC_UN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u16Un = JC_UN;
		}
		else
		{
			u16Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u16Uc,&pstRam_Data->u8Data0201FF00[4],2);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(((u16Uc * 100) < (70 * u16Un)) || ((u16Uc * 100) > (130 * u16Un)))
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16Value,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16Value = QCSG_BCD2HEX((u8)(u16Value>>8)) * 100 + QCSG_BCD2HEX((u8)u16Value);
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800200,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800200,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800200>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800200);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800202,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800202>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800202);
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16Value > u16UpLimitValue)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(1,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000022,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000022);				
				if(QCSG_CheckAlarmReport(0xE2000022) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16Value < u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(0,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000022,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000022);
				if(QCSG_CheckAlarmReport(0xE2000022) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����,A���������,ARD14
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000023(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡA�����ʧ���0x02090100��
	 */
	QCSG_ALARM_E2000023_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000023_LIST_S*)QCSG_ALARM_E2000023_LIST;
	QCSG_ALARM_E2000023_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	PowerQualityParaStruct stPowerQualityPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD14Struct stARD14Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16Value = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0,u32Ia = 0,u32In = 0;	

	if((QCSG_CheckAlarmConfig(0xE2000023) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000023_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000023Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000023_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000023Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000023Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000023Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u32In = JC_IN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}			
	}
	MemCopy(&u32Ia,&pstRam_Data->u8Data0202FF00[0],3);
	u32Ia &= 0x7FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16))* 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8))* 100 + QCSG_BCD2HEX((u8)u32Ia);
	//�����������ƫ��ʱ��A�����Ϊ1.6Aʱ������A��������䣬���Ӵ�����������澯�Ĳ���
	if(((u32Ia * 100) < (20 * u32In)) || (u32Ia > 1300))
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16Value,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16Value = QCSG_BCD2HEX((u8)(u16Value>>8)) * 100 + QCSG_BCD2HEX((u8)u16Value);
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;	
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;		
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;			
		}	
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800201,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800201,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;	
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800201>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800201);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800203,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800203>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800203);
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16Value > u16UpLimitValue)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(1,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000023,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000023);
				if(QCSG_CheckAlarmReport(0xE2000023) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16Value < u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(0,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000023,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000023);
				if(QCSG_CheckAlarmReport(0xE2000023) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����,B���������,ARD14
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000024(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡB�����ʧ���0x02090200��
	 */
	QCSG_ALARM_E2000024_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000024_LIST_S*)QCSG_ALARM_E2000024_LIST;
	QCSG_ALARM_E2000024_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	PowerQualityParaStruct stPowerQualityPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD14Struct stARD14Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16Value = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0,u32Ib = 0,u32In = 0;	

	if((QCSG_CheckAlarmConfig(0xE2000024) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000024_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000024Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000024_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000024Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000024Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000024Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u32In = JC_IN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}			
	}
	MemCopy(&u32Ib,&pstRam_Data->u8Data0202FF00[3],3);
	u32Ib &= 0x7FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16))* 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8))* 100 + QCSG_BCD2HEX((u8)u32Ib);		
	//�����������ƫ��ʱ��A�����Ϊ1.6Aʱ������A��������䣬���Ӵ�����������澯�Ĳ���
	if(((u32Ib * 100) < (20 * u32In)) || (u32Ib > 1300))
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16Value,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16Value = QCSG_BCD2HEX((u8)(u16Value>>8)) * 100 + QCSG_BCD2HEX((u8)u16Value);
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;	
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;		
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;			
		}	
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800201,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800201,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;	
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800201>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800201);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800203,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800203>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800203);
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16Value > u16UpLimitValue)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(1,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000024,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000024);				
				if(QCSG_CheckAlarmReport(0xE2000024) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16Value < u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(0,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000024,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000024);				
				if(QCSG_CheckAlarmReport(0xE2000024) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}		
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����,C���������,ARD14
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000025(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡC�����ʧ���0x02090300��
	 */
	QCSG_ALARM_E2000025_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000025_LIST_S*)QCSG_ALARM_E2000025_LIST;
	QCSG_ALARM_E2000025_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	PowerQualityParaStruct stPowerQualityPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD14Struct stARD14Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16Value = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0,u32Ic = 0,u32In = 0;	

	if((QCSG_CheckAlarmConfig(0xE2000025) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000025_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000025Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000025_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000025Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000025Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000025Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u32In = JC_IN;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}			
	}
	MemCopy(&u32Ic,&pstRam_Data->u8Data0202FF00[6],3);
	u32Ic &= 0x7FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16))* 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8))* 100 + QCSG_BCD2HEX((u8)u32Ic);		
	if(((u32Ic * 100) < (20 * u32In)) || (u32Ic > 1300))
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16Value,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16Value = QCSG_BCD2HEX((u8)(u16Value>>8)) * 100 + QCSG_BCD2HEX((u8)u16Value);
	//��ȡ����ʱ��	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;	
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;			
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;		
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;			
		}	
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800201,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800201,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x05) * 100;	
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800201>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800201);
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800203,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x03) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800203>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800203);
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u16Value > u16UpLimitValue)
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(1,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000025,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000025);
				if(QCSG_CheckAlarmReport(0xE2000025) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u16Value < u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD14_Data(0,pstTime,&stARD14Data);
				u32ReadLen = sizeof(stARD14Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000025,&stYYTime,(u8*)&stARD14Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000025);				
				if(QCSG_CheckAlarmReport(0xE2000025) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}		
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼�������޹�������,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000026(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��������0x0206FF00��
	 */
	QCSG_ALARM_E2000026_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000026_LIST_S*)QCSG_ALARM_E2000026_LIST;
	QCSG_ALARM_E2000026_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;		
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32Ib = 0,u32Ic = 0, u32In= 0,u32Iaver = 0;
	u32 u32Pkw = 0,u32Pkvar = 0,u32Pfactor = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000026) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}	
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000026_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000026Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000026_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000026Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000026Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000026Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0202FF00,9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0203FF00,3) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0204FF00,3) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ia,&pstRam_Data->u8Data0202FF00[0],3);
	MemCopy(&u32Ib,&pstRam_Data->u8Data0202FF00[3],3);
	MemCopy(&u32Ic,&pstRam_Data->u8Data0202FF00[6],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Pkw,&pstRam_Data->u8Data0203FF00[0],3);
	MemCopy(&u32Pkvar,&pstRam_Data->u8Data0204FF00[0],3);
	MemCopy(&u32Pfactor,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u32Pfactor &= 0x7FFF;
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	if(u8Meterphase == 0x04)
	{
		u32Iaver = (u32Ia + u32Ib + u32Ic)/3;
	}
	else
	{
		u32Iaver = (u32Ia + u32Ic)/2;		
	}
	if(100 * u32Iaver < 20 * u32In)
	{
		//ƽ������С��20%��������������¼��ж�
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080020D,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = 0x0800;
		u16LowLimitValue = 0x0900;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E080020D,2) != TRUE)
		{
			u16UpLimitValue = 0x0800;
		}
		else
		{
			u16UpLimitValue = stPowerQualityPara.E080020D;
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E080020E,2) != TRUE)
		{
			u16LowLimitValue = 0x0900;
		}
		else
		{
			u16LowLimitValue = stPowerQualityPara.E080020E;
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if(((u32Pkw & 0x800000) == 0)&&((u32Pkvar & 0x800000) != 0)&&(u32Pfactor < u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000026,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000026);
				if(QCSG_CheckAlarmReport(0xE2000026) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if(u32Pfactor > u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000026,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000026);				
				if(QCSG_CheckAlarmReport(0xE2000026) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼�������޹�Ƿ����,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000027(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��������0x0206FF00��
	 */
	QCSG_ALARM_E2000027_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000027_LIST_S*)QCSG_ALARM_E2000027_LIST;
	QCSG_ALARM_E2000027_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Ia = 0,u32Ib = 0,u32Ic = 0, u32In= 0,u32Iaver = 0;
	u32 u32Pkw = 0,u32Pkvar = 0,u32Pfactor = 0;
	
	if((QCSG_CheckAlarmConfig(0xE2000027) != TRUE))
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}	
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000027_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000027Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000027_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000027Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000027Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000027Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0202FF00,9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0203FF00,3) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0204FF00,3) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ia,&pstRam_Data->u8Data0202FF00[0],3);
	MemCopy(&u32Ib,&pstRam_Data->u8Data0202FF00[3],3);
	MemCopy(&u32Ic,&pstRam_Data->u8Data0202FF00[6],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Pkw,&pstRam_Data->u8Data0203FF00[0],3);
	MemCopy(&u32Pkvar,&pstRam_Data->u8Data0204FF00[0],3);
	MemCopy(&u32Pfactor,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u32Pfactor &= 0x7FFF;
	//��ȡ��������߷�ʽ����ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);
		}
	}
	if(u8Meterphase == 0x04)
	{
		u32Iaver = (u32Ia + u32Ib + u32Ic)/3;
	}
	else
	{
		u32Iaver = (u32Ia + u32Ic)/2;		
	}
	if(100 * u32Iaver < 20 * u32In)
	{
		//ƽ������С��20%��������������¼��ж�
		return;
	}
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080020D,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = 0x0800;
		u16LowLimitValue = 0x0900;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E080020D,2) != TRUE)
		{
			u16UpLimitValue = 0x0800;
		}
		else
		{
			u16UpLimitValue = stPowerQualityPara.E080020D;
		}
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E080020E,2) != TRUE)
		{
			u16LowLimitValue = 0x0900;
		}
		else
		{
			u16LowLimitValue = stPowerQualityPara.E080020E;
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if(((u32Pkw & 0x800000) == 0)&&((u32Pkvar & 0x800000) == 0)&&(u32Pfactor < u16UpLimitValue))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000027,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000027);
				if(QCSG_CheckAlarmReport(0xE2000027) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}				
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if(u32Pfactor > u16LowLimitValue)
		{
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000027,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000027);
				if(QCSG_CheckAlarmReport(0xE2000027) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}			
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ʳ���ֵ
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000028(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ɹ���,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000029(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ���ڹ���0x02050000��
	 */
	QCSG_ALARM_E2000029_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000029_LIST_S*)QCSG_ALARM_E2000029_LIST;
	QCSG_ALARM_E2000029_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Pva = 0,u32Puesr = 0;//,u32Un = 0,u32In = 0;
	
	if(QCSG_CheckAlarmConfig(0xE2000029) != TRUE)
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000029_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000029Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000029_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000029Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000029Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000029Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	//��ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800013,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
	#if (TEST_ON == 1)
		u32Puesr  = 92 ; //err20180709 �󱨸��ɹ���
	#else
		u32In = JC_IN;
		u32Un = JC_UN;
	#endif
	}
	else
	{
	#if (TEST_ON == 1)
		u32Puesr =  QCSG_BCD2HEX((u8)(stPnPara.UserDeclareCapacity>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.UserDeclareCapacity);//err20180709 �󱨸��ɹ���
	#else
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
		//�����
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
		//���ѹ
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}
	#endif
	}	
#if(TEST_ON == 0) //err20180709 �󱨸��ɹ���
	u32Puesr = u32Un * u32In/1000;//����10000��
#endif

	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
			//��������ʱ��

		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}

			//�ָ�����ʱ��

		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	//��ȡԽ�޷�ֵ	��E0800105 ��Խ��ֵ(ϵ��)�����жϸ��ɹ�����Զֵ�ı�����ȱʡΪ 1.2
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800105,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x20);
		u16LowLimitValue = QCSG_BCD2HEX(0x00) * 100 + QCSG_BCD2HEX(0x95);
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800105,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x01) * 100 + QCSG_BCD2HEX(0x20);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800105>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800105);
		}

		//E0800106 ,�ָ���ֵ(ϵ��)���жϸ��ɹ��ػָ�����Զֵ�ı���
		if(QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800106,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x00) * 100 + QCSG_BCD2HEX(0x95);			
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800106>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800106);	
		}
	}

	
	MemCopy(&u32Pva,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32Pva &= 0x007FFFFF;
	u32Pva = QCSG_BCD2HEX((u8)(u32Pva>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pva>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pva);//����10000��
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
	#if(TEST_ON == 0)
		if(u32Pva * 100 > u32Puesr * u16UpLimitValue)
	#else 
		if(u32Pva  > u32Puesr * u16UpLimitValue)
	#endif
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000029,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);	
				TerminalSetLastAlarmDI(0xE2000029);
				if(QCSG_CheckAlarmReport(0xE2000029) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
	#if(TEST_ON == 0)
		if(u32Pva * 100 < u32Puesr * u16LowLimitValue)
	#else 
		if(u32Pva  < u32Puesr * u16LowLimitValue)
	#endif
		{
			//׼�������ָ��¼�
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000029,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000029);
				if(QCSG_CheckAlarmReport(0xE2000029) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����������ͬ�����õ�,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200002A(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ���ڹ���0x02050000��
	 */
	QCSG_ALARM_E200002A_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002A_LIST_S*)QCSG_ALARM_E200002A_LIST;
	QCSG_ALARM_E200002A_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32MaxOccurTick = 0,u32MaxRestoreTick = 0,u32CurrTick = 0,u32ReadLen = 0;
	u32 u32Pva = 0,u32Puesr = 0;
	
	if(QCSG_CheckAlarmConfig(0xE200002A) != TRUE)
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstAlarm_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002A_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200002AData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002A_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200002AData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002AData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002AData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	//��ȡ�����
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800013,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stPnPara.UserDeclareCapacity,2) != TRUE)
		{
			return;
		}
		u32Puesr = QCSG_BCD2HEX((u8)(stPnPara.UserDeclareCapacity>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.UserDeclareCapacity);
	}		
	//��ȡ����ʱ��
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0000180,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u32MaxOccurTick = 15 * 60 * 100;
		u32MaxRestoreTick = 15 * 60 * 100;
	}
	else
	{
		if(QCSG_BCDCheck(&stThresholdPara.E0000180,1) != TRUE)
		{
			u32MaxOccurTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxOccurTick = QCSG_BCD2HEX(stThresholdPara.E0000180) * 60 * 100;
		}
		if(QCSG_BCDCheck(&stThresholdPara.E0000181,1) != TRUE)
		{
			u32MaxRestoreTick = 15 * 60 * 100;
		}
		else
		{
			u32MaxRestoreTick = QCSG_BCD2HEX(stThresholdPara.E0000181) * 60 * 100;
		}
	}
	MemCopy(&u32Pva,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32Pva &= 0x007FFFFF;
	u32Pva = QCSG_BCD2HEX((u8)(u32Pva>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pva>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pva);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		//���κ��¼�������ʱ��ֻ�ж��¼��Ƿ���
		if(u32Pva > (u32Puesr * 100))
		{
			if(pstAlarm_Info->u8OccurStep != ALARM_START)
			{
				pstAlarm_Info->u8OccurStep = ALARM_START;
				pstAlarm_Info->u32OccurTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32OccurTick) >= u32MaxOccurTick)
			{
				//���������¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200002A,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200002A);
				if(QCSG_CheckAlarmReport(0xE200002A) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 1;
			}
		}
		else
		{
			pstAlarm_Info->u8OccurStep = ALARM_END;
			pstAlarm_Info->u32OccurTick = u32CurrTick;	
			pstAlarm_Info->u8AlarmFlag = 0xFF;					
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 1)
	{
		if(u32Pva < (u32Puesr * 100))
		{
			//׼�������ָ��¼�
			if(pstAlarm_Info->u8RestoreStep != ALARM_START)
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
			}
			if((u32CurrTick - pstAlarm_Info->u32RestoreTick) >= u32MaxRestoreTick)
			{
				//�����ָ��¼�
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD02_Data(0,pstTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200002A,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);	
				TerminalSetLastAlarmDI(0xE200002A);
				if(QCSG_CheckAlarmReport(0xE200002A) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u32RestoreTick = u32CurrTick;
				pstAlarm_Info->u8AlarmFlag = 0xFF;						
			}
		}
		else
		{
			pstAlarm_Info->u8RestoreStep = ALARM_END;
			pstAlarm_Info->u32RestoreTick = u32CurrTick;				
		}
	}		
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʣ�����,ARD4
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200002B(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶��һ�£��¼������������ڵ�ʣ���ʣ����С�ڷ�ֵ���б���,0x00900200
	 */
	QCSG_ALARM_E200002B_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002B_LIST_S*)QCSG_ALARM_E200002B_LIST;
	QCSG_ALARM_E200002B_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	RealTimeYYStruct stYYTime;
	ARD04Struct stARD04Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32Value = 0,u32ReadLen = 0;	
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200002B) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],4) != TRUE)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002B_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200002BData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002B_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200002BData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002BData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002BData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE000018B,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		stThresholdPara.E000018B = 0x2000;
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stThresholdPara.E000018B,2) != TRUE)
		{
			stThresholdPara.E000018B = 0x2000;
		}
	}
	MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[14],4);
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if(u32Value < stThresholdPara.E000018B)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD04_Data(1,pstTime,&u32Value,pstRam_Data,&stARD04Data);//err201080911 ���ʣ����
			u32ReadLen = sizeof(stARD04Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002B,&stYYTime,(u8*)&stARD04Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002B);
			if(QCSG_CheckAlarmReport(0xE200002B) == TRUE)
			{
				TerminalReportAlarmInc();
			}
			pstAlarm_Info->u8AlarmFlag = 1;			
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if(u32Value > stThresholdPara.E000018B)
		{
			//�����ָ��¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD04_Data(0,pstTime,&u32Value,pstRam_Data,&stARD04Data);//err201080911 ���ʣ����
			u32ReadLen = sizeof(stARD04Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002B,&stYYTime,(u8*)&stARD04Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002B);
			if(QCSG_CheckAlarmReport(0xE200002B) == TRUE)
			{
				TerminalReportAlarmInc();
			}
			pstAlarm_Info->u8AlarmFlag = 0xFF;			
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʾ���½�,ARD03
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200002C(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������й�����0x0001FF00��0x0002FF00
	 */
	QCSG_ALARM_E200002C_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002C_LIST_S*)QCSG_ALARM_E200002C_LIST;
	QCSG_ALARM_E200002C_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD03Struct stARD03Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32Ident = 0,u32LastValue = 0,u32CurrValue = 0,u32ReadLen = 0;
	u32 u32LastValueList[2],u32CurrValueList[2];
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200002C) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],4) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
	u8PnNum = pstAlarm_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200002CData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002CData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002CData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			if(u32Ident == 0x0001FF00)
			{
				MemCopy(pstAlarm_Info->u8Data00010000,&pstDlt645Frame_Info->u8RevBuf[14],4);
				MemSet(pstAlarm_Info->u8Data00020000,0xFF,4);

				pstAlarm_List->u8PnNum ++;
			}
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	if(u32Ident == 0x0002FF00)
	{
		if(pstAlarm_Info->u8Data00020000[0] == 0xFF)
		{
			MemCopy(pstAlarm_Info->u8Data00020000,&pstDlt645Frame_Info->u8RevBuf[14],4);	
			return;
		}
	}
	if(u32Ident == 0x0001FF00)
	{
		MemCopy(&u32LastValue,pstAlarm_Info->u8Data00010000,4);
		MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[14],4);
		u32CurrValueList[0] = u32CurrValue;
		MemCopy(&u32CurrValueList[1],pstAlarm_Info->u8Data00020000,4);
		MemCopy(&u32LastValueList[0],pstAlarm_Info->u8Data00010000,4);
		MemCopy(&u32LastValueList[1],pstAlarm_Info->u8Data00020000,4);
		if(u32CurrValue < u32LastValue)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD03_Data(1,pstTime,u32LastValueList,u32CurrValueList,pstRam_Data,&stARD03Data);
			u32ReadLen = sizeof(stARD03Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002C,&stYYTime,(u8*)&stARD03Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002C);
			if(QCSG_CheckAlarmReport(0xE200002C) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
		MemCopy(pstAlarm_Info->u8Data00010000,&pstDlt645Frame_Info->u8RevBuf[14],4);	
	}
	if(u32Ident == 0x0002FF00)
	{
		MemCopy(&u32LastValue,pstAlarm_Info->u8Data00020000,4);
		MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[14],4);
		u32CurrValueList[1] = u32CurrValue;
		MemCopy(&u32CurrValueList[0],pstAlarm_Info->u8Data00010000,4);
		MemCopy(&u32LastValueList[0],pstAlarm_Info->u8Data00010000,4);
		MemCopy(&u32LastValueList[1],pstAlarm_Info->u8Data00020000,4);		
		if(u32CurrValue < u32LastValue)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD03_Data(1,pstTime,u32LastValueList,u32CurrValueList,pstRam_Data,&stARD03Data);
			u32ReadLen = sizeof(stARD03Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002C,&stYYTime,(u8*)&stARD03Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002C);
			if(QCSG_CheckAlarmReport(0xE200002C) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
		MemCopy(pstAlarm_Info->u8Data00020000,&pstDlt645Frame_Info->u8RevBuf[14],4);	
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼����������
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200002D(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ���й�����0x0203FF00��
	 */
	QCSG_ALARM_E200002D_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002D_LIST_S*)QCSG_ALARM_E200002D_LIST;
	QCSG_ALARM_E200002D_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	RealTimeYYStruct stYYTime;
	ARD03Struct stARD03Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,u8Value = 0,i = 0;
	u16 u16Tmin = 0;
	u32 u32CalcEkwh = 0,u32ReadEkwh = 0,u32LastPEkwh = 0,u32LastNEkwh = 0,u32CurrPEkwh = 0,u32CurrNEkwh = 0,u32Pkw = 0,u32TimeSubRes[4],u32ReadLen = 0;
	u32 u32LastValueList[2],u32CurrValueList[2];

	//err20180823 ���߸澯 ���������з���ǰ��������д��ͬ ����һ����ʱ�����洢��һ�ε�������
	u32 u32Temp_LastData00010000,u32Temp_LastData00020000;
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200002D) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	MemCopy(&u32Pkw,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Pkw &= 0x7FFFFF;
	u32Pkw = QCSG_BCD2HEX((u8)(u32Pkw>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pkw>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pkw);
	u8PnNum = pstPn_Ram_Data->u8PnNum;	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	if(QCSG_BCDCheck(pstRam_Data->u8Data00010000,4) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data00020000,4) != TRUE)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200002DData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002DData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002DData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			MemCopy(&pstAlarm_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstAlarm_Info->u8LastData00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstAlarm_Info->u8LastData00020000,pstRam_Data->u8Data00020000,4);
			pstAlarm_Info->u32CalcEkwh = 0;
			pstAlarm_Info->u32ReadEkwh = 0;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	MemCopy(&u32LastPEkwh,pstAlarm_Info->u8LastData00010000,4);
	MemCopy(&u32LastNEkwh,pstAlarm_Info->u8LastData00020000,4);
	MemCopy(&u32CurrPEkwh,pstRam_Data->u8Data00010000,4);
	MemCopy(&u32CurrNEkwh,pstRam_Data->u8Data00020000,4);
	u32LastPEkwh = QCSG_BCD2HEX((u8)(u32LastPEkwh>>24)) * 1000000 +  QCSG_BCD2HEX((u8)(u32LastPEkwh>>16)) * 10000 
					+ QCSG_BCD2HEX((u8)(u32LastPEkwh>>8)) * 100 + QCSG_BCD2HEX((u8)u32LastPEkwh);
	u32LastNEkwh = QCSG_BCD2HEX((u8)(u32LastNEkwh>>24)) * 1000000 +  QCSG_BCD2HEX((u8)(u32LastNEkwh>>16)) * 10000 
					+ QCSG_BCD2HEX((u8)(u32LastNEkwh>>8)) * 100 + QCSG_BCD2HEX((u8)u32LastNEkwh);
	u32CurrPEkwh = QCSG_BCD2HEX((u8)(u32CurrPEkwh>>24)) * 1000000 +  QCSG_BCD2HEX((u8)(u32CurrPEkwh>>16)) * 10000 
					+ QCSG_BCD2HEX((u8)(u32CurrPEkwh>>8)) * 100 + QCSG_BCD2HEX((u8)u32CurrPEkwh);
	u32CurrNEkwh = QCSG_BCD2HEX((u8)(u32CurrNEkwh>>24)) * 1000000 +  QCSG_BCD2HEX((u8)(u32CurrNEkwh>>16)) * 10000 
					+ QCSG_BCD2HEX((u8)(u32CurrNEkwh>>8)) * 100 + QCSG_BCD2HEX((u8)u32CurrNEkwh);
	//ˢ�¼�¼����һ�ε���ʾֵ
	//err20180823 ���ܱ����߸澯 ���������з���ǰ��������д��ͬ ������ʱ���������һ�ε���ֵ			
	MemCopy(&u32Temp_LastData00010000,pstAlarm_Info->u8LastData00010000,4);//
	MemCopy(&u32Temp_LastData00020000,pstAlarm_Info->u8LastData00020000,4);//

	MemCopy(pstAlarm_Info->u8LastData00010000,pstRam_Data->u8Data00010000,4);
	MemCopy(pstAlarm_Info->u8LastData00020000,pstRam_Data->u8Data00020000,4);
	if((u32CurrPEkwh < u32LastPEkwh)||(u32CurrNEkwh < u32LastNEkwh))
	{
		return;
	}
	u32ReadEkwh = (u32CurrPEkwh - u32LastPEkwh) + (u32CurrNEkwh - u32LastNEkwh);
	pstAlarm_Info->u32ReadEkwh += u32ReadEkwh;
	
	if(QCSG_TimerSub(pstTime,&pstAlarm_Info->stStartTime,u32TimeSubRes) < 0)
	{
		//ˢ����һ��ʾֵ��¼��ʱ��
		MemCopy(&pstAlarm_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
		return;
	}
	//ˢ����һ��ʾֵ��¼��ʱ��
	MemCopy(&pstAlarm_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
	if(u32TimeSubRes[0] != 0)
	{
		return;
	}
	u16Tmin = u32TimeSubRes[1] * 60 + u32TimeSubRes[2];
	//u32Pkw����10000����ʾֵ������100���������ٳ���100�����ָ�u32ReadEkwh����һ��
	u32CalcEkwh = u32Pkw * u16Tmin/(60 * 100);	
	pstAlarm_Info->u32CalcEkwh += u32CalcEkwh;	
	if(pstAlarm_Info->u32CalcEkwh < 10)
	{
		return;
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE0800189,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u8Value = 10;
	}
	else
	{
		u8Value = QCSG_BCD2HEX(stThresholdPara.E0000189);
	}
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if(pstAlarm_Info->u32ReadEkwh > (pstAlarm_Info->u32CalcEkwh * u8Value))
		{
			//���������¼�
			MemCopy(&u32CurrValueList[0],pstRam_Data->u8Data00010000,4);
			MemCopy(&u32CurrValueList[1],pstRam_Data->u8Data00020000,4);
			
			//err20180823 ���ܱ����߸澯 ���������з���ǰ��������д��ͬ ������ʱ���������һ�ε���ֵ
			MemCopy(&u32LastValueList[0],&u32Temp_LastData00010000,4);
			MemCopy(&u32LastValueList[1],&u32Temp_LastData00020000,4);
			//MemCopy(&u32LastValueList[0],pstAlarm_Info->u8LastData00010000,4);
			//MemCopy(&u32LastValueList[1],pstAlarm_Info->u8LastData00020000,4);	
			
			MemSet(&stYYTime,0,sizeof(stYYTime));			
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD03_Data(1,pstTime,u32LastValueList,u32CurrValueList,pstRam_Data,&stARD03Data);
			u32ReadLen = sizeof(stARD03Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002D,&stYYTime,(u8*)&stARD03Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002D);
			if(QCSG_CheckAlarmReport(0xE200002D) == TRUE)
			{
				TerminalReportAlarmInc();
			}							
			pstAlarm_Info->u8AlarmFlag = 0x01;
		}	
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if(pstAlarm_Info->u32ReadEkwh < (pstAlarm_Info->u32CalcEkwh * u8Value))
		{
			pstAlarm_Info->u8AlarmFlag = 0xFF;
		}	
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ͣ��,ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200002E(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£���������0x0203FF00
	 */
	QCSG_ALARM_E200002E_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002E_LIST_S*)QCSG_ALARM_E200002E_LIST;
	QCSG_ALARM_E200002E_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ThresholdParaStruct stThresholdPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16Value = 0;
	u32 u32CurrTick = 0,u32P = 0,u32ReadLen = 0;
	u32 u32Start_PE = 0,u32Start_NE = 0,u32Curr_PE = 0,u32Curr_NE = 0;
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200002E) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	u8PnNum = pstPn_Ram_Data->u8PnNum;	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200002EData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002EData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002EData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			MemCopy(pstAlarm_Info->u8Data00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);
			pstAlarm_Info->u32E_Quantity = 0;
			pstAlarm_Info->u32LastTick = u32CurrTick;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(ThresholdParaStruct);
	if(NAND_ReadPara(PARA_Threshold,0,0xE080018A,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
	{
		u16Value = QCSG_BCD2HEX(0x10);
	}
	else
	{
		if(QCSG_BCDCheck((u8*)&stThresholdPara.E000018A,2) != TRUE)
		{
			u16Value = QCSG_BCD2HEX(0x10);
		}
		else
		{
			u16Value = QCSG_BCD2HEX((u8)(stThresholdPara.E000018A>>8)) * 100 + QCSG_BCD2HEX((u8)(stThresholdPara.E000018A));
		}
	}
	MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32P &= 0x007FFFFF;
	u32P = QCSG_BCD2HEX((u8)(u32P>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32P>>8)) * 100 + QCSG_BCD2HEX((u8)u32P);
	pstAlarm_Info->u32E_Quantity += (u32CurrTick - pstAlarm_Info->u32LastTick) * u32P/(3600 * 100);
	if(pstAlarm_Info->u32E_Quantity < u16Value * 100)
	{
		pstAlarm_Info->u32LastTick = u32CurrTick;
		return;
	}
	MemCopy(&u32Start_PE,pstAlarm_Info->u8Data00010000,4);
	MemCopy(&u32Start_NE,pstAlarm_Info->u8Data00020000,4);
	u32Start_PE	= QCSG_BCD2HEX((u8)(u32Start_PE>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32Start_PE>>16)) * 10000 + 
				  QCSG_BCD2HEX((u8)(u32Start_PE>>8)) * 100 + QCSG_BCD2HEX((u8)u32Start_PE);
	u32Start_NE	= QCSG_BCD2HEX((u8)(u32Start_NE>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32Start_NE>>16)) * 10000 + 
				  QCSG_BCD2HEX((u8)(u32Start_NE>>8)) * 100 + QCSG_BCD2HEX((u8)u32Start_NE);
	MemCopy(&u32Curr_PE,pstRam_Data->u8Data00010000,4);
	MemCopy(&u32Curr_NE,pstRam_Data->u8Data00020000,4);
	u32Curr_PE	= QCSG_BCD2HEX((u8)(u32Curr_PE>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32Curr_PE>>16)) * 10000 + 
				  QCSG_BCD2HEX((u8)(u32Curr_PE>>8)) * 100 + QCSG_BCD2HEX((u8)u32Curr_PE);
	u32Curr_NE	= QCSG_BCD2HEX((u8)(u32Curr_NE>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32Curr_NE>>16)) * 10000 + 
				  QCSG_BCD2HEX((u8)(u32Curr_NE>>8)) * 100 + QCSG_BCD2HEX((u8)u32Curr_NE);	
	if((u32Start_PE + u32Start_NE) == (u32Curr_PE + u32Curr_NE))
	{
		if(pstAlarm_Info->u8AlarmFlag == 0xFF)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));			
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
			u32ReadLen = sizeof(stARD02Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002E,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002E);			
			if(QCSG_CheckAlarmReport(0xE200002E) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
		pstAlarm_Info->u8AlarmFlag = 1;
	}
	else
	{
		pstAlarm_Info->u16Pn = u16Pn;
		pstAlarm_Info->u8AlarmFlag = 0xFF;
		MemCopy(pstAlarm_Info->u8Data00010000,pstRam_Data->u8Data00010000,4);
		MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);
		pstAlarm_Info->u32E_Quantity = 0;
		pstAlarm_Info->u32LastTick = u32CurrTick;		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͨ��ʧ��,ARD1
*�������:u8FailFlag:�ɹ�ʧ�ܱ�ʶ��pstTime������ʱ�䣬u16Pn���������
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200002F(u8 u8FailFlag,TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId)
{
	QCSG_ALARM_E200002F_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002F_LIST_S*)QCSG_ALARM_E200002F_LIST;
	QCSG_ALARM_E200002F_INFO_S* pstAlarm_Info = NULL;
	RealTimeYYStruct stYYTime;
	ARD01Struct stARD01Data;		
	u8 u8PnNum = 0,i = 0;
	u32 u32ReadLen = 0;
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200002F) != TRUE)
	{
		return;
	}	
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002F_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200002FData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002F_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200002FData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002FData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200002FData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_Info->u8FailTimes = 0;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	if(u8FailFlag == TRUE)
	{
		//�����ɹ�
		if(pstAlarm_Info->u8AlarmFlag == 0x01)
		{
			//�����ָ��¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD01_Data(0,pstTime,&stARD01Data);
			u32ReadLen = sizeof(stARD01Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE200002F,&stYYTime,(u8*)&stARD01Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE200002F);
			if(QCSG_CheckAlarmReport(0xE200002F) == TRUE)
			{
				TerminalReportAlarmInc();
			}
			pstAlarm_Info->u8FailTimes = 0;
			pstAlarm_Info->u8AlarmFlag = 0xFF;			
		}
	}
	if(u8FailFlag == FALSE)
	{
		//����ʧ��
		pstAlarm_Info->u8FailTimes++;
		if(pstAlarm_Info->u8FailTimes >= 3)
		{
			//���������¼�
			if(pstAlarm_Info->u8AlarmFlag != 0x01)
			{
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD01_Data(1,pstTime,&stARD01Data);
				u32ReadLen = sizeof(stARD01Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200002F,&stYYTime,(u8*)&stARD01Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200002F);
				if(QCSG_CheckAlarmReport(0xE200002F) == TRUE)
				{
					TerminalReportAlarmInc();
				}				
			}
			pstAlarm_Info->u8AlarmFlag = 0x01;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼��������澯
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000030(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£���������0x0001FF00,ֻ��ɵ����������
	 */
	QCSG_ALARM_E2000030_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000030_LIST_S*)QCSG_ALARM_E2000030_LIST;
	QCSG_ALARM_E2000030_INFO_S* pstAlarm_Info = NULL;
	QCSG_DAY_ENERGY_LIST_S* pst_Day_Energy_List = (QCSG_DAY_ENERGY_LIST_S*)QCSG_DAY_ENERGY_LIST_LCD;
	QCSG_ENERGY_S* pstEnergy_Data = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;	
	DiffCaculationParaStruct* pstDiffPara = NULL;
	ms_Type* pMS_Info = NULL;	
	RealTimeYYStruct stYYTime;
	ARD03Struct stARD03Data;
	u8 u8PnNum = 0,i = 0,j = 0,k = 0,u8CtrlCode = 0,u8TotalDiffNum = 0,u8ValidDiffNum = 0,u8ValidDiffList[MAX_DIFF_CACU_NUM];
	u16 u16Pnref = 0,u16Pndiff = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0,u16CurrValue = 0;
	u32 u32MaxMallocSize = 0,u32ReadLen = 0,u32DiffIdent = 0;
	u32 u32PnrefDataEnergy = 0,u32PndifDataEnergy = 0,u32MinDiffValue = 0;	
	u32 u32PnrefStartEnergy = 0,u32PndifStartEnergy = 0;
	u32 u32PnrefUp = 0,u32PndiffUp = 0,u32CurrData00010000 = 0;
	u32 u32LastValueList[2],u32CurrValueList[2];
	
	if(u16Pn == 0)
	{
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE2000030) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],4) != TRUE)
	{
		return;
	}
	MemCopy(&u32CurrData00010000,&pstDlt645Frame_Info->u8RevBuf[14],4);

	
	pMS_Info = Get_ms();
	u32MaxMallocSize = pMS_Info->ms_malloc_sizemax();
	if(u32MaxMallocSize < sizeof(DiffCaculationParaStruct))
	{
		//��̬�ڴ���䲻���ã�ֱ�ӷ��أ��˺����޷�ִ���� 
		return;
	}

	
	pstDiffPara = pMS_Info->ms_malloc(sizeof(DiffCaculationParaStruct));
	u32ReadLen = sizeof(DiffCaculationParaStruct);
	if(NAND_ReadPara(PARA_Caculation,0,0xE0000800,(u8*)pstDiffPara,&u32ReadLen) != NO_ERROR)
	{
		pMS_Info->ms_free(pstDiffPara);
		return;
	}
	u8TotalDiffNum = pstDiffPara->DiffNum;
	if((u8TotalDiffNum > MAX_DIFF_CACU_NUM) || (u8TotalDiffNum == 0))
	{
		MemSet((u8*)QCSG_ALARM_E2000030_LIST,0,512);		
		pMS_Info->ms_free(pstDiffPara);
		return;		
	}
	for(i = 0;i<u8TotalDiffNum;i++)
	{
		if((pstDiffPara->DiffStateMark[0] & (1 << i)) != 0)
		{
			u8ValidDiffList[u8ValidDiffNum] = i;	//����Ч����ı��
			u8ValidDiffNum++;
		}
	}
	if(u8ValidDiffNum == 0)
	{
		MemSet((u8*)QCSG_ALARM_E2000030_LIST,0,512);
		pMS_Info->ms_free(pstDiffPara);
		return;				
	}
	for(i = 0;i < u8ValidDiffNum;i++)
	{
		k = u8ValidDiffList[i];
		//��������
		MemCopy(&u32DiffIdent,&pstDiffPara->DiffID[k][0],4);
		if(u32DiffIdent != 0x00010000)
		{
			//ֻ���������
			continue;
		}
		MemCopy(&u16Pndiff,&pstDiffPara->DiffID[k][6],2);//	���Ƚϲ������
		u16Pndiff = (u16)SwitchNumToPn(u16Pndiff);
		if(u16Pndiff != u16Pn)
		{
			//��������㲻���ڲ���÷�Χ
			continue;
		}
		MemCopy(&u16Pnref,&pstDiffPara->DiffID[k][4],2);	//�Ƚϲ�����ż��ο�������
		if(u16Pnref != 0)
		{
			//�ο�������ֻ�����ն�
			continue;
		}


		//ȡ�ο�������
		u8PnNum = pst_Day_Energy_List->u8PnNum;
		for(j = 0;j < u8PnNum;j++)
		{
			//Ѱ���ڴ��м�¼�ı��Ƚϲ������������Ϣ
			if(0 == pst_Day_Energy_List->stPn_Ram_Data[j].u16Pn)
			{
				pstEnergy_Data = &pst_Day_Energy_List->stPn_Ram_Data[j];
				break;
			}
		}	
		if(j >= u8PnNum)
		{
			pMS_Info->ms_free(pstDiffPara);
			return;
		}
		if(QCSG_BCDCheck((u8*)&pstEnergy_Data->u32Energy00010000,4) != TRUE)
		{
			pMS_Info->ms_free(pstDiffPara);
			return;
		}			
		MemCopy(&u32PnrefDataEnergy,&pstEnergy_Data->u32Energy00010000,4);


//ȡҪ�Ƚϵĵ������
		
		for(j = 0;j < u8PnNum;j++)
		{
			//Ѱ���ڴ��м�¼�ıȽϲ�����������Ϣ
			if(u16Pn == pst_Day_Energy_List->stPn_Ram_Data[j].u16Pn)
			{
				pstEnergy_Data = &pst_Day_Energy_List->stPn_Ram_Data[j];
				break;
			}
		}	
		if(j >= u8PnNum)
		{
			pMS_Info->ms_free(pstDiffPara);
			return;
		}
		MemCopy(&u32PndifDataEnergy,&pstEnergy_Data->u32Energy00010000,4);

		
		for(j = 0;j < u8PnNum;j++)
		{
			//Ѱ���ڴ��м�¼�ıȽϲ�����������Ϣ
			if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[j].u16Pn)
			{
				pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[j];
				break;
			}
		}	
		if(j >= u8PnNum)
		{
			pMS_Info->ms_free(pstDiffPara);
			return;
		}		


		//ȡҪ�Ƚϵĵ�ĸ澯��Ϣ
		u8PnNum = pstAlarm_List->u8PnNum;
		for(j = 0;j < u8PnNum;j++)
		{
			//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
			if(u16Pn == pstAlarm_List->stE2000030Data[j].u16Pn)
			{
				pstAlarm_Info = &pstAlarm_List->stE2000030Data[j];
				break;
			}
		}
		if(j >= u8PnNum)
		{
			//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
			if(u8PnNum < MAX_PN_NUM2)
			{
				pstAlarm_Info = &pstAlarm_List->stE2000030Data[u8PnNum];
				pstAlarm_Info->u16Pn = u16Pn;
				pstAlarm_Info->u8AlarmFlag = 0xFF;
				MemCopy(&pstAlarm_Info->u32PnrefStartEnergy,&u32PnrefDataEnergy,4);
				MemCopy(&pstAlarm_Info->u32PndifStartEnergy,&u32PndifDataEnergy,4);
				MemCopy(&pstAlarm_Info->u32PndifData00010000,&pstDlt645Frame_Info->u8RevBuf[14],4);
				MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);//err20180925 � ���ӷ���ǰ����	
				pstAlarm_Info->u32PnrefEnergy = 0;
				pstAlarm_Info->u32PndifEnergy = 0;
				pstAlarm_List->u8PnNum ++;
			}
			else
			{
				pMS_Info->ms_free(pstDiffPara);
				return;				
			}
		}
		if(u32CurrData00010000 < pstAlarm_Info->u32PndifData00010000)
		{
			//̨����Ըø澯�ָ�ʱ�����µ������ʾֵ���˴���Ϊ̨��ָ���һ�����Ա��
			MemCopy(&pstAlarm_Info->u32PnrefStartEnergy,&u32PnrefDataEnergy,4);
			MemCopy(&pstAlarm_Info->u32PndifStartEnergy,&u32PndifDataEnergy,4);
			MemCopy(&pstAlarm_Info->u32PndifData00010000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);//err20180925 � ���ӷ���ǰ����
			pstAlarm_Info->u32PnrefEnergy = 0;
			pstAlarm_Info->u32PndifEnergy = 0;			
		}				
		MemCopy(&u32MinDiffValue,&pstDiffPara->DiffID[k][8],3);//�����ò���ʱ�������С���Բ�ֵ
		u32MinDiffValue = QCSG_BCD2HEX((u8)(u32MinDiffValue>>16)) * 10000 + 
						  QCSG_BCD2HEX((u8)(u32MinDiffValue>>8)) * 100 +	//err20180716 ̨���澯���ϸ� *10 -> *100
						  QCSG_BCD2HEX((u8)(u32MinDiffValue));
		u32PndifDataEnergy = QCSG_BCD2HEX((u8)(u32PndifDataEnergy>>24)) * 1000000 + 
								QCSG_BCD2HEX((u8)(u32PndifDataEnergy>>16)) * 10000 + 
								QCSG_BCD2HEX((u8)(u32PndifDataEnergy>>8)) * 100 +
								QCSG_BCD2HEX((u8)(u32PndifDataEnergy));
		u32PndifStartEnergy = QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PndifStartEnergy>>24)) * 1000000 + 
								 QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PndifStartEnergy>>16)) * 10000 + 
								 QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PndifStartEnergy>>8)) * 100 + 
								 QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PndifStartEnergy));
		u32PnrefDataEnergy = QCSG_BCD2HEX((u8)(u32PnrefDataEnergy>>24)) * 1000000 + 
							   QCSG_BCD2HEX((u8)(u32PnrefDataEnergy>>16)) * 10000 + 
							   QCSG_BCD2HEX((u8)(u32PnrefDataEnergy>>8)) * 100 +
							   QCSG_BCD2HEX((u8)(u32PnrefDataEnergy));		
		u32PnrefStartEnergy = QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PnrefStartEnergy>>24)) * 1000000 + 
								QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PnrefStartEnergy>>16)) * 10000 + 
								QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PnrefStartEnergy>>8)) * 100 +
								QCSG_BCD2HEX((u8)(pstAlarm_Info->u32PnrefStartEnergy));
		if((u32PndifDataEnergy - u32PndifStartEnergy) < u32MinDiffValue)
		{
			continue;
		}
		//�����������ˣ������жϲ�澯
		u32PndiffUp = u32PndifDataEnergy - u32PndifStartEnergy;
		u32PnrefUp = u32PnrefDataEnergy - u32PnrefStartEnergy;
		pstAlarm_Info->u32PnrefEnergy += u32PnrefUp;
		pstAlarm_Info->u32PndifEnergy += u32PndiffUp;
		u32PnrefUp = pstAlarm_Info->u32PnrefEnergy;
		MemCopy(&u16UpLimitValue,&pstDiffPara->DiffID[k][11],2);	//����ʱ�����ֵ(%)
		MemCopy(&u16LowLimitValue,&pstDiffPara->DiffID[k][13],2);	//����ʱ����ָ���ֵ(%)
		u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 +
						  QCSG_BCD2HEX((u8)(u16UpLimitValue));	
		u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 +
						  QCSG_BCD2HEX((u8)(u16LowLimitValue));	
		if(u32PnrefUp == 0)
		{
			continue;
		}
		if(pstAlarm_Info->u32PndifEnergy >= pstAlarm_Info->u32PnrefEnergy)
		{
			u16CurrValue = pstAlarm_Info->u32PndifEnergy - pstAlarm_Info->u32PnrefEnergy;
		}
		else
		{
			u16CurrValue = pstAlarm_Info->u32PnrefEnergy - pstAlarm_Info->u32PndifEnergy;
		}
		if(pstAlarm_Info->u8AlarmFlag == 0xFF)
		{
			if((u16CurrValue * 10000 /  u32PnrefUp) > u16UpLimitValue)
			{
				//���������¼�
				//MemCopy(&u32LastValueList[0],pstRam_Data->u8Data00010000,4);
				//MemCopy(&u32LastValueList[1],pstRam_Data->u8Data00020000,4);
				MemCopy(&u32LastValueList[0],&pstAlarm_Info->u32PndifData00010000,4);
				MemCopy(&u32LastValueList[1],pstAlarm_Info->u8Data00020000,4);//err20180925 � ���ӷ���ǰ����
				
				MemCopy(&u32CurrValueList[0],pstRam_Data->u8Data00010000,4);
				MemCopy(&u32CurrValueList[1],pstRam_Data->u8Data00020000,4);
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD03_Data(1,pstTime,u32LastValueList,u32CurrValueList,pstRam_Data,&stARD03Data);
				u32ReadLen = sizeof(stARD03Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000030,&stYYTime,(u8*)&stARD03Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000030);
				if(QCSG_CheckAlarmReport(0xE2000030) == TRUE)
				{
					TerminalReportAlarmInc();
				}	
				pstAlarm_Info->u8AlarmFlag = 0x01;				
			}
			pstAlarm_Info->u32PndifStartEnergy = hex_bcd(u32PndifDataEnergy);
			pstAlarm_Info->u32PnrefStartEnergy = hex_bcd(u32PnrefDataEnergy);			
		}
		if(pstAlarm_Info->u8AlarmFlag == 0x01)
		{
			if((u16CurrValue * 10000 /  u32PnrefUp) < u16LowLimitValue)
			{
				//�����ָ��¼�
				//MemCopy(&u32LastValueList[0],pstRam_Data->u8Data00010000,4);
				//MemCopy(&u32LastValueList[1],pstRam_Data->u8Data00020000,4);
				MemCopy(&u32LastValueList[0],&pstAlarm_Info->u32PndifData00010000,4);
				MemCopy(&u32LastValueList[1],pstAlarm_Info->u8Data00020000,4);//err20180925 � ���ӷ���ǰ����
				
				MemCopy(&u32CurrValueList[0],pstRam_Data->u8Data00010000,4);
				MemCopy(&u32CurrValueList[1],pstRam_Data->u8Data00020000,4);
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD03_Data(0,pstTime,u32LastValueList,u32CurrValueList,pstRam_Data,&stARD03Data);
				u32ReadLen = sizeof(stARD03Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE2000030,&stYYTime,(u8*)&stARD03Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE2000030);				
				if(QCSG_CheckAlarmReport(0xE2000030) == TRUE)
				{
					TerminalReportAlarmInc();
				}					
				pstAlarm_Info->u8AlarmFlag = 0xFF;	
				pstAlarm_Info->u32PndifStartEnergy = hex_bcd(u32PndifDataEnergy);
				pstAlarm_Info->u32PnrefStartEnergy = hex_bcd(u32PnrefDataEnergy);				
			}			
		}			
	}
	pMS_Info->ms_free(pstDiffPara);
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������������������¼,ARD7
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000031(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�����������0x03300200��
	 */
	QCSG_ALARM_E2000031_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000031_LIST_S*)QCSG_ALARM_E2000031_LIST;
	QCSG_ALARM_E2000031_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD07Struct stARD07Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;
	if(QCSG_CheckAlarmConfig(0xE2000031) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000031_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000031Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000031_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000031Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000031Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000031Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			MemCopy(pstAlarm_Info->u8Data03300200,&pstDlt645Frame_Info->u8RevBuf[14],3);
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	if(memcmp(pstAlarm_Info->u8Data03300200,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{
		//���������¼�
		MemSet(&stYYTime,0,sizeof(stYYTime));
		RD_Time_TO_YYTime(pstTime,&stYYTime);
		RD_Write_ARD07_Data(1,pstTime,pstRam_Data,&stARD07Data);
		u32ReadLen = sizeof(stARD07Data);
		NAND_WriteAlarmRecord(pu8MeterId,0xE2000031,&stYYTime,(u8*)&stARD07Data,&u32ReadLen);
		TerminalSetLastAlarmDI(0xE2000031);
		if(QCSG_CheckAlarmReport(0xE2000031) == TRUE)
		{
			TerminalReportAlarmInc();
		}			
	}
	MemCopy(pstAlarm_Info->u8Data03300200,&pstDlt645Frame_Info->u8RevBuf[14],3);		
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ܱ�ʱ�ӵ�ص�ѹ����,ARD1
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000032(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ���ܱ�����״̬��0x04000501��
	 */
	QCSG_ALARM_E2000032_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000032_LIST_S*)QCSG_ALARM_E2000032_LIST;
	QCSG_ALARM_E2000032_INFO_S* pstAlarm_Info = NULL;
	RealTimeYYStruct stYYTime;
	ARD01Struct stARD01Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;
	
	if(QCSG_CheckAlarmConfig(0xE2000032) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],1) != TRUE)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000032_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000032Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000032_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000032Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000032Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000032Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	if(pstAlarm_Info->u8AlarmFlag == 0xFF)
	{
		if((pstDlt645Frame_Info->u8RevBuf[14] & 0x04) != 0)//״̬��bit2:ʱ�ӵ���Ƿ�Ƿѹ
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD01_Data(1,pstTime,&stARD01Data);
			u32ReadLen = sizeof(stARD01Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000032,&stYYTime,(u8*)&stARD01Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000032);
			if(QCSG_CheckAlarmReport(0xE2000032) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
			pstAlarm_Info->u8AlarmFlag = 0x01;//err20180619 �¼����������ñ�־λ���Է�ÿ�γ����������¼�
		}
	}
	if(pstAlarm_Info->u8AlarmFlag == 0x01)
	{
		if((pstDlt645Frame_Info->u8RevBuf[14] & 0x04) == 0)
		{
			//�ָ�Ϊ�澯��ʼ״̬��̨����Բ������ָ��¼�
			pstAlarm_Info->u8AlarmFlag = 0xFF;			
		}
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ܱ����ʱ����ġ�ARD2
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000035(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��̴���0x03300000��
	 */
	QCSG_ALARM_E2000035_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000035_LIST_S*)QCSG_ALARM_E2000035_LIST;
	QCSG_ALARM_E2000035_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000035_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000035Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000035_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000035Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000035Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000035Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			MemCopy(pstAlarm_Info->u8Data03300000,&pstDlt645Frame_Info->u8RevBuf[14],3);
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	if(memcmp(pstAlarm_Info->u8Data03300000,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{
		if(QCSG_CheckAlarmConfig(0xE2000035) == TRUE)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD02_Data(1,pstTime,pstRam_Data,&stARD02Data);
			u32ReadLen = sizeof(stARD02Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000035,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000035);
			if(QCSG_CheckAlarmReport(0xE2000035) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
	}
	MemCopy(pstAlarm_Info->u8Data03300000,&pstDlt645Frame_Info->u8RevBuf[14],3);
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ܱ�����ʱ�θ���,ARD1
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000036(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ��
	 */
	QCSG_ALARM_E2000036_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000036_LIST_S*)QCSG_ALARM_E2000036_LIST;
	QCSG_ALARM_E2000036_INFO_S* pstAlarm_Info = NULL;
	RealTimeYYStruct stYYTime;
	ARD01Struct stARD01Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;
	
	u32 u32Ident = 0;//err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE2000036) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4); //err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000036_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000036Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000036_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000036Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000036Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000036Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			if(u32Ident == 0x04010001)	//err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
			{
				MemCopy(pstAlarm_Info->u8Data04010001,&pstDlt645Frame_Info->u8RevBuf[14],42);
				MemSet(pstAlarm_Info->u8Data04020001,0xFF,42);
				pstAlarm_List->u8PnNum ++;
			}
		}
		return;
	}
	if(u32Ident == 0x04020001)//err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
	{
		if(pstAlarm_Info->u8Data04020001[0] == 0xFF)//��֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		{
			MemCopy(pstAlarm_Info->u8Data04020001,&pstDlt645Frame_Info->u8RevBuf[14],42);	
			return;
		}
	}

	if(u32Ident == 0x04010001)	//err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
	{
		if(memcmp(pstAlarm_Info->u8Data04010001,&pstDlt645Frame_Info->u8RevBuf[14],42) != 0)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD01_Data(1,pstTime,&stARD01Data);
			u32ReadLen = sizeof(stARD01Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000036,&stYYTime,(u8*)&stARD01Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000036);
			if(QCSG_CheckAlarmReport(0xE2000036) == TRUE)
			{
				TerminalReportAlarmInc();
			}			
		}
		MemCopy(pstAlarm_Info->u8Data04010001,&pstDlt645Frame_Info->u8RevBuf[14],42);	
	}

	if(u32Ident == 0x04020001)	//err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
	{
		if(memcmp(pstAlarm_Info->u8Data04020001,&pstDlt645Frame_Info->u8RevBuf[14],42) != 0)
		{
			//���������¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD01_Data(1,pstTime,&stARD01Data);
			u32ReadLen = sizeof(stARD01Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000036,&stYYTime,(u8*)&stARD01Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000036);
			if(QCSG_CheckAlarmReport(0xE2000036) == TRUE)
			{
				TerminalReportAlarmInc();
			}			
		}
		MemCopy(pstAlarm_Info->u8Data04020001,&pstDlt645Frame_Info->u8RevBuf[14],42);	
	}
	
	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ܱ����峣������,ARD6
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000037(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ���޹����峣��0x04000409��0x0400040A��
	 */
	QCSG_ALARM_E2000037_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000037_LIST_S*)QCSG_ALARM_E2000037_LIST;
	QCSG_ALARM_E2000037_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD06Struct stARD06Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32Ident = 0,u32ReadLen = 0,u32ValueList[4];
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE2000037) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
	
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000037_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000037Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000037_LIST_S));		
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000037Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000037Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000037Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			if(u32Ident == 0x04000409)//����й�����
			{
				MemCopy(pstAlarm_Info->u8Data04000409,&pstDlt645Frame_Info->u8RevBuf[14],3);
				MemSet(pstAlarm_Info->u8Data0400040A,0xFF,3);
				pstAlarm_List->u8PnNum ++;
			}
		}
		return;
	}
	if(u32Ident == 0x0400040A)//����޹�����
	{
		if(pstAlarm_Info->u8Data0400040A[0] == 0xFF)
		{
			MemCopy(pstAlarm_Info->u8Data0400040A,&pstDlt645Frame_Info->u8RevBuf[14],3);	
			return;
		}
	}
	MemCopy(&u32ValueList[0],pstAlarm_Info->u8Data04000409,3);
	MemCopy(&u32ValueList[1],pstAlarm_Info->u8Data0400040A,3);	
	if(u32Ident == 0x04000409)
	{
		if(memcmp(pstAlarm_Info->u8Data04000409,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
		{
			//���������¼�
			MemCopy(&u32ValueList[2],&pstDlt645Frame_Info->u8RevBuf[14],3);
			MemCopy(&u32ValueList[3],pstAlarm_Info->u8Data0400040A,3);
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD06_Data(1,pstTime,u32ValueList,pstRam_Data,&stARD06Data);
			u32ReadLen = sizeof(stARD06Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000037,&stYYTime,(u8*)&stARD06Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000037);
			if(QCSG_CheckAlarmReport(0xE2000037) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
		MemCopy(pstAlarm_Info->u8Data04000409,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	}
	if(u32Ident == 0x0400040A)
	{
		if(memcmp(pstAlarm_Info->u8Data0400040A,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
		{
			//���������¼�
			MemCopy(&u32ValueList[2],pstAlarm_Info->u8Data04000409,3);
			MemCopy(&u32ValueList[3],&pstDlt645Frame_Info->u8RevBuf[14],3);
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD06_Data(1,pstTime,u32ValueList,pstRam_Data,&stARD06Data);
			u32ReadLen = sizeof(stARD06Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000037,&stYYTime,(u8*)&stARD06Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000037);
			if(QCSG_CheckAlarmReport(0xE2000037) == TRUE)
			{
				TerminalReportAlarmInc();
			}	
		}
		MemCopy(pstAlarm_Info->u8Data0400040A,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼�������ն˻��������ʸ���,ARD5
*�������:��
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E2000038_PN(void)
{
	QCSG_ALARM_E2000038_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000038_LIST_S*)QCSG_ALARM_E2000038_LIST;
	QCSG_ALARM_E2000038_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	TIME_PARAM_S stTime;
	RealTimeYYStruct stYYTime;
	ARD05Struct stARD05Data;
	u8 i = 0,u8PnNum = 0,j = 0;
	u16 u16LastPt = 0,u16LastCt = 0,u16CurrPt = 0,u16CurrCt = 0,u16Pn = 0,u16ValueList[4];
	u32 u32ReadLen = 0;
	
	if(QCSG_CheckAlarmConfig(0xE2000038) != TRUE)
	{
		return;
	}
	//err20180920 �������������ʸ���ֻ���ն� ȥ�����ܱ�	
	//err20181115 ����̨����Ҫ���Ե��ܱ��澯�����Լӻص��ܱ��澯����
	for(j = 0;j < MAX_PN_NUM2;j++)
	{
		u16Pn = (u16)SwitchNumToPn(j);
	
		//u16Pn = (u16)SwitchNumToPn(0);
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			continue;
		}
		if(stPnPara.State == 0)
		{
			continue;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.CT,2) != TRUE)
		{
			continue;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.PT,2) != TRUE)
		{
			continue;
		}
		u16CurrPt = stPnPara.PT;
		u16CurrCt = stPnPara.CT;
		u8PnNum = pstAlarm_List->u8PnNum;	
		for(i = 0;i < u8PnNum;i++)
		{
			//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
			if(u16Pn == pstAlarm_List->stE2000038Data[i].u16Pn)
			{
				pstAlarm_Info = &pstAlarm_List->stE2000038Data[i];
				break;
			}
		}
		if(i >= u8PnNum)
		{
			//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
			if(u8PnNum < MAX_PN_NUM2)
			{
				pstAlarm_Info = &pstAlarm_List->stE2000038Data[u8PnNum];
				pstAlarm_Info->u16Pn = u16Pn;
				pstAlarm_Info->u16Ct = u16CurrCt;		
				pstAlarm_Info->u16Pt = u16CurrPt;				
				pstAlarm_List->u8PnNum ++;
			}
			continue;
		}
		u8PnNum = pstPn_Ram_Data->u8PnNum;		
		for(i = 0;i < u8PnNum;i++)
		{
			//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
			if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
			{
				pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
				break;
			}
		}	
		if(i >= u8PnNum)
		{
			continue;
		}
		u16LastCt = pstAlarm_Info->u16Ct;
		u16LastPt = pstAlarm_Info->u16Pt;
		u16ValueList[0] = u16LastCt;
		u16ValueList[1] = u16LastPt;		
		if((u16CurrCt != u16LastCt) || (u16CurrPt != u16LastPt))
		{
			//���������¼�
			HAL_GetSystemTime(&stTime);
			u16ValueList[2] = u16CurrCt;
			u16ValueList[3] = u16CurrPt;
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(&stTime,&stYYTime);
			RD_Write_ARD05_Data(1,&stTime,u16ValueList,pstRam_Data,&stARD05Data);
			u32ReadLen = sizeof(stARD05Data);
			NAND_WriteAlarmRecord(stPnPara.Addr,0xE2000038,&stYYTime,(u8*)&stARD05Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000038);
			if(QCSG_CheckAlarmReport(0xE2000038) == TRUE)
			{
				TerminalReportAlarmInc();
			}		
		}
		pstAlarm_Info->u16Ct = u16CurrCt;
		pstAlarm_Info->u16Pt = u16CurrPt;		
	} //err20180920 �������������ʸ���ֻ���ն� ȥ�����ܱ�			
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ܱ����������ʸ���,ARD5
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
#if 0
void RD_Alarm_E2000038(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��ѹ����������������0x04000306��0x04000307��
	 */
	QCSG_ALARM_E2000038_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000038_LIST_S*)QCSG_ALARM_E2000038_LIST;
	QCSG_ALARM_E2000038_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD05Struct stARD05Data;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32Ident = 0,u32ReadLen = 0,u32ValueList[4];
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE2000038) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000038_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE2000038Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E2000038_LIST_S));		
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	u8PnNum = pstAlarm_List->u8PnNum;	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE2000038Data[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000038Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE2000038Data[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			if(u32Ident == 0x04000306)
			{
				MemCopy(pstAlarm_Info->u8Data04000306,&pstDlt645Frame_Info->u8RevBuf[14],3);
				MemSet(pstAlarm_Info->u8Data04000307,0xFF,3);
				pstAlarm_List->u8PnNum ++;
			}
		}
		return;
	}
	if(u32Ident == 0x04000307)
	{
		if(pstAlarm_Info->u8Data04000307[0] == 0xFF)
		{
			MemCopy(pstAlarm_Info->u8Data04000307,&pstDlt645Frame_Info->u8RevBuf[14],3);	
			return;
		}
	}
	MemCopy(&u32ValueList[0],pstAlarm_Info->u8Data04000306,3);
	MemCopy(&u32ValueList[1],pstAlarm_Info->u8Data04000307,3);		
	if(u32Ident == 0x04000306)
	{
		if(memcmp(pstAlarm_Info->u8Data04000306,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
		{
			//���������¼�
			MemCopy(&u32ValueList[2],&pstDlt645Frame_Info->u8RevBuf[14],3);
			MemCopy(&u32ValueList[3],pstAlarm_Info->u8Data04000307,3);
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD05_Data(1,pstTime,u32ValueList,pstRam_Data,&stARD05Data);
			u32ReadLen = sizeof(stARD05Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000038,&stYYTime,(u8*)&stARD05Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000038);
			if(QCSG_CheckAlarmReport(0xE2000038) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
		MemCopy(pstAlarm_Info->u8Data04000306,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	}
	if(u32Ident == 0x04000307)
	{
		if(memcmp(pstAlarm_Info->u8Data04000307,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
		{
			//���������¼�
			MemCopy(&u32ValueList[2],pstAlarm_Info->u8Data04000306,3);
			MemCopy(&u32ValueList[3],&pstDlt645Frame_Info->u8RevBuf[14],3);
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ARD05_Data(1,pstTime,u32ValueList,pstRam_Data,&stARD05Data);
			u32ReadLen = sizeof(stARD05Data);
			NAND_WriteAlarmRecord(pu8MeterId,0xE2000038,&stYYTime,(u8*)&stARD05Data,&u32ReadLen);
			TerminalSetLastAlarmDI(0xE2000038);
			if(QCSG_CheckAlarmReport(0xE2000038) == TRUE)
			{
				TerminalReportAlarmInc();
			}				
		}
		MemCopy(pstAlarm_Info->u8Data04000307,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	}	
}
#endif
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʱ���쳣,ARD13
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200003E(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£���������0x04000102
	 */
	QCSG_ALARM_E200003E_LIST_S* pstAlarm_List = (QCSG_ALARM_E200003E_LIST_S*)QCSG_ALARM_E200003E_LIST;
	QCSG_ALARM_E200003E_INFO_S* pstAlarm_Info = NULL;
	TIME_PARAM_S stT_Time,stM_Time;
	ThresholdParaStruct stThresholdPara;	
	RealTimeYYStruct stYYTime;
	ARD13Struct stARD13Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8Value = 0;
	u32 u32Ident = 0,u32ReadLen = 0,u32Res[4];
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200003E) != TRUE)
	{
		return;
	}
	if(HAL_GetSystemTime(&stT_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}	
	MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
	if(u32Ident == 0x04000101)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],4) != TRUE)
		{
			return;
		}		
	}
	if(u32Ident == 0x04000102)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200003E_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200003EData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200003E_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200003EData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200003EData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200003EData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			pstAlarm_Info->u8AlarmFlag = 0xFF;
			pstAlarm_List->u8PnNum ++;
		}
	}
	if(u32Ident == 0x04000101)
	{
		MemCopy(pstAlarm_Info->u8Data04000101,&pstDlt645Frame_Info->u8RevBuf[14],4);
		return;
	}
	if(u32Ident == 0x04000102)
	{
		MemCopy(pstAlarm_Info->u8Data04000102,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		//��ȡԽ�޷�ֵ	
		u32ReadLen = 1;
		if(NAND_ReadPara(PARA_Threshold,0,0xE0800188,(u8*)&stThresholdPara,&u32ReadLen) != NO_ERROR)
		{
			u8Value = QCSG_BCD2HEX(0x10);
		}
		else
		{
			if(QCSG_BCDCheck((u8*)&stThresholdPara.E0000188,1) != TRUE)
			{
				u8Value = QCSG_BCD2HEX(0x10);
			}
			else
			{
				u8Value = QCSG_BCD2HEX(stThresholdPara.E0000188);
			}
		}		
		stM_Time.u8Year = pstAlarm_Info->u8Data04000101[3];
		stM_Time.u8Month = pstAlarm_Info->u8Data04000101[2];
		stM_Time.u8Day = pstAlarm_Info->u8Data04000101[1];
		stM_Time.u8Hour = pstAlarm_Info->u8Data04000102[2];
		stM_Time.u8Minute = pstAlarm_Info->u8Data04000102[1];
		stM_Time.u8Second  = pstAlarm_Info->u8Data04000102[0];
		QCSG_TimerSub(&stT_Time,&stM_Time,u32Res);
		if((u32Res[0] != 0) || (u32Res[1] != 0) || (u32Res[2] > u8Value))
		{
			//���������¼�
			if(pstAlarm_Info->u8AlarmFlag == 0xFF)
			{
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ARD13_Data(1,pstTime,pstAlarm_Info->u8Data04000101,
									pstAlarm_Info->u8Data04000102,&stARD13Data);
				u32ReadLen = sizeof(stARD13Data);
				NAND_WriteAlarmRecord(pu8MeterId,0xE200003E,&stYYTime,(u8*)&stARD13Data,&u32ReadLen);
				TerminalSetLastAlarmDI(0xE200003E);
				if(QCSG_CheckAlarmReport(0xE200003E) == TRUE)
				{
					TerminalReportAlarmInc();
				}
				pstAlarm_Info->u8AlarmFlag = 1;
			}			
		}
		else
		{
			pstAlarm_Info->u8AlarmFlag = 0xFF;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼��������ť�п��Ǹ澯,ARD11
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200004D(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��ť�п��Ǵ���0x03300E00��
	 */
	QCSG_ALARM_E200004D_LIST_S* pstAlarm_List = (QCSG_ALARM_E200004D_LIST_S*)QCSG_ALARM_E200004D_LIST;
	QCSG_ALARM_E200004D_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD11Struct stARD11Data;		
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;
	
	u32 u32ValueList[6];//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
	u8 u8NewData03300E00[3];//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200004D) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200004D_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200004DData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200004D_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200004DData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200004DData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200004DData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			MemCopy(pstAlarm_Info->u8Data03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3);
			//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
			MemCopy(pstAlarm_Info->u8Data00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstAlarm_Info->u8Data00050000,pstRam_Data->u8Data00050000,4);
			MemCopy(pstAlarm_Info->u8Data00060000,pstRam_Data->u8Data00060000,4);
			MemCopy(pstAlarm_Info->u8Data00070000,pstRam_Data->u8Data00070000,4);
			MemCopy(pstAlarm_Info->u8Data00080000,pstRam_Data->u8Data00080000,4);
			
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	if(memcmp(pstAlarm_Info->u8Data03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{

		MemCopy(u8NewData03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3);//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
		MemCopy(&u32ValueList[0],pstAlarm_Info->u8Data00010000,4);//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
		MemCopy(&u32ValueList[1],pstAlarm_Info->u8Data00020000,4);
		MemCopy(&u32ValueList[2],pstAlarm_Info->u8Data00050000,4);
		MemCopy(&u32ValueList[3],pstAlarm_Info->u8Data00060000,4);
		MemCopy(&u32ValueList[4],pstAlarm_Info->u8Data00070000,4);
		MemCopy(&u32ValueList[5],pstAlarm_Info->u8Data00080000,4);
		//���������¼�
		MemSet(&stYYTime,0,sizeof(stYYTime));
		RD_Time_TO_YYTime(pstTime,&stYYTime);
		RD_Write_ARD11_Data(1,pstTime,u8NewData03300E00,u32ValueList,pstRam_Data,&stARD11Data);//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯���� //err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
		u32ReadLen = sizeof(stARD11Data);
		NAND_WriteAlarmRecord(pu8MeterId,0xE200004D,&stYYTime,(u8*)&stARD11Data,&u32ReadLen);
		TerminalSetLastAlarmDI(0xE200004D);
		if(QCSG_CheckAlarmReport(0xE200004D) == TRUE)
		{
			TerminalReportAlarmInc();
		}			
	}
	MemCopy(pstAlarm_Info->u8Data03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3);
	//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
	MemCopy(pstAlarm_Info->u8Data00010000,pstRam_Data->u8Data00010000,4);
	MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);
	MemCopy(pstAlarm_Info->u8Data00050000,pstRam_Data->u8Data00050000,4);
	MemCopy(pstAlarm_Info->u8Data00060000,pstRam_Data->u8Data00060000,4);
	MemCopy(pstAlarm_Info->u8Data00070000,pstRam_Data->u8Data00070000,4);
	MemCopy(pstAlarm_Info->u8Data00080000,pstRam_Data->u8Data00080000,4);
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ǿ����澯,ARD11
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Alarm_E200004E(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ���ǿ�������0x03300D00��
	 */
	QCSG_ALARM_E200004E_LIST_S* pstAlarm_List = (QCSG_ALARM_E200004E_LIST_S*)QCSG_ALARM_E200004E_LIST;
	QCSG_ALARM_E200004E_INFO_S* pstAlarm_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ARD11Struct stARD11Data;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;

	u32 u32ValueList[6];//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
	u8 u8NewData03300D00[3];//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����

	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckAlarmConfig(0xE200004E) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstAlarm_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200004E_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstAlarm_List->stE200004EData[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200004E_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstAlarm_List->stE200004EData[i].u16Pn)
		{
			pstAlarm_Info = &pstAlarm_List->stE200004EData[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstAlarm_Info = &pstAlarm_List->stE200004EData[u8PnNum];
			pstAlarm_Info->u16Pn = u16Pn;
			MemCopy(pstAlarm_Info->u8Data03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3);
			//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
			MemCopy(pstAlarm_Info->u8Data00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstAlarm_Info->u8Data00050000,pstRam_Data->u8Data00050000,4);
			MemCopy(pstAlarm_Info->u8Data00060000,pstRam_Data->u8Data00060000,4);
			MemCopy(pstAlarm_Info->u8Data00070000,pstRam_Data->u8Data00070000,4);
			MemCopy(pstAlarm_Info->u8Data00080000,pstRam_Data->u8Data00080000,4);
			
			pstAlarm_List->u8PnNum ++;
		}
		return;
	}	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	if(memcmp(pstAlarm_Info->u8Data03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{
		MemCopy(u8NewData03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3);//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
		MemCopy(&u32ValueList[0],pstAlarm_Info->u8Data00010000,4);//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
		MemCopy(&u32ValueList[1],pstAlarm_Info->u8Data00020000,4);
		MemCopy(&u32ValueList[2],pstAlarm_Info->u8Data00050000,4);
		MemCopy(&u32ValueList[3],pstAlarm_Info->u8Data00060000,4);
		MemCopy(&u32ValueList[4],pstAlarm_Info->u8Data00070000,4);
		MemCopy(&u32ValueList[5],pstAlarm_Info->u8Data00080000,4);
		//���������¼�
		MemSet(&stYYTime,0,sizeof(stYYTime));
		RD_Time_TO_YYTime(pstTime,&stYYTime);
		RD_Write_ARD11_Data(1,pstTime,u8NewData03300D00,u32ValueList,pstRam_Data,&stARD11Data);//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
		u32ReadLen = sizeof(stARD11Data);
		NAND_WriteAlarmRecord(pu8MeterId,0xE200004E,&stYYTime,(u8*)&stARD11Data,&u32ReadLen);
		TerminalSetLastAlarmDI(0xE200004E);
		if(QCSG_CheckAlarmReport(0xE200004E) == TRUE)
		{
			TerminalReportAlarmInc();
		}			
	}
	MemCopy(pstAlarm_Info->u8Data03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
	MemCopy(pstAlarm_Info->u8Data00010000,pstRam_Data->u8Data00010000,4);
	MemCopy(pstAlarm_Info->u8Data00020000,pstRam_Data->u8Data00020000,4);
	MemCopy(pstAlarm_Info->u8Data00050000,pstRam_Data->u8Data00050000,4);
	MemCopy(pstAlarm_Info->u8Data00060000,pstRam_Data->u8Data00060000,4);
	MemCopy(pstAlarm_Info->u8Data00070000,pstRam_Data->u8Data00070000,4);
	MemCopy(pstAlarm_Info->u8Data00080000,pstRam_Data->u8Data00080000,4);
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʧѹ��¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010004(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_EVENT_E2010004_06_LIST_S* pstEvent_List = (QCSG_EVENT_E2010004_06_LIST_S*)QCSG_EVENT_E2010004_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32Ia = 0,u32In = 0,u32Ua = 0,u32Un = 0,u32ReadLen = 0;	

	if(QCSG_CheckEventMask(0xE2010004) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}						
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);	
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	if((u32Ia * 1000) < (5 * u32In))
	{
		return;
	}
	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ua * 10000) < (u32Un * u16UpLimitValue))	
		{
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;			
		}
	}
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ua * 10000) > (u32Un * u16LowLimitValue))
		{
			u8PnNum = pstEvent_List->u8PnNum;
			for(i = 0;i < u8PnNum;i++)
			{
				if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
				{
					pstEvent_Info = &pstEvent_List->stERD3Data[i];
					break;
				}						
			}
			if(i < u8PnNum)
			{
				MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
				MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
				MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
				MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
				MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
				u32ReadLen = sizeof(stERD03Data);
				NAND_WriteEventRecord(pu8MeterId,0xE2010004,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
				pstEvent_Info->u8EventFlag = 0xFF;
			}	
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʧѹ��¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010005(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_EVENT_E2010004_06_LIST_S* pstEvent_List = (QCSG_EVENT_E2010004_06_LIST_S*)QCSG_EVENT_E2010005_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32Ib = 0,u32In = 0,u32Ub = 0,u32Un = 0,u32ReadLen = 0;	

	if(QCSG_CheckEventMask(0xE2010005) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}						
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);	
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	if((u32Ib * 1000) < (5 * u32In))
	{
		return;
	}
	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ub * 10000) < (u32Un * u16UpLimitValue))	
		{
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;			
		}
	}
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ub * 10000) > (u32Un * u16LowLimitValue))
		{
			u8PnNum = pstEvent_List->u8PnNum;
			for(i = 0;i < u8PnNum;i++)
			{
				if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
				{
					pstEvent_Info = &pstEvent_List->stERD3Data[i];
					break;
				}						
			}
			if(i < u8PnNum)
			{
				MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
				MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
				MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
				MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
				MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
				u32ReadLen = sizeof(stERD03Data);
				NAND_WriteEventRecord(pu8MeterId,0xE2010005,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
				pstEvent_Info->u8EventFlag = 0xFF;
			}	
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʧѹ��¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010006(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_EVENT_E2010004_06_LIST_S* pstEvent_List = (QCSG_EVENT_E2010004_06_LIST_S*)QCSG_EVENT_E2010006_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32Ic = 0,u32In = 0,u32Uc = 0,u32Un = 0,u32ReadLen = 0;	

	if(QCSG_CheckEventMask(0xE2010006) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}						
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);	
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	if((u32Ic * 1000) < (5 * u32In))
	{
		return;
	}
	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Uc * 10000) < (u32Un * u16UpLimitValue))	
		{
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;			
		}
	}
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Uc * 10000) > (u32Un * u16LowLimitValue))
		{
			u8PnNum = pstEvent_List->u8PnNum;
			for(i = 0;i < u8PnNum;i++)
			{
				if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
				{
					pstEvent_Info = &pstEvent_List->stERD3Data[i];
					break;
				}						
			}
			if(i < u8PnNum)
			{
				MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
				MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
				MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
				MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
				MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
				u32ReadLen = sizeof(stERD03Data);
				NAND_WriteEventRecord(pu8MeterId,0xE2010006,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
				pstEvent_Info->u8EventFlag = 0xFF;
			}	
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʧ����¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010007(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_EVENT_E2010007_09_LIST_S* pstEvent_List = (QCSG_EVENT_E2010007_09_LIST_S*)QCSG_EVENT_E2010007_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32Ia = 0,u32In = 0,u32Ua = 0,u32Ub = 0,u32Uc = 0,u32Un = 0,u32ReadLen = 0;	
	
	if(QCSG_CheckEventMask(0xE2010007) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}						
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x50);
		u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[2],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[3],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x50);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);	
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);		
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	if(((u32Ua * 100) < (78 * u32Un))&&((u32Ub * 100) < (78 * u32Un))&&((u32Uc * 100) < (78 * u32Un)))
	{
		return;
	}
	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ia * 10000) < (u32In * u16UpLimitValue))
		{
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;			
		}
	}
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ia * 10000) > (u32In * u16LowLimitValue))
		{
			u8PnNum = pstEvent_List->u8PnNum;
			for(i = 0;i < u8PnNum;i++)
			{
				if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
				{
					pstEvent_Info = &pstEvent_List->stERD3Data[i];
					break;
				}						
			}
			if(i < u8PnNum)
			{
				MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
				MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
				MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
				MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
				MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
				u32ReadLen = sizeof(stERD03Data);
				NAND_WriteEventRecord(pu8MeterId,0xE2010007,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
				pstEvent_Info->u8EventFlag = 0xFF;
			}	
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʧ����¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010008(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_EVENT_E2010007_09_LIST_S* pstEvent_List = (QCSG_EVENT_E2010007_09_LIST_S*)QCSG_EVENT_E2010008_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32Ib = 0,u32In = 0,u32Ua = 0,u32Ub = 0,u32Uc = 0,u32Un = 0,u32ReadLen = 0;	
	
	if(QCSG_CheckEventMask(0xE2010008) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}						
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x50);
		u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[2],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[3],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x50);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}	
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);	
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);		
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	if(((u32Ua * 100) < (78 * u32Un))&&((u32Ub * 100) < (78 * u32Un))&&((u32Uc * 100) < (78 * u32Un)))
	{
		return;
	}
	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ib * 10000) < (u32In * u16UpLimitValue))
		{
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;			
		}
	}
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ib * 10000) > (u32In * u16LowLimitValue))
		{
			u8PnNum = pstEvent_List->u8PnNum;
			for(i = 0;i < u8PnNum;i++)
			{
				if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
				{
					pstEvent_Info = &pstEvent_List->stERD3Data[i];
					break;
				}						
			}
			if(i < u8PnNum)
			{
				MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
				MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
				MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
				MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
				MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
				u32ReadLen = sizeof(stERD03Data);
				NAND_WriteEventRecord(pu8MeterId,0xE2010008,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
				pstEvent_Info->u8EventFlag = 0xFF;
			}	
		}		
	}	
}
/**--------------------------------------------------------------------
*��    ��:�¼���¼������ʧ����¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010009(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_EVENT_E2010007_09_LIST_S* pstEvent_List = (QCSG_EVENT_E2010007_09_LIST_S*)QCSG_EVENT_E2010009_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;	
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;	
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32Ic = 0,u32In = 0,u32Ua = 0,u32Ub = 0,u32Uc = 0,u32Un = 0,u32ReadLen = 0;	
	
	if(QCSG_CheckEventMask(0xE2010009) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}						
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x50);
		u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[2],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[3],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x50);
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x05) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}		
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);	
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);		
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);
	if(((u32Ua * 100) < (78 * u32Un))&&((u32Ub * 100) < (78 * u32Un))&&((u32Uc * 100) < (78 * u32Un)))
	{
		return;
	}
	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ic * 10000) < (u32In * u16UpLimitValue))
		{
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;			
		}
	}
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ic * 10000) > (u32In * u16LowLimitValue))
		{
			u8PnNum = pstEvent_List->u8PnNum;
			for(i = 0;i < u8PnNum;i++)
			{
				if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
				{
					pstEvent_Info = &pstEvent_List->stERD3Data[i];
					break;
				}						
			}
			if(i < u8PnNum)
			{
				MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
				MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
				MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
				MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
				MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);
				RD_Time_TO_YYTime(pstTime,&stYYTime);
				RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
				u32ReadLen = sizeof(stERD03Data);
				NAND_WriteEventRecord(pu8MeterId,0xE2010009,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
				pstEvent_Info->u8EventFlag = 0xFF;
			}	
		}		
	}	
}

//err20181105 ���Ӷ����¼�
/**--------------------------------------------------------------------
*��    ��:�¼���¼������A������¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E201000B(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_EVENT_E201000B_0D_LIST_S* pstEvent_List = (QCSG_EVENT_E201000B_0D_LIST_S*)QCSG_EVENT_E201000B_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	//ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32ReadLen = 0;
	u32 u32Ia = 0,u32In = 0,u32Ua = 0,u32Un = 0;
	
	if((QCSG_CheckEventMask(0xE201000B) != TRUE))
	{
		return;
	}
	//u32CurrTick = HAL_GetSystemTick();
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}


	u8PnNum = pstEvent_List->u8PnNum; 
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E201000B_0D_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstEvent_List->stERD3Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E201000B_0D_LIST_S)); 	
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum ++;
		}
		return;
	}
	

	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	MemCopy(&u32Ua,&pstRam_Data->u8Data0201FF00[0],2);
	u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);

	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ia * 1000) > (5 * u32In))
		{
			return;
		}
		if((u32Ua * 10000) < (u32Un * u16UpLimitValue)) //�ж��¼�����
		{
			//��ʱ����¼�¼���ֻ�Ǽ�¼�¼�����ʱ ʱ�����������
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;	
		}		
		
	}	
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ua * 10000) > (u32Un * u16LowLimitValue)) //�ж��¼��ָ�
		{
			//��ʱ��¼�¼�����ʱ ʱ�����������
			MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);

			//��¼�¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
			u32ReadLen = sizeof(stERD03Data);
			NAND_WriteEventRecord(pu8MeterId,0xE201000B,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
			pstEvent_Info->u8EventFlag = 0xFF;
		}	
	}	
}

//err20181105 ���Ӷ����¼�
/**--------------------------------------------------------------------
*��    ��:�¼���¼������B������¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E201000C(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_EVENT_E201000B_0D_LIST_S* pstEvent_List = (QCSG_EVENT_E201000B_0D_LIST_S*)QCSG_EVENT_E201000C_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	//ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32ReadLen = 0;
	u32 u32Ib = 0,u32In = 0,u32Ub = 0,u32Un = 0;
	
	if((QCSG_CheckEventMask(0xE201000C) != TRUE))
	{
		return;
	}
	//u32CurrTick = HAL_GetSystemTick();
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}


	u8PnNum = pstEvent_List->u8PnNum; 
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E201000B_0D_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstEvent_List->stERD3Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E201000B_0D_LIST_S)); 	
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum ++;
		}
		return;
	}
	

	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
	MemCopy(&u32Ub,&pstRam_Data->u8Data0201FF00[2],2);
	u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);

	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ib * 1000) > (5 * u32In))
		{
			return;
		}
		if((u32Ub * 10000) < (u32Un * u16UpLimitValue)) //�ж��¼�����
		{
			//��ʱ����¼�¼���ֻ�Ǽ�¼�¼�����ʱ ʱ�����������
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;	
		}		
		
	}	
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Ub * 10000) > (u32Un * u16LowLimitValue)) //�ж��¼��ָ�
		{
			//��ʱ��¼�¼�����ʱ ʱ�����������
			MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);

			//��¼�¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
			u32ReadLen = sizeof(stERD03Data);
			NAND_WriteEventRecord(pu8MeterId,0xE201000C,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
			pstEvent_Info->u8EventFlag = 0xFF;
		}	
	}	
}

//err20181105 ���Ӷ����¼�
/**--------------------------------------------------------------------
*��    ��:�¼���¼������C������¼,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E201000D(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ�������ݿ�0x0202FF00��
	 */
	QCSG_EVENT_E201000B_0D_LIST_S* pstEvent_List = (QCSG_EVENT_E201000B_0D_LIST_S*)QCSG_EVENT_E201000D_LIST;
	QCSG_EVENT_ERD3_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	//ThresholdParaStruct stThresholdPara;
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
	RealTimeYYStruct stYYTime;
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8Meterphase = 0x04,u8CtrlCode = 0,i = 0;
	u16 u16UpLimitValue = 0,u16LowLimitValue = 0;
	u32 u32ReadLen = 0;
	u32 u32Ic = 0,u32In = 0,u32Uc = 0,u32Un = 0;
	
	if((QCSG_CheckEventMask(0xE201000D) != TRUE))
	{
		return;
	}
	//u32CurrTick = HAL_GetSystemTick();
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstRam_Data->u8Data0201FF00[0],6) != TRUE)
	{
		return;
	}


	u8PnNum = pstEvent_List->u8PnNum; 
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E201000B_0D_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstEvent_List->stERD3Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}	
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E201000B_0D_LIST_S)); 	
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstEvent_List->stERD3Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stERD3Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			pstEvent_Info->u8EventFlag = 0xFF;
			pstEvent_List->u8PnNum ++;
		}
		return;
	}
	

	//��ȡԽ�޷�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE080010C,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
	}
	else
	{
		MemCopy(&u16UpLimitValue,&stPowerQualityPara.E080010C[0],2);
		MemCopy(&u16LowLimitValue,&stPowerQualityPara.E080010C[1],2);
		if(QCSG_BCDCheck((u8*)&u16UpLimitValue,2) != TRUE)
		{
			u16UpLimitValue = QCSG_BCD2HEX(0x78) * 100;
		}
		else
		{
			u16UpLimitValue = QCSG_BCD2HEX((u8)(u16UpLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16UpLimitValue);
		}
		if(QCSG_BCDCheck((u8*)&u16LowLimitValue,2) != TRUE)
		{
			u16LowLimitValue = QCSG_BCD2HEX(0x85) * 100;
		}
		else
		{
			u16LowLimitValue = QCSG_BCD2HEX((u8)(u16LowLimitValue>>8)) * 100 + QCSG_BCD2HEX((u8)u16LowLimitValue);
		}
	}
	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u8Meterphase = 0x04;
		u32In = JC_IN;
		u32Un = JC_UN;
	}
	else
	{
		u8Meterphase = stPnPara.ConnectionType;
		if(u8Meterphase > 0x04)
		{
			u8Meterphase = 0x04;
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
		{
			u32In = JC_IN;
		}
		else
		{
			u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);		
		}
		if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
		{
			u32Un = JC_UN;
		}
		else
		{
			u32Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
		}			
	}
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u32Uc,&pstRam_Data->u8Data0201FF00[4],2);
	u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);

	if(pstEvent_Info->u8EventFlag == 0xFF)
	{
		if((u32Ic * 1000) > (5 * u32In))
		{
			return;
		}
		if((u32Uc * 10000) < (u32Un * u16UpLimitValue)) //�ж��¼�����
		{
			//��ʱ����¼�¼���ֻ�Ǽ�¼�¼�����ʱ ʱ�����������
			MemCopy(&pstEvent_Info->stStartTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8Start00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8Start00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8Start00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8Start00040000,pstRam_Data->u8Data00040000,4);	
			pstEvent_Info->u8EventFlag = 0x01;	
		}		
		
	}	
	if(pstEvent_Info->u8EventFlag == 0x01)
	{
		if((u32Uc * 10000) > (u32Un * u16LowLimitValue)) //�ж��¼��ָ�
		{
			//��ʱ��¼�¼�����ʱ ʱ�����������
			MemCopy(&pstEvent_Info->stEndTime,pstTime,sizeof(TIME_PARAM_S));
			MemCopy(pstEvent_Info->u8End00010000,pstRam_Data->u8Data00010000,4);
			MemCopy(pstEvent_Info->u8End00020000,pstRam_Data->u8Data00020000,4);
			MemCopy(pstEvent_Info->u8End00030000,pstRam_Data->u8Data00030000,4);
			MemCopy(pstEvent_Info->u8End00040000,pstRam_Data->u8Data00040000,4);

			//��¼�¼�
			MemSet(&stYYTime,0,sizeof(stYYTime));
			RD_Time_TO_YYTime(pstTime,&stYYTime);
			RD_Write_ERD3_Data(pstEvent_Info,&stERD03Data);
			u32ReadLen = sizeof(stERD03Data);
			NAND_WriteEventRecord(pu8MeterId,0xE201000D,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
			pstEvent_Info->u8EventFlag = 0xFF;
		}	
	}	
}

//err20181107 �����¼� Уʱ��¼
/**--------------------------------------------------------------------
*��    ��:�¼���¼������Уʱ��¼�¼�,ERD9
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010010(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡУʱ�ܴ���0x03300400��
	 */
	QCSG_EVENT_E2010010_LIST_S* pstEvent_List = (QCSG_EVENT_E2010010_LIST_S*)QCSG_EVENT_E2010010_LIST;
	QCSG_EVENT_E2010010_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	ERD09Struct stERD09Data;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;

	//QCSG_EVENT_ERD3_INFO_S pstEvent_Info_ERD3;

	//������λ��ֱ�ӶԽ���Уʱ�����ɵ�Уʱ�ܴ�����+1�����Բ����澯��
	//��ֱ����������λ�����ն�Уʱ���򽻲ɵ�ʱ������ն�ͬ�������ǽ����ܴ������䡣
	//�ʽ��ն�Уʱ���¼���¼����AFN04�ĺ����в�����
	//if(u16Pn == 0)
	//{
		//�ն��������������¼�
	//	return;
	//}
	if(QCSG_CheckEventMask(0xE2010010) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E2010010_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstEvent_List->stE2010010Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E2010010_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstEvent_List->stE2010010Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stE2010010Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stE2010010Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			MemCopy(pstEvent_Info->u8Data03300400,&pstDlt645Frame_Info->u8RevBuf[14],3);
			pstEvent_List->u8PnNum ++;
		}
		return;
	}	
	if(memcmp(pstEvent_Info->u8Data03300400,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{
		//��¼�¼�����ʱ ʱ�����������
		MemCopy(&stERD09Data.Time,pstTime,sizeof(TIME_PARAM_S));
		MemCopy(stERD09Data.C_00010000,pstRam_Data->u8Data00010000,4);
		MemCopy(stERD09Data.C_00020000,pstRam_Data->u8Data00020000,4);
		
		//��¼�¼�
		MemSet(&stYYTime,0,sizeof(stYYTime));
		RD_Time_TO_YYTime(pstTime,&stYYTime);
		u32ReadLen = sizeof(stERD09Data);
		NAND_WriteEventRecord(pu8MeterId,0xE2010010,&stYYTime,(u8*)&stERD09Data,&u32ReadLen);
	}
	MemCopy(pstEvent_Info->u8Data03300400,&pstDlt645Frame_Info->u8RevBuf[14],3);

}




//err20181106 �����¼� ����ť�п�����¼
/**--------------------------------------------------------------------
*��    ��:�¼���¼��������ť�п����¼�,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010011(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 * TODO:�뼼���淶һ�£��������ڶ�ȡ��ť�п��Ǵ���0x03300E00��
	 * err20190509 �����¼���������Ҫ�����ӳ�����һ�ζ�ť�п����¼���¼0x03300E01.
	 * ���Խ���ť�п��������������洢���ڱ����¼���¼������ɺ����жϲ����¼�
	 */
	QCSG_EVENT_E2010011_LIST_S* pstEvent_List = (QCSG_EVENT_E2010011_LIST_S*)QCSG_EVENT_E2010011_LIST;
	QCSG_EVENT_E2010011_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	//ARD11Struct stARD11Data;	
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;

	QCSG_EVENT_ERD3_INFO_S pstEvent_Info_ERD3;
	//u32 u32ValueList[6];//err20180925 ���ӿ��Ǹ澯�ķ���ǰ���� 
	//u8 u8NewData03300E00[3];//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckEventMask(0xE2010011) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E2010011_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstEvent_List->stE2010011Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E2010011_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstEvent_List->stE2010011Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stE2010011Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stE2010011Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			MemCopy(pstEvent_Info->u8Data03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3);
			pstEvent_List->u8PnNum ++;
		}
		return;
	}	
	if(memcmp(pstEvent_Info->u8Data03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{
		//��¼�¼�����ʱ ʱ�����������
		MemCopy(&pstEvent_Info_ERD3.stStartTime,pstTime,sizeof(TIME_PARAM_S));
		MemCopy(pstEvent_Info_ERD3.u8Start00010000,pstRam_Data->u8Data00010000,4);
		MemCopy(pstEvent_Info_ERD3.u8Start00020000,pstRam_Data->u8Data00020000,4);
		MemCopy(pstEvent_Info_ERD3.u8Start00030000,pstRam_Data->u8Data00030000,4);
		MemCopy(pstEvent_Info_ERD3.u8Start00040000,pstRam_Data->u8Data00040000,4);	

		//��ʱ��¼�¼�����ʱ ʱ�����������
		MemCopy(&pstEvent_Info_ERD3.stEndTime,pstTime,sizeof(TIME_PARAM_S));
		MemCopy(pstEvent_Info_ERD3.u8End00010000,pstRam_Data->u8Data00010000,4);
		MemCopy(pstEvent_Info_ERD3.u8End00020000,pstRam_Data->u8Data00020000,4);
		MemCopy(pstEvent_Info_ERD3.u8End00030000,pstRam_Data->u8Data00030000,4);
		MemCopy(pstEvent_Info_ERD3.u8End00040000,pstRam_Data->u8Data00040000,4);
		
		//��¼�¼�
		MemSet(&stYYTime,0,sizeof(stYYTime));
		RD_Time_TO_YYTime(pstTime,&stYYTime);
		RD_Write_ERD3_Data(&pstEvent_Info_ERD3,&stERD03Data);
		u32ReadLen = sizeof(stERD03Data);
		NAND_WriteEventRecord(pu8MeterId,0xE2010011,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
	}
	MemCopy(pstEvent_Info->u8Data03300E00,&pstDlt645Frame_Info->u8RevBuf[14],3);

}

//err20181107 �����¼� ���ǿ�����¼
/**--------------------------------------------------------------------
*��    ��:�¼���¼���������ǿ�����¼�¼�,ERD3
*�������:pstTime��u16Pn��pu8MeterId��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Event_E2010012(TIME_PARAM_S* pstTime,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	/*
	 *	TODO:�뼼���淶һ�£��������ڶ�ȡ���ǿ�������0x03300D00��
	 */
	QCSG_EVENT_E2010012_LIST_S* pstEvent_List = (QCSG_EVENT_E2010012_LIST_S*)QCSG_EVENT_E2010012_LIST;
	QCSG_EVENT_E2010012_INFO_S* pstEvent_Info = NULL;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	RealTimeYYStruct stYYTime;
	//ARD11Struct stARD11Data;	
	ERD03Struct stERD03Data;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32ReadLen = 0;

	QCSG_EVENT_ERD3_INFO_S pstEvent_Info_ERD3;

	
	if(u16Pn == 0)
	{
		//�ն��������������¼�
		return;
	}
	if(QCSG_CheckEventMask(0xE2010012) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	u8PnNum = pstEvent_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E2010012_LIST_S));
	}
	for(i = 0; i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstEvent_List->stE2010012Data[i].u16Pn,1) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstEvent_List,0,sizeof(QCSG_EVENT_E2010012_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstEvent_List->stE2010012Data[i].u16Pn)
		{
			pstEvent_Info = &pstEvent_List->stE2010012Data[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEvent_Info = &pstEvent_List->stE2010012Data[u8PnNum];
			pstEvent_Info->u16Pn = u16Pn;
			MemCopy(pstEvent_Info->u8Data03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3);
			pstEvent_List->u8PnNum ++;
		}
		return;
	}	
	if(memcmp(pstEvent_Info->u8Data03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3) != 0)
	{
		//��¼�¼�����ʱ ʱ�����������
		MemCopy(&pstEvent_Info_ERD3.stStartTime,pstTime,sizeof(TIME_PARAM_S));
		MemCopy(pstEvent_Info_ERD3.u8Start00010000,pstRam_Data->u8Data00010000,4);
		MemCopy(pstEvent_Info_ERD3.u8Start00020000,pstRam_Data->u8Data00020000,4);
		MemCopy(pstEvent_Info_ERD3.u8Start00030000,pstRam_Data->u8Data00030000,4);
		MemCopy(pstEvent_Info_ERD3.u8Start00040000,pstRam_Data->u8Data00040000,4);	

		//��ʱ��¼�¼�����ʱ ʱ�����������
		MemCopy(&pstEvent_Info_ERD3.stEndTime,pstTime,sizeof(TIME_PARAM_S));
		MemCopy(pstEvent_Info_ERD3.u8End00010000,pstRam_Data->u8Data00010000,4);
		MemCopy(pstEvent_Info_ERD3.u8End00020000,pstRam_Data->u8Data00020000,4);
		MemCopy(pstEvent_Info_ERD3.u8End00030000,pstRam_Data->u8Data00030000,4);
		MemCopy(pstEvent_Info_ERD3.u8End00040000,pstRam_Data->u8Data00040000,4);
		
		//��¼�¼�
		MemSet(&stYYTime,0,sizeof(stYYTime));
		RD_Time_TO_YYTime(pstTime,&stYYTime);
		RD_Write_ERD3_Data(&pstEvent_Info_ERD3,&stERD03Data);
		u32ReadLen = sizeof(stERD03Data);
		NAND_WriteEventRecord(pu8MeterId,0xE2010012,&stYYTime,(u8*)&stERD03Data,&u32ReadLen);
	}
	MemCopy(pstEvent_Info->u8Data03300D00,&pstDlt645Frame_Info->u8RevBuf[14],3);

}


/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�ͣ��ͳ�ƣ��ն�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000020_21_CL3200(u8 u8Cycle,u16 u16Pn,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8CtrlCode = 0;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16RatedVoltage = 0;	
	u32 u32ReadLen = 0,u32PowerDownNum_Day = 0,u32PowerDownTime_Day = 0,u32PowerDownNum_Month = 0,u32PowerDownTime_Month = 0;
	u32* pu32PowerDownNum_Day = (u32*)QCSG_POWER_DOWN_NUM_DAY;
	u32* pu32PowerDownTime_Day = (u32*)QCSG_POWER_DOWN_TIME_DAY;
	u32* pu32PowerDownNum_Month = (u32*)QCSG_POWER_DOWN_NUM_MONTH;
	u32* pu32PowerDownTime_Month = (u32*)QCSG_POWER_DOWN_TIME_MONTH;
	u32* pu32Flag_Day = (u32*)QCSG_POWER_DOWN_DAY_FLAG;
	u32* pu32Flag_Month = (u32*)QCSG_POWER_DOWN_MONTH_FLAG;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE)
	{
		return;
	}
	MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);	
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	if((u16Ua * 100 < u16RatedVoltage * 60)&&(u16Ub * 100 < u16RatedVoltage * 60)&&(u16Uc * 100 < u16RatedVoltage * 60))
	{
		u32PowerDownTime_Day = *pu32PowerDownTime_Day;
		u32PowerDownTime_Month = *pu32PowerDownTime_Month;	
		u32PowerDownTime_Day &= 0xFFFFFF;
		u32PowerDownTime_Month &= 0xFFFFFF;
		u32PowerDownTime_Day += u8Cycle;
		u32PowerDownTime_Month += u8Cycle;	
		(*pu32PowerDownTime_Day) = u32PowerDownTime_Day;		
		(*pu32PowerDownTime_Month) = u32PowerDownTime_Month;	
		if((*pu32Flag_Day) == 0x00)
		{
			(*pu32Flag_Day) = 1;
			u32PowerDownNum_Day	= *pu32PowerDownNum_Day;
			u32PowerDownNum_Day &= 0xFFFFFF;
			u32PowerDownNum_Day ++;
			(*pu32PowerDownNum_Day) = u32PowerDownNum_Day;	
		}
		if((*pu32Flag_Month) == 0x00)
		{
			(*pu32Flag_Month) = 1;
			u32PowerDownNum_Month = *pu32PowerDownNum_Month;
			u32PowerDownNum_Month &= 0xFFFFFF;
			u32PowerDownNum_Month ++;		
			(*pu32PowerDownNum_Month) = u32PowerDownNum_Month;		
		}
	}
	else
	{
		(*pu32Flag_Day) = 0;
		(*pu32Flag_Month) = 0;		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�ͣ��ͳ�ƣ��ն�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000020_21(u8 u8Cycle,u16 u16Pn,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8CtrlCode = 0,u8PnNum = 0,i = 0;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Un = 0;	
	u32 u32Ia = 0,u32Ib = 0,u32Ic = 0,u32In = 0;
	u32 u32ReadLen = 0,u32PowerDownNum_Day = 0,u32PowerDownTime_Day = 0,u32PowerDownNum_Month = 0,u32PowerDownTime_Month = 0;
	u32* pu32PowerDownNum_Day = (u32*)QCSG_POWER_DOWN_NUM_DAY;
	u32* pu32PowerDownTime_Day = (u32*)QCSG_POWER_DOWN_TIME_DAY;
	u32* pu32PowerDownNum_Month = (u32*)QCSG_POWER_DOWN_NUM_MONTH;
	u32* pu32PowerDownTime_Month = (u32*)QCSG_POWER_DOWN_TIME_MONTH;
	u32* pu32Flag_Day = (u32*)QCSG_POWER_DOWN_DAY_FLAG;
	u32* pu32Flag_Month = (u32*)QCSG_POWER_DOWN_MONTH_FLAG;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(u16Pn != 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE)
	{
		return;
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
	MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);	
	u32Ia = u32Ia & 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	u32Ib = u32Ib & 0x007FFFFF;
	u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);	
	u32Ic = u32Ic & 0x007FFFFF;
	u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	MemCopy(&u16Ua,&pstRam_Data->u8Data0201FF00[0],2);
	MemCopy(&u16Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u16Uc,&pstRam_Data->u8Data0201FF00[4],2);	
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		u32In = JC_IN;
		u16Un = JC_UN;
	}
	if(QCSG_BCDCheck((u8*)&stPnPara.RatedCurrent,3) != TRUE)
	{
		u32In = JC_IN;
	}
	else
	{
		u32In = QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(stPnPara.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedCurrent);	
	}
	if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
	{
		u16Un = JC_UN;
	}
	else
	{			
		u16Un = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
	}
	if((u16Ua * 100 < u16Un * 60)&&(u16Ub * 100 < u16Un * 60)&&(u16Uc * 100 < u16Un * 60)
	 &&(u32Ia * 1000 < u32In * 5)&&(u32Ib * 1000 < u32In * 5)&&(u32Ic * 1000 < u32In * 5))
	{
		u32PowerDownTime_Day = *pu32PowerDownTime_Day;
		u32PowerDownTime_Month = *pu32PowerDownTime_Month;	
		u32PowerDownTime_Day &= 0xFFFFFF;
		u32PowerDownTime_Month &= 0xFFFFFF;
		u32PowerDownTime_Day += u8Cycle;
		u32PowerDownTime_Month += u8Cycle;	
		(*pu32PowerDownTime_Day) = u32PowerDownTime_Day;		
		(*pu32PowerDownTime_Month) = u32PowerDownTime_Month;	
		if((*pu32Flag_Day) == 0x00)
		{
			(*pu32Flag_Day) = 1;
			u32PowerDownNum_Day	= *pu32PowerDownNum_Day;
			u32PowerDownNum_Day &= 0xFFFFFF;
			u32PowerDownNum_Day ++;
			(*pu32PowerDownNum_Day) = u32PowerDownNum_Day;	
		}
		if((*pu32Flag_Month) == 0x00)
		{
			(*pu32Flag_Month) = 1;
			u32PowerDownNum_Month = *pu32PowerDownNum_Month;
			u32PowerDownNum_Month &= 0xFFFFFF;
			u32PowerDownNum_Month ++;		
			(*pu32PowerDownNum_Month) = u32PowerDownNum_Month;		
		}
	}
	else
	{
		(*pu32Flag_Day) = 0;
		(*pu32Flag_Month) = 0;
	}
}
#if (DLT500_REPORT == 0)
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000010_97(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = NULL;	
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}		
	if(QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) != TRUE) && ((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	//���ݺϷ���У��
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstRam_Data->u8Data0201FF00[0],2);
	MemCopy(&u16Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u16Uc,&pstRam_Data->u8Data0201FF00[4],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	if(u8MeterType == 3)
	{
		if((u16Ua * 100 < u16RatedVoltage * 78) || (u16Ub * 100 < u16RatedVoltage * 78) || (u16Uc * 100 < u16RatedVoltage * 78))
		{
			u8MonitorFlag = FALSE;
		}
		if((u16Ua * 100 > u16RatedVoltage * 130) || (u16Ub * 100 > u16RatedVoltage * 130) || (u16Uc * 100 > u16RatedVoltage * 130))
		{
			u8MonitorFlag = FALSE;
		}
	}
	if(u8MeterType == 1)
	{
		if(u16Ua * 100 < u16RatedVoltage * 78)
		{
			u8MonitorFlag = FALSE;
		}
		if(u16Ua * 100 > u16RatedVoltage * 130)
		{
			u8MonitorFlag = FALSE;
		}		
	}
	if(u8MonitorFlag == TRUE)
	{
		u32MonitorTime += u8Cycle;
	}
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32PassRateTime += u8Cycle;
			}
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32LowLimitTime += u8Cycle;
			}
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32UpLimitTime += u8Cycle;
			}
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32PassRateTime += u8Cycle;
			}
		}
		if(u16Ua < u16LowLimit)
		{
			if(u8MonitorFlag == TRUE)
			{
				u32LowLimitTime += u8Cycle;
			}
		}
		if(u16Ua > u16UpLimit)
		{
			if(u8MonitorFlag == TRUE)
			{
				u32UpLimitTime += u8Cycle;
			}
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);	
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000010(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE) && ((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	//���ݺϷ���У��
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u8MeterType == 3)
	{
		if((u16Ua * 100 < u16RatedVoltage * 78) || (u16Ub * 100 < u16RatedVoltage * 78) || (u16Uc * 100 < u16RatedVoltage * 78))
		{
			u8MonitorFlag = FALSE;
		}
		if((u16Ua * 100 > u16RatedVoltage * 130) || (u16Ub * 100 > u16RatedVoltage * 130) || (u16Uc * 100 > u16RatedVoltage * 130))
		{
			u8MonitorFlag = FALSE;
		}
	}

	if(u8MeterType == 1)
	{
		if(u16Ua * 100 < u16RatedVoltage * 78)
		{
			u8MonitorFlag = FALSE;
		}
		if(u16Ua * 100 > u16RatedVoltage * 130)
		{
			u8MonitorFlag = FALSE;
		}		
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			u32PassRateTime += u8Cycle;
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			u32LowLimitTime += u8Cycle;
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			u32PassRateTime += u8Cycle;
		}
		if(u16Ua < u16LowLimit)
		{
			u32LowLimitTime += u8Cycle;
		}
		if(u16Ua > u16UpLimit)
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);	
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����A���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000011(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	}
	else
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ua = (u16Ua << 4) & 0xFFF0;
	}
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ua * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ua * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}		
	u32MonitorTime += u8Cycle;
	if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ua < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ua > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ua > u16Umax)
	{
		u16Umax = u16Ua;
		u8MaxFlag = 1;
	}
	if(u16Ua < u16Umin)
	{
		u16Umin = u16Ua;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����B���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000012(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ub = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[16],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	}
	else
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ub = (u16Ub << 4) & 0xFFF0;
	}
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ub * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ub * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{		
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ub < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ub > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ub > u16Umax)
	{
		u16Umax = u16Ub;
		u8MaxFlag = 1;
	}
	if(u16Ub < u16Umin)
	{
		u16Umin = u16Ub;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����C���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000013(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;

	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}			
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[18],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	}
	else
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Uc = (u16Uc << 4) & 0xFFF0;
	}
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Uc * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Uc * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{	
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Uc < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Uc > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Uc > u16Umax)
	{
		u16Umax = u16Uc;
		u8MaxFlag = 1;
	}
	if(u16Uc < u16Umin)
	{
		u16Umin = u16Uc;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
#else
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000010_97(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = NULL;	
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}		
	if(QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) != TRUE) && ((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	//���ݺϷ���У��
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = (QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime))*60;
	u32PassRateTime = (QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime))*60;
	u32UpLimitTime = (QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime))*60;
	u32LowLimitTime = (QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime))*60;
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstRam_Data->u8Data0201FF00[0],2);
	MemCopy(&u16Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u16Uc,&pstRam_Data->u8Data0201FF00[4],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	if(u8MeterType == 3)
	{
		if((u16Ua * 100 < u16RatedVoltage * 78) || (u16Ub * 100 < u16RatedVoltage * 78) || (u16Uc * 100 < u16RatedVoltage * 78))
		{
			u8MonitorFlag = FALSE;
		}
		if((u16Ua * 100 > u16RatedVoltage * 130) || (u16Ub * 100 > u16RatedVoltage * 130) || (u16Uc * 100 > u16RatedVoltage * 130))
		{
			u8MonitorFlag = FALSE;
		}
	}
	if(u8MeterType == 1)
	{
		if(u16Ua * 100 < u16RatedVoltage * 78)
		{
			u8MonitorFlag = FALSE;
		}
		if(u16Ua * 100 > u16RatedVoltage * 130)
		{
			u8MonitorFlag = FALSE;
		}		
	}
	if(u8MonitorFlag == TRUE)
	{
		u32MonitorTime += u8Cycle;
	}
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32PassRateTime += u8Cycle;
			}
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32LowLimitTime += u8Cycle;
			}
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32UpLimitTime += u8Cycle;
			}
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			if(u8MonitorFlag == TRUE)
			{
				u32PassRateTime += u8Cycle;
			}
		}
		if(u16Ua < u16LowLimit)
		{
			if(u8MonitorFlag == TRUE)
			{
				u32LowLimitTime += u8Cycle;
			}
		}
		if(u16Ua > u16UpLimit)
		{
			if(u8MonitorFlag == TRUE)
			{
				u32UpLimitTime += u8Cycle;
			}
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);	
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime/60);
	u32LowLimitTime = hex_bcd(u32LowLimitTime/60);
	u32MonitorTime = hex_bcd(u32MonitorTime/60);
	u32PassRateTime = hex_bcd(u32PassRateTime/60);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}

/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000010(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE) && ((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	//���ݺϷ���У��
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			u32PassRateTime += u8Cycle;
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			u32LowLimitTime += u8Cycle;
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			u32PassRateTime += u8Cycle;
		}
		if(u16Ua < u16LowLimit)
		{
			u32LowLimitTime += u8Cycle;
		}
		if(u16Ua > u16UpLimit)
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);	
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate =u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����A���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000011(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	}
	else
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ua = (u16Ua << 4) & 0xFFF0;
	}
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ua * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ua * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}		
	u32MonitorTime += u8Cycle;
	if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ua < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ua > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ua > u16Umax)
	{
		u16Umax = u16Ua;
		u8MaxFlag = 1;
	}
	if(u16Ua < u16Umin)
	{
		u16Umin = u16Ua;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����B���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000012(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ub = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[16],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	}
	else
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ub = (u16Ub << 4) & 0xFFF0;
	}
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ub * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ub * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{		
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ub < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ub > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ub > u16Umax)
	{
		u16Umax = u16Ub;
		u8MaxFlag = 1;
	}
	if(u16Ub < u16Umin)
	{
		u16Umin = u16Ub;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate =  u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����C���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000013(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;

	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}			
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[18],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	}
	else
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Uc = (u16Uc << 4) & 0xFFF0;
	}
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Uc * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Uc * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{	
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Uc < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Uc > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Uc > u16Umax)
	{
		u16Umax = u16Uc;
		u8MaxFlag = 1;
	}
	if(u16Uc < u16Umin)
	{
		u16Umin = u16Uc;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}

#endif
#if (DLT500_REPORT == 0)
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100000_97(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = NULL;	
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}		
	if(QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) != TRUE) && ((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	//���ݺϷ���У��
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstRam_Data->u8Data0201FF00[0],2);
	MemCopy(&u16Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u16Uc,&pstRam_Data->u8Data0201FF00[4],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u8MeterType == 3)
	{
		if((u16Ua * 100 < u16RatedVoltage * 78) || (u16Ub * 100 < u16RatedVoltage * 78) || (u16Uc * 100 < u16RatedVoltage * 78))
		{
			u8MonitorFlag = FALSE;
		}
		if((u16Ua * 100 > u16RatedVoltage * 130) || (u16Ub * 100 > u16RatedVoltage * 130) || (u16Uc * 100 > u16RatedVoltage * 130))
		{
			u8MonitorFlag = FALSE;
		}
	}
	if(u8MeterType == 1)
	{
		if(u16Ua * 100 < u16RatedVoltage * 78)
		{
			u8MonitorFlag = FALSE;
		}
		if(u16Ua * 100 > u16RatedVoltage * 130)
		{
			u8MonitorFlag = FALSE;
		}		
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			u32PassRateTime += u8Cycle;
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			u32LowLimitTime += u8Cycle;
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			u32PassRateTime += u8Cycle;
		}
		if(u16Ua < u16LowLimit)
		{
			u32LowLimitTime += u8Cycle;
		}
		if(u16Ua > u16UpLimit)
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);	
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100000(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;

	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE) && ((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u8MeterType == 3)
	{
		if((u16Ua * 100 < u16RatedVoltage * 78) || (u16Ub * 100 < u16RatedVoltage * 78) || (u16Uc * 100 < u16RatedVoltage * 78))
		{
			u8MonitorFlag = FALSE;
		}
		if((u16Ua * 100 > u16RatedVoltage * 130) || (u16Ub * 100 > u16RatedVoltage * 130) || (u16Uc * 100 > u16RatedVoltage * 130))
		{
			u8MonitorFlag = FALSE;
		}
	}
	if(u8MeterType == 1)
	{
		if(u16Ua * 100 < u16RatedVoltage * 78)
		{
			u8MonitorFlag = FALSE;
		}
		if(u16Ua * 100 > u16RatedVoltage * 130)
		{
			u8MonitorFlag = FALSE;
		}		
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			u32PassRateTime += u8Cycle;
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			u32LowLimitTime += u8Cycle;
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			u32PassRateTime += u8Cycle;
		}
		if(u16Ua < u16LowLimit)
		{
			u32LowLimitTime += u8Cycle;
		}
		if(u16Ua > u16UpLimit)
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);			
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����A���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100100(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	}
	else
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ua = (u16Ua << 4) & 0xFFF0;
	}
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ua * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ua * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ua < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ua > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ua > u16Umax)
	{
		u16Umax = u16Ua;
		u8MaxFlag = 1;
	}
	if(u16Ua < u16Umin)
	{
		u16Umin = u16Ua;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����B���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100200(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ub = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[16],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	}
	else
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ub = (u16Ub << 4) & 0xFFF0;
	}
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ub * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ub * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{		
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ub < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ub > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ub > u16Umax)
	{
		u16Umax = u16Ub;
		u8MaxFlag = 1;
	}
	if(u16Ub < u16Umin)
	{
		u16Umin = u16Ub;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����C���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100300(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32Overrrun = 0,u32UpLimitTime = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[18],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	}
	else
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Uc = (u16Uc << 4) & 0xFFF0;
	}
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Uc * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Uc * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{		
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Uc < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Uc > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Uc > u16Umax)
	{
		u16Umax = u16Uc;
		u8MaxFlag = 1;
	}
	if(u16Uc < u16Umin)
	{
		u16Umin = u16Uc;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32Overrrun = (u32MonitorTime - u32PassRateTime) * 10000/u32MonitorTime;
	u32Overrrun = hex_bcd(u32Overrrun);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8Overrrun,&u32Overrrun,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);		
}
#else
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100000_97(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = NULL;	
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}		
	if(QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,6) != TRUE) && ((QCSG_BCDCheck(pstRam_Data->u8Data0201FF00,2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	//���ݺϷ���У��
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = (QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime))*60;
	u32PassRateTime = (QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime))*60;
	u32UpLimitTime = (QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime))*60;
	u32LowLimitTime = (QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime))*60;
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstRam_Data->u8Data0201FF00[0],2);
	MemCopy(&u16Ub,&pstRam_Data->u8Data0201FF00[2],2);
	MemCopy(&u16Uc,&pstRam_Data->u8Data0201FF00[4],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u8MeterType == 3)
	{
		if((u16Ua * 100 < u16RatedVoltage * 78) || (u16Ub * 100 < u16RatedVoltage * 78) || (u16Uc * 100 < u16RatedVoltage * 78))
		{
			u8MonitorFlag = FALSE;
		}
		if((u16Ua * 100 > u16RatedVoltage * 130) || (u16Ub * 100 > u16RatedVoltage * 130) || (u16Uc * 100 > u16RatedVoltage * 130))
		{
			u8MonitorFlag = FALSE;
		}
	}
	if(u8MeterType == 1)
	{
		if(u16Ua * 100 < u16RatedVoltage * 78)
		{
			u8MonitorFlag = FALSE;
		}
		if(u16Ua * 100 > u16RatedVoltage * 130)
		{
			u8MonitorFlag = FALSE;
		}		
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			u32PassRateTime += u8Cycle;
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			u32LowLimitTime += u8Cycle;
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			u32PassRateTime += u8Cycle;
		}
		if(u16Ua < u16LowLimit)
		{
			u32LowLimitTime += u8Cycle;
		}
		if(u16Ua > u16UpLimit)
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);	
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime/60);
	u32LowLimitTime = hex_bcd(u32LowLimitTime/60);
	u32MonitorTime = hex_bcd(u32MonitorTime/60);
	u32PassRateTime = hex_bcd(u32PassRateTime/60);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}

/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100000(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MeterType = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Ub = 0,u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;

	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],6) != TRUE) && ((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if(u8MeterType == 3)
	{
		if(((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit)) && ((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit)) && ((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit)))
		{
			u32PassRateTime += u8Cycle;
		}
		if((u16Ua < u16LowLimit) || (u16Ub < u16LowLimit) || (u16Uc < u16LowLimit))
		{
			u32LowLimitTime += u8Cycle;
		}
		if((u16Ua > u16UpLimit) || (u16Ub > u16UpLimit) || (u16Uc > u16UpLimit))
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ub > u16Umax)
		{
			u16Umax = u16Ub;
			u8MaxFlag = 1;
		}
		if(u16Uc > u16Umax)
		{
			u16Umax = u16Uc;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
		if(u16Ub < u16Umin)
		{
			u16Umin = u16Ub;
			u8MinFlag = 1;
		}	
		if(u16Uc < u16Umin)
		{
			u16Umin = u16Uc;
			u8MinFlag = 1;
		}	
	}
	if(u8MeterType == 1)
	{
		if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
		{
			u32PassRateTime += u8Cycle;
		}
		if(u16Ua < u16LowLimit)
		{
			u32LowLimitTime += u8Cycle;
		}
		if(u16Ua > u16UpLimit)
		{
			u32UpLimitTime += u8Cycle;
		}
		if(u16Ua > u16Umax)
		{
			u16Umax = u16Ua;
			u8MaxFlag = 1;
		}
		if(u16Ua < u16Umin)
		{
			u16Umin = u16Ua;
			u8MinFlag = 1;
		}
	}	
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);			
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����A���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100100(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ua = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[14],2);
	}
	else
	{
		MemCopy(&u16Ua,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ua = (u16Ua << 4) & 0xFFF0;
	}
	u16Ua = QCSG_BCD2HEX((u8)(u16Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ua);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ua * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ua * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Ua < u16UpLimit) && (u16Ua > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ua < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ua > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ua > u16Umax)
	{
		u16Umax = u16Ua;
		u8MaxFlag = 1;
	}
	if(u16Ua < u16Umin)
	{
		u16Umin = u16Ua;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����B���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100200(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Ub = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[16],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[16],2);
	}
	else
	{
		MemCopy(&u16Ub,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Ub = (u16Ub << 4) & 0xFFF0;
	}
	u16Ub = QCSG_BCD2HEX((u8)(u16Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u16Ub);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Ub * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Ub * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{		
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Ub < u16UpLimit) && (u16Ub > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Ub < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Ub > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Ub > u16Umax)
	{
		u16Umax = u16Ub;
		u8MaxFlag = 1;
	}
	if(u16Ub < u16Umin)
	{
		u16Umin = u16Ub;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����C���ѹ�ϸ���ͳ�����ݣ��ն˼����ܱ�֧��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_03100300(u8 u8Cycle,u16 u16UpLimit,u16 u16LowLimit,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	TIME_PARAM_S stCurr_Time;
	MeasurePointStruct stPnPara;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MaxFlag = 0,u8MinFlag = 0,u8MonitorFlag = TRUE;
	u16 u16Uc = 0,u16Umax = 0,u16Umin = 0,u16RatedVoltage = 0;
	u32 u32MonitorTime = 0,u32PassRateTime = 0,u32PassRate = 0,u32UpLimitRate = 0,u32UpLimitTime = 0,u32LowLimitRate = 0,u32LowLimitTime = 0,u32ReadLen = 0;
	
	if(u8Cycle == 0)
	{
		return;
	}	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if((u8CtrlCode != 0x91) && (u8CtrlCode != 0x81))
	{
		return;
	}		
	if(u8ProtocolType == METER_DLT645_07)
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[18],2) != TRUE)
		{
			return;
		}
	}
	else
	{
		if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[12],2) != TRUE)
		{
			return;
		}		
	}
	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPassRate_List->stPassRate[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPassRate_List->stPassRate[i].u8MonitorTime,sizeof(QCSG_VOLTAGE_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));		
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[u8PnNum];
			MemSet(pstPassRate_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPassRate_info->u16Pn = u16Pn;
			pstPassRate_List->u8PnNum ++;
		}
	}
	MemCopy(&u32MonitorTime,pstPassRate_info->u8MonitorTime,3);
	MemCopy(&u32PassRateTime,pstPassRate_info->u8PassRateTime,3);
	MemCopy(&u32UpLimitTime,pstPassRate_info->u8UpLimitTime,3);
	MemCopy(&u32LowLimitTime,pstPassRate_info->u8LowLimitTime,3);
	u32MonitorTime = QCSG_BCD2HEX((u8)(u32MonitorTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32MonitorTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32MonitorTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32MonitorTime);
	u32PassRateTime = QCSG_BCD2HEX((u8)(u32PassRateTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32PassRateTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32PassRateTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32PassRateTime);
	u32UpLimitTime = QCSG_BCD2HEX((u8)(u32UpLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32UpLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32UpLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32UpLimitTime);
	u32LowLimitTime = QCSG_BCD2HEX((u8)(u32LowLimitTime>>24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LowLimitTime>>16)) * 10000
					+QCSG_BCD2HEX((u8)(u32LowLimitTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32LowLimitTime);
	MemCopy(&u16Umax,pstPassRate_info->u8MaxVoltage,2);
	MemCopy(&u16Umin,pstPassRate_info->u8MinVoltage,2);
	if(u16Umin == 0x0000)
	{
		u16Umin = 0x9999;
	}	
	u16Umax = QCSG_BCD2HEX((u8)(u16Umax>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umax);
	u16Umin = QCSG_BCD2HEX((u8)(u16Umin>>8)) * 100 + QCSG_BCD2HEX((u8)u16Umin);
	if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[18],2);
	}
	else
	{
		MemCopy(&u16Uc,&pstDlt645Frame_Info->u8RevBuf[12],2);
		u16Uc = (u16Uc << 4) & 0xFFF0;
	}
	u16Uc = QCSG_BCD2HEX((u8)(u16Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u16Uc);
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	u16RatedVoltage = stPnPara.RatedVoltage;
	if(QCSG_BCDCheck((u8*)&u16RatedVoltage,2) != TRUE)
	{
		u16RatedVoltage = JC_UN;
	}
	else
	{
		u16RatedVoltage = QCSG_BCD2HEX((u8)(u16RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)u16RatedVoltage);
	}
	if(u16Uc * 100 < u16RatedVoltage * 78)
	{
		u8MonitorFlag = FALSE;
	}
	if(u16Uc * 100 > u16RatedVoltage * 130)
	{
		u8MonitorFlag = FALSE;
	}
	if(u8MonitorFlag == FALSE)
	{		
		return;
	}
	u32MonitorTime += u8Cycle;
	if((u16Uc < u16UpLimit) && (u16Uc > u16LowLimit))
	{
		u32PassRateTime += u8Cycle;
	}
	if(u16Uc < u16LowLimit)
	{
		u32LowLimitTime += u8Cycle;
	}
	if(u16Uc > u16UpLimit)
	{
		u32UpLimitTime += u8Cycle;
	}
	if(u16Uc > u16Umax)
	{
		u16Umax = u16Uc;
		u8MaxFlag = 1;
	}
	if(u16Uc < u16Umin)
	{
		u16Umin = u16Uc;
		u8MinFlag = 1;
	}
	u16Umax = (u16)hex_bcd(u16Umax);	
	u16Umin = (u16)hex_bcd(u16Umin);		
	MemCopy(pstPassRate_info->u8MaxVoltage,&u16Umax,2);
	MemCopy(pstPassRate_info->u8MinVoltage,&u16Umin,2);
	if(u8MaxFlag == 1)
	{
		pstPassRate_info->u8MaxVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MaxVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MaxVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MaxVoltageTime[3] = stCurr_Time.u8Month;
	}
	if(u8MinFlag == 1)
	{
		pstPassRate_info->u8MinVoltageTime[0] = stCurr_Time.u8Minute;
		pstPassRate_info->u8MinVoltageTime[1] = stCurr_Time.u8Hour;
		pstPassRate_info->u8MinVoltageTime[2] = stCurr_Time.u8Day;
		pstPassRate_info->u8MinVoltageTime[3] = stCurr_Time.u8Month; 
	}
	u32PassRate = u32PassRateTime * 10000/u32MonitorTime;
	u32PassRate = hex_bcd(u32PassRate);
	u32UpLimitRate = u32UpLimitTime * 10000/u32MonitorTime;
	u32UpLimitRate = hex_bcd(u32UpLimitRate);
	u32LowLimitRate = u32LowLimitTime * 10000/u32MonitorTime;
	u32LowLimitRate = hex_bcd(u32LowLimitRate);
	u32UpLimitTime = hex_bcd(u32UpLimitTime);
	u32LowLimitTime = hex_bcd(u32LowLimitTime);
	u32MonitorTime = hex_bcd(u32MonitorTime);
	u32PassRateTime = hex_bcd(u32PassRateTime);
	MemCopy(pstPassRate_info->u8PassRate,&u32PassRate,3);
	MemCopy(pstPassRate_info->u8UpLimintRate,&u32UpLimitRate,3);
	MemCopy(pstPassRate_info->u8LowLimintRate,&u32LowLimitRate,3);
	MemCopy(pstPassRate_info->u8MonitorTime,&u32MonitorTime,3);
	MemCopy(pstPassRate_info->u8PassRateTime,&u32PassRateTime,3);
	MemCopy(pstPassRate_info->u8LowLimitTime,&u32LowLimitTime,3);
	MemCopy(pstPassRate_info->u8UpLimitTime,&u32UpLimitTime,3);	
}

#endif
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�����ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000014_16(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_CURRENT_EXTREME_PASSRATE_LIST_S* pstCurrentExtreme_List = (QCSG_CURRENT_EXTREME_PASSRATE_LIST_S*)QCSG_CURRENT_EXTREME_DAY;
	QCSG_CURRENT_EXTREME_PASSRATE_S* pstCurrentExtreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MeterType = 0;
	u32 u32Ia = 0,u32Ib = 0,u32Ic = 0,u32Imax = 0,u32Imin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE) && ((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstCurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstCurrentExtreme_List,0,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstCurrentExtreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstCurrentExtreme_List->stCurrentExtreme[i].u8Max_A_Current,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstCurrentExtreme_List,0,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstCurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pstCurrentExtreme_info = &pstCurrentExtreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstCurrentExtreme_info = &pstCurrentExtreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pstCurrentExtreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstCurrentExtreme_info->u16Pn = u16Pn;
			pstCurrentExtreme_List->u8PnNum ++;
		}
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia &= 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	if(u8MeterType == 3)
	{
		MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
		MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
		u32Ib &= 0x007FFFFF;
		u32Ic &= 0x007FFFFF;		
		u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
		u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	}
	MemCopy(&u32Imax,pstCurrentExtreme_info->u8Max_A_Current,3);
	MemCopy(&u32Imin,pstCurrentExtreme_info->u8Min_A_Current,3);
	if(u32Imin == 0)
	{
		u32Imin = 0x799999;
	}			
	u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
	u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
	if(u32Ia > u32Imax)
	{
		MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
		u32Ia &= 0x007FFFFF;
		MemCopy(pstCurrentExtreme_info->u8Max_A_Current,&u32Ia,3);
		pstCurrentExtreme_info->u8Max_A_Current_Time[0] = stCurr_Time.u8Minute;
		pstCurrentExtreme_info->u8Max_A_Current_Time[1] = stCurr_Time.u8Hour;
		pstCurrentExtreme_info->u8Max_A_Current_Time[2] = stCurr_Time.u8Day;
		pstCurrentExtreme_info->u8Max_A_Current_Time[3] = stCurr_Time.u8Month;
	}
	if(u32Ia < u32Imin)
	{
		MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
		u32Ia &= 0x007FFFFF;
		MemCopy(pstCurrentExtreme_info->u8Min_A_Current,&u32Ia,3);
		pstCurrentExtreme_info->u8Min_A_Current_Time[0] = stCurr_Time.u8Minute;
		pstCurrentExtreme_info->u8Min_A_Current_Time[1] = stCurr_Time.u8Hour;
		pstCurrentExtreme_info->u8Min_A_Current_Time[2] = stCurr_Time.u8Day;
		pstCurrentExtreme_info->u8Min_A_Current_Time[3] = stCurr_Time.u8Month;		
	}
	if(u8MeterType == 3)
	{	
		MemCopy(&u32Imax,pstCurrentExtreme_info->u8Max_B_Current,3);
		MemCopy(&u32Imin,pstCurrentExtreme_info->u8Min_B_Current,3);
		u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
		u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
		if(u32Imin == 0)
		{
			u32Imin = 0x799999;
		}			
		if(u32Ib > u32Imax)
		{
			MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
			u32Ib &= 0x007FFFFF;		
			MemCopy(pstCurrentExtreme_info->u8Max_B_Current,&u32Ib,3);
			pstCurrentExtreme_info->u8Max_B_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Max_B_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Max_B_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Max_B_Current_Time[3] = stCurr_Time.u8Month;
		}
		if(u32Ib < u32Imin)
		{
			MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
			u32Ib &= 0x007FFFFF;
			MemCopy(pstCurrentExtreme_info->u8Min_B_Current,&u32Ib,3);
			pstCurrentExtreme_info->u8Min_B_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Min_B_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Min_B_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Min_B_Current_Time[3] = stCurr_Time.u8Month;		
		}
		
		MemCopy(&u32Imax,pstCurrentExtreme_info->u8Max_C_Current,3);
		MemCopy(&u32Imin,pstCurrentExtreme_info->u8Min_C_Current,3);
		u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
		u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
		if(u32Imin == 0)
		{
			u32Imin = 0x799999;
		}			
		if(u32Ic > u32Imax)
		{
			MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
			u32Ic &= 0x007FFFFF;		
			MemCopy(pstCurrentExtreme_info->u8Max_C_Current,&u32Ic,3);
			pstCurrentExtreme_info->u8Max_C_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Max_C_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Max_C_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Max_C_Current_Time[3] = stCurr_Time.u8Month;
		}
		if(u32Ic < u32Imin)
		{
			MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
			u32Ic &= 0x007FFFFF;		
			MemCopy(pstCurrentExtreme_info->u8Min_C_Current,&u32Ic,3);
			pstCurrentExtreme_info->u8Min_C_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Min_C_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Min_C_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Min_C_Current_Time[3] = stCurr_Time.u8Month;		
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�����ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004014_16(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_CURRENT_EXTREME_PASSRATE_LIST_S* pstCurrentExtreme_List = (QCSG_CURRENT_EXTREME_PASSRATE_LIST_S*)QCSG_CURRENT_EXTREME_MONTH;
	QCSG_CURRENT_EXTREME_PASSRATE_S* pstCurrentExtreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0,u8MeterType = 0;
	u32 u32Ia = 0,u32Ib = 0,u32Ic = 0,u32Imax = 0,u32Imin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) == TRUE)
	{
		//�������ݶ���BCD����˵���������������ֻ�������жϣ����ٶ�ȡ���������
		u8MeterType = 3;
	}
	else if((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],9) != TRUE) && ((QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) == TRUE)))
	{
		//�����
		u8MeterType = 1;
	}
	else
	{
		return;
	}
	u8PnNum = pstCurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstCurrentExtreme_List,0,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstCurrentExtreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstCurrentExtreme_List->stCurrentExtreme[i].u8Max_A_Current,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstCurrentExtreme_List,0,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ������¼���Ϣ
		if(u16Pn == pstCurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pstCurrentExtreme_info = &pstCurrentExtreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstCurrentExtreme_info = &pstCurrentExtreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pstCurrentExtreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstCurrentExtreme_info->u16Pn = u16Pn;
			pstCurrentExtreme_List->u8PnNum ++;
		}
	}
	MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32Ia &= 0x007FFFFF;
	u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);
	if(u8MeterType == 3)
	{
		MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
		MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
		u32Ib &= 0x007FFFFF;
		u32Ic &= 0x007FFFFF;		
		u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);
		u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);
	}
	MemCopy(&u32Imax,pstCurrentExtreme_info->u8Max_A_Current,3);
	MemCopy(&u32Imin,pstCurrentExtreme_info->u8Min_A_Current,3);
	if(u32Imin == 0)
	{
		u32Imin = 0x799999;
	}				
	u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
	u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
	if(u32Ia > u32Imax)
	{
		MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
		u32Ia &= 0x007FFFFF;
		MemCopy(pstCurrentExtreme_info->u8Max_A_Current,&u32Ia,3);
		pstCurrentExtreme_info->u8Max_A_Current_Time[0] = stCurr_Time.u8Minute;
		pstCurrentExtreme_info->u8Max_A_Current_Time[1] = stCurr_Time.u8Hour;
		pstCurrentExtreme_info->u8Max_A_Current_Time[2] = stCurr_Time.u8Day;
		pstCurrentExtreme_info->u8Max_A_Current_Time[3] = stCurr_Time.u8Month;
	}
	if(u32Ia < u32Imin)
	{
		MemCopy(&u32Ia,&pstDlt645Frame_Info->u8RevBuf[14],3);
		u32Ia &= 0x007FFFFF;
		MemCopy(pstCurrentExtreme_info->u8Min_A_Current,&u32Ia,3);
		pstCurrentExtreme_info->u8Min_A_Current_Time[0] = stCurr_Time.u8Minute;
		pstCurrentExtreme_info->u8Min_A_Current_Time[1] = stCurr_Time.u8Hour;
		pstCurrentExtreme_info->u8Min_A_Current_Time[2] = stCurr_Time.u8Day;
		pstCurrentExtreme_info->u8Min_A_Current_Time[3] = stCurr_Time.u8Month;		
	}
	if(u8MeterType == 3)
	{	
		MemCopy(&u32Imax,pstCurrentExtreme_info->u8Max_B_Current,3);
		MemCopy(&u32Imin,pstCurrentExtreme_info->u8Min_B_Current,3);
		u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
		u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
		if(u32Imin == 0)
		{
			u32Imin = 0x799999;
		}	
		if(u32Ib > u32Imax)
		{
			MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
			u32Ib &= 0x007FFFFF;	
			MemCopy(pstCurrentExtreme_info->u8Max_B_Current,&u32Ib,3);
			pstCurrentExtreme_info->u8Max_B_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Max_B_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Max_B_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Max_B_Current_Time[3] = stCurr_Time.u8Month;
		}
		if(u32Ib < u32Imin)
		{
			MemCopy(&u32Ib,&pstDlt645Frame_Info->u8RevBuf[17],3);
			u32Ib &= 0x007FFFFF;	
			MemCopy(pstCurrentExtreme_info->u8Min_B_Current,&u32Ib,3);
			pstCurrentExtreme_info->u8Min_B_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Min_B_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Min_B_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Min_B_Current_Time[3] = stCurr_Time.u8Month;		
		}
		
		MemCopy(&u32Imax,pstCurrentExtreme_info->u8Max_C_Current,3);
		MemCopy(&u32Imin,pstCurrentExtreme_info->u8Min_C_Current,3);
		u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
		u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
		if(u32Imin == 0)
		{
			u32Imin = 0x799999;
		}	
		if(u32Ic > u32Imax)
		{
			MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
			u32Ic &= 0x007FFFFF;		
			MemCopy(pstCurrentExtreme_info->u8Max_C_Current,&u32Ic,3);
			pstCurrentExtreme_info->u8Max_C_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Max_C_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Max_C_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Max_C_Current_Time[3] = stCurr_Time.u8Month;
		}
		if(u32Ic < u32Imin)
		{
			MemCopy(&u32Ic,&pstDlt645Frame_Info->u8RevBuf[20],3);
			u32Ic &= 0x007FFFFF;		
			MemCopy(pstCurrentExtreme_info->u8Min_C_Current,&u32Ic,3);
			pstCurrentExtreme_info->u8Min_C_Current_Time[0] = stCurr_Time.u8Minute;
			pstCurrentExtreme_info->u8Min_C_Current_Time[1] = stCurr_Time.u8Hour;
			pstCurrentExtreme_info->u8Min_C_Current_Time[2] = stCurr_Time.u8Day;
			pstCurrentExtreme_info->u8Min_C_Current_Time[3] = stCurr_Time.u8Month;		
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ��������������ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000017(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_N_CurrentExtreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_N_CURRENT_EXTREME_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_N_CurrentExtreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32In = 0,u32Imax = 0,u32Imin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pst_N_CurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_N_CurrentExtreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_N_CurrentExtreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}			
		if(QCSG_BCDCheck(pst_N_CurrentExtreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_N_CurrentExtreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_N_CurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_N_CurrentExtreme_info = &pst_N_CurrentExtreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_N_CurrentExtreme_info = &pst_N_CurrentExtreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_N_CurrentExtreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_N_CurrentExtreme_info->u16Pn = u16Pn;
			pst_N_CurrentExtreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32In &= 0x007FFFFF;
	u32In = QCSG_BCD2HEX((u8)(u32In>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32In>>8)) * 100 + QCSG_BCD2HEX((u8)u32In);
	MemCopy(&u32Imax,pst_N_CurrentExtreme_info->u8Max_Value,3);
	MemCopy(&u32Imin,pst_N_CurrentExtreme_info->u8Min_Value,3);
	if(u32Imin == 0)
	{
		u32Imin = 0x799999;
	}				
	u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
	u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
	if(u32In > u32Imax)
	{
		MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32In &= 0x007FFFFF;
		MemCopy(pst_N_CurrentExtreme_info->u8Max_Value,&u32In,3);
		pst_N_CurrentExtreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_N_CurrentExtreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_N_CurrentExtreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_N_CurrentExtreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32In < u32Imin)
	{
		MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32In &= 0x007FFFFF;
		MemCopy(pst_N_CurrentExtreme_info->u8Min_Value,&u32In,3);
		pst_N_CurrentExtreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_N_CurrentExtreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_N_CurrentExtreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_N_CurrentExtreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ��������������ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004017(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_N_CurrentExtreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_N_CURRENT_EXTREME_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_N_CurrentExtreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32In = 0,u32Imax = 0,u32Imin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pst_N_CurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_N_CurrentExtreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_N_CurrentExtreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}					
		if(QCSG_BCDCheck(pst_N_CurrentExtreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_N_CurrentExtreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_N_CurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_N_CurrentExtreme_info = &pst_N_CurrentExtreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_N_CurrentExtreme_info = &pst_N_CurrentExtreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_N_CurrentExtreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_N_CurrentExtreme_info->u16Pn = u16Pn;
			pst_N_CurrentExtreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],3);
	u32In &= 0x007FFFFF;	
	u32In = QCSG_BCD2HEX((u8)(u32In>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32In>>8)) * 100 + QCSG_BCD2HEX((u8)u32In);
	MemCopy(&u32Imax,pst_N_CurrentExtreme_info->u8Max_Value,3);
	MemCopy(&u32Imin,pst_N_CurrentExtreme_info->u8Min_Value,3);
	if(u32Imin == 0)
	{
		u32Imin = 0x799999;
	}				
	u32Imax = QCSG_BCD2HEX((u8)(u32Imax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imax);
	u32Imin = QCSG_BCD2HEX((u8)(u32Imin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Imin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Imin);
	if(u32In > u32Imax)
	{
		MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],3);
		u32In &= 0x007FFFFF;
		MemCopy(pst_N_CurrentExtreme_info->u8Max_Value,&u32In,3);
		pst_N_CurrentExtreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_N_CurrentExtreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_N_CurrentExtreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_N_CurrentExtreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32In < u32Imin)
	{
		MemCopy(&u32In,&pstDlt645Frame_Info->u8RevBuf[14],3);
		u32In &= 0x007FFFFF;		
		MemCopy(pst_N_CurrentExtreme_info->u8Min_Value,&u32In,3);
		pst_N_CurrentExtreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_N_CurrentExtreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_N_CurrentExtreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_N_CurrentExtreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ������й����ʼ�ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000018(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Active_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_ACTIVE_POWER_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Active_Power_Extreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32P = 0,u32Pmax = 0,u32Pmin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pst_Active_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Active_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Active_Power_Extreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pst_Active_Power_Extreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Active_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Active_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Active_Power_Extreme_info = &pst_Active_Power_Extreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_Active_Power_Extreme_info = &pst_Active_Power_Extreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_Active_Power_Extreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_Active_Power_Extreme_info->u16Pn = u16Pn;
			pst_Active_Power_Extreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32P &= 0x007FFFFF;
	u32P = QCSG_BCD2HEX((u8)(u32P>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32P>>8)) * 100 + QCSG_BCD2HEX((u8)u32P);
	MemCopy(&u32Pmax,pst_Active_Power_Extreme_info->u8Max_Value,3);
	MemCopy(&u32Pmin,pst_Active_Power_Extreme_info->u8Min_Value,3);
	if(u32Pmin == 0)
	{
		u32Pmin = 0x799999;
	}				
	u32Pmax = QCSG_BCD2HEX((u8)(u32Pmax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmax);
	u32Pmin = QCSG_BCD2HEX((u8)(u32Pmin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmin);
	if(u32P > u32Pmax)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;
		MemCopy(pst_Active_Power_Extreme_info->u8Max_Value,&u32P,3);
		pst_Active_Power_Extreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Active_Power_Extreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Active_Power_Extreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_Active_Power_Extreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32P < u32Pmin)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;
		MemCopy(pst_Active_Power_Extreme_info->u8Min_Value,&u32P,3);
		pst_Active_Power_Extreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Active_Power_Extreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Active_Power_Extreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_Active_Power_Extreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}		
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ������й����ʼ�ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004018(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Active_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_ACTIVE_POWER_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Active_Power_Extreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32P = 0,u32Pmax = 0,u32Pmin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pst_Active_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Active_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Active_Power_Extreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}			
		if(QCSG_BCDCheck(pst_Active_Power_Extreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Active_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}			
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Active_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Active_Power_Extreme_info = &pst_Active_Power_Extreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_Active_Power_Extreme_info = &pst_Active_Power_Extreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_Active_Power_Extreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_Active_Power_Extreme_info->u16Pn = u16Pn;
			pst_Active_Power_Extreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32P &= 0x007FFFFF;
	u32P = QCSG_BCD2HEX((u8)(u32P>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32P>>8)) * 100 + QCSG_BCD2HEX((u8)u32P);
	MemCopy(&u32Pmax,pst_Active_Power_Extreme_info->u8Max_Value,3);
	MemCopy(&u32Pmin,pst_Active_Power_Extreme_info->u8Min_Value,3);
	if(u32Pmin == 0)
	{
		u32Pmin = 0x799999;
	}		
	u32Pmax = QCSG_BCD2HEX((u8)(u32Pmax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmax);
	u32Pmin = QCSG_BCD2HEX((u8)(u32Pmin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmin);
	if(u32P > u32Pmax)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;
		MemCopy(pst_Active_Power_Extreme_info->u8Max_Value,&u32P,3);
		pst_Active_Power_Extreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Active_Power_Extreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Active_Power_Extreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_Active_Power_Extreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32P < u32Pmin)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;
		MemCopy(pst_Active_Power_Extreme_info->u8Min_Value,&u32P,3);
		pst_Active_Power_Extreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Active_Power_Extreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Active_Power_Extreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_Active_Power_Extreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}			
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ������޹����ʼ�ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000019(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Reactive_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_REACTIVE_POWER_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Reactive_Power_Extreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32P = 0,u32Pmax = 0,u32Pmin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pst_Reactive_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Reactive_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Reactive_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Reactive_Power_Extreme_info = &pst_Reactive_Power_Extreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_Reactive_Power_Extreme_info = &pst_Reactive_Power_Extreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_Reactive_Power_Extreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_Reactive_Power_Extreme_info->u16Pn = u16Pn;
			pst_Reactive_Power_Extreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32P &= 0x007FFFFF;	
	u32P = QCSG_BCD2HEX((u8)(u32P>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32P>>8)) * 100 + QCSG_BCD2HEX((u8)u32P);
	MemCopy(&u32Pmax,pst_Reactive_Power_Extreme_info->u8Max_Value,3);
	MemCopy(&u32Pmin,pst_Reactive_Power_Extreme_info->u8Min_Value,3);
	if(u32Pmin == 0)
	{
		u32Pmin = 0x799999;
	}		
	u32Pmax = QCSG_BCD2HEX((u8)(u32Pmax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmax);
	u32Pmin = QCSG_BCD2HEX((u8)(u32Pmin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmin);
	if(u32P > u32Pmax)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;	
		MemCopy(pst_Reactive_Power_Extreme_info->u8Max_Value,&u32P,3);
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32P < u32Pmin)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;	
		MemCopy(pst_Reactive_Power_Extreme_info->u8Min_Value,&u32P,3);
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}		
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ������޹����ʼ�ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004019(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Reactive_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_REACTIVE_POWER_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Reactive_Power_Extreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32P = 0,u32Pmax = 0,u32Pmin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],3) != TRUE)
	{
		return;
	}
	u8PnNum = pst_Reactive_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Reactive_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}				
		if(QCSG_BCDCheck(pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Reactive_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}			
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Reactive_Power_Extreme_info = &pst_Reactive_Power_Extreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_Reactive_Power_Extreme_info = &pst_Reactive_Power_Extreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_Reactive_Power_Extreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_Reactive_Power_Extreme_info->u16Pn = u16Pn;
			pst_Reactive_Power_Extreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
	u32P &= 0x007FFFFF;	
	u32P = QCSG_BCD2HEX((u8)(u32P>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32P>>8)) * 100 + QCSG_BCD2HEX((u8)u32P);
	MemCopy(&u32Pmax,pst_Reactive_Power_Extreme_info->u8Max_Value,3);
	MemCopy(&u32Pmin,pst_Reactive_Power_Extreme_info->u8Min_Value,3);
	if(u32Pmin == 0)
	{
		u32Pmin = 0x799999;
	}		
	u32Pmax = QCSG_BCD2HEX((u8)(u32Pmax>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmax);
	u32Pmin = QCSG_BCD2HEX((u8)(u32Pmin>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Pmin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pmin);
	if(u32P > u32Pmax)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;	
		MemCopy(pst_Reactive_Power_Extreme_info->u8Max_Value,&u32P,3);
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_Reactive_Power_Extreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32P < u32Pmin)
	{
		MemCopy(&u32P,&pstDlt645Frame_Info->u8RevBuf[14],3);	
		u32P &= 0x007FFFFF;	
		MemCopy(pst_Reactive_Power_Extreme_info->u8Min_Value,&u32P,3);
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_Reactive_Power_Extreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}				
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����չ���������ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E100001A(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Power_Factor_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_POWER_FACTOR_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Power_Factor_Extreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32Pf = 0,u32Pfmax = 0,u32Pfmin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	u8PnNum = pst_Power_Factor_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Power_Factor_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Power_Factor_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}			
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Power_Factor_Extreme_info = &pst_Power_Factor_Extreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_Power_Factor_Extreme_info = &pst_Power_Factor_Extreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_Power_Factor_Extreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_Power_Factor_Extreme_info->u16Pn = u16Pn;
			pst_Power_Factor_Extreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32Pf,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u32Pf &= 0x00007FFF;	
	u32Pf = QCSG_BCD2HEX((u8)(u32Pf>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pf);
	MemCopy(&u32Pfmax,pst_Power_Factor_Extreme_info->u8Max_Value,2);
	MemCopy(&u32Pfmin,pst_Power_Factor_Extreme_info->u8Min_Value,2);
	u32Pfmax = QCSG_BCD2HEX((u8)(u32Pfmax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pfmax);
	u32Pfmin = QCSG_BCD2HEX((u8)(u32Pfmin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pfmin);
	if(u32Pfmin == 0)
	{
		u32Pfmin = 0x7999;
	}			
	if(u32Pf > u32Pfmax)
	{
		MemCopy(&u32Pf,&pstDlt645Frame_Info->u8RevBuf[14],2);
		u32Pf &= 0x00007FFF;	
		MemCopy(pst_Power_Factor_Extreme_info->u8Max_Value,&u32Pf,2);
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32Pf < u32Pfmin)
	{
		MemCopy(&u32Pf,&pstDlt645Frame_Info->u8RevBuf[14],2);
		u32Pf &= 0x00007FFF;		
		MemCopy(pst_Power_Factor_Extreme_info->u8Min_Value,&u32Pf,2);
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}		
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����¹���������ֵͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E100401A(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Power_Factor_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_POWER_FACTOR_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Power_Factor_Extreme_info = NULL;	
	TIME_PARAM_S stCurr_Time;
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u32 u32Pf = 0,u32Pfmax = 0,u32Pfmin = 0;
	
	if(HAL_GetSystemTime(&stCurr_Time) != TRUE)
	{
		return;
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	u8PnNum = pst_Power_Factor_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Power_Factor_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}		
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u16Pn,1) != TRUE)
		{
			break;
		}				
		if(QCSG_BCDCheck(pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u8Max_Value,sizeof(QCSG_VALUE_EXTREME_PASSRATE_S) - 4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Power_Factor_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Power_Factor_Extreme_info = &pst_Power_Factor_Extreme_List->stCurrentExtreme[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pst_Power_Factor_Extreme_info = &pst_Power_Factor_Extreme_List->stCurrentExtreme[u8PnNum];
			MemSet(pst_Power_Factor_Extreme_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pst_Power_Factor_Extreme_info->u16Pn = u16Pn;
			pst_Power_Factor_Extreme_List->u8PnNum ++;
		}
	}	
	MemCopy(&u32Pf,&pstDlt645Frame_Info->u8RevBuf[14],2);	
	u32Pf &= 0x00007FFF;
	u32Pf = QCSG_BCD2HEX((u8)(u32Pf>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pf);
	MemCopy(&u32Pfmax,pst_Power_Factor_Extreme_info->u8Max_Value,2);
	MemCopy(&u32Pfmin,pst_Power_Factor_Extreme_info->u8Min_Value,2);
	if(u32Pfmin == 0)
	{
		u32Pfmin = 0x7999;
	}		
	u32Pfmax = QCSG_BCD2HEX((u8)(u32Pfmax>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pfmax);
	u32Pfmin = QCSG_BCD2HEX((u8)(u32Pfmin>>8)) * 100 + QCSG_BCD2HEX((u8)u32Pfmin);
	if(u32Pf > u32Pfmax)
	{
		MemCopy(&u32Pf,&pstDlt645Frame_Info->u8RevBuf[14],2);	
		u32Pf &= 0x00007FFF;		
		MemCopy(pst_Power_Factor_Extreme_info->u8Max_Value,&u32Pf,2);
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[2] = stCurr_Time.u8Day;
		pst_Power_Factor_Extreme_info->u8Max_Value_Time[3] = stCurr_Time.u8Month;
	}
	if(u32Pf < u32Pfmin)
	{
		MemCopy(&u32Pf,&pstDlt645Frame_Info->u8RevBuf[14],2);	
		u32Pf &= 0x00007FFF;		
		MemCopy(pst_Power_Factor_Extreme_info->u8Min_Value,&u32Pf,2);
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[0] = stCurr_Time.u8Minute;
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[1] = stCurr_Time.u8Hour;
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[2] = stCurr_Time.u8Day;
		pst_Power_Factor_Extreme_info->u8Min_Value_Time[3] = stCurr_Time.u8Month;		
	}		
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�����ƽ��Խ��ͳ������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E100001B(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	//ͳ�Ʊ��ղ�ƽ�⣨ʵʱ���ݣ���ͳ���ն��᲻ƽ�⣨�������ݣ�
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����յ�ѹ��ƽ��Խ��ͳ������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E100001C(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	//ͳ�Ʊ��ղ�ƽ�⣨ʵʱ���ݣ���ͳ���ն��᲻ƽ�⣨�������ݣ�	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����ն�������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000030(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����ʧѹ����
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000031(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����ʧ������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000032(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����չ�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000033(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����չ�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000034(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_A_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[16],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[16],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����չ�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000035(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_B_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[18],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[18],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����չ�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1000036(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_C_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[20],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[20],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}		
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�����ƽ��Խ��ͳ������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E100401B(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����µ�ѹ��ƽ��Խ��ͳ������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E100401C(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����¶�������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004030(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����ʧѹ����
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004031(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ�����ʧ������
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004032(u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����¹�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004033(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[14],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[14],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}				
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����¹�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004034(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_A_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[16],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[16],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}				
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����¹�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004035(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_B_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[18],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[18],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}				
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݣ����¹�����������ͳ��
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info��pu8MeterId
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_StatisticsData_E1004036(u8 u8Cycle,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_C_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;	
	u8 u8PnNum = 0,u8CtrlCode = 0,i = 0;
	u16 u16PowerFactor = 0;
	u32 u32ReadLen = 0,u32SumTime = 0;
	PowerQualityParaStruct stPowerQualityPara;
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	if(u8CtrlCode != 0x91)
	{
		return;
	}		
	if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[20],2) != TRUE)
	{
		return;
	}
	MemCopy(&u16PowerFactor,&pstDlt645Frame_Info->u8RevBuf[20],2);
	u16PowerFactor &= 0x7FFF;
	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn,1) != TRUE)
		{
			break;
		}				
		if(QCSG_BCDCheck(pstPowerFactor_Zone_List->stPowerFactorZone[i].u8MonitorTime,sizeof(QCSG_POWER_FACTOR_ZONE_S) - 2) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			break;
		}
	}
	if(i >= u8PnNum)
	{
		//������֮ǰδ�м�¼�������ӵ���¼��,�˴β����κδ���
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[u8PnNum];
			MemSet(pstPowerFactor_Zone_info,0,sizeof(QCSG_VOLTAGE_PASSRATE_S));
			pstPowerFactor_Zone_info->u16Pn = u16Pn;
			pstPowerFactor_Zone_List->u8PnNum ++;
		}
	}
	//��ȡ�����������ζ�ֵ	
	u32ReadLen = sizeof(PowerQualityParaStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800205,(u8*)&stPowerQualityPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8MonitorTime,3);
	u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
	u32SumTime += u8Cycle;
	u32SumTime = hex_bcd(u32SumTime);
	MemCopy(pstPowerFactor_Zone_info->u8MonitorTime,&u32SumTime,3);
	if(u16PowerFactor < stPowerQualityPara.E0800205[0])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone1Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone1Time,&u32SumTime,3);		
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[0]) && (u16PowerFactor < stPowerQualityPara.E0800205[1]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone2Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone2Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[1]) && (u16PowerFactor < stPowerQualityPara.E0800205[2]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone3Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone3Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[2]) && (u16PowerFactor < stPowerQualityPara.E0800205[3]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone4Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone4Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[3]) && (u16PowerFactor < stPowerQualityPara.E0800205[4]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone5Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone5Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[4]) && (u16PowerFactor < stPowerQualityPara.E0800205[5]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone6Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone6Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[5]) && (u16PowerFactor < stPowerQualityPara.E0800205[6]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone7Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone7Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[6]) && (u16PowerFactor < stPowerQualityPara.E0800205[7]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone8Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone8Time,&u32SumTime,3);			
	}
	if((u16PowerFactor >= stPowerQualityPara.E0800205[7]) && (u16PowerFactor < stPowerQualityPara.E0800205[8]))
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone9Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone9Time,&u32SumTime,3);			
	}
	if(u16PowerFactor >= stPowerQualityPara.E0800205[8])
	{
		MemCopy(&u32SumTime,pstPowerFactor_Zone_info->u8Zone10Time,3);
		u32SumTime = QCSG_BCD2HEX((u8)(u32SumTime>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32SumTime>>8)) * 100 + QCSG_BCD2HEX((u8)u32SumTime);
		u32SumTime += u8Cycle;
		u32SumTime = hex_bcd(u32SumTime);
		MemCopy(pstPowerFactor_Zone_info->u8Zone10Time,&u32SumTime,3);			
	}	
}
/**--------------------------------------------------------------------
*��    ��:����������µ�����д��RAM�ݴ�
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Calc_Energy_TO_Ram(u16 u16Pn,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	QCSG_MONTH_ENERGY_LIST_S* pst_Month_Energy_List = (QCSG_MONTH_ENERGY_LIST_S*)QCSG_MONTH_ENERGY_LIST_CTRL;
	QCSG_ENERGY_S* pstEnergy_Data = NULL;
	MeasurePointStruct stPnPara;
	u8 u8PnNum,i = 0,u8Dlt645DataPos = 0;
	u32 u32Ident = 0,u32LastValue = 0,u32CurrValue = 0,u32SaveValue = 0,u32ReadLen = 0;	
	
	if(u8ProtocolType == METER_DLT645_97)
	{
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],2);
		if(u32Ident != 0x901F)
		{
			return;
		}
		u8Dlt645DataPos = 12;
	}
	else if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
		if(u32Ident != 0x0001FF00)
		{
			return;
		}
		u8Dlt645DataPos = 14;		//��λ�����ݵ�Ԫ
	}
	else
	{
		return;
	}
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn,1) != TRUE)
		{
			break;
		}				
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}	
	MemCopy(&u32LastValue,pstRam_Data->u8Data00010000,4);
	MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4);
	u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
				 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
				 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
				 + QCSG_BCD2HEX((u8)u32LastValue);
	u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
				 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
				 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
				 + QCSG_BCD2HEX((u8)u32CurrValue);
	if(u32CurrValue < u32LastValue)
	{
		return;
	}
	u8PnNum = pst_Month_Energy_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u16Pn,1) != TRUE)
		{
			break;
		}		
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pst_Month_Energy_List->stPn_Ram_Data[i].u16Pn)	//ԭ����������
		{
			pstEnergy_Data = &pst_Month_Energy_List->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)			//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEnergy_Data = &pst_Month_Energy_List->stPn_Ram_Data[u8PnNum];
			pstEnergy_Data->u16Pn = u16Pn;
			pstEnergy_Data->u32Energy00010000 = 0;
			pst_Month_Energy_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}	
	if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
	{
		return;
	}
	if((stPnPara.CT == 0) || (stPnPara.PT == 0) || (stPnPara.CT == 0xFF) || (stPnPara.PT == 0xFF))
	{
		return;
	}
	u32SaveValue = pstEnergy_Data->u32Energy00010000;
	u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
				 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
				 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
				 + QCSG_BCD2HEX((u8)u32SaveValue);
	stPnPara.CT = QCSG_BCD2HEX((u8)(stPnPara.CT>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.CT);
	stPnPara.PT = QCSG_BCD2HEX((u8)(stPnPara.PT>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.PT);
	u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue) * stPnPara.CT * stPnPara.PT;
	pstEnergy_Data->u32Energy00010000 = hex_bcd(u32SaveValue);
}
/**--------------------------------------------------------------------
*��    ��:����������µ�����д��RAM�ݴ�
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Calc_Lcd_Energy_TO_Ram(u16 u16Pn,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	QCSG_DAY_ENERGY_LIST_S* pst_Day_Energy_List = (QCSG_DAY_ENERGY_LIST_S*)QCSG_DAY_ENERGY_LIST_LCD;
	QCSG_MONTH_ENERGY_LIST_S* pst_Month_Energy_List = (QCSG_MONTH_ENERGY_LIST_S*)QCSG_MONTH_ENERGY_LIST_LCD;
	QCSG_ENERGY_S* pstEnergy_Data = NULL;	
	QCSG_ENERGY_S* pstMonth_Energy_Data = NULL;	
	u8 u8PnNum,i = 0,u8Dlt645DataPos = 0,u8DataLen = 0,u8CtrlCode = 0;
	u32 u32Ident = 0,u32LastValue = 0,u32CurrValue = 0,u32SaveValue = 0;	
	
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8DataLen = pstDlt645Frame_Info->u8RevBuf[9];
	if((u8CtrlCode == 0xC1) || (u8CtrlCode == 0xD1))
	{
		return;
	}
	if(u8ProtocolType == METER_DLT645_97)
	{
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],2);
		u8Dlt645DataPos = 12;
	}
	else if(u8ProtocolType == METER_DLT645_07)
	{
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);
		u8Dlt645DataPos = 14;
	}
	else
	{
		return;
	}	
	//У��RAM�д洢��ʾֵ����
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
		return;
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn,1) != TRUE)
		{
			break;
		}		
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));	
		return;
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return;
	}		
	//��������й�ʾֵû�����������������������ݣ�����������������ʾֵҲ������
	if(pstRam_Data->u8RdDoneFlag != TRUE)
	{
		return;
	}


	
	u8PnNum = pst_Day_Energy_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Day_Energy_List,0,sizeof(QCSG_DAY_ENERGY_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00010000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00010100,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00010200,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00010300,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00010400,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00020000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00020100,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00020200,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00020300,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00020400,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00030000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Day_Energy_List->stPn_Ram_Data[i].u32Energy00040000,4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Day_Energy_List,0,sizeof(QCSG_DAY_ENERGY_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pst_Day_Energy_List->stPn_Ram_Data[i].u16Pn)
		{
			pstEnergy_Data = &pst_Day_Energy_List->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)		//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstEnergy_Data = &pst_Day_Energy_List->stPn_Ram_Data[u8PnNum];
			pstEnergy_Data->u16Pn = u16Pn;
			pstEnergy_Data->u32Energy00010000 = 0;
			pstEnergy_Data->u32Energy00010100 = 0;
			pstEnergy_Data->u32Energy00010200 = 0;
			pstEnergy_Data->u32Energy00010300 = 0;
			pstEnergy_Data->u32Energy00010400 = 0;
			pstEnergy_Data->u32Energy00020000 = 0;
			pstEnergy_Data->u32Energy00020100 = 0;
			pstEnergy_Data->u32Energy00020200 = 0;
			pstEnergy_Data->u32Energy00020300 = 0;
			pstEnergy_Data->u32Energy00020400 = 0;
			pstEnergy_Data->u32Energy00030000 = 0;
			pstEnergy_Data->u32Energy00040000 = 0;
			pst_Day_Energy_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}		




	
	u8PnNum = pst_Month_Energy_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010100,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010200,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010300,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010400,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00020000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00020100,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00020200,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00020300,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00020400,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00030000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00040000,4) != TRUE)
		{
			break;
		}
	}
	if(i < MAX_PN_NUM2)
	{
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));		
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pst_Month_Energy_List->stPn_Ram_Data[i].u16Pn)
		{
			pstMonth_Energy_Data = &pst_Month_Energy_List->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)		//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)	
		{
			pstMonth_Energy_Data = &pst_Month_Energy_List->stPn_Ram_Data[u8PnNum];
			pstMonth_Energy_Data->u16Pn = u16Pn;
			pstMonth_Energy_Data->u32Energy00010000 = 0;
			pstMonth_Energy_Data->u32Energy00010100 = 0;
			pstMonth_Energy_Data->u32Energy00010200 = 0;
			pstMonth_Energy_Data->u32Energy00010300 = 0;
			pstMonth_Energy_Data->u32Energy00010400 = 0;
			pstMonth_Energy_Data->u32Energy00020000 = 0;
			pstMonth_Energy_Data->u32Energy00020100 = 0;
			pstMonth_Energy_Data->u32Energy00020200 = 0;
			pstMonth_Energy_Data->u32Energy00020300 = 0;
			pstMonth_Energy_Data->u32Energy00020400 = 0;
			pstMonth_Energy_Data->u32Energy00030000 = 0;
			pstMonth_Energy_Data->u32Energy00040000 = 0;
			pst_Month_Energy_List->u8PnNum++;
		}
		else
		{
			return;
		}
	}		

	
	if((u32Ident == 0x901F) || (u32Ident == 0x0001FF00))
	{
		if(QCSG_BCDCheck(pstRam_Data->u8Data00010000,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00010000,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00010000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00010000 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00010000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00010000 = hex_bcd(u32SaveValue);
				}
			}

		}
		if((u8DataLen/4) == 1)
		{
			return;
		}

		
		if(QCSG_BCDCheck(pstRam_Data->u8Data00010100,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00010100,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 4],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 4],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00010100;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00010100 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00010100;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00010100 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 2)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00010200,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00010200,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 8],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 8],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00010200;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00010200 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00010200;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00010200 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 3)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00010300,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00010300,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 12],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 12],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00010300;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00010300 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00010300;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00010300 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 4)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00010400,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00010400,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 16],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 16],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00010400;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00010400 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00010400;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00010400 = hex_bcd(u32SaveValue);
				}
			}
		}
	}
	if((u32Ident == 0x902F) || (u32Ident == 0x0002FF00))
	{
		if(QCSG_BCDCheck(pstRam_Data->u8Data00020000,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00020000,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00020000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00020000 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00020000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00020000 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 1)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00020100,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00020100,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 4],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 4],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00020100;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00020100 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00020100;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00020100 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 2)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00020200,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00020200,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 8],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 8],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00020200;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00020200 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00020200;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00020200 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 3)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00020300,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00020300,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 12],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 12],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00020300;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00020300 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00020300;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00020300 = hex_bcd(u32SaveValue);
				}
			}
		}
		if((u8DataLen/4) == 4)
		{
			return;
		}
		if(QCSG_BCDCheck(pstRam_Data->u8Data00020400,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00020400,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 16],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos + 16],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00020400;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00020400 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00020400;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00020400 = hex_bcd(u32SaveValue);
				}
			}
		}		
	}
	if((u32Ident == 0x911F) || (u32Ident == 0x0003FF00))
	{
		if(QCSG_BCDCheck(pstRam_Data->u8Data00030000,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00030000,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);	
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00030000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00030000 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00030000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00030000 = hex_bcd(u32SaveValue);
				}				
			}			
		}		
	}
	if((u32Ident == 0x902F) || (u32Ident == 0x0004FF00))
	{
		if(QCSG_BCDCheck(pstRam_Data->u8Data00040000,4) == TRUE)
		{
			MemCopy(&u32LastValue,pstRam_Data->u8Data00040000,4);
			if(QCSG_BCDCheck(&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4) == TRUE)
			{
				MemCopy(&u32CurrValue,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],4);
				u32LastValue = QCSG_BCD2HEX((u8)(u32LastValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32LastValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32LastValue);
				u32CurrValue = QCSG_BCD2HEX((u8)(u32CurrValue >> 24)) * 1000000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 16)) * 10000
							 + QCSG_BCD2HEX((u8)(u32CurrValue >> 8)) * 100
							 + QCSG_BCD2HEX((u8)u32CurrValue);	
				if(u32CurrValue >= u32LastValue)
				{
					u32SaveValue = pstEnergy_Data->u32Energy00040000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstEnergy_Data->u32Energy00040000 = hex_bcd(u32SaveValue);
					u32SaveValue = pstMonth_Energy_Data->u32Energy00040000;
					u32SaveValue = QCSG_BCD2HEX((u8)(u32SaveValue >> 24)) * 1000000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 16)) * 10000
						 + QCSG_BCD2HEX((u8)(u32SaveValue >> 8)) * 100
						 + QCSG_BCD2HEX((u8)u32SaveValue);
					u32SaveValue = u32SaveValue + (u32CurrValue - u32LastValue);
					pstMonth_Energy_Data->u32Energy00040000 = hex_bcd(u32SaveValue);
				}				
			}			
		}			
	}
}
/**--------------------------------------------------------------------
*��    ��:LCD��ʾ������д���ڴ�
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_WriteLcdData_TO_Ram(u16 u16Pn,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = NULL;	
	u8 u8PnNum = 0,i = 0,u8CtrlCode = 0,u8DataLen = 0;
	u32 u32Ident = 0,u32Value = 0;
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_LCD_RAMDATA_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[i];
			break;
		}
	}	
	if(i >= u8PnNum)		//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[u8PnNum];
			pstRam_Data->u16Pn = u16Pn;
			pstPn_Ram_Data->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	if(u8ProtocolType == METER_DLT645_97)
	{
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],2);
	}
	else
	{
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);		
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];	
	u8CtrlCode &= 0xDF;
	u8DataLen = pstDlt645Frame_Info->u8RevBuf[9];
	switch(u32Ident)
	{
		case 0x901F:
			MemSet(pstRam_Data->u8Data0001FF00,0xFF,sizeof(pstRam_Data->u8Data0001FF00));
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0001FF00,&pstDlt645Frame_Info->u8RevBuf[12],u8DataLen - 2);
			}				
			break;
		case 0x902F:
			MemSet(pstRam_Data->u8Data0002FF00,0xFF,sizeof(pstRam_Data->u8Data0002FF00));
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0002FF00,&pstDlt645Frame_Info->u8RevBuf[12],u8DataLen - 2);
			}					
			break;	
		case 0x911F:
			MemSet(pstRam_Data->u8Data0003FF00,0xFF,sizeof(pstRam_Data->u8Data0003FF00));
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0003FF00,&pstDlt645Frame_Info->u8RevBuf[12],u8DataLen - 2);
			}					
			break;	
		case 0x912F:
			MemSet(pstRam_Data->u8Data0004FF00,0xFF,sizeof(pstRam_Data->u8Data0004FF00));
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0004FF00,&pstDlt645Frame_Info->u8RevBuf[12],u8DataLen - 2);
			}					
			break;				
		    //A���ѹ
		case 0xB611:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[12],2);
				u32Value = (u32Value << 4) & 0xFFFFFFF0;
				MemCopy(pstRam_Data->u8Data0201FF00,&u32Value,2);
			}
			break;
			//B���ѹ
		case 0xB612:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[12],2);
				u32Value = (u32Value << 4) & 0xFFFFFFF0;				
				MemCopy(&pstRam_Data->u8Data0201FF00[2],&u32Value,2);
			}			
			break;
			//C���ѹ
		case 0xB613:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[12],2);
				u32Value = (u32Value << 4) & 0xFFFFFFF0;					
				MemCopy(&pstRam_Data->u8Data0201FF00[4],&u32Value,2);
			}				
			break;
			//A�����
		case 0xB621:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[12],2);
				u32Value = (u32Value << 4) & 0xfffffff0;
				MemCopy(pstRam_Data->u8Data0202FF00,&u32Value,3);
			}					
			break;
			//B�����
		case 0xB622:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[12],2);
				u32Value = (u32Value << 4) & 0xfffffff0;
				MemCopy(&pstRam_Data->u8Data0202FF00[3],&u32Value,3);
			}	
			break;
			//C�����
		case 0xB623:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&u32Value,&pstDlt645Frame_Info->u8RevBuf[12],2);
				u32Value = (u32Value << 4) & 0xfffffff0;
				MemCopy(&pstRam_Data->u8Data0202FF00[6],&u32Value,3);
			}				
			break;
			//�й�����
		case 0xB630:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0203FF00,&pstDlt645Frame_Info->u8RevBuf[12],3);
			}				
			break;
		case 0xB631:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0203FF00[3],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}						
			break;
		case 0xB632:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0203FF00[6],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}				
			break;
		case 0xB633:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0203FF00[9],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}							
			break;
			//�޹�����
		case 0xB640:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0204FF00,&pstDlt645Frame_Info->u8RevBuf[12],3);
			}						
			break;
		case 0xB641:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0204FF00[3],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}					
			break;
		case 0xB642:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0204FF00[6],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}						
			break;
		case 0xB643:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0204FF00[9],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}						
			break;
			//�ܹ�������
		case 0xB650:
			MemSet(pstRam_Data->u8Data0206FF00,0xFF,sizeof(pstRam_Data->u8Data0206FF00));
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0206FF00,&pstDlt645Frame_Info->u8RevBuf[12],2);
			}					
			break;
		case 0xB651:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0206FF00[2],&pstDlt645Frame_Info->u8RevBuf[12],2);
			}					
			break;
		case 0xB652:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0206FF00[4],&pstDlt645Frame_Info->u8RevBuf[12],2);
			}					
			break;
		case 0xB653:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0206FF00[6],&pstDlt645Frame_Info->u8RevBuf[12],2);
			}					
			break;
		case 0x0001FF00:
			MemSet(pstRam_Data->u8Data0001FF00,0xFF,sizeof(pstRam_Data->u8Data0001FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0001FF00))
				{
					MemCopy(pstRam_Data->u8Data0001FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}		
			break;
		case 0x0002FF00:
			MemSet(pstRam_Data->u8Data0002FF00,0xFF,sizeof(pstRam_Data->u8Data0002FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0002FF00))
				{
					MemCopy(pstRam_Data->u8Data0002FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}	
			break;
		case 0x0003FF00:
			MemSet(pstRam_Data->u8Data0003FF00,0xFF,sizeof(pstRam_Data->u8Data0003FF00));		
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0003FF00))
				{
					MemCopy(pstRam_Data->u8Data0003FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}				
			break;
		case 0x0004FF00:
			MemSet(pstRam_Data->u8Data0004FF00,0xFF,sizeof(pstRam_Data->u8Data0004FF00));		
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0004FF00))
				{
					MemCopy(pstRam_Data->u8Data0004FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}			
			break;	
		case 0x0201FF00:
			MemSet(pstRam_Data->u8Data0201FF00,0xFF,sizeof(pstRam_Data->u8Data0201FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0201FF00))
				{
					MemCopy(pstRam_Data->u8Data0201FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}				
			break;
		case 0x0202FF00:
			MemSet(pstRam_Data->u8Data0202FF00,0xFF,sizeof(pstRam_Data->u8Data0202FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0202FF00))
				{
					MemCopy(pstRam_Data->u8Data0202FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}					
			break;	
		case 0x0203FF00:
			MemSet(pstRam_Data->u8Data0203FF00,0xFF,sizeof(pstRam_Data->u8Data0203FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0203FF00))
				{
					MemCopy(pstRam_Data->u8Data0203FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}							
			break;	
		case 0x0204FF00:
			MemSet(pstRam_Data->u8Data0204FF00,0xFF,sizeof(pstRam_Data->u8Data0204FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0204FF00))
				{
					MemCopy(pstRam_Data->u8Data0204FF00,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}							
			break;	
		case 0x0206FF00:
			MemSet(pstRam_Data->u8Data0206FF00,0xFF,sizeof(pstRam_Data->u8Data0206FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0206FF00))
				{
					MemCopy(pstRam_Data->u8Data0206FF00,&pstDlt645Frame_Info->u8RevBuf[14],8);
				}
			}
			break;		
		case 0x0207FF00:
			MemSet(pstRam_Data->u8Data0207FF00,0xFF,sizeof(pstRam_Data->u8Data0207FF00));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data0207FF00))
				{
					MemCopy(&pstRam_Data->u8Data0207FF00[6],&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}//err20180921 ��ȷ���Ƿ�Ϊ����
			}				
			break;
		case 0x040005FF:
			MemSet(pstRam_Data->u8Data040005FF,0xFF,sizeof(pstRam_Data->u8Data040005FF));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data040005FF))
				{
					MemCopy(&pstRam_Data->u8Data040005FF,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}				
			break;
		case 0x02800002:
			MemSet(pstRam_Data->u8Data02800002,0xFF,sizeof(pstRam_Data->u8Data02800002));
			if(u8CtrlCode == 0x91)
			{
				if((u8DataLen - 4) <= sizeof(pstRam_Data->u8Data02800002))
				{
					MemCopy(&pstRam_Data->u8Data02800002,&pstDlt645Frame_Info->u8RevBuf[14],u8DataLen - 4);
				}
			}	
			break;
	}
}


/**--------------------------------------------------------------------
*��    ��:LCD��ʾ������д���ڴ�
*�������:u16Pn��u32Ident��InBuf
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_WriteLcdData_TO_Ram_EDMI(u16 u16Pn,u32 u32Ident,u8* InBuf)
{
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = NULL;	
	u8 u8PnNum = 0,i = 0;
	
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_LCD_RAMDATA_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[i];
			break;
		}
	}	
	if(i >= u8PnNum)		//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstRam_Data = &pstPn_Ram_Data->stPnLcdRamData[u8PnNum];
			pstRam_Data->u16Pn = u16Pn;
			pstPn_Ram_Data->u8PnNum++;
		}
		else
		{
			return;
		}
	}
	switch(u32Ident)
	{
		case 0x0001FF00:
			MemCopy(pstRam_Data->u8Data0001FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0001FF00));				
			break;
		case 0x0002FF00:
			MemCopy(pstRam_Data->u8Data0002FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0002FF00));
			break;
		case 0x0003FF00:
			MemCopy(pstRam_Data->u8Data0003FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0003FF00));	
			break;
		case 0x0004FF00:
			MemCopy(pstRam_Data->u8Data0004FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0004FF00));
			break;	
		case 0x0201FF00:
			MemCopy(pstRam_Data->u8Data0201FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0201FF00));	
			break;
		case 0x0202FF00:
			MemCopy(pstRam_Data->u8Data0202FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0202FF00));			
			break;	
		case 0x0203FF00:
			MemCopy(pstRam_Data->u8Data0203FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0203FF00));				
			break;	
		case 0x0204FF00:
			MemCopy(pstRam_Data->u8Data0204FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0204FF00));				
			break;	
		case 0x0206FF00:
			MemCopy(pstRam_Data->u8Data0206FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0206FF00));
			break;		
		case 0x0207FF00:
			MemCopy(pstRam_Data->u8Data0207FF00,&InBuf[0],sizeof(pstRam_Data->u8Data0207FF00));		
			break;
		case 0x040005FF:
			MemCopy(pstRam_Data->u8Data040005FF,&InBuf[0],sizeof(pstRam_Data->u8Data040005FF));			
			break;
		case 0x02800002:
			MemCopy(pstRam_Data->u8Data02800002,&InBuf[0],sizeof(pstRam_Data->u8Data02800002));		
			break;
	}
}



/**--------------------------------------------------------------------
*��    ��:��������д��RAM�ݴ�
*�������:u16Pn��u8ProtocolType��pstDlt645Frame_Info
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_WriteData_TO_Ram(u16 u16Pn,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	u8 u8PnNum,i = 0,u8CtrlCode = 0,u8DataLen = 0;
	u32 u32Ident = 0;
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)		//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[u8PnNum];
			pstRam_Data->u16Pn = u16Pn;
			pstPn_Ram_Data->u8PnNum++;
		}
		else
		{
			return;
		}		
	}
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
	u8CtrlCode &= 0xDF;
	u8DataLen = pstDlt645Frame_Info->u8RevBuf[9];

	//err20181031 ��������� ���֡�������ݱ�ʶ
	if((u8CtrlCode & DLT_645_ERROR_BIT) == 0)//��֡Ϊ��ȷ֡
	{
		if(u8ProtocolType == METER_DLT645_97)
		{
			MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],2);
		}
		else
		{
			MemCopy(&u32Ident,&pstDlt645Frame_Info->u8RevBuf[10],4);		
		}
	}
	else //err20181031 ����֡�ӷ���֡��ȡ���ݱ�ʶ
		MemCopy(&u32Ident,&pstDlt645Frame_Info->u8SendBuf[10],4);
	switch(u32Ident)
	{
		case 0x0000901F:
			if(u8CtrlCode == 0x81)
			{
				for(i = 0;i < (u8DataLen/4);i++)
				{
					switch(i)
					{
						case 0:
							MemCopy(pstRam_Data->u8Data00010000,&pstDlt645Frame_Info->u8RevBuf[12],4);
							break;
						case 1:
							MemCopy(pstRam_Data->u8Data00010100,&pstDlt645Frame_Info->u8RevBuf[12 + 4],4);
							break;
						case 2:
							MemCopy(pstRam_Data->u8Data00010200,&pstDlt645Frame_Info->u8RevBuf[12 + 8],4);
							break;
						case 3:
							MemCopy(pstRam_Data->u8Data00010300,&pstDlt645Frame_Info->u8RevBuf[12 + 12],4);
							break;
						case 4:
							MemCopy(pstRam_Data->u8Data00010400,&pstDlt645Frame_Info->u8RevBuf[12 + 16],4);
							break;
					}
				}
			}
			else
			{
				MemSet(pstRam_Data->u8Data00010000,0xFF,4);
				MemSet(pstRam_Data->u8Data00010100,0xFF,4);
				MemSet(pstRam_Data->u8Data00010200,0xFF,4);
				MemSet(pstRam_Data->u8Data00010300,0xFF,4);
				MemSet(pstRam_Data->u8Data00010400,0xFF,4);
			}			
			break;
		case 0x0000902F:
			if(u8CtrlCode == 0x81)
			{
				for(i = 0;i < (u8DataLen/4);i++)
				{
					switch(i)
					{
						case 0:
							MemCopy(pstRam_Data->u8Data00020000,&pstDlt645Frame_Info->u8RevBuf[12],4);
							break;
						case 1:
							MemCopy(pstRam_Data->u8Data00020100,&pstDlt645Frame_Info->u8RevBuf[12 + 4],4);
							break;
						case 2:
							MemCopy(pstRam_Data->u8Data00020200,&pstDlt645Frame_Info->u8RevBuf[12 + 8],4);
							break;
						case 3:
							MemCopy(pstRam_Data->u8Data00020300,&pstDlt645Frame_Info->u8RevBuf[12 + 12],4);
							break;
						case 4:
							MemCopy(pstRam_Data->u8Data00020400,&pstDlt645Frame_Info->u8RevBuf[12 + 16],4);
							break;
					}
				}
				pstRam_Data->u8RdDoneFlag = TRUE;
			}
			else
			{
				MemSet(pstRam_Data->u8Data00020000,0xFF,4);
				MemSet(pstRam_Data->u8Data00020100,0xFF,4);
				MemSet(pstRam_Data->u8Data00020200,0xFF,4);
				MemSet(pstRam_Data->u8Data00020300,0xFF,4);
				MemSet(pstRam_Data->u8Data00020400,0xFF,4);
			}			
			break;
			//�й�����
		case 0xB630:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(pstRam_Data->u8Data0203FF00,&pstDlt645Frame_Info->u8RevBuf[12],3);
			}				
			break;
		case 0xB631:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0203FF00[3],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}						
			break;
		case 0xB632:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0203FF00[6],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}				
			break;
		case 0xB633:
			if(u8CtrlCode == 0x81)
			{
				MemCopy(&pstRam_Data->u8Data0203FF00[9],&pstDlt645Frame_Info->u8RevBuf[12],3);
			}							
			break;			
		case 0x0001FF00:
			if(u8CtrlCode == 0x91)
			{
				for(i = 0;i < (u8DataLen/4);i++)
				{
					switch(i)
					{
						case 0:
							MemCopy(pstRam_Data->u8Data00010000,&pstDlt645Frame_Info->u8RevBuf[14],4);
							break;
						case 1:
							MemCopy(pstRam_Data->u8Data00010100,&pstDlt645Frame_Info->u8RevBuf[14 + 4],4);
							break;
						case 2:
							MemCopy(pstRam_Data->u8Data00010200,&pstDlt645Frame_Info->u8RevBuf[14 + 8],4);
							break;
						case 3:
							MemCopy(pstRam_Data->u8Data00010300,&pstDlt645Frame_Info->u8RevBuf[14 + 12],4);
							break;
						case 4:
							MemCopy(pstRam_Data->u8Data00010400,&pstDlt645Frame_Info->u8RevBuf[14 + 16],4);
							break;
					}
				}
			}
			else
			{
				MemSet(pstRam_Data->u8Data00010000,0xFF,4);
				MemSet(pstRam_Data->u8Data00010100,0xFF,4);
				MemSet(pstRam_Data->u8Data00010200,0xFF,4);
				MemSet(pstRam_Data->u8Data00010300,0xFF,4);
				MemSet(pstRam_Data->u8Data00010400,0xFF,4);
			}
			break;
		case 0x0002FF00:
			if(u8CtrlCode == 0x91)
			{
				for(i = 0;i < (u8DataLen/4);i++)
				{
					switch(i)
					{
						case 0:
							MemCopy(pstRam_Data->u8Data00020000,&pstDlt645Frame_Info->u8RevBuf[14],4);
							break;
						case 1:
							MemCopy(pstRam_Data->u8Data00020100,&pstDlt645Frame_Info->u8RevBuf[14 + 4],4);
							break;
						case 2:
							MemCopy(pstRam_Data->u8Data00020200,&pstDlt645Frame_Info->u8RevBuf[14 + 8],4);
							break;
						case 3:
							MemCopy(pstRam_Data->u8Data00020300,&pstDlt645Frame_Info->u8RevBuf[14 + 12],4);
							break;
						case 4:
							MemCopy(pstRam_Data->u8Data00020400,&pstDlt645Frame_Info->u8RevBuf[14 + 16],4);
							break;
					}
				}				
				pstRam_Data->u8RdDoneFlag = TRUE;
			}
			else
			{
				MemSet(pstRam_Data->u8Data00020000,0xFF,4);
				MemSet(pstRam_Data->u8Data00020100,0xFF,4);
				MemSet(pstRam_Data->u8Data00020200,0xFF,4);
				MemSet(pstRam_Data->u8Data00020300,0xFF,4);
				MemSet(pstRam_Data->u8Data00020400,0xFF,4);
			}
			break;		
		case 0x0003FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data00030000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			}
			else
			{
				MemSet(pstRam_Data->u8Data00030000,0xFF,4);
			}
			break;	
		case 0x0004FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data00040000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			}
			else
			{
				MemSet(pstRam_Data->u8Data00040000,0xFF,4);
			}
			break;	
		case 0x0005FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data00050000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			}
			else
			{
				MemSet(pstRam_Data->u8Data00050000,0xFF,4);
			}
			break;
		case 0x0006FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data00060000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			}
			else
			{
				MemSet(pstRam_Data->u8Data00060000,0xFF,4);
			}
			break;		
		case 0x0007FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data00070000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			}
			else
			{
				MemSet(pstRam_Data->u8Data00070000,0xFF,4);
			}
			break;
		case 0x0008FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data00080000,&pstDlt645Frame_Info->u8RevBuf[14],4);
			}
			else
			{
				MemSet(pstRam_Data->u8Data00080000,0xFF,4);
			}
			break;	
		case 0x0201FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data0201FF00,&pstDlt645Frame_Info->u8RevBuf[14],6);
			}
			else
			{
				MemSet(pstRam_Data->u8Data0201FF00,0xFF,6);
			}
			break;
		case 0x0202FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data0202FF00,&pstDlt645Frame_Info->u8RevBuf[14],9);
			}
			else
			{
				MemSet(pstRam_Data->u8Data0202FF00,0xFF,9);
			}
			break;
		case 0x0203FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data0203FF00,&pstDlt645Frame_Info->u8RevBuf[14],12);
			}
			else
			{
				MemSet(pstRam_Data->u8Data0203FF00,0xFF,12);
			}
			break;
		case 0x0204FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data0204FF00,&pstDlt645Frame_Info->u8RevBuf[14],12);
			}
			else
			{
				MemSet(pstRam_Data->u8Data0204FF00,0xFF,12);
			}
			break;
		case 0x0206FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data0206FF00,&pstDlt645Frame_Info->u8RevBuf[14],8);
			}
			else
			{
				MemSet(pstRam_Data->u8Data0206FF00,0xFF,8);
			}
			break;
		case 0x0207FF00:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data0207FF00,&pstDlt645Frame_Info->u8RevBuf[14],6);//err20180921 �޸ı���
			}
			else
			{
				MemSet(pstRam_Data->u8Data0207FF00,0xFF,6);
			}
			break;			
		case 0x01010000:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data01010000,&pstDlt645Frame_Info->u8RevBuf[14],8);
			}
			else
			{
				MemSet(pstRam_Data->u8Data01010000,0xFF,8);
			}
			break;
		case 0x01020000:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data01020000,&pstDlt645Frame_Info->u8RevBuf[14],8);
			}
			else
			{
				MemSet(pstRam_Data->u8Data01020000,0xFF,8);
			}
			break;
		case 0x01030000:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data01030000,&pstDlt645Frame_Info->u8RevBuf[14],8);
			}
			else
			{
				MemSet(pstRam_Data->u8Data01030000,0xFF,8);
			}
			break;
		case 0x01040000:
			if(u8CtrlCode == 0x91)
			{
				MemCopy(pstRam_Data->u8Data01040000,&pstDlt645Frame_Info->u8RevBuf[14],8);
			}
			else
			{
				MemSet(pstRam_Data->u8Data01040000,0xFF,8);
			}
			break;
	}
	return;
}


/**--------------------------------------------------------------------
*��    ��:��������д��RAM�ݴ�
*�������:u16Pn��u32Ident��InBuf
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_WriteData_TO_Ram_EDMI(u16 u16Pn,u32 u32Ident,u8* InBuf)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	u8 u8PnNum,i = 0;
	//u32 u32Ident = 0;
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPn_Ram_Data,0,sizeof(QCSG_PN_RAM_DATA_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)		//��һ�α���
	{
		if(u8PnNum < MAX_PN_NUM2)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[u8PnNum];
			pstRam_Data->u16Pn = u16Pn;
			pstPn_Ram_Data->u8PnNum++;
		}
		else
		{
			return;
		}		
	}

	switch(u32Ident)
	{	
		case 0x0001FF00:			
			MemCopy(pstRam_Data->u8Data00010000,&InBuf[0],4);
			MemCopy(pstRam_Data->u8Data00010100,&InBuf[4],4);			
			MemCopy(pstRam_Data->u8Data00010200,&InBuf[8],4);			
			MemCopy(pstRam_Data->u8Data00010300,&InBuf[12],4);			
			MemCopy(pstRam_Data->u8Data00010400,&InBuf[16],4);											
			break;
		case 0x0002FF00:			
			MemCopy(pstRam_Data->u8Data00020000,&InBuf[0],4);							
			MemCopy(pstRam_Data->u8Data00020100,&InBuf[4],4);							
			MemCopy(pstRam_Data->u8Data00020200,&InBuf[8],4);							
			MemCopy(pstRam_Data->u8Data00020300,&InBuf[12],4);							
			MemCopy(pstRam_Data->u8Data00020400,&InBuf[16],4);						
			break;		
		case 0x0003FF00:			
			MemCopy(pstRam_Data->u8Data00030000,&InBuf[0],4);			
			break;	
		case 0x0004FF00:			
			MemCopy(pstRam_Data->u8Data00040000,&InBuf[0],4);			
			break;	
		case 0x0005FF00:			
			MemCopy(pstRam_Data->u8Data00050000,&InBuf[0],4);
			break;
		case 0x0006FF00:			
			MemCopy(pstRam_Data->u8Data00060000,&InBuf[0],4);			
			break;		
		case 0x0007FF00:			
			MemCopy(pstRam_Data->u8Data00070000,&InBuf[0],4);			
			break;
		case 0x0008FF00:			
			MemCopy(pstRam_Data->u8Data00080000,&InBuf[0],4);			
			break;	
		case 0x0201FF00:			
			MemCopy(pstRam_Data->u8Data0201FF00,&InBuf[0],6);
			break;
		case 0x0202FF00:
			MemCopy(pstRam_Data->u8Data0202FF00,&InBuf[0],9);			
			break;
		case 0x0203FF00:			
			MemCopy(pstRam_Data->u8Data0203FF00,&InBuf[0],12);			
			break;
		case 0x0204FF00:			
			MemCopy(pstRam_Data->u8Data0204FF00,&InBuf[0],12);			
			break;
		case 0x0206FF00:
			MemCopy(pstRam_Data->u8Data0206FF00,&InBuf[0],8);		
			break;
		case 0x0207FF00:	
			MemCopy(pstRam_Data->u8Data0207FF00,&InBuf[0],6);//err20180921 �޸ı���			
			break;			
		case 0x01010000:			
			MemCopy(pstRam_Data->u8Data01010000,&InBuf[0],8);			
			break;
		case 0x01020000:
			MemCopy(pstRam_Data->u8Data01020000,&InBuf[0],8);			
			break;
		case 0x01030000:		
			MemCopy(pstRam_Data->u8Data01030000,&InBuf[0],8);			
			break;
		case 0x01040000:			
			MemCopy(pstRam_Data->u8Data01040000,&InBuf[0],8);			
			break;
	}
	return;
}

/**--------------------------------------------------------------------
*��    ��:97645����ת��Ϊ�����������ݴ洢
*�������:u32Dlt97Ident:64597���ݱ�ʶ��pu8InData���������ݣ�u8InLen���������ݳ���
*�������:pstFrozenData:��������
*��    ��:��
--------------------------------------------------------------------**/
u8 DLT645DataToNW_CurveData(u32 u32Dlt97Ident,u8* pu8InData,u8 u8InLen,QCSG_FROZEN_RAM_DATA_S* pstFrozenData)
{
	u8 u8Res = TRUE;
	u32 u32Ident = 0,u32TempData = 0;
	switch(u32Dlt97Ident)
	{
		case 0xB611:
			u32Ident = 0x0201FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 18;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u32TempData,2);
				pstFrozenData->u32DataLen += (6 + 2);			
			}
			else
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 18;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,2);
				pstFrozenData->u32DataLen += (6 + 2);							
			}
			break;
		case 0xB612:
			if(u8InLen != 0)
			{
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TempData,2);
				pstFrozenData->u32DataLen += 2;			
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,2);
				pstFrozenData->u32DataLen += 2;							
			}
			break;			
		case 0xB613:
			if(u8InLen != 0)
			{
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TempData,2);
				pstFrozenData->u32DataLen += 2;			
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,2);
				pstFrozenData->u32DataLen += 2;							
			}
			MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,12);
			pstFrozenData->u32DataLen += 12;
			break;
		case 0xB621:
			u32Ident = 0x0202FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 18;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u32TempData,3);
				pstFrozenData->u32DataLen += (6 + 3);			
			}
			else
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 18;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,3);
				pstFrozenData->u32DataLen += (6 + 3);							
			}
			break;
		case 0xB622:
			if(u8InLen != 0)
			{
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TempData,3);
				pstFrozenData->u32DataLen += 3;			
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,3);
				pstFrozenData->u32DataLen += 3;							
			}
			break;			
		case 0xB623:
			if(u8InLen != 0)
			{
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TempData,3);
				pstFrozenData->u32DataLen += 3;			
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,3);
				pstFrozenData->u32DataLen += 3;							
			}
			MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,9);
			pstFrozenData->u32DataLen += 9;	
			break;
		case 0xB630:
			u32Ident = 0x0203FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 12;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],pu8InData,3);
				pstFrozenData->u32DataLen += (6 + 3);			
			}
			else
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 12;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,3);
				pstFrozenData->u32DataLen += (6 + 3);							
			}
			break;
		case 0xB631:
		case 0xB632:
		case 0xB633:
			if(u8InLen != 0)
			{
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],pu8InData,3);
				pstFrozenData->u32DataLen += 3;			
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,3);
				pstFrozenData->u32DataLen += 3;							
			}			
			break;
		case 0xB640:
			u32Ident = 0x0204FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 12;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u32TempData,3);
				pstFrozenData->u32DataLen += (6 + 3);	
			}
			else
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 12;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,3);
				pstFrozenData->u32DataLen += (6 + 3);							
			}
			break;
		case 0xB641:
		case 0xB642:
		case 0xB643:
			if(u8InLen != 0)
			{
				MemCopy(&u32TempData,pu8InData,2);
				u32TempData = (u32TempData << 4)& 0xFFFFFFF0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TempData,3);
				pstFrozenData->u32DataLen += 3;	
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,3);
				pstFrozenData->u32DataLen += 3;							
			}			
			break;
		case 0xB650:
			u32Ident = 0x0206FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 8;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],pu8InData,2);
				pstFrozenData->u32DataLen += (6 + 2);			
			}
			else
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 8;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,2);
				pstFrozenData->u32DataLen += (6 + 2);							
			}			
			break;
		case 0xB651:
		case 0xB652:
		case 0xB653:
			if(u8InLen != 0)
			{
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],pu8InData,2);
				pstFrozenData->u32DataLen += 2;			
			}
			else
			{
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],0xFF,2);
				pstFrozenData->u32DataLen += 2;							
			}				
			break;
		case 0x0201FF00:
			u32Ident = 0x0201FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 18;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,18);
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],pu8InData,6);
			}
			pstFrozenData->u32DataLen += (6 + 18);	
			break;
		case 0x0202FF00:
			u32Ident = 0x0202FF00;
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 18;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,18);
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],pu8InData,9);
			}
			pstFrozenData->u32DataLen += (6 + 18);	
			break;
		case 0x0207FF00:
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
			if(u8InLen != 0)
			{
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = 12;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,12);
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6 + 6],pu8InData,6);
			}
			pstFrozenData->u32DataLen += 12;	
			break;
		default:
			u8Res = FALSE;
	}
	return u8Res;
}
/**--------------------------------------------------------------------
*��    ��:�����������ݴ洢
*�������:pstFrozenTime:����ʱ�䣬u8FrozenFlag�����ݴ洢��־��,u32 u32Ident:���ݱ�ʶ��pu8MeterId�����ţ�pstDlt645Frame_Info��645����
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_METER_FrozenData_Save(u8 u8ChNo,TIME_PARAM_S* pstFrozenTime,u8 u8FrozenFlag,u32 u32Ident,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	//�������ݲ������¼�ṹ��
	QCSG_RECORD_FROZEN_PN_LIST_S* pstRD_Meter_TD_Pn_List = (QCSG_RECORD_FROZEN_PN_LIST_S*)QCSG_RECORD_FROZEN_PN_LIST;
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = NULL;
	RealTimeYYStruct stFrozenYYTime;
    u8 u8CtrlCode = 0,u8Dlt645DataLen = 0,u8Dlt645DataPos = 0,i = 0;
	
    if(u8FrozenFlag == 0)
    {
        return;
    }
	if((u8ChNo != RS485_1PORT)&&(u8ChNo != RS485_2PORT)&&(u8ChNo != RS485_3PORT)&&(u8ChNo != ACSAMPLEPORT))
	{
		return;
	}
	if(u8ChNo == RS485_1PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485I_RAM_FROZEN_DATA_S;
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485II_RAM_FROZEN_DATA_S;
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485III_RAM_FROZEN_DATA_S;
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;		
	}
    if(u8ProtocolType == METER_DLT645_97)
    {
        u8Dlt645DataLen = pstDlt645Frame_Info->u8RevBuf[9] - 2;
		u8Dlt645DataPos = 10 + 2;
    }
    else if(u8ProtocolType == METER_DLT645_07)
    {
        u8Dlt645DataLen = pstDlt645Frame_Info->u8RevBuf[9] - 4;	//��ȥ���ݱ�ʶ����=���ݵ�Ԫ����
		u8Dlt645DataPos = 10 + 4;		//�������ݱ�ʶ�������ݵ�Ԫ��ʼ
    }
    else
    {
        return;
    }
	RealTimeToYYTime(pstFrozenTime,&stFrozenYYTime);
	if(pstDlt645Frame_Info->u8RevLen != 0)
	{
		u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
		if((u8CtrlCode == 0xC1)||(u8CtrlCode == 0xD1))	//�쳣֡
		{//u32Identǰ���Ѿ���645��ʶת��������ʶ
			if(QCSG_GetDlt645DataLen_By_Ident(0x0D,u32Ident,&u8Dlt645DataLen)== TRUE)//07��Լ������TRUE��Ҳ��˵����07��Լ
			{
				//��������ֱ��д��RAM��
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = u8Dlt645DataLen;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,u8Dlt645DataLen);
				pstFrozenData->u32DataLen += (6 + u8Dlt645DataLen);			
			}
			else
			{
				if(u8ProtocolType == METER_DLT645_97)		//97��Լ
				{
					//������������ݱ�ʶת�����ڴ˴���
					u8Dlt645DataLen = 0;		//����Ϊ0ʱ��������������ḳFF
					DLT645DataToNW_CurveData(u32Ident,&u8Dlt645DataLen,u8Dlt645DataLen,pstFrozenData);
				}
			}
		}
		else
		{
			//��������ֱ��д��RAM��
			if(DLT645DataToNW_CurveData(u32Ident,&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],u8Dlt645DataLen,pstFrozenData) != TRUE)
			{
				//���û����ת������ֱ��д��RAM
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32Ident,4);
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = u8Dlt645DataLen;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&pstDlt645Frame_Info->u8RevBuf[u8Dlt645DataPos],u8Dlt645DataLen);
				pstFrozenData->u32DataLen += (6 + u8Dlt645DataLen);		
			}
		}
	}
	else
	{
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));		
		return;
	}
	if(pstFrozenData->u32SaveFlag == 0x01)//ĳ���������賭���������ű��浽flash���ļ���֮ǰֻ�Ǳ������ڴ�
	{
		if(pstFrozenData->u32DataLen != 0)
		{
			if((u8FrozenFlag & TD_D_TYPE) != 0)	//�����ն���
			{
				NAND_WriteMeterAMRData(AMR_DailyFreeze,pu8MeterId,0,u32Ident,&stFrozenYYTime,
				pstFrozenData->u8Data,&pstFrozenData->u32DataLen);
				for(i = 0;i < MAX_PN_NUM2;i++)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u16PnList[i] == u16Pn)
					{
						break;
					}
				}
				if(i >= MAX_PN_NUM2)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum < MAX_PN_NUM2)
					{
						pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u16PnList[pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum] = u16Pn;
						pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum++;
					}
				}
			}
			if((u8FrozenFlag & TD_M_TYPE) != 0)//�����¶���
			{
				NAND_WriteMeterAMRData(AMR_MonthFreeze,pu8MeterId,0,u32Ident,&stFrozenYYTime,
				pstFrozenData->u8Data,&pstFrozenData->u32DataLen);
				for(i = 0;i < MAX_PN_NUM2;i++)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u16PnList[i] == u16Pn)
					{
						break;
					}
				}
				if(i >= MAX_PN_NUM2)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum < MAX_PN_NUM2)
					{
						pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u16PnList[pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum] = u16Pn;
						pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum++;
					}
				}
			}
			if((u8FrozenFlag & TD_C_TYPE) != 0)	//��������
			{//pu8MeterId:����ַ u32Ident:�������ݱ�ʶ stFrozenYYTime:��ǰʱ�䱣��Ϊ����ʱ�� 
			// u8Data:N*(Fn(4)+L(2)(Data�ĳ���)+Data(n)) u32DataLen:����u8Data�ĳ���
				NAND_WriteMeterCurveData(AMR_CurveFreeze,pu8MeterId,0,u32Ident,&stFrozenYYTime,
				pstFrozenData->u8Data,&pstFrozenData->u32DataLen);
			}
		}
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
	}
}

/**--------------------------------------------------------------------
*��    ��:�����������ݴ洢 �����ר��
*�������:pstFrozenTime:����ʱ�䣬u8FrozenFlag�����ݴ洢��־��,u32 u32Ident:���ݱ�ʶ��
pu8MeterId�����ţ�
NWDataLen��������������(pu8InBuff)�ĳ���  			(ԭ����u8ProtocolType)
pu8InBuf�������������� (ԭ��pstDlt645Frame_Info��645����)

*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_METER_FrozenData_Save_EDMI(u8 u8ChNo,TIME_PARAM_S* pstFrozenTime,u8 u8FrozenFlag,u32 u32NWIdent,u16 u16Pn,u8* pu8MeterId,u8 u8NWDataLen,u8* pu8InBuf)
{
	//�������ݲ������¼�ṹ��
	QCSG_RECORD_FROZEN_PN_LIST_S* pstRD_Meter_TD_Pn_List = (QCSG_RECORD_FROZEN_PN_LIST_S*)QCSG_RECORD_FROZEN_PN_LIST;
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = NULL;
	RealTimeYYStruct stFrozenYYTime;
   //u8 u8CtrlCode = 0,u8Dlt645DataLen = 0,u8Dlt645DataPos = 0
	u8 i = 0;
	
    if(u8FrozenFlag == 0)
    {
        return;
    }
	if((u8ChNo != RS485_1PORT)&&(u8ChNo != RS485_2PORT)&&(u8ChNo != RS485_3PORT)&&(u8ChNo != ACSAMPLEPORT))
	{
		return;
	}
	if(u8ChNo == RS485_1PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485I_RAM_FROZEN_DATA_S;
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485II_RAM_FROZEN_DATA_S;
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485III_RAM_FROZEN_DATA_S;
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;		
	}

	RealTimeToYYTime(pstFrozenTime,&stFrozenYYTime);
	if(u8NWDataLen != 0)
	{
		//���û����ת������ֱ��д��RAM
		//ֱ����������ʶ ����Ҫת��
		MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32NWIdent,4);
		pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = u8NWDataLen;
		pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
		MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],pu8InBuf,u8NWDataLen);
		pstFrozenData->u32DataLen += (6 + u8NWDataLen);		
	}
	else
	{
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));		
		return;
	}
	if(pstFrozenData->u32SaveFlag == 0x01)//ĳ���������賭���������ű��浽flash���ļ���֮ǰֻ�Ǳ������ڴ�
	{
		if(pstFrozenData->u32DataLen != 0)
		{
			if((u8FrozenFlag & TD_D_TYPE) != 0)	//�����ն���
			{
				NAND_WriteMeterAMRData(AMR_DailyFreeze,pu8MeterId,0,u32NWIdent,&stFrozenYYTime,
				pstFrozenData->u8Data,&pstFrozenData->u32DataLen);
				for(i = 0;i < MAX_PN_NUM2;i++)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u16PnList[i] == u16Pn)
					{
						break;
					}
				}
				if(i >= MAX_PN_NUM2)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum < MAX_PN_NUM2)
					{
						pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u16PnList[pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum] = u16Pn;
						pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum++;
					}
				}
			}
			if((u8FrozenFlag & TD_M_TYPE) != 0)//�����¶���
			{
				NAND_WriteMeterAMRData(AMR_MonthFreeze,pu8MeterId,0,u32NWIdent,&stFrozenYYTime,
				pstFrozenData->u8Data,&pstFrozenData->u32DataLen);
				for(i = 0;i < MAX_PN_NUM2;i++)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u16PnList[i] == u16Pn)
					{
						break;
					}
				}
				if(i >= MAX_PN_NUM2)
				{
					if(pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum < MAX_PN_NUM2)
					{
						pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u16PnList[pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum] = u16Pn;
						pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum++;
					}
				}
			}
			if((u8FrozenFlag & TD_C_TYPE) != 0)	//��������
			{//pu8MeterId:����ַ u32Ident:�������ݱ�ʶ stFrozenYYTime:��ǰʱ�䱣��Ϊ����ʱ�� 
			// u8Data:N*(Fn(4)+L(2)(Data�ĳ���)+Data(n)) u32DataLen:����u8Data�ĳ���
				NAND_WriteMeterCurveData(AMR_CurveFreeze,pu8MeterId,0,u32NWIdent,&stFrozenYYTime,
				pstFrozenData->u8Data,&pstFrozenData->u32DataLen);
			}
		}
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
	}
}

/**--------------------------------------------------------------------
*��    ��:�������ص�645֡����
*�������:pstFrozenTime:����ʱ�䣬u8FrozenFlag�����ݴ洢��־��u16Pn��������ţ�pstDlt645Frame_Info��645����
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_METER_Dlt645Data_Process(u8 u8Cycle,u8 u8ChNo,TIME_PARAM_S* pstFrozenTime,u8 u8FrozenFlag,u16 u16Pn,u8* pu8MeterId,u8 u8ProtocolType,DLT645_FRAME_S* pstDlt645Frame_Info)
{
	MeasurePointStruct stPnPara;
	PowerQualityParaStruct stPowerQualityPara;
    u8 u8CtrlCode = 0,u8DataLen = 0,i = 0;
	u16 u16UpLimit = 0,u16LowLimit = 0;
    u32 u32RequestIdent = 0;
    u32 u32ResponseIdent = 0,u32ReadLen = 0;
	
	TIME_PARAM_S stSysCurrTime;
	HAL_GetSystemTime(&stSysCurrTime);

	if(pstDlt645Frame_Info->u8RevLen == 0)
	{
	//�ú�������u8RevLenΪ0ʱ��ֱ���ͷ���ʱ����pstFrozenData��
	//Ȼ��return���൱�ڻ���û�б������ݵ�flash���ļ�
	//Ϊ�ζ��һ��?
       	RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
		return;
	}
    if(QCSG_Dlt645FrameJudge(pstDlt645Frame_Info->u8SendBuf,&pstDlt645Frame_Info->u8SendLen) != TRUE)
    {
        return;
    }
    if(QCSG_Dlt645FrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
    {
        return;
    }
	u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];	//ȡ������ 91 D1
	u8DataLen = pstDlt645Frame_Info->u8RevBuf[9];	//ȡ���ݳ��� 
	for(i = 0;i < u8DataLen;i++)
	{
		pstDlt645Frame_Info->u8RevBuf[10 + i] -= 0x33;			
	}	
    if(u8ProtocolType == METER_DLT645_97)
    {
		for(i = 0;i < 2;i++)
		{
			pstDlt645Frame_Info->u8SendBuf[10 + i] -= 0x33;
		}
        MemCopy(&u32RequestIdent,&pstDlt645Frame_Info->u8SendBuf[10],2);
        MemCopy(&u32ResponseIdent,&pstDlt645Frame_Info->u8RevBuf[10],2);
    }
    if(u8ProtocolType == METER_DLT645_07)
    {
		for(i = 0;i < 4;i++)
		{
			pstDlt645Frame_Info->u8SendBuf[10 + i] -= 0x33;
		}
	    MemCopy(&u32RequestIdent,&pstDlt645Frame_Info->u8SendBuf[10],4);
	    MemCopy(&u32ResponseIdent,&pstDlt645Frame_Info->u8RevBuf[10],4);
	}
	
	//err20181031 ��������� ���֡�������ݱ�ʶ
	if((u8CtrlCode & DLT_645_ERROR_BIT) == 0)//��֡Ϊ��ȷ֡
	{
	    if(u32RequestIdent != u32ResponseIdent)
	    {
			if((u8CtrlCode != 0xC1)&&(u8CtrlCode != 0xD1))
			{
				return;
			}
	    }
	}
    switch(u32RequestIdent)
    {
		case 0x0000FF00:
			u32RequestIdent = 0x050600FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;		
		case 0x05060101:
			u32RequestIdent = 0x050601FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060201:
			u32RequestIdent = 0x050602FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060301:
			u32RequestIdent = 0x050603FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060401:
			u32RequestIdent = 0x050604FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060501:
			u32RequestIdent = 0x050605FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060601:
			u32RequestIdent = 0x050606FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060701:
			u32RequestIdent = 0x050607FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060801:
			u32RequestIdent = 0x050608FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060901:
			u32RequestIdent = 0x050609FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x05060A01:
			u32RequestIdent = 0x05060AFF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0001FF00:
			RD_Calc_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_Calc_Lcd_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			//E200002C 		ʾ���½�  ARD3
			RD_Alarm_E200002C(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			
			//E2000030 		��澯 ARD3
			RD_Alarm_E2000030(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0002FF00:
			RD_Calc_Lcd_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200002C(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0003FF00:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0004FF00:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;	
		case 0x0005FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0006FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0007FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0008FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x01010000:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x01020000:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x01030000:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x01040000:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0201FF00:

			
		
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			//E2000005		��ѹ��ƽ�� ARD2
			RD_Alarm_E2000005(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			
			//E200001A 		A ���ѹ��ѹ ARD2
			RD_Alarm_E200001A(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);

			//E200001B 		B ���ѹ��ѹ ARD2
			RD_Alarm_E200001B(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);

			//E200001C 		C ���ѹ��ѹ ARD2
			RD_Alarm_E200001C(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);


			if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) == NO_ERROR)
			{
				if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
				{
					break;
				}
				stPnPara.RatedVoltage = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);

//��������������	
				if(NAND_ReadMeasurePoint(u16Pn,0xE0800100,(u8*)&stPowerQualityPara,&u32ReadLen) == NO_ERROR)
				{
					if((QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800102,2) != TRUE) || (stPowerQualityPara.E0800102 == 0))
					{
						u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
					}
					else
					{
						stPowerQualityPara.E0800102 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800102>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800102);						
						u16UpLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800102 / 100;					
					}
					if((QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800103,2) != TRUE) || (stPowerQualityPara.E0800103 == 0))
					{
						u16LowLimit = stPnPara.RatedVoltage * 93 / 100;												
					}					
					else
					{
						stPowerQualityPara.E0800103 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800103>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800103);
						u16LowLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800103 / 100;						
					}
				}
				else
				{
					u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
					u16LowLimit = stPnPara.RatedVoltage * 93 / 100;
				}


				
#if(TEST_ON == 1) //err20180802 ����30Sһ�� ֻȡ0s�Ľ���ͳ�ơ�
			if(((stSysCurrTime.u8Second >=0x30) && (stSysCurrTime.u8Second <=0x35)) && (u16Pn == 0)) return;
#endif
				//QCSG_LogToRS232Port((u32)u16Pn,(u32)&stSysCurrTime.u8Second,1);
				RD_StatisticsData_E1000010(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_E1000011(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_E1000012(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_E1000013(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_03100000(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_03100100(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_03100200(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
				RD_StatisticsData_03100300(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
			}

#if defined(FKGA43_CL3200_TEST)
			if(((stSysCurrTime.u8Second >=0x30) && (stSysCurrTime.u8Second <=0x35)) && (u16Pn == 0)) return;
			RD_StatisticsData_E1000020_21_CL3200(u8Cycle,u16Pn,pstDlt645Frame_Info);
#endif
			break;
		case 0x0202FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000004(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200000D(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000010(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000016(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200000E(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000011(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000017(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200000F(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000012(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000018(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200001D(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200001E(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200001F(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010004(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010005(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010006(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010007(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010008(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010009(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E201000B(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//err20181105 ���Ӷ����¼�A��
			RD_Event_E201000C(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//err20181105 ���Ӷ����¼�B��
			RD_Event_E201000D(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//err20181105 ���Ӷ����¼�C��
			RD_StatisticsData_E1000014_16(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004014_16(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);

		//ABC��ʧ���澯
			RD_Alarm_E2000013(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000014(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000015(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);

			
#if !defined(FKGA43_CL3200_TEST)
			RD_StatisticsData_E1000020_21(u8Cycle,u16Pn,pstDlt645Frame_Info);
#endif			
			break;
		case 0x02800001:
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000006(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1000017(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004017(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02800002:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);			
			break;
		case 0x0203FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200002D(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200002E(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1000018(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004018(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0204FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1000019(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004019(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0206FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000026(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E2000027(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E100001A(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E100401A(u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			
#if(TEST_ON == 1) //err20180802 ����30Sһ�� ֻȡ0s�Ľ���ͳ�ơ�			
			if(((stSysCurrTime.u8Second >=0x30) && (stSysCurrTime.u8Second <=0x35)) && (u16Pn == 0)) return;
#endif	
			RD_StatisticsData_E1000033(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1000034(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1000035(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1000036(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004033(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004034(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004035(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_E1004036(u8Cycle,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x0207FF00:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x03300200:
			RD_Alarm_E2000031(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x03300400://err20181107 �����¼� Уʱ��¼
			RD_Event_E2010010(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x04000501:
			RD_Alarm_E2000032(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x04000507:
			RD_Alarm_E2000003(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x040005FF:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x14000001:
			RD_Alarm_E2000003(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02080100://A���ѹ����ʧ���
			RD_Alarm_E2000020(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02080200://B���ѹ����ʧ���
			RD_Alarm_E2000021(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02080300://C���ѹ����ʧ���
			RD_Alarm_E2000022(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02090100:
			RD_Alarm_E2000023(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02090200:
			RD_Alarm_E2000024(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02090300:
			RD_Alarm_E2000025(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x02050000:	//˲ʱ�����ڹ���
			RD_Alarm_E2000029(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Alarm_E200002A(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x03300000://����ܴ���
			RD_Alarm_E2000035(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//���ܱ����ʱ����� ARD2
			break;
		case 0x04010001:	//��һ�׵�1��ʱ�α�����
			RD_Alarm_E2000036(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x04020001://err20181101 ���ӵڶ��׵�һ��ʱ�α����ݸ澯�ж�
			RD_Alarm_E2000036(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x04000409:
			RD_Alarm_E2000037(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//���ܱ����峣������ ARD6
			break;
		case 0x0400040A:
			RD_Alarm_E2000037(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x04000101://:���ڼ�����
			RD_Alarm_E200003E(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x04000102://ʱ��
			RD_Alarm_E200003E(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x03300E00:
			RD_Alarm_E200004D(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010011(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//err20181106 �����¼� ����ť�п�����¼
			break;
		case 0x03300D00:
			RD_Alarm_E200004E(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_Event_E2010012(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);//err20181106 �����¼� ���ǿ�����¼
			break;
		case 0x00900200:
			RD_Alarm_E200002B(pstFrozenTime,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);		
			break;
		case 0x901F:
			if((u8FrozenFlag & TD_D_TYPE) != 0)
			{
				u32RequestIdent = 0x050601FF;
			}
			else if((u8FrozenFlag & TD_C_TYPE) != 0)
			{
				u32RequestIdent = 0x0001FF00;
			}
			else
			{
				break;
			}
			RD_Calc_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_Calc_Lcd_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);	
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);			
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x902F:
			if((u8FrozenFlag & TD_D_TYPE) != 0)
			{
				u32RequestIdent = 0x050602FF;
			}
			else if((u8FrozenFlag & TD_C_TYPE) != 0)
			{
				u32RequestIdent = 0x0002FF00;
			}
			else
			{
				break;
			}
			RD_Calc_Lcd_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);	
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);			
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x911F:
			u32RequestIdent = 0x050603FF;
			RD_Calc_Lcd_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);			
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x912F:
			u32RequestIdent = 0x050604FF;
			RD_Calc_Lcd_Energy_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);			
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x913F:
			u32RequestIdent = 0x050605FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x914F:
			u32RequestIdent = 0x050608FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x915F:
			u32RequestIdent = 0x050606FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0x916F:
			u32RequestIdent = 0x050607FF;
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB611:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) == NO_ERROR)
			{
				if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
				{
					break;
				}
				stPnPara.RatedVoltage = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
				if(NAND_ReadMeasurePoint(u16Pn,0xE0800100,(u8*)&stPowerQualityPara,&u32ReadLen) == NO_ERROR)
				{
					if((QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800102,2) != TRUE) || (QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800103,2) != TRUE))
					{
						u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
						u16LowLimit = stPnPara.RatedVoltage * 93 / 100;						
					}
					else
					{
						stPowerQualityPara.E0800102 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800102>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800102);
						stPowerQualityPara.E0800103 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800103>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800103);
						u16UpLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800102 / 100;
						u16LowLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800103 / 100;
					}
				}
				else
				{
					u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
					u16LowLimit = stPnPara.RatedVoltage * 93 / 100;
				}
			}				
			RD_StatisticsData_E1000011(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
			RD_StatisticsData_03100100(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);		
			break;
		case 0xB612:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) == NO_ERROR)
			{
				if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
				{
					break;
				}
				stPnPara.RatedVoltage = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
				if(NAND_ReadMeasurePoint(u16Pn,0xE0800100,(u8*)&stPowerQualityPara,&u32ReadLen) == NO_ERROR)
				{
					if((QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800102,2) != TRUE) || (QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800103,2) != TRUE))
					{
						u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
						u16LowLimit = stPnPara.RatedVoltage * 93 / 100;						
					}
					else
					{
						stPowerQualityPara.E0800102 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800102>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800102);
						stPowerQualityPara.E0800103 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800103>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800103);
						u16UpLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800102 / 100;
						u16LowLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800103 / 100;
					}					
				}
				else
				{
					u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
					u16LowLimit = stPnPara.RatedVoltage * 93 / 100;
				}
			}			
			RD_StatisticsData_E1000012(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
			RD_StatisticsData_03100200(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
			break;
		case 0xB613:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
			if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) == NO_ERROR)
			{
				if(QCSG_BCDCheck((u8*)&stPnPara.RatedVoltage,2) != TRUE)
				{
					break;
				}
				stPnPara.RatedVoltage = QCSG_BCD2HEX((u8)(stPnPara.RatedVoltage>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.RatedVoltage);
				if(NAND_ReadMeasurePoint(u16Pn,0xE0800100,(u8*)&stPowerQualityPara,&u32ReadLen) == NO_ERROR)
				{
					if((QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800102,2) != TRUE) || (QCSG_BCDCheck((u8*)&stPowerQualityPara.E0800103,2) != TRUE))
					{
						u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
						u16LowLimit = stPnPara.RatedVoltage * 93 / 100;						
					}
					else
					{
						stPowerQualityPara.E0800102 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800102>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800102);
						stPowerQualityPara.E0800103 = QCSG_BCD2HEX((u8)(stPowerQualityPara.E0800103>>8)) * 100 + QCSG_BCD2HEX((u8)stPowerQualityPara.E0800103);
						u16UpLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800102 / 100;
						u16LowLimit = stPnPara.RatedVoltage * stPowerQualityPara.E0800103 / 100;
					}
				}
				else
				{
					u16UpLimit = stPnPara.RatedVoltage * 107 / 100;
					u16LowLimit = stPnPara.RatedVoltage * 93 / 100;
				}
			}	
			RD_StatisticsData_E1000013(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);	
			RD_StatisticsData_03100300(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);		
			RD_StatisticsData_E1000010_97(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			RD_StatisticsData_03100000_97(u8Cycle,u16UpLimit,u16LowLimit,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);			
			break;
		case 0xB621:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB622:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB623:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB630:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB631:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB632:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB633:
			RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;	
		case 0xB640:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB641:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB642:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB643:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB650:
			RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB651:
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB652:
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		case 0xB653:
			RD_METER_FrozenData_Save(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8ProtocolType,pstDlt645Frame_Info);
			break;
		default:
			break;
    }
}


/**--------------------------------------------------------------------
*��    ��:�������ص�֡���� �Ѿ��������������ݱ�ʶ����������
*�������:pstFrozenTime:����ʱ�䣬u8FrozenFlag�����ݴ洢��־��u16Pn��������ţ�pstDlt645Frame_Info��645����
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_METER_EDMIData_Process(u8 u8ChNo,TIME_PARAM_S* pstFrozenTime,u8 u8FrozenFlag,u16 u16Pn,u8* pu8MeterId,u8 u8NwDataLen,u8* pu8InBuf,u32 u32NWIdent)
{
	//MeasurePointStruct stPnPara;
	//PowerQualityParaStruct stPowerQualityPara;
   // u8 u8CtrlCode = 0,u8DataLen = 0,i = 0;
	//u16 u16UpLimit = 0,u16LowLimit = 0;
   // u32 u32RequestIdent = 0;
  //  u32 u32ResponseIdent = 0,u32ReadLen = 0;
	
	TIME_PARAM_S stSysCurrTime;
	HAL_GetSystemTime(&stSysCurrTime);

	//if(pstDlt645Frame_Info->u8RevLen == 0)
	//{
	//�ú�������u8RevLenΪ0ʱ��ֱ���ͷ���ʱ����pstFrozenData��
	//Ȼ��return���൱�ڻ���û�б������ݵ�flash���ļ�
	//Ϊ�ζ��һ��?
    //   	RD_METER_FrozenData_Save_EDMI(u8ChNo,pstFrozenTime,u8FrozenFlag,u32RequestIdent,u16Pn,pu8MeterId,u8NwDataLen,pu8InBuf);
	//	return;
	//}
    
    switch(u32NWIdent)
    {
    	case 0x050600FF:
		case 0x050601FF:
		case 0x050602FF:
		case 0x050603FF:
		case 0x050604FF:
		case 0x050605FF:
		case 0x050606FF:
		case 0x050607FF:
		case 0x050608FF:
		case 0x050609FF:
		case 0x05060AFF:
			RD_METER_FrozenData_Save_EDMI(u8ChNo,pstFrozenTime,u8FrozenFlag,u32NWIdent,u16Pn,pu8MeterId,u8NwDataLen,pu8InBuf);
			break;
		case 0x0001FF00:
		case 0x0002FF00:
		case 0x0003FF00:
		case 0x0004FF00:
			RD_WriteLcdData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			//RD_WriteLcdData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			//RD_WriteData_TO_Ram(u16Pn,u8ProtocolType,pstDlt645Frame_Info);
			RD_WriteData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			RD_METER_FrozenData_Save_EDMI(u8ChNo,pstFrozenTime,u8FrozenFlag,u32NWIdent,u16Pn,pu8MeterId,u8NwDataLen,pu8InBuf);
			break;	
		case 0x0005FF00:
		case 0x0006FF00:
		case 0x0007FF00:
		case 0x0008FF00:
		case 0x01010000:
		case 0x01020000:
		case 0x01030000:
		case 0x01040000:
			RD_WriteData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			break;
		case 0x0201FF00:
		case 0x0202FF00:
			RD_WriteData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			RD_WriteLcdData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			RD_METER_FrozenData_Save_EDMI(u8ChNo,pstFrozenTime,u8FrozenFlag,u32NWIdent,u16Pn,pu8MeterId,u8NwDataLen,pu8InBuf);
			break;
		case 0x02800001:
			RD_METER_FrozenData_Save_EDMI(u8ChNo,pstFrozenTime,u8FrozenFlag,u32NWIdent,u16Pn,pu8MeterId,u8NwDataLen,pu8InBuf);
			break;
		case 0x02800002:
			RD_WriteLcdData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);		
			break;
		case 0x0203FF00:
		case 0x0204FF00:
		case 0x0206FF00:
			RD_WriteData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			RD_WriteLcdData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			RD_METER_FrozenData_Save_EDMI(u8ChNo,pstFrozenTime,u8FrozenFlag,u32NWIdent,u16Pn,pu8MeterId,u8NwDataLen,pu8InBuf);
			break;
		case 0x0207FF00:
			RD_WriteData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			RD_WriteLcdData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			break;
		case 0x040005FF:
			RD_WriteLcdData_TO_Ram_EDMI(u16Pn,u32NWIdent,pu8InBuf);
			break;
		
		default:
			break;
    }
}





#if (DLT500_REPORT == 0)
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000010_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000011_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000012_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000013_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
#else
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000010_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000011_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000012_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000013_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_DAY;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);
			break;
		}
	}
}

#endif
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000014_16_DataCopy(u16 u16Pn,u32 u32Ident,u8* pu8OutBuf)
{
	QCSG_CURRENT_EXTREME_PASSRATE_LIST_S* pstCurrentExtreme_List = (QCSG_CURRENT_EXTREME_PASSRATE_LIST_S*)QCSG_CURRENT_EXTREME_DAY;
	QCSG_CURRENT_EXTREME_PASSRATE_S* pstCurrentExtreme_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstCurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstCurrentExtreme_List,0,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstCurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pstCurrentExtreme_info = &pstCurrentExtreme_List->stCurrentExtreme[i];
			if(u32Ident == 0xE1000014)
			{
				MemCopy(&pu8OutBuf[0],pstCurrentExtreme_info->u8Max_A_Current,3);
				MemCopy(&pu8OutBuf[3],pstCurrentExtreme_info->u8Max_A_Current_Time,4);
				MemCopy(&pu8OutBuf[7],pstCurrentExtreme_info->u8Min_A_Current,3);
				MemCopy(&pu8OutBuf[10],pstCurrentExtreme_info->u8Min_A_Current_Time,4);
			}
			if(u32Ident == 0xE1000015)
			{
				MemCopy(&pu8OutBuf[0],pstCurrentExtreme_info->u8Max_B_Current,3);
				MemCopy(&pu8OutBuf[3],pstCurrentExtreme_info->u8Max_B_Current_Time,4);
				MemCopy(&pu8OutBuf[7],pstCurrentExtreme_info->u8Min_B_Current,3);
				MemCopy(&pu8OutBuf[10],pstCurrentExtreme_info->u8Min_B_Current_Time,4);
			}
			if(u32Ident == 0xE1000016)
			{
				MemCopy(&pu8OutBuf[0],pstCurrentExtreme_info->u8Max_C_Current,3);
				MemCopy(&pu8OutBuf[3],pstCurrentExtreme_info->u8Max_C_Current_Time,4);
				MemCopy(&pu8OutBuf[7],pstCurrentExtreme_info->u8Min_C_Current,3);
				MemCopy(&pu8OutBuf[10],pstCurrentExtreme_info->u8Min_C_Current_Time,4);
			}			
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000017_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_N_CurrentExtreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_N_CURRENT_EXTREME_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_N_CurrentExtreme_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_N_CurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_N_CurrentExtreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_N_CurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_N_CurrentExtreme_info = &pst_N_CurrentExtreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_N_CurrentExtreme_info->u8Max_Value,3);
			MemCopy(&pu8OutBuf[3],pst_N_CurrentExtreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[7],pst_N_CurrentExtreme_info->u8Min_Value,3);
			MemCopy(&pu8OutBuf[10],pst_N_CurrentExtreme_info->u8Min_Value_Time,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000018_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Active_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_ACTIVE_POWER_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Active_Power_Extreme_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_Active_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Active_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Active_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Active_Power_Extreme_info = &pst_Active_Power_Extreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_Active_Power_Extreme_info->u8Max_Value,3);
			MemCopy(&pu8OutBuf[3],pst_Active_Power_Extreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[7],pst_Active_Power_Extreme_info->u8Min_Value,3);
			MemCopy(&pu8OutBuf[10],pst_Active_Power_Extreme_info->u8Min_Value_Time,4);			
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000019_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Reactive_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_REACTIVE_POWER_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Reactive_Power_Extreme_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_Reactive_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Reactive_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Reactive_Power_Extreme_info = &pst_Reactive_Power_Extreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_Reactive_Power_Extreme_info->u8Max_Value,3);
			MemCopy(&pu8OutBuf[3],pst_Reactive_Power_Extreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[7],pst_Reactive_Power_Extreme_info->u8Min_Value,3);
			MemCopy(&pu8OutBuf[10],pst_Reactive_Power_Extreme_info->u8Min_Value_Time,4);			
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE100001A_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Power_Factor_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_POWER_FACTOR_DAY;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Power_Factor_Extreme_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_Power_Factor_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Power_Factor_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Power_Factor_Extreme_info = &pst_Power_Factor_Extreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_Power_Factor_Extreme_info->u8Max_Value,2);
			MemCopy(&pu8OutBuf[2],pst_Power_Factor_Extreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[6],pst_Power_Factor_Extreme_info->u8Min_Value,2);
			MemCopy(&pu8OutBuf[8],pst_Power_Factor_Extreme_info->u8Min_Value_Time,4);				
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000020_DataCopy(u8* pu8OutBuf)
{		
	u32 u32ValueHex = 0,u32ValueBcd = 0;
	
	u8* pu8 = (u8*)QCSG_POWER_DOWN_NUM_DAY;
	MemCopy(&u32ValueHex,pu8,3);
	u32ValueBcd = hex_bcd(u32ValueHex);
	MemCopy(pu8OutBuf,&u32ValueBcd,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000021_DataCopy(u8* pu8OutBuf)
{		
	u32 u32ValueHex = 0,u32ValueBcd = 0;

	u8* pu8 = (u8*)QCSG_POWER_DOWN_TIME_DAY;
	MemCopy(&u32ValueHex,pu8,3);
	u32ValueBcd = hex_bcd(u32ValueHex);
	MemCopy(pu8OutBuf,&u32ValueBcd,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000033_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000034_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_A_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000035_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_B_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1000036_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_C_DAY;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
#if (DLT500_REPORT == 0)
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100000_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);			
			break;
		}				
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100100_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);			
			break;
		}				
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100200_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);			
			break;
		}				
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100300_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8Overrrun,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[17],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MinVoltageTime,4);			
			break;
		}				
	}
}
#else
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100000_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);			
			break;
		}				
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100100_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_A_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);	
			break;
		}				
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100200_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_B_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);			
			break;
		}				
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0x03100300_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VOLTAGE_PASSRATE_LIST_S* pstPassRate_List = (QCSG_VOLTAGE_PASSRATE_LIST_S*)QCSG_C_VOLTAGE_PASSRATE_MONTH;
	QCSG_VOLTAGE_PASSRATE_S* pstPassRate_info = NULL;
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPassRate_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPassRate_List,0,sizeof(QCSG_VOLTAGE_PASSRATE_LIST_S));
	}		
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPassRate_List->stPassRate[i].u16Pn)
		{
			pstPassRate_info = &pstPassRate_List->stPassRate[i];
			MemCopy(&pu8OutBuf[0],pstPassRate_info->u8MonitorTime,3);
			MemCopy(&pu8OutBuf[3],pstPassRate_info->u8PassRate,3);
			MemCopy(&pu8OutBuf[6],pstPassRate_info->u8UpLimintRate,3);
			MemCopy(&pu8OutBuf[9],pstPassRate_info->u8LowLimintRate,3);
			MemCopy(&pu8OutBuf[12],pstPassRate_info->u8PassRateTime,3);
			MemCopy(&pu8OutBuf[15],pstPassRate_info->u8UpLimitTime,3);
			MemCopy(&pu8OutBuf[18],pstPassRate_info->u8LowLimitTime,3);
			MemCopy(&pu8OutBuf[21],pstPassRate_info->u8MaxVoltage,2);
			MemCopy(&pu8OutBuf[23],pstPassRate_info->u8MaxVoltageTime,4);
			MemCopy(&pu8OutBuf[27],pstPassRate_info->u8MinVoltage,2);
			MemCopy(&pu8OutBuf[29],pstPassRate_info->u8MinVoltageTime,4);	
			break;
		}				
	}
}

#endif
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004014_16_DataCopy(u16 u16Pn,u32 u32Ident,u8* pu8OutBuf)
{
	QCSG_CURRENT_EXTREME_PASSRATE_LIST_S* pstCurrentExtreme_List = (QCSG_CURRENT_EXTREME_PASSRATE_LIST_S*)QCSG_CURRENT_EXTREME_MONTH;
	QCSG_CURRENT_EXTREME_PASSRATE_S* pstCurrentExtreme_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstCurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstCurrentExtreme_List,0,sizeof(QCSG_CURRENT_EXTREME_PASSRATE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstCurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pstCurrentExtreme_info = &pstCurrentExtreme_List->stCurrentExtreme[i];
			if(u32Ident == 0xE1004014)
			{
				MemCopy(&pu8OutBuf[0],pstCurrentExtreme_info->u8Max_A_Current,3);
				MemCopy(&pu8OutBuf[3],pstCurrentExtreme_info->u8Max_A_Current_Time,4);
				MemCopy(&pu8OutBuf[7],pstCurrentExtreme_info->u8Min_A_Current,3);
				MemCopy(&pu8OutBuf[10],pstCurrentExtreme_info->u8Min_A_Current_Time,4);
			}
			if(u32Ident == 0xE1004015)
			{
				MemCopy(&pu8OutBuf[0],pstCurrentExtreme_info->u8Max_B_Current,3);
				MemCopy(&pu8OutBuf[3],pstCurrentExtreme_info->u8Max_B_Current_Time,4);
				MemCopy(&pu8OutBuf[7],pstCurrentExtreme_info->u8Min_B_Current,3);
				MemCopy(&pu8OutBuf[10],pstCurrentExtreme_info->u8Min_B_Current_Time,4);
			}
			if(u32Ident == 0xE1004016)
			{
				MemCopy(&pu8OutBuf[0],pstCurrentExtreme_info->u8Max_C_Current,3);
				MemCopy(&pu8OutBuf[3],pstCurrentExtreme_info->u8Max_C_Current_Time,4);
				MemCopy(&pu8OutBuf[7],pstCurrentExtreme_info->u8Min_C_Current,3);
				MemCopy(&pu8OutBuf[10],pstCurrentExtreme_info->u8Min_C_Current_Time,4);
			}			
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004017_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_N_CurrentExtreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_N_CURRENT_EXTREME_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_N_CurrentExtreme_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_N_CurrentExtreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_N_CurrentExtreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_N_CurrentExtreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_N_CurrentExtreme_info = &pst_N_CurrentExtreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_N_CurrentExtreme_info->u8Max_Value,3);
			MemCopy(&pu8OutBuf[3],pst_N_CurrentExtreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[7],pst_N_CurrentExtreme_info->u8Min_Value,3);
			MemCopy(&pu8OutBuf[10],pst_N_CurrentExtreme_info->u8Min_Value_Time,4);
			break;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004018_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Active_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_ACTIVE_POWER_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Active_Power_Extreme_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_Active_Power_Extreme_List->u8PnNum;	
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Active_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Active_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Active_Power_Extreme_info = &pst_Active_Power_Extreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_Active_Power_Extreme_info->u8Max_Value,3);
			MemCopy(&pu8OutBuf[3],pst_Active_Power_Extreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[7],pst_Active_Power_Extreme_info->u8Min_Value,3);
			MemCopy(&pu8OutBuf[10],pst_Active_Power_Extreme_info->u8Min_Value_Time,4);			
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004019_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Reactive_Power_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_REACTIVE_POWER_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Reactive_Power_Extreme_info = NULL;			
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_Reactive_Power_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Reactive_Power_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Reactive_Power_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Reactive_Power_Extreme_info = &pst_Reactive_Power_Extreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_Reactive_Power_Extreme_info->u8Max_Value,3);
			MemCopy(&pu8OutBuf[3],pst_Reactive_Power_Extreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[7],pst_Reactive_Power_Extreme_info->u8Min_Value,3);
			MemCopy(&pu8OutBuf[10],pst_Reactive_Power_Extreme_info->u8Min_Value_Time,4);			
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE100401A_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_VALUE_EXTREME_PASSRATE_LIST_S* pst_Power_Factor_Extreme_List = (QCSG_VALUE_EXTREME_PASSRATE_LIST_S*)QCSG_POWER_FACTOR_MONTH;
	QCSG_VALUE_EXTREME_PASSRATE_S* pst_Power_Factor_Extreme_info = NULL;	
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pst_Power_Factor_Extreme_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pst_Power_Factor_Extreme_List,0,sizeof(QCSG_VALUE_EXTREME_PASSRATE_LIST_S));
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pst_Power_Factor_Extreme_List->stCurrentExtreme[i].u16Pn)
		{
			pst_Power_Factor_Extreme_info = &pst_Power_Factor_Extreme_List->stCurrentExtreme[i];
			MemCopy(&pu8OutBuf[0],pst_Power_Factor_Extreme_info->u8Max_Value,2);
			MemCopy(&pu8OutBuf[2],pst_Power_Factor_Extreme_info->u8Max_Value_Time,4);
			MemCopy(&pu8OutBuf[6],pst_Power_Factor_Extreme_info->u8Min_Value,2);
			MemCopy(&pu8OutBuf[8],pst_Power_Factor_Extreme_info->u8Min_Value_Time,4);				
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004020_DataCopy(u8* pu8OutBuf)
{		
	u32 u32ValueHex = 0,u32ValueBcd = 0;

	u8* pu8 = (u8*)QCSG_POWER_DOWN_NUM_MONTH;
	MemCopy(&u32ValueHex,pu8,3);
	u32ValueBcd = hex_bcd(u32ValueHex);
	MemCopy(pu8OutBuf,&u32ValueBcd,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004021_DataCopy(u8* pu8OutBuf)
{		
	u32 u32ValueHex = 0,u32ValueBcd = 0;

	u8* pu8 = (u8*)QCSG_POWER_DOWN_TIME_MONTH;
	MemCopy(&u32ValueHex,pu8,3);
	u32ValueBcd = hex_bcd(u32ValueHex);
	MemCopy(pu8OutBuf,&u32ValueBcd,3);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004033_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004034_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_A_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004035_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_B_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݿ���
*�������:u16Pn��������ţ�
*�������:pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_0xE1004036_DataCopy(u16 u16Pn,u8* pu8OutBuf)
{
	QCSG_POWER_FACTOR_ZONE_LIST_S* pstPowerFactor_Zone_List = (QCSG_POWER_FACTOR_ZONE_LIST_S*)QCSG_POWER_FACTOR_ZONE_C_MONTH;
	QCSG_POWER_FACTOR_ZONE_S* pstPowerFactor_Zone_info = NULL;		
	u8 u8PnNum = 0,i = 0;	

	u8PnNum = pstPowerFactor_Zone_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM2)
	{
		MemSet(pstPowerFactor_Zone_List,0,sizeof(QCSG_POWER_FACTOR_ZONE_LIST_S));
	}
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼
		if(u16Pn == pstPowerFactor_Zone_List->stPowerFactorZone[i].u16Pn)
		{
			pstPowerFactor_Zone_info = &pstPowerFactor_Zone_List->stPowerFactorZone[i];
			MemCopy(&pu8OutBuf[0],pstPowerFactor_Zone_info->u8MonitorTime,33);
			break;
		}		
	}
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݸ�ֵ
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_RtStatisticsData(u16 u16Pn,u32 u32Ident,u8* pu8OutBuf,u16* pu16OutLen)
{
	(*pu16OutLen) = 0;
	MemCopy(pu8OutBuf,&u16Pn,2);
	MemCopy(&pu8OutBuf[4],&u32Ident,4);
	switch(u32Ident)
	{
		case 0xE1000001:
		case 0xE1000002:
			(*pu16OutLen) = 4;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;
		case 0xE1000003:
		case 0xE1000004:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;		
		case 0xE1000008:	//ʣ����
			(*pu16OutLen) = 4;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;
		case 0xE1000010:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000010_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;
		case 0xE1000011:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000011_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1000012:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000012_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1000013:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000013_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;
		case 0xE1000014:
		case 0xE1000015:
		case 0xE1000016:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000014_16_DataCopy(u16Pn,u32Ident,&pu8OutBuf[8]);
			break;	
		case 0xE1000017:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000017_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;		
		case 0xE1000018:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000018_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1000019:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000019_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE100001A:
			(*pu16OutLen) = 12;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE100001A_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;		
		case 0xE100001B:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE100001C:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;		
		case 0xE1000020:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1000020_DataCopy(&pu8OutBuf[8]);
			}
			break;	
		case 0xE1000021:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1000021_DataCopy(&pu8OutBuf[8]);
			}
			break;	
		case 0xE1000030:
			(*pu16OutLen) = 72;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;		
		case 0xE1000031:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE1000032:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE1000033:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000033_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1000034:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000034_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1000035:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000035_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;		
		case 0xE1000036:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1000036_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;			
		case 0xE1000040:
		case 0xE1000041:
		case 0xE1000042:
		case 0xE1000043:	
		case 0xE1000044:
		case 0xE1000045:			
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;			
		case 0xE1000101:
		case 0xE1000102:
		case 0xE1000103:
		case 0xE1000104:
		case 0xE1000105:
		case 0xE1000106:
		case 0xE1000107:
		case 0xE1000108:
		case 0xE1000109:
		case 0xE100010A:
		case 0xE100010B:
		case 0xE100010C:
		case 0xE100010D:
		case 0xE100010E:
		case 0xE100010F:
		case 0xE1000110:			
		case 0xE1000201:
		case 0xE1000202:
		case 0xE1000203:
		case 0xE1000204:
		case 0xE1000205:
		case 0xE1000206:
		case 0xE1000207:
		case 0xE1000208:
		case 0xE1000209:
		case 0xE100020A:
		case 0xE100020B:
		case 0xE100020C:
		case 0xE100020D:
		case 0xE100020E:
		case 0xE100020F:
		case 0xE1000210:
		case 0xE1000301:
		case 0xE1000302:
		case 0xE1000303:
		case 0xE1000304:
		case 0xE1000305:
		case 0xE1000306:
		case 0xE1000307:
		case 0xE1000308:
		case 0xE1000309:
		case 0xE100030A:
		case 0xE100030B:
		case 0xE100030C:
		case 0xE100030D:
		case 0xE100030E:
		case 0xE100030F:
		case 0xE1000310:			
			(*pu16OutLen) = 2;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE10001FF:
		case 0xE10002FF:
		case 0xE10003FF:			
			(*pu16OutLen) = 32;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;			
		case 0x03100000:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0x03100000_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;
		case 0x03100100:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0x03100100_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0x03100200:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0x03100200_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0x03100300:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0x03100300_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1004014:
		case 0xE1004015:
		case 0xE1004016:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004014_16_DataCopy(u16Pn,u32Ident,&pu8OutBuf[8]);
			break;	
		case 0xE1004017:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004017_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;			
		case 0xE1004018:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004018_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1004019:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004019_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE100401A:
			(*pu16OutLen) = 12;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE100401A_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE100401B:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE100401C:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE1004020:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1004020_DataCopy(&pu8OutBuf[8]);
			}
			break;	
		case 0xE1004021:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1004021_DataCopy(&pu8OutBuf[8]);
			}
			break;
		case 0xE1004030:
			(*pu16OutLen) = 72;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;		
		case 0xE1004031:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;	
		case 0xE1004032:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			break;
		case 0xE1004033:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004033_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;
		case 0xE1004034:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004034_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;	
		case 0xE1004035:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004035_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;
		case 0xE1004036:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[8],0xFF,(*pu16OutLen));
			RD_0xE1004036_DataCopy(u16Pn,&pu8OutBuf[8]);
			break;		
	}
	MemCopy(&pu8OutBuf[2],pu16OutLen,2);
}
/**--------------------------------------------------------------------
*��    ��:ͳ�����ݸ�ֵ
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_FrozenStatisticsData(u16 u16Pn,u32 u32Ident,u8* pu8OutBuf,u16* pu16OutLen)
{
	(*pu16OutLen) = 0;	
	switch(u32Ident)
	{
		case 0xE1000008:
			(*pu16OutLen) = 4;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;
		case 0xE1000010:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000010_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;
		case 0xE1000011:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000011_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE1000012:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000012_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE1000013:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000013_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;
		case 0xE1000014:
		case 0xE1000015:
		case 0xE1000016:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000014_16_DataCopy(u16Pn,u32Ident,&pu8OutBuf[0]);
			break;	
		case 0xE1000017:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000017_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;		
		case 0xE1000018:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000018_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE1000019:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000019_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE100001A:
			(*pu16OutLen) = 12;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE100001A_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;		
		case 0xE100001B:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE100001C:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;		
		case 0xE1000020:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1000020_DataCopy(&pu8OutBuf[0]);
			}
			break;	
		case 0xE1000021:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1000021_DataCopy(&pu8OutBuf[0]);
			}
			break;	
		case 0xE1000030:
			(*pu16OutLen) = 72;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;		
		case 0xE1000031:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE1000032:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE1000033:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000033_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;
		case 0xE1000034:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000034_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;		
		case 0xE1000035:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000035_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;		
		case 0xE1000036:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1000036_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;			
		case 0xE1000040:
		case 0xE1000041:
		case 0xE1000042:
		case 0xE1000043:	
		case 0xE1000044:
		case 0xE1000045:			
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;			
		case 0xE1000101:
		case 0xE1000102:
		case 0xE1000103:
		case 0xE1000104:
		case 0xE1000105:
		case 0xE1000106:
		case 0xE1000107:
		case 0xE1000108:
		case 0xE1000109:
		case 0xE100010A:
		case 0xE100010B:
		case 0xE100010C:
		case 0xE100010D:
		case 0xE100010E:
		case 0xE100010F:
		case 0xE1000110:			
		case 0xE1000201:
		case 0xE1000202:
		case 0xE1000203:
		case 0xE1000204:
		case 0xE1000205:
		case 0xE1000206:
		case 0xE1000207:
		case 0xE1000208:
		case 0xE1000209:
		case 0xE100020A:
		case 0xE100020B:
		case 0xE100020C:
		case 0xE100020D:
		case 0xE100020E:
		case 0xE100020F:
		case 0xE1000210:
		case 0xE1000301:
		case 0xE1000302:
		case 0xE1000303:
		case 0xE1000304:
		case 0xE1000305:
		case 0xE1000306:
		case 0xE1000307:
		case 0xE1000308:
		case 0xE1000309:
		case 0xE100030A:
		case 0xE100030B:
		case 0xE100030C:
		case 0xE100030D:
		case 0xE100030E:
		case 0xE100030F:
		case 0xE1000310:			
			(*pu16OutLen) = 2;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE10001FF:
		case 0xE10002FF:
		case 0xE10003FF:			
			(*pu16OutLen) = 32;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;			
		case 0x03100000:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0x03100000_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;
		case 0x03100100:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0x03100100_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0x03100200:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0x03100200_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0x03100300:
#if (DLT500_REPORT == 0)
			(*pu16OutLen) = 27;
#else
			(*pu16OutLen) = 33;
#endif
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0x03100300_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE1004014:
		case 0xE1004015:
		case 0xE1004016:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004014_16_DataCopy(u16Pn,u32Ident,&pu8OutBuf[0]);
			break;	
		case 0xE1004017:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004017_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;			
		case 0xE1004018:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004018_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE1004019:
			(*pu16OutLen) = 14;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004019_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE100401A:
			(*pu16OutLen) = 12;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE100401A_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;	
		case 0xE100401B:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE100401C:
			(*pu16OutLen) = 9;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE1004020:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1004020_DataCopy(&pu8OutBuf[0]);
			}
			break;	
		case 0xE1004021:
			(*pu16OutLen) = 3;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			if(u16Pn == 0)
			{
				RD_0xE1004021_DataCopy(&pu8OutBuf[0]);
			}
			break;
		case 0xE1004030:
			(*pu16OutLen) = 72;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;		
		case 0xE1004031:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;	
		case 0xE1004032:
			(*pu16OutLen) = 40;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			break;
		case 0xE1004033:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004033_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;
		case 0xE1004034:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004034_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;			
		case 0xE1004035:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004035_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;
		case 0xE1004036:
			(*pu16OutLen) = 33;
			MemSet(&pu8OutBuf[0],0xFF,(*pu16OutLen));
			RD_0xE1004036_DataCopy(u16Pn,&pu8OutBuf[0]);
			break;			
	}
}
/**--------------------------------------------------------------------
*��    ��:����ն���ͳ�����ݸ�ֵ
*�������:��
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_ClearTDStatisticsData(u8 u8Flag)
{
	MemSet((u8*)QCSG_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_A_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_B_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_C_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_CURRENT_EXTREME_DAY,0,1024);
	MemSet((u8*)QCSG_N_CURRENT_EXTREME_DAY,0,512);
	MemSet((u8*)QCSG_ACTIVE_POWER_DAY,0,512);
	MemSet((u8*)QCSG_REACTIVE_POWER_DAY,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_DAY,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_A_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_B_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_C_DAY,0,1024);
	if(u8Flag == 1)
	{
		MemSet((u8*)QCSG_POWER_DOWN_NUM_DAY,0,4);
		MemSet((u8*)QCSG_POWER_DOWN_TIME_DAY,0,4);
	}
}
/**--------------------------------------------------------------------
*��    ��:����¶���ͳ�����ݸ�ֵ
*�������:��
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_ClearTMStatisticsData(u8 u8Flag)
{
	MemSet((u8*)QCSG_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_A_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_B_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_C_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_CURRENT_EXTREME_MONTH,0,1024);
	MemSet((u8*)QCSG_N_CURRENT_EXTREME_MONTH,0,512);
	MemSet((u8*)QCSG_ACTIVE_POWER_MONTH,0,512);
	MemSet((u8*)QCSG_REACTIVE_POWER_MONTH,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_MONTH,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_A_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_B_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_C_MONTH,0,1024);
	if(u8Flag == 1)
	{
		MemSet((u8*)QCSG_POWER_DOWN_NUM_MONTH,0,4);
		MemSet((u8*)QCSG_POWER_DOWN_TIME_MONTH,0,4);	
	}
}
/**--------------------------------------------------------------------
*��    ��:�������
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_ClearAlarmData(void)
{
	//���ܱ����߸澯��������
	MemSet((u8*)QCSG_ALARM_E200002D_LIST,0,512);	
	//ʱ�ӵ�ع��͸澯��������
	MemSet((u8*)QCSG_ALARM_E2000032_LIST,0,512);
	//���ܱ�ʱ���쳣�澯��������
	MemSet((u8*)QCSG_ALARM_E200003E_LIST,0,512);
}
/**--------------------------------------------------------------------
*��    ��:�ն���ͳ������ת��
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_TDStatisticsDataSave(TIME_PARAM_S* pstTime)
{
	//ͳ�����ݴ洢û�з�ͨ���洢��������һ��list�У��������JCRAM����ʱ����
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stFrozenYYTime;
	u8 i = 0,j = 0;
	u16 u16Pn = 0,u16ReadLen = 0;
	u32 u32CurrIdent = 0,u32TDIdent = 0,u32ReadLen = 0;
	
	RealTimeToYYTime(pstTime,&stFrozenYYTime);
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
		u16Pn = (u16)SwitchNumToPn(i);
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			continue;
		}
		j = 0;
		while(1)
		{
			u32CurrIdent = gc_StatisticsIdent_Table[j].u32CurrDayStatisticsIdent;
			u32TDIdent = gc_StatisticsIdent_Table[j].u32TDStatisticsIdent;
			j++;
			if((u16Pn != 0) && (u32CurrIdent == 0xE1000020))	// (����)��ͣ���ܴ���
			{
				continue;
			}
			if((u16Pn != 0) && (u32CurrIdent == 0xE1000021))	//(����)��ͣ����ʱ��
			{
				continue;
			}
			if((u32CurrIdent == 0xFFFFFFFF) || (u32CurrIdent == 0))
			{
				break;
			}
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TDIdent,4);
			RD_FrozenStatisticsData(u16Pn,u32CurrIdent,&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u16ReadLen);	
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = (u8)u16ReadLen;
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
			pstFrozenData->u32DataLen += (6 + u16ReadLen);		
		}
		NAND_WriteMeterAMRData(AMR_DailyStatisticFreeze,stPnPara.Addr,0,u32TDIdent,&stFrozenYYTime,
		pstFrozenData->u8Data,&pstFrozenData->u32DataLen);		
	}
}

/**--------------------------------------------------------------------
*��    ��:�ն���ͳ������ת��_���
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_TDStatisticsDataSave_PB(TIME_PARAM_S* pstTime)
{
	//ͳ�����ݴ洢û�з�ͨ���洢��������һ��list�У��������JCRAM����ʱ����
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stFrozenYYTime;
	u8 j = 0;
	u16 u16Pn = 0,u16ReadLen = 0;
	u32 u32CurrIdent = 0,u32TDIdent = 0,u32ReadLen = 0;
	
	RealTimeToYYTime(pstTime,&stFrozenYYTime);

		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			return;//continue;
		}
		j = 0;
		while(1)
		{
			u32CurrIdent = gc_StatisticsIdent_Table[j].u32CurrDayStatisticsIdent;
			u32TDIdent = gc_StatisticsIdent_Table[j].u32TDStatisticsIdent;
			j++;

			if((u32CurrIdent == 0xFFFFFFFF) || (u32CurrIdent == 0))
			{
				break;
			}
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TDIdent,4);
			RD_FrozenStatisticsData(u16Pn,u32CurrIdent,&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u16ReadLen);	
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = (u8)u16ReadLen;
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
			pstFrozenData->u32DataLen += (6 + u16ReadLen);		
		}
		NAND_WriteMeterAMRData(AMR_DailyStatisticFreeze,stPnPara.Addr,0,u32TDIdent,&stFrozenYYTime,
		pstFrozenData->u8Data,&pstFrozenData->u32DataLen);		
	
}

/**--------------------------------------------------------------------
*��    ��:�¶���ͳ������ת��
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_TMStatisticsDataSave(TIME_PARAM_S* pstTime)
{
	//ͳ�����ݴ洢û�з�ͨ���洢��������һ��list�У��������JCRAM����ʱ����
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stFrozenYYTime;	
	u8 i = 0,j = 0;
	u16 u16Pn = 0,u16ReadLen = 0;
	u32 u32CurrIdent = 0,u32TMIdent = 0,u32ReadLen = 0;

	RealTimeToYYTime(pstTime,&stFrozenYYTime);	
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
		u16Pn = (u16)SwitchNumToPn(i);
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			continue;
		}		
		j = 0;
		while(1)
		{
			u32CurrIdent = gc_StatisticsIdent_Table[j].u32CurrMonthStatisticsIdent;
			u32TMIdent = gc_StatisticsIdent_Table[j].u32TMStatisticsIdent;
			j++;
			if((u16Pn != 0) && (u32CurrIdent == 0xE1004020))
			{
				continue;
			}
			if((u16Pn != 0) && (u32CurrIdent == 0xE1004021))
			{
				continue;
			}			
			if((u32CurrIdent == 0xFFFFFFFF) || (u32CurrIdent == 0))
			{
				break;
			}
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TMIdent,4);
			RD_FrozenStatisticsData(u16Pn,u32CurrIdent,&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u16ReadLen);	
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = (u8)u16ReadLen;
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
			pstFrozenData->u32DataLen += (6 + u16ReadLen);		
		}
		NAND_WriteMeterAMRData(AMR_MonthStatisticFreeze,stPnPara.Addr,0,u32TMIdent,&stFrozenYYTime,
		pstFrozenData->u8Data,&pstFrozenData->u32DataLen);		
	}	
}

/**--------------------------------------------------------------------
*��    ��:�¶���ͳ������ת��_���
*�������:u32Ident�����ݱ�ʶ��u16MaxOutSize�������С
*�������:pu16OutLen:������ȣ�pu8OutBuf���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_TMStatisticsDataSave_PB(TIME_PARAM_S* pstTime)
{
	//ͳ�����ݴ洢û�з�ͨ���洢��������һ��list�У��������JCRAM����ʱ����
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stFrozenYYTime;	
	u8 j = 0;
	u16 u16Pn = 0,u16ReadLen = 0;
	u32 u32CurrIdent = 0,u32TMIdent = 0,u32ReadLen = 0;

	RealTimeToYYTime(pstTime,&stFrozenYYTime);	
	
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			return;//continue;
		}		
		j = 0;
		while(1)
		{
			u32CurrIdent = gc_StatisticsIdent_Table[j].u32CurrMonthStatisticsIdent;
			u32TMIdent = gc_StatisticsIdent_Table[j].u32TMStatisticsIdent;
			j++;			
			if((u32CurrIdent == 0xFFFFFFFF) || (u32CurrIdent == 0))
			{
				break;
			}
			MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32TMIdent,4);
			RD_FrozenStatisticsData(u16Pn,u32CurrIdent,&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],&u16ReadLen);	
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = (u8)u16ReadLen;
			pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
			pstFrozenData->u32DataLen += (6 + u16ReadLen);		
		}
		NAND_WriteMeterAMRData(AMR_MonthStatisticFreeze,stPnPara.Addr,0,u32TMIdent,&stFrozenYYTime,
		pstFrozenData->u8Data,&pstFrozenData->u32DataLen);			
}

/**--------------------------------------------------------------------
*��    ��:645���ݱ�ʶ���������ݱ�ʶ��ת��
*�������:u8ConvertMethod��ת��������pu8InData�����뻺�壬u16InLen�����볤��
*�������:pu8OutData��������壬pu16OutLen���������
*��    ��:��
--------------------------------------------------------------------**/
void RD_DLT645Data_To_NwData(u8 u8ConvertMethod,u8* pu8InData,u8 u8InLen,u8* pu8OutData,u8* pu8OutLen)
{
	TIME_PARAM_S stFrozenTime;
	u32 u32Value = 0;
	
	switch(u8ConvertMethod)
	{
		case COMMON_COV_METHOD:
		//����ֱ��ת��
			MemCopy(pu8OutData,pu8InData,u8InLen);
			(*pu8OutLen) = u8InLen;
			break;
		case PRICE_COV_METHOD:
		//����ͷ����1���ֽڷ����ֶ�
			if((u8InLen/4) > 0)
			{
				pu8OutData[0] = u8InLen/4 - 1;
				MemCopy(&pu8OutData[1],pu8InData,u8InLen);
				(*pu8OutLen) = u8InLen + 1;
			}
			else
			{
				pu8OutData[0] = 0x04;
				(*pu8OutLen) = 21;
				MemSet(&pu8OutData[1],0xFF,20);
			}
			break;
		case HBYTE_ADD_YEAR_COV_METHOD:
		//���ֽڲ���1���ֽ������Ϣ,����97�����������ʱ��ת��
			if(HAL_GetSystemTime(&stFrozenTime) != TRUE)
			{
				stFrozenTime.u8Year = 0x00;
			}	
			if(QCSG_BCDCheck(pu8InData,u8InLen) != TRUE)
			{
				stFrozenTime.u8Year = 0xFF;
			}
			MemCopy(&pu8OutData[0],pu8InData,u8InLen);
			pu8OutData[u8InLen] = stFrozenTime.u8Year;
			(*pu8OutLen) = u8InLen + 1;			
			break;
		case HLBIT_ADD_ZERO_COV_METHOD:
		//����λ���㣬����λ���㣬����97Э���������ת��
			if(u8InLen < 4)
			{
				if(QCSG_BCDCheck(pu8InData,u8InLen) != TRUE)
				{
					(*pu8OutLen) = u8InLen + 1;
					MemSet(pu8OutData,0xFF,(*pu8OutLen));
				}
				else
				{
					MemCopy(&u32Value,pu8InData,u8InLen);
					u32Value = (u32Value << 4) & 0xFFFFFFF0;
					(*pu8OutLen) = u8InLen + 1;
					MemCopy(pu8OutData,&u32Value,(*pu8OutLen));
				}
			}
			break;
		case LBYTE_ADD_ZERO_COV_METHOD:
		//���ֽڲ����㣬����97�޹����ʸ�ʽת��	
			if(QCSG_BCDCheck(pu8InData,u8InLen) != TRUE)
			{
				pu8OutData[0] = 0xFF;
			}
			else
			{
				pu8OutData[0] = 0x00;
			}
			MemCopy(&pu8OutData[1],pu8InData,u8InLen);
			(*pu8OutLen) = u8InLen + 1;
			break;
		case HBYTE_ADD_ZERO_COV_METHOD:
			if(QCSG_BCDCheck(pu8InData,u8InLen) != TRUE)
			{
				pu8OutData[u8InLen] = 0xFF;
			}
			else
			{
				pu8OutData[u8InLen] = 0x00;
			}
			(*pu8OutLen) = u8InLen + 1;
			break;
		case TAKE_PRICE_DATA_COV_METHOD:
		//�����������ȡ�������ݣ���Ҫ���ڶ������ݴ��������ڶ��ᴦ��û�ñ��񣬹ʴ���Ŀǰ����
//			i = pu8InData[0];
//			i *= 4;
//			MR((u32)&pu8OutData[0], (u32)&pu8InData[0], 4);
//			MR((u32)&pu8OutData[4], (u32)&pu8InData[4 + i], 4);
//			(*pu8OutLen) = 8;
			break;
		case LBIT_ADD_ZERO_COV_METHOD:
			if((QCSG_BCDCheck(pu8InData,u8InLen) != TRUE) || (u8InLen > 4))
			{
				(*pu8OutLen) = u8InLen;
				MemSet(pu8OutData,0xFF,(*pu8OutLen));
			}			
			MemCopy(&u32Value,pu8InData,u8InLen);
			u32Value = (u32Value << 4) & 0xFFFFFFF0;
			MemCopy(pu8OutData,&u32Value,u8InLen);
			(*pu8OutLen) = u8InLen;
			break;
		case LBYTE_ADD_PRICE_METHOD:
			pu8OutData[0] = 0x04;
			MemCopy(&pu8OutData[1],pu8InData,u8InLen);
			(*pu8OutLen) = u8InLen + 1;			
			break;
		case DEMAND_PRICE_COV_METHOD:
		//����ͷ����1���ֽڷ����ֶ�,���������ݵĴ���
			if((u8InLen/8) > 0)
			{
				pu8OutData[0] = u8InLen/8 - 1;
				MemCopy(&pu8OutData[1],pu8InData,u8InLen);
				(*pu8OutLen) = u8InLen + 1;
			}
			else
			{
				pu8OutData[0] = 0x04;
				(*pu8OutLen) = 41;
				MemSet(&pu8OutData[1],0xFF,40);
			}			
			break;
			
		case LBYTE_ABANDON_HBYTE_ZERO:	//20190507 ʣ�����õ�
			if(QCSG_BCDCheck(pu8InData,u8InLen) != TRUE){	//�ظ�d1�򳭲���
				MemSet(pu8OutData,0xFF,4);
			}
			else{
				MemCopy(pu8OutData,&pu8InData[1],3);
				pu8OutData[3]=0;
			}
			(*pu8OutLen) = u8InLen;
			break;

				
		default:
			MemCopy(pu8OutData,pu8InData,u8InLen);
			(*pu8OutLen) = u8InLen;
			break;
	}
}


 /**--------------------------------------------------------------------
 *��	 ��:645���ݱ�ʶ���������ݱ�ʶ��ת��
 *�������:u8ConvertMethod��ת��������pu8InData�����뻺�壬u16InLen�����볤��
 *�������:pu8OutData��������壬pu16OutLen���������
 *��	 ��:��
 --------------------------------------------------------------------**/
 void RD_EDMIData_To_NwData(u8 u8ConvertMethod,u8* pu8InData,u8 u8InLen,u8* pu8OutData,u8* pu8OutLen)
 {
 	 u8 i=0;
	 //TIME_PARAM_S stFrozenTime;
	 u64 u64Value = 0;
	 u32 u32Value = 0;//��λ10Wh
	 u64 Float_Value = 0;
	 u64 Output_Value[2];//��λWh
	 u32 u32Output_Value[2];
	 u8 Zero_Value[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	 u8 Zero_Time[6] = {1,1,96,0,0,0};//�������׼��׼ʱ��96��1��1��0��0��0�� HEX��ʽ
	 u8 u8Sign_bit=0;

	switch(u8ConvertMethod)
	{
		case EDMI_IEEE754_FLOAT_METHOD://������������  ֱ��ȡ
			if(MemCompare(pu8InData, Zero_Value, 8) == 0)
			{
				MemSet(pu8OutData,0x00,4);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] /10;// ��λת��XX   XXXX Wh -> XXXXXX.XX KWH
				pu8OutData[3] = QCSG_HEX2BCD((u8)(u32Value/1000000%100));
				pu8OutData[2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
			}
			(*pu8OutLen) = 4;
			break;	
		case EDMI_IEEE754_DOUBLE_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 8) == 0)
			{
				MemSet(pu8OutData,0x00,4);
			}
			else 
			{
				Float_Value = ((u64)pu8InData[0]<<56) + ((u64)pu8InData[1]<<48) + ((u64)pu8InData[2]<<40) + ((u64)pu8InData[3]<<32)	+ ((u64)pu8InData[4]<<24) + ((u64)pu8InData[5]<<16) + ((u64)pu8InData[6]<<8) + ((u64)pu8InData[7]);
				IEEE754_Double_Arithmetic(Float_Value,Output_Value);
				u64Value = Output_Value[0] /10;// ��λת��XX   XXXX Wh -> XXXXXX.XX KWH
				pu8OutData[3] = QCSG_HEX2BCD((u8)(u64Value/1000000%100));
				pu8OutData[2] = QCSG_HEX2BCD((u8)(u64Value/10000%100));
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u64Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u64Value/1%100));
			}
			(*pu8OutLen) = 4 ;
			break;
		case PRICE_COV_EDMI_IEEE754_DOUBLE_METHOD:
			pu8OutData[0] = 4;
			for(i=0;i<5;i++)
			{
				if(MemCompare(&pu8InData[i*8], Zero_Value, 8) == 0)
				{
					MemSet(&pu8OutData[i*4+1],0x00,4);
				}
				else 
				{
					Float_Value = ((u64)pu8InData[i*8]<<56) + ((u64)pu8InData[i*8+1]<<48) + ((u64)pu8InData[i*8+2]<<40) + ((u64)pu8InData[i*8+3]<<32)	+ ((u64)pu8InData[i*8+4]<<24) + ((u64)pu8InData[i*8+5]<<16) + ((u64)pu8InData[i*8+6]<<8) + ((u64)pu8InData[i*8+7]);
					IEEE754_Double_Arithmetic(Float_Value,Output_Value);
					u64Value = Output_Value[0] /10;// ��λת��XX   XXXX Wh -> XXXXXX.XX KWH
					pu8OutData[i*4+4] = QCSG_HEX2BCD((u8)(u64Value/1000000%100));
					pu8OutData[i*4+3] = QCSG_HEX2BCD((u8)(u64Value/10000%100));
					pu8OutData[i*4+2] = QCSG_HEX2BCD((u8)(u64Value/100%100));
					pu8OutData[i*4+1] = QCSG_HEX2BCD((u8)(u64Value/1%100));
				}
			}
			(*pu8OutLen) = 21;
			break;
		case NOPRICE_COV_EDMI_IEEE754_DOUBLE_METHOD://�����ӷ����ֶ� �������ߺͶ���
			//pu8OutData[0] = 4;
			for(i=0;i<5;i++)
			{
				if(MemCompare(&pu8InData[i*8], Zero_Value, 8) == 0)
				{
					MemSet(&pu8OutData[i*4],0x00,4);
				}
				else 
				{
					Float_Value = ((u64)pu8InData[i*8]<<56) + ((u64)pu8InData[i*8+1]<<48) + ((u64)pu8InData[i*8+2]<<40) + ((u64)pu8InData[i*8+3]<<32)	+ ((u64)pu8InData[i*8+4]<<24) + ((u64)pu8InData[i*8+5]<<16) + ((u64)pu8InData[i*8+6]<<8) + ((u64)pu8InData[i*8+7]);
					IEEE754_Double_Arithmetic(Float_Value,Output_Value);
					u64Value = Output_Value[0] /10;// ��λת��XX   XXXX Wh -> XXXXXX.XX KWH
					pu8OutData[i*4+3] = QCSG_HEX2BCD((u8)(u64Value/1000000%100));
					pu8OutData[i*4+2] = QCSG_HEX2BCD((u8)(u64Value/10000%100));
					pu8OutData[i*4+1] = QCSG_HEX2BCD((u8)(u64Value/100%100));
					pu8OutData[i*4+0] = QCSG_HEX2BCD((u8)(u64Value/1%100));
				}
			}
			(*pu8OutLen) = 20;
			break;
		case EDMI_REQ_BLOCK_METHOD:
			pu8OutData[0] = 4;
			for(i=0;i<5;i++)
			{
				if(MemCompare(&pu8InData[i*10], Zero_Value, 4) == 0)//�ж������Ƿ���Ч
				{
					MemSet(&pu8OutData[i*8+1],0x00,3);
				}
				else
				{
					Float_Value = (pu8InData[i*10+0]<<24) + (pu8InData[i*10+1]<<16) + (pu8InData[i*10+2]<<8) + (pu8InData[i*10+3]);
					IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
					u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;//XXXXW -> XXXXXX 0.1W 
					pu8OutData[i*8+2+1] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
					pu8OutData[i*8+1+1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
					pu8OutData[i*8+0+1] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				}
				if(MemCompare(&pu8InData[i*10+4], Zero_Time, 6) == 0)//�ж�ʱ���Ƿ���Ч 
				{
					MemSet(&pu8OutData[i*8+3+1],0x00,5);
				}
				else 
				{
					pu8OutData[i*8+3+4+1] = QCSG_HEX2BCD(pu8InData[i*10+4+2]);//�� 96
					pu8OutData[i*8+3+3+1] = QCSG_HEX2BCD(pu8InData[i*10+4+1]);//�� 1
					pu8OutData[i*8+3+2+1] = QCSG_HEX2BCD(pu8InData[i*10+4+0]);//�� 1
					pu8OutData[i*8+3+1+1] = QCSG_HEX2BCD(pu8InData[i*10+4+3]);//ʱ 0
					pu8OutData[i*8+3+0+1] = QCSG_HEX2BCD(pu8InData[i*10+4+4]);//�� 0
				}
			}
			(*pu8OutLen) = 41;
			break;
		case NOPRICE_COV_EDMI_REQ_BLOCK_METHOD://�����ӷ����ֶ� �������ߺͶ���
			//pu8OutData[0] = 4;
			for(i=0;i<5;i++)
			{
				if(MemCompare(&pu8InData[i*10], Zero_Value, 4) == 0)//�ж������Ƿ���Ч
				{
					MemSet(&pu8OutData[i*8],0x00,3);
				}
				else
				{
					Float_Value = (pu8InData[i*10+0]<<24) + (pu8InData[i*10+1]<<16) + (pu8InData[i*10+2]<<8) + (pu8InData[i*10+3]);
					IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
					u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;//XXXXW -> XXXXXX 0.1W 
					pu8OutData[i*8+2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
					pu8OutData[i*8+1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
					pu8OutData[i*8+0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				}
				if(MemCompare(&pu8InData[i*10+4], Zero_Time, 6) == 0)//�ж�ʱ���Ƿ���Ч
				{
					MemSet(&pu8OutData[i*8+3],0x00,5);
				}
				else 
				{
					pu8OutData[i*8+3+4] = QCSG_HEX2BCD(pu8InData[i*10+4+2]);//��
					pu8OutData[i*8+3+3] = QCSG_HEX2BCD(pu8InData[i*10+4+1]);//��
					pu8OutData[i*8+3+2] = QCSG_HEX2BCD(pu8InData[i*10+4+0]);//��
					pu8OutData[i*8+3+1] = QCSG_HEX2BCD(pu8InData[i*10+4+3]);//ʱ
					pu8OutData[i*8+3+0] = QCSG_HEX2BCD(pu8InData[i*10+4+4]);//��
				}
			}
			(*pu8OutLen) = 40;
			break;
		case EDMI_REQ_SINGLE_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)//�ж������Ƿ���Ч
			{
				MemSet(pu8OutData,0x00,3);
			}
			else
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;//XXXXW -> XXXXXX 0.1W 
				pu8OutData[2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
			}
			if(MemCompare(&pu8InData[4], Zero_Time, 6) == 0)//�ж�ʱ���Ƿ���Ч
			{
				MemSet(&pu8OutData[3],0x00,5);
			}
			else 
			{
				pu8OutData[3+4] = QCSG_HEX2BCD(pu8InData[4+2]);//�� //ǰ4��������
				pu8OutData[3+3] = QCSG_HEX2BCD(pu8InData[4+1]);//��
				pu8OutData[3+2] = QCSG_HEX2BCD(pu8InData[4+0]);//��
				pu8OutData[3+1] = QCSG_HEX2BCD(pu8InData[4+3]);//ʱ
				pu8OutData[3+0] = QCSG_HEX2BCD(pu8InData[4+4]);//��
			}
			(*pu8OutLen) = 8;
			break;
		case EDMI_VOLTAGE_BLOCK_METHOD:
			for(i=0;i<3;i++)
			{
				if(MemCompare(&pu8InData[i*4], Zero_Value, 4) == 0)
				{
					MemSet(&pu8OutData[i*2],0x00,2);
				}
				else 
				{
					Float_Value = (pu8InData[i*4+0]<<24) + (pu8InData[i*4+1]<<16) + (pu8InData[i*4+2]<<8) + (pu8InData[i*4+3]);
					IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
					u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;// ��λת��XXXX V -> XXXX 0.1V     XXXXXX Wh -> XXXXXXXX 10WH
					pu8OutData[i*2+1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
					pu8OutData[i*2+0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				}
			}
			MemSet(&pu8OutData[6],0xFF,12);
			(*pu8OutLen) = 18;
			break;
		case EDMI_VOLTAGE_SINGLE_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)
			{
				MemSet(pu8OutData,0x00,2);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;// ��λת��XXXX V -> XXXX 0.1V     XXXXXX Wh -> XXXXXXXX 10WH
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
			}
			(*pu8OutLen) = 2;
			break;	
		case EDMI_CURRENT_BLOCK_METHOD://�������ݿ�
			for(i=0;i<3;i++)
			{
				if(MemCompare(&pu8InData[i*4], Zero_Value, 4) == 0)
				{
					MemSet(&pu8OutData[i*3],0x00,3);
				}
				else 
				{
					Float_Value = (pu8InData[i*4+0]<<24) + (pu8InData[i*4+1]<<16) + (pu8InData[i*4+2]<<8) + (pu8InData[i*4+3]);
					IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
					u32Value = u32Output_Value[0] *1000 + u32Output_Value[1]*10;//XXXXA -> XXXXXX 0.001A
					pu8OutData[i*3+2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
					pu8OutData[i*3+1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
					pu8OutData[i*3+0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				}
			}
			MemSet(&pu8OutData[9],0xFF,9);
			(*pu8OutLen) = 18;
			break;
		case EDMI_CURRENT_SINGLE_METHOD://�������� ����
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)
			{
				MemSet(pu8OutData,0x00,3);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] *1000 + u32Output_Value[1]*10;//XXXXA -> XXXXXX 0.001A
				// ��λת��XXXX V -> XXXX 0.1V (*10) 
				//		XXXXXX Wh -> XXXXXXXX 10WH (/10)
				pu8OutData[2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
			}
			(*pu8OutLen) = 3;
			break;	
		case EDMI_POWER_BLOCK_METHOD://�������ݿ�
			for(i=0;i<4;i++)
			{
				if(MemCompare(&pu8InData[i*4], Zero_Value, 4) == 0)
				{
					MemSet(&pu8OutData[i*3],0x00,3);
				}
				else 
				{
					Float_Value = (pu8InData[i*4+0]<<24) + (pu8InData[i*4+1]<<16) + (pu8InData[i*4+2]<<8) + (pu8InData[i*4+3]);
					IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
					u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;//XXXXW -> XXXXXX 0.1W
					pu8OutData[i*3+2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
					pu8OutData[i*3+1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
					pu8OutData[i*3+0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				}
			}
			(*pu8OutLen) = 12;
			break;
		case EDMI_POWER_SINGLE_METHOD://�������� ����
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)
			{
				MemSet(pu8OutData,0x00,3);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] *10 + u32Output_Value[1]/10;//XXXXW -> XXXXXX 0.1W
				// ��λת��XXXX V -> XXXX 0.1V (*10) 
				//		XXXXXX Wh -> XXXXXXXX 10WH (/10)
				pu8OutData[2] = QCSG_HEX2BCD((u8)(u32Value/10000%100));
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
			}
			(*pu8OutLen) = 3;
			break;	
		case EDMI_FACTOR_BLOCK_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)
			{
				MemSet(pu8OutData,0x00,2);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				u8Sign_bit = IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] *1000 + u32Output_Value[1]*10;//XXXXQ -> XXXX 0.001Q
				
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				if(u8Sign_bit == 1) 
					pu8OutData[1] |= 0x80;
			}
			MemSet(&pu8OutData[2],0xFF,6);//�������������ֻ���� ������FF
			(*pu8OutLen) = 8;
			break;	
		case EDMI_FACTOR_SINGLE_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)
			{
				MemSet(pu8OutData,0x00,2);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				u8Sign_bit = IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				u32Value = u32Output_Value[0] *1000 + u32Output_Value[1]*10;//XXXXQ -> XXXX 0.001Q
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				if(u8Sign_bit == 1) 
					pu8OutData[1] |= 0x80;
			}
			(*pu8OutLen) = 2;
			break;	
		case EDMI_ANGLE_BLOCK_METHOD:
			for(i=0;i<6;i++)
			{
				if(MemCompare(&pu8InData[i*4], Zero_Value, 4) == 0)
				{
					MemSet(&pu8OutData[i*2],0x00,2);
				}
				else 
				{
					Float_Value = (pu8InData[i*4+0]<<24) + (pu8InData[i*4+1]<<16) + (pu8InData[i*4+2]<<8) + (pu8InData[i*4+3]);
					u8Sign_bit = IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
					if(u8Sign_bit == 0) 
						u32Output_Value[0] = 360-u32Output_Value[0];
					u32Value = u32Output_Value[0] *10;//XXXXD -> XXX.XD 0.1D
					pu8OutData[i*2+1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
					pu8OutData[i*2+0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
				}
			}
			(*pu8OutLen) = 12;
			break;	
		case EDMI_ANGLE_SINGLE_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 4) == 0)
			{
				MemSet(pu8OutData,0x00,2);
			}
			else 
			{
				Float_Value = (pu8InData[0]<<24) + (pu8InData[1]<<16) + (pu8InData[2]<<8) + (pu8InData[3]);
				u8Sign_bit = IEEE754_Float_Arithmetic((u32)Float_Value,u32Output_Value);
				if(u8Sign_bit == 0) 
						u32Output_Value[0] = 360-u32Output_Value[0];
				u32Value = u32Output_Value[0] *10;//XXXXD -> XXX.XD 0.1D
				pu8OutData[1] = QCSG_HEX2BCD((u8)(u32Value/100%100));
				pu8OutData[0] = QCSG_HEX2BCD((u8)(u32Value/1%100));	
			}
			(*pu8OutLen) = 2;
			break;
		case EDMI_TIME_METHOD:
			if(MemCompare(pu8InData, Zero_Value, 3) == 0)
			{
				MemSet(pu8OutData,0x00,3);
			}
			else
			{
				pu8OutData[2] = QCSG_HEX2BCD((u8)pu8InData[0]);//ʱ
				pu8OutData[1] = QCSG_HEX2BCD((u8)pu8InData[1]);//��
				pu8OutData[0] = QCSG_HEX2BCD((u8)pu8InData[2]);//��
			}
			(*pu8OutLen) = 3;
			break;
		case EDMI_DATA_METHOD://err20190713-2 ����֧�����ڳ��� ���ڲ�֧����FF ����YYMMDDWW ����DDMMYY
			if(MemCompare(pu8InData, Zero_Value, 3) == 0)
			{
				MemSet(pu8OutData,0x00,4);
			}
			else
			{
				pu8OutData[0] = 0xFF;							//����
				pu8OutData[1] = QCSG_HEX2BCD((u8)pu8InData[0]);//��
				pu8OutData[2] = QCSG_HEX2BCD((u8)pu8InData[1]);//��
				pu8OutData[3] = QCSG_HEX2BCD((u8)pu8InData[2]);//��
			}
			(*pu8OutLen) = 4;
			break;
		case COMMON_COV_METHOD:
		//����ֱ��ת��
			MemCopy(pu8OutData,pu8InData,u8InLen);
			(*pu8OutLen) = u8InLen;
			break;
		default:
			MemCopy(pu8OutData,pu8InData,u8InLen);
			(*pu8OutLen) = u8InLen;
			break;
	}
	
}



 /**--------------------------------------------------------------------
*��    ��:LCD����ˢ��
*�������:��
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_LcdData_Refresh(void)
{
	QCSG_LCD_RAMDATA_LIST_S* pstPn_Ram_Data = (QCSG_LCD_RAMDATA_LIST_S*)QCSG_LCD_RAM_LIST;
	QCSG_LCD_RAMDATA_S* pstRam_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	QCSG_DAY_ENERGY_LIST_S* pst_Day_Energy_List = (QCSG_DAY_ENERGY_LIST_S*)QCSG_DAY_ENERGY_LIST_LCD;
	QCSG_MONTH_ENERGY_LIST_S* pst_Month_Energy_List = (QCSG_MONTH_ENERGY_LIST_S*)QCSG_MONTH_ENERGY_LIST_LCD;
	u8 i = 0,j = 0;
	u16 u16Pn = 0,u16PnBack = 0;

	if((pstRam_Data->u16Res1 & LCD_RT_BOTTOM_ICO) != 0)
	{
		pstRam_Data->u16Res1 &= (~LCD_RT_BOTTOM_ICO);
		u16Pn = 0;
		for(i = 0;i < pstPn_Ram_Data->u8PnNum;i++)
		{
			if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
			{
				MemCopy(pstRam_Data->u8Terminal040005FF,pstPn_Ram_Data->stPnLcdRamData[i].u8Data040005FF,sizeof(pstRam_Data->u8Terminal040005FF));
				break;
			}
		}
	}
	if((pstRam_Data->u16Res1 & LCD_RT_AMR_REQ) != 0)
	{
		pstRam_Data->u16Res1 &= (~LCD_RT_AMR_REQ);
		u16PnBack = pstRam_Data->u16Pn;
		u16Pn = (u16)SwitchNumToPn(pstRam_Data->u16Pn);
		for(i = 0;i < pstPn_Ram_Data->u8PnNum;i++)
		{
			if(u16Pn == pstPn_Ram_Data->stPnLcdRamData[i].u16Pn)
			{
				MemCopy(pstRam_Data,&pstPn_Ram_Data->stPnLcdRamData[i],sizeof(QCSG_LCD_RAMDATA_S));
				pstRam_Data->u16Pn = u16PnBack;
				break;
			}
		}
		//������������
		for(j = 0;j < pst_Day_Energy_List->u8PnNum;j++)
		{
			if(u16Pn == pst_Day_Energy_List->stPn_Ram_Data[j].u16Pn)
			{
				MemCopy(&pstRam_Data->u8CurrDay0001FF00[0],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00010000,4);
				MemCopy(&pstRam_Data->u8CurrDay0001FF00[4],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00010100,4);
				MemCopy(&pstRam_Data->u8CurrDay0001FF00[8],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00010200,4);
				MemCopy(&pstRam_Data->u8CurrDay0001FF00[12],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00010300,4);
				MemCopy(&pstRam_Data->u8CurrDay0001FF00[16],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00010400,4);
				MemCopy(&pstRam_Data->u8CurrDay0002FF00[0],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00020000,4);
				MemCopy(&pstRam_Data->u8CurrDay0002FF00[4],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00020100,4);
				MemCopy(&pstRam_Data->u8CurrDay0002FF00[8],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00020200,4);
				MemCopy(&pstRam_Data->u8CurrDay0002FF00[12],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00020300,4);
				MemCopy(&pstRam_Data->u8CurrDay0002FF00[16],&pst_Day_Energy_List->stPn_Ram_Data[j].u32Energy00020400,4);
			}
		}
		for(j = 0;j < pst_Month_Energy_List->u8PnNum;j++)
		{
			if(u16Pn == pst_Month_Energy_List->stPn_Ram_Data[j].u16Pn)
			{
				MemCopy(&pstRam_Data->u8CurrMonth0001FF00[0],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00010000,4);
				MemCopy(&pstRam_Data->u8CurrMonth0001FF00[4],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00010100,4);
				MemCopy(&pstRam_Data->u8CurrMonth0001FF00[8],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00010200,4);
				MemCopy(&pstRam_Data->u8CurrMonth0001FF00[12],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00010300,4);
				MemCopy(&pstRam_Data->u8CurrMonth0001FF00[16],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00010400,4);
				MemCopy(&pstRam_Data->u8CurrMonth0002FF00[0],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00020000,4);
				MemCopy(&pstRam_Data->u8CurrMonth0002FF00[4],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00020100,4);
				MemCopy(&pstRam_Data->u8CurrMonth0002FF00[8],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00020200,4);
				MemCopy(&pstRam_Data->u8CurrMonth0002FF00[12],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00020300,4);
				MemCopy(&pstRam_Data->u8CurrMonth0002FF00[16],&pst_Month_Energy_List->stPn_Ram_Data[j].u32Energy00020400,4);
			}
		}
		//���ͳ������
		RD_0xE1000010_DataCopy(u16Pn,pstRam_Data->u8DataE1000010);
		RD_0xE1000011_DataCopy(u16Pn,pstRam_Data->u8DataE1000011);
		RD_0xE1000012_DataCopy(u16Pn,pstRam_Data->u8DataE1000012);
		RD_0xE1000013_DataCopy(u16Pn,pstRam_Data->u8DataE1000013);	
		RD_0x03100000_DataCopy(u16Pn,pstRam_Data->u8Data03100000);
		RD_0x03100100_DataCopy(u16Pn,pstRam_Data->u8Data03100100);
		RD_0x03100200_DataCopy(u16Pn,pstRam_Data->u8Data03100200);
		RD_0x03100300_DataCopy(u16Pn,pstRam_Data->u8Data03100300);
		RD_0xE1000033_DataCopy(u16Pn,pstRam_Data->u8DataE1000033);
		RD_0xE1004033_DataCopy(u16Pn,pstRam_Data->u8DataE1004033);
		//����ն�״̬��
		QCSG_ReadTerminal_State(&pstRam_Data->u8DataTerminalStateWord[0],&pstRam_Data->u8DataTerminalStateWord[1]);
	}
}
/**--------------------------------------------------------------------
*��    ��:ʵʱ��������
*�������:u8ChNo:ͨ���ţ�pstRT_RDMeter_Info:������Ϣ�ṹ�壬pstDlt645Frame_Info:DLT645֡�ṹ��
*�������:pu8Lock:��״̬
*��    ��:��
--------------------------------------------------------------------**/
void RD_Meter_RT(DLT645_FRAME_S* pstDlt645Frame_Info,u8* pu8Lock)
{
	QCSG_RTREADMETER_REQ_INFO_S* pstRt_Req_Info = (QCSG_RTREADMETER_REQ_INFO_S*)QCSG_RT_RDMETER_REQ;
	QCSG_RTREADMETER_RES_INFO_S* pstRt_Res_Info = (QCSG_RTREADMETER_RES_INFO_S*)QCSG_RT_RDMETER_RES;
	MeasurePointStruct stPnPara;
	MultiFrameSendReqStruct stSendReq_Info;
	u8 u8DLT645IdentNum = 0,u8DataLen = 0,u8FailLen = 0,u8Res = 0,u8FrameType = 0,u8DataType = 0,u8DataPos = 0;
	u8 u8CtrlCode = 0,u8IdentLen = 0,u8Ident[4],u8MeterId[6],u8FEFlag = TRUE,u8HwPort = 0;
	u8 u8ConvertMethodList[12],u8LenList[12];
	u8 u8PortBaudRate = 0,u8PortParity = 0,u8PortDataBit = 0,u8PortStopBit = 0,u8DestPort = 0,u8PortTimeout = 0;
	u16 i = 0,j = 0,k = 0,u16Pn = 0xFFFF;
	u32 u32NwIdent = 0,u32ReadLen = 0,u32DLT645Identlist[12];
	u32 u32RdStartTick = 0,u32CurrTick = 0,u32BpsCtrl = 0;
	u32* pu32Value = (u32*)(ADDR_MS_ENTRY);

	
	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RT = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RT;

	//err20190611 EDMIʹ�� ʵʱռ�õ�485�˿��Ƿ�����������
	//u8PnNum_EDMI_IsLoad=0��ʾû���(δ��½)������u8PnNum_EDMI_IsLoad��ʾ��������Ӧ�Ĳ������(�ѵ�½)
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_485 = NULL;
	MeasurePointStruct stPnPara_Temp;//����EDMI���������ͨ�Ų���
	u32 u32BpsCtrl_Temp =0 ;//EDMI���������ͨ�Ų���

	UARTCtrl_TypeDef * UARTCtrl;//err20190226 ����485-II,485-III�������ж� 
	
	if((pstRt_Req_Info->PnListNum == 0) || (pstRt_Req_Info->PnListLen == 0))
	{
		return;
	}
	if((pstRt_Req_Info->DataType != DATA_TYPE_RELAY) && (pstRt_Req_Info->DataType != DATA_TYPE_STRUCT))
	{
		if(pstRt_Req_Info->PnListNum != 0)
		{
			pstRt_Req_Info->PnListNum--;
		}
		pstRt_Req_Info->Lock = 0;		
		return;		
	}
	if(pstRt_Req_Info->DataType != DATA_TYPE_RELAY)
	{
		if(pstRt_Req_Info->PnListLen != (8 * pstRt_Req_Info->PnListNum))
		{
			pstRt_Req_Info->PnListNum--;
			pstRt_Req_Info->Lock = 0;
			return;
		}
	}
	if(pstRt_Req_Info->DataType == DATA_TYPE_RELAY)
	{
		//�м����������ʵʱ�������ݴ���
		MemSet(&stSendReq_Info,0,sizeof(MultiFrameSendReqStruct));
		stSendReq_Info.DataType = pstRt_Req_Info->DataType;
		stSendReq_Info.FrameType = pstRt_Req_Info->FrameType;
		MemCopy(&stSendReq_Info.QCSGFrameInfo,&pstRt_Req_Info->QCSGFrameInfo,sizeof(QCSGFrameInfoStruct));
		stSendReq_Info.DataBuf = pstRt_Res_Info->u8PnFnData;
		stSendReq_Info.PnListNum = 1;		
		stSendReq_Info.SrcTask = TASK_ID_RT_SEND;
		(*pu8Lock) = FUNC_LOCK;
		pstDlt645Frame_Info->u8SendLen = pstRt_Req_Info->Data[15];
		MemCopy(pstDlt645Frame_Info->u8SendBuf,&pstRt_Req_Info->Data[16],pstDlt645Frame_Info->u8SendLen);
		u8DestPort = pstRt_Req_Info->Data[9];
		u8PortBaudRate = pstRt_Req_Info->Data[10];
		u8PortParity = pstRt_Req_Info->Data[11];
		u8PortDataBit = pstRt_Req_Info->Data[12];
		u8PortStopBit = pstRt_Req_Info->Data[13];
		u8PortTimeout = pstRt_Req_Info->Data[14];
		if((u8DestPort != 0) && (u8DestPort != 1) && (u8DestPort != 2))
		{
			(*pu8Lock) = FUNC_UNLOCK;
			pstRt_Req_Info->Lock = 0;
			pstRt_Req_Info->PnListNum--;
			return;
		}
		if(QCSG_PortToHwPort(u8DestPort,&u8HwPort) != TRUE)
		{
			(*pu8Lock) = FUNC_UNLOCK;
			pstRt_Req_Info->Lock = 0;
			pstRt_Req_Info->PnListNum--;
			return;			
		}
		u32RdStartTick = HAL_GetSystemTick();
		pu32Value = (u32*)(ADDR_MS_ENTRY);
		MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
		pstDlt645Frame_Info->u8RevLen = 0;
		RD_GetPn_Bps(u8PortBaudRate,u8PortParity,u8PortDataBit,u8PortStopBit,&u32BpsCtrl);		
		UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,u8PortTimeout);
		while(1)
		{
			//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
			if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
			{			
				pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
				if(pstDlt645Frame_Info->u8RevLen > 0)
				{
					break;
				}
			}
			(*(void(*)())pu32Value[115])();
			u32CurrTick = HAL_GetSystemTick();
			if((u32CurrTick - u32RdStartTick) > 150)//err20190611 Ϊ��EDMI�� 100������150
			{
				break;
			}
		}
		if(pstDlt645Frame_Info->u8RevLen > 0)
		{
			MemCopy(&pstRt_Res_Info->u8PnFnData[0],pstRt_Req_Info->Data,8);
			pstRt_Res_Info->u8PnFnData[8] = pstRt_Req_Info->RelayType;
			pstRt_Res_Info->u8PnFnData[9] = pstDlt645Frame_Info->u8RevLen;
			MemCopy(&pstRt_Res_Info->u8PnFnData[10],pstDlt645Frame_Info->u8RevBuf,pstDlt645Frame_Info->u8RevLen);			
			pstDlt645Frame_Info->u8RevLen += 2;
			pstRt_Res_Info->u8PnFnData[2] = (u8)pstDlt645Frame_Info->u8RevLen ;
			pstRt_Res_Info->u8PnFnData[3] = (u8)(pstDlt645Frame_Info->u8RevLen >> 8);
			stSendReq_Info.DataLen = pstDlt645Frame_Info->u8RevLen + 8;
			u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
			//�ص���ѭ����������Э����Է���
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
			(*(void(*)())pu32Value[115])();	
			while(u8Res != NO_ERROR)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
				u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
				(*(void(*)())pu32Value[115])();	
				u32CurrTick = HAL_GetSystemTick();
				if((u32CurrTick - u32RdStartTick) > 300)
				{
					break;
				}				
			}			
		}
		(*pu8Lock) = FUNC_UNLOCK;
		pstRt_Req_Info->Lock = 0;
		pstRt_Req_Info->PnListNum--;
		return;
	}
	//����ΪAFN0C������ʵʱ���ݷ��ʹ���
	(*pu8Lock) = FUNC_LOCK;			
	for(i = 0;i < pstRt_Req_Info->PnListNum;i++)
	{
		MemSet(&stSendReq_Info,0,sizeof(MultiFrameSendReqStruct));
		MemSet(pstRt_Res_Info,0,sizeof(QCSG_RTREADMETER_RES_INFO_S));
		stSendReq_Info.DataType = pstRt_Req_Info->DataType;
		stSendReq_Info.FrameType = pstRt_Req_Info->FrameType;
		MemCopy(&stSendReq_Info.QCSGFrameInfo,&pstRt_Req_Info->QCSGFrameInfo,sizeof(QCSGFrameInfoStruct));
		stSendReq_Info.DataBuf = pstRt_Res_Info->u8PnFnData;
		stSendReq_Info.PnListNum = 1;
		stSendReq_Info.SrcTask = TASK_ID_RT_SEND;		
		if(memcmp(&u16Pn,&pstRt_Req_Info->Data[8 * i],2) != 0)
		{
			//���ǰ������������һ�£����ÿ������¶�ȡ��������ż�Э����Ϣ
			MemCopy(&u16Pn,&pstRt_Req_Info->Data[8 * i],2);
			
			//#if ((Project/100)==9)
			//	if(u16Pn != 0) continue;
			//#endif
			
			u32ReadLen = sizeof(MeasurePointStruct);
			if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
			{
				continue;
			}
			if(stPnPara.State == 0)
			{
				continue;
			}
			if(u16Pn != 0)
			{
				if(QCSG_PortToHwPort(stPnPara.PortID,&u8HwPort) != TRUE)
				{
					continue;
				}
				UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(u8HwPort);
				if(UARTCtrl->Function == 0)//err20190619�����ά���ھ�����
				{
					continue;
				}
				RD_GetPn_Bps(stPnPara.PortBaudRate,stPnPara.PortParity,stPnPara.PortDataBit,stPnPara.PortStopBit,&u32BpsCtrl);
			}
			else
			{
				u8HwPort = ACSAMPLEPORT;
				stPnPara.ProtocolType = METER_DLT645_07;
				u32BpsCtrl = 0;
			}
		}
		if(u16Pn == 0)
		{
			u8FEFlag = FALSE;
			//MC(0xaa,(u32)u8MeterId,6);//19-05-28
		}
		else
		{
			u8FEFlag = TRUE;
			//MemCopy(u8MeterId,stPnPara.Addr,6);		
		}
		MemCopy(u8MeterId,stPnPara.Addr,6);
		MemCopy(&u32NwIdent,&pstRt_Req_Info->Data[8 * i + 4],4);
		//ͳ�������ݴ�����ֱ���������
		if(QCSG_Check_RDIdent(u32NwIdent) == TRUE)
		{
			MemCopy(&pstRt_Res_Info->u8PnFnData[0],&u16Pn,2);
			pstRt_Res_Info->u8PnFnData[2] = u8DataLen;
			pstRt_Res_Info->u8PnFnData[3] = 0;
			MemCopy(&pstRt_Res_Info->u8PnFnData[4],&u32NwIdent,4);			
			RD_RtStatisticsData(u16Pn,u32NwIdent,pstRt_Res_Info->u8PnFnData,&pstRt_Res_Info->DataLen);
			stSendReq_Info.DataLen = 8 + pstRt_Res_Info->DataLen;
			u32RdStartTick = HAL_GetSystemTick();
			u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
			//�ص���ѭ����������Э����Է���
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			
			//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
			
			(*(void(*)())pu32Value[115])();	
			
			while(u8Res != NO_ERROR)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
				u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
				(*(void(*)())pu32Value[115])();	
				u32CurrTick = HAL_GetSystemTick();
				if((u32CurrTick - u32RdStartTick) > 300)
				{
					break;
				}				
			}
			continue;
		}
		//��ͳ�������ݴ������Ƚ�BUF���ݸ�ֵ��Ч
		if(QCSG_GetDlt645DataLen_By_Ident(0x0C,u32NwIdent,&u8FailLen) != TRUE)
		{
			continue;
		}
		MemCopy(&pstRt_Res_Info->u8PnFnData[0],&u16Pn,2);
		pstRt_Res_Info->u8PnFnData[2] = u8FailLen;
		pstRt_Res_Info->u8PnFnData[3] = 0;
		MemCopy(&pstRt_Res_Info->u8PnFnData[4],&u32NwIdent,4);		
		MemSet(&pstRt_Res_Info->u8PnFnData[8],0xFF,u8FailLen);
		stSendReq_Info.DataLen = 8 + u8FailLen;		
		//if(QCSG_GetDlt645Ident_By_AFN0C0DTable(u32NwIdent,stPnPara.ProtocolType,&u8DLT645IdentNum,u32DLT645Identlist,u8ConvertMethodList,u8LenList) != TRUE)
		if(QCSG_GetDlt645_EDMI_Ident_By_AFN0C0DTable(u32NwIdent,stPnPara.ProtocolType,&u8DLT645IdentNum,u32DLT645Identlist,u8ConvertMethodList,u8LenList) != TRUE)
		{
			//��֧�ֳ��������ݣ�ֱ�����0xFF,97Э�����ӷ����ֶε���Ҫ���⴦���¡�
			if(stPnPara.ProtocolType == METER_DLT645_97)
			{
				if((u8ConvertMethodList[0] == PRICE_COV_METHOD) || (u8ConvertMethodList[0] == DEMAND_PRICE_COV_METHOD))
				{
					RD_DLT645Data_To_NwData(u8ConvertMethodList[0],&pstRt_Res_Info->u8PnFnData[8],0,
					&pstRt_Res_Info->u8PnFnData[8],&u8DataLen);
					pstRt_Res_Info->u8PnFnData[2] = u8DataLen;
					pstRt_Res_Info->u8PnFnData[3] = 0;
					stSendReq_Info.DataLen += 1;			
				}
			}
			u32RdStartTick = HAL_GetSystemTick();	
			u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
			//�ص���ѭ����������Э����Է���
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
			(*(void(*)())pu32Value[115])();	
			while(u8Res != NO_ERROR)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
				u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
				(*(void(*)())pu32Value[115])();	
				u32CurrTick = HAL_GetSystemTick();
				if((u32CurrTick - u32RdStartTick) > 300)
				{
					break;
				}				
			}		
			continue;
		}
		//֧�ֵ����ݱ�ʶ��׼������
		if(stPnPara.ProtocolType == METER_DLT645_97)
		{
			u8IdentLen = 2;
			u8CtrlCode = 0x01;		
		}
		else if(stPnPara.ProtocolType == METER_DLT645_07)
		{
			u8IdentLen = 4;
			u8CtrlCode = 0x11;				
		}
		else if(stPnPara.ProtocolType == METER_EDMI)
		{
			u8IdentLen = 4;


			if(u8HwPort == RS485_1PORT)
			{
				pstRDMeter_Ctrl_485 = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			}
			else if(u8HwPort == RS485_2PORT)
			{
				pstRDMeter_Ctrl_485 = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			}
			else if(u8HwPort == RS485_3PORT)
			{
				pstRDMeter_Ctrl_485 = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			}
			else//һ�㲻�ᷢ�������
			{
				pstRDMeter_Ctrl_485 = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			}
			
			if(pstRDMeter_Ctrl_485->u8PnNum_EDMI_IsLoad == SwitchPnToNum(u16Pn))//��ǰ�˿�ʵʱҪ�����ı��ѵ�½����ֱ�ӳ��Ҳ���Ҫ�˳���¼��		
			{
				;//����Ҫ��½������Ҫ�˳���
			}
			else if(pstRDMeter_Ctrl_485->u8PnNum_EDMI_IsLoad == 0)//��ǰ�˿�û���ѵ�½������
			{
				//ֱ�ӵ�½���ɣ���ɺ��˳���
				//��½֡
				QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_Load_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{			
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0) break;
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200) break;
				}//��ɷ���
			}
			else //��ǰ�˿����ѵ�½�����㣬��Ҫ���˳���¼�ò����㡣�ٵ�½ʵʱ�����㡣���ʵʱ�������ָ�ԭ��״̬��
			{
				//�˳�ԭ������½ʵʱ���˳�ʵʱ����½ԭ��
				//�����������
				NAND_ReadMeasurePoint(SwitchNumToPn(pstRDMeter_Ctrl_485->u8PnNum_EDMI_IsLoad),0xE0800002,(u8*)&stPnPara_Temp,&u32ReadLen);
				//�õ�������ͨ�Ų���
				RD_GetPn_Bps(stPnPara_Temp.PortBaudRate,stPnPara_Temp.PortParity,stPnPara_Temp.PortDataBit,stPnPara_Temp.PortStopBit,&u32BpsCtrl_Temp);

				//�˳���½֡ ԭ���Ĳ�����
				QCSG_MakeEDMIFrame(stPnPara_Temp.Addr,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl_Temp,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{			
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0) break;
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200) break;
				}//��ɷ���

				//��½֡ ʵʱ������
				QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_Load_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{			
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0) break;
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200) break;
				}//��ɷ���
			}
			
		}
		else
		{
			continue;
		}	
		pstRt_Res_Info->DataLen = 8;
		for(j = 0;j < u8DLT645IdentNum;j++)
		{
			if(stPnPara.ProtocolType == METER_DLT645_97)
			{
				u8IdentLen = 2;
				u8CtrlCode = 0x01;		
			}
			if(stPnPara.ProtocolType == METER_DLT645_07)
			{
				u8IdentLen = 4;
				u8CtrlCode = 0x11;				
			}
			else if(stPnPara.ProtocolType == METER_EDMI)
			{
				u8IdentLen = 4;
				//u8CtrlCode = 0x11;				
			}
			MemCopy(u8Ident,&u32DLT645Identlist[j],u8IdentLen);
			if(stPnPara.ProtocolType == METER_EDMI)
			{
				if(QCSG_MakeEDMIFrame_RT(u8MeterId,u32NwIdent,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE);
				u8DLT645IdentNum = 1;
				//{}
				//�����¼֡�ٷ���0169֡
				//QCSG_MakeEDMIFrame_Test(1,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
				//sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf); 
			}
			else
			{
				QCSG_MakeDLT645Frame(u8FEFlag,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
            	sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
			}
			MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
			pstDlt645Frame_Info->u8RevLen = 0;	
			u32ReadLen = 0;
			UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
			u32RdStartTick = HAL_GetSystemTick();
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			while(1)
			{
			
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
				if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
				{			
					pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
					if(pstDlt645Frame_Info->u8RevLen > 0)
					{
						break;
					}
				}
				(*(void(*)())pu32Value[115])();
				u32CurrTick = HAL_GetSystemTick();
				
				if(stPnPara.ProtocolType == METER_EDMI)
				{
					if((u32CurrTick - u32RdStartTick) > 300)//EDMI���ڲ�����1200�� ��Ӧʱ����ܳ���1s
					{
						break;
					}
				}
				else 
				{
					if((u32CurrTick - u32RdStartTick) > 100)
					{
						break;
					}
				}
			}
			if(pstDlt645Frame_Info->u8RevLen == 0)	//������
			{
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{			
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0)
						{
							break;
						}
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					
					if(stPnPara.ProtocolType == METER_EDMI)
					{
						if((u32CurrTick - u32RdStartTick) > 300)//EDMI���ڲ�����1200�� ��Ӧʱ����ܳ���1s
						{
							break;
						}
					}
					else 
					{
						if((u32CurrTick - u32RdStartTick) > 100)
						{
							break;
						}
					}
				}
				if(pstDlt645Frame_Info->u8RevLen == 0)	//�ط����ǳ�����
				{
					if(stPnPara.ProtocolType == METER_EDMI)
					{
						MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[0]);
						RD_EDMIData_To_NwData(COMMON_COV_METHOD,pstDlt645Frame_Info->u8RevBuf,u8LenList[0],
						&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
						pstRt_Res_Info->DataLen += u8DataLen;	
					}
					else
					{
						MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[j]);
						RD_DLT645Data_To_NwData(u8ConvertMethodList[j],pstDlt645Frame_Info->u8RevBuf,u8LenList[j],
						&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
						pstRt_Res_Info->DataLen += u8DataLen;
					}
				}
			}
			if(pstDlt645Frame_Info->u8RevLen != 0)	//���յ�����
			{
				if(stPnPara.ProtocolType == METER_EDMI)
				{
					//ת��EDMI������֡Ϊԭʼ����֡
					//err20190709 ����ʱ�Ѿ���������
					//QCSG_GetEDMIRealFrame(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen);
					
					if(QCSG_EDMIFrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
					{//�ж��Ƿ���Ч֡ ��Ч
						MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[0]);
						RD_EDMIData_To_NwData(COMMON_COV_METHOD,pstDlt645Frame_Info->u8RevBuf,u8LenList[0],
						&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
						pstRt_Res_Info->DataLen += u8DataLen;
					}
					else 
					{	u8DataLen = 8;
						u8DataPos = 17;//����·�M<0000FFF0>�ķ�ʽ���� �����������ڵ�17�ֽ�
						RD_EDMIData_To_NwData(u8ConvertMethodList[j],&pstDlt645Frame_Info->u8RevBuf[u8DataPos],u8DataLen,
						&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
						pstRt_Res_Info->DataLen += u8DataLen;
					}
				}
				else 
				{
					if(QCSG_Dlt645FrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
					{
						MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[j]);
						RD_DLT645Data_To_NwData(u8ConvertMethodList[j],pstDlt645Frame_Info->u8RevBuf,u8LenList[j],
						&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
						pstRt_Res_Info->DataLen += u8DataLen;
					}
					else
					{
						u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
						u8DataLen = pstDlt645Frame_Info->u8RevBuf[9];
						if((u8CtrlCode == 0xC1)||(u8CtrlCode == 0xD1))
						{
							MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[j]);
							RD_DLT645Data_To_NwData(u8ConvertMethodList[j],pstDlt645Frame_Info->u8RevBuf,u8LenList[j],
							&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
							pstRt_Res_Info->DataLen += u8DataLen;
						}
						else
						{
							for(k = 0;k < u8DataLen;k++)
							{
								pstDlt645Frame_Info->u8RevBuf[10 + k] -= 0x33;			
							}				
							if(stPnPara.ProtocolType == METER_DLT645_97)
							{
								u8DataPos = 12;
								u8DataLen -= 2;
							}
							else
							{
								u8DataPos = 14;
								u8DataLen -= 4;
							}
							if(u32NwIdent == 0x0207FF00)
							{
								MemCopy(&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen + 6],&pstDlt645Frame_Info->u8RevBuf[u8DataPos],u8DataLen);
							}
							else
							{
								RD_DLT645Data_To_NwData(u8ConvertMethodList[j],&pstDlt645Frame_Info->u8RevBuf[u8DataPos],u8DataLen,
								&pstRt_Res_Info->u8PnFnData[pstRt_Res_Info->DataLen],&u8DataLen);
							}
							pstRt_Res_Info->DataLen += u8DataLen;
						}
					}
				}
			}
		}
		if((u32NwIdent == 0x0201FF00) || (u32NwIdent == 0x0202FF00) || (u32NwIdent == 0x0207FF00))
		{
			pstRt_Res_Info->u8PnFnData[2] = u8FailLen;
			pstRt_Res_Info->u8PnFnData[3] = 0;		
			stSendReq_Info.DataLen = 8 + u8FailLen;						
		}
		else
		{
			stSendReq_Info.DataLen = pstRt_Res_Info->DataLen;
			pstRt_Res_Info->DataLen -= 8;
			pstRt_Res_Info->u8PnFnData[2] = (u8)pstRt_Res_Info->DataLen;
			pstRt_Res_Info->u8PnFnData[3] = (u8)(pstRt_Res_Info->DataLen >> 8);					
		}
		u32RdStartTick = HAL_GetSystemTick();	
		u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
		//�ص���ѭ����������Э����Է���
		pu32Value = (u32*)(ADDR_MS_ENTRY);
		//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
		pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
		(*(void(*)())pu32Value[115])();	
		while(u8Res != NO_ERROR)
		{
			//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
			u8Res = QCSGSingleFrameSendReq(stSendReq_Info.QCSGFrameInfo.Port,&stSendReq_Info);
			(*(void(*)())pu32Value[115])();	
			u32CurrTick = HAL_GetSystemTick();
			if((u32CurrTick - u32RdStartTick) > 300)
			{
				break;
			}				
			
		}

		if(stPnPara.ProtocolType == METER_EDMI)
		{
			if(pstRDMeter_Ctrl_485->u8PnNum_EDMI_IsLoad == SwitchPnToNum(u16Pn))//��ǰ�˿�ʵʱҪ�����ı��ѵ�½����ֱ�ӳ��Ҳ���Ҫ�˳���¼��		
			{
				;//����Ҫ��½������Ҫ�˳���
			}
			else if(pstRDMeter_Ctrl_485->u8PnNum_EDMI_IsLoad == 0)//��ǰ�˿�û���ѵ�½������
			{
				//ֱ�ӵ�½���ɣ���ɺ��˳���
				//���˳���¼֡
				QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0) break;
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200) break;	
				}//�������
			}
			else //��ǰ�˿����ѵ�½�����㣬��Ҫ���˳���¼�ò����㡣�ٵ�½ʵʱ�����㡣���ʵʱ�������ָ�ԭ��״̬��
			{

				//�˳���½֡ ʵʱ������
				QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{			
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0) break;
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200) break;
				}//��ɷ���
				
				//��½֡ ԭ���Ĳ�����
				QCSG_MakeEDMIFrame(stPnPara_Temp.Addr,(u8*)EDMI_Load_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8HwPort,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl_Temp,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl_RT->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8HwPort,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{			
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0) break;
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200) break;
				}//��ɷ���
			}	
		}
	}
	(*pu8Lock) = FUNC_UNLOCK;	
	pstRt_Req_Info->Lock = 0;
	pstRt_Req_Info->PnListNum = 0;
}
/**--------------------------------------------------------------------
*��    ��:���᳭������
*�������:u8ChNo:ͨ���ţ�pstDlt645Frame_Info:DLT645֡�ṹ�壬pstRDMeter_Info��������Ϣ�ṹ�壬u8FrozenFlag�������־
*�������:pu8Lock:��״̬
*��    ��:��
--------------------------------------------------------------------**/
void RD_Meter_TDD(u8 u8Cycle,u8 u8ChNo,DLT645_FRAME_S* pstDlt645Frame_Info,QCSG_RDMETER_INFO_S* pstRDMeter_Info,u8 u8FrozenFlag,u8* pu8Lock)
{
	QCSG_RECORD_FROZEN_PN_LIST_S* pstRD_Meter_TD_Pn_List = (QCSG_RECORD_FROZEN_PN_LIST_S*)QCSG_RECORD_FROZEN_PN_LIST;
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = NULL;
    TIME_PARAM_S stFrozenTime;
	RealTimeYYStruct stFrozenYYTime;
	MeasurePointStruct stPnPara;
    u8 u8MeterId[6],u8Ident[4],u8FrameType = 0,u8DataType = 0,u8FEFlag = TRUE,i = 0;
    u8 u8ProtocolType = 0,u8IdentNum = 0,u8IdentLen = 0,u8CtrlCode = 0;
    u16 u16Pn = 0;
	u32 u32RdStartTick = 0,u32CurrTick = 0,u32ReadLen = 0,u32BpsCtrl = 0;
	u32* pu32Value = NULL;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl = NULL;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	u32 u32NWIdent = 0;
	//u8 pu8DataBuf[60] = {0};
	u8* pu8DataBuf = (u8*)ADDR_DATABUFF;//ybwhzָ�����ݻ�������ʱ������� ���ܳ���ռ��
	u8 u8DataLen = 0;

	if(pstRDMeter_Info->u16RDMeterIndex >= MAX_PN_NUM2)
	{
		//��ȫ�Լ��
		return;
	}
	if(pstRDMeter_Info->u16RDMeterIndex >= pstRDMeter_Info->u16PnNum)
	{
		//��ȫ�Լ��
		return;
	}
	if(u8ChNo == RS485_1PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485I_RAM_FROZEN_DATA_S;
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485II_RAM_FROZEN_DATA_S;
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485III_RAM_FROZEN_DATA_S;
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;		
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(HAL_GetSystemTime(&stFrozenTime) != TRUE)
	{
		return;
	}
	else
	{
		RealTimeToYYTime(&stFrozenTime,&stFrozenYYTime);
	}
	u16Pn = pstRDMeter_Info->u16PnList[pstRDMeter_Info->u16RDMeterIndex];
	for(i = 0;i < pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum;i++)
	{
		if(pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u16PnList[i] == u16Pn)
		{
			break;
		}
	}
	if(i < pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum)
	{
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;		
		return;		
	}
	for(i = 0;i < pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum;i++)
	{
		if(pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u16PnList[i] == u16Pn)
		{
			break;
		}
	}
	if(i >= pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum)
	{
		//����ñ������¶���û��ת�������ת���־������
		u8FrozenFlag |= TD_M_TYPE;
	}
	if((u8ChNo != ACSAMPLEPORT) && (u16Pn == 0))
	{
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;		
		return;
	}	
	if(u16Pn == 0)
	{
		u8FEFlag = FALSE;
	}
	if(pstRDMeter_Info->u16RDIdentIndex == 0)
	{
		//�������ݱ�ʶָ��Ϊ0��˵���Ǹò������״ν��룬��ȡ��������Ϣ
		u32ReadLen = sizeof(MeasurePointStruct);
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
			return;
		}
		else
		{
			MemCopy(pstRDMeter_Info->u8MeterId,stPnPara.Addr,6);
			pstRDMeter_Info->u8ProtocolType = stPnPara.ProtocolType;
			if(u16Pn == 0)
			{
				pstRDMeter_Info->u8ProtocolType = METER_DLT645_07;
				u32BpsCtrl = 0;
				pstRDMeter_Info->u32BpsCtrl = u32BpsCtrl;
			}
			else
			{
				RD_GetPn_Bps(stPnPara.PortBaudRate,stPnPara.PortParity,stPnPara.PortDataBit,stPnPara.PortStopBit,&u32BpsCtrl);
				pstRDMeter_Info->u32BpsCtrl = u32BpsCtrl;
			}
		}
		if(stPnPara.State == 0)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
			return;
		}		
	}
	u32BpsCtrl = pstRDMeter_Info->u32BpsCtrl;
	MemCopy(u8MeterId,pstRDMeter_Info->u8MeterId,6);
	u8ProtocolType = pstRDMeter_Info->u8ProtocolType;
    if(u8ProtocolType == METER_DLT645_97)
    {
        u8IdentNum = MAX_FROZEN_RD97_IDENT_NUM;
        u8IdentLen = 2;
        u8CtrlCode = 0x01;
    }
    else if(u8ProtocolType == METER_DLT645_07)
    {
        u8IdentNum = MAX_FROZEN_RD07_IDENT_NUM;
        u8IdentLen = 4;
        u8CtrlCode = 0x11;
    }
	 else if(u8ProtocolType == METER_EDMI)
    {
        u8IdentNum = MAX_FROZEN_EDMI_IDENT_NUM;
        u8IdentLen = 4;
        //u8CtrlCode = 0x11;
    }
    else
    {
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;
        return;
    }
    if(u8IdentLen == 2)
    {
        MemCopy(u8Ident,&gc_Jc_Rs485_FrozenReadCode_97[pstRDMeter_Info->u16RDIdentIndex],2);
    }
    if(u8IdentLen == 4)
    {
    	if(u8ProtocolType == METER_DLT645_07)
    	{
        	MemCopy(u8Ident,&gc_Jc_Rs485_FrozenReadCode_07[pstRDMeter_Info->u16RDIdentIndex],4);
    	}
		if(u8ProtocolType == METER_EDMI)
	    {
	        MemCopy(u8Ident,&gc_Jc_Rs485_FrozenReadCode_EDMI[pstRDMeter_Info->u16RDIdentIndex],4);
	    }
    }
    if(u8ProtocolType == METER_EDMI)
	{
	    if(QCSG_MakeEDMIFrame(u8MeterId,u8Ident,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE)
	    {
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;

			//err20190611 ������˳���¼����ղ������###########################################
			if(u8ProtocolType == METER_EDMI)
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
			
	        return;
	    }
	}	
	if((u8ProtocolType == METER_DLT645_07)||(u8ProtocolType == METER_DLT645_97))
	{
	//�����ڲ�ͨ���������ݱ�ʶ�ж����ն������ݻ�����������
	    if(QCSG_MakeDLT645Frame(u8FEFlag,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
	            sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE)
	    {
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
	        return;
	    }
	}
	(*pu8Lock) = FUNC_LOCK;
	MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
	pstDlt645Frame_Info->u8RevLen = 0;	
	UartPortTxReq(TASK_ID_RD_TDD_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
	u32RdStartTick = HAL_GetSystemTick();
	pu32Value = (u32*)(ADDR_MS_ENTRY);
	while(1)
	{
		//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
		pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
		if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
		{			
			pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
			if(pstDlt645Frame_Info->u8RevLen > 0)
			{
				break;
			}
		}
		(*(void(*)())pu32Value[115])();
		u32CurrTick = HAL_GetSystemTick();
		if(u8ProtocolType == METER_EDMI)
		{
			if((u32CurrTick - u32RdStartTick) > 300)//EDMI���ڲ�����1200�� ��Ӧʱ����ܳ���1s
			{
				break;
			}
		}
		else 
		{
			if((u32CurrTick - u32RdStartTick) > 100)
			{
				break;
			}
		}
	}
	(*pu8Lock) = FUNC_UNLOCK;

	//err20190611 �������½��Ǽǲ������##################################################################
	if((u8ProtocolType == METER_EDMI) &&(pstRDMeter_Info->u16RDIdentIndex == 0))//��һ����¼֡
		pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = SwitchPnToNum(u16Pn);//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�

	
	if(YBWHZ_TEST)
		if(u8ChNo == RS485_1PORT)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'D';
			p8[count++] = 'A';
			p8[count++] = 'Y';
			p8[count++] = ':';
			//p8[count++] = '';
			//p8[count++] = pstRDMeter_Info->u16RDIdentIndex/10%10 +'0';
			//p8[count++] = pstRDMeter_Info->u16RDIdentIndex/1%10 +'0';
			p8[count++] = '*';
			p8[count++] = (pstRDMeter_Info->u16RDIdentIndex+1)/10%10 +'0';
			p8[count++] = (pstRDMeter_Info->u16RDIdentIndex+1)/1%10 +'0';
			p8[count++] = '/';
			p8[count++] = u8IdentNum/10%10 +'0';
			p8[count++] = u8IdentNum/1%10 +'0';
			p8[count++] = '#';
			p8[count++] = '\n';
			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}

	
	if(pstDlt645Frame_Info->u8RevLen == 0)
	{
		pstRDMeter_Info->u8FailTimes++;
		if(pstRDMeter_Info->u8FailTimes >= 2)
		{
			//��ǰ��ͬһ����������ʧ��2�Σ�����
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;	
			pstRDMeter_Info->u8FailTimes = 0;

			//err20190611 ������˳���¼����ղ������###########################################
			if(u8ProtocolType == METER_EDMI)
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
			
		}
	}
	else
	{
		pstRDMeter_Info->u8FailTimes = 0;
		pstRDMeter_Info->u16RDIdentIndex++;
		if(pstRDMeter_Info->u16RDIdentIndex >= u8IdentNum)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
			pstFrozenData->u32SaveFlag = 1;		

			//err20190611 ������˳���¼����ղ������###########################################
			if(u8ProtocolType == METER_EDMI)
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
		}
	}

	if(u8ProtocolType == METER_EDMI)
	{
		if(pstDlt645Frame_Info->u8RevLen == 0)
		{
			MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));		
			
			return;
		}
		
		u32NWIdent = u8Ident[0]+u8Ident[1]*0x100+u8Ident[2]*0x10000+u8Ident[3]*0x1000000;
		
		//ת��EDMI������֡Ϊԭʼ����֡
		//err20190713-3 ����ʱ�Ѿ���������
		//QCSG_GetEDMIRealFrame(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen);
		
		if(QCSG_EDMIFrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
		{//�ж��Ƿ���Ч֡
			if(QCSG_GetDlt645DataLen_By_Ident(0x0D,u32NWIdent,&u8DataLen)== TRUE)//ȡ����
			{
				//��������ֱ��д��RAM��
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32NWIdent,4);
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = u8DataLen;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,u8DataLen);
				pstFrozenData->u32DataLen += (6 + u8DataLen);
				return;
			}
			else 
			{
				MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S)); 	
				return;
			}
		}
		else 
		{	//u8DataLen = 8;
			//u8DataPos = 17;//����·�M<0000FFF0>�ķ�ʽ���� �����������ڵ�17�ֽ�-
			//���볤��û���õ���ֱ�Ӵ�0.
			if((u32NWIdent != 0x0000fffd) && (u32NWIdent != 0x0000fffc)) 
			RD_EDMIData_To_NwData((u8)get_edmi_method(u32NWIdent),&pstDlt645Frame_Info->u8RevBuf[17],0,
			pu8DataBuf,&u8DataLen);
		}
		RD_METER_EDMIData_Process(u8ChNo,&stFrozenTime,u8FrozenFlag,u16Pn,u8MeterId,u8DataLen,pu8DataBuf,u32NWIdent);

		if(u32NWIdent == gc_Jc_Rs485_FrozenReadCode_EDMI[MAX_FROZEN_EDMI_IDENT_NUM-1].u32Fn)//���һ�����ݱ�ʶ ��ɴ洢
		{
			(*pu8Lock) = FUNC_LOCK;
			//���˳���¼֡
			QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
					
			MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
			pstDlt645Frame_Info->u8RevLen = 0;	
			u32ReadLen = 0;
			UartPortTxReq(TASK_ID_RT_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
			u32RdStartTick = HAL_GetSystemTick();
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			while(1)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
				if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
				{
					pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
					if(pstDlt645Frame_Info->u8RevLen > 0)
					{
						break;
					}
				}
				(*(void(*)())pu32Value[115])();
				u32CurrTick = HAL_GetSystemTick();
				if((u32CurrTick - u32RdStartTick) > 300)
				{	
					break;
				}
			}

			//err20190611 ������˳���¼����ղ������###########################################
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�
			
			(*pu8Lock) = FUNC_UNLOCK;
		}




	}
	else	
		RD_METER_Dlt645Data_Process(u8Cycle,u8ChNo,&stFrozenTime,u8FrozenFlag,u16Pn,u8MeterId,u8ProtocolType,pstDlt645Frame_Info);
    return;
}
/**--------------------------------------------------------------------
*��    ��:���߳�������
*�������:u8ChNo:ͨ���ţ�pstDlt645Frame_Info:DLT645֡�ṹ�壬pstRDMeter_Info��������Ϣ�ṹ�壬u8FrozenFlag�������־
*�������:pu8Lock:��״̬
*��    ��:��
--------------------------------------------------------------------**/
void RD_Meter_TDC(u8 u8Cycle,u8 u8ChNo,DLT645_FRAME_S* pstDlt645Frame_Info,QCSG_RDMETER_INFO_S* pstRDMeter_Info,u8 u8FrozenFlag,u8* pu8Lock)
{
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = NULL;
    TIME_PARAM_S stFrozenTime;
	MeasurePointStruct stPnPara;
    u8 u8MeterId[6],u8Ident[4],u8FrameType = 0,u8DataType = 0,u8FEFlag = TRUE;
    u8 u8ProtocolType = 0,u8IdentNum = 0,u8IdentLen = 0,u8CtrlCode = 0;
    u16 u16Pn = 0;
	u32 u32RdStartTick = 0,u32CurrTick = 0,u32ReadLen = 0,u32BpsCtrl = 0;
	u32* pu32Value = NULL;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl = NULL;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	u32 u32NWIdent = 0;
	u8* pu8DataBuf = (u8*)ADDR_DATABUFF;//ybwhzָ�����ݻ�������ʱ������� ���ܳ���ռ��
	//u8 pu8DataBuf[60] = {0};
	u8 u8DataLen = 0;
					

	if(pstRDMeter_Info->u16RDMeterIndex >= MAX_PN_NUM2)
	{
		//��ȫ�Լ��
		return;
	}
	if(pstRDMeter_Info->u16RDMeterIndex >= pstRDMeter_Info->u16PnNum)
	{
		//��ȫ�Լ��
		return;
	}
	if(u8ChNo == RS485_1PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485I_RAM_FROZEN_DATA_S;
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485II_RAM_FROZEN_DATA_S;
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_RS485III_RAM_FROZEN_DATA_S;
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_JC_RAM_FROZEN_DATA_S;	
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	}
	if(HAL_GetSystemTime(&stFrozenTime) != TRUE)
	{
		return;
	}
	u16Pn = pstRDMeter_Info->u16PnList[pstRDMeter_Info->u16RDMeterIndex];
	if((u8ChNo != ACSAMPLEPORT) && (u16Pn == 0))
	{
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;		
		return;
	}	
	if(u16Pn == 0)
	{
		u8FEFlag = FALSE;
	}
	if(pstRDMeter_Info->u16RDIdentIndex == 0)
	{
		//�������ݱ�ʶָ��Ϊ0��˵���Ǹò������״ν��룬��ȡ��������Ϣ
		u32ReadLen = sizeof(MeasurePointStruct);
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;			
			return;
		}
		else
		{
			MemCopy(pstRDMeter_Info->u8MeterId,stPnPara.Addr,6);
			pstRDMeter_Info->u8ProtocolType = stPnPara.ProtocolType;
			if(u16Pn == 0)
			{
			    //MC(0xaa,(u32)pstRDMeter_Info->u8MeterId,6);//19-05-28
				pstRDMeter_Info->u8ProtocolType = METER_DLT645_07;
				u32BpsCtrl = 0;
				pstRDMeter_Info->u32BpsCtrl = u32BpsCtrl;
			}
			else
			{
				RD_GetPn_Bps(stPnPara.PortBaudRate,stPnPara.PortParity,stPnPara.PortDataBit,stPnPara.PortStopBit,&u32BpsCtrl);
				pstRDMeter_Info->u32BpsCtrl = u32BpsCtrl;
			}
		}
		if(stPnPara.State == 0)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
			return;
		}
	}
	u32BpsCtrl = pstRDMeter_Info->u32BpsCtrl;
	MemCopy(u8MeterId,pstRDMeter_Info->u8MeterId,6);
	u8ProtocolType = pstRDMeter_Info->u8ProtocolType;
    if(u8ProtocolType == METER_DLT645_97)
    {
        u8IdentNum = MAX_FIXED_RD97_IDENT_NUM;
        u8IdentLen = 2;
        u8CtrlCode = 0x01;
    }
    else if(u8ProtocolType == METER_DLT645_07)
    {
        u8IdentNum = MAX_FIXED_RD07_IDENT_NUM;
        u8IdentLen = 4;
        u8CtrlCode = 0x11;
    }
    else if(u8ProtocolType == METER_EDMI){

        u8IdentNum = MAX_FIXED_EDMI_NUM;
        u8IdentLen = 2;
     }  
    else
    {
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;			
        return;
    }
    if(u8IdentLen == 2)
    {
    	  if(u8ProtocolType == METER_DLT645_97)
        	 MemCopy(u8Ident,&gc_Jc_Rs485_FixedReadCode_97[pstRDMeter_Info->u16RDIdentIndex],2);
	 	  else if(u8ProtocolType == METER_EDMI)
        	 MemCopy(u8Ident,&EDMI_cmd[pstRDMeter_Info->u16RDIdentIndex].NwCmd,4);
     }
	
    if(u8IdentLen == 4)
    {
        MemCopy(u8Ident,&gc_Jc_Rs485_FixedReadCode_07[pstRDMeter_Info->u16RDIdentIndex],4);
    }

   if(u8ProtocolType == METER_DLT645_97||u8ProtocolType == METER_DLT645_07){

	    if(QCSG_MakeDLT645Frame(u8FEFlag,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
	            sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE)
	    {
	        pstRDMeter_Info->u16RDIdentIndex++;
			if(pstRDMeter_Info->u16RDIdentIndex >= u8IdentNum)
			{
				pstRDMeter_Info->u16RDMeterIndex++;
				pstRDMeter_Info->u16RDIdentIndex = 0;
			}
			pstRDMeter_Info->u8FailTimes = 0;			
	        return;
	    }
     }
    else if(u8ProtocolType == METER_EDMI){

		if(QCSG_MakeEDMIFrame(u8MeterId,u8Ident,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE)
		{
 			pstRDMeter_Info->u16RDIdentIndex++;
			if(pstRDMeter_Info->u16RDIdentIndex >= u8IdentNum)
			{
				pstRDMeter_Info->u16RDMeterIndex++;
				pstRDMeter_Info->u16RDIdentIndex = 0;
				//err20190611 ������˳���¼����ղ������###########################################
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
			}
			pstRDMeter_Info->u8FailTimes = 0;			
	        	return;
		}

	}
   
	(*pu8Lock) = FUNC_LOCK;
	MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
	pstDlt645Frame_Info->u8RevLen = 0;	
	UartPortTxReq(TASK_ID_RD_TDC_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
	u32RdStartTick = HAL_GetSystemTick();
	pu32Value = (u32*)(ADDR_MS_ENTRY);
	while(1)
	{	
		//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
		pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
		if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
		{			
			pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
			if(pstDlt645Frame_Info->u8RevLen > 0)
			{
				break;
			}
		}
		(*(void(*)())pu32Value[115])();
		u32CurrTick = HAL_GetSystemTick();
		if(u8ProtocolType == METER_EDMI)
		{
			if((u32CurrTick - u32RdStartTick) > 300)//EDMI���ڲ�����1200�� ��Ӧʱ����ܳ���1s
			{
				break;
			}
		}
		else 
		{
			if((u32CurrTick - u32RdStartTick) > 100)
			{
				break;
			}
		}
	}
	(*pu8Lock) = FUNC_UNLOCK;

	//err20190611 �������½��Ǽǲ������##################################################################
	if((u8ProtocolType == METER_EDMI) &&(pstRDMeter_Info->u16RDIdentIndex == 0))//��һ����¼֡
		pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = SwitchPnToNum(u16Pn);//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�

	if(YBWHZ_TEST)
		if(u8ChNo == RS485_1PORT)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'C';
			p8[count++] = 'U';
			p8[count++] = 'V';
			p8[count++] = ':';
			//p8[count++] = '';
			//p8[count++] = pstRDMeter_Info->u16RDIdentIndex/10%10 +'0';
			//p8[count++] = pstRDMeter_Info->u16RDIdentIndex/1%10 +'0';
			p8[count++] = '*';
			p8[count++] = (pstRDMeter_Info->u16RDIdentIndex+1)/10%10 +'0';
			p8[count++] = (pstRDMeter_Info->u16RDIdentIndex+1)/1%10 +'0';
			p8[count++] = '/';
			p8[count++] = u8IdentNum/10%10 +'0';
			p8[count++] = u8IdentNum/1%10 +'0';
			p8[count++] = '#';
			p8[count++] = '\n';
			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}

	if(pstDlt645Frame_Info->u8RevLen == 0)
	{
		pstRDMeter_Info->u8FailTimes++;
		if(pstRDMeter_Info->u8FailTimes >= 2)
		{
			//��ǰ��ͬһ����������ʧ��2�Σ�����
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;	
			pstRDMeter_Info->u8FailTimes = 0;	
			pstFrozenData->u32SaveFlag = 1;	
			RD_Alarm_E200002F(FALSE,&stFrozenTime,u16Pn,u8MeterId);	//���ܱ�ͨѶʧ�� ARD1
			
			//err20190611 ������˳���¼����ղ������###########################################
			if(u8ProtocolType == METER_EDMI)
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
		}
	}
	else
	{
		pstRDMeter_Info->u8FailTimes = 0;
		pstRDMeter_Info->u16RDIdentIndex++;
		if(pstRDMeter_Info->u16RDIdentIndex >= u8IdentNum)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;	
			pstFrozenData->u32SaveFlag = 1;	

			//err20190611 ������˳���¼����ղ������###########################################
			if(u8ProtocolType == METER_EDMI)
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
		}
		RD_Alarm_E200002F(TRUE,&stFrozenTime,u16Pn,u8MeterId);
	}

	
	if(u8ProtocolType == METER_EDMI)
	{
		if(pstDlt645Frame_Info->u8RevLen == 0)
		{
			MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));		
			return;
		}
		
		u32NWIdent = u8Ident[0]+u8Ident[1]*0x100+u8Ident[2]*0x10000+u8Ident[3]*0x1000000;
		
		//ת��EDMI������֡Ϊԭʼ����֡
		//err20190713-3 ����ʱ�Ѿ���������
		//QCSG_GetEDMIRealFrame(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen);
		
		if(QCSG_EDMIFrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
		{//�ж��Ƿ���Ч֡
			if(QCSG_GetDlt645DataLen_By_Ident(0x0D,u32NWIdent,&u8DataLen)== TRUE)//ȡ����
			{
				//��������ֱ��д��RAM��
				MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32NWIdent,4);
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = u8DataLen;
				pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
				MemSet(&pstFrozenData->u8Data[pstFrozenData->u32DataLen + 6],0xFF,u8DataLen);
				pstFrozenData->u32DataLen += (6 + u8DataLen);
				return;
			}
			else 
			{
				MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));		
				return;
			}
		}
		else 
		{	//u8DataLen = 8;
			//u8DataPos = 17;//����·�M<0000FFF0>�ķ�ʽ���� �����������ڵ�17�ֽ�-
			//���볤��û���õ���ֱ�Ӵ�0.
			if((u32NWIdent != 0x0000fffd) && (u32NWIdent != 0x0000fffc))
				RD_EDMIData_To_NwData((u8)get_edmi_method(u32NWIdent),&pstDlt645Frame_Info->u8RevBuf[17],0,
			pu8DataBuf,&u8DataLen);
		}
		RD_METER_EDMIData_Process(u8ChNo,&stFrozenTime,u8FrozenFlag,u16Pn,u8MeterId,u8DataLen,pu8DataBuf,u32NWIdent);

		if(u32NWIdent == EDMI_cmd[MAX_FIXED_EDMI_NUM-1].NwCmd)//���һ�����ݱ�ʶ ��ɴ洢
		{
			(*pu8Lock) = FUNC_LOCK;
			//���˳���¼֡
			QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
					
			MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
			pstDlt645Frame_Info->u8RevLen = 0;	
			u32ReadLen = 0;
			UartPortTxReq(TASK_ID_RT_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
			u32RdStartTick = HAL_GetSystemTick();
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			while(1)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
				if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
				{
					pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
					if(pstDlt645Frame_Info->u8RevLen > 0)
					{
						break;
					}
				}
				(*(void(*)())pu32Value[115])();
				u32CurrTick = HAL_GetSystemTick();
				if((u32CurrTick - u32RdStartTick) > 300)
				{	
					break;
				}
			}

			//err20190611 ������˳���¼����ղ������###########################################
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�
			
			(*pu8Lock) = FUNC_UNLOCK;
		}

		
	}
	else	
		RD_METER_Dlt645Data_Process(u8Cycle,u8ChNo,&stFrozenTime,u8FrozenFlag,u16Pn,u8MeterId,u8ProtocolType,pstDlt645Frame_Info);
	
	return;
}
/**--------------------------------------------------------------------
*��    ��:�¼���������
*�������:u8ChNo:ͨ���ţ�pstDlt645Frame_Info:DLT645֡�ṹ�壬pstRDMeter_Info��������Ϣ�ṹ�壬u8FrozenFlag�������־
*�������:pu8Lock:��״̬
*��    ��:��
--------------------------------------------------------------------**/
void RD_Meter_EVENT(u8 u8Cycle,u8 u8ChNo,DLT645_FRAME_S* pstDlt645Frame_Info,QCSG_RDMETER_INFO_S* pstRDMeter_Info,u8 u8FrozenFlag,u8* pu8Lock)
{
    TIME_PARAM_S stFrozenTime;
	MeasurePointStruct stPnPara;
    u8 u8ProtocolType = 0,u8IdentLen = 0,u8CtrlCode = 0,u8EventIdentNum = 0;
    u8 u8MeterId[6],u8Ident[4],u8FrameType = 0,u8DataType = 0,u8FEFlag = TRUE;
    u8 u8IdentNull[4],u8IdentEnd[4];
    u16 u16Pn = 0;
	u32 u32RdStartTick = 0,u32CurrTick = 0,u32ReadLen = 0,u32BpsCtrl = 0;
	u32* pu32Value = NULL;
	u8 u8First_Alarm=0;
	u8 Index = 0;
	u32 u32Ident = 0;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl = NULL;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	MemSet(u8IdentNull,0,4);
    MemSet(u8IdentEnd,0xFF,4);
	if(pstRDMeter_Info->u16RDMeterIndex >= MAX_PN_NUM2)
	{
		//��ȫ�Լ��
		return;
	}
	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	if(u8ChNo == RS485_1PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;
	}
	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	
	if(HAL_GetSystemTime(&stFrozenTime) != TRUE)
	{
		return;
	}
    u8EventIdentNum = MAX_ALARM_IDENT_NUM;
    if(pstRDMeter_Info->u16RDEventIndex >= u8EventIdentNum)
    {
        pstRDMeter_Info->u16RDMeterIndex++;
        pstRDMeter_Info->u16RDEventIndex = 0;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;
		pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 ��ǰ�������ѳ�645֡��
		return;
    }
	//err20190411 �����¼���¼������������жϣ���ֹ�澯������δ�򿪾Ͳ�ȥ�����������޷����ɸ澯��
	if((gc_stQCSGAlarm_DLT645_Table[pstRDMeter_Info->u16RDEventIndex].u32AlarmIdent & 0x00010000) == 0)//�ж�Ϊ�澯���ݱ�ʶ
	{
	    if(QCSG_CheckAlarmConfig(gc_stQCSGAlarm_DLT645_Table[pstRDMeter_Info->u16RDEventIndex].u32AlarmIdent) != TRUE)
	    {
	        pstRDMeter_Info->u16RDEventIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
	        return;
	    }
	}
	else 
	{
		 if(QCSG_CheckEventMask(gc_stQCSGAlarm_DLT645_Table[pstRDMeter_Info->u16RDEventIndex].u32AlarmIdent) != TRUE)
	    {
	        pstRDMeter_Info->u16RDEventIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
	        return;
	    }
	}
	u16Pn = pstRDMeter_Info->u16PnList[pstRDMeter_Info->u16RDMeterIndex];
	if((u8ChNo != ACSAMPLEPORT) && (u16Pn == 0))
	{
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDEventIndex = 0;		
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;		
		pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 ��ǰ�������ѳ�645֡��
		return;
	}	
	if(u16Pn == 0)
	{
		u8FEFlag = FALSE;
	}

	for(u8First_Alarm=0;u8First_Alarm<MAX_ALARM_IDENT_NUM;u8First_Alarm++)//err 2018-11-1 �ҳ���һ��Ҫ���ĸ澯
	{
		if((gc_stQCSGAlarm_DLT645_Table[u8First_Alarm].u32AlarmIdent & 0x00010000) == 0)//�ж�Ϊ�澯���ݱ�ʶ
		{
			if(QCSG_CheckAlarmConfig(gc_stQCSGAlarm_DLT645_Table[u8First_Alarm].u32AlarmIdent)==TRUE)//err 2018-11-1 �ҵ���
			break;
		}
		else //�¼���¼
		{
			if(QCSG_CheckEventMask(gc_stQCSGAlarm_DLT645_Table[u8First_Alarm].u32AlarmIdent)==TRUE)//err 2018-11-1 �ҵ���
			break;
		}
		
	}
	if(u8First_Alarm>=MAX_ALARM_IDENT_NUM) return;//err 2018-11-1 û�и澯Ҫ��
	
	//if(pstRDMeter_Info->u16RDEventIndex == 0)
	//err20190510 ����u16RDIdentIndex���ж� Ϊ0 �ű�ʾ���²����㡣
	if((pstRDMeter_Info->u16RDEventIndex == u8First_Alarm) && (pstRDMeter_Info->u16RDIdentIndex == 0) )	//err 2018-11-1 
	{
		pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 �������״ν��� ���Ѷ�645���ݱ�ʶ����
		//�������ݱ�ʶָ��Ϊ0��˵���Ǹò������״ν��룬��ȡ��������Ϣ
		u32ReadLen = sizeof(MeasurePointStruct);
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
			pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 ��ǰ�������ѳ�645֡��
			return;
		}
		else
		{
			MemCopy(pstRDMeter_Info->u8MeterId,stPnPara.Addr,6);
			pstRDMeter_Info->u8ProtocolType = stPnPara.ProtocolType;			
			if(u16Pn == 0)
			{
				pstRDMeter_Info->u8ProtocolType = METER_DLT645_07;
				u32BpsCtrl = 0;
				pstRDMeter_Info->u32BpsCtrl = u32BpsCtrl;
			}
			else
			{
				RD_GetPn_Bps(stPnPara.PortBaudRate,stPnPara.PortParity,stPnPara.PortDataBit,stPnPara.PortStopBit,&u32BpsCtrl);
				pstRDMeter_Info->u32BpsCtrl = u32BpsCtrl;
			}
		}
		if(stPnPara.State == 0)
		{
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
			pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 ��ǰ�������ѳ�645֡��
			return;
		}
	}
	u32BpsCtrl = pstRDMeter_Info->u32BpsCtrl;
	MemCopy(u8MeterId,pstRDMeter_Info->u8MeterId,6);
	u8ProtocolType = pstRDMeter_Info->u8ProtocolType;
    if(u8ProtocolType == METER_DLT645_97)
    {
        u8IdentLen = 2;
        u8CtrlCode = 0x01;
    }
    else if(u8ProtocolType == METER_DLT645_07)
    {
        u8IdentLen = 4;
        u8CtrlCode = 0x11;
    }
    else
    {			
		pstRDMeter_Info->u16RDMeterIndex++;
		pstRDMeter_Info->u16RDEventIndex = 0;
		pstRDMeter_Info->u16RDIdentIndex = 0;
		pstRDMeter_Info->u8FailTimes = 0;
		pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 ��ǰ�������ѳ�645֡��
        return;
    }
    if(u8IdentLen == 2)
    {
        MemCopy(u8Ident,&gc_stQCSGAlarm_DLT645_Table[pstRDMeter_Info->u16RDEventIndex].u32Dlt64597Ident[pstRDMeter_Info->u16RDIdentIndex],2);
        if((memcmp(u8Ident,u8IdentNull,2) == 0)||(memcmp(u8Ident,u8IdentEnd,2) == 0))
        {
            pstRDMeter_Info->u16RDEventIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
            return;
        }
    }
    if(u8IdentLen == 4)
    {
        MemCopy(u8Ident,&gc_stQCSGAlarm_DLT645_Table[pstRDMeter_Info->u16RDEventIndex].u32Dlt64507Ident[pstRDMeter_Info->u16RDIdentIndex],4);	
        if((memcmp(u8Ident,u8IdentNull,4) == 0)||(memcmp(u8Ident,u8IdentEnd,4) == 0))
        {
            pstRDMeter_Info->u16RDEventIndex++;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;
            return;
        }
    }
	//�Ƚ��Ƿ���������ѳ����� ��������������
	u32Ident = gc_stQCSGAlarm_DLT645_Table[pstRDMeter_Info->u16RDEventIndex].u32Dlt64507Ident[pstRDMeter_Info->u16RDIdentIndex];
	for(Index = 0 ;Index < pstRDMeter_Info->ReadOK_DI_645_07_Num;Index++)
	{
		if(u32Ident == pstRDMeter_Info->ReadOK_DI_645_07[Index])
		{
			pstRDMeter_Info->u16RDIdentIndex++;
			pstRDMeter_Info->u8FailTimes = 0;
       		return;
		}
	}
	
    if(QCSG_MakeDLT645Frame(u8FEFlag,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
            sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE)
    {
        pstRDMeter_Info->u16RDIdentIndex++;
		pstRDMeter_Info->u8FailTimes = 0;
        return;
    }
		
	///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'A';
			p8[count++] = 'M';
			p8[count++] = 'R';
			p8[count++] = ':';
			p8[count++] = pstRDMeter_Info->u16RDMeterIndex/10%10 + '0';
			p8[count++] = pstRDMeter_Info->u16RDMeterIndex/1%10 + '0';
			p8[count++] = '|';
			p8[count++] = 'E';
			p8[count++] = 'T';
			p8[count++] = ':';
			p8[count++] = pstRDMeter_Info->u16RDEventIndex/10%10 + '0';
			p8[count++] = pstRDMeter_Info->u16RDEventIndex/1%10 + '0';
			p8[count++] = '|';
			p8[count++] = 'I';
			p8[count++] = 'D';
			p8[count++] = ':';
			p8[count++] = pstRDMeter_Info->u16RDIdentIndex/10%10 + '0';
			p8[count++] = pstRDMeter_Info->u16RDIdentIndex/1%10 + '0';
			p8[count++] = '#';
			p8[count++] = '\n';
			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}
	////////////////////////////ybwhz����/////////////////////////////



			
	(*pu8Lock) = FUNC_LOCK;
	MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
	pstDlt645Frame_Info->u8RevLen = 0;	
	UartPortTxReq(TASK_ID_RD_EVENT_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
	u32RdStartTick = HAL_GetSystemTick();
	pu32Value = (u32*)(ADDR_MS_ENTRY);
	while(1)
	{
		//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
		pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
		if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
		{			
			pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
			if(pstDlt645Frame_Info->u8RevLen > 0)
			{
				break;
			}
		}
		(*(void(*)())pu32Value[115])();
		u32CurrTick = HAL_GetSystemTick();
		if((u32CurrTick - u32RdStartTick) > 100)
		{
			break;
		}
	}
	(*pu8Lock) = FUNC_UNLOCK;
	if(pstDlt645Frame_Info->u8RevLen > 0)
	{
		pstRDMeter_Info->u8FailTimes = 0;
		RD_METER_Dlt645Data_Process(u8Cycle,u8ChNo,&stFrozenTime,u8FrozenFlag,u16Pn,u8MeterId,u8ProtocolType,pstDlt645Frame_Info);
	}
	else
	{
		pstRDMeter_Info->u8FailTimes++;
		if(pstRDMeter_Info->u8FailTimes >= 2)
		{
			//��ǰ��ͬһ����������ʧ��2�Σ�����
			pstRDMeter_Info->u16RDMeterIndex++;
			pstRDMeter_Info->u16RDEventIndex = 0;
			pstRDMeter_Info->u16RDIdentIndex = 0;
			pstRDMeter_Info->u8FailTimes = 0;		
			pstRDMeter_Info->ReadOK_DI_645_07_Num = 0;//err20190411 ��ǰ�������ѳ�645֡��
		}
		return;		
	}
    pstRDMeter_Info->u16RDIdentIndex++;
	pstRDMeter_Info->ReadOK_DI_645_07[pstRDMeter_Info->ReadOK_DI_645_07_Num] = u32Ident;
	pstRDMeter_Info->ReadOK_DI_645_07_Num++;
    return;
}
/**--------------------------------------------------------------------
*��    ��:�м����񳭱�����
*�������:u8ChNo:ͨ���ţ�pstDlt645Frame_Info:DLT645֡�ṹ�壬pstRD_Task_Info�����񳭱���Ϣ�ṹ��
*�������:pu8Lock:��״̬
*��    ��:��
--------------------------------------------------------------------**/
void RD_Relay_Task(u8 u8ChNo,DLT645_FRAME_S* pstDlt645Frame_Info,QCSG_RDRELAYTASK_INFO_S* pstRD_Task_Info,u8* pu8Lock)
{
	/*
	 * TODO:�м�����ִ�д���Ŀǰδ���д�����
	 */
	RelayTaskParaStruct* pstRelayTaskParam = NULL;
	ms_Type* pMS_Info = NULL;
    TIME_PARAM_S stLastTime;
    TIME_PARAM_S stCurrTime;
    TIME_PARAM_S stRefTime;
	TIME_PARAM_S stTempTime;
	RealTimeYYStruct stCurrYYTime;
    u8 u8PeriodValue = 0,u8PeriodUnit = 0,u8TaskIndex = 0,u8RelayType = 0,u8FrameType = 0,u8DataType = 0;
	u32 u32RdStartTick = 0,u32CurrTick = 0,u32MaxMallocSize = 0,u32ReadLen = 0,u32BpsCtrl = 0;
	u32 u32RelayTaskIdent = 0xE0000400;
	u32* pu32Value = NULL;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl = NULL;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

    pMS_Info = Get_ms();
	u32MaxMallocSize = pMS_Info->ms_malloc_sizemax();
	if(u32MaxMallocSize < sizeof(RelayTaskParaStruct))
	{
		//��̬�ڴ���䲻���ã�ֱ�ӷ��أ��˺����޷�ִ���� 
		return;
	}

	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	if(u8ChNo == RS485_1PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;
	}
	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	
	if(HAL_GetSystemTime(&stCurrTime) != TRUE)
	{
		return;
	}
	if(pstRD_Task_Info->u8RDRelayTaskIndex >= pstRD_Task_Info->u8RelayTaskNum)
    {
        return;
    }
	pstRelayTaskParam = pMS_Info->ms_malloc(sizeof(RelayTaskParaStruct));
    u8TaskIndex = pstRD_Task_Info->u8RDRelayTaskIndex;
	u32RelayTaskIdent |= pstRD_Task_Info->u8RelayTaskList[u8TaskIndex];
	u32ReadLen = sizeof(RelayTaskParaStruct);
	if(NAND_ReadPara(PARA_Task,0,u32RelayTaskIdent,(u8*)pstRelayTaskParam,&u32ReadLen)!= NO_ERROR)
	{
		pstRD_Task_Info->u8RDRelayTaskIndex++;
		pMS_Info->ms_free(pstRelayTaskParam);
		return;
	}
	if(pstRelayTaskParam->Flag != 1)
	{
		pstRD_Task_Info->u8RDRelayTaskIndex++;
		pMS_Info->ms_free(pstRelayTaskParam);
		return;		
	}
	if(TimeValidCheck(&pstRelayTaskParam->ReportTime) != NO_ERROR)
	{
		pstRD_Task_Info->u8RDRelayTaskIndex++;
		pMS_Info->ms_free(pstRelayTaskParam);
		return;		
	}	
	if(TimeValidCheck(&pstRelayTaskParam->SampleTime) != NO_ERROR)
	{
		pstRD_Task_Info->u8RDRelayTaskIndex++;
		pMS_Info->ms_free(pstRelayTaskParam);
		return;		
	}		
    u8PeriodValue = pstRelayTaskParam->SamplePeriod;
	u8PeriodUnit = pstRelayTaskParam->SamplePeriodUnit;
	u8RelayType = pstRelayTaskParam->RelayType;
	MemCopy(&stRefTime,&pstRelayTaskParam->SampleTime,sizeof(TIME_PARAM_S));
	MemCopy(pstDlt645Frame_Info->u8SendBuf,pstRelayTaskParam->Frame,pstRelayTaskParam->Length);
	pstDlt645Frame_Info->u8SendLen = pstRelayTaskParam->Length;
	pMS_Info->ms_free(pstRelayTaskParam);
	MemCopy(&stLastTime,&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],sizeof(TIME_PARAM_S));
	if(TimeValidCheck(&stLastTime) != NO_ERROR)
	{
		RD_Task_LastTimeCalc(&stLastTime,&stCurrTime,&stRefTime,u8PeriodValue,u8PeriodUnit);
		MemCopy(&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],&stLastTime,sizeof(TIME_PARAM_S));
	}
    if(QCSG_Judge_TaskTiming(&stLastTime,&stCurrTime,&stRefTime,u8PeriodValue,u8PeriodUnit) != TRUE)
    {
        pstRD_Task_Info->u8RDRelayTaskIndex++;
		return;
    }
	else
	{
		QCSG_TimerAdd(&stCurrTime,60,&stTempTime);
		RD_Task_LastTimeCalc(&stLastTime,&stTempTime,&stRefTime,u8PeriodValue,u8PeriodUnit);
		MemCopy(&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],&stLastTime,sizeof(TIME_PARAM_S));		
	}
	RD_GetPn_Bps(pstRelayTaskParam->Baud,pstRelayTaskParam->Parity,pstRelayTaskParam->DataBit,pstRelayTaskParam->StopBit,&u32BpsCtrl);
	(*pu8Lock) = FUNC_LOCK;
	/*
	 * TODO:�����������ô��룬������
	 */
	MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
	pstDlt645Frame_Info->u8RevLen = 0;
	UartPortTxReq(TASK_ID_RD_RELAY_TASK_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0501,u32BpsCtrl,1);
	u32RdStartTick = HAL_GetSystemTick();
	pu32Value = (u32*)(ADDR_MS_ENTRY);
	while(1)
	{
		//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
		pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
		if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
		{			
			pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
			if(pstDlt645Frame_Info->u8RevLen > 0)
			{
				break;
			}
		}
		(*(void(*)())pu32Value[115])();
		u32CurrTick = HAL_GetSystemTick();
		if((u32CurrTick - u32RdStartTick) > 100)
		{
			break;
		}
	}
	(*pu8Lock) = FUNC_UNLOCK;
	if(pstDlt645Frame_Info->u8RevLen > 0)
	{
		u32ReadLen = pstDlt645Frame_Info->u8RevLen;
		stCurrYYTime.YearH = 0x20;
		stCurrYYTime.YearL = stCurrTime.u8Year;
		stCurrYYTime.Month = stCurrTime.u8Month;
		stCurrYYTime.Day = stCurrTime.u8Day;
		stCurrYYTime.Hour = stCurrTime.u8Hour;
		stCurrYYTime.Minute = stCurrTime.u8Minute;
		NAND_WriteRelayTask(u8RelayType,u32RelayTaskIdent,&stCurrYYTime,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen);
	}
    MemCopy(&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],&stCurrTime,sizeof(TIME_PARAM_S));
	pstRD_Task_Info->u8RDRelayTaskIndex++;
}
/**--------------------------------------------------------------------
*��    ��:��ͨ���񳭱�����
*�������:u8ChNo:ͨ���ţ�pstDlt645Frame_Info:DLT645֡�ṹ�壬pstRD_Task_Info�����񳭱���Ϣ�ṹ��
*�������:pu8Lock:��״̬
*��    ��:��
--------------------------------------------------------------------**/
void RD_Common_Task(u8 u8ChNo,DLT645_FRAME_S* pstDlt645Frame_Info,QCSG_RDCOMMONTASK_INFO_S* pstRD_Task_Info,u8* pu8Lock)
{
	NormalTaskParaStruct* pstNormalTaskParam = NULL;
	QCSG_FROZEN_RAM_DATA_S* pstFrozenData = NULL;
	MeasurePointStruct stPnPara;
	ms_Type* pMS_Info = NULL;
    TIME_PARAM_S stLastTime;
    TIME_PARAM_S stCurrTime;
    TIME_PARAM_S stRefTime;
	TIME_PARAM_S stTempTime;
	RealTimeYYStruct stCurrYYTime;
    u8 u8PeriodValue = 0,u8PeriodUnit = 0,u8TaskIndex = 0,i = 0,j = 0,u8FEFlag = TRUE,u8DataLen = 0,u8NwLen = 0;
	u8 u8MeterId[6],u8Ident[4],u8ProtocolType = 0,u8IdentLen = 0,u8CtrlCode = 0,u8FrameType = 0,u8DataType = 0,u8DataPos = 0;
	u8 u8Dlt645IdentNum = 0,u8PnNum = 0,u8FailLen = 0;
	u8 u8ConvertMethodList[12],u8LenList[12],u8FrozenDataLenPos = 0,u8FrozenDataPos = 0;
	u16 u16Pn = 0;
	u16 u16PnList[MAX_PN_NUM2];
	u32 u32NwIdent = 0;
	u32 u32Dlt645Identlist[12];
	u32 u32RdStartTick = 0,u32CurrTick = 0,u32MaxMallocSize = 0,u32ReadLen = 0,u32BpsCtrl = 0;
	u32 u32CommonTaskIdent = 0xE0000300;
	u32* pu32Value = NULL;

	u32 Temp1;//err20190527 �������ݴ���
	
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl = NULL;//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	pMS_Info = Get_ms();
	u32MaxMallocSize = pMS_Info->ms_malloc_sizemax();
	if(u32MaxMallocSize < sizeof(NormalTaskParaStruct))
	{
		//��̬�ڴ���䲻���ã�ֱ�ӷ��أ��˺����޷�ִ���� 
		return;
	}

	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
	if(u8ChNo == RS485_1PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;
	}
	//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)

	
	if(HAL_GetSystemTime(&stCurrTime) != TRUE)
	{
		return;
	}
    if(pstRD_Task_Info->u8RDCommonTaskIndex >= pstRD_Task_Info->u8CommonTaskNum)
    {
        return;
    }
	if(u8ChNo == RS485_1PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_COMMTASK_RS485I_RAM_DATA_S;
	}
	if(u8ChNo == RS485_2PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_COMMTASK_RS485II_RAM_DATA_S;
	}
	if(u8ChNo == RS485_3PORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_COMMTASK_RS485III_RAM_DATA_S;
	}
	if(u8ChNo == ACSAMPLEPORT)
	{
		pstFrozenData = (QCSG_FROZEN_RAM_DATA_S*)QCSG_COMMTASK_JC_RAM_DATA_S;	
	}
	u8TaskIndex = pstRD_Task_Info->u8RDCommonTaskIndex;
	u32CommonTaskIdent |= pstRD_Task_Info->u8CommonTaskList[u8TaskIndex];
    if((pstRD_Task_Info->u8RDMeterIndex == 0)&&(pstRD_Task_Info->u8RDCommonTaskIdentIndex == 0))
    {
		//����ָ��ͳ������ݱ�ʶָ�붼Ϊ0,��ȡ������Ϣ
		pstNormalTaskParam = pMS_Info->ms_malloc(sizeof(NormalTaskParaStruct));
		u32ReadLen = sizeof(NormalTaskParaStruct);
		if(NAND_ReadPara(PARA_Task,0,u32CommonTaskIdent,(u8*)pstNormalTaskParam,&u32ReadLen)!= NO_ERROR)
		{
			pstRD_Task_Info->u8RDCommonTaskIndex++;
			pstRD_Task_Info->u8RDMeterIndex = 0;
			pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
			pstFrozenData->u32DataLen = 0;
			pMS_Info->ms_free(pstNormalTaskParam);
			return;
		}
		if(pstNormalTaskParam->Flag != 1)
		{
			pstRD_Task_Info->u8RDCommonTaskIndex++;
			pstRD_Task_Info->u8RDMeterIndex = 0;
			pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
			pstFrozenData->u32DataLen = 0;
			pMS_Info->ms_free(pstNormalTaskParam);
			return;			
		}
		if(TimeValidCheck(&pstNormalTaskParam->ReportTime) != NO_ERROR)
		{
			pstRD_Task_Info->u8RDCommonTaskIndex++;
			pstRD_Task_Info->u8RDMeterIndex = 0;
			pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
			pstFrozenData->u32DataLen = 0;
			pMS_Info->ms_free(pstNormalTaskParam);
			return;						
		}
		if(TimeValidCheck(&pstNormalTaskParam->SampleTime) != NO_ERROR)
		{
			pstRD_Task_Info->u8RDCommonTaskIndex++;
			pstRD_Task_Info->u8RDMeterIndex = 0;
			pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
			pstFrozenData->u32DataLen = 0;
			pMS_Info->ms_free(pstNormalTaskParam);
			return;						
		}
		u8PeriodValue = pstNormalTaskParam->SamplePeriod;
		u8PeriodUnit = pstNormalTaskParam->SamplePeriodUnit;	
		//��ȡ�����㵽�����ṹ����
		pstRD_Task_Info->u8PnNum = 0;
		for(i = 0;i < pstNormalTaskParam->DANum;i++)
		{
			u8PnNum = SplitPnToList(pstNormalTaskParam->DA[i],u16PnList,PN_CHECK_ENABLE);
			if((u8PnNum + pstRD_Task_Info->u8PnNum) >= MAX_PN_NUM2)
			{
				u8PnNum = MAX_PN_NUM2 - pstRD_Task_Info->u8PnNum;
				MemCopy((u8*)&pstRD_Task_Info->u16PnList[pstRD_Task_Info->u8PnNum],(u8*)u16PnList,u8PnNum*2);
				pstRD_Task_Info->u8PnNum = MAX_PN_NUM2;
				break;
			}
			else
			{
				MemCopy((u8*)&pstRD_Task_Info->u16PnList[pstRD_Task_Info->u8PnNum],(u8*)u16PnList,u8PnNum*2);
				pstRD_Task_Info->u8PnNum += u8PnNum;
			}
		}
		MemCopy(&stRefTime,&pstNormalTaskParam->SampleTime,sizeof(TIME_PARAM_S));
		pstRD_Task_Info->u8RDCommonTaskIdentNum = pstNormalTaskParam->DataIDNum;
		MemCopy((u8*)pstRD_Task_Info->u8RDTaskIdentList,(u8*)pstNormalTaskParam->DataID,pstNormalTaskParam->DataIDNum * 4);
		pMS_Info->ms_free(pstNormalTaskParam);
        //����ָ��ͳ������ݱ�ʶָ�붼Ϊ0��˵�����������ж�ʱ���Ƿ�
		MemCopy(&stLastTime,&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],sizeof(TIME_PARAM_S));
		if(TimeValidCheck(&stLastTime) != NO_ERROR)
		{
			RD_Task_LastTimeCalc(&stLastTime,&stCurrTime,&stRefTime,u8PeriodValue,u8PeriodUnit);
			MemCopy(&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],&stLastTime,sizeof(TIME_PARAM_S));
		}
        if(QCSG_Judge_TaskTiming(&stLastTime,&stCurrTime,&stRefTime,u8PeriodValue,u8PeriodUnit) != TRUE)
        {
            //ʱ��δ��
            pstRD_Task_Info->u8RDCommonTaskIndex++;
            pstRD_Task_Info->u8RDMeterIndex = 0;
            pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
			pstFrozenData->u32DataLen = 0;
            return;
        }
		else
		{
			//ʱ�䵽������ָ��ͳ������ݱ�ʶָ�붼Ϊ0��˵�����״ο�ʼ�����񳭶���������һ������ִ��ʱ��
			QCSG_TimerAdd(&stCurrTime,60,&stTempTime);
			RD_Task_LastTimeCalc(&stLastTime,&stTempTime,&stRefTime,u8PeriodValue,u8PeriodUnit);
			MemCopy(&pstRD_Task_Info->stLastTaskTime[u8TaskIndex],&stLastTime,sizeof(TIME_PARAM_S));			
		}
    }
    if(pstRD_Task_Info->u8RDCommonTaskIdentIndex >= pstRD_Task_Info->u8RDCommonTaskIdentNum)
    {
        //һ�����������������ʶ��������ɣ����������㳭��
        pstRD_Task_Info->u8RDMeterIndex++;
        pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
		pstFrozenData->u32DataLen = 0;
		return;
    }
	if(pstRD_Task_Info->u8RDMeterIndex >= pstRD_Task_Info->u8PnNum)
    {
        //�������в����㶼�������,�������񳭶�
        pstRD_Task_Info->u8RDCommonTaskIndex++;
        pstRD_Task_Info->u8RDMeterIndex = 0;
        pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
		pstFrozenData->u32DataLen = 0;
        return;
    }
	u16Pn = pstRD_Task_Info->u16PnList[pstRD_Task_Info->u8RDMeterIndex];
	if(u16Pn == 0)
	{
		pstRD_Task_Info->u32BpsCtrl = 0;
		u8FEFlag = FALSE;
	}
	if(pstRD_Task_Info->u8RDCommonTaskIdentIndex == 0)
	{
		//�������ݱ�ʶָ��Ϊ0��˵�����±���������ȡ��������ز���
		MemSet(pstFrozenData,0,sizeof(QCSG_FROZEN_RAM_DATA_S));
		if(u16Pn != 0)
		{
			u32ReadLen = sizeof(MeasurePointStruct);
			if(NAND_ReadMeasurePoint(u16Pn,0xE0800002,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;
			}
			else
			{
				MemCopy(pstRD_Task_Info->u8MeterId,stPnPara.Addr,6);		
				pstRD_Task_Info->u8ProtocolType = stPnPara.ProtocolType;
				pstRD_Task_Info->u8MeterPort = stPnPara.PortID;
				RD_GetPn_Bps(stPnPara.PortBaudRate,stPnPara.PortParity,stPnPara.PortDataBit,stPnPara.PortStopBit,&u32BpsCtrl);
				pstRD_Task_Info->u32BpsCtrl = u32BpsCtrl;
			}
			if(stPnPara.State == 0)
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;
			}
			//�жϲ������Ƿ��ں�������Ķ˿���
			if((u16Pn != 0)&&(u8ChNo == ACSAMPLEPORT))
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;			
			}
			if((pstRD_Task_Info->u8MeterPort == 0)&&(u8ChNo != RS485_1PORT))
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;			
			}
			if((pstRD_Task_Info->u8MeterPort == 1)&&(u8ChNo != RS485_2PORT))
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;			
			}
			if((pstRD_Task_Info->u8MeterPort == 2)&&(u8ChNo != RS485_3PORT))
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;			
			}
		}
		else
		{
			if(u8ChNo != ACSAMPLEPORT)
			{
				pstRD_Task_Info->u8RDMeterIndex++;
				pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
				return;			
			}
			GetACSampleAddr(pstRD_Task_Info->u8MeterId);
			//MC(0xaa,(u32)pstRD_Task_Info->u8MeterId,6);//19-05-28,���ڴ��ڱ�485���޸ĵ�������ĳ�ʹ�ù㲥�·� 
			pstRD_Task_Info->u8ProtocolType = METER_DLT645_07;			
		}
	}
	u32BpsCtrl = pstRD_Task_Info->u32BpsCtrl;
	MemCopy(u8MeterId,pstRD_Task_Info->u8MeterId,6);
	u8ProtocolType = pstRD_Task_Info->u8ProtocolType;	
	u32NwIdent = pstRD_Task_Info->u8RDTaskIdentList[pstRD_Task_Info->u8RDCommonTaskIdentIndex];
    if(u8ProtocolType == METER_DLT645_97)
    {
        u8IdentLen = 2;
        u8CtrlCode = 0x01;
    }
    else if(u8ProtocolType == METER_DLT645_07)
    {
        u8IdentLen = 4;
        u8CtrlCode = 0x11;
    }
	else if(u8ProtocolType == METER_EDMI)
	{
		u8IdentLen = 4;
	}
    else
    {
		pstRD_Task_Info->u8RDMeterIndex++;
		pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
		pstFrozenData->u32DataLen = 0;
        return;
    }
	if((QCSG_GetDlt645DataLen_By_Ident(0x0C,u32NwIdent,&u8FailLen) == TRUE) 
	 ||(QCSG_GetDlt645DataLen_By_Ident(0x0D,u32NwIdent,&u8FailLen) == TRUE))
	{
		//��Ҫ����������Ҫ���������ݱ�ʶ
		pstFrozenData->u8Data[pstFrozenData->u32DataLen + 4] = u8FailLen;
		pstFrozenData->u8Data[pstFrozenData->u32DataLen + 5] = 0;
	}
	else
	{
		return;
	}
	MemCopy(&pstFrozenData->u8Data[pstFrozenData->u32DataLen],&u32NwIdent,4);
	u8FrozenDataPos = pstFrozenData->u32DataLen + 6;
	u8FrozenDataLenPos = pstFrozenData->u32DataLen + 4;
	u8NwLen = 0;	
	//�������ݱ�ʶ��645���ݱ�ʶת��
	if(QCSG_GetDlt645_EDMI_Ident_By_AFN0C0DTable(u32NwIdent,u8ProtocolType,&u8Dlt645IdentNum,u32Dlt645Identlist,u8ConvertMethodList,u8LenList) != TRUE)
	{
		//�������Ҳ�����˵��Ŀǰ��֧�ֳ���
		MemSet(&pstFrozenData->u8Data[u8FrozenDataPos],0xFF,u8FailLen);
		pstFrozenData->u32DataLen = pstFrozenData->u32DataLen + 6 + u8FailLen;
		pstRD_Task_Info->u8RDCommonTaskIdentIndex++;
		if(pstRD_Task_Info->u8RDCommonTaskIdentIndex >= pstRD_Task_Info->u8RDCommonTaskIdentNum)
		{
			//��������������һ���������ݴ洢
			pstRD_Task_Info->u8RDMeterIndex++;
			pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
			//pstFrozenData->u32DataLen = 0;
			stCurrYYTime.YearH = 0x20;
			stCurrYYTime.YearL = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Year;
			stCurrYYTime.Month = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Month;
			stCurrYYTime.Day = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Day;
			stCurrYYTime.Hour = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Hour;
			stCurrYYTime.Minute = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Minute;
			NAND_WriteNormalTask(u8MeterId,0,u32CommonTaskIdent,&stCurrYYTime,pstFrozenData->u8Data,&pstFrozenData->u32DataLen);

			if(u8ProtocolType == METER_EDMI)//�������ɳ��� �˳���¼
			{
				(*pu8Lock) = FUNC_LOCK;
				//���˳���¼֡
				QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
						
				MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
				pstDlt645Frame_Info->u8RevLen = 0;	
				u32ReadLen = 0;
				UartPortTxReq(TASK_ID_RT_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
				u32RdStartTick = HAL_GetSystemTick();
				pu32Value = (u32*)(ADDR_MS_ENTRY);
				while(1)
				{
					//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
					pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
					if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
					{
						pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
						if(pstDlt645Frame_Info->u8RevLen > 0)
						{
							break;
						}
					}
					(*(void(*)())pu32Value[115])();
					u32CurrTick = HAL_GetSystemTick();
					if((u32CurrTick - u32RdStartTick) > 200)
					{	
						break;
					}
				}

				//err20190611 ������˳���¼����ղ������###########################################
				pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�
				
				(*pu8Lock) = FUNC_UNLOCK;
			}

		}
		return;
	}
//err20190527 ���Դ�����������
//	if((u8Dlt645IdentNum == 1) && (u8ConvertMethodList[0]==TAKE_PRICE_DATA_COV_METHOD))//ֻ��һ����������
//	{
//		u8ConvertMethodList[1] = (u32Dlt645Identlist[0] & 0x000000FF);//�ڼ�������
//		u8ConvertMethodList[2] = u8LenList[0];//�������ݳ���
//	}

	if(u8ProtocolType == METER_EDMI)//�ж�Ϊ�����
	{
		if(pstRD_Task_Info->u8RDCommonTaskIdentIndex == 0)//��һ�γ��ñ� ��Ҫ�ȵ�¼
		{
			(*pu8Lock) = FUNC_LOCK;
			//��½֡
			QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_Load_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);

			//QCSG_MakeEDMIFrame_Test(0,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
			//sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);	

			MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
			pstDlt645Frame_Info->u8RevLen = 0;	
			u32ReadLen = 0;
			UartPortTxReq(TASK_ID_RT_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
			u32RdStartTick = HAL_GetSystemTick();
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			while(1)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
				if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
				{			
					pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
					if(pstDlt645Frame_Info->u8RevLen > 0)
					{
						break;
					}
				}
				(*(void(*)())pu32Value[115])();
				u32CurrTick = HAL_GetSystemTick();
				
				
				if((u32CurrTick - u32RdStartTick) > 300)
				{	
					break;
				}
			}

			//err20190611 �������½��Ǽǲ������##################################################################
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = SwitchPnToNum(u16Pn);//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�
			
			(*pu8Lock) = FUNC_UNLOCK;

		}
	}

	
	for(i = 0;i< u8Dlt645IdentNum;i++)
	{
		MemCopy(u8Ident,&u32Dlt645Identlist[i],u8IdentLen);
		if(u8ProtocolType == METER_EDMI)
		{
			if(QCSG_MakeEDMIFrame_RT(u8MeterId,u32NwIdent,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE);
			u8Dlt645IdentNum = 1;
			//�����¼֡�ٷ���0169֡
			//QCSG_MakeEDMIFrame_Test(1,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
			//sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf); 
		}
		else
		{
			if(QCSG_MakeDLT645Frame(u8FEFlag,u8MeterId,u8CtrlCode,u8Ident,u8IdentLen,
	            sizeof(pstDlt645Frame_Info->u8SendBuf),&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf)!=TRUE)
			{
				continue;
			}
		}
		(*pu8Lock) = FUNC_LOCK;
		MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
		pstDlt645Frame_Info->u8RevLen = 0;
		UartPortTxReq(TASK_ID_RD_COMMON_TASK_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
		u32RdStartTick = HAL_GetSystemTick();
		pu32Value = (u32*)(ADDR_MS_ENTRY);
		while(1)
		{
			//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
			pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
			if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
			{			
				pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
				if(pstDlt645Frame_Info->u8RevLen > 0)
				{
					break;
				}
			}
			(*(void(*)())pu32Value[115])();
			u32CurrTick = HAL_GetSystemTick();
			if(u8ProtocolType == METER_EDMI)
			{
				if((u32CurrTick - u32RdStartTick) > 300)//EDMI���ڲ�����1200�� ��Ӧʱ����ܳ���1s
				{
					break;
				}
			}
			else 
			{
				if((u32CurrTick - u32RdStartTick) > 100)
				{
					break;
				}
			}
		}
		if(pstDlt645Frame_Info->u8RevLen == 0)
		{
			UartPortTxReq(TASK_ID_RD_COMMON_TASK_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
			u32RdStartTick = HAL_GetSystemTick();
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			while(1)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
				if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
				{			
					pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
					if(pstDlt645Frame_Info->u8RevLen > 0)
					{
						break;
					}
				}
				(*(void(*)())pu32Value[115])();
				u32CurrTick = HAL_GetSystemTick();
				if(u8ProtocolType == METER_EDMI)
				{
					if((u32CurrTick - u32RdStartTick) > 300)//EDMI���ڲ�����1200�� ��Ӧʱ����ܳ���1s
					{
						break;
					}
				}
				else 
				{
					if((u32CurrTick - u32RdStartTick) > 100)
					{
						break;
					}
				}
			}
			if(pstDlt645Frame_Info->u8RevLen == 0)
			{
				MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[i]);
				RD_DLT645Data_To_NwData(u8ConvertMethodList[i],pstDlt645Frame_Info->u8RevBuf,u8LenList[i],
				&pstFrozenData->u8Data[u8FrozenDataPos],&u8DataLen);
				u8FrozenDataPos += u8DataLen;
				pstFrozenData->u32DataLen += u8DataLen;	
			}						
		}
		(*pu8Lock) = FUNC_UNLOCK;
		if(pstDlt645Frame_Info->u8RevLen != 0)
		{
			if(u8ProtocolType == METER_EDMI)
			{//�����
				//ת��EDMI������֡Ϊԭʼ����֡
				//err20190713-3 ����ʱ�Ѿ���������
				//QCSG_GetEDMIRealFrame(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen);
				
				if(QCSG_EDMIFrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
				{//�ж��Ƿ���Ч֡
					MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[j]);
					RD_EDMIData_To_NwData(u8ConvertMethodList[0],pstDlt645Frame_Info->u8RevBuf,u8LenList[j],
					&pstFrozenData->u8Data[u8FrozenDataPos],&u8DataLen);
					pstFrozenData->u32DataLen += u8DataLen;
				}
				else 
				{	u8DataLen = 8;
					u8DataPos = 17;//����·�M<0000FFF0>�ķ�ʽ���� �����������ڵ�17�ֽ�
					RD_EDMIData_To_NwData(u8ConvertMethodList[j],&pstDlt645Frame_Info->u8RevBuf[u8DataPos],u8DataLen,
					&pstFrozenData->u8Data[u8FrozenDataPos],&u8DataLen);
					pstFrozenData->u32DataLen += u8DataLen;
				}
			}
			else 
			{//645��
				if(QCSG_Dlt645FrameJudge(pstDlt645Frame_Info->u8RevBuf,&pstDlt645Frame_Info->u8RevLen) != TRUE)
				{
					MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[i]);
					RD_DLT645Data_To_NwData(u8ConvertMethodList[i],pstDlt645Frame_Info->u8RevBuf,u8LenList[i],
					&pstFrozenData->u8Data[u8FrozenDataPos],&u8DataLen);
					u8FrozenDataPos += u8DataLen;
					pstFrozenData->u32DataLen += u8DataLen;		
				}
				else
				{
					u8CtrlCode = pstDlt645Frame_Info->u8RevBuf[8];
					u8DataLen = pstDlt645Frame_Info->u8RevBuf[9];
					if((u8CtrlCode == 0xC1)||(u8CtrlCode == 0xD1))
					{
						MemSet(pstDlt645Frame_Info->u8RevBuf,0xFF,u8LenList[i]);
						RD_DLT645Data_To_NwData(u8ConvertMethodList[i],pstDlt645Frame_Info->u8RevBuf,u8LenList[i],
						&pstFrozenData->u8Data[u8FrozenDataPos],&u8DataLen);
						u8FrozenDataPos += u8DataLen;
						pstFrozenData->u32DataLen += u8DataLen;	
					}
					else
					{
						for(j = 0;j < u8DataLen;j++)
						{
							pstDlt645Frame_Info->u8RevBuf[10 + j] -= 0x33;			
						}		
						if(pstRD_Task_Info->u8ProtocolType == METER_DLT645_97)
						{
							u8DataPos = 12;
							u8DataLen -= 2;
						}
						else
						{
							u8DataPos = 14;
							u8DataLen -= 4;
						}
						if(u8ConvertMethodList[i] == TAKE_PRICE_DATA_COV_METHOD)//�����������⴦��
						{
							Temp1 = (u32NwIdent & 0x000000FF);
							Temp1 *= u8LenList[i]/5;
							MR((u32)&pstFrozenData->u8Data[u8FrozenDataPos], (u32)&pstDlt645Frame_Info->u8RevBuf[u8DataPos + Temp1], u8LenList[i]/5);
							u8DataLen = u8LenList[i]/5;
						}
						else 
						{
							RD_DLT645Data_To_NwData(u8ConvertMethodList[i],&pstDlt645Frame_Info->u8RevBuf[u8DataPos],u8DataLen,
							&pstFrozenData->u8Data[u8FrozenDataPos],&u8DataLen);		
						}
						u8FrozenDataPos += u8DataLen;
						pstFrozenData->u32DataLen += u8DataLen;	
					}
				}
			}
		}
		u8NwLen += u8DataLen;
	}
	if((u32NwIdent == 0x0201FF00) || (u32NwIdent == 0x0202FF00) || (u32NwIdent == 0x0207FF00))
	{
		//���⴦��
		pstFrozenData->u8Data[u8FrozenDataLenPos] = u8FailLen;
		pstFrozenData->u8Data[u8FrozenDataLenPos + 1] = 0;	
	}
	else
	{
		pstFrozenData->u8Data[u8FrozenDataLenPos] = u8NwLen;
		pstFrozenData->u8Data[u8FrozenDataLenPos + 1] = 0;
	}
	pstFrozenData->u32DataLen += 6;
	pstRD_Task_Info->u8RDCommonTaskIdentIndex++;
	if(pstRD_Task_Info->u8RDCommonTaskIdentIndex >= pstRD_Task_Info->u8RDCommonTaskIdentNum)
	{
		//��������������һ���������ݴ洢
		pstRD_Task_Info->u8RDMeterIndex++;
		pstRD_Task_Info->u8RDCommonTaskIdentIndex = 0;
		stCurrYYTime.YearH = 0x20;
		stCurrYYTime.YearL = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Year;
		stCurrYYTime.Month = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Month;
		stCurrYYTime.Day = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Day;
		stCurrYYTime.Hour = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Hour;
		stCurrYYTime.Minute = pstRD_Task_Info->stLastTaskTime[u8TaskIndex].u8Minute;
		NAND_WriteNormalTask(u8MeterId,0,u32CommonTaskIdent,&stCurrYYTime,pstFrozenData->u8Data,&pstFrozenData->u32DataLen);	
		pstFrozenData->u32DataLen = 0;


		if(u8ProtocolType == METER_EDMI)//�������ɳ��� �˳���¼
		{
			(*pu8Lock) = FUNC_LOCK;
			//���˳���¼֡
			QCSG_MakeEDMIFrame(u8MeterId,(u8*)EDMI_UnLoad_Fram,&pstDlt645Frame_Info->u8SendLen,pstDlt645Frame_Info->u8SendBuf);
					
			MemSet(pstDlt645Frame_Info->u8RevBuf,0,sizeof(pstDlt645Frame_Info->u8RevBuf));
			pstDlt645Frame_Info->u8RevLen = 0;	
			u32ReadLen = 0;
			UartPortTxReq(TASK_ID_RT_SEND,u8ChNo,pstDlt645Frame_Info->u8SendBuf,pstDlt645Frame_Info->u8SendLen,0x0301,u32BpsCtrl,1);
			u32RdStartTick = HAL_GetSystemTick();
			pu32Value = (u32*)(ADDR_MS_ENTRY);
			while(1)
			{
				//err20190227 ���Ӽ�����ʱʱ�䣬��ֹ����̫��������OS�������µ�����״̬(��ջ����պ��޷����������)
				pstRDMeter_Ctrl->u32LockTimeOutTick = HAL_GetSystemTick();
				if(UartPortRxReq(u8ChNo,pstDlt645Frame_Info->u8RevBuf,&u32ReadLen,&u8FrameType,&u8DataType) == NO_ERROR)
				{
					pstDlt645Frame_Info->u8RevLen = (u8)u32ReadLen;
					if(pstDlt645Frame_Info->u8RevLen > 0)
					{
						break;
					}
				}
				(*(void(*)())pu32Value[115])();
				u32CurrTick = HAL_GetSystemTick();
				if((u32CurrTick - u32RdStartTick) > 200)
				{	
					break;
				}
			}

			//err20190611 ������˳���¼����ղ������###########################################
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�
			
			(*pu8Lock) = FUNC_UNLOCK;
		}
		
	}
}
/**--------------------------------------------------------------------
*��    ��:��������
*�������:u8ChNo:����ͨ��,pu8Lock:����pstRD_Meter_Info�������ṹ��
*�������:pstRD_Meter_Info�������ṹ��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Meter_Process(u8 u8Cycle,u8 u8ChNo,QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl,DLT645_FRAME_S* pstDlt645Frame_Info,QCSG_RDMETER_INFO_S* pstRD_Meter_Info,QCSG_RDRELAYTASK_INFO_S* pstRD_RelayTask_Info,QCSG_RDCOMMONTASK_INFO_S* pstRD_CommonTask_Info)
{
    u8 u8FrozenFlag = 0;                           
	u32 u32Tick = 0;
	
	UartRxChCtrlStruct* UartRxChCtrl;//err20190402 ������̫��������ʱ
	UartTaskCtrlStruct* pUTask;
			
	if((pstRDMeter_Ctrl->u8Lock != FUNC_LOCK)&&(pstRDMeter_Ctrl->u8Lock != FUNC_UNLOCK))
	{
		//��ֻ��������״̬�������ֵ�����״̬��˵�����RAM���ִ��󣬴�ʱ��ʼ����������ȫ�ֱ���
		MemSet(pstRDMeter_Ctrl,0,sizeof(QCSG_RDMETER_CTRL_S));
		MemSet(pstDlt645Frame_Info,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRD_Meter_Info,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRD_RelayTask_Info,0,sizeof(QCSG_RDRELAYTASK_INFO_S));
		MemSet(pstRD_CommonTask_Info,0,sizeof(QCSG_RDCOMMONTASK_INFO_S));
		return;
	}
	if((pstRDMeter_Ctrl->u8Rdm_Sem & 0xC0) != 0)
	{
		//����״̬��2λ���������壬�����֣�˵�����RAM���ִ��󣬴�ʱ��ʼ����������ȫ�ֱ���
		MemSet(pstRDMeter_Ctrl,0,sizeof(QCSG_RDMETER_CTRL_S));
		MemSet(pstDlt645Frame_Info,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRD_Meter_Info,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRD_RelayTask_Info,0,sizeof(QCSG_RDRELAYTASK_INFO_S));
		MemSet(pstRD_CommonTask_Info,0,sizeof(QCSG_RDCOMMONTASK_INFO_S));
		return;
	}
	if(pstRD_Meter_Info->u16PnNum > MAX_PN_NUM2)
	{
		//������������Ϊ8��������8��˵�����RAM���ִ��󣬴�ʱ��ʼ����������ȫ�ֱ���
		MemSet(pstRDMeter_Ctrl,0,sizeof(QCSG_RDMETER_CTRL_S));
		MemSet(pstDlt645Frame_Info,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRD_Meter_Info,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRD_RelayTask_Info,0,sizeof(QCSG_RDRELAYTASK_INFO_S));
		MemSet(pstRD_CommonTask_Info,0,sizeof(QCSG_RDCOMMONTASK_INFO_S));
		return;
	}
	if(pstRD_Meter_Info->u16RDMeterIndex > pstRD_Meter_Info->u16PnNum)
	{
		//����ָ�룬���ֵΪpstRD_Meter_Info->u16PnNum�������ڣ�˵�����RAM���ִ��󣬴�ʱ��ʼ����������ȫ�ֱ���
		MemSet(pstRDMeter_Ctrl,0,sizeof(QCSG_RDMETER_CTRL_S));
		MemSet(pstDlt645Frame_Info,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRD_Meter_Info,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRD_RelayTask_Info,0,sizeof(QCSG_RDRELAYTASK_INFO_S));
		MemSet(pstRD_CommonTask_Info,0,sizeof(QCSG_RDCOMMONTASK_INFO_S));
		return;		
	}
	if(pstRD_RelayTask_Info->u8RDRelayTaskIndex > pstRD_RelayTask_Info->u8RelayTaskNum)
	{
		//�м̳���ָ�룬���ֵΪpstRD_RelayTask_Info->u8RelayTaskNum�������ڣ�˵�����RAM���ִ��󣬴�ʱ��ʼ����������ȫ�ֱ���
		MemSet(pstRDMeter_Ctrl,0,sizeof(QCSG_RDMETER_CTRL_S));
		MemSet(pstDlt645Frame_Info,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRD_Meter_Info,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRD_RelayTask_Info,0,sizeof(QCSG_RDRELAYTASK_INFO_S));
		MemSet(pstRD_CommonTask_Info,0,sizeof(QCSG_RDCOMMONTASK_INFO_S));
		return;
	}
	if(pstRD_CommonTask_Info->u8RDCommonTaskIndex > pstRD_CommonTask_Info->u8CommonTaskNum)
	{
		//���񳭶�ָ�룬���ֵΪpstRD_CommonTask_Info->u8CommonTaskNum�������ڣ�˵�����RAM���ִ��󣬴�ʱ��ʼ����������ȫ�ֱ���
		MemSet(pstRDMeter_Ctrl,0,sizeof(QCSG_RDMETER_CTRL_S));
		MemSet(pstDlt645Frame_Info,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRD_Meter_Info,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRD_RelayTask_Info,0,sizeof(QCSG_RDRELAYTASK_INFO_S));
		MemSet(pstRD_CommonTask_Info,0,sizeof(QCSG_RDCOMMONTASK_INFO_S));
		return;	
	}			
	u32Tick = HAL_GetSystemTick();
    if(pstRDMeter_Ctrl->u8Lock == FUNC_LOCK)
    {
    	if(u32Tick - pstRDMeter_Ctrl->u32LockTimeOutTick >=350)//err20190226 ������ʱʱ��3.5s
		{
			pstRDMeter_Ctrl->u8Lock = FUNC_UNLOCK;//��ʱ����

			//err20190402 ������ʱ��������ڽ��ջ��������ݣ���ֹ��һ�ν����ȡ����������ݡ�
			//�����������������Ϊ������ǰ��ʱ֡��������һ�����ݱ�ʶֱ�ӳ����¸����ݱ�ʶ��
			UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(u8ChNo);//err20190402
			if(UartRxChCtrl->Empty != BUFFER_EMPTY)
			{
				UartRxChCtrl->Empty = BUFFER_EMPTY;
				UartRxChCtrl->FrameLen = 0;
			
				pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(u8ChNo);
				if(pUTask != NULL)
				{
					pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
				}
			}
			///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = u8ChNo/10%10 + '0';
			p8[count++] = u8ChNo/1%10 + '0';
			p8[count++] = ':';
			p8[count++] = 'U';
			p8[count++] = 'N';
			p8[count++] = 'L';
			p8[count++] = 'O';
			p8[count++] = 'C';
			p8[count++] = 'K';
			p8[count++] = pstRD_Meter_Info->u16RDMeterIndex/10%10 + '0';
			p8[count++] = pstRD_Meter_Info->u16RDMeterIndex/1%10 + '0';
			p8[count++] = '|';
			p8[count++] = 'E';
			p8[count++] = 'T';
			p8[count++] = ':';
			p8[count++] = pstRD_Meter_Info->u16RDEventIndex/10%10 + '0';
			p8[count++] = pstRD_Meter_Info->u16RDEventIndex/1%10 + '0';
			p8[count++] = '|';
			p8[count++] = 'I';
			p8[count++] = 'D';
			p8[count++] = ':';
			p8[count++] = pstRD_Meter_Info->u16RDIdentIndex/10%10 + '0';
			p8[count++] = pstRD_Meter_Info->u16RDIdentIndex/1%10 + '0';

			p8[count++] = '#';
			p8[count++] = '\n';
			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}
	////////////////////////////ybwhz����/////////////////////////////
		}
		else 
        	return;
    }
	if(pstRD_Meter_Info->u16PnNum == 0)
	{
		pstRDMeter_Ctrl->u8Rdm_Sem = 0;
		return;
	}

#if ((Project/100)==9)
	if(pstRDMeter_Ctrl->u8PB_Task_Lock == TASK_JZQ_LOCK)//��� ���輯������������ִ�У���ȴ�����������ִ����ɡ�
	{
		return;
	}
#endif
	/*
     * TODO:���߳�������
     */
    if((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_TDC_SEM) != 0)
    {
        //���߳����������������ڵ�����
		u8FrozenFlag = TD_C_TYPE;
		RD_Meter_TDC(u8Cycle,u8ChNo,pstDlt645Frame_Info,pstRD_Meter_Info,u8FrozenFlag,&pstRDMeter_Ctrl->u8Lock);
		if(pstRD_Meter_Info->u16RDMeterIndex >= pstRD_Meter_Info->u16PnNum)
		{
			//������ɣ�����ر�ʶ������
			pstRDMeter_Ctrl->u8Rdm_Sem &= (~RD_METER_TDC_SEM);
			pstRD_Meter_Info->u16RDMeterIndex = 0;
			pstRD_Meter_Info->u16RDIdentIndex = 0;

			//err20190611 ������˳���¼����ղ������###########################################
			//if(u8ProtocolType == METER_EDMI)
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
		}
		return;
    }
	/*
     * TODO:�¼����������������߳���һ�𴥷�
     */
    if((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_EVENT_SEM) != 0)
    {
		//���߳�����ɺ��ٽ����¼������������¼��������ҹ��õ�pstRD_Meter_Infoȫ�ֱ���
		u8FrozenFlag = 0;
		RD_Meter_EVENT(u8Cycle,u8ChNo,pstDlt645Frame_Info,pstRD_Meter_Info,u8FrozenFlag,&pstRDMeter_Ctrl->u8Lock);
		if(pstRD_Meter_Info->u16RDMeterIndex >= pstRD_Meter_Info->u16PnNum)
		{
			//������ɣ�����ر�ʶ������
			pstRDMeter_Ctrl->u8Rdm_Sem &= (~RD_METER_EVENT_SEM);
			pstRD_Meter_Info->u16RDMeterIndex = 0;
			pstRD_Meter_Info->u16RDIdentIndex = 0;			
			pstRD_Meter_Info->u16RDEventIndex = 0;
		}
        return;
    }
    if((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_TDD_SEM) != 0)
    {
		//���߳����Լ��¼�������ɺ��ٽ����ն��᳭���������ն��᳭�����ҹ��õ�pstRD_Meter_Infoȫ�ֱ���
		if((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_TDM_SEM) != 0)
		{
			u8FrozenFlag = TD_D_TYPE | TD_M_TYPE;
		}
		else
		{
			u8FrozenFlag = TD_D_TYPE;
		}
		RD_Meter_TDD(u8Cycle,u8ChNo,pstDlt645Frame_Info,pstRD_Meter_Info,u8FrozenFlag,&pstRDMeter_Ctrl->u8Lock);
		if(pstRD_Meter_Info->u16RDMeterIndex >= pstRD_Meter_Info->u16PnNum)
		{
			//������ɣ�����ر�ʶ������
			pstRDMeter_Ctrl->u8Rdm_Sem &= (~RD_METER_TDD_SEM);
			pstRDMeter_Ctrl->u8Rdm_Sem &= (~RD_METER_TDM_SEM);
			pstRD_Meter_Info->u16RDMeterIndex = 0;
			pstRD_Meter_Info->u16RDIdentIndex = 0;
			
			//err20190611 ������˳���¼����ղ������###########################################
			//if(u8ProtocolType == METER_EDMI)
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ��������գ�����ʵʱ�����ж�
		}
		return;
    }
    /*
     * TODO:�м����񳭱�����,1min����
     */
    if((pstRDMeter_Ctrl->u8Rdm_Sem & RD_RELAY_TASK_SEM) != 0)
    {
        RD_Relay_Task(u8ChNo,pstDlt645Frame_Info,pstRD_RelayTask_Info,&pstRDMeter_Ctrl->u8Lock);
        if(pstRD_RelayTask_Info->u8RDRelayTaskIndex >= pstRD_RelayTask_Info->u8RelayTaskNum)
        {
            //������ɣ�����ر�ʶ������
            pstRDMeter_Ctrl->u8Rdm_Sem &= (~RD_RELAY_TASK_SEM);
            pstRD_RelayTask_Info->u8RDRelayTaskIndex = 0;
        }
        return;
    }
    /*
     * TODO:��ͨ���񳭱�������1min����
     */
    if((pstRDMeter_Ctrl->u8Rdm_Sem & RD_COMMON_TASK_SEM) != 0)
    {
        RD_Common_Task(u8ChNo,pstDlt645Frame_Info,pstRD_CommonTask_Info,&pstRDMeter_Ctrl->u8Lock);
        if(pstRD_CommonTask_Info->u8RDCommonTaskIndex >= pstRD_CommonTask_Info->u8CommonTaskNum)
        {
            //������ɣ�����ر�ʶ������
            pstRDMeter_Ctrl->u8Rdm_Sem &= (~RD_COMMON_TASK_SEM);
            pstRD_CommonTask_Info->u8RDCommonTaskIndex = 0;
			pstRD_CommonTask_Info->u8RDMeterIndex = 0;
			pstRD_CommonTask_Info->u8RDCommonTaskIdentIndex = 0;

			//err20190611 ������˳���¼����ղ������###########################################
			pstRDMeter_Ctrl->u8PnNum_EDMI_IsLoad = 0;//����ǰ��¼�Ĳ�����Ŵ洢������ʵʱ�����ж�
        }
        return;
    }
	
#if ((Project/100)==9)
	if(((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_TDC_SEM) == 0)
		 &&((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_TDD_SEM) == 0)
		 &&((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_TDM_SEM) == 0)
		 &&((pstRDMeter_Ctrl->u8Rdm_Sem & RD_METER_EVENT_SEM) == 0)
		 &&((pstRDMeter_Ctrl->u8Rdm_Sem & RD_RELAY_TASK_SEM) == 0)
		 &&((pstRDMeter_Ctrl->u8Rdm_Sem & RD_COMMON_TASK_SEM) == 0))
		{
			if(pstRDMeter_Ctrl->u8PB_Task_Lock == TASK_FK_LOCK)//������������ס�����ж�����ִ����ɺ����
			pstRDMeter_Ctrl->u8PB_Task_Lock = TASK_PB_UNLOCK;
		}
#endif
}
/**--------------------------------------------------------------------
*��    ��:����ͨ����������
*�������:u8ChNo
*�������:pu8Period
*��    ��:��
--------------------------------------------------------------------**/
void RD_Update_Period(u8 u8ChNo,u8* pu8Period)
{
	AMRChTimeStruct stRDChParam;
	ResultEnum Res = NO_ERROR;
	u32 u32ReadLen = 0;
	switch(u8ChNo)
	{
		case RS485_1PORT:
			u32ReadLen = sizeof(AMRChTimeStruct);
			Res = NAND_ReadPara(PARA_AMR,0,0xE0000200,(u8*)&stRDChParam,&u32ReadLen);
			break;
		case RS485_2PORT:
			u32ReadLen = sizeof(AMRChTimeStruct);
			Res = NAND_ReadPara(PARA_AMR,0,0xE0000201,(u8*)&stRDChParam,&u32ReadLen);
			break;
		case RS485_3PORT:
			u32ReadLen = sizeof(AMRChTimeStruct);
			Res = NAND_ReadPara(PARA_AMR,0,0xE0000202,(u8*)&stRDChParam,&u32ReadLen);
			break;
		default:
			Res = ERROR_InvalidData;
			break;
	}
	if(Res == NO_ERROR)
	{
		if(stRDChParam.PeriodUnit >= 1)
		{
			//�������ڴ���1Сʱ�����ڸ�����˵��������Ĭ��15min
			(*pu8Period) = 15;
		}
		else
		{
			if(stRDChParam.Period >= 60)
			{
				//�������ڴ���1Сʱ�����ڸ�����˵��������Ĭ��15min
				(*pu8Period) = 15;
			}
			else
			{
				(*pu8Period) = stRDChParam.Period;
			}				
		}
	}
	else
	{
		//ȡ����������Ϣ����ʧ�ܣ�Ĭ��15min
		(*pu8Period) = 15;
	}	
	return;
}
/**--------------------------------------------------------------------
*��    ��:����ͨ������������Ϣ
*�������:u8Period
*�������:pstRdMeter_Ctrl
*��    ��:��
--------------------------------------------------------------------**/
void RD_Update_RDSem(u8 u8Period,QCSG_RDMETER_CTRL_S* pstRdMeter_Ctrl,QCSG_RDMETER_INFO_S* pstRdMeter_Info)
{
	TIME_PARAM_S stSysCurrTime;
	u8 u8Minute = 0;
	#if(TEST_ON == 1)//����ˢ��ʱ���޸�Ϊ30sһ��
		u8 u8Second = 0;
	#endif
	
	HAL_GetSystemTime(&stSysCurrTime);
	u8Minute = QCSG_BCD2HEX(stSysCurrTime.u8Minute);

	if(stSysCurrTime.u8Minute != pstRdMeter_Ctrl->stSysTime_Back.u8Minute)
	{
		//���񳭶�����
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_RELAY_TASK_SEM;
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_COMMON_TASK_SEM;
	}

//err20180801 ̨�� ����ʱ���޸�Ϊ30sһ�� ��ֹʧѹ����
#if(TEST_ON == 1)//����ˢ��ʱ���޸�Ϊ30sһ��
	u8Second = QCSG_BCD2HEX(stSysCurrTime.u8Second);
	if(u8Period == 1)//˵��Ϊ����
	{
		if((u8Second == 0 || u8Second == 30 ) && (stSysCurrTime.u8Second != pstRdMeter_Ctrl->stSysTime_Back.u8Second))
		{
			//���߼��¼���������
			pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_TDC_SEM;
			pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_EVENT_SEM;
		}
	}
	else if(((u8Period != 0) && (u8Minute % u8Period) == 0) && (stSysCurrTime.u8Minute != pstRdMeter_Ctrl->stSysTime_Back.u8Minute))
	{
		//���߼��¼���������
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_TDC_SEM;
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_EVENT_SEM;
	}

#else
	if(((u8Period != 0) && (u8Minute % u8Period) == 0) && (stSysCurrTime.u8Minute != pstRdMeter_Ctrl->stSysTime_Back.u8Minute))
	{
		//���߼��¼���������
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_TDC_SEM;
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_EVENT_SEM;
	}
#endif
#if defined(FKGA43_CL3200_TEST)
	if((stSysCurrTime.u8Hour == 0) && (stSysCurrTime.u8Minute == 0x01) && (stSysCurrTime.u8Minute != pstRdMeter_Ctrl->stSysTime_Back.u8Minute))
#else
	if((stSysCurrTime.u8Hour == 0) && (stSysCurrTime.u8Minute == 0x10) && (stSysCurrTime.u8Minute != pstRdMeter_Ctrl->stSysTime_Back.u8Minute))
#endif	
	{
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_TDD_SEM;
	}
	if((stSysCurrTime.u8Hour != 0) && (stSysCurrTime.u8Hour != pstRdMeter_Ctrl->stSysTime_Back.u8Hour))
	{
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_TDD_SEM;
	}
	if(stSysCurrTime.u8Month != pstRdMeter_Ctrl->stSysTime_Back.u8Month)
	{
		pstRdMeter_Ctrl->u8Rdm_Sem |= RD_METER_TDM_SEM;
	}
	MemCopy(&pstRdMeter_Ctrl->stSysTime_Back,&stSysCurrTime,sizeof(TIME_PARAM_S));

#if ((Project/100)==9)
	if(((pstRdMeter_Ctrl->u8Rdm_Sem & RD_METER_TDC_SEM) != 0)
	 ||((pstRdMeter_Ctrl->u8Rdm_Sem & RD_METER_TDD_SEM) != 0)
	 ||((pstRdMeter_Ctrl->u8Rdm_Sem & RD_METER_TDM_SEM) != 0)
	 ||((pstRdMeter_Ctrl->u8Rdm_Sem & RD_METER_EVENT_SEM) != 0)
	 ||((pstRdMeter_Ctrl->u8Rdm_Sem & RD_RELAY_TASK_SEM) != 0)
	 ||((pstRdMeter_Ctrl->u8Rdm_Sem & RD_COMMON_TASK_SEM) != 0))
	{
		if(pstRdMeter_Ctrl->u8PB_Task_Lock == TASK_PB_UNLOCK)
		pstRdMeter_Ctrl->u8PB_Task_Lock = TASK_FK_LOCK;
	}
#endif
		
}
/**--------------------------------------------------------------------
*��    ��:������������ͨ����������ͳһ�����ڴ˺�����
*�������:��
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void RD_Meter_Task(void)
{
#if ((Project/100)==3) || ((Project/100)==9)	
	ResultEnum Res = NO_ERROR;
	u8 u8Period = 0,i = 0;
	u16 u16PnNum = 0;
	u32 u32Tick = 0,u32ReadLen = 0;

	//UartRxChCtrlStruct* UartRxChCtrl;//err20190402 ������̫��������ʱ
	//UartTaskCtrlStruct* pUTask;
#endif

	UARTCtrl_TypeDef * UARTCtrl;//err20190226 ����485-II,485-III�������ж�

#if !defined(FKGA43_CL3200_TEST)
	u32* pu32PowerUpFlag = (u32*)QCSG_RDMETER_POWER_UP_FLAG;
	u32* pu32Flag_Day = (u32*)QCSG_POWER_DOWN_DAY_FLAG;
	u32* pu32Flag_Month = (u32*)QCSG_POWER_DOWN_MONTH_FLAG;
#endif
	TIME_PARAM_S* pstSysBackTime = (TIME_PARAM_S*)QCSG_GLOBAL_TIME;
	TIME_PARAM_S stSysCurrTime,stTempTime;
	
	RS485PortParaStruct stRs485PortParam;
	//���������õ���������Ϣ����Ϣ�Ŀ��ƽṹ��
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485I = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485II = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485III = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_JC = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RT = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RT;
	//DLT645��֡�ṹ��
	DLT645_FRAME_S* pstDlt645_Frame_RS485I = (DLT645_FRAME_S*)QCSG_DLT645_FRAME_RS485_I;
	DLT645_FRAME_S* pstDlt645_Frame_RS485II = (DLT645_FRAME_S*)QCSG_DLT645_FRAME_RS485_II;
	DLT645_FRAME_S* pstDlt645_Frame_RS485III = (DLT645_FRAME_S*)QCSG_DLT645_FRAME_RS485_III;
	DLT645_FRAME_S* pstDlt645_Frame_JC = (DLT645_FRAME_S*)QCSG_DLT645_FRAME_JC;
	DLT645_FRAME_S* pstDlt645_Frame_RT = (DLT645_FRAME_S*)QCSG_DLT645_FRAME_RT;
	//������Ϣ�ṹ��
	QCSG_RDMETER_INFO_S* pstRDMeter_Info_RS485I = (QCSG_RDMETER_INFO_S*)QCSG_RDMETER_INFO_RS485_I;
	QCSG_RDMETER_INFO_S* pstRDMeter_Info_RS485II = (QCSG_RDMETER_INFO_S*)QCSG_RDMETER_INFO_RS485_II;
	QCSG_RDMETER_INFO_S* pstRDMeter_Info_RS485III = (QCSG_RDMETER_INFO_S*)QCSG_RDMETER_INFO_RS485_III;
	QCSG_RDMETER_INFO_S* pstRDMeter_Info_JC = (QCSG_RDMETER_INFO_S*)QCSG_RDMETER_INFO_JC;
	//�м�����ṹ��
	QCSG_RDRELAYTASK_INFO_S* pstRDRelayTask_Info_RS485I = (QCSG_RDRELAYTASK_INFO_S*)QCSG_RDRELAYTASK_INFO_RS485_I;
	QCSG_RDRELAYTASK_INFO_S* pstRDRelayTask_Info_RS485II = (QCSG_RDRELAYTASK_INFO_S*)QCSG_RDRELAYTASK_INFO_RS485_II;
	QCSG_RDRELAYTASK_INFO_S* pstRDRelayTask_Info_RS485III = (QCSG_RDRELAYTASK_INFO_S*)QCSG_RDRELAYTASK_INFO_RS485_III;
	QCSG_RDRELAYTASK_INFO_S* pstRDRelayTask_Info_JC = (QCSG_RDRELAYTASK_INFO_S*)QCSG_RDRELAYTASK_INFO_JC;
	//��ͨ����ṹ��
	QCSG_RDCOMMONTASK_INFO_S* pstRDCommonTask_Info_RS485I = (QCSG_RDCOMMONTASK_INFO_S*)QCSG_RDCOMMONTASK_INFO_RS485_I;
	QCSG_RDCOMMONTASK_INFO_S* pstRDCommonTask_Info_RS485II = (QCSG_RDCOMMONTASK_INFO_S*)QCSG_RDCOMMONTASK_INFO_RS485_II;
	QCSG_RDCOMMONTASK_INFO_S* pstRDCommonTask_Info_RS485III = (QCSG_RDCOMMONTASK_INFO_S*)QCSG_RDCOMMONTASK_INFO_RS485_III;
	QCSG_RDCOMMONTASK_INFO_S* pstRDCommonTask_Info_JC = (QCSG_RDCOMMONTASK_INFO_S*)QCSG_RDCOMMONTASK_INFO_JC;
	QCSG_RDCOMMONTASK_INFO_S* pstRDCommonTask_Info_Public = (QCSG_RDCOMMONTASK_INFO_S*)QCSG_RDCOMMONTASK_INFO_PUBLIC;

	//�������ݲ������¼�ṹ��
	QCSG_RECORD_FROZEN_PN_LIST_S* pstRD_Meter_TD_Pn_List = (QCSG_RECORD_FROZEN_PN_LIST_S*)QCSG_RECORD_FROZEN_PN_LIST;
	//������������ṹ��
	QCSG_MONTH_ENERGY_LIST_S* pst_Month_Energy_List = (QCSG_MONTH_ENERGY_LIST_S*)QCSG_MONTH_ENERGY_LIST_CTRL;
	//LCD�����ṹ��
	QCSG_DAY_ENERGY_LIST_S* pst_Day_Energy_List_Lcd = (QCSG_DAY_ENERGY_LIST_S*)QCSG_DAY_ENERGY_LIST_LCD;
	QCSG_MONTH_ENERGY_LIST_S* pst_Month_Energy_List_Lcd = (QCSG_MONTH_ENERGY_LIST_S*)QCSG_MONTH_ENERGY_LIST_LCD;
	
	if((pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u8PnNum > MAX_PN_NUM2) || (pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u8PnNum > MAX_PN_NUM2))
	{
		MemSet(pstRD_Meter_TD_Pn_List,0,sizeof(QCSG_RECORD_FROZEN_PN_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM2;i++)
	{
		if(QCSG_BCDCheck((u8*)&pstRD_Meter_TD_Pn_List->st_TD_D_PnList.u16PnList[i],1) != TRUE)
		{
			MemSet(pstRD_Meter_TD_Pn_List,0,sizeof(QCSG_RECORD_FROZEN_PN_LIST_S));
			break;
		}
		if(QCSG_BCDCheck((u8*)&pstRD_Meter_TD_Pn_List->st_TD_M_PnList.u16PnList[i],1) != TRUE)
		{
			MemSet(pstRD_Meter_TD_Pn_List,0,sizeof(QCSG_RECORD_FROZEN_PN_LIST_S));
			break;
		}
	}
	RD_LcdData_Refresh();	
	u32Tick = HAL_GetSystemTick();
	HAL_GetSystemTime(&stSysCurrTime);
	if(stSysCurrTime.u8Day != pstSysBackTime->u8Day)
	{
		//����
		MemSet(&pstRD_Meter_TD_Pn_List->st_TD_D_PnList,0,sizeof(QCSG_FROZEN_PN_LIST_S));
		MemCopy(&stTempTime,&stSysCurrTime,sizeof(TIME_PARAM_S));
		QCSG_DecOneDay(&stTempTime);
#if !defined(FKGA43_CL3200_TEST)
		if((stTempTime.u8Day == pstSysBackTime->u8Day) && (stSysCurrTime.u8Minute == 0))
		{
			//�������գ�ת��ͳ�����ݼ�����ڴ��е�ͳ������
			RD_TDStatisticsDataSave(&stSysCurrTime);
			RD_ClearTDStatisticsData(1);	
		}	
		else
		{
			if((*pu32PowerUpFlag) == 0)
			{
				RD_ClearTDStatisticsData(0);
			}
			else
			{
				RD_ClearTDStatisticsData(1);
			}
		}
		RD_ClearAlarmData();
		//���ȫ�ֱ���
		(*pu32Flag_Day) = 0;
#endif
#if defined(FKGA43_CL3200_TEST)
		if(stTempTime.u8Day == pstSysBackTime->u8Day)
		{
			RD_TDStatisticsDataSave(&stSysCurrTime);
		}			
		RD_ClearTDStatisticsData(1);	
		RD_ClearAlarmData();		
#endif
		MemSet(pstRDMeter_Ctrl_RS485I,0,sizeof(QCSG_RDMETER_CTRL_S) - 8);
		MemSet(pstRDMeter_Ctrl_RS485II,0,sizeof(QCSG_RDMETER_CTRL_S) - 8);
		MemSet(pstRDMeter_Ctrl_RS485III,0,sizeof(QCSG_RDMETER_CTRL_S) - 8);
		MemSet(pstDlt645_Frame_RS485I,0,sizeof(DLT645_FRAME_S));
		MemSet(pstDlt645_Frame_RS485II,0,sizeof(DLT645_FRAME_S));
		MemSet(pstDlt645_Frame_RS485III,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRDMeter_Info_RS485I,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRDMeter_Info_RS485II,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRDMeter_Info_RS485III,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRDRelayTask_Info_RS485I,0,sizeof(QCSG_RDRELAYTASK_INFO_S)-2048);//err20190528 ����������������
		MemSet(pstRDRelayTask_Info_RS485II,0,sizeof(QCSG_RDRELAYTASK_INFO_S)-2048);//err20190528 ����������������
		MemSet(pstRDRelayTask_Info_RS485III,0,sizeof(QCSG_RDRELAYTASK_INFO_S)-2048);//err20190528 ����������������
		MemSet(pstRDCommonTask_Info_RS485I,0,sizeof(QCSG_RDCOMMONTASK_INFO_S) -256);//err20190528 ����������������
		MemSet(pstRDCommonTask_Info_RS485II,0,sizeof(QCSG_RDCOMMONTASK_INFO_S)-256);//err20190528 ����������������
		MemSet(pstRDCommonTask_Info_RS485III,0,sizeof(QCSG_RDCOMMONTASK_INFO_S)-256);//err20190528 ����������������
		QCSG_UpdatePnList(0x00,MAX_PN_NUM2,pstRDMeter_Info_RS485I->u16PnList,&pstRDMeter_Info_RS485I->u16PnNum);
		QCSG_UpdatePnList(0x01,MAX_PN_NUM2,pstRDMeter_Info_RS485II->u16PnList,&pstRDMeter_Info_RS485II->u16PnNum);		
		QCSG_UpdatePnList(0x02,MAX_PN_NUM2,pstRDMeter_Info_RS485III->u16PnList,&pstRDMeter_Info_RS485III->u16PnNum);
		MemSet(pstRDMeter_Ctrl_JC,0,sizeof(QCSG_RDMETER_CTRL_S) - 8);
		MemSet(pstRDMeter_Ctrl_RT,0,sizeof(QCSG_RDMETER_CTRL_S) - 8);
		MemSet(pstDlt645_Frame_JC,0,sizeof(DLT645_FRAME_S));
		MemSet(pstRDMeter_Info_JC,0,sizeof(QCSG_RDMETER_INFO_S));
		MemSet(pstRDRelayTask_Info_JC,0,sizeof(QCSG_RDRELAYTASK_INFO_S)-2048);//err20190528 ����������������
		MemSet(pstRDCommonTask_Info_JC,0,sizeof(QCSG_RDCOMMONTASK_INFO_S)-256);//err20190528 ����������������
		MemSet(pst_Day_Energy_List_Lcd,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));
		
	}
	if(stSysCurrTime.u8Month != pstSysBackTime->u8Month)
	{
		//����
		MemSet(&pstRD_Meter_TD_Pn_List->st_TD_M_PnList,0,sizeof(QCSG_FROZEN_PN_LIST_S));
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));
		MemSet(pst_Month_Energy_List_Lcd,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));
		MemCopy(&stTempTime,&stSysCurrTime,sizeof(TIME_PARAM_S));
		QCSG_DecOneMonth(&stTempTime);
#if !defined(FKGA43_CL3200_TEST)
		if((stTempTime.u8Month == pstSysBackTime->u8Month) && (stSysCurrTime.u8Minute == 0))
		{
			RD_TMStatisticsDataSave(&stSysCurrTime);
			RD_ClearTMStatisticsData(1);	
		}		
		else
		{
			if((*pu32PowerUpFlag) == 0)
			{
				RD_ClearTMStatisticsData(0);
			}
			else
			{
				RD_ClearTMStatisticsData(1);
			}
		}
		(*pu32Flag_Month) = 0;
#endif
#if defined(FKGA43_CL3200_TEST)
		if(stTempTime.u8Month == pstSysBackTime->u8Month)
		{
			RD_TMStatisticsDataSave(&stSysCurrTime);
		}			
		RD_ClearTMStatisticsData(1);			
#endif
	}
#if !defined(FKGA43_CL3200_TEST)
	(*pu32PowerUpFlag) = 1;
#endif
	MemCopy(pstSysBackTime,&stSysCurrTime,sizeof(TIME_PARAM_S));
	if(pstRDMeter_Ctrl_RT->u8Lock == FUNC_LOCK)
	{//ʵʱ���񵽣���ͨ���ĳ���������ͣ���ȴ�ʵʱ�������
		if(u32Tick - pstRDMeter_Ctrl_RT->u32LockTimeOutTick >=350)//err20190226 ������ʱʱ��3.5s
		{
			pstRDMeter_Ctrl_RT->u8Lock = FUNC_UNLOCK;//��ʱ����

			//err20190402 ������ʱ��������ڽ��ջ��������ݣ���ֹ��һ�ν����ȡ����������ݡ�
			//�����������������Ϊ������ǰ��ʱ֡��������һ�����ݱ�ʶֱ�ӳ����¸����ݱ�ʶ��
			//ʵʱ������ʱ����������ȷ���˿ں�
			/*UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(u8ChNo);//err20190402
			if(UartRxChCtrl->Empty != BUFFER_EMPTY)
			{
				UartRxChCtrl->Empty = BUFFER_EMPTY;
				UartRxChCtrl->FrameLen = 0;
			
				pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(u8ChNo);
				if(pUTask != NULL)
				{
					pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
				}
			}*/
			///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'R';
			p8[count++] = 'T';
			p8[count++] = ':';
			p8[count++] = 'U';
			p8[count++] = 'N';
			p8[count++] = 'L';
			p8[count++] = 'O';
			p8[count++] = 'C';
			p8[count++] = 'K';
			p8[count++] = '#';
			p8[count++] = '\n';
			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}
	////////////////////////////ybwhz����/////////////////////////////
		}
		else 
		{
			return;
		}
	}
	if((pstRDMeter_Ctrl_JC->u8Lock == FUNC_UNLOCK)&&(pstRDMeter_Ctrl_RS485I->u8Lock == FUNC_UNLOCK)
		&&(pstRDMeter_Ctrl_RS485II->u8Lock == FUNC_UNLOCK)&&(pstRDMeter_Ctrl_RS485III->u8Lock == FUNC_UNLOCK))
	{
		//������ʱ����������ɺ��ٽ���ʵʱ���񳭶�
		RD_Meter_RT(pstDlt645_Frame_RT,&pstRDMeter_Ctrl_RT->u8Lock);
	}
	MemSet(&stRs485PortParam,0xFF,sizeof(RS485PortParaStruct));

	//JC��������
	if(u32Tick - pstRDMeter_Ctrl_JC->u32ReadPortParamTick >= 6000)		//60s
	{
		// 1min����һ��JC�˿ڵ��м������б���ȫ���˿ڵ���ͨ�����б�
		QCSG_UpdateRelayTaskList(ACSAMPLEPORT,pstRDRelayTask_Info_JC);
		QCSG_UpdateCommonTaskList(pstRDCommonTask_Info_Public);
		pstRDCommonTask_Info_RS485I->u8CommonTaskNum = pstRDCommonTask_Info_Public->u8CommonTaskNum;
		MemCopy(pstRDCommonTask_Info_RS485I->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskNum);
		pstRDCommonTask_Info_RS485II->u8CommonTaskNum = pstRDCommonTask_Info_Public->u8CommonTaskNum;
		MemCopy(pstRDCommonTask_Info_RS485II->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskNum);
		pstRDCommonTask_Info_RS485III->u8CommonTaskNum = pstRDCommonTask_Info_Public->u8CommonTaskNum;
		MemCopy(pstRDCommonTask_Info_RS485III->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskNum);		

		RD_Alarm_E2000038_PN();		
		pstRDCommonTask_Info_JC->u8CommonTaskNum = pstRDCommonTask_Info_Public->u8CommonTaskNum;
		MemCopy(pstRDCommonTask_Info_JC->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskList,pstRDCommonTask_Info_Public->u8CommonTaskNum);
		
		
		pstRDMeter_Ctrl_JC->u32ReadPortParamTick = u32Tick;
	}
	u8Period = 1;
	//��ȡ������Ϣ
	RD_Update_RDSem(u8Period,pstRDMeter_Ctrl_JC,pstRDMeter_Info_JC);
	//��������		
	pstRDMeter_Info_JC->u16PnList[0] = 0;
	pstRDMeter_Info_JC->u16PnNum = 1;
	if(u32Tick >= 3000)
	{
		//JC �ϵ�30s���ٽ��г��������⽻���������û��ˢ�µ���ͳ�Ƴ��������
		RD_Meter_Process(u8Period,ACSAMPLEPORT,pstRDMeter_Ctrl_JC,pstDlt645_Frame_JC,pstRDMeter_Info_JC,pstRDRelayTask_Info_JC,pstRDCommonTask_Info_JC);		
	}


	//��һ·RS485��������
	if(u32Tick - pstRDMeter_Ctrl_RS485I->u32ReadPortParamTick >= 6000)
	{
		//���µ�һ·RS485�������ڣ�Ŀǰ����10msTick,Ϊ1min
		RD_Update_Period(RS485_1PORT,&pstRDMeter_Ctrl_RS485I->u8Rd_Period);
		//���²������б�
		u16PnNum = pstRDMeter_Info_RS485I->u16PnNum;
		if(u16PnNum != 0)
		{
			if(((pstRDMeter_Ctrl_RS485I->u8Rdm_Sem & RD_METER_TDC_SEM) == 0)
			 &&((pstRDMeter_Ctrl_RS485I->u8Rdm_Sem & RD_METER_TDD_SEM) == 0)
			 &&((pstRDMeter_Ctrl_RS485I->u8Rdm_Sem & RD_METER_EVENT_SEM) == 0))
			{
				
				//��ʱ�����ڳ������񣬸��²������б�
				pstRDMeter_Info_RS485I->u16PnNum = 0;
				MemSet(pstRDMeter_Info_RS485I->u16PnList,0,sizeof(pstRDMeter_Info_RS485I->u16PnList));
				QCSG_UpdatePnList(0x00,MAX_PN_NUM2,pstRDMeter_Info_RS485I->u16PnList,&pstRDMeter_Info_RS485I->u16PnNum);	
			}	
		}
		else
		{
			//û�м�¼�Ĳ�������Ϣ
			pstRDMeter_Info_RS485I->u16PnNum = 0;
			MemSet(pstRDMeter_Info_RS485I->u16PnList,0,sizeof(pstRDMeter_Info_RS485I->u16PnList));
			QCSG_UpdatePnList(0x00,MAX_PN_NUM2,pstRDMeter_Info_RS485I->u16PnList,&pstRDMeter_Info_RS485I->u16PnNum);				
		}	
		if(u16PnNum != pstRDMeter_Info_RS485I->u16PnNum)
		{
			//������仯�����������ն������ݲ�����֮ǰ�������������ʱֻ�����²�ѯһ��
			pstRDMeter_Ctrl_RS485I->u8Rdm_Sem |= RD_METER_TDD_SEM;
			if(stSysCurrTime.u8Day == 0x01)
			{
				pstRDMeter_Ctrl_RS485I->u8Rdm_Sem |= RD_METER_TDM_SEM;				
			}			
		}
		//�����м������б�
		QCSG_UpdateRelayTaskList(0,pstRDRelayTask_Info_RS485I);
		pstRDMeter_Ctrl_RS485I->u32ReadPortParamTick = u32Tick;
	}
	u8Period = pstRDMeter_Ctrl_RS485I->u8Rd_Period;
#if defined(FKGA43_CL3200_TEST)
	//��CL3200����̨�壬�������ڹ̶�Ϊ2��Ϊ�˲����澯��
	if(u8Period < 5)
	u8Period = 2;									
#endif
	//��ȡ������Ϣ
	RD_Update_RDSem(u8Period,pstRDMeter_Ctrl_RS485I,pstRDMeter_Info_RS485I);
	//��������
	RD_Meter_Process(u8Period,RS485_1PORT,pstRDMeter_Ctrl_RS485I,pstDlt645_Frame_RS485I,pstRDMeter_Info_RS485I,pstRDRelayTask_Info_RS485I,pstRDCommonTask_Info_RS485I);
	
	//�ڶ�·RS485��������
	if(u32Tick - pstRDMeter_Ctrl_RS485II->u32ReadPortParamTick >= 6000)
	{
		//��ȡ�ڶ�·RS485���ܣ����ǳ���RS485����ó�������,Ŀǰ����10msTick,Ϊ1min
		u32ReadLen = sizeof(RS485PortParaStruct);
		Res = NAND_ReadPara(PARA_RS485Port,0,0xE0000A01,(u8*)&stRs485PortParam,&u32ReadLen);
		Refresh485PortFunction(RS485_2PORT, &stRs485PortParam);//ˢ��485�˿ڲ��� 1min
		if((Res == NO_ERROR)&&(stRs485PortParam.Function == 00))
		{	
			//���³�������			
			RD_Update_Period(RS485_2PORT,&pstRDMeter_Ctrl_RS485II->u8Rd_Period);
			//���²������б�
			u16PnNum = pstRDMeter_Info_RS485II->u16PnNum;
			if(u16PnNum != 0)
			{
				if(((pstRDMeter_Ctrl_RS485II->u8Rdm_Sem & RD_METER_TDC_SEM) == 0)
				 &&((pstRDMeter_Ctrl_RS485II->u8Rdm_Sem & RD_METER_TDD_SEM) == 0)
				 &&((pstRDMeter_Ctrl_RS485II->u8Rdm_Sem & RD_METER_EVENT_SEM) == 0))
				{
					//��ʱ�����ڳ������񣬸��²������б�					
					pstRDMeter_Info_RS485II->u16PnNum = 0;
					MemSet(pstRDMeter_Info_RS485II->u16PnList,0,sizeof(pstRDMeter_Info_RS485II->u16PnList));
					QCSG_UpdatePnList(0x01,MAX_PN_NUM2,pstRDMeter_Info_RS485II->u16PnList,&pstRDMeter_Info_RS485II->u16PnNum);		
				}
			}
			else
			{
				//û�м�¼�Ĳ�������Ϣ
				pstRDMeter_Info_RS485II->u16PnNum = 0;
				MemSet(pstRDMeter_Info_RS485II->u16PnList,0,sizeof(pstRDMeter_Info_RS485II->u16PnList));
				QCSG_UpdatePnList(0x01,MAX_PN_NUM2,pstRDMeter_Info_RS485II->u16PnList,&pstRDMeter_Info_RS485II->u16PnNum);				
			}
			if(u16PnNum != pstRDMeter_Info_RS485II->u16PnNum)
			{
				//������仯�����������ն������ݲ�����֮ǰ�������������ʱֻ�����²�ѯһ��
				pstRDMeter_Ctrl_RS485II->u8Rdm_Sem |= RD_METER_TDD_SEM;
				if(stSysCurrTime.u8Day == 0x01)
				{
					pstRDMeter_Ctrl_RS485II->u8Rdm_Sem |= RD_METER_TDM_SEM;				
				}
			}
			//�����м������б�
			QCSG_UpdateRelayTaskList(1,pstRDRelayTask_Info_RS485II);
		}
		pstRDMeter_Ctrl_RS485II->u32ReadPortParamTick = u32Tick;
	}
	
	
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	if(UARTCtrl->Function == 1)
	{
		u8Period = pstRDMeter_Ctrl_RS485II->u8Rd_Period;
		//��ȡ������Ϣ
		RD_Update_RDSem(u8Period,pstRDMeter_Ctrl_RS485II,pstRDMeter_Info_RS485II);
		//��������		
		RD_Meter_Process(u8Period,RS485_2PORT,pstRDMeter_Ctrl_RS485II,pstDlt645_Frame_RS485II,pstRDMeter_Info_RS485II,pstRDRelayTask_Info_RS485II,pstRDCommonTask_Info_RS485II);
	}



	
	//����·RS485��������
	MemSet(&stRs485PortParam,0xFF,sizeof(RS485PortParaStruct));
	if(u32Tick - pstRDMeter_Ctrl_RS485III->u32ReadPortParamTick >= 6000)
	{
		//��ȡ����·RS485���ܣ����ǳ���RS485����ó�������,Ŀǰ����10msTick,Ϊ1min
		u32ReadLen = sizeof(RS485PortParaStruct);
		Res = NAND_ReadPara(PARA_RS485Port,0,0xE0000A02,(u8*)&stRs485PortParam,&u32ReadLen);
		Refresh485PortFunction(RS485_3PORT, &stRs485PortParam);				//ˢ��485�˿ڲ��� 1min
		if((Res == NO_ERROR)&&(stRs485PortParam.Function == 00))
		{
			//���³�������
			RD_Update_Period(RS485_3PORT,&pstRDMeter_Ctrl_RS485III->u8Rd_Period);
			//���²������б�
			u16PnNum = pstRDMeter_Info_RS485III->u16PnNum;	
			if(u16PnNum != 0)
			{
				if(((pstRDMeter_Ctrl_RS485III->u8Rdm_Sem & RD_METER_TDC_SEM) == 0)
				 &&((pstRDMeter_Ctrl_RS485III->u8Rdm_Sem & RD_METER_TDD_SEM) == 0)
				 &&((pstRDMeter_Ctrl_RS485III->u8Rdm_Sem & RD_METER_EVENT_SEM) == 0))
				{
					//��ʱ�����ڳ������񣬸��²������б�					
					pstRDMeter_Info_RS485III->u16PnNum = 0;
					MemSet(pstRDMeter_Info_RS485III->u16PnList,0,sizeof(pstRDMeter_Info_RS485III->u16PnList));
					QCSG_UpdatePnList(0x02,MAX_PN_NUM2,pstRDMeter_Info_RS485III->u16PnList,&pstRDMeter_Info_RS485III->u16PnNum);		
				}	
			}
			else
			{
				//û�м�¼�Ĳ�������Ϣ
				pstRDMeter_Info_RS485III->u16PnNum = 0;
				MemSet(pstRDMeter_Info_RS485III->u16PnList,0,sizeof(pstRDMeter_Info_RS485III->u16PnList));
				QCSG_UpdatePnList(0x02,MAX_PN_NUM2,pstRDMeter_Info_RS485III->u16PnList,&pstRDMeter_Info_RS485III->u16PnNum);					
			}	
			if(u16PnNum != pstRDMeter_Info_RS485III->u16PnNum)
			{
				//������仯�����������ն������ݲ�����֮ǰ�������������ʱֻ�����²�ѯһ��
				pstRDMeter_Ctrl_RS485III->u8Rdm_Sem |= RD_METER_TDD_SEM;
				if(stSysCurrTime.u8Day == 0x01)
				{
					pstRDMeter_Ctrl_RS485III->u8Rdm_Sem |= RD_METER_TDM_SEM;				
				}
			}
			//�����м������б�
			QCSG_UpdateRelayTaskList(2,pstRDRelayTask_Info_RS485III);
		}
		pstRDMeter_Ctrl_RS485III->u32ReadPortParamTick = u32Tick;
	}
	
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
	if(UARTCtrl->Function == 1)
	{
		u8Period = pstRDMeter_Ctrl_RS485III->u8Rd_Period;
		//��ȡ������Ϣ
		RD_Update_RDSem(u8Period,pstRDMeter_Ctrl_RS485III,pstRDMeter_Info_RS485III);
		//��������
		RD_Meter_Process(u8Period,RS485_3PORT,pstRDMeter_Ctrl_RS485III,pstDlt645_Frame_RS485III,pstRDMeter_Info_RS485III,pstRDRelayTask_Info_RS485III,pstRDCommonTask_Info_RS485III);
	}
}
#endif
/************************ (C) COPYRIGHT M***********************END OF FILE****/