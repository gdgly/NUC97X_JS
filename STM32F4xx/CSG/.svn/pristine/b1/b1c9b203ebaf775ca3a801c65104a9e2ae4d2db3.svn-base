/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0E.c
作  者:zhaoyun
版  本:1.00
日  期:20170801
描  述:南网AFN0E处理文件
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

#include "../QCSG_Head/QCSG_AFN0E.h"
#include "Terminal_Task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#define ERD01_DESC_NUM				(5)
#define ERD01_DATA_LEN				(16)
const DataDescriptorStruct ERD01Desc[ERD01_DESC_NUM] = 
{
	{0,  6},
	{8,  1},
	{9,  1},
	{12, 4},
	{16, 4}
};

#define ERD02_DESC_NUM				(1)
#define ERD02_DATA_LEN				(6)
const DataDescriptorStruct ERD02Desc[ERD02_DESC_NUM] = 
{
	{0, 6}
};

#define ERD03_DESC_NUM				(6)
#define ERD03_DATA_LEN				(28)
const DataDescriptorStruct ERD03Desc[ERD03_DESC_NUM] = 
{
	{0,  6},
	{8,  6},
	{16, 4},
	{20, 4},
	{24, 4},
	{28, 4}
};

#define ERD04_DESC_NUM				(2)
#define ERD04_DATA_LEN				(12)
const DataDescriptorStruct ERD04Desc[ERD04_DESC_NUM] = 
{
	{0, 6},
	{8, 6}
};

#define ERD05_DESC_NUM				(10)
#define ERD05_DATA_LEN				(28)
const DataDescriptorStruct ERD05Desc[ERD05_DESC_NUM] = 
{
	{0,  6},
	{8,  6},
	{16, 2},
	{18, 2},
	{20, 2},
	{22, 2},
	{24, 2},
	{26, 2},
	{28, 2},
	{30, 2}
};

#define ERD06_DESC_NUM				(4)
#define ERD06_DATA_LEN				(15)
const DataDescriptorStruct ERD06Desc[ERD06_DESC_NUM] = 
{
	{0,  6},
	{8,  1},
	{12, 4},
	{16, 4}
};


#define ERD07_DESC_NUM				(3)
#define ERD07_DATA_LEN				(23)
const DataDescriptorStruct ERD07Desc[ERD07_DESC_NUM] = 
{
	{0,  6},
	{8,  16},
	{24, 1}
};


#define ERD08_DESC_NUM				(2)
#define ERD08_DATA_LEN				(10)
const DataDescriptorStruct ERD08Desc[ERD08_DESC_NUM] = 
{
	{0,  6},
	{8,  4}
};


#define ERD09_DESC_NUM				(3)
#define ERD09_DATA_LEN				(14)
const DataDescriptorStruct ERD09Desc[ERD09_DESC_NUM] = 
{
	{0,  6},
	{8,  4},
	{12, 4}
};

#define EVENT_DI_NUM				(21)
const DIDataInfoStruct EventDIInfoTbl[EVENT_DI_NUM] = 
{
	{0xE2010001, 0, sizeof(ERD01Struct), ERD01_DATA_LEN,  R_ENABLE, ERD01_DESC_NUM, ERD01Desc},
	{0xE2010002, 0, sizeof(ERD02Struct), ERD02_DATA_LEN,  R_ENABLE, ERD02_DESC_NUM, ERD02Desc},
	{0xE2010003, 0, sizeof(ERD02Struct), ERD02_DATA_LEN,  R_ENABLE, ERD02_DESC_NUM, ERD02Desc},
	{0xE2010004, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010005, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010006, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010007, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010008, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010009, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},	
	{0xE201000A, 0, sizeof(ERD04Struct), ERD04_DATA_LEN,  R_ENABLE, ERD04_DESC_NUM, ERD04Desc},
	{0xE201000B, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE201000C, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE201000D, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},	
	{0xE201000E, 0, sizeof(ERD06Struct), ERD06_DATA_LEN,  R_ENABLE, ERD06_DESC_NUM, ERD06Desc},	
	{0xE201000F, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010010, 0, sizeof(ERD09Struct), ERD09_DATA_LEN,  R_ENABLE, ERD09_DESC_NUM, ERD09Desc},	
	{0xE2010011, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},
	{0xE2010012, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc},	
	{0xE2010013, 0, sizeof(ERD07Struct), ERD07_DATA_LEN,  R_ENABLE, ERD07_DESC_NUM, ERD07Desc},
	{0xE2010014, 0, sizeof(ERD08Struct), ERD08_DATA_LEN,  R_ENABLE, ERD08_DESC_NUM, ERD08Desc},
	{0xE2010015, 0, sizeof(ERD03Struct), ERD03_DATA_LEN,  R_ENABLE, ERD03_DESC_NUM, ERD03Desc}
	
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
u16 FindDIInEventDIInfoTbl(u32 DI);


/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
ResultEnum QCSG_AFN0E_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	MeasurePointStruct MeasurePoint;
	MultiFrameSendReqStruct MultiFrameSendReq;
	RealTimeStruct RealTime;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0, TotalPnDataLen = 0, LenPos = 0, TempDI = 0, DataMaxLen = 0;
	u16 i = 0, EventDataPos = 0, DIIndex = 0, SinglePnDataLen = 0, TmpPnListNum = 0, TotalPnListNum = 0, LastPn = QCSG_INVALID_PN;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	u8* PnListDataBuf = NULL;
	u16* PnList = (u16*)(ADDR_PnListBuf);	

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
		
		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0))
		{
			continue;
		}

		if((QCSGFp.StartTime.YearH == 0xFF) && (QCSGFp.StartTime.YearL == 0xFF) && (QCSGFp.StartTime.Month == 0xFF) && 
		   (QCSGFp.StartTime.Day == 0xFF) && (QCSGFp.StartTime.Hour == 0xFF) && (QCSGFp.StartTime.Minute == 0xFF) &&
		   (QCSGFp.EndTime.YearH == 0xFF) && (QCSGFp.EndTime.YearL == 0xFF) && (QCSGFp.EndTime.Month == 0xFF) && 
		   (QCSGFp.EndTime.Day == 0xFF) && (QCSGFp.EndTime.Hour == 0xFF) && (QCSGFp.EndTime.Minute == 0xFF))
		{

		}
		else
		{
			YYTimeToRealTime(&QCSGFp.StartTime, &RealTime);
			if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;
			YYTimeToRealTime(&QCSGFp.EndTime, &RealTime);
			if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;		
			if(TimeCompare(AMR_EVENT, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		}
		
		if(LastPn != QCSGFp.Pn)
		{
			TmpPnListNum = SplitPnToList(QCSGFp.Pn, PnList, PN_CHECK_DISABLE);
			LastPn = QCSGFp.Pn;
		}	
		
		i = 0;
		while(i < TmpPnListNum)
		{// 读事件记录，要求返回格式为: DI(4) L(2) Data
			Result = NAND_ReadMeasurePoint(PnList[i], 0xE0800000, (u8*)&MeasurePoint, &DataLen);
			SinglePnDataLen = 0;
			DataLen = 0;
			Result = NAND_ReadEventRecord(MeasurePoint.Addr, QCSGFp.CurDataID, &QCSGFp.StartTime, &QCSGFp.EndTime, PnData, &DataLen);
			EventDataPos = 0;
			while(EventDataPos < DataLen)
			{
				TempDI = 0;
				TempDI += PnData[EventDataPos++];
				TempDI += PnData[EventDataPos++] << 8;
				TempDI += PnData[EventDataPos++] << 16;
				TempDI += PnData[EventDataPos++] << 24;

				SinglePnDataLen = 0;
				SinglePnDataLen += PnData[EventDataPos++];
				SinglePnDataLen += (u16)PnData[EventDataPos++] << 8;
				DIIndex = FindDIInEventDIInfoTbl(TempDI);
				if((DIIndex >= EVENT_DI_NUM) || (SinglePnDataLen != EventDIInfoTbl[DIIndex].StoreLen))
				{
					//EventDataPos += EventDIInfo[DIIndex].StoreLen;
					EventDataPos += SinglePnDataLen;
					continue;
				}
				PnListDataBuf[TotalPnDataLen++] = PnList[i];
				PnListDataBuf[TotalPnDataLen++] = PnList[i] >> 8;
				LenPos = TotalPnDataLen;
				TotalPnDataLen += 2;
				PnListDataBuf[TotalPnDataLen++] = TempDI;
				PnListDataBuf[TotalPnDataLen++] = TempDI >> 8;
				PnListDataBuf[TotalPnDataLen++] = TempDI >> 16;
				PnListDataBuf[TotalPnDataLen++] = TempDI >> 24;
				SinglePnDataLen = SetAlarmEventDIDataByDIInfo(&PnListDataBuf[TotalPnDataLen], &PnData[EventDataPos], &EventDIInfoTbl[DIIndex]);
				TotalPnDataLen += SinglePnDataLen;
				PnListDataBuf[LenPos] = SinglePnDataLen;
				PnListDataBuf[LenPos + 1] = SinglePnDataLen >> 8;
				EventDataPos += EventDIInfoTbl[DIIndex].StoreLen;
			}
			i++;
			TotalPnListNum++;
			if(TotalPnDataLen >= DataMaxLen) break;
		}	
		if(TotalPnDataLen >= DataMaxLen) break;
	}

	DataMaxLen = GetUartPortTxBufLen(PORTn);
	if((TotalPnDataLen > 0) && (TotalPnDataLen <= (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum)))
	{
		DataLen = 0;
		QCSGFp.DstDataPos = 0;		
		while(DataLen < TotalPnDataLen)
		{
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];		// 获取 Pn
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			SinglePnDataLen = PnListDataBuf[DataLen + 1];						// 获取 Pn 对应的数据长度
			SinglePnDataLen <<= 8;
			SinglePnDataLen += PnListDataBuf[DataLen];
			DataLen += 2;

			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];		// 获取 DI
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			// 根据长度获取 Pn 对应的数据
			MR((u32)&DstFrame->Data[QCSGFp.DstDataPos], (u32)&PnListDataBuf[DataLen], SinglePnDataLen);
			QCSGFp.DstDataPos += SinglePnDataLen;
			DataLen += SinglePnDataLen;
		}	

		DstFrame->FC = SrcFrame->FC;
		DstFrame->FC |= QCSG_FC_DIR_BIT;
		DstFrame->FC &= ~QCSG_FC_PRM_BIT;
		DstFrame->FC &= ~QCSG_FC_ACD_BIT;
		DstFrame->A1 = SrcFrame->A1;
		DstFrame->A2 = SrcFrame->A2;
		DstFrame->A3 = SrcFrame->A3;
		DstFrame->AFN = SrcFrame->AFN;
		DstFrame->SEQ = SrcFrame->SEQ & (~(QCSG_SEQ_TPV_BIT + QCSG_SEQ_CON_BIT));
		DstFrame->DataLen = QCSGFp.DstDataPos;
		return NO_ERROR;
	}
	
	if(TotalPnDataLen > (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum))
	{
		MC(0, (u32)&MultiFrameSendReq, sizeof(MultiFrameSendReqStruct));
		MultiFrameSendReq.SrcTask = GetUartTaskID(PORTn);
		MultiFrameSendReq.RetryCnt = 0;
		MultiFrameSendReq.FrameType = FRAME_TYPE_QCSG_UP;
		MultiFrameSendReq.DataType = DATA_TYPE_STRUCT;
		MultiFrameSendReq.AckToSrcTask = 0;
		MultiFrameSendReq.NeedAck = 0;
		
		MultiFrameSendReq.QCSGFrameInfo.A1 = SrcFrame->A1;
		MultiFrameSendReq.QCSGFrameInfo.A2 = SrcFrame->A2;
		MultiFrameSendReq.QCSGFrameInfo.A3 = SrcFrame->A3;
		MultiFrameSendReq.QCSGFrameInfo.FC = SrcFrame->FC;
		MultiFrameSendReq.QCSGFrameInfo.AFN = SrcFrame->AFN;
		MultiFrameSendReq.QCSGFrameInfo.SEQ = SrcFrame->SEQ;
		MultiFrameSendReq.QCSGFrameInfo.Port = (u8)PORTn;
		MultiFrameSendReq.WaitAckTimeOut = 0;
		MultiFrameSendReq.PnListNum = TotalPnListNum;
		MultiFrameSendReq.DataLen = TotalPnDataLen;
		MultiFrameSendReq.DataBuf = PnListDataBuf;

		Result = QCSGMultiFrameSendReq(PORTn, &MultiFrameSendReq);
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
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=0E
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
u16 SetDIDataByDIInfo(u8* DstData, u8* SrcData, DIDataInfoStruct* DiInfo)
{
	u16 i = 0, Len = 0, DescNum = 0;

	for(DescNum = 0; DescNum < DiInfo->DescNum; DescNum++)
	{
		//MR((u32)&DstData[Len], (u32)&SrcData[DiInfo->Descriptor[DescNum].OffSet], DiInfo->Descriptor[DescNum].Len);
		//Len += DiInfo->Descriptor[DescNum].Len;
		for(i = 0; i < DiInfo->Descriptor[DescNum].Len; i++)
		{
			DstData[Len++] = SrcData[DiInfo->Descriptor[DescNum].OffSet + i];
		}
	}	
	return Len;
}
*/

u16 FindDIInEventDIInfoTbl(u32 DI)
{
	u16 i = 0;
	for(i = 0; i < EVENT_DI_NUM; i++)
	{
		if(DI == EventDIInfoTbl[i].DI) break;
	}
	return i;
}

/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/

