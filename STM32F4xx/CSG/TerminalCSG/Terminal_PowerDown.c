
//终端电源下降
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"

#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "../QCSG_Uart/Terminal_ReadMeter_ACSample.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_IO.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"

#include "../MS/Data_Save.h"
#include "../GPRS/GPRS_QCSG.h"
#include "Terminal_Task.h"
#include "Terminal_PowerDown.h"

#ifdef IC_STM32F4xx
#include "../STM32F4xx/STM32F4xx_MAC.h"
#endif
#ifdef IC_NUC9xx
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"
#endif


void Terminal_PowerDown(void)		// 2=电网供电转电池供电
{
	u32 PORTn;
  	UARTCtrl_TypeDef * UARTCtrl;
	RealTimeStruct RealTime;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	PORTn = 0;
	
	if(UART1Ctrl->LinkTask >= 101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		PORTn = GPRSPORT;
	}
	else
	{
		UART1Ctrl->LinkTask = 1;	// 1=关总电源
	}
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask >= 101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		PORTn = ENETPORT;
	}
	else
	{
		UARTCtrl->LinkTask = 1;		// 1=关总电源
	}

	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);	
	switch(pTerminalRam->PowerDownTask)//电源掉电过程任务
	{
		case 0://掉电事件产生
			DisplayStringAutoUp(0,9,(u8*)"\x0""电网停电");		// 显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			pCommRam->BattWork_S_timer = 180;					// 180;//4 电池工作关机秒定时器
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 1://检查当前是否在线
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;							// 1=关电源
				UART1Ctrl->PORT=1;								// 本机端口号,同时用于掉电时已连接过1次识别
				UnInit_GPRS_QCSG();								// GPRS模块引脚为输入下拉
				OffR5V0();										//关R5V0电源
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				UARTCtrl->LinkTask=1;//1=关电源
				UARTCtrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
			#ifdef IC_STM32F4xx
				UnInit_STM32F4xx_MAC();
				OffE5V0();//关E5V0电源
			#endif
			#ifdef IC_NUC9xx
				UnInit_NUC97x_MAC0();
				UnInit_NUC97x_MAC1();
				OffE5V0();//关E5V0电源
				Off2E5V0();//关E5V0电源
			#endif
				pTerminalRam->PowerDownTask=50;//电源掉电过程任务.无上报
			}
			else
			{
				if(PORTn==GPRSPORT)		
				{												// GPRS 在线，以太网不在线，则关闭以太网电源
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;//1=关电源
					UARTCtrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
				#ifdef IC_STM32F4xx
					//UnInit_GPRS_QCSG();								// GPRS模块引脚为输入下拉
					UnInit_STM32F4xx_MAC();
					OffE5V0();//关E5V0电源
				#endif
				#ifdef IC_NUC9xx
					UnInit_NUC97x_MAC0();
					UnInit_NUC97x_MAC1();
					OffE5V0();//关E5V0电源
					Off2E5V0();//关E5V0电源
				#endif
				}
				else
				{												// GPRS 不在线，太网不在线，则关闭 GPRS 电源
					UART1Ctrl->LinkTask=1;						// 1=关电源
					UART1Ctrl->PORT=1;							// 本机端口号,同时用于掉电时已连接过1次识别
					UnInit_GPRS_QCSG();							// GPRS模块引脚为输入下拉
					OffR5V0();									// 关R5V0电源
				}
				pCommRam->BattWork_S_timer = 60;				// 设置 1 分钟掉电告警延时计数
				pTerminalRam->PowerDownTask++;					// 电源掉电过程任务
			}
			break;
		case 2://在定时内等待上报
			if(pCommRam->BattWork_S_timer == 0)					// 电池工作关机秒定时器
			{
				pTerminalRam->PowerDownTask++;					// 电源掉电过程任务
				HAL_GetSystemTime(&RealTime);
				GenerateTerminalPowerUpAndDownAlarm(&RealTime, 0);
				pTSysCtrl->Flag |= TERMINAL_POWER_DOWN_DONE_FLAG;
				pCommRam->BattWork_S_timer = 120;				// 设置上报告警数据时间为 1分钟
			}
			break;
		case 3:
			
#if (TEST_ON == 1)//台体测试时至少保持在线3分钟
			if(pCommRam->BattWork_S_timer == 0)
			{
				pTerminalRam->PowerDownTask++;
			}
#else
			if((pTSysCtrl->LatelyAlarmNum == 0) || (pCommRam->BattWork_S_timer == 0))
			{
				pTerminalRam->PowerDownTask++;
			}
#endif
			break;
		case 4://发退出登录
			DisplayStringAutoUp(0,9,(u8*)"\x0""停电退出登录");	// 显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol

#if (TEST_ON == 1)
			pCommRam->BattWork_S_timer = 15;
#else 
			pCommRam->BattWork_S_timer = 120;					// 等待发发退出登录定时 //err20180228-9 断电时间15改为120
#endif
			UARTCtrl->LinkTask = 102;
			pTerminalRam->ENETHeartBeat_S_Timer=0;				// 2 以太网心跳秒定时器
			pTerminalRam->GPRSHeartBeat_S_Timer=0;				// 5 GPRS心跳秒定时器
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 5://等待退出登录
			if((UARTCtrl->LinkTask == 103)||(pCommRam->BattWork_S_timer==0))//电池工作关机秒定时器
			{
				pTerminalRam->PowerDownTask++;					// 电源掉电过程任务
			}
			break;
		case 6://关机
			UART1Ctrl->LinkTask=104;							// 链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask=104;								// 链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 7://结束
			if(pCommRam->BattWork_S_timer==0)					//	电池工作关机秒定时器
			{
				Battery_Data_Save();							// 电池工作时关机前数据保存
				DeepPowerDown();								// 深度电源下降模式
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if((UART1Ctrl->LinkTask==2)||(UARTCtrl->LinkTask==2))//开总电源(等待关机延时)
			{
				Battery_Data_Save();							// 电池工作时关机前数据保存
				DeepPowerDown();								// 深度电源下降模式
			}
			break;
		
		case 50://无上报
			DisplayStringAutoUp(0,9,(u8*)"\x0""不在线不上报");	// 显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			//pCommRam->BattWork_S_timer = 5;						// 4 电池工作关机秒定时器 //err20180228-9 去掉该行
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 51://等待定时后关机
			if(pCommRam->BattWork_S_timer==0)					// 电池工作关机秒定时器
			{
				Battery_Data_Save();						// 电池工作时关机前数据保存
				DeepPowerDown();							// 深度电源下降模式
			}
			break;	
		default:
			pTerminalRam->PowerDownTask=0;						// 电源掉电过程任务
			break;
	}
}





//err20190304    II型集中器掉电处理
void JZQ_II_Terminal_PowerDown(void)		// 2=电网供电转电池供电
{
	u32 PORTn;
  	UARTCtrl_TypeDef * UARTCtrl;
	RealTimeStruct RealTime;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	PORTn = 0;

	
	//II型集中器在线灯和信号强、弱灯处理
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UARTCtrl->LinkTask == 101)
	{
		Terminal_Ram->GPRS_ONLINELED_MS_Timer = 2000/10;
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
	}
	
	if(UART1Ctrl->LinkTask >= 101)//+链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		PORTn = GPRSPORT;
	}
	else
	{
		UART1Ctrl->LinkTask = 1;	// 1=关总电源
	}
//	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
//	if(UARTCtrl->LinkTask >= 101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
//	{
//		PORTn = ENETPORT;
//	}
//	else
//	{
//		UARTCtrl->LinkTask = 1;		// 1=关总电源
//	}

	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);	
	switch(pTerminalRam->PowerDownTask)//电源掉电过程任务
	{
		case 0://掉电事件产生
			//DisplayStringAutoUp(0,9,(u8*)"\x0""电网停电");		// 显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			//pCommRam->BattWork_S_timer = 180;					// 180;//4 电池工作关机秒定时器
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 1://检查当前是否在线
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;							// 1=关电源
				UART1Ctrl->PORT=1;								// 本机端口号,同时用于掉电时已连接过1次识别
				UnInit_GPRS_QCSG();								// GPRS模块引脚为输入下拉
				OffR5V0();										//关R5V0电源
				//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				//UARTCtrl->LinkTask=1;							// 1=关电源
				//UARTCtrl->PORT=1;								// 本机端口号,同时用于掉电时已连接过1次识别
				UnInit_STM32F4xx_MAC();
				OffE5V0();//关E5V0电源
				pTerminalRam->PowerDownTask = 50;				// 电源掉电过程任务.无上报
			}
			else
			{
				if(PORTn==GPRSPORT)		
				{												// GPRS 在线，以太网不在线，则关闭以太网电源
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;						// 1=关电源
					UARTCtrl->PORT=1;							// 本机端口号,同时用于掉电时已连接过1次识别
					UnInit_STM32F4xx_MAC();
					OffE5V0();									// 关E5V0电源
				}
				//else
				//{												// GPRS 不在线，太网不在线，则关闭 GPRS 电源
				//	UART1Ctrl->LinkTask=1;						// 1=关电源
				//	UART1Ctrl->PORT=1;							// 本机端口号,同时用于掉电时已连接过1次识别
				//	UnInit_GPRS_QCSG();							// GPRS模块引脚为输入下拉
				//	OffR5V0();									// 关R5V0电源
				//}
				pCommRam->BattWork_S_timer = 5;				// 设置 1 分钟掉电告警延时计数
				pTerminalRam->PowerDownTask++;					// 电源掉电过程任务
			}
			break;
		case 2://在定时内等待上报
			if(pCommRam->BattWork_S_timer == 0)					// 电池工作关机秒定时器
			{
				pTerminalRam->PowerDownTask++;					// 电源掉电过程任务
				HAL_GetSystemTime(&RealTime);
				GenerateTerminalPowerUpAndDownAlarm(&RealTime, 0);
				pTSysCtrl->Flag |= TERMINAL_POWER_DOWN_DONE_FLAG;
				pCommRam->BattWork_S_timer = 60;				// 设置上报告警数据时间为 1分钟
			}
			break;
		case 3:
			
#if (TEST_ON == 1)//台体测试时至少保持在线3分钟
			if(pCommRam->BattWork_S_timer == 0)
			{
				pTerminalRam->PowerDownTask++;
			}
#else
			if((pTSysCtrl->LatelyAlarmNum == 0) || (pCommRam->BattWork_S_timer == 0))
			{
				pTerminalRam->PowerDownTask++;
			}
#endif
			break;
		case 4://发退出登录
			DisplayStringAutoUp(0,9,(u8*)"\x0""停电退出登录");	// 显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol

#if (TEST_ON == 1)
			pCommRam->BattWork_S_timer = 15;
#else 
			pCommRam->BattWork_S_timer = 120;					// 等待发发退出登录定时 //err20180228-9 断电时间15改为120
#endif
			UART1Ctrl->LinkTask = 102;
			//pTerminalRam->ENETHeartBeat_S_Timer=0;				// 2 以太网心跳秒定时器
			pTerminalRam->GPRSHeartBeat_S_Timer=0;				// 5 GPRS心跳秒定时器
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 5://等待退出登录
			if((UART1Ctrl->LinkTask == 103)||(pCommRam->BattWork_S_timer==0))//电池工作关机秒定时器
			{
				pTerminalRam->PowerDownTask++;					// 电源掉电过程任务
			}
			break;
		case 6://关机
			UART1Ctrl->LinkTask=104;							// 链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			//UARTCtrl->LinkTask=104;								// 链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 7://结束
			if(pCommRam->BattWork_S_timer==0)					//	电池工作关机秒定时器
			{
				Battery_Data_Save();							// 电池工作时关机前数据保存
				DeepPowerDown();								// 深度电源下降模式
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if((UART1Ctrl->LinkTask==2)||(UARTCtrl->LinkTask==2))//开总电源(等待关机延时)
			{
				Battery_Data_Save();							// 电池工作时关机前数据保存
				DeepPowerDown();								// 深度电源下降模式
			}
			break;
		
		case 50://无上报
			DisplayStringAutoUp(0,9,(u8*)"\x0""不在线不上报");	// 显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			pCommRam->BattWork_S_timer = 0;						// 4 电池工作关机秒定时器 //err20180228-9 去掉该行
			pTerminalRam->PowerDownTask++;						// 电源掉电过程任务
			break;
		case 51://等待定时后关机
			if(pCommRam->BattWork_S_timer==0)					// 电池工作关机秒定时器
			{
				Battery_Data_Save();						// 电池工作时关机前数据保存
				DeepPowerDown();							// 深度电源下降模式
			}
			break;	
		default:
			pTerminalRam->PowerDownTask=0;						// 电源掉电过程任务
			break;
	}
}







