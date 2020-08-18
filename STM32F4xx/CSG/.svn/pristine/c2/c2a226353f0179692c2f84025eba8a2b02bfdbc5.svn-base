/*************************************************
版  权:正泰仪表智能终端部
文  件:CH_RS485_AMR_Task.h
作  者:zhaoyun
版  本:1.00
日  期:20170819
描  述:南网 RS485 通道抄表处理文件
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
#include "../QCSG_Head/QCSG_AFN12.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
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


void CH_RS485_AMR_Task(u32 PORTn);

ResultEnum RS485_AMR_IdleProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* RealTime);

ResultEnum RS485_AMR_TxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_Build645AndTxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_TxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_WaitRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_RxTimeOutProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_RxDayFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

ResultEnum RS485_AMR_RxHourFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

ResultEnum RS485_AMR_RxNormalTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

ResultEnum RS485_AMR_RxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

ResultEnum RS485_AMR_RxAlarmTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

ResultEnum RS485_AMR_PauseProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_RealTimeTxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_WaitRealTimeRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum RS485_AMR_ResumeProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl);






/**************************************** (C) COPYRIGHT zhaoyun ***************************************END OF FILE****/


