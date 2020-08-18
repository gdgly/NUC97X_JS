
//菜单9 系统管理
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"



const u8 pMenu_9[]=
{
	"终端管理与维护\n\r"
	"  1.重启终端F1\n\r"
	"  2.数据区初始化F2\n\r"
	"  3.恢复出厂配置F3\n\r"
	"  4.参数区初始化F4\n\r"
	"  5.变更密码\n\r"
	"  6.485口功能设置\n\r"
};

const u8 pMenu_9_6[]=
{
	"485口功能设置\n\r"
	"485第 路\n\r"
	"功能:\n\r"
	"      确认设置\n\r"
};

const u8 Function_485List[][20]=//485口功能列表
{
	"终端维护口",
	"终端抄表口",
};

const u8 pMenu_9_5[]=
{
	"菜单密码\n\r"
	"密码:\n\r"
	"      确认设置\n\r"
};

#if ((Project/100)==2)//集中器
const u8 pMenu2_3[]=
{
	"终端管理与维护\n\r"
	"  1.重启终端F1\n\r"
	"  2.数据区初始化F2\n\r"
	"  3.恢复出厂配置F3\n\r"
	"  4.参数区初始化F4\n\r"
	"  5.集中器版本\n\r"
	"  6.载波抄表管理\n\r"
	"  7.现场调试\n\r"
	"  8.设备编号设置\n\r"
#if ((USER/100)==5)//上海集中器
	"  9.手动抄表\n\r"
#endif
#if ((USER/100)==8)//通用集中器
	"  9.交采通信地址\n\r"
#endif
};

#if ((USER/100)==2)//山东版
const u8 Function_232List[][20]=//232口功能列表
{
	"终端监测口",
	"终端维护口",
};
const u8 pMenu2_3_7_2[]=
{
	"232口功能设置\n\r"
	"功能：\n\r"
	"      确认设置\n\r"
};
const u8 pMenu2_3_7[]=
{
	"现场调试\n\r"
	"  1.485口功能设置\n\r"
	"  2.232口功能设置\n\r"
};
#else
	#if (Project/100)==2//集中器
const u8 pMenu2_3_7[]=
{
	"现场调试\n\r"
	"  1.485口功能设置\n\r"
	"  2.自动搜表\n\r"
};
const u8 pMenu2_3_7_2[]=
{
	"自动搜表\n\r"
	"  1.搜表关闭\n\r"
	"  2.启用并更新F10\n\r"
	"  3.启用不更新F10\n\r"
};
	#else
const u8 pMenu2_3_7[]=
{
	"现场调试\n\r"
	"  1.485口功能设置\n\r"
};
	#endif
#endif

const u8 pMenu2_3_5[]=
{
	"集中器版本\n\r"
	"软件版本:\n\r"
	"硬件版本:\n\r"
};

const u8 pMenu2_3_6_4[]=
{
	"抄表间隔设置\n\r"
	"抄表间隔:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu2_3_6_5[]=
{
	"抄表时段设置\n\r"
	"时段数:\n\r"
};

const u8 pMenu2_3_6[]=
{
	"载波抄表管理\n\r"
	"  1.暂停抄表F49\n\r"
	"  2.恢复抄表F50\n\r"
	"  3.重新抄表F51\n\r"
	"  4.抄表间隔设置\n\r"
	"  5.抄表时段设置\n\r"
};
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_1
// Descriptions:        重启终端
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"终端重启");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=1;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_2
// Descriptions:        数据区初始化
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_2(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,5,0,"数据初始化");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					//数据区初始化
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=2;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,4,0,"数据初始化中");
					DisplayString(5,6,0,"请等待...");
				}
			}
			if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			{
				DisplayString(4,4,0,"数据初始化OK");
				DisplayString(5,4,0,"按返回键退出");
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_3
// Descriptions:        恢复出厂配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_3(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"恢复出厂配置");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					//恢复出厂配置
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=3;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,3,0,"恢复出厂配置中");
					DisplayString(5,6,0,"请等待...");
				}
			}
			if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			{
				DisplayString(4,3,0,"恢复出厂配置OK");
				DisplayString(5,4,0,"按返回键退出");
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_4
// Descriptions:        参数区初始化
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_4(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,5,0,"参数初始化");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					//参数区初始化
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=4;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,4,0,"参数初始化中");
					DisplayString(5,6,0,"请等待...");
				}
			}
			if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			{
				DisplayString(4,4,0,"参数初始化OK");
				DisplayString(5,4,0,"按返回键退出");
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_5
// Descriptions:        变更子菜单密码
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_5(void)
{
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u8 *p8;
	u16 *p16;
	u32 *p32;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_9_5);//显示菜单
			/*显示内存中数据*/
			x=MRR(ADDR_MenuKey,3);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
			DisplayData(x,6,0,p16);
			MenuCount->v1=5;
			MenuCount->v2=10;
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
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
							MenuCount->v1=5;
							MenuCount->v2=10;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+Stack->MenuStack[MenuCount->Count].EditRow))
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8_add1(p8,6);
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8_sub1(p8,6);
								break;
						}
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
								DisplayString(Ln+2,6,0,"   OK   ");
								/*保存设置的数据*/
								MR(ADDR_DATABUFF,ADDR_MenuKey,3);//读菜单密码
								p32=(u32 *)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								p32[0]=(p8[0]-0x30)*16*16*16*16*16+(p8[2]-0x30)*16*16*16*16+(p8[4]-0x30)*16*16*16+(p8[6]-0x30)*16*16+(p8[8]-0x30)*16+(p8[10]-0x30)*1;
								MW(ADDR_DATABUFF,ADDR_MenuKey,3);//写菜单密码
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
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
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_6_1
// Descriptions:        工厂状态下禁止485口功能设置提示
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_6_1(void)
{
	DisplayClr();
	DisplayString(4,1,0,"工厂状态下禁止设置");
	DisplayString(5,3,0,"请按返回键退出");
	Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回键
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_6
// Descriptions:        485口功能设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9_6(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Num485;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

//专变、集中器为2路485， 公变、采集器为3路485
#if ((Project/100)==3 || (Project/100)==2)//专变终端、集中器
	Num485=2;
#endif
#if ((Project/100)==4)//公变终端
	Num485=3;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_9_6);//显示菜单
			if(MenuCount->v3>Num485)
			{//MenuCount->v3记录第n路485
				MenuCount->v3=Num485;
			}
			if(MenuCount->v3<1)
			{
				MenuCount->v3=1;
			}		
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
			bcd_p8(p8,MenuCount->v3,1);
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=5;
			MenuCount->v2=5;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//显示内存中数据
			//第n路485功能（终端抄表or电能表口）//0 终端维护口 非0 终端抄表口
			x=MRR(ADDR_UART_PORT_SET+(4*MenuCount->v3),1);
			if(x!=0)
			{
				x=1;
			}
			DisplayString(Ln+2,5,0,(u8 *)Function_485List[x]);//0 终端维护口 1 终端抄表口
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=3)
					{
						DisplayString(Ln+3,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=5;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+Stack->MenuStack[MenuCount->Count].EditRow))
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,1);
								if(x<Num485)
								{
									p8_add1(p8,1);
								}
								else
								{
									bcd_p8(p8,1,1);
								}
								MenuCount->v3=p8_bcd(p8,1);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								switch(p8[0])
								{
									case 0xE8://终端抄表口
										i=0;
										break;
									case 0xBA://终端维护口
										i=1;
										break;
									default:
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_485List[i]);//0 终端维护口 1 终端抄表口
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,1);
								if(x>1)
								{
									p8_sub1(p8,1);
								}
								else
								{
									bcd_p8(p8,Num485,1);
								}
								MenuCount->v3=p8_bcd(p8,1);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								switch(p8[0])
								{
									case 0xE8://终端抄表口
										i=0;
										break;
									case 0xBA://终端维护口
										i=1;
										break;
									default:
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_485List[i]);//0 终端维护口 1 终端抄表口
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
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 3:
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
//#if ((Project/100)==3 && (USER/100)==5)//专变终端且用户标识:上海版
//								//RS485-1工作于主动模式，只进行抄表
//								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"RS485-1只抄表");
//#else
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
//#endif
								//保存设置的数据
								p=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//读485口功能
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
								switch(p8[0])
								{
									case 0xE8://终端抄表口
										p[0]=1;
										break;
									case 0xBA://终端维护口
										p[0]=0;
										break;
								}
//#if ((Project/100)==3 && (USER/100)==5)//专变终端且用户标识:上海版
//								//RS485-1工作于主动模式，只进行抄表
//								if(MenuCount->v3==2)
//								{//RS485-2
//									if(p[0]==0)//终端维护口
//									{
//										MWR(180,ADDR_TerminalFMTimer+42,2);
//									}
//									MW(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//写485口功能
//								}
//#else
//								MW(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//写485口功能
//#endif
#if ((USER/100)==5)//用户标识:上海版
								if(MenuCount->v3==2)
								{//RS485-2
									if(p[0]==0)//终端维护口
									{
										MWR(180,ADDR_TerminalFMTimer+42,2);
									}
								}
#endif
								MW(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//写485口功能
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,1);
								if(x>Num485 || x<1)
								{
									bcd_p8(p8,1,1);
								}
								MenuCount->v3=p8_bcd(p8,1);
								Stack->MenuStack[MenuCount->Count].Task=0;								
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
#if ((Project/100)==3 || (Project/100)==4)//专变终端、公变终端
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9
// Descriptions:        系统管理菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_9(void)//菜单9
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
//#if ((Project/100)==3 && (USER/100)==5)//上海专变
//			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
//																			"终端管理与维护\n\r"
//																			"  1.重启终端F1\n\r"
//																			"  2.数据区初始化F2\n\r"
//																			"  3.恢复出厂配置F3\n\r"
//																			"  4.参数区初始化F4\n\r"
//																			"  5.变更密码\n\r"
//																			);//显示菜单
//#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_9);//显示菜单
//#endif
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_9_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_9_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_9_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_9_4);//创建下级菜单
						break;
					case 5:
						MenuCreate((u32)Menu_9_5);//创建下级菜单
						break;
					case 6:
//#if (!((Project/100)==3 && (USER/100)==5))//非上海专变
						if(Comm_Ram->Factory!=0x55)
						{//非工厂状态
							MenuCreate((u32)Menu_9_6);//创建下级菜单 485口功能设置
						}
						else//工厂状态
						{
							MenuCreate((u32)Menu_9_6_1);//创建下级菜单 工厂状态下禁止485口功能设置
						}
//#endif
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

#if ((Project/100)==2)//集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_5
// Descriptions:        集中器版本
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_5(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i;
	u32 Row=1;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_5);//显示菜单
	//1.软件版本
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF1+12,4);
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	for(i=0;i<=3;i++)
	{
		p[i*2]=p8[i];
		p[i*2+1]=0x20;
	}
	Row++;
	//2.硬件版本
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(HVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x2030;
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_5
// Descriptions:        抄表时段设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 y;
	u32 z;
	u32 Row=1;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_5);//显示菜单
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=7;
			MenuCount->v2=7;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//端口31允许抄表时段数
			x=MRR(ADDR_AFN04F33+((14+(4*24))*30)+13,1);
			if(x>7)
			{
				x=7;
			}
			MenuCount->v3=x;//记录抄表时段数
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+7*2);
			DisplayData(x,1,0,p16);
			Row++;
			if(x!=0)
			{
				for(i=0;i<x;i++)
				{
					y=MRR(ADDR_AFN04F33+((14+(4*24))*30)+14+4*i,2);//第i+1时段开始时间
					p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+0*2);
					p16[0]=i+1+0x2030;
					p16[1]='.'+0x2000;
					if(y==0xEEEE)
					{//无此数据填-----
						DisplayString(Row,2,0,"-----");	
					}
					else
					{
						DisplayData1(y,4,2,':',p16+2);
					}
					p16[7]='-'+0x2000;	
					z=MRR(ADDR_AFN04F33+((14+(4*24))*30)+14+4*i+2,2);//第i+1时段结束时间
					if(z==0xEEEE)
					{//无此数据填-----
						DisplayString(Row,8,0,"-----");	
					}
					else
					{
						DisplayData1(z,4,2,':',p16+8);
					}
					Row++;
				}
			}
			for(i=0;i<=MenuCount->v3+2;i++)
			{
				StrBuff->Row[i].Row=i;
			}
			DisplayString(Row,6,0,"确认设置");
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				for(i=0;i<=MenuCount->v3+2;i++)
				{
					StrBuff->Row[i].Row=i;
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=(MenuCount->v3+2))
					{
						DisplayString(MenuCount->v3+2,6,0,"确认设置");
					}
					if(Stack->MenuStack[MenuCount->Count].EditRow==1)
					{
						MenuCount->v1=7;
						MenuCount->v2=7;
						ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					}
					else
					{
						for(k=0;k<10;k++)
						{
							if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
								if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
								{
									for(i=2;i<=3;i++)
									{
										p16[i]=(p16[i]&0x8000)+0x2020;
									}
								}
							}
						}
						if(Stack->MenuStack[MenuCount->Count].EditRow==MenuCount->v3+2)
						{
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
						}
						else
						{
							for(i=0;i<MenuCount->v3;i++)
							{
								MenuCount->v1=2;
								MenuCount->v2=12;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							}
						}
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
							x=p8_bcd(p8,1);
							if(x<7)
							{
								p8_add1(p8,1);
							}
							else
							{
								bcd_p8(p8,0,1);
							}
							x=p8_bcd(p8,1);
							if(x>MenuCount->v3)
							{
								DisplayClrRow(MenuCount->v3+2,MenuCount->v3+2);//清屏幕显示的指定行
								y=x-MenuCount->v3;
								for(i=0;i<y;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(MenuCount->v3+2+i)*84+4+0*2);
									p16[0]=MenuCount->v3+1+i+0x2030;
									p16[1]='.'+0x2000;
									p16[2]=0x2030;
									p16[3]=0x2030;
									p16[4]=':'+0x2000;
									p16[5]=0x2030;
									p16[6]=0x2030;
									p16[7]='-'+0x2000;
									p16[8]=0x2030;
									p16[9]=0x2030;
									p16[10]=':'+0x2000;
									p16[11]=0x2030;
									p16[12]=0x2030;
								}
								DisplayString(x+2,6,0,"确认设置");
								MenuCount->v3=x;
							}
							if(x<MenuCount->v3)
							{
								DisplayClrRow(x+2,MenuCount->v3+2);//清屏幕显示的指定行
								DisplayString(x+2,6,0,"确认设置");
								MenuCount->v3=x;
							}
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
							x=p8_bcd(p8,1);
							if(x>0)
							{
								p8_sub1(p8,1);
							}
							else
							{
								bcd_p8(p8,7,1);
							}
							x=p8_bcd(p8,1);
							if(x>MenuCount->v3)
							{
								DisplayClrRow(MenuCount->v3+2,MenuCount->v3+2);//清屏幕显示的指定行
								y=x-MenuCount->v3;
								for(i=0;i<y;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(MenuCount->v3+2+i)*84+4+0*2);
									p16[0]=MenuCount->v3+1+i+0x2030;
									p16[1]='.'+0x2000;
									p16[2]=0x2030;
									p16[3]=0x2030;
									p16[4]=':'+0x2000;
									p16[5]=0x2030;
									p16[6]=0x2030;
									p16[7]='-'+0x2000;
									p16[8]=0x2030;
									p16[9]=0x2030;
									p16[10]=':'+0x2000;
									p16[11]=0x2030;
									p16[12]=0x2030;
								}
								DisplayString(x+2,6,0,"确认设置");
								MenuCount->v3=x;
							}
							if(x<MenuCount->v3)
							{
								DisplayClrRow(x+2,MenuCount->v3+2);//清屏幕显示的指定行
								DisplayString(x+2,6,0,"确认设置");
								MenuCount->v3=x;
							}
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<(MenuCount->v3+2) && Stack->MenuStack[MenuCount->Count].EditRow>1)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
							DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
						}
						else
						{
							//保存设置的数据
							DisplayClrRow_4(MenuCount->v3+2);//清屏幕显示的指定行(4个控制字不清)
							DisplayString(MenuCount->v3+2,9,0,"OK");
							p8=(u8 *)ADDR_DATABUFF;
							MR(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+13,1+4*24);
							p=(u8 *)(ADDR_STRINGBUFF+1*84+4+7*2);
							p8[0]=p[0]-0x30;
							for(i=0;i<MenuCount->v3;i++)
							{
								p=(u8 *)(ADDR_STRINGBUFF+(i+2)*84+4+2*2);
								p8[1+4*i]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//开始时间：分
								p8[2+4*i]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//开始时间：时
								p8[3+4*i]=(p[9*2]-0x30)*16+(p[10*2]-0x30)*1;//结束时间：分
								p8[4+4*i]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//结束时间：时
							}
							MW(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+13,1+4*24);
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
						if(Stack->MenuStack[MenuCount->Count].EditRow<(MenuCount->v3+2))
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=0x2020;
							p16[3]=0x2020;
							if(Stack->MenuStack[MenuCount->Count].EditRow==1)
							{
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+7*2);
								if(p8[0]-0x30>7)
								{
									p8[0]=0x37;
								}
							}
							else
							{
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+2*2);
								for(i=0;i<2;i++)
								{
									if(((p8[(0+i*6)*2]-0x30)*10+(p8[(1+i*6)*2]-0x30)*1>23) || (p8[(0+i*6)*2]=='-') || (p8[(1+i*6)*2]=='-'))
									{
										p8[(0+i*6)*2]=0x30;
										p8[(1+i*6)*2]=0x30;
									}
									if(((p8[(3+i*6)*2]-0x30)*10+(p8[(4+i*6)*2]-0x30)*1>59) || (p8[(3+i*6)*2]=='-') || (p8[(4+i*6)*2]=='-'))
									{
										p8[(3+i*6)*2]=0x30;
										p8[(4+i*6)*2]=0x30;
									}
								}
							}
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
// Function name:       Menu2_3_6_4
// Descriptions:        抄表时间间隔设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_4(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row=1;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_4);//显示菜单
			//显示内存中数据
			//端口31抄表时间间隔
			x=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);
			x=hex_bcd(x);
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
			DisplayData(x,2,0,p16);

			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
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
						DisplayString(2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=10;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_add1(p8,2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_sub1(p8,2);
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
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//(1~60) 判断内存数据如果有错，则初始化
								for(i=0;i<=1;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=2)
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 2:
								DisplayClrRow_4(2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(2,9,0,"OK");
								//保存设置的数据
								MR(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+9,1);//读端口31抄表时间间隔
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
								p8[0]=p8_bcd(p,2);
								MW(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+9,1);//写端口31抄表时间间隔
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,2);
								if(x>60 || x<1)
								{
									bcd_p8(p8,1,2);
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
// Function name:       Menu2_3_6_3
// Descriptions:        重新抄表F51
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_3(void)
{
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"载波重新抄表");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0x55;
					MW(ADDR_DATABUFF,ADDR_AFN05F51+30,1);
					DisplayString(4,3,0,"载波已重新抄表");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_2
// Descriptions:        恢复抄表F50
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_2(void)
{
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"载波恢复抄表");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0;
					MW(ADDR_DATABUFF,ADDR_AFN05F50+30,1);
					DisplayClr();
					DisplayString(4,3,0,"载波已恢复抄表");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_1
// Descriptions:        暂停抄表F49
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_1(void)
{
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"载波暂停抄表");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0x55;
					MW(ADDR_DATABUFF,ADDR_AFN05F49+30,1);
					DisplayClr();
					DisplayString(4,3,0,"载波已暂停抄表");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6
// Descriptions:        载波抄表管理
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu2_3_6_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu2_3_6_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu2_3_6_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu2_3_6_4);//创建下级菜单
						break;
					case 5:
						MenuCreate((u32)Menu2_3_6_5);//创建下级菜单
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
#if ((USER/100)==2)//山东版
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2
// Descriptions:        232口功能设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_7_2);//显示菜单
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=5;
			MenuCount->v2=14;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//显示内存中数据
			//232功能//0=终端监测口,!=0终端维护口
			x=MRR(ADDR_UART_PORT_SET,1);
			if(x!=0)
			{
				x=1;
			}
			DisplayString(1,5,0,(u8 *)Function_232List[x]);//0=终端监测口,!=0终端维护口
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								if(p8[0]==0xBA)
								{//终端维护口
									i=0;//终端监测口
								}
								else
								{//终端监测口
									i=1;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_232List[i]);//0=终端监视口,!=0终端维护口
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								if(p8[0]==0xBA)
								{//终端维护口
									i=0;//终端监测口
								}
								else
								{//终端监测口
									i=1;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_232List[i]);//0=终端监视口,!=0终端维护口
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 2:
								DisplayClrRow_4(Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
								//保存设置的数据
								p=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_UART_PORT_SET,1);//读232口功能
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
								if(p8[0]==0xBA)
								{//终端维护口
									p[0]=1;
								}
								else
								{//终端监测口
									p[0]=0;
									MWR(180,ADDR_TerminalFMTimer+38,2);
								}
								MW(ADDR_DATABUFF,ADDR_UART_PORT_SET,1);//写232口功能
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
#else
	#if (Project/100)==2//集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2_3
// Descriptions:        搜表启用不更新F10
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2_3(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"启用不更新F10");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					MWR(0x55,ADDR_AFN05F149,1);
					MWR(2,ADDR_AFN05F149+1,1);
					MC(0,ADDR_AFN0CF13,2);//搜索到的电表信息
					MWR(0x55555555,ADDR_AFN05F149+2,4);
					DisplayClr();
					DisplayString(4,5,0,"搜表已开启");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2_2
// Descriptions:        搜表启用并更新F10
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2_2(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"启用并更新F10");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					MWR(0x55,ADDR_AFN05F149,1);
					MWR(1,ADDR_AFN05F149+1,1);
					MC(0,ADDR_AFN0CF13,2);//搜索到的电表信息
					MWR(0x55555555,ADDR_AFN05F149+2,4);
					DisplayClr();
					DisplayString(4,5,0,"搜表已开启");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2_1
// Descriptions:        搜表关闭
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2_1(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"搜表关闭");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			break;
		case 1://任务1:选择
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(p16[0]>>15)
				{//NO
					MenuDel();//返回上级菜单
				}
				if(p16[5]>>15)
				{//YES
					MWR(0,ADDR_AFN05F149,6);
					DisplayClr();
					DisplayString(4,5,0,"搜表已关闭");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3C;//键使用后清0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2
// Descriptions:        自动搜表
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_7_2);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu2_3_7_2_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu2_3_7_2_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu2_3_7_2_3);//创建下级菜单
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
#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7
// Descriptions:        现场调试
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_7);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						if(Comm_Ram->Factory!=0x55)
						{//非工厂状态
							MenuCreate((u32)Menu_9_6);//创建下级菜单 485口功能设置
						}
						else//工厂状态
						{
							MenuCreate((u32)Menu_9_6_1);//创建下级菜单 工厂状态下禁止485口功能设置
						}
						break;
					case 2:
#if ((USER/100)==2)//山东版
							MenuCreate((u32)Menu2_3_7_2);//创建下级菜单 232口功能设置
#else
	#if (Project/100)==2//集中器
							MenuCreate((u32)Menu2_3_7_2);//创建下级菜单 232口功能设置
	#endif
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

#if ((USER/100)==5)//上海集中器
u32 Check_AFN04F10_MeterNo(u32 MeterNo)//检查终端电能表/交流采样装置配置参数指定的通信地址后6位;返回值为相同后6位通信地址的测量点总数
{
	u32 i;
	u32 Meter;
	u32 Num;
	u8 *p8;

	Num=0;
	p8=(u8*)(ADDR_AFN04F10+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[4]+(p8[5]<<8)+(p8[6]<<16);
		if(i==MeterNo)
		{
			if((p8[0]+(p8[1]<<8))!=0)
			{//测量点号不为0
				Num++;
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return Num;
}
u32 Check_AFN04F10_MeterNo1(u32 Pn,u32 MeterNo)//从测量点Pn号开始检查终端电能表/交流采样装置配置参数指定的通信地址后6位;返回值!=0为相同后6位通信地址的测量点号,0=没有
{
	u32 i;
	u32 Meter;
	u8 *p8;

	p8=(u8*)(ADDR_AFN04F10+2+LEN_AFN04F10_Pn*(Pn-1));
	for(Meter=Pn;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[4]+(p8[5]<<8)+(p8[6]<<16);
		if(i==MeterNo)
		{
			if((p8[0]+(p8[1]<<8))!=0)
			{//测量点号不为0
				return (p8[0]+(p8[1]<<8));
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------
// Function name:       Menu2_SH_9_9_2
// Descriptions:        上海集中器菜单9_9_2 手动抄表2
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_SH_9_9_2(void)
{
/*
	集中器具备手动抄表功能，可通过表号或测量点号进行手动抄表。
	通过按键输入表号后六位查询并列出集中器档案中对应的测量点信息，
	移动光标对选定测量点进行手动抄表；
	通过按键输入测量点号进行手动抄表。
	针对两种抄表方式，集中器返回对应测量点当前正向有功总电能示值（一类数据F129）。
*/
	u32 i;
	u32 j;
	u64 x;
	u64 y;
	u64 MeterNo;
//	u32 cs;
	u8 *p8;
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
																			"手动抄表\n\r"
																			"表址后6位\n\r"
																			"   查询测量点信息\n\r"
																			);//显示菜单
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+9*2);
			DisplayData(0x000001,6,0,p16);
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
						DisplayString(2,3,0,"查询测量点信息");
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
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_add1(p8,6);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_sub1(p8,6);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
						}
						if(Stack->MenuStack[MenuCount->Count].EditRow==2)
						{
							Stack->MenuStack[MenuCount->Count].Task=2;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								for(i=0;i<6;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=6)
								{
									bcd_p8(p8,1,6);
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
			p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
			MeterNo=0;
			y=1;
			for(i=0;i<6;i++)
			{
				MeterNo+=(p8[(6-1-i)*2]-0x30)*y;
				y*=16;
			}

			MenuCount->v3=Check_AFN04F10_MeterNo(MeterNo);//检查终端电能表/交流采样装置配置参数指定的通信地址后6位;返回值为相同后6位通信地址的测量点总数
			DisplayClr();//清全屏
			if(MenuCount->v3!=0)
			{
				Stack->MenuStack[MenuCount->Count].Task=3;
				Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
				Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
				Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"手动抄表\n\r"
																				"      第    个\n\r"
																				"      确认发送\n\r"
																				);//显示菜单字符串
				p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+8*2);
				DisplayData(0x0001,4,0,p16);
				//测量点号
				DisplayString(3,0,0,(u8 *)"测量点号");
				x=Check_AFN04F10_MeterNo1(1,MeterNo);//从测量点Pn号开始检查终端电能表/交流采样装置配置参数指定的通信地址后6位;返回值!=0为相同后6位通信地址的测量点号,0=没有
				p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
				bcd_p8(p8,x,4);
				//通信地址
				DisplayString(4,0,0,(u8 *)"通信地址");
				p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(x-1)+6,6);
				DisplayData(x,12,0,p16);

				MenuCount->v1=8;
				MenuCount->v2=11;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			}
			else
			{
				DisplayMenuString(4,0,2,(u8 *)
												"      查无此表\n\r"
												"   请按任意键退出\n\r"
												);//显示菜单字符串
			}
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{//按任意键返回上级菜单
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 3:
			p8=(u8 *)(ADDR_STRINGBUFF+4*84+4+14*2);
			MeterNo=0;
			y=1;
			for(i=0;i<6;i++)
			{
				MeterNo+=(p8[(6-1-i)*2]-0x30)*y;
				y*=16;
			}

			for(j=0;j<2;j++)
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
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//右键
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
							x=p8_bcd(p8,4);
							if(x<MenuCount->v3)
							{
								p8_add1(p8,4);
							}
							else
							{
								bcd_p8(p8,1,4);
							}
							break;
						default:
							break;
					}
					//测量点号
					p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
					x=p8_bcd(p8,4);
					p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
					if(x!=1)
					{
						y=p8_bcd(p8,4);
						y+=1;
					}
					else
					{
						y=1;
					}
					x=Check_AFN04F10_MeterNo1(y,MeterNo);//从测量点Pn号开始检查终端电能表/交流采样装置配置参数指定的通信地址后6位;返回值!=0为相同后6位通信地址的测量点号,0=没有
					bcd_p8(p8,x,4);
					//通信地址
					p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+8*2);
					x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(x-1)+6,6);
					DisplayData(x,12,0,p16);
				}
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					//手动抄表 AFN0CF129电能示值
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
						if(UARTCtrl->Lock==0x55)
						{
							return;//0x55=通信功能正在被调用禁止其他程序再次调用
						}
						if(UARTCtrl->Task==0)
						{
/*
终端接收AFN0CF129的帧格式
68 
32 00 //L
32 00 //L
68 
7B //C
34 12 01 00 //终端地址、行政区域码 
02 
0C //AFN
61 //SEQ
01 01 01 10 //数据单元标识
44 //1字节 校验和CS
16 
*/
							p8=(u8 *)ADDR_DATABUFF;
							p8[0]=0x68;
							p8[1]=0x32;//L
							p8[2]=0x00;
							p8[3]=0x32;//L
							p8[4]=0x00;
							p8[5]=0x68;
							p8[6]=0x7B;//C
							MR(((u32)ADDR_DATABUFF)+7,ADDR_AREACODE,4);//行政码,终端地址
							p8[11]=0x02;
							p8[12]=0x0C;//AFN									 
							p8[13]=0x61;//SEQ
							//数据标识
							p8[14]=0;
							p8[15]=0;									 
							i=p8_bcd((u8 *)ADDR_STRINGBUFF+3*84+4+8*2,4);
							MenuCount->v3=i;//测量点号
							if(i!=0x0)
							{
								i-=1;
								p8[14]|=(1<<(i%8));
								p8[15]=((i/8)+1);
							}
							p8[16]=0x01;
							p8[17]=0x10;
//							//校验和CS计算
//							cs=0;
//							for(i=0;i<12;i++)
//							{
//								cs+=p8[6+i];
//							}
//							cs&=0xff;
//							p8[18]=cs;
//							p8[19]=0x16;

							p8[1]=0x0C;//去标志位D1D0,同时长度最大只有0-16383
//							MW(ADDR_DATABUFF,(u32)p8rxbuff,20);									 
							MW(ADDR_DATABUFF,(u32)p8rxbuff,18);									 

							UARTCtrl->Task=1;
							Stack->MenuStack[MenuCount->Count].Task=4;
						}
					}
					Comm_Ram->ButtonVal&=~0x1B;//键使用后清0
				}
				else
				{
					Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
					break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		case 4:
//从发送缓冲区读取数据
			if(UARTCtrl->Task==0)
			{
				DisplayClr();//清全屏
				DisplayMenuString(2,0,2,(u8 *)
												"正向有功电能(kWh):\n\r"
												"  总 = ----\n\r"
												"  峰 = ----\n\r"
												"  平 = ----\n\r"
												"  尖 = ----\n\r"
												"  谷 = ----\n\r"
												);//显示菜单字符串
/*
68 
B6 00 
B6 00 
68 
A8 
01 22 01 00 
02 
0C 
66 
01 01 01 10 //14数据标识
48 03 10 01 14 //18抄表时间
04 //费率数
00 00 00 00 00 //24总
00 00 00 00 00 //29费率1 
00 00 00 00 00 //34费率2 
00 00 00 00 00 //39费率3 
00 00 00 00 00 //44费率4 
29 2C //EC
1C 
16
*/
				//正向有功电能示值
				//比较AFN、SEQ、数据标识
				for(i=0;i<6;i++)
				{
					if(p8rxbuff[12+i]!=p8txbuff[12+i])
					{
						break;
					}
				}
				if(i==6)
				{//AFN、SEQ、数据标识接收和发送的一致
					p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v3-1)*LEN_AFN04F10_Pn)+5);
					if((p8[0]==21) && (p8[13]!=4))
					{//上海规约、费率2
			/*
				针对上海表，集中器档案中费率数为2时，集中器费率1对应上海表费率平，集中器费率2对应上海表费率谷；
				集中器档案中费率数为4时，集中器费率1～4分别对应无效数据、上海表费率平、无效数据、上海表费率谷。
				此费率顺序对一类和二类数据均有要求。
			*/			
						//总
						y=MRR((u32)p8txbuff+24,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//峰

						//平
						y=MRR((u32)p8txbuff+24+5,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+2)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//尖

						//谷
						y=MRR((u32)p8txbuff+24+10,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+4)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
					}
					else
					{
						x=p8[13];
						if(x>4)
						{//费率数＞4
							x=4;
						}
						for(j=0;j<(x+1);j++)
						{
							y=MRR((u32)p8txbuff+24+5*j,5);	                   
							if(y!=0xEEEEEEEEEE)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(3+j)*84+6*2);
								DisplayData_OffZero(y,10,4,p16);
							}
						}
					}
				}
			}
			else
			{
				DisplayClr();//清全屏
				DisplayMenuString(4,0,2,(u8 *)
												"      正在抄表\n\r"
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
//------------------------------------------------------------------------------------------------
// Function name:       Menu2_SH_9_9_1
// Descriptions:        上海集中器菜单9_9_1 手动抄表1
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_SH_9_9_1(void)
{
/*
	集中器具备手动抄表功能，可通过表号或测量点号进行手动抄表。
	通过按键输入表号后六位查询并列出集中器档案中对应的测量点信息，
	移动光标对选定测量点进行手动抄表；
	通过按键输入测量点号进行手动抄表。
	针对两种抄表方式，集中器返回对应测量点当前正向有功总电能示值（一类数据F129）。
*/
	u32 i;
	u32 j;
	u32 x;
	u64 y;
//	u32 cs;
	u8 *p8;
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
																			"手动抄表\n\r"
																			"测量点号\n\r"
																			"      确认发送\n\r"
																			);//显示菜单
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+8*2);
			DisplayData(0x0001,4,0,p16);
			MenuCount->v1=8;
			MenuCount->v2=11;
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
						DisplayString(2,6,0,"确认发送");
					}
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
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								y=p8_bcd(p8,4);
								if(y<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
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
								y=p8_bcd(p8,4);
								if(y>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						//手动抄表 AFN0CF129电能示值
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
							if(UARTCtrl->Lock==0x55)
							{
								return;//0x55=通信功能正在被调用禁止其他程序再次调用
							}
							if(UARTCtrl->Task==0)
							{
/*
终端接收AFN0CF129的帧格式
68 
32 00 //L
32 00 //L
68 
7B //C
34 12 01 00 //终端地址、行政区域码 
02 
0C //AFN
61 //SEQ
01 01 01 10 //数据单元标识
44 //1字节 校验和CS
16 
*/
								p8=(u8 *)ADDR_DATABUFF;
								p8[0]=0x68;
								p8[1]=0x32;//L
								p8[2]=0x00;
								p8[3]=0x32;//L
								p8[4]=0x00;
								p8[5]=0x68;
								p8[6]=0x7B;//C
								MR(((u32)ADDR_DATABUFF)+7,ADDR_AREACODE,4);//行政码,终端地址
								p8[11]=0x02;
								p8[12]=0x0C;//AFN									 
								p8[13]=0x61;//SEQ
								//数据标识
								p8[14]=0;
								p8[15]=0;									 
								i=p8_bcd((u8 *)ADDR_STRINGBUFF+1*84+4+8*2,4);
								MenuCount->v3=i;//测量点号
								if(i!=0x0)
								{
									i-=1;
									p8[14]|=(1<<(i%8));
									p8[15]=((i/8)+1);
								}
								p8[16]=0x01;
								p8[17]=0x10;
//								//校验和CS计算
//								cs=0;
//								for(i=0;i<12;i++)
//								{
//									cs+=p8[6+i];
//								}
//								cs&=0xff;
//								p8[18]=cs;
//								p8[19]=0x16;

								p8[1]=0x0C;//去标志位D1D0,同时长度最大只有0-16383
//								MW(ADDR_DATABUFF,(u32)p8rxbuff,20);									 
								MW(ADDR_DATABUFF,(u32)p8rxbuff,18);									 

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
								for(i=0;i<4;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,1,4);
								}
								y=p8_bcd(p8,4);
								if(y>MaxRS485AddCarrierPn || y<1)
								{
									bcd_p8(p8,1,4);
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
				DisplayMenuString(2,0,2,(u8 *)
												"正向有功电能(kWh):\n\r"
												"  总 = ----\n\r"
												"  峰 = ----\n\r"
												"  平 = ----\n\r"
												"  尖 = ----\n\r"
												"  谷 = ----\n\r"
												);//显示菜单字符串
/*
68 
B6 00 
B6 00 
68 
A8 
01 22 01 00 
02 
0C 
66 
01 01 01 10 //14数据标识
48 03 10 01 14 //18抄表时间
04 //费率数
00 00 00 00 00 //24总
00 00 00 00 00 //29费率1 
00 00 00 00 00 //34费率2 
00 00 00 00 00 //39费率3 
00 00 00 00 00 //44费率4 
29 2C //EC
1C 
16
*/
				//正向有功电能示值
				//比较AFN、SEQ、数据标识
				for(i=0;i<6;i++)
				{
					if(p8rxbuff[12+i]!=p8txbuff[12+i])
					{
						break;
					}
				}
				if(i==6)
				{//AFN、SEQ、数据标识接收和发送的一致
					p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v3-1)*LEN_AFN04F10_Pn)+5);
					if((p8[0]==21) && (p8[13]!=4))
					{//上海规约、费率2
			/*
				针对上海表，集中器档案中费率数为2时，集中器费率1对应上海表费率平，集中器费率2对应上海表费率谷；
				集中器档案中费率数为4时，集中器费率1～4分别对应无效数据、上海表费率平、无效数据、上海表费率谷。
				此费率顺序对一类和二类数据均有要求。
			*/			
						//总
						y=MRR((u32)p8txbuff+24,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//峰

						//平
						y=MRR((u32)p8txbuff+24+5,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+2)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//尖

						//谷
						y=MRR((u32)p8txbuff+24+10,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+4)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
					}
					else
					{
						x=p8[13];
						if(x>4)
						{//费率数＞4
							x=4;
						}
						for(j=0;j<(x+1);j++)
						{
							y=MRR((u32)p8txbuff+24+5*j,5);	                   
							if(y!=0xEEEEEEEEEE)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(3+j)*84+6*2);
								DisplayData_OffZero(y,10,4,p16);
							}
						}
					}
				}
			}
			else
			{
				DisplayClr();//清全屏
				DisplayMenuString(4,0,2,(u8 *)
												"      正在抄表\n\r"
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
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_SH_9_9
// Descriptions:        上海集中器菜单9_9 手动抄表
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_SH_9_9(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"手动抄表\n\r"
																		"  1.测量点方式\n\r"
																		"  2.表地址方式\n\r"
																		);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu2_SH_9_9_1);
						break;
					case 2:
						MenuCreate((u32)Menu2_SH_9_9_2);
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
#if ((USER/100)==8)//通用集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_9_9
// Descriptions:        辽宁锦州要求显示交采通信地址
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_9_9(void)
{
	u16 *p16;
	u32 Row=1;
	u64 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																"交采通信地址\n\r"
																);//显示菜单
	//交采通信地址
	x=MRR(ADDR_04000401,6);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+0*2);
	DisplayData(x,12,0,p16);
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3
// Descriptions:        集中器终端管理与维护
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_9_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_9_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_9_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_9_4);//创建下级菜单
						break;
					case 5:
						MenuCreate((u32)Menu2_3_5);//创建下级菜单
						break;
					case 6:
						MenuCreate((u32)Menu2_3_6);//创建下级菜单
						break;
					case 7:
						MenuCreate((u32)Menu2_3_7);//现场调试
						break;
					case 8:
						MenuCreate((u32)Menu_8_2_5);//设备编号设置
						break;
#if ((USER/100)==5)//上海集中器
					case 9://手动抄表
						MenuCreate((u32)Menu2_SH_9_9);//创建下级菜单	上海集中器 手动抄表
						break;
#endif
#if ((USER/100)==8)//通用集中器
					case 9://显示交采通信地址
						MenuCreate((u32)Menu2_9_9);//创建下级菜单	辽宁锦州要求显示交采通信地址
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
}
#endif

