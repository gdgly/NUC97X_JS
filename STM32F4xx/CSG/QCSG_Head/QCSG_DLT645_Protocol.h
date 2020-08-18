
/*************************************** (C) COPYRIGHT 2017 CHINT ***************************************
* File Name          : QCSG_DLT645_Protocol.h
* Author             : zhaoyun
* Version            : V1.0.0
* Date               : 2017/06/26
* Description        : This file Contains the DLT645_07/97 Protocol defines
********************************************************************************************************/




#ifndef __QCSG_DLT645_PROTOCOL_H__
#define __QCSG_DLT645_PROTOCOL_H__



#define MIN_645_FRAME_LEN			(12)
#define MAX_645_FRAME_LEN 			(230)
#define MAX_645_DATA_LEN 			(216)
#define ADD_FE_IN_645				(0x01)
#define NO_FE_IN_645				(0x00)
#define DLT_645_HEAD   				(0x68)
#define DLT_645_TAIL   				(0x16)
#define DLT_645_FIXED_DATA_SUM		(0x33)
#define DLT_645_FIXED_LEN			(10)
#define DLT_645_CODE_MASK			(0x1F)
#define DLT_645_RESP_BIT			(0x80)
#define DLT_645_ERROR_BIT			(0x40)
#define DLT_645_FOLLOW_BIT			(0x20)
#define DLT_645_FRAME_GAP			(3)			// Time:30ms
#define DLT_645_MAX_TX_TIME			(20)		// Time:200ms


#define DLT_645_PROTOCOL_97    		(0x00)
#define DLT_645_PROTOCOL_07   		(0x01)
#define DLT_645_PROTOCOL_NONE    	(0x02)

#define DLT_645_97_ID_SIZE			(2)
#define DLT_645_07_ID_SIZE			(4)
#define DLT_645_97_CODE_NUM			(8)
#define DLT_645_07_CODE_NUM			(8)

#define DLT_97_READ      			(0x01)
#define DLT_97_READ_LAST	 		(0x02)
#define DLT_97_WRITE	 			(0x04)
#define DLT_97_TIME_SYNC			(0x08)
#define DLT_97_WRITE_ADDR      		(0x0A)
#define DLT_97_BAUD_RATE	 		(0x0C)
#define DLT_97_PASS_WORD      		(0x0F)
#define DLT_97_MAX_NEED_CLEAR	 	(0x10)

#define DLT_07_TIME_SYNC			(0x08)
#define DLT_07_READ      			(0x11)
#define DLT_07_READ_ADDR			(0x13)
#define DLT_07_WRITE				(0x14)
#define DLT_07_WRITE_ADDR			(0x15)
#define DLT_07_PASS_WORD			(0x18)
//#define DLT_07_CLEAR_EVENT		(0x1B)
#define DLT_07_DIN_XIN_READ			(0x1D)
#define DLT_07_DONG_RUAN_READ		(0x1F)
//#define DLT_07_ZJ_EVENT_ID		(0x1E)
#define DLT_07_RESERVED				(0xFF)

//err20190531 增加EDMI红相表参数
#define MIN_EDMI_FRAME_LEN 			(0x0F)//最小长度
#define EDMI_HEAD					(0X02)//帧起始符<STX>的ASCII码为  02h
#define EDMI_TAIL   				(0x03)//帧结束符<ETX>的ASCII码为  03h
#define EDMI_FRAME_FLAG				(0x45)//标志位 ‘E’，表示可以采集一对多方式(Rs485,Rs422)；



typedef struct
{
	//u8 UsartID;	
	//u8 BaudRate;
	//u8 NeedResp;
	//u8 WaitRespTime;
	u8 Addr[METER_ADDR_SIZE];
	u8 CtrlCode;
	u8 DataLen;
	u8 Data[MAX_645_DATA_LEN];
}DLT645FrameStruct;
























#endif


