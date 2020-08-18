/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_Ctrl.c
��  ��:mxn
��  ��:1.00
��  ��:20170601
��  ��:���ؿ����ļ�
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

#if ((Project/100)==3)	//�����ն�
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
*��    ��:��ȡ�ն˱���״̬��������������µı��磬��Э�����õı���״̬
*�������:pstCtrl_Param_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
u8 GetPowerHoldState(QCSG_CTRL_INFO_S* pstCtrl_Info,LoadManagementParaStruct* pstCtrl_Param_Info)
{	
	u8 u8Timeout = 15;
	
#if defined(FKGA43_CL3200_TEST)
	u8Timeout = 2;
#endif
	//�ն��ϵ�15min�ڣ�Ϊ����״̬
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
	//����վ�������ߣ�����״̬Ϊ1���˲��ִ�����
	if(pstCtrl_Info->u8PowerHoldState == 0x01)
	{
		//����״̬����������ն˿���״̬
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
		//���������
		pstCtrl_Info->u8LedState |= LED_HOLDPOWER;
		//�رո澯��
		pstCtrl_Info->u8LedState &= (~LED_ALARM);
		//�ر��ִε�
		pstCtrl_Info->u8LedState &= (~LED_SWITCH1);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH2);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH3);
		pstCtrl_Info->u8LedState &= (~LED_SWITCH4);		
		return TRUE;
	}
	//�رձ����
	pstCtrl_Info->u8LedState &= (~LED_HOLDPOWER);
	return FALSE;
}
/**--------------------------------------------------------------------
*��    ��:���¿��ƽṹ����Ϣ
*�������:pstCtrl_Param_Info
*�������:pstCtrl_Info
*��    ��:��
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
	//����ң�ſ�������
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
	//���±���״̬
#if defined(FKGA43_CL3200_TEST)
	if((pstCtrl_Info->u8PowerHoldState == 1)&&(pstCtrl_Param_Info->PowerHoldState == 0))
	{
		//Ϊ��̨�屣����ԣ��˴��������������磬ԭң����բ��Ϊȫ��
		MemSet(pstCtrl_Param_Info->OutputSwitchState,0,sizeof(pstCtrl_Param_Info->OutputSwitchState));
		u32ReadLen = sizeof(LoadManagementParaStruct);
		NAND_WritePara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen);		
		pstCtrl_Info->u8YKCtrlSwitchState = 0;
		pstCtrl_Info->u8YKSwitchOutState = 0;
#if (TEST_ON == 1)//����̨�屣�����
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
	//�澯��ʱ
	pstCtrl_Info->u32AlarmTimeout = QCSG_BCD2HEX(pstCtrl_Param_Info->AlarmDelayTime) * 60 * 100;	
	/**************************************�µ�ز�������***************************************************/
	//���µ����Ч��־
	if(pstCtrl_Info->u8EnergyCtrlFlag != pstCtrl_Param_Info->MonthKwhCtrlFlag)
	{
		u8EnergyChange = 1;		
	}
	pstCtrl_Info->u8EnergyCtrlFlag = pstCtrl_Param_Info->MonthKwhCtrlFlag;	
	//������ֵ����ϵ��
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
	//����ۼӱ�־
	if(pstCtrl_Info->u8EnergySumFlag != pstCtrl_Param_Info->KwhCtrlIncFlag)
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u8EnergySumFlag = pstCtrl_Param_Info->KwhCtrlIncFlag;
	//����ܼ���
	if(memcmp(pstCtrl_Info->u16EnergySumPnList, pstCtrl_Param_Info->KwhCtrlPoint, sizeof(pstCtrl_Info->u16EnergySumPnList)) != 0)
	{
		u8EnergyChange = 1;
	}
	MemCopy(pstCtrl_Info->u16EnergySumPnList, pstCtrl_Param_Info->KwhCtrlPoint, sizeof(pstCtrl_Info->u16EnergySumPnList));
	//����ִ���Ч��־
	if(pstCtrl_Info->u8EnergyCtrl_RoundFlag != (pstCtrl_Param_Info->MonthKwhCtrlRoundFlag & 0x0F))
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u8EnergyCtrl_RoundFlag = (pstCtrl_Param_Info->MonthKwhCtrlRoundFlag & 0x0F);
	//�µ����޶�
	u32MaxEnergy_Month = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00 >> 24)) * 1000000
									 + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00 >> 16)) * 10000
									 + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00 >> 8)) * 100
									 + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E00));
	if(pstCtrl_Info->u32MaxEnergy_Month != u32MaxEnergy_Month)
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u32MaxEnergy_Month = u32MaxEnergy_Month;
	//�ִε���
	u32RoundEnergy_Month = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01 >> 24)) * 1000000
									   + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01 >> 16)) * 10000
									   + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01 >> 8)) * 100
									   + QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000E01));
	if(pstCtrl_Info->u32RoundEnergy_Month != u32RoundEnergy_Month)
	{
		u8EnergyChange = 1;
	}
	pstCtrl_Info->u32RoundEnergy_Month = u32RoundEnergy_Month;
	/**************************************�µ�ز�������***************************************************/
	/**************************************��ʱ�ز�������***************************************************/
	//������ʱ����Ч��־������ʱ����Чʱ���
	if((pstCtrl_Param_Info->E0000C70 & 0x0F000000) != 0x00)		//ȡ��Ч��ʶ
	{
		if(pstCtrl_Info->u8TempRationPowerUpFlag == 0)
		{
			//���ϵ�,�Ӵ洢��ʱ����ȡ����ʱ�޵���ʼʱ��
			pstCtrl_Info->u8TempRationPowerUpFlag = 1;
			pstCtrl_Info->u8TempRationFlag = 1;
			//������ʱ�޵���ʼʱ�䣬�Ӽ�¼��ʱ����
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
			//��ʱ������Ч��־
			u8TempCtrlChange = 1;
			pstCtrl_Info->u8TempRationFlag = 0x01;
			//������ʱ�޵���ʼʱ��
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
	//������ʱ�޵�ָ��
	pstCtrl_Info->u8TempRationFloatCoeff = (u8)(pstCtrl_Param_Info->E0000C70>>16);	
	//������ʱ�޵�ʱ��
	u8Time_Min = QCSG_BCD2HEX((u8)pstCtrl_Param_Info->E0000C70);
	u8Time_Hour = QCSG_BCD2HEX((u8)(pstCtrl_Param_Info->E0000C70>>8));
	pstCtrl_Info->u32TempRation_Time = u8Time_Hour * 3600 + u8Time_Min * 60;
	if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->E0000C6D,4) != TRUE)
	{
		//�����͸���û�����ã���������ʱ�ش���
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
	//���¹��ʿ��ƻ���ʱ��
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
	//��CL3200����̨�壬����ʱ�����Ϊ�̶�1���ӡ�
	pstCtrl_Info->u8SlipTime = 1;
#endif
	/**************************************��ʱ�ز�������***************************************************/
	/**************************************ʱ�οز�������***************************************************/
	//���¹�����Ч��־
	pstCtrl_Info->u8PowerCtrlFlag = pstCtrl_Param_Info->PowerCtrlFlag;
	//�����ɳ���ʱ��
	if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->PowerOverLoadTime,4) != TRUE)
	{
		pstCtrl_Info->u32PowerOccurTimeout = 10 * 60 * 100;
	}
	else
	{
		pstCtrl_Info->u32PowerOccurTimeout = QCSG_BCD2HEX(pstCtrl_Param_Info->PowerOverLoadTime) * 60 * 100;
	}
	//���ɻָ�ʱ��
	if(QCSG_BCDCheck((u8*)&pstCtrl_Param_Info->PowerResetTime,4) != TRUE)
	{
		pstCtrl_Info->u32PowerRestoreTimeout = 10 * 60 * 100;
	}
	else
	{
		pstCtrl_Info->u32PowerRestoreTimeout = QCSG_BCD2HEX(pstCtrl_Param_Info->PowerResetTime) * 60 * 100;
	}
	//���ʶ�ֵ����ϵ��
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
	/**************************************ʱ�οز�������***************************************************/
	/**************************************���ݿز�������***************************************************/
	//���³��ݿ���Ч��־
	pstCtrl_Info->u8FactoryRestFlag = pstCtrl_Param_Info->E0000E10;
	/**************************************���ݿز�������***************************************************/
	/**************************************���ع�����������*************************************************/	
	//�����ۼӱ�־
	if(pstCtrl_Info->u8PowerSumFlag != pstCtrl_Param_Info->PowerCtrlIncFlag)
	{
		u8TempCtrlChange = 1;
		u8TimeCtrlChange = 1;
	}
	pstCtrl_Info->u8PowerSumFlag = pstCtrl_Param_Info->PowerCtrlIncFlag;
	//�����ܼ���
	if(memcmp(pstCtrl_Info->u16PowerSumPnList, pstCtrl_Param_Info->PowerCtrlPoint, sizeof(pstCtrl_Info->u16PowerSumPnList)) != 0)
	{
		u8TempCtrlChange = 1;
		u8TimeCtrlChange = 1;
	}
	MemCopy(pstCtrl_Info->u16PowerSumPnList, pstCtrl_Param_Info->PowerCtrlPoint, sizeof(pstCtrl_Info->u16PowerSumPnList));
	//�����ִ���Ч��־
	if(pstCtrl_Info->u8PowerCtrl_RoundFlag != (pstCtrl_Param_Info->PowerCtrlRoundFlag & 0x0F))
	{
		u8TempCtrlChange = 1;
		u8TimeCtrlChange = 1;
	}
	pstCtrl_Info->u8PowerCtrl_RoundFlag = (pstCtrl_Param_Info->PowerCtrlRoundFlag & 0x0F);
	/**************************************���ع�����������*************************************************/
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
		//ʱ�οز������
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
*��    ��:���¿��ƽṹ����Ϣ
*�������:pstCtrl_Param_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
void RealTime_Update_Ctrl_Info(LoadManagementParaStruct* pstCtrl_Param_Info,QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8Time_Hour = 0,u8Time_Min = 0;
	u32 u32ReadLen = 0,u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	//�Žڵ�����ʵʱ����
	if(QCSG_GetDIChange(DI_E0000C00) == TRUE)	//������������ù�
	{
		//Ϊ��̨���Žڵ��������Ҫ������ȡ����Э��ò��������ڸ���ʱ�Ͳ���Ҫ�ٸ��´˲���
		QCSG_NoticeDIChange(DI_E0000C00,FALSE);
		if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) != NO_ERROR)
		{
			return;
		}	
		//����ң�ſ�������
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
	//����״̬ʵʱ����
	if(QCSG_GetDIChange(DI_E0000C50) == TRUE)
	{
		QCSG_NoticeDIChange(DI_E0000C50,FALSE);	
		if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) != NO_ERROR)
		{
			return;
		}
#if (TEST_ON == 0)//err20180628 ��������޸�
		pstCtrl_Info->u8PowerHoldState = pstCtrl_Param_Info->PowerHoldState;//Ϊ��̨�屣����ԣ�����ʵʱ���¡���Update_Ctrl_Info�н���1min����һ��
#endif
	}	
	//Զ��ң��״̬ʵʱ����
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
		//����Զ��ң��״̬
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
		//����ң���ִΣ�û�а���Э���ִΣ�Э�������ִο�������ʽ�淶�ִζԲ�����������ʽ�淶Ϊ׼���ִ�������������ñ���һ��
		pstCtrl_Info->u8YKCtrl_RoundFlag = pstCtrl_Info->u8YKSwitchOutState;
		//����ң�عر�ʱ��
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
*��    ��:��ȡ�������й�����
*�������:u16Pn
*�������:ps32Power
*��    ��:�Ƿ��ȡ��
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
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
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
		//����Ϊ����,�������λ
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
*��    ��:���㻬���
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:�Ƿ��в����������㹦��
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
	// 1min��ȡһ�β����㹦�ʣ�����1min������
	if((u32CurrTick - pstCtrl_Info->u32SlipTick) < 6000)
	{
		return TRUE;
	}
	pstCtrl_Info->u32SlipTick = u32CurrTick;
	//����������ۼӹ���
	for(i = 0;i < 8;i++)
	{
		u16Pn = pstCtrl_Info->u16PowerSumPnList[i];
		if(u16Pn == 0xFFFF)
		{
			continue;
		}		
		u16Pn = (u16)SwitchNumToPn(pstCtrl_Info->u16PowerSumPnList[i]);
		u8Pn_Flag = 1;
		//��ȡ�����㹦��
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
		//��ȡ�ò������ۼӱ�ǣ��Ǽӻ��Ǽ�
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
		//δ��һ����������
		pstCtrl_Info->u8SlipTimeoutFlag = 0;
		return TRUE;
	}
	pstCtrl_Info->u8SlipTimeoutFlag = 1;
	pstCtrl_Info->u32SlipCnt = 0;
	//���л���ʼ���
	for(i = 0;i < pstCtrl_Info->u8SlipTime;i++)
	{
		s32SlipPower += pstCtrl_Info->s32MinutePowerList[i];
	}
	s32SlipPower = s32SlipPower / pstCtrl_Info->u8SlipTime;
	pstCtrl_Info->s32SlipPower = s32SlipPower;
	return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:�����ۼӹ���
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:�Ƿ��в����������㹦��
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
	// 1 min��ȡһ�β����㹦�ʣ�����1min������
	if((u32CurrTick - pstCtrl_Info->u32SumPowerTick) < 6000)
	{
		return TRUE;
	}
	pstCtrl_Info->u32SumPowerTick = u32CurrTick;
	//����������ۼӹ���
	for(i = 0;i < 8;i++)
	{
		u16Pn = pstCtrl_Info->u16PowerSumPnList[i];		
		if(u16Pn == 0xFFFF)
		{
			continue;
		}
		u16Pn = (u16)SwitchNumToPn(pstCtrl_Info->u16PowerSumPnList[i]);
		u8Pn_Flag = 1;
		//��ȡ�����㹦��
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
		//��ȡ�ò������ۼӱ�ǣ��Ǽӻ��Ǽ�
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
*��    ��:�жϵ�ǰʱ���Ƿ��ڿ���ʱ����
*�������:pstCtrl_Info
*�������:pu32LimitValue
*��    ��:ʱ���Ƿ���Ч
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
	for(i = 0;i < 4; i++)	// 4�����ʿ��Ƽ���
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
		//---------------------����ʱ���ж�---------------------
		stStartTime.u8Year = stCurrTime.u8Year;
		u16Smd = stTimeCtrl_Info.StartMonth * 0x100 + stTimeCtrl_Info.StartDay;
		u16Emd = stTimeCtrl_Info.EndMonth * 0x100 + stTimeCtrl_Info.EndDay;
		if(u16Smd > u16Emd)
		{
			//��ֹ���õ���ʼʱ�估����ʱ�����
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
			//С����ʼʱ�䣬���ڽ���ʱ�䣬˵��ʱ��δ��������Ѱ����һ��ʱ��
			continue;
		}
		//��ִ�������ڣ�����ִ�������жϵ����Ƿ���Ҫ����
		if((stTimeCtrl_Info.TI != 0x04)&&(stTimeCtrl_Info.TI != 0x05)&&(stTimeCtrl_Info.TI != 0x06))
		{
			//�������ò���ȷ
			continue;
		}
		switch(stTimeCtrl_Info.TI)
		{
			case 0x04:	//��
				if(stCurrTime.u8Day == stStartTime.u8Day)	//�պõ�������ʼʱ�䣬����һ��ִ��
				{
					u8ValidFlag = TRUE;					
				}
				else
				{
					if(stTimeCtrl_Info.N0 == 1||stTimeCtrl_Info.N0 == 0)	//ÿ��ִ��,N0=0�����Ҳ�鵽���������ѭ��
					{
						u8ValidFlag = TRUE;					
					}
					else
					{
						while(1)
						{
							for(j = 0;j < stTimeCtrl_Info.N0;j++)
							{
								QCSG_IncOneDay(&stStartTime);	//��ʼʱ���ٹ�N0����Ƿ�Ϊ��ǰ��
							}
							if((stCurrTime.u8Month == stStartTime.u8Month) && (stCurrTime.u8Day == stStartTime.u8Day))
							{
								u8ValidFlag = TRUE;	
								break;
							}
							if(QCSG_TimerSub(&stStartTime,&stEndTime,u32Res) >= 0)
							{
								//ʱ���Ѿ����ڽ���ʱ��
								u8ValidFlag = FALSE;	
								break;
							}
						}
					}
				}
				break;
			case 0x06:	//��
				u32DateFlag = stTimeCtrl_Info.N0;
				u8Pos = QCSG_BCD2HEX(stCurrTime.u8Week);
				if(((u32DateFlag >> u8Pos) & 0x01) != 0)
				{
					u8ValidFlag = TRUE;
				}
				break;
			case 0x05:	//��
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
		//---------------------����ʱ���ж�---------------------
		u8Res = FALSE;
		for(j = 0;j < stTimeCtrl_Info.PowerCtrlPeriodCnt;j++)
		{
			if(stTimeCtrl_Info.Option[j].EffectiveFlag != 0x01)
			{
				//��ʱ�ο��ƽ��
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
				//����ʱ��С�ڿ�ʼʱ��
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
*��    ��:Զ��ң��
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
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
		//����ʱ�䵽���ſ�ң��
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
*��    ��:��ʱ�޵��
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
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
		//��ǰʱ���Ѿ�������ʱ���ƵĽ���ʱ�䣬����ն˿���״̬
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
		//��ǰʱ��С����ʱ���ƵĿ�ʼʱ�䣬�˲���Ϊ���ÿ��ƺ󣬸���������Уʱ��ֻ�в���ʱ����ִ�����
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;		
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;		
	}
	if(pstCtrl_Info->u32PowerAlarmTimeoutTick != 0)
	{
		//���ڸ澯�׶Σ������к�������
		return;
	}
	if(Calc_Mp_SlipPower(pstCtrl_Info) != TRUE)
	{
		//û�в�������뻬�����
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
		//��������δ���������к�������
		return;
	}
	pstCtrl_Info->u8SlipTimeoutFlag = 0;
	//��ȡ����ϵ����X%��
	u8FloatCoeff = pstCtrl_Info->u8TempRationFloatCoeff;
	u8FloatCoeff = 100 + QCSG_BCD2HEX(u8FloatCoeff);
	//pstCtrl_Info->u32LimitPower:NNNNNN.NN,����100����u8FloatCoeff��X%,����100��
	u32LimitPower = pstCtrl_Info->u32LimitPower;
	u32LimitPower = u32LimitPower * u8FloatCoeff;
	//pstCtrl_Info->s32SlipPower:NN.NNNN,����10000��
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
			//���ʳ������ö�ֵ����ʼ�澯��׼���̵�������
			if(pstCtrl_Info->u32PowerAlarmTimeoutTick == 0)
			{
				//�澯��ʱ����ֵΪ0��˵����ǰû�ڽ��и澯�����ʼ���澯��־
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
*��    ��:���ݿ�
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
void Power_Ctrl_FactoryRest(QCSG_CTRL_INFO_S* pstCtrl_Info)
{

}
/**--------------------------------------------------------------------
*��    ��:ʱ�ο�
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
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
		//û�в�������뻬����㣬����ն˿���״̬	
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;	
		pstCtrl_Info->u8PowerCtrlStep = POWER_IDLE_STEP;			
		return;
	}
	//��ȡ����ϵ����X%��
	u8FloatCoeff = pstCtrl_Info->u8PowerFloatCoeff & 0x7F;
	if((pstCtrl_Info->u8PowerFloatCoeff & 0x80) == 0)
	{
		u8FloatCoeff = 100 + QCSG_BCD2HEX(u8FloatCoeff);
	}
	else
	{
		u8FloatCoeff = 100 - QCSG_BCD2HEX(u8FloatCoeff);
	}
	//u32LimitPower:NNNNNN.NN,����100����u8FloatCoeff��X%,����100��
	u32LimitPower = QCSG_BCD2HEX((u8)(u32LimitPower >> 24)) * 1000000 + QCSG_BCD2HEX((u8)(u32LimitPower >> 16)) * 10000
					+ QCSG_BCD2HEX((u8)(u32LimitPower >> 8)) * 100 + QCSG_BCD2HEX((u8)u32LimitPower);
	u32LimitPower = u32LimitPower * u8FloatCoeff;
	//pstCtrl_Info->s32SlipPower:NN.NNNN,����10000��
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
			//���ʳ������ö�ֵ����ʼ�澯��׼���̵�������
			if(pstCtrl_Info->u32PowerAlarmTimeoutTick == 0)
			{
				//�澯��ʱ����ֵΪ0��˵����ǰû�ڽ��и澯�����ʼ���澯��־
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
*��    ��:�������ʿ��ƴ�������ʱ�޵��>���ݿ�>ʱ�ο�
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
void Power_Ctrl_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{	
	if((pstCtrl_Info->u8PowerCtrlFlag != 0x01) && (pstCtrl_Info->u8TempRationFlag != 0x01))
	{
		//���ʿ�����Ч,�رչ��ص�
		pstCtrl_Info->u8LedState &= (~LED_POWERCTRL);
		//��ո澯��ر���
		pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
		pstCtrl_Info->u8PowerAlarmFlag = 0;
		pstCtrl_Info->u8PowerCtrlSwitchState = 0;	
		pstCtrl_Info->u32SlipCnt = 0;
		pstCtrl_Info->s32SlipPower = 0;	
		pstCtrl_Info->u8SlipTimeoutFlag = 0;		
		return;
	}
	//�������ص�
	pstCtrl_Info->u8LedState |= LED_POWERCTRL;
	if(pstCtrl_Info->u8PowerCtrl_RoundFlag == 0)
	{
		//���ظ��ִζ���Ч
		return;
	}
/*
	if(pstCtrl_Info->u8TempRationFlag == 0x01)
	{
		//��ʱ�޵��Ͷ��
		Power_Ctrl_TempRation(pstCtrl_Info);
	}
	else if(pstCtrl_Info->u8FactoryRestFlag == 0x01)
	{
		//���ݿ�Ͷ��
		Power_Ctrl_FactoryRest(pstCtrl_Info);
	}
	else if(pstCtrl_Info->u8PowerCtrlFlag == 0x01)
	{
		//ʱ�ο�
		Power_Ctrl_TimeInterval(pstCtrl_Info);
	}
*/
	if(pstCtrl_Info->u8TempRationFlag == 0x01)
	{
		//��ʱ�޵��Ͷ��
		Power_Ctrl_TempRation(pstCtrl_Info);
	}
	else if(pstCtrl_Info->u8PowerCtrlFlag == 0x01)
	{
		//ʱ�ο�
		Power_Ctrl_TimeInterval(pstCtrl_Info);
	}
}
/**--------------------------------------------------------------------
*��    ��:��ȡ�����������й��ܵ���
*�������:u16Pn
*�������:pu32Energy
*��    ��:�Ƿ��ȡ��
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
		//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
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
*��    ��:�����ܼ����ۼƵ���
*�������:pstCtrl_Info
*�������:pu32SumEnergy
*��    ��:��
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
		//��ȡ�������µ���
		if(Get_Mp_Energy(u16Pn,&u32Energy) != TRUE)
		{
			continue;
		}
		//��ȡ�ò������ۼӱ�ǣ��Ǽӻ��Ǽ�
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
*��    ��:�µ������ƴ���
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
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
	//��ȡ����ϵ����X%��
	u8FloatCoeff = pstCtrl_Info->u8EnergyFloatCoeff & 0x7F;
	if((pstCtrl_Info->u8EnergyFloatCoeff & 0x80) == 0)
	{
		u8FloatCoeff = 100 + QCSG_BCD2HEX(u8FloatCoeff);
	}
	else
	{
		u8FloatCoeff = 100 - QCSG_BCD2HEX(u8FloatCoeff);
	}	
	//u32Energy:NNNNNN.NN,����100����u8FloatCoeff��X%,����100��
	if(pstCtrl_Info->u8EnergyCtrlSwitchState == 0)//���û����բ�����жϵ����Ƿ񳬹��µ����޶�

	{
		if(u32Energy >= (pstCtrl_Info->u32MaxEnergy_Month * u8FloatCoeff))//�µ����������ö�ֵ����ʼ�澯��׼���̵�������
		{
			
			if(pstCtrl_Info->u32EnergyAlarmTimeoutTick == 0)//�澯��ʱ����ֵΪ0��˵����ǰû�ڽ��и澯�����ʼ���澯��־
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
		//����Ѿ�����բ�����жϵ����Ƿ񳬹��ִε���
		if(u32Energy < pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy)
		{
			return;
		}
		pstCtrl_PowerDown_Save_Info->u32RationEnergy = u32Energy - pstCtrl_PowerDown_Save_Info->u32OverLimitEnergy;
		if((pstCtrl_PowerDown_Save_Info->u32RationEnergy * 10) >= (pstCtrl_Info->u32RoundEnergy_Month * u8FloatCoeff))
		{
			//���ִε����������ö�ֵ����ʼ�澯��׼���̵�������
			if(pstCtrl_Info->u32EnergyAlarmTimeoutTick == 0)
			{
				//�澯��ʱ����ֵΪ0��˵����ǰû�ڽ��и澯�����ʼ���澯��־
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
*��    ��:�����������ƴ���
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
void Energy_Ctrl_Process(QCSG_CTRL_INFO_S* pstCtrl_Info,QCSG_CTRL_POWERDOWN_SAVE_INFO_S* pstCtrl_PowerDown_Save_Info)
{
	TIME_PARAM_S stSysCurrTime;
	
	HAL_GetSystemTime(&stSysCurrTime);
	if(pstCtrl_Info->u8EnergyCtrlFlag != 0x01)
	{
		//�µ�������Ч���رյ�ص�
		pstCtrl_Info->u8LedState &= (~LED_ENERGYCTRL);
		//��ո澯��ر���
		pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
		pstCtrl_Info->u8EnergyAlarmFlag = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;		
		return;
	}
	if(stSysCurrTime.u8Month != pstCtrl_PowerDown_Save_Info->stSysCurrTime.u8Month)
	{
		//���������ؼ�¼����
		pstCtrl_Info->u32EnergyAlarmTimeoutTick = 0;
		pstCtrl_Info->u8EnergyAlarmFlag = 0;
		pstCtrl_Info->u8EnergyCtrlSwitchState = 0;	
		pstCtrl_PowerDown_Save_Info->stSysCurrTime.u8Month = stSysCurrTime.u8Month;
	}
	//������ص�
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
*��    ��:�������Ƹ澯����
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
void Ctrl_Alarm_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8AlarmFlag = 0,i = 0,u8YKSwitchState_Old = 0,u8YKSwitchState_New = 0;
	u32 u32CurrTick = 0,u32YKAlarmTimeoutTick = 0,u32AlarmTimeout = 0;


	//err20181107 ���Ӹ澯 ���ʳ���ֵ ���澯���ں���ǰ�棬��ֹ�޷������ָ��澯
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
			//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
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
		{//���ν���,��ʼ��
			pstE2000028_Alarm_Info->u8AlarmFlag = 0xFF;
		}
	
		//err20181107 ���Ӹ澯 ���ʳ���ֵ
		
		if(pstE2000028_Alarm_Info->u8AlarmFlag == 0xFF)
		{
			if((pstCtrl_Info->u8PowerAlarmFlag != 0) || ((pstCtrl_Info->u8PowerCtrlSwitchState & 0x04) != 0))//���ʳ���ֵ�澯����
			{
				if(pstE2000028_Alarm_Info->u8OccurStep != ALARM_START)
				{
					pstE2000028_Alarm_Info->u8OccurStep = ALARM_START;
					pstE2000028_Alarm_Info->u32OccurTick = u32CurrTick;
				}
				if((u32CurrTick - pstE2000028_Alarm_Info->u32OccurTick) >= (1 * 30 * 100))
				{
				
					//���������¼�
					GetACSampleAddr(u8MeterId); //ȡ���ɵ�ַ
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
			if((pstCtrl_Info->u8PowerAlarmFlag == 0) && ((pstCtrl_Info->u8PowerCtrlSwitchState & 0x04) == 0))//���ʳ���ֵ�澯�ָ�
			{
				if(pstE2000028_Alarm_Info->u8RestoreStep != ALARM_START)
				{
					pstE2000028_Alarm_Info->u8RestoreStep = ALARM_START;
					pstE2000028_Alarm_Info->u32RestoreTick = u32CurrTick;
				}
				if((u32CurrTick - pstE2000028_Alarm_Info->u32RestoreTick) >= (1 * 30 * 100))
				{
					//�����ָ��¼�
					GetACSampleAddr(u8MeterId); //ȡ���ɵ�ַ
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
		//���ء���ض�û�и澯
		Pin_OutH(PIN_ALARM_OUT);
		//�رո澯��
		pstCtrl_Info->u8LedState &= (~LED_ALARM);				
		return;
	}
	u32CurrTick = HAL_GetSystemTick();
	// 1s����һ��
	if((u32CurrTick - pstCtrl_Info->u32AlarmTick) < 100)
	{
		return;
	}
	pstCtrl_Info->u32AlarmTick = u32CurrTick;
	if(pstCtrl_Info->u32PowerAlarmTimeoutTick != 0)
	{
		if((u32CurrTick - pstCtrl_Info->u32PowerAlarmTimeoutTick) >= pstCtrl_Info->u32AlarmTimeout)
		{
			//��澯״̬��־����ʼ�������
			pstCtrl_Info->u8PowerAlarmFlag = 0;
			pstCtrl_Info->u32PowerAlarmTimeoutTick = 0;
			for(i = 0;i < 4;i++)
			{
				if(((pstCtrl_Info->u8PowerCtrl_RoundFlag >> i) & 0x01) != 0)
				{
					if(((pstCtrl_Info->u8PowerCtrlSwitchState >> i) & 0x01) != 0)//��ԭ���Ѿ�����բ״̬
					{
						continue;
					}
					else
					{
						pstCtrl_Info->u8PowerCtrlSwitchState |= (1 << i);//��բ
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
			//��澯״̬��־����ʼ�������
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
				//����µ��������״̬��������С�������Ŀ���������˵���Ѿ��к�բ�����������и澯������ֱ�Ӷ����̵���
				u32YKAlarmTimeoutTick = 0;
				u32AlarmTimeout = 0;
			}
		}
		if((u32CurrTick - u32YKAlarmTimeoutTick) >= u32AlarmTimeout)
		{
			//��澯״̬��־����ʼ�������
			pstCtrl_Info->u8YKAlarmFlag = 0;
			pstCtrl_Info->u32YKAlarmTimeoutTick = 0;
			for(i = 0;i < 4;i++)
			{
				if(((pstCtrl_Info->u8YKCtrl_RoundFlag >> i) & 0x01) != 0)	//Ҫ����բ
				{
					if(((pstCtrl_Info->u8YKCtrlSwitchState >> i) & 0x01) != 0)	//ԭ������������
					{
						continue;
					}
					else	//ԭ��δ��������
					{
						pstCtrl_Info->u8YKCtrlSwitchState |= (1 << i);
						break;
					}
				}
				else		//Ҫ���բ
				{
					pstCtrl_Info->u8YKCtrlSwitchState &= (~(1 << i));		//ǿ�ƺ�բ
				}
			}		
		}
	}
	u8AlarmFlag = pstCtrl_Info->u8PowerAlarmFlag | pstCtrl_Info->u8EnergyAlarmFlag | pstCtrl_Info->u8YKAlarmFlag;
	if(u8AlarmFlag != 0)
	{
		//�����澯��
		pstCtrl_Info->u8LedState |= LED_ALARM;
		HAL_BuzzerON(2,20);		
		if((u32CurrTick / 100) % 2 == 0)
		{
			//ż���������澯�̵���
			Pin_OutL(PIN_ALARM_OUT);
		}
		else
		{
			//��������ԭ�̵���״̬
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
*��    ��:���������������
*�������:pstCtrl_Info
*�������:pstCtrl_Info
*��    ��:��
--------------------------------------------------------------------**/
void Ctrl_Out_Process(QCSG_CTRL_INFO_S* pstCtrl_Info)
{
	u8 u8SwitchState = 0;
	u32 u32CurrTick = 0;
	
	u32CurrTick = HAL_GetSystemTick();
	// 1s����һ��
	if((u32CurrTick - pstCtrl_Info->u32CtrlOutTick) < 100)
	{
		return;
	}
	pstCtrl_Info->u32CtrlOutTick = u32CurrTick;
	u8SwitchState = pstCtrl_Info->u8PowerCtrlSwitchState | pstCtrl_Info->u8EnergyCtrlSwitchState | pstCtrl_Info->u8YKCtrlSwitchState;
	//����̵���״̬�����仯�����Ͻ�����բ����
	if(u8SwitchState != pstCtrl_Info->u8YKState)
	{
		if((u8SwitchState & 0x01) != 0)	//�ִ�1
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
	//����LED����
	if((u8SwitchState & 0x01) != 0)	//�ִ�1
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
	//�̵����������
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
*��    ��:��ģ��LED�ƿ���
*�������:��
*�������:��
*��    ��:��
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
	//�������ͣ�645֡������Ҫ�ȴ�Ӧ��
	UartPortTxReq(TASK_ID_TerminalCtrl,PLC_PORT,u8FrameBuf,u8DataLen,0x0300,0,0);
}
/**--------------------------------------------------------------------
*��    ��:����ң�Ÿ澯����
*�������:��
*�������:��
*��    ��:��
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
	LoadManagementParaStruct LoadPara;//err20180911 ң�ű�λ�澯���жϿ�������

	
	ERD03Struct stERD03Data;//err20181107 �����¼� �������俪�ռ�¼
	
	u32CurrTick = HAL_GetSystemTick();
	HAL_GetSystemTime(&stTime);
	MemSet(&stYYTime,0,sizeof(stYYTime));
	RealTimeToYYTime(&stTime,&stYYTime);

	MemSet((u8*)&LoadPara, 0, sizeof(LoadManagementParaStruct));
	MR((u32)&LoadPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));
/*
	if(pstE2000001_Alarm_Info->u8GetSwitchValueFlag != 0x01)
	{
		//����Ϊ״̬Ϊ��
		pstE2000001_Alarm_Info->u8SwitchValue = 0x01; 
	}
	if(pstE2000039_Alarm_Info->u8GetSwitchValueFlag != 0x01)
	{
		//����Ϊ״̬Ϊ��
		pstE2000039_Alarm_Info->u8Switch1Value = 0x01; 
		pstE2000039_Alarm_Info->u8Switch2Value = 0x01; 
		pstE2000039_Alarm_Info->u8Switch3Value = 0x01; 
		pstE2000039_Alarm_Info->u8Switch4Value = 0x01; 
	}
*/
	if(pstE2000001_Alarm_Info->u8GetSwitchValueFlag == 0x01)	//E2000001 ����װ���ſ��� ARD2���μ���D.2
	{
		if(u32CurrTick >= pstCtrl_Info->u32Switch0Timeout)
		{
			if(pstCtrl_Info->u8Switch0Attr == 0x00)	//����
			{
				//��������
				if(pstE2000001_Alarm_Info->u8SwitchValue == 0x00)	//��
				{
					//����״̬Ϊ�պ�
					if(pstE2000001_Alarm_Info->u8AlarmFlag != 0x01)
					{
						//���������澯
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 1;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x01;
					}
				}
				else//��
				{
					//����״̬Ϊ��
					if(pstE2000001_Alarm_Info->u8AlarmFlag == 0x01)
					{
						//�����ָ��澯
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 0;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x00;
					}
				}
			}
			if(pstCtrl_Info->u8Switch0Attr == 0x01)	//����
			{
				//��������
				if(pstE2000001_Alarm_Info->u8SwitchValue == 0x01)	//��
				{
					//����״̬Ϊ��
					if(pstE2000001_Alarm_Info->u8AlarmFlag != 0x01)
					{
						//���������澯
						u8ALARM_E2000001_Flag = true;
						stARD02Data.State = 1;
						pstE2000001_Alarm_Info->u8AlarmFlag = 0x01;
					}
				}
				else		//��
				{
					//����״̬Ϊ�պ�
					if(pstE2000001_Alarm_Info->u8AlarmFlag == 0x01)
					{
						//�����ָ��澯
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
		if(pstCtrl_Info->u8Switch1Attr == 0x00)	//����
		{
			//��������	
			if(pstE2000039_Alarm_Info->u8Switch1Value == 0x00)	//��
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm1Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x01;
				}			
			}
			else		//��
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm1Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch1Attr == 0x01)	//����
		{
			//��������
			if(pstE2000039_Alarm_Info->u8Switch1Value == 0x01)	//��
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm1Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x01;
				}
			}
			else	//��
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm1Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm1Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch2Attr == 0x00)
		{
			//��������	
			if(pstE2000039_Alarm_Info->u8Switch2Value == 0x00)	
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm2Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x01;
				}			
			}
			else
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm2Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch2Attr == 0x01)
		{
			//��������
			if(pstE2000039_Alarm_Info->u8Switch2Value == 0x01)
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm2Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x01;
				}
			}
			else
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm2Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm2Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch3Attr == 0x00)
		{
			//��������	
			if(pstE2000039_Alarm_Info->u8Switch3Value == 0x00)	
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm3Flag != 0x01)//err20180506 2 -> 3  
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x01;//err20180506 2 -> 3  
				}			
			}
			else
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm3Flag == 0x01)//err20180506 2 -> 3  
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x00;//err20180506 2 -> 3  
				}
			}		
		}
		if(pstCtrl_Info->u8Switch3Attr == 0x01)
		{
			//��������
			if(pstE2000039_Alarm_Info->u8Switch3Value == 0x01)
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm3Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x01;
				}
			}
			else
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm3Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm3Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch4Attr == 0x00)
		{
			//��������	
			if(pstE2000039_Alarm_Info->u8Switch4Value == 0x00)	
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm4Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x01;
				}			
			}
			else
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm4Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x00;
				}
			}		
		}
		if(pstCtrl_Info->u8Switch4Attr == 0x01)
		{
			//��������
			if(pstE2000039_Alarm_Info->u8Switch4Value == 0x01)
			{
				//����״̬Ϊ��
				if(pstE2000039_Alarm_Info->u8Alarm4Flag != 0x01)
				{
					//���������澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x01;
				}
			}
			else
			{
				//����״̬Ϊ�պ�
				if(pstE2000039_Alarm_Info->u8Alarm4Flag == 0x01)
				{
					//�����ָ��澯
					u8ALARM_E2000039_Flag = true;
					stARD09Data.State = 1;
					pstE2000039_Alarm_Info->u8Alarm4Flag = 0x00;
				}
			}		
		}
	}	
	if(u8ALARM_E2000001_Flag == true)	//��E2000001�澯����
	{
		if((QCSG_CheckAlarmConfig(0xE2000001) != TRUE))
		{
			return;
		}
		u8PnNum = pstPn_Ram_Data->u8PnNum;
		for(i = 0;i < u8PnNum;i++)
		{
			//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
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

		////err20181107 �����¼� �������俪�ռ�¼
		if(QCSG_CheckEventMask(0xE2010015) == TRUE)
		{
			//����ʱ��ͻָ�ʱ�� ��Ϊ��ǰʱ��
			MemCopy(&stERD03Data.StartTime,&stTime,sizeof(TIME_PARAM_S));
			MemCopy(&stERD03Data.EndTime,&stTime,sizeof(TIME_PARAM_S));
			//ֱ�ӽ��¼�����Ϊ 0
			MemSet(stERD03Data.C_00010000,0x00,4);
			MemSet(stERD03Data.C_00020000,0x00,4);
			MemSet(stERD03Data.C_00030000,0x00,4);
			MemSet(stERD03Data.C_00040000,0x00,4);
			
			//��¼�¼�
			//MemSet(&stYYTime,0,sizeof(stYYTime));
			//RD_Time_TO_YYTime(pstTime,&stYYTime);
			//RD_Write_ERD3_Data(&pstEvent_Info_ERD3,&stERD03Data);
			u32RevLen = sizeof(stERD03Data);
			NAND_WriteEventRecord(u8MeterId,0xE2010015,&stYYTime,(u8*)&stERD03Data,&u32RevLen);
		}


		
		u8ALARM_E2000001_Flag = false;
	}
	if(u8ALARM_E2000039_Flag == true)	//��E2000039�澯����
	{
		if((QCSG_CheckAlarmConfig(0xE2000039) != TRUE))
		{
			return;
		}
		stARD09Data.ReportState = 0;
		MemCopy(&stARD09Data.Time,&stTime,sizeof(TIME_PARAM_S));
		MemSet(stARD09Data.B_E0000C1F,0x00,16);
		MemSet(stARD09Data.A_E0000C1F,0x00,16);

//err20180911 ң�ű�λ�澯���жϿ�������
//���ݿ����������ýڵ�״̬
		if(LoadPara.SwitchNodeType[0] == 0x00) //00��������
		{
			if((pstCtrl_Info->u8YXLastState & 0x10) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[0] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.B_E0000C1F[0] = 0x00;	//00��״̬

			if((pstCtrl_Info->u8YXState & 0x10) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[0] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.A_E0000C1F[0] = 0x00;	//00��״̬
		}
		else												//01���մ���
		{
			if((pstCtrl_Info->u8YXLastState & 0x10) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[0] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.B_E0000C1F[0] = 0x01;	//01��״̬

			if((pstCtrl_Info->u8YXState & 0x10) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[0] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.A_E0000C1F[0] = 0x01;	//01��״̬
		}

		if(LoadPara.SwitchNodeType[1] == 0x00) //00��������
		{
			if((pstCtrl_Info->u8YXLastState & 0x01) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[1] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.B_E0000C1F[1] = 0x00;	//00��״̬

			if((pstCtrl_Info->u8YXState & 0x01) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[1] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.A_E0000C1F[1] = 0x00;	//00��״̬
		}
		else												//01���մ���
		{
			if((pstCtrl_Info->u8YXLastState & 0x01) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[1] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.B_E0000C1F[1] = 0x01;	//01��״̬

			if((pstCtrl_Info->u8YXState & 0x01) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[1] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.A_E0000C1F[1] = 0x01;	//01��״̬
		}

		if(LoadPara.SwitchNodeType[2] == 0x00) //00��������
		{
			if((pstCtrl_Info->u8YXLastState & 0x02) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[2] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.B_E0000C1F[2] = 0x00;	//00��״̬

			if((pstCtrl_Info->u8YXState & 0x02) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[2] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.A_E0000C1F[2] = 0x00;	//00��״̬
		}
		else												//01���մ���
		{
			if((pstCtrl_Info->u8YXLastState & 0x02) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[2] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.B_E0000C1F[2] = 0x01;	//01��״̬

			if((pstCtrl_Info->u8YXState & 0x02) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[2] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.A_E0000C1F[2] = 0x01;	//01��״̬
		}

		if(LoadPara.SwitchNodeType[3] == 0x00) //00��������
		{
			if((pstCtrl_Info->u8YXLastState & 0x04) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[3] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.B_E0000C1F[3] = 0x00;	//00��״̬

			if((pstCtrl_Info->u8YXState & 0x04) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[3] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.A_E0000C1F[3] = 0x00;	//00��״̬
		}
		else												//01���մ���
		{
			if((pstCtrl_Info->u8YXLastState & 0x04) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[3] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.B_E0000C1F[3] = 0x01;	//01��״̬

			if((pstCtrl_Info->u8YXState & 0x04) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[3] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.A_E0000C1F[3] = 0x01;	//01��״̬
		}

		if(LoadPara.SwitchNodeType[4] == 0x00) //00��������
		{
			if((pstCtrl_Info->u8YXLastState & 0x08) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[4] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.B_E0000C1F[4] = 0x00;	//00��״̬

			if((pstCtrl_Info->u8YXState & 0x08) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[4] = 0x01;	//01��״̬
			}
			else 
				stARD09Data.A_E0000C1F[4] = 0x00;	//00��״̬
		}
		else												//01���մ���
		{
			if((pstCtrl_Info->u8YXLastState & 0x08) == 0)		//0�̽�
			{
				stARD09Data.B_E0000C1F[4] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.B_E0000C1F[4] = 0x01;	//01��״̬

			if((pstCtrl_Info->u8YXState & 0x08) == 0)		//0�̽�
			{
				stARD09Data.A_E0000C1F[4] = 0x00;	//00��״̬
			}
			else 
				stARD09Data.A_E0000C1F[4] = 0x01;	//01��״̬
		}

/*	
//err20180911 ң�ű�λ�澯���жϿ������� ԭ����
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



	//��ң��ģ������
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
		case 0x01CCCCCC:	//ң��״̬
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
			if(pstE2000039_Alarm_Info->u8GetSwitchValueFlag == 0)//��һ�ν��սڵ�״̬
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
*��    ��:�����������񣬿�������
*�������:��
*�������:��
*��    ��:��
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
		//ˢ�¿��Ʋ���
		Update_Ctrl_Info(pstCtrl_Param_Info,pstCtrl_Info,pstCtrl_PowerDown_Save_Info);
		pstCtrl_Info->u32ReadCtrlParamTick = u32CurrTick;
	}
	//ʵʱˢ�²��ֿ��Ʋ���
	RealTime_Update_Ctrl_Info(pstCtrl_Param_Info,pstCtrl_Info);
	//ң�Ŵ���
	Ctrl_YX_Process(pstCtrl_Info);
	//��ģ��LED�ƴ���
	Ctrl_Led_Process(pstCtrl_Info);	
	if((pstCtrl_Info->u8YKState == 0) && (pstCtrl_Info->u8YKStateBack != 0))
	{
		//���ƽ��
		pstCtrl_Info->u8YKStateBack = 0;
		pstCtrl_Info->u32BuzzerTimeout = u32CurrTick;
		pstCtrl_Info->u32BuzzerTickBack = u32CurrTick;
	}
	if(pstCtrl_Info->u8YKState != 0)
	{
		//���ڿ���״̬
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
	//��ȡ�ն˱���״̬	
	if(GetPowerHoldState(pstCtrl_Info,pstCtrl_Param_Info) == TRUE)
	{
		return;
	}
	//ң��
	YK_Ctrl_Process(pstCtrl_Info);	
	//����
	Power_Ctrl_Process(pstCtrl_Info);
	//���
	Energy_Ctrl_Process(pstCtrl_Info,pstCtrl_PowerDown_Save_Info);
	//�澯
	Ctrl_Alarm_Process(pstCtrl_Info);
	//�������
	Ctrl_Out_Process(pstCtrl_Info);
}
#endif
/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/

