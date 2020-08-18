//菜单5 中文信息
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"



const u8 pMenu_5[]=
{
	"中文信息\n\r"
	"  1.普通中文信息\n\r"
	"  2.重要中文信息\n\r"
};

const u8 pMenu_5_1[]=
{
	"普通中文信息\n\r"
	"信息编号:\n\r"
};

const u8 pMenu_5_2[]=
{
	"重要中文信息\n\r"
	"信息编号:\n\r"
};

#if ((Project/100)==3)||((Project/100)==4)//专变终端

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_1
// Descriptions:        普通中文信息菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_1(void)//菜单5_1
{
	return;
	/*
	u8 *p8;
	u16 *p16;
	u32 m;
	u32 y;
	u32 x;
	u32 Ln;
	u32 i;
	u32 j;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_1);//显示菜单
			if(MenuCount->v2>16)
			{//MenuCount->v2记录信息编号（1~16对应0~15）
				MenuCount->v2=16;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v1=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
			break;
		case 1://任务1:选择
			NdigitSelect(2,9,1,16);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	for(j=0;j<2;j++)
	{
		DisplayClrRow(2,9);//清屏幕显示的指定行
		if(MenuCount->v2<=16 && MenuCount->v2>=1)
		{
			m=MEMReadReg(ADDR_AFN05F32+202*(MenuCount->v2-1),1);
			if(m>>4==0 && (m&0xf)==MenuCount->v2-1)//modified by sym 20120504
			{//判断是否为普通信息0，信息编号是否为MenuCount->v2-1
				y=MEMReadReg(ADDR_AFN05F32+202*(MenuCount->v2-1)+1,1);//读普通信息长度
				p8=(u8*)ADDR_DATABUFF;
				MEMClr(0,ADDR_DATABUFF,202);
				MEMRead(ADDR_DATABUFF,ADDR_AFN05F32+202*(MenuCount->v2-1)+2,y);
				x=DisplayString1(2,0,0,p8+MenuCount->v1);
				Ln=x&0xff;
				i=x>>16;
				if(Ln==9 && p8[i]!=0)
				{
					DownArrow();
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//下
						MenuCount->v1=i;
						UpArrow();
					}
					Comm_Ram->ButtonVal&=~0x01;//键使用后清0
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//右上角有↑
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//上
						p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
						p16[0]=0x0;
						p16[1]=0x0;
						MenuCount->v1=0;
						DownArrow();
					}
					Comm_Ram->ButtonVal&=~0x02;//键使用后清0
				}
			}
		}
		else
		{
			DisplayString(2,5,0,"无效信息号");
		}
	}
	*/
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_2
// Descriptions:        重要中文信息菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5_2(void)//菜单5_2
{
	return;
	/*
	u8 *p8;
	u16 *p16;
	u32 m;
	u32 y;
	u32 x;
	u32 Ln;
	u32 i;
	u32 j;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_2);//显示菜单
			if(MenuCount->v2>16)
			{//MenuCount->v2记录信息编号（1~16对应0~15）
				MenuCount->v2=16;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v1=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
			break;
		case 1://任务1:选择
			NdigitSelect(2,9,1,16);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	for(j=0;j<2;j++)
	{
		DisplayClrRow(2,9);//清屏幕显示的指定行
		if(MenuCount->v2<=16 && MenuCount->v2>=1)
		{
			m=MEMReadReg(ADDR_AFN05F32+202*16+202*(MenuCount->v2-1),1);
			if(m>>4==1 && (m&0xf)==MenuCount->v2-1)//modified by sym 20120504
			{//判断是否为重要信息1，信息编号是否为MenuCount->v2-1
				y=MEMReadReg(ADDR_AFN05F32+202*16+202*(MenuCount->v2-1)+1,1);//读重要信息长度
				p8=(u8*)ADDR_DATABUFF;
				MEMClr(0,ADDR_DATABUFF,202);
				MEMRead(ADDR_DATABUFF,ADDR_AFN05F32+202*16+202*(MenuCount->v2-1)+2,y);
				x=DisplayString1(2,0,0,p8+MenuCount->v1);
				Ln=x&0xff;
				i=x>>16;
				if(Ln==9 && p8[i]!=0)
				{
					DownArrow();
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//下
						MenuCount->v1=i;
						UpArrow();
					}
					Comm_Ram->ButtonVal&=~0x01;//键使用后清0
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//右上角有↑
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//上
						p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
						p16[0]=0x0;
						p16[1]=0x0;
						MenuCount->v1=0;
						DownArrow();
					}
					Comm_Ram->ButtonVal&=~0x02;//键使用后清0
				}
			}
		}
		else
		{
			DisplayString(2,5,0,"无效信息号");
		}
	}
	*/
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5
// Descriptions:        中文信息菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_5(void)//菜单5
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{	  //确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCount->v1=0;
						//MenuCreate((u32)Menu_5_1);//创建下级菜单
						break;
					case 2:
						MenuCount->v1=0;
						//MenuCreate((u32)Menu_5_2);//创建下级菜单
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
