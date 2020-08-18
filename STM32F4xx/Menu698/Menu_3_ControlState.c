
//菜单3 控制状态
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_DataDef.h"



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
#if ((Project/100)==3)||((Project/100)==4)//专变终端

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
	u32 y;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=NUMmax_TAG;
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
			NdigitSelect(1,9,1,NUMmax_TAG);
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
	if(MenuCount->v2<=NUMmax_TAG && MenuCount->v2>=1)
	{
		y=0;
		i=MRR(ADDR_Terminal_kWCtrl4_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<0);
		
		i=MRR(ADDR_Terminal_kWCtrl3_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<1);

		i=MRR(ADDR_Terminal_kWCtrl2_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<2);

		i=MRR(ADDR_Terminal_kWCtrl1_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<3);
		
		i=MRR(ADDR_Terminal_kWhCtrl1_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<4);
		
		i=MRR(ADDR_Terminal_kWhCtrl2_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<5);
	
		if(y==0)//功控，电控都没有
		{
			DisplayString(2,0,0,(u8 *)NControlList[0]);
		}
		else
		{
			for(i=0;i<6;i++)
			{
				DisplayString(2+i,0,0,(u8 *)ControlList[i]);
				DisplayString(2+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0时段控D1厂休控D2营业报停控D3当前功率下浮控 解除 投入
			}
			
			y=MRR(ADDR_8001_2+2,1);//读保电状态
			DisplayString(8,0,0,(u8 *)ControlList[6]);
			DisplayString(8,14,0,(u8 *)ControlStateList[y&0x01]);//D0保电 解除 投入
		}
	}
	else
	{
		DisplayString(2,4,0,"无效总加组号");
	}
}
#endif
