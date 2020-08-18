/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_DLT645_Table.h
作  者:mxn
版  本:1.00
日  期:20170501
描  述:南网数据标识与645数据标识对应表
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_PUBLICDEFINE_H
#define __QCSG_PUBLICDEFINE_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_UpLinkProtocol.h"
/* Exported define -----------------------------------------------------------*/

//CL3200台体测试宏，台体测试时打开此宏
#define FKGA43_CL3200_TEST

//CT二次开路短路告警使能宏，测试时打开此宏，实际使用时为避免误报，关闭
//#define FKGA43_TEST_Alarm_CT
#define CT_OPEN_LIMIT_VALUE 0x010	//CT二次开路 阈值0.10V	自接开路0.08V左右 台体开路0.08V
#define	CT_SHORT_LIMIT_VALUE 0x080	//CT二次短路 阈值0.80V 自接短路最高2.0V 台体短路1.35V-1.75V 

/*
 * TODO:南网协议相关定义
 */
//协议数据长度定义
#define MAX_QCSG_FRAME_LEN                  1024                    //协议包最大数据长度
#define MAX_DLT645_FRAME_LEN				220						//最大645数据包长度
//协议帧关键域定义
//#define QCSG_FRAME_HEAD                     0x68                    //协议帧头
//#define QCSG_FRAME_TAIL                     0x16                    //协议帧尾
//帧标识
#define FIR_FIN_00                          0x00                    //多帧中间帧
#define FIR_FIN_01                          0x20                    //多帧结束帧
#define FIR_FIN_10                          0x40                    //多帧起始帧
#define FIR_FIN_11                          0x60                    //单帧
//终端上行控制码定义
#define C_LINK_ACTREPORT                    0xC9
#define C_TASK_ACTREPORT                    0xCB
//测量点号相关定义
#if ((Project/100)==2)||((Project/100)==9)			// I 型集中器
	#define MAX_PN_NUM                      1024	//用户表数
	#define MAX_PN_NUM2						8		//总表数
#elif ((Project/100)==5)	// II 型集中器
	#define MAX_PN_NUM                      512	//err20180519 II型集中器支持最大测量点数
	#define MAX_PN_NUM2						8	//没用
#else
	#define MAX_PN_NUM  					16	//负控表数
	#define MAX_PN_NUM2                     16	//负控表数
#endif

#define MAX_ALARM_645_DI_NUM2				(36)	//最大告警+事件抄读645数据标识数量


#define MAX_RELAYTASK_NUM                   256
#define MAX_COMMONTASK_NUM                  32
#define P(n)                                ((u16)(((n-1)/8+1) << 8)+(1 << ((n-1)%8)))
#define P0                                  0
#define PN_MAX                              P(MAX_PN_NUM)
//测量点规约定义
#define METER_DLT645_97                     0
#define METER_DLT645_07                     1
#define METER_GD_97                         2
#define METER_GD_07                         3
#define METER_WASION                        4
#define METER_LANDISGYR_B                   5
#define METER_LANDISGYR_D                   6
#define METER_EDMI                          7
//协议错误字定义
#define QCSG_NO_ERR                         0           //无错误
#define QCSG_ERR_NO_RES                     1           //中继任务没有返回
#define QCSG_ERR_DATA_ERR                   2           //设置内容非法
#define QCSG_ERR_PWD_ERR                    3           //密码权限不足
#define QCSG_ERR_NO_FN                      4           //无此数据项
#define QCSG_ERR_REQ_TIMEOUT                5           //命令时间失效
#define QCSG_ERR_DESTADDR_ERR               6           //目标地址错误
#define QCSG_ERR_CRC_ERR                    7           //crc错误
/*
 * TODO:协议表格相关定义
 */
//数据类型定义
#define TD_NOW_TYPE                         0x01        //实时抄读类型
#define TD_C_TYPE                           0x02        //曲线冻结时标
#define TD_D_TYPE                           0x04        //日冻结时标
#define TD_M_TYPE                           0x08        //月冻结时标
//检查方法定义
#define NO_CHECK_METHOD                     0x00        //不需要检查
#define BCD_CHECK_METHOD                    0x01        //BCD格式检查
//提取方法定义
#define COMMON_TAKE_METHOD                  0x00        //通用提取办法
#define PRICE_TAKE_METHOD                   0x01        //费率数据的提取办法
#define THREE_PHASE_TAKE_METHOD1            0x02        //相序相关的提取办法1(三相：总、A、B、C，单相 A）
#define THREE_PHASE_TAKE_METHOD2            0x04        //相序相关的提取办法2(三相：A  、B、C，单相 A）
//数据转换方法定义
#define COMMON_COV_METHOD                   (0x00)     	//数据直接拷贝
#define PRICE_COV_METHOD                    (0x01)		//数据增加费率字段
#define HBYTE_ADD_YEAR_COV_METHOD           (0x02)		//高字节补充当前年，主要用于64597最大需量发生时间
#define HLBIT_ADD_ZERO_COV_METHOD           (0x03)		//高四位、低四位补0，主要用于64597协议电流数据格式转换
#define LBYTE_ADD_ZERO_COV_METHOD           (0x04)		//低字节补充0，用于64597瞬时无功功率格式转换
#define HBYTE_ADD_FF_COV_METHOD				(0x05)		
//高字节补 FF,字节数为，南网标识对应的长度 - 当前数据长度,此方法主要用于将07数据标识长度补齐到南网数据标识长度
#define TAKE_PRICE_DATA_COV_METHOD          (0x06)     	//从块数据，提取其中的费率数据
#define LBIT_ADD_ZERO_COV_METHOD			(0x07)	   	//低四位补0，主要用于97协议当前电压值数据转换
#define HBYTE_ADD_ZERO_COV_METHOD           (0x08)		//高字节补充0，用于64597编程总次数、需量清零总次数格式转换
#define TAKE_SIG_DATA_COV_METHOD          	(0x09)     	//从块数据，提取单条数据
//20180307补充
#define LBYTE_ADD_PRICE_METHOD				(0x0A)		//低字节直接增加费率数字段，用于97表最大需量块抄处理
#define DEMAND_PRICE_COV_METHOD				(0x0B)		//需量数据增加费率字段

//20190109补充
#define LBYTE_ABANDON_HBYTE_ZERO			(0x0C)	//舍弃最低的一个字节，最高字节补0，适用于剩余金额


//err20190531 EDMI规约补充
#define EDMI_IEEE754_FLOAT_METHOD			(0x0D)	//EDMI IEEE754浮点数转换算法32bit
#define EDMI_IEEE754_DOUBLE_METHOD			(0x0E)	//EDMI IEEE754浮点数转换算法64bit

#define EDMI_Q_TYPE_TIME_METHOD				(0x0F)	//3字节 Time HH/MM/SS
#define EDMI_R_TYPE_TIME_METHOD				(0x10)	//3字节 Data DD/MM/HH
#define EDMI_T_TYPE_TIME_METHOD				(0x11)	//6字节 Time/Data DD/MM/HH HH/MM/SS

#define PRICE_COV_EDMI_IEEE754_DOUBLE_METHOD (0x12)	//D数据增加费率字段
#define EDMI_REQ_BLOCK_METHOD				(0x13)	//最大需量及发生时间数据块
#define EDMI_REQ_SINGLE_METHOD				(0x14)	//最大需量及发生时间单个项
#define EDMI_VOLTAGE_BLOCK_METHOD			(0x15)	//电压数据块
#define EDMI_VOLTAGE_SINGLE_METHOD			(0x16)	//单相电压数据
#define EDMI_CURRENT_BLOCK_METHOD			(0x17)	//电流数据块
#define EDMI_CURRENT_SINGLE_METHOD			(0x18)	//单相电流数据
#define EDMI_POWER_BLOCK_METHOD				(0x19)	//功率数据块
#define EDMI_POWER_SINGLE_METHOD			(0x20)	//单相功率数据
#define EDMI_FACTOR_BLOCK_METHOD			(0x21)	//功率因数数据块
#define EDMI_FACTOR_SINGLE_METHOD			(0x22)	//总功率因数
#define EDMI_ANGLE_BLOCK_METHOD				(0x23)	//相位角数据块
#define EDMI_ANGLE_SINGLE_METHOD			(0x24)	//相位角
#define EDMI_TIME_METHOD					(0x25)	//时间
#define EDMI_DATA_METHOD					(0x26)	//日期与星期 星期填FF	//err20190713-2 南网YYMMDDWW 红相DDMMYY

#define NOPRICE_COV_EDMI_IEEE754_DOUBLE_METHOD	(0x27)	//5组D数据不增加费率字段 用于曲线和冻结
#define NOPRICE_COV_EDMI_REQ_BLOCK_METHOD		(0x28)	//5组最大需量数据及发生时间数据块不增加费率字段


#define FLOAT_TYPE 							(1)		//4字节浮点数
#define DOUBLE_TYPE 						(2)		//8字节浮点数
#define R_TYPE 								(3)		//3字节 Data DD/MM/HH
#define Q_TYPE 								(4)		//3字节 Time HH/MM/SS
#define T_TYPE 								(5)		//6字节 Time/Data DD/MM/HH HH/MM/SS





/*
 * TODO:通道相关定义
 */
//上行通道定义
#define UP_CHNO_ETH                         1           //上行以太网通道
#define UP_CHNO_4G                          2           //远程模块通道
#define UP_CHNO_IR                          3           //红外通道
#define UP_CHNO_PS2                         4           //PS2通道
#define UP_CHNO_RS485                       5           //RS485通道
//抄表通道定义
#define RT_CHNO_JC                          1           //交采
#define RT_CHNO_RS485_I                     2           //RS485I
#define RT_CHNO_RS485_II                    3           //RS485II
/*
 * TODO:程序状态定义
 */
//链路维护步骤定义
#define LOGIN_STEP                          0x01        //发送登录
#define HEARTBEAT_STEP                      0x02        //发送心跳
#define MAX_LINKERR_CNT                     0x02        //链路维护帧失败次数
/*
 * TODO:其他定义
 */
//锁定义
#define FUNC_LOCK                           0x55        //函数锁住状态
#define FUNC_UNLOCK                         0           //函数解锁状态
#define TASK_JZQ_LOCK						0xAA		//配变 集中器任务锁住状态
#define TASK_FK_LOCK						0xBB		//配变 负控任务锁住状态
#define TASK_PB_UNLOCK						0			//配变 任务解锁状态

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	DI_E0000C00 = 0,		//开关编号 0的开关接点类	型
	DI_E0000C50,			//0xE0000C50：保电状态：
	DI_E0000C2F,			//输出开关0的状态集合
	DI_E0000C3F,			//输出开关轮次
	DI_E0000C40,			//遥控拉闸时间
}QCSG_DICHANGE_E;
//时间结构体
typedef RealTimeStruct TIME_PARAM_S;
/*
typedef struct tagTimeParam
{
    u8 u8Second;
    u8 u8Minute;
    u8 u8Hour;
    u8 u8Day;
    u8 u8Month;
    u8 u8Year;
	u8 u8Week;
	u8 u8Res1;
}TIME_PARAM_S;
*/
//冻结数据时间结构体
typedef struct tagFrozenTimeParam
{
    u8 u8YearH;
    u8 u8YearL;
    u8 u8Month;
    u8 u8Day;
    u8 u8Hour;
    u8 u8Minute;	
	u8 u8Second;
	u8 u8Res1;
}FROZEN_TIME_PARAM_S;
//645数据结构体
typedef struct
{
    u8 u8RevLen;
    u8 u8SendLen;
	u8 u8Res1;
	u8 u8Res2;
    u8 u8RevBuf[MAX_DLT645_FRAME_LEN];
    u8 u8SendBuf[MAX_DLT645_FRAME_LEN];
}DLT645_FRAME_S;
typedef struct
{
    u32 u32Fn;                                          //Fn
    u8  u8AFN;                                          //对应的AFN
    u8  u8FunEnable;                                    //功能是否支持
    u16 u8Len;                                          //长度等于0xFFFF表示非定长
    u16 u16PnMin;                                       //对应Pn最小值
    u16 u16PnMax;                                       //对应Pn最大值
}QCSG_RESOLVE_S;
typedef struct
{
    u32 u32Ident;                  //数据标识
    u8  u8CheckMethod;             //对获取的数据采用什么方法校验
    u8  u8SiDataLen;               //当为块数据标识时，表示每个单元的数据长度，当为单个数据标识时，即645数据标识对应的数据长度
    u8  u8BlDataLen;               //当为块数据标识时，表示所有单元的数据总长度，当为单个数据标识时，即对应南网数据标识表示的数据长度
    u8  u8TakeMethod;              //格式转换方法
}DLT645_RESOLVE_S;

typedef struct
{
    u32 u32Ident;                  //数据标识
    //u8  u8CheckMethod;             //对获取的数据采用什么方法校验
    //u8 	DataType;					//EDMI数据对应的类型
    //u8  u8SiDataLen;               //当为块数据标识时，表示每个单元的数据长度，当为单个数据标识时，即645数据标识对应的数据长度
    //u8  u8BlDataLen;               //当为块数据标识时，表示所有单元的数据总长度，当为单个数据标识时，即对应南网数据标识表示的数据长度
    //u8  u8TakeMethod;              //格式转换方法
}EDMI_RESOLVE_S;

typedef struct
{
    u32 u32Ident;                  //数据标识
    //u8  u8TakeMethod;              //格式转换方法
}EDMI_FROZEN_RESOLVE_S;


typedef struct
{
    u32 u32Ident;                                       //数据标识
    u8  u8SiDataLen;                                    //电能表返回的单个单元数据长度
    u8  u8BlDataLen;                                    //电能表返回的总的长度,如带费率或不定长(0xFF)
}DLT645_DATALEN_S;
//南网0x0C、0x0D数据标识对应表
typedef struct
{
    u32 u32Fn;                                          //南网协议对应的Fn
    DLT645_RESOLVE_S stDLT64597Resolve[12];             //南网协议Fn对应的64597数据表格
    DLT645_RESOLVE_S stDLT64507Resolve[12];             //南网协议Fn对应的64507数据表格
    u16 u16Dlen;                                        //南网协议Fn对应的数据长度,如带费率或不定长，则是按照单默认 4 费率计算的数据长度，或默认数据长度
    u8  u8FnType;                                       //Fn的类型，如日冻结、实时数据等
}QCSG_DLT645_CON_S;

//南网0x0C、0x0D数据标识对应表 EDMI表
typedef struct
{
    u32 u32Fn;                                          //南网协议对应的Fn
    //DLT645_RESOLVE_S stDLT64597Resolve[12];             //南网协议Fn对应的64597数据表格
    EDMI_RESOLVE_S stEDMIResolve[12];             //南网协议Fn对应的64507数据表格
    u16 u16Dlen;                                        //南网协议Fn对应的数据长度,如带费率或不定长，则是按照单默认 4 费率计算的数据长度，或默认数据长度
    u16  u8FixMethod;                                       //格式转换类型//改为u16对齐
}QCSG_EDMI_CON_S;


//南网冻结数据标识对应表 EDMI表
typedef struct
{
    u32 u32Fn;                                          //南网协议对应的Fn
    EDMI_FROZEN_RESOLVE_S stEDMIFROZENResolve[12];      //南网协议Fn对应的64507数据表格
    u32  u32FixMethod;                                  //格式转换类型
}QCSG_EDMI_FROZEN_CON_S;


//南网告警数据与645数据标识对应表
typedef struct
{
    u32 u32AlarmIdent;                                  //告警数据标识
    u32 u32Dlt64597Ident[2];                            //对应的64597数据标识
    u32 u32Dlt64507Ident[6];                            //对应的64507数据标识
}QCSG_ALARM_CON_S;
//协议解析用结构体
typedef struct
{
    //接收的数据
    u16 u16RxLField;                                    //接收长度域
    u16 u16TxLField;                                    //发送长度域
	u8  u8TxCField;                                     //发送控制域
	u8  u8RxCField;                                     //接收控制域
	u8  u8RxAddrField[7];                               //接收地址域
    u8  u8TxAddrField[7];                               //发送地址域	
    u8  u8RxAFNField;                                   //接收数据AFN
    u8  u8RxSEQField;                                   //接收的SEQ域
    u8  u8TpVTransferDelayTime;                         //传输延时
    u8  u8TxAFNField;                                   //发送数据AFN
    u8  u8TxSEQField;                                   //接收的SEQ域
	u8	u8Res1;
	u8	u8Res2;
	u8	u8Res3;
    u16 u16TxLen;                                       //协议解析过程数据长度
    u16 u16SendLen;                                     //发送数据长度
    u8  u8TxBuf[MAX_QCSG_FRAME_LEN];                    //协议解析过程数据存放处
    u8  u8SendBuf[MAX_QCSG_FRAME_LEN+100];              //发送缓冲区
    //以下是组织发送的帧的缓冲区
    u16 u16RsStartPt;                                   //处理的起始指针
    u16 u16RsEndPt;                                     //处理的结束指针
    u16 u16OutPt;                                       //输出的指针
	u16 u16Res4;
}QCSG_RESOLVE_INFO_S;
//原始南网协议帧结构体
typedef struct
{
    u16 u16ReqFrameLen;                                 //提取的帧长度
    u16 u16ResFrameLen;                                 //提取的帧长度
    u8  u8ReqFrame[MAX_QCSG_FRAME_LEN];                 //提取的协议帧
    u8  u8ResFrame[MAX_QCSG_FRAME_LEN/4];               //提取的协议帧
}QCSG_REVFRAME_S;
//主动上报结构体
typedef struct
{
    u8  u8CurrConnectState;                             //当前连接状态
    u8  u8LinkProcessStep;                              //链路维护处理阶段
    u8  u8LinkErrCnt;                                   //链路维护连续失败次数
    u8  u8ReportPseq;                                   //主动上报帧序列号
    TIME_PARAM_S stLastHeartBeatTime;                   //上一次心跳时间
    QCSG_RESOLVE_INFO_S stReport_Info;                  //上报信息结构体
}QCSG_ACTIVEREPORT_S;


//实时抄读请求结构体
typedef struct
{
	TaskIDEnum SrcTask;
	u8 Lock;
	u8 FrameType;
	u8 DataType;
	u8 RelayType;
	u8 RelayDstPort;
	u8 RelayBaudRate;
	u8 RelayParityMode;
	u8 RelayDataBits;
	u8 RelayStopBits;
	u8 RelayTimeOut;
	u8 RelayFrameLen;
	u16 PnListNum;
	u16 PnListLen;
	QCSGFrameInfoStruct QCSGFrameInfo;
	u8 Data[SIZE_8K];
}QCSG_RTREADMETER_REQ_INFO_S; // RealTimeAMRReqStruct;
//实时抄读返回结构体
typedef struct
{
	u16 DataLen;
	u16 u16PnFnListNum;
	u8 FrameType;
	u8 DataType;
	u8 Res1;
	u8 Res2;
	
	QCSGFrameInfoStruct QCSGFrameInfo;	
	u8 u8PnFnData[SIZE_1K];
}QCSG_RTREADMETER_RES_INFO_S;


//抄表信息结构体
typedef struct
{
	u32 u32BpsCtrl;										//测量点波特率
    u16 u16RDMeterIndex;                                //抄表电能表指针
    u16 u16RDEventIndex;                                //事件抄读指针
    u16 u16RDIdentIndex;                                //抄表数据标识指针
    u16 u16PnNum;                                       //待抄表的总数量
    u16 u16PnList[MAX_PN_NUM2];                          //测量点列表
	u8	u8MeterId[6];									//保存当前抄表轮次的表号
	u8	u8ProtocolType;									//保存当前抄表轮次的电能表协议
	u8	u8FailTimes;									//保存当前抄读数据项的失败次数 以上52byte

	//err20190411 增加645抄读数据项去重复
	u32 ReadOK_DI_645_07_Num;							//已经抄读过的645数据标识个数
	u32 ReadOK_DI_645_07[MAX_ALARM_645_DI_NUM2];		//已经抄读过的645数据标识 4*36 = 144byte
}QCSG_RDMETER_INFO_S;
//抄表任务用到的锁及消息等信息的控制结构体

#if ((Project/100)==9)
	typedef struct 
	{
		u8 u8Lock;
		u8 u8Rdm_Sem;
		u8 u8Rd_Period;
		u8 u8PB_Task_Lock;//u8Res2;//err20181019 配变 集中器与负控代码抄表任务锁 
		u32 u32ReadPortParamTick;
		TIME_PARAM_S stSysTime_Back;

		u32 u32LockTimeOutTick;//err20190226 加锁超时定时器
	}QCSG_RDMETER_CTRL_S;
#else
	typedef struct 
	{
		u8 u8Lock;
		u8 u8Rdm_Sem;
		u8 u8Rd_Period;
		u8 u8PnNum_EDMI_IsLoad;//err20190611红相表用当前端口已登陆的表测量点号
		//u8 u8Res2;
		u32 u32ReadPortParamTick;
		
		u32 u32LockTimeOutTick;//err20190226 加锁超时定时器
		TIME_PARAM_S stSysTime_Back;
		
		
	}QCSG_RDMETER_CTRL_S;
#endif 

//中继任务抄读信息
typedef struct
{
    u8 u8RDRelayTaskIndex;                              //抄读中继任务指针
    u8 u8RelayTaskNum;                                  //中继任务数量
    u8 u8NewRelayTaskNum;                               //新读取的中继任务数量
	u8 u8Res1;
    u8 u8RelayTaskList[MAX_RELAYTASK_NUM];              //中继任务列表0,1,2,3...
    u8 u8NewRelayTaskList[MAX_RELAYTASK_NUM];           //新读取的中继任务列表
    TIME_PARAM_S stLastTaskTime[MAX_RELAYTASK_NUM];     //上一次执行过的时间
}QCSG_RDRELAYTASK_INFO_S;
//中继任务上报信息
typedef struct
{
    u8 u8ReportTaskIndex;                               //中继任务指针
    u8 u8RelayTaskNum;                                  //中继任务数量
	u8 u8Res1;
	u8 u8Res2;
    u8 u8RelayTaskList[MAX_RELAYTASK_NUM];              //中继任务列表0,1,2,3...
    TIME_PARAM_S stLastTaskTime[MAX_RELAYTASK_NUM];     //上一次上报时间
}QCSG_RELAYTASK_REPORT_INFO_S;
//普通任务抄读信息
typedef struct
{
    u8  u8PnNum;                                        //待抄表的总数量
    u8  u8RDMeterIndex;                                 //抄表电能表指针
    u8  u8RDCommonTaskIdentNum;                         //某一任务配置的具体数据标识个数
    u8  u8RDCommonTaskIdentIndex;                       //抄读的任务具体数据项指针
	u32 u8RDTaskIdentList[AMR_MAX_DATA_ID];				//配置的具体抄读任务列表
	u32 u32BpsCtrl;
    u16 u16PnList[MAX_PN_NUM2];                          //测量点列表,对于负控，最大支持8个测量点，这里为8
    u8  u8CommonTaskNum;                                //普通任务数量
    u8  u8RDCommonTaskIndex;                            //抄读的任务指针
    u8  u8NewCommonTaskNum;                             //新读取的普通任务数量
	u8	u8Res1;
	u8	u8MeterId[6];									//当前抄的表的表号
	u8	u8ProtocolType;									//当前抄的表的协议
	u8	u8MeterPort;									//当前抄的表的端口号
    u8  u8CommonTaskList[MAX_COMMONTASK_NUM];           //普通任务列表0,1,2,3...
    u8  u8NewCommonTaskList[MAX_COMMONTASK_NUM];        //新读取的普通任务列表
    TIME_PARAM_S stLastTaskTime[MAX_COMMONTASK_NUM];    //上一次执行过的时间
}QCSG_RDCOMMONTASK_INFO_S;
//普通任务上报信息
typedef struct
{
    u8  u8PnNum;                                        //单个任务配置的测量点数量
    u8  u8ReportTaskIndex;                              //上报的任务指针
	u8  u8CommonTaskNum;                                //普通任务数量
	u8	u8Res1;
    u16 u16PnList[MAX_PN_NUM2];                          //测量点列表,对于负控，最大支持8个测量点，这里为8
    u8  u8CommonTaskList[MAX_COMMONTASK_NUM];           //普通任务列表0,1,2,3...
    TIME_PARAM_S stLastTaskTime[MAX_COMMONTASK_NUM];    //记录上一次执行的时间
}QCSG_COMMONTASK_REPORT_INFO_S;
//冻结数据临时存储结构体
typedef struct
{
	u32 u32DataLen;
	u32 u32SaveFlag;
	u8 u8Data[4088];
}QCSG_FROZEN_RAM_DATA_S;
typedef struct
{
    u32 DataID;                                                                      
	u32 FlashAddr;  
	u16 DataLen;
	u8 ReadEn;
	u8 WriteEn;
}QCSG_DIDInfo;
//告警相关定义
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data00010000[4];//当前正有总
	u8 u8Data00010100[4];//当前正有 费率1
	u8 u8Data00010200[4];//当前正有 费率2
	u8 u8Data00010300[4];//当前正有 费率3
	u8 u8Data00010400[4];//当前正有 费率4
	u8 u8Data00020000[4];//当前反有总
	u8 u8Data00020100[4];//当前反有 费率1
	u8 u8Data00020200[4];//当前反有 费率2
	u8 u8Data00020300[4];//当前反有 费率3
	u8 u8Data00020400[4];//当前反有 费率4
	u8 u8Data00030000[4];//当前组合无功1总
	u8 u8Data00040000[4];//当前组合无功2总
	u8 u8Data00050000[4];//一象限无功总
	u8 u8Data00060000[4];//二象限无功总
	u8 u8Data00070000[4];//三象限无功总
	u8 u8Data00080000[4];//四象限无功总
	u8 u8Data0201FF00[6];//电压块
	u8 u8Data0202FF00[9];//电流块	
	u8 u8RdDoneFlag;
	u8 u8Data0203FF00[12];	//有功功率块
	u8 u8Data0204FF00[12];	//无功功率块
	u8 u8Data0206FF00[8];//功率因数块
	u8 u8Data0207FF00[6];//相角块
	u8 u8Res[2];
	u8 u8Data01010000[8];//(当前)正有总最大需量及时间
	u8 u8Data01020000[8];//(当前)反有总最大需量及时间
	u8 u8Data01030000[8];//(当前)组合无功1总最大需量及时间
	u8 u8Data01040000[8];//(当前)组合无功2 总最大需量及时间
	//156byte
	//err20190509 表盖端钮盖增加告警与事件数据内容填充
	//u8 u8Data03300D00[3];//开表盖次数
	//u8 u8Res2;
	//u8 u8Data03300E00[3];//端钮盖开启次数
	//u8 u8Res3;
	
}QCSG_RAM_DATA_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_RAM_DATA_S stPn_Ram_Data[MAX_PN_NUM2];	//err20190509 156*16 = 2496 ->2624
}QCSG_PN_RAM_DATA_LIST_S;
typedef struct
{
	//左模块已经上报开关数值标志
	u8 u8GetSwitchValueFlag;
	//开关数值
	u8 u8SwitchValue;	//开(1)	关(0)
	//告警产生及恢复标志
	u8 u8AlarmFlag;
	u8 u8Res1;
}QCSG_ALARM_E2000001_INFO_S;
typedef struct
{
	//开关数值
	u8 u8Switch1Value;
	u8 u8Switch2Value;
	u8 u8Switch3Value;
	u8 u8Switch4Value;
	//告警产生及恢复标志
	u8 u8Alarm1Flag;
	u8 u8Alarm2Flag;
	u8 u8Alarm3Flag;
	u8 u8Alarm4Flag;
	//左模块已经上报开关数值标志
	u8 u8GetSwitchValueFlag;	
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
}QCSG_ALARM_E2000039_INFO_S;

//err20181107 增加告警 功率超定值
typedef struct
{
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;	
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000028_INFO_S;

typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//电表运行状态字7
	u8 u8Data04000507[2];
	//电压逆相序次数
	u8 u8Data14000001[3];
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;	
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000003_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000003_INFO_S stE2000003Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000003_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000004_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000004_INFO_S stE2000004Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000004_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000005_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000005_INFO_S stE2000005Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000005_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000006_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000006_INFO_S stE2000006Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000006_LIST_S;

typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000007_0C_INFO_S;



typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200000D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200000D_INFO_S stE200000DData[MAX_PN_NUM2];
}QCSG_ALARM_E200000D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200000E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200000E_INFO_S stE200000EData[MAX_PN_NUM2];
}QCSG_ALARM_E200000E_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200000F_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200000F_INFO_S stE200000FData[MAX_PN_NUM2];
}QCSG_ALARM_E200000F_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000010_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000010_INFO_S stE2000010Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000010_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000011_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000011_INFO_S stE2000011Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000011_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000012_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000012_INFO_S stE2000012Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000012_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000013_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000013_INFO_S stE2000013Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000013_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000014_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000014_INFO_S stE2000014Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000014_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000015_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000015_INFO_S stE2000015Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000015_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000016_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000016_INFO_S stE2000016Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000016_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000017_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000017_INFO_S stE2000017Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000017_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000018_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000018_INFO_S stE2000018Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000018_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001A_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001A_INFO_S stE200001AData[MAX_PN_NUM2];
}QCSG_ALARM_E200001A_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001B_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001B_INFO_S stE200001BData[MAX_PN_NUM2];
}QCSG_ALARM_E200001B_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001C_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001C_INFO_S stE200001CData[MAX_PN_NUM2];
}QCSG_ALARM_E200001C_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001D_INFO_S stE200001DData[MAX_PN_NUM2];
}QCSG_ALARM_E200001D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001E_INFO_S stE200001EData[MAX_PN_NUM2];
}QCSG_ALARM_E200001E_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001F_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001F_INFO_S stE200001FData[MAX_PN_NUM2];
}QCSG_ALARM_E200001F_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000020_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000020_INFO_S stE2000020Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000020_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000021_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000021_INFO_S stE2000021Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000021_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000022_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000022_INFO_S stE2000022Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000022_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000023_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000023_INFO_S stE2000023Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000023_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000024_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000024_INFO_S stE2000024Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000024_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000025_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000025_INFO_S stE2000025Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000025_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000026_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000026_INFO_S stE2000026Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000026_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000027_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000027_INFO_S stE2000027Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000027_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000029_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000029_INFO_S stE2000029Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000029_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	//事件发生及恢复阶段
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//发生及恢复持续时间
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200002A_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002A_INFO_S stE200002AData[MAX_PN_NUM2];
}QCSG_ALARM_E200002A_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8Res1;
}QCSG_ALARM_E200002B_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002B_INFO_S stE200002BData[MAX_PN_NUM2];
}QCSG_ALARM_E200002B_LIST_S;
typedef struct
{
	u16 u16Pn;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8Res1;
	//首次记录的时间
	TIME_PARAM_S stStartTime;
	u8 u8LastData00010000[4];	//上一次读取的正向有功总	
	u8 u8LastData00020000[4];	//上一次读取的反向有功总
	u32 u32CalcEkwh;			//用功率计算出的电能增量
	u32 u32ReadEkwh;			//用读取到当前值及上一次值计算的电能增量
}QCSG_ALARM_E200002D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002D_INFO_S stE200002DData[MAX_PN_NUM2];
}QCSG_ALARM_E200002D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300200[3];
	u8 u8Res1;
}QCSG_ALARM_E2000031_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000031_INFO_S stE2000031Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000031_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8Res1;
}QCSG_ALARM_E2000032_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000032_INFO_S stE2000032Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000032_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300000[3];
	u8 u8Res1;
}QCSG_ALARM_E2000035_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000035_INFO_S stE2000035Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000035_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data04010001[42];//第一套第一日时段表数据
	u8 u8Data04020001[42];//err20181101 增加 第二套第一日时段表数据
	u8 u8Res1;
	u8 u8Res2;
}QCSG_ALARM_E2000036_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000036_INFO_S stE2000036Data[MAX_PN_NUM2];//48byte * Num
}QCSG_ALARM_E2000036_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data04000409[3];
	u8 u8Data0400040A[3];
	u8 u8Res1;
	u8 u8Res2;
}QCSG_ALARM_E2000037_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000037_INFO_S stE2000037Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000037_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u16 u16Ct;
	u16 u16Pt;
}QCSG_ALARM_E2000038_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000038_INFO_S stE2000038Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000038_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300E00[3];
	u8 u8Res1;
	u8 u8Res2;

//err20180925 增加数据
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
	u8 u8Data00050000[4];
	u8 u8Data00060000[4];
	u8 u8Data00070000[4];
	u8 u8Data00080000[4];
	
}QCSG_ALARM_E200004D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200004D_INFO_S stE200004DData[MAX_PN_NUM2];
}QCSG_ALARM_E200004D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300D00[3];
	u8 u8Res1;
	u8 u8Res2;

	//err20180925 增加数据
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
	u8 u8Data00050000[4];
	u8 u8Data00060000[4];
	u8 u8Data00070000[4];
	u8 u8Data00080000[4];
	
}QCSG_ALARM_E200004E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200004E_INFO_S stE200004EData[MAX_PN_NUM2];
}QCSG_ALARM_E200004E_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
}QCSG_ALARM_E200002C_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002C_INFO_S stE200002CData[MAX_PN_NUM2];
}QCSG_ALARM_E200002C_LIST_S;
typedef struct
{
	u16 u16Pn;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8Res1;
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
	u32 u32E_Quantity;
	u32 u32LastTick;
}QCSG_ALARM_E200002E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002E_INFO_S stE200002EData[MAX_PN_NUM2];
}QCSG_ALARM_E200002E_LIST_S;
typedef struct
{
	u16 u16Pn;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8FailTimes;
}QCSG_ALARM_E200002F_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002F_INFO_S stE200002FData[MAX_PN_NUM2];
}QCSG_ALARM_E200002F_LIST_S;
typedef struct
{
	u16 u16Pn;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8Res1;
	u32 u32PnrefStartEnergy;
	u32 u32PndifStartEnergy;
	u32 u32PnrefEnergy;
	u32 u32PndifEnergy;
	u32 u32PndifData00010000;
	u8  u8Data00020000[4];//err20180925 增加上次电能数据记录
}QCSG_ALARM_E2000030_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000030_INFO_S stE2000030Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000030_LIST_S;
typedef struct
{
	u16 u16Pn;
	//事件发生标识
	u8 u8AlarmFlag;
	u8 u8Res1;
	u8 u8Data04000101[4];
	u8 u8Data04000102[3];
	u8 u8Res2;
	u32 u32E_Quantity;
	u32 u32LastTick;
}QCSG_ALARM_E200003E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200003E_INFO_S stE200003EData[MAX_PN_NUM2];
}QCSG_ALARM_E200003E_LIST_S;
//事件记录相关定义
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	u8 u8Start00010000[4];
	u8 u8End00010000[4];
	u8 u8Start00020000[4];
	u8 u8End00020000[4];
	u8 u8Start00030000[4];
	u8 u8End00030000[4];
	u8 u8Start00040000[4];
	u8 u8End00040000[4];
}QCSG_EVENT_ERD3_INFO_S;

//事件记录相关定义
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	
	u8 u8Data03300400[3];
	u8 u8Res1;

}QCSG_EVENT_E2010010_INFO_S;

//事件记录相关定义
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	
	u8 u8Data03300E00[3];
	u8 u8Res1;

}QCSG_EVENT_E2010011_INFO_S;

//事件记录相关定义
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	
	u8 u8Data03300D00[3];
	u8 u8Res1;

}QCSG_EVENT_E2010012_INFO_S;

//失压记录
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010004_06_LIST_S;
//失流记录
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010007_09_LIST_S;
//断相记录
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E201000B_0D_LIST_S;

//校时记录

typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_E2010010_INFO_S stE2010010Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010010_LIST_S;


//表端钮盒开启记录
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_E2010011_INFO_S stE2010011Data[MAX_PN_NUM2];//52*16 = 832 -> 56*16
}QCSG_EVENT_E2010011_LIST_S;

//表盖开启记录
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_E2010012_INFO_S stE2010012Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010012_LIST_S;

//计量门箱开闭记录
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010015_LIST_S;


//统计相关定义
#if (DLT500_REPORT == 0)
typedef struct
{
	u16 u16Pn;
	u8 u8MonitorTime[3];
	u8 u8PassRate[3];
	u8 u8Overrrun[3];
	u8 u8PassRateTime[3];					//此域为计算方便增加，存储时不用此结构体
	u8 u8UpLimitTime[3];
	u8 u8LowLimitTime[3];
	u8 u8MaxVoltage[2];
	u8 u8MinVoltage[2];	
	u8 u8MaxVoltageTime[4];
	u8 u8MinVoltageTime[4];
}QCSG_VOLTAGE_PASSRATE_S;
#else
typedef struct
{
	u16 u16Pn;
	u8 u8MonitorTime[3];					//电压检测时间
	u8 u8PassRate[3];						//电压合格率
	u8 u8UpLimintRate[3];					//电压超上限率
	u8 u8LowLimintRate[3];                  //电压超下限率
	u8 u8PassRateTime[3];	                //电压合格率累计时间				
	u8 u8UpLimitTime[3];                    //电压超上限累计时间
	u8 u8LowLimitTime[3];                   //电压超下限累计时间
	u8 u8MaxVoltage[2];
	u8 u8MinVoltage[2];	
	u8 u8MaxVoltageTime[4];
	u8 u8MinVoltageTime[4];
}QCSG_VOLTAGE_PASSRATE_S;
#endif
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_VOLTAGE_PASSRATE_S stPassRate[MAX_PN_NUM2];
}QCSG_VOLTAGE_PASSRATE_LIST_S;
typedef struct
{
	u16 u16Pn;
	u8 u8Max_A_Current[3];
	u8 u8Min_A_Current[3];
	u8 u8Max_B_Current[3];
	u8 u8Min_B_Current[3];
	u8 u8Max_C_Current[3];
	u8 u8Min_C_Current[3];		
	u8 u8Max_A_Current_Time[4];
	u8 u8Min_A_Current_Time[4];
	u8 u8Max_B_Current_Time[4];
	u8 u8Min_B_Current_Time[4];
	u8 u8Max_C_Current_Time[4];
	u8 u8Min_C_Current_Time[4];
}QCSG_CURRENT_EXTREME_PASSRATE_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_CURRENT_EXTREME_PASSRATE_S stCurrentExtreme[MAX_PN_NUM2];
}QCSG_CURRENT_EXTREME_PASSRATE_LIST_S;
//极值统计结构体
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Max_Value[4];
	u8 u8Min_Value[4];
	u8 u8Max_Value_Time[4];
	u8 u8Min_Value_Time[4];
}QCSG_VALUE_EXTREME_PASSRATE_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_VALUE_EXTREME_PASSRATE_S stCurrentExtreme[MAX_PN_NUM2];
}QCSG_VALUE_EXTREME_PASSRATE_LIST_S;
//功率因数区段统计结构体
typedef struct
{
	u16 u16Pn;
	u8 u8MonitorTime[3];
	u8 u8Zone1Time[3];
	u8 u8Zone2Time[3];
	u8 u8Zone3Time[3];
	u8 u8Zone4Time[3];
	u8 u8Zone5Time[3];
	u8 u8Zone6Time[3];
	u8 u8Zone7Time[3];
	u8 u8Zone8Time[3];
	u8 u8Zone9Time[3];
	u8 u8Zone10Time[3];
}QCSG_POWER_FACTOR_ZONE_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_POWER_FACTOR_ZONE_S stPowerFactorZone[MAX_PN_NUM2];
}QCSG_POWER_FACTOR_ZONE_LIST_S;
//负荷控制数据结构体
typedef struct
{
	u32 u32CtrlLedTick;
	//读取控制参数计时器,基于10msTick，过程记录变量
	u32 u32ReadCtrlParamTick;
	//临时限电结束时间，基于10msTick，过程记录变量
	u32 u32TempRation_Time;	//限电时长
	//遥控结束时间
	u32 u32YK_Timeout;		//在参数0xE0000C40遥控拉闸时间的基础上加了u32CurrTick
	//滑差计数器，基于10msTick，过程记录变量
	u32 u32SlipTick;
	//功率累加技术器，基于10msTick，过程记录变量
	u32 u32SumPowerTick;
	//过负荷持续时间，基于10msTick
	u32 u32PowerOccurTimeoutTick;
	//恢复负荷持续时间，基于10msTick
	u32 u32PowerRestoreTimeoutTick;
	//告警蜂鸣器及告警继电器动作计时器，基于10msTick，过程记录变量
	u32 u32AlarmTick;
	//轮次继电器动作计时器，基于10msTick
	u32 u32CtrlOutTick;
	//告警超时定时器，基于10msTick，超时时间到，停止警告，动作继电器，过程记录变量
	u32 u32PowerAlarmTimeoutTick;
	//告警超时定时器，基于10msTick，超时时间到，停止警告，动作继电器，过程记录变量
	u32 u32EnergyAlarmTimeoutTick;
	//告警超时定时器，基于10msTick，超时时间到，停止警告，动作继电器，过程记录变量
	u32 u32YKAlarmTimeoutTick;		
	//最低负荷，协议设置变量
	u32 u32LimitPower;
	//过负荷持续时间，协议设置变量
	u32 u32PowerOccurTimeout;
	//过负荷恢复时间，协议设置变量
	u32 u32PowerRestoreTimeout;
	//告警超时，协议设置变量
	u32 u32AlarmTimeout;//0xE0000C68：拉闸命令告警延时，缺省 10 分钟
	//月电量限额，协议设置变量
	u32 u32MaxEnergy_Month;		//E0000E00月电量控制，月电量限额（一次值）
	//轮次电量，协议设置变量
	u32 u32RoundEnergy_Month;		//E0000E01月电量控制，轮次电量(上限)
	//滑差计算次数,1min一次功率计算，到15计算15min的滑差，并且此值变为0，过程记录变量
	u32 u32SlipCnt;
	//门节点属性变化后的延时定时器，为了符合台体测试流程，台体更改属性后，过30sec在判断告警条件
	u32 u32Switch0Timeout;
	//蜂鸣器持续时间
	u32 u32BuzzerTimeout;
	u32 u32BuzzerTickBack;
	//每分钟计算的功率累加值，过程记录变量
	s32	s32MinutePowerList[15];
	//滑差功率，过程记录变量
	s32 s32SlipPower;
	//时段控总加功率
	s32 s32SumPower;
	//功控总加组
	u16 u16PowerSumPnList[8];//0xE0000C69参与累加的测量点号1~8
	//电控总加组
	u16 u16EnergySumPnList[8];
	//功控参数
	KwCtrlStruct stKwCtrl[4];
	//初始上电标识，刚上电=0，保电时间过后=1，过程记录变量
	u8 u8PowerUpFlag;	
	//功控累加标志，协议设置变量
	u8 u8PowerSumFlag;
	//电控累加标志，协议设置变量
	u8 u8EnergySumFlag;
	//保电状态，协议设置变量
	u8 u8PowerHoldState;
	//功控有效标志，协议设置变量
	u8 u8PowerCtrlFlag;
	//电控有效标志，协议设置变量
	u8 u8EnergyCtrlFlag;
	//临时控制有效标志，协议设置变量
	u8 u8TempRationFlag;
	u8 u8TempRationFloatCoeff;	//临时限电指标
	//临时限电开始时间
	TIME_PARAM_S stTempRationStartTime;
	//临时限电上电标志
	u8 u8TempRationPowerUpFlag;
	//功率定值浮动系数，协议设置变量
	u8 u8PowerFloatCoeff;
	//电量定值浮动系数，协议设置变量
	u8 u8EnergyFloatCoeff;
	//厂休控有效标志，协议设置变量
	u8 u8FactoryRestFlag;
	//功率控制滑差时间，协议设置变量
	u8 u8SlipTime;
	//功控轮次有效标志，协议设置变量
	u8 u8PowerCtrl_RoundFlag;
	//电量控轮次有效标志，协议设置变量
	u8 u8EnergyCtrl_RoundFlag;	//0xE0000C64月电量控制跳闸轮次有效标志
	//遥控轮次有效标志，协议设置变量
	u8 u8YKCtrl_RoundFlag;		//主站要求要跳哪几轮
	//告警标识，过程记录变量
	u8 u8PowerAlarmFlag;	//有功控告警时，置1
	//告警标识，过程记录变量
	u8 u8EnergyAlarmFlag;
	//告警标识，过程记录变量
	u8 u8YKAlarmFlag;
	//功控继电器输出状态，根据此状态进行跳闸,过程记录变量
	u8 u8PowerCtrlSwitchState;
	//电控继电器输出状态，根据此状态进行跳闸,过程记录变量
	u8 u8EnergyCtrlSwitchState;
	//远程遥控状态，根据此状态进行跳闸，过程记录变量
	u8 u8YKCtrlSwitchState;	//开关的实际状态
	//远程遥控开关输出状态，协议配置变量
	u8 u8YKSwitchOutState;	//0xE0000C20~0xE0000C2E四路开关的拉合闸状态，将OutputSwitchState整合成按位的
	//LED灯状态，过程记录变量
	u8 u8LedState;
	u8 u8LedStateBack;
	u8 u8Switch0Attr;	//常开(0)常闭(1)
	u8 u8Switch1Attr;
	u8 u8Switch2Attr;
	u8 u8Switch3Attr;
	u8 u8Switch4Attr;	
	//遥信状态，遥信模块上报记录变量
	u8 u8YXState;
	u8 u8YXLastState;
	//遥控状态，遥控过程记录变量
	u8 u8YKState;
	//遥控状态备份，用于解除时的报警
	u8 u8YKStateBack;
	//功控阶段变量，用于过负荷持续时间辅助标志
	u8 u8PowerCtrlStep;
	//继电器输出定时器，用于周期输出脉冲
	u8 u8SwitchOutTick;
	//滑差周期到标志
	u8 u8SlipTimeoutFlag;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
}QCSG_CTRL_INFO_S;
//电能量结构体
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u32 u32Energy00010000;	//电量，而非电能，下同
	u32 u32Energy00010100;
	u32 u32Energy00010200;
	u32 u32Energy00010300;
	u32 u32Energy00010400;
	u32 u32Energy00020000;
	u32 u32Energy00020100;
	u32 u32Energy00020200;
	u32 u32Energy00020300;
	u32 u32Energy00020400;
	u32 u32Energy00030000;
	u32 u32Energy00040000;
}QCSG_ENERGY_S;
//月电量
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_MONTH_ENERGY_LIST_S;
//日电量
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_DAY_ENERGY_LIST_S;
//控制掉电需要保存的信息
typedef struct 
{
	u32 u32OverLimitEnergy;	//上一次产生告警的电能，作为下一次的限值
	u32 u32RationEnergy;		//当前电量超过轮次电量多少，即二者差值,该差值与轮次电量上限u32RoundEnergy_Month比较
	//备份的系统时间，月电控数据清零使用
	TIME_PARAM_S stSysCurrTime;
	//备份的临时限电控起始时间
	TIME_PARAM_S stTempRationStartTime;
}QCSG_CTRL_POWERDOWN_SAVE_INFO_S;
typedef struct
{
	u32 u32CurrDayStatisticsIdent;		//本日
	u32 u32TDStatisticsIdent;		//日
	u32 u32CurrMonthStatisticsIdent;	//本月
	u32 u32TMStatisticsIdent;			//月
}QCSG_STATISTICSDATA_TABLE_S;

#define LCD_RT_AMR_REQ			(0x0001)

#define LCD_RT_AMR_REQ_TEST_START	(0x1000) //err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。
#define LCD_RT_AMR_REQ_TEST_END		(0x2000) //err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。

#define LCD_RT_AMR_READY		(0x0010)
#define LCD_RT_BOTTOM_ICO		(0x0080)

typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//需要抄读的数据标识
	u32 u32ReqNwIdentNum;
	u32 u32ReqNwIdentList[8];
	//分相电流
	u8 u8Data0202FF00[12];
	//分相电压
	u8 u8Data0201FF00[6];	
	//电网频率
	u8 u8Data02800002[2];
	//总功率因数
	u8 u8Data0206FF00[8];
	//分相相位角
	u8 u8Data0207FF00[12];
	//有功功率
	u8 u8Data0203FF00[12];	
	//无功功率
	u8 u8Data0204FF00[12];	
	//当前正向有功电能示值
	u8 u8Data0001FF00[20];		
	//当反正向有功电能示值
	u8 u8Data0002FF00[20];	
	//组合无功1
	u8 u8Data0003FF00[20];	
	//组合无功2
	u8 u8Data0004FF00[20];	
	//当日正向有功电能量
	u8 u8CurrDay0001FF00[20];		
	//当日反向有功电能量
	u8 u8CurrDay0002FF00[20];			
	//当月正向有功电能量
	u8 u8CurrMonth0001FF00[20];	
	//当月反向有功电能量
	u8 u8CurrMonth0002FF00[20];		
	//当日正向无功总电能量
	u8 u8CurrDay0003FF00[4];
	//当日反向无功总电能量	
	u8 u8CurrDay0004FF00[4];
	//当月正向无功总电能量	
	u8 u8CurrMonth0003FF00[4];	 
	//当月反向无功总电能量	
	u8 u8CurrMonth0004FF00[4];		
	
	//谐波数据（实时数据）
	//A相电压总谐波含有量
	u8 u8Data020A0100[2];  
	//B相电压总谐波含有量	
	u8 u8Data020A0200[2];  
	//C相电压总谐波含有量	
	u8 u8Data020A0300[2];  
	//A相电流总谐波含有量	
	u8 u8Data020B0100[2]; 
	//B相电流总谐波含有量	
	u8 u8Data020B0200[2];   
	//C相电流总谐波含有量		
	u8 u8Data020B0300[2];    

#if (DLT500_REPORT == 1)
	//电压监测（实时数据）
	//(当日)总电压合格率统计数据
	u8 u8DataE1000010[33];   
	//(当日)A相电压合格率统计数据	
	u8 u8DataE1000011[33];  
	//(当日)B相电压合格率统计数据	
	u8 u8DataE1000012[33];        
	//(当日)C相电压合格率统计数据
	u8 u8DataE1000013[33];    
	//(当月)总电压合格率统计数据    
	u8 u8Data03100000[33];  
	//(当月)A相电压合格率统计数据	
	u8 u8Data03100100[33];  
	//(当月)B相电压合格率统计数据	
	u8 u8Data03100200[33];   
	//(当月)C相电压合格率统计数据	
	u8 u8Data03100300[33];   
#else
	//电压监测（实时数据）
	//(当日)总电压合格率统计数据
	u8 u8DataE1000010[27];   
	//(当日)A相电压合格率统计数据	
	u8 u8DataE1000011[27];  
	//(当日)B相电压合格率统计数据	
	u8 u8DataE1000012[27];        
	//(当日)C相电压合格率统计数据
	u8 u8DataE1000013[27];    
	//(当月)总电压合格率统计数据    
	u8 u8Data03100000[27];  
	//(当月)A相电压合格率统计数据	
	u8 u8Data03100100[27];  
	//(当月)B相电压合格率统计数据	
	u8 u8Data03100200[27];   
	//(当月)C相电压合格率统计数据	
	u8 u8Data03100300[27];        
#endif

	//电压不平衡度越限统计数据（实时数据）
	//(当日)极值、极值发生时间、极值累计时间
	u8 u8DataE100001C[9];    
	//(当月)极值、极值发生时间、极值累计时间	
	u8 u8DataE100401C[9];  
	
	//电流不平衡度越限统计数据（实时数据）
	//(当日)极值、极值发生时间、极值累计时间
	u8 u8DataE100001B[9];       
	//(当月)极值、极值发生时间、极值累计时间			
	u8 u8DataE100401B[9];     
    
	//功率因数区段统计数据（实时数据）	
	//(当日)区段1~区段10的累计时间
	u8 u8DataE1000033[33];     
	//(当月)区段1~区段10的累计时间		
	u8 u8DataE1004033[33];        
	//电表运行状态字
	u8 u8Data040005FF[18];
	//终端状态字
	u8 u8Terminal040005FF[18];
	u8 u8DataTerminalStateWord[2]; //终端状态字
}QCSG_LCD_RAMDATA_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_LCD_RAMDATA_S stPnLcdRamData[MAX_PN_NUM2];
}QCSG_LCD_RAMDATA_LIST_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_LCD_DAY_ENERGY_LIST_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_LCD_MONTH_ENERGY_LIST_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	u16 u16PnList[MAX_PN_NUM2];
}QCSG_FROZEN_PN_LIST_S;
typedef struct
{
	QCSG_FROZEN_PN_LIST_S st_TD_D_PnList;
	QCSG_FROZEN_PN_LIST_S st_TD_M_PnList;
}QCSG_RECORD_FROZEN_PN_LIST_S;


typedef struct {
	u32 EdmiCmd;
	//u8   EdmiMethod;
}stEdmi;


typedef struct {

  u32 NwCmd;
  stEdmi Edmi_info[8];	//8需用MAX_EDMI_INFO_NUM代替
  u32  u32FixMethod;                                       //格式转换类型
  
}stNwEdmi;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __QCSG_PUBLICDEFINE_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/

