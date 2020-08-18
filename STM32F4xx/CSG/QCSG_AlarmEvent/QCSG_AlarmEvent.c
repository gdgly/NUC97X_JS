/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_AlarmEvent.c
��  ��:zhaoyun
��  ��:1.00
��  ��:20171120
��  ��:�ն�����ӿ�
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "../QCSG_Uart/Terminal_ReadMeter_ACSample.h"//err20181031 ����ͣ�ϵ����������

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../LIB/Ethernet.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"
#include "../QCSG_Head/ReadMeter_Task.h"



//typedef QCSG_RTREADMETER_REQ_INFO_S RealTimeAMRReqStruct;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

ResultEnum GenerateVoltageLossCntAlarm_E2000040(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD15Struct ARD15;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, VoltageLossCntNew = 0, VoltageLossCntOld = 0; 

	AlarmDI = 0xE2000040;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	if(NodeList[MeterIndex].PointMeterType != METER_TYPE_MULTI)  return ERROR_WRONG_ACCESS;
	
	MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_10010001, 3);
	MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_10010001, 3);
	if(VoltageLossCntNew == VoltageLossCntOld)
	{
		MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_10020001, 3);
		MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_10020001, 3);
		if(VoltageLossCntNew == VoltageLossCntOld)
		{
			MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_10030001, 3);
			MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_10030001, 3);
			if(VoltageLossCntNew == VoltageLossCntOld)
			{
				 return ERROR_WRONG_ACCESS;
			}
		}
	}
	
	MC(0xFF, (u32)&ARD15, sizeof(ARD15Struct));	
	MR((u32)&ARD15.C_03010000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_10010001, 3);
	DataLen += 3;
	MR((u32)&ARD15.C_03010000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_10010002, 3);
	DataLen += 3;

	MR((u32)&ARD15.C_03010000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_10020001, 3);
	DataLen += 3;
	MR((u32)&ARD15.C_03010000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_10020002, 3);
	DataLen += 3;

	MR((u32)&ARD15.C_03010000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_10030001, 3);
	DataLen += 3;
	MR((u32)&ARD15.C_03010000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_10030002, 3);
	DataLen += 3;

	ARD15.State = 0x01;
	ARD15.ReportState = 0;
	HAL_GetSystemTime(&ARD15.Time);
	RealTimeToYYTime(&ARD15.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD15Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD15, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

ResultEnum GenerateCurrentLossCntAlarm_E2000041(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD16Struct ARD16;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, VoltageLossCntNew = 0, VoltageLossCntOld = 0; 

	AlarmDI = 0xE2000041;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	if(NodeList[MeterIndex].PointMeterType != METER_TYPE_MULTI)  return ERROR_WRONG_ACCESS;
	
	MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_18010001, 3);
	MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_18010001, 3);
	if(VoltageLossCntNew == VoltageLossCntOld)
	{
		MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_18020001, 3);
		MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_18020001, 3);
		if(VoltageLossCntNew == VoltageLossCntOld)
		{
			MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_18030001, 3);
			MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_18030001, 3);
			if(VoltageLossCntNew == VoltageLossCntOld)
			{
				 return ERROR_WRONG_ACCESS;
			}
		}
	}
	
	MC(0xFF, (u32)&ARD16, sizeof(ARD16Struct));	
	MR((u32)&ARD16.C_030B0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_18010001, 3);
	DataLen += 3;
	MR((u32)&ARD16.C_030B0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_18010002, 3);
	DataLen += 3;

	MR((u32)&ARD16.C_030B0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_18020001, 3);
	DataLen += 3;
	MR((u32)&ARD16.C_030B0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_18020002, 3);
	DataLen += 3;

	MR((u32)&ARD16.C_030B0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_18030001, 3);
	DataLen += 3;
	MR((u32)&ARD16.C_030B0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_18030002, 3);
	DataLen += 3;

	ARD16.State = 0x01;
	ARD16.ReportState = 0;
	HAL_GetSystemTime(&ARD16.Time);
	RealTimeToYYTime(&ARD16.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD16Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD16, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

ResultEnum GenerateCurrentReverseCntAlarm_E2000042(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD17Struct ARD17;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, VoltageLossCntNew = 0, VoltageLossCntOld = 0; 

	AlarmDI = 0xE2000042;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	if(NodeList[MeterIndex].PointMeterType != METER_TYPE_MULTI)  return ERROR_WRONG_ACCESS;
	
	MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_1B010001, 3);
	MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_1B010001, 3);
	if(VoltageLossCntNew == VoltageLossCntOld)
	{
		MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_1B020001, 3);
		MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_1B020001, 3);
		if(VoltageLossCntNew == VoltageLossCntOld)
		{
			MR((u32)&VoltageLossCntNew, (u32)AlarmAMRDataNew[MeterIndex].DI_1B030001, 3);
			MR((u32)&VoltageLossCntOld, (u32)AlarmAMRDataOld[MeterIndex].DI_1B030001, 3);
			if(VoltageLossCntNew == VoltageLossCntOld)
			{
				 return ERROR_WRONG_ACCESS;
			}
		}
	}
	
	MC(0xFF, (u32)&ARD17, sizeof(ARD17Struct));	
	MR((u32)&ARD17.C_030E0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_1B010001, 3);
	DataLen += 3;
	MR((u32)&ARD17.C_030E0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_1B010002, 3);
	DataLen += 3;

	MR((u32)&ARD17.C_030E0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_1B020001, 3);
	DataLen += 3;
	MR((u32)&ARD17.C_030E0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_1B020002, 3);
	DataLen += 3;

	MR((u32)&ARD17.C_030E0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_1B030001, 3);
	DataLen += 3;
	MR((u32)&ARD17.C_030E0000[DataLen], (u32)AlarmAMRDataNew[MeterIndex].DI_1B030002, 3);
	DataLen += 3;

	ARD17.State = 0x01;
	ARD17.ReportState = 0;
	HAL_GetSystemTime(&ARD17.Time);
	RealTimeToYYTime(&ARD17.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD17Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD17, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E2000016 	A ���ѹʧѹ ARD2
ResultEnum GenerateAVoltageLossAlarm_E2000016(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE2000016;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[6], 2);//err20181026 ȡ�����ַδ��ȡ��ַ��& ��ͬ
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_10010001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_10010001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	//if((New & 0x00000001) == 0)
	//{
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016) == 0) return ERROR_WRONG_ACCESS;
		//MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		//ARD02.State = 0x00;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E2000017 	B ���ѹʧѹ ARD2
ResultEnum GenerateBVoltageLossAlarm_E2000017(u32 MeterIndex)//err20181218 ����B C��ʧѹʧ����������澯
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE2000017;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[8], 2);//err20181026 ȡ�����ַδ��ȡ��ַ��& ��ͬ
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_10020001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_10020001, 3);
		
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯

	
	//if((New & 0x00000001) == 0)
	//{
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_VOLTAGE_B_LOSS_E2000017;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_VOLTAGE_B_LOSS_E2000017) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_VOLTAGE_B_LOSS_E2000017;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E2000018 	C ���ѹʧѹ ARD2
ResultEnum GenerateCVoltageLossAlarm_E2000018(u32 MeterIndex)//err20181218 ����B C��ʧѹʧ����������澯
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE2000018;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[10], 2);//err20181026 ȡ�����ַδ��ȡ��ַ��& ��ͬ
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_10030001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_10030001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	
	//if((New & 0x00000001) == 0)
	//{
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_VOLTAGE_C_LOSS_E2000018;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_VOLTAGE_C_LOSS_E2000018) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_VOLTAGE_C_LOSS_E2000018;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E2000013A �����ʧ�� ARD2
ResultEnum GenerateACurrentLossAlarm_E2000013(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE2000013;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[6], 2);
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_18010001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_18010001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	
	//if((New & 0x00000008) == 0)
	//{
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016) == 0) return ERROR_WRONG_ACCESS;
		//MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		//ARD02.State = 0x00;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CURRENT_A_LOSS_E2000013;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CURRENT_A_LOSS_E2000013) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CURRENT_A_LOSS_E2000013;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E2000014 B �����ʧ�� ARD2
ResultEnum GenerateBCurrentLossAlarm_E2000014(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE2000014;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[8], 2);
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_18020001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_18020001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	
	//if((New & 0x00000008) == 0)
	//{
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CURRENT_B_LOSS_E2000014;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CURRENT_B_LOSS_E2000014) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CURRENT_B_LOSS_E2000014;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E2000015 C �����ʧ�� ARD2
ResultEnum GenerateCCurrentLossAlarm_E2000015(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE2000015;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[10], 2);
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_18030001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_18030001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	
	//if((New & 0x00000008) == 0)
	//{
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CURRENT_C_LOSS_E2000015;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CURRENT_C_LOSS_E2000015) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CURRENT_C_LOSS_E2000015;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E200000D 	A �ೱ������ ARD2
ResultEnum GenerateACurrentReverseAlarm_E200000D(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE200000D;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[6], 2);
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_1B010001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_1B010001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	
	//if((New & 0x00000040) == 0)
	//{
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016) == 0) return ERROR_WRONG_ACCESS;
		//MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		//ARD02.State = 0x00;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CURRENT_A_REVERSE_E200000D;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CURRENT_A_REVERSE_E200000D) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CURRENT_A_REVERSE_E200000D;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E200000E 	B �ೱ������ ARD2
ResultEnum GenerateBCurrentReverseAlarm_E200000E(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE200000E;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[8], 2);
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_1B020001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_1B020001, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	
	//if((New & 0x00000040) == 0)
	//{
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CURRENT_B_REVERSE_E200000E;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CURRENT_B_REVERSE_E200000E) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CURRENT_B_REVERSE_E200000E;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//E200000F 	C �ೱ������ ARD2
ResultEnum GenerateCCurrentReverseAlarm_E200000F(u32 MeterIndex)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0, Old = 0;

	AlarmDI = 0xE200000F;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[10], 2);
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_1B030001, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_1B030001, 3);

	
	if(New == Old) return ERROR_WRONG_ACCESS;//err20190216 �������жϷ��������ķ�ʽ�����澯
	//if((New & 0x00000040) == 0)
	//{
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CURRENT_C_REVERSE_E200000F;
	//	return ERROR_WRONG_ACCESS;
	//}
	//else
	//{
	//	if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CURRENT_C_REVERSE_E200000F) != 0) return ERROR_WRONG_ACCESS;
	//	AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CURRENT_C_REVERSE_E200000F;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		ARD02.State = 0x01;
	//}	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

//���ܱ�ʱ���쳣 ARD13
ResultEnum GenerateTimeFaultAlarm_E200003E(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD13Struct ARD13;
	//MeasurePointStruct Mp;
	RealTimeStruct MeterTime;
	RealTimeYYStruct YYTime;
	u32 TimeResult[4];
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	//AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, DiffTime = 0, ThresholdTime = 0; 
	u32 Pos = 0, Bit = 0; 

	//AlarmEventMaskParaStruct AlarmMaskPara;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	//MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	AlarmSaveParaStruct AlarmSavePara;
	MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
	
	AlarmDI = 0xE200003E;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	
	Pos = MeterIndex >> 3;//ȡ���澯��־λ��Ӧ������
	Bit = MeterIndex & 0x07;

	MeterTime.u8Second = AlarmAMRDataNew[MeterIndex].DI_04000102[0];
	MeterTime.u8Minute = AlarmAMRDataNew[MeterIndex].DI_04000102[1];
	MeterTime.u8Hour = AlarmAMRDataNew[MeterIndex].DI_04000102[2];

	MeterTime.u8Day = AlarmAMRDataNew[MeterIndex].DI_04000101[1];
	MeterTime.u8Month = AlarmAMRDataNew[MeterIndex].DI_04000101[2];
	MeterTime.u8Year = AlarmAMRDataNew[MeterIndex].DI_04000101[3];
	MeterTime.u8Week = AlarmAMRDataNew[MeterIndex].DI_04000101[0];

	MC(0xFF, (u32)&ARD13, sizeof(ARD13Struct));
	HAL_GetSystemTime(&ARD13.Time);
	MC(0, (u32)TimeResult, 16);
	QCSG_TimerSub(&ARD13.Time, &MeterTime, TimeResult);
	if(Threshold->E0000188 == 0xFF)
	{
		ThresholdTime = 0x0A;
	}
	else
	{
		ThresholdTime = bcd_hex(Threshold->E0000188);
	}
	DiffTime = TimeResult[0] * 24 *60;
	DiffTime += TimeResult[1] * 60;
	DiffTime += TimeResult[2];
	//if((TimeResult[0] == 0) && (TimeResult[1] == 0) && (TimeResult[2] <= ThresholdTime))
	if(DiffTime <= ThresholdTime)
	{
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_TIME_FAULT_E200003E) == 0) return ERROR_WRONG_ACCESS;
		//ARD13.State = 0x00;
		
		//err20190109 ���ܱ�ʱ���쳣�澯ÿ���ϱ�һ�� ���澯��־����洢
		AlarmSavePara.TimeFaultAlarmFLagList[Pos] &= ~(1 << Bit);
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		//AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_TIME_FAULT_E200003E;
		return ERROR_WRONG_ACCESS;
	}
	else
	{
		//err20190109 ʱ�ӵ�ص�ѹ�͸澯ÿ���ϱ�һ�� ���澯��־����洢
		if((AlarmSavePara.TimeFaultAlarmFLagList[Pos] & (1 << Bit)) != 0) return ERROR_WRONG_ACCESS;
		AlarmSavePara.TimeFaultAlarmFLagList[Pos] |= (1 << Bit);	
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_TIME_FAULT_E200003E) != 0) return ERROR_WRONG_ACCESS;
		//AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_TIME_FAULT_E200003E;
		ARD13.State = 0x01;
	}
		
	MR((u32)ARD13.C_04000101, (u32)AlarmAMRDataNew[MeterIndex].DI_04000101, 4);
	MR((u32)ARD13.C_04000102, (u32)AlarmAMRDataNew[MeterIndex].DI_04000102, 4);
	MR((u32)ARD13.C_E0000130, (u32)&ARD13.Time, sizeof(RealTimeStruct));

	ARD13.ReportState = 0;
	RealTimeToYYTime(&ARD13.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD13Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD13, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

ResultEnum GenerateMeterAMRFailAlarm_E200003D(u32 MeterIndex)
{	
	ARD21Struct ARD21;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	//AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, Pos = 0, Bit = 0; 
	u32 MeterIndex_Temp=0;
	AlarmDI = 0xE200003D;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;

	//err20190214 �������ʧ��һ��ֻ�ϱ�һ��
	MC(0, (u32)&ARD21, sizeof(ARD21Struct));
	for(MeterIndex_Temp = 0; MeterIndex_Temp < MAX_PN_NUM; MeterIndex_Temp++)
	{
		if(NodeList[MeterIndex_Temp].FailCnt < 3)
		{
			continue;
		}
		else
		{
			#if(TEST_ON == 1)
				NodeList[MeterIndex_Temp].FailCnt = 0;
			#else
				NodeList[MeterIndex_Temp].FailCnt--;
			#endif	
			ARD21.State = 0x01;
			AlarmAMRDataOld[MeterIndex_Temp].PowerNoneZeroStartSecond = 0;	// ��ֹ���Գ���ʧ�ܸ澯ʱ������ͣ�߸澯
			AlarmAMRDataOld[MeterIndex_Temp].AlarmFlag |= ALAMR_FLAG_TIME_FAULT_E200003E; // ��ֹ���Գ���ʧ�ܸ澯ʱ������ʱ���쳣�澯
			AlarmAMRDataOld[MeterIndex_Temp].AlarmFlag |= ALAMR_FLAG_POWER_STOP_E200002E; // ��ֹ���Գ���ʧ�ܸ澯ʱ������ͣ�߸澯
			Pos = MeterIndex_Temp >> 3;//err20181120 �������� Pos = PN/8; Bit = PN%8;  ͨ��>>��&ʵ�֡�
			Bit = MeterIndex_Temp & 0x07;	
			ARD21.Di[Pos] |= 1 << Bit;	
		}
		
	
	}
	if(ARD21.State == 0x00)//���в����㶼δ����澯������RETURN
	{
		return ERROR_WRONG_ACCESS;
	}


/*
	if(NodeList[MeterIndex].FailCnt < 3)
	{
		return ERROR_WRONG_ACCESS;
	}
	else
	{
		MC(0, (u32)&ARD21, sizeof(ARD21Struct));
		#if(TEST_ON == 1)
			NodeList[MeterIndex].FailCnt = 0;
		#else
			NodeList[MeterIndex].FailCnt--;
		#endif	
		ARD21.State = 0x01;
		AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond = 0;	// ��ֹ���Գ���ʧ�ܸ澯ʱ������ͣ�߸澯
		AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_TIME_FAULT_E200003E; // ��ֹ���Գ���ʧ�ܸ澯ʱ������ʱ���쳣�澯
		AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_POWER_STOP_E200002E; // ��ֹ���Գ���ʧ�ܸ澯ʱ������ͣ�߸澯
	}
	//Pos = MeterIndex << 3;//err20181120 �˴����� ���� д�� ����
	Pos = MeterIndex >> 3;//err20181120 �������� Pos = PN/8; Bit = PN%8;  ͨ��>>��&ʵ�֡�
	Bit = MeterIndex & 0x07;	
	ARD21.Di[Pos] |= 1 << Bit;	
*/
	
	ARD21.ReportState = 0;
	HAL_GetSystemTime(&ARD21.Time);
	RealTimeToYYTime(&ARD21.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD21Struct);
	//NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD21, &DataLen);
	NAND_WriteAlarmRecord(NodeList[0].Addr, AlarmDI, &YYTime, (u8*)&ARD21, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E200003C ���ܱ�����բʧ�� ARD12
ResultEnum GenerateMeterRelayControlAlarm_E200003C(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD12Struct ARD12;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, SystemSeconds = 0, New = 0; 

	if(AlarmAMRDataOld[MeterIndex].MeterControlStartSecond == 0) return ERROR_WRONG_ACCESS;
	
	SystemSeconds = HAL_GetSystemSecond();
	MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[4], 2);
	if(AlarmAMRDataOld[MeterIndex].MeterControlType == METER_CONTROL_SWITCH_ON)//ң�غ�բ
	{
		if((SystemSeconds - AlarmAMRDataOld[MeterIndex].MeterControlStartSecond) < 
		    AlarmAMRDataOld[MeterIndex].MeterControlLastSecond)
		{
			return ERROR_WRONG_ACCESS;
		}

		if((New & 0x00000010) == 0) return ERROR_WRONG_ACCESS;//0ͨ(��բ) 1��(��բ)
	}
	else if(AlarmAMRDataOld[MeterIndex].MeterControlType == METER_CONTROL_SWITCH_OFF)//ң����բ
	{
		if((SystemSeconds - AlarmAMRDataOld[MeterIndex].MeterControlStartSecond) < 
		    AlarmAMRDataOld[MeterIndex].MeterControlLastSecond)
		{
			return ERROR_WRONG_ACCESS;
		}
		#if(TEST_ON == 0)
		if((New & 0x00000010) != 0) return ERROR_WRONG_ACCESS;//0ͨ(��բ) 1��(��բ)
		#endif
	}
	else
	{
		 return ERROR_WRONG_ACCESS;
	}

	AlarmDI = 0xE200003C;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;

	//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_RELAY_CONTROL_FAIL_E200003C) != 0) return ERROR_WRONG_ACCESS;
	//AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_RELAY_CONTROL_FAIL_E200003C;

	AlarmAMRDataOld[MeterIndex].MeterControlStartSecond = 0;
	AlarmAMRDataOld[MeterIndex].MeterControlLastSecond = 0;
	AlarmAMRDataOld[MeterIndex].MeterControlType = 0;
	MC(0xFF, (u32)&ARD12, sizeof(ARD12Struct));	
	MR((u32)ARD12.C_04000503, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[4], 2);

	ARD12.State = 0x01;
	ARD12.ReportState = 0;
	HAL_GetSystemTime(&ARD12.Time);
	RealTimeToYYTime(&ARD12.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD12Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD12, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E200003B �̵�����λ ARD12
//̨�����ʱ���� ̨�������Ӧ����ܱ�״̬�����ݿ�D1 ���޷������澯
//�����޷����������ݲ����澯��
ResultEnum GenerateRelayChangeAlarm_E200003B(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD12Struct ARD12;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, Old = 0, New = 0; 

	AlarmDI = 0xE200003B;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;

	MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_04000503[0], 2);//err20190313 ̨�岻��Ӧ���ݿ� ��Ϊ���ܱ�״̬��3
	MR((u32)&Old, (u32)&AlarmAMRDataOld[MeterIndex].DI_04000503[0], 2);

	if((Old & 0x00000010) == (New & 0x00000010)) return ERROR_WRONG_ACCESS;

	AlarmAMRDataOld[MeterIndex].MeterControlStartSecond = 0;
	AlarmAMRDataOld[MeterIndex].MeterControlLastSecond = 0;
	
	MC(0xFF, (u32)&ARD12, sizeof(ARD12Struct));	
	MR((u32)ARD12.C_04000503, (u32)&AlarmAMRDataNew[MeterIndex].DI_04000503[0], 2);
	ARD12.State = 0x01;
	ARD12.ReportState = 0;
	HAL_GetSystemTime(&ARD12.Time);
	RealTimeToYYTime(&ARD12.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD12Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD12, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//���ܱ���ʱ�����
ResultEnum GenerateProgramTimeChangeAlarm_E2000035(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, Old = 0, New = 0;

	AlarmDI = 0xE2000035;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_03300000, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_03300000, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;
	if(QCSG_BCDCheck((u8 *)&Old,4) != TRUE) return ERROR_WRONG_ACCESS;
	if(QCSG_BCDCheck((u8 *)&New,4) != TRUE) return ERROR_WRONG_ACCESS;
	
	
	MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));	
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);
	ARD02.State = 0x01;
	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E2000036 ���ܱ�ʱ�λ���ʸ��� ARD1
ResultEnum GenerateTimeRateChangeAlarm_E2000036(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD01Struct ARD01;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, Old = 0, New = 0;

	AlarmDI = 0xE2000036;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	MR((u32)&New, (u32)AlarmAMRDataNew[MeterIndex].DI_03300500, 3);
	MR((u32)&Old, (u32)AlarmAMRDataOld[MeterIndex].DI_03300500, 3);
	
	if(New == Old) return ERROR_WRONG_ACCESS;
	
	MC(0xFF, (u32)&ARD01, sizeof(ARD01Struct));	
	ARD01.State = 0x01;
	ARD01.ReportState = 0;
	HAL_GetSystemTime(&ARD01.Time);
	RealTimeToYYTime(&ARD01.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD01Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD01, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E200002B 	ʣ���Ѳ���
ResultEnum GenerateLessMoneyAlarm_E200002B(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD04Struct ARD04;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, LeftMoney = 0;

	AlarmDI = 0xE200002B;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	//20190109 ʣ����ΪFF(���d1)ʱ�����и澯
	if(QCSG_BCDCheck(AlarmAMRDataNew[MeterIndex].DI_00900200,4)!=TRUE)
		return ERROR_WRONG_ACCESS;

	MR((u32)&LeftMoney, (u32)AlarmAMRDataNew[MeterIndex].DI_00900200,4);
	if(LeftMoney >= Threshold->E000018B)
	{
		//MC(0xFF, (u32)&ARD04, sizeof(ARD04Struct));
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_LESS_MONEY_E200002B) == 0) return ERROR_WRONG_ACCESS;
		AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_LESS_MONEY_E200002B;
		//ARD04.State = 0x00;
		return ERROR_WRONG_ACCESS;
	}
	else
	{
		if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_LESS_MONEY_E200002B) != 0) return ERROR_WRONG_ACCESS;
		AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_LESS_MONEY_E200002B;
		MC(0xFF, (u32)&ARD04, sizeof(ARD04Struct));
		ARD04.State = 0x01;	
	}
		
	MR((u32)ARD04.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD04.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD04.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD04.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	
	//MR((u32)ARD04.C_E1000008, (u32)AlarmAMRDataNew[MeterIndex].DI_00900200, 4);
	ARD04.C_E1000008[0]=AlarmAMRDataNew[MeterIndex].DI_00900200[1];
	ARD04.C_E1000008[1]=AlarmAMRDataNew[MeterIndex].DI_00900200[2];
	ARD04.C_E1000008[2]=AlarmAMRDataNew[MeterIndex].DI_00900200[3];
	ARD04.C_E1000008[3]=0;


	
	ARD04.ReportState = 0;
	HAL_GetSystemTime(&ARD04.Time);
	RealTimeToYYTime(&ARD04.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD04Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD04, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E200002E 	���ܱ�ͣ�� ARD2
ResultEnum GeneratePowerStopAlarm_E200002E(u32 MeterIndex, ThresholdParaStruct* Threshold)
{	
	ARD02Struct ARD02;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 OldPower = 0, NewPower = 0, AlarmDI = 0, DataLen = 0, PowerSecond = 0;
	u32 ThresholdPower = 0, CaculatePower = 0;

	AlarmDI = 0xE200002E;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	MR((u32)&NewPower, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 3);

	if(QCSG_BCDCheck((u8 *) &NewPower, 3)!=TRUE) return ERROR_WRONG_ACCESS;
	
	if(NewPower == 0) //����Ϊ0��������ͣ��
	{
		AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond = 0;
		AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_POWER_STOP_E200002E;
		return ERROR_WRONG_ACCESS;
	}
	if(AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond == 0)	//��һ�ν���
	{
		AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond = HAL_GetSystemSecond();
		return ERROR_WRONG_ACCESS;
	}

	NewPower = 0;
	MR((u32)&NewPower, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)&OldPower, (u32)AlarmAMRDataOld[MeterIndex].DI_00010000, 4);
	
	if(QCSG_BCDCheck((u8 *) &NewPower, 4)!=TRUE) return ERROR_WRONG_ACCESS;//
	if(QCSG_BCDCheck((u8 *) &OldPower, 4)!=TRUE) return ERROR_WRONG_ACCESS;//

	
	if(NewPower != OldPower) 	//ǰ�����ι��ʲ�һ�����޷�����
	{
		//MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_POWER_STOP_E200002E) == 0) return ERROR_WRONG_ACCESS;
		AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_POWER_STOP_E200002E;
		//ARD02.State = 0x00;
		return ERROR_WRONG_ACCESS;
	}
	else
	{
		if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_POWER_STOP_E200002E) != 0) return ERROR_WRONG_ACCESS;
		NewPower = 0;
		MC(0xFF, (u32)&ARD02, sizeof(ARD02Struct));
		MR((u32)&NewPower, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 3);
		NewPower = bcd_hex(NewPower);
		ThresholdPower = bcd_hex(Threshold->E000018A);
		if(ThresholdPower == 0) ThresholdPower = 10;
		PowerSecond = HAL_GetSystemSecond();
		PowerSecond -= AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond;
			CaculatePower = (NewPower * PowerSecond)/3600;
		if(CaculatePower < (ThresholdPower*100)) return ERROR_WRONG_ACCESS;//err20180623 ���ڴ洢��λ��ͬ��ͣ����ֵӦ*100������������бȽϡ�
		AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond = 0;	//����ͣ�߸澯�������´���Ҫ���¼�ʱ
		AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_POWER_STOP_E200002E;
		ARD02.State = 0x01;
	}
		
	MR((u32)ARD02.C_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD02.C_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD02.C_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD02.C_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD02.C_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD02.C_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD02.C_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD02.C_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);
	MR((u32)ARD02.C_0206FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0206FF00, 8);

	ARD02.ReportState = 0;
	HAL_GetSystemTime(&ARD02.Time);
	RealTimeToYYTime(&ARD02.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD02, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}
//E200002C ʾ���½� ARD3
ResultEnum GeneratePowerGoDownAlarm_E200002C(u32 MeterIndex)
{	
	ARD03Struct ARD03;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 OldPower = 0, NewPower = 0, AlarmDI = 0, DataLen = 0;

	AlarmDI = 0xE200002C;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;
	
	MR((u32)&OldPower, (u32)AlarmAMRDataOld[MeterIndex].DI_00010000, 4);
	MR((u32)&NewPower, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);

	if(NewPower >= OldPower)
	{
		MR((u32)&OldPower, (u32)AlarmAMRDataOld[MeterIndex].DI_00020000, 4);
		MR((u32)&NewPower, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	}
	if(NewPower >= OldPower)
	{
		//MC(0xFF, (u32)&ARD03, sizeof(ARD03Struct));
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_POWER_GO_DOWN_E200002C) == 0) return ERROR_WRONG_ACCESS;
		AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_POWER_GO_DOWN_E200002C;
		//ARD03.State = 0x00;
		return ERROR_WRONG_ACCESS;
	}
	else
	{
		if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_POWER_GO_DOWN_E200002C) != 0) return ERROR_WRONG_ACCESS;
		MC(0xFF, (u32)&ARD03, sizeof(ARD03Struct));
		AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_POWER_GO_DOWN_E200002C;
		AlarmAMRDataOld[MeterIndex].PowerNoneZeroStartSecond = 0; //���ܱ�ʾ���½��澯��ͣ�߸澯���⣬����ͬʱ����
		ARD03.State = 0x01;
	}
	
	MR((u32)ARD03.B_00010000, (u32)AlarmAMRDataOld[MeterIndex].DI_00010000, 4);
	MR((u32)ARD03.B_00020000, (u32)AlarmAMRDataOld[MeterIndex].DI_00020000, 4);
	MR((u32)ARD03.B_00030000, (u32)AlarmAMRDataOld[MeterIndex].DI_00030000, 4);
	MR((u32)ARD03.B_00040000, (u32)AlarmAMRDataOld[MeterIndex].DI_00040000, 4);
	MR((u32)ARD03.B_0201FF00, (u32)AlarmAMRDataOld[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD03.B_0202FF00, (u32)AlarmAMRDataOld[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD03.B_0203FF00, (u32)AlarmAMRDataOld[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD03.B_0204FF00, (u32)AlarmAMRDataOld[MeterIndex].DI_0204FF00, 12);

	MR((u32)ARD03.A_00010000, (u32)AlarmAMRDataNew[MeterIndex].DI_00010000, 4);
	MR((u32)ARD03.A_00020000, (u32)AlarmAMRDataNew[MeterIndex].DI_00020000, 4);
	MR((u32)ARD03.A_00030000, (u32)AlarmAMRDataNew[MeterIndex].DI_00030000, 4);
	MR((u32)ARD03.A_00040000, (u32)AlarmAMRDataNew[MeterIndex].DI_00040000, 4);
	MR((u32)ARD03.A_0201FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0201FF00, 6);
	MR((u32)ARD03.A_0202FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0202FF00, 9);
	MR((u32)ARD03.A_0203FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0203FF00, 12);
	MR((u32)ARD03.A_0204FF00, (u32)AlarmAMRDataNew[MeterIndex].DI_0204FF00, 12);

	ARD03.ReportState = 0;
	HAL_GetSystemTime(&ARD03.Time);
	RealTimeToYYTime(&ARD03.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD03Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD03, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

ResultEnum GenerateTerminalGPRSRxTxByteAlarm_E200003A(void)
{	
	ARD10Struct ARD10;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	ThresholdParaStruct Threshold;
	//AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, RxTxBytes = 0;

	//AlarmEventMaskParaStruct AlarmMaskPara;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	//MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	AlarmSaveParaStruct AlarmSavePara;//err20190216 �޸ĵ���洢��־�Ĵ洢λ��
	MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		
	MR((u32)&Threshold, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));
	if(Threshold.E000018C == 0) return ERROR_WRONG_ACCESS;

	//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	if((AlarmSavePara.AlarmDayReportFlag & ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A_SAVE_FLAG) != 0) return ERROR_WRONG_ACCESS;
	//if((AlarmAMRDataOld[0].AlarmFlag & ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A) != 0) return ERROR_WRONG_ACCESS;
	
	AlarmDI = 0xE200003A;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	RxTxBytes = Get_GPRSCurMonthRxTxBytes();
	if(RxTxBytes <= Threshold.E000018C) return ERROR_WRONG_ACCESS;	

	////err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	AlarmSavePara.AlarmDayReportFlag |= ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A_SAVE_FLAG;
	//AlarmAMRDataOld[0].AlarmFlag |= ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
	
	MC(0, (u32)&ARD10, sizeof(ARD10Struct));
	MR((u32)ARD10.C_E1800013, (u32)&RxTxBytes, 3);
	ARD10.State = 0x01;
	ARD10.ReportState = 0;
	HAL_GetSystemTime(&ARD10.Time);
	RealTimeToYYTime(&ARD10.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD10Struct);
	NAND_WriteAlarmRecord(NodeList[0].Addr, AlarmDI, &YYTime, (u8*)&ARD10, &DataLen);
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}


#if (Project == 500)
//E2000032 	ʱ�ӵ�ص�ѹ���� ARD1
ResultEnum	GenerateMeterClockBatteryLowVoltageAlarm_E2000032(u32 MeterIndex)
{
	ARD01Struct ARD01;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	//AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0, New = 0;
	u32 Pos = 0, Bit = 0; 

	//AlarmEventMaskParaStruct AlarmMaskPara;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	//MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	
	AlarmSaveParaStruct AlarmSavePara;
	MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
	
	AlarmDI = 0xE2000032;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	if(MeterIndex > MAX_PN_NUM) return ERROR_WRONG_ACCESS;

	Pos = MeterIndex >> 3;//ȡ���澯��־λ��Ӧ������
	Bit = MeterIndex & 0x07;
	
	MR((u32)&New, (u32)&AlarmAMRDataNew[MeterIndex].DI_040005FF[0], 2);	
	if((New & 0x00000004) == 0)//�����ϸ澯����
	{	
		//err20190109 ʱ�ӵ�ص�ѹ�͸澯ÿ���ϱ�һ�� ���澯��־����洢
		AlarmSavePara.ClkBatLowVolAlarmFLagList[Pos] &= ~(1 << Bit);
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		//MW((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
		//AlarmAMRDataOld[MeterIndex].AlarmFlag &= ~ALAMR_FLAG_CLK_BATT_LOW_E2000032;
		return ERROR_WRONG_ACCESS;
	}
	else//���ϸ澯����
	{	
		//err20190109 ʱ�ӵ�ص�ѹ�͸澯ÿ���ϱ�һ�� ���澯��־����洢
		if((AlarmSavePara.ClkBatLowVolAlarmFLagList[Pos] & (1 << Bit)) != 0) return ERROR_WRONG_ACCESS;
		AlarmSavePara.ClkBatLowVolAlarmFLagList[Pos] |= (1 << Bit);	
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		//if((AlarmAMRDataOld[MeterIndex].AlarmFlag & ALAMR_FLAG_CLK_BATT_LOW_E2000032) != 0) return ERROR_WRONG_ACCESS;
		//AlarmAMRDataOld[MeterIndex].AlarmFlag |= ALAMR_FLAG_CLK_BATT_LOW_E2000032;
		
		MC(0xFF, (u32)&ARD01, sizeof(ARD01Struct));
		ARD01.State = 0x01;
	}
	ARD01.ReportState = 0;
	HAL_GetSystemTime(&ARD01.Time);
	RealTimeToYYTime(&ARD01.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	DataLen = sizeof(ARD01Struct);
	NAND_WriteAlarmRecord(NodeList[MeterIndex].Addr, AlarmDI, &YYTime, (u8*)&ARD01, &DataLen);
	
	if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(AlarmDI);
	return NO_ERROR;
}

#endif

#if(Project == 500||Project == 300)
ResultEnum	GenerateTerminalClockBatteryLowVoltageAlarm_E2000032(void)
{
	ARD01Struct ARD01;
	//MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	//AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	//AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u32 AlarmDI = 0, DataLen = 0;
	u32 Pos = 0, Bit = 0; 
	
	//AlarmEventMaskParaStruct AlarmMaskPara;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	//MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	AlarmSaveParaStruct AlarmSavePara;//err20190216 �޸ĵ���洢��־�Ĵ洢λ��
	MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));

	AlarmDI = 0xE2000032;
	if(QCSG_CheckAlarmConfig(AlarmDI) == FALSE) return ERROR_WRONG_ACCESS;

	Pos = 0 ;//�ն˶�Ӧ������0
	Bit = 0 ;

	if(Comm_Ram->URTC<0x220)     //������ʱ�ӵ�ص�ѹС��2.2V
	{
		//err20190109 ʱ�ӵ�ص�ѹ�͸澯ÿ���ϱ�һ�� ���澯��־����洢
		if((AlarmSavePara.ClkBatLowVolAlarmFLagList[Pos] & (1 << Bit)) != 0) return ERROR_WRONG_ACCESS;
		AlarmSavePara.ClkBatLowVolAlarmFLagList[Pos] |= (1 << Bit);	
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		//if((AlarmAMRDataOld[0].AlarmFlag & ALAMR_FLAG_CLK_BATT_LOW_E2000032) != 0) return ERROR_WRONG_ACCESS;
		//AlarmAMRDataOld[0].AlarmFlag |= ALAMR_FLAG_CLK_BATT_LOW_E2000032;
		
		MC(0xFF, (u32)&ARD01, sizeof(ARD01Struct));
		ARD01.State = 0x01;		
	
		ARD01.ReportState = 0;
		HAL_GetSystemTime(&ARD01.Time);
		RealTimeToYYTime(&ARD01.Time, &YYTime);
		YYTime.Report = 0;
		YYTime.Ack = 0;
		DataLen = sizeof(ARD01Struct);
		NAND_WriteAlarmRecord(NodeList[0].Addr, AlarmDI, &YYTime, (u8*)&ARD01, &DataLen);
		if(QCSG_CheckAlarmReport(AlarmDI) == TRUE) 
		{
			TerminalReportAlarmInc();
		}
		TerminalSetLastAlarmDI(AlarmDI);
		return NO_ERROR;
	}
	else//err20181120 ԭ����δ�жϸ澯�ָ���־���㣬������
	{
		//err20190109 ʱ�ӵ�ص�ѹ�͸澯ÿ���ϱ�һ�� ���澯��־����洢
		AlarmSavePara.ClkBatLowVolAlarmFLagList[Pos] &= ~(1 << Bit);
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));

		//AlarmAMRDataOld[0].AlarmFlag &= ~ALAMR_FLAG_CLK_BATT_LOW_E2000032;
		return ERROR_WRONG_ACCESS;
	}
	
}
#endif


//err20181112 ���Ӹ澯 CT���β࿪·��· �������ʱ�򿪣�ʵ��ʹ��ʱ�ر� ��ֹ�󱨡�
//AlarmDI:�澯���ݱ�ʶ
//AlarmState: 1������0�ָ�
#if ((Project/100) == 3)

#define JC_IN								1000
#define JC_UN								2200
#define ALARM_START							0x01
#define ALARM_END							0x02

ResultEnum GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(u32 AlarmDI,u32 AlarmState)
{	
	//QCSG_ALARM_E2000007_LIST_S* pstAlarm_List = (QCSG_ALARM_E2000010_LIST_S*)QCSG_ALARM_E2000010_LIST;
	QCSG_ALARM_E2000007_0C_INFO_S* pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E2000007_LIST;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	MeasurePointStruct stPnPara;
	RealTimeYYStruct stYYTime;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0,i = 0,u8Meterphase = 0x04;
	u8 u8MeterId[6];
	u32 u32ReadLen = 0;
	TIME_PARAM_S stSysCurrTime;
	u32 u32Ix = 0,u32Ux = 0;
	u32 u32Un = 0;
	
	HAL_GetSystemTime(&stSysCurrTime);

	switch(AlarmDI)
	{
		case 0xE2000007:
			pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E2000007_LIST;
			break;
		case 0xE2000008:
			pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E2000008_LIST;
			break;
		case 0xE2000009:
			pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E2000009_LIST;
			break;
		case 0xE200000A:
			pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E200000A_LIST;
			break;
		case 0xE200000B:
			pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E200000B_LIST;
			break;
		case 0xE200000C:
			pstAlarm_Info = (QCSG_ALARM_E2000007_0C_INFO_S*)QCSG_ALARM_E200000C_LIST;
			break;
		default: 
			return ERROR_WRONG_ACCESS;
			//break;
	}


	if((QCSG_CheckAlarmConfig(AlarmDI) != TRUE))
	{
		return ERROR_WRONG_ACCESS;
	}
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;	
	for(i = 0;i < u8PnNum;i++)
	{
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
		if(0 == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return ERROR_WRONG_ACCESS;
	}

	if(0x00 == pstAlarm_Info->u8AlarmFlag)
	{//���ν���,��ʼ��
		pstAlarm_Info->u8AlarmFlag = 0xFF;
	}

	//��ȡ��������߷�ʽ�����ѹ�������
	u32ReadLen = sizeof(MeasurePointStruct);
	if(NAND_ReadMeasurePoint(0,0xE0800010,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
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

	
	switch(AlarmDI)
	{
		case 0xE2000007:
		case 0xE200000A:
			MemCopy(&u32Ix,&pstRam_Data->u8Data0202FF00[0],3);
			MemCopy(&u32Ux,&pstRam_Data->u8Data0201FF00[0],2);	
			break;
		case 0xE2000008:
		case 0xE200000B:
			MemCopy(&u32Ix,&pstRam_Data->u8Data0202FF00[3],3);	
			MemCopy(&u32Ux,&pstRam_Data->u8Data0201FF00[2],2);
			break;
		case 0xE2000009:
		case 0xE200000C:
			MemCopy(&u32Ix,&pstRam_Data->u8Data0202FF00[6],3);
			MemCopy(&u32Ux,&pstRam_Data->u8Data0201FF00[4],2);
			break;
		default: 
			return ERROR_WRONG_ACCESS;
			//break;
	}


	u32Ix = u32Ix & 0x007FFFFF;
	u32Ix = QCSG_BCD2HEX((u8)(u32Ix>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ix>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ix);

	u32Ux = QCSG_BCD2HEX((u8)(u32Ux>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ux);

	//����ѹ����������Χ����ȥ�жϸ澯
	if( ((u32Ux * 100) < (u32Un * 78)) || ((u32Ux * 100) > (u32Un * 120)) )
	{
		return ERROR_WRONG_ACCESS;
	}
	if(AlarmState == 1)//�澯��������
	{
		if(u32Ix >= 20)	//�����жϵ���0.02A���� ̨���·��0.01A���� ��·Ϊ0 �ָ������Ƶ�����С
		return ERROR_WRONG_ACCESS;
		
		pstAlarm_Info->u8RestoreStep = ALARM_END;
		if(pstAlarm_Info->u8AlarmFlag == 0xFF)
		{
			if(pstAlarm_Info->u8OccurStep == ALARM_START)
			{//�ڶ����������� �����澯
				//���������¼�
				GetACSampleAddr(u8MeterId);	//ȡ���ɵ�ַ
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(&stSysCurrTime,&stYYTime);
				RD_Write_ARD02_Data(1,&stSysCurrTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(u8MeterId,AlarmDI,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(AlarmDI);
				if(QCSG_CheckAlarmReport(AlarmDI) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8OccurStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 0x01;
			}
			else 
			{//��һ���������� �������澯
				pstAlarm_Info->u8OccurStep = ALARM_START;
			}
		}
	}
	else //�澯�ָ�
	{
		pstAlarm_Info->u8OccurStep = ALARM_END;
		if(pstAlarm_Info->u8AlarmFlag == 0x01)
		{
			if(pstAlarm_Info->u8RestoreStep == ALARM_START)
			{
				//�����ָ��¼�
				GetACSampleAddr(u8MeterId);	//ȡ���ɵ�ַ
				MemSet(&stYYTime,0,sizeof(stYYTime));
				RD_Time_TO_YYTime(&stSysCurrTime,&stYYTime);
				RD_Write_ARD02_Data(0,&stSysCurrTime,pstRam_Data,&stARD02Data);
				u32ReadLen = sizeof(stARD02Data);
				NAND_WriteAlarmRecord(u8MeterId,AlarmDI,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
				TerminalSetLastAlarmDI(AlarmDI);
				if(QCSG_CheckAlarmReport(AlarmDI) == TRUE)
				{
					TerminalReportAlarmInc();
				}								
				pstAlarm_Info->u8RestoreStep = ALARM_END;
				pstAlarm_Info->u8AlarmFlag = 0xFF;	
			}
			else
			{
				pstAlarm_Info->u8RestoreStep = ALARM_START;
			}
		}
	}
	
	return NO_ERROR;
}

#endif

ResultEnum GenerateTerminalPowerUpAndDownAlarm(RealTimeStruct* PowerTime, u32 PowerType)
{
	ARD02Struct PowerAlarm;
	//MeasurePointStruct Mp;//err20180529
	RealTimeYYStruct YYTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList; //err20180529 ͣ�ϵ�澯����RAM�е�ַ ��ֹ��FLASH�в�һ�µ����޷��ϱ� 
	u32 DataLen = 0;
	u32 PowerAlarmDI = 0;
	#if((Project/100)==3 || (Project/100)==9)
		u8* pData = NULL;
	//err20181031 ͣ�ϵ�澯 �� ��������
		u32 Temp_E;//���ɵ��ܻ������
		u32 ADDR_DATA;
		ADDR_DATA=Get_ADDR_UARTnTx(ACSAMPLEPORT)+14;
	#endif
	

	if(PowerType == 0)
	{
		PowerAlarmDI = 0xE2000033;		// �ն˵���澯
	}
	else
	{
		PowerAlarmDI = 0xE2000034;		// �ն��ϵ��澯
	}

	if(QCSG_CheckAlarmConfig(PowerAlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	
	MC(0xFF, (u32)&PowerAlarm, sizeof(ARD02Struct));	
	PowerAlarm.State = ALARM_OCCUR;
	PowerAlarm.ReportState = 0;
	MR((u32)&PowerAlarm.Time, (u32)PowerTime, sizeof(RealTimeStruct));
	RealTimeToYYTime(PowerTime, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	
	#if((Project/100)==3 || (Project/100)==9)
	//err20181031 ͣ�ϵ�澯 �� �������� 
		GetACSample(0x0001ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
		Temp_E=MRR(ADDR_DATA,4);
		MR((u32)PowerAlarm.C_00010000,(u32)&Temp_E, 4);
		
		GetACSample(0x0002ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
		Temp_E=MRR(ADDR_DATA,4);
		MR((u32)PowerAlarm.C_00020000,(u32)&Temp_E, 4);

		GetACSample(0x0003ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
		Temp_E=MRR(ADDR_DATA,4);
		MR((u32)PowerAlarm.C_00030000,(u32)&Temp_E, 4);

		GetACSample(0x0004ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
		Temp_E=MRR(ADDR_DATA,4);
		MR((u32)PowerAlarm.C_00040000,(u32)&Temp_E, 4);

	
		//MR((u32)PowerAlarm.C_00010000, ADDR_00010000L, 4);
		//MR((u32)PowerAlarm.C_00020000, ADDR_00020000L, 4);
		//MR((u32)PowerAlarm.C_00030000, ADDR_00030000L, 4);
		//MR((u32)PowerAlarm.C_00040000, ADDR_00040000L, 4);

		if(PowerType == 0)//err20181031 ����澯�� ������0
		{
			MC(0xFF, (u32)PowerAlarm.C_0201FF00, SIZE_20);
			MC(0, (u32)PowerAlarm.C_0201FF00, 6);//��ѹ���� ��0 
			MC(0xFF, (u32)PowerAlarm.C_0202FF00, SIZE_20);
			MC(0, (u32)PowerAlarm.C_0202FF00, 9);//�������� ��0 
			MC(0, (u32)PowerAlarm.C_0203FF00, 12);//�й��������� ��0 
			MC(0, (u32)PowerAlarm.C_0204FF00, 12);//�޹��������� ��0 
			MC(0, (u32)PowerAlarm.C_0206FF00, 8);//�������� ��0
		}
		else 
		{
			MC(0xFF, (u32)PowerAlarm.C_0201FF00, SIZE_20);
			pData = (u8*)ADDR_UA;
			DataLen = 0;
			PowerAlarm.C_0201FF00[DataLen++] = pData[2];
			PowerAlarm.C_0201FF00[DataLen++] = pData[3];
			pData = (u8*)ADDR_UB;
			PowerAlarm.C_0201FF00[DataLen++] = pData[2];
			PowerAlarm.C_0201FF00[DataLen++] = pData[3];
			pData = (u8*)ADDR_UC;
			PowerAlarm.C_0201FF00[DataLen++] = pData[2];
			PowerAlarm.C_0201FF00[DataLen++] = pData[3];

			MC(0xFF, (u32)PowerAlarm.C_0202FF00, SIZE_20);
			pData = (u8*)ADDR_IA;
			DataLen = 0;
			PowerAlarm.C_0202FF00[DataLen++] = pData[1];
			PowerAlarm.C_0202FF00[DataLen++] = pData[2];
			PowerAlarm.C_0202FF00[DataLen++] = pData[3];
			pData = (u8*)ADDR_IB;
			PowerAlarm.C_0202FF00[DataLen++] = pData[1];
			PowerAlarm.C_0202FF00[DataLen++] = pData[2];
			PowerAlarm.C_0202FF00[DataLen++] = pData[3];
			pData = (u8*)ADDR_IC;
			PowerAlarm.C_0202FF00[DataLen++] = pData[1];
			PowerAlarm.C_0202FF00[DataLen++] = pData[2];
			PowerAlarm.C_0202FF00[DataLen++] = pData[3];

			pData = (u8*)ADDR_PT;
			MR((u32)PowerAlarm.C_0203FF00, (u32)pData, SIZE_12);
			pData = (u8*)ADDR_QT;
			MR((u32)PowerAlarm.C_0204FF00, (u32)pData, SIZE_12);
			pData = (u8*)ADDR_COST;
			MR((u32)PowerAlarm.C_0206FF00, (u32)pData, SIZE_8);
		}
	#endif
	
	//NAND_ReadMeasurePointByPn(0, &Mp);//err20180529 FLASH�в������ַ��Ϊ RAM�е�ַ
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(NodeList[0].Addr, PowerAlarmDI, &YYTime, (u8*)&PowerAlarm, &DataLen);//err20180529 FLASH�в������ַ��Ϊ RAM�е�ַ
	if(QCSG_CheckAlarmReport(PowerAlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	TerminalSetLastAlarmDI(PowerAlarmDI);
	return NO_ERROR;
}

ResultEnum GenerateParaChangeEvent(u32 DI)
{
	MeasurePointStruct Mp;
	ERD08Struct ERD08;
	RealTimeYYStruct YYTime;
	u32 DataLen = 0;
	u32 ParaChangeDI = 0xE2010014;

	if(QCSG_CheckEventMask(ParaChangeDI) == FALSE) return ERROR_WRONG_ACCESS;
	ERD08.DI = DI;
	HAL_GetSystemTime(&ERD08.Time);
	RealTimeToYYTime(&ERD08.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	NAND_ReadMeasurePointByPn(0, &Mp);
	DataLen = sizeof(ERD08Struct);
	NAND_WriteEventRecord(Mp.Addr, ParaChangeDI, &YYTime, (u8*)&ERD08, &DataLen);
	return NO_ERROR;
}

ResultEnum GeneratePowerEvent(RealTimeStruct* PowerUpTime, RealTimeStruct* PowerDownTime)
{
	ERD04Struct PowerEvent;
	MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	u32 DataLen = 0;
	u32 PowerEventDI = 0xE201000A;
	if(QCSG_CheckEventMask(PowerEventDI) == FALSE) return ERROR_WRONG_ACCESS;

	MR((u32)&PowerEvent.PowerUpTime, (u32)PowerUpTime, sizeof(RealTimeStruct));
	MR((u32)&PowerEvent.PowerDownTime, (u32)PowerDownTime, sizeof(RealTimeStruct));
	RealTimeToYYTime(PowerUpTime, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	NAND_ReadMeasurePointByPn(0, &Mp);
	DataLen = sizeof(ERD04Struct);
	NAND_WriteEventRecord(Mp.Addr, PowerEventDI, &YYTime, (u8*)&PowerEvent, &DataLen);
	return NO_ERROR;
}

ResultEnum GenerateMpControlEvent(u32 ControlType, MeasurePointStruct* Mp, u32 Pn, u32 Time)
{
	ERD06Struct ERD06;
	RealTimeYYStruct YYTime;
	u32 PnNum = 0;
	u32 DataLen = 0;
	u32 MpControlDI = 0xE201000E;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	
	if(QCSG_CheckEventMask(MpControlDI) == FALSE) return ERROR_WRONG_ACCESS;
	HAL_GetSystemTime(&ERD06.Time);
	RealTimeToYYTime(&ERD06.Time, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	ERD06.Type = ControlType;
	PnNum = SwitchPnToNum(Pn);
	MR((u32)ERD06.C_00010000, (u32)AlarmAMRDataOld[PnNum].DI_00010000, SIZE_4);
	MR((u32)ERD06.C_00020000, (u32)AlarmAMRDataOld[PnNum].DI_00020000, SIZE_4);
	DataLen = sizeof(ERD06Struct);
	NAND_WriteEventRecord(Mp->Addr, MpControlDI, &YYTime, (u8*)&ERD06, &DataLen);
	if((ControlType == METER_CONTROL_SWITCH_ON) || (ControlType == METER_CONTROL_SWITCH_OFF))
	{
		AlarmAMRDataOld[PnNum].MeterControlStartSecond = HAL_GetSystemSecond();
		AlarmAMRDataOld[PnNum].MeterControlLastSecond = Time * 60;
		AlarmAMRDataOld[PnNum].MeterControlType = ControlType;
	}	
	return NO_ERROR;
}

void ClearNodeAlarmFlag(void)
{
	u32 i = 0;
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;

	#if ((Project/100)==3)//err20190507 ���ظ澯 ʣ�����澯ÿ�����
		QCSG_ALARM_E200002B_LIST_S* pstAlarm_List = (QCSG_ALARM_E200002B_LIST_S*)QCSG_ALARM_E200002B_LIST;
	#endif
	
	//AlarmEventMaskParaStruct AlarmMaskPara;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	//MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	AlarmSaveParaStruct AlarmSavePara;//err20190216 �޸ĵ���洢��־�Ĵ洢λ��
	MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));

	AlarmSavePara.AlarmDayReportFlag = 0;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	
	for(i = 0; i < SIZE_128; i++)//err20190109 ���ʱ�ӵ�ص�ѹ�͡����ܱ�ʱ���쳣�澯��־
	{
		AlarmSavePara.ClkBatLowVolAlarmFLagList[i] = 0x00;
		AlarmSavePara.TimeFaultAlarmFLagList[i] = 0x00;
	}
	
	//MW((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
	MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));

	for(i = 0; i < MAX_PN_NUM; i++)
	{
		AlarmAMRDataOld[i].AlarmFlag = 0;
	}

	//err20190507 ���ظ澯 ʣ�����澯ÿ�����
	#if ((Project/100)==3)
		MemSet(pstAlarm_List,0,sizeof(QCSG_ALARM_E200002B_LIST_S));
	#endif
}

/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


