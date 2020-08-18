/*************************************************
版权:正泰仪表智能终端部
文件:Menu_1_RealTimeData.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单1实时数据处理文件
*************************************************/


//菜单1 实时数据
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu1.h"
#include "Menu3.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"





const u8 pMenu_1[]=
{
	"\n\r"
	"测量点号：\n\r"
	"        确认\n\r"
};


#if ((Project/100)==2)
const u8 JZQpMenu_1[]=
{
	"\n\r"
	"实时数据显示\n\r"
	"历史数据显示\n\r"
	"抄表统计数据\n\r"
	"异常告警\n\r"
};
#endif



const u8 pMenu_1_1_5[][20]=//电量列表
{
	"有功总         kWh",
	"有功尖         kWh",
	"有功峰         kWh",
	"有功平         kWh",
	"有功谷         kWh",
};


const u8 pMenu_1_2_1[]=
{
	"\n\r"
	"选择日期:\n\r"
	"        确认\n\r"
};


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1
// Descriptions:        南网集中器实时数据选择测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1(void)//南网集中器实时数据选择测量点
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln;	
	u32 FlashLenAddr = 0, FlashLen = 0;	
	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;		//菜单从第0行开始
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//显示菜单
			
		  //默认显示测量点号为0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
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
								MenuCreate((u32)Menu_1_1_1);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_1
// Descriptions:        测量点当前电压数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_1(void)//测量点当前电压数据
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	

	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"\n\r"
																	"      测量点   \n\r"
																	"电压(V):       \n\r"	
																	"  Ua/Uab =     \n\r"
																	"  Ub/Uac =     \n\r"
																	"  Uc/Ubc =     \n\r"
																	);//显示菜单
  if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(Ln+4,4,0,"测量点不存在");
//		if((Comm_Ram->ButtonVal&0x10)!=0)
//		{//确认键被按下返回
//			MenuDel();//返回上级菜单
//		}
//		Comm_Ram->ButtonVal&=~0x10;//键使用后清0
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
		//A相电压
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0201FF00[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,11,0,"---.-");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+11*2);
			DisplayData(Databuff,4,1,q16);
		}
		//B相电压
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0201FF00[2],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"---.-");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
			DisplayData(Databuff,4,1,q16);
		}
		//C相电压
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0201FF00[4],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+6,11,0,"---.-");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
			DisplayData(Databuff,4,1,q16);
		}
		
		if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
		{//右移
			CreateScreen((u32)Menu_1_1_2);//翻屛
		}
		if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
		{//左移
			CreateScreen((u32)Menu_1_1_6);//翻屛
		}
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
// Function name:       Menu_1_1_2
// Descriptions:        测量点当前电流数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_2(void)
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"\n\r"
																	"      测量点    \n\r"
																	"电流(A):        \n\r"
																	"  Ia =          \n\r"
																	"  Ib =          \n\r"
																	"  Ic =          \n\r"
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
	//A相电流
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0202FF00[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"---.---");
	}
	else
	{
		if((Databuff&0x800000) != 0)
		{
			Databuff &= 0x7FFFFF;
			DisplayString(Ln+4,7,0,"-");
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);
			DisplayData(Databuff,5,3,q16);
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
			DisplayData(Databuff,6,3,q16);
		}
	}
	//B相电流
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0202FF00[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,7,0,"---.---");
	}
	else
	{
		if((Databuff&0x800000) != 0)
		{
			Databuff &= 0x7FFFFF;
			DisplayString(Ln+5,7,0,"-");
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+8*2);
			DisplayData(Databuff,5,3,q16);
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
			DisplayData(Databuff,6,3,q16);
		}
		
//		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
//		DisplayData(Databuff,6,3,q16);
	}
	//C相电流
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0202FF00[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,7,0,"---.---");
	}
	else
	{
		if((Databuff&0x800000) != 0)
		{
			Databuff &= 0x7FFFFF;
			DisplayString(Ln+6,7,0,"-");
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+8*2);
			DisplayData(Databuff,5,3,q16);
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			DisplayData(Databuff,6,3,q16);
		}
//		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
//		DisplayData(Databuff,6,3,q16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_1);//翻屛
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
// Function name:       Menu_1_1_3
// Descriptions:        测量点当前有功功率、分相有功功率数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_3(void)//测量点当前有功功率、分相有功功率数据
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      测量点    \n\r"
																	"总有功功率(kW):\n\r"
																	"  P总 =        \n\r"
																	"分相有功功率(kW):\n\r"
																	"  Pa =         \n\r"
																	"  Pb =         \n\r"
																	"  Pc =         \n\r"
																	);//显示菜单
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	//总有功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,8,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+8*2);
		DisplayData(Databuff,6,4,q16);
	}
	//A相有功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//B相有功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//C相有功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_2);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_4
// Descriptions:        南网负控测量点当前无功功率、分相无功功率数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_4(void)//南网负控测量点当前无功功率、分相无功功率数据
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      测量点    \n\r"
																	"总无功功率(kvar):\n\r"
																	"  Q总 =         \n\r"
																	"分相无功功率(kvar):\n\r"
																	"  Qa =          \n\r"
																	"  Qb =          \n\r"
																	"  Qc =          \n\r"
																	);//显示菜单	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	//总无功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,8,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+8*2);
		DisplayData(Databuff,6,4,q16);
	}
	//A相无功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//B相无功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//C相无功功率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_5);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_4
// Descriptions:        南网集中器测量点当前功率因数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_4(void)//南网集中器测量点当前功率因数
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      测量点    \n\r"
																	"总功率因数:     \n\r"
																	"  COSφ总 =      \n\r"
																	"分相功率因数:\n\r"
																	"  COSφa =       \n\r"
																	"  COSφb =       \n\r"
																	"  COSφc =       \n\r"
																	);//显示菜单
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	
	//总功率因数
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+3,12,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+12*2);
		DisplayData(Databuff,4,3,q16);
	}
	//A相功率因数
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[2],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,4,3,q16);
	}
	//B相功率因数
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[4],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,11,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,4,3,q16);
	}
	//C相功率因数
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[6],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,4,3,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_5);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_3);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	
#endif


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_5
// Descriptions:        南网负控测量点当前相角数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_5(void)//南网负控测量点当前相角数据
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      测量点    \n\r"
																	"电压电流相位角(度):\n\r"
																	"  Uab/Ua =      \n\r"
																	"  Ub =          \n\r"
																	"  Uc =          \n\r"
																	"  Ia =          \n\r"
																	"  Ib =          \n\r"
																	"  Ic =          \n\r"
																	);//显示菜单
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	
	//Uab/Ua
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+3,11,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+11*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ub
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[2],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Uc
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[4],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ia
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[6],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ib
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[8],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ic
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[10],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+8,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_6);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_5
// Descriptions:        南网集中器测量点正向有功总电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_5(void)//南网集中器测量点正向有功总电能示值
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"\n\r"
																			"  正向有功电能示值  "
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
	
	DisplayClrRow(Ln+3,Ln+7);//清屏幕显示的指定行		
	for(i=0;i<=4;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_1_1_5[i]);
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
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_6);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_4);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	
#endif


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_6
// Descriptions:        南网负控测量点当前功率因数、频率数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_6(void)//南网负控测量点当前功率因数、频率数据
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"\n\r"
																	"      测量点    \n\r"
																	"功率因数:    \n\r"
																	"  PF =        \n\r"
																	"频率(HZ):    \n\r"
																	"  f =         \n\r"
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
	
	//总功率因数
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,7,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,4,3,q16);
	}
	//频率
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data02800002[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,6,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,4,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_5);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_6
// Descriptions:        南网集中器测量点反向有功电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_6(void)//南网集中器测量点反向有功电能示值
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"\n\r"
																			"  反向有功电能示值  "
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
	
	DisplayClrRow(Ln+3,Ln+7);//清屏幕显示的指定行		
	for(i=0;i<=4;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_1_1_5[i]);
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
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_1_1_1);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_1_1_5);//翻屛
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}	
#endif


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2
// Descriptions:        历史数据显示（选择测量点）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2(void)
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln=0;	   //菜单从第0行开始
	u32 FlashLenAddr = 0, FlashLen = 0;	
	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//显示菜单
			
		    //默认显示测量点号为0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
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
								MenuCreate((u32)Menu_1_2_1);
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
// Function name:       Menu_1_2_1
// Descriptions:        历史数据显示(选择日期)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;//菜单从第0行开始
	
	
	MenuCount->v1=0;
	MenuCount->v2=0;
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(Ln+4,4,0,"测量点不存在");
	}
	else
	{
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				DisplayClr();//清全屏
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_1);//显示菜单
				
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
									Menu_1_2_1_1(&RealTime);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_1_1
// Descriptions:        历史日正向有功电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_1_1(RealTimeStruct *Time)
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayClr();//清全屏
	DisplayString(2,6,0,"测量点");
	DisplayString(3,3,0,"正向有功总电能");
	DisplayString(4,1,0,"有功总         kWh");
	DisplayString(5,3,0,"反向有功总电能");
	DisplayString(6,1,0,"有功总         kWh");
	
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
	
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050601FF, &DataTime, PnData, &DataLen);
		//正向有功总
		MR((u32)&Databuff,(u32)&PnData[1],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+4,7,0,"------.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
			DisplayData(Databuff,8,2,q16);
		}
		
		//反向有功总
		NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050602FF, &DataTime, PnData, &DataLen);		
		MR((u32)&Databuff,(u32)&PnData[1],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+6,7,0,"------.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+7*2);
			DisplayData(Databuff,8,2,q16);
		}
	//}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3
// Descriptions:        抄表统计数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_3(void)
{
	u8 AmrInfo[8];
	u16 *p;
	u32 x = 0;
	
	DisplayClr();//清全屏
	Get_MeterAMRSuccessInfo(AmrInfo);
	DisplayString(1,4,0,"抄表统计数据");
	
	DisplayString(2,0,0,"日正向有功成功:");
	p=(u16*)(ADDR_STRINGBUFF+2*84+4+15*2);
	
	x = AmrInfo[0];
	x += AmrInfo[1] << 8;
	bcd_p8((u8*)p,x,4);
	
	DisplayString(3,0,0,"日正向有功失败:");
	p=(u16*)(ADDR_STRINGBUFF+3*84+4+15*2);
	x = AmrInfo[2];
	x += AmrInfo[3] << 8;
	bcd_p8((u8*)p,x,4);
	
	DisplayString(4,0,0,"月正向有功成功:");
	p=(u16*)(ADDR_STRINGBUFF+4*84+4+15*2);
	x = AmrInfo[4];
	x += AmrInfo[5] << 8;
	bcd_p8((u8*)p,x,4);
	
	DisplayString(5,0,0,"月正向有功失败:");
	p=(u16*)(ADDR_STRINGBUFF+5*84+4+15*2);
	x = AmrInfo[6];
	x += AmrInfo[7] << 8;
	bcd_p8((u8*)p,x,4);

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0	
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4
// Descriptions:        异常告警
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_4(void)
{
	Menu_6();
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        南网负控实时数据选择测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//南网负控实时数据选择测量点
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln;	
	u32 FlashLenAddr = 0, FlashLen = 0;
	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;		//菜单从第0行开始

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//显示菜单
			
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
								MenuCreate((u32)Menu_1_1_1);
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
#endif



#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        南网集中器数据查询主菜单
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//南网集中器数据查询主菜单
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_1);//显示菜单
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
						MenuCreate((u32)Menu_1_1);//实时数据显示
						break;
					case 2:
						MenuCreate((u32)Menu_1_2);//历史数据显示							
						break;
					case 3:							
						MenuCreate((u32)Menu_1_3);//抄表统计数据							
						break;
					case 4:
						MenuCreate((u32)Menu_1_4);//异常告警
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
#endif
