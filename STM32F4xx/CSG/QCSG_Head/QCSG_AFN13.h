/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN13.h
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网AFN13处理文件
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_AFN13_H
#define __QCSG_AFN13_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_PublicDefine.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define ALARM_DI_NUM				(78)
extern const DIDataInfoStruct AlarmDIInfoTbl[ALARM_DI_NUM];


u16 FindDIInAlarmDIInfoTbl(u32 DI);

ResultEnum QCSG_AFN13_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);



#endif /* __QCSG_AFN13_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
