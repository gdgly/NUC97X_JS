/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN10.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN10处理文件
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
#include "../QCSG_Head/QCSG_AFN10.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
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

ResultEnum QCSG_AFN10_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	RS485PortParaStruct RS485PortPara;
	u8* PnListData;
	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
	#else

	//#if ((Project/100)==2) || ((Project/100)==5)
		#if ((Project/100)==9)
			u8 u8Flag_JC = 0, u8Flag_Pn = 0; 
			QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
		#endif
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
		
	#endif
	//NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	ResultEnum Result = NO_ERROR;
	u16* PnList = (u16*)(ADDR_PnListBuf);
	u32 Addr_Len = 0, TotalPnListNum = 0, TotalPnListLen = 0, MaxPnDataLen = 0;
	u8 RelayType = 0, RelayDstPort = 0, RelayBaudRate = 0, RelayParityMode = 0;//i = 0, ;
	u8 RelayDataBits = 0, RelayStopBits = 0, RelayTimeOut = 0, RelayFrameLen = 0;	
	
	MC(0, (u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	PnListData = (u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
	MaxPnDataLen = Get_LEN_UARTnFnDataBuff(PORTn);
	QCSGFp.PORTn = PORTn;
	QCSGFp.UserDataLen = SrcFrame->DataLen;
    while(QCSGFp.SrcDataPos < QCSGFp.UserDataLen)
    {
		QCSGFp.Pn = QCSG_TakePn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 2;
		QCSGFp.CurDataID = QCSG_TakeFn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 4;
		
		RelayType       = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayDstPort    = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayBaudRate   = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayParityMode = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayDataBits   = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayStopBits   = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayTimeOut    = SrcFrame->Data[QCSGFp.SrcDataPos++];
		RelayFrameLen   = SrcFrame->Data[QCSGFp.SrcDataPos++];
		
 		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0))
		{
			QCSGFp.SrcDataPos += RelayFrameLen;
			continue;
		}
		if(SplitPnToList(QCSGFp.Pn, PnList, PN_CHECK_DISABLE) > 1)
		{
			QCSGFp.SrcDataPos += RelayFrameLen;
			continue;
		}
		if(QCSGFp.CurDataID != 0xE3020001)
		{
			QCSGFp.SrcDataPos += RelayFrameLen;
			continue;
		}	
		if(RelayType > RELAY_TYPE_RELEASE) continue;
		
		if(DLT645FrameCheck(&SrcFrame->Data[QCSGFp.SrcDataPos], RelayFrameLen) != NO_ERROR)
		{
			if(EDMI_FrameCheck(&SrcFrame->Data[QCSGFp.SrcDataPos],RelayFrameLen) != NO_ERROR)
			{
				QCSGFp.SrcDataPos += RelayFrameLen;
				continue;
			}
		}
		QCSGFp.TempPn = SwitchPnToNum(QCSGFp.Pn);
/*
		for(i = 0; i < RelayFrameLen; i++)
		{
			if(SrcFrame->Data[QCSGFp.SrcDataPos + i] == DLT_645_HEAD) break;
		}	
		if(MemCompare(&SrcFrame->Data[QCSGFp.SrcDataPos + i + 1], NodeList[QCSGFp.TempPn].Addr, METER_ADDR_SIZE) != 0)
		{
			QCSGFp.SrcDataPos += RelayFrameLen;
			continue;
		}
*/			
		if((RelayDstPort == PORT_ID_RS485_1) || (RelayDstPort == PORT_ID_RS485_2) || RelayDstPort == PORT_ID_RS485_3)
		{
			Addr_Len =  RelayDstPort * sizeof(RS485PortParaStruct);
			Addr_Len += ADDR_RS485PortParaTbl;
			MR((u32)&RS485PortPara, Addr_Len, sizeof(RS485PortParaStruct));
			if(RS485PortPara.Function != UART_FUNC_AMR)//终端端口参数设置不是485抄表，跳过该转发指令。
			{
				QCSGFp.SrcDataPos += RelayFrameLen;
				continue;
			}
		}
		else if((RelayDstPort == PORT_ID_PLC) || (RelayDstPort == PORT_ID_WIRELESS))
		{
			RelayType       = RelayType;
			RelayDstPort    = RelayDstPort;
			RelayBaudRate   = RelayBaudRate;
			RelayParityMode = RelayParityMode;
			RelayDataBits   = RelayDataBits;
			RelayStopBits   = RelayStopBits;
			RelayTimeOut    = RelayTimeOut;
			RelayFrameLen   = RelayFrameLen;
			
//			if((NodeList[QCSGFp.TempPn].PortID != PORT_ID_PLC) && (NodeList[QCSGFp.TempPn].PortID != PORT_ID_WIRELESS))
//			{
//				QCSGFp.SrcDataPos += RelayFrameLen;
//				continue;
//			}			
		}
		else
		{
			QCSGFp.SrcDataPos += RelayFrameLen;
			continue;
		}

		Addr_Len = RelayFrameLen;
		Addr_Len += 8;
			
		PnListData[TotalPnListLen++] = QCSGFp.Pn;
		PnListData[TotalPnListLen++] = QCSGFp.Pn >> 8;
		PnListData[TotalPnListLen++] = Addr_Len;
		PnListData[TotalPnListLen++] = Addr_Len >> 8;
		PnListData[TotalPnListLen++] = QCSGFp.CurDataID;
		PnListData[TotalPnListLen++] = QCSGFp.CurDataID >> 8;
		PnListData[TotalPnListLen++] = QCSGFp.CurDataID >> 16;
		PnListData[TotalPnListLen++] = QCSGFp.CurDataID >> 24;
		MR((u32)&PnListData[TotalPnListLen],  (u32)&SrcFrame->Data[QCSGFp.SrcDataPos - 8], Addr_Len);
		TotalPnListLen += Addr_Len;
		TotalPnListNum++;
		QCSGFp.SrcDataPos += RelayFrameLen;
		if(TotalPnListLen >= MaxPnDataLen) break;
	}
	
	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(PORTn);
		if((TotalPnListNum > 0) && (RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{		
			RT_AMRReq->SrcTask = GetUartTaskID(PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_QCSG_UP;
			RT_AMRReq->DataType  = DATA_TYPE_RELAY;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = PORTn;
			RT_AMRReq->PnListNum = TotalPnListNum;
			RT_AMRReq->PnListLen = TotalPnListLen;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, TotalPnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
		else
		{
			TotalPnListNum = 0;
		}
	//#else

	#elif ((Project/100)==2) || ((Project/100)==5)	//err20180518 I 型集中器、 II 型集中器
		
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
		if((TotalPnListNum > 0) && (UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			UartRxChCtrl->SrcPort = PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_QCSG_UP;
			UartRxChCtrl->DataType = DATA_TYPE_RELAY;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = PORTn;
			UartRxChCtrl->PnListNum  = TotalPnListNum;
			UartRxChCtrl->FrameLen = TotalPnListLen;
			UartRxChCtrl->FrameBuf = PnListData;
			switch((u8)PORTn)
			{
				case RS485_1PORT:
					RT_AMRTask->Flag |= RT_AMR_RS485_CH1_REQ;
					break;
				case RS485_2PORT:
					RT_AMRTask->Flag |= RT_AMR_RS485_CH2_REQ;
					break;
				case RS485_3PORT:
					RT_AMRTask->Flag |= RT_AMR_RS485_CH3_REQ;
					break;
				case RS232PORT:
					RT_AMRTask->Flag |= RT_AMR_RS232_PORT_REQ;
					break;
				case GPRSPORT:
					RT_AMRTask->Flag |= RT_AMR_GPRS_PORT_REQ;
					break;
				case IRPORT:
					RT_AMRTask->Flag |= RT_AMR_IR_PORT_REQ;
					break;
				case ENETPORT:
					RT_AMRTask->Flag |= RT_AMR_ETHE_PORT_REQ;
					break;
				default:
					TotalPnListNum = 0;
					break;
			}
		}
		else
		{
			TotalPnListNum = 0;
		}
		
	#elif ((Project/100)==9) 
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(PORTn);
		if((TotalPnListNum > 0) && (RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{		
			u8Flag_JC = 1;
			RT_AMRReq->SrcTask = GetUartTaskID(PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_QCSG_UP;
			RT_AMRReq->DataType  = DATA_TYPE_RELAY;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = PORTn;
			RT_AMRReq->PnListNum = TotalPnListNum;
			RT_AMRReq->PnListLen = TotalPnListLen;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, TotalPnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
			
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
		if((TotalPnListNum > 0) && (UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
		 	u8Flag_Pn = 1 ;
			UartRxChCtrl->SrcPort = PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_QCSG_UP;
			UartRxChCtrl->DataType = DATA_TYPE_RELAY;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = PORTn;
			UartRxChCtrl->PnListNum  = TotalPnListNum;
			UartRxChCtrl->FrameLen = TotalPnListLen;
			UartRxChCtrl->FrameBuf = PnListData;
			switch((u8)PORTn)
			{
				case RS485_1PORT:
					RT_AMRTask->Flag |= RT_AMR_RS485_CH1_REQ;
					break;
				case RS485_2PORT:
					RT_AMRTask->Flag |= RT_AMR_RS485_CH2_REQ;
					break;
				case RS485_3PORT:
					RT_AMRTask->Flag |= RT_AMR_RS485_CH3_REQ;
					break;
				case RS232PORT:
					RT_AMRTask->Flag |= RT_AMR_RS232_PORT_REQ;
					break;
				case GPRSPORT:
					RT_AMRTask->Flag |= RT_AMR_GPRS_PORT_REQ;
					break;
				case IRPORT:
					RT_AMRTask->Flag |= RT_AMR_IR_PORT_REQ;
					break;
				case ENETPORT:
					RT_AMRTask->Flag |= RT_AMR_ETHE_PORT_REQ;
					break;
				default:
					TotalPnListNum = 0;
					break;
			}
		}
		
		if((u8Flag_JC == 0) && (u8Flag_Pn == 0))
		{
			TotalPnListNum = 0;
		}
	
		#endif

	
	
	if(TotalPnListNum == 0)
	{
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=10
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
		return Result;
	}
	return ERROR_DO_NOT_ANSWER;
}



/**************************************** (C) COPYRIGHT zhaoyun ***************************************END OF FILE****/

