/*************************************************
版  权:正泰仪表智能终端部
文  件:CH_RS485_AMS_Task.h
作  者:luxiaowen
版  本:1.00
日  期:20190310
描  述:南网 RS485 通道搜表处理文件
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

void CH_RS485_AMS_Task(u32 PORTn);

ResultEnum RS485_AMS_IdleProcess(u32 PORTn,AMSCtrlStruct* Task, RealTimeStruct* RealTime);

ResultEnum RS485_AMS_TxProcess(u32 PORTn,AMSCtrlStruct* Task);

ResultEnum RS485_AMS_WaitRxProcess(u32 PORTn,AMSCtrlStruct* Task);

ResultEnum RS485_AMS_PauseProcess(u32 PORTn,AMSCtrlStruct* Task);

ResultEnum RS485_AMS_ResumeProcess(u32 PORTn,AMSCtrlStruct* Task);

ResultEnum SaveAMS645MeterParamAndE2010014Event(u32 PORTn, AMSCtrlStruct* Task, u8* TrueAddr);

ResultEnum BuildAMS645FrameStruct(DLT645FrameStruct* Frame, AMSCtrlStruct* Task);

ResultEnum Get_RS485Port_Function(u32 PORTn);

ResultEnum Get_NextBaudRate_ID(AMSCtrlStruct* Task);

ResultEnum Get_NextAmsMeter_ID(AMSCtrlStruct* Task);

/**************************************** (C) COPYRIGHT luxiaowen ***************************************END OF FILE****/


