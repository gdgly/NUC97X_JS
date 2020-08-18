/*************************************************
版权:正泰仪表智能终端部
文件:Menu_2_PowerCalculation.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单2电能计量处理文件
*************************************************/


//菜单2 电能计量
#include "../Hcsg/ProjectCSG.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Menu.h"
#include "Menu2.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"





const u8 EQList[][20]=//电量列表
{
	"有功总         kWh",
	"有功尖         kWh",
	"有功峰         kWh",
	"有功平         kWh",
	"有功谷         kWh",
	"无功总         kvarh",
};


const u8 pMenu_2[]=
{
	"\n\r"
	"当前电量数据\n\r"
	"历史日数据\n\r"
	"历史月数据\n\r"
};

const u8 pMenu_2_1[]=
{
	"\n\r"
	"测量点号：\n\r"
	"        确认\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1
// Descriptions:        当前电量数据（选择测量点）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1(void)//当前电量数据（选择测量点）
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j,Ln=0;//菜单从第0行开始	
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;		
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//显示菜单
			
		  //默认显示测量点号为0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			if(Stack->MenuStack[MenuCount->Count].EditRow==2)
			{
				Stack->MenuStack[MenuCount->Count].EditRow=1;
				MenuCount->v1=9;
				MenuCount->v2=12;
				NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
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
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}					
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 2:
								//保存设置的数据
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,4);
								FlashLen = x;
								FlashLenAddr = ADDR_MP_START + (FlashLen * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
								FlashLen = sizeof(MeasurePointStruct) ;
								MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
								if((MeasurePoint.State == 0x01) && (BCD_Check(MeasurePoint.Addr, METER_ADDR_SIZE) == NO_ERROR))
								{
									MC(0,(u32)Pn_Lcd_Data,sizeof(QCSG_LCD_RAMDATA_S));
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;								
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}
								MenuCreate((u32)Menu_2_1_1);
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
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>MAX_PN_NUM)
								{
									bcd_p8(p8,9999,4);
								}
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
			Comm_Ram->ButtonVal&=~0x20;
			break;
		default:
			break;
	}
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_1
// Descriptions:        当前正向有功电能示值、组合无功1总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_1(void)//当前正向有功电能示值、组合无功1电能示值
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																					"\n\r"
																					"  当前正向电能示值  "
																					"    测量点          "
																					);//显示菜单
	
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
	{
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(Ln+4,4,0,"测量点不存在");
	}
	else
	{			
		if(Pn_Lcd_Data->u16Pn<0x0A)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);
		}
		else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
		}
		else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
		}
		else if(Pn_Lcd_Data->u16Pn>=0x3E8)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
		}
		
		DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
		}
		//有功总
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[0],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+3,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}
		//有功尖
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[4],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+4,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}
		//有功峰
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[8],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+5,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}
		//有功平
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[12],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+6,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}	
		//有功谷
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[16],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+7,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}
		//无功总
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0003FF00[0],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+8,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_1_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_1_6);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_2
// Descriptions:        当前反向有功电能示值、组合无功2总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_2(void)//当前反向有功电能示值、组合无功2电能示值
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
		
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																					"\n\r"
																					"  当前反向电能示值  "
																					"    测量点          "
																					);//显示菜单
	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}

	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//有功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功尖
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功峰
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功平
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//有功谷
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//无功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0004FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_3
// Descriptions:        当日正向有功电能量、组合无功1总电能量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_3(void)//当日正向有功电能量、组合无功1电能量
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   当日正向电能量   "
																				"    测量点          "
																				);//显示菜单	

	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
		
	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//有功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功尖
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功峰
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功平
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//有功谷
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//无功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0003FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_1_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_1_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_4
// Descriptions:        当日反向有功电能量、组合无功2总电能量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_4(void)//当日反向有功电能量、组合无功2总电能量
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   当日反向电能量   "
																				"    测量点          "
																				);//显示菜单
	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}

	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//有功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功尖
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功峰
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功平
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//有功谷
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//无功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0004FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
		
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_1_5);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_5
// Descriptions:        当月正向有功电能量、组合无功1总电能量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_5(void)//当月正向有功电能量、组合无功1总电能量
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   当月正向电能量   "
																				"    测量点          "
																				);//显示菜单	

	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
		
	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//有功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功尖
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功峰
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功平
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//有功谷
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//无功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0003FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_1_6);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_1_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_6
// Descriptions:        当月反向有功电能量、组合无功2总电能量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_6(void)//当月反向有功电能量、组合无功2总电能量
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   当月反向电能量   "
																				"    测量点          "
																				);//显示菜单
	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}

	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//有功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功尖
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功峰
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//有功平
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//有功谷
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//无功总
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0004FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_1_5);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_2
// Descriptions:        历史日数据（选择测量点）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_2(void)//历史日数据（选择测量点）
{
	u8 *p,*p8;
	u16 *p16;
	u32 j,Ln;
	u64 x;
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;		
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;//菜单从第0行开始


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//显示菜单
			
		    //默认显示测量点号为0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			if(Stack->MenuStack[MenuCount->Count].EditRow==2)
			{
				Stack->MenuStack[MenuCount->Count].EditRow=1;
				MenuCount->v1=9;
				MenuCount->v2=12;
				NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
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
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}					
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 2:
								//保存设置的数据
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,4);
								FlashLen = x;
								FlashLenAddr = ADDR_MP_START + (FlashLen * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
								FlashLen = sizeof(MeasurePointStruct) ;
								MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
								if((MeasurePoint.State == 0x01) && (BCD_Check(MeasurePoint.Addr, METER_ADDR_SIZE) == NO_ERROR))
								{
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;
									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}
								MenuCreate((u32)Menu_2_2_1);
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
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>MAX_PN_NUM)
								{
									bcd_p8(p8,9999,4);
								}
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
			Comm_Ram->ButtonVal&=~0x20;
			break;
		default:
			break;
	}
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_2_1
// Descriptions:        日冻结正向有功电能示值、组合无功1总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_2_1(void)//日冻结正向有功电能示值、组合无功1总电能示值
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" 历史日正向电表示数"
																	"     测量点   \n\r"
																	);//显示菜单

		
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(Ln+4,4,0,"测量点不存在");
	}
	else
	{	
		if(Pn_Lcd_Data->u16Pn<0x0A)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
		}
		else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
		}
		else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
		}
		else if(Pn_Lcd_Data->u16Pn>=0x3E8)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
		}
		
		DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
		}
		
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
			//Ret=QCSG_DecOneDay(&RealTime);
			//if(Ret==TRUE)
			//{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050601FF, &DataTime, PnData, &DataLen);
				//有功总
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+3,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功尖
				MR((u32)&Databuff,(u32)&PnData[5],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+4,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功峰
				MR((u32)&Databuff,(u32)&PnData[9],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+5,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功平
				MR((u32)&Databuff,(u32)&PnData[13],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+6,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功谷
				MR((u32)&Databuff,(u32)&PnData[17],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+7,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050603FF, &DataTime, PnData, &DataLen);
				//无功总
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+8,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}				
			//}
		}
	}		
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_2_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_2_2);//翻屛
	}

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0

}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_2_2
// Descriptions:        日冻结反向有功电能示值、组合无功2总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_2_2(void)//日冻结反向有功电能示值、组合无功2总电能示值
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" 历史日反向电表示数"
																	"     测量点    \n\r"
																	);//显示菜单	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
		
	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
		
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
		//Ret=QCSG_DecOneDay(&RealTime);
		//if(Ret==TRUE)
		//{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050602FF, &DataTime, PnData, &DataLen);
			//有功总
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+3,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功尖
			MR((u32)&Databuff,(u32)&PnData[5],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+4,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功峰
			MR((u32)&Databuff,(u32)&PnData[9],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+5,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功平
			MR((u32)&Databuff,(u32)&PnData[13],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+6,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功谷
			MR((u32)&Databuff,(u32)&PnData[17],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+7,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050604FF, &DataTime, PnData, &DataLen);
			//无功总
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+8,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}				
		//}
	}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_2_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3
// Descriptions:        历史月数据（选择测量点）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//历史月数据（选择测量点）
{
	u8 *p,*p8;
	u16 *p16;
	u32 j,Ln;
	u64 x;
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;		
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;//菜单从第0行开始


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//显示菜单
			
		  //默认显示测量点号为0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			if(Stack->MenuStack[MenuCount->Count].EditRow==2)
			{
				Stack->MenuStack[MenuCount->Count].EditRow=1;
				MenuCount->v1=9;
				MenuCount->v2=12;
				NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
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
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}					
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 2:
								//保存设置的数据
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,4);
								FlashLen = x;
								FlashLenAddr = ADDR_MP_START + (FlashLen * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
								FlashLen = sizeof(MeasurePointStruct) ;
								MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
								if((MeasurePoint.State == 0x01) && (BCD_Check(MeasurePoint.Addr, METER_ADDR_SIZE) == NO_ERROR))
								{
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;
									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}
								MenuCreate((u32)Menu_2_3_1);
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
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>MAX_PN_NUM)
								{
									bcd_p8(p8,9999,4);
								}
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
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3_1
// Descriptions:        月冻结正向有功电能示值、组合无功1总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_3_1(void)//月冻结正向有功电能示值、组合无功1总电能示值
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" 历史月正向电表示数"
																	"     测量点   \n\r"
																	);//显示菜单
		
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(Ln+4,4,0,"测量点不存在");
	}
	else
	{	
		if(Pn_Lcd_Data->u16Pn<0x0A)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
		}
		else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
		}
		else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
		}
		else if(Pn_Lcd_Data->u16Pn>=0x3E8)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
		}
		
		DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
		}
		
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
			//Ret=QCSG_DecOneMonth(&RealTime);
			//if(Ret==TRUE)
			//{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0001FF01, &DataTime, PnData, &DataLen);
				//有功总
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+3,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功尖
				MR((u32)&Databuff,(u32)&PnData[5],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+4,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功峰
				MR((u32)&Databuff,(u32)&PnData[9],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+5,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功平
				MR((u32)&Databuff,(u32)&PnData[13],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+6,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//有功谷
				MR((u32)&Databuff,(u32)&PnData[17],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+7,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0003FF01, &DataTime, PnData, &DataLen);
				//无功总
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+8,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}				
			//}
		}
	}		
		
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_3_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_3_2);//翻屛
	}

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3_2
// Descriptions:        月冻结反向有功电能示值、组合无功2总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_3_2(void)//月冻结反向有功电能示值、组合无功2总电能示值
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" 历史月反向电表示数"
																	"     测量点    \n\r"
																	);//显示菜单
	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
		
	DisplayClrRow(Ln+3,Ln+8);//清屏幕显示的指定行		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
		
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
		//Ret=QCSG_DecOneMonth(&RealTime);
		//if(Ret==TRUE)
		//{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0002FF01, &DataTime, PnData, &DataLen);
			//有功总
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+3,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功尖
			MR((u32)&Databuff,(u32)&PnData[5],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+4,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功峰
			MR((u32)&Databuff,(u32)&PnData[9],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+5,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功平
			MR((u32)&Databuff,(u32)&PnData[13],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+6,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//有功谷
			MR((u32)&Databuff,(u32)&PnData[17],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+7,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0004FF01, &DataTime, PnData, &DataLen);
			//无功总
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+8,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}				
		//}
	}
			
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_2_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_2_3_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2
// Descriptions:        电能计量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2(void)
{
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2);//显示菜单
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
							MenuCreate((u32)Menu_2_1);
							break;
						case 2:
							MenuCreate((u32)Menu_2_2);
							break;
						case 3:
							MenuCreate((u32)Menu_2_3);
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

