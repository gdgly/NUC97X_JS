/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN04.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN04处理文件
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
#include "../Device/MEMRW.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Head/QCSG_AFN04.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DAC.h"

#include "Terminal_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"


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
功     能: 	南网上行协议写参数AFN=04操作
入口参 数: 	PORTn: 帧源端口号
			SrcFrame: 输入的源数据帧指针
			DstFrame: 输出的应答数据帧指针
返     回: 返回操作是否成功
编     写: zhaoyun
编写时 间; 2017-7-10
*****************************************************************************************************/
ResultEnum QCSG_AFN04_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	ResultEnum Result = NO_ERROR;
	QCSGFrameProcessStruct QCSGFp;
	u16 i = 0, TmpPnListNum = 0, LastPn = QCSG_INVALID_PN;
	u16* PnList = (u16*)(ADDR_PnListBuf);
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	pTSysCtrl->LastPORTn = PORTn;
	
	MC(0,(u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	QCSGFp.PORTn = PORTn;
	QCSGFp.UserDataLen = SrcFrame->DataLen;
	
//	if((PORTn == RS485_1PORT) || (PORTn == RS485_2PORT) || (PORTn == RS485_3PORT))
//	{
//		QCSGFp.SrcDataPos = QCSGFp.UserDataLen;
//	}

//	if((PORTn == IRPORT) && ((pTSysCtrl->Flag & TERMINAL_PROKEY_PRESS_FLAG) == 0))
//	{
//		QCSGFp.SrcDataPos = QCSGFp.UserDataLen;
//	}
	
    while(QCSGFp.SrcDataPos < QCSGFp.UserDataLen)
    {
		QCSGFp.Pn = QCSG_TakePn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 2;
		QCSGFp.CurDataID = QCSG_TakeFn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 4;
		QCSGFp.SrcDataPosCopy = QCSGFp.SrcDataPos;

		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0))
		{
			TmpPnListNum = 1;
			PnList[0] = QCSGFp.Pn;
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
			QCSGFp.TempPn = PnList[i];
			i++;
			QCSGFp.SrcDataPos = QCSGFp.SrcDataPosCopy;
			if((0xE0000100 <= QCSGFp.CurDataID) && ((0xE000012F >= QCSGFp.CurDataID)))
			{
				Result = QCSG_AFN04_E0000100_E000012F(SrcFrame, DstFrame, &QCSGFp);			  	// 通信参数
			}
			else if((QCSGFp.CurDataID >= 0xE0000130) && (QCSGFp.CurDataID <= 0xE0000174))  	// 时间参数、复位操作、告警屏蔽、结算日参数、身份认证
			{
				Result = QCSG_AFN04_E0000130_E0000174(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000180) && (QCSGFp.CurDataID <= 0xE000018F))   	// 阀值参数
			{
				Result = QCSG_AFN04_E0000180_E000018F(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000200) && (QCSGFp.CurDataID <= 0xE0000220))   	//抄表参数
			{
				Result = QCSG_AFN04_E0000200_E0000220(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000221) && (QCSGFp.CurDataID <= 0xE0000250))   	//抄表通道配置表参数
			{
				Result = QCSG_AFN04_E0000221_E0000250(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000300) && (QCSGFp.CurDataID <= 0xE00003FE))  	//任务参数  普通
			{
				Result = QCSG_AFN04_E0000300_E00003FE(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000400) && (QCSGFp.CurDataID <= 0xE00004FE))  	//任务参数  中继
			{
				Result = QCSG_AFN04_E0000400_E00004FE(SrcFrame, DstFrame, &QCSGFp);
			}
			else if(((QCSGFp.CurDataID >= 0xE0000500) && (QCSGFp.CurDataID <= 0xE00005FF)) || (QCSGFp.CurDataID == 0xE0000900))  
			{
				Result = QCSG_AFN04_E0000500_E00005FF(SrcFrame, DstFrame, &QCSGFp);				//运算量参数   模拟量
			}
			else if((QCSGFp.CurDataID >= 0xE0000600) && (QCSGFp.CurDataID <= 0xE00006FF))  	//运算量参数   脉冲量
			{
				Result = QCSG_AFN04_E0000600_E00006FF(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000700) && (QCSGFp.CurDataID <= 0xE00007FF))  	//运算量参数   运算量
			{
				Result = QCSG_AFN04_E0000700_E00007FF(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000800) && (QCSGFp.CurDataID <= 0xE00008FF))  	//运算量参数   差动量
			{
				Result = QCSG_AFN04_E0000800_E00008FF(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000A00) && (QCSGFp.CurDataID <= 0xE0000B02))  	//端口参数、级联参数、级联控制、版本参数    
			{
				Result = QCSG_AFN04_E0000A00_E0000B02(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0000C00) && (QCSGFp.CurDataID <= 0xE0000E11))  	//负荷管理参数
			{
				Result = QCSG_AFN04_E0000C00_E0000E11(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0001100) && (QCSGFp.CurDataID <= 0xE0002000))  	//测量点控制参数、用户自定义数据   
			{
				Result = QCSG_AFN04_E0001100_E0002000(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0003000) && (QCSGFp.CurDataID <= 0xE0003001))	//err20190403 自动搜表参数
			{
				Result = QCSG_AFN04_E0003000_E0003001(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0800000) && (QCSGFp.CurDataID <= 0xE080002F))  	//测量点参数
			{
				Result = QCSG_AFN04_E0800000_E080002F(SrcFrame, DstFrame, &QCSGFp);
			}
			else if((QCSGFp.CurDataID >= 0xE0800100) && (QCSGFp.CurDataID <= 0xE080020E))	//电能质量检测参数
			{
				Result = QCSG_AFN04_E0800100_E080020E(SrcFrame, DstFrame, &QCSGFp);
			}
			//else if((QCSGFp.CurDataID >= 0xE0AAAA00) && (QCSGFp.CurDataID <= 0xE0AAAA05))	//err20190403 ybwhz自定义
			//{
			//	Result = QCSG_AFN04_E0AAAA00_E0AAAA05(SrcFrame, DstFrame, &QCSGFp);			
			//}
			else
			{
				QCSGFp.SrcDataPos = QCSGFp.UserDataLen;
				Result = ERROR_NoSuchDataID;
			}
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.TempPn;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.TempPn >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 16;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 24;
			DstFrame->Data[QCSGFp.DstDataPos++] = Result;	
			if(Result == NO_ERROR)
			{
				GenerateParaChangeEvent(QCSGFp.CurDataID);
			}
		}		
	}
	if(QCSGFp.DstDataPos == 0)
	{
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=04
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
		return NO_ERROR;
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

ResultEnum QCSG_AFN04_E0000100_E000012F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	CommunicationParaStruct CommPara;
	#if ((Project/100 == 3) && (USER == 3400))
		E000011x_CommunicationParaStruct E0000111_CommPara;
		E000011x_CommunicationParaStruct E0000112_CommPara;
		E000011x_CommunicationParaStruct E0000113_CommPara;
		MemSet((u8*)&E0000111_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MemSet((u8*)&E0000112_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MemSet((u8*)&E0000113_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		
	#endif	
	MemSet((u8*)&CommPara, 0, sizeof(CommunicationParaStruct));
	
	if(QCSGFp->TempPn == 0)
	{
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		#if ((Project/100 == 3) && (USER == 3400))//err20190528 增加云南负控GPRS自适应参数
			MR((u32)&E0000111_CommPara, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));
			MR((u32)&E0000112_CommPara, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));
			MR((u32)&E0000113_CommPara, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));
		#endif
	}
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000100:
			QCSGFp->SrcDataPos += 9;
			Result = QCSG_AFN04_E0000100_Write(pData, &CommPara);
			break;
		case 0xE0000101:
			QCSGFp->SrcDataPos += 9;
			Result = QCSG_AFN04_E0000101_Write(pData, &CommPara);
			break;
		case 0xE0000102:
			QCSGFp->SrcDataPos += 9;
			Result = QCSG_AFN04_E0000102_Write(pData, &CommPara);
			break;
		case 0xE0000103:
			QCSGFp->SrcDataPos += 8;
			Result = QCSG_AFN04_E0000103_Write(pData, &CommPara);
			break;
		case 0xE0000104:
			QCSGFp->SrcDataPos += 16;
			Result = QCSG_AFN04_E0000104_Write(pData, &CommPara);
			break;
		case 0xE0000105:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E0000105_Write(pData, &CommPara);
			break;
		case 0xE0000106:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E0000106_Write(pData, &CommPara);
			break;
		case 0xE0000107:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000107_Write(pData, &CommPara);
			break;
		case 0xE0000108:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000108_Write(pData, &CommPara);
			break;
		case 0xE0000109:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000109_Write(pData, &CommPara);
			break;
		case 0xE000010A:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E000010A_Write(pData, &CommPara);
			break;
		case 0xE000010B:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E000010B_Write(pData, &CommPara);
			break;
		case 0xE000010F:
			QCSGFp->SrcDataPos += 120;
			Result = QCSG_AFN04_E000010F_Write(pData, &CommPara);
			break;
			
	#if ((Project/100 == 3) && (USER == 3400))//err20190528 增加云南负控GPRS自适应参数
		case 0xE0000111://err20190528 增加自适应参数对应数据标识
			QCSGFp->SrcDataPos += 420;
			Result = QCSG_AFN04_E0000111_Write(pData, &E0000111_CommPara);
			break;
		case 0xE0000112:
			QCSGFp->SrcDataPos += 420;
			Result = QCSG_AFN04_E0000112_Write(pData, &E0000112_CommPara);
			break;
		case 0xE0000113:
			QCSGFp->SrcDataPos += 420;
			Result = QCSG_AFN04_E0000113_Write(pData, &E0000113_CommPara);
			break;
	#endif
			
		case 0xE0000120:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E0000120_Write(pData, &CommPara);
			break;
		case 0xE0000121:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E0000121_Write(pData, &CommPara);
			break;
		case 0xE0000122:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000122_Write(pData, &CommPara);
			break;
		case 0xE0000123:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0000123_Write(pData, &CommPara);
			break;
		case 0xE0000124:
			QCSGFp->SrcDataPos += 4;
			Result = QCSG_AFN04_E0000124_Write(pData, &CommPara);
			break;
		case 0xE0000125:
			QCSGFp->SrcDataPos += 4;
			Result = QCSG_AFN04_E0000125_Write(pData, &CommPara);
			break;
		case 0xE0000126:
			QCSGFp->SrcDataPos += 4;
			Result = QCSG_AFN04_E0000126_Write(pData, &CommPara);
			break;
		case 0xE0000127:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000127_Write(pData, &CommPara);
			break;
		case 0xE000012F:
			QCSGFp->SrcDataPos += 22;
			Result = QCSG_AFN04_E000012F_Write(pData, &CommPara);
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			#if ((Project/100 == 3) && (USER == 3400))//err20190528 增加云南负控GPRS自适应参数
				MW((u32)&E0000111_CommPara, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));
				MW((u32)&E0000112_CommPara, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));
				MW((u32)&E0000113_CommPara, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));
			#endif
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}

ResultEnum QCSG_AFN04_E0000100_Write(u8* Data, CommunicationParaStruct* Para)
{	
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	if((Data[8] != MASTER_CH_TYPE_GPRS) && (Data[8] != MASTER_CH_TYPE_PSTN) &&
	   (Data[8] != MASTER_CH_TYPE_ETHE) && (Data[8] != MASTER_CH_TYPE_RS232) &&
	   (Data[8] != MASTER_CH_TYPE_CSD)  && (Data[8] != MASTER_CH_TYPE_RADIO))
	{
		return ERROR_InvalidData;
	}
	if((Data[8] == MASTER_CH_TYPE_GPRS) || (Data[8] == MASTER_CH_TYPE_ETHE))
	{
		if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;
	}
	if((MemCompare(Para->MasterAddr, Data, COMM_AADR_SIZE) != 0) || (Para->MasterAddrType != Data[8]))
	{
		pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	}
	Para->MasterAddrType = Data[8];
	MemCopy(Para->MasterAddr, Data, COMM_AADR_SIZE);	
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000101_Write(u8* Data, CommunicationParaStruct* Para)
{
#if (TEST_ON == 0)
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
#endif
	if((Data[8] != MASTER_CH_TYPE_GPRS) && (Data[8] != MASTER_CH_TYPE_PSTN) &&
	   (Data[8] != MASTER_CH_TYPE_ETHE) && (Data[8] != MASTER_CH_TYPE_RS232) &&
	   (Data[8] != MASTER_CH_TYPE_CSD)  && (Data[8] != MASTER_CH_TYPE_RADIO))
	{
		return ERROR_InvalidData;
	}
	if((Data[8] == MASTER_CH_TYPE_GPRS) || (Data[8] == MASTER_CH_TYPE_ETHE))
	{
		if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;
	}

	if((MemCompare(Para->MasterBackAddr1, Data, COMM_AADR_SIZE) != 0) || (Para->MasterBackAddr1Type != Data[8]))
	{
#if (TEST_ON == 0)
		pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
#endif
	}
	Para->MasterBackAddr1Type = Data[8];
	MemCopy(Para->MasterBackAddr1, Data, COMM_AADR_SIZE);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000102_Write(u8* Data, CommunicationParaStruct* Para)
{
#if (TEST_ON == 0)
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
#endif
	if((Data[8] != MASTER_CH_TYPE_GPRS) && (Data[8] != MASTER_CH_TYPE_PSTN) &&
	   (Data[8] != MASTER_CH_TYPE_ETHE) && (Data[8] != MASTER_CH_TYPE_RS232) &&
	   (Data[8] != MASTER_CH_TYPE_CSD)  && (Data[8] != MASTER_CH_TYPE_RADIO))
	{
		return ERROR_InvalidData;
	}
	if((Data[8] == MASTER_CH_TYPE_GPRS) || (Data[8] == MASTER_CH_TYPE_ETHE))
	{
		if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;
	}

	if((MemCompare(Para->MasterBackAddr2, Data, COMM_AADR_SIZE) != 0) || (Para->MasterBackAddr2Type != Data[8]))
	{
#if (TEST_ON == 0)
		pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
#endif
	}
	Para->MasterBackAddr2Type = Data[8];
	MemCopy(Para->MasterBackAddr2, Data, COMM_AADR_SIZE);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000103_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;

	if(MemCompare(Para->GateWayAddr, Data, COMM_AADR_SIZE) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy(Para->GateWayAddr, Data, COMM_AADR_SIZE);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000104_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare(Para->APN, Data, APN_NAME_MAX_SIZE) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy(Para->APN, Data, APN_NAME_MAX_SIZE);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000105_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare(Para->APNUserName, Data, APN_USER_NAME_MAX_SIZE) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy(Para->APNUserName, Data, APN_USER_NAME_MAX_SIZE);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000106_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare(Para->APNPassWord, Data, APN_USER_PWD_MAX_SIZE) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy(Para->APNPassWord, Data, APN_USER_PWD_MAX_SIZE);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000107_Write(u8* Data, CommunicationParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->HeartBeatTimeGap = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000108_Write(u8* Data, CommunicationParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->RedialTimeGap = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000109_Write(u8* Data, CommunicationParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->RedialCnt = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000010A_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	if(Data[0] > 0x01) return ERROR_InvalidData;
	if(Para->IpLinkType != Data[0]) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	Para->IpLinkType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000010B_Write(u8* Data, CommunicationParaStruct* Para)
{
	if(Data[0] > 0x01) return ERROR_InvalidData;
	Para->CompressType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000010F_Write(u8* Data, CommunicationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000100_Write(Data, Para);
	Data += 9;
	Result += QCSG_AFN04_E0000101_Write(Data, Para);
	Data += 9;
	Result += QCSG_AFN04_E0000102_Write(Data, Para);
	Data += 9;
	Result += QCSG_AFN04_E0000103_Write(Data, Para);
	Data += 8;
	Result += QCSG_AFN04_E0000104_Write(Data, Para);
	Data += 16;
	Result += QCSG_AFN04_E0000105_Write(Data, Para);
	Data += 32;
	Result += QCSG_AFN04_E0000106_Write(Data, Para);
	Data += 32;
	Result += QCSG_AFN04_E0000107_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000108_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000109_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E000010A_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E000010B_Write(Data, Para);
	Data += 1;
	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}

//err20190528 移动自适应参数
ResultEnum QCSG_AFN04_E0000111_Write(u8* Data, E000011x_CommunicationParaStruct* Para)
{
	//参数暂时不做限制
	//TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	//if(Data[8] != MASTER_CH_TYPE_GPRS)	return ERROR_InvalidData;//限制通信类型必须是GPRS
	//if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;//限制端口号必须大于1024
	
	//if((MemCompare(Para->MasterAddr, Data, COMM_AADR_SIZE) != 0) || (Para->MasterAddrType != Data[8]))
	//{
	//	pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	//}
	//Para->MasterAddrType = Data[8];
	MemCopy(Para->MasterAddr,&Data[0], COMM_AADR_SIZE);	
	MemCopy(Para->MasterBackAddr1,&Data[9], COMM_AADR_SIZE);	
	MemCopy(Para->MasterBackAddr2,&Data[18], COMM_AADR_SIZE);	
	MemCopy(Para->GateWayAddr,&Data[27], COMM_AADR_SIZE);

	Para->MasterAddrType = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr3Type = MASTER_CH_TYPE_GPRS;

	MemCopy(Para->APN1,&Data[36], APN_USER_NAME_MAX_SIZE);	//此处为32字节
	MemCopy(Para->APN1UserName,&Data[68], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN1PassWord,&Data[100], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN2,&Data[132], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN2UserName,&Data[164], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN2PassWord,&Data[196], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN3,&Data[228], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN3UserName,&Data[260], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN3PassWord,&Data[292], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN4,&Data[324], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN4UserName,&Data[356], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN4PassWord,&Data[388], APN_USER_PWD_MAX_SIZE);	
	
	return NO_ERROR;	
}

//err20190528 联通自适应参数
ResultEnum QCSG_AFN04_E0000112_Write(u8* Data, E000011x_CommunicationParaStruct* Para)
{
	//参数暂时不做限制
	//TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	//if(Data[8] != MASTER_CH_TYPE_GPRS)	return ERROR_InvalidData;//限制通信类型必须是GPRS
	//if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;//限制端口号必须大于1024
	
	//if((MemCompare(Para->MasterAddr, Data, COMM_AADR_SIZE) != 0) || (Para->MasterAddrType != Data[8]))
	//{
	//	pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	//}
	//Para->MasterAddrType = Data[8];
	MemCopy(Para->MasterAddr,&Data[0], COMM_AADR_SIZE);	
	MemCopy(Para->MasterBackAddr1,&Data[9], COMM_AADR_SIZE);	
	MemCopy(Para->MasterBackAddr2,&Data[18], COMM_AADR_SIZE);	
	MemCopy(Para->GateWayAddr,&Data[27], COMM_AADR_SIZE);

	Para->MasterAddrType = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr3Type = MASTER_CH_TYPE_GPRS;

	MemCopy(Para->APN1,&Data[36], APN_USER_NAME_MAX_SIZE);	//此处为32字节
	MemCopy(Para->APN1UserName,&Data[68], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN1PassWord,&Data[100], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN2,&Data[132], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN2UserName,&Data[164], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN2PassWord,&Data[196], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN3,&Data[228], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN3UserName,&Data[260], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN3PassWord,&Data[292], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN4,&Data[324], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN4UserName,&Data[356], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN4PassWord,&Data[388], APN_USER_PWD_MAX_SIZE);	
	
	return NO_ERROR;	
}

//err20190528 电信自适应参数
ResultEnum QCSG_AFN04_E0000113_Write(u8* Data, E000011x_CommunicationParaStruct* Para)
{
	//参数暂时不做限制
	//TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	//if(Data[8] != MASTER_CH_TYPE_GPRS)	return ERROR_InvalidData;//限制通信类型必须是GPRS
	//if(*(u16*)Data < MIN_TCP_PORT_ID) return ERROR_InvalidData;//限制端口号必须大于1024
	
	//if((MemCompare(Para->MasterAddr, Data, COMM_AADR_SIZE) != 0) || (Para->MasterAddrType != Data[8]))
	//{
	//	pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	//}
	//Para->MasterAddrType = Data[8];
	MemCopy(Para->MasterAddr,&Data[0], COMM_AADR_SIZE);	
	MemCopy(Para->MasterBackAddr1,&Data[9], COMM_AADR_SIZE);	
	MemCopy(Para->MasterBackAddr2,&Data[18], COMM_AADR_SIZE);	
	MemCopy(Para->GateWayAddr,&Data[27], COMM_AADR_SIZE);

	Para->MasterAddrType = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;
	Para->MasterBackAddr3Type = MASTER_CH_TYPE_GPRS;

	MemCopy(Para->APN1,&Data[36], APN_USER_NAME_MAX_SIZE);	//此处为32字节
	MemCopy(Para->APN1UserName,&Data[68], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN1PassWord,&Data[100], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN2,&Data[132], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN2UserName,&Data[164], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN2PassWord,&Data[196], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN3,&Data[228], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN3UserName,&Data[260], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN3PassWord,&Data[292], APN_USER_PWD_MAX_SIZE);	

	MemCopy(Para->APN4,&Data[324], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN4UserName,&Data[356], APN_USER_NAME_MAX_SIZE);	
	MemCopy(Para->APN4PassWord,&Data[388], APN_USER_PWD_MAX_SIZE);	
	
	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000120_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	if(BCD_Check(Data, 3) != NO_ERROR) return ERROR_InvalidData;
	MemCopy((u8*)&Para->TerminalLocationCode, Data, 3);
	if(pTSysCtrl->TerminalLocation != Para->TerminalLocationCode) pTSysCtrl->Flag |= TERMINAL_ADDR_CHANGE_FLAG;
	pTSysCtrl->TerminalLocation = Para->TerminalLocationCode;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000121_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	MemCopy((u8*)&Para->TerminalAddr, Data, 3);
	if(pTSysCtrl->TerminalAddr != Para->TerminalAddr) pTSysCtrl->Flag |= TERMINAL_ADDR_CHANGE_FLAG;
	pTSysCtrl->TerminalAddr = Para->TerminalAddr;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000122_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	if(Data[0] > 0x02) return ERROR_InvalidData;
	if(Para->CommunicationMode != Data[0]) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	Para->CommunicationMode = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000123_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare((u8*)&Para->TcpPortNumber, Data, 2) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy((u8*)&Para->TcpPortNumber, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000124_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare((u8*)&Para->IpAddr, Data, 4) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy((u8*)&Para->IpAddr, Data, 4);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000125_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare((u8*)&Para->IpSubNetMask, Data, 4) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy((u8*)&Para->IpSubNetMask, Data, 4);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000126_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(MemCompare((u8*)&Para->IpGateWayAddr, Data, 4) != 0) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	MemCopy((u8*)&Para->IpGateWayAddr, Data, 4);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000127_Write(u8* Data, CommunicationParaStruct* Para)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	if(Data[0] > 0x01) return ERROR_InvalidData;
	if(Para->IpAddrAccessType != Data[0]) pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	Para->IpAddrAccessType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000012F_Write(u8* Data, CommunicationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000120_Write(Data, Para);
	Data += 3;
	Result += QCSG_AFN04_E0000121_Write(Data, Para);
	Data += 3;
	Result += QCSG_AFN04_E0000122_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000123_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0000124_Write(Data, Para);
	Data += 4;
	Result += QCSG_AFN04_E0000125_Write(Data, Para);
	Data += 4;
	Result += QCSG_AFN04_E0000126_Write(Data, Para);
	Data += 4;
	Result += QCSG_AFN04_E0000127_Write(Data, Para);
	Data += 1;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;
}


ResultEnum QCSG_AFN04_E0000130_E0000174(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 WriteLen = 0;
	u8 DataBuf[100];//err20180710 每日上报一次的告警标识，需掉电存储
	SettleDateParaStruct* pSettleDate = (SettleDateParaStruct*)DataBuf;
	AlarmEventMaskParaStruct* pAlarmEventMask = (AlarmEventMaskParaStruct*)DataBuf;	
	IDAuthenticationParaStruct* pIDAuthentication = (IDAuthenticationParaStruct*)DataBuf;
	#if ((Project/100) == 3)
		u8 u8PnNum = 0,i = 0;
		u8 u8MeterId[6];
		u32 u32RevLen = 0;
		ERD09Struct stERD09Data;
		RealTimeYYStruct stYYTime;
		TIME_PARAM_S pstTime;			
		QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
		QCSG_RAM_DATA_S* pstRam_Data = NULL;
	#endif
#if (TEST_ON == 1)
	TerminalRunTimeCtrlStruct* pTSysCtrl;
#endif
	
	MemSet(DataBuf, 0, 100);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000130:
			QCSGFp->SrcDataPos += 6;
			if(QCSGFp->TempPn == 0)
			{
				WriteLen = 6;
				Result = NAND_WritePara(PARA_RealTime, QCSGFp->Pn, QCSGFp->CurDataID, pData, &WriteLen);

				//err20181107 增加事件 负控 终端校时记录事件
			#if ((Project/100) == 3)
				HAL_GetSystemTime(&pstTime);
				
				if(QCSG_CheckEventMask(0xE2010010) == TRUE)
				{
					u8PnNum = pstPn_Ram_Data->u8PnNum;
					for(i = 0;i < u8PnNum;i++)
					{
						//寻找内存中记录的测量点数据信息
						if(0 == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
						{
							pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
							break;
						}
					}	
					if(i >= u8PnNum)
					{
						Result = ERROR_VerifyFail;
						return Result;
					}	
					MemCopy(&stERD09Data.Time,&pstTime,sizeof(TIME_PARAM_S));
					MemCopy(stERD09Data.C_00010000,pstRam_Data->u8Data00010000,4);
					MemCopy(stERD09Data.C_00020000,pstRam_Data->u8Data00020000,4);

					GetACSampleAddr(u8MeterId);	//取交采地址
					//记录事件
					MemSet(&stYYTime,0,sizeof(stYYTime));
					//RD_Time_TO_YYTime(&pstTime,&stYYTime);//转换时间格式
					stYYTime.YearH = 0x20;
					stYYTime.YearL = pstTime.u8Year;
					stYYTime.Month = pstTime.u8Month;
					stYYTime.Day = pstTime.u8Day;
					stYYTime.Hour = pstTime.u8Hour;
					stYYTime.Minute = pstTime.u8Minute;
					
					u32RevLen = sizeof(stERD09Data);
					NAND_WriteEventRecord(u8MeterId,0xE2010010,&stYYTime,(u8*)&stERD09Data,&u32RevLen);
				}
			#endif
			
			}
			else
			{
				Result = ERROR_VerifyFail;
			}			
			break;
		case 0xE0000131:
			QCSGFp->SrcDataPos += 4;
			if(QCSGFp->TempPn == 0)
			{
				Result = QCSG_AFN04_E0000131_Write(pData, QCSGFp, SrcFrame);
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000140:
			QCSGFp->SrcDataPos += 1;
			if(QCSGFp->TempPn == 0)
			{
				Result = QCSG_AFN04_E0000140_Write(pData, QCSGFp);
			}
			else
			{
				Result = ERROR_VerifyFail;
			}	
			break;
		case 0xE0000150:
			QCSGFp->SrcDataPos += 32;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
				MemCopy(pAlarmEventMask->AlarmReportMask, pData, 32);
				MW((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
#if (TEST_ON == 1)
				pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
				TerminalReportCheckAlarm(pTSysCtrl);//err20180627 台体测试时 设置完150屏蔽字后将所有未上报告警全部上报。
#endif
			}
			else
			{
				Result = ERROR_VerifyFail;
			}	
			break;
		case 0xE0000151:
			QCSGFp->SrcDataPos += 32;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
				MemCopy(pAlarmEventMask->AlarmJudgeMask, pData, 32);
				MW((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000152:
			QCSGFp->SrcDataPos += 32;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
				MemCopy(pAlarmEventMask->EventRecordMask, pData, 32);
				MW((u32)pAlarmEventMask, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000160:
			QCSGFp->SrcDataPos += 2;
			if((QCSGFp->TempPn == 0) && (BCD_Check(pData, 2) == NO_ERROR) && (pData[1] <= HOUR_BCD_MAX) && 
			   (pData[0] >= DAY_BCD_MIN) && (pData[0] < DAY_BCD_MAX))
			{
				MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
				pSettleDate->MonthPowerSettleDay = pData[0];
				pSettleDate->MonthPowerSettleHour = pData[1];
				MW((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			}
			else
			{
				if(QCSGFp->TempPn != 0)
				{
					Result = ERROR_VerifyFail;
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}
			break;
		case 0xE0000161:
			QCSGFp->SrcDataPos += 1;
			if((QCSGFp->TempPn == 0) && (BCD_Check(pData, 1) == NO_ERROR) && (pData[0] <= HOUR_BCD_MAX))
			{
				MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
				pSettleDate->DayPowerSettleHour = pData[0];
				MW((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			}
			else
			{
				if(QCSGFp->TempPn != 0)
				{
					Result = ERROR_VerifyFail;
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}
			break;
		case 0xE0000162:
			QCSGFp->SrcDataPos += 2;
			if((QCSGFp->TempPn == 0) && (BCD_Check(pData, 2) == NO_ERROR) && (pData[1] <= HOUR_BCD_MAX) && 
			   (pData[0] >= DAY_BCD_MIN) && (pData[0] < DAY_BCD_MAX))
			{
				MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
				pSettleDate->MonthVoltageSettleDay = pData[0];
				pSettleDate->MonthVoltageSettleHour = pData[1];
				MW((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			}
			else
			{
				if(QCSGFp->TempPn != 0)
				{
					Result = ERROR_VerifyFail;
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}
			break;
		case 0xE0000163:
			QCSGFp->SrcDataPos += 1;
			if((QCSGFp->TempPn == 0) && (BCD_Check(pData, 1) == NO_ERROR) && (pData[0] <= HOUR_BCD_MAX))
			{
				MR((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
				pSettleDate->DayVoltageSettleHour= pData[0];
				MW((u32)pSettleDate, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));
			}
			else
			{
				if(QCSGFp->TempPn != 0)
				{
					Result = ERROR_VerifyFail;
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}			
			break;
		case 0xE0000170:
			QCSGFp->SrcDataPos += 1;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
				pIDAuthentication->VerifyType = pData[0];
				MW((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}			
			break;
		case 0xE0000171:
			QCSGFp->SrcDataPos += 16;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
				MemCopy(pIDAuthentication->AuthReqInfo, pData, 16);
				MW((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000172:
			QCSGFp->SrcDataPos += 16;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
				MemCopy(pIDAuthentication->AuthRespInfo, pData, 16);
				MW((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000173:
			QCSGFp->SrcDataPos += 16;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
				MemCopy(pIDAuthentication->RandomReqInfo, pData, 16);
				MW((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000174:
			QCSGFp->SrcDataPos += 16;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
				MemCopy(pIDAuthentication->RandomRespInfo, pData, 16);
				MW((u32)pIDAuthentication, ADDR_SettlementDatePara, sizeof(IDAuthenticationParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}

//实时校时
ResultEnum QCSG_AFN04_E0000131_Write(u8* Data, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame)
{
	u8* PnListData;
	RealTimeStruct RealTime;
	DLT645FrameStruct Frame;
#if (TEST_ON == 1)
	u32 temp;
	Terminal_Router_TypeDef* pTerminalRouter;
#endif
	
	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
	#endif

	#if ((Project/100)==2||(Project/100)==5 ||(Project/100)==9) //II型集中器也应包含，20180531
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#endif
	u32 Pn = 0, Time = 0, i = 0;
	
	HAL_GetSystemTime(&RealTime);
	Pn = Data[0];
	Pn += Data[1] << 8;

	if(Pn != 0xFFFF) return ERROR_InvalidData;

	Time = Data[2];
	Time += Data[3] << 8;

	if((Time & 0x00008000) != 0)
	{// 符号位为 负
		Time &= ~0x00008000;
		for(i = 0; i < Time; i++)
		{
			QCSG_DecOneSecond(&RealTime);
		}
	}
	else
	{// 符号位为 正
		for(i = 0; i < Time; i++)
		{
			QCSG_IncOneSecond(&RealTime);
		}
	}
	
#if (TEST_ON == 1)
	
	pTerminalRouter = Terminal_Router;
	temp = pTerminalRouter->RouterMFCode[0];
	temp += pTerminalRouter->RouterMFCode[1] << 8;
	switch((u16)temp)		//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
	{	
		case (('T'<<8)+'C')://鼎信载波 27s
			for(i = 0 ; i < 27 ; i++)
			{
				QCSG_IncOneSecond(&RealTime);//台体测试延迟+1s
			}
			break;
		case (('F'<<8)+'C')://友讯达微功率无线 10s
			for(i = 0 ; i < 10 ; i++)
			{
				QCSG_IncOneSecond(&RealTime);//台体测试延迟+1s
			}
			break;
		default: 
			for(i = 0 ; i < 27 ; i++)
			{
				QCSG_IncOneSecond(&RealTime);//台体测试延迟+1s
			}
			break;
	}
	#endif

	i = 0;
	PnListData = (u8*)Get_ADDR_UARTnFnDataBuff(QCSGFp->PORTn);

	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = QCSGFp->CurDataID;
	PnListData[i++] = QCSGFp->CurDataID >> 8;
	PnListData[i++] = QCSGFp->CurDataID >> 16;
	PnListData[i++] = QCSGFp->CurDataID >> 24;

	MR((u32)Frame.Addr, (u32)BroadCastAddr, METER_ADDR_SIZE);
	Frame.CtrlCode = 0x08;
	Frame.DataLen = 0x06;
	Frame.Data[0] = RealTime.u8Second;
	Frame.Data[1] = RealTime.u8Minute;
	Frame.Data[2] = RealTime.u8Hour;
	Frame.Data[3] = RealTime.u8Day;
	Frame.Data[4] = RealTime.u8Month;
	Frame.Data[5] = RealTime.u8Year;

	i += Build645Frame(&Frame, &PnListData[i], NO_FE_IN_645);

	i -= 8;
	PnListData[2] = i;
	PnListData[3] = i >> 8;

	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(QCSGFp->PORTn);
		if((RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			RT_AMRReq->SrcTask = GetUartTaskID(QCSGFp->PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_DLT_645;
			RT_AMRReq->DataType  = DATA_TYPE_BCAST_CMD;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = QCSGFp->PORTn;
			RT_AMRReq->PnListNum = 1;
			RT_AMRReq->PnListLen = i + 8;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, RT_AMRReq->PnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
	#endif

	#if ((Project/100)==2||(Project/100)==5||(Project/100)==9)		// I 型、II型集中器，20180531
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(QCSGFp->PORTn);
		if((UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			UartRxChCtrl->SrcPort = QCSGFp->PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_DLT_645;
			UartRxChCtrl->DataType = DATA_TYPE_BCAST_CMD;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = QCSGFp->PORTn;
			UartRxChCtrl->PnListNum  = 1;
			UartRxChCtrl->FrameLen = i + 8;
			UartRxChCtrl->FrameBuf = PnListData;
			switch(QCSGFp->PORTn)
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
					UartRxChCtrl->Empty = 0;
					UartRxChCtrl->PnListNum  = 0;
					UartRxChCtrl->FrameLen = 0;
					UartRxChCtrl->FrameBuf = NULL;
					break;
			}
		}
	#endif
	
	return NO_ERROR;	
}
ResultEnum QCSG_AFN04_E0000140_Write(u8* Data, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
		
	if(Data[0] <= 0x02)
	{
		DataResetAction(Data[0], QCSGFp->PORTn);
	}	
	else
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN04_E0000180_E000018F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	ThresholdParaStruct ThresholdPara;
	MemSet((u8*)&ThresholdPara, 0, sizeof(ThresholdParaStruct));
	if(QCSGFp->TempPn == 0)
	{
		MR((u32)&ThresholdPara, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));
	}
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000180:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000180_Write(pData, &ThresholdPara);
			break;
		case 0xE0000181:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000181_Write(pData, &ThresholdPara);
			break;
		case 0xE0000182:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000182_Write(pData, &ThresholdPara);
			break;
		case 0xE0000183:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000183_Write(pData, &ThresholdPara);
			break;
		case 0xE0000184:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000184_Write(pData, &ThresholdPara);
			break;
		case 0xE0000185:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000185_Write(pData, &ThresholdPara);
			break;
		case 0xE0000186:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000186_Write(pData, &ThresholdPara);
			break;
		case 0xE0000187:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000187_Write(pData, &ThresholdPara);
			break;
		case 0xE0000188:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000188_Write(pData, &ThresholdPara);
			break;
		case 0xE0000189:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000189_Write(pData, &ThresholdPara);
			break;
		case 0xE000018A:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E000018A_Write(pData, &ThresholdPara);
			break;
		case 0xE000018B:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E000018B_Write(pData, &ThresholdPara);
			break;
		case 0xE000018C:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E000018C_Write(pData, &ThresholdPara);
			break;
		case 0xE000018F:
			QCSGFp->SrcDataPos += 18;
			Result = QCSG_AFN04_E000018F_Write(pData, &ThresholdPara);
			break;		
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
		
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&ThresholdPara, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));	
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}


ResultEnum QCSG_AFN04_E0000180_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000180 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000181_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000181 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000182_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000182 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000183_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000183 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000184_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000184 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000185_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000185 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000186_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000186 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000187_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000187 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000188_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000188 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000189_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 1) != NO_ERROR) return ERROR_InvalidData;
	Para->E0000189 = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000018A_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 2) != NO_ERROR) return ERROR_InvalidData;
	Para->E000018A = Data[1];
	Para->E000018A <<= 8;
	Para->E000018A += Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000018B_Write(u8* Data, ThresholdParaStruct* Para)
{
	if(BCD_Check(Data, 3) != NO_ERROR) return ERROR_InvalidData;
	MemCopy((u8*)&Para->E000018B, Data, 3);

	/*
	SelfAdaptionParaStruct temp;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	MR((u32)&temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));
	
	if(MemCompare(&temp.SelfAdaptionFlag, Data, 1) != 0)
	{
		pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
	}
	
	if(QCSG_BCD2HEX(Data[0])>=1)
		temp.SelfAdaptionFlag = 0x01;
	else 
		temp.SelfAdaptionFlag = 0x00;
	MW((u32)&temp, ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));	
*/
	
	//err20181109 CT二次 测试用 
	/*
	u32 Temp=0;
	//MemCopy((u8*)&Temp, Data, 3);
	Temp = QCSG_BCD2HEX(Data[2]) *10000 +  QCSG_BCD2HEX(Data[1]) *100 + QCSG_BCD2HEX(Data[0]);
	offSquareWave();
	onSquareWave(Temp);
	*/
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000018C_Write(u8* Data, ThresholdParaStruct* Para)
{
	AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
	if(BCD_Check(Data, 3) != NO_ERROR) return ERROR_InvalidData;
	MemCopy((u8*)&Para->E000018C, Data, 3);
	AlarmAMRDataOld[0].AlarmFlag &= ~ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A;
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E000018F_Write(u8* Data, ThresholdParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;

	Result += QCSG_AFN04_E0000180_Write(Data, Para);
	Data += 1;	
	Result += QCSG_AFN04_E0000181_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000182_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000183_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000184_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000185_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000186_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000187_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000188_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000189_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E000018A_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E000018B_Write(Data, Para);
	Data += 3;
	Result += QCSG_AFN04_E000018C_Write(Data, Para);
	Data += 3;
	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}	
	return Result;
}



ResultEnum QCSG_AFN04_E0000200_E0000220(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	AMRChTimeStruct AMRChTime;
	RealTimeStruct Time;
	
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 FlashAddr = 0;
	MemSet((u8*)&AMRChTime, 0, sizeof(AMRChTimeStruct));
	
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000200:		//第一路485抄表参数
			QCSGFp->SrcDataPos += 7;
			if(QCSGFp->TempPn == 0)
			{
				Time.u8Second = 0;
				Time.u8Minute = pData[0];
				Time.u8Hour = pData[1];
				Time.u8Day = pData[2];
				Time.u8Month = pData[3];
				Time.u8Year = pData[4];
				Time.u8Week = 0;
				Result = TimeValidCheck(&Time);
				if((Result == NO_ERROR) && (pData[5] <= PERIOD_UNIT_MONTH))
				{
					MR((u32)&AMRChTime, ADDR_AMRChTimeTbl, sizeof(AMRChTimeStruct));
					MemCopy((u8*)&AMRChTime, pData, 7);
					MW((u32)&AMRChTime, ADDR_AMRChTimeTbl, sizeof(AMRChTimeStruct));
				}
				else
				{//如果设置非法值 则给定一个基准值//err20190612 为过台体 台体下发的时间非法。
					//AMRChTime.u8Second = 0x00;
					AMRChTime.Minute = 0x00;
					AMRChTime.Hour = 0x00;
					AMRChTime.Day = 0x01;
					AMRChTime.Month = 0x01;
					AMRChTime.Year = 0x00;
					//Time.u8Week = 0;
					AMRChTime.PeriodUnit = PERIOD_UNIT_MINUTE;
					AMRChTime.Period = 0x02;
					//MR((u32)&AMRChTime, ADDR_AMRChTimeTbl, sizeof(AMRChTimeStruct));
					//MemCopy((u8*)&AMRChTime, pData, 7);
					MW((u32)&AMRChTime, ADDR_AMRChTimeTbl, sizeof(AMRChTimeStruct));
					Result = ERROR_InvalidData;
				}
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000201:
			QCSGFp->SrcDataPos += 7;
			if(QCSGFp->TempPn == 0)
			{
				Time.u8Second = 0;
				Time.u8Minute = pData[0];
				Time.u8Hour = pData[1];
				Time.u8Day = pData[2];
				Time.u8Month = pData[3];
				Time.u8Year = pData[4];
				Time.u8Week = 0;
				Result = TimeValidCheck(&Time);
				if((Result == NO_ERROR) && (pData[5] <= PERIOD_UNIT_MONTH))
				{
					FlashAddr = ADDR_AMRChTimeTbl + sizeof(AMRChTimeStruct);
					MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
					MemCopy((u8*)&AMRChTime, pData, 7);
					MW((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}			
			}
			else
			{
				Result = ERROR_VerifyFail;
			}		
			break;
		case 0xE0000202:
			QCSGFp->SrcDataPos += 7;
			if(QCSGFp->TempPn == 0)
			{
				Time.u8Second = 0;
				Time.u8Minute = pData[0];
				Time.u8Hour = pData[1];
				Time.u8Day = pData[2];
				Time.u8Month = pData[3];
				Time.u8Year = pData[4];
				Time.u8Week = 0;
				Result = TimeValidCheck(&Time);
				if((Result == NO_ERROR) && (pData[5] <= PERIOD_UNIT_MONTH))
				{
					FlashAddr = ADDR_AMRChTimeTbl + (2 * sizeof(AMRChTimeStruct));
					MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
					MemCopy((u8*)&AMRChTime, pData, 7);
					MW((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}	
			}
			else
			{
				Result = ERROR_VerifyFail;
			}			
			break;
		case 0xE000021F:
			QCSGFp->SrcDataPos += 7;
			if(QCSGFp->TempPn == 0)
			{
				Time.u8Second = 0;
				Time.u8Minute = pData[0];
				Time.u8Hour = pData[1];
				Time.u8Day = pData[2];
				Time.u8Month = pData[3];
				Time.u8Year = pData[4];
				Time.u8Week = 0;
				Result = TimeValidCheck(&Time);
				if((Result == NO_ERROR) && (pData[5] <= PERIOD_UNIT_MONTH))
				{
					FlashAddr = ADDR_AMRChTimeTbl + (3 * sizeof(AMRChTimeStruct));
					MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
					MemCopy((u8*)&AMRChTime, pData, 7);
					MW((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}			
			}
			else
			{
				Result = ERROR_VerifyFail;
			}			
			break;
		case 0xE0000220:
			QCSGFp->SrcDataPos += 7;
			if(QCSGFp->TempPn == 0)
			{
				Time.u8Second = 0;
				Time.u8Minute = pData[0];
				Time.u8Hour = pData[1];
				Time.u8Day = pData[2];
				Time.u8Month = pData[3];
				Time.u8Year = pData[4];
				Time.u8Week = 0;
				Result = TimeValidCheck(&Time);
				if((Result == NO_ERROR) && (pData[5] <= PERIOD_UNIT_MONTH))
				{
					FlashAddr = ADDR_AMRChTimeTbl + (4 * sizeof(AMRChTimeStruct));
					MR((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
					MemCopy((u8*)&AMRChTime, pData, 7);
					MW((u32)&AMRChTime, FlashAddr, sizeof(AMRChTimeStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}				
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}



ResultEnum QCSG_AFN04_E0000221_E0000250(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	AMRChSetStruct AMRChSet;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];	
	u32 FlashAddr = 0, Index = 0;
	MemSet((u8*)&AMRChSet, 0, sizeof(AMRChSetStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000221:			//抄表参数配置表总数
			QCSGFp->SrcDataPos += 1;
			Result = ERROR_VerifyFail;
			break;
		case 0xE0000230:		//485-1抄表参数(有效或无效)
			FlashAddr = ADDR_AMRChSetTbl;			
			break;
		case 0xE0000231:		//485-2抄表参数(有效或无效)
			FlashAddr = ADDR_AMRChSetTbl + sizeof(AMRChSetStruct);
			break;
		case 0xE0000232:		//485-3抄表参数(有效或无效)
			FlashAddr = ADDR_AMRChSetTbl + (2 * sizeof(AMRChSetStruct));
			break;
		case 0xE000024F:		//载波端口抄表参数(有效或无效)
			FlashAddr = ADDR_AMRChSetTbl + (3 * sizeof(AMRChSetStruct));
			break;
		case 0xE0000250:		//无线端口抄表参数(有效或无效)
			FlashAddr = ADDR_AMRChSetTbl + (4 * sizeof(AMRChSetStruct));
			break;		
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	if(FlashAddr != 0)
	{
#if (TEST_ON == 1)
		MR((u32)&AMRChSet, FlashAddr, sizeof(AMRChSetStruct));
		AMRChSet.Flag	  = pData[Index++];
		AMRChSet.StoreNum = pData[Index + 1];
		AMRChSet.StoreNum <<= 8;
		AMRChSet.StoreNum += pData[Index];
		Index += 2;
		AMRChSet.ChMeterType = pData[Index++];
		AMRChSet.UserType = pData[Index++];
		AMRChSet.DANum = pData[Index++];
		if(AMRChSet.DANum == 0xFF)
		{
			AMRChSet.DA[0] = pData[Index++];
			AMRChSet.DA[0] += pData[Index++] << 8;
		}
		else
		{
			MemCopy((u8*)AMRChSet.DA, &pData[Index], (u16)AMRChSet.DANum * 2);
			Index += AMRChSet.DANum * 2;
		}

		AMRChSet.DataIDNum = pData[Index++];
		if(AMRChSet.DataIDNum == 0xFF)
		{
			AMRChSet.DataID[0] = pData[Index++];
			AMRChSet.DataID[0] += pData[Index++] << 8;
			AMRChSet.DataID[0] += pData[Index++] << 16;
			AMRChSet.DataID[0] += pData[Index++] << 24;
		}
		else
		{
			if(AMRChSet.DataIDNum > AMR_MAX_DATA_ID) 
			{
				Result = ERROR_InvalidData;
			}
			else
			{
				MemCopy((u8*)AMRChSet.DataID, &pData[Index], (u16)AMRChSet.DataIDNum * 4);
			}
			Index += ((u32)AMRChSet.DataIDNum) * 4;
		}

		if(QCSGFp->TempPn == 0)
		{	
			if(Result == NO_ERROR)
			{
				MW((u32)&AMRChSet, FlashAddr, sizeof(AMRChSetStruct));
			}
		}
		else
		{
			Result = ERROR_VerifyFail;
		}			
		
		QCSGFp->SrcDataPos += Index;
#else
		MR((u32)&AMRChSet, FlashAddr, sizeof(AMRChSetStruct));
		AMRChSet.Flag	  = pData[Index++];		
		if(AMRChSet.Flag > 1) Result = ERROR_InvalidData;//err20180404-16 有效性 范围 0无效 1有效 
		
		AMRChSet.StoreNum = pData[Index + 1];
		AMRChSet.StoreNum <<= 8;
		AMRChSet.StoreNum += pData[Index];
		if(BCD_Check((u8*)&AMRChSet.StoreNum, 2) != NO_ERROR) 
			Result = ERROR_InvalidData;//err20180404-16 存储点数范围 BCD格式 

		Index += 2;
		AMRChSet.ChMeterType = pData[Index++];
		if((AMRChSet.ChMeterType != METER_TYPE_SINGLE) &&
			(AMRChSet.ChMeterType != METER_TYPE_MULTI) &&
			(AMRChSet.ChMeterType != METER_TYPE_OTHER)) 
			Result = ERROR_InvalidData;//err20180404-16 电表类型 范围 1单相 2多功能 3其他
		
		AMRChSet.UserType = pData[Index++];
		if(AMRChSet.UserType > 1) //&& (AMRChSet.UserType != 0xFF)) 
				Result = ERROR_InvalidData;//err20180404-16 有效性 范围 0普通 1重点

		
		AMRChSet.DANum = pData[Index++];
		if(AMRChSet.DANum == 0xFF)
		{
			AMRChSet.DA[0] = pData[Index++];
			AMRChSet.DA[0] += pData[Index++] << 8;
		}
		else
		{
			MemCopy((u8*)AMRChSet.DA, &pData[Index], (u16)AMRChSet.DANum * 2);
			Index += AMRChSet.DANum * 2;
		}

		AMRChSet.DataIDNum = pData[Index++];
		if(AMRChSet.DataIDNum == 0xFF)
		{
			AMRChSet.DataID[0] = pData[Index++];
			AMRChSet.DataID[0] += pData[Index++] << 8;
			AMRChSet.DataID[0] += pData[Index++] << 16;
			AMRChSet.DataID[0] += pData[Index++] << 24;
		}
		else
		{
			if(AMRChSet.DataIDNum > AMR_MAX_DATA_ID) 
			{
				Result = ERROR_InvalidData;
			}
			else
			{
				MemCopy((u8*)AMRChSet.DataID, &pData[Index], (u16)AMRChSet.DataIDNum * 4);
			}
			Index += ((u32)AMRChSet.DataIDNum) * 4;
		}

		if(QCSGFp->TempPn == 0)
		{	
			if(Result == NO_ERROR)
			{
				MW((u32)&AMRChSet, FlashAddr, sizeof(AMRChSetStruct));
			}
		}
		else
		{
			Result = ERROR_VerifyFail;
		}			
		
		QCSGFp->SrcDataPos += Index;
#endif
	}	
	return Result;
}



ResultEnum QCSG_AFN04_E0000300_E00003FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	NormalTaskParaStruct NormalTaskPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];	
	u32 FlashAddr = 0, Index = 0;
	MemSet((u8*)&NormalTaskPara, 0, sizeof(NormalTaskParaStruct));

	if(QCSGFp->CurDataID == 0xE0000300)//有效的普通任务总数
	{
		QCSGFp->SrcDataPos += 1;
		Result = ERROR_InvalidData;
	}
	//else if((QCSGFp->CurDataID >= 0xE0000301) && (QCSGFp->CurDataID <= 0xE00003FE))
	else if((QCSGFp->CurDataID >= 0xE0000301) && (QCSGFp->CurDataID <= 0xE0000320))
	{
		FlashAddr = ADDR_NormalTaskPara + (((QCSGFp->CurDataID - 1) & 0x000000FF) * sizeof(NormalTaskParaStruct));
	}
	else
	{
		QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
		Result = ERROR_InvalidData;
	}
	
	if(FlashAddr != 0)
	{
		MR((u32)&NormalTaskPara, FlashAddr, sizeof(NormalTaskParaStruct));
		HAL_GetSystemTime(&NormalTaskPara.ReportTime);
		HAL_GetSystemTime(&NormalTaskPara.SampleTime);
		Result = NO_ERROR;
			
		NormalTaskPara.Flag = pData[Index++];
		NormalTaskPara.ReportTime.u8Minute = pData[Index++];
		NormalTaskPara.ReportTime.u8Hour   = pData[Index++];
		NormalTaskPara.ReportTime.u8Day    = pData[Index++];
		NormalTaskPara.ReportTime.u8Month  = pData[Index++];
		NormalTaskPara.ReportTime.u8Year   = pData[Index++];
		//NormalTaskPara.ReportTime.u8Week   = 0;
		Result += TimeValidCheck(&NormalTaskPara.ReportTime);
		
		NormalTaskPara.ReportPeriodUnit	   = pData[Index++];
		NormalTaskPara.ReportPeriod		   = pData[Index++];
		NormalTaskPara.DataStructureType   = pData[Index++];
#if (TEST_ON == 1)
		if((NormalTaskPara.DataStructureType > DATA_STRUCT_TYPE_TASK) || 
		   (NormalTaskPara.ReportPeriodUnit > PERIOD_UNIT_MONTH) )
#else
		if((NormalTaskPara.DataStructureType > DATA_STRUCT_TYPE_TASK) || 
		   	(NormalTaskPara.ReportPeriodUnit > PERIOD_UNIT_MONTH) ||
			(NormalTaskPara.Flag > 0x01))	//err20180404-08 有效性参数可设范围为 00 01
#endif
		{
			Result = ERROR_InvalidData;
		}

		NormalTaskPara.SampleTime.u8Minute = pData[Index++];
		NormalTaskPara.SampleTime.u8Hour   = pData[Index++];
		NormalTaskPara.SampleTime.u8Day    = pData[Index++];
		NormalTaskPara.SampleTime.u8Month  = pData[Index++];
		NormalTaskPara.SampleTime.u8Year   = pData[Index++];
		//NormalTaskPara.SampleTime.u8Week   = 0;
		Result += TimeValidCheck(&NormalTaskPara.SampleTime);
		
		NormalTaskPara.SamplePeriodUnit	   = pData[Index++];
		if(NormalTaskPara.SamplePeriodUnit > PERIOD_UNIT_MONTH)
		{
			Result = ERROR_InvalidData;
		}
		NormalTaskPara.SamplePeriod		   = pData[Index++];
		NormalTaskPara.GetRate		       = pData[Index++];
		
		NormalTaskPara.ExecCnt = pData[Index++];
		NormalTaskPara.ExecCnt += pData[Index++] << 8;
		NormalTaskPara.RealExeCnt = 0;	
		
		NormalTaskPara.DANum = pData[Index++];
		if(NormalTaskPara.DANum != 0)
		{
			MemCopy((u8*)NormalTaskPara.DA, &pData[Index], NormalTaskPara.DANum * 2);
			Index += NormalTaskPara.DANum * 2;
		}
		
		NormalTaskPara.DataIDNum = pData[Index++];		
		if(NormalTaskPara.DataIDNum > AMR_MAX_DATA_ID) 
		{
			Result = ERROR_InvalidData;
		}
		else
		{
			if(QCSGFp->TempPn == 0)
			{
				if(Result == NO_ERROR)
				{
					if(NormalTaskPara.DataIDNum != 0)
					{
						MemCopy((u8*)NormalTaskPara.DataID, &pData[Index], NormalTaskPara.DataIDNum * 4);				
					}
					MW((u32)&NormalTaskPara, FlashAddr, sizeof(NormalTaskParaStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}		
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
		}
		Index += NormalTaskPara.DataIDNum * 4;
		QCSGFp->SrcDataPos += Index;
	}	
	return Result;
}




ResultEnum QCSG_AFN04_E0000400_E00004FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	RelayTaskParaStruct RelayTaskPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];	
	u32 FlashAddr = 0, Index = 0;
	MemSet((u8*)&RelayTaskPara, 0, sizeof(RelayTaskParaStruct));

	if(QCSGFp->CurDataID == 0xE0000400)
	{
		QCSGFp->SrcDataPos += 1;
		Result = ERROR_InvalidData;
	}
	else if((QCSGFp->CurDataID >= 0xE0000401) && (QCSGFp->CurDataID <= 0xE0000420))
	{
		FlashAddr = ADDR_RelayTaskPara + (((QCSGFp->CurDataID - 1) & 0x000000FF) * sizeof(RelayTaskParaStruct));
	}
	else
	{
		QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
		Result = ERROR_InvalidData;
	}
	
	if(FlashAddr != 0)
	{
		MR((u32)&RelayTaskPara, FlashAddr, sizeof(RelayTaskParaStruct));
		HAL_GetSystemTime(&RelayTaskPara.ReportTime);
		HAL_GetSystemTime(&RelayTaskPara.SampleTime);
		Result = NO_ERROR;
		
		RelayTaskPara.Flag = pData[Index++];
		if(RelayTaskPara.Flag > 0x01)//err20180404-08 有效性标志参数可设范围 00 01
		{
			Result = ERROR_InvalidData;
		}
		RelayTaskPara.ReportTime.u8Minute = pData[Index++];
		RelayTaskPara.ReportTime.u8Hour   = pData[Index++];
		RelayTaskPara.ReportTime.u8Day    = pData[Index++];
		RelayTaskPara.ReportTime.u8Month  = pData[Index++];
		RelayTaskPara.ReportTime.u8Year   = pData[Index++];
		//RelayTaskPara.ReportTime.u8Week   = 0;
		Result += TimeValidCheck(&RelayTaskPara.ReportTime);
		
		RelayTaskPara.ReportPeriodUnit    = pData[Index++];
		if(RelayTaskPara.ReportPeriodUnit > PERIOD_UNIT_MONTH)
		{
			Result = ERROR_InvalidData;
		}
		RelayTaskPara.ReportPeriod 		  = pData[Index++];

		RelayTaskPara.SampleTime.u8Minute = pData[Index++];
		RelayTaskPara.SampleTime.u8Hour   = pData[Index++];
		RelayTaskPara.SampleTime.u8Day    = pData[Index++];
		RelayTaskPara.SampleTime.u8Month  = pData[Index++];
		RelayTaskPara.SampleTime.u8Year   = pData[Index++];
		//RelayTaskPara.SampleTime.u8Week   = 0;
		Result += TimeValidCheck(&RelayTaskPara.SampleTime);
		
		RelayTaskPara.SamplePeriodUnit 	  = pData[Index++];
		if(RelayTaskPara.SamplePeriodUnit > PERIOD_UNIT_MONTH)
		{
			Result = ERROR_InvalidData;
		}
		RelayTaskPara.SamplePeriod 		  = pData[Index++];
		
		RelayTaskPara.GetRate   = pData[Index++];
		RelayTaskPara.RelayType = pData[Index++];
		RelayTaskPara.ExecCnt = pData[Index++];
		RelayTaskPara.ExecCnt += pData[Index++] << 8;
		RelayTaskPara.RealExeCnt = 0;	
		
		RelayTaskPara.ComPortId = pData[Index++];

		if( RelayTaskPara.RelayType > 0x04 )//err20180404-09 中继类型参数可设范围 00 普通中继； 01 转发主站对电能表的拉闸命令； 02 转发主站对电能表的允许合闸命令； 03 转发主站对电能表的保电投入命令； 04 转发主站对电能表的保电解除命令。
		{
			Result = ERROR_InvalidData;
		}
		if((RelayTaskPara.ComPortId != PORT_ID_RS485_1) && 
			(RelayTaskPara.ComPortId != PORT_ID_RS485_2) &&
			(RelayTaskPara.ComPortId != PORT_ID_RS485_3) &&
			(RelayTaskPara.ComPortId != PORT_ID_PLC) &&
			(RelayTaskPara.ComPortId != PORT_ID_WIRELESS)) //err20180404-09 测量点端口号参数可设范围          00H~1EH 表示 485 1~31；1FH 表示载波通道；20H 表示无线；30H 表示其他
		{
			Result = ERROR_InvalidData;
		}
		RelayTaskPara.Baud = pData[Index++];
		RelayTaskPara.Parity = pData[Index++];
		RelayTaskPara.DataBit = pData[Index++];
		RelayTaskPara.StopBit = pData[Index++];
		if( (RelayTaskPara.Baud != 4) &&
			(RelayTaskPara.Baud != 8) &&
			(RelayTaskPara.Baud != 16) &&
			(RelayTaskPara.Baud != 32))	//err20180404-09 波特率参数可设范围   
		{
			Result = ERROR_InvalidData;
		}
		if(RelayTaskPara.Parity > 2)	//err20180404-09 校验方式参数可设范围 0：无校验； 1：偶校验；2：奇校验；
		{
			Result = ERROR_InvalidData;
		}
		
		if((RelayTaskPara.DataBit > 8) ||
			(RelayTaskPara.DataBit < 5) )	//err20180404-09 数据位参数可设范围 数据位 5,6,7,8
		{
			Result = ERROR_InvalidData;
		}
		if(RelayTaskPara.StopBit > 2)	//err20180404-09 停止位参数可设范围 0-1 位； 1-1.5 位； 2-2 位；
		{
			Result = ERROR_InvalidData;
		}


		
		RelayTaskPara.TimeOut = pData[Index++];
		RelayTaskPara.Length = pData[Index++];
		if(RelayTaskPara.Length != 0)
		{
			MemCopy(RelayTaskPara.Frame, &pData[Index], RelayTaskPara.Length);
		}
		if(QCSGFp->TempPn == 0)
		{
			if(Result == NO_ERROR)
			{
				MW((u32)&RelayTaskPara, FlashAddr, sizeof(RelayTaskParaStruct));
			}
			else
			{
				Result = ERROR_InvalidData;
			}
		}
		else
		{
			Result = ERROR_VerifyFail;
		}		
		Index += RelayTaskPara.Length;
		QCSGFp->SrcDataPos += Index;
	}	
	return Result;
}


ResultEnum QCSG_AFN04_E0000500_E00005FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	AnalogCaculationParaStruct AnalogPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];	
	MemSet((u8*)&AnalogPara, 0, sizeof(AnalogCaculationParaStruct));
	MR((u32)&AnalogPara, ADDR_AnalogPara, sizeof(AnalogCaculationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000500:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000500_Write(pData, &AnalogPara);
			break;
		case 0xE0000501:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E0000501_Write(pData, &AnalogPara);
			break;
		case 0xE0000502:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E0000502_Write(pData, &AnalogPara);
			break;
		case 0xE00005FE:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E00005FE_Write(pData, &AnalogPara);
			break;
		case 0xE00005FF:
			QCSGFp->SrcDataPos += 39;
			Result = QCSG_AFN04_E00005FF_Write(pData, &AnalogPara);
			break;	
		case 0xE0000900:
			QCSGFp->SrcDataPos += 3;
			MemCopy((u8*)&AnalogPara.E0000900, pData, 3);
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
		
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&AnalogPara, ADDR_AnalogPara, sizeof(AnalogCaculationParaStruct));
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}

ResultEnum QCSG_AFN04_E0000500_Write(u8* Data, AnalogCaculationParaStruct* Para)
{	
	if(Data[0] > MAX_ANALOG_NUM) return ERROR_InvalidData;
	Para->AnalogNum = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000501_Write(u8* Data, AnalogCaculationParaStruct* Para)
{
	Para->AnalogID[0][0] = Data[0];
	Para->AnalogID[0][1] = Data[1];
	Para->AnalogID[0][2] = Data[2];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000502_Write(u8* Data, AnalogCaculationParaStruct* Para)
{
	Para->AnalogID[1][0] = Data[0];
	Para->AnalogID[1][1] = Data[1];
	Para->AnalogID[1][2] = Data[2];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E00005FE_Write(u8* Data, AnalogCaculationParaStruct* Para)
{
	MemCopy(Para->AnalogStateMark, Data, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E00005FF_Write(u8* Data, AnalogCaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000500_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000501_Write(Data, Para);
	Data += 3;
	Result += QCSG_AFN04_E0000502_Write(Data, Para);
	Data += 3;
	Result += QCSG_AFN04_E00005FE_Write(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN04_E0000600_E00006FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	PulseCaculationParaStruct PulsePara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];	
	MemSet((u8*)&PulsePara, 0, sizeof(PulseCaculationParaStruct));
	MR((u32)&PulsePara, ADDR_PulsePara, sizeof(PulseCaculationParaStruct));
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000600:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000600_Write(pData, &PulsePara);
			break;
		case 0xE0000601:
			QCSGFp->SrcDataPos += 6;
			Result = QCSG_AFN04_E0000601_Write(pData, &PulsePara);
			break;
		case 0xE0000602:
			QCSGFp->SrcDataPos += 6;
			Result = QCSG_AFN04_E0000602_Write(pData, &PulsePara);
			break;
		case 0xE00006FE:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E00006FE_Write(pData, &PulsePara);
			break;
		case 0xE00006FF:
			QCSGFp->SrcDataPos += 45;
			Result = QCSG_AFN04_E00006FF_Write(pData, &PulsePara);
			break;	
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
		
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&PulsePara, ADDR_PulsePara, sizeof(PulseCaculationParaStruct));
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}

ResultEnum QCSG_AFN04_E0000600_Write(u8* Data, PulseCaculationParaStruct* Para)
{	
	if(Data[0] > MAX_PULSE_NUM) return ERROR_InvalidData;
	Para->PulseNum = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000601_Write(u8* Data, PulseCaculationParaStruct* Para)
{
	u32 Pn = 0;
	Pn = Data[0];
	Pn += Data[1] << 8;
	//Pn = SwitchPnToNum(Pn);
	if(Pn > MAX_PN_NUM)  return ERROR_InvalidData;
	if((Data[2] == 0) || (Data[2] > 0x06)) return ERROR_InvalidData;
	MemCopy(Para->PulseID[0], Data, 6);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000602_Write(u8* Data, PulseCaculationParaStruct* Para)
{
	u32 Pn = 0;
	Pn = Data[0];
	Pn += Data[1] << 8;
	//Pn = SwitchPnToNum(Pn);
	if(Pn > MAX_PN_NUM)  return ERROR_InvalidData;
	if((Data[2] == 0) || (Data[2] > 0x06)) return ERROR_InvalidData;
	MemCopy(Para->PulseID[1], Data, 6);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E00006FE_Write(u8* Data, PulseCaculationParaStruct* Para)
{
	MemCopy(Para->PulseStateMark, Data, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E00006FF_Write(u8* Data, PulseCaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000600_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000601_Write(Data, Para);
	Data += 6;
	Result += QCSG_AFN04_E0000602_Write(Data, Para);
	Data += 6;
	Result += QCSG_AFN04_E00006FE_Write(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN04_E0000700_E00007FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	CaculationParaStruct CaclPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	if(QCSGFp->TempPn == 0)
	{
		MemSet((u8*)&CaclPara, 0, sizeof(CaculationParaStruct));
		MR((u32)&CaclPara, ADDR_CaculationPara, sizeof(CaculationParaStruct));
	}
	
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000700:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000700_Write(pData, &CaclPara);
			break;
		case 0xE0000701:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000701_Write(pData, &CaclPara);
			break;
		case 0xE0000702:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000702_Write(pData, &CaclPara);
			break;
		case 0xE0000703:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000703_Write(pData, &CaclPara);
			break;
		case 0xE0000704:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000704_Write(pData, &CaclPara);
			break;
		case 0xE0000705:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000705_Write(pData, &CaclPara);
			break;
		case 0xE0000706:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000706_Write(pData, &CaclPara);
			break;
		case 0xE0000707:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000707_Write(pData, &CaclPara);
			break;
		case 0xE0000708:
			QCSGFp->SrcDataPos += 30;
			Result = QCSG_AFN04_E0000708_Write(pData, &CaclPara);
			break;
		case 0xE00007FE:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E00007FE_Write(pData, &CaclPara);
			break;
		case 0xE00007FF:
			QCSGFp->SrcDataPos += 273;
			Result = QCSG_AFN04_E00007FF_Write(pData, &CaclPara);
			break;	
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
		
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&CaclPara, ADDR_CaculationPara, sizeof(CaculationParaStruct));
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}

ResultEnum QCSG_AFN04_E0000700_Write(u8* Data, CaculationParaStruct* Para)
{	
	if(Data[0] > MAX_CACU_NUM) return ERROR_InvalidData;
	Para->CaculationNum = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000701_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[0], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000702_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[1], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000703_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[2], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000704_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[3], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000705_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[4], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000706_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[5], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000707_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[6], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000708_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationID[7], Data, 30);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E00007FE_Write(u8* Data, CaculationParaStruct* Para)
{
	MemCopy(Para->CaculationStateMark, Data, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E00007FF_Write(u8* Data, CaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000700_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000701_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000702_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000703_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000704_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000705_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000706_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000707_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E0000708_Write(Data, Para);
	Data += 30;
	Result += QCSG_AFN04_E00007FE_Write(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN04_E0000800_E00008FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	DiffCaculationParaStruct DiffPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];	
	
	if(QCSGFp->TempPn == 0)
	{
		MemSet((u8*)&DiffPara, 0, sizeof(DiffCaculationParaStruct));
		MR((u32)&DiffPara, ADDR_DiffPara, sizeof(DiffCaculationParaStruct));
	}
	
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000800:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000800_Write(pData, &DiffPara);
			break;
		case 0xE0000801://差动规则 第01条
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000801_Write(pData, &DiffPara);
			break;
		case 0xE0000802:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000802_Write(pData, &DiffPara);
			break;
		case 0xE0000803:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000803_Write(pData, &DiffPara);
			break;
		case 0xE0000804:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000804_Write(pData, &DiffPara);
			break;
		case 0xE0000805:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000805_Write(pData, &DiffPara);
			break;
		case 0xE0000806:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000806_Write(pData, &DiffPara);
			break;
		case 0xE0000807:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000807_Write(pData, &DiffPara);
			break;
		case 0xE0000808:
			QCSGFp->SrcDataPos += 15;
			Result = QCSG_AFN04_E0000808_Write(pData, &DiffPara);
			break;
		case 0xE00008FE:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E00008FE_Write(pData, &DiffPara);
			break;
		case 0xE00008FF:
			QCSGFp->SrcDataPos += 153;
			Result = QCSG_AFN04_E00008FF_Write(pData, &DiffPara);
			break;	
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&DiffPara, ADDR_DiffPara, sizeof(DiffCaculationParaStruct));
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}

ResultEnum QCSG_AFN04_E0000800_Write(u8* Data, DiffCaculationParaStruct* Para)
{	
	if(Data[0] > MAX_DIFF_CACU_NUM) return ERROR_InvalidData;
	Para->DiffNum = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000801_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[0], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000802_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[1], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000803_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[2], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000804_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[3], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000805_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[4], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000806_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[5], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000807_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[6], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000808_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffID[7], Data, 15);

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E00008FE_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	MemCopy(Para->DiffStateMark, Data, 32);

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E00008FF_Write(u8* Data, DiffCaculationParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000800_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000801_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000802_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000803_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000804_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000805_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000806_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000807_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E0000808_Write(Data, Para);
	Data += 15;
	Result += QCSG_AFN04_E00008FE_Write(Data, Para);
	Data += 32;

	if(Result != NO_ERROR)
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}



ResultEnum QCSG_AFN04_E0000A00_E0000B02(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 WriteLen = 0;
	u8 DataBuf[48];
	RS485PortParaStruct * pRS485PortPara = (RS485PortParaStruct*)DataBuf;
	AttachmentParaStruct* pAttachmentPara = (AttachmentParaStruct*)DataBuf;	
	MemSet(DataBuf, 0, 48);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000A00://485-1端口参数
		case 0xE0000A01://485-2端口参数
		case 0xE0000A02://485-3端口参数
			QCSGFp->SrcDataPos += 5;
			if(QCSGFp->TempPn == 0)
			{
				if((pData[0] <= UART_FUNC_UPLINK) && (pData[2] <= UART_PARITY_ODD) && 
				   (pData[3] >= UART_DATA_BIT_5) && (pData[3] <= UART_DATA_BIT_8) && (pData[4] <= UART_STOP_BIT_2))
				{
					Result = NAND_ReadPara(PARA_RS485Port, QCSGFp->Pn, QCSGFp->CurDataID, (u8*)pRS485PortPara, &WriteLen);
					MemCopy((u8*)pRS485PortPara, pData, 5);	
					Result = NAND_WritePara(PARA_RS485Port, QCSGFp->Pn, QCSGFp->CurDataID, (u8*)pRS485PortPara, &WriteLen);
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000A20:
			QCSGFp->SrcDataPos += 24;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				MemCopy(pAttachmentPara->SlaveAddr1, pData, 24);
				MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}	
			break;
		case 0xE0000A21:
			QCSGFp->SrcDataPos += 4;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				MemCopy(pAttachmentPara->MasterAddr, pData, 4);
				MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000A22:
			QCSGFp->SrcDataPos += 4;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				MemCopy(pAttachmentPara->AMRSlaveAddr, pData, 4);
				MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000A23:
			QCSGFp->SrcDataPos += 1;
			if(QCSGFp->TempPn == 0)
			{
				if(BCD_Check(pData, 1) == NO_ERROR)
				{
					MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
					pAttachmentPara->MasterTimeOut = pData[0];
					MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000A24:
			QCSGFp->SrcDataPos += 1;
			if(QCSGFp->TempPn == 0)
			{
				if(BCD_Check(pData, 1) == NO_ERROR)
				{
					MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
					pAttachmentPara->SlaveTimeOut = pData[0];
					MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				}
				else
				{
					Result = ERROR_InvalidData;
				}
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000A25:
			QCSGFp->SrcDataPos += 2;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				MemCopy((u8*)&pAttachmentPara->CT, pData, 2);
				MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0000A26:
			QCSGFp->SrcDataPos += 2;
			if(QCSGFp->TempPn == 0)
			{
				MR((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
				MemCopy((u8*)&pAttachmentPara->PT, pData, 2);
				MW((u32)pAttachmentPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;	
		case 0xE0000A30:
			QCSGFp->SrcDataPos += 1;
			Result = ERROR_InvalidData;
			break;
		case 0xE0000A31:
			QCSGFp->SrcDataPos += 4;
			Result = ERROR_InvalidData;
			break;
		case 0xE0000A32:
			QCSGFp->SrcDataPos += 4;
			Result = ERROR_InvalidData;
			break;
		case 0xE0000A33:
			QCSGFp->SrcDataPos += 8;
			Result = ERROR_InvalidData;
			break;
		case 0xE0000B00:
			QCSGFp->SrcDataPos += 2;
			Result = ERROR_InvalidData;
			break;
		case 0xE0000B01:
			QCSGFp->SrcDataPos += 6;
			Result = ERROR_InvalidData;
			break;
		case 0xE0000B02:
			QCSGFp->SrcDataPos += 2;
			Result = ERROR_InvalidData;
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	return Result;
}



ResultEnum QCSG_AFN04_E0000C00_E0000E11(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 i = 0;
	LoadManagementParaStruct LoadPara;
	MemSet((u8*)&LoadPara, 0, sizeof(LoadManagementParaStruct));
	if(QCSGFp->TempPn == 0)
	{
		MR((u32)&LoadPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));
	}
	switch(QCSGFp->CurDataID)
	{
		case 0xE0000C00://　　开关编号 0的开关接点类	型
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
		case 0xE0000C0E://　　开关编号 14的开关接点类	型
			QCSGFp->SrcDataPos += 1;
			LoadPara.SwitchNodeType[QCSGFp->CurDataID & 0x0000000F] = pData[0];
			QCSG_NoticeDIChange(DI_E0000C00, TRUE);
			break;
		case 0xE0000C0F:
			QCSGFp->SrcDataPos += 15;
			MemCopy(LoadPara.SwitchNodeType, pData, 15);
			QCSG_NoticeDIChange(DI_E0000C00, TRUE);
			break;
		case 0xE0000C10:
		case 0xE0000C11:
		case 0xE0000C12:
		case 0xE0000C13:
		case 0xE0000C14:
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
			QCSGFp->SrcDataPos += 1;
			LoadPara.InputSwitchState[QCSGFp->CurDataID & 0x0000000F] = pData[0];	
			Result = ERROR_InvalidData;
			break;
		case 0xE0000C1F:
			QCSGFp->SrcDataPos += 15;
			MemCopy(LoadPara.InputSwitchState, pData, 15);	
			Result = ERROR_InvalidData;
			break;
		case 0xE0000C20:		//输出开关0的状态
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
		case 0xE0000C2E:		//输出开关14的状态
			QCSGFp->SrcDataPos += 1;
			LoadPara.OutputSwitchState[QCSGFp->CurDataID & 0x0000000F] = pData[0];
			QCSG_NoticeDIChange(DI_E0000C2F,TRUE);
			break;
		case 0xE0000C2F:
			QCSGFp->SrcDataPos += 15;
			MemCopy(LoadPara.OutputSwitchState, pData, 15);
			QCSG_NoticeDIChange(DI_E0000C2F,TRUE);
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
			QCSGFp->SrcDataPos += 1;
			LoadPara.OutputSwitchRound[QCSGFp->CurDataID & 0x0000000F] = pData[0];
			break;
		case 0xE0000C3F:
			QCSGFp->SrcDataPos += 15;
			MemCopy(LoadPara.OutputSwitchRound, pData, 15);
			break;
		case 0xE0000C40:
			QCSGFp->SrcDataPos += 2;
			MemCopy((u8*)&LoadPara.RemoteSwitchOffTime, pData, 2);			
			break;
		case 0xE0000C50:
			QCSGFp->SrcDataPos += 1;
			LoadPara.PowerHoldState = pData[0];
			QCSG_NoticeDIChange(DI_E0000C50,TRUE);	
			break;
		case 0xE0000C60://功率控制是否有效
			QCSGFp->SrcDataPos += 1;
			LoadPara.PowerCtrlFlag = pData[0];
			break;
		case 0xE0000C61:
			QCSGFp->SrcDataPos += 1;
			LoadPara.MonthKwhCtrlFlag = pData[0];
			break;
		case 0xE0000C62:
			QCSGFp->SrcDataPos += 1;
			LoadPara.PurchaseKwhCtrlFlag = pData[0];
			break;
		case 0xE0000C63://功率控制各轮次跳闸是否有效
			QCSGFp->SrcDataPos += 1;
			LoadPara.PowerCtrlRoundFlag = pData[0];
			break;
		case 0xE0000C64://月电量控制跳闸轮次有效标志
			QCSGFp->SrcDataPos += 1;
			LoadPara.MonthKwhCtrlRoundFlag = pData[0];
			break;
		case 0xE0000C65:
			QCSGFp->SrcDataPos += 1;
			LoadPara.PurchaseKwhCtrlRoundFlag = pData[0];
			break;
		case 0xE0000C66:	//超过设定值跳闸的过负荷持续时间.缺省 10 分钟
			QCSGFp->SrcDataPos += 3;
			if(BCD_Check(pData, 3) == NO_ERROR)
			{
				MemCopy((u8*)&LoadPara.PowerOverLoadTime, pData, 3);
			}
			else
			{
				Result = ERROR_InvalidData;
			}
			break;
		case 0xE0000C67:
			QCSGFp->SrcDataPos += 3;
			if(BCD_Check(pData, 3) == NO_ERROR)
			{
				MemCopy((u8*)&LoadPara.PowerResetTime, pData, 3);
			}
			else
			{
				Result = ERROR_InvalidData;
			}	
			break;
		case 0xE0000C68:
			QCSGFp->SrcDataPos += 1;
			if(BCD_Check(pData, 1) == NO_ERROR)
			{
				LoadPara.AlarmDelayTime = pData[0];
			}
			else
			{
				Result = ERROR_InvalidData;
			}	
			break;
		case 0xE0000C69://功率控制用功率累加标志
			QCSGFp->SrcDataPos += 17;
			LoadPara.PowerCtrlIncFlag = pData[0];
			MemCopy((u8*)LoadPara.PowerCtrlPoint, &pData[1], 16);
			for(i = 0; i < SIZE_8; i++)
			{
				if(BCD_Check((u8*)&LoadPara.PowerCtrlPoint[i], 2) == NO_ERROR) continue;
				if(LoadPara.PowerCtrlPoint[i] == 0xFFFF) continue;
				break;
			}			
			if(i < SIZE_8)
			{
				Result = ERROR_InvalidData;
			}
			break;
		case 0xE0000C6A://电量控制用电量累加标志
			QCSGFp->SrcDataPos += 17;
			LoadPara.KwhCtrlIncFlag= pData[0];
			MemCopy((u8*)LoadPara.KwhCtrlPoint, &pData[1], 16);
			for(i = 0; i < SIZE_8; i++)
			{
				if(BCD_Check((u8*)&LoadPara.KwhCtrlPoint[i], 2) == NO_ERROR) continue;
				if(LoadPara.KwhCtrlPoint[i] == 0xFFFF) continue;
				break;
			}			
			if(i < SIZE_8)
			{
				Result = ERROR_InvalidData;
			}		
			break;
		case 0xE0000C6B:	//功率定值浮动系数（％）
			QCSGFp->SrcDataPos += 1;
			LoadPara.E0000C6B = pData[0];
			break;
		case 0xE0000C6C:
			QCSGFp->SrcDataPos += 1;
			LoadPara.E0000C6C = pData[0];
			break;
		case 0xE0000C6D:
			QCSGFp->SrcDataPos += 4;
			MemCopy((u8*)&LoadPara.E0000C6D, pData, 4);
			break;
		case 0xE0000C70:
			QCSGFp->SrcDataPos += 4;
			MemCopy((u8*)&LoadPara.E0000C70, pData, 4);
			break;
		case 0xE0000C71:
			QCSGFp->SrcDataPos += 1;
			LoadPara.E0000C71 = pData[0];
			break;
		case 0xE0000D00:	//时段数
			QCSGFp->SrcDataPos += 1;	
			Result = QCSG_AFN04_E0000D00_Write(pData, &LoadPara);
			break;
		case 0xE0000D01:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D01_Write(pData, &LoadPara);
			break;
		case 0xE0000D02:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D02_Write(pData, &LoadPara);
			break;
		case 0xE0000D03:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D03_Write(pData, &LoadPara);
			break;
		case 0xE0000D04:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D04_Write(pData, &LoadPara);
			break;
		case 0xE0000D05:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D05_Write(pData, &LoadPara);
			break;
		case 0xE0000D06:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D06_Write(pData, &LoadPara);
			break;
		case 0xE0000D07:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D07_Write(pData, &LoadPara);
			break;
		case 0xE0000D08:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D08_Write(pData, &LoadPara);
			break;
		case 0xE0000D09:	//　　功率控制系列 1,执行时间
			QCSGFp->SrcDataPos += 9;
			Result = QCSG_AFN04_E0000D09_Write(pData, &LoadPara);
			break;
		case 0xE0000D0F:
			QCSGFp->SrcDataPos += 66;
			Result = QCSG_AFN04_E0000D0F_Write(pData, &LoadPara);
			break;

		case 0xE0000D10:
			QCSGFp->SrcDataPos += 1;	
			Result = QCSG_AFN04_E0000D10_Write(pData, &LoadPara);
			break;
		case 0xE0000D11:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D11_Write(pData, &LoadPara);
			break;
		case 0xE0000D12:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D12_Write(pData, &LoadPara);
			break;
		case 0xE0000D13:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D13_Write(pData, &LoadPara);
			break;
		case 0xE0000D14:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D14_Write(pData, &LoadPara);
			break;
		case 0xE0000D15:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D15_Write(pData, &LoadPara);
			break;
		case 0xE0000D16:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D16_Write(pData, &LoadPara);
			break;
		case 0xE0000D17:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D17_Write(pData, &LoadPara);
			break;
		case 0xE0000D18:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D18_Write(pData, &LoadPara);
			break;
		case 0xE0000D19:
			QCSGFp->SrcDataPos += 9;
			Result = QCSG_AFN04_E0000D19_Write(pData, &LoadPara);						
			break;
		case 0xE0000D1F:
			QCSGFp->SrcDataPos += 66;
			Result = QCSG_AFN04_E0000D1F_Write(pData, &LoadPara);						
			break;

		case 0xE0000D20:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000D20_Write(pData, &LoadPara);
			break;
		case 0xE0000D21:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D21_Write(pData, &LoadPara);
			break;
		case 0xE0000D22:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D22_Write(pData, &LoadPara);
			break;
		case 0xE0000D23:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D23_Write(pData, &LoadPara);
			break;
		case 0xE0000D24:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D24_Write(pData, &LoadPara);
			break;
		case 0xE0000D25:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D25_Write(pData, &LoadPara);
			break;
		case 0xE0000D26:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D26_Write(pData, &LoadPara);
			break;
		case 0xE0000D27:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D27_Write(pData, &LoadPara);
			break;
		case 0xE0000D28:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D28_Write(pData, &LoadPara);
			break;
		case 0xE0000D29:
			QCSGFp->SrcDataPos += 9;			
			Result = QCSG_AFN04_E0000D29_Write(pData, &LoadPara);
			break;
		case 0xE0000D2F:
			QCSGFp->SrcDataPos += 66;			
			Result = QCSG_AFN04_E0000D2F_Write(pData, &LoadPara);
			break;

		case 0xE0000D30:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0000D30_Write(pData, &LoadPara);
			break;
		case 0xE0000D31:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D31_Write(pData, &LoadPara);
			break;
		case 0xE0000D32:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D32_Write(pData, &LoadPara);
			break;
		case 0xE0000D33:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D33_Write(pData, &LoadPara);
			break;
		case 0xE0000D34:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D34_Write(pData, &LoadPara);
			break;
		case 0xE0000D35:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D35_Write(pData, &LoadPara);
			break;
		case 0xE0000D36:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D36_Write(pData, &LoadPara);
			break;
		case 0xE0000D37:
			QCSGFp->SrcDataPos += 7;
			Result = QCSG_AFN04_E0000D37_Write(pData, &LoadPara);
			break;
		case 0xE0000D38:
			QCSGFp->SrcDataPos += 7;			
			Result = QCSG_AFN04_E0000D38_Write(pData, &LoadPara);
			break;
		case 0xE0000D39:
			QCSGFp->SrcDataPos += 9;			
			Result = QCSG_AFN04_E0000D39_Write(pData, &LoadPara);
			break;
		case 0xE0000D3F:
			QCSGFp->SrcDataPos += 66;			
			Result = QCSG_AFN04_E0000D3F_Write(pData, &LoadPara);
			break;
		
		case 0xE0000E00:	//月电量控制，月电量限额（一次值）
			QCSGFp->SrcDataPos += 4;
			if(BCD_Check(pData, 4) == NO_ERROR)
			{			
				MemCopy((u8*)&LoadPara.E0000E00, pData, 4);
			}
			else
			{
				Result = ERROR_InvalidData;
			}
			break;	
		case 0xE0000E01://月电量控制，轮次电量（一次值）
			QCSGFp->SrcDataPos += 4;
			if(BCD_Check(pData, 4) == NO_ERROR)
			{
				MemCopy((u8*)&LoadPara.E0000E01, pData, 4);
			}
			else
			{
				Result = ERROR_InvalidData;
			}
			break;
		case 0xE0000E02:
			QCSGFp->SrcDataPos += 5;
			if(BCD_Check(pData, 5) == NO_ERROR)
			{
				MemCopy(LoadPara.E0000E02, pData, 5);
			}
			else
			{
				Result = ERROR_InvalidData;
			}
			break;
		case 0xE0000E03:
			QCSGFp->SrcDataPos += 5;
			if(BCD_Check(pData, 5) == NO_ERROR)
			{
				MemCopy(LoadPara.E0000E03, pData, 5);
			}
			else
			{
				Result = ERROR_InvalidData;
			}
			break;
		case 0xE0000E04:
			QCSGFp->SrcDataPos += 49;
			if(BCD_Check(pData, 49) == NO_ERROR)
			{
				MemCopy(LoadPara.E0000E04, pData, 49);
			}
			else
			{
				Result = ERROR_InvalidData;
			}	
			break;
		case 0xE0000E05:
			QCSGFp->SrcDataPos += 1;
			LoadPara.E0000E05 = pData[0];			
			break;
		case 0xE0000E10:
			QCSGFp->SrcDataPos += 1;
			LoadPara.E0000E10 = pData[0];
			break;
		case 0xE0000E11:
			QCSGFp->SrcDataPos += 10;
			if(BCD_Check(pData, 10) == NO_ERROR)
			{
				MemCopy(LoadPara.E0000E11, pData, 10);
			}
			else
			{
				Result = ERROR_InvalidData;
			}				
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}
	
	if(QCSGFp->TempPn == 0)
	{
		if(Result == NO_ERROR)
		{
			MW((u32)&LoadPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));
		}
	}
	else
	{
		Result = ERROR_VerifyFail;
	}
	return Result;
}


ResultEnum QCSG_AFN04_E0000D00_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].PowerCtrlPeriodCnt = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D01_Write(u8* Data, LoadManagementParaStruct* Para)
{	
	Para->KwCtrl[0].Option[0].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[0].StartHour     = Data[1];
	Para->KwCtrl[0].Option[0].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[0].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[0].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[0].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[0].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D02_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[1].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[1].StartHour     = Data[1];
	Para->KwCtrl[0].Option[1].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[1].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[1].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[1].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[1].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D03_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[2].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[2].StartHour     = Data[1];
	Para->KwCtrl[0].Option[2].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[2].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[2].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[2].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[2].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D04_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[3].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[3].StartHour     = Data[1];
	Para->KwCtrl[0].Option[3].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[3].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[3].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[3].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[3].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D05_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[4].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[4].StartHour     = Data[1];
	Para->KwCtrl[0].Option[4].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[4].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[4].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[4].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[4].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D06_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[5].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[5].StartHour     = Data[1];
	Para->KwCtrl[0].Option[5].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[5].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[5].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[5].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[5].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D07_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[6].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[6].StartHour     = Data[1];
	Para->KwCtrl[0].Option[6].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[6].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[6].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[6].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[6].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D08_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].Option[7].StartMinute   = Data[0];
	Para->KwCtrl[0].Option[7].StartHour     = Data[1];
	Para->KwCtrl[0].Option[7].EffectiveFlag = Data[2];
	Para->KwCtrl[0].Option[7].PowerLoad     = Data[6];
	Para->KwCtrl[0].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[7].PowerLoad    += Data[5];
	Para->KwCtrl[0].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[7].PowerLoad    += Data[4];
	Para->KwCtrl[0].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[0].Option[7].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D09_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[0].StartDay   = Data[0];
	Para->KwCtrl[0].StartMonth = Data[1];
	Para->KwCtrl[0].EndDay     = Data[2];
	Para->KwCtrl[0].EndMonth   = Data[3];
	Para->KwCtrl[0].N0 = Data[4];
	Para->KwCtrl[0].N1 = Data[5];
	Para->KwCtrl[0].N2 = Data[6];
	Para->KwCtrl[0].N3 = Data[7];
	Para->KwCtrl[0].TI = Data[8];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D0F_Write(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000D00_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000D01_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D02_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D03_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D04_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D05_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D06_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D07_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D08_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D09_Write(Data, Para);
	Data += 9;
	return Result;	
}



ResultEnum QCSG_AFN04_E0000D10_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].PowerCtrlPeriodCnt = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D11_Write(u8* Data, LoadManagementParaStruct* Para)
{	
	Para->KwCtrl[1].Option[0].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[0].StartHour     = Data[1];
	Para->KwCtrl[1].Option[0].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[0].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[0].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[0].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[0].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D12_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[1].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[1].StartHour     = Data[1];
	Para->KwCtrl[1].Option[1].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[1].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[1].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[1].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[1].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D13_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[2].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[2].StartHour     = Data[1];
	Para->KwCtrl[1].Option[2].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[2].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[2].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[2].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[2].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D14_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[3].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[3].StartHour     = Data[1];
	Para->KwCtrl[1].Option[3].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[3].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[3].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[3].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[3].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D15_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[4].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[4].StartHour     = Data[1];
	Para->KwCtrl[1].Option[4].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[4].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[4].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[4].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[4].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D16_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[5].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[5].StartHour     = Data[1];
	Para->KwCtrl[1].Option[5].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[5].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[5].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[5].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[5].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D17_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[6].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[6].StartHour     = Data[1];
	Para->KwCtrl[1].Option[6].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[6].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[6].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[6].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[6].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D18_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].Option[7].StartMinute   = Data[0];
	Para->KwCtrl[1].Option[7].StartHour     = Data[1];
	Para->KwCtrl[1].Option[7].EffectiveFlag = Data[2];
	Para->KwCtrl[1].Option[7].PowerLoad     = Data[6];
	Para->KwCtrl[1].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[7].PowerLoad    += Data[5];
	Para->KwCtrl[1].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[7].PowerLoad    += Data[4];
	Para->KwCtrl[1].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[1].Option[7].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D19_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[1].StartDay   = Data[0];
	Para->KwCtrl[1].StartMonth = Data[1];
	Para->KwCtrl[1].EndDay     = Data[2];
	Para->KwCtrl[1].EndMonth   = Data[3];
	Para->KwCtrl[1].N0 = Data[4];
	Para->KwCtrl[1].N1 = Data[5];
	Para->KwCtrl[1].N2 = Data[6];
	Para->KwCtrl[1].N3 = Data[7];
	Para->KwCtrl[1].TI = Data[8];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D1F_Write(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000D10_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000D11_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D12_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D13_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D14_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D15_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D16_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D17_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D18_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D19_Write(Data, Para);
	Data += 9;
	return Result;	
}


ResultEnum QCSG_AFN04_E0000D20_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].PowerCtrlPeriodCnt = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D21_Write(u8* Data, LoadManagementParaStruct* Para)
{	
	Para->KwCtrl[2].Option[0].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[0].StartHour     = Data[1];
	Para->KwCtrl[2].Option[0].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[0].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[0].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[0].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[0].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D22_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[1].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[1].StartHour     = Data[1];
	Para->KwCtrl[2].Option[1].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[1].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[1].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[1].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[1].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D23_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[2].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[2].StartHour     = Data[1];
	Para->KwCtrl[2].Option[2].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[2].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[2].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[2].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[2].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D24_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[3].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[3].StartHour     = Data[1];
	Para->KwCtrl[2].Option[3].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[3].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[3].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[3].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[3].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D25_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[4].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[4].StartHour     = Data[1];
	Para->KwCtrl[2].Option[4].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[4].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[4].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[4].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[4].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D26_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[5].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[5].StartHour     = Data[1];
	Para->KwCtrl[2].Option[5].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[5].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[5].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[5].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[5].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D27_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[6].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[6].StartHour     = Data[1];
	Para->KwCtrl[2].Option[6].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[6].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[6].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[6].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[6].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D28_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].Option[7].StartMinute   = Data[0];
	Para->KwCtrl[2].Option[7].StartHour     = Data[1];
	Para->KwCtrl[2].Option[7].EffectiveFlag = Data[2];
	Para->KwCtrl[2].Option[7].PowerLoad     = Data[6];
	Para->KwCtrl[2].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[7].PowerLoad    += Data[5];
	Para->KwCtrl[2].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[7].PowerLoad    += Data[4];
	Para->KwCtrl[2].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[2].Option[7].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D29_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[2].StartDay   = Data[0];
	Para->KwCtrl[2].StartMonth = Data[1];
	Para->KwCtrl[2].EndDay     = Data[2];
	Para->KwCtrl[2].EndMonth   = Data[3];
	Para->KwCtrl[2].N0 = Data[4];
	Para->KwCtrl[2].N1 = Data[5];
	Para->KwCtrl[2].N2 = Data[6];
	Para->KwCtrl[2].N3 = Data[7];
	Para->KwCtrl[2].TI = Data[8];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D2F_Write(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000D20_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000D21_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D22_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D23_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D24_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D25_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D26_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D27_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D28_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D29_Write(Data, Para);
	Data += 9;
	return Result;	
}


ResultEnum QCSG_AFN04_E0000D30_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].PowerCtrlPeriodCnt = Data[0];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D31_Write(u8* Data, LoadManagementParaStruct* Para)
{	
	Para->KwCtrl[3].Option[0].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[0].StartHour     = Data[1];
	Para->KwCtrl[3].Option[0].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[0].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[0].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[0].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[0].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[0].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D32_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[1].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[1].StartHour     = Data[1];
	Para->KwCtrl[3].Option[1].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[1].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[1].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[1].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[1].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[1].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D33_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[2].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[2].StartHour     = Data[1];
	Para->KwCtrl[3].Option[2].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[2].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[2].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[2].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[2].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[2].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D34_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[3].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[3].StartHour     = Data[1];
	Para->KwCtrl[3].Option[3].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[3].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[3].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[3].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[3].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[3].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D35_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[4].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[4].StartHour     = Data[1];
	Para->KwCtrl[3].Option[4].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[4].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[4].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[4].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[4].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[4].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D36_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[5].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[5].StartHour     = Data[1];
	Para->KwCtrl[3].Option[5].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[5].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[5].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[5].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[5].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[5].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D37_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[6].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[6].StartHour     = Data[1];
	Para->KwCtrl[3].Option[6].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[6].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[6].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[6].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[6].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[6].PowerLoad    += Data[3];

	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E0000D38_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].Option[7].StartMinute   = Data[0];
	Para->KwCtrl[3].Option[7].StartHour     = Data[1];
	Para->KwCtrl[3].Option[7].EffectiveFlag = Data[2];
	Para->KwCtrl[3].Option[7].PowerLoad     = Data[6];
	Para->KwCtrl[3].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[7].PowerLoad    += Data[5];
	Para->KwCtrl[3].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[7].PowerLoad    += Data[4];
	Para->KwCtrl[3].Option[7].PowerLoad   <<= 8;
	Para->KwCtrl[3].Option[7].PowerLoad    += Data[3];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D39_Write(u8* Data, LoadManagementParaStruct* Para)
{
	Para->KwCtrl[3].StartDay   = Data[0];
	Para->KwCtrl[3].StartMonth = Data[1];
	Para->KwCtrl[3].EndDay     = Data[2];
	Para->KwCtrl[3].EndMonth   = Data[3];
	Para->KwCtrl[3].N0 = Data[4];
	Para->KwCtrl[3].N1 = Data[5];
	Para->KwCtrl[3].N2 = Data[6];
	Para->KwCtrl[3].N3 = Data[7];
	Para->KwCtrl[3].TI = Data[8];

	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0000D3F_Write(u8* Data, LoadManagementParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0000D30_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0000D31_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D32_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D33_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D34_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D35_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D36_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D37_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D38_Write(Data, Para);
	Data += 7;
	Result += QCSG_AFN04_E0000D39_Write(Data, Para);
	Data += 9;
	return Result;	
}



ResultEnum QCSG_AFN04_E0001100_E0002000(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	MeterPointCtrlParaStruct PointCtrlPara;
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 WriteLen = 0;
	
	MC(0, (u32)&PointCtrlPara, sizeof(MeterPointCtrlParaStruct));
	MR((u32)&PointCtrlPara, PARA_MeterPointControl, sizeof(MeterPointCtrlParaStruct));
	
	switch(QCSGFp->CurDataID)
	{
		case 0xE0001100:
			QCSGFp->SrcDataPos += 15;
			WriteLen = 15;
			if(BCD_Check(pData, 15) == NO_ERROR) //err20180404-11 BCD检查
				Result = QCSG_AFN04_E0001100_Write(pData, &PointCtrlPara, QCSGFp, SrcFrame);
			else 
				Result = ERROR_InvalidData;//err20180404-11 BCD检查
			break;
		case 0xE0001101:
			QCSGFp->SrcDataPos += 16;
			WriteLen = 16;
			if(BCD_Check(pData, 16) == NO_ERROR) 	//err20180404-11 BCD检查
				Result = QCSG_AFN04_E0001101_Write(pData, &PointCtrlPara, QCSGFp, SrcFrame);
			else 
				Result = ERROR_InvalidData;//err20180404-11 BCD检查
			break;
		case 0xE0001102:
			QCSGFp->SrcDataPos += 15;
			WriteLen = 15;
			if(BCD_Check(pData, 15) == NO_ERROR) 	//err20180404-11 BCD检查
				Result = QCSG_AFN04_E0001102_Write(pData, &PointCtrlPara, QCSGFp, SrcFrame);
			else 
				Result = ERROR_InvalidData;//err20180404-11 BCD检查
			break;
		case 0xE0001103:
			QCSGFp->SrcDataPos += 15;
			WriteLen = 15;
			if(BCD_Check(pData, 15) == NO_ERROR) 	//err20180404-11 BCD检查
				Result = QCSG_AFN04_E0001103_Write(pData, &PointCtrlPara, QCSGFp, SrcFrame);
			else 
				Result = ERROR_InvalidData;//err20180404-11 BCD检查
			break;
		case 0xE0001104:
			QCSGFp->SrcDataPos += 8;
			if(QCSGFp->TempPn == 0)
			{
				Result = QCSG_AFN04_E0001104_Write(pData, &PointCtrlPara);
			}
			else
			{
				Result = ERROR_VerifyFail;
			}
			break;
		case 0xE0002000:
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}

	if((Result == NO_ERROR) && (WriteLen != 0))
	{
		MW((u32)&PointCtrlPara, PARA_MeterPointControl, sizeof(MeterPointCtrlParaStruct));
	}
	return Result;
}


//遥控拉闸
ResultEnum QCSG_AFN04_E0001100_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame)
{
	MeasurePointStruct Mp;
	u8* PnListData;
	RealTimeStruct RealTime;
	DLT645FrameStruct Frame;

	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))//err20180531 增加II型集中器
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#endif
	
	u32 Temp = 0;
	u32 i = 0;
	u32 ControlType = METER_CONTROL_SWITCH_OFF;//拉闸
	
	Para->E0001100.Addr[0] = Data[0];
	Para->E0001100.Addr[1] = Data[1];
	Para->E0001100.Addr[2] = Data[2];
	Para->E0001100.Addr[3] = Data[3];
	Para->E0001100.Addr[4] = Data[4];
	Para->E0001100.Addr[5] = Data[5];
	Para->E0001100.PassWord[0]  = Data[6];
	Para->E0001100.PassWord[1]  = Data[7];
	Para->E0001100.PassWord[2]  = Data[8];
	Para->E0001100.PassWord[3]  = Data[9];
	Para->E0001100.UserCode[0]  = Data[10];
	Para->E0001100.UserCode[1]  = Data[11];
	Para->E0001100.UserCode[2]  = Data[12];
	Para->E0001100.UserCode[3]  = Data[13];
	Para->E0001100.EffectiveTime = Data[14];

	NAND_ReadMeasurePointByPn(QCSGFp->TempPn, &Mp);
	GenerateMpControlEvent(ControlType, &Mp, QCSGFp->TempPn, Para->E0001100.EffectiveTime);
	
	if(MemCompare(Mp.Addr, Para->E0001100.Addr, METER_ADDR_SIZE) != 0) return ERROR_InvalidData;
	if(Mp.SwitchOff != 0x01)  return ERROR_InvalidData;

	PnListData = (u8*)Get_ADDR_UARTnFnDataBuff(QCSGFp->PORTn);
	PnListData[i++] = QCSGFp->TempPn;
	PnListData[i++] = QCSGFp->TempPn >> 8;
	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = QCSGFp->CurDataID;
	PnListData[i++] = QCSGFp->CurDataID >> 8;
	PnListData[i++] = QCSGFp->CurDataID >> 16;
	PnListData[i++] = QCSGFp->CurDataID >> 24;

	
	//MC(0, (u32)&Frame, sizeof(DLT645FrameStruct));
	MR((u32)Frame.Addr, (u32)Para->E0001100.Addr, METER_ADDR_SIZE);
	Frame.CtrlCode = 0x1C;
	Frame.DataLen = 0;
	
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001100.PassWord, 4);
	Frame.DataLen += 4;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001100.UserCode, 4);
	Frame.DataLen += 4;
	Frame.Data[Frame.DataLen++] = 0x1A;
	Frame.Data[Frame.DataLen++] = 0x00;

	HAL_GetSystemTime(&RealTime);
	for(Temp = 0 ;Temp < Para->E0001100.EffectiveTime ; Temp++)//err20180404-12 当前时间增加控制有效时间为有效截止时间
	{
		QCSG_IncOneMinute(&RealTime);
	}
	MR((u32)&Frame.Data[Frame.DataLen], (u32)&RealTime, 6);
	Frame.DataLen += 6;

	i += Build645Frame(&Frame, &PnListData[i], NO_FE_IN_645);

	i -= 8;
	PnListData[2] = i;
	PnListData[3] = i >> 8;

	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(QCSGFp->PORTn);
		if((RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			RT_AMRReq->SrcTask = GetUartTaskID(QCSGFp->PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_DLT_645;
			RT_AMRReq->DataType  = DATA_TYPE_FRAME;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = QCSGFp->PORTn;
			RT_AMRReq->PnListNum = 1;
			RT_AMRReq->PnListLen = i + 8;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, RT_AMRReq->PnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))//I型集中器 //err20180531 增加II型集中器
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(QCSGFp->PORTn);
		if((UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			UartRxChCtrl->SrcPort = QCSGFp->PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_DLT_645;
			UartRxChCtrl->DataType = DATA_TYPE_FRAME;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = QCSGFp->PORTn;
			UartRxChCtrl->PnListNum  = 1;
			UartRxChCtrl->FrameLen = i + 8;
			UartRxChCtrl->FrameBuf = PnListData;
			switch(QCSGFp->PORTn)
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
					UartRxChCtrl->Empty = 0;
					UartRxChCtrl->PnListNum  = 0;
					UartRxChCtrl->FrameLen = 0;
					UartRxChCtrl->FrameBuf = NULL;
					break;
			}
		}
	#endif
	
	return NO_ERROR;
}

//遥控合闸
ResultEnum QCSG_AFN04_E0001101_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame)
{
	MeasurePointStruct Mp;
	u8* PnListData;
	RealTimeStruct RealTime;
	DLT645FrameStruct Frame;

	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))//err20180531 增加II型集中器
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#endif
	
	u32 Temp = 0;
	u32 i = 0;
	u32 ControlType = METER_CONTROL_SWITCH_ON;//合闸
	
	Para->E0001101.Addr[0] = Data[0];
	Para->E0001101.Addr[1] = Data[1];
	Para->E0001101.Addr[2] = Data[2];
	Para->E0001101.Addr[3] = Data[3];
	Para->E0001101.Addr[4] = Data[4];
	Para->E0001101.Addr[5] = Data[5];
	Para->E0001101.PassWord[0]  = Data[6];
	Para->E0001101.PassWord[1]  = Data[7];
	Para->E0001101.PassWord[2]  = Data[8];
	Para->E0001101.PassWord[3]  = Data[9];
	Para->E0001101.UserCode[0]  = Data[10];
	Para->E0001101.UserCode[1]  = Data[11];
	Para->E0001101.UserCode[2]  = Data[12];
	Para->E0001101.UserCode[3]  = Data[13];
	Para->E0001101.EffectiveTime = Data[14];
	Para->E0001101.SwitchType = Data[15];
	
	NAND_ReadMeasurePointByPn(QCSGFp->TempPn, &Mp);
	GenerateMpControlEvent(ControlType, &Mp, QCSGFp->TempPn, Para->E0001101.EffectiveTime);
	
	if(MemCompare(Mp.Addr, Para->E0001101.Addr, METER_ADDR_SIZE) != 0) return ERROR_InvalidData;
	if(Mp.SwitchOff != 0x01) return ERROR_InvalidData;

	PnListData = (u8*)Get_ADDR_UARTnFnDataBuff(QCSGFp->PORTn);
	PnListData[i++] = QCSGFp->TempPn;
	PnListData[i++] = QCSGFp->TempPn >> 8;
	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = QCSGFp->CurDataID;
	PnListData[i++] = QCSGFp->CurDataID >> 8;
	PnListData[i++] = QCSGFp->CurDataID >> 16;
	PnListData[i++] = QCSGFp->CurDataID >> 24;

	HAL_GetSystemTime(&RealTime);
	//MC(0, (u32)&Frame, sizeof(DLT645FrameStruct));
	MR((u32)Frame.Addr, (u32)Para->E0001101.Addr, METER_ADDR_SIZE);//err20180404-13
	Frame.CtrlCode = 0x1C;
	Frame.DataLen = 0;
	
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001101.PassWord, 4);//err20180404-13
	Frame.DataLen += 4;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001101.UserCode, 4);//err20180404-13
	Frame.DataLen += 4;
#if (TEST_ON == 1) 
	Frame.Data[Frame.DataLen++] = 0x1C; //台体测试按三星发 直接合闸?
#else	
	Frame.Data[Frame.DataLen++] = 0x1B;
#endif
	Frame.Data[Frame.DataLen++] = 0x00;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)&RealTime, 6);
	Frame.DataLen += 6;
	
	HAL_GetSystemTime(&RealTime);
	for(Temp = 0 ;Temp < Para->E0001101.EffectiveTime ; Temp++)//err20180404-12 当前时间增加控制有效时间为有效截止时间
	{
		QCSG_IncOneMinute(&RealTime);
	}


	i += Build645Frame(&Frame, &PnListData[i], NO_FE_IN_645);

	i -= 8;
	PnListData[2] = i;
	PnListData[3] = i >> 8;

	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(QCSGFp->PORTn);
		if((RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			RT_AMRReq->SrcTask = GetUartTaskID(QCSGFp->PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_DLT_645;
			RT_AMRReq->DataType  = DATA_TYPE_FRAME;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = QCSGFp->PORTn;
			RT_AMRReq->PnListNum = 1;
			RT_AMRReq->PnListLen = i + 8;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, RT_AMRReq->PnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))// I 型集中器  //err20180531 增加II型集中器	
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(QCSGFp->PORTn);
		if((UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			UartRxChCtrl->SrcPort = QCSGFp->PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_DLT_645;
			UartRxChCtrl->DataType = DATA_TYPE_FRAME;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = QCSGFp->PORTn;
			UartRxChCtrl->PnListNum  = 1;
			UartRxChCtrl->FrameLen = i + 8;
			UartRxChCtrl->FrameBuf = PnListData;
			switch(QCSGFp->PORTn)
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
					UartRxChCtrl->Empty = 0;
					UartRxChCtrl->PnListNum  = 0;
					UartRxChCtrl->FrameLen = 0;
					UartRxChCtrl->FrameBuf = NULL;
					break;
			}
		}
	#endif
	
	return NO_ERROR;
}

//对电能表保电操作
ResultEnum QCSG_AFN04_E0001102_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame)
{
	MeasurePointStruct Mp;
	u8* PnListData;
	RealTimeStruct RealTime;
	DLT645FrameStruct Frame;

	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))//err20180531 增加II型集中器
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#endif
	u32 i = 0;
	u32 ControlType = METER_CONTROL_POWER_HOLD;
	
	Para->E0001102.Addr[0] = Data[0];
	Para->E0001102.Addr[1] = Data[1];
	Para->E0001102.Addr[2] = Data[2];
	Para->E0001102.Addr[3] = Data[3];
	Para->E0001102.Addr[4] = Data[4];
	Para->E0001102.Addr[5] = Data[5];
	Para->E0001102.PassWord[0]  = Data[6];
	Para->E0001102.PassWord[1]  = Data[7];
	Para->E0001102.PassWord[2]  = Data[8];
	Para->E0001102.PassWord[3]  = Data[9];
	Para->E0001102.UserCode[0]  = Data[10];
	Para->E0001102.UserCode[1]  = Data[11];
	Para->E0001102.UserCode[2]  = Data[12];
	Para->E0001102.UserCode[3]  = Data[13];
	Para->E0001102.EffectiveTime = Data[14];
	
	NAND_ReadMeasurePointByPn(QCSGFp->TempPn, &Mp);
	GenerateMpControlEvent(ControlType, &Mp, QCSGFp->TempPn, Para->E0001102.EffectiveTime);
	
	if(MemCompare(Mp.Addr, Para->E0001102.Addr, METER_ADDR_SIZE) != 0) return ERROR_InvalidData;//err20180404-15 

	PnListData = (u8*)Get_ADDR_UARTnFnDataBuff(QCSGFp->PORTn);
	PnListData[i++] = QCSGFp->TempPn;
	PnListData[i++] = QCSGFp->TempPn >> 8;
	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = QCSGFp->CurDataID;
	PnListData[i++] = QCSGFp->CurDataID >> 8;
	PnListData[i++] = QCSGFp->CurDataID >> 16;
	PnListData[i++] = QCSGFp->CurDataID >> 24;

	HAL_GetSystemTime(&RealTime);
	//MC(0, (u32)&Frame, sizeof(DLT645FrameStruct));
	MR((u32)Frame.Addr, (u32)Para->E0001102.Addr, METER_ADDR_SIZE);//err20180404-15 
	Frame.CtrlCode = 0x1C;
	Frame.DataLen = 0;
	
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001102.PassWord, 4);//err20180404-15 
	Frame.DataLen += 4;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001102.UserCode, 4);//err20180404-15 
	Frame.DataLen += 4;
	Frame.Data[Frame.DataLen++] = 0x3A;
	Frame.Data[Frame.DataLen++] = 0x00;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)&RealTime, 6);
	Frame.DataLen += 6;

	i += Build645Frame(&Frame, &PnListData[i], NO_FE_IN_645);

	i -= 8;
	PnListData[2] = i;
	PnListData[3] = i >> 8;

	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(QCSGFp->PORTn);
		if((RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			RT_AMRReq->SrcTask = GetUartTaskID(QCSGFp->PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_DLT_645;
			RT_AMRReq->DataType  = DATA_TYPE_FRAME;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = QCSGFp->PORTn;
			RT_AMRReq->PnListNum = 1;
			RT_AMRReq->PnListLen = i + 8;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, RT_AMRReq->PnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))// I 型集中器   //err20180531 增加II型集中器	
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(QCSGFp->PORTn);
		if((UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			UartRxChCtrl->SrcPort = QCSGFp->PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_DLT_645;
			UartRxChCtrl->DataType = DATA_TYPE_FRAME;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = QCSGFp->PORTn;
			UartRxChCtrl->PnListNum  = 1;
			UartRxChCtrl->FrameLen = i + 8;
			UartRxChCtrl->FrameBuf = PnListData;
			switch(QCSGFp->PORTn)
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
					UartRxChCtrl->Empty = 0;
					UartRxChCtrl->PnListNum  = 0;
					UartRxChCtrl->FrameLen = 0;
					UartRxChCtrl->FrameBuf = NULL;
					break;
			}
		}
	#endif
	
	return NO_ERROR;
}

//对电能表保电解除操作
ResultEnum QCSG_AFN04_E0001103_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame)
{
	MeasurePointStruct Mp;
	u8* PnListData;
	RealTimeStruct RealTime;
	DLT645FrameStruct Frame;

	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))//err20180531 增加II型集中器
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#endif
	u32 i = 0;
	u32 ControlType = METER_CONTROL_POWER_HOLD_RELEASE;
	
	Para->E0001103.Addr[0] = Data[0];
	Para->E0001103.Addr[1] = Data[1];
	Para->E0001103.Addr[2] = Data[2];
	Para->E0001103.Addr[3] = Data[3];
	Para->E0001103.Addr[4] = Data[4];
	Para->E0001103.Addr[5] = Data[5];
	Para->E0001103.PassWord[0]  = Data[6];
	Para->E0001103.PassWord[1]  = Data[7];
	Para->E0001103.PassWord[2]  = Data[8];
	Para->E0001103.PassWord[3]  = Data[9];
	Para->E0001103.UserCode[0]  = Data[10];
	Para->E0001103.UserCode[1]  = Data[11];
	Para->E0001103.UserCode[2]  = Data[12];
	Para->E0001103.UserCode[3]  = Data[13];
	Para->E0001103.EffectiveTime = Data[14];
	
	NAND_ReadMeasurePointByPn(QCSGFp->TempPn, &Mp);
	GenerateMpControlEvent(ControlType, &Mp, QCSGFp->TempPn, Para->E0001103.EffectiveTime);
	
	if(MemCompare(Mp.Addr, Para->E0001103.Addr, METER_ADDR_SIZE) != 0) return ERROR_InvalidData;//err20180404-15 

	PnListData = (u8*)Get_ADDR_UARTnFnDataBuff(QCSGFp->PORTn);
	PnListData[i++] = QCSGFp->TempPn;
	PnListData[i++] = QCSGFp->TempPn >> 8;
	PnListData[i++] = 0x00;
	PnListData[i++] = 0x00;
	PnListData[i++] = QCSGFp->CurDataID;
	PnListData[i++] = QCSGFp->CurDataID >> 8;
	PnListData[i++] = QCSGFp->CurDataID >> 16;
	PnListData[i++] = QCSGFp->CurDataID >> 24;

	HAL_GetSystemTime(&RealTime);
	//MC(0, (u32)&Frame, sizeof(DLT645FrameStruct));
	MR((u32)Frame.Addr, (u32)Para->E0001103.Addr, METER_ADDR_SIZE);//err20180404-15 
	Frame.CtrlCode = 0x1C;
	Frame.DataLen = 0;
	
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001103.PassWord, 4);//err20180404-15 
	Frame.DataLen += 4;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)Para->E0001103.UserCode, 4);//err20180404-15 
	Frame.DataLen += 4;
	Frame.Data[Frame.DataLen++] = 0x3B;
	Frame.Data[Frame.DataLen++] = 0x00;
	MR((u32)&Frame.Data[Frame.DataLen], (u32)&RealTime, 6);
	Frame.DataLen += 6;

	i += Build645Frame(&Frame, &PnListData[i], NO_FE_IN_645);

	i -= 8;
	PnListData[2] = i;
	PnListData[3] = i >> 8;

	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(QCSGFp->PORTn);
		if((RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			RT_AMRReq->SrcTask = GetUartTaskID(QCSGFp->PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_DLT_645;
			RT_AMRReq->DataType  = DATA_TYPE_FRAME;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = QCSGFp->PORTn;
			RT_AMRReq->PnListNum = 1;
			RT_AMRReq->PnListLen = i + 8;
			MR((u32)RT_AMRReq->Data, (u32)PnListData, RT_AMRReq->PnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
	#endif

	#if (((Project/100)==2) || ((Project/100)==5)||((Project/100)==9))	// I 型集中器   //err20180531 增加II型集中器
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(QCSGFp->PORTn);
		if((UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			UartRxChCtrl->SrcPort = QCSGFp->PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_DLT_645;
			UartRxChCtrl->DataType = DATA_TYPE_FRAME;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = QCSGFp->PORTn;
			UartRxChCtrl->PnListNum  = 1;
			UartRxChCtrl->FrameLen = i + 8;
			UartRxChCtrl->FrameBuf = PnListData;
			switch(QCSGFp->PORTn)
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
					UartRxChCtrl->Empty = 0;
					UartRxChCtrl->PnListNum  = 0;
					UartRxChCtrl->FrameLen = 0;
					UartRxChCtrl->FrameBuf = NULL;
					break;
			}
		}
	#endif
	
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0001104_Write(u8* Data, MeterPointCtrlParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	u32 PnNum = 0, DataLen= 0;
	MeasurePointStruct MeasurePoint;
	MR((u32)Para->E0001104, (u32)Data, 8);
	PnNum = Para->E0001104[0];
	PnNum += Para->E0001104[1] << 8;

	if(BCD_Check(Data, 8) != NO_ERROR) return ERROR_InvalidData;
	PnNum = bcd_hex(PnNum);
	if(PnNum >= MAX_PN_NUM) return ERROR_InvalidData;
	//MC(0, (u32)&MeasurePoint, sizeof(MeasurePointStruct));
	Result = NAND_ReadMeasurePointByPnNum(PnNum, &MeasurePoint);
	if(Result != NO_ERROR) return Result;

	if(MemCompare(&Para->E0001104[2], MeasurePoint.Addr, METER_ADDR_SIZE) != 0)
	{
		return ERROR_DestAddrNotExist;
	}

	MC(0xFF, (u32)&MeasurePoint, sizeof(MeasurePointStruct));
	DataLen = sizeof(MeasurePointStruct);
	PnNum = SwitchNumToPn(PnNum);
	Result = NAND_WriteMeasurePoint(PnNum, 0xE0800000, (u8*)&MeasurePoint, &DataLen);
	
	return Result;	
}

ResultEnum QCSG_AFN04_E0003000_E0003001(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	//err20190403 自动搜表参数 待填充
	return NO_ERROR;
}

//定时自动搜表参数
ResultEnum QCSG_AFN04_E0003000_Write(u8* Data, MeasurePointStruct* Para)
{
	//err20190403 自动搜表参数 待填充
	return NO_ERROR;
}

//手动启动搜表
ResultEnum QCSG_AFN04_E0003001_Write(u8* Data, MeasurePointStruct* Para)
{
	//err20190403 自动搜表参数 待填充
	return NO_ERROR;
}


ResultEnum QCSG_AFN04_E0800000_E080002F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 WriteLen = sizeof(MeasurePointStruct);
	MeasurePointStruct MeasurePoint;
	MemSet((u8*)&MeasurePoint, 0, sizeof(MeasurePointStruct));
	NAND_ReadMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&MeasurePoint, &WriteLen);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0800000:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800000_Write(pData, &MeasurePoint);
			break;
		case 0xE0800001:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800001_Write(pData, &MeasurePoint);			
			break;
		case 0xE0800002:
			QCSGFp->SrcDataPos += 6;
			Result = QCSG_AFN04_E0800002_Write(QCSGFp, pData, &MeasurePoint);
			break;
		case 0xE0800003:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800003_Write(pData, &MeasurePoint);
			break;
		case 0xE0800004:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800004_Write(pData, &MeasurePoint);
			break;
		case 0xE0800005:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800005_Write(pData, &MeasurePoint);
			break;
		case 0xE0800006:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800006_Write(pData, &MeasurePoint);
			break;
		case 0xE0800007:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800007_Write(pData, &MeasurePoint);			
			break;
		case 0xE0800008:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800008_Write(pData, &MeasurePoint);
			break;
		case 0xE0800009:
			QCSGFp->SrcDataPos += 6;
			Result = QCSG_AFN04_E0800009_Write(pData, &MeasurePoint);
			break;
		case 0xE080000A:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E080000A_Write(pData, &MeasurePoint);
			break;
		case 0xE080000B:
			QCSGFp->SrcDataPos += 4;
			Result = QCSG_AFN04_E080000B_Write(pData, &MeasurePoint);
			break;
		case 0xE080000C:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080000C_Write(pData, &MeasurePoint);
			break;
		case 0xE080000D:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080000D_Write(pData, &MeasurePoint);
			break;
		case 0xE080000F:
			QCSGFp->SrcDataPos += 29;
			Result = QCSG_AFN04_E080000F_Write(QCSGFp, pData, &MeasurePoint);
			break;
		case 0xE0800010:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800010_Write(pData, &MeasurePoint);
			break;
		case 0xE0800011:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800011_Write(pData, &MeasurePoint);
			break;
		case 0xE0800012:
			QCSGFp->SrcDataPos += 3;
			Result = QCSG_AFN04_E0800012_Write(pData, &MeasurePoint);
			break;
		case 0xE0800013:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800013_Write(pData, &MeasurePoint);
			break;
		case 0xE0800014:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800014_Write(pData, &MeasurePoint);
			break;
		case 0xE080001F:
			QCSGFp->SrcDataPos += 10;
			Result = QCSG_AFN04_E080001F_Write(pData, &MeasurePoint);
			break;
		case 0xE0800021:
			QCSGFp->SrcDataPos += 8;
			Result = QCSG_AFN04_E0800021_Write(pData, &MeasurePoint);
			break;
		case 0xE0800022:
			QCSGFp->SrcDataPos += 8;
			Result = QCSG_AFN04_E0800022_Write(pData, &MeasurePoint);
			break;
		case 0xE080002F:
			QCSGFp->SrcDataPos += 16;
			Result = QCSG_AFN04_E080002F_Write(pData, &MeasurePoint);
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}

	if((Result == NO_ERROR) && (WriteLen != 0))
	{
		if(MeasurePoint.PortBaudRate == 0xFF) MeasurePoint.PortBaudRate = BAUD_RATE_1200/300;
		if(MeasurePoint.PortParity == 0xFF) MeasurePoint.PortParity = 0x01;
		if(MeasurePoint.PortDataBit == 0xFF) MeasurePoint.PortDataBit = 0x08;
		if(MeasurePoint.PortStopBit == 0xFF) MeasurePoint.PortStopBit = 0x00;
		
		Result = NAND_WriteMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&MeasurePoint, &WriteLen);
	}
	return Result;
}


ResultEnum QCSG_AFN04_E0800000_Write(u8* Data, MeasurePointStruct* Para)
{
	if(Data[0] > 0x01) return ERROR_InvalidData;
	Para->State = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800001_Write(u8* Data, MeasurePointStruct* Para)
{
	if((Data[0] == 0) || (Data[0] > 0x05)) return ERROR_InvalidData;
	Para->MeasureType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800002_Write(QCSGFrameProcessStruct* QCSGFp, u8* Data, MeasurePointStruct* Para)
{
	if(MeterAddrCheck(Data) != NO_ERROR) return ERROR_InvalidData;

	if(QCSG_CheckPnAddrIsDuplicate(QCSGFp->TempPn, Data) != NO_ERROR) return ERROR_InvalidData;
			
	Para->Addr[0] = Data[0];
	Para->Addr[1] = Data[1];
	Para->Addr[2] = Data[2];
	Para->Addr[3] = Data[3];
	Para->Addr[4] = Data[4];
	Para->Addr[5] = Data[5];
	
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800003_Write(u8* Data, MeasurePointStruct* Para)
{
	if(Data[0] > 0x0B) return ERROR_InvalidData;
	Para->ProtocolType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800004_Write(u8* Data, MeasurePointStruct* Para)
{
	if((Data[0] == 0) || (Data[0] > 0x03)) return ERROR_InvalidData;
	Para->PointMeterType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800005_Write(u8* Data, MeasurePointStruct* Para)
{
	if(Data[0] > 0x01) return ERROR_InvalidData;
	Para->TotalType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800006_Write(u8* Data, MeasurePointStruct* Para)
{
	if(Data[0] > 0x01) return ERROR_InvalidData;
	Para->PriorityType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800007_Write(u8* Data, MeasurePointStruct* Para)
{
	if(Data[0] > 0x01) return ERROR_InvalidData;
	Para->SwitchOff = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800008_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->MaxRateNum = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800009_Write(u8* Data, MeasurePointStruct* Para)
{
	if(MeterAddrCheck(Data) != NO_ERROR)
	{
		if(MemCompare(Data, EmptyAddr, METER_ADDR_SIZE) != 0) return ERROR_InvalidData;
	}
	Para->CollectorAddr[0] = Data[0];
	Para->CollectorAddr[1] = Data[1];
	Para->CollectorAddr[2] = Data[2];
	Para->CollectorAddr[3] = Data[3];
	Para->CollectorAddr[4] = Data[4];
	Para->CollectorAddr[5] = Data[5];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080000A_Write(u8* Data, MeasurePointStruct* Para)
{
	if(((Data[0] > 0x20) && (Data[0] < 0x30)) || (Data[0] > 0x30)) return ERROR_InvalidData;
	Para->PortID = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080000B_Write(u8* Data, MeasurePointStruct* Para)
{

//err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。
#if(TEST_ON == 1)
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
#endif

	if(Data[1] > 0x02) return ERROR_InvalidData;
	if((Data[2] < 0x05) || (Data[2] > 0x08)) return ERROR_InvalidData;
	if(Data[3] > 0x02) return ERROR_InvalidData;
	
	//err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。
#if(TEST_ON == 1)
	
	if(Para->PortBaudRate != Data[0])
	{
		if((Para->State == 0x01) && (BCD_Check(Para->Addr, METER_ADDR_SIZE) == NO_ERROR) && ((Para->PortID == 0x20) || (Para->PortID == 0x1F)))
		{
			MC(0,(u32)Pn_Lcd_Data,sizeof(QCSG_LCD_RAMDATA_S));
			Pn_Lcd_Data->u16Pn = 9;
			Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ_TEST_START;	
			pTerminalRam->LCD_RT_AMR_REQ_TEST_START_TIME = pTerminalRam->Seconds; 
		}
	}

	
#endif
	Para->PortBaudRate = Data[0];
	Para->PortParity   = Data[1];
	Para->PortDataBit  = Data[2];
	Para->PortStopBit  = Data[3];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080000C_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->CT = Data[1];
	Para->CT <<= 8;
	Para->CT += Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080000D_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->PT = Data[1];
	Para->PT <<= 8;
	Para->PT += Data[0];
	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E080000F_Write(QCSGFrameProcessStruct* QCSGFp, u8* Data, MeasurePointStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0800000_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800001_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800002_Write(QCSGFp, Data, Para);
	Data += 6;
	Result += QCSG_AFN04_E0800003_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800004_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800005_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800006_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800007_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800008_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800009_Write(Data, Para);
	Data += 6;
	Result += QCSG_AFN04_E080000A_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E080000B_Write(Data, Para);
	Data += 4;
	Result += QCSG_AFN04_E080000C_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E080000D_Write(Data, Para);
	Data += 2;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}



ResultEnum QCSG_AFN04_E0800010_Write(u8* Data, MeasurePointStruct* Para)
{
	if((Data[0] != 0x01) && (Data[0] != 0x03) && (Data[0] != 0x04)) return ERROR_InvalidData;
	Para->ConnectionType = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800011_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->RatedVoltage = Data[1];
	Para->RatedVoltage <<= 8;
	Para->RatedVoltage += Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800012_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->RatedCurrent = Data[2];
	Para->RatedCurrent <<= 8;
	Para->RatedCurrent += Data[1];
	Para->RatedCurrent <<= 8;
	Para->RatedCurrent += Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800013_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->UserDeclareCapacity = Data[1];
	Para->UserDeclareCapacity <<= 8;
	Para->UserDeclareCapacity += Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800014_Write(u8* Data, MeasurePointStruct* Para)
{
	Para->TransformerCapacity = Data[1];
	Para->TransformerCapacity <<= 8;
	Para->TransformerCapacity += Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080001F_Write(u8* Data, MeasurePointStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0800010_Write(Data, Para);
	Data += 1;
	Result += QCSG_AFN04_E0800011_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800012_Write(Data, Para);
	Data += 3;
	Result += QCSG_AFN04_E0800013_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800014_Write(Data, Para);
	Data += 2;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;		
}


ResultEnum QCSG_AFN04_E0800021_Write(u8* Data, MeasurePointStruct* Para)
{
	MemCopy(Para->PassWord, Data, 8);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800022_Write(u8* Data, MeasurePointStruct* Para)
{
	MemCopy(Para->UserName, Data, 8);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080002F_Write(u8* Data, MeasurePointStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0800021_Write(Data, Para);
	Data += 8;
	Result += QCSG_AFN04_E0800022_Write(Data, Para);
	Data += 8;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN04_E0800100_E080020E(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	u32 WriteLen = sizeof(PowerQualityParaStruct);
	PowerQualityParaStruct PowerQuality;
	MemSet((u8*)&PowerQuality, 0, sizeof(PowerQualityParaStruct));
	NAND_ReadMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&PowerQuality, &WriteLen);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0800100:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800100_Write(pData, &PowerQuality);
			break;
		case 0xE0800101:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800101_Write(pData, &PowerQuality);			
			break;
		case 0xE0800102:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800102_Write(pData, &PowerQuality);
			break;
		case 0xE0800103:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800103_Write(pData, &PowerQuality);
			break;
		case 0xE0800104:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800104_Write(pData, &PowerQuality);
			break;
		case 0xE0800105:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800105_Write(pData, &PowerQuality);
			break;
		case 0xE0800106:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800106_Write(pData, &PowerQuality);
			break;
		case 0xE0800107:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800107_Write(pData, &PowerQuality);			
			break;
		case 0xE0800108:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800108_Write(pData, &PowerQuality);
			break;
		case 0xE0800109:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800109_Write(pData, &PowerQuality);
			break;
		case 0xE080010A:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080010A_Write(pData, &PowerQuality);
			break;
		case 0xE080010B:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080010B_Write(pData, &PowerQuality);
			break;
		case 0xE080010C:
			QCSGFp->SrcDataPos += 8;
			Result = QCSG_AFN04_E080010C_Write(pData, &PowerQuality);
			break;
		case 0xE080010F:
			QCSGFp->SrcDataPos += 32;
			Result = QCSG_AFN04_E080010F_Write(pData, &PowerQuality);
			break;
		case 0xE0800200:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800200_Write(pData, &PowerQuality);
			break;
		case 0xE0800201:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800201_Write(pData, &PowerQuality);			
			break;
		case 0xE0800202:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800202_Write(pData, &PowerQuality);
			break;
		case 0xE0800203:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800203_Write(pData, &PowerQuality);
			break;
		case 0xE0800205:
			QCSGFp->SrcDataPos += 18;
			Result = QCSG_AFN04_E0800205_Write(pData, &PowerQuality);
			break;
		case 0xE0800206:
			QCSGFp->SrcDataPos += 1;
			Result = QCSG_AFN04_E0800206_Write(pData, &PowerQuality);
			break;
		case 0xE0800207:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800207_Write(pData, &PowerQuality);			
			break;
		case 0xE0800208:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800208_Write(pData, &PowerQuality);
			break;
		case 0xE0800209:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800209_Write(pData, &PowerQuality);
			break;
		case 0xE080020A:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080020A_Write(pData, &PowerQuality);
			break;
		case 0xE080020B:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080020B_Write(pData, &PowerQuality);
			break;
		case 0xE080020C:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080020C_Write(pData, &PowerQuality);
			break;
		case 0xE080020D:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080020D_Write(pData, &PowerQuality);
			break;
		case 0xE080020E:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E080020E_Write(pData, &PowerQuality);
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}

	if((Result == NO_ERROR) && (WriteLen != 0))
	{
		Result = NAND_WriteMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&PowerQuality, &WriteLen);
	}
	return Result;
}


ResultEnum QCSG_AFN04_E0800100_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800100, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800101_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800101, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800102_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800102, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800103_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800103, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800104_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800104, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800105_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800105, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800106_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800106, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800107_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800107, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800108_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800108, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800109_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800109, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080010A_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080010A, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080010B_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080010B, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080010C_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080010C, Data, 8);
	return NO_ERROR;	
}


ResultEnum QCSG_AFN04_E080010F_Write(u8* Data, PowerQualityParaStruct* Para)
{
	ResultEnum Result = NO_ERROR;
	Result += QCSG_AFN04_E0800100_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800101_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800102_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800103_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800104_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800105_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800106_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800107_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800108_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E0800109_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E080010A_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E080010B_Write(Data, Para);
	Data += 2;
	Result += QCSG_AFN04_E080010C_Write(Data, Para);
	Data += 8;

	if(Result != NO_ERROR) 
	{
		Result = ERROR_InvalidData;
	}
	return Result;	
}


ResultEnum QCSG_AFN04_E0800200_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800200, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800201_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800201, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800202_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800202, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800203_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800203, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800205_Write(u8* Data, PowerQualityParaStruct* Para)
{
	if(BCD_Check(Data, 18) != NO_ERROR) return ERROR_InvalidData;
	MemCopy((u8*)&Para->E0800205, Data, 18);
//	if(Para->E0800205[0] >= 0x1000) return ERROR_InvalidData;
//	if(Para->E0800205[0] >= Para->E0800205[1]) return ERROR_InvalidData;
//	if(Para->E0800205[1] >= Para->E0800205[2]) return ERROR_InvalidData;
//	if(Para->E0800205[2] >= Para->E0800205[3]) return ERROR_InvalidData;
//	if(Para->E0800205[3] >= Para->E0800205[4]) return ERROR_InvalidData;
//	if(Para->E0800205[4] >= Para->E0800205[5]) return ERROR_InvalidData;
//	if(Para->E0800205[5] >= Para->E0800205[6]) return ERROR_InvalidData;
//	if(Para->E0800205[6] >= Para->E0800205[7]) return ERROR_InvalidData;
//	if(Para->E0800205[7] >= Para->E0800205[8]) return ERROR_InvalidData;
//	if(Para->E0800205[8] > 0x1000) return ERROR_InvalidData;
//	MemCopy((u8*)&Para->E0800205, Data, 18);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800206_Write(u8* Data, PowerQualityParaStruct* Para)
{
	Para->E0800206 = Data[0];
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800207_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800207, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800208_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800208, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E0800209_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E0800209, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080020A_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080020A, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080020B_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080020B, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080020C_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080020C, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080020D_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080020D, Data, 2);
	return NO_ERROR;	
}

ResultEnum QCSG_AFN04_E080020E_Write(u8* Data, PowerQualityParaStruct* Para)
{
	MemCopy((u8*)&Para->E080020E, Data, 2);
	return NO_ERROR;	
}
/*
//err20190403 ybwhz 自定义
ResultEnum QCSG_AFN04_E0AAAA00_E0AAAA05(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp)
{
	ResultEnum Result = NO_ERROR;
	u8* pData = &SrcFrame->Data[QCSGFp->SrcDataPos];
	//u32 WriteLen = sizeof(PowerQualityParaStruct);
	//PowerQualityParaStruct PowerQuality;
	//MemSet((u8*)&PowerQuality, 0, sizeof(PowerQualityParaStruct));
	//NAND_ReadMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&PowerQuality, &WriteLen);
	switch(QCSGFp->CurDataID)
	{
		case 0xE0AAAA00:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0AAAA00_Write(pData);
			break;
		case 0xE0AAAA01:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800101_Write(pData, &PowerQuality);			
			break;
		case 0xE0AAAA02:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800102_Write(pData, &PowerQuality);
			break;
		case 0xE0AAAA03:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800103_Write(pData, &PowerQuality);
			break;
		case 0xE0AAAA04:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800104_Write(pData, &PowerQuality);
			break;
		case 0xE0AAAA05:
			QCSGFp->SrcDataPos += 2;
			Result = QCSG_AFN04_E0800105_Write(pData, &PowerQuality);
			break;
		default:
			QCSGFp->SrcDataPos = QCSGFp->UserDataLen;
			Result = ERROR_NoSuchDataID;
			break;			
	}

	//if((Result == NO_ERROR) && (WriteLen != 0))
	//{
	//	Result = NAND_WriteMeasurePoint(QCSGFp->TempPn, QCSGFp->CurDataID, (u8*)&PowerQuality, &WriteLen);
	//}
	return Result;
}

ResultEnum QCSG_AFN04_E0AAAA00_Write(u8* Data)
{
	
}
*/


ResultEnum QCSG_Check_AFN04_DataID(u32 DataID, u16* Index)
{
    u32 i = 0; //, u16FnNum = 0;

    if((DataID & 0xFFFFFF00) == 0xE0000300)
    {
        //任务参数表格做了特殊处理
        DataID = 0xE00003FF;
    }
    if((DataID & 0xFFFFFF00) == 0xE0000400)
    {
        //任务参数表格做了特殊处理
        DataID = 0xE00004FF;
    }
    for(i = 0; i < MAX_AFN040A_IDENT_NUM; i++)
    {
        if(DataID == gc_stQCSGResolve_AFN04[i].u32Fn) break;
    }
    if(i >= MAX_AFN040A_IDENT_NUM)
    {
        return ERROR_NoSuchDataID;
    }
    *Index = i;
    return NO_ERROR;
}



ResultEnum CheckANF04Pn(u32 Pn)
{
	u32 i = 0, Bit1Cnt = 0;
	for(i = 0; i < 8; i++)
	{
		if((Pn & (1 << i)) != 0) Bit1Cnt++;
	}
	if(Bit1Cnt > 1) return ERROR_WRONG_ACCESS;
	return NO_ERROR;
}



/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


