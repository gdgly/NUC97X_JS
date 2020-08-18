
//菜单4 电能表示数
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Terminal/Terminal_AFN0C_RealTimeData_Fn.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"



extern const u8 UnitList[][6];//通用单位列表
extern const u8 SList[][2];//正负极性列表

#if ((Project/100)==2)//集中器
const u8 pMenu2_4[]=//集中器测量点数据显示
{
	"测量点数据显示\n\r"
	"  1.电压电流\n\r"
	"  2.电表功率\n\r"
	"  3.正向电能\n\r"
	"  4.反向电能\n\r"
	"  5.最大需量\n\r"
	"  6.功率因数\n\r"
	"  7.四象限无功电能\n\r"
};

const u8 pMenu_4_7[]=//四象限无功电能
{
	"四象限无功电能\n\r"
	"  1.一象限无功电能\n\r"
	"  2.二象限无功电能\n\r"
	"  3.三象限无功电能\n\r"
	"  4.四象限无功电能\n\r"
};
#endif

const u8 pMenu_4[]=
{
	"电能表示数\n\r"
	"  1.电压电流\n\r"
	"  2.电表功率\n\r"
	"  3.正向电能\n\r"
	"  4.反向电能\n\r"
	"  5.最大需量\n\r"
	"  6.功率因数\n\r"
};

const u8 pMenu_4_1[]=
{
	"三相电压电流\n\r"
	"测量点\n\r"
};

const u8 UIList[][20]=//三相电压电流列表
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//集中器、非上海专变
	"局编号:",
	"表地址:",
#endif
	"电压A :",
	"    B :",
	"    C :",
	"电流A :",
	"    B :",
	"    C :",
};

const u8 pMenu_4_2[]=
{
	"电表功率\n\r"
	"测量点\n\r"
};

const u8 ActivePowerList[][20]=//电表有功功率列表
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//集中器、非上海专变
	"局编号:",
#endif
	"表地址:",
	"有功总:",
	"     A:",
	"     B:",
	"     C:",
};

const u8 ReactivePowerList[][20]=//电表无功功率列表
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//集中器、非上海专变
	"局编号:",
#endif
	"表地址:",
	"无功总:",
	"     A:",
	"     B:",
	"     C:",
};

const u8 ElectricEnergyList[][20]=//电能量列表
{
#if ((USER/100)==5)//上海用户
	#if ((Project/100)==3)//专变
	"表地址:",
	"  总:",
	"  峰:",
	"  平:",
	"  尖:",
	"  谷:",
	#elif ((Project/100)==2)//集中器
	"局编号:",
	"表地址:",
	"  总:",
	"  峰:",
	"  平:",
	"  尖:",
	"  谷:",
	#endif
#else
	"局编号:",
	"表地址:",
	"  总:",
	"  尖:",
	"  峰:",
	"  平:",
	"  谷:",
#endif
};

const u8 pMenu_4_5[]=
{
	"最大需量\n\r"
	"  1.当月正向有功\n\r"
	"  2.当月正向无功\n\r"
	"  3.当月反向有功\n\r"
	"  4.当月反向无功\n\r"
	"  5.上月正向有功\n\r"
	"  6.上月正向无功\n\r"
	"  7.上月反向有功\n\r"
	"  8.上月反向无功\n\r"
};

const u8 Demand1List[][20]=
{
#if ((USER/100)==5)//上海用户
	#if ((Project/100)==3)//专变
	"总:",
	" ",
	"峰:",
	" ",
	"平:",
	#elif ((Project/100)==2)//集中器
	"局编号:",
	"表地址:",
	"总:",
	" ",
	"峰:",
	" ",
	"平:",
	#endif
#else
	"局编号:",
	"表地址:",
	"总:",
	" ",
	"尖:",
	" ",
	"峰:",
#endif
};

const u8 Demand2List[][20]=
{
#if ((USER/100)==5)//上海用户
	#if ((Project/100)==3)//专变
	"尖:",
	" ",
	"谷:",
	#elif ((Project/100)==2)//集中器
	"局编号:",
	"表地址:",
	"尖:",
	" ",
	"谷:",
	#endif
#else
	"局编号:",
	"表地址:",
	"平:",
	" ",
	"谷:",
#endif
};

const u8 pMenu_4_5_1[]=
{
	"当月正向有功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_2[]=
{
	"当月正向无功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_3[]=
{
	"当月反向有功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_4[]=
{
	"当月反向无功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_5[]=
{
	"上月正向有功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_6[]=
{
	"上月正向无功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_7[]=
{
	"上月反向有功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_5_8[]=
{
	"当月反向无功\n\r"
	"测量点\n\r"
};

const u8 pMenu_4_6[]=
{
	"电表功率因数\n\r"
	"测量点\n\r"
};

const u8 PFList[][20]=//电表功率因数列表
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//集中器、非上海专变
	"局编号:",
#endif
	"表地址:",
	"因数总:",
	"     A:",
	"     B:",
	"     C:",
};

/*
u32 Check_AFN04F10_Pn(u32 Pn)//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
{
#if IC_SST39VF6401B==0//0=没,1=有
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F10+2;
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=MRR(Addr,2);
		if(i==Pn)
		{
			return Meter;
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return 0;
#else
	u32 i;
	u32 Meter;
	u8 *p8;
	if(Pn==0)
	{
		return 0;
	}
	p8=(u8*)(ADDR_AFN04F10+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[0]+(p8[1]<<8);
		if(i==Pn)
		{
			return Meter;
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
#endif
}
*/

#if ((Project/100)==2)//2为集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_4
// Descriptions:        四象限无功电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_7_4(void)//菜单4_7_4
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"四象限无功电能\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//四象限无功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(136,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_3
// Descriptions:        三象限无功电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_7_3(void)//菜单4_7_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"三象限无功电能\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//四象限无功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(135,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_2
// Descriptions:        二象限无功电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_7_2(void)//菜单4_7_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"二象限无功电能\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//二象限无功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(134,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_1
// Descriptions:        一象限无功电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_7_1(void)//菜单4_7_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"一象限无功电能\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//一象限无功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(133,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7
// Descriptions:        四象限无功电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_7(void)//菜单4_7
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_7);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_4_7_1);//一象限无功电能
						break;
					case 2:
						MenuCreate((u32)Menu_4_7_2);//二象限无功电能
						break;
					case 3:
						MenuCreate((u32)Menu_4_7_3);//三象限无功电能
						break;
					case 4:
						MenuCreate((u32)Menu_4_7_4);//四象限无功电能
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_6
// Descriptions:        电表功率因数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_6(void)//菜单4_6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_6);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PFList[i]);
		}
#else
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PFList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif

		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//功率因数 总ABC
		for(i=0;i<=3;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+29+2*i,2);	       
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(4+i,8,0,"-----");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>15]);//+ -
				y&=0x7fff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,4,1,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[6]);//6 %
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_8_1
// Descriptions:        上月反向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_8_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_8);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_8);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//上月反向无功最大需量 平谷
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_8
// Descriptions:        上月反向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_8(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_8);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_8_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//上月反向无功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分 
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_7_1
// Descriptions:        上月反向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_7_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_7);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_7);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//上月反向有功最大需量 平谷
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_7
// Descriptions:        上月反向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_7(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_7);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_7_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//上月反向有功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_6_1
// Descriptions:        上月正向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_6_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_6);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_6);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//上月正向无功最大需量 平谷
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_6
// Descriptions:        上月正向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_6(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_6);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_6_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//上月正向无功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_5_1
// Descriptions:        上月正向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_5_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_5);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_5);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//上月正向有功最大需量 平谷
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_5
// Descriptions:        上月正向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_5(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_5);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{ 
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_5_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//上月正向有功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_4_1
// Descriptions:        当月反向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_4_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_4);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
				CreateScreen((u32)Menu_4_5_4);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//当月反向无功最大需量 平谷
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_4
// Descriptions:        当月反向无功月最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_4(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_4);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_4_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//当月反向无功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_3_1
// Descriptions:        当月反向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_3_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_3);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{ 
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_3);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//当月反向有功最大需量 平谷
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_3
// Descriptions:        当月反向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_3(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_3);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_3_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//当月反向有功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_2_1
// Descriptions:        当月正向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_2_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_2);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_2);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//当月正向无功最大需量 平谷
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_2
// Descriptions:        当月正向无功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_2(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_2);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_2_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//当月正向无功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_1_1
// Descriptions:        当月正向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_1_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_1);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_5_1);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	#if ((Project/100)==2 && (USER/100)==5)//上海集中器
		p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v2-1)*LEN_AFN04F10_Pn)+5);
		if((p8[0]==21) && (p8[13]!=4))
		{//上海规约、费率2
/*
	针对上海表，集中器档案中费率数为2时，集中器费率1对应上海表费率平，集中器费率2对应上海表费率谷；
	集中器档案中费率数为4时，集中器费率1～4分别对应无效数据、上海表费率平、无效数据、上海表费率谷。
	此费率顺序对一类和二类数据均有要求。
*/			
		//尖
			DisplayString(Ln,5,0,"-------");	
			DisplayString(Ln,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			DisplayString(Ln+1,13,0,"-----");	
			//发生时间 月-日
			DisplayString(Ln+1,7,0,"-----");	
		//谷
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+14,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+14,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+14,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		else
		{
			for(i=0;i<=1;i++)
			{
				//当月正向有功最大需量 平谷
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+27+7*i,3);	                   
				if(y==0xEEEEEE)
				{//无此数据填-------
					DisplayString(Ln+2*i,5,0,"-------");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
					DisplayData_OffZero(y,6,4,p16);
				}
				DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
				//发生时间 时:分
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+30+7*i,2);	                   
				if(y==0xEEEE)
				{//无此数据填-----
					DisplayString(Ln+1+2*i,13,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
					DisplayData1(y,4,2,':',p16);
				}
				//发生时间 月-日
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+32+7*i,2);	                   
				if(y==0xEEEE)
				{//无此数据填-----
					DisplayString(Ln+1+2*i,7,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
					DisplayData1(y,4,2,'-',p16);
				}
			}
		}
	#else
		for(i=0;i<=1;i++)
		{
			//当月正向有功最大需量 平谷
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
	#endif
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_1
// Descriptions:        当月正向有功最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_1);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_5_1_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	#if ((Project/100)==2 && (USER/100)==5)//上海集中器
		p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v2-1)*LEN_AFN04F10_Pn)+5);
		if((p8[0]==21) && (p8[13]!=4))
		{//上海规约、费率2
/*
	针对上海表，集中器档案中费率数为2时，集中器费率1对应上海表费率平，集中器费率2对应上海表费率谷；
	集中器档案中费率数为4时，集中器费率1～4分别对应无效数据、上海表费率平、无效数据、上海表费率谷。
	此费率顺序对一类和二类数据均有要求。
*/			
		//总
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		//峰
			DisplayString(Ln+2,5,0,"-------");	
			DisplayString(Ln+2,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			DisplayString(Ln+3,13,0,"-----");	
			//发生时间 月-日
			DisplayString(Ln+3,7,0,"-----");	
		//平
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+4,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+4,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+4,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+4)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+4,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+4)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		else
		{
			for(i=0;i<=2;i++)
			{
				//当月正向有功最大需量 总尖峰
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7*i,3);	                   
				if(y==0xEEEEEE)
				{//无此数据填-------
					DisplayString(Ln+2*i,5,0,"-------");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
					DisplayData_OffZero(y,6,4,p16);
				}
				DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
				//发生时间 时:分
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7*i,2);	                   
				if(y==0xEEEE)
				{//无此数据填-----
					DisplayString(Ln+1+2*i,13,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
					DisplayData1(y,4,2,':',p16);
				}
				//发生时间 月-日
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7*i,2);	                   
				if(y==0xEEEE)
				{//无此数据填-----
					DisplayString(Ln+1+2*i,7,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
					DisplayData1(y,4,2,'-',p16);
				}
			}
		}
	#else
		for(i=0;i<=2;i++)
		{
			//当月正向有功最大需量 总尖峰
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
	#endif
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5
// Descriptions:        最大需量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_5(void)//菜单4_5
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_4_5_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_4_5_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_4_5_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_4_5_4);//创建下级菜单
						break;
					case 5:
						MenuCreate((u32)Menu_4_5_5);//创建下级菜单
						break;
					case 6:
						MenuCreate((u32)Menu_4_5_6);//创建下级菜单
						break;
					case 7:
						MenuCreate((u32)Menu_4_5_7);//创建下级菜单
						break;
					case 8:
						MenuCreate((u32)Menu_4_5_8);//创建下级菜单
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
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_4_2
// Descriptions:        电表正向无功电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_4_2(void)//菜单4_4_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"反向无功电能示值\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//反向无功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(132,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_4_1
// Descriptions:        电表反向有功电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_4_1(void)//菜单4_4_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"反向有功电能示值\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//反向有功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(131,MenuCount->v2)+6+5*i,5);	                   
			if(y==0xEEEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_4
// Descriptions:        电表反向电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_4(void)//菜单4_4
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"反向电能\n\r"
																		"  1.反向有功电能示值\n\r"
																		"  2.反向无功电能示值\n\r"
			);//显示菜单
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
						MenuCreate((u32)Menu_4_4_1);
						break;
					case 2:
						MenuCreate((u32)Menu_4_4_2);
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
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3_2
// Descriptions:        电表正向无功电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_3_2(void)//菜单4_3_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"正向无功电能示值\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//正向无功电能示值
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(130,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3_1
// Descriptions:        电表正向有功电能示值
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_3_1(void)//菜单4_3_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"正向有功电能示值\n\r"
																		"测量点\n\r"
			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//正向有功电能示值
	#if ((Project/100)==2 && (USER/100)==5)//上海集中器
		p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v2-1)*LEN_AFN04F10_Pn)+5);
		if((p8[0]==21) && (p8[13]!=4))
		{//上海规约、费率2
/*
	针对上海表，集中器档案中费率数为2时，集中器费率1对应上海表费率平，集中器费率2对应上海表费率谷；
	集中器档案中费率数为4时，集中器费率1～4分别对应无效数据、上海表费率平、无效数据、上海表费率谷。
	此费率顺序对一类和二类数据均有要求。
*/			
			//总
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6,5);	                   
			if(y==0xEEEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			//峰
			DisplayString(4+1,6,0,"-----------");	
			//平
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5,5);	                   
			if(y==0xEEEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+2,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+2)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			//尖
			DisplayString(4+3,6,0,"-----------");	
			//谷
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+10,5);	                   
			if(y==0xEEEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+4,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+4)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			for(i=0;i<5;i++)
			{
				DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
			}
		}
		else
		{
			for(i=0;i<5;i++)
			{
				y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5*i,5);	                   
				if(y==0xEEEEEEEEEE)
				{//无此数据填-----------
					DisplayString(4+i,6,0,"-----------");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
					DisplayData_OffZero(y,10,4,p16);
				}
				DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
			}
		}
	#else
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5*i,5);	                   
			if(y==0xEEEEEEEEEE)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
		}
	#endif
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3
// Descriptions:        电表正向电能
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_3(void)//菜单4_3
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"正向电能\n\r"
																		"  1.正向有功电能示值\n\r"
																		"  2.正向无功电能示值\n\r"
			);//显示菜单
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
						MenuCreate((u32)Menu_4_3_1);
						break;
					case 2:
						MenuCreate((u32)Menu_4_3_2);
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

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_2_1
// Descriptions:        电表无功功率(第1屏)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuContent_4_2_1(void)//菜单4_2_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_2);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//向上键按下
					CreateScreen((u32)Menu_4_2);
				}
				Comm_Ram->ButtonVal&=~0x02;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,6);//清屏幕显示的指定行
#else
	DisplayClrRow(2,7);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ReactivePowerList[i]);
		}
#else
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ReactivePowerList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当前无功功率 总ABC
		for(i=0;i<=3;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+17+3*i,3);	       
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(4+i,8,0,"-------");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(4+i,16,0,(u8 *)UnitList[1]);//1 kvar
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_2
// Descriptions:        电表有功功率
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_2(void)//菜单4_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_2);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//向下键按下
					CreateScreen((u32)MenuContent_4_2_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ActivePowerList[i]);
		}
#else
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ActivePowerList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当前有功功率 总ABC
		for(i=0;i<=3;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+5+3*i,3);	       
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(4+i,8,0,"-------");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(4+i,16,0,(u8 *)UnitList[0]);//0 kW
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_1
// Descriptions:        电压电流
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4_1(void)//菜单4_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_1);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)UIList[i]);
		}
#else
		for(i=0;i<=7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)UIList[i]);
		}
		//局编号
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//电压ABC		
		for(i=0;i<=2;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+37+2*i,2);  
			if(y==0xEEEE)
			{//无此数据填-----
				DisplayString(Ln+i,8,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+i)*84+8*2);
				DisplayData_OffZero(y,4,1,p16);
			}
			DisplayString(Ln+i,14,0,(u8 *)UnitList[4]);//4 V
		}
		//电流ABC
		Ln+=3;
		for(i=0;i<=2;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+43+3*i,3);   
			if(y==0xEEEEEE)
			{//无此数据填------
				DisplayString(Ln+i,8,0,"------");	
			}
			else
			{
				DisplayString(Ln+i,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+i)*84+8*2);
				DisplayData_OffZero(y,6,3,p16);
			}
			DisplayString(Ln+i,14,0,(u8 *)UnitList[5]);//4 A
		}
	}
	else
	{
		DisplayString(Ln,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4
// Descriptions:        电能表示数/集中器测量点数据显示
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_4(void)//菜单4
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
#if ((Project/100)!=2)//2为集中器
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4);//显示菜单
#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_4);//显示菜单
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
						MenuCreate((u32)Menu_4_1);
						break;
					case 2:
						MenuCreate((u32)Menu_4_2);
						break;
					case 3:
						MenuCreate((u32)Menu_4_3);
						break;
					case 4:
						MenuCreate((u32)Menu_4_4);
						break;
					case 5:
						MenuCreate((u32)Menu_4_5);
						break;
					case 6:
						MenuCreate((u32)Menu_4_6);
						break;
					case 7:
#if ((Project/100)==2)//2为集中器
						MenuCreate((u32)Menu_4_7);//四象限无功电能
#endif
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

