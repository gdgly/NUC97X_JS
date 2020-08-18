/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN12.h
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网AFN12处理文件
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_AFN12_H
#define __QCSG_AFN12_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_PublicDefine.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ResultEnum QCSG_AFN12_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);

u32 QCSG_AFN12_GetNormalTaskData(QCSGFrameProcessStruct* pQCSGF, NormalTaskParaStruct* TaskPara, u8* PnListDataBuf, u32* TotalPnListNum, u32 PnListDataMaxLen);

u32 FindDIDataInAMRData(u8* AMRData, u32 AMRDataLen, u32 DI);

u32 QCSG_AFN12_GetRelayTaskData(QCSGFrameProcessStruct* pQCSGF, RelayTaskParaStruct* TaskPara, u8* PnListDataBuf, u32* TotalPnListNum, u32 PnListDataMaxLen);



#endif /* __QCSG_AFN12_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
