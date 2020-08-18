

/*************************************** (C) COPYRIGHT 2017 CHINT ***************************************
* File Name          : QCSG_NandDataInterface.h
* Author             : zhaoyun
* Version            : V1.0.0
* Date               : 2017/06/26
* Description        : This file Contains the nand flash read and write interface 
********************************************************************************************************/

#ifndef __QCSG_NandDataInterface__
#define __QCSG_NandDataInterface__

//#include "../Hcsg/ProjectCSG.h"
//#include "../QCSG_Head/QCSG_UpLinkProtocol.h"


#if ((Project/100)==3) //����
//����������������16 ������20
#define AMR_CURVE_MAX_METER_COUNT 16
//��������¼������288 ������24
#define AMR_CURVE_MAX_DEEP 288 
//�����ܶ� ����5���� ������һСʱ(60����)
#define AMR_CURVE_DENSITY 5
#else
#define AMR_CURVE_MAX_METER_COUNT 20
//��������¼������288 ������24
#define AMR_CURVE_MAX_DEEP 24 
//�����ܶ� ����5���� ������һСʱ(60����)
#define AMR_CURVE_DENSITY 60
#endif
#define AMR_CURVE_MAX_FILEDEEP (AMR_CURVE_MAX_METER_COUNT * AMR_CURVE_MAX_DEEP)

//err. ע��˴��������������ն˵�ʱ��Ӧ���Ƕ��٣�

#if ((Project/100)==3) //����
//����������������16 
#define NORMAL_TASK_MAX_METER_COUNT 16
//��������¼������288 ������24
#define NORMAL_TASK_MAX_DEEP 288 
#else
#define NORMAL_TASK_MAX_METER_COUNT MAX_PN_NUM
//��������¼��������24
#define NORMAL_TASK_MAX_DEEP 6 
#endif
#define NORMAL_TASK_FILE_MAXDEEP (NORMAL_TASK_MAX_METER_COUNT * NORMAL_TASK_MAX_DEEP)

#define LATELY_EVENT_MAX_CNT		(10)

u32 checkFile(u32 FileName);


ResultEnum NAND_ReadPara(PARA_ENUM ParaID, u32 Pn, u32 DataID, u8* ReadData, u32* DataLen);
ResultEnum NAND_WritePara(PARA_ENUM ParaID, u32 Pn, u32 DataID, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadMeasurePoint(u32 Pn, u32 DataID, u8* ReadData, u32* DataLen);
ResultEnum NAND_WriteMeasurePoint(u32 Pn, u32 DataID, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadMeterAMRData(AMR_ENUM AmrEnum, u8* addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen);

ResultEnum NAND_WriteMeterAMRData (AMR_ENUM AmrEnum, u8* addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadStatisticsData(u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen);

ResultEnum NAND_WriteStatisticsData(u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadAlarmRecord(u8* Addr, u32 DataID, RealTimeYYStruct* StartTime, RealTimeYYStruct* EndTime, u8* ReadData, u32* DataLen);

ResultEnum NAND_ReadAlarmRecordByLatelyNum(u32 LatelyNum, AlarmEventFileHeadStruct* AlarmHead, u8* ReadData, u32* DataLen, u32* OutFileName);

u32 NAND_GetAlarmRecordNum(void);

ResultEnum NAND_WriteAlarmRecordReportState(AlarmEventFileHeadStruct* AlarmHead, u32 FileName, u32 ReportDst, u32 ReportState);

ResultEnum NAND_ReadEventRecordByLatelyNum(u32 DataID, u32 LatelyNum, AlarmEventFileHeadStruct* EventHead, u8* ReadData, u32* DataLen);

u32 NAND_GetEventRecordNum(u32 DataID);

ResultEnum NAND_WriteAlarmRecord(u8* Addr, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* StartTime, RealTimeYYStruct* EndTime, u8* ReadData, u32* DataLen);

ResultEnum NAND_WriteEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadNormalTask(u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen);

ResultEnum NAND_WriteNormalTask(u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadRelayTask(u8 RelayType, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen);

ResultEnum NAND_WriteRelayTask(u8 RelayType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

void NAND_SyncMeasurePointToRam(void);

ResultEnum NAND_ReadMeasurePointByPn(u32 Pn, MeasurePointStruct *MeasurePoint);

ResultEnum NAND_ReadMeasurePointByPnNum(u32 PnNum, MeasurePointStruct *MeasurePoint);

ResultEnum NAND_ReadMeterCurveData(AMR_ENUM AmrEnum, u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen);

ResultEnum NAND_WriteMeterCurveData(AMR_ENUM AmrEnum, u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

ResultEnum NAND_ReadWriteTerminalExeFile(u32 PORTn, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen);

void NAND_TerminalExeFileUpdate(void);

ResultEnum SaveTerminalPowerDownFreezData(AMR_ENUM AmrEnum,RealTimeStruct* RealTime);



























#endif

