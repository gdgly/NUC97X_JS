//主菜单
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "Menu.h"
#include "Menu1.h"
#include "Menu2.h"
#include "Menu3.h"
#include "Menu4.h"
#include "Menu5.h"
#include "Menu6.h"
#include "Menu7.h"
#include "Menu8.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"

void _0(void)
{
}


#if ((Project/100)==3) || ((Project/100)==9)//负控终端
const u8 pMenu_Main[]=//负控终端主菜单
{
	"主菜单\n\r"
	"实时数据\n\r"
	"电能计量\n\r"
	"电能质量监测\n\r"
	"停电统计\n\r"
	"事件记录\n\r"
	"异常告警\n\r"
	"参数设置与查询\n\r"
	"终端管理与维护\n\r"
};
#endif

#if ((Project/100)==2)//集中器
const u8 JZQpMenu_Main[]=//集中器主菜单
{
	"\n\r"
	"       主菜单\n\r"	
	"数据查询\n\r"
	"参数设置与查询\n\r"
	"终端管理与维护\n\r"
};
#endif
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
	u32 k;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			MenuCount->v1=0;
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
				else	//err20181130 增加液晶 密码首尾循环功能
				{
					Stack->MenuStack[MenuCount->Count].EditCol = 6;//回到第一个数字
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				MemCopy(&x,p16,2);
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
					
						i=Stack->MenuStack[MenuCount->Count].EditCol-6;
						i=(Length-i-1)*4;
						MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
						p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				else //err20181130 增加液晶 密码首尾循环功能
				{
					Stack->MenuStack[MenuCount->Count].EditCol = 12;//选择最后一个字符
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				MemCopy(&x,p16,2);
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
		case 2://任务2：判断密码是否正确，正确则跳转到菜单7或8，错误提示对话框			
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_GUIPASSWORD,3);
			p32[0]&=0x00FFFFFF;
			k=hex_bcd(p32[0]);
			k&=0x00FFFFFF;
			MenuCount->v1&=0xFFFFFF;
			if((k==MenuCount->v1)||(PWD==MenuCount->v1))	//代码优化，添加一个超级密码，当输入的密码与设置的密码不匹配时，输入超级密码PWD也可进入 ybzyl_20170901		
			{//比较输入的密码与正确的密码是否相等
				#if ((Project/100)==3) || ((Project/100)==9)//负控
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==7)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_7;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;
					}		
					else
					{
						MenuDel();//返回上级菜单	    
					}
				#endif
				#if ((Project/100)==2)//集中器
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_7;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==4)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;
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



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_Main
// Descriptions:        主菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_Main(void)//主菜单
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if ((Project/100)==3) || ((Project/100)==9)//负控终端
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_Main);//显示菜单
			#endif
			#if ((Project/100)==2)//集中器
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_Main);//显示菜单
//			Stack->MenuStack[MenuCount->Count].EditRow=2;
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
						#if ((Project/100)==3) || ((Project/100)==9)//负控终端&配变
							MenuCreate((u32)Menu_1);//创建下级菜单  1.实时数据
						#endif
						break;
					case 2:
						#if ((Project/100)==3) || ((Project/100)==9)//负控终端
							MenuCreate((u32)Menu_2);//创建下级菜单  2.电能计量							
						#endif
						#if ((Project/100)==2)//集中器
							MenuCreate((u32)Menu_1);//数据查询
						#endif
						break;
					case 3:							
						#if ((Project/100)==3) || ((Project/100)==9)//负控终端
							MenuCreate((u32)Menu_3);//创建下级菜单  3.电能质量监测							
						#endif
						#if ((Project/100)==2)//集中器
							PassWordCreateEdit();//创建密码编辑框
						#endif
						break;
					case 4:
						#if ((Project/100)==3) || ((Project/100)==9)//负控终端
							MenuCreate((u32)Menu_4);//创建下级菜单  4.停电统计						
						#endif
						#if ((Project/100)==2)//集中器
							PassWordCreateEdit();//创建密码编辑框
						#endif
						break;
					case 5:
						MenuCreate((u32)Menu_5);//创建下级菜单  5.事件记录
						break;
					case 6:
						MenuCreate((u32)Menu_6);//创建下级菜单  6.异常告警
						break;
					case 7:						
						PassWordCreateEdit();//创建密码编辑框
						break;
					case 8:
						PassWordCreateEdit();//创建密码编辑框
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
