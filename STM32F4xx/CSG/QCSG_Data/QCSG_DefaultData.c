
/************************* (C) COPYRIGHT 2017 CHINT ***************************
* File Name          : QCSG_DefaultData.c
* Author             : zhaoyun
* Version            : V1.0.0
* Date               : 2017/06/26
* Description        : This file Contains the Terminal Deault Data 
*******************************************************************************/




#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Data/QCSG_DefaultData.h"



#if ((Project/100 == 3) && (USER == 3400))//云南出厂默认通信参数
const CommunicationParaStruct DefaultCommuniactionPara = 
{
	{0x21,0x1C,0x74,0xA2,0x14,0x64, 0, 0},					// MasterAddr
	{0x21,0x1C,0x74,0xA2,0x14,0x64, 0, 0},					// MasterBackAddr1
	{0x21,0x1C,0x74,0xA2,0x14,0x64, 0, 0},					// MasterBackAddr2
	0x02,										// MasterAddrType
	0x02,										// MasterBackAddr1Type
	0x02,										// MasterBackAddr2Type
	0,											// CompressType
	{0, 0, 0, 0, 0, 0, 0, 0},					// GateWayAddr
	{											// APN
		'Y','N','D','W','F','K','.','Y','N',
	 	0, 0, 0, 0, 0, 0, 0
	},
	{											// APNUserName
		//'C', 'M', 'N', 'E', 'T', 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APNPassWord
		//'C', 'M', 'N', 'E', 'T', 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	0x05,										// HeartbeatTimeGap
	1,										// RedialTimeGap
	3,											// RedialCnt
	0,											// IpLinkType
	0x0000,										// TerminalLocationCode
	0x0000,										// TerminalAddr
	1,											// CommunicationMode
	0,											// IpAddrAccessType
	6666,										// TcpPortNumber
	0x0A2F12E4,								// IpAddr
	0xFFFFFF00,								// IpSubNetMask
	0x0A2F1201,								// IpGateWayAddr	
	0,											// Reserved1
	0											// Crc
};
#else
const CommunicationParaStruct DefaultCommuniactionPara = 
{
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterAddr
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterBackAddr1
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterBackAddr2
	0xFF,										// MasterAddrType
	0xFF,										// MasterBackAddr1Type
	0xFF,										// MasterBackAddr2Type
	0,											// CompressType
	{0, 0, 0, 0, 0, 0, 0, 0},					// GateWayAddr
	{											// APN
		'C', 'M', 'N', 'E', 'T', 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0,0
	},
	{											// APNUserName
		'C', 'M', 'N', 'E', 'T', 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APNPassWord
		'C', 'M', 'N', 'E', 'T', 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	0x15,										// HeartbeatTimeGap
	1,										// RedialTimeGap
	3,											// RedialCnt
	0,											// IpLinkType
	0x0000,										// TerminalLocationCode
	0x0000,										// TerminalAddr
	1,											// CommunicationMode
	0,											// IpAddrAccessType
	6666,										// TcpPortNumber
	0x0A2F12E4,								// IpAddr
	0xFFFFFF00,								// IpSubNetMask
	0x0A2F1201,								// IpGateWayAddr	
	0,											// Reserved1
	0											// Crc
};
#endif

const AlarmEventMaskParaStruct DefaultAlarmEventMaskPara = 
{
	{											// AlarmReportMask
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	},
	{											// AlarmJudgeMask
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	},
	{											// EventRecordMask
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	},
	0,											// Reserved1
	0											// Crc
};

const SettleDateParaStruct DefaultSettleDatePara = 
{
	0x01,			// MonthPowerSettleDay
	0x00,			// MonthPowerSettleHour
	0x00,			// DayPowerSettleHour
	0x21,			// MonthVoltageSettleDay
	0x00,			// MonthVoltageSettleHour
	0x00,			// DayVoltageSettleHour
	0				// Crc
};

const IDAuthenticationParaStruct DefaultIDAuthPara = 
{
	{											// AuthReqInfo
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// AuthRespInfo
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// RandomReqInfo
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// RandomRespInfo
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	0,											// VerifyType
	0,											// Reserved
	0											// Crc
};

const ThresholdParaStruct DeaultThresholdPara = 
{
	0x15,					// E0000180
	0x15,					// E0000181
	0x1,					// E0000182
	0x1,					// E0000183
	0x15,					// E0000184
	0x15,					// E0000185
	0x15,					// E0000186
	0x15,					// E0000187
	0x10,					// E0000188 //err20180917 修改时钟异常阈值默认值5改为10
	0x10,					// E0000189
	0x0010,				// E000018A
	0x00002000,		// E000018B
	0x00000003,		// E000018C
	0,					// Reserved
	0					// Crc
};

const	u32	UartBaudRate[MAX_BAUD_NUM] =
{
	BAUD_RATE_300,
	BAUD_RATE_600,
	BAUD_RATE_1200,
	BAUD_RATE_2400,
	BAUD_RATE_4800,
	BAUD_RATE_7200,
	BAUD_RATE_9600,
	BAUD_RATE_19200,
	BAUD_RATE_38400,
	BAUD_RATE_57600,
	BAUD_RATE_115200,
	BAUD_RATE_230400
};

const	u16	UartBPS[MAX_BAUD_NUM] =
{
	BPS_300,
	BPS_600,
	BPS_1200,
	BPS_2400,
	BPS_4800,
	BPS_7200,
	BPS_9600,
	BPS_19200,
	BPS_38400,
	BPS_57600,
	BPS_115200,
	BPS_230400
};


const RS485PortParaStruct DefaultRS485Port1Para = 
{
	UART_FUNC_AMR,			// Function: 抄表通道
	8,			// BaudRate: 2400
	1,			// Parity: 偶校验
	8,			// DataBit: 8
	0,			// StopBit: 1
	5,			// UsartID = UART5
	0			// Crc	
};


#if (((Project/100)==2)||((Project/100)==3) || ((Project/100)==9))	
const RS485PortParaStruct DefaultRS485Port2Para = 
{
	UART_FUNC_AMR,			// Function: 抄表通道
	8,			// BaudRate: 2400
	1,			// Parity: 偶校验
	8,			// DataBit: 8
	0,			// StopBit: 1
	7,			// UsartID = UART7
	0			// Crc	
};
#endif

#if ((Project/100)==5)	
const RS485PortParaStruct DefaultRS485Port2Para = 
{
	UART_FUNC_UPLINK,			// Function: 上行通道
	32,			// BaudRate: 9600
	1,			// Parity: 偶校验
	8,			// DataBit: 8
	0,			// StopBit: 1
	3,			// UsartID = USART3
	0			// Crc	
};
#endif


#if (((Project/100)==2)||((Project/100)==3) || ((Project/100)==9))      //II型集中器没有RS485_3	
const RS485PortParaStruct DefaultRS485Port3Para = 
{
	UART_FUNC_UPLINK,			// Function: 上行通道
	32,			// BaudRate: 9600
	1,			// Parity: 偶校验
	8,			// DataBit: 8
	0,			// StopBit: 1
	3,			// UsartID = USART3
	0			// Crc	
};
#endif


const AttachmentParaStruct DefaultAttachPara = 
{
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},		// TerminalAddr1
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},		// TerminalAddr2
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},		// TerminalAddr3
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},		// TerminalAddr4
	{0xFF, 0xFF, 0xFF, 0xFF},			// MasterTerminalAddr
	{0xFF, 0xFF, 0xFF, 0xFF},			// SlaveTerminalAddr
	0x20,						// MasterTimeOut
	0x20,						// SlaveTimeOut
	0,						// CT
	0,						// PT
	0						// Crc
};

//#if(TEST_ON == 0)
#if ((Project/100)==2)
const VersionParaStruct DefaultVersionPara = 
{
	0x0100,				// ProtocolVersion
	CHINT_MFC_ID,		// ManufactureID
	0x04,				// TerminalType
	0x01,				// Day
	0x03,				// Month
	0x19,				// Year
	0x0103,				// SoftwareVersion
	0x0300				// HardwareVersion
};
#endif

#if ((Project/100)==3)
	#if (DLT500_REPORT == 1)     //配变取DLT500报告时的版本
		const VersionParaStruct DefaultVersionPara = 
		{
			0x0100,				// ProtocolVersion
			CHINT_MFC_ID,		// ManufactureID
			0x03,				// TerminalType
			0x30,				// Day
			0x06,				// Month
			0x18,				// Year
			0x0201,				// SoftwareVersion
			0x0100				// HardwareVersion
		};
	#else
		const VersionParaStruct DefaultVersionPara = 
		{
			0x0100,				// ProtocolVersion
			CHINT_MFC_ID,		// ManufactureID
			0x02,				// TerminalType	//01厂站 02专变 03公变 04集中器
			0x13,				// Day
			0x07,				// Month
			0x19,				// Year
			0x0109,				// SoftwareVersion
			0x0400				// HardwareVersion
		};
	#endif
#endif

#if ((Project/100)==5)
const VersionParaStruct DefaultVersionPara = 
{
	0x0100,				// ProtocolVersion
	CHINT_MFC_ID,		// ManufactureID
	0x04,				// TerminalType
	0x18,				// Day
	0x02,				// Month
	0x19,				// Year
	0x0103,				// SoftwareVersion
	0x0300				// HardwareVersion
};
#endif

#if ((Project/100)==9)
const VersionParaStruct DefaultVersionPara = 
{
	0x0100,				// ProtocolVersion
	CHINT_MFC_ID,		// ManufactureID
	0x04,				// TerminalType
	0x28,				// Day
	0x09,				// Month
	0x18,				// Year
	0x0100,				// SoftwareVersion
	0x0200				// HardwareVersion
};
#endif

//#else //送检版本号
/*
#if ((Project/100)==2)
const VersionParaStruct DefaultVersionPara = 
{
	0x0100,				// ProtocolVersion
	CHINT_MFC_ID,		// ManufactureID
	0x04,				// TerminalType
	0x13,				// Day
	0x05,				// Month
	0x18,				// Year
	0x0201,				// SoftwareVersion
	0x0200				// HardwareVersion
};
#endif


#if ((Project/100)==3)
const VersionParaStruct DefaultVersionPara = 
{
	0x0100,				// ProtocolVersion
	CHINT_MFC_ID,		// ManufactureID
	0x02,				// TerminalType
	0x10,				// Day
	0x05,				// Month
	0x18,				// Year
	0x0201,				// SoftwareVersion
	0x0200				// HardwareVersion
};
#endif

#if ((Project/100)==5)
const VersionParaStruct DefaultVersionPara = 
{
	0x0100,				// ProtocolVersion
	CHINT_MFC_ID,		// ManufactureID
	0x04,				// TerminalType
	0x04,				// Day
	0x04,				// Month
	0x18,				// Year
	0x0102,				// SoftwareVersion
	0x0200				// HardwareVersion
};
#endif
//#endif
*/

const LoadManagementParaStruct DefaultLoadManagementPara = 
{
	{											// SwitchNodeType
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// InputSwitchState
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// OutputSwitchState
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// OutputSwitchRound
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	0x0000,										// SwitchTime
	0,											// PowerHoldState
	0, 											// PowerCtrlFlag
	0, 											// MonthKwhCtrlFlag
	0,											// PurchaseKwhCtrlFlag
	0,											// PowerCtrlRoundFlag
	0,											// MonthKwhCtrlRoundFlag
	0,											// PurchaseKwhCtrlRoundFlag
	0x10, 										// AlarmDelayTime
	0,											// PowerCtrlIncFlag
	0,											// KwhCtrlIncFlag
	0x001000,									// PowerOverLoadTime
	0x001000,									// PowerResetTime
	{0, 0, 0, 0, 0, 0, 0, 0},					// PowerCtrlPoint
	{0, 0, 0, 0, 0, 0, 0, 0},					// KwhCtrlPoint
	0,											// E0000C6B
	0,											// E0000C6C
	0,											// E0000C71
	0,											// Reserved1
	0,											// E0000C6D
	0,											// E0000C70
	{											// KwCtrlStruct
		{
			{
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// KwOptionStruct
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// StartHour,StartMinute,
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// EffectiveFlag,Reserved
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}		// PowerLoad
			},
			0,										// PowerCtrlPeriodCnt
			0x01,									// StartMonth
			0x01,									// StartDay
			0x12,									// EndMonth
			0x31,									// EndDay
			5,										// TI: 频率单位 04：日, 05：月, 06：周
			0x00,									// N0
			0x00,									// N1
			0x00,									// N2
			0x00,									// N3
			0,										// Reserved1
			0										// Reserved2
	 	},
	 	{											// KwCtrlStruct
			{
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// KwOptionStruct
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// StartHour,StartMinute,
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// EffectiveFlag,Reserved
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}		// PowerLoad
			},
			0,										// PowerCtrlPeriodCnt
			0x01,									// StartMonth
			0x01,									// StartDay
			0x12,									// EndMonth
			0x31,									// EndDay
			5,										// TI: 频率单位 04：日, 05：月, 06：周
			0x00,									// N0
			0x00,									// N1
			0x00,									// N2
			0x00,									// N3
			0,										// Reserved1
			0										// Reserved2
		},
		{											// KwCtrlStruct
			{
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// KwOptionStruct
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// StartHour,StartMinute,
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// EffectiveFlag,Reserved
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}		// PowerLoad
			},
			0,										// PowerCtrlPeriodCnt
			0x01,									// StartMonth
			0x01,									// StartDay
			0x12,									// EndMonth
			0x31,									// EndDay
			5,										// TI: 频率单位 04：日, 05：月, 06：周
			0x00,									// N0
			0x00,									// N1
			0x00,									// N2
			0x00,									// N3
			0,										// Reserved1
			0										// Reserved2
		},
		{											// KwCtrlStruct
			{
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// KwOptionStruct
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// StartHour,StartMinute,
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},		// EffectiveFlag,Reserved
				{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}		// PowerLoad
			},
			0,										// PowerCtrlPeriodCnt
			0x01,									// StartMonth
			0x01,									// StartDay
			0x12,									// EndMonth
			0x31,									// EndDay
			5,										// TI: 频率单位 04：日, 05：月, 06：周
			0x00,									// N0
			0x00,									// N1
			0x00,									// N2
			0x00,									// N3
			0,										// Reserved1
			0										// Reserved2
		}
	},
		
	0,											// E0000E00
	0,											// E0000E01
	{0, 0, 0, 0, 0, 0, 0, 0},					// E0000E02
	{0, 0, 0, 0, 0, 0, 0, 0},					// E0000E03
	{											// E0000E04
		0, 0, 0, 0, 0, 0, 0, 0,	
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},		// E0000E11
	0,											// E0000E05
	0,											// E0000E10
	0											// Crc
};


const MeterPointCtrlParaStruct DefaultMeterPointCtrlPara = 
{
	{
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		0,
		0
	},
	{
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		0,
		0
	},
	{
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		0,
		0
	},
	{
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		0,
		0
	},
	0,
	0
};

const SelfAdaptionParaStruct DefaultSelfAdaptionPara = 
{
	0, 		//SelfAdaptionFlag  自适应通信参数 0:非自适应 1:自适应
	0,		//Reserved1
	0,		//Reserved2		
	0		//Reserved3
};

//err20190528 移动自适应初始参数
const E000011x_CommunicationParaStruct Default_E0000111_CommunicationParaStruct = 
{
	{0x21,0x1C,0x74,0xA2,0x14,0x64},			// MasterAddr //普通卡
	//{0xda,0x07,178,151,247,183},
	{0x73,0x17,0xC6,0x1B,0x6F,0x0A},			// MasterBackAddr1	//物联卡
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterBackAddr2
	{0, 0, 0, 0, 0, 0, 0, 0},					// GateWayAddr
	
	0x02,										// MasterAddrType
	0x02,										// MasterBackAddr1Type
	0x02,										// MasterBackAddr2Type
	0x02,										// MasterBackAddr3Type

	{											// APN1
		'Y','N','D','W','F','K','.','Y','N',
		0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN1UserName
		0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN1PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2
		'C','M','I','O','T','Y','N','D','W','F','K','.','Y','N',0 ,0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	}
};

//err20190528 联通自适应初始参数
const E000011x_CommunicationParaStruct Default_E0000112_CommunicationParaStruct = 
{
	{0x21,0x1C,0x2E,0x01,0x64,0xAC},			// MasterAddr //普通卡
	//{0xda,0x07,178,151,247,183},			////测试用183.247.151.178 2010
	{0x79,0x17,0xC6,0x1B,0x6F,0x0A},			// MasterBackAddr1	//物联卡
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterBackAddr2
	{0, 0, 0, 0, 0, 0, 0, 0},					// GateWayAddr
	
	0x02,										// MasterAddrType
	0x02,										// MasterBackAddr1Type
	0x02,										// MasterBackAddr2Type
	0x02,										// MasterBackAddr3Type

	{											// APN1
		'Y','N','D','W','.','F','K','.','Y','N','A','P','N', 0, 0, 0,
		//'c','m','n','e','t',0,0,0,0,0,0,0,0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN1UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN1PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','m','n','e','t',0,0,0,0,0,0,0,0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	}
};

//err20190528 电信自适应初始参数
const E000011x_CommunicationParaStruct Default_E0000113_CommunicationParaStruct = 
{
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterAddr //普通卡
	//{0xda,0x07,178,151,247,183},				////测试用183.247.151.178 2010
	{0x71,0x17,0xC6,0x1B,0x6F,0x0A},			// MasterBackAddr1	//物联卡
	
	{0, 0, 0, 0, 0, 0, 0, 0},					// MasterBackAddr2
	{0, 0, 0, 0, 0, 0, 0, 0},					// GateWayAddr
	
	0x02,										// MasterAddrType
	0x02,										// MasterBackAddr1Type
	0x02,										// MasterBackAddr2Type
	0x02,										// MasterBackAddr3Type
	{											// APN1
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','m','n','e','t',0,0,0,0,0,0,0,0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN1UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN1PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
	 	0, 0, 0, 0, 0, 0, 0, 0,
	 	0, 0, 0, 0, 0, 0, 0, 0, 
	 	0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2
		'y','n','d','w','s','c','.','v','p','d','n','.','y','n',0,0,
		//'c','m','n','e','t',0,0,0,0,0,0,0,0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN2PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		//'c','a','r','d',0,0,0,0,//测试用
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN3PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4UserName
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{											// APN4PassWord
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	}
};




const u8 	GeneralAddr[METER_ADDR_SIZE] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
const u8 	YaoXinAddr[METER_ADDR_SIZE] = {0xBB,0xBB,0xBB,0xBB,0xBB,0xBB};

const u8 	BroadCastAddr[METER_ADDR_SIZE] = {0x99,0x99,0x99,0x99,0x99,0x99};

const u8 	EmptyAddr[METER_ADDR_SIZE] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

const u8 	ZeroAddr[METER_ADDR_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00};

const u8	DLT645CtrlCode97[DLT_645_97_CODE_NUM] = 
{ 
	DLT_97_READ,				//读数据
	DLT_97_READ_LAST,			//读后续数据
	DLT_97_WRITE,				//写数据
	DLT_97_TIME_SYNC,			//广播校时
	DLT_97_WRITE_ADDR,			//写设备地址
	DLT_97_BAUD_RATE,			//更改通信速率
	DLT_97_PASS_WORD,			//修改密码
	DLT_97_MAX_NEED_CLEAR		//最大需量清零
};

const u8    DLT645CtrlCode07[DLT_645_07_CODE_NUM] = 
{
	DLT_07_TIME_SYNC,			//广播校时
	DLT_07_READ,				//读数据
	DLT_07_READ_ADDR,			//读通信地址
	DLT_07_WRITE,				//写数据
	DLT_07_WRITE_ADDR,			//写通信地址
	DLT_07_PASS_WORD,			//修改密码
//	DLT_07_CLEAR_EVENT,			//事件清零
	DLT_07_DIN_XIN_READ,		//顶信载波模块读采集器档案信息
	DLT_07_DONG_RUAN_READ		//东软读采集器地址控制码
};

const u8 HexToChar[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

