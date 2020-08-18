/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_AlarmEvent.h
��  ��:zhaoyun
��  ��:1.00
��  ��:20171120
��  ��:�ն˸澯�Լ��¼��ӿ�
*************************************************/
/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

ResultEnum GenerateVoltageLossCntAlarm_E2000040(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateCurrentLossCntAlarm_E2000041(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateCurrentReverseCntAlarm_E2000042(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateAVoltageLossAlarm_E2000016(u32 MeterIndex);

ResultEnum GenerateBVoltageLossAlarm_E2000017(u32 MeterIndex);//err20181218 ����B C��ʧѹʧ����������澯

ResultEnum GenerateCVoltageLossAlarm_E2000018(u32 MeterIndex);//err20181218 ����B C��ʧѹʧ����������澯

ResultEnum GenerateACurrentLossAlarm_E2000013(u32 MeterIndex);

ResultEnum GenerateBCurrentLossAlarm_E2000014(u32 MeterIndex);

ResultEnum GenerateCCurrentLossAlarm_E2000015(u32 MeterIndex);

ResultEnum GenerateACurrentReverseAlarm_E200000D(u32 MeterIndex);

ResultEnum GenerateBCurrentReverseAlarm_E200000E(u32 MeterIndex);

ResultEnum GenerateCCurrentReverseAlarm_E200000F(u32 MeterIndex);


ResultEnum GenerateTimeFaultAlarm_E200003E(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateMeterAMRFailAlarm_E200003D(u32 MeterIndex);

ResultEnum GenerateMeterRelayControlAlarm_E200003C(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateRelayChangeAlarm_E200003B(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateProgramTimeChangeAlarm_E2000035(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateTimeRateChangeAlarm_E2000036(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GenerateLessMoneyAlarm_E200002B(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GeneratePowerStopAlarm_E200002E(u32 MeterIndex, ThresholdParaStruct* Threshold);

ResultEnum GeneratePowerGoDownAlarm_E200002C(u32 MeterIndex);

ResultEnum GenerateTerminalGPRSRxTxByteAlarm_E200003A(void);
#if (Project == 500)
	ResultEnum GenerateMeterClockBatteryLowVoltageAlarm_E2000032(u32 MeterIndex);

#endif
#if (Project == 500 ||Project == 300)
	ResultEnum GenerateTerminalClockBatteryLowVoltageAlarm_E2000032(void);
#endif
ResultEnum GeneratePowerEvent(RealTimeStruct* PowerUpTime, RealTimeStruct* PowerDownTime);

ResultEnum GenerateTerminalPowerUpAndDownAlarm(RealTimeStruct* PowerTime, u32 PowerType);

ResultEnum GenerateParaChangeEvent(u32 DI);

ResultEnum GenerateMpControlEvent(u32 ControlType, MeasurePointStruct* Mp, u32 Pn, u32 Time);

void ClearNodeAlarmFlag(void);

#if ((Project/100) == 3)
//err20181112 ���Ӹ澯 CT���β࿪·��· �������ʱ�򿪣�ʵ��ʹ��ʱ�ر� ��ֹ�󱨡�
ResultEnum GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(u32 AlarmDI,u32 AlarmState);//AlarmDI:�澯���ݱ�ʶ  AlarmState: ������ָ�
#endif

/************************ (C) COPYRIGHT zhaoyun***********************END OF FILE****/


