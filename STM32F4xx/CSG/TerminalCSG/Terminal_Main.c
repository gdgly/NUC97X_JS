

#include "../Hcsg/ProjectCSG.h"
#include "../MS/MS.h"

#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Head/QCSG_Resolve.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_FMC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_ExtMem.h"

#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayError.h"
#include "../Display/DisplayFTP.h"
#include "../Display/DisplayLoop.h"
#include "../Device/MEMRW.h"
#include "Terminal_PowerDown.h"
#include "Terminal_Init.h"
#include "Terminal_Timer.h"
#include "../QCSG_Head/ReadMeter_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/CH_PLC_AMR_Task.h" 
#include "../QCSG_Head/CH_RS485_AMR_Task.h" 
#include "../QCSG_Head/CH_RS485_AMS_Task.h" 
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Head/QCSG_Ctrl.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"
#include "../Test/Test_RAM.h"
#include "Terminal_Task.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Warning.h"
#ifdef IC_NUC9xx
#include "../STM32F4xx/NUC97x_ADC.h"
#endif
#include "../QCSG_Uart/Terminal_ReadMeter_ACSample.h"


#ifndef Project
	#error
#endif


void SdramTest(void);
void Terminal_RunTimeProcess(void);

void ACTION_RESET(void);		//��λ�������

uint32_t SystemCoreClock = 16000000;	//��stm32f4xx_hal_rcc.c�ļ�����ʱ��Ҫ,linkʱ����Ҫ

void _0(void)
{
}


void ACTION_RESET(void)//��λ�������
{
	u32 PORTn;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Comm_Ram->RESET != 0x55)//0x55=������(����ʹ�ÿ��Ź���λ)
	{
		return;
	}
	PORTn = Comm_Ram->ACTION_RESET_PORTn;//������λ��ͨ�Ŷ˿ں�
	switch(PORTn)
	{
		case RS232PORT:				// 0//RS232
		case GPRSPORT:				// 1//GPRS
		case RS485_1PORT:			// 2//RS485_1ͨ�ſں�
		case RS485_2PORT:			// 3//RS485-2ͨ�ſں�
		case RS485_3PORT:			// 5//RS485_3ͨ�ſں�(WI-FI)
		case IRPORT:				// 6//����ͨ�ſں�
		case ENETPORT:				// 10//��̫��ͨ�ſں�
		case SMSPORT:				// 11//����ͨ�ſں�
			UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
			if(UARTCtrl->Task != 0)
			{// 0=���ڽ���
				return;
			}
	}
	//��ʱ�ȴ�����ֽڷ���
	Comm_Ram->GPMSTimer = 500/10;	// 18 ͨ��(������UART11) ÿ10MS��1��ʱ��
	while(Comm_Ram->GPMSTimer)
	{
	}
	WWDT_RESET();//����ʹ�ܿ��Ź���λ
}

void SdramTest(void)
{
#ifdef IC_STM32F4xx
	u32* p32;
	if(RAM_Test(ADDR_128KDATABUFF,64))
	{
		Comm_Ram->GPMSTimer=1000/10;		//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
		while(1)
		{
			Init_ExtMem();					//sdram��ʼ��
			SDRAM_FMC_Init();
			if(RAM_Test(ADDR_128KDATABUFF,64)==0)		//RAM����;����:0=��ȷ,1=����
			{
				break;
			}
			if(Comm_Ram->GPMSTimer==0)					//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
			{
				__disable_irq();
				p32=(u32*)(ADDR_Meter_SaveRam_Start+LEN_SaveRam-8);
				p32[0]=0x12345678;
				p32[1]=RAM_CRC32(ADDR_Meter_SaveRam_Start,LEN_SaveRam-4);		//RAM������CRC32����
				RTC->BKP0R&=0xffff00ff;										//B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
				RTC->BKP0R|=0x5500;
				//	RCC->CSR|=1;//LSI RC ��������
				while(IWDG->SR!=0)
				{
				}
				IWDG->KR=0x5555;
				IWDG->PR=0;	// 4 ��Ƶ
				IWDG->RLR=1;
				IWDG->KR=0xAAAA;
				IWDG->KR=0xCCCC;
				while(1);
			}
		}
	}
#endif
}


void Terminal_RunTimeProcess(void)
{

#if(TEST_ON == 1)//err20180730 ̨�� ΢��������ģ��ʵʱ�ٲ������ʱ�л������ʣ��·�һ��GUIʵʱ�ٲ�֡��Ӧ�����ʡ�
		QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)(ADDR_LCD_MeterAMRDataBuf);
#endif

	CommunicationParaStruct CommPara;
	UARTCtrl_TypeDef* UartCtrl;
	u32 TimeSub[4];
	u32 i = 0;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	u8* pPowerDownTime = (u8*)ADDR_TFM_YMDHMS;		// ����ǰ��¼��ʱ��
	u8 TimeFlag = 0;
	
#if ((Project/100)==3)	
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	u32 WriteLen = sizeof(MeasurePointStruct);
	MeasurePointStruct MeasurePoint;
	u8 AC_Addr[6];
	//u32 u32Ua = 0,u32Ub = 0,u32Uc = 0,u32Ia = 0,u32Ib = 0,u32Ic = 0,u32Un = 0,u32In = 0;
	//u8 u8Meterphase = 0x04;
	
	#ifdef FKGA43_TEST_Alarm_CT
			u32 Voltage = 0;
	#endif
#endif
	
	
#if ((Project/100)==5) //err20181122 ���� ��/�¶��� �ն�ͣ�������ʱ�� �õ�

	u32 TempPowerDownTime;
	u8 TempTime[8];//err20181212 �޸�ͣ�綳�����ݴ���
	RealTimeStruct RealTime;
	HAL_GetSystemTime(&RealTime);
#endif

	if(pTerminalRam->PowerDownTask != 0)
	{												// �˴�Ϊ������������󣬱��������״̬������Һ����Ϩ��ǰ�ٴ����������״̬����
		pTSysCtrl->Flag |= TERMINAL_POWER_UP_FLAG;
		pTerminalRam->PowerDownTask = 0;
	}
	
//err20180730 ̨�� ΢��������ģ��ʵʱ�ٲ������ʱ�л������ʣ��·�һ��GUIʵʱ�ٲ�֡��Ӧ�����ʡ�
#if(TEST_ON == 1)
	if((Pn_Lcd_Data->u16Res1 & LCD_RT_AMR_REQ_TEST_START) != 0)
	{
		if(pTerminalRam->Seconds - pTerminalRam->LCD_RT_AMR_REQ_TEST_START_TIME >= 20)//��ʱ����20s
		{
			Pn_Lcd_Data->u16Res1 &= ~LCD_RT_AMR_REQ_TEST_START;
			Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ_TEST_END;
			pTerminalRam->LCD_RT_AMR_REQ_TEST_START_TIME = 0;
		}
	}
#endif
	
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)	//II�ͼ��������б�̼�
	if(Pin_Read(PIN_PROKEY) == 0)
	{
		i = pTerminalRam->Ticks;
		if((i - pTSysCtrl->ProKeyPressTick) > 10)
		{
			pTSysCtrl->ProKeyLastSeconds = PRO_KEY_HOLD_SECOND;
			pTSysCtrl->Flag |= TERMINAL_PROKEY_PRESS_FLAG;
			//HAL_BuzzerON(11, 18000);

			/*Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
			PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
			AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)ADDR_PLC_AMR_TaskCtrl;
			PLCManage->State = PLC_YBWHZ_TEST;//PLC_RESET_INIT;
			AMRCtrl->OldState = AMRCtrl->State;
			pTerminalRouter->Old_RouterInitTask = pTerminalRouter->RouterInitTask;
			pTerminalRouter->RouterResetCnt = 0;		
			pTerminalRouter->RouterInitTask = 0;
			pTerminalRouter->RetryCnt = 0;	*/

			
		}
	}
	else
	{
		pTSysCtrl->ProKeyPressTick = pTerminalRam->Ticks;
	}
#endif
#if ((Project/100)==5) //II�ͼ��������ߵƺ��ź�ǿ�����ƴ���
	UartCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UartCtrl->LinkTask == 101)
	{
		Terminal_Ram->GPRS_ONLINELED_MS_Timer = 2000/10;
	}
	if(Terminal_Ram ->Signal == 1)
	{
		Terminal_Ram ->GPRS_WEAK_SINGLE_LED_MS_Timer = 2000/10;
	}
	if((Terminal_Ram ->Signal == 2)||(Terminal_Ram ->Signal == 3))
	{
		Terminal_Ram ->GPRS_WEAK_SINGLE_LED_MS_Timer = 2000/10;
		Terminal_Ram ->GPRS_STRONG_SINGLE_LED_MS_Timer = 2000/10;
	}
	if(Terminal_Ram ->Signal == 4)
	{
		Terminal_Ram ->GPRS_STRONG_SINGLE_LED_MS_Timer = 2000/10;
	}
#endif
	if(pTSysCtrl->CheckSecond != pCommRam->RTCBuff[0])
	{
		pTSysCtrl->CheckSecond = pCommRam->RTCBuff[0];
		if(pTSysCtrl->CurExeFileUpdateDelay > 0) pTSysCtrl->CurExeFileUpdateDelay--;
		#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)       //II�ͼ��������б�̼�20180515ybzyl
			if(pTSysCtrl->ProKeyLastSeconds > 0) pTSysCtrl->ProKeyLastSeconds--;
			if(pTSysCtrl->ProKeyLastSeconds == 0) pTSysCtrl->Flag &= ~TERMINAL_PROKEY_PRESS_FLAG;
		#endif

		#if ((Project/100)==5) //II�ͼ������澯ָʾ�ƴ��� �и澯ʱ��һ����һ��
			if(pTSysCtrl->LatelyAlarmNum != 0)//err20190108 ����II�ͼ������澯�ƴ���
			{
				if((pTSysCtrl->CheckSecond & 0x01) != 0)
				{
					Pin_OutH(PIN_ATTR_LED);
				}
				else
				{
					Pin_OutL(PIN_ATTR_LED);
				}
			}
			else
			{
				Pin_OutH(PIN_ATTR_LED);
			}
		#endif

	#if ((Project/100) == 3)
	
//return;	
			
	MemSet((u8*)&MeasurePoint, 0, sizeof(MeasurePointStruct));
	NAND_ReadMeasurePoint(0, 0xE0800002, (u8*)&MeasurePoint, &WriteLen);
    GetACSampleAddr(AC_Addr);
	if(MeterFileInfo->CountFlag != METER_FILE_SYNC_TO_RAM_FLAG)
	{
		if(MemCompare(NodeList[0].Addr,AC_Addr, METER_ADDR_SIZE) != 0)
		{
			MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
			MeasurePoint.Addr[0] = AC_Addr[0];
			MeasurePoint.Addr[1] = AC_Addr[1];
			MeasurePoint.Addr[2] = AC_Addr[2];
			MeasurePoint.Addr[3] = AC_Addr[3];
			MeasurePoint.Addr[4] = AC_Addr[4];
			MeasurePoint.Addr[5] = AC_Addr[5];
			
			NAND_WriteMeasurePoint(0,0xE0800002, (u8*)&MeasurePoint, &WriteLen);
		}			
	}
	
//return;		
	
	#endif
//err20181112 ���Ӹ澯 CT���β࿪·��· �������ʱ�򿪣�ʵ��ʹ��ʱ�ر� ��ֹ�󱨡�
	/**************************************************ybwhz ���Զ� 1sִ��һ��****************************************************/
/*���CT���ο�·��·
ÿ��������AD����A/B/C�༤����ѹ 
�ȴ������ȶ�ʱ�жϵ�ѹ��С �����澯
����:
CT_A_CHK --- PF9  --- AD3_IN7 	7.2���Ŵ�(��ʱ����)
CT_B_CHK --- PF10 --- AD3_IN8 	11���Ŵ�(Ŀǰʹ�ø�ͨ��)
CH_CTL0  --- PK1  				74HC4052оƬͨ��ѡ��
CH_CTL1  --- PK0   				74HC4052оƬͨ��ѡ��
CTT      --- PA4				��������DAC�����(������ ����6kHZ���� ������ѹ���2.0V,���Ҳ�3k-50k ������ѹ1.24V)
CT_KEY   --- PJ8 				ADC���Žӵؿ���(Ĭ�ϵ͵�ƽ������AD�˿���Ч)

74HC4052оƬͨ��ѡ��:
 ��       ����       CTL0  CTL1
 A�� --- T2IN ---    0     0
 B�� --- T4IN ---    0	   1
 C�� --- T6IN ---    1		 0
*/
#if ((Project/100) == 3)
#ifdef FKGA43_TEST_Alarm_CT
		switch(pTSysCtrl->CheckSecond/16)//BCDȡ��һλ
		{
			case 0:
			case 1:
				Pin_OutL(PIN_CH_CTL0);Pin_OutL(PIN_CH_CTL1);//A�� ��Ȧ	//���������,�������ʹ����
				//DisplayData(1 ,4,2,(u16 *)(ADDR_STRINGBUFF+(4*84)+4));
				break;
			case 2:
			case 3:
				Pin_OutL(PIN_CH_CTL0);Pin_OutH(PIN_CH_CTL1);//B�� ��Ȧ	//���������,�������ʹ����
				//DisplayData(2 ,4,2,(u16 *)(ADDR_STRINGBUFF+(4*84)+4));
				break;
			case 4:
			case 5:
				Pin_OutH(PIN_CH_CTL0);Pin_OutL(PIN_CH_CTL1);//C�� ��Ȧ	//���������,�������ʹ����
				//DisplayData(3 ,4,2,(u16 *)(ADDR_STRINGBUFF+(4*84)+4));
				break;
			default: break;
		}
#ifdef IC_STM32F4xx
		Voltage = Get_ADC3(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
		//DisplayData(Voltage ,4,2,(u16 *)(ADDR_STRINGBUFF+(3*84)+4));
#endif
#ifdef IC_NUC9xx
		Voltage = ADCx(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
#endif
		
		

		
		if(pTSysCtrl->CheckSecond == 0x10) //����A��
		{
			#ifdef IC_STM32F4xx
				Voltage = Get_ADC3(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
			#endif
			#ifdef IC_NUC9xx
				Voltage = ADCx(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
			#endif
			if(Voltage < CT_OPEN_LIMIT_VALUE)	// ��· һ��Ϊ0.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000A,1);//A�� ��·����
			else 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000A,0);//A�� ��·�ָ�

			if(Voltage > CT_SHORT_LIMIT_VALUE)	// ��· һ��Ϊ1.99-2.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000007,1);//A�� ��·����
			else
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000007,0);//A�� ��·�ָ�
		}

		if(pTSysCtrl->CheckSecond == 0x30) //����B��
		{
			#ifdef IC_STM32F4xx
				Voltage = Get_ADC3(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
			#endif
			#ifdef IC_NUC9xx
				Voltage = ADCx(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
			#endif
			if(Voltage < CT_OPEN_LIMIT_VALUE)	// ��· һ��Ϊ0.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000B,1);//B�� ��·����
			else 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000B,0);//B�� ��·�ָ�

			if(Voltage > CT_SHORT_LIMIT_VALUE)	// ��· һ��Ϊ1.99-2.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000008,1);//B�� ��·����
			else
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000008,0);//B�� ��·�ָ�
		}

		if(pTSysCtrl->CheckSecond == 0x50) //����C��
		{
			#ifdef IC_STM32F4xx
				Voltage = Get_ADC3(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
			#endif
			#ifdef IC_NUC9xx
				Voltage = ADCx(8,1,1,0);//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
			#endif
			if(Voltage < CT_OPEN_LIMIT_VALUE)	// ��· һ��Ϊ0.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000C,1);//C�� ��·����
			else 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000C,0);//C�� ��·�ָ�

			if(Voltage > CT_SHORT_LIMIT_VALUE)	// ��· һ��Ϊ1.99-2.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000009,1);//C�� ��·����
			else
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000009,0);//C�� ��·�ָ�
		}


#endif
#endif
	/**************************************************ybwhz ���Զ� 1sִ��һ��****************************************************/
		i = Get_GPRSRxTxBytes();
		if(i != 0)
		{
			pTSysCtrl->GPRSRxTxBytes += i;
			Clear_GPRSRxTxBytes();
		}
		#if ((Project/100)==2)||((Project/100)==9) 
			GetTerminalReadMeterFlag();
		#endif
		if((pTSysCtrl->Flag & TERMINAL_POWER_UP_FLAG) != 0)
		{
			pTSysCtrl->Flag &= ~TERMINAL_POWER_UP_FLAG;
			MC(0, (u32)TimeSub, SIZE_16);
			QCSG_TimerSub((TIME_PARAM_S*)pCommRam->MYMDHMS, (TIME_PARAM_S*)pPowerDownTime, TimeSub);

			#if ((Project/100) == 2) || ((Project/100) == 3) || ((Project/100) == 9)
		 	//err20190304 I�ͼ���������ʱ�䳬��1min�жϵ����¼���澯��
				if((TimeSub[0] > 0x00) || (TimeSub[1] > 0x00) || (TimeSub[2] > 0x00))
				{
					GeneratePowerEvent((RealTimeStruct*)pCommRam->MYMDHMS, (RealTimeStruct*)pPowerDownTime);
					if((pTSysCtrl->Flag & TERMINAL_POWER_DOWN_DONE_FLAG) == 0) 
					{
						GenerateTerminalPowerUpAndDownAlarm((RealTimeStruct*)pPowerDownTime, 0);
					}
					pTSysCtrl->Flag &= ~TERMINAL_POWER_DOWN_DONE_FLAG;
					GenerateTerminalPowerUpAndDownAlarm((RealTimeStruct*)pCommRam->MYMDHMS, 1);
				}
				else
				{
					if(TimeSub[3] > 0x0A)//�����¼�����10s����¼
					{
						GeneratePowerEvent((RealTimeStruct*)pCommRam->MYMDHMS, (RealTimeStruct*)pPowerDownTime);
					}
				}

			#elif ((Project/100) == 5) //err20190304 II�ͼ���������ʱ�䳬��5s�жϵ����¼���澯��
				if((TimeSub[0] > 0x00) || (TimeSub[1] > 0x00) || (TimeSub[2] > 0x00) || (TimeSub[3] > 0x05))//err20190423 �����ж����� ����1��04����ܲ������澯
				{
					GeneratePowerEvent((RealTimeStruct*)pCommRam->MYMDHMS, (RealTimeStruct*)pPowerDownTime);
					if((pTSysCtrl->Flag & TERMINAL_POWER_DOWN_DONE_FLAG) == 0) 
					{
						GenerateTerminalPowerUpAndDownAlarm((RealTimeStruct*)pPowerDownTime, 0);
					}
					pTSysCtrl->Flag &= ~TERMINAL_POWER_DOWN_DONE_FLAG;
					GenerateTerminalPowerUpAndDownAlarm((RealTimeStruct*)pCommRam->MYMDHMS, 1);
				}
			#endif 
			
			MR((u32)&i, ADDR_TerminalResetCnt, 4);
			if(i > 0)
			{
				if((TimeSub[0] > 0x00) || (TimeSub[1] > 0x00) || (TimeSub[2] > 0x00) || (TimeSub[3] > 0x0A))
				{
					pTSysCtrl->DayPowerDownTime = TimeSub[0] * 24 * 60;
					pTSysCtrl->DayPowerDownTime += TimeSub[1] * 60;
					pTSysCtrl->DayPowerDownTime += TimeSub[2];

					pTSysCtrl->MonthPowerDownTime = TimeSub[0] * 24 * 60;
					pTSysCtrl->MonthPowerDownTime += TimeSub[1] * 60;
					pTSysCtrl->MonthPowerDownTime += TimeSub[2];

					if(pCommRam->MYMDHMS[3] != pPowerDownTime[3])
					{
						TimeFlag |= 0x01;
					}
					if(pCommRam->MYMDHMS[4] != pPowerDownTime[4])
					{
						TimeFlag |= 0x02;
					}
					if((TimeFlag & 0x01) != 0)
					{	
					//�������
					#if ((Project/100)==5)
						//err20181212 ������һ������ĩʱ�����ڶ���
						TempTime[0] = 0;
						TempTime[1] = 0;
						TempTime[2] = 0;
						TempTime[3] = pCommRam->MYMDHMS[3];
						TempTime[4] = pCommRam->MYMDHMS[4];
						TempTime[5] = pCommRam->MYMDHMS[5];
						QCSG_TimerSub((TIME_PARAM_S*)TempTime, (TIME_PARAM_S*)pPowerDownTime, TimeSub);
						TempPowerDownTime = 0;
						TempPowerDownTime += TimeSub[1] * 60;
						TempPowerDownTime += TimeSub[2];
						MR((u32)&pTSysCtrl->DayPowerDownCnt, QCSG_POWER_DOWN_NUM_DAY, 4);
						pTSysCtrl->DayPowerDownCnt ++;
						MW((u32)&pTSysCtrl->DayPowerDownCnt, QCSG_POWER_DOWN_NUM_DAY, 4);
						
						MR((u32)&pTSysCtrl->DayPowerDownTime, QCSG_POWER_DOWN_TIME_DAY, 4);
						pTSysCtrl->DayPowerDownTime += TempPowerDownTime;
						MW((u32)&pTSysCtrl->DayPowerDownTime, QCSG_POWER_DOWN_TIME_DAY, 4);
						//�˴���ʱ������ͣ�����ղ�����������
						SaveTerminalPowerDownFreezData(AMR_DailyStatisticFreeze,&RealTime);	//err20181122 �����ն���ͣ�����
					#endif
					
						//���㵱�յ���ʱ��
						pPowerDownTime[0] = 0;
						pPowerDownTime[1] = 0;
						pPowerDownTime[2] = 0;
						pPowerDownTime[3] = pCommRam->MYMDHMS[3];
						pPowerDownTime[4] = pCommRam->MYMDHMS[4];
						pPowerDownTime[5] = pCommRam->MYMDHMS[5];
						QCSG_TimerSub((TIME_PARAM_S*)pCommRam->MYMDHMS, (TIME_PARAM_S*)pPowerDownTime, TimeSub);
						i = 0;
						MW((u32)&i, QCSG_POWER_DOWN_NUM_DAY, 4);
						MW((u32)&i, QCSG_POWER_DOWN_TIME_DAY, 4);
						pTSysCtrl->DayPowerDownTime = TimeSub[0] * 24 * 60;//err20181212 ��ͣ��ʱ�䲻�ᳬ��1*24*60
						pTSysCtrl->DayPowerDownTime += TimeSub[1] * 60;
						pTSysCtrl->DayPowerDownTime += TimeSub[2];
					}
					if((TimeFlag & 0x02) != 0)
					{
						//�������

					#if ((Project/100)==5)
						//err20181212 ������һ������ĩʱ�����ڶ���
						TempTime[0] = 0;
						TempTime[1] = 0;
						TempTime[2] = 0;
						TempTime[3] = 1;
						TempTime[4] = pCommRam->MYMDHMS[4];
						TempTime[5] = pCommRam->MYMDHMS[5];
						QCSG_TimerSub((TIME_PARAM_S*)TempTime, (TIME_PARAM_S*)pPowerDownTime, TimeSub);
						TempPowerDownTime =  TimeSub[0] * 24 * 60;
						TempPowerDownTime += TimeSub[1] * 60;
						TempPowerDownTime += TimeSub[2];
						MR((u32)&pTSysCtrl->MonthPowerDownCnt, QCSG_POWER_DOWN_NUM_MONTH, 4);
						pTSysCtrl->MonthPowerDownCnt ++;
						MW((u32)&pTSysCtrl->MonthPowerDownCnt, QCSG_POWER_DOWN_NUM_MONTH, 4);
						
						MR((u32)&pTSysCtrl->MonthPowerDownTime, QCSG_POWER_DOWN_TIME_MONTH, 4);
						pTSysCtrl->MonthPowerDownTime += TempPowerDownTime;
						MW((u32)&pTSysCtrl->MonthPowerDownTime, QCSG_POWER_DOWN_TIME_MONTH, 4);
						//�˴���ʱ������ͣ�����²�����������
						SaveTerminalPowerDownFreezData(AMR_MonthStatisticFreeze,&RealTime); //err20181122 �����¶���ͣ�����
					#endif

						//���㵱�µ���ʱ��
						pPowerDownTime[0] = 0;
						pPowerDownTime[1] = 0;
						pPowerDownTime[2] = 0;
						pPowerDownTime[3] = 1;
						pPowerDownTime[4] = pCommRam->MYMDHMS[4];
						pPowerDownTime[5] = pCommRam->MYMDHMS[5];
						QCSG_TimerSub((TIME_PARAM_S*)pCommRam->MYMDHMS, (TIME_PARAM_S*)pPowerDownTime, TimeSub);
						i = 0;
						MW((u32)&i, QCSG_POWER_DOWN_NUM_MONTH, 4);
						MW((u32)&i, QCSG_POWER_DOWN_TIME_MONTH, 4);
						pTSysCtrl->MonthPowerDownTime = TimeSub[0] * 24 * 60;
						pTSysCtrl->MonthPowerDownTime += TimeSub[1] * 60;
						pTSysCtrl->MonthPowerDownTime += TimeSub[2];						
					}
					MR((u32)&i, QCSG_POWER_DOWN_NUM_DAY, 4);
					i ++;
					MW((u32)&i, QCSG_POWER_DOWN_NUM_DAY, 4);

					MR((u32)&i, QCSG_POWER_DOWN_TIME_DAY, 4);
					i += pTSysCtrl->DayPowerDownTime;
					MW((u32)&i, QCSG_POWER_DOWN_TIME_DAY, 4);

					MR((u32)&i, QCSG_POWER_DOWN_NUM_MONTH, 4);
					i ++;
					MW((u32)&i, QCSG_POWER_DOWN_NUM_MONTH, 4);

					MR((u32)&i, QCSG_POWER_DOWN_TIME_MONTH, 4);
					i += pTSysCtrl->MonthPowerDownTime;
					MW((u32)&i, QCSG_POWER_DOWN_TIME_MONTH, 4);
				}		
			}
			else
			{
				pTSysCtrl->DayPowerDownCnt = 0;
				pTSysCtrl->DayPowerDownTime = 0;
				pTSysCtrl->MonthPowerDownCnt = 0;
				pTSysCtrl->MonthPowerDownTime = 0;

				MR(QCSG_POWER_DOWN_NUM_DAY, (u32)&pTSysCtrl->DayPowerDownCnt, 4);
				MR(QCSG_POWER_DOWN_TIME_DAY, (u32)&pTSysCtrl->DayPowerDownTime, 4);
				MR(QCSG_POWER_DOWN_NUM_MONTH, (u32)&pTSysCtrl->MonthPowerDownCnt, 4);
				MR(QCSG_POWER_DOWN_TIME_MONTH, (u32)&pTSysCtrl->MonthPowerDownTime, 4);
			}
			TerminalReportCheckAlarm(pTSysCtrl);
		}
		if((pTSysCtrl->Flag & TERMINAL_GO_TO_TEST_FLAG) != 0)
		{
			pTSysCtrl->Flag &= ~TERMINAL_GO_TO_TEST_FLAG;
			Comm_Ram->Init_OS|=1<<7;//OS��Ҫ����ԭ��:B0=????,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
			ReSTART_OS();//����OS����Ӳ���Լ����
			while(1)
			{
			}
		}
		if((pTSysCtrl->Flag & TERMINAL_ADDR_CHANGE_FLAG) != 0)
		{			
			#if(USER != 3400) //ybzyl20181031 �������ϰ漼��֧������������ʱ���������������������������߼���ַ�󣬸��ز�Ҫ���������ⱻ��Ϊ�ǳ������
				pTSysCtrl->Flag &= ~TERMINAL_ADDR_CHANGE_FLAG;
			  DataResetAction(0x02, pTSysCtrl->LastPORTn);
			#endif
		}

		//if((pTSysCtrl->Flag & TERMINAL_COMM_PARA_CHANGE_FLAG) != 0)
		if(((pTSysCtrl->Flag & TERMINAL_COMM_PARA_CHANGE_FLAG) != 0)
			||((pTSysCtrl->Flag & TERMINAL_ADDR_CHANGE_FLAG) != 0))
		{
			pTSysCtrl->Flag &= ~TERMINAL_COMM_PARA_CHANGE_FLAG;
			pTSysCtrl->Flag &= ~TERMINAL_ADDR_CHANGE_FLAG;
			//err20190708 ���������޸��ն˵�ַ���������ñ�־�������㽫����GPRSģ��״̬������LinkTask=1
			//�޷������������ڴ˴����������־��
			UartCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);
			if(UartCtrl->LinkTask < 101)
			{
				UartCtrl->LinkTask = 1;
			}
			else
			{
				UartCtrl->LinkTask = 102;
				pTerminalRam->GPRSHeartBeat_S_Timer = 0;
			}
			
			UartCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(ENETPORT);
			if(UartCtrl->LinkTask < 101)
			{
				UartCtrl->LinkTask = 0;
			}
			else
			{
				UartCtrl->LinkTask = 102;
				pTerminalRam->ENETHeartBeat_S_Timer = 0;
			}		
		}

/*
		#if (Project/100 == 3)
		#define JC_IN								1000
		#define JC_UN								2200
		u32 ADDR_DATA;
		ADDR_DATA=Get_ADDR_UARTnTx(ACSAMPLEPORT)+14;
			//��ȡ��������߷�ʽ�����ѹ�������
			//u32ReadLen = sizeof(MeasurePointStruct);
			MemSet((u8*)&MeasurePoint, 0, sizeof(MeasurePointStruct));
			if(NAND_ReadMeasurePoint(0,0xE0800010,(u8*)&MeasurePoint,&WriteLen) != NO_ERROR)
			{
				u8Meterphase = 0x04;
				u32In = JC_IN;
				u32Un = JC_UN;
			}
			else
			{
				u8Meterphase = MeasurePoint.ConnectionType;
				if(u8Meterphase > 0x04)
				{
					u8Meterphase = 0x04;
				}
				if(QCSG_BCDCheck((u8*)&MeasurePoint.RatedCurrent,3) != TRUE)
				{
					u32In = JC_IN;
				}
				else
				{
					u32In = QCSG_BCD2HEX((u8)(MeasurePoint.RatedCurrent>>16))* 10000 + QCSG_BCD2HEX((u8)(MeasurePoint.RatedCurrent>>8))* 100 + QCSG_BCD2HEX((u8)MeasurePoint.RatedCurrent);		
				}
				if(QCSG_BCDCheck((u8*)&MeasurePoint.RatedVoltage,2) != TRUE)
				{
					u32Un = JC_UN;
				}
				else
				{
					u32Un = QCSG_BCD2HEX((u8)(MeasurePoint.RatedVoltage>>8))* 100 + QCSG_BCD2HEX((u8)MeasurePoint.RatedVoltage);
				}			
			}

			GetACSample(0x0201ff00);
			MR((u32)&u32Ua,ADDR_DATA,2);
			u32Ua = QCSG_BCD2HEX((u8)(u32Ua>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ua);//2200 
			MR((u32)&u32Ua,ADDR_DATA+2,2);
			u32Ub = QCSG_BCD2HEX((u8)(u32Ub>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ub);//2200 
			MR((u32)&u32Ua,ADDR_DATA+4,2);
			u32Uc = QCSG_BCD2HEX((u8)(u32Uc>>8)) * 100 + QCSG_BCD2HEX((u8)u32Uc);//2200  220V
			GetACSample(0x0202ff00);
			MR((u32)&u32Ua,ADDR_DATA,3);
			u32Ia &= 0x7fffff;
			u32Ia = QCSG_BCD2HEX((u8)(u32Ia>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ia>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ia);//1000 1A
			MR((u32)&u32Ua,ADDR_DATA+3,3);
			u32Ib &= 0x7fffff;
			u32Ib = QCSG_BCD2HEX((u8)(u32Ib>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ib>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ib);//1000 1A
			MR((u32)&u32Ua,ADDR_DATA+6,3);
			u32Ic &= 0x7fffff;
			u32Ic = QCSG_BCD2HEX((u8)(u32Ic>>16)) * 10000 + QCSG_BCD2HEX((u8)(u32Ic>>8)) * 100 + QCSG_BCD2HEX((u8)u32Ic);//1000 1A

			//Un 2200 220V 
			//In 1000 1A
			
			if((100*u32Ua < 60*u32Un) && (100*u32Ub < 60*u32Un) && (100*u32Uc < 60*u32Un) && 
				(1000*u32Ia < 5*u32In) && (1000*u32Ib < 5*u32In) && (1000*u32Ic < 5*u32In))
			{
				//�����������	
				//pTSysCtrl->Flag |= TERMINAL_POWER_DOWN_DONE_FLAG;
			}
			else
			{	//������������
				MW((u32)pCommRam->MYMDHMS, (u32)pPowerDownTime, 6);//����ǰʱ�Ӹ���
				pPowerDownTime[6] = 0x20;
			}
		#else 
		*/
		MW((u32)pCommRam->MYMDHMS, (u32)pPowerDownTime, 6);//����ǰʱ�Ӹ���
		pPowerDownTime[6] = 0x20;
		//#endif
		
		SdramTest();
		ACTION_RESET();
		if((pTSysCtrl->CurExeFileReady == TERMINAL_EXE_FILE_READY) && (pTSysCtrl->CurExeFileUpdateDelay == 0) && (pTSysCtrl->CurExeFileType <= EXE_FILE_TYPE_PLC))
		{
			NAND_TerminalExeFileUpdate();
		}

		

	}

	if(pTSysCtrl->CheckMinute != pCommRam->RTCBuff[1])
	{
		pTSysCtrl->CheckMinute = pCommRam->RTCBuff[1];	
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		pTSysCtrl->TerminalAddr = CommPara.TerminalAddr;
		pTSysCtrl->TerminalLocation = CommPara.TerminalLocationCode;
		UartCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);
		UartCtrl->LinkCtrl = 1;
		if((CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS) && 
		   (CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS) &&
		   (CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS))
		{		
			#if ((Project/100 == 3) && (USER == 3400))
				UartCtrl->LinkCtrl = 1;//err20190530���ϸ���������Ӧ���������Բ��ر�GPRSģ��
			#else
				UartCtrl->LinkCtrl = 0;
			#endif
		}
		i = UartCtrl->LINKCHECK_RTCD;
		if((u8)i != pCommRam->RTCBuff[4])
		{
			UartCtrl->LINKCHECK_RTCD = pCommRam->RTCBuff[4];
			UartCtrl->LinkCount = 0;
		}

		UartCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(ENETPORT);
		UartCtrl->LinkCtrl = 1;
		if((CommPara.MasterAddrType != MASTER_CH_TYPE_ETHE) && 
		   (CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_ETHE) &&
		   (CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_ETHE))
		{
			UartCtrl->LinkCtrl = 0;
		}
		i = UartCtrl->LINKCHECK_RTCD;
		if((u8)i != pCommRam->RTCBuff[4])
		{
			UartCtrl->LINKCHECK_RTCD = pCommRam->RTCBuff[4];
			UartCtrl->LinkCount = 0;
		}

		GenerateTerminalGPRSRxTxByteAlarm_E200003A();
		#if (Project == 500 ||Project == 300)
			GenerateTerminalClockBatteryLowVoltageAlarm_E2000032();
		#endif
	}

	if(pTSysCtrl->CheckDay != pCommRam->RTCBuff[4])
	{
		pTSysCtrl->CheckDay = pCommRam->RTCBuff[4];
		MR((u32)&i, ADDR_E1800013, 3);
		i = bcd_hex(i);
		i += pTSysCtrl->GPRSRxTxBytes >> 10;	//������λ: kb	
		i = hex_bcd(i);
		MW((u32)&i, ADDR_E1800013, 3);		
		i = pTSysCtrl->GPRSRxTxBytes >> 10;
		i = hex_bcd(i);
		MW((u32)&i, ADDR_E1800012, 2);
		pTSysCtrl->GPRSRxTxBytes = 0;

	#if ((Project/100)==5) //err20181122 ���� ��/�¶��� �ն�ͣ�������ʱ��		
		if((TimeFlag & 0x01) == 0)//err20181211 ������ͣ����գ��������մ�����ʱ��
		{
			SaveTerminalPowerDownFreezData(AMR_DailyStatisticFreeze,&RealTime);	//err20181122 �����ն���ͣ�����
				
			pTSysCtrl->DayPowerDownCnt = 0;
			pTSysCtrl->DayPowerDownTime = 0;
			MR(QCSG_POWER_DOWN_NUM_DAY, (u32)&pTSysCtrl->DayPowerDownCnt, 4);//err20181122 ԭ������ ���ղ�����
			MR(QCSG_POWER_DOWN_TIME_DAY, (u32)&pTSysCtrl->DayPowerDownTime, 4);
		}
	#endif	
		ClearNodeAlarmFlag();
	}

	if(pTSysCtrl->CheckMonth != pCommRam->RTCBuff[5])
	{
		pTSysCtrl->CheckMonth = pCommRam->RTCBuff[5];
		MR((u32)&i, ADDR_E1800013, 3);
		MW((u32)&i, ADDR_E1800014, 3);

		MR((u32)&i, ADDR_E1800016, 2);
		MW((u32)&i, ADDR_E1800017, 2);
		i = 0;
		MW((u32)&i, ADDR_E1800013, 3);
		MW((u32)&i, ADDR_E1800016, 2);
		
	#if ((Project/100)==5) //err20181122 ���� ��/�¶��� �ն�ͣ�������ʱ��
		if((TimeFlag & 0x02) == 0)//err20181211 ������ͣ����£��������´�����ʱ��
		{	
			SaveTerminalPowerDownFreezData(AMR_MonthStatisticFreeze,&RealTime); //err20181122 �����¶���ͣ�����
			
			pTSysCtrl->MonthPowerDownCnt = 0;
			pTSysCtrl->MonthPowerDownTime = 0;
			MR(QCSG_POWER_DOWN_NUM_MONTH, (u32)&pTSysCtrl->MonthPowerDownCnt, 4);//err20181122 ԭ������ ���²�����
			MR(QCSG_POWER_DOWN_TIME_MONTH, (u32)&pTSysCtrl->MonthPowerDownTime, 4);
		}
	#endif
	}

	if((pTSysCtrl->BuzzMode != 0) && (pTSysCtrl->BuzzOnTime != 0))
	{
		if((pTerminalRam->Ticks - pTSysCtrl->BuzzOnTick) > pTSysCtrl->BuzzOnTime)
		{
			HAL_BuzzerOFF(pTSysCtrl->BuzzMode);
		}
	}
	if(pTerminalRam->Ticks > 600) pCommRam->DisplayAllErrorTimer = 0;
}
u32 COV2DIS(u8 dis1,u32 intnum, u8* dec)
{
	
 	u32 i =0;
	while(dec[i] != '\0')
		i++;
	
	dec[i++] = dis1;
	dec[i++] = intnum/1000%10 + '0';
	dec[i++] = intnum/100%10 + '0';
	dec[i++] = intnum/10%10 + '0';
	dec[i++] = intnum/1%10 + '0';
	dec[i++] = 'T';
	dec[i++] = '\0';
	return i-1;
}

#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
void Terminal_Task(void)			// ��ʱ����(�����е�)
{
	//u8* Test_data = (u8*)(ADDR_EFLASH_Start+5);
	//u8 Test_data2[10] = {1,2,3,4,5,6,7,8,9,10};
	//MemCopy(Test_data,Test_data2,5);
	
	
	NAND_SyncMeasurePointToRam();//err20180529 ��һ��˳�� �ϵ�������ͬ����ɺ��ٲ���ͣ�ϵ�澯
	Terminal_RunTimeProcess();
	

	MultiFrameSendTask(RS232PORT);
	MultiFrameSendTask(GPRSPORT);
	MultiFrameSendTask(IRPORT);
	MultiFrameSendTask(ENETPORT);
	MultiFrameSendTask(RS485_1PORT);
	MultiFrameSendTask(RS485_2PORT);
	MultiFrameSendTask(RS485_3PORT);

	TerminalReportTask(ENETPORT);
	TerminalReportTask(GPRSPORT);
	
	#if ((Project/100)==3)	//ר��
		RD_Meter_Task();
		QCSG_Ctrl_Task();
	#endif
	#if ((Project/100)==2)		//I�ͼ�����
		AMRTaskManage();
		RealTimeAMRTask();
		PLC_ModuleManage(PLC_PORT);
		CH_RS485_AMR_Task(RS485_1PORT);
		CH_RS485_AMR_Task(RS485_2PORT);
		CH_RS485_AMR_Task(RS485_3PORT);
		CH_RS485_AMS_Task(RS485_1PORT);
		CH_RS485_AMS_Task(RS485_2PORT);
		CH_RS485_AMS_Task(RS485_3PORT);
		MeterAlarmRecordGenerate();
	#endif
	#if ((Project/100)==9)
		AMRTaskManage();
		RealTimeAMRTask();
		PLC_ModuleManage(PLC_PORT);
		CH_RS485_AMR_Task(RS485_1PORT);
		CH_RS485_AMR_Task(RS485_2PORT);
		CH_RS485_AMR_Task(RS485_3PORT);
		MeterAlarmRecordGenerate();
		RD_Meter_Task();
		//QCSG_Ctrl_Task();
	#endif

}
#endif

#if ((Project/100)==5)
void Terminal_Task(void)			// ��ʱ����(�����е�)
{
	
	NAND_SyncMeasurePointToRam();
	Terminal_RunTimeProcess();//err20180529 ��һ��˳�� �ϵ�������ͬ����ɺ��ٲ���ͣ�ϵ�澯

	MultiFrameSendTask(GPRSPORT);
	MultiFrameSendTask(IRPORT);
	MultiFrameSendTask(RS485_1PORT);
	MultiFrameSendTask(RS485_2PORT);

	TerminalReportTask(GPRSPORT);
	
	AMRTaskManage();
	RealTimeAMRTask();
	//PLC_ModuleManage(PLC_PORT);
	CH_RS485_AMR_Task(RS485_1PORT);
	CH_RS485_AMR_Task(RS485_2PORT);
	CH_RS485_AMS_Task(RS485_1PORT);
	CH_RS485_AMS_Task(RS485_2PORT);
	MeterAlarmRecordGenerate();
}
#endif

__asm void Call_E2(u32 r0)//����E2�˵�,���R0=PC��ַ
{
	PUSH {LR}
	LDR LR,=ADDR_SystemStack
	STR SP,[LR]
	CMP R0,#0
	BNE Call_E2_1
	LDR R0,=ADDR_Menu_ENTRY//E2��ڵ�ַ
	LDR R0,[R0,#(4*1)];
Call_E2_1
	LDR LR,=Call_E2_End
	BX R0
Call_E2_End
	LDR LR,=ADDR_SystemStack
	MOV R0,#0
	STR R0,[LR]
	POP {PC}
	ALIGN
}


#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)

void Terminal_DisplayTask(void)
{
	u32 i;
	u32 * p32;
	//u8 *p8;
	
	if(MenuCount->Count==0)//�˵���ջ����
  	{	//�˵���ջ����=0
    	if((Comm_Ram->ButtonVal&0x10)==0)
    	{//��ȷ�ϼ����£�ѭ��
				if(Comm_Ram->DisplayScanTimer==0x0)
				{
					//������ʾ���
					if(DOTMIXPMODE->Task==0)
					{//�ϵ���Ļ
							DisplayClr();//��ȫ��
							Startdisplay();
							HAL_BuzzerON(BUZZ_MODE_00, 50);
							Comm_Ram->DisplayScanTimer=30/10;							
					}
					else
					{//������Ļ
						Comm_Ram->DisplayScanTimer=1000/10;        //���ȡDLT500����ʱ�޸�Ϊ1s��ˢһ��
					}
					
				#ifdef IC_STM32F4xx
					if(((RTC->BKP0R&0xff)>=2)&&((RTC->BKP0R&0xff)<=4))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
				#endif
				#ifdef IC_NUC9xx
					if((FileDownload->USB0Task>=2)&&(FileDownload->USB0Task<=4))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
				#endif
					{//USB�ļ���ɱ�־��Ч
						Comm_Ram->DisplayAllErrorTimer=0;
						Comm_Ram->DisplayScanTimer=5000/10;
						DOTMIXPMODE->Task=1;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
						DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
					}
					else
					{
						i=MRR(ADDR_FTPFileFlags+4,1);
						if((i<(MaxFileNum+3))&&(Comm_Ram->DisplayAllErrorTimer!=0))
						{//FTP�ļ�������Ч
							DOTMIXPMODE->Task=1;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
							DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
							DisplayFTP();//FTP�ļ�������ʾ
						}
						else
						{
							if((i==(MaxFileNum+2))&&(Comm_Ram->DisplayAllErrorTimer==0))
							{
								MWR(MaxFileNum+3,ADDR_FTPFileFlags+4,1);//FTP�ļ���������ʾ�����־
							}
							#if ErrorAutoDisplay==1//0=����,1=��ֹ;ϵͳ�����Զ�������ʾ
								if(Comm_Ram->Factory!=0x55)//0x55=����״̬
								{//���ǹ���״̬
									if(Comm_Ram->ErrorIC)
									{
										Comm_Ram->ErrorIC=0;
										Comm_Ram->DisplayAllErrorTimer=0;
									}
								}
							#endif
							if((Comm_Ram->ErrorIC)&&(Comm_Ram->DisplayAllErrorTimer))
							{//������ʾ
								DOTMIXPMODE->Task=2;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
								DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
								DisplayError();//Ӳ�������ģ�������ʾ
							}
							else
							{
								//ѭ����ʾ
								DOTMIXPMODE->Task=4;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
								DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
								DisplayLoop();//�ն�ѭ��
							}
						}
					}
					LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
				}				
			}    
			else
			{//ȷ�ϼ����£������˵�
				DOTMIXPMODE->Task=3;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
				DOTMIXPMODE->Mix=2;//2;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
				Comm_Ram->ButtonVal&=~0x10;//ȷ�ϼ�ʹ�ú����
				if(FileDownload->File&0x10)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
				{
					MenuCount->Count=1;
					p32=(u32 *)(ADDR_Menu_ENTRY);
					Stack->MenuStack[MenuCount->Count].PC=p32[1];
					Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
					Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
					#if ((Project/100)==3) || ((Project/100)==9)//�����ն�&���
						Stack->MenuStack[MenuCount->Count].EditRow=1;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
					#endif
					#if ((Project/100)==2)//I�ͼ�����
						Stack->MenuStack[MenuCount->Count].EditRow=2;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
					#endif
					Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
					Call_E2(0);//����E2�˵�
					DisplayClrRow(0,0);//����Ļ��ʾ��ָ����
					//DisplayClrRow(9,9);//����Ļ��ʾ��ָ����
					DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
					DisplayLoop_BottomICO(9);//��ͼ����ʾ
				}
				//������ʾ���
				Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
				LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
			}
			
	}		
	else
	{//�˵���ջ����!=0
		if(FileDownload->File&0x10)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
		{
			Call_E2(Stack->MenuStack[MenuCount->Count].PC);//����E2�˵�
		}
		if(Comm_Ram->DisplayScanTimer==0x0)
		{
			//p8=(u8*)(ADDR_MenuICO);//1byte �˵�����ʾ���к͵���ͼ�� 0=��,1=��
			//if(p8[0]!=0)
			//{
			DisplayClrRow(0,0);//����Ļ��ʾ��ָ����
			//DisplayClrRow(9,9);//����Ļ��ʾ��ָ����
			DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
			DisplayLoop_BottomICO(9);//��ͼ����ʾ
			//}
			//������ʾ���
			Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
		}
		if(Comm_Ram->MenuOutTimer==0)
		{//�˵��˳� 10MS��ʱ��
			MenuCount->Count=0;
		}
	}
}

#endif

void Terminal_Main_PowerUp(void)
{
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
	Terminal_DisplayTask();			// ��ʾ����
#endif
	Terminal_Uart_Manage();			// Terminal_Uart����
	
	Terminal_Task();				// ��ʱ����(�����е�)	
}

#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
void Terminal_PowerDownTask(void)
{

	Terminal_DisplayTask();

	MultiFrameSendTask(GPRSPORT);
	MultiFrameSendTask(ENETPORT);
	
	EthernetModuleManage(ENETPORT);
	GPRSModuleManage(GPRSPORT);

	TerminalReportTask(ENETPORT);
	TerminalReportTask(GPRSPORT);

	UartPortManage(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
	UartTaskProcess(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);

	UartPortManage(RS232PORT, (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl);
	UartTaskProcess(RS232PORT, (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl);
}
#endif

#if ((Project/100)==5)
void Terminal_PowerDownTask(void)
{
	MultiFrameSendTask(GPRSPORT);
	GPRSModuleManage(GPRSPORT);
	TerminalReportTask(GPRSPORT);

	UartPortManage(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
	UartTaskProcess(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
}
#endif

#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)

void Terminal_Main_Battery(void)			// ��ع���
{
#if ((Project/100) < 2)						//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ��
	return;
#else
	if(Comm_Ram->BattWork_S_timer==0)		// 4 ��ع����ػ��붨ʱ��
	{
		DeepPowerDown();					// ��ȵ�Դ�½�ģʽ
	}
	else
	{
		Terminal_DisplayTask();//I2��ʾ
	#ifdef IC_STM32F4xx
		if((RTC->BKP0R&0xff)<=1)//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
		{//��USB����
			SCB->SCR = 0x0;//SLEEPDEEP=0
			__WFI();
		}
	#endif
		if(Pin_Read(PIN_WKUP)!=0)
		{
			WWDT_RESET();					// ����ʹ�ܿ��Ź���λ
		}
	}		
#endif
}
#endif

void Terminal_Main(void)
{
	
	switch(Comm_Ram->POWER)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		case 0:														// 0=��ع���
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)		// �ɳ���ֹͣ�������ѹ4.40V
			{
				DeepPowerDown();									// ��ȵ�Դ�½�ģʽ
			}
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
				Terminal_Main_Battery();								// I2��ع���
			#endif
			break;
		case 1:														// 1=��������ת��ع���������
			DeepPowerDown();										// ��ȵ�Դ�½�ģʽ
			break;
		case 2:														// 2=��������ת��ع���
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
				if(Comm_Ram->UBACKUP<RechargeableBatteryStartMin)		// �ɳ�������������С��ѹ4.80V
				{
					DeepPowerDown();									// ��ȵ�Դ�½�ģʽ
					return;
				}
				Terminal_PowerDown();									// 2=��������ת��ع���
				Terminal_PowerDownTask();
				
			#elif ((Project/100)==5)
				//err20190304    II�ͼ��������紦��
				JZQ_II_Terminal_PowerDown();									// 2=��������ת��ع���
				//Terminal_PowerDownTask();

				MultiFrameSendTask(GPRSPORT);
				GPRSModuleManage(GPRSPORT);
				TerminalReportTask(GPRSPORT);
				UartPortManage(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
				UartTaskProcess(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
				
			#endif
			
			break;
		case 3:														// 3=��������
			Terminal_Main_PowerUp();								// I2�����е�
			break;
	}
}






