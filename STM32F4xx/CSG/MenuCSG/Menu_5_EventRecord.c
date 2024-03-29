/*************************************************
版权:正泰仪表智能终端部
文件:Menu_5_EventRecord.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单5事件记录处理文件
*************************************************/


//菜单5 事件记录
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu5.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"


const u8 pMenu_5[]=//负控终端事件记录
{
	"\n\r"
	"需量事件\n\r"
	"编程事件\n\r"
	"失压事件\n\r"
	"失流事件\n\r"
	"断相事件\n\r"
	"掉电/上电事件\n\r"
	"清零事件\n\r"
	"校时事件\n\r"
	"表端钮盒开启事件\n\r"
	"表盖开启事件\n\r"
	"箱门开闭事件\n\r"
	"消息认证错误事件\n\r"
};

const u8 pMenu_5_3[]=//
{
	"\n\r"	
	"A相失压事件\n\r"
	"B相失压事件\n\r"
	"C相失压事件\n\r"
};


const u8 pMenu_5_4[]=//
{
	"\n\r"	
	"A相失流事件\n\r"
	"B相失流事件\n\r"
	"C相失流事件\n\r"
};


const u8 pMenu_5_5[]=//
{
	"\n\r"	
	"A相断相事件\n\r"
	"B相断相事件\n\r"
	"C相断相事件\n\r"
};


//-------------------------------------------------------------------------------------------------
// Function name:       DispEventDataInfo(AlarmEventFileHeadStruct* AlarmHead, u8* AlarmData, u32 DataLen)
// Descriptions:        显示事件记录信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DispEventDataInfo(AlarmEventFileHeadStruct* EventHead, u8* EventData, u32 DataLen)
{	
	u8 *p8,*p;
	u64 x = 0;
	StrBuff_TypeDef* DispBuf = StrBuff;	
	
	p8 = (u8*)&DispBuf->Row[2].Col[5];                              //地址
	MR((u32)&x, (u32)EventHead->MeterAddr, METER_ADDR_SIZE);
	DisplayData(x,12,0,(u16*)p8);		

	
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+15*2);                          //发生时间	 秒							
	DisplayData(EventData[0],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+12*2);                          //分
	DisplayData(EventData[1],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+9*2);                           //时
	DisplayData(EventData[2],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+6*2);                           //日
	DisplayData(EventData[3],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+3*2);	                          //月
	DisplayData(EventData[4],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+0*2);                           //年
	DisplayData(EventData[5],2,0,(u16*)p8);
	
	switch(EventHead->DataID)
	{
		case 0xE2010003:
			DisplayString(3,5,0,"需量清零事件");		
			DisplayString(4,9,0,"0xE2010003");
			break;
		case 0xE2010001:
			DisplayString(3,5,0,"编程事件");
			DisplayString(4,9,0,"E2010001");
			break;
		case 0xE2010004:
			DisplayString(3,5,0,"A相失压事件");
			DisplayString(4,9,0,"E2010004");
		
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010005:
			DisplayString(3,5,0,"B相失压事件");
			DisplayString(4,9,0,"E2010005");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010006:
			DisplayString(3,5,0,"C相失压事件");
			DisplayString(4,9,0,"E2010006");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010007:
			DisplayString(3,5,0,"A相失流事件");
			DisplayString(4,9,0,"E2010007");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010008:
			DisplayString(3,5,0,"B相失流事件");
			DisplayString(4,9,0,"E2010008");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010009:
			DisplayString(3,5,0,"C相失流事件");
			DisplayString(4,9,0,"E2010009");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000B:
			DisplayString(3,5,0,"A相断相事件");
			DisplayString(4,9,0,"E201000B");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000C:
			DisplayString(3,5,0,"B相断相事件");
			DisplayString(4,9,0,"E201000C");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000D:
			DisplayString(3,5,0,"C相断相事件");
			DisplayString(4,9,0,"E201000D");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000A:
			DisplayString(3,5,0,"掉电/上电事件");
			DisplayString(4,9,0,"E201000A");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //上电时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000F:
			DisplayString(3,5,0,"电量清零事件");
			DisplayString(4,9,0,"E201000F");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010010:
			DisplayString(3,5,0,"校时事件");
			DisplayString(4,9,0,"E2010010");
			break;							
		case 0xE2010011:
			DisplayString(3,5,0,"表端钮盒开启事件");
			DisplayString(4,9,0,"E2010011");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010012:
			DisplayString(3,5,0,"表盖开启事件");
			DisplayString(4,9,0,"E2010012");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010015:
			DisplayString(3,5,0,"计量门箱开闭事件");
			DisplayString(4,9,0,"E2010015");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //结束时间	 秒							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //分
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //时
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //日
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //月
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //年
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010013:
			DisplayString(3,5,0,"消息认证错误事件");
			DisplayString(4,9,0,"E2010013");
			break;
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       EventRecordDealFunction(u32 DataID)
// Descriptions:        事件记录处理函数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void EventRecordDealFunction(u32 DataID)
{	
	u8 *p8;
	u8 ReadData[SIZE_512];
	u32 EventNum = 0, DataLen = 0;
	AlarmEventFileHeadStruct EventHead;	
	StrBuff_TypeDef* DispBuf = StrBuff;	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{	
		case 0://任务0:初始化
			DisplayClr();//清全屏	
			MenuCount->v1=0;
			MenuCount->v2=0;		
			switch(DataID)
			{
				case 0xE2010003://需量清零事件
					EventNum = NAND_GetEventRecordNum(0xE2010003);					
					break;
				case 0xE2010001://编程事件
					EventNum = NAND_GetEventRecordNum(0xE2010001);
					break;
				case 0xE2010004://A相失压事件
					EventNum = NAND_GetEventRecordNum(0xE2010004);
					break;
				case 0xE2010005://B相失压事件
					EventNum = NAND_GetEventRecordNum(0xE2010005);
					break;
				case 0xE2010006://C相失压事件
					EventNum = NAND_GetEventRecordNum(0xE2010006);
					break;
				case 0xE2010007://A相失流事件
					EventNum = NAND_GetEventRecordNum(0xE2010007);
					break;
				case 0xE2010008://B相失流事件
					EventNum = NAND_GetEventRecordNum(0xE2010008);
					break;
				case 0xE2010009://C相失流事件
					EventNum = NAND_GetEventRecordNum(0xE2010009);
					break;
				case 0xE201000B://A相断相事件
					EventNum = NAND_GetEventRecordNum(0xE201000B);
					break;
				case 0xE201000C://B相断相事件
					EventNum = NAND_GetEventRecordNum(0xE201000C);
					break;
				case 0xE201000D://C相断相事件
					EventNum = NAND_GetEventRecordNum(0xE201000D);
					break;
				case 0xE201000A://终端掉/上电事件
					EventNum = NAND_GetEventRecordNum(0xE201000A);
					break;
				case 0xE201000F://电量清零事件
					EventNum = NAND_GetEventRecordNum(0xE201000F);
					break;
				case 0xE2010010://校时事件
					EventNum = NAND_GetEventRecordNum(0xE2010010);
					break;							
				case 0xE2010011://表端钮盒开启事件
					EventNum = NAND_GetEventRecordNum(0xE2010011);
					break;
				case 0xE2010012://表盖开启事件
					EventNum = NAND_GetEventRecordNum(0xE2010012);
					break;
				case 0xE2010015://计量门箱开闭事件
					EventNum = NAND_GetEventRecordNum(0xE2010015);
					break;
				case 0xE2010013://消息认证错误事件
					EventNum = NAND_GetEventRecordNum(0xE2010013);
					break;
			}		
			
			if(EventNum > 10) EventNum = 10;		
		  if(EventNum == 0)
			{
				switch(DataID)
				{
					case 0xE2010003://需量清零事件
						DisplayString(4,1,0,"无需量清零事件记录");				
						break;
					case 0xE2010001://编程事件
						DisplayString(4,3,0,"无编程事件记录");	
						break;
					case 0xE2010004://A相失压事件
						DisplayString(4,1,0,"无A相失压事件记录");	
						break;
					case 0xE2010005://B相失压事件
						DisplayString(4,1,0,"无B相失压事件记录");	
						break;
					case 0xE2010006://C相失压事件
						DisplayString(4,1,0,"无C相失压事件记录");	
						break;
					case 0xE2010007://A相失流事件
						DisplayString(4,1,0,"无A相失流事件记录");	
						break;
					case 0xE2010008://B相失流事件
						DisplayString(4,1,0,"无B相失流事件记录");	
						break;
					case 0xE2010009://C相失流事件
						DisplayString(4,1,0,"无C相失流事件记录");	
						break;
					case 0xE201000B://A相断相事件
						DisplayString(4,1,0,"无A相断相事件记录");	
						break;
					case 0xE201000C://B相断相事件
						DisplayString(4,1,0,"无B相断相事件记录");	
						break;
					case 0xE201000D://C相断相事件
						DisplayString(4,1,0,"无C相断相事件记录");	
						break;
					case 0xE201000A://终端掉/上电事件
						DisplayString(4,0,0,"无终端掉电/上电事件");	
						break;
					case 0xE201000F://电量清零事件
						DisplayString(4,1,0,"无电量清零事件记录");	
						break;
					case 0xE2010010://校时事件
						DisplayString(4,3,0,"无校时事件记录");	
						break;							
					case 0xE2010011://表端钮盒开启事件
						DisplayString(4,1,0,"无表端钮盒开启事件");	
						break;
					case 0xE2010012://表盖开启事件
						DisplayString(4,1,0,"无表盖开启事件记录");	
						break;
					case 0xE2010015://计量门箱开闭事件
						DisplayString(4,1,0,"无计量门箱开闭事件");	
						break;
					case 0xE2010013://消息认证错误事件
						DisplayString(4,1,0,"无消息认证错误事件");	
						break;
				}
				Stack->MenuStack[MenuCount->Count].Task = 2;
			}
			else
			{			
				MenuCount->v2 = EventNum;
				MenuCount->v1 = EventNum;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			break;
		case 1:
			DisplayClr();//清全屏	
			DisplayString(1,0,0,"第   条/总   条事件 "); 
			DisplayString(2,0,0,"地址:            \n\r"); 
			DisplayString(3,0,0,"名称:            \n\r");
			DisplayString(4,0,0,"事件编码:        \n\r");
			DisplayString(6,0,0,"  -  -     :  :  ");		
			switch(DataID)
			{
				case 0xE2010003://需量清零事件
					DisplayString(5,0,0,"清零时间:        \n\r"); 		 								
					break;
				case 0xE2010001://编程事件
					DisplayString(5,0,0,"编程时间:        \n\r"); 	
					break;
				case 0xE2010004://A相失压事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010005://B相失压事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE2010006://C相失压事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010007://A相失流事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010008://B相失流事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE2010009://C相失流事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000B://A相断相事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000C://B相断相事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000D://C相断相事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE201000A://终端掉/上电事件
					DisplayString(5,0,0,"掉电时间:        \n\r"); 
					DisplayString(7,0,0,"上电时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000F://电量清零事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE2010010://校时事件
					DisplayString(5,0,0,"校时发生时间:        \n\r"); 		
					break;							
				case 0xE2010011://表端钮盒开启事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010012://表盖开启事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");				
					break;
				case 0xE2010015://计量门箱开闭事件
					DisplayString(5,0,0,"开始时间:        \n\r"); 
					DisplayString(7,0,0,"结束时间:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");				
					break;
				case 0xE2010013://消息认证错误事件
					DisplayString(5,0,0,"发生时间:        \n\r"); 
					break;
			}

			EventNum = MenuCount->v1;
			p8 = (u8*)&DispBuf->Row[1].Col[2];
			bcd_p8(p8, EventNum, 3);

			if(MenuCount->v2 > 10)
			{
				MenuCount->v2 = 10;
			}
			EventNum = MenuCount->v2;
			p8 = (u8*)&DispBuf->Row[1].Col[10];
			bcd_p8(p8, EventNum, 3);
			
			switch(DataID)
			{
				case 0xE2010003://需量清零事件
					NAND_ReadEventRecordByLatelyNum(0xE2010003, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);
					break;
				case 0xE2010001://编程事件
					NAND_ReadEventRecordByLatelyNum(0xE2010001, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);
					break;
				case 0xE2010004://A相失压事件
					NAND_ReadEventRecordByLatelyNum(0xE2010004, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010005://B相失压事件
					NAND_ReadEventRecordByLatelyNum(0xE2010005, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010006://C相失压事件
					NAND_ReadEventRecordByLatelyNum(0xE2010006, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010007://A相失流事件
					NAND_ReadEventRecordByLatelyNum(0xE2010007, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);				
					break;
				case 0xE2010008://B相失流事件
					NAND_ReadEventRecordByLatelyNum(0xE2010008, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010009://C相失流事件
					NAND_ReadEventRecordByLatelyNum(0xE2010009, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);	
					break;
				case 0xE201000B://A相断相事件
					NAND_ReadEventRecordByLatelyNum(0xE201000B, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);		
					break;
				case 0xE201000C://B相断相事件
					NAND_ReadEventRecordByLatelyNum(0xE201000C, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);		
					break;
				case 0xE201000D://C相断相事件
					NAND_ReadEventRecordByLatelyNum(0xE201000D, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE201000A://终端掉电/上电事件
					NAND_ReadEventRecordByLatelyNum(0xE201000A, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);		
					break;
				case 0xE201000F://电量清零事件
					NAND_ReadEventRecordByLatelyNum(0xE201000F, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);					
					break;
				case 0xE2010010://校时事件
					NAND_ReadEventRecordByLatelyNum(0xE2010010, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen); 		
					break;							
				case 0xE2010011://表端钮盒开启事件
					NAND_ReadEventRecordByLatelyNum(0xE2010011, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010012://表盖开启事件
					NAND_ReadEventRecordByLatelyNum(0xE2010012, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);	
					break;
				case 0xE2010015://计量门箱开闭事件
					NAND_ReadEventRecordByLatelyNum(0xE2010015, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);	
					break;
				case 0xE2010013://消息认证错误事件
					NAND_ReadEventRecordByLatelyNum(0xE2010013, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
			}			
			DispEventDataInfo(&EventHead, ReadData, DataLen);	 									
			
			Stack->MenuStack[MenuCount->Count].Task = 2;
			Stack->MenuStack[MenuCount->Count].EditRow = 1;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,4,2);
			break;
		case 2://任务1:选择
				if(Stack->MenuStack[MenuCount->Count].EditRow == 1)
				{
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{// 右键
						Comm_Ram->ButtonVal &= ~0x08;
						EventNum = MenuCount->v1;			
						if(EventNum < MenuCount->v2)
						{
							MenuCount->v1++;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
						if((EventNum != 0)&&(EventNum == MenuCount->v2))
						{
							MenuCount->v1 = 1;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
					}

					if((Comm_Ram->ButtonVal&0x04)!=0)
					{// 左键
						Comm_Ram->ButtonVal &= ~0x04;
						EventNum = MenuCount->v1;			
						if(EventNum > 1)
						{
							MenuCount->v1--;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
						if(EventNum == 1)
						{
							MenuCount->v1 = MenuCount->v2;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}		
					}				
				}		
//			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_1
// Descriptions:        需量事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_1(void)//需量事件
{
	EventRecordDealFunction(0xE2010003);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_2
// Descriptions:        编程事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_2(void)//编程事件
{
	EventRecordDealFunction(0xE2010001);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3
// Descriptions:        失压事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_3(void)//失压事件
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_3);//显示菜单
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
				break;
			case 1://任务1:选择
				MenuSelect();//菜单选择
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_3_1);//创建下级菜单  1.A相失压事件
							break;
						case 2:
							MenuCreate((u32)Menu_5_3_2);//创建下级菜单  2.B相失压事件
							break;
						case 3:
							MenuCreate((u32)Menu_5_3_3);//创建下级菜单  3.C相失压事件
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
				break;
			default:
				break;
		}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3_1
// Descriptions:        A相失压事件(E2010004)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_3_1(void)//A相失压事件
{
	EventRecordDealFunction(0xE2010004);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3_2
// Descriptions:        B相失压事件(E2010005)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_3_2(void)//B相失压事件
{
	EventRecordDealFunction(0xE2010005);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3_3
// Descriptions:        C相失压事件(E2010006)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_3_3(void)//C相失压事件
{
	EventRecordDealFunction(0xE2010006);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4
// Descriptions:        失流事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_4(void)//失流事件
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_4);//显示菜单
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
				break;
			case 1://任务1:选择
				MenuSelect();//菜单选择
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_4_1);//创建下级菜单  1.A相失流事件
							break;
						case 2:
							MenuCreate((u32)Menu_5_4_2);//创建下级菜单  2.B相失流事件
							break;
						case 3:
							MenuCreate((u32)Menu_5_4_3);//创建下级菜单  3.C相失流事件
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
				break;
			default:
				break;
		}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4_1
// Descriptions:        A相失流事件(E2010007)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_4_1(void)//A相失流事件
{
	EventRecordDealFunction(0xE2010007);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4_2
// Descriptions:        B相失流事件(E2010008)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_4_2(void)//B相失流事件
{
	EventRecordDealFunction(0xE2010008);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4_3
// Descriptions:        C相失流事件(E2010009)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_4_3(void)//C相失流事件
{
	EventRecordDealFunction(0xE2010009);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5
// Descriptions:        断相事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_5(void)//断相事件
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_5);//显示菜单
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
				break;
			case 1://任务1:选择
				MenuSelect();//菜单选择
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_5_1);//创建下级菜单  1.A相断相事件
							break;
						case 2:
							MenuCreate((u32)Menu_5_5_2);//创建下级菜单  2.B相断相事件
							break;
						case 3:
							MenuCreate((u32)Menu_5_5_3);//创建下级菜单  3.C相断相事件
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
				break;
			default:
				break;
		}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5_1
// Descriptions:        A相断相事件(E201000B)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_5_1(void)//A相断相事件
{
	EventRecordDealFunction(0xE201000B);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5_2
// Descriptions:        B相断相事件(E201000C)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_5_2(void)//B相断相事件
{
	EventRecordDealFunction(0xE201000C);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5_3
// Descriptions:        C相断相事件(E201000D)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_5_3(void)//C相断相事件
{
	EventRecordDealFunction(0xE201000D);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_6
// Descriptions:        掉电/上电事件(E201000A)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_6(void)//掉电/上电事件
{
	EventRecordDealFunction(0xE201000A);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_7(E201000F)
// Descriptions:        清零事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_7(void)//清零事件
{
	EventRecordDealFunction(0xE201000F);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_8(E2010010)
// Descriptions:        校时事件
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_8(void)//校时事件
{
	EventRecordDealFunction(0xE2010010);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_9
// Descriptions:        表端钮盒开启事件(E2010011)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_9(void)//表端钮盒开启事件
{
	EventRecordDealFunction(0xE2010011);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_10
// Descriptions:        表盖开启事件(E2010012)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_10(void)//表盖开启事件
{
	EventRecordDealFunction(0xE2010012);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_11
// Descriptions:        箱门开闭事件(E2010015)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_11(void)//箱门开闭事件
{
	EventRecordDealFunction(0xE2010015);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_12
// Descriptions:        消息认证错误事件(E2010013)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_12(void)//消息认证错误事件
{
	EventRecordDealFunction(0xE2010013);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5
// Descriptions:        事件记录
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5(void)//事件记录
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayClr();//清全屏
				DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5);//显示菜单
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
				break;
			case 1://任务1:选择
				MenuSelect();//菜单选择
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_1);//创建下级菜单  1.需量事件
							break;
						case 2:
							MenuCreate((u32)Menu_5_2);//创建下级菜单  2.编程事件
							break;
						case 3:
							MenuCreate((u32)Menu_5_3);//创建下级菜单  3.失压事件
							break;
						case 4:
							MenuCreate((u32)Menu_5_4);//创建下级菜单  4.失流事件
							break;
						case 5:
							MenuCreate((u32)Menu_5_5);//创建下级菜单  5.断相事件
							break;
						case 6:
							MenuCreate((u32)Menu_5_6);//创建下级菜单  6.掉电/上电事件
							break;
						case 7:						
							MenuCreate((u32)Menu_5_7);//创建下级菜单  7.清零事件
							break;
						case 8:
							MenuCreate((u32)Menu_5_8);//创建下级菜单  8.校时事件
							break;
						case 9:
							MenuCreate((u32)Menu_5_9);//创建下级菜单  9.表端钮盒开启事件
							break;
						case 10:
							MenuCreate((u32)Menu_5_10);//创建下级菜单  10.表盖开启事件
							break;
						case 11:
							MenuCreate((u32)Menu_5_11);//创建下级菜单  11.箱门开闭事件
							break;
						case 12:
							MenuCreate((u32)Menu_5_12);//创建下级菜单  12.消息认证错误事件
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
				break;
			default:
				break;
		}
}

