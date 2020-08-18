/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_Hal.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:硬件抽象层
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../Device/RTC.h"
#include "../Device/IC_BUZZ.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**--------------------------------------------------------------------
*功    能:通道发送数据
*输入参数:u8ChNo：通道编号，u16SendLen：发送长度，pu8SendBuf：发送缓冲
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void HAL_ChNoSendData(u8 u8ChNo,u8 u8SendLen,u8* pu8SendBuf)
{

}
/**--------------------------------------------------------------------
*功    能:通道接收数据
*输入参数:u8ChNo：通道编号,u16MaxRevBufLen:输出缓冲最大长度
*输出参数:pu16RevLen:接收长度，pu8RevBuf：接收数据缓冲
*返    回:无
--------------------------------------------------------------------**/
void HAL_ChNoRevData(u8 u8ChNo,u8 u8MaxRevBufLen,u8* pu8RevLen,u8* pu8RevBuf)
{

}
/**--------------------------------------------------------------------
*功    能:获取当前系统节拍值，10ms一个节拍
*输入参数:无
*输出参数:无
*返    回:当前节拍值
--------------------------------------------------------------------**/
u32 HAL_GetSystemTick(void)
{
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
    return pTerminal_Ram->Ticks;
}

u32 HAL_GetSystemSecond(void)
{
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
    return pTerminal_Ram->Seconds;
}
/**--------------------------------------------------------------------
*功    能:获取当前时间
*输入参数:无
*输出参数:pstTime
*返    回:读取是否成功
--------------------------------------------------------------------**/
u8 HAL_GetSystemTime(RealTimeStruct* pTime)
{
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	//RX8025T_Read((u32)pComm_Ram->RTCBuff, 0, 7);
	pTime->u8Second	= pComm_Ram->RTCBuff[0];
	pTime->u8Minute	= pComm_Ram->RTCBuff[1];
	pTime->u8Hour	= pComm_Ram->RTCBuff[2];
	
	pTime->u8Day	= pComm_Ram->RTCBuff[4];
	pTime->u8Month	= pComm_Ram->RTCBuff[5];
	pTime->u8Year	= pComm_Ram->RTCBuff[6];

	pTime->u8Week	= pComm_Ram->RTCBuff[3];
	
    return TRUE;
}
/**--------------------------------------------------------------------
*功    能:获取掉电前时间
*输入参数:无
*输出参数:pstTime
*返    回:读取是否成功
--------------------------------------------------------------------**/
u8 HAL_GetBeforePoweroffTime(RealTimeYYStruct* pTime)
{
	u8* PoweroffTime = (u8*)ADDR_TFM_YMDHMS;
	pTime->Report   = PoweroffTime[0];
	pTime->Minute	= PoweroffTime[1];
	pTime->Hour		= PoweroffTime[2];
	pTime->Day		= PoweroffTime[3];
	pTime->Month	= PoweroffTime[4];
	pTime->YearL 	= PoweroffTime[5];
	pTime->YearH	= PoweroffTime[6];

    return TRUE;
}


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
// Time,时间以10ms为单位递减
void HAL_BuzzerON(u32 Mode, u32 Time)
{
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	if(Mode == 0)
	{
		pTerminal_Ram->BUZZMSTimer = Time;
		pComm_Ram->BUZZCTRL[8] = 0x00;
	}
	else
	{
		pComm_Ram->BUZZCTRL[8] = 0x01;
		BUZZ_Generate(Mode);
		if(Mode >= BUZZ_MODE_11)
		{
			pTSysCtrl->BuzzMode = Mode;
			pTSysCtrl->BuzzOnTime = Time;
			pTSysCtrl->BuzzOnTick = pTerminal_Ram->Ticks;
		}
	}
}

void HAL_BuzzerOFF(u32 Mode)
{
	//Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;

	BUZZ_Cancel(Mode);
	pTSysCtrl->BuzzOnTime = 0;
	pTSysCtrl->BuzzMode = 0;
	//pComm_Ram->BUZZCTRL[8] = 0;
}

void HAL_PinReverse(u32 PinName)
{
	if(Pin_Read(PinName) != 0)
	{
		Pin_OutL(PinName);
	}
	else
	{
		Pin_OutH(PinName);
	}
}

u32 HAL_GetPortLinkState(u32 PORTn)
{
	u32 LinkState = 0;
	UARTCtrl_TypeDef* UARTCtrl = NULL;
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->LinkTask == 101) LinkState = 1;
	return LinkState;
}

/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


