/*************************************************
版权:正泰仪表智能终端部
文件:Menu_4_PowerOffStatistics.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单4停电统计处理文件
*************************************************/


//菜单4 停电统计
#include "../Hcsg/ProjectCSG.h"
#include "../Hcsg/IRAM_TerminalSaveCSG.h"
#include "../Display/Display.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Menu.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include "../QCSG_Head/QCSG_Hal.h"


const u8 pMenu_4[]=//负控终端停电统计数据
{
	"\n\r"
	"当前停电统计数据\n\r"
	"历史日停电统计数据\n\r"
	"历史月停电统计数据\n\r"
};

const u8 pMenu_4_1[][20]=
{
	"停电次数:      (次)",
	"停电时间:      (分)",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_1
// Descriptions:        本日、本月停电统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_1(void)//当前停电统计数据
{
	u16 *p16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																															"\n\r"
																															"  当前停电统计数据 "
																															);//显示菜单	
	DisplayClrRow(Ln+3,Ln+7);//清屏幕显示的指定行		
	//本日停电统计数据
	DisplayString(Ln+2,0,0,"本日:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	//停电次数
	MR((u32)&Databuff,QCSG_POWER_DOWN_NUM_DAY,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	//停电时间
	MR((u32)&Databuff,QCSG_POWER_DOWN_TIME_DAY,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	
	//本月停电统计数据
	DisplayString(Ln+5,0,0,"本月:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+6+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	
	//停电次数
	MR((u32)&Databuff,QCSG_POWER_DOWN_NUM_MONTH,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	//停电时间
	MR((u32)&Databuff,QCSG_POWER_DOWN_TIME_MONTH,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_2
// Descriptions:        历史日停电统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_2(void)//历史日停电统计数据
{
	u16 *p16;
	u32 i,Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																															"\n\r"
																															" 历史日停电统计数据 "
																															);//显示菜单
	DisplayClrRow(Ln+2,Ln+4);//清屏幕显示的指定行		
	//历史日停电统计数据
	DisplayString(Ln+2,0,0,"上一日:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008030, &DataTime, PnData, &DataLen);
			//停电次数
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
			//停电时间
			NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008031, &DataTime, PnData, &DataLen);
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3
// Descriptions:        历史月停电统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_3(void)//历史月停电统计数据
{
	u16 *p16;
	u32 i,Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																															"\n\r"
																															" 历史月停电统计数据 "
																														);//显示菜单	
	DisplayClrRow(Ln+2,Ln+4);//清屏幕显示的指定行		
	//历史日停电统计数据
	DisplayString(Ln+2,0,0,"上一月:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C030, &DataTime, PnData, &DataLen);
			//停电次数
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
			//停电时间
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C031, &DataTime, PnData, &DataLen);
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
		//}
	}			
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	




//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4
// Descriptions:        停电统计
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4(void)
{
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4);//显示菜单
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
							MenuCreate((u32)Menu_4_1);
							break;
						case 2:
							MenuCreate((u32)Menu_4_2);
							break;
						case 3:
							MenuCreate((u32)Menu_4_3);
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

