
/*******************************************************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_UpLinkProtocol.h
��  ��:zhaoyun
��  ��:1.00
��  ��:20170501
��  ��:��������ͨ�Ź�Լ
*******************************************************************************************/
#ifndef __QCSG_UP_LINK_PROTOCOL__
#define __QCSG_UP_LINK_PROTOCOL__

#define TEST_ON			(1)
#define YBWHZ_TEST      (0)//����ʱʹ��
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
// ���²���Ϊ��������Э���֡��ʽ�����봦����صĺ궨��
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
	u8 Report;		//�Ƿ�����������(0:��	��0:��)
	u8 Ack;			//�Ƿ��յ���վ��ȷ��֡(1:�� 0:��)
}RealTimeYYStruct;

#define TERMINAL_POWER_UP_FLAG					(0x0001)
#define TERMINAL_POWER_DOWN_DONE_FLAG			(0x0002)
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)       //II�ͼ��������б�̼�20180515ybzyl
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
	u16 DataLen;		// Data���������ݳ��ȣ�ע�����֡�ṹ�齨����֡ʱ��DataLen���� FC, A1, A2, A3, AFN, SEQ �ĳ���
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
	u16 Pn;							// ��ǰ������ Pn ֵ
	u16 SrcDataPos;					// ����Դ���������ݵ�λ��
	u16 SrcDataPosCopy;
	u16 DataIDIndex;				// ���ݱ�ʶ�ڱ����е�������
	u16 DstDataPos;					// Ӧ��֡��ǰ���ݱ�ʶ�����ݴ��λ��
	u16 UserDataLen;				// Դ�����ܳ���
	u16 TempPn;						// ���ܵ� Pn ���õ��Ķ�Ӧ����������� Pn ֵ
	u32 CurDataID;					// ��ǰ���������ݱ�ʶ
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

#define UART_TX_DELAY_GAP				(2)				// ���ڷ��͵�֡����ʱ�䣬10ms ��λ
#define UART_DEFAULT_ACK_TIME			(3)				// Ĭ�ϵĴ��ڵȴ�Ӧ��ʱʱ�䣬 S ��λ
#define UART_TASK_NUM					(9)
typedef struct
{
	TaskIDEnum SrcTaskID;		// �������������Դ�����
	UartStateEnum State;		// ��������״̬
	u8 NeedAck;					// ͨ�����ڷ��͵�֡�Ƿ�Ҫ��Ӧ��, 0 ����Ҫ, 1 ��ҪӦ��
	u8 FrameType;				// ��ǰ֡��Э������
	u8 RetryCnt;				// ���Դ������ݼ�������Ϊ0ʱֹͣ�ط���
	u8 Reserved;
	u16 BpsCtrl;				// ������ʱʹ�õĲ����ʿ���
	u32 Reserved2;
	u16 AckTiemOutSetByTask;	// ����������õĳ�ʱӦ��ʱ��
	u16 WaitAckTimeOut;			// ��ǰ�˿ڵȴ�Ӧ��ʱʱ��
	u32 PriorityLock;			// 
	u16 Reserved1;
	u16 Flag;					// ������־λ
	u16 AckLen;					// ��Ӧ֡�ܳ���
	u16 FrameLen;				// ����֡�ܳ���
	u8* FrameBuf;				// ����ָ֡��
}UartTaskCtrlStruct;

#define BUFFER_EMPTY		(0x00)
#define BUFFER_FULL			(0x01)

#define DATA_TYPE_STRUCT		(0x00)	//ʵʱ����	
#define DATA_TYPE_RELAY			(0x01)	//�м�
#define DATA_TYPE_FRAME			(0x02)	//����ָ��
#define DATA_TYPE_BCAST_CMD		(0x03)	//�㲥
#define PN_LIST_MAX_NUM			SIZE_1K
#if ((Project/100)==9)
#define PN_LIST_MAX_NUM_JC		SIZE_8 //��佻�����֧��һ�γ���64��������
#endif
#define TASK_FRAME_INDICATION	(0x0001)
#define UART_FRAME_INDICATION	(0x0002)
typedef struct
{
	u8 SrcPort;					// Դ�˿�
	u8 Empty;					// Դ�˿ڵĻ������Ƿ�Ϊ��, 0��ʾ�գ����Ի������ݣ������Ӧ�����������
	u8 FrameType;				// ��ǰ֡��Э������
	u8 DataType;				// ��ǰ����ָ���Ӧ��������֯��ʽ��0: ��֡���ݣ������֡���� 1: ����������ݣ���������ݻ���
	QCSGFrameInfoStruct QCSGFrameInfo;
	u16 Reserved5;
	u16 PnListNum;				// ������Э����Ҫ���� PnDIList�б�ʱ����ʾ�б�����
	u16 Flag;					// ������־λ
	u16 FrameLen;				// ����֡�ܳ���
	u8* FrameBuf;				// ����ָ֡��
}UartRxChCtrlStruct;


typedef struct
{
	TaskIDEnum SrcTask;
	u8 NeedAck;
	u8 FrameType;
	u8 RetryCnt;
	u16 BpsCtrl;
	u16 TimeOut;
	u32 DataLen;				// ����֡�ܳ���
	u8* DataBuf;				// ����ָ֡��
}UartTxReqStruct;

#define MF_SEND_START				(0x0001)
#define MF_SEND_SINGLE_OK			(0x0002)
#define MF_SEND_SINGLE_TIMEOUT		(0x0004)
typedef struct					// �����ݽṹ��Ҫ���ڣ�����Э��ͨ������Ҫ���з�֡���͵�����
{
	TaskStateEnum State;		// ��֡��������״̬
	TaskIDEnum SrcTask;
	u8 RetryCnt;				// ���Դ������ݼ�������Ϊ0ʱֹͣ�ط���
	u8 Empty;					// Ŀ��˿ڵĻ������Ƿ�Ϊ��
	u8 FrameType;				// ��ǰ֡��Э������
	u8 DataType;				// ��ǰ����ָ���Ӧ��������֯��ʽ��0: ��֡���ݣ� 1: �����������
	u8 AckToSrcTask;			// ��֡��������ɺ��Ƿ���Ҫ�����֡���͵��������ȷ��
	u8 NeedAck;					// ��֡���͹����У�ÿһ֡�Ƿ���ҪĿ��˿ڽ���Ӧ��ȷ��
	u16 PnListNum;
	u16 Round;
	
	QCSGFrameInfoStruct QCSGFrameInfo;
	u16 WaitAckTimeOut;			// ��ǰ�˿ڵȴ�Ӧ��ʱʱ��
	u16 Flag;					// ������־λ
	u32 CurTimePos;				// ��Ҫ�û���ͨ�����֡����ʾ��ǰ����ʱ���λ��
	u32 CurTimeDataLen;			// ��Ҫ�û���ͨ�����֡����ʾ��ǰ����ʱ���Ӧ�����ݳ���
	u32 CurTimeDataEndPos;		// ��Ҫ�û���ͨ�����֡����ʾ��ǰ����ʱ���Ӧ���ݵĽ���λ��
	u32 LastDataPos;			// �ϴ��齨����֡ʱ����ʼλ��
	u32 DataPos;				// ���ݷ��͵�λ��
	u32 DataLen;				// ����֡�ܳ���
	u8* DataBuf;				// ����ָ֡��
}MultiFrameSendTaskStruct;


typedef struct					// �����ݽṹ��Ҫ��������Э��ͨ������Ҫ���з�֡���͵���������
{
	TaskIDEnum SrcTask;
	u8 RetryCnt;				// ���Դ������ݼ�������Ϊ0ʱֹͣ�ط���
	u8 FrameType;				// ��ǰ֡��Э������
	u8 DataType;				// ��ǰ����ָ���Ӧ��������֯��ʽ��0: ��֡���ݣ� 1: �����������
	u8 AckToSrcTask;			// ��֡��������ɺ��Ƿ���Ҫ�����֡���͵��������ȷ��
	u8 NeedAck;					// ��֡���͹����У�ÿһ֡�Ƿ���ҪĿ��˿ڽ���Ӧ��ȷ��
	u16 Res;
	
	QCSGFrameInfoStruct QCSGFrameInfo;
	u16 WaitAckTimeOut;			// ��ǰ�˿ڵȴ�Ӧ��ʱʱ��
	u16 PnListNum;				// ���ݷ��͵�λ��
	u32 DataLen;				// ����֡�ܳ���
	u8* DataBuf;				// ����ָ֡��
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

//---------------- ͨ�Ų���
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
	
	u8 MasterAddrType;		//ͨ��ͨ������
	u8 MasterBackAddr1Type;
	u8 MasterBackAddr2Type;
	u8 CompressType;

	u8 GateWayAddr[COMM_AADR_SIZE];
	u8 APN[APN_NAME_MAX_SIZE];
	u8 APNUserName[APN_USER_NAME_MAX_SIZE];
	u8 APNPassWord[APN_USER_PWD_MAX_SIZE];

	u8 HeartBeatTimeGap;
	u8 RedialTimeGap;	//�����ز������ȱʡ 30 ����
	u8 RedialCnt;
	u8 IpLinkType;
	
	u32 TerminalLocationCode;
	u32 TerminalAddr;
	
	u8 CommunicationMode;
	u8 IpAddrAccessType;	//�������� IP ��ַ��ȡ��ʽ0���ֶ����� IP ��ַ    1���Զ����� IP ��ַ
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
	
	u8 MasterAddrType;		//ͨ��ͨ������

	u8 APN[APN_NAME_MAX_SIZE];
	u8 APNUserName[APN_USER_NAME_MAX_SIZE];
	u8 APNPassWord[APN_USER_PWD_MAX_SIZE];
}SelfAdaptionCommunicationParaStruct;

//---------------- �澯�¼����β���
typedef struct
{
	u8 AlarmReportMask[SIZE_32];
	u8 AlarmJudgeMask[SIZE_32];
	u8 EventRecordMask[SIZE_32];
	
	u16 Reserved1;//u16 AlarmDayReportFlag;  //err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢 //err20190216 �ı� �洢λ��
	
	//err20190109 ʱ�ӵ�ص�ѹ�͡����ܱ�ʱ���쳣�澯һ���ϱ�һ����Ҫ����洢����ʱ��Flag���ݴ���ڴ˴����ýṹ������256�ֽ�
	//u8 ClkBatLowVolAlarmFLagList[SIZE_128];	//	1024/8=128 ÿ��λ������Ӧ������ŵĸ澯��־ 1��ʾ���� 0��ʾ�ָ�//err20190216 �ı� �洢λ��
	//u8 TimeFaultAlarmFLagList[SIZE_128];	//	1024/8=128 ÿ��λ������Ӧ������ŵĸ澯��־ 1��ʾ���� 0��ʾ�ָ�//err20190216 �ı� �洢λ��

	u16 Crc;
}AlarmEventMaskParaStruct;


#define HOUR_BCD_MIN		(0x00)
#define HOUR_BCD_MAX		(0x23)
#define DAY_BCD_MIN			(0x01)
#define DAY_BCD_MAX			(0x31)
//---------------- �����ղ���
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

//---------------- ������֤����
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

//---------------- ��ֵ����
typedef struct
{
	u8 E0000180;		//���ַ�������ʱ��
	u8 E0000181;		//���ָֻ�����ʱ��
	u8 E0000182;		//�ж϶��ࡢʧѹ��ʧ����������ʱ�䣬ȱʡֵ1 ����
	u8 E0000183;		//�ж϶��ࡢʧѹ��ʧ���ָ�����ʱ�䣬ȱʡֵ1 ����
	
	u8 E0000184;		//�жϵ�ѹ������ʱ�䣬ȱʡֵ15 ����
	u8 E0000185;		//�жϵ�ѹ������ָ�ʱ�䣬ȱʡֵ15 ����
	u8 E0000186;		//�жϵ���������ʱ�䣬ȱʡֵ15 ����
	u8 E0000187;		//	�жϵ��������Իָ�ʱ�䣬ȱʡֵ15 ����
	
	u8 E0000188;		//	���ܱ�ʱ���쳣��ֵ��ȱʡֵ10 ����
	u8 E0000189;		//	���ܱ�������ֵ��ȱʡΪ10 ��
	u16 E000018A;		//	���ܱ�ͣ����ֵ��
	
	u32 E000018B;		//ʣ�������ֵ��ȱʡΪ20.00 Ԫ
	u32 E000018C;		//��ͨ������Խ��ֵ

	u16 Reserved1;
	u16 Crc;
}ThresholdParaStruct;

//---------------- ��������

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
	u8 Year;			//��׼ʱ��
	u8 PeriodUnit;		//���ڵ�λ
	u8 Period;			//����ֵ
	u8 Reserved1;

	u16 Reserved2;
	u16 Crc;
}AMRChTimeStruct;		//��������(����)


//����ͨ������
#define AMR_MAX_DA					SIZE_256
#define AMR_MAX_DATA_ID				SIZE_32

typedef struct
{
	u8 AMRChSetNum;
	u8 Flag;				//��Ч�Ա�־
	u16 StoreNum;		//�洢����
	
	u8 ChMeterType;		//�������
	u8 UserType;			//�ص��û�����
	u8 DANum;			//��Ϣ���ʶ����
	u8 DataIDNum;		//���ݱ�ʶ��������
	
	u16 DA[AMR_MAX_DA];	
	u32 DataID[AMR_MAX_DATA_ID];

	u16 Reserved1;
	u16 Crc;
}AMRChSetStruct;		//��������(��Ч��)

//---------------- ��ͨ�������
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
	
	u16 ExecCnt;//ִ�д���
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

//---------------- �м��������
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
	u8 SelfAdaptionFlag; //����Ӧͨ�Ų��� 0:������Ӧ 1:����Ӧ
	u8 Reserved1;
	u8 Reserved2;
	u8 Reserved3;
}SelfAdaptionParaStruct;

typedef struct
{//258�ֽ�
	u16 AlarmDayReportFlag; // Reserved1;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
	u16 Reserved1;
	
	//err20190109 ʱ�ӵ�ص�ѹ�͡����ܱ�ʱ���쳣�澯һ���ϱ�һ����Ҫ����洢����ʱ��Flag���ݴ���ڴ˴����ýṹ������256�ֽ�
	u8 ClkBatLowVolAlarmFLagList[SIZE_128]; //	1024/8=128 ÿ��λ������Ӧ������ŵĸ澯��־ 1��ʾ���� 0��ʾ�ָ�
	u8 TimeFaultAlarmFLagList[SIZE_128];	//	1024/8=128 ÿ��λ������Ӧ������ŵĸ澯��־ 1��ʾ���� 0��ʾ�ָ�

}AlarmSaveParaStruct;
//err20190216 �޸��ڴ洢����ַ������Ӹò���


typedef struct
{
	u8 MasterAddr[COMM_AADR_SIZE];
	u8 MasterBackAddr1[COMM_AADR_SIZE];
	u8 MasterBackAddr2[COMM_AADR_SIZE];
	u8 GateWayAddr[COMM_AADR_SIZE];
	
	u8 MasterAddrType;		//ͨ��ͨ������
	u8 MasterBackAddr1Type;
	u8 MasterBackAddr2Type;
	u8 MasterBackAddr3Type;	//����ͨ��ͨ������
	
	u8 APN1[APN_USER_NAME_MAX_SIZE];//����ԼҪ��Ϊ32�ֽ�
	u8 APN1UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN1PassWord[APN_USER_PWD_MAX_SIZE];
	
	u8 APN2[APN_USER_NAME_MAX_SIZE];//����ԼҪ��Ϊ32�ֽ�
	u8 APN2UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN2PassWord[APN_USER_PWD_MAX_SIZE];

	u8 APN3[APN_USER_NAME_MAX_SIZE];//����ԼҪ��Ϊ32�ֽ�
	u8 APN3UserName[APN_USER_NAME_MAX_SIZE];
	u8 APN3PassWord[APN_USER_PWD_MAX_SIZE];

	u8 APN4[APN_USER_NAME_MAX_SIZE];//����ԼҪ��Ϊ32�ֽ�
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

//---------------- RS485�˿ڲ���
#define MAX_BAUD_NUM					(12)
#define UART_FUNC_AMR					(0x00)	//00�� 485 ������
#define UART_FUNC_ATTACH				(0x01)	//01�� 485 ���߼�����
#define UART_FUNC_SLAVE				(0x02)	//02�� 485 ������������ DL/T645-07��
#define UART_FUNC_UPLINK				(0x03)	//03�� 485 �ϴ��������ϱ�ͨ�Ź�Լ��
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

//---------------- ��������
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

//---------------- �汾����
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



//---------------- ���ɹ�������
typedef struct
{
	u8 StartMinute;
	u8 StartHour;	
	u8 EffectiveFlag;	//��ʱ���Ƿ񹦿�Ͷ��
	u8 Reserved;
	u32 PowerLoad;	//���ʶ�ֵ
}KwOptionStruct;

typedef struct
{
	KwOptionStruct Option[SIZE_8];	//8��ʱ��
	u8 PowerCtrlPeriodCnt;		//���ʿ���ϵ�� 1��ʱ����
	u8 StartMonth;		//0xE0000D09:���ʿ���ϵ�� 1,ִ��ʱ��
	u8 StartDay;			//0xE0000D09:���ʿ���ϵ�� 1,ִ��ʱ��
	u8 EndMonth;		//0xE0000D09:���ʿ���ϵ�� 1,ִ��ʱ��
	
	u8 EndDay;			//0xE0000D09:���ʿ���ϵ�� 1,ִ��ʱ��
	u8 TI;			//0xE0000D09:Ƶ�ʵ�λ
	u8 N0;		//0xE0000D09:Ƶ��ֵ
	u8 N1;		//0xE0000D09:Ƶ��ֵ
	
	u8 N2;	//0xE0000D09:Ƶ��ֵ
	u8 N3;	//0xE0000D09:Ƶ��ֵ
	u8 Reserved1;
	u8 Reserved2;	
}KwCtrlStruct;

typedef struct
{
	u8 SwitchNodeType[SIZE_16];	//���ر�� 0~14�Ŀ��ؽӵ���	��,����������
	u8 InputSwitchState[SIZE_16];
	u8 OutputSwitchState[SIZE_16];	////0xE0000C20~0xE0000C2E�������0~14��״̬����բ���բ
	u8 OutputSwitchRound[SIZE_16];
	
	u16 RemoteSwitchOffTime;		//0xE0000C40��ң����բʱ�䣬
	u8 PowerHoldState;
	u8 PowerCtrlFlag;			//0xE0000C60:���ʿ����Ƿ���Ч
	
	u8 MonthKwhCtrlFlag;
	u8 PurchaseKwhCtrlFlag;
	u8 PowerCtrlRoundFlag;		//0xE0000C63:���ʿ��Ƹ��ִ���բ�Ƿ���Ч
	u8 MonthKwhCtrlRoundFlag;//0xE0000C64�µ���������բ�ִ���Ч��־
	
	u8 PurchaseKwhCtrlRoundFlag;
	u8 AlarmDelayTime;	//0xE0000C68����բ����澯��ʱ��ȱʡ 10 ����
	u8 PowerCtrlIncFlag;//0xE0000C69���ʿ����ù����ۼӱ�־
	u8 KwhCtrlIncFlag;	//0xE0000C6A�����������õ����ۼӱ�־
	
	u32 PowerOverLoadTime;	//0xE0000C66�������趨ֵ��բ�Ĺ����ɳ���ʱ��.ȱʡ 10 ����
	u32 PowerResetTime;
	
	u16 PowerCtrlPoint[SIZE_8];	//0xE0000C69�����ۼӵĲ������1~8
	u16 KwhCtrlPoint[SIZE_8];			//0xE0000C6A�����ۼӵĲ������1~8

	u8 E0000C6B;		//���ʶ�ֵ����ϵ��������
	u8 E0000C6C;
	u8 E0000C71;	//��ʱ�޵�ļ��㻬��ʱ�䣬��λ����
	u8 Reserved1;
	
	u32 E0000C6D;	//��͸���
	u32 E0000C70;	//��ʱ�޵��
	KwCtrlStruct KwCtrl[SIZE_4];
	u32 E0000E00;		//�µ������ƣ��µ����޶һ��ֵ��
	u32 E0000E01;		//�µ������ƣ��ִε�����һ��ֵ����
	u8 E0000E02[SIZE_8];
	u8 E0000E03[SIZE_8];
	u8 E0000E04[SIZE_64];
	u8 E0000E11[SIZE_12];
	
	u8 E0000E05;
	u8 E0000E10;	
	u16 Crc;
}LoadManagementParaStruct;

//---------------- ��������Ʋ���
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
	u16 E0800100;		//������ƽ�����ֵ��
	u16 E0800101;		//������ƽ��Ȼָ���ֵ
	u16 E0800102;		//�ϸ��ѹ����
	u16 E0800103;		//�ϸ��ѹ����
	u16 E0800104;		// �жϵ�����������Զֵ�ı�����ȱʡΪ 1.3
	u16 E0800105;		//�жϸ��ɹ�����Զֵ�ı�����ȱʡΪ 1.2
	u16 E0800106;		//�жϸ��ɹ��ػָ�����Զֵ�ı�����ȱʡΪ 0.95
	u16 E0800107;		//�������������ֵ��ȱʡֵ������� 25%
	u16 E0800108;		//������������ָ���ֵ��ȱʡֵ������� 15%
	u16 E0800109;		// �жϵ��������ɻָ�����Զֵ�ı�����ȱʡֵΪ 1.0
	u16 E080010A;		// ��ѹ��ƽ�����ֵ
	u16 E080010B;			//��ѹ��ƽ��Ȼָ���ֵ
	u16 E080010C[SIZE_4];	//ʧѹ����౨�����ж���ֵ��
	
	u16 E0800200;			//��ѹ��������ֵ��ȱʡֵ 5%
	u16 E0800201;			//������������ֵ��ȱʡֵ 5%
	u16 E0800202;		//��ѹ�����ʱ����ָ���ֵȱʡֵ 3%
	u16 E0800203;			//���������ʱ����ָ���ֵȱʡֵ 3%
	
	u16 E0800205[SIZE_24];	//������������
	
	//u16 E0800204;
	u16 E0800206;		//���г������
	u16 E0800207;		//��ѹƫ������
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



//---------------- ���������ݽṹ
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
	u16 RatedVoltage;		//E0800011	���ѹ(���β�)
	
	u32 RatedCurrent;			//E0800012 	�����
	u16 UserDeclareCapacity;		//E0800013	�û��걨����(���㵽���β�)
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

#if ((Project/100)==3) //����

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

#if ((Project/100)==2) //I�ͼ�����

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

#if ((Project/100)==5) //II�ͼ�����

	#define DAILY_FREEZE_FILE_START				(0)
	#define DAILY_FREEZE_FILE_NUM				(31)
	#define DAILY_FREEZE_FILE_END				(DAILY_FREEZE_FILE_START + DAILY_FREEZE_FILE_NUM)

	#define DAILY_STATISTIC_FILE_START			(DAILY_FREEZE_FILE_END)
	#define DAILY_STATISTIC_FILE_NUM			(12)	//err20181122 Ϊ�˴洢ͣ�����ʱ�䶳������
	#define DAILY_STATISTIC_FILE_END			(DAILY_STATISTIC_FILE_START + DAILY_STATISTIC_FILE_NUM)

	#define MONTH_FREEZE_FILE_START				(DAILY_STATISTIC_FILE_END)
	#define MONTH_FREEZE_FILE_NUM				(12)	//err20181122 Ϊ�˴洢ͣ�����ʱ�䶳������
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

#if ((Project/100)==9) //���

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
}AMRFileHeadStruct;		// ��20bytes

#define ALARM_OCCUR					(0x01)
#define ALARM_RESUME				(0x00)

//---------------- �澯���ݽṹ
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
	u8 C_00010000[SIZE_4];					// 4 byte ��ǰ�����й��ܵ���, 		12, 4
	u8 C_00020000[SIZE_4];					// 4 byte ��ǰ�����й��ܵ���, 		16, 4
	u8 C_00030000[SIZE_4];					// 4 byte ��ǰ����޹� 1 �ܵ���, 	20, 4
	u8 C_00040000[SIZE_4];					// 4 byte ��ǰ����޹� 2 �ܵ���, 	24, 4
	u8 C_0201FF00[SIZE_20];			// 3*2 byte ��ѹ���ݿ飬A B C ���ѹ		28, 18
	u8 C_0202FF00[SIZE_20];			// 3*3 byte �������ݿ飬A B C �����		48, 18
	u8 C_0203FF00[SIZE_12];			// 4*3 byte �й����ʣ�A B C ���й�����		68, 12
	u8 C_0204FF00[SIZE_12];			// 4*3 byte �޹����ʣ�A B C ���޹�����		80, 12
	u8 C_0206FF00[SIZE_8];			// 4*2 byte �ܹ���������A B C �๦������	92, 8
}ARD02Struct;


typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 B_00010000[SIZE_4];					// 4 byte ��ǰ�����й��ܵ���		12, 4
	u8 B_00020000[SIZE_4];					// 4 byte ��ǰ�����й��ܵ���		16, 4
	u8 B_00030000[SIZE_4];					// 4 byte ��ǰ����޹� 1 �ܵ���		20, 4
	u8 B_00040000[SIZE_4];					// 4 byte ��ǰ����޹� 2 �ܵ���		24, 4
	u8 B_0201FF00[SIZE_20];			// 3*2 byte ��ѹ���ݿ飬A B C ���ѹ		28, 18
	u8 B_0202FF00[SIZE_20];			// 3*3 byte �������ݿ飬A B C �����		48, 18
	u8 B_0203FF00[SIZE_12];			// 4*3 byte �й����ʣ�A B C ���й�����		68, 12
	u8 B_0204FF00[SIZE_12];			// 4*3 byte �޹����ʣ�A B C ���޹�����		80, 12
	
	u8 A_00010000[SIZE_4];					// 4 byte ��ǰ�����й��ܵ���		92, 4
	u8 A_00020000[SIZE_4];					// 4 byte ��ǰ�����й��ܵ���		96, 4
	u8 A_00030000[SIZE_4];					// 4 byte ��ǰ����޹� 1 �ܵ���		100, 4
	u8 A_00040000[SIZE_4];					// 4 byte ��ǰ����޹� 2 �ܵ���		104, 4
	u8 A_0201FF00[SIZE_20];			// 3*2 byte ��ѹ���ݿ飬A B C ���ѹ		108, 18
	u8 A_0202FF00[SIZE_20];			// 3*3 byte �������ݿ飬A B C �����		128, 18
	u8 A_0203FF00[SIZE_12];			// 4*3 byte �й����ʣ�A B C ���й�����		148, 12
	u8 A_0204FF00[SIZE_12];			// 4*3 byte �޹����ʣ�A B C ���޹�����		160, 12
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
	
	u8 C_03300X00[SIZE_3];	//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯����
	u8 Reserved2;//err20180925 ���Ӷ�ť������Ǵ򿪴������ϱ��澯���� ���ݶ��� //err20190305 ������������
	
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
	
	u8 C_04000101[SIZE_4];			// ������ڼ�����							12, 4
	u8 C_04000102[SIZE_4];			// ���ʱ��: ʱ����							16, 3
	u8 C_E0000130[SIZE_8];			// �ն�ʱ��									20, 6

}ARD13Struct;

typedef struct
{
	u8 State;						// 											0,  1
	u8 ReportState;
	u16 Reserved1;
	RealTimeStruct Time;			// 											4,  6
	
	u8 C_020A0100[SIZE_2];			// A ���ѹ��г������������ 645 ���ݱ�ʶ��Ӧ	12, 2
	u8 C_020A0200[SIZE_2];			// B ���ѹ��г������������ 645 ���ݱ�ʶ��Ӧ	14, 2
	u8 C_020A0300[SIZE_2];			// C ���ѹ��г������������ 645 ���ݱ�ʶ��Ӧ	16, 2
	u8 C_020B0100[SIZE_2];			// A �������г������������ 645 ���ݱ�ʶ��Ӧ	18, 2
	u8 C_020B0200[SIZE_2];			// A �������г������������ 645 ���ݱ�ʶ��Ӧ	20, 2
	u8 C_020B0300[SIZE_2];			// A �������г������������ 645 ���ݱ�ʶ��Ӧ	22, 2

	u8 C_E1000101[SIZE_2];			// ���� A ���ѹƫ�����ֵ						24, 2
	u8 C_E1000102[SIZE_2];			// ���� A ���ѹƫ�����ֵ						26, 2
	u8 C_E1000103[SIZE_2];			// ���� A ���ѹƫ�����ֵ						28, 2
	u8 C_E1000104[SIZE_2];			// ���� Ƶ��ƫ�����ֵ							30, 2
	u8 C_E1000105[SIZE_2];			// ���յ�ѹ��ƽ������ֵ						32, 2
	u8 C_E100010D[SIZE_2];			// ��ʱ�������ֵ								34, 2
	u8 C_E100010E[SIZE_2];			// ��ʱ�������ֵ								36, 2
	u8 C_E100010F[SIZE_2];			// ���Ľ��͸���ǰ: ��ѹ�ܻ��������ֵ�����ĺ�: A ���ѹ�ܻ��������ֵ��	38, 2
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


//---------------- �¼����ݽṹ
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
#define ALAMR_FLAG_CLK_BATT_LOW_E2000032			(0x00010000)     	//ybzyl 20180627 //20181218 ���ָ���һ��ALAMR_FLAG_CLK_BATT_LOW_E2000032
#define ALAMR_FLAG_VOLTAGE_B_LOSS_E2000017			(0x00020000)		//err20181218 ����B C��ʧѹʧ����������澯
#define ALAMR_FLAG_VOLTAGE_C_LOSS_E2000018			(0x00040000)
#define ALAMR_FLAG_CURRENT_B_LOSS_E2000014			(0x00080000)
#define ALAMR_FLAG_CURRENT_C_LOSS_E2000015			(0x00100000)
#define ALAMR_FLAG_CURRENT_B_REVERSE_E200000E		(0x00200000)
#define ALAMR_FLAG_CURRENT_C_REVERSE_E200000F		(0x00400000)





#define ALAMR_FLAG_GPRS_RX_TX_BYTES_E200003A_SAVE_FLAG (0x0001) //��ͨ������Խ�� ���籣���ʶ����ֹ�ϵ��ʶ�������²����澯

#define METER_CONTROL_SWITCH_OFF					(0x01)
#define METER_CONTROL_SWITCH_ON						(0x02)
#define METER_CONTROL_POWER_HOLD					(0x03)
#define METER_CONTROL_POWER_HOLD_RELEASE			(0x04)

typedef struct
{
	u32 PowerNoneZeroStartSecond;	// ������ʷ� 0ֵ����ʼ Second
	u32 MeterControlStartSecond;	// ���������ʼ Second
	u16 MeterControlLastSecond;		// �����������Чʱ���С����
	u8  MeterControlType;		// 4������:��բ����բ�����磬������
	u8  Res1;
	u32 AlarmFlag;					// �Ѳ����ĸ澯��־λ
	u8 State[SIZE_4];				// byte0:��ʾ�����Ƿ���Ч��0����Ч��1����Ч��byte1:��ʾ�����Ƿ���£�0��δ���£�1���Ѹ���

	u8 DI_040005FF[SIZE_16];		// 16 byte ���ܱ�����״̬��  ybzyl20180627
	u8 DI_04000503[SIZE_2];			// 2 byte ���ܱ�����״̬�� 3          ybzyl20180627 err20190312 ̨��̵�����λ�澯��״̬��3�����������ݿ�ظ�D1��
	u16 Res2;						// ����
	
	//u8 DI_04000504[SIZE_2];			// 2 byte ���ܱ�����״̬�� 4          ybzyl20180627
	// ���²��ֺϼ�160�ֽ�
	u8 DI_04000101[SIZE_4];			// 4 byte ���ܱ������Լ�����
	u8 DI_04000102[SIZE_4];			// 3 byte ���ܱ�ʱ�䣬�롢�֡�ʱ
	u8 DI_03300500[SIZE_4];			// 3 byte ���ܱ�ʱ�λ���ʸ���
	u8 DI_00900200[SIZE_4];			// 4 byte ���ܱ���ǰʣ���� XXXXXX.XX(Ԫ)
	u8 DI_00010000[SIZE_4];			// 4 byte ��ǰ�����й��ܵ���
	u8 DI_00020000[SIZE_4];			// 4 byte ��ǰ�����й��ܵ���
	u8 DI_00030000[SIZE_4];			// 4 byte ��ǰ����޹� 1 �ܵ���
	u8 DI_00040000[SIZE_4];			// 4 byte ��ǰ����޹� 2 �ܵ���
	u8 DI_0201FF00[SIZE_8];			// 3*2 byte ��ѹ���ݿ飬A B C ���ѹ
	u8 DI_0202FF00[SIZE_12];		// 3*3 byte �������ݿ飬A B C �����
	u8 DI_0203FF00[SIZE_12];		// 4*3 byte �й����ʣ�A B C ���й�����
	u8 DI_0204FF00[SIZE_12];		// 4*3 byte �޹����ʣ�A B C ���޹�����
	u8 DI_0206FF00[SIZE_8];			// 4*2 byte �ܹ���������A B C �๦������

	u8 DI_03300000[SIZE_4];			// 3 byte ���ܱ����ʱ�����

	u8 DI_10010001[SIZE_4];			// A B C ��ʧѹ������ʱ��
	u8 DI_10010002[SIZE_4];
	u8 DI_10020001[SIZE_4];
	u8 DI_10020002[SIZE_4];
	u8 DI_10030001[SIZE_4];
	u8 DI_10030002[SIZE_4];

	u8 DI_18010001[SIZE_4];			// A B C ��ʧ��������ʱ��
	u8 DI_18010002[SIZE_4];
	u8 DI_18020001[SIZE_4];
	u8 DI_18020002[SIZE_4];
	u8 DI_18030001[SIZE_4];
	u8 DI_18030002[SIZE_4];

	u8 DI_1B010001[SIZE_4];			// A B C �ೱ�����������ʱ��
	u8 DI_1B010002[SIZE_4];
	u8 DI_1B020001[SIZE_4];
	u8 DI_1B020002[SIZE_4];
	u8 DI_1B030001[SIZE_4];
	u8 DI_1B030002[SIZE_4];
	
	//err20181026 ���º���񳤶�Ϊ196 ԭΪ184	//20190312 ����Ϊ200����
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
	u16 Flag;		//���ͳɹ���ʧ��
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
	u16 Type;						// �ļ����ԣ���ʾ�����ļ�������
	u16 TotalCount;					// �����ļ����ܰ���
	u32 TotalLen;					// �����ļ����ܳ���
	u8 Name[SIZE_32];				// �����ļ�������
	u16 Crc;						// �����ļ��� CRC У��
	u16 CountLen;					// �����ļ��������ݳ��ȣ��������� TotalCount �� TotalLen ����ó�
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


#define NODE_FLAG_SYNC_TO_PLC_OK			(0x80)			// �ڵ��Ƿ���ͬ���� PLC ģ�飬0:δͬ����1:��ͬ��
#define NODE_FLAG_NEED_FILE_SYNC			(0x40)			// �ڵ��Ƿ���Ҫͬ���� PLC ģ�飬0:����Ҫ��1:��Ҫ
#define NODE_FLAG_DAY_FREEZ_OK				(0x01)			// �ڵ��ն��᳭���Ƿ�ɹ���0:δ�ɹ���1:�ɹ�
#define NODE_FLAG_AMR_HOUR_OK				(0x02)			// �ڵ����㳭���Ƿ�ɹ���0:δ�ɹ���1:�ɹ�
#define NODE_FLAG_POWER_STOP				(0x04)			// �ڵ����ͣ�߸澯����


#define METER_FILE_SYNC_TO_RAM_FLAG			(0x55)
#define METER_FILE_SYNC_TO_PLC_FLAG			(0x55)
#define METER_FILE_SYNC_DELAY_TIME			(60)
typedef struct 
{
	u8 CountFlag;				// �����㵵��ͳ�Ʊ�־λ 0: ����ͳ�ƣ�0x55: ��Ҫͳ��
	u8 PLCMeterSyncFlag;		// �ز���ͬ��������ģ���ʶ,0: ����ͬ����0x55: ��Ҫͬ��
    u16 MaxMeters;				// �������������
    u16 TotalMeters;			// �ܱ���
    u16 ImportantMeters;		// �ص����
    u16 NormalMeters;			// ��ͨ����
    u16 SinglePhaseMeters;		// �������
    u16 MultifunctionMeters;	// �������
    u16 OtherMeters;			// ��������
    u16 PLCMeters;				// PLC ͨ��������
    u16 NewPLCMeters;			// ����ͬ���󣬵��������䶯ʱ���µ���Ҫ���ӵ� PLC ģ��ı�����
    u16 RS485Meters;			// RS485ͨ��������
    u16 RS485_CH1_Meters;		// RS485ͨ�� 1 ������
    u16 RS485_CH2_Meters;		// RS485ͨ�� 2 ������
    u16 RS485_CH3_Meters;		// RS485ͨ�� 3 ������
	u16 AMRDayFreezFailMeters;
	u16 AMRMonthFreezFailMeters;
    u16 PulseAndPortNum;		// �ն�����������봮�ж˿�����
    //u16 PLCNodeListCrc;
    //u16 RS485NodeListCrc;	 	//У����
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










