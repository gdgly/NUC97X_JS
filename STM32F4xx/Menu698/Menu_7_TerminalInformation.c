
//菜单7 终端信息
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"


extern const u8 UnitList[][6];//通用单位列表
//extern const u8 OnlineModeList[][11];//在线模式列表
//extern const u8 ConnectWayList[][6];//连接方式列表

const u8 pMenu_7[]=
{
	"终端信息\n\r"
	"  1.软件版本\n\r"
	"  2.终端地址\n\r"
};

const u8 pMenu_7_1[]=
{
	"终端版本\n\r"
	"软件版本:\n\r"
	"日期:\n\r"
	"硬件版本:\n\r"
	"日期:\n\r"
};

const u8 pMenu_7_2[]=
{
	"通信地址\n\r"
	"地址\n\r"
};
#if ((Project/100)==3)||((Project/100)==4)//专变终端

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2
// Descriptions:        终端地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2(void)
{
	u16 *p16;
	u64 x=0;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2);//显示菜单
	//1.地址
	p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+4*2);
	MR(ADDR_DATABUFF,ADDR_4001+2,6);
	Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
	x=MRR(ADDR_DATABUFF,6);
	DisplayData(x,12,0,p16);

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1
// Descriptions:        版本信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1(void)
{
	u16 *p16;
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1);//显示菜单
	//1.厂商代码
	//1.软件版本
	p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(SVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((SVER>>4)&0xf)+0x2030;
	p16[4]=((SVER>>0)&0xf)+0x2030;

	//2.软件版本日期
	p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2);
	p16[0]=2+0x2030;
	p16[1]=0+0x2030;
	p16[2]=((SVDATE>>20)&0xf)+0x2030;
	p16[3]=((SVDATE>>16)&0xf)+0x2030;
	p16[4]='-'+0x2000;
	p16[5]=((SVDATE>>12)&0xf)+0x2030;
	p16[6]=((SVDATE>>8)&0xf)+0x2030;
	p16[7]='-'+0x2000;
	p16[8]=((SVDATE>>4)&0xf)+0x2030;
	p16[9]=((SVDATE>>0)&0xf)+0x2030;
	//3.硬件版本
	p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(HVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x2030;
	p16[4]=((HVER>>0)&0xf)+0x2030;
	//4.硬件版本日期
	p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2);
	p16[0]=2+0x2030;
	p16[1]=0x2030;
	p16[2]=((HVDATE>>20)&0xf)+0x2030;
	p16[3]=((HVDATE>>16)&0xf)+0x2030;
	p16[4]='-'+0x2000;
	p16[5]=((HVDATE>>12)&0xf)+0x2030;
	p16[6]=((HVDATE>>8)&0xf)+0x2030;
	p16[7]='-'+0x2000;
	p16[8]=((HVDATE>>4)&0xf)+0x2030;
	p16[9]=((HVDATE>>0)&0xf)+0x2030;	
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7
// Descriptions:        终端信息菜单
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_7_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_7_2);//创建下级菜单
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
}
#endif
