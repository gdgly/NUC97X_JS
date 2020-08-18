/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:CH_AMR_TaskManage.h
��  ��:zhaoyun
��  ��:1.00
��  ��:20170819
��  ��:���������������
*************************************************/
#define POWER_UP_AMR_DELAY			(10)

typedef enum
{	
	AMR_Idle = 0,
	AMR_Tx,
	AMR_WaitRx,
	AMR_Pause,
	AMR_RealTimeTx,
	AMR_RealTimeWaitRx,
	AMR_Resume,
	AMR_Done,
	
}AMR_StateEnum;



#define AMR_DAY_FREEZ_ID			(0x0001)
#define AMR_DAY_FREEZ_PATCH_ID		(0x0002)
#define AMR_HOUR_FREEZ_ID			(0x0004)
#define AMR_NORMAL_TASK_ID			(0x0008)
#define AMR_RELAY_TASK_ID			(0x0010)
#define AMR_ALARM_TASK_ID			(0x0020)
#define AMR_EVENT_TASK_ID			(0x0040)

#define MAX_AMR_TRY_CNT				(2)

#define METER_DAY_S_DI_START		(0)				//  ������������ݣ����ݱ�ʶ��ʼλ��
#define METER_DAY_S_DI_NUM			(2)				//  ������������ݣ����ݱ�ʶ����							//����� 2���ն���������
#define METER_DAY_M_DI_START		(0)				//  �๦�ܱ��������ݣ����ݱ�ʶ��ʼλ��
#define METER_DAY_M_DI_NUM			(8)				//  �๦�ܱ��������ݣ����ݱ�ʶ����						//Ŀǰ8��������
#define METER_MONTH_M_ADD			(2)				//  �๦�ܱ��¶������ݣ����ݱ�ʶ���ӵ�����					//�¶��� ���ն����2��������(����)

#define METER_HOUR_S_DI_START		(0)				//  ������������ݣ����ݱ�ʶ��ʼλ��
#define METER_HOUR_S_DI_NUM			(2)				//  ������������ݣ����ݱ�ʶ����
#define METER_HOUR_M_DI_START		(0)				//  �๦�ܱ��������ݣ����ݱ�ʶ��ʼλ��
#define METER_HOUR_M_DI_NUM			(7)				//  �๦�ܱ��������ݣ����ݱ�ʶ����

#define MAX_RT_AMR_SE_DI_NUM		(6)
#define MAX_HOUR_SE_DI_NUM			(7)
#define MAX_DAY_SE_DI_NUM			(12)
#if ((Project/100)==2)||((Project/100)==3)	
	#define MAX_ALARM_SE_DI_NUM			(12+9+1)	//err20190218 ���� 9��ʧѹʧ����������澯��1�������¼�
#endif
#if (Project == 500 || ((Project/100)==9))
	#define MAX_ALARM_SE_DI_NUM			(13+9+1)	//err20190218 ���� 9��ʧѹʧ����������澯��1�������¼�
#endif
#if (Project == 200)
	#define TEST_ALARM_SE_DI_NUM		(9+9+1)	//I�ͼ����� //err20190218 ���� 9��ʧѹʧ����������澯��1�������¼�
#endif 
#if (Project == 500 || ((Project/100)==9))
	#define TEST_ALARM_SE_DI_NUM		(10+9+1)	//err20190218 ���� 9��ʧѹʧ����������澯��1�������¼�
#endif

#define MAX_ALARM_645_DI_NUM		(36)

#define AMR_FLAG_START						(0x0001)
#define AMR_FLAG_PAUSE						(0x0002)
#define AMR_FLAG_RT_AMR_START				(0x0004)
#define AMR_FLAG_TIME_CROSS_HOUR			(0x8000)
#define AMR_MAX_645_BUF_LEN					(512)
#define AMR_PAUSE_MAX_TIME					(10)
typedef struct
{
	u32 NW_DI;
	u16 TblIndex;
	u16 DataLen;
	u16 DI645Num;
	u16 TaskIDNum;
	u16 TaskIDList[SIZE_12];
	u8 Data[AMR_MAX_645_BUF_LEN];
}NwDIAMRDataStruct;


typedef struct
{	
	u16 CurTaskID;									// ��ǰ��ͨ�����
	u16 PnNum;										// ��ǰ��ͨ�����£�Pn ����
	u16 DINum;										// ��ǰ��ͨ�����£�DI �б�����
	u16 PnIndex;									// ��ǰ��ͨ�����£������� Pn ����
	u16 PnListIndex;								// ��ǰ Pn �����£��������� PnList ��������
	u16 PnListNum;									// ��ǰ��ͨ�����£���ǰ������ Pn �������������� PnList �б��ڵ� Pn ����
	u16 DA_Pn[AMR_MAX_DA];							// ��ǰ��ͨ�����£�Pn �б�
	u16 PnList[PN_LIST_MAX_NUM];					// ��ǰ��ͨ�����£���ǰ������ Pn �������������� PnList �б�
	NwDIAMRDataStruct DataList[AMR_MAX_DATA_ID];	// ��ǰ��ͨ�����£���ǰ������ PnListIndex ������Ӧ�ĳ������ݱ�ʶ�Լ�����
}NormalTaskCtrlStruct;

typedef struct
{	
	u8 CurTaskID;
	u8 RelayTaskIndex;
	u8 RelayTasKPnIndex;
	u8 RelayType;
	
}RelayTaskCtrlStruct;


#define DI_645_FLAG_SINGLE_METER	(0x01)
#define DI_645_FLAG_MULTI_METER		(0x02)
typedef struct
{	
	u8 NW_DI_Num;
	u8 DI_645_07_Num;
	u8 DI_645_97_Num;
	u8 Res;
	u32 NW_DI[MAX_ALARM_SE_DI_NUM];						
	u32 DI_645_07[MAX_ALARM_645_DI_NUM];
	u8 DI_645_07_Flag[MAX_ALARM_645_DI_NUM];
	u32 DI_645_97[MAX_ALARM_645_DI_NUM];
	u8 DI_645_97_Flag[MAX_ALARM_645_DI_NUM];
}AlarmAMRStruct;

typedef struct
{	
	u8 Addr[METER_ADDR_SIZE];
	u16 PnNum;
	RealTimeStruct LastCheckTime;
	RealTimeStruct LastChAMRTime;
	RealTimeStruct LastDayFrozenTime;
	RealTimeStruct LastHourFrozenTime;
	
	u16 Flag;
	u8 Res;
	AMR_StateEnum OldState; 
	
	AMR_StateEnum State;
	u8 Protocol;
	u8 PMType;			// ��ʾ������๦�ܱ�
	u8 UerType;
	u8 TryCnt;
	u8 FailCnt;
	u8 PortBaudRate;
	u8 PortParity;
	u8 PortDataBit;	
	u8 PortStopBit;
	u8 DayFreezNwDINum;
	u8 HourFreezNwDINum;	
	u8 AlarmNwDINum;
	u8 EventNwDINum;	
	u8 CurNW_DI_Step;
	u8 Cur645_DI_Step;

	u16 TotalMeters;
	u16 HourFreezFailMeters;
	u16 HourFreezOkMeters;	
	u16 DayFreezFailMeters;
	u16 DayFreezOkMeters;
	u16 MonthFreezFailMeters;
	u16 MonthFreezOkMeters;
	u16 FailMeters;
	u16 SuccessMeters;
	
	u16 PLCTaskIDAckNum;
	u16 PLCTaskIDAddNum;
	u16 AMR_ID;
	u16 AMR_ID_Step;
	u16 MeterIndex;
	u16 CurNW_DI_TblIndex;		//��NW������λ�ڱ����ĸ�λ��
	u16 Cur645DataLen;	//���ݵ�Ԫ���ȣ��������ݱ�ʶ
	
	u32 AMRTotalDataLen;
	u32 Cur645_DI;		//����Ҫ����645������
	u32 CurNW_DI;		//����Ҫ����NW������

	u8 Cur645Data[AMR_MAX_645_BUF_LEN];		//���ݵ�Ԫ���������ݱ�ʶ
	AlarmAMRStruct* AlarmTask;
	NormalTaskCtrlStruct* NormalTask;
	RelayTaskCtrlStruct* RelayTask;
	u8 NormalTaskExe[MAX_NORMAL_TASK_NUM];			// ������ͨ�����ִ�п��ƣ�0����ʾ��Ӧ����ִ�У�1��ʾִ��
	u8 RelayTaskExe[MAX_RELAY_TASK_NUM];

	//u8 PLCTaskIDFlagList[SIZE_16];		//err20190103 ����PLC�����־ ǰ8�ֽڶ�Ӧ���ڳ��� ��8�ֽڶ�Ӧʵʱ���� ��Ӧλ��1��ʾ���·�δ�ظ�
}AMRCtrlStruct;

#define RT_AMR_PLC_FIRST_TIME_DELAY		(3)
#define RT_AMR_485_FIRST_TIME_DELAY		(2)
#define RT_AMR_TASK_ID_START			(0x0040)
#define RT_AMR_RS232_PORT_REQ			(0x00000001)
#define RT_AMR_GPRS_PORT_REQ			(0x00000002)
#define RT_AMR_IR_PORT_REQ				(0x00000004)
#define RT_AMR_ETHE_PORT_REQ			(0x00000008)
#define RT_AMR_GUI_PORT_REQ				(0x00000010)
#define RT_AMR_RS485_CH1_REQ			(0x00000020)
#define RT_AMR_RS485_CH2_REQ			(0x00000040)
#define RT_AMR_RS485_CH3_REQ			(0x00000080)

#define RT_AMR_CH_READY					(0x00000100)
#define RT_AMR_DATA_REFUSE				(0x00000200)
#define RT_AMR_DATA_READY				(0x00000400)
#define RT_AMR_DATA_TIME_OUT			(0x00000800)

#define RT_AMR_WAIT_REQ_TIME_OUT		(5)
#define RT_AMR_WAIT_DATA_TIME_OUT		(60)
#define RT_AMR_MAX_RETRY_CNT			(2)
typedef enum 
{
	RT_AMR_IDLE = 0, 			// ʵʱ�������״̬
	RT_AMR_SEND_REQ,			// ʵʱ���������󳭱�״̬
	RT_AMR_WAIT_REQ_RESP,		// ʵʱ����ȴ����󳭱�Ӧ��״̬
	//RT_AMR_SEND_DATA,			// ʵʱ����������״̬
	RT_AMR_WAIT_DATA_RESP,		// ʵʱ����ȴ�����������Ӧ��״̬
	RT_AMR_ACK_TO_SRC_PORT,		// ʵʱ����,���������ݷ��ص�Դ�˿�
	RT_AMR_DONE,
	
}RT_AMRState;

typedef struct
{
	u32 Flag;
	RT_AMRState State;
	TaskIDEnum SrcTaskID;
	TaskIDEnum AckTaskID;
	u8 Lock;
	
	u8 Addr[METER_ADDR_SIZE];
	u8 NW_DI_Num;
	u8 RequestCnt;
	
	u8 Protocol;
	u8 PMType;			// ��ʾ������๦�ܱ�
	u8 PortBaudRate;
	u8 PortParity;
	u8 PortDataBit;	
	u8 PortStopBit;
	u8 CurNW_DI_Step;
	u8 Cur645_DI_Step;	

	u8 RelayType;
	u8 RelayDstPort;
	u8 RelayBaudRate;
	u8 RelayParityMode;
	u8 RelayDataBits;
	u8 RelayStopBits;
	u8 RelayTimeOut;
	u8 RelayFrameLen;

	u32 Cur645_DI;
	u32 CurNW_DI;
	u16 CurNW_DI_TblIndex;
	u16 CurPn;
	u16 CurPnDataLen;
	u8 Deny_Frame;	//�Ƿ�Ϊ����֡20181218
	u8 BCAST_BaudRate_Index;	//err20190312 ���ڹ㲥Уʱ�л�����������	//u8 Res1;
	u16 PLCTaskIDAddNum;
	u16 PLCTaskIDAckNum;
	u8 RetryCnt;
	u8 NeedAck;
	u8 SrcPort;
	u8 SrcDataType;
	u8 SrcFrameType;
	u8 AckFrameType;
	u8 DstPort;
	u8 AckPort;
	QCSGFrameInfoStruct QCSGFrameInfo;

	u16 PnListNum;
	u16 PnIndex;
	u16 TimeOut;
	u16 SrcDataPos;
	u16 SrcDataLen;
	u16 AckDataLen;
	u8 SrcData[SIZE_8K];
	u8 AckData[SIZE_512];
	AMRCtrlStruct* AMRCtrl;
}RT_AMRCtrlStruct;

typedef enum 
{
	AMS_Idel = 0, 			
	AMS_Tx,			       
	AMS_WaitRx,		
	AMS_Pause,	
    AMS_Resume,
	
}AMS_StateEnum;


#define AMS_1200BPS_07_ID   (0x0001)
#define AMS_1200BPS_97_ID   (0x0002)
#define AMS_2400BPS_07_ID   (0x0004)
#define AMS_2400BPS_97_ID   (0x0008)
#define AMS_4800BPS_07_ID   (0x0010)
#define AMS_4800BPS_97_ID   (0x0020)
#define AMS_9600BPS_07_ID   (0x0040)
#define AMS_9600BPS_97_ID   (0x0080)
#define AMS_BAUDPRO_TOTAL   (0x00FF) 
#define AMS_BAUDPRO_EMPTY   (0x0000)

#define SEARCH_97_DI        (0x9010)
#define SEARCH_07_DI        (0x00010000)

#define STATE_UN_TX         (0x00)
#define STATE_NO_ACK        (0x01)
#define STATE_RIGHT_ACK     (0x02)
#define STATE_MIXED_ACK     (0x03)


#define AMS_PAUSE_MAX_TIME  (5)
#define AMS_WAITRX_MAX_TIME (5)

typedef struct
{
	AMS_StateEnum OldState;
	AMS_StateEnum State;
	u8 AckState;
	u8 Res1;

	u16 MeterIndex;
	u8 LnIndex;
	u8 CLIndex;
	u8 Addr[METER_ADDR_SIZE];
	u8 RequestCnt;
	u8 Res3;
	RealTimeStruct LastCheckTime;
	RealTimeStruct LastSearchTime;

	u16 Baud_ID;
	u16 Baud_ID_Step;
	u8 Protocol;
	u8 PMType;			// ��ʾ������๦�ܱ�
	u8 PortBaudRate;
	u8 PortParity;
	u8 PortDataBit;	
	u8 PortStopBit;
	u8 Cur645_DI_Step;	
    u8 Res4;


	u32 Cur645_DI;
	u16 CurPn;
	u16 CurPnDataLen;
	u8 RetryCnt;
	u8 NeedAck;
	u8 SrcPort;
	u8 SrcDataType;
	u8 SrcFrameType;
	u8 AckFrameType;

	AMRCtrlStruct* AMRCtrl;
}AMSCtrlStruct;


typedef struct
{	
	AMRCtrlStruct* RS485_1_AMRTask;
	AMRCtrlStruct* RS485_2_AMRTask;
	AMRCtrlStruct* RS485_3_AMRTask;
	AMRCtrlStruct* PLC_AMRTask;
	RT_AMRCtrlStruct* RT_AMRCtrl;
	RealTimeStruct LastCheckTime;
	u16 TotalMeters;
	u16 PLCMeters;
	u16 PLCSuccessMeters;
	u16 PLCFailMeters;
	u16 RS485Meters;
	u16 RS485SuccessMeters;
	u16 RS485FailMeters;
	u16 AlarmGenerateIndex;
	u8 NormalTaskCheckLock;
	u8 RelayTaskCheckLock;
	u8 AlarmTaskCheckLock;
	u8 Res;
	//u8 NormalTaskExe[MAX_NORMAL_TASK_NUM];			// ������ͨ�����ִ�п��ƣ�0����ʾ��Ӧ����ִ�У�1��ʾִ��
	//u8 RelayTaskExe[MAX_RELAY_TASK_NUM];

	AlarmAMRStruct AlarmTask;
}AMRTaskManageStruct;



#define RS485_AMR_TIME_OUT				(3)
#define PLC_AMR_RT_TIME_OUT				(60)
#if (TEST_ON == 1)
	#define PLC_AMR_SIGLE_TIME_OUT			(100)//(60) err20190305 ��ʱ������ᵼ��̨�峭��ʧ�ܸ澯��ϸ�(��ʱʱ�䳬��̨��ȴ�ʱ��)
	#define PLC_AMR_MULTI_TIME_OUT			(100)//(60)
#else
	#define PLC_AMR_SIGLE_TIME_OUT			(300)
	#define PLC_AMR_MULTI_TIME_OUT			(300)
#endif
typedef struct
{	
	PLC_STATE State;	
	u8 SEQ;
	u16 AMRTimeOut;
	u8 TerminalAddr[METER_ADDR_SIZE];
	u8 SrcAddr[METER_ADDR_SIZE];
	u8 DstAddr[METER_ADDR_SIZE];
	u16 DataLenInDataBuf;
	u16 ReportTaskID;
	u16 ReportDataLen;
	u32 UnPlugCnt;
	u32 LastRcvTicks;
	u32 Flag;
	NwDIAMRDataStruct AMR_Data[SIZE_12];
}PLCManageStruct;


void CH_AMR_TaskCtrlDataValidCheck(u32 PORTn);

void AMRTaskManage(void);


void AMRTaskManageCheckAlarmTask(AMRTaskManageStruct* AMRTask, RealTimeStruct* CurTime);

void AMRTaskManageCheckNormalTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime);

void AMRTaskManageCheckRelayTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime);

void CheckAlarmTasktoExecute(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime);

void CheckNormalTasktoExecute(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime);

void CheckRelayTasktoExecute(u32 PORTn, AMRCtrlStruct* AMRCtrl, RealTimeStruct* CurTime);

void MeterAlarmRecordGenerate(void);

void RealTimeAMRTask(void);

ResultEnum RT_AMR_TaskIdle(RT_AMRCtrlStruct* Task);

ResultEnum RT_AMR_TaskSendReq(RT_AMRCtrlStruct* Task);

ResultEnum RT_AMR_TaskWaitReqResp(RT_AMRCtrlStruct* Task);

ResultEnum RT_AMR_TaskSendData(RT_AMRCtrlStruct* Task);

ResultEnum RT_AMR_TaskWaitDataResp(RT_AMRCtrlStruct* Task);

ResultEnum RT_AMR_TaskAckToSrcPort(RT_AMRCtrlStruct* Task);

ResultEnum RT_AMR_TaskDone(RT_AMRCtrlStruct* Task);

ResultEnum Convert645ToNWAMRData(DLT645FrameStruct* Frame, u32 NW_DI_TblIndex, u32 DI_Step, u8 Protocol, u8* OutData, u16* OutLen);


ResultEnum Get_NextAMR_ID(AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextDayFreezMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextDayFreezPatchMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextHourFreezMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextNormalTaskMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextNormalTask(AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextNormalTaskPn(AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextNormalTaskPnMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextRelayTaskMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextAlarmTaskMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextAMR_NW_DI(AMRCtrlStruct* AMRCtrl);

ResultEnum Get_NextAMR_645_DI(AMRCtrlStruct* AMRCtrl);

ResultEnum AMR_SwitchToNextMeter(u32 PORTn, AMRCtrlStruct* AMRCtrl);

void SwitchMeterClearAMRInfo(AMRCtrlStruct* AMRCtrl);

ResultEnum StoreNormalTaskAMRData(u32 PORTn, AMRCtrlStruct* AMRCtrl);

ResultEnum StoreAlarmTaskAMRData_07(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

ResultEnum StoreAlarmTaskAMRData_97(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame);

//u16 FindNwDIFrom_AFN0C_0D_Tbl(u32 NwDI);

//u16 Find_AFN0D_NW_DI_Info(u32 NwDI);

//u16 FindMeterInNodeList(u8* MeterAddr, NodeListStruct* NodeList, u16 NodeListNum);

//u16 FindPortNextMeterInNodeList(u32 PORTn, NodeListStruct* NodeList, u16 StartIndex, u16 NodeListNum);

u32 Get_AMR_NormalTaskCtrlAddr(u32 PORTn);

u32 Get_AMR_RelayTaskCtrlAddr(u32 PORTn);

u32 Get_AMR_TaskCtrlAddr(u32 PORTn);

TaskIDEnum Get_AMR_TaskID(u32 PORTn);

u32 Get_AMS_TaskCtrlAddr(u32 PORTn);

TaskIDEnum Get_AMS_TaskID(u32 PORTn);

u32 Get_AMR_CH_DataBufAddr(u32 PORTn);

u32 Get_UartPortByNodePortID(u32 PortID);

ResultEnum BuildMeterAMR645FrameStruct(DLT645FrameStruct* Frame, AMRCtrlStruct* AMR);

ResultEnum DLT645FrameDcompress(DLT645FrameStruct* Frame, u8* FrameBuf, u32 FrameLen);

u32 Get645FrameDataID(DLT645FrameStruct* Frame, u8 ProtocolType);

u8 GetProtocolTypeByCtrlCode(u8 CtrlCode);

void AMR_ADDR_Display(u32 PORTn, u8* Addr);												//�����ַ��ʾ

void AMR_NUM_Display(u32 PORTn, u32 TotalMeters, u32 SuccessMeters, u32 FailMeters);		//����������ʾ

#define RT_AMR_INFO_CODE_RT			(0x00)
#define RT_AMR_INFO_CODE_RELAY		(0x01)
#define RT_AMR_INFO_CODE_CONTROL	(0x02)
#define RT_AMR_INFO_CODE_BCAST		(0x03)


void RT_AMR_Info_Display(u32 PORTn, u8* Addr, u32 Code);
void GetTerminalReadMeterFlag(void);

/**************************************** (C) COPYRIGHT zhaoyun **********************************END OF FILE****/


