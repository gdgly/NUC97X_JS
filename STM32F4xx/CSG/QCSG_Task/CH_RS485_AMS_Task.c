/*************************************************
版  权:正泰仪表智能终端部
文  件:CH_RS485_AMS_Task.c
作  者:luxiaowen
版  本:1.00
日  期:20190310
描  述:南网RS485自动搜表处理文件
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
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "Terminal_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/CH_RS485_AMS_Task.h" 
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

#if (((Project/100)==2) || ((Project/100)==5))

void CH_RS485_AMS_Task(u32 PORTn)
{
	RealTimeStruct RealTime;
	AMSCtrlStruct* Task = (AMSCtrlStruct*)Get_AMS_TaskCtrlAddr(PORTn);
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(Task == NULL) return;                                                                                                                                                                                                                                                                                                                                                                                
	Task->AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	
	if(GetTerminalPower_S_TimerValue() > 0) return;    
	HAL_GetSystemTime(&RealTime);

	//每天定时启动自动搜表
	if(((RealTime.u8Year != Task->LastSearchTime.u8Year) || 
	   (RealTime.u8Month != Task->LastSearchTime.u8Month) || 
	   (RealTime.u8Day != Task->LastSearchTime.u8Day))&&
	   (RealTime.u8Hour == 0x08) && (RealTime.u8Minute == 0x00) && (RealTime.u8Second == 0x00))
	{
		Task->Baud_ID	=  AMS_BAUDPRO_TOTAL;  
		Task->State     =  AMS_Idel;
		Task->LastSearchTime.u8Second = RealTime.u8Second;
		Task->LastSearchTime.u8Minute = RealTime.u8Minute;
		Task->LastSearchTime.u8Hour   = RealTime.u8Hour;
		Task->LastSearchTime.u8Day	  = RealTime.u8Day;  
		Task->LastSearchTime.u8Month  = RealTime.u8Month;
		Task->LastSearchTime.u8Year   = RealTime.u8Year;
		Task->LastSearchTime.u8Week   = RealTime.u8Week;
		Task->LastSearchTime.u8Sync   = RealTime.u8Sync;
		return; 
	}

	if(Task->AMRCtrl->State != AMR_Idle)
	{
		if(Task->State != AMS_Pause)
		{
			Task->OldState = Task->State;
		}
		
		Task->State = AMS_Pause;
		pTerminalRam->AT_AMSTimeOut = AMS_PAUSE_MAX_TIME;
		return;
	}
	
	if(PORTn == RS485_1PORT)
	{
		HAL_GetSystemTime(&RealTime);
	}
	
	switch(Task->State)
	{
		case AMS_Idel:
			RS485_AMS_IdleProcess(PORTn,Task,&RealTime);
			break;
		case AMS_Tx:
			RS485_AMS_TxProcess(PORTn,Task);
			break;
		case AMS_WaitRx:
			RS485_AMS_WaitRxProcess(PORTn,Task);
			break;
		case AMS_Pause:
			RS485_AMS_PauseProcess(PORTn,Task);
			break;
		case AMS_Resume:
			RS485_AMS_ResumeProcess(PORTn,Task);
			break;
		default:
			Task->State = AMS_Idel;
			break;
	}
		
}

ResultEnum RS485_AMS_IdleProcess(u32 PORTn,AMSCtrlStruct* Task, RealTimeStruct* RealTime)
{	
	RS485PortParaStruct RS485PortPara;
	u32 FlashAddr = 0;

	if(Task->Baud_ID == AMS_BAUDPRO_EMPTY)	return ERROR_WRONG_ACCESS;
	
    switch((u8)PORTn)
	{
		case RS485_1PORT:
			FlashAddr = ADDR_RS485PortParaTbl;
			break;
		case RS485_2PORT:
			FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
			break;
		case RS485_3PORT:
			FlashAddr = ADDR_RS485PortParaTbl + 2*sizeof(RS485PortParaStruct);
			break;
		default:
			FlashAddr = 0;
			break;
	}
	if(FlashAddr == 0) return ERROR_WRONG_ADDR;
	MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
	Refresh485PortFunction(PORTn, &RS485PortPara);//刷新485端口参数 1min
	
	if(RS485PortPara.Function != UART_FUNC_AMR)
	{
		Task->Baud_ID = AMS_BAUDPRO_EMPTY;
		return ERROR_WRONG_ACCESS;
	}
	
	if(Get_NextBaudRate_ID(Task) != NO_ERROR) 
	{
		Task->Baud_ID = 0;
		return ERROR_NO_AMS_TASK;
	}

	while(NO_ERROR != Get_NextAmsMeter_ID(Task))
	{
		if(NO_ERROR != Get_NextBaudRate_ID(Task))
		{
			Task->State = AMS_Idel;
			return ERROR_NO_AMS_TASK;
		}
	}
	
	Task->State = AMS_Tx; 
	
	return NO_ERROR;
}

ResultEnum RS485_AMS_TxProcess(u32 PORTn,AMSCtrlStruct* Task)
{		
	UartTxReqStruct UartTxReq;
	DLT645FrameStruct DLT645Frame;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	ResultEnum Result = NO_ERROR;
	u32 TxBuf = 0, TxLen = 0;
	
	if(NO_ERROR != Get_RS485Port_Function(PORTn))
	{
		Task->State = AMS_Idel;
		Task->Baud_ID = AMS_BAUDPRO_EMPTY;  
		return ERROR_WRONG_ACCESS;
	}
	Result = BuildAMS645FrameStruct(&DLT645Frame, Task);

	TxBuf = GetUartPortTxBuf(PORTn);
	TxLen = Build645Frame(&DLT645Frame, (u8*)TxBuf, NO_FE_IN_645);

	MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));

	UartTxReq.SrcTask = Get_AMS_TaskID(PORTn);
	UartTxReq.NeedAck = 1;
	UartTxReq.FrameType = FRAME_TYPE_DLT_645;
	UartTxReq.RetryCnt = 0;
	UartTxReq.BpsCtrl = ConvertToBpsCtrl(Task->PortBaudRate, Task->PortParity, Task->PortDataBit, Task->PortStopBit);
	UartTxReq.TimeOut = RS485_AMR_TIME_OUT;
	UartTxReq.DataLen = TxLen;
	UartTxReq.DataBuf = (u8*)TxBuf;
	pTerminalRam->AT_AMSTimeOut = RS485_AMR_TIME_OUT;
	Result = UartPortTxReqNew(PORTn, &UartTxReq);
	
	if(Result == NO_ERROR)
	{
		Task->State = AMS_WaitRx;
	}
	return Result;
}

ResultEnum RS485_AMS_WaitRxProcess(u32 PORTn,AMSCtrlStruct* Task)
{	
	UartTaskCtrlStruct* pUTask;
	UartRxChCtrlStruct* UartRxChCtrl;
	DLT645FrameStruct DLT645Frame;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	ResultEnum Result = NO_ERROR;


	if(pTerminalRam->AT_AMSTimeOut == 0)
	{
		//没有收到任何数据跳表
		Task->State = AMS_Tx;
		Task->AckState = STATE_NO_ACK;
		while(NO_ERROR != Get_NextAmsMeter_ID(Task))
		{
			if(NO_ERROR != Get_NextBaudRate_ID(Task))
			{
				Task->State = AMS_Idel;
				return ERROR_NO_AMS_TASK;
			}
		}
		return ERROR_TIME_OUT;
	}

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
	if(UartRxChCtrl == NULL) return ERROR_POINTER_NULL;
	if(UartRxChCtrl->Empty == BUFFER_EMPTY) return ERROR_BUF_EMPTY;
	if(UartRxChCtrl->FrameBuf == NULL) return ERROR_POINTER_NULL;
	pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(PORTn);
	if(pUTask == NULL) return ERROR_POINTER_NULL;
	pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	UartRxChCtrl->Empty = BUFFER_EMPTY;
	
	if((UartRxChCtrl->FrameType != FRAME_TYPE_DLT_645)
	&&(UartRxChCtrl->FrameType != FRAME_TYPE_MIXED_645))
	{
		return ERROR_WRONG_PROTOCOL;
	}
	

	if(UartRxChCtrl->FrameType == FRAME_TYPE_DLT_645)
	{
		Result = DLT645FrameDcompress(&DLT645Frame, UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
		if(Result != NO_ERROR) return Result;
		if((DLT645Frame.CtrlCode != 0x91)&&(DLT645Frame.CtrlCode != 0x81)) return ERROR_VerifyFail;
		SaveAMS645MeterParamAndE2010014Event(PORTn,Task,DLT645Frame.Addr);
		Task->AckState = STATE_RIGHT_ACK;
		while(NO_ERROR != Get_NextAmsMeter_ID(Task))
		{
			if(NO_ERROR != Get_NextBaudRate_ID(Task))
			{
				Task->State = AMS_Idel;
				return ERROR_NO_AMS_TASK;
			}
		}
	}

	if(UartRxChCtrl->FrameType == FRAME_TYPE_MIXED_645)
	{
		Task->AckState = STATE_MIXED_ACK;
		while(NO_ERROR != Get_NextAmsMeter_ID(Task))
		{
			if(NO_ERROR != Get_NextBaudRate_ID(Task))
			{
				Task->State = AMS_Idel;
				return ERROR_NO_AMS_TASK;
			}
		}
	}
	
	Task->State = AMS_Tx;
	return NO_ERROR;
}

ResultEnum RS485_AMS_PauseProcess(u32 PORTn,AMSCtrlStruct* Task)
{	
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;

	if(pTerminalRam->AT_AMSTimeOut == 0)
	{
		Task->State = AMS_Resume;
		return NO_ERROR;
	}
		
	return NO_ERROR;
}

ResultEnum RS485_AMS_ResumeProcess(u32 PORTn,AMSCtrlStruct* Task)
{
	if(Task->Baud_ID != AMS_BAUDPRO_EMPTY)
	{
		if(Task->MeterIndex == 0)
		{
			Task->State = AMS_Idel;
		}
		else
		{
			Task->State = AMS_Tx;
		}
		return NO_ERROR;
	}

	Task->State = AMS_Idel;
	return NO_ERROR;
}


ResultEnum SaveAMS645MeterParamAndE2010014Event(u32 PORTn, AMSCtrlStruct* Task, u8* TrueAddr)
{
	MeasurePointStruct MeasurePoint;
	u32 pn = 0;
	u32 DataID = 0xE080000F;
	u32 WriteLen = sizeof(MeasurePointStruct);
	ResultEnum Result = NO_ERROR;

	if(NO_ERROR != QCSG_GetLastUnUsedPnByAddr(&pn,TrueAddr)) return ERROR_WRONG_ADDR;
	
	MemSet((u8*)&MeasurePoint, 0, sizeof(MeasurePointStruct));	
	MeasurePoint.State = 0x01;
	MeasurePoint.MeasureType = 0x01;
	MeasurePoint.PortID = (PORTn > RS485_2PORT)?(PORTn-3):(PORTn-2);
	MemCopy(MeasurePoint.Addr ,TrueAddr, METER_ADDR_SIZE);
	MeasurePoint.ProtocolType = Task->Protocol;
	MeasurePoint.PointMeterType = Task->PMType;
	MeasurePoint.PortBaudRate = Task->PortBaudRate;
	MeasurePoint.PortParity = Task->PortParity;
	MeasurePoint.PortDataBit = Task->PortDataBit;
	MeasurePoint.PortStopBit = Task->PortStopBit;
	
	Result = NAND_WriteMeasurePoint(pn,DataID, (u8*)&MeasurePoint, &WriteLen);
	if(Result == NO_ERROR)
	{
		GenerateParaChangeEvent(DataID);
	}
	
	return NO_ERROR;
}

ResultEnum BuildAMS645FrameStruct(DLT645FrameStruct* Frame, AMSCtrlStruct* Task)
{
	ResultEnum Result = NO_ERROR;
	MC(0, (u32)Frame, sizeof(DLT645FrameStruct));
	
	MR((u32)Frame->Addr, (u32)Task->Addr, METER_ADDR_SIZE);
	if(Task->Protocol == DLT_645_PROTOCOL_07)
	{
		Frame->CtrlCode = DLT_07_READ;
		Frame->DataLen = DLT_645_07_ID_SIZE;
		Frame->Data[0] = (u8)SEARCH_07_DI;
		Frame->Data[1] = (u8)(SEARCH_07_DI >> 8);
		Frame->Data[2] = (u8)(SEARCH_07_DI >> 16);
		Frame->Data[3] = (u8)(SEARCH_07_DI >> 24);
	}
	else if(Task->Protocol == DLT_645_PROTOCOL_97)
	{
		Frame->CtrlCode = DLT_97_READ;
		Frame->DataLen = DLT_645_97_ID_SIZE;
		Frame->Data[0] =(u8)(SEARCH_97_DI);
		Frame->Data[1] =(u8)(SEARCH_97_DI >> 8);		
	}
	else
	{
		Result = ERROR_WRONG_ACCESS;
	}
	return Result;
}

ResultEnum Get_RS485Port_Function(u32 PORTn)
{
	RS485PortParaStruct RS485PortPara;
	u32 FlashAddr = 0;	

	switch((u8)PORTn)
	{
		case RS485_1PORT:
			FlashAddr = ADDR_RS485PortParaTbl;
			break;
		case RS485_2PORT:
			FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
			break;
		case RS485_3PORT:
			FlashAddr = ADDR_RS485PortParaTbl + 2*sizeof(RS485PortParaStruct);
			break;
		default:
			FlashAddr = 0;
			break;
	}
	if(FlashAddr == 0) return ERROR_WRONG_ADDR;
	MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
	Refresh485PortFunction(PORTn, &RS485PortPara);//刷新485端口参数 1min
	
	if(RS485PortPara.Function != UART_FUNC_AMR)
	{
		return ERROR_WRONG_ACCESS;
	}

	return NO_ERROR;
}


ResultEnum Get_NextBaudRate_ID(AMSCtrlStruct* Task)
{
	u8 ID_Step = 0;
		
	if(Task->Baud_ID == 0) return ERROR_InvalidData;

	Task->Baud_ID_Step = AMS_1200BPS_07_ID;
	while(Task->Baud_ID_Step <= AMS_9600BPS_97_ID)
	{	
		if((Task->Baud_ID_Step & Task->Baud_ID) != 0)
		{
			break;
		}
		Task->Baud_ID_Step <<= 1;
		ID_Step++;
	}
	if(Task->Baud_ID_Step > AMS_9600BPS_97_ID) 	return ERROR_WRONG_ACCESS;
	
	//搜表只切换协议和波特率，偶校验、8位、1位停止位固定
	MemSet(Task->Addr, 0xAA,METER_ADDR_SIZE);
	Task->MeterIndex = 0;
	Task->LnIndex = 0;
	Task->CLIndex = 0;
	Task->AckState = STATE_UN_TX;
	Task->Protocol = (ID_Step % 2 == 0)?DLT_645_PROTOCOL_97:DLT_645_PROTOCOL_07;
	switch(ID_Step/2)
	{
		case 0:			
			Task->PortBaudRate = BAUD_RATE_1200/BAUD_RATE_300;
			break;
		case 1:			
			Task->PortBaudRate = BAUD_RATE_2400/BAUD_RATE_300;
			break;
		case 2:
			Task->PortBaudRate = BAUD_RATE_4800/BAUD_RATE_300;
			break;
		case 3:
			Task->PortBaudRate = BAUD_RATE_9600/BAUD_RATE_300;
			break;
		default:		
			Task->PortBaudRate = BAUD_RATE_2400/BAUD_RATE_300;;
			break;
	}
	Task->PortParity = 1; 	
	Task->PortDataBit = 8;	
	Task->PortStopBit = 0;	
	Task->Cur645_DI_Step = 0;
	Task->PMType = 0x01;
	
	return NO_ERROR;
}


ResultEnum Get_NextAmsMeter_ID(AMSCtrlStruct* Task)
{

	if((Task->MeterIndex == 0)&&((Task->AckState == STATE_NO_ACK)||(Task->AckState == STATE_RIGHT_ACK))) 
	{
		Task->Baud_ID &= (~Task->Baud_ID_Step);
		return ERROR_WRONG_ACCESS;
	}
		
	if((Task->MeterIndex == 0)&&(Task->AckState == STATE_UN_TX))
	{
		MemSet(Task->Addr, 0xAA,METER_ADDR_SIZE);
		return NO_ERROR;
	}

	if((Task->AckState == STATE_NO_ACK)||(Task->AckState == STATE_RIGHT_ACK))
	{
		Task->AckState = STATE_UN_TX;
		if(Task->LnIndex < 0x99)
		{
			if(Task->MeterIndex == 1)
			{
				Task->LnIndex = QCSG_IncBCD(Task->LnIndex);
			}
			Task->Addr[Task->CLIndex] = Task->LnIndex;
		}
		else
		{
			while(Task->CLIndex > 0)
			{
				Task->CLIndex--;
				Task->LnIndex = QCSG_IncBCD(Task->Addr[Task->CLIndex]);

				if(Task->LnIndex < 0x99)
				{
					Task->Addr[Task->CLIndex + 1] = 0xAA;
					Task->Addr[Task->CLIndex] = Task->LnIndex;
					return NO_ERROR;
				}
			}
			
			if(Task->CLIndex == 0) 
			{
				Task->Baud_ID &= (~Task->Baud_ID_Step);
				Task->MeterIndex = 0;
				return ERROR_WRONG_ACCESS;
			}
		}
	}

	if(Task->AckState == STATE_MIXED_ACK)
	{
		Task->AckState = STATE_UN_TX;
		if(Task->MeterIndex == 1)
		{
			if(Task->CLIndex < 0x05)
			{
				Task->CLIndex++;
			}
		}
		Task->LnIndex = 0;
		Task->Addr[Task->CLIndex] = Task->LnIndex;
	}
	
	Task->MeterIndex = 1;
	return NO_ERROR;
}

#endif
/**************************************** (C) COPYRIGHT luxiaowen ***************************************END OF FILE****/


