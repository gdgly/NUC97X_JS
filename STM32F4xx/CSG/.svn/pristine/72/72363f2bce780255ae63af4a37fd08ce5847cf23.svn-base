/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_Hal.h
作  者:mxn
版  本:1.00
日  期:20170601
描  述:硬件抽象层头文件
*************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_HAL_H
#define __QCSG_HAL_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_UpLinkProtocol.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void HAL_ChNoSendData(u8 u8ChNo,u8 u8SendLen,u8* pu8SendBuf);
extern void HAL_ChNoRevData(u8 u8ChNo,u8 u8MaxRevBufLen,u8* pu8RevLen,u8* pu8RevBuf);
extern u32 HAL_GetSystemTick(void);

extern u32 HAL_GetSystemSecond(void);

extern u8 HAL_GetSystemTime(RealTimeStruct* pTime);

extern u8 HAL_GetBeforePoweroffTime(RealTimeYYStruct* pTime);


// 不循环1-10
// 蜂鸣模式库1,按键声
// 蜂鸣模式库2,参数设置声
// 蜂鸣模式库3,控制时段变化声
// 循环11-N
// 蜂鸣模式库11,崔费告警声
// 蜂鸣模式库12,允许合闸声
// 蜂鸣模式库13,功控告警声
// 蜂鸣模式库14,电控告警声
// 蜂鸣模式库15,遥控跳闸告警声
#define BUZZ_MODE_00			(0)
#define BUZZ_MODE_KEY			(1)
#define BUZZ_MODE_RW_PARA		(2)
#define BUZZ_MODE_CTRL_TIME		(3)
#define BUZZ_MODE_11			(11)
#define BUZZ_MODE_12			(12)
#define BUZZ_MODE_13			(13)
#define BUZZ_MODE_14			(14)
#define BUZZ_MODE_15			(15)

extern void HAL_BuzzerON(u32 Mode, u32 Time);

extern void HAL_BuzzerOFF(u32 Mode);

extern void HAL_PinReverse(u32 PinName);
extern u32 HAL_GetPortLinkState(u32 PORTn);

#endif /* __QCSG_HAL_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
