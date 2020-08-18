



#ifndef __ERAM_FOR_LIN_YUN_H__
#define __ERAM_FOR_LIN_YUN_H__


#define LIN_YUN_ERAM_START	ADDR_LIN_YUN_ERAM_START


//�����㵵��ͬ����־ 0δͬ�� 1�Ѿ�ͬ��
#define ADDR_MeasurePointSyncFlag   (LIN_YUN_ERAM_START)
//����������
#define ADDR_MeasurePointCount  (ADDR_MeasurePointSyncFlag + 4)
//�ڴ�ͬ������������
#define ADDR_MeasurePointBuff  (ADDR_MeasurePointCount + 4)
#define ADDR_RouteSyncFlag (ADDR_MeasurePointBuff + (68*1024))
/*
typedef __packed struct
{
    u8 Address[METER_ADDR_SIZE];//���
    u8 opFlag;//�����־
    u8 validFlag;//������Ч��־
    u8 readTime[7];//����ʱ��yyyymmddhhnnss
    u8 reverse1;
    u16 dataCount;
}LyAMRFileHeadInfo;

#define ADDR_AMRFILEHEADINFO ADDR_METERCOUNT + 4u
typedef __packed struct
{
    u32 dataFlag;//���ݱ�ʶ
    u8 readTime[7];//����ʱ��yyyymmddhhnnss
    u8 dataLen;//���ݳ���-���ݵ��ֽ���������ʱ������ݱ�ʶ
    u8 *dataValue;
}LyAMRFileContentInfo;
#define ADDR_AMRFILECONTENTINFO ADDR_AMRFILEHEADINFO + 24u
//�������ļ�ͷ������ͳ����Ϣ
typedef __packed struct
{
    u16 totalMeters;//�ܱ���
    u16 importantMeters;//�ص����
    u16 normalMeters;//��ͨ����
    u16 singlePhaseMeters;//�������
    u16 trianglePhaseMeters;//�������
    u16 otherMeters;//��������
}LyMeterFileHeadInfo;
#define ADDR_MeterFileHeadInfo ADDR_AMRFILECONTENTINFO + 12u
//�������ļ����� 64�ֽ�
#define ADDR_MeterFileContentInfo ADDR_MeterFileHeadInfo + 12u


typedef __packed struct
{
    u32 constType;//��������
    u32 groupId;//���ʶ
    u32 groupSeqno;//�����
    u32 groupSize;//���С
    u32 dataId;//���ݱ�ʶ
    u32 dataType;//��������
    u32 dataLen;//���ݳ���
    u32 allocLen;//ռλ����
    u32 startAddr;//��ʼ��ַ
    u32 offset;//ƫ����
    u32 storeType;//�洢����
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
    u32 dataId;			//	���ݱ�ʶ
    u32 searchId;		//	�������������ݱ�ʶ
    u16 maxLen;			//	������ݳ�������������ʶ�����ʾ������ʵ����
    u8 groupFlag;		//	�Ƿ�Ϊ���ʶ
    u8 eachDataLen;		//	����ÿ�����ݵĳ���
    u8 groupPos;		//	���ʶʱ�仯���ǵڼ����ֽ�
    //u8 SrcGroupPos;		//	���ʶʱ�仯���ǵڼ����ֽ�
    //u8 DstGroupPos;		// 	�������������ݱ�ʶ�仯���ǵڼ����ֽ�
    u8 groupMin;		//	�������ݱ�ʶ��Сֵ
    u8 groupMax;		//	�������ݱ�ʶ���ֵ
    u8 groupHeadSize;	//	��ͷ����Ϊ���ʾ��ͷ����Ҫ����������
}LyAMRFileContentDefinition;//�ն�����¶���ĳ������ݼ�ͳ�����ݶ���
//�������ݴ洢������ĳ��ȣ��ݶ�������ͺ����޸�
#define LyAMRFileContentDefinitionLen (128)
//������ͳ����Ϣ
typedef __packed struct
{
    u16 maxMeters;//�������������
    u16 totalMeters;//�ܱ���
    u16 importantMeters;//�ص����
    u16 normalMeters;//��ͨ����
    u16 singlePhaseMeters;//�������
    u16 multifunctionMeters;//�������
    u16 otherMeters;//��������
    u16 crc;//У����
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
//������ͳ�����ݵ�ȫ�ֱ���
#define ADDR_MeterCountInfo (ADDR_RouteSyncFlag + 4)


#define ALARM_FILE_MAXDEEP 			(400)
#define EVENT_FILE_MAXDEEP 			(128)
#define RELAY_TASK_FILE_MAXDEEP 	(120)
#define cmpEqual 					(0)
#define cmpLessThan 				(1)
#define cmpGreaterThan 				(2)


#define ADDR_AlarmTaskAMRDataBuf_Old	(ADDR_MeterCountInfo + 4)
#define LEN_AlarmTaskAMRDataBuf			(200*1024)//err20190312 ����Ϊ200����
#define ADDR_AlarmTaskAMRDataBuf_New	(ADDR_AlarmTaskAMRDataBuf_Old + LEN_AlarmTaskAMRDataBuf)















































#define ADDR_LIN_YUN_ERAM_END  (LIN_YUN_ERAM_START + 1024*1024)


#endif





