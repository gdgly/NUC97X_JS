/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_Ctrl.c
作  者:mxn
版  本:1.00
日  期:20170601
描  述:负控控制文件
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../MS/MS.h"
#include "../Hcsg/ProjectCSG.h"
#include "../Hcsg/Pin_ProjectCSG_3xx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/ReadMeter_Task.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_Publicfunc.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Terminal_Task.h"

#if ((Project/100)==3)	//负控终端
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_SWITCH1				0x01
#define LED_SWITCH2				0x02
#define LED_SWITCH3				0x04
#define LED_SWITCH4				0x08
#define LED_ALARM				0x10
#define LED_HOLDPOWER			0x20
#define LED_POWERCTRL			0x40
#define LED_ENERGYCTRL			0x80
#define POWER_IDLE_STEP			0x00
#define POWER_OCCUR_STEP		0x01
#define POWER_RESTORE_STEP		0x02
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**--------------------------------------------------------------------
*功    能:获取终端保电状态，两种特殊情况下的保电，非协议设置的保电状态
*输入参数:pstCtrl_Param_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
u8 GetPowerHoldState(QCSG_CTRL_INFO_S* pstCtrl_Info,LoadManagementParaStruct* pstCtrl_Param_Info)
{	
	u8 u8Timeout = 15;
	
#if defined(FKGA43_CL3200_TEST)
	u8Timeout = 2;
#endif
	//终端上电15min内，为保电状态
	if(pstCtrl_Info->u8PowerUpFlag == 0)
	{
		if(pstCtrl_Info->u32ReadCtrlParamTick < 6000 * u8Timeout)
		{
			pstCtrl_Info->u8PowerHoldState = 1;
		}
		else
		{
			pstCtrl_Info->u8PowerUpFlag = 0x01;
		}
	}
	//与主站持续断线，保电状态为1，此部分待补充
	if(pstCtrl_Info->u8PowerHoldState == 0x01)
	{
		//保电状态，清除所有终端控制状态
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;
		pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;		
		pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
		pstCtrl_Info->u8EnergyAlarmFlag = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;
		pstCtrl_Info->u8YKCtrlSwitchState = 0;
		pstCtrl_Info->u8YKAlarmFlag = 0;
		pstCtrl_Info->u32YKAlarmTimeoutTick = 0;		
		pstCtrl_Info->u8YKState = 0;
		QCSG_WriteTerminal_State(pstCtrl_Info->u8YXState,pstCtrl_Info->u8YKState);
		Pin_OutH(PIN_SWITCH_OUT1);
		Pin_OutH(PIN_SWITCH_OUT2);
		Pin_OutH(PIN_SWITCH_OUT3);
		Pin_OutH(PIN_SWITCH_OUT4);
		Pin_OutH(PIN_ALARM_OUT);
		//点亮保电灯
		pstCtrl_Info->u8LedState |= LED_HOLDPOWER;
		//关闭告警灯
		pstCtrl_Info->u8LedState &= (~LED_ALARM);
		//关闭轮次灯
		pstCtrl_Info->u8LedState &= (~LED_SWITCH1);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH2);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH3);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH4);		
		return TRUE;
	}
	//关闭保电灯
	pstCtrl_Info->u8LedState &= (~LED_HOLDPOWER);
	return FALSE;
}
/**--------------------------------------------------------------------
*功    能:更新控制结构体信息
*输入参数:pstCtrl_Param_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Update_Ctrl_Info(LoadManagementParaStruct* pstCtrl_Param_Info,QCSG_CTRL_INFO_S* pstCtrl_Info,QCSG_CTRL_POWERDOWN_SAVE_INFO_S* pstCtrl_PowerDown_Save_Info)
{
	TIME_PARAM_S stSysCurrTime;
	u8 u8Time_Hour = 0,u8Time_Min = 0,u8FloatCoeff = 0,u8TempCtrlChange = 0,u8TimeCtrlChange = 0,u8EnergyChange = 0;
	u32 u32ReadLen = 0,u32LimitPower = 0,u32MaxEnergy_Month = 0,u32RoundEnergy_Month = 0;
	
	HAL_GetSystemTime(&stSysCurrTime);
	if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) != NO_ERROR)
	{
		return;
	}	
	//更新遥信开关属性
	if((pstCtrl_Param_Info->SwitchNodeType[1] != 0x00) && (pstCtrl_Param_Info->SwitchNodeType[1] != 0x01))
	{
		pstCtrl_Info->u8Switch1Attr = 0x00;
	}
	else
	{
		pstCtrl_Info->u8Switch1Attr = pstCtrl_Param_Info->SwitchNodeType[1];
	}
	if((pstCtrl_Param_Info->SwitchNodeType[2] != 0x00) && (pstCtrl_Param_Info->SwitchNodeType[2] != 0x01))
	{
		pstCtrl_Info->u8Switch2Attr = 0x00;
	}
	else
	{
		pstCtrl_Info->u8Switch2Attr = pstCtrl_Param_Info->SwitchNodeType[2];
	}
	if((pstCtrl_Param_Info->SwitchNodeType[3] != 0x00) && (pstCtrl_Param_Info->SwitchNodeType[3] != 0x01))
	{
		pstCtrl_Info->u8Switch3Attr = 0x00;
	}
	else
	{
		pstCtrl_Info->u8Switch3Attr = pstCtrl_Param_Info->SwitchNodeType[3];
	}
	if((pstCtrl_Param_Info->SwitchNodeType[4] != 0x00) && (pstCtrl_Param_Info->SwitchNodeType[4] != 0x01))
	{
		pstCtrl_Info->u8Switch4Attr = 0x00;
	}
	else
	{
		pstCtrl_Info->u8Switch4Attr = pstCtrl_Param_Info->SwitchNodeType[4];
	}
	//更新保电状态
#if defined(FKGA43_CL3200_TEST)
	if((pstCtrl_Info->u8PowerHoldState == 1)&&(pstCtrl_Param_Info->PowerHoldState == 0))
	{
		//为过台体保电测试，此处增加如果解除保电，原遥控跳闸改为全合
		MemSet(pstCtrl_Param_Info->OutputSwitchState,0,sizeof(pstCtrl_Param_Info->OutputSwitchState));
		u32ReadLen = sizeof(LoadManagementParaStruct);
		NAND_WritePara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen);		
		pstCtrl_Info->u8YKCtrlSwitchState = 0;
		pstCtrl_Info->u8YKSwitchOutState = 0;
#if (TEST_ON == 1)//调试台体保电测试
		pstCtrl_Info->u8YKState = 0;
		QCSG_WriteTerminal_State(pstCtrl_Info->u8YXState,pstCtrl_Info->u8YKState);
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;
		pstCtrl_Info->u8YKSwitchOutState = 0;
		//pstCtrl_Info->u8YKCtrlSwitchState = 0;
#endif
	}
#endif
	pstCtrl_Info->u8PowerHoldState = pstCtrl_Param_Info->PowerHoldState;
	//告警超时
	pstCtrl_Info->u32AlarmTimeout = QCSG_BCD2HEX(pstCtrl_Param_Info->AlarmDelayTime) * 60 * 100;	
	/**************************************月电控参数更新***************************************************/
	//更新电控有效标志
	if(pstCtrl_Info->u8EnergyCtrlFlag != pstCtrl_Param_Info->MonthKwhCtrlFlag)
	{
		u8EnergyChange = 1;		
	}
	pstCtrl_Info->u8EnergyCtrlFlag = pstCtrl_Param_Info->MonthKwhCtrlFlag;	
	//电量定值浮动系数
	u8FloatCoeff = pstCtrl_Param_Info->E0000C6C & 0x7F;
	if(QCSG_BCDCheck(&u8FloatCoeff,1) != TRUE)
	{
		pstCtrl_Info->u8EnergyFloatCoeff = 0;
	}
	else
	{
		if(pstCtrl_Info->u8EnergyFloatCoeff != pstCtrl_Param_Info->E0000C6C)
		{
			u8EnergyChange = 1;
		}
		pstCtrl_Info->u8EnergyFloatCoeff = pstCtrl_Param_Info->E0000C6C;
	}
	//电控累加标志
	if(pstCtrl_Info->u8EnergySumFlag != pstCtrl_Param_Info->KwhCtrlIncFlag)
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u8EnergySumFlag = pstCtrl_Param_Info->KwhCtrlIncFlag;
	//电控总加组
	if(memcmp(pstCtrl_Info->u16EnergySumPnList, pstCtrl_Param_Info->KwhCtrlPoint, sizeof(pstCtrl_Info->u16EnergySumPnList)) != 0)
	{
		u8EnergyChange = 1;
	}
	MemCopy(pstCtrl_Info->u16EnergySumPnList, pstCtrl_Param_Info->KwhCtrlPoint, sizeof(pstCtrl_Info->u16EnergySumPnList));
	//电控轮次有效标志
	if(pstCtrl_Info->u8EnergyCtrl_RoundFlag != (pstCtrl_Param_Info->MonthKwhCtrlRoundFlag & 0x0F))
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u8EnergyCtrl_RoundFlag = (pstCtrl_Param_Info->MonthKwhCtrlRoundFlag & 0x0F);
	//月电量限额
	u32MaxEnergy_Month = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00 >> 24)) * 1000000
									 + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00 >> 16)) * 10000
									 + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00 >> 8)) * 100
									 + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00));
	if(pstCtrl_Info->u32MaxEnergy_Month != u32MaxEnergy_Month)
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u32MaxEnergy_Month = u32MaxEnergy_Month;
	//轮次电量
	u32RoundEnergy_Month = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01 >> 24)) * 1000000
									   + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01 >> 16)) * 10000
									   + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01 >> 8)) * 100
									   + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01));
	if(pstCtrl_Info->u32RoundEnergy_Month != u32RoundEnergy_Month)
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u32RoundEnergy_Month = u32RoundEnergy_Month;
	/**************************************月电控参数结束***************************************************/
	/**************************************临时控参数更新***************************************************/
	//更新临时控有效标志，及临时控有效时间等
	if((pstCtrl_Param_Info->E0000C70 & 0x0F000000) != 0x00)		//取有效标识
	{
		if(pstCtrl_Info->u8TempRationPowerUpFlag == 0)
		{
			//刚上电,从存储的时间中取得临时限电起始时间
			pstCtrl_Info->u8TempRationPowerUpFlag = 1;
			pstCtrl_Info->u8TempRationFlag = 1;
			//更新临时限电起始时间，从记录的时间中
			if(TimeValidCheck(&pstCtrl_PowerDown_Save_Info->stTempRationStartTime) != NO_ERROR)
			{
				MemCopy(&pstCtrl_Info->stTempRationStartTime,&stSysCurrTime,sizeof(TIME_PARAM_S));
				MemCopy(&pstCtrl_PowerDown_Save_Info->stTempRationStartTime,&stSysCurrTime,sizeof(TIME_PARAM_S));	
			}
			else
			{
				MemCopy(&pstCtrl_Info->stTempRationStartTime,&pstCtrl_PowerDown_Save_Info->stTempRationStartTime,sizeof(TIME_PARAM_S));
			}
			pstCtrl_Info->u32SlipTick = 0;
			MemSet(pstCtrl_Info->s32MinutePowerList,0,sizeof(pstCtrl_Info->s32MinutePowerList));
			pstCtrl_Info->u32SlipCnt = 0;
			pstCtrl_Info->s32SlipPower = 0;
			pstCtrl_Info->u8SlipTimeoutFlag = 0;
		}
		if(pstCtrl_Info->u8TempRationFlag == 0x00)
		{
			//临时控制有效标志
			u8TempCtrlChange = 1;
			pstCtrl_Info->u8TempRationFlag = 0x01;
			//更新临时限电起始时间
			MemCopy(&pstCtrl_Info->stTempRationStartTime,&stSysCurrTime,sizeof(TIME_PARAM_S));
			MemCopy(&pstCtrl_PowerDown_Save_Info->stTempRationStartTime,&stSysCurrTime,sizeof(TIME_PARAM_S));
		}
	}
	else
	{
		if(pstCtrl_Info->u8TempRationPowerUpFlag == 0)
		{
			pstCtrl_Info->u8TempRationPowerUpFlag = 1;
		}
		if(pstCtrl_Info->u8TempRationFlag == 0x01)
		{
			u8TempCtrlChange = 1;			
		}
		pstCtrl_Info->u8TempRationFlag = 0;
	}
	//更新临时限电指标
	pstCtrl_Info->u8TempRationFloatCoeff = (u8)(pstCtrl_Param_Info->E0000C70>>16);	
	//更新临时限电时间
	u8Time_Min = QCSG_BCD2HEX((u8)pstCtrl_Param_Info->E0000C70);
	u8Time_Hour = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000C70>>8));
	pstCtrl_Info->u32TempRation_Time = u8Time_Hour * 3600 + u8Time_Min * 60;
	if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->E0000C6D,4) != TRUE)
	{
		//如果最低负荷没有设置，不进行临时控处理
		pstCtrl_Info->u8TempRationFlag = 0;
	}
	else
	{
		u32LimitPower = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000C6D >> 24)) * 1000000
									+ QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000C6D >> 16)) * 10000
									+ QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000C6D >> 8)) * 100
									+ QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000C6D));
		pstCtrl_Info->u32LimitPower = u32LimitPower;
	}
	//更新功率控制滑差时间
	if(QCSG_BCDCheck(&pstCtrl_Param_Info->E0000C71,1) != TRUE)
	{
		pstCtrl_Info->u8SlipTime = 15;
	}
	else
	{
		pstCtrl_Info->u8SlipTime = QCSG_BCD2HEX(pstCtrl_Param_Info->E0000C71);
		if((pstCtrl_Info->u8SlipTime > 15) || (pstCtrl_Info->u8SlipTime == 0))
		{
			pstCtrl_Info->u8SlipTime = 15;
		}
	}
#if defined(FKGA43_CL3200_TEST)
	//过CL3200南网台体，滑差时间更改为固定1分钟。
	pstCtrl_Info->u8SlipTime = 1;
#endif
	/**************************************临时控参数结束***************************************************/
	/**************************************时段控参数更新***************************************************/
	//更新功控有效标志
	pstCtrl_Info->u8PowerCtrlFlag = pstCtrl_Param_Info->PowerCtrlFlag;
	//过负荷持续时间
	if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->PowerOverLoadTime,4) != TRUE)
	{
		pstCtrl_Info->u32PowerOccurTimeout = 10 * 60 * 100;
	}
	else
	{
		pstCtrl_Info->u32PowerOccurTimeout = QCSG_BCD2HEX(pstCtrl_Param_Info->PowerOverLoadTime) * 60 * 100;
	}
	//负荷恢复时间
	if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->PowerResetTime,4) != TRUE)
	{
		pstCtrl_Info->u32PowerRestoreTimeout = 10 * 60 * 100;
	}
	else
	{
		pstCtrl_Info->u32PowerRestoreTimeout = QCSG_BCD2HEX(pstCtrl_Param_Info->PowerResetTime) * 60 * 100;
	}
	//功率定值浮动系数
	u8FloatCoeff = pstCtrl_Param_Info->E0000C6B & 0x7F;
	if(QCSG_BCDCheck(&u8FloatCoeff,1) != TRUE)
	{
		pstCtrl_Info->u8PowerFloatCoeff = 0;
	}
	else
	{
		pstCtrl_Info->u8PowerFloatCoeff = pstCtrl_Param_Info->E0000C6B;
	}
	if(memcmp(&pstCtrl_Info->stKwCtrl[0],&pstCtrl_Param_Info->KwCtrl[0],sizeof(KwCtrlStruct) * 4) != 0)
	{
		u8TimeCtrlChange = 1;
	}
	MemCopy(&pstCtrl_Info->stKwCtrl[0],&pstCtrl_Param_Info->KwCtrl[0],sizeof(KwCtrlStruct) * 4);
	/**************************************时段控参数结束***************************************************/
	/**************************************厂休控参数更新***************************************************/
	//更新厂休控有效标志
	pstCtrl_Info->u8FactoryRestFlag = pstCtrl_Param_Info->E0000E10;
	/**************************************厂休控参数结束***************************************************/
	/**************************************功控公共参数更新*************************************************/	
	//功控累加标志
	if(pstCtrl_Info->u8PowerSumFlag != pstCtrl_Param_Info->PowerCtrlIncFlag)
	{
		u8TempCtrlChange = 1;
		u8TimeCtrlChange = 1;
	}
	pstCtrl_Info->u8PowerSumFlag = pstCtrl_Param_Info->PowerCtrlIncFlag;
	//功控总加组
	if(memcmp(pstCtrl_Info->u16PowerSumPnList, pstCtrl_Param_Info->PowerCtrlPoint, sizeof(pstCtrl_Info->u16PowerSumPnList)) != 0)
	{
		u8TempCtrlChange = 1;
		u8TimeCtrlChange = 1;
	}
	MemCopy(pstCtrl_Info->u16PowerSumPnList, pstCtrl_Param_Info->PowerCtrlPoint, sizeof(pstCtrl_Info->u16PowerSumPnList));
	//功控轮次有效标志
	if(pstCtrl_Info->u8PowerCtrl_RoundFlag != (pstCtrl_Param_Info->PowerCtrlRoundFlag & 0x0F))
	{
		u8TempCtrlChange = 1;
		u8TimeCtrlChange = 1;
	}
	pstCtrl_Info->u8PowerCtrl_RoundFlag = (pstCtrl_Param_Info->PowerCtrlRoundFlag & 0x0F);
	/**************************************功控公共参数结束*************************************************/
	if(u8EnergyChange == 1)
	{
		pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
		pstCtrl_Info->u8EnergyAlarmFlag = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;
		pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy = 0;
		pstCtrl_PowerDown_Save_Info->u32RationEnergy = 0;		
	}
	if(u8TimeCtrlChange == 1)
	{
		//时段控参数变更
		if((pstCtrl_Info->u8PowerCtrlFlag == 1) && (pstCtrl_Info->u8TempRationFlag == 0))
		{
			pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
			pstCtrl_Info->u8PowerAlarmFlag = 0;
			pstCtrl_Info->u8PowerCtrlSwitchState = 0;				
		}
		pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;					
	}
	if(u8TempCtrlChange == 1)
	{
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;
		MemSet(pstCtrl_Info->s32MinutePowerList,0,sizeof(pstCtrl_Info->s32MinutePowerList));
	}
	if((pstCtrl_Info->u8TempRationFlag == 1) || (pstCtrl_Info->u8PowerCtrlFlag == 1))
	{
		pstCtrl_Info->u8LedState |= LED_POWERCTRL;
	}
	if(pstCtrl_Info->u8EnergyCtrlFlag == 1)
	{
		pstCtrl_Info->u8LedState |= LED_ENERGYCTRL;
	}		
}
/**--------------------------------------------------------------------
*功    能:更新控制结构体信息
*输入参数:pstCtrl_Param_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void RealTime_Update_Ctrl_Info(LoadManagementParaStruct* pstCtrl_Param_Info,QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8Time_Hour = 0,u8Time_Min = 0;
	u32 u32ReadLen = 0,u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	//门节点属性实时更新
	if(QCSG_GetDIChange(DI_E0000C00) == TRUE)	//如果参数有设置过
	{
		//为过台体门节点防抖，需要立即获取上行协议该参数，周期更新时就不需要再更新此参数
		QCSG_NoticeDIChange(DI_E0000C00,FALSE);
		if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) != NO_ERROR)
		{
			return;
		}	
		//更新遥信开关属性
		if((pstCtrl_Param_Info->SwitchNodeType[0] != 0x00) && (pstCtrl_Param_Info->SwitchNodeType[0] != 0x01))
		{
			pstCtrl_Info->u8Switch0Attr = 0x00;
		}
		else
		{
			if(pstCtrl_Info->u8Switch0Attr != pstCtrl_Param_Info->SwitchNodeType[0])
			{
				pstCtrl_Info->u32Switch0Timeout = u32CurrTick + 30 * 100;
			}
			pstCtrl_Info->u8Switch0Attr = pstCtrl_Param_Info->SwitchNodeType[0];
		}
	}
	//保电状态实时更新
	if(QCSG_GetDIChange(DI_E0000C50) == TRUE)
	{
		QCSG_NoticeDIChange(DI_E0000C50,FALSE);	
		if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) != NO_ERROR)
		{
			return;
		}
#if (TEST_ON == 0)//err20180628 保电测试修改
		pstCtrl_Info->u8PowerHoldState = pstCtrl_Param_Info->PowerHoldState;//为过台体保电测试，不能实时更新。在Update_Ctrl_Info中进行1min更新一次
#endif
	}	
	//远程遥控状态实时更新
	if((QCSG_GetDIChange(DI_E0000C2F) == TRUE) ||(QCSG_GetDIChange(DI_E0000C40) == TRUE))
	{
		if(QCSG_GetDIChange(DI_E0000C2F) == TRUE)
		{
			QCSG_NoticeDIChange(DI_E0000C2F,FALSE);	
		}
		if(QCSG_GetDIChange(DI_E0000C40) == TRUE)
		{
			QCSG_NoticeDIChange(DI_E0000C40,FALSE);	
		}
		if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) != NO_ERROR)
		{
			return;
		}
		//更新远程遥控状态
		if(((pstCtrl_Param_Info->OutputSwitchState[1] != 0)&&(pstCtrl_Param_Info->OutputSwitchState[1] != 1)) || (pstCtrl_Param_Info->OutputSwitchState[1] == 0))
		{
			pstCtrl_Info->u8YKSwitchOutState &= 0xFE;
		}
		if(((pstCtrl_Param_Info->OutputSwitchState[2] != 0)&&(pstCtrl_Param_Info->OutputSwitchState[2] != 1)) || (pstCtrl_Param_Info->OutputSwitchState[2] == 0))
		{
			pstCtrl_Info->u8YKSwitchOutState &= 0xFD;
		}	
		if(((pstCtrl_Param_Info->OutputSwitchState[3] != 0)&&(pstCtrl_Param_Info->OutputSwitchState[3] != 1)) || (pstCtrl_Param_Info->OutputSwitchState[3] == 0))
		{
			pstCtrl_Info->u8YKSwitchOutState &= 0xFB;
		}
		if(((pstCtrl_Param_Info->OutputSwitchState[4] != 0)&&(pstCtrl_Param_Info->OutputSwitchState[4] != 1)) || (pstCtrl_Param_Info->OutputSwitchState[4] == 0))
		{
			pstCtrl_Info->u8YKSwitchOutState &= 0xF7;
		}
		if(pstCtrl_Param_Info->OutputSwitchState[1] == 1)
		{
			pstCtrl_Info->u8YKSwitchOutState |= 0x01;
		}
		if(pstCtrl_Param_Info->OutputSwitchState[2] == 1)
		{
			pstCtrl_Info->u8YKSwitchOutState |= 0x02;
		}
		if(pstCtrl_Param_Info->OutputSwitchState[3] == 1)
		{
			pstCtrl_Info->u8YKSwitchOutState |= 0x04;
		}
		if(pstCtrl_Param_Info->OutputSwitchState[4] == 1)
		{
			pstCtrl_Info->u8YKSwitchOutState |= 0x08;
		}
		pstCtrl_Info->u8YKSwitchOutState &= 0x0F;
		//更新遥控轮次，没有按照协议轮次，协议设置轮次可能与型式规范轮次对不起来，以型式规范为准，轮次与输出开关配置保持一致
		pstCtrl_Info->u8YKCtrl_RoundFlag = pstCtrl_Info->u8YKSwitchOutState;
		//更新遥控关闭时间
		if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->RemoteSwitchOffTime,2) != TRUE)
		{
			pstCtrl_Info->u32YK_Timeout = 0;			
		}
		else
		{
			if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->RemoteSwitchOffTime,2) != TRUE)
			{
				pstCtrl_Info->u32YK_Timeout = 0;
			}
			else if(pstCtrl_Param_Info->RemoteSwitchOffTime == 0)
			{
				pstCtrl_Info->u32YK_Timeout = 0;
			}
			else
			{
				u8Time_Min = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->RemoteSwitchOffTime));
				u8Time_Hour = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->RemoteSwitchOffTime>>8));
				pstCtrl_Info->u32YK_Timeout = u32CurrTick + u8Time_Hour * 60 * 60 * 100 + u8Time_Min * 60 * 100;				
			}
		}
/*		
		pstCtrl_Info->u8YKCtrlSwitchState = 0;	
		pstCtrl_Info->u8YKAlarmFlag = 0;
		pstCtrl_Info->u32YKAlarmTimeoutTick = 0;
*/		
	}
}
/**--------------------------------------------------------------------
*功    能:获取测量点有功功率
*输入参数:u16Pn
*输出参数:ps32Power
*返    回:是否获取到
--------------------------------------------------------------------**/
u8 Get_Mp_Power(u16 u16Pn,s32* ps32Power)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	s32 s32Temp_Power = 0;
	u8 u8PnNum = 0,i = 0;
	
	u8PnNum = pstPn_Ram_Data->u8PnNum;
	for(i = 0;i < u8PnNum;i++)
	{
		//寻找内存中记录的测量点数据信息
		if(u16Pn == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
		{
			pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return FALSE;
	}
	if(QCSG_BCDCheck(pstRam_Data->u8Data0203FF00,3) != TRUE)
	{
		return FALSE;
	}
	MemCopy(&s32Temp_Power,pstRam_Data->u8Data0203FF00,3);
	if((s32Temp_Power & 0x00800000) != 0)
	{
		//功率为负数,灭掉符号位
		s32Temp_Power &= 0x7FFFFF;
		s32Temp_Power = QCSG_BCD2HEX((u8)(s32Temp_Power>>16)) * 10000 + QCSG_BCD2HEX((u8)(s32Temp_Power>>8)) * 100
						+ QCSG_BCD2HEX((u8)s32Temp_Power);
		s32Temp_Power = s32Temp_Power * (-1);
		
	}
	else
	{
		s32Temp_Power = QCSG_BCD2HEX((u8)(s32Temp_Power>>16)) * 10000 + QCSG_BCD2HEX((u8)(s32Temp_Power>>8)) * 100
						+ QCSG_BCD2HEX((u8)s32Temp_Power);		
	}
	(*ps32Power) = s32Temp_Power;
	return TRUE;
}
/**--------------------------------------------------------------------
*功    能:计算滑差功率
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:是否有测量点参与计算功率
--------------------------------------------------------------------**/
u8 Calc_Mp_SlipPower(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	MeasurePointStruct stPnPara;
	u8 i = 0,u8Pn_Flag = 0,u8Index = 0;
	s8 s8Sign = 1;
	u16 u16Pn = 0;
	u32 u32CurrTick = 0,u32ReadLen = 0;
	s32 s32Power = 0,s32SumPower = 0,s32SlipPower = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	// 1min获取一次测量点功率，不到1min不处理
	if((u32CurrTick - pstCtrl_Info->u32SlipTick) < 6000)
	{
		return TRUE;
	}
	pstCtrl_Info->u32SlipTick = u32CurrTick;
	//计算测量点累加功率
	for(i = 0;i < 8;i++)
	{
		u16Pn = pstCtrl_Info->u16PowerSumPnList[i];
		if(u16Pn == 0xFFFF)
		{
			continue;
		}		
		u16Pn = (u16)SwitchNumToPn(pstCtrl_Info->u16PowerSumPnList[i]);
		u8Pn_Flag = 1;
		//获取测量点功率
		if(Get_Mp_Power(u16Pn,&s32Power) != TRUE)
		{
			continue;
		}
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			continue;
		}
		if((stPnPara.CT == 0) || (stPnPara.PT == 0) || (stPnPara.CT == 0xFF) || (stPnPara.PT == 0xFF))
		{
			continue;
		}
		//获取该测量点累加标记，是加还是减
		if((pstCtrl_Info->u8PowerSumFlag & (1 << i)) == 0x00)
		{
			s8Sign = -1;
		}
		else
		{
			s8Sign = 1;
		}
		stPnPara.CT = QCSG_BCD2HEX((u8)(stPnPara.CT>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.CT);
		stPnPara.PT = QCSG_BCD2HEX((u8)(stPnPara.PT>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.PT);
		s32Power = s8Sign * s32Power * stPnPara.CT * stPnPara.PT;
		s32SumPower += s32Power;
	}
	if(u8Pn_Flag == 0)
	{
		return FALSE;
	}
	u8Index = pstCtrl_Info->u32SlipCnt % pstCtrl_Info->u8SlipTime;
	pstCtrl_Info->s32MinutePowerList[u8Index] = s32SumPower;
	pstCtrl_Info->u32SlipCnt++;
	if(pstCtrl_Info->u32SlipCnt <= pstCtrl_Info->u8SlipTime)
	{
		//未到一个滑差周期
		pstCtrl_Info->u8SlipTimeoutFlag = 0;
		return TRUE;
	}
	pstCtrl_Info->u8SlipTimeoutFlag = 1;
	pstCtrl_Info->u32SlipCnt = 0;
	//进行滑差功率计算
	for(i = 0;i < pstCtrl_Info->u8SlipTime;i++)
	{
		s32SlipPower += pstCtrl_Info->s32MinutePowerList[i];
	}
	s32SlipPower = s32SlipPower / pstCtrl_Info->u8SlipTime;
	pstCtrl_Info->s32SlipPower = s32SlipPower;
	return TRUE;
}
/**--------------------------------------------------------------------
*功    能:计算累加功率
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:是否有测量点参与计算功率
--------------------------------------------------------------------**/
u8 Calc_Mp_SumPower(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	MeasurePointStruct stPnPara;
	u8 i = 0,u8Pn_Flag = 0;
	s8 s8Sign = 1;
	u16 u16Pn = 0;
	u32 u32CurrTick = 0,u32ReadLen = 0;
	s32 s32Power = 0,s32SumPower = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	// 1 min获取一次测量点功率，不到1min不处理
	if((u32CurrTick - pstCtrl_Info->u32SumPowerTick) < 6000)
	{
		return TRUE;
	}
	pstCtrl_Info->u32SumPowerTick = u32CurrTick;
	//计算测量点累加功率
	for(i = 0;i < 8;i++)
	{
		u16Pn = pstCtrl_Info->u16PowerSumPnList[i];		
		if(u16Pn == 0xFFFF)
		{
			continue;
		}
		u16Pn = (u16)SwitchNumToPn(pstCtrl_Info->u16PowerSumPnList[i]);
		u8Pn_Flag = 1;
		//获取测量点功率
		if(Get_Mp_Power(u16Pn,&s32Power) != TRUE)
		{
			continue;
		}
		if(NAND_ReadMeasurePoint(u16Pn,0xE0800000,(u8*)&stPnPara,&u32ReadLen) != NO_ERROR)
		{
			continue;
		}
		if((stPnPara.CT == 0) || (stPnPara.PT == 0) || (stPnPara.CT == 0xFF) || (stPnPara.PT == 0xFF))
		{
			continue;
		}
		//获取该测量点累加标记，是加还是减
		if((pstCtrl_Info->u8PowerSumFlag & (1 << i)) == 0x00)
		{
			s8Sign = -1;
		}
		else
		{
			s8Sign = 1;
		}
		stPnPara.CT = QCSG_BCD2HEX((u8)(stPnPara.CT>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.CT);
		stPnPara.PT = QCSG_BCD2HEX((u8)(stPnPara.PT>>8)) * 100 + QCSG_BCD2HEX((u8)stPnPara.PT);
		s32Power = s8Sign * s32Power * stPnPara.CT * stPnPara.PT;
		s32SumPower += s32Power;
	}
	if(u8Pn_Flag == 0)
	{
		return FALSE;
	}
	pstCtrl_Info->s32SumPower = s32SumPower;
	return TRUE;
}
/**--------------------------------------------------------------------
*功    能:判断当前时间是否在控制时段内
*输入参数:pstCtrl_Info
*输出参数:pu32LimitValue
*返    回:时段是否有效
--------------------------------------------------------------------**/
u8 Judge_Ctrl_TimeInterval(QCSG_CTRL_INFO_S* pstCtrl_Info,u32* pu32LimitValue)
{
	TIME_PARAM_S stStartTime,stEndTime,stCurrTime;
	KwCtrlStruct stTimeCtrl_Info;
	u8 i = 0,j = 0,u8Res = FALSE,u8ValidFlag = FALSE,u8Pos = 0;
	u16 u16Smd = 0,u16Emd = 0,u16Shm = 0,u16Ehm = 0,u16Currhm = 0;
	u32 u32Res[4],u32DateFlag = 0;
	
	MemSet(&stStartTime,0,sizeof(TIME_PARAM_S));
	MemSet(&stEndTime,0,sizeof(TIME_PARAM_S));
	MemSet(&stCurrTime,0,sizeof(TIME_PARAM_S));
	for(i = 0;i < 4; i++)	// 4个功率控制集合
	{
		HAL_GetSystemTime(&stCurrTime);
		MemCopy(&stTimeCtrl_Info,&pstCtrl_Info->stKwCtrl[i],sizeof(KwCtrlStruct));
		if(QCSG_BCDCheck(&stTimeCtrl_Info.StartMonth,1) != TRUE)
		{
			continue;
		}
		if(QCSG_BCDCheck(&stTimeCtrl_Info.StartDay,1) != TRUE)
		{
			continue;
		}	
		if(QCSG_BCDCheck(&stTimeCtrl_Info.EndMonth,1) != TRUE)
		{
			continue;
		}
		if(QCSG_BCDCheck(&stTimeCtrl_Info.EndDay,1) != TRUE)
		{
			continue;
		}				
		//---------------------控制时间判断---------------------
		stStartTime.u8Year = stCurrTime.u8Year;
		u16Smd = stTimeCtrl_Info.StartMonth * 0x100 + stTimeCtrl_Info.StartDay;
		u16Emd = stTimeCtrl_Info.EndMonth * 0x100 + stTimeCtrl_Info.EndDay;
		if(u16Smd > u16Emd)
		{
			//防止配置的起始时间及结束时间跨年
			stEndTime.u8Year = QCSG_IncBCD(stCurrTime.u8Year);
		}
		else
		{
			stEndTime.u8Year = stCurrTime.u8Year;
		}
		stStartTime.u8Month = stTimeCtrl_Info.StartMonth;
		stStartTime.u8Day = stTimeCtrl_Info.StartDay;
		stEndTime.u8Month = stTimeCtrl_Info.EndMonth;
		stEndTime.u8Day = stTimeCtrl_Info.EndDay;
		if((QCSG_TimerSub(&stCurrTime,&stStartTime,u32Res) < 0) || (QCSG_TimerSub(&stCurrTime,&stEndTime,u32Res) > 0))
		{
			//小于起始时间，大于结束时间，说明时间未到，继续寻找下一个时段
			continue;
		}
		//在执行日期内，根据执行周期判断当天是否需要控制
		if((stTimeCtrl_Info.TI != 0x04)&&(stTimeCtrl_Info.TI != 0x05)&&(stTimeCtrl_Info.TI != 0x06))
		{
			//周期配置不正确
			continue;
		}
		switch(stTimeCtrl_Info.TI)
		{
			case 0x04:	//日
				if(stCurrTime.u8Day == stStartTime.u8Day)	//刚好当天是起始时间，即第一次执行
				{
					u8ValidFlag = TRUE;					
				}
				else
				{
					if(stTimeCtrl_Info.N0 == 1||stTimeCtrl_Info.N0 == 0)	//每日执行,N0=0的情况也归到这里，否则死循环
					{
						u8ValidFlag = TRUE;					
					}
					else
					{
						while(1)
						{
							for(j = 0;j < stTimeCtrl_Info.N0;j++)
							{
								QCSG_IncOneDay(&stStartTime);	//起始时间再过N0天后，是否为当前日
							}
							if((stCurrTime.u8Month == stStartTime.u8Month) && (stCurrTime.u8Day == stStartTime.u8Day))
							{
								u8ValidFlag = TRUE;	
								break;
							}
							if(QCSG_TimerSub(&stStartTime,&stEndTime,u32Res) >= 0)
							{
								//时间已经大于结束时间
								u8ValidFlag = FALSE;	
								break;
							}
						}
					}
				}
				break;
			case 0x06:	//周
				u32DateFlag = stTimeCtrl_Info.N0;
				u8Pos = QCSG_BCD2HEX(stCurrTime.u8Week);
				if(((u32DateFlag >> u8Pos) & 0x01) != 0)
				{
					u8ValidFlag = TRUE;
				}
				break;
			case 0x05:	//月
				u32DateFlag = stTimeCtrl_Info.N3 * 0x1000000 
							+ stTimeCtrl_Info.N2 * 0x10000
							+ stTimeCtrl_Info.N1 * 0x100
							+ stTimeCtrl_Info.N0;
				u8Pos = QCSG_BCD2HEX(stCurrTime.u8Day);
				if(((u32DateFlag >> (u8Pos -1)) & 0x01) != 0)
				{
					u8ValidFlag = TRUE;
				}
				break;
		}
		if(u8ValidFlag == FALSE)
		{
			continue;
		}
		//---------------------控制时段判断---------------------
		u8Res = FALSE;
		for(j = 0;j < stTimeCtrl_Info.PowerCtrlPeriodCnt;j++)
		{
			if(stTimeCtrl_Info.Option[j].EffectiveFlag != 0x01)
			{
				//该时段控制解除
				continue;
			}
			if( j == (stTimeCtrl_Info.PowerCtrlPeriodCnt -1))
			{
				u16Shm = stTimeCtrl_Info.Option[j].StartHour * 0x100 + stTimeCtrl_Info.Option[j].StartMinute;
				u16Ehm = 0x2400;
			}
			else
			{
				u16Shm = stTimeCtrl_Info.Option[j].StartHour * 0x100 + stTimeCtrl_Info.Option[j].StartMinute;
				u16Ehm = stTimeCtrl_Info.Option[j + 1].StartHour * 0x100 + stTimeCtrl_Info.Option[j + 1].StartMinute;;				
			}
			if(u16Ehm < u16Shm)
			{
				//结束时间小于开始时间
				continue;
			}
			u16Currhm = stCurrTime.u8Hour * 0x100 + stCurrTime.u8Minute;
			if((u16Currhm > u16Shm)&&(u16Currhm < u16Ehm))
			{
				u8Res = TRUE;
				break;
			}
		}
		if(u8Res != TRUE)
		{
			continue;
		}
		if(stTimeCtrl_Info.Option[j].EffectiveFlag == 1)
		{
			(*pu32LimitValue) = stTimeCtrl_Info.Option[j].PowerLoad;
			break;
		}
	}
	return u8Res;
}
/**--------------------------------------------------------------------
*功    能:远程遥控
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void YK_Ctrl_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u32 u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	if((pstCtrl_Info->u8YKSwitchOutState == 0) && (pstCtrl_Info->u8YKCtrlSwitchState == 0))
	{
		return;
	}
	if((pstCtrl_Info->u8YKCtrl_RoundFlag == 0) && (pstCtrl_Info->u8YKCtrlSwitchState == 0))
	{
		return;
	}
	if(pstCtrl_Info->u32YK_Timeout != 0)
	{
		//控制时间到，放开遥控
		if(u32CurrTick >= pstCtrl_Info->u32YK_Timeout)
		{
			pstCtrl_Info->u8YKCtrlSwitchState = 0;
			return;
		}
	}
	if(pstCtrl_Info->u32YKAlarmTimeoutTick == 0)
	{
		if(pstCtrl_Info->u8YKCtrlSwitchState != pstCtrl_Info->u8YKCtrl_RoundFlag)
		{
			pstCtrl_Info->u8YKAlarmFlag = 1;
			pstCtrl_Info->u32YKAlarmTimeoutTick = u32CurrTick;		
		}
	}
}
/**--------------------------------------------------------------------
*功    能:临时限电控
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Power_Ctrl_TempRation(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	TIME_PARAM_S stSysCurrTime,stTempRationEndTime;
	u8 u8FloatCoeff = 0;
	u32 u32CurrTick = 0,u32LimitPower = 0,u32Res[4];
	s8 s8Res = 0;
	s32 s32SlipPower = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	HAL_GetSystemTime(&stSysCurrTime);
	QCSG_TimerAdd(&pstCtrl_Info->stTempRationStartTime,pstCtrl_Info->u32TempRation_Time,&stTempRationEndTime);
	s8Res = QCSG_TimerSub(&stSysCurrTime,&stTempRationEndTime,u32Res);
	if(s8Res >= 0)
	{
		//当前时间已经大于临时控制的结束时间，清除终端控制状态
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;		
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;
		pstCtrl_Info->u8SlipTimeoutFlag = 0;
		return;		
	}
	s8Res = QCSG_TimerSub(&stSysCurrTime,&pstCtrl_Info->stTempRationStartTime,u32Res);
	if(s8Res < 0)	
	{
		//当前时间小于临时控制的开始时间，此操作为设置控制后，给负控往回校时，只有测试时会出现此现象
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;		
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;		
	}
	if(pstCtrl_Info->u32PowerAlarmTimeoutTick != 0)
	{
		//还在告警阶段，不进行后续处理
		return;
	}
	if(Calc_Mp_SlipPower(pstCtrl_Info) != TRUE)
	{
		//没有测量点参与滑差计算
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;			
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;		
		return;
	}
	if(pstCtrl_Info->u8SlipTimeoutFlag == 0)
	{
		//滑差周期未到，不进行后续处理
		return;
	}
	pstCtrl_Info->u8SlipTimeoutFlag = 0;
	//获取浮动系数，X%。
	u8FloatCoeff = pstCtrl_Info->u8TempRationFloatCoeff;
	u8FloatCoeff = 100 + QCSG_BCD2HEX(u8FloatCoeff);
	//pstCtrl_Info->u32LimitPower:NNNNNN.NN,扩大100倍，u8FloatCoeff：X%,扩大100倍
	u32LimitPower = pstCtrl_Info->u32LimitPower;
	u32LimitPower = u32LimitPower * u8FloatCoeff;
	//pstCtrl_Info->s32SlipPower:NN.NNNN,扩大10000倍
	s32SlipPower = pstCtrl_Info->s32SlipPower;
	if(s32SlipPower < 0)
	{
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;	
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;	
	}
	else
	{
		if(s32SlipPower > u32LimitPower)
		{
			//功率超过设置定值，开始告警，准备继电器动作
			if(pstCtrl_Info->u32PowerAlarmTimeoutTick == 0)
			{
				//告警计时器初值为0，说明当前没在进行告警，则初始化告警标志
				if(pstCtrl_Info->u8PowerCtrlSwitchState != pstCtrl_Info->u8PowerCtrl_RoundFlag)
				{
					pstCtrl_Info->u8PowerAlarmFlag = 1;
					pstCtrl_Info->u32PowerAlarmTimeoutTick = u32CurrTick;
				}						
			}
		}
		else
		{
			pstCtrl_Info->u8PowerAlarmFlag = 0;
			pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;	
			pstCtrl_Info->u8PowerCtrlSwitchState = 0;						
		}
	}
}
/**--------------------------------------------------------------------
*功    能:厂休控
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Power_Ctrl_FactoryRest(QCSG_CTRL_INFO_S* pstCtrl_Info)
{

}
/**--------------------------------------------------------------------
*功    能:时段控
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Power_Ctrl_TimeInterval(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8FloatCoeff = 0;
	u32 u32CurrTick = 0,u32LimitPower = 0;
	s32 s32SumPower = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	if(Judge_Ctrl_TimeInterval(pstCtrl_Info,&u32LimitPower) != TRUE)
	{
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;
		pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;	
		return;
	}
	if(Calc_Mp_SumPower(pstCtrl_Info) != TRUE)
	{
		//没有测量点参与滑差计算，清除终端控制状态	
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;	
		pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;			
		return;
	}
	//获取浮动系数，X%。
	u8FloatCoeff = pstCtrl_Info->u8PowerFloatCoeff & 0x7F;
	if((pstCtrl_Info->u8PowerFloatCoeff & 0x80) == 0)
	{
		u8FloatCoeff = 100 + QCSG_BCD2HEX(u8FloatCoeff);
	}
	else
	{
		u8FloatCoeff = 100 - QCSG_BCD2HEX(u8FloatCoeff);
	}
	//u32LimitPower:NNNNNN.NN,扩大100倍，u8FloatCoeff：X%,扩大100倍
	u32LimitPower = QCSG_BCD2HEX((u8)(u32LimitPower >> 24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LimitPower >> 16)) * 10000
					+ QCSG_BCD2HEX((u8)(u32LimitPower >> 8)) * 100 + QCSG_BCD2HEX((u8)u32LimitPower);
	u32LimitPower = u32LimitPower * u8FloatCoeff;
	//pstCtrl_Info->s32SlipPower:NN.NNNN,扩大10000倍
	s32SumPower = pstCtrl_Info->s32SumPower;
	if(s32SumPower < 0)
	{
		if(pstCtrl_Info->u8PowerCtrlStep != POWER_RESTORE_STEP)
		{
			pstCtrl_Info->u8PowerCtrlStep = POWER_RESTORE_STEP;
			pstCtrl_Info->u32PowerRestoreTimeoutTick = u32CurrTick;

		}
		else
		{
			if((u32CurrTick - pstCtrl_Info->u32PowerRestoreTimeoutTick) > pstCtrl_Info->u32PowerRestoreTimeout)
			{
				pstCtrl_Info->u8PowerAlarmFlag = 0;
				pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;	
				pstCtrl_Info->u8PowerCtrlSwitchState = 0;		
				pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;	
			}				
		}
	}
	else
	{
		if(s32SumPower > u32LimitPower)
		{
			//功率超过设置定值，开始告警，准备继电器动作
			if(pstCtrl_Info->u32PowerAlarmTimeoutTick == 0)
			{
				//告警计时器初值为0，说明当前没在进行告警，则初始化告警标志
				if(pstCtrl_Info->u8PowerCtrlStep != POWER_OCCUR_STEP)
				{
					pstCtrl_Info->u8PowerCtrlStep = POWER_OCCUR_STEP;
					pstCtrl_Info->u32PowerOccurTimeoutTick = u32CurrTick;
				}
				else
				{
					if((u32CurrTick - pstCtrl_Info->u32PowerOccurTimeoutTick) > pstCtrl_Info->u32PowerOccurTimeout)
					{
						if(pstCtrl_Info->u8PowerCtrlSwitchState != pstCtrl_Info->u8PowerCtrl_RoundFlag)
						{
							pstCtrl_Info->u8PowerAlarmFlag = 1;
							pstCtrl_Info->u32PowerAlarmTimeoutTick = u32CurrTick;
							pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;	
						}						
					}
				}
			}
		}
		else
		{
			if(pstCtrl_Info->u8PowerCtrlStep != POWER_RESTORE_STEP)
			{
				pstCtrl_Info->u8PowerCtrlStep = POWER_RESTORE_STEP;
				pstCtrl_Info->u32PowerRestoreTimeoutTick = u32CurrTick;

			}
			else
			{
				if((u32CurrTick - pstCtrl_Info->u32PowerRestoreTimeoutTick) > pstCtrl_Info->u32PowerRestoreTimeout)
				{
					pstCtrl_Info->u8PowerAlarmFlag = 0;
					pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;	
					pstCtrl_Info->u8PowerCtrlSwitchState = 0;		
					pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;	
				}				
			}
		}	
	}	
}
/**--------------------------------------------------------------------
*功    能:南网功率控制处理，临时限电控>厂休控>时段控
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Power_Ctrl_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{	
	if((pstCtrl_Info->u8PowerCtrlFlag != 0x01) && (pstCtrl_Info->u8TempRationFlag != 0x01))
	{
		//功率控制无效,关闭功控灯
		pstCtrl_Info->u8LedState &= (~LED_POWERCTRL);
		//清空告警相关变量
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;	
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;		
		return;
	}
	//点亮功控灯
	pstCtrl_Info->u8LedState |= LED_POWERCTRL;
	if(pstCtrl_Info->u8PowerCtrl_RoundFlag == 0)
	{
		//功控各轮次都无效
		return;
	}
/*
	if(pstCtrl_Info->u8TempRationFlag == 0x01)
	{
		//临时限电控投入
		Power_Ctrl_TempRation(pstCtrl_Info);
	}
	else if(pstCtrl_Info->u8FactoryRestFlag == 0x01)
	{
		//厂休控投入
		Power_Ctrl_FactoryRest(pstCtrl_Info);
	}
	else if(pstCtrl_Info->u8PowerCtrlFlag == 0x01)
	{
		//时段控
		Power_Ctrl_TimeInterval(pstCtrl_Info);
	}
*/
	if(pstCtrl_Info->u8TempRationFlag == 0x01)
	{
		//临时限电控投入
		Power_Ctrl_TempRation(pstCtrl_Info);
	}
	else if(pstCtrl_Info->u8PowerCtrlFlag == 0x01)
	{
		//时段控
		Power_Ctrl_TimeInterval(pstCtrl_Info);
	}
}
/**--------------------------------------------------------------------
*功    能:获取测量点正向有功总电能
*输入参数:u16Pn
*输出参数:pu32Energy
*返    回:是否获取到
--------------------------------------------------------------------**/
u8 Get_Mp_Energy(u16 u16Pn,u32* pu32Energy)
{
	QCSG_MONTH_ENERGY_LIST_S* pst_Month_Energy_List = (QCSG_MONTH_ENERGY_LIST_S*)QCSG_MONTH_ENERGY_LIST_CTRL;
	QCSG_ENERGY_S* pstEnergy_Data = NULL;
	u32 u32Temp_Energy = 0;
	u8 u8PnNum = 0,i = 0;
	
	u8PnNum = pst_Month_Energy_List->u8PnNum;
	if(u8PnNum > MAX_PN_NUM)
	{
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));
	}
	for(i = 0;i < MAX_PN_NUM;i++)
	{
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u32Energy00010000,4) != TRUE)
		{
			break;
		}
		if(QCSG_BCDCheck((u8*)&pst_Month_Energy_List->stPn_Ram_Data[i].u16Pn,1) != TRUE)
		{
			break;
		}		
	}
	if(i < MAX_PN_NUM)
	{
		MemSet(pst_Month_Energy_List,0,sizeof(QCSG_MONTH_ENERGY_LIST_S));		
	}	
	for(i = 0;i < u8PnNum;i++)
	{
		//寻找内存中记录的测量点数据信息
		if(u16Pn == pst_Month_Energy_List->stPn_Ram_Data[i].u16Pn)
		{
			pstEnergy_Data = &pst_Month_Energy_List->stPn_Ram_Data[i];
			break;
		}
	}	
	if(i >= u8PnNum)
	{
		return FALSE;
	}	
	if(QCSG_BCDCheck((u8*)&pstEnergy_Data->u32Energy00010000,4) != TRUE)
	{
		return FALSE;
	}
	u32Temp_Energy = pstEnergy_Data->u32Energy00010000;
	u32Temp_Energy = QCSG_BCD2HEX((u8)(u32Temp_Energy>>24)) * 1000000 
				   + QCSG_BCD2HEX((u8)(u32Temp_Energy>>16)) * 10000 
				   + QCSG_BCD2HEX((u8)(u32Temp_Energy>>8)) * 100
				   + QCSG_BCD2HEX((u8)u32Temp_Energy);
	(*pu32Energy) = u32Temp_Energy;
	return TRUE;
}
/**--------------------------------------------------------------------
*功    能:计算总加组累计电量
*输入参数:pstCtrl_Info
*输出参数:pu32SumEnergy
*返    回:无
--------------------------------------------------------------------**/
u8 Calc_Mp_SumEnergy_Month(QCSG_CTRL_INFO_S* pstCtrl_Info,u32* pu32SumEnergy)
{
	u8 i = 0;
	s8 s8Sign = 1;
	u16 u16Pn = 0;
	u32 u32Energy = 0;
	s32 s32SumEnergy = 0;
	for(i = 0;i < 8;i++)
	{
		u16Pn = pstCtrl_Info->u16EnergySumPnList[i];
		if(u16Pn == 0xFFFF)
		{
			continue;
		}
		u16Pn = (u16)SwitchNumToPn(pstCtrl_Info->u16EnergySumPnList[i]);
		//获取测量点月电量
		if(Get_Mp_Energy(u16Pn,&u32Energy) != TRUE)
		{
			continue;
		}
		//获取该测量点累加标记，是加还是减
		if((pstCtrl_Info->u8EnergySumFlag & (1 << i)) == 0x00)
		{
			s8Sign = -1;
		}
		else
		{
			s8Sign = 1;
		}
		s32SumEnergy += (s8Sign * u32Energy);
	}		
	if(s32SumEnergy <= 0)
	{
		return FALSE;
	}
	(*pu32SumEnergy) = s32SumEnergy;
	return TRUE;
}
/**--------------------------------------------------------------------
*功    能:月电量控制处理
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Energy_Ctrl_Month(QCSG_CTRL_INFO_S* pstCtrl_Info,QCSG_CTRL_POWERDOWN_SAVE_INFO_S* pstCtrl_PowerDown_Save_Info)
{
	u8 u8FloatCoeff = 0;
	u32 u32Energy = 0,u32CurrTick = 0;

	u32CurrTick = HAL_GetSystemTick();
	if(pstCtrl_Info->u32EnergyAlarmTimeoutTick != 0)
	{
		return;
	}
	if(Calc_Mp_SumEnergy_Month(pstCtrl_Info,&u32Energy)==FALSE)
	{
		return;
	}
	//获取浮动系数，X%。
	u8FloatCoeff = pstCtrl_Info->u8EnergyFloatCoeff & 0x7F;
	if((pstCtrl_Info->u8EnergyFloatCoeff & 0x80) == 0)
	{
		u8FloatCoeff = 100 + QCSG_BCD2HEX(u8FloatCoeff);
	}
	else
	{
		u8FloatCoeff = 100 - QCSG_BCD2HEX(u8FloatCoeff);
	}	
	//u32Energy:NNNNNN.NN,扩大100倍，u8FloatCoeff：X%,扩大100倍
	if(pstCtrl_Info->u8EnergyCtrlSwitchState == 0)//如果没跳过闸，则判断电量是否超过月电量限额

	{
		if(u32Energy >= (pstCtrl_Info->u32MaxEnergy_Month * u8FloatCoeff))//月电量超过设置定值，开始告警，准备继电器动作
		{
			
			if(pstCtrl_Info->u32EnergyAlarmTimeoutTick == 0)//告警计时器初值为0，说明当前没在进行告警，则初始化告警标志
			{
				
				if(pstCtrl_Info->u8EnergyCtrlSwitchState != pstCtrl_Info->u8EnergyCtrl_RoundFlag)
				{
					pstCtrl_Info->u8EnergyAlarmFlag = 1;
					pstCtrl_Info->u32EnergyAlarmTimeoutTick = u32CurrTick;
					pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy = u32Energy;
					pstCtrl_PowerDown_Save_Info->u32RationEnergy = 0;
				}
			}
		}
	}
	else
	{
		//如果已经跳过闸，则判断电量是否超过轮次电量
		if(u32Energy < pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy)
		{
			return;
		}
		pstCtrl_PowerDown_Save_Info->u32RationEnergy = u32Energy - pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy;
		if((pstCtrl_PowerDown_Save_Info->u32RationEnergy * 10) >= (pstCtrl_Info->u32RoundEnergy_Month * u8FloatCoeff))
		{
			//月轮次电量超过设置定值，开始告警，准备继电器动作
			if(pstCtrl_Info->u32EnergyAlarmTimeoutTick == 0)
			{
				//告警计时器初值为0，说明当前没在进行告警，则初始化告警标志
				if(pstCtrl_Info->u8EnergyCtrlSwitchState != pstCtrl_Info->u8EnergyCtrl_RoundFlag)
				{
					pstCtrl_Info->u8EnergyAlarmFlag = 1;
					pstCtrl_Info->u32EnergyAlarmTimeoutTick = u32CurrTick;
					pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy = u32Energy;
					pstCtrl_PowerDown_Save_Info->u32RationEnergy = 0;
				}
			}			
		}
	}
}
/**--------------------------------------------------------------------
*功    能:南网电量控制处理
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Energy_Ctrl_Process(QCSG_CTRL_INFO_S* pstCtrl_Info,QCSG_CTRL_POWERDOWN_SAVE_INFO_S* pstCtrl_PowerDown_Save_Info)
{
	TIME_PARAM_S stSysCurrTime;
	
	HAL_GetSystemTime(&stSysCurrTime);
	if(pstCtrl_Info->u8EnergyCtrlFlag != 0x01)
	{
		//月电量控无效，关闭电控灯
		pstCtrl_Info->u8LedState &= (~LED_ENERGYCTRL);
		//清空告警相关变量
		pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
		pstCtrl_Info->u8EnergyAlarmFlag = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;		
		return;
	}
	if(stSysCurrTime.u8Month != pstCtrl_PowerDown_Save_Info->stSysCurrTime.u8Month)
	{
		//跨月清除相关记录变量
		pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
		pstCtrl_Info->u8EnergyAlarmFlag = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;	
		pstCtrl_PowerDown_Save_Info->stSysCurrTime.u8Month = stSysCurrTime.u8Month;
	}
	//点亮电控灯
	pstCtrl_Info->u8LedState |= LED_ENERGYCTRL;	
	if(pstCtrl_Info->u8EnergyCtrl_RoundFlag == 0)
	{
		return;
	}
	Energy_Ctrl_Month(pstCtrl_Info,pstCtrl_PowerDown_Save_Info);
}




#define ALARM_START							0x01
#define ALARM_END							0x02
/**--------------------------------------------------------------------
*功    能:南网控制告警处理
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Ctrl_Alarm_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8AlarmFlag = 0,i = 0,u8YKSwitchState_Old = 0,u8YKSwitchState_New = 0;
	u32 u32CurrTick = 0,u32YKAlarmTimeoutTick = 0,u32AlarmTimeout = 0;


	//err20181107 增加告警 功率超定值 将告警放在函数前面，防止无法产生恢复告警
	QCSG_ALARM_E2000028_INFO_S* pstE2000028_Alarm_Info = (QCSG_ALARM_E2000028_INFO_S*)QCSG_ALARM_E2000028_LIST;
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	ARD02Struct stARD02Data;	
	u8 u8PnNum = 0;
	u32 u32ReadLen = 0;
	u8 u8MeterId[6];
	RealTimeYYStruct stYYTime;
	TIME_PARAM_S stSysCurrTime;
	HAL_GetSystemTime(&stSysCurrTime);
	u32CurrTick = HAL_GetSystemTick();
	if(QCSG_CheckAlarmConfig(0xE2000028) == TRUE)
	{
		u8PnNum = pstPn_Ram_Data->u8PnNum;	
		for(i = 0;i < u8PnNum;i++)
		{
			//寻找内存中记录的测量点数据信息
			if(0 == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
			{
				pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
				break;
			}
		}	
		if(i >= u8PnNum)
		{
			return;
		}
	
		if(0x00 == pstE2000028_Alarm_Info->u8AlarmFlag)
		{//初次进入,初始化
			pstE2000028_Alarm_Info->u8AlarmFlag = 0xFF;
		}
	
		//err20181107 增加告警 功率超定值
		
		if(pstE2000028_Alarm_Info->u8AlarmFlag == 0xFF)
		{
			if((pstCtrl_Info->u8PowerAlarmFlag != 0) || ((pstCtrl_Info->u8PowerCtrlSwitchState & 0x04) != 0))//功率超定值告警产生
			{
				if(pstE2000028_Alarm_Info->u8OccurStep != ALARM_START)
				{
					pstE2000028_Alarm_Info->u8OccurStep = ALARM_START;
					pstE2000028_Alarm_Info->u32OccurTick = u32CurrTick;
				}
				if((u32CurrTick - pstE2000028_Alarm_Info->u32OccurTick) >= (1 * 30 * 100))
				{
				
					//产生发生事件
					GetACSampleAddr(u8MeterId); //取交采地址
					MemSet(&stYYTime,0,sizeof(stYYTime));
					RD_Time_TO_YYTime(&stSysCurrTime,&stYYTime);
					RD_Write_ARD02_Data(1,&stSysCurrTime,pstRam_Data,&stARD02Data);
					u32ReadLen = sizeof(stARD02Data);
					NAND_WriteAlarmRecord(u8MeterId,0xE2000028,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
					TerminalSetLastAlarmDI(0xE2000028);
					if(QCSG_CheckAlarmReport(0xE2000028) == TRUE)
					{
						TerminalReportAlarmInc();
					}								
					//pstAlarm_Info->u8OccurStep = ALARM_END;
					pstE2000028_Alarm_Info->u8AlarmFlag = 0x01;
				}
				
			}
			else 
			{
				pstE2000028_Alarm_Info->u8OccurStep = ALARM_END;
				pstE2000028_Alarm_Info->u32OccurTick = u32CurrTick;	
				//pstE2000028_Alarm_Info->u8AlarmFlag = 0xFF;	
			}
		}
		
		if(pstE2000028_Alarm_Info->u8AlarmFlag == 0x01)
		{
			if((pstCtrl_Info->u8PowerAlarmFlag == 0) && ((pstCtrl_Info->u8PowerCtrlSwitchState & 0x04) == 0))//功率超定值告警恢复
			{
				if(pstE2000028_Alarm_Info->u8RestoreStep != ALARM_START)
				{
					pstE2000028_Alarm_Info->u8RestoreStep = ALARM_START;
					pstE2000028_Alarm_Info->u32RestoreTick = u32CurrTick;
				}
				if((u32CurrTick - pstE2000028_Alarm_Info->u32RestoreTick) >= (1 * 30 * 100))
				{
					//产生恢复事件
					GetACSampleAddr(u8MeterId); //取交采地址
					MemSet(&stYYTime,0,sizeof(stYYTime));
					RD_Time_TO_YYTime(&stSysCurrTime,&stYYTime);
					RD_Write_ARD02_Data(0,&stSysCurrTime,pstRam_Data,&stARD02Data);
					u32ReadLen = sizeof(stARD02Data);
					NAND_WriteAlarmRecord(u8MeterId,0xE2000028,&stYYTime,(u8*)&stARD02Data,&u32ReadLen);
					TerminalSetLastAlarmDI(0xE2000028);
					if(QCSG_CheckAlarmReport(0xE2000028) == TRUE)
					{
						TerminalReportAlarmInc();
					}								
					//pstAlarm_Info->u8RestoreStep = ALARM_END;
					//pstAlarm_Info->u32RestoreTick = u32CurrTick;
					pstE2000028_Alarm_Info->u8AlarmFlag = 0xFF; 		
				}		
			}
			else
			{
				pstE2000028_Alarm_Info->u8RestoreStep = ALARM_END;
				pstE2000028_Alarm_Info->u32RestoreTick = u32CurrTick;				
			}
		}
	}


	
	u8AlarmFlag = pstCtrl_Info->u8PowerAlarmFlag | pstCtrl_Info->u8EnergyAlarmFlag | pstCtrl_Info->u8YKAlarmFlag;
	if(u8AlarmFlag == 0)
	{
		//功控、电控都没有告警
		Pin_OutH(PIN_ALARM_OUT);
		//关闭告警灯
		pstCtrl_Info->u8LedState &= (~LED_ALARM);				
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	// 1s进入一次
	if((u32CurrTick - pstCtrl_Info->u32AlarmTick) < 100)
	{
		return;
	}
	pstCtrl_Info->u32AlarmTick = u32CurrTick;
	if(pstCtrl_Info->u32PowerAlarmTimeoutTick != 0)
	{
		if((u32CurrTick - pstCtrl_Info->u32PowerAlarmTimeoutTick) >= pstCtrl_Info->u32AlarmTimeout)
		{
			//清告警状态标志，开始控制输出
			pstCtrl_Info->u8PowerAlarmFlag = 0;
			pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
			for(i = 0;i < 4;i++)
			{
				if(((pstCtrl_Info->u8PowerCtrl_RoundFlag >> i) & 0x01) != 0)
				{
					if(((pstCtrl_Info->u8PowerCtrlSwitchState >> i) & 0x01) != 0)//若原先已经是跳闸状态
					{
						continue;
					}
					else
					{
						pstCtrl_Info->u8PowerCtrlSwitchState |= (1 << i);//跳闸
						break;
					}
				}
			}
		}
	}
	if(pstCtrl_Info->u32EnergyAlarmTimeoutTick != 0)
	{
		if((u32CurrTick - pstCtrl_Info->u32EnergyAlarmTimeoutTick) >= pstCtrl_Info->u32AlarmTimeout)
		{
			//清告警状态标志，开始控制输出
			pstCtrl_Info->u8EnergyAlarmFlag = 0;
			pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
			for(i = 0;i < 4;i++)
			{
				if(((pstCtrl_Info->u8EnergyCtrl_RoundFlag >> i) & 0x01) != 0)
				{
					if(((pstCtrl_Info->u8EnergyCtrlSwitchState >> i) & 0x01) != 0)
					{
						continue;
					}
					else
					{
						pstCtrl_Info->u8EnergyCtrlSwitchState |= (1 << i);
						break;
					}
				}
			}		
		}
	}
	if(pstCtrl_Info->u32YKAlarmTimeoutTick != 0)
	{
		u32YKAlarmTimeoutTick = pstCtrl_Info->u32YKAlarmTimeoutTick;
		u32AlarmTimeout = pstCtrl_Info->u32AlarmTimeout;
		for(i = 0;i < 4;i++)
		{
			if((pstCtrl_Info->u8YKCtrlSwitchState & (1 << i)) != 0)
			{
				u8YKSwitchState_Old++;
			}
			if((pstCtrl_Info->u8YKCtrl_RoundFlag & (1 << i)) != 0)
			{
				u8YKSwitchState_New++;
			}
			if(u8YKSwitchState_New < u8YKSwitchState_Old)
			{
				//如果新的输出开关状态配置数量小于已跳的开关数量，说明已经有合闸操作，不进行告警处理，直接动作继电器
				u32YKAlarmTimeoutTick = 0;
				u32AlarmTimeout = 0;
			}
		}
		if((u32CurrTick - u32YKAlarmTimeoutTick) >= u32AlarmTimeout)
		{
			//清告警状态标志，开始控制输出
			pstCtrl_Info->u8YKAlarmFlag = 0;
			pstCtrl_Info->u32YKAlarmTimeoutTick = 0;
			for(i = 0;i < 4;i++)
			{
				if(((pstCtrl_Info->u8YKCtrl_RoundFlag >> i) & 0x01) != 0)	//要求跳闸
				{
					if(((pstCtrl_Info->u8YKCtrlSwitchState >> i) & 0x01) != 0)	//原先已跳，则不跳
					{
						continue;
					}
					else	//原先未跳，则跳
					{
						pstCtrl_Info->u8YKCtrlSwitchState |= (1 << i);
						break;
					}
				}
				else		//要求合闸
				{
					pstCtrl_Info->u8YKCtrlSwitchState &= (~(1 << i));		//强制合闸
				}
			}		
		}
	}
	u8AlarmFlag = pstCtrl_Info->u8PowerAlarmFlag | pstCtrl_Info->u8EnergyAlarmFlag | pstCtrl_Info->u8YKAlarmFlag;
	if(u8AlarmFlag != 0)
	{
		//点亮告警灯
		pstCtrl_Info->u8LedState |= LED_ALARM;
		HAL_BuzzerON(2,20);		
		if((u32CurrTick / 100) % 2 == 0)
		{
			//偶数，动作告警继电器
			Pin_OutL(PIN_ALARM_OUT);
		}
		else
		{
			//奇数，还原继电器状态
			Pin_OutH(PIN_ALARM_OUT);
		}	
	}
	else
	{
		Pin_OutH(PIN_ALARM_OUT);
		pstCtrl_Info->u8LedState &= (~LED_ALARM);			
	}

	

	
}
/**--------------------------------------------------------------------
*功    能:南网控制输出处理
*输入参数:pstCtrl_Info
*输出参数:pstCtrl_Info
*返    回:无
--------------------------------------------------------------------**/
void Ctrl_Out_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8SwitchState = 0;
	u32 u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	// 1s进入一次
	if((u32CurrTick - pstCtrl_Info->u32CtrlOutTick) < 100)
	{
		return;
	}
	pstCtrl_Info->u32CtrlOutTick = u32CurrTick;
	u8SwitchState = pstCtrl_Info->u8PowerCtrlSwitchState | pstCtrl_Info->u8EnergyCtrlSwitchState | pstCtrl_Info->u8YKCtrlSwitchState;
	//输出继电器状态发生变化，马上进行跳闸处理
	if(u8SwitchState != pstCtrl_Info->u8YKState)
	{
		if((u8SwitchState & 0x01) != 0)	//轮次1
		{
			Pin_OutL(PIN_SWITCH_OUT1);			
		}	
		if((u8SwitchState & 0x02) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT2);			
		}
		if((u8SwitchState & 0x04) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT3);			
		}
		if((u8SwitchState & 0x08) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT4);			
		}			
	}
	pstCtrl_Info->u8YKState = u8SwitchState;
	if(u8SwitchState != 0)
	{
		pstCtrl_Info->u8YKStateBack = u8SwitchState;
	}
	QCSG_WriteTerminal_State(pstCtrl_Info->u8YXState,pstCtrl_Info->u8YKState);
	if(u8SwitchState == 0)
	{
		Pin_OutH(PIN_SWITCH_OUT1);
		Pin_OutH(PIN_SWITCH_OUT2);
		Pin_OutH(PIN_SWITCH_OUT3);
		Pin_OutH(PIN_SWITCH_OUT4);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH1);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH2);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH3);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH4);
		return;
	}
	//控制LED处理
	if((u8SwitchState & 0x01) != 0)	//轮次1
	{
		pstCtrl_Info->u8LedState |= LED_SWITCH1;		
	}
	else
	{
		pstCtrl_Info->u8LedState &= (~LED_SWITCH1);			
	}
	if((u8SwitchState & 0x02) != 0)
	{
		pstCtrl_Info->u8LedState |= LED_SWITCH2;		
	}
	else
	{
		pstCtrl_Info->u8LedState &= (~LED_SWITCH2);			
	}
	if((u8SwitchState & 0x04) != 0)
	{
		pstCtrl_Info->u8LedState |= LED_SWITCH3;		
	}
	else
	{
		pstCtrl_Info->u8LedState &= (~LED_SWITCH3);			
	}
	if((u8SwitchState & 0x08) != 0)
	{
		pstCtrl_Info->u8LedState |= LED_SWITCH4;		
	}
	else
	{
		pstCtrl_Info->u8LedState &= (~LED_SWITCH4);			
	}
	//继电器输出处理
	pstCtrl_Info->u8SwitchOutTick++;
	if(pstCtrl_Info->u8SwitchOutTick == 60)
	{
		if((u8SwitchState & 0x01) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT1);			
		}	
		if((u8SwitchState & 0x02) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT2);			
		}
		if((u8SwitchState & 0x04) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT3);			
		}
		if((u8SwitchState & 0x08) != 0)
		{
			Pin_OutL(PIN_SWITCH_OUT4);			
		}				
	}
	if(pstCtrl_Info->u8SwitchOutTick > 60)
	{
		pstCtrl_Info->u8SwitchOutTick = 0;
		Pin_OutH(PIN_SWITCH_OUT1);
		Pin_OutH(PIN_SWITCH_OUT2);
		Pin_OutH(PIN_SWITCH_OUT3);
		Pin_OutH(PIN_SWITCH_OUT4);
	}
}
/**--------------------------------------------------------------------
*功    能:左模块LED灯控制
*输入参数:无
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void Ctrl_Led_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8FrameBuf[20];
	u8 u8DataLen = 0;
	u32 u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	if((pstCtrl_Info->u8PowerCtrlFlag == 0x01) || (pstCtrl_Info->u8TempRationFlag == 0x01))
	{
		pstCtrl_Info->u8LedState |= LED_POWERCTRL;		
	}
	else
	{
		pstCtrl_Info->u8LedState &= (~LED_POWERCTRL);		
	}
	if(pstCtrl_Info->u8EnergyCtrlFlag == 0x01)
	{
		pstCtrl_Info->u8LedState |= LED_ENERGYCTRL;			
	}
	else
	{
		pstCtrl_Info->u8LedState &= (~LED_ENERGYCTRL);				
	}
	if(pstCtrl_Info->u8LedState == pstCtrl_Info->u8LedStateBack)
	{
		if((u32CurrTick - pstCtrl_Info->u32CtrlLedTick) < 1000)
		{			
			return;
		}
	}
	pstCtrl_Info->u32CtrlLedTick = u32CurrTick;
	pstCtrl_Info->u8LedStateBack = pstCtrl_Info->u8LedState;
	u8FrameBuf[u8DataLen++] = 0x68;
	MemSet(&u8FrameBuf[u8DataLen],0xAA,6);
	u8DataLen += 6;
	u8FrameBuf[u8DataLen++] = 0x68;
	u8FrameBuf[u8DataLen++] = 0x14;
	u8FrameBuf[u8DataLen++] = 0x05;
	u8FrameBuf[u8DataLen++] = 0x01 + 0x33;
	u8FrameBuf[u8DataLen++] = 0xBB + 0x33;	
	u8FrameBuf[u8DataLen++] = 0xBB + 0x33;	
	u8FrameBuf[u8DataLen++] = 0xBB + 0x33;
	u8FrameBuf[u8DataLen++] = pstCtrl_Info->u8LedState + 0x33;	
	u8FrameBuf[u8DataLen] = (u8)SumCaculation(u8FrameBuf,u8DataLen);
	u8DataLen++;
	u8FrameBuf[u8DataLen++] = 0x16;
	//发送类型：645帧，不需要等待应答
	UartPortTxReq(TASK_ID_TerminalCtrl,PLC_PORT,u8FrameBuf,u8DataLen,0x0300,0,0);
}
/**--------------------------------------------------------------------
*功    能:控制遥信告警处理
*输入参数:无
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void Ctrl_YX_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
	QCSG_ALARM_E2000001_INFO_S* pstE2000001_Alarm_Info = (QCSG_ALARM_E2000001_INFO_S*)QCSG_ALARM_E2000001_LIST;	
	QCSG_ALARM_E2000039_INFO_S* pstE2000039_Alarm_Info = (QCSG_ALARM_E2000039_INFO_S*)QCSG_ALARM_E2000039_LIST;	
	ARD02Struct stARD02Data;
	ARD09Struct stARD09Data;
	TIME_PARAM_S stTime;	
	RealTimeYYStruct stYYTime;
	u8 u8RevFrameBuf[32],u8MeterId[6],u8ALARM_E2000001_Flag = false,u8ALARM_E2000039_Flag = false;
	u8 u8PnNum = 0,u8FrameType = 0,u8DataType = 0,u8LastState = 0,u8CurrState = 0,i = 0;
	u32 u32RevLen = 0,u32Ident = 0,u32CurrTick = 0;
	LoadManagementParaStruct LoadPara;//err20180911 遥信变位告警中判断开关类型

	
	ERD03Struct stERD03Data;//err20181107 增加事件 计量门箱开闭记录
	
	u32CurrTick = HAL_GetSystemTick();
	HAL_GetSystemTime(&stTime);
	MemSet(&stYYTime,0,sizeof(stYYTime));
	RealTimeToYYTime(&stTime,&stYYTime);

	MemSet((u8*)&LoadPara, 0, sizeof(LoadManagementParaStruct));
	MR((u32)&LoadPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));
/*
	if(pstE2000001_Alarm_Info->u8GetSwitchValueFlag != 0x01)
	{
		//开关为状态为开
		pstE2000001_Alarm_Info->u8SwitchValue = 0x01; 
	}
	if(pstE2000039_Alarm_Info->u8GetSwitchValueFlag != 0x01)
	{
		//开关为状态为开
		pstE2000039_Alarm_Info->u8Switch1Value = 0x01; 
		pstE2000039_Alarm_Info->u8Switch2Value = 0x01; 
		pstE2000039_Alarm_Info->u8Switch3Value = 0x01; 
		pstE2000039_Alarm_Info->u8Switch4Value = 0x01; 
	}
*/
	if(pstE2000001_Alarm_Info->u8GetSwitchValueFlag == 0x01)	//E2000001 计量装置门开闭 ARD2，参见表D.2
	{
		if(u32CurrTick >= pstCtrl_Info->u32Switch0Timeout)
		{
			if(pstCtrl_Info->u8Switch0Attr == 0x00)	//常开
			{
				//常开属性
				if(pstE2000001_Alarm_Info->u8SwitchValue == 0x00)	//闭
				{
					//开关状态为闭合
					if(pstE2000001_Alarm_Info->u8AlarmFlag != 0x01)
					{
						//产生发生告警
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 1;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x01;
					}
				}
				else//开
				{
					//开关状态为开
					if(pstE2000001_Alarm_Info->u8AlarmFlag == 0x01)
					{
						//产生恢复告警
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 0;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x00;
					}
				}
			}
			if(pstCtrl_Info->u8Switch0Attr == 0x01)	//常闭
			{
				//常闭属性
				if(pstE2000001_Alarm_Info->u8SwitchValue == 0x01)	//开
				{
					//开关状态为开
					if(pstE2000001_Alarm_Info->u8AlarmFlag != 0x01)
					{
						//产生发生告警
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 1;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x01;
					}
				}
				else		//闭
				{
					//开关状态为闭合
					if(pstE2000001_Alarm_Info->u8AlarmFlag == 0x01)
					{
						//产生恢复告警
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 0;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x00;
					}
				}		
			}
		}
	}	
	if(pstE2000039_Alarm_Info->u8GetSwitchValueFlag == 0x01)
	{
		if(pstCtrl_Info->u8Switch1Attr == 0x00)	//常开
		{
			//常开属性	
			if(pstE2000039_Alarm_Info->u8Switch1Value == 0x00)	//闭
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm1Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x01;
				}			
			}
			else		//开
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm1Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch1Attr == 0x01)	//常闭
		{
			//常闭属性
			if(pstE2000039_Alarm_Info->u8Switch1Value == 0x01)	//开
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm1Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x01;
				}
			}
			else	//闭
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm1Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch2Attr == 0x00)
		{
			//常开属性	
			if(pstE2000039_Alarm_Info->u8Switch2Value == 0x00)	
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm2Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x01;
				}			
			}
			else
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm2Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch2Attr == 0x01)
		{
			//常闭属性
			if(pstE2000039_Alarm_Info->u8Switch2Value == 0x01)
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm2Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x01;
				}
			}
			else
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm2Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch3Attr == 0x00)
		{
			//常开属性	
			if(pstE2000039_Alarm_Info->u8Switch3Value == 0x00)	
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm3Flag != 0x01)//err20180506 2 -> 3  
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x01;//err20180506 2 -> 3  
				}			
			}
			else
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm3Flag == 0x01)//err20180506 2 -> 3  
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x00;//err20180506 2 -> 3  
				}
			}		
		}
		if(pstCtrl_Info->u8Switch3Attr == 0x01)
		{
			//常闭属性
			if(pstE2000039_Alarm_Info->u8Switch3Value == 0x01)
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm3Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x01;
				}
			}
			else
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm3Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch4Attr == 0x00)
		{
			//常开属性	
			if(pstE2000039_Alarm_Info->u8Switch4Value == 0x00)	
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm4Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x01;
				}			
			}
			else
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm4Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch4Attr == 0x01)
		{
			//常闭属性
			if(pstE2000039_Alarm_Info->u8Switch4Value == 0x01)
			{
				//开关状态为开
				if(pstE2000039_Alarm_Info->u8Alarm4Flag != 0x01)
				{
					//产生发生告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x01;
				}
			}
			else
			{
				//开关状态为闭合
				if(pstE2000039_Alarm_Info->u8Alarm4Flag == 0x01)
				{
					//产生恢复告警
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x00;
				}
			}		
		}
	}	
	if(u8ALARM_E2000001_Flag == true)	//有E2000001告警产生
	{
		if((QCSG_CheckAlarmConfig(0xE2000001) != TRUE))
		{
			return;
		}
		u8PnNum = pstPn_Ram_Data->u8PnNum;
		for(i = 0;i < u8PnNum;i++)
		{
			//寻找内存中记录的测量点数据信息
			if(0 == pstPn_Ram_Data->stPn_Ram_Data[i].u16Pn)
			{
				pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[i];
				break;
			}
		}	
		if(i >= u8PnNum)
		{
			MemSet(&stARD02Data.C_00010000[0],0xFF,4);
			MemSet(&stARD02Data.C_00020000[0],0xFF,4);
			MemSet(&stARD02Data.C_00030000[0],0xFF,4);
			MemSet(&stARD02Data.C_00040000[0],0xFF,4);	
			MemSet(&stARD02Data.C_0201FF00[0],0xFF,6);		
			MemSet(&stARD02Data.C_0202FF00[0],0xFF,9);	
			MemSet(&stARD02Data.C_0203FF00[0],0xFF,12);
			MemSet(&stARD02Data.C_0204FF00[0],0xFF,12);
			MemSet(&stARD02Data.C_0206FF00[0],0xFF,8);		
		}
		else
		{
			MemCopy(&stARD02Data.C_00010000[0],pstRam_Data->u8Data00010000,4);
			MemCopy(&stARD02Data.C_00020000[0],pstRam_Data->u8Data00020000,4);
			MemCopy(&stARD02Data.C_00030000[0],pstRam_Data->u8Data00030000,4);
			MemCopy(&stARD02Data.C_00040000[0],pstRam_Data->u8Data00040000,4);	
			MemCopy(&stARD02Data.C_0201FF00[0],&pstRam_Data->u8Data0201FF00,6);		
			MemCopy(&stARD02Data.C_0202FF00[0],&pstRam_Data->u8Data0202FF00,9);	
			MemCopy(&stARD02Data.C_0203FF00[0],&pstRam_Data->u8Data0203FF00,12);
			MemCopy(&stARD02Data.C_0204FF00[0],&pstRam_Data->u8Data0204FF00,12);
			MemCopy(&stARD02Data.C_0206FF00[0],&pstRam_Data->u8Data0206FF00,8);				
		}				
		GetACSampleAddr(u8MeterId);		
		stARD02Data.ReportState = 0;
		MemCopy(&stARD02Data.Time,&stTime,sizeof(TIME_PARAM_S));
		u32RevLen = sizeof(stARD02Data);
		NAND_WriteAlarmRecord(u8MeterId,0xE2000001,&stYYTime,(u8*)&stARD02Data,&u32RevLen);
		TerminalSetLastAlarmDI(0xE2000001);
		if(QCSG_CheckAlarmReport(0xE2000001) == TRUE)
		{
			TerminalReportAlarmInc();
		}						

		////err20181107 增加事件 计量门箱开闭记录
		if(QCSG_CheckEventMask(0xE2010015) == TRUE)
		{
			//发送时间和恢复时间 均为当前时间
			MemCopy(&stERD03Data.StartTime,&stTime,sizeof(TIME_PARAM_S));
			MemCopy(&stERD03Data.EndTime,&stTime,sizeof(TIME_PARAM_S));
			//直接将事件增量为 0
			MemSet(stERD03Data.C_00010000,0x00,4);
			MemSet(stERD03Data.C_00020000,0x00,4);
			MemSet(stERD03Data.C_00030000,0x00,4);
			MemSet(stERD03Data.C_00040000,0x00,4);
			
			//记录事件
			//MemSet(&stYYTime,0,sizeof(stYYTime));
			//RD_Time_TO_YYTime(pstTime,&stYYTime);
			//RD_Write_ERD3_Data(&pstEvent_Info_ERD3,&stERD03Data);
			u32RevLen = sizeof(stERD03Data);
			NAND_WriteEventRecord(u8MeterId,0xE2010015,&stYYTime,(u8*)&stERD03Data,&u32RevLen);
		}


		
		u8ALARM_E2000001_Flag = false;
	}
	if(u8ALARM_E2000039_Flag == true)	//有E2000039告警产生
	{
		if((QCSG_CheckAlarmConfig(0xE2000039) != TRUE))
		{
			return;
		}
		stARD09Data.ReportState = 0;
		MemCopy(&stARD09Data.Time,&stTime,sizeof(TIME_PARAM_S));
		MemSet(stARD09Data.B_E0000C1F,0x00,16);
		MemSet(stARD09Data.A_E0000C1F,0x00,16);

//err20180911 遥信变位告警中判断开关类型
//根据开关类型设置节点状态
		if(LoadPara.SwitchNodeType[0] == 0x00) //00常开触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x10) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[0] = 0x01;	//01分状态
			}
			else 
				stARD09Data.B_E0000C1F[0] = 0x00;	//00合状态

			if((pstCtrl_Info->u8YXState & 0x10) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[0] = 0x01;	//01分状态
			}
			else 
				stARD09Data.A_E0000C1F[0] = 0x00;	//00合状态
		}
		else												//01常闭触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x10) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[0] = 0x00;	//00合状态
			}
			else 
				stARD09Data.B_E0000C1F[0] = 0x01;	//01分状态

			if((pstCtrl_Info->u8YXState & 0x10) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[0] = 0x00;	//00合状态
			}
			else 
				stARD09Data.A_E0000C1F[0] = 0x01;	//01分状态
		}

		if(LoadPara.SwitchNodeType[1] == 0x00) //00常开触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x01) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[1] = 0x01;	//01分状态
			}
			else 
				stARD09Data.B_E0000C1F[1] = 0x00;	//00合状态

			if((pstCtrl_Info->u8YXState & 0x01) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[1] = 0x01;	//01分状态
			}
			else 
				stARD09Data.A_E0000C1F[1] = 0x00;	//00合状态
		}
		else												//01常闭触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x01) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[1] = 0x00;	//00合状态
			}
			else 
				stARD09Data.B_E0000C1F[1] = 0x01;	//01分状态

			if((pstCtrl_Info->u8YXState & 0x01) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[1] = 0x00;	//00合状态
			}
			else 
				stARD09Data.A_E0000C1F[1] = 0x01;	//01分状态
		}

		if(LoadPara.SwitchNodeType[2] == 0x00) //00常开触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x02) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[2] = 0x01;	//01分状态
			}
			else 
				stARD09Data.B_E0000C1F[2] = 0x00;	//00合状态

			if((pstCtrl_Info->u8YXState & 0x02) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[2] = 0x01;	//01分状态
			}
			else 
				stARD09Data.A_E0000C1F[2] = 0x00;	//00合状态
		}
		else												//01常闭触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x02) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[2] = 0x00;	//00合状态
			}
			else 
				stARD09Data.B_E0000C1F[2] = 0x01;	//01分状态

			if((pstCtrl_Info->u8YXState & 0x02) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[2] = 0x00;	//00合状态
			}
			else 
				stARD09Data.A_E0000C1F[2] = 0x01;	//01分状态
		}

		if(LoadPara.SwitchNodeType[3] == 0x00) //00常开触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x04) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[3] = 0x01;	//01分状态
			}
			else 
				stARD09Data.B_E0000C1F[3] = 0x00;	//00合状态

			if((pstCtrl_Info->u8YXState & 0x04) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[3] = 0x01;	//01分状态
			}
			else 
				stARD09Data.A_E0000C1F[3] = 0x00;	//00合状态
		}
		else												//01常闭触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x04) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[3] = 0x00;	//00合状态
			}
			else 
				stARD09Data.B_E0000C1F[3] = 0x01;	//01分状态

			if((pstCtrl_Info->u8YXState & 0x04) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[3] = 0x00;	//00合状态
			}
			else 
				stARD09Data.A_E0000C1F[3] = 0x01;	//01分状态
		}

		if(LoadPara.SwitchNodeType[4] == 0x00) //00常开触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x08) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[4] = 0x01;	//01分状态
			}
			else 
				stARD09Data.B_E0000C1F[4] = 0x00;	//00合状态

			if((pstCtrl_Info->u8YXState & 0x08) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[4] = 0x01;	//01分状态
			}
			else 
				stARD09Data.A_E0000C1F[4] = 0x00;	//00合状态
		}
		else												//01常闭触点
		{
			if((pstCtrl_Info->u8YXLastState & 0x08) == 0)		//0短接
			{
				stARD09Data.B_E0000C1F[4] = 0x00;	//00合状态
			}
			else 
				stARD09Data.B_E0000C1F[4] = 0x01;	//01分状态

			if((pstCtrl_Info->u8YXState & 0x08) == 0)		//0短接
			{
				stARD09Data.A_E0000C1F[4] = 0x00;	//00合状态
			}
			else 
				stARD09Data.A_E0000C1F[4] = 0x01;	//01分状态
		}

/*	
//err20180911 遥信变位告警中判断开关类型 原代码
		if((pstCtrl_Info->u8YXLastState & 0x10) == 0)
		{
			stARD09Data.B_E0000C1F[0] = 0;				
		}
		if((pstCtrl_Info->u8YXLastState & 0x01) == 0)
		{
			stARD09Data.B_E0000C1F[1] = 0;
		}
		if((pstCtrl_Info->u8YXLastState & 0x02) == 0)
		{
			stARD09Data.B_E0000C1F[2] = 0;
		}	
		if((pstCtrl_Info->u8YXLastState & 0x04) == 0)
		{
			stARD09Data.B_E0000C1F[3] = 0;
		}
		if((pstCtrl_Info->u8YXLastState & 0x08) == 0)
		{
			stARD09Data.B_E0000C1F[4] = 0;
		}		
		if((pstCtrl_Info->u8YXState & 0x10) == 0)
		{
			stARD09Data.A_E0000C1F[0] = 0;				
		}
		if((pstCtrl_Info->u8YXState & 0x01) == 0)
		{
			stARD09Data.A_E0000C1F[1] = 0;
		}
		if((pstCtrl_Info->u8YXState & 0x02) == 0)
		{
			stARD09Data.A_E0000C1F[2] = 0;
		}	
		if((pstCtrl_Info->u8YXState & 0x04) == 0)
		{
			stARD09Data.A_E0000C1F[3] = 0;
		}
		if((pstCtrl_Info->u8YXState & 0x08) == 0)
		{
			stARD09Data.A_E0000C1F[4] = 0;
		}		
*/
		GetACSampleAddr(u8MeterId);	
		u32RevLen = sizeof(stARD09Data);		
		NAND_WriteAlarmRecord(u8MeterId,0xE2000039,&stYYTime,(u8*)&stARD09Data,&u32RevLen);	
		TerminalSetLastAlarmDI(0xE2000039);		
		if(QCSG_CheckAlarmReport(0xE2000039) == TRUE)
		{
			TerminalReportAlarmInc();
		}				
		u8ALARM_E2000039_Flag = false;
	}	



	//读遥信模块数据
	if(UartPortRxReq(PLC_PORT,u8RevFrameBuf,&u32RevLen,&u8FrameType,&u8DataType) != NO_ERROR)
	{
		return;
	}
   	if(QCSG_Dlt645FrameJudge(u8RevFrameBuf,(u8*)&u32RevLen) != TRUE)
	{
		return;
	}	
	for(i = 0;i < u32RevLen;i++)
	{
		u8RevFrameBuf[10 + i] -= 0x33;			
	}		
    MemCopy(&u32Ident,&u8RevFrameBuf[10],4);	
	switch(u32Ident)
	{
		case 0x01CCCCCC:	//遥信状态
			u8CurrState = u8RevFrameBuf[15];
			pstCtrl_Info->u8YXState = u8CurrState;
			pstE2000001_Alarm_Info->u8GetSwitchValueFlag = 0x01;
			pstE2000001_Alarm_Info->u8SwitchValue = (u8CurrState >> 4) & 0x01;
			break;
		case 0x02CCCCCC:
		case 0x03CCCCCC:
		case 0x04CCCCCC:
		case 0x05CCCCCC:
			u8LastState = u8RevFrameBuf[14];
			u8CurrState = u8RevFrameBuf[15];
			pstCtrl_Info->u8YXState = u8CurrState;
			pstCtrl_Info->u8YXLastState = u8LastState;
			pstE2000039_Alarm_Info->u8GetSwitchValueFlag = 0x01;
			pstE2000039_Alarm_Info->u8Switch1Value = (u8CurrState) & 0x01;
			pstE2000039_Alarm_Info->u8Switch2Value = (u8CurrState>>1) & 0x01;
			pstE2000039_Alarm_Info->u8Switch3Value = (u8CurrState>>2) & 0x01;
			pstE2000039_Alarm_Info->u8Switch4Value = (u8CurrState>>3) & 0x01;
			break;
		case 0xFFCCCCCC:
			u8CurrState = u8RevFrameBuf[14];	
			pstCtrl_Info->u8YXState = u8CurrState;
#if (TEST_ON == 1)
			if(pstE2000039_Alarm_Info->u8GetSwitchValueFlag == 0)//第一次接收节点状态
				pstCtrl_Info->u8YXLastState = u8CurrState;
#endif
			pstE2000039_Alarm_Info->u8GetSwitchValueFlag = 0x01;
			pstE2000001_Alarm_Info->u8GetSwitchValueFlag = 0x01;
			pstE2000039_Alarm_Info->u8Switch1Value = (u8CurrState) & 0x01;
			pstE2000039_Alarm_Info->u8Switch2Value = (u8CurrState>>1) & 0x01;
			pstE2000039_Alarm_Info->u8Switch3Value = (u8CurrState>>2) & 0x01;
			pstE2000039_Alarm_Info->u8Switch4Value = (u8CurrState>>3) & 0x01;	
			pstE2000001_Alarm_Info->u8SwitchValue = (u8CurrState >> 4) & 0x01;			
			break;
	}			
	QCSG_WriteTerminal_State(pstCtrl_Info->u8YXState,pstCtrl_Info->u8YKState);
}
/**--------------------------------------------------------------------
*功    能:南网控制任务，控制任务
*输入参数:无
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void QCSG_Ctrl_Task(void)
{
	LoadManagementParaStruct* pstCtrl_Param_Info = (LoadManagementParaStruct*)QCSG_CTRL_PARAM_INFO;
	QCSG_CTRL_INFO_S* pstCtrl_Info = (QCSG_CTRL_INFO_S*)QCSG_CTRL_INFO;
	QCSG_CTRL_POWERDOWN_SAVE_INFO_S* pstCtrl_PowerDown_Save_Info = (QCSG_CTRL_POWERDOWN_SAVE_INFO_S*)QCSG_CTRL_POWERDOWN_SAVE_INFO;
	u32 u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	if((u32CurrTick - pstCtrl_Info->u32ReadCtrlParamTick) > 6000)
	{
		//刷新控制参数
		Update_Ctrl_Info(pstCtrl_Param_Info,pstCtrl_Info,pstCtrl_PowerDown_Save_Info);
		pstCtrl_Info->u32ReadCtrlParamTick = u32CurrTick;
	}
	//实时刷新部分控制参数
	RealTime_Update_Ctrl_Info(pstCtrl_Param_Info,pstCtrl_Info);
	//遥信处理
	Ctrl_YX_Process(pstCtrl_Info);
	//左模块LED灯处理
	Ctrl_Led_Process(pstCtrl_Info);	
	if((pstCtrl_Info->u8YKState == 0) && (pstCtrl_Info->u8YKStateBack != 0))
	{
		//控制解除
		pstCtrl_Info->u8YKStateBack = 0;
		pstCtrl_Info->u32BuzzerTimeout = u32CurrTick;
		pstCtrl_Info->u32BuzzerTickBack = u32CurrTick;
	}
	if(pstCtrl_Info->u8YKState != 0)
	{
		//还在控制状态
		pstCtrl_Info->u32BuzzerTimeout = 0;
	}
	if((pstCtrl_Info->u32BuzzerTimeout != 0) && (u32CurrTick != pstCtrl_Info->u32BuzzerTickBack))
	{
		pstCtrl_Info->u32BuzzerTickBack = u32CurrTick;
		if((u32CurrTick % 100) == 0)
		{
			HAL_BuzzerON(2,20);  
		}
		if((u32CurrTick - pstCtrl_Info->u32BuzzerTimeout) >= 100 * 60 * 3)
		{
			pstCtrl_Info->u32BuzzerTimeout = 0;
		}
	}
	//获取终端保电状态	
	if(GetPowerHoldState(pstCtrl_Info,pstCtrl_Param_Info) == TRUE)
	{
		return;
	}
	//遥控
	YK_Ctrl_Process(pstCtrl_Info);	
	//功控
	Power_Ctrl_Process(pstCtrl_Info);
	//电控
	Energy_Ctrl_Process(pstCtrl_Info,pstCtrl_PowerDown_Save_Info);
	//告警
	Ctrl_Alarm_Process(pstCtrl_Info);
	//控制输出
	Ctrl_Out_Process(pstCtrl_Info);
}
#endif
/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


