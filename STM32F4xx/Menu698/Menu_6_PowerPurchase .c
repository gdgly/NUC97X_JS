
//菜单6 购电信息
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"





const u8 QCUnitList[][3]=//电量(费)单位列表
{
	"kWh",
	"元 ",
};

const u8 PurchaseWayList[][5]=//购电方式列表
{
	"追加",
	"刷新",
	"无效",
};

const u8 PurchaseTypeList[][5]=//购电类型列表
{
	"电量",
	"电费",
	"无效",
};

const u8 pMenu_6[]=
{
	"购电信息\n\r"
	"控制对象\n\r"
};

const u8 PPList[][20]=//购电控参数列表
{
	"单号:",//0
	"购电方式:",//1
	"购电类型:",//1
	"购电量费:",//2	
	" ",	
	"报警门限:",//4
	" ",
	"跳闸门限:",//6
	" ",
	"剩余电量费:",//8	
};
#if ((Project/100)==3)||((Project/100)==4)//专变终端

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
	u64 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//显示菜单
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			for(i=0;i<NUMmax_TAG;i++)
			{
				y=Get_ElementADDR(ADDR_8107_2,i+1,1,LENper_8107_2);//读取 总加组对象      OI，
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=i;
						break;
					}
				}
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
			NdigitSelect(1,9,1,NUMmax_TAG);
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
	if(MenuCount->v3!=0xffff)
	{
		for(i=7;i<10;i++)
		{
			DisplayString(2+i-7,0,0,(u8 *)PPList[i]);
		}
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,4,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			if(y>1)
				y=2;
		}
		else
		{
			y=2;
		}
		i=y;
		//跳闸门限
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,7,LENper_8107_2);
		if(y==0)
		{//无此数据填-------
			DisplayString(3,3,0,"-------"); 
		}
		else
		{
			z=MRR(y+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+3*2);

			if(i==0)
				disp_long64_kw(z,1,4,p16,2);
			else
				disp_long64_kw(z,1,4,p16,4);
		}
		
		//剩余
		y=MRR(ADDR_230x_11+(MenuCount->v2-1)*LENper_230x_11,1);
		if(y!=DataType_long64)
		{//无此数据填-------
			DisplayString(5,3,0,"-------");	
		}
		else
		{
			z=MRR(ADDR_230x_11+(MenuCount->v2-1)*LENper_230x_11+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+3*2);
			if(i==0)
				disp_long64_kw(z,0,1,p16,2);
			else
				disp_long64_kw(z,0,1,p16,4);
		}

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
	u32 y;
	u64 z;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//显示菜单
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			for(i=0;i<NUMmax_TAG;i++)
			{
				y=Get_ElementADDR(ADDR_8107_2,i+1,1,LENper_8107_2);//读取 总加组对象      OI，
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=i;
						break;
					}
				}
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
			NdigitSelect(1,9,1,NUMmax_TAG);
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
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PPList[i]);
		}
		//1.购电单号
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,2,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,4);
			y=R_Inverse(y,4);
		}
		z=hex_bcd16(y);
		p16=(u16 *)(ADDR_STRINGBUFF+4+2*84+5*2);
		DisplayData_OffZero(z,10,0,p16);
		//2.购电方式
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,3,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			if(y>1)
				y=2;
		}
		else
		{
			y=2;
		}

		DisplayString(3,10,0,(u8 *)PurchaseWayList[y]);//0追加 1刷新 2无效
		//3.购电类型
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,4,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			if(y>1)
				y=2;
		}
		else
		{
			y=2;
		}
		i=y;
		DisplayString(4,10,0,(u8 *)PurchaseTypeList[y]);//0电量 1电费 2无效
		
		//4.购电量（费）4.报警门限5.跳闸门限
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,5,LENper_8107_2);
		if(y==0)
		{//无此数据填-------
			DisplayString(6,3,0,"-------");	
		}
		else
		{
			z=MRR(y+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+3*2);

			if(i==0)
				disp_long64_kw(z,1,4,p16,2);
			else
				disp_long64_kw(z,1,4,p16,4);
		}
		//5.报警门限
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,6,LENper_8107_2);
		if(y==0)
		{//无此数据填-------
			DisplayString(8,3,0,"-------"); 
		}
		else
		{
			z=MRR(y+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);

			if(i==0)
				disp_long64_kw(z,1,4,p16,2);
			else
				disp_long64_kw(z,1,4,p16,4);
		}

		DownArrow();
	}
	else
	{
		DisplayString(2,4,0,"无效控制对象");	
	}
}

#endif
