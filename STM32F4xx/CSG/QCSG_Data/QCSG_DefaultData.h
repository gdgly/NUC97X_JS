
/************************* (C) COPYRIGHT 2017 CHINT ***************************
* File Name          : QCSG_DefaultData.h
* Author             : zhaoyun
* Version            : V1.0.0
* Date               : 2017/06/26
* Description        : This file Contains the Terminal Deault Data 
*******************************************************************************/

#ifndef __QCSG_DefaultData_H__
#define __QCSG_DefaultData_H__

#define BAUD_RATE_300  		(300)
#define BAUD_RATE_600  		(600)
#define BAUD_RATE_1200  	(1200)
#define BAUD_RATE_2400  	(2400)
#define BAUD_RATE_4800  	(4800)
#define BAUD_RATE_7200  	(7200)
#define BAUD_RATE_9600  	(9600)
#define BAUD_RATE_19200  	(19200)
#define BAUD_RATE_38400  	(38400)
#define BAUD_RATE_57600  	(57600)
#define BAUD_RATE_115200  	(115200)
#define BAUD_RATE_230400  	(230400)

#define BAUD_RATE_300_ID  		(0)
#define BAUD_RATE_600_ID  		(1)
#define BAUD_RATE_1200_ID  		(2)
#define BAUD_RATE_2400_ID  		(3)
#define BAUD_RATE_4800_ID  		(4)
#define BAUD_RATE_7200_ID  		(5)
#define BAUD_RATE_9600_ID  		(6)
#define BAUD_RATE_19200_ID  	(7)
#define BAUD_RATE_38400_ID  	(8)
#define BAUD_RATE_57600_ID  	(9)
#define BAUD_RATE_115200_ID  	(10)
#define BAUD_RATE_230400_ID  	(11)

extern const CommunicationParaStruct DefaultCommuniactionPara;

extern const AlarmEventMaskParaStruct DefaultAlarmEventMaskPara;

extern const SettleDateParaStruct DefaultSettleDatePara; 

extern const IDAuthenticationParaStruct DefaultIDAuthPara;

extern const ThresholdParaStruct DeaultThresholdPara;

extern const u32 UartBaudRate[MAX_BAUD_NUM];
extern const u16 UartBPS[MAX_BAUD_NUM];

extern const RS485PortParaStruct DefaultRS485Port1Para;

extern const RS485PortParaStruct DefaultRS485Port2Para;

extern const RS485PortParaStruct DefaultRS485Port3Para;

extern const AttachmentParaStruct DefaultAttachPara;

extern const VersionParaStruct DefaultVersionPara;

extern const LoadManagementParaStruct DefaultLoadManagementPara;

extern const MeterPointCtrlParaStruct DefaultMeterPointCtrlPara;

extern const SelfAdaptionParaStruct DefaultSelfAdaptionPara;

extern const E000011x_CommunicationParaStruct Default_E0000111_CommunicationParaStruct;

extern const E000011x_CommunicationParaStruct Default_E0000112_CommunicationParaStruct;

extern const E000011x_CommunicationParaStruct Default_E0000113_CommunicationParaStruct;

extern const u8 	GeneralAddr[METER_ADDR_SIZE];

extern const u8 	YaoXinAddr[METER_ADDR_SIZE];

extern const u8 	BroadCastAddr[METER_ADDR_SIZE];

extern const u8 	EmptyAddr[METER_ADDR_SIZE];

extern const u8 	ZeroAddr[METER_ADDR_SIZE];

extern const u8		DLT645CtrlCode97[DLT_645_97_CODE_NUM];

extern const u8    	DLT645CtrlCode07[DLT_645_07_CODE_NUM];

extern const u8 	HexToChar[16];
#endif
