/*************************************************
版  权:正泰仪表智能终端部
文  件:Terminal_Task.c
作  者:zhaoyun
版  本:1.00
日  期:20170720
描  述:终端任务接口
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

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../LIB/Ethernet.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Head/QCSG_AFN12.h"
#include "../QCSG_Head/QCSG_AFN13.h"
#include "../QCSG_Uart/Terminal_ReadMeter_ACSample.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"
#include "Terminal_Task.h"

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



void TerminalReportTask(u32 PORTn)
{
	ReportTaskStruct* ReportTask = NULL;

	if((PORTn != GPRSPORT) && (PORTn != ENETPORT)) return;
	
	ReportTask = (ReportTaskStruct*)GetTerminalReportTaskAddr(PORTn);
	if(ReportTask == NULL) return;
	if(HAL_GetPortLinkState(PORTn) == 0)
	{
		ReportTask->State = REPORT_IDLE;
		return;
	}
	if(((u8)PORTn == GPRSPORT) && (HAL_GetPortLinkState(ENETPORT) != 0) && (ReportTask->State == REPORT_IDLE))
	{
		ReportTask->State = REPORT_IDLE;
		return;
	}
	switch(ReportTask->State)
	{
		case REPORT_IDLE:
			ReportTaskIdleStateProcess(PORTn, ReportTask);
			break;
		case REPORT_READ_DATA:
			ReportTaskReadDataProcess(PORTn, ReportTask);
			break;
		case REPORT_TX:
			ReportTaskTxProcess(PORTn, ReportTask);
			break;
		case REPORT_WAIT_ACK:
			ReportTaskWaitAckProcess(PORTn, ReportTask);
			break;
		default:
			ReportTask->State = REPORT_IDLE;
			break;
	}
}


ResultEnum ReportTaskIdleStateProcess(u32 PORTn, ReportTaskStruct* ReportTask)
{
	RealTimeStruct CurTime;
	u32 CurTicks = 0;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	CurTicks = HAL_GetSystemTick();
	if((CurTicks - ReportTask->DelayStartTick) < REPORT_DELAY_TICKS) return ERROR_WRONG_TIME;
	
	if(pTSysCtrl->LatelyAlarmNum > 0)
	{
		//ReportTask->LatelyAlarmNum = pTSysCtrl->LatelyAlarmNum;
		ReportTask->ID |= REPORT_ID_ALARM;
	}
	else
	{
		HAL_GetSystemTime(&CurTime);
		if(ReportTask->CheckMinute == CurTime.u8Minute) 
			return ERROR_WRONG_TIME;
		ReportTask->CheckMinute = CurTime.u8Minute;
		TerminalReportCheckNormalTask(ReportTask, &CurTime);
		TerminalReportCheckRelayTask(ReportTask, &CurTime);
	}
	if(ReportTask->ID == 0) return ERROR_WRONG_ACCESS;
	ReportTask->Step = 0x01;	
	if(GetNextReportStep(ReportTask) != NO_ERROR)
	{
		ReportTask->ID = 0;
		ReportTask->State = REPORT_IDLE;
		return ERROR_WRONG_ACCESS;
	}
	ReportTask->State = REPORT_READ_DATA;
	return NO_ERROR;
}

ResultEnum ReportTaskReadDataProcess(u32 PORTn, ReportTaskStruct* ReportTask)
{
	ResultEnum Result = NO_ERROR;
	u32 CurTicks = 0;
	CurTicks = HAL_GetSystemTick();
	if((CurTicks - ReportTask->DelayStartTick) < REPORT_DELAY_TICKS) return ERROR_WRONG_TIME;
	switch(ReportTask->Step)
	{
		case REPORT_ID_NORMAL_TASK:
			Result = ReportTaskReadNormalTaskData(PORTn, ReportTask);
			break;
		case REPORT_ID_RELAY_TASK:
			Result = ReportTaskReadRelayTaskData(PORTn, ReportTask);
			break;
		case REPORT_ID_ALARM:
			Result = ReportTaskReadAlarmData(PORTn, ReportTask);
			break;
		default:
			ReportTask->ID = 0;
			ReportTask->Step = 0;
			ReportTask->State = REPORT_IDLE;	
			break;
	}
	return Result;
}


ResultEnum ReportTaskTxProcess(u32 PORTn, ReportTaskStruct* ReportTask)
{
	ResultEnum Result = NO_ERROR;
	switch(ReportTask->Step)
	{
		case REPORT_ID_NORMAL_TASK:
		case REPORT_ID_RELAY_TASK:		
		case REPORT_ID_ALARM:
			Result = ReportTaskTxData(PORTn, ReportTask);
			break;
		default:
			ReportTask->ID = 0;
			ReportTask->Step = 0;
			ReportTask->State = REPORT_IDLE;	
			break;
	}
	return Result;
}

ResultEnum ReportTaskWaitAckProcess(u32 PORTn, ReportTaskStruct* ReportTask)
{
	u32 CurTicks = 0, ReportState = 0, ReportDst = 0;
	CurTicks = HAL_GetSystemTick();
	
	if(((ReportTask->Flag & (REPORT_FLAG_SEND_OK | REPORT_FLAG_SEND_FAIL)) == 0) && 
	   ((CurTicks - ReportTask->SendStartTick) < REPORT_ACK_TIME_OUT_TICK))	//等待发送完，时间太短会返回
	{
		return ERROR_WRONG_TIME;
	}
	
	if(PORTn == GPRSPORT)
	{
		if((ReportTask->Flag & REPORT_FLAG_SEND_OK) != 0)
		{
			ReportState |= REPORT_CH_GPRS;	
		}
		ReportDst |= REPORT_CH_GPRS;
	}
	if(PORTn == ENETPORT)
	{
		if((ReportTask->Flag & REPORT_FLAG_SEND_OK) != 0)
		{
			ReportState |= REPORT_CH_ETHE;	//发送成功
		}
		ReportDst |= REPORT_CH_ETHE;	//发送目标
	}

	ReportTask->Flag &= ~(REPORT_FLAG_SEND_OK | REPORT_FLAG_SEND_FAIL);//该标志已完成使命
	switch(ReportTask->Step)
	{
		case REPORT_ID_NORMAL_TASK:
		case REPORT_ID_RELAY_TASK:
			ReportTask->Index++;
			ReportTask->State = REPORT_READ_DATA;
			ReportTask->DelayStartTick = CurTicks;
			break;
		case REPORT_ID_ALARM:
			NAND_WriteAlarmRecordReportState(&ReportTask->AlarmHead, ReportTask->FileName, ReportDst, ReportState);
			TerminalReportAlarmDec();//去读取下一个告警
			ReportTask->ID = 0;
			ReportTask->Step = 0;
			ReportTask->State = REPORT_IDLE;
			ReportTask->DelayStartTick = CurTicks;
			break;
		default:
			ReportTask->ID = 0;
			ReportTask->Step = 0;
			ReportTask->State = REPORT_IDLE;	
			break;
	}
	return NO_ERROR;
}

ResultEnum ReportTaskReadNormalTaskData(u32 PORTn, ReportTaskStruct* ReportTask)
{
	NormalTaskParaStruct NormalTaskPara;
	CommunicationParaStruct CommPara;
	#if (TEST_ON == 1)//err20190107 下方导致了任务上报时间点混乱的问题。暂时修改： (TEST_ON == 1) -> (TEST_ON == 0)
		RealTimeYYStruct AMRTimePoint;
	#endif
	RealTimeStruct* LastReportTime = (RealTimeStruct*)ADDR_LastNormalTaskReportTime;
	u32 TempLen = 0, TotalPnListNum = 0, Period = 0, PeriodTime = 0;

	u8* ReportDataBuf = (u8*)ADDR_ReportDataBuf;
	if(GetNextNormalTaskToRead(ReportTask) != NO_ERROR)
	{
		ReportTask->Step <<= 1;
		if(GetNextReportStep(ReportTask) != NO_ERROR)
		{
			ReportTask->ID = 0;
			ReportTask->Step = 0;
			ReportTask->State = REPORT_IDLE;
		}
		else
		{
			ReportTask->State = REPORT_READ_DATA;
		}
		return ERROR_WRONG_ACCESS;
	}
	TempLen = ReportTask->Index;
	TempLen *= sizeof(NormalTaskParaStruct);
	MR((u32)&NormalTaskPara, ADDR_NormalTaskPara + TempLen, sizeof(NormalTaskParaStruct));

	if((NormalTaskPara.ReportPeriodUnit > QCSG_SAMPLE_UNIT_MONTH) || 
	   (NormalTaskPara.SamplePeriodUnit > QCSG_SAMPLE_UNIT_MONTH))
	{
		ReportTask->Index++;
		ReportTask->State = REPORT_READ_DATA;
		return ERROR_WRONG_ACCESS;
	}
	
	ReportTask->QCSGFp.CurDataID = 0xE0000301 + (ReportTask->Index & 0x00FF);
	RealTimeToYYTime(&LastReportTime[ReportTask->Index], &ReportTask->QCSGFp.StartTime);
	RealTimeToYYTime(&LastReportTime[ReportTask->Index], &ReportTask->QCSGFp.EndTime);
	switch(NormalTaskPara.ReportPeriodUnit)
	{
		case QCSG_SAMPLE_UNIT_MINUTE:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;	
			break;
		case QCSG_SAMPLE_UNIT_HOUR:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_HOUR;
			break;
		case QCSG_SAMPLE_UNIT_DAY:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_DAY;
			break;
		case QCSG_SAMPLE_UNIT_MONTH:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MONTH;
			break;
		default:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;
			break;
	}
	PeriodTime = NormalTaskPara.ReportPeriod;
	for(Period = 0; Period < PeriodTime; Period++)
	{
		DecTimeByDataDensity(&ReportTask->QCSGFp.StartTime, ReportTask->QCSGFp.DataDensity);
	}
	#if (TEST_ON == 1)//err20190107 下面的代码导致了任务上报时间点混乱的问题。不确定为何要这样做？
	//err20190417 经台体测试发现。此处代码是为了过 日冻结 月冻结 台体测试项。
	//#if (TEST_ON == 0)	//暂时进行修改：(TEST_ON == 1) -> (TEST_ON == 0)
		switch(NormalTaskPara.SamplePeriodUnit)
		{
			case QCSG_SAMPLE_UNIT_MINUTE:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;	
				break;
			case QCSG_SAMPLE_UNIT_HOUR:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_HOUR;
				break;
			case QCSG_SAMPLE_UNIT_DAY:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_DAY;
				break;
			case QCSG_SAMPLE_UNIT_MONTH:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MONTH;
				break;
			default:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;
				break;
		}
		RealTimeToYYTime(&NormalTaskPara.SampleTime, &AMRTimePoint);
		PeriodTime = NormalTaskPara.SamplePeriod;
		while(TimeCompare(AMR_NormalTask, &AMRTimePoint, &ReportTask->QCSGFp.StartTime) == ERROR_TIME_IS_SMALLER)
		{	
			for(Period = 0; Period < PeriodTime; Period++)
			{
				AddTimeByDataDensity(&AMRTimePoint, ReportTask->QCSGFp.DataDensity);
			}
		}
		MR((u32)&ReportTask->QCSGFp.StartTime, (u32)&AMRTimePoint, sizeof(RealTimeYYStruct));
		if((ReportTask->QCSGFp.DataDensity == QCSG_DATA_DENSITY_1_DAY) || 
			(ReportTask->QCSGFp.DataDensity == QCSG_DATA_DENSITY_1_MONTH)) //err20190418 过台体
			{
				ReportTask->QCSGFp.StartTime.Minute = 0x00;
			}
	#endif
	ReportTask->QCSGFp.StartTime.Ack = 0;
	ReportTask->QCSGFp.StartTime.Report = 0;
	ReportTask->QCSGFp.DataDensity = 0;
	TempLen = QCSG_AFN12_GetNormalTaskData(&ReportTask->QCSGFp, &NormalTaskPara, ReportDataBuf, &TotalPnListNum, LEN_ReportDataBuf);
	if(TempLen == 0) 
	{
		ReportTask->Index++;
		ReportTask->State = REPORT_READ_DATA;
		return ERROR_WRONG_ACCESS;
	}
	if(PORTn == GPRSPORT)
	{
		ReportTask->SendReq.SrcTask = TASK_ID_GPRS_Report;
	}
	else if(PORTn == ENETPORT)
	{
		ReportTask->SendReq.SrcTask = TASK_ID_ETHE_Report;
	}
	else
	{
		ReportTask->SendReq.SrcTask = TASK_ID_NONE;
	}
	ReportTask->SendReq.RetryCnt = REPORT_MAX_RETRY_CNT;
	ReportTask->SendReq.FrameType = FRAME_TYPE_QCSG_UP;
	ReportTask->SendReq.DataType = DATA_TYPE_STRUCT;
	ReportTask->SendReq.AckToSrcTask = 0x01;
	ReportTask->SendReq.NeedAck = 0x01;

	MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
	ReportTask->SendReq.QCSGFrameInfo.A1 = CommPara.TerminalLocationCode;
	ReportTask->SendReq.QCSGFrameInfo.A2 = CommPara.TerminalAddr;
	ReportTask->SendReq.QCSGFrameInfo.A3 = 0;
	ReportTask->SendReq.QCSGFrameInfo.FC = QCSG_FC_DIR_BIT | QCSG_FC_PRM_BIT | QCSG_FC_PRM1_F11;
	ReportTask->SendReq.QCSGFrameInfo.AFN = QCSG_AFN_12;
	ReportTask->SendReq.QCSGFrameInfo.SEQ = GetUart_QCSG_SEQ(PORTn);
	ReportTask->SendReq.QCSGFrameInfo.Port = PORTn;
	ReportTask->SendReq.QCSGFrameInfo.AmrEnum = (u8)AMR_NormalTask;
	ReportTask->SendReq.PnListNum = TotalPnListNum;
	ReportTask->SendReq.WaitAckTimeOut = REPORT_ACK_TIME_OUT;
	ReportTask->SendReq.DataLen = TempLen;
	ReportTask->SendReq.DataBuf = (u8*)ADDR_ReportDataBuf;
	
	ReportTask->State = REPORT_TX;
	return NO_ERROR;
}

ResultEnum ReportTaskReadRelayTaskData(u32 PORTn, ReportTaskStruct* ReportTask)
{
	RelayTaskParaStruct RelayTaskPara;
	CommunicationParaStruct CommPara;
	#if (TEST_ON == 1)
		RealTimeYYStruct AMRTimePoint;
	#endif
	RealTimeStruct* LastReportTime = (RealTimeStruct*)ADDR_LastRelayTaskReportTime;
	u32 TempLen = 0, TotalPnListNum = 0, Period = 0, PeriodTime = 0;
	u8* ReportDataBuf = (u8*)ADDR_ReportDataBuf;
	if(GetNextRelayTaskToRead(ReportTask) != NO_ERROR)
	{
		ReportTask->Step <<= 1;
		if(GetNextReportStep(ReportTask) != NO_ERROR)
		{
			ReportTask->ID = 0;
			ReportTask->Step = 0;
			ReportTask->State = REPORT_IDLE;
		}
		else
		{
			ReportTask->State = REPORT_READ_DATA;
		}
		return ERROR_WRONG_ACCESS;
	}
	TempLen = ReportTask->Index;
	TempLen *= sizeof(RelayTaskParaStruct);
	MR((u32)&RelayTaskPara, ADDR_RelayTaskPara + TempLen, sizeof(RelayTaskParaStruct));

	if((RelayTaskPara.ReportPeriodUnit > QCSG_SAMPLE_UNIT_MONTH) || 
	   (RelayTaskPara.SamplePeriodUnit > QCSG_SAMPLE_UNIT_MONTH))
	{
		ReportTask->Index++;
		ReportTask->State = REPORT_READ_DATA;
		return ERROR_WRONG_ACCESS;
	}
	ReportTask->QCSGFp.CurDataID = 0xE0000401 + (ReportTask->Index & 0x00FF);
	RealTimeToYYTime(&LastReportTime[ReportTask->Index], &ReportTask->QCSGFp.StartTime);
	RealTimeToYYTime(&LastReportTime[ReportTask->Index], &ReportTask->QCSGFp.EndTime);
	switch(RelayTaskPara.ReportPeriodUnit)
	{
		case QCSG_SAMPLE_UNIT_MINUTE:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;	
			break;
		case QCSG_SAMPLE_UNIT_HOUR:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_HOUR;
			break;
		case QCSG_SAMPLE_UNIT_DAY:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_DAY;
			break;
		case QCSG_SAMPLE_UNIT_MONTH:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MONTH;
			break;
		default:
			ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;
			break;
	}
	PeriodTime = RelayTaskPara.ReportPeriod;
	for(Period = 0; Period < PeriodTime; Period++)
	{
		DecTimeByDataDensity(&ReportTask->QCSGFp.StartTime, ReportTask->QCSGFp.DataDensity);
	}

	#if (TEST_ON == 1)
		switch(RelayTaskPara.SamplePeriodUnit)
		{
			case QCSG_SAMPLE_UNIT_MINUTE:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;	
				break;
			case QCSG_SAMPLE_UNIT_HOUR:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_HOUR;
				break;
			case QCSG_SAMPLE_UNIT_DAY:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_DAY;
				break;
			case QCSG_SAMPLE_UNIT_MONTH:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MONTH;
				break;
			default:
				ReportTask->QCSGFp.DataDensity = QCSG_DATA_DENSITY_1_MINUTE;
				break;
		}
		RealTimeToYYTime(&RelayTaskPara.SampleTime, &AMRTimePoint);
		PeriodTime = RelayTaskPara.SamplePeriod;
		while(TimeCompare(AMR_RelayTask, &AMRTimePoint, &ReportTask->QCSGFp.StartTime) == ERROR_TIME_IS_SMALLER)
		{	
			for(Period = 0; Period < PeriodTime; Period++)
			{
				AddTimeByDataDensity(&AMRTimePoint, ReportTask->QCSGFp.DataDensity);
			}
		}
		MR((u32)&ReportTask->QCSGFp.StartTime, (u32)&AMRTimePoint, sizeof(RealTimeYYStruct));
		ReportTask->QCSGFp.StartTime.Ack = 0;
		ReportTask->QCSGFp.StartTime.Report = 0;
	#endif
	ReportTask->QCSGFp.DataDensity = 0;
	TempLen = QCSG_AFN12_GetRelayTaskData(&ReportTask->QCSGFp, &RelayTaskPara, ReportDataBuf, &TotalPnListNum, LEN_ReportDataBuf);

	if(TempLen == 0) 
	{
		ReportTask->Index++;
		ReportTask->State = REPORT_READ_DATA;
		return ERROR_WRONG_ACCESS;
	}
	if(PORTn == GPRSPORT)
	{
		ReportTask->SendReq.SrcTask = TASK_ID_GPRS_Report;
	}
	else if(PORTn == ENETPORT)
	{
		ReportTask->SendReq.SrcTask = TASK_ID_ETHE_Report;
	}
	else
	{
		ReportTask->SendReq.SrcTask = TASK_ID_NONE;
	}
	ReportTask->SendReq.RetryCnt = REPORT_MAX_RETRY_CNT;
	ReportTask->SendReq.FrameType = FRAME_TYPE_QCSG_UP;
	ReportTask->SendReq.DataType = DATA_TYPE_STRUCT;
	ReportTask->SendReq.AckToSrcTask = 0x01;
	ReportTask->SendReq.NeedAck = 0x01;

	MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
	ReportTask->SendReq.QCSGFrameInfo.A1 = CommPara.TerminalLocationCode;
	ReportTask->SendReq.QCSGFrameInfo.A2 = CommPara.TerminalAddr;
	ReportTask->SendReq.QCSGFrameInfo.A3 = 0;
	ReportTask->SendReq.QCSGFrameInfo.FC = QCSG_FC_DIR_BIT | QCSG_FC_PRM_BIT | QCSG_FC_PRM1_F11;
	ReportTask->SendReq.QCSGFrameInfo.AFN = QCSG_AFN_12;
	ReportTask->SendReq.QCSGFrameInfo.SEQ = GetUart_QCSG_SEQ(PORTn);
	ReportTask->SendReq.QCSGFrameInfo.Port = PORTn;
	ReportTask->SendReq.QCSGFrameInfo.AmrEnum = (u8)AMR_RelayTask;
	ReportTask->SendReq.PnListNum = TotalPnListNum;
	ReportTask->SendReq.WaitAckTimeOut = REPORT_ACK_TIME_OUT;
	ReportTask->SendReq.DataLen = TempLen;
	ReportTask->SendReq.DataBuf = (u8*)ADDR_ReportDataBuf;
	
	ReportTask->State = REPORT_TX;
	return NO_ERROR;
}

ResultEnum ReportTaskReadAlarmData(u32 PORTn, ReportTaskStruct* ReportTask)
{
	CommunicationParaStruct CommPara;
	u8 TempBuf[SIZE_512];
	u32 DataLen = 0, DIIndex = 0, TotalPnDataLen = 0, LenPos = 0, TempPn = 0;
	u8* ReportDataBuf = (u8*)ADDR_ReportDataBuf;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	ResultEnum Result = NO_ERROR;
	
	ReportTask->LatelyAlarmNum = pTSysCtrl->LatelyAlarmNum;
	ReportTask->FileName = 0;
	Result = NAND_ReadAlarmRecordByLatelyNum(ReportTask->LatelyAlarmNum, &ReportTask->AlarmHead, TempBuf, &DataLen, &ReportTask->FileName);
	if((Result != NO_ERROR) || (ReportTask->FileName < ALARM_FILE_START) || 
	   (ReportTask->FileName >= ALARM_FILE_END) || (ReportTask->AlarmHead.OcurTime.Ack != 0))
	{
		TerminalReportAlarmDec();
		ReportTask->ID = 0;
		ReportTask->Step = 0;
		ReportTask->State = REPORT_IDLE;
		return ERROR_WRONG_ACCESS;
	}
	DIIndex = FindDIInAlarmDIInfoTbl(ReportTask->AlarmHead.DataID);
	if((DIIndex >= ALARM_DI_NUM) || (ReportTask->AlarmHead.contentLen != AlarmDIInfoTbl[DIIndex].StoreLen))
	{
		TerminalReportAlarmDec();
		ReportTask->ID = 0;
		ReportTask->Step = 0;
		ReportTask->State = REPORT_IDLE;
		return ERROR_WRONG_ACCESS;
	}

	//err20180519 - 取消终端掉上电告警的地址判断，防止未设置测量点0地址导致告警无法上报。 20180525 在产生时就使用RAM中地址，故无需处理。
	//if((ReportTask->AlarmHead.DataID == 0xE2000033) || (ReportTask->AlarmHead.DataID == 0xE2000034))
	//	TempPn = 0;
	//else 
#if ((Project/100) == 9)//err20181023 配变 由于总表未同步至RAM中故测量点号从FLASH中搜索
//err20181030 如果是集中器终端告警，则直接设置为0 否则无法从FLASH中搜到表地址，无法正常上报。(停上电，通信流量越线，抄表失败)
	if((ReportTask->AlarmHead.DataID == 0xE2000033) || 
	   (ReportTask->AlarmHead.DataID == 0xE2000034) || 
	   (ReportTask->AlarmHead.DataID == 0xE200003A) ||
	   (ReportTask->AlarmHead.DataID == 0xE200003D))
			TempPn = 0;
	else
	TempPn = FindMeterInMP(ReportTask->AlarmHead.MeterAddr,MAX_PN_NUM);
#else
	TempPn = FindMeterInNodeList(ReportTask->AlarmHead.MeterAddr, (NodeListStruct*)ADDR_NodeList, MAX_PN_NUM);
#endif
	if(TempPn >= MAX_PN_NUM)
	{
		TerminalReportAlarmDec();
		ReportTask->ID = 0;
		ReportTask->Step = 0;
		ReportTask->State = REPORT_IDLE;
		return ERROR_WRONG_ACCESS;
	}
	TempPn = SwitchNumToPn(TempPn);
	ReportDataBuf[TotalPnDataLen++] = TempPn;
	ReportDataBuf[TotalPnDataLen++] = TempPn >> 8;
	LenPos = TotalPnDataLen;
	ReportDataBuf[TotalPnDataLen++] = 0;//2字节留给长度
	ReportDataBuf[TotalPnDataLen++] = 0;
	
	ReportDataBuf[TotalPnDataLen++] = ReportTask->AlarmHead.DataID;
	ReportDataBuf[TotalPnDataLen++] = ReportTask->AlarmHead.DataID >> 8;
	ReportDataBuf[TotalPnDataLen++] = ReportTask->AlarmHead.DataID >> 16;
	ReportDataBuf[TotalPnDataLen++] = ReportTask->AlarmHead.DataID >> 24;

	DataLen = SetAlarmEventDIDataByDIInfo(&ReportDataBuf[TotalPnDataLen], TempBuf, &AlarmDIInfoTbl[DIIndex]);
	TotalPnDataLen += DataLen;
	ReportDataBuf[LenPos] = DataLen;
	ReportDataBuf[LenPos + 1] = DataLen >> 8;

	if(PORTn == GPRSPORT)
	{
		ReportTask->SendReq.SrcTask = TASK_ID_GPRS_Report;
	}
	else if(PORTn == ENETPORT)
	{
		ReportTask->SendReq.SrcTask = TASK_ID_ETHE_Report;
	}
	else
	{
		ReportTask->SendReq.SrcTask = TASK_ID_NONE;
	}
	ReportTask->SendReq.RetryCnt = REPORT_MAX_RETRY_CNT;
	ReportTask->SendReq.FrameType = FRAME_TYPE_QCSG_UP;
	ReportTask->SendReq.DataType = DATA_TYPE_STRUCT;
	ReportTask->SendReq.AckToSrcTask = 0x01;
	ReportTask->SendReq.NeedAck = 0x01;

	MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
	ReportTask->SendReq.QCSGFrameInfo.A1 = CommPara.TerminalLocationCode;
	ReportTask->SendReq.QCSGFrameInfo.A2 = CommPara.TerminalAddr;
	ReportTask->SendReq.QCSGFrameInfo.A3 = 0;
	ReportTask->SendReq.QCSGFrameInfo.FC = QCSG_FC_DIR_BIT | QCSG_FC_PRM_BIT | QCSG_FC_PRM1_F11;
	ReportTask->SendReq.QCSGFrameInfo.AFN = QCSG_AFN_13;
	ReportTask->SendReq.QCSGFrameInfo.SEQ = GetUart_QCSG_SEQ(PORTn);
	ReportTask->SendReq.QCSGFrameInfo.Port = PORTn;
	ReportTask->SendReq.QCSGFrameInfo.AmrEnum = (u8)AMR_ALARM;
	ReportTask->SendReq.PnListNum = 1;
	ReportTask->SendReq.WaitAckTimeOut = REPORT_ACK_TIME_OUT;
	ReportTask->SendReq.DataLen = TotalPnDataLen;
	ReportTask->SendReq.DataBuf = (u8*)ADDR_ReportDataBuf;
	
	ReportTask->State = REPORT_TX;
	return NO_ERROR;
}
ResultEnum ReportTaskTxData(u32 PORTn, ReportTaskStruct* ReportTask)
{
	MultiFrameSendTaskStruct* SendTask = NULL;
	ResultEnum Result = NO_ERROR;
	SendTask = (MultiFrameSendTaskStruct*)GetUartMultiFrameSendTaskAddr(PORTn);

	if(SendTask == NULL)
	{
		ReportTask->State = REPORT_IDLE;
		return ERROR_POINTER_NULL;
	}
	Result = QCSGMultiFrameSendReq(PORTn, &ReportTask->SendReq);

	if(Result != NO_ERROR) return ERROR_WRONG_ACCESS;

	ReportTask->State = REPORT_WAIT_ACK;
	ReportTask->SendStartTick = HAL_GetSystemTick();
	return NO_ERROR;
}

ResultEnum GetNextNormalTaskToRead(ReportTaskStruct* ReportTask)
{
	while(ReportTask->Index < MAX_NORMAL_TASK_NUM)
	{
		if(ReportTask->NormalTaskReportExe[ReportTask->Index] != 0) break;
		ReportTask->Index++;
	}
	if(ReportTask->Index >= MAX_NORMAL_TASK_NUM) return ERROR_WRONG_ACCESS;
	return NO_ERROR;
}

ResultEnum GetNextRelayTaskToRead(ReportTaskStruct* ReportTask)
{
	while(ReportTask->Index < MAX_RELAY_TASK_NUM)
	{
		if(ReportTask->RelayTaskReportExe[ReportTask->Index] != 0) break;
		ReportTask->Index++;
	}
	if(ReportTask->Index >= MAX_RELAY_TASK_NUM) return ERROR_WRONG_ACCESS;
	return NO_ERROR;
}


ResultEnum GetNextReportStep(ReportTaskStruct* ReportTask)
{
	if(ReportTask->ID == 0) return ERROR_WRONG_ACCESS;
	while(ReportTask->Step <= REPORT_ID_ALARM)
	{
		if((ReportTask->Step & ReportTask->ID) != 0) break;
		ReportTask->Step <<= 1;
	}

	if(ReportTask->Step > REPORT_ID_ALARM)  return ERROR_WRONG_ACCESS;
	ReportTask->Index = 0;
	ReportTask->Flag = 0;
	return NO_ERROR;
}

void TerminalReportCheckNormalTask(ReportTaskStruct* ReportTask, RealTimeStruct* CurTime)
{
	NormalTaskParaStruct NormalTaskPara;
	u32 i = 0;
	RealTimeStruct* LastReportTime = (RealTimeStruct*)ADDR_LastNormalTaskReportTime;
	
	for(i = 0; i < MAX_NORMAL_TASK_NUM; i++)
	{
		MR((u32)&NormalTaskPara, ADDR_NormalTaskPara + i*sizeof(NormalTaskParaStruct), sizeof(NormalTaskParaStruct));
		if(NormalTaskPara.Flag != 0x01) continue;
		if(NormalTaskPara.DANum == 0) continue;
		if(NormalTaskPara.DataIDNum == 0) continue;
		if(NormalTaskPara.DataIDNum > AMR_MAX_DATA_ID) continue;
		if(NormalTaskPara.ReportPeriod == 0) continue;
		if(NormalTaskPara.ReportPeriodUnit > QCSG_SAMPLE_UNIT_MONTH) continue;
		if(NormalTaskPara.GetRate == 0) continue;
		if(TimeValidCheck(&LastReportTime[i]) != NO_ERROR)
		{
			MR((u32)&LastReportTime[i], (u32)CurTime, sizeof(RealTimeStruct));
		}
		ReportTask->NormalTaskReportExe[i] = QCSG_Judge_TaskTiming(&LastReportTime[i], CurTime, &NormalTaskPara.ReportTime, NormalTaskPara.ReportPeriod, NormalTaskPara.ReportPeriodUnit);
		if(ReportTask->NormalTaskReportExe[i] != 0) 
		{
			MR((u32)&LastReportTime[i], (u32)CurTime, sizeof(RealTimeStruct));
			ReportTask->ID |= REPORT_ID_NORMAL_TASK;
		}
		else continue;

		if(NormalTaskPara.ExecCnt != 0)//err20180404-07 执行次数不只对PLC有效 对应上报次数
		{
			if(NormalTaskPara.ExecCnt > NormalTaskPara.RealExeCnt)
			{	
				//if(PORTn == PLC_PORT)
				{
					NormalTaskPara.RealExeCnt++;
					MW((u32)&NormalTaskPara, ADDR_NormalTaskPara + i*sizeof(NormalTaskParaStruct), sizeof(NormalTaskParaStruct));
				}	
			}
			else
			{
				ReportTask->NormalTaskReportExe[i] = 0;//执行次数到 停止上报
			}
		}
		

		
	}
}

void TerminalReportCheckRelayTask(ReportTaskStruct* ReportTask, RealTimeStruct* CurTime)
{
	RelayTaskParaStruct RelayTaskPara;
	u32 i = 0;
	RealTimeStruct* LastReportTime = (RealTimeStruct*)ADDR_LastRelayTaskReportTime;
	
	for(i = 0; i < MAX_RELAY_TASK_NUM; i++)
	{
		MR((u32)&RelayTaskPara, ADDR_RelayTaskPara + i*sizeof(RelayTaskParaStruct), sizeof(RelayTaskParaStruct));
		if(RelayTaskPara.Flag != 0x01) continue;
		if(RelayTaskPara.Length == 0) continue;
		if(RelayTaskPara.ReportPeriod == 0) continue;
		if(RelayTaskPara.ReportPeriodUnit > QCSG_SAMPLE_UNIT_MONTH) continue;
		if(TimeValidCheck(&LastReportTime[i]) != NO_ERROR)
		{
			MR((u32)&LastReportTime[i], (u32)CurTime, sizeof(RealTimeStruct));
		}
		ReportTask->RelayTaskReportExe[i] = QCSG_Judge_TaskTiming(&LastReportTime[i], CurTime, &RelayTaskPara.ReportTime, RelayTaskPara.ReportPeriod, RelayTaskPara.ReportPeriodUnit);
		if(ReportTask->RelayTaskReportExe[i] != 0)
		{
			MR((u32)&LastReportTime[i], (u32)CurTime, sizeof(RealTimeStruct));
			ReportTask->ID |= REPORT_ID_RELAY_TASK;
		}
		else continue;
		
		if(RelayTaskPara.ExecCnt != 0)//err20180404-07 执行次数不只对PLC有效 对应上报次数
		{
			if(RelayTaskPara.ExecCnt > RelayTaskPara.RealExeCnt)
			{	
				//if(PORTn == PLC_PORT)
				{
					RelayTaskPara.RealExeCnt++;
					MW((u32)&RelayTaskPara, ADDR_RelayTaskPara + i*sizeof(RelayTaskParaStruct), sizeof(RelayTaskParaStruct));
				}	
			}
			else
			{
				ReportTask->RelayTaskReportExe[i] = 0;//执行次数到 停止上报
			}
		}

		
	}
}

void TerminalReportCheckAlarm(TerminalRunTimeCtrlStruct* pTSysCtrl)
{
	u8 ReadData[SIZE_512];
	AlarmEventFileHeadStruct AlarmHead;
	u32 i = 0, DataLen = 0, FileName = 0;
	ResultEnum Result = NO_ERROR;
	
	if(pTSysCtrl->LatelyAlarmNum > REPORT_ALARM_MAX_NUM) 
	{
		pTSysCtrl->LatelyAlarmNum = REPORT_ALARM_MAX_NUM;
	}
	else
	{
		for(i = pTSysCtrl->LatelyAlarmNum + 1; i <= REPORT_ALARM_MAX_NUM; i++)
		{
			Result = NAND_ReadAlarmRecordByLatelyNum(i, &AlarmHead, ReadData, &DataLen, &FileName);
			if((Result == NO_ERROR) && (FileName != 0) && (AlarmHead.OcurTime.Ack == 0))
			{
				if(QCSG_CheckAlarmReport(AlarmHead.DataID) == TRUE) pTSysCtrl->LatelyAlarmNum++;
				if(pTSysCtrl->LatelyAlarmNum > REPORT_ALARM_MAX_NUM) break;
			}
		}
	}
}

/*
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

ResultEnum GeneratePowerAlarm(RealTimeStruct* PowerTime, u32 PowerType)
{
	ARD02Struct PowerAlarm;
	MeasurePointStruct Mp;
	RealTimeYYStruct YYTime;
	#if((Project/100)==3)
		u8* pData = NULL;
	#endif
	u32 DataLen = 0;
	u32 PowerAlarmDI = 0;

	if(PowerType == 0)
	{
		PowerAlarmDI = 0xE2000033;		// 终端掉电告警
	}
	else
	{
		PowerAlarmDI = 0xE2000034;		// 终端上电电告警
	}

	if(QCSG_CheckAlarmConfig(PowerAlarmDI) == FALSE) return ERROR_WRONG_ACCESS;
	
	MC(0xFF, (u32)&PowerAlarm, sizeof(ARD02Struct));	
	PowerAlarm.State = ALARM_OCCUR;
	PowerAlarm.ReportState = 0;
	MR((u32)&PowerAlarm.Time, (u32)PowerTime, sizeof(RealTimeStruct));
	RealTimeToYYTime(PowerTime, &YYTime);
	YYTime.Report = 0;
	YYTime.Ack = 0;
	
	#if((Project/100)==3)
		MR((u32)PowerAlarm.C_00010000, ADDR_00010000L, 4);
		MR((u32)PowerAlarm.C_00020000, ADDR_00020000L, 4);
		MR((u32)PowerAlarm.C_00030000, ADDR_00030000L, 4);
		MR((u32)PowerAlarm.C_00040000, ADDR_00040000L, 4);

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
	#endif
	
	NAND_ReadMeasurePointByPn(0, &Mp);
	DataLen = sizeof(ARD02Struct);
	NAND_WriteAlarmRecord(Mp.Addr, PowerAlarmDI, &YYTime, (u8*)&PowerAlarm, &DataLen);
	if(QCSG_CheckAlarmReport(PowerAlarmDI) == TRUE) 
	{
		TerminalReportAlarmInc();
	}
	return NO_ERROR;
}
*/

void MultiFrameSendTask(u32 PORTn)
{
	MultiFrameSendTaskStruct* SendTask;
	SendTask = (MultiFrameSendTaskStruct*)GetUartMultiFrameSendTaskAddr(PORTn);
	if(SendTask == NULL) return;
	if((SendTask->DataBuf == NULL) || (SendTask->DataLen == 0) || (SendTask->Empty == 0)) 
	{
		SendTask->State = TASK_STATE_IDLE;
		return;
	}

	switch(SendTask->State)
	{
		case TASK_STATE_IDLE:
			MultiFrameSendTaskIdleProcess(PORTn, SendTask);
			break;
		case TASK_STATE_SEND:
			MultiFrameSendTaskSendProcess(PORTn, SendTask);
			break;
		case TASK_STATE_WAIT_ACK:
			MultiFrameSendTaskWaitAckProcess(PORTn, SendTask);
			break;
		case TASK_STATE_DONE:
			MultiFrameSendTaskDoneProcess(PORTn, SendTask);
			break;
		default:
			SendTask->State = TASK_STATE_IDLE;
			SendTask->Flag = 0;
			SendTask->Empty = 0;
			break;
	}
}

ResultEnum MultiFrameSendTaskIdleProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask)
{
	if((SendTask->Flag & MF_SEND_START) != 0)
	{	
		SendTask->Flag &= ~MF_SEND_START;
		SendTask->LastDataPos = 0;
		SendTask->DataPos = 0;	
		SendTask->State = TASK_STATE_SEND;
	}
	else if(SendTask->Empty != 0)
	{
		SendTask->State = TASK_STATE_SEND;
	}
	else
	{
		SendTask->State = TASK_STATE_IDLE;
	}
	return NO_ERROR;
}


ResultEnum MultiFrameSendTaskSendProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask)
{
	UartTaskCtrlStruct* UartTask;
	QCSGFrameStruct QCSGFrame;
	UartTxReqStruct UartTxReq;
	u16* HostAckTimer;
	u32 UartTxBuf = 0, UartTxBufLen = 0, DataLen = 0;

	UartTxBuf = GetUartPortTxBuf(PORTn);
	if(UartTxBuf == 0)
	{
		SendTask->State = TASK_STATE_IDLE;
		SendTask->Flag = 0;
		SendTask->Empty = 0;
		return ERROR_POINTER_NULL;
	}
	UartTask = GetUartTaskCtrl(PORTn);
	if(UartTask == NULL) return ERROR_POINTER_NULL;
	if(UartTask->State != UART_STATE_IDLE) return ERROR_WRONG_ACCESS;	

	MC(0, (u32)&QCSGFrame, sizeof(QCSGFrameStruct));
	UartTxBufLen = GetUartPortTxBufLen(PORTn);
	
	if(SendTask->QCSGFrameInfo.AFN == QCSG_AFN_12)
	{
		if(SendTask->QCSGFrameInfo.AmrEnum == (u8)AMR_NormalTask)
		{
			DataLen = QCSGBuildNormalTaskFrameStruct(UartTxBufLen, SendTask, &QCSGFrame);
		}
		else if(SendTask->QCSGFrameInfo.AmrEnum == (u8)AMR_RelayTask)
		{
			DataLen = QCSGBuildRelayTaskFrameStruct(UartTxBufLen, SendTask, &QCSGFrame);
		}
		else
		{
			DataLen = 0;
		}
	}
	else
	{
		DataLen = BuildFrameFromMultiFrameTask(UartTxBufLen, SendTask, &QCSGFrame);
	}

	if(DataLen > 0)
	{
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));
		UartTxReq.SrcTask = GetUartMultiFrameSendTaskID(PORTn);
		UartTxReq.NeedAck = 0;
		UartTxReq.FrameType = SendTask->FrameType;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = SendTask->WaitAckTimeOut;		
		UartTxReq.DataLen = QCSG_BuildFrameByStruct(&QCSGFrame, (u8*)UartTxBuf);
		UartTxReq.DataBuf = (u8*)UartTxBuf;
		UartPortTxReqNew(PORTn, &UartTxReq);
		SendTask->State = TASK_STATE_WAIT_ACK;
		if(SendTask->NeedAck != 0)
		{
			HostAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
			HostAckTimer[0] = SendTask->WaitAckTimeOut;
		}
		UartResetHostAckState(PORTn);
	}
	else
	{
		SendTask->State = TASK_STATE_DONE;
	}
	return NO_ERROR;
}

ResultEnum MultiFrameSendTaskWaitAckProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask)
{
	UartTaskCtrlStruct* UartTask;
	u16* HostAckTimer;
	UartTask = GetUartTaskCtrl(PORTn);
	if(UartTask == NULL) return ERROR_POINTER_NULL;
	if(UartTask->State != UART_STATE_IDLE) return ERROR_WRONG_ACCESS;

	if((SendTask->NeedAck == 0) || (GetUartHostAckState(PORTn) != 0)) 
	{
		if(SendTask->DataPos < SendTask->DataLen)
		{
			SendTask->State = TASK_STATE_SEND;
			SendTask->LastDataPos = SendTask->DataPos;
		}
		else
		{
			SendTask->State = TASK_STATE_DONE;
		}
		SendTask->Round = 0;
		UartResetHostAckState(PORTn);
		return NO_ERROR;
	}

	HostAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(HostAckTimer[0] != 0) return ERROR_WRONG_TIME;

	SendTask->Round++;
	if(SendTask->Round < SendTask->RetryCnt)
	{
		SendTask->State = TASK_STATE_SEND;
		SendTask->DataPos = SendTask->LastDataPos;
		SendTask->QCSGFrameInfo.SEQ--;
	}
	else
	{
		SendTask->State = TASK_STATE_DONE;
	}
	return NO_ERROR;
}

ResultEnum MultiFrameSendTaskDoneProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask)
{
	ReportTaskStruct* ReportTask;
	u16* HostAckTimer;
	if(SendTask->AckToSrcTask != 0)
	{
		if(SendTask->SrcTask == TASK_ID_GPRS_Report)
		{
			ReportTask = (ReportTaskStruct*)GetTerminalReportTaskAddr(GPRSPORT);
		}

		if(SendTask->SrcTask == TASK_ID_ETHE_Report)
		{
			ReportTask = (ReportTaskStruct*)GetTerminalReportTaskAddr(ENETPORT);			
		}
		HostAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
		if(HostAckTimer[0] != 0)
		{
			ReportTask->Flag |= REPORT_FLAG_SEND_OK;
		}
		else
		{
			ReportTask->Flag |= REPORT_FLAG_SEND_FAIL;
		}	
	}
	MC(0, (u32)SendTask, sizeof(MultiFrameSendTaskStruct));
	SendTask->State = TASK_STATE_IDLE;
	return NO_ERROR;
}


u32 BuildFrameFromMultiFrameTask(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* QCSGFrame)
{
	QCSG_PnListStruct PnList;
	u32 StartPos = Task->DataPos;

	while(Task->DataPos < Task->DataLen)
	{
		//MC(0, (u32)&PnList, sizeof(QCSG_PnListStruct));
		PnList.Pn = Task->DataBuf[Task->DataPos++];
		PnList.Pn += Task->DataBuf[Task->DataPos++] << 8;
		PnList.Len = Task->DataBuf[Task->DataPos++];
		PnList.Len += Task->DataBuf[Task->DataPos++] << 8;
		PnList.DI = Task->DataBuf[Task->DataPos++];
		PnList.DI += Task->DataBuf[Task->DataPos++] << 8;
		PnList.DI += Task->DataBuf[Task->DataPos++] << 16;
		PnList.DI += Task->DataBuf[Task->DataPos++] << 24;

		if((QCSGFrame->DataLen + PnList.Len + 6) > (MaxTxBufLen - QCSG_FIXED_LEN))
		{
			Task->DataPos -= sizeof(QCSG_PnListStruct);
			break;
		}
		
		QCSGFrame->Data[QCSGFrame->DataLen++] = PnList.Pn;
		QCSGFrame->Data[QCSGFrame->DataLen++] = PnList.Pn >> 8;
		QCSGFrame->Data[QCSGFrame->DataLen++] = PnList.DI;
		QCSGFrame->Data[QCSGFrame->DataLen++] = PnList.DI >> 8;
		QCSGFrame->Data[QCSGFrame->DataLen++] = PnList.DI >> 16;
		QCSGFrame->Data[QCSGFrame->DataLen++] = PnList.DI >> 24;
		MR((u32)&QCSGFrame->Data[QCSGFrame->DataLen], (u32)&Task->DataBuf[Task->DataPos], PnList.Len);
		QCSGFrame->DataLen += PnList.Len;
		Task->DataPos += PnList.Len;
	}
	QCSGFrame->A1 = Task->QCSGFrameInfo.A1;
	QCSGFrame->A2 = Task->QCSGFrameInfo.A2;
	QCSGFrame->A3 = Task->QCSGFrameInfo.A3;
	QCSGFrame->FC = Task->QCSGFrameInfo.FC;
	QCSGFrame->FC |= QCSG_FC_DIR_BIT;
	//QCSGFrame->FC &= ~QCSG_FC_PRM_BIT;		// PRM 位在进入此函数前已被设置
	QCSGFrame->FC &= ~QCSG_FC_ACD_BIT;
	QCSGFrame->AFN = Task->QCSGFrameInfo.AFN;
	QCSGFrame->SEQ = Task->QCSGFrameInfo.SEQ;
	if((Task->QCSGFrameInfo.SEQ & 0x0F) == 0x0F)
	{
		Task->QCSGFrameInfo.SEQ &= 0xF0;
	}
	else
	{
		Task->QCSGFrameInfo.SEQ++;
	}
	QCSGFrame->SEQ &= ~QCSG_SEQ_TPV_BIT;
	if(Task->NeedAck != 0)
	{
		QCSGFrame->SEQ |= QCSG_SEQ_CON_BIT;
	}
	else
	{
		QCSGFrame->SEQ &= ~QCSG_SEQ_CON_BIT;
	}
	
	if(StartPos == 0)
	{
		QCSGFrame->SEQ |= QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			QCSGFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			QCSGFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	else
	{
		QCSGFrame->SEQ &= ~QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			QCSGFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			QCSGFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	QCSGFrame->SEQ &= ~QCSG_SEQ_TPV_BIT;

	return QCSGFrame->DataLen;
}

u32 QCSGBuildNormalTaskFrameStruct(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* DstFrame)
{
	RealTimeYYStruct DataTime;
	u32 CurDI = 0, LastDI = 0, DataStructType = 0, ALLTimeDataIn = 0, PnDataLen = 0;
	u32 PnNumPos = 0, DINumPos = 0, PnNum = 0, DINum = 0;
	u32 CurPn = 0, LastPn = 0xFFFF;
	u32 StartPos = Task->DataPos;

	DstFrame->DataLen = 0;
	CurDI = Task->DataBuf[0];
	CurDI += Task->DataBuf[1] << 8;
	CurDI += Task->DataBuf[2] << 16;
	CurDI += Task->DataBuf[3] << 24;
	if(Task->DataBuf[4] != TASK_TYPE_NORMAL) return DstFrame->DataLen;
	DataStructType = Task->DataBuf[5];
	if(DataStructType > DATA_STRUCT_TYPE_TASK)  return DstFrame->DataLen;
	
	DstFrame->FC = Task->QCSGFrameInfo.FC;
	DstFrame->FC |= QCSG_FC_DIR_BIT;
	//DstFrame->FC &= ~QCSG_FC_PRM_BIT;
	DstFrame->FC &= ~QCSG_FC_ACD_BIT;
	DstFrame->A1 = Task->QCSGFrameInfo.A1;
	DstFrame->A2 = Task->QCSGFrameInfo.A2;
	DstFrame->A3 = Task->QCSGFrameInfo.A3;
	DstFrame->AFN = Task->QCSGFrameInfo.AFN;
	DstFrame->SEQ = Task->QCSGFrameInfo.SEQ & (~QCSG_SEQ_TPV_BIT);

	DstFrame->Data[DstFrame->DataLen++] = 0x00;
	DstFrame->Data[DstFrame->DataLen++] = 0x00;
	DstFrame->Data[DstFrame->DataLen++] = CurDI;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 8;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 16;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 24;

	if(Task->DataPos == 0)
	{
		Task->DataPos += 6;
		Task->CurTimePos = Task->DataPos;
		Task->DataPos += 6;
		Task->CurTimeDataLen = Task->DataBuf[Task->DataPos++];
		Task->CurTimeDataLen += Task->DataBuf[Task->DataPos++] << 8;
		Task->CurTimeDataEndPos = Task->DataPos + Task->CurTimeDataLen;
	}

	while(Task->DataPos < Task->DataLen)
	{
		if((u8)DataStructType == DATA_STRUCT_TYPE_TASK)
		{	
			if(Task->DataPos >= Task->CurTimeDataEndPos)
			{
				Task->CurTimePos = Task->DataPos;
				Task->DataPos += 6;
				Task->CurTimeDataLen = Task->DataBuf[Task->DataPos++];
				Task->CurTimeDataLen += Task->DataBuf[Task->DataPos++] << 8;
				Task->CurTimeDataEndPos = Task->DataPos + Task->CurTimeDataLen;
			}
			MR((u32)&DataTime, (u32)&Task->DataBuf[Task->CurTimePos], 6);	
			DstFrame->Data[DstFrame->DataLen++] = DataStructType;
			while(Task->DataPos < Task->CurTimeDataEndPos)
			{
				CurPn = Task->DataBuf[Task->DataPos++];
				CurPn += Task->DataBuf[Task->DataPos++] << 8;
				PnDataLen = Task->DataBuf[Task->DataPos++];
				PnDataLen += Task->DataBuf[Task->DataPos++] << 8;

				if((DstFrame->DataLen + PnDataLen) > (MaxTxBufLen - QCSG_FIXED_LEN))
				{	
					Task->DataPos -= 4;
					break;
				}
				DstFrame->Data[DstFrame->DataLen++] = CurPn;
				DstFrame->Data[DstFrame->DataLen++] = CurPn >> 8;
				
				MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&Task->DataBuf[Task->DataPos], PnDataLen);
				DstFrame->DataLen += PnDataLen;
				Task->DataPos += PnDataLen;
			}
			MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&DataTime, 6);
			DstFrame->DataLen += 6;
			if(Task->DataPos < Task->CurTimeDataEndPos) break;
		}
		else
		{
			if(Task->DataPos >= Task->CurTimeDataEndPos)
			{
				Task->CurTimePos = Task->DataPos;
				Task->DataPos += 6;
				Task->CurTimeDataLen = Task->DataBuf[Task->DataPos++];
				Task->CurTimeDataLen += Task->DataBuf[Task->DataPos++] << 8;
				Task->CurTimeDataEndPos = Task->DataPos + Task->CurTimeDataLen;
				PnNum = 0;
				DINum = 0;
			}
			MR((u32)&DataTime, (u32)&Task->DataBuf[Task->CurTimePos], 6);	
			if((DstFrame->DataLen + Task->CurTimeDataLen) < (MaxTxBufLen - QCSG_FIXED_LEN))
			{
				ALLTimeDataIn = 1;	// 当前时间点数据根据长度判断，可全部装入
			}
			DstFrame->Data[DstFrame->DataLen++] = DataStructType;
			PnNumPos = DstFrame->DataLen;
			DstFrame->Data[DstFrame->DataLen++] = 0;		// 数据组数中的 Pn 数量
			DINumPos = DstFrame->DataLen;
			DstFrame->Data[DstFrame->DataLen++] = 0;		// 数据组数中的 DI 数量
			while(Task->DataPos < Task->CurTimeDataEndPos)
			{
				CurPn = Task->DataBuf[Task->DataPos++];
				CurPn += Task->DataBuf[Task->DataPos++] << 8;
				PnDataLen = Task->DataBuf[Task->DataPos++];
				PnDataLen += Task->DataBuf[Task->DataPos++] << 8;
				CurDI = Task->DataBuf[Task->DataPos++];
				CurDI += Task->DataBuf[Task->DataPos++] << 8;
				CurDI += Task->DataBuf[Task->DataPos++] << 16;
				CurDI += Task->DataBuf[Task->DataPos++] << 24;
				if((DstFrame->DataLen + PnDataLen) > (MaxTxBufLen - QCSG_FIXED_LEN))
				{
					Task->DataPos -= 8;
					break;
				}

				if(CurPn != LastPn)
				{
					LastPn = CurPn;
					PnNum++;
				}
				if((ALLTimeDataIn == 0) && (PnNum > 1))
				{
					Task->DataPos -= 8;
					PnNum = 1;
					break;
				}
				if(CurDI != LastDI)
				{
					LastDI = CurDI;
					DINum++;
				}

				DstFrame->Data[DstFrame->DataLen++] = CurPn;
				DstFrame->Data[DstFrame->DataLen++] = CurPn >> 8;
				DstFrame->Data[DstFrame->DataLen++] = CurDI;
				DstFrame->Data[DstFrame->DataLen++] = CurDI >> 8;
				DstFrame->Data[DstFrame->DataLen++] = CurDI >> 16;
				DstFrame->Data[DstFrame->DataLen++] = CurDI >> 24;
				MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&Task->DataBuf[Task->DataPos], PnDataLen);
				DstFrame->DataLen += PnDataLen;
				Task->DataPos += PnDataLen;	
			}	
			DstFrame->Data[PnNumPos] = PnNum;
			DstFrame->Data[DINumPos] = DINum/PnNum;
			MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&DataTime, 6);
			DstFrame->DataLen += 6;
			if(Task->DataPos < Task->CurTimeDataEndPos) break;
		}
	}
	
	if((Task->QCSGFrameInfo.SEQ & 0x0F) == 0x0F)
	{
		Task->QCSGFrameInfo.SEQ &= 0xF0;
	}
	else
	{
		Task->QCSGFrameInfo.SEQ++;
	}
	if(Task->NeedAck != 0)
	{
		DstFrame->SEQ |= QCSG_SEQ_CON_BIT;
	}
	else
	{
		DstFrame->SEQ &= ~QCSG_SEQ_CON_BIT;
	}
	
	if(StartPos == 0)
	{
		DstFrame->SEQ |= QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			DstFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			DstFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	else
	{
		DstFrame->SEQ &= ~QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			DstFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			DstFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	return DstFrame->DataLen;	
}

/*
u32 QCSGBuildNormalTaskFrameStruct(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* DstFrame)
{
	RealTimeYYStruct DataTime;
	u32 CurDI = 0, LastDI = 0, DataStructType = 0, TimeDataLen = 0, TimeDataPos = 0, PnDataLen = 0;
	u32 PnNumPos = 0, DINumPos = 0, PnNum = 0, DINum = 0;
	u32 CurPn = 0, LastPn = 0xFFFF;
	u32 StartPos = Task->DataPos;

	DstFrame->DataLen = 0;
	
	CurDI = Task->DataBuf[0];
	CurDI += Task->DataBuf[1] << 8;
	CurDI += Task->DataBuf[2] << 16;
	CurDI += Task->DataBuf[3] << 24;
	if(Task->DataBuf[4] != TASK_TYPE_NORMAL) return DstFrame->DataLen;
	DataStructType = Task->DataBuf[5];
	if(DataStructType > DATA_STRUCT_TYPE_TASK)  return DstFrame->DataLen;
	
	DstFrame->FC = Task->QCSGFrameInfo.FC;
	DstFrame->FC |= QCSG_FC_DIR_BIT;
	DstFrame->FC &= ~QCSG_FC_PRM_BIT;
	DstFrame->FC &= ~QCSG_FC_ACD_BIT;
	DstFrame->A1 = Task->QCSGFrameInfo.A1;
	DstFrame->A2 = Task->QCSGFrameInfo.A2;
	DstFrame->A3 = Task->QCSGFrameInfo.A3;
	DstFrame->AFN = Task->QCSGFrameInfo.AFN;
	DstFrame->SEQ = Task->QCSGFrameInfo.SEQ & (~QCSG_SEQ_TPV_BIT);

	DstFrame->Data[DstFrame->DataLen++] = 0x00;
	DstFrame->Data[DstFrame->DataLen++] = 0x00;
	DstFrame->Data[DstFrame->DataLen++] = CurDI;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 8;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 16;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 24;

	if(Task->DataPos == 0)
	{
		Task->DataPos += 6;
	}

	while(Task->DataPos < Task->DataLen)
	{
		if((u8)DataStructType == DATA_STRUCT_TYPE_TASK)
		{	
			MR((u32)&DataTime, (u32)&Task->DataBuf[Task->DataPos], 6);
			Task->DataPos += 6;
			TimeDataLen = Task->DataBuf[Task->DataPos++];
			TimeDataLen += Task->DataBuf[Task->DataPos++] << 8;
			if((DstFrame->DataLen + TimeDataLen) > (MaxTxBufLen - QCSG_FIXED_LEN))
			{
				Task->DataPos -= 8;
				break;
			}
			DstFrame->Data[DstFrame->DataLen++] = DataStructType;
			TimeDataPos = 0;
			while(TimeDataPos < TimeDataLen)
			{
				DstFrame->Data[DstFrame->DataLen++] = Task->DataBuf[Task->DataPos++];
				DstFrame->Data[DstFrame->DataLen++] = Task->DataBuf[Task->DataPos++];
				PnDataLen = Task->DataBuf[Task->DataPos++];
				PnDataLen += Task->DataBuf[Task->DataPos++] << 8;
				TimeDataPos += 4;
				
				MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&Task->DataBuf[Task->DataPos], PnDataLen);
				DstFrame->DataLen += PnDataLen;
				Task->DataPos += PnDataLen;
				TimeDataPos += PnDataLen;
			}
			MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&DataTime, 6);
			DstFrame->DataLen += 6;
		}
		else
		{
			MR((u32)&DataTime, (u32)&Task->DataBuf[Task->DataPos], 6);
			Task->DataPos += 6;
			TimeDataLen = Task->DataBuf[Task->DataPos++];
			TimeDataLen += Task->DataBuf[Task->DataPos++] << 8;
			if((DstFrame->DataLen + TimeDataLen) > (MaxTxBufLen - QCSG_FIXED_LEN))
			{
				Task->DataPos -= 8;
				break;
			}
			
			DstFrame->Data[DstFrame->DataLen++] = DataStructType;
			PnNumPos = DstFrame->DataLen;
			DstFrame->Data[DstFrame->DataLen++] = 0;		// 数据组数中的 Pn 数量
			DINumPos = DstFrame->DataLen;
			DstFrame->Data[DstFrame->DataLen++] = 0;		// 数据组数中的 DI 数量
			TimeDataPos = 0;
			while(TimeDataPos < TimeDataLen)
			{
				CurPn = Task->DataBuf[Task->DataPos++];
				CurPn += Task->DataBuf[Task->DataPos++] << 8;
				PnDataLen = Task->DataBuf[Task->DataPos++];
				PnDataLen += Task->DataBuf[Task->DataPos++] << 8;
				CurDI = Task->DataBuf[Task->DataPos++];
				CurDI += Task->DataBuf[Task->DataPos++] << 8;
				CurDI += Task->DataBuf[Task->DataPos++] << 16;
				CurDI += Task->DataBuf[Task->DataPos++] << 24;
				TimeDataPos += 8;
				if(CurPn != LastPn)
				{
					LastPn = CurPn;
					PnNum++;
				}
				if(CurDI != LastDI)
				{
					LastDI = CurDI;
					DINum++;
				}
				DstFrame->Data[DstFrame->DataLen++] = CurPn;
				DstFrame->Data[DstFrame->DataLen++] = CurPn >> 8;
				DstFrame->Data[DstFrame->DataLen++] = CurDI;
				DstFrame->Data[DstFrame->DataLen++] = CurDI >> 8;
				DstFrame->Data[DstFrame->DataLen++] = CurDI >> 16;
				DstFrame->Data[DstFrame->DataLen++] = CurDI >> 24;
				MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&Task->DataBuf[Task->DataPos], PnDataLen);
				DstFrame->DataLen += PnDataLen;
				Task->DataPos += PnDataLen;
				TimeDataPos += PnDataLen;
			}
			DstFrame->Data[PnNumPos] = PnNum;
			DstFrame->Data[DINumPos] = DINum/PnNum;
			MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&DataTime, 6);
			DstFrame->DataLen += 6;
		}
	}
	
	if((Task->QCSGFrameInfo.SEQ & 0x0F) == 0x0F)
	{
		Task->QCSGFrameInfo.SEQ &= 0xF0;
	}
	else
	{
		Task->QCSGFrameInfo.SEQ++;
	}
	if(Task->NeedAck != 0)
	{
		DstFrame->SEQ |= QCSG_SEQ_CON_BIT;
	}
	else
	{
		DstFrame->SEQ &= ~QCSG_SEQ_CON_BIT;
	}
	
	if(StartPos == 0)
	{
		DstFrame->SEQ |= QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			DstFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			DstFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	else
	{
		DstFrame->SEQ &= ~QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			DstFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			DstFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	return DstFrame->DataLen;	
}
*/
u32 QCSGBuildRelayTaskFrameStruct(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* DstFrame)
{
	RealTimeYYStruct DataTime;
	u32 CurDI = 0, RelayType = 0, TimeDataLen = 0;
	u32 StartPos = Task->DataPos;

	DstFrame->DataLen = 0;
	
	CurDI = Task->DataBuf[0];
	CurDI += Task->DataBuf[1] << 8;
	CurDI += Task->DataBuf[2] << 16;
	CurDI += Task->DataBuf[3] << 24;
	if(Task->DataBuf[4] != TASK_TYPE_RELAY) return DstFrame->DataLen;
	RelayType = Task->DataBuf[5];
	if(RelayType > RELAY_TYPE_RELEASE)  return DstFrame->DataLen;
	
	DstFrame->FC = Task->QCSGFrameInfo.FC;
	DstFrame->FC |= QCSG_FC_DIR_BIT;
	//DstFrame->FC &= ~QCSG_FC_PRM_BIT;
	DstFrame->FC &= ~QCSG_FC_ACD_BIT;
	DstFrame->A1 = Task->QCSGFrameInfo.A1;
	DstFrame->A2 = Task->QCSGFrameInfo.A2;
	DstFrame->A3 = Task->QCSGFrameInfo.A3;
	DstFrame->AFN = Task->QCSGFrameInfo.AFN;
	DstFrame->SEQ = Task->QCSGFrameInfo.SEQ & (~QCSG_SEQ_TPV_BIT);

	DstFrame->Data[DstFrame->DataLen++] = 0x00;
	DstFrame->Data[DstFrame->DataLen++] = 0x00;
	DstFrame->Data[DstFrame->DataLen++] = CurDI;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 8;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 16;
	DstFrame->Data[DstFrame->DataLen++] = CurDI >> 24;

	if(Task->DataPos == 0)
	{
		Task->DataPos += 6;
	}

	while(Task->DataPos < Task->DataLen)
	{
		MR((u32)&DataTime, (u32)&Task->DataBuf[Task->DataPos], 6);
		Task->DataPos += 6;
		TimeDataLen = Task->DataBuf[Task->DataPos++];
		TimeDataLen += Task->DataBuf[Task->DataPos++] << 8;
		if((DstFrame->DataLen + TimeDataLen) > (MaxTxBufLen - QCSG_FIXED_LEN))
		{
			Task->DataPos -= 8;
			break;
		}
		DstFrame->Data[DstFrame->DataLen++] = RelayType;
		DstFrame->Data[DstFrame->DataLen++] = TimeDataLen;
		
		MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&Task->DataBuf[Task->DataPos], TimeDataLen);
		DstFrame->DataLen += TimeDataLen;
		Task->DataPos += TimeDataLen;
		
		MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&DataTime, 6);
		DstFrame->DataLen += 6;		
	}
	
	if((Task->QCSGFrameInfo.SEQ & 0x0F) == 0x0F)
	{
		Task->QCSGFrameInfo.SEQ &= 0xF0;
	}
	else
	{
		Task->QCSGFrameInfo.SEQ++;
	}
	if(Task->NeedAck != 0)
	{
		DstFrame->SEQ |= QCSG_SEQ_CON_BIT;
	}
	else
	{
		DstFrame->SEQ &= ~QCSG_SEQ_CON_BIT;
	}
	
	if(StartPos == 0)
	{
		DstFrame->SEQ |= QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			DstFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			DstFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}
	else
	{
		DstFrame->SEQ &= ~QCSG_SEQ_FIR_BIT;
		if(Task->DataPos < Task->DataLen)
		{
			DstFrame->SEQ &= ~QCSG_SEQ_FIN_BIT;
		}
		else
		{			
			DstFrame->SEQ |= QCSG_SEQ_FIN_BIT;
		}
	}	
	return DstFrame->DataLen;
}


/***********************************************************************************************
功     能: 端口发送请求
入口参 数: DstPort: 发送数据的目标端口
		   MultiFrameSendReqStruct* MultiFrameSendReq:多帧发送请求指针，详见结构说明
		   
返     回: 请求失败或成功	   
编     写: zhaoyun
编写时 间; 2017-8-03
***********************************************************************************************/
ResultEnum QCSGMultiFrameSendReq(u32 DstPort, MultiFrameSendReqStruct* MultiFrameSendReq)
{
	MultiFrameSendTaskStruct* MultiFrameTask;
	MultiFrameTask = (MultiFrameSendTaskStruct*)GetUartMultiFrameSendTaskAddr(DstPort);

	if(MultiFrameTask == NULL) return ERROR_WRONG_ACCESS;
	if(MultiFrameTask->Empty != BUFFER_EMPTY) return ERROR_BUF_FULL;
	if(MultiFrameTask->State != TASK_STATE_IDLE) return ERROR_WRONG_ACCESS;
	
	MultiFrameTask->SrcTask = MultiFrameSendReq->SrcTask;
	MultiFrameTask->RetryCnt = MultiFrameSendReq->RetryCnt;
	MultiFrameTask->Empty = BUFFER_FULL;
	MultiFrameTask->FrameType = MultiFrameSendReq->FrameType;
	MultiFrameTask->DataType = MultiFrameSendReq->DataType;
	MultiFrameTask->AckToSrcTask = MultiFrameSendReq->AckToSrcTask;
	MultiFrameTask->NeedAck = MultiFrameSendReq->NeedAck;
	
	MultiFrameTask->QCSGFrameInfo.A1 = MultiFrameSendReq->QCSGFrameInfo.A1;
	MultiFrameTask->QCSGFrameInfo.A2 = MultiFrameSendReq->QCSGFrameInfo.A2;
	MultiFrameTask->QCSGFrameInfo.A3 = MultiFrameSendReq->QCSGFrameInfo.A3;
	MultiFrameTask->QCSGFrameInfo.FC = MultiFrameSendReq->QCSGFrameInfo.FC;
	MultiFrameTask->QCSGFrameInfo.AFN = MultiFrameSendReq->QCSGFrameInfo.AFN;
	MultiFrameTask->QCSGFrameInfo.SEQ = MultiFrameSendReq->QCSGFrameInfo.SEQ;
	MultiFrameTask->QCSGFrameInfo.Port = MultiFrameSendReq->QCSGFrameInfo.Port;
	MultiFrameTask->QCSGFrameInfo.AmrEnum = MultiFrameSendReq->QCSGFrameInfo.AmrEnum;
	MultiFrameTask->PnListNum = MultiFrameSendReq->PnListNum;
	MultiFrameTask->WaitAckTimeOut = MultiFrameSendReq->WaitAckTimeOut;
	MultiFrameTask->Flag |= MF_SEND_START;	
	MultiFrameTask->LastDataPos = 0;
	MultiFrameTask->DataPos = 0;
	MultiFrameTask->DataLen = MultiFrameSendReq->DataLen;
	MultiFrameTask->DataBuf = MultiFrameSendReq->DataBuf;
	//MR((u32)MultiFrameTask->DataBuf, (u32)MultiFrameSendReq->DataBuf, MultiFrameTask->DataLen);
	return NO_ERROR;
}

/***********************************************************************************************
功     能: QCSG上行端口发送请求，单帧发送请求
入口参 数: DstPort: 发送数据的目标端口
		   MultiFrameSendReqStruct* MultiFrameSendReq:多帧发送请求指针，详见结构说明
		   输入参数虽然是MultiFrameSendReqStruct，但实际要保证一帧可发送完全部数据，由调用者保证
		   函数内部会使用大于2048字节的局部变量空间，注意栈空间的使用情况。
返     回: 请求失败或成功	   
编     写: zhaoyun
编写时 间; 2017-8-03
***********************************************************************************************/
ResultEnum QCSGSingleFrameSendReq(u32 DstPort, MultiFrameSendReqStruct* MultiFrameSendReq)
{
	MultiFrameSendTaskStruct MultiFrameTask;
	UartTaskCtrlStruct* UartTask;
	QCSGFrameStruct QCSGFrame;
	UartTxReqStruct UartTxReq;
	u32 UartTxBuf = 0, UartTxBufLen = 0, DataLen = 0;
	ResultEnum Result = NO_ERROR;
	
	if(MultiFrameSendReq == NULL) return ERROR_POINTER_NULL;

	UartTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(DstPort);
	if(UartTask == NULL) return ERROR_POINTER_NULL;
	if(UartTask->State != UART_STATE_IDLE) return ERROR_TASK_BUSY;	
	//if(UartTask->FrameLen != 0) return ERROR_TASK_BUSY;
	
	UartTxBuf = GetUartPortTxBuf(DstPort);
	if(UartTxBuf == 0) return ERROR_POINTER_NULL;

	UartTxBufLen = GetUartPortTxBufLen(DstPort);
	MC(0, (u32)&QCSGFrame, sizeof(QCSGFrameStruct));
	
	MultiFrameTask.SrcTask = MultiFrameSendReq->SrcTask;
	MultiFrameTask.RetryCnt = MultiFrameSendReq->RetryCnt;
	MultiFrameTask.Empty = BUFFER_FULL;
	MultiFrameTask.FrameType = MultiFrameSendReq->FrameType;
	MultiFrameTask.DataType = MultiFrameSendReq->DataType;
	MultiFrameTask.AckToSrcTask = MultiFrameSendReq->AckToSrcTask;
	MultiFrameTask.NeedAck = MultiFrameSendReq->NeedAck;
	
	MultiFrameTask.QCSGFrameInfo.A1 = MultiFrameSendReq->QCSGFrameInfo.A1;
	MultiFrameTask.QCSGFrameInfo.A2 = MultiFrameSendReq->QCSGFrameInfo.A2;
	MultiFrameTask.QCSGFrameInfo.A3 = MultiFrameSendReq->QCSGFrameInfo.A3;
	MultiFrameTask.QCSGFrameInfo.FC = MultiFrameSendReq->QCSGFrameInfo.FC;
	MultiFrameTask.QCSGFrameInfo.AFN = MultiFrameSendReq->QCSGFrameInfo.AFN;
	MultiFrameTask.QCSGFrameInfo.SEQ = MultiFrameSendReq->QCSGFrameInfo.SEQ;
	MultiFrameTask.QCSGFrameInfo.Port = MultiFrameSendReq->QCSGFrameInfo.Port;
	MultiFrameTask.QCSGFrameInfo.AmrEnum = MultiFrameSendReq->QCSGFrameInfo.AmrEnum;
	MultiFrameTask.PnListNum = MultiFrameSendReq->PnListNum;
	MultiFrameTask.WaitAckTimeOut = MultiFrameSendReq->WaitAckTimeOut;
	MultiFrameTask.Flag |= MF_SEND_START;	
	MultiFrameTask.DataPos = 0;
	MultiFrameTask.DataLen = MultiFrameSendReq->DataLen;
	MultiFrameTask.DataBuf = MultiFrameSendReq->DataBuf;

	DataLen = BuildFrameFromMultiFrameTask(UartTxBufLen, &MultiFrameTask, &QCSGFrame);
	if(DataLen > 0)
	{
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));
		UartTxReq.SrcTask = MultiFrameSendReq->SrcTask;
		//UartTxReq.NeedAck = MultiFrameTask->NeedAck;
		//UartTxReq.FrameType = MultiFrameTask->FrameType;
		//UartTxReq.RetryCnt = MultiFrameTask->RetryCnt;
		//UartTxReq.BpsCtrl = 0;
		//UartTxReq.TimeOut = MultiFrameTask->WaitAckTimeOut;		
		UartTxReq.NeedAck = 0;
		UartTxReq.FrameType = MultiFrameTask.FrameType;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = 0;	
		UartTxReq.DataLen = QCSG_BuildFrameByStruct(&QCSGFrame, (u8*)UartTxBuf);
		UartTxReq.DataBuf = (u8*)UartTxBuf;
		Result = UartPortTxReqNew(DstPort, &UartTxReq);

		#if (TEST_ON == 1)
			//QCSG_LogToRS232Port(DstPort, (u32)UartTxReq.DataBuf, UartTxReq.DataLen);
		#endif
	}
	return Result;
}
/***********************************************************************************************
功     能: 端口发送请求
入口参 数: SrcTask: 数据发送请求的源任务ID,
		   DstPort: 发送数据的目标端口
		   FrameBuf: 源数据指针
		   DataLen: 发送的数据长度
		   Option: byte0，表示是否应答，byte1表示帧类型，byte3表示重试次数
		   BpsCtrl: D8-D5位表示波特率,O～11依次表示0=300,1=600,2=1200,3=2400,4=4800,
		            5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400
		            D4位表示停止位位数0/1分别表示1/2个停止位
		            D3位表示有无校验位0/1分别表示无/有
		            D2位表示偶／奇校验位0/1分别表示偶／奇校验
		            D1-D0 数据位数0～3分别表示5-8位
		            BpsCtrl == 0,表示保持目标端口原波特率控制不变
		   TimeOut: 等待应答超时时间
		   此函数会发生数据拷贝过程，最大字节数为 2048
返     回: 请求失败或成功	   
编     写: zhaoyun
编写时 间; 2017-8-03
***********************************************************************************************/
ResultEnum UartPortTxReq(TaskIDEnum SrcTask, u32 DstPort, u8* FrameBuf, u32 DataLen, u32 Option, u32 BpsCtrl, u16 TimeOut)
{
	UartTaskCtrlStruct* pUartTask;
	UARTCtrl_TypeDef* UARTCtrl;
	u32 ACRxBuf = 0, UartTxBuf = 0;
	u32* p32;
	u16* p16timer;
	if(FrameBuf == NULL) return ERROR_POINTER_NULL;
	
	if((u8)DstPort == ACSAMPLEPORT)
	{
		UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
		ACRxBuf = Get_ADDR_UARTnRx(ACSAMPLEPORT);
		MR(ACRxBuf, (u32)FrameBuf, DataLen);
		UARTCtrl->RxByte = (u16)DataLen;
		UARTCtrl->Task = UART_TASK_RX_DONE;
		UARTCtrl->Lock = 0;								// 0x55=通信功能正在被调用禁止其他程序再次调用
		
		p32 = (u32 *)(ADDR_Meter_ENTRY);
		(*(void(*)())p32[130])((u32)ACSAMPLEPORT);		//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送

		UARTCtrl->TxCount = UARTCtrl->TxByte;
		UARTCtrl->Task = UART_TASK_RX_DONE;				// 1=接收完成 

		return NO_ERROR;
	}
		
	pUartTask = GetUartTaskCtrl(DstPort);
	if(pUartTask == NULL) return ERROR_POINTER_NULL;
	if(pUartTask->State != UART_STATE_IDLE) return ERROR_TASK_BUSY;

	UartTxBuf = GetUartPortTxBuf(DstPort);
	if(UartTxBuf == 0) return ERROR_POINTER_NULL;

	pUartTask->State = UART_STATE_SEND_TASK_FRAME;
	pUartTask->SrcTaskID = SrcTask;
	pUartTask->NeedAck = Option;
	pUartTask->FrameType = Option >> 8;
	pUartTask->RetryCnt = Option >> 16;
	pUartTask->BpsCtrl = BpsCtrl;
	pUartTask->AckTiemOutSetByTask = TimeOut;
	pUartTask->FrameLen = DataLen;
	MR(UartTxBuf, (u32)FrameBuf, DataLen);
	pUartTask->FrameBuf = (u8*)UartTxBuf;
	pUartTask->Flag |= UART_FLAG_TX_TASK_FRAME;
	p16timer = (u16*)Get_ADDR_UARTnTimer(DstPort);
	p16timer[0] = UART_TX_DELAY_GAP;
	return NO_ERROR;
}

/***********************************************************************************************
功     能: 端口发送请求
入口参 数: DstPort: 发送数据的目标端口
		   UartTxReq:按照实际帧需求填写成员变量，其中 DataBuf 指针必须在发送期间数据保持不变
		   一般为发送通道的专用发送缓冲区，获得此发送通道的专用发送缓冲区的函数如下
		   UartTxReq->DataBuf = (u32)GetUartPortTxBuf(DstPort);
		   此函数不进行数据拷贝，直接使用指针传递，然后发送数据
返     回: 请求失败或成功	   
编     写: zhaoyun
编写时 间; 2017-8-03
***********************************************************************************************/
ResultEnum UartPortTxReqNew(u32 DstPort, UartTxReqStruct* UartTxReq)
{
	UartTaskCtrlStruct* pUartTask;
	UARTCtrl_TypeDef* UARTCtrl;
	u32 ACRxBuf = 0;
	u32* p32;
	u16* p16timer;
	if(UartTxReq == NULL) return ERROR_POINTER_NULL;
	if(UartTxReq->DataBuf == NULL) return ERROR_POINTER_NULL;
	
	if((u8)DstPort == ACSAMPLEPORT)
	{
		UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
		ACRxBuf = Get_ADDR_UARTnRx(ACSAMPLEPORT);
		MR(ACRxBuf, (u32)UartTxReq->DataBuf, UartTxReq->DataLen);
		UARTCtrl->RxByte = (u16)UartTxReq->DataLen;
		UARTCtrl->Task = UART_TASK_RX_DONE;
		UARTCtrl->Lock = 0;								// 0x55=通信功能正在被调用禁止其他程序再次调用
		
		p32 = (u32 *)(ADDR_Meter_ENTRY);
		(*(void(*)())p32[130])((u32)ACSAMPLEPORT);		//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送

		UARTCtrl->TxCount = UARTCtrl->TxByte;
		UARTCtrl->Task = UART_TASK_RX_DONE;				// 1=接收完成 

		return NO_ERROR;
	}
		
	pUartTask = GetUartTaskCtrl(DstPort);
	if(pUartTask == NULL) return ERROR_POINTER_NULL;
	if(pUartTask->State != UART_STATE_IDLE) return ERROR_TASK_BUSY;
	
	pUartTask->State = UART_STATE_SEND_TASK_FRAME;
	pUartTask->SrcTaskID = UartTxReq->SrcTask;
	pUartTask->NeedAck = UartTxReq->NeedAck;
	pUartTask->FrameType = UartTxReq->FrameType;
	pUartTask->RetryCnt = UartTxReq->RetryCnt;
	pUartTask->BpsCtrl = UartTxReq->BpsCtrl;
	pUartTask->AckTiemOutSetByTask = UartTxReq->TimeOut;
	pUartTask->FrameLen = UartTxReq->DataLen;
	pUartTask->FrameBuf = UartTxReq->DataBuf;
	pUartTask->Flag |= UART_FLAG_TX_TASK_FRAME;

	p16timer = (u16*)Get_ADDR_UARTnTimer(DstPort);
	p16timer[0] = UART_TX_DELAY_GAP;
	return NO_ERROR;
}

/***********************************************************************************************
功     能: 端口接收请求
入口参 数: SrcPort: 请求接收数据的端口
		   FrameBuf: 源数据指针
		   DataLen: 发送的数据长度
		   FrameType: 接收到的帧类型
		   DataType: 接收缓冲数据组织方式		   
返     回: 请求失败或成功	   
编     写: zhaoyun
编写时 间; 2017-8-03
***********************************************************************************************/
ResultEnum UartPortRxReq(u32 SrcPort, u8* FrameBuf, u32* DataLen, u8* FrameType, u8* DataType)
{
	UartRxChCtrlStruct* UartRxChCtrl;
	UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* pUTask;
	u32 ACTxBuf = 0;
	
	if(FrameBuf == NULL) return ERROR_POINTER_NULL;
	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(SrcPort);
	if(UartRxChCtrl == NULL) return ERROR_POINTER_NULL;
	
	if((u8)SrcPort == ACSAMPLEPORT)
	{
		UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
		if(UARTCtrl->Task == UART_TASK_RX_DONE)
		{
			ACTxBuf = Get_ADDR_UARTnTx(ACSAMPLEPORT);		
			MR((u32)FrameBuf, ACTxBuf, UARTCtrl->TxCount);
			DataLen[0] = UARTCtrl->TxCount;
			FrameType[0] = FRAME_TYPE_DLT_645;
			DataType[0] = DATA_TYPE_FRAME;

			UARTCtrl->Task = UART_TASK_RX;
			UARTCtrl->TxByte = 0;
			UARTCtrl->TxCount = 0;
			return NO_ERROR;
		}
		
		return ERROR_BUF_EMPTY;
	}

	if(UartRxChCtrl->Empty == BUFFER_EMPTY) return ERROR_BUF_EMPTY;
	MR((u32)FrameBuf, (u32)UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
	DataLen[0] = UartRxChCtrl->FrameLen;
	FrameType[0] = UartRxChCtrl->FrameType;
	DataType[0] = UartRxChCtrl->DataType;
	
	UartRxChCtrl->Empty = BUFFER_EMPTY;
	//MC(0, (u32)UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
	UartRxChCtrl->FrameLen = 0;
	
	pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(SrcPort);
	if(pUTask != NULL)
	{
		pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	}
	
	return NO_ERROR;
}

ResultEnum SetRealTimeAMRTaskReq(TaskIDEnum SrcTask, u32 SrcPort, QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq)
{
	UartRxChCtrlStruct* UartRxChCtrl;

	if(RT_AMRReq->Lock != 0) return ERROR_TASK_BUSY;

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(SrcPort);
	if(UartRxChCtrl == NULL) return ERROR_POINTER_NULL;
	
	RT_AMRReq->PnListNum = UartRxChCtrl->PnListNum;
	RT_AMRReq->PnListLen = UartRxChCtrl->FrameLen;
	RT_AMRReq->FrameType = UartRxChCtrl->FrameType;
	RT_AMRReq->DataType = UartRxChCtrl->DataType;

	MR((u32)&RT_AMRReq->QCSGFrameInfo, (u32)&UartRxChCtrl->QCSGFrameInfo, sizeof(QCSGFrameInfoStruct));
	
	MR((u32)RT_AMRReq->Data, (u32)UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);	
	
	RT_AMRReq->Lock = 0x01;
	UartRxChCtrl->Empty = BUFFER_EMPTY;
	MC(0, (u32)UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
	UartRxChCtrl->FrameLen = 0;
	return NO_ERROR;
}

UartTaskCtrlStruct* GetUartTaskCtrl(u32 PORTn)
{
	UartTaskCtrlStruct* pUTask = NULL;
	switch((u8)PORTn)
	{
		case RS232PORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl;
			break;		
		case GPRSPORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_GPRS_UART_Task_Ctrl;
			break;		
		case RS485_1PORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_RS485_CH1_Task_Ctrl;
			break;
		case RS485_2PORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_RS485_CH2_Task_Ctrl;
			break;
		case RS485_3PORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_RS485_CH3_Task_Ctrl;
			break;		
		case PLC_PORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_PLC_YX_UART_Task_Ctrl;
			break;
		case IRPORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl;
			break;
		case ENETPORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_ETHE_UART_Task_Ctrl;
			break;
		case ACSAMPLEPORT:
			pUTask = (UartTaskCtrlStruct*)ADDR_ACSAMPLE_UART_Task_Ctrl;
			break;
		default:
			break;
	}
	return pUTask;
}


u32 GetUartPortRxChCtrl(u32 PORTn)
{
	u32 Addr = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			Addr = ADDR_RS232_RX_ChCtrl;
			break;		
		case GPRSPORT:
			Addr = ADDR_GPRS_RX_ChCtrl;
			break;		
		case RS485_1PORT:
			Addr = ADDR_RS485_CH1_RX_ChCtrl;
			break;
		case RS485_2PORT:
			Addr = ADDR_RS485_CH2_RX_ChCtrl;
			break;
		case RS485_3PORT:
			Addr = ADDR_RS485_CH3_RX_ChCtrl;
			break;		
		case PLC_PORT:
			Addr = ADDR_PLC_YX_RX_ChCtrl;
			break;
		case IRPORT:
			Addr = ADDR_IR_RX_ChCtrl;
			break;
		case ENETPORT:
			Addr = ADDR_ETHE_RX_ChCtrl;
			break;
		case ACSAMPLEPORT:
			Addr = ADDR_ACSAMPLE_RX_ChCtrl;
			break;
		default:
			break;
	}
	return Addr;
}

u32 GetUartPortRxBuf(u32 PORTn, u8 DataType)
{
	u32 FrameBufAddr = 0;
	u32 DataBufAddr = 0;
	DataBufAddr = Get_ADDR_UARTnFnDataBuff(PORTn);
	switch((u8)PORTn)
	{
		case RS232PORT:
			FrameBufAddr = ADDR_RS232_RxBuf;			
			break;		
		case GPRSPORT:
			FrameBufAddr = ADDR_GPRS_RxBuf;
			break;		
		case RS485_1PORT:
			FrameBufAddr = ADDR_RS485_CH1_RxBuf;
			break;
		case RS485_2PORT:
			FrameBufAddr = ADDR_RS485_CH2_RxBuf;
			break;
		case RS485_3PORT:
			FrameBufAddr = ADDR_RS485_CH3_RxBuf;
			break;		
		case PLC_PORT:
			FrameBufAddr = ADDR_PLC_YX_RxBuf;
			break;
		case IRPORT:
			FrameBufAddr = ADDR_IR_RxBuf;
			break;
		case ENETPORT:
			FrameBufAddr = ADDR_ETHE_RxBuf;
			break;
		default:
			FrameBufAddr = 0;
			DataBufAddr = 0;
			break;
	}
	if(DataType != DATA_TYPE_FRAME)
	{
		return DataBufAddr;
	}
	else
	{
		return FrameBufAddr;
	}
}

u32 GetUartPortRxBufLen(u32 PORTn)
{
	u32 Len = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			Len = LEN_RS232_RxBuf;
			break;		
		case GPRSPORT:
			Len = LEN_GPRS_RxBuf;
			break;		
		case RS485_1PORT:
			Len = LEN_RS485_CH1_RxBuf;
			break;
		case RS485_2PORT:
			Len = LEN_RS485_CH2_RxBuf;
			break;
		case RS485_3PORT:
			Len = LEN_RS485_CH3_RxBuf;
			break;		
		case PLC_PORT:
			Len = LEN_PLC_YX_RxBuf;
			break;
		case IRPORT:
			Len = LEN_IR_RxBuf;
			break;
		case ENETPORT:
			Len = LEN_ETHE_RxBuf;
			break;
		case ACSAMPLEPORT:
			break;
		default:
			break;
	}
	return Len;
}

u32 GetUartPortTxBuf(u32 PORTn)
{
	u32 TxBufAddr = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			TxBufAddr = ADDR_RS232_TxBuf;			
			break;		
		case GPRSPORT:
			TxBufAddr = ADDR_GPRS_TxBuf;
			break;		
		case RS485_1PORT:
			TxBufAddr = ADDR_RS485_CH1_TxBuf;
			break;
		case RS485_2PORT:
			TxBufAddr = ADDR_RS485_CH2_TxBuf;
			break;
		case RS485_3PORT:
			TxBufAddr = ADDR_RS485_CH3_TxBuf;
			break;		
		case PLC_PORT:
			TxBufAddr = ADDR_PLC_YX_TxBuf;
			break;
		case IRPORT:
			TxBufAddr = ADDR_IR_TxBuf;
			break;
		case ENETPORT:
			TxBufAddr = ADDR_ETHE_TxBuf;
			break;
		case ACSAMPLEPORT:
			TxBufAddr = ADDR_AC_SAMPLE_TxBuf;
			break;
		default:
			TxBufAddr = 0;
			break;
	}
	return TxBufAddr;
}

u32 GetUartPortTxBufLen(u32 PORTn)
{
	u32 Len = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			Len = LEN_RS232_TxBuf;
			break;		
		case GPRSPORT:
			Len = LEN_GPRS_TxBuf - SIZE_1K;
			break;		
		case RS485_1PORT:
			Len = LEN_RS485_CH1_TxBuf;
			break;
		case RS485_2PORT:
			Len = LEN_RS485_CH2_TxBuf;
			break;
		case RS485_3PORT:
			Len = LEN_RS485_CH3_TxBuf;
			break;		
		case PLC_PORT:
			Len = LEN_PLC_YX_TxBuf;
			break;
		case IRPORT:
			Len = LEN_IR_TxBuf;
			break;
		case ENETPORT:
			Len = LEN_ETHE_TxBuf - SIZE_1K;
			break;
		case ACSAMPLEPORT:
			Len = LEN_AC_SAMPLE_TxBuf;
			break;
		default:
			Len = LEN_RS232_TxBuf;
			break;
	}
	return Len;
}

TaskIDEnum GetUartTaskID(u32 PORTn)
{
	TaskIDEnum TaskID = TASK_ID_NONE;
	switch((u8)PORTn)
	{
		case RS232PORT:
			TaskID = TASK_ID_RS232_UART;
			break;		
		case GPRSPORT:
			TaskID = TASK_ID_GPRS_UART;
			break;		
		case RS485_1PORT:
			TaskID = TASK_ID_RS485_1_UART;
			break;
		case RS485_2PORT:
			TaskID = TASK_ID_RS485_2_UART;
			break;
		case RS485_3PORT:
			TaskID = TASK_ID_RS485_3_UART;
			break;		
		case PLC_PORT:
			TaskID = TASK_ID_PLC_YX_UART;
			break;
		case IRPORT:
			TaskID = TASK_ID_IR_UART;
			break;
		case ENETPORT:
			TaskID = TASK_ID_ETHE_UART;
			break;
		default:
			TaskID = TASK_ID_RS232_UART;
			break;
	}
	return TaskID;
}

TaskIDEnum GetUartMultiFrameSendTaskID(u32 PORTn)
{
	TaskIDEnum TaskID = TASK_ID_NONE;
	switch((u8)PORTn)
	{
		case RS232PORT:
			TaskID = TASK_ID_RS232_MULTI_FRAME_SEND;
			break;		
		case GPRSPORT:
			TaskID = TASK_ID_GPRS_MULTI_FRAME_SEND;
			break;		
		case RS485_1PORT:
			TaskID = TASK_ID_RS485_CH1_MULTI_FRAME_SEND;
			break;
		case RS485_2PORT:
			TaskID = TASK_ID_RS485_CH2_MULTI_FRAME_SEND;
			break;
		case RS485_3PORT:
			TaskID = TASK_ID_RS485_CH3_MULTI_FRAME_SEND;
			break;		
		case PLC_PORT:
			TaskID = TASK_ID_NONE;
			break;
		case IRPORT:
			TaskID = TASK_ID_IR_MULTI_FRAME_SEND;
			break;
		case ENETPORT:
			TaskID = TASK_ID_ETHE_MULTI_FRAME_SEND;
			break;
		default:
			TaskID = TASK_ID_NONE;
			break;
	}
	return TaskID;
}


u32 GetUartMultiFrameSendTaskAddr(u32 PORTn)
{
	u32 TaskCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			TaskCtrlAddr = ADDR_RS232_MultiFrameSendTask;
			break;		
		case GPRSPORT:
			TaskCtrlAddr = ADDR_GPRS_MultiFrameSendTask;
			break;		
		case RS485_1PORT:
			TaskCtrlAddr = ADDR_RS485_CH1_MultiFrameSendTask;
			break;
		case RS485_2PORT:
			TaskCtrlAddr = ADDR_RS485_CH2_MultiFrameSendTask;
			break;
		case RS485_3PORT:
			TaskCtrlAddr = ADDR_RS485_CH3_MultiFrameSendTask;
			break;		
		case PLC_PORT:
			TaskCtrlAddr = ADDR_PLC_YX_MultiFrameSendTask;
			break;
		case IRPORT:
			TaskCtrlAddr = ADDR_IR_MultiFrameSendTask;
			break;
		case ENETPORT:
			TaskCtrlAddr = ADDR_ETHE_MultiFrameSendTask;
			break;
		default:
			TaskCtrlAddr = 0;
			break;
	}
	return TaskCtrlAddr;
}

u32 GetTerminalTaskCtrlAddr(TaskIDEnum Task)
{
	u32 TaskCtrlAddr = 0;
	switch(Task)
	{
		case TASK_ID_RealTimeAMR:
			TaskCtrlAddr = ADDR_RealTimeAMRTaskCtrl;
			break;		
		case TASK_ID_TerminalCtrl:
			TaskCtrlAddr = ADDR_TerminalTaskCtrl;
			break;
		case TASK_ID_RS232_UART:
			TaskCtrlAddr = ADDR_RS232_Task_Ctrl;
			break;		
		case TASK_ID_IR_UART:
			TaskCtrlAddr = ADDR_IR_Task_Ctrl;
			break;		
		case TASK_ID_RS485_1_UART:
			TaskCtrlAddr = ADDR_RS485_CH1_Task_Ctrl;
			break;
		case TASK_ID_RS485_2_UART:
			TaskCtrlAddr = ADDR_RS485_CH2_Task_Ctrl;
			break;
		case TASK_ID_RS485_3_UART:
			TaskCtrlAddr = ADDR_RS485_CH3_Task_Ctrl;
			break;		
		case TASK_ID_GPRS_UART:
			TaskCtrlAddr = ADDR_GPRS_UART_Task_Ctrl;
			break;
		case TASK_ID_PLC_YX_UART:
			TaskCtrlAddr = ADDR_PLC_YX_UART_Task_Ctrl;
			break;
		case TASK_ID_ETHE_UART:
			TaskCtrlAddr = ADDR_ETHE_UART_Task_Ctrl;
			break;
		case TASK_ID_ETH_LINK_MAINTAIN:
			TaskCtrlAddr = 0;
			break;
		case TASK_ID_GPRS_LINK_MAINTAIN:
			TaskCtrlAddr = 0;
			break;
		default:
			TaskCtrlAddr = 0;
			break;
	}
	return TaskCtrlAddr;
}



u32 GetUartPortAckTimerAddr(u32 PORTn)
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	u32 AckTimerAddr = 0;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			AckTimerAddr = (u32)&pTerminalRam->RS485_1_Ack_S_Timer;
			break;
		case RS485_2PORT:
			AckTimerAddr = (u32)&pTerminalRam->RS485_2_Ack_S_Timer;
			break;
		case RS485_3PORT:
			AckTimerAddr = (u32)&pTerminalRam->RS485_3_Ack_S_Timer;
			break;
		case PLC_PORT:
			AckTimerAddr = (u32)&pTerminalRam->RouterAck_S_Timer;
			break;
		case GPRSPORT:
			AckTimerAddr = (u32)&pTerminalRam->GPRS_WaitHostACK_S_Timer;
			break;
		case RS232PORT:
			AckTimerAddr = (u32)&pTerminalRam->RS232_WaitHostACK_S_Timer;
			break;
		case IRPORT:
			AckTimerAddr = (u32)&pTerminalRam->IR_WaitHostAck_S_Timer;
			break;
		case ENETPORT:
			AckTimerAddr = (u32)&pTerminalRam->ENET_WaitHostACK_S_Timer;
			break;
		default:
			break;
	}
	return AckTimerAddr;
}

u32 GetTerminalPower_S_TimerValue(void)
{
	u32 TimerValue = 0;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	TimerValue = pTerminalRam->Power_S_Timer;
	return TimerValue;
}
u32 GetTerminalReportTaskAddr(u32 PORTn)
{
	u32 TaskCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case GPRSPORT:
			TaskCtrlAddr = ADDR_GPRS_ReportTask;
			break;		
		case ENETPORT:
			TaskCtrlAddr = ADDR_ETHE_ReportTask;
			break;
		default:
			TaskCtrlAddr = 0;
			break;
	}
	return TaskCtrlAddr;
}

u32 GetUart_QCSG_DataBufAddr(u32 PORTn)
{
	u32 TaskCtrlAddr = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			TaskCtrlAddr = ADDR_RS232_QCSG_DataBuf;
			break;		
		case GPRSPORT:
			TaskCtrlAddr = ADDR_GPRS_QCSG_DataBuf;
			break;		
		case RS485_1PORT:
			TaskCtrlAddr = ADDR_RS485_1_QCSG_DataBuf;
			break;
		case RS485_2PORT:
			TaskCtrlAddr = ADDR_RS485_2_QCSG_DataBuf;
			break;
		case RS485_3PORT:
			TaskCtrlAddr = ADDR_RS485_3_QCSG_DataBuf;
			break;		
		case IRPORT:
			TaskCtrlAddr = ADDR_IR_QCSG_DataBuf;
			break;
		case ENETPORT:
			TaskCtrlAddr = ADDR_ENET_QCSG_DataBuf;
			break;
		default:
			TaskCtrlAddr = ADDR_RS232_QCSG_DataBuf;
			break;
	}
	return TaskCtrlAddr;
}

u32 GetUart_QCSG_DataBufLen(u32 PORTn)
{
	u32 MaxLen = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			MaxLen = LEN_RS232_QCSG_DataBuf;
			break;		
		case GPRSPORT:
			MaxLen = LEN_GPRS_QCSG_DataBuf;
			break;		
		case RS485_1PORT:
			MaxLen = LEN_RS485_1_QCSG_DataBuf;
			break;
		case RS485_2PORT:
			MaxLen = LEN_RS485_2_QCSG_DataBuf;
			break;
		case RS485_3PORT:
			MaxLen = LEN_RS485_3_QCSG_DataBuf;
			break;		
		case IRPORT:
			MaxLen = LEN_IR_QCSG_DataBuf;
			break;
		case ENETPORT:
			MaxLen = LEN_ENET_QCSG_DataBuf;
			break;
		default:
			MaxLen = LEN_IR_QCSG_DataBuf;
			break;
	}
	return MaxLen;
}

u32 GetUart_QCSG_SEQ(u32 PORTn)
{
	u32 Seq = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	Seq = UARTCtrl->ReTxSEQ++;
	
	return Seq;
}

u32 GetUartHostAckState(u32 PORTn)
{
	u32 HostAck = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	HostAck = UARTCtrl->ReceivedHostAck;
	
	return HostAck;
}
void UartResetHostAckState(u32 PORTn)
{
	UARTCtrl_TypeDef* UARTCtrl;
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->ReceivedHostAck = 0;
}


ResultEnum QCSG_LogToRS232Port(u32 PORTn, u32 SrcFrameAddr, u32 LogLen)
{

	UartTaskCtrlStruct* UartTask;
	u32 DstFrameAddr = 0;
	
	//if(PORTn == RS232PORT) return ERROR_WRONG_ACCESS;
	UartTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(RS232PORT);
	DstFrameAddr = Get_ADDR_UARTnTx(RS232PORT);
	if((UartTask->Flag & UART_FLAG_LOG_OUT) == 0)
	{
		UartTask->AckLen = LogLen;
		UartTask->Flag |= UART_FLAG_LOG_OUT;
		MR(DstFrameAddr, SrcFrameAddr, LogLen);
	}
	else
	{
		MR(DstFrameAddr + UartTask->AckLen, SrcFrameAddr, LogLen);
		UartTask->AckLen += LogLen;
	}	
	if(UartTask->State != UART_STATE_IDLE) 
		return ERROR_TASK_BUSY;
	else
		UartTask->Flag |= UART_FLAG_TX_ACK;
	
	return NO_ERROR;
	
	/*

	RealTimeStruct TestTime;
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	

	UartTaskCtrlStruct* UartTask;
	u32 DstFrameAddr = 0;
	UartTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(RS232PORT);
	DstFrameAddr = Get_ADDR_UARTnTx(RS232PORT);

	TestTime.u8Second	= pComm_Ram->RTCBuff[0];
	TestTime.u8Minute	= pComm_Ram->RTCBuff[1];
	TestTime.u8Hour 	= pComm_Ram->RTCBuff[2];
	if(PORTn == 0)	TestTime.u8Day	= 0xAA;
	else 			TestTime.u8Day	= (u8)PORTn;
	MR(DstFrameAddr, (u32)&TestTime.u8Second, 4);
	
	if((UartTask->Flag & UART_FLAG_LOG_OUT) == 0)
	{
		UartTask->AckLen = LogLen + 4;
		UartTask->Flag |= UART_FLAG_LOG_OUT;
		MR(DstFrameAddr + 4, SrcFrameAddr, LogLen);
	}
	else
	{
		MR(DstFrameAddr + UartTask->AckLen + 4, SrcFrameAddr, LogLen);
		UartTask->AckLen += LogLen + 4;
	}	
	UartTask->Flag |= UART_FLAG_TX_ACK;
	return NO_ERROR;
	*/
}


/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


