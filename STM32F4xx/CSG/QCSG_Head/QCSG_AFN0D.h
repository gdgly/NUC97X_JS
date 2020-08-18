/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0D.h
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网AFN0D处理文件
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_AFN0D_H
#define __QCSG_AFN0D_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_PublicDefine.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//extern u8 QCSG_AFN0D_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);

extern ResultEnum QCSG_AFN0D_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);

#endif /* __QCSG_AFN0D_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
