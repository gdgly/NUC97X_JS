
//菜单3 控制状态
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"


#if ((Project/100)==3)//专变终端

const u8 ControlStateList[][5]=//控制解除投入列表
{
	"解除",
	"投入",
};

const u8 pMenu_3[]=
{
	"控制状态\n\r"
	"总加组\n\r"
};

const u8 NControlList[][18]=//无控制投入列表
{
	"   当前无控制投入",
};

#if(USER/100==5)//上海用户
const u8 ControlList[][20]=//控制列表
{
	"时段控    :",
	"厂休控    :",
	"营业报停控:",
	"功率下浮控:",
	"月电控    :",
	"购电控:    保电:",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3
// Descriptions:        控制状态
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3(void)//菜单3
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3);
			p8=(u8*)(ADDR_STRINGBUFF+2*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://任务1:选择
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(3,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		x=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,2);//读功控状态、电控状态
		y=MRR(ADDR_AFN0CF5,1);//读保电状态
		if(x==0&&y==0)
		{
			DisplayString(3,0,0,(u8 *)NControlList[0]);
		}
		else
		{
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,1);//读功控状态
			for(i=0;i<4;i++)
			{
				DisplayString(3+i,0,0,(u8 *)ControlList[i]);
				DisplayString(3+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0时段控D1厂休控D2营业报停控D3当前功率下浮控 解除 投入
			}
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+3,1);//读电控状态
			for(i=0;i<2;i++)
			{
				DisplayString(7+i,0,0,(u8 *)ControlList[i+4]);
			}
			DisplayString(7,14,0,(u8 *)ControlStateList[(y>>0)&0x01]);//D0月电控D1购电控 解除 投入
			DisplayString(8,7,0,(u8 *)ControlStateList[(y>>1)&0x01]);//D0月电控D1购电控 解除 投入
			y=MRR(ADDR_AFN0CF5,1);//读保电状态
			DisplayString(8,16,0,(u8 *)ControlStateList[y&0x01]);//D0保电 解除 投入
		}
	}
	else
	{
		DisplayString(3,4,0,"无效总加组号");
	}
}

#else
const u8 ControlList[][14]=//控制列表
{
	"时段控    :",
	"厂休控    :",
	"营业报停控:",
	"功率下浮控:",
	"月电控    :",
	"购电控    :",
	"保电      :",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3
// Descriptions:        控制状态
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_3(void)//菜单3
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3);
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://任务1:选择
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(2,8);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		x=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,2);//读功控状态、电控状态
		y=MRR(ADDR_AFN0CF5,1);//读保电状态
		if(x==0&&y==0)
		{
			DisplayString(2,0,0,(u8 *)NControlList[0]);
		}
		else
		{
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,1);//读功控状态
			for(i=0;i<4;i++)
			{
				DisplayString(2+i,0,0,(u8 *)ControlList[i]);
				DisplayString(2+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0时段控D1厂休控D2营业报停控D3当前功率下浮控 解除 投入
			}
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+3,1);//读电控状态
			for(i=0;i<2;i++)
			{
				DisplayString(6+i,0,0,(u8 *)ControlList[i+4]);
				DisplayString(6+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0月电控D1购电控 解除 投入
			}
			y=MRR(ADDR_AFN0CF5,1);//读保电状态
			DisplayString(8,0,0,(u8 *)ControlList[6]);
			DisplayString(8,16,0,(u8 *)ControlStateList[y&0x01]);//D0保电 解除 投入
		}
	}
	else
	{
		DisplayString(2,4,0,"无效总加组号");
	}
}
#endif

#endif
