/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_Resolve.h
��  ��:mxn
��  ��:1.00
��  ��:20170601
��  ��:����Э������ļ�
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_RESOLVE_H
#define __QCSG_RESOLVE_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ResultEnum QCSG_Up_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask);


ResultEnum QCSG_AFN00_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);

ResultEnum QCSG_FrameDecompress(u8* FrameBuf, u16 FrameLen, QCSGFrameStruct* Frame);

ResultEnum QCSG_AddrCheck(QCSGFrameStruct* Frame);

ResultEnum QCSG_TpvCheck(QCSGFrameStruct* Frame);

ResultEnum QCSG_PwCheck(QCSGFrameStruct* Frame);


#endif /* __QCSG_RESOLVE_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
