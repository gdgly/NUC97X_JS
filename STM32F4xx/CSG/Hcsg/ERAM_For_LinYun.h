



#ifndef __ERAM_FOR_LIN_YUN_H__
#define __ERAM_FOR_LIN_YUN_H__


#define LIN_YUN_ERAM_START	ADDR_LIN_YUN_ERAM_START


//测量点档案同步标志 0未同步 1已经同步
#define ADDR_MeasurePointSyncFlag   (LIN_YUN_ERAM_START)
//测量点数量
#define ADDR_MeasurePointCount  (ADDR_MeasurePointSyncFlag + 4)
//内存同步测量点数据
#define ADDR_MeasurePointBuff  (ADDR_MeasurePointCount + 4)
#define ADDR_RouteSyncFlag (ADDR_MeasurePointBuff + (68*1024))
/*
typedef __packed struct
{
    u8 Address[METER_ADDR_SIZE];//表号
    u8 opFlag;//抄表标志
    u8 validFlag;//数据有效标志
    u8 readTime[7];//抄读时间yyyymmddhhnnss
    u8 reverse1;
    u16 dataCount;
}LyAMRFileHeadInfo;

#define ADDR_AMRFILEHEADINFO ADDR_METERCOUNT + 4u
typedef __packed struct
{
    u32 dataFlag;//数据标识
    u8 readTime[7];//抄读时间yyyymmddhhnnss
    u8 dataLen;//数据长度-数据的字节数不包含时间和数据标识
    u8 *dataValue;
}LyAMRFileContentInfo;
#define ADDR_AMRFILECONTENTINFO ADDR_AMRFILEHEADINFO + 24u
//测量点文件头，包括统计信息
typedef __packed struct
{
    u16 totalMeters;//总表数
    u16 importantMeters;//重点表数
    u16 normalMeters;//普通表数
    u16 singlePhaseMeters;//单相表数
    u16 trianglePhaseMeters;//三相表数
    u16 otherMeters;//其他表数
}LyMeterFileHeadInfo;
#define ADDR_MeterFileHeadInfo ADDR_AMRFILECONTENTINFO + 12u
//测量点文件内容 64字节
#define ADDR_MeterFileContentInfo ADDR_MeterFileHeadInfo + 12u


typedef __packed struct
{
    u32 constType;//数据类型
    u32 groupId;//组标识
    u32 groupSeqno;//组序号
    u32 groupSize;//组大小
    u32 dataId;//数据标识
    u32 dataType;//数据类型
    u32 dataLen;//数据长度
    u32 allocLen;//占位长度
    u32 startAddr;//起始地址
    u32 offset;//偏移量
    u32 storeType;//存储类型
}LyConstDefinitionInfo;
*/
typedef __packed struct
{
    u32 AMRDataType;
    u32 FileNameStart;
	u32 FileNameEnd;
    u32 FileCount;
    
}LyAMRFileNameRangeInfo;
#define LyAMRFileNameRangeInfoLen 9
typedef __packed struct
{
    u32 dataId;			//	数据标识
    u32 searchId;		//	用来搜索的数据标识
    u16 maxLen;			//	最大数据长度如果不是组标识，则表示数据真实长度
    u8 groupFlag;		//	是否为组标识
    u8 eachDataLen;		//	组内每个数据的长度
    u8 groupPos;		//	组标识时变化的是第几个字节
    //u8 SrcGroupPos;		//	组标识时变化的是第几个字节
    //u8 DstGroupPos;		// 	用于搜索的数据标识变化的是第几个字节
    u8 groupMin;		//	组内数据标识最小值
    u8 groupMax;		//	组内数据标识最大值
    u8 groupHeadSize;	//	组头长度为零表示组头不需要给长度数据
}LyAMRFileContentDefinition;//日冻结和月冻结的抄表数据及统计数据定义
//抄表数据存储定义表格的长度，暂定。表格定型后再修改
#define LyAMRFileContentDefinitionLen (128)
//测量点统计信息
typedef __packed struct
{
    u16 maxMeters;//测量点最大数量
    u16 totalMeters;//总表数
    u16 importantMeters;//重点表数
    u16 normalMeters;//普通表数
    u16 singlePhaseMeters;//单相表数
    u16 multifunctionMeters;//三相表数
    u16 otherMeters;//其他表数
    u16 crc;//校验码
}LyMeterCountInfo;
typedef __packed struct
{
    u32 pnNum;
    u8 State;
	u8 MeasureType;
	u8 Addr[6];
	
	u8 ProtocolType;
	u8 PointMeterType;
	u8 TotalType;
	u8 PriorityType;
	
	u8 SwitchOff;
	u8 MaxRateNum;
	u8 CollectorAddr[6];
	
	u8 PortID;
	u8 PortBaudRate;
	u8 PortParity;
	u8 PortDataBit;
	
	u8 PortStopBit;
	u8 Reserved1;
	u8 Reserved2;
	u8 Reserved3;
	
	u16 CT;
	u16 PT;
	
	u8 ConnectionType;
	u8 Reserved4;
	u16 RatedVoltage;
	
	u32 RatedCurrent;
	u16 UserDeclareCapacity;
	u16 TransformerCapacity;
	
	u8 PassWord[8];
	u8 UserName[8];
	
	u16 Reserved5;
	u16 Crc;
} LyMeasurePointStruct;
//测量点统计数据的全局变量
#define ADDR_MeterCountInfo (ADDR_RouteSyncFlag + 4)


#define ALARM_FILE_MAXDEEP 			(400)
#define EVENT_FILE_MAXDEEP 			(128)
#define RELAY_TASK_FILE_MAXDEEP 	(120)
#define cmpEqual 					(0)
#define cmpLessThan 				(1)
#define cmpGreaterThan 				(2)


#define ADDR_AlarmTaskAMRDataBuf_Old	(ADDR_MeterCountInfo + 4)
#define LEN_AlarmTaskAMRDataBuf			(200*1024)//err20190312 更改为200长度
#define ADDR_AlarmTaskAMRDataBuf_New	(ADDR_AlarmTaskAMRDataBuf_Old + LEN_AlarmTaskAMRDataBuf)















































#define ADDR_LIN_YUN_ERAM_END  (LIN_YUN_ERAM_START + 1024*1024)


#endif





