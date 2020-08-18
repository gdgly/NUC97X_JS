
//主菜单
#include "Project698.h"
#include "Menu.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
const u8 SList[][2]=//正负极性列表
{
	" ",//+不显示
	"-",
};
const u8 UnitList[][6]=//通用单位列表
{
	"kW",    //0
	"kvar",  //1
	"kWh",   //2
	"kvarh", //3
	"V",     //4
	"A",     //5
	"%",     //6
	"h",     //7
	"min",   //8
	"s",     //9
	"次",    //10
};
const u8 ProtocolList[][13]=//通信协议列表
{
	"未知    ",//0 
	"645-1997",//1 DL/T 645-1997
	"645-2007",//2 DL/T 645-2007	
	"698.45  ",//3 DL/T698.45
	"188-2004",//4 CJ/T 188-2004
	"ABB     ",//5 ABB
	"IEC1107 ",//6 兰吉尔
};

void _0(void)
{
}

#if (USER/100)==17 //江苏
const u8 pE2_Main2[]=//集中器主菜单
{
	"\n\r"
	"   测量点数据显示\n\r"
	"   参数设置与查看\n\r"
	"   终端管理与维护\n\r"
};

#else
const u8 pE2_Main2[]=//集中器主菜单
{
	"主菜单\n\r"
	"   测量点数据显示\n\r"
	"   参数设置与查看\n\r"
	"   终端管理与维护\n\r"
};
#endif
const u8 pE2_Main3[]=//专变主菜单
{
	"主菜单\n\r"
	"  1.实时数据\n\r"
	"  2.参数定值\n\r"
	"  3.控制状态\n\r"
	"  4.电能表示数\n\r"
	"  5.中文信息\n\r"
	"  6.购电信息\n\r"
	"  7.终端信息\n\r"
	"  8.终端管理与维护\n\r"
};


//-------------------------------------------------------------------------------------------------
// Function name:       PassWord
// Descriptions:        输入密码
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------
void Menu2_2_1(void);
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
	#if(USER/100)==17
	u32 Flag;
	Flag=0;
	#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(LnTitle,ColTitle,0,(u8*)"输入密码");
			DisplayString(Ln,Col,0,(u8*)"0*****>");
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
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
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
					i=Stack->MenuStack[MenuCount->Count].EditCol-7;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
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
			#if(USER/100)==17 //江苏省
			i=Stack->MenuStack[MenuCount->Count].EditRow;
			if(i>0)
			{
				i--;
				Stack->MenuStack[MenuCount->Count].EditRow=i;
				Flag=1;
			}
			#endif
			UpDownSelect();
			#if(USER/100)==17 //江苏省
			if(Flag!=0)
			{
				i=Stack->MenuStack[MenuCount->Count].EditRow;
				i++;
				Stack->MenuStack[MenuCount->Count].EditRow=i;
			}
			#endif
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
		case 2://任务2：判断密码是否正确，正确则跳转到菜单8，错误提示对话框
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_MenuKey,3);
			if(((p32[0]&0xffffff)==(MenuCount->v1&0xffffff))||((PWD_1==(MenuCount->v1&0xffffff))))
			{//比较输入的密码与正确的密码是否相等
				#if ((Project/100)==2)//集中器
//					#if (USER/100)==0//电科院测试
//					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==1)
//					{
//						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2_1;
//					}
//					#else
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==2)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
					}
//					#endif
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
					{
						#if (USER/100)==17 //江苏
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3_JS;//创建下级菜单 3.终端管理与维护
						#else
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
						#endif
					}
					else
					{
						MenuDel();//返回上级菜单	    
					}
				#endif
				#if ((Project/100)==3)//专变
					i=Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow;
					i=MenuCount->Count;
					if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==3))
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
					}
					else if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==2))
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
					}
					else
					{
						MenuDel();//返回上级菜单	    
					}
				#endif
				#if ((Project/100)==4)//三合一
					if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
					{
						if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==2)
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
						}
						else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
						}
						else
						{
							MenuDel();//返回上级菜单	    
						}
					}
					else
					{
						i=Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow;
						i=MenuCount->Count;
						if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==3))
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
						}
						else if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==2))
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
						}
						else
						{
							MenuDel();//返回上级菜单	    
						}
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
				DisplayString(4,6,0,(u8*)"密码错误");
				DisplayString(5,3,0,(u8*)"请按返回键返回");
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
// Function name:       E2_Main
// Descriptions:        主菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_Main(void)//主菜单
{
	DOTMIXPMODE->Mix=2;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
	Draw_Clr();
	#if ((Project/100)==2)//集中器
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main2);//显示菜单
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
							#if (USER/100)==17 //江苏
							MenuCreate((u32)Menu_4_JS);//创建下级菜单  1.电能表数据显示
							#else
							MenuCreate((u32)Menu_4);//创建下级菜单  1.电能表数据显示
							#endif
							break;
						case 2:
//						#if (USER/100)==0//电科院测试
//							MenuCreate((u32)Menu2_2);//创建下级菜单 2.参数设置与查看
//							break;
//						#endif
						case 3:
							//非工厂模式输入正确密码，方可进入2、3子菜单
							//工厂模式下，随时可进入2、3子菜单
							if(Comm_Ram->Factory==0x55)
							{//工厂状态
								if(Stack->MenuStack[MenuCount->Count].EditRow==2)
								{
									MenuCreate((u32)Menu2_2);//创建下级菜单 2.参数设置与查看
								}
								if(Stack->MenuStack[MenuCount->Count].EditRow==3)
								{
									#if (USER/100)==17 //江苏
									MenuCreate((u32)Menu2_3_JS);//创建下级菜单 3.终端管理与维护
									#else
									MenuCreate((u32)Menu2_3);//创建下级菜单 3.终端管理与维护
									#endif
								
								}
							}
							else
							{
								PassWordCreateEdit();//创建密码编辑框
							}
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
	#endif
	#if ((Project/100)==3)//专变终端
			switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
			{
				case 0://任务0:初始化
					Stack->MenuStack[MenuCount->Count].Task=1;
					DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main3);//显示菜单
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
								MenuCreate((u32)Menu_1);//创建下级菜单
								break;
							case 2:
								MenuCreate((u32)Menu_2);//创建下级菜单
								break;
							case 3:
								MenuCreate((u32)Menu_3);//创建下级菜单
								break;
							case 4:
								MenuCreate((u32)Menu_4);//创建下级菜单
								break;
							case 5:
								MenuCreate((u32)Menu_5);//创建下级菜单
								break;
							case 6:
								MenuCreate((u32)Menu_6);
								break;
							case 7:
								MenuCreate((u32)Menu_7);//创建下级菜单
								break;
							case 8:
								//按下编程键后、输入正确密码，方可进入设置菜单
								//工厂模式下，随时可进入设置菜单
								if(Comm_Ram->Factory==0x55)
								{//工厂状态
									MenuCreate((u32)Menu2_3);//创建下级菜单
								}
								else
								{
									PassWordCreateEdit();
								}
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
	#endif
	#if ((Project/100)==4)//三合一终端
		if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
		{
			switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
			{
				case 0://任务0:初始化
					Stack->MenuStack[MenuCount->Count].Task=1;
					DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main2);//显示菜单
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
								MenuCreate((u32)Menu_4);//创建下级菜单  1.电能表数据显示
								break;
							case 2:
							case 3:
								//非工厂模式输入正确密码，方可进入2、3子菜单
								//工厂模式下，随时可进入2、3子菜单
								if(Comm_Ram->Factory==0x55)
								{//工厂状态
									if(Stack->MenuStack[MenuCount->Count].EditRow==2)
									{
										MenuCreate((u32)Menu2_2);//创建下级菜单 2.参数设置与查看
									}
									if(Stack->MenuStack[MenuCount->Count].EditRow==3)
									{
										MenuCreate((u32)Menu2_3);//创建下级菜单 3.终端管理与维护
									}
								}
								else
								{
									PassWordCreateEdit();//创建密码编辑框
								}
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
		else
		{
			switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
			{
				case 0://任务0:初始化
					Stack->MenuStack[MenuCount->Count].Task=1;
					DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main3);//显示菜单
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
								MenuCreate((u32)Menu_1);//创建下级菜单
								break;
							case 2:
								MenuCreate((u32)Menu_2);//创建下级菜单
								break;
							case 3:
								MenuCreate((u32)Menu_3);//创建下级菜单
								break;
							case 4:
								MenuCreate((u32)Menu_4);//创建下级菜单
								break;
							case 5:
								MenuCreate((u32)Menu_5);//创建下级菜单
								break;
							case 6:
								MenuCreate((u32)Menu_6);
								break;
							case 7:
								MenuCreate((u32)Menu_7);//创建下级菜单
								break;
							case 8:
								//按下编程键后、输入正确密码，方可进入设置菜单
								//工厂模式下，随时可进入设置菜单
								if(Comm_Ram->Factory==0x55)
								{//工厂状态
									MenuCreate((u32)Menu2_3);//创建下级菜单
								}
								else
								{
									PassWordCreateEdit();
								}
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
}


/*
void DisplayMenu(u32 MoveRow,u8 * pStr)//显示菜单
---------------void MenuCreate(u32 FunctionAddr)//创建下级菜单------------
---------------void MenuDel(void)//返回上级菜单---------------------------
void MenuSelect(void)//菜单选择

void DisplayClr(void)//清全屏
void DisplayClrRow(u32 StartRow,u32 EndRow)//清屏幕显示的指定行
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示字符串
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr)//显示字符串仅在1行内
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//数据显示(调试用),入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF

void MR(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//存储器读
void MEMWrite(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//存储器写
void MEMClr(u8 Data,u32 ADDR_FLASH,u32 Byte);//存储器清除
u64 MEMReadReg(u32 ADDR_FLASH,u8 Byte);//存储器读到寄存器(<=8字节)
void MEMWriteReg(u64 Data,u32 ADDR_FLASH,u8 Byte);//寄存器写到存储器(<=8字节)


//读写数据缓冲(定义在最后和堆栈相连)
#define LEN_DATABUFF     4096
#define ADDR_DATABUFF     (ADDR_Terminal_Ram_Start+LEN_Terminal_Ram)
//保留字对齐的4096字节



Comm_Ram->ButtonVal//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=
Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器

typedef __packed struct
{
  u8 Count;//菜单堆栈计数
  u8 Res1;
  u8 Res2;
  u8 Res3;
  u32 v1;//菜单用变量1
  u32 v2;//菜单用变量2
  u32 v3;//菜单用变量3
}MenuCount_TypeDef;
#define MenuCount     ((MenuCount_TypeDef *)(ADDR_MenuStack))
#define MaxMenuCount     10//最大菜单级数
typedef __packed struct
{
  u32 PC;//菜单任务运行地址(特别注意:此值不可随便改动)
  u8 Task;//菜单任务计数(进入和退出时均清为0)
  u8 MoveRow;//菜单上移条数(进入时总为0,返回时不变)
  u8 EditRow;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
  u8 EditCol;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
  u8 TotalRow;//菜单总条数
  u8 MenuRow;//菜单当前装入条数
  u8 res2;
  u8 res3;
}MenuSave_TypeDef;
typedef __packed struct
{
  MenuSave_TypeDef     MenuStack[MaxMenuCount+1];
}MenuStack_TypeDef;
#define Stack     ((MenuStack_TypeDef *)(ADDR_MenuStack+16))



typedef __packed struct
{
  u8 DotLn;//与上行间距点数(0-255)
  u8 Row;//字符串号(0-9)(1个字符串在屏上可分多行显示)
  u8 Font;//本行字体号0-4
  u8 Align;//对齐方式,0=左对齐,1=居中,2=右对齐
  u16 Code[40];//显示编码
               //显示编码每二字节代表8*16半字,每行显示缓冲80字节40个半字
               //D15=1表示反显
              //B14=0B13=0从自建小字库取点阵,最大编号空间0-0X2000=8192
              //B14=0B13=1,B12-B0=0-255从ASCII8*16标准字库取点阵,最大编号空间=256
              //B14=0B13=1,B12-B0=256-511从ASCII8*12标准取点阵(暂不用),最大编号空间=256
              //B14=0B13=1,B12-B0=512-767从ASCII8*8标准取点阵(暂不用),最大编号空间=256
              //B14=1B13-B0表示从GB2312标准字库(修改后)取点阵,最大编号空间0-0X4000
}StrLnBuff_TypeDef;
typedef __packed struct
{
  StrLnBuff_TypeDef Row[16];
}StrBuff_TypeDef;
#define StrBuff     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF))


*/

/*
菜单中设置键判断用如下：
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	if(MEMReadReg(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
#else
	if(MEMReadReg(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
#endif

  	if(Comm_Ram->Factory=0x55)//工厂状态
	{
		p16[0]=25;// '工厂模式'
		p16[1]=26;
	}
	//在工厂模式下，设置键不论如何总是有效的。

*/
/*
王造中 13:57:04
  Terminal_Ram->AFN04F10UpDate=0;//0=AFN04F10终端电能表/交流采样装置配置参数更新标志,!=0没更新
王造中 14:33:48
  Terminal_Ram->AFN04F10F11F13UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
王造中 14:34:30
  原Terminal_Ram->AFN04F10UpDate不用了。
王造中 10:47:13
  #define IC_MAC     0//0=没,1=有
王造中 18:50:41
  IC_MAC已改为IC_24AAxxx
*/
/*
Comm_Ram->RouterType//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
(u32)&Terminal_Router->RouterAddr
typedef __packed struct
{
 u8 RouterAddr[6];//路由主节点地址(总是放在最前,通信库地址定位用)
 u8 RouterCODE[4];//厂商代码 ASCII 2,芯片代码 ASCII 2
 u8 RouterVer[5];//路由器版本号,分别为:日,月,年,2BYTE版本
*/






