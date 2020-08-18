/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0A.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN0A处理文件
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
#include "Terminal_Task.h"
#include "../QCSG_Head/QCSG_AFN0A.h"

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
功     能: 	南网上行协议写参数AFN=0A操作
入口参 数: 	PORTn: 帧源端口号
			SrcFrame: 输入的源数据帧指针
			DstFrame: 输出的应答数据帧指针
返     回: 返回操作是否成功
编     写: zhaoyun
编写时 间; 2017-7-10
*****************************************************************************************************/

ResultEnum QCSG_AFN0A_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	MultiFrameSendReqStruct MultiFrameSendReq;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	ResultEnum Result = NO_ERROR;
	u32 TotalPnDataLen = 0, LenPos = 0, DataMaxLen = 0;
	u16 i = 0, PnDataPos = 0, SinglePnDataLen = 0, TmpPnListNum = 0, LastPn = QCSG_INVALID_PN;
	u16 DataLen = 0, TotalPnListNum = 0;
	u16* PnList = (u16*)(ADDR_PnListBuf);
	u8* PnListDataBuf = NULL;
	
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
			if((QCSGFp.TempPn != 0) && (QCSGFp.CurDataID <= 0xE0002000))
			{
				TotalPnDataLen -= 8;
				continue;
			}
			if((0xE0000100 <= QCSGFp.CurDataID) && ((0xE000012F >= QCSGFp.CurDataID)))
			{
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000100_E000012F(SrcFrame, DstFrame, &QCSGFp);				// 通信参数
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000130) && (QCSGFp.CurDataID <= 0xE0000174))  	// 时间参数、复位操作、告警屏蔽、结算日参数、身份认证
			{
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000130_E0000174(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000180) && (QCSGFp.CurDataID <= 0xE000018F))   	// 阀值参数
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000180_E000018F(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000200) && (QCSGFp.CurDataID <= 0xE0000220))   	//抄表参数
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000200_E0000220(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000221) && (QCSGFp.CurDataID <= 0xE0000250))   	//抄表通道配置表参数
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000221_E0000250(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000300) && (QCSGFp.CurDataID <= 0xE00003FE))  	//任务参数  普通
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000300_E00003FE(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000400) && (QCSGFp.CurDataID <= 0xE00004FE))  	//任务参数  中继
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000400_E00004FE(SrcFrame, DstFrame, &QCSGFp);			
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if(((QCSGFp.CurDataID >= 0xE0000500) && (QCSGFp.CurDataID <= 0xE00005FF)) || (QCSGFp.CurDataID == 0xE0000900))  //运算量参数   模拟量
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000500_E00005FF(SrcFrame, DstFrame, &QCSGFp);			
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000600) && (QCSGFp.CurDataID <= 0xE00006FF))  	//运算量参数   脉冲量
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000600_E00006FF(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000700) && (QCSGFp.CurDataID <= 0xE00007FF))  	//运算量参数   运算量
			{			
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000700_E00007FF(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000800) && (QCSGFp.CurDataID <= 0xE00008FF))  	//运算量参数   差动量
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000800_E00008FF(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000A00) && (QCSGFp.CurDataID <= 0xE0000B02))  	//端口参数、级联参数、级联控制、版本参数    
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000A00_E0000B02(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0000C00) && (QCSGFp.CurDataID <= 0xE0000E11))  	//负荷管理参数
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0000C00_E0000E11(SrcFrame, DstFrame, &QCSGFp);			
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0001100) && (QCSGFp.CurDataID <= 0xE0002000))  	//测量点控制参数、用户自定义数据   
			{				
				PnDataPos = QCSGFp.DstDataPos;
				Result = QCSG_AFN0A_E0001100_E0002000(SrcFrame, DstFrame, &QCSGFp);				
				DataLen = QCSGFp.DstDataPos - PnDataPos;
				QCSGFp.DstDataPos -= DataLen;
			}
			else if((QCSGFp.CurDataID >= 0xE0800000) && (QCSGFp.CurDataID <= 0xE080002F))  	//测量点参数、电能质量检测参数
			{				
				if(SwitchPnToNum(QCSGFp.TempPn) < MAX_PN_NUM)
				{
					PnDataPos = QCSGFp.DstDataPos;
					Result = QCSG_AFN0A_E0800000_E080002F(SrcFrame, DstFrame, &QCSGFp);				
					DataLen = QCSGFp.DstDataPos - PnDataPos;
					QCSGFp.DstDataPos -= DataLen;
				}
				else
				{
					DataLen = 0;
				}
			}
			else if((QCSGFp.CurDataID >= 0xE0800030) && (QCSGFp.CurDataID <= 0xE0800031))  	//err20190403相位信息、台区信息
			{				
				if(SwitchPnToNum(QCSGFp.TempPn) < MAX_PN_NUM)
				{
					PnDataPos = QCSGFp.DstDataPos;
					Result = QCSG_AFN0A_E0800030_E0800031(SrcFrame, DstFrame, &QCSGFp);				
					DataLen = QCSGFp.DstDataPos - PnDataPos;
					QCSGFp.DstDataPos -= DataLen;
				}
				else
				{
					DataLen = 0;
				}
			}
			else if((QCSGFp.CurDataID >= 0xE0800100) && (QCSGFp.CurDataID <= 0xE080020E))	//电能质量检测参数
			{
				if(SwitchPnToNum(QCSGFp.TempPn) < MAX_PN_NUM)
				{
					PnDataPos = QCSGFp.DstDataPos;
					Result = QCSG_AFN0A_E0800100_E080020E(SrcFrame, DstFrame, &QCSGFp);			
					DataLen = QCSGFp.DstDataPos - PnDataPos;
					QCSGFp.DstDataPos -= DataLen;
				}
				else
				{
					DataLen = 0;
				}	
			}
			else
			{
				DataLen = 0;
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
		//DstFrame->A1 = SrcFrame->A1;
		//DstFrame->A2 = SrcFrame->A2;
		DstFrame->A1 = pTSysCtrl->TerminalLocation & 0x00FFFFFF;
		DstFrame->A2 = pTSysCtrl->TerminalAddr & 0x00FFFFFF;
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
		
		//MultiFrameSendReq.QCSGFrameInfo.A1 = SrcFrame->A1;
		//MultiFrameSendReq.QCSGFrameInfo.A2 = SrcFrame->A2;
		MultiFrameSendReq.QCSGFrameInfo.A1 = pTSysCtrl->TerminalLocation & 0x00FFFFFF;
		MultiFrameSendReq.QCSGFrameInfo.A2 = pTSysCtrl->TerminalAddr & 0x00FFFFFF;
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
		//DstFrame->A1 = SrcFrame->A1;
		//DstFrame->A2 = SrcFrame->A2;
		DstFrame->A1 = pTSysCtrl->TerminalLocation & 0x00FFFFFF;
		DstFrame->A2 = pTSysCtrl->TerminalAddr & 0x00FFFFFF;
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

ResultEnum QCSG_AFN0A_E0000100_E000012F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	CommunicationParaStruct CommPara;

#if ((Project/100 == 3) && (USER == 3400))
	E000011x_CommunicationParaStruct E0000111_CommPara;
	E000011x_CommunicationParaStruct E0000112_CommPara;
	E000011x_CommunicationParaStruct E0000113_CommPara;
	MemSet((u8*)&E0000111_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
	MemSet((u8*)&E0000112_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
	MemSet((u8*)&E0000113_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));

	MR((u32)&E0000111_CommPara, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));
	MR((u32)&E0000112_CommPara, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));
	MR((u32)&E0000113_CommPara, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));
#endif	
	
	MemSet((u8*)&CommPara, 0, sizeof(CommunicationParaStruct));
	MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000100:
			QCSGFp->DstDataPos += 9;
			Result = QCSG_AFN0A_E0000100_Read(pData, &CommPara);
			break;
		case 0xE0000101:
			QCSGFp->DstDataPos += 9;
			Result = QCSG_AFN0A_E0000101_Read(pData, &CommPara);
			break;
		case 0xE0000102:
			QCSGFp->DstDataPos += 9;
			Result = QCSG_AFN0A_E0000102_Read(pData, &CommPara);
			break;
		case 0xE0000103:
			QCSGFp->DstDataPos += 8;
			Result = QCSG_AFN0A_E0000103_Read(pData, &CommPara);
			break;
		case 0xE0000104:
			QCSGFp->DstDataPos += 16;
			Result = QCSG_AFN0A_E0000104_Read(pData, &CommPara);
			break;
		case 0xE0000105:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E0000105_Read(pData, &CommPara);
			break;
		case 0xE0000106:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E0000106_Read(pData, &CommPara);
			break;
		case 0xE0000107:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000107_Read(pData, &CommPara);
			break;
		case 0xE0000108:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000108_Read(pData, &CommPara);
			break;
		case 0xE0000109:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000109_Read(pData, &CommPara);
			break;
		case 0xE000010A:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E000010A_Read(pData, &CommPara);
			break;
		case 0xE000010B:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E000010B_Read(pData, &CommPara);
			break;
		case 0xE000010F:
			QCSGFp->DstDataPos += 120;
			Result = QCSG_AFN0A_E000010F_Read(pData, &CommPara);
			break;
			
	#if ((Project/100 == 3) && (USER == 3400))//err20190528 增加云南负控GPRS自适应参数
		case 0xE0000111://err20190528 增加自适应参数对应数据标识
			QCSGFp->DstDataPos += 420;
			Result = QCSG_AFN0A_E0000111_Read(pData, &E0000111_CommPara);
			break;
		case 0xE0000112:
			QCSGFp->DstDataPos += 420;
			Result = QCSG_AFN0A_E0000112_Read(pData, &E0000112_CommPara);
			break;
		case 0xE0000113:
			QCSGFp->DstDataPos += 420;
			Result = QCSG_AFN0A_E0000113_Read(pData, &E0000113_CommPara);
			break;
	#endif
			
		case 0xE0000120:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E0000120_Read(pData, &CommPara);
			break;
		case 0xE0000121:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E0000121_Read(pData, &CommPara);
			break;
		case 0xE0000122:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000122_Read(pData, &CommPara);
			break;
		case 0xE0000123:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0000123_Read(pData, &CommPara);
			break;
		case 0xE0000124:
			QCSGFp->DstDataPos += 4;
			Result = QCSG_AFN0A_E0000124_Read(pData, &CommPara);
			break;
		case 0xE0000125:
			QCSGFp->DstDataPos += 4;
			Result = QCSG_AFN0A_E0000125_Read(pData, &CommPara);
			break;
		case 0xE0000126:
			QCSGFp->DstDataPos += 4;
			Result = QCSG_AFN0A_E0000126_Read(pData, &CommPara);
			break;
		case 0xE0000127:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000127_Read(pData, &CommPara);
			break;
		case 0xE000012F:
			QCSGFp->DstDataPos += 22;
			Result = QCSG_AFN0A_E000012F_Read(pData, &CommPara);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}

ResultEnum QCSG_AFN0A_E0000100_Read(u8* Data, CommunicationParaStruct* Para)
{	
	MemCopy(Data, Para->MasterAddr, COMM_AADR_SIZE);	
	Data[8] = Para->MasterAddrType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000101_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, Para->MasterBackAddr1, COMM_AADR_SIZE);
	Data[8] = Para->MasterBackAddr1Type;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000102_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, Para->MasterBackAddr2, COMM_AADR_SIZE);
	Data[8] = Para->MasterBackAddr2Type;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000103_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, Para->GateWayAddr, COMM_AADR_SIZE);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000104_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, Para->APN, APN_NAME_MAX_SIZE);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000105_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, Para->APNUserName, APN_USER_NAME_MAX_SIZE);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000106_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, Para->APNPassWord, APN_USER_PWD_MAX_SIZE);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000107_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->HeartBeatTimeGap;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000108_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->RedialTimeGap;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000109_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->RedialCnt;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000010A_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->IpLinkType;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000010B_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->CompressType;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000010F_Read(u8* Data, CommunicationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000100_Read(Data, Para);
	Data += 9;
	Result += QCSG_AFN0A_E0000101_Read(Data, Para);
	Data += 9;
	Result += QCSG_AFN0A_E0000102_Read(Data, Para);
	Data += 9;
	Result += QCSG_AFN0A_E0000103_Read(Data, Para);
	Data += 8;
	Result += QCSG_AFN0A_E0000104_Read(Data, Para);
	Data += 16;
	Result += QCSG_AFN0A_E0000105_Read(Data, Para);
	Data += 32;
	Result += QCSG_AFN0A_E0000106_Read(Data, Para);
	Data += 32;
	Result += QCSG_AFN0A_E0000107_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000108_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000109_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E000010A_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E000010B_Read(Data, Para);
	Data += 1;
	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}

ResultEnum QCSG_AFN0A_E0000111_Read(u8* Data, E000011x_CommunicationParaStruct* Para)
{
	MemCopy(&Data[0], (u8*)&Para->MasterAddr, COMM_AADR_SIZE);
	MemCopy(&Data[9], (u8*)&Para->MasterBackAddr1, COMM_AADR_SIZE);
	MemCopy(&Data[18], (u8*)&Para->MasterBackAddr2, COMM_AADR_SIZE);
	MemCopy(&Data[27], (u8*)&Para->GateWayAddr, COMM_AADR_SIZE);

	Data[8] = Para->MasterAddrType;
	Data[17] = Para->MasterBackAddr1Type;
	Data[26] = Para->MasterBackAddr2Type;
	Data[35] = Para->MasterBackAddr3Type;

	MemCopy(&Data[36], (u8*)&Para->APN1, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[68], (u8*)&Para->APN1UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[100], (u8*)&Para->APN1PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[132], (u8*)&Para->APN2, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[164], (u8*)&Para->APN2UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[196], (u8*)&Para->APN2PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[228], (u8*)&Para->APN3, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[260], (u8*)&Para->APN3UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[292], (u8*)&Para->APN3PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[324], (u8*)&Para->APN4, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[356], (u8*)&Para->APN4UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[388], (u8*)&Para->APN4PassWord, APN_USER_PWD_MAX_SIZE);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000112_Read(u8* Data, E000011x_CommunicationParaStruct* Para)
{
	MemCopy(&Data[0], (u8*)&Para->MasterAddr, COMM_AADR_SIZE);
	MemCopy(&Data[9], (u8*)&Para->MasterBackAddr1, COMM_AADR_SIZE);
	MemCopy(&Data[18], (u8*)&Para->MasterBackAddr2, COMM_AADR_SIZE);
	MemCopy(&Data[27], (u8*)&Para->GateWayAddr, COMM_AADR_SIZE);

	Data[8] = Para->MasterAddrType;
	Data[17] = Para->MasterBackAddr1Type;
	Data[26] = Para->MasterBackAddr2Type;
	Data[35] = Para->MasterBackAddr3Type;

	MemCopy(&Data[36], (u8*)&Para->APN1, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[68], (u8*)&Para->APN1UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[100], (u8*)&Para->APN1PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[132], (u8*)&Para->APN2, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[164], (u8*)&Para->APN2UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[196], (u8*)&Para->APN2PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[228], (u8*)&Para->APN3, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[260], (u8*)&Para->APN3UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[292], (u8*)&Para->APN3PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[324], (u8*)&Para->APN4, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[356], (u8*)&Para->APN4UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[388], (u8*)&Para->APN4PassWord, APN_USER_PWD_MAX_SIZE);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000113_Read(u8* Data, E000011x_CommunicationParaStruct* Para)
{
	MemCopy(&Data[0], (u8*)&Para->MasterAddr, COMM_AADR_SIZE);
	MemCopy(&Data[9], (u8*)&Para->MasterBackAddr1, COMM_AADR_SIZE);
	MemCopy(&Data[18], (u8*)&Para->MasterBackAddr2, COMM_AADR_SIZE);
	MemCopy(&Data[27], (u8*)&Para->GateWayAddr, COMM_AADR_SIZE);

	Data[8] = Para->MasterAddrType;
	Data[17] = Para->MasterBackAddr1Type;
	Data[26] = Para->MasterBackAddr2Type;
	Data[35] = Para->MasterBackAddr3Type;

	MemCopy(&Data[36], (u8*)&Para->APN1, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[68], (u8*)&Para->APN1UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[100], (u8*)&Para->APN1PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[132], (u8*)&Para->APN2, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[164], (u8*)&Para->APN2UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[196], (u8*)&Para->APN2PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[228], (u8*)&Para->APN3, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[260], (u8*)&Para->APN3UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[292], (u8*)&Para->APN3PassWord, APN_USER_PWD_MAX_SIZE);

	MemCopy(&Data[324], (u8*)&Para->APN4, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[356], (u8*)&Para->APN4UserName, APN_USER_NAME_MAX_SIZE);
	MemCopy(&Data[388], (u8*)&Para->APN4PassWord, APN_USER_PWD_MAX_SIZE);

	return NO_ERROR;	
}



ResultEnum QCSG_AFN0A_E0000120_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->TerminalLocationCode, 3);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000121_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->TerminalAddr, 3);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000122_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->CommunicationMode;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000123_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->TcpPortNumber, 2);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000124_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->IpAddr, 4);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000125_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->IpSubNetMask, 4);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000126_Read(u8* Data, CommunicationParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->IpGateWayAddr, 4);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000127_Read(u8* Data, CommunicationParaStruct* Para)
{
	Data[0] = Para->IpAddrAccessType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000012F_Read(u8* Data, CommunicationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000120_Read(Data, Para);
	Data += 3;
	Result += QCSG_AFN0A_E0000121_Read(Data, Para);
	Data += 3;
	Result += QCSG_AFN0A_E0000122_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000123_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0000124_Read(Data, Para);
	Data += 4;
	Result += QCSG_AFN0A_E0000125_Read(Data, Para);
	Data += 4;
	Result += QCSG_AFN0A_E0000126_Read(Data, Para);
	Data += 4;
	Result += QCSG_AFN0A_E0000127_Read(Data, Para);
	Data += 1;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;
}


ResultEnum QCSG_AFN0A_E0000130_E0000174(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	u32 ReadLen = 0;
	u8 DataBuf[100];//err20180710 每日上报一次的告警标识，需掉电存储
	SettleDateParaStruct* pSettleDate = (SettleDateParaStruct*)DataBuf;
	AlarmEventMaskParaStruct* pAlarmEventMask = (AlarmEventMaskParaStruct*)DataBuf;	
	IDAuthenticationParaStruct* pIDAuthentication = (IDAuthenticationParaStruct*)DataBuf;
	MemSet(DataBuf, 0, 100);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000130:
			QCSGFp->DstDataPos += 6;
			Result = NAND_ReadPara(PARA_RealTime, QCSGFp->Pn, QCSGFp->CurDataID, pData, &ReadLen);

			#if (TEST_ON == 1)//err20180623 台体测试抵消GPRS延时，时间加法。
			if(QCSGFp->PORTn == GPRSPORT)
			{
				RealTimeStruct TestTime;//台体对时测试时抵消GPRS延时
				
				TestTime.u8Second	= DstFrame->Data[QCSGFp->DstDataPos - 6];
				TestTime.u8Minute	= DstFrame->Data[QCSGFp->DstDataPos - 5];
				TestTime.u8Hour	= DstFrame->Data[QCSGFp->DstDataPos - 4];
				
				TestTime.u8Day		= DstFrame->Data[QCSGFp->DstDataPos - 3];
				TestTime.u8Month	= DstFrame->Data[QCSGFp->DstDataPos - 2];
				TestTime.u8Year	= DstFrame->Data[QCSGFp->DstDataPos - 1];
				TestTime.u8Week	= 0;
			
				QCSG_IncOneSecond(&TestTime);
				QCSG_IncOneSecond(&TestTime);
				QCSG_IncOneSecond(&TestTime);
				QCSG_IncOneSecond(&TestTime);

			
				DstFrame->Data[QCSGFp->DstDataPos - 6] = TestTime.u8Second;
				DstFrame->Data[QCSGFp->DstDataPos - 5] = TestTime.u8Minute;
				DstFrame->Data[QCSGFp->DstDataPos - 4] = TestTime.u8Hour;
				DstFrame->Data[QCSGFp->DstDataPos - 3] = TestTime.u8Day;
				DstFrame->Data[QCSGFp->DstDataPos - 2] = TestTime.u8Month;
				DstFrame->Data[QCSGFp->DstDataPos - 1] = TestTime.u8Year;
			}
			#endif
			break;
		case 0xE0000131:			
		case 0xE0000140:
			Result = ERROR_Permission_denied;
			break;
		case 0xE0000150:
			QCSGFp->DstDataPos += 32;
			MR((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
			MemCopy(pData, pAlarmEventMask->AlarmReportMask, 32);
			break;
		case 0xE0000151:
			QCSGFp->DstDataPos += 32;
			MR((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
			MemCopy(pData, pAlarmEventMask->AlarmJudgeMask, 32);			
			break;
		case 0xE0000152:
			QCSGFp->DstDataPos += 32;
			MR((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
			MemCopy(pData, pAlarmEventMask->EventRecordMask, 32);
			break;
		case 0xE0000160:
			QCSGFp->DstDataPos += 2;
			MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			pData[0] = pSettleDate->MonthPowerSettleDay;
			pData[1] = pSettleDate->MonthPowerSettleHour;
			break;
		case 0xE0000161:
			QCSGFp->DstDataPos += 1;
			MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			pData[0] = pSettleDate->DayPowerSettleHour;
			break;
		case 0xE0000162:
			QCSGFp->DstDataPos += 2;
			MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			pData[0] = pSettleDate->MonthVoltageSettleDay;
			pData[1] = pSettleDate->MonthVoltageSettleHour;
			break;
		case 0xE0000163:
			QCSGFp->DstDataPos += 1;
			MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			pData[0] = pSettleDate->DayVoltageSettleHour;
			break;
		case 0xE0000170:
			QCSGFp->DstDataPos += 1;
			MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			pData[0] = pIDAuthentication->VerifyType;
			break;
		case 0xE0000171:
			QCSGFp->DstDataPos += 16;
			MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			MemCopy(pData, pIDAuthentication->AuthReqInfo, 16);
			break;
		case 0xE0000172:
			QCSGFp->DstDataPos += 16;
			MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			MemCopy(pData, pIDAuthentication->AuthRespInfo, 16);
			break;
		case 0xE0000173:
			QCSGFp->DstDataPos += 16;
			MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			MemCopy(pData, pIDAuthentication->RandomReqInfo, 16);
			break;
		case 0xE0000174:
			QCSGFp->DstDataPos += 16;
			MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			MemCopy(pData, pIDAuthentication->RandomRespInfo, 16);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}


ResultEnum QCSG_AFN0A_E0000180_E000018F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	ThresholdParaStruct ThresholdPara;
	MemSet((u8*)&ThresholdPara, 0, sizeof(ThresholdParaStruct));
	MR((u32)&ThresholdPara, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000180:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000180_Read(pData, &ThresholdPara);
			break;
		case 0xE0000181:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000181_Read(pData, &ThresholdPara);
			break;
		case 0xE0000182:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000182_Read(pData, &ThresholdPara);
			break;
		case 0xE0000183:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000183_Read(pData, &ThresholdPara);
			break;
		case 0xE0000184:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000184_Read(pData, &ThresholdPara);
			break;
		case 0xE0000185:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000185_Read(pData, &ThresholdPara);
			break;
		case 0xE0000186:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000186_Read(pData, &ThresholdPara);
			break;
		case 0xE0000187:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000187_Read(pData, &ThresholdPara);
			break;
		case 0xE0000188:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000188_Read(pData, &ThresholdPara);
			break;
		case 0xE0000189:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000189_Read(pData, &ThresholdPara);
			break;
		case 0xE000018A:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E000018A_Read(pData, &ThresholdPara);
			break;
		case 0xE000018B:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E000018B_Read(pData, &ThresholdPara);
			break;
		case 0xE000018C:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E000018C_Read(pData, &ThresholdPara);
			break;
		case 0xE000018F:
			QCSGFp->DstDataPos += 18;
			Result = QCSG_AFN0A_E000018F_Read(pData, &ThresholdPara);
			break;		
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	return Result;
}


ResultEnum QCSG_AFN0A_E0000180_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000180;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000181_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000181;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000182_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000182;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000183_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000183;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000184_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000184;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000185_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000185;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000186_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000186;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000187_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000187;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000188_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000188;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000189_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E0000189;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000018A_Read(u8* Data, ThresholdParaStruct* Para)
{
	Data[0] = Para->E000018A;
	Data[1] = Para->E000018A >> 8;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000018B_Read(u8* Data, ThresholdParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E000018B, 3);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000018C_Read(u8* Data, ThresholdParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E000018C, 3);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E000018F_Read(u8* Data, ThresholdParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;

	Result += QCSG_AFN0A_E0000180_Read(Data, Para);
	Data += 1;	
	Result += QCSG_AFN0A_E0000181_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000182_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000183_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000184_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000185_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000186_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000187_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000188_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000189_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E000018A_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E000018B_Read(Data, Para);
	Data += 3;
	Result += QCSG_AFN0A_E000018C_Read(Data, Para);
	Data += 3;
	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}	
	return Result;
}



ResultEnum QCSG_AFN0A_E0000200_E0000220(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	u32 FlashAddr = 0;
	AMRChTimeStruct AMRChTime;
	MemSet((u8*)&AMRChTime, 0, sizeof(AMRChTimeStruct));
	
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000200:
			QCSGFp->DstDataPos += 7;
			MR((u32)&AMRChTime, ADDR_AMRChTimeTbl, sizeof(AMRChTimeStruct));
			MemCopy(pData, (u8*)&AMRChTime, 7);
			break;
		case 0xE0000201:
			QCSGFp->DstDataPos += 7;
			FlashAddr = ADDR_AMRChTimeTbl + sizeof(AMRChTimeStruct);
			MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
			MemCopy(pData, (u8*)&AMRChTime, 7);
			break;
		case 0xE0000202:
			QCSGFp->DstDataPos += 7;
			FlashAddr = ADDR_AMRChTimeTbl + (2 * sizeof(AMRChTimeStruct));
			MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
			MemCopy(pData, (u8*)&AMRChTime, 7);
			break;
		case 0xE000021F:
			QCSGFp->DstDataPos += 7;
			FlashAddr = ADDR_AMRChTimeTbl + (3 * sizeof(AMRChTimeStruct));
			MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
			MemCopy(pData, (u8*)&AMRChTime, 7);
			break;
		case 0xE0000220:
			QCSGFp->DstDataPos += 7;
			FlashAddr = ADDR_AMRChTimeTbl + (4 * sizeof(AMRChTimeStruct));
			MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
			MemCopy(pData, (u8*)&AMRChTime, 7);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}



ResultEnum QCSG_AFN0A_E0000221_E0000250(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	AMRChSetStruct AMRChSet;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	u32 FlashAddr = 0, Index = 0;
	MemSet((u8*)&AMRChSet, 0, sizeof(AMRChSetStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000221:
			QCSGFp->DstDataPos += 1;
			pData[0] = 0;
			for(Index = 0; Index < AMR_CH_NUM; Index++)
			{
				FlashAddr = ADDR_AMRChSetTbl + Index*sizeof(AMRChSetStruct);
				MR((u32)&AMRChSet, FlashAddr, sizeof(AMRChSetStruct));
				if(AMRChSet.Flag == 0x01) pData[0]++;
			}
			FlashAddr = 0;
			break;
		case 0xE0000230:
			FlashAddr = ADDR_AMRChSetTbl;			
			break;
		case 0xE0000231:
			FlashAddr = ADDR_AMRChSetTbl + sizeof(AMRChSetStruct);
			break;
		case 0xE0000232:
			FlashAddr = ADDR_AMRChSetTbl + (2 * sizeof(AMRChSetStruct));
			break;
		case 0xE000024F:
			FlashAddr = ADDR_AMRChSetTbl + (3 * sizeof(AMRChSetStruct));
			break;
		case 0xE0000250:
			FlashAddr = ADDR_AMRChSetTbl + (4 * sizeof(AMRChSetStruct));
			break;		
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	if(FlashAddr != 0)
	{
		MR((u32)&AMRChSet, FlashAddr, sizeof(AMRChSetStruct));
		
		pData[Index++] = AMRChSet.Flag;
		pData[Index++] = AMRChSet.StoreNum;
		pData[Index++] = AMRChSet.StoreNum >> 8;
		pData[Index++] = AMRChSet.ChMeterType;
		pData[Index++] = AMRChSet.UserType;
		pData[Index++] = AMRChSet.DANum;
		if(AMRChSet.DANum == 0xFF)
		{
			pData[Index++] = AMRChSet.DA[0];
			pData[Index++] = AMRChSet.DA[0] >> 8;
		}
		else
		{
			MemCopy(&pData[Index], (u8*)AMRChSet.DA, AMRChSet.DANum * 2);
			Index += AMRChSet.DANum * 2;
		}
		
		pData[Index++] = AMRChSet.DataIDNum;
		if(AMRChSet.DataIDNum == 0xFF)
		{
			pData[Index++] = AMRChSet.DataID[0];
			pData[Index++] = AMRChSet.DataID[0] >> 8;
			pData[Index++] = AMRChSet.DataID[0] >> 16;
			pData[Index++] = AMRChSet.DataID[0] >> 24;
		}
		else
		{
			if(AMRChSet.DataIDNum > AMR_MAX_DATA_ID) 
			{
				AMRChSet.DataIDNum = AMR_MAX_DATA_ID;
			}
			MemCopy(&pData[Index], (u8*)AMRChSet.DataID, AMRChSet.DataIDNum * 4);
			Index += AMRChSet.DataIDNum * 4;
		}
		QCSGFp->DstDataPos += Index;
	}	
	return Result;
}



ResultEnum QCSG_AFN0A_E0000300_E00003FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	NormalTaskParaStruct NormalTaskPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	u32 FlashAddr = 0, Index = 0;
	MemSet((u8*)&NormalTaskPara, 0, sizeof(NormalTaskParaStruct));

	if(QCSGFp->CurDataID == 0xE0000300)
	{
		QCSGFp->DstDataPos += 1;
		pData[0] = 0;
		for(Index = 0; Index < NormalTaskPara_Num; Index++)
		{
			MR((u32)&NormalTaskPara, ADDR_NormalTaskPara + Index*sizeof(NormalTaskParaStruct), sizeof(NormalTaskParaStruct));
			if(NormalTaskPara.Flag == 0x01) pData[0]++;
		}
	}
	else if((QCSGFp->CurDataID >= 0xE0000301) && (QCSGFp->CurDataID <= 0xE0000320))
	{
		FlashAddr = ADDR_NormalTaskPara + (((QCSGFp->CurDataID - 1) & 0x000000FF) * sizeof(NormalTaskParaStruct));
	}
	else
	{
		Result = ERROR_InvalidData;
	}
	
	if(FlashAddr != 0)
	{
		MR((u32)&NormalTaskPara, FlashAddr, sizeof(NormalTaskParaStruct));
		if(NormalTaskPara.Flag == 0x01)
		{
			pData[Index++] = NormalTaskPara.Flag;
			pData[Index++] = NormalTaskPara.ReportTime.u8Minute;
			pData[Index++] = NormalTaskPara.ReportTime.u8Hour;
			pData[Index++] = NormalTaskPara.ReportTime.u8Day;
			pData[Index++] = NormalTaskPara.ReportTime.u8Month;
			pData[Index++] = NormalTaskPara.ReportTime.u8Year;
			pData[Index++] = NormalTaskPara.ReportPeriodUnit;
			pData[Index++] = NormalTaskPara.ReportPeriod;
			pData[Index++] = NormalTaskPara.DataStructureType;

			pData[Index++] = NormalTaskPara.SampleTime.u8Minute;
			pData[Index++] = NormalTaskPara.SampleTime.u8Hour;
			pData[Index++] = NormalTaskPara.SampleTime.u8Day;
			pData[Index++] = NormalTaskPara.SampleTime.u8Month;
			pData[Index++] = NormalTaskPara.SampleTime.u8Year;
			pData[Index++] = NormalTaskPara.SamplePeriodUnit;
			pData[Index++] = NormalTaskPara.SamplePeriod;
			pData[Index++] = NormalTaskPara.GetRate;

			pData[Index++] = NormalTaskPara.ExecCnt;
			pData[Index++] = NormalTaskPara.ExecCnt >> 8;
			pData[Index++] = NormalTaskPara.DANum;
			if(NormalTaskPara.DANum != 0)
			{
				MemCopy(&pData[Index], (u8*)NormalTaskPara.DA, NormalTaskPara.DANum * 2);
				Index += NormalTaskPara.DANum * 2;
			}	

			if(NormalTaskPara.DataIDNum > AMR_MAX_DATA_ID) 
			{
				NormalTaskPara.DataIDNum = AMR_MAX_DATA_ID;
			}
			pData[Index++] = NormalTaskPara.DataIDNum;
			if(NormalTaskPara.DataIDNum != 0)
			{
				MemCopy(&pData[Index], (u8*)NormalTaskPara.DataID, NormalTaskPara.DataIDNum * 4);
				Index += NormalTaskPara.DataIDNum * 4;
			}
			
			QCSGFp->DstDataPos += Index;
		}	
	}	
	return Result;
}




ResultEnum QCSG_AFN0A_E0000400_E00004FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	RelayTaskParaStruct RelayTaskPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	u32 FlashAddr = 0, Index = 0;
	MemSet((u8*)&RelayTaskPara, 0, sizeof(RelayTaskParaStruct));

	if(QCSGFp->CurDataID == 0xE0000400)
	{
		QCSGFp->DstDataPos += 1;
		pData[0] = 0;
		for(Index = 0; Index < RelayTaskPara_Num; Index++)
		{
			MR((u32)&RelayTaskPara, ADDR_RelayTaskPara + Index*sizeof(RelayTaskParaStruct), sizeof(RelayTaskParaStruct));
			if(RelayTaskPara.Flag == 0x01) pData[0]++;
		}
	}
	else if((QCSGFp->CurDataID >= 0xE0000401) && (QCSGFp->CurDataID <= 0xE0000420))
	{
		FlashAddr = ADDR_RelayTaskPara + (((QCSGFp->CurDataID - 1) & 0x000000FF) * sizeof(RelayTaskParaStruct));
	}
	else
	{
		Result = ERROR_InvalidData;
	}
	
	if(FlashAddr != 0)
	{
		MR((u32)&RelayTaskPara, FlashAddr, sizeof(RelayTaskParaStruct));
		if(RelayTaskPara.Flag == 0x01)
		{
			pData[Index++] = RelayTaskPara.Flag;
			pData[Index++] = RelayTaskPara.ReportTime.u8Minute;
			pData[Index++] = RelayTaskPara.ReportTime.u8Hour;
			pData[Index++] = RelayTaskPara.ReportTime.u8Day;
			pData[Index++] = RelayTaskPara.ReportTime.u8Month;
			pData[Index++] = RelayTaskPara.ReportTime.u8Year;
			pData[Index++] = RelayTaskPara.ReportPeriodUnit;
			pData[Index++] = RelayTaskPara.ReportPeriod;

			pData[Index++] = RelayTaskPara.SampleTime.u8Minute;
			pData[Index++] = RelayTaskPara.SampleTime.u8Hour;
			pData[Index++] = RelayTaskPara.SampleTime.u8Day;
			pData[Index++] = RelayTaskPara.SampleTime.u8Month;
			pData[Index++] = RelayTaskPara.SampleTime.u8Year;
			pData[Index++] = RelayTaskPara.SamplePeriodUnit;
			pData[Index++] = RelayTaskPara.SamplePeriod;
			
			pData[Index++] = RelayTaskPara.GetRate;
			pData[Index++] = RelayTaskPara.RelayType;
			
			pData[Index++] = RelayTaskPara.ExecCnt;
			pData[Index++] = RelayTaskPara.ExecCnt >> 8;
			pData[Index++] = RelayTaskPara.ComPortId;
			pData[Index++] = RelayTaskPara.Baud;
			pData[Index++] = RelayTaskPara.Parity;
			pData[Index++] = RelayTaskPara.DataBit;
			pData[Index++] = RelayTaskPara.StopBit;
			pData[Index++] = RelayTaskPara.TimeOut;
			pData[Index++] = RelayTaskPara.Length;
			if(RelayTaskPara.Length != 0)
			{
				MemCopy(&pData[Index], RelayTaskPara.Frame, RelayTaskPara.Length);
				Index += RelayTaskPara.Length;
			}	
			QCSGFp->DstDataPos += Index;
		}	
	}	
	return Result;
}




ResultEnum QCSG_AFN0A_E0000500_E00005FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	AnalogCaculationParaStruct AnalogPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	MemSet((u8*)&AnalogPara, 0, sizeof(AnalogCaculationParaStruct));
	MR((u32)&AnalogPara, ADDR_AnalogPara, sizeof(AnalogCaculationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000500:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000500_Read(pData, &AnalogPara);
			break;
		case 0xE0000501:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E0000501_Read(pData, &AnalogPara);
			break;
		case 0xE0000502:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E0000502_Read(pData, &AnalogPara);
			break;
		case 0xE00005FE:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E00005FE_Read(pData, &AnalogPara);
			break;
		case 0xE00005FF:
			QCSGFp->DstDataPos += 39;
			Result = QCSG_AFN0A_E00005FF_Read(pData, &AnalogPara);
			break;	
		case 0xE0000900:
			QCSGFp->DstDataPos += 3;
			if(AnalogPara.E0000900 == 0xFFFFFFFF)
			{
				AnalogPara.E0000900 = 0xFF1441FF;
			}
			MemCopy(pData, (u8*)&AnalogPara.E0000900, 3);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	return Result;
}

ResultEnum QCSG_AFN0A_E0000500_Read(u8* Data, AnalogCaculationParaStruct* Para)
{	
	Data[0] = Para->AnalogNum;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000501_Read(u8* Data, AnalogCaculationParaStruct* Para)
{
	Data[0] = Para->AnalogID[0][0];
	Data[1] = Para->AnalogID[0][1];
	Data[2] = Para->AnalogID[0][2];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000502_Read(u8* Data, AnalogCaculationParaStruct* Para)
{
	Data[0] = Para->AnalogID[1][0];
	Data[1] = Para->AnalogID[1][1];
	Data[2] = Para->AnalogID[1][2];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E00005FE_Read(u8* Data, AnalogCaculationParaStruct* Para)
{
	MemCopy(Data, Para->AnalogStateMark, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E00005FF_Read(u8* Data, AnalogCaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000500_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000501_Read(Data, Para);
	Data += 3;
	Result += QCSG_AFN0A_E0000502_Read(Data, Para);
	Data += 3;
	Result += QCSG_AFN0A_E00005FE_Read(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN0A_E0000600_E00006FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	PulseCaculationParaStruct PulsePara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	MemSet((u8*)&PulsePara, 0, sizeof(PulseCaculationParaStruct));
	MR((u32)&PulsePara, ADDR_PulsePara, sizeof(PulseCaculationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000600:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000600_Read(pData, &PulsePara);
			break;
		case 0xE0000601:
			QCSGFp->DstDataPos += 6;
			Result = QCSG_AFN0A_E0000601_Read(pData, &PulsePara);
			break;
		case 0xE0000602:
			QCSGFp->DstDataPos += 6;
			Result = QCSG_AFN0A_E0000602_Read(pData, &PulsePara);
			break;
		case 0xE00006FE:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E00006FE_Read(pData, &PulsePara);
			break;
		case 0xE00006FF:
			QCSGFp->DstDataPos += 45;
			Result = QCSG_AFN0A_E00006FF_Read(pData, &PulsePara);
			break;	
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	return Result;
}

ResultEnum QCSG_AFN0A_E0000600_Read(u8* Data, PulseCaculationParaStruct* Para)
{	
	Data[0] = Para->PulseNum;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000601_Read(u8* Data, PulseCaculationParaStruct* Para)
{
	MemCopy(Data, Para->PulseID[0], 6);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000602_Read(u8* Data, PulseCaculationParaStruct* Para)
{
	MemCopy(Data, Para->PulseID[1], 6);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E00006FE_Read(u8* Data, PulseCaculationParaStruct* Para)
{
	MemCopy(Data, Para->PulseStateMark, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E00006FF_Read(u8* Data, PulseCaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000600_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000601_Read(Data, Para);
	Data += 6;
	Result += QCSG_AFN0A_E0000602_Read(Data, Para);
	Data += 6;
	Result += QCSG_AFN0A_E00006FE_Read(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN0A_E0000700_E00007FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	CaculationParaStruct CaclPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	MemSet((u8*)&CaclPara, 0, sizeof(CaculationParaStruct));
	MR((u32)&CaclPara, ADDR_CaculationPara, sizeof(CaculationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000700:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000700_Read(pData, &CaclPara);
			break;
		case 0xE0000701:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000701_Read(pData, &CaclPara);
			break;
		case 0xE0000702:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000702_Read(pData, &CaclPara);
			break;
		case 0xE0000703:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000703_Read(pData, &CaclPara);
			break;
		case 0xE0000704:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000704_Read(pData, &CaclPara);
			break;
		case 0xE0000705:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000705_Read(pData, &CaclPara);
			break;
		case 0xE0000706:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000706_Read(pData, &CaclPara);
			break;
		case 0xE0000707:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000707_Read(pData, &CaclPara);
			break;
		case 0xE0000708:
			QCSGFp->DstDataPos += 30;
			Result = QCSG_AFN0A_E0000708_Read(pData, &CaclPara);
			break;
		case 0xE00007FE:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E00007FE_Read(pData, &CaclPara);
			break;
		case 0xE00007FF:
			QCSGFp->DstDataPos += 273;
			Result = QCSG_AFN0A_E00007FF_Read(pData, &CaclPara);
			break;	
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}

	return Result;
}

ResultEnum QCSG_AFN0A_E0000700_Read(u8* Data, CaculationParaStruct* Para)
{	
	Data[0] = Para->CaculationNum;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000701_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[0], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000702_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[1], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000703_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[2], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000704_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[3], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000705_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[4], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000706_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[5], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000707_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[6], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000708_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationID[7], 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E00007FE_Read(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Data, Para->CaculationStateMark, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E00007FF_Read(u8* Data, CaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000700_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000701_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000702_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000703_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000704_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000705_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000706_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000707_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E0000708_Read(Data, Para);
	Data += 30;
	Result += QCSG_AFN0A_E00007FE_Read(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN0A_E0000800_E00008FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	DiffCaculationParaStruct DiffPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];	
	MemSet((u8*)&DiffPara, 0, sizeof(DiffCaculationParaStruct));
	MR((u32)&DiffPara, ADDR_DiffPara, sizeof(DiffCaculationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000800:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000800_Read(pData, &DiffPara);
			break;
		case 0xE0000801:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000801_Read(pData, &DiffPara);
			break;
		case 0xE0000802:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000802_Read(pData, &DiffPara);
			break;
		case 0xE0000803:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000803_Read(pData, &DiffPara);
			break;
		case 0xE0000804:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000804_Read(pData, &DiffPara);
			break;
		case 0xE0000805:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000805_Read(pData, &DiffPara);
			break;
		case 0xE0000806:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000806_Read(pData, &DiffPara);
			break;
		case 0xE0000807:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000807_Read(pData, &DiffPara);
			break;
		case 0xE0000808:
			QCSGFp->DstDataPos += 15;
			Result = QCSG_AFN0A_E0000808_Read(pData, &DiffPara);
			break;
		case 0xE00008FE:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E00008FE_Read(pData, &DiffPara);
			break;
		case 0xE00008FF:
			QCSGFp->DstDataPos += 153;
			Result = QCSG_AFN0A_E00008FF_Read(pData, &DiffPara);
			break;	
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}

ResultEnum QCSG_AFN0A_E0000800_Read(u8* Data, DiffCaculationParaStruct* Para)
{	
	Data[0] = Para->DiffNum;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000801_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[0], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000802_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[1], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000803_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[2], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000804_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[3], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000805_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[4], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000806_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[5], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000807_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[6], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000808_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffID[7], 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E00008FE_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Data, Para->DiffStateMark, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E00008FF_Read(u8* Data, DiffCaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000800_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000801_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000802_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000803_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000804_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000805_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000806_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000807_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E0000808_Read(Data, Para);
	Data += 15;
	Result += QCSG_AFN0A_E00008FE_Read(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}



ResultEnum QCSG_AFN0A_E0000A00_E0000B02(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	u32 ReadLen = 0;
	u8 DataBuf[48];
	RS485PortParaStruct* pRS485PortPara = (RS485PortParaStruct*)DataBuf;
	AttachmentParaStruct* pAttachmentPara = (AttachmentParaStruct*)DataBuf;	
	MemSet(DataBuf, 0, 48);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000A00:
		case 0xE0000A01:
		case 0xE0000A02:
			QCSGFp->DstDataPos += 5;
			Result = NAND_ReadPara(PARA_RS485Port, QCSGFp->Pn, QCSGFp->CurDataID, (u8*)pRS485PortPara, &ReadLen);
			MemCopy(pData, (u8*)pRS485PortPara, 5);
			break;
		case 0xE0000A20:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			MemCopy(pData, pAttachmentPara->SlaveAddr1, 24);
			QCSGFp->DstDataPos += 24;
			break;
		case 0xE0000A21:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			MemCopy(pData, pAttachmentPara->MasterAddr, 4);
			QCSGFp->DstDataPos += 4;
			break;
		case 0xE0000A22:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			MemCopy(pData, pAttachmentPara->AMRSlaveAddr, 4);
			QCSGFp->DstDataPos += 4;
			break;
		case 0xE0000A23:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			pData[0] = pAttachmentPara->MasterTimeOut;
			QCSGFp->DstDataPos += 1;
			break;
		case 0xE0000A24:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			pData[0] = pAttachmentPara->SlaveTimeOut;
			QCSGFp->DstDataPos += 1;
			break;
		case 0xE0000A25:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			MemCopy(pData, (u8*)&pAttachmentPara->CT, 2);
			QCSGFp->DstDataPos += 2;
			break;
		case 0xE0000A26:
			MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			MemCopy(pData, (u8*)&pAttachmentPara->PT, 2);
			QCSGFp->DstDataPos += 2;
			break;	
		case 0xE0000A30:
			pData[0] = 0xFF;			// 无上报需求
			QCSGFp->DstDataPos += 1;
			break;
		case 0xE0000A31:
			Result = ERROR_DO_NOT_ANSWER;
			break;
		case 0xE0000A32:
			pData[0] = 0xFF;			// 无上报需求
			QCSGFp->DstDataPos += 1;
			break;
		case 0xE0000A33:
			QCSGFp->DstDataPos += 8;
			pData[0] = 0x00;pData[1] = 0x01;
			pData[2] = 0x06;pData[3] = 0x05;
			pData[4] = 0x99;pData[5] = 0x99;
			pData[6] = 0x99;pData[7] = 0x99;
			break;
		case 0xE0000B00:
			QCSGFp->DstDataPos += 2;
			pData[0] = DefaultVersionPara.ProtocolVersion;
			pData[1] = DefaultVersionPara.ProtocolVersion >> 8;			
			break;
		case 0xE0000B01:
			QCSGFp->DstDataPos += 6;
			pData[0] = DefaultVersionPara.ManufactureID;
			pData[1] = DefaultVersionPara.ManufactureID >> 8;	
			pData[2] = DefaultVersionPara.TerminalType;
			pData[3] = DefaultVersionPara.Day;	
			pData[4] = DefaultVersionPara.Month;
			pData[5] = DefaultVersionPara.Year;	
			break;
		case 0xE0000B02:
			QCSGFp->DstDataPos += 2;
			pData[0] = DefaultVersionPara.HardWareVersion;
			pData[1] = DefaultVersionPara.HardWareVersion >> 8;
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}



ResultEnum QCSG_AFN0A_E0000C00_E0000E11(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	u32 YXState = 0, YKState = 0;
	LoadManagementParaStruct LoadPara;
	MemSet((u8*)&LoadPara, 0, sizeof(LoadManagementParaStruct));
	MR((u32)&LoadPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000C00:
		case 0xE0000C01:
		case 0xE0000C02:
		case 0xE0000C03:
		case 0xE0000C04:
		case 0xE0000C05:
		case 0xE0000C06:
		case 0xE0000C07:
		case 0xE0000C08:
		case 0xE0000C09:
		case 0xE0000C0A:
		case 0xE0000C0B:
		case 0xE0000C0C:
		case 0xE0000C0D:
		case 0xE0000C0E:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.SwitchNodeType[QCSGFp->CurDataID & 0x0000000F];
			break;
		case 0xE0000C0F:
			QCSGFp->DstDataPos += 15;
			MemCopy(pData, LoadPara.SwitchNodeType, 15);
			break;
		case 0xE0000C10:
			QCSGFp->DstDataPos += 1;
			QCSG_ReadTerminal_State((u8*)&YXState, (u8*)&YKState);
			//err20180506 改变输入开关状态
			if(LoadPara.SwitchNodeType[QCSGFp->CurDataID & 0x0000000F] == 0x00)	//00常开触点
			{
				if((YXState & 0x01) == 0)		//0短接
				{
					pData[0] = 0x01;	//01分状态
				}
				else 
					pData[0] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & 0x01) == 0)		//0短接
				{
					pData[0] = 0x00;	//00合状态
				}
				else 
					pData[0] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态
			break;
		case 0xE0000C11:
		case 0xE0000C12:
		case 0xE0000C13:
		case 0xE0000C14:
			QCSGFp->DstDataPos += 1;
			QCSG_ReadTerminal_State((u8*)&YXState, (u8*)&YKState);
			//err20180506 改变输入开关状态
			if(LoadPara.SwitchNodeType[QCSGFp->CurDataID & 0x0000000F] == 0x00)	//00常开触点
			{
				if((YXState & (0x04 << (QCSGFp->CurDataID & 0x0000000F))) != 0)		//0短接
				{
					pData[0] = 0x01;	//01分状态
				}
				else 
					pData[0] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & (0x04 << (QCSGFp->CurDataID & 0x0000000F))) != 0)		//0短接
				{
					pData[0] = 0x00;	//00合状态
				}
				else 
					pData[0] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态
			break;
		case 0xE0000C15:
		case 0xE0000C16:
		case 0xE0000C17:
		case 0xE0000C18:
		case 0xE0000C19:
		case 0xE0000C1A:
		case 0xE0000C1B:
		case 0xE0000C1C:
		case 0xE0000C1D:
		case 0xE0000C1E:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.InputSwitchState[QCSGFp->CurDataID & 0x0000000F];
			break;
		case 0xE0000C1F:
			QCSGFp->DstDataPos += 15;
			MemCopy(pData, LoadPara.InputSwitchState, 15);
			QCSG_ReadTerminal_State((u8*)&YXState, (u8*)&YKState);
			if(LoadPara.SwitchNodeType[0] == 0x00)	//00常开触点
			{
				if((YXState & 0x01) == 0)		//0短接
				{
					pData[0] = 0x01;	//01分状态
				}
				else 
					pData[0] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & 0x01) == 0)		//0短接
				{
					pData[0] = 0x00;	//00合状态
				}
				else 
					pData[0] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态

			if(LoadPara.SwitchNodeType[1] == 0x00)	//00常开触点
			{
				if((YXState & 0x08) != 0)		//0短接
				{
					pData[1] = 0x01;	//01分状态
				}
				else 
					pData[1] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & 0x08) != 0)		//0短接
				{
					pData[1] = 0x00;	//00合状态
				}
				else 
					pData[1] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态

			if(LoadPara.SwitchNodeType[2] == 0x00)	//00常开触点
			{
				if((YXState & 0x10) != 0)		//0短接
				{
					pData[2] = 0x01;	//01分状态
				}
				else 
					pData[2] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & 0x10) != 0)		//0短接
				{
					pData[2] = 0x00;	//00合状态
				}
				else 
					pData[2] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态

			if(LoadPara.SwitchNodeType[3] == 0x00)	//00常开触点
			{
				if((YXState & 0x20) != 0)		//0短接
				{
					pData[3] = 0x01;	//01分状态
				}
				else 
					pData[3] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & 0x20) != 0)		//0短接
				{
					pData[3] = 0x00;	//00合状态
				}
				else 
					pData[3] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态

			if(LoadPara.SwitchNodeType[4] == 0x00)	//00常开触点
			{
				if((YXState & 0x40) != 0)		//0短接
				{
					pData[4] = 0x01;	//01分状态
				}
				else 
					pData[4] = 0x00;	//00合状态
			}
			else 												//01常闭触点
			{
				if((YXState & 0x40) != 0)		//0短接
				{
					pData[4] = 0x00;	//00合状态
				}
				else 
					pData[4] = 0x01;	//01分状态
			}
			//err20180506 改变输入开关状态
			
			break;
		case 0xE0000C20:
		case 0xE0000C21:
		case 0xE0000C22:
		case 0xE0000C23:
		case 0xE0000C24:
		case 0xE0000C25:
		case 0xE0000C26:
		case 0xE0000C27:
		case 0xE0000C28:
		case 0xE0000C29:
		case 0xE0000C2A:
		case 0xE0000C2B:
		case 0xE0000C2C:
		case 0xE0000C2D:
		case 0xE0000C2E:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.OutputSwitchState[QCSGFp->CurDataID & 0x0000000F];
			break;
		case 0xE0000C2F:
			QCSGFp->DstDataPos += 15;
			MemCopy(pData, LoadPara.OutputSwitchState, 15);
			break;
		case 0xE0000C30:
		case 0xE0000C31:
		case 0xE0000C32:
		case 0xE0000C33:
		case 0xE0000C34:
		case 0xE0000C35:
		case 0xE0000C36:
		case 0xE0000C37:
		case 0xE0000C38:
		case 0xE0000C39:
		case 0xE0000C3A:
		case 0xE0000C3B:
		case 0xE0000C3C:
		case 0xE0000C3D:
		case 0xE0000C3E:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.OutputSwitchRound[QCSGFp->CurDataID & 0x0000000F];
			break;
		case 0xE0000C3F:
			QCSGFp->DstDataPos += 15;
			MemCopy(pData, LoadPara.OutputSwitchRound, 15);
			break;
		case 0xE0000C40:
			QCSGFp->DstDataPos += 2;
			MemCopy(pData, (u8*)&LoadPara.RemoteSwitchOffTime, 2);			
			break;
		case 0xE0000C50:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.PowerHoldState;
			break;
		case 0xE0000C60:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.PowerCtrlFlag;
			break;
		case 0xE0000C61:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.MonthKwhCtrlFlag;
			break;
		case 0xE0000C62:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.PurchaseKwhCtrlFlag;
			break;
		case 0xE0000C63:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.PowerCtrlRoundFlag;
			break;
		case 0xE0000C64:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.MonthKwhCtrlRoundFlag;
			break;
		case 0xE0000C65:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.PurchaseKwhCtrlRoundFlag;
			break;
		case 0xE0000C66:
			QCSGFp->DstDataPos += 3;
			MemCopy(pData, (u8*)&LoadPara.PowerOverLoadTime, 3);
			break;
		case 0xE0000C67:
			QCSGFp->DstDataPos += 3;
			MemCopy(pData, (u8*)&LoadPara.PowerResetTime, 3);
			break;
		case 0xE0000C68:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.AlarmDelayTime;
			break;
		case 0xE0000C69:
			QCSGFp->DstDataPos += 17;
			pData[0] = LoadPara.PowerCtrlIncFlag;
			MemCopy(&pData[1], (u8*)LoadPara.PowerCtrlPoint, 16);
			break;
		case 0xE0000C6A:
			QCSGFp->DstDataPos += 17;
			pData[0] = LoadPara.KwhCtrlIncFlag;
			MemCopy(&pData[1], (u8*)LoadPara.KwhCtrlPoint, 16);
			break;
		case 0xE0000C6B:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.E0000C6B;
			break;
		case 0xE0000C6C:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.E0000C6C;
			break;
		case 0xE0000C6D:
			QCSGFp->DstDataPos += 4;
			MemCopy(pData, (u8*)&LoadPara.E0000C6D, 4);
			break;
		case 0xE0000C70:
			QCSGFp->DstDataPos += 4;
			MemCopy(pData, (u8*)&LoadPara.E0000C70, 4);
			break;
		case 0xE0000C71:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.E0000C71;
			break;
		case 0xE0000D00:
			QCSGFp->DstDataPos += 1;	
			Result = QCSG_AFN0A_E0000D00_Read(pData, &LoadPara);
			break;
		case 0xE0000D01:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D01_Read(pData, &LoadPara);
			break;
		case 0xE0000D02:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D02_Read(pData, &LoadPara);
			break;
		case 0xE0000D03:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D03_Read(pData, &LoadPara);
			break;
		case 0xE0000D04:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D04_Read(pData, &LoadPara);
			break;
		case 0xE0000D05:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D05_Read(pData, &LoadPara);
			break;
		case 0xE0000D06:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D06_Read(pData, &LoadPara);
			break;
		case 0xE0000D07:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D07_Read(pData, &LoadPara);
			break;
		case 0xE0000D08:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D08_Read(pData, &LoadPara);
			break;
		case 0xE0000D09:
			QCSGFp->DstDataPos += 9;
			Result = QCSG_AFN0A_E0000D09_Read(pData, &LoadPara);
			break;
		case 0xE0000D0F:
			QCSGFp->DstDataPos += 66;
			Result = QCSG_AFN0A_E0000D0F_Read(pData, &LoadPara);
			break;

		case 0xE0000D10:
			QCSGFp->DstDataPos += 1;	
			Result = QCSG_AFN0A_E0000D10_Read(pData, &LoadPara);
			break;
		case 0xE0000D11:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D11_Read(pData, &LoadPara);
			break;
		case 0xE0000D12:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D12_Read(pData, &LoadPara);
			break;
		case 0xE0000D13:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D13_Read(pData, &LoadPara);
			break;
		case 0xE0000D14:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D14_Read(pData, &LoadPara);
			break;
		case 0xE0000D15:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D15_Read(pData, &LoadPara);
			break;
		case 0xE0000D16:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D16_Read(pData, &LoadPara);
			break;
		case 0xE0000D17:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D17_Read(pData, &LoadPara);
			break;
		case 0xE0000D18:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D18_Read(pData, &LoadPara);
			break;
		case 0xE0000D19:
			QCSGFp->DstDataPos += 9;
			Result = QCSG_AFN0A_E0000D19_Read(pData, &LoadPara);						
			break;
		case 0xE0000D1F:
			QCSGFp->DstDataPos += 66;
			Result = QCSG_AFN0A_E0000D1F_Read(pData, &LoadPara);						
			break;

		case 0xE0000D20:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000D20_Read(pData, &LoadPara);
			break;
		case 0xE0000D21:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D21_Read(pData, &LoadPara);
			break;
		case 0xE0000D22:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D22_Read(pData, &LoadPara);
			break;
		case 0xE0000D23:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D23_Read(pData, &LoadPara);
			break;
		case 0xE0000D24:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D24_Read(pData, &LoadPara);
			break;
		case 0xE0000D25:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D25_Read(pData, &LoadPara);
			break;
		case 0xE0000D26:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D26_Read(pData, &LoadPara);
			break;
		case 0xE0000D27:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D27_Read(pData, &LoadPara);
			break;
		case 0xE0000D28:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D28_Read(pData, &LoadPara);
			break;
		case 0xE0000D29:
			QCSGFp->DstDataPos += 9;			
			Result = QCSG_AFN0A_E0000D29_Read(pData, &LoadPara);
			break;
		case 0xE0000D2F:
			QCSGFp->DstDataPos += 66;			
			Result = QCSG_AFN0A_E0000D2F_Read(pData, &LoadPara);
			break;

		case 0xE0000D30:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0000D30_Read(pData, &LoadPara);
			break;
		case 0xE0000D31:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D31_Read(pData, &LoadPara);
			break;
		case 0xE0000D32:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D32_Read(pData, &LoadPara);
			break;
		case 0xE0000D33:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D33_Read(pData, &LoadPara);
			break;
		case 0xE0000D34:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D34_Read(pData, &LoadPara);
			break;
		case 0xE0000D35:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D35_Read(pData, &LoadPara);
			break;
		case 0xE0000D36:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D36_Read(pData, &LoadPara);
			break;
		case 0xE0000D37:
			QCSGFp->DstDataPos += 7;
			Result = QCSG_AFN0A_E0000D37_Read(pData, &LoadPara);
			break;
		case 0xE0000D38:
			QCSGFp->DstDataPos += 7;			
			Result = QCSG_AFN0A_E0000D38_Read(pData, &LoadPara);
			break;
		case 0xE0000D39:
			QCSGFp->DstDataPos += 9;			
			Result = QCSG_AFN0A_E0000D39_Read(pData, &LoadPara);
			break;
		case 0xE0000D3F:
			QCSGFp->DstDataPos += 66;			
			Result = QCSG_AFN0A_E0000D3F_Read(pData, &LoadPara);
			break;
		
		case 0xE0000E00:
			QCSGFp->DstDataPos += 4;
			MemCopy(pData, (u8*)&LoadPara.E0000E00, 4);
			break;	
		case 0xE0000E01:
			QCSGFp->DstDataPos += 4;
			MemCopy(pData, (u8*)&LoadPara.E0000E01, 4);
			break;
		case 0xE0000E02:
			QCSGFp->DstDataPos += 5;
			MemCopy(pData, LoadPara.E0000E02, 5);
			break;
		case 0xE0000E03:
			QCSGFp->DstDataPos += 5;
			MemCopy(pData, LoadPara.E0000E03, 5);
			break;
		case 0xE0000E04:
			QCSGFp->DstDataPos += 49;
			MemCopy(pData, LoadPara.E0000E04, 49);
			break;
		case 0xE0000E05:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.E0000E05;	
			break;
		case 0xE0000E10:
			QCSGFp->DstDataPos += 1;
			pData[0] = LoadPara.E0000E10;
			break;
		case 0xE0000E11:
			QCSGFp->DstDataPos += 10;
			MemCopy(pData, LoadPara.E0000E11, 10);			
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	return Result;
}


ResultEnum QCSG_AFN0A_E0000D00_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].PowerCtrlPeriodCnt;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D01_Read(u8* Data, LoadManagementParaStruct* Para)
{	
	Data[0] = Para->KwCtrl[0].Option[0].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[0].StartHour;
	Data[2] = Para->KwCtrl[0].Option[0].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[0].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[0].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[0].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[0].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D02_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[1].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[1].StartHour;
	Data[2] = Para->KwCtrl[0].Option[1].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[1].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[1].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[1].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[1].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D03_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[2].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[2].StartHour;
	Data[2] = Para->KwCtrl[0].Option[2].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[2].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[2].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[2].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[2].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D04_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[3].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[3].StartHour;
	Data[2] = Para->KwCtrl[0].Option[3].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[3].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[3].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[3].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[3].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D05_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[4].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[4].StartHour;
	Data[2] = Para->KwCtrl[0].Option[4].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[4].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[4].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[4].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[4].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D06_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[5].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[5].StartHour;
	Data[2] = Para->KwCtrl[0].Option[5].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[5].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[5].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[5].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[5].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D07_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[6].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[6].StartHour;
	Data[2] = Para->KwCtrl[0].Option[6].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[6].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[6].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[6].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[6].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D08_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].Option[7].StartMinute;
	Data[1] = Para->KwCtrl[0].Option[7].StartHour;
	Data[2] = Para->KwCtrl[0].Option[7].EffectiveFlag;
	Data[3] = Para->KwCtrl[0].Option[7].PowerLoad;
	Data[4] = Para->KwCtrl[0].Option[7].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[0].Option[7].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[0].Option[7].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D09_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[0].StartDay;
	Data[1] = Para->KwCtrl[0].StartMonth;
	Data[2] = Para->KwCtrl[0].EndDay;
	Data[3] = Para->KwCtrl[0].EndMonth;
	Data[4] = Para->KwCtrl[0].N0;
	Data[5] = Para->KwCtrl[0].N1;
	Data[6] = Para->KwCtrl[0].N2;
	Data[7] = Para->KwCtrl[0].N3;
	Data[8] = Para->KwCtrl[0].TI;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D0F_Read(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000D00_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000D01_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D02_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D03_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D04_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D05_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D06_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D07_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D08_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D09_Read(Data, Para);
	Data += 9;
	return Result;	
}



ResultEnum QCSG_AFN0A_E0000D10_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].PowerCtrlPeriodCnt;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D11_Read(u8* Data, LoadManagementParaStruct* Para)
{	
	Data[0] = Para->KwCtrl[1].Option[0].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[0].StartHour;
	Data[2] = Para->KwCtrl[1].Option[0].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[0].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[0].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[0].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[0].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D12_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[1].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[1].StartHour;
	Data[2] = Para->KwCtrl[1].Option[1].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[1].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[1].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[1].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[1].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D13_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[2].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[2].StartHour;
	Data[2] = Para->KwCtrl[1].Option[2].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[2].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[2].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[2].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[2].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D14_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[3].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[3].StartHour;
	Data[2] = Para->KwCtrl[1].Option[3].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[3].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[3].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[3].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[3].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D15_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[4].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[4].StartHour;
	Data[2] = Para->KwCtrl[1].Option[4].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[4].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[4].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[4].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[4].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D16_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[5].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[5].StartHour;
	Data[2] = Para->KwCtrl[1].Option[5].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[5].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[5].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[5].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[5].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D17_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[6].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[6].StartHour;
	Data[2] = Para->KwCtrl[1].Option[6].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[6].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[6].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[6].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[6].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D18_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].Option[7].StartMinute;
	Data[1] = Para->KwCtrl[1].Option[7].StartHour;
	Data[2] = Para->KwCtrl[1].Option[7].EffectiveFlag;
	Data[3] = Para->KwCtrl[1].Option[7].PowerLoad;
	Data[4] = Para->KwCtrl[1].Option[7].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[1].Option[7].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[1].Option[7].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D19_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[1].StartDay;
	Data[1] = Para->KwCtrl[1].StartMonth;
	Data[2] = Para->KwCtrl[1].EndDay;
	Data[3] = Para->KwCtrl[1].EndMonth;
	Data[4] = Para->KwCtrl[1].N0;
	Data[5] = Para->KwCtrl[1].N1;
	Data[6] = Para->KwCtrl[1].N2;
	Data[7] = Para->KwCtrl[1].N3;
	Data[8] = Para->KwCtrl[1].TI;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D1F_Read(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000D10_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000D11_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D12_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D13_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D14_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D15_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D16_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D17_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D18_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D19_Read(Data, Para);
	Data += 9;
	return Result;	
}


ResultEnum QCSG_AFN0A_E0000D20_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].PowerCtrlPeriodCnt;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D21_Read(u8* Data, LoadManagementParaStruct* Para)
{	
	Data[0] = Para->KwCtrl[2].Option[0].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[0].StartHour;
	Data[2] = Para->KwCtrl[2].Option[0].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[0].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[0].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[0].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[0].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D22_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[1].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[1].StartHour;
	Data[2] = Para->KwCtrl[2].Option[1].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[1].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[1].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[1].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[1].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D23_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[2].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[2].StartHour;
	Data[2] = Para->KwCtrl[2].Option[2].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[2].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[2].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[2].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[2].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D24_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[3].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[3].StartHour;
	Data[2] = Para->KwCtrl[2].Option[3].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[3].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[3].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[3].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[3].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D25_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[4].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[4].StartHour;
	Data[2] = Para->KwCtrl[2].Option[4].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[4].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[4].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[4].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[4].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D26_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[5].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[5].StartHour;
	Data[2] = Para->KwCtrl[2].Option[5].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[5].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[5].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[5].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[5].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D27_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[6].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[6].StartHour;
	Data[2] = Para->KwCtrl[2].Option[6].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[6].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[6].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[6].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[6].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D28_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].Option[7].StartMinute;
	Data[1] = Para->KwCtrl[2].Option[7].StartHour;
	Data[2] = Para->KwCtrl[2].Option[7].EffectiveFlag;
	Data[3] = Para->KwCtrl[2].Option[7].PowerLoad;
	Data[4] = Para->KwCtrl[2].Option[7].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[2].Option[7].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[2].Option[7].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D29_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[2].StartDay;
	Data[1] = Para->KwCtrl[2].StartMonth;
	Data[2] = Para->KwCtrl[2].EndDay;
	Data[3] = Para->KwCtrl[2].EndMonth;
	Data[4] = Para->KwCtrl[2].N0;
	Data[5] = Para->KwCtrl[2].N1;
	Data[6] = Para->KwCtrl[2].N2;
	Data[7] = Para->KwCtrl[2].N3;
	Data[8] = Para->KwCtrl[2].TI;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D2F_Read(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000D20_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000D21_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D22_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D23_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D24_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D25_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D26_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D27_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D28_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D29_Read(Data, Para);
	Data += 9;
	return Result;	
}


ResultEnum QCSG_AFN0A_E0000D30_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].PowerCtrlPeriodCnt;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D31_Read(u8* Data, LoadManagementParaStruct* Para)
{	
	Data[0] = Para->KwCtrl[3].Option[0].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[0].StartHour;
	Data[2] = Para->KwCtrl[3].Option[0].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[0].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[0].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[0].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[0].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D32_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[1].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[1].StartHour;
	Data[2] = Para->KwCtrl[3].Option[1].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[1].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[1].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[1].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[1].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D33_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[2].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[2].StartHour;
	Data[2] = Para->KwCtrl[3].Option[2].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[2].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[2].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[2].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[2].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D34_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[3].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[3].StartHour;
	Data[2] = Para->KwCtrl[3].Option[3].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[3].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[3].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[3].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[3].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D35_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[4].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[4].StartHour;
	Data[2] = Para->KwCtrl[3].Option[4].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[4].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[4].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[4].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[4].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D36_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[5].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[5].StartHour;
	Data[2] = Para->KwCtrl[3].Option[5].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[5].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[5].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[5].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[5].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D37_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[6].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[6].StartHour;
	Data[2] = Para->KwCtrl[3].Option[6].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[6].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[6].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[6].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[6].PowerLoad >> 24;

	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E0000D38_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].Option[7].StartMinute;
	Data[1] = Para->KwCtrl[3].Option[7].StartHour;
	Data[2] = Para->KwCtrl[3].Option[7].EffectiveFlag;
	Data[3] = Para->KwCtrl[3].Option[7].PowerLoad;
	Data[4] = Para->KwCtrl[3].Option[7].PowerLoad >> 8;
	Data[5] = Para->KwCtrl[3].Option[7].PowerLoad >> 16;
	Data[6] = Para->KwCtrl[3].Option[7].PowerLoad >> 24;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D39_Read(u8* Data, LoadManagementParaStruct* Para)
{
	Data[0] = Para->KwCtrl[3].StartDay;
	Data[1] = Para->KwCtrl[3].StartMonth;
	Data[2] = Para->KwCtrl[3].EndDay;
	Data[3] = Para->KwCtrl[3].EndMonth;
	Data[4] = Para->KwCtrl[3].N0;
	Data[5] = Para->KwCtrl[3].N1;
	Data[6] = Para->KwCtrl[3].N2;
	Data[7] = Para->KwCtrl[3].N3;
	Data[8] = Para->KwCtrl[3].TI;

	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0000D3F_Read(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0000D30_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0000D31_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D32_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D33_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D34_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D35_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D36_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D37_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D38_Read(Data, Para);
	Data += 7;
	Result += QCSG_AFN0A_E0000D39_Read(Data, Para);
	Data += 9;
	return Result;	
}


ResultEnum QCSG_AFN0A_E0001100_E0002000(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	//u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	//u32 ReadLen = 0;
	//MeterPointCtrlParaStruct PointCtrlPara;
	//MemSet((u8*)&PointCtrlPara, 0, sizeof(MeterPointCtrlParaStruct));
	//NAND_ReadPara(PARA_MeterPointControl, QCSGFp->Pn, QCSGFp->CurDataID, (u8*)&PointCtrlPara, &ReadLen);
	//MR((u32)&PointCtrlPara, PARA_MeterPointControl, sizeof(MeterPointCtrlParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0001100:
		case 0xE0001101:
		case 0xE0001102:
		case 0xE0001103:
		case 0xE0001104:
		case 0xE0002000:
			Result = ERROR_VerifyFail;
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}


ResultEnum QCSG_AFN0A_E0800000_E080002F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	u32 ReadLen = sizeof(MeasurePointStruct);
	MeasurePointStruct MeasurePoint;
	MemSet((u8*)&MeasurePoint, 0, sizeof(MeasurePointStruct));
	NAND_ReadMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&MeasurePoint, &ReadLen);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0800000:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800000_Read(pData, &MeasurePoint);
			break;
		case 0xE0800001:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800001_Read(pData, &MeasurePoint);			
			break;
		case 0xE0800002:
			QCSGFp->DstDataPos += 6;
			Result = QCSG_AFN0A_E0800002_Read(pData, &MeasurePoint);
			break;
		case 0xE0800003:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800003_Read(pData, &MeasurePoint);
			break;
		case 0xE0800004:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800004_Read(pData, &MeasurePoint);
			break;
		case 0xE0800005:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800005_Read(pData, &MeasurePoint);
			break;
		case 0xE0800006:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800006_Read(pData, &MeasurePoint);
			break;
		case 0xE0800007:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800007_Read(pData, &MeasurePoint);			
			break;
		case 0xE0800008:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800008_Read(pData, &MeasurePoint);
			break;
		case 0xE0800009:
			QCSGFp->DstDataPos += 6;
			Result = QCSG_AFN0A_E0800009_Read(pData, &MeasurePoint);
			break;
		case 0xE080000A:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E080000A_Read(pData, &MeasurePoint);
			break;
		case 0xE080000B:
			QCSGFp->DstDataPos += 4;
			Result = QCSG_AFN0A_E080000B_Read(pData, &MeasurePoint);
			break;
		case 0xE080000C:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080000C_Read(pData, &MeasurePoint);
			break;
		case 0xE080000D:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080000D_Read(pData, &MeasurePoint);
			break;
		case 0xE080000F:
			QCSGFp->DstDataPos += 29;
			Result = QCSG_AFN0A_E080000F_Read(pData, &MeasurePoint);
			break;
		case 0xE0800010:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800010_Read(pData, &MeasurePoint);
			break;
		case 0xE0800011:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800011_Read(pData, &MeasurePoint);
			break;
		case 0xE0800012:
			QCSGFp->DstDataPos += 3;
			Result = QCSG_AFN0A_E0800012_Read(pData, &MeasurePoint);
			break;
		case 0xE0800013:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800013_Read(pData, &MeasurePoint);
			break;
		case 0xE0800014:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800014_Read(pData, &MeasurePoint);
			break;
		case 0xE080001F:
			QCSGFp->DstDataPos += 10;
			Result = QCSG_AFN0A_E080001F_Read(pData, &MeasurePoint);
			break;
		case 0xE0800021:
			QCSGFp->DstDataPos += 8;
			Result = QCSG_AFN0A_E0800021_Read(pData, &MeasurePoint);
			break;
		case 0xE0800022:
			QCSGFp->DstDataPos += 8;
			Result = QCSG_AFN0A_E0800022_Read(pData, &MeasurePoint);
			break;
		case 0xE080002F:
			QCSGFp->DstDataPos += 16;
			Result = QCSG_AFN0A_E080002F_Read(pData, &MeasurePoint);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}


ResultEnum QCSG_AFN0A_E0800000_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->State;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800001_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->MeasureType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800002_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->Addr[0];
	Data[1] = Para->Addr[1];
	Data[2] = Para->Addr[2];
	Data[3] = Para->Addr[3];
	Data[4] = Para->Addr[4];
	Data[5] = Para->Addr[5];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800003_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->ProtocolType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800004_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->PointMeterType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800005_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->TotalType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800006_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->PriorityType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800007_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->SwitchOff;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800008_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->MaxRateNum;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800009_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->CollectorAddr[0];
	Data[1] = Para->CollectorAddr[1];
	Data[2] = Para->CollectorAddr[2];
	Data[3] = Para->CollectorAddr[3];
	Data[4] = Para->CollectorAddr[4];
	Data[5] = Para->CollectorAddr[5];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080000A_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->PortID;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080000B_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->PortBaudRate;
	Data[1] = Para->PortParity;
	Data[2] = Para->PortDataBit;
	Data[3] = Para->PortStopBit;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080000C_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->CT;
	Data[1] = Para->CT >> 8;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080000D_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->PT;
	Data[1] = Para->PT >> 8;
	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E080000F_Read(u8* Data, MeasurePointStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0800000_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800001_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800002_Read(Data, Para);
	Data += 6;
	Result += QCSG_AFN0A_E0800003_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800004_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800005_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800006_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800007_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800008_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800009_Read(Data, Para);
	Data += 6;
	Result += QCSG_AFN0A_E080000A_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E080000B_Read(Data, Para);
	Data += 4;
	Result += QCSG_AFN0A_E080000C_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E080000D_Read(Data, Para);
	Data += 2;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}



ResultEnum QCSG_AFN0A_E0800010_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->ConnectionType;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800011_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->RatedVoltage;
	Data[1] = Para->RatedVoltage >> 8;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800012_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->RatedCurrent;
	Data[1] = Para->RatedCurrent >> 8;
	Data[2] = Para->RatedCurrent >> 16;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800013_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->UserDeclareCapacity;
	Data[1] = Para->UserDeclareCapacity >> 8;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800014_Read(u8* Data, MeasurePointStruct* Para)
{
	Data[0] = Para->TransformerCapacity;
	Data[1] = Para->TransformerCapacity >> 8;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080001F_Read(u8* Data, MeasurePointStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0800010_Read(Data, Para);
	Data += 1;
	Result += QCSG_AFN0A_E0800011_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800012_Read(Data, Para);
	Data += 3;
	Result += QCSG_AFN0A_E0800013_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800014_Read(Data, Para);
	Data += 2;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;		
}


ResultEnum QCSG_AFN0A_E0800021_Read(u8* Data, MeasurePointStruct* Para)
{
	MemCopy(Data, Para->PassWord, 8);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800022_Read(u8* Data, MeasurePointStruct* Para)
{
	MemCopy(Data, Para->UserName, 8);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080002F_Read(u8* Data, MeasurePointStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0800021_Read(Data, Para);
	Data += 8;
	Result += QCSG_AFN0A_E0800022_Read(Data, Para);
	Data += 8;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN0A_E0800030_E0800031(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	//err20190403 相位参数、台区信息 待填充
	return NO_ERROR;
}

ResultEnum QCSG_AFN0A_E0800030_Read(u8* Data, MeasurePointStruct* Para)
{
	//err20190403 相位参数、台区信息 待填充
	return NO_ERROR;
}

ResultEnum QCSG_AFN0A_E0800031_Read(u8* Data, MeasurePointStruct* Para)
{
	//err20190403 相位参数、台区信息 待填充
	return NO_ERROR;

}




ResultEnum QCSG_AFN0A_E0800100_E080020E(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &DstFrame->Data[QCSGFp->DstDataPos];
	u32 ReadLen = sizeof(PowerQualityParaStruct);
	PowerQualityParaStruct PowerQuality;
	MemSet((u8*)&PowerQuality, 0, sizeof(PowerQualityParaStruct));
	NAND_ReadMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&PowerQuality, &ReadLen);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0800100:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800100_Read(pData, &PowerQuality);
			break;
		case 0xE0800101:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800101_Read(pData, &PowerQuality);			
			break;
		case 0xE0800102:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800102_Read(pData, &PowerQuality);
			break;
		case 0xE0800103:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800103_Read(pData, &PowerQuality);
			break;
		case 0xE0800104:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800104_Read(pData, &PowerQuality);
			break;
		case 0xE0800105:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800105_Read(pData, &PowerQuality);
			break;
		case 0xE0800106:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800106_Read(pData, &PowerQuality);
			break;
		case 0xE0800107:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800107_Read(pData, &PowerQuality);			
			break;
		case 0xE0800108:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800108_Read(pData, &PowerQuality);
			break;
		case 0xE0800109:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800109_Read(pData, &PowerQuality);
			break;
		case 0xE080010A:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080010A_Read(pData, &PowerQuality);
			break;
		case 0xE080010B:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080010B_Read(pData, &PowerQuality);
			break;
		case 0xE080010C:
			QCSGFp->DstDataPos += 8;
			Result = QCSG_AFN0A_E080010C_Read(pData, &PowerQuality);
			break;
		case 0xE080010F:
			QCSGFp->DstDataPos += 32;
			Result = QCSG_AFN0A_E080010F_Read(pData, &PowerQuality);
			break;
		case 0xE0800200:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800200_Read(pData, &PowerQuality);
			break;
		case 0xE0800201:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800201_Read(pData, &PowerQuality);			
			break;
		case 0xE0800202:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800202_Read(pData, &PowerQuality);
			break;
		case 0xE0800203:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800203_Read(pData, &PowerQuality);
			break;
		case 0xE0800205:
			QCSGFp->DstDataPos += 18;
			Result = QCSG_AFN0A_E0800205_Read(pData, &PowerQuality);
			break;
		case 0xE0800206:
			QCSGFp->DstDataPos += 1;
			Result = QCSG_AFN0A_E0800206_Read(pData, &PowerQuality);
			break;
		case 0xE0800207:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800207_Read(pData, &PowerQuality);			
			break;
		case 0xE0800208:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800208_Read(pData, &PowerQuality);
			break;
		case 0xE0800209:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E0800209_Read(pData, &PowerQuality);
			break;
		case 0xE080020A:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080020A_Read(pData, &PowerQuality);
			break;
		case 0xE080020B:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080020B_Read(pData, &PowerQuality);
			break;
		case 0xE080020C:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080020C_Read(pData, &PowerQuality);
			break;
		case 0xE080020D:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080020D_Read(pData, &PowerQuality);
			break;
		case 0xE080020E:
			QCSGFp->DstDataPos += 2;
			Result = QCSG_AFN0A_E080020E_Read(pData, &PowerQuality);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}


ResultEnum QCSG_AFN0A_E0800100_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800100, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800101_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800101, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800102_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800102, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800103_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800103, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800104_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800104, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800105_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800105, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800106_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800106, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800107_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800107, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800108_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800108, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800109_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800109, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080010A_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080010A, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080010B_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080010B, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080010C_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080010C, 8);
	return NO_ERROR;	
}


ResultEnum QCSG_AFN0A_E080010F_Read(u8* Data, PowerQualityParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN0A_E0800100_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800101_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800102_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800103_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800104_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800105_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800106_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800107_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800108_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E0800109_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E080010A_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E080010B_Read(Data, Para);
	Data += 2;
	Result += QCSG_AFN0A_E080010C_Read(Data, Para);
	Data += 8;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN0A_E0800200_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800200, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800201_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800201, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800202_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800202, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800203_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800203, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800205_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800205, 18);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800206_Read(u8* Data, PowerQualityParaStruct* Para)
{
	Data[0] = Para->E0800206;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800207_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800207, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800208_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800208, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E0800209_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E0800209, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080020A_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080020A, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080020B_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080020B, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080020C_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080020C, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080020D_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080020D, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN0A_E080020E_Read(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy(Data, (u8*)&Para->E080020E, 2);
	return NO_ERROR;	
}


/*
ResultEnum QCSG_Check_AFN0A_DataID(u32 DataID, u16* Index)
{
    u32 i = 0; //, u16FnNum = 0;

    if((DataID & 0xFFFFFF00) == 0xE0000300)
    {
        //任务参数表格做了特殊处理
        DataID = 0xE00003FF;
    }
    if((DataID & 0xFFFFFF00) == 0xE0000A00)
    {
        //任务参数表格做了特殊处理
        DataID = 0xE0000AFF;
    }
    for(i = 0; i < MAX_AFN040A_IDENT_NUM; i++)
    {
        if(DataID == gc_stQCSGResolve_AFN0A[i].u32Fn) break;
    }
    if(i >= MAX_AFN040A_IDENT_NUM)
    {
        return ERROR_NoSuchDataID;
    }
    *Index = i;
    return NO_ERROR;
}



ResultEnum CheckANF0APn(u32 Pn)
{
	u32 i = 0, Bit1Cnt = 0;
	for(i = 0; i < 8; i++)
	{
		if((Pn & (1 << i)) != 0) Bit1Cnt++;
	}
	if(Bit1Cnt > 1) return ERROR_WRONG_ACCESS;
	return NO_ERROR;
}
*/







/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


