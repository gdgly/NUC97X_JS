/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_AFN12.c
��  ��:zhaoyun
��  ��:1.00
��  ��:20170601
��  ��:����AFN12�����ļ�
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
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"

#include "../QCSG_Head/QCSG_AFN12.h"
#include "Terminal_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
ResultEnum QCSG_AFN12_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	NormalTaskParaStruct NormalTaskPara;
	RelayTaskParaStruct RelayTaskPara;
	MultiFrameSendTaskStruct SendTask;
	MultiFrameSendReqStruct SendReq;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	AMR_ENUM AmrEnum = AMR_UNKNOWN;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0, TotalPnDataLen = 0, Addr_Len = 0, TotalPnListNum = 0, DataMaxLen = 0;
	u8* PnListDataBuf = NULL;

	PnListDataBuf = (u8*)GetUart_QCSG_DataBufAddr(PORTn);
	DataMaxLen = GetUart_QCSG_DataBufLen(PORTn);
	HAL_GetSystemTime(&RealTime);
	MC(0, (u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	QCSGFp.PORTn = PORTn;
	QCSGFp.UserDataLen = SrcFrame->DataLen;
    while(QCSGFp.SrcDataPos < QCSGFp.UserDataLen)
    {
		QCSGFp.Pn = QCSG_TakePn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 2;
		QCSGFp.CurDataID = QCSG_TakeFn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 4;
		MemCopy((u8*)&QCSGFp.StartTime, &SrcFrame->Data[QCSGFp.SrcDataPos], 6);
		QCSGFp.SrcDataPos += 6;
		MemCopy((u8*)&QCSGFp.EndTime, &SrcFrame->Data[QCSGFp.SrcDataPos], 6);
		QCSGFp.SrcDataPos += 6;
		QCSGFp.DataDensity = SrcFrame->Data[QCSGFp.SrcDataPos];
		QCSGFp.SrcDataPos += 1;

		if(QCSGFp.SrcDataPos < QCSGFp.UserDataLen) continue;
		if(QCSGFp.Pn != 0) continue;
		AmrEnum = GetAMREnumByDI(QCSGFp.CurDataID);	
		if((AmrEnum != AMR_NormalTask) && (AmrEnum != AMR_RelayTask)) continue;
		
		RealTimeToYYTime(&RealTime, &DataTime);
		if(TimeCompare(AmrEnum, &DataTime, &QCSGFp.StartTime) == ERROR_TIME_IS_SMALLER) continue;
		if(TimeCompare(AmrEnum, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		
		if(AmrEnum == AMR_NormalTask)
		{
			Addr_Len = ADDR_NormalTaskPara + (((QCSGFp.CurDataID - 1) & 0x000000FF)*sizeof(NormalTaskParaStruct));
			MR((u32)&NormalTaskPara, Addr_Len, sizeof(NormalTaskParaStruct));
			if(NormalTaskPara.Flag != 0x01) continue;
			if(NormalTaskPara.DANum == 0) continue;
			if(NormalTaskPara.DataIDNum == 0) continue;
			if(NormalTaskPara.SamplePeriod == 0) continue;
			if(NormalTaskPara.SamplePeriodUnit > QCSG_SAMPLE_UNIT_MONTH) continue;
			if(NormalTaskPara.DataStructureType > DATA_STRUCT_TYPE_TASK) continue;
			if(NormalTaskPara.DataIDNum > AMR_MAX_DATA_ID) continue;
			if((QCSGFp.DataDensity == 0) && (NormalTaskPara.GetRate == 0)) continue;
			if(CheckDataDensity(AmrEnum, QCSGFp.DataDensity, NormalTaskPara.SamplePeriodUnit, NormalTaskPara.SamplePeriod) != NO_ERROR) continue;

			TotalPnDataLen = QCSG_AFN12_GetNormalTaskData(&QCSGFp, &NormalTaskPara, PnListDataBuf, &TotalPnListNum, DataMaxLen);
		}
		else
		{
			Addr_Len = ADDR_RelayTaskPara + (((QCSGFp.CurDataID - 1) & 0x000000FF)*sizeof(RelayTaskParaStruct));
			MR((u32)&RelayTaskPara, Addr_Len, sizeof(RelayTaskParaStruct));
			if(RelayTaskPara.Flag != 0x01) continue;
			if(RelayTaskPara.Length == 0) continue;
			if(RelayTaskPara.SamplePeriod == 0) continue;
			if(RelayTaskPara.SamplePeriodUnit > QCSG_SAMPLE_UNIT_MONTH) continue;
			if((QCSGFp.DataDensity == 0) && (RelayTaskPara.GetRate == 0)) continue;
			if(CheckDataDensity(AmrEnum, QCSGFp.DataDensity, RelayTaskPara.SamplePeriodUnit, RelayTaskPara.SamplePeriod) != NO_ERROR) continue;

			TotalPnDataLen = QCSG_AFN12_GetRelayTaskData(&QCSGFp, &RelayTaskPara, PnListDataBuf, &TotalPnListNum, DataMaxLen);
		}
	}

	DataMaxLen = GetUartPortTxBufLen(PORTn);
	if((TotalPnDataLen > 0) && (TotalPnDataLen <= (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum)))
	{
		MC(0, (u32)&SendTask, sizeof(MultiFrameSendTaskStruct));
		SendTask.SrcTask = GetUartTaskID(PORTn);
		SendTask.FrameType = FRAME_TYPE_QCSG_UP;
		SendTask.DataType = DATA_TYPE_STRUCT;
		SendTask.QCSGFrameInfo.A1 = SrcFrame->A1;
		SendTask.QCSGFrameInfo.A2 = SrcFrame->A2;
		SendTask.QCSGFrameInfo.A3 = SrcFrame->A3;
		SendTask.QCSGFrameInfo.FC = SrcFrame->FC;
		SendTask.QCSGFrameInfo.AFN = SrcFrame->AFN;
		SendTask.QCSGFrameInfo.SEQ = SrcFrame->SEQ;
		SendTask.QCSGFrameInfo.Port = (u8)PORTn;
		SendTask.PnListNum = TotalPnListNum;
		SendTask.DataPos = 0;
		SendTask.DataLen = TotalPnDataLen;
		SendTask.DataBuf = PnListDataBuf;
		Addr_Len = GetUartPortTxBufLen(PORTn);
		if(AmrEnum == AMR_NormalTask)
		{
			DataLen = QCSGBuildNormalTaskFrameStruct(Addr_Len, &SendTask, DstFrame);
		}
		else
		{
			DataLen = QCSGBuildRelayTaskFrameStruct(Addr_Len, &SendTask, DstFrame);
		}
		if((DataLen != 0) && (DstFrame->DataLen != 0))
		{
			Result = NO_ERROR;
		}
		else
		{
			TotalPnDataLen = 0;
		}
	}
	if(TotalPnDataLen > (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum))
	{
		MC(0, (u32)&SendReq, sizeof(MultiFrameSendReqStruct));
		SendReq.SrcTask = GetUartTaskID(PORTn);
		SendReq.FrameType = FRAME_TYPE_QCSG_UP;
		SendReq.DataType = DATA_TYPE_STRUCT;
		SendReq.QCSGFrameInfo.A1 = SrcFrame->A1;
		SendReq.QCSGFrameInfo.A2 = SrcFrame->A2;
		SendReq.QCSGFrameInfo.A3 = SrcFrame->A3;
		SendReq.QCSGFrameInfo.FC = SrcFrame->FC;
		SendReq.QCSGFrameInfo.AFN = SrcFrame->AFN;
		SendReq.QCSGFrameInfo.SEQ = SrcFrame->SEQ;
		SendReq.QCSGFrameInfo.Port = (u8)PORTn;
		SendReq.QCSGFrameInfo.AmrEnum = (u8)AmrEnum;
		SendReq.PnListNum = TotalPnListNum;
		SendReq.DataLen = TotalPnDataLen;
		SendReq.DataBuf = PnListDataBuf;
		
		Result = QCSGMultiFrameSendReq(PORTn, &SendReq);
		if(Result == NO_ERROR)
		{
			Result = ERROR_DO_NOT_ANSWER;
		}
		else
		{
			TotalPnDataLen = 0;
		}
	}
	if(TotalPnDataLen == 0)
	{
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //ԭ����:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=12
		DstFrame->FC |= QCSG_FC_DIR_BIT;
		DstFrame->FC &= ~QCSG_FC_PRM_BIT;	
		DstFrame->FC &= ~QCSG_FC_ACD_BIT;
		DstFrame->A1 = SrcFrame->A1;
		DstFrame->A2 = SrcFrame->A2;
		DstFrame->A3 = SrcFrame->A3;
		DstFrame->AFN = 00;
		DstFrame->SEQ = SrcFrame->SEQ & (~(QCSG_SEQ_TPV_BIT + QCSG_SEQ_CON_BIT));
		QCSGFp.DstDataPos = 0;
		DstFrame->Data[QCSGFp.DstDataPos++] = 0;
		DstFrame->Data[QCSGFp.DstDataPos++] = 0;
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)QCSG_FOR_ALL_DI;
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(QCSG_FOR_ALL_DI >> 8);
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(QCSG_FOR_ALL_DI >> 16);
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(QCSG_FOR_ALL_DI >> 24);
		DstFrame->Data[QCSGFp.DstDataPos++] = QCSG_DENY;
		DstFrame->DataLen = QCSGFp.DstDataPos;
		Result = NO_ERROR;
	}
	
	return Result;
}



u32 QCSG_AFN12_GetNormalTaskData(QCSGFrameProcessStruct* pQCSGF, NormalTaskParaStruct* TaskPara, u8* PnListDataBuf, u32* TotalPnListNum, u32 PnListDataMaxLen)
{
	MeasurePointStruct MeasurePoint;
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	u32 DataLen = 0, TotalPnDataLen = 0, SinglePnDataLen = 0, TmpPnListNum = 0, PnDataLenPos = 0;
	u32 SamplePeriod = 0, SamplePeriodTime = 0, PnListIndex = 0, DataDensity = 0;
	u32 DataOffSet = 0, DITblIndex = 0, DIDataLen = 0, TimeDataLenPos = 0, TimeDataLen = 0;
	u8 PnIndex = 0, DIIndex = 0, DataExsit = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	u16* PnList = (u16*)(ADDR_PnListBuf);

	DataDensity = pQCSGF->DataDensity;
	if(pQCSGF->DataDensity == 0)
	{
		if((TaskPara->SamplePeriod == 0) || (TaskPara->GetRate == 0)) return TotalPnDataLen;
		SamplePeriod = TaskPara->SamplePeriod;
		SamplePeriodTime = SamplePeriod * TaskPara->GetRate;
	
		switch(TaskPara->SamplePeriodUnit)
		{
			case QCSG_SAMPLE_UNIT_MINUTE:
				DataDensity = QCSG_DATA_DENSITY_1_MINUTE;
				break;
			case QCSG_SAMPLE_UNIT_HOUR:
				DataDensity = QCSG_DATA_DENSITY_1_HOUR;
				break;
			case QCSG_SAMPLE_UNIT_DAY:
				DataDensity = QCSG_DATA_DENSITY_1_DAY;
				break;
			case QCSG_SAMPLE_UNIT_MONTH:
				DataDensity = QCSG_DATA_DENSITY_1_MONTH;
				break;
			default:
				DataDensity = 0xFFFFFFFF;
				break;
		}
		if(DataDensity == 0xFFFFFFFF) return TotalPnDataLen;
	}
	else
	{
		ConvertTimeToDataDensityTime(&pQCSGF->StartTime, pQCSGF->DataDensity);
		if(TimeCompare(AMR_NormalTask, &pQCSGF->StartTime, &pQCSGF->EndTime) != ERROR_TIME_IS_SMALLER) return TotalPnDataLen;
	}
		
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID;
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID >> 8;
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID >> 16;
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID >> 24;
	//PnListDataBuf[TotalPnDataLen++] = 0;			//��ǰ����Ŷ�Ӧ�����ݳ����ֶΣ�2�ֽ�
	//PnListDataBuf[TotalPnDataLen++] = 0;
	PnListDataBuf[TotalPnDataLen++] = TASK_TYPE_NORMAL;
	PnListDataBuf[TotalPnDataLen++] = TaskPara->DataStructureType;
	MR((u32)&DataTime, (u32)&pQCSGF->StartTime, sizeof(RealTimeYYStruct));
	while(TimeCompare(AMR_NormalTask, &DataTime, &pQCSGF->EndTime) == ERROR_TIME_IS_SMALLER)
	{
		//err20190418 �˴�δ�����ϱ���ѯ��ʼʱ��StartTime������ȷӦ���������ݲɼ�ʱ��LastTime
		//���ڸ����⣬̨��������/����������� ���ϸ�
		MR((u32)&PnListDataBuf[TotalPnDataLen], (u32)&DataTime, 6);	
		TotalPnDataLen += 6;						//��ǰ����Ŷ�Ӧ������ʱ���ֶΣ�6�ֽ�
		TimeDataLenPos = TotalPnDataLen;
		PnListDataBuf[TotalPnDataLen++] = 0;		//��ǰ����Ŷ�Ӧ������ʱ�����������ݳ����ֶΣ�2�ֽ�
		PnListDataBuf[TotalPnDataLen++] = 0;
		TimeDataLen = 0;
				
		for(PnIndex = 0; PnIndex < TaskPara->DANum; PnIndex++)
		{
			TmpPnListNum = SplitPnToList(TaskPara->DA[PnIndex], PnList, PN_CHECK_DISABLE);
			for(PnListIndex = 0; PnListIndex < TmpPnListNum; PnListIndex++)
			{
				DataLen = 0;
				NAND_ReadMeasurePoint(PnList[PnListIndex], 0xE0800000, (u8*)&MeasurePoint, &DataLen);
				if(MeasurePoint.State != 0x01) continue;
				DataLen = 0;
				NAND_ReadNormalTask(MeasurePoint.Addr, &AMRFileHead, pQCSGF->CurDataID, &DataTime, PnData, &DataLen);
				if(DataLen != 0) DataExsit++;
				if(TaskPara->DataStructureType == DATA_STRUCT_TYPE_TASK)
				{
					PnListDataBuf[TotalPnDataLen++] = PnList[PnListIndex];
					PnListDataBuf[TotalPnDataLen++] = PnList[PnListIndex] >> 8;
					PnDataLenPos = TotalPnDataLen;
					PnListDataBuf[TotalPnDataLen++] = 0;
					PnListDataBuf[TotalPnDataLen++] = 0;
					TimeDataLen += 4;
					SinglePnDataLen = 0;
					for(DIIndex = 0; DIIndex < TaskPara->DataIDNum; DIIndex++)
					{
						DITblIndex = FindNwDIFrom_AFN0C_0D_Tbl(TaskPara->DataID[DIIndex]);
						if(DITblIndex >= MAX_AFN0C_0D_IDENT_NUM) continue;
							
						DataOffSet = FindDIDataInAMRData(PnData, DataLen, TaskPara->DataID[DIIndex]);
						if(DataOffSet < DataLen)
						{
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							SinglePnDataLen += 4;
							TimeDataLen += 4;
							
							DIDataLen = PnData[DataOffSet++];
							DIDataLen += PnData[DataOffSet++] << 8;
							MR((u32)&PnListDataBuf[TotalPnDataLen], (u32)&PnData[DataOffSet], DIDataLen);
							TotalPnDataLen += DIDataLen;	
							SinglePnDataLen += DIDataLen;
							TimeDataLen += DIDataLen;
						}
						else
						{
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex];
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex] >> 8;
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex] >> 16;
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex] >> 24;
							SinglePnDataLen += 4;
							TimeDataLen += 4;
							
							MC(0xFF, (u32)&PnListDataBuf[TotalPnDataLen], gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen);
							if((gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen == 21) ||
							   (gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen == 41))
							{
								PnListDataBuf[TotalPnDataLen] = 0x04;
							}
							TotalPnDataLen += gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen;
							SinglePnDataLen += gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen;
							TimeDataLen += gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen;
						}
					}
					if(SinglePnDataLen != 0)
					{
						PnListDataBuf[PnDataLenPos] = SinglePnDataLen;
						PnListDataBuf[PnDataLenPos + 1] = SinglePnDataLen >> 8;
						TotalPnListNum[0]++;
					}
					else
					{
						TotalPnDataLen -= 4;
						TimeDataLen -= 4;
					}	
				}
				else
				{
					for(DIIndex = 0; DIIndex < TaskPara->DataIDNum; DIIndex++)
					{
						DITblIndex = FindNwDIFrom_AFN0C_0D_Tbl(TaskPara->DataID[DIIndex]);
						if(DITblIndex >= MAX_AFN0C_0D_IDENT_NUM) continue;
							
						DataOffSet = FindDIDataInAMRData(PnData, DataLen, TaskPara->DataID[DIIndex]);						
						SinglePnDataLen = 0;
						if(DataOffSet < DataLen)
						{
							PnListDataBuf[TotalPnDataLen++] = PnList[PnListIndex];
							PnListDataBuf[TotalPnDataLen++] = PnList[PnListIndex] >> 8;
							PnDataLenPos = TotalPnDataLen;
							PnListDataBuf[TotalPnDataLen++] = 0;
							PnListDataBuf[TotalPnDataLen++] = 0;
							TimeDataLen += 4;
						
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							PnListDataBuf[TotalPnDataLen++] = PnData[DataOffSet++];
							TimeDataLen += 4;
							DIDataLen = PnData[DataOffSet++];
							DIDataLen += PnData[DataOffSet++] << 8;
							MR((u32)&PnListDataBuf[TotalPnDataLen], (u32)&PnData[DataOffSet], DIDataLen);
							TotalPnDataLen += DIDataLen;	
							SinglePnDataLen += DIDataLen;
							TimeDataLen += DIDataLen;

						}
						else
						{
							PnListDataBuf[TotalPnDataLen++] = PnList[PnListIndex];
							PnListDataBuf[TotalPnDataLen++] = PnList[PnListIndex] >> 8;
							PnDataLenPos = TotalPnDataLen;
							PnListDataBuf[TotalPnDataLen++] = 0;
							PnListDataBuf[TotalPnDataLen++] = 0;
							TimeDataLen += 4;
							
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex];
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex] >> 8;
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex] >> 16;
							PnListDataBuf[TotalPnDataLen++] = TaskPara->DataID[DIIndex] >> 24;
							TimeDataLen += 4;
							MC(0xFF, (u32)&PnListDataBuf[TotalPnDataLen], gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen);
							if((gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen == 21) ||
							   (gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen == 41))
							{
								PnListDataBuf[TotalPnDataLen] = 0x04;
							}
							TotalPnDataLen += gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen;
							SinglePnDataLen += gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen;
							TimeDataLen += gc_stQCSGAFN0C_0D_DLT645_Table[DITblIndex].u16Dlen;
						}


						PnListDataBuf[TotalPnDataLen++] = DataTime.YearL;
						PnListDataBuf[TotalPnDataLen++] = DataTime.Month;
						PnListDataBuf[TotalPnDataLen++] = DataTime.Day;
						PnListDataBuf[TotalPnDataLen++] = DataTime.Hour;
						PnListDataBuf[TotalPnDataLen++] = DataTime.Minute;
						SinglePnDataLen += 5;
						TimeDataLen += 5;
						PnListDataBuf[PnDataLenPos] = SinglePnDataLen;
						PnListDataBuf[PnDataLenPos + 1] = SinglePnDataLen >> 8;
						TotalPnListNum[0]++;
					}
				}
			}
		}

		if(TimeDataLen != 0)
		{
			PnListDataBuf[TimeDataLenPos] = TimeDataLen;
			PnListDataBuf[TimeDataLenPos + 1] = TimeDataLen >> 8;
		}
		else
		{
			TotalPnDataLen -= 8;
			TimeDataLen -= 8;
		}
			
		if(TotalPnDataLen >= PnListDataMaxLen) break;
		if(pQCSGF->DataDensity == 0)
		{
			for(SamplePeriod = 0; SamplePeriod < SamplePeriodTime; SamplePeriod++)
			{
				AddTimeByDataDensity(&DataTime, DataDensity);
			}
		}
		else
		{
			AddTimeByDataDensity(&DataTime, pQCSGF->DataDensity);
		}
	}

//	if(DataExsit == 0)
//	{
//		TotalPnDataLen = 0;
//		TotalPnListNum[0] = 0;
//	}
	return TotalPnDataLen;
}


u32 FindDIDataInAMRData(u8* AMRData, u32 AMRDataLen, u32 DI)
{
	u32 DataPos = 0, TempDI = 0, TempLen = 0;
	while(DataPos < AMRDataLen)
	{
		TempDI = AMRData[DataPos];
		TempDI += AMRData[DataPos + 1] << 8;
		TempDI += AMRData[DataPos + 2] << 16;
		TempDI += AMRData[DataPos + 3] << 24;

		TempLen = AMRData[DataPos + 4];
		TempLen += AMRData[DataPos + 5] << 8;
		if(TempDI == DI) break;
		DataPos += 6;
		DataPos += TempLen;
	}
	return DataPos;
}

u32 QCSG_AFN12_GetRelayTaskData(QCSGFrameProcessStruct* pQCSGF, RelayTaskParaStruct* TaskPara, u8* PnListDataBuf, u32* TotalPnListNum, u32 PnListDataMaxLen)
{
	RealTimeYYStruct DataTime;
	u32 DataLen = 0, TotalPnDataLen = 0, TimeDataLenPos = 0, TimeDataLen = 0;
	u32 SamplePeriod = 0, SamplePeriodTime = 0, DataDensity = 0;	//, DataExsit = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);

	DataDensity = pQCSGF->DataDensity;
	if(pQCSGF->DataDensity == 0)
	{
		if((TaskPara->SamplePeriod == 0) || (TaskPara->GetRate == 0)) return TotalPnDataLen;
		SamplePeriod = TaskPara->SamplePeriod;
		SamplePeriodTime = SamplePeriod * TaskPara->GetRate;
	
		switch(TaskPara->SamplePeriodUnit)
		{
			case QCSG_SAMPLE_UNIT_MINUTE:
				DataDensity = QCSG_DATA_DENSITY_1_MINUTE;
				break;
			case QCSG_SAMPLE_UNIT_HOUR:
				DataDensity = QCSG_DATA_DENSITY_1_HOUR;
				break;
			case QCSG_SAMPLE_UNIT_DAY:
				DataDensity = QCSG_DATA_DENSITY_1_DAY;
				break;
			case QCSG_SAMPLE_UNIT_MONTH:
				DataDensity = QCSG_DATA_DENSITY_1_MONTH;
				break;
			default:
				DataDensity = 0xFFFFFFFF;
				break;
		}
		if(DataDensity == 0xFFFFFFFF) return TotalPnDataLen;
	}
	else
	{
		ConvertTimeToDataDensityTime(&pQCSGF->StartTime, pQCSGF->DataDensity);
		if(TimeCompare(AMR_RelayTask, &pQCSGF->StartTime, &pQCSGF->EndTime) != ERROR_TIME_IS_SMALLER) return TotalPnDataLen;
	}

	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID;
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID >> 8;
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID >> 16;
	PnListDataBuf[TotalPnDataLen++] = pQCSGF->CurDataID >> 24;
	//PnListDataBuf[TotalPnDataLen++] = 0;			//��ǰ����Ŷ�Ӧ�����ݳ����ֶΣ�2�ֽ�
	//PnListDataBuf[TotalPnDataLen++] = 0;
	PnListDataBuf[TotalPnDataLen++] = TASK_TYPE_RELAY;
	PnListDataBuf[TotalPnDataLen++] = TaskPara->RelayType;
	MR((u32)&DataTime, (u32)&pQCSGF->StartTime, sizeof(RealTimeYYStruct));
	while(TimeCompare(AMR_RelayTask, &DataTime, &pQCSGF->EndTime) == ERROR_TIME_IS_SMALLER)
	{
		MR((u32)&PnListDataBuf[TotalPnDataLen], (u32)&DataTime, 6);	
		TotalPnDataLen += 6;						//��ǰ����Ŷ�Ӧ������ʱ���ֶΣ�6�ֽ�
		TimeDataLenPos = TotalPnDataLen;
		PnListDataBuf[TotalPnDataLen++] = 0;		//��ǰ����Ŷ�Ӧ������ʱ�����������ݳ����ֶΣ�2�ֽ�
		PnListDataBuf[TotalPnDataLen++] = 0;
		TimeDataLen = 0;	
		DataLen = 0;
		NAND_ReadRelayTask(TaskPara->RelayType, pQCSGF->CurDataID, &DataTime, PnData, &DataLen);

		if(DataLen != 0)
		{
			//DataExsit++;
			MR((u32)&PnListDataBuf[TotalPnDataLen], (u32)PnData, DataLen);
			TotalPnDataLen += DataLen;
			TimeDataLen += DataLen;
			PnListDataBuf[TimeDataLenPos] = TimeDataLen;
			PnListDataBuf[TimeDataLenPos + 1] = TimeDataLen >> 8;
			TotalPnListNum[0]++;
		}
		else
		{
			TotalPnDataLen -= 8;
			TimeDataLen -= 8;
		}
			
		if(TotalPnDataLen >= PnListDataMaxLen) break;
		if(pQCSGF->DataDensity == 0)
		{
			for(SamplePeriod = 0; SamplePeriod < SamplePeriodTime; SamplePeriod++)
			{
				AddTimeByDataDensity(&DataTime, DataDensity);
			}
		}
		else
		{
			AddTimeByDataDensity(&DataTime, pQCSGF->DataDensity);
		}
	}

	if(TotalPnDataLen <= 6) TotalPnDataLen = 0;
	return TotalPnDataLen;
}

/**************************************** (C) COPYRIGHT zhaoyun ***************************************END OF FILE****/

