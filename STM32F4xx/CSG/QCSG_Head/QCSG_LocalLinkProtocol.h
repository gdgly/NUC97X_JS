
/*******************************************************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_DownLinkProtocol.h
作  者:zhaoyun
版  本:1.00
日  期:20170814
描  述:南网本地通信规约
*******************************************************************************************/
#ifndef __QCSG_LOCAL_LINK_PROTOCOL__
#define __QCSG_LOCAL_LINK_PROTOCOL__

typedef enum 
{
	LM_ERROR_TimeOut = 0x00, 	
	LM_ERROR_InvalidData,
	LM_ERROR_WrongLength,
	LM_ERROR_WrongCS,
	LM_ERROR_NoSuchDI,
	LM_ERROR_WrongFormat,
	LM_ERROR_RepeatMeterAddr,
	LM_ERROR_NoSuchMeterAddr,
	LM_ERROR_MeterNoResponse,
	LM_ERROR_MasterNodeBusy,
	LM_ERROR_MasterNotSurportCmd,
	LM_ERROR_SlaveNodeNoResponse,
	LM_ERROR_SlaveNodeFree,
	LM_ERROR_LessTaskNum,
	LM_ERROR_ReportTaskNotExsit,
	LM_ERROR_TaskIdRepeat,
	LM_ERROR_QueryTaskNotExsit,
	LM_ERROR_TaskIdNotExsit,
	LM_ERROR_UNKNOWN = 0xFF
}LM_ERROR_ENUM;

#define LOCAL_FRAME_HEAD			(0x68)
#define LOCAL_FRAME_TAIL			(0x16)

#define LOCAL_FRAME_DIR_BIT			(0x80)
#define LOCAL_FRAME_PRM_BIT			(0x40)
#define LOCAL_FRAME_ADR_BIT			(0x20)
#define LOCAL_FRAME_VER_BIT			(0x10)

#define LOCAL_FRAME_AFN_00			(0x00)
#define LOCAL_FRAME_AFN_01			(0x01)
#define LOCAL_FRAME_AFN_02			(0x02)
#define LOCAL_FRAME_AFN_03			(0x03)
#define LOCAL_FRAME_AFN_04			(0x04)
#define LOCAL_FRAME_AFN_05			(0x05)
#define LOCAL_FRAME_AFN_06			(0x06)
#define LOCAL_FRAME_AFN_07			(0x07)
#define LOCAL_FRAME_AFN_F0			(0xF0)
#define LOCAL_FRAME_FIX_LEN			(0x06)


typedef struct
{
	u16 DataLen;		// Data缓冲区数据长度，注意根据帧结构组帧时，DataLen不含 FC, SrcAddr, DstAddr, AFN, SEQ DI的长度
	u16 FC;	//控制域C
	u8 SrcAddr[METER_ADDR_SIZE];
	u8 DstAddr[METER_ADDR_SIZE];
	u8 AFN;
	u8 SEQ;
	u16 Res;
	u32 DI;
	u8 Data[QCSG_MAX_FRAME_LEN];	
}QCSGLocalFrameStruct;

typedef struct
{	
	u16 DataLen;		// Data缓冲区数据长度，注意根据帧结构组帧时，DataLen不含 FC, SrcAddr, DstAddr, AFN, SEQ 的长度
	u16 FC;
	u8 SrcAddr[METER_ADDR_SIZE];
	u8 DstAddr[METER_ADDR_SIZE];
	u8 AFN;
	u8 SEQ;
	u16 Res;
}QCSGLocalFrameInfoStruct;

typedef enum
{	
	PLC_POWER_UP_INIT = 0x01,
	PLC_RESET_INIT,
	PLC_MODLE_RECOGNIZE,
	PLC_FILE_SYNC,
	PLC_FILE_ADD,
	PLC_AMR_STATE,
	PLC_CHECK_MODLE,//容错机制-检查PLC模块
	PLC_YBWHZ_TEST,//err20190403 PLC测试用
	//PLC_AMR_IDLE,
	//PLC_AMR_IN_PROGRESS,
	//PLC_AMR_PAUSE,
	//PLC_AMR_RESUME,
	PLC_MODLE_ERROR
	
}PLC_STATE;

#define PLC_FLAG_MODLE_RESET_DONE		(0x00000001)
#define PLC_FLAG_READ_MODLE_INFO_DONE	(0x00000002)
#define PLC_FLAG_INIT_FILE_DONE			(0x00000004)
#define PLC_FLAG_INIT_TASK_DONE			(0x00000008)
#define PLC_FLAG_SET_MODLE_ADDR_DONE	(0x00000010)
#define PLC_FLAG_READ_MODLE_ADDR_DONE	(0x00000020)
#define PLC_FLAG_READ_NODE_NUM_DONE		(0x00000040)
#define PLC_FLAG_READ_NODE_INFO_DONE	(0x00000080)
#define PLC_FLAG_ADD_NODE_DONE			(0x00000100)
#define PLC_FLAG_STOP_TASK_DONE			(0x00000200)
#define PLC_FLAG_ADD_ONE_TASK_DONE		(0x00000400)
#define PLC_FLAG_START_TASK_DONE		(0x00000800)

#define PLC_FLAG_READ_UNFINISH_TASK_DONE	(0x00001000)
//#define PLC_FLAG_READ_REMAIN_TASK_DONE	(0x00002000)

#define PLC_FLAG_YBWHZ_TEST_DONE		(0x00004000)


#define PLC_WAIT_ACK_TIME_OUT			(3)
#define PLC_RECOGNIZE_TIME_OUT			(20)
//#define PLC_READ_INFO_MAX_CNT			(0x01)
#define PLC_RESET_MAX_CNT				(0x01)
#define PLC_CMD_MAX_CNT					(0x03)

#define PLC_TASK_LEVEL_MASK				(0x0F)
#define PLC_TASK_LEVEL_0				(0x00)
#define PLC_TASK_LEVEL_1				(0x01)
#define PLC_TASK_LEVEL_2				(0x02)
#define PLC_TASK_LEVEL_3				(0x03)
#define PLC_TASK_RESP_BIT				(0x80)


#define PLC_RECOGNIZE_READ_MODLE_INFO				(0)		// 模块识别:读取模块运行模式信息
#define PLC_RECOGNIZE_WAIT_READ_MODLE_INFO_ACK		(1)		// 模块识别:等待读取模块运行模式信息应答
#define PLC_RECOGNIZE_INIT_FILE						(2)		// 模块识别:初始化档案
#define PLC_RECOGNIZE_WAIT_INIT_FILE_ACK			(3)		// 模块识别:等待初始化档案应答
#define PLC_RECOGNIZE_INIT_TASK						(4)		// 模块识别:添加从节点
#define PLC_RECOGNIZE_WAIT_INIT_TASK_ACK			(5)		// 模块识别:等待添加从节点应答
#define PLC_RECOGNIZE_SET_MODLE_ADDR				(6)		// 模块识别:设置模块地址
#define PLC_RECOGNIZE_WAIT_SET_MODLE_ADDR_ACK		(7)		// 模块识别:等待设置模块地址应答
#define PLC_RECOGNIZE_READ_MODLE_ADDR				(8)		// 模块识别:初始化任务
#define PLC_RECOGNIZE_WAIT_READ_MODLE_ADDR_ACK		(9)		// 模块识别:等待初始化任务应答
#define PLC_RECOGNIZE_DONE							(10)	// 模块识别:完成，进入档案同步状态

#define PLC_FILE_SYNC_READ_NODE_NUM					(0)		// 档案同步:读取从节点数量
#define PLC_FILE_SYNC_WAIT_READ_NODE_NUM_ACK		(1)		// 档案同步:等待读取从节点数量应答
#define PLC_FILE_SYNC_INIT_FILE						(2)		// 档案同步:初始化档案
#define PLC_FILE_SYNC_WAIT_INIT_FILE_ACK			(3)		// 档案同步:等待初始化档案应答
#define PLC_FILE_SYNC_ADD_NODE						(4)		// 档案同步:添加从节点
#define PLC_FILE_SYNC_WAIT_ADD_NODE_ACK				(5)		// 档案同步:等待添加从节点应答
#define PLC_FILE_SYNC_READ_NODE_INFO				(6)		// 档案同步:读取从节点信息
#define PLC_FILE_SYNC_WAIT_READ_NODE_INFO_ACK		(7)		// 档案同步:等待读取从节点信息应答
#define PLC_FILE_SYNC_INIT_TASK						(8)		// 档案同步:初始化任务
#define PLC_FILE_SYNC_WAIT_INIT_TASK_ACK			(9)		// 档案同步:等待初始化任务应答
#define PLC_FILE_SYNC_DONE							(10)	// 档案同步:完成，进入抄表空闲状态


#define PLC_FILE_ADD_ADD_NODE						(0)		// 档案添加:添加从节点
#define PLC_FILE_ADD_WAIT_ADD_NODE_ACK				(1)		// 档案添加:等待添加从节点应答
#define PLC_FILE_ADD_DONE							(2)		// 档案添加:完成，进入抄表空闲状态

#define PLC_ADD_TASK_STOP_TASK						(0)		// 添加任务:首先停止模块中的任务
#define PLC_ADD_TASK_STOP_TASK_WAIT_ACK				(1)		// 添加任务:等待停止模块中的任务应答
#define PLC_ADD_TASK_REMAIN_TASK					(2)		// 添加任务:查询剩余可分配任务数
//#define PLC_ADD_TASK_REMAIN_TASK_WAIT_ACK			(3)		// 添加任务:返回查询剩余可分配任务数
#define PLC_ADD_TASK_INIT							(3)		// 添加任务:初始化任务
#define PLC_ADD_TASK_INIT_WAIT_ACK					(4)		// 添加任务:等待初始化任务应答
#define PLC_ADD_TASK_ADD_ONE						(5)		// 添加任务:添加单个任务
#define PLC_ADD_TASK_ADD_ONE_WAIT_ACK				(6)		// 添加任务:等待添加单个任务应答
#define PLC_ADD_TASK_START_TASK						(7)		// 添加任务:任务添加完毕后，启动任务
#define PLC_ADD_TASK_START_TASK_WAIT_ACK			(8)		// 添加任务:等待启动任务应答
#define PLC_ADD_TASK_DONE							(9)		// 添加任务:启动完成，进入等待抄表数据上报状态

//#define PLC_ADD_TASK_


#define PLC_CHECK_MODLE_TASK_NUM                    (0)		// 容错机制:检查未完成任务数
#define PLC_CHECK_MODLE_TASK_NUM_WAIT_ACK           (1)		// 容错机制:等待检查未完成任务数应答
#define PLC_CHECK_MODLE_INIT_TASK					(2)		// 容错机制:初始化任务
#define PLC_CHECK_MODLE_INIT_WAIT_ACK				(3)		// 容错机制:等待初始化任务应答


#define PLC_YBWHZ_TEST_NUM1                   		 (0)	// err20190403 PLC测试
#define PLC_YBWHZ_TEST_NUM1_WAIT_ACK	  		 	 (1)	// 
#define PLC_YBWHZ_TEST_NUM2							 (2)	// 
#define PLC_YBWHZ_TEST_NUM2_WAIT_ACK				 (3)	// 



#endif

