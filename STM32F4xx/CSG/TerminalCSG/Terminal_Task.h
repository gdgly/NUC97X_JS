/*************************************************
版  权:正泰仪表智能终端部
文  件:Terminal_Task.h
作  者:zhaoyun
版  本:1.00
日  期:20170720
描  述:终端任务接口
*************************************************/

void TerminalReportTask(u32 PORTn);

ResultEnum ReportTaskIdleStateProcess(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskReadDataProcess(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskTxProcess(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskWaitAckProcess(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskReadNormalTaskData(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskReadRelayTaskData(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskReadAlarmData(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum ReportTaskTxData(u32 PORTn, ReportTaskStruct* ReportTask);

ResultEnum GetNextNormalTaskToRead(ReportTaskStruct* ReportTask);

ResultEnum GetNextRelayTaskToRead(ReportTaskStruct* ReportTask);

ResultEnum GetNextReportStep(ReportTaskStruct* ReportTask);

void TerminalReportCheckNormalTask(ReportTaskStruct* ReportTask, RealTimeStruct* CurTime);

void TerminalReportCheckRelayTask(ReportTaskStruct* ReportTask, RealTimeStruct* CurTime);

void TerminalReportCheckAlarm(TerminalRunTimeCtrlStruct* pTSysCtrl);

void MultiFrameSendTask(u32 PORTn);

ResultEnum MultiFrameSendTaskIdleProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask);

ResultEnum MultiFrameSendTaskSendProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask);

ResultEnum MultiFrameSendTaskWaitAckProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask);

ResultEnum MultiFrameSendTaskDoneProcess(u32 PORTn, MultiFrameSendTaskStruct* SendTask);

u32 BuildFrameFromMultiFrameTask(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* QCSGFrame);

u32 QCSGBuildNormalTaskFrameStruct(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* DstFrame);

u32 QCSGBuildRelayTaskFrameStruct(u32 MaxTxBufLen, MultiFrameSendTaskStruct* Task, QCSGFrameStruct* DstFrame);

ResultEnum QCSGMultiFrameSendReq(u32 DstPort, MultiFrameSendReqStruct* MultiFrameSendReq);

ResultEnum QCSGSingleFrameSendReq(u32 DstPort, MultiFrameSendReqStruct* MultiFrameSendReq);


ResultEnum UartPortTxReq(TaskIDEnum SrcTask, u32 DstPort, u8* FrameBuf, u32 DataLen, u32 Option, u32 BpsCtrl, u16 TimeOut);

ResultEnum UartPortTxReqNew(u32 DstPort, UartTxReqStruct* UartTxReq);

ResultEnum UartPortRxReq(u32 SrcPort, u8* FrameBuf, u32* DataLen, u8* FrameType, u8* DataType);

ResultEnum SetRealTimeAMRTaskReq(TaskIDEnum SrcTask, u32 SrcPort, QCSG_RTREADMETER_REQ_INFO_S* RT_AMRReq);

UartTaskCtrlStruct* GetUartTaskCtrl(u32 PORTn);

u32 GetUartPortRxChCtrl(u32 PORTn);

u32 GetUartPortRxBuf(u32 PORTn, u8 DataType);

u32 GetUartPortRxBufLen(u32 PORTn);

u32 GetUartPortTxBuf(u32 PORTn);

u32 GetUartPortTxBufLen(u32 PORTn);


TaskIDEnum GetUartTaskID(u32 PORTn);

TaskIDEnum GetUartMultiFrameSendTaskID(u32 PORTn);

u32 GetUartMultiFrameSendTaskAddr(u32 PORTn);

u32 GetTerminalTaskCtrlAddr(TaskIDEnum Task);

u32 GetUartPortAckTimerAddr(u32 PORTn);

u32 GetTerminalPower_S_TimerValue(void);

u32 GetTerminalReportTaskAddr(u32 PORTn);

u32 GetUart_QCSG_DataBufAddr(u32 PORTn);

u32 GetUart_QCSG_DataBufLen(u32 PORTn);

u32 GetUart_QCSG_SEQ(u32 PORTn);

u32 GetUartHostAckState(u32 PORTn);

void UartResetHostAckState(u32 PORTn);

ResultEnum QCSG_LogToRS232Port(u32 PORTn, u32 SrcFrameAddr, u32 LogLen);


/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/



