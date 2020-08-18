
/*******************************************************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_UpLinkProtocol.h
作  者:zhaoyun
版  本:1.00
日  期:20170501
描  述:南网上行通信规约
*******************************************************************************************/
#ifndef __QCSG_UP_LINK_PROTOCOL__
#define __QCSG_UP_LINK_PROTOCOL__

#define TEST_ON			(1)
#define YBWHZ_TEST      (0)//测试时使用
typedef enum 
{
	NO_ERROR = 0, 	
	ERROR_RealyCmdNoAck,
	ERROR_InvalidData,
	ERROR_Permission_denied,
	ERROR_NoSuchDataID,
	ERROR_CmdTimeInvalid,
	ERROR_DestAddrNotExist,
	ERROR_VerifyFail,
	ERROR_UNKNOW_DEVICE,
	ERROR_WRONG_VERSION,
	ERROR_WRONG_TIME,
	ERROR_WRONG_PROTOCOL,
	ERROR_WRONG_LENGTH,
	ERROR_WRONG_ACCESS,
	ERROR_TIME_OUT,
	ERROR_TIME_IS_BIGGER,
	ERROR_TIME_IS_EQUAL,
	ERROR_TIME_IS_SMALLER,
	ERROR_WRONG_ADDR,
	ERROR_WRONG_CRC,
	ERROR_DO_NOT_ANSWER,
	ERROR_TASK_BUSY,
	ERROR_TASK_PAUSE,
	ERROR_POINTER_NULL,
	ERROR_BUF_EMPTY,
	ERROR_BUF_FULL,
	ERROR_NO_AMR_TASK,
	ERROR_NO_AMS_TASK,
	ERROR_FILE_USEDUP,
	ERROR_UNKNOW
}ResultEnum;

#define SIZE_2						(2)
#define SIZE_3						(3)
#define SIZE_4						(4)
#define SIZE_6						(6)
#define SIZE_8						(8)
#define SIZE_12						(12)
#define SIZE_16						(16)
#define SIZE_20						(20)
#define SIZE_24						(24)
#define SIZE_32						(32)
#define SIZE_48						(48)
#define SIZE_64						(64)
#define SIZE_128					(128)
#define SIZE_256					(256)
#define SIZE_512					(512)
#define SIZE_1K						(1024)
#define SIZE_2K						(2048)
#define SIZE_8K						(8192)
#define CHINT_MFC_ID				(0x3530)	
#define METER_ADDR_SIZE				SIZE_6

#define FRAME_TYPE_NONE						(0x00)
#define FRAME_TYPE_QCSG_UP					(0x01)
#define FRAME_TYPE_QCSG_LOCAL				(0x02)
#define FRAME_TYPE_DLT_645					(0x03)
#define FRAME_TYPE_YAOXIN					(0x04)
#define FRAME_TYPE_UNKNOWN					(0x05)
#define FRAME_TYPE_MIXED_645                (0x06)
#define FRAME_TYPE_EDMI                		(0x07)

/*******************************************************************************************
// 以下部分为南网上行协议的帧格式解析与处理相关的宏定义
//
//
*******************************************************************************************/
/**************************** Start of QCSG Frame Define **********************************/
typedef struct
{
	u8 u8Second;
	u8 u8Minute;
	u8 u8Hour;
	u8 u8Day;
	u8 u8Month;
	u8 u8Year;
	u8 u8Week;
	u8 u8Sync;
}RealTimeStruct;

typedef struct
{
	u8 YearH;
	u8 YearL;
	u8 Month;
	u8 Day;
	u8 Hour;
	u8 Minute;
	u8 Report;		//是否有上送数据(0:无	非0:有)
	u8 Ack;			//是否收到主站的确认帧(1:是 0:否)
}RealTimeYYStruct;

#define TERMINAL_POWER_UP_FLAG					(0x0001)
#define TERMINAL_POWER_DOWN_DONE_FLAG			(0x0002)
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)       //II型集中器不判编程键20180515ybzyl
	#define TERMINAL_PROKEY_PRESS_FLAG				(0x0004)
#endif
#define TERMINAL_KG_KEY_OPEN_FLAG				(0x0008)

#define TERMINAL_ADDR_CHANGE_FLAG				(0x0010)
#define TERMINAL_COMM_PARA_CHANGE_FLAG			(0x0020)
#define TERMINAL_READ_METER_ON_FLAG             (0x0040)
#define TERMINAL_GO_TO_TEST_FLAG                (0x0080)


#define TERMINAL_EXE_FILE_READY		(0x55)

#define PRO_KEY_HOLD_SECOND			(30)
typedef struct
{
	u8 CheckSecond;
	u8 CheckMinute;
	u8 CheckHour;
	u8 CheckDay;
	u8 CheckMonth;
	u8 CheckYear;
	u8 CurExeFileReady;
	u8 AlarmSecond;
	u16 LatelyAlarmNum;
	u16 Flag;
	u32 LastPORTn;
	u32 SdramTestTime;
	u32 BuzzMode;
	u32 BuzzOnTime;
	u32 BuzzOnTick;
	u32 CurExeFileType;
	u32 CurExeFilePort;
	u32 CurExeFileCnt;
	u32 GPRSRxTxBytes;
	u32 DayPowerDownCnt;
	u32 DayPowerDownTime;
	u32 MonthPowerDownCnt;
	u32 MonthPowerDownTime;

	u32 TerminalAddr;
	u32 TerminalLocation;
	u32 LastAlarmDI;

	u8 ProKeyPressTick;
	u8 ProKeyLastSeconds;
	u8 KGKeyPressTick;
	u8 KGkeyReleaseTick;
	u32 CurExeFileUpdateDelay;
}TerminalRunTimeCtrlStruct;

typedef struct
{
	u32 MaxMeters;
	u16 ComPortAndPulseNum;
	
	
}TerminalSpecDataStruct;



#define QCSG_AFN_00					(0x00)
#define QCSG_AFN_02					(0x02)
#define QCSG_AFN_04					(0x04)
#define QCSG_AFN_06					(0x06)
#define QCSG_AFN_0A					(0x0A)
#define QCSG_AFN_0C					(0x0C)
#define QCSG_AFN_0D					(0x0D)
#define QCSG_AFN_0E					(0x0E)
#define QCSG_AFN_0F					(0x0F)
#define QCSG_AFN_10					(0x10)
#define QCSG_AFN_12					(0x12)
#define QCSG_AFN_13					(0x13)
#define QCSG_AFN_15					(0x15)


#define QCSG_FRAME_HEAD				(0x68)
#define QCSG_FRAME_TAIL				(0x16)
#define QCSG_INVALID_PN				(0xFFFE)
#define QCSG_ALL_PN					(0xFFFF)

#define QCSG_MAX_FRAME_LEN			SIZE_2K
#define QCSG_FIXED_LEN				(18)
#define QCSG_DI_SIZE				SIZE_4

#define QCSG_FC_DIR_BIT				(0x80)
#define QCSG_FC_PRM_BIT				(0x40)
#define QCSG_FC_FCB_BIT				(0x20)
#define QCSG_FC_ACD_BIT				(0x20)
#define QCSG_FC_FCV_BIT				(0x10)
#define QCSG_FC_FMASK_BIT			(0x0F)

#define QCSG_FC_PRM1_F1				(0x01)
#define QCSG_FC_PRM1_F4				(0x04)
#define QCSG_FC_PRM1_F9				(0x09)
#define QCSG_FC_PRM1_F10			(0x0A)
#define QCSG_FC_PRM1_F11			(0x0B)

#define QCSG_FC_PRM0_F0				(0x00)
#define QCSG_FC_PRM0_F8				(0x08)
#define QCSG_FC_PRM0_F9				(0x09)
#define QCSG_FC_PRM0_F11			(0x0B)

#define QCSG_SEQ_TPV_BIT			(0x80)
#define QCSG_SEQ_FIR_BIT			(0x40)
#define QCSG_SEQ_FIN_BIT			(0x20)
#define QCSG_SEQ_CON_BIT			(0x10)
#define QCSG_SEQ_MASK_BIT			(0x0F)

#define QCSG_TPV_LEN				(5)
#define QCSG_PW_LEN					(16)

#define PN_CHECK_DISABLE			(0x00)
#define PN_CHECK_ENABLE				(0x01)

#define QCSG_DATA_DENSITY_DEFAULT	(0x00)
#define QCSG_DATA_DENSITY_1_MINUTE	(0x01)
#define QCSG_DATA_DENSITY_5_MINUTE	(0x02)
#define QCSG_DATA_DENSITY_15_MINUTE	(0x03)
#define QCSG_DATA_DENSITY_30_MINUTE	(0x04)
#define QCSG_DATA_DENSITY_1_HOUR	(0x05)
#define QCSG_DATA_DENSITY_1_DAY		(0x06)
#define QCSG_DATA_DENSITY_1_MONTH	(0x07)

#define QCSG_SAMPLE_UNIT_MINUTE		(0x00)
#define QCSG_SAMPLE_UNIT_HOUR		(0x01)
#define QCSG_SAMPLE_UNIT_DAY		(0x02)
#define QCSG_SAMPLE_UNIT_MONTH		(0x03)

typedef struct
{
	u16 DataLen;		// Data缓冲区数据长度，注意根据帧结构组建上行帧时，DataLen不含 FC, A1, A2, A3, AFN, SEQ 的长度
	u16 FC;
	u32 A1;
	u32 A2;
	u16 A3;
	u8 AFN;
	u8 SEQ;
	u8 Data[QCSG_MAX_FRAME_LEN];	
}QCSGFrameStruct;

typedef struct
{	
	u32 A1;
	u32 A2;
	u16 A3;
	u16 FC;	
	u8 AFN;
	u8 SEQ;	
	u8 Port;
	u8 AmrEnum;
}QCSGFrameInfoStruct;

typedef struct
{
	u16 PORTn;
	u16 Pn;							// 当前处理的 Pn 值
	u16 SrcDataPos;					// 数据源待处理数据的位置
	u16 SrcDataPosCopy;
	u16 DataIDIndex;				// 数据标识在表格中的索引号
	u16 DstDataPos;					// 应答帧当前数据标识下数据存放位置
	u16 UserDataLen;				// 源数据总长度
	u16 TempPn;						// 由总的 Pn 拆解得到的对应单个测量点的 Pn 值
	u32 CurDataID;					// 当前处理的数据标识
	u8 DataDensity;
	u8 AllPn;
	u8 Addr[METER_ADDR_SIZE];
	RealTimeYYStruct StartTime;
	RealTimeYYStruct EndTime;
}QCSGFrameProcessStruct;

typedef enum 
{
	TASK_ID_NONE = 0, 
	TASK_ID_RealTimeAMR,
	TASK_ID_TerminalCtrl,
	TASK_ID_RS232_UART,
	TASK_ID_IR_UART,
	TASK_ID_RS485_1_UART,
	TASK_ID_RS485_2_UART,
	TASK_ID_RS485_3_UART,
	TASK_ID_GPRS_UART,
	TASK_ID_PLC_YX_UART,
	TASK_ID_ETHE_UART,
	TASK_ID_GPRS_Report,
	TASK_ID_ETHE_Report,
	TASK_ID_ETH_LINK_MAINTAIN,
	TASK_ID_GPRS_LINK_MAINTAIN,
	TASK_ID_RS232_MULTI_FRAME_SEND,
	TASK_ID_IR_MULTI_FRAME_SEND,
	TASK_ID_RS485_CH1_MULTI_FRAME_SEND,
	TASK_ID_RS485_CH2_MULTI_FRAME_SEND,
	TASK_ID_RS485_CH3_MULTI_FRAME_SEND,
	TASK_ID_GPRS_MULTI_FRAME_SEND,
	TASK_ID_ETHE_MULTI_FRAME_SEND,
	TASK_ID_PLC_INIT,
	TASK_ID_PLC_FILE_SYNC,
	TASK_ID_PLC_ADD_NODE,
	TASK_ID_RS485_1_AMR,
	TASK_ID_RS485_2_AMR,
	TASK_ID_RS485_3_AMR,
	TASK_ID_PLC_AMR,
	//mxn
	TASK_ID_RD_TDD_SEND,
	TASK_ID_RD_TDC_SEND,
	TASK_ID_RD_EVENT_SEND,
	TASK_ID_RD_RELAY_TASK_SEND,
	TASK_ID_RD_COMMON_TASK_SEND,
	TASK_ID_RT_SEND,
	//lxw
	TASK_ID_RS485_1_AMS,
	TASK_ID_RS485_2_AMS,
	TASK_ID_RS485_3_AMS,
	TASK_ID_PLC_AMS

}TaskIDEnum;

typedef enum 
{
	TASK_STATE_IDLE = 0, 
	TASK_STATE_SEND,
	TASK_STATE_WAIT_ACK,
	TASK_STATE_DONE
}TaskStateEnum;



typedef enum 
{
	UART_STATE_IDLE = 0, 
	UART_STATE_SEND_TASK_FRAME,
	UART_STATE_SEND_LOCAL_RESPONSE,
	UART_STATE_WAIT_ACK	
}UartStateEnum;

#define UART_FLAG_RX_DONE				(0x0001)
#define UART_FLAG_TX_ACK				(0x0002)
#define UART_FLAG_TX_TASK_FRAME			(0x0004)
#define UART_FLAG_TX_DONE				(0x0008)
#define UART_FLAG_TX_TIME_OUT			(0x0010)
#define UART_FLAG_WAIT_ACK_TIME_OUT		(0x0020)
#define UART_FLAG_RX_BUF_READ_DONE		(0x0040)
#define UART_FLAG_LOG_OUT				(0x0080)

#define UART_TX_DELAY_GAP				(2)				// 串口发送的帧间间隔时间，10ms 单位
#define UART_DEFAULT_ACK_TIME			(3)				// 默认的串口等待应答超时时间， S 单位
#define UART_TASK_NUM					(9)
typedef struct
{
	TaskIDEnum SrcTaskID;		// 触发串口任务的源任务号
	UartStateEnum State;		// 串口任务状态
	u8 NeedAck;					// 通过串口发送的帧是否要求应答, 0 不需要, 1 需要应答
	u8 FrameType;				// 当前帧的协议类型
	u8 RetryCnt;				// 重试次数，递减计数，为0时停止重发。
	u8 Reserved;
	u16 BpsCtrl;				// 请求发送时使用的波特率控制
	u32 Reserved2;
	u16 AckTiemOutSetByTask;	// 任务发起端设置的超时应答时间
	u16 WaitAckTimeOut;			// 当前端口等待应答超时时间
	u32 PriorityLock;			// 
	u16 Reserved1;
	u16 Flag;					// 触发标志位
	u16 AckLen;					// 响应帧总长度
	u16 FrameLen;				// 数据帧总长度
	u8* FrameBuf;				// 数据帧指针
}UartTaskCtrlStruct;

#define BUFFER_EMPTY		(0x00)
#define BUFFER_FULL			(0x01)

#define DATA_TYPE_STRUCT		(0x00)	//实时抄表	
#define DATA_TYPE_RELAY			(0x01)	//中继
#define DATA_TYPE_FRAME			(0x02)	//控制指令
#define DATA_TYPE_BCAST_CMD		(0x03)	//广播
#define PN_LIST_MAX_NUM			SIZE_1K
#if ((Project/100)==9)
#define PN_LIST_MAX_NUM_JC		SIZE_8 //配变交采最多支持一次抄读64个数据项
#endif
#define TASK_FRAME_INDICATION	(0x0001)
#define UART_FRAME_INDICATION	(0x0002)
typedef struct
{
	u8 SrcPort;					// 源端口
	u8 Empty;					// 源端口的缓冲区是否为空, 0表示空，可以缓存数据，否则对应缓冲存在数据
	u8 FrameType;				// 当前帧的协议类型
	u8 DataType;				// 当前缓冲指针对应的数据组织方式，0: 整帧数据，存放在帧缓冲 1: 解析后的数据，存放在数据缓冲
	QCSGFrameInfoStruct QCSGFrameInfo;
	u16 Reserved5;
	u16 PnListNum;				// 当上行协议需要拆解成 PnDIList列表时，表示列表数量
	u16 Flag;					// 触发标志位
	u16 FrameLen;				// 数据帧总长度
	u8* FrameBuf;				// 数据帧指针
}UartRxChCtrlStruct;


typedef struct
{
	TaskIDEnum SrcTask;
	u8 NeedAck;
	u8 FrameType;
	u8 RetryCnt;
	u16 BpsCtrl;
	u16 TimeOut;
	u32 DataLen;				// 数据帧总长度
	u8* DataBuf;				// 数据帧指针
}UartTxReqStruct;

#define MF_SEND_START				(0x0001)
#define MF_SEND_SINGLE_OK			(0x0002)
#define MF_SEND_SINGLE_TIMEOUT		(0x0004)
typedef struct					// 该数据结构主要用于，上行协议通信中需要进行分帧发送的任务
{
	TaskStateEnum State;		// 多帧发送任务状态
	TaskIDEnum SrcTask;
	u8 RetryCnt;				// 重试次数，递减计数，为0时停止重发。
	u8 Empty;					// 目标端口的缓冲区是否为空
	u8 FrameType;				// 当前帧的协议类型
	u8 DataType;				// 当前缓冲指针对应的数据组织方式，0: 整帧数据， 1: 解析后的数据
	u8 AckToSrcTask;			// 多帧任务发送完成后是否需要向发起多帧发送的任务进行确认
	u8 NeedAck;					// 多帧发送过程中，每一帧是否需要目标端口进行应答确认
	u16 PnListNum;
	u16 Round;
	
	QCSGFrameInfoStruct QCSGFrameInfo;
	u16 WaitAckTimeOut;			// 当前端口等待应答超时时间
	u16 Flag;					// 触发标志位
	u32 CurTimePos;				// 主要用户普通任务分帧，表示当前数据时间的位置
	u32 CurTimeDataLen;			// 主要用户普通任务分帧，表示当前数据时间对应的数据长度
	u32 CurTimeDataEndPos;		// 主要用户普通任务分帧，表示当前数据时间对应数据的结束位置
	u32 LastDataPos;			// 上次组建数据帧时的起始位置
	u32 DataPos;				// 数据发送的位置
	u32 DataLen;				// 数据帧总长度
	u8* DataBuf;				// 数据帧指针
}MultiFrameSendTaskStruct;


typedef struct					// 该数据结构主要用于上行协议通信中需要进行分帧发送的任务请求
{
	TaskIDEnum SrcTask;
	u8 RetryCnt;				// 重试次数，递减计数，为0时停止重发。
	u8 FrameType;				// 当前帧的协议类型
	u8 DataType;				// 当前缓冲指针对应的数据组织方式，0: 整帧数据， 1: 解析后的数据
	u8 AckToSrcTask;			// 多帧任务发送完成后是否需要向发起多帧发送的任务进行确认
	u8 NeedAck;					// 多帧发送过程中，每一帧是否需要目标端口进行应答确认
	u16 Res;
	
	QCSGFrameInfoStruct QCSGFrameInfo;
	u16 WaitAckTimeOut;			// 当前端口等待应答超时时间
	u16 PnListNum;				// 数据发送的位置
	u32 DataLen;				// 数据帧总长度
	u8* DataBuf;				// 数据帧指针
}MultiFrameSendReqStruct;


#define TASK_LOCK						(0x55)
#define TASK_UNLOCK						(0x00)
typedef struct
{
	u16 Pn;
	u16 Len;
	u32 DI;	
}QCSG_PnListStruct;

/****************************** End of QCSG Frame Define **********************************/
#define QCSG_FOR_ALL_DI				(0xE0000000)
#define QCSG_PARA_MIN_DATA_ID		(0xE0000100)
#define QCSG_PARA_MAX_DATA_ID		(0xE080020F)

#define QCSG_MIN_COMM_PARA_ID		(0xE0000100)
#define QCSG_MAX_COMM_PARA_ID		(0xE000012F)

#define QCSG_CONFIRM				(0x00)
#define QCSG_DENY					(0x01)


#define COMM_AADR_SIZE				SIZE_8
#define APN_NAME_MAX_SIZE			SIZE_16	
#define APN_USER_NAME_MAX_SIZE		SIZE_32
#define APN_USER_PWD_MAX_SIZE		SIZE_32

typedef enum
{
	PARA_Communication = 0x01,
	PARA_RealTime,
	PARA_Reset,
	PARA_AlarmEventMask,
	PARA_SettlementDate,
	PARA_IDAuthentication,
	PARA_Threshold,
	PARA_AMR,
	PARA_Task,
	PARA_Caculation,
	PARA_RS485Port,
	PARA_Attachment,
	PARA_Version,
	PARA_LoadManagement,
	PARA_LinkInterface,
	PARA_MeterPointControl,
	PARA_PowerQuality,
	PARA_UserDefine	
}PARA_ENUM;

//---------------- 通信参数
#define MASTER_CH_TYPE_GPRS		(0x02)
#define MASTER_CH_TYPE_PSTN		(0x03)
#define MASTER_CH_TYPE_ETHE		(0x04)
#define MASTER_CH_TYPE_RS232	(0x06)
#define MASTER_CH_TYPE_CSD		(0x07)
#define MASTER_CH_TYPE_RADIO	(0x08)

#define MASTER_ADDR_NUM			(3)
#define MIN_TCP_PORT_ID			(1025)

#define DEFAULT_IP_DNS			(0xC0A80101)		// 1.1.168.192
typedef struct
{
	u8 MasterAddr[COMM_AADR_SIZE];
	u8 MasterBackAddr1[COMM_AADR_SIZE];
	u8 MasterBackAddr2[COMM_AADR_SIZE];
	
	u8 MasterAddrType;		//通信通道类型
	u8 MasterBackAddr1Type;
	u8 MasterBackAddr2Type;
	u8 CompressType;

	u8 GateWayAddr[COMM_AADR_SIZE];
	u8 APN[APN_NAME_MAX_SIZE];
	u8 APNUserName[APN_USER_NAME_MAX_SIZE];
	u8 APNPassWord[APN_USER_PWD_MAX_SIZE];

	u8 HeartBeatTimeGap;
	u8 RedialTimeGap;	//掉线重拨间隔，缺省 30 分钟
	u8 RedialCnt;
	u8 IpLinkType;
	
	u32 TerminalLocationCode;
	u32 TerminalAddr;
	
	u8 CommunicationMode;
	u8 IpAddrAccessType;	//有线网络 IP 地址获取方式0：手动设置 IP 地址    1：自动设置 IP 地址
	u16 TcpPortNumber;

	u32 IpAddr;
	u32 IpSubNetMask;
	u32 IpGateWayAddr;

	u16 Reserved1;
	u16 Crc;
}CommunicationParaStruct;

typedef struct
{
	u8 MasterAddr[COMM_AADR_SIZE];
	
	u8 MasterAddrType;		//通信通道类型

	u8 APN[APN_NAME_MAX_SIZE];
	u8 APNUserName[APN_USER_NAME_MAX_SIZE];
	u8 APNPassWord[APN_USER_PWD_MAX_SIZE];
}SelfAdaptionCommunicationParaStruct;

//---------------- 告警事件屏蔽参数
typedef struct
{
	u8 AlarmReportMask[SIZE_32];
	u8 AlarmJudgeMask[SIZE_32];
	u8 EventRecordMask[SIZE_32];
	
	u16 Reserved1;//u16 AlarmDayReportFlag;  //err20180710 每日上报一次的告警标识，需掉电存储 //err20190216 改变 存储位置
	
	//err20190109 时钟电池电压低、电能表时钟异常告警一天上报一次需要掉电存储，暂时将Flag数据存放在此处。该结构体增加256字节
	//u8 ClkBatLowVolAlarmFLagList[SIZE_128];	//	1024/8=128 每个位代表对应测量点号的告警标志 1表示产生 0表示恢复//err20190216 改变 存储位置
	//u8 TimeFaultAlarmFLagList[SIZE_128];	//	1024/8=128 每个位代表对应测量点号的告警标志 1表示产生 0表示恢复//err20190216 改变 存储位置

	u16 Crc;
}AlarmEventMaskParaStruct;


#define HOUR_BCD_MIN		(0x00)
#define HOUR_BCD_MAX		(0x23)
#define DAY_BCD_MIN			(0x01)
#define DAY_BCD_MAX			(0x31)
//---------------- 结算日参数
typedef struct
{
	u8 MonthPowerSettleDay;
	u8 MonthPowerSettleHour;
	u8 DayPowerSettleHour;
	u8 MonthVoltageSettleDay;
	u8 MonthVoltageSettleHour;
	u8 DayVoltageSettleHour;
	u16 Crc;
}SettleDateParaStruct;

//---------------- 身份认证参数
typedef struct
{
	u8 AuthReqInfo[SIZE_16];
	u8 AuthRespInfo[SIZE_16];
	u8 RandomReqInfo[SIZE_16];
	u8 RandomRespInfo[SIZE_16];
	
	u8 VerifyType;
	u8 Reserved1;
	u16 Crc;
}IDAuthenticationParaStruct;

//---------------- 阈值参数
typedef struct
{
	u8 E0000180;		//各种发生持续时间
	u8 E0000181;		//各种恢复持续时间
	u8 E0000182;		//判断断相、失压、失流发生持续时间，缺省值1 分钟
	u8 E0000183;		//判断断相、失压、失流恢复持续时间，缺省值1 分钟
	
	u8 E0000184;		//判断电压逆相序时间，缺省值15 分钟
	u8 E0000185;		//判断电压逆相序恢复时间，缺省值15 分钟
	u8 E0000186;		//判断电流反极性时间，缺省值15 分钟
	u8 E0000187;		//	判断电流反极性恢复时间，缺省值15 分钟
	
	u8 E0000188;		//	电能表时钟异常阈值，缺省值10 分钟
	u8 E0000189;		//	电能表飞走阈值，缺省为10 倍
	u16 E000018A;		//	电能表停走阈值：
	
	u32 E000018B;		//剩余金额不足阈值：缺省为20.00 元
	u32 E000018C;		//月通信流量越限值

	u16 Reserved1;
	u16 Crc;
}ThresholdParaStruct;

//---------------- 抄表参数

#define PERIOD_UNIT_MINUTE		(0x00)
#define PERIOD_UNIT_HOUR		(0x01)
#define PERIOD_UNIT_DAY			(0x02)
#define PERIOD_UNIT_MONTH		(0x03)

typedef struct
{
	u8 Minute;
	u8 Hour;
	u8 Day;
	u8 Month;
	u8 Year;			//基准时间
	u8 PeriodUnit;		//周期单位
	u8 Period;			//周期值
	u8 Reserved1;

	u16 Reserved2;
	u16 Crc;
}AMRChTimeStruct;		//抄表参数(周期)


//抄表通道配置
#define AMR_MAX_DA					SIZE_256
#define AMR_MAX_DATA_ID				SIZE_32

typedef struct
{
	u8 AMRChSetNum;
	u8 Flag;				//有效性标志
	u16 StoreNum;		//存储点数
	
	u8 ChMeterType;		//电表类型
	u8 UserType;			//重点用户属性
	u8 DANum;			//信息点标识组数
	u8 DataIDNum;		//数据标识编码组数
	
	u16 DA[AMR_MAX_DA];	
	u32 DataID[AMR_MAX_DATA_ID];

	u16 Reserved1;
	u16 Crc;
}AMRChSetStruct;		//抄表参数(有效性)

//---------------- 普通任务参数
#define MAX_NORMAL_TASK_NUM			(32)
#define DATA_STRUCT_TYPE_TASK		(0x01)
#define DATA_STRUCT_TYPE_SELF		(0x00)

#define TASK_TYPE_NORMAL			(0x00)
#define TASK_TYPE_RELAY				(0x01)
typedef struct
{
	u8 Flag;
	u8 ReportPeriodUnit;
	u8 ReportPeriod;
	u8 DataStructureType;
	
	RealTimeStruct ReportTime;	
	RealTimeStruct SampleTime;
	
	u8 SamplePeriodUnit;
	u8 SamplePeriod;
	u8 GetRate;
	u8 NormalTaskNum;
	
	u16 ExecCnt;//执行次数
	u8 DANum;
	u8 DataIDNum;	

	u16 DA[AMR_MAX_DA];	
	u32 DataID[AMR_MAX_DATA_ID];

	u16 RealExeCnt;
	u16 Crc;
}NormalTaskParaStruct;

#define RELAY_TYPE_NORMAL				(0x00)
#define RELAY_TYPE_OFF					(0x01)
#define RELAY_TYPE_ON					(0x02)
#define RELAY_TYPE_HOLD					(0x03)
#define RELAY_TYPE_RELEASE				(0x04)

//---------------- 中继任务参数
#define MAX_RELAY_TASK_NUM			(32)

typedef struct
{
	u8 Flag;
	u8 ReportPeriodUnit;
	u8 ReportPeriod;
	u8 SamplePeriodUnit;
	u8 SamplePeriod;
	u8 GetRate;		
	u8 RelayType;
	u8 RelayTaskNum;
	
	RealTimeStruct ReportTime;	
	RealTimeStruct SampleTime;
	
	u16 ExecCnt;
	u16 RealExeCnt;
	u8 Reserved2;
	u8 ComPortId;
	u8 Baud;
	u8 Parity;
	u8 DataBit;
	u8 StopBit;
	u8 TimeOut;
	u8 Length;
	u8 Frame[SIZE_256];

	u16 Reserved3;
	u16 Crc;
}RelayTaskParaStruct;

typedef struct
{
	u8 SelfAdaptionFlag; //自适应通信参数 0:非自适应 1:自适应
	u8 Reserved1;
	u8 Reserved2;
	u8 Reserved3;
}SelfAdaptionParaStruct;

typedef struct
{//258字节
	u16 AlarmDayReportFlag; // Reserved1;//err20180710 每日上报一次的告警标识，需掉电存储
	u16 Reserved1;
	
	//err20190109 时钟电池电压低、电能表时钟异常告警一天上报一次需要掉电存储，暂时将Flag数据存放在此处。该结构体增加256字节
	u8 ClkBatLowVolAlarmFLagList[SIZE_128]; //	1024/8=128 每个位代表对应测量点号的告警标志 1表示产生 0表示恢复
	u8 TimeFaultAlarmFLagList[SIZE_128];	//	1024/8=128 每个位代表对应测量点号的告警标志 1表示产生 0表示恢复

}AlarmSaveParaStruct;
//err20190216 修改在存储区地址向后增加该参数


typedef struct
{
	u8 MasterAddr[COMM_AADR_SIZE];
	u8 MasterBackAddr1[COMM_AADR_SIZE];
	u8 MasterBackAddr2[COMM_AADR_SIZE];
	u8 GateWayAddr[COMM_AADR_SIZE];
	
	u8 MasterAddrType;		//通信通道类型
	u8 MasterBackAddr1Type;
	u8 MasterBackAddr2Type;
	u8 MasterBackAddr3Type;	//网关通信通道类型
	
	u8 APN1[APN_USER_NAME_MAX_SIZE];//按规约要求为32字节
	u8 APN1UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN1PassWord[APN_USER_PWD_MAX_SIZE];
	
	u8 APN2[APN_USER_NAME_MAX_SIZE];//按规约要求为32字节
	u8 APN2UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN2PassWord[APN_USER_PWD_MAX_SIZE];

	u8 APN3[APN_USER_NAME_MAX_SIZE];//按规约要求为32字节
	u8 APN3UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN3PassWord[APN_USER_PWD_MAX_SIZE];

	u8 APN4[APN_USER_NAME_MAX_SIZE];//按规约要求为32字节
	u8 APN4UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN4PassWord[APN_USER_PWD_MAX_SIZE];

}E000011x_CommunicationParaStruct;





#define MAX_ANALOG_NUM				SIZE_2
typedef struct
{
	u8 AnalogNum;
	u8 Reserved1;
	u16 Reserved2;
	u8 AnalogID[MAX_ANALOG_NUM][SIZE_4];
	u8 AnalogStateMark[SIZE_32];
	u32 E0000900;
	u16 Reserved3;
	u16 Crc;
}AnalogCaculationParaStruct;

#define MAX_PULSE_NUM				SIZE_2
typedef struct
{
	u8 PulseNum;
	u8 Reserved1;
	u16 Reserved2;
	
	u8 PulseID[MAX_PULSE_NUM][SIZE_6];
	u8 PulseStateMark[SIZE_32];

	u16 Reserved3;
	u16 Crc;
}PulseCaculationParaStruct;

#define MAX_CACU_NUM 				SIZE_8
typedef struct
{
	u8 CaculationNum;
	u8 Reserved1;
	u16 Reserved2;
	
	u8 CaculationID[MAX_CACU_NUM][SIZE_32];
	u8 CaculationStateMark[SIZE_32];	
	
	u16 Reserved3;
	u16 Crc;
}CaculationParaStruct;


#define MAX_DIFF_CACU_NUM 			SIZE_8
typedef struct
{
	u8 DiffNum;
	u8 Reserved1;
	u16 Reserved2;	
	
	u8 DiffID[MAX_DIFF_CACU_NUM][SIZE_16];
	u8 DiffStateMark[SIZE_32];
	
	u16 Reserved3;
	u16 Crc;
}DiffCaculationParaStruct;

//---------------- RS485端口参数
#define MAX_BAUD_NUM					(12)
#define UART_FUNC_AMR					(0x00)	//00： 485 抄表；
#define UART_FUNC_ATTACH				(0x01)	//01： 485 总线级联；
#define UART_FUNC_SLAVE				(0x02)	//02： 485 被抄；（符合 DL/T645-07）
#define UART_FUNC_UPLINK				(0x03)	//03： 485 上传；（符合本通信规约）
#define UART_FUNC_MAINTAIN				(0x00)			
#define UART_FUNC_UNKNOWN				(0xFF)			

#define UART_PARITY_NONE				(0x00)
#define UART_PARITY_EVEN				(0x01)
#define UART_PARITY_ODD					(0x02)

#define UART_DATA_BIT_5					(0x05)
#define UART_DATA_BIT_6					(0x06)
#define UART_DATA_BIT_7					(0x07)
#define UART_DATA_BIT_8					(0x08)

#define UART_STOP_BIT_1					(0x00)
#define UART_STOP_BIT_1_5				(0x01)
#define UART_STOP_BIT_2					(0x02)
typedef struct
{
	u8 Function;
	u8 BaudRate;
	u8 Parity;
	u8 DataBit;
	u8 StopBit;
	u8 UsartID;
	u16 Crc;
}RS485PortParaStruct;

//---------------- 级联参数
typedef struct
{
	u8 SlaveAddr1[METER_ADDR_SIZE];
	u8 SlaveAddr2[METER_ADDR_SIZE];
	u8 SlaveAddr3[METER_ADDR_SIZE];
	u8 SlaveAddr4[METER_ADDR_SIZE];
	
	u8 MasterAddr[SIZE_4];
	u8 AMRSlaveAddr[SIZE_4];
	
	u8 MasterTimeOut;
	u8 SlaveTimeOut;
	u16 CT;
	u16 PT;
	u16 Crc;
}AttachmentParaStruct;

//---------------- 版本参数
typedef struct
{
	u16 ProtocolVersion;
	u16 ManufactureID;
	
	u8 TerminalType;
	u8 Day;
	u8 Month;
	u8 Year;
	u16 SoftWareVersion;
	u16 HardWareVersion;
}VersionParaStruct;



//---------------- 负荷管理参数
typedef struct
{
	u8 StartMinute;
	u8 StartHour;	
	u8 EffectiveFlag;	//该时段是否功控投入
	u8 Reserved;
	u32 PowerLoad;	//功率定值
}KwOptionStruct;

typedef struct
{
	KwOptionStruct Option[SIZE_8];	//8个时段
	u8 PowerCtrlPeriodCnt;		//功率控制系列 1，时段数
	u8 StartMonth;		//0xE0000D09:功率控制系列 1,执行时间
	u8 StartDay;			//0xE0000D09:功率控制系列 1,执行时间
	u8 EndMonth;		//0xE0000D09:功率控制系列 1,执行时间
	
	u8 EndDay;			//0xE0000D09:功率控制系列 1,执行时间
	u8 TI;			//0xE0000D09:频率单位
	u8 N0;		//0xE0000D09:频率值
	u8 N1;		//0xE0000D09:频率值
	
	u8 N2;	//0xE0000D09:频率值
	u8 N3;	//0xE0000D09:频率值
	u8 Reserved1;
	u8 Reserved2;	
}KwCtrlStruct;

typedef struct
{
	u8 SwitchNodeType[SIZE_16];	//开关编号 0~14的开关接点类	型,即常开常闭
	u8 InputSwitchState[SIZE_16];
	u8 OutputSwitchState[SIZE_16];	////0xE0000C20~0xE0000C2E输出开关0~14的状态，拉闸或合闸
	u8 OutputSwitchRound[SIZE_16];
	
	u16 RemoteSwitchOffTime;		//0xE0000C40：遥控拉闸时间，
	u8 PowerHoldState;
	u8 PowerCtrlFlag;			//0xE0000C60:功率控制是否有效
	
	u8 MonthKwhCtrlFlag;
	u8 PurchaseKwhCtrlFlag;
	u8 PowerCtrlRoundFlag;		//0xE0000C63:功率控制各轮次跳闸是否有效
	u8 MonthKwhCtrlRoundFlag;//0xE0000C64月电量控制跳闸轮次有效标志
	
	u8 PurchaseKwhCtrlRoundFlag;
	u8 AlarmDelayTime;	//0xE0000C68：拉闸命令告警延时，缺省 10 分钟
	u8 PowerCtrlIncFlag;//0xE0000C69功率控制用功率累加标志
	u8 KwhCtrlIncFlag;	//0xE0000C6A：电量控制用电量累加标志
	
	u32 PowerOverLoadTime;	//0xE0000C66：超过设定值跳闸的过负荷持续时间.缺省 10 分钟
	u32 PowerResetTime;
	
	u16 PowerCtrlPoint[SIZE_8];	//0xE0000C69参与累加的测量点号1~8
	u16 KwhCtrlPoint[SIZE_8];			//0xE0000C6A参与累加的测量点号1~8

	u8 E0000C6B;		//功率定值浮动系数（％）
	u8 E0000C6C;
	u8 E0000C71;	//临时限电的计算滑窗时间，单位分钟
	u8 Reserved1;
	
	u32 E0000C6D;	//最低负荷
	u32 E0000C70;	//临时限电控
	KwCtrlStruct KwCtrl[SIZE_4];
	u32 E0000E00;		//月电量控制，月电量限额（一次值）
	u32 E0000E01;		//月电量控制，轮次电量（一次值），
	u8 E0000E02[SIZE_8];
	u8 E0000E03[SIZE_8];
	u8 E0000E04[SIZE_64];
	u8 E0000E11[SIZE_12];
	
	u8 E0000E05;
	u8 E0000E10;	
	u16 Crc;
}LoadManagementParaStruct;

//---------------- 测量点控制参数
typedef struct
{
	u8 Addr[METER_ADDR_SIZE];
	
	u8 PassWord[SIZE_4];
	u8 UserCode[SIZE_4];
	
	u8 EffectiveTime;
	u8 SwitchType;
}PointParaStruct;

typedef struct
{
	PointParaStruct E0001100;
	PointParaStruct E0001101;
	PointParaStruct E0001102;
	PointParaStruct E0001103;

	u8 E0001104[SIZE_8];
	u16 Reserved1;
	u16 Crc;
		
}MeterPointCtrlParaStruct;


typedef struct
{
	u16 E0800100;		//电流不平衡度阈值，
	u16 E0800101;		//电流不平衡度恢复阈值
	u16 E0800102;		//合格电压上限
	u16 E0800103;		//合格电压下限
	u16 E0800104;		// 判断电流过流的相对额定值的比例，缺省为 1.3
	u16 E0800105;		//判断负荷过载相对额定值的比例，缺省为 1.2
	u16 E0800106;		//判断负荷过载恢复的相对额定值的比例，缺省为 0.95
	u16 E0800107;		//零相电流报警阈值，缺省值额定电流的 25%
	u16 E0800108;		//零相电流报警恢复阈值，缺省值额定电流的 15%
	u16 E0800109;		// 判断电流过负荷恢复的相对额定值的比例，缺省值为 1.0
	u16 E080010A;		// 电压不平衡度阈值
	u16 E080010B;			//电压不平衡度恢复阈值
	u16 E080010C[SIZE_4];	//失压或断相报警的判断阈值。
	
	u16 E0800200;			//电压畸变率阈值，缺省值 5%
	u16 E0800201;			//电流畸变率阈值，缺省值 5%
	u16 E0800202;		//电压畸变率报警恢复阈值缺省值 3%
	u16 E0800203;			//电流畸变率报警恢复阈值缺省值 3%
	
	u16 E0800205[SIZE_24];	//功率因数区段
	
	//u16 E0800204;
	u16 E0800206;		//监测谐波次数
	u16 E0800207;		//电压偏差上限
	u16 E0800208;
	u16 E0800209;	
	u16 E080020A;	
	u16 E080020B;	
	u16 E080020C;
	u16 E080020D;
	u16 E080020E;
	u16 Reserved;
	u16 Crc;	
}PowerQualityParaStruct;



//---------------- 测量点数据结构
#define METER_TYPE_SINGLE		(0x01)
#define METER_TYPE_MULTI		(0x02)
#define METER_TYPE_OTHER		(0x03)
#define USER_TYPE_NORMAL		(0x00)
#define USER_TYPE_IMPORTANT		(0x01)

#define PORT_ID_RS485_1			(0x00)
#define PORT_ID_RS485_2			(0x01)
#define PORT_ID_RS485_3			(0x02)
#define PORT_ID_PLC				(0x1F)
#define PORT_ID_WIRELESS		(0x20)
typedef struct
{
	u8 State;
	u8 MeasureType;
	u8 Addr[METER_ADDR_SIZE];
	
	u8 ProtocolType;
	u8 PointMeterType;
	u8 TotalType;
	u8 PriorityType;
	
	u8 SwitchOff;
	u8 MaxRateNum;
	u8 CollectorAddr[METER_ADDR_SIZE];
	
	u8 PortID;
	u8 PortBaudRate;
	u8 PortParity;
	u8 PortDataBit;
	
	u8 PortStopBit;
	u8 AMRFailCnt;
	u8 Reserved2;
	u8 Reserved3;
	
	u16 CT;
	u16 PT;
	
	u8 ConnectionType;
	u8 Reserved4;
	u16 RatedVoltage;		//E0800011	额定电压(二次侧)
	
	u32 RatedCurrent;			//E0800012 	额定电流
	u16 UserDeclareCapacity;		//E0800013	用户申报容量(折算到二次侧)
	u16 TransformerCapacity;	
	
	u8 PassWord[SIZE_8];
	u8 UserName[SIZE_8];
	
	u16 Reserved5;
	u16 Crc;
}MeasurePointStruct;



typedef enum
{
	AMR_DailyFreeze = 0x00,
	AMR_DailyStatisticFreeze,
	AMR_MonthFreeze,
	AMR_MonthStatisticFreeze,
	AMR_CurveFreeze,
	AMR_NormalTask,
	AMR_RelayTask,
    AMR_ALARM,
    AMR_EVENT,
	AMR_UNKNOWN,
	
}AMR_ENUM;

#if ((Project/100)==3) //负控

	#define DAILY_FREEZE_FILE_START				(0)
	#define DAILY_FREEZE_FILE_NUM				(62)
	#define DAILY_FREEZE_FILE_END				(DAILY_FREEZE_FILE_START + DAILY_FREEZE_FILE_NUM)

	#define DAILY_STATISTIC_FILE_START			(DAILY_FREEZE_FILE_END)
	#define DAILY_STATISTIC_FILE_NUM			(62)
	#define DAILY_STATISTIC_FILE_END			(DAILY_STATISTIC_FILE_START + DAILY_STATISTIC_FILE_NUM)

	#define MONTH_FREEZE_FILE_START				(DAILY_STATISTIC_FILE_END)
	#define MONTH_FREEZE_FILE_NUM				(12)
	#define MONTH_FREEZE_FILE_END				(MONTH_FREEZE_FILE_START + MONTH_FREEZE_FILE_NUM)

	#define MONTH_STATISTIC_FILE_START			(MONTH_FREEZE_FILE_END)
	#define MONTH_STATISTIC_FILE_NUM			(12)
	#define MONTH_STATISTIC_FILE_END			(MONTH_STATISTIC_FILE_START + MONTH_STATISTIC_FILE_NUM)

	#define CURVE_FREEZE_FILE_START				(MONTH_STATISTIC_FILE_END)
	#define CURVE_FREEZE_FILE_NUM				(30)
	#define CURVE_FREEZE_FILE_END				(CURVE_FREEZE_FILE_START + CURVE_FREEZE_FILE_NUM)

	#define NORMAL_TASK_FILE_START				(CURVE_FREEZE_FILE_END)
	#define NORMAL_TASK_FILE_NUM				(MAX_NORMAL_TASK_NUM*2)
	#define NORMAL_TASK_FILE_END				(NORMAL_TASK_FILE_START + NORMAL_TASK_FILE_NUM)

	#define RELAY_TASK_FILE_START				(NORMAL_TASK_FILE_END)
	#define RELAY_TASK_FILE_NUM					(MAX_RELAY_TASK_NUM*2)
	#define RELAY_TASK_FILE_END					(RELAY_TASK_FILE_START + RELAY_TASK_FILE_NUM)
	#define ALARM_FILE_START                    (RELAY_TASK_FILE_END)
	#define ALARM_FILE_NUM                      (2)
	#define ALARM_FILE_END                      (ALARM_FILE_START + ALARM_FILE_NUM)
	#define EVENT_FILE_START                    (ALARM_FILE_END)
	#define EVENT_FILE_NUM                      (42)
	#define EVENT_FILE_END                      (EVENT_FILE_START + EVENT_FILE_NUM)
	#define TERMINAL_EXE_FILE_START				(EVENT_FILE_END)
	#define TERMINAL_EXE_FILE_NUM				(8)
	#define TERMINAL_EXE_FILE_END				(TERMINAL_EXE_FILE_START + TERMINAL_EXE_FILE_NUM)
#endif

#if ((Project/100)==2) //I型集中器

	#define DAILY_FREEZE_FILE_START				(0)
	#define DAILY_FREEZE_FILE_NUM				(31)
	#define DAILY_FREEZE_FILE_END				(DAILY_FREEZE_FILE_START + DAILY_FREEZE_FILE_NUM)

	#define DAILY_STATISTIC_FILE_START			(DAILY_FREEZE_FILE_END)
	#define DAILY_STATISTIC_FILE_NUM			0
	#define DAILY_STATISTIC_FILE_END			(DAILY_STATISTIC_FILE_START + DAILY_STATISTIC_FILE_NUM)

	#define MONTH_FREEZE_FILE_START				(DAILY_STATISTIC_FILE_END)
	#define MONTH_FREEZE_FILE_NUM				(12)
	#define MONTH_FREEZE_FILE_END				(MONTH_FREEZE_FILE_START + MONTH_FREEZE_FILE_NUM)

	#define MONTH_STATISTIC_FILE_START			(MONTH_FREEZE_FILE_END)
	#define MONTH_STATISTIC_FILE_NUM			(0)
	#define MONTH_STATISTIC_FILE_END			(MONTH_STATISTIC_FILE_START + MONTH_STATISTIC_FILE_NUM)

	#define CURVE_FREEZE_FILE_START				(MONTH_STATISTIC_FILE_END)
	#define CURVE_FREEZE_FILE_NUM				(10)
	#define CURVE_FREEZE_FILE_END				(CURVE_FREEZE_FILE_START + CURVE_FREEZE_FILE_NUM)

	#define NORMAL_TASK_FILE_START				(CURVE_FREEZE_FILE_END)
	#define NORMAL_TASK_FILE_NUM				(MAX_NORMAL_TASK_NUM*2)
	#define NORMAL_TASK_FILE_END				(NORMAL_TASK_FILE_START + NORMAL_TASK_FILE_NUM)

	#define RELAY_TASK_FILE_START				(NORMAL_TASK_FILE_END)
	#define RELAY_TASK_FILE_NUM					(MAX_RELAY_TASK_NUM*2)
	#define RELAY_TASK_FILE_END					(RELAY_TASK_FILE_START + RELAY_TASK_FILE_NUM)
	#define ALARM_FILE_START                    (RELAY_TASK_FILE_END)
	#define ALARM_FILE_NUM                      (2)
	#define ALARM_FILE_END                      (ALARM_FILE_START + ALARM_FILE_NUM)
	#define EVENT_FILE_START                    (ALARM_FILE_END)
	#define EVENT_FILE_NUM                      (42)
	#define EVENT_FILE_END                      (EVENT_FILE_START + EVENT_FILE_NUM)
	#define TERMINAL_EXE_FILE_START				(EVENT_FILE_END)
	#define TERMINAL_EXE_FILE_NUM				(8)
	#define TERMINAL_EXE_FILE_END				(TERMINAL_EXE_FILE_START + TERMINAL_EXE_FILE_NUM)
#endif

#if ((Project/100)==5) //II型集中器

	#define DAILY_FREEZE_FILE_START				(0)
	#define DAILY_FREEZE_FILE_NUM				(31)
	#define DAILY_FREEZE_FILE_END				(DAILY_FREEZE_FILE_START + DAILY_FREEZE_FILE_NUM)

	#define DAILY_STATISTIC_FILE_START			(DAILY_FREEZE_FILE_END)
	#define DAILY_STATISTIC_FILE_NUM			(12)	//err20181122 为了存储停电次数时间冻结数据
	#define DAILY_STATISTIC_FILE_END			(DAILY_STATISTIC_FILE_START + DAILY_STATISTIC_FILE_NUM)

	#define MONTH_FREEZE_FILE_START				(DAILY_STATISTIC_FILE_END)
	#define MONTH_FREEZE_FILE_NUM				(12)	//err20181122 为了存储停电次数时间冻结数据
	#define MONTH_FREEZE_FILE_END				(MONTH_FREEZE_FILE_START + MONTH_FREEZE_FILE_NUM)

	#define MONTH_STATISTIC_FILE_START			(MONTH_FREEZE_FILE_END)
	#define MONTH_STATISTIC_FILE_NUM			(12)
	#define MONTH_STATISTIC_FILE_END			(MONTH_STATISTIC_FILE_START + MONTH_STATISTIC_FILE_NUM)

	#define CURVE_FREEZE_FILE_START				(MONTH_STATISTIC_FILE_END)
	#define CURVE_FREEZE_FILE_NUM				(10)
	#define CURVE_FREEZE_FILE_END				(CURVE_FREEZE_FILE_START + CURVE_FREEZE_FILE_NUM)

	#define NORMAL_TASK_FILE_START				(CURVE_FREEZE_FILE_END)
	#define NORMAL_TASK_FILE_NUM				(MAX_NORMAL_TASK_NUM*2)
	#define NORMAL_TASK_FILE_END				(NORMAL_TASK_FILE_START + NORMAL_TASK_FILE_NUM)

	#define RELAY_TASK_FILE_START				(NORMAL_TASK_FILE_END)
	#define RELAY_TASK_FILE_NUM					(MAX_RELAY_TASK_NUM*2)
	#define RELAY_TASK_FILE_END					(RELAY_TASK_FILE_START + RELAY_TASK_FILE_NUM)
	#define ALARM_FILE_START                    (RELAY_TASK_FILE_END)
	#define ALARM_FILE_NUM                      (2)
	#define ALARM_FILE_END                      (ALARM_FILE_START + ALARM_FILE_NUM)
	#define EVENT_FILE_START                    (ALARM_FILE_END)
	#define EVENT_FILE_NUM                      (42)
	#define EVENT_FILE_END                      (EVENT_FILE_START + EVENT_FILE_NUM)
	#define TERMINAL_EXE_FILE_START				(EVENT_FILE_END)
	#define TERMINAL_EXE_FILE_NUM				(8)
	#define TERMINAL_EXE_FILE_END				(TERMINAL_EXE_FILE_START + TERMINAL_EXE_FILE_NUM)
#endif

#if ((Project/100)==9) //配变

	#define DAILY_FREEZE_FILE_START				(0)
	#define DAILY_FREEZE_FILE_NUM				(31)
	#define DAILY_FREEZE_FILE_END				(DAILY_FREEZE_FILE_START + DAILY_FREEZE_FILE_NUM)

	#define DAILY_STATISTIC_FILE_START			(DAILY_FREEZE_FILE_END)
	#define DAILY_STATISTIC_FILE_NUM			31
	#define DAILY_STATISTIC_FILE_END			(DAILY_STATISTIC_FILE_START + DAILY_STATISTIC_FILE_NUM)

	#define MONTH_FREEZE_FILE_START				(DAILY_STATISTIC_FILE_END)
	#define MONTH_FREEZE_FILE_NUM				(12)
	#define MONTH_FREEZE_FILE_END				(MONTH_FREEZE_FILE_START + MONTH_FREEZE_FILE_NUM)

	#define MONTH_STATISTIC_FILE_START			(MONTH_FREEZE_FILE_END)
	#define MONTH_STATISTIC_FILE_NUM			(12)
	#define MONTH_STATISTIC_FILE_END			(MONTH_STATISTIC_FILE_START + MONTH_STATISTIC_FILE_NUM)

	#define CURVE_FREEZE_FILE_START				(MONTH_STATISTIC_FILE_END)
	#define CURVE_FREEZE_FILE_NUM				(10)
	#define CURVE_FREEZE_FILE_END				(CURVE_FREEZE_FILE_START + CURVE_FREEZE_FILE_NUM)

	#define NORMAL_TASK_FILE_START				(CURVE_FREEZE_FILE_END)
	#define NORMAL_TASK_FILE_NUM				(MAX_NORMAL_TASK_NUM*2)
	#define NORMAL_TASK_FILE_END				(NORMAL_TASK_FILE_START + NORMAL_TASK_FILE_NUM)

	#define RELAY_TASK_FILE_START				(NORMAL_TASK_FILE_END)
	#define RELAY_TASK_FILE_NUM					(MAX_RELAY_TASK_NUM*2)
	#define RELAY_TASK_FILE_END					(RELAY_TASK_FILE_START + RELAY_TASK_FILE_NUM)
	#define ALARM_FILE_START                    (RELAY_TASK_FILE_END)
	#define ALARM_FILE_NUM                      (2)
	#define ALARM_FILE_END                      (ALARM_FILE_START + ALARM_FILE_NUM)
	#define EVENT_FILE_START                    (ALARM_FILE_END)
	#define EVENT_FILE_NUM                      (42)
	#define EVENT_FILE_END                      (EVENT_FILE_START + EVENT_FILE_NUM)
	#define TERMINAL_EXE_FILE_START				(EVENT_FILE_END)
	#define TERMINAL_EXE_FILE_NUM				(8)
	#define TERMINAL_EXE_FILE_END				(TERMINAL_EXE_FILE_START + TERMINAL_EXE_FILE_NUM)
#endif


typedef struct
{
	u8 Addr[METER_ADDR_SIZE];	// 6bytes
	u16 DataLen;				// 2bytes
	RealTimeYYStruct Time;	//8bytes
	u32 OffSet;				// 4bytes
}AMRFileHeadStruct;		// 共20bytes

#define ALARM_OCCUR					(0x01)
#define ALARM_RESUME				(0x00)

//---------------- 告警数据结构
typedef struct
{
	u8 State;						// 									0, 	1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 									4,  6
}ARD01Struct;


typedef struct
{
	u8 State;								// 									0, 	1
	u8 ReportState;	
	u16 Reserved1;
	RealTimeStruct Time;					// 									4,  6
	u8 C_00010000[SIZE_4];					// 4 byte 当前正向有功总电能, 		12, 4
	u8 C_00020000[SIZE_4];					// 4 byte 当前反向有功总电能, 		16, 4
	u8 C_00030000[SIZE_4];					// 4 byte 当前组合无功 1 总电能, 	20, 4
	u8 C_00040000[SIZE_4];					// 4 byte 当前组合无功 2 总电能, 	24, 4
	u8 C_0201FF00[SIZE_20];			// 3*2 byte 电压数据块，A B C 相电压		28, 18
	u8 C_0202FF00[SIZE_20];			// 3*3 byte 电流数据块，A B C 相电流		48, 18
	u8 C_0203FF00[SIZE_12];			// 4*3 byte 有功功率，A B C 相有功功率		68, 12
	u8 C_0204FF00[SIZE_12];			// 4*3 byte 无功功率，A B C 相无功功率		80, 12
	u8 C_0206FF00[SIZE_8];			// 4*2 byte 总功率因数，A B C 相功率因数	92, 8
}ARD02Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 B_00010000[SIZE_4];					// 4 byte 当前正向有功总电能		12, 4
	u8 B_00020000[SIZE_4];					// 4 byte 当前反向有功总电能		16, 4
	u8 B_00030000[SIZE_4];					// 4 byte 当前组合无功 1 总电能		20, 4
	u8 B_00040000[SIZE_4];					// 4 byte 当前组合无功 2 总电能		24, 4
	u8 B_0201FF00[SIZE_20];			// 3*2 byte 电压数据块，A B C 相电压		28, 18
	u8 B_0202FF00[SIZE_20];			// 3*3 byte 电流数据块，A B C 相电流		48, 18
	u8 B_0203FF00[SIZE_12];			// 4*3 byte 有功功率，A B C 相有功功率		68, 12
	u8 B_0204FF00[SIZE_12];			// 4*3 byte 无功功率，A B C 相无功功率		80, 12
	
	u8 A_00010000[SIZE_4];					// 4 byte 当前正向有功总电能		92, 4
	u8 A_00020000[SIZE_4];					// 4 byte 当前反向有功总电能		96, 4
	u8 A_00030000[SIZE_4];					// 4 byte 当前组合无功 1 总电能		100, 4
	u8 A_00040000[SIZE_4];					// 4 byte 当前组合无功 2 总电能		104, 4
	u8 A_0201FF00[SIZE_20];			// 3*2 byte 电压数据块，A B C 相电压		108, 18
	u8 A_0202FF00[SIZE_20];			// 3*3 byte 电流数据块，A B C 相电流		128, 18
	u8 A_0203FF00[SIZE_12];			// 4*3 byte 有功功率，A B C 相有功功率		148, 12
	u8 A_0204FF00[SIZE_12];			// 4*3 byte 无功功率，A B C 相无功功率		160, 12
}ARD03Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	u8 C_00010000[SIZE_4];			// 											12, 4
	u8 C_00020000[SIZE_4];			// 											16, 4
	u8 C_00030000[SIZE_4];			// 											20, 4
	u8 C_00040000[SIZE_4];			// 											24, 4
	u8 C_E1000008[SIZE_4];			// 											28, 4
}ARD04Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	u8 B_00010000[SIZE_4];			// 											12, 4
	u8 B_00020000[SIZE_4];			// 											16, 4
	u8 B_00030000[SIZE_4];			// 											20, 4
	u8 B_00040000[SIZE_4];			// 											24, 4
	
	u8 B_E080000D[SIZE_2];			// 											28, 2
	u8 B_E080000C[SIZE_2];			// 											30, 2
	u8 A_E080000D[SIZE_2];			// 											32, 2
	u8 A_E080000C[SIZE_2];			// 											34, 2

}ARD05Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 B_00010000[SIZE_4];			// 											12, 4
	u8 B_00020000[SIZE_4];			// 											16, 4
	u8 B_00030000[SIZE_4];			// 											20, 4
	u8 B_00040000[SIZE_4];			// 											24, 4
	u8 B_E1000003[SIZE_4];			// 											28, 3
	u8 B_E1000004[SIZE_4];			// 											32, 3
	u8 A_E1000003[SIZE_4];			// 											36, 3
	u8 A_E1000004[SIZE_4];			// 											40, 3
}ARD06Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 B_00010000[SIZE_4];			// 											12, 4
	u8 B_00020000[SIZE_4];			// 											16, 4
	u8 B_00030000[SIZE_4];			// 											20, 4
	u8 B_00040000[SIZE_4];			// 											24, 4
	u8 B_01010000[SIZE_8];			// 											28, 8
	u8 B_01020000[SIZE_8];			// 											36, 8
	u8 B_01030000[SIZE_8];			// 											44, 8
	u8 B_01040000[SIZE_8];			// 											52, 8
}ARD07Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 B_040005FF[SIZE_16];			// 											12, 14
	u8 A_040005FF[SIZE_16];			// 											28, 14
}ARD08Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 B_E0000C1F[SIZE_16];			// 											12, 15
	u8 A_E0000C1F[SIZE_16];			// 											28, 15
}ARD09Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_E1800013[SIZE_4];			// 											12, 3
}ARD10Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_03300X00[SIZE_3];	//err20180925 添加端钮盖与表盖打开次数在上报告警数据
	u8 Reserved2;//err20180925 添加端钮盖与表盖打开次数在上报告警数据 数据对齐 //err20190305 修正对齐问题
	
	u8 B_00010000[SIZE_4];			// 											12, 4
	u8 B_00020000[SIZE_4];			// 											16, 4
	u8 B_00050000[SIZE_4];			// 											20, 4
	u8 B_00060000[SIZE_4];			// 											24, 4
	u8 B_00070000[SIZE_4];			// 											28, 4
	u8 B_00080000[SIZE_4];			// 											32, 4

	u8 A_00010000[SIZE_4];			// 											36, 4
	u8 A_00020000[SIZE_4];			// 											40, 4
	u8 A_00050000[SIZE_4];			// 											44, 4
	u8 A_00060000[SIZE_4];			// 											48, 4
	u8 A_00070000[SIZE_4];			// 											52, 4
	u8 A_00080000[SIZE_4];			// 											56, 4
	
}ARD11Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_04000503[SIZE_4];			// 											12, 2
}ARD12Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_04000101[SIZE_4];			// 电表日期及星期							12, 4
	u8 C_04000102[SIZE_4];			// 电表时间: 时分秒							16, 3
	u8 C_E0000130[SIZE_8];			// 终端时间									20, 6

}ARD13Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_020A0100[SIZE_2];			// A 相电压总谐波含有量，无 645 数据标识对应	12, 2
	u8 C_020A0200[SIZE_2];			// B 相电压总谐波含有量，无 645 数据标识对应	14, 2
	u8 C_020A0300[SIZE_2];			// C 相电压总谐波含有量，无 645 数据标识对应	16, 2
	u8 C_020B0100[SIZE_2];			// A 相电流总谐波含有量，无 645 数据标识对应	18, 2
	u8 C_020B0200[SIZE_2];			// A 相电流总谐波含有量，无 645 数据标识对应	20, 2
	u8 C_020B0300[SIZE_2];			// A 相电流总谐波含有量，无 645 数据标识对应	22, 2

	u8 C_E1000101[SIZE_2];			// 本日 A 相电压偏差最大值						24, 2
	u8 C_E1000102[SIZE_2];			// 本日 A 相电压偏差最大值						26, 2
	u8 C_E1000103[SIZE_2];			// 本日 A 相电压偏差最大值						28, 2
	u8 C_E1000104[SIZE_2];			// 本日 频率偏差最大值							30, 2
	u8 C_E1000105[SIZE_2];			// 本日电压不平衡度最大值						32, 2
	u8 C_E100010D[SIZE_2];			// 短时闪变最大值								34, 2
	u8 C_E100010E[SIZE_2];			// 长时闪变最大值								36, 2
	u8 C_E100010F[SIZE_2];			// 条文解释更改前: 电压总畸变率最大值，更改后: A 相电压总畸变率最大值，	38, 2
}ARD14Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_03010000[SIZE_24];			// 											12, 18
}ARD15Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_030B0000[SIZE_24];			// 											12, 18
}ARD16Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_030E0000[SIZE_24];			// 											12, 18

}ARD17Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_03300000[SIZE_4];			// 											12, 3

}ARD18Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_00010000[SIZE_4];			// 											12, 4
	u8 C_00020000[SIZE_4];			// 											16, 4
	u8 C_00030000[SIZE_4];			// 											20, 4
	u8 C_00040000[SIZE_4];			// 											24, 4

}ARD19Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Type;						// 											2,  1
	RealTimeStruct Time;			// 											4,  6

	u16 Duration;					// 											12, 2
	u16 MaxVoltage;					// 											14, 2
}ARD20Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 Di[SIZE_256];				// 											12, 256
}ARD21Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u8 MeterNum;					// 											2,  1
	u8 Res1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 MeterInfo[SIZE_8];			// 											12, 8
}ARD22Struct;

typedef enum
{
	EVENT_Program = 0x00,
	EVENT_MaxRequestAutoReset = 0x01,
	EVENT_MaxRequestManualReset,
	EVENT_PhaseAVoltageLost,
	EVENT_PhaseBVoltageLost,
	EVENT_PhaseCVoltageLost,
	EVENT_PhaseACurrentLost,
	EVENT_PhaseBCurrentLost,
	EVENT_PhaseCCurrentLost,
	EVENT_TerminalPowerDown,
	EVENT_PhaseAOff,
	EVENT_PhaseBOff,
	EVENT_PhaseCOff,
	EVENT_Control,
	EVENT_PowerClear,
	EVENT_TimeCorrection,
	EVENT_MeterBoxOpen,
	EVENT_MeterCapOpen,
	EVENT_InfoCheckError,
	EVENT_ParameterChange,
	EVENT_MeasureDoorOpen,	
}EVENT_ENUM;


//---------------- 事件数据结构
typedef struct
{
	RealTimeStruct ProgramTime;

	u8 ChType;
	u8 DINum;
	u16 Reserved1;

	u32 DI;
	u32 Data;
}ERD01Struct;


typedef struct
{
	RealTimeStruct MaxNeedClearTime;

}ERD02Struct;

typedef struct
{
	RealTimeStruct StartTime;
	RealTimeStruct EndTime;
	u8 C_00010000[SIZE_4];
	u8 C_00020000[SIZE_4];
	u8 C_00030000[SIZE_4];
	u8 C_00040000[SIZE_4];

}ERD03Struct;


typedef struct
{
	RealTimeStruct PowerDownTime;
	RealTimeStruct PowerUpTime;

}ERD04Struct;


typedef struct
{
	RealTimeStruct StartTime;
	RealTimeStruct EndTime;
	
	u8 C_E1000101[SIZE_2];
	u8 C_E1000102[SIZE_2];
	u8 C_E1000103[SIZE_2];
	u8 C_E1000104[SIZE_2];
	u8 C_E1000105[SIZE_2];
	u8 C_E100010D[SIZE_2];
	u8 C_E100010E[SIZE_2];
	u8 C_E100010F[SIZE_2];

}ERD05Struct;


typedef struct
{
	RealTimeStruct Time;
	u32 Type;
	u8 C_00010000[SIZE_4];
	u8 C_00020000[SIZE_4];	
}ERD06Struct;


typedef struct
{
	RealTimeStruct ErrorTime;	
	u8 PW[SIZE_16];	
	u8 MSA[SIZE_4];
}ERD07Struct;

typedef struct
{
	RealTimeStruct Time;
	u32 DI;

}ERD08Struct;


typedef struct
{
	RealTimeStruct Time;

	u8 C_00010000[SIZE_4];
	u8 C_00020000[SIZE_4];

}ERD09Struct;


typedef struct
{
    u8 MeterAddr[6];
    u16 contentLen;
    u32 DataID;
	u32 contentOffset ;
    RealTimeYYStruct OcurTime;
} AlarmEventFileHeadStruct;


#define NONE_ACESS		(0x00)
#define R_ENABLE		(0x01)
#define W_ENABLE		(0x02)
#define W_R_ENABLE		(0x03)
typedef struct
{
	u32 DI;
	u32 Addr;
	u16 Len;
	u8 RW;
	u8 Reserved;
	//u8* InitData;
}QCSG_DITableStruct;


typedef struct
{
	u16 OffSet;
	u16 Len;
}DataDescriptorStruct;

typedef struct
{
	u32 DI;
	u32 Addr;
	u16 StoreLen;
	u16 DataLen;
	u16 RW;
	u16 DescNum;
	const DataDescriptorStruct* Descriptor;
}DIDataInfoStruct;


typedef  struct
{
    RealTimeYYStruct OcurTime;
    u32 offset;
    u32 dataLen;
}RelayTaskDataFileHeadStruct;

typedef struct
{
    RealTimeYYStruct OcurTime;
    u32 offset;
}normalTaskDataFileHeadStruct;

typedef struct
{
	u32 DI_645;
	u16 MaxLen;
	u8 DefLen;
	u8 SingleLen;
}DI_645Struct;


typedef struct
{
	u32 SE_DI;
	u16 Len;
	u8 DI_645_07_Num;
	u8 DI_645_97_Num;
	const DI_645Struct* DI_645_07;
	const DI_645Struct* DI_645_97;
}SE_645_DIStruct;

#define ALAMR_FLAG_LESS_MONEY_E200002B				(0x00000001)
#define ALAMR_FLAG_POWER_GO_DOWN_E200002C			(0x00000002)
#define ALAMR_FLAG_POWER_STOP_E200002E				(0x00000004)
#define ALAMR_FLAG_PROGRAM_TIME_CHANGE_E2000035		(0x00000008)
#define ALAMR_FLAG_TIME_RATE_CHANGE_E2000036		(0x00000010)
#define ALAMR_FLAG_RELAY_CHANGE_E200003B			(0x00000020)
#define ALAMR_FLAG_RELAY_CONTROL_FAIL_E200003C		(0x00000040)
#define ALAMR_FLAG_AMR_FAIL_E200003D				(0x00000080)
#define ALAMR_FLAG_TIME_FAULT_E200003E				(0x00000100)
#define ALAMR_FLAG_VOLTAGE_LOSS_E2000040			(0x00000200)
#define ALAMR_FLAG_CURRENT_LOSS_E2000041			(0x00000400)
#define ALAMR_FLAG_CURRENT_REVERSE_E2000042			(0x00000800)
#define ALAMR_FLAG_VOLTAGE_A_LOSS_E2000016			(0x00001000)
#define ALAMR_FLAG_CURRENT_A_LOSS_E2000013			(0x00002000)
#define ALAMR_FLAG_CURRENT_A_REVERSE_E200000D		(0x00004000)
#define ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A		(0x00008000)
#define ALAMR_FLAG_CLK_BATT_LOW_E2000032			(0x00010000)     	//ybzyl 20180627 //20181218 名字改了一下ALAMR_FLAG_CLK_BATT_LOW_E2000032
#define ALAMR_FLAG_VOLTAGE_B_LOSS_E2000017			(0x00020000)		//err20181218 增加B C相失压失流潮流反向告警
#define ALAMR_FLAG_VOLTAGE_C_LOSS_E2000018			(0x00040000)
#define ALAMR_FLAG_CURRENT_B_LOSS_E2000014			(0x00080000)
#define ALAMR_FLAG_CURRENT_C_LOSS_E2000015			(0x00100000)
#define ALAMR_FLAG_CURRENT_B_REVERSE_E200000E		(0x00200000)
#define ALAMR_FLAG_CURRENT_C_REVERSE_E200000F		(0x00400000)





#define ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A_SAVE_FLAG (0x0001) //月通信流量越限 掉电保存标识，防止上电标识清零重新产生告警

#define METER_CONTROL_SWITCH_OFF					(0x01)
#define METER_CONTROL_SWITCH_ON						(0x02)
#define METER_CONTROL_POWER_HOLD					(0x03)
#define METER_CONTROL_POWER_HOLD_RELEASE			(0x04)

typedef struct
{
	u32 PowerNoneZeroStartSecond;	// 电表功率非 0值，起始 Second
	u32 MeterControlStartSecond;	// 电表控制起始 Second
	u16 MeterControlLastSecond;		// 测量点控制生效时间大小，秒
	u8  MeterControlType;		// 4种类型:拉闸，合闸，保电，保电解除
	u8  Res1;
	u32 AlarmFlag;					// 已产生的告警标志位
	u8 State[SIZE_4];				// byte0:表示数据是否有效，0；无效，1；有效，byte1:表示数据是否更新，0；未更新，1；已更新

	u8 DI_040005FF[SIZE_16];		// 16 byte 电能表运行状态字  ybzyl20180627
	u8 DI_04000503[SIZE_2];			// 2 byte 电能表运行状态字 3          ybzyl20180627 err20190312 台体继电器变位告警读状态字3产生，读数据块回复D1。
	u16 Res2;						// 对齐
	
	//u8 DI_04000504[SIZE_2];			// 2 byte 电能表运行状态字 4          ybzyl20180627
	// 以下部分合计160字节
	u8 DI_04000101[SIZE_4];			// 4 byte 电能表日期以及星期
	u8 DI_04000102[SIZE_4];			// 3 byte 电能表时间，秒、分、时
	u8 DI_03300500[SIZE_4];			// 3 byte 电能表时段或费率更改
	u8 DI_00900200[SIZE_4];			// 4 byte 电能表当前剩余金额 XXXXXX.XX(元)
	u8 DI_00010000[SIZE_4];			// 4 byte 当前正向有功总电能
	u8 DI_00020000[SIZE_4];			// 4 byte 当前反向有功总电能
	u8 DI_00030000[SIZE_4];			// 4 byte 当前组合无功 1 总电能
	u8 DI_00040000[SIZE_4];			// 4 byte 当前组合无功 2 总电能
	u8 DI_0201FF00[SIZE_8];			// 3*2 byte 电压数据块，A B C 相电压
	u8 DI_0202FF00[SIZE_12];		// 3*3 byte 电流数据块，A B C 相电流
	u8 DI_0203FF00[SIZE_12];		// 4*3 byte 有功功率，A B C 相有功功率
	u8 DI_0204FF00[SIZE_12];		// 4*3 byte 无功功率，A B C 相无功功率
	u8 DI_0206FF00[SIZE_8];			// 4*2 byte 总功率因数，A B C 相功率因数

	u8 DI_03300000[SIZE_4];			// 3 byte 电能表编程时间更改

	u8 DI_10010001[SIZE_4];			// A B C 相失压次数及时间
	u8 DI_10010002[SIZE_4];
	u8 DI_10020001[SIZE_4];
	u8 DI_10020002[SIZE_4];
	u8 DI_10030001[SIZE_4];
	u8 DI_10030002[SIZE_4];

	u8 DI_18010001[SIZE_4];			// A B C 相失流次数及时间
	u8 DI_18010002[SIZE_4];
	u8 DI_18020001[SIZE_4];
	u8 DI_18020002[SIZE_4];
	u8 DI_18030001[SIZE_4];
	u8 DI_18030002[SIZE_4];

	u8 DI_1B010001[SIZE_4];			// A B C 相潮流反向次数及时间
	u8 DI_1B010002[SIZE_4];
	u8 DI_1B020001[SIZE_4];
	u8 DI_1B020002[SIZE_4];
	u8 DI_1B030001[SIZE_4];
	u8 DI_1B030002[SIZE_4];
	
	//err20181026 更新后表格长度为196 原为184	//20190312 更改为200长度
}AlarmAMRDataStruct;



#define REPORT_IDLE					(0x00)
#define REPORT_READ_DATA			(0x01)
#define REPORT_TX					(0x02)
#define REPORT_WAIT_ACK				(0x03)

#define REPORT_ID_NORMAL_TASK		(0x01)
#define REPORT_ID_RELAY_TASK		(0x02)
#define REPORT_ID_ALARM				(0x04)

#define REPORT_ALARM_MAX_NUM		(20)
#define REPORT_MAX_RETRY_CNT		(0x03)
#define REPORT_ACK_TIME_OUT			(20)
#define REPORT_ACK_TIME_OUT_TICK	(6000)
#define REPORT_FLAG_SEND_OK			(0x0001)
#define REPORT_FLAG_SEND_FAIL		(0x0002)

#define REPORT_CH_ETHE				(0x01)
#define REPORT_CH_GPRS				(0x02)

#define REPORT_DELAY_TICKS			(100)
typedef struct
{
	u8 State;
	u8 ID;
	u8 Step;
	u8 CheckMinute;
	u16 Flag;		//发送成功或失败
	u16 Index;
	u32 DelayStartTick;
	u32 SendStartTick;
	u32 LatelyAlarmNum;
	u32 FileName;
	QCSGFrameProcessStruct QCSGFp;
	MultiFrameSendReqStruct SendReq;
	u8 NormalTaskReportExe[MAX_NORMAL_TASK_NUM];
	u8 RelayTaskReportExe[MAX_RELAY_TASK_NUM];
	AlarmEventFileHeadStruct AlarmHead;
}ReportTaskStruct;



typedef struct
{
	u16 Type;						// 文件属性，表示升级文件的类型
	u16 TotalCount;					// 升级文件的总包数
	u32 TotalLen;					// 升级文件的总长度
	u8 Name[SIZE_32];				// 升级文件的名称
	u16 Crc;						// 升级文件的 CRC 校验
	u16 CountLen;					// 升级文件单包数据长度，此数据由 TotalCount 与 TotalLen 计算得出
}TerminalExeFileInfoStruct;

#define EXE_FILE_TYPE_MS			(0x0000)
#define EXE_FILE_TYPE_OS			(0x0001)
#define EXE_FILE_TYPE_Meter			(0x0002)
#define EXE_FILE_TYPE_Terminal		(0x0003)
#define EXE_FILE_TYPE_Menu			(0x0004)
#define EXE_FILE_TYPE_Font			(0x0005)
#define EXE_FILE_TYPE_PACK			(0x0006)
#define EXE_FILE_TYPE_PLC			(0x0007)

typedef struct
{	
	u8 Addr[METER_ADDR_SIZE];
	u8 CollectorAddr[METER_ADDR_SIZE];
	
	u8 ProtocolType;
	u8 PointMeterType;	
	u8 TotalType;
	u8 UerType;
	
	u8 PortID;
	u8 PortBaudRate;
	u8 PortParity;
	u8 PortDataBit;
	
	u8 PortStopBit;	
	u8 Effective;
	u8 FailCnt;
	u8 Res1;
	
	u32 Flag;
	u16 Res2;
	u16 PnNum;
}NodeListStruct;


#define NODE_FLAG_SYNC_TO_PLC_OK			(0x80)			// 节点是否已同步到 PLC 模块，0:未同步，1:已同步
#define NODE_FLAG_NEED_FILE_SYNC			(0x40)			// 节点是否需要同步到 PLC 模块，0:不需要，1:需要
#define NODE_FLAG_DAY_FREEZ_OK				(0x01)			// 节点日冻结抄表是否成功，0:未成功，1:成功
#define NODE_FLAG_AMR_HOUR_OK				(0x02)			// 节点整点抄表是否成功，0:未成功，1:成功
#define NODE_FLAG_POWER_STOP				(0x04)			// 节点电能停走告警产生


#define METER_FILE_SYNC_TO_RAM_FLAG			(0x55)
#define METER_FILE_SYNC_TO_PLC_FLAG			(0x55)
#define METER_FILE_SYNC_DELAY_TIME			(60)
typedef struct 
{
	u8 CountFlag;				// 测量点档案统计标志位 0: 不需统计，0x55: 需要统计
	u8 PLCMeterSyncFlag;		// 载波表同步到本地模块标识,0: 不需同步，0x55: 需要同步
    u16 MaxMeters;				// 测量点最大数量
    u16 TotalMeters;			// 总表数
    u16 ImportantMeters;		// 重点表数
    u16 NormalMeters;			// 普通表数
    u16 SinglePhaseMeters;		// 单相表数
    u16 MultifunctionMeters;	// 三相表数
    u16 OtherMeters;			// 其他表数
    u16 PLCMeters;				// PLC 通道表总数
    u16 NewPLCMeters;			// 档案同步后，档案发生变动时，新的需要添加到 PLC 模块的表数量
    u16 RS485Meters;			// RS485通道表总数
    u16 RS485_CH1_Meters;		// RS485通道 1 表总数
    u16 RS485_CH2_Meters;		// RS485通道 2 表总数
    u16 RS485_CH3_Meters;		// RS485通道 3 表总数
	u16 AMRDayFreezFailMeters;
	u16 AMRMonthFreezFailMeters;
    u16 PulseAndPortNum;		// 终端最大脉冲量与串行端口数量
    //u16 PLCNodeListCrc;
    //u16 RS485NodeListCrc;	 	//校验码
    u16 NodeListCrc;
}MeterFileInfoStruct;


#define GPRS_STATE_UNPLUGGED				(0)
#define GPRS_STATE_POWER_OFF				(1)
#define GPRS_STATE_POWER_ON					(2)
#define GPRS_STATE_MATCH_BAUD_RATE			(3)
#define GPRS_STATE_RECONGNIZE_SIM_CARD		(4)
#define GPRS_STATE_REG_NET					(5)
#define GPRS_STATE_CONFIG_LINK_PARA			(6)
#define GPRS_STATE_LINKING					(7)
#define GPRS_STATE_LINK_FAIL				(8)
#define GPRS_STATE_LINK_SUCCESS				(9)
#define GPRS_STATE_LOG_SUCCESS				(10)





#endif











