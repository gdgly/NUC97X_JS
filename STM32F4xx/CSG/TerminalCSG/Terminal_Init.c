

#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../TerminalCSG/Terminal_Init.h"
#include "../MS/SetInit.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_DAC.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Device/NAND.h"

#include "../Test/Test_RAM.h"

#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/CH_PLC_AMR_Task.h" 
#include "../QCSG_Head/CH_RS485_AMR_Task.h" 
#include "../QCSG_Head/QCSG_Hal.h"

#include "../MS/msFILE.h"
#include "../MS/MS.h"


void Terminal_Reset_Count(void);
void GeneralGPIOInit(void);
void JZQ_II_GeneralGPIOInit(void);

#if ((Project/100)==2)||((Project/100)==9)		// I 型集中器 配变
void Init_Terminal(void)//启动初始化
{
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;

	GeneralGPIOInit();
	if(pCommRam->POWER < 3)	return;		// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电

	Terminal_UartInit_Manage();			
	Terminal_Reset_Count();
	CH_AMR_TaskCtrlDataValidCheck(RS485_1PORT);
	CH_AMR_TaskCtrlDataValidCheck(RS485_2PORT);
	CH_AMR_TaskCtrlDataValidCheck(RS485_3PORT);
	CH_AMR_TaskCtrlDataValidCheck(PLC_PORT);
	pTerminalRam->MeterSyncToRam_S_Timer = 0;
	MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;	
}

#endif


#if ((Project/100)==3)	//负控终端
void Init_Terminal(void)//启动初始化
{
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;	
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	
	GeneralGPIOInit();
	if(pCommRam->POWER < 3)	return;		// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	Pin_Configure(PIN_SWITCH_OUT1,	// 继电器输出控制 1
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_SWITCH_OUT2,	// 继电器输出控制 2
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_SWITCH_OUT3,	// 继电器输出控制 3
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_SWITCH_OUT4,	// 继电器输出控制 4
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ALARM_OUT,	// 继电器报警输出控制 1
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
		 
	Pin_Configure(PIN_CH_CTL0,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_CH_CTL1,//
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_CT_KEY,//
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_CT_A_CHK,//err20181108 CT二次开路 模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_CT_B_CHK,//err20181108 CT二次开路 模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1


	Terminal_UartInit_Manage();//Terminal_Uart初始化管理
	Terminal_Reset_Count();//终端复位次数
#ifdef FKGA43_TEST_Alarm_CT
	onSquareWave(6000);
#else
	Pin_Configure(PIN_DAC1,//err20181108 CT二次开路 模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_CT_A_CHK,//err20181108 CT二次开路 模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_CT_B_CHK,//err20181108 CT二次开路 模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1 
#endif
	pTerminalRam->MeterSyncToRam_S_Timer = 0;
	MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;

	
}

#endif

#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)		

void GeneralGPIOInit(void)
{
	u32 i;
	u32 x;
	u16 *p16;
	u32 *p32;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
#if (TEST_ON == 1)//err20180626 上电后就同步节点状态C0F至RAM，避免产生遥信变位告警
	LoadManagementParaStruct* pstCtrl_Param_Info;
	QCSG_CTRL_INFO_S* pstCtrl_Info;
	QCSG_ALARM_E2000039_INFO_S* pstE2000039_Alarm_Info;
	u32 u32ReadLen;
#endif
	
	//QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;//err20181031 初始化负控抄表数据区
	
	//IRAM数据清0(除显示外)
	pCommRam->ErrorIC |= RAM_Test(ADDR_TerminalDisplay_IRAM_End, ADDR_Terminal_IRAM_End - ADDR_TerminalDisplay_IRAM_End);//RAM测试;返回:0=正确,1=错误

	//ERAM数据清0
	p32 = (u32*)ADDR_SDRAM_CLR_START;
	x = ADDR_ERAM_END - ADDR_SDRAM_CLR_START;
	x /= 4;
	for(i = 0; i < x; i++)
	{
		p32[i] = 0;	//err20180529 笔误 原为p32[0] 未对ERAM数据区初始化 导致周期抄表存储的数据错误(目前只在II型集中器中出现错误情况)
	}
	pTerminalRam->S_PR = 60;			// 60秒分频产生分
	pCommRam->BattWork_S_timer = 30;	// 电池工作关机秒定时器

	// 以下只有在电网有电时进行
	if(pCommRam->POWER < 3)	return;		// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	
//引脚初始化
	Pin_Configure(PIN_UART5_OE,	//抄表 485_1 收发控制
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_UART7_OE,	//抄表 485_2 收发控制
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_USART3_OE,	//抄表 485_3 收发控制
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1		 
	Pin_Configure(PIN_PLC_PWR,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ENE5V0,//以太网电源控制引脚
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	OnE5V0();		// 打开以太网电源
	Pin_Configure(PIN_ENR5V0,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	OffR5V0();		// 打开GPRS电源
	Pin_Configure(PIN_GPRS_PCTRL,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RST_GPRS,//输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ON_GPRS,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	/*Pin_Configure(PIN_ENUSB5V0,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	OnUSB5V0();		// 打开USB电源
	*/
	Pin_Configure(PIN_ENIO3V3,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	OnIO3V3();
	Pin_Configure(PIN_ENLED3V3,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	OnLED3V3();
	Pin_Configure(PIN_ENA5V0,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	OnA5V0();		// 打开计量芯片电源
	Pin_Configure(PIN_GPRS_ID0,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID1,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID2,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID3,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID4,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_BUZZ,
		 0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	//HAL_BuzzerON(BUZZ_MODE_00, 50);
	Pin_Configure(PIN_RUNLED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ATTR_LED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485T1_LED,//抄表485_1LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485R1_LED,//抄表485_1LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485T2_LED,//抄表485_2LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485R2_LED,//抄表485_2LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ENET_LINK_LED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ENET_ACT_LED,//
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_NEXTI,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_NEXTD,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ENTER,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ESC,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_LEFT,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RIGHT,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_PROKEY,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1

	Pin_Configure(PIN_BV7_A5V,	//电压检测，模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_BV6_V4V8,//电压检测，模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_BV5_V3V6,//电压检测，模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_BV3_V5V,//电压检测，模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_BV1_E3V3,//电压检测，模拟输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
		 
	MC(0,ADDR_TerminalRealTimer,1);						// 0 = 掉电时间未计算, 1 = 掉电时间已计算
	pTerminalRam->SlipCYC_S_Timer = 60;					// 23 终端输入脉冲功率计算和需量脉冲滑移秒定时器
	pTerminalRam->Power_S_Timer = 10;					// 38 停电事件秒定时器
	p16 = (u16 *)Get_ADDR_UARTnTimer_S(RS485_1PORT);
	p16[0] = 10;										// 10秒上电后延时抄表
	p16 = (u16 *)Get_ADDR_UARTnTimer_S(RS485_2PORT);
	p16[0] = 10;										// 10秒上电后延时抄表
	p16 = (u16 *)Get_ADDR_UARTnTimer_S(PLC_PORT);		// (集中器载波,专变控制模块)
	p16[0] = 10;										// 10秒上电后延时抄表
	p16 = (u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	p16[0] = 2000/10;									// 上电时置关机延时2s作为LED亮灯检查

	pTerminalRam->RS485_1_TxLED_MS_Timer=2000/10;		// 10 RS485_1抄表发送LED指示MS定时器,每10MS减1定时器
	pTerminalRam->RS485_1_RxLED_MS_Timer=2000/10;		// 11 RS485_1抄表接收LED指示MS定时器,每10MS减1定时器
	pTerminalRam->RS485_2_TxLED_MS_Timer=2000/10;		// 12 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
	pTerminalRam->RS485_2_RxLED_MS_Timer=2000/10;		// 13 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
	pTerminalRam->ENETLED_MS_Timer = 2000/10;			// 14 以太网接收发送LED指示MS定时器,每10MS减1定时器
	#if (Project/100) == 3
		pTerminalRam->ContrlLoopCheck_S_Timer=60;		// 36 控制输出回路检测有效等待秒定时器
	#endif
	pCommRam->CheckLEDTimer = 3000/10;					// 上电全部LED检查显示 每10MS减1定时器

#if (TEST_ON == 1)//err20180626 上电后就同步节点状态C0F至RAM，避免产生遥信变位告警

	pstCtrl_Param_Info = (LoadManagementParaStruct*)QCSG_CTRL_PARAM_INFO;
	pstCtrl_Info = (QCSG_CTRL_INFO_S*)QCSG_CTRL_INFO;
	pstE2000039_Alarm_Info = (QCSG_ALARM_E2000039_INFO_S*)QCSG_ALARM_E2000039_LIST;	
	u32ReadLen = 0;

	if(NAND_ReadPara(PARA_LoadManagement,0,0xE0000C00,(u8*)pstCtrl_Param_Info,&u32ReadLen) == NO_ERROR)
	{
		//更新遥信开关属性
		//err20181101 增加门节点上电同步状态。
		if((pstCtrl_Param_Info->SwitchNodeType[0] != 0x00) && (pstCtrl_Param_Info->SwitchNodeType[0] != 0x01))
		{
			pstCtrl_Info->u8Switch0Attr = 0x00;
		}
		else
		{
			//if(pstCtrl_Info->u8Switch0Attr != pstCtrl_Param_Info->SwitchNodeType[0])
			//{
			//	pstCtrl_Info->u32Switch0Timeout = u32CurrTick + 30 * 100;
			//}
			pstCtrl_Info->u8Switch0Attr = pstCtrl_Param_Info->SwitchNodeType[0];
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
		//更新告警Flag信息 防止上电报遥信变位告警
		pstE2000039_Alarm_Info->u8Alarm1Flag = pstCtrl_Info->u8Switch1Attr;
		pstE2000039_Alarm_Info->u8Alarm2Flag = pstCtrl_Info->u8Switch2Attr;
		pstE2000039_Alarm_Info->u8Alarm3Flag = pstCtrl_Info->u8Switch3Attr;
		pstE2000039_Alarm_Info->u8Alarm4Flag = pstCtrl_Info->u8Switch4Attr;
	}

#endif

}

#endif

#if ((Project/100)==5)	// II 型集中器

void On38KHz(void);//开38KHz时钟(TIM3_CH3)
void Init_Terminal(void)//启动初始化
{
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;

	JZQ_II_GeneralGPIOInit();
	if(pCommRam->POWER < 3)	return;		// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电

	Terminal_UartInit_Manage();			
	Terminal_Reset_Count();
	CH_AMR_TaskCtrlDataValidCheck(RS485_1PORT);
	CH_AMR_TaskCtrlDataValidCheck(RS485_2PORT);
	//CH_AMR_TaskCtrlDataValidCheck(PLC_PORT);
	pTerminalRam->MeterSyncToRam_S_Timer = 0;
	MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;	
	
	
//test start
//	On38KHz();//开38KHz时钟(TIM3_CH3)
	

//test end
}

void JZQ_II_GeneralGPIOInit(void)
{
	u32 i;
	u32 x;
	u16 *p16;
	u32 *p32;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	
	//IRAM数据清0(除显示外)
	pCommRam->ErrorIC |= RAM_Test(ADDR_TerminalDisplay_IRAM_End, ADDR_Terminal_IRAM_End - ADDR_TerminalDisplay_IRAM_End);//RAM测试;返回:0=正确,1=错误

	//ERAM数据清0
	p32 = (u32*)ADDR_SDRAM_CLR_START;
	x = ADDR_ERAM_END - ADDR_SDRAM_CLR_START;
	x /= 4;
	for(i = 0; i < x; i++)
	{
		p32[i] = 0;		//err20180529 笔误 原为p32[0] 未对ERAM数据区初始化 导致周期抄表存储的数据错误(目前只在II型集中器中出现错误情况)
		
	}
	pTerminalRam->S_PR = 60;			// 60秒分频产生分
	pCommRam->BattWork_S_timer = 30;	// 电池工作关机秒定时器

	// 以下只有在电网有电时进行
	if(pCommRam->POWER < 3)	return;		// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	
//引脚初始化
	Pin_Configure(PIN_UART5_OE,	//抄表 485_1 收发控制
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_UART7_OE,	//抄表 485_2 收发控制
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1	 
	Pin_Configure(PIN_ENR5V0,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	OffR5V0();		// 打开GPRS电源
	Pin_Configure(PIN_GPRS_PCTRL,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RST_GPRS,//输入
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ON_GPRS,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	/*Pin_Configure(PIN_ENUSB5V0,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
	OnUSB5V0();		// 打开USB电源
	*/
	Pin_Configure(PIN_GPRS_ID1,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID2,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID3,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1

	Pin_Configure(PIN_RUNLED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ATTR_LED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485T1_LED,//抄表485_1LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485R1_LED,//抄表485_1LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRSTX_LED,//GPRS数据发送灯
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRSRX_LED,//GPRS数据接收灯
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_LED_ONLINE,//GPRS在线灯
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1		 
//	Pin_Configure(PIN_ATTR_LED,//告警指示灯
//					0|//B3-B0:复用功能选择AF(0-15)
//		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
//		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
//		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
//		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
//		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_STRONG_SINGLE,//远程通道信号强度最强灯
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_WEAK_SINGLE,//远程通道信号强度最弱灯
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	
		 
	MC(0,ADDR_TerminalRealTimer,1);						// 0 = 掉电时间未计算, 1 = 掉电时间已计算
	pTerminalRam->SlipCYC_S_Timer = 60;					// 23 终端输入脉冲功率计算和需量脉冲滑移秒定时器
	pTerminalRam->Power_S_Timer = 10;					// 38 停电事件秒定时器
	p16 = (u16 *)Get_ADDR_UARTnTimer_S(RS485_1PORT);
	p16[0] = 10;										// 10秒上电后延时抄表
	p16 = (u16 *)Get_ADDR_UARTnTimer_S(RS485_2PORT);
	p16[0] = 10;										// 10秒上电后延时抄表
	

	pTerminalRam->RS485_1_TxLED_MS_Timer=2000/10;		// 10 RS485_1抄表发送LED指示MS定时器,每10MS减1定时器
	pTerminalRam->RS485_1_RxLED_MS_Timer=2000/10;		// 11 RS485_1抄表接收LED指示MS定时器,每10MS减1定时器
	pTerminalRam->RS485_2_TxLED_MS_Timer=2000/10;		// 12 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
	pTerminalRam->RS485_2_RxLED_MS_Timer=2000/10;		// 13 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
	pTerminalRam->GPRS_RxLED_MS_Timer = 2000/10;		//8   GPRS 接收LED 10MS定时器
	pTerminalRam->GPRS_TxLED_MS_Timer = 2000/10;    //9 	GPRS 发送LED 10MS定时器
	pTerminalRam->GPRS_ONLINELED_MS_Timer = 2000/10;        //19 GPRS在线LED指示MS定时器,每10MS减1定时器
	pTerminalRam->GPRS_WEAK_SINGLE_LED_MS_Timer = 2000/10;  //20 GPRS信号最弱LED指示MS定时器,每10MS减1定时器
	pTerminalRam->GPRS_STRONG_SINGLE_LED_MS_Timer= 2000/10; //21 GPRS信号最强LED指示MS定时器,每10MS减1定时器
	
	pCommRam->CheckLEDTimer = 3000/10;					// 上电全部LED检查显示 每10MS减1定时器
}

#endif




void Terminal_Reset_Count(void)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	u32 ResetCnt = 0;
	//u32 i = 0;
	//u32* p32;
	MR((u32)&ResetCnt, ADDR_TerminalResetCnt, 4);
	if(ResetCnt == 0xFFFFFFFF)
	{
		ResetCnt = 0;
		Init_ALLPara();
		MC(0, (u32)ADDR_RS485_CH1_AMR_TaskCtrl, sizeof(AMRCtrlStruct));
		MC(0, (u32)ADDR_RS485_CH2_AMR_TaskCtrl, sizeof(AMRCtrlStruct));
		MC(0, (u32)ADDR_RS485_CH3_AMR_TaskCtrl, sizeof(AMRCtrlStruct));
		MC(0, (u32)ADDR_PLC_AMR_TaskCtrl, sizeof(AMRCtrlStruct));

		MC(0, ADDR_E1800001, 256);	
		/*
		Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
		p32=(u32*)(ADDR_msFILEmanage);
		for(i=0;i<(LEN_msFILEmanage/4);i++)
		{
			p32[i]=0;
		}
		p32=(u32*)(ADDR_RAMmsFILEmanage);
		for(i=0;i<(LEN_RAMmsFILEmanage/4);i++)
		{
			p32[i]=0;
		}
		*/	
		pTSysCtrl->DayPowerDownCnt = 0;
		pTSysCtrl->DayPowerDownTime = 0;
		pTSysCtrl->MonthPowerDownCnt = 0;
		pTSysCtrl->MonthPowerDownTime = 0;

		MR(QCSG_POWER_DOWN_NUM_DAY, (u32)&pTSysCtrl->DayPowerDownCnt, 4);
		MR(QCSG_POWER_DOWN_TIME_DAY, (u32)&pTSysCtrl->DayPowerDownTime, 4);
		MR(QCSG_POWER_DOWN_NUM_MONTH, (u32)&pTSysCtrl->MonthPowerDownCnt, 4);
		MR(QCSG_POWER_DOWN_NUM_MONTH, (u32)&pTSysCtrl->MonthPowerDownTime, 4);
	}
	else
	{
		ResetCnt++;
		MW((u32)&ResetCnt, ADDR_TerminalResetCnt, 4);

		ResetCnt = 0;
		MR((u32)&ResetCnt, ADDR_E1800016, 2);
		ResetCnt = bcd_hex(ResetCnt);
		ResetCnt++;
		ResetCnt = hex_bcd(ResetCnt);
		MW((u32)&ResetCnt, ADDR_E1800016, 2);
	}
		
	pTSysCtrl->CheckMinute = 0xFF;
	pTSysCtrl->Flag |= TERMINAL_POWER_UP_FLAG;

}



/*
void Init_EnergyInc_Souser(void)//测量点电能增量计算用原始数据清为0xee
{
	u32 i;
//正向有功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_PKWH_INC+((1+(9*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
	}
//正向无功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_PKVARH_INC+((1+(8*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
	}
//反向有功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_NKWH_INC+((1+(9*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
	}
//反向无功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_NKVARH_INC+((1+(8*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
	}
}
void Init_IncRemainder(void)//总加组总加电能量增量小数位清为0x0
{
//当日总加有功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF19_Remainder,(4*(TMaxTariff+1)*AFN0CF19MaxPn));
//当日总加无功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF20_Remainder,(4*(TMaxTariff+1)*AFN0CF20MaxPn));
//当月总加有功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF21_Remainder,(4*(TMaxTariff+1)*AFN0CF21MaxPn));
//当月总加无功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF22_Remainder,(4*(TMaxTariff+1)*AFN0CF22MaxPn));
//终端当前剩余电量(费)增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF23_Remainder,(4*(TMaxTariff+1)*AFN0CF23MaxPn));
}
*/
/*
void Init_ReadMeter(void)//抄表初始化
{
  UARTCtrl_TypeDef * UARTCtrl;
	
	Terminal_Ram->ACSampleRM_S_Timer=0;
	Terminal_Ram->RS4851RM_S_Timer=0;
	Terminal_Ram->RS4852RM_S_Timer=0;
	Terminal_Ram->RS4853RM_S_Timer=0;
	if(Get_LEN_UARTnCtrl(ACSAMPLEPORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
	if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
	if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
	if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
}
*/

void Init_CSG(u8 Init_Mode)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	ms_Type *ms;
	CommunicationParaStruct CommPara;
	CommunicationParaStruct CommPara_Temp;
	//u32 i = 0;
	//AlarmSaveParaStruct AlarmSavePara;//err20190216 修改掉电存储标志的存储位置

	switch (Init_Mode)
	{
		case 0:
			break;
		case 1:
			DataResetAction(2,RS232PORT);//硬件复位
			break;
		case 2:
			DataResetAction(1,RS232PORT);//数据区初始化
			break;
		case 3:							//所有参数及数据区初始化
			MC(0, (u32)&CommPara_Temp, sizeof(CommunicationParaStruct));
			MR((u32)&CommPara_Temp, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			ME(ADDR_TERMINAL_PARA_START, 128*1024);
			ME(ADDR_MP_START, 256*1024);
			Init_ALLPara();
			MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			CommPara.TerminalAddr = CommPara_Temp.TerminalAddr;
			CommPara.TerminalLocationCode = CommPara_Temp.TerminalLocationCode;
			MW((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			ms = Get_ms();
			/*MaxFileName = FILENAMEmax_FLASH;
			for(i = 0; i < MaxFileName; i++)
			{
				ms->msfile_Delete(i);
				checkFile(i);
				pCommRam->msFILEchange = 0;
			}*/
			ms->msfile_format();
			MC(0, ADDR_E1800001, 256);
			QCSG_ClearStatisticsData();
			pTSysCtrl->DayPowerDownCnt = 0;
			pTSysCtrl->DayPowerDownTime = 0;
			pTSysCtrl->MonthPowerDownCnt = 0;
			pTSysCtrl->MonthPowerDownTime = 0;
			pTSysCtrl->LastAlarmDI = 0;
			pTSysCtrl->LatelyAlarmNum = 0;	
			pTSysCtrl->GPRSRxTxBytes = 0;//err20180404-20 通信流量清空
			MC(0, ADDR_RS485_CH1_AMR_TaskCtrl, LEN_RS485_CH1_AMR_TaskCtrl * 4);
			
			//MC(0, ADDR_GUIPASSWORD, 4);//菜单初始密码为0
			break;
		case 4:
		case 5:
			DataResetAction(0,RS232PORT);//除通信参数外所有参数及数据区初始化
			break;
		default:
			DataResetAction(2,RS232PORT);//硬件复位
			break;
			
	}
}



#include "../GPRS/ATNo.h"
void Init_Terminal_SoftwareUpdate(void)//软件更新初始化
{
	TerminalRunTimeCtrlStruct* pTsysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime; 

	Init_CSG(Terminal_UpDataInit);
	
	pTsysCtrl->Flag = 0;
	
	//MS在BOOT更新,可能因选择NAND128kBuff和NAND4kBuff不同,数据保护恢复可能错误,故在此先将数据写入FLASH
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
 NAND4kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
#else
 NAND128kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#endif 
}




