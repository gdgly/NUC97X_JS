/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_AFN12.h
��  ��:mxn
��  ��:1.00
��  ��:20170601
��  ��:����AFN12�����ļ�
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
