/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0F.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN0F处理文件
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
#include "../QCSG_Head/QCSG_AFN0F.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
ResultEnum QCSG_AFN0F_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0;
	
	MC(0, (u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	QCSGFp.PORTn = PORTn;
	QCSGFp.UserDataLen = SrcFrame->DataLen;
    while(QCSGFp.SrcDataPos < QCSGFp.UserDataLen)
    {
		QCSGFp.Pn = QCSG_TakePn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 2;
		QCSGFp.CurDataID = QCSG_TakeFn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 4;

		switch(QCSGFp.CurDataID)
		{
			case 0xE3010001:
			case 0xE3010002:				
				if(QCSGFp.Pn != 0)
				{
					Result = ERROR_VerifyFail;
					DataLen = 0;
				}
				else
				{
					HAL_GetSystemTime(&RealTime);
					RealTimeToYYTime(&RealTime, &YYTime);
					DataLen = QCSGFp.UserDataLen - QCSGFp.SrcDataPos;
					Result = NAND_ReadWriteTerminalExeFile(PORTn, QCSGFp.CurDataID, &YYTime, &SrcFrame->Data[QCSGFp.SrcDataPos], &DataLen);
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn >> 8;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 8;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 16;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 24;
					DstFrame->Data[QCSGFp.DstDataPos++] = Result;
					DataLen = QCSGFp.DstDataPos;
				}
				QCSGFp.SrcDataPos = QCSGFp.UserDataLen;		
				break;
			case 0xE3010003:				
				if(QCSGFp.Pn != 0)
				{
					Result = ERROR_InvalidData;
				}
				else
				{
					HAL_GetSystemTime(&RealTime);
					RealTimeToYYTime(&RealTime, &YYTime);	
					DataLen = QCSGFp.UserDataLen - QCSGFp.SrcDataPos;
					Result = NAND_ReadWriteTerminalExeFile(PORTn, QCSGFp.CurDataID, &YYTime, &SrcFrame->Data[QCSGFp.SrcDataPos], &DataLen);
				}				
				if(Result == ERROR_InvalidData)
				{
					DataLen = 0;
				}
				else
				{
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn >> 8;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 8;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 16;
					DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 24;
					MR((u32)&DstFrame->Data[QCSGFp.DstDataPos], (u32)&SrcFrame->Data[QCSGFp.SrcDataPos], DataLen);
					QCSGFp.DstDataPos += DataLen ;// 添加回复帧长度
				}
				QCSGFp.SrcDataPos = QCSGFp.UserDataLen;
				break;
			default:
				Result = ERROR_NoSuchDataID;
				break;
		}
	}

	if(DataLen == 0)
	{
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=0F
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
	else
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
		DstFrame->DataLen = QCSGFp.DstDataPos;
	}
	
	return Result;
}
/**************************************** (C) COPYRIGHT zhaoyun ***************************************END OF FILE****/

