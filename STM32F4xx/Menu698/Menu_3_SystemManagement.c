
//菜单3 终端管理与维护
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
//#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"

#include "../MS/msFILE.h"
#include "../MS/MS.h"


extern void Init_ALLDATA(void);//终端全部数据初始化(清0)
const u8 pMenu2_3_Project2[]=
{
	"终端管理与维护\n\r"
	"  1.集中器版本\n\r"
	"  2.现场调试\n\r"
	"  3.集中器重启\n\r"
	"  4.数据初始化\n\r"
	"  5.参数初始化\n\r"
	"  6.集中器数据\n\r"
	"  7.载波抄表管理\n\r"
//	"  8.手动抄表\n\r"
};
const u8 pMenu2_3_Project3[]=
{
	"终端管理与维护\n\r"
	"  1.现场调试\n\r"// -----2
	"  2.终端重启\n\r"// -----3
	"  3.数据初始化\n\r"// -----4
	"  4.参数初始化\n\r"// -----5
	"  5.终端数据\n\r"// -----6
};

const u8 pMenu2_3_1[]=
{
	"版本信息\n\r"
	"软件版本:\n\r"
	"日期:\n\r"
	"硬件版本:\n\r"
	"日期:\n\r"
};
const u8 pMenu2_3_2[]=
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


const u8 pMenu2_3_6_Project2[]=
{
	"集中器数据\n\r"
	"采集档案个数:\n\r"
	"任务配置个数:\n\r"
	"普通采集方案个数:\n\r"
	"事件采集方案个数:\n\r"
	"上报方案个数:\n\r"
};

const u8 pMenu2_3_6_Project3[]=
{
	"终端数据\n\r"
	"采集档案个数:\n\r"
	"任务配置个数:\n\r"
	"普通采集方案个数:\n\r"
	"事件采集方案个数:\n\r"
	"上报方案个数:\n\r"
};


const u8 pMenu2_3_7[]=
{
	"载波抄表管理\n\r"
	"  1.暂停抄表\n\r"
	"  2.恢复抄表\n\r"
	"  3.重新抄表\n\r"
//	"  4.抄表间隔设置\n\r"
//	"  5.抄表时段设置\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_1
// Descriptions:        集中器版本
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_1(void)
{
	u16 *p16;
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1);//显示菜单
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
// Function name:       Menu2_3_2
// Descriptions:        现场调试
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_2(void)
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
#elif((Project/100)==2 && (USER/100)==17)//江苏集中器
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_2);//显示菜单
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_3
// Descriptions:        重启终端
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_3(void)
{
//	u8 *p8;
	u16 *p16;
//	u32 i;

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
					WWDT_RESET();
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
// Function name:       Menu2_3_4
// Descriptions:        数据区初始化
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_4(void)
{
//	u8 *p8;
	u16 *p16;
	u32 *p32;

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
					//p8=(u8 *)ADDR_DATABUFF;
					//for(i=0;i<=3;i++)
					//{
					//	p8[i]=0x55;
					//}
					//p8[4]=2;
					
					//MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,4,0,"数据初始化中");
					DisplayString(5,6,0,"请等待...");
					//Init_ALLDATA();
					p32=(u32*)ADDR_Terminal_ENTRY;
					(*(void(*)())p32[116])();//终端全部数据初始化(清0)
					
					DisplayString(4,4,0,"数据初始化OK");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			//if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			//{
				//DisplayString(4,4,0,"数据初始化OK");
				//DisplayString(5,4,0,"按返回键退出");
			//}
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
// Function name:       Menu2_3_5
// Descriptions:        参数初始化
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_5(void)
{
//	u8 *p8;
	u16 *p16;
	u32 *p32;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"参数初始化");
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
					//p8=(u8 *)ADDR_DATABUFF;
					//for(i=0;i<=3;i++)
					//{
					//	p8[i]=0x55;
					//}
					//p8[4]=3;
					//MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					
					
					DisplayString(4,3,0,"恢复出厂配置中");
					DisplayString(5,6,0,"请等待...");
					//DL698_SETINIT(3,0);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
					p32=(u32*)ADDR_Terminal_ENTRY;
					(*(void(*)())p32[115])((u32)3,(u8*)0);
					
					DisplayString(4,3,0,"恢复出厂配置OK");
					DisplayString(5,4,0,"按返回键退出");
				}
			}
			//if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			//{
				//DisplayString(4,3,0,"恢复出厂配置OK");
				//DisplayString(5,4,0,"按返回键退出");
			//}
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
// Descriptions:        集中器数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_6(void)
{
	u8 *p;
	u32 num;
	u32 index;
	u32 i;
	u32 Ln;
#if((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
#if ((Project/100)==2)//集中器
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project2);//显示菜单
#endif
#if ((Project/100)==3)//专变终端
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project3);//显示菜单
#endif
#if ((Project/100)==4)//3in1
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project2);//显示菜单
	}
	else
	{
		DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project3);//显示菜单
	}
#endif
	//采集档案个数
	num=0;
	for(index=1;index<NUM_RMmax;index++){
		i=MRR(ADDR_6000_SDRAM+index*LENper_6000,2);
		if(i==0x0402)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//返回
			break;
		}
	}
	p=(u8*)(ADDR_STRINGBUFF+(1+Ln)*84+4+13*2);
	bcd_p8(p,num,4);


	//任务配置个数
	num=0;
	for(index=1;index<NUMmax_6012_2;index++){
		i=MRR(ADDR_6012_2_SDRAM+index*LENper_6012_2,2);
		if(i==0x0C02)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//返回
			break;
		}
		
	}
	p=(u8*)(ADDR_STRINGBUFF+(2+Ln)*84+4+13*2);
	bcd_p8(p,num,3);
	
	//普通采集方案个数
	num=0;
	for(index=0;index<NUMmax_6014;index++){
		i=MRR(ADDR_6014_SDRAM+index*LENper_6014,2);
		if(i==0x0602)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//返回
			break;
		}
	}
	p=(u8*)(ADDR_STRINGBUFF+(3+Ln)*84+4+17*2);
	bcd_p8(p,num,3);	

	//事件采集方案个数
	num=0;
	for(index=0;index<NUMmax_6016;index++){
		i=MRR(ADDR_6016_SDRAM+index*LENper_6016,2);
		if(i==0x0502)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//返回
			break;
		}
	}
	p=(u8*)(ADDR_STRINGBUFF+(4+Ln)*84+4+17*2);
	bcd_p8(p,num,3);	

	//上报方案个数			
	num=0;
	for(index=0;index<NUMmax_601C;index++){
		i=MRR(ADDR_601C_SDRAM+index*LENper_601C,2);
		if(i==0x0502)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//返回
			break;
		}
		
	}
	p=(u8*)(ADDR_STRINGBUFF+(5+Ln)*84+4+13*2);
	bcd_p8(p,num,3);	

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_3
// Descriptions:        重新抄表
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_3(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"重新抄表");
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
				//if(p16[0]>>15)
				//{//NO
					MenuDel();//返回上级菜单
				//}
				//if(p16[5]>>15)
				//{//YES
				//	WWDT_RESET();
				//}
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
// Descriptions:        恢复抄表
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"恢复抄表");
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
				//if(p16[0]>>15)
				//{//NO
					MenuDel();//返回上级菜单
				//}
				//if(p16[5]>>15)
				//{//YES
				//	WWDT_RESET();
				//}
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
// Function name:       Menu2_3_7_1
// Descriptions:        暂停抄表
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_1(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"暂停抄表");
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
				//if(p16[0]>>15)
				//{//NO
					MenuDel();//返回上级菜单
				//}
				//if(p16[5]>>15)
				//{//YES
				//	WWDT_RESET();
				//}
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
// Function name:       Menu2_3_7
// Descriptions:        载波抄表管理
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
					case 1://.暂停抄表
						MenuCreate((u32)Menu2_3_7_1);//创建下级菜单
						break;
					case 2://.恢复抄表
						MenuCreate((u32)Menu2_3_7_2);//创建下级菜单
						break;
					case 3://.重新抄表
						MenuCreate((u32)Menu2_3_7_3);//创建下级菜单
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
		#if ((Project/100)==2)//集中器
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project2);//显示菜单
		#endif
		#if ((Project/100)==3)//专变
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project3);//显示菜单
		#endif
		#if ((Project/100)==4)//三合一
			if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
			{
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project2);//显示菜单
			}
			else
			{
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project3);//显示菜单
			}
		#endif
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				#if ((Project/100)==2)//集中器
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. 集中器版本
						MenuCreate((u32)Menu2_3_1);//创建下级菜单
						break;
					case 2://2. 现场调试
						MenuCreate((u32)Menu2_3_2);//创建下级菜单
						break;
					case 3://3. 重启终端
						MenuCreate((u32)Menu2_3_3);//创建下级菜单
						break;
					case 4://4. 数据区初始化
						MenuCreate((u32)Menu2_3_4);//创建下级菜单
						break;
					case 5://5参数初始化
						MenuCreate((u32)Menu2_3_5);//创建下级菜单
						break;
					case 6://6集中器数据
						MenuCreate((u32)Menu2_3_6);//创建下级菜单
						break;
					case 7://7载波抄表管理
						MenuCreate((u32)Menu2_3_7);//创建下级菜单
						break;
					default:
						break;
				}    
			#endif
			#if ((Project/100)==3)//专变
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://2. 现场调试
						MenuCreate((u32)Menu2_3_2);//创建下级菜单
						break;
					case 2://3. 重启终端
						MenuCreate((u32)Menu2_3_3);//创建下级菜单
						break;
					case 3://4. 数据区初始化
						MenuCreate((u32)Menu2_3_4);//创建下级菜单
						break;
					case 4://5参数初始化
						MenuCreate((u32)Menu2_3_5);//创建下级菜单
						break;
					case 5://6集中器数据
						MenuCreate((u32)Menu2_3_6);//创建下级菜单
						break;
					default:
						break;
				}    
			#endif
			#if ((Project/100)==4)
				if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
				{
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1://1. 集中器版本
							MenuCreate((u32)Menu2_3_1);//创建下级菜单
							break;
						case 2://2. 现场调试
							MenuCreate((u32)Menu2_3_2);//创建下级菜单
							break;
						case 3://3. 重启终端
							MenuCreate((u32)Menu2_3_3);//创建下级菜单
							break;
						case 4://4. 数据区初始化
							MenuCreate((u32)Menu2_3_4);//创建下级菜单
							break;
						case 5://5参数初始化
							MenuCreate((u32)Menu2_3_5);//创建下级菜单
							break;
						case 6://6集中器数据
							MenuCreate((u32)Menu2_3_6);//创建下级菜单
							break;
						case 7://7载波抄表管理
							MenuCreate((u32)Menu2_3_7);//创建下级菜单
							break;
						default:
							break;
					}    
				}
				else
				{
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1://2. 现场调试
							MenuCreate((u32)Menu2_3_2);//创建下级菜单
							break;
						case 2://3. 重启终端
							MenuCreate((u32)Menu2_3_3);//创建下级菜单
							break;
						case 3://4. 数据区初始化
							MenuCreate((u32)Menu2_3_4);//创建下级菜单
							break;
						case 4://5参数初始化
							MenuCreate((u32)Menu2_3_5);//创建下级菜单
							break;
						case 5://6集中器数据
							MenuCreate((u32)Menu2_3_6);//创建下级菜单
							break;
						default:
							break;
					}    
				}
			#endif
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
const u8 pMenu2_3_JS[]=//集中器终端管理与维护
{
	"终端管理与维护\n\r"
	"    1.终端数据\n\r"
	"    2.终端信息\n\r"
	"    3.终端管理\n\r"
};
const u8 pMenu2_3_1_JS[]=//集中器数据
{
	"终端数据\n\r"
	"     1.遥信状态\n\r"
	"     2.事件信息\n\r"
};
const u8 pMenu2_3_1_1_JS[]=//集中器遥信状态
{
	"遥信状态\n\r"
	"编号:\n\r"
	"\n\r"
	"状态:\n\r"
	"\n\r"
	"变位:\n\r"
	"\n\r"
	"属性:\n\r"
};
const u8 pMenu2_3_1_2_JS[]=//集中器事件信息
{
	"事件信息\n\r"
	"     1.终端事件\n\r"
	"     2.表计事件\n\r"
};
const u8 pMenu2_3_1_2_1_JS[]=//终端事件信息
{
	"事件信息\n\r"
	"类型:\n\r"
	"\n\r"
	"发生:\n\r"
	"\n\r"
	"结束:\n\r"
};
const u8 pMenu2_3_1_2_2_JS[]=//表计事件信息
{
	"表计事件信息\n\r"
	"测量点:\n\r"
	"\n\r"
	"发生:\n\r"
	"结束:\n\r"
	"地址:\n\r"
	
};
const u8 pMenu2_3_2_JS[]=//终端信息
{
	"本地通讯模块信息\n\r"
	"HPLC模块\n\r"
	"版本\n\r"
	"厂商代码:\n\r"
};

const u8 pMenu2_3_3_JS[]=//终端管理
{
	"终端管理\n\r"
	"  1.现场调试\n\r"
	"  2.集中器重启\n\r"
	"  3.数据初始化\n\r"
	"  4.参数初始化\n\r"
	"  5.集中器数据\n\r"
	"  6.载波抄表管理\n\r"
	"  7.USB功能\n\r"
};
typedef struct
{
	u8 *pEventName;
	u32 EventOAD;
}Lcd_EventInfo_t;
const Lcd_EventInfo_t Lcd_EventInfo[]=
{
	{"终端对时    ",0x31140200},
	{"终端初始化  ",0x31000200},
	{"版本变更    ",0x31010200},
	{"状态量变位  ",0x31040200},
	{"停上电      ",0x31060200},
	{"认证错误    ",0x31090200},
	{"设备故障    ",0x310A0200},
	{"终端编程    ",0x31180200},
	{"电表数据变更",0x311C0200},
	{"电表示度下降",0x311B0200},
	{"电能量超差  ",0x310C0200},
	{"电能表飞走  ",0x310D0200},
	{"电能表停走  ",0x310E0200},
	{"时间超差    ",0x31050200},
	{"485抄表失败 ",0x310F0200},
};

u32 DEPTH_EventFile(u32 OI)//事件文件存储深度;入口:OI;返回:存储深度
{
	switch(OI)
	{
		case 0x3111://发现未知电能表事件
		case 0x3112://跨台区电能表事件
#if ((2*NUM_RMmax)*LEN_EventFileHead)>LEN_128KDATABUFF
	#error
#endif
			return NUM_RMmax;//最大抄表数
		default:
			return NUMmax_Event;//最大事件记录数
	}
}
u32 GET_EventFileName(u32 OAD,u32 FileCount)//搜索事件记录文件名:入口:OAD,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
{
	u32 FileID;
	msFILE_TypeDef *file;
	u32 SearchCount;
	
	if(OAD!=0xffffffff)
	{
		OAD&=0xffffff00;
		OAD|=2;//文件分类
	}
	SearchCount=Comm_Ram->msFILESearchCount;
	if(SearchCount<recordFILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(SearchCount*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((SearchCount-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	for(;SearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);SearchCount++)
	{
		if(SearchCount==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//文件分类
			if((FileID&0xff)!=2)
			{
				file++;
				continue;
			}
			//文件ID
			if(OAD!=0xffffffff)
			{
				if(FileID!=OAD)
				{
					file++;
					continue;
				}
			}
			//文件计数
			if(FileCount!=0xffffffff)//文件计数
			{
				if(FileCount!=file->FILECOUNT)
				{
					file++;
					continue;
				}
			}
			Comm_Ram->msFILESearchCount=SearchCount+1;//文件搜索计数
			if(SearchCount>=recordFILENAMEmax_FLASH)
			{
				SearchCount-=recordFILENAMEmax_FLASH;
				SearchCount+=FILENAMEmax_FLASH;
			}
			return SearchCount;
		}
		file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//文件搜索计数
	return 0xffffffff;
}
u32 Get_RecordFileName(u32 FileID,u32 FileCount,u8* pTime);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到

u32 Lcd_Get_EventData(u16 *pAddr,u32 OAD,u8 *pOut,u32 Type)//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表,LENMAX_NUM缓存中字节的最大长度,return取出数据的长度
{
	//u8 *p8;
	u32 i;
	//u32 n;
	u32 NUM;
	u32 NUMmax_DATA;
	//u32 offset;
	ms_Type *ms;
	u32 filename;
	u32 result=0;
	u32 Len;
	msFILE_TypeDef *file;
	EventFileHead_TypeDef* EventFileHead;
	//RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头文件
	switch (Type)
	{
		case 0://终端事件		
			NUMmax_DATA=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
			ms=Get_ms();
			Comm_Ram->msFILESearchCount=0;//文件搜索计数
			filename=GET_EventFileName(OAD&0xffffff00,0);//搜索事件记录文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename==0xffffffff)
			{//没找到
				return result;
			}
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			i=(OAD&0xff);
			NUM=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
			if(i>NUM)
			{//
				return result;
			}
			NUM=file->NUM_DATA;//事件个数
			if(i>NUM)
			{//在文件计数1文件中
				i-=NUM;
				Comm_Ram->msFILESearchCount=0;//文件搜索计数
				filename=GET_EventFileName(OAD&0xffffff00,1);//搜索事件记录文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
				if(filename==0xffffffff)
				{//没找到
					return result;
				}
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				NUM=file->NUM_DATA;//事件个数
				if(i>NUM)
				{//文件不在计数1文件中
					return result;
				}
			}
			NUM-=i;
			Len=(NUM+1)*sizeof(EventFileHead_TypeDef);
			if(Len>LEN_128KDATABUFF)
			{
				return result;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,Len);
			EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF);
			i=0;
			for(Len=0;Len<NUM;Len++)
			{
				i+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
				EventFileHead++;
			}
			Len=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);			
			ms->msfile_read(filename,((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+i,pOut,Len);
			return Len;
			//break;
		case 1://电表事件
		/*
			if(pAddr==0)
			{
				return result;
			}
			for(NUM=0;NUM<127;NUM++)
			{
				ms=(ms_Type *)Get_ms();
				Comm_Ram->msFILESearchCount=0;
				filename=Get_RecordFileName(0xffffffff,NUM,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
				if(filename==0xffffffff)
				{
					continue;
				}
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				if(file->BLOCK_ENTRY==0)
				{
					return result;
				}
				
				n=file->NUM_DATA;
				if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
				{
					n=0;
				}
				ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
				offset=0;
				RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
				for(i=0;i<n;i++)
				{
					if(RecordFileHead->Addr0==pAddr[0])
					{
						if(RecordFileHead->Addr1==pAddr[1])
						{
							if(RecordFileHead->Addr2==pAddr[2])
							{
								break;
							}
						}
					}
					offset+=RecordFileHead->LEN_DATA;
					RecordFileHead++;
				}
				if(i>=n)
				{//没找到
					return 0;
				}
				i=RecordFileHead->LEN_DATA;
				if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
				{
					return 0;
				}
				ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
				p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+21;
				
			}
			result=0;
			*/
			break;
		default:
		break;			
	}
	return result;
}
void DisPlayEventInfo(u32 S_Row,u32 E_Row,u32 Addr_Row)//(第一行为0)显示事件的发生时间和结束时间 还有表的表地址u32 S_Row开始时间所在行,u32 E_Row结束时间所在行,u32 Addr_Row地址所在行
{
	u32 i;
	u64 Data;
	u8 *p8;
	u16 *p16;
	DisplayClrRow(8, 8);
	p8=(u8 *)ADDR_128KDATABUFF;
	//开始时间
	p8+=7;
	if(p8[0]==0x1c)
	{
		p8++;
	p16=(u16 *)(ADDR_STRINGBUFF+4+(S_Row)*84+6*2);
	//年
	i=(p8[0]<<8)|p8[1];
	i=hex_bcd(i);
	p16[0]=((i>>4)&0x0f)+0x30+0x2000;
	p16[1]=(i&0xf)+0x30+0x2000;
	p16[2]=0x2D+0x2000;//符号-
	//月
	i=hex_bcd(p8[2]);
	p16[3]=(i>>4)+0x30+0x2000;
	p16[4]=(i&0xf)+0x30+0x2000;
	p16[5]=0x2D+0x2000;//符号-
	//日
	i=hex_bcd(p8[3]);
	p16[6]=(i>>4)+0x30+0x2000;
	p16[7]=(i&0xf)+0x30+0x2000;
	p16[8]=0x2D+0x2000;//符号-
	//时
	i=hex_bcd(p8[4]);
	p16[9]=(i>>4)+0x30+0x2000;
	p16[10]=(i&0xf)+0x30+0x2000;
	p16[11]=0x2D+0x2000;//符号-
	//分
	i=hex_bcd(p8[5]);
	p16[12]=(i>>4)+0x30+0x2000;
	p16[13]=(i&0xf)+0x30+0x2000;
	}else
	{
		p8++;
		DisplayString(S_Row,6,0,(u8 *)"--------------");
	}
	//结束时间
	p8+=7;
	if(p8[0]==0x1c)
	{
		p8++;
	p16=(u16 *)(ADDR_STRINGBUFF+4+(E_Row)*84+6*2);
	//年
	i=(p8[0]<<8)|p8[1];
	i=hex_bcd(i);
	p16[0]=((i>>4)&0x0f)+0x30+0x2000;
	p16[1]=(i&0xf)+0x30+0x2000;
	p16[2]=0x2D+0x2000;//符号-
	//月
	i=hex_bcd(p8[2]);
	p16[3]=(i>>4)+0x30+0x2000;
	p16[4]=(i&0xf)+0x30+0x2000;
	p16[5]=0x2D+0x2000;//符号-
	//日
	i=hex_bcd(p8[3]);
	p16[6]=(i>>4)+0x30+0x2000;
	p16[7]=(i&0xf)+0x30+0x2000;
	p16[8]=0x2D+0x2000;//符号-
	//时
	i=hex_bcd(p8[4]);
	p16[9]=(i>>4)+0x30+0x2000;
	p16[10]=(i&0xf)+0x30+0x2000;
	p16[11]=0x2D+0x2000;//符号-
	//分
	i=hex_bcd(p8[5]);
	p16[12]=(i>>4)+0x30+0x2000;
	p16[13]=(i&0xf)+0x30+0x2000;
	}else
	{
		p8++;
		DisplayString(E_Row,6,0,(u8 *)"--------------");
	}
	//设备地址
	if(p8[0]==0x55)
	{			
		p8+=3;
		DisplayString(Addr_Row,0,0,(u8 *)"地址:");
		Data_Inverse((u32)p8,6);//数据倒序(高低字节调换),入口Le>=2
		Data=MRR((u32)p8,6);
		p16=(u16 *)(ADDR_STRINGBUFF+Addr_Row*84+4+6*2);
		DisplayData(Data,12,0,p16);
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_3_1_1_JS
// Descriptions:		集中器遥信状态
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_1_JS(void)
{
	u32 i;
	u32 x;
	u8 *p8;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1_1_JS);//显示菜单
			p8=(u8 *)ADDR_F203_2;
			//开关量状态,变位信息					
			if(p8[0]==1)//取得有效数据
			{
				x=p8[1];//得到开关量输入路数
				p8+=2;
				if(x==4)//集中器
				{
					//开关编号
					DisplayString(2, 6, 0, (u8 *)" 1");
					//开关状态
					p8+=3;
					if(p8[0]==1)
					{
						DisplayString(4, 6, 0, (u8 *)"合");
					}else
					{
						DisplayString(4, 6, 0, (u8 *)"分");
					}
					//开关变位
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 6, 0, (u8 *)"是");
					}else
					{
						DisplayString(6, 6, 0, (u8 *)"否");
					}
					//开关编号
					DisplayString(2, 10, 0, (u8 *)" 2");
					//开关状态
					p8+=4;
					if(p8[0]==1)
					{
						DisplayString(4, 10, 0, (u8 *)"合");
					}else
					{
						DisplayString(4, 10, 0, (u8 *)"分");
					}
					//开关变位
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 10, 0, (u8 *)"是");
					}else
					{
						DisplayString(6, 10, 0, (u8 *)"否");
					}
					//开关编号
					DisplayString(2, 14, 0, (u8 *)" 3");
					//开关状态
					p8+=4;
					if(p8[0]==1)
					{
						DisplayString(4, 14, 0, (u8 *)"合");
					}else
					{
						DisplayString(4, 14, 0, (u8 *)"分");
					}
					//开关变位
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 14, 0, (u8 *)"是");
					}else
					{
						DisplayString(6, 14, 0, (u8 *)"否");
					}
					//开关编号
					DisplayString(2, 18, 0, (u8 *)" 4");
					//开关状态
					p8+=4;
					if(p8[0]==1)
					{
						DisplayString(4, 18, 0, (u8 *)"合");
					}else
					{
						DisplayString(4, 18, 0, (u8 *)"分");
					}
					//开关变位
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 18, 0, (u8 *)"是");
					}else
					{
						DisplayString(6, 18, 0, (u8 *)"否");
					}
					//开关属性
					MR((u32)ADDR_DATABUFF,(u32)ADDR_F203_4,LENmax_F203_4);
					p8=(u8 *)ADDR_DATABUFF;
					if(p8[0]==0x02)
					{
						i=p8[7];
						//第一路开关属性
						if(i&0x80)//1是常开
						{
							DisplayString(8, 6, 0, (u8 *)"断");
						}else
						{
							DisplayString(8, 6, 0, (u8 *)"合");
						}
						i<<=1;
						//第二路开关属性
						if(i&0x80)//1是常开
						{
							DisplayString(8, 10, 0, (u8 *)"断");
						}else
						{
							DisplayString(8, 10, 0, (u8 *)"合");
						}
						i<<=1;
						//第三路开关属性
						if(i&0x80)//1是常开
						{
							DisplayString(8, 14, 0, (u8 *)"断");
						}else
						{
							DisplayString(8, 14, 0, (u8 *)"合");
						}
						i<<=1;
						//第四路开关属性
						if(i&0x80)//1是常开
						{
							DisplayString(8, 18, 0, (u8 *)"断");
						}else
						{
							DisplayString(8, 18, 0, (u8 *)"合");
						}
					}else
					{
						DisplayString(8, 6, 0, (u8 *)"没有该信息");
					}
						
					
				}
			}else
			{
				DisplayClrRow(1, 8);
				DisplayString(6, 6, 0, (u8 *)"没有有效数据");
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
// Function name:		Menu2_3_1_1_JS
// Descriptions:		终端事件信息
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_2_JS(void)
{
	u32 i;	
	u32 x;
	u32 Ln;
	u32 S_Row=4;		//事件的发生时间所在行(从第0行开始算)
	u32 E_Row=6;		//事件的结束时间所在行		(从第0行开始算)
	u32 Addr_Row=8;		//事件的电表地址所在行(从第0行开始算)
	u32 MaxScrnNum;
	Ln=1;
	MaxScrnNum=(sizeof(Lcd_EventInfo)/sizeof(Lcd_EventInfo_t));//上下翻滚有多少个屏幕要显示(对应下面switch case的有效数量)
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			DisplayMenu(0,(u8 *)pMenu2_3_1_2_1_JS);//显示菜单
			for(x=0;x<MaxScrnNum;x++)
			{
				i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
				if(i>0)//找到有效数据
				{
					Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
					break;
				}
			}
			if(x>=MaxScrnNum)//说明已经找到列表的最后一个,都没有找到数据
			{
				DisplayClrRow(2, 8);
				DisplayString(Ln+4,4,0,(u8 *)"没有事件信息");
			}else
			{
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayString(Ln+1, 6, 0, Lcd_EventInfo[x].pEventName);
				DisPlayEventInfo(S_Row,E_Row,Addr_Row);//显示事件的发生时间和结束时间 还有表的表地址
			}
			break;
		case 1://任务1:选择
			//NextScreenSelect(MaxScrnNum);//按键翻屏操作选择,MaxScrnNum最大有多少个屏幕,Stack->MenuStack[MenuCount->Count].MoveRow从0开始算
			if(Comm_Ram->ButtonVal&0x01)//下移
			{
				Comm_Ram->ButtonVal&=~0x03;//清楚键位
				x=Stack->MenuStack[MenuCount->Count].MoveRow;//保存当前屏幕的下标数(便于后面按键搜索用)
				if((x+1)<MaxScrnNum)//x肯定比MaxScrnNum小(MaxScrnNum最大屏幕数)
				{
					x++;
					for(;x<MaxScrnNum;x++)
					{
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
						if(i>0)//找到有效数据
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
							break;
						}
					}
					if(x>=MaxScrnNum)//没有有效数据
					{
						MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//保存当前屏幕的下标数(便于后面按键搜索用)
						x=0;
						for(;x<MaxScrnNum;x++)
						{
							i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
							if(i>0)//找到有效数据
							{
								Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
								break;
							}
						}
						if(x>=MaxScrnNum)
						{
							return ;//没有新的数据画面没有变化
						}
					}
				}else
				{
					MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//保存当前屏幕的下标数(便于后面按键搜索用)
					x=0;
					for(;x<MaxScrnNum;x++)
					{
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
						if(i>0)//找到有效数据
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
							break;
						}
					}
					if(x>=MaxScrnNum)
					{
						return ;//没有新的数据画面没有变化
					}
				}
				DisplayString(Ln+1, 6, 0, Lcd_EventInfo[x].pEventName);
				DisPlayEventInfo(S_Row,E_Row,Addr_Row);//显示事件的发生时间和结束时间 还有表的表地址
			}			
			if(Comm_Ram->ButtonVal&0x02)//上移
			{
				Comm_Ram->ButtonVal&=~0x03;//清楚键位
				x=Stack->MenuStack[MenuCount->Count].MoveRow;//保存当前屏幕的下标数(便于后面按键搜索用)
				if(x!=0)//如果当前显示的不是第一个事件
				{
					do
					{
						x--;//跳过当前事件搜索下标
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
						if(i>0)//找到有效数据
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
							break;
						}
					}while(x!=0);
					if((x==0)&&(i==0))//没有数据(从最大的下标数字开始往前搜)
					{
						x=MaxScrnNum;//最大屏幕数
						x--;//最大屏幕下标数
						MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//保存当前屏幕的下标数(便于后面按键搜索用)
						while(x>MaxScrnNum)
						{
							i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表				
							if(i>0)//找到有效数据
							{
								Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
								break;
							}
							x--;//最大屏幕下标
						}
						if((i==0)&&(x==MaxScrnNum))//没有搜到有效数据
						{
							return;//没有新的数据画面没有变化
						}
					}
				}else//当面屏幕正好是第一个屏幕
				{
					x=MaxScrnNum;//最大屏幕数
					x--;//最大屏幕下标数
					MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//保存当前屏幕的下标数(便于后面按键搜索用)
					while(x!=MaxScrnNum)
					{
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表				
						if(i>0)//找到有效数据
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//保存当前屏幕的下标数(便于后面按键搜索用)
							break;
						}
						x--;//最大屏幕下标
					}
					if((i==0)&&(x==MaxScrnNum))//没有搜到有效数据
					{
						return;//没有新的数据画面没有变化
					}
				}
				DisplayString(Ln+1, 6, 0, Lcd_EventInfo[x].pEventName);
				DisPlayEventInfo(S_Row,E_Row,Addr_Row);//显示事件的发生时间和结束时间 还有表的表地址
			}
			break;
		default:
			break;
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x33;//键使用后清0

}
//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_3_1_2_JS
// Descriptions:		电表事件信息
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
/*
void Menu2_3_1_2_2_JS(void)
{
	u8 *p8;
	u16 *p16;
	u16 *pAddr;
	u32 i;
	u32 Ln;
	u64 Data;
	u32 MaxScrnNum=8;//上下翻滚有多少个屏幕要显示(对应下面switch case的有效数量)
	Ln=1;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(0,(u8 *)pMenu2_3_1_2_2_JS);//显示菜单
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=NUM_RMmax-1;
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

			//通信地址
			p8=(u8*)ADDR_DATABUFF+1024;
			MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v2,LENper_6000);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,1,LENper_6000);
			if(p8)
			{
				MR(ADDR_DATABUFF,(u32)p8+3,6);
				p16=(u16 *)ADDR_DATABUFF;
				pAddr=(u16 *)ADDR_AddrList;
				for(i=0;i<3;i++)
				{
					pAddr[i]=p16[i];
				}
				Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
				Data=MRR(ADDR_DATABUFF,6);
				p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+6*2);
				DisplayData(Data,12,0,p16);
			}else
			{
				DisplayString(Ln+5,6,0,(u8 *)"------------");
				pAddr=0;
			}
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
			NextScreenSelect(MaxScrnNum);//按键翻屏操作选择,MaxScrnNum最大有多少个屏幕,Stack->MenuStack[MenuCount->Count].MoveRow从0开始算
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
	switch(Stack->MenuStack[MenuCount->Count].MoveRow)//菜单任务计数
	{
		case 0://电表数据变更
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  电表数据变更");
			i=Lcd_Get_EventData(pAddr,0x311C0200+1,(u8*)ADDR_128KDATABUFF,1);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			break;
		case 1://电能表示度下降
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  电表示度下降");
			i=Lcd_Get_EventData(pAddr,0x310B0200+1,(u8*)ADDR_128KDATABUFF,1);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			break;
		case 2://电能量超差
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  电能量超差");
			i=Lcd_Get_EventData(pAddr,0x310C0200+1,(u8*)ADDR_128KDATABUFF,1);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			break;
		case 4://电能表飞走
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  电能表飞走");
			i=Lcd_Get_EventData(pAddr,0x310D0200+1,(u8*)ADDR_128KDATABUFF,1);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			break;		
		case 5://电能表停走
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  电能表停走");
			i=Lcd_Get_EventData(pAddr,0x310E0200+1,(u8*)ADDR_128KDATABUFF,1);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			break;
		case 6://时间超差
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  时间超差");
			i=Lcd_Get_EventData(pAddr,0x31050200+1,(u8*)ADDR_128KDATABUFF,0);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			break;
		case 7://485抄表失败
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"类型:  485抄表失败");
			//i=Lcd_Get_EventData(pAddr,0x310A0200+1,(u8*)ADDR_128KDATABUFF,1);//Get最近一次事件基本信息 pOut传出事件发生结束时间,type 0终端1电表
			i=1;
			break;
		default:
		break;
	}
	if(i==0)//取得有效数据
	{
		p8=(u8 *)ADDR_128KDATABUFF;
		p8+=7;
		if(p8[0]==0x1c)
		{
			p8++;
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
		//年
		i=(p8[0]<<8)|p8[1];
		i=hex_bcd(i);
		p16[0]=((i>>4)&0x0f)+0x30+0x2000;
		p16[1]=(i&0xf)+0x30+0x2000;
		p16[2]=0x2D+0x2000;//符号-
		//月
		i=hex_bcd(p8[2]);
		p16[3]=(i>>4)+0x30+0x2000;
		p16[4]=(i&0xf)+0x30+0x2000;
		p16[5]=0x2D+0x2000;//符号-
		//日
		i=hex_bcd(p8[3]);
		p16[6]=(i>>4)+0x30+0x2000;
		p16[7]=(i&0xf)+0x30+0x2000;
		p16[8]=0x2D+0x2000;//符号-
		//时
		i=hex_bcd(p8[4]);
		p16[9]=(i>>4)+0x30+0x2000;
		p16[10]=(i&0xf)+0x30+0x2000;
		p16[11]=0x2D+0x2000;//符号-
		//分
		i=hex_bcd(p8[5]);
		p16[12]=(i>>4)+0x30+0x2000;
		p16[13]=(i&0xf)+0x30+0x2000;
		}else
		{
			DisplayString(Ln+3,6,0,(u8 *)"--------------");
		}
		p8+=7;
		if(p8[0]==0x1c)
		{
			p8++;
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
		//年
		i=(p8[0]<<8)|p8[1];
		i=hex_bcd(i);
		p16[0]=((i>>4)&0x0f)+0x30+0x2000;
		p16[1]=(i&0xf)+0x30+0x2000;
		p16[2]=0x2D+0x2000;//符号-
		//月
		i=hex_bcd(p8[2]);
		p16[3]=(i>>4)+0x30+0x2000;
		p16[4]=(i&0xf)+0x30+0x2000;
		p16[5]=0x2D+0x2000;//符号-
		//日
		i=hex_bcd(p8[3]);
		p16[6]=(i>>4)+0x30+0x2000;
		p16[7]=(i&0xf)+0x30+0x2000;
		p16[8]=0x2D+0x2000;//符号-
		//时
		i=hex_bcd(p8[4]);
		p16[9]=(i>>4)+0x30+0x2000;
		p16[10]=(i&0xf)+0x30+0x2000;
		p16[11]=0x2D+0x2000;//符号-
		//分
		i=hex_bcd(p8[5]);
		p16[12]=(i>>4)+0x30+0x2000;
		p16[13]=(i&0xf)+0x30+0x2000;
		}else
		{
			DisplayString(Ln+4,6,0,(u8 *)"--------------");
		}
	}else
	{
		DisplayString(Ln+3,6,0,(u8 *)"--------------");
		DisplayString(Ln+4,6,0,(u8 *)"--------------");
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_3_1_1_JS
// Descriptions:		事件信息
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_2_JS(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1_2_JS);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. 遥信状态
						MenuCount->v1=0;
						MenuCreate((u32)Menu2_3_1_2_1_JS);//创建下级菜单
						break;
					case 2://2. 事件信息
						MenuCount->v2=1;
						MenuCreate((u32)Menu2_3_1_2_2_JS);//创建下级菜单
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
*/
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_1_JS
// Descriptions:        集中器数据
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_JS(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1_JS);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. 遥信状态
						MenuCount->v1=0;//保证进入一次对Flash操作一次
						MenuCreate((u32)Menu2_3_1_1_JS);//创建下级菜单
						break;
					case 2://2. 事件信息
						MenuCreate((u32)Menu2_3_1_2_JS);//创建下级菜单
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
// Function name:       Menu2_3_2_JS
// Descriptions:        终端信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_2_JS(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 MaxScrnNum=2;
	NextScreenSelect(MaxScrnNum);//按键翻屏操作选择,MaxScrnNum最大有多少个屏幕
	switch (Stack->MenuStack[MenuCount->Count].MoveRow)
	{
		case 0:
			DisplayMenu(0,(u8 *)pMenu2_3_1);//显示菜单
			//1.厂商代码
			//1.软件版本
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+9*2);
			p16[0]=0x2000+'V';
			p16[1]=(SVER>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((SVER>>4)&0xf)+0x2030;
			p16[4]=((SVER>>0)&0xf)+0x2030;

			//2.软件版本日期
			p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+5*2);
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
			p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+9*2);
			p16[0]=0x2000+'V';
			p16[1]=(HVER>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((HVER>>4)&0xf)+0x2030;
			p16[4]=((HVER>>0)&0xf)+0x2030;
			//4.硬件版本日期
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+5*2);
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
			break;
		case 1:
			DisplayMenu(0,(u8 *)pMenu2_3_2_JS);//显示菜单
			//软件版本+版本日期
			p8=(u8*)ADDR_F209_2;
			p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+4*2);
			i=p8[29];
			//i=hex_bcd(i);
			p16[0]=((i>>4)&0xf)+0x2000+0x30;
			p16[1]=(i&0xf)+0x2000+0x30;
			p16[2]='.'+0x2000;
			i=p8[30];
			//i=hex_bcd(i);
			p16[3]=((i>>4)&0xf)+0x2000+0x30;
			p16[4]=(i&0xf)+0x2000+0x30;
			p16[5]=' '+0x2000;
			//年
			i=(p8[23]<<8)|p8[24];
			i=hex_bcd(i);
			p16[6]=((i>>12)&0xf)+0x2000+0x30;
			p16[7]=((i>>8)&0xf)+0x2000+0x30;
			p16[8]=((i>>4)&0xf)+0x2000+0x30;
			p16[9]=(i&0xf)+0x2000+0x30;
			p16[10]='-'+0x2000;
			//月
			i=p8[25];
			i=hex_bcd(i);
			p16[11]=((i>>4)&0xf)+0x2000+0x30;
			p16[12]=(i&0xf)+0x2000+0x30;
			p16[13]='-'+0x2000;
			//日
			i=p8[26];
			i=hex_bcd(i);
			p16[14]=((i>>4)&0xf)+0x2000+0x30;
			p16[15]=(i&0xf)+0x2000+0x30;
			//厂商代码
			p16=(u16 *)(ADDR_STRINGBUFF+4+4*84+10*2);
			i=p8[16];
			p16[0]=(i&0xff)+0x2000;
			i=p8[17];
			p16[1]=(i&0xff)+0x2000;
			break;
		default:
		break;
	}
	UpArrow();//向上箭头
	DownArrow();//向下箭头	
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_3_JS
// Descriptions:        终端管理
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_3_JS(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_3_JS);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://2. 现场调试
						MenuCreate((u32)Menu2_3_2);//创建下级菜单
						break;
					case 2://3. 重启终端
						MenuCreate((u32)Menu2_3_3);//创建下级菜单
						break;
					case 3://4. 数据区初始化
						MenuCreate((u32)Menu2_3_4);//创建下级菜单
						break;
					case 4://5参数初始化
						MenuCreate((u32)Menu2_3_5);//创建下级菜单
						break;
					case 5://6集中器数据
						MenuCreate((u32)Menu2_3_6);//创建下级菜单
						break;
					case 6://7载波抄表管理
						MenuCreate((u32)Menu2_3_7);//创建下级菜单
						break;
					case 7://USB功能
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
// Function name:       Menu2_3_JS
// Descriptions:        集中器终端管理与维护
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_3_JS(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_JS);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. 集中器数据
						MenuCreate((u32)Menu2_3_1_JS);//创建下级菜单
						break;
					case 2://2. 集中器信息
						MenuCreate((u32)Menu2_3_2_JS);//创建下级菜单
						break;
					case 3://3. 集中器管理
						MenuCreate((u32)Menu2_3_3_JS);//创建下级菜单
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

