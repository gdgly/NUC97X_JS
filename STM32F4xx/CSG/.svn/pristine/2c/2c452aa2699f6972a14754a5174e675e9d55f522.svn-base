/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN13.c
作  者:zhaoyun
版  本:1.00
日  期:20170601
描  述:南网AFN13处理文件
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../QCSG_Head/QCSG_AFN13.h"
#include "Terminal_Task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#define ARD01_DESC_NUM				(2)
#define ARD01_DATA_LEN				(7)
const DataDescriptorStruct ARD01Desc[ARD01_DESC_NUM] = 
{
	{0,  1},
	{4,  6}
};

#define ARD02_DESC_NUM				(11)
#define ARD02_DATA_LEN				(91)
const DataDescriptorStruct ARD02Desc[ARD02_DESC_NUM] = 
{
	{0,  1},
	{4,  6},
	{12, 4},
	{16, 4},
	{20, 4},
	{24, 4},
	{28, 18},
	{48, 18},
	{68, 12},
	{80, 12},
	{92, 8}
};


#define ARD03_DESC_NUM				(18)
#define ARD03_DATA_LEN				(159)
const DataDescriptorStruct ARD03Desc[ARD03_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  4},
	{20,  4},
	{24,  4},
	{28,  18},
	{48,  18},
	{68,  12},
	{80,  12},
	{92,  4},
	{96,  4},
	{100,  4},
	{104,  4},
	{108,  18},
	{128,  18},
	{148, 12},
	{160, 12}
};

#define ARD04_DESC_NUM				(7)
#define ARD04_DATA_LEN				(27)
const DataDescriptorStruct ARD04Desc[ARD04_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  4},
	{20,  4},
	{24,  4},
	{28,  4}
};


#define ARD05_DESC_NUM				(10)
#define ARD05_DATA_LEN				(31)
const DataDescriptorStruct ARD05Desc[ARD05_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  4},
	{20,  4},
	{24,  4},
	{28,  2},
	{30,  2},
	{32,  2},
	{34,  2}
};


#define ARD06_DESC_NUM				(10)
#define ARD06_DATA_LEN				(35)
const DataDescriptorStruct ARD06Desc[ARD06_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  4},
	{20,  4},
	{24,  4},
	{28,  3},
	{32,  3},
	{36,  3},
	{40,  3}
};


#define ARD07_DESC_NUM				(10)
#define ARD07_DATA_LEN				(55)
const DataDescriptorStruct ARD07Desc[ARD07_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  4},
	{20,  4},
	{24,  4},
	{28,  8},
	{36,  8},
	{44,  8},
	{52,  8}
};

#define ARD08_DESC_NUM				(4)
#define ARD08_DATA_LEN				(35)
const DataDescriptorStruct ARD08Desc[ARD08_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12, 14},
	{28, 14}
};

#define ARD09_DESC_NUM				(4)
#define ARD09_DATA_LEN				(37)
const DataDescriptorStruct ARD09Desc[ARD09_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12, 15},
	{28, 15}
};

#define ARD10_DESC_NUM				(3)
#define ARD10_DATA_LEN				(10)
const DataDescriptorStruct ARD10Desc[ARD10_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  3}
};


#define ARD11_DESC_NUM				(15)//(14)//err20181026 增加电能表端钮盒或表盖开启次数数据 改变表格 数量+1
#define ARD11_DATA_LEN				(58)//(55)//err20181026 增加电能表端钮盒或表盖开启次数数据 改变表格 长度+3
const DataDescriptorStruct ARD11Desc[ARD11_DESC_NUM] = 
{
	{0,   1},//告警状态state
	{4,   6},//告警发生时间（YYMMDDhhmmss）
	{12,  3},//现插入 电能表端钮盒或表盖开启次数
	//{12,  4},//原表格 正向有功总电能00010000H
	{16,  4},//现表格 正向有功总电能00010000H
	{20,  4},
	{24,  4},
	{28,  4},
	{32,  4},
	{36,  4},
	{40,  4},
	{44,  4},
	{48,  4},
	{52,  4},
	{56,  4},//原表格 第四象限无功总电能
	{60,  4},//现表格 第四象限无功总电能
};


#define ARD12_DESC_NUM				(3)
#define ARD12_DATA_LEN				(9)
const DataDescriptorStruct ARD12Desc[ARD12_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  2}
};


#define ARD13_DESC_NUM				(5)
#define ARD13_DATA_LEN				(20)
const DataDescriptorStruct ARD13Desc[ARD13_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  3},
	{20,  6}
};


#define ARD14_DESC_NUM				(16)
#define ARD14_DATA_LEN				(32)
const DataDescriptorStruct ARD14Desc[ARD14_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  2},
	{14,  2},
	{16,  2},
	{18,  2},
	{20,  2},
	{22,  2},
	{24,  2},
	{26,  2},
	{28,  2},
	{30,  1},
	{32,  2},
	{34,  1},
	{36,  1},
	{38,  2}
};


#define ARD15_DESC_NUM				(3)
#define ARD15_DATA_LEN				(25)
const DataDescriptorStruct ARD15Desc[ARD15_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  18}
};


#define ARD16_DESC_NUM				(3)
#define ARD16_DATA_LEN				(25)
const DataDescriptorStruct ARD16Desc[ARD16_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  18}
};


#define ARD17_DESC_NUM				(3)
#define ARD17_DATA_LEN				(25)
const DataDescriptorStruct ARD17Desc[ARD17_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  18}
};

#define ARD18_DESC_NUM				(3)
#define ARD18_DATA_LEN				(10)
const DataDescriptorStruct ARD18Desc[ARD18_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  3}
};


#define ARD19_DESC_NUM				(6)
#define ARD19_DATA_LEN				(23)
const DataDescriptorStruct ARD19Desc[ARD19_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  4},
	{16,  4},
	{20,  4},
	{24,  4}
};


#define ARD20_DESC_NUM				(5)
#define ARD20_DATA_LEN				(12)
const DataDescriptorStruct ARD20Desc[ARD20_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{2,   1},
	{12,  2},
	{14,  2}
};


#define ARD21_DESC_NUM				(3)
#define ARD21_DATA_LEN				(263)
const DataDescriptorStruct ARD21Desc[ARD21_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{12,  256},
};

#define ARD22_DESC_NUM				(4)
#define ARD22_DATA_LEN				(16)
const DataDescriptorStruct ARD22Desc[ARD22_DESC_NUM] = 
{
	{0,   1},
	{4,   6},
	{2,   1},
	{12,  8},
};

const DIDataInfoStruct AlarmDIInfoTbl[ALARM_DI_NUM] = 
{
	{0xE2000001, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000002, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000003, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000004, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000005, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000006, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000007, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000008, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000009, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200000A, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE200000B, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE200000C, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE200000D, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200000E, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200000F, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},

	{0xE2000010, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000011, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000012, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000013, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000014, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000015, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000016, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000017, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000018, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000019, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200001A, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE200001B, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE200001C, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE200001D, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200001E, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200001F, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},

	{0xE2000020, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000021, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000022, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000023, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000024, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000025, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000026, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000027, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000028, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000029, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200002A, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200002B, 0, sizeof(ARD04Struct), ARD04_DATA_LEN,  R_ENABLE, ARD04_DESC_NUM, ARD04Desc},	
	{0xE200002C, 0, sizeof(ARD03Struct), ARD03_DATA_LEN,  R_ENABLE, ARD03_DESC_NUM, ARD03Desc},
	{0xE200002D, 0, sizeof(ARD03Struct), ARD03_DATA_LEN,  R_ENABLE, ARD03_DESC_NUM, ARD03Desc},	
	{0xE200002E, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},	
	{0xE200002F, 0, sizeof(ARD01Struct), ARD01_DATA_LEN,  R_ENABLE, ARD01_DESC_NUM, ARD01Desc},
	

	{0xE2000030, 0, sizeof(ARD03Struct), ARD03_DATA_LEN,  R_ENABLE, ARD03_DESC_NUM, ARD03Desc},
	{0xE2000031, 0, sizeof(ARD07Struct), ARD07_DATA_LEN,  R_ENABLE, ARD07_DESC_NUM, ARD07Desc},
	{0xE2000032, 0, sizeof(ARD01Struct), ARD01_DATA_LEN,  R_ENABLE, ARD01_DESC_NUM, ARD01Desc},
	{0xE2000033, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000034, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000035, 0, sizeof(ARD02Struct), ARD02_DATA_LEN,  R_ENABLE, ARD02_DESC_NUM, ARD02Desc},
	{0xE2000036, 0, sizeof(ARD01Struct), ARD01_DATA_LEN,  R_ENABLE, ARD01_DESC_NUM, ARD01Desc},
	{0xE2000037, 0, sizeof(ARD06Struct), ARD06_DATA_LEN,  R_ENABLE, ARD06_DESC_NUM, ARD06Desc},
	{0xE2000038, 0, sizeof(ARD05Struct), ARD05_DATA_LEN,  R_ENABLE, ARD05_DESC_NUM, ARD05Desc},
	{0xE2000039, 0, sizeof(ARD09Struct), ARD09_DATA_LEN,  R_ENABLE, ARD09_DESC_NUM, ARD09Desc},	
	{0xE200003A, 0, sizeof(ARD10Struct), ARD10_DATA_LEN,  R_ENABLE, ARD10_DESC_NUM, ARD10Desc},
	{0xE200003B, 0, sizeof(ARD12Struct), ARD12_DATA_LEN,  R_ENABLE, ARD12_DESC_NUM, ARD12Desc},
	{0xE200003C, 0, sizeof(ARD12Struct), ARD12_DATA_LEN,  R_ENABLE, ARD12_DESC_NUM, ARD12Desc},
	{0xE200003D, 0, sizeof(ARD21Struct), ARD21_DATA_LEN,  R_ENABLE, ARD21_DESC_NUM, ARD21Desc},	
	{0xE200003E, 0, sizeof(ARD13Struct), ARD13_DATA_LEN,  R_ENABLE, ARD13_DESC_NUM, ARD13Desc},	
	{0xE200003F, 0, sizeof(ARD01Struct), ARD01_DATA_LEN,  R_ENABLE, ARD01_DESC_NUM, ARD01Desc},

	{0xE2000040, 0, sizeof(ARD15Struct), ARD15_DATA_LEN,  R_ENABLE, ARD15_DESC_NUM, ARD15Desc},
	{0xE2000041, 0, sizeof(ARD16Struct), ARD16_DATA_LEN,  R_ENABLE, ARD16_DESC_NUM, ARD16Desc},
	{0xE2000042, 0, sizeof(ARD17Struct), ARD17_DATA_LEN,  R_ENABLE, ARD17_DESC_NUM, ARD17Desc},
	{0xE2000043, 0, sizeof(ARD18Struct), ARD18_DATA_LEN,  R_ENABLE, ARD18_DESC_NUM, ARD18Desc},
	{0xE2000044, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000045, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000046, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000047, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000048, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE2000049, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},	
	{0xE200004A, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE200004B, 0, sizeof(ARD14Struct), ARD14_DATA_LEN,  R_ENABLE, ARD14_DESC_NUM, ARD14Desc},
	{0xE200004C, 0, sizeof(ARD22Struct), ARD22_DATA_LEN,  R_ENABLE, ARD22_DESC_NUM, ARD22Desc},
	{0xE200004D, 0, sizeof(ARD11Struct), ARD11_DATA_LEN,  R_ENABLE, ARD11_DESC_NUM, ARD11Desc},	
	{0xE200004E, 0, sizeof(ARD11Struct), ARD11_DATA_LEN,  R_ENABLE, ARD11_DESC_NUM, ARD11Desc}	
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

ResultEnum QCSG_AFN13_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame)
{
	QCSGFrameProcessStruct QCSGFp;
	MeasurePointStruct MeasurePoint;
	MultiFrameSendReqStruct MultiFrameSendReq;
	RealTimeStruct RealTime;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0, TotalPnDataLen = 0, LenPos = 0, TempDI = 0, DataMaxLen = 0;
	u16 i = 0, AlarmDataPos = 0, DIIndex = 0, SinglePnDataLen = 0, TmpPnListNum = 0, TotalPnListNum = 0, LastPn = QCSG_INVALID_PN;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	u8* PnListDataBuf = NULL;
	u16* PnList = (u16*)(ADDR_PnListBuf);	

	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList; //err20180519 停上电告警改用RAM中地址 防止与FLASH中不一致导致无法上报

	PnListDataBuf = (u8*)GetUart_QCSG_DataBufAddr(PORTn);
	DataMaxLen = GetUart_QCSG_DataBufLen(PORTn);
	MC(0, (u32)&QCSGFp, sizeof(QCSGFrameProcessStruct));
	QCSGFp.PORTn = PORTn;
	QCSGFp.UserDataLen = SrcFrame->DataLen;
    while(QCSGFp.SrcDataPos < QCSGFp.UserDataLen)
    {
		QCSGFp.Pn = QCSG_TakePn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 2;
		QCSGFp.CurDataID = QCSG_TakeFn(&SrcFrame->Data[QCSGFp.SrcDataPos]);
		QCSGFp.SrcDataPos += 4;
		MemCopy((u8*)&QCSGFp.StartTime, &SrcFrame->Data[QCSGFp.SrcDataPos], 6);
		QCSGFp.SrcDataPos += 6;
		MemCopy((u8*)&QCSGFp.EndTime, &SrcFrame->Data[QCSGFp.SrcDataPos], 6);
		QCSGFp.SrcDataPos += 6;
		
		if(((QCSGFp.Pn & 0xFF00) == 0) && ((QCSGFp.Pn & 0x00FF) != 0))
		{
			continue;
		}

		YYTimeToRealTime(&QCSGFp.StartTime, &RealTime);
		if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;
		YYTimeToRealTime(&QCSGFp.EndTime, &RealTime);
		if(TimeValidCheck(&RealTime) != NO_ERROR)  continue;		
		if(TimeCompare(AMR_ALARM, &QCSGFp.StartTime, &QCSGFp.EndTime) != ERROR_TIME_IS_SMALLER) continue;
		#if (TEST_ON == 1)
			QCSG_IncOneMinute(&RealTime);
			RealTimeToYYTime(&RealTime, &QCSGFp.EndTime);
		#endif	
		if(LastPn != QCSGFp.Pn)
		{
			TmpPnListNum = SplitPnToList(QCSGFp.Pn, PnList, PN_CHECK_DISABLE);
			LastPn = QCSGFp.Pn;
		}	
		
		i = 0;
		while(i < TmpPnListNum)
		{// 读告警记录，要求返回格式为: DI(4) L(2) Data
			Result = NAND_ReadMeasurePoint(PnList[i], 0xE0800000, (u8*)&MeasurePoint, &DataLen);//E0800000测量点的状态
			SinglePnDataLen = 0;
			DataLen = 0;
			if(QCSGFp.Pn == 0) //err20180519 停上电告警改用RAM中地址 防止与FLASH中不一致导致无法上报
			{
				Result = NAND_ReadAlarmRecord(NodeList[0].Addr, QCSGFp.CurDataID, &QCSGFp.StartTime, &QCSGFp.EndTime, PnData, &DataLen);
			}
			else
				Result = NAND_ReadAlarmRecord(MeasurePoint.Addr, QCSGFp.CurDataID, &QCSGFp.StartTime, &QCSGFp.EndTime, PnData, &DataLen);
			AlarmDataPos = 0;
			while(AlarmDataPos < DataLen)
			{
				TempDI = 0;
				TempDI += PnData[AlarmDataPos++];
				TempDI += (u32)PnData[AlarmDataPos++] << 8;
				TempDI += (u32)PnData[AlarmDataPos++] << 16;
				TempDI += (u32)PnData[AlarmDataPos++] << 24;

				SinglePnDataLen = 0;
				SinglePnDataLen += PnData[AlarmDataPos++];
				SinglePnDataLen += (u16)PnData[AlarmDataPos++] << 8;
				DIIndex = FindDIInAlarmDIInfoTbl(TempDI);
				if((DIIndex >= ALARM_DI_NUM) || (SinglePnDataLen != AlarmDIInfoTbl[DIIndex].StoreLen))
				{
					//AlarmDataPos += EventDIInfo[DIIndex].StoreLen;
					AlarmDataPos += SinglePnDataLen;
					continue;
				}
				PnListDataBuf[TotalPnDataLen++] = PnList[i];
				PnListDataBuf[TotalPnDataLen++] = PnList[i] >> 8;
				LenPos = TotalPnDataLen;
				TotalPnDataLen += 2;
				PnListDataBuf[TotalPnDataLen++] = TempDI;
				PnListDataBuf[TotalPnDataLen++] = TempDI >> 8;
				PnListDataBuf[TotalPnDataLen++] = TempDI >> 16;
				PnListDataBuf[TotalPnDataLen++] = TempDI >> 24;
				SinglePnDataLen = SetAlarmEventDIDataByDIInfo(&PnListDataBuf[TotalPnDataLen], &PnData[AlarmDataPos], &AlarmDIInfoTbl[DIIndex]);
				TotalPnDataLen += SinglePnDataLen;
				PnListDataBuf[LenPos] = SinglePnDataLen;
				PnListDataBuf[LenPos + 1] = SinglePnDataLen >> 8;
				AlarmDataPos += AlarmDIInfoTbl[DIIndex].StoreLen;
			}
			i++;
			TotalPnListNum++;
			if(TotalPnDataLen >= DataMaxLen) break;		
		}	
		if(TotalPnDataLen >= DataMaxLen) break;
	}

	DataMaxLen = GetUartPortTxBufLen(PORTn);
	if((TotalPnDataLen > 0) && (TotalPnDataLen <= (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum)))
	{
		DataLen = 0;
		QCSGFp.DstDataPos = 0;		
		while(DataLen < TotalPnDataLen)
		{
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];		// 获取 Pn
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			SinglePnDataLen = PnListDataBuf[DataLen + 1];						// 获取 Pn 对应的数据长度
			SinglePnDataLen <<= 8;
			SinglePnDataLen += PnListDataBuf[DataLen];
			DataLen += 2;

			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];		// 获取 DI
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			DstFrame->Data[QCSGFp.DstDataPos++] = PnListDataBuf[DataLen++];
			// 根据长度获取 Pn 对应的数据
			MR((u32)&DstFrame->Data[QCSGFp.DstDataPos], (u32)&PnListDataBuf[DataLen], SinglePnDataLen);
			QCSGFp.DstDataPos += SinglePnDataLen;
			DataLen += SinglePnDataLen;
		}	
		DstFrame->FC = SrcFrame->FC;
		DstFrame->FC |= QCSG_FC_DIR_BIT;
		DstFrame->FC &= ~QCSG_FC_PRM_BIT;
		DstFrame->FC &= ~QCSG_FC_ACD_BIT;
		DstFrame->A1 = SrcFrame->A1;
		DstFrame->A2 = SrcFrame->A2;
		DstFrame->A3 = SrcFrame->A3;
		DstFrame->AFN = SrcFrame->AFN;
		DstFrame->SEQ = SrcFrame->SEQ & (~(QCSG_SEQ_TPV_BIT + QCSG_SEQ_CON_BIT));
		DstFrame->DataLen = QCSGFp.DstDataPos;
		return NO_ERROR;
	}
	
	if(TotalPnDataLen > (DataMaxLen - QCSG_FIXED_LEN + 2*TotalPnListNum))
	{
		MC(0, (u32)&MultiFrameSendReq, sizeof(MultiFrameSendReqStruct));
		MultiFrameSendReq.SrcTask = GetUartTaskID(PORTn);
		MultiFrameSendReq.RetryCnt = 0;
		MultiFrameSendReq.FrameType = FRAME_TYPE_QCSG_UP;
		MultiFrameSendReq.DataType = DATA_TYPE_STRUCT;
		MultiFrameSendReq.AckToSrcTask = 0;
		MultiFrameSendReq.NeedAck = 0;
		
		MultiFrameSendReq.QCSGFrameInfo.A1 = SrcFrame->A1;
		MultiFrameSendReq.QCSGFrameInfo.A2 = SrcFrame->A2;
		MultiFrameSendReq.QCSGFrameInfo.A3 = SrcFrame->A3;
		MultiFrameSendReq.QCSGFrameInfo.FC = SrcFrame->FC;
		MultiFrameSendReq.QCSGFrameInfo.AFN = SrcFrame->AFN;
		MultiFrameSendReq.QCSGFrameInfo.SEQ = SrcFrame->SEQ;
		MultiFrameSendReq.QCSGFrameInfo.Port = (u8)PORTn;
		MultiFrameSendReq.WaitAckTimeOut = 0;
		MultiFrameSendReq.PnListNum = TotalPnListNum;
		MultiFrameSendReq.DataLen = TotalPnDataLen;
		MultiFrameSendReq.DataBuf = PnListDataBuf;

		Result = QCSGMultiFrameSendReq(PORTn, &MultiFrameSendReq);
		if(Result == NO_ERROR)
		{
			Result = ERROR_DO_NOT_ANSWER;
		}
		else
		{
			TotalPnDataLen = 0;
		}
	}

	if(TotalPnDataLen == 0)
	{
		DstFrame->FC = (SrcFrame->FC & 0xFFF0) | QCSG_FC_PRM0_F9; //原代码:DstFrame->FC = SrcFrame->FC; err20180228-30 AFN=13
		DstFrame->FC |= QCSG_FC_DIR_BIT;
		DstFrame->FC &= ~QCSG_FC_PRM_BIT;	
		DstFrame->FC &= ~QCSG_FC_ACD_BIT;
		DstFrame->A1 = SrcFrame->A1;
		DstFrame->A2 = SrcFrame->A2;
		DstFrame->A3 = SrcFrame->A3;
		DstFrame->AFN = 00;
		DstFrame->SEQ = SrcFrame->SEQ & (~(QCSG_SEQ_TPV_BIT + QCSG_SEQ_CON_BIT));
		QCSGFp.DstDataPos = 0;
		DstFrame->Data[QCSGFp.DstDataPos++] = 0;
		DstFrame->Data[QCSGFp.DstDataPos++] = 0;
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)QCSG_FOR_ALL_DI;
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(QCSG_FOR_ALL_DI >> 8);
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(QCSG_FOR_ALL_DI >> 16);
		DstFrame->Data[QCSGFp.DstDataPos++] = (u8)(QCSG_FOR_ALL_DI >> 24);
		DstFrame->Data[QCSGFp.DstDataPos++] = QCSG_DENY;
		DstFrame->DataLen = QCSGFp.DstDataPos;
		Result = NO_ERROR;
	}
	
	return Result;
}

u16 FindDIInAlarmDIInfoTbl(u32 DI)
{
	u16 i = 0;
	for(i = 0; i < ALARM_DI_NUM; i++)
	{
		if(DI == AlarmDIInfoTbl[i].DI) break;
	}
	return i;
}



/**************************************** (C) COPYRIGHT zhaoyun **********************************END OF FILE****/


