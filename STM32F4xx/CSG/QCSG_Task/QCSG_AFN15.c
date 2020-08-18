/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN15.c
作  者:luxiaowen
版  本:1.00
日  期:20190501
描  述:南网AFN15处理文件
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../STM32F4xx/STM32F4xx_ADC.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../QCSG_Head/QCSG_AFN15.h"
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

ResultEnum QCSG_AFN15_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	QCSGFrameProcessStruct QCSGFp;
	MultiFrameSendReqStruct MultiFrameSendReq;
	ResultEnum Result = NO_ERROR;
	u32 TotalPnDataLen = 0, LenPos = 0, DataMaxLen = 0,ResultAddr = 0;
	u16 i = 0, PnDataPos = 0, SinglePnDataLen = 0, TmpPnListNum = 0, LastPn = QCSG_INVALID_PN;
	u16 DataLen = 0, TotalPnListNum = 0;
	u16* PnList = (u16*)(ADDR_PnListBuf);
	u8* PnListDataBuf = NULL;
	u8  Voltage[6];
	
	PnListDataBuf = (u8*)GetUart_QCSG_DataBufAddr(PORTn);
	DataMaxLen = GetUart_QCSG_DataBufLen(PORTn);
	MC(0,(u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	QCSGFp.PORTn = PORTn;
	QCSGFp.UserDataLen = SrcFrame->DataLen;
    while(QCSGFp.SrcDataPos < QCSGFp.UserDataLen)
    {
		QCSGFp.Pn = QCSG_TakePn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 2;
		QCSGFp.CurDataID = QCSG_TakeFn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 4;

		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0))
		{
			continue;
		}
		else
		{
			if(LastPn != QCSGFp.Pn)
			{
				TmpPnListNum = SplitPnToList(QCSGFp.Pn, PnList, PN_CHECK_DISABLE);			
				LastPn = QCSGFp.Pn; 
			}
		}
		
		i = 0;
		while(i < TmpPnListNum)
		{
			if(TotalPnDataLen >= DataMaxLen) break;
			QCSGFp.TempPn = PnList[i];
			PnListDataBuf[TotalPnDataLen++] = PnList[i];
			PnListDataBuf[TotalPnDataLen++] = PnList[i] >> 8;
			LenPos = TotalPnDataLen;
			TotalPnDataLen += 2;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 8;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 16;
			PnListDataBuf[TotalPnDataLen++] = QCSGFp.CurDataID >> 24;
			TotalPnListNum++;
			i++;
			if(QCSGFp.TempPn != 0) 
			{
				TotalPnDataLen -= 8;
				continue;
			}
			switch(QCSGFp.CurDataID)
			{

				case 0xFFFF7F00://进入工厂自测
					 PnDataPos = QCSGFp.DstDataPos;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;				
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;
					 DataLen = QCSGFp.DstDataPos - PnDataPos;
					 QCSGFp.DstDataPos -= DataLen;
					 pTSysCtrl->Flag |= TERMINAL_GO_TO_TEST_FLAG;
					 break;
			   case 0xFFFF0200://读工厂自测结果
					 PnDataPos = QCSGFp.DstDataPos;
					 ResultAddr = ADDR_Characteristics_BASE1 + OFFSET_TestResult_Characteristics;
					 MR((u32)&DstFrame->Data[QCSGFp.DstDataPos],ResultAddr, 1);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
					 QCSGFp.DstDataPos++;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;	
					 DataLen = QCSGFp.DstDataPos - PnDataPos;
					 QCSGFp.DstDataPos -= DataLen;
					 break;
			   case 0xFFFF0300://读4.8V可充电电池电压
			   		 Charge_Battery_Voltage();//可充电4.8V电池电压ADC测量
					 PnDataPos = QCSGFp.DstDataPos;
					 DstFrame->Data[QCSGFp.DstDataPos++] = (u8)pCommRam->UBACKUP;
					 DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(pCommRam->UBACKUP>>8);
					 DataLen = QCSGFp.DstDataPos - PnDataPos;
					 QCSGFp.DstDataPos -= DataLen;
					 break;
			  case 0xFFFF0400://读RTC电池电压
			   		 RTC_Battery_Voltage();//RTC电池电压ADC测量
					 PnDataPos = QCSGFp.DstDataPos;
					 DstFrame->Data[QCSGFp.DstDataPos++] = (u8)pCommRam->URTC;
					 DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(pCommRam->URTC>>8);
					 DataLen = QCSGFp.DstDataPos - PnDataPos;
					 QCSGFp.DstDataPos -= DataLen;
					 break;
				case 0xFFFF0500://读交采电压规格
					 PnDataPos = QCSGFp.DstDataPos;
					 MR((u32)Voltage, (u32)(ADDR_04000401+44), 6);
				   //暂时由终端转换ASCLL码,以后由MES转换解析
					 if(Voltage[2] == 0x2E) //220V
				 	 {
				 	   DstFrame->Data[QCSGFp.DstDataPos] = Voltage[3] - 0x30;
					   DstFrame->Data[QCSGFp.DstDataPos] += ((Voltage[4] - 0x30)<<4);
					   QCSGFp.DstDataPos++;
					   DstFrame->Data[QCSGFp.DstDataPos++] = Voltage[5] - 0x30;
				 	 }
					 else//57.7         
				 	 {
				 	    DstFrame->Data[QCSGFp.DstDataPos] = Voltage[2] - 0x30;
					    DstFrame->Data[QCSGFp.DstDataPos] += ((Voltage[4] - 0x30)<<4);
				 		  QCSGFp.DstDataPos++;
					    DstFrame->Data[QCSGFp.DstDataPos++] = Voltage[5] - 0x30;
				 	 }
					 DataLen = QCSGFp.DstDataPos - PnDataPos;
					 QCSGFp.DstDataPos -= DataLen;
					 break;
			  case 0xFFFF8000://退出工厂自测
					 PnDataPos = QCSGFp.DstDataPos;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;				
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;
					 DstFrame->Data[QCSGFp.DstDataPos++] = 0x00;
					 DataLen = QCSGFp.DstDataPos - PnDataPos;
					 QCSGFp.DstDataPos -= DataLen;
					 MC(0x00, (u32)(ADDR_MeterFMTimer+14), 2);//清工厂模式分定时器
				   MC(0x00, (u32)(ADDR_TerminalFMTimer+14),2);//清工厂模式分定时器
					 break;
			  default:
			  	    DataLen = 0;
					break;
			}

			if(DataLen != 0)
			{
				MR((u32)&PnListDataBuf[TotalPnDataLen], (u32)&DstFrame->Data[PnDataPos], DataLen);
				TotalPnDataLen += DataLen;
				PnListDataBuf[LenPos] = DataLen;
				PnListDataBuf[LenPos + 1] = DataLen >> 8;
			}
			else
			{
				TotalPnDataLen -= 8;
			}
		}
		if(TotalPnDataLen >= DataMaxLen) break;						
	}

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
		QCSGFp.DstDataPos = 0;
		while(DataLen < (u16)TotalPnDataLen)
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
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=0A
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

/**************************************** (C) COPYRIGHT luxiaowen **********************************END OF FILE****/


