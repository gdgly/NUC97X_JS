/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN12.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN12处理文件
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
#include "../QCSG_Head/CH_RS485_AMR_Task.h" 
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

#if (((Project/100)==2) || ((Project/100)==5) || ((Project/100)==9))

void CH_RS485_AMR_Task(u32 PORTn)
{
	RealTimeStruct RealTime;
	u16* pAckTimer;
	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	#if ((Project/100)==2)||((Project/100)==5)||((Project/100)==9) //配变 实时任务加锁，防止负控代码进行抄表冲突。
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RT = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RT;
	#endif
	
	if(AMRCtrl == NULL) return;
	if(GetTerminalPower_S_TimerValue() > 0) return;
	HAL_GetSystemTime(&RealTime);
	if(RealTime.u8Minute == 0x00)
	{
		AMRCtrl->Flag |= AMR_FLAG_TIME_CROSS_HOUR;
	}

	if((MeterFileInfo->RS485Meters == 0) && ((RealTime.u8Second & 0x07) == 0) && 
	   (AMRCtrl->State > AMR_Idle) && (AMRCtrl->State < AMR_Pause))
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
	}
	
	if((AMRCtrl->Flag & AMR_FLAG_PAUSE) != 0)
	{
	#if ((Project/100)==9)////其他负控代码中的定时抄表任务完成后再进行实时任务抄读
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485I = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485II = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485III = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_JC = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_JC;
		if((pstRDMeter_Ctrl_JC->u8Lock == FUNC_LOCK)
		||(pstRDMeter_Ctrl_RS485I->u8Lock == FUNC_LOCK)
		||(pstRDMeter_Ctrl_RS485II->u8Lock == FUNC_LOCK)
		||(pstRDMeter_Ctrl_RS485III->u8Lock == FUNC_LOCK))
		{
			return ;
		}
	#endif
	
		pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
		AMRCtrl->Flag &= ~AMR_FLAG_PAUSE;
		if(AMRCtrl->State <= AMR_Pause)
		{
			if(AMRCtrl->State != AMR_Pause)
			{
				AMRCtrl->OldState = AMRCtrl->State;
				AMRCtrl->State = AMR_Pause;	
			
				pstRDMeter_Ctrl_RT->u8Lock = FUNC_LOCK;
			}
			pAckTimer[0] = AMR_PAUSE_MAX_TIME;
			Task->Flag |= RT_AMR_CH_READY;
		}
	}
	
	switch(AMRCtrl->State)
	{
		case AMR_Idle:
			RS485_AMR_IdleProcess(PORTn, AMRCtrl, &RealTime);
			break;
		case AMR_Tx:
			RS485_AMR_TxProcess(PORTn, AMRCtrl);
			break;
		case AMR_WaitRx:
			RS485_AMR_WaitRxProcess(PORTn, AMRCtrl);
			break;
		case AMR_Pause:
			RS485_AMR_PauseProcess(PORTn, AMRCtrl);
			break;
		case AMR_RealTimeTx:
			RS485_AMR_RealTimeTxProcess(PORTn, AMRCtrl);
			break;
		case AMR_RealTimeWaitRx:
			RS485_AMR_WaitRealTimeRxProcess(PORTn, AMRCtrl);
			break;
		case AMR_Resume:
			RS485_AMR_ResumeProcess(PORTn, AMRCtrl);
			break;
		default:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
	}
}

ResultEnum RS485_AMR_IdleProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* RealTime)
{
	RS485PortParaStruct RS485PortPara;
	u16* pAckTimer;
	u32 FlashAddr = 0;
	u16 RS485ChMeterNum = 0;	
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	#if ((Project/100)==9)
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl =  (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
	#endif
	#if ((Project/100) == 5)
		RealTimeYYStruct YYTime;
		NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList; 
		u8* AMRDataBuf = NULL;	
		u32 TotalDataLen = 0;
		u32 NW_DI_Temp = 0;
		AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	#endif
	
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if((pAckTimer == NULL) || (pAckTimer[0] != 0)) return ERROR_WRONG_TIME;	
	
#if ((Project/100)==9)
	switch((u8)PORTn)
	{
		case RS485_1PORT: 
			pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
			break;
		case RS485_2PORT:
			pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
			break;
		case RS485_3PORT: 
			pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
			break;
		default:break;
	}
	if(AMRCtrl->AMR_ID == 0) //err20181019 集中器无任务
	{
		if(pstRDMeter_Ctrl->u8PB_Task_Lock == TASK_JZQ_LOCK)//若集中器任务锁住，则判断任务执行完成后解锁
			pstRDMeter_Ctrl->u8PB_Task_Lock = TASK_PB_UNLOCK;
		
		//return ERROR_NO_AMR_TASK;
	}
#endif
	
	if(AMRCtrl->LastCheckTime.u8Minute == RealTime->u8Minute) return ERROR_WRONG_TIME;
	AMRCtrl->LastCheckTime.u8Second = RealTime->u8Second;
	AMRCtrl->LastCheckTime.u8Minute = RealTime->u8Minute;
	AMRCtrl->LastCheckTime.u8Hour   = RealTime->u8Hour;
	AMRCtrl->LastCheckTime.u8Day    = RealTime->u8Day;
	AMRCtrl->LastCheckTime.u8Month  = RealTime->u8Month;
	AMRCtrl->LastCheckTime.u8Year   = RealTime->u8Year;
	AMRCtrl->LastCheckTime.u8Week   = RealTime->u8Week;
	AMRCtrl->LastCheckTime.u8Sync   = RealTime->u8Sync;
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			FlashAddr = ADDR_RS485PortParaTbl;
			RS485ChMeterNum = MeterFileInfo->RS485_CH1_Meters;
			break;
		case RS485_2PORT:
			FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
			RS485ChMeterNum = MeterFileInfo->RS485_CH2_Meters;
			break;
		case RS485_3PORT:
			FlashAddr = ADDR_RS485PortParaTbl + 2*sizeof(RS485PortParaStruct);
			RS485ChMeterNum = MeterFileInfo->RS485_CH3_Meters;
			break;
		default:
			FlashAddr = 0;
			break;
	}
	if(FlashAddr == 0) return ERROR_WRONG_ADDR;
	MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
	Refresh485PortFunction(PORTn, &RS485PortPara);//刷新485端口参数 1min
	
	if(RS485PortPara.Function != UART_FUNC_AMR) return ERROR_WRONG_ACCESS;
	if(RS485ChMeterNum == 0) return ERROR_DestAddrNotExist;
	
	AMRCtrl->TotalMeters = RS485ChMeterNum;
	AMRCtrl->AMR_ID = 0;

	if((RealTime->u8Year != AMRCtrl->LastDayFrozenTime.u8Year) || 
	   (RealTime->u8Month != AMRCtrl->LastDayFrozenTime.u8Month))
	{
		AMRCtrl->MonthFreezFailMeters = 0;
		AMRCtrl->MonthFreezOkMeters = 0;
		MeterFileInfo->AMRMonthFreezFailMeters = 0;
	}
	
	if((RealTime->u8Year != AMRCtrl->LastDayFrozenTime.u8Year) || 
	   (RealTime->u8Month != AMRCtrl->LastDayFrozenTime.u8Month) || 
	   (RealTime->u8Day != AMRCtrl->LastDayFrozenTime.u8Day))
	{
		#if (TEST_ON == 1)
		if(RealTime->u8Minute > 0x01)
		#else
		if(RealTime->u8Minute > 0x04)
		#endif
		{
			AMRCtrl->AMR_ID |= AMR_DAY_FREEZ_ID;
			AMRCtrl->DayFreezFailMeters = 0;
			AMRCtrl->DayFreezOkMeters = 0;
			MeterFileInfo->AMRDayFreezFailMeters = 0;
			MC(0, (u32)ADDR_MeterDayFreezFailList, LEN_MeterDayFreezFailList);
		}	
	}

	if((RealTime->u8Year != AMRCtrl->LastHourFrozenTime.u8Year) || 
	   (RealTime->u8Month != AMRCtrl->LastHourFrozenTime.u8Month) || 
	   (RealTime->u8Day != AMRCtrl->LastHourFrozenTime.u8Day) || 
	   (RealTime->u8Hour != AMRCtrl->LastHourFrozenTime.u8Hour))
	{
		#if (TEST_ON == 0)
			if(RealTime->u8Minute > 0x04)
		#endif
		{
			AMRCtrl->AMR_ID |= AMR_HOUR_FREEZ_ID;
			AMRCtrl->HourFreezFailMeters = 0;
			AMRCtrl->HourFreezOkMeters = 0;
			if(AMRCtrl->DayFreezFailMeters != 0)
			{	//err20181214 如果该端口下有一个表抄失败，则其他表均无法进行日冻结。
				//原因是补抄日冻结后冻结时间被更新，不再进入正常日冻结流程。
				//若端口下失败表数与总失败表数不一致，就不去补抄冻结数据。(其他端口日冻结流程会清总失败表数)
				//若是跨日零点，不去补日冻结数据。等待正常日冻结流程。
				if((MeterFileInfo->AMRDayFreezFailMeters != 0) && (RealTime->u8Hour != 0x00))
				AMRCtrl->AMR_ID |= AMR_DAY_FREEZ_PATCH_ID;
			}
		}	
	}
	AMRTaskManageCheckNormalTask(PORTn, AMRCtrl, RealTime);
	AMRTaskManageCheckRelayTask(PORTn, AMRCtrl, RealTime);
	CheckNormalTasktoExecute(PORTn, AMRCtrl, RealTime);
	CheckRelayTasktoExecute(PORTn, AMRCtrl, RealTime);
	CheckAlarmTasktoExecute(PORTn, AMRCtrl, RealTime);

	if(AMRCtrl->AMR_ID == 0) return ERROR_NO_AMR_TASK;
	if((AMRCtrl->AMR_ID & (AMR_DAY_FREEZ_ID | AMR_DAY_FREEZ_PATCH_ID)) != 0)
	{
		//MR((u32)&AMRCtrl->LastDayFrozenTime, (u32)RealTime, sizeof(RealTimeStruct));
		AMRCtrl->LastDayFrozenTime.u8Second = RealTime->u8Second;
		AMRCtrl->LastDayFrozenTime.u8Minute = RealTime->u8Minute;
		AMRCtrl->LastDayFrozenTime.u8Hour   = RealTime->u8Hour;
		AMRCtrl->LastDayFrozenTime.u8Day    = RealTime->u8Day;
		AMRCtrl->LastDayFrozenTime.u8Month  = RealTime->u8Month;
		AMRCtrl->LastDayFrozenTime.u8Year   = RealTime->u8Year;
		AMRCtrl->LastDayFrozenTime.u8Week   = RealTime->u8Week;
		AMRCtrl->LastDayFrozenTime.u8Sync   = RealTime->u8Sync;
	}

	if((AMRCtrl->AMR_ID & AMR_HOUR_FREEZ_ID) != 0)
	{
		//MR((u32)&AMRCtrl->LastHourFrozenTime, (u32)RealTime, sizeof(RealTimeStruct));
		AMRCtrl->LastHourFrozenTime.u8Second = RealTime->u8Second;
		AMRCtrl->LastHourFrozenTime.u8Minute = RealTime->u8Minute;
		AMRCtrl->LastHourFrozenTime.u8Hour   = RealTime->u8Hour;
		AMRCtrl->LastHourFrozenTime.u8Day    = RealTime->u8Day;
		AMRCtrl->LastHourFrozenTime.u8Month  = RealTime->u8Month;
		AMRCtrl->LastHourFrozenTime.u8Year   = RealTime->u8Year;
		AMRCtrl->LastHourFrozenTime.u8Week   = RealTime->u8Week;
		AMRCtrl->LastHourFrozenTime.u8Sync   = RealTime->u8Sync;
	}
	
//err20181121 ****************************** ybwhz测试段 增加信号强度曲线数据**************************
	
	/*需要注意曲线数据存储函数的几个参数
	ResultEnum NAND_WriteMeterCurveData(AMR_ENUM AmrEnum, u8*addr, u8 PointMeterType, u32 DataID, RealTimeYYStruct* Time, u8* WriteData, u32* DataLen)
	@ PointMeterType 	表型号，暂保留不用
	@ DataID 			数据标识，暂保留不用
	@ Time 				数据时间，数据格式yyyy mm dd hh mi 00  00
	@ WriteData 		要写入的数据，格式: dataId(4) L(2) data(x)
	@ DataLen 			要写入的数据长度即 WriteData的总长度
	*/
#if ((Project/100) == 5)
	if((AMRCtrl->AMR_ID & AMR_HOUR_FREEZ_ID) != 0)
	{
		NW_DI_Temp = 0xE1800015;
		AMRDataBuf[TotalDataLen++] = NW_DI_Temp;		//数据标识DI
		AMRDataBuf[TotalDataLen++] = NW_DI_Temp >> 8;
		AMRDataBuf[TotalDataLen++] = NW_DI_Temp >> 16;
		AMRDataBuf[TotalDataLen++] = NW_DI_Temp >> 24;	
		
		AMRDataBuf[TotalDataLen++] = 1;
		AMRDataBuf[TotalDataLen++] = 1 >> 8;	//数据长度 1

		AMRDataBuf[TotalDataLen++] = (u8)Get_GPRSSignaldBm();
	
		RealTimeToYYTime(&AMRCtrl->LastHourFrozenTime, &YYTime);
		YYTime.Minute = 0x00;
		YYTime.Ack = 0;
		YYTime.Report = 0;
		NAND_WriteMeterCurveData(AMR_CurveFreeze, NodeList[0].Addr, METER_TYPE_OTHER, NW_DI_Temp, &YYTime, (u8*)AMRDataBuf, &TotalDataLen);
	}
	
#endif	
	
//err20181121 ****************************** ybwhz测试段 增加信号强度曲线数据**************************   

	AMRCtrl->AMR_ID_Step = 0x0001;
	if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR) 
	{
		AMRCtrl->AMR_ID = 0;
		return ERROR_NO_AMR_TASK;
	}

	while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->AMR_ID_Step <<= 1;
		if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
		{
			return ERROR_NO_AMR_TASK;
		}
	}
	
	while(Get_NextAMR_NW_DI(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->MeterIndex++;
		while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->AMR_ID_Step <<= 1;
			if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
			{
				return ERROR_NO_AMR_TASK;
			}
		}
	}

	while(Get_NextAMR_645_DI(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->CurNW_DI_Step++;
		while(Get_NextAMR_NW_DI(AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->MeterIndex++;
			while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
			{
				AMRCtrl->AMR_ID_Step <<= 1;
				if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
				{
					return ERROR_NO_AMR_TASK;
				}
			}
		}
	}
	
	AMRCtrl->FailMeters = 0;
	AMRCtrl->SuccessMeters = 0;
	AMRCtrl->TryCnt = 0;
	AMRCtrl->State = AMR_Tx;
#if ((Project/100)==9)
	if(pstRDMeter_Ctrl->u8PB_Task_Lock == TASK_PB_UNLOCK)
		pstRDMeter_Ctrl->u8PB_Task_Lock = TASK_JZQ_LOCK;
#endif
	
	AMR_ADDR_Display(PORTn, AMRCtrl->Addr);

	
	if((AMRCtrl->AMR_ID_Step & (AMR_DAY_FREEZ_ID | AMR_HOUR_FREEZ_ID)) != 0)
	{
		AMR_NUM_Display(PORTn, AMRCtrl->TotalMeters, AMRCtrl->SuccessMeters, AMRCtrl->FailMeters);
	}
	return NO_ERROR;
}


ResultEnum RS485_AMR_TxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	ResultEnum Result = NO_ERROR;
#if(Project/100 == 9)
	QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl =  (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
	switch((u8)PORTn)
	{
		case RS485_1PORT: 
			pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
			break;
		case RS485_2PORT:
			pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
			break;
		case RS485_3PORT: 
			pstRDMeter_Ctrl = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
			break;
		default:break;
	}
	if(pstRDMeter_Ctrl->u8PB_Task_Lock == TASK_FK_LOCK)//若负控任务锁住，则等待负控任务执行完成后发送。
		return ERROR_TASK_BUSY;
#endif
	
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
			Result = RS485_AMR_Build645AndTxProcess(PORTn, AMRCtrl);
			break;
		case AMR_DAY_FREEZ_PATCH_ID:
			Result = RS485_AMR_Build645AndTxProcess(PORTn, AMRCtrl);
			break;
		case AMR_HOUR_FREEZ_ID:
			Result = RS485_AMR_Build645AndTxProcess(PORTn, AMRCtrl);
			break;	
		case AMR_NORMAL_TASK_ID:
			Result = RS485_AMR_Build645AndTxProcess(PORTn, AMRCtrl);
			break;
		case AMR_RELAY_TASK_ID:
			Result = RS485_AMR_TxRelayTaskProcess(PORTn, AMRCtrl);
			break;
		case AMR_ALARM_TASK_ID:
			Result = RS485_AMR_Build645AndTxProcess(PORTn, AMRCtrl);
			break;
		case AMR_EVENT_TASK_ID:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
		default:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
	}
	return Result;
}

ResultEnum RS485_AMR_Build645AndTxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	u16* pPortAckTimer;
	UartTxReqStruct UartTxReq;
	DLT645FrameStruct DLT645Frame;
	
	ResultEnum Result = NO_ERROR;
	u32 TxBuf = 0, TxLen = 0;

	Result = BuildMeterAMR645FrameStruct(&DLT645Frame, AMRCtrl);

	TxBuf = GetUartPortTxBuf(PORTn);
	TxLen = Build645Frame(&DLT645Frame, (u8*)TxBuf, NO_FE_IN_645);

	MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));

	UartTxReq.SrcTask = Get_AMR_TaskID(PORTn);
	UartTxReq.NeedAck = 1;
	UartTxReq.FrameType = FRAME_TYPE_DLT_645;
	UartTxReq.RetryCnt = 0;
	UartTxReq.BpsCtrl = ConvertToBpsCtrl(AMRCtrl->PortBaudRate, AMRCtrl->PortParity, AMRCtrl->PortDataBit, AMRCtrl->PortStopBit);
	UartTxReq.TimeOut = RS485_AMR_TIME_OUT;
	UartTxReq.DataLen = TxLen;
	UartTxReq.DataBuf = (u8*)TxBuf;
	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer != NULL)
	{
		pPortAckTimer[0] = RS485_AMR_TIME_OUT;
	}
	Result = UartPortTxReqNew(PORTn, &UartTxReq);
	
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_WaitRx;
	}
	return Result;
}

ResultEnum RS485_AMR_TxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	u16* pPortAckTimer;
	UartTxReqStruct UartTxReq;
	RelayTaskParaStruct RelayTaskPara;
	
	ResultEnum Result = NO_ERROR;
	u32 TxBuf = 0, CurTaskID = 0, Addr = 0;

	TxBuf = GetUartPortTxBuf(PORTn);
	CurTaskID = AMRCtrl->RelayTask->CurTaskID;
	MC(0, (u32)&RelayTaskPara, sizeof(RelayTaskParaStruct));
	Addr = ADDR_RelayTaskPara + CurTaskID*sizeof(RelayTaskParaStruct);
	MR((u32)&RelayTaskPara, Addr, sizeof(RelayTaskParaStruct));

	for(Addr = 0; Addr < RelayTaskPara.Length; Addr++)
	{
		if(RelayTaskPara.Frame[Addr] == DLT_645_HEAD) break;
	}

	MR((u32)AMRCtrl->Addr, (u32)&RelayTaskPara.Frame[Addr + 1], METER_ADDR_SIZE);
	MR(TxBuf, (u32)RelayTaskPara.Frame, RelayTaskPara.Length);
	
	MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));
	UartTxReq.SrcTask = Get_AMR_TaskID(PORTn);
	UartTxReq.NeedAck = 1;
	UartTxReq.FrameType = FRAME_TYPE_DLT_645;
	UartTxReq.RetryCnt = 0;
	UartTxReq.BpsCtrl = ConvertToBpsCtrl(AMRCtrl->PortBaudRate, AMRCtrl->PortParity, AMRCtrl->PortDataBit, AMRCtrl->PortStopBit);
	UartTxReq.TimeOut = RS485_AMR_TIME_OUT;
	UartTxReq.DataLen = RelayTaskPara.Length;
	UartTxReq.DataBuf = (u8*)TxBuf;
	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer != NULL)
	{
		pPortAckTimer[0] = RS485_AMR_TIME_OUT;
	}
	Result = UartPortTxReqNew(PORTn, &UartTxReq);
	
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_WaitRx;
	}
	return Result;
}

ResultEnum RS485_AMR_WaitRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	UartTaskCtrlStruct* pUTask;
	UartRxChCtrlStruct* UartRxChCtrl;
	DLT645FrameStruct DLT645Frame;
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;

	//u32 FrameLen = 0;
	//u8 DataType = 0, FrameType = 0;
	//u8* FrameBuf = (u8*)GetUartPortRxBuf(PORTn, DATA_TYPE_FRAME);
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] == 0)
	{
		Result = RS485_AMR_RxTimeOutProcess(PORTn, AMRCtrl);
		return Result;
	}

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
	if(UartRxChCtrl == NULL) return ERROR_POINTER_NULL;
	if(UartRxChCtrl->Empty == BUFFER_EMPTY) return ERROR_BUF_EMPTY;
	if(UartRxChCtrl->FrameBuf == NULL) return ERROR_POINTER_NULL;
	pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(PORTn);
	if(pUTask == NULL) return ERROR_POINTER_NULL;
	pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	UartRxChCtrl->Empty = BUFFER_EMPTY;
	if(UartRxChCtrl->FrameType != FRAME_TYPE_DLT_645) return ERROR_WRONG_PROTOCOL;
	
	//Result = UartPortRxReq(PORTn, FrameBuf, &FrameLen, &FrameType, &DataType);
	//if(Result != NO_ERROR) return Result;
	//if(FrameType != FRAME_TYPE_DLT_645) return ERROR_WRONG_PROTOCOL;

	Result = DLT645FrameDcompress(&DLT645Frame, UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
	if(Result != NO_ERROR) return Result;
	
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
			Result = RS485_AMR_RxDayFreezProcess(PORTn, AMRCtrl, &DLT645Frame);
			break;
		case AMR_DAY_FREEZ_PATCH_ID:
			Result = RS485_AMR_RxDayFreezProcess(PORTn, AMRCtrl, &DLT645Frame);
			break;
		case AMR_HOUR_FREEZ_ID:
			Result = RS485_AMR_RxHourFreezProcess(PORTn, AMRCtrl, &DLT645Frame);
			break;	
		case AMR_NORMAL_TASK_ID:
			Result = RS485_AMR_RxNormalTaskProcess(PORTn, AMRCtrl, &DLT645Frame);
			break;
		case AMR_RELAY_TASK_ID:
			Result = RS485_AMR_RxRelayTaskProcess(PORTn, AMRCtrl, &DLT645Frame);
			break;
		case AMR_ALARM_TASK_ID:
			Result = RS485_AMR_RxAlarmTaskProcess(PORTn, AMRCtrl, &DLT645Frame);
			break;
		case AMR_EVENT_TASK_ID:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
		default:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
	}


	return Result;
}


ResultEnum RS485_AMR_RxTimeOutProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	RealTimeYYStruct YYTime;
	//RealTimeStruct RealTime;
	ResultEnum Result = NO_ERROR;
	u8* AMRDataBuf = NULL;
	u16 Index = 0, LeftLen = 0, OffSet = 0;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	u8* DayFreezFailMeterList = (u8*)ADDR_MeterDayFreezFailList;
		
	AMRCtrl->TryCnt++;
	if(AMRCtrl->TryCnt < MAX_AMR_TRY_CNT)
	{
		AMRCtrl->State = AMR_Tx;
		return NO_ERROR;
	}
	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	
	if(AMRCtrl->AMR_ID_Step == AMR_DAY_FREEZ_ID)
	{// 记录日冻结抄表失败，保存到抄表失败列表等待下次整点补抄
		OffSet = LEN_ListForOneMeter * MeterFileInfo->AMRDayFreezFailMeters;
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[0];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[1];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[2];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[3];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[4];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[5];
		AMRCtrl->DayFreezFailMeters++;
		MeterFileInfo->AMRDayFreezFailMeters++;
		
		if(AMRCtrl->LastDayFrozenTime.u8Day == 0x01)
		{// 记录日冻结抄表失败，保存到抄表失败列表等待下次整点补抄
			AMRCtrl->MonthFreezFailMeters++;
			MeterFileInfo->AMRMonthFreezFailMeters++;
		}
		NodeList[AMRCtrl->MeterIndex].FailCnt++;

		if(AMRCtrl->DayFreezOkMeters + AMRCtrl->DayFreezFailMeters == AMRCtrl->TotalMeters)
			GenerateMeterAMRFailAlarm_E200003D(AMRCtrl->MeterIndex);
		//GenerateMeterAMRFailAlarm_E200003D(AMRCtrl->MeterIndex);//err20190214 多个表抄表失败一次只上报一条
	}
		
	if((AMRCtrl->AMR_ID_Step == AMR_HOUR_FREEZ_ID) && (AMRDataBuf != NULL))
	{	
		//Index = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->CurNW_DI);
		Index = AMRCtrl->CurNW_DI_TblIndex;
		if((AMRCtrl->Cur645DataLen != 0) && (Index < MAX_AFN0C_0D_IDENT_NUM))
		{
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 8;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 16;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 24;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen >> 8;
			MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
			AMRCtrl->AMRTotalDataLen += AMRCtrl->Cur645DataLen;
			
			if(AMRCtrl->Cur645DataLen < gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen)
			{
				LeftLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].u16Dlen - AMRCtrl->Cur645DataLen;
				MC(0xFF, (u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], LeftLen);
				AMRCtrl->AMRTotalDataLen += LeftLen;
			}
		}	
		if(AMRCtrl->AMRTotalDataLen != 0)
		{
			//HAL_GetSystemTime(&RealTime);
			//RealTimeToYYTime(&RealTime, &YYTime);	
			RealTimeToYYTime(&AMRCtrl->LastHourFrozenTime, &YYTime);
			YYTime.Ack = 0;
			YYTime.Report = 0;
			YYTime.Minute = 0x00;
			NAND_WriteMeterCurveData(AMR_CurveFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);	
		}
		else
		{
			AMRCtrl->HourFreezFailMeters++;
		}
	}

	if((AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID) && (AMRDataBuf != NULL))
	{
		StoreNormalTaskAMRData(PORTn, AMRCtrl);
	}
	
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}

ResultEnum RS485_AMR_RxDayFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	AMRFileHeadStruct FileHead;
	ResultEnum Result = NO_ERROR;
	u8 DI_Size = 0;
	u16 Index = 0;
	u32 DataLen = 0;
	u8* AMRDataBuf = NULL;
	u8* PnData = (u8*)(ADDR_DATABUFF);
	//MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u8* DayFreezFailMeterList = (u8*)ADDR_MeterDayFreezFailList;	
	//u8* MonthFreezFailMeterList = (u8*)ADDR_MeterMonthFreezFailList;

	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;	
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;

	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)	//正常帧
	{
		if(Get645FrameDataID(Frame, AMRCtrl->Protocol) != AMRCtrl->Cur645_DI) return ERROR_InvalidData;
		DI_Size = DLT_645_07_ID_SIZE;
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			DI_Size = DLT_645_97_ID_SIZE;
		}
		if(Frame->DataLen > DI_Size)
		{
			MR((u32)&AMRCtrl->Cur645Data[AMRCtrl->Cur645DataLen], (u32)&Frame->Data[DI_Size], Frame->DataLen - DI_Size);
			AMRCtrl->Cur645DataLen += Frame->DataLen - DI_Size;
		}
	}
	else //if((Frame->CtrlCode & DLT_645_ERROR_BIT) != 0)	//异常帧，赋FF
	{		
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			DI_Size = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].stDLT64597Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
		}
		else
		{
			DI_Size = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].stDLT64507Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
		}
		MC(0xFF, (u32)&AMRCtrl->Cur645Data[AMRCtrl->Cur645DataLen], DI_Size);
		AMRCtrl->Cur645DataLen += DI_Size;
	}

	AMRCtrl->Cur645_DI_Step++;
	if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		return NO_ERROR;
	}

	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	//Index = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->CurNW_DI);
	Index = AMRCtrl->CurNW_DI_TblIndex;
	if((AMRCtrl->Cur645DataLen != 0) && (AMRDataBuf != NULL) && (Index < MAX_AFN0C_0D_IDENT_NUM))
	{
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 8;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 16;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 24;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->Cur645DataLen;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->Cur645DataLen >> 8;
		MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
		AMRCtrl->AMRTotalDataLen += AMRCtrl->Cur645DataLen;
	}

	AMRCtrl->CurNW_DI_Step++;
	if((AMRCtrl->PMType == METER_TYPE_MULTI) && (AMRCtrl->CurNW_DI_Step == METER_DAY_M_DI_NUM))	//多功能表的8个日冻结数据项都抄完了，存储日冻结数据
	{
		AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
		if((AMRCtrl->AMRTotalDataLen != 0) && (AMRDataBuf != NULL))
		{
			HAL_GetSystemTime(&RealTime);
			RealTimeToYYTime(&RealTime, &YYTime);
			//RealTimeToYYTime(&AMRCtrl->LastDayFrozenTime, &YYTime);
			YYTime.Ack = 0;
			YYTime.Report = 0;
			NAND_WriteMeterAMRData(AMR_DailyFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
		}
	}

	while(Get_NextAMR_NW_DI(AMRCtrl) == NO_ERROR)
	{
		if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
		{
			AMRCtrl->State = AMR_Tx;
			return NO_ERROR;
		}
		AMRCtrl->CurNW_DI_Step++;
	}

	if((AMRCtrl->CurNW_DI_Step >= AMRCtrl->DayFreezNwDINum))	//所有冻结数据都抄完了(多功能表抄完10个(月冻结)，单项抄完2个(日冻结))
	{
		AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
		if((AMRCtrl->AMRTotalDataLen != 0) && (AMRDataBuf != NULL))
		{
			HAL_GetSystemTime(&RealTime);
			RealTimeToYYTime(&RealTime, &YYTime);
			//RealTimeToYYTime(&AMRCtrl->LastDayFrozenTime, &YYTime);
			YYTime.Ack = 0;
			YYTime.Report = 0;
			if(AMRCtrl->PMType == METER_TYPE_SINGLE)
			{
				NAND_WriteMeterAMRData(AMR_DailyFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
			}

			if(YYTime.Day == 0x01)//1号进行月冻结。
			{
				NAND_WriteMeterAMRData(AMR_MonthFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
				if(AMRCtrl->MonthFreezOkMeters < AMRCtrl->TotalMeters) AMRCtrl->MonthFreezOkMeters++;
			}
			else//不是1号则查询当月是否已有月冻结数据，没有则存入。
			{
				Result = NAND_ReadMeterAMRData(AMR_MonthFreeze, AMRCtrl->Addr, &FileHead, AMRCtrl->CurNW_DI, &YYTime, PnData, &DataLen);
				if(Result != NO_ERROR)
				{
					NAND_WriteMeterAMRData(AMR_MonthFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
					if(AMRCtrl->MonthFreezOkMeters < AMRCtrl->TotalMeters) AMRCtrl->MonthFreezOkMeters++;
				}
			}
		}	
	}

	if(AMRCtrl->AMR_ID_Step == AMR_DAY_FREEZ_ID)
	{
		AMRCtrl->DayFreezOkMeters++;
		AMRCtrl->SuccessMeters++;
		NodeList[AMRCtrl->MeterIndex].FailCnt = 0;
		AMR_NUM_Display(PORTn, AMRCtrl->TotalMeters, AMRCtrl->SuccessMeters, AMRCtrl->FailMeters);

		//err20190214 多个表抄表失败一次只上报一条
		if(AMRCtrl->DayFreezOkMeters + AMRCtrl->DayFreezFailMeters == AMRCtrl->TotalMeters)
			GenerateMeterAMRFailAlarm_E200003D(AMRCtrl->MeterIndex);
	}
	
	if(AMRCtrl->AMR_ID_Step == AMR_DAY_FREEZ_PATCH_ID)
	{
		if(AMRCtrl->DayFreezFailMeters > 0) AMRCtrl->DayFreezFailMeters--;
		//if(MeterFileInfo->AMRDayFreezFailMeters > 0) MeterFileInfo->AMRDayFreezFailMeters--;
		Index = FindMeterInNodeList(AMRCtrl->Addr, NodeList, MAX_PN_NUM);
		NodeList[Index].FailCnt = 0;
		MC(0, (u32)&DayFreezFailMeterList[AMRCtrl->MeterIndex * METER_ADDR_SIZE], METER_ADDR_SIZE);
	}	
	
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}

ResultEnum RS485_AMR_RxHourFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	RealTimeYYStruct YYTime;
	ResultEnum Result = NO_ERROR;
	u8 DI_Size = 0;
	u16 Index = 0;
	u32 DataLenPos = 0;
	u8* AMRDataBuf = NULL;	
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;

	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)
	{
		if(Get645FrameDataID(Frame, AMRCtrl->Protocol) != AMRCtrl->Cur645_DI) return ERROR_InvalidData;

		DI_Size = DLT_645_07_ID_SIZE;
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			DI_Size = DLT_645_97_ID_SIZE;
		}
		if(Frame->DataLen > DI_Size)
		{
			if(AMRCtrl->CurNW_DI == 0x0202FF00 ||AMRCtrl->CurNW_DI == 0x0201FF00)	//整点电流电压，数据需要转换
			{
				Convert645ToNWAMRData(Frame, AMRCtrl->CurNW_DI_TblIndex, AMRCtrl->Cur645_DI_Step, AMRCtrl->Protocol, AMRCtrl->Cur645Data, &AMRCtrl->Cur645DataLen);
			}
			else
			{
				Index = Frame->DataLen - DI_Size;
				MR((u32)&AMRCtrl->Cur645Data[AMRCtrl->Cur645DataLen], (u32)&Frame->Data[DI_Size], Index);
				AMRCtrl->Cur645DataLen += Index;
			}
		}
	}
	else //if((Frame->CtrlCode & DLT_645_ERROR_BIT) != 0)
	{		
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			DI_Size = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].stDLT64597Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
		}
		else
		{
			DI_Size = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].stDLT64507Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
		}
		MC(0xFF, (u32)&AMRCtrl->Cur645Data[AMRCtrl->Cur645DataLen], DI_Size);
		AMRCtrl->Cur645DataLen += DI_Size;
	}

	AMRCtrl->Cur645_DI_Step++;
	if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		return NO_ERROR;
	}

	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	//Index = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->CurNW_DI);
	Index = AMRCtrl->CurNW_DI_TblIndex;
	if((AMRCtrl->Cur645DataLen != 0) && (AMRDataBuf != NULL) && (Index < MAX_AFN0C_0D_IDENT_NUM))
	{
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 8;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 16;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = AMRCtrl->CurNW_DI >> 24;
		DataLenPos = AMRCtrl->AMRTotalDataLen;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
		MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
		AMRCtrl->AMRTotalDataLen += AMRCtrl->Cur645DataLen;
		
		if((AMRCtrl->CurNW_DI == 0x0201FF00) || (AMRCtrl->CurNW_DI == 0x0202FF00))
		{
			Index = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].u16Dlen;
			for(; AMRCtrl->Cur645DataLen < Index; AMRCtrl->Cur645DataLen++)
			{
				AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0xFF;
			}
		}
		AMRDataBuf[DataLenPos]     = AMRCtrl->Cur645DataLen;
		AMRDataBuf[DataLenPos + 1] = AMRCtrl->Cur645DataLen >> 8;
	}

	AMRCtrl->CurNW_DI_Step++;
	while(Get_NextAMR_NW_DI(AMRCtrl) == NO_ERROR)
	{
		if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
		{
			AMRCtrl->State = AMR_Tx;
			return NO_ERROR;
		}
		AMRCtrl->CurNW_DI_Step++;
	}

	if(AMRCtrl->CurNW_DI_Step >= AMRCtrl->HourFreezNwDINum)
	{
		AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
		if((AMRCtrl->AMRTotalDataLen != 0) && (AMRDataBuf != NULL))
		{
			RealTimeToYYTime(&AMRCtrl->LastHourFrozenTime, &YYTime);
			YYTime.Minute = 0x00;
			YYTime.Ack = 0;
			YYTime.Report = 0;
			NAND_WriteMeterCurveData(AMR_CurveFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
		}
		AMRCtrl->HourFreezOkMeters++;
		AMRCtrl->SuccessMeters++;
		NodeList[AMRCtrl->MeterIndex].FailCnt = 0;
		AMR_NUM_Display(PORTn, AMRCtrl->TotalMeters, AMRCtrl->SuccessMeters, AMRCtrl->FailMeters);
	}

	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}


ResultEnum RS485_AMR_RxNormalTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	ResultEnum Result = NO_ERROR;
	u16 Index = 0;
	
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;
	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)
	{
		if(Get645FrameDataID(Frame, AMRCtrl->Protocol) != AMRCtrl->Cur645_DI) return ERROR_InvalidData;

		AMRCtrl->Cur645DataLen = 0;
		Convert645ToNWAMRData(Frame, AMRCtrl->CurNW_DI_TblIndex, AMRCtrl->Cur645_DI_Step, AMRCtrl->Protocol, AMRCtrl->Cur645Data, &AMRCtrl->Cur645DataLen);

		if(AMRCtrl->Cur645DataLen != 0)
		{
			Index = AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].DataLen;
			MR((u32)&AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].Data[Index], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
			AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].DataLen += AMRCtrl->Cur645DataLen;
		}
	}

	AMRCtrl->Cur645_DI_Step++;
	if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		return NO_ERROR;
	}

	AMRCtrl->CurNW_DI_Step++;
	while(Get_NextAMR_NW_DI(AMRCtrl) == NO_ERROR)
	{
		if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
		{
			AMRCtrl->State = AMR_Tx;
			return NO_ERROR;
		}
		AMRCtrl->CurNW_DI_Step++;
	}

	StoreNormalTaskAMRData(PORTn, AMRCtrl);

	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}


ResultEnum RS485_AMR_RxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	ResultEnum Result = NO_ERROR;
	u32 RelayTaskID = 0, DataLen = 0;

	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	DataLen = Build645Frame(Frame, AMRCtrl->Cur645Data, NO_FE_IN_645);

	if(DataLen != 0)
	{
		HAL_GetSystemTime(&RealTime);
		RealTimeToYYTime(&RealTime, &YYTime);
		YYTime.Ack = 0;
		YYTime.Report = 0;
		RelayTaskID = 0xE0000401 + AMRCtrl->RelayTask->CurTaskID;
		NAND_WriteRelayTask(AMRCtrl->RelayTask->RelayType,RelayTaskID, &YYTime, AMRCtrl->Cur645Data, &DataLen);
	}

	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}


ResultEnum RS485_AMR_RxAlarmTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame)
{
	ResultEnum Result = NO_ERROR;
	AlarmAMRDataStruct* AlarmAMRData = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRData += AMRCtrl->MeterIndex;
	
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;

	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)
	{
		if(Get645FrameDataID(Frame, AMRCtrl->Protocol) != AMRCtrl->Cur645_DI) return ERROR_InvalidData;
		
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			StoreAlarmTaskAMRData_97(PORTn, AMRCtrl, Frame);
		}
		else
		{
			StoreAlarmTaskAMRData_07(PORTn, AMRCtrl, Frame);
		}
	}
	else //20190109 防止抄剩余金额，表回d1时，误报剩余金额不足
	{
		if(AMRCtrl->Cur645_DI==0x00900200)
			MemSet(AlarmAMRData->DI_00900200, 0xFF, 4);
	}

	AMRCtrl->Cur645_DI_Step++;
	if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		return NO_ERROR;
	}

	AMRCtrl->CurNW_DI_Step++;
	while(Get_NextAMR_NW_DI(AMRCtrl) == NO_ERROR)
	{
		if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
		{
			AMRCtrl->State = AMR_Tx;
			return NO_ERROR;
		}
		AMRCtrl->CurNW_DI_Step++;
	}

	AlarmAMRData->State[1] = 0x01;	//某个表的所有数据项都抄完再置1
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}


ResultEnum RS485_AMR_PauseProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	u16* pPortAckTimer;
	RS485PortParaStruct RS485PortPara;
	u32 FlashAddr = 0;
	ResultEnum Result = NO_ERROR;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	
	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] == 0)
	{
		AMRCtrl->State = AMR_Resume;
		return Result;
	}

	if((AMRCtrl->Flag & AMR_FLAG_RT_AMR_START) == 0) return Result;
	AMRCtrl->Flag &= ~AMR_FLAG_RT_AMR_START;

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
	MR((u32)&RS485PortPara, FlashAddr, sizeof(RS485PortParaStruct));
	if((FlashAddr == 0) || (RS485PortPara.Function != UART_FUNC_AMR))
	{
		Task->Flag |= RT_AMR_DATA_REFUSE;
		return Result;
	}

	if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{
		AMRCtrl->State = AMR_RealTimeTx;
		pPortAckTimer[0] = 0;
		if(Task->RequestCnt == 0x01) pPortAckTimer[0] = RT_AMR_485_FIRST_TIME_DELAY;
		RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_BCAST);
	}
	else if(Task->SrcDataType == DATA_TYPE_FRAME)
	{
		AMRCtrl->State = AMR_RealTimeTx;
		pPortAckTimer[0] = 0;
		if(Task->RequestCnt == 0x01) pPortAckTimer[0] = RT_AMR_485_FIRST_TIME_DELAY;
		RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_CONTROL);
	}
	else if(Task->SrcDataType == DATA_TYPE_RELAY)
	{
		AMRCtrl->State = AMR_RealTimeTx;
		pPortAckTimer[0] = 0;
		if(Task->RequestCnt == 0x01) pPortAckTimer[0] = RT_AMR_485_FIRST_TIME_DELAY;
		RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_RELAY);
	}
	else
	{
		if(Task->Protocol == DLT_645_PROTOCOL_07)
		{
			Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64507Resolve[Task->Cur645_DI_Step].u32Ident;
		}
		else //if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64597Resolve[Task->Cur645_DI_Step].u32Ident;
		
		}
		if(Task->Cur645_DI == 0xFFFFFFFF)
		{	
			Task->Flag |= RT_AMR_DATA_READY;
			AMRCtrl->State = AMR_Pause;
			pPortAckTimer[0] = AMR_PAUSE_MAX_TIME;
		}
		else
		{
			AMRCtrl->State = AMR_RealTimeTx;
			pPortAckTimer[0] = 0;
			if(Task->RequestCnt == 0x01) pPortAckTimer[0] = RT_AMR_485_FIRST_TIME_DELAY;
			RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_RT);
		}
	}
	
	return Result;
}


ResultEnum RS485_AMR_RealTimeTxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	u16* pPortAckTimer;
	UartRxChCtrlStruct* UartRxChCtrl;
	UartTaskCtrlStruct* pUTask;
	UartTxReqStruct UartTxReq;
	DLT645FrameStruct DLT645Frame;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	ResultEnum Result = NO_ERROR;
	u32 TxBuf = 0, TxLen = 0;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
	if((UartRxChCtrl != NULL) && (UartRxChCtrl->Empty != BUFFER_EMPTY))
	{
		UartRxChCtrl->Empty = BUFFER_EMPTY;
		pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(PORTn);
		if(pUTask != NULL)
		{
			pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
		}
	}

	MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));
	TxBuf = GetUartPortTxBuf(PORTn);
	if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{
		MR(TxBuf, (u32)&Task->SrcData[Task->SrcDataPos], Task->CurPnDataLen);
		UartTxReq.NeedAck = 0x00;
		TxLen = Task->CurPnDataLen;
	}
	else if(Task->SrcDataType == DATA_TYPE_FRAME)
	{
		MR(TxBuf, (u32)&Task->SrcData[Task->SrcDataPos], Task->CurPnDataLen);	
		UartTxReq.NeedAck = 0x00;
		TxLen = Task->CurPnDataLen;
	}
	else if(Task->SrcDataType == DATA_TYPE_RELAY)
	{
		MR(TxBuf, (u32)&Task->SrcData[Task->SrcDataPos], Task->CurPnDataLen);	
		UartTxReq.NeedAck = 0x01;
		TxLen = Task->CurPnDataLen;
	}
	else
	{
		MC(0, (u32)&DLT645Frame, sizeof(DLT645FrameStruct));
		MR((u32)DLT645Frame.Addr, (u32)Task->Addr, METER_ADDR_SIZE);
		if(Task->Protocol == DLT_645_PROTOCOL_07)
		{
			DLT645Frame.CtrlCode = DLT_07_READ;
			DLT645Frame.DataLen = DLT_645_07_ID_SIZE;
			DLT645Frame.Data[0] = Task->Cur645_DI;
			DLT645Frame.Data[1] = Task->Cur645_DI >> 8;
			DLT645Frame.Data[2] = Task->Cur645_DI >> 16;
			DLT645Frame.Data[3] = Task->Cur645_DI >> 24;
		}
		else //if(Task->Protocol == DLT_645_PROTOCOL_97)
		{
			DLT645Frame.CtrlCode = DLT_97_READ;
			DLT645Frame.DataLen = DLT_645_97_ID_SIZE;
			DLT645Frame.Data[0] = Task->Cur645_DI;
			DLT645Frame.Data[1] = Task->Cur645_DI >> 8;		
		}
		TxLen = Build645Frame(&DLT645Frame, (u8*)TxBuf, NO_FE_IN_645);
		UartTxReq.NeedAck = 0x01;
	}

	UartTxReq.SrcTask = Get_AMR_TaskID(PORTn);
	UartTxReq.FrameType = FRAME_TYPE_DLT_645;
	UartTxReq.RetryCnt = 0;
	UartTxReq.BpsCtrl = ConvertToBpsCtrl(Task->PortBaudRate, Task->PortParity, Task->PortDataBit, Task->PortStopBit);
	UartTxReq.TimeOut = RS485_AMR_TIME_OUT;
	UartTxReq.DataLen = TxLen;
	UartTxReq.DataBuf = (u8*)TxBuf;
	Result = UartPortTxReqNew(PORTn, &UartTxReq);
	
	if(Result != NO_ERROR) 
	{
		return Result;
	}

	pPortAckTimer[0] = RS485_AMR_TIME_OUT;

	if(Task->SrcDataType == DATA_TYPE_RELAY)
	{//err20190311 中继转发设置的超时时间大于最小超时时间时，则按设置的时间计算超时。
		if(Task->RelayTimeOut > RS485_AMR_TIME_OUT)
			pPortAckTimer[0] = Task->RelayTimeOut;
	}
	if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{//err20190312 广播校时4波特率下发 不需要等待接收ACK的过程 	
		pPortAckTimer[0] = 0;
		Task->Flag |= RT_AMR_DATA_READY;
	}

	AMRCtrl->State = AMR_RealTimeWaitRx;
	
	return Result;
}

ResultEnum RS485_AMR_WaitRealTimeRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	UartTaskCtrlStruct* pUTask;
	UartRxChCtrlStruct* UartRxChCtrl;
	DLT645FrameStruct DLT645Frame;
	
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	u32 DataLen = 0, i = 0, LeftLen = 0;
	//u32 FrameLen = 0;
	//u8 DataType = 0, FrameType = 0;
	//u8* FrameBuf = (u8*)GetUartPortRxBuf(PORTn, DATA_TYPE_FRAME);
	
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] == 0)
	{
		if(Task->NeedAck != 0)
		{
			Task->RetryCnt++;
			if(Task->RetryCnt >= RT_AMR_MAX_RETRY_CNT)
			{
				AMRCtrl->State = AMR_Pause;
				pAckTimer[0] = AMR_PAUSE_MAX_TIME;
				Task->Flag |= RT_AMR_DATA_READY;
			}
			else
			{
				AMRCtrl->State = AMR_RealTimeTx;
			}
		}
		else
		{
			AMRCtrl->State = AMR_Pause;
			pAckTimer[0] = AMR_PAUSE_MAX_TIME;
		}
		return Result;
	}

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
	if(UartRxChCtrl == NULL) return ERROR_POINTER_NULL;
	if(UartRxChCtrl->Empty == BUFFER_EMPTY) return ERROR_BUF_EMPTY;
	if(UartRxChCtrl->FrameBuf == NULL) return ERROR_POINTER_NULL;
	pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(PORTn);
	if(pUTask == NULL) return ERROR_POINTER_NULL;
	pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	UartRxChCtrl->Empty = BUFFER_EMPTY;
	if(UartRxChCtrl->FrameType != FRAME_TYPE_DLT_645) return ERROR_WRONG_PROTOCOL;
	
	//Result = UartPortRxReq(PORTn, FrameBuf, &FrameLen, &FrameType, &DataType);
	//if(Result != NO_ERROR) return Result;
	//if(FrameType != FRAME_TYPE_DLT_645) return ERROR_WRONG_PROTOCOL;

	Result = DLT645FrameDcompress(&DLT645Frame, UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen);
	if(Result != NO_ERROR) return Result;

	if(Task->SrcDataType == DATA_TYPE_STRUCT)
	{
		if(MemCompare(DLT645Frame.Addr, Task->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
		if(GetProtocolTypeByCtrlCode(DLT645Frame.CtrlCode & DLT_645_CODE_MASK) != Task->Protocol) return ERROR_InvalidData;

		if((DLT645Frame.CtrlCode & DLT_645_ERROR_BIT) == 0)
		{
			if(Get645FrameDataID(&DLT645Frame, Task->Protocol) != Task->Cur645_DI) return ERROR_InvalidData;
		}

		Result = Convert645ToNWAMRData(&DLT645Frame, Task->CurNW_DI_TblIndex, Task->Cur645_DI_Step, Task->Protocol, Task->AckData, &Task->AckDataLen);	
		Task->Cur645_DI_Step++;
		if(Task->Protocol == DLT_645_PROTOCOL_07)
		{
			Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64507Resolve[Task->Cur645_DI_Step].u32Ident;
		}
		else //if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64597Resolve[Task->Cur645_DI_Step].u32Ident;
		}
		if(Task->Cur645_DI == 0xFFFFFFFF)
		{	
			if((Task->Protocol == DLT_645_PROTOCOL_97) && (Task->AckDataLen > sizeof(QCSG_PnListStruct)))
			{
				DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].u16Dlen;
				
				if((u16)DataLen > (Task->AckDataLen - sizeof(QCSG_PnListStruct)))
				{	
					LeftLen = DataLen - (Task->AckDataLen - sizeof(QCSG_PnListStruct));
					for(i = 0; i < LeftLen; i++)
					{
						Task->AckData[Task->AckDataLen++] = 0xFF;
					}
				}
			}

			Task->Flag |= RT_AMR_DATA_READY;
			AMRCtrl->State = AMR_Pause;
			pAckTimer[0] = AMR_PAUSE_MAX_TIME;
		}
		else
		{
			AMRCtrl->State = AMR_RealTimeTx;
			pAckTimer[0] = 0;
		}
	}
	else if(Task->SrcDataType == DATA_TYPE_RELAY)
	{
		if(MemCompare(DLT645Frame.Addr, Task->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
		DataLen = UartRxChCtrl->FrameLen;
		Task->AckData[Task->AckDataLen++] = Task->RelayType;
		Task->AckData[Task->AckDataLen++] = DataLen;
		MR((u32)&Task->AckData[Task->AckDataLen], (u32)UartRxChCtrl->FrameBuf, DataLen);
		Task->AckDataLen += DataLen;

		Task->Flag |= RT_AMR_DATA_READY;
		AMRCtrl->State = AMR_Pause;
		pAckTimer[0] = AMR_PAUSE_MAX_TIME;
	}
	else if(Task->SrcDataType == DATA_TYPE_FRAME)
	{
		if(MemCompare(DLT645Frame.Addr, Task->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
		DataLen = UartRxChCtrl->FrameLen;
		MR((u32)&Task->AckData[Task->AckDataLen], (u32)UartRxChCtrl->FrameBuf, DataLen);
		Task->AckDataLen += DataLen;
		Task->Flag |= RT_AMR_DATA_READY;
		AMRCtrl->State = AMR_Pause;
		pAckTimer[0] = AMR_PAUSE_MAX_TIME;
	}
	else //if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{
		Task->Flag |= RT_AMR_DATA_READY;//err20190315 485完成广播校时任务就直接应答请求，否则等待超时需要5s。
		AMRCtrl->State = AMR_Pause;
		pAckTimer[0] = AMR_PAUSE_MAX_TIME;
	}

	return Result;
}



ResultEnum RS485_AMR_ResumeProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl)
{
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	#if ((Project/100)==9)//配变 实时任务加锁，防止负控代码进行抄表冲突。 此处完成实时任务，解锁
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RT = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RT;
	#endif
	
	
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
#if ((Project/100)==9)//配变 实时任务加锁，防止负控代码进行抄表冲突。 此处完成实时任务，解锁
	pstRDMeter_Ctrl_RT->u8Lock = FUNC_UNLOCK;
#endif
	AMRCtrl->State = AMRCtrl->OldState;
	if(AMRCtrl->OldState != AMR_Idle)
	{
		AMRCtrl->MeterIndex--;
		if(AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID)
		{
			AMRCtrl->NormalTask->PnListIndex--;
		}
		else if(AMRCtrl->AMR_ID_Step == AMR_RELAY_TASK_ID)
		{
			AMRCtrl->RelayTask->CurTaskID--;
		}
		else 
		{

		}
		Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
		if(Result == NO_ERROR)
		{
			AMRCtrl->State = AMR_Tx;
		}
		else
		{
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
		}
	}
	else
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
	}
	WarningCancel(PORTn);
	return Result;
}

#endif
/**************************************** (C) COPYRIGHT zhaoyun ***************************************END OF FILE****/


