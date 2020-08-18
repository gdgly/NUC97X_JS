
//菜单6 购电信息
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




#if ((Project/100)==3)//专变终端

extern const u8 SList[][2];//正负极性列表

const u8 QCUnitList[][7]=//电量(费)单位列表
{
	"kWh/厘",
	"MWh/元",
};

const u8 PurchaseWayList[][5]=//购电方式列表
{
	"追加",
	"刷新",
	"无效",
};

const u8 pMenu_6[]=
{
	"购电信息\n\r"
	"控制对象\n\r"
};

#if ((Project/100)==3 && (USER/100)==5)//上海专变
const u8 PPList[][20]=//购电控参数列表
{
	"购电单号:",//0
	"购电方式:",//1
	"购前电量:",//2
	" ",
	"购后电量:",//4
	" ",	
	"报警门限:",//6
	" ",
	"跳闸门限:",//8
	" ",
	"剩余电量:",//10	
	" ",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6_1
// Descriptions:        购电信息（第2屏）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_6_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
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
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//上键
					CreateScreen((u32)Menu_6);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			break;
		default:
			break;
	}
	DisplayClrRow(3,8);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PPList[i+6]);
		}
		//1.报警门限2.跳闸门限
		for(i=0;i<2;i++)
		{
			y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+9+4*i,4);
			if(y==0xEEEEEEEE)
			{//无此数据填-------
				DisplayString(4+2*i,3,0,"-------");	
			}
			else
			{
				z=y&0xfffffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+2*i)*84+3*2);
				p16=DisplayData_OffZero(z,7,0,p16);
				DisplayString(4+2*i,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
				for(j=4;j<20;j++)
				{
					if(p16 == (u16 *)(ADDR_STRINGBUFF+(4+2*i)*84+4+j*2))
					{
						break;
					}
				}
				DisplayString(4+2*i,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
			}
			DispalyAlign(4+2*i,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
		}
		//3.剩余电量（费）
		y=MRR(ADDR_AFN0CF23+4*(MenuCount->v2-1),4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(8,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(8,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(8,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
		}
		DispalyAlign(8,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
		UpArrow();
	}
	else
	{
		DisplayString(3,4,0,"无效控制对象");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6
// Descriptions:        购电信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_6(void)//菜单6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u64 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
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
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//下键
					CreateScreen((u32)Menu_6_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			break;
		default:
			break;
	}
	DisplayClrRow(3,8);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PPList[i]);
		}
		//1.购电单号
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1),4);
		z=hex_bcd16(y);
		p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+10*2);
		DisplayData(z,10,0,p16);
		//2.购电方式
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+4,1);
		switch(y)
		{
			case 0x55:
				i=0;
				break;
			case 0xAA:
				i=1;
				break;
			default:
				i=2;
				break;
		}
		DisplayString(4,10,0,(u8 *)PurchaseWayList[i]);//0追加 1刷新 2无效
		//3.购前电量
		y=MRR(ADDR_AFN04F47_Ext+8*(MenuCount->v2-1),4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(6,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(6,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+6*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(6,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
		}
		DispalyAlign(6,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
		//4.购后电量
		y=MRR(ADDR_AFN04F47_Ext+8*(MenuCount->v2-1)+4,4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(8,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(8,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(8,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
		}
		DispalyAlign(8,17,3);//行显示对齐(1左对齐,2居中,3右对齐)

		DownArrow();
	}
	else
	{
		DisplayString(3,4,0,"无效控制对象");	
	}
}

#else

const u8 PPList[][20]=//购电控参数列表
{
	"购电单号:",//0
	"购电方式:",//1
	"购电量费:",//2	
	" ",	
	"报警门限:",//4
	" ",
	"跳闸门限:",//6
	" ",
	"剩余电量费:",//8	
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6_1
// Descriptions:        购电信息（第2屏）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_6_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
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
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//上键
					CreateScreen((u32)Menu_6);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			break;
		default:
			break;
	}
	DisplayClrRow(2,3);//清屏幕显示的指定行
	p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		DisplayString(2,0,0,(u8 *)PPList[8]);
		//剩余电量（费）
		y=MRR(ADDR_AFN0CF23+4*(MenuCount->v2-1),4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(3,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(3,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(3,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
		}
		DispalyAlign(3,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
		UpArrow();
	}
	else
	{
		DisplayString(2,4,0,"无效控制对象");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6
// Descriptions:        购电信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_6(void)//菜单6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 y;
	u64 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
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
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//下键
					CreateScreen((u32)Menu_6_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PPList[i]);
		}
		//1.购电单号
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1),4);
		z=hex_bcd16(y);
		p16=(u16 *)(ADDR_STRINGBUFF+4+2*84+10*2);
		DisplayData(z,10,0,p16);
		//2.购电方式
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+4,1);
		switch(y)
		{
			case 0x55:
				i=0;
				break;
			case 0xAA:
				i=1;
				break;
			default:
				i=2;
				break;
		}
		DisplayString(3,10,0,(u8 *)PurchaseWayList[i]);//0追加 1刷新 2无效
		//3.购电量（费）4.报警门限5.跳闸门限
		for(i=0;i<=2;i++)
		{
			y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+5+4*i,4);
			if(y==0xEEEEEEEE)
			{//无此数据填-------
				DisplayString(5+2*i,3,0,"-------");	
			}
			else
			{
				z=y&0xfffffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(5+2*i)*84+3*2);
				p16=DisplayData_OffZero(z,7,0,p16);
				DisplayString(5+2*i,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
				for(j=4;j<20;j++)
				{
					if(p16 == (u16 *)(ADDR_STRINGBUFF+(5+2*i)*84+4+j*2))
					{
						break;
					}
				}
				DisplayString(5+2*i,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
			}
			DispalyAlign(5+2*i,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
		}
		DownArrow();
	}
	else
	{
		DisplayString(2,4,0,"无效控制对象");	
	}
}
#endif

#endif
