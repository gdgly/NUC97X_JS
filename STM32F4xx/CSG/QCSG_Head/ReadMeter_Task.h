/*************************************************
版  权:正泰仪表智能终端部
文  件:ReadMeter_Task.h
作  者:苗晓楠
版  本:1.00
日  期:20170601
描  述:抄表任务文件
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __READMETER_TASK_H
#define __READMETER_TASK_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
#include "QCSG_PublicDefine.h"

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void RD_Meter_Task(void);

extern void RD_Time_TO_YYTime(TIME_PARAM_S* pstTime,RealTimeYYStruct* pstYYTime);

extern void RD_Write_ARD02_Data(u8 u8AlarmFlag,TIME_PARAM_S* pstTime,QCSG_RAM_DATA_S* pstRam_Data,ARD02Struct* pstARD02Data);




#endif /* __READMETER_TASK_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
