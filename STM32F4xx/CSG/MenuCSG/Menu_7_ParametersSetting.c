/*************************************************
版权:正泰仪表智能终端部
文件:Menu_7_ParametersSetting.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单7参数设置与查询处理文件
*************************************************/


//菜单7 参数设置与查询
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop.h"
#include "../Display/DisplayLoop_Warning.h"
#include "Menu.h"
#include "Menu7.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include <string.h>
#include <stdio.h>


const u8 pMenu_7[]=//负控终端参数设置与查询
{
	"\n\r"
	"通讯通道设置\n\r"
	"测量点参数设置\n\r"
	"限值参数查询\n\r"
};


const u8 JZQpMenu_7[]=//南网集中器参数设置与查询
{
	"\n\r"
	"通讯通道设置\n\r"
	"测量点参数设置\n\r"
	"限值参数查询\n\r"
	"级联参数查询与设置\n\r"
};


const u8 pMenu_7_1[]=
{
	"\n\r"
	"IP及端口号设置\n\r"
	"通信方式设置\n\r"
	"APN设置\n\r"
	"专网用户名设置\n\r"
	"专网密码设置\n\r"
	"终端IP地址\n\r"
#if((Project == 300)&&(USER == 3400))
	"云南通信参数一键设置\n\r"
#endif
};


const u8 pMenu_7_1_1[]=
{
	"\n\r"
	"主用IP及端口号设置\n\r"
	"备用IP及端口号设置\n\r"
};


const u8 pMenu_7_1_1_1[]=
{
	"\n\r"
	"主用端口：\n\r"
	"主用IP类型：\n\r"
	"主IP：\n\r"
	"      确认设置\n\r"
};


const u8 pMenu_7_1_1_2[]=
{
	"\n\r"
	"备用1端口：\n\r"
	"备用1类型：\n\r"
	"备1：\n\r"
	"备用2端口：\n\r"
	"备用2类型：\n\r"
	"备2：\n\r"
	"      确认设置\n\r"
};


const u8 pMenu_7_1_2[]=
{
	"\n\r"
	"连接方式：\n\r"
	"重拨间隔(秒)：\n\r"
	"重拨次数(次)：\n\r"
	"心跳周期(分)：\n\r"
	"      确认设置\n\r"
};


const u8 pMenu_7_1_6[]=
{
	"\n\r"
	"IP  ：\n\r"
	"子网：\n\r"
	"网关：\n\r"
	"      确认设置\n\r"
};




const u8 pMenu_7_1_7[]=
{
	"\n\r"
	"曲靖\n\r"
};


const u8 pMenu_7_1_7_1[]=
{
	"\n\r"
	"普通卡\n\r"
	"物联卡\n\r"
};


const u8 pMenu_7_1_7_1_1[]=
{
	"\n\r"
	"中国移动\n\r"
	"中国联通\n\r"
	"中国电信\n\r"
};


const u8 pMenu_7_1_7_1_1_1[]=
{
	"\n\r"
	"IP:                 \n\r"
	"端口:               \n\r"
	"APN:                \n\r"
	"      确认设置\n\r"
};



const u8 pMenu_7_1_7_1_1_2[]=
{
	"\n\r"
	"IP:                 \n\r"
	"端口:               \n\r"
	"APN:                \n\r"
	"用户名:\n\r"
	"　　　　　　　　　　\n\r"
	"密码:\n\r"
	//"      确认设置\n\r"
};




const u8 pMenu_7_2[]=
{
	"\n\r"	
	"设置测量点\n\r"
	"查询测量点\n\r"
	"修改测量点\n\r"
	"删除测量点\n\r"
};
	

const u8 pMenu_7_2_1[]=
{
	"\n\r"	
	"测量点号    \n\r"
	"有效位      \n\r"
	"通信速率    \n\r"	
	"通信端口       \n\r"
	"协议类型      \n\r"
	"通信地址            \n\r"
	"采集器              \n\r"
	"      确认设置\n\r"
};


//#if (Project == 200) || (Project == 900) //南网集中器
const u8 pJZQMenu_7_2_1[]=
{
	"\n\r"	
	"测量点号    \n\r"
	"有效位      \n\r"
	"通信速率    \n\r"	
	"通信端口       \n\r"
	"协议类型      \n\r"
	"通信地址            \n\r"
	"采集器              \n\r"
	"电表类型        \n\r"
	"总分类型        \n\r"	
	"重点户          \n\r"
	"拉闸功能        \n\r"
	"费率数      \n\r"
	"CT变比      \n\r"
	"PT变比      \n\r"
	"      确认设置\n\r"
};
//#endif


const u8 pMenu_7_2_2[]=
{
	"\n\r"
	"测量点号    \n\r"
	"        确认\n\r"
};


const u8 pMenu_7_2_3[]=
{
	"\n\r"
	"从测量点:\n\r"
	"到测量点:\n\r"
	"        确认\n\r"
};


const u8 pMenu_7_2_4[]=
{
	"\n\r"	
	"测量点号    \n\r"
	"有效位      \n\r"
	"通信速率    \n\r"	
	"通信端口       \n\r"
	"协议类型      \n\r"
	"通信地址            \n\r"
	"采集器              \n\r"
};

//#if Project == 200 //南网集中器
const u8 pJZQMenu_7_2_4[]=
{
	"\n\r"	
	"测量点号    \n\r"
	"有效位      \n\r"
	"通信速率    \n\r"	
	"通信端口       \n\r"
	"协议类型      \n\r"
	"通信地址            \n\r"
	"采集器              \n\r"
	"电表类型        \n\r"
	"总分类型        \n\r"	
	"重点户          \n\r"
	"拉闸功能        \n\r"
	"费率数      \n\r"
	"CT变比      \n\r"
	"PT变比      \n\r"
};
//#endif


const u8 pMenu_7_3[]=
{
	"\n\r"
	"月通信流量越限值\n\r"
	"电能表时钟超差阈值\n\r"
	"电能表飞走阈值\n\r"
};


const u8 pJZQMenu_7_3[]=
{
	"\n\r"
	"月通信流量越限值\n\r"
	"电能表时钟超差阈值\n\r"
	"电能表停走阈值\n\r"
};


const u8 pMenu_7_3_1[]=
{
	"\n\r"
	"越限值(kB)：\n\r"
};


const u8 pMenu_7_3_2[]=
{
	"\n\r"
	"时钟超差(min)：\n\r"
};


const u8 pMenu_7_3_3[]=
{
	"\n\r"
	"飞走阈值(倍)：\n\r"
};


const u8 pJZQMenu_7_3_3[]=
{
	"\n\r"
	"停走阈值(kWh)：\n\r"
};



#if Project == 200 //南网集中器
const u8 pMenu_7_4[]=
{
	"\n\r"
	"级联从终端地址\n\r"
	"主终端地址\n\r"
	"被抄终端地址\n\r"
};


const u8 pMenu_7_4_1[]=
{
	"\n\r"
	"级联从终端地址1\n\r"
	"级联从终端地址2\n\r"
	"级联从终端地址3\n\r"
	"级联从终端地址4\n\r"
};


const u8 pMenu_7_4_1_1[]=
{
	"\n\r"
	"终端地市区县码\n\r"
	"终端地址\n\r"
};


const u8 pMenu_7_4_1_1_1[]=
{
	"\n\r"
	"设置值：\n\r"
	"      确认设置\n\r"
};


const u8 pMenu_7_4_1_1_2[]=
{
	"\n\r"
	"终端地址：\n\r"
	"      确认设置\n\r"
};
#endif





//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1
// Descriptions:        通讯通道设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1(void)//负控终端通讯通道设置
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1);//显示菜单
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
						MenuCreate((u32)Menu_7_1_1);//IP及端口号设置
						break;
					case 2:
						MenuCreate((u32)Menu_7_1_2);//通信方式设置
						break;
					case 3:
						MenuCreate((u32)Menu_7_1_3);//APN设置 
						break;
					case 4:
						MenuCreate((u32)Menu_7_1_4);//专网用户名设置
						break;
					case 5:
						MenuCreate((u32)Menu_7_1_5);//专网密码设置
						break;
					case 6:
						MenuCreate((u32)Menu_7_1_6);//终端IP地址
						break;
					#if((Project == 300)&&(USER == 3400))
						case 7:
							MenuCreate((u32)Menu_7_1_7);//云南通信参数一键设置
							break;
					#endif
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
// Function name:       Menu_7_1_1
// Descriptions:        IP及端口号设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_1(void)//IP及端口号设置
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_1_1_1);//主用IP及端口号设置
						break;
					case 2:
						MenuCreate((u32)Menu_7_1_1_2);//备用IP及端口号设置
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
// Function name:       Menu_7_1_1_1
// Descriptions:        主用IP及端口号设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_1_1(void)//主用IP及端口号设置
{
	u8 *p,*p8;
	u8 MasterAddrValue[6];
	u16 *p16;
	u32 i,j,x,Ln;
	CommunicationParaStruct CommPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	Ln=0;//菜单从第0行开始
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_1_1);//显示菜单
			
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));			
			//显示内存中数据
			//主站端口号
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&CommPara.MasterAddr,2);		
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);			
			bcd_p8(p,p16[0],5);
			//主用IP类型			
			switch(CommPara.MasterAddrType)
			{
				case 2:
					DisplayString(Ln+2,11,0," GPRS ");
					pMenuCount->v3 = MASTER_CH_TYPE_GPRS;
					break;
				case 4:
					DisplayString(Ln+2,11,0,"以太网");	
					pMenuCount->v3 = MASTER_CH_TYPE_ETHE;
					break;
				default:
					DisplayString(Ln+2,11,0," 无效 ");
					pMenuCount->v3 = 0xFF;
					break;
			}
			//主站IP地址	
			p8=(u8*)ADDR_DATABUFF;			
			MR(ADDR_DATABUFF,(u32)&CommPara.MasterAddr[2],4);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
			for(i = 0; i <= 3; i++)
			{
				bcd_p8(p+i*8,p8[3-i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			
			MenuCount->v1=9;
			MenuCount->v2=19;
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
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=4)
					{
						DisplayClrRow_4(Ln+4);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(Ln+4,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=11;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;						
						case 4:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}	
					if(((Comm_Ram->ButtonVal&0x08)!=0) || ((Comm_Ram->ButtonVal&0x04)!=0))
					{//右键 或 左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑主站端口 IP 类型
								x = pMenuCount->v3;
								if(x == MASTER_CH_TYPE_GPRS)
								{
									pMenuCount->v3 = MASTER_CH_TYPE_ETHE;
									DisplayString(2,11,0,"以太网");									
								}
								else
								{
									pMenuCount->v3 = MASTER_CH_TYPE_GPRS;
									DisplayString(2,11,0," GPRS ");	
								}
								break;	
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 4:
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));	
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,5);
								if(x<=1024)
								{
									DisplayClrRow(1,4);//清屏幕显示的指定行
									DisplayString(4,2,0,"提示:端口号>1024");
									Stack->MenuStack[MenuCount->Count].Task=2;								
								}
								else
								{
									p8=(u8*)&x;
									MR((u32)&MasterAddrValue,(u32)p8,2);
									//MR((u32)&CommPara.MasterAddr,(u32)p8,2);		// 保存端口
									//CommPara.MasterAddrType = pMenuCount->v3;		// 保存 IP 类型							
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);		// 主站IP地址
									for(i = 0; i < 4; i++)
									{
										p8[3 - i] = p8_bcd(p+8*i, 3);
									}
									MR((u32)&MasterAddrValue[2],(u32)p8,4);
									//
									if((MemCompare(CommPara.MasterAddr, MasterAddrValue, 6) != 0) 
										 ||(CommPara.MasterAddrType != pMenuCount->v3))
									{
										pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
									}
									//
									MR((u32)&CommPara.MasterAddr,(u32)&MasterAddrValue,6);   // 设置端口、IP地址
									CommPara.MasterAddrType = pMenuCount->v3;                // 设置IP类型	
									MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
									
									DisplayClrRow_4(4);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(4,9,0,"OK");
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://主站端口
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 2://主站IP类型
								break;
							case 3://主站IP地址							
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
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
// Function name:       Menu_7_1_1_2
// Descriptions:        备用IP及端口号设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_1_2(void)//备用IP及端口号设置
{
	u8 *p,*p8;
	u8 MasterBackAddrValue1[6];
	u8 MasterBackAddrValue2[6];
	u16 *p16;
	u32 i,j,x,Ln,CommType1,CommType2;
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	Ln=0;//菜单从第0行开始

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_1_2);//显示菜单
					
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));		
			//显示内存中数据
			//备用IP1端口号
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&CommPara.MasterBackAddr1,2);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);			
			bcd_p8(p,p16[0],5);	
			//备用IP1类型
			switch(CommPara.MasterBackAddr1Type)
			{
				case 2:
					DisplayString(Ln+2,11,0," GPRS ");
					pMenuCount->v3 &= 0xFFFFFF00;
					pMenuCount->v3 |= MASTER_CH_TYPE_GPRS;
					break;
				case 4:
					DisplayString(Ln+2,11,0,"以太网");	
					pMenuCount->v3 &= 0xFFFFFF00;
					pMenuCount->v3 |= MASTER_CH_TYPE_ETHE;
					break;
				default:
					DisplayString(Ln+2,11,0," 无效 ");
					pMenuCount->v3 &= 0xFFFFFF00;
					pMenuCount->v3 |= 0xFF;
					break;
			}
			//备用IP1地址
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&CommPara.MasterBackAddr1[2],4);			
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);			
			for(i = 0; i <= 3; i++)
			{
				bcd_p8(p+i*8,p8[3 - i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//备用IP2端口号
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&CommPara.MasterBackAddr2,2);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);			
			bcd_p8(p,p16[0],5);
			//备用IP2类型		
			switch(CommPara.MasterBackAddr2Type)
			{
				case 2:
					DisplayString(Ln+5,11,0," GPRS ");
					pMenuCount->v3 &= 0xFFFF00FF;
					pMenuCount->v3 |= MASTER_CH_TYPE_GPRS << 8;
					break;
				case 4:
					DisplayString(Ln+5,11,0,"以太网");	
					pMenuCount->v3 &= 0xFFFF00FF;
					pMenuCount->v3 |= MASTER_CH_TYPE_ETHE << 8;
					break;
				default:
					DisplayString(Ln+5,11,0," 无效 ");
					pMenuCount->v3 &= 0xFFFF00FF;
					pMenuCount->v3 |= 0xFF00;
					break;
			}
			//备用IP2地址
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&CommPara.MasterBackAddr2[2],4);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
			for(i = 0; i <= 3; i++)
			{
				bcd_p8(p+i*8,p8[3 - i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
						
			MenuCount->v1=10;
			MenuCount->v2=19;
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
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=7)
					{
						DisplayClrRow_4(Ln+7);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(Ln+7,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=10;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=10;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=10;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=10;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=5;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if(((Comm_Ram->ButtonVal&0x08)!=0) || ((Comm_Ram->ButtonVal&0x04)!=0))
					{//右键 或 左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// 编辑主站备用端口1 IP 类型
								x = pMenuCount->v3 & 0x000000FF;
								if(x == MASTER_CH_TYPE_GPRS)
								{
									pMenuCount->v3 &= 0xFFFFFF00;
									pMenuCount->v3 |= MASTER_CH_TYPE_ETHE;
									DisplayString(2,11,0,"以太网");									
								}
								else
								{
									pMenuCount->v3 &= 0xFFFFFF00;
									pMenuCount->v3 |= MASTER_CH_TYPE_GPRS;
									DisplayString(2,11,0," GPRS ");	
								}
								break;	
							case 5:		// 编辑主站备用端口2 IP 类型
								x = pMenuCount->v3 & 0x0000FF00;
								x >>= 8;
								if(x == MASTER_CH_TYPE_GPRS)
								{
									pMenuCount->v3 &= 0xFFFF00FF;
									pMenuCount->v3 |= MASTER_CH_TYPE_ETHE << 8;
									DisplayString(5,11,0,"以太网");									
								}
								else
								{
									pMenuCount->v3 &= 0xFFFF00FF;
									pMenuCount->v3 |= MASTER_CH_TYPE_GPRS << 8;
									DisplayString(5,11,0," GPRS ");	
								}
								break;	
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 5:
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 7:
								//保存设置的数据
								//备用IP1端口
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
								x=p8_bcd(p,5);
								if(x<=1024)
								{
									DisplayClrRow(1,7);//清屏幕显示的指定行
									DisplayString(4,1,0,"提示:端口号1>1024");
									Stack->MenuStack[MenuCount->Count].Task=2;		
								}
								else
								{
									p8=(u8*)&x;		
									MR((u32)&MasterBackAddrValue1,(u32)p8,2);										
									//备用IP1类型
									CommType1 = pMenuCount->v3&0x000000FF;
									//备用IP1地址
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[3 - i]=p8_bcd(p+8*i,3);
									}
									MR((u32)&MasterBackAddrValue1[2],(u32)p8,4);										
									//备用IP2端口
									p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
									x=p8_bcd(p,5);
									if(x<=1024)
									{
										DisplayClrRow(1,7);//清屏幕显示的指定行
										DisplayString(4,1,0,"提示:端口号2>1024");
										Stack->MenuStack[MenuCount->Count].Task=2;		
									}
									else
									{
										p8=(u8*)&x;
										MR((u32)&MasterBackAddrValue2,(u32)p8,2);										
										//备用IP2类型
										CommType2 = pMenuCount->v3 >> 8;
										//备用IP2地址
										p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
										for(i=0;i<=3;i++)
										{
											p8[3 - i]=p8_bcd(p+8*i,3);
										}
										MR((u32)&MasterBackAddrValue2[2],(u32)p8,4);
										
										if((MemCompare(CommPara.MasterBackAddr1, MasterBackAddrValue1, 6) != 0) 
										||(CommPara.MasterBackAddr1Type != CommType1)
										||(MemCompare(CommPara.MasterBackAddr2, MasterBackAddrValue2, 6) != 0) 
										||(CommPara.MasterBackAddr2Type != CommType2))
										{
											pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
										}
										
										MR((u32)&CommPara.MasterBackAddr1,(u32)&MasterBackAddrValue1,6);   // 设置备用1端口、IP地址
										CommPara.MasterBackAddr1Type = CommType1;                               // 设置备用1 IP类型	
										MR((u32)&CommPara.MasterBackAddr2,(u32)&MasterBackAddrValue2,6);   // 设置备用2端口、IP地址
										CommPara.MasterBackAddr2Type = CommType2;											         // 设置备用2 IP类型	
										MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
										
										DisplayClrRow_4(7);//清屏幕显示的指定行(4个控制字不清)
										DisplayString(7,9,0,"OK");
									}
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://备用IP1端口
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 2://备用IP1类型
								break;
							case 3://备用IP1地址
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 4://备用IP2端口
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 5://备用IP2类型
								break;
							case 6://备用IP2地址
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
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
// Function name:       Menu_7_1_2
// Descriptions:        通信方式设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_2(void)//通信方式设置
{
	u8 *p,*p8;
	u8 m[2]={0};
	u16 *p16;
	u32 j,i,x,Ln;
	CommunicationParaStruct CommPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	Ln=0;//菜单从第0行开始


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_2);//显示菜单
			
			//显示内存中数据
			//连接方式
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));			
			switch(CommPara.IpLinkType)
			{
				case 0:
					DisplayString(Ln+1,9,0," TCP  ");
					pMenuCount->v3 = 0x00;
					break;
				case 1:
					DisplayString(Ln+1,9,0," UDP  ");	
					pMenuCount->v3 = 0x01;
					break;
				default:
					DisplayString(Ln+1,9,0," 无效 ");
					pMenuCount->v3 = 0xFF;
					break;
			}
			//重拨间隔
			p8=(u8*)ADDR_DATABUFF;
			MR((u32)p8,(u32)&CommPara.RedialTimeGap,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+13*2);
			DisplayData(p8[0],2,0,(u16*)p);
			//重拨次数
			p8=(u8*)ADDR_DATABUFF;
			MR((u32)p8,(u32)&CommPara.RedialCnt,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+13*2);
			DisplayData(p8[0],2,0,(u16*)p);		
			//心跳周期
			if(BCD_Check(&CommPara.HeartBeatTimeGap,1) != NO_ERROR)
			{		
				CommPara.HeartBeatTimeGap = 0;
				MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));				
				MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
				p8=(u8*)ADDR_DATABUFF;
				MR((u32)p8,(u32)&CommPara.HeartBeatTimeGap,1);
				p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+13*2);
				DisplayData(p8[0],2,0,(u16*)p);
			}
			else
			{
				p8=(u8*)ADDR_DATABUFF;
				MR((u32)p8,(u32)&CommPara.HeartBeatTimeGap,1);
				p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+13*2);
				DisplayData(p8[0],2,0,(u16*)p);
			}
					
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
					if(Stack->MenuStack[MenuCount->Count].EditRow!=5)
					{
						DisplayString(Ln+5,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=13;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=13;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=13;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}		
					if(((Comm_Ram->ButtonVal&0x08)!=0) || ((Comm_Ram->ButtonVal&0x04)!=0))
					{//右键 或 左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:	// 编辑连接方式
								x = pMenuCount->v3;
								if(x == 0x01)
								{
									pMenuCount->v3 = 0x00;
									DisplayString(1,9,0," TCP  ");									
								}
								else
								{
									pMenuCount->v3 = 0x01;
									DisplayString(1,9,0," UDP  ");	
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
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								//连接方式
								CommPara.IpLinkType = pMenuCount->v3;
								//重拨间隔
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+13*2);
								for(i=0;i<2;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<4;
								CommPara.RedialTimeGap=m[0]|m[1];
								//重拨次数
								MC(0,m[0],2);
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+13*2);
								for(i=0;i<2;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<4;
								CommPara.RedialCnt=m[0]|m[1];
								//心跳周期
								MC(0,m[0],2);
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+13*2);
								for(i=0;i<2;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<4;
								CommPara.HeartBeatTimeGap=m[0]|m[1];
								
								
								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+5,9,0,"OK");
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
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+13*2);
								x=p8_bcd(p8,2);
								if(x>99)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+13*2);
								x=p8_bcd(p8,2);
								if(x>99)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+13*2);
								x=p8_bcd(p8,2);
								if(x>99)
								{
									bcd_p8(p8,0,2);
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
// Function name:       Menu_7_1_3
// Descriptions:        APN设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_3(void)//APN设置
{
	u8 *p,*p8;
	u32 i,j,k;
	u32 MinLn=4;           //软键盘最小行号
	u32 MaxLn=8;           //软键盘最大行号
	u32 LastLnMaxCol=14;   //软键盘的最后一行最大列号
	u32 MinCol=0;          //软键盘最小列号
	u32 MaxCol=19;         //软键盘最大行号
	u32 EditLn=2;
	u32 EditCol=0;
	u32 EditLength=19;	
	u32 Ln=0;              //菜单从第0行开始
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(Ln+1,7,0,"APN设置");			
			DisplayString(MinLn,MinCol,0,p8);
			//显示内存中的数据
			MC(0,(u32)&CommPara,sizeof(CommunicationParaStruct));
			MC(0,ADDR_DATABUFF,100);
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			p=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&CommPara.APN,16);
			for(k=0;k<=15;k++)
			{
				if((p[k]==0x5E)||(p[k]==0x5F)||(p[k]==0xFF)||(p[k]==0x7F))
				{
					p[k]=0x20;
				}
				else
				{
					DisplayString(EditLn,EditCol,0,p);
				}
			}					
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://任务1:键盘选择
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,EditLn,EditCol);//键盘选择
			break;
		case 2://任务2:编辑框选择
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//下键
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p=(u8*)(ADDR_STRINGBUFF+4*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//右键
					Stack->MenuStack[MenuCount->Count].MoveRow ++;
					if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//左键
					if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow --;
					}
					else
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
					}
				}
				NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					//保存数据
					MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF,100);
					p=(u8*)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=15;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
							for(j=i;j<=15;j++)
							{
								p8[j]=0x00;
							}
							break;
						}
					}
					if((MemCompare(CommPara.APN, p8, sizeof(CommPara.APN)) != 0))
					{
						pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
					}
					MR((u32)&CommPara.APN,(u32)p8,sizeof(CommPara.APN));
					MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
					DisplayClrRow(1,9);
					DisplayString(2,0,0,p8);
					DispalyAlign(2,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,"设置OK");
					DisplayString(5,4,0,"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功后按任意键退出
			Stack->MenuStack[MenuCount->Count].MoveRow =0;
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}	
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1_4
// Descriptions:        专网用户名设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_4(void)//专网用户名设置
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,k;
	u32 EditLn=2;
	u32 EditCol=0;
	u32 EditLength=20;	
	u32 MinLn=4;            //软键盘最小行号
	u32 MaxLn=8;            //软键盘最大行号
	u32 MinCol=0;           //软键盘最小列号
	u32 MaxCol=19;          //软键盘最大列号
	u32 LastLnMaxCol=14;    //软键盘的最后一行最大列号
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;



	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=2;
			DisplayClr();//清全屏
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(1,3,0,"专网用户名设置");			
			DisplayString(MinLn,MinCol,0,p8);
			
			p=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF,100);
			MC(0,(u32)&CommPara,sizeof(CommunicationParaStruct));
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			MR((u32)p,(u32)&CommPara.APNUserName,32);
			for(k=0;k<=31;k++)
			{
				if((p[k]==0x5E)||(p[k]==0x5F)||(p[k]==0xFF)||(p[k]==0x7F))
				{
					p[k]=0x20;
				}
			}
			DisplayString(EditLn,EditCol,0,p);
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://任务1:键盘选择
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,EditLn,EditCol);//键盘选择
			break;
		case 2://任务2:编辑框选择
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//下键
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p16|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//右键
					Stack->MenuStack[MenuCount->Count].MoveRow ++;
					if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//左键
					if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow --;
					}
					else
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
					}
				}
				NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					//保存数据
					MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));				
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF,100);
					p=(u8 *)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=31;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
								for(j=i;j<=31;j++)
								{
									p8[j]=0x00;
								}
								break;
						}
					}
					if((MemCompare(CommPara.APNUserName, p8, sizeof(CommPara.APNUserName)) != 0))
					{
						pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
					}
					MR((u32)&CommPara.APNUserName,(u32)p8,sizeof(CommPara.APNUserName));
					MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));			
					//设置成功提示OK
					DisplayClrRow(1,9);
					DisplayString(2,0,0,p8);
					DispalyAlign(2,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,"设置OK");
					DisplayString(5,4,0,"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功后按任意键退出
			Stack->MenuStack[MenuCount->Count].MoveRow =0;
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1_5
// Descriptions:        专网密码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_5(void)//专网密码设置
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,k;
	u32 MinLn=4;            //软键盘最小行号
	u32 MaxLn=8;            //软键盘最大行号
	u32 LastLnMaxCol=14;    //软键盘的最后一行最大列号
	u32 MinCol=0;           //软键盘最小列号
	u32 MaxCol=19;          //软键盘最大列号
	u32 EditLn=2;
	u32 EditCol=0;
	u32 EditLength=20;
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			DisplayClr();//清全屏
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(1,4,0,"专网密码设置");
			DisplayString(MinLn,MinCol,0,p8);
			
			MC(0,(u32)&CommPara,sizeof(CommunicationParaStruct));
			MC(0,ADDR_DATABUFF,100);
			p=(u8 *)ADDR_DATABUFF;
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			MR((u32)p,(u32)&CommPara.APNPassWord,32);
			for(k=0;k<=31;k++)
			{
				if((p[k]==0x5E)||(p[k]==0x5F)||(p[k]==0xFF)||(p[k]==0x7F))
				{
					p[k]=0x20;
				}
			}
			DisplayString(EditLn,EditCol,0,p);			
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://任务1:键盘选择
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,EditLn,EditCol);//键盘选择
			break;
		case 2://任务2:编辑框选择
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//下键
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p16|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//右键
					Stack->MenuStack[MenuCount->Count].MoveRow ++;
					if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//左键
					if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow --;
					}
					else
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
					}
				}
				NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					//保存数据
					MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF,100);
					p=(u8 *)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=31;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
							for(j=i;j<=31;j++)
							{
								p8[j]=0x00;
							}
							break;
						}
					}
					if((MemCompare(CommPara.APNPassWord, p8, sizeof(CommPara.APNPassWord)) != 0))
					{
						pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
					}
					MR((u32)&CommPara.APNPassWord,(u32)p8,sizeof(CommPara.APNPassWord));
					MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));				
					//设置成功提示OK
					DisplayClrRow(1,9);
					DisplayString(2,0,0,p8);
					DispalyAlign(2,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,"设置OK");
					DisplayString(5,4,0,"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功后按任意键退出
			Stack->MenuStack[MenuCount->Count].MoveRow =0;
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1_6
// Descriptions:        终端IP地址设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_6(void)//终端IP地址设置
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,x,Ln;
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	Ln=0;//菜单从第0行开始

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_6);//显示菜单
			
			//显示内存中数据
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			MR(ADDR_DATABUFF,(u32)&CommPara.IpAddr,4);
			MR(ADDR_DATABUFF+4,(u32)&CommPara.IpSubNetMask,4);
			MR(ADDR_DATABUFF+8,(u32)&CommPara.IpGateWayAddr,4);
			//终端IP
			p8=(u8 *)ADDR_DATABUFF;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[3-i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//子网掩码
			p8=(u8 *)ADDR_DATABUFF;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[4+3-i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//网关
			p8=(u8 *)ADDR_DATABUFF;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[8+3-i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			
			MenuCount->v1=5;
			MenuCount->v2=19;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=4)
					{
						DisplayString(Ln+4,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
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
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 4:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[3-i]=p8_bcd(p+8*i,3);
								}
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[4+3-i]=p8_bcd(p+8*i,3);
								}
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[8+3-i]=p8_bcd(p+8*i,3);
								}
								
								if((MemCompare(&CommPara.IpAddr, p8, 4) != 0)
								||(MemCompare(&CommPara.IpSubNetMask, p8+4 , 4) != 0)
								||(MemCompare(&CommPara.IpGateWayAddr, p8+8 , 4) != 0))
								{
									pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
								}
								
								MR((u32)&CommPara.IpAddr,(u32)p8,4);
								MR((u32)&CommPara.IpSubNetMask,(u32)(p8+4),4);
								MR((u32)&CommPara.IpGateWayAddr,(u32)(p8+8),4);
								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
								DisplayClrRow_4(Ln+4);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+4,9,0,"OK");
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+20*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;							
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+20*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;							
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+20*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;							
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
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


#if((Project == 300)&&(USER == 3400))
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1_7
// Descriptions:        选择地区
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7(void)//选择地区
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7);//显示菜单
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
						MenuCreate((u32)Menu_7_1_7_1);//默认模式       
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
// Function name:       Menu_7_1_7_1
// Descriptions:        选择卡类型
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1(void)//选择卡类型
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1);//显示菜单
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
						MenuCreate((u32)Menu_7_1_7_1_1);//普通卡   
						break;
					case 2:
						MenuCreate((u32)Menu_7_1_7_1_2);//物联卡
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
// Function name:       Menu_7_1_7_1_1
// Descriptions:        选择运营商
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_1(void)//选择运营商
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_1_7_1_1_1);//移动
						break;
					case 2:
						MenuCreate((u32)Menu_7_1_7_1_1_2);//联通
						break;
					case 3:
						MenuCreate((u32)Menu_7_1_7_1_1_3);//电信
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
// Function name:       Menu_7_1_7_1_1_1
// Descriptions:       普通卡移动
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_1_1(void)// 普通卡移动
{
	u32 j = 0;	
	u8 IPPORT[6] = {0x21,0x1C,0x74,0xA2,0x14,0x64};
	u8 APNValue[16] = {'Y','N','D','W','F','K','.','Y','N',};
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1_1);//显示菜单	
			DisplayString(1,6,0,"100.20.162.116");      //IP地址
			DisplayString(2,6,0,"7201");						    //端口号		
			DisplayString(3,6,0,"YNDWFK.YN");						//APN				
			DisplayString(4,0,0,"      确认设置\n\r");
			MenuCount->v1=6;
			MenuCount->v2=19;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=20;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=6;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:		
								break;
							case 4:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
								if((MemCompare(CommPara.MasterAddr, IPPORT, 6) != 0) 
									||(CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr1, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr2, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.APN, APNValue, sizeof(CommPara.APN)) != 0))
								{
									pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
								}
								MR((u32)&CommPara.MasterAddr,(u32)IPPORT,sizeof(IPPORT));                //主用IP地址、端口
								CommPara.MasterAddrType = MASTER_CH_TYPE_GPRS;            //主用通道类型
								MR((u32)&CommPara.MasterBackAddr1,(u32)IPPORT,sizeof(IPPORT));           //备用IP1地址、端口
								CommPara.MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;       //备用IP1通道类型
								MR((u32)&CommPara.MasterBackAddr2,(u32)IPPORT,sizeof(IPPORT));           //备用IP2地址、端口
								CommPara.MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;       //备用IP2通道类型
								MR((u32)&CommPara.APN,(u32)APNValue,sizeof(APNValue));                     //APN

								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								DisplayString(4,0,0,"         OK         ");					
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
// Function name:       Menu_7_1_7_1_1_1
// Descriptions:        普通卡联通
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_1_2(void)// 普通卡联通
{
	u8 IPPORT[6] = {0x21,0x1C,0x2E,0x01,0x64,0xAC};
	u8 APNValue[16] = {'Y','N','D','W','.','F','K','.','Y','N','A','P','N'};
	u32 j = 0;	
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1_1);//显示菜单	
			DisplayString(1,6,0,"172.100.1.46");      //IP地址
			DisplayString(2,6,0,"7201");						    //端口号		
			DisplayString(3,6,0,"YNDW.FK.YNAPN");						//APN				
			DisplayString(4,0,0,"      确认设置\n\r");
			MenuCount->v1=6;
			MenuCount->v2=17;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=6;
							MenuCount->v2=18;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:		
								break;
							case 4:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								if((MemCompare(CommPara.MasterAddr, IPPORT, 6) != 0) 
									||(CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr1, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr2, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.APN, APNValue, sizeof(CommPara.APN)) != 0))
								{
									pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
								}
								MR((u32)&CommPara.MasterAddr,(u32)IPPORT,sizeof(IPPORT));                //主用IP地址、端口
								CommPara.MasterAddrType = MASTER_CH_TYPE_GPRS;            //主用通道类型
								MR((u32)&CommPara.MasterBackAddr1,(u32)IPPORT,sizeof(IPPORT));           //备用IP1地址、端口
								CommPara.MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;       //备用IP1通道类型
								MR((u32)&CommPara.MasterBackAddr2,(u32)IPPORT,sizeof(IPPORT));           //备用IP2地址、端口
								CommPara.MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;       //备用IP2通道类型
								MR((u32)&CommPara.APN,(u32)APNValue,sizeof(APNValue));                     //APN
								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
								DisplayString(4,0,0,"         OK         ");	
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
// Function name:       Menu_7_1_7_1_1_3
// Descriptions:        普通卡电信
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_1_3(void)// 普通卡电信
{

/*
//	u8 IPPORT[6] = {0x21,0x1C,0x47,0x01,0x64,0xAC};
//	u8 APNValue[16] = {'Y','E','P','G','F','K','.','V','P','D','N','.','Y','N'};
//	u8 USERNAMEValue[32] = {'q','j','d','w','@','y','n','d','w','s','k','.','v','p','d','n','.','y','n'};
//	u8 PASSWORDValue[32] = {'g','d','j','1','7','2'};
	u32 j = 0;	
//	CommunicationParaStruct CommPara;
//	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1_2);//显示菜单	
//			DisplayString(1,6,0,"172.100.1.71");      //IP地址
//			DisplayString(2,6,0,"7201");						    //端口号		
//			DisplayString(3,6,0,"YEPGFK.VPDN.YN");						//APN		
//			DisplayString(4,7,7,"qjdw@yndwsk.vpdn.yn");						//用户名
//			DisplayString(6,7,0,"gdj172");										//密码			
//			DisplayString(7,0,0,"      确认设置\n\r");
			MenuCount->v1=7;
			MenuCount->v2=19;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=7;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=7;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=7;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:
							case 4:
							case 5:	
							case 6:
								break;
							case 7:
								//保存设置的数据
//								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
//								MR((u32)&p1SelfAdaptionPara,ADDR_SelfAdaptionPara,sizeof(SelfAdaptionParaStruct));	
//									if((MemCompare(CommPara.MasterAddr, IPPORT, 6) != 0) 
//										||(CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS)
//										||(MemCompare(CommPara.MasterBackAddr1, IPPORT, 6) != 0) 
//										||(CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS)
//										||(MemCompare(CommPara.MasterBackAddr2, IPPORT, 6) != 0) 
//										||(CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS)
//										||(MemCompare(CommPara.APN, APNValue, sizeof(CommPara.APN)) != 0)
//										||(MemCompare(CommPara.APNUserName, USERNAMEValue, sizeof(CommPara.APNUserName)) != 0)
//										||(MemCompare(CommPara.APNPassWord, PASSWORDValue, sizeof(CommPara.APNPassWord)) != 0))
//									{
//										pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
//									}
//									MR((u32)&CommPara.MasterAddr,(u32)IPPORT,sizeof(IPPORT));                //主用IP地址、端口
//									CommPara.MasterAddrType = MASTER_CH_TYPE_GPRS;            //主用通道类型
//									MR((u32)&CommPara.MasterBackAddr1,(u32)IPPORT,sizeof(IPPORT));           //备用IP1地址、端口
//									CommPara.MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;       //备用IP1通道类型
//									MR((u32)&CommPara.MasterBackAddr2,(u32)IPPORT,sizeof(IPPORT));           //备用IP2地址、端口
//									CommPara.MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;       //备用IP2通道类型
//									MR((u32)&CommPara.APN,(u32)APNValue,sizeof(APNValue));                     //APN
//									MR((u32)&CommPara.APNUserName,(u32)USERNAMEValue,sizeof(USERNAMEValue));        //APN用户名
//									MR((u32)&CommPara.APNPassWord,(u32)PASSWORDValue,sizeof(PASSWORDValue));         //APN密码
//									MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
//									
//									DisplayString(7,0,0,"         OK         ");	
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
*/

	if((Comm_Ram->ButtonVal&0x2F)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2F;//键使用后清0

}




//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1_7_1_1_1_2
// Descriptions:        曲靖物联卡
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_2(void)//曲靖物联卡
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_1_7_1_2_1);//移动      
						break;
					case 2:
						MenuCreate((u32)Menu_7_1_7_1_2_2);//联通
						break;
					case 3:
						MenuCreate((u32)Menu_7_1_7_1_2_3);//电信
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
// Function name:       Menu_7_1_7_1_2_1
// Descriptions:        曲靖物联卡移动
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_2_1(void)//曲靖物联卡移动
{
	u32 j = 0;	
	u8 IPPORT[6] = {0x73,0x17,0xC6,0x1B,0x6F,0x0A};
	u8 APNValue[16] = {'C','M','I','O','T','Y','N','D','W','F','K','.','Y','N'};
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1_1);//显示菜单	
			DisplayString(1,6,0,"10.111.27.198");      //IP地址
			DisplayString(2,6,0,"6003");						    //端口号		
			DisplayString(3,6,0,"CMIOTYNDWFK.YN");						//APN				
			DisplayString(4,0,0,"      确认设置\n\r");
			MenuCount->v1=6;
			MenuCount->v2=18;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=18;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=6;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:		
								break;
							case 4:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								if((MemCompare(CommPara.MasterAddr, IPPORT, 6) != 0) 
									||(CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr1, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr2, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.APN, APNValue, sizeof(CommPara.APN)) != 0))
								{
									pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
								}
								MR((u32)&CommPara.MasterAddr,(u32)IPPORT,sizeof(IPPORT));                //主用IP地址、端口
								CommPara.MasterAddrType = MASTER_CH_TYPE_GPRS;            //主用通道类型
								MR((u32)&CommPara.MasterBackAddr1,(u32)IPPORT,sizeof(IPPORT));           //备用IP1地址、端口
								CommPara.MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;       //备用IP1通道类型
								MR((u32)&CommPara.MasterBackAddr2,(u32)IPPORT,sizeof(IPPORT));           //备用IP2地址、端口
								CommPara.MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;       //备用IP2通道类型
								MR((u32)&CommPara.APN,(u32)APNValue,sizeof(APNValue));                     //APN
								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
								DisplayString(4,0,0,"         OK         ");	
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
// Function name:       Menu_7_1_7_1_2_2
// Descriptions:        曲靖物联卡联通
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_2_2(void)//曲靖物联卡联通
{
	u32 j = 0;	
	u8 IPPORT[6] = {0x79,0x17,0xC6,0x1B,0x6F,0x0A};
	//u8 APNValue[16] = {'C','M','I','O','T','Y','N','D','W','F','K','.','Y','N'};
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1_1);//显示菜单	
			DisplayString(1,6,0,"10.111.27.198");      //IP地址
			DisplayString(2,6,0,"6009");						    //端口号		
			DisplayString(3,6,0,"");						//APN				
			DisplayString(4,0,0,"      确认设置\n\r");
			MenuCount->v1=6;
			MenuCount->v2=18;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=18;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=6;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:		
								break;
							case 4:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								if((MemCompare(CommPara.MasterAddr, IPPORT, 6) != 0) 
									||(CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr1, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr2, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS)
									//||(MemCompare(CommPara.APN, APNValue, sizeof(CommPara.APN)) != 0)
									)
								{
									pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
								}
								MR((u32)&CommPara.MasterAddr,(u32)IPPORT,sizeof(IPPORT));                //主用IP地址、端口
								CommPara.MasterAddrType = MASTER_CH_TYPE_GPRS;            //主用通道类型
								MR((u32)&CommPara.MasterBackAddr1,(u32)IPPORT,sizeof(IPPORT));           //备用IP1地址、端口
								CommPara.MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;       //备用IP1通道类型
								MR((u32)&CommPara.MasterBackAddr2,(u32)IPPORT,sizeof(IPPORT));           //备用IP2地址、端口
								CommPara.MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;       //备用IP2通道类型
								//MR((u32)&CommPara.APN,(u32)APNValue,sizeof(APNValue));                     //APN
								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
								DisplayString(4,0,0,"         OK         ");
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
// Function name:       Menu_7_1_7_1_2_3
// Descriptions:        曲靖物联卡电信
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1_7_1_2_3(void)//曲靖物联卡电信
{
	u8 IPPORT[6] = {0x71,0x17,0xC6,0x1B,0x6F,0x0A};
	u8 APNValue[16] = {'y','n','d','w','s','c','.','v','p','d','n','.','y','n'};
//	u8 USERNAMEValue[32] = {'q','j','d','w','@','y','n','d','w','s','k','.','v','p','d','n','.','y','n'};
//	u8 PASSWORDValue[32] = {'g','d','j','1','7','2'};
	u32 j = 0;	
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1_7_1_1_1);//显示菜单	
			DisplayString(1,6,0,"10.111.27.198");      //IP地址
			DisplayString(2,6,0,"6001");						    //端口号		
			DisplayString(3,6,0,"yndwsc.vpdn.yn");						//APN		
			MenuCount->v1=6;
			MenuCount->v2=18;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=18;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=6;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:
								break;
							case 4:
								//保存设置的数据
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								if((MemCompare(CommPara.MasterAddr, IPPORT, 6) != 0) 
									||(CommPara.MasterAddrType != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr1, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr1Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.MasterBackAddr2, IPPORT, 6) != 0) 
									||(CommPara.MasterBackAddr2Type != MASTER_CH_TYPE_GPRS)
									||(MemCompare(CommPara.APN, APNValue, sizeof(CommPara.APN)) != 0)
//										||(MemCompare(CommPara.APNUserName, USERNAMEValue, sizeof(CommPara.APNUserName)) != 0)
//										||(MemCompare(CommPara.APNPassWord, PASSWORDValue, sizeof(CommPara.APNPassWord)) != 0)
									)
								{
									pTSysCtrl->Flag |= TERMINAL_COMM_PARA_CHANGE_FLAG;
								}
								MR((u32)&CommPara.MasterAddr,(u32)IPPORT,sizeof(IPPORT));                //主用IP地址、端口
								CommPara.MasterAddrType = MASTER_CH_TYPE_GPRS;            //主用通道类型
								MR((u32)&CommPara.MasterBackAddr1,(u32)IPPORT,sizeof(IPPORT));           //备用IP1地址、端口
								CommPara.MasterBackAddr1Type = MASTER_CH_TYPE_GPRS;       //备用IP1通道类型
								MR((u32)&CommPara.MasterBackAddr2,(u32)IPPORT,sizeof(IPPORT));           //备用IP2地址、端口
								CommPara.MasterBackAddr2Type = MASTER_CH_TYPE_GPRS;       //备用IP2通道类型
								MR((u32)&CommPara.APN,(u32)APNValue,sizeof(APNValue));                     //APN
//								MR((u32)&CommPara.APNUserName,(u32)USERNAMEValue,sizeof(USERNAMEValue));        //APN用户名
//								MR((u32)&CommPara.APNPassWord,(u32)PASSWORDValue,sizeof(PASSWORDValue));         //APN密码
								MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
								DisplayString(4,0,0,"         OK         ");	
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
#endif



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2
// Descriptions:        测量点参数设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2(void)
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2);//显示菜单
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
						MenuCreate((u32)Menu_7_2_1);//设置测量点           
						break;
					case 2:
						MenuCreate((u32)Menu_7_2_2);//查询测量点 
						break;
					case 3:
						MenuCreate((u32)Menu_7_2_3);//修改测量点 
						break;
					case 4:
						MenuCreate((u32)Menu_7_2_4);//删除测量点 	
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




u32 ConvertPnParaToBitValue(MeasurePointStruct* Pn)
{
	u32 BitValue = 0, BaudRateValue = 0, i = 0;

	if(Pn->State == 0x01) BitValue |= PN_EFFECTIVE_BIT_MASK;

	BaudRateValue = Pn->PortBaudRate;
	BaudRateValue *= 300;

	for(i = 0; i < MAX_BAUD_NUM; i++)
	{
		if(UartBaudRate[i] == BaudRateValue) break;
	}
	if(i >= MAX_BAUD_NUM) i = BAUD_RATE_1200_ID;
	
	BitValue |= i << 1;

	if((Pn->PortID == PORT_ID_RS485_1))
	{
		BitValue |= PORT_ID_RS485_1 << 5;
	}
	else if(Pn->PortID == PORT_ID_RS485_2)
	{
		BitValue |= PORT_ID_RS485_2 << 5;
	}
	else if(Pn->PortID == PORT_ID_RS485_3)
	{
		BitValue |= PORT_ID_RS485_3 << 5;
	}
	else if(Pn->PortID == PORT_ID_PLC)
	{
		BitValue |= (PORT_ID_RS485_3 + 1) << 5;
	}
	else// if(Pn->PortID == PORT_ID_WIRELESS)
	{
		BitValue |= (PORT_ID_RS485_3 + 2) << 5;
	}


	if(Pn->ProtocolType == 0)		// 97规约
	{
		BitValue |= 0 << 8;
	}
	else if(Pn->ProtocolType == 1)	// 07规约
	{
		BitValue |= 1 << 8;
	}
	else //if(Pn->ProtocolType == 0x07)	// EDMI规约
	{
		BitValue |= 2 << 8;
	}
//	else
//	{
//		BitValue |= PN_PROTOCOL_BIT_MASK;
//	}


	if(Pn->PointMeterType == METER_TYPE_SINGLE)
	{
		BitValue |= METER_TYPE_SINGLE << 12;
	}
	else if(Pn->PointMeterType == METER_TYPE_MULTI)
	{
		BitValue |= METER_TYPE_MULTI << 12;
	}
	else
	{
		BitValue |= METER_TYPE_OTHER << 12;
	}

	if(Pn->TotalType == 0)
	{
		BitValue |= 0 << 15;
	}
	else
	{
		BitValue |= 1 << 15;
	}

	if(Pn->PriorityType == 0)
	{
		BitValue |= 0 << 17;
	}
	else
	{
		BitValue |= 1 << 17;
	}

	if(Pn->SwitchOff == 0)
	{
		BitValue |= 0 << 18;
	}
	else
	{
		BitValue |= 1 << 18;
	}

	return BitValue;
}

void ConvertBitValueToPnPara(MeasurePointStruct* Pn, u32 BitValue)
{
	u32 i = 0;

	if((BitValue & PN_EFFECTIVE_BIT_MASK) != 0)
	{
		Pn->State = 0x01;
	}
	else
	{
		Pn->State = 0x00;
	}

	i = (BitValue & PN_BAUD_RATE_BIT_MASK) >> 1;
	if(i >= MAX_BAUD_NUM)  i = BAUD_RATE_1200_ID;
	Pn->PortBaudRate = UartBaudRate[i]/300;

	i = (BitValue & PN_PORT_ID_BIT_MASK) >> 5;

	if(i == PORT_ID_RS485_1)
	{
		Pn->PortID = PORT_ID_RS485_1;
	}
	else if(i == PORT_ID_RS485_2)
	{
		Pn->PortID = PORT_ID_RS485_2;
	}
	else if(i == PORT_ID_RS485_3)
	{
		Pn->PortID = PORT_ID_RS485_3;
	}
	else if(i == (PORT_ID_RS485_3 + 1))
	{
		Pn->PortID = PORT_ID_PLC;
	}
	else// if(i == PORT_ID_WIRELESS)
	{
		Pn->PortID = PORT_ID_WIRELESS;
	}

	i = (BitValue & PN_PROTOCOL_BIT_MASK) >> 8;
	//Pn->ProtocolType = i;
	if(i == METER_DLT645_97)
	{
		Pn->ProtocolType = METER_DLT645_97;
	}
	else if(i == METER_DLT645_07)
	{
		Pn->ProtocolType = METER_DLT645_07;
	}
	else// if(i == METER_EDMI)
	{
		Pn->ProtocolType = METER_EDMI;
	}

	

	i = (BitValue & PN_METER_TYPE_BIT_MASK) >> 12;
	Pn->PointMeterType = i;

	i = (BitValue & PN_TOTAL_TYPE_BIT_MASK) >> 15;
	Pn->TotalType = i;

	i = (BitValue & PN_USER_TYPE_BIT_MASK) >> 17;
	Pn->PriorityType = i;

	i = (BitValue & PN_ON_OFF_BIT_MASK) >> 18;
	Pn->SwitchOff = i;	
}


#if Project == 200
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2_1
// Descriptions:        设置测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_1(void)//设置测量点参数
{
	u8 *p8;
	u16 *p16;
	u32 Pn,Ret;
	u64 x;
	MeasurePointStruct MeasurePoint;
	u32 PnNum = 0, FlashAddr = 0;
	u32 i = 0, j = 0, EditRow = 0;	//菜单从第0行开始	
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	StrBuff_TypeDef* DispBuf = StrBuff;
	StrBuff_TypeDef* BackDispBuf = StrBuff2;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_2_1);//显示菜单	
			DisplayString(1,8,0,"0000");
			DisplayString(2,8,0,"无效");																		//有效位		
			DisplayString(3,8,0,"1200");																		//通信速率					
			DisplayString(4,8,0,"RS485_1");																	//端口号				
			DisplayString(5,8,0,"  07协议");																	//协议类型
			DisplayString(6,8,0,"000000000000");
			DisplayString(7,8,0,"000000000000");
			DisplayString(8,8,0,"  单相表");			//电能表类型							
			DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");				//总分类型											
			DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");				//重点户属性								
			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");				//拉闸功能
			MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
			DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
			DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
			DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");
			//DisplayStringOutoffScreenLine(15,0,0,"      确认设置\n\r");
			pMenuCount->v3 = 0x00001104;
			MenuCount->v1=8;
			MenuCount->v2=11;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelectOutofScreenLine();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=8;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 10:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 11:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 12:
							MenuCount->v1=8;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 13:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 14:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);				
							break;
						case 15:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							DisplayStringOutoffScreenLine(8,0,0,"      确认设置      ");
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{							
							case 2:		
								x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"有效");
								}
								else
								{
									pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"无效");
								}
								break;
							case 3:		
								x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
								x >>= 1;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(EditRow,8,0,"1200");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(EditRow,8,0,"2400");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(EditRow,8,0,"4800");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(EditRow,8,0,"9600");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
										break;
								}
								break;
							case 4:		
								x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
								x >>= 5;
								x++;
								if(x > (PORT_ID_RS485_3 + 2)) x = PORT_ID_RS485_1;
								switch(x)
								{
									case PORT_ID_RS485_1:
										DisplayString(EditRow,8,0,"RS485_1");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
										break;
									case PORT_ID_RS485_2:
										DisplayString(EditRow,8,0,"RS485_2");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
										break;
									case PORT_ID_RS485_3:
										DisplayString(EditRow,8,0,"RS485_3");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
										break;
									case (PORT_ID_RS485_3 + 1):
										DisplayString(EditRow,8,0,"   载波");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= (PORT_ID_RS485_3 + 1) << 5;
										break;
									case (PORT_ID_RS485_3 + 2):
									default:
										DisplayString(EditRow,8,0,"   无线");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= (PORT_ID_RS485_3 + 2) << 5;
										break;
								}
								break;
							case 5:	
								x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
								x >>= 8;
								x++;
								if(x > (METER_DLT645_07 + 1) ) x = METER_DLT645_97;
								switch(x)
								{
									case METER_DLT645_97:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 0 << 8;
										DisplayString(EditRow,8,0,"  97协议");				//协议类型
										break;
									case METER_DLT645_07:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 1 << 8;
										DisplayString(EditRow,8,0,"  07协议");				//协议类型
										break;
									case (METER_DLT645_07 + 1):
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 2 << 8;
										DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
										break;
								}
								break;
							case 8:		
								x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
								x >>= 12;
								x++;
								if(x > METER_TYPE_OTHER) x = METER_TYPE_SINGLE;
								switch(x)
								{
									case METER_TYPE_SINGLE:
										DisplayString(EditRow,8,0,"  单相表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
										break;
									case METER_TYPE_MULTI:
										DisplayString(EditRow,8,0,"多功能表");			//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_MULTI << 12;
										break;
									case METER_TYPE_OTHER:
									default:
										DisplayString(EditRow,8,0,"  其它表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_OTHER << 12;
										break;
								}
								break;
							case 9:
								x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
								x >>= 15;
								if(x == 0)
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 1 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
								}
								else
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 0 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
								}
								break;
							case 10:
								x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
								}
								else
								{
									pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
								}
								break;
							case 11:	
								x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");									
								}
								else
								{
									pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");			
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{						
							case 2:		
								x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"有效");
								}
								else
								{
									pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"无效");
								}
								break;
							case 3:		
								x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
								x >>= 1;
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
										DisplayString(EditRow,8,0,"1200");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(EditRow,8,0,"2400");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(EditRow,8,0,"4800");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(EditRow,8,0,"9600");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
										break;
								}
								break;
							case 4:		
								x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
								x >>= 5;
								if(x <= PORT_ID_RS485_1)
								{
									x = PORT_ID_RS485_3 + 2;
								}
								else
								{
									x--;
								}
								switch(x)
								{
									case PORT_ID_RS485_1:
										DisplayString(EditRow,8,0,"RS485_1");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
										break;
									case PORT_ID_RS485_2:
										DisplayString(EditRow,8,0,"RS485_2");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
										break;
									case PORT_ID_RS485_3:
										DisplayString(EditRow,8,0,"RS485_3");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
										break;
									case (PORT_ID_RS485_3 + 1):
										DisplayString(EditRow,8,0,"   载波");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= (PORT_ID_RS485_3 + 1) << 5;
										break;
									case (PORT_ID_RS485_3 + 2):
									default:
										DisplayString(EditRow,8,0,"   无线");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= (PORT_ID_RS485_3 + 2) << 5;
										break;
								}
								break;
							case 5:	
								x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
								x >>= 8;
								if(x <= METER_DLT645_97)
								{
									x = (METER_DLT645_07 + 1);
								}
								else
								{
									x--;
								}
								switch(x)
								{
									case METER_DLT645_97:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 0 << 8;
										DisplayString(EditRow,8,0,"  97协议");				//协议类型
										break;
									case METER_DLT645_07:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 1 << 8;
										DisplayString(EditRow,8,0,"  07协议");				//协议类型
										break;
									case (METER_DLT645_07 + 1):
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 2 << 8;
										DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
										break;
								}
								break;
							case 8:		
								x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
								x >>= 12;
								if(x <= METER_TYPE_SINGLE)
								{
									x = METER_TYPE_OTHER;
								}
								else
								{
									x--;
								}
								switch(x)
								{
									case METER_TYPE_SINGLE:
										DisplayString(EditRow,8,0,"  单相表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
										break;
									case METER_TYPE_MULTI:
										DisplayString(EditRow,8,0,"多功能表");			//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_MULTI << 12;
										break;
									case METER_TYPE_OTHER:
									default:
										DisplayString(EditRow,8,0,"  其它表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_OTHER << 12;
										break;
								}
								break;
							case 9:
								x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
								x >>= 15;
								if(x == 0)
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 1 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
								}
								else
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 0 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
								}
								break;
							case 10:
								x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
								}
								else
								{
									pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
								}
								break;
							case 11:	
								x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");												
								}
								else
								{
									pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
							case 6:
							case 7:
							case 12:
							case 13:
							case 14:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:
							case 4:
							case 5:					
							case 8:
							case 9:
							case 10:
							case 11:						
								break;
							case 15:
								//保存设置的数据
								Stack->MenuStack[MenuCount->Count].Task = 2;
								j = 2;									// 跳出最外层 for 循环
								p8 = (u8*)&BackDispBuf->Row[0].Col[8];
								PnNum = p8_bcd(p8,4);
								if(PnNum > MAX_PN_NUM)
								{//测量点超过最大值 则不可设置
									DisplayString(8,0,0,"错误:测量点号 > 1023");
								}
								else
								{	
									FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
									MR((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
									ConvertBitValueToPnPara(&MeasurePoint, pMenuCount->v3);
									
									for(i = 0; i < METER_ADDR_SIZE; i++)
									{	// 采集器地址显示缓冲，位于备份缓冲区
										MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[6].Col[8 + i*2]);
										MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] <<= 4;
										MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[6].Col[8 + i*2 +1]);
									}

									for(i = 0; i < METER_ADDR_SIZE; i++)
									{	// 电能表地址显示缓冲，位于备份缓冲区
										MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[5].Col[8 + i*2]);
										MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] <<= 4;
										MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[5].Col[8 + i*2 +1]);
									}

									i = CharToHex(DispBuf->Row[5].Col[8]) << 4;
									i += CharToHex(DispBuf->Row[5].Col[9]);
									MeasurePoint.MaxRateNum = i;

									i = CharToHex(DispBuf->Row[6].Col[8]) << 12;
									i += CharToHex(DispBuf->Row[6].Col[9]) << 8;
									i += CharToHex(DispBuf->Row[6].Col[10]) << 4;
									i += CharToHex(DispBuf->Row[6].Col[11]);
									MeasurePoint.CT = i;

									i = CharToHex(DispBuf->Row[7].Col[8]) << 12;
									i += CharToHex(DispBuf->Row[7].Col[9]) << 8;
									i += CharToHex(DispBuf->Row[7].Col[10]) << 4;
									i += CharToHex(DispBuf->Row[7].Col[11]);
									MeasurePoint.PT = i;
									
									Pn=SwitchNumToPn(PnNum);                                     //判断表地址是否重复
									Ret=QCSG_CheckPnAddrIsDuplicate(Pn,MeasurePoint.Addr);
									if(Ret==ERROR_InvalidData)
									{
										DisplayString(8,0,0,"  错误:表地址重复   ");		
									}
									else
									{
										MeasurePoint.PortParity  = 1;                //ybzyl默认设置测量点校验方式为偶校验、数据位为8位，停止位为1位
										MeasurePoint.PortDataBit = 8;                
										MeasurePoint.PortStopBit = 0;
																		
										MW((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
										DisplayString(8,0,0,"         OK         ");

										MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
										pTerminalRam->MeterSyncToRam_S_Timer = METER_FILE_SYNC_DELAY_TIME;
									}
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
				else //if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://测量点
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x > MAX_PN_NUM) bcd_p8(p8,9999,4);
								break;
							case 2://有效位
								break;
							case 3://速率			
								break;
							case 4://端口
								break;
							case 5://协议
								break;
							case 6://通信地址
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,12);
								if(x>999999999999) bcd_p8(p8,0,12);
								break;
							case 7://采集器地址
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,12);
								if(x>999999999999) bcd_p8(p8,0,12);
								break;
							case 8://电能表类型
								break;
							case 9://总分类型
								break;
							case 10://重点户属性
								break;
							case 11://拉闸功能
								break;
							case 12://最大费率数
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x > 16) bcd_p8(p8,0,2);
								break;
							case 13://CT变比
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
							case 14://PT变比
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
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
		default:
			break;
	}
}
#endif



#if (Project == 300) || (Project == 900)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2_1
// Descriptions:        设置测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_1(void)//设置测量点参数
{
	u8 *p8;
	u16 *p16;
	u32 Pn,Ret;
	u64 x;
	MeasurePointStruct MeasurePoint;
	u32 PnNum = 0, FlashAddr = 0;
	u32 i = 0, j = 0, EditRow = 0;	//菜单从第0行开始	
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	StrBuff_TypeDef* DispBuf = StrBuff;
	StrBuff_TypeDef* BackDispBuf = StrBuff2;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_2_1);//显示菜单	
			DisplayString(1,8,0,"0000");
			DisplayString(2,8,0,"无效");																		//有效位		
			DisplayString(3,8,0,"1200");																		//通信速率					
			DisplayString(4,8,0,"RS485_1");																	//端口号				
			DisplayString(5,8,0,"  07协议");																	//协议类型
			DisplayString(6,8,0,"000000000000");
			DisplayString(7,8,0,"000000000000");
			DisplayString(8,8,0,"  单相表");			//电能表类型							
			DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");				//总分类型											
			DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");				//重点户属性								
			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");				//拉闸功能
			MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
			DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
			DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
			DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");
			//DisplayStringOutoffScreenLine(15,0,0,"      确认设置\n\r");
			pMenuCount->v3 = 0x00001104;
			MenuCount->v1=8;
			MenuCount->v2=11;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2:
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				Stack->MenuStack[MenuCount->Count].Task = 0;
				Stack->MenuStack[MenuCount->Count].MoveRow = 0;
				Stack->MenuStack[MenuCount->Count].EditRow = 1;
				Comm_Ram->ButtonVal&=~0x20;
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelectOutofScreenLine();
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=8;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 10:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 11:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 12:
							MenuCount->v1=8;
							MenuCount->v2=9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 13:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 14:
							MenuCount->v1=8;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);				
							break;
						case 15:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							DisplayStringOutoffScreenLine(8,0,0,"      确认设置      ");
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{							
							case 2:		
								x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"有效");
								}
								else
								{
									pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"无效");
								}
								break;
							case 3:		
								x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
								x >>= 1;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(EditRow,8,0,"1200");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(EditRow,8,0,"2400");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(EditRow,8,0,"4800");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(EditRow,8,0,"9600");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
										break;
								}
								break;
							case 4:		
								x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
								x >>= 5;
								x++;
								if(x > PORT_ID_RS485_3) x = PORT_ID_RS485_1;
								switch(x)
								{
									case PORT_ID_RS485_1:
										DisplayString(EditRow,8,0,"RS485_1");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
										break;
									case PORT_ID_RS485_2:
										DisplayString(EditRow,8,0,"RS485_2");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
										break;
									case PORT_ID_RS485_3:
										DisplayString(EditRow,8,0,"RS485_3");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
										break;
								}
								break;
							case 5:	
								x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
								x >>= 8;
								x++;
								if(x > (METER_DLT645_07 + 1) ) x = METER_DLT645_97;
								switch(x)
								{
									case METER_DLT645_97:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 0 << 8;
										DisplayString(EditRow,8,0,"  97协议");				//协议类型
										break;
									case METER_DLT645_07:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 1 << 8;
										DisplayString(EditRow,8,0,"  07协议");				//协议类型
										break;
									case (METER_DLT645_07 + 1):
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 2 << 8;
										DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
										break;
								}
//								if(x == 0)
//								{
//									pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
//									pMenuCount->v3 |= 1 << 8;
//									DisplayString(EditRow,8,0,"07协议");				//协议类型
//								}
//								else
//								{
//									pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
//									pMenuCount->v3 |= 0 << 8;
//									DisplayString(EditRow,8,0,"97协议");				//协议类型
//								}
								break;
							case 8:		
								x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
								x >>= 12;
								x++;
								if(x > METER_TYPE_OTHER) x = METER_TYPE_SINGLE;
								switch(x)
								{
									case METER_TYPE_SINGLE:
										DisplayString(EditRow,8,0,"  单相表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
										break;
									case METER_TYPE_MULTI:
										DisplayString(EditRow,8,0,"多功能表");			//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_MULTI << 12;
										break;
									case METER_TYPE_OTHER:
									default:
										DisplayString(EditRow,8,0,"  其它表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_OTHER << 12;
										break;
								}
								break;
							case 9:
								x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
								x >>= 15;
								if(x == 0)
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 1 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
								}
								else
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 0 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
								}
								break;
							case 10:
								x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
								}
								else
								{
									pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
								}
								break;
							case 11:	
								x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");									
								}
								else
								{
									pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");			
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{						
							case 2:		
								x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"有效");
								}
								else
								{
									pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
									DisplayString(EditRow,8,0,"无效");
								}
								break;
							case 3:		
								x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
								x >>= 1;
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
										DisplayString(EditRow,8,0,"1200");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(EditRow,8,0,"2400");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(EditRow,8,0,"4800");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(EditRow,8,0,"9600");
										pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
										break;
								}
								break;
							case 4:		
								x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
								x >>= 5;
								if(x <= PORT_ID_RS485_1)
								{
									x = PORT_ID_RS485_3;
								}
								else
								{
									x--;
								}
								switch(x)
								{
									case PORT_ID_RS485_1:
										DisplayString(EditRow,8,0,"RS485_1");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
										break;
									case PORT_ID_RS485_2:
										DisplayString(EditRow,8,0,"RS485_2");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
										break;
									case PORT_ID_RS485_3:
										DisplayString(EditRow,8,0,"RS485_3");				//端口号
										pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
										pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
										break;
								}
								break;
							case 5:	
								x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
								x >>= 8;
								if(x <= METER_DLT645_97)
								{
									x = (METER_DLT645_07 + 1);
								}
								else
								{
									x--;
								}
								switch(x)
								{
									case METER_DLT645_97:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 0 << 8;
										DisplayString(EditRow,8,0,"  97协议");				//协议类型
										break;
									case METER_DLT645_07:
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 1 << 8;
										DisplayString(EditRow,8,0,"  07协议");				//协议类型
										break;
									case (METER_DLT645_07 + 1):
										pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
										pMenuCount->v3 |= 2 << 8;
										DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
										break;
								}
//								if(x == 0)
//								{
//									pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
//									pMenuCount->v3 |= 1 << 8;
//									DisplayString(EditRow,8,0,"07协议");				//协议类型
//								}
//								else
//								{
//									pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
//									pMenuCount->v3 |= 0 << 8;
//									DisplayString(EditRow,8,0,"97协议");				//协议类型
//								}
								break;
							case 8:		
								x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
								x >>= 12;
								if(x <= METER_TYPE_SINGLE)
								{
									x = METER_TYPE_OTHER;
								}
								else
								{
									x--;
								}
								switch(x)
								{
									case METER_TYPE_SINGLE:
										DisplayString(EditRow,8,0,"  单相表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
										break;
									case METER_TYPE_MULTI:
										DisplayString(EditRow,8,0,"多功能表");			//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_MULTI << 12;
										break;
									case METER_TYPE_OTHER:
									default:
										DisplayString(EditRow,8,0,"  其它表");				//电能表类型
										pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
										pMenuCount->v3 |= METER_TYPE_OTHER << 12;
										break;
								}
								break;
							case 9:
								x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
								x >>= 15;
								if(x == 0)
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 1 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
								}
								else
								{
									pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
									pMenuCount->v3 |= 0 << 15;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
								}
								break;
							case 10:
								x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
								}
								else
								{
									pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
								}
								break;
							case 11:	
								x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
								if(x == 0)
								{
									pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");												
								}
								else
								{
									pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
									DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认						
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
							case 6:
							case 7:
							case 12:
							case 13:
							case 14:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
							case 3:
							case 4:
							case 5:					
							case 8:
							case 9:
							case 10:
							case 11:						
								break;
							case 15:
								//保存设置的数据
								Stack->MenuStack[MenuCount->Count].Task = 2;
								j = 2;									// 跳出最外层 for 循环
								p8 = (u8*)&BackDispBuf->Row[0].Col[8];
								PnNum = p8_bcd(p8,4);
								if(PnNum > MAX_PN_NUM)
								{//测量点超过最大值 则不可设置
									DisplayString(8,0,0,"错误:测量点号 > 16");
								}
								else
								{	
									FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
									MR((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
									ConvertBitValueToPnPara(&MeasurePoint, pMenuCount->v3);
									
									for(i = 0; i < METER_ADDR_SIZE; i++)
									{	// 采集器地址显示缓冲，位于备份缓冲区
										MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[6].Col[8 + i*2]);
										MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] <<= 4;
										MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[6].Col[8 + i*2 +1]);
									}

									for(i = 0; i < METER_ADDR_SIZE; i++)
									{	// 电能表地址显示缓冲，位于备份缓冲区
										MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[5].Col[8 + i*2]);
										MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] <<= 4;
										MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[5].Col[8 + i*2 +1]);
									}

									i = CharToHex(DispBuf->Row[5].Col[8]) << 4;
									i += CharToHex(DispBuf->Row[5].Col[9]);
									MeasurePoint.MaxRateNum = i;

									i = CharToHex(DispBuf->Row[6].Col[8]) << 12;
									i += CharToHex(DispBuf->Row[6].Col[9]) << 8;
									i += CharToHex(DispBuf->Row[6].Col[10]) << 4;
									i += CharToHex(DispBuf->Row[6].Col[11]);
									MeasurePoint.CT = i;

									i = CharToHex(DispBuf->Row[7].Col[8]) << 12;
									i += CharToHex(DispBuf->Row[7].Col[9]) << 8;
									i += CharToHex(DispBuf->Row[7].Col[10]) << 4;
									i += CharToHex(DispBuf->Row[7].Col[11]);
									MeasurePoint.PT = i;
									
									Pn=SwitchNumToPn(PnNum);                                     //判断表地址是否重复
									Ret=QCSG_CheckPnAddrIsDuplicate(Pn,MeasurePoint.Addr);
									if(Ret==ERROR_InvalidData)
									{
										DisplayString(8,0,0,"  错误:表地址重复   ");		
									}
									else
									{
										if(MeasurePoint.ProtocolType == 0x07)
										{
											MeasurePoint.PortParity  = 0;                //ybzyl红相表默认设置测量点校验方式为无校验、数据位为8位，停止位为1位
											MeasurePoint.PortDataBit = 8;                
											MeasurePoint.PortStopBit = 0;
										}
										else
										{
											MeasurePoint.PortParity  = 1;                //ybzyl默认设置测量点校验方式为偶校验、数据位为8位，停止位为1位
											MeasurePoint.PortDataBit = 8;                
											MeasurePoint.PortStopBit = 0;
										}
																		
										MW((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
										DisplayString(8,0,0,"         OK         ");
										
										if(Pn == 0)//err20190605 菜单设置测量点0地址，同步到交采。
										{
											SetACSampleAddr(MeasurePoint.Addr);
										}

										MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
										pTerminalRam->MeterSyncToRam_S_Timer = METER_FILE_SYNC_DELAY_TIME;
									}
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
				else //if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://测量点
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x > MAX_PN_NUM) bcd_p8(p8,9999,4);
								break;
							case 2://有效位
								break;
							case 3://速率			
								break;
							case 4://端口
								break;
							case 5://协议
								break;
							case 6://通信地址
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,12);
								if(x>999999999999) bcd_p8(p8,0,12);
								break;
							case 7://采集器地址
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,12);
								if(x>999999999999) bcd_p8(p8,0,12);
								break;
							case 8://电能表类型
								break;
							case 9://总分类型
								break;
							case 10://重点户属性
								break;
							case 11://拉闸功能
								break;
							case 12://最大费率数
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x > 16) bcd_p8(p8,0,2);
								break;
							case 13://CT变比
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
							case 14://PT变比
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
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
		default:
			break;
	}
}
#endif






//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2_2
// Descriptions:        查询测量点 
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_2(void)//查询测量点 
{
	u8 *p8;
	u16 *p16;
	u32 PnNum = 0, EditRow = 0, j = 0;
	u64 x;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task = 1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2_2);//显示菜单
			p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
			bcd_p8(p8,0,4);		
			MenuCount->v1=8;
			MenuCount->v2=11;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2://任务2:滚动屏幕查询测量点信息
			BoxSelectOutofScreenLine();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=8;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=8;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=8;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 9:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 10:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 11:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 12:
					MenuCount->v1=8;
					MenuCount->v2=9;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 13:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 14:
					MenuCount->v1=8;
					MenuCount->v2=11;
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
		case 1://任务1:选择查询的测量点
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelectOutofScreenLine();
					EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
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
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								break;
							case 2:
								p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								PnNum=(u32)x;
								DisplayMPInformation(PnNum);
								Stack->MenuStack[MenuCount->Count].Task = 2;
								Stack->MenuStack[MenuCount->Count].EditRow = 1;
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								Stack->MenuStack[MenuCount->Count].EditCol = 0;
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x10;//键使用后清0
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
							p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
							p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
							x=p8_bcd(p8,4);
							if(x>MAX_PN_NUM) bcd_p8(p8,9999,4);
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
// Function name:       Menu_7_2_3
// Descriptions:        修改测量点 
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_3(void)//修改测量点
{
	u8 *p8;
	u16 *p16;
	u64 x;
//	u32 PnNum = 0;
	u32 EditRow = 0, j = 0;
//	u32 FlashLenAddr = 0, FlashLen = 0;	
//	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task = 1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2_2);//显示菜单
			p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
			bcd_p8(p8,0,4);		
			MenuCount->v1=8;
			MenuCount->v2=11;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 2://任务2:滚动屏幕查询测量点信息
			BoxSelectOutofScreenLine();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=8;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=8;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=8;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=8;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 9:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 10:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 11:
					MenuCount->v1=8;
					MenuCount->v2=15;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 12:
					MenuCount->v1=8;
					MenuCount->v2=9;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 13:
					MenuCount->v1=8;
					MenuCount->v2=11;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 14:
					MenuCount->v1=8;
					MenuCount->v2=11;
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
		case 1://任务1:选择查询的测量点
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelectOutofScreenLine();
					EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
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
								p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								break;
							case 2:
								p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								//PnNum=(u32)x;
//								FlashLen = x;
//								FlashLenAddr = ADDR_MP_START + (FlashLen * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
//								FlashLen = sizeof(MeasurePointStruct) ;
//								MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
//								if((MeasurePoint.State == 0x01) && (BCD_Check(MeasurePoint.Addr, METER_ADDR_SIZE) == NO_ERROR))
//								{
									Pn_Lcd_Data->u16Pn = x;							
//								}
//								else
//								{
//									Pn_Lcd_Data->u16Pn = 0xFFFF;
//								}								
								MenuCreate((u32)Menu_7_2_3_1);//修改测量点     
//								Stack->MenuStack[MenuCount->Count].Task = 0;
//								Stack->MenuStack[MenuCount->Count].EditRow = 1;
//								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
//								Stack->MenuStack[MenuCount->Count].EditCol = 0;
//								MenuCount->v1=8;
//								MenuCount->v2=11;
//								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x10;//键使用后清0
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
							p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
							p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
							x=p8_bcd(p8,4);
							if(x>MAX_PN_NUM) bcd_p8(p8,9999,4);
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




#if (Project == 200)|| (Project == 900)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2_3_1
// Descriptions:        修改测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_3_1(void)//集中器修改测量点参数
{
	u8 *p8,*p;
	u16 *p16;
	u32 Pn,Ret;
	u64 x;
	MeasurePointStruct MeasurePoint;
	u32 PnNum = 0, FlashAddr = 0;
	u32 FlashLenAddr = 0,FlashLen = 0;
	u32 i = 0, j = 0,Ln = 0, EditRow = 0;	//菜单从第0行开始	
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	StrBuff_TypeDef* DispBuf = StrBuff;
	StrBuff_TypeDef* BackDispBuf = StrBuff2;


	FlashAddr = ADDR_MP_START + (Pn_Lcd_Data->u16Pn) * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
	MR((u32)&MeasurePoint,FlashAddr,sizeof(MeasurePointStruct));
	if(MemCompare(MeasurePoint.Addr, EmptyAddr, METER_ADDR_SIZE) == 0)
	{
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(4,4,0,"测量点不存在");
	}
	else
	{
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayClr();//清全屏
				MenuCount->v1=0;
				MenuCount->v2=0;
				MenuCount->v3=0;
				DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_2_1);//显示菜单	
	//			DisplayString(8,8,0,"  单相表");			//电能表类型							
	//			DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");				//总分类型											
	//			DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");				//重点户属性								
	//			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");				//拉闸功能
	//			MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
	//			DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
	//			DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
	//			DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");
	//			//DisplayStringOutoffScreenLine(15,0,0,"      确认设置\n\r");
	//			pMenuCount->v3 = 0x00001104;

				FlashLenAddr = ADDR_MP_START + (Pn_Lcd_Data->u16Pn * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
				FlashLen = sizeof(MeasurePointStruct) ;
				MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
				//第一行测量点号
				p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+8*2);			
				bcd_p8(p,Pn_Lcd_Data->u16Pn,4);
				//第二行测量点有效性
				switch(MeasurePoint.State)
				{
					case 0:
						DisplayString(Ln+2,8,0,"无效");
						break;
					case 1:
						DisplayString(Ln+2,8,0,"有效");	
						break;
					default:
						DisplayString(Ln+2,8,0,"未知");
						break;
				}
				//第三行波特率
				switch(MeasurePoint.PortBaudRate)
				{
					case 4:
						DisplayString(Ln+3,8,0,"1200");
						break;
					case 8:
						DisplayString(Ln+3,8,0,"2400");	
						break;
					case 0x10:
						DisplayString(Ln+3,8,0,"4800");
						break;
					case 0x20:
						DisplayString(Ln+3,8,0,"9600");	
						break;
					default:
						DisplayString(Ln+3,8,0,"未知");
						break;
				}
				//第四行端口号
				switch(MeasurePoint.PortID)
				{
					case PORT_ID_RS485_1:
						DisplayString(4,8,0,"RS485_1");
						break;									
					case PORT_ID_RS485_2:
						DisplayString(4,8,0,"RS485_2");		
						break;
					case PORT_ID_RS485_3:
						DisplayString(4,8,0,"RS485_3");
						break;
					case PORT_ID_PLC:
						DisplayString(4,8,0,"   载波");
						break;
					case PORT_ID_WIRELESS:
						DisplayString(4,8,0,"   无线");
						break;
					default:
						DisplayString(4,8,0,"   未知");
						break;
				}
				//第五行协议类型
				switch(MeasurePoint.ProtocolType)
				{
					case 0:
						DisplayString(Ln+5,8,0,"  97协议");
						break;
					case 1:
						DisplayString(Ln+5,8,0,"  07协议");	
						break;
					case 0x07:
						DisplayString(Ln+5,8,0,"EDMI协议");	
						break;
					default:
						DisplayString(Ln+5,8,0,"    保留");
						break;
				}
				//第六行测量点地址
				for(i = 0; i < METER_ADDR_SIZE; i++)
				{
					DispBuf->Row[6].Col[8 + i*2 + 1] = 0x2000 + HexToChar[MeasurePoint.Addr[METER_ADDR_SIZE-i-1] & 0x0F];
					DispBuf->Row[6].Col[8 + i*2] = 0x2000 + HexToChar[(MeasurePoint.Addr[METER_ADDR_SIZE-i-1] >> 4) & 0x0F];
				}
				//第七行采集器地址
				for(i = 0; i < METER_ADDR_SIZE; i++)
				{
					DispBuf->Row[7].Col[8 + i*2 + 1] = 0x2000 + HexToChar[MeasurePoint.CollectorAddr[METER_ADDR_SIZE-i-1] & 0x0F];
					DispBuf->Row[7].Col[8 + i*2] = 0x2000 + HexToChar[(MeasurePoint.CollectorAddr[METER_ADDR_SIZE-i-1] >> 4) & 0x0F];
				}

				switch(MeasurePoint.PointMeterType)
				{
					case METER_TYPE_SINGLE:
						DisplayString(8,8,0,"  单相表");
						break;
					case METER_TYPE_MULTI:
						DisplayString(8,8,0,"多功能表");
						break;
					default:
						DisplayString(8,8,0,"  其他表");
						break;
				}
				if(MeasurePoint.TotalType == 0)
				{
					DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");		    //总分类型	
				}	
				else if(MeasurePoint.TotalType == 1)
				{
					DisplayStringOutoffScreenLine(10,0,0,"总分类型    总表");		    //总分类型	
				}
				else
				{
					DisplayStringOutoffScreenLine(10,0,0,"总分类型    未知");		    //总分类型
				}
									
				if(MeasurePoint.PriorityType == 0)
				{
					DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");       		//重点户属性		
				}	
				else if(MeasurePoint.PriorityType == 1)
				{
					DisplayStringOutoffScreenLine(11,0,0,"重点户  重点用户");		        //重点户属性		
				}
				else
				{
					DisplayStringOutoffScreenLine(11,0,0,"重点户      未知");		        //重点户属性
				}								
				if(MeasurePoint.SwitchOff == 0)
				{
					DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");		            //拉闸功能		
				}	
				else if(MeasurePoint.SwitchOff == 1)
				{
					DisplayStringOutoffScreenLine(12,0,0,"拉闸功能  带拉闸");		        //拉闸功能
				}
				else
				{
					DisplayStringOutoffScreenLine(12,0,0,"拉闸功能    未知");	         	//拉闸功能
				}

				MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
				DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
				DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
				DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");		
				
				x = bcd_hex(MeasurePoint.MaxRateNum);
				p8=(u8*)(ADDR_STRINGBUFF+13*84+4+8*2);
				bcd_p8(p8, x, 2);

				x = bcd_hex(MeasurePoint.CT);
				p8=(u8*)(ADDR_STRINGBUFF+14*84+4+8*2);
				bcd_p8(p8, x, 4);		

				x = bcd_hex(MeasurePoint.PT);
				p8=(u8*)(ADDR_STRINGBUFF+15*84+4+8*2);
				bcd_p8(p8, x, 4);
				
				MenuCount->v3 = ConvertPnParaToBitValue(&MeasurePoint);
				MenuCount->v1=8;
				MenuCount->v2=11;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
				break;
			case 2:
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					Stack->MenuStack[MenuCount->Count].Task = 0;
					Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					Stack->MenuStack[MenuCount->Count].EditRow = 1;
					Comm_Ram->ButtonVal&=~0x20;
				}
				break;
			case 1://任务1:选择
				for(j=0;j<2;j++)
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol==0)
					{
						BoxSelectOutofScreenLine();
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 2:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 3:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 4:
								MenuCount->v1=8;
								MenuCount->v2=14;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 5:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 6:
								MenuCount->v1=8;
								MenuCount->v2=19;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 7:
								MenuCount->v1=8;
								MenuCount->v2=19;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 8:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 9:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 10:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 11:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 12:
								MenuCount->v1=8;
								MenuCount->v2=9;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 13:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 14:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);				
								break;
							case 15:
								MenuCount->v1=0;
								MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
								DisplayStringOutoffScreenLine(8,0,0,"      确认设置      ");
								break;
							default:
								break;
						}
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//右键
							EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{							
								case 2:		
									x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"有效");
									}
									else
									{
										pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"无效");
									}
									break;
								case 3:		
									x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
									x >>= 1;
									x++;
									if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
									switch(x)
									{
										case BAUD_RATE_1200_ID:
											DisplayString(EditRow,8,0,"1200");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
											break;
										case BAUD_RATE_2400_ID:
											DisplayString(EditRow,8,0,"2400");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
											break;
										case BAUD_RATE_4800_ID:
											DisplayString(EditRow,8,0,"4800");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
											break;
										case BAUD_RATE_9600_ID:
										default:
											DisplayString(EditRow,8,0,"9600");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
											break;
									}
									break;
								case 4:		
									x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
									x >>= 5;
									x++;
									if(x > (PORT_ID_RS485_3 + 2)) x = PORT_ID_RS485_1;
									switch(x)
									{
										case PORT_ID_RS485_1:
											DisplayString(EditRow,8,0,"RS485_1");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
											break;
										case PORT_ID_RS485_2:
											DisplayString(EditRow,8,0,"RS485_2");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
											break;
										case PORT_ID_RS485_3:
											DisplayString(EditRow,8,0,"RS485_3");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
											break;
										case (PORT_ID_RS485_3 + 1):
											DisplayString(EditRow,8,0,"   载波");				    //端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= (PORT_ID_RS485_3 + 1) << 5;
											break;
										case (PORT_ID_RS485_3 + 2):
										default:
											DisplayString(EditRow,8,0,"   无线");				    //端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= (PORT_ID_RS485_3 + 2) << 5;
											break;
									}
									break;
								case 5:	
									x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
									x >>= 8;
									x++;
									if(x > (METER_DLT645_07 + 1) ) x = METER_DLT645_97;
									switch(x)
									{
										case METER_DLT645_97:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 0 << 8;
											DisplayString(EditRow,8,0,"  97协议");				//协议类型
											break;
										case METER_DLT645_07:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 1 << 8;
											DisplayString(EditRow,8,0,"  07协议");				//协议类型
											break;
										case (METER_DLT645_07 + 1):
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 2 << 8;
											DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
											break;
									}
									break;
								case 8:		
									x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
									x >>= 12;
									x++;
									if(x > METER_TYPE_OTHER) x = METER_TYPE_SINGLE;
									switch(x)
									{
										case METER_TYPE_SINGLE:
											DisplayString(EditRow,8,0,"  单相表");				//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
											break;
										case METER_TYPE_MULTI:
											DisplayString(EditRow,8,0,"多功能表");			   //电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_MULTI << 12;
											break;
										case METER_TYPE_OTHER:
										default:
											DisplayString(EditRow,8,0,"  其它表");			   //电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_OTHER << 12;
											break;
									}
									break;
								case 9:
									x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
									x >>= 15;
									if(x == 0)
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 1 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
									}
									else
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 0 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
									}
									break;
								case 10:
									x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
									}
									else
									{
										pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
									}
									break;
								case 11:	
									x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");									
									}
									else
									{
										pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");			
									}
									break;
								default:
									break;
							}
						}
						if((Comm_Ram->ButtonVal&0x04)!=0)
						{//左键
							EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{						
								case 2:		
									x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"有效");
									}
									else
									{
										pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"无效");
									}
									break;
								case 3:		
									x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
									x >>= 1;
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
											DisplayString(EditRow,8,0,"1200");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
											break;
										case BAUD_RATE_2400_ID:
											DisplayString(EditRow,8,0,"2400");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
											break;
										case BAUD_RATE_4800_ID:
											DisplayString(EditRow,8,0,"4800");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
											break;
										case BAUD_RATE_9600_ID:
										default:
											DisplayString(EditRow,8,0,"9600");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
											break;
									}
									break;
								case 4:		
									x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
									x >>= 5;
									if(x <= PORT_ID_RS485_1)
									{
										x = PORT_ID_RS485_3 + 2;
									}
									else
									{
										x--;
									}
									switch(x)
									{
										case PORT_ID_RS485_1:
											DisplayString(EditRow,8,0,"RS485_1");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
											break;
										case PORT_ID_RS485_2:
											DisplayString(EditRow,8,0,"RS485_2");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
											break;
										case PORT_ID_RS485_3:
											DisplayString(EditRow,8,0,"RS485_3");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
											break;
										case (PORT_ID_RS485_3 + 1):
											DisplayString(EditRow,8,0,"   载波");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= (PORT_ID_RS485_3 + 1) << 5;
											break;
										case (PORT_ID_RS485_3 + 2):
										default:
											DisplayString(EditRow,8,0,"   无线");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= (PORT_ID_RS485_3 + 2) << 5;
											break;
									}
									break;
								case 5:	
									x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
									x >>= 8;
									if(x <= METER_DLT645_97)
									{
										x = (METER_DLT645_07 + 1);
									}
									else
									{
										x--;
									}
									switch(x)
									{
										case METER_DLT645_97:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 0 << 8;
											DisplayString(EditRow,8,0,"  97协议");				//协议类型
											break;
										case METER_DLT645_07:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 1 << 8;
											DisplayString(EditRow,8,0,"  07协议");				//协议类型
											break;
										case (METER_DLT645_07 + 1):
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 2 << 8;
											DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
											break;
									}
									break;
								case 8:		
									x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
									x >>= 12;
									if(x <= METER_TYPE_SINGLE)
									{
										x = METER_TYPE_OTHER;
									}
									else
									{
										x--;
									}
									switch(x)
									{
										case METER_TYPE_SINGLE:
											DisplayString(EditRow,8,0,"  单相表");				//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
											break;
										case METER_TYPE_MULTI:
											DisplayString(EditRow,8,0,"多功能表");			//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_MULTI << 12;
											break;
										case METER_TYPE_OTHER:
										default:
											DisplayString(EditRow,8,0,"  其它表");				//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_OTHER << 12;
											break;
									}
									break;
								case 9:
									x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
									x >>= 15;
									if(x == 0)
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 1 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
									}
									else
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 0 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
									}
									break;
								case 10:
									x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
									}
									else
									{
										pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
									}
									break;
								case 11:	
									x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");												
									}
									else
									{
										pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");
									}
									break;
								default:
									break;
							}
						}
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确认						
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{
								case 1:
								case 6:
								case 7:
								case 12:
								case 13:
								case 14:
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
									break;
								case 2:
								case 3:
								case 4:
								case 5:					
								case 8:
								case 9:
								case 10:
								case 11:						
									break;
								case 15:
									//保存设置的数据
									Stack->MenuStack[MenuCount->Count].Task = 2;
									j = 2;									// 跳出最外层 for 循环
									p8 = (u8*)&BackDispBuf->Row[0].Col[8];
									PnNum = p8_bcd(p8,4);
									if(PnNum > MAX_PN_NUM)
									{//测量点超过最大值 则不可设置
										DisplayString(8,0,0,"错误:测量点号 > 1023");
									}
									else
									{	
										FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
										MR((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
										ConvertBitValueToPnPara(&MeasurePoint, pMenuCount->v3);
										
										for(i = 0; i < METER_ADDR_SIZE; i++)
										{	// 采集器地址显示缓冲，位于备份缓冲区
											MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[6].Col[8 + i*2]);
											MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] <<= 4;
											MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[6].Col[8 + i*2 +1]);
										}

										for(i = 0; i < METER_ADDR_SIZE; i++)
										{	// 电能表地址显示缓冲，位于备份缓冲区
											MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[5].Col[8 + i*2]);
											MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] <<= 4;
											MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[5].Col[8 + i*2 +1]);
										}

										i = CharToHex(DispBuf->Row[5].Col[8]) << 4;
										i += CharToHex(DispBuf->Row[5].Col[9]);
										MeasurePoint.MaxRateNum = i;

										i = CharToHex(DispBuf->Row[6].Col[8]) << 12;
										i += CharToHex(DispBuf->Row[6].Col[9]) << 8;
										i += CharToHex(DispBuf->Row[6].Col[10]) << 4;
										i += CharToHex(DispBuf->Row[6].Col[11]);
										MeasurePoint.CT = i;

										i = CharToHex(DispBuf->Row[7].Col[8]) << 12;
										i += CharToHex(DispBuf->Row[7].Col[9]) << 8;
										i += CharToHex(DispBuf->Row[7].Col[10]) << 4;
										i += CharToHex(DispBuf->Row[7].Col[11]);
										MeasurePoint.PT = i;
										
										Pn=SwitchNumToPn(PnNum);                                     //判断表地址是否重复
										Ret=QCSG_CheckPnAddrIsDuplicate(Pn,MeasurePoint.Addr);
										if(Ret==ERROR_InvalidData)
										{
											DisplayString(8,0,0,"  错误:表地址重复   ");		
										}
										else
										{
											MeasurePoint.PortParity  = 1;                //ybzyl默认设置测量点校验方式为偶校验、数据位为8位，停止位为1位
											MeasurePoint.PortDataBit = 8;                
											MeasurePoint.PortStopBit = 0;
																			
											MW((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
											DisplayString(8,0,0,"         OK         ");

											MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
											pTerminalRam->MeterSyncToRam_S_Timer = METER_FILE_SYNC_DELAY_TIME;
										}
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
					else //if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
					{
						Select(MenuCount->v2,MenuCount->v1);
						UpDownSelect();
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确认
							EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{
								case 1://测量点
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,4);
									if(x > MAX_PN_NUM) bcd_p8(p8,9999,4);
									break;
								case 2://有效位
									break;
								case 3://速率			
									break;
								case 4://端口
									break;
								case 5://协议
									break;
								case 6://通信地址
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,12);
									if(x>999999999999) bcd_p8(p8,0,12);
									break;
								case 7://采集器地址
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,12);
									if(x>999999999999) bcd_p8(p8,0,12);
									break;
								case 8://电能表类型
									break;
								case 9://总分类型
									break;
								case 10://重点户属性
									break;
								case 11://拉闸功能
									break;
								case 12://最大费率数
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,2);
									if(x > 16) bcd_p8(p8,0,2);
									break;
								case 13://CT变比
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
								case 14://PT变比
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
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
			default:
				break;
		}
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
#endif



#if (Project == 300)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2_3_1
// Descriptions:        修改测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_3_1(void)//负控修改测量点参数
{
	u8 *p8,*p;
	u16 *p16;
	u32 Pn,Ret;
	u64 x;
	MeasurePointStruct MeasurePoint;
	u32 PnNum = 0, FlashAddr = 0;
	u32 FlashLenAddr = 0,FlashLen = 0;
	u32 i = 0, j = 0,Ln = 0, EditRow = 0;	//菜单从第0行开始	
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	StrBuff_TypeDef* DispBuf = StrBuff;
	StrBuff_TypeDef* BackDispBuf = StrBuff2;

	
	FlashAddr = ADDR_MP_START + (Pn_Lcd_Data->u16Pn) * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
	MR((u32)&MeasurePoint,FlashAddr,sizeof(MeasurePointStruct));
	if(MemCompare(MeasurePoint.Addr, EmptyAddr, METER_ADDR_SIZE) == 0)
	{
		DisplayClrRow(1,6);//清屏幕显示的指定行
		DisplayString(4,4,0,"测量点不存在");
	}
	else
	{
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayClr();//清全屏
				MenuCount->v1=0;
				MenuCount->v2=0;
				MenuCount->v3=0;
				DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_2_1);//显示菜单	
	//			DisplayString(8,8,0,"  单相表");			//电能表类型							
	//			DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");				//总分类型											
	//			DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");				//重点户属性								
	//			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");				//拉闸功能
	//			MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
	//			DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
	//			DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
	//			DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");
	//			//DisplayStringOutoffScreenLine(15,0,0,"      确认设置\n\r");
	//			pMenuCount->v3 = 0x00001104;

				FlashLenAddr = ADDR_MP_START + (Pn_Lcd_Data->u16Pn * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
				FlashLen = sizeof(MeasurePointStruct) ;
				MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
				//第一行测量点号
				p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+8*2);			
				bcd_p8(p,Pn_Lcd_Data->u16Pn,4);
				//第二行测量点有效性
				switch(MeasurePoint.State)
				{
					case 0:
						DisplayString(Ln+2,8,0,"无效");
						break;
					case 1:
						DisplayString(Ln+2,8,0,"有效");	
						break;
					default:
						DisplayString(Ln+2,8,0,"未知");
						break;
				}
				//第三行波特率
				switch(MeasurePoint.PortBaudRate)
				{
					case 4:
						DisplayString(Ln+3,8,0,"1200");
						break;
					case 8:
						DisplayString(Ln+3,8,0,"2400");	
						break;
					case 0x10:
						DisplayString(Ln+3,8,0,"4800");
						break;
					case 0x20:
						DisplayString(Ln+3,8,0,"9600");	
						break;
					default:
						DisplayString(Ln+3,8,0,"未知");
						break;
				}
				//第四行端口号
				switch(MeasurePoint.PortID)
				{
					case PORT_ID_RS485_1:
						DisplayString(4,8,0,"RS485_1");
						break;									
					case PORT_ID_RS485_2:
						DisplayString(4,8,0,"RS485_2");		
						break;
					case PORT_ID_RS485_3:
						DisplayString(4,8,0,"RS485_3");
						break;
					default:
						DisplayString(4,8,0,"   未知");
						break;
				}
				//第五行协议类型
				switch(MeasurePoint.ProtocolType)
				{
					case 0:
						DisplayString(Ln+5,8,0,"  97协议");
						break;
					case 1:
						DisplayString(Ln+5,8,0,"  07协议");	
						break;
					case 0x07:
						DisplayString(Ln+5,8,0,"EDMI协议");	
						break;
					default:
						DisplayString(Ln+5,8,0,"    保留");
						break;
				}
				//第六行测量点地址
				for(i = 0; i < METER_ADDR_SIZE; i++)
				{
					DispBuf->Row[6].Col[8 + i*2 + 1] = 0x2000 + HexToChar[MeasurePoint.Addr[METER_ADDR_SIZE-i-1] & 0x0F];
					DispBuf->Row[6].Col[8 + i*2] = 0x2000 + HexToChar[(MeasurePoint.Addr[METER_ADDR_SIZE-i-1] >> 4) & 0x0F];
				}
				//第七行采集器地址
				for(i = 0; i < METER_ADDR_SIZE; i++)
				{
					DispBuf->Row[7].Col[8 + i*2 + 1] = 0x2000 + HexToChar[MeasurePoint.CollectorAddr[METER_ADDR_SIZE-i-1] & 0x0F];
					DispBuf->Row[7].Col[8 + i*2] = 0x2000 + HexToChar[(MeasurePoint.CollectorAddr[METER_ADDR_SIZE-i-1] >> 4) & 0x0F];
				}

				switch(MeasurePoint.PointMeterType)
				{
					case METER_TYPE_SINGLE:
						DisplayString(8,8,0,"  单相表");
						break;
					case METER_TYPE_MULTI:
						DisplayString(8,8,0,"多功能表");
						break;
					default:
						DisplayString(8,8,0,"  其他表");
						break;
				}
				if(MeasurePoint.TotalType == 0)
				{
					DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");		    //总分类型	
				}	
				else if(MeasurePoint.TotalType == 1)
				{
					DisplayStringOutoffScreenLine(10,0,0,"总分类型    总表");		    //总分类型	
				}
				else
				{
					DisplayStringOutoffScreenLine(10,0,0,"总分类型    未知");		    //总分类型
				}
									
				if(MeasurePoint.PriorityType == 0)
				{
					DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");       		//重点户属性		
				}	
				else if(MeasurePoint.PriorityType == 1)
				{
					DisplayStringOutoffScreenLine(11,0,0,"重点户  重点用户");		        //重点户属性		
				}
				else
				{
					DisplayStringOutoffScreenLine(11,0,0,"重点户      未知");		        //重点户属性
				}								
				if(MeasurePoint.SwitchOff == 0)
				{
					DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");		            //拉闸功能		
				}	
				else if(MeasurePoint.SwitchOff == 1)
				{
					DisplayStringOutoffScreenLine(12,0,0,"拉闸功能  带拉闸");		        //拉闸功能
				}
				else
				{
					DisplayStringOutoffScreenLine(12,0,0,"拉闸功能    未知");	         	//拉闸功能
				}

				MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
				DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
				DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
				DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");		
				
				x = bcd_hex(MeasurePoint.MaxRateNum);
				p8=(u8*)(ADDR_STRINGBUFF+13*84+4+8*2);
				bcd_p8(p8, x, 2);

				x = bcd_hex(MeasurePoint.CT);
				p8=(u8*)(ADDR_STRINGBUFF+14*84+4+8*2);
				bcd_p8(p8, x, 4);		

				x = bcd_hex(MeasurePoint.PT);
				p8=(u8*)(ADDR_STRINGBUFF+15*84+4+8*2);
				bcd_p8(p8, x, 4);
				
				MenuCount->v3 = ConvertPnParaToBitValue(&MeasurePoint);
				MenuCount->v1=8;
				MenuCount->v2=11;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
				break;
			case 2:
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					Stack->MenuStack[MenuCount->Count].Task = 0;
					Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					Stack->MenuStack[MenuCount->Count].EditRow = 1;
					Comm_Ram->ButtonVal&=~0x20;
				}
				break;
			case 1://任务1:选择
				for(j=0;j<2;j++)
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol==0)
					{
						BoxSelectOutofScreenLine();
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 2:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 3:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 4:
								MenuCount->v1=8;
								MenuCount->v2=14;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 5:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 6:
								MenuCount->v1=8;
								MenuCount->v2=19;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 7:
								MenuCount->v1=8;
								MenuCount->v2=19;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 8:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 9:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 10:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 11:
								MenuCount->v1=8;
								MenuCount->v2=15;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 12:
								MenuCount->v1=8;
								MenuCount->v2=9;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 13:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
								break;
							case 14:
								MenuCount->v1=8;
								MenuCount->v2=11;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);				
								break;
							case 15:
								MenuCount->v1=0;
								MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
								DisplayStringOutoffScreenLine(8,0,0,"      确认设置      ");
								break;
							default:
								break;
						}
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//右键
							EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{							
								case 2:		
									x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"有效");
									}
									else
									{
										pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"无效");
									}
									break;
								case 3:		
									x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
									x >>= 1;
									x++;
									if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
									switch(x)
									{
										case BAUD_RATE_1200_ID:
											DisplayString(EditRow,8,0,"1200");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
											break;
										case BAUD_RATE_2400_ID:
											DisplayString(EditRow,8,0,"2400");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
											break;
										case BAUD_RATE_4800_ID:
											DisplayString(EditRow,8,0,"4800");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
											break;
										case BAUD_RATE_9600_ID:
										default:
											DisplayString(EditRow,8,0,"9600");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
											break;
									}
									break;
								case 4:		
									x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
									x >>= 5;
									x++;
									if(x > PORT_ID_RS485_3) x = PORT_ID_RS485_1;
									switch(x)
									{
										case PORT_ID_RS485_1:
											DisplayString(EditRow,8,0,"RS485_1");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
											break;
										case PORT_ID_RS485_2:
											DisplayString(EditRow,8,0,"RS485_2");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
											break;
										case PORT_ID_RS485_3:
											DisplayString(EditRow,8,0,"RS485_3");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
											break;
//										case (PORT_ID_RS485_3 + 1):
//											DisplayString(EditRow,8,0,"   载波");				    //端口号
//											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
//											pMenuCount->v3 |= (PORT_ID_RS485_3 + 1) << 5;
//											break;
//										case (PORT_ID_RS485_3 + 2):
//										default:
//											DisplayString(EditRow,8,0,"   无线");				    //端口号
//											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
//											pMenuCount->v3 |= (PORT_ID_RS485_3 + 2) << 5;
//											break;
									}
									break;
								case 5:	
									x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
									x >>= 8;
									x++;
									if(x > (METER_DLT645_07 + 1) ) x = METER_DLT645_97;
									switch(x)
									{
										case METER_DLT645_97:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 0 << 8;
											DisplayString(EditRow,8,0,"  97协议");				//协议类型
											break;
										case METER_DLT645_07:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 1 << 8;
											DisplayString(EditRow,8,0,"  07协议");				//协议类型
											break;
										case (METER_DLT645_07 + 1):
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 2 << 8;
											DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
											break;
									}
									break;
								case 8:		
									x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
									x >>= 12;
									x++;
									if(x > METER_TYPE_OTHER) x = METER_TYPE_SINGLE;
									switch(x)
									{
										case METER_TYPE_SINGLE:
											DisplayString(EditRow,8,0,"  单相表");				//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
											break;
										case METER_TYPE_MULTI:
											DisplayString(EditRow,8,0,"多功能表");			   //电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_MULTI << 12;
											break;
										case METER_TYPE_OTHER:
										default:
											DisplayString(EditRow,8,0,"  其它表");			   //电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_OTHER << 12;
											break;
									}
									break;
								case 9:
									x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
									x >>= 15;
									if(x == 0)
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 1 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
									}
									else
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 0 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
									}
									break;
								case 10:
									x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
									}
									else
									{
										pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
									}
									break;
								case 11:	
									x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");									
									}
									else
									{
										pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");			
									}
									break;
								default:
									break;
							}
						}
						if((Comm_Ram->ButtonVal&0x04)!=0)
						{//左键
							EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{						
								case 2:		
									x = pMenuCount->v3 & PN_EFFECTIVE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"有效");
									}
									else
									{
										pMenuCount->v3 &= ~PN_EFFECTIVE_BIT_MASK;
										DisplayString(EditRow,8,0,"无效");
									}
									break;
								case 3:		
									x = pMenuCount->v3 & PN_BAUD_RATE_BIT_MASK;
									x >>= 1;
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
											DisplayString(EditRow,8,0,"1200");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_1200_ID << 1;
											break;
										case BAUD_RATE_2400_ID:
											DisplayString(EditRow,8,0,"2400");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_2400_ID << 1;
											break;
										case BAUD_RATE_4800_ID:
											DisplayString(EditRow,8,0,"4800");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_4800_ID << 1;
											break;
										case BAUD_RATE_9600_ID:
										default:
											DisplayString(EditRow,8,0,"9600");
											pMenuCount->v3 &= ~PN_BAUD_RATE_BIT_MASK;
											pMenuCount->v3 |= BAUD_RATE_9600_ID << 1;
											break;
									}
									break;
								case 4:		
									x = pMenuCount->v3 & PN_PORT_ID_BIT_MASK;
									x >>= 5;
									if(x <= PORT_ID_RS485_1)
									{
										x = PORT_ID_RS485_3;
									}
									else
									{
										x--;
									}
									switch(x)
									{
										case PORT_ID_RS485_1:
											DisplayString(EditRow,8,0,"RS485_1");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_1 << 5;
											break;
										case PORT_ID_RS485_2:
											DisplayString(EditRow,8,0,"RS485_2");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_2 << 5;
											break;
										case PORT_ID_RS485_3:
											DisplayString(EditRow,8,0,"RS485_3");				//端口号
											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
											pMenuCount->v3 |= PORT_ID_RS485_3 << 5;
											break;
//										case (PORT_ID_RS485_3 + 1):
//											DisplayString(EditRow,8,0,"   载波");				//端口号
//											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
//											pMenuCount->v3 |= (PORT_ID_RS485_3 + 1) << 5;
//											break;
//										case (PORT_ID_RS485_3 + 2):
//										default:
//											DisplayString(EditRow,8,0,"   无线");				//端口号
//											pMenuCount->v3 &= ~PN_PORT_ID_BIT_MASK;
//											pMenuCount->v3 |= (PORT_ID_RS485_3 + 2) << 5;
//											break;
									}
									break;
								case 5:	
									x = pMenuCount->v3 & PN_PROTOCOL_BIT_MASK;
									x >>= 8;
									if(x <= METER_DLT645_97)
									{
										x = (METER_DLT645_07 + 1);
									}
									else
									{
										x--;
									}
									switch(x)
									{
										case METER_DLT645_97:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 0 << 8;
											DisplayString(EditRow,8,0,"  97协议");				//协议类型
											break;
										case METER_DLT645_07:
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 1 << 8;
											DisplayString(EditRow,8,0,"  07协议");				//协议类型
											break;
										case (METER_DLT645_07 + 1):
											pMenuCount->v3 &= ~PN_PROTOCOL_BIT_MASK;
											pMenuCount->v3 |= 2 << 8;
											DisplayString(EditRow,8,0,"EDMI协议");				//协议类型
											break;
									}
									break;
								case 8:		
									x = pMenuCount->v3 & PN_METER_TYPE_BIT_MASK;
									x >>= 12;
									if(x <= METER_TYPE_SINGLE)
									{
										x = METER_TYPE_OTHER;
									}
									else
									{
										x--;
									}
									switch(x)
									{
										case METER_TYPE_SINGLE:
											DisplayString(EditRow,8,0,"  单相表");				//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_SINGLE << 12;
											break;
										case METER_TYPE_MULTI:
											DisplayString(EditRow,8,0,"多功能表");			//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_MULTI << 12;
											break;
										case METER_TYPE_OTHER:
										default:
											DisplayString(EditRow,8,0,"  其它表");				//电能表类型
											pMenuCount->v3 &= ~PN_METER_TYPE_BIT_MASK;
											pMenuCount->v3 |= METER_TYPE_OTHER << 12;
											break;
									}
									break;
								case 9:
									x = pMenuCount->v3 & PN_TOTAL_TYPE_BIT_MASK;
									x >>= 15;
									if(x == 0)
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 1 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"    总表");			//总分类型	
									}
									else
									{
										pMenuCount->v3 &= ~PN_TOTAL_TYPE_BIT_MASK;
										pMenuCount->v3 |= 0 << 15;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  用户表");			//总分类型								
									}
									break;
								case 10:
									x = pMenuCount->v3 & PN_USER_TYPE_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"重点用户");			
									}
									else
									{
										pMenuCount->v3 &= ~PN_USER_TYPE_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"普通用户");									
									}
									break;
								case 11:	
									x = pMenuCount->v3 & PN_ON_OFF_BIT_MASK;
									if(x == 0)
									{
										pMenuCount->v3 |= PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"  带拉闸");												
									}
									else
									{
										pMenuCount->v3 &= ~PN_ON_OFF_BIT_MASK;
										DisplayStringOutoffScreenLine(EditRow,8,0,"不带拉闸");
									}
									break;
								default:
									break;
							}
						}
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确认						
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{
								case 1:
								case 6:
								case 7:
								case 12:
								case 13:
								case 14:
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
									break;
								case 2:
								case 3:
								case 4:
								case 5:					
								case 8:
								case 9:
								case 10:
								case 11:						
									break;
								case 15:
									//保存设置的数据
									Stack->MenuStack[MenuCount->Count].Task = 2;
									j = 2;									// 跳出最外层 for 循环
									p8 = (u8*)&BackDispBuf->Row[0].Col[8];
									PnNum = p8_bcd(p8,4);
									if(PnNum > MAX_PN_NUM)
									{//测量点超过最大值 则不可设置
										DisplayString(8,0,0,"错误:测量点号 > 1023");
									}
									else
									{	
										FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
										MR((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
										ConvertBitValueToPnPara(&MeasurePoint, pMenuCount->v3);
										
										for(i = 0; i < METER_ADDR_SIZE; i++)
										{	// 采集器地址显示缓冲，位于备份缓冲区
											MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[6].Col[8 + i*2]);
											MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] <<= 4;
											MeasurePoint.CollectorAddr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[6].Col[8 + i*2 +1]);
										}

										for(i = 0; i < METER_ADDR_SIZE; i++)
										{	// 电能表地址显示缓冲，位于备份缓冲区
											MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] = CharToHex(BackDispBuf->Row[5].Col[8 + i*2]);
											MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] <<= 4;
											MeasurePoint.Addr[METER_ADDR_SIZE-1 - i] |= CharToHex(BackDispBuf->Row[5].Col[8 + i*2 +1]);
										}

										i = CharToHex(DispBuf->Row[5].Col[8]) << 4;
										i += CharToHex(DispBuf->Row[5].Col[9]);
										MeasurePoint.MaxRateNum = i;

										i = CharToHex(DispBuf->Row[6].Col[8]) << 12;
										i += CharToHex(DispBuf->Row[6].Col[9]) << 8;
										i += CharToHex(DispBuf->Row[6].Col[10]) << 4;
										i += CharToHex(DispBuf->Row[6].Col[11]);
										MeasurePoint.CT = i;

										i = CharToHex(DispBuf->Row[7].Col[8]) << 12;
										i += CharToHex(DispBuf->Row[7].Col[9]) << 8;
										i += CharToHex(DispBuf->Row[7].Col[10]) << 4;
										i += CharToHex(DispBuf->Row[7].Col[11]);
										MeasurePoint.PT = i;
										
										Pn=SwitchNumToPn(PnNum);                                     //判断表地址是否重复
										Ret=QCSG_CheckPnAddrIsDuplicate(Pn,MeasurePoint.Addr);
										if(Ret==ERROR_InvalidData)
										{
											DisplayString(8,0,0,"  错误:表地址重复   ");		
										}
										else
										{
											MeasurePoint.PortParity  = 1;                //ybzyl默认设置测量点校验方式为偶校验、数据位为8位，停止位为1位
											MeasurePoint.PortDataBit = 8;                
											MeasurePoint.PortStopBit = 0;
																			
											MW((u32)&MeasurePoint, FlashAddr, sizeof(MeasurePointStruct));
											DisplayString(8,0,0,"         OK         ");

											MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
											pTerminalRam->MeterSyncToRam_S_Timer = METER_FILE_SYNC_DELAY_TIME;
										}
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
					else //if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
					{
						Select(MenuCount->v2,MenuCount->v1);
						UpDownSelect();
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确认
							EditRow = Stack->MenuStack[MenuCount->Count].EditRow;
							switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
							{
								case 1://测量点
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,4);
									if(x > MAX_PN_NUM) bcd_p8(p8,9999,4);
									break;
								case 2://有效位
									break;
								case 3://速率			
									break;
								case 4://端口
									break;
								case 5://协议
									break;
								case 6://通信地址
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,12);
									if(x>999999999999) bcd_p8(p8,0,12);
									break;
								case 7://采集器地址
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,12);
									if(x>999999999999) bcd_p8(p8,0,12);
									break;
								case 8://电能表类型
									break;
								case 9://总分类型
									break;
								case 10://重点户属性
									break;
								case 11://拉闸功能
									break;
								case 12://最大费率数
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+EditRow*84+4+8*2);
									x=p8_bcd(p8,2);
									if(x > 16) bcd_p8(p8,0,2);
									break;
								case 13://CT变比
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
								case 14://PT变比
									p16=(u16 *)(ADDR_STRINGBUFF+EditRow*84+4+16*2);
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
			default:
				break;
		}
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
#endif



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2_4
// Descriptions:        删除测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2_4(void)//删除测量点
{
	u8 *p1,*p2;
	u16 *p16;
	u32 j,Ln;	
	u32 PnNum1,PnNum2;
	u64 x,k;	
	Ln=0;//菜单从第0行开始


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2_3);//显示菜单
			
		  //默认显示测量点号为0
			p1=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p2=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
			bcd_p8(p1,0,4);	
			bcd_p8(p2,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
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
						DisplayString(Ln+3,8,0,"确认");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=9;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
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
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 3:
								//保存设置的数据
								p1=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p1,4);
								PnNum1=(u32)x;
								p2=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
								k=p8_bcd(p2,4);
								PnNum2=(u32)k;
								MeasurePointClr(PnNum1,PnNum2);							
								
								DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+3,9,0,"OK");
							
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
								p1=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p1,4);
								if(x>MAX_PN_NUM)
								{
									bcd_p8(p1,9999,4);
								}
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;		
								p2=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
								k=p8_bcd(p2,4);
								if(k>MAX_PN_NUM)
								{
									bcd_p8(p2,9999,4);
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


//#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMPInformation
// Descriptions:        
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayMPInformation(u32 PnNum)
{
	u8 *p8;
	u64 x = 0;
	u32 FlashAddr = 0, i = 0;		
	StrBuff_TypeDef* DispBuf = StrBuff;
	MeasurePointStruct MeasurePoint;
	
	
	FlashAddr = ADDR_MP_START + PnNum * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
	MR((u32)&MeasurePoint,FlashAddr,sizeof(MeasurePointStruct));
	if(MemCompare(MeasurePoint.Addr, EmptyAddr, METER_ADDR_SIZE) == 0)
	{
		DisplayClrRow(1,8);//清屏幕显示的指定行
		DisplayString(4,4,0,"测量点不存在");
	}
	else
	{
		DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_2_4);//显示菜单
		p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
		bcd_p8(p8,PnNum,4);							// 测量点号
		MR(ADDR_DATABUFF,(u32)&MeasurePoint.State,1);
		if(MeasurePoint.State == 0x01)
		{
			DisplayString(2,8,0,"有效");
		}
		else
		{
			DisplayString(2,8,0,"无效");
		}
		switch(MeasurePoint.PortBaudRate)
		{
			case 4:
				DisplayString(3,8,0,"1200");				
				break;
			case 8:
				DisplayString(3,8,0,"2400");				
				break;
			case 16:
				DisplayString(3,8,0,"4800");				
				break;
			case 32:
				DisplayString(3,8,0,"9600");				
				break;
			default:
				DisplayString(3,8,0,"未知");
				break;	
		}									
		switch(MeasurePoint.PortID)
		{
			case PORT_ID_RS485_1:
				DisplayString(4,8,0,"RS485_1");
				break;									
			case PORT_ID_RS485_2:
				DisplayString(4,8,0,"RS485_2");		
				break;
			case PORT_ID_RS485_3:
				DisplayString(4,8,0,"RS485_3");
				break;
			case PORT_ID_PLC:
				DisplayString(4,8,0,"   载波");
				break;
			case PORT_ID_WIRELESS:
				DisplayString(4,8,0,"   无线");
				break;
			default:
				DisplayString(4,8,0,"   未知");
				break;
		}
		switch(MeasurePoint.ProtocolType)
		{
			case 0:
				DisplayString(5,8,0,"  97协议");
				break;
			case 1:
				DisplayString(5,8,0,"  07协议");
				break;
			case 0x07:
				DisplayString(5,8,0,"EDMI协议");
				break;
			default:
				DisplayString(5,8,0,"    保留");
				break;
		}

		for(i = 0; i < METER_ADDR_SIZE; i++)
		{
			DispBuf->Row[6].Col[8 + i*2 + 1] = 0x2000 + HexToChar[MeasurePoint.Addr[METER_ADDR_SIZE-i-1] & 0x0F];
			DispBuf->Row[6].Col[8 + i*2] = 0x2000 + HexToChar[(MeasurePoint.Addr[METER_ADDR_SIZE-i-1] >> 4) & 0x0F];
		}

		for(i = 0; i < METER_ADDR_SIZE; i++)
		{
			DispBuf->Row[7].Col[8 + i*2 + 1] = 0x2000 + HexToChar[MeasurePoint.CollectorAddr[METER_ADDR_SIZE-i-1] & 0x0F];
			DispBuf->Row[7].Col[8 + i*2] = 0x2000 + HexToChar[(MeasurePoint.CollectorAddr[METER_ADDR_SIZE-i-1] >> 4) & 0x0F];
		}
			
		switch(MeasurePoint.PointMeterType)
		{
			case METER_TYPE_SINGLE:
				DisplayString(8,8,0,"  单相表");
				break;
			case METER_TYPE_MULTI:
				DisplayString(8,8,0,"多功能表");
				break;
			default:
				DisplayString(8,8,0,"  其他表");
				break;
		}
		if(MeasurePoint.TotalType == 0)
		{
			DisplayStringOutoffScreenLine(10,0,0,"总分类型  用户表");		//总分类型	
		}	
		else if(MeasurePoint.TotalType == 1)
		{
			DisplayStringOutoffScreenLine(10,0,0,"总分类型    总表");		//总分类型	
		}
		else
		{
			DisplayStringOutoffScreenLine(10,0,0,"总分类型    未知");		//总分类型
		}
							
		if(MeasurePoint.PriorityType == 0)
		{
			DisplayStringOutoffScreenLine(11,0,0,"重点户  普通用户");		//重点户属性		
		}	
		else if(MeasurePoint.PriorityType == 1)
		{
			DisplayStringOutoffScreenLine(11,0,0,"重点户  重点用户");		//重点户属性
		}
		else
		{
			DisplayStringOutoffScreenLine(11,0,0,"重点户      未知");		//重点户属性
		}								
		if(MeasurePoint.SwitchOff == 0)
		{
			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能不带拉闸");		//拉闸功能		
		}	
		else if(MeasurePoint.SwitchOff == 1)
		{
			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能  带拉闸");		//拉闸功能
		}
		else
		{
			DisplayStringOutoffScreenLine(12,0,0,"拉闸功能    未知");		//拉闸功能
		}

		MC(0, (u32)&DispBuf->Row[13], sizeof(StrLnBuff_TypeDef));
		DisplayStringOutoffScreenLine(13,0,0,"费率数  04");
		DisplayStringOutoffScreenLine(14,0,0,"CT变比  0000");
		DisplayStringOutoffScreenLine(15,0,0,"PT变比  0000");		
		
		x = bcd_hex(MeasurePoint.MaxRateNum);
		p8=(u8*)(ADDR_STRINGBUFF+13*84+4+8*2);
		bcd_p8(p8, x, 2);

		x = bcd_hex(MeasurePoint.CT);
		p8=(u8*)(ADDR_STRINGBUFF+14*84+4+8*2);
		bcd_p8(p8, x, 4);		

		x = bcd_hex(MeasurePoint.PT);
		p8=(u8*)(ADDR_STRINGBUFF+15*84+4+8*2);
		bcd_p8(p8, x, 4);
	}				
}
//#endif




//-------------------------------------------------------------------------------------------------
// Function name:       MeasurePointClr
// Descriptions:        
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MeasurePointClr(u32 PnNum1,u32 PnNum2)
{
	u32 i=0;
	u32 FlashAddr = 0;
	MeasurePointStruct MeasurePoint;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;	
	
	if(PnNum1 > MAX_PN_NUM) 
	{
		PnNum1 = MAX_PN_NUM;
	}
	if(PnNum2 > MAX_PN_NUM) 
	{
		PnNum2 = MAX_PN_NUM;
	}		
	if(PnNum1 > PnNum2)
	{
		return;
	}
	
	i=PnNum1;
	for(i=PnNum1;i<=PnNum2;i++)
	{
		FlashAddr = ADDR_MP_START + i * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
		MR((u32)&MeasurePoint,FlashAddr,sizeof(MeasurePointStruct));
		MC(0xFF,FlashAddr,sizeof(MeasurePointStruct));                   //测量点的信息全部清为0xFF
		//MR((u32)&MeasurePoint,FlashAddr,sizeof(MeasurePointStruct));   //帮助调试时使用
		//同步测量点
		MeterFileInfo->CountFlag = METER_FILE_SYNC_TO_RAM_FLAG;
		pTerminalRam->MeterSyncToRam_S_Timer = METER_FILE_SYNC_DELAY_TIME;	
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回 
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_3
// Descriptions:        限值参数查询
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_3(void)
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if ((Project/100)==3) || ((Project/100)==9)//负控终端
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_3);//显示菜单
			#endif
			#if ((Project/100)==2)//集中器
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_3);//显示菜单
			#endif
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
						MenuCreate((u32)Menu_7_3_1);//月通信流量越限值
						break;
					case 2:
						MenuCreate((u32)Menu_7_3_2);//电能表时钟超差阈值
						break;
					case 3:
						MenuCreate((u32)Menu_7_3_3);//电能表飞走阈值
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
// Function name:       Menu_7_3_1
// Descriptions:        月通信流量越限值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_3_1(void)//月通信流量越限值
{
	u32 *p32,*q32;
	u32 Ln=0;          //菜单从第0行开始
	ThresholdParaStruct ThresholdPara;

	
	DisplayClr();//清全屏
	MenuCount->v1=0;
	MenuCount->v2=0;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_3_1);//显示菜单
	
	//显示内存中数据	
	//月通信流量越限值		
	MR((u32)&ThresholdPara,ADDR_ThresholdPara,sizeof(ThresholdParaStruct));	
	p32=(u32*)ADDR_DATABUFF;				
	MR(ADDR_DATABUFF,(u32)&ThresholdPara.E000018C,3);			
	q32=(u32*)(ADDR_STRINGBUFF+(Ln+1)*84+4+11*2);
	DisplayData(p32[0]&0x00FFFFFF,6,0,(u16*)q32);
	MenuCount->v1=11;
	MenuCount->v2=19;
	NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_3_2
// Descriptions:        电能表时钟超差阈值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_3_2(void)//电能表时钟超差阈值
{	
	u8 *p,*p8;
	u32 Ln=0;         //菜单从第0行开始
	ThresholdParaStruct ThresholdPara;

	
	DisplayClr();//清全屏
	MenuCount->v1=0;
	MenuCount->v2=0;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_3_2);//显示菜单
	
	//显示内存中数据	
	//电能表时钟超差阈值			
	MR((u32)&ThresholdPara,ADDR_ThresholdPara,sizeof(ThresholdParaStruct));
	p8=(u8*)ADDR_DATABUFF;
	MR((u32)p8,(u32)&ThresholdPara.E0000188,1);
	p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+14*2);
	DisplayData(p8[0],2,0,(u16*)p);
	MenuCount->v1=14;
	MenuCount->v2=19;
	NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

#if ((Project/100)==3) || ((Project/100)==9) //负控终端
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_3_3
// Descriptions:        电能表飞走阈值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_3_3(void)//电能表飞走阈值
{	
	u8 *p,*p8;
	u32 Ln=0;//菜单从第0行开始
	ThresholdParaStruct ThresholdPara;
	
	
	DisplayClr();//清全屏
	MenuCount->v1=0;
	MenuCount->v2=0;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_3_3);//显示菜单
	
	//显示内存中数据	
	//电能表飞走阈值
	MR((u32)&ThresholdPara,ADDR_ThresholdPara,sizeof(ThresholdParaStruct));
	p8=(u8*)ADDR_DATABUFF;
	MR((u32)p8,(u32)&ThresholdPara.E0000189,1);
	p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+13*2);
	DisplayData(p8[0],2,0,(u16*)p);
	MenuCount->v1=13;
	MenuCount->v2=19;
	NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);	

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
#endif


#if ((Project/100)==2)//集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_3_3
// Descriptions:        电能表停走阈值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_3_3(void)//电能表停走阈值
{
	u16 *p16,*q16;
	u32 Ln=0;          //菜单从第0行开始
	ThresholdParaStruct ThresholdPara;
	
	
	DisplayClr();//清全屏
	MenuCount->v1=0;
	MenuCount->v2=0;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pJZQMenu_7_3_3);//显示菜单
	
	//显示内存中数据	
	//电能表停走阈值
	MR((u32)&ThresholdPara,ADDR_ThresholdPara,sizeof(ThresholdParaStruct));	
	p16=(u16*)ADDR_DATABUFF;				
	MR(ADDR_DATABUFF,(u32)&ThresholdPara.E000018A,2);			
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+14*2);
	DisplayData(p16[0],4,2,(u16*)q16);
	
	MenuCount->v1=14;
	MenuCount->v2=19;
	NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
#endif



#if Project == 200 //南网集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_4
// Descriptions:        级联参数查询与设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4(void)//级联参数查询与设置
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4);//显示菜单
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
						MenuCreate((u32)Menu_7_4_1);//级联从终端地址
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_2);//主终端地址
						break;
					case 3:
						MenuCreate((u32)Menu_7_4_3);//被抄终端地址
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
// Function name:       Menu_7_4_1
// Descriptions:        级联从终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1(void)//级联从终端地址
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_1_1);//级联从终端地址1
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_1_2);//级联从终端地址2
						break;
					case 3:
						MenuCreate((u32)Menu_7_4_1_3);//级联从终端地址3
						break;
					case 4:
						MenuCreate((u32)Menu_7_4_1_4);//级联从终端地址4
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
// Function name:       Menu_7_4_1_1
// Descriptions:        级联从终端地址1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_1(void)//级联从终端地址1
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_1_1_1);//终端地市区县码
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_1_1_2);//终端地址
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
// Function name:       Menu_7_4_1_1_1
// Descriptions:        级联从终端地址1终端地市区县码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_1_1(void)//级联从终端地址1终端地市区县码
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_1);//显示菜单
			
			//显示内存中数据
			//级联从终端地址1终端地市区县码
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr1,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
			MenuCount->v1=7;
			MenuCount->v2=12;
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
							MenuCount->v1=7;
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
								break;
							case 2:
								//保存设置的数据
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr1,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);							
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_1_2
// Descriptions:        级联从终端地址1终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_1_2(void)//级联从终端地址1终端地址
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_2);//显示菜单
			
			//显示内存中数据
			//级联从终端地址1终端地址
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr1+3,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
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
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr1+3,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_2
// Descriptions:        级联从终端地址2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_2(void)//级联从终端地址2
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_1_2_1);//终端地市区县码
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_1_2_2);//终端地址
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
// Function name:       Menu_7_4_1_2_1
// Descriptions:        级联从终端地址2终端地市区县码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_2_1(void)//级联从终端地址2终端地市区县码
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_1);//显示菜单
			
			//显示内存中数据
			//级联从终端地址2终端地市区县码
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr2,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
			MenuCount->v1=7;
			MenuCount->v2=12;
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
							MenuCount->v1=7;
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
								break;
							case 2:
								//保存设置的数据
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr2,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);							
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_2_2
// Descriptions:        级联从终端地址2终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_2_2(void)//级联从终端地址2终端地址
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_2);//显示菜单
			
			//显示内存中数据
			//级联从终端地址2终端地址
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr2+3,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
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
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr2+3,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_3
// Descriptions:        级联从终端地址3
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_3(void)//级联从终端地址3
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_1_3_1);//终端地市区县码
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_1_3_2);//终端地址
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
// Function name:       Menu_7_4_1_3_1
// Descriptions:        级联从终端地址3终端地市区县码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_3_1(void)//级联从终端地址3终端地市区县码
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_1);//显示菜单
			
			//显示内存中数据
			//级联从终端地址3终端地市区县码
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr3,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
			MenuCount->v1=7;
			MenuCount->v2=12;
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
							MenuCount->v1=7;
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
								break;
							case 2:
								//保存设置的数据
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr3,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);							
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_3_2
// Descriptions:        级联从终端地址3终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_3_2(void)//级联从终端地址3终端地址
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_2);//显示菜单
			
			//显示内存中数据
			//级联从终端地址3终端地址
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr3+3,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
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
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr3+3,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_4
// Descriptions:        级联从终端地址4
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_4(void)//级联从终端地址4
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_1_4_1);//终端地市区县码
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_1_4_2);//终端地址
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
// Function name:       Menu_7_4_1_4_1
// Descriptions:        级联从终端地址4终端地市区县码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_4_1(void)//级联从终端地址4终端地市区县码
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_1);//显示菜单
			
			//显示内存中数据
			//级联从终端地址4终端地市区县码
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr4,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
			MenuCount->v1=7;
			MenuCount->v2=12;
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
							MenuCount->v1=7;
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
								break;
							case 2:
								//保存设置的数据
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr4,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);							
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_1_4_2
// Descriptions:        级联从终端地址4终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_1_4_2(void)//级联从终端地址3终端地址
{
	u8 *p8,*p;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_2);//显示菜单
			
			//显示内存中数据
			//级联从终端地址4终端地址
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.SlaveAddr4+3,3);
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p32=(u32*)p8;			
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);
			
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
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.SlaveAddr4+3,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
								if((x>0xFFFFFF) || (x<1))
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
// Function name:       Menu_7_4_2
// Descriptions:        主终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_2(void)//主终端地址
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_2_1);//终端地市区县码
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_2_2);//终端地址
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
// Function name:       Menu_7_4_2_1
// Descriptions:        主终端地址终端地市区县码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_2_1(void)//主终端地址终端地市区县码
{
	u8 *p,*p8;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;
	

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_1);//显示菜单
			
			//显示内存中数据
			//主终端地址终端地市区县码
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.MasterAddr,3);			
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
			p32=(u32*)p8;
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);			
		
			MenuCount->v1=7;
			MenuCount->v2=12;
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
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=7;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据	
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));							
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.MasterAddr,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));							
								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF) || (x<1))
								{
									bcd_p8(p8,0,6);
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
// Function name:       Menu_7_4_2_2
// Descriptions:        主终端地址终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_2_2(void)//主终端地址终端地址
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;
	

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_2);//显示菜单
			
			//显示内存中数据
			//主终端地址终端地址
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			MR(ADDR_DATABUFF,(u32)&AttaPara.MasterAddr+3,1);
			p8=(u8*)ADDR_DATABUFF;
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);		
			DisplayData(p8[0],2,0,p16);
			
			MenuCount->v1=9;
			MenuCount->v2=10;
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
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=10;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据	
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));							
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<1;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.MasterAddr+3,(u32)p8,1);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
															
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,2);
								if((x>0xFF) || (x<1))
								{
									bcd_p8(p8,0,2);
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
// Function name:       Menu_7_4_3
// Descriptions:        被抄终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_3(void)//被抄终端地址
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1);//显示菜单
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
						MenuCreate((u32)Menu_7_4_3_1);//终端地市区县码
						break;
					case 2:
						MenuCreate((u32)Menu_7_4_3_2);//终端地址
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
// Function name:       Menu_7_4_3_1
// Descriptions:        被抄终端地址终端地市区县码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_3_1(void)//被抄终端地址终端地市区县码
{
	u8 *p,*p8;
	u16 *p16;
	u32 *p32;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_1);//显示菜单
			
			//显示内存中数据
			//被抄终端地址终端地市区县码
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&AttaPara.AMRSlaveAddr,3);			
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
			p32=(u32*)p8;
			DisplayData(p32[0]&0x00FFFFFF,6,0,p16);		
		
			MenuCount->v1=7;
			MenuCount->v2=12;
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
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=7;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据	
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);						
								for(i=0;i<=2;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[2-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.AMRSlaveAddr,(u32)p8,3);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));							
								
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://主站端口
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+7*2);
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF) || (x<1))
								{
									bcd_p8(p8,0,6);
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
// Function name:       Menu_7_4_3_2
// Descriptions:        被抄终端地址终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_4_3_2(void)//被抄终端地址终端地址
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,x;
	u32 Ln=0;//菜单从第0行开始
	AttachmentParaStruct AttaPara;


	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_4_1_1_2);//显示菜单
			
			//显示内存中数据
			//被抄终端地址终端地址
			MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
			MR(ADDR_DATABUFF,(u32)&AttaPara.AMRSlaveAddr+3,1);
			p8=(u8*)ADDR_DATABUFF;
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);		
			DisplayData(p8[0],2,0,p16);		
		
			MenuCount->v1=9;
			MenuCount->v2=10;
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
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=10;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								MR((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));							
								p8=(u8*)ADDR_DATABUFF;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<1;i++)     //十进制数转化为十六进制数
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MR((u32)&AttaPara.AMRSlaveAddr+3,(u32)p8,1);
								MW((u32)&AttaPara,ADDR_AttachmentPara,sizeof(AttachmentParaStruct));
															
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://主站端口
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,2);
								if((x>0xFF) || (x<1))
								{
									bcd_p8(p8,0,2);
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
#endif


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_5
// Descriptions:        SIM卡IP地址查询
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_5(void)
{
	u16 *p16;
	u32 State;	
	u8 IPAddr[16];	
	DisplayClr();//清全屏
	DisplayString(2,0,0,"SIM卡IP地址:");
	DisplayString(4,0,0,"                  ");//183.247.151.178
	
	State=Get_GPRSState(GPRSPORT);
	
	if((State==GPRS_STATE_LINK_SUCCESS)||
	  (State==GPRS_STATE_LOG_SUCCESS))          //9
	{
		Get_GPRSLocalIPAddr(IPAddr);
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
		DisplayString(4,3,0,"                  ");		
	}	
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回 
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7
// Descriptions:        参数设置与查看
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7(void)//参数设置与查看
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if (Project == 300) || (Project == 900)//南网负控
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7);//显示菜单
			#endif
			#if Project == 200 //南网集中器
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_7);//显示菜单
			#endif
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
						MenuCreate((u32)Menu_7_1);//通讯通道设置  
						break;
					case 2:
						MenuCreate((u32)Menu_7_2);//测量点参数设置
						break;
					case 3:
						MenuCreate((u32)Menu_7_3);//限值参数查询
					break;
					#if ((Project/100) == 2) //南网集中器
						case 4:
							MenuCreate((u32)Menu_7_4);//级联参数查询与设置			
							break;
						case 5:
							MenuCreate((u32)Menu_7_5);//SIM卡IP地址查询
							break;
					#endif
					#if ((Project/100) == 3)||((Project/100) == 9) //南网负控终端&配变
						case 4:
							MenuCreate((u32)Menu_7_5);//级联参数查询与设置			
							break;
					#endif
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
