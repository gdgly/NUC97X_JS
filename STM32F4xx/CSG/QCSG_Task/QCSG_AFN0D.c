/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0D.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN0D处理文件
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

#include "../QCSG_Head/QCSG_AFN0D.h"
#include "Terminal_Task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/*****************************************************************************************************
功     能: 	南网上行协议写参数AFN = 0D操作
入口参 数: 	PORTn: 帧源端口号
			SrcFrame: 输入的源数据帧指针
			DstFrame: 输出的应答数据帧指针
返     回: 返回操作是否成功
编     写: zhaoyun
编写时 间; 2017-7-10
*****************************************************************************************************/
ResultEnum QCSG_AFN0D_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	MeasurePointStruct MeasurePoint;
	MultiFrameSendReqStruct SendReq;
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	AMR_ENUM AmrEnum = AMR_UNKNOWN;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0, TotalPnDataLen = 0, LenPos = 0, DataExist = 0, DataMaxLen = 0;
	u32 i = 0, SinglePnDataLen = 0, TotalPnListNum = 0, TmpPnListNum = 0;
	u16 LastPn = QCSG_INVALID_PN, PnDataNotComplete = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	u16* PnList = (u16*)(ADDR_PnListBuf);	
	u8* PnListDataBuf = NULL;

#if ((Project/100)==5)
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList; //err20181122 II型集中器 增加曲线数据 信号强度 使用RAM终端地址
#endif

	PnListDataBuf = (u8*)GetUart_QCSG_DataBufAddr(PORTn);
	DataMaxLen = GetUart_QCSG_DataBufLen(PORTn);
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
		
		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0)) continue;

		AmrEnum = GetAMREnumByDI(QCSGFp.CurDataID);	
		if(AmrEnum > AMR_CurveFreeze) continue;

		YYTimeToRealTime(&QCSGFp.StartTime, &RealTime);
		if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;
		YYTimeToRealTime(&QCSGFp.EndTime, &RealTime);
		if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;
		
		HAL_GetSystemTime(&RealTime);
		RealTimeToYYTime(&RealTime, &DataTime);
		if(TimeCompare(AmrEnum, &DataTime, &QCSGFp.StartTime) == ERROR_TIME_IS_SMALLER) continue;
		if(TimeCompare(AmrEnum, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		
		#if ((Project/100)==2)
			if((AmrEnum == AMR_DailyStatisticFreeze) || (AmrEnum == AMR_MonthStatisticFreeze)) continue;
		#endif

		if(QCSGFp.DataDensity == 0)
		{
			QCSGFp.DataDensity = GetDefaultDataDensityByAMREnum(AmrEnum);
		}
		if(CheckDataDensity(AmrEnum, QCSGFp.DataDensity, 0, 0) != NO_ERROR) continue;

		ConvertTimeToDataDensityTime(&QCSGFp.StartTime, QCSGFp.DataDensity);
		if(TimeCompare(AmrEnum, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		
		if(LastPn != QCSGFp.Pn)
		{
			TmpPnListNum = SplitPnToList(QCSGFp.Pn, PnList, PN_CHECK_DISABLE);
			LastPn = QCSGFp.Pn;
		}	
		i = 0;
		PnDataNotComplete = 0;
		while(i < TmpPnListNum)
		{
			PnListDataBuf[TotalPnDataLen++] = PnList[i];
			PnListDataBuf[TotalPnDataLen++] = PnList[i] >> 8;
			LenPos = TotalPnDataLen;
			PnListDataBuf[TotalPnDataLen++] = 0;
			PnListDataBuf[TotalPnDataLen++] = 0;
			
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 8;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 16;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 24;
			Result = NAND_ReadMeasurePoint(PnList[i], 0xE0800000, (u8*)&MeasurePoint, &DataLen);
			SinglePnDataLen = 0;
			
			if(PnDataNotComplete == 0) MR((u32)&DataTime, (u32)&QCSGFp.StartTime, sizeof(RealTimeYYStruct));
			while(TimeCompare(AmrEnum, &DataTime, &QCSGFp.EndTime) == ERROR_TIME_IS_SMALLER)
			{
				DataLen = 0;
				switch(AmrEnum)
				{
					case AMR_DailyFreeze:
						Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					case AMR_DailyStatisticFreeze:
						#if ((Project/100)==5)
							if(PnList[i] == 0) //err20181122 II型集中器 增加日冻结数据 停电次数时间  	        	 使用RAM终端地址
								Result = NAND_ReadMeterAMRData(AmrEnum, NodeList[0].Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						#else 		
							Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						#endif
						break;
					case AMR_MonthFreeze:
						Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					case AMR_MonthStatisticFreeze:
						#if ((Project/100)==5)
							if(PnList[i] == 0) //err20181122 II型集中器 增加月冻结数据 停电次数时间  	         	 使用RAM终端地址
								Result = NAND_ReadMeterAMRData(AmrEnum, NodeList[0].Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						#else 		
							Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						#endif
						break;
					case AMR_CurveFreeze:
						#if ((Project/100)==5)
							if(PnList[i] == 0) //err20181122 II型集中器 增加曲线数据 信号强度 使用RAM终端地址
								Result = NAND_ReadMeterCurveData(AmrEnum, NodeList[0].Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
							else 
								Result = NAND_ReadMeterCurveData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						#else
							Result = NAND_ReadMeterCurveData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						#endif
						break;
					default:
						break;
				}
				if(Result == NO_ERROR) DataExist++;
				if(DataLen != 0)
				{
					if((SinglePnDataLen + DataLen + 6) < (SIZE_1K - 2*QCSG_FIXED_LEN))
					{
						MemCopy(&PnListDataBuf[TotalPnDataLen], PnData, DataLen);
						TotalPnDataLen += DataLen;
						SinglePnDataLen += (u16)DataLen;
						MemCopy(&PnListDataBuf[TotalPnDataLen], (u8*)&DataTime, 6);
						TotalPnDataLen += 6;
						SinglePnDataLen += 6;
						AddTimeByDataDensity(&DataTime, QCSGFp.DataDensity);
					}
					else
					{
						break;
					}	
				}
				else
				{
					AddTimeByDataDensity(&DataTime, QCSGFp.DataDensity);
				}
				if(TotalPnDataLen >= DataMaxLen) break;
			}
			if(SinglePnDataLen != 0)
			{
				PnListDataBuf[LenPos] = SinglePnDataLen;
				PnListDataBuf[LenPos + 1] = SinglePnDataLen >> 8;
				TotalPnListNum++;
			}
			else
			{
				TotalPnDataLen -= 8;
			}
			if(TotalPnDataLen >= DataMaxLen) break;
			if(TimeCompare(AmrEnum, &DataTime, &QCSGFp.EndTime) == ERROR_TIME_IS_SMALLER)
			{
				PnDataNotComplete = 1;
				continue;
			}		
			i++;
			PnDataNotComplete = 0;
		}	
		if(TotalPnDataLen >= DataMaxLen) break;
	}
		
#if (TEST_ON == 0)
	if(DataExist == 0)
	{
		TotalPnDataLen = 0;
		TotalPnListNum = 0;
	}
#endif
	
	DataMaxLen = GetUartPortTxBufLen(PORTn);
	if((TotalPnDataLen > 0) && (TotalPnDataLen <= (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum)))
	{
		DstFrame->FC = SrcFrame->FC;
		DstFrame->FC |= QCSG_FC_DIR_BIT;
		DstFrame->FC &= ~QCSG_FC_PRM_BIT;
		DstFrame->FC &= ~QCSG_FC_ACD_BIT;
		DstFrame->A1 = SrcFrame->A1;
		DstFrame->A2 = SrcFrame->A2;
		DstFrame->A3 = SrcFrame->A3;
		DstFrame->AFN = SrcFrame->AFN;
		DstFrame->SEQ = SrcFrame->SEQ & (~(QCSG_SEQ_TPV_BIT + QCSG_SEQ_CON_BIT));

		DataLen = 0;
		DstFrame->DataLen = 0;		
		while(DataLen < TotalPnDataLen)
		{
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];		// 获取 Pn
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			SinglePnDataLen = PnListDataBuf[DataLen + 1];						// 获取 Pn 对应的数据长度
			SinglePnDataLen <<= 8;
			SinglePnDataLen += PnListDataBuf[DataLen];
			DataLen += 2;

			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];		// 获取 DI
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			// 根据长度获取 Pn 对应的数据
			MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&PnListDataBuf[DataLen], SinglePnDataLen);
			DstFrame->DataLen += SinglePnDataLen;
			DataLen += SinglePnDataLen;
		}
		return NO_ERROR;
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
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=0D
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

/*
ResultEnum QCSG_AFN0D_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	MeasurePointStruct MeasurePoint;
	MultiFrameSendReqStruct SendReq;
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	AMR_ENUM AmrEnum = AMR_UNKNOWN;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0, TotalPnDataLen = 0, LenPos = 0, DataExist = 0, DataMaxLen = 0;
	u32 i = 0, SinglePnDataLen = 0, TotalPnListNum = 0, TmpPnListNum = 0;
	u16 LastPn = QCSG_INVALID_PN;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	u16* PnList = (u16*)(ADDR_PnListBuf);	
	u8* PnListDataBuf = NULL;

	PnListDataBuf = (u8*)GetUart_QCSG_DataBufAddr(PORTn);
	DataMaxLen = GetUart_QCSG_DataBufLen(PORTn);
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
		
		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0))
		{
			continue;
		}

		AmrEnum = GetAMREnumByDI(QCSGFp.CurDataID);	
		if(AmrEnum > AMR_CurveFreeze) continue;

		YYTimeToRealTime(&QCSGFp.StartTime, &RealTime);
		if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;
		YYTimeToRealTime(&QCSGFp.EndTime, &RealTime);
		if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;
		
		HAL_GetSystemTime(&RealTime);
		RealTimeToYYTime(&RealTime, &DataTime);
		if(TimeCompare(AmrEnum, &DataTime, &QCSGFp.StartTime) == ERROR_TIME_IS_SMALLER) continue;
		if(TimeCompare(AmrEnum, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		
		#if ((Project/100)==2)
			if((AmrEnum == AMR_DailyStatisticFreeze) || (AmrEnum == AMR_MonthStatisticFreeze)) continue;
		#endif

		if(QCSGFp.DataDensity == 0)
		{
			QCSGFp.DataDensity = GetDefaultDataDensityByAMREnum(AmrEnum);
		}
		if(CheckDataDensity(AmrEnum, QCSGFp.DataDensity, 0, 0) != NO_ERROR) continue;

		ConvertTimeToDataDensityTime(&QCSGFp.StartTime, QCSGFp.DataDensity);
		if(TimeCompare(AmrEnum, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		
		if(LastPn != QCSGFp.Pn)
		{
			TmpPnListNum = SplitPnToList(QCSGFp.Pn, PnList, PN_CHECK_DISABLE);
			LastPn = QCSGFp.Pn;
		}	
		i = 0;
		while(i < TmpPnListNum)
		{
			PnListDataBuf[TotalPnDataLen++] = PnList[i];
			PnListDataBuf[TotalPnDataLen++] = PnList[i] >> 8;
			LenPos = TotalPnDataLen;
			PnListDataBuf[TotalPnDataLen++] = 0;
			PnListDataBuf[TotalPnDataLen++] = 0;
			
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 8;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 16;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 24;
			Result = NAND_ReadMeasurePoint(PnList[i], 0xE0800000, (u8*)&MeasurePoint, &DataLen);
			SinglePnDataLen = 0;
			MR((u32)&DataTime, (u32)&QCSGFp.StartTime, sizeof(RealTimeYYStruct));
			while(TimeCompare(AmrEnum, &DataTime, &QCSGFp.EndTime) == ERROR_TIME_IS_SMALLER)
			{
				DataLen = 0;
				switch(AmrEnum)
				{
					case AMR_DailyFreeze:
						Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					case AMR_DailyStatisticFreeze:
						Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					case AMR_MonthFreeze:
						Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					case AMR_MonthStatisticFreeze:
						Result = NAND_ReadMeterAMRData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					case AMR_CurveFreeze:
						//#if ((Project/100)==2)
						//	#if (TEST_ON == 1)	// 此处用途是测试曲线数据，发送的起始时间小 1 个小时，因此第一次读取曲线数据时，增加 1 小时
						//		if(SinglePnDataLen == 0) AddTimeByDataDensity(&DataTime, QCSGFp.DataDensity);
						//	#endif
						//	Result = NAND_ReadMeterCurveData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						//	#if (TEST_ON == 1)	// 读取完毕后，恢复为起始时间
						//		if(SinglePnDataLen == 0) DecTimeByDataDensity(&DataTime, QCSGFp.DataDensity);
						//	#endif
						//#else
						//	Result = NAND_ReadMeterCurveData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						//#endif
						Result = NAND_ReadMeterCurveData(AmrEnum, MeasurePoint.Addr, &AMRFileHead, QCSGFp.CurDataID, &DataTime, PnData, &DataLen);
						break;
					default:
						break;
				}
				if(Result == NO_ERROR) DataExist++;
				if(DataLen != 0)
				{
					MemCopy(&PnListDataBuf[TotalPnDataLen], PnData, DataLen);
					TotalPnDataLen += DataLen;
					SinglePnDataLen += (u16)DataLen;
					MemCopy(&PnListDataBuf[TotalPnDataLen], (u8*)&DataTime, 6);
					TotalPnDataLen += 6;
					SinglePnDataLen += 6;
				}	
				if(TotalPnDataLen >= DataMaxLen) break;
				AddTimeByDataDensity(&DataTime, QCSGFp.DataDensity);
			}
			if(SinglePnDataLen != 0)
			{
				PnListDataBuf[LenPos] = SinglePnDataLen;
				PnListDataBuf[LenPos + 1] = SinglePnDataLen >> 8;
				TotalPnListNum++;
			}
			else
			{
				TotalPnDataLen -= 8;
			}
			i++;
			if(TotalPnDataLen >= DataMaxLen) break;
		}	
		if(TotalPnDataLen >= DataMaxLen) break;
	}

//	if(DataExist == 0)
//	{
//		TotalPnDataLen = 0;
//		TotalPnListNum = 0;
//	}

	DataMaxLen = GetUartPortTxBufLen(PORTn);
	if((TotalPnDataLen > 0) && (TotalPnDataLen <= (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum)))
	{
		DstFrame->FC = SrcFrame->FC;
		DstFrame->FC |= QCSG_FC_DIR_BIT;
		DstFrame->FC &= ~QCSG_FC_PRM_BIT;
		DstFrame->FC &= ~QCSG_FC_ACD_BIT;
		DstFrame->A1 = SrcFrame->A1;
		DstFrame->A2 = SrcFrame->A2;
		DstFrame->A3 = SrcFrame->A3;
		DstFrame->AFN = SrcFrame->AFN;
		DstFrame->SEQ = SrcFrame->SEQ & (~(QCSG_SEQ_TPV_BIT + QCSG_SEQ_CON_BIT));

		DataLen = 0;
		DstFrame->DataLen = 0;		
		while(DataLen < TotalPnDataLen)
		{
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];		// 获取 Pn
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			SinglePnDataLen = PnListDataBuf[DataLen + 1];						// 获取 Pn 对应的数据长度
			SinglePnDataLen <<= 8;
			SinglePnDataLen += PnListDataBuf[DataLen];
			DataLen += 2;

			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];		// 获取 DI
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			DstFrame->Data[DstFrame->DataLen++] = PnListDataBuf[DataLen++];
			// 根据长度获取 Pn 对应的数据
			MR((u32)&DstFrame->Data[DstFrame->DataLen], (u32)&PnListDataBuf[DataLen], SinglePnDataLen);
			DstFrame->DataLen += SinglePnDataLen;
			DataLen += SinglePnDataLen;
		}
		return NO_ERROR;
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
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=0D
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

*/








/************************ (C) COPYRIGHT M***********************END OF FILE****/


