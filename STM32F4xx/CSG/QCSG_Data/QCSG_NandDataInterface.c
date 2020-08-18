
/*************************************** (C) COPYRIGHT 2017 CHINT ***************************************
* File Name          : QCSG_NandDataInterface.c
* Author             : zhaoyun
* Version            : V1.0.0
* Date               : 2017/06/26
* Description        : This file Contains the nand flash read and write interface 
********************************************************************************************************/

#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../Device/RTC.h"
#include "../Device/MEMRW.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include <string.h>
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"

u32 getWriteAMRFileName(AMR_ENUM DataType, RealTimeYYStruct * Time);
u32 checkFile(u32 FileName);
u32 getReadAMRFileName(AMR_ENUM DataType, RealTimeYYStruct* Time);
u32 getWriteAlarmEventFileName(AMR_ENUM DataType);
u32 getTaskFileName(AMR_ENUM DataType,u32 DataID,u32 oldFile);
u32 cmpDateTime(AMR_ENUM dataType, u8* srcTime, RealTimeYYStruct *Time);
u32 NAND_CmpDateTimeRange(RealTimeYYStruct *curTime,RealTimeYYStruct *StartTime,RealTimeYYStruct *EndTime);
//void realTimeToYYTime(RealTimeStruct *srcTime,RealTimeYYStruct *dstTime);
//void YYTimeTorealTime(RealTimeYYStruct *srcTime,RealTimeStruct *dstTime);
u32 isTaskDataTime(AMR_ENUM dataType,RealTimeStruct *baseTime,RealTimeStruct *dataTime,RealTimeStruct *readTime,u8 period,u8 periodUnit );
//void clearDailyFreeData(void);
//void testReadAMRData(void);
//void testWriteAMRData(void);

const LyAMRFileNameRangeInfo constAMRFileDef[LyAMRFileNameRangeInfoLen] = {
    {AMR_DailyFreeze,			DAILY_FREEZE_FILE_START,	DAILY_FREEZE_FILE_END, 		DAILY_FREEZE_FILE_NUM},
    {AMR_DailyStatisticFreeze, 	DAILY_STATISTIC_FILE_START, DAILY_STATISTIC_FILE_END, 	DAILY_STATISTIC_FILE_NUM},
    {AMR_MonthFreeze, 			MONTH_FREEZE_FILE_START, 	MONTH_FREEZE_FILE_END, 		MONTH_FREEZE_FILE_NUM},
    {AMR_MonthStatisticFreeze,	MONTH_STATISTIC_FILE_START, MONTH_STATISTIC_FILE_END, 	MONTH_STATISTIC_FILE_NUM},
    {AMR_CurveFreeze, 			CURVE_FREEZE_FILE_START,	CURVE_FREEZE_FILE_END, 		CURVE_FREEZE_FILE_NUM},
    {AMR_NormalTask, 			NORMAL_TASK_FILE_START,		NORMAL_TASK_FILE_END, 		NORMAL_TASK_FILE_NUM},
    {AMR_RelayTask, 			RELAY_TASK_FILE_START,		RELAY_TASK_FILE_END, 		RELAY_TASK_FILE_NUM},
    {AMR_ALARM, 				ALARM_FILE_START,			ALARM_FILE_END, 			ALARM_FILE_NUM},
    {AMR_EVENT, 				EVENT_FILE_START,			EVENT_FILE_END, 			EVENT_FILE_NUM}
  };
const LyAMRFileContentDefinition constAMRFileContentDefinition[LyAMRFileContentDefinitionLen]={
//�������ݣ�
{0x0001FF00,	0x0001FF00,	20,		1,	4,	3,	0,	4,		1},
{0x0002FF00,	0x0002FF00,	20,		1,	4,	3,	0,	4,		1},
{0x0003FF00,	0x0003FF00,	20,		1,	4,	3,	0,	4,		1},
{0x0004FF00,	0x0004FF00,	20,		1,	4,	3,	0,	4,		1},
{0x0201FF00,	0x0201FF00,	18,		1,	2,	3,	1,	9,		0},
{0x0202FF00,	0x0202FF00,	18,		1,	3,	3,	1,	6,		0},
{0x0203FF00,	0x0203FF00,	12,		1,	3,	3,	0,	3,		0},
{0x0204FF00,	0x0204FF00,	12,		1,	3,	3,	0,	3,		0},
{0x0206FF00,	0x0206FF00,	8,		1,	2,	3,	0,	3,		0},
{0x02800001,	0x02800001,	3,		0,	0,	0,	0,	0,		0},
{0xE1800015,	0xE1800015, 1,		0,	0,	0,	0,	0,		0},//err20181122 ������������ �ź�ǿ��


//�ն��᳭������
{0x050600FF, 	0x050600FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050601FF, 	0x050601FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050602FF,	0x050602FF, 	20,		1,	4,	4,	0,	4, 		1},
{0x050603FF, 	0x050603FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050604FF,	0x050604FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050605FF,	0x050605FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050606FF,	0x050606FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050607FF,	0x050607FF,		20,		1,	4,	4,	0,	4, 		1},
{0x050608FF,	0x050608FF,   	20,		1,	4,	4,	0,	4, 		1},
{0x050609FF,	0x050609FF,   	40,		1,	8,	4,	0,	4, 		1},
{0x05060AFF,	0x05060AFF,   	40,		1,	8,	4,	0,	4, 		1},
{0x05060BFF,	0x05060BFF,   	40,		1,	8,	4,	0,	4, 		1},
{0x05060CFF,	0x05060CFF,   	40,		1,	8,	4,	0,	4, 		1},

//�ն���ͳ������ 
{0xE1008010,	0xE1008010,	27,		0,	0,	0,	0,	0,		0},
{0xE1008011,	0xE1008011,	27,		0,	0,	0,	0,	0,		0},
{0xE1008012,	0xE1008012,	27,		0,	0,	0,	0,	0,		0},
{0xE1008013,	0xE1008013,	27,		0,	0,	0,	0,	0,		0},
{0xE1008014,	0xE1008014,	24,		0,	0,	0,	0,	0,		0},
{0xE1008015,	0xE1008015,	14,		0,	0,	0,	0,	0,		0},
{0xE1008016,	0xE1008016,	14,		0,	0,	0,	0,	0,		0},
{0xE1008017,	0xE1008017,	14,		0,	0,	0,	0,	0,		0},
{0xE1008018,	0xE1008018,	14,		0,	0,	0,	0,	0,		0},
{0xE1008019,	0xE1008019,	14,		0,	0,	0,	0,	0,		0},
{0xE100801A,	0xE100801A,	12,		0,	0,	0,	0,	0,		0},
{0xE100801B,	0xE100801B,	14,		0,	0,	0,	0,	0,		0},
{0xE100801C,	0xE100801C,	14,		0,	0,	0,	0,	0,		0},
{0xE100801D,	0xE100801D,	14,		0,	0,	0,	0,	0,		0},
{0xE1008020,	0xE1008020,	16,		0,	0,	0,	0,	0,		0},
{0xE1008021,	0xE1008021,	26,		0,	0,	0,	0,	0,		0},
{0xE1008022,	0xE1008022,	26,		0,	0,	0,	0,	0,		0},
{0xE1008023,	0xE1008023,	26,		0,	0,	0,	0,	0,		0},
{0xE1008024,	0xE1008024,	26,		0,	0,	0,	0,	0,		0},
{0xE1008025,	0xE1008025,	22,		0,	0,	0,	0,	0,		0},
{0xE1008026,	0xE1008026,	22,		0,	0,	0,	0,	0,		0},
{0xE1008027,	0xE1008027,	22,		0,	0,	0,	0,	0,		0},
{0xE1008028,	0xE1008028,	24,		0,	0,	0,	0,	0,		0},
{0xE1008029,	0xE1008029,	24,		0,	0,	0,	0,	0,		0},
{0xE100802A,	0xE100802A,	24,		0,	0,	0,	0,	0,		0},
{0xE1008030,	0xE1008030,	3,		0,	0,	0,	0,	0,		0},
{0xE1008031,	0xE1008031,	3,		0,	0,	0,	0,	0,		0},
{0xE1008040,	0xE1008040,	78,		0,	0,	0,	0,	0,		0},
{0xE1008041,	0xE1008041,	40,		0,	0,	0,	0,	0,		0},
{0xE1008042,	0xE1008042,	40,		0,	0,	0,	0,	0,		0},
{0xE1008043,	0xE1008043,	33,		0,	0,	0,	0,	0,		0},
{0xE1008044,	0xE1008044,	33,		0,	0,	0,	0,	0,		0},
{0xE1008045,	0xE1008045,	33,		0,	0,	0,	0,	0,		0},
{0xE1008046,	0xE1008046,	33,		0,	0,	0,	0,	0,		0},
{0xE10081FF,	0xE10081FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10082FF,	0xE10082FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10083FF,	0xE10083FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10084FF,	0xE10084FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10085FF,	0xE10085FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10086FF,	0xE10086FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10087FF,	0xE10087FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10088FF,	0xE10088FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10089FF,	0xE10089FF,	42,		1,	2,	4,	0,	21,		0},
{0xE1008AFF,	0xE1008AFF,	42,		1,	2,	4,	0,	21,		0},
{0xE1008BFF,	0xE1008BFF,	42,		1,	2,	4,	0,	21,		0},
{0xE1008CFF,	0xE1008CFF,	42,		1,	2,	4,	0,	21,		0},
{0xE10090FF,	0xE10090FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10091FF,	0xE10091FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10092FF,	0xE10092FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10093FF,	0xE10093FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10094FF,	0xE10094FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10095FF,	0xE10095FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10096FF,	0xE10096FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10097FF,	0xE10097FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10098FF,	0xE10098FF,	42,		1,	2,	4,	0,	21,		0},
{0xE10099FF,	0xE10099FF,	42,		1,	2,	4,	0,	21,		0},
{0xE1009AFF,	0xE1009AFF,	42,		1,	2,	4,	0,	21,		0},
{0xE1009BFF,	0xE1009BFF,	42,		1,	2,	4,	0,	21,		0},
{0xE1009C0F,	0xE1009C0F,	64,		1,	4,	4,	0,	15,		0},
{0xE1009C1F,	0xE1009C1F,	64,		1,	4,	4,	16,	31,		0},
{0xE1009C20,	0xE1009C20,	4,		0,	0,	0,	0,	0,		0},


//�¶��᳭������
{0x0000FF01,	0x050600FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0001FF01,	0x050601FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0002FF01,	0x050602FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0003FF01,	0x050603FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0004FF01,	0x050604FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0005FF01,	0x050605FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0006FF01,	0x050606FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0007FF01,	0x050607FF,   	20,		1,	4,	3,	0,	4, 		1},
{0x0008FF01,	0x050608FF,   	20,		1,	4,	3,	0,	4, 		1},
//{0x0009FF01,	0x050609FF,   	40,		1,	8,	3,	0,	4, 		1},
//{0x000AFF01,	0x05060AFF,		40,		1,	8,	3,	0,	4, 		1},
{0x0101FF01,	0x050609FF, 	40,		1,	8,	3,	0,	4,		1},
{0x0102FF01,	0x05060AFF,	40,		1,	8,	3,	0,	4,		1},
{0x0103FF01,	0x0103FF01,	40,		1,	8,	3,	0,	4,		1},
{0x0104FF01,	0x0104FF01,	40,		1,	8,	3,	0,	4,		1},
{0x0105FF01,	0x0105FF01,	40,		1,	8,	3,	0,	4,		1},
{0x0106FF01,	0x0106FF01,	40,		1,	8,	3,	0,	4,		1},
{0x0107FF01,	0x0107FF01,	40,		1,	8,	3,	0,	4,		1},
{0x0108FF01,	0x0108FF01,	40,		1,	8,	3,	0,	4,		1},
{0x0109FF01,	0x0109FF01,	40,		1,	8,	3,	0,	4,		1},
{0x010AFF01,	0x010AFF01,	40,		1,	8,	3,	0,	4,		1},
//�¶���ͳ������
{0xE100C010,	0xE100C010,	27,		0,	0,	0,	0,	0,		0},
{0xE100C011,	0xE100C011,	27,		0,	0,	0,	0,	0,		0},
{0xE100C012,	0xE100C012,	27,		0,	0,	0,	0,	0,		0},
{0xE100C013,	0xE100C013,	27,		0,	0,	0,	0,	0,		0},
{0xE100C015,	0xE100C015,	14,		0,	0,	0,	0,	0,		0},
{0xE100C016,	0xE100C016,	14,		0,	0,	0,	0,	0,		0},
{0xE100C017,	0xE100C017,	14,		0,	0,	0,	0,	0,		0},
{0xE100C018,	0xE100C018,	14,		0,	0,	0,	0,	0,		0},
{0xE100C019,	0xE100C019,	14,		0,	0,	0,	0,	0,		0},
{0xE100C01A,	0xE100C01A,	12,		0,	0,	0,	0,	0,		0},
{0xE100C01D,	0xE100C01D,	14,		0,	0,	0,	0,	0,		0},
{0xE100C030,	0xE100C030,	3,		0,	0,	0,	0,	0,		0},
{0xE100C031,	0xE100C031,	3,		0,	0,	0,	0,	0,		0},
{0xE100C043,	0xE100C043,	33,		0,	0,	0,	0,	0,		0},
{0xE100C044,	0xE100C044,	33,		0,	0,	0,	0,	0,		0},
{0xE100C045,	0xE100C045,	33,		0,	0,	0,	0,	0,		0},
{0xE100C046,	0xE100C046,	33,		0,	0,	0,	0,	0,		0}
};
const u32 constGroupMask[QCSG_DI_SIZE + 1]={0xFFFFFFFF,0x00FFFFFF,0xFF00FFFF,0xFFFF00FF,0xFFFFFF00};

ResultEnum NAND_ReadPara(PARA_ENUM ParaID, u32 Pn, u32 DataID, u8* ReadData, u32* DataLen)
{
	ResultEnum Result = NO_ERROR;
	u32 FlashAddr = 0, ReadLen = 0;
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	if(Pn != 0) return ERROR_InvalidData;
	switch(ParaID)
	{
		case PARA_Communication:
			FlashAddr = ADDR_CommunicationPara;
			ReadLen = sizeof(CommunicationParaStruct);
			break;
		case PARA_RealTime:			
			//RX8025T_Read((u32)pComm_Ram->RTCBuff, 0, 7);
			ReadData[0]	= pComm_Ram->RTCBuff[0];
			ReadData[1]	= pComm_Ram->RTCBuff[1];
			ReadData[2]	= pComm_Ram->RTCBuff[2];
			ReadData[3]	= pComm_Ram->RTCBuff[4];
			ReadData[4]	= pComm_Ram->RTCBuff[5];
			ReadData[5]	= pComm_Ram->RTCBuff[6];
			DataLen[0] = 6;
			break;
		case PARA_AlarmEventMask:
			FlashAddr = ADDR_AlarmEventMaskPara;
			ReadLen = sizeof(AlarmEventMaskParaStruct);
			break;
		case PARA_SettlementDate:
			FlashAddr = ADDR_SettlementDatePara;
			ReadLen = sizeof(SettleDateParaStruct);
			break;
		case PARA_IDAuthentication:
			FlashAddr = ADDR_IDAuthenticationPara;
			ReadLen = sizeof(IDAuthenticationParaStruct);
			break;
		case PARA_Threshold:
			FlashAddr = ADDR_ThresholdPara;
			ReadLen = sizeof(ThresholdParaStruct);
			break;
		case PARA_AMR:
			switch(DataID)
			{
				case 0xE0000200:
					FlashAddr = ADDR_AMRChTimeTbl;
					ReadLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000201:
					FlashAddr = ADDR_AMRChTimeTbl + sizeof(AMRChTimeStruct);
					ReadLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000202:
					FlashAddr = ADDR_AMRChTimeTbl + (2 * sizeof(AMRChTimeStruct));
					ReadLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE000021F:
					FlashAddr = ADDR_AMRChTimeTbl + (3 * sizeof(AMRChTimeStruct));
					ReadLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000220:
					FlashAddr = ADDR_AMRChTimeTbl + (4 * sizeof(AMRChTimeStruct));
					ReadLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000230:
					FlashAddr = ADDR_AMRChSetTbl;
					ReadLen = sizeof(AMRChSetStruct);
					break;
				case 0xE0000231:
					FlashAddr = ADDR_AMRChSetTbl + sizeof(AMRChSetStruct);
					ReadLen = sizeof(AMRChSetStruct);
					break;
				case 0xE0000232:
					FlashAddr = ADDR_AMRChSetTbl + (2 * sizeof(AMRChSetStruct));
					ReadLen = sizeof(AMRChSetStruct);
					break;
				case 0xE000024F:
					FlashAddr = ADDR_AMRChSetTbl + (3 * sizeof(AMRChSetStruct));
					ReadLen = sizeof(AMRChSetStruct);
					break;
				case 0xE0000250:
					FlashAddr = ADDR_AMRChSetTbl + (4 * sizeof(AMRChSetStruct));
					ReadLen = sizeof(AMRChSetStruct);
					break;
				default:
					Result = ERROR_WRONG_ACCESS;
					break;
			}
			break;
		case PARA_Task:
			if((DataID >= 0xE0000301) && (DataID <= 0xE00003FE))
			{
				FlashAddr = ADDR_NormalTaskPara + (((DataID - 1) & 0x000000FF)  * sizeof(NormalTaskParaStruct));
				ReadLen = sizeof(NormalTaskParaStruct);
			}
			else if((DataID >= 0xE0000401) && (DataID <= 0xE00004FE))
			{
				FlashAddr = ADDR_RelayTaskPara + (((DataID - 1) & 0x000000FF)  * sizeof(RelayTaskParaStruct));
				ReadLen = sizeof(RelayTaskParaStruct);
			}
			else
			{
				Result = ERROR_WRONG_ACCESS;
			}			
			break;
		case PARA_Caculation:			
			if((DataID == 0xE0000900) || (DataID == 0xE0000500) || (DataID == 0xE0000501) ||
			   (DataID == 0xE0000502) || (DataID == 0xE00005FE) || (DataID == 0xE00005FF))
			{
				FlashAddr = ADDR_AnalogPara;
				ReadLen = sizeof(AnalogCaculationParaStruct);
			}
			else if((DataID == 0xE0000600) || (DataID == 0xE0000601) || (DataID == 0xE0000602) ||
			        (DataID == 0xE00006FE) || (DataID == 0xE00006FF))
			{
				FlashAddr = ADDR_PulsePara;
				ReadLen = sizeof(PulseCaculationParaStruct);
			}
			else if(((DataID >= 0xE0000700) && (DataID <= 0xE0000710)) || 
				    (DataID == 0xE00007FE) || (DataID == 0xE00007FF))
			{
				FlashAddr = ADDR_CaculationPara;
				ReadLen = sizeof(CaculationParaStruct);
			}
			else if(((DataID >= 0xE0000800) && (DataID <= 0xE0000810)) || 
				    (DataID == 0xE00008FE) || (DataID == 0xE00008FF))
			{
				FlashAddr = ADDR_DiffPara;
				ReadLen = sizeof(DiffCaculationParaStruct);
			}
			else
			{
				Result = ERROR_WRONG_ACCESS;
			}
			break;
		case PARA_RS485Port:
			if(DataID == 0xE0000A00)
			{
				FlashAddr = ADDR_RS485PortParaTbl;
				ReadLen = sizeof(RS485PortParaStruct);
			}
			else if(DataID == 0xE0000A01)
			{
				FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
				ReadLen = sizeof(RS485PortParaStruct);
			}
			else if(DataID == 0xE0000A02)
			{
				FlashAddr = ADDR_RS485PortParaTbl + (2 *sizeof(RS485PortParaStruct));
				ReadLen = sizeof(RS485PortParaStruct);
			}
			else
			{
				Result = ERROR_WRONG_ACCESS;
			}			
			break;
		case PARA_Attachment:
			FlashAddr = ADDR_AttachmentPara;
			ReadLen = sizeof(AttachmentParaStruct);
			break;
		case PARA_Version:
			MemCopy(ReadData, (u8*)&DefaultVersionPara, sizeof(VersionParaStruct));
			DataLen[0] = sizeof(VersionParaStruct);
			break;
		case PARA_LoadManagement:
			FlashAddr = ADDR_LoadManagementPara;
			ReadLen = sizeof(LoadManagementParaStruct);
			break;
		
		case PARA_PowerQuality:
			//Result = ERROR_WRONG_ACCESS;
			//break;
		case PARA_LinkInterface:
		case PARA_MeterPointControl:
		case PARA_UserDefine:
		case PARA_Reset:
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
		
	}
	if((FlashAddr != 0) && (ReadLen != 0))
	{
		MR((u32)ReadData, FlashAddr, ReadLen);
		DataLen[0] = ReadLen;
	}
	
	return Result;
}

ResultEnum NAND_WritePara(PARA_ENUM ParaID, u32 Pn, u32 DataID, u8* WriteData, u32* DataLen)
{
	ResultEnum Result = NO_ERROR;
	u32 FlashAddr = 0, WriteLen = 0;
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	if(Pn != 0) return ERROR_VerifyFail;
	switch(ParaID)
	{
		case PARA_Communication:
			FlashAddr = ADDR_CommunicationPara;
			WriteLen = sizeof(CommunicationParaStruct);
			break;
		case PARA_RealTime:	
			pComm_Ram->RTCBuff[0] = WriteData[0];
			pComm_Ram->MYMDHMS[0] = WriteData[0];
			
			pComm_Ram->RTCBuff[1] = WriteData[1];
			pComm_Ram->MYMDHMS[1] = WriteData[1];
			
			pComm_Ram->RTCBuff[2] = WriteData[2];
			pComm_Ram->MYMDHMS[2] = WriteData[2];
			
			pComm_Ram->RTCBuff[4] = WriteData[3];
			pComm_Ram->MYMDHMS[3] = WriteData[3];
			
			pComm_Ram->RTCBuff[5] = WriteData[4];
			pComm_Ram->MYMDHMS[4] = WriteData[4];
			
			pComm_Ram->RTCBuff[6] = WriteData[5];
			pComm_Ram->MYMDHMS[5] = WriteData[5];
			
			pComm_Ram->MYMDHMS[6] = CaculateWeek((RealTimeStruct*)pComm_Ram->MYMDHMS);
			//pComm_Ram->RTCBuff[3] = 1 << pComm_Ram->MYMDHMS[6];
			pComm_Ram->RTCBuff[3] = pComm_Ram->MYMDHMS[6];
			Result = TimeValidCheck((RealTimeStruct*)pComm_Ram->MYMDHMS);
			if(Result == NO_ERROR) 
			{
				WriteRTC((u32)pComm_Ram->RTCBuff);
			}		
			break;
		case PARA_AlarmEventMask:
			FlashAddr = ADDR_AlarmEventMaskPara;
			WriteLen = sizeof(AlarmEventMaskParaStruct);
			break;
		case PARA_SettlementDate:
			FlashAddr = ADDR_SettlementDatePara;
			WriteLen = sizeof(SettleDateParaStruct);
			break;
		case PARA_IDAuthentication:
			FlashAddr = ADDR_IDAuthenticationPara;
			WriteLen = sizeof(IDAuthenticationParaStruct);
			break;
		case PARA_Threshold:
			FlashAddr = ADDR_ThresholdPara;
			WriteLen = sizeof(ThresholdParaStruct);
			break;
		case PARA_AMR:
			switch(DataID)
			{
				case 0xE0000200:
					FlashAddr = ADDR_AMRChTimeTbl;
					WriteLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000201:
					FlashAddr = ADDR_AMRChTimeTbl + sizeof(AMRChTimeStruct);
					WriteLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000202:
					FlashAddr = ADDR_AMRChTimeTbl + (2 * sizeof(AMRChTimeStruct));
					WriteLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE000021F:
					FlashAddr = ADDR_AMRChTimeTbl + (3 * sizeof(AMRChTimeStruct));
					WriteLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000220:
					FlashAddr = ADDR_AMRChTimeTbl + (4 * sizeof(AMRChTimeStruct));
					WriteLen = sizeof(AMRChTimeStruct);
					break;
				case 0xE0000230:
					FlashAddr = ADDR_AMRChSetTbl;
					WriteLen = sizeof(AMRChSetStruct);
					break;
				case 0xE0000231:
					FlashAddr = ADDR_AMRChSetTbl + sizeof(AMRChSetStruct);
					WriteLen = sizeof(AMRChSetStruct);
					break;
				case 0xE0000232:
					FlashAddr = ADDR_AMRChSetTbl + (2 * sizeof(AMRChSetStruct));
					WriteLen = sizeof(AMRChSetStruct);
					break;
				case 0xE000024F:
					FlashAddr = ADDR_AMRChSetTbl + (3 * sizeof(AMRChSetStruct));
					WriteLen = sizeof(AMRChSetStruct);
					break;
				case 0xE0000250:
					FlashAddr = ADDR_AMRChSetTbl + (4 * sizeof(AMRChSetStruct));
					WriteLen = sizeof(AMRChSetStruct);
					break;
				default:
					Result = ERROR_NoSuchDataID;
					break;
			}
			break;
		case PARA_Task:
			if((DataID >= 0xE0000301) && (DataID <= 0xE00003FE))
			{
				FlashAddr = ADDR_NormalTaskPara + ((DataID & 0x000000FF)  * sizeof(NormalTaskParaStruct));
				WriteLen = sizeof(NormalTaskParaStruct);
			}
			else if((DataID >= 0xE0000401) && (DataID <= 0xE00004FE))
			{
				FlashAddr = ADDR_RelayTaskPara + ((DataID & 0x000000FF)  * sizeof(RelayTaskParaStruct));
				WriteLen = sizeof(RelayTaskParaStruct);
			}
			else
			{
				Result = ERROR_NoSuchDataID;
			}			
			break;
		case PARA_Caculation:			
			if((DataID == 0xE0000900) || (DataID == 0xE0000500) || (DataID == 0xE0000501) ||
			   (DataID == 0xE0000502) || (DataID == 0xE00005FE) || (DataID == 0xE00005FF))
			{
				FlashAddr = ADDR_AnalogPara;
				WriteLen = sizeof(AnalogCaculationParaStruct);
			}
			else if((DataID == 0xE0000600) || (DataID == 0xE0000601) || (DataID == 0xE0000602) ||
			        (DataID == 0xE00006FE) || (DataID == 0xE00006FF))
			{
				FlashAddr = ADDR_PulsePara;
				WriteLen = sizeof(PulseCaculationParaStruct);
			}
			else if(((DataID >= 0xE0000700) && (DataID <= 0xE0000708)) || 
				    (DataID == 0xE00007FE) || (DataID == 0xE00007FF))
			{
				FlashAddr = ADDR_CaculationPara;
				WriteLen = sizeof(CaculationParaStruct);
			}
			else if(((DataID >= 0xE0000800) && (DataID <= 0xE0000808)) || 
				    (DataID == 0xE00008FE) || (DataID == 0xE00008FF))
			{
				FlashAddr = ADDR_DiffPara;
				WriteLen = sizeof(DiffCaculationParaStruct);
			}
			else
			{
				Result = ERROR_NoSuchDataID;
			}
			break;
		case PARA_RS485Port:
			if(DataID == 0xE0000A00)
			{
				FlashAddr = ADDR_RS485PortParaTbl;
				WriteLen = sizeof(RS485PortParaStruct);
			}
			else if(DataID == 0xE0000A01)
			{
				FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
				WriteLen = sizeof(RS485PortParaStruct);
			}
			else if(DataID == 0xE0000A02)
			{
				FlashAddr = ADDR_RS485PortParaTbl + (2 *sizeof(RS485PortParaStruct));
				WriteLen = sizeof(RS485PortParaStruct);
			}
			else
			{
				Result = ERROR_NoSuchDataID;
			}			
			break;
		case PARA_Attachment:
			FlashAddr = ADDR_AttachmentPara;
			WriteLen = sizeof(AttachmentParaStruct);
			break;
		case PARA_LoadManagement:
			FlashAddr = ADDR_LoadManagementPara;
			WriteLen = sizeof(LoadManagementParaStruct);
			break;
		case PARA_MeterPointControl:
			FlashAddr = ADDR_MeterPointCtrlPara;
			WriteLen = sizeof(MeterPointCtrlParaStruct);
			break;
		case PARA_Reset:
			
			break;
		case PARA_Version:
		case PARA_PowerQuality:
		case PARA_LinkInterface:
		case PARA_UserDefine:
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
		
	}
	if((FlashAddr != 0) && (WriteLen != 0))
	{
		MW((u32)WriteData, FlashAddr, WriteLen);
	}
	
	return Result;
}

ResultEnum NAND_ReadMeasurePointByPn(u32 Pn, MeasurePointStruct *MeasurePoint)
{
    u32 readLen=0;
    ResultEnum Result = NAND_ReadMeasurePoint(Pn, 0xE080000F, (u8*) MeasurePoint, &readLen);
    return Result;
}

ResultEnum NAND_ReadMeasurePointByPnNum(u32 PnNum, MeasurePointStruct *MeasurePoint)
{
    u32 readLen=0;
    ResultEnum Result = NAND_ReadMeasurePoint(SwitchNumToPn(PnNum), 0xE080000F, (u8*) MeasurePoint, &readLen);
    return Result;
}

ResultEnum NAND_ReadMeasurePoint(u32 Pn, u32 DataID, u8* ReadData, u32* DataLen)
{
	ResultEnum Result = NO_ERROR;
	u32 FlashAddr = 0, ReadLen = 0, PnNum = 0;
	PnNum = SwitchPnToNum(Pn);
	if(PnNum >= MAX_PN_NUM) return ERROR_DestAddrNotExist;
	switch(DataID)
	{
		case 0xE0800000:
		case 0xE0800001:
		case 0xE0800002:
		case 0xE0800003:
		case 0xE0800004:
		case 0xE0800005:
		case 0xE0800006:
		case 0xE0800007:
		case 0xE0800008:
		case 0xE0800009:
		case 0xE080000A:
		case 0xE080000B:
		case 0xE080000C:
		case 0xE080000D:
		case 0xE080000F:
		case 0xE0800010:
		case 0xE0800011:
		case 0xE0800012:
		case 0xE0800013:
		case 0xE0800014:
		case 0xE080001F:
		case 0xE0800021:
		case 0xE0800022:
		case 0xE080002F:			
			FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
			ReadLen = sizeof(MeasurePointStruct);
			break;
		//��������������	
		case 0xE0800100:		//������ƽ�����ֵ��
		case 0xE0800101:
		case 0xE0800102:
		case 0xE0800103:
		case 0xE0800104:
		case 0xE0800105:
		case 0xE0800106:
		case 0xE0800107:
		case 0xE0800108:
		case 0xE0800109:
		case 0xE080010A:
		case 0xE080010B:
		case 0xE080010C:
		case 0xE080010F:
		case 0xE0800200:
		case 0xE0800201:
		case 0xE0800202:
		case 0xE0800203:
		//case 0xE0800204:
		case 0xE0800205:
		case 0xE0800206:
		case 0xE0800207:
		case 0xE0800208:
		case 0xE0800209:
		case 0xE080020A:
		case 0xE080020B:
		case 0xE080020C:
		case 0xE080020D:
		case 0xE080020E:
			FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
			FlashAddr += sizeof(MeasurePointStruct);
			ReadLen = sizeof(PowerQualityParaStruct);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;
			
	}

	if((FlashAddr != 0) && (ReadLen != 0))
	{
		MR((u32)ReadData, FlashAddr, ReadLen);
		DataLen[0] = ReadLen;
	}
	return Result;
}

ResultEnum NAND_WriteMeasurePoint(u32 Pn, u32 DataID, u8* WriteData, u32* DataLen)
{
	ResultEnum Result = NO_ERROR;
	u32 FlashAddr = 0, WriteLen = 0, PnNum = 0;
    MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	PnNum = SwitchPnToNum(Pn);
	if(PnNum >= MAX_PN_NUM) return ERROR_DestAddrNotExist;
	switch(DataID)
	{
		case 0xE0800000:
		case 0xE0800001:
		case 0xE0800002:
		case 0xE0800003:
		case 0xE0800004:
		case 0xE0800005:
		case 0xE0800006:
		case 0xE0800007:
		case 0xE0800008:
		case 0xE0800009:
		case 0xE080000A:
		case 0xE080000B:
		case 0xE080000C:
		case 0xE080000D:
		case 0xE080000F:
		case 0xE0800010:
		case 0xE0800011:
		case 0xE0800012:
		case 0xE0800013:
		case 0xE0800014:
		case 0xE080001F:
		case 0xE0800021:
		case 0xE0800022:
		case 0xE080002F:			
			FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
			WriteLen = sizeof(MeasurePointStruct);
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)	
				if(PnNum == 0)
				{
					SetACSampleAddr(&WriteData[2]);
				}
			#endif
			break;
		case 0xE0800100:
		case 0xE0800101:
		case 0xE0800102:
		case 0xE0800103:
		case 0xE0800104:
		case 0xE0800105:
		case 0xE0800106:
		case 0xE0800107:
		case 0xE0800108:
		case 0xE0800109:
		case 0xE080010A:
		case 0xE080010B:
		case 0xE080010C:
		case 0xE080010F:
		case 0xE0800200:
		case 0xE0800201:
		case 0xE0800202:
		case 0xE0800203:
		//case 0xE0800204:
		case 0xE0800205:
		case 0xE0800206:
		case 0xE0800207:
		case 0xE0800208:
		case 0xE0800209:
		case 0xE080020A:
		case 0xE080020B:
		case 0xE080020C:
		case 0xE080020D:
		case 0xE080020E:
			FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
			FlashAddr += sizeof(MeasurePointStruct);
			WriteLen = sizeof(PowerQualityParaStruct);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;
			
	}

	if((FlashAddr != 0) && (WriteLen != 0))
	{
		MW((u32)WriteData, FlashAddr, WriteLen);
        MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
		pTerminalRam->MeterSyncToRam_S_Timer = METER_FILE_SYNC_DELAY_TIME;
	}
	return Result;
}

/**
 *  @brief    �������ݶ�ȡ����
 *  
 *  @param  AmrEnum �������ͣ�ö���ͣ�������Ҫ�洢����������һ������
 *  @param  addr ����ַ
 *  @param  PointMeterType���ͺţ��ݱ�������
 *  @param DataID ���ݱ�ʶ
 *  @param Time����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param ReadData ��ȡ�����ݻ�����
 *  @param DataLen ��ȡ�ɹ����ֽ���
 *  @return  ���ݶ�ȡ�ɹ�������NO_ERROR���������ش������
 *
 *  @note    
 *
//����:
//1.���ݱ�ʶ���û�ж��壬�򷵻ش��󣬲�����0�ֽ�
//2.���ڿռ�ԭ�����ݴ洢ʱ��ѹ����ʽ�洢��û��Ԥ���ռ䡣����д����ʱȫ�������ݿ�д�롣
//3.���ڵ�2�㣬�����ݵ�ʱ�����������ǿ����ݣ��򵥷��ؿ����ݣ����������ǿ������е�����һ����ʶ������Ҫ����
//4.����ԭ����Ϊ���ݿ��е�ÿ�����ݶ���һ�����ȡ�
*/
ResultEnum NAND_ReadMeterAMRData(AMR_ENUM AmrEnum, u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen)
{
	AMRFileHeadStruct* pAMRFileHead;
	const LyAMRFileContentDefinition * pAMRFileContentDefinition;
	u8* pBuf;
	ms_Type *ms;
	
	ResultEnum Result = NO_ERROR;
	u32 FileName = 0, i = 0, DataPos = 0, DI = 0;
    u32 searchId = 0xFFFFFFFF;			//������ѯ�����ݱ�ʶ
    u32 realDataSize = 0;				//���շ��ص����ݳ���
    u32 tempLen = 0, realGroupSize = 0, groupSeqno = 0;
	msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
        
   	for(i = 0; i < LyAMRFileContentDefinitionLen; i++)
   	{	//�Ȳ������ȡ���ݱ�ʶ��Ӧ����Ϣ
        pAMRFileContentDefinition = &constAMRFileContentDefinition[i];
        if((pAMRFileContentDefinition->dataId & constGroupMask[pAMRFileContentDefinition->groupPos]) == 
		   (DataID & constGroupMask[pAMRFileContentDefinition->groupPos]))
        {//�ж����ݱ�ʶ�Ƿ�Ϸ�
            searchId = pAMRFileContentDefinition->searchId;
            if(pAMRFileContentDefinition->groupFlag == 1)
            {//�����������ʶ
                if(pAMRFileContentDefinition->dataId == DataID)
                {
                    realDataSize = pAMRFileContentDefinition->maxLen + pAMRFileContentDefinition->groupHeadSize;
					MC(0xFF, (u32)ReadData, realDataSize);
					if(pAMRFileContentDefinition->groupHeadSize == 1) ReadData[0] = pAMRFileContentDefinition->groupMax;
                }
                else
                {
                    realDataSize=pAMRFileContentDefinition->eachDataLen;
					MC(0xFF, (u32)ReadData, realDataSize);
                }
            }
            else
            {
                realDataSize = pAMRFileContentDefinition->maxLen;
				MC(0xFF, (u32)ReadData, realDataSize);
            }
            break;
        }
    }
    if(searchId == 0xFFFFFFFF)
    {//û���ѵ�
        *DataLen = realDataSize;
        return ERROR_NoSuchDataID;
    }
    
	MC(0, (u32)FileHead, sizeof(AMRFileHeadStruct));	
	msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
	FileName = getReadAMRFileName(AmrEnum, Time);
    if(FileName == 0xFFFFFFFF) 
    {
        *DataLen = realDataSize;
        return ERROR_WRONG_TIME;
    }
	ms = Get_ms();
	pBuf = (u8*)ADDR_128KDATABUFF;
	pAMRFileHead = (AMRFileHeadStruct*)ADDR_128KDATABUFF;
	//���ļ�ͷ��Ϣ���Ի�ȡ�����ݴ洢��ʼ��ַ�����ݳ���
	ms->msfile_read(FileName, 0, pBuf, msFile[FileName].NUM_DATA * sizeof(AMRFileHeadStruct));	
	for(i = 0; i < msFile[FileName].NUM_DATA; i++)
	{
		if(MemCompare((u8*)&pAMRFileHead[i], addr, METER_ADDR_SIZE) == 0) 
		{//����ҵ��ñ��ŵ����ݣ����ȱ����ļ�ͷ��Ϣ��Ȼ������ļ�ͷ�����ݶ���ȡ������
			MemCopy((u8*)FileHead, (u8*)&pAMRFileHead[i], sizeof(AMRFileHeadStruct));   
			ms->msfile_read(FileName, FileHead->OffSet, pBuf, FileHead->DataLen);
			break;
		}
	}
	if(i >= msFile[FileName].NUM_DATA)
    {
        *DataLen = realDataSize;
        return ERROR_DestAddrNotExist;
	}
        //��ȡ�ص��������ҵ�Ŀ������ÿ�����ݸ�ʽDI(4) L(2) data(x)
	for(DataPos = 0; DataPos < FileHead->DataLen; )
	{
		MemCopy((u8*)&DI, &pBuf[DataPos], QCSG_DI_SIZE);
		DataPos += QCSG_DI_SIZE;
		if(DI == searchId) break;	//�ҵ�Ŀ�����ݿ�
		MemCopy((u8*)&tempLen, &pBuf[DataPos], 2);
		DataPos += 2 + tempLen;
	}
    if(DataPos >= FileHead->DataLen)
    {//û���ҵ����ݿ�
        *DataLen = realDataSize;
        return ERROR_NoSuchDataID;
    }
    MemCopy((u8*)&tempLen, &pBuf[DataPos], 2);
    DataPos += 2;
    if(pAMRFileContentDefinition->dataId == DataID)
    {
       if((pAMRFileContentDefinition->groupFlag == 1) && (pAMRFileContentDefinition->groupHeadSize > 0))
       {
            realDataSize = tempLen;
            //�ȸ�������
            MemCopy(&ReadData[pAMRFileContentDefinition->groupHeadSize], &pBuf[DataPos], tempLen);
            //�ٲ���ͷ�ֽ�
            tempLen = tempLen / (pAMRFileContentDefinition->eachDataLen);
            tempLen = tempLen - 1;
            for(i = 0; i < pAMRFileContentDefinition->groupHeadSize; i++)
            {
                ReadData[i] = ((tempLen >> (8 * i)) & 0xff);
            }
            *DataLen = realDataSize + pAMRFileContentDefinition->groupHeadSize;
       }
       else
       {
            MemCopy(&ReadData[0], &pBuf[DataPos], tempLen);
            *DataLen = tempLen;
       }
    }
    else
    {//�������ڵ����ݱ�ʶ,�ж����ݱ�ʶ�Ƿ��ںϷ�����
        realGroupSize = tempLen /  (pAMRFileContentDefinition->eachDataLen) ;
        if(realGroupSize <= (pAMRFileContentDefinition->groupMax - pAMRFileContentDefinition->groupMin + 1))
        {//��������¶�������
            realGroupSize = pAMRFileContentDefinition->groupMin + realGroupSize ;
        }
        else
        {
            realGroupSize = pAMRFileContentDefinition->groupMax;
        }
        groupSeqno = ((~constGroupMask[pAMRFileContentDefinition->groupPos])&DataID) >> (8*(4-pAMRFileContentDefinition->groupPos));
        if((groupSeqno >= pAMRFileContentDefinition->groupMin) && (groupSeqno <= realGroupSize))
        {//�Ϸ�����Ѱַ
            DataPos += pAMRFileContentDefinition->eachDataLen * (groupSeqno - pAMRFileContentDefinition->groupMin);
            MemCopy(&ReadData[0], &pBuf[DataPos], pAMRFileContentDefinition->eachDataLen);
            *DataLen = pAMRFileContentDefinition->eachDataLen;
        }
        else
        {//�Ƿ�
            *DataLen = realDataSize;
            return ERROR_NoSuchDataID;
        }
    }
	return Result;
}

/**
 *  @brief    �������ݱ��溯��
 *  
 *  @param  AmrEnum �������ͣ�ö���ͣ�������Ҫ�洢����������һ������
 *  @param  addr ����ַ
 *  @param  PointMeterType ���ͺţ��ݱ�������
 *  @param DataID ���ݱ�ʶ���ݱ�������
 *  @param Time ����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param WriteData Ҫд������ݣ���ʽ: dataId(4) L(2) data(x)
 *  @param DataLen Ҫд������ݳ���
 *  @return  ����д��ɹ�������NO_ERROR���������ش������
 *
 *  @note    
 *
 */
ResultEnum NAND_WriteMeterAMRData (AMR_ENUM AmrEnum, u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	ResultEnum Result = NO_ERROR;
	u32 FileName = 0,deep;
    u32 i=0;
	ms_Type *ms;
	msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	AMRFileHeadStruct AMRFileHead;
	AMRFileHeadStruct *pAMRFileHead; 
	MemSet(&AMRFileHead, 0, sizeof(AMRFileHeadStruct));
	
	FileName = getWriteAMRFileName(AmrEnum, Time);
	MemCopy(msFile[FileName].date_time_s, (u8*)Time, 7);
	if(msFile[FileName].NUM_DATA > MAX_PN_NUM)
	{
		msFile[FileName].NUM_DATA = 0;											// msFile->NUM_DATA���ڱ�ʾ�洢�ı������ļ���
		//msFile[FileName].LEN_DATA = MAX_PN_NUM * sizeof(AMRFileHeadStruct);
	}
    /*�鵱ǰ���Ƿ��Ѿ�����������*/
	ms = Get_ms();
    pAMRFileHead = (AMRFileHeadStruct*)ADDR_128KDATABUFF ;//���ָ�벢������ȫ
    ms->msfile_read(FileName, 0, (u8*)ADDR_128KDATABUFF, msFile[FileName].NUM_DATA * sizeof(AMRFileHeadStruct));
    deep = msFile[FileName].NUM_DATA;
    for(i = 0; i < msFile[FileName].NUM_DATA; i++)
    {
        if(MemCompare((u8*)&pAMRFileHead[i], addr, METER_ADDR_SIZE) == 0) 
        {
            deep = i;//�����ǰ���ŵ������Ѿ����ڣ����滻֮
            break;
        }
    }
    MemCopy(AMRFileHead.Addr, addr, METER_ADDR_SIZE);
    AMRFileHead.DataLen = DataLen[0];
	MR((u32)&AMRFileHead.Time, (u32)Time, sizeof(RealTimeYYStruct));
    if(msFile[FileName].NUM_DATA == 0)
    {
        AMRFileHead.OffSet = MAX_PN_NUM * sizeof(AMRFileHeadStruct);
    }
    else
    {
        AMRFileHead.OffSet = msFile[FileName].LEN_DATA;
    }

    ms->msfile_write(FileName, deep*sizeof(AMRFileHeadStruct), (u8*)&AMRFileHead, sizeof(AMRFileHeadStruct));
    ms->msfile_write(FileName, AMRFileHead.OffSet, WriteData, DataLen[0]);	

    if(deep == msFile[FileName].NUM_DATA) msFile[FileName].NUM_DATA++;
    pComm_Ram->msFILEchange = 0;

    return Result;
}

/**
 *  @brief    �������ݶ�ȡ����
 *  
 *  @param  AmrEnum �������ͣ�ö���ͣ�������Ҫ�洢����������һ������
 *  @param  addr ����ַ
 *  @param  PointMeterType���ͺţ��ݱ�������
 *  @param DataID ���ݱ�ʶ
 *  @param Time����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi ss  00
 *  @param ReadData ��ȡ�����ݻ�����
 *  @param DataLen ��ȡ�ɹ����ֽ���
 *  @return  ���ݶ�ȡ�ɹ�������NO_ERROR���������ش������
 *
 *  @note    
 *
//����:
//1.���ݱ�ʶ���û�ж��壬�򷵻ش��󣬲�����0�ֽ�
//2.���ڿռ�ԭ�����ݴ洢ʱ��ѹ����ʽ�洢��û��Ԥ���ռ䡣����д����ʱȫ�������ݿ�д�롣
//3.���ڵ�2�㣬�����ݵ�ʱ�����������ǿ����ݣ��򵥷��ؿ����ݣ����������ǿ������е�����һ����ʶ������Ҫ����
//4.����ԭ����Ϊ���ݿ��е�ÿ�����ݶ���һ�����ȡ�
*/
ResultEnum NAND_ReadMeterCurveData(AMR_ENUM AmrEnum, u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen)
{
	AMRFileHeadStruct *pAMRFileHead;
	const LyAMRFileContentDefinition * pAMRFileContentDefinition;
	ms_Type *ms;
	
    ResultEnum Result = NO_ERROR;
    u32 FileName = 0, i = 0, DataPos = 0, DI = 0;
    u8* pBuf = (u8*)ADDR_128KDATABUFF;
    u32 searchId = 0xFFFFFFFF;												//������ѯ�����ݱ�ʶ
    u32 realDataSize = 0;													//���շ��ص����ݳ���
    u32 tempLen = 0, realGroupSize = 0, groupSeqno = 0, contentoffset = 0;	//ͷƫ�ƣ���ǰ��ȵ�����ƫ�ƣ���һ��ȵ�����ƫ��
    u32 dataExist = 0, dataSize = 0, MinuteHexDst = 0, MinuteHexSrc = 0; 			//��� �������Ƿ���ڣ����������ݿ��С
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
        
	for(i = 0; i < LyAMRFileContentDefinitionLen; i++)
	{	//�Ȳ������ȡ���ݱ�ʶ��Ӧ����Ϣ
	    pAMRFileContentDefinition = &constAMRFileContentDefinition[i];
	    if((pAMRFileContentDefinition->dataId & constGroupMask[pAMRFileContentDefinition->groupPos]) ==
		   (DataID & constGroupMask[pAMRFileContentDefinition->groupPos]))
	    {//�ж����ݱ�ʶ�Ƿ�Ϸ�
	        searchId=pAMRFileContentDefinition->searchId;
	        if(pAMRFileContentDefinition->groupFlag == 1)
	        {//�����������ʶ
	            if(pAMRFileContentDefinition->dataId == DataID)
	            {
	                realDataSize = pAMRFileContentDefinition->maxLen + pAMRFileContentDefinition->groupHeadSize;
					MC(0xFF, (u32)ReadData, realDataSize);
					if(pAMRFileContentDefinition->groupHeadSize == 1) ReadData[0] = pAMRFileContentDefinition->groupMax;
	            }
	            else
	            {
	                realDataSize=pAMRFileContentDefinition->eachDataLen;
					MC(0xFF, (u32)ReadData, realDataSize);
	            }
	        }
	        else
	        {
	            realDataSize=pAMRFileContentDefinition->maxLen;
				MC(0xFF, (u32)ReadData, realDataSize);
	        }
	        break;
	    }
	}
    if(searchId == 0xFFFFFFFF)
    {//û���ѵ�
        *DataLen = realDataSize;
        return ERROR_NoSuchDataID;
    }
	
	FileName = getReadAMRFileName(AmrEnum, Time);
    if(FileName == 0xFFFFFFFF) 
    {
        *DataLen = realDataSize;
        //*DataLen = 0;
        return ERROR_WRONG_TIME;
    }
	ms = Get_ms();
    pBuf = (u8*)ADDR_128KDATABUFF;
	pAMRFileHead = (AMRFileHeadStruct*)ADDR_128KDATABUFF;	
	ms->msfile_read(FileName, 0, pBuf, msFile[FileName].NUM_DATA * sizeof(AMRFileHeadStruct));	//���ļ�ͷ��Ϣ���Ի�ȡ�����ݴ洢��ʼ��ַ�����ݳ���
    for(i = 0; i < msFile[FileName].NUM_DATA; i++)
    {//���ļ�ͷ
        MemCopy((u8*)FileHead, (u8*)&pAMRFileHead[i], sizeof(AMRFileHeadStruct));
        if(MemCompare(FileHead->Addr,addr, METER_ADDR_SIZE) == 0)
        {
            //if(((FileHead->Time.Hour * 60+FileHead->Time.Minute)/AMR_CURVE_DENSITY) ==
            //    ((Time->Hour * 60 +Time->Minute)/AMR_CURVE_DENSITY))
            MinuteHexSrc = bcd_hex(Time->Minute);
			MinuteHexDst = bcd_hex(FileHead->Time.Minute);
            if((FileHead->Time.Hour == Time->Hour) && ((MinuteHexSrc/AMR_CURVE_DENSITY) == (MinuteHexDst/AMR_CURVE_DENSITY)))
            {//������ͬ��ʱ������ͬ������ϣ�ȡ������
                contentoffset = FileHead->OffSet;
                dataSize = FileHead->DataLen;
                ms->msfile_read(FileName, contentoffset, pBuf, dataSize);
                dataExist = 1;
                break;
            }
        }
    }
	if(dataExist == 0)
    {
        *DataLen = realDataSize;
		//*DataLen = 0;
        return ERROR_DestAddrNotExist;
	}
    //��ȡ�ص��������ҵ�Ŀ������ÿ�����ݸ�ʽDI(4) L(2) data(x)
	for(DataPos = 0; DataPos < dataSize; )
	{
		MemCopy((u8*)&DI, &pBuf[DataPos], QCSG_DI_SIZE);
		DataPos += QCSG_DI_SIZE;
		if(DI == searchId) break;//�ҵ�Ŀ�����ݿ�
		MemCopy((u8*)&tempLen, &pBuf[DataPos], 2);
		DataPos += 2 + tempLen;
	}
    if(DataPos >= dataSize)
    {//û���ҵ����ݿ�
        *DataLen = realDataSize;
		//*DataLen = 0;
        return ERROR_NoSuchDataID;
    }
    //�ҵ����ݿ�
    MemCopy((u8*)&tempLen, &pBuf[DataPos], 2);
    DataPos += 2;
    if(pAMRFileContentDefinition->dataId == DataID)
    {//�����ǰҪ���������ݱ�ʶ����Ŀ�����ݱ�ʶ������������������ǿ����ݣ������ǵ������ݱ�ʶ������
       if((pAMRFileContentDefinition->groupFlag == 1)&&(pAMRFileContentDefinition->groupHeadSize > 0))
       {
            realDataSize = tempLen;  
            MemCopy(&ReadData[pAMRFileContentDefinition->groupHeadSize], &pBuf[DataPos], tempLen);	//�ȸ�������
            //�ٲ���ͷ�ֽ�
            tempLen = tempLen / (pAMRFileContentDefinition->eachDataLen);
            tempLen = tempLen - 1;
            for(i = 0; i < pAMRFileContentDefinition->groupHeadSize; i++)
            {
                ReadData[i] = ((tempLen >> (8 * i)) & 0xff);
            }
            *DataLen = realDataSize + pAMRFileContentDefinition->groupHeadSize;
       }
       else
       {
            MemCopy(&ReadData[0], &pBuf[DataPos], tempLen);
            *DataLen = tempLen;
       }
    }
    else
    {//�������ڵ����ݱ�ʶ,�ж����ݱ�ʶ�Ƿ��ںϷ�����
        realGroupSize = tempLen /  (pAMRFileContentDefinition->eachDataLen) ;
        if(realGroupSize <=(pAMRFileContentDefinition->groupMax - pAMRFileContentDefinition->groupMin + 1))
        {//��������¶�������
            realGroupSize = pAMRFileContentDefinition->groupMin + realGroupSize ;
        }
        else
        {
            realGroupSize = pAMRFileContentDefinition->groupMax;
        }
        groupSeqno = ((~constGroupMask[pAMRFileContentDefinition->groupPos]) & DataID) >> (8*(4-pAMRFileContentDefinition->groupPos));
        if((groupSeqno >= pAMRFileContentDefinition->groupMin) && (groupSeqno <= realGroupSize))
        {//�Ϸ�����Ѱַ
            DataPos += pAMRFileContentDefinition->eachDataLen * (groupSeqno - pAMRFileContentDefinition->groupMin);
            MemCopy(&ReadData[0], &pBuf[DataPos], pAMRFileContentDefinition->eachDataLen);
            *DataLen = pAMRFileContentDefinition->eachDataLen;
        }
        else
        {//�Ƿ�
            *DataLen = realDataSize;
            return ERROR_NoSuchDataID;
        }
    }
	return Result;
}

/**
 *  @brief    �������ݱ��溯��
 *   
 *  @param  AmrEnum �������ͣ�ö���ͣ�������Ҫ�洢����������һ������
 *  @param  addr ����ַ
 *  @param  PointMeterType ���ͺţ��ݱ�������
 *  @param DataID ���ݱ�ʶ���ݱ�������
 *  @param Time ����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param WriteData Ҫд������ݣ���ʽ: dataId(4) L(2) data(x)
 *  @param DataLen Ҫд������ݳ���
 *  @return  ����д��ɹ�������NO_ERROR���������ش������
 *
 *  @note    �ļ���ʽ:�ļ�ͷ��288*4�ֽڷֱ��ʾ288�����ƫ�������ļ������Ա��ţ��ܳ��ȣ�DI(4) L1(2) data(L1) DI(4) L2(2) data(L2)...
 *
 */
ResultEnum NAND_WriteMeterCurveData(AMR_ENUM AmrEnum, u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
    AMRFileHeadStruct amrFileHeadInfo,*pAMRFileHead;
    ms_Type *ms;
    u32 FileName = 0, HeadOffset = 0, ContentOffset = 0, i = 0;
	u32 MinuteHexDst = 0, MinuteHexSrc = 0;
	u8* pBuf = (u8*)ADDR_128KDATABUFF;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
    
    FileName = getWriteAMRFileName(AmrEnum, Time);
    MemSet(msFile[FileName].date_time_s, 0, 7);
    MemCopy(msFile[FileName].date_time_s, (u8*)Time, 4);//ֻȡyyyymmdd
    //�ȿ���ǰд��������Ƿ��Ѿ�д��
    ms = Get_ms();
    if(msFile[FileName].BLOCK_ENTRY > 0)
    {
        pAMRFileHead = (AMRFileHeadStruct*)ADDR_128KDATABUFF;
		//���ļ�ͷ��Ϣ���Ի�ȡ�����ݴ洢��ʼ��ַ�����ݳ���
		ms->msfile_read(FileName, 0, pBuf, msFile[FileName].NUM_DATA * sizeof(AMRFileHeadStruct));	
        for(i = 0; i < msFile[FileName].NUM_DATA; i++)
        {
            if(MemCompare(pAMRFileHead[i].Addr,addr,METER_ADDR_SIZE) == 0)
            {
                //if(((pAMRFileHead[i].Time.Hour * 60 + pAMRFileHead[i].Time.Minute) / AMR_CURVE_DENSITY)==
                //    ((Time->Hour*60 + Time->Minute)/AMR_CURVE_DENSITY))
                MinuteHexSrc = bcd_hex(Time->Minute);
				MinuteHexDst = bcd_hex(pAMRFileHead[i].Time.Minute);
            	if((pAMRFileHead[i].Time.Hour == Time->Hour) && ((MinuteHexSrc/AMR_CURVE_DENSITY) == (MinuteHexDst/AMR_CURVE_DENSITY)))
                {
                    break;
                }
            }
        }
        HeadOffset = i * sizeof(AMRFileHeadStruct);
        MemCopy((u8*)&amrFileHeadInfo.Time,(u8*)Time, sizeof(RealTimeYYStruct));
        MemCopy((u8*)amrFileHeadInfo.Addr,(u8*)addr, METER_ADDR_SIZE);
        amrFileHeadInfo.OffSet=msFile[FileName].LEN_DATA;
        amrFileHeadInfo.DataLen=*DataLen;
		if(i >= msFile[FileName].NUM_DATA)
		{
			msFile[FileName].NUM_DATA++;
		}
    }
    else
    {
        HeadOffset = 0;
        ContentOffset = AMR_CURVE_MAX_FILEDEEP *sizeof(AMRFileHeadStruct);
        MemCopy((u8*)&amrFileHeadInfo.Time,(u8*)Time, sizeof(RealTimeYYStruct));
        MemCopy((u8*)amrFileHeadInfo.Addr,(u8*)addr, METER_ADDR_SIZE);
        amrFileHeadInfo.OffSet=ContentOffset;
        amrFileHeadInfo.DataLen=*DataLen;
        msFile[FileName].NUM_DATA++;
    }
    //�ļ�ͷ��ʽ-������Ч��ƫ����
   
    ms->msfile_write(FileName,HeadOffset,(u8*)&amrFileHeadInfo,sizeof(AMRFileHeadStruct)); //д�ļ�ͷ
    ms->msfile_write(FileName,amrFileHeadInfo.OffSet,WriteData,DataLen[0]); //���� 
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}



ResultEnum NAND_ReadStatisticsData(u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen)
{
	return NO_ERROR;
}


ResultEnum NAND_WriteStatisticsData(u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	return NO_ERROR;
}

/**
 *  @brief    �澯���ݶ�ȡ����
 *  
 *  @param  addr ����ַ
 *  @param DataID ���ݱ�ʶ
 *  @param StartTime���ݿ�ʼʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param EndTime ���ݽ���ʱ��ͬ��
 *  @param ReadData ��ȡ�����ݻ������������ݵĸ�ʽ��洢�ĸ�ʽ��ͬ����ʹ�������ж���
 *  @param DataLen ��ȡ�ɹ����ֽ���
 *  @return  ���ݶ�ȡ�ɹ�������NO_ERROR���������ش������
 *
 *  @note    �澯���ݷ������ļ�
 */
ResultEnum NAND_ReadAlarmRecord(u8* Addr, u32 DataID, RealTimeYYStruct* StartTime, RealTimeYYStruct* EndTime, u8* ReadData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    ResultEnum Result = NO_ERROR;
    u32 FileName = 0, headOffset = 0, contentOffset = 0, contentLen = 0, i = 0, DataPos = 0;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage + LEN_msFILEunltFlags); 

	if(DataID < 0xE2000001) return ERROR_NoSuchDataID;
    if(DataID > 0xE200004E)
	{
		if((DataID != 0xE20010FF) && (DataID != 0xE200FFFF)) return ERROR_NoSuchDataID;
	}
	ms = Get_ms();
	for(FileName = ALARM_FILE_START; FileName < ALARM_FILE_END; FileName++)
	{
		    if(msFile[FileName].BLOCK_ENTRY == 0) continue;
		    for(i = 0; i < msFile[FileName].NUM_DATA; i++)
		    {
		        headOffset = i *sizeof(AlarmEventFileHeadStruct);
		        ms->msfile_read(FileName, headOffset, (u8*)&AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
		        if((MemCompare(AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE) == 0) &&
		           (NAND_CmpDateTimeRange(&AlarmEventFileHeadInfo.OcurTime, StartTime, EndTime) == 1))
		        {//�ѵ��ˣ������Ǳ�����ͬ�����ݱ�ʶ��ͬ��ʱ���ڲ�ѯʱ�䷶Χ��
			            if((AlarmEventFileHeadInfo.DataID == DataID) || (DataID == 0xE200FFFF) || //���б�������        
			               ((DataID == 0xE20010FF) && (AlarmEventFileHeadInfo.OcurTime.Report != 0) && (AlarmEventFileHeadInfo.OcurTime.Ack == 0))) //����δ���ͳɹ���)
			            {//�з���Ŀ�꣬ȴû���ͳɹ���
				                contentOffset = AlarmEventFileHeadInfo.contentOffset;
				                contentLen = AlarmEventFileHeadInfo.contentLen;
				                MemCopy((u8*)&ReadData[DataPos], (u8*)&AlarmEventFileHeadInfo.DataID, QCSG_DI_SIZE);
				                DataPos += QCSG_DI_SIZE;
				                MemCopy((u8*)&ReadData[DataPos], (u8*)&contentLen, 2) ;
				                DataPos += 2;
				                ms->msfile_read(FileName, contentOffset, (u8*)&ReadData[DataPos], contentLen);
				                DataPos += contentLen;
			            }
		        }
		    }
	}
    
    *DataLen = DataPos;
	if(DataPos == 0) Result = ERROR_DestAddrNotExist;
    return Result;
}

ResultEnum NAND_ReadAlarmRecordByLatelyNum(u32 LatelyNum, AlarmEventFileHeadStruct* AlarmHead, u8* ReadData, u32* DataLen, u32* OutFileName)
{
	ms_Type *ms;	
    u32 FileName = 0, headOffset = 0, AlarmNum = 0;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	if(LatelyNum == 0) return ERROR_WRONG_ACCESS;
	AlarmNum = NAND_GetAlarmRecordNum();
	if(AlarmNum == 0) return ERROR_WRONG_ACCESS;
	if(LatelyNum > AlarmNum) return ERROR_WRONG_ACCESS;

	for(FileName = ALARM_FILE_START; FileName < ALARM_FILE_END; FileName++)
	{
		AlarmNum = msFile[FileName].NUM_DATA;
		if((AlarmNum >= ALARM_FILE_MAXDEEP) || (AlarmNum == 0)) continue;
		break;
	}
	AlarmNum = msFile[FileName].NUM_DATA;
	while(LatelyNum > AlarmNum)
	{
		LatelyNum -= AlarmNum;
		if(FileName == EVENT_FILE_START)
		{
			FileName = EVENT_FILE_END - 1;
		}
		else
		{
			FileName--;
		}
		AlarmNum = msFile[FileName].NUM_DATA;
	}

	ms = Get_ms();
	headOffset = AlarmNum - LatelyNum;		//���ĸ��澯��ʼ�ϱ�
	headOffset *= sizeof(AlarmEventFileHeadStruct);
	ms->msfile_read(FileName, headOffset, (u8*)AlarmHead, sizeof(AlarmEventFileHeadStruct));
	ms->msfile_read(FileName, AlarmHead->contentOffset, ReadData, AlarmHead->contentLen);
	DataLen[0] = AlarmHead->contentLen;
	OutFileName[0] = FileName;
	return NO_ERROR;
}
u32 NAND_GetAlarmRecordNum(void)
{
    u32 FileName = 0, AlarmNum = 0;  
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	for(FileName = ALARM_FILE_START; FileName < ALARM_FILE_END; FileName++)
	{
		if(msFile[FileName].BLOCK_ENTRY > 0) AlarmNum += msFile[FileName].NUM_DATA;

		if(AlarmNum >= ALARM_FILE_MAXDEEP) break;
	}

	if(AlarmNum > ALARM_FILE_MAXDEEP) AlarmNum = ALARM_FILE_MAXDEEP;
	return AlarmNum;
}
ResultEnum NAND_WriteAlarmRecordReportState(AlarmEventFileHeadStruct* AlarmHead, u32 FileName, u32 ReportDst, u32 ReportState)
{
	ms_Type *ms;
	AlarmEventFileHeadStruct* pAlarmHead;
	
	u32 HeadOffSet = 0, AlarmNum = 0, DataLen = 0, i = 0; 
	u8* pBuf = (u8*)ADDR_128KDATABUFF; 
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	if((FileName < ALARM_FILE_START) || (FileName >= ALARM_FILE_END)) return ERROR_WRONG_ACCESS;
	if((msFile[FileName].BLOCK_ENTRY == 0) || (msFile[FileName].NUM_DATA == 0)) return ERROR_WRONG_ACCESS;
	ms = Get_ms();

	AlarmNum = msFile[FileName].NUM_DATA;
	DataLen = msFile[FileName].NUM_DATA;
	DataLen *= sizeof(AlarmEventFileHeadStruct);
	ms->msfile_read(FileName, 0, pBuf, DataLen);
	pAlarmHead = (AlarmEventFileHeadStruct*)pBuf;
	for(i = 0; i < AlarmNum; i++)
	{
		if(MemCompare((u8*)AlarmHead, (u8*)&pAlarmHead[i], sizeof(AlarmEventFileHeadStruct)) == 0)
		{//����Ҫ��ʼ�ϱ����Ǹ��澯��λ��һ��
			AlarmHead->OcurTime.Report = ReportDst;
			AlarmHead->OcurTime.Ack = ReportState;
			HeadOffSet = i;	
//��Ҫ��ʼ�ϱ����Ǹ��澯��λ��
			HeadOffSet *= sizeof(AlarmEventFileHeadStruct);
			ms->msfile_write(FileName, HeadOffSet, (u8*)AlarmHead, sizeof(AlarmEventFileHeadStruct));
			return NO_ERROR;
		}
	}
	return ERROR_WRONG_ACCESS;
} 
/**
 *  @brief    �澯���ݴ洢����
 *  
 *  @param  addr ����ַ
 *  @param DataID ���ݱ�ʶ
 *  @param Time���ݿ�ʼʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param WriteData д������ݻ��������ݸ�ʽ����:DI(4) L(2) Data(L)
 *  @param DataLen д��ɹ����ֽ���
 *  @return  ����д��ɹ�������NO_ERROR���������ش������
 *
 *  @note    �澯���ݷ������ļ�ÿ���ļ�400����¼���ļ�ͷ-���ţ����ݱ�ʶ������ʱ�䣬����ƫ�ƣ����ݳ���
 */
ResultEnum NAND_WriteAlarmRecord(u8* Addr, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    u32 FileName = 0, headOffset = 0, contentOffset = 0;
    u8* pBuf = (u8*)ADDR_128KDATABUFF;
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
    
	for(FileName = ALARM_FILE_START; FileName < ALARM_FILE_END; FileName++)
	{
		if(msFile[FileName].BLOCK_ENTRY == 0) break;
		if((msFile[FileName].BLOCK_ENTRY > 0) && (msFile[FileName].NUM_DATA < ALARM_FILE_MAXDEEP)) break;
	}
	if(FileName >= ALARM_FILE_END)//�ļ������ˣ���ͷ��ʼ��
	{
		FileName = ALARM_FILE_START;
		msFile[FileName].BLOCK_ENTRY = 0;
		ms->msfile_Delete(FileName);
	}
    FileName = checkFile(FileName);
	ms = Get_ms();
    if(msFile[FileName].NUM_DATA == 0)
    {//���ļ�ͷ
        MemSet(pBuf, 0xff, ALARM_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
        ms->msfile_write(FileName, 0, pBuf, ALARM_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
    }
    //��֯�ļ�ͷ������
    headOffset = msFile[FileName].NUM_DATA * sizeof(AlarmEventFileHeadStruct);
    contentOffset = msFile[FileName].LEN_DATA;
    MemSet((u8*)&AlarmEventFileHeadInfo, 0, sizeof(AlarmEventFileHeadStruct));
    MemCopy(AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE);
    AlarmEventFileHeadInfo.DataID = DataID;
    AlarmEventFileHeadInfo.contentOffset = contentOffset;
    AlarmEventFileHeadInfo.contentLen = *DataLen;
    MemCopy((u8*)&AlarmEventFileHeadInfo.OcurTime, Time, sizeof(RealTimeYYStruct));

    ms->msfile_write(FileName, headOffset, (u8*)&AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
    ms->msfile_write(FileName, contentOffset, WriteData, *DataLen);
    msFile[FileName].NUM_DATA++;					//��������
    if(msFile[FileName].NUM_DATA >= ALARM_FILE_MAXDEEP)
    {//��ǰ�ļ�д��������һ���ļ���գ�����һ��д��
        msFile[FileName].FILECOUNT++;

		FileName++;
		if(FileName >= ALARM_FILE_END)
		{
			FileName = ALARM_FILE_START;		
		}
		if(msFile[FileName].BLOCK_ENTRY > 0)
		{
			msFile[FileName].BLOCK_ENTRY = 0;
			ms->msfile_Delete(FileName);
		}
    }
	
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}
/*
ResultEnum NAND_ReadEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* StartTime, RealTimeYYStruct* EndTime, u8* ReadData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	u32 FileInuseArray[EVENT_FILE_NUM];
	
    u32 FileName = 0, headOffset = 0, contentOffset = 0, TempFileName = 0, i = 0, j = 0;
	u32 FileInuseCount = 0, DataPos = 0, contentLen = 0, EventCnt = 0, EventCntEnable = 0;    
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
    
    TempFileName = EVENT_FILE_START;
    FileInuseCount = 0;
    for(FileName = EVENT_FILE_START; FileName < EVENT_FILE_END; FileName++)
    {//ȡ�����������ļ�
        if(msFile[FileName].BLOCK_ENTRY > 0)
        {
            FileInuseArray[FileInuseCount++] = FileName;
        }
    }
    if(FileInuseCount == 0)
    {
        *DataLen = 0;
        return ERROR_DestAddrNotExist;
    }
    //�����������ļ������¾ɳ̶�����ð������
    for(i = 0; i < FileInuseCount - 1; i++)
    {
        for(j = 0; j < FileInuseCount - 1 - i; j++)
        {
            if(msFile[j].FILECOUNT > msFile[j + 1].FILECOUNT)
            {
                TempFileName = FileInuseArray[j];
                FileInuseArray[j] = FileInuseArray[j + 1];
                FileInuseArray[j + 1] = TempFileName;
            }
        }
    }

	if((StartTime->YearH == 0xFF) && (StartTime->YearL == 0xFF) && (StartTime->Month == 0xFF) && 
	   (StartTime->Day == 0xFF) && (StartTime->Hour == 0xFF) && (StartTime->Minute == 0xFF) &&
	   (EndTime->YearH == 0xFF) && (EndTime->YearL == 0xFF) && (EndTime->Month == 0xFF) && 
	   (EndTime->Day == 0xFF) && (EndTime->Hour == 0xFF) && (EndTime->Minute == 0xFF))
	{
		EventCntEnable = 1;
	}

	ms = Get_ms();
    for(i = 0; i < FileInuseCount; i++)
    {
        FileName = FileInuseArray[i];
        for(i = 0; i < msFile[FileName].NUM_DATA; i++)
        {
            headOffset = i *sizeof(AlarmEventFileHeadStruct);
            ms->msfile_read(FileName, headOffset, (u8*)&AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
            if((MemCompare((u8*)&AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE) == 0) &&
                ((AlarmEventFileHeadInfo.DataID == DataID) || (DataID == 0xE20100FF)) &&
                (NAND_CmpDateTimeRange(&AlarmEventFileHeadInfo.OcurTime, StartTime, EndTime) == 1))
            {//�ѵ��ˣ������Ǳ�����ͬ�����ݱ�ʶ��ͬ��ʱ���ڲ�ѯʱ�䷶Χ��
                contentOffset = AlarmEventFileHeadInfo.contentOffset;
                contentLen = AlarmEventFileHeadInfo.contentLen;
                MemCopy((u8*)&ReadData[DataPos], (u8*)&AlarmEventFileHeadInfo.DataID, QCSG_DI_SIZE);
                DataPos += QCSG_DI_SIZE;
                MemCopy((u8*)&ReadData[DataPos], (u8*)&contentLen, 2) ;
                DataPos += 2;
                ms->msfile_read(FileName, contentOffset, (u8*)&ReadData[DataPos], contentLen);
                DataPos += contentLen;

				if(EventCntEnable != 0) EventCnt++;
            }
			if((EventCntEnable != 0) && (EventCnt >= LATELY_EVENT_MAX_CNT)) break;
        }
		if((EventCntEnable != 0) && (EventCnt >= LATELY_EVENT_MAX_CNT)) break;
    }
    *DataLen = DataPos;
	if(DataPos == 0) return ERROR_DestAddrNotExist;
	return NO_ERROR;
}
*/

#if (TEST_ON == 1)
u32 NAND_GetEventRecordNum(u32 DataID)
{
    u32 FileName = 0, EventNum = 0;  
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	if(DataID == 0xE20100FF) return EventNum;
	if((DataID < 0xE2010001) || (DataID > 0xE2010015)) return EventNum;
	
	FileName = EVENT_FILE_START + ((DataID & 0x000000FF) << 1) - 2;
	if(msFile[FileName].BLOCK_ENTRY > 0) EventNum += msFile[FileName].NUM_DATA;
	FileName++;
	if(msFile[FileName].BLOCK_ENTRY > 0) EventNum += msFile[FileName].NUM_DATA;

	return EventNum;
}

ResultEnum NAND_ReadEventRecordByLatelyNum(u32 DataID, u32 LatelyNum, AlarmEventFileHeadStruct* EventHead, u8* ReadData, u32* DataLen)
{
	ms_Type *ms;
	
    u32 FileName = 0, FilePos = 0, headOffset = 0, EventNum = 0;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	
	if(LatelyNum == 0) return ERROR_WRONG_ACCESS;
	if(DataID == 0xE20100FF) return ERROR_WRONG_ACCESS;
	if((DataID < 0xE2010001) || (DataID > 0xE2010015)) return ERROR_NoSuchDataID;
	
	EventNum = NAND_GetEventRecordNum(DataID);
	if(EventNum == 0) return ERROR_WRONG_ACCESS;
	if(LatelyNum > EventNum) return ERROR_WRONG_ACCESS;

	FilePos = ((DataID & 0x000000FF) << 1) - 2;
	FileName = EVENT_FILE_START + FilePos;
	EventNum = msFile[FileName].NUM_DATA;
	if((EventNum >= EVENT_FILE_MAXDEEP) || (EventNum == 0))
	{
		FileName++;
		EventNum = msFile[FileName].NUM_DATA;
	}

	if(LatelyNum > EventNum)
	{
		LatelyNum -= EventNum;
		if(FileName == (EVENT_FILE_START + FilePos))
		{
			FileName = EVENT_FILE_START + FilePos + 1;
		}
		else
		{
			FileName = EVENT_FILE_START + FilePos;
		}
		EventNum = msFile[FileName].NUM_DATA;
		if(LatelyNum > EventNum) return ERROR_NoSuchDataID;
	}

	ms = Get_ms();
	headOffset = EventNum - LatelyNum;
	headOffset *= sizeof(AlarmEventFileHeadStruct);
	ms->msfile_read(FileName, headOffset, (u8*)EventHead, sizeof(AlarmEventFileHeadStruct));
	ms->msfile_read(FileName, EventHead->contentOffset, ReadData, EventHead->contentLen);
	DataLen[0] = EventHead->contentLen;
	return NO_ERROR;
}

ResultEnum NAND_ReadEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* StartTime, RealTimeYYStruct* EndTime, u8* ReadData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    u32 FileName = 0, FileNameCnt = 0, headOffset = 0, contentOffset = 0, i = 0;
	u32 DataPos = 0, contentLen = 0, EventCnt = 0, EventCntEnable = 0, j = 0;  

	u32 FileNameArray[EVENT_FILE_NUM];
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	if(DataID == 0xE20100FF)
	{
		for(FileName = EVENT_FILE_START; FileName < EVENT_FILE_END; FileName++)
		{
			if((msFile[FileName].BLOCK_ENTRY > 0) && (msFile[FileName].NUM_DATA > 0))
			{
				FileNameArray[FileNameCnt++] = FileName;
			}
		}
	}
	else
	{
		FileNameArray[FileNameCnt++] = EVENT_FILE_START + ((DataID & 0x000000FF) << 1) - 2;
		FileNameArray[FileNameCnt++] = EVENT_FILE_START + ((DataID & 0x000000FF) << 1) - 1;
	}

	if((StartTime->YearH == 0xFF) && (StartTime->YearL == 0xFF) && (StartTime->Month == 0xFF) && 
	   (StartTime->Day == 0xFF) && (StartTime->Hour == 0xFF) && (StartTime->Minute == 0xFF) &&
	   (EndTime->YearH == 0xFF) && (EndTime->YearL == 0xFF) && (EndTime->Month == 0xFF) && 
	   (EndTime->Day == 0xFF) && (EndTime->Hour == 0xFF) && (EndTime->Minute == 0xFF))
	{
		EventCntEnable = 1;
	}

	ms = Get_ms();
    for(i = 0; i < FileNameCnt; i++)
    {
        FileName = FileNameArray[i];
        for(j = 0; j < msFile[FileName].NUM_DATA; j++)
        {
            headOffset = j *sizeof(AlarmEventFileHeadStruct);
            ms->msfile_read(FileName, headOffset, (u8*)&AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
            if((MemCompare((u8*)&AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE) == 0) &&
                ((AlarmEventFileHeadInfo.DataID == DataID) || (DataID == 0xE20100FF)) &&
                (NAND_CmpDateTimeRange(&AlarmEventFileHeadInfo.OcurTime, StartTime, EndTime) == 1))
            {//�ѵ��ˣ������Ǳ�����ͬ�����ݱ�ʶ��ͬ��ʱ���ڲ�ѯʱ�䷶Χ��
                contentOffset = AlarmEventFileHeadInfo.contentOffset;
                contentLen = AlarmEventFileHeadInfo.contentLen;
                MemCopy((u8*)&ReadData[DataPos], (u8*)&AlarmEventFileHeadInfo.DataID, QCSG_DI_SIZE);
                DataPos += QCSG_DI_SIZE;
                MemCopy((u8*)&ReadData[DataPos], (u8*)&contentLen, 2) ;
                DataPos += 2;
                ms->msfile_read(FileName, contentOffset, (u8*)&ReadData[DataPos], contentLen);
                DataPos += contentLen;

				if(EventCntEnable != 0) EventCnt++;
            }
			if((EventCntEnable != 0) && (EventCnt >= LATELY_EVENT_MAX_CNT)) break;
        }
		if((EventCntEnable != 0) && (EventCnt >= LATELY_EVENT_MAX_CNT)) break;
    }
    *DataLen = DataPos;
	if(DataPos == 0) return ERROR_DestAddrNotExist;
	return NO_ERROR;
}

ResultEnum NAND_WriteEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    u32 FileName = 0, FilePos = 0, headOffset = 0, contentOffset = 0;	
    u8* pBuf = (u8*)ADDR_128KDATABUFF;
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	if((DataID < 0xE2010001) || (DataID > 0xE2010015)) return ERROR_NoSuchDataID;
	ms = Get_ms();
	FilePos = ((DataID & 0x000000FF) << 1) - 2;
	FileName = EVENT_FILE_START + FilePos;
	if(msFile[FileName].NUM_DATA >= EVENT_FILE_MAXDEEP)
	{
		FileName++;
		if(msFile[FileName].NUM_DATA >= EVENT_FILE_MAXDEEP)
		{											// �����ļ������������ȣ���ɾ����һ���ļ�
			FileName = EVENT_FILE_START + FilePos;
			ms->msfile_Delete(FileName);
			checkFile(FileName);
		}
	}
	
    if(msFile[FileName].NUM_DATA == 0)
    {//���ļ�ͷ
        MemSet(pBuf, 0xff, EVENT_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
        ms->msfile_write(FileName, 0, pBuf, EVENT_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
    }
	MR((u32)msFile[FileName].date_time_s, (u32)Time, 7);
    //��֯�ļ�ͷ������
    headOffset = msFile[FileName].NUM_DATA * sizeof(AlarmEventFileHeadStruct);
    contentOffset = msFile[FileName].LEN_DATA;
    MemSet((u8*)&AlarmEventFileHeadInfo, 0, sizeof(AlarmEventFileHeadStruct));
    MemCopy((u8*)&AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE);
    AlarmEventFileHeadInfo.DataID = DataID;
    AlarmEventFileHeadInfo.contentOffset = contentOffset;
    AlarmEventFileHeadInfo.contentLen = *DataLen;
    MemCopy((u8*)&AlarmEventFileHeadInfo.OcurTime, (u8*)Time, sizeof(RealTimeYYStruct));
	
    ms->msfile_write(FileName, headOffset, (u8*) &AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
    ms->msfile_write(FileName, contentOffset, WriteData, *DataLen);
    msFile[FileName].NUM_DATA++;		//�������
	msFile[FileName].FILEID = DataID;
	if(msFile[FileName].NUM_DATA >= EVENT_FILE_MAXDEEP) 
	{
		if(FileName == (EVENT_FILE_START + FilePos))
		{
			FileName = EVENT_FILE_START + FilePos + 1;
		}
		else
		{
			FileName = EVENT_FILE_START + FilePos;
		}	
		ms->msfile_Delete(FileName);
		checkFile(FileName);
	}
	
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}
#else	//err20190419 ��ȷ���·��������; ���ܵ���Һ�����¼���¼��ʾ����
u32 NAND_GetEventRecordNum(u32 DataID)
{
    u32 FileName = 0, EventNum = 0;  
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	if(DataID == 0xE20100FF) return EventNum;
	if((DataID < 0xE2010001) || (DataID > 0xE2010015)) return EventNum;
	
	FileName = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
	if(msFile[FileName].BLOCK_ENTRY > 0) EventNum += msFile[FileName].NUM_DATA;
	FileName = EVENT_FILE_START + (DataID & 0x000000FF);
	if(msFile[FileName].BLOCK_ENTRY > 0) EventNum += msFile[FileName].NUM_DATA;

	return EventNum;
}

ResultEnum NAND_ReadEventRecordByLatelyNum(u32 DataID, u32 LatelyNum, AlarmEventFileHeadStruct* EventHead, u8* ReadData, u32* DataLen)
{
	ms_Type *ms;
	
    u32 FileName = 0, headOffset = 0, EventNum = 0;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	
	if(LatelyNum == 0) return ERROR_WRONG_ACCESS;
	if(DataID == 0xE20100FF) return ERROR_WRONG_ACCESS;
	if((DataID < 0xE2010001) || (DataID > 0xE2010015)) return ERROR_NoSuchDataID;
	
	EventNum = NAND_GetEventRecordNum(DataID);
	if(EventNum == 0) return ERROR_WRONG_ACCESS;
	if(LatelyNum > EventNum) return ERROR_WRONG_ACCESS;
	
	FileName = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
	EventNum = msFile[FileName].NUM_DATA;
	if((EventNum >= EVENT_FILE_MAXDEEP) || (EventNum == 0))
	{
		FileName = EVENT_FILE_START + (DataID & 0x000000FF);
	}

	EventNum = msFile[FileName].NUM_DATA;
	if(LatelyNum > EventNum)
	{
		LatelyNum -= EventNum;
		if(FileName == (EVENT_FILE_START + (DataID & 0x000000FF)))
		{
			FileName = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
		}
		else
		{
			FileName = EVENT_FILE_START + (DataID & 0x000000FF);
		}
		EventNum = msFile[FileName].NUM_DATA;
		if(LatelyNum > EventNum) return ERROR_NoSuchDataID;
	}

	ms = Get_ms();
	headOffset = EventNum - LatelyNum;
	headOffset *= sizeof(AlarmEventFileHeadStruct);
	ms->msfile_read(FileName, headOffset, (u8*)EventHead, sizeof(AlarmEventFileHeadStruct));
	ms->msfile_read(FileName, EventHead->contentOffset, ReadData, EventHead->contentLen);
	DataLen[0] = EventHead->contentLen;
	return NO_ERROR;
}

ResultEnum NAND_ReadEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* StartTime, RealTimeYYStruct* EndTime, u8* ReadData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    u32 FileName = 0, FileNameCnt = 0, headOffset = 0, contentOffset = 0, i = 0;
	u32 DataPos = 0, contentLen = 0, EventCnt = 0, EventCntEnable = 0, j = 0;  

	u32 FileNameArray[EVENT_FILE_NUM];
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	if(DataID == 0xE20100FF)
	{
		for(FileName = EVENT_FILE_START; FileName < EVENT_FILE_END; FileName++)
		{
			if((msFile[FileName].BLOCK_ENTRY > 0) && (msFile[FileName].NUM_DATA > 0))
			{
				FileNameArray[FileNameCnt++] = FileName;
			}
		}
	}
	else
	{
		FileNameArray[FileNameCnt++] = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
		FileNameArray[FileNameCnt++] = EVENT_FILE_START + (DataID & 0x000000FF);
	}

	if((StartTime->YearH == 0xFF) && (StartTime->YearL == 0xFF) && (StartTime->Month == 0xFF) && 
	   (StartTime->Day == 0xFF) && (StartTime->Hour == 0xFF) && (StartTime->Minute == 0xFF) &&
	   (EndTime->YearH == 0xFF) && (EndTime->YearL == 0xFF) && (EndTime->Month == 0xFF) && 
	   (EndTime->Day == 0xFF) && (EndTime->Hour == 0xFF) && (EndTime->Minute == 0xFF))
	{
		EventCntEnable = 1;
	}

	ms = Get_ms();
    for(i = 0; i < FileNameCnt; i++)
    {
        FileName = FileNameArray[i];
        for(j = 0; j < msFile[FileName].NUM_DATA; j++)
        {
        	if(EventCntEnable != 0)//err20181218 ȫFFʱ��ȡ�����10������
				headOffset = (msFile[FileName].NUM_DATA - j - 1) * sizeof(AlarmEventFileHeadStruct);
			else if (EventCntEnable == 0)
            	headOffset = j *sizeof(AlarmEventFileHeadStruct);
            ms->msfile_read(FileName, headOffset, (u8*)&AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
            if((MemCompare((u8*)&AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE) == 0) &&
                ((AlarmEventFileHeadInfo.DataID == DataID) || (DataID == 0xE20100FF)) &&
                (NAND_CmpDateTimeRange(&AlarmEventFileHeadInfo.OcurTime, StartTime, EndTime) == 1))
            {//�ѵ��ˣ������Ǳ�����ͬ�����ݱ�ʶ��ͬ��ʱ���ڲ�ѯʱ�䷶Χ��
                contentOffset = AlarmEventFileHeadInfo.contentOffset;
                contentLen = AlarmEventFileHeadInfo.contentLen;
                MemCopy((u8*)&ReadData[DataPos], (u8*)&AlarmEventFileHeadInfo.DataID, QCSG_DI_SIZE);
                DataPos += QCSG_DI_SIZE;
                MemCopy((u8*)&ReadData[DataPos], (u8*)&contentLen, 2) ;
                DataPos += 2;
                ms->msfile_read(FileName, contentOffset, (u8*)&ReadData[DataPos], contentLen);
                DataPos += contentLen;

				if(EventCntEnable != 0) EventCnt++;
            }
			if((EventCntEnable != 0) && (EventCnt >= LATELY_EVENT_MAX_CNT)) break;
        }
		if((EventCntEnable != 0) && (EventCnt >= LATELY_EVENT_MAX_CNT)) break;
    }
    *DataLen = DataPos;
	if(DataPos == 0) return ERROR_DestAddrNotExist;
	return NO_ERROR;
}

ResultEnum NAND_WriteEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    u32 FileName = 0, headOffset = 0, contentOffset = 0;	
    u8* pBuf = (u8*)ADDR_128KDATABUFF;
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);

	if((DataID < 0xE2010001) || (DataID > 0xE2010015)) return ERROR_NoSuchDataID;
	ms = Get_ms();
	FileName = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
	if(msFile[FileName].NUM_DATA >= EVENT_FILE_MAXDEEP)
	{
		FileName = EVENT_FILE_START + (DataID & 0x000000FF);
		if(msFile[FileName].NUM_DATA >= EVENT_FILE_MAXDEEP)
		{
			FileName = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
			ms->msfile_Delete(FileName);
			checkFile(FileName);
		}
	}
	
    if(msFile[FileName].NUM_DATA == 0)
    {//���ļ�ͷ
        MemSet(pBuf, 0xff, EVENT_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
        ms->msfile_write(FileName, 0, pBuf, EVENT_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
    }
	MR((u32)msFile[FileName].date_time_s, (u32)Time, 7);
    //��֯�ļ�ͷ������
    headOffset = msFile[FileName].NUM_DATA * sizeof(AlarmEventFileHeadStruct);
    contentOffset = msFile[FileName].LEN_DATA;
    MemSet((u8*)&AlarmEventFileHeadInfo, 0, sizeof(AlarmEventFileHeadStruct));
    MemCopy((u8*)&AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE);
    AlarmEventFileHeadInfo.DataID = DataID;
    AlarmEventFileHeadInfo.contentOffset = contentOffset;
    AlarmEventFileHeadInfo.contentLen = *DataLen;
    MemCopy((u8*)&AlarmEventFileHeadInfo.OcurTime, (u8*)Time, sizeof(RealTimeYYStruct));
	
    ms->msfile_write(FileName, headOffset, (u8*) &AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
    ms->msfile_write(FileName, contentOffset, WriteData, *DataLen);
    msFile[FileName].NUM_DATA++;		//�������
	msFile[FileName].FILEID = DataID;
	if(msFile[FileName].NUM_DATA >= EVENT_FILE_MAXDEEP) 
	{
		if(FileName == (EVENT_FILE_START + (DataID & 0x000000FF)))
		{
			FileName = EVENT_FILE_START + (DataID & 0x000000FF) - 1;
		}
		else
		{
			FileName = EVENT_FILE_START + (DataID & 0x000000FF);
		}	
		ms->msfile_Delete(FileName);
		checkFile(FileName);
	}
	
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}
#endif
/*
ResultEnum NAND_WriteEventRecord(u8* Addr, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	AlarmEventFileHeadStruct AlarmEventFileHeadInfo;
	ms_Type *ms;
	
    u32 FileName = 0, headOffset = 0, contentOffset = 0;
    u8* pBuf = (u8*)ADDR_128KDATABUFF;
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
    msFILE_TypeDef *msFile = (msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
    
    FileName = getWriteAlarmEventFileName(AMR_EVENT);
	ms = Get_ms();
    if(msFile[FileName].NUM_DATA == 0)
    {//���ļ�ͷ
        MemSet(pBuf, 0xff, EVENT_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
        ms->msfile_write(FileName, 0, pBuf, EVENT_FILE_MAXDEEP*sizeof(AlarmEventFileHeadStruct));
    }
    //��֯�ļ�ͷ������
    headOffset = msFile[FileName].NUM_DATA * sizeof(AlarmEventFileHeadStruct);
    contentOffset = msFile[FileName].LEN_DATA;
    MemSet((u8*)&AlarmEventFileHeadInfo, 0, sizeof(AlarmEventFileHeadStruct));
    MemCopy((u8*)&AlarmEventFileHeadInfo.MeterAddr, Addr, METER_ADDR_SIZE);
    AlarmEventFileHeadInfo.DataID = DataID;
    AlarmEventFileHeadInfo.contentOffset = contentOffset;
    AlarmEventFileHeadInfo.contentLen = *DataLen;
    MemCopy((u8*)&AlarmEventFileHeadInfo.OcurTime, (u8*)Time, sizeof(Time));
	
    ms->msfile_write(FileName, headOffset, (u8*) &AlarmEventFileHeadInfo, sizeof(AlarmEventFileHeadStruct));
    ms->msfile_write(FileName, contentOffset, WriteData, *DataLen);
    msFile[FileName].NUM_DATA++;		//�������
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}
*/
/**
 *  @brief    ��ͨ��������ݺ���
 *  
 *  @param  addr ������ʱ��ʹ��
 * @param PointMeterType ��������ʱ��ʹ��
 *  @param DataID ���ݱ�ʶ����Ӧ���������
 *  @param Time ����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param ReadData ��ȡ�����ݻ��壬
 *  @param DataLen �ɹ���ȡ�����ݳ���
 *  @return  ���ݶ�ȡ�ɹ�������NO_ERROR���������ش������
 *
 *  @note    1.һ�ζ�ȡ�������������ݣ�ע:������ڼ��˵�������ܻ�ܴ�
 *           2.��ȡ���������ڴ��������ʱ��֮ǰ���͸�ʱ�䴦��һ���ϱ����ڣ�
 *           3.�������ݵĸ�ʽ������(L)����������ʹ�����Լ�����ĸ�ʽ
 *           4.
 */
ResultEnum NAND_ReadNormalTask(u8*addr, AMRFileHeadStruct* FileHead, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen)
{
    u32 FileNames[2], i, findCount, FileCnt;  
    u32 FileName = 0, tempLen = 0, contentOffset = 0;
	AMRFileHeadStruct * pNormalTaskDataFileHeadInfo;
    NormalTaskParaStruct normalTaskParam;
    RealTimeStruct baseTime,readTime,lastTime;
    ms_Type* ms;
    msFILE_TypeDef *msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
    u8* pBuf = (u8*)ADDR_128KDATABUFF;
	ResultEnum Result = NO_ERROR;
    
    Result = NAND_ReadPara(PARA_Task, 0, DataID, (u8*)&normalTaskParam, &tempLen);
    if(Result != NO_ERROR)
    {
        *DataLen = 0;
        return Result;
    }
    MemCopy((u8*)&baseTime, (u8*) &normalTaskParam.ReportTime, sizeof(RealTimeStruct));
    YYTimeToRealTime(Time, &readTime);
    findCount = 0;
    for(i = NORMAL_TASK_FILE_START; i < NORMAL_TASK_FILE_END; i++)
    {
        if(msFile[i].FILEID == DataID)
        {
            if(findCount < 2)
            {
                FileNames[findCount++] = i;
            }
        }
    }
    if(findCount == 0)
    {//û�ҵ���˵����ǰ�������û�г�������
        *DataLen = 0;
        return ERROR_NoSuchDataID;
    }
	tempLen = 0;
	ms = Get_ms();
    for(FileCnt = 0; FileCnt < findCount; FileCnt++)
    {
    	FileName = FileNames[FileCnt];
        pNormalTaskDataFileHeadInfo = (AMRFileHeadStruct*)ADDR_128KDATABUFF;
		//���ļ�ͷ��Ϣ���Ի�ȡ�����ݴ洢��ʼ��ַ�����ݳ���
		ms->msfile_read(FileName, 0, pBuf, msFile[FileName].NUM_DATA * sizeof(AMRFileHeadStruct));	
        for(i = 0; i < msFile[FileName].NUM_DATA; i++)
        {            
            YYTimeToRealTime(&pNormalTaskDataFileHeadInfo[i].Time, &lastTime);
			if(MemCompare(pNormalTaskDataFileHeadInfo[i].Addr, addr, METER_ADDR_SIZE) != 0) continue;
            if(isTaskDataTime(AMR_NormalTask,&baseTime, &lastTime, &readTime, normalTaskParam.SamplePeriod, normalTaskParam.SamplePeriodUnit)==1)
            {
                MemCopy((u8*)FileHead, (u8*)&pNormalTaskDataFileHeadInfo[i], sizeof(AMRFileHeadStruct));
                contentOffset = pNormalTaskDataFileHeadInfo[i].OffSet;
                tempLen = pNormalTaskDataFileHeadInfo[i].DataLen;
                ms->msfile_read(FileName, contentOffset, ReadData, tempLen);//��������
                break;
            }
        }
		if(i < msFile[FileName].NUM_DATA) break;
    }
	
    *DataLen = tempLen;
    if(tempLen > 0)
    {
        return NO_ERROR;
    }
    else
    {
        return ERROR_NoSuchDataID;
    }
}

/**
 *  @brief    ��ͨ����д���ݺ���
 *  
 *  @param  addr ����
 * @param PointMeterType ������
 *  @param DataID ���ݱ�ʶ����Ӧ���������
 *  @param Time ����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param WriteData д������ݣ���ʽ: ���ݲ��ָ�ʽʹ�����Լ����壬�������ݱ�ʶ(4)+����L(2)+��������(L)
 *  @param DataLen Ҫд������ݳ���
 *  @return  ����д��ɹ�������NO_ERROR���������ش������
 *
 *  @note    1. һ���ļ�ֻд��һ���ϱ����ڵ����ݣ���ͬ�ϱ����ڵ�����һ��д�ڲ�ͬ���ļ���
 *                2. һ��д��һֻ������������
 *               3. 
 *               4.
 */
ResultEnum NAND_WriteNormalTask(u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
	u32 timeSpan[4];
	NormalTaskParaStruct normalTaskParam;
    RealTimeStruct baseTime,curTime,lastTime;//��׼ʱ�䣬�˴�д��ʱ�䣬���һ�γ���ʱ��
    RealTimeYYStruct lastYYTime;
    AMRFileHeadStruct normalTaskDataFileHeadInfo, *pNormalTaskFileHeadInfo;
    ms_Type *ms;
    u32 FileName = 0, tempFileName = 0, tempLen = 0, fileCount = 0, i = 0, headOffset = 0;
    u8* pBuf=(u8*)ADDR_128KDATABUFF;
    msFILE_TypeDef *msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	ResultEnum Result = NO_ERROR;
	
    Result = NAND_ReadPara(PARA_Task, 0, DataID, (u8*)&normalTaskParam, &tempLen);
    if(Result != NO_ERROR)
    {
        *DataLen = 0;
        return Result;
    }
    YYTimeToRealTime(Time, &curTime);
    MemSet((u8*)&lastYYTime, 0, sizeof(RealTimeYYStruct));
    MemSet((u8*)&lastTime, 0, sizeof(RealTimeStruct));
    MemCopy((u8*)&baseTime, (u8*)&normalTaskParam.ReportTime, sizeof(RealTimeStruct));
    tempFileName = 0xffffffff;
    fileCount = 0xffffffff;
    for(FileName = NORMAL_TASK_FILE_START; FileName < NORMAL_TASK_FILE_END; FileName++)
    {
        if(msFile[FileName].BLOCK_ENTRY > 0)
        {
            if(msFile[FileName].FILEID == DataID)
            {
                if(fileCount > msFile[FileName].FILECOUNT)
                {//ȡ���µ��ļ���ʱ����Ϊ��ǰ����д����ļ�
                    MemCopy((u8*)&lastYYTime, msFile[FileName].date_time_s, 6);
                    tempFileName = FileName;
                    fileCount = msFile[FileName].FILECOUNT;
                }
            }
        }
    }
    if(tempFileName == 0xffffffff)
    {//**û���ҵ�������ǵ�һ��д����������ݣ��Ǿ���һ���ļ�д��****
        FileName = getTaskFileName(AMR_NormalTask, DataID, tempFileName);
    }
    else
    {//***�ҵ��ˣ���Ҫ�жϴ˴�д���ʱ���Ƿ����һ��д���ʱ����һ���ϱ�����
        YYTimeToRealTime(&lastYYTime, &lastTime);
		//err20180509 3������2���ݱ�ʶ����ȱʧ���� ����2min�ϱ�-����֤�� ������δ��֤
		if(QCSG_TimerSub(&lastTime, &curTime, timeSpan) == 0)
		{//�˴�д��ʱ������һ��д��ʱ����ͬ�����л��ļ�
			if(msFile[tempFileName].NUM_DATA >= NORMAL_TASK_FILE_MAXDEEP)
            {//����������¼��
                FileName = getTaskFileName(AMR_NormalTask,DataID,tempFileName);
            }
            else
            {
                FileName = tempFileName;
            }
		}
		else
		{//ʱ�䲻��ȣ��ж��Ƿ���һ���ϱ�����֮��
			if(QCSG_Judge_TaskTiming(&lastTime, &curTime, &baseTime, normalTaskParam.ReportPeriod, normalTaskParam.ReportPeriodUnit))
			{//����һ�����ڣ��л��ļ�
				FileName = getTaskFileName(AMR_NormalTask, DataID, tempFileName);
			}
			else 
			{
				if(msFile[tempFileName].NUM_DATA >= NORMAL_TASK_FILE_MAXDEEP)
				{//����������¼��
					FileName = getTaskFileName(AMR_NormalTask,DataID,tempFileName);
				}
				else
				{
					FileName = tempFileName;
				}
			}
		}
		
		
//        if(QCSG_Judge_TaskTiming(&lastTime, &curTime, &baseTime, normalTaskParam.ReportPeriod, normalTaskParam.ReportPeriodUnit) )	
//        {//����һ�����ڣ��л��ļ�
//             FileName = getTaskFileName(AMR_NormalTask, DataID, tempFileName);
//        }
//        else
//        {
//            if(msFile[tempFileName].NUM_DATA >= NORMAL_TASK_FILE_MAXDEEP)
//            {//����������¼��
//                FileName = getTaskFileName(AMR_NormalTask,DataID,tempFileName);
//            }
//            else
//            {
//                FileName = tempFileName;
//            }
//        }
    }
    if(FileName == 0xffffffff)
    {//*****û�п����ļ�
        return ERROR_FILE_USEDUP;
    }
	ms = Get_ms();
    msFile[FileName].FILEID = DataID;
    if(msFile[FileName].BLOCK_ENTRY > 0)
    {//�����ǰ�ļ����ǿ��ļ�������Ҫ�жϵ�ǰʱ���������Ƿ��Ѿ�д����
        pNormalTaskFileHeadInfo = (AMRFileHeadStruct*)ADDR_128KDATABUFF;
		//���ļ�ͷ��Ϣ���Ի�ȡ�����ݴ洢��ʼ��ַ�����ݳ���
		ms->msfile_read(FileName, 0, pBuf, msFile[FileName].NUM_DATA * sizeof(AMRFileHeadStruct));	
        for(i = 0; i < msFile[FileName].NUM_DATA; i++)
        {
            if(MemCompare(pNormalTaskFileHeadInfo[i].Addr, addr, METER_ADDR_SIZE) == 0)
            {
                MemCopy((u8*)&lastYYTime, &pNormalTaskFileHeadInfo[i].Time, 6);
                YYTimeToRealTime(&lastYYTime, &lastTime);
                if(isTaskDataTime(AMR_NormalTask, &baseTime, &lastTime, &curTime, normalTaskParam.ReportPeriod, normalTaskParam.ReportPeriodUnit) ==1)
                {
                    break;
                }
            }
        }
        headOffset = i*sizeof(AMRFileHeadStruct);
        MemCopy((u8*)&normalTaskDataFileHeadInfo.Time,(u8*)Time, sizeof(RealTimeYYStruct));
        MemCopy((u8*)normalTaskDataFileHeadInfo.Addr,(u8*)addr, METER_ADDR_SIZE);
        normalTaskDataFileHeadInfo.OffSet = msFile[FileName].LEN_DATA;
        normalTaskDataFileHeadInfo.DataLen = *DataLen;
        if(i >= msFile[FileName].NUM_DATA)
        {
            msFile[FileName].NUM_DATA++;
        }
    }
    else
    {	
        headOffset = 0;
		MemCopy(msFile[FileName].date_time_s, (u8*)Time, 6);
        MemCopy((u8*)&normalTaskDataFileHeadInfo.Time,(u8*)Time, sizeof(RealTimeYYStruct));
        MemCopy((u8*)normalTaskDataFileHeadInfo.Addr,(u8*)addr, METER_ADDR_SIZE);
        normalTaskDataFileHeadInfo.OffSet = NORMAL_TASK_FILE_MAXDEEP*sizeof(AMRFileHeadStruct);
        normalTaskDataFileHeadInfo.DataLen = *DataLen;
        msFile[FileName].NUM_DATA++;
    }
    //д�ļ�ͷ
    ms->msfile_write(FileName,headOffset,(u8*) &normalTaskDataFileHeadInfo,sizeof(AMRFileHeadStruct));
    //**д�ļ����� **//
    ms->msfile_write(FileName, normalTaskDataFileHeadInfo.OffSet, WriteData, *DataLen);
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}
/**
 *  @brief    �м����������ȡ����
 *  
 *  @param  RelayType �м����ͣ��ݱ�������
 *  @param DataID ���ݱ�ʶ����Ӧ���������
 *  @param Time ����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param ReadData ��ȡ�����ݻ��壬
 *  @param DataLen ��ȡ�ɹ������ݳ���
 *  @return  ���ݶ�ȡ�ɹ�������NO_ERROR���������ش������
 *
 *  @note    1.��ȡָ��ʱ�����δ�ϱ�������
 *           2.�����������ʱ�䳬����һ�����ڣ�����Ϊ��Ч
 *           3.�������ݸ�ʽ:����ʱ��yyyymmddhhnn + ����L(2) +���� (L)
 */
ResultEnum NAND_ReadRelayTask(u8 RelayType, u32 DataID, RealTimeYYStruct* Time, u8* ReadData, u32* DataLen)
{
    u32 FileNames[2];
    u32 FileName = 0, FileCnt = 0, tempLen = 0, findCount = 0, i = 0, dataPos = 0;
	RelayTaskDataFileHeadStruct* pRelayTaskDataFileHeadInfo;
    RelayTaskParaStruct relayTaskParam;
    RealTimeStruct baseTime,readTime,lastTime;
    ms_Type *ms;
	u8* pBuf = (u8*)ADDR_128KDATABUFF;
    msFILE_TypeDef *msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);  
	ResultEnum Result = NO_ERROR;
	
    Result = NAND_ReadPara(PARA_Task, 0, DataID, (u8*)&relayTaskParam, &tempLen);
    if(Result != NO_ERROR)
    {
        *DataLen = 0;
        return Result;
    }
    MemCopy((u8*)&baseTime, (u8*) &relayTaskParam.ReportTime, sizeof(RealTimeStruct));
    YYTimeToRealTime(Time, &readTime);
    for(i = RELAY_TASK_FILE_START; i < RELAY_TASK_FILE_END; i++)
    {
        if(msFile[i].FILEID == DataID)
        {
            if(findCount < 2)
            {
                FileNames[findCount++] = i;
            }
        }
    }
    if(findCount == 0)
    {//û�ҵ���˵����ǰ�������û�г�������
        *DataLen = 0;
        return ERROR_NoSuchDataID;
    }

	ms = Get_ms();
    for(FileCnt = 0; FileCnt < findCount; FileCnt++)
    {
    	FileName = FileNames[FileCnt];
		pRelayTaskDataFileHeadInfo = (RelayTaskDataFileHeadStruct*)ADDR_128KDATABUFF;
		//���ļ�ͷ��Ϣ���Ի�ȡ�����ݴ洢��ʼ��ַ�����ݳ���
		ms->msfile_read(FileName, 0, pBuf, msFile[FileName].NUM_DATA * sizeof(RelayTaskDataFileHeadStruct));
        for(i = 0; i < msFile[FileName].NUM_DATA; i++)
        {
            YYTimeToRealTime(&pRelayTaskDataFileHeadInfo[i].OcurTime, &lastTime);
            if(isTaskDataTime(AMR_RelayTask,&baseTime, &lastTime, &readTime, relayTaskParam.SamplePeriod, relayTaskParam.SamplePeriodUnit)==1)
            {
                dataPos = 0;
                //MemCopy((u8*)&ReadData[dataPos],(u8*)&pRelayTaskDataFileHeadInfo->OcurTime, sizeof(RealTimeYYStruct));
                //dataPos += sizeof(RealTimeYYStruct);
                //MemCopy((u8*)&ReadData[dataPos], (u8*)&pRelayTaskDataFileHeadInfo->dataLen, 2);
                //dataPos += 2;
                ms->msfile_read(FileName, pRelayTaskDataFileHeadInfo->offset, (u8*)&ReadData[dataPos], pRelayTaskDataFileHeadInfo->dataLen);
                dataPos += pRelayTaskDataFileHeadInfo->dataLen;
                break;
            }
        }
		if(i < msFile[FileName].NUM_DATA) break;
    }
    *DataLen = dataPos;
    if(dataPos > 0)
    {
        return NO_ERROR;
    }
    else
    {
        return ERROR_NoSuchDataID;
    }
}
/**
 * @brief    �ж�����ʱ���Ƿ����ȡʱ����ͬһ������
 *
 * @param dataType �������ͣ��˴�ֻ������ֵnormal task ��relay task
 * @param baseTime ��׼ʱ�� 
 * @param dataTime ����ʱ��
 * @param readTime ��ѯʱ��
 * @param period ����
 * @param periodUnit ��������0-3�ֱ��ʾ ��ʱ����
 * @return ����һ�����ڣ�����1 ���򷵻�0
 * @note 1.����ʱ��С�ڻ�׼ʱ�䣬��Ч2.��ѯʱ��С������ʱ�䣬��Ч3.��ѯʱ��������ʱ����һ�����ڣ���Ч
 */
u32 isTaskDataTime(AMR_ENUM dataType,RealTimeStruct *baseTime,RealTimeStruct *dataTime,RealTimeStruct *readTime,u8 period,u8 periodUnit )
{
    u32 timeSpan[4],i;
    RealTimeStruct tempTime;
    RealTimeYYStruct baseYYTime,dataYYTime,readYYTime,tempYYTime;
    RealTimeToYYTime(baseTime, &baseYYTime);
    RealTimeToYYTime(dataTime, &dataYYTime);
    RealTimeToYYTime(readTime, &readYYTime);
	
    //if(cmpDateTime(dataType,(u8*)&baseYYTime, &readYYTime)==cmpGreaterThan)
    //{//��ȡʱ��Ȼ���ʱ��С���϶�û������
    //    return 0;
    //}
    
    if( QCSG_TimerSub(dataTime, readTime, timeSpan) < 0)//����ʱ��ȶ�ȡ��ʱ��С
    {
        return 0;
    }
    /*����ʱ��ȶ�ȡ��ʱ���ʱ��Ҫ�ж������Ƿ���ͬ*/
    switch(periodUnit)
    {
        case 0://����
            if((timeSpan[0]*24*60 +timeSpan[1]*60 +timeSpan[2] )>period )
            {
                return 0;
            }
            break;
        case 1://ʱ
            if((timeSpan[0] * 24 + timeSpan[1] )>period)
            {
                return 0;
            }
            break;
        case 2://��
	        if((timeSpan[0] *24 +timeSpan[1])>(period*24))
	        {//���ʱ��Сʱ�������ڵ�Сʱ��ʾ��ȡ�
	            return 0;
	        }
	        break;
        case 3://��
            MemCopy((u8*)&tempTime, readTime, sizeof(RealTimeStruct));
            for(i=0;i<period;i++)
            {
                QCSG_DecOneMonth(&tempTime);
            }
            RealTimeToYYTime(&tempTime, &tempYYTime);
            if(cmpDateTime(dataType, (u8 *)&tempYYTime, &dataYYTime)==cmpGreaterThan)
            {
                return 0;
            }
        break;
    }
    return 1;
}

/**
 *  @brief    �м�����������溯��
 *  
 *  @param  RelayType �м����ͣ��ݱ�������
 *  @param DataID ���ݱ�ʶ����Ӧ���������
 *  @param Time ����ʱ�䣬���ݸ�ʽyyyy mm dd hh mi 00  00
 *  @param WriteData Ҫд������ݣ���ʽ: ���ݲ���{����(L)}ת�����ص���֡����
 *  @param DataLen Ҫд������ݳ���
 *  @return  ����д��ɹ�������NO_ERROR���������ش������
 *
 *  @note    1.һ������ռ�����ļ����ֱ𱣴浱ǰ�ϱ����ڵ����ݺ���һ���ϱ����ڵ�����
 *                2.�ļ��滻����-�л��ϱ����ڵ�ʱ���л��ļ�
 *               3.�����л���ʱ�������ǰ����δ�������ݣ����������ļ����Ѿ�ʹ�ã���ɾ��ʱ������ķǵ��յ��ļ���
 */
ResultEnum NAND_WriteRelayTask(u8 RelayType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
    
    u32 FileName,tempFileName,tempLen;
    u32 headOffset,contentOffset;
    ms_Type *ms;
    RelayTaskParaStruct relayTaskParam;
    RealTimeStruct baseTime,curTime,lastTime;//��׼ʱ�䣬�˴�д��ʱ�䣬���һ�γ���ʱ��
    RealTimeYYStruct lastYYTime;
    RelayTaskDataFileHeadStruct relayTaskDataFileHeadInfo;
	msFILE_TypeDef *msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	ResultEnum Result = NO_ERROR;
	
    Result = NAND_ReadPara(PARA_Task, 0, DataID, (u8*)&relayTaskParam, &tempLen);
    if(Result != NO_ERROR)
    {
        *DataLen = 0;
        return Result;
    }
    YYTimeToRealTime(Time, &curTime);
    MemSet((u8*)&lastYYTime, 0, 8);
    MemSet((u8*)&lastTime, 0, 8);
    MemCopy((u8*)&baseTime, (u8*)&relayTaskParam.ReportTime, sizeof(RealTimeStruct));
    tempFileName = 0xffffffff;
    for(FileName = RELAY_TASK_FILE_START; FileName < RELAY_TASK_FILE_END; FileName++)
    {
        if(msFile[FileName].BLOCK_ENTRY > 0)
        {
            if(msFile[FileName].FILEID == DataID)
            {
                if(cmpDateTime(AMR_RelayTask, msFile[FileName].date_time_s, &lastYYTime) == cmpGreaterThan)
                {//ȡ����ʱ����Ϊ��Ϊ���д���ʱ��
                    MemCopy((u8*)&lastYYTime, msFile[FileName].date_time_s, 7);
                    tempFileName = FileName;
                }
            }
        }
    }
    if(tempFileName == 0xffffffff)
    {//**û���ҵ�������ǵ�һ��д����������ݣ��Ǿ���һ���ļ�д��****
        FileName = getTaskFileName(AMR_RelayTask,DataID,tempFileName);
    }
    else
    {//***�ҵ��ˣ���Ҫ�жϴ˴�д���ʱ���Ƿ����һ��д���ʱ����һ���ϱ�����
        YYTimeToRealTime(&lastYYTime, &lastTime);
        if(QCSG_Judge_TaskTiming(&lastTime, &curTime, &baseTime, relayTaskParam.ReportPeriod, relayTaskParam.ReportPeriodUnit) )
        {
            FileName = getTaskFileName(AMR_RelayTask, DataID, tempFileName);
        }
        else
        {
            if(msFile[tempFileName].NUM_DATA >= RELAY_TASK_FILE_MAXDEEP)
            {
                FileName = getTaskFileName(AMR_RelayTask, DataID, tempFileName);
            }
            else
            {
                FileName = tempFileName;
            }
        }
    }
    if(FileName == 0xffffffff)
    {//*****û�п����ļ�
        return ERROR_FILE_USEDUP;
    }

	ms = Get_ms();
    msFile[FileName].FILEID = DataID;
    MemCopy(msFile[FileName].date_time_s, (u8*)Time, 6);
    //**  д�ļ�ͷ **//
    headOffset = msFile[FileName].NUM_DATA * sizeof(RelayTaskDataFileHeadStruct);
    if(msFile[FileName].BLOCK_ENTRY == 0)
    {
        contentOffset = RELAY_TASK_FILE_MAXDEEP * sizeof(RelayTaskDataFileHeadStruct);
    }
    else
    {
        contentOffset = msFile[FileName].LEN_DATA;
    }
    MemCopy((u8*)&relayTaskDataFileHeadInfo.OcurTime, (u8*)Time, sizeof(RealTimeYYStruct) );
    relayTaskDataFileHeadInfo.offset = contentOffset;
    relayTaskDataFileHeadInfo.dataLen = *DataLen;
    ms->msfile_write(FileName, headOffset, (u8*)&relayTaskDataFileHeadInfo, sizeof(RelayTaskDataFileHeadStruct));
    ms->msfile_write(FileName, contentOffset, WriteData, *DataLen);
    msFile[FileName].NUM_DATA++;
    pComm_Ram->msFILEchange = 0;
    return NO_ERROR;
}


ResultEnum NAND_ReadWriteTerminalExeFile(u32 PORTn, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
    u32 FileName = 0, Temp = 0, contentOffset = 0, Temp1 = 0, Temp2 = 0;
    ms_Type *ms;
    TerminalExeFileInfoStruct TerminalExeFileInfo;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	msFILE_TypeDef *msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	ResultEnum Result = NO_ERROR;

	switch(DataID)
	{
		case 0xE3010001:		
			if(DataLen[0] != (sizeof(TerminalExeFileInfoStruct) - 2)) return ERROR_InvalidData;
			MR((u32)&TerminalExeFileInfo, (u32)WriteData, DataLen[0]);
			if(TerminalExeFileInfo.Type >= TERMINAL_EXE_FILE_NUM)  return ERROR_InvalidData;		
			FileName = TERMINAL_EXE_FILE_START + TerminalExeFileInfo.Type;
			ms = Get_ms();
			ms->msfile_Delete(FileName);
			checkFile(FileName);
			//if((TerminalExeFileInfo.TotalLen % TerminalExeFileInfo.TotalCount) != 0)
			//{
			//	TerminalExeFileInfo.CountLen = TerminalExeFileInfo.TotalLen / (TerminalExeFileInfo.TotalCount - 1);
			//}
			//else
			//{
			//	TerminalExeFileInfo.CountLen = TerminalExeFileInfo.TotalLen / TerminalExeFileInfo.TotalCount;
			//}
			
			MR((u32)msFile[FileName].date_time_s, (u32)Time, sizeof(RealTimeYYStruct));
			contentOffset = 0;
			ms->msfile_write(FileName, 0, WriteData, sizeof(TerminalExeFileInfoStruct));
			msFile[FileName].NUM_DATA = 0;
			pComm_Ram->msFILEchange = 0;
			pTSysCtrl->CurExeFileType = TerminalExeFileInfo.Type;
			pTSysCtrl->CurExeFilePort = PORTn;
			break;
		case 0xE3010002:
			if(pTSysCtrl->CurExeFileType >= TERMINAL_EXE_FILE_NUM) return ERROR_InvalidData;
			Temp = WriteData[2];
			Temp += WriteData[3] << 8;							// ��ǰ�����ļ�֡�κ����ݳ���
			if(Temp != (DataLen[0] - 6))  return ERROR_InvalidData;
			Temp1 = CRC16_XMODEM(&WriteData[4], Temp);			//hz ����У�鷽ʽ����λ��һ��
			Temp2 = WriteData[DataLen[0] - 2];
			Temp2 += WriteData[DataLen[0] - 1] << 8;			// ��ǰ�����ļ�֡�κ����� CRC
			if(Temp1 != Temp2) return ERROR_InvalidData;
			FileName = TERMINAL_EXE_FILE_START + pTSysCtrl->CurExeFileType;
			ms = Get_ms();
			ms->msfile_read(FileName, 0, (u8*)&TerminalExeFileInfo, sizeof(TerminalExeFileInfoStruct));	
			Temp1 = WriteData[0];		
			Temp1 += WriteData[1] << 8;							// ��ǰ�����ļ�֡�κ�
			//if((Temp1 != (TerminalExeFileInfo.TotalCount - 1)) && ((u16)Temp != TerminalExeFileInfo.CountLen)) return ERROR_InvalidData;
			
			//contentOffset = Temp1;
			//contentOffset *= Temp;
			//contentOffset += sizeof(TerminalExeFileInfoStruct);
			if(Temp1 == msFile[FileName].NUM_DATA)//err20190215 ��ʾ���������һ��(��ȷ) �����ʾ����ĶκŲ����ϣ������д洢��
			{
				contentOffset = msFile[FileName].LEN_DATA;
				ms->msfile_write(FileName, contentOffset, &WriteData[4], Temp);
				msFile[FileName].NUM_DATA = Temp1+1;//NUM_DATA�ѳɹ�������� Ϊ��ǰ�����+1
			}
			else if(Temp1 < msFile[FileName].NUM_DATA)//����ö��ѽ��չ�����ֱ�ӻظ�ȷ��֡ ������(����ȷ��֡��λ��δ�յ��ط�ǰһ������)
			{
				;
			}
			else //����м�ν��ն�ʧ��ظ�����
			{
				return ERROR_InvalidData;
			}
			if(msFile[FileName].NUM_DATA == TerminalExeFileInfo.TotalCount) //���жδ������
			{
				pTSysCtrl->CurExeFileUpdateDelay = 3;//��ɴ����3s��ʱ ȷ���ظ�֡����
				pTSysCtrl->CurExeFileReady = TERMINAL_EXE_FILE_READY;
			}
			pComm_Ram->msFILEchange = 0;
			pTSysCtrl->CurExeFilePort = PORTn;
			pTSysCtrl->CurExeFileCnt = Temp1;
			break;
		case 0xE3010003:
			if(DataLen[0] != (sizeof(TerminalExeFileInfoStruct) + 2)) return ERROR_InvalidData;
			MR((u32)&TerminalExeFileInfo, (u32)WriteData, sizeof(TerminalExeFileInfoStruct) - 2);
			if(TerminalExeFileInfo.Type >= TERMINAL_EXE_FILE_NUM) return ERROR_InvalidData;
			FileName = TERMINAL_EXE_FILE_START + TerminalExeFileInfo.Type;
			ms = Get_ms();
			ms->msfile_read(FileName, 0, (u8*)&TerminalExeFileInfo, sizeof(TerminalExeFileInfoStruct));

			Temp2 = WriteData[DataLen[0] - 2];
			Temp2 += WriteData[DataLen[0] - 1] << 8;		// �����κ� Temp2
			Temp1 = WriteData[DataLen[0] - 4];				
			Temp1 += WriteData[DataLen[0] - 3] << 8;		// ��ʼ�κ� Temp1
			if(MemCompare((u8*)&TerminalExeFileInfo, WriteData, sizeof(TerminalExeFileInfoStruct) - 2) != 0)//return ERROR_InvalidData;//ԭ�ļ�������ļ���Ϣ����ͬ
			{
				Temp = 0;
			}
			else //�ļ���ͬ ���жϲ�ѯ�κŷ�Χ
			{
				Temp = msFile[FileName].NUM_DATA;				// ��ǰ�κ�
				if(Temp1 >= TerminalExeFileInfo.TotalCount) return ERROR_InvalidData;	//��ʼ�κ� ���ڵ��� �ܶκ�
				if(Temp2 >= TerminalExeFileInfo.TotalCount) return ERROR_InvalidData;	//�����κ� ���ڵ��� �ܶκ�				
			}			
			
			if(Temp1 > Temp2) return ERROR_InvalidData;		// ��ʼ�κ� > �����κ�	
			if(Temp2 > Temp)								
			{												// �����κ� > ��ǰ�նκ�	
				if(Temp > 1024) return ERROR_InvalidData;	//�κŴ���470 
				//err20190611��ȷ��Ϊ����470֡����������ļ�������ܻ�����ʧ�ܣ������ȸ�Ϊ1024֡
				if(Temp1 >= Temp)							
				{											// ��ʼ�κ� >= ��ǰ�κ�,Ӧ��֡��ʼ�κŲ������
					Temp = Temp2 - Temp1 + 1;
				}
				else
				{											// ��ʼ�κ� < ��ǰ�κ�,Ӧ��֡��ʼ�κ������
					Temp1 = Temp;							//Temp1 ��ǰ�κ�
					Temp = Temp2 - Temp + 1;				//Tempʣ��κ��� = �����κ�-��ǰ�κ� +1 
					//Temp1 ++;								//ת����δ�����κ�
				}
				WriteData[DataLen[0]++] = Temp;
				WriteData[DataLen[0]++] = Temp >> 8;		// �б�����
				for(Temp2 = 0; Temp2 < Temp; Temp2++)
				{
					WriteData[DataLen[0]++] = Temp1;
					WriteData[DataLen[0]++] = Temp1 >> 8;	//δ�ɹ����յĶκ��б�
					Temp1++;
				}
			}
			else
			{			
				WriteData[DataLen[0]++] = 0;				// �����κ� <= �ѽ��նκţ����ѯ��ȫ�����ճɹ�
				WriteData[DataLen[0]++] = 0;
			}		
			pTSysCtrl->CurExeFileType = TerminalExeFileInfo.Type;
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;
	}
	return Result;
}


void NAND_TerminalExeFileUpdate(void)
{
	TerminalExeFileInfoStruct TerminalExeFileInfo;
	u8* p8s;
	u8* p8d;
	u32* p32;
	os_Type *os;
	ms_Type *ms;
#ifdef IC_NUC9xx
	u32 x;
	int yaffs;
	char *pFileName;
#endif	
	u32 i = 0, y = 0, contentOffset = 0, ExeFileLen = 0, FileName = 0;  
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
#ifdef IC_STM32F4xx
	FILE *file = NULL;
#endif
	

	
	if(pTSysCtrl->CurExeFileReady != TERMINAL_EXE_FILE_READY) return;
	if(pTSysCtrl->CurExeFileType > EXE_FILE_TYPE_PLC) return;
	if((pTSysCtrl->CurExeFilePort != GPRSPORT) && (pTSysCtrl->CurExeFilePort != ENETPORT) && 
	   (pTSysCtrl->CurExeFilePort != RS485_1PORT) && (pTSysCtrl->CurExeFilePort != RS485_2PORT) &&
	   (pTSysCtrl->CurExeFilePort != RS485_3PORT) && (pTSysCtrl->CurExeFilePort != RS232PORT))
	{
		return;
	}

	pTSysCtrl->CurExeFileReady = 0;
	os = Get_os();
	ms = Get_ms();
	FileName = TERMINAL_EXE_FILE_START + pTSysCtrl->CurExeFileType;
	ms->msfile_read(FileName, 0, (u8*)&TerminalExeFileInfo, sizeof(TerminalExeFileInfoStruct));
	if(TerminalExeFileInfo.TotalLen > LENmax_Download)
	{	
		WarningOccur(pTSysCtrl->CurExeFilePort, "�����ļ����ȴ���");
		return;
	}
	
	#ifdef IC_STM32F4xx
		i=os->ffree("N0:");
		if(i<LENmax_Download)
		{//�ռ䲻��
			os->fdelete("N0:/TEST/",NULL);//ɾ��TEST�ļ���
			i=os->ffree("N0:");
			if(i<LENmax_Download)
			{//�ռ仹����		
				os->fdelete("N0:/Program Files/",NULL);//ɾ��Program Files�ļ���
			}
		}
	#endif
	#ifdef IC_NUC9xx
		i=os->yaffs_freespace("user/Update");
		if(i<LENmax_Download)
		{//�ռ䲻��
				os->yaffs_unlinkdir("user/TEST/");//ɾ��TEST�ļ���
				i=os->yaffs_freespace("user/Update");
				if(i<LENmax_Download)
				{//�ռ仹����		
					os->yaffs_unlinkdir("user/Program Files/");//ɾ��Program Files�ļ���
				}
		}
	#endif
						
						
	p8s = ms->ms_malloc((size_t)LENmax_Download);//��̬�����ҵ��ڴ��
	p8d = ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
	if((p8s == 0)||(p8d == 0))
	{
		ms->Init_my_heap();
		p8s=ms->ms_malloc((size_t)LENmax_Download);//��̬�����ҵ��ڴ��
		p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
	}
	contentOffset = sizeof(TerminalExeFileInfoStruct);
	ms->msfile_read(FileName, contentOffset, p8s, 1);
	if(p8s[0] == 0)
	{//��ѹ��
		ExeFileLen = TerminalExeFileInfo.TotalLen;
		ms->msfile_read(FileName, contentOffset, p8s, ExeFileLen);	
		p32 = (u32*)p8s;
		#ifdef IC_STM32F4xx
		switch(p32[128])//������:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
		{
			case 0:
				file=os->fopen("N0:/Download/MSCSG.bin","w");
				break;
			case 1:
				file=os->fopen("N0:/Download/OSCSG.bin","w");
				break;
			case 2:
				file=os->fopen("N0:/Download/MeterCSG.bin","w");
				break;
			case 3:
				file=os->fopen("N0:/Download/TerminalCSG.bin","w");
				break;
			case 4:
				file=os->fopen("N0:/Download/MenuCSG.bin","w");
				break;
			case 5:
				file=os->fopen("N0:/Download/Font.bin","w");
				break;
			case 7:
				file=os->fopen("N0:/Download/ModuleCSG.bin","w");
				break;
			default:
				file = NULL;
				break;
		}
		if(file != NULL)
		{
			os->fwrite(p8s, 1, ExeFileLen, file);
			os->fclose(file);
		}
		#endif
	}
	else
	{//Lzmaѹ��
		ExeFileLen = TerminalExeFileInfo.TotalLen;
		ms->msfile_read(FileName, contentOffset, p8s, ExeFileLen);
		i = p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);
		if(i < (2*LENmax_Download))
		{
			y = os->RAM_LzmaDecode((u32)p8s, TerminalExeFileInfo.TotalLen, (u32)p8d, i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
		}	
		if(y != i)
		{
			WarningOccur(pTSysCtrl->CurExeFilePort, "�����ļ����ȴ���");
		}
		else
		{
			p32 = (u32*)p8d;
			#ifdef IC_STM32F4xx
			switch(p32[128])//������:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
			{
				case 0:
					file=os->fopen("N0:/Download/MSCSG.zip","w");
					break;
				case 1:
					file=os->fopen("N0:/Download/OSCSG.zip","w");
					break;
				case 2:
					file=os->fopen("N0:/Download/MeterCSG.zip","w");
					break;
				case 3:
					file=os->fopen("N0:/Download/TerminalCSG.zip","w");
					break;
				case 4:
					file=os->fopen("N0:/Download/MenuCSG.zip","w");
					break;
				case 5:
					file=os->fopen("N0:/Download/Font.zip","w");
					break;
				case 6:
					file=os->fopen("N0:/Download/PACKCSG.zip","w");
					break;
				case 7:
					file=os->fopen("N0:/Download/ModuleCSG.zip","w");
					break;
				default:
					file = NULL;
					break;
			}
			if(file != NULL)
			{
				os->fwrite(p8s, 1, ExeFileLen, file);
				os->fclose(file);
			}
			#endif
			#ifdef IC_NUC9xx
				pFileName=(char*)ADDR_DATABUFF;
				pFileName=strcpy(pFileName,"user/Update/");
				switch(p32[128])//������:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
				{
					case 0:
						pFileName=strcpy(pFileName,FileFactory_MS);
						break;
					case 1:
						pFileName=strcpy(pFileName,FileFactory_OS);
						break;
					case 2:
						pFileName=strcpy(pFileName,FileFactory_Meter);
						break;
					case 3:
						pFileName=strcpy(pFileName,FileFactory_Terminal);
						break;
					case 4:
						pFileName=strcpy(pFileName,FileFactory_Menu);
						break;
					case 5:
						pFileName=strcpy(pFileName,FileFactory_Font);
						break;
					case 6:
						pFileName=strcpy(pFileName,FileFactory_PACK);
						break;
					case 7:
						pFileName=strcpy(pFileName,FileFactory_Modele);
						break;
					default:
						//goto FileErr;
						pFileName = NULL;
				}
	//			pFileName[0]=0;
	//			yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
	//			if(yaffs<0)
	//			{
	//				goto FileErr;
	//			}
				if(pFileName != NULL)
				{
					os->yaffs_WRITE(yaffs,p8s,x);
					os->yaffs_close(yaffs);
				}
	//			//д���򿪶�һ��
	//			yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_RDWR,0);
	//			os->yaffs_close(yaffs);
		#endif
		}
	}
	#ifdef IC_STM32F4xx
		if(file == NULL)
		{
			WarningOccur(pTSysCtrl->CurExeFilePort, "�ļ�·��������");
		}
	#endif
	#ifdef IC_NUC9xx
		if(pFileName == NULL)
		{
			WarningOccur(pTSysCtrl->CurExeFilePort, "�ļ�·��������");
		}
	#endif
	ms->ms_free(p8s);			//�ͷ��ҵ��ڴ�ѷ���
	ms->ms_free(p8d);			//�ͷ��ҵ��ڴ�ѷ���
	FileDownload->New = 0;		//�����ļ������־:0=��,1=û
}

/*
ResultEnum NAND_ReadWriteTerminalExeFile(u32 PORTn, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
{
    u32 Temp = 0, contentOffset = 0, Temp1 = 0, Temp2 = 0;
    TerminalExeFileInfoStruct TerminalExeFileInfo;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
    Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	ResultEnum Result = NO_ERROR;

	switch(DataID)
	{
		case 0xE3010001:		
			if(DataLen[0] != (sizeof(TerminalExeFileInfoStruct) - 2)) return ERROR_InvalidData;
			MR((u32)&TerminalExeFileInfo, (u32)WriteData, DataLen[0]);
			if(TerminalExeFileInfo.Type >= TERMINAL_EXE_FILE_NUM)  return ERROR_InvalidData;		
			TerminalExeFileInfo.CountLen = TerminalExeFileInfo.TotalLen / TerminalExeFileInfo.TotalCount;
			MW((u32)&TerminalExeFileInfo, ADDR_Download_ENTRY, sizeof(TerminalExeFileInfoStruct));
			pTSysCtrl->CurExeFileType = TerminalExeFileInfo.Type;
			pTSysCtrl->CurExeFilePort = PORTn;
			break;
		case 0xE3010002:
			if(pTSysCtrl->CurExeFileType >= TERMINAL_EXE_FILE_NUM) return ERROR_InvalidData;
			Temp = WriteData[2];
			Temp += WriteData[3] << 8;							// ��ǰ�����ļ�֡�κ����ݳ���
			if(Temp != (DataLen[0] - 6))  return ERROR_InvalidData;
			Temp1 = CCIT16_CRC(&WriteData[4], Temp);
			Temp2 = WriteData[DataLen[0] - 2];
			Temp2 += WriteData[DataLen[0] - 1] << 8;			// ��ǰ�����ļ�֡�κ����� CRC
			if(Temp1 != Temp2) return ERROR_InvalidData;
			MR((u32)&TerminalExeFileInfo, ADDR_Download_ENTRY, sizeof(TerminalExeFileInfoStruct));
			Temp1 = WriteData[0];		
			Temp1 += WriteData[1] << 8;							// ��ǰ�����ļ�֡�κ�
			if((Temp1 != (TerminalExeFileInfo.TotalCount - 1)) && ((u16)Temp != TerminalExeFileInfo.CountLen)) return ERROR_InvalidData;
			
			contentOffset = Temp1;
			contentOffset *= Temp;
			contentOffset += sizeof(TerminalExeFileInfoStruct);
			contentOffset += ADDR_Download_ENTRY;
			MW((u32)&TerminalExeFileInfo, contentOffset, Temp);
			if(Temp1 == (TerminalExeFileInfo.TotalCount - 1)) pTSysCtrl->CurExeFileReady = TERMINAL_EXE_FILE_READY;
			pComm_Ram->msFILEchange = 0;
			pTSysCtrl->CurExeFileCnt = Temp1;
			pTSysCtrl->CurExeFilePort = PORTn;
			break;
		case 0xE3010003:
			if(DataLen[0] != (sizeof(TerminalExeFileInfoStruct) + 2)) return ERROR_InvalidData;
			MR((u32)&TerminalExeFileInfo, (u32)WriteData, sizeof(TerminalExeFileInfoStruct) - 2);
			if(TerminalExeFileInfo.Type >= TERMINAL_EXE_FILE_NUM) return ERROR_InvalidData;
			MR((u32)&TerminalExeFileInfo, ADDR_Download_ENTRY, sizeof(TerminalExeFileInfoStruct));
			if(MemCompare((u8*)&TerminalExeFileInfo, WriteData, sizeof(TerminalExeFileInfoStruct) - 2) != 0) return ERROR_InvalidData;
			Temp = pTSysCtrl->CurExeFileCnt;				// ��ǰ�κ�
			Temp2 = WriteData[DataLen[0] - 2];
			Temp2 += WriteData[DataLen[0] - 1] << 8;		// �����κ�
			Temp1 = WriteData[DataLen[0] - 4];				
			Temp1 += WriteData[DataLen[0] - 3] << 8;		// ��ʼ�κ�
			if(Temp1 >= TerminalExeFileInfo.TotalCount) return ERROR_InvalidData;	
			if(Temp2 >= TerminalExeFileInfo.TotalCount) return ERROR_InvalidData;
			if(Temp1 > Temp2) return ERROR_InvalidData;		// ��ʼ�κ� > �����κ�
			if(Temp2 > Temp)								
			{												// �����κ� > ��ǰ�նκ�	
				if(Temp > 470) return ERROR_InvalidData;
				if(Temp1 > Temp)							
				{											// ��ʼ�κ� > ��ǰ�κ�,Ӧ��֡��ʼ�κŲ������
					Temp = Temp2 - Temp1;
				}
				else
				{											// ��ʼ�κ� <= ��ǰ�κ�,Ӧ��֡��ʼ�κ������
					Temp1 = Temp;
					Temp = Temp2 - Temp;
				}
				WriteData[DataLen[0]++] = Temp;
				WriteData[DataLen[0]++] = Temp >> 8;		// �б�����
				for(Temp2 = 0; Temp2 < Temp; Temp2++)
				{
					WriteData[DataLen[0]++] = Temp1;
					WriteData[DataLen[0]++] = Temp1 >> 8;
					Temp1++;
				}
			}
			else
			{			
				WriteData[DataLen[0]++] = 0;				// �����κ� <= �ѽ��նκţ����ѯ��ȫ�����ճɹ�
				WriteData[DataLen[0]++] = 0;
			}		
			pTSysCtrl->CurExeFileType = TerminalExeFileInfo.Type;
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;
	}
	return Result;
}
void NAND_TerminalExeFileUpdate(void)
{
	TerminalExeFileInfoStruct TerminalExeFileInfo;
	u8* p8s, p8d;
	u32* p32;
	os_Type *os;
	ms_Type *ms;
    FILE *file = NULL;
	u32 i = 0, y = 0, contentOffset = 0, ExeFileLen = 0;   
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(pTSysCtrl->CurExeFileReady != TERMINAL_EXE_FILE_READY) return;
	if(pTSysCtrl->CurExeFileType > EXE_FILE_TYPE_PLC) return;
	if((pTSysCtrl->CurExeFilePort != GPRSPORT) && (pTSysCtrl->CurExeFilePort != ENETPORT) && 
	   (pTSysCtrl->CurExeFilePort != RS485_1PORT) && (pTSysCtrl->CurExeFilePort != RS485_2PORT) &&
	   (pTSysCtrl->CurExeFilePort != RS485_3PORT) && (pTSysCtrl->CurExeFilePort != RS232PORT))
	{
		return;
	}
	pTSysCtrl->CurExeFileReady = 0;
	MR((u32)&TerminalExeFileInfo, ADDR_Download_ENTRY, sizeof(TerminalExeFileInfoStruct));
	if(TerminalExeFileInfo.TotalLen > LENmax_Download)
	{	
		WarningOccur(pTSysCtrl->CurExeFilePort, "�����ļ����ȴ���");
		return;
	}
	os = Get_os();
	ms = Get_ms();
	i = os->ffree("N0:");
	if(i < LENmax_Download)
	{//�ռ䲻��	
		os->fdelete("N0:/TEST/",NULL);//ɾ��TEST�ļ���
		i = os->ffree("N0:");
		if(i < LENmax_Download)
		{//�ռ仹����		
			os->fdelete("N0:/Program Files/",NULL);//ɾ��Program Files�ļ���
		}
	}
	p8s = ms->ms_malloc((size_t)LENmax_Download);//��̬�����ҵ��ڴ��
	p8d = ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
	if((p8s == 0)||(p8d == 0))
	{
		ms->Init_my_heap();
		p8s=ms->ms_malloc((size_t)LENmax_Download);//��̬�����ҵ��ڴ��
		p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
	}
	contentOffset = ADDR_Download_ENTRY + sizeof(TerminalExeFileInfoStruct);
	
	i = MRR(contentOffset, 1);
	if(i == 0)
	{//��ѹ��
		ExeFileLen = TerminalExeFileInfo.TotalLen;
		MR((u32)p8s, contentOffset, ExeFileLen);	
		p32 = (u32*)p8s;
		switch(p32[128])//������:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
		{
			case 0:
				file=os->fopen("N0:/Download/MS.bin","w");
				break;
			case 1:
				file=os->fopen("N0:/Download/OS.bin","w");
				break;
			case 2:
				file=os->fopen("N0:/Download/Meter.bin","w");
				break;
			case 3:
				file=os->fopen("N0:/Download/Terminal.bin","w");
				break;
			case 4:
				file=os->fopen("N0:/Download/Menu.bin","w");
				break;
			case 5:
				file=os->fopen("N0:/Download/Font.bin","w");
				break;
			case 7:
				file=os->fopen("N0:/Download/Module.bin","w");
				break;
			default:
				file = NULL;
				break;
		}
		if(file != NULL)
		{
			os->fwrite(p8s, 1, ExeFileLen, file);
			os->fclose(file);
		}
	}
	else
	{//Lzmaѹ��
		MR((u32)p8s, contentOffset, TerminalExeFileInfo.TotalLen);
		i = p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);
		if(i < (2*LENmax_Download))
		{
			y = os->RAM_LzmaDecode((u32)p8s, TerminalExeFileInfo.TotalLen, (u32)p8d, i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
		}	
		if(y != i)
		{
			WarningOccur(pTSysCtrl->CurExeFilePort, "�����ļ����ȴ���");
		}
		else
		{
			ExeFileLen = y;
			p32 = (u32*)p8d;
			switch(p32[128])//������:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
			{
				case 0:
					file=os->fopen("N0:/Download/MS.zip","w");
					break;
				case 1:
					file=os->fopen("N0:/Download/OS.zip","w");
					break;
				case 2:
					file=os->fopen("N0:/Download/Meter.zip","w");
					break;
				case 3:
					file=os->fopen("N0:/Download/Terminal.zip","w");
					break;
				case 4:
					file=os->fopen("N0:/Download/Menu.zip","w");
					break;
				case 5:
					file=os->fopen("N0:/Download/Font.zip","w");
					break;
				case 6:
					file=os->fopen("N0:/Download/PACK.zip","w");
					break;
				case 7:
					file=os->fopen("N0:/Download/Module.zip","w");
					break;
				default:
					file = NULL;
					break;
			}
			if(file != NULL)
			{
				os->fwrite(p8d, 1, ExeFileLen, file);
				os->fclose(file);
			}
		}
	}
	if(file == NULL)
	{
		WarningOccur(pTSysCtrl->CurExeFilePort, "�ļ�·��������");
	}
	
	ms->ms_free(p8s);			//�ͷ��ҵ��ڴ�ѷ���
	ms->ms_free(p8d);			//�ͷ��ҵ��ڴ�ѷ���
	FileDownload->New = 0;		//�����ļ������־:0=��,1=û
}
*/

u32 getTaskFileName(AMR_ENUM DataType, u32 DataID, u32 oldFile)
{
    u32 FileName,i,TempFileName,TempFileCount;
    ms_Type *ms = Get_ms();
    msFILE_TypeDef *msFile= (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
    const LyAMRFileNameRangeInfo* FileRangeInfo = &constAMRFileDef[DataType];
	
    for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
    {
        if((FileName != oldFile) && (msFile[FileName].BLOCK_ENTRY > 0) && (msFile[FileName].FILEID == DataID))
        {//�ҵ�ǰ�����Ѿ�ʹ�õ����ļ�
            ms->msfile_Delete(FileName);
            for(i = FileRangeInfo->FileNameStart; i < FileRangeInfo->FileNameEnd; i++)
            {
                if(msFile[i].BLOCK_ENTRY > 0)
                {
                    msFile[i].FILECOUNT++;
                }
                else
                {
                }
            }
            break;
        }
    }
    if(FileName >= FileRangeInfo->FileNameEnd)
    {//û�ҵ������ҿ��ļ�
        for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
        {
            if(msFile[FileName].BLOCK_ENTRY == 0)
            {
                for(i = FileRangeInfo->FileNameStart; i < FileRangeInfo->FileNameEnd; i++)
                {
                    if(msFile[i].BLOCK_ENTRY > 0)
                    {
                        msFile[i].FILECOUNT++;
                    }
                    else
                    {
                    }
                }
                break;
            }
        }
    }
    if(FileName >= FileRangeInfo->FileNameEnd)
    {//�����ļ�ʹ�ù��ˣ�����ɵ��ļ�
        TempFileName = FileRangeInfo->FileNameStart;
        TempFileCount = msFile[TempFileName].FILECOUNT;
        for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
        {
            if((msFile[FileName].BLOCK_ENTRY>0)&&(msFile[FileName].FILECOUNT >TempFileCount))
            {
                TempFileName = FileName;
                TempFileCount = msFile[FileName].FILECOUNT ;
            }
        }
        FileName = TempFileName;
        ms->msfile_Delete(FileName);
        for(i = FileRangeInfo->FileNameStart; i < FileRangeInfo->FileNameEnd; i++)
        {
            if(msFile[i].BLOCK_ENTRY > 0)
            {
            	msFile[i].FILECOUNT++;
            }
        }
    }
    return checkFile(FileName);
}

u32 checkFile(u32 FileName)
{
    msFILE_TypeDef *msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
    if(msFile[FileName].BLOCK_ENTRY==0)
    {//�ļ������õ�ʱ�������������Ϣ
        msFile[FileName].FILECOUNT=0;
        msFile[FileName].NUM_DATA=0;
        msFile[FileName].NUM_FAIL=0;
        msFile[FileName].FILEID=0;
        MemSet((u8*)msFile[FileName].date_time_s, 0, 7);
    }
    return FileName;
}

u32 getWriteAMRFileName(AMR_ENUM DataType, RealTimeYYStruct * Time)
{
        u32 FileName,TempFileName,TempFileCount,i;
        ms_Type *ms;
        msFILE_TypeDef *msFile;
        const LyAMRFileNameRangeInfo* FileRangeInfo = &constAMRFileDef[DataType];
        ms = Get_ms();
                
        msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
		
        for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
        {/*��ʱ����ͬ���Ǹ��ļ�*/
            if((msFile[FileName].BLOCK_ENTRY>0)&&(cmpDateTime(DataType, msFile[FileName].date_time_s , Time) == cmpEqual))
            {
                break;
            }
        }
        if(FileName >= FileRangeInfo->FileNameEnd)
        {/*���û���ҵ�ʱ����ͬ���ļ������ʾ��ǰ����δд�ļ����ҿ����ļ�*/
             for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
             {
                if(msFile[FileName].BLOCK_ENTRY == 0)
                {//�п����ļ���ʱ�򣬽����������ļ������+ 1
                    for(i=FileRangeInfo->FileNameStart;i<FileRangeInfo->FileNameEnd;i++)
                    {
                       if(msFile[i].BLOCK_ENTRY>0)
                       {
                            msFile[i].FILECOUNT++;
                       }
                       else
                       {
                       }
                    }
                    break;
                }
             }
             if(FileName >= FileRangeInfo->FileNameEnd)
             {/*���û���ҵ����е��ļ�����ʹ����������ļ�*/
                TempFileName=FileRangeInfo->FileNameStart;
                TempFileCount = msFile[TempFileName].FILECOUNT;
                for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
                {
                    if((msFile[FileName].BLOCK_ENTRY>0)&&(msFile[FileName].FILECOUNT >TempFileCount))
                    {
                        TempFileName =FileName;
                        TempFileCount = msFile[FileName].FILECOUNT ;
                    }
                }
                FileName=TempFileName;
                ms->msfile_Delete(FileName);
                for(i=FileRangeInfo->FileNameStart;i<FileRangeInfo->FileNameEnd;i++)
                {
                   if(msFile[i].BLOCK_ENTRY>0)
                   {
                        msFile[i].FILECOUNT++;
                   }
                }
             }
        }
	
    return checkFile(FileName);
	
}


u32 getReadAMRFileName(AMR_ENUM DataType, RealTimeYYStruct* Time)
{
	u32 FileName;
	msFILE_TypeDef *msFile;
	const LyAMRFileNameRangeInfo * FileRangeInfo = &constAMRFileDef[DataType];

	msFile = (msFILE_TypeDef*)(ADDR_msFILEmanage + LEN_msFILEunltFlags);
	
	for(FileName = FileRangeInfo->FileNameStart; FileName < FileRangeInfo->FileNameEnd; FileName++)
	{/*��ʱ����ͬ���Ǹ��ļ�*/
		if((msFile[FileName].BLOCK_ENTRY>0)&&(cmpDateTime(DataType, msFile[FileName].date_time_s, Time) == cmpEqual))
		{
			break;
		}
	}

	if(FileName >= FileRangeInfo->FileNameEnd)
	{
		FileName = 0xFFFFFFFF;
	}
    return FileName;
}

u32 getWriteAlarmEventFileName(AMR_ENUM DataType)
{
	ms_Type *ms;
 	u32 FileName = 0, FileNameStart = 0, FileNameEnd = 0, MaxDeep = 0,TempFileName = 0, TempFileCount = 0, i = 0;
    msFILE_TypeDef *msFile=(msFILE_TypeDef *)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
    const LyAMRFileNameRangeInfo * FileRangeInfo = &constAMRFileDef[DataType];
    ms = Get_ms();
    if(DataType == AMR_ALARM)
    {
        MaxDeep = ALARM_FILE_MAXDEEP;
    }
    else
    {
        MaxDeep = EVENT_FILE_MAXDEEP;
    }
    FileNameStart = FileRangeInfo->FileNameStart;
    FileNameEnd = FileRangeInfo->FileNameEnd;
    for(FileName = FileNameStart; FileName < FileNameEnd; FileName++)
    {//������д���ļ�
        if((msFile[FileName].BLOCK_ENTRY  >0) && (msFile[FileName].NUM_DATA < MaxDeep))
        {
            break;
        }
    }
    if(FileName >= FileNameEnd)
    {//û������д���ļ����ҿյ�
        for(FileName = FileNameStart; FileName < FileNameEnd; FileName++)
        {
            if(msFile[FileName].BLOCK_ENTRY == 0)
            {
                for(i = FileNameStart; i < FileNameEnd; i++)
                {
                    if(msFile[i].BLOCK_ENTRY > 0)
                    {//�����ļ���ʱ�����������ļ���
                        msFile[i].FILECOUNT++ ;
                    }
                }
                break;
            }
        }
    }
    if(FileName >= FileNameEnd)
    {//���û�пյ��ļ���������ɵ��ļ���ɾ��������ʹ��
        TempFileName = FileNameStart;
        TempFileCount = msFile[TempFileName].FILECOUNT;
        for(FileName = FileNameStart; FileName < FileNameEnd; FileName++)
        {
            if(msFile[FileName].FILECOUNT > TempFileCount)
            {
                TempFileCount = msFile[FileName].FILECOUNT;
                TempFileName = FileName;
            }
        }
        FileName = TempFileName;
        ms->msfile_Delete(FileName);
        for(i = FileNameStart; i < FileNameEnd; i++)
        {
            if(msFile[i].BLOCK_ENTRY > 0)
            {//�����ļ���ʱ�����������ļ�����ȼ����� 1
                msFile[i].FILECOUNT++ ;
            }
        }
    }
    return checkFile(FileName);
}

/** �Ƚ�ĳ��ʱ���Ƿ�������ʱ�䷶Χ��
 * ǰ�պ󿪵�����
 *
 *
 */
u32 NAND_CmpDateTimeRange(RealTimeYYStruct *curTime,RealTimeYYStruct *StartTime,RealTimeYYStruct *EndTime)
{
    //u32 cmpResult = 0;
    //u32 i = 0;
    //u8 *src = (u8*)curTime, *dst1 = (u8*)StartTime, *dst2 = (u8*)EndTime;
	if((StartTime->YearH == 0xFF) && (StartTime->YearL == 0xFF) && (StartTime->Month == 0xFF) && 
	   (StartTime->Day == 0xFF) && (StartTime->Hour == 0xFF) && (StartTime->Minute == 0xFF) &&
	   (EndTime->YearH == 0xFF) && (EndTime->YearL == 0xFF) && (EndTime->Month == 0xFF) && 
	   (EndTime->Day == 0xFF) && (EndTime->Hour == 0xFF) && (EndTime->Minute == 0xFF))
	{
		return 1;
	}
	
	if(TimeCompare(AMR_UNKNOWN, curTime, StartTime) == ERROR_TIME_IS_SMALLER) return 0;
	if(TimeCompare(AMR_UNKNOWN, curTime, EndTime) != ERROR_TIME_IS_SMALLER) return 0;
	return 1;
}




/*���ݲ�ͬ�Ķ���Ƚ�ʱ��*/
u32 cmpDateTime(AMR_ENUM dataType, u8* srcTime, RealTimeYYStruct *Time)
{
    u32 cmpResult=cmpEqual;
    u8 *dstTime =(u8*)Time;
    u32 i;
    switch(dataType)
    {
        case AMR_DailyFreeze:
        case AMR_DailyStatisticFreeze:
        case AMR_CurveFreeze:
           /* if((srcTime[0] == Time->YearH) && (srcTime[1] == Time->YearL) && 
			   (srcTime[2] == Time->Month) && (srcTime[3] == Time->Day))
            {
                cmpResult=1;
            }*/
            for(i=0;i<4;i++)
            {
                if(srcTime[i]>dstTime[i])
                {
                    cmpResult=cmpGreaterThan;
                    break;
                }
                else if(srcTime[i]<dstTime[i])
                {
                    cmpResult=cmpLessThan;
                    break;
                }
                else
                {
                    cmpResult=cmpEqual;
                }
            }
    
            
            break;
        case AMR_MonthFreeze:
        case AMR_MonthStatisticFreeze:
           /* if((srcTime[0] == Time->YearH) && (srcTime[1] == Time->YearL) && (srcTime[2] == Time->Month))
            {
                cmpResult=1;
            }*/
            for(i=0;i<3;i++)
            {
                if(srcTime[i]>dstTime[i])
                {
                    cmpResult=cmpGreaterThan;
                    break;
                }
                else if(srcTime[i]<dstTime[i])
                {
                    cmpResult=cmpLessThan;
                    break;
                }
                else
                {
                    cmpResult=cmpEqual;
                }
            }
             break;
        case AMR_NormalTask:
        case AMR_RelayTask:
               for(i=0;i<6;i++)
               {
                    if(srcTime[i]>dstTime[i])
                    {
                        cmpResult=cmpGreaterThan;
                        break;
                    }
                    else if(srcTime[i]<dstTime[i])
                    {
                        cmpResult=cmpLessThan;
                        break;
                    }
                    else
                    {
                        cmpResult=cmpEqual;
                    }
               }
		break;
	default:
		break;
    }
    return cmpResult;
}


void NAND_SyncMeasurePointToRam(void)
{
    MeasurePointStruct  MeasurePoint;
	u16 ImportantUser[40];
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
    MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
    u32 PnNum = 0;
    u32 mpTotal = 0;		//�ܱ���
    u32 mpNormal = 0;		//��ͨ����
    u32 mpSinglePhase = 0;	//�������
    u32 mpMultifun = 0;		//�๦�ܱ���
    u32 mpMaster = 0;		//�ܱ���
    u32 mpImportant = 0;	//�ص����
    u32 FlashLenAddr = 0, FlashLen = 0;
	u32 PLCMeterNum = 0, NewPLCMeters = 0;
	u32 RS485MeterNum = 0, RS485_CH1_Meters = 0, RS485_CH2_Meters = 0, RS485_CH3_Meters = 0;

    if(MeterFileInfo->CountFlag != METER_FILE_SYNC_TO_RAM_FLAG) return;
	if(pTerminalRam->MeterSyncToRam_S_Timer != 0) return;

	MC(0xFF, (u32)ImportantUser, 80);
    for(PnNum = 0; PnNum < MAX_PN_NUM; PnNum++)
    {
        FlashLenAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
        FlashLen = sizeof(MeasurePointStruct) ;
        MR((u32)&MeasurePoint, FlashLenAddr, FlashLen);

		#if ((Project/100)==9)
		if(PnNum == 0)	//err20181008 ��� 0�Ų����� ��ͬ������
        {
        	MC(0, (u32)&NodeList[PnNum], sizeof(NodeListStruct));
			continue;
		}
		if(MeasurePoint.TotalType == 0x01) //err20181017 ��� �ܱ���ͬ�����������ش��봦��
        {
        	MC(0, (u32)&NodeList[PnNum], sizeof(NodeListStruct));
			continue;
		}
		#endif
		
        if(MeasurePoint.State != 0x01)
        {
        	MC(0, (u32)&NodeList[PnNum], sizeof(NodeListStruct));
			continue;
		}

		if(BCD_Check(MeasurePoint.Addr, METER_ADDR_SIZE) != NO_ERROR)
		{
			MC(0, (u32)&NodeList[PnNum], sizeof(NodeListStruct));
			continue;
		}
		if(PnNum != 0)
		{	
		#if (TEST_ON == 0)//err201807xx ̨������жϵ��ܱ�����
			if((MeasurePoint.PointMeterType != METER_TYPE_SINGLE) && (MeasurePoint.PointMeterType != METER_TYPE_MULTI))
			{
				MC(0, (u32)&NodeList[PnNum], sizeof(NodeListStruct));
				continue;
			}
		#endif
	
			if((MeasurePoint.PortID != PORT_ID_RS485_1) && (MeasurePoint.PortID != PORT_ID_RS485_2) && 
			   (MeasurePoint.PortID != PORT_ID_RS485_3) && (MeasurePoint.PortID != PORT_ID_PLC) && 
			   (MeasurePoint.PortID != PORT_ID_WIRELESS))
			{
				MC(0, (u32)&NodeList[PnNum], sizeof(NodeListStruct));
				continue;
			}
		}
		if((MeasurePoint.PortID == PORT_ID_PLC) || (MeasurePoint.PortID == PORT_ID_WIRELESS))
		{
			PLCMeterNum++;
			if(MemCompare(NodeList[PnNum].Addr, MeasurePoint.Addr, METER_ADDR_SIZE) != 0)
			{
				NodeList[PnNum].Flag |= NODE_FLAG_NEED_FILE_SYNC;
				NodeList[PnNum].Flag &= ~NODE_FLAG_SYNC_TO_PLC_OK;
				MeterFileInfo->PLCMeterSyncFlag = METER_FILE_SYNC_TO_PLC_FLAG;
				NewPLCMeters++;
			}
			NodeList[PnNum].PortID = PORT_ID_PLC;
		}
		else
		{
			NodeList[PnNum].PortID = MeasurePoint.PortID;
		}
		MR((u32)NodeList[PnNum].Addr, (u32)MeasurePoint.Addr, METER_ADDR_SIZE);
		MR((u32)NodeList[PnNum].CollectorAddr, (u32)MeasurePoint.CollectorAddr, METER_ADDR_SIZE);		
		NodeList[PnNum].ProtocolType = MeasurePoint.ProtocolType;
		NodeList[PnNum].PointMeterType = MeasurePoint.PointMeterType;
		NodeList[PnNum].UerType = MeasurePoint.PriorityType;
		NodeList[PnNum].PortBaudRate = MeasurePoint.PortBaudRate;
		NodeList[PnNum].PortParity = MeasurePoint.PortParity;
		NodeList[PnNum].PortDataBit = MeasurePoint.PortDataBit;
		NodeList[PnNum].PortStopBit = MeasurePoint.PortStopBit;
		NodeList[PnNum].Effective = MeasurePoint.State;
		NodeList[PnNum].PnNum = (u16)PnNum;		

		if(MeasurePoint.PortID == PORT_ID_RS485_1)
		{
			RS485_CH1_Meters++;
			RS485MeterNum++;
		}
		if(MeasurePoint.PortID == PORT_ID_RS485_2)
		{
			RS485_CH2_Meters++;
			RS485MeterNum++;
		}
		if(MeasurePoint.PortID == PORT_ID_RS485_3)
		{
			RS485_CH3_Meters++;
			RS485MeterNum++;
		}

		mpTotal++;
        if(MeasurePoint.TotalType == 1)
        {
            mpMaster++;
        }
        if(MeasurePoint.PointMeterType == METER_TYPE_MULTI)
        {
            mpMultifun++;
        }
        if(MeasurePoint.PointMeterType == METER_TYPE_SINGLE)
        {
            mpSinglePhase++;
        }
        if(MeasurePoint.PriorityType == USER_TYPE_IMPORTANT)
        {            		
			if(mpImportant < 20)
			{
				ImportantUser[mpImportant++] = hex_bcd(PnNum);
			}
        }
        else
        {
            mpNormal++;
        }
    }
    
    MeterFileInfo->MaxMeters = hex_bcd(PnNum);	//(u16)PnNum;
	MeterFileInfo->TotalMeters = hex_bcd(mpTotal);//(u16)mpTotal;				//err20180404-25 BCDת��
    MeterFileInfo->ImportantMeters = hex_bcd(mpImportant);//(u16)mpImportant;	//err20180404-25 BCDת��
    MeterFileInfo->NormalMeters = hex_bcd(mpNormal);//(u16)mpNormal;			//err20180404-25 BCDת��
	MeterFileInfo->SinglePhaseMeters = hex_bcd(mpSinglePhase);//(u16)mpSinglePhase;		//err20180404-25 BCDת��
    MeterFileInfo->MultifunctionMeters = hex_bcd(mpMultifun);//(u16)mpMultifun;   		//err20180404-25 BCDת��
    MeterFileInfo->OtherMeters = (u16)(mpTotal - mpSinglePhase - mpMultifun);
	if(MeterFileInfo->PLCMeters != (u16)PLCMeterNum)
	{
		MeterFileInfo->PLCMeterSyncFlag = METER_FILE_SYNC_TO_PLC_FLAG;
	}
	MeterFileInfo->PLCMeters = (u16)PLCMeterNum;
	MeterFileInfo->NewPLCMeters = NewPLCMeters;
	MeterFileInfo->RS485Meters = (u16)RS485MeterNum;
	MeterFileInfo->RS485_CH1_Meters = RS485_CH1_Meters;
	MeterFileInfo->RS485_CH2_Meters = RS485_CH2_Meters;
	MeterFileInfo->RS485_CH3_Meters = RS485_CH3_Meters;
	
	MeterFileInfo->NodeListCrc = CCIT16_CRC((u8*)NodeList, MAX_PN_NUM*sizeof(NodeListStruct));
    MeterFileInfo->CountFlag = 0;

	#if ((Project/100)==2 || (Project/100)==9)
		MeterFileInfo->PulseAndPortNum = 0x0600;			// ������: 0�����ж˿�: 6 			//err20181217 RS232/RS485/USB/���ⶼ�Ǵ���
	#endif

	#if ((Project/100)==3)
		MeterFileInfo->PulseAndPortNum = 0x0600;			// ������: 0�����ж˿�: 6 			//err20181217 RS232/RS485/USB/���ⶼ�Ǵ���
	#endif

	#if ((Project/100)==5)
		MeterFileInfo->PulseAndPortNum = 0x0400;			// ������: 0�����ж˿�: 4 			//err20181217 RS232/RS485/USB/���ⶼ�Ǵ���
	#endif
	MW((u32)&MeterFileInfo->PulseAndPortNum, ADDR_E1800002, LEN_E1800002 + LEN_E1800003);		// �ն�����������봮�ж˿�����
	
	MW((u32)&MeterFileInfo->MaxMeters, ADDR_E1800004, LEN_E1800004);				// д�ն�֧������������
	
	MW((u32)&MeterFileInfo->TotalMeters, ADDR_E1800005, LEN_E1800005);				// д��Ч�ܱ���
	MW((u32)&MeterFileInfo->NormalMeters, ADDR_E1800006, LEN_E1800006);				// д��ͨ�û��ܱ���
	MW((u32)&MeterFileInfo->MultifunctionMeters, ADDR_E1800007, LEN_E1800007);		// д�๦���ܱ���
	MW((u32)&MeterFileInfo->ImportantMeters, ADDR_E1800008, LEN_E1800008);			// д�ص��û��ܱ���

	MW((u32)ImportantUser, ADDR_E1800020, LEN_E1800020);							// д�ص��û��������
}


ResultEnum SaveTerminalPowerDownFreezData(AMR_ENUM AmrEnum,RealTimeStruct* RealTime)
{
	
	RealTimeYYStruct YYTime;
	
	u32 TotalDataLen = 0;
	u32 Temp1 = 0,Temp2 = 0;
	u32 NW_DI_Temp1 = 0,  NW_DI_Temp2 = 0;
	u8 Date_Buf[20];
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;

	switch(AmrEnum)
	{
		case AMR_DailyStatisticFreeze:
			MR((u32)&Temp1, QCSG_POWER_DOWN_NUM_DAY, 4);
			MR((u32)&Temp2, QCSG_POWER_DOWN_TIME_DAY, 4);
			NW_DI_Temp1 = 0xE1008030;
			NW_DI_Temp2 = 0xE1008031;
			break;
		case AMR_MonthStatisticFreeze:
			MR((u32)&Temp1, QCSG_POWER_DOWN_NUM_MONTH, 4);
			MR((u32)&Temp2, QCSG_POWER_DOWN_TIME_MONTH, 4);
			NW_DI_Temp1 = 0xE100C030;
			NW_DI_Temp2 = 0xE100C031;
			break;
		default:
			AmrEnum = AMR_UNKNOWN;
			break;
	}
	if(AmrEnum == AMR_UNKNOWN ) return ERROR_NoSuchDataID;
	
	Date_Buf[TotalDataLen++] = NW_DI_Temp1;		//���ݱ�ʶDI
	Date_Buf[TotalDataLen++] = NW_DI_Temp1 >> 8;
	Date_Buf[TotalDataLen++] = NW_DI_Temp1 >> 16;
	Date_Buf[TotalDataLen++] = NW_DI_Temp1 >> 24;	
	
	Date_Buf[TotalDataLen++] = 3;
	Date_Buf[TotalDataLen++] = 3 >> 8;	//�������ݳ��� 3

	Temp1 = hex_bcd(Temp1);					//err20181213 ��������ֱ�Ӱ�BCD�洢
	Date_Buf[TotalDataLen++] = Temp1;		//��������
	Date_Buf[TotalDataLen++] = Temp1 >> 8;
	Date_Buf[TotalDataLen++] = Temp1 >> 16;

	Date_Buf[TotalDataLen++] = NW_DI_Temp2; 	//���ݱ�ʶDI
	Date_Buf[TotalDataLen++] = NW_DI_Temp2 >> 8;
	Date_Buf[TotalDataLen++] = NW_DI_Temp2 >> 16;
	Date_Buf[TotalDataLen++] = NW_DI_Temp2 >> 24;	
	
	Date_Buf[TotalDataLen++] = 3;
	Date_Buf[TotalDataLen++] = 3 >> 8;	//�������ݳ��� 3

	Temp2 = hex_bcd(Temp2);					//err20181213 ��������ֱ�Ӱ�BCD�洢
	Date_Buf[TotalDataLen++] = Temp2;		//��������
	Date_Buf[TotalDataLen++] = Temp2 >> 8;
	Date_Buf[TotalDataLen++] = Temp2 >> 16;


	RealTimeToYYTime(RealTime, &YYTime);
	YYTime.Ack = 0;
	YYTime.Report = 0;
	
	NAND_WriteMeterAMRData(AmrEnum, NodeList[0].Addr, METER_TYPE_OTHER ,NW_DI_Temp2, &YYTime, (u8*)Date_Buf, &TotalDataLen);
	
	return NO_ERROR;
}

