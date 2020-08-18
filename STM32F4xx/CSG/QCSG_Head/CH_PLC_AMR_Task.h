/*************************************************
版  权:正泰仪表智能终端部
文  件:CH_PLC_AMR_Task.h
作  者:zhaoyun
版  本:1.00
日  期:20170812
描  述:南网PLC载波模块抄表处理文件
*************************************************/

void PLC_ModuleManage(u32 PORTn);

void PLC_RT_AMR_ReqCheck(u32 PORTn, PLCManageStruct* PLCManage);

void PLC_PowerUpInit(u32 PORTn, PLCManageStruct* PLCManage);

void PLCModuleGPIOInit(u32 PORTn);

void PLCModulePowerUpInit(u32 PORTn);

void PLC_ResetInit(u32 PORTn, PLCManageStruct* PLCManage);

void PLCModuleResetInit(u32 PORTn);

void PLCModuleResetRelease(u32 PORTn);

void PLC_ModleRecognizeProcess(u32 PORTn, PLCManageStruct* PLCManage);

ResultEnum PLC_TxReadRunMode(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_TxInitFile(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_TxInitTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_TxSetMasterNodeAddr(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_TxReadMasterNodeAddr(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

void PLC_ModleFileSync(u32 PORTn, PLCManageStruct* PLCManage);

void PLC_FileSyncSetOkFlag(u8* MeterList, u32 MeterListNum, NodeListStruct* NodeList);

ResultEnum PLC_FileSyncTxReadNodeNum(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_FileSyncTxAddNode(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_FileSyncTxReadNodeInfo(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_FileAddNodeTxAddNode(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

void PLC_ModleFileAddNode(u32 PORTn, PLCManageStruct* PLCManage);


void PLC_ModleAMR_Task(u32 PORTn, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_IdleProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, RealTimeStruct* RealTime);

ResultEnum PLC_AMR_TxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_AddTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_ADD_TASK_TxStopTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_AMR_ADD_OneTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_AMR_ADD_TASK_SwitchToNextTaskID(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_ADD_TASK_TxStartTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

//ResultEnum PLC_AMR_TxNormalTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_ADD_OneNormalTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

//ResultEnum PLC_AMR_TxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_ADD_OneRelayTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_AMR_WaitRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RxTimeOutProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RxDayFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RxHourFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RxNormalTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RxAlarmTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage);


ResultEnum PLC_AMR_PauseProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RealTimeTxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_RealTimeAddTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_AMR_WaitRealTimeRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

ResultEnum PLC_AMR_ResumeProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage);

void PLC_CheckModle(u32 PORTn, PLCManageStruct* PLCManage);

void PLC_YBWHZ_Test(u32 PORTn, PLCManageStruct* PLCManage);


ResultEnum PLC_TxCheckUnfinishTaskNum(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_TxCheckRemainTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN02_E8000203_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN02_E8000206_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum PLC_Tx_YBWHZ_Test(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame);

ResultEnum QCSG_LocalModuleFrameProcess(u32 PORTn, PLCManageStruct* PLCManage);

ResultEnum LocalFrame_AFN00_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN01_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN02_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8000301_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8000302_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8000303_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8000305_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8000307_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8040304_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8040306_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8040308_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8040311_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8040312_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN03_E8040321_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN04_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN05_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN05_E8050501_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);
ResultEnum LocalFrame_AFN05_E8050502_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);
ResultEnum LocalFrame_AFN05_E8050503_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);
ResultEnum LocalFrame_AFN05_E8050504_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);
ResultEnum LocalFrame_AFN05_E8050505_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);


ResultEnum LocalFrame_AFN06_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

ResultEnum LocalFrame_AFN07_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame);

void ConvertTerminalAddrToBCD(u32 TerminalAddr, u32 TerminalLocation, u8* BCDAddr);

void DisplayRouterVer(u32 PORTn);

ResultEnum QCSG_LocalFrameDecompress(u8* FrameBuf, u16 FrameLen, QCSGLocalFrameStruct* Frame);

u32 BuildLocalModuleFrame(u8* FrameBuf, QCSGLocalFrameStruct* Frame);

u16 CompareMeterAddrInNodeList(u8* MeterAddrStart, u16 MeterNum, NodeListStruct* NodeList, u16 NodeListNum);

u16 GetNextBPS(u16 CurBps, u16 BpsSelectBit);

/**************************************** (C) COPYRIGHT zhaoyun **********************************END OF FILE****/


