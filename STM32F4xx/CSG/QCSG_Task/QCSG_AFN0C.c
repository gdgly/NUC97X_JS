/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0C.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN0C处理文件
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
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "Terminal_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/QCSG_AFN0C.h"
/* Private typedef -----------------------------------------------------------*/
ResultEnum GetTerminalSpecData(u32 DI, u8* Data, u32 MaxLen, u32* Len);

u16 FindTerminalSpecDataIndex(const QCSG_DITableStruct* DITable, u32 DI, u32 Size);

//u16 GetMaxPnNum(void);

//u16 SplitPnToList(u16 Pn, u16* List, u8 PnCheck);

/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#define TERMINAL_SPEC_DI_NUM				(19)
#define TERMINAL_SPEC_DATA_MAX_LEN			(40)
const QCSG_DITableStruct TerminalSpecDataTbl[TERMINAL_SPEC_DI_NUM] = 
{
	{0xE1800001, ADDR_E1800001, 2, R_ENABLE, 0},
	{0xE1800002, ADDR_E1800002, 1, R_ENABLE, 0},
	{0xE1800003, ADDR_E1800003, 1, R_ENABLE, 0},
	{0xE1800004, ADDR_E1800004, 2, R_ENABLE, 0},

	{0xE1800005, ADDR_E1800005, 2, R_ENABLE, 0},
	{0xE1800006, ADDR_E1800006, 2, R_ENABLE, 0},
	{0xE1800007, ADDR_E1800007, 2, R_ENABLE, 0},
	{0xE1800008, ADDR_E1800008, 2, R_ENABLE, 0},

	{0xE1800009, ADDR_E1800009, 16, R_ENABLE, 0},
	{0xE1800010, ADDR_E1800010, 3, R_ENABLE, 0},
	{0xE1800011, ADDR_E1800011, 2, R_ENABLE, 0},
	{0xE1800012, ADDR_E1800012, 2, R_ENABLE, 0},

	{0xE1800013, ADDR_E1800013, 3, R_ENABLE, 0},
	{0xE1800014, ADDR_E1800014, 3, R_ENABLE, 0},
	{0xE1800015, ADDR_E1800015, 1, R_ENABLE, 0},
	{0xE1800016, ADDR_E1800016, 2, R_ENABLE, 0},

	{0xE1800017, ADDR_E1800017, 2, R_ENABLE, 0},
	{0xE1800018, ADDR_E1800018, 9, R_ENABLE, 0},
	{0xE1800020, ADDR_E1800020, 40, R_ENABLE, 0}
	
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/*****************************************************************************************************
功     能: 	南网上行协议写参数AFN = 0C操作
入口参 数: 	PORTn: 帧源端口号
			SrcFrame: 输入的源数据帧指针
			DstFrame: 输出的应答数据帧指针
返     回: 返回操作是否成功
编     写: zhaoyun
编写时 间; 2017-7-10
*****************************************************************************************************/
ResultEnum QCSG_AFN0C_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	QCSG_PnListStruct* QCSG_PnList;
	#if ((Project/100)==5)	//err20181122 增加当前数据 日/月停电次数及时间
		u32 Temp = 0;
	#endif
	
/*#if (TEST_ON == 1)//err20180713 台体不停发00010000 直接回复抄表值
		AlarmAMRDataStruct* AlarmAMRDataNew = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
		AlarmAMRDataStruct* AlarmAMRDataOld = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_Old;
#endif*/


	
	#if ((Project/100)==3)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
		u32 u32ReadLen = 0;
		u8 u8HwPort = 0;
		MeasurePointStruct MeasurePointStruct_Temp;
		UARTCtrl_TypeDef * FK_UARTCtrl;//err20190226 增加485-II,485-III抄表口判断 
	#endif

	#if ((Project/100)==2)||((Project/100)==5)	
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#endif

	#if ((Project/100)==9)
		QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq;
		UartRxChCtrlStruct* UartRxChCtrl;
		RT_AMRCtrlStruct* RT_AMRTask = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
		u8 u8Flag_JC=0, u8Flag_Pn=0;//交采与其他测量点的标志
		QCSG_PnListStruct QCSG_PnList_JC[PN_LIST_MAX_NUM_JC];
		QCSG_PnListStruct QCSG_PnList_PN[SIZE_256];
		u16 TotalPnListNum_JC = 0;
		u16 TotalPnListNum_PN = 0;
		MeasurePointStruct  MeasurePoint;
    	u32 offset=0,flashLen=0;
		u32 PnNum = 0;
	#endif
	
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0;
	u16 i = 0, TotalPnListNum = 0, TmpPnListNum = 0, LastPn = QCSG_INVALID_PN, TotalPnListLen = 0;
	u8* PnData = (u8*)(ADDR_DATABUFF + SIZE_2K);
	u16* TmpPnList = (u16*)(ADDR_PnListBuf);	
	
	MC(0, (u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	QCSGFp.PORTn = PORTn;
	QCSG_PnList = (QCSG_PnListStruct*)Get_ADDR_UARTnFnDataBuff(PORTn);
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

		if((QCSGFp.Pn == 0) && (QCSGFp.CurDataID >= 0xE1800001) && (QCSGFp.CurDataID <= 0xE1800020))
		{
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 16;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 24;
			DataLen = 0;
			Result = GetTerminalSpecData(QCSGFp.CurDataID, PnData, TERMINAL_SPEC_DATA_MAX_LEN, &DataLen);
			if(Result != NO_ERROR)
			{
				QCSGFp.DstDataPos -= 6;
			}
			else
			{
				MR((u32)&DstFrame->Data[QCSGFp.DstDataPos], (u32)PnData, DataLen);
				QCSGFp.DstDataPos += (u16)DataLen;
			}
			continue;
		}
		
	#if ((Project/100)==5)	//err20181122 增加当前数据 日/月停电次数及时间
		if((QCSGFp.Pn == 0) && 
			((QCSGFp.CurDataID == 0xE1000020) || 
			(QCSGFp.CurDataID == 0xE1000021) ||
			(QCSGFp.CurDataID == 0xE1004020) || 
			(QCSGFp.CurDataID == 0xE1004021)))
		{
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 16;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 24;
			switch(QCSGFp.CurDataID)
			{
				case 0xE1000020:
					MR((u32)&Temp, QCSG_POWER_DOWN_NUM_DAY, 4);				
					break;
				case 0xE1000021:
					MR((u32)&Temp, QCSG_POWER_DOWN_TIME_DAY, 4);
					break;	
				case 0xE1004020:
					MR((u32)&Temp, QCSG_POWER_DOWN_NUM_MONTH, 4);				
					break;
				case 0xE1004021:
					MR((u32)&Temp, QCSG_POWER_DOWN_TIME_MONTH, 4);
					break;	
				default:
					break;
			}
			Temp = hex_bcd(Temp);//err20181211 召读返回BCD
			DstFrame->Data[QCSGFp.DstDataPos++] = Temp;
			DstFrame->Data[QCSGFp.DstDataPos++] = Temp >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = Temp >> 16;
			continue;
		}
	#endif
		
#if ((Project/100)==3)//err20180806 掉电后不抄实时数据
#if (TEST_ON == 1)
	if(Comm_Ram->POWER != 3) continue;
#endif
#endif

//err20180713 台体不停发00010000 直接回复抄表值 9号测量点
/*//err20190225 修改载波周期抄表流程，实时抄表将不会打断周期抄表，故删除下方代码。
#if (TEST_ON == 1)

		if((QCSGFp.Pn == 0x0201) && (QCSGFp.CurDataID == 0x00010000))
		{
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.Pn >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 8;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 16;
			DstFrame->Data[QCSGFp.DstDataPos++] = QCSGFp.CurDataID >> 24;
			DataLen = 4;//4字节

			if(AlarmAMRDataOld[9].State[0] == 0x01)//已经至少执行过一次周期抄表
			{//使用周期抄表值
				MR((u32)&DstFrame->Data[QCSGFp.DstDataPos], (u32)AlarmAMRDataNew[9].DI_00010000, DataLen);
				QCSGFp.DstDataPos+= (u16)DataLen;
			}
			else 
			{//否则使用固定值
				DstFrame->Data[QCSGFp.DstDataPos++] = 0x77;
				DstFrame->Data[QCSGFp.DstDataPos++] = 0x77;
				DstFrame->Data[QCSGFp.DstDataPos++] = 0x77;
				DstFrame->Data[QCSGFp.DstDataPos++] = 0x77;
			}
			continue;
		}
#endif	
*/
//err20180713 台体不停发00010000


		if(LastPn != QCSGFp.Pn)
		{
			TmpPnListNum = SplitPnToList(QCSGFp.Pn, TmpPnList, PN_CHECK_ENABLE);
			LastPn = QCSGFp.Pn;
		}	
		i = 0;
		while(i < TmpPnListNum)
		{
			#if ((Project/100)==3)
				u32ReadLen = sizeof(MeasurePointStruct);
				if(NAND_ReadMeasurePoint(TmpPnList[i],0xE0800002,(u8*)&MeasurePointStruct_Temp,&u32ReadLen) != NO_ERROR)
				{
					i++;
					continue;
				}
				if(TmpPnList[i] != 0)
				{
					if(QCSG_PortToHwPort(MeasurePointStruct_Temp.PortID,&u8HwPort) != TRUE)
					{
						i++;
						continue;
					}
					FK_UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(u8HwPort);
					if(FK_UARTCtrl->Function == 0)//err20190619如果是维护口就跳过
					{
						i++;
						continue;
					}
				}	
			#endif
			QCSG_PnList[TotalPnListNum].Pn = TmpPnList[i];
			QCSG_PnList[TotalPnListNum].Len = 0;
			QCSG_PnList[TotalPnListNum].DI = QCSGFp.CurDataID;
			i++;
			TotalPnListNum++;
			if(TotalPnListNum >= PN_LIST_MAX_NUM) break;
		}	
		if(TotalPnListNum >= PN_LIST_MAX_NUM) break;				
	}

	#if ((Project/100)==3)		// 负荷管理终端
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(PORTn);
		if((TotalPnListNum > 0) && (RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			TotalPnListLen = TotalPnListNum * sizeof(QCSG_PnListStruct);
			RT_AMRReq->SrcTask = GetUartTaskID(PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_QCSG_UP;
			RT_AMRReq->DataType  = DATA_TYPE_STRUCT;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = PORTn;
			RT_AMRReq->PnListNum = TotalPnListNum;
			RT_AMRReq->PnListLen = TotalPnListLen;
			MR((u32)RT_AMRReq->Data, (u32)QCSG_PnList, TotalPnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
		else
		{
			TotalPnListNum = 0;
		}
	#endif

	#if ((Project/100)==2)||((Project/100)==5)		// I 型集中器
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
		if((TotalPnListNum > 0) && (UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			TotalPnListLen = TotalPnListNum * sizeof(QCSG_PnListStruct);
			UartRxChCtrl->SrcPort = PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_QCSG_UP;
			UartRxChCtrl->DataType = DATA_TYPE_STRUCT;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = PORTn;
			UartRxChCtrl->PnListNum  = TotalPnListNum;
			UartRxChCtrl->FrameLen = TotalPnListLen;
			UartRxChCtrl->FrameBuf = (u8*)QCSG_PnList;
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
	#endif

	#if ((Project/100)==9)		// 配变

 
		for(i=0;i < TotalPnListNum ;i++)
		{
			PnNum = SwitchPnToNum(QCSG_PnList[i].Pn);
			offset = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
	        flashLen= sizeof(MeasurePointStruct) ;
	        MR((u32)&MeasurePoint, offset, flashLen);
			if ((QCSG_PnList[i].Pn == 0)) //|| (MeasurePoint.TotalType == 1))//交采测量点或者总表传递至负控代码抄表
			{
				if(TotalPnListNum_JC >= PN_LIST_MAX_NUM_JC)
					continue;
				QCSG_PnList_JC[TotalPnListNum_JC].Pn = QCSG_PnList[i].Pn;
				QCSG_PnList_JC[TotalPnListNum_JC].Len = 0;
				QCSG_PnList_JC[TotalPnListNum_JC].DI = QCSG_PnList[i].DI;
				TotalPnListNum_JC++;
			}
			else //集中器用其他测量点
			{
				if(TotalPnListNum_PN >= PN_LIST_MAX_NUM)
					continue;
				QCSG_PnList_PN[TotalPnListNum_PN].Pn = QCSG_PnList[i].Pn;
				QCSG_PnList_PN[TotalPnListNum_PN].Len = 0;
				QCSG_PnList_PN[TotalPnListNum_PN].DI = QCSG_PnList[i].DI;
				TotalPnListNum_PN++;
			}
		}	
		RT_AMRReq = (QCSG_RTREADMETER_REQ_INFO_S*)GetRealTimeAMRReqAddr(PORTn);
		UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
		if((TotalPnListNum_JC > 0) && (RT_AMRReq != NULL) && (RT_AMRReq->Lock == 0)) 
		{
			u8Flag_JC = 1;
			TotalPnListLen = TotalPnListNum_JC * sizeof(QCSG_PnListStruct);
			RT_AMRReq->SrcTask = GetUartTaskID(PORTn);
			RT_AMRReq->FrameType = FRAME_TYPE_QCSG_UP;
			RT_AMRReq->DataType  = DATA_TYPE_STRUCT;
			RT_AMRReq->QCSGFrameInfo.A1 = SrcFrame->A1;
			RT_AMRReq->QCSGFrameInfo.A2 = SrcFrame->A2;
			RT_AMRReq->QCSGFrameInfo.A3 = SrcFrame->A3;
			RT_AMRReq->QCSGFrameInfo.FC = SrcFrame->FC;
			RT_AMRReq->QCSGFrameInfo.AFN = SrcFrame->AFN;
			RT_AMRReq->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			RT_AMRReq->QCSGFrameInfo.Port = PORTn;
			RT_AMRReq->PnListNum = TotalPnListNum_JC;
			RT_AMRReq->PnListLen = TotalPnListLen;
			MR((u32)RT_AMRReq->Data, (u32)QCSG_PnList_JC, TotalPnListLen);
			RT_AMRReq->Lock = 0x01;
			//SetRealTimeAMRTaskReq(GetUartTaskID(PORTn), PORTn, RT_AMRReq);
		}
		
		if((TotalPnListNum_PN > 0) && (UartRxChCtrl != NULL) && (RT_AMRTask->Lock == 0)) 
		{
			u8Flag_Pn = 1;
			TotalPnListLen = TotalPnListNum_PN * sizeof(QCSG_PnListStruct);
			UartRxChCtrl->SrcPort = PORTn;
			UartRxChCtrl->Empty = BUFFER_FULL;
			UartRxChCtrl->FrameType = FRAME_TYPE_QCSG_UP;
			UartRxChCtrl->DataType = DATA_TYPE_STRUCT;
			UartRxChCtrl->QCSGFrameInfo.A1 = SrcFrame->A1;
			UartRxChCtrl->QCSGFrameInfo.A2 = SrcFrame->A2;
			UartRxChCtrl->QCSGFrameInfo.A3 = SrcFrame->A3;
			UartRxChCtrl->QCSGFrameInfo.FC = SrcFrame->FC;
			UartRxChCtrl->QCSGFrameInfo.AFN = SrcFrame->AFN;
			UartRxChCtrl->QCSGFrameInfo.SEQ = SrcFrame->SEQ;
			UartRxChCtrl->QCSGFrameInfo.Port = PORTn;
			UartRxChCtrl->PnListNum  = TotalPnListNum_PN;
			UartRxChCtrl->FrameLen = TotalPnListLen;
			UartRxChCtrl->FrameBuf = (u8*)QCSG_PnList_PN;
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


	
	if(QCSGFp.DstDataPos != 0)
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
		return NO_ERROR;
	}
	if(TotalPnListNum == 0)	
	{
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=0C
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
	
	return ERROR_DO_NOT_ANSWER;
}




ResultEnum GetTerminalSpecData(u32 DI, u8* Data, u32 MaxLen, u32* Len)
{
	LoadManagementParaStruct LoadManagementPara;
	#if ((Project/100)==2) || ((Project/100)==5) || ((Project/100)==9)		// I 型集中器 配变
		AMRCtrlStruct* AMRCtrl;
	#endif
	RealTimeStruct RealTime;
	u32 Index = 0, YXState = 0, YKState = 0;
	//Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Index = FindTerminalSpecDataIndex(TerminalSpecDataTbl, DI, TERMINAL_SPEC_DI_NUM);
	if(Index >= TERMINAL_SPEC_DI_NUM) return ERROR_NoSuchDataID;

	Len[0] = TerminalSpecDataTbl[Index].Len;
	if(Len[0] > MaxLen) return ERROR_WRONG_LENGTH;
	switch(DI)
	{
		case 0xE1800001:
			QCSG_ReadTerminal_State((u8*)&YXState, (u8*)&YKState);
			MR((u32)&LoadManagementPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));

			if(LoadManagementPara.PowerHoldState == 0x01)
			{
				YKState |= 0x80;
			}
			else
			{
				YKState &= ~0x80;
			}
#if (TEST_ON == 1)//台体测试时 屏蔽
			if(LoadManagementPara.PowerCtrlFlag == 0x01)
#else
			if((LoadManagementPara.PowerCtrlFlag == 0x01) || (LoadManagementPara.E0000E10 == 0x01) || 
			   ((LoadManagementPara.E0000C70  & 0xFF000000) == 0x01000000))
#endif
			{
				YKState |= 0x02;
			}
			else
			{
				YKState &= ~0x02;
			}			
			if(LoadManagementPara.MonthKwhCtrlFlag == 0x01)
			{
				YKState |= 0x01;
			}
			else
			{
				YKState &= ~0x01;
			}

			if((LoadManagementPara.OutputSwitchState[1] == 0x01) || (LoadManagementPara.OutputSwitchState[2] == 0x01)
			 ||(LoadManagementPara.OutputSwitchState[3] == 0x01) || (LoadManagementPara.OutputSwitchState[4] == 0x01))
			{
				YKState |= 0x04;
			}
			else
			{
				YKState &= ~0x04;
			}
			
#if (TEST_ON == 0)//台体测试时 屏蔽
				//err20180507 根据常开常闭类型改变终端状态字
				if(LoadManagementPara.SwitchNodeType[0] == 0x00)//常开触点
				{
					YXState ^= 0x01;
				}
				if(LoadManagementPara.SwitchNodeType[1] == 0x00)//常开触点
				{
					YXState ^= 0x08;
				}
				if(LoadManagementPara.SwitchNodeType[2] == 0x00)//常开触点
				{
					YXState ^= 0x10;
				}
				if(LoadManagementPara.SwitchNodeType[3] == 0x00)//常开触点
				{
					YXState ^= 0x20;
				}
				if(LoadManagementPara.SwitchNodeType[4] == 0x00)//常开触点
				{
					YXState ^= 0x40;
				}
				//err20180507 根据常开常闭类型改变终端状态字
#endif
			Data[0] = YXState;
			Data[1] = YKState;
			break;
		case 0xE1800009:	
			Index = hex_bcd(MeterFileInfo->TotalMeters);
			Data[0] = Index;
			Data[1] = Index >> 8;
			#if ((Project/100)==2) || ((Project/100)==9)		// I 型集中器	  配变
				Index = 0;
				AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_1PORT);
				Index += AMRCtrl->DayFreezOkMeters;
				AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_2PORT);
				Index += AMRCtrl->DayFreezOkMeters;
				AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_3PORT);
				Index += AMRCtrl->DayFreezOkMeters;
				AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PLC_PORT);
				Index += AMRCtrl->DayFreezOkMeters;
				Index = hex_bcd(Index);
				Data[2] = Index;
				Data[3] = Index >> 8;
			#endif
			#if ((Project/100)==3)		// 负荷管理终端
				Data[2] = 0;
				Data[3] = 0;
			#endif
			#if ((Project/100)==5)		// II 型集中器
				//Data[2] = 0;
				//Data[3] = 0;
				Index = 0;
				AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_1PORT);
				Index += AMRCtrl->DayFreezOkMeters;
				AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_2PORT);
				Index += AMRCtrl->DayFreezOkMeters;
				Index = hex_bcd(Index);
				Data[2] = Index;
				Data[3] = Index >> 8;
			#endif
			HAL_GetSystemTime(&RealTime);
			Data[4] = 0x00;
			Data[5] = 0x05;
			Data[6] = 0x00;
			Data[7] = RealTime.u8Day;
			Data[8] = RealTime.u8Month;
			Data[9] = RealTime.u8Year;
			Data[10] = 0x00;
			Data[11] = 0x59;
			Data[12] = 0x23;
			Data[13] = RealTime.u8Day;
			Data[14] = RealTime.u8Month;
			Data[15] = RealTime.u8Year;			
			break;
		case 0xE1800011:
			Index = Get_GPRSStoredRxTxBytes();
			Index >>= 10;
			Index = hex_bcd(Index);
			Data[0] = Index;
			Data[1] = Index >> 8;
			break;
		case 0xE1800013:
			Index = Get_GPRSCurMonthRxTxBytes();	
			Data[0] = Index;
			Data[1] = Index >> 8;
			Data[2] = Index >> 16;
			break;
		case 0xE1800015:
			Data[0] = Get_GPRSSignaldBm();
			break;
		case 0xE1800018:
			#if ((Project/100)==2) || ((Project/100)==9) // I 型集中器	  配变
				GetTerminalRouterInfo(Data);
			#else
				MC(0xFF, (u32)Data, 9);
			#endif
			break;
		default:
			MR((u32)Data, TerminalSpecDataTbl[Index].Addr, TerminalSpecDataTbl[Index].Len);
			break;
	}
	return NO_ERROR;
}


u16 FindTerminalSpecDataIndex(const QCSG_DITableStruct* DITable, u32 DI, u32 Size)
{
	u16 i = 0;
	for(i = 0; i < Size; i++)
	{
		if(DITable[i].DI == DI) break;
	}
	return i;
}
/*
u16 GetMaxPnNum(void)
{
	return 0;
}
u16 SplitPnToList(u16 Pn, u16* List, u8 PnCheck)
{
	u16 i = 0, Num = 0, PnIncEn = 0, TempPn = 0, MaxPn = 0;

	if(List == NULL) return Num;
	if(Pn == 0)
	{
		List[Num] = Pn;
		Num++;
	}
	else
	{
		if(Pn == QCSG_ALL_PN)
		{
			Pn = 0x01FF;
			PnIncEn = 1;
		}
		// 等待完成获取当前最大测量点号，暂用 MAX_PN_NUM 代替
		MaxPn = SwitchNumToPn(MAX_PN_NUM);
		
		for(i = 0; i < 8; i++)
		{
			if((Pn & (1 << i)) != 0) 
			{
				TempPn = Pn & 0xFF00;
				TempPn |= (1 << i);
				if((PnCheck != PN_CHECK_DISABLE) && ((TempPn > MaxPn) || (CheckPnValid(TempPn) != NO_ERROR)))
				{
					continue;
				}
				List[Num] = TempPn;
				Num++;
			}
			if((i >= 8) && (PnIncEn != 0))
			{
				i = 0;
				Pn += 0x0100;
				if((Pn & 0xFF00) == 0xFF00) break;
			}
		}
	}
	
	return Num;
}
*/


/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/

