

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

void ACTION_RESET(void);		//复位命令操作

uint32_t SystemCoreClock = 16000000;	//仅stm32f4xx_hal_rcc.c文件编译时需要,link时不需要

void _0(void)
{
}


void ACTION_RESET(void)//复位命令操作
{
	u32 PORTn;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Comm_Ram->RESET != 0x55)//0x55=重启动(立即使用看门狗复位)
	{
		return;
	}
	PORTn = Comm_Ram->ACTION_RESET_PORTn;//操作复位的通信端口号
	switch(PORTn)
	{
		case RS232PORT:				// 0//RS232
		case GPRSPORT:				// 1//GPRS
		case RS485_1PORT:			// 2//RS485_1通信口号
		case RS485_2PORT:			// 3//RS485-2通信口号
		case RS485_3PORT:			// 5//RS485_3通信口号(WI-FI)
		case IRPORT:				// 6//红外通信口号
		case ENETPORT:				// 10//以太网通信口号
		case SMSPORT:				// 11//短信通信口号
			UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
			if(UARTCtrl->Task != 0)
			{// 0=正在接收
				return;
			}
	}
	//延时等待最后字节发送
	Comm_Ram->GPMSTimer = 500/10;	// 18 通用(兼虚拟UART11) 每10MS减1定时器
	while(Comm_Ram->GPMSTimer)
	{
	}
	WWDT_RESET();//立即使能看门狗复位
}

void SdramTest(void)
{
#ifdef IC_STM32F4xx
	u32* p32;
	if(RAM_Test(ADDR_128KDATABUFF,64))
	{
		Comm_Ram->GPMSTimer=1000/10;		//18 通用(兼虚拟UART11) 每10MS减1定时器
		while(1)
		{
			Init_ExtMem();					//sdram初始化
			SDRAM_FMC_Init();
			if(RAM_Test(ADDR_128KDATABUFF,64)==0)		//RAM测试;返回:0=正确,1=错误
			{
				break;
			}
			if(Comm_Ram->GPMSTimer==0)					//18 通用(兼虚拟UART11) 每10MS减1定时器
			{
				__disable_irq();
				p32=(u32*)(ADDR_Meter_SaveRam_Start+LEN_SaveRam-8);
				p32[0]=0x12345678;
				p32[1]=RAM_CRC32(ADDR_Meter_SaveRam_Start,LEN_SaveRam-4);		//RAM中数据CRC32计算
				RTC->BKP0R&=0xffff00ff;										//B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
				RTC->BKP0R|=0x5500;
				//	RCC->CSR|=1;//LSI RC 振荡器开启
				while(IWDG->SR!=0)
				{
				}
				IWDG->KR=0x5555;
				IWDG->PR=0;	// 4 分频
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

#if(TEST_ON == 1)//err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。
		QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)(ADDR_LCD_MeterAMRDataBuf);
#endif

	CommunicationParaStruct CommPara;
	UARTCtrl_TypeDef* UartCtrl;
	u32 TimeSub[4];
	u32 i = 0;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	u8* pPowerDownTime = (u8*)ADDR_TFM_YMDHMS;		// 掉点前记录的时间
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
	
	
#if ((Project/100)==5) //err20181122 增加 日/月冻结 终端停电次数及时间 用到

	u32 TempPowerDownTime;
	u8 TempTime[8];//err20181212 修改停电冻结数据错误
	RealTimeStruct RealTime;
	HAL_GetSystemTime(&RealTime);
#endif

	if(pTerminalRam->PowerDownTask != 0)
	{												// 此处为清除休眠醒来后，保存的休眠状态，避免液晶屏熄灭前再次来电后休眠状态不变
		pTSysCtrl->Flag |= TERMINAL_POWER_UP_FLAG;
		pTerminalRam->PowerDownTask = 0;
	}
	
//err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。
#if(TEST_ON == 1)
	if((Pn_Lcd_Data->u16Res1 & LCD_RT_AMR_REQ_TEST_START) != 0)
	{
		if(pTerminalRam->Seconds - pTerminalRam->LCD_RT_AMR_REQ_TEST_START_TIME >= 20)//延时大于20s
		{
			Pn_Lcd_Data->u16Res1 &= ~LCD_RT_AMR_REQ_TEST_START;
			Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ_TEST_END;
			pTerminalRam->LCD_RT_AMR_REQ_TEST_START_TIME = 0;
		}
	}
#endif
	
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)	//II型集中器不判编程键
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
#if ((Project/100)==5) //II型集中器在线灯和信号强、弱灯处理
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
		#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)       //II型集中器不判编程键20180515ybzyl
			if(pTSysCtrl->ProKeyLastSeconds > 0) pTSysCtrl->ProKeyLastSeconds--;
			if(pTSysCtrl->ProKeyLastSeconds == 0) pTSysCtrl->Flag &= ~TERMINAL_PROKEY_PRESS_FLAG;
		#endif

		#if ((Project/100)==5) //II型集中器告警指示灯处理 有告警时亮一秒灭一秒
			if(pTSysCtrl->LatelyAlarmNum != 0)//err20190108 增加II型集中器告警灯处理
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
//err20181112 增加告警 CT二次侧开路短路 建议测试时打开，实际使用时关闭 防止误报。
	/**************************************************ybwhz 测试段 1s执行一次****************************************************/
/*检测CT二次开路短路
每分钟轮流AD采样A/B/C相激励电压 
等待采样稳定时判断电压大小 产生告警
引脚:
CT_A_CHK --- PF9  --- AD3_IN7 	7.2倍放大(暂时不用)
CT_B_CHK --- PF10 --- AD3_IN8 	11倍放大(目前使用该通道)
CH_CTL0  --- PK1  				74HC4052芯片通道选择
CH_CTL1  --- PK0   				74HC4052芯片通道选择
CTT      --- PA4				激励波形DAC输出口(经测试 方波6kHZ左右 激励电压最高2.0V,正弦波3k-50k 激励电压1.24V)
CT_KEY   --- PJ8 				ADC引脚接地开关(默认低电平，保持AD端口有效)

74HC4052芯片通道选择:
 相       引脚       CTL0  CTL1
 A相 --- T2IN ---    0     0
 B相 --- T4IN ---    0	   1
 C相 --- T6IN ---    1		 0
*/
#if ((Project/100) == 3)
#ifdef FKGA43_TEST_Alarm_CT
		switch(pTSysCtrl->CheckSecond/16)//BCD取第一位
		{
			case 0:
			case 1:
				Pin_OutL(PIN_CH_CTL0);Pin_OutL(PIN_CH_CTL1);//A相 线圈	//引脚输出低,入口引脚使用名
				//DisplayData(1 ,4,2,(u16 *)(ADDR_STRINGBUFF+(4*84)+4));
				break;
			case 2:
			case 3:
				Pin_OutL(PIN_CH_CTL0);Pin_OutH(PIN_CH_CTL1);//B相 线圈	//引脚输出低,入口引脚使用名
				//DisplayData(2 ,4,2,(u16 *)(ADDR_STRINGBUFF+(4*84)+4));
				break;
			case 4:
			case 5:
				Pin_OutH(PIN_CH_CTL0);Pin_OutL(PIN_CH_CTL1);//C相 线圈	//引脚输出低,入口引脚使用名
				//DisplayData(3 ,4,2,(u16 *)(ADDR_STRINGBUFF+(4*84)+4));
				break;
			default: break;
		}
#ifdef IC_STM32F4xx
		Voltage = Get_ADC3(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
		//DisplayData(Voltage ,4,2,(u16 *)(ADDR_STRINGBUFF+(3*84)+4));
#endif
#ifdef IC_NUC9xx
		Voltage = ADCx(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
#endif
		
		

		
		if(pTSysCtrl->CheckSecond == 0x10) //测量A相
		{
			#ifdef IC_STM32F4xx
				Voltage = Get_ADC3(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
			#endif
			#ifdef IC_NUC9xx
				Voltage = ADCx(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
			#endif
			if(Voltage < CT_OPEN_LIMIT_VALUE)	// 开路 一般为0.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000A,1);//A相 开路产生
			else 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000A,0);//A相 开路恢复

			if(Voltage > CT_SHORT_LIMIT_VALUE)	// 短路 一般为1.99-2.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000007,1);//A相 短路产生
			else
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000007,0);//A相 短路恢复
		}

		if(pTSysCtrl->CheckSecond == 0x30) //测量B相
		{
			#ifdef IC_STM32F4xx
				Voltage = Get_ADC3(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
			#endif
			#ifdef IC_NUC9xx
				Voltage = ADCx(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
			#endif
			if(Voltage < CT_OPEN_LIMIT_VALUE)	// 开路 一般为0.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000B,1);//B相 开路产生
			else 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000B,0);//B相 开路恢复

			if(Voltage > CT_SHORT_LIMIT_VALUE)	// 短路 一般为1.99-2.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000008,1);//B相 短路产生
			else
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000008,0);//B相 短路恢复
		}

		if(pTSysCtrl->CheckSecond == 0x50) //测量C相
		{
			#ifdef IC_STM32F4xx
				Voltage = Get_ADC3(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
			#endif
			#ifdef IC_NUC9xx
				Voltage = ADCx(8,1,1,0);//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
			#endif
			if(Voltage < CT_OPEN_LIMIT_VALUE)	// 开路 一般为0.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000C,1);//C相 开路产生
			else 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE200000C,0);//C相 开路恢复

			if(Voltage > CT_SHORT_LIMIT_VALUE)	// 短路 一般为1.99-2.08V 
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000009,1);//C相 短路产生
			else
				GenerateCTOpenOrShortCircuitAlarm_E2000007_0C(0xE2000009,0);//C相 短路恢复
		}


#endif
#endif
	/**************************************************ybwhz 测试段 1s执行一次****************************************************/
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
		 	//err20190304 I型集中器掉电时间超过1min判断掉电事件与告警。
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
					if(TimeSub[3] > 0x0A)//掉电事件超过10s即记录
					{
						GeneratePowerEvent((RealTimeStruct*)pCommRam->MYMDHMS, (RealTimeStruct*)pPowerDownTime);
					}
				}

			#elif ((Project/100) == 5) //err20190304 II型集中器掉电时间超过5s判断掉电事件与告警。
				if((TimeSub[0] > 0x00) || (TimeSub[1] > 0x00) || (TimeSub[2] > 0x00) || (TimeSub[3] > 0x05))//err20190423 完善判定条件 否则1分04秒可能不产生告警
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
					//掉电跨日
					#if ((Project/100)==5)
						//err20181212 计算上一日至日末时间用于冻结
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
						//此处暂时不考虑停电跨多日补冻结的情况！
						SaveTerminalPowerDownFreezData(AMR_DailyStatisticFreeze,&RealTime);	//err20181122 增加日冻结停电次数
					#endif
					
						//计算当日掉电时间
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
						pTSysCtrl->DayPowerDownTime = TimeSub[0] * 24 * 60;//err20181212 日停电时间不会超过1*24*60
						pTSysCtrl->DayPowerDownTime += TimeSub[1] * 60;
						pTSysCtrl->DayPowerDownTime += TimeSub[2];
					}
					if((TimeFlag & 0x02) != 0)
					{
						//掉电跨月

					#if ((Project/100)==5)
						//err20181212 计算上一月至月末时间用于冻结
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
						//此处暂时不考虑停电跨多月补冻结的情况！
						SaveTerminalPowerDownFreezData(AMR_MonthStatisticFreeze,&RealTime); //err20181122 增加月冻结停电次数
					#endif

						//计算当月掉电时间
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
			Comm_Ram->Init_OS|=1<<7;//OS需要重启原因:B0=????,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
			ReSTART_OS();//重启OS进入硬件自检程序
			while(1)
			{
			}
		}
		if((pTSysCtrl->Flag & TERMINAL_ADDR_CHANGE_FLAG) != 0)
		{			
			#if(USER != 3400) //ybzyl20181031 负控云南版技术支持吴累累联调时提出，负控设置完行政区划码或逻辑地址后，负控不要重启，以免被认为是程序错误
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
			//err20190708 南网负控修改终端地址后不重启，该标志不被清零将导致GPRS模块状态被卡在LinkTask=1
			//无法启动，所以在此处必须清零标志。
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
			//读取测量点接线方式、额定电压、额定电流
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
				//满足掉电条件	
				//pTSysCtrl->Flag |= TERMINAL_POWER_DOWN_DONE_FLAG;
			}
			else
			{	//不满足掉电情况
				MW((u32)pCommRam->MYMDHMS, (u32)pPowerDownTime, 6);//掉电前时钟更新
				pPowerDownTime[6] = 0x20;
			}
		#else 
		*/
		MW((u32)pCommRam->MYMDHMS, (u32)pPowerDownTime, 6);//掉电前时钟更新
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
				UartCtrl->LinkCtrl = 1;//err20190530云南负控有自适应参数，所以不关闭GPRS模块
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
		i += pTSysCtrl->GPRSRxTxBytes >> 10;	//流量单位: kb	
		i = hex_bcd(i);
		MW((u32)&i, ADDR_E1800013, 3);		
		i = pTSysCtrl->GPRSRxTxBytes >> 10;
		i = hex_bcd(i);
		MW((u32)&i, ADDR_E1800012, 2);
		pTSysCtrl->GPRSRxTxBytes = 0;

	#if ((Project/100)==5) //err20181122 增加 日/月冻结 终端停电次数及时间		
		if((TimeFlag & 0x01) == 0)//err20181211 若不是停电跨日，则清零日次数及时间
		{
			SaveTerminalPowerDownFreezData(AMR_DailyStatisticFreeze,&RealTime);	//err20181122 增加日冻结停电次数
				
			pTSysCtrl->DayPowerDownCnt = 0;
			pTSysCtrl->DayPowerDownTime = 0;
			MR(QCSG_POWER_DOWN_NUM_DAY, (u32)&pTSysCtrl->DayPowerDownCnt, 4);//err20181122 原被屏蔽 跨日不更新
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
		
	#if ((Project/100)==5) //err20181122 增加 日/月冻结 终端停电次数及时间
		if((TimeFlag & 0x02) == 0)//err20181211 若不是停电跨月，则清零月次数及时间
		{	
			SaveTerminalPowerDownFreezData(AMR_MonthStatisticFreeze,&RealTime); //err20181122 增加月冻结停电次数
			
			pTSysCtrl->MonthPowerDownCnt = 0;
			pTSysCtrl->MonthPowerDownTime = 0;
			MR(QCSG_POWER_DOWN_NUM_MONTH, (u32)&pTSysCtrl->MonthPowerDownCnt, 4);//err20181122 原被屏蔽 跨月不更新
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
void Terminal_Task(void)			// 分时任务(电网有电)
{
	//u8* Test_data = (u8*)(ADDR_EFLASH_Start+5);
	//u8 Test_data2[10] = {1,2,3,4,5,6,7,8,9,10};
	//MemCopy(Test_data,Test_data2,5);
	
	
	NAND_SyncMeasurePointToRam();//err20180529 换一下顺序 上电后测量点同步完成后再产生停上电告警
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
	
	#if ((Project/100)==3)	//专变
		RD_Meter_Task();
		QCSG_Ctrl_Task();
	#endif
	#if ((Project/100)==2)		//I型集中器
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
void Terminal_Task(void)			// 分时任务(电网有电)
{
	
	NAND_SyncMeasurePointToRam();
	Terminal_RunTimeProcess();//err20180529 换一下顺序 上电后测量点同步完成后再产生停上电告警

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

__asm void Call_E2(u32 r0)//调用E2菜单,入口R0=PC地址
{
	PUSH {LR}
	LDR LR,=ADDR_SystemStack
	STR SP,[LR]
	CMP R0,#0
	BNE Call_E2_1
	LDR R0,=ADDR_Menu_ENTRY//E2入口地址
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
	
	if(MenuCount->Count==0)//菜单堆栈计数
  	{	//菜单堆栈计数=0
    	if((Comm_Ram->ButtonVal&0x10)==0)
    	{//无确认键按下，循显
				if(Comm_Ram->DisplayScanTimer==0x0)
				{
					//点阵显示输出
					if(DOTMIXPMODE->Task==0)
					{//上电屏幕
							DisplayClr();//清全屏
							Startdisplay();
							HAL_BuzzerON(BUZZ_MODE_00, 50);
							Comm_Ram->DisplayScanTimer=30/10;							
					}
					else
					{//其他屏幕
						Comm_Ram->DisplayScanTimer=1000/10;        //配变取DLT500报告时修改为1s钟刷一次
					}
					
				#ifdef IC_STM32F4xx
					if(((RTC->BKP0R&0xff)>=2)&&((RTC->BKP0R&0xff)<=4))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
				#endif
				#ifdef IC_NUC9xx
					if((FileDownload->USB0Task>=2)&&(FileDownload->USB0Task<=4))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
				#endif
					{//USB文件完成标志有效
						Comm_Ram->DisplayAllErrorTimer=0;
						Comm_Ram->DisplayScanTimer=5000/10;
						DOTMIXPMODE->Task=1;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
						DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
					}
					else
					{
						i=MRR(ADDR_FTPFileFlags+4,1);
						if((i<(MaxFileNum+3))&&(Comm_Ram->DisplayAllErrorTimer!=0))
						{//FTP文件下载有效
							DOTMIXPMODE->Task=1;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
							DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
							DisplayFTP();//FTP文件下载显示
						}
						else
						{
							if((i==(MaxFileNum+2))&&(Comm_Ram->DisplayAllErrorTimer==0))
							{
								MWR(MaxFileNum+3,ADDR_FTPFileFlags+4,1);//FTP文件下载且显示清除标志
							}
							#if ErrorAutoDisplay==1//0=允许,1=禁止;系统错误自动立即显示
								if(Comm_Ram->Factory!=0x55)//0x55=工厂状态
								{//不是工厂状态
									if(Comm_Ram->ErrorIC)
									{
										Comm_Ram->ErrorIC=0;
										Comm_Ram->DisplayAllErrorTimer=0;
									}
								}
							#endif
							if((Comm_Ram->ErrorIC)&&(Comm_Ram->DisplayAllErrorTimer))
							{//错误显示
								DOTMIXPMODE->Task=2;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
								DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
								DisplayError();//硬件和软件模块错误显示
							}
							else
							{
								//循环显示
								DOTMIXPMODE->Task=4;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
								DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
								DisplayLoop();//终端循显
							}
						}
					}
					LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
				}				
			}    
			else
			{//确认键按下，进主菜单
				DOTMIXPMODE->Task=3;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
				DOTMIXPMODE->Mix=2;//2;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
				Comm_Ram->ButtonVal&=~0x10;//确认键使用后清楚
				if(FileDownload->File&0x10)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
				{
					MenuCount->Count=1;
					p32=(u32 *)(ADDR_Menu_ENTRY);
					Stack->MenuStack[MenuCount->Count].PC=p32[1];
					Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
					Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
					#if ((Project/100)==3) || ((Project/100)==9)//负控终端&配变
						Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
					#endif
					#if ((Project/100)==2)//I型集中器
						Stack->MenuStack[MenuCount->Count].EditRow=2;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
					#endif
					Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
					Call_E2(0);//调用E2菜单
					DisplayClrRow(0,0);//清屏幕显示的指定行
					//DisplayClrRow(9,9);//清屏幕显示的指定行
					DisplayLoop_TopICO(0);//顶图标和时钟显示
					DisplayLoop_BottomICO(9);//底图标显示
				}
				//点阵显示输出
				Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
				LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
			}
			
	}		
	else
	{//菜单堆栈计数!=0
		if(FileDownload->File&0x10)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
		{
			Call_E2(Stack->MenuStack[MenuCount->Count].PC);//调用E2菜单
		}
		if(Comm_Ram->DisplayScanTimer==0x0)
		{
			//p8=(u8*)(ADDR_MenuICO);//1byte 菜单中显示顶行和底行图标 0=否,1=是
			//if(p8[0]!=0)
			//{
			DisplayClrRow(0,0);//清屏幕显示的指定行
			//DisplayClrRow(9,9);//清屏幕显示的指定行
			DisplayLoop_TopICO(0);//顶图标和时钟显示
			DisplayLoop_BottomICO(9);//底图标显示
			//}
			//点阵显示输出
			Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
		}
		if(Comm_Ram->MenuOutTimer==0)
		{//菜单退出 10MS定时器
			MenuCount->Count=0;
		}
	}
}

#endif

void Terminal_Main_PowerUp(void)
{
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
	Terminal_DisplayTask();			// 显示任务
#endif
	Terminal_Uart_Manage();			// Terminal_Uart管理
	
	Terminal_Task();				// 分时任务(电网有电)	
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

void Terminal_Main_Battery(void)			// 电池供电
{
#if ((Project/100) < 2)						//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块
	return;
#else
	if(Comm_Ram->BattWork_S_timer==0)		// 4 电池工作关机秒定时器
	{
		DeepPowerDown();					// 深度电源下降模式
	}
	else
	{
		Terminal_DisplayTask();//I2显示
	#ifdef IC_STM32F4xx
		if((RTC->BKP0R&0xff)<=1)//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
		{//无USB操作
			SCB->SCR = 0x0;//SLEEPDEEP=0
			__WFI();
		}
	#endif
		if(Pin_Read(PIN_WKUP)!=0)
		{
			WWDT_RESET();					// 立即使能看门狗复位
		}
	}		
#endif
}
#endif

void Terminal_Main(void)
{
	
	switch(Comm_Ram->POWER)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		case 0:														// 0=电池供电
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)		// 可充电池停止工作最电压4.40V
			{
				DeepPowerDown();									// 深度电源下降模式
			}
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
				Terminal_Main_Battery();								// I2电池供电
			#endif
			break;
		case 1:														// 1=电网供电转电池供电重启动
			DeepPowerDown();										// 深度电源下降模式
			break;
		case 2:														// 2=电网供电转电池供电
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
				if(Comm_Ram->UBACKUP<RechargeableBatteryStartMin)		// 可充电池启动工作最小电压4.80V
				{
					DeepPowerDown();									// 深度电源下降模式
					return;
				}
				Terminal_PowerDown();									// 2=电网供电转电池供电
				Terminal_PowerDownTask();
				
			#elif ((Project/100)==5)
				//err20190304    II型集中器掉电处理
				JZQ_II_Terminal_PowerDown();									// 2=电网供电转电池供电
				//Terminal_PowerDownTask();

				MultiFrameSendTask(GPRSPORT);
				GPRSModuleManage(GPRSPORT);
				TerminalReportTask(GPRSPORT);
				UartPortManage(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
				UartTaskProcess(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
				
			#endif
			
			break;
		case 3:														// 3=电网供电
			Terminal_Main_PowerUp();								// I2电网有电
			break;
	}
}






