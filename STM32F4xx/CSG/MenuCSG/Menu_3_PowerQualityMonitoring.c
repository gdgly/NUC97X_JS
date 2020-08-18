/*************************************************
版权:正泰仪表智能终端部
文件:Menu_3_PowerQualityMonitoring.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单3电能质量监测处理文件
*************************************************/


//菜单3 电能质量监测
#include "../Hcsg/ProjectCSG.h"
#include "../Display/Display.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Menu.h"
#include "Menu3.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include "../QCSG_Head/QCSG_Hal.h"


const u8 pMenu_3[]=//负控终端电能质量监测
{
	"\n\r"
	"谐波数据\n\r"
	"电压监测数据\n\r"
	"电压不平衡度越限数据\n\r"
	"电流不平衡度越限数据\n\r"
	"功率因数区段统计数据\n\r"
};


//const u8 pMenu_3_1[]=//谐波数据
//{
//	"\n\r"
//	"\n\r"
//	"A相电压畸变:\n\r"
//	"B相电压畸变:\n\r"
//	"C相电压畸变:\n\r"
//	"A相电流畸变:\n\r"
//	"B相电流畸变:\n\r"
//	"C相电流畸变:\n\r"
//};


const u8 pMenu_3_1[]=//谐波数据
{
	"\n\r"
	"实时数据\n\r"
	"当日数据\n\r"
	"当月数据\n\r"
	"历史日数据\n\r"
	"历史月数据\n\r"
};

const u8 pMenu_3_1_1[]=//谐波实时数据
{
	"\n\r"
	"电压谐波含有率\n\r"
	"电流谐波含有率\n\r"
};

const u8 pMenu_3_1_4[]=//谐波数据
{
	"\n\r"
	"A相电压总畸变数据\n\r"
	"B相电压总畸变数据\n\r"
	"C相电压总畸变数据\n\r"
};

const u8 pMenu_3_1_4_1[]=
{
	"\n\r"
	"选择日期:\n\r"
	"        确认\n\r"
};


const u8 pMenu_3_1_4_1_2[]=
{
	"\n\r"
	"监测时间:    分\n\r"
	"合格率:       %\n\r"
	"超上限率:       %\n\r"
	"上限时间:    分\n\r"
	"最高总畸变:     %\n\r"
	"出现时间:  -  -  -  \n\r"
	"最低总畸变:     %\n\r"
	"出现时间:  -  -  -  \n\r"
};


const u8 pMenu_3_1_4_1_3[]=
{
	"\n\r"
	"监测时间:      分\n\r"
	"合格率:       %\n\r"
	"超上限率:       %\n\r"
	"上限时间:      分\n\r"
	"最高总畸变:     %\n\r"
	"出现时间:  -  -  -  \n\r"
	"最低总畸变:     %\n\r"
	"出现时间:  -  -  -  \n\r"
};

const u8 pMenu_3_2[]=//
{
	"\n\r"
	"当日数据\n\r"
	"当月数据\n\r"
};





#if (DLT500_REPORT == 1)
	const u8 pMenu_3_2_1[]=//电压合格率统计数据
	{
		"\n\r"
		"A相电压合格率数据\n\r"
		"B相电压合格率数据\n\r"
		"C相电压合格率数据\n\r"
	};


	const u8 pMenu_3_2_1_1[]=//
	{
		"\n\r"
		"\n\r"
		"监测时间:      分\n\r"
		"合格率:       %\n\r"
		"超上限率:       %\n\r"
		"超下限率:       %\n\r"
		"合格时间:      分\n\r"
		"超上限时间:      分\n\r"
		"超下限时间:      分\n\r"
	};

#else
	const u8 pMenu_3_2_1[]=//电压合格率统计数据
	{
		"\n\r"
		"总电压合格率数据\n\r"
		"A相电压合格率数据\n\r"
		"B相电压合格率数据\n\r"
		"C相电压合格率数据\n\r"
	};



	const u8 pMenu_3_2_1_1[]=//
	{
		"\n\r"
		"\n\r"
		"监测时间:      分\n\r"
		"合格率:       %\n\r"
		"超限率:       %\n\r"
		"超上限时间:      分\n\r"
		"超下限时间:      分\n\r"
	};
#endif

	
const u8 pMenu_3_2_1_1_1[]=//
{
	"\n\r"
	"\n\r"
	"最高电压:     V\n\r"
	"最高电压出现时间:\n\r"
	"  月  日  时  分\n\r"
	"最低电压:     V\n\r"
	"最低电压出现时间:\n\r"
	"  月  日  时  分\n\r"
};


const u8 pMenu_3_3[]=//
{
	"\n\r"
	"历史日数据\n\r"
	"历史月数据\n\r"
};



const u8 pMenu_3_3_1[]=//电压不平衡度越限统计数据1
{
	"\n\r"
	"\n\r"
	"最大值:     %\n\r"
	"最小值:     %\n\r"
	"平均值:     %\n\r"
	"95%值:     %\n\r"
};

const u8 pMenu_3_3_2[]=//电压不平衡度越限统计数据2
{
	"\n\r"
	"\n\r"
	"极值:     %\n\r"
	"极值发生时间:\n\r"	
	"  月  日  时  分\n\r"
	"极值累计时间:\n\r"
	"分钟:        \n\r"
};


const u8 pMenu_3_5[]=//功率因数区段统计数据
{
	"\n\r"
	"当日数据\n\r"
	"当月数据\n\r"
	"历史日数据\n\r"
	"历史月数据\n\r"
};



const u8 pMenu_3_5_1[]=//功率因数区段统计数据1
{
	"\n\r"
	"功率因数监测时间:\n\r"
	"      分\n\r"
	"区段1时间:      分\n\r"
	"区段2时间:      分\n\r"
	"区段3时间:      分\n\r"
	"区段4时间:      分\n\r"
};


const u8 pMenu_3_5_2[]=//功率因数区段统计数据2
{
	"\n\r"
	"区段5时间:      分\n\r"
	"区段6时间:      分\n\r"
	"区段7时间:      分\n\r"
	"区段8时间:      分\n\r"
	"区段9时间:      分\n\r"
	"区段10时间:      分\n\r"
};



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1
// Descriptions:        谐波数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1(void)//谐波数据
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1);//显示菜单
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
						MenuCreate((u32)Menu_3_1_1);//实时数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_2);//当日数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_3);//当月数据
						break;
					case 4:
						MenuCreate((u32)Menu_3_1_4);//历史日数据
						break;
					case 5:
						MenuCreate((u32)Menu_3_1_5);//历史月数据
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
// Function name:       Menu_3_1_1
// Descriptions:        谐波实时数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_1(void)//谐波实时数据
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_1);//显示菜单
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
						MenuCreate((u32)Menu_3_1_1_1);//电压谐波含有率
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_1_2);//电流谐波含有率
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
// Function name:       Menu_3_1_1_1
// Descriptions:        电压谐波含有率
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_1_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"A相电压总谐波含有量:\n\r"
																	"     %\n\r"	
																	"B相电压总谐波含有量:\n\r"
																	"     %\n\r"
																	"C相电压总谐波含有量:\n\r"
																	"     %\n\r"
																	);//显示菜单
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//A相电压总谐波含有量
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0100[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//B相电压总谐波含有量
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0200[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//C相电压总谐波含有量
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0300[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_1_2
// Descriptions:        电流谐波含有率
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_1_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"A相电流总谐波含有量:\n\r"
																	"     %\n\r"	
																	"B相电流总谐波含有量:\n\r"
																	"     %\n\r"
																	"C相电流总谐波含有量:\n\r"
																	"     %\n\r"
																	);//显示菜单
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//A相电流总谐波含有量
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0200[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//B相电流总谐波含有量
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020B0200[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//C相电流总谐波含有量
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020B0300[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
		
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_2
// Descriptions:        当日数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//显示菜单
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
						MenuCreate((u32)Menu_3_1_2_1);//A相电压总畸变数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_2_2);//B相电压总畸变数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_2_3);//C相电压总畸变数据
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
// Function name:       Menu_3_1_2_1
// Descriptions:        当日数据（A相电压总畸变数据）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneDay(&RealTime);
//			if(Ret==TRUE)
//			{
		RealTimeToYYTime(&RealTime, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008025, &DataTime, PnData, &DataLen);
		//总畸变监测时间
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//总畸变合格率
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变超上限率
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变上限时间
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//最高总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//最高总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//最低总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//最低总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
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
// Function name:       Menu_3_1_2_2
// Descriptions:        B相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2_2(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;		
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(&RealTime, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008026, &DataTime, PnData, &DataLen);
		//总畸变监测时间
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//总畸变合格率
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变超上限率
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变上限时间
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//最高总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//最高总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//最低总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//最低总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
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
// Function name:       Menu_3_1_2_3
// Descriptions:        C相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2_3(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(&RealTime, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008027, &DataTime, PnData, &DataLen);
		//总畸变监测时间
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//总畸变合格率
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变超上限率
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变上限时间
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//最高总畸变
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//最高总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//最低总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//最低总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
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
// Function name:       Menu_3_1_3
// Descriptions:        当月数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//显示菜单
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
						MenuCreate((u32)Menu_3_1_3_1);//A相电压总畸变数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_3_2);//B相电压总畸变数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_3_3);//C相电压总畸变数据
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
// Function name:       Menu_3_1_3_1
// Descriptions:        当月数据（A相电压总畸变数据）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneMonth(&RealTime);
//			if(Ret==TRUE)
//			{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C025, &DataTime, PnData, &DataLen);
			//总畸变监测时间
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+1,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//总畸变合格率
			MR((u32)&Databuff,(u32)&PnData[3],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+2,7,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
				DisplayData(Databuff,6,2,q16);
			}
			//总畸变超上限率
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[6],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				DisplayData(Databuff,6,2,q16);
			}
			//总畸变上限时间
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[9],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//最高总畸变
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[12],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+5,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}
			//最高总畸变出现时间
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
			if(PnData[14]==0xFF)
			{
				DisplayString(Ln+6,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[14],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
			if(PnData[15]==0xFF)
			{
				DisplayString(Ln+6,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[15],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
			if(PnData[16]==0xFF)
			{
				DisplayString(Ln+6,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[16],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
			if(PnData[17]==0xFF)
			{
				DisplayString(Ln+6,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[17],2,0,q16);
			}
			//最低总畸变
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[18],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+7,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}	
			//最低总畸变出现时间
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
			if(PnData[20]==0xFF)
			{
				DisplayString(Ln+8,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[18],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
			if(PnData[21]==0xFF)
			{
				DisplayString(Ln+8,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[19],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
			if(PnData[22]==0xFF)
			{
				DisplayString(Ln+8,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[20],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
			if(PnData[23]==0xFF)
			{
				DisplayString(Ln+8,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[21],2,0,q16);
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
// Function name:       Menu_3_1_3_2
// Descriptions:        B相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3_2(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneMonth(&RealTime);
//			if(Ret==TRUE)
//			{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C026, &DataTime, PnData, &DataLen);
			//总畸变监测时间
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+1,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//总畸变合格率
			MR((u32)&Databuff,(u32)&PnData[3],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+2,7,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
				DisplayData(Databuff,6,2,q16);
			}
			//总畸变超上限率
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[6],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				DisplayData(Databuff,6,2,q16);
			}
			//总畸变上限时间
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[9],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//最高总畸变
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[12],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+5,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}
			//最高总畸变出现时间
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
			if(PnData[14]==0xFF)
			{
				DisplayString(Ln+6,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[14],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
			if(PnData[15]==0xFF)
			{
				DisplayString(Ln+6,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[15],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
			if(PnData[16]==0xFF)
			{
				DisplayString(Ln+6,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[16],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
			if(PnData[17]==0xFF)
			{
				DisplayString(Ln+6,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[17],2,0,q16);
			}
			//最低总畸变
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[18],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+7,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}	
			//最低总畸变出现时间
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
			if(PnData[20]==0xFF)
			{
				DisplayString(Ln+8,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[18],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
			if(PnData[21]==0xFF)
			{
				DisplayString(Ln+8,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[19],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
			if(PnData[22]==0xFF)
			{
				DisplayString(Ln+8,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[20],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
			if(PnData[23]==0xFF)
			{
				DisplayString(Ln+8,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[21],2,0,q16);
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
// Function name:       Menu_3_1_3_3
// Descriptions:        C相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3_3(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneMonth(&RealTime);
//			if(Ret==TRUE)
//			{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C027, &DataTime, PnData, &DataLen);
			//总畸变监测时间
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+1,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//总畸变合格率
			MR((u32)&Databuff,(u32)&PnData[3],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+2,7,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
				DisplayData(Databuff,6,2,q16);
			}
			//总畸变超上限率
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[6],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				DisplayData(Databuff,6,2,q16);
			}
			//总畸变上限时间
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[9],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//最高总畸变
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[12],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+5,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}
			//最高总畸变出现时间
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
			if(PnData[14]==0xFF)
			{
				DisplayString(Ln+6,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[14],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
			if(PnData[15]==0xFF)
			{
				DisplayString(Ln+6,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[15],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
			if(PnData[16]==0xFF)
			{
				DisplayString(Ln+6,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[16],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
			if(PnData[17]==0xFF)
			{
				DisplayString(Ln+6,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[17],2,0,q16);
			}
			//最低总畸变
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[18],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+7,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}	
			//最低总畸变出现时间
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
			if(PnData[20]==0xFF)
			{
				DisplayString(Ln+8,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[18],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
			if(PnData[21]==0xFF)
			{
				DisplayString(Ln+8,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[19],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
			if(PnData[22]==0xFF)
			{
				DisplayString(Ln+8,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[20],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
			if(PnData[23]==0xFF)
			{
				DisplayString(Ln+8,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[21],2,0,q16);
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
// Function name:       Menu_3_1_4
// Descriptions:        历史日数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//显示菜单
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
						MenuCreate((u32)Menu_3_1_4_1);//A相电压总畸变数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_4_2);//B相电压总畸变数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_4_3);//C相电压总畸变数据
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
// Function name:       Menu_3_1_4_1
// Descriptions:        历史日A相电压总畸变数据（选择日期）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,3);
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p[0*2]=(p8[2]>>4)+0x30;//年
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[1]>>4)+0x30;//月
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[0]>>4)+0x30;//日
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}				
			
			MenuCount->v1=9;
			MenuCount->v2=16;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回 
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}			
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=16;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年				
								
								RealTime.u8Day=p8[0];
								RealTime.u8Month=p8[1];
								RealTime.u8Year=p8[2];
								RealTime.u8Second=0;
								RealTime.u8Minute=0;
								RealTime.u8Hour=0;
								Menu_3_1_4_1_2(&RealTime);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=9;
								MenuCount->v2=16;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<2)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}	  
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_1_2
// Descriptions:        历史日A相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_1_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
//Ret=QCSG_DecOneDay(&RealTime);
//if(Ret==TRUE)
//{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008025, &DataTime, PnData, &DataLen);
		//总畸变监测时间
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//总畸变合格率
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变超上限率
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变上限时间
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//最高总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//最高总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//最低总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//最低总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
	//}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_2
// Descriptions:        历史日B相电压总畸变数据（选择日期）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_2(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,3);
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p[0*2]=(p8[2]>>4)+0x30;//年
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[1]>>4)+0x30;//月
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[0]>>4)+0x30;//日
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
							
			MenuCount->v1=9;
			MenuCount->v2=16;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回 
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}			
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=16;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年				
								
								RealTime.u8Day=p8[0];
								RealTime.u8Month=p8[1];
								RealTime.u8Year=p8[2];
								RealTime.u8Second=0;
								RealTime.u8Minute=0;
								RealTime.u8Hour=0;
								Menu_3_1_4_2_2(&RealTime);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=9;
								MenuCount->v2=16;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<2)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}	  
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_2_2
// Descriptions:        历史日B相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_2_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008026, &DataTime, PnData, &DataLen);
		//总畸变监测时间
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//总畸变合格率
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变超上限率
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变上限时间
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//最高总畸变
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//最高总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//最低总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//最低总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
	//}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_3
// Descriptions:        历史日C相电压总畸变数据（选择日期）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_3(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,3);
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p[0*2]=(p8[2]>>4)+0x30;//年
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[1]>>4)+0x30;//月
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[0]>>4)+0x30;//日
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
					
			MenuCount->v1=9;
			MenuCount->v2=16;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回 
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}			
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=16;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年				
								
								RealTime.u8Day=p8[0];
								RealTime.u8Month=p8[1];
								RealTime.u8Year=p8[2];
								RealTime.u8Second=0;
								RealTime.u8Minute=0;
								RealTime.u8Hour=0;
								Menu_3_1_4_3_2(&RealTime);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=9;
								MenuCount->v2=16;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<2)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}	  
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_3_2
// Descriptions:        历史日C相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_3_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008027, &DataTime, PnData, &DataLen);
		//总畸变监测时间
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//总畸变合格率
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变超上限率
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//总畸变上限时间
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//最高总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//最高总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//最低总畸变
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//最低总畸变出现时间
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
	//}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5
// Descriptions:        历史月数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//显示菜单
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
						MenuCreate((u32)Menu_3_1_5_1);//A相电压总畸变数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_5_2);//B相电压总畸变数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_5_3);//C相电压总畸变数据
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
// Function name:       Menu_3_1_5_1
// Descriptions:        历史月A相电压总畸变数据（选择日期）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,3);
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p[0*2]=(p8[2]>>4)+0x30;//年
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[1]>>4)+0x30;//月
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[0]>>4)+0x30;//日
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}				
			
			MenuCount->v1=9;
			MenuCount->v2=16;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回 
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}			
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=16;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年				
								
								RealTime.u8Day=p8[0];
								RealTime.u8Month=p8[1];
								RealTime.u8Year=p8[2];
								RealTime.u8Second=0;
								RealTime.u8Minute=0;
								RealTime.u8Hour=0;
								Menu_3_1_5_1_2(&RealTime);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=9;
								MenuCount->v2=16;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<2)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}	  
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_1_2
// Descriptions:        历史月A相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_1_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	RealTimeToYYTime(Time, &DataTime);
	NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C025, &DataTime, PnData, &DataLen);
	//总畸变监测时间
	MR((u32)&Databuff,(u32)&PnData[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//总畸变合格率
	MR((u32)&Databuff,(u32)&PnData[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,7,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//总畸变超上限率
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//总畸变上限时间
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//最高总畸变
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[12],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}
	//最高总畸变出现时间
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
	if(PnData[14]==0xFF)
	{
		DisplayString(Ln+6,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[14],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
	if(PnData[15]==0xFF)
	{
		DisplayString(Ln+6,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[15],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
	if(PnData[16]==0xFF)
	{
		DisplayString(Ln+6,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[16],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
	if(PnData[17]==0xFF)
	{
		DisplayString(Ln+6,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[17],2,0,q16);
	}
	//最低总畸变
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[18],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}	
	//最低总畸变出现时间
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
	if(PnData[20]==0xFF)
	{
		DisplayString(Ln+8,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[18],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
	if(PnData[21]==0xFF)
	{
		DisplayString(Ln+8,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[19],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
	if(PnData[22]==0xFF)
	{
		DisplayString(Ln+8,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[20],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
	if(PnData[23]==0xFF)
	{
		DisplayString(Ln+8,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[21],2,0,q16);
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_2
// Descriptions:        历史月B相电压总畸变数据（选择日期）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_2(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,3);
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p[0*2]=(p8[2]>>4)+0x30;//年
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[1]>>4)+0x30;//月
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[0]>>4)+0x30;//日
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
							
			MenuCount->v1=9;
			MenuCount->v2=16;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回 
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}			
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=16;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年				
								
								RealTime.u8Day=p8[0];
								RealTime.u8Month=p8[1];
								RealTime.u8Year=p8[2];
								RealTime.u8Second=0;
								RealTime.u8Minute=0;
								RealTime.u8Hour=0;
								Menu_3_1_5_2_2(&RealTime);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=9;
								MenuCount->v2=16;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<2)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}	  
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_2_2
// Descriptions:        历史月B相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_2_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	RealTimeToYYTime(Time, &DataTime);
	NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C026, &DataTime, PnData, &DataLen);
	//总畸变监测时间
	MR((u32)&Databuff,(u32)&PnData[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//总畸变合格率
	MR((u32)&Databuff,(u32)&PnData[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,7,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//总畸变超上限率
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//总畸变上限时间
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//最高总畸变
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[12],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}
	//最高总畸变出现时间
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
	if(PnData[14]==0xFF)
	{
		DisplayString(Ln+6,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[14],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
	if(PnData[15]==0xFF)
	{
		DisplayString(Ln+6,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[15],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
	if(PnData[16]==0xFF)
	{
		DisplayString(Ln+6,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[16],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
	if(PnData[17]==0xFF)
	{
		DisplayString(Ln+6,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[17],2,0,q16);
	}
	//最低总畸变
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[18],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}	
	//最低总畸变出现时间
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
	if(PnData[20]==0xFF)
	{
		DisplayString(Ln+8,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[18],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
	if(PnData[21]==0xFF)
	{
		DisplayString(Ln+8,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[19],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
	if(PnData[22]==0xFF)
	{
		DisplayString(Ln+8,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[20],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
	if(PnData[23]==0xFF)
	{
		DisplayString(Ln+8,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[21],2,0,q16);
	}	
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_3
// Descriptions:        历史日C相电压总畸变数据（选择日期）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_3(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,3);
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p[0*2]=(p8[2]>>4)+0x30;//年
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[1]>>4)+0x30;//月
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[0]>>4)+0x30;//日
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
					
			MenuCount->v1=9;
			MenuCount->v2=16;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回 
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}			
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=16;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年				
								
								RealTime.u8Day=p8[0];
								RealTime.u8Month=p8[1];
								RealTime.u8Year=p8[2];
								RealTime.u8Second=0;
								RealTime.u8Minute=0;
								RealTime.u8Hour=0;
								Menu_3_1_5_3_2(&RealTime);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=9;
								MenuCount->v2=16;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<2)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}	  
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_3_2
// Descriptions:        历史月C相电压总畸变数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_3_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//显示菜单	
	Pn_Lcd_Data->u16Pn=0x0000;
	RealTimeToYYTime(Time, &DataTime);
	NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C027, &DataTime, PnData, &DataLen);
	//总畸变监测时间
	MR((u32)&Databuff,(u32)&PnData[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//总畸变合格率
	MR((u32)&Databuff,(u32)&PnData[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,7,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//总畸变超上限率
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//总畸变上限时间
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//最高总畸变
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[12],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}
	//最高总畸变出现时间
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //分
	if(PnData[14]==0xFF)
	{
		DisplayString(Ln+6,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[14],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //时
	if(PnData[15]==0xFF)
	{
		DisplayString(Ln+6,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[15],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //日
	if(PnData[16]==0xFF)
	{
		DisplayString(Ln+6,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[16],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //月
	if(PnData[17]==0xFF)
	{
		DisplayString(Ln+6,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[17],2,0,q16);
	}
	//最低总畸变
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[18],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}	
	//最低总畸变出现时间
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //分
	if(PnData[20]==0xFF)
	{
		DisplayString(Ln+8,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[18],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //时
	if(PnData[21]==0xFF)
	{
		DisplayString(Ln+8,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[19],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //日
	if(PnData[22]==0xFF)
	{
		DisplayString(Ln+8,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[20],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //月
	if(PnData[23]==0xFF)
	{
		DisplayString(Ln+8,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[21],2,0,q16);
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2
// Descriptions:        电压监测数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2(void)//电压监测数据
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2);//显示菜单
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
						MenuCreate((u32)Menu_3_2_1);//当日数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_2);//当月数据
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


#if (DLT500_REPORT == 1)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1
// Descriptions:        当日数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//显示菜单
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
						MenuCreate((u32)Menu_3_2_1_1);//A相电压合格率统计数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_1_2);//B相电压合格率统计数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_1_3);//C相电压合格率统计数据
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
// Function name:       Menu_3_2_1_1
// Descriptions:        A相电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超下限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压合格累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A相电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3
// Descriptions:        B相电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超下限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压合格累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3_1
// Descriptions:        B相电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4
// Descriptions:        C相电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超下限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压合格时累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_3_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4_1
// Descriptions:        C相电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_3);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2
// Descriptions:        当月数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//显示菜单
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
						MenuCreate((u32)Menu_3_2_2_1);//A相电压合格率统计数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_2_2);//B相电压合格率统计数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_2_3);//C相电压合格率统计数据
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
// Function name:       Menu_3_2_2_2
// Descriptions:        A相电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超下限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压合格累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A相电压合格率数据2（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_2
// Descriptions:        B相电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超下限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压合格累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_3_1
// Descriptions:        B相电压合格率数据2（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4
// Descriptions:        C相电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超下限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压合格累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_3_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4_1
// Descriptions:        C相电压合格率数据2（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_3);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1
// Descriptions:        当日数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//显示菜单
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
						MenuCreate((u32)Menu_3_2_1_1);//总电压合格率统计数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_1_2);//A相电压合格率统计数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_1_3);//B相电压合格率统计数据
						break;
					case 4:
						MenuCreate((u32)Menu_3_2_1_4);//C相电压合格率统计数据
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
// Function name:       Menu_3_2_1_1
// Descriptions:        总电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"总电压合格率数据1");	
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_1_1
// Descriptions:        总电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"总电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000010[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000010[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000010[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000010[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000010[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000010[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000010[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000010[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2
// Descriptions:        A相电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A相电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3
// Descriptions:        B相电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_3_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3_1
// Descriptions:        B相电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_3);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4
// Descriptions:        C相电压合格率数据1（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_4(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_4_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_4_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4_1
// Descriptions:        C相电压合格率数据2（当日）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_4_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_1_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_1_4);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2
// Descriptions:        当月数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//显示菜单
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
						MenuCreate((u32)Menu_3_2_2_1);//总电压合格率统计数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_2_2);//A相电压合格率统计数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_2_3);//B相电压合格率统计数据
						break;
					case 4:
						MenuCreate((u32)Menu_3_2_2_4);//C相电压合格率统计数据
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
// Function name:       Menu_3_2_2_1
// Descriptions:        总电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"总电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0

}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_1_1（当月）
// Descriptions:        总电压合格率数据2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"总电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100000[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100000[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100000[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100000[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100000[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100000[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100000[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100000[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_2
// Descriptions:        A相电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A相电压合格率数据2（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"A相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_3
// Descriptions:        B相电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_3_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_3_1
// Descriptions:        B相电压合格率数据2（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"B相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_3);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4
// Descriptions:        C相电压合格率数据1（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_4(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//电压监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//电压合格率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超限率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//电压超上限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//电压超下限时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_4_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_4_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4_1
// Descriptions:        C相电压合格率数据2（当月）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_4_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//清全屏
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//显示菜单	
	DisplayString(1,1,0,"C相电压合格率数据2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //只显示交采测量点数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//最高电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最高电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//最低电压
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//最低电压出现时刻
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_2_2_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_2_2_4);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
#endif



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3
// Descriptions:        电压不平衡度越限统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_3(void)//电压不平衡度越限统计数据
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3);//显示菜单
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
						MenuCreate((u32)Menu_3_3_1);//历史日数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_3_2);//历史月数据
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
// Function name:       Menu_3_3_1
// Descriptions:        电压不平衡度越限统计数据（历史日电压不平衡度1）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_3_1(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//显示菜单
	DisplayString(Ln+1,0,0,"历史日电压不平衡度1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008021, &DataTime, PnData, &DataLen);
				//最大值
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//最小值
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//平均值
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%值
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_3_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_3_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3_1_1
// Descriptions:        历史日电压不平衡度2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_3_1_1(void)//历史日电压不平衡度2
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//显示菜单
	DisplayString(Ln+1,0,0,"历史日电压不平衡度2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//极值
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001C[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//极值发生时间
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100001C[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100001C[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100001C[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100001C[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//极值累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001C[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_3_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3_2
// Descriptions:        电压不平衡度越限统计数据（历史月电压不平衡度1）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_3_2(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//显示菜单
	DisplayString(Ln+1,0,0,"历史月电压不平衡度1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C021, &DataTime, PnData, &DataLen);
				//最大值
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//最小值
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//平均值
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%值
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_3_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_3_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3_2_1
// Descriptions:        历史月电压不平衡度2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_3_2_1(void)
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//显示菜单
	DisplayString(Ln+1,0,0,"历史月电压不平衡度2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//极值
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401C[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//极值发生时间
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100401C[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100401C[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100401C[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100401C[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//极值累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401C[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_3_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_3_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4
// Descriptions:        电流不平衡度越限统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3);//显示菜单
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
						MenuCreate((u32)Menu_3_4_1);//历史日数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_4_2);//历史月数据
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
// Function name:       Menu_3_4_1
// Descriptions:        电流不平衡度越限统计数据（历史日电流不平衡度1）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_4_1(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//显示菜单
	DisplayString(Ln+1,0,0,"历史日电流不平衡度1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008020, &DataTime, PnData, &DataLen);
				//最大值
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//最小值
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//平均值
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%值
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_4_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_4_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4_1_1
// Descriptions:        历史日电流不平衡度2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_4_1_1(void)//历史日电流不平衡度2
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//显示菜单
	DisplayString(Ln+1,0,0,"历史日电流不平衡度2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//极值
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001B[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//极值发生时间
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100001B[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100001B[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100001B[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100001B[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//极值累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001B[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_4_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_4_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4_2
// Descriptions:        电流不平衡度越限统计数据（历史月电流不平衡度1）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_4_2(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//显示菜单
	DisplayString(Ln+1,0,0,"历史月电流不平衡度1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C020, &DataTime, PnData, &DataLen);
				//最大值
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//最小值
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//平均值
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%值
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_4_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_4_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4_2_1
// Descriptions:        历史月电流不平衡度2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_4_2_1(void)
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//显示菜单
	DisplayString(Ln+1,0,0,"历史月电流不平衡度2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//极值
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401B[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//极值发生时间
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100401B[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100401B[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100401B[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100401B[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //分
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //时
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //日
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //月
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//极值累计时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401B[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_4_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_4_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5
// Descriptions:        功率因数区段统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5(void)//功率因数区段统计数据
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5);//显示菜单
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
						MenuCreate((u32)Menu_3_5_1);//当日数据
						break;
					case 2:
						MenuCreate((u32)Menu_3_5_2);//当月数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_5_3);//历史日数据
						break;
					case 4:
						MenuCreate((u32)Menu_3_5_4);//历史月数据
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
// Function name:       Menu_3_5_1
// Descriptions:        当日功率因数区段统计数据1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_1(void)//功率因数区段统计数据
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//功率因数监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,0,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段1时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段2时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段3时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段4时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_1_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_1_1
// Descriptions:        当日功率因数区段统计数据2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_1_1(void)
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//区段5时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段6时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段7时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[21],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段8时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[24],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段9时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[27],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段10时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[30],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,p16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_2
// Descriptions:        当月功率因数区段统计数据1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_2(void)//功率因数区段统计数据
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//功率因数监测时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,0,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段1时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段2时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段3时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段4时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_2_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_2_1
// Descriptions:        当月功率因数区段统计数据2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_2_1(void)
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//区段5时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段6时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段7时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[21],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段8时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[24],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段9时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[27],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//区段10时间
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[30],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,p16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_2);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_3
// Descriptions:        历史日功率因数区段统计数据1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_3(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008043, &DataTime, PnData, &DataLen);
				//功率因数监测时间
				MR((u32)&Databuff,(u32)&PnData[0],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,0,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+0*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段1时间
				MR((u32)&Databuff,(u32)&PnData[3],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段2时间
				MR((u32)&Databuff,(u32)&PnData[6],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段3时间
				MR((u32)&Databuff,(u32)&PnData[9],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段4时间
				MR((u32)&Databuff,(u32)&PnData[12],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_3_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_3_1
// Descriptions:        历史日功率因数区段统计数据2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_3_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008043, &DataTime, PnData, &DataLen);
				//区段5时间
				MR((u32)&Databuff,(u32)&PnData[15],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+1,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段6时间
				MR((u32)&Databuff,(u32)&PnData[18],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段7时间
				MR((u32)&Databuff,(u32)&PnData[21],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段8时间
				MR((u32)&Databuff,(u32)&PnData[24],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段9时间
				MR((u32)&Databuff,(u32)&PnData[27],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段10时间
				MR((u32)&Databuff,(u32)&PnData[30],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,11,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+11*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_3);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_4
// Descriptions:        历史月功率因数区段统计数据1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_4(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C043, &DataTime, PnData, &DataLen);
				//功率因数监测时间
				MR((u32)&Databuff,(u32)&PnData[0],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,0,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+0*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段1时间
				MR((u32)&Databuff,(u32)&PnData[3],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段2时间
				MR((u32)&Databuff,(u32)&PnData[6],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段3时间
				MR((u32)&Databuff,(u32)&PnData[9],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段4时间
				MR((u32)&Databuff,(u32)&PnData[12],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_4_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_4_1);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_4_1
// Descriptions:        历史月功率因数区段统计数据2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3_5_4_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//显示菜单

	Pn_Lcd_Data->u16Pn = 0x0000;//只显示终端本身测量点0的数据
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C043, &DataTime, PnData, &DataLen);
				//区段5时间
				MR((u32)&Databuff,(u32)&PnData[15],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+1,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段6时间
				MR((u32)&Databuff,(u32)&PnData[18],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段7时间
				MR((u32)&Databuff,(u32)&PnData[21],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段8时间
				MR((u32)&Databuff,(u32)&PnData[24],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段9时间
				MR((u32)&Databuff,(u32)&PnData[27],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//区段10时间
				MR((u32)&Databuff,(u32)&PnData[30],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,11,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+11*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_3_5_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_3_5_4);//翻屛
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3
// Descriptions:        电能质量监测
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3(void)//电能质量监测
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3);//显示菜单
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
						MenuCreate((u32)Menu_3_1);//谐波数据 
						break;
					case 2:
						MenuCreate((u32)Menu_3_2);//电压监测数据
						break;
					case 3:
						MenuCreate((u32)Menu_3_3);//电压不平衡度越限统计数据
						break;
					case 4:
						MenuCreate((u32)Menu_3_4);//电流不平衡度越限统计数据
						break;
					case 5:
						MenuCreate((u32)Menu_3_5);//功率因数区段统计数据
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
