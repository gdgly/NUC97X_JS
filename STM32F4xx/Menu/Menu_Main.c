
//主菜单
#include "Project.h"
#include "Menu.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


void _0(void)
{
}

#if ((Project/100)==2 && (USER/100)>1)//非国网送样集中器
//Comm_Ram->RouterType//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓程,5=瑞斯康,6=力合微,7=讯能,8=钱龙,9=华瑞
const u8 RouterTypeList[][10]=//载波路由类型列表
{
	"保留",//0
	"正泰",//1
	"鼎信",//2
	"东软",//3
	"晓程",//4
	"瑞斯康",//5
	"力合微",//6
	"讯能",//7
	"钱龙",//8
	"华瑞",//9
};
//Comm_Ram->RouterType//路由器类型:100=保留,101=麦希,102=友讯达,103=有方,104=
const u8 RouterTypeWL_List[][10]=//本地无线路由类型列表
{
	"保留",//0
	"麦希",//1
	"友讯达",//2
	"有方",//3
};
#endif
#if ((Project/100)==4)//公变终端
const u8 pE2_Main4[]=//公变主菜单
{
	"主菜单\n\r"
	"  1.实时数据\n\r"
	"  2.参数定值\n\r"
	"  3.电能表示数\n\r"
	"  4.中文信息\n\r"
	"  5.终端信息\n\r"
	"  6.终端参数设置\n\r"
	"  7.终端管理与维护\n\r"
};
#endif

#if ((Project/100)==3)//专变终端
const u8 pE2_Main3[]=//专变主菜单
{
	"主菜单\n\r"
	"  1.实时数据\n\r"
	"  2.参数定值\n\r"
	"  3.控制状态\n\r"
	"  4.电能表示数\n\r"
	"  5.中文信息\n\r"
	"  6.购电信息\n\r"
	"  7.终端信息\n\r"
//	"  8.终端参数设置\n\r"
//	"  9.终端管理与维护\n\r"
	"  8.终端管理与维护\n\r"
};
#endif

#if ((Project/100)==2)//集中器
const u8 pE2_Main2[]=//集中器主菜单
{
	"主菜单\n\r"
	"  1.测量点数据显示\n\r"
	"  2.参数设置与查看\n\r"
	"  3.终端管理与维护\n\r"
//#if ((USER/100)==6)//湖南系
//	"  4.载波测试\n\r"
//#endif
#if ((USER/100)>1)//非国网送样
	"  4.本地模块测试\n\r"
	"  5.本地模块信息\n\r"
#endif
};
#endif

	#if ((Project/100)==3 && (USER/100)==5)//上海专变
//-------------------------------------------------------------------------------------------------
// Function name:       PassWord
// Descriptions:        输入密码
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------

void PassWord(void)
{
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u32 LnTitle=3;
	u32 ColTitle=6;
	u32 Ln=5;
	u32 Col=6;
	u32 Length=6;
	u32 i;
	u32 x;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(LnTitle,ColTitle,0,"输入密码");
			DisplayString(Ln,Col,0,"0*****>");
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
			MenuCount->v1=0;
			break;
		case 1://任务1:输入密码
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右移键按下
				p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					i=Stack->MenuStack[MenuCount->Count].EditCol-Col;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					Stack->MenuStack[MenuCount->Count].EditCol++;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
				x&=0x00ff;
				if((x<0x30 && x!=0x2a) || (x>0x39 && x!=0x3e))//只有数字、>、*能选中，其他符号全部跳过
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
					{
						Stack->MenuStack[MenuCount->Count].EditCol++;
					}
				}	
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
					p16[0]=0x2030;
				}
			}
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左移键按下
				p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					i=Stack->MenuStack[MenuCount->Count].EditCol-7;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
				x&=0x00ff;
				if((x<0x30 && x!=0x2a) || (x>0x39 && x!=0x3e))//只有数字、>、*能选中，其他符号全部跳过
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
					{
						Stack->MenuStack[MenuCount->Count].EditCol--;
					}
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
					p16[0]=0x2030;
				}
			}
			NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
			UpDownSelect_SH(Stack->MenuStack[MenuCount->Count].EditRow);
			if(((Comm_Ram->ButtonVal&0x10)!=0)&&(Stack->MenuStack[MenuCount->Count].EditCol==Col+Length))
			{//确认
				Stack->MenuStack[MenuCount->Count].Task=2;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 2://任务2：判断密码是否正确，正确则跳转到菜单8，错误提示对话框
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_MenuKey,3);
			if((p32[0]&0xffffff)==(MenuCount->v1&0xffffff))
			{//比较输入的密码与正确的密码是否相等
//菜单1下的第8子菜单为终端管理与维护
//菜单1下的第2子菜单下的第6子菜单为终端参数设置与查询
				if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)
				{
					if((MenuCount->Count-1)==1)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_9;//终端管理与维护
					}
				}
				else
				{
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==6)
					{
						if((MenuCount->Count-1)==2)
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;//终端参数设置与查询
						}
					}
					else
					{
						MenuDel();//返回上级菜单	    
					}
				} 
				Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
				Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
				Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
				Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
			}
			else
			{
				DisplayClr();
				DisplayString(4,6,0,"密码错误");
				DisplayString(5,3,0,"请按返回键返回");
				Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单	    
				}
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       PassWordCreateEdit
// Descriptions:        创建密码编辑框
// input parameters:    下一屏函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------
void PassWordCreateEdit(void)//创建密码编辑框
{
	MenuCount->Count++;
	Stack->MenuStack[MenuCount->Count].PC=(u32)PassWord;//菜单任务运行地址(特别注意:此值不可随便改动)
	Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditRow=5;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditCol=6;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
}
	#else
//-------------------------------------------------------------------------------------------------
// Function name:       PassWord
// Descriptions:        输入密码
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------

void PassWord(void)
{
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u32 LnTitle=3;
	u32 ColTitle=6;
	u32 Ln=5;
	u32 Col=6;
	u32 Length=6;
	u32 i;
	u32 x;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(LnTitle,ColTitle,0,"输入密码");
			DisplayString(Ln,Col,0,"0*****>");
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
			MenuCount->v1=0;
			break;
		case 1://任务1:输入密码
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右移键按下
				p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					i=Stack->MenuStack[MenuCount->Count].EditCol-Col;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					Stack->MenuStack[MenuCount->Count].EditCol++;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
				x&=0x00ff;
				if((x<0x30 && x!=0x2a) || (x>0x39 && x!=0x3e))//只有数字、>、*能选中，其他符号全部跳过
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
					{
						Stack->MenuStack[MenuCount->Count].EditCol++;
					}
				}	
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
					p16[0]=0x2030;
				}
			}
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左移键按下
				p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					i=Stack->MenuStack[MenuCount->Count].EditCol-7;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
				x&=0x00ff;
				if((x<0x30 && x!=0x2a) || (x>0x39 && x!=0x3e))//只有数字、>、*能选中，其他符号全部跳过
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
					{
						Stack->MenuStack[MenuCount->Count].EditCol--;
					}
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
					p16[0]=0x2030;
				}
			}
			NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
			UpDownSelect();
			if(((Comm_Ram->ButtonVal&0x10)!=0)&&(Stack->MenuStack[MenuCount->Count].EditCol==Col+Length))
			{//确认
				Stack->MenuStack[MenuCount->Count].Task=2;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 2://任务2：判断密码是否正确，正确则跳转到菜单8，错误提示对话框
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_MenuKey,3);
			if((p32[0]&0xffffff)==(MenuCount->v1&0xffffff))
			{//比较输入的密码与正确的密码是否相等
				#if ((Project/100)==3)//专变终端
//菜单1下的第8子菜单为终端管理与维护
//菜单1下的第2子菜单下的第8子菜单为终端参数设置与查询
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)
					{
						switch(MenuCount->Count-1)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_9;//终端管理与维护
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;//终端参数设置与查询
								break;
							default:
								MenuDel();//返回上级菜单	    
								break;
						}
					}
					else
					{
						MenuDel();//返回上级菜单	    
					}
				#endif
				#if ((Project/100)==4)//公变终端
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==6)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==7)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_9;
					}
					else
					{
						MenuDel();//返回上级菜单	    
					}
				#endif
				#if ((Project/100)==2)//集中器
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==2)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
					}
					else
					{
						MenuDel();//返回上级菜单	    
					}
				#endif
				Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
				Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
				Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
				Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
			}
			else
			{
				DisplayClr();
				DisplayString(4,6,0,"密码错误");
				DisplayString(5,3,0,"请按返回键返回");
				Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单	    
				}
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			}
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       PassWordCreateEdit
// Descriptions:        创建密码编辑框
// input parameters:    下一屏函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------
void PassWordCreateEdit(void)//创建密码编辑框
{
	MenuCount->Count++;
	Stack->MenuStack[MenuCount->Count].PC=(u32)PassWord;//菜单任务运行地址(特别注意:此值不可随便改动)
	Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditRow=5;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditCol=6;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
}
	#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_0
// Descriptions:        提示未按下编程键
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

//void Menu_0(void)
//{
//	DisplayClr();
//	DisplayString(4,4,0,"编程键未按下");
//	DisplayString(5,1,0,"请按编程键／返回键");
//	Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
//	if((Comm_Ram->ButtonVal&0x20)!=0 || Comm_Ram->Factory==0x55 || MRR(ADDR_MeterFMTimer,2))
//	{//返回键、工厂模式、编程键
//		MenuDel();//返回上级菜单
//	}
//	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
//}

//	#if ((Project/100)==2 && (USER/100)==6)//湖南系集中器
	#if ((Project/100)==2 && (USER/100)>1)//非国网送样集中器
//------------------------------------------------------------------------------------------------
// Function name:       Menu2_HN_4
// Descriptions:        湖南集中器菜单4 载波测试
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_HN_4(void)
{
	u64 i;
	u32 j;
//	u32 cs;
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);//得到UART控制区地址
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(RS232PORT);//得到UART接收缓冲地址
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(RS232PORT);//得到UART发送缓冲地址
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"本地模块测试\n\r"
																			"目标地址\n\r"
																			"      确认发送\n\r"
																			);//显示菜单
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+8*2);
//			DisplayData(0x434030100004,12,0,p16);
			DisplayData(0x000000000001,12,0,p16);
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(2,6,0,"确认发送");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=19;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						//载波测试数据转发F9 AFN0CF129电能示值
						//步骤：
						//1.当任务号为0时，组帧写到接收缓冲区
						//2.任务号置1
						//3.当任务号为0时，从发送缓冲区读取数据
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
						}
						if(Stack->MenuStack[MenuCount->Count].EditRow==2)
						{
							if(UARTCtrl->Task==0)
							{
/*
终端接收AFN10F9的帧格式
68 
A6 00 //L
A6 00 //L
68 
7B //C
34 12 01 00 //终端地址、行政区域码
02 //
10 //AFN
61 //SEQ
00 00 01 01 //数据单元标识
1F //终端通信端口号
00 //转发中继级数
12 90 78 56 34 12 //6字节 转发目标地址
01 //转发直接抄读的数据标识类型，0：符合DLT 645-1997,1：符合DL/T 645-2007
00 FF 01 00 //4字节 转发直接抄读的数据标识
90 78 56 34 12 90 78 56 34 12 90 78 56 34 12 90 //16字节 PW
89 //1字节 校验和CS
16 
*/
								p8=(u8 *)ADDR_DATABUFF;
								p8[0]=0x68;
								p8[1]=0xA6;//L
								p8[2]=0x00;
								p8[3]=0xA6;//L
								p8[4]=0x00;
								p8[5]=0x68;
								p8[6]=0x7B;//C
								MR(((u32)ADDR_DATABUFF)+7,ADDR_AREACODE,4);//行政码,终端地址
								p8[11]=0x02;
								p8[12]=0x10;//AFN									 
								p8[13]=0x61;//SEQ									 
								p8[14]=0x00;//数据单元标识									 
								p8[15]=0x00;									 
								p8[16]=0x01;									 
								p8[17]=0x01;
								p8[18]=0x1F;//终端通信端口号
								p8[19]=0x00;//转发中继计数
								//转发目标地址
								p8d=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								for(i=0;i<6;i++)
								{
									p8[25-i]=(p8d[4*i]-0x30)*16+(p8d[4*i+2]-0x30);
								}

								p8[26]=0x01;//转发直接抄读的数据标识类型，0：符合DLT 645-1997,1：符合DL/T 645-2007									 
								p8[27]=0x00;//转发直接抄读的数据标识
								p8[28]=0x00;									 
								p8[29]=0x01;
								p8[30]=0x00;
								for(i=0;i<16;i++)
								{
									p8[31+i]=0x00;//16字节PW
								}
//								//校验和CS计算
//								cs=0;
//								for(i=0;i<41;i++)
//								{
//									cs+=p8[6+i];
//								}
//								cs&=0xff;
//								p8[47]=cs;
//								p8[48]=0x16;

								p8[1]=0x29;//去标志位D1D0,同时长度最大只有0-16383
//								MW(ADDR_DATABUFF,(u32)p8rxbuff,49);									 
								MW(ADDR_DATABUFF,(u32)p8rxbuff,47);									 

								UARTCtrl->Task=1;
								Stack->MenuStack[MenuCount->Count].Task=2;
							}
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
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
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
		case 2:
//从发送缓冲区读取数据
			if(UARTCtrl->Task==0)
			{
				DisplayClr();//清全屏
				DisplayMenuString(4,0,2,(u8 *)
												"正向有功电能(kWh):\n\r"
												"  总 =\n\r"
												);//显示菜单字符串
/*
终端发送AFN10F9的帧格式
68H
L
L
68H
C
A
AFN=10H
SEQ
数据单元标识
数据单元
						转发通信端口号 1BIN
						转发目标地址 6
						转发结果标志 1BIN
						转发直接抄读的数据内容字节数k+4 1BIN
						转发直接抄读的数据标识 4BIN
						转发直接抄读的数据内容 K
EC（见本部分4.3.4.6.3）
Tp（见本部分4.3.4.6.4）
CS
16H
*/
				p8=(u8 *)ADDR_DATABUFF;
				MR(ADDR_DATABUFF,(u32)p8txbuff,40);
				if(p8[25]==5)
				{//转发接收数据
					i=MRR((u32)p8txbuff+31,4);	                   
					p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+6*2);
					DisplayData_OffZero(i,8,2,p16);
				}
				else
				{
					DisplayString(5,8,0,"------");
				}
			}
			else
			{
				DisplayClr();//清全屏
				DisplayMenuString(4,0,2,(u8 *)
												"     载波测试中\n\r"
												"   请耐心等待结果\n\r"
												);//显示菜单字符串
			}
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{//按任意键返回上级菜单
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}

void Menu2_SX_5(void)//陕西集中器 载波信息
{
	u32 Row;
	u32 i;
	u32 x;
	u8 *p8;
	u8 *p;
	u16 *p16;

	Row=1;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"本地模块信息\n\r"
																	"模块厂家:\n\r"
																	"厂商代码:\n\r"
																	"软件日期:\n\r"
																	"软件版本:\n\r"
																	);//显示菜单字符串
	i=Comm_Ram->RouterType;
	if(i<101)
	{//载波模块
//Comm_Ram->RouterType//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓程,5=瑞斯康,6=力合微,7=讯能,8=钱龙,9=华瑞
		//载波厂家
		if(i>0 && i<=9)
		{
			DisplayString(Row,9,0,(u8 *)RouterTypeList[i]);
		}
		else
		{
			DisplayString(Row,9,0,(u8 *)"未知\n\r");
		}
	}
	else
	{//微功率无线模块
//Comm_Ram->RouterType//路由器类型:100=保留,101=麦希,102=友讯达,103=有方,104=
		//本地无线厂家
		i-=100;
		if(i>0 && i<=3)
		{
			DisplayString(Row,9,0,(u8 *)RouterTypeWL_List[i]);
		}
		else
		{
			DisplayString(Row,9,0,(u8 *)"未知\n\r");
		}
	}


	//厂商代码
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p8=(u8 *)&Terminal_Router->RouterAddr+6;
	p[0]=p8[0];
	p[1]=0x20;
	p[2]=p8[1];
	p[3]=0x20;

	//软件日期
	Row++;
	p8=(u8 *)&Terminal_Router->RouterAddr+10;
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
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
	//软件版本
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=MRR((u32)&Terminal_Router->RouterAddr+13,2);
	p16=DisplayData(x,4,0,p16);

	if((Comm_Ram->ButtonVal&0x20)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
	#endif
//-------------------------------------------------------------------------------------------------
// Function name:       E2_Main
// Descriptions:        主菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_Main(void)//主菜单
{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	u8 * p8;
	p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
	p8[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif

#if (!((Project/100)==3 && (USER/100)==5))//非上海专变
	u16 *p16;
	p16=(u16 *)(ADDR_STRINGBUFF+9*84+4+18*2);
#endif
	DOTMIXPMODE->Mix=2;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
	Draw_Clr();
	#if ((Project/100)==3)//专变终端
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main3);//显示菜单
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
							MenuCreate((u32)Menu_1);//创建下级菜单
							break;
						case 2:
							MenuCreate((u32)Menu_2);//创建下级菜单
							break;
						case 3:
							MenuCreate((u32)Menu_3);//创建下级菜单
							break;
						case 4:
							MenuCreate((u32)Menu_4);//创建下级菜单
							break;
						case 5:
							MenuCreate((u32)Menu_5);//创建下级菜单
							break;
						case 6:
							MenuCreate((u32)Menu_6);
							break;
						case 7:
							MenuCreate((u32)Menu_7);//创建下级菜单
							break;
						case 8:
							//按下编程键后、输入正确密码，方可进入设置菜单
							//工厂模式下，随时可进入设置菜单
							if(Comm_Ram->Factory==0x55)
							{//工厂状态
								MenuCreate((u32)Menu_9);//创建下级菜单
							}
							else
							{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//按下编程键
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//创建下级菜单
//								}
							}
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x33;//键使用后清0
				break;
			default:
				break;
		}
	#endif
	#if ((Project/100)==4)//公变终端
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main4);//显示菜单
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
							MenuCreate((u32)Menu_1);//创建下级菜单
							break;
						case 2:
							MenuCreate((u32)Menu_2);//创建下级菜单
							break;
						case 3:
							MenuCreate((u32)Menu_4);//创建下级菜单
							break;
						case 4:
							MenuCreate((u32)Menu_5);//创建下级菜单
							break;
						case 5:
							MenuCreate((u32)Menu_7);
							break;
						case 6:
						case 7:
							//按下编程键后、输入正确密码，方可进入8、9子菜单
							//工厂模式下，随时可进入8、9子菜单
							if(Comm_Ram->Factory==0x55)
							{//工厂状态
								if(Stack->MenuStack[MenuCount->Count].EditRow==6)
								{
									MenuCreate((u32)Menu_8);//创建下级菜单
								}
								if(Stack->MenuStack[MenuCount->Count].EditRow==7)
								{
									MenuCreate((u32)Menu_9);//创建下级菜单
								}
							}
							else
							{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//按下编程键
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//创建下级菜单
//								}
							}
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x33;//键使用后清0
				break;
			default:
				break;
		}
	#endif
	#if ((Project/100)==2)//集中器
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main2);//显示菜单
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
							MenuCreate((u32)Menu_4);//创建下级菜单 电能表示数/集中器测量点数据显示
							break;
						case 2:
						case 3:
							//按下编程键后、输入正确密码，方可进入3、4子菜单
							//工厂模式下，随时可进入3、4子菜单
							if(Comm_Ram->Factory==0x55)
							{//工厂状态
								if(Stack->MenuStack[MenuCount->Count].EditRow==2)
								{
									MenuCreate((u32)Menu2_2);//创建下级菜单 集中器参数设置与查看
								}
								if(Stack->MenuStack[MenuCount->Count].EditRow==3)
								{
									MenuCreate((u32)Menu2_3);//创建下级菜单	集中器终端管理与维护
								}
							}
							else
							{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//按下编程键
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//创建下级菜单
//								}
							}
							break;
//#if ((USER/100)==6)//湖南系
#if ((USER/100)>1)//非国网送样
						case 4:
//							MenuCreate((u32)Menu2_HN_4);//创建下级菜单 湖南集中器菜单4 载波测试
							if(MenuCount->Count>=MaxMenuCount)
							{
								return;
							}
							MenuCount->Count++; 
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_HN_4;//菜单任务运行地址(特别注意:此值不可随便改动)	湖南集中器菜单4 载波测试
							Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
							Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
							Stack->MenuStack[MenuCount->Count].EditRow=2;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
							Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
							break;
						case 5://载波信息 陕西要求
							MenuCreate((u32)Menu2_SX_5);//创建下级菜单	陕西集中器 载波信息
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
				Comm_Ram->ButtonVal&=~0x33;//键使用后清0
				break;
			default:
				break;
		}
	#endif
//	#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
//		if(MRR(ADDR_MeterFMTimer,2))
//		{
//			p16[0]=23;// '编程按键'
//			p16[1]=24;
//		}
//		else
//		{
//			p16[0]=0x2000;
//			p16[1]=0x2000;
//		}
//	#else
//		if(MRR(ADDR_TerminalFMTimer+12,2))
//		{
//			p16[0]=23;// '编程按键'
//			p16[1]=24;
//		}
//		else
//		{
//			p16[0]=0x2000;
//			p16[1]=0x2000;
//		}
//	#endif
#if (!((Project/100)==3 && (USER/100)==5))//非上海专变
	//工厂模式(优先于设置键)
		if(Comm_Ram->Factory==0x55)//工厂状态
		{
			p16[0]=25;// '工厂模式'
			p16[1]=26;
		}
		if(Stack->MenuStack[MenuCount->Count].EditRow==9)
		{
			p16[0]|=0x8000;
			p16[1]|=0x8000;
		}
#endif
}


/*
void DisplayMenu(u32 MoveRow,u8 * pStr)//显示菜单
---------------void MenuCreate(u32 FunctionAddr)//创建下级菜单------------
---------------void MenuDel(void)//返回上级菜单---------------------------
void MenuSelect(void)//菜单选择

void DisplayClr(void)//清全屏
void DisplayClrRow(u32 StartRow,u32 EndRow)//清屏幕显示的指定行
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示字符串
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr)//显示字符串仅在1行内
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//数据显示(调试用),入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF

void MR(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//存储器读
void MW(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//存储器写
void MEMClr(u8 Data,u32 ADDR_FLASH,u32 Byte);//存储器清除
u64 MRR(u32 ADDR_FLASH,u8 Byte);//存储器读到寄存器(<=8字节)
void MEMWriteReg(u64 Data,u32 ADDR_FLASH,u8 Byte);//寄存器写到存储器(<=8字节)


//读写数据缓冲(定义在最后和堆栈相连)
#define LEN_DATABUFF     4096
#define ADDR_DATABUFF     (ADDR_Terminal_Ram_Start+LEN_Terminal_Ram)
//保留字对齐的4096字节



Comm_Ram->ButtonVal//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=
Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器

typedef __packed struct
{
  u8 Count;//菜单堆栈计数
  u8 Res1;
  u8 Res2;
  u8 Res3;
  u32 v1;//菜单用变量1
  u32 v2;//菜单用变量2
  u32 v3;//菜单用变量3
}MenuCount_TypeDef;
#define MenuCount     ((MenuCount_TypeDef *)(ADDR_MenuStack))
#define MaxMenuCount     10//最大菜单级数
typedef __packed struct
{
  u32 PC;//菜单任务运行地址(特别注意:此值不可随便改动)
  u8 Task;//菜单任务计数(进入和退出时均清为0)
  u8 MoveRow;//菜单上移条数(进入时总为0,返回时不变)
  u8 EditRow;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
  u8 EditCol;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
  u8 TotalRow;//菜单总条数
  u8 MenuRow;//菜单当前装入条数
  u8 res2;
  u8 res3;
}MenuSave_TypeDef;
typedef __packed struct
{
  MenuSave_TypeDef     MenuStack[MaxMenuCount+1];
}MenuStack_TypeDef;
#define Stack     ((MenuStack_TypeDef *)(ADDR_MenuStack+16))



typedef __packed struct
{
  u8 DotLn;//与上行间距点数(0-255)
  u8 Row;//字符串号(0-9)(1个字符串在屏上可分多行显示)
  u8 Font;//本行字体号0-4
  u8 Align;//对齐方式,0=左对齐,1=居中,2=右对齐
  u16 Code[40];//显示编码
               //显示编码每二字节代表8*16半字,每行显示缓冲80字节40个半字
               //D15=1表示反显
              //B14=0B13=0从自建小字库取点阵,最大编号空间0-0X2000=8192
              //B14=0B13=1,B12-B0=0-255从ASCII8*16标准字库取点阵,最大编号空间=256
              //B14=0B13=1,B12-B0=256-511从ASCII8*12标准取点阵(暂不用),最大编号空间=256
              //B14=0B13=1,B12-B0=512-767从ASCII8*8标准取点阵(暂不用),最大编号空间=256
              //B14=1B13-B0表示从GB2312标准字库(修改后)取点阵,最大编号空间0-0X4000
}StrLnBuff_TypeDef;
typedef __packed struct
{
  StrLnBuff_TypeDef Row[16];
}StrBuff_TypeDef;
#define StrBuff     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF))


*/

/*
菜单中设置键判断用如下：
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
#endif

  	if(Comm_Ram->Factory=0x55)//工厂状态
	{
		p16[0]=25;// '工厂模式'
		p16[1]=26;
	}
	//在工厂模式下，设置键不论如何总是有效的。

*/
/*
王造中 13:57:04
  Terminal_Ram->AFN04F10UpDate=0;//0=AFN04F10终端电能表/交流采样装置配置参数更新标志,!=0没更新
王造中 14:33:48
  Terminal_Ram->AFN04F10F11F13UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
王造中 14:34:30
  原Terminal_Ram->AFN04F10UpDate不用了。
王造中 10:47:13
  #define IC_MAC     0//0=没,1=有
王造中 18:50:41
  IC_MAC已改为IC_24AAxxx
*/
/*
Comm_Ram->RouterType//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
(u32)&Terminal_Router->RouterAddr
typedef __packed struct
{
 u8 RouterAddr[6];//路由主节点地址(总是放在最前,通信库地址定位用)
 u8 RouterCODE[4];//厂商代码 ASCII 2,芯片代码 ASCII 2
 u8 RouterVer[5];//路由器版本号,分别为:日,月,年,2BYTE版本
*/






