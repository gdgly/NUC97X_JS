/*************************************************
版权:正泰仪表智能终端部
文件:Menu_8_TerminalManagement.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单8终端管理与维护处理文件
*************************************************/

#include <string.h>

//菜单8 终端管理与维护
#include "../Hcsg/ProjectCSG.h"
#include "../Display/DisplayLoop.h"
#include "../Display/DisplayLoop_Warning.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../Display/Display.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"

#include "Menu.h"
#include "Menu8.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Device/RTC.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../QCSG_Head/QCSG_Hal.h"




const u8 pMenu_8[]=
{
	"终端管理与维护\n\r"
	"重启终端\n\r"
	"液晶调节\n\r"
	"状态查看\n\r"
	"信号强度和电池电量\n\r"
	"无线实时状态\n\r"
	"终端时间设置\n\r"
	"界面密码设置\n\r"
	"测试通道设置\n\r"
	"最大需量清零\n\r"
	"终端编号\n\r"
	"终端版本\n\r"
};

#if ((Project/100)==2)
const u8 JZQpMenu_8[]=
{
	"终端管理与维护\n\r"
	"重启终端\n\r"
	"液晶调节\n\r"
	"信号强度和电池电量\n\r"
	"无线实时状态\n\r"
	"终端时间设置\n\r"
	"界面密码设置\n\r"
	"测试通道设置\n\r"
	"终端编号\n\r"
	"终端版本\n\r"
};
#endif

const u8 pMenu_8_1[]=
{
	"  \n\r"
	"终端硬件复位\n\r"
	"数据区复位\n\r"
	"恢复出厂设置\n\r"
};

const u8 pMenu_8_2[]=
{
	"\n\r"
	"开启背光\n\r"
	"关闭背光\n\r"
	"对比度增加\n\r"
	"对比度降低\n\r"
};


const u8 pMenu_8_3[]=
{
	"\n\r"
	"测量点状态信息\n\r"
	"终端状态信息\n\r"
};


const u8 pMenu_8_3_1[]=
{
	"\n\r"
	"测量点号：\n\r"
	"        确认\n\r"
};

const u8 pMenu_8_3_2[]=
{
	"\n\r"
	"遥信状态\n\r"
	"门节点状态\n\r"
	"停电统计\n\r"
};


const u8 pMenu_8_4[]=
{
	"  信号强度和电池电量\n\r"
	
	"信号强度：\n\r"
	"停电电池电压：\n\r"
	"停电电池电量：\n\r"
	"时钟电池电量：\n\r"
};


const u8 pMenu_8_6[]=
{
	"\n\r"
	"日期:\n\r"
	"时间:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_7[]=
{
	"\n\r"
	"界面密码:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_8[]=
{
	"\n\r"
	"485端口1\n\r"
	"485端口2\n\r"
	"485端口3\n\r"
};


const u8 pMenu_8_8_1[]=
{
	"\n\r"
	"\n\r"
	"功能:\n\r"	
	"波特率:\n\r"
	"校验方式:\n\r"
	"数据位:\n\r"
	"停止位:\n\r"
	"      确认设置\n\r"
};


const u8 pMenu_8_10[]=
{
	"\n\r"
	"行政区码设置\n\r"
	"逻辑地址设置\n\r"
};

const u8 pMenu_8_10_1[]=
{
	"\n\r"
	"行政区码:\n\r"
	"      确认设置\n\r"
};


const u8 pMenu_8_10_2[]=
{
	"\n\r"
	"逻辑地址:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_11[]=
{
	"\n\r"
	"      终端信息\n\r"
	"  厂商代码：\n\r"
	"  软件版本：\n\r"
	"  发布日期：\n\r"
	"  硬件版本：\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1
// Descriptions:        重启终端
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_1(void)//重启终端
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_1);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. 终端硬件复位
						MenuCreate((u32)Menu_8_1_1);//创建下级菜单
						break;
					case 2://2. 数据区复位
						MenuCreate((u32)Menu_8_1_2);//创建下级菜单
						break;
					case 3://3. 恢复出厂设置
						MenuCreate((u32)Menu_8_1_3);//创建下级菜单
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
// Function name:       Menu_8_1_1
// Descriptions:        终端硬件复位
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_1_1(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"终端重启");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES 终端硬件复位
					DataResetAction(0x02,GUI_PORT);
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1_2
// Descriptions:        数据区复位
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_1_2(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,5,0,"数据初始化");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES 数据区初始化
					DataResetAction(0x01,GUI_PORT);				
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1_3
// Descriptions:        恢复出厂设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_1_3(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"恢复出厂配置");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES 恢复出厂配置
					DataResetAction(0x00,GUI_PORT);		
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_2
// Descriptions:        液晶调节
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_2(void)
{	
	if((Comm_Ram->ButtonVal&0x2F)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2F;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3
// Descriptions:        状态查看
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3);//显示菜单	
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_3_1);//测量点状态信息
						break;
					case 2:
						MenuCreate((u32)Menu_8_3_2);//终端状态信息
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
// Function name:       Menu_8_3_1
// Descriptions:        测量点状态信息（选择测量点）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln=0;		//菜单从第0行开始
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3_1);//显示菜单
			
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
									//MC(0,(u32)Pn_Lcd_Data,sizeof(QCSG_LCD_RAMDATA_S));
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;								
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}								
								MenuCreate((u32)Menu_8_3_1_1);
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
// Function name:       Menu_8_3_1_1
// Descriptions:        测量点状态信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3_1_1(void)//测量点状态信息
{
	u16 *p16;
	u32 Ln = 0,Databuff;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	
	DisplayClr();//清全屏
	DisplayString(1,6,0,"测量点");
	DisplayString(2,0,0,"电压断相:否 否 否");
	DisplayString(3,0,0,"电流极性:正 正 正");
	DisplayString(4,0,0,"电压逆向序:否    ");
	DisplayString(5,0,0,"电流逆向序:否    ");
  if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,5);//清屏幕显示的指定行
		DisplayString(Ln+4,4,0,"测量点不存在");
	}
	else
	{
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

		//电压断相(A相断相)
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[6],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(2,9,0,"否");
		}
		else
		{
			Databuff &= 0x8000;
			if(Databuff == 0x8000)
			{
				DisplayString(2,9,0,"是");
			}
			else
			{
				DisplayString(2,9,0,"否");
			}		
		}
		//B相断相
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(2,12,0,"否");
		}
		else
		{
			Databuff &= 0x8000;
			if(Databuff == 0x8000)
			{
				DisplayString(2,12,0,"是");
			}
			else
			{
				DisplayString(2,12,0,"否");
			}		
		}
		//C相断相
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(2,15,0,"否");
		}
		else
		{
			Databuff &= 0x8000;
			if(Databuff == 0x8000)
			{
				DisplayString(2,15,0,"是");
			}
			else
			{
				DisplayString(2,15,0,"否");
			}		
		}
		//ABC相电流极性(A相电流极性)
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[6],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(3,9,0,"正");
		}
		else
		{
			Databuff &= 0x4000;
			if(Databuff == 0x4000)
			{
				DisplayString(3,9,0,"反");
			}
			else
			{
				DisplayString(3,9,0,"正");
			}		
		}
		//B相电流极性
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(3,12,0,"正");
		}
		else
		{
			Databuff &= 0x4000;
			if(Databuff == 0x4000)
			{
				DisplayString(3,12,0,"反");
			}
			else
			{
				DisplayString(3,12,0,"正");
			}		
		}
		//C相电流极性
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(3,15,0,"正");
		}
		else
		{
			Databuff &= 0x4000;
			if(Databuff == 0x4000)
			{
				DisplayString(3,15,0,"反");
			}
			else
			{
				DisplayString(3,15,0,"正");
			}		
		}
		//电压逆向序
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[12],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(4,0,0,"电压逆向序:否        ");
		}
		else
		{
			Databuff &= 0x0100;
			if(Databuff == 0x0100)
			{
				DisplayString(4,0,0,"电压逆向序:是        ");
			}
			else
			{
				DisplayString(4,0,0,"电压逆向序:否        ");
			}		
		}
		//电流逆向序
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[12],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(5,0,0,"电流逆向序:否        ");
		}
		else
		{
			Databuff &= 0x0200;
			if(Databuff == 0x0200)
			{
				DisplayString(5,0,0,"电流逆向序:是        ");
			}
			else
			{
				DisplayString(5,0,0,"电流逆向序:否        ");
			}		
		}
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2
// Descriptions:        终端状态信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3_2);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_3_2_1);//遥信状态
						break;
					case 2:
						MenuCreate((u32)Menu_8_3_2_2);//门节点状态					
						break;
					case 3:							
						MenuCreate((u32)Menu_8_3_2_3);//停电统计		
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
// Function name:       Menu_8_3_2_1
// Descriptions:        遥信状态
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2_1(void)
{
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//清全屏
	DisplayString(1,6,0,"遥信状态");
	DisplayString(2,0,0,"4路遥信状态:");
	
	//遥信状态
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataTerminalStateWord[0],1);
	if(Databuff==0xFF)
	{
		DisplayString(Ln+3,0,0,"分 分 分 分");
	}
	if((Databuff&0x08)==0x08)
	{
		DisplayString(Ln+3,0,0,"合");
	}
	if((Databuff&0x08)==0)
	{
		DisplayString(Ln+3,0,0,"分");
	}
	if((Databuff&0x10)==0x10)
	{
		DisplayString(Ln+3,3,0,"合");
	}
	if((Databuff&0x10)==0)
	{
		DisplayString(Ln+3,3,0,"分");
	}
	if((Databuff&0x20)==0x20)
	{
		DisplayString(Ln+3,6,0,"合");
	}
	if((Databuff&0x20)==0)
	{
		DisplayString(Ln+3,6,0,"分");
	}
	if((Databuff&0x40)==0x40)
	{
		DisplayString(Ln+3,9,0,"合");
	}
	if((Databuff&0x40)==0)
	{
		DisplayString(Ln+3,9,0,"分");
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2_2
// Descriptions:        门节点状态
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2_2(void)
{
	u32 Ln = 0, Databuff = 0;   //菜单从第0行开始
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//清全屏
	DisplayString(1,5,0,"门节点状态");
	DisplayString(2,0,0,"门节点状态:");
	
	//门节点状态
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataTerminalStateWord[0],1);
	if(Databuff==0xFF)
	{
		DisplayString(Ln+2,11,0,"闭");
	}
	if((Databuff&0x01)==1)
	{
		DisplayString(Ln+2,11,0,"开");
	}
	if((Databuff&0x01)==0)
	{
		DisplayString(Ln+2,11,0,"闭");
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2_3
// Descriptions:        停电统计
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2_3(void)
{
	Menu_4();
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_4
// Descriptions:        信号强度和电池电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_4(void)
{
	u8 *p8;
	u32 x,SignaldBm;
	
	DisplayClr();//清全屏
	DisplayString(1,1,0,"信号强度和电池电量");
	DisplayString(3,0,0,"信号强度:            ");
	DisplayString(4,0,0,"备用电池电量:        ");
	#if (Project/100)==3 || ((Project/100)==9)
		DisplayString(5,0,0,"时钟电池电量:        ");
	#endif
	
	SignaldBm = Get_GPRSSignaldBm();                //获取信号强度
	if(SignaldBm == 0x99)	
  {
		DisplayString(3,9,0,"0");
	}
	else
	{
		p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+9*2);
		DisplayData(SignaldBm,2,0,(u16*)p8);
	}
	
	
	//电池电量
	x=Comm_Ram->UBACKUP;                    
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		x=bcd_hex(x);
		if(x>=20)
		{
			x-=20;//电网供电正在充电-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	if(x==0)
	{
		DisplayString(4,13,0,"0%");
	}
	if((x>0)&&(x<=RechargeableBatteryGrid1))
	{
		DisplayString(4,13,0,"25%");
	}
	if(x>RechargeableBatteryGrid1)
	{
	  DisplayString(4,13,0,"50%");
	}
	if(x>RechargeableBatteryGrid2)
	{
	  DisplayString(4,13,0,"75%");
	}
	if(x>RechargeableBatteryGrid3)
	{
	  DisplayString(4,13,0,"100%");
	}
		
	#if (Project/100)==3 || ((Project/100)==9)
		//时钟电池电量
		if(Comm_Ram->URTC<0x200)
		{//电压小于2.00V
			DisplayString(5,13,0,"0%");
		}
		if((Comm_Ram->URTC >= 0x200)&&(Comm_Ram->URTC < 0x240))
		{//电压在2.00V~2.40V之间
			DisplayString(5,13,0,"25%");
		}
		if((Comm_Ram->URTC >= 0x240)&&(Comm_Ram->URTC < 0x280))
		{//电压在2.40V~2.80V之间
			DisplayString(5,13,0,"50%");
		}
		if((Comm_Ram->URTC >= 0x280)&&(Comm_Ram->URTC < 0x320))
		{//电压在2.80V~3.20V之间
			DisplayString(5,13,0,"75%");
		}
		if((Comm_Ram->URTC >= 0x320)&&(Comm_Ram->URTC < 0x380))
		{//电压在3.20V~3.80V之间
			DisplayString(5,13,0,"100%");
		}
	#endif
		
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回 
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_5
// Descriptions:        无线实时状态
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_5(void)
{
	u16 *p16;
	u32 State;	
	u8 IPAddr[16];
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
	
	DisplayClr();//清全屏
	DisplayString(1,4,0,"无线实时状态");
	DisplayString(2,0,0,"当前状态:            ");
	DisplayString(4,0,0,"IP:                  ");//183.247.151.178
	DisplayString(5,0,0,"运营商:    ");
	DisplayString(6,0,0,"连接类型:      ");

		
	
	State=Get_GPRSState(GPRSPORT);
	
	if(State==GPRS_STATE_UNPLUGGED)             	//0
	{
		DisplayString(2,9,0,"正在初始化...");
	}
	else if(State==GPRS_STATE_POWER_OFF)             //1
	{
		DisplayString(2,9,0,"GPRS正在关机...");
	}
	else if(State==GPRS_STATE_POWER_ON)              //2
	{
		DisplayString(2,9,0,"GPRS正在开机...");
	}
	else if(State==GPRS_STATE_MATCH_BAUD_RATE)       //3
	{
		DisplayString(2,9,0,"正在匹配波特率...");
	}
	else if(State==GPRS_STATE_RECONGNIZE_SIM_CARD)   //4
	{
		DisplayString(2,9,0,"正在识别SIM卡...");
	}
	else if(State==GPRS_STATE_REG_NET)               //5
	{
		DisplayString(2,9,0,"正在搜索网络...");
	}
	else if(State==GPRS_STATE_CONFIG_LINK_PARA)      //6
	{
		DisplayString(2,9,0,"正在配置参数...");
	}
	else if(State==GPRS_STATE_LINKING)               //7
	{
		DisplayString(2,9,0,"正在连接...");
	}
	else if(State==GPRS_STATE_LINK_FAIL)             //8
	{
		DisplayString(2,9,0,"连接失败");
	}
	else if(State==GPRS_STATE_LINK_SUCCESS)          //9
	{
		Get_GPRSLocalIPAddr(IPAddr);
		DisplayString(2,9,0,"连接成功");
		p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+3*2);
		p16[0]=0x2000 + IPAddr[0];
		p16[1]=0x2000 + IPAddr[1];
		p16[2]=0x2000 + IPAddr[2];
		p16[3]=0x2000 + IPAddr[3];
		p16[4]=0x2000 + IPAddr[4];
		p16[5]=0x2000 + IPAddr[5];
		p16[6]=0x2000 + IPAddr[6];
		p16[7]=0x2000 + IPAddr[7];
		p16[8]=0x2000 + IPAddr[8];
		p16[9]=0x2000 + IPAddr[9];
		p16[10]=0x2000 + IPAddr[10];
		p16[11]=0x2000 + IPAddr[11];
		p16[12]=0x2000 + IPAddr[12];
		p16[13]=0x2000 + IPAddr[13];
		p16[14]=0x2000 + IPAddr[14];
		p16[15]=0x2000 + IPAddr[15];
	}
	else if(State==GPRS_STATE_LOG_SUCCESS)           //10
	{
		Get_GPRSLocalIPAddr(IPAddr);
		DisplayString(2,9,0,"登陆主站成功");
		p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+3*2);
		p16[0]=0x2000 + IPAddr[0];
		p16[1]=0x2000 + IPAddr[1];
		p16[2]=0x2000 + IPAddr[2];
		p16[3]=0x2000 + IPAddr[3];
		p16[4]=0x2000 + IPAddr[4];
		p16[5]=0x2000 + IPAddr[5];
		p16[6]=0x2000 + IPAddr[6];
		p16[7]=0x2000 + IPAddr[7];
		p16[8]=0x2000 + IPAddr[8];
		p16[9]=0x2000 + IPAddr[9];
		p16[10]=0x2000 + IPAddr[10];
		p16[11]=0x2000 + IPAddr[11];
		p16[12]=0x2000 + IPAddr[12];
		p16[13]=0x2000 + IPAddr[13];
		p16[14]=0x2000 + IPAddr[14];
		p16[15]=0x2000 + IPAddr[15];	
	}
	else
	{
		DisplayString(2,9,0,"            ");
		DisplayString(4,3,0,"                  ");	
		DisplayString(5,7,0,"    ");	
		DisplayString(6,9,0,"      ");		
	}	
	switch(pComm_Ram->CSPN)
	{
		case 0x01:
			DisplayString(5,7,0,"移动");
			break;
		case 0x02:
			DisplayString(5,7,0,"联通");
			break;
		case 0x03:
			DisplayString(5,7,0,"电信");
			break;
		default:
			DisplayString(5,7,0,"其他");
		break;	
	}
	
	switch(pTerminal_Ram->LinkCount_SelfAdaption)
	{
		case 0x03:
			DisplayString(6,9,0,"普通卡");
			break;
		case 0x04:
			DisplayString(6,9,0,"物联卡");
			break;
		case 0x00:
			DisplayString(6,9,0,"主用IP");//修改为从主用1开始连接
			break;
		case 0x01:
			DisplayString(6,9,0,"备用1");
			break;
		case 0x02:
			DisplayString(6,9,0,"备用2");
			break;
		default:
			DisplayString(6,9,0,"未知");
		break;	
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回 
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_6
// Descriptions:        终端时间设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_6(void)
{
#if ((Project/100) == 3)
	u8 u8PnNum = 0,ii = 0;
	u8 u8MeterId[6];
	u32 u32RevLen = 0;
	ERD09Struct stERD09Data;
	RealTimeYYStruct stYYTime;
	TIME_PARAM_S pstTime;			
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
#endif
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,k,Ln;
	Ln=0;//菜单从第0行开始

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_6);//显示菜单
			
			//日期
			p8=(u8 *)ADDR_MYMDHMS;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
			p[0*2]=(p8[5]>>4)+0x30;//年
			p[1*2]=(p8[5]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[4]>>4)+0x30;//月
			p[4*2]=(p8[4]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[3]>>4)+0x30;//日
			p[7*2]=(p8[3]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
			//时间
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
			p[0*2]=(p8[2]>>4)+0x30;//时
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]=':';
			p[3*2]=(p8[1]>>4)+0x30;//分
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]=':';
			p[6*2]=(p8[0]>>4)+0x30;//秒
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
			
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头,同时清掉其他行的左右箭头
			MenuCount->v1=6;
			MenuCount->v2=13;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=3)
					{
						DisplayString(Ln+3,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头,同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头,同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+Stack->MenuStack[MenuCount->Count].EditRow))
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 3:
								//保存设置的数据
								p8=(u8 *)(ADDR_RTCBUFF);
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								p8[4]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
								p8[5]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
								p8[6]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//秒
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//分
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//时							
								WriteRTC(ADDR_RTCBUFF);//写实时钟
								
								DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+3,9,0,"OK");

								
								//err20181107 增加事件 负控 终端校时记录事件 液晶修改时间
								#if ((Project/100) == 3)
									

									HAL_GetSystemTime(&pstTime);
									
									if(QCSG_CheckEventMask(0xE2010010) == TRUE)
									{
										u8PnNum = pstPn_Ram_Data->u8PnNum;
										for(ii = 0;ii < u8PnNum;i++)
										{
											//寻找内存中记录的测量点数据信息
											if(0 == pstPn_Ram_Data->stPn_Ram_Data[ii].u16Pn)
											{
												pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[ii];
												break;
											}
										}	
										if(ii >= u8PnNum)
										{
											//Result = ERROR_VerifyFail;
											return;//break;
										}	
										MemCopy(&stERD09Data.Time,&pstTime,sizeof(TIME_PARAM_S));
										MemCopy(stERD09Data.C_00010000,pstRam_Data->u8Data00010000,4);
										MemCopy(stERD09Data.C_00020000,pstRam_Data->u8Data00020000,4);

										GetACSampleAddr(u8MeterId);	//取交采地址
										//记录事件
										MemSet(&stYYTime,0,sizeof(stYYTime));
										//RD_Time_TO_YYTime(&pstTime,&stYYTime);//转换时间格式
										stYYTime.YearH = 0x20;
										stYYTime.YearL = pstTime.u8Year;
										stYYTime.Month = pstTime.u8Month;
										stYYTime.Day = pstTime.u8Day;
										stYYTime.Hour = pstTime.u8Hour;
										stYYTime.Minute = pstTime.u8Minute;
										
										u32RevLen = sizeof(stERD09Data);
										NAND_WriteEventRecord(u8MeterId,0xE2010010,&stYYTime,(u8*)&stERD09Data,&u32RevLen);
									}
								#endif
								
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
						if(Stack->MenuStack[MenuCount->Count].EditRow<3)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
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
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								if((p8[0*2]-0x30)*10+(p8[1*2]-0x30)*1>23)
								{
									p8[0*2]=0x30;
									p8[1*2]=0x30;
								}
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>59)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x30;
								}
								if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>59)
								{
									p8[7*2]=0x30;
									p8[6*2]=0x30;
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
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_7
// Descriptions:        界面密码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_7(void)
{	
	u8 *p,*p8;
	u16 *p16;
	u32 *p32;
	u32 x,j,Ln=0;//菜单从第0行开始
	

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_7);//显示菜单
				
			//显示内存中数据
			p8=(u8*)ADDR_DATABUFF;		
			MR(ADDR_DATABUFF,ADDR_GUIPASSWORD,3);			
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p32=(u32*)p8;			
			bcd_p8(p,p32[0]&0x00FFFFFF,6);
					
			MenuCount->v1=9;
			MenuCount->v2=14;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(Ln+2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
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
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,6);
								p8=(u8*)&x;
								MW((u32)p8,ADDR_GUIPASSWORD,3);							
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,6,0,"   OK   ");
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
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
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8
// Descriptions:        测试通道设置（选择端口号）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_8(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://485端口1
						MenuCreate((u32)Menu_8_8_1);//创建下级菜单           
						break;
					case 2://485端口2
						MenuCreate((u32)Menu_8_8_2);//创建下级菜单
						break;
					case 3://485端口3
						MenuCreate((u32)Menu_8_8_3);//创建下级菜单
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



u32 ConvertRs485PortParaToBitValue(u32 Func, u32 BaudRate, u32 Parity, u32 DataBit, u32 StopBit)
{
	u32 BitValue = 0, BaudRateValue = 0, i = 0;

	BitValue |= (Func & 0x00000007);
	
	BaudRateValue = BaudRate;
	BaudRateValue *= 300;
	for(i = 0; i < MAX_BAUD_NUM; i++)
	{
		if(UartBaudRate[i] == BaudRateValue) break;
	}
	if(i >= MAX_BAUD_NUM) i = BAUD_RATE_1200_ID;

	BitValue |= (i & 0x0000000F)<< 3;

	BitValue |= (Parity & 0x00000003) << 7;

	BitValue |= (DataBit & 0x0000000F) << 9;
	
	BitValue |= (StopBit & 0x00000003) << 13;
	return BitValue;
}

void ConvertBitValueToRs485PortPara(u8* Func, u8* BaudRate, u8* Parity, u8* DataBit, u8* StopBit, u32 BitValue)
{
	u32 i = 0;

	i = BitValue & RS485_FUNCTION_BIT_MASK;
	Func[0] = (u8)i;

	i = BitValue & RS485_BAUD_RATE_BIT_MASK;
	i >>= 3;
	if(i >= MAX_BAUD_NUM) i = BAUD_RATE_1200_ID;

	BaudRate[0] = UartBaudRate[i]/300;

	i = BitValue & RS485_PARITY_BIT_MASK;
	i >>= 7;
	Parity[0] = (u8)i;

	i = BitValue & RS485_DATA_BIT_MASK;
	i >>= 9;
	DataBit[0] = (u8)i;

	i = BitValue & RS485_STOP_BIT_MASK;
	i >>= 13;
	StopBit[0] = (u8)i;
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8_1
// Descriptions:        485端口1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_8_1(void)
{
	u8 *p,*p8;	
	u64 x;
	u32 j, FlashAddr,Ln = 0;   //菜单从第0行开始
	RS485PortParaStruct RS485PortPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8_1);//显示菜单
			DisplayString(Ln+1,5,0,"RS485端口 1");			
		
			FlashAddr = ADDR_RS485PortParaTbl;
			MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortParaStruct));	
			//显示内存中的数据
			//端口功能
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Function,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			DisplayData(p8[0],2,0,(u16*)p);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+2,5,0,"485抄表口    ");			
					break;
				case 1:
					DisplayString(Ln+2,5,0,"485总线级联  ");			
					break;
				case 2:
					DisplayString(Ln+2,5,0,"485被抄      ");			
					break;
				case 3:
					DisplayString(Ln+2,5,0,"485上传维护口");		
					break;
				default:
					DisplayString(Ln+2,5,0,"485抄表口    ");
					break;
			}
			//通信波特率
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.BaudRate,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0x01:
					DisplayString(Ln+3,7,0,"300");				
					break;
				case 0x02:
					DisplayString(Ln+3,7,0,"600");				
					break;
				case 0x04:
					DisplayString(Ln+3,7,0,"1200");				
					break;
				case 0x08:
					DisplayString(Ln+3,7,0,"2400");				
					break;
				case 0x10:
					DisplayString(Ln+3,7,0,"4800");				
					break;
				case 0x20:
					DisplayString(Ln+3,7,0,"9600");
					break;
				case 0x40:
					DisplayString(Ln+3,7,0,"19200");
					break;
				case 0x80:
					DisplayString(Ln+3,7,0,"38400");
					break;
				case 0xC0:
					DisplayString(Ln+3,7,0,"57600");
					break;
				default:
					DisplayString(Ln+3,7,0,"1200");	
					break;	
			}							
			//校验方式
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Parity,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+4,9,0,"无校验");			
					break;
				case 1:
					DisplayString(Ln+4,9,0,"偶校验");		
					break;
				case 2:
					DisplayString(Ln+4,9,0,"奇校验");			
					break;
				default:
					DisplayString(Ln+4,9,0,"偶校验");
					break;
			}				
			//数据位
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.DataBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);	
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 5:
					DisplayString(Ln+5,7,0,"5 位");			
					break;
				case 6:
					DisplayString(Ln+5,7,0,"6 位");		
					break;
				case 7:
					DisplayString(Ln+5,7,0,"7 位");			
					break;
				case 8:
					DisplayString(Ln+5,7,0,"8 位");		
					break;
				default:
					DisplayString(Ln+5,7,0,"8 位");
					break;
			}	
			//停止位
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.StopBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+6,7,0," 1  位");			
					break;
				case 1:
					DisplayString(Ln+6,7,0,"1.5 位");		
					break;
				case 2:
					DisplayString(Ln+6,7,0," 2  位");			
					break;
				default:
					DisplayString(Ln+6,7,0," 1  位");
					break;
			}	
			pMenuCount->v3 = ConvertRs485PortParaToBitValue(RS485PortPara.Function, RS485PortPara.BaudRate, 
				               RS485PortPara.Parity, RS485PortPara.DataBit, RS485PortPara.StopBit);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)  //编辑列为0的时候
				{
					BoxSelect();  //编辑框选择				
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=10;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑端口功能
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485上传维护口");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485抄表口    ");
								}
								break;
							case 3:		// 编辑端口波特率
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// 编辑端口检验方式
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								x++;
								if(x > UART_PARITY_ODD) x = UART_PARITY_NONE;
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"无校验");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"偶校验");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"奇校验");
								}
								break;
							case 5:		// 编辑端口数据位
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								x++;
								if(x > UART_DATA_BIT_8) x = UART_DATA_BIT_5;
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 位");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 位");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 位");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 位");
								}
								break;
							case 6:		// 编辑端口停止位
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								x++;
								if(x > UART_STOP_BIT_2) x = UART_STOP_BIT_1;
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  位");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 位");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  位");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑端口功能
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485上传维护口");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485抄表口    ");
								}
								break;
							case 3:		// 编辑端口波特率
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								if(x == 0)
								{
									x = BAUD_RATE_9600_ID;
								}
								else
								{
									x--;
									if(x == BAUD_RATE_7200_ID) x = BAUD_RATE_4800_ID;
								}							
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// 编辑端口检验方式
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								if(x == UART_PARITY_NONE)
								{
									x = UART_PARITY_ODD;
								}
								else
								{
									x--;
								}
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_NONE << 7);
									DisplayString(4,9,0,"无校验");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_EVEN << 7);
									DisplayString(4,9,0,"偶校验");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_ODD << 7);
									DisplayString(4,9,0,"奇校验");
								}
								break;
							case 5:		// 编辑端口数据位
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								if(x == UART_DATA_BIT_5) 
								{
									x = UART_DATA_BIT_8;
								}
								else
								{
									x--;
								}
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 位");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 位");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 位");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 位");
								}
								break;
							case 6:		// 编辑端口停止位
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								if(x == UART_STOP_BIT_1)
								{
									x = UART_STOP_BIT_2;
								}
								else
								{
									x--;
								}
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  位");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 位");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  位");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								//保存设置的数据					
								//端口功能
								FlashAddr = ADDR_RS485PortParaTbl;
								MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
								ConvertBitValueToRs485PortPara(&RS485PortPara.Function, &RS485PortPara.BaudRate, &RS485PortPara.Parity, 
									                           &RS485PortPara.DataBit, &RS485PortPara.StopBit, pMenuCount->v3);
								MW((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
										
								DisplayClrRow_4(Ln+7);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+7,9,0,"OK");																					
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
// Function name:       Menu_8_8_2
// Descriptions:        485端口2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_8_2(void)
{
	u8 *p,*p8;
	u64 x;	
	u32 j, FlashAddr,Ln = 0;   //菜单从第0行开始
	RS485PortParaStruct RS485PortPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8_1);//显示菜单
			DisplayString(Ln+1,5,0,"RS485端口 2");			
		
			FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
			MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortParaStruct));	
			//显示内存中的数据
			//端口功能
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Function,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			DisplayData(p8[0],2,0,(u16*)p);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+2,5,0,"485抄表口    ");			
					break;
				case 1:
					DisplayString(Ln+2,5,0,"485总线级联  ");			
					break;
				case 2:
					DisplayString(Ln+2,5,0,"485被抄      ");			
					break;
				case 3:
					DisplayString(Ln+2,5,0,"485上传维护口");		
					break;
				default:
					DisplayString(Ln+2,5,0,"485抄表口    ");
					break;
			}
			//通信波特率
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.BaudRate,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0x01:
					DisplayString(Ln+3,7,0,"300");				
					break;
				case 0x02:
					DisplayString(Ln+3,7,0,"600");				
					break;
				case 0x04:
					DisplayString(Ln+3,7,0,"1200");				
					break;
				case 0x08:
					DisplayString(Ln+3,7,0,"2400");				
					break;
				case 0x10:
					DisplayString(Ln+3,7,0,"4800");				
					break;
				case 0x20:
					DisplayString(Ln+3,7,0,"9600");
					break;
				case 0x40:
					DisplayString(Ln+3,7,0,"19200");
					break;
				case 0x80:
					DisplayString(Ln+3,7,0,"38400");
					break;
				case 0xC0:
					DisplayString(Ln+3,7,0,"57600");
					break;
				default:
					DisplayString(Ln+3,7,0,"1200");	
					break;	
			}							
			//校验方式
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Parity,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+4,9,0,"无校验");			
					break;
				case 1:
					DisplayString(Ln+4,9,0,"偶校验");		
					break;
				case 2:
					DisplayString(Ln+4,9,0,"奇校验");			
					break;
				default:
					DisplayString(Ln+4,9,0,"偶校验");
					break;
			}				
			//数据位
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.DataBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);	
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 5:
					DisplayString(Ln+5,7,0,"5 位");			
					break;
				case 6:
					DisplayString(Ln+5,7,0,"6 位");		
					break;
				case 7:
					DisplayString(Ln+5,7,0,"7 位");			
					break;
				case 8:
					DisplayString(Ln+5,7,0,"8 位");		
					break;
				default:
					DisplayString(Ln+5,7,0,"8 位");
					break;
			}	
			//停止位
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.StopBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+6,7,0," 1  位");			
					break;
				case 1:
					DisplayString(Ln+6,7,0,"1.5 位");		
					break;
				case 2:
					DisplayString(Ln+6,7,0," 2  位");			
					break;
				default:
					DisplayString(Ln+6,7,0," 1  位");
					break;
			}	
			pMenuCount->v3 = ConvertRs485PortParaToBitValue(RS485PortPara.Function, RS485PortPara.BaudRate, 
				               RS485PortPara.Parity, RS485PortPara.DataBit, RS485PortPara.StopBit);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)  //编辑列为0的时候
				{
					BoxSelect();  //编辑框选择				
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=10;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑端口功能
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485上传维护口");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485抄表口    ");
								}
								break;
							case 3:		// 编辑端口波特率
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// 编辑端口检验方式
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								x++;
								if(x > UART_PARITY_ODD) x = UART_PARITY_NONE;
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"无校验");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"偶校验");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"奇校验");
								}
								break;
							case 5:		// 编辑端口数据位
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								x++;
								if(x > UART_DATA_BIT_8) x = UART_DATA_BIT_5;
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 位");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 位");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 位");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 位");
								}
								break;
							case 6:		// 编辑端口停止位
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								x++;
								if(x > UART_STOP_BIT_2) x = UART_STOP_BIT_1;
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  位");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 位");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  位");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑端口功能
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485上传维护口");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485抄表口    ");
								}
								break;
							case 3:		// 编辑端口波特率
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								if(x == 0)
								{
									x = BAUD_RATE_9600_ID;
								}
								else
								{
									x--;
									if(x == BAUD_RATE_7200_ID) x = BAUD_RATE_4800_ID;
								}							
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// 编辑端口校验方式
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								if(x == UART_PARITY_NONE)
								{
									x = UART_PARITY_ODD;
								}
								else
								{
									x--;
								}
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_NONE << 7);
									DisplayString(4,9,0,"无校验");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_EVEN << 7);
									DisplayString(4,9,0,"偶校验");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_ODD << 7);
									DisplayString(4,9,0,"奇校验");
								}
								break;
							case 5:		// 编辑端口数据位
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								if(x == UART_DATA_BIT_5) 
								{
									x = UART_DATA_BIT_8;
								}
								else
								{
									x--;
								}
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 位");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 位");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 位");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 位");
								}
								break;
							case 6:		// 编辑端口停止位
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								if(x == UART_STOP_BIT_1)
								{
									x = UART_STOP_BIT_2;
								}
								else
								{
									x--;
								}
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  位");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 位");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  位");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								//保存设置的数据					
								//端口功能
								FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
								MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
								ConvertBitValueToRs485PortPara(&RS485PortPara.Function, &RS485PortPara.BaudRate, &RS485PortPara.Parity, 
									                           &RS485PortPara.DataBit, &RS485PortPara.StopBit, pMenuCount->v3);
								MW((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
										
								DisplayClrRow_4(Ln+7);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+7,9,0,"OK");																					
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
// Function name:       Menu_8_8_3
// Descriptions:        485端口3
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_8_3(void)
{
	u8 *p,*p8;	
	u64 x;
	u32 j, FlashAddr,Ln = 0;   //菜单从第0行开始
	RS485PortParaStruct RS485PortPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8_1);//显示菜单
			DisplayString(Ln+1,5,0,"RS485端口 3");			
		
			FlashAddr = 2 * sizeof(RS485PortParaStruct);
			FlashAddr += ADDR_RS485PortParaTbl;
			MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortParaStruct));	
			//显示内存中的数据
			//端口功能
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Function,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			DisplayData(p8[0],2,0,(u16*)p);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+2,5,0,"485抄表口    ");			
					break;
				case 1:
					DisplayString(Ln+2,5,0,"485总线级联  ");			
					break;
				case 2:
					DisplayString(Ln+2,5,0,"485被抄      ");			
					break;
				case 3:
					DisplayString(Ln+2,5,0,"485上传维护口");		
					break;
				default:
					DisplayString(Ln+2,5,0,"485抄表口    ");
					break;
			}
			//通信波特率
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.BaudRate,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0x01:
					DisplayString(Ln+3,7,0,"300");				
					break;
				case 0x02:
					DisplayString(Ln+3,7,0,"600");				
					break;
				case 0x04:
					DisplayString(Ln+3,7,0,"1200");				
					break;
				case 0x08:
					DisplayString(Ln+3,7,0,"2400");				
					break;
				case 0x10:
					DisplayString(Ln+3,7,0,"4800");				
					break;
				case 0x20:
					DisplayString(Ln+3,7,0,"9600");
					break;
				case 0x40:
					DisplayString(Ln+3,7,0,"19200");
					break;
				case 0x80:
					DisplayString(Ln+3,7,0,"38400");
					break;
				case 0xC0:
					DisplayString(Ln+3,7,0,"57600");
					break;
				default:
					DisplayString(Ln+3,7,0,"1200");	
					break;	
			}							
			//校验方式
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Parity,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+4,9,0,"无校验");			
					break;
				case 1:
					DisplayString(Ln+4,9,0,"偶校验");		
					break;
				case 2:
					DisplayString(Ln+4,9,0,"奇校验");			
					break;
				default:
					DisplayString(Ln+4,9,0,"偶校验");
					break;
			}				
			//数据位
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.DataBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);	
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 5:
					DisplayString(Ln+5,7,0,"5 位");			
					break;
				case 6:
					DisplayString(Ln+5,7,0,"6 位");		
					break;
				case 7:
					DisplayString(Ln+5,7,0,"7 位");			
					break;
				case 8:
					DisplayString(Ln+5,7,0,"8 位");		
					break;
				default:
					DisplayString(Ln+5,7,0,"8 位");
					break;
			}	
			//停止位
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.StopBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+6,7,0," 1  位");			
					break;
				case 1:
					DisplayString(Ln+6,7,0,"1.5 位");		
					break;
				case 2:
					DisplayString(Ln+6,7,0," 2  位");			
					break;
				default:
					DisplayString(Ln+6,7,0," 1  位");
					break;
			}	
			pMenuCount->v3 = ConvertRs485PortParaToBitValue(RS485PortPara.Function, RS485PortPara.BaudRate, 
				               RS485PortPara.Parity, RS485PortPara.DataBit, RS485PortPara.StopBit);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)  //编辑列为0的时候
				{
					BoxSelect();  //编辑框选择				
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=10;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑端口功能
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485上传维护口");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485抄表口    ");
								}
								break;
							case 3:		// 编辑端口波特率
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// 编辑端口检验方式
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								x++;
								if(x > UART_PARITY_ODD) x = UART_PARITY_NONE;
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"无校验");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"偶校验");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"奇校验");
								}
								break;
							case 5:		// 编辑端口数据位
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								x++;
								if(x > UART_DATA_BIT_8) x = UART_DATA_BIT_5;
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 位");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 位");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 位");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 位");
								}
								break;
							case 6:		// 编辑端口停止位
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								x++;
								if(x > UART_STOP_BIT_2) x = UART_STOP_BIT_1;
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  位");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 位");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  位");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑端口功能
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485上传维护口");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485抄表口    ");
								}
								break;
							case 3:		// 编辑端口波特率
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								if(x == 0)
								{
									x = BAUD_RATE_9600_ID;
								}
								else
								{
									x--;
									if(x == BAUD_RATE_7200_ID) x = BAUD_RATE_4800_ID;
								}							
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// 编辑端口检验方式
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								if(x == UART_PARITY_NONE)
								{
									x = UART_PARITY_ODD;
								}
								else
								{
									x--;
								}
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_NONE << 7);
									DisplayString(4,9,0,"无校验");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_EVEN << 7);
									DisplayString(4,9,0,"偶校验");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_ODD << 7);
									DisplayString(4,9,0,"奇校验");
								}
								break;
							case 5:		// 编辑端口数据位
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								if(x == UART_DATA_BIT_5) 
								{
									x = UART_DATA_BIT_8;
								}
								else
								{
									x--;
								}
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 位");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 位");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 位");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 位");
								}
								break;
							case 6:		// 编辑端口停止位
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								if(x == UART_STOP_BIT_1)
								{
									x = UART_STOP_BIT_2;
								}
								else
								{
									x--;
								}
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  位");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 位");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  位");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								//保存设置的数据					
								//端口功能
								FlashAddr = 2 * sizeof(RS485PortParaStruct);
								FlashAddr += ADDR_RS485PortParaTbl;
								MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
								ConvertBitValueToRs485PortPara(&RS485PortPara.Function, &RS485PortPara.BaudRate, &RS485PortPara.Parity, 
									                           &RS485PortPara.DataBit, &RS485PortPara.StopBit, pMenuCount->v3);
								MW((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
										
								DisplayClrRow_4(Ln+7);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+7,9,0,"OK");																					
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
// Function name:       Menu_8_9
// Descriptions:        最大需量清零
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_9(void)
{
//	MenuSelect();//菜单选择
//	if((Comm_Ram->ButtonVal&0x10)!=0)
//	{//确认          
//	}
	if((Comm_Ram->ButtonVal&0x2F)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2F;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_10
// Descriptions:        终端编号
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_10(void)
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_10);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://行政区码设置
						MenuCreate((u32)Menu_8_10_1);//创建下级菜单           
						break;
					case 2://终端地址设置
						MenuCreate((u32)Menu_8_10_2);//创建下级菜单
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
// Function name:       Menu_8_10_1
// Descriptions:        行政区码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_10_1(void)
{	
	u8 *p8,*p;
	u16 *p16;
	u32 i,j,x;
	u32 *p32,*q32;
	u32 Ln=0;          //菜单从第0行开始	
	u32 m[6]={0};
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_10_1);//显示菜单
			
			//显示内存中数据
			//行政区域码			
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			p32=(u32*)ADDR_DATABUFF;		
			MR(ADDR_DATABUFF,(u32)&CommPara.TerminalLocationCode,3);			
			q32=(u32*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);		
			DisplayData(p32[0]&0x00FFFFFF,6,0,(u16*)q32);
		
			MenuCount->v1=9;
			MenuCount->v2=14;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(Ln+2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=14;
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
								break;
							case 2:							
								//保存设置的数据				
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								for(i=0;i<6;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<20;
								m[1]=m[1]<<16;
								m[2]=m[2]<<12;
								m[3]=m[3]<<8;
								m[4]=m[4]<<4;		
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								if(CommPara.TerminalLocationCode == (m[0]|m[1]|m[2]|m[3]|m[4]|m[5]))//如果设置的行政区划与原来相同，则不重新写入内存
								{
									DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(Ln+2,9,0,"OK");
								}
								else//如果设置的行政区划与原来不同，则重新设置，向终端同步后硬件复位
								{
									CommPara.TerminalLocationCode = m[0]|m[1]|m[2]|m[3]|m[4]|m[5];
									MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
									DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(Ln+2,9,0,"OK");
									
									pTSysCtrl->TerminalLocation = CommPara.TerminalLocationCode;    //设置后向终端同步行政区划码
									#if(USER != 3400) //ybzyl20181031 负控云南版技术支持吴累累提出设置完行政区码负控不要重启，以免让人以为是错误
										DataResetAction(0x02,GUI_PORT);//硬件复位
									#endif
								}									
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF)||(x<1))
								{
									bcd_p8(p8,0,6);
								}
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
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_10_2
// Descriptions:        终端地址设置（十六进制）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_10_2(void)
{	
	u8 *p,*p8;
	u16 *p16;
	u32 *p32,*q32;
	u32 m[6]={0};
	u32 i,j,x,Ln=0;//菜单从第0行开始
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_10_2);//显示菜单
			
			//显示内存中数据
			//逻辑地址
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			p32=(u32*)ADDR_DATABUFF;		
			MR(ADDR_DATABUFF,(u32)&CommPara.TerminalAddr,3);
			p32[0]=p32[0]&0x00FFFFFF;
			q32=(u32*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);					
			DisplayData(p32[0],6,0,(u16*)q32);
		
			MenuCount->v1=9;
			MenuCount->v2=14;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(Ln+2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1: 
							MenuCount->v1=9;
							MenuCount->v2=14;
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
								break;
							case 2:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<6;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<20;
								m[1]=m[1]<<16;
								m[2]=m[2]<<12;
								m[3]=m[3]<<8;
								m[4]=m[4]<<4;	
								if(CommPara.TerminalAddr == (m[0]|m[1]|m[2]|m[3]|m[4]|m[5]))//如果设置的地址与原地址相同，则不重新写入内存
								{
									DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(Ln+2,9,0,"OK");
								}
								else //如果设置的地址与原地址不同，则重新设置，向终端同步地址后硬件复位
								{
									CommPara.TerminalAddr = m[0]|m[1]|m[2]|m[3]|m[4]|m[5];
									MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));								
									
									DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(Ln+2,9,0,"OK");
									
									pTSysCtrl->TerminalAddr = CommPara.TerminalAddr;        //设置后向终端同步终端地址
									#if(USER != 3400) //ybzyl20181031 负控云南版技术支持吴累累提出设置完终端地址负控不要重启，以免认为程序有问题
										DataResetAction(0x02,GUI_PORT);//硬件复位
									#endif
								}
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
					SelectH(MenuCount->v2,MenuCount->v1);
					UpDownSelectH();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);							
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF)||(x<1))
								{
									bcd_p8(p8,0,6);
								}
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
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_11
// Descriptions:        终端版本
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_11(void)
{
	u16 *p16;
	u32 Ln;
	Ln=0;//菜单从第0行开始
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_11);//显示菜单					
	//厂商代码
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+11*2);
	p16[0]=0x2000+'C';
	p16[1]=0x2000+'H';
	p16[2]=0x2000+'N';
	p16[3]=0x2000+'T';	
	//软件版本
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+11*2);
	p16[0]=((DefaultVersionPara.SoftWareVersion>>8)&0xf)+0x2030;
	p16[1]='.'+0x2000;
	p16[2]=((DefaultVersionPara.SoftWareVersion>>4)&0xf)+0x2030;
	p16[3]=(DefaultVersionPara.SoftWareVersion&0xf)+0x2030;
	//发布日期
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+11*2);
	p16[0]=((DefaultVersionPara.Year>>4)&0xf)+0x2030;
	p16[1]=(DefaultVersionPara.Year&0xf)+0x2030;
	p16[2]=((DefaultVersionPara.Month>>4)&0xf)+0x2030;
	p16[3]=(DefaultVersionPara.Month&0xf)+0x2030;
	p16[4]=((DefaultVersionPara.Day>>4)&0xf)+0x2030;
	p16[5]=(DefaultVersionPara.Day&0xf)+0x2030;
	//硬件版本
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
	p16[0]=((DefaultVersionPara.HardWareVersion>>8)&0xf)+0x2030;
	p16[1]='.'+0x2000;
	p16[2]=((DefaultVersionPara.HardWareVersion>>4)&0xf)+0x2030;
	p16[3]=(DefaultVersionPara.HardWareVersion&0xf)+0x2030;
	
	#if ((Project/100)==3) || ((Project/100)==9)
	//计量版本
	DisplayString(Ln+6,2,0,"计量版本:");
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
	p16[0]=0x2031;             //1
	p16[1]='.'+0x2000;         //.       
	p16[2]=0x2030;             //0
	p16[3]=0x2030;             //0
	#endif
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回 
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        终端管理与维护
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8(void)//终端管理与维护
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_1);        //1. 重启终端
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);        //2. 液晶调节
						break;
					case 3:
						MenuCreate((u32)Menu_8_3);        //3. 状态查看
						break;
					case 4:
						MenuCreate((u32)Menu_8_4);        //4. 信号强度和电池电量
						break;
					case 5:
						MenuCreate((u32)Menu_8_5);        //5. 无线实时状态
						break;
					case 6:
						MenuCreate((u32)Menu_8_6);        //6.终端时间设置
						break;
					case 7:
						MenuCreate((u32)Menu_8_7);        //7.界面密码设置
						break;
					case 8:
						MenuCreate((u32)Menu_8_8);        //8.测试通道设置
						break;
					case 9:
						MenuCreate((u32)Menu_8_9);        //9.最大需量清零
						break;
					case 10:
						MenuCreate((u32)Menu_8_10);       //10.终端编号
						break;
					case 11:
						MenuCreate((u32)Menu_8_11);       //11.终端版本
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


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        终端管理与维护
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8(void)//终端管理与维护
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_8);//显示菜单	
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_1);       //1. 重启终端
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);       //2. 液晶调节
						break;
					case 3:
						MenuCreate((u32)Menu_8_4);       //3. 信号强度和电池电量
						break;
					case 4:
						MenuCreate((u32)Menu_8_5);       //4. 无线实时状态
						break;
					case 5:
						MenuCreate((u32)Menu_8_6);       //5.终端时间设置
						break;
					case 6:
						MenuCreate((u32)Menu_8_7);       //6.界面密码设置
						break;
					case 7:
						MenuCreate((u32)Menu_8_8);       //7.测试通道设置
						break;
					case 8:
						MenuCreate((u32)Menu_8_10);      //8.终端编号
						break;
					case 9:
						MenuCreate((u32)Menu_8_11);      //9.终端版本
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
